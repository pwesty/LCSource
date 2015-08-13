#include "stdh.h"

#include <sys\types.h>
#include <sys\stat.h>
#include <fcntl.h>
#include <io.h>
#include <direct.h>
#include <Engine/Base/Protection.h>

#include <Engine/Base/Stream.h>

#include <Engine/Base/Memory.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Base/Console.h>
#include <Engine/Base/Synchronization.h>
#include <Engine/Base/ErrorReporting.h>
#include <Engine/Base/ListIterator.inl>
#include <Engine/Base/Unzip.h>
#include <Engine/Base/CRC.h>
#include <Engine/Base/Shell.h>
#include <Engine/Templates/NameTable_CTFileName.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Templates/DynamicStackArray.cpp>
#include <Engine/Templates/DynamicContainer.cpp>

#include <Engine/Templates/Stock_CTextureData.h>
#include <Engine/Templates/Stock_CModelData.h>
#include <Engine/Templates/Stock_CSoundData.h>
#include <Engine/Templates/Stock_CModelInstance.h>
#include <Engine/Templates/Stock_CMesh.h>
#include <Engine/Templates/Stock_CSkeleton.h>
#include <Engine/Templates/Stock_CAnimSet.h>
#include <Engine/Templates/Stock_CEntityClass.h>
#include <Engine/Templates/Stock_CShader.h>
#include <Engine/Templates/Stock_CAnimData.h>
#include <Loading.h>

// default size of page used for stream IO operations (4Kb)
ULONG _ulPageSize = 0;
// maximum lenght of file that can be saved (default: 128Mb)
// [070531: Su-won] 파일을 열때마다 128MB의 메모리를 요청하여 에디터 작업시 메모리 부족 현상 발생.
// 최대 파일 크기를 128MB나 될 필요가 없을 듯 하여 64MB로 수정. 
//ULONG _ulMaxLenghtOfSavingFile = (1UL<<20)*128;
ULONG _ulMaxLenghtOfSavingFile = (1UL<<20)*64;

extern INDEX fil_bPreferZips = FALSE;
extern INDEX fil_iReportStats = 0;

// variables used for loader profiling
// NOTE: for zipped files, ammount loaded from disk is smaller than ammount of effective data
extern INDEX _ctTotalFromDisk = 0;  // amount of data actually read from disk
extern INDEX _ctTotalEffective = 0; // ammount of effective data loaded

// set if current thread has currently enabled stream handling
// static _declspec(thread) BOOL _bThreadCanHandleStreams = FALSE;
static TLVar<BOOL> _bThreadCanHandleStreams(FALSE);
// list of currently opened streams
static CListHead *_plhOpenedStreams = NULL;
static CTCriticalSection _csStreams;

static void DefaultErrorReadingCallback(const CTFileName &fnmFile);
static void DefaultErrorWritingCallback(const CTFileName &fnmFile);
// Callback function that will be called on read error
static void (*_pErrorReadingFile)(const CTFileName &fnmFile) = DefaultErrorReadingCallback;
// Callback function that will be called on write error
static void (*_pErrorWritingFile)(const CTFileName &fnmFile) = DefaultErrorWritingCallback;


// global string with application path
CTFileName _fnmApplicationPath;
// global string with filename of the started application
CTFileName _fnmApplicationExe;
// global string with current MOD path
CTFileName _fnmMod;
// global string with current name (the parameter that is passed on cmdline)
CTString _strModName;
// global string with url to be shown to users that don't have the mod installed
// (should be set by game.dll)
CTString _strModURL;
// global string with current MOD extension (for adding to dlls)
CTString _strModExt;
// global string with CD path (for minimal installations)
CTFileName _fnmCDPath;

// include/exclude lists for base dir writing/browsing
CDynamicStackArray<CTFileName> _afnmBaseWriteInc;
CDynamicStackArray<CTFileName> _afnmBaseWriteExc;
CDynamicStackArray<CTFileName> _afnmBaseBrowseInc;
CDynamicStackArray<CTFileName> _afnmBaseBrowseExc;
// list of paths or patterns that are not included when making CRCs for network connection
// this is used to enable connection between different localized versions
CDynamicStackArray<CTFileName> _afnmNoCRC;


// load a filelist
static BOOL LoadFileList(CDynamicStackArray<CTFileName> &afnm, const CTFileName &fnmList)
{
  afnm.PopAll();
  try {
    CTFileStream strm;
    strm.Open_t(fnmList);
    while(!strm.AtEOF()) {
      CTString strLine;
      strm.GetLine_t(strLine);
      strLine.TrimSpacesLeft();
      strLine.TrimSpacesRight();
      if (strLine!="") {
        afnm.Push() = strLine;
      }
    }
    return TRUE;
  } catch(char *strError) {
    CPrintF("%s\n", strError);
    return FALSE;
  }
}

extern BOOL FileMatchesList(CDynamicStackArray<CTFileName> &afnm, const CTFileName &fnm)
{
  for(INDEX i=0; i<afnm.Count(); i++) {
    if (fnm.Matches(afnm[i]) || fnm.HasPrefix(afnm[i])) {
      return TRUE;
    }
  }
  return FALSE;
}

static BOOL IsGroCurrentLocale(const CTString &strFileName)
{
  /*   // this part is only for testing on PC
  DWORD dwLanguage = 'ESP';
  if (strFileName.Matches("*_DEU.gro") && dwLanguage != 'DEU') {
    return FALSE;
  }
  if (strFileName.Matches("*_ESP.gro") && dwLanguage != 'ESP') {
    return FALSE;
  }
  if (strFileName.Matches("*_FRA.gro") && dwLanguage != 'FRA') {
    return FALSE;
  }
  if (strFileName.Matches("*_ITL.gro") && dwLanguage != 'ITL') {
    return FALSE;
  }
//*/
  return TRUE;  
}

static CTFileName _fnmApp;

void InitStreams(void)
{
  TRACKMEM(Mem, ".gro file headers");
  // obtain information about system
  SYSTEM_INFO siSystemInfo;
  GetSystemInfo( &siSystemInfo);
  // and remember page size
  _ulPageSize = siSystemInfo.dwPageSize*16;   // cca. 64kB on WinNT/Win95

  // keep a copy of path for setting purposes
  _fnmApp = _fnmApplicationPath;

  _csStreams.cs_iIndex = -1;

  // if no mod defined yet
  // deleted by seo 40619
  /*if (_fnmMod=="") {
    // check for 'default mod' file
    LoadStringVar(CTString("DefaultMod.txt"), _fnmMod);
  }

  CPrintF(TRANS("Current mod: %s\n"), _fnmMod==""?TRANS("<none>"):(CTString&)_fnmMod);
  
  // if there is a mod active
  if (_fnmMod!="") {
    // load mod's include/exclude lists
    CPrintF(TRANS("Loading mod include/exclude lists...\n"));
    BOOL bOK = FALSE;
    bOK |= LoadFileList(_afnmBaseWriteInc , CTString("BaseWriteInclude.lst"));
    bOK |= LoadFileList(_afnmBaseWriteExc , CTString("BaseWriteExclude.lst"));
    bOK |= LoadFileList(_afnmBaseBrowseInc, CTString("BaseBrowseInclude.lst"));
    bOK |= LoadFileList(_afnmBaseBrowseExc, CTString("BaseBrowseExclude.lst"));

    // if none found
    if (!bOK) {
      // the mod is not valid
      _fnmMod = CTString("");
      CPrintF(TRANS("Error: MOD not found!\n"));
    // if mod is ok
    } else {
      // remember mod name (the parameter that is passed on cmdline)
      _strModName = _fnmMod;
      _strModName.DeleteChar(_strModName.Length()-1);
      _strModName = CTFileName(_strModName).FileName();
    }
  }*/
  // find eventual extension for the mod's dlls
  if (_strModExt == "") {
    LoadStringVar(CTString("ModExt.txt"), _strModExt);
  }


  CPrintF(TRANS("Loading group files...\n"));

  // for each group file in base directory
  struct _finddata_t c_file;
  long hFile;
  hFile = _findfirst(_fnmApplicationPath+"*.gro", &c_file);
  BOOL bOK = (hFile!=-1);
  while(bOK) {
    if (CTString(c_file.name).Matches("*.gro") && IsGroCurrentLocale(CTString(c_file.name))) {
      // add it to active set
      UNZIPAddArchive(_fnmApplicationPath+c_file.name);
    }
    bOK = _findnext(hFile, &c_file)==0;
  }
  _findclose( hFile );

  // if there is a mod active
  if (_fnmMod!="") {
    // for each group file in mod directory
    struct _finddata_t c_file;
    long hFile;
    hFile = _findfirst(_fnmApplicationPath+_fnmMod+"*.gro", &c_file);
    BOOL bOK = (hFile!=-1);
    while(bOK) {
      if (CTString(c_file.name).Matches("*.gro") && IsGroCurrentLocale(CTString(c_file.name))) {
        // add it to active set
        UNZIPAddArchive(_fnmApplicationPath+_fnmMod+c_file.name);
      }
      bOK = _findnext(hFile, &c_file)==0;
    }
    _findclose( hFile );
  }

  // if there is a CD path
  if (_fnmCDPath!="") {
    // for each group file on the CD
    struct _finddata_t c_file;
    long hFile;
    hFile = _findfirst(_fnmCDPath+"*.gro", &c_file);
    BOOL bOK = (hFile!=-1);
    while(bOK) {
      if (CTString(c_file.name).Matches("*.gro") && IsGroCurrentLocale(CTString(c_file.name))) {
        // add it to active set
        UNZIPAddArchive(_fnmCDPath+c_file.name);
      }
      bOK = _findnext(hFile, &c_file)==0;
    }
    _findclose( hFile );

    // if there is a mod active
    if (_fnmMod!="") {
      // for each group file in mod directory
      struct _finddata_t c_file;
      long hFile;
      hFile = _findfirst(_fnmCDPath+_fnmMod+"*.gro", &c_file);
      BOOL bOK = (hFile!=-1);
      while(bOK) {
        if (CTString(c_file.name).Matches("*.gro") && IsGroCurrentLocale(CTString(c_file.name))) {
          // add it to active set
          UNZIPAddArchive(_fnmCDPath+_fnmMod+c_file.name);
        }
        bOK = _findnext(hFile, &c_file)==0;
      }
      _findclose( hFile );
    }
  }

  // try to
  try {
    // read the zip directories
    UNZIPReadDirectoriesReverse_t();
  // if failed
  } catch( char *strError) {
    // report warning
    CPrintF( TRANS("There were group file errors:\n%s"), strError);
  }
  CPrintF("\n");

  LoadFileList(_afnmNoCRC, CTFILENAME("Data\\NoCRC.lst"));

#if COPY_PROTECTION
  CTFileName fnmKey = CTString("T")+"S"+"E"+"."+"k"+"e"+"y";
  INDEX iSerial = _pShell->GetINDEX(CTString("sys")+"_iHDD"+"Misc");

  // init encryption table two times
  BLOWFISH_CTX ctx1;
  BLOWFISH_CTX ctx2;
  extern CTString _strEngineBuild;
  CTString strEnKey = _strEngineBuild;
  strEnKey.TrimLeft(16);
  char aubKey1[16];
  char aubKey2[16];
  for(INDEX i=0; i<16; i++) {
    aubKey1[i] = strEnKey[i];
    aubKey2[i] = strEnKey[16-1-i];
  }
  Blowfish_Init (&ctx1, (unsigned char*)aubKey1, 16);
  Blowfish_Init (&ctx2, (unsigned char*)aubKey2, 16);

  BOOL bKeyOK = TRUE;

  // if no key
  if (!FileExists(fnmKey)) {
    bKeyOK = FALSE;
  // if key is here
  } else {
    // load it
    CTString strKey;
    LoadStringVar(fnmKey, strKey);
    __int64 llCodeRead = 0;
    strKey.ScanF("%I64x", &llCodeRead);
    __int64 llCode1 = llCodeRead;
    __int64 llCode2 = llCodeRead;

    // decrypt the code
    Blowfish_Decrypt(&ctx1, (ULONG*)&llCode1+1, ((ULONG*)&llCode1));
    Blowfish_Decrypt(&ctx2, (ULONG*)&llCode2+1, ((ULONG*)&llCode2));
    // if not valid
    if (llCode1!=iSerial && llCode2!=iSerial) {
      bKeyOK = FALSE;
    }
  }

  _pShell->SetINDEX(CTString("sys")+"_iCPU"+"Misc", bKeyOK);
#else
  _pShell->SetINDEX(CTString("sys")+"_iCPU"+"Misc", 1);
#endif
}

