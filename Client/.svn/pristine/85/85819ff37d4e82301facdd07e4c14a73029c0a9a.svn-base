#include "stdafx.h"
#include "DecodeReport.h"

static void SubMain( int argc, char *argv[]);
static CTFileName _fnSymbolsDir;
static CTFileStream _strmDst;

static CTString _strRefVarName = "";
static ULONG _ulRefVarAbsAddress = -1;
static ULONG _ulRefVarLogAddress = -1;
static ULONG _ulRefVarLogSection = -1;

static CStaticStackArray<MapFile> _amfMapFiles; // Array of preloaded map files

int main( int argc, char *argv[])
{
  CTSTREAM_BEGIN {
    SubMain(argc, argv);
  } CTSTREAM_END;
  return 0;
}

static INDEX FindImage(const CTString &strImage)
{
  INDEX ctmf = _amfMapFiles.Count();
  for(INDEX imf=0;imf<ctmf;imf++) {
    MapFile &mf = _amfMapFiles[imf];
    if(mf.mf_strImage == strImage) {
      return imf;
    }
  }
  return -1;
}

static void SortMapFile(MapFile &mf)
{
  // undone
}

static INDEX PreloadMapFile(const CTString &strImage)
{
  INDEX imf = FindImage(strImage);
  // if allready preloaded
  if(imf!=(-1)) {
    // bail out
    return imf;
  }

  // Add new map file
  MapFile &mf = _amfMapFiles.Push();
  mf.mf_strImage = strImage;
  mf.mf_aamAddresses.SetAllocationStep(32768);

  CTFileName fnmMap = _fnSymbolsDir+((CTFileName)strImage).FileName()+".map";

  try {
    CTFileStream strmMap;
    strmMap.Open_t(fnmMap, CTStream::OM_READ);
    // find beginning of functions in map file
    for(;;) {
      if (strmMap.AtEOF()) {
        return -1;
      }
      // read the line
      CTString strLine;
      strmMap.GetLine_t(strLine);
      if (strncmp(strLine, "  Address", 9)==0) {
        break;
      }
    }

    CTString strEmpty;
    strmMap.GetLine_t(strEmpty);

    while (!strmMap.AtEOF()) {
      // read the line
      CTString strLine;
      strmMap.GetLine_t(strLine);
      char strFunctionLine[1024] = {0};
      ULONG ulSegLine=-1;
      ULONG ulOfsLine=-1;
      strLine.ScanF("%x:%x %s", &ulSegLine, &ulOfsLine, strFunctionLine);
      // Add new address mapping
      AddressMapping &am = mf.mf_aamAddresses.Push();
      am.am_strFunction = strFunctionLine;
      am.am_ulSection = ulSegLine;
      am.am_ulOffset = ulOfsLine;
    }
  } catch (char *strError) {
    (void)strError;
    return -1;
  }
  
  // Sort map file 
  SortMapFile(mf);

  return FindImage(strImage);
}

static void FindLogAddressInMapFile(const CTString &strImage, ULONG ulSection, ULONG ulOff, CTString &strFunction, SLONG &slDelta)
{
  CTFileName fnmMap = _fnSymbolsDir+((CTFileName)strImage).FileName()+".map";

  // Default return values
  strFunction = CTString(0, "<not found in '%s'>", (const char*)fnmMap);
  slDelta = 0;

  // preload map file if required
  INDEX imf = PreloadMapFile(strImage);
  if(imf==(-1)) {
    return;
  }

  const MapFile &mf = _amfMapFiles[imf];
  INDEX ctam=mf.mf_aamAddresses.Count();
  INDEX iamLast = -1;

  // for each address mapping in map file
  for(INDEX iam=0;iam<ctam;iam++) {
    const AddressMapping &am = mf.mf_aamAddresses[iam];
    if(am.am_ulSection!=ulSection) {
      continue;
    }
    if(am.am_ulOffset>ulOff) {
      // if last image was valid
      if(iamLast!=(-1)) {
        // store last function before return
        strFunction = mf.mf_aamAddresses[iamLast].am_strFunction;
      }
      return;
    }
    iamLast = iam;
    slDelta = ulOff-am.am_ulOffset;
  }
  // if last image was valid
  if(iamLast!=(-1)) {
    // store last function before return
    strFunction = mf.mf_aamAddresses[iamLast].am_strFunction;
  }
}

static INDEX FindAddressMappingOfVariable(const char *strRefVar, INDEX imf)
{
  const MapFile &mf = _amfMapFiles[imf];
  INDEX ctam = mf.mf_aamAddresses.Count();
  // for each address mapping
  for(INDEX iam=0;iam<ctam;iam++) {
    const AddressMapping &am = mf.mf_aamAddresses[iam];
    const char *pstrRef = strstr(am.am_strFunction, strRefVar);
    // found it
    if(pstrRef!=NULL) {
      return iam;
    }
  }
  return -1;
}

