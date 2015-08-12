
#include "stdh.h"

#include <d3dx8tex.h>
#pragma comment(lib, "d3dx8.lib")

#include <Engine/Graphics/GfxLibrary.h>


// asm shortcuts
#define O offset
#define Q qword ptr
#define D dword ptr
#define W  word ptr
#define B  byte ptr


static ULONG _ulAlphaMask = 0;
static D3DFORMAT _d3dLastFormat = (D3DFORMAT)1234567890; // flush!
static void (*pConvertMipmap)( ULONG *pulSrc, void *pDst, PIX pixWidth, PIX pixHeight, SLONG slDstPitch);
static void (*pSwizzleMipmap)( ULONG *pulSrc, void *pDst, PIX pixWidth, PIX pixHeight, SLONG slDstPitch);

// some stuff for MMX registers
static const __int64 _mm000000F8 = 0x000000F8000000F8;
static const __int64 _mm0000FC00 = 0x0000FC000000FC00;
static const __int64 _mm00F80000 = 0x00F8000000F80000;
static const __int64 _mm000000F0 = 0x000000F0000000F0;
static const __int64 _mm0000F000 = 0x0000F0000000F000;
static const __int64 _mm00F00000 = 0x00F0000000F00000;
static const __int64 _mmF0000000 = 0xF0000000F0000000;
static const __int64 _mm0000F800 = 0x0000F8000000F800;
static const __int64 _mm80000000 = 0x8000000080000000;
static const __int64 _mm000000FF = 0x000000FF000000FF;
static const __int64 _mmFF000000 = 0xFF000000FF000000;
static const __int64 _mmSignSub  = 0x00007FFF00007FFF;
static const __int64 _mmSignAdd  = 0x7FFF7FFF7FFF7FFF;



// convert to any D3DFormat (thru D3DX functions - slow!)
static void ConvertAny( ULONG *pulSrc, LPDIRECT3DTEXTURE8 ptexDst, PIX pixWidth, PIX pixHeight, INDEX iMip)
{
  // alloc temporary memory and flip colors there
  const PIX pixSize = pixWidth*pixHeight;
  ULONG *pulFlipped = (ULONG*)AllocMemory( pixSize*sizeof(ULONG));
  abgr2argb( pulSrc, pulFlipped, pixSize);
  pulSrc = pulFlipped;

  // get mipmap surface
  HRESULT hr;
  LPDIRECT3DSURFACE8 pd3dSurf;
  hr = ptexDst->GetSurfaceLevel( iMip, &pd3dSurf);
  D3D_CHECKERROR(hr);

  // prepare and upload surface
  const RECT rect = { 0,0, pixWidth, pixHeight };
  const SLONG slSrcPitch = pixWidth*sizeof(ULONG);
  hr = D3DXLoadSurfaceFromMemory( pd3dSurf, NULL, NULL, pulSrc, D3DFMT_A8R8G8B8, slSrcPitch, NULL, &rect, D3DX_FILTER_NONE, 0);
  D3D_CHECKERROR(hr);

  // done
  pd3dSurf->Release();  // must not use D3DRELEASE, because freeing all istances will free texture also when using DXTC!? (Bravo MS!)
  FreeMemory(pulFlipped);
}



// fast conversion from RGBA memory format to one of D3D color formats


// LINEAR
         

static void ConvARGB8( ULONG *pulSrc, void *pDst, PIX pixWidth, PIX pixHeight, SLONG slDstPitch)
{
  const ULONG slRowModulo = slDstPitch - (pixWidth<<2);
  __asm {
    mov     esi,D [pulSrc]
    mov     edi,D [pDst]
    mov     ebx,D [pixHeight]
rowLoop:
    mov     ecx,D [pixWidth]
pixLoop:
    prefetchnta [esi+16]
    mov     eax,D [esi]
    or      eax,D [_ulAlphaMask]
    bswap   eax
    ror     eax,8
    mov     D [edi],eax
    add     esi,4
    add     edi,4
    dec     ecx
    jnz     pixLoop
    add     edi,D [slRowModulo]
    dec     ebx
    jnz     rowLoop
  }
}


static void ConvARGB5( ULONG *pulSrc, void *pDst, PIX pixWidth, PIX pixHeight, SLONG slDstPitch)
{
  const ULONG slRowModulo = slDstPitch - (pixWidth<<1);
  __asm {
    mov     esi,D [pulSrc]
    mov     edi,D [pDst]
    mov     ebx,D [pixHeight]
rowLoop:
    push    ebx
    mov     ecx,D [pixWidth]
pixLoop:
    prefetchnta [esi+16]
    mov     eax,D [esi]
    or      eax,D [_ulAlphaMask]
    mov     ebx,eax
    mov     edx,eax
    and     eax,0x000000F8 // R
    and     ebx,0x0000F800 // G
    shl     eax,7
    shr     ebx,6
    or      eax,ebx
    mov     ebx,edx
    and     ebx,0x00F80000 // B
    and     edx,0x80000000 // A
    shr     ebx,16+3
    shr     edx,16
    or      eax,ebx
    or      eax,edx
    mov     W [edi],ax
    add     esi,4
    add     edi,2
    dec     ecx
    jnz     pixLoop
    add     edi,D [slRowModulo]
    pop     ebx
    dec     ebx
    jnz     rowLoop
  }
}


