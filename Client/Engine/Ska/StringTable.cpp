#include "StdH.h"
#include <Engine/Base/CTString.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Base/Console.h>
#include <Engine/Base/SimpleHashTable.h>
#include <Engine/Ska/StringTable.h>
#include <Engine/Templates/StaticStackArray.h>
#include <Engine/Templates/StaticStackArray.cpp>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>

static void RecreateHashTable(void);
static BOOL CompareStrings(INDEX iObject, const void *pObject);
static ULONG GetStringHash(const void *pObject);

CStaticStackArray<CTString> _astrStringTable;   // Table of ska strings
CStaticStackArray<INDEX> _aiNetSkaIDConversion; // Network id to local id conversion table
static CSimpleHashTable _shtHashTable(CompareStrings,GetStringHash,RecreateHashTable,10000); // Hash table for ska strings lookup


static void ValidateIDToString(const CTString &strString,INDEX iIndex)
{
#if _DEBUG
	INDEX ctString = _astrStringTable.Count();
	for(INDEX istr=0;istr<ctString;istr++) {
		if(istr == iIndex) {
//안태훈 수정 시작	//(For Performance)(0.3)
			ASSERT(strString == _astrStringTable[istr]);
			//ASSERT(IsEqualCaseSensitive(strString, _astrStringTable[istr]));
		} else {
			ASSERT(strString != _astrStringTable[istr]);
			//ASSERT(!IsEqualCaseSensitive(strString, _astrStringTable[istr]));
//안태훈 수정 끝	//(For Performance)(0.3)
		}
	}
#endif
}

static void RecreateHashTable(void)
{
	const INDEX ctStrings = _astrStringTable.Count();
	// Add all strings to hash table, again
	for(INDEX istr=0;istr<ctStrings;istr++) {
		const CTString &strString = _astrStringTable[istr];
		_shtHashTable.AddObject(&strString,istr);
		ValidateIDToString(strString,istr);
	}
}

// Callback function for string comparing
static BOOL CompareStrings(INDEX iObject, const void *pObject)
{
	const CTString &strString = *(const CTString*)pObject;
	const CTString &strInTable = _astrStringTable[iObject];
//안태훈 수정 시작	//(For Performance)(0.3)
	return strString == strInTable;
	//return strString.IsEqualCaseSensitive(strInTable);
//안태훈 수정 끝	//(For Performance)(0.3)
}

// Callback function for hash calculation
static ULONG GetStringHash(const void *pObject)
{
	const CTString &strString = *(const CTString*)pObject;
	return strString.GetHash();
}

// Add new string in string table
static INDEX AddStringToTable(const CTString &strString)
{
	// Add string to hash table
	INDEX iNewIndex = _astrStringTable.Count();
	_shtHashTable.AddObject(&strString,iNewIndex);
	
	// Add string to string table
	CTString &strNewString = _astrStringTable.Push();
	strNewString = strString;

	ASSERT(_astrStringTable.Count() == _shtHashTable.GetUsedCount());

	if (_pNetwork!=NULL && _pNetwork->IsServer()) {
		extern CEntityMessage _emEntityMessage;
		_emEntityMessage.WriteAddSkaString(iNewIndex,strString);
		_pNetwork->ga_srvServer.SendMessage(_emEntityMessage);
	}

	return iNewIndex;
}

// Convert string to ska id
extern INDEX ska_StringToID(const CTString &strString)
{
	MEMTRACK_SETFLAGS(mem,MTF_NOSTACKTRACE);
	// safety check
	if(strString=="") {
	//0729 kwon 임시삭제.
	//  ASSERTALWAYS("Converting empty string to id");
		return -1;
	}

	// Check if this string allready exists
	const INDEX iIndex = _shtHashTable.FindObject(&strString);
	// if string exists in table
	if(iIndex!=(-1)) {
		// make sure that found id match string
		ValidateIDToString(strString,iIndex);
		// return it
		return iIndex;
	// if not create new string
	} else {
		const INDEX iNewIndex = AddStringToTable(strString);
		// make sure that added string match its id
		ValidateIDToString(strString,iNewIndex);
		return iNewIndex;
	}
}

// Returns id of string (-1 if string does not exist)
extern INDEX ska_FindID(const CTString &strString)
{
	// safety check
	if(strString=="") {
		ASSERTALWAYS("Converting empty string to id");
		return -1;
	}
	// Find string in hash table
	const INDEX iIndex = _shtHashTable.FindObject(&strString);
	// make sure that found id match string
	ValidateIDToString(strString,iIndex);
	return iIndex;
}

// Convert ska id to string
extern CTString ska_IDToString(INDEX iID)
{
	if(iID==(-1)) {
		ASSERTALWAYS("Converting id -1 to string");
		return "";
	}
	return _astrStringTable[iID];
}

extern void DumpSkaStringToIDTable(void)
{
	_shtHashTable.DumpHashTableUsage(&_astrStringTable[0],sizeof(CTString),_astrStringTable.Count());

	CPrintF("\n\n");
	CPrintF("Dumping used strings:\n");
	// for each string 
	INDEX ctstr = _astrStringTable.Count();
	for(INDEX istr=0;istr<ctstr;istr++) {
		const CTString &strSting = _astrStringTable[istr];
		CPrintF("%5d: %s\n",istr, (const char*)strSting);
	}
}