void EndStreams(void)
{
}


void UseApplicationPath(void) 
{
  _fnmApplicationPath = _fnmApp;
}

void IgnoreApplicationPath(void)
{
  _fnmApplicationPath = CTString("");
}


/////////////////////////////////////////////////////////////////////////////
// Helper functions

/* Static function enable stream handling. */
void CTStream::EnableStreamHandling(void)
{
  ASSERT( 0 == _bThreadCanHandleStreams);

  _bThreadCanHandleStreams = TRUE;
  if (_plhOpenedStreams == NULL) {
    _plhOpenedStreams = new CListHead;
  }
}

/* Static function disable stream handling. */
void CTStream::DisableStreamHandling(void)
{
  ASSERT(0 != _bThreadCanHandleStreams );

  _bThreadCanHandleStreams = FALSE;
//안태훈 수정 시작	//(DevPartner Bug Fix)(2005-01-13)
  //delete _plhOpenedStreams;
  //_plhOpenedStreams = NULL;
//안태훈 수정 끝	//(DevPartner Bug Fix)(2005-01-13)
}

void CTStream::ClearStreamHandling(void)
{
  delete _plhOpenedStreams;
  _plhOpenedStreams = NULL;
}

int CTStream::ExceptionFilter(DWORD dwCode, _EXCEPTION_POINTERS *pExceptionInfoPtrs)
{
  // If the exception is not a page fault, exit.
  if( dwCode != EXCEPTION_ACCESS_VIOLATION)
  {
    return EXCEPTION_CONTINUE_SEARCH;
  }
  CTSingleLock sl(&_csStreams, TRUE);

  // obtain access violation virtual address
  UBYTE *pIllegalAdress = (UBYTE *)pExceptionInfoPtrs->ExceptionRecord->ExceptionInformation[1];

  CTStream *pstrmAccessed = NULL;

  // search for stream that was accessed
  FOREACHINLIST( CTStream, strm_lnListNode, (*_plhOpenedStreams), itStream)
  {
    // if access violation happened inside curently testing stream
    if( (pIllegalAdress>=itStream->strm_pubBufferBegin) &&
        (pIllegalAdress<itStream->strm_pubBufferEnd) )
    {
      // remember accesed stream ptr
      pstrmAccessed = &itStream.Current();
      // stream found, stop searching
      break;
    }
  }

  // if none of our streams was accessed, real access violation occured
  if( pstrmAccessed == NULL)
  {
    // so continue default exception handling
    return EXCEPTION_CONTINUE_SEARCH;
  }

  // calculate accessed page
  int iAccessedPage = (pIllegalAdress-pstrmAccessed->strm_pubBufferBegin)/_ulPageSize;
  // update max ptr
  pstrmAccessed->UpdateMaxPos( pIllegalAdress);

  // call inherited class's exception handler
  pstrmAccessed->HandleAccess( iAccessedPage,
                pExceptionInfoPtrs->ExceptionRecord->ExceptionInformation[0] == 0);
  // Continue execution where the page fault occurred
  return EXCEPTION_CONTINUE_EXECUTION;
}

/*
 * Static function to report fatal exception error.
 */
void CTStream::ExceptionFatalError(void)
{
  FatalError( GetWindowsError( GetLastError()) );
}

/*
 * Throw an exception of formatted string.
 */
void CTStream::Throw_t(char *strFormat, ...)  // throws char *
{
  const SLONG slBufferSize = 256;
  char strFormatBuffer[slBufferSize];
  char strBuffer[slBufferSize];
  // add the stream description to the format string
  _snprintf(strFormatBuffer, slBufferSize, "%s (%s)", strFormat, strm_strStreamDescription);
  // format the message in buffer
  va_list arg;
  va_start(arg, strFormat); // variable arguments start after this argument
  _vsnprintf(strBuffer, slBufferSize, strFormatBuffer, arg);
  throw strBuffer;
}

/////////////////////////////////////////////////////////////////////////////
// Binary access methods
/*
 * Seek in file.
 */
void CTStream::Seek_t(SLONG slOffset, enum SeekDir sd)  // throws char *
{
  CTSingleLock sl(&_csStreams, TRUE);
  // update max pos with current position
  UpdateMaxPos();
  // check that the stream can be seeked
  ASSERT(IsSeekable());

  // if "seek relative to beginning of the file" requested
  if( sd == SD_BEG)
  {
    strm_pubCurrentPos = strm_pubBufferBegin + slOffset;
  }
  // else if "seek relative to end of the file" requested
  else if( sd == SD_END)
  {
    strm_pubCurrentPos = strm_pubEOF + slOffset;
  }
  // else if "seek relative to current file position" requested
  else if( sd == SD_CUR)
  {
    strm_pubCurrentPos += slOffset;
  }
  else
  {
    FatalError(TRANS("Stream seeking requested with unknown seek direction: %d\n"), sd);
  }
  // if not successfull
  if( (strm_pubCurrentPos<strm_pubBufferBegin) || (strm_pubCurrentPos>strm_pubBufferEnd) )
  {
    // throw exception
    Throw_t(TRANS("Error while seeking"));
  }
}

/*
 * Set absolute position in file.
 */
void CTStream::SetPos_t(SLONG slPosition) // throws char *
{
  CTSingleLock sl(&_csStreams, TRUE);
  // seek from start
  Seek_t(slPosition, CTStream::SD_BEG);
}

/*
 * Get absolute position in file.
 */
SLONG CTStream::GetPos_t(void)  // throws char *
{
  CTSingleLock sl(&_csStreams, TRUE);
  // check that the file is open
  ASSERT(IsSeekable());
  // calculate position
  return strm_pubCurrentPos-strm_pubBufferBegin;
}

/*
 * Check if file position points to the EOF
 */
BOOL CTStream::AtEOF(void)
{
  CTSingleLock sl(&_csStreams, TRUE);
  // check that the file is open and not writeable
  ASSERT(IsSeekable());
  ASSERT(!IsWriteable());
  // calculate position
  return (strm_pubCurrentPos >= strm_pubEOF);
}

SLONG CTStream::GetStreamSize(void)
{
  CTSingleLock sl(&_csStreams, TRUE);
  if (!IsWriteable()) {
    return strm_pubEOF-strm_pubBufferBegin;
  } else {
    UpdateMaxPos();
    return strm_pubMaxPos-strm_pubBufferBegin;
  }
}

/* Get CRC32 of stream */
ULONG CTStream::GetStreamCRC32_t(void)
{
  CTSingleLock sl(&_csStreams, TRUE);
  // remember where stream is now
  SLONG slOldPos = GetPos_t();
  // go to start of file
  SetPos_t(0);
  // get size of file
  SLONG slFileSize = GetStreamSize();

  ULONG ulCRC;
  CRC_Start(ulCRC);

  // for each block in file
  const SLONG slBlockSize = 4096;
  for(SLONG slPos=0; slPos<slFileSize; slPos+=slBlockSize) {
    // read the block
    UBYTE aubBlock[slBlockSize];
    SLONG slThisBlockSize = Min(slFileSize-slPos, slBlockSize);
    Read_t(aubBlock, slThisBlockSize);
    // checksum it
    CRC_AddBlock(ulCRC, aubBlock, slThisBlockSize);
  }

  // restore position
  SetPos_t(slOldPos);

  CRC_Finish(ulCRC);
  return ulCRC;
}

/////////////////////////////////////////////////////////////////////////////
// Text access methods