static void ConvARGB4( ULONG *pulSrc, void *pDst, PIX pixWidth, PIX pixHeight, SLONG slDstPitch)
{
  const ULONG slRowModulo = slDstPitch - (pixWidth<<1);
  __asm {
    mov     esi,D [pulSrc]
    mov     edi,D [pDst]
    mov     ebx,D [pixHeight]
rowLoop:
    push    ebx
    mov     ecx,D [pixWidth]
pixLoop:
    prefetchnta [esi+16]
    mov     eax,D [esi]
    mov     ebx,eax
    mov     edx,eax
    and     eax,0x000000F0 // R
    and     ebx,0x0000F000 // G
    shl     eax,4
    shr     ebx,8
    or      eax,ebx
    mov     ebx,edx
    and     ebx,0x00F00000 // B
    and     edx,0xF0000000 // A
    shr     ebx,16+4
    shr     edx,16
    or      eax,ebx
    or      eax,edx
    mov     W [edi],ax
    add     esi,4
    add     edi,2
    dec     ecx
    jnz     pixLoop
    add     edi,D [slRowModulo]
    pop     ebx
    dec     ebx
    jnz     rowLoop
  }
}


static void ConvRGB5( ULONG *pulSrc, void *pDst, PIX pixWidth, PIX pixHeight, SLONG slDstPitch)
{
  const ULONG slRowModulo = slDstPitch - (pixWidth<<1);
  __asm {
    mov     esi,D [pulSrc]
    mov     edi,D [pDst]
    mov     ebx,D [pixHeight]
rowLoop:
    push    ebx
    mov     ecx,D [pixWidth]
pixLoop:
    prefetchnta [esi+16]
    mov     eax,D [esi]
    mov     ebx,eax
    mov     edx,eax
    and     eax,0x000000F8 // R
    and     ebx,0x0000FC00 // G
    and     edx,0x00F80000 // B
    shl     eax,8
    shr     ebx,5
    shr     edx,8+5+6
    or      eax,ebx
    or      eax,edx
    mov     W [edi],ax
    add     esi,4
    add     edi,2
    dec     ecx
    jnz     pixLoop
    add     edi,D [slRowModulo]
    pop     ebx
    dec     ebx
    jnz     rowLoop
  }
}


static void ConvAL8( ULONG *pulSrc, void *pDst, PIX pixWidth, PIX pixHeight, SLONG slDstPitch)
{
  const ULONG slRowModulo = slDstPitch - (pixWidth<<1);
  __asm {
    mov     esi,D [pulSrc]
    mov     edi,D [pDst]
    mov     ebx,D [pixHeight]
rowLoop:
    mov     ecx,D [pixWidth]
pixLoop:
    prefetchnta [esi+16]
    mov     eax,D [esi]
    ror     eax,8
    bswap   eax
    mov     W [edi],ax
    add     esi,4
    add     edi,2
    dec     ecx
    jnz     pixLoop
    add     edi,D [slRowModulo]
    dec     ebx
    jnz     rowLoop
  }
}



static void ConvL8( ULONG *pulSrc, void *pDst, PIX pixWidth, PIX pixHeight, SLONG slDstPitch)
{
  const ULONG slRowModulo = slDstPitch - (pixWidth<<0);
  __asm {
    mov     esi,D [pulSrc]
    mov     edi,D [pDst]
    mov     ebx,D [pixHeight]
rowLoop:
    mov     ecx,D [pixWidth]
pixLoop:
    prefetchnta [esi+16]
    mov     eax,D [esi]
    mov     B [edi],al
    add     esi,4
    add     edi,1
    dec     ecx
    jnz     pixLoop
    add     edi,D [slRowModulo]
    dec     ebx
    jnz     rowLoop
  }
}



