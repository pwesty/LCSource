#include "stdh.h"
#include <Engine/Base/Memory.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Base/ErrorReporting.h>
#include <Engine/Base/Console.h>
#include <Engine/Base/Synchronization.h>
#include <Engine/Base/Translation.h>
#include <Engine/Templates/StaticStackArray.cpp>
#include <Engine/Templates/AllocationArray.h>
#include <Engine/Templates/AllocationArray.cpp>

#if MEMORY_TRACKING   // entire file is not compiled if the tracking is disabled

#undef _msize

class CMemDescriptor 
{
public:
	const char *md_strDescription;
	INDEX md_ctBlocks;
	INDEX md_ctBytes;
	INDEX md_ctReqBytes;
};

#define STACKTRACKDEPTH 32

struct ReallocParams 
{
	ULONG rp_ulAge;
	ULONG rp_ulFlags;
#if STACKTRACKDEPTH>0
	ULONG rp_aulStack[STACKTRACKDEPTH];
#endif
};

//static _declspec(thread) ULONG _ulCurrentFlags = 0;
//static _declspec(thread) ReallocParams _rpReallocTemp;
//static _declspec(thread) BOOL _bDoingRealloc = FALSE;
TLVar<ULONG> _ulCurrentFlags = 0;
TLVar<ReallocParams> _rpReallocTemp;
TLVar<BOOL> _bDoingRealloc = FALSE;


CMemFlags::CMemFlags(ULONG ulAddFlags) {
	// if not changing any flags
	if(_ulCurrentFlags&ulAddFlags) {
		// dont remove any flags on destructor
		mf_ulAddedFlags = 0;
	} else {
		// remove flags set on destructor
		mf_ulAddedFlags = ulAddFlags;
	}
	_ulCurrentFlags |= mf_ulAddedFlags;
};
CMemFlags::~CMemFlags(void) {
	_ulCurrentFlags &= ~mf_ulAddedFlags;
}

class CMemBlock 
{
public:
	void *mb_pvBlock;
	INDEX mb_imdDescriptor;
	INDEX mb_ctBytes;
	INDEX mb_ctReqBytes;
	ULONG mb_ulAge;
	ULONG mb_ulFlags;
#if STACKTRACKDEPTH>0
	ULONG mb_aulStack[STACKTRACKDEPTH];
#endif
	
	void Clear(void)
	{
		mb_pvBlock = NULL;
		mb_imdDescriptor = -1;
		mb_ctBytes = 0;
		mb_ctReqBytes = 0;
		mb_ulFlags = 0;
	}
	CMemBlock(void) 
	{
		Clear();
	};
};

#define VALUE_TYPE ULONG
#define TYPE CMemBlock
#define CHashTableSlot_TYPE CHashTableSlot_CMemBlock
#define CHashTable_TYPE     CHashTable_CMemBlock
#include <Engine\Templates\HashTableTemplate.h>
#include <Engine\Templates\HashTableTemplate.cpp>
#undef VALUE_TYPE
#undef TYPE
#undef CHashTableSlot_TYPE
#undef CHashTable_TYPE

// global memory statistics
INDEX _ctMemBlocks = 0;
INDEX _ctMemBytes = 0;
INDEX _ctMemReqBytes = 0;

// memory statistics for tracking
INDEX _ctTrackingBlocks = 0;
INDEX _ctTrackingBytes = 0;

// tables used to track memory blocks
CTCriticalSection *_pcsMemTracking = NULL;
//_declspec(thread) INDEX _imdCurrentDescriptor = -1;
TLVar<INDEX> _imdCurrentDescriptor = -1;

CStaticStackArray<CMemDescriptor> *_pamdDescriptors = NULL;
CAllocationArray<CMemBlock> *_pambBlocks = NULL;
CHashTable_CMemBlock *_phbmBlocks = NULL;
extern ULONG _ulCurrentAllocationAge = 0;
static ULONG _ulMemUsedAtInit = -1;

extern BOOL GetLogicalAddress(PVOID addr, PTSTR szModule, DWORD len, DWORD& section, DWORD& offset );

