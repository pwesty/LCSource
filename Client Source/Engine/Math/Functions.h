#ifndef SE_INCL_FUNCTIONS_H
#define SE_INCL_FUNCTIONS_H
#ifdef PRAGMA_ONCE
	#pragma once
#endif


#define SQRTTABLESIZE   8192
#define SQRTTABLESIZELOG2 13


// asm shortcuts
#define O offset
#define Q qword ptr
#define D dword ptr
#define W  word ptr
#define B  byte ptr

/* 
 *  template implementations
 */

template<class Type>
__forceinline Type Abs( const Type x)
{
	return ( x>=Type(0) ? x : -x );
}

template<class Type>
__forceinline Type Max( const Type a, const Type b)
{
	return ( a<b ? b : a );
}

template<class Type>
__forceinline Type Min( const Type a, const Type b)
{
	return ( a>b ? b : a );
}

// linear interpolation
template<class Type>
__forceinline Type Lerp( const Type x0, const Type x1, const FLOAT fRatio)
{
			 if( fRatio==0) return x0;
	else if( fRatio==1) return x1;
	else return x0+(x1-x0)*fRatio;
}

template<class Type>
__forceinline Type Sgn( const Type x)
{
	return (x)>Type(0) ? Type(1):( x<0 ? Type(-1):Type(0) );
}

template<class Type>
__forceinline Type SgnNZ( const Type x)
{
	return (x)>=Type(0) ? Type(1):Type(-1);
}

template<class Type>
__forceinline void Swap( Type &a, Type &b)
{
	Type t=a; a=b;  b=t;
} 

template<class Type>
__forceinline Type ClampUp( const Type x, const Type uplimit)
{
	return ( x<=uplimit ? x : uplimit );
}

template<class Type>
__forceinline Type ClampDn( const Type x, const Type dnlimit)
{
	return ( x>=dnlimit ? x : dnlimit );
}

template<class Type>
__forceinline Type Clamp( const Type x, const Type dnlimit, const Type uplimit)
{
	return ( x>=dnlimit ? (x<=uplimit ? x : uplimit): dnlimit );
}

/* 
 *  fast implementations
 */


__forceinline DOUBLE Abs( const DOUBLE f) { return fabs(f); }
__forceinline FLOAT  Abs( const FLOAT f)  { return (FLOAT)fabs(f); }
__forceinline SLONG  Abs( const SLONG sl) { return labs(sl); }


/*
__forceinline FLOAT Min( const FLOAT fA, const FLOAT fB)
{
	FLOAT fRet;
	__asm {
		fld     D [fA]
		fld     D [fB]
		fucomi  st(0),st(1)
		fcmovnb st(0),st(1)
		ffree   st(1)
		fstp    D [fRet]
	}
	return fRet;
}

__forceinline FLOAT Max( const FLOAT fA, const FLOAT fB)
{
	FLOAT fRet;
	__asm {
		fld     D [fA]
		fld     D [fB]
		fucomi  st(0),st(1)
		fcmovb  st(0),st(1)
		ffree   st(1)
		fstp    D [fRet]
	}
	return fRet;
}


__forceinline SLONG Min( const SLONG slA, const SLONG slB)
{
	SLONG slRet;
	__asm {
		mov     eax,D [slA]
		cmp     eax,D [slB]
		cmovg   eax,D [slB]
		mov     D [slRet],eax
	}
	return slRet;
}

__forceinline ULONG Min( const ULONG slA, const ULONG slB)
{
	ULONG ulRet;
	__asm {
		mov     eax,D [slA]
		cmp     eax,D [slB]
		cmova   eax,D [slB]
		mov     D [ulRet],eax
	}
	return ulRet;
}

__forceinline SLONG Max( const SLONG slA, const SLONG slB)
{
	SLONG slRet;
	__asm {
		mov     eax,D [slA]
		cmp     eax,D [slB]
		cmovl   eax,D [slB]
		mov     D [slRet],eax
	}
	return slRet;
}

__forceinline ULONG Max( const ULONG slA, const ULONG slB)
{
	ULONG ulRet;
	__asm {
		mov     eax,D [slA]
		cmp     eax,D [slB]
		cmovb   eax,D [slB]
		mov     D [ulRet],eax
	}
	return ulRet;
}



__forceinline FLOAT ClampUp( const FLOAT f, const FLOAT fuplimit)
{
	FLOAT fRet;
	__asm {
		fld     D [fuplimit]
		fld     D [f]
		fucomi  st(0),st(1)
		fcmovnb st(0),st(1)
		fstp    D [fRet]
		fstp    st(0)
	}
	return fRet;
}

__forceinline FLOAT ClampDn( const FLOAT f, const FLOAT fdnlimit)
{
	FLOAT fRet;
	__asm {
		fld     D [fdnlimit]
		fld     D [f]
		fucomi  st(0),st(1)
		fcmovb  st(0),st(1)
		fstp    D [fRet]
		fstp    st(0)
	}
	return fRet;
}

__forceinline FLOAT Clamp( const FLOAT f, const FLOAT fdnlimit, const FLOAT fuplimit)
{
	FLOAT fRet;
	__asm {
		fld     D [fdnlimit]
		fld     D [fuplimit]
		fld     D [f]
		fucomi  st(0),st(2)
		fcmovb  st(0),st(2)
		fucomi  st(0),st(1)
		fcmovnb st(0),st(1)
		fstp    D [fRet]
		fcompp
	}
	return fRet;
}


__forceinline SLONG ClampDn( const SLONG sl, const SLONG sldnlimit)
{
	SLONG slRet;
	__asm {
		mov     eax,D [sl]
		cmp     eax,D [sldnlimit]
		cmovl   eax,D [sldnlimit]
		mov     D [slRet],eax
	}
	return slRet;
}

__forceinline SLONG ClampUp( const SLONG sl, const SLONG sluplimit)
{
	SLONG slRet;
	__asm {
		mov     eax,D [sl]
		cmp     eax,D [sluplimit]
		cmovg   eax,D [sluplimit]
		mov     D [slRet],eax
	}
	return slRet;
}

__forceinline SLONG Clamp( const SLONG sl, const SLONG sldnlimit, const SLONG sluplimit)
{
	SLONG slRet;
	__asm {
		mov     eax,D [sl]
		cmp     eax,D [sldnlimit]
		cmovl   eax,D [sldnlimit]
		cmp     eax,D [sluplimit]
		cmovg   eax,D [sluplimit]
		mov     D [slRet],eax
	}
	return slRet;
}

*/