// generates mask for U and V depending on texture dimenstion
inline void GetMasks( const PIX pixWidth, const PIX pixHeight, const INDEX iShiftBPT,
                      ULONG &ulMaskU, ULONG &ulMaskV, SLONG &slAddU, SLONG &slAddV) 
{
  // dimensions must not be to small
  ASSERT( pixWidth>=4 && pixHeight>=2);
  ASSERT( pixWidth<=4 || pixHeight!=2);

	const INDEX iLogWidth  = FastLog2(pixWidth); 
	const INDEX iLogHeight = FastLog2(pixHeight);
  const ULONG ulSizeMask = (1<<(iLogWidth+iLogHeight)) -1;
  if( iLogWidth>iLogHeight) {
    const ULONG ulMask = (1<<(iLogHeight<<1)) -1;
    ulMaskU = 0x55555555 | (~ulMask);
    ulMaskV = 0xAAAAAAAA & ulMask;
  } else {
    const ULONG ulMask = (1<<(iLogWidth<<1)) -1;
    ulMaskU = 0x55555555 & ulMask;
    ulMaskV = 0xAAAAAAAA | (~ulMask);
  }
  // we're letting U & V just loop, so we need to limit the bits to the ones we need
  ulMaskU &= ulSizeMask;
  ulMaskV &= ulSizeMask;
	slAddU = (-16) & ulMaskU;
	slAddV =  (-8) & ulMaskV;

  // Bits-per-Texel adjustment
  ulMaskU <<= iShiftBPT;  
  ulMaskV <<= iShiftBPT;  
  slAddU  <<= iShiftBPT;
  slAddV  <<= iShiftBPT;
}



static void SwizARGB8( ULONG *pulSrc, void *pDst, PIX pixWidth, PIX pixHeight, SLONG slDstPitch)
{
  // pitch must equals width
  const SLONG slSrcPitch = pixWidth*sizeof(ULONG);
  ASSERT( slDstPitch==(slSrcPitch>>0));

  // fetch masks and advancers
	ULONG ulMaskU, ulMaskV;
  SLONG slAddU,  slAddV;
  GetMasks( pixWidth,pixHeight, 2, ulMaskU,ulMaskV, slAddU,slAddV);

	// eax,edx=scratch, ebx=U, ecx=V, edi=dest/pitch, esi=pulSrc (incrimented)
	__asm {
		mov     esi,D [pulSrc]
    mov     edi,D [slSrcPitch]
		xor     ebx,ebx
		xor     ecx,ecx
    movd    mm7,D [_ulAlphaMask]
  punpckldq mm7,mm7

clusterLoop:
    movq    mm1,Q [esi+ 0]
    movq    mm2,Q [esi+ 8]
    movq    mm3,Q [esi+ edi+ 0]
    movq    mm4,Q [esi+ edi+ 8]

    prefetchnta [esi+ 16]
    prefetchnta [esi+ edi+ 16]
    add     esi,16
 		mov     edi,ebx				  // eax = U
		or      edi,ecx 				// eax = U | V
    add     edi,D [pDst]

    por     mm1,mm7
    por     mm2,mm7
    por     mm3,mm7
    por     mm4,mm7

    movd    eax,mm1
    psrlq   mm1,32
    movd    edx,mm1
    bswap   eax
    bswap   edx
    ror     eax,8
    ror     edx,8
    movd    mm1,eax
    movd    mm0,edx
  punpckldq mm1,mm0

    movd    eax,mm2
    psrlq   mm2,32
    movd    edx,mm2
    bswap   eax
    bswap   edx
    ror     eax,8
    ror     edx,8
    movd    mm2,eax
    movd    mm0,edx
  punpckldq mm2,mm0

    movd    eax,mm3
    psrlq   mm3,32
    movd    edx,mm3
    bswap   eax
    bswap   edx
    ror     eax,8
    ror     edx,8
    movd    mm3,eax
    movd    mm0,edx
  punpckldq mm3,mm0

    movd    eax,mm4
    psrlq   mm4,32
    movd    edx,mm4
    bswap   eax
    bswap   edx
    ror     eax,8
    ror     edx,8
    movd    mm4,eax
    movd    mm0,edx
  punpckldq mm4,mm0

		movntq  Q [edi+  0],mm1
		movntq  Q [edi+  8],mm3
		movntq  Q [edi+ 16],mm2
		movntq  Q [edi+ 24],mm4
    mov     edi,D [slSrcPitch]
	
		sub     ebx,D [slAddU]			// part 1 of adding 8 to U
		and     ebx,D [ulMaskU]			// part 2 of "U += 8"
		jnz     clusterLoop					// if U hasn't looped to 0, keep going

		add     esi,edi             // move source pointer 2 rows down
		sub     ecx,D [slAddV] 			// part 1 of "V += 4"
		and     ecx,D [ulMaskV]			// part 2 of "V += 4"
		jnz     clusterLoop					// if V hasn't looped to 0, repeat
    emms
	}
}


