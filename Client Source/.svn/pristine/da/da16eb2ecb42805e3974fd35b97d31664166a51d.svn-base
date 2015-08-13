#include "stdh.h"

#include <Engine/Graphics/Color.h>
#include <Engine/Math/Functions.h>

// asm shortcuts
#define O offset
#define Q qword ptr
#define D dword ptr
#define W  word ptr
#define B  byte ptr


// convert HSV components to CroTeam COLOR format
COLOR HSVToColor( UBYTE const ubH, UBYTE const ubS, UBYTE const ubV)
{
  if( ubS>1) {
    SLONG xH   = (SLONG)ubH *1536; // ->FIXINT/(256/6)
    INDEX iHlo = xH & 0xFFFF; 
    SLONG slP  = ((SLONG)ubV * (256-  (SLONG)ubS)) >>8;
    SLONG slQ  = ((SLONG)ubV * (256-(((SLONG)ubS*iHlo)>>16))) >>8;
    SLONG slT  = ((SLONG)ubV * (256-(((SLONG)ubS*(65536-iHlo))>>16))) >>8;
    switch( xH>>16) {
      case 0:  return RGBToColor(ubV,slT,slP);
      case 1:  return RGBToColor(slQ,ubV,slP);
      case 2:  return RGBToColor(slP,ubV,slT);
      case 3:  return RGBToColor(slP,slQ,ubV);
      case 4:  return RGBToColor(slT,slP,ubV);
      case 5:  return RGBToColor(ubV,slP,slQ);
      default: ASSERTALWAYS("WHAT???"); return C_BLACK;
    }
  } else return RGBToColor(ubV,ubV,ubV);
}


// convert CroTeam COLOR format to HSV components
void ColorToHSV( COLOR const colSrc, UBYTE &ubH, UBYTE &ubS, UBYTE &ubV)
{
  UBYTE ubR, ubG, ubB;
  ColorToRGB( colSrc, ubR,ubG,ubB);
  ubH = 0;
  ubS = 0;
  ubV = Max( Max(ubR,ubG),ubB);
  if( ubV>1) {
    SLONG slD = ubV - Min( Min(ubR,ubG),ubB);
    if( slD<1) return;
    ubS = (slD*255) /ubV; 
         if( ubR==ubV) ubH =   0+ (((SLONG)ubG-ubB)*85) / (slD*2);
    else if( ubG==ubV) ubH =  85+ (((SLONG)ubB-ubR)*85) / (slD*2);
    else               ubH = 170+ (((SLONG)ubR-ubG)*85) / (slD*2);
  }
}



// color checking routines

#define  GRAY_TRESHOLD  4
#define WHITE_TRESHOLD (255-GRAY_TRESHOLD)

BOOL IsGray( COLOR const col)
{
  UBYTE ubR,ubG,ubB;
  ColorToRGB( col, ubR,ubG,ubB);
  INDEX iMaxDelta = Max( Max(ubR,ubG),ubB) - Min( Min(ubR,ubG),ubB);
  if( iMaxDelta<GRAY_TRESHOLD) return TRUE;
  return FALSE;
}

BOOL IsBlack( COLOR const col)
{
  UBYTE ubR,ubG,ubB;
  ColorToRGB( col, ubR,ubG,ubB);
  if( ubR<GRAY_TRESHOLD && ubG<GRAY_TRESHOLD && ubB<GRAY_TRESHOLD) return TRUE;
  return FALSE;
}

BOOL IsWhite( COLOR const col)
{
  UBYTE ubR,ubG,ubB;
  ColorToRGB( col, ubR,ubG,ubB);
  if( ubR>WHITE_TRESHOLD && ubG>WHITE_TRESHOLD && ubB>WHITE_TRESHOLD) return TRUE;
  return FALSE;
}

BOOL IsBigger( COLOR const col1, COLOR const col2)
{
  UBYTE ubR1,ubG1,ubB1;
  UBYTE ubR2,ubG2,ubB2;
  ColorToRGB( col1, ubR1,ubG1,ubB1);
  ColorToRGB( col2, ubR2,ubG2,ubB2);
  SLONG slGray1 = (((SLONG)ubR1+ubG1+ubB1)*21846) >>16;
  SLONG slGray2 = (((SLONG)ubR2+ubG2+ubB2)*21846) >>16;
  return (slGray1>slGray2);
}