// hash table callbacks
ULONG GetItemKey(ULONG &Item)
{
	// move the low bits away (as they are likely to be always 0)
	return _rotr(Item, 5);
}

ULONG GetItemValue(CMemBlock *Item)
{
	ULONG iBlock = ((ULONG)Item)-1;
	return (ULONG)(*_pambBlocks)[iBlock].mb_pvBlock;
}

// set description for future memory allocations (call with null to turn off descriptions)
void SetMemoryDescription(const char *strDescription)
{
	CTSingleLock slMem(_pcsMemTracking, _pcsMemTracking!=NULL);
	
	// if turning off
	if (strDescription==NULL) {
		// just remember that there is no description
		_imdCurrentDescriptor = -1;
		return;
	}
	
	// mark self-tracking, so we don't recurse
	_imdCurrentDescriptor = -2;
	
	// if no descriptors used ever before
	if (_pamdDescriptors==NULL) {
		// create the array of descriptors
		_pamdDescriptors = new CStaticStackArray<CMemDescriptor>;
	}
	
	// try to find this descriptor
	INDEX imdFound = -1;
	for(INDEX i=0; i<_pamdDescriptors->Count(); i++) {
		CMemDescriptor &md = (*_pamdDescriptors)[i];
		if (stricmp(md.md_strDescription, strDescription)==0) {
			imdFound = i;
			break;
		}
	}
	
	// if not found
	if (imdFound==-1) {
		// create new one
		imdFound = _pamdDescriptors->Count();
		CMemDescriptor &mdNew = _pamdDescriptors->Push();
#undef strdup   // make sure we call basic strup, as this is not tracked
		mdNew.md_strDescription = strdup(strDescription);
		mdNew.md_ctBlocks = 0;
		mdNew.md_ctBytes = 0;
		mdNew.md_ctReqBytes = 0;
	}
	
	_imdCurrentDescriptor = imdFound;
}
// set the current memory descriptor directly
void SetMemoryDescription(INDEX imdNew)
{
	CTSingleLock slMem(_pcsMemTracking, TRUE);
	_imdCurrentDescriptor = imdNew;
}
// get the current memory descriptor
INDEX GetMemoryDescription(void)
{
	return _imdCurrentDescriptor;
}

// mark that next pair of TrackMemoryFree/TrackMemoryAlloc is in fact a 'realloc'
void TrackMemoryAnnounceRealloc(BOOL bDoingRealloc)
{
	_bDoingRealloc = bDoingRealloc;
}

static void InitTracking(void)
{
	MEMORYSTATUS ms;
	GlobalMemoryStatus(&ms);
	_ulMemUsedAtInit = ms.dwTotalPhys-ms.dwAvailPhys;
	
	// create the array of blocks and hashing table
	_pambBlocks = new CAllocationArray<CMemBlock>;
	_pambBlocks->PreAllocate(65536);
	_phbmBlocks = new CHashTable_CMemBlock;
	_phbmBlocks->SetAllocationParameters(100000, 10, 10);
	_phbmBlocks->SetCallbacks(GetItemKey, GetItemValue);
	_pcsMemTracking = new CTCriticalSection;
	_pcsMemTracking->cs_iIndex = -1;
}