static void SwizARGB5( ULONG *pulSrc, void *pDst, PIX pixWidth, PIX pixHeight, SLONG slDstPitch)
{
  // pitch must equals width
  const SLONG slSrcPitch = pixWidth*sizeof(ULONG);
  ASSERT( slDstPitch==(slSrcPitch>>1));

  // fetch masks and advancers
	ULONG ulMaskU, ulMaskV;
  SLONG slAddU,  slAddV;
  GetMasks( pixWidth,pixHeight, 1, ulMaskU,ulMaskV, slAddU,slAddV);

	// eax,edx=scratch, ebx=U, ecx=V, edi=dest/pitch, esi=pulSrc (incrimented)
	__asm {
		mov     esi,D [pulSrc]
    mov     edi,D [slSrcPitch]
		xor     ebx,ebx
		xor     ecx,ecx
    movd    mm0,D [_ulAlphaMask]
  punpckldq mm0,mm0

clusterLoop:
    movq    mm1,Q [esi+ 0]
    movq    mm2,Q [esi+ 8]
    movq    mm3,Q [esi+ edi+ 0]
    movq    mm4,Q [esi+ edi+ 8]

    prefetchnta [esi+ 16]
    prefetchnta [esi+ edi+ 16]
    add     esi,16
 		mov     edi,ebx				  // eax = U
		or      edi,ecx 				// eax = U | V
    add     edi,D [pDst]

    por     mm1,mm0
    por     mm2,mm0
    por     mm3,mm0
    por     mm4,mm0

    movq    mm5,mm1
    movq    mm6,mm1
    movq    mm7,mm1
    pand    mm1,Q [_mm000000F8]
    pand    mm5,Q [_mm0000F800]
    pand    mm6,Q [_mm00F80000]
    pand    mm7,Q [_mm80000000]
    pslld   mm1,7
    psrld   mm5,6
    psrld   mm6,16+3
    psrld   mm7,16
    por     mm1,mm5
    por     mm6,mm7
    por     mm1,mm6

    movq    mm5,mm2
    movq    mm6,mm2
    movq    mm7,mm2
    pand    mm2,Q [_mm000000F8]
    pand    mm5,Q [_mm0000F800]
    pand    mm6,Q [_mm00F80000]
    pand    mm7,Q [_mm80000000]
    pslld   mm2,7   
    psrld   mm5,6   
    psrld   mm6,16+3
    psrld   mm7,16  
    por     mm2,mm5
    por     mm6,mm7
    por     mm2,mm6

    movq    mm5,mm3
    movq    mm6,mm3
    movq    mm7,mm3
    pand    mm3,Q [_mm000000F8]
    pand    mm5,Q [_mm0000F800]
    pand    mm6,Q [_mm00F80000]
    pand    mm7,Q [_mm80000000]
    pslld   mm3,7   
    psrld   mm5,6   
    psrld   mm6,16+3
    psrld   mm7,16  
    por     mm3,mm5
    por     mm6,mm7
    por     mm3,mm6

    movq    mm5,mm4
    movq    mm6,mm4
    movq    mm7,mm4
    pand    mm4,Q [_mm000000F8]
    pand    mm5,Q [_mm0000F800]
    pand    mm6,Q [_mm00F80000]
    pand    mm7,Q [_mm80000000]
    pslld   mm4,7   
    psrld   mm5,6   
    psrld   mm6,16+3
    psrld   mm7,16  
    por     mm4,mm5
    por     mm6,mm7
    por     mm4,mm6

    psubd   mm1,Q [_mmSignSub]
    psubd   mm2,Q [_mmSignSub]
    psubd   mm3,Q [_mmSignSub]
    psubd   mm4,Q [_mmSignSub]
   packssdw mm1,mm1
   packssdw mm2,mm2
   packssdw mm3,mm3
   packssdw mm4,mm4
  punpckldq mm1,mm3
  punpckldq mm2,mm4
    paddw   mm1,Q [_mmSignAdd]
    paddw   mm2,Q [_mmSignAdd]

		movntq  Q [edi+ 0],mm1
		movntq  Q [edi+ 8],mm2
    mov     edi,D [slSrcPitch]
	
		sub     ebx,D [slAddU]			// part 1 of adding 8 to U
		and     ebx,D [ulMaskU]			// part 2 of "U += 8"
		jnz     clusterLoop					// if U hasn't looped to 0, keep going

		add     esi,edi             // move source pointer 2 rows down
		sub     ecx,D [slAddV] 			// part 1 of "V += 4"
		and     ecx,D [ulMaskV]			// part 2 of "V += 4"
		jnz     clusterLoop					// if V hasn't looped to 0, repeat
    emms
	}
}


