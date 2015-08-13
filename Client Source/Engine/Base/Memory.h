#ifndef SE_INCL_MEMORY_H
#define SE_INCL_MEMORY_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Base/Types.h>

// global memory allocation functions implementations - ANSI C compatible
ENGINE_API extern void *MEM_MAlloc( size_t size );
ENGINE_API extern void MEM_Free( void *memblock );
ENGINE_API extern void *MEM_ReAlloc( void *memblock, size_t size );
ENGINE_API extern void *MEM_CAlloc( size_t num, size_t size );
ENGINE_API extern size_t MEM_Size( void *memblock); // this one is not really ANSI must is often used
ENGINE_API extern size_t MEM_GetFreeMemory(void);  // how much memory is free

// we hide the basic ANSI C/C++ memory access system, so that ours is used instead
#define malloc MEM_MAlloc
#define free MEM_Free
#define realloc MEM_ReAlloc
#define calloc MEM_CAlloc
#define _msize MEM_Size

//inline void *operator new(size_t size) { return MEM_MAlloc(size); };
//inline void operator delete(void *pv) { MEM_Free(pv);  };

// some wrappers
/* Allocate a block of memory - fatal error if not enough memory. */
ENGINE_API extern void *AllocMemory( SLONG memsize );
ENGINE_API extern void *AllocMemoryAligned( SLONG memsize, SLONG slAlignPow2);
/* Free a block of memory. */
ENGINE_API extern void FreeMemory( void *memory);
ENGINE_API extern void FreeMemoryAligned( void *memory);

ENGINE_API extern void ResizeMemory( void **memory, SLONG memsize );
ENGINE_API extern void GrowMemory( void **memory, SLONG memsize );
ENGINE_API extern void ShrinkMemory( void **memory,SLONG memsize );

ENGINE_API extern BOOL MemoryConsistencyCheck( void );
ENGINE_API extern BOOL AllMemoryFreed( void );

// Set callback function that will be called on out of memory error
ENGINE_API extern void SetErrorInsufficientMemoryCallback(void(*pErrorInsufficientMemoryCallback)(SLONG slMemoryRequired));

//
// below are some very fast memcpy, memset and memfind extensions
//

#pragma warning(disable: 4731) // EBX reg usage

// fast memory copy of ULONGs
inline void CopyLongs( ULONG *pulSrc, ULONG *pulDst, INDEX ctLongs)
{
#if (defined _MSC_VER)
  __asm {
    mov   ecx,dword ptr [ctLongs]
    jecxz skip
    cld
    mov   esi,dword ptr [pulSrc]
    mov   edi,dword ptr [pulDst]
    rep   movsd
skip:
  }
#else
  memcpy( pulDst, pulSrc, ctLongs*4);
#endif
}


// fast memory set of ULONGs
inline void StoreLongs( ULONG ulVal, ULONG *pulDst, INDEX ctLongs)
{
#if (defined _MSC_VER)
  __asm {
    mov   ecx,dword ptr [ctLongs]
    jecxz skip
    mov   eax,dword ptr [ulVal]
    mov   edi,dword ptr [pulDst]
    cld
    rep   stosd
skip:
  }
#else
  for( INDEX i=0; i<ctLongs; i++) pulDst[i] = ulVal;
#endif
}


// return position (offset) where we encounter specified byte or ctBytes
inline INDEX FindByte( UBYTE ubByte, UBYTE *pubMemory, INDEX ctBytes)
{
#if (defined _MSC_VER)
  __asm {
    cld
    mov    al, byte ptr [ubByte]
    mov   edi,dword ptr [pubMemory]
    mov   edx,dword ptr [ctBytes]
    mov   ecx,edx
    repne scasb
    setne al
    cmp   al,1
    sbb   edx,ecx   // don't decrement offset if we didn't find a match
    mov   dword ptr [ctBytes],edx
  } return ctBytes;
#else
  for( INDEX i=0; i<ctBytes; i++) if( pubMemory[i]==ubByte) return i;
  return ctBytes;
#endif
}


// return position (offset) where we encounter specified long or ctLongs
inline INDEX FindWord( UWORD uwWord, UWORD *puwMemory, INDEX ctWords)
{
#if (defined _MSC_VER)
  __asm {
    cld
    mov   ax,word ptr [uwWord]
    mov   edi,dword ptr [puwMemory]
    mov   edx,dword ptr [ctWords]
    mov   ecx,edx
    repne scasw
    setne al
    cmp   al,1
    sbb   edx,ecx   // don't decrement offset if we didn't find a match
    mov   dword ptr [ctWords],edx
  } return ctWords;
#else
  for( INDEX i=0; i<ctWords; i++) if( puwMemory[i]==uwWord) return i;
  return ctWords;
#endif
}


// return position (offset) where we encounter specified long or ctLongs
inline INDEX FindLong( ULONG ulLong, ULONG *pulMemory, INDEX ctLongs)
{
#if (defined _MSC_VER)
  __asm {
    cld
    mov   eax,dword ptr [ulLong]
    mov   edi,dword ptr [pulMemory]
    mov   edx,dword ptr [ctLongs]
    mov   ecx,edx
    repne scasd
    setne al
    cmp   al,1
    sbb   edx,ecx   // don't decrement offset if we didn't find a match
    mov   dword ptr [ctLongs],edx
  } return ctLongs;
#else
  for( INDEX i=0; i<ctLongs; i++) if( pulMemory[i]==ulLong) return i;
  return ctLongs;
#endif
}

#pragma warning(default: 4731) // EBX reg usage


#endif  /* include-once check. */