// (use to track system allocations outside of engine's control)
void TrackMemoryAlloc(void *pvBlock, INDEX ctBytes, INDEX ctReqBytes)
{
	CTSingleLock slMem(_pcsMemTracking, _pcsMemTracking!=NULL);
	
	// if no block
	if (pvBlock==NULL) {
		// don't track
		return;
	}
	
	// if special case of self-tracking
	if (_imdCurrentDescriptor==-2) {
		// track locally
		_ctTrackingBlocks+=1;
		_ctTrackingBytes+=_msize(pvBlock);
		return;
	}
	
	// mark self-tracking, so we don't recurse
	INDEX imdOrg = _imdCurrentDescriptor;
	_imdCurrentDescriptor = -2;
	
	// if never allocated before
	if (_pambBlocks==NULL) {
		InitTracking();
	}
	
	// get the block
	BOOL bNew = FALSE;
	ULONG ulBlock = (ULONG)pvBlock;
	INDEX imbBlock = (INDEX)_phbmBlocks->Find(ulBlock)-1;
	if (imbBlock==-1) {
		bNew = TRUE;
		imbBlock = _pambBlocks->Allocate();
	}
	CMemBlock &mbBlock= (*_pambBlocks)[imbBlock];
	// if this is not a new block
	if (!bNew) {
		// clear its old stats
		_ctMemBlocks-=1;
		_ctMemBytes-=mbBlock.mb_ctBytes;
		_ctMemReqBytes-=mbBlock.mb_ctReqBytes;
		if (mbBlock.mb_imdDescriptor >= 0) {
			(*_pamdDescriptors)[mbBlock.mb_imdDescriptor].md_ctBlocks-=1;
			(*_pamdDescriptors)[mbBlock.mb_imdDescriptor].md_ctBytes-=mbBlock.mb_ctBytes;
			(*_pamdDescriptors)[mbBlock.mb_imdDescriptor].md_ctReqBytes-=mbBlock.mb_ctReqBytes;
		}
	} else {
		mbBlock.mb_ulAge = _ulCurrentAllocationAge++;
		mbBlock.mb_ulFlags = _ulCurrentFlags;
	}
	
	// set new block data
	mbBlock.mb_ctBytes = ctBytes;
	mbBlock.mb_ctReqBytes = ctReqBytes;
	mbBlock.mb_imdDescriptor = imdOrg;
	mbBlock.mb_pvBlock = pvBlock;
	
	// if doing a realloc 
	if(_bDoingRealloc) {
		// remember stack that created block and age in which it was created
		mbBlock.mb_ulAge = _rpReallocTemp.rp_ulAge;
		mbBlock.mb_ulFlags = _rpReallocTemp.rp_ulFlags;
#if STACKTRACKDEPTH>0
		memcpy(&mbBlock.mb_aulStack,&_rpReallocTemp.rp_aulStack,sizeof(_rpReallocTemp.rp_aulStack));
#endif
	} else {
		// if tracking stack 
#if STACKTRACKDEPTH>0
		// Reset all values
		for(INDEX isd=0;isd<STACKTRACKDEPTH;isd++) {
			mbBlock.mb_aulStack[isd] = 0;
		}
		
		ULONG ulFrame;
		// Get current frame (ebp)
		__asm mov dword ptr ulFrame, ebp;
		// walk all frames
		for(isd=0;isd<STACKTRACKDEPTH;isd++) {
			// Get previous frame
			ULONG ulPrevFrame = *(ULONG*)ulFrame;
			// Get calling address
			ULONG ulRetAddr = *(ULONG*)(ulFrame+4);
			mbBlock.mb_aulStack[isd] = ulRetAddr;
			
			// Frame pointer must be aligned on a ULONG boundary
			if(ulFrame&3) {
				break;
			}
			if(ulPrevFrame<=ulFrame) {
				break;
			}
			
			if(IsBadWritePtr((ULONG*)ulFrame,sizeof(ULONG)) || IsBadWritePtr((ULONG*)ulPrevFrame,sizeof(ULONG))) {
				break;
			}
			ulFrame = ulPrevFrame; // go to next frame
		}
#endif
	}
	
	// if it is a new block
	if (bNew) {
		// add the block to the hashing table
		_phbmBlocks->Add((CMemBlock*)(imbBlock+1));
	}
	
	// track the block's stats
	_ctMemBlocks+=1;
	_ctMemBytes+=ctBytes;
	_ctMemReqBytes+=ctReqBytes;
	if (imdOrg >= 0) {
		(*_pamdDescriptors)[imdOrg].md_ctBlocks+=1;
		(*_pamdDescriptors)[imdOrg].md_ctBytes+=ctBytes;
		(*_pamdDescriptors)[imdOrg].md_ctReqBytes+=ctReqBytes;
	}
	
	// return original descriptor
	_imdCurrentDescriptor = imdOrg;
}

