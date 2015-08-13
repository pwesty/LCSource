// unzip.cpp : Defines the entry point for the console application.
//

#include "stdh.h"
#include <Engine/Base/Stream.h>
#include <Engine/Base/FileName.h>
#include <Engine/Base/Translation.h>
#include <Engine/Base/ErrorReporting.h>
#include <Engine/Base/Console.h>
#include <Engine/Base/Synchronization.h>
#include <Engine/Math/Functions.h>
#include <Engine/Base/ListIterator.inl>
#include <Engine/Base/MemoryTracking.h>

#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Templates/StaticStackArray.cpp>
#include <Engine/Templates/DynamicStackArray.cpp>

#include <3rdparty/zlib-1.2.8/zlib.h>
extern CTCriticalSection zip_csLock; // critical section for access to zlib functions
extern INDEX fil_iReportStats;

// runing under NT kernel ?
extern BOOL _bNTKernel;

static INDEX zip_iBufferSectors = 10;
static INDEX zip_iPrefetchSectors = 1;
static SLONG zip_slSectorSize = 32*1024;

#pragma pack(1)

#define ZEF_STORED        (1UL<<0) // is zip entry only stored
#define ZEF_MOD           (1UL<<1) // is zip entry for mod
#define ZEF_DATAISHEADER  (1UL<<2) // is data pointing to file header or file data

#define DEBUG_UNZIP 0

// before each file in the zip
#define SIGNATURE_LFH 0x04034b50
struct LocalFileHeader {
  SWORD lfh_swVersionToExtract;
  SWORD lfh_swGPBFlag;
  SWORD lfh_swCompressionMethod;
  SWORD lfh_swModFileTime;
  SWORD lfh_swModFileDate;
  SLONG lfh_slCRC32;
  SLONG lfh_slCompressedSize;
  SLONG lfh_slUncompressedSize;
  SWORD lfh_swFileNameLen;
  SWORD lfh_swExtraFieldLen;

// follows:
//	filename (variable size)
//  extra field (variable size)
};
  
// after file data, only if compressed from a non-seekable stream
// this exists only if bit 3 in GPB flag is set
#define SIGNATURE_DD 0x08074b50
struct DataDescriptor {
	SLONG dd_slCRC32;
	SLONG dd_slCompressedSize;
	SLONG dd_slUncompressedSize;
};

// one file in central dir
#define SIGNATURE_FH 0x02014b50
struct FileHeader {
  SWORD fh_swVersionMadeBy;
  SWORD fh_swVersionToExtract;
  SWORD fh_swGPBFlag;
  SWORD fh_swCompressionMethod;
  SWORD fh_swModFileTime;
  SWORD fh_swModFileDate;
  SLONG fh_slCRC32;
  SLONG fh_slCompressedSize;
  SLONG fh_slUncompressedSize;
  SWORD fh_swFileNameLen;
  SWORD fh_swExtraFieldLen;
  SWORD fh_swFileCommentLen;
  SWORD fh_swDiskNoStart;
  SWORD fh_swInternalFileAttributes;
  SLONG fh_swExternalFileAttributes;
  SLONG fh_slLocalHeaderOffset;

// follows:
//  filename (variable size)
//  extra field (variable size)
//  file comment (variable size)
};

// at the end of entire zip file
#define SIGNATURE_EOD 0x06054b50
struct EndOfDir {
  SWORD eod_swDiskNo;
  SWORD eod_swDirStartDiskNo;
  SWORD eod_swEntriesInDirOnThisDisk;
  SWORD eod_swEntriesInDir;
  SLONG eod_slSizeOfDir;
  SLONG eod_slDirOffsetInFile;
  SWORD eod_swCommentLenght;
// follows: 
//  zipfile comment (variable size)
};

#pragma pack()

// an archive
class CZipArchive {
public:
  CTFileName za_fnm;  // file name
  int za_fFile;       // file handle
  SLONG za_slSize;    // archive file size

  void Clear(void) {};

  // open the archive for later reading
  void Open_t(void);
  // read from a given position in the archive (uses read-ahead buffering)
  SLONG Read_t(void *pvBuffer, SLONG slOffset, SLONG slSize);
  // read ahead from a given position in the archive (uses read-ahead buffering)
  void ReadAhead_t(SLONG slOffset, SLONG slSize);
};

// zip archive buffer for one sector
class CZipSector {
public:
  CZipArchive *zs_pzaArchive; // archive that this sector belongs to
  SLONG zs_slOffset;      // offset of the sector in the archive (<0 if buffer is not used)
  UBYTE *zs_pubBuffer;    // the sector contents
  OVERLAPPED zs_ol;       // for async reading
  HANDLE zs_hEvent;
  BOOL zs_bBusy;          // set when reading starts, reset when finished and got the results
  CListNode zs_lnInLRU;   // for linking in a LRU list

  CZipSector(void);
  ~CZipSector(void);
  void Clear(void);
  void CreateBuffer(SLONG slSectorSize);
};

class CZipBuffer {
public:
  // for buffering
  SLONG zb_slSectorSize;
  CStaticArray<CZipSector> zb_azsBuffered;
  CListHead zb_lhBuffersLRU;  // LRU list of buffers

  // internal reading functions used for buffering
  void MarkSectorTouched(INDEX iSector);
  INDEX RequestReadSector_t(CZipArchive *pza, SLONG slOffset, BOOL bReadAhead=FALSE);
  void WaitSectorRead_t(INDEX iSector);
  BOOL IsSectorUsed(INDEX iSector);

  void InitBuffers(void);
  void ClearBuffers(void);
};

// one entry (a zipped file) in a zip archive
class CZipEntry {
public:
  CZipArchive *ze_pza;          // the archive
  CTFileName ze_fnm;            // file name with path inside archive
  SLONG ze_slCompressedSize;    // size of file in the archive
  SLONG ze_slUncompressedSize;  // size when uncompressed
  SLONG ze_slDataOffset;        // position of compressed data inside archive (may point to file header. Check flags)
  ULONG ze_ulCRC;               // checksum of the file
  ULONG ze_ulFlags;             // zip entry flags