static void FindInMapFileAbsolute(const CTString &strImage, ULONG ulAbsAddress, CTString &strFunction, SLONG &slDelta)
{
  CTFileName fnmMap = _fnSymbolsDir+((CTFileName)strImage).FileName()+".map";

  // Default return values
  strFunction = CTString(0, "<not found in '%s'>", (const char*)fnmMap);
  slDelta = 0;
  // preload map file if required
  INDEX imf = PreloadMapFile(strImage);
  if(imf==(-1)) {
    return;
  }

  // if no ref var
  if(_strRefVarName=="" || _ulRefVarAbsAddress==(-1)) {
    // bail out
    return;
  }

  // if log address or section is invalid
  if(_ulRefVarLogAddress==(-1) || _ulRefVarLogSection==(-1)) {
    // calculate them from
    INDEX iam = FindAddressMappingOfVariable(_strRefVarName,imf);
    // if invalid
    if(iam==(-1)) {
      return;
    }
    const MapFile &mf = _amfMapFiles[imf];
    const AddressMapping &am = mf.mf_aamAddresses[iam];
    // Copy data from address mapping
    _ulRefVarLogAddress = am.am_ulOffset;
    _ulRefVarLogSection = am.am_ulSection;
  }

  const ULONG ulAbsDelta   = _ulRefVarAbsAddress - _ulRefVarLogAddress;
  const ULONG ulLogAddress = ulAbsAddress - ulAbsDelta;
  const ULONG ulSection    = _ulRefVarLogSection;

  FindLogAddressInMapFile(strImage,ulSection,ulLogAddress,strFunction,slDelta);
}

static void ParseLogicalAddressLine(const char *strLogAdr, const char *strLine)
{
  // parse the line
  char strImage[1024] = "";
  ULONG ulSegment=-1;
  ULONG ulOffset=-1;
  sscanf(strLogAdr, "$adr: %s %x:%x", strImage, &ulSegment, &ulOffset);

  // find the function
  CTString strFunction;
  SLONG slDelta;
  FindLogAddressInMapFile(CTString(strImage), ulSegment, ulOffset, strFunction, slDelta);
  
  // output the result
  CTString strResult;
  strResult.PrintF("%s (%s+0X%X)", strLine, (const char*)strFunction, slDelta);
  _strmDst.PutLine_t(strResult);
}

static void ParseAbsoluteAddressLine(const char *strAbsAdr, const char *strLine)
{
  if(_strRefVarName=="") {
    CTString strError = CTString(0,"Referent variable not specified '%s'!\n",strLine);
    _strmDst.PutLine_t(strError);
  } else {
    // parse the line
    char strImage[1024] = "";
    ULONG ulAbsAddress=0;
    sscanf(strAbsAdr, "@adr: %s 0x%x", strImage, &ulAbsAddress);

    // find the function
    CTString strFunction;
    SLONG slDelta;
    FindInMapFileAbsolute(CTString(strImage), ulAbsAddress, strFunction, slDelta);

    // output the result
    CTString strResult;
    strResult.PrintF("%s (%s+0X%X)", strLine, (const char*)strFunction, slDelta);
    _strmDst.PutLine_t(strResult);
  }
}

static void ParseRefenentVariableLine(const char *strRefVar, const char *strLine)
{
  // parse the line
  char strRefVarName[1024] = "";
  ULONG ulAddress=0;
  sscanf(strRefVar, "Referent variable: %s 0x%x", strRefVarName, &ulAddress);
  _strRefVarName = strRefVarName;
  _ulRefVarAbsAddress = ulAddress;

  // Reset logical address and section of referent variable
  _ulRefVarLogAddress = -1;
  _ulRefVarLogSection = -1;

  // Just copy line
  _strmDst.PutLine_t(strLine);
}

static void SubMain( int argc, char *argv[])
{
  printf("\nDecodeReport - '.RPT' file decoder V1.1\n");
  printf(  "           (C)1999 CROTEAM Ltd\n\n");

  if( argc!=3+1)
  {
    printf( "USAGE:\nDecodeReport <infilename> <outfilename> <symbolsdir>\n");
    exit( EXIT_FAILURE);
  }
  
  // initialize engine
  SE_InitEngine("");
  _fnmApplicationPath = CTString("");


  CTFileName fnSrc = CTString(argv[1]);
  CTFileName fnDst = CTString(argv[2]);
  _fnSymbolsDir = CTString(argv[3]);

  try {
    if (fnSrc==fnDst) {
      throw "Use different files!";
    }

    CTFileStream strmSrc;
    strmSrc.Open_t(fnSrc, CTStream::OM_READ);
    _strmDst.Create_t(fnDst);
    
    // while there is some line in src
    while(!strmSrc.AtEOF()) {
      // read the line
      CTString strLine;
      strmSrc.GetLine_t(strLine);


      // try to find log address string
      char *strLogAdr = strstr(strLine, "$adr:");
      // if logical address was found
      if(strLogAdr!=NULL) {
        // Parse its line
        ParseLogicalAddressLine(strLogAdr, strLine);
      // else 
      } else {
        // try to find abs address string
        char *strAbsAdr = strstr(strLine, "@adr:");
        // if abs address was found
        if(strAbsAdr!=NULL) {
          // Parse its line
          ParseAbsoluteAddressLine(strAbsAdr, strLine);
        // else
        } else {
          // try to find referent variable string for absolute address calculation
          char *strRefVar = strstr(strLine, "Referent variable:");
          if(strRefVar!=NULL) {
            // Parse its line
            ParseRefenentVariableLine(strRefVar, strLine);
          // else nothing was found
          } else {
            // just copy the line
            _strmDst.PutLine_t(strLine);
          }
        }
      }
    }
    _strmDst.Close();
  } catch(char *strError) {
    _strmDst.Close();
    printf("\nError: %s\n", strError);
    exit(EXIT_FAILURE);
  }
}

