
#ifndef SE_INCL_STREAM_H
#define SE_INCL_STREAM_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Base/Translation.h>
#include <Engine/Base/Lists.h>
#include <Engine/Base/CTString.h>
#include <Engine/Base/FileName.h>
#include <Engine/Base/ErrorReporting.h>
#include <Engine/Templates/DynamicStackArray.h>

// maximum length of file that can be saved (default: 8Mb)
ENGINE_API extern ULONG _ulMaxLengthOfSavingFile;

#define CTSTREAM_BEGIN CTStream::EnableStreamHandling(); __try
#define CTSTREAM_END __except( CTStream::ExceptionFilter( GetExceptionCode(),\
                                                          GetExceptionInformation()) )\
  {\
     CTStream::ExceptionFatalError();\
  }; CTStream::DisableStreamHandling();

/*
 * Chunk ID class
 */
#define CID_LENGTH 4
class ENGINE_API CChunkID {
public:
  char cid_ID[CID_LENGTH+1];
public:
  /* Default constructor and string constructor. */
  inline CChunkID(const char *strString = "    ");
  /* Comparison operator. */
  inline int operator==(const CChunkID &cidOther) const;
  inline int operator!=(const CChunkID &cidOther) const;
  inline operator const char *(void) const { return cid_ID; }
  inline operator char *(void) { return cid_ID; }
};

// inline implementations
/* Default constructor and string constructor. */
inline CChunkID::CChunkID(const char *strString /*= "    "*/) {
  ASSERT(strlen(strString)==CID_LENGTH);
  memcpy(cid_ID, strString, CID_LENGTH+1);
};
/* Comparison operator. */
inline int CChunkID::operator==(const CChunkID &cidOther) const {
  return (*((ULONG *)&cid_ID[0]) == *((ULONG *)&cidOther.cid_ID[0]));
};
inline int CChunkID::operator!=(const CChunkID &cidOther) const {
  return (*((ULONG *)&cid_ID[0]) != *((ULONG *)&cidOther.cid_ID[0]));
};

/*
 * CroTeam stream class -- abstract base class
 */
class ENGINE_API CTStream {
public:
  CListNode strm_lnListNode;  // for linking into main library's list of opened streams
public:
  UBYTE *strm_pubBufferBegin; // starting address of stram buffer in virtual adress space
  UBYTE *strm_pubBufferEnd;   // ending address of stram buffer in virtual adress space
  UBYTE *strm_pubCurrentPos;  // current position of stream ptr in virtual adress space
  UBYTE *strm_pubEOF;         // end of file ptr (if file stream is opened for reading)
  UBYTE *strm_pubMaxPos;      // maximum position in stream reached by current ptr
  CTString strm_strStreamDescription; // descriptive string

  enum DictionaryMode {
    DM_NONE,        // no dictionary on this file (yet)
    DM_ENABLED,     // dictionary is enabled, reading/writing rest of file
    DM_PROCESSING,  // reading/writing the dictionary itself
  } strm_dmDictionaryMode;    // dictionary mode of operation on this file
  SLONG strm_slDictionaryPos; // dictionary position in file (0 for no dictionary)
  INDEX strm_ctDictionaryImported;  // how many filenames were imported
  class CNameTable_CTFileName &strm_ntDictionary;  // name table for the dictionary
  CDynamicStackArray<CTFileName> strm_afnmDictionary; // dictionary is stored here

  BOOL m_bUseVirtualMem;

  /* Throw an exception of formatted string. */
  void Throw_t(char *strFormat, ...); // throw char *
  // read the dictionary from given offset in file (internal function)
  void ReadDictionary_intenal_t(SLONG slOffset);
  // copy filename dictionary from another stream
  void CopyDictionary(CTStream &strmOther);
public:
  // modes for opening streams
  enum OpenMode {
    OM_READ  = 1,
    OM_WRITE = 2,
    OM_READTEXT  = OM_READ ,
    OM_WRITETEXT = OM_WRITE,
    OM_READBINARY  = OM_READ ,
    OM_WRITEBINARY = OM_WRITE,
  };
  enum CreateMode { // OBSOLETE!
    CM_TEXT  = 1,
    CM_BINARY = 2,
  };
  // direction for seeking
  enum SeekDir {
    SD_BEG = SEEK_SET,
    SD_END = SEEK_END,
    SD_CUR = SEEK_CUR,
  };