static void SwizARGB4( ULONG *pulSrc, void *pDst, PIX pixWidth, PIX pixHeight, SLONG slDstPitch)
{
  // pitch must equals width
  const SLONG slSrcPitch = pixWidth*sizeof(ULONG);
  ASSERT( slDstPitch==(slSrcPitch>>1));

  // fetch masks and advancers
	ULONG ulMaskU, ulMaskV;
  SLONG slAddU,  slAddV;
  GetMasks( pixWidth,pixHeight, 1, ulMaskU,ulMaskV, slAddU,slAddV);

	// eax,edx=scratch, ebx=U, ecx=V, edi=dest/pitch, esi=pulSrc (incrimented)
	__asm {
		mov     esi,D [pulSrc]
    mov     edi,D [slSrcPitch]
		xor     ebx,ebx
		xor     ecx,ecx
    movd    mm0,D [_ulAlphaMask]
  punpckldq mm0,mm0

clusterLoop:
    movq    mm1,Q [esi+ 0]
    movq    mm2,Q [esi+ 8]
    movq    mm3,Q [esi+ edi+ 0]
    movq    mm4,Q [esi+ edi+ 8]

    prefetchnta [esi+ 16]
    prefetchnta [esi+ edi+ 16]
    add     esi,16
 		mov     edi,ebx				  // eax = U
		or      edi,ecx 				// eax = U | V
    add     edi,D [pDst]

    por     mm1,mm0
    por     mm2,mm0
    por     mm3,mm0
    por     mm4,mm0

    movq    mm5,mm1
    movq    mm6,mm1
    movq    mm7,mm1
    pand    mm1,Q [_mm000000F0]
    pand    mm5,Q [_mm0000F000]
    pand    mm6,Q [_mm00F00000]
    pand    mm7,Q [_mmF0000000]
    pslld   mm1,4
    psrld   mm5,8
    psrld   mm6,16+4
    psrld   mm7,16
    por     mm1,mm5
    por     mm6,mm7
    por     mm1,mm6

    movq    mm5,mm2
    movq    mm6,mm2
    movq    mm7,mm2
    pand    mm2,Q [_mm000000F0]
    pand    mm5,Q [_mm0000F000]
    pand    mm6,Q [_mm00F00000]
    pand    mm7,Q [_mmF0000000]
    pslld   mm2,4
    psrld   mm5,8
    psrld   mm6,16+4
    psrld   mm7,16
    por     mm2,mm5
    por     mm6,mm7
    por     mm2,mm6

    movq    mm5,mm3
    movq    mm6,mm3
    movq    mm7,mm3
    pand    mm3,Q [_mm000000F0]
    pand    mm5,Q [_mm0000F000]
    pand    mm6,Q [_mm00F00000]
    pand    mm7,Q [_mmF0000000]
    pslld   mm3,4
    psrld   mm5,8
    psrld   mm6,16+4
    psrld   mm7,16
    por     mm3,mm5
    por     mm6,mm7
    por     mm3,mm6

    movq    mm5,mm4
    movq    mm6,mm4
    movq    mm7,mm4
    pand    mm4,Q [_mm000000F0]
    pand    mm5,Q [_mm0000F000]
    pand    mm6,Q [_mm00F00000]
    pand    mm7,Q [_mmF0000000]
    pslld   mm4,4
    psrld   mm5,8
    psrld   mm6,16+4
    psrld   mm7,16
    por     mm4,mm5
    por     mm6,mm7
    por     mm4,mm6

    psubd   mm1,Q [_mmSignSub]
    psubd   mm2,Q [_mmSignSub]
    psubd   mm3,Q [_mmSignSub]
    psubd   mm4,Q [_mmSignSub]
   packssdw mm1,mm1
   packssdw mm2,mm2
   packssdw mm3,mm3
   packssdw mm4,mm4
  punpckldq mm1,mm3
  punpckldq mm2,mm4
    paddw   mm1,Q [_mmSignAdd]
    paddw   mm2,Q [_mmSignAdd]

		movntq  Q [edi+ 0],mm1
		movntq  Q [edi+ 8],mm2
    mov     edi,D [slSrcPitch]
	
		sub     ebx,D [slAddU]			// part 1 of adding 8 to U
		and     ebx,D [ulMaskU]			// part 2 of "U += 8"
		jnz     clusterLoop					// if U hasn't looped to 0, keep going

		add     esi,edi             // move source pointer 2 rows down
		sub     ecx,D [slAddV] 			// part 1 of "V += 4"
		and     ecx,D [ulMaskV]			// part 2 of "V += 4"
		jnz     clusterLoop					// if V hasn't looped to 0, repeat
    emms
	}
}


