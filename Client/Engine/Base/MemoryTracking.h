#ifndef SE_INCL_MEMORYTRACKING_H
#define SE_INCL_MEMORYTRACKING_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Base/CTString.h>

#define MEMORY_TRACKING 0 // enable/disable memory usage tracking

#define MTF_STRING       (1UL<<0) // memory block is string (will be displayed in stack trace list as string)
#define MTF_EOLIST       (1UL<<1) // memory block will be put at end of stack trace list 
#define MTF_NOSTACKTRACE (1UL<<2) // do not trace next blocks
//
// The memory tracking API functions/objects are compiled only if memory tracking is enabled.
//

#if MEMORY_TRACKING

  class CMemDescriptor;

  // set description for future memory allocations (call with null to turn off descriptions)
  ENGINE_API extern void SetMemoryDescription(const char *strDescription);
  ENGINE_API extern void SetMemoryDescription(INDEX imdNew);
  ENGINE_API extern INDEX GetMemoryDescription(void);

  // track memory allocation/deallocation 
  // (use to track system allocations outside of engine's control)
  ENGINE_API extern void TrackMemoryAlloc(void *pvBlock, INDEX ctBytes, INDEX ctRequestedBytes);
  ENGINE_API extern void TrackMemoryFree(void *pvBlock);
  // get size of a tracked block
  ENGINE_API extern INDEX TrackMemorySize(void *pvBlock);
  // get requested size of a memory block
  ENGINE_API extern INDEX TrackMemoryReqSize(void *pvBlock);
  // mark that next pair of TrackMemoryFree/TrackMemoryAlloc is in fact a 'realloc'
  ENGINE_API extern void TrackMemoryAnnounceRealloc(BOOL bDoingRealloc);

  ENGINE_API extern void TrackMemorySetBlockFlags(void *pvBlock, ULONG ulFlags);
  ENGINE_API extern ULONG TrackMemoryGetBlockFlags(const void *pvBlock);

  // dump memory use per description
  ENGINE_API extern void DumpMemoryDescriptions(CTFileStream *pstrm=NULL);
  // stack trace
  ENGINE_API extern void MemoryStackTraceDump(CTFileStream *pstrm=NULL);

  // object used for automatic setting/resetting of current descriptor
  class CTrackMemory {
  public:
    INDEX tm_imdOld;

    CTrackMemory(const char *strDescription)
    {
      tm_imdOld = GetMemoryDescription();
      SetMemoryDescription(strDescription);
    }
    ~CTrackMemory()
    {
      SetMemoryDescription(tm_imdOld);
    }
  };

  // object used for automatic setting/resetting of flags for memory blocks
  class CMemFlags {
  public:
    CMemFlags(ULONG ulAddFlags);
    ~CMemFlags(void);
  public:
    ULONG mf_ulAddedFlags;
  };

#endif // MEMORY_TRACKING

//
// The memory tracking macros map either to functions/objects or to dummies,
// depending on whether memory tracking is enabled.
//

#if MEMORY_TRACKING

  #define MEMTRACK_SETDESC(x) SetMemoryDescription(x)
  #define MEMTRACK_GETDESC() GetMemoryDescription()
  #define MEMTRACK_ALLOC(pvBlock, ctBytes, ctReqSize) TrackMemoryAlloc(pvBlock, ctBytes, ctReqSize)
  #define MEMTRACK_ANNOUNCEREALLOC(bDoingRealloc) TrackMemoryAnnounceRealloc(bDoingRealloc)
  #define MEMTRACK_FREE(pvBlock) TrackMemoryFree(pvBlock)
  #define MEMTRACK_SIZE(pvBlock) TrackMemorySize(pvBlock)
  #define MEMTRACK_REQSIZE(pvBlock) TrackMemoryReqSize(pvBlock)
  #define MEMTRACK_DUMP() DumpMemoryDescriptions()
  #define MEMTRACK_DUMP_STACK() MemoryStackTraceDump()
  #define MEMTRACK_DUMP_INSTREAM(pstrm) DumpMemoryDescriptions(pstrm)
  #define MEMTRACK_DUMP_STACK_INSTREAM(pstrm) MemoryStackTraceDump(pstrm)
  #define MEMTRACK_SETFLAGS(var, x) CMemFlags mf##var(x)
  #define MEMTRACK_SETBLOCKFLAGS(pvBlock, ulFlags) TrackMemorySetBlockFlags(pvBlock, ulFlags);
  #define MEMTRACK_GETBLOCKFLAGS(pvBlock) TrackMemoryGetBlockFlags(pvBlock);
  #define TRACKMEM(var, desc) CTrackMemory tm##var(desc)

#else // MEMORY_TRACKING

  #define MEMTRACK_SETDESC(x)
  #define MEMTRACK_GETDESC()
  #define MEMTRACK_ALLOC(pvBlock, ctBytes, ctReqSize)
  #define MEMTRACK_ANNOUNCEREALLOC(bDoingRealloc)
  #define MEMTRACK_FREE(pvBlock)
  #define MEMTRACK_SIZE(pvBlock)
  #define MEMTRACK_REQSIZE(pvBlock)
  #define MEMTRACK_DUMP()
  #define MEMTRACK_DUMP_INSTREAM(pstrm)
  #define MEMTRACK_DUMP_INSTREAM(pstrm)
  #define MEMTRACK_DUMP_STACK_INSTREAM(pstrm)
  #define MEMTRACK_SETFLAGS(var, x)
  #define MEMTRACK_SETBLOCKFLAGS(pvBlock, ulFlags)
  #define MEMTRACK_GETBLOCKFLAGS(pvBlock)
  #define TRACKMEM(var, desc)

#endif // MEMORY_TRACKING

#endif  /* include-once check. */