/* 
 *  fast functions
 */

#define FP_ONE_BITS  0x3F800000

// fast reciprocal value
__forceinline FLOAT FastRcp( const FLOAT f)
{
	INDEX i = 2*FP_ONE_BITS - *(INDEX*)&(f); 
	FLOAT r = *(FLOAT*)&i;
	return( r * (2.0f - f*r));
}

// convert float from 0.0f to 1.0f -> ulong form 0 to 255
__forceinline ULONG NormFloatToByte( const FLOAT f)
{
		/* rcg10042001 !!! FIXME: Move this elsewhere. */
#ifdef _MSC_VER
	const FLOAT f255 = 255.0f;
	ULONG ulRet;
	__asm {
		fld   D [f]
		fmul  D [f255]
		fistp D [ulRet]
	}
	return ulRet;
#else
	assert((f >= 0.0) && (f <= 1.0));
	return( (ULONG) (f * 255.0) );
#endif
}

// convert ulong from 0 to 255 -> float form 0.0f to 255.0f
__forceinline FLOAT NormByteToFloat( const ULONG ul)
{
	return (FLOAT)ul * (1.0f/255.0f);
}


// fast float to int conversion
__forceinline SLONG FloatToInt( FLOAT f)
{
#if (defined USE_PORTABLE_C)
	return((SLONG) f);  /* best of luck to you. */

#elif (defined _MSC_VER)
	SLONG slRet;
	__asm {
		fld    D [f]
		fistp  D [slRet]
	}
	return slRet;

#elif (defined __GNUC__)
	SLONG slRet;
	__asm__ __volatile__ (
		"flds     (%%ebx)   \n\t"
		"fistpl   (%%esi)   \n\t"
				:
				: "b" (&f), "S" (&slRet)
				: "memory"
	);
	return(slRet);
#else
	#error Fill this in for your platform.
#endif
}

// log base 2 of any float numero
__forceinline FLOAT Log2( FLOAT f) {
#if (defined USE_PORTABLE_C)
	return (FLOAT)(log10(x)*3.321928094887);  // log10(x)/log10(2)

#elif (defined _MSC_VER)
	FLOAT fRet;
	_asm {
		fld1
		fld     D [f]
		fyl2x
		fstp    D [fRet]
	}
	return fRet;

#elif (defined __GNUC__)
	FLOAT fRet;
	__asm__ __volatile__ (
		"fld1               \n\t"
		"flds     (%%ebx)   \n\t"
		"fyl2x              \n\t"
		"fstps    (%%esi)   \n\t"
				:
				: "b" (&f), "S" (&fRet)
				: "memory"
	);
	return(fRet);
#else
	#error Fill this in for your platform.
#endif
}