// has color same hue and saturation (with little tolerance) ?
BOOL CompareChroma( COLOR col1, COLOR col2)
{ 
  // make color1 bigger
  if( IsBigger(col2,col1)) Swap(col1,col2);

  // find biggest component
  SLONG slR1=0, slG1=0, slB1=0;
  SLONG slR2=0, slG2=0, slB2=0;
  ColorToRGB( col1, (UBYTE&)slR1, (UBYTE&)slG1, (UBYTE&)slB1);
  ColorToRGB( col2, (UBYTE&)slR2, (UBYTE&)slG2, (UBYTE&)slB2);
  SLONG slMax1 = Max(Max(slR1,slG1),slB1);
  SLONG slMax2 = Max(Max(slR2,slG2),slB2);
  // trivial?
  if( slMax1<GRAY_TRESHOLD || slMax2<GRAY_TRESHOLD) return TRUE;

  // find expected color
  SLONG slR,slG,slB, slDiv;
  if( slR1==slMax1) {
    slDiv = 65536 / slR1;
    slR =  slR2;
    slG = (slR2*slG1*slDiv)>>16;
    slB = (slR2*slB1*slDiv)>>16;
  } else if( slG1==slMax1) {
    slDiv = 65536 / slG1;
    slR = (slG2*slR1*slDiv)>>16;
    slG =  slG2;
    slB = (slG2*slB1*slDiv)>>16;
  } else {
    slDiv = 65536 / slB1;
    slR = (slB2*slR1*slDiv)>>16;
    slG = (slB2*slG1*slDiv)>>16;
    slB =  slB2;
  }

  // check expected color
  if( Abs(slR-slR2) > (GRAY_TRESHOLD/2)) return FALSE;
  if( Abs(slG-slG2) > (GRAY_TRESHOLD/2)) return FALSE;
  if( Abs(slB-slB2) > (GRAY_TRESHOLD/2)) return FALSE;
  return TRUE;
}


// find corresponding desaturated color (it's not same as gray!)
COLOR DesaturateColor( COLOR const col)
{
  UBYTE ubR,ubG,ubB,ubA, ubMax;
  ColorToRGBA( col, ubR,ubG,ubB,ubA);
  ubMax = Max(Max(ubR,ubG),ubB);
  return RGBAToColor( ubMax,ubMax,ubMax,ubA);
}


// adjust color saturation and/or hue
COLOR AdjustColor( COLOR const col, SLONG const slHueShift, SLONG const slSaturation)
{
  // nothing?
  if( slHueShift==0 && slSaturation==256) return col;
  // saturation?
  COLOR colRes = col;
  UBYTE ubA = (col&CT_AMASK)>>CT_ASHIFT;
  if( slSaturation!=256)
  { // calculate gray factor
    UBYTE ubR,ubG,ubB;
    ColorToRGB( col, ubR,ubG,ubB);
    SLONG slGray = (ubR*72 + ubG*152 + ubB*32)>>8;
    // saturate color components
    SLONG slR = slGray + (((ubR-slGray)*slSaturation)>>8);
    SLONG slG = slGray + (((ubG-slGray)*slSaturation)>>8);
    SLONG slB = slGray + (((ubB-slGray)*slSaturation)>>8);
    // clamp color components
    extern const UBYTE *_pubClipByte;
    colRes = RGBToColor( _pubClipByte[slR], _pubClipByte[slG], _pubClipByte[slB]);
  }
  // hue?
  if( slHueShift==0) return colRes|ubA;
  UBYTE ubH,ubS,ubV;
  ColorToHSV( colRes, ubH,ubS,ubV);
  ubH += slHueShift;
  return HSVAToColor( ubH,ubS,ubV,ubA);
}


// adjust color gamma correction
COLOR AdjustGamma( COLOR const col, FLOAT const fGamma)
{
  if( fGamma==1.0f || fGamma<0.2f) return col;
  const FLOAT f1oGamma = 1.0f / fGamma;
  const FLOAT f1o255   = 1.0f / 255.0f;
  UBYTE ubR,ubG,ubB,ubA;
  ColorToRGBA( col, ubR,ubG,ubB,ubA);
  ubR = ClampUp( NormFloatToByte(pow(ubR*f1o255,f1oGamma)), 255UL);
  ubG = ClampUp( NormFloatToByte(pow(ubG*f1o255,f1oGamma)), 255UL);
  ubB = ClampUp( NormFloatToByte(pow(ubB*f1o255,f1oGamma)), 255UL);
  return RGBAToColor( ubR,ubG,ubB,ubA);
}



// color lerping functions

