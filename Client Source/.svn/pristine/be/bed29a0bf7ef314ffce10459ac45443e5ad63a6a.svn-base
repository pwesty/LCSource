#ifndef SE_INCL_STRINGTABLE_H
#define SE_INCL_STRINGTABLE_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

extern CStaticStackArray<CTString> _astrStringTable; // Table of ska strings
extern CStaticStackArray<INDEX> _aiNetSkaIDConversion;    // Network ska id to local ska id conversion table

// Convert string to ska id
ENGINE_API extern INDEX ska_StringToID(const CTString &strString);
// Returns id of string (-1 if string does not exist)
ENGINE_API extern INDEX ska_FindID(const CTString &strString);
// Convert ska id to string
ENGINE_API extern CTString ska_IDToString(INDEX iID);
// Dump ska string to id table to information
extern void DumpSkaStringToIDTable(void);

// Old function names to string to id conversion
#define ska_GetIDFromStringTable ska_StringToID 
#define ska_GetStringFromTable   ska_IDToString

#endif // include once