// returns accurate values only for integers that are power of 2
__forceinline SLONG FastLog2( SLONG x)
{
#if (defined USE_PORTABLE_C)
	#error write me.

#elif (defined _MSC_VER)
	SLONG slRet;
	__asm {
		bsr   eax,D [x]
		mov   D [slRet],eax
	}
	return slRet;

#elif (defined __GNUC__)
	SLONG slRet;
	__asm__ __volatile__ (
		"bsrl  (%%ebx), %%eax     \n\t"
		"movl   %%eax, (%%esi)    \n\t"
				:
				: "b" (&x), "S" (&slRet)
				: "memory"
	);
	return(slRet);
#else
	#error Fill this in for your platform.
#endif
}

// returns log2 of first larger value that is a power of 2
__forceinline SLONG FastMaxLog2( SLONG x)
{ 
#if (defined USE_PORTABLE_C)
	#error write me.

#elif (defined _MSC_VER)
	SLONG slRet;
	__asm {
		bsr   eax,D [x]
		bsf   edx,D [x]
		cmp   edx,eax
		adc   eax,0
		mov   D [slRet],eax
	}
	return slRet;

#elif (defined __GNUC__)
	SLONG slRet;
	__asm__ __volatile__ (
		"bsrl  (%%ebx), %%eax     \n\t"
		"bsfl  (%%ebx), %%edx     \n\t"
		"cmpl  %%eax, %%edx       \n\t"
		"adcl  $0, %%eax          \n\t"
		"movl  %%eax, (%%esi)     \n\t"
				:
				: "b" (&x), "S" (&slRet)
				: "memory"
	);
	return(slRet);
#else
	#error Fill this in for your platform.
#endif
}



// square root (works with negative numbers)
__forceinline FLOAT Sqrt( FLOAT x) { return (FLOAT)sqrt( ClampDn( x, 0.0f)); }

//------------------------------------------------------------------------------
// GetAngle
// Explain: 두 이차원 백터 사이의 각을 구해줌 
// * 360 도 기준 
// Date : 2005-11-01,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
__forceinline float GetAngle( float x1, float y1, float x2, float y2 )
{
	float fTemp;

	float fSize1 = sqrtf( x1 * x1 + y1 * y1 );
	float fSize2 = sqrtf( x2 * x2 + y2 * y2 );

	fTemp = ( x1 * x2 + y1 * y2) / (fSize1 * fSize2 );

	return acosf( fTemp );
}

/*
 * Trigonometrical functions
 */

//#define ANGLE_MASK 0x3fff
#define ANGLE_SNAP (0.25f)   //0x0010
// Wrap angle to be between 0 and 360 degrees
__forceinline ANGLE WrapAngle(ANGLE a) {
//안태훈 수정 시작	//(For Performance)(0.2)
	return a - floorf(a / 360.0f) * 360.0f;
	//ANGLE result = a - floorf(a / 360.0f) * 360.0f;
	//result = result < 0 ? 360.0f + result : result;
	//return (ANGLE)result;
	//return (ANGLE) fmod( fmod(a,360.0) + 360.0, 360.0);  // 0..360
//안태훈 수정 끝	//(For Performance)(0.2)
}

// Normalize angle to be between -180 and +180 degrees
__forceinline ANGLE NormalizeAngle(ANGLE a) {
	return WrapAngle(a+ANGLE_180)-ANGLE_180;
}

// math constants
static const FLOAT    PI = (3.14159265359f);
static const FLOAT InvPI = (0.31830988618f);

// convert degrees into angle
__forceinline ANGLE AngleDeg(FLOAT fDegrees) {
	//return ANGLE (fDegrees*ANGLE_180/FLOAT(180.0));
	return fDegrees;
}
// convert radians into angle
__forceinline ANGLE AngleRad(FLOAT fRadians) {
	return ANGLE (fRadians*ANGLE_180/PI);
}
// convert radians into angle
__forceinline ANGLE AngleRad(DOUBLE dRadians) {
	return ANGLE (dRadians*ANGLE_180/PI);
}
// convert angle into degrees
__forceinline FLOAT DegAngle(ANGLE aAngle) {
	//return FLOAT (WrapAngle(aAngle)*FLOAT(180.0)/ANGLE_180);
	return WrapAngle(aAngle);
}
// convert angle into radians
__forceinline FLOAT RadAngle(ANGLE aAngle) {
	return FLOAT (WrapAngle(aAngle)*PI/ANGLE_180);
}

ENGINE_API FLOAT Sin(ANGLE a);
ENGINE_API FLOAT Cos(ANGLE a);
ENGINE_API FLOAT Tan(ANGLE a);