static void SwizRGB5( ULONG *pulSrc, void *pDst, PIX pixWidth, PIX pixHeight, SLONG slDstPitch)
{
  // pitch must equals width
  const SLONG slSrcPitch = pixWidth*sizeof(ULONG);
  ASSERT( slDstPitch==(slSrcPitch>>1));

  // fetch masks and advancers
	ULONG ulMaskU, ulMaskV;
  SLONG slAddU,  slAddV;
  GetMasks( pixWidth,pixHeight, 1, ulMaskU,ulMaskV, slAddU,slAddV);

	// eax,edx=scratch, ebx=U, ecx=V, edi=dest/pitch, esi=pulSrc (incrimented)
	__asm {
		mov     esi,D [pulSrc]
    mov     edi,D [slSrcPitch]
		xor     ebx,ebx
		xor     ecx,ecx
    movq    mm0,Q [_mmSignSub]
    movq    mm7,Q [_mmSignAdd]

clusterLoop:
    movq    mm1,Q [esi+ 0]
    movq    mm2,Q [esi+ 8]
    movq    mm3,Q [esi+ edi+ 0]
    movq    mm4,Q [esi+ edi+ 8]

    prefetchnta [esi+ 16]
    prefetchnta [esi+ edi+ 16]
    add     esi,16
 		mov     edi,ebx				  // eax = U
		or      edi,ecx 				// eax = U | V
    add     edi,D [pDst]

    movq    mm5,mm1
    movq    mm6,mm1
    pand    mm1,Q [_mm000000F8]
    pand    mm5,Q [_mm0000FC00]
    pand    mm6,Q [_mm00F80000]
    pslld   mm1,8
    psrld   mm5,5
    psrld   mm6,8+5+6
    por     mm1,mm5
    por     mm1,mm6

    movq    mm5,mm2
    movq    mm6,mm2
    pand    mm2,Q [_mm000000F8]
    pand    mm5,Q [_mm0000FC00]
    pand    mm6,Q [_mm00F80000]
    pslld   mm2,8
    psrld   mm5,5
    psrld   mm6,8+5+6
    por     mm2,mm5
    por     mm2,mm6

    movq    mm5,mm3
    movq    mm6,mm3
    pand    mm3,Q [_mm000000F8]
    pand    mm5,Q [_mm0000FC00]
    pand    mm6,Q [_mm00F80000]
    pslld   mm3,8
    psrld   mm5,5
    psrld   mm6,8+5+6
    por     mm3,mm5
    por     mm3,mm6

    movq    mm5,mm4
    movq    mm6,mm4
    pand    mm4,Q [_mm000000F8]
    pand    mm5,Q [_mm0000FC00]
    pand    mm6,Q [_mm00F80000]
    pslld   mm4,8
    psrld   mm5,5
    psrld   mm6,8+5+6
    por     mm4,mm5
    por     mm4,mm6
    
    psubd   mm1,mm0
    psubd   mm2,mm0
    psubd   mm3,mm0
    psubd   mm4,mm0
   packssdw mm1,mm1
   packssdw mm2,mm2
   packssdw mm3,mm3
   packssdw mm4,mm4
  punpckldq mm1,mm3
  punpckldq mm2,mm4
    paddw   mm1,mm7
    paddw   mm2,mm7

		movntq  Q [edi+ 0],mm1
		movntq  Q [edi+ 8],mm2
    mov     edi,D [slSrcPitch]
	
		sub     ebx,D [slAddU]			// part 1 of adding 8 to U
		and     ebx,D [ulMaskU]			// part 2 of "U += 8"
		jnz     clusterLoop					// if U hasn't looped to 0, keep going

		add     esi,edi             // move source pointer 2 rows down
		sub     ecx,D [slAddV] 			// part 1 of "V += 4"
		and     ecx,D [ulMaskV]			// part 2 of "V += 4"
		jnz     clusterLoop					// if V hasn't looped to 0, repeat
    emms
	}
}