/* Get a line of text from file. */
// throws char *
void CTStream::GetLine_t(char *strBuffer, SLONG slBufferSize, char cDelimiter /*='\n'*/ )
{
  CTSingleLock sl(&_csStreams, TRUE);
  // check parameters
  ASSERT(strBuffer!=NULL && slBufferSize>0);
  // check that the stream can be read
  ASSERT(IsReadable());
  // and that stream has valid stream ptr set
  ASSERT( strm_pubEOF!=NULL);
  // letters slider
  INDEX iLetters = 0;
  // test if EOF reached
  if( strm_pubCurrentPos >= strm_pubEOF)
  {
    Throw_t(TRANS("EOF reached, file %s"), strm_strStreamDescription);
  }
  // get line from istream
  FOREVER
  {
    // don't read "\r" characters but rather act like they don't exist
    if( *strm_pubCurrentPos != '\r')
    {
      strBuffer[ iLetters] = *strm_pubCurrentPos;
      // stop reading when delimiter loaded
      if( strBuffer[ iLetters] == cDelimiter)
      {
        // convert delimiter to zero
        strBuffer[ iLetters] = 0;
        // jump over delimiter
        strm_pubCurrentPos++;
        break;
      }
      // jump to next destination letter
      iLetters++;
    }
    // jump to next source letter
    strm_pubCurrentPos++;
    // test if maximum buffer lenght reached
    if( iLetters==slBufferSize)
    {
      return;
    }

    if( strm_pubCurrentPos >= strm_pubEOF)
    {
      strBuffer[ iLetters] = 0;
      return;
    }
  }
}

void CTStream::GetLine_t(CTString &strLine, char cDelimiter/*='\n'*/) // throw char *
{
  char strBuffer[1024];
  GetLine_t(strBuffer, sizeof(strBuffer)-1, cDelimiter);
  strLine = strBuffer;
}


/* Put a line of text into file. */
void CTStream::PutLine_t(const char *strBuffer) // throws char *
{
  CTSingleLock sl(&_csStreams, TRUE);
  // check parameters
  ASSERT(strBuffer!=NULL);
  // check that the stream is writteable
  ASSERT(IsWriteable());
  // get string lenght
  INDEX iStringLenght = strlen(strBuffer);
  // put line into stream
  for( INDEX iLetter=0; iLetter<iStringLenght; iLetter++)
  {
    *strm_pubCurrentPos++ = *strBuffer++;
  }
  // write "\r\n" into stream
  *strm_pubCurrentPos++ = '\r';
  *strm_pubCurrentPos++ = '\n';
}

void CTStream::PutString_t(const char *strString) // throw char *
{
  CTSingleLock sl(&_csStreams, TRUE);
  // check parameters
  ASSERT(strString!=NULL);
  // check that the stream is writteable
  ASSERT(IsWriteable());
  // get string lenght
  INDEX iStringLenght = strlen(strString);
  // put line into stream
  for( INDEX iLetter=0; iLetter<iStringLenght; iLetter++)
  {
    if (*strString=='\n') {
      // write "\r\n" into stream
      *strm_pubCurrentPos++ = '\r';
      *strm_pubCurrentPos++ = '\n';
      strString++;
    } else {
      *strm_pubCurrentPos++ = *strString++;
    }
  }
}

void CTStream::FPrintF_t(const char *strFormat, ...) // throw char *
{
  CTSingleLock sl(&_csStreams, TRUE);
  const SLONG slBufferSize = 2048;
  char strBuffer[slBufferSize];
  // format the message in buffer
  va_list arg;
  va_start(arg, strFormat); // variable arguments start after this argument
  _vsnprintf(strBuffer, slBufferSize, strFormat, arg);
  // print the buffer
  PutString_t(strBuffer);
}

/////////////////////////////////////////////////////////////////////////////
// Chunk reading/writing methods

CChunkID CTStream::GetID_t(void) // throws char *
{
  CTSingleLock sl(&_csStreams, TRUE);
	CChunkID cidToReturn;
	Read_t( &cidToReturn.cid_ID[0], CID_LENGTH);
	return( cidToReturn);
}

CChunkID CTStream::PeekID_t(void) // throw char *
{
  CTSingleLock sl(&_csStreams, TRUE);
  // read the chunk id
	CChunkID cidToReturn;
	Read_t( &cidToReturn.cid_ID[0], CID_LENGTH);
  // return the stream back
  Seek_t(-CID_LENGTH, SD_CUR);
	return( cidToReturn);
}

void CTStream::ExpectID_t(const CChunkID &cidExpected) // throws char *
{
  CTSingleLock sl(&_csStreams, TRUE);
	CChunkID cidToCompare, cidpng;

	Read_t( &cidToCompare.cid_ID[0], CID_LENGTH);
	if( !(cidToCompare == cidExpected))
	{
		if (*(int*)(&cidToCompare.cid_ID[0]) == 0x474E5089)		// png signature
		{
			strm_bPNG = true;
			return;
		}

		Throw_t(TRANS("Chunk ID validation failed.\nExpected ID \"%s\" but found \"%s\"\n"),
      cidExpected.cid_ID, cidToCompare.cid_ID);
	}
}
void CTStream::ExpectKeyword_t(const CTString &strKeyword) // throw char *
{
  CTSingleLock sl(&_csStreams, TRUE);
  // check that the keyword is present
  for(INDEX iKeywordChar=0; iKeywordChar<(INDEX)strlen(strKeyword); iKeywordChar++) {
    SBYTE chKeywordChar;
    (*this)>>chKeywordChar;
    if (chKeywordChar!=strKeyword[iKeywordChar]) {
      Throw_t(TRANS("Expected keyword %s not found"), strKeyword);
    }
  }
}


SLONG CTStream::GetSize_t(void) // throws char *
{
  CTSingleLock sl(&_csStreams, TRUE);
	SLONG chunkSize;

	Read_t( (char *) &chunkSize, sizeof( SLONG));
	return( chunkSize);
}

void CTStream::ReadRawChunk_t(void *pvBuffer, SLONG slSize)  // throws char *
{
  CTSingleLock sl(&_csStreams, TRUE);
	Read_t((char *)pvBuffer, slSize);
}

void CTStream::ReadChunk_t(void *pvBuffer, SLONG slExpectedSize) // throws char *
{
  CTSingleLock sl(&_csStreams, TRUE);
	if( slExpectedSize != GetSize_t())
		throw TRANS("Chunk size not equal as expected size");
	Read_t((char *)pvBuffer, slExpectedSize);
}

void CTStream::ReadFullChunk_t(const CChunkID &cidExpected, void *pvBuffer,
                             SLONG slExpectedSize) // throws char *
{
  CTSingleLock sl(&_csStreams, TRUE);
	ExpectID_t( cidExpected);
	ReadChunk_t( pvBuffer, slExpectedSize);
};

void* CTStream::ReadChunkAlloc_t(SLONG slSize) // throws char *
{
  CTSingleLock sl(&_csStreams, TRUE);
	UBYTE *buffer;
	SLONG chunkSize;

	if( slSize > 0)
		chunkSize = slSize;
	else
		chunkSize = GetSize_t(); // throws char *
	buffer = (UBYTE *) AllocMemory( chunkSize);
	if( buffer == NULL)
		throw TRANS("ReadChunkAlloc: Unable to allocate needed amount of memory.");
	Read_t((char *)buffer, chunkSize); // throws char *
	return buffer;
}
void CTStream::ReadStream_t(CTStream &strmOther) // throw char *
{
  CTSingleLock sl(&_csStreams, TRUE);
  // implement this !!!! @@@@
}

void CTStream::WriteID_t(const CChunkID &cidSave) // throws char *
{
  CTSingleLock sl(&_csStreams, TRUE);
	Write_t( &cidSave.cid_ID[0], CID_LENGTH);
}

void CTStream::WriteSize_t(SLONG slSize) // throws char *
{
  CTSingleLock sl(&_csStreams, TRUE);
	Write_t( (char *)&slSize, sizeof( SLONG));
}

void CTStream::WriteRawChunk_t(void *pvBuffer, SLONG slSize) // throws char *
{
  CTSingleLock sl(&_csStreams, TRUE);
	Write_t( (char *)pvBuffer, slSize);
}

void CTStream::WriteChunk_t(void *pvBuffer, SLONG slSize) // throws char *
{
  CTSingleLock sl(&_csStreams, TRUE);
	WriteSize_t( slSize);
	WriteRawChunk_t( pvBuffer, slSize);
}

void CTStream::WriteFullChunk_t(const CChunkID &cidSave, void *pvBuffer,
                              SLONG slSize) // throws char *
{
  CTSingleLock sl(&_csStreams, TRUE);
	WriteID_t( cidSave); // throws char *
	WriteChunk_t( pvBuffer, slSize); // throws char *
}
void CTStream::WriteStream_t(CTStream &strmOther) // throw char *
{
  CTSingleLock sl(&_csStreams, TRUE);
  // implement this !!!! @@@@
}

/////////////////////////////////////////////////////////////////////////////
// filename dictionary operations

// enable dictionary in writable file from this point
void CTStream::DictionaryWriteBegin_t(const CTFileName &fnmImportFrom, SLONG slImportOffset)
{
  CTSingleLock sl(&_csStreams, TRUE);
  ASSERT(strm_slDictionaryPos==0);
  ASSERT(strm_dmDictionaryMode == DM_NONE);
  strm_ntDictionary.SetAllocationParameters(100, 5, 5);
  strm_ctDictionaryImported = 0;

  // if importing an existing dictionary to start with
  if (fnmImportFrom!="") {
    // open that file
    CTFileStream strmOther;
    strmOther.Open_t(fnmImportFrom);
    // read the dictionary in that stream
    strmOther.ReadDictionary_intenal_t(slImportOffset);
    // copy the dictionary here
    CopyDictionary(strmOther);
    // write dictionary importing data
    WriteID_t("DIMP");  // dictionary import
    *this<<fnmImportFrom<<slImportOffset;
    // remember how many filenames were imported
    strm_ctDictionaryImported = strm_afnmDictionary.Count();
  }

  // write dictionary position chunk id
  WriteID_t("DPOS");  // dictionary position
  // remember where position will be placed
  strm_slDictionaryPos = GetPos_t();
  // leave space for position
  *this<<SLONG(0);

  // start dictionary
  strm_dmDictionaryMode = DM_ENABLED;
}