  /* Static function enable stream handling. */
  static void EnableStreamHandling(void);
  /* Static function disable stream handling. */
  static void DisableStreamHandling(void);
  static void ClearStreamHandling(void);

#ifdef PLATFORM_WIN32 /* rcg10042001 !!! FIXME */
  /* Static function to filter exceptions and intercept access violation */
  static int ExceptionFilter(DWORD dwCode, _EXCEPTION_POINTERS *pExceptionInfoPtrs);
#endif

  /* Static function to report fatal exception error. */
  static void ExceptionFatalError(void);
  /* Pure virtual function that handles access violation exception */
  virtual void HandleAccess(INDEX iAccessedPage, BOOL bReadAttempted) = 0;

  /* Default constructor. */
  CTStream(void);
  /* Destruction. */
  void FreeBuffer(void);
  virtual ~CTStream(void);

  /* Function that updates maximum position reached with given adress */
  inline void UpdateMaxPos(UBYTE *pAdress) {
    if(pAdress>strm_pubMaxPos) strm_pubMaxPos=pAdress;
  };
  /* Function that updates maximum position reached */
  inline void UpdateMaxPos(void) {
    if(strm_pubCurrentPos>strm_pubMaxPos) strm_pubMaxPos=strm_pubCurrentPos;
  };
  /* Function that allocates given ammount of virtual address space */
  void AllocateVirtualMemory(ULONG ulBytesToAllocate);
  /* Function that commits given page */
  void CommitPage(INDEX iPage);
  /* Function that decommits given page */
  void DecommitPage(INDEX iPage);
  /* Function that pritects given page from writting (for read-only streams) */
  void ProtectPageFromWritting(INDEX iPage);

  /* Check if the stream can be read. -- used mainly for assertions */
  virtual BOOL IsReadable(void) = 0;
  /* Check if the stream can be written. -- used mainly for assertions */
  virtual BOOL IsWriteable(void) = 0;
  /* Check if the stream can be seeked. -- used mainly for assertions */
  virtual BOOL IsSeekable(void) = 0;

  /* Read a block of data from stream. */
  inline void Read_t(void *pvBuffer, SLONG slSize); // throw char *
  /* Write a block of data to stream. */
  inline void Write_t(const void *pvBuffer, SLONG slSize); // throw char *

  /* Seek in stream. */
  void Seek_t(SLONG slOffset, enum SeekDir sd); // throw char *
  /* Set absolute position in stream. */
  void SetPos_t(SLONG slPosition); // throw char *
  /* Get absolute position in stream. */
  SLONG GetPos_t(void); // throw char *
  /* Get size of stream */
  SLONG GetStreamSize(void);
  /* Get CRC32 of stream */
  virtual ULONG GetStreamCRC32_t(void);
  /* Check if file position points to the EOF */
  BOOL AtEOF(void);

  /* Get description of this stream (e.g. filename for a CFileStream). */
  inline CTString &GetDescription(void) { return strm_strStreamDescription; };