void TrackMemoryFree(void *pvBlock)
{
	CTSingleLock slMem(_pcsMemTracking, TRUE);
	// if no block
	if (pvBlock==NULL) {
		// don't track
		return;
	}
	
	// if special case of self-tracking
	if (_imdCurrentDescriptor==-2) {
		// track locally
		_ctTrackingBlocks-=1;
		_ctTrackingBytes-=_msize(pvBlock);
		return;
	}
	
	// if never allocated before
	if (_pambBlocks==NULL) {
		// report error
		FatalError("TrackMemoryFree(): called before TrackMemoryAlloc()!\n");
		// ignore the call
		return;
	}
	
	// try to find the block
	ULONG ulBlock = (ULONG)pvBlock;
	INDEX imbFound = (INDEX)_phbmBlocks->Find(ulBlock)-1;
	
	// if not found
	if (imbFound==-1) {
		for(INDEX imb=0; imb<_pambBlocks->CStaticArray<CMemBlock>::Count(); imb++) {
			if ((*_pambBlocks)[imb].mb_pvBlock == pvBlock ) {
				ASSERT(FALSE);
				break;
			}
		}
		ASSERT(FALSE);
		//    FatalError("TrackMemoryFree(): cannot find block!\n");
		// ignore the call
		return;
	}
	CMemBlock &mb = (*_pambBlocks)[imbFound];
	
	// if doing a realloc
	if(_bDoingRealloc) {
		_rpReallocTemp.rp_ulAge = mb.mb_ulAge;
		_rpReallocTemp.rp_ulFlags = mb.mb_ulFlags;
#if STACKTRACKDEPTH>0
		memcpy(&_rpReallocTemp.rp_aulStack,&mb.mb_aulStack,sizeof(_rpReallocTemp.rp_aulStack));
#endif
	}
	
	// track global stats
	_ctMemBlocks -= 1;
	_ctMemBytes -= mb.mb_ctBytes;
	_ctMemReqBytes -= mb.mb_ctReqBytes;
	
	// track per-descriptor stats
	INDEX imd = mb.mb_imdDescriptor;
	if (imd != -1) {
		(*_pamdDescriptors)[imd].md_ctBlocks-=1;
		(*_pamdDescriptors)[imd].md_ctBytes-=mb.mb_ctBytes;
		(*_pamdDescriptors)[imd].md_ctReqBytes-=mb.mb_ctReqBytes;
	}
	
	// mark self-tracking, so we don't recurse
	INDEX imdOrg = _imdCurrentDescriptor;
	_imdCurrentDescriptor = -2;
	// free this tracking block (note that freeing from an allocation array might actually allocate more memory!)
	_phbmBlocks->Remove((CMemBlock*)(imbFound+1));
	mb.mb_ctBytes = 0;
	mb.mb_ctReqBytes = 0;
	mb.mb_imdDescriptor = -1;
	mb.mb_pvBlock = NULL;
	_pambBlocks->Free(imbFound);
	
	// return original descriptor
	_imdCurrentDescriptor = imdOrg;
}