void LerpColor( COLOR col0, COLOR col1, FLOAT fRatio, UBYTE &ubR, UBYTE &ubG, UBYTE &ubB)
{
	UBYTE ubR0, ubG0, ubB0;
	UBYTE ubR1, ubG1, ubB1;
	ColorToRGB( col0, ubR0, ubG0, ubB0);
	ColorToRGB( col1, ubR1, ubG1, ubB1);
	ubR = Lerp( ubR0, ubR1, fRatio);
	ubG = Lerp( ubG0, ubG1, fRatio);
	ubB = Lerp( ubB0, ubB1, fRatio);
}


COLOR LerpColor( COLOR col0, COLOR col1, FLOAT fRatio)
{
//안태훈 수정 시작	//(Open beta)(2004-12-15)
	UBYTE ubR0, ubG0, ubB0, ubA0;
	UBYTE ubR1, ubG1, ubB1, ubA1;
	ColorToRGBA( col0, ubR0, ubG0, ubB0, ubA0);
	ColorToRGBA( col1, ubR1, ubG1, ubB1, ubA1);
	ubR0 = Lerp( ubR0, ubR1, fRatio);
	ubG0 = Lerp( ubG0, ubG1, fRatio);
	ubB0 = Lerp( ubB0, ubB1, fRatio);
	ubA0 = Lerp( ubA0, ubA1, fRatio);
	return RGBAToColor( ubR0, ubG0, ubB0, ubA0);
/*
	COLOR colRet;
	__asm
	{
		pxor		mm0, mm0

		movd		mm1, col0
		punpcklbw	mm1, mm0
		movq		mm2, mm1
		punpcklwd	mm1, mm0
		punpckhwd	mm2, mm0
		cvtpi2ps	xmm1, mm1
		cvtpi2ps	xmm2, mm2
		movlhps		xmm1, xmm2	//col0를 4개의 float으로 변환
		
		movd		mm1, col1
		punpcklbw	mm1, mm0
		movq		mm2, mm1
		punpcklwd	mm1, mm0
		punpckhwd	mm2, mm0
		cvtpi2ps	xmm3, mm1
		cvtpi2ps	xmm4, mm2
		movlhps		xmm3, xmm4	//col1를 4개의 float으로 변환

		xorps		xmm2, xmm2
		movss		xmm2, fRatio
		movaps		xmm4, xmm2
		shufps		xmm2, xmm4, 0x00	//fRatio를 4개의 float에 복사한다.

		subps		xmm3, xmm1	//col1 - col0
		mulps		xmm3, xmm2	//(col1 - col0) * fRatio
		addps		xmm3, xmm1	//(col1 - col0) * fRatio + col0

		movhlps		xmm4, xmm3
		cvtps2pi	mm2, xmm4
		cvtps2pi	mm1, xmm3
		packssdw	mm1, mm0
		packssdw	mm0, mm2
		por			mm1, mm0
		pxor		mm0, mm0
		packuswb	mm1, mm0	//result를 4개의 unsigned byte로 변환
		movd		colRet, mm1
		emms
	}
	return colRet;
*/
}
//안태훈 수정 끝	//(Open beta)(2004-12-15)



// fast color multiply function - RES = 1ST * 2ND /255
COLOR MulColors( COLOR col1, COLOR col2) 
{
  if( col1==0xFFFFFFFF)   return col2;
  if( col2==0xFFFFFFFF)   return col1;
  if( col1==0 || col2==0) return 0;
  COLOR colRet;
//안태훈 수정 시작	//(Open beta)(2004-12-15)
/*
  __asm {
    xor     ebx,ebx
    // red 
    mov     eax,D [col1]
    and     eax,CT_RMASK
    shr     eax,CT_RSHIFT
    mov     ecx,eax
    shl     ecx,8
    or      eax,ecx
    mov     edx,D [col2]
    and     edx,CT_RMASK
    shr     edx,CT_RSHIFT
    mov     ecx,edx
    shl     ecx,8
    or      edx,ecx
    imul    eax,edx
    shr     eax,16+8
    shl     eax,CT_RSHIFT
    or      ebx,eax
    // green
    mov     eax,D [col1]
    and     eax,CT_GMASK
    shr     eax,CT_GSHIFT
    mov     ecx,eax
    shl     ecx,8
    or      eax,ecx
    mov     edx,D [col2]
    and     edx,CT_GMASK
    shr     edx,CT_GSHIFT
    mov     ecx,edx
    shl     ecx,8
    or      edx,ecx
    imul    eax,edx
    shr     eax,16+8
    shl     eax,CT_GSHIFT
    or      ebx,eax
    // blue
    mov     eax,D [col1]
    and     eax,CT_BMASK
    shr     eax,CT_BSHIFT
    mov     ecx,eax
    shl     ecx,8
    or      eax,ecx
    mov     edx,D [col2]
    and     edx,CT_BMASK
    shr     edx,CT_BSHIFT
    mov     ecx,edx
    shl     ecx,8
    or      edx,ecx
    imul    eax,edx
    shr     eax,16+8
    shl     eax,CT_BSHIFT
    or      ebx,eax
    // alpha
    mov     eax,D [col1]
    and     eax,CT_AMASK
    shr     eax,CT_ASHIFT
    mov     ecx,eax
    shl     ecx,8
    or      eax,ecx
    mov     edx,D [col2]
    and     edx,CT_AMASK
    shr     edx,CT_ASHIFT
    mov     ecx,edx
    shl     ecx,8
    or      edx,ecx
    imul    eax,edx
    shr     eax,16+8
    shl     eax,CT_ASHIFT
    or      ebx,eax
    // done
    mov     D [colRet],ebx
  }
*/
	__asm
	{
		movd		mm0, col1
		movd		mm1, col2
		pxor		mm3, mm3
		punpcklbw	mm0, mm3
		punpcklbw	mm1, mm3
		pmullw		mm0, mm1
		psrlw		mm0, 8
		packuswb	mm0, mm3	//ClampUp
		movd		colRet, mm0
		emms
	}
//안태훈 수정 끝	//(Open beta)(2004-12-15)
	return colRet;
}