// write the dictionary (usually at the end of file)
void CTStream::DictionaryWriteEnd_t(void)
{
  CTSingleLock sl(&_csStreams, TRUE);
  ASSERT(strm_dmDictionaryMode == DM_ENABLED);
  ASSERT(strm_slDictionaryPos>0);
  // remember the dictionary position chunk position
  SLONG slDictPos = strm_slDictionaryPos;
  // mark that now saving dictionary
  strm_slDictionaryPos = -1;
  // remember where dictionary begins
  SLONG slDictBegin = GetPos_t();
  // start dictionary processing
  strm_dmDictionaryMode = DM_PROCESSING;

  WriteID_t("DICT");  // dictionary
  // write number of used filenames
  INDEX ctFileNames = strm_afnmDictionary.Count();
  INDEX ctFileNamesNew = ctFileNames-strm_ctDictionaryImported;
  *this<<ctFileNamesNew;
  // for each filename
  for(INDEX iFileName=strm_ctDictionaryImported; iFileName<ctFileNames; iFileName++) {
    // write it to disk
    *this<<strm_afnmDictionary[iFileName];
  }
  WriteID_t("DEND");  // dictionary end

  // remember where is end of dictionary
  SLONG slContinue = GetPos_t();

  // write the position back to dictionary position chunk
  SetPos_t(slDictPos);
  *this<<slDictBegin;

  // stop dictionary processing
  strm_dmDictionaryMode = DM_NONE;
  strm_ntDictionary.Clear();
  strm_afnmDictionary.Clear();

  // return to end of dictionary
  SetPos_t(slContinue);
  strm_slDictionaryPos=0;
}

// read the dictionary from given offset in file (internal function)
void CTStream::ReadDictionary_intenal_t(SLONG slOffset)
{
  CTSingleLock sl(&_csStreams, TRUE);
  // remember where to continue loading
  SLONG slContinue = GetPos_t();

  // go to dictionary beginning
  SetPos_t(slOffset);

  // start dictionary processing
  strm_dmDictionaryMode = DM_PROCESSING;

  ExpectID_t("DICT");  // dictionary
  // read number of new filenames
  INDEX ctFileNamesOld = strm_afnmDictionary.Count();
  INDEX ctFileNamesNew;
  *this>>ctFileNamesNew;
  // if there are any new filenames
  if (ctFileNamesNew>0) {
    // create that much space
    strm_afnmDictionary.Push(ctFileNamesNew);
    // for each filename
    for(INDEX iFileName=ctFileNamesOld; iFileName<ctFileNamesOld+ctFileNamesNew; iFileName++) {
      // read it
      *this>>strm_afnmDictionary[iFileName];
    }
  }
  ExpectID_t("DEND");  // dictionary end

  // remember where end of dictionary is
  strm_slDictionaryPos = GetPos_t();

  // return to continuing position
  SetPos_t(slContinue);
}

// copy filename dictionary from another stream
void CTStream::CopyDictionary(CTStream &strmOther)
{
  CTSingleLock sl(&_csStreams, TRUE);
   strm_afnmDictionary = strmOther.strm_afnmDictionary;
   for (INDEX i=0; i<strm_afnmDictionary.Count(); i++) {
     strm_ntDictionary.Add(&strm_afnmDictionary[i]);
   }
}

SLONG CTStream::DictionaryReadBegin_t(void)
{
  CTSingleLock sl(&_csStreams, TRUE);
  ASSERT(strm_dmDictionaryMode == DM_NONE);
  ASSERT(strm_slDictionaryPos==0);
  strm_ntDictionary.SetAllocationParameters(100, 5, 5);

  SLONG slImportOffset = 0;
  // if there is imported dictionary
  if (PeekID_t()==CChunkID("DIMP")) {  // dictionary import
    // read dictionary importing data
    ExpectID_t("DIMP");  // dictionary import
    CTFileName fnmImportFrom;
    *this>>fnmImportFrom>>slImportOffset;

    // open that file
    CTFileStream strmOther;
    strmOther.Open_t(fnmImportFrom);
    // read the dictionary in that stream
    strmOther.ReadDictionary_intenal_t(slImportOffset);
    // copy the dictionary here
    CopyDictionary(strmOther);
  }

  // if the dictionary is not here
  if (PeekID_t()!=CChunkID("DPOS")) {  // dictionary position
    // do nothing
    return 0;
  }

  // read dictionary position
  ExpectID_t("DPOS"); // dictionary position
  SLONG slDictBeg;
  *this>>slDictBeg;

  // read the dictionary from that offset in file
  ReadDictionary_intenal_t(slDictBeg);

  // stop dictionary processing - go to dictionary using
  strm_dmDictionaryMode = DM_ENABLED;

  // return offset of dictionary for later cross-file importing
  if (slImportOffset!=0) {
    return slImportOffset;
  } else {
    return slDictBeg;
  }
}

void CTStream::DictionaryReadEnd_t(void)
{
  CTSingleLock sl(&_csStreams, TRUE);
  if (strm_dmDictionaryMode == DM_ENABLED) {
    ASSERT(strm_slDictionaryPos>0);
    // just skip the dictionary (it was already read)
    SetPos_t(strm_slDictionaryPos);
    strm_slDictionaryPos=0;
    strm_dmDictionaryMode = DM_NONE;
    strm_ntDictionary.Clear();

    // for each filename
    INDEX ctFileNames = strm_afnmDictionary.Count();
    for(INDEX iFileName=0; iFileName<ctFileNames; iFileName++) {
      CTFileName &fnm = strm_afnmDictionary[iFileName];
      // if not preloaded
      if (fnm.fnm_pserPreloaded==NULL) {
        // skip
        continue;
      }
      // free preloaded instance
      CTString strExt = fnm.FileExt();
      if (strExt==".tex") {
        _pTextureStock->Release((CTextureData*)fnm.fnm_pserPreloaded);
      } else if (strExt==".mdl") {
        _pModelStock->Release((CModelData*)fnm.fnm_pserPreloaded);
      } else if (strExt==".wav" || strExt==".xwv") {
        _pSoundStock->Release((CSoundData*)fnm.fnm_pserPreloaded);
      } else if (strExt==".smc") {
        _pModelInstanceStock->Release((CModelInstanceSerial*)fnm.fnm_pserPreloaded);
      }
	  //0428 kwon 지우기
	  /* else if (strExt==".bm") {
        _pMeshStock->Release((CMesh*)fnm.fnm_pserPreloaded);
      } else if (strExt==".bs") {
        _pSkeletonStock->Release((CSkeleton*)fnm.fnm_pserPreloaded);
      } else if (strExt==".ba") {
        _pAnimSetStock->Release((CAnimSet*)fnm.fnm_pserPreloaded);
      } */else if (strExt==".ecl") {
        _pEntityClassStock->Release((CEntityClass*)fnm.fnm_pserPreloaded);
      } else if (strExt==".sha") {
        _pShaderStock->Release((CShader*)fnm.fnm_pserPreloaded);
      } else if (strExt==".ani") {
        _pAnimStock->Release((CAnimData*)fnm.fnm_pserPreloaded);
      } else if (strExt==".ogg" || strExt==".mp3" || strExt==".wma") {
        _pSoundStock->Release((CSoundData*)fnm.fnm_pserPreloaded);
      }
    }

    strm_afnmDictionary.Clear();
  }
}

static int qsortCompareCTFileName_ByIndex(const void *pv0, const void *pv1)
{
  return (const INDEX&)(**(const CTFileName**)pv0).fnm_pserPreloaded
        -(const INDEX&)(**(const CTFileName**)pv1).fnm_pserPreloaded;
}

void CTStream::DictionaryPreload_t(void)
{
  CTSingleLock sl(&_csStreams, TRUE);
  INDEX ctFileNames = strm_afnmDictionary.Count(); //0428 현재 401개 파일.
  
  // make a secondary container for the filenames
  CDynamicContainer<CTFileName> cfnmFiles;

  // for each filename
  {for(INDEX iFileName=0; iFileName<ctFileNames; iFileName++) {
    CTFileName &fnm = strm_afnmDictionary[iFileName];
    // find its index in the gro file
    (INDEX&)fnm.fnm_pserPreloaded = UNZIPGetFileIndex(fnm);
    // add the file to the container
    cfnmFiles.Add(&fnm);
  }}

  // sort the container
  if (ctFileNames>0) {
    qsort(cfnmFiles.sa_Array, ctFileNames, sizeof(CTFileName*), qsortCompareCTFileName_ByIndex);
  }

  // for each filename
  {for(INDEX iFileName=0; iFileName<ctFileNames; iFileName++) {
    // preload it
    CTFileName &fnm = cfnmFiles[iFileName];
    CTString strExt = fnm.FileExt();
    CallProgressHook_t((FLOAT)iFileName / ctFileNames);
    try {
      if (strExt==".tex") {
        fnm.fnm_pserPreloaded = _pTextureStock->Obtain_t(fnm);
      } else if (strExt==".mdl") {
        fnm.fnm_pserPreloaded = _pModelStock->Obtain_t(fnm);
      } else if (strExt==".wav" || strExt==".xwv") {
        fnm.fnm_pserPreloaded = _pSoundStock->Obtain_t(fnm);
      } else if (strExt==".smc") {
        fnm.fnm_pserPreloaded = _pModelInstanceStock->Obtain_t(fnm);
      }
	   //0428 kwon 지우기
	  /* else if (strExt==".bm") {
        fnm.fnm_pserPreloaded = _pMeshStock->Obtain_t(fnm);
      } else if (strExt==".bs") {
        fnm.fnm_pserPreloaded = _pSkeletonStock->Obtain_t(fnm);
      } else if (strExt==".ba") {
        fnm.fnm_pserPreloaded = _pAnimSetStock->Obtain_t(fnm);
      } */else if (strExt==".ecl") {
        fnm.fnm_pserPreloaded = _pEntityClassStock->Obtain_t(fnm);
      } else if (strExt==".sha") {
        fnm.fnm_pserPreloaded = _pShaderStock->Obtain_t(fnm);
      } else if (strExt==".ani") {
        fnm.fnm_pserPreloaded = _pAnimStock->Obtain_t(fnm);
      } else if (strExt==".ogg" || strExt==".mp3" || strExt==".wma") {
        CTFileName fnmActual;
        ExpandFilePath(EFP_READ,fnm,fnmActual);
        fnm.fnm_pserPreloaded = _pSoundStock->Obtain_t(fnmActual);
      }
    } catch (char *strError) {
		fnm.fnm_pserPreloaded = NULL;
		//CPrintF( TRANS("Cannot preload %s: %s\n"), (CTString&)fnm, strError);
		FatalError(TRANS("[%s] File is not exist!!!\n"), (CTString&)fnm, strError);
    }
  }}
}

/////////////////////////////////////////////////////////////////////////////
// General construction/destruction

