#include <Engine/Base/Stream.h>

#include <Engine/Templates/DynamicContainer.cpp>
#include <Engine/Base/Console.h>
#include <Engine/Base/MemoryTracking.h>

/*
 * Default constructor.
 */

CStock_TYPE::CStock_TYPE(void)
{
	st_ntObjects.SetAllocationParameters(50, 2, 2);
}

/*
 * Destructor.
 */

CStock_TYPE::~CStock_TYPE(void)
{
	// free all unused elements of the stock
//안태훈 수정 시작	//(DevPartner Bug Fix)(2005-01-10)
	FreeUnused();
//안태훈 수정 끝	//(DevPartner Bug Fix)(2005-01-10)
}

/*
 * Obtain an object from stock - loads if not loaded.
 */

TYPE *CStock_TYPE::Obtain_t(const CTFileName &fnmFileName)
{
	// find stocked object with same name
	TYPE *pExisting = st_ntObjects.Find(fnmFileName);
	
	// if found
	if (pExisting!=NULL) {
		// mark that it is used once again
		pExisting->MarkUsed();
		// return its pointer
		return pExisting;
	}

	/* if not found, */
	TRACKMEM(Mem, strrchr((const char*)fnmFileName, '.'));

	// create new stock object
	TYPE *ptNew = new TYPE;
	ptNew->ser_FileName = fnmFileName;
	st_ctObjects.Add(ptNew);
	st_ntObjects.Add(ptNew);

	// load it
	try {
		ptNew->Load_t(fnmFileName);
	} catch(char *) {
		st_ctObjects.Remove(ptNew);
		st_ntObjects.Remove(ptNew);
		delete ptNew;
		throw;
	}

	// mark that it is used for the first time
	//ASSERT(!ptNew->IsUsed());
	ptNew->MarkUsed();

	// return the pointer to the new one
	return ptNew;
}

/*
 * Release an object when not needed any more.
 */

void CStock_TYPE::Release(TYPE *ptObject)
{
	// mark that it is used one less time
	ptObject->MarkUnused();
	// if it is not used at all any more and should be freed automatically
	if (!ptObject->IsUsed() && ptObject->IsAutoFreed()) {
		// remove it from stock
		st_ctObjects.Remove(ptObject);
		st_ntObjects.Remove(ptObject);
		delete ptObject;
	}
}

// free all unused elements of the stock

void CStock_TYPE::FreeUnused(void)
{
	BOOL bAnyRemoved;
	// repeat
	do {
		// create container of objects that should be freed
		CDynamicContainer<TYPE> ctToFree;
		{FOREACHINDYNAMICCONTAINER(st_ctObjects, TYPE, itt) {
			if (!itt->IsUsed()) {
				ctToFree.Add(itt);
			}
		}}
		bAnyRemoved = ctToFree.Count()>0;
		// for each object that should be freed
		{FOREACHINDYNAMICCONTAINER(ctToFree, TYPE, itt) {
			extern INDEX dbg_bShowDeletedObjectsFromStock;
			if(dbg_bShowDeletedObjectsFromStock) {
				CPrintF("Deleting '%s'\n", itt->ser_FileName);
			}
			st_ctObjects.Remove(itt);
			st_ntObjects.Remove(itt);
			delete (&*itt);
		}}

	// as long as there is something to remove
	} while (bAnyRemoved);

}

// calculate amount of memory used by all objects in the stock

SLONG CStock_TYPE::CalculateUsedMemory(void)
{
	SLONG slUsedTotal = 0;
	{FOREACHINDYNAMICCONTAINER(st_ctObjects, TYPE, itt) {
		SLONG slUsedByObject = itt->GetUsedMemory();
		if (slUsedByObject<0) {
			return -1;
		}
		slUsedTotal+=slUsedByObject;
	}}

	return slUsedTotal;
}

struct SortLine {
	char *sl_pstrString;
	SLONG sl_slSize;
};

static int qsort_CompareStrings(const void *pLine1, const void *pLine2)
{
	SortLine *psl1 = (SortLine*)pLine1;
	SortLine *psl2 = (SortLine*)pLine2;
	INDEX iLength1 = strlen(psl1->sl_pstrString);
	INDEX iLength2 = strlen(psl2->sl_pstrString);
	INDEX iMinLen = iLength1>iLength2 ? iLength2 : iLength1; // Min(iLength1,iLength2)
	SLONG slValue1 = psl1->sl_slSize;
	SLONG slValue2 = psl2->sl_slSize;

	// Enable this to sort by memory usage
#if 1
	if(slValue1!=slValue2) {
		return slValue1 - slValue2;
	}
#endif

	for(INDEX ich=0;ich<iMinLen;ich++) {
		if(psl1->sl_pstrString[ich]!=psl2->sl_pstrString[ich]) {
			return psl1->sl_pstrString[ich] - psl2->sl_pstrString[ich];
		}
	}

	if(iLength1!=iLength2) {
		return iLength1 - iLength2;
	}

	return 0;
}

// dump memory usage report to a file
void CStock_TYPE::DumpMemoryUsage_t(CTStream &strm) // throw char *
{
	CStaticStackArray<SortLine> aslDump;
	SLONG slUsedTotal = 0;
	{FOREACHINDYNAMICCONTAINER(st_ctObjects, TYPE, itt) {
		SLONG slUsedByObject = itt->GetUsedMemory();
		if (slUsedByObject<0) {
			strm.PutLine_t("Error!");
			return;
		}
		CTString strLine;
		strLine.PrintF("%s(%d) %s", (const char*)(itt->GetName()), itt->GetUsedCount(), itt->GetDescription());
		SortLine &sl = aslDump.Push();
		sl.sl_pstrString = new char[strLine.Length()+1];
		sl.sl_slSize = slUsedByObject;
		strcpy(sl.sl_pstrString,(const char*)strLine);
	}}

	INDEX ctLines = aslDump.Count();
	if(ctLines>0) {
		qsort(&aslDump[0],ctLines,sizeof(SortLine),qsort_CompareStrings);
	}

	for(INDEX iln=0;iln<ctLines;iln++) {
		SortLine &sl = aslDump[iln];
		CTString strLine(0, "%7.1fk %s", sl.sl_slSize/1024.0f, (const char*)sl.sl_pstrString);
		strm.PutLine_t(strLine);
		delete[] sl.sl_pstrString;
		sl.sl_pstrString = NULL;
	}
}

// get number of total elements in stock
INDEX CStock_TYPE::GetTotalCount(void)
{
	return st_ctObjects.Count();
}

// get number of used elements in stock

INDEX CStock_TYPE::GetUsedCount(void)
{
	INDEX ctUsed = 0;
	{FOREACHINDYNAMICCONTAINER(st_ctObjects, TYPE, itt) {
		if (itt->IsUsed()) {
			ctUsed++;
		}
	}}
	return ctUsed;
}
