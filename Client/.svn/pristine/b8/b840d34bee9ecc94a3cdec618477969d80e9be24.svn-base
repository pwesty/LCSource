#include "stdh.h"

#include <Engine/Base/Memory.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Base/Translation.h>
#include <Engine/Base/Console.h>

#include <Engine/Base/ErrorReporting.h>

// unhide ANSI C functions so we can use them in our wrappers
#undef malloc
#undef free
#undef realloc
#undef calloc
#undef _msize
#undef strdup

extern FLOAT _bCheckAllAllocations = FALSE;

static void DefaultErrorInsufficientMemoryCallback(SLONG slMemoryRequired);
static void (*_pErrorInsufficientMemoryCallback)(SLONG slMemoryRequired) = DefaultErrorInsufficientMemoryCallback;

#if MEMORY_TRACKING
// we calculate memory slack for a block manually - _msize is not an accurate measure,
// as it doesn't account for block headers
		#define REALSIZE(size) ( (size+15)/16*16 + 8 )

// the above values are empirically determined for each system. to get your own measurements,
// use the following code snippet:

/*
	for(int i=0; i<100; i++) {
		char *a = (char*)malloc(i);
		char *b = (char*)malloc(i);

		int i1 = _msize(a);
		int i2 = b-a;
		printf("requested: %4d _msize: %4d realsize: %4d\n", i, i1, i2);
	}
*/

// note that this doesn't have to be exact for all memory sizes, but overheads are usually much more of
// a problem for a lot of small blocks than for several big ones.

#endif

// global memory allocation functions implementations - ANSI C compatible
void *MEM_MAlloc( size_t size )
{
	void *pmem;
	ASSERTMSG(size>=0, "MEM_MAlloc: Block size is less than zero.");
	pmem = malloc(size);

	// memory handler asures no null results here?!
	if (pmem==NULL) {
		_CrtCheckMemory();
		if(_pErrorInsufficientMemoryCallback!=NULL) {
			_pErrorInsufficientMemoryCallback(size);
		}
		FatalError(TRANS("Not enough memory (%d bytes needed)![MEM_MAlloc]"), size);
	}

	MEMTRACK_ALLOC(pmem, REALSIZE(size), size);

	return pmem;
}

void MEM_Free( void *memblock )
{
	// no need for assert (freeing null pointer is perfectly legal)
	//ASSERTMSG(memblock!=NULL, "FreeMemory: NULL pointer input.");

	MEMTRACK_FREE(memblock);

	free( (char *)memblock);
}

void *MEM_ReAlloc( void *memblock, size_t size )
{
	MEMTRACK_ANNOUNCEREALLOC(TRUE);

	MEMTRACK_FREE(memblock);

	memblock = realloc(memblock, size);
	// memory handler asures no null results here?!
	if (memblock==NULL) {
		FatalError(TRANS("Not enough memory (%d bytes needed)![MEM_ReAlloc]"), size);
	}

	MEMTRACK_ALLOC(memblock, REALSIZE(size), size);

	MEMTRACK_ANNOUNCEREALLOC(FALSE);
	return memblock;
}

void *MEM_CAlloc( size_t num, size_t size )
{
	size_t memsize = num*size;
	void *pmem = MEM_MAlloc(memsize);
	if (pmem!=NULL) {
		memset(pmem, 0, memsize);
	}
	return pmem;
}

size_t MEM_Size( void *memblock)
{
	size_t size;
#if MEMORY_TRACKING
	size = TrackMemorySize(memblock);
#else
	size = _msize(memblock);
#endif
	return size;
}

size_t MEM_GetFreeMemory(void)
{
	MEMORYSTATUS ms;
	GlobalMemoryStatus(&ms);
	return ms.dwAvailPhys;
}

// aligned allocation functions (do we really need this currently?)
void *AllocMemoryAligned( SLONG memsize, SLONG slAlignPow2)
{
	ULONG ulMem = (ULONG)AllocMemory(memsize+slAlignPow2*2);
	ULONG ulMemAligned = ((ulMem+slAlignPow2-1) & ~(slAlignPow2-1)) + slAlignPow2;
	((ULONG *)ulMemAligned)[-1] = ulMem;
	return (void*)ulMemAligned;
}
void FreeMemoryAligned( void *memory)
{
	FreeMemory((void*) ( ( (ULONG*)memory )[-1] ) );
}


// deprecated functions
void *AllocMemory( SLONG memsize )
{
	return MEM_MAlloc(memsize);
}

void FreeMemory( void *memory )
{
	MEM_Free(memory);
}

void ResizeMemory( void **ppv, SLONG slSize )
{
	*ppv = MEM_ReAlloc(*ppv, slSize);
}

void GrowMemory( void **ppv, SLONG newSize )
{
	ResizeMemory(ppv, newSize);
}

void ShrinkMemory( void **ppv, SLONG newSize )
{
	ResizeMemory(ppv, newSize);
}

// Set callback function that will be called on out of memory error
extern void SetErrorInsufficientMemoryCallback(void(*pErrorInsufficientMemoryCallback)(SLONG slMemoryRequired))
{
	_pErrorInsufficientMemoryCallback = pErrorInsufficientMemoryCallback;
}

// Default function that will be caller on out of memory error
static void DefaultErrorInsufficientMemoryCallback(SLONG slMemoryRequired)
{
	FatalError(TRANS("Not enough memory (%d bytes needed)![DefaultErrorInsufficientMemoryCallback]"), slMemoryRequired);
}