/* Default constructor. */
CTStream::CTStream(void) : strm_ntDictionary(*new CNameTable_CTFileName)
	, strm_bPNG(false)
{
  CTSingleLock sl(&_csStreams, TRUE);
  strm_pubBufferBegin = NULL;
  strm_pubBufferEnd = NULL;
  strm_pubCurrentPos = NULL;
  strm_pubMaxPos = NULL;
  strm_pubEOF = NULL;
  strm_strStreamDescription = "";
  strm_slDictionaryPos = 0;
  strm_dmDictionaryMode = DM_NONE;

  m_bUseVirtualMem =TRUE;
}

void CTStream::FreeBuffer(void)
{
  CTSingleLock sl(&_csStreams, TRUE);
  // if we have some virtual memory allocated
  if( strm_pubBufferBegin != NULL)
  {
    // release it
	BOOL bSuccess = TRUE;
	if(m_bUseVirtualMem)
		bSuccess = VirtualFree( strm_pubBufferBegin, 0, MEM_RELEASE);
	else
		free(strm_pubBufferBegin);
    // if failed to release
    if( !bSuccess)
    {
      // report failure in releasing memory
      FatalError( TRANS("Unable to release virtual memory\n"));
    }
    strm_pubBufferBegin = NULL;
  }
}

/* Destructor. */
CTStream::~CTStream(void)
{
  CTSingleLock sl(&_csStreams, TRUE);

  FreeBuffer();

  strm_ntDictionary.Clear();
  strm_afnmDictionary.Clear();

  delete &strm_ntDictionary;
}

/*
 * Function that allocates given ammount of virtual address space
 */
void CTStream::AllocateVirtualMemory(ULONG ulBytesToAllocate)
{
  CTSingleLock sl(&_csStreams, TRUE);
  // reserve virtual memory of requested ammount
  strm_pubBufferBegin = (UBYTE *)VirtualAlloc( NULL, ulBytesToAllocate, MEM_RESERVE, PAGE_NOACCESS);
  if( strm_pubBufferBegin == NULL )
  {
	  m_bUseVirtualMem =FALSE;

	  strm_pubBufferBegin = (UBYTE*)malloc(ulBytesToAllocate);

	  if( strm_pubBufferBegin ==NULL )
		FatalError(TRANS("VirtualAlloc() & malloc() failed\n"));
  }
  // calculate end ptr
  strm_pubBufferEnd = strm_pubBufferBegin+ulBytesToAllocate;
  // reset current and max ptrs
  strm_pubCurrentPos = strm_pubBufferBegin;
  strm_pubMaxPos = strm_pubBufferBegin;
}

/*
 * Function that commits given page
 */
void CTStream::CommitPage(INDEX iPage)
{
  CTSingleLock sl(&_csStreams, TRUE);
  // commit requested page
  if( VirtualAlloc( strm_pubBufferBegin+iPage*_ulPageSize, _ulPageSize, MEM_COMMIT,
                    PAGE_READWRITE) == NULL)
  {
    // report failure in commiting page
    FatalError( TRANS("Unable to commit page: %s\n"), GetWindowsError( GetLastError()) );
  }
}

/*
 * Function that decommits given page
 */
void CTStream::DecommitPage(INDEX iPage)
{
  CTSingleLock sl(&_csStreams, TRUE);
  // decommit given page (decommiting page automaticly protects page from access)
  BOOL bSuccess = VirtualFree( strm_pubBufferBegin+iPage*_ulPageSize, _ulPageSize,
                               MEM_DECOMMIT);
  // if not successefull
  if( !bSuccess)
  {
    // report failure in decommiting page
    FatalError( TRANS("Unable to decommit page: %s\n"), GetWindowsError( GetLastError()) );
  }
}

/*
 * Function that pritects given page from writting (for read-only streams)
 */
void CTStream::ProtectPageFromWritting(INDEX iPage)
{
  CTSingleLock sl(&_csStreams, TRUE);
  DWORD dwLastProtection;
  // protect from writting given page
  BOOL bSuccess = VirtualProtect( strm_pubBufferBegin+iPage*_ulPageSize, _ulPageSize,
                                  PAGE_READONLY, &dwLastProtection);
  // if not successefull
  if( !bSuccess)
  {
    // report failure in protecting page
    FatalError( TRANS("Unable to protect page\n"));
  }
}

/////////////////////////////////////////////////////////////////////////////
// File stream opening/closing methods

/*
 * Default constructor.
 */
CTFileStream::CTFileStream(void)
{
  CTSingleLock sl(&_csStreams, TRUE);
  fstrm_pFile = NULL;
  fstrm_iLastAccessedPage = -1;
  // mark that file is created for writting
  fstrm_bReadOnly = TRUE;
  fstrm_iZipHandle = -1;
}

/*
 * Destructor.
 */
CTFileStream::~CTFileStream(void)
{
  CTSingleLock sl(&_csStreams, TRUE);
  // close stream
  if (fstrm_pFile != NULL || fstrm_iZipHandle!=-1) {
    Close();
  }
}

/*
 * Open an existing file.
 */
// throws char *
void CTFileStream::Open_t(const CTFileName &fnFileName, CTStream::OpenMode om/*=OM_READ*/)
{
	CTSingleLock sl(&_csStreams, TRUE);
	// if current thread has not enabled stream handling
	if (0 == _bThreadCanHandleStreams)
	{
		// error
		::ThrowF_t(TRANS("Cannot open file `%s', stream handling is not enabled for this thread"),
		  (CTString&)fnFileName);
	}

	// check parameters
	ASSERT(strlen(fnFileName)>0);
	// check that the file is not open
	ASSERT(fstrm_pFile==NULL && fstrm_iZipHandle==-1);

	// expand the filename to full path
	CTFileName fnmFullFileName;
	INDEX iFile = ExpandFilePath((om == OM_READ)?EFP_READ:EFP_WRITE, fnFileName, fnmFullFileName);

	// if read only mode requested
	if( om == OM_READ)
	{
		// initially, no physical file
		fstrm_pFile = NULL;
		// if zip file
		if( iFile==EFP_MODZIP || iFile==EFP_BASEZIP)
		{
			// open from zip
			fstrm_iZipHandle = UNZIPOpen_t(fnmFullFileName);
			ULONG ulFileSize = UNZIPGetSize(fstrm_iZipHandle);
			// allocate amount of memory needed to read whole subfile from group file into
			// memory plus page that could be accessed while reading long from "Page Border"-1
			// address
			AllocateVirtualMemory( (ulFileSize/_ulPageSize+2)*_ulPageSize);
			// remember EOF ptr
			strm_pubEOF = strm_pubBufferBegin+ulFileSize;
		}
		// if it is a physical file
		else if (iFile==EFP_FILE)
		{
			if(fil_iReportStats>=2)
			{
				CPrintF("      Open: %s\n", (const char*)fnmFullFileName);
			}
			// open file in read only mode
			fstrm_pFile = fopen(fnmFullFileName, "rb");
			// seek to the end of the physical disc file
			fseek( fstrm_pFile, 0, SEEK_END);
			// get current position in file (acctually file size)
			ULONG ulFileSize = ftell( fstrm_pFile);
			// seek back to the beginning of the file
			fseek( fstrm_pFile, 0, SEEK_SET);
			// allocate amount of memory needed to read whole file into memory (in case of file
			// containing 25 bytes (and page 10 bytes), we would have to allocate 40 bytes)
			AllocateVirtualMemory( (ulFileSize/_ulPageSize+2)*_ulPageSize);
			// remember EOF ptr
			strm_pubEOF = strm_pubBufferBegin+ulFileSize;
		}
		fstrm_bReadOnly = TRUE;
	}
	// if write mode requested
	else if( om == OM_WRITE)
	{
		// open file for reading and writting
		fstrm_pFile = fopen(fnmFullFileName, "rb+");
		fstrm_bReadOnly = FALSE;
		// allocate amount of memory needed to hold maximum allowed file lenght (when saving)
		// plus page that is allocated at the end of the file area (due to two-pages commiting
		// technique)
		AllocateVirtualMemory( _ulMaxLenghtOfSavingFile+_ulPageSize);
	}
	// if unknown mode
	else
	{
		FatalError(TRANS("File stream opening requested with unknown open mode: %d\n"), om);
	}

	// if openning operation was not successfull
	if(fstrm_pFile == NULL && fstrm_iZipHandle==-1)
	{
		CTFileName	fnmNewFileName;
		fnmNewFileName = fnFileName;
		if( fnmNewFileName.FindSubstr( CTString( "Textures\\" ) ) != -1 ||
			fnmNewFileName.FindSubstr( CTString( "TexturesMP\\" ) ) != -1 ||
			fnmNewFileName.FindSubstr( CTString( "Models\\" ) ) != -1 ||
			fnmNewFileName.FindSubstr( CTString( "ModelsMP\\" ) ) != -1 )
		{
			fnmNewFileName = CTString( "Data\\" ) + fnmNewFileName;

			// expand the filename to full path
			CTFileName fnmFullFileName;
			INDEX iFile = ExpandFilePath((om == OM_READ)?EFP_READ:EFP_WRITE, fnmNewFileName, fnmFullFileName);

			// if read only mode requested
			if( om == OM_READ)
			{
				// initially, no physical file
				fstrm_pFile = NULL;
				// if zip file
				if( iFile==EFP_MODZIP || iFile==EFP_BASEZIP)
				{
					// open from zip
					fstrm_iZipHandle = UNZIPOpen_t(fnmFullFileName);
					ULONG ulFileSize = UNZIPGetSize(fstrm_iZipHandle);
					// allocate amount of memory needed to read whole subfile from group file into
					// memory plus page that could be accessed while reading long from "Page Border"-1
					// address
					AllocateVirtualMemory( (ulFileSize/_ulPageSize+2)*_ulPageSize);
					// remember EOF ptr
					strm_pubEOF = strm_pubBufferBegin+ulFileSize;
				}
				// if it is a physical file
				else if (iFile==EFP_FILE)
				{
					if(fil_iReportStats>=2)
					{
						CPrintF("      Open: %s\n", (const char*)fnmFullFileName);
					}
					// open file in read only mode
					fstrm_pFile = fopen(fnmFullFileName, "rb");
					// seek to the end of the physical disc file
					fseek( fstrm_pFile, 0, SEEK_END);
					// get current position in file (acctually file size)
					ULONG ulFileSize = ftell( fstrm_pFile);
					// seek back to the beginning of the file
					fseek( fstrm_pFile, 0, SEEK_SET);
					// allocate amount of memory needed to read whole file into memory (in case of file
					// containing 25 bytes (and page 10 bytes), we would have to allocate 40 bytes)
					AllocateVirtualMemory( (ulFileSize/_ulPageSize+2)*_ulPageSize);
					// remember EOF ptr
					strm_pubEOF = strm_pubBufferBegin+ulFileSize;
				}
				fstrm_bReadOnly = TRUE;
			}
			// if write mode requested
			else if( om == OM_WRITE)
			{
				// open file for reading and writting
				fstrm_pFile = fopen(fnmFullFileName, "rb+");
				fstrm_bReadOnly = FALSE;
				// allocate amount of memory needed to hold maximum allowed file lenght (when saving)
				// plus page that is allocated at the end of the file area (due to two-pages commiting
				// technique)
				AllocateVirtualMemory( _ulMaxLenghtOfSavingFile+_ulPageSize);
			}
			// if unknown mode
			else
			{
				FatalError(TRANS("File stream opening requested with unknown open mode: %d\n"), om);
			}

			// if openning operation was not successfull
			if(fstrm_pFile == NULL && fstrm_iZipHandle==-1)
			{
				// throw exception
				Throw_t(TRANS("Cannot open file `%s' (%s)"), (CTString&)fnmFullFileName,
						strerror(errno));
			}
		}
		else
		{
			// throw exception
			Throw_t(TRANS("Cannot open file `%s' (%s)"), (CTString&)fnmFullFileName,
					strerror(errno));
		}
	}

	// if file opening was successefull, set stream description to file name
	strm_strStreamDescription = fnFileName;
	// add this newly opened file into opened stream list
	_plhOpenedStreams->AddTail( strm_lnListNode);
	// notify progress hook of new file
	//SetProgressFile(fnFileName);	// 로딩 개선
}