INDEX TrackMemorySize(void *pvBlock)
{
	CTSingleLock slMem(_pcsMemTracking, TRUE);
	// if no block
	if (pvBlock==NULL) {
		// it's not tracked
		return 0;
	}
	
	// try to find the block
	ULONG ulBlock = (ULONG)pvBlock;
	INDEX imbFound = (INDEX)_phbmBlocks->Find(ulBlock)-1;
	// if not found
	if (imbFound==-1) {
		
		for(INDEX imb=0; imb<_pambBlocks->CStaticArray<CMemBlock>::Count(); imb++) {
			if ((*_pambBlocks)[imb].mb_pvBlock == pvBlock ) {
				ASSERT(FALSE);
				break;
			}
		}
		
		ASSERT(FALSE);
		//    FatalError("TrackMemorySize(): cannot find block!\n");
		// ignore the call
		return 0;
	}
	CMemBlock &mb = (*_pambBlocks)[imbFound];
	
	return mb.mb_ctBytes;
}
INDEX TrackMemoryReqSize(void *pvBlock)
{
	CTSingleLock slMem(_pcsMemTracking, TRUE);
	// if no block
	if (pvBlock==NULL) {
		// it's not tracked
		return 0;
	}
	
	// try to find the block
	ULONG ulBlock = (ULONG)pvBlock;
	INDEX imbFound = (INDEX)_phbmBlocks->Find(ulBlock)-1;
	// if not found
	if (imbFound==-1) {
		
		for(INDEX imb=0; imb<_pambBlocks->CStaticArray<CMemBlock>::Count(); imb++) {
			if ((*_pambBlocks)[imb].mb_pvBlock == pvBlock ) {
				ASSERT(FALSE);
				break;
			}
		}
		
		ASSERT(FALSE);
		//    FatalError("TrackMemorySize(): cannot find block!\n");
		// ignore the call
		return 0;
	}
	CMemBlock &mb = (*_pambBlocks)[imbFound];
	
	return mb.mb_ctReqBytes;
}

extern void FCPrintF(CTFileStream *pstrm,const char *strFormat, ...);
// dump memory use per description
void DumpMemoryDescriptions(CTFileStream *pstrm/*=NULL*/)
{
	INDEX ctBlocks = 0;
	INDEX ctBytes = 0;
	INDEX ctReqBytes = 0;
	for(INDEX i=0; i<_pamdDescriptors->Count(); i++) {
		CMemDescriptor &md = (*_pamdDescriptors)[i];
		FCPrintF(pstrm,"%20s: %6d, %5.2fMB (%8dB) +%5.2fMB (%8dB)\n", 
			md.md_strDescription, md.md_ctBlocks, 
			md.md_ctBytes/1024.0f/1024.0f, md.md_ctBytes,
			(md.md_ctBytes-md.md_ctReqBytes)/1024.0f/1024.0f, md.md_ctBytes-md.md_ctReqBytes);
		ctBlocks += md.md_ctBlocks;
		ctBytes += md.md_ctBytes;
		ctReqBytes += md.md_ctReqBytes;
	}
	
	MEMORYSTATUS ms;
	GlobalMemoryStatus(&ms);
	ULONG ulMemUsedNow = ms.dwTotalPhys-ms.dwAvailPhys;
	ULONG ulOverheadStart = _ulMemUsedAtInit-(0               +0          );
	ULONG ulOverheadNow   = ulMemUsedNow    -(_ctTrackingBytes+_ctMemBytes);
	
	FCPrintF(pstrm,"%20s: %6d, %5.2fMB (%8dB) +%5.2fMB (%8dB)\n", "Unclassified", _ctMemBlocks-ctBlocks, (_ctMemBytes-ctBytes)/1024.0f/1024.0f, _ctMemBytes-ctBytes, (_ctMemReqBytes-ctReqBytes)/1024.0f/1024.0f, _ctMemReqBytes-ctReqBytes);
	FCPrintF(pstrm,"%20s: %6d, %5.2fMB (%8dB) +%5.2fMB (%8dB)\n", "TOTAL overall", _ctMemBlocks, _ctMemBytes/1024.0f/1024.0f, _ctMemBytes, (_ctMemBytes-_ctMemReqBytes)/1024.0f/1024.0f, _ctMemBytes-_ctMemReqBytes);
	FCPrintF(pstrm,"%20s: %6d, %5.2fMB (%8dB)\n", "<MemoryTracking>", _ctTrackingBlocks, _ctTrackingBytes/1024.0f/1024.0f, _ctTrackingBytes);
	FCPrintF(pstrm,"%20s: %6d, %5.2fMB (%8dB)\n", "StartupOverhead", 0, ulOverheadStart/1024.0f/1024.0f, ulOverheadStart);
	FCPrintF(pstrm,"%20s: %6d, %5.2fMB (%8dB)\n", "CurrentOverhead", 0, ulOverheadNow/1024.0f/1024.0f, ulOverheadNow);
	FCPrintF(pstrm,"%20s: %6d, %5.2fMB (%8dB)\n", "UntrackedMemory", 0, (ulOverheadNow-ulOverheadStart)/1024.0f/1024.0f, (ulOverheadNow-ulOverheadStart));
	
	extern INDEX _ctStrings;
	extern INDEX _ctStringBytes;
	extern INDEX _ctStringReqBytes;
	extern INDEX _ctEmptyStrings;
	FCPrintF(pstrm,"  String memory: %d strings, total %d bytes, %d empty strings, slack %d bytes\n",
		_ctStrings, _ctStringBytes, _ctEmptyStrings, _ctStringBytes-_ctStringReqBytes);
	
	INDEX ctDesc = 0;
	INDEX ctMemBlocks = 0;
	INDEX ctFreeElements = 0;
	INDEX ctHashBlocks = 0;
	if(_pamdDescriptors!=NULL) {
		ctDesc = _pamdDescriptors->sa_Count;
	}
	if(_pambBlocks!=NULL) {
		ctMemBlocks = _pambBlocks->sa_Count;
		ctFreeElements = _pambBlocks->aa_aiFreeElements.sa_Count;
	}
	if(_phbmBlocks!=NULL) {
		ctHashBlocks = _phbmBlocks->ht_ahtsSlots.Count();
	}
	
	FCPrintF(pstrm,"  Memory tracking: Desc %d, Memblocks %d\n",ctDesc,ctMemBlocks);
	FCPrintF(pstrm,"                   FreeElements %d, Hashblocks %d\n",ctFreeElements,ctHashBlocks);
}


