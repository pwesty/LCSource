#ifndef SE_INCL_UNZIP_H
#define SE_INCL_UNZIP_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

// add one zip archive to current active set
void UNZIPAddArchive(const CTFileName &fnm);
// read directories of all currently added archives, in reverse alphabetical order
void UNZIPReadDirectoriesReverse_t(void);
// check if a zip file entry exists
BOOL UNZIPFileExists(const CTFileName &fnm);
// open a zip file entry for reading
INDEX UNZIPOpen_t(const CTFileName &fnm);
// get uncompressed size of a file
SLONG UNZIPGetSize(INDEX iHandle);
// get CRC of a file
ULONG UNZIPGetCRC(INDEX iHandle);
// read a block from zip file
void UNZIPReadBlock_t(INDEX iHandle, UBYTE *pub, SLONG slStart, SLONG slLen);
// read ahead a block from zip file
void UNZIPReadAheadBlock_t(INDEX iHandle, SLONG slStart, SLONG slLen);
// close a zip file entry
void UNZIPClose(INDEX iHandle);
// get info on a zip file entry
void UNZIPGetFileInfo(INDEX iHandle, CTFileName &fnmZip,
  SLONG &slOffset, SLONG &slSizeCompressed, SLONG &slSizeUncompressed, 
  BOOL &bCompressed);
// enumeration for all files in all zips
INDEX UNZIPGetFileCount(void);
const CTFileName &UNZIPGetFileAtIndex(INDEX i);

// get index of a file (-1 for no file)
INDEX UNZIPGetFileIndex(const CTFileName &fnm);
// check if a file is from a mod's zip
BOOL UNZIPIsFileAtIndexMod(INDEX i);
// get crc on a zip file entry, by index
ULONG UNZIPGetCRCByIndex(INDEX iFile);

// adjust parameters for zip streaming caching
ENGINE_API SLONG UNZIPGetSectorSize(void);
ENGINE_API void UNZIPSetSectorSize(SLONG slSize);
ENGINE_API INDEX UNZIPGetBufferSectors(void);
ENGINE_API void UNZIPSetBufferSectors(INDEX iSectors);
ENGINE_API INDEX UNZIPGetPrefetchSectors(void);
ENGINE_API void UNZIPSetPrefetchSectors(INDEX iSectors);


#endif  /* include-once check. */