  void Clear(void)
  {
    ze_pza = NULL;
    ze_fnm.Clear();
  }
};

// an open instance of a file inside a zip
class CZipHandle {
public:
  BOOL zh_bOpen;          // set if the handle is used
  CZipEntry zh_zeEntry;   // the entry itself
  z_stream zh_zstream;    // zlib filestream for decompression
  SLONG zh_slPosition;    // current file position inside archive file
#define BUF_SIZE  1024
  UBYTE *zh_pubBufIn;     // input buffer

  CZipHandle(void);
  void Clear(void);
  void ThrowZLIBError_t(int ierr, const CTString &strDescription);
  // read more data into the buffer
  SLONG ReadMoreData(void);
};

// all files in all active zip archives
static CStaticStackArray<CZipEntry>  _azeFiles;
// handles for currently open files
static CStaticStackArray<CZipHandle> _azhHandles;
// all zip archives
static CDynamicStackArray<CZipArchive> _azaArchives;
// the zip buffer
static CZipBuffer _zbBuffer;

// get error string for a zlib error
CTString GetZlibError(int ierr)
{
  switch(ierr) {
  case Z_OK           : return TRANS("Z_OK           "); break;
  case Z_STREAM_END   : return TRANS("Z_STREAM_END   "); break;   
  case Z_NEED_DICT    : return TRANS("Z_NEED_DICT    "); break;
  case Z_STREAM_ERROR : return TRANS("Z_STREAM_ERROR "); break;
  case Z_DATA_ERROR   : return TRANS("Z_DATA_ERROR   "); break;
  case Z_MEM_ERROR    : return TRANS("Z_MEM_ERROR    "); break; 
  case Z_BUF_ERROR    : return TRANS("Z_BUF_ERROR    "); break;
  case Z_VERSION_ERROR: return TRANS("Z_VERSION_ERROR"); break;
  case Z_ERRNO        : {
    CTString strError;
    strError.PrintF(TRANS("Z_ERRNO: %s"), strerror(errno));
    return strError; 
                        } break;
  default: {
    CTString strError;
    strError.PrintF(TRANS("Unknown ZLIB error: %d"), ierr);
    return strError; 
           } break;
  }
}

CZipSector::CZipSector(void)
{
  zs_pubBuffer = NULL;
  zs_pzaArchive = NULL;
  zs_slOffset = -1;
  zs_bBusy = FALSE;
}

CZipSector::~CZipSector(void)
{
  Clear();
}

void CZipSector::Clear(void)
{

  if (zs_pubBuffer!=NULL) {
    FreeMemory(zs_pubBuffer);
    zs_pubBuffer = NULL;
  }
  zs_pzaArchive = NULL;
  zs_slOffset = -1;
  zs_bBusy = FALSE;
}