static void DumpBlockStackTrace(CTFileStream *pstrm, CMemBlock *pmb)
{
	ASSERT(pmb!=NULL);
	ULONG ulFlags = pmb->mb_ulFlags;
	FCPrintF(pstrm,"Memory block size = %d(%d) B:\n", pmb->mb_ctReqBytes, pmb->mb_ctBytes);
	FCPrintF(pstrm,"Memory block address = 0x%08X\n", (INDEX)pmb->mb_pvBlock);
	if(ulFlags&MTF_EOLIST) {
		FCPrintF(pstrm,"Block is put on end of list\n");
	}
	if(ulFlags&MTF_STRING) {
		FCPrintF(pstrm,"Block is marked as string\n");
	}
	
	for(INDEX isd=0;isd<STACKTRACKDEPTH;isd++) {
		ULONG ulAddress = pmb->mb_aulStack[isd];
		// if done
		if(ulAddress==0) {
			// bail out
			break;
		}
		
		// Get module for calling address
		char strModule[MAX_PATH];
		ULONG ulSection = 0;
		ULONG ulOffset = 0;
		// if getting of address was successfull
		if(GetLogicalAddress((ULONG*)ulAddress,strModule,sizeof(strModule),ulSection,ulOffset)) {
			// dump stack address
			FCPrintF(pstrm," $adr: %s %04X:%08X\n",strModule,ulSection,ulOffset);
		} else {
			ASSERT(FALSE);
		}
	}
	FCPrintF(pstrm,"\n");
}