static BOOL CreateDirectoryPath(const CTFileName &fnFileName)
{
  CTFileName fnTemp = fnFileName;

  try {
    fnTemp.RemoveApplicationPath_t();
  } catch(char *) {
  }
  // remove ending backslash
  CTString strFullPath = fnTemp.FileDir();

  CTString strExistingPath;
  CTString strNextPath;
  INDEX iExisting = 0;
  const char *pstrCurrent  = (const char*)strFullPath;
  const char *pstrFullPath = (const char*)strFullPath;

  while(TRUE) {
    pstrCurrent = strchr(pstrCurrent,'\\');

    if(pstrCurrent==NULL) {
      break;
    } else {
      pstrCurrent++;
    }

    INDEX iExisting = pstrCurrent-pstrFullPath;
    strFullPath.Split(iExisting,strExistingPath,strNextPath);

    // if next directory does not exist
    if(GetFileAttributes((const char*)_fnmApplicationPath+strExistingPath) == (-1)) {
      // Create it
      if(CreateDirectory((const char*)_fnmApplicationPath+strExistingPath,NULL)==0) {
        // if failed return FALSE
        return FALSE;
      }
    }
  }
  return TRUE;
}
/*
 * Create a new file or overwrite existing.
 */
void CTFileStream::Create_t(const CTFileName &fnFileName,
                            enum CTStream::CreateMode cm) // throws char *
{
  CTSingleLock sl(&_csStreams, TRUE);
  (void)cm; // OBSOLETE!

  // if current thread has not enabled stream handling
  if (0 == _bThreadCanHandleStreams) {
    // error
    ::ThrowF_t(TRANS("Cannot create file `%s', stream handling is not enabled for this thread"),
      (CTString&)fnFileName);
  }

  CTFileName fnmFullFileName;
  INDEX iFile = ExpandFilePath(EFP_WRITE, fnFileName, fnmFullFileName);

  // check parameters
  ASSERT(strlen(fnFileName)>0);
  // check that the file is not open
  ASSERT(fstrm_pFile == NULL);
  // open file stream for writting (destroy file context if file existed before)
  fstrm_pFile = fopen(fnmFullFileName, "wb+");
  // if not successfull
  if(fstrm_pFile == NULL) {
    // if directory for file does not exist
    ULONG ulError = GetLastError();
    if(ulError == ERROR_PATH_NOT_FOUND) {
      // if creation of directory path succeeded
      if(CreateDirectoryPath(fnmFullFileName)) {
        // open file stream for writting
        fstrm_pFile = fopen(fnmFullFileName, "wb+");
        // if still not successfull
        if(fstrm_pFile == NULL) {
          // throw exception
          Throw_t(TRANS("Cannot create file `%s' (%s)"), (CTString&)fnmFullFileName, strerror(errno));
        }
      }
    // else if some other error
    } else {
      // throw exception
      Throw_t(TRANS("Cannot create file `%s' (%s)"), (CTString&)fnmFullFileName, strerror(errno));
    }
  }
  // allocate amount of memory needed to hold maximum allowed file lenght (when saving)
  // plus page that is allocated at the end of the file area (due to two-pages commiting
  // technique)
  AllocateVirtualMemory( _ulMaxLenghtOfSavingFile+_ulPageSize);
  // if file creation was successefull, set stream description to file name
  strm_strStreamDescription = fnFileName;
  // mark that file is created for writting
  fstrm_bReadOnly = FALSE;
  // add this newly created file into opened stream list
  _plhOpenedStreams->AddTail( strm_lnListNode);
}

void CTFileStream::SetFileSize_t(SLONG slSize)
{
  CTSingleLock sl(&_csStreams, TRUE);
  // Get current position in file
  SLONG slOriginalPos = GetStreamSize();
  ASSERT(!fstrm_bReadOnly);
  try {
    // Get stream size
    if(slOriginalPos>=slSize) {
      Throw_t("Error while setting file size to %d\n");
    }
    // Write something at requested address
    SetPos_t(slSize-1);
    UBYTE ubDummy = 0;
    Write_t(&ubDummy,sizeof(UBYTE));
    // Restore original position in file
    SetPos_t(slOriginalPos);
  } catch(char *) {
    // Restore original position in file
    SetPos_t(slOriginalPos);
    // throw exception
    throw;
  }
}

void CTFileStream::WritePageToFile(INDEX iPageToWrite)
{
  CTSingleLock sl(&_csStreams, TRUE);
  // must be writeable
  ASSERT( fstrm_bReadOnly == FALSE);
  // update max pos with current position
  UpdateMaxPos();
  // set that we will write whole page
  ULONG ctBytesToWrite = _ulPageSize;
  // calculate last commited page starting offset
  UBYTE *pubCommitedPageStart = strm_pubBufferBegin+iPageToWrite*_ulPageSize;
  // if commited page start is behind max pos, don't write page
  if( pubCommitedPageStart>=strm_pubMaxPos)
  {
    return;
  }
  // if max pos is inside last commited page
  if( (strm_pubMaxPos>=pubCommitedPageStart) &&
      (strm_pubMaxPos<(pubCommitedPageStart+_ulPageSize)) )
  {
    // we will write only limited ammount of data (not whole page)
    ctBytesToWrite = strm_pubMaxPos-pubCommitedPageStart;
  }
  fpos_t posInFile = pubCommitedPageStart-strm_pubBufferBegin;
  // move file ptr to place inside file where we must write last commited page
  fsetpos( fstrm_pFile, &posInFile);
  // write last commited page
  ULONG ctWriten = fwrite( pubCommitedPageStart, 1, ctBytesToWrite, fstrm_pFile);
  // If all bytes weren't writen
  if(ctWriten!=ctBytesToWrite) {
    NotifyWritingError(GetDescription());
  }
  // flush file stream
  fflush( fstrm_pFile);
}

/*
 * Close an open file.
 */
void CTFileStream::Close(void)
{
  CTSingleLock sl(&_csStreams, TRUE);
  // if file is not open
  if (fstrm_pFile==NULL && fstrm_iZipHandle==-1) {
    ASSERT(FALSE);
    return;
  }

  // if any page was accessed and file was opened for writting
  if( (fstrm_iLastAccessedPage != -1) && (!fstrm_bReadOnly) )
  {
    // writte pair of last commited pages into file
    WritePageToFile( fstrm_iLastAccessedPage);
    WritePageToFile( fstrm_iLastAccessedPage+1);
  }
  // clear commited page indexes
  fstrm_iLastAccessedPage = -1;
  // clear stream description
  strm_strStreamDescription = "";
  // remove file from list of curently opened streams
  strm_lnListNode.Remove();

  // if file on disk
  if (fstrm_pFile != NULL) {
    // close file
    fclose( fstrm_pFile);
    fstrm_pFile = NULL;
  // if file in zip
  } else if (fstrm_iZipHandle>=0) {
    // close zip entry
    UNZIPClose(fstrm_iZipHandle);
    fstrm_iZipHandle = -1;
  }

  // free virtual memory used up by the buffer
  FreeBuffer();

  // clear dictionary vars
  strm_dmDictionaryMode = DM_NONE;
  strm_ntDictionary.Clear();
  strm_afnmDictionary.Clear();
  strm_slDictionaryPos=0;
}

void CTFileStream::FileCommitPage( INDEX iPageToCommit)
{
  CTSingleLock sl(&_csStreams, TRUE);
  // commit requested page
  CommitPage( iPageToCommit);
  // calculate accessed page adress
  UBYTE *pubCommitingPageStart = strm_pubBufferBegin+iPageToCommit*_ulPageSize;
  // if accessed page is beyond pages that mapped file occupies in memory
  if( pubCommitingPageStart>=strm_pubBufferEnd || 
    !IsWriteable() && pubCommitingPageStart>=strm_pubEOF)
  {
    // don't read from file
    return;
  }

  // if file on disk
  if (fstrm_pFile != NULL) {
    // seek to the correct file position iside physical file where wanted page data starts
    fseek( fstrm_pFile,pubCommitingPageStart-strm_pubBufferBegin,SEEK_SET);
    // temporary save read-only flag because we need to write over this block of memory
    // (to read file into memory)
    BOOL bReadOnly = fstrm_bReadOnly;
    fstrm_bReadOnly = FALSE;
    // read one page of data from file into commited page
//    CPrintF("           read: 0x%08x\n", pubCommitingPageStart);
    SLONG slRead = fread( pubCommitingPageStart, 1, _ulPageSize, fstrm_pFile);
    // if not all bytes were read
    if(slRead!=_ulPageSize) {
      // Test file for error
      if(ferror(fstrm_pFile)) {
        NotifyReadingError(GetDescription());
      }
    }
    _ctTotalFromDisk+=slRead;
    _ctTotalEffective+=slRead;
    // restore read-only flag
    fstrm_bReadOnly = bReadOnly;
  // if file in zip
  } else if (fstrm_iZipHandle >=0) {
    // read from zip
    try {
      UNZIPReadBlock_t(fstrm_iZipHandle, pubCommitingPageStart, 
        pubCommitingPageStart-strm_pubBufferBegin, _ulPageSize);
    } catch (char *strError) {
      CPrintF(TRANS("Cannot commit from zip: %s\n"), strError);
    }
  }
}