__forceinline ENGINE_API FLOAT SinFast(ANGLE a) { return (FLOAT)sin(a*(PI/ANGLE_180)); };
__forceinline ENGINE_API FLOAT CosFast(ANGLE a) { return (FLOAT)cos(a*(PI/ANGLE_180)); };
__forceinline ENGINE_API FLOAT TanFast(ANGLE a) { return (FLOAT)tan(a*(PI/ANGLE_180)); };

__forceinline ANGLE ASin(FLOAT y) {
	return AngleRad (asin(Clamp(y, -1.0f, 1.0f)));
}
__forceinline ANGLE ASin(DOUBLE y) {
	return AngleRad (asin(Clamp(y, -1.0, 1.0)));
}
__forceinline ANGLE ACos(FLOAT x) {
	return AngleRad (acos(Clamp(x, -1.0f, 1.0f)));
}
__forceinline ANGLE ACos(DOUBLE x) {
	return AngleRad (acos(Clamp(x, -1.0, 1.0)));
}
__forceinline ANGLE ATan(FLOAT z) {
	return AngleRad (atan(z));
}
__forceinline ANGLE ATan(DOUBLE z) {
	return AngleRad (atan(z));
}
__forceinline ANGLE ATan2(FLOAT y, FLOAT x) {
	return AngleRad (atan2(y, x));
}
__forceinline ANGLE ATan2(DOUBLE y, DOUBLE x) {
	return AngleRad (atan2(y, x));
}

// does "snap to grid" for given coordinate
ENGINE_API void Snap( FLOAT &fDest, FLOAT fStep);
ENGINE_API void Snap( DOUBLE &fDest, DOUBLE fStep);
// does "snap to grid" for given angle
//ENGINE_API void Snap( ANGLE &angDest, ANGLE angStep);


/* 
 *  linear interpolation, special functions for floats and angles
 */

__forceinline FLOAT LerpFLOAT(FLOAT f0, FLOAT f1, FLOAT fFactor)
{
	return f0+(f1-f0)*fFactor;
}

__forceinline ANGLE LerpANGLE(ANGLE a0, ANGLE a1, FLOAT fFactor)
{
	// calculate delta
	ANGLE aDelta = WrapAngle(a1)-WrapAngle(a0);
	// adjust delta not to wrap around 360
	if (aDelta>ANGLE_180) {
		aDelta-=ANGLE(ANGLE_360);
	} else if (aDelta<-ANGLE_180) {
		aDelta+=ANGLE(ANGLE_360);
	}
	// interpolate the delta
	return a0+ANGLE(fFactor*aDelta);
}

// Calculates ratio function /~~\ where 0<x<1, taking in consideration fade in and fade out percentages
// (ie. 0.2f means 20% fade in, 0.1f stands for 10% fade out)
__forceinline FLOAT CalculateRatio(FLOAT fCurr, FLOAT fMin, FLOAT fMax, FLOAT fFadeInRatio, FLOAT fFadeOutRatio)
{
	if(fCurr<=fMin || fCurr>=fMax)
	{
		return 0.0f;
	}
	FLOAT fDelta = fMax-fMin;
	FLOAT fRatio=(fCurr-fMin)/fDelta;
	if(fRatio<fFadeInRatio) {
		fRatio = Clamp( fRatio/fFadeInRatio, 0.0f, 1.0f);
	} else if(fRatio>(1-fFadeOutRatio)) {
		fRatio = Clamp( (1.0f-fRatio)/fFadeOutRatio, 0.0f, 1.0f);
	} else {
		fRatio = 1.0f;
	}
	return fRatio;
}

__forceinline FLOAT StepUp(FLOAT fCur, FLOAT fMin, FLOAT fMax)
{
	if( fCur<=fMin) return 0.0f;
	if( fCur>=fMax) return 1.0f;
	return (fCur-fMin)/(fMax-fMin);
}

__forceinline FLOAT StepDown(FLOAT fCur, FLOAT fMin, FLOAT fMax)
{
	if( fCur<=fMin) return 1.0f;
	if( fCur>=fMax) return 0.0f;
	return (fMax-fCur)/(fMax-fMin);
}

__forceinline DOUBLE StepUpD(FLOAT dCur, FLOAT dMin, FLOAT dMax)
{
	if( dCur<=dMin) return 0.0;
	if( dCur>=dMax) return 1.0;
	return (dCur-dMin)/(dMax-dMin);
}

__forceinline DOUBLE StepDownD(DOUBLE dCur, DOUBLE dMin, DOUBLE dMax)
{
	if( dCur<=dMin) return 1.0;
	if( dCur>=dMax) return 0.0;
	return (dMax-dCur)/(dMax-dMin);
}

#undef O
#undef Q
#undef D
#undef W
#undef B


#endif  /* include-once check. */