  /* Read an object from stream. */
  inline CTStream &operator>>(float  &f) { Read_t( &f, sizeof( f)); return *this; } // throw char *
  inline CTStream &operator>>(double &d) { Read_t( &d, sizeof( d)); return *this; } // throw char *
  inline CTStream &operator>>(ULONG &ul) { Read_t(&ul, sizeof(ul)); return *this; } // throw char *
  inline CTStream &operator>>(UWORD &uw) { Read_t(&uw, sizeof(uw)); return *this; } // throw char *
  inline CTStream &operator>>(UBYTE &ub) { Read_t(&ub, sizeof(ub)); return *this; } // throw char *
  inline CTStream &operator>>(SLONG &sl) { Read_t(&sl, sizeof(sl)); return *this; } // throw char *
  inline CTStream &operator>>(SWORD &sw) { Read_t(&sw, sizeof(sw)); return *this; } // throw char *
  inline CTStream &operator>>(SBYTE &sb) { Read_t(&sb, sizeof(sb)); return *this; } // throw char *
  inline CTStream &operator>>(BOOL   &b) { Read_t( &b, sizeof( b)); return *this; } // throw char *
  inline CTStream &operator>>(__int64 &i64) { Read_t( &i64, sizeof(i64)); return *this; } // throw char *
  /* Write an object into stream. */
  inline CTStream &operator<<(const float  &f) { Write_t( &f, sizeof( f)); return *this; } // throw char *
  inline CTStream &operator<<(const double &d) { Write_t( &d, sizeof( d)); return *this; } // throw char *
  inline CTStream &operator<<(const ULONG &ul) { Write_t(&ul, sizeof(ul)); return *this; } // throw char *
  inline CTStream &operator<<(const UWORD &uw) { Write_t(&uw, sizeof(uw)); return *this; } // throw char *
  inline CTStream &operator<<(const UBYTE &ub) { Write_t(&ub, sizeof(ub)); return *this; } // throw char *
  inline CTStream &operator<<(const SLONG &sl) { Write_t(&sl, sizeof(sl)); return *this; } // throw char *
  inline CTStream &operator<<(const SWORD &sw) { Write_t(&sw, sizeof(sw)); return *this; } // throw char *
  inline CTStream &operator<<(const SBYTE &sb) { Write_t(&sb, sizeof(sb)); return *this; } // throw char *
  inline CTStream &operator<<(const BOOL   &b) { Write_t( &b, sizeof( b)); return *this; } // throw char *
  inline CTStream &operator<<(const __int64 &i64) { Write_t( &i64, sizeof(i64)); return *this; } // throw char *

  // CTFileName reading/writing
  ENGINE_API friend CTStream &operator>>(CTStream &strmStream, CTFileName &fnmFileName);
  ENGINE_API friend CTStream &operator<<(CTStream &strmStream, const CTFileName &fnmFileName);

  /* Put a line of text into stream. */
  void PutLine_t(const char *strBuffer); // throw char *
  void PutString_t(const char *strString); // throw char *
  void FPrintF_t(const char *strFormat, ...); // throw char *
  /* Get a line of text from stream. */
  void GetLine_t(char *strBuffer, SLONG slBufferSize, char cDelimiter='\n'); // throw char *
  void GetLine_t(CTString &strLine, char cDelimiter='\n'); // throw char *

  CChunkID GetID_t(void); // throw char *
  CChunkID PeekID_t(void); // throw char *
  void ExpectID_t(const CChunkID &cidExpected); // throw char *
  void ExpectKeyword_t(const CTString &strKeyword); // throw char *
  SLONG GetSize_t(void); // throw char *
  void ReadRawChunk_t(void *pvBuffer, SLONG slSize); // throw char *
  void ReadChunk_t(void *pvBuffer, SLONG slExpectedSize); // throw char *
  void ReadFullChunk_t(const CChunkID &cidExpected, void *pvBuffer, SLONG slExpectedSize); // throw char *
  void *ReadChunkAlloc_t(SLONG slSize=0); // throw char *
  void ReadStream_t(CTStream &strmOther); // throw char *

  void WriteID_t(const CChunkID &cidSave); // throw char *
  void WriteSize_t(SLONG slSize); // throw char *
  void WriteRawChunk_t(void *pvBuffer, SLONG slSize); // throw char *  // doesn't write length
  void WriteChunk_t(void *pvBuffer, SLONG slSize); // throw char *
  void WriteFullChunk_t(const CChunkID &cidSave, void *pvBuffer, SLONG slSize); // throw char *
  void WriteStream_t(CTStream &strmOther); // throw char *

  // filename dictionary operations

  // start writing a with a dictionary
  void DictionaryWriteBegin_t(const CTFileName &fnmImportFrom, SLONG slImportOffset);
  // stop writing a with a dictionary
  void DictionaryWriteEnd_t(void);
  // start reading a with a dictionary
  SLONG DictionaryReadBegin_t(void);  // returns offset of dictionary for cross-file importing
  // stop reading a with a dictionary
  void DictionaryReadEnd_t(void);
  // preload all files mentioned in the dictionary
  void DictionaryPreload_t(void);

  bool is_png()		{ return strm_bPNG; }
protected:
	bool	strm_bPNG;
};

/*
 * CroTeam file stream class
 */