static int qsort_CompareBlocks(const void *pBlock1, const void *pBlock2)
{
	ASSERT(pBlock1!=NULL);
	ASSERT(pBlock2!=NULL);
	
	INDEX imbBlock1 = *(INDEX*)pBlock1;
	INDEX imbBlock2 = *(INDEX*)pBlock2;
	
	// Get memory block 
	CMemBlock *pmb1 = &(*_pambBlocks)[imbBlock1];
	CMemBlock *pmb2 = &(*_pambBlocks)[imbBlock2];
	CMemDescriptor *pmd1 = NULL;
	CMemDescriptor *pmd2 = NULL;
	if(pmb1->mb_imdDescriptor>=0) {
		pmd1 = &(*_pamdDescriptors)[pmb1->mb_imdDescriptor];
	}
	if(pmb2->mb_imdDescriptor>=0) {
		pmd2 = &(*_pamdDescriptors)[pmb2->mb_imdDescriptor];
	}
	
	const BOOL bEol1 = pmb1->mb_ulFlags&MTF_EOLIST;
	const BOOL bEol2 = pmb2->mb_ulFlags&MTF_EOLIST;
	if(bEol1 && !bEol2) {
		return 1;
	} else if(bEol2 && !bEol1) {
		return -1;
	}
	
	if((pmb1->mb_ctReqBytes-pmb2->mb_ctReqBytes)!=0) {
		return pmb2->mb_ctReqBytes-pmb1->mb_ctReqBytes;
	}
	
	if(pmd1==NULL && pmd2==NULL) {
		return 0;
	} else if(pmd1==NULL && pmd2!=NULL) {
		return -1;
	} else if(pmd1!=NULL && pmd2==NULL) {
		return 1;
	} else {
		return stricmp(pmd1->md_strDescription,pmd2->md_strDescription);
	}
	
	return 0;
}

CMemBlock *FindMemBlock(const void *pvBlock)
{
	CTSingleLock slMem(_pcsMemTracking, TRUE);
	// if no block
	if (pvBlock==NULL) 
	{
		// don't track
		return NULL;
	}
	
	// try to find the block
	ULONG ulBlock = (ULONG)pvBlock;
	INDEX imbFound = (INDEX)_phbmBlocks->Find(ulBlock)-1;
	
	// if not found
	if (imbFound==-1) 
	{
		for(INDEX imb=0; imb<_pambBlocks->CStaticArray<CMemBlock>::Count(); imb++) 
		{
			if ((*_pambBlocks)[imb].mb_pvBlock == pvBlock ) 
			{
				ASSERT(FALSE);
				break;
			}
		}
		ASSERT(FALSE);
		return NULL;
	}
	CMemBlock &mb = (*_pambBlocks)[imbFound];
	return &mb;
}

void TrackMemorySetBlockFlags(void *pvBlock, ULONG ulFlags)
{
	CMemBlock *pmb = FindMemBlock(pvBlock);
	if(pmb!=NULL) 
	{
		pmb->mb_ulFlags=ulFlags;
	} 
	else 
	{
		ASSERTALWAYS("Could not find memory block");
	}
}

ULONG TrackMemoryGetBlockFlags(const void *pvBlock)
{
	CMemBlock *pmb = FindMemBlock(pvBlock);
	if(pmb!=NULL) 
	{
		return pmb->mb_ulFlags;
	} 
	else 
	{
		ASSERTALWAYS("Could not find memory block");
		return 0;
	}
}