void CZipSector::CreateBuffer(SLONG slSectorSize)
{
  zs_pubBuffer = (UBYTE*)AllocMemory(slSectorSize);
  zs_pzaArchive = NULL;
  zs_slOffset = -slSectorSize*100;
  zs_bBusy = FALSE;
  zs_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

void CZipBuffer::InitBuffers(void)
{
  zb_slSectorSize = zip_slSectorSize;
  zb_azsBuffered.New(zip_iBufferSectors);
  for(INDEX iSector=0; iSector<zb_azsBuffered.Count(); iSector++) {
    zb_azsBuffered[iSector].CreateBuffer(zb_slSectorSize);
    zb_lhBuffersLRU.AddTail(zb_azsBuffered[iSector].zs_lnInLRU);
  }
}

void CZipBuffer::ClearBuffers(void)
{
  // wait all reads to finish
  {for (INDEX iSector=0; iSector<zb_azsBuffered.Count(); iSector++) {
    WaitSectorRead_t(iSector);
  }}
  // free old buffers
  zb_azsBuffered.Clear();
}

// open the archive for later reading
void CZipArchive::Open_t(void)
{
  // determine flags
  DWORD dwFlags = FILE_ATTRIBUTE_NORMAL|FILE_FLAG_NO_BUFFERING;
  if( _bNTKernel) dwFlags |= FILE_FLAG_OVERLAPPED; // async read only supported on NT kernels

  // open the file
  za_fFile = (int)CreateFile(za_fnm, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, dwFlags, NULL);
  
  if (za_fFile==NULL) {
    ThrowF_t(TRANS("%s: Cannot open file (%s)"), (CTString&)za_fnm, strerror(errno));
  }

  // query its size
  za_slSize = GetFileSize((HANDLE)za_fFile, NULL);
}

// internal reading functions used for buffering
void CZipBuffer::MarkSectorTouched(INDEX iSector)
{
  zb_azsBuffered[iSector].zs_lnInLRU.Remove();
  zb_lhBuffersLRU.AddTail(zb_azsBuffered[iSector].zs_lnInLRU);
}


static BOOL _bPreReading = FALSE;
INDEX CZipBuffer::RequestReadSector_t(CZipArchive *pza, SLONG slOffset, BOOL bReadAhead/*=FALSE*/)
{
  // ignore any requests to read after the end of the file
  if (slOffset>pza->za_slSize) {
    return -1;
  }

#if DEBUG_UNZIP
  INDEX ctBusySectors = 0;
  CTString strUsedSectors;

  INDEX ctSectors = zb_azsBuffered.Count();
  for(INDEX is=0;is<ctSectors;is++) {
    CZipSector &zs = zb_azsBuffered[is];
    if(zs.zs_bBusy) {
      strUsedSectors+= CTString(0,"%d ",is);
      ctBusySectors++;
    }
  }

  extern CTString _strDebug;
  extern PIX _pixDebugStringX, _pixDebugStringY;
  _pixDebugStringX = 150;
  _pixDebugStringY = 90;

  _strDebug.PrintF("%d sectors pending ( %s )\n", ctBusySectors, (const char*)strUsedSectors);
#endif

  // find if it is already read/pending
  INDEX iSector = -1;
  ASSERT(slOffset%zb_slSectorSize == 0);
  {for(INDEX i=0; i<zb_azsBuffered.Count(); i++) {
    CZipSector &zs = zb_azsBuffered[i];
    if (zs.zs_pzaArchive==pza && zs.zs_slOffset==slOffset) {
      iSector = i;
      break;
    }
  }}

  // if yes, just return this buffer number
  if (iSector>=0) {
    MarkSectorTouched(iSector);
    return iSector;
  }

  // find a new sector to use
  CZipSector *pzs = LIST_HEAD(zb_lhBuffersLRU, CZipSector, zs_lnInLRU);
  iSector = zb_azsBuffered.Index(pzs);
  CZipSector &zs = zb_azsBuffered[iSector];

  // if only reading ahead
  if(bReadAhead && zs.zs_bBusy) {
    // and if sector is used
    if(IsSectorUsed(iSector)) {
      // bail out
      return -1;
    }
  }
  // if the sector is currently pending for some other read
  if(zs.zs_bBusy) {
    // wait for it to finish
    WaitSectorRead_t(iSector);
  }


  // setup overlapped structure fields. 
  memset(&zs.zs_ol, 0, sizeof(zs.zs_ol));
  zs.zs_ol.Offset     = slOffset; 
  zs.zs_ol.OffsetHigh = 0; 
  zs.zs_ol.hEvent = zs.zs_hEvent;
  zs.zs_pzaArchive = pza;
  zs.zs_slOffset = slOffset;
  zs.zs_bBusy = TRUE;

  DWORD dwResult = 0;
  BOOL  bResult  = FALSE;

  // for NT kernels
  if( _bNTKernel) {
    // use async read
    bResult = ReadFile( (HANDLE)pza->za_fFile, zs.zs_pubBuffer, zb_slSectorSize, &dwResult, &zs.zs_ol);
  }
  // for 9X kernels
  else {
    // use standard read
    DWORD dwNewPtr = SetFilePointer( (HANDLE)pza->za_fFile, slOffset, NULL, FILE_BEGIN);
    ASSERT( dwNewPtr == slOffset); // must make it
    bResult = ReadFile( (HANDLE)pza->za_fFile, zs.zs_pubBuffer, zb_slSectorSize, &dwResult, NULL);
  }

  // analyze the read
  if (bResult) {
    // already buffered by the system
    zs.zs_bBusy = FALSE;
  } else {
    if(GetLastError()!=ERROR_IO_PENDING) {
      // Notify reading error
      NotifyReadingError(pza->za_fnm);
    }
  }

  MarkSectorTouched(iSector);
  return iSector;
}


BOOL CZipBuffer::IsSectorUsed(INDEX iSector)
{
  if (!_bNTKernel || iSector<0) {
    return TRUE;
  }

  CZipSector &zs = zb_azsBuffered[iSector];
  if (!zs.zs_bBusy) {
    return FALSE;
  }

  // Check on the results of the asynchronous read. 
  DWORD dwResult;
  BOOL bResult = GetOverlappedResult((HANDLE)zs.zs_pzaArchive->za_fFile, &zs.zs_ol, &dwResult, FALSE); 
  return !bResult;
}


void CZipBuffer::WaitSectorRead_t(INDEX iSector)
{
  if (iSector<0) {
    return;
  }
  CZipSector &zs = zb_azsBuffered[iSector];
  MarkSectorTouched(iSector);

  // CallProgressHook_t(-1);

  if (!zs.zs_bBusy) {
    return;
  }

  BOOL bResult;
  DWORD dwResult;
  DWORD dwError;
  for(;;)
  {
#if DEBUG_UNZIP
    // Check on the results of the asynchronous read.
    if( _bNTKernel) {
      bResult = GetOverlappedResult( (HANDLE)zs.zs_pzaArchive->za_fFile, &zs.zs_ol, &dwResult, FALSE); 
    } else {
      bResult = TRUE;
    }
    if (!bResult) {
      dwError = GetLastError();
      if (dwError==ERROR_IO_PENDING || dwError==ERROR_IO_INCOMPLETE) {
        if(_bPreReading) {
          CPrintF("Waiting sector %d on pre read\n", iSector);
        } else {
          CPrintF("Waiting sector %d on read\n", iSector);
        }
        bResult = GetOverlappedResult((HANDLE)zs.zs_pzaArchive->za_fFile, &zs.zs_ol, &dwResult, TRUE); 
        if (bResult) {
          break;
        }
      }
    } else {
      break;
    }
#else
    // Check on the results of the asynchronous read.
    if( _bNTKernel) {
      bResult = GetOverlappedResult( (HANDLE)zs.zs_pzaArchive->za_fFile, &zs.zs_ol, &dwResult, TRUE); 
    } else {
      bResult = TRUE;
    }
    if( bResult) {
      break;
    }
#endif

    dwError = GetLastError();
    if (dwError==ERROR_HANDLE_EOF) {
      dwResult = zb_slSectorSize;
      break;
    }
    if (dwError!=ERROR_IO_PENDING && dwError!=ERROR_IO_INCOMPLETE) {
      // Notify reading error
      NotifyReadingError(zs.zs_pzaArchive->za_fnm);
      zs.zs_bBusy = FALSE;
      ThrowF_t("Error reading from zip.");
    }
    // CallProgressHook_t(-1);
    //Sleep(1);
  }
  zs.zs_bBusy = FALSE;

  extern INDEX _ctTotalFromDisk;
  _ctTotalFromDisk += dwResult;
}


// read from a given position in the archive
SLONG CZipArchive::Read_t(void *pvBuffer, SLONG slOffset, SLONG slSize)
{
  UBYTE *pubBuffer = (UBYTE*)pvBuffer;
  SLONG slReqBeg = slOffset;
  SLONG slReqEnd = slOffset+slSize;
  SLONG slRead = 0;
  // while there is something to read
  while(slReqEnd>slReqBeg) {
    // determine the sector offset containing the beginning of the request
    SLONG slSectorOffset = slReqBeg&~(_zbBuffer.zb_slSectorSize-1);
    // request the sector to be loaded if not
    INDEX iSector = _zbBuffer.RequestReadSector_t(this, slSectorOffset);
    ASSERT(iSector>=0);
    // also request reads for some sectors ahead
    for (INDEX i=0; i<zip_iPrefetchSectors; i++) {
      _zbBuffer.RequestReadSector_t(this, slSectorOffset+_zbBuffer.zb_slSectorSize*(i+1));
    }

    // wait the sector loading to finish if not
    _zbBuffer.WaitSectorRead_t(iSector);

    // determine sector extents
    SLONG slBufBeg = _zbBuffer.zb_azsBuffered[iSector].zs_slOffset;
    SLONG slBufEnd = slBufBeg+_zbBuffer.zb_slSectorSize;
    ASSERT(slReqBeg>=slBufBeg && slReqBeg<slBufEnd);

    // copy a part from it
    SLONG slCopyBeg = slReqBeg;
    SLONG slCopyEnd = Min(slReqEnd, slBufEnd);
    SLONG slCopySize = slCopyEnd-slCopyBeg;
    memcpy(pubBuffer, _zbBuffer.zb_azsBuffered[iSector].zs_pubBuffer + (slCopyBeg-slBufBeg), slCopySize);
    
    // and trim it from the request
    pubBuffer += slCopySize;
    slReqBeg += slCopySize;
    slRead += slCopySize;
  }

  return slRead;
}

void CZipArchive::ReadAhead_t(SLONG slOffset, SLONG slSize)
{
  _bPreReading = TRUE;
  SLONG slReqBeg = slOffset;
  SLONG slReqEnd = slOffset+slSize;

  // while there is something to prepare
  while(slReqEnd>slReqBeg) {
    // determine the sector offset containing the beginning of the request
    SLONG slSectorOffset = slReqBeg&~(_zbBuffer.zb_slSectorSize-1);
    // request the sector to be loaded if not
    _zbBuffer.RequestReadSector_t(this, slSectorOffset,TRUE);
    slReqBeg += _zbBuffer.zb_slSectorSize;
  }
  _bPreReading = FALSE;
}

CZipHandle::CZipHandle(void) 
{
  zh_bOpen = FALSE;
  zh_pubBufIn = NULL;
  memset(&zh_zstream, 0, sizeof(zh_zstream));
}
void CZipHandle::Clear(void) 
{
  zh_bOpen = FALSE;
  zh_zeEntry.Clear();

  // clear the zlib stream
  CTSingleLock slZip(&zip_csLock, TRUE);
  inflateEnd(&zh_zstream);
  memset(&zh_zstream, 0, sizeof(zh_zstream));

  // free buffers
  if (zh_pubBufIn!=NULL) {
    FreeMemory(zh_pubBufIn);
    zh_pubBufIn = NULL;
  }
}
void CZipHandle::ThrowZLIBError_t(int ierr, const CTString &strDescription)
{
  ThrowF_t(TRANS("(%s/%s) %s - ZLIB error: %s - %s"), 
    (const CTString&)*zh_zeEntry.ze_pza->za_fnm, 
    (const CTString&)zh_zeEntry.ze_fnm,
    strDescription, GetZlibError(ierr), zh_zstream.msg);
}

// read more data into the buffer
SLONG CZipHandle::ReadMoreData(void)
{
  SLONG slMaxSize = zh_zeEntry.ze_slCompressedSize-zh_zstream.total_in;
  slMaxSize = Clamp(slMaxSize, SLONG(1024), SLONG(BUF_SIZE));
  SLONG slRead = zh_zeEntry.ze_pza->Read_t(zh_pubBufIn, zh_slPosition, slMaxSize);
  zh_slPosition+=slRead;
  if (fil_iReportStats>=3) {
    CPrintF("   zipRead: %9d->%9d\n", slMaxSize, slRead);
  }
  return slRead;
}


// convert slashes to backslashes in a file path
void ConvertSlashes(char *p)
{
  while (*p!=0) {
    if (*p=='/') {
      *p = '\\';
    }
    p++;
  }
}

// read directory of a zip archive and add all files in it to active set
void ReadZIPDirectory_t(CZipArchive &za)
{
  za.Open_t();

  // start at the end of file, minus expected minimum overhead
  int iPos = za.za_slSize-sizeof(long)-sizeof(EndOfDir)+2;
  // do not search more than 128k (should be around 65k at most)
  int iMinPos = iPos-128*1024;
  if (iMinPos<0) {
    iMinPos = 0;
  }

  EndOfDir eod;
  BOOL bEODFound = FALSE;
  // while not at beginning
  for(; iPos>iMinPos; iPos--) {
    // read signature
    int slSig;
    za.Read_t(&slSig, iPos, sizeof(slSig));
    // if this is the sig
    if (slSig==SIGNATURE_EOD) {
      iPos+=sizeof(slSig);
      // read directory end
      za.Read_t(&eod, iPos, sizeof(eod));
      // if multi-volume zip
      if (eod.eod_swDiskNo!=0||eod.eod_swDirStartDiskNo!=0
        ||eod.eod_swEntriesInDirOnThisDisk!=eod.eod_swEntriesInDir) {
        // fail
        ThrowF_t(TRANS("%s: Multi-volume zips are not supported"), (CTString&)za.za_fnm);
      }                                                     
      // check against empty zips
      if (eod.eod_swEntriesInDir<=0) {
        // fail
        ThrowF_t(TRANS("%s: Empty zip"), (CTString&)za.za_fnm);
      }                                                     
      // all ok
      bEODFound = TRUE;
      break;
    }
  }
  // if eod not found
  if (!bEODFound) {
    // fail
    ThrowF_t(TRANS("%s: Cannot find 'end of central directory'"), (CTString&)za.za_fnm);
  }

  // check if the zip is from a mod
  BOOL bMod = 
    za.za_fnm.HasPrefix(_fnmApplicationPath+"Mods\\") || 
    za.za_fnm.HasPrefix(_fnmCDPath+"Mods\\");

  // load the entire central dir in one go
  UBYTE *pubDirOrg = (UBYTE *)AllocMemory(eod.eod_slSizeOfDir);
  UBYTE *pubDir = pubDirOrg;
  za.Read_t(pubDir, eod.eod_slDirOffsetInFile, eod.eod_slSizeOfDir);

  INDEX ctFiles = 0;
  { // prepare enough space for files
    INDEX ctOrgFiles = _azeFiles.Count();
    _azeFiles.Push(eod.eod_swEntriesInDir);
    if (ctOrgFiles==0) {
      _azeFiles.PopAll();
    } else {
      _azeFiles.PopUntil(ctOrgFiles-1);
    }
  }

  // for each file
  for (INDEX iFile=0; iFile<eod.eod_swEntriesInDir; iFile++) {
    // read the sig
    int slSig;
    
    //zip_fread(&slSig, sizeof(slSig), 1, f);
    memcpy(&slSig, pubDir, sizeof(slSig));
    pubDir+=sizeof(slSig);

    // if this is not the expected sig
    if (slSig!=SIGNATURE_FH) {
      // fail
      FreeMemory(pubDirOrg); pubDirOrg = NULL;
      ThrowF_t(TRANS("%s: Wrong signature for 'file header' number %d'"), 
        (CTString&)za.za_fnm, iFile);
    }
    // read its header
    FileHeader fh;
    //zip_fread(&fh, sizeof(fh), 1, f);
    memcpy(&fh, pubDir, sizeof(fh));
    pubDir+=sizeof(fh);

    // read the filename
    const SLONG slMaxFileName = 512;
    char strBuffer[slMaxFileName+1];
    memset(strBuffer, 0, sizeof(strBuffer));
    if (fh.fh_swFileNameLen>slMaxFileName) {
      FreeMemory(pubDirOrg); pubDirOrg = NULL;
      ThrowF_t(TRANS("%s: Too long filepath in zip"), (CTString&)za.za_fnm);
    }
    if (fh.fh_swFileNameLen<=0) {
      FreeMemory(pubDirOrg); pubDirOrg = NULL;
      ThrowF_t(TRANS("%s: Invalid filepath length in zip"), (CTString&)za.za_fnm);
    }
    //zip_fread(strBuffer, fh.fh_swFileNameLen, 1, f);
    memcpy(strBuffer, pubDir, fh.fh_swFileNameLen);
    pubDir+=fh.fh_swFileNameLen;

    // skip eventual comment and extra fields
    if (fh.fh_swFileCommentLen+fh.fh_swExtraFieldLen>0) {
      //zip_fseek(f, fh.fh_swFileCommentLen+fh.fh_swExtraFieldLen, SEEK_CUR);
      pubDir+=fh.fh_swFileCommentLen+fh.fh_swExtraFieldLen;
    }

    // if the file is directory
    if (strBuffer[strlen(strBuffer)-1]=='/') {
      // check size
      if (fh.fh_slUncompressedSize!=0
        ||fh.fh_slCompressedSize!=0) {
        FreeMemory(pubDirOrg); pubDirOrg = NULL;
        ThrowF_t(TRANS("%s/%s: Invalid directory"), 
          (CTString&)za.za_fnm, strBuffer);
      }

    // if the file is real file
    } else {
      ctFiles++;
      // convert filename
      ConvertSlashes(strBuffer);
      // create a new entry
      CZipEntry &ze = _azeFiles.Push();
      // remember the file's data
      ze.ze_fnm = CTString(strBuffer);
      ze.ze_pza = &za;
      ze.ze_ulFlags = 0;
      ze.ze_slCompressedSize = fh.fh_slCompressedSize;
      ze.ze_slUncompressedSize = fh.fh_slUncompressedSize;
      ze.ze_slDataOffset = fh.fh_slLocalHeaderOffset;
      ze.ze_ulFlags |= ZEF_DATAISHEADER;
      ze.ze_ulCRC = fh.fh_slCRC32;
      if(bMod) {
        ze.ze_ulFlags|=ZEF_MOD;
      }
      // check for compression
      if (fh.fh_swCompressionMethod==0) {
        ze.ze_ulFlags |= ZEF_STORED;
      } else if (fh.fh_swCompressionMethod==8) {
        ze.ze_ulFlags &= ~ZEF_STORED;
      } else {
        FreeMemory(pubDirOrg); pubDirOrg = NULL;
        ThrowF_t(TRANS("%s/%s: Only 'deflate' compression is supported"),
          (CTString&)ze.ze_pza->za_fnm, ze.ze_fnm);
      }
    }
  }
  FreeMemory(pubDirOrg); pubDirOrg = NULL;

  // report that file was read
  CPrintF(TRANS("  %s: %d files\n"), (CTString&)za.za_fnm, ctFiles++);
}

// add one zip archive to current active set
void UNZIPAddArchive(const CTFileName &fnm)
{
  TRACKMEM(Mem, "UnZip");
  // remember its filename
  CZipArchive &zaNew = _azaArchives.Push();
  zaNew.za_fnm = fnm;
} 

// read directory of an archive
void ReadOneArchiveDir_t(CZipArchive &za)
{
  // remember current number of files
  INDEX ctOrgFiles = _azeFiles.Count();

  // try to
  try {
    // read the directory and add all files
    ReadZIPDirectory_t(za);
  // if failed
  } catch (char *) {
    // if some files were added
    if (ctOrgFiles<_azeFiles.Count()) {
      // remove them
      if (ctOrgFiles==0) {
        _azeFiles.PopAll();
      } else {
        _azeFiles.PopUntil(ctOrgFiles-1);
      }
    }
    // cascade the error
    throw;
  }
}

int qsort_CZipArchive_reverse(const void *elem1, const void *elem2 )
{                   
  // get the filenames
  const CTFileName &fnm1 = (*(CZipArchive **)elem1)->za_fnm;
  const CTFileName &fnm2 = (*(CZipArchive **)elem2)->za_fnm;
  // find if any is in a mod or on CD
  BOOL bMod1 = fnm1.HasPrefix(_fnmApplicationPath+"Mods\\");
  BOOL bCD1 = fnm1.HasPrefix(_fnmCDPath);
  BOOL bModCD1 = fnm1.HasPrefix(_fnmCDPath+"Mods\\");
  BOOL bMod2 = fnm2.HasPrefix(_fnmApplicationPath+"Mods\\");
  BOOL bCD2 = fnm2.HasPrefix(_fnmCDPath);
  BOOL bModCD2 = fnm2.HasPrefix(_fnmCDPath+"Mods\\");

  // calculate priorities based on location of gro file
  INDEX iPriority1 = 0;
  if (bMod1) {
    iPriority1 = 3;
  } else if (bModCD1) {
    iPriority1 = 2;
  } else if (bCD1) {
    iPriority1 = 0;
  } else {
    iPriority1 = 1;
  }

  INDEX iPriority2 = 0;
  if (bMod2) {
    iPriority2 = 3;
  } else if (bModCD2) {
    iPriority2 = 2;
  } else if (bCD2) {
    iPriority2 = 0;
  } else {
    iPriority2 = 1;
  }

  // find sorting order
  if (iPriority1<iPriority2) {
    return +1;
  } else if (iPriority1>iPriority2) {
    return -1;
  } else {
    return -stricmp(fnm1, fnm2);
  }
}
// read directories of all currently added archives, in reverse alphabetical order
void UNZIPReadDirectoriesReverse_t(void)
{
  TRACKMEM(Mem, "UnZip");
  // if no archives
  if (_azaArchives.Count()==0) {
    // do nothing
    return;
  }

  // sort the archive filenames reversely
  qsort(_azaArchives.da_Pointers, _azaArchives.Count(), sizeof(CZipArchive*), 
    qsort_CZipArchive_reverse);

  // init the zip buffer
  _zbBuffer.InitBuffers();

  CTString strAllErrors = "";
  // for each archive
  for (INDEX iArchive=0; iArchive<_azaArchives.Count(); iArchive++) {
    //try to
    try {
      // read its directory
      ReadOneArchiveDir_t(_azaArchives[iArchive]);
    // if failed
    } catch (char *strError) {
      // remember the error
      strAllErrors += strError;
      strAllErrors += "\n";
    }
  }

  // if there were errors
  if (strAllErrors!="") {
    // report them
    ThrowF_t("%s", strAllErrors);
  }
}

// check if a zip file entry exists
BOOL UNZIPFileExists(const CTFileName &fnm)
{
  TRACKMEM(Mem, "UnZip");
  // for each file
  for(INDEX iFile=0; iFile<_azeFiles.Count(); iFile++) {
    // if it is that one
    if (_azeFiles[iFile].ze_fnm == fnm) {
      return TRUE;
    }
  }
  return FALSE;
}

// enumeration for all files in all zips
INDEX UNZIPGetFileCount(void)
{
  return _azeFiles.Count();
}
const CTFileName &UNZIPGetFileAtIndex(INDEX i)
{
  return _azeFiles[i].ze_fnm;
}

BOOL UNZIPIsFileAtIndexMod(INDEX i)
{
  return _azeFiles[i].ze_ulFlags&ZEF_MOD;
}

// get index of a file (-1 for no file)
INDEX UNZIPGetFileIndex(const CTFileName &fnm)
{
  // for each file
  for(INDEX iFile=0; iFile<_azeFiles.Count(); iFile++) {
    // if it is that one
    if (_azeFiles[iFile].ze_fnm == fnm) {
      return iFile;
    }
  }
  return -1;
}

// get crc on a zip file entry, by index
ULONG UNZIPGetCRCByIndex(INDEX iFile)
{
  return _azeFiles[iFile].ze_ulCRC;
}

// get info on a zip file entry
void UNZIPGetFileInfo(INDEX iHandle, CTFileName &fnmZip, 
  SLONG &slOffset, SLONG &slSizeCompressed, SLONG &slSizeUncompressed, 
  BOOL &bCompressed)
{
  TRACKMEM(Mem, "UnZip");
  // check handle number
  if(iHandle<0 || iHandle>=_azhHandles.Count()) {
    ASSERT(FALSE);
    return;
  }
  // get the handle
  CZipHandle &zh = _azhHandles[iHandle];
  // check the handle
  if (!zh.zh_bOpen) {
    ASSERT(FALSE);
    return;
  }

  // get parameters
  fnmZip = zh.zh_zeEntry.ze_pza->za_fnm;
  bCompressed = !(zh.zh_zeEntry.ze_ulFlags&ZEF_STORED);
  // data must not point to header
  ASSERT(!(zh.zh_zeEntry.ze_ulFlags&ZEF_DATAISHEADER));
  slOffset = zh.zh_zeEntry.ze_slDataOffset;
  slSizeCompressed = zh.zh_zeEntry.ze_slCompressedSize;
  slSizeUncompressed = zh.zh_zeEntry.ze_slUncompressedSize;
}

// open a zip file entry for reading
INDEX UNZIPOpen_t(const CTFileName &fnm)
{
  TRACKMEM(Mem, "UnZip");
  CZipEntry *pze = NULL;
  // for each file
  INDEX iFile;
  for(iFile = 0; iFile < _azeFiles.Count(); iFile++) {
    // if it is that one
    if (_azeFiles[iFile].ze_fnm == fnm) {
      // stop searching
      pze = &_azeFiles[iFile];
      break;
    }
  }

  if(fil_iReportStats>=2) {
    CPrintF("   zipOpen: (%4d) %s\n", iFile, (const char*)fnm);
  }

  // if not found
  if (pze==NULL) {
    // fail
    ThrowF_t(TRANS("File not found: %s"), (const CTString&)fnm);
  }

  // for each existing handle
  BOOL bHandleFound = FALSE;
  INDEX iHandle;
  for (iHandle = 1; iHandle < _azhHandles.Count(); iHandle++) {
    // if unused
    if (!_azhHandles[iHandle].zh_bOpen) {
      // use that one
      bHandleFound = TRUE;
      break;
    }
  }
  // if no free handle found
  if (!bHandleFound) {
    // create a new one
    iHandle = _azhHandles.Count();
    _azhHandles.Push(1);
  }
  
  // get the handle
  CZipHandle &zh = _azhHandles[iHandle];
  ASSERT(!zh.zh_bOpen);
  zh.zh_zeEntry = *pze;
  CZipArchive &za = *zh.zh_zeEntry.ze_pza;

  BOOL bDataIsHeader = pze->ze_ulFlags&ZEF_DATAISHEADER;

  // if data is pointing to header
  if(bDataIsHeader) {
    // start reading from the local header of the entry
    SLONG slHeaderOffset = zh.zh_zeEntry.ze_slDataOffset;
    // read the sig
    int slSig;
    slHeaderOffset+=za.Read_t(&slSig, slHeaderOffset, sizeof(slSig));
    // if this is not the expected sig
    if (slSig!=SIGNATURE_LFH) {
      // fail
      ThrowF_t(TRANS("%s/%s: Wrong signature for 'local file header'"), 
        (CTString&)*zh.zh_zeEntry.ze_pza->za_fnm, zh.zh_zeEntry.ze_fnm);
    }
    // read the header
    LocalFileHeader lfh;
    slHeaderOffset+=za.Read_t(&lfh, slHeaderOffset, sizeof(lfh));

    // determine exact compressed data position
    SLONG slDataOffset = slHeaderOffset+lfh.lfh_swFileNameLen+lfh.lfh_swExtraFieldLen;
    // Update zip entry with file offset so this code will be executed only once
    pze->ze_slDataOffset = slDataOffset;
    // Data is pointing to data not to header of file
    pze->ze_ulFlags &= ~ZEF_DATAISHEADER;
    // update zip entry
    zh.zh_zeEntry.ze_slDataOffset = slDataOffset;
    zh.zh_zeEntry.ze_ulFlags = pze->ze_ulFlags;
    // start reading from file offset
    zh.zh_slPosition = slDataOffset;
  } else {
    // just update zip handle position
    zh.zh_slPosition = zh.zh_zeEntry.ze_slDataOffset;
  }

  // allocate buffers
  zh.zh_pubBufIn  = (UBYTE*)AllocMemory(BUF_SIZE);

  // initialize zlib stream
  CTSingleLock slZip(&zip_csLock, TRUE);
  zh.zh_zstream.next_out  = NULL;
  zh.zh_zstream.avail_out = 0;
  zh.zh_zstream.next_in   = NULL;
  zh.zh_zstream.avail_in  = 0;
  zh.zh_zstream.zalloc = (alloc_func)Z_NULL;
  zh.zh_zstream.zfree = (free_func)Z_NULL;
  int err = inflateInit2(&zh.zh_zstream, -15);  // 32k windows
  // if failed
  if (err!=Z_OK) {
    // clean up what is possible
    FreeMemory(zh.zh_pubBufIn );
    zh.zh_pubBufIn  = NULL;
    // throw error
    zh.ThrowZLIBError_t(err, TRANS("Cannot init inflation"));
  }

  // return the handle successfully
  zh.zh_bOpen = TRUE;
  return iHandle;
}

// get uncompressed size of a file
SLONG UNZIPGetSize(INDEX iHandle)
{
  TRACKMEM(Mem, "UnZip");
  // check handle number
  if(iHandle<0 || iHandle>=_azhHandles.Count()) {
    ASSERT(FALSE);
    return 0;
  }
  // get the handle
  CZipHandle &zh = _azhHandles[iHandle];
  // check the handle
  if (!zh.zh_bOpen) {
    ASSERT(FALSE);
    return 0;
  }

  return zh.zh_zeEntry.ze_slUncompressedSize;
}

// get CRC of a file
ULONG UNZIPGetCRC(INDEX iHandle)
{
  TRACKMEM(Mem, "UnZip");
  // check handle number
  if(iHandle<0 || iHandle>=_azhHandles.Count()) {
    ASSERT(FALSE);
    return 0;
  }
  // get the handle
  CZipHandle &zh = _azhHandles[iHandle];
  // check the handle
  if (!zh.zh_bOpen) {
    ASSERT(FALSE);
    return 0;
  }

  return zh.zh_zeEntry.ze_ulCRC;
}

// read a block from zip file
void UNZIPReadBlock_t(INDEX iHandle, UBYTE *pub, SLONG slStart, SLONG slLen)
{
  TRACKMEM(Mem, "UnZip");
  // check handle number
  if(iHandle<0 || iHandle>=_azhHandles.Count()) {
    ASSERT(FALSE);
    return;
  }
  // get the handle
  CZipHandle &zh = _azhHandles[iHandle];
  // check the handle
  if (!zh.zh_bOpen) {
    ASSERT(FALSE);
    return;
  }
  CZipArchive &za = *zh.zh_zeEntry.ze_pza;

  // if behind the end of file
  if (slStart>=zh.zh_zeEntry.ze_slUncompressedSize) {
    // do nothing
    return;
  }

  // clamp length to end of the entry data
  slLen = Min(slLen, zh.zh_zeEntry.ze_slUncompressedSize-slStart);
  extern INDEX _ctTotalEffective;
  _ctTotalEffective+=slLen;

  // data must not point to header
  ASSERT(!(zh.zh_zeEntry.ze_ulFlags&ZEF_DATAISHEADER));

  // if not compressed
  if (zh.zh_zeEntry.ze_ulFlags&ZEF_STORED) {
    // just read from file
    SLONG slRead = za.Read_t(pub, zh.zh_zeEntry.ze_slDataOffset+slStart, slLen);
    if (fil_iReportStats>=3) {
      CPrintF("   stoRead: %9d->%9d\n", slLen, slRead);
    }
    return;
  }

  CTSingleLock slZip(&zip_csLock, TRUE);

  // if behind the current pointer
  if (slStart<zh.zh_zstream.total_out) {
    // reset the zlib stream to beginning
    inflateReset(&zh.zh_zstream);
    zh.zh_zstream.avail_in = 0;
    zh.zh_zstream.next_in = NULL;
    // seek to start of zip entry data inside archive
    zh.zh_slPosition = zh.zh_zeEntry.ze_slDataOffset;
  }

  // while ahead of the current pointer
  while (slStart>zh.zh_zstream.total_out) {
    // if zlib has no more input
    while(zh.zh_zstream.avail_in==0) {
      // read more to it
      SLONG slRead = zh.ReadMoreData();
      if (slRead<=0) {
        return; // !!!!
      }
      // tell zlib that there is more to read
      zh.zh_zstream.next_in = zh.zh_pubBufIn;
      zh.zh_zstream.avail_in  = slRead;
    }
    // read dummy data from the output
    #define DUMMY_SIZE 256
    UBYTE aubDummy[DUMMY_SIZE];
    // decode to output
    zh.zh_zstream.avail_out = Min(SLONG(slStart-zh.zh_zstream.total_out), SLONG(DUMMY_SIZE));
    zh.zh_zstream.next_out = aubDummy;
    int ierr = inflate(&zh.zh_zstream, Z_SYNC_FLUSH);
    if (ierr!=Z_OK && ierr!=Z_STREAM_END) {
      zh.ThrowZLIBError_t(ierr, TRANS("Error seeking in zip"));
    }
  }

  // if not streaming continuously
  if (slStart!=zh.zh_zstream.total_out) {
    // this should not happen
    ASSERT(FALSE);
    // read empty
    memset(pub, 0, slLen);
    return;
  }

  // set zlib for writing to the block
  zh.zh_zstream.avail_out = slLen;
  zh.zh_zstream.next_out = pub;

  // while there is something to write to given block
  while (zh.zh_zstream.avail_out>0) {
    // if zlib has no more input
    while(zh.zh_zstream.avail_in==0) {
      // read more to it
      SLONG slRead = zh.ReadMoreData();
      if (slRead<=0) {
        return; // !!!!
      }
      // tell zlib that there is more to read
      zh.zh_zstream.next_in = zh.zh_pubBufIn;
      zh.zh_zstream.avail_in  = slRead;
    }
    // decode to output
    int ierr = inflate(&zh.zh_zstream, Z_SYNC_FLUSH);
    if (ierr!=Z_OK && ierr!=Z_STREAM_END) {
      zh.ThrowZLIBError_t(ierr, TRANS("Error reading from zip"));
    }
  }
}

void UNZIPReadAheadBlock_t(INDEX iHandle, SLONG slStart, SLONG slLen)
{
  TRACKMEM(Mem, "UnZip");
  // check handle number
  if(iHandle<0 || iHandle>=_azhHandles.Count()) {
    ASSERT(FALSE);
    return;
  }
  // get the handle
  CZipHandle &zh = _azhHandles[iHandle];
  // check the handle
  if (!zh.zh_bOpen) {
    ASSERT(FALSE);
    return;
  }
  CZipArchive &za = *zh.zh_zeEntry.ze_pza;

  // if behind the end of file
  if (slStart>=zh.zh_zeEntry.ze_slUncompressedSize) {
    // do nothing
    return;
  }

  // clamp length to end of the entry data
  slLen = Min(slLen, zh.zh_zeEntry.ze_slUncompressedSize-slStart);
  extern INDEX _ctTotalEffective;
  _ctTotalEffective+=slLen;

  // data must not point to header
  ASSERT(!(zh.zh_zeEntry.ze_ulFlags&ZEF_DATAISHEADER));

  // if not compressed
  if (zh.zh_zeEntry.ze_ulFlags&ZEF_STORED) {
    // just read from file
    za.ReadAhead_t(zh.zh_zeEntry.ze_slDataOffset+slStart, slLen);
    if (fil_iReportStats>=3) {
      CPrintF("   stoAhead: %9d\n", slLen);
    }
    return;
  } else {
    ASSERTALWAYS("Read ahead for compressed files isn't supported for now");
  }
}

// close a zip file entry
void UNZIPClose(INDEX iHandle)
{
  TRACKMEM(Mem, "UnZip");
  // check handle number
  if(iHandle<0 || iHandle>=_azhHandles.Count()) {
    ASSERT(FALSE);
    return;
  }
  // get the handle
  CZipHandle &zh = _azhHandles[iHandle];
  // check the handle
  if (!zh.zh_bOpen) {
    ASSERT(FALSE);
    return;
  }
  // clear it
  zh.Clear();
}

// reinitialize all buffers in all zips (called when buffering params change)
void UNZIPReinitBuffers(void)
{
  TRACKMEM(Mem, "UnZip");
  _zbBuffer.ClearBuffers();
  _zbBuffer.InitBuffers();
}

// adjust parameters for zip streaming caching
SLONG UNZIPGetSectorSize(void)
{
  return zip_slSectorSize;
}
void UNZIPSetSectorSize(SLONG slSize)
{
  if (zip_slSectorSize!=slSize) {

    zip_slSectorSize=slSize;
    UNZIPReinitBuffers();
  }
}

INDEX UNZIPGetBufferSectors(void)
{
  return zip_iBufferSectors;
}
void UNZIPSetBufferSectors(INDEX iSectors)
{
  if (zip_iBufferSectors!=iSectors) {

    zip_iBufferSectors=iSectors;
    UNZIPReinitBuffers();
  }
}

INDEX UNZIPGetPrefetchSectors(void)
{
  return zip_iPrefetchSectors;
}

void UNZIPSetPrefetchSectors(INDEX iSectors)
{
  zip_iPrefetchSectors = iSectors;
}