// fast color additon function - RES = clamp (1ST + 2ND)
COLOR AddColors( COLOR col1, COLOR col2) 
{
  if( col1==0) return col2;
  if( col2==0) return col1;
  if( col1==0xFFFFFFFF || col2==0xFFFFFFFF) return 0xFFFFFFFF;
  COLOR colRet;
//안태훈 수정 시작	//(Open beta)(2004-12-15)
/*
  __asm {
    xor     ebx,ebx
    mov     esi,255
    // red 
    mov     eax,D [col1]
    and     eax,CT_RMASK
    shr     eax,CT_RSHIFT
    mov     edx,D [col2]
    and     edx,CT_RMASK
    shr     edx,CT_RSHIFT
    add     eax,edx
    cmp     esi,eax  // clamp
    sbb     ecx,ecx
    or      eax,ecx
    shl     eax,CT_RSHIFT
    and     eax,CT_RMASK
    or      ebx,eax
    // green
    mov     eax,D [col1]
    and     eax,CT_GMASK
    shr     eax,CT_GSHIFT
    mov     edx,D [col2]
    and     edx,CT_GMASK
    shr     edx,CT_GSHIFT
    add     eax,edx
    cmp     esi,eax  // clamp
    sbb     ecx,ecx
    or      eax,ecx
    shl     eax,CT_GSHIFT
    and     eax,CT_GMASK
    or      ebx,eax
    // blue
    mov     eax,D [col1]
    and     eax,CT_BMASK
    shr     eax,CT_BSHIFT
    mov     edx,D [col2]
    and     edx,CT_BMASK
    shr     edx,CT_BSHIFT
    add     eax,edx
    cmp     esi,eax  // clamp
    sbb     ecx,ecx
    or      eax,ecx
    shl     eax,CT_BSHIFT
    and     eax,CT_BMASK
    or      ebx,eax
    // alpha
    mov     eax,D [col1]
    and     eax,CT_AMASK
    shr     eax,CT_ASHIFT
    mov     edx,D [col2]
    and     edx,CT_AMASK
    shr     edx,CT_ASHIFT
    add     eax,edx
    cmp     esi,eax  // clamp
    sbb     ecx,ecx
    or      eax,ecx
    shl     eax,CT_ASHIFT
    and     eax,CT_AMASK
    or      ebx,eax
    // done
    mov     D [colRet],ebx
  }
 */
	__asm
	{
		movd		mm0, col1
		movd		mm1, col2
		pxor		mm2, mm2
		punpcklbw	mm0, mm2
		punpcklbw	mm1, mm2
		paddw		mm0, mm1	//add
		packuswb	mm0, mm2	//ClampUp
		movd		colRet, mm0
		emms
	}
//안태훈 수정 끝	//(Open beta)(2004-12-15)
	return colRet;
}