extern void MemoryStackTraceDump(CTFileStream *pstrm/*=NULL*/)
{
	extern INDEX mem_ulStackTraceBegin;
	extern INDEX mem_ulStackTraceEnd;
	ULONG ulTraceBegin = mem_ulStackTraceBegin;
	ULONG ulTraceEnd = mem_ulStackTraceEnd;
	
	if(ulTraceBegin==(-1)) {
		ulTraceBegin = 0;
	}
	if(ulTraceEnd==(-1)) {
		ulTraceEnd = _ulCurrentAllocationAge;
	}
	if(ulTraceBegin>ulTraceEnd) {
		ulTraceEnd = _ulCurrentAllocationAge;
	}
	
	CPrintF("Tracking allocations from age 0x%08X to age 0x%08X\n", ulTraceBegin, ulTraceEnd);
	
	INDEX cthc = _phbmBlocks->ht_ctCompartments;
	INDEX cthcs = _phbmBlocks->ht_ctSlotsPerComp;
	
	CStaticStackArray<INDEX> aTracedBlocks;
	aTracedBlocks.SetAllocationStep(256);
	
	INDEX iCompSlot = 0;
	// for each compartment
	for(INDEX ihc=0;ihc<cthc;ihc++) {
		// for each slot in this compartment
		for(INDEX ihcs=0;ihcs<cthcs;ihcs++) {
			// Get memory block
			CHashTableSlot_CMemBlock &hts = _phbmBlocks->ht_ahtsSlots[iCompSlot++];
			INDEX imbBlock = INDEX(hts.hts_ptElement)-1;
			if(imbBlock==-1) {
				continue;
			}
			CMemBlock *pmb = &(*_pambBlocks)[imbBlock];
			CMemDescriptor *pmd = NULL;
			if(pmb->mb_imdDescriptor>=0) {
				pmd = &(*_pamdDescriptors)[pmb->mb_imdDescriptor];
			}
			// if block is inside begin and end block
			ULONG ulBlockAge = pmb->mb_ulAge;
			if(ulBlockAge>=ulTraceBegin && ulBlockAge<=ulTraceEnd) {
				// Add block index to traced array 
				INDEX &iBlockIndex = aTracedBlocks.Push();
				iBlockIndex = imbBlock;
			}
		}
	}
	
	// Sort blocks
	INDEX ctTracedBlocks = aTracedBlocks.Count();
	if(ctTracedBlocks>0) {
		qsort(&aTracedBlocks[0],ctTracedBlocks,sizeof(INDEX),qsort_CompareBlocks);
	}
	
	ULONG ctDumpedBlocks = 0;
	ULONG ctUnDumpedBlocks = 0;
	ULONG ulBytesTracked = 0;
	ULONG ulBytesUnTracked = 0;
	BOOL  bDumpingEol = FALSE;
	// for each traced block
	for(INDEX itb=0;itb<ctTracedBlocks;itb++) {
		INDEX imbBlock = aTracedBlocks[itb];
		// Get memory block 
		CMemBlock *pmb = &(*_pambBlocks)[imbBlock];
		CMemDescriptor *pmd = NULL;
		if(pmb->mb_imdDescriptor>=0) {
			pmd = &(*_pamdDescriptors)[pmb->mb_imdDescriptor];
		}
		
		// if this block is flaged not to be traced
		if(pmb->mb_ulFlags&MTF_NOSTACKTRACE) {
			// do not dump it
			ctUnDumpedBlocks++;
			ulBytesUnTracked+=pmb->mb_ctBytes;
			continue;
		}
		BOOL bIsString = FALSE;
		if(pmb->mb_ulFlags&MTF_STRING) {
			bIsString = TRUE;
		}
		
		if(pmb->mb_ulFlags&MTF_EOLIST && !bDumpingEol) {
			FCPrintF(pstrm,"\n************* Dumping blocks flaged as less important ***********\n");
			FCPrintF(pstrm,"\n\n");
			bDumpingEol = TRUE;
		}
		// if block has description
		if(pmd!=NULL) {
			FCPrintF(pstrm,"Block descriptor is '%s'\n",pmd->md_strDescription);
		} else {
			FCPrintF(pstrm,"Unclassified block\n");
		}
		// If this block is string, print if
		if(bIsString) {
			CTString str = *(CTString*)&pmb->mb_pvBlock;
			FCPrintF(pstrm,"Dumping string '%s'\n",(const char*)str);
		}
		// Dump block stack trace
		DumpBlockStackTrace(pstrm,pmb);
		ulBytesTracked+=pmb->mb_ctBytes;
		ctDumpedBlocks++;
	}
	
	// Show dump info
	if(pstrm!=NULL) {
		CPrintF("Tracked memory allocations %d (%dB = %gKB)\n%d allocations marked as not traced (%dB = %gKB)\n",
			ctDumpedBlocks, ulBytesTracked, ulBytesTracked/1024.0f,
			ctUnDumpedBlocks, ulBytesUnTracked, ulBytesUnTracked/1024.0f);
	}
	FCPrintF(pstrm,"Tracked memory allocations %d (%dB = %gKB)\n%d allocations marked as not traced (%dB = %gKB)\n",
		ctDumpedBlocks, ulBytesTracked, ulBytesTracked/1024.0f,
		ctUnDumpedBlocks, ulBytesUnTracked, ulBytesUnTracked/1024.0f);
}

#endif // MEMORY_TRACKING