static void SwizAL8( ULONG *pulSrc, void *pDst, PIX pixWidth, PIX pixHeight, SLONG slDstPitch)
{
  // pitch must equals width
  const SLONG slSrcPitch = pixWidth*sizeof(ULONG);
  ASSERT( slDstPitch==(slSrcPitch>>1));

  // fetch masks and advancers
	ULONG ulMaskU, ulMaskV;
  SLONG slAddU,  slAddV;
  GetMasks( pixWidth,pixHeight, 1, ulMaskU,ulMaskV, slAddU,slAddV);

	// eax,edx=scratch, ebx=U, ecx=V, edi=dest/pitch, esi=pulSrc (incrimented)
	__asm {
		mov     esi,D [pulSrc]
    mov     edi,D [slSrcPitch]
		xor     ebx,ebx
		xor     ecx,ecx
    movq    mm0,Q [_mmSignSub]
    movq    mm7,Q [_mmSignAdd]

clusterLoop:
    movq    mm1,Q [esi+ 0]
    movq    mm2,Q [esi+ 8]
    movq    mm3,Q [esi+ edi+ 0]
    movq    mm4,Q [esi+ edi+ 8]

    prefetchnta [esi+ 16]
    prefetchnta [esi+ edi+ 16]
    add     esi,16
 		mov     edi,ebx				  // eax = U
		or      edi,ecx 				// eax = U | V
    add     edi,D [pDst]

    movq    mm5,mm1
    movq    mm6,mm2
    pand    mm1,Q [_mm000000FF]
    pand    mm2,Q [_mm000000FF]
    pand    mm5,Q [_mmFF000000]
    pand    mm6,Q [_mmFF000000]
    psrld   mm5,16
    psrld   mm6,16
    por     mm1,mm5
    por     mm2,mm6

    movq    mm5,mm3
    movq    mm6,mm4
    pand    mm3,Q [_mm000000FF]
    pand    mm4,Q [_mm000000FF]
    pand    mm5,Q [_mmFF000000]
    pand    mm6,Q [_mmFF000000]
    psrld   mm5,16
    psrld   mm6,16
    por     mm3,mm5
    por     mm4,mm6
    
    psubd   mm1,mm0
    psubd   mm2,mm0
    psubd   mm3,mm0
    psubd   mm4,mm0
   packssdw mm1,mm1
   packssdw mm2,mm2
   packssdw mm3,mm3
   packssdw mm4,mm4
  punpckldq mm1,mm3
  punpckldq mm2,mm4
    paddw   mm1,mm7
    paddw   mm2,mm7

		movntq  Q [edi+ 0],mm1
		movntq  Q [edi+ 8],mm2
    mov     edi,D [slSrcPitch]
	
		sub     ebx,D [slAddU]			// part 1 of adding 8 to U
		and     ebx,D [ulMaskU]			// part 2 of "U += 8"
		jnz     clusterLoop					// if U hasn't looped to 0, keep going

		add     esi,edi             // move source pointer 2 rows down
		sub     ecx,D [slAddV] 			// part 1 of "V += 4"
		and     ecx,D [ulMaskV]			// part 2 of "V += 4"
		jnz     clusterLoop					// if V hasn't looped to 0, repeat
    emms
	}
}


static void SwizL8( ULONG *pulSrc, void *pDst, PIX pixWidth, PIX pixHeight, SLONG slDstPitch)
{
  // pitch must equals width
  const SLONG slSrcPitch = pixWidth*sizeof(ULONG);
  ASSERT( slDstPitch==(slSrcPitch>>2));

  // fetch masks and advancers
	ULONG ulMaskU, ulMaskV;
  SLONG slAddU,  slAddV;
  GetMasks( pixWidth,pixHeight, 0, ulMaskU,ulMaskV, slAddU,slAddV);

	// eax,edx=scratch, ebx=U, ecx=V, edi=dest/pitch, esi=pulSrc (incrimented)
	__asm {
		mov     esi,D [pulSrc]
    mov     edi,D [slSrcPitch]
		xor     ebx,ebx
		xor     ecx,ecx
    movq    mm0,Q [_mmSignSub]
    movq    mm7,Q [_mmSignAdd]

clusterLoop:
    movq    mm1,Q [esi+ 0]
    movq    mm2,Q [esi+ 8]
    movq    mm3,Q [esi+ edi+ 0]
    movq    mm4,Q [esi+ edi+ 8]

    prefetchnta [esi+ 16]
    prefetchnta [esi+ edi+ 16]
    add     esi,16
 		mov     edi,ebx				  // eax = U
		or      edi,ecx 				// eax = U | V
    add     edi,D [pDst]

    pand    mm1,Q [_mm000000FF]
    pand    mm2,Q [_mm000000FF]
    pand    mm3,Q [_mm000000FF]
    pand    mm4,Q [_mm000000FF]
   packuswb mm1,mm3
   packuswb mm2,mm4
   packuswb mm1,mm2

		movntq  Q [edi],mm1
    mov     edi,D [slSrcPitch]
	
		sub     ebx,D [slAddU]			// part 1 of adding 8 to U
		and     ebx,D [ulMaskU]			// part 2 of "U += 8"
		jnz     clusterLoop					// if U hasn't looped to 0, keep going

		add     esi,edi             // move source pointer 2 rows down
		sub     ecx,D [slAddV] 			// part 1 of "V += 4"
		and     ecx,D [ulMaskV]			// part 2 of "V += 4"
		jnz     clusterLoop					// if V hasn't looped to 0, repeat
    emms
	}
}