class ENGINE_API CTFileStream : public CTStream {
private:
  FILE *fstrm_pFile;    // ptr to opened file
  INDEX fstrm_iZipHandle; // handle of zip-file entry
  INDEX fstrm_iLastAccessedPage; // index of last commited page
  BOOL fstrm_bReadOnly;  // set if file is opened in read-only mode
public:
  /* Default constructor. */
  CTFileStream(void);
  /* Destructor. */
  virtual ~CTFileStream(void);

  /* Open an existing file. */
  void Open_t(const CTFileName &fnFileName, enum CTStream::OpenMode om=CTStream::OM_READ); // throw char *
  /* Create a new file or overwrite existing. */
  void Create_t(const CTFileName &fnFileName, enum CTStream::CreateMode cm=CTStream::CM_BINARY); // throw char *
  /* Set size of file (must be opened for write)*/
  void SetFileSize_t(SLONG slSize);
  /* Close an open file. */
  void Close(void);
  /* Prepare data to be read later */
  void FileReadAhead_t(SLONG slOffset, SLONG slReadSize);
  /* Writes given page into file */
  void WritePageToFile(INDEX iPageToWrite);
  /* Commites given page and reads it from file */
  void FileCommitPage( INDEX iPageToCommit);
  /* Decommites given page and writes it into file (if file is opened for writting) */
  void FileDecommitPage( INDEX iPageToDecommit);
  /* Get CRC32 of stream */
  ULONG GetStreamCRC32_t(void);

  // from CTStream
  inline virtual BOOL IsWriteable(void){ return !fstrm_bReadOnly;};
  inline virtual BOOL IsReadable(void){ return TRUE;};
  inline virtual BOOL IsSeekable(void){ return TRUE;};
  virtual void HandleAccess(INDEX iAccessedPage, BOOL bReadAttempted);
};

/*
 * CroTeam memory stream class
 */
class ENGINE_API CTMemoryStream : public CTStream {
private:
  BOOL mstrm_bReadable;    // set if stream is readable
  BOOL mstrm_bWriteable;   // set if stream is writeable
  INDEX mstrm_ctLocked;    // counter for buffer locking
public:
  /* Create dynamically resizing stream for reading/writing. */
  CTMemoryStream(void);
  /* Create static stream from given buffer. */
  CTMemoryStream(void *pvBuffer, SLONG slSize, CTStream::OpenMode om = CTStream::OM_READ);
  /* Destructor. */
  virtual ~CTMemoryStream(void);

  /* Lock the buffer contents and it's size. */
  void LockBuffer(void **ppvBuffer, SLONG *pslSize);
    /* Unlock buffer. */
  void UnlockBuffer(void);

  // memory stream can be opened only for reading and writting
  virtual BOOL IsWriteable(void);
  virtual BOOL IsReadable(void);
  virtual BOOL IsSeekable(void);
  virtual void HandleAccess(INDEX iAccessedPage, BOOL bReadAttempted);
};

// --------------------------- Inline function implementations
/*
 * Read a block of data from stream.
 */
inline void CTStream::Read_t(void *pvBuffer, SLONG slSize)  // throws char *
{
  // check parameters
  ASSERT(pvBuffer!=NULL && slSize>=0);
  // check that the stream can be read
  ASSERT(IsReadable());

#ifndef NDEBUG
  // if EOF would be hit before reading entire chunk
  if(strm_pubCurrentPos+slSize > strm_pubBufferEnd)
  {
    ASSERTALWAYS("Trying to read data after EOF!");
  }
#endif
  // read from istream
  memcpy( (char *)pvBuffer, strm_pubCurrentPos, slSize);
  // increase file read ptr
  strm_pubCurrentPos += slSize;
}
//! 스트림에 데이타 블록을 쓴다.
/*
 * Write a block of data to stream.
 */
inline void CTStream::Write_t(const void *pvBuffer, SLONG slSize) // throws char *
{
  // check parameters
  ASSERT(pvBuffer!=NULL && slSize>0);
  // check that the stream can be written
  ASSERT(IsWriteable());

#ifndef NDEBUG
  // if not successfull
  if(strm_pubCurrentPos > strm_pubBufferEnd)
  {
    // throw exception
    Throw_t(TRANS("EOF hit while writting"));
  }
#endif
  // write to ostream
  memcpy( strm_pubCurrentPos, (char *)pvBuffer, slSize);
  // increase file read ptr
  strm_pubCurrentPos += slSize;
}