void CTFileStream::FileDecommitPage( INDEX iPageToDecommit)
{
  CTSingleLock sl(&_csStreams, TRUE);
  // if file was opened for writting
  if( !fstrm_bReadOnly)
  {
    // writte last commited page into file
    WritePageToFile( iPageToDecommit);
  }
  DecommitPage( iPageToDecommit);
}

/* Prepare data to be read later */
void CTFileStream::FileReadAhead_t(SLONG slOffset, SLONG slReadSize)
{
  // read ahead works only for files in zip arhives
  if(fstrm_iZipHandle>=0) {
    UNZIPReadAheadBlock_t(fstrm_iZipHandle,slOffset,slReadSize);
  }
}

/* Get CRC32 of stream */
ULONG CTFileStream::GetStreamCRC32_t(void)
{
  CTSingleLock sl(&_csStreams, TRUE);
  // if file on disk
  if (fstrm_pFile != NULL) {
    // use base class implementation (really calculates the CRC)
    return CTStream::GetStreamCRC32_t();
  // if file in zip
  } else if (fstrm_iZipHandle >=0) {
    return UNZIPGetCRC(fstrm_iZipHandle);
  } else {
    ASSERT(FALSE);
    return 0;
  }
}

void CTFileStream::HandleAccess(INDEX iAccessedPage, BOOL bReadAttempted)
{
  CTSingleLock sl(&_csStreams, TRUE);
  // we can't have access violation on pages that are commited
  ASSERT( fstrm_iLastAccessedPage != iAccessedPage);

  // if file stream is marked as read only and write access attempted
  if( fstrm_bReadOnly && !bReadAttempted)
  {
    // report error
    throw( TRANS("Write attempted on read-only file stream!"));
  }

  // if we are committing pages for the first time
  if( fstrm_iLastAccessedPage == -1)
  {
    // commit access-violated page
    FileCommitPage( iAccessedPage);
    // commit page following accessed one (because of pair-commiting method)
    FileCommitPage( iAccessedPage+1);
    // remember last accessed page
    fstrm_iLastAccessedPage = iAccessedPage;
  }
  // if we were commiting pages before
  else
  {
    // *
    // * all comments use example of file reading, situation is |-|la|la+1|-|-
    // * pages numbers are in brackets, '-' means not commited, 'la' means last accessed
    // * 'ap' means accessed page, 'C' means 'to commit', D means 'to decommit'
    // * our goal is to have newly accessed page and its following page commited
    // * we can notice three different cases (ap=la is not allowed!):
    // * a) when ap=la+2:  to do: [la]D       [ap+1]C,  la = ap-1
    // * b) when la=ap+1:  to do: [la+1]D     [ap]C,    la = ap
    // * c) other cases:   to do: [la,la+1]D  [ap,ap+1]C, la = ap

    // case a)
    if( iAccessedPage==(fstrm_iLastAccessedPage+2) )
    {
      FileDecommitPage( fstrm_iLastAccessedPage);
      FileCommitPage( iAccessedPage);
      fstrm_iLastAccessedPage = iAccessedPage-1;
    }
    // case b)
    else if( fstrm_iLastAccessedPage==(iAccessedPage+1) )
    {
      FileDecommitPage( fstrm_iLastAccessedPage+1);
      FileCommitPage( iAccessedPage);
      fstrm_iLastAccessedPage = iAccessedPage;
    }
    // case c)
    else
    {
      FileDecommitPage( fstrm_iLastAccessedPage);
      FileDecommitPage( fstrm_iLastAccessedPage+1);
      FileCommitPage( iAccessedPage);
      FileCommitPage( iAccessedPage+1);
      fstrm_iLastAccessedPage = iAccessedPage;
    }
  }

  // if file was opened in read only mode
  if( fstrm_bReadOnly)
  {
    // protect pages from writting
    ProtectPageFromWritting( fstrm_iLastAccessedPage);
    ProtectPageFromWritting( fstrm_iLastAccessedPage+1);
  }
}

/////////////////////////////////////////////////////////////////////////////
// Memory stream construction/destruction

/*
 * Create dynamically resizing stream for reading/writing.
 */
CTMemoryStream::CTMemoryStream(void)
{
  CTSingleLock sl(&_csStreams, TRUE);
  // if current thread has not enabled stream handling
  if (0 == _bThreadCanHandleStreams) {
    // error
    ::FatalError(TRANS("Can create memory stream, stream handling is not enabled for this thread"));
  }

  // allocate amount of memory needed to hold maximum allowed file lenght (when saving)
  AllocateVirtualMemory( _ulMaxLenghtOfSavingFile);
  mstrm_ctLocked = 0;
  mstrm_bReadable = TRUE;
  mstrm_bWriteable = TRUE;
  // set stream description
  strm_strStreamDescription = "dynamic memory stream";
  // add this newly created memory stream into opened stream list
  _plhOpenedStreams->AddTail( strm_lnListNode);
}

/*
 * Create static stream from given buffer.
 */
CTMemoryStream::CTMemoryStream(void *pvBuffer, SLONG slSize,
                               CTStream::OpenMode om /*= CTStream::OM_READ*/)
{
  CTSingleLock sl(&_csStreams, TRUE);
  // if current thread has not enabled stream handling
  if (0 == _bThreadCanHandleStreams) {
    // error
    ::FatalError(TRANS("Can create memory stream, stream handling is not enabled for this thread"));
  }

  // allocate amount of memory needed to hold maximum allowed file lenght (when saving)
  AllocateVirtualMemory( _ulMaxLenghtOfSavingFile);
  // copy given block of memory into memory file
  memcpy( strm_pubCurrentPos, pvBuffer, slSize);

  mstrm_ctLocked = 0;
  mstrm_bReadable = TRUE;
  // if stram is opened in read only mode
  if( om == OM_READ)
  {
    mstrm_bWriteable = FALSE;
  }
  // otherwise, write is enabled
  else
  {
    mstrm_bWriteable = TRUE;
  }
  // set stream description
  strm_strStreamDescription = "dynamic memory stream";
  // add this newly created memory stream into opened stream list
  _plhOpenedStreams->AddTail( strm_lnListNode);
}

/* Destructor. */
CTMemoryStream::~CTMemoryStream(void)
{
  CTSingleLock sl(&_csStreams, TRUE);
  ASSERT(mstrm_ctLocked==0);
  // remove memory stream from list of curently opened streams
  strm_lnListNode.Remove();
}

/////////////////////////////////////////////////////////////////////////////
// Memory stream buffer operations

/*
 * Lock the buffer contents and it's size.
 */
void CTMemoryStream::LockBuffer(void **ppvBuffer, SLONG *pslSize)
{
  CTSingleLock sl(&_csStreams, TRUE);
  mstrm_ctLocked++;
  ASSERT(mstrm_ctLocked>0);
  UpdateMaxPos();

  *ppvBuffer = strm_pubBufferBegin;
  *pslSize = strm_pubMaxPos - strm_pubBufferBegin;
}

/*
 * Unlock buffer.
 */
void CTMemoryStream::UnlockBuffer()
{
  CTSingleLock sl(&_csStreams, TRUE);
  mstrm_ctLocked--;
  ASSERT(mstrm_ctLocked>=0);
}

/////////////////////////////////////////////////////////////////////////////
// Memory stream overrides from CTStream

BOOL CTMemoryStream::IsReadable(void)
{
  return mstrm_bReadable && (mstrm_ctLocked==0);
}
BOOL CTMemoryStream::IsWriteable(void)
{
  return mstrm_bWriteable && (mstrm_ctLocked==0);
}
BOOL CTMemoryStream::IsSeekable(void)
{
  return TRUE;
}

void CTMemoryStream::HandleAccess(INDEX iAccessedPage, BOOL bReadAttempted)
{
  CTSingleLock sl(&_csStreams, TRUE);
  // if memory stream is locekd, neather read nor write are allowed
  ASSERT( mstrm_ctLocked==0);
  // if memory stream is not writeable and write attempted
  if( !IsWriteable() && !bReadAttempted)
  {
    throw( TRANS("Write attempted on read-only memory stream!"));
  }
  // commit access-violated page
  CommitPage( iAccessedPage);
}

// Test if a file exists.
BOOL FileExists(const CTFileName &fnmFile)
{
  // if no file
  if (fnmFile=="") {
    // it doesn't exist
    return FALSE;
  }
  // try to
  try {
    // open the file for reading
    CTFileStream strmFile;
    strmFile.Open_t(fnmFile);
    // if successful, it means that it exists,
    return TRUE;
  // if failed, it means that it doesn't exist
  } catch (char *strError) {
    (void) strError;
    return FALSE;
  }
}

// Test if a file exists for writing. 
// (this is can be diferent than normal FileExists() if a mod uses basewriteexclude.lst
BOOL FileExistsForWriting(const CTFileName &fnmFile)
{
  // if no file
  if (fnmFile=="") {
    // it doesn't exist
    return FALSE;
  }
  // expand the filename to full path for writing
  CTFileName fnmFullFileName;
  INDEX iFile = ExpandFilePath(EFP_WRITE, fnmFile, fnmFullFileName);

  // check if it exists
  FILE *f = fopen(fnmFullFileName, "rb");
  if (f!=NULL) { 
    fclose(f);
    return TRUE;
  } else {
    return FALSE;
  }
}