// set color conversion routine
extern void SetInternalFormat_D3D( const D3DFORMAT d3dFormat)
{
  // skip if fast upload is not allowed
  extern INDEX d3d_bFastUpload;
  if( !d3d_bFastUpload) {
    pConvertMipmap = NULL;
    pSwizzleMipmap = NULL;
    _d3dLastFormat = (D3DFORMAT)1234567890;
    return; 
  }

  // if not the same as last time, try to set corresponding fast-upload routine
  if( _d3dLastFormat==d3dFormat) return;
  switch( d3dFormat) {
  case D3DFMT_A8R8G8B8:  _ulAlphaMask=0x00000000;  pSwizzleMipmap=SwizARGB8;  pConvertMipmap=ConvARGB8;  break;
  case D3DFMT_X8R8G8B8:  _ulAlphaMask=0xFF000000;  pSwizzleMipmap=SwizARGB8;  pConvertMipmap=ConvARGB8;  break;
  case D3DFMT_A1R5G5B5:  _ulAlphaMask=0x00000000;  pSwizzleMipmap=SwizARGB5;  pConvertMipmap=ConvARGB5;  break;
  case D3DFMT_X1R5G5B5:  _ulAlphaMask=0xFF000000;  pSwizzleMipmap=SwizARGB5;  pConvertMipmap=ConvARGB5;  break;
  case D3DFMT_A4R4G4B4:  _ulAlphaMask=0x00000000;  pSwizzleMipmap=SwizARGB4;  pConvertMipmap=ConvARGB4;  break;
  case D3DFMT_R5G6B5:    _ulAlphaMask=0x00000000;  pSwizzleMipmap=SwizRGB5;   pConvertMipmap=ConvRGB5;   break;
  case D3DFMT_A8L8:      _ulAlphaMask=0x00000000;  pSwizzleMipmap=SwizAL8;    pConvertMipmap=ConvAL8;    break;
  case D3DFMT_L8:        _ulAlphaMask=0x00000000;  pSwizzleMipmap=SwizL8;     pConvertMipmap=ConvL8;     break;
  default:  pSwizzleMipmap=NULL;  pConvertMipmap=NULL;  break; // must go thru D3DX :(
  }
  _d3dLastFormat = d3dFormat;
}



// convert one mipmap from memory to surface
extern void UploadMipmap_D3D( ULONG *pulSrc, LPDIRECT3DTEXTURE8 ptexDst, PIX pixWidth, PIX pixHeight, INDEX iMip)
{
    const BOOL bSwizzle = FALSE;

  // general case thru D3DX approach (for edge-case swizzler, too - don't ask why!)
  if( pConvertMipmap==NULL || (pixHeight==2 && pixWidth>4 && bSwizzle)) {
    ConvertAny( pulSrc, ptexDst, pixWidth, pixHeight, iMip);
    return;
  }

  // yeah! - optimized case :)
  HRESULT hr;
  D3DLOCKED_RECT rectLocked;
  hr = ptexDst->LockRect( iMip, &rectLocked, NULL, NONE);
  D3D_CHECKERROR(hr);
  
  // (only if size is sufficient for swizzling)
  if( !bSwizzle || pixWidth<4 || pixHeight<2) {
    // too small texture or plain D3D linear format - just convert
    pConvertMipmap( pulSrc, rectLocked.pBits, pixWidth, pixHeight, rectLocked.Pitch);
  } else {
    // swizzle and convert
    pSwizzleMipmap( pulSrc, rectLocked.pBits, pixWidth, pixHeight, rectLocked.Pitch);
  }

  // done
  hr = ptexDst->UnlockRect(iMip);
  D3D_CHECKERROR(hr);
}


// unpack from some of D3D color formats to COLOR
extern COLOR UnpackColor_D3D( UBYTE *pd3dColor, D3DFORMAT d3dFormat, SLONG &slColorSize)
{
  UWORD uw;
  UBYTE ubR,ubG,ubB;
  switch(d3dFormat) {
  case D3DFMT_X8R8G8B8:
  case D3DFMT_A8R8G8B8:
    ubB = pd3dColor[0];
    ubG = pd3dColor[1];
    ubR = pd3dColor[2];
    slColorSize = 4;
    break; 
  case D3DFMT_R5G6B5:
    uw  = (UWORD&)*pd3dColor;
    ubR = (uw&0xF800)>>8;  ubR |= ubR>>5;
    ubG = (uw&0x07E0)>>3;  ubG |= ubG>>6;
    ubB = (uw&0x001F)<<3;  ubB |= ubB>>5;
    slColorSize = 2;
    break;
  case D3DFMT_X1R5G5B5:
  case D3DFMT_A1R5G5B5:
    uw  = (UWORD&)*pd3dColor;
    ubR = (uw&0x7C00)>>7;  ubR |= ubR>>5;
    ubG = (uw&0x03E0)>>2;  ubG |= ubG>>5;
    ubB = (uw&0x001F)<<3;  ubB |= ubB>>5;
    slColorSize = 2;
    break;
  case D3DFMT_A4R4G4B4:
    uw  = (UWORD&)*pd3dColor;
    ubR = (uw&0x0F00)>>4;  ubR |= ubR>>4;
    ubG = (uw&0x00F0)>>0;  ubG |= ubG>>4;
    ubB = (uw&0x000F)<<4;  ubB |= ubB>>4;
    slColorSize = 2;
    break;
  default: // unsupported format
    ubR = ubG = ubB = 0;
    slColorSize = 0;
    break;
  }
  // done
  return RGBToColor(ubR,ubG,ubB);
}