// multiple conversion from OpenGL color to DirectX color
extern void abgr2argb( ULONG *pulSrc, ULONG *pulDst, INDEX ct)
{
  __asm {
    mov   esi,dword ptr [pulSrc]
    mov   edi,dword ptr [pulDst]
    mov   ecx,dword ptr [ct]
    shr   ecx,2
    jz    colSkip4
colLoop4:
    push  ecx
    prefetchnta [esi+16]
    mov   eax,dword ptr [esi+ 0]
    mov   ebx,dword ptr [esi+ 4]
    mov   ecx,dword ptr [esi+ 8]
    mov   edx,dword ptr [esi+12]
    bswap eax
    bswap ebx
    bswap ecx
    bswap edx
    ror   eax,8
    ror   ebx,8
    ror   ecx,8
    ror   edx,8
    mov   dword ptr [edi+ 0],eax
    mov   dword ptr [edi+ 4],ebx
    mov   dword ptr [edi+ 8],ecx
    mov   dword ptr [edi+12],edx
    add   esi,4*4
    add   edi,4*4
    pop   ecx
    dec   ecx
    jnz   colLoop4
colSkip4:
    test  dword ptr [ct],2
    jz    colSkip2
    mov   eax,dword ptr [esi+0]
    mov   ebx,dword ptr [esi+4]
    bswap eax
    bswap ebx
    ror   eax,8
    ror   ebx,8
    mov   dword ptr [edi+0],eax
    mov   dword ptr [edi+4],ebx
    add   esi,4*2
    add   edi,4*2
colSkip2:
    test  dword ptr [ct],1
    jz    colSkip1
    mov   eax,dword ptr [esi]
    bswap eax
    ror   eax,8
    mov   dword ptr [edi],eax
colSkip1:
  }
}


// multiple conversion from Croteam color to current API format
extern void ColorAPI( COLOR *pcolSrc, ULONG *pulDst, INDEX ct)
{
  if( _iGfxAPI!=0 || _iGfxAPI!=1) {
    ASSERT(FALSE);
    memcpy( pulDst, pcolSrc, ct*sizeof(ULONG));
    return;
  }

  __asm {
    push  ebx
    mov   esi,dword ptr [pcolSrc]
    mov   edi,dword ptr [pulDst]
    mov   ecx,dword ptr [ct]

    cmp   dword ptr [_iGfxAPI],1
    je    convD3D

//convOGL:
    shr   ecx,2
    jz    skipOGL4
loopOGL4:
    push  ecx
    prefetchnta [esi+16]
    mov   eax,dword ptr [esi+ 0]
    mov   ebx,dword ptr [esi+ 4]
    mov   ecx,dword ptr [esi+ 8]
    mov   edx,dword ptr [esi+12]
    bswap eax
    bswap ebx
    bswap ecx
    bswap edx
    mov   dword ptr [edi+ 0],eax
    mov   dword ptr [edi+ 4],ebx
    mov   dword ptr [edi+ 8],ecx
    mov   dword ptr [edi+12],edx
    add   esi,4*4
    add   edi,4*4
    pop   ecx
    dec   ecx
    jnz   loopOGL4
skipOGL4:
    test  dword ptr [ct],2
    jz    skipOGL2
    mov   eax,dword ptr [esi+0]
    mov   ebx,dword ptr [esi+4]
    bswap eax
    bswap ebx
    mov   dword ptr [edi+0],eax
    mov   dword ptr [edi+4],ebx
    add   esi,4*2
    add   edi,4*2
skipOGL2:
    test  dword ptr [ct],1
    jz    convDone
    mov   eax,dword ptr [esi]
    bswap eax
    mov   dword ptr [edi],eax
    jmp   convDone

convD3D:
    shr   ecx,2
    jz    skipD3D4
loopD3D4:
    push  ecx
    prefetchnta [esi+16]
    mov   eax,dword ptr [esi+ 0]
    mov   ebx,dword ptr [esi+ 4]
    mov   ecx,dword ptr [esi+ 8]
    mov   edx,dword ptr [esi+12]
    ror   eax,8
    ror   ebx,8
    ror   ecx,8
    ror   edx,8
    mov   dword ptr [edi+ 0],eax
    mov   dword ptr [edi+ 4],ebx
    mov   dword ptr [edi+ 8],ecx
    mov   dword ptr [edi+12],edx
    add   esi,4*4
    add   edi,4*4
    pop   ecx
    dec   ecx
    jnz   loopD3D4
skipD3D4:
    test  dword ptr [ct],2
    jz    skipD3D2
    mov   eax,dword ptr [esi+0]
    mov   ebx,dword ptr [esi+4]
    ror   eax,8
    ror   ebx,8
    mov   dword ptr [edi+0],eax
    mov   dword ptr [edi+4],ebx
    add   esi,4*2
    add   edi,4*2
skipD3D2:
    test  dword ptr [ct],1
    jz    convDone
    mov   eax,dword ptr [esi]
    ror   eax,8
    mov   dword ptr [edi],eax

convDone:
  }
}