// Get file timestamp
SLONG GetFileTimeStamp_t(const CTFileName &fnm)
{
  // expand the filename to full path
  CTFileName fnmExpanded;
  INDEX iFile = ExpandFilePath(EFP_READ, fnm, fnmExpanded);
  if (iFile!=EFP_FILE) {
    return FALSE;
  }

  int file_handle;
  // try to open file for reading
  file_handle = _open( fnmExpanded, _O_RDONLY | _O_BINARY);
  if(file_handle==-1) {
    ThrowF_t(TRANS("Cannot open file '%s' for reading"), CTString(fnm));
    return -1;
  }
  struct stat statFileStatus;
  // get file status
  fstat( file_handle, &statFileStatus);
  _close( file_handle);
  ASSERT(statFileStatus.st_mtime<=time(NULL));
  return statFileStatus.st_mtime;
}

// Get CRC32 of a file
ULONG GetFileCRC32_t(const CTFileName &fnmFile) // throw char *
{
  // if the file exists in a zip
  INDEX iInZip = UNZIPGetFileIndex(fnmFile);
  if (iInZip>=0) {
    // just get the crc from the zip file entry
    return UNZIPGetCRCByIndex(iInZip);
  }

  // open the file
  CTFileStream fstrm;
  fstrm.Open_t(fnmFile);
  // return the checksum
  return fstrm.GetStreamCRC32_t();
}

// Test if a file is read only (also returns FALSE if file does not exist)
BOOL IsFileReadOnly(const CTFileName &fnm)
{
  // expand the filename to full path
  CTFileName fnmExpanded;
  INDEX iFile = ExpandFilePath(EFP_READ, fnm, fnmExpanded);
  if (iFile!=EFP_FILE) {
    return FALSE;
  }

  int file_handle;
  // try to open file for reading
  file_handle = _open( fnmExpanded, _O_RDONLY | _O_BINARY);
  if(file_handle==-1) {
    return FALSE;
  }
  struct stat statFileStatus;
  // get file status
  fstat( file_handle, &statFileStatus);
  _close( file_handle);
  ASSERT(statFileStatus.st_mtime<=time(NULL));
  return !(statFileStatus.st_mode&_S_IWRITE);
}

// Delete a file
BOOL CTDeleteFile(const CTFileName &fnmFile)
{
  // expand the filename to full path
  CTFileName fnmExpanded;
  INDEX iFile = ExpandFilePath(EFP_WRITE, fnmFile, fnmExpanded);
  if (iFile==EFP_FILE) {
    int ires = remove(fnmExpanded);
    return ires==0;
  } else {
    return FALSE;
  }
}

// Move a file
BOOL CTMoveFile(const CTFileName &fnmFileOld, const CTFileName &fnmFileNew)
{
  // expand the filename to full path
  CTFileName fnmExpandedOld;
  CTFileName fnmExpandedNew;
  INDEX iFileOld = ExpandFilePath(EFP_WRITE, fnmFileOld, fnmExpandedOld);
  INDEX iFileNew = ExpandFilePath(EFP_WRITE, fnmFileNew, fnmExpandedNew);
  if(iFileOld==EFP_FILE && iFileNew==EFP_FILE) {
    int ires = rename(fnmExpandedOld,fnmExpandedNew);
    return ires==0;
  } else {
    return FALSE;
  }
}


static BOOL IsFileReadable_internal(const CTFileName &fnmFullFileName)
{
  FILE *pFile = fopen(fnmFullFileName, "rb");
  if (pFile!=NULL) {
    fclose(pFile);
    return TRUE;
  } else {
    return FALSE;
  }
}

// check for some file extensions that can be substituted
static BOOL SubstExt_internal(CTFileName &fnmFullFileName)
{
  if(fnmFullFileName.FileExt()==".mp3") {
    fnmFullFileName = fnmFullFileName.NoExt()+".ogg";
    return TRUE;
  } else if (fnmFullFileName.FileExt()==".ogg") {
    fnmFullFileName = fnmFullFileName.NoExt()+".mp3";
    return TRUE;
  } else {
    return TRUE;
  }
}


static INDEX ExpandFilePath_read(ULONG ulType, const CTFileName &fnmFile, CTFileName &fnmExpanded)
{
  // search for the file in zips
  INDEX iFileInZip = UNZIPGetFileIndex(fnmFile);

  // if a mod is active
  if (_fnmMod!="") {

    // first try in the mod's dir
    if (!fil_bPreferZips) {
      fnmExpanded = _fnmApplicationPath+_fnmMod+fnmFile;
      if (IsFileReadable_internal(fnmExpanded)) {
        return EFP_FILE;
      }
    }

    // if not disallowing zips
    if (!(ulType&EFP_NOZIPS)) {
      // if exists in mod's zip
      if (iFileInZip>=0 && UNZIPIsFileAtIndexMod(iFileInZip)) {
        // use that one
        fnmExpanded = fnmFile;
        return EFP_MODZIP;
      }
    }

    // try in the mod's dir after
    if (fil_bPreferZips) {
      fnmExpanded = _fnmApplicationPath+_fnmMod+fnmFile;
      if (IsFileReadable_internal(fnmExpanded)) {
        return EFP_FILE;
      }
    }
  }

  // try in the app's base dir
  if (!fil_bPreferZips) {
    fnmExpanded = _fnmApplicationPath+fnmFile;
    if (IsFileReadable_internal(fnmExpanded)) {
      return EFP_FILE;
    }
  }

  // if not disallowing zips
  if (!(ulType&EFP_NOZIPS)) {
    // if exists in any zip
    if (iFileInZip>=0) {
      // use that one
      fnmExpanded = fnmFile;
      return EFP_BASEZIP;
    }
  }

  // try in the app's base dir
  if (fil_bPreferZips) {
    fnmExpanded = _fnmApplicationPath+fnmFile;
    if (IsFileReadable_internal(fnmExpanded)) {
      return EFP_FILE;
    }
  }

  // finally, try in the CD path
  if (_fnmCDPath!="") {

    // if a mod is active
    if (_fnmMod!="") {
      // first try in the mod's dir
      fnmExpanded = _fnmCDPath+_fnmMod+fnmFile;
      if (IsFileReadable_internal(fnmExpanded)) {
        return EFP_FILE;
      }
    }

    fnmExpanded = _fnmCDPath+fnmFile;
    if (IsFileReadable_internal(fnmExpanded)) {
      return EFP_FILE;
    }
  }
  return EFP_NONE;
}


// Get replacement file name
static CTFileName GetFileReplacement( const CTFileName &fnm)
{

  // Replace smc files with bmc files
  extern INDEX ska_bReplaceSmcWithBmc;
  if( fnm.FileExt()==".smc" && ska_bReplaceSmcWithBmc) {
    return fnm.NoExt() + ".bmc";
  }
  return fnm;
}

// Expand a file's filename to full path
INDEX ExpandFilePath( ULONG ulType, const CTFileName &fnmOriginal, CTFileName &fnmExpanded)
{
  const CTFileName fnmFile = GetFileReplacement(fnmOriginal);

  // if this is a fully qualified path
  if( (fnmFile[0]!=0 && fnmFile[1]==':')
    ||(fnmFile[0]=='\\' && fnmFile[1]=='\\')) {
    if((ulType&EFP_WRITE) || IsFileReadable_internal(fnmFile)) {
      // use it directly
      fnmExpanded = fnmFile;
      return EFP_FILE;
    }
  }

  // if writing
  if (ulType&EFP_WRITE) {
    // if should write to mod dir
    if (_fnmMod!="" && (!FileMatchesList(_afnmBaseWriteInc, fnmFile) || FileMatchesList(_afnmBaseWriteExc, fnmFile))) {
      // do that
      fnmExpanded = _fnmApplicationPath+_fnmMod+fnmFile;
      return EFP_FILE;
    // if should not write to mod dir
    } else {
      // write to base dir
      fnmExpanded = _fnmApplicationPath+fnmFile;
      return EFP_FILE;
    }

  // if reading
  } else if (ulType&EFP_READ) {

    // check for expansions for reading
    INDEX iRes = ExpandFilePath_read(ulType, fnmFile, fnmExpanded);
    // if not found
    if (iRes==EFP_NONE) {
      //check for some file extensions that can be substituted
      CTFileName fnmSubst = fnmFile;
      if (SubstExt_internal(fnmSubst)) {
        iRes = ExpandFilePath_read(ulType, fnmSubst, fnmExpanded);
      }
    }

    if (iRes!=EFP_NONE) {
      return iRes;
    }

  // in other cases
  } else  {
    ASSERT(FALSE);
    fnmExpanded = _fnmApplicationPath+fnmFile;
    return EFP_FILE;
  }

  fnmExpanded = _fnmApplicationPath+fnmFile;
  return EFP_NONE;
}

extern __int64 GetDiskFreeSpace(const CTString &strDirectory)
{
  ULARGE_INTEGER ullFreeSize;
  ULARGE_INTEGER ullTotalSize;
  ullFreeSize.QuadPart = 0;
  BOOL bSuccess = GetDiskFreeSpaceEx((const char*)strDirectory,&ullFreeSize,&ullTotalSize,NULL);
  ASSERT(bSuccess);
  return ullFreeSize.QuadPart;
}




/*
 * File error handling
 */
// Nofify that error accurred while reading
extern void NotifyReadingError(const CTFileName &fnmFile)
{
  // Call error reading callback function
  if(_pErrorReadingFile!=NULL) {
    _pErrorReadingFile(fnmFile);
  }
}

// Nofify that error accurred while writing
extern void NotifyWritingError(const CTFileName &fnmFile)
{
  // Call error writing callback function
  if(_pErrorWritingFile!=NULL) {
    _pErrorWritingFile(fnmFile);
  }
}

// Set callback function that will be called on read error
extern void SetErrorReadingCallback(void(*pErrorReadingFile)(const CTFileName &fnmFile))
{
  _pErrorReadingFile = pErrorReadingFile;
}

// Set callback function that will be called on write error
extern void SetErrorWritingCallback(void(*pErrorWritingFile)(const CTFileName &fnmFile))
{
  _pErrorWritingFile = pErrorWritingFile;
}

// Default function that will be caller on reading error
static void DefaultErrorReadingCallback(const CTFileName &fnmFile)
{
  FatalError("Error reading from file '%s'", (const char*)fnmFile);
}

// Default function that will be caller on writing error
static void DefaultErrorWritingCallback(const CTFileName &fnmFile)
{
  FatalError("Error writing in file '%s'", (const char*)fnmFile);
}