// Test if a file exists.
ENGINE_API BOOL FileExists(const CTFileName &fnmFile);
// Test if a file exists for writing. 
// (this is can be diferent than normal FileExists() if a mod uses basewriteexclude.lst
ENGINE_API BOOL FileExistsForWriting(const CTFileName &fnmFile);
// Get file timestamp
ENGINE_API SLONG GetFileTimeStamp_t(const CTFileName &fnmFile); // throw char *
// Get CRC32 of a file
ENGINE_API ULONG GetFileCRC32_t(const CTFileName &fnmFile); // throw char *
// Test if a file is read only (also returns FALSE if file does not exist)
ENGINE_API BOOL IsFileReadOnly(const CTFileName &fnmFile);
// Delete a file (called 'CTDeleteFile' to avid name clashes with win32)
ENGINE_API BOOL CTDeleteFile(const CTFileName &fnmFile);
// Move a file (called 'CTMoveFile' to avid name clashes with win32)
ENGINE_API BOOL CTMoveFile(const CTFileName &fnmFileOld, const CTFileName &fnmFileNew);
#define RemoveFile CTDeleteFile

// Expand a file's filename to full path

// these are input flags for describing what you need the file for
#define EFP_READ   (1UL<<0)  // will open for reading
#define EFP_WRITE  (1UL<<1)  // will open for writing
#define EFP_NOZIPS (1UL<<31) // add this flag to forbid searching in zips
// these are return values 
#define EFP_NONE       0  // doesn't exist
#define EFP_FILE       1  // generic file on disk
#define EFP_BASEZIP    2  // file in one of base zips
#define EFP_MODZIP     3  // file in one of mod zips
ENGINE_API INDEX ExpandFilePath(ULONG ulType, const CTFileName &fnmOriginal, CTFileName &fnmExpanded);

// these are input flags for directory reading
#define DLI_RECURSIVE  (1UL<<0)  // recurse into subdirs
#define DLI_SEARCHCD   (1UL<<1)  // search the CD path also
#define DLI_ONLYDIRS   (1UL<<2)  // search only subdirs (implies DLI_NOZIPS)
#define DLI_NOZIPS     (1UL<<3)  // do not search in zip archives
// make a list of all files in a directory
ENGINE_API void MakeDirList(
  CDynamicStackArray<CTFileName> &adeDir, 
  const CTFileName &fnmDir,     // directory to list
  const CTString &strPattern,   // pattern for each file to match ("" matches all)
  ULONG ulFlags                 // additional flags
);

// global string with application path
ENGINE_API extern CTFileName _fnmApplicationPath;
// global string with current MOD path
ENGINE_API extern CTFileName _fnmMod;
// global string with current name (the parameter that is passed on cmdline)
ENGINE_API extern CTString _strModName;
// global string with url to be shown to users that don't have the mod installed
// (should be set by game.dll)
ENGINE_API extern CTString _strModURL;
// global string with current MOD extension (for adding to dlls)
ENGINE_API extern CTString _strModExt;
// global string with CD path (for minimal installations)
ENGINE_API extern CTFileName _fnmCDPath;
// global string with filename of the started application
ENGINE_API extern CTFileName _fnmApplicationExe;

// application path usage funtions
ENGINE_API void UseApplicationPath(void);
ENGINE_API void IgnoreApplicationPath(void);

#define DISK_BLOCK_SIZE (16L*1024L)
// Get free disk space
ENGINE_API extern __int64 GetDiskFreeSpace(const CTString &strDirectory);

/*
 * File signatures 
 */
#define SIGNING_BUFFER_SIZE (16L*1024L) // buffer size for signature calculation
#define SIGNATURE_SIZE 20               // bytes count stored in file as signature

/*
 * File error handling
 */
// Nofify that error accurred while reading
extern void NotifyReadingError(const CTFileName &fnmFile);
// Nofify that error accurred while writing
extern void NotifyWritingError(const CTFileName &fnmFile);
// Set callback function that will be called on read error
ENGINE_API extern void SetErrorReadingCallback(void(*pErrorReadingFile)(const CTFileName &fnmFile));
// Set callback function that will be called on write error
ENGINE_API extern void SetErrorWritingCallback(void(*pErrorWritingFile)(const CTFileName &fnmFile));

#endif  /* include-once check. */

