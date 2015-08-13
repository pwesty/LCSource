#include "stdh.h"

#include <Engine/Interface/UIWindow.h>
#include <Engine/Base/Memory.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Base/CRC.h>
#include <Engine/Math/Functions.h>
#include <Engine/Math/Projection.h>
#include <Engine/Math/AABBox.h>
#include <Engine/Graphics/GfxLibrary.h>
#include <Engine/Graphics/DrawPort.h>
#include <Engine/Graphics/Raster.h>
#include <Engine/Graphics/ImageInfo.h>
#include <Engine/Graphics/Color.h>
#include <Engine/Graphics/Texture.h>
#include <Engine/Graphics/ViewPort.h>
#include <Engine/Graphics/Font.h>
#include <Engine/Interface/UITextureManager.h>		// yjpark
#include <Engine/Interface/UICommon.h>

#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Templates/StaticStackArray.cpp>

//강동민 수정 시작 로그인 처리 작업	07.10
// FIXME : Clip Near Plane 테스트를 위한...
#include <D3dx8.h>
//강동민 수정 끝 로그인 처리 작업		07.10

extern INDEX gfx_bDecoratedText;
extern INDEX ogl_iFinish;
extern INDEX d3d_iFinish;

// currently locked drawport
static CDrawPort *_pdpCurrent = NULL;

// wooss 050915
// For malaysia  wooss 060330
extern BOOL g_bIsMalEng;

// RECT HANDLING ROUTINES
		
#define ASCII_CHARACTER_COUNT	98
#define KATA_CHARACTER_COUNT	63

// GBK Code Page
#define GBK_HI_START			0x81
#define GBK_HI_END				0xff
#define GBK_LOW_START			0x40
#define GBK_LOW_END				0xfe	
#define GBK_HI_BYTE_COUNT		(GBK_LOW_END - GBK_LOW_START + 1)
#define GBK_LOW_BYTE_COUNT		(GBK_LOW_END - GBK_LOW_START + 1)
#define GBK_PAGE_PER_CHARACTER	1764
#define GBK_COUNT				42
const int cBrzFontOffsetV 	=	2;		// 브라질 폰트 출력시 문자 높이 오프셋

int FindVowel(const char* strText)
{
	if (strText == NULL)
		return 0;
	int len = strlen(strText);
	int vowelNum = 0;
	for (int i = 0; i < len; i++)
	{
		unsigned char cInsert = strText[i];
		// vowel display
		if (cInsert == 0xd1 || (cInsert >= 0xd4 && cInsert <= 0xda))
			vowelNum++;
		else if (cInsert >= 0xe7 && cInsert <= 0xee)
			vowelNum++;
	}
	return vowelNum;
}
//wooss 051001
int FindThaiLen(const char* strText, int fromText/*=0*/, int toText/*=-1*/)
{
	int thaiLen = 0;
	if (toText == -1)
		toText = strlen(strText);
	for (int i = fromText; i < toText; i++)
	{
		unsigned char cInsert = strText[i];
		// vowel display
		if (cInsert >= 0x00 && cInsert <= 0xa0)
			thaiLen += _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
		else if (cInsert == 0xd1 || (cInsert >= 0xd4 && cInsert <= 0xda))
			continue;
		else if (cInsert >= 0xe7 && cInsert <= 0xee)
			continue;
		else if (cInsert == 0xd3)
			thaiLen += _pUIFontTexMgr->GetFontWidthThai(cInsert) + _pUIFontTexMgr->GetFontSpacing() - 4;
		else
			thaiLen += _pUIFontTexMgr->GetFontWidthThai(cInsert) + _pUIFontTexMgr->GetFontSpacing();
	}
	return thaiLen;
}

static BOOL ClipToDrawPort( const CDrawPort *pdp, PIX &pixI, PIX &pixJ, PIX &pixW, PIX &pixH)
{
	pixI+=pdp->dp_MinI;
	pixJ+=pdp->dp_MinJ;
	if( pixI < pdp->dp_ScissorMinI) { pixW -= pdp->dp_ScissorMinI-pixI;  pixI = pdp->dp_ScissorMinI; }
	else if( pixI >= pdp->dp_ScissorMaxI) return FALSE;

	if( pixJ < pdp->dp_ScissorMinJ) { pixH -= pdp->dp_ScissorMinJ-pixJ;  pixJ = pdp->dp_ScissorMinJ; }
	else if( pixJ >= pdp->dp_ScissorMaxJ) return FALSE;

	if( pixW<1 || pixH<1) return FALSE;

	if( (pixI+pixW) > pdp->dp_ScissorMaxI) pixW = pdp->dp_ScissorMaxI - pixI;
	if( (pixJ+pixH) > pdp->dp_ScissorMaxJ) pixH = pdp->dp_ScissorMaxJ - pixJ;

	ASSERT( pixI>=pdp->dp_ScissorMinI && pixI<pdp->dp_ScissorMaxI || pdp->dp_ScissorMinI==pdp->dp_ScissorMaxI);
	ASSERT( pixJ>=pdp->dp_ScissorMinJ && pixJ<pdp->dp_ScissorMaxJ || pdp->dp_ScissorMinJ==pdp->dp_ScissorMaxJ);
	ASSERT( pixW>=0  && pixH>=0);

	pixI-=pdp->dp_MinI;
	pixJ-=pdp->dp_MinJ;

	return TRUE;
}



// set scissor (clipping) to window inside drawport
static void SetScissor( const CDrawPort *pdp, PIX pixI, PIX pixJ, PIX pixW, PIX pixH)
{
	ASSERT( pixI>=0 && pixI<pdp->GetWidth());
	ASSERT( pixJ>=0 && pixJ<pdp->GetHeight());
	ASSERT( pixW>0  && pixH>0);
	const PIX pixInvMinJ = pdp->dp_Raster->ra_Height - (pdp->dp_MinJ+pixJ+pixH);
	pglScissor( pdp->dp_MinI+pixI, pixInvMinJ, pixW, pixH);
	ASSERT( pglIsEnabled(GL_SCISSOR_TEST));
	OGL_CHECKERROR;
}


// reset scissor (clipping) to whole drawport
static void ResetScissor( const CDrawPort *pdp)
{
	// create scissor rect with lower left origin
	const PIX pixMinSI = pdp->dp_ScissorMinI;
	const PIX pixMaxSI = pdp->dp_ScissorMaxI;
	const PIX pixMinSJ = pdp->dp_Raster->ra_Height - pdp->dp_ScissorMaxJ;
	const PIX pixMaxSJ = pdp->dp_Raster->ra_Height - pdp->dp_ScissorMinJ;
	pglScissor( pixMinSI, pixMinSJ, pixMaxSI-pixMinSI, pixMaxSJ-pixMinSJ);
	ASSERT( pglIsEnabled(GL_SCISSOR_TEST));
	OGL_CHECKERROR;
}




// DRAWPORT ROUTINES

// default constructor
CDrawPort::CDrawPort(void)
{
	dp_Raster = NULL;
	dp_fWideAdjustment = 1.0f;
	dp_bRenderingOverlay = FALSE;
	dp_ScissorMinI = dp_MinI = 0;
	dp_ScissorMinJ = dp_MinJ = 0;
	dp_ScissorMaxI = dp_MaxI = 0;
	dp_ScissorMaxJ = dp_MaxJ = 0;
	dp_FontData = NULL;
	dp_pixTextCharSpacing = 1;
	dp_pixTextLineSpacing = 0;
	dp_fTextScaling = 1.0f;
	dp_fTextAspect  = 1.0f;
	dp_ulTextFlags = NONE;
	dp_fTextShadow = 0;
	dp_ulTextBlendingType = PBT_BLEND;
	dp_ulBlendingRA = 0;
	dp_ulBlendingGA = 0;
	dp_ulBlendingBA = 0;
	dp_ulBlendingA  = 0;
}
// destructor
CDrawPort::~CDrawPort(void)
{
	if (_pdpCurrent==this) {
		Unlock_internal();
	}
}
// assignment operation
CDrawPort &CDrawPort::operator=(CDrawPort &dpOriginal)
{
	if (_pdpCurrent==this) {
		Unlock_internal();
	}

	// copy all parameters
	dp_Raster = dpOriginal.dp_Raster;
	dp_ScissorMinI = dpOriginal.dp_ScissorMinI;
	dp_ScissorMinJ = dpOriginal.dp_ScissorMinJ;
	dp_ScissorMaxI = dpOriginal.dp_ScissorMaxI;
	dp_ScissorMaxJ = dpOriginal.dp_ScissorMaxJ;
	dp_MinI = dpOriginal.dp_MinI;
	dp_MinJ = dpOriginal.dp_MinJ;
	dp_MaxI = dpOriginal.dp_MaxI;
	dp_MaxJ = dpOriginal.dp_MaxJ;
	dp_FontData = dpOriginal.dp_FontData;
	dp_pixTextCharSpacing = dpOriginal.dp_pixTextCharSpacing;
	dp_pixTextLineSpacing = dpOriginal.dp_pixTextLineSpacing;
	dp_fTextScaling = dpOriginal.dp_fTextScaling;
	dp_fTextAspect  = dpOriginal.dp_fTextAspect;
	dp_ulTextFlags = dpOriginal.dp_ulTextFlags;
	dp_fTextShadow = dpOriginal.dp_fTextShadow;
	dp_ulTextBlendingType = dpOriginal.dp_ulTextBlendingType;
	dp_fWideAdjustment   = dpOriginal.dp_fWideAdjustment;
	dp_bRenderingOverlay = dpOriginal.dp_bRenderingOverlay;

	// reset rest of vars
	dp_ulBlendingRA = 0;
	dp_ulBlendingGA = 0;
	dp_ulBlendingBA = 0;
	dp_ulBlendingA  = 0;

	return *this;
}

extern void DrawPort_NoCurrentDrawPort(void)
{
	if (_pdpCurrent!=NULL) {
		_pdpCurrent->Unlock_internal();
	}
}
// copy constructor
CDrawPort::CDrawPort(const CDrawPort *pdpOriginal)
{
	// copy all parameters
	dp_Raster = pdpOriginal->dp_Raster;
	dp_ScissorMinI = pdpOriginal->dp_ScissorMinI;
	dp_ScissorMinJ = pdpOriginal->dp_ScissorMinJ;
	dp_ScissorMaxI = pdpOriginal->dp_ScissorMaxI;
	dp_ScissorMaxJ = pdpOriginal->dp_ScissorMaxJ;
	dp_MinI = pdpOriginal->dp_MinI;
	dp_MinJ = pdpOriginal->dp_MinJ;
	dp_MaxI = pdpOriginal->dp_MaxI;
	dp_MaxJ = pdpOriginal->dp_MaxJ;
	dp_FontData = pdpOriginal->dp_FontData;
	dp_pixTextCharSpacing = pdpOriginal->dp_pixTextCharSpacing;
	dp_pixTextLineSpacing = pdpOriginal->dp_pixTextLineSpacing;
	dp_fTextScaling = pdpOriginal->dp_fTextScaling;
	dp_fTextAspect  = pdpOriginal->dp_fTextAspect;
	dp_ulTextFlags = pdpOriginal->dp_ulTextFlags;
	dp_fTextShadow = pdpOriginal->dp_fTextShadow;
	dp_ulTextBlendingType = pdpOriginal->dp_ulTextBlendingType;
	dp_fWideAdjustment   = pdpOriginal->dp_fWideAdjustment;
	dp_bRenderingOverlay = pdpOriginal->dp_bRenderingOverlay;

	// reset rest of vars
	dp_ulBlendingRA = 0;
	dp_ulBlendingGA = 0;
	dp_ulBlendingBA = 0;
	dp_ulBlendingA  = 0;
}

// set physical rectangle in physical coordinates
void CDrawPort::SetPhysicalRectInPhysical(const PIXaabbox2D &rect)
{
	SetPhysicalRectInRaster( rect + PIX2D(dp_ScissorMinI, dp_ScissorMinJ) );
}

// set physical rectangle in logical coordinates
void CDrawPort::SetPhysicalRectInLogical(const PIXaabbox2D &rect)
{
	SetPhysicalRectInRaster( rect + PIX2D(dp_MinI, dp_MinJ) );
}
// set physical rectangle in raster coordinates (low-level function)
void CDrawPort::SetPhysicalRectInRaster(const PIXaabbox2D &rect)
{
	// get new scissor coords in raster coords
	PIX pixNewScissorMinI = rect.Min()(1);
	PIX pixNewScissorMinJ = rect.Min()(2);
	PIX pixNewScissorMaxI = rect.Max()(1);
	PIX pixNewScissorMaxJ = rect.Max()(2);

	// clamp from inside to original scissor
	pixNewScissorMinI = Clamp(pixNewScissorMinI, dp_ScissorMinI, dp_ScissorMaxI);
	pixNewScissorMinJ = Clamp(pixNewScissorMinJ, dp_ScissorMinJ, dp_ScissorMaxJ);
	pixNewScissorMaxI = Clamp(pixNewScissorMaxI, dp_ScissorMinI, dp_ScissorMaxI);
	pixNewScissorMaxJ = Clamp(pixNewScissorMaxJ, dp_ScissorMinJ, dp_ScissorMaxJ);

	// make sure the scissor is not inverted
	pixNewScissorMaxI = ClampDn(pixNewScissorMaxI, pixNewScissorMinI);
	pixNewScissorMaxJ = ClampDn(pixNewScissorMaxJ, pixNewScissorMinJ);

	// use the new scissor
	dp_ScissorMinI = pixNewScissorMinI;
	dp_ScissorMinJ = pixNewScissorMinJ;
	dp_ScissorMaxI = pixNewScissorMaxI;
	dp_ScissorMaxJ = pixNewScissorMaxJ;
}

// set logical rectangle in physical coordinates
void CDrawPort::SetLogicalRectInPhysical(const PIXaabbox2D &rect)
{
	SetLogicalRectInRaster( rect + PIX2D(dp_ScissorMinI, dp_ScissorMinJ) );
}

// set logical rectangle in logical coordinates
void CDrawPort::SetLogicalRectInLogical(const PIXaabbox2D &rect)
{
	SetLogicalRectInRaster( rect + PIX2D(dp_MinI, dp_MinJ) );
}

// set logical rectangle in raster coordinates (low-level function)
void CDrawPort::SetLogicalRectInRaster(const PIXaabbox2D &rect)
{
	// get new coords in raster coords
	PIX pixNewMinI = rect.Min()(1);
	PIX pixNewMinJ = rect.Min()(2);
	PIX pixNewMaxI = rect.Max()(1);
	PIX pixNewMaxJ = rect.Max()(2);

	// clamp from outside to current scissor
	pixNewMinI = ClampUp(pixNewMinI, dp_ScissorMinI);
	pixNewMinJ = ClampUp(pixNewMinJ, dp_ScissorMinJ);
	pixNewMaxI = ClampDn(pixNewMaxI, dp_ScissorMaxI);
	pixNewMaxJ = ClampDn(pixNewMaxJ, dp_ScissorMaxJ);

	// use the new coordinates
	dp_MinI = pixNewMinI;
	dp_MinJ = pixNewMinJ;
	dp_MaxI = pixNewMaxI;
	dp_MaxJ = pixNewMaxJ;
}

// create a smaller subdrawport
CDrawPort CDrawPort::CreateSubDrawPort(const PIXaabbox2D &rect)
{
	// make a copy
	CDrawPort dpResult = *this;

	// get new coords in raster coords
	PIXaabbox2D rectNew( rect + PIX2D(dp_MinI, dp_MinJ) );

	// adjust new coords
	dpResult.SetPhysicalRectInRaster(rectNew);
	dpResult.SetLogicalRectInRaster(rectNew);

	// return the copy
	return dpResult;
}

CDrawPort CDrawPort::CreateSubDrawPortRatio(const FLOATaabbox2D &rect)
{
	PIX pixMinI = rect.Min()(1)*GetWidth();
	PIX pixMinJ = rect.Min()(2)*GetHeight();
	PIX pixMaxI = rect.Max()(1)*GetWidth();
	PIX pixMaxJ = rect.Max()(2)*GetHeight();
	return CreateSubDrawPort(PIXaabbox2D( PIX2D(pixMinI, pixMinJ), PIX2D(pixMaxI, pixMaxJ) ));
}

// set cloned drawport dimensions
void CDrawPort::InitCloned( CDrawPort *pdpBase, DOUBLE rMinI,DOUBLE rMinJ, DOUBLE rSizeI,DOUBLE rSizeJ)
{
	*this = pdpBase->CreateSubDrawPortRatio(FLOATaabbox2D(FLOAT2D(rMinI, rMinJ), FLOAT2D(rMinI+rSizeI, rMinJ+rSizeJ) ));
}

/* Clone a drawport */
CDrawPort::CDrawPort( CDrawPort *pdpBase,
											DOUBLE rMinI,DOUBLE rMinJ, DOUBLE rSizeI,DOUBLE rSizeJ)
{
	InitCloned( pdpBase, rMinI,rMinJ, rSizeI,rSizeJ);
}

CDrawPort::CDrawPort( CDrawPort *pdpBase, const PIXaabbox2D &box)
{
	*this = pdpBase->CreateSubDrawPort(box);
}


// check if a drawport is dualhead
BOOL CDrawPort::IsDualHead(void)
{
	return GetWidth()*3 == GetHeight()*8;
}

// check if a drawport is dualhead
BOOL CDrawPort::IsTripleHead(void)
{
	return GetWidth()*3 == GetHeight()*12;
}


// check if a drawport is already wide screen
BOOL CDrawPort::IsWideScreen(void)
{
	return GetWidth()*9 == GetHeight()*16;
}


// returns unique drawports number
ULONG CDrawPort::GetID(void)
{
	ULONG ulCRC;
	CRC_Start(   ulCRC);
	CRC_AddLONG( ulCRC, (ULONG)dp_Raster);
	CRC_AddLONG( ulCRC, (ULONG)dp_MinI);
	CRC_AddLONG( ulCRC, (ULONG)dp_MinJ);
	CRC_AddLONG( ulCRC, (ULONG)dp_MaxI);
	CRC_AddLONG( ulCRC, (ULONG)dp_MaxJ);
	CRC_AddLONG( ulCRC, (ULONG)dp_ScissorMinI);
	CRC_AddLONG( ulCRC, (ULONG)dp_ScissorMinJ);
	CRC_AddLONG( ulCRC, (ULONG)dp_ScissorMaxI);
	CRC_AddLONG( ulCRC, (ULONG)dp_ScissorMaxJ);
	CRC_Finish(  ulCRC);
	return ulCRC;
}


// dual/triple head cloning
CDrawPort::CDrawPort( CDrawPort *pdpBase, const INDEX iMonitor)
{  
	ASSERT( (iMonitor>=-1 && iMonitor<=+1) || iMonitor==3);
	// if it is a triplehead drawport
	if( pdpBase->IsTripleHead() && iMonitor!=3) {
		static const DOUBLE d1o3 = 1.0 / 3.0;
		static const DOUBLE d2o3 = 2.0 / 3.0;
		switch(iMonitor) {
		case -1:  InitCloned( pdpBase,    0,0, d1o3,1);  break;
		case +1:  InitCloned( pdpBase, d2o3,0, d1o3,1);  break;
		default:  InitCloned( pdpBase, d1o3,0, d1o3,1);  break;
		}
	}
	// if it is a dualhead drawport
	else if( pdpBase->IsDualHead()) {
		// use left or right
		if( iMonitor==+1) {
			InitCloned( pdpBase, 0.5,0, 0.5,1);
		} else {
			InitCloned( pdpBase, 0,  0, 0.5,1);
		}
	}
	// if regular drawport
	else {
		// always use entire drawport
		InitCloned( pdpBase, 0,0, 1,1);
	}
}



//  wide-screen cloning
void CDrawPort::MakeWideScreen(CDrawPort *pdp)
{
	// already wide?
	if( IsWideScreen()) {
		pdp->InitCloned( this, 0,0, 1,1);
		return;
	}
	// make wide!
	else {
		// get size
		const PIX pixSizeI = GetWidth();
		const PIX pixSizeJ = GetHeight();
		// make horiz width
		PIX pixSizeJW = pixSizeI*9/16;
		// if already too wide
		if (pixSizeJW>pixSizeJ-10) {
			// no wide screen
			pdp->InitCloned( this, 0,0, 1,1);
			return;
		}
		// clear upper and lower blanks
		const PIX pixJ0 = (pixSizeJ-pixSizeJW)/2;
		SetAsCurrent();
		Fill(0, 0, pixSizeI, pixJ0, C_BLACK|CT_OPAQUE);
		Fill(0, pixJ0+pixSizeJW, pixSizeI, pixJ0, C_BLACK|CT_OPAQUE);
		// init
		pdp->InitCloned( this, 0, FLOAT(pixJ0)/pixSizeJ, 1, FLOAT(pixSizeJW)/pixSizeJ);
		pdp->dp_fWideAdjustment = 9.0f / 12.0f;
	}
}


/*****
 * Recalculate pixel dimensions from relative dimensions and raster size.
 */

// set orthogonal projection
void CDrawPort::SetOrtho(void) const
{
	// finish all pending render-operations (if required)
	ogl_iFinish = Clamp( ogl_iFinish, 0L, 3L);
	d3d_iFinish = Clamp( d3d_iFinish, 0L, 3L);
	if( (ogl_iFinish==3 && _pGfx->gl_eCurrentAPI==GAT_OGL) 
	 || (d3d_iFinish==3 && _pGfx->gl_eCurrentAPI==GAT_D3D)) gfxFinish(FALSE);

	// prepare ortho dimensions with lowerleft origin
	const PIX pixMinI  = dp_MinI;
	const PIX pixMinSI = dp_ScissorMinI;
	const PIX pixMaxSI = dp_ScissorMaxI;
	const PIX pixMaxJ  = dp_Raster->ra_Height - dp_MinJ;
	const PIX pixMinSJ = dp_Raster->ra_Height - dp_ScissorMaxJ;
	const PIX pixMaxSJ = dp_Raster->ra_Height - dp_ScissorMinJ;

	// init matrices (D3D needs sub-pixel adjustment)
	gfxSetOrtho( pixMinSI-pixMinI, pixMaxSI-pixMinI, pixMaxJ-pixMaxSJ, pixMaxJ-pixMinSJ, 0.0f, -1.0f, TRUE);
	gfxDepthRange(0,1);
	gfxSetViewMatrix(NULL);
	// disable face culling, custom clip plane and truform
	gfxCullFace(GFX_NONE);
	gfxDisableClipPlane();
	gfxDisableTruform();
}


// set given projection
void CDrawPort::SetProjection(CAnyProjection3D &apr)
{
	// finish all pending render-operations (if required)
	ogl_iFinish = Clamp( ogl_iFinish, 0L, 3L);
	d3d_iFinish = Clamp( d3d_iFinish, 0L, 3L);
	if( (ogl_iFinish==3 && _pGfx->gl_eCurrentAPI==GAT_OGL) 
	 || (d3d_iFinish==3 && _pGfx->gl_eCurrentAPI==GAT_D3D)) gfxFinish(FALSE);

	// if projection is mirrored/warped and mirroring is allowed
	if( apr->pr_bMirror || apr->pr_bWarp) 
	{
		// set custom clip plane 0 to mirror plane
		gfxEnableClipPlane();
		DOUBLE adViewPlane[4];
		adViewPlane[0] = +apr->pr_plMirrorView(1); 
		adViewPlane[1] = +apr->pr_plMirrorView(2); 
		adViewPlane[2] = +apr->pr_plMirrorView(3); 
		adViewPlane[3] = -apr->pr_plMirrorView.Distance(); 
		gfxClipPlane(adViewPlane); // NOTE: view clip plane is multiplied by inverse modelview matrix at time when specified
	}
//강동민 수정 시작 Water 구현		04.13  
	else if(apr->pr_bNiceWater)	
	{
		// set custom clip plane 0 to mirror plane
		// 반사 평면을 위한 클리핑 평면 0을 사용자 설정함.		
		gfxEnableClipPlane();
		DOUBLE adViewPlane[4];
		adViewPlane[0] = +apr->pr_plNiceWaterView(1); 
		adViewPlane[1] = +apr->pr_plNiceWaterView(2); 
		adViewPlane[2] = +apr->pr_plNiceWaterView(3); 
		adViewPlane[3] = -apr->pr_plNiceWaterView.Distance(); 
		gfxClipPlane(adViewPlane); 
	}
//강동민 수정 끝 Water 구현			04.13

	// if projection is not mirrored
	else 
	{
		// just disable custom clip plane 0
		gfxDisableClipPlane();
	}

	// if isometric projection
	// BackGround에서 쓰임...
	if( apr.IsIsometric()) 
	{
		CIsometricProjection3D &ipr = (CIsometricProjection3D&)*apr;
		const FLOAT2D vMin  = ipr.pr_ScreenBBox.Min()-ipr.pr_ScreenCenter;
		const FLOAT2D vMax  = ipr.pr_ScreenBBox.Max()-ipr.pr_ScreenCenter;
		const FLOAT fFactor = 1.0f / (ipr.ipr_ZoomFactor*ipr.pr_fViewStretch);
		const FLOAT fNear   = ipr.pr_NearClipDistance;
		const FLOAT fLeft   = +vMin(1) *fFactor;
		const FLOAT fRight  = +vMax(1) *fFactor;
		const FLOAT fTop    = -vMin(2) *fFactor;
		const FLOAT fBottom = -vMax(2) *fFactor;
		// if far clip plane is not specified use maximum expected dimension of the world
		FLOAT fFar = ipr.pr_FarClipDistance;
		if( fFar<0) fFar = 1E5f;  // max size 32768, 3D (sqrt(3)), rounded up
		gfxSetOrtho( fLeft, fRight, fTop, fBottom, fNear, fFar, FALSE);
	}
	// if perspective projection
	else 
	{
		ASSERT( apr.IsPerspective());
		CPerspectiveProjection3D &ppr = (CPerspectiveProjection3D&)*apr;
		const FLOAT fNear   = ppr.pr_NearClipDistance;
		FLOAT fLeft   = ppr.pr_plClipL(3) / ppr.pr_plClipL(1) *fNear;
		FLOAT fRight  = ppr.pr_plClipR(3) / ppr.pr_plClipR(1) *fNear;
		FLOAT fTop    = ppr.pr_plClipU(3) / ppr.pr_plClipU(2) *fNear;
		FLOAT fBottom = ppr.pr_plClipD(3) / ppr.pr_plClipD(2) *fNear;
		FLOAT fMpPI = (fRight-fLeft)/(dp_MaxI-dp_MinI);
		FLOAT fMpPJ = (fBottom-fTop)/(dp_MaxJ-dp_MinJ);
		fLeft   += (dp_ScissorMinI-dp_MinI)*fMpPI;
		fRight  += (dp_ScissorMaxI-dp_MaxI)*fMpPI;
		fTop    += (dp_ScissorMinJ-dp_MinJ)*fMpPJ;
		fBottom += (dp_ScissorMaxJ-dp_MaxJ)*fMpPJ;
		
		// if far clip plane is not specified use maximum expected dimension of the world
		FLOAT fFar = ppr.pr_FarClipDistance;
		if( fFar<0) fFar = 1E5f;  // max size 32768, 3D (sqrt(3)), rounded up

		// D3DTS_PROJECTION 설정함.
		gfxSetFrustum( fLeft, fRight, fTop, fBottom, fNear, fFar);
	}
	
	// set some rendering params
	gfxDepthRange( apr->pr_fDepthBufferNear, apr->pr_fDepthBufferFar);
	gfxCullFace(GFX_BACK);
	gfxSetViewMatrix(NULL);

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////		
	//----------Custom Clip Plane---------------------
	// if custom clip plane not supported
	// Using Custom Near Clip!
	extern INDEX gfx_bRenderReflection;
	if( apr->pr_bNiceWater && !(_pGfx->gl_ulFlags&GLF_D3D_CLIPPLANE))
	//if(gfx_bRenderReflection && !(_pGfx->gl_ulFlags&GLF_D3D_CLIPPLANE))
	{
		D3DXPLANE	plNew;
		HRESULT		hr;
		plNew.a		= +apr->pr_plNiceWaterView(1);
		plNew.b		= +apr->pr_plNiceWaterView(2);
		plNew.c		= +apr->pr_plNiceWaterView(3);
		plNew.d		= -apr->pr_plNiceWaterView.Distance();

		D3DXMATRIX  matClip;
		D3DXMatrixIdentity(&matClip);
		D3DXMATRIX	matProj;
		D3DXMATRIX	matView;
		hr			= _pGfx->gl_pd3dDevice->GetTransform( D3DTS_PROJECTION, &matProj);
		hr			= _pGfx->gl_pd3dDevice->GetTransform( D3DTS_VIEW, &matView);
		D3DXMatrixMultiply(&matClip, &matView, &matProj);
		
		// Normal값을 변환함.
		D3DXMatrixInverse(&matClip, NULL, &matClip);
		D3DXMatrixTranspose(&matClip, &matClip);

		// Ensure Near clip always faces away from Eye point.		
		// if (plNew.d > 0)
		// {
		// 	D3DXPlaneTransform(&plNew, &(-plNew), &matClip);
		//}
		//else
		//{
			D3DXPlaneTransform(&plNew, &plNew, &matClip);
		//}

		// Create a transform to convert the projection Matrix
		// to our custom near clip space.
		D3DXMATRIX matClipProj;
		D3DXMatrixIdentity(&matClipProj);
		matClipProj(0, 2) = plNew.a;
		matClipProj(1, 2) = plNew.b;
		matClipProj(2, 2) = plNew.c;
		matClipProj(3, 2) = plNew.d;

		// Create a new custom clip projection matrix
		D3DXMATRIX projClipMatrix = matProj * matClipProj;
		hr = _pGfx->gl_pd3dDevice->SetTransform( D3DTS_PROJECTION, &projClipMatrix);
	}
	//----------Custom Clip Plane---------------------
	gfxDisableTruform();
}



// (!!!! this should be recoded)
// lock/unlock is deprecated as external interface. we still use it internally, until the calls are 
// completely removed. in the mean time, setascurrent is implemented using lock/unlock with tracking 
// of the currently active drawport. 

void CDrawPort::Unlock_internal(void)
{
	dp_Raster->Unlock();
	_pGfx->UnlockDrawPort(this);

	if (_pdpCurrent==this) {
		_pdpCurrent=NULL;
	}
}


BOOL CDrawPort::Lock_internal(void)
{
	if (_pdpCurrent!=NULL) {
		_pdpCurrent->Unlock_internal();
	}

	BOOL bRasterLocked = dp_Raster->Lock();
	ASSERT(bRasterLocked);
	// try to lock drawport with driver
	BOOL bDrawportLocked = _pGfx->LockDrawPort(this);
	ASSERT(bDrawportLocked);
	_pdpCurrent = this; 
	
	return bRasterLocked;
}


void CDrawPort::SetAsCurrent(void)
{
	// just do the lock, it will unlock eventual active drawport
	Lock_internal();
}


// DRAWING ROUTINES -------------------------------------



// draw one point
void CDrawPort::DrawPoint( PIX pixI, PIX pixJ, COLOR col, PIX pixRadius/*=1*/) const
{
	// check API and radius
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);
	ASSERT( pixRadius>=0);
	if( pixRadius==0) return; // do nothing if radius is 0

	// setup rendering mode
	gfxDisableTexture(); 
	gfxDisableDepthTest();
	gfxDisableDepthWrite();
	gfxDisableAlphaTest();
	gfxEnableBlend();
	gfxBlendFunc( GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
	gfxEnableColorArray();

	// set point color/alpha and radius
	const FLOAT fR = pixRadius;

	// OpenGL
	if( eAPI==GAT_OGL) {
		const FLOAT fI = pixI+0.5f;
		const FLOAT fJ = pixJ+0.5f;
		glCOLOR(col);
		pglPointSize(fR);
		pglBegin(GL_POINTS);
			pglVertex2f(fI,fJ);
		pglEnd();
		OGL_CHECKERROR;
	} // Direct3D
	else if( eAPI==GAT_D3D) {
		HRESULT hr;
		const FLOAT fI = pixI+0.75f;
		const FLOAT fJ = pixJ+0.75f;
		const ULONG d3dColor = rgba2argb(col);
		CTVERTEX avtx = {fI,fJ,0, d3dColor, 0,0};
		hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_POINTSIZE, *((DWORD*)&fR));
		D3D_CHECKERROR(hr);
		// set vertex shader and draw
		d3dSetVertexShader(D3DFVF_CTVERTEX);
		hr = _pGfx->gl_pd3dDevice->DrawPrimitiveUP( D3DPT_POINTLIST, 1, &avtx, sizeof(CTVERTEX));
		D3D_CHECKERROR(hr);
	}
}


// draw one point in 3D
void CDrawPort::DrawPoint3D( FLOAT3D v, COLOR col, FLOAT fRadius/*=1.0f*/) const
{
	// check API and radius
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);
	ASSERT( fRadius>=0);
	if( fRadius==0) return; // do nothing if radius is 0

	// setup rendering mode
	gfxDisableTexture(); 
	gfxDisableDepthWrite();
	gfxDisableAlphaTest();
	gfxEnableBlend();
	gfxBlendFunc( GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
	gfxEnableColorArray();

	// OpenGL
	if( eAPI==GAT_OGL) {
		glCOLOR(col);
		pglPointSize(fRadius);
		pglBegin(GL_POINTS);
			pglVertex3f( v(1),v(2),v(3));
		pglEnd();
		OGL_CHECKERROR;
	} // Direct3D
	else if( eAPI==GAT_D3D) {
		HRESULT hr;
		const ULONG d3dColor = rgba2argb(col);
		CTVERTEX avtx = {v(1),v(2),v(3), d3dColor, 0,0};
		hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_POINTSIZE, *((DWORD*)&fRadius));
		D3D_CHECKERROR(hr);
		// set vertex shader and draw
		d3dSetVertexShader(D3DFVF_CTVERTEX);
		hr = _pGfx->gl_pd3dDevice->DrawPrimitiveUP( D3DPT_POINTLIST, 1, &avtx, sizeof(CTVERTEX));
		D3D_CHECKERROR(hr);
	}
}



// draw one line
void CDrawPort::DrawLine( PIX pixI0, PIX pixJ0, PIX pixI1, PIX pixJ1, COLOR col, ULONG typ/*=_FULL*/) const
{
	// check API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);

	// setup rendering mode
	gfxDisableDepthTest();
	gfxDisableDepthWrite();
	gfxDisableAlphaTest();
	gfxEnableBlend();
	gfxBlendFunc( GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);

	FLOAT fD;
	INDEX iTexFilter, iTexAnisotropy;
	if( typ==_FULL_) {
		// no pattern - just disable texturing
		gfxDisableTexture(); 
		fD = 0;
	} else {
		// revert to simple point-sample filtering without mipmaps
		INDEX iNewFilter=10, iNewAnisotropy=1;
		gfxGetTextureFiltering( iTexFilter, iTexAnisotropy);
		gfxSetTextureFiltering( iNewFilter, iNewAnisotropy);
		// prepare line pattern and mapping
		extern void gfxSetPattern( ULONG ulPattern); 
		gfxSetPattern(typ);
		fD = Max( Abs(pixI0-pixI1), Abs(pixJ0-pixJ1)) /32.0f;
	} 

	// OpenGL
	if( eAPI==GAT_OGL) {
		const FLOAT fI0 = pixI0+0.5f;  const FLOAT fJ0 = pixJ0+0.5f;
		const FLOAT fI1 = pixI1+0.5f;  const FLOAT fJ1 = pixJ1+0.5f;
		glCOLOR(col);
		pglBegin( GL_LINES);
			pglTexCoord2f( 0,0); pglVertex2f(fI0,fJ0);
			pglTexCoord2f(fD,0); pglVertex2f(fI1,fJ1);
		pglEnd();
		OGL_CHECKERROR;
	} // Direct3D
	else if( eAPI==GAT_D3D) {
		HRESULT hr;
		const FLOAT fI0 = pixI0+0.75f;  const FLOAT fJ0 = pixJ0+0.75f;
		const FLOAT fI1 = pixI1+0.75f;  const FLOAT fJ1 = pixJ1+0.75f;
		const ULONG d3dColor = rgba2argb(col);
		CTVERTEX avtxLine[2] = {
			{fI0,fJ0,0, d3dColor,  0,0},
			{fI1,fJ1,0, d3dColor, fD,0} };
		// set vertex shader and draw
		d3dSetVertexShader(D3DFVF_CTVERTEX);
		hr = _pGfx->gl_pd3dDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, avtxLine, sizeof(CTVERTEX));
		D3D_CHECKERROR(hr);
	}
	// revert to old filtering
	if( typ!=_FULL_) gfxSetTextureFiltering( iTexFilter, iTexAnisotropy);
}



// draw one line in 3D
void CDrawPort::DrawLine3D( FLOAT3D v0, FLOAT3D v1, COLOR col) const
{
	// check API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);

	// setup rendering mode
	gfxDisableTexture(); 
	gfxDisableDepthWrite();
	gfxDisableAlphaTest();
	gfxEnableBlend();
	gfxBlendFunc( GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
	gfxEnableColorArray();

	// OpenGL
	if( eAPI==GAT_OGL) {
		glCOLOR(col);
		pglBegin( GL_LINES);
			pglVertex3f( v0(1),v0(2),v0(3));
			pglVertex3f( v1(1),v1(2),v1(3));
		pglEnd();
		OGL_CHECKERROR;
	} // Direct3D
	else if( eAPI==GAT_D3D) {
		HRESULT hr;
		const ULONG d3dColor = rgba2argb(col);
		CTVERTEX avtxLine[2] = {
			{v0(1),v0(2),v0(3), d3dColor, 0,0},
			{v1(1),v1(2),v1(3), d3dColor, 0,0} };
		// set vertex shader and draw
		d3dSetVertexShader(D3DFVF_CTVERTEX);
		hr = _pGfx->gl_pd3dDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, avtxLine, sizeof(CTVERTEX));
		D3D_CHECKERROR(hr);
	}
}



// draw border
void CDrawPort::DrawBorder( PIX pixI, PIX pixJ, PIX pixWidth, PIX pixHeight, COLOR col, ULONG typ/*=_FULL_*/) const
{
	// check API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);

	// setup rendering mode
	gfxDisableDepthTest();
	gfxDisableDepthWrite();
	gfxDisableAlphaTest();
	gfxEnableBlend();
	gfxBlendFunc( GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
	gfxEnableColorArray();

	// for non-full lines, must have 
	FLOAT fD;
	INDEX iTexFilter, iTexAnisotropy;
	if( typ==_FULL_) {
		// no pattern - just disable texturing
		gfxDisableTexture(); 
		fD = 0;
	} else {
		// revert to simple point-sample filtering without mipmaps
		INDEX iNewFilter=10, iNewAnisotropy=1;
		gfxGetTextureFiltering( iTexFilter, iTexAnisotropy);
		gfxSetTextureFiltering( iNewFilter, iNewAnisotropy);
		// prepare line pattern
		extern void gfxSetPattern( ULONG ulPattern); 
		gfxSetPattern(typ);
		fD = Max( pixWidth, pixHeight) /32.0f;
	}

	const FLOAT fI0 = pixI+0.5f;
	const FLOAT fJ0 = pixJ+0.5f;
	const FLOAT fI1 = pixI-0.5f +pixWidth;
	const FLOAT fJ1 = pixJ-0.5f +pixHeight;

	// OpenGL
	if( eAPI==GAT_OGL) {
		glCOLOR(col);
		pglBegin( GL_LINES);
			pglTexCoord2f(0,0); pglVertex2f(fI0,fJ0);   pglTexCoord2f(fD,0);  pglVertex2f(fI1,  fJ0);  // up
			pglTexCoord2f(0,0); pglVertex2f(fI1,fJ0);   pglTexCoord2f(fD,0);  pglVertex2f(fI1,  fJ1);  // right
			pglTexCoord2f(0,0); pglVertex2f(fI0,fJ1);   pglTexCoord2f(fD,0);  pglVertex2f(fI1+1,fJ1);  // down
			pglTexCoord2f(0,0); pglVertex2f(fI0,fJ0+1); pglTexCoord2f(fD,0);  pglVertex2f(fI0,  fJ1);  // left
		pglEnd();
		OGL_CHECKERROR;
	}
	// Direct3D
	else if( eAPI==GAT_D3D) {
		HRESULT hr;
		const ULONG d3dColor = rgba2argb(col);
		CTVERTEX avtxLines[8] = { // setup lines
			{fI0,fJ0,  0, d3dColor, 0,0}, {fI1,  fJ0,0, d3dColor, fD,0},   // up
			{fI1,fJ0,  0, d3dColor, 0,0}, {fI1,  fJ1,0, d3dColor, fD,0},   // right
			{fI0,fJ1,  0, d3dColor, 0,0}, {fI1+1,fJ1,0, d3dColor, fD,0},   // down
			{fI0,fJ0+1,0, d3dColor, 0,0}, {fI0,  fJ1,0, d3dColor, fD,0} }; // left
		// set vertex shader and draw
		d3dSetVertexShader(D3DFVF_CTVERTEX);
		hr = _pGfx->gl_pd3dDevice->DrawPrimitiveUP( D3DPT_LINELIST, 4, avtxLines, sizeof(CTVERTEX));
		D3D_CHECKERROR(hr);
	}
	// revert to old filtering
	if( typ!=_FULL_) gfxSetTextureFiltering( iTexFilter, iTexAnisotropy);
}
 


// fill part of a drawport with a given color
void CDrawPort::Fill( PIX pixI, PIX pixJ, PIX pixWidth, PIX pixHeight, COLOR col) const
{
	// if color is tranlucent
	if( ((col&CT_AMASK)>>CT_ASHIFT) != CT_OPAQUE)
	{ // draw thru polygon
		Fill( pixI,pixJ, pixWidth,pixHeight, col,col,col,col);
		return;
	}

	// clip and eventually reject
	const BOOL bInside = ClipToDrawPort( this, pixI, pixJ, pixWidth, pixHeight);
	if( !bInside) return;

	// check API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);

	// OpenGL
	if( eAPI==GAT_OGL)
	{ 
		// determine if clear area is the whole drawport
		const BOOL bFullDP = (pixI==0 && pixJ==0 && pixWidth==GetWidth() && pixHeight==GetHeight());
		// do fast filling
		if( !bFullDP) SetScissor( this, pixI, pixJ, pixWidth, pixHeight);
		UBYTE ubR, ubG, ubB;
		ColorToRGB( col, ubR,ubG,ubB);
		pglClearColor( ubR/255.0f, ubG/255.0f, ubB/255.0f, 1.0f);
		pglClear( GL_COLOR_BUFFER_BIT);
		OGL_CHECKERROR;
		if( !bFullDP) ResetScissor(this);
	}
	// Direct3D
	else if( eAPI==GAT_D3D)
	{
		HRESULT hr;
		// must convert coordinates to raster (i.e. surface)
		pixI += dp_MinI;
		pixJ += dp_MinJ;
		const PIX pixRasterW = dp_Raster->ra_Width;
		const PIX pixRasterH = dp_Raster->ra_Height;
		const ULONG d3dColor = rgba2argb(col);
		// do fast filling
		if( pixI==0 && pixJ==0 && pixWidth==pixRasterW && pixHeight==pixRasterH) {
			hr = _pGfx->gl_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, d3dColor,0,0);
		} else {
			D3DRECT d3dRect = { pixI, pixJ, pixI+pixWidth, pixJ+pixHeight };
			hr = _pGfx->gl_pd3dDevice->Clear( 1, &d3dRect, D3DCLEAR_TARGET, d3dColor,0,0);
		} // done
		D3D_CHECKERROR(hr);
	}
}


// fill part of a drawport with a four corner colors
void CDrawPort::Fill( PIX pixI, PIX pixJ, PIX pixWidth, PIX pixHeight, 
											COLOR colUL, COLOR colUR, COLOR colDL, COLOR colDR) const
{
	// clip and eventually reject
	const BOOL bInside = ClipToDrawPort( this, pixI, pixJ, pixWidth, pixHeight);
	if( !bInside) return;

	// check API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);

	// setup rendering mode
	gfxDisableDepthTest();
	gfxDisableDepthWrite();
	gfxDisableAlphaTest();
	gfxDisableTexture();
	gfxEnableBlend();
	gfxBlendFunc( GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);

	// prepare coords
	const FLOAT fI0 = pixI;  const FLOAT fI1 = pixI +pixWidth; 
	const FLOAT fJ0 = pixJ;  const FLOAT fJ1 = pixJ +pixHeight;

	// render rectangle
	if( eAPI==GAT_OGL) {
		// thru OpenGL
		pglBegin(GL_QUADS);
			glCOLOR(colUL);  pglVertex2f(fI0,fJ0);   // up-left
			glCOLOR(colDL);  pglVertex2f(fI0,fJ1);   // down-left
			glCOLOR(colDR);  pglVertex2f(fI1,fJ1);   // down-right
			glCOLOR(colUR);  pglVertex2f(fI1,fJ0);   // up-right
		pglEnd();
	}
	else if( eAPI==GAT_D3D) { 
		// thru Direct3D
		HRESULT hr;
		const ULONG d3dColUL = rgba2argb(colUL);  const ULONG d3dColUR = rgba2argb(colUR);
		const ULONG d3dColDL = rgba2argb(colDL);  const ULONG d3dColDR = rgba2argb(colDR);
		CTVERTEX avtxTris[6] = {
			{fI0,fJ0,0, d3dColUL, 0,0}, {fI0,fJ1,0, d3dColDL, 0,1}, {fI1,fJ1,0, d3dColDR, 1,1},
			{fI0,fJ0,0, d3dColUL, 0,0}, {fI1,fJ1,0, d3dColDR, 1,1}, {fI1,fJ0,0, d3dColUR, 1,0} };
		// set vertex shader and draw
		d3dSetVertexShader(D3DFVF_CTVERTEX);
		hr = _pGfx->gl_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 2, avtxTris, sizeof(CTVERTEX));
		D3D_CHECKERROR(hr);
	}
}


// fill an entire drawport with a given color
void CDrawPort::Fill( COLOR col) const
{
	// if color is tranlucent
	if( ((col&CT_AMASK)>>CT_ASHIFT) != CT_OPAQUE)
	{ // draw thru polygon
		Fill( 0,0, GetWidth(),GetHeight(), col,col,col,col);
		return;
	}

	// check API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);

	// OpenGL
	if( eAPI==GAT_OGL)
	{ 
		// do fast filling
		UBYTE ubR, ubG, ubB;
		ColorToRGB( col, ubR,ubG,ubB);
		pglClearColor( ubR/255.0f, ubG/255.0f, ubB/255.0f, 1.0f);
		pglClear( GL_COLOR_BUFFER_BIT);
		OGL_CHECKERROR;
	}
	// Direct3D
	else if( eAPI==GAT_D3D)
	{
		const ULONG d3dColor = rgba2argb(col);
		HRESULT hr = _pGfx->gl_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, d3dColor,0,0);
		D3D_CHECKERROR(hr);
	}
}


// fill an part of Z-Buffer with a given value
// (must clear stencil buffer too in case it exist for the performance sake - we don't actually need it)
void CDrawPort::FillZBuffer( PIX pixI, PIX pixJ, PIX pixWidth, PIX pixHeight, FLOAT zval) const
{ 
	// check API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);

	// clip and eventually reject
	const BOOL bInside = ClipToDrawPort( this, pixI, pixJ, pixWidth, pixHeight);
	if( !bInside) return;

	gfxEnableDepthWrite();

	// OpenGL
	if( eAPI==GAT_OGL)
	{
		// fast clearing thru scissor
		SetScissor( this, pixI, pixJ, pixWidth, pixHeight);
		pglClearDepth(zval);
		GLbitfield bMask = GL_DEPTH_BUFFER_BIT;
		if( _pGfx->gl_ulFlags & GLF_STENCILBUFFER) {
			bMask |= GL_STENCIL_BUFFER_BIT;
			pglClearStencil(0);
		} // do it
		pglClear(bMask);
		OGL_CHECKERROR;
		ResetScissor(this);
	}
	// Direct3D
	else if( eAPI==GAT_D3D)
	{
		DWORD dwMask = D3DCLEAR_ZBUFFER;
		if( _pGfx->gl_ulFlags & GLF_STENCILBUFFER) dwMask |= D3DCLEAR_STENCIL;
		pixI += dp_MinI; // must convert coordinates to raster (i.e. surface)
		pixJ += dp_MinJ;
		// clear just part of the drawport
		D3DRECT d3dRect = { pixI, pixJ, pixI+pixWidth, pixJ+pixHeight };
		HRESULT hr = _pGfx->gl_pd3dDevice->Clear( 1, &d3dRect, dwMask, 0,zval,0);
		D3D_CHECKERROR(hr);
	}
}


// fill an entire Z-Buffer with a given value
// (must clear stencil buffer too in case it exist for the performance sake - we don't actually need it)
void CDrawPort::FillZBuffer( FLOAT zval) const
{ 
	// check API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);
	gfxEnableDepthWrite();

	// OpenGL
	if( eAPI==GAT_OGL)
	{
		pglClearDepth(zval);
		GLbitfield bMask = GL_DEPTH_BUFFER_BIT;
		if( _pGfx->gl_ulFlags & GLF_STENCILBUFFER) {
			bMask |= GL_STENCIL_BUFFER_BIT;
			pglClearStencil(0);
		} // do it
		pglClearStencil(0);
		pglClear(bMask);
		OGL_CHECKERROR
	}
	// Direct3D
	else if( eAPI==GAT_D3D)
	{
		// must clear stencil buffer too in case it exist (we don't need it) for the performance sake
		DWORD dwMask = D3DCLEAR_ZBUFFER;
		if( _pGfx->gl_ulFlags & GLF_STENCILBUFFER) dwMask |= D3DCLEAR_STENCIL;
		HRESULT hr = _pGfx->gl_pd3dDevice->Clear( 0, NULL, dwMask, 0,zval,0);
		D3D_CHECKERROR(hr);
	}
}


// grab screen
void CDrawPort::GrabScreen( class CImageInfo &iiGrabbedImage, INDEX iGrabZBuffer/*=0*/) const
{
	// check API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);
	extern INDEX ogl_bGrabDepthBuffer;
	const BOOL bGrabDepth = (eAPI==GAT_OGL) && ((iGrabZBuffer==1 && ogl_bGrabDepthBuffer) || iGrabZBuffer==2);

	// prepare image info's dimensions
	iiGrabbedImage.Clear();
	iiGrabbedImage.ii_Width  = GetWidth();
	iiGrabbedImage.ii_Height = GetHeight();
	iiGrabbedImage.ii_BitsPerPixel = bGrabDepth ? 32 : 24;

	// allocate memory for 24-bit raw picture and copy buffer context
	const PIX pixPicSize = iiGrabbedImage.ii_Width * iiGrabbedImage.ii_Height;
	const SLONG slBytes  = pixPicSize * iiGrabbedImage.ii_BitsPerPixel/8;
	iiGrabbedImage.ii_Picture = (UBYTE*)AllocMemory( slBytes);
	
	// OpenGL
	if( eAPI==GAT_OGL)
	{ 
		// determine drawport starting location inside raster
		const PIX pixStartI = dp_MinI;
		const PIX pixStartJ = dp_Raster->ra_Height-(dp_MinJ+GetHeight());
		pglReadPixels( pixStartI, pixStartJ, GetWidth(), GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)iiGrabbedImage.ii_Picture);
		OGL_CHECKERROR;
		// grab z-buffer to alpha channel, if needed
		if( bGrabDepth) {
			// grab
			FLOAT *pfZBuffer = (FLOAT*)AllocMemory( pixPicSize*sizeof(FLOAT));
			pglReadPixels( pixStartI, pixStartJ, GetWidth(), GetHeight(), GL_DEPTH_COMPONENT, GL_FLOAT, (GLvoid*)pfZBuffer);
			OGL_CHECKERROR;
			// convert
			UBYTE *pubZBuffer = (UBYTE*)pfZBuffer;
			for( INDEX i=0; i<pixPicSize; i++) pubZBuffer[i] = 255-NormFloatToByte(pfZBuffer[i]);
			// add as alpha channel
			AddAlphaChannel( iiGrabbedImage.ii_Picture, (ULONG*)iiGrabbedImage.ii_Picture,
											 iiGrabbedImage.ii_Width * iiGrabbedImage.ii_Height, pubZBuffer);
			FreeMemory(pfZBuffer);
		}
		// flip image vertically
		FlipBitmap( iiGrabbedImage.ii_Picture, iiGrabbedImage.ii_Picture,
								iiGrabbedImage.ii_Width, iiGrabbedImage.ii_Height, 2, iiGrabbedImage.ii_BitsPerPixel==32);
	}

	// Direct3D
	else if( eAPI==GAT_D3D)
	{
		// get back buffer
		HRESULT hr;
		D3DLOCKED_RECT rectLocked;
		D3DSURFACE_DESC surfDesc;
		LPDIRECT3DSURFACE8 pBackBuffer;
		const BOOL bFullScreen = _pGfx->gl_ulFlags & GLF_FULLSCREEN;
		if( bFullScreen) hr = _pGfx->gl_pd3dDevice->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
		else hr = dp_Raster->ra_pvpViewPort->vp_pSwapChain->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
		D3D_CHECKERROR(hr);
		pBackBuffer->GetDesc(&surfDesc);
		ASSERT( surfDesc.Width==dp_Raster->ra_Width && surfDesc.Height==dp_Raster->ra_Height);
		const RECT rectToLock = { dp_MinI, dp_MinJ, dp_MaxI, dp_MaxJ };
		hr = pBackBuffer->LockRect( &rectLocked, &rectToLock, D3DLOCK_READONLY);
		D3D_CHECKERROR(hr);

		// prepare to copy'n'convert
		SLONG slColSize;    
		UBYTE *pubSrc = (UBYTE*)rectLocked.pBits;
		UBYTE *pubDst = iiGrabbedImage.ii_Picture;
		// loop thru rows
		for( INDEX j=0; j<GetHeight(); j++) {
			// loop thru pixles in row
			for( INDEX i=0; i<GetWidth(); i++) {
				UBYTE ubR,ubG,ubB;
				extern COLOR UnpackColor_D3D( UBYTE *pd3dColor, D3DFORMAT d3dFormat, SLONG &slColorSize);
				COLOR col = UnpackColor_D3D( pubSrc, surfDesc.Format, slColSize);
				ColorToRGB( col, ubR,ubG,ubB);
				*pubDst++ = ubR;
				*pubDst++ = ubG;
				*pubDst++ = ubB;
				pubSrc += slColSize;
			} // advance modulo
			pubSrc += rectLocked.Pitch - (GetWidth()*slColSize);
		} // all done
		pBackBuffer->UnlockRect();
		D3DRELEASE( pBackBuffer, TRUE);
	}
}


//강동민 수정 시작 Water 구현		04.22
//BOOL CDrawPort::IsPointVisible(PIX pixI, PIX pixJ, FLOAT fOoK, INDEX iID, INDEX iMirrorLevel/*=0*/) const		// 원본.
//강동민 수정 끝 Water 구현			04.22
BOOL CDrawPort::IsPointVisible(CAnyProjection3D &apr, PIX pixI, PIX pixJ, FLOAT fOoK, INDEX iID, INDEX iMirrorLevel/*=0*/) const
{
	// must have raster!
	if( dp_Raster==NULL) { ASSERT(FALSE);  return FALSE; }

	// if the point is out or at the edge of drawport, it is not visible by default
	if( pixI<1 || pixI>GetWidth()-2 || pixJ<1 || pixJ>GetHeight()-2) return FALSE;

	// check API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);

//강동민 수정 시작 Water 구현		04.20	
	if(!apr->pr_bNiceWater)
	{
//강동민 수정 끝 Water 구현			04.20
		// use delayed mechanism for checking
		extern BOOL CheckDepthPoint( const CDrawPort *pdp, PIX pixI, PIX pixJ, FLOAT fOoK, INDEX iID, INDEX iMirrorLevel=0);
		return CheckDepthPoint( this, pixI, pixJ, fOoK, iID, iMirrorLevel);
//강동민 수정 시작 Water 구현		04.20
	}
//강동민 수정 끝 Water 구현			04.20
//강동민 수정 시작 Water 구현		04.22
	return TRUE;
//강동민 수정 끝 Water 구현			04.22
}


void CDrawPort::RenderLensFlare( CTextureObject *pto, FLOAT fI, FLOAT fJ,
								 FLOAT fSizeI, FLOAT fSizeJ, ANGLE aRotation, COLOR colLight) const
{
	// check API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);

	// setup rendering mode
	gfxEnableDepthTest();
	gfxSetBlendType(PBT_ADD);
	gfxResetArrays();
	GFXVertex   *pvtx = _avtxCommon.Push(4);
	GFXTexCoord *ptex = _atexCommon[0].Push(4);
	GFXColor    *pcol = _acolCommon.Push(4);

	// find lens location and dimension
	const FLOAT fRI = fSizeI*0.5f;
	const FLOAT fRJ = fSizeJ*0.5f;
	extern const FLOAT *_pfSinTable;
	extern const FLOAT *_pfCosTable;
	const INDEX iRot256 = FloatToInt(aRotation*0.7111f) & 255; // *256/360
	const FLOAT fSinA = _pfSinTable[iRot256];
	const FLOAT fCosA = _pfCosTable[iRot256];
	const FLOAT fRICosA = fRI * -fCosA;
	const FLOAT fRJSinA = fRJ * +fSinA;
	const FLOAT fRISinA = fRI * -fSinA;
	const FLOAT fRJCosA = fRJ * -fCosA;

	// get texture parameters for current frame and needed mip factor
	CTextureData *ptd = (CTextureData*)pto->GetData();
	ptd->SetAsCurrent(pto->GetFrame());
	const GFXColor glcol(colLight);

	// prepare coordinates of the rectangle
	pvtx[0].x = fI- fRICosA+fRJSinA;  pvtx[0].y = fJ- fRISinA+fRJCosA;  pvtx[0].z = 0.01f;
	pvtx[1].x = fI- fRICosA-fRJSinA;  pvtx[1].y = fJ- fRISinA-fRJCosA;  pvtx[1].z = 0.01f;
	pvtx[2].x = fI+ fRICosA-fRJSinA;  pvtx[2].y = fJ+ fRISinA-fRJCosA;  pvtx[2].z = 0.01f;
	pvtx[3].x = fI+ fRICosA+fRJSinA;  pvtx[3].y = fJ+ fRISinA+fRJCosA;  pvtx[3].z = 0.01f;
	ptex[0].s = 0;  ptex[0].t = 0;
	ptex[1].s = 0;  ptex[1].t = 1;
	ptex[2].s = 1;  ptex[2].t = 1;
	ptex[3].s = 1;  ptex[3].t = 0;
	pcol[0] = glcol;
	pcol[1] = glcol;
	pcol[2] = glcol;
	pcol[3] = glcol;
	// render it
	_pGfx->gl_ctWorldElements += 6; 
	gfxFlushQuads();
}



/********************************************************
 * Routines for manipulating drawport's text capabilities
 */


// sets font to be used to printout some text on this drawport 
// WARNING: this resets text spacing, scaling and mode variables
void CDrawPort::SetFont( CFontData *pfd)
{
	// check if we're using font that's not even loaded yet
	ASSERT( pfd!=NULL); 

	// adjust char mapping if texture was reduced
	if( (pfd->fd_ptdTextureData->td_ulFlags&TEX_DISPOSED) && !(pfd->fd_ulFlags&FNF_DISPOSED)) {
		pfd->fd_ulFlags |= FNF_DISPOSED;
		pfd->fd_pixCharWidth   >>= 1; // one should be enough
		pfd->fd_pixCharHeight  >>= 1;
		pfd->fd_pixCharSpacing >>= 1; 
		pfd->fd_pixLineSpacing >>= 1;
		for( INDEX iChar=0; iChar<256; iChar++) {
			CFontCharData &fcd = pfd->fd_fcdFontCharData[iChar];
			fcd.fcd_pixXOffset >>= 1;  
			fcd.fcd_pixYOffset >>= 1;
			fcd.fcd_pixStart   >>= 1;
			fcd.fcd_pixEnd     >>= 1;
		}
	}

	// set to drawport
	dp_FontData = pfd;
	dp_pixTextCharSpacing = pfd->fd_pixCharSpacing; 
	dp_pixTextLineSpacing = pfd->fd_pixLineSpacing;
	dp_fTextScaling = 1.0f;                         
	dp_fTextAspect  = 1.0f;
	dp_ulTextFlags = NONE;
	dp_fTextShadow = 0;
	dp_ulTextBlendingType = PBT_BLEND;

}

CTString CDrawPort::GetTextWidth(const CTString &strText, INDEX nWidth) const
{
	CTString strReturn;

	// prepare scaling factors
	const SLONG fixTextScalingX = FloatToInt(dp_fTextScaling*dp_fTextAspect*65536.0f);
	const BOOL bFixedWidth = dp_FontData->fd_ulFlags & FNF_FIXED;

	// calculate width of entire text line
	PIX pixStringWidth=0, pixOldWidth=0;
	PIX pixCharStart=0, pixCharEnd=dp_FontData->fd_pixCharWidth;
	INDEX ctCharsPrinted=0;
	for( INDEX i=0; i<(INDEX)strlen(strText); i++)
	{ // get current letter
		unsigned char chrCurrent = strText[i];
		// next line situation?
		if( chrCurrent == '\n') {
			if( pixOldWidth < pixStringWidth) pixOldWidth = pixStringWidth;
			pixStringWidth=0;
			continue;
		}
		// special char encountered and allowed?
		else if( chrCurrent=='^' && !(dp_ulTextFlags&DPTF_PRINTSPECIALCODES)) {
			// get next char
			chrCurrent = strText[++i];
			switch( chrCurrent) {
			// skip corresponding number of characters
			case 'c':  i += FindByte( 0, (UBYTE*)&strText[i], 6);  continue;
			case 'a':  i += FindByte( 0, (UBYTE*)&strText[i], 2);  continue;
			case 'f':  i += 1;  continue;
			case 'b':  case 'i':  case 'r':  case 'o':
			case 'C':  case 'A':  case 'F':  case 'B':  case 'I':  i+=0;  continue;
			default:   break; // if we get here this means that ^ or an unrecognized special code was specified
			}
		}
		// ignore tab
		else if( chrCurrent == '\t') continue;

		// if character is not defined (could be missing small letters in font)
		if( !dp_FontData->IsCharDefined(chrCurrent)) {
			char chrUpper = _toupper(chrCurrent);
			if( (UBYTE)chrUpper==0xFF) chrUpper=(char)0x9F;
			// if its upper version is defined and font uses small caps
			if( dp_FontData->IsCharDefined(chrUpper) && dp_FontData->fd_bSmallCaps) chrCurrent = chrUpper;
		}

		// add current letter's width to result width
		if( !bFixedWidth) {
			// proportional font case
			pixCharStart = dp_FontData->fd_fcdFontCharData[chrCurrent].fcd_pixStart;
			pixCharEnd   = dp_FontData->fd_fcdFontCharData[chrCurrent].fcd_pixEnd;
		}

		if (pixStringWidth < nWidth)
		{
			pixStringWidth += (((pixCharEnd-pixCharStart)*fixTextScalingX)>>16) +dp_pixTextCharSpacing;
			ctCharsPrinted++;
		}
		else
		{
			CTString tmpText = strText;
			CTString tmpRight;

			tmpText.Split(ctCharsPrinted, strReturn, tmpRight);
			break;
		}
	}

	return strReturn;
}

// returns width of the longest line in text string
ULONG CDrawPort::GetTextWidth(const char* strText)
{
	// prepare scaling factors
	const SLONG fixTextScalingX = FloatToInt(dp_fTextScaling*dp_fTextAspect*65536.0f);
	const BOOL bFixedWidth = dp_FontData->fd_ulFlags & FNF_FIXED;

	// calculate width of entire text line
	PIX pixStringWidth=0, pixOldWidth=0;
	PIX pixCharStart=0, pixCharEnd=dp_FontData->fd_pixCharWidth;
	INDEX ctCharsPrinted=0;

	int nLen = strlen(strText);
	for( INDEX i=0; i < nLen; i++)
	{ // get current letter
		unsigned char chrCurrent = strText[i];
		// next line situation?
		if( chrCurrent == '\n') {
			if( pixOldWidth < pixStringWidth) pixOldWidth = pixStringWidth;
			pixStringWidth=0;
			continue;
		}
		// special char encountered and allowed?
		else if( chrCurrent=='^' && !(dp_ulTextFlags&DPTF_PRINTSPECIALCODES)) {
			// get next char
			chrCurrent = strText[++i];
			switch( chrCurrent) {
			// skip corresponding number of characters
			case 'c':  i += FindByte( 0, (UBYTE*)&strText[i], 6);  continue;
			case 'a':  i += FindByte( 0, (UBYTE*)&strText[i], 2);  continue;
			case 'f':  i += 1;  continue;
			case 'b':  case 'i':  case 'r':  case 'o':
			case 'C':  case 'A':  case 'F':  case 'B':  case 'I':  i+=0;  continue;
			default:   break; // if we get here this means that ^ or an unrecognized special code was specified
			}
		}
		// ignore tab
		else if( chrCurrent == '\t') continue;

		// if character is not defined (could be missing small letters in font)
		if( !dp_FontData->IsCharDefined(chrCurrent)) {
			char chrUpper = _toupper(chrCurrent);
			if( (UBYTE)chrUpper==0xFF) chrUpper=(char)0x9F;
			// if its upper version is defined and font uses small caps
			if( dp_FontData->IsCharDefined(chrUpper) && dp_FontData->fd_bSmallCaps) chrCurrent = chrUpper;
		}

		// add current letter's width to result width
		if( !bFixedWidth) {
			// proportional font case
			pixCharStart = dp_FontData->fd_fcdFontCharData[chrCurrent].fcd_pixStart;
			pixCharEnd   = dp_FontData->fd_fcdFontCharData[chrCurrent].fcd_pixEnd;
		}
		pixStringWidth += (((pixCharEnd-pixCharStart)*fixTextScalingX)>>16) + dp_pixTextCharSpacing;
		ctCharsPrinted++;
	}
	// determine largest width
	if( pixStringWidth < pixOldWidth) pixStringWidth = pixOldWidth;
	return pixStringWidth;
}

ULONG CDrawPort::GetTextWidth2(const CTString &strText)
{
#if defined (G_THAI)
	return FindThaiLen(strText);
#elif defined(G_RUSSIA)
	return GetTextSectionWidth(strText.str_String, strText.Length(), FALSE);
#else
	return strText.Length() * (_pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing());
#endif
}

// writes text string on drawport (left aligned if not forced otherwise)
void CDrawPort::PutText( const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend/*=0xFFFFFFFF*/) const
{
	// check API and adjust position for D3D by half pixel
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);

	// skip drawing if text falls above or below draw port
	if( pixY0>GetHeight() || pixX0>GetWidth()) return;
	// check if string even exists
	if( strText.Length()<=0) return;
	char acTmp[7]; // needed for strtoul()
	char *pcDummy; 
//	int code;
	INDEX iRet;

	// cache char and texture dimensions
	const FLOAT fTextScalingX   = dp_fTextScaling*dp_fTextAspect;
	const SLONG fixTextScalingX = FloatToInt(fTextScalingX  *65536.0f);
	const SLONG fixTextScalingY = FloatToInt(dp_fTextScaling*65536.0f);
	const PIX pixCharHeight     = dp_FontData->fd_pixCharHeight-1;
	const PIX pixScaledHeight   = (pixCharHeight*fixTextScalingY)>>16;
	const PIX pixShadowSpacing  = dp_FontData->fd_pixShadowSpacing;
	const FLOAT fItalicAdjustX  = fTextScalingX * (pixScaledHeight)*0.2f;  // 20% slanted
	const FLOAT fBoldAdjustX    = fTextScalingX * (dp_FontData->fd_pixCharWidth)*0.2f;  // 20% fat (extra light mayonnaise:)
	const BOOL bFixedWidth = dp_FontData->fd_ulFlags & FNF_FIXED;
	const BOOL bCellBased  = dp_FontData->fd_ulFlags & FNF_CELLBASED;
	const FLOAT fDropShadowX = dp_fTextShadow * dp_fTextScaling*dp_fTextAspect;
	const FLOAT fDropShadowY = dp_fTextShadow * dp_fTextScaling;

	// prepare font texture
	gfxSetTextureWrapping( GFX_REPEAT, GFX_REPEAT);
	CTextureData &td = *dp_FontData->fd_ptdTextureData;
	ASSERT( td.td_ctFineMipLevels<2);  // font texture cannot have mipmaps!
	td.SetAsCurrent();

	// setup rendering mode
	const enum PredefinedBlendType pbt = (const enum PredefinedBlendType)dp_ulTextBlendingType;
	gfxDisableDepthTest();
	gfxSetBlendType(pbt);

	// calculate and apply correction factor
	FLOAT fCorrectionU = 1.0f / td.GetPixWidth();
	FLOAT fCorrectionV = 1.0f / td.GetPixHeight();
	INDEX ctMaxChars = (INDEX)strlen(strText);
	// determine text color
	GFXColor glcolDefault(colBlend);
	GFXColor glcol = glcolDefault;
	ULONG ulAlphaDefault = (colBlend&CT_AMASK)>>CT_ASHIFT;;  // for flasher

	// prepare some text control and output vars
	FLOAT fBold   = (dp_ulTextFlags&DPTF_BOLD)   ? fBoldAdjustX   : 0;
	FLOAT fItalic = (dp_ulTextFlags&DPTF_ITALIC) ? fItalicAdjustX : 0;
	INDEX iFlash  = 0;
	ULONG ulAlpha = ulAlphaDefault;
	TIME  tmFrame = _pGfx->gl_tvFrameTime.GetSeconds();
	BOOL  bParse  = !(dp_ulTextFlags&(DPTF_PRINTSPECIALCODES|DPTF_IGNORESPECIALCODES));

	// prepare arrays
	gfxResetArrays();
	GFXVertex   *pvtx = _avtxCommon.Push( 2*ctMaxChars*4);  // 2* because of bold
	GFXTexCoord *ptex = _atexCommon[0].Push( 2*ctMaxChars*4);
	GFXColor    *pcol = _acolCommon.Push( 2*ctMaxChars*4);

	// cell/advancer for fixed width and old font (default)
	PIX pixAdvancer  = ((dp_FontData->fd_pixCharWidth *fixTextScalingX)>>16) + dp_pixTextCharSpacing - pixShadowSpacing;
	PIX pixCellWidth = dp_FontData->fd_pixCharWidth;

	// loop thru chars
	PIX pixStartX = pixX0;
	INDEX ctCharsPrinted=0, ctShadowsPrinted=0;
	for( INDEX iChar=0; iChar<ctMaxChars; iChar++)
	{
		// get current char
		unsigned char chrCurrent = strText[iChar];

		// if at end of current line
		if( chrCurrent=='\n') {
			// advance to next line
			pixX0  = pixStartX;
			pixY0 += pixScaledHeight + dp_pixTextLineSpacing - pixShadowSpacing;
			if( pixY0>GetHeight()) break;
			// skip to next char
			continue;
		}
		// special char encountered and allowed?
		else if( chrCurrent=='^' && !(dp_ulTextFlags&DPTF_PRINTSPECIALCODES)) {
			// get next char
			chrCurrent = strText[++iChar];
			COLOR col;
			switch( chrCurrent)
			{
			// color change?
			case 'c':
				strncpy( acTmp, &strText[iChar+1], 6);
				iRet = FindByte( 0, (UBYTE*)&strText[iChar+1], 6);
				iChar+=iRet;
				if( !bParse || iRet<6) continue;
				acTmp[6] = '\0'; // terminate string
				col = strtoul( acTmp, &pcDummy, 16) <<8;
				glcol.Set( col|glcol.a); // do color change but keep original alpha
				continue;
			// alpha change?
			case 'a':
				strncpy( acTmp, &strText[iChar+1], 2);
				iRet = FindByte( 0, (UBYTE*)&strText[iChar+1], 2);
				iChar+=iRet;
				if( !bParse || iRet<2) continue;
				acTmp[2] = '\0'; // terminate string
				ulAlpha = strtoul( acTmp, &pcDummy, 16);
				continue;
			// flash?
			case 'f':
				chrCurrent = strText[++iChar];
				if( bParse) iFlash = 1+ 2* Clamp( (INDEX)(chrCurrent-'0'), 0L, 9L);
				continue;
			// reset all?
			case 'r':
				fBold   = 0;
				fItalic = 0;
				iFlash  = 0;
				glcol   = glcolDefault;
				ulAlpha = ulAlphaDefault;
				continue;
			// simple codes ...
			case 'o':  bParse = bParse && gfx_bDecoratedText;  continue;  // allow console override settings?
			case 'b':  if( bParse) fBold   = fBoldAdjustX;     continue;  // bold?
			case 'i':  if( bParse) fItalic = fItalicAdjustX;   continue;  // italic?
			case 'C':  glcol   = glcolDefault;    continue;  // color reset?
			case 'A':  ulAlpha = ulAlphaDefault;  continue;  // alpha reset?
			case 'B':  fBold   = 0;  continue;  // no bold?
			case 'I':  fItalic = 0;  continue;  // italic?
			case 'F':  iFlash  = 0;  continue;  // no flash?
			default:   break;
			} // unrecognized special code or just plain ^
			if( chrCurrent!='^') { iChar--; break; }
		}
		// ignore tab
		else if( chrCurrent=='\t') continue;

		// if character is not defined (could be missing small letters in font)
		if( !dp_FontData->IsCharDefined(chrCurrent)) {
			char chrUpper = _toupper(chrCurrent);
			if( (UBYTE)chrUpper==0xFF) chrUpper = (char)0x9F;
			// if its upper version is defined and font uses small caps
			if( dp_FontData->IsCharDefined(chrUpper) && dp_FontData->fd_bSmallCaps) chrCurrent = chrUpper;
		}

		/*code = chrCurrent; // 한글
		if (code & 0x80)
			code = (code<<8) | (unsigned char)strText[++iChar];

		// fetch char props
		const CFontCharData &fcdCurrent = dp_FontData->fd_fcdFontCharData[dp_FontData->SearchIndex(code)];*/
		const CFontCharData &fcdCurrent = dp_FontData->fd_fcdFontCharData[chrCurrent];
		const PIX pixCharX = fcdCurrent.fcd_pixXOffset;
		const PIX pixCharY = fcdCurrent.fcd_pixYOffset;
		const PIX pixCharStart = fcdCurrent.fcd_pixStart;
		const PIX pixCharEnd   = fcdCurrent.fcd_pixEnd;
		if( !bCellBased) pixCellWidth = pixCharEnd;
		const PIX pixScaledWidth = (pixCellWidth*fixTextScalingX)>>16;

		// determine corresponding char width and position adjustments
		PIX pixXA; // adjusted starting X location of printout
		if( bFixedWidth) {
			// for fixed font
			pixXA = pixX0 - ((pixCharStart*fixTextScalingX)>>16)
						+ (((pixScaledWidth<<16) - ((pixCharEnd-pixCharStart)*fixTextScalingX) +0x10000) >>17);
		} else {
			// for proportional font
			pixXA = pixX0 - ((pixCharStart*fixTextScalingX)>>16);
			pixAdvancer = (((pixCharEnd-pixCharStart)*fixTextScalingX)>>16) +dp_pixTextCharSpacing - pixShadowSpacing;
		}
		// out of screen (left) ?
		if( pixXA>GetWidth() || (pixXA+pixCharEnd)<0) {
			// skip to next char
			pixX0 += pixAdvancer;
			continue; 
		}

		// adjust alpha for flashing
		if( iFlash>0) glcol.a = ulAlpha*(sin(iFlash*tmFrame)*0.5f+0.5f);
		else glcol.a = ulAlpha; 

		// prepare coordinates for screen and texture
		const FLOAT fX0 = pixXA;  const FLOAT fX1 = fX0 +pixScaledWidth;
		const FLOAT fY0 = pixY0;  const FLOAT fY1 = fY0 +pixScaledHeight;
		const FLOAT fU0 = pixCharX *fCorrectionU;  const FLOAT fU1 = (pixCharX+pixCellWidth)  *fCorrectionU;
		const FLOAT fV0 = pixCharY *fCorrectionV;  const FLOAT fV1 = (pixCharY+pixCharHeight) *fCorrectionV;

		// add drop shadow
		if( fDropShadowX) {
			GFXColor glShdCol = 0;
			glShdCol.a = glcol.a;
			pvtx[0].x = fX0 +fDropShadowX+fItalic;        pvtx[0].y = fY0 +fDropShadowY;  pvtx[0].z = 0;
			pvtx[1].x = fX0 +fDropShadowX;                pvtx[1].y = fY1 +fDropShadowY;  pvtx[1].z = 0;
			pvtx[2].x = fX1 +fDropShadowX+fBold;          pvtx[2].y = fY1 +fDropShadowY;  pvtx[2].z = 0;
			pvtx[3].x = fX1 +fDropShadowX+fBold+fItalic;  pvtx[3].y = fY0 +fDropShadowY;  pvtx[3].z = 0;
			ptex[0].s = fU0;  ptex[0].t = fV0;
			ptex[1].s = fU0;  ptex[1].t = fV1;
			ptex[2].s = fU1;  ptex[2].t = fV1;
			ptex[3].s = fU1;  ptex[3].t = fV0;
			pcol[0] = glShdCol;
			pcol[1] = glShdCol;
			pcol[2] = glShdCol;
			pcol[3] = glShdCol;
			// advance to next vetrices group
			pvtx += 4;
			ptex += 4;
			pcol += 4;
			ctShadowsPrinted++;
		}

		// add char
		pvtx[0].x = fX0+fItalic;        pvtx[0].y = fY0;  pvtx[0].z = 0;
		pvtx[1].x = fX0;                pvtx[1].y = fY1;  pvtx[1].z = 0;
		pvtx[2].x = fX1+fBold;          pvtx[2].y = fY1;  pvtx[2].z = 0;
		pvtx[3].x = fX1+fBold+fItalic;  pvtx[3].y = fY0;  pvtx[3].z = 0;
		ptex[0].s = fU0;  ptex[0].t = fV0;
		ptex[1].s = fU0;  ptex[1].t = fV1;
		ptex[2].s = fU1;  ptex[2].t = fV1;
		ptex[3].s = fU1;  ptex[3].t = fV0;
		pcol[0] = glcol;
		pcol[1] = glcol;
		pcol[2] = glcol;
		pcol[3] = glcol;
		// advance to next vetrices group
		pvtx += 4;
		ptex += 4;
		pcol += 4;

		// advance to next char
		pixX0 += pixAdvancer;
		ctCharsPrinted++;
	}

	// adjust vertex arrays size according to chars that really got printed out
	ctCharsPrinted += ctShadowsPrinted;
	_avtxCommon.PopUntil( ctCharsPrinted*4-1);
	_atexCommon[0].PopUntil( ctCharsPrinted*4-1);
	_acolCommon.PopUntil( ctCharsPrinted*4-1);
	gfxFlushQuads();
}



// writes text string on drawport (centered arround X)
void CDrawPort::PutTextC( const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend/*=0xFFFFFFFF*/)
{
	PutText( strText, pixX0-GetTextWidth(strText)/2, pixY0, colBlend);
}

// writes text string on drawport (centered arround X and Y)
void CDrawPort::PutTextCXY( const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend/*=0xFFFFFFFF*/)
{
	PIX pixTextWidth  = GetTextWidth(strText);
	PIX pixTextHeight = dp_FontData->fd_pixCharHeight * dp_fTextScaling;
	PutText( strText, pixX0-pixTextWidth/2, pixY0-pixTextHeight/2, colBlend);
}

// writes text string on drawport (right-aligned)
void CDrawPort::PutTextR( const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend/*=0xFFFFFFFF*/)
{
	PutText( strText, pixX0-GetTextWidth(strText), pixY0, colBlend);
}

// yjpark |<--
void CDrawPort::PutTextEx( const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend,
							FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const
{
	switch( _pUIFontTexMgr->GetLanguage() )
	{
	case FONT_KOREAN:
		PutTextExKor( strText, pixX0, pixY0, colBlend, fZ, bShadow, colShadow );
		break;

	case FONT_CHINESE_T:
		PutTextExCht( strText, pixX0, pixY0, colBlend, fZ, bShadow, colShadow );
		break;

	case FONT_CHINESE_S:
		PutTextExChs( strText, pixX0, pixY0, colBlend, fZ, bShadow, colShadow );
		break;

	case FONT_JAPANESE: // wooss 051021
		PutTextExJap( strText, pixX0, pixY0, colBlend, fZ, bShadow, colShadow );
		break;

	case FONT_THAILAND:
		PutTextExThai( strText, pixX0, pixY0, colBlend, fZ, bShadow, colShadow );
		break;

	case FONT_PROTUGES:
	case FONT_GERMAN:
	case FONT_SPAIN://FRANCE_SPAIN_CLOSEBETA_NA_20081124
	case FONT_FRANCE:
	case FONT_POLAND:
	case FONT_TURKEY:
	case FONT_MEXICO:
	case FONT_ITALY:
	case FONT_USA_FRANCE:
	case FONT_USA_SPAIN:
	case FONT_NETHERLANDS:
		PutTextExBrz( strText, pixX0, pixY0, colBlend, fZ, bShadow, colShadow );
		break;
		
	case FONT_RUSSIAN:
		PutTextExRus( strText, pixX0, pixY0, colBlend, fZ, bShadow, colShadow );
		break;
	}
}

void CDrawPort::PutTextCharEx( const char *pText, int nLength, PIX pixX0, PIX pixY0, const COLOR colBlend,
								FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const
{
	switch( _pUIFontTexMgr->GetLanguage() )
	{
	case FONT_KOREAN:
		PutTextCharExKor( pText, nLength, pixX0, pixY0, colBlend, fZ, bShadow, colShadow );
		break;

	case FONT_CHINESE_T:
		PutTextCharExCht( pText, nLength, pixX0, pixY0, colBlend, fZ, bShadow, colShadow );
		break;

	case FONT_CHINESE_S:
		PutTextCharExChs( pText, nLength, pixX0, pixY0, colBlend, fZ, bShadow, colShadow );
		break;

	case FONT_JAPANESE:
		PutTextCharExJap( pText, nLength, pixX0, pixY0, colBlend, fZ, bShadow, colShadow );
		break;

	case FONT_THAILAND:
		PutTextCharExThai( pText, nLength, pixX0, pixY0, colBlend, fZ, bShadow, colShadow );
		break;
	case FONT_PROTUGES:
	case FONT_GERMAN:
	case FONT_SPAIN://FRANCE_SPAIN_CLOSEBETA_NA_20081124
	case FONT_FRANCE:
	case FONT_POLAND:
	case FONT_TURKEY:
	case FONT_MEXICO:
	case FONT_ITALY:
	case FONT_USA_FRANCE:
	case FONT_USA_SPAIN:
	case FONT_NETHERLANDS:
		PutTextCharExBrz( pText, nLength, pixX0, pixY0, colBlend, fZ, bShadow, colShadow );
		break;
		
	case FONT_RUSSIAN:
		PutTextCharExRus( pText, nLength, pixX0, pixY0, colBlend, fZ, bShadow, colShadow );
		break;
	}
}

void CDrawPort::PutTextExKor( const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend,
								FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const
{
	// Skip drawing if text falls above or below draw port
	if( pixY0 > GetHeight() || pixX0 > GetWidth() )
		return;

	// Check if string even exists
	INDEX	ctMaxChars = strText.Length();
	if( ctMaxChars <= 0 )
		return;

	GFXColor	glcol( colBlend );
	GFXColor	glShadow( colShadow );
	const int	nTextureCount = _pUIFontTexMgr->GetFontTextureCount();
	const PIX	pixFontWidth = _pUIFontTexMgr->GetFontWidth();
	const PIX	pixFontWidth2Byte = _pUIFontTexMgr->GetFontWidth2Byte();
	const PIX	pixFontUVOffset = _pUIFontTexMgr->GetFontWidth2Byte();
	const PIX	pixFontHeight = _pUIFontTexMgr->GetFontHeight();
	const PIX	pixFontAdvancer = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	const PIX	pixFontAdvancer2Byte = pixFontWidth2Byte + _pUIFontTexMgr->GetFontSpacing();

	int			nSequentialCode, nTextureID, nU, nV;
	FLOAT		fU0, fV0, fU1, fV1, fInvU, fInvV;
	PIX			pixX1, pixY1;
	PIX			pixStartX = pixX0;

	_pUIFontTexMgr->GetInvUV( fInvU, fInvV );

	// Loop thru chars
	for( INDEX iChar = 0; iChar < ctMaxChars; iChar++ )
	{
		// Get current char
		char	cCurrent = strText[iChar];

		// If at end of current line
		if( cCurrent == '\n' || cCurrent == '\r' )
		{
			continue;
		}
		else if( cCurrent == ' ' )
		{
			// Advance to next char
			pixX0 += pixFontAdvancer;
			continue;
		}

		if( cCurrent & 0x80 )
		{
			nSequentialCode = ( (unsigned char)cCurrent - 0x81 ) * 0xBE +
								( (unsigned char)strText[iChar + 1] - 0x41 ) + 98;
			nTextureID = nSequentialCode / 1764;

			if( nTextureID >= nTextureCount )
				break;

			// Texture coordinate
			nU = ( nSequentialCode % 42 ) * pixFontUVOffset;
			nV = ( ( nSequentialCode % 1764 ) / 42 ) * pixFontUVOffset;
			fU0 = nU * fInvU;
			fV0 = nV * fInvV;
			fU1 = ( nU + pixFontWidth2Byte ) * fInvU;
			fV1 = ( nV + pixFontHeight ) * fInvV;

			pixX1 = pixX0 + pixFontWidth2Byte;
			pixY1 = pixY0 + pixFontHeight;

			// Shadow
			if( bShadow )
			{
				// Prepare arrays
				INDEX		iStart = _avtxText[nTextureID].Count();
				GFXVertex	*pvtx = _avtxText[nTextureID].Push( 4 );
				GFXTexCoord	*ptex = _atexText[nTextureID].Push( 4 );
				GFXColor	*pcol = _acolText[nTextureID].Push( 4 );
				UWORD		*pelm = _auwText[nTextureID].Push( 6 );

				// Add char
				pvtx[0].x = pvtx[1].x = pixX0 + 1;
				pvtx[2].x = pvtx[3].x = pixX1 + 1;
				pvtx[0].y = pvtx[3].y = pixY0 + 1;
				pvtx[1].y = pvtx[2].y = pixY1 + 1;
				pvtx[0].z = pvtx[1].z = pvtx[2].z = pvtx[3].z = fZ;
				ptex[0].s = fU0;	ptex[0].t = fV0;
				ptex[1].s = fU0;	ptex[1].t = fV1;
				ptex[2].s = fU1;	ptex[2].t = fV1;
				ptex[3].s = fU1;	ptex[3].t = fV0;
				pcol[0] = glShadow;	pcol[1] = glShadow;	pcol[2] = glShadow;	pcol[3] = glShadow;
				(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
				(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
				(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );
			}

			// Prepare arrays
			INDEX		iStart = _avtxText[nTextureID].Count();
			GFXVertex	*pvtx = _avtxText[nTextureID].Push( 4 );
			GFXTexCoord	*ptex = _atexText[nTextureID].Push( 4 );
			GFXColor	*pcol = _acolText[nTextureID].Push( 4 );
			UWORD		*pelm = _auwText[nTextureID].Push( 6 );

			// Add char
			pvtx[0].x = pixX0;	pvtx[0].y = pixY0;	pvtx[0].z = fZ;
			pvtx[1].x = pixX0;	pvtx[1].y = pixY1;	pvtx[1].z = fZ;
			pvtx[2].x = pixX1;	pvtx[2].y = pixY1;	pvtx[2].z = fZ;
			pvtx[3].x = pixX1;	pvtx[3].y = pixY0;	pvtx[3].z = fZ;
			ptex[0].s = fU0;	ptex[0].t = fV0;
			ptex[1].s = fU0;	ptex[1].t = fV1;
			ptex[2].s = fU1;	ptex[2].t = fV1;
			ptex[3].s = fU1;	ptex[3].t = fV0;
			pcol[0] = glcol;	pcol[1] = glcol;	pcol[2] = glcol;	pcol[3] = glcol;
			(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
			(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
			(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );

			// Advance to next char
			pixX0 += pixFontAdvancer2Byte;
			iChar++;
		}
		else
		{
			// Texture coordinate
			nU = ( ( cCurrent - 0x1e ) % 42 ) * pixFontUVOffset;
			nV = ( ( cCurrent - 0x1e ) / 42 ) * pixFontUVOffset;
			fU0 = nU * fInvU;
			fV0 = nV * fInvV;
			//fU1 = ( nU + pixFontWidth+1 ) * fInvU;
			fU1 = ( nU + pixFontWidth ) * fInvU;
			fV1 = ( nV + pixFontHeight ) * fInvV;

			pixX1 = pixX0 + pixFontWidth;
			pixY1 = pixY0 + pixFontHeight;

			// Shadow
			if( bShadow )
			{
				// Prepare arrays
				INDEX		iStart = _avtxText[0].Count();
				GFXVertex	*pvtx = _avtxText[0].Push( 4 );
				GFXTexCoord	*ptex = _atexText[0].Push( 4 );
				GFXColor	*pcol = _acolText[0].Push( 4 );
				UWORD		*pelm = _auwText[0].Push( 6 );

				// Add char
				pvtx[0].x = pvtx[1].x = pixX0 + 1;
				pvtx[2].x = pvtx[3].x = pixX1 + 1;
				pvtx[0].y = pvtx[3].y = pixY0 + 1;
				pvtx[1].y = pvtx[2].y = pixY1 + 1;
				pvtx[0].z = pvtx[1].z = pvtx[2].z = pvtx[3].z = fZ;
				ptex[0].s = fU0;	ptex[0].t = fV0;
				ptex[1].s = fU0;	ptex[1].t = fV1;
				ptex[2].s = fU1;	ptex[2].t = fV1;
				ptex[3].s = fU1;	ptex[3].t = fV0;
				pcol[0] = glShadow;	pcol[1] = glShadow;	pcol[2] = glShadow;	pcol[3] = glShadow;
				(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
				(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
				(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );
			}

			// Prepare arrays
			INDEX		iStart = _avtxText[0].Count();
			GFXVertex	*pvtx = _avtxText[0].Push( 4 );
			GFXTexCoord	*ptex = _atexText[0].Push( 4 );
			GFXColor	*pcol = _acolText[0].Push( 4 );
			UWORD		*pelm = _auwText[0].Push( 6 );

			// Add char
			pvtx[0].x = pixX0;	pvtx[0].y = pixY0;	pvtx[0].z = fZ;
			pvtx[1].x = pixX0;	pvtx[1].y = pixY1;	pvtx[1].z = fZ;
			pvtx[2].x = pixX1;	pvtx[2].y = pixY1;	pvtx[2].z = fZ;
			pvtx[3].x = pixX1;	pvtx[3].y = pixY0;	pvtx[3].z = fZ;
			ptex[0].s = fU0;	ptex[0].t = fV0;
			ptex[1].s = fU0;	ptex[1].t = fV1;
			ptex[2].s = fU1;	ptex[2].t = fV1;
			ptex[3].s = fU1;	ptex[3].t = fV0;
			pcol[0] = glcol;	pcol[1] = glcol;	pcol[2] = glcol;	pcol[3] = glcol;
			(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
			(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
			(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );

			// Advance to next char
			pixX0 += pixFontAdvancer;
		}
	}
}

void CDrawPort::PutTextExCht( const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend,
								FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const
{
	// Skip drawing if text falls above or below draw port
	if( pixY0 > GetHeight() || pixX0 > GetWidth() )
		return;

	// Check if string even exists
	INDEX	ctMaxChars = strText.Length();
	if( ctMaxChars <= 0 )
		return;

	GFXColor	glcol( colBlend );
	GFXColor	glShadow( colShadow );
	const int	nTextureCount = _pUIFontTexMgr->GetFontTextureCount();
	const PIX	pixFontWidth = _pUIFontTexMgr->GetFontWidth();
	const PIX	pixFontWidth2Byte = _pUIFontTexMgr->GetFontWidth2Byte();
	const PIX	pixFontUVOffset = _pUIFontTexMgr->GetFontWidth2Byte();
	const PIX	pixFontHeight = _pUIFontTexMgr->GetFontHeight();
	const PIX	pixFontAdvancer = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	const PIX	pixFontAdvancer2Byte = pixFontWidth2Byte + _pUIFontTexMgr->GetFontSpacing();

	int			nSequentialCode, nTextureID, nU, nV;
	FLOAT		fU0, fV0, fU1, fV1, fInvU, fInvV;
	PIX			pixX1, pixY1;
	PIX			pixStartX = pixX0;

	_pUIFontTexMgr->GetInvUV( fInvU, fInvV );

	// Loop thru chars
	for( INDEX iChar = 0; iChar < ctMaxChars; iChar++ )
	{
		// Get current char
		char	cCurrent = strText[iChar];

		// If at end of current line
		if( cCurrent == '\n' || cCurrent == '\r' )
		{
			continue;
		}
		else if( cCurrent == ' ' )
		{
			// Advance to next char
			pixX0 += pixFontAdvancer;
			continue;
		}

		if( cCurrent & 0x80 )
		{
			if( (unsigned char)cCurrent >= 0xC9 )
			{
				if( (unsigned char)strText[iChar + 1] < 0xA1 )
				{
					nSequentialCode = ( (unsigned char)cCurrent - 0xC9 + 0x26 ) * 0x9D +
								( (unsigned char)strText[iChar + 1] - 0x40 ) + 98;
				}
				else
				{
					nSequentialCode = ( (unsigned char)cCurrent - 0xC9 + 0x26 ) * 0x9D +
								( (unsigned char)strText[iChar + 1] - 0xA1 + 0x3F ) + 98;
				}
			}
			else
			{ 
				if( (unsigned char)strText[iChar + 1] < 0xA1 )
				{
					nSequentialCode = ( (unsigned char)cCurrent - 0xA1 ) * 0x9D +
								( (unsigned char)strText[iChar + 1] - 0x40 ) + 98;
				}
				else
				{
					nSequentialCode = ( (unsigned char)cCurrent - 0xA1 ) * 0x9D +
								( (unsigned char)strText[iChar + 1] - 0xA1 + 0x3F ) + 98;
				}
			}
			nTextureID = nSequentialCode / 1764;

			if( nTextureID >= nTextureCount )
				break;

			// Texture coordinate
			nU = ( nSequentialCode % 42 ) * pixFontUVOffset;
			nV = ( ( nSequentialCode % 1764 ) / 42 ) * pixFontUVOffset;
			fU0 = nU * fInvU;
			fV0 = nV * fInvV;
			fU1 = ( nU + pixFontWidth2Byte ) * fInvU;
			fV1 = ( nV + pixFontHeight ) * fInvV;

			pixX1 = pixX0 + pixFontWidth2Byte;
			pixY1 = pixY0 + pixFontHeight;

			// Shadow
			if( bShadow )
			{
				// Prepare arrays
				INDEX		iStart = _avtxText[nTextureID].Count();
				GFXVertex	*pvtx = _avtxText[nTextureID].Push( 4 );
				GFXTexCoord	*ptex = _atexText[nTextureID].Push( 4 );
				GFXColor	*pcol = _acolText[nTextureID].Push( 4 );
				UWORD		*pelm = _auwText[nTextureID].Push( 6 );

				// Add char
				pvtx[0].x = pvtx[1].x = pixX0 + 1;
				pvtx[2].x = pvtx[3].x = pixX1 + 1;
				pvtx[0].y = pvtx[3].y = pixY0 + 1;
				pvtx[1].y = pvtx[2].y = pixY1 + 1;
				pvtx[0].z = pvtx[1].z = pvtx[2].z = pvtx[3].z = fZ;
				ptex[0].s = fU0;	ptex[0].t = fV0;
				ptex[1].s = fU0;	ptex[1].t = fV1;
				ptex[2].s = fU1;	ptex[2].t = fV1;
				ptex[3].s = fU1;	ptex[3].t = fV0;
				pcol[0] = glShadow;	pcol[1] = glShadow;	pcol[2] = glShadow;	pcol[3] = glShadow;
				(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
				(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
				(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );
			}

			// Prepare arrays
			INDEX		iStart = _avtxText[nTextureID].Count();
			GFXVertex	*pvtx = _avtxText[nTextureID].Push( 4 );
			GFXTexCoord	*ptex = _atexText[nTextureID].Push( 4 );
			GFXColor	*pcol = _acolText[nTextureID].Push( 4 );
			UWORD		*pelm = _auwText[nTextureID].Push( 6 );

			// Add char
			pvtx[0].x = pixX0;	pvtx[0].y = pixY0;	pvtx[0].z = fZ;
			pvtx[1].x = pixX0;	pvtx[1].y = pixY1;	pvtx[1].z = fZ;
			pvtx[2].x = pixX1;	pvtx[2].y = pixY1;	pvtx[2].z = fZ;
			pvtx[3].x = pixX1;	pvtx[3].y = pixY0;	pvtx[3].z = fZ;
			ptex[0].s = fU0;	ptex[0].t = fV0;
			ptex[1].s = fU0;	ptex[1].t = fV1;
			ptex[2].s = fU1;	ptex[2].t = fV1;
			ptex[3].s = fU1;	ptex[3].t = fV0;
			pcol[0] = glcol;	pcol[1] = glcol;	pcol[2] = glcol;	pcol[3] = glcol;
			(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
			(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
			(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );

			// Advance to next char
			pixX0 += pixFontAdvancer2Byte;
			iChar++;
		}
		else
		{
			// Texture coordinate
			nU = ( ( cCurrent - 0x1e ) % 42 ) * pixFontUVOffset;
			nV = ( ( cCurrent - 0x1e ) / 42 ) * pixFontUVOffset;
			fU0 = nU * fInvU;
			fV0 = nV * fInvV;
			fU1 = ( nU + pixFontWidth ) * fInvU;
			fV1 = ( nV + pixFontHeight ) * fInvV;

			pixX1 = pixX0 + pixFontWidth;
			pixY1 = pixY0 + pixFontHeight;

			// Shadow
			if( bShadow )
			{
				// Prepare arrays
				INDEX		iStart = _avtxText[0].Count();
				GFXVertex	*pvtx = _avtxText[0].Push( 4 );
				GFXTexCoord	*ptex = _atexText[0].Push( 4 );
				GFXColor	*pcol = _acolText[0].Push( 4 );
				UWORD		*pelm = _auwText[0].Push( 6 );

				// Add char
				pvtx[0].x = pvtx[1].x = pixX0 + 1;
				pvtx[2].x = pvtx[3].x = pixX1 + 1;
				pvtx[0].y = pvtx[3].y = pixY0 + 1;
				pvtx[1].y = pvtx[2].y = pixY1 + 1;
				pvtx[0].z = pvtx[1].z = pvtx[2].z = pvtx[3].z = fZ;
				ptex[0].s = fU0;	ptex[0].t = fV0;
				ptex[1].s = fU0;	ptex[1].t = fV1;
				ptex[2].s = fU1;	ptex[2].t = fV1;
				ptex[3].s = fU1;	ptex[3].t = fV0;
				pcol[0] = glShadow;	pcol[1] = glShadow;	pcol[2] = glShadow;	pcol[3] = glShadow;
				(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
				(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
				(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );
			}

			// Prepare arrays
			INDEX		iStart = _avtxText[0].Count();
			GFXVertex	*pvtx = _avtxText[0].Push( 4 );
			GFXTexCoord	*ptex = _atexText[0].Push( 4 );
			GFXColor	*pcol = _acolText[0].Push( 4 );
			UWORD		*pelm = _auwText[0].Push( 6 );

			// Add char
			pvtx[0].x = pixX0;	pvtx[0].y = pixY0;	pvtx[0].z = fZ;
			pvtx[1].x = pixX0;	pvtx[1].y = pixY1;	pvtx[1].z = fZ;
			pvtx[2].x = pixX1;	pvtx[2].y = pixY1;	pvtx[2].z = fZ;
			pvtx[3].x = pixX1;	pvtx[3].y = pixY0;	pvtx[3].z = fZ;
			ptex[0].s = fU0;	ptex[0].t = fV0;
			ptex[1].s = fU0;	ptex[1].t = fV1;
			ptex[2].s = fU1;	ptex[2].t = fV1;
			ptex[3].s = fU1;	ptex[3].t = fV0;
			pcol[0] = glcol;	pcol[1] = glcol;	pcol[2] = glcol;	pcol[3] = glcol;
			(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
			(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
			(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );

			// Advance to next char
			pixX0 += pixFontAdvancer;
		}
	}
}

//------------------------------------------------------------------------------
// CDrawPort::PutTextCharExChs
// Explain:  
// Date : 2005-03-03(오후 1:51:55) Lee Ki-hwan
//------------------------------------------------------------------------------
void CDrawPort::PutTextCharExChs( const char *pText, int nLength, PIX pixX0, PIX pixY0, const COLOR colBlend,
									FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const
{
	// Skip drawing if text falls above or below draw port
	if( pixY0 > GetHeight() || pixX0 > GetWidth() )
		return;

	// Check if string even exists
	INDEX	ctMaxChars;
	if( nLength == 0 ) ctMaxChars = strlen( pText );
	else ctMaxChars = nLength;
	if( ctMaxChars <= 0 )
		return;

	GFXColor	glcol( colBlend );
	GFXColor	glShadow( colShadow );
	const int	nTextureCount = _pUIFontTexMgr->GetFontTextureCount();
	const PIX	pixFontWidth = _pUIFontTexMgr->GetFontWidth();
	const PIX	pixFontWidth2Byte = _pUIFontTexMgr->GetFontWidth2Byte();
	const PIX	pixFontUVOffset = _pUIFontTexMgr->GetFontWidth2Byte();
	const PIX	pixFontHeight = _pUIFontTexMgr->GetFontHeight();
	const PIX	pixFontAdvancer = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	const PIX	pixFontAdvancer2Byte = pixFontWidth2Byte + _pUIFontTexMgr->GetFontSpacing();

	int			nSequentialCode, nTextureID, nU, nV;
	FLOAT		fU0, fV0, fU1, fV1, fInvU, fInvV;
	PIX			pixX1, pixY1;
	PIX			pixStartX = pixX0;

	_pUIFontTexMgr->GetInvUV( fInvU, fInvV );

	// Loop thru chars
	for( INDEX iChar = 0; iChar < ctMaxChars; iChar++ )
	{
		// Get current char
		char	cCurrent = pText[iChar];

		// If at end of current line
		if( cCurrent == '\n' || cCurrent == '\r' )
		{
			continue;
		}
		else if( cCurrent == ' ' )
		{
			// Advance to next char
			pixX0 += pixFontAdvancer;
			continue;
		}

		if( (unsigned char)cCurrent >= GBK_HI_START) //  Hi start 0xa1
		{ 
			if( (unsigned char)cCurrent <= GBK_HI_END) // Hi End
			{
				if( (unsigned char)pText[iChar + 1] >= GBK_LOW_START ) // Low start
				{
					nSequentialCode = ( (unsigned char)cCurrent - GBK_HI_START ) * GBK_LOW_BYTE_COUNT		// Hi
									+ ( (unsigned char)pText[iChar + 1] - GBK_LOW_START )	// Low
									+ ASCII_CHARACTER_COUNT;
				}
				else return;
			}
			else return;
			
			nTextureID = nSequentialCode / GBK_PAGE_PER_CHARACTER;

			if( nTextureID >= nTextureCount )
				break;

			// Texture coordinate
			nU = ( nSequentialCode % GBK_COUNT ) * pixFontUVOffset;
			nV = ( ( nSequentialCode % GBK_PAGE_PER_CHARACTER ) / GBK_COUNT ) * pixFontUVOffset;
			fU0 = nU * fInvU;
			fV0 = nV * fInvV;
			fU1 = ( nU + pixFontWidth2Byte ) * fInvU;
			fV1 = ( nV + pixFontHeight ) * fInvV;

			pixX1 = pixX0 + pixFontWidth2Byte;
			pixY1 = pixY0 + pixFontHeight;

			// Shadow
			if( bShadow )
			{
				// Prepare arrays
				INDEX		iStart = _avtxText[nTextureID].Count();
				GFXVertex	*pvtx = _avtxText[nTextureID].Push( 4 );
				GFXTexCoord	*ptex = _atexText[nTextureID].Push( 4 );
				GFXColor	*pcol = _acolText[nTextureID].Push( 4 );
				UWORD		*pelm = _auwText[nTextureID].Push( 6 );

				// Add char
				pvtx[0].x = pvtx[1].x = pixX0 + 1;
				pvtx[2].x = pvtx[3].x = pixX1 + 1;
				pvtx[0].y = pvtx[3].y = pixY0 + 1;
				pvtx[1].y = pvtx[2].y = pixY1 + 1;
				pvtx[0].z = pvtx[1].z = pvtx[2].z = pvtx[3].z = fZ;
				ptex[0].s = fU0;	ptex[0].t = fV0;
				ptex[1].s = fU0;	ptex[1].t = fV1;
				ptex[2].s = fU1;	ptex[2].t = fV1;
				ptex[3].s = fU1;	ptex[3].t = fV0;
				pcol[0] = glShadow;	pcol[1] = glShadow;	pcol[2] = glShadow;	pcol[3] = glShadow;
				(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
				(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
				(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );
			}

			// Prepare arrays
			INDEX		iStart = _avtxText[nTextureID].Count();
			GFXVertex	*pvtx = _avtxText[nTextureID].Push( 4 );
			GFXTexCoord	*ptex = _atexText[nTextureID].Push( 4 );
			GFXColor	*pcol = _acolText[nTextureID].Push( 4 );
			UWORD		*pelm = _auwText[nTextureID].Push( 6 );

			// Add char
			pvtx[0].x = pixX0;	pvtx[0].y = pixY0;	pvtx[0].z = fZ;
			pvtx[1].x = pixX0;	pvtx[1].y = pixY1;	pvtx[1].z = fZ;
			pvtx[2].x = pixX1;	pvtx[2].y = pixY1;	pvtx[2].z = fZ;
			pvtx[3].x = pixX1;	pvtx[3].y = pixY0;	pvtx[3].z = fZ;
			ptex[0].s = fU0;	ptex[0].t = fV0;
			ptex[1].s = fU0;	ptex[1].t = fV1;
			ptex[2].s = fU1;	ptex[2].t = fV1;
			ptex[3].s = fU1;	ptex[3].t = fV0;
			pcol[0] = glcol;	pcol[1] = glcol;	pcol[2] = glcol;	pcol[3] = glcol;
			(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
			(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
			(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );

			// Advance to next char
			pixX0 += pixFontAdvancer2Byte;
			iChar++;
		}
		else
		{
			// Texture coordinate
			nU = ( ( cCurrent - 0x1e ) % GBK_COUNT ) * pixFontUVOffset;
			nV = ( ( cCurrent - 0x1e ) / GBK_COUNT ) * pixFontUVOffset;
			fU0 = nU * fInvU;
			fV0 = nV * fInvV;
			fU1 = ( nU + pixFontWidth ) * fInvU;
			fV1 = ( nV + pixFontHeight ) * fInvV;

			pixX1 = pixX0 + pixFontWidth;
			pixY1 = pixY0 + pixFontHeight;

			// Shadow
			if( bShadow )
			{
				// Prepare arrays
				INDEX		iStart = _avtxText[0].Count();
				GFXVertex	*pvtx = _avtxText[0].Push( 4 );
				GFXTexCoord	*ptex = _atexText[0].Push( 4 );
				GFXColor	*pcol = _acolText[0].Push( 4 );
				UWORD		*pelm = _auwText[0].Push( 6 );

				// Add char
				pvtx[0].x = pvtx[1].x = pixX0 + 1;
				pvtx[2].x = pvtx[3].x = pixX1 + 1;
				pvtx[0].y = pvtx[3].y = pixY0 + 1;
				pvtx[1].y = pvtx[2].y = pixY1 + 1;
				pvtx[0].z = pvtx[1].z = pvtx[2].z = pvtx[3].z = fZ;
				ptex[0].s = fU0;	ptex[0].t = fV0;
				ptex[1].s = fU0;	ptex[1].t = fV1;
				ptex[2].s = fU1;	ptex[2].t = fV1;
				ptex[3].s = fU1;	ptex[3].t = fV0;
				pcol[0] = glShadow;	pcol[1] = glShadow;	pcol[2] = glShadow;	pcol[3] = glShadow;
				(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
				(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
				(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );
			}

			// Prepare arrays
			INDEX		iStart = _avtxText[0].Count();
			GFXVertex	*pvtx = _avtxText[0].Push( 4 );
			GFXTexCoord	*ptex = _atexText[0].Push( 4 );
			GFXColor	*pcol = _acolText[0].Push( 4 );
			UWORD		*pelm = _auwText[0].Push( 6 );

			// Add char
			pvtx[0].x = pixX0;	pvtx[0].y = pixY0;	pvtx[0].z = fZ;
			pvtx[1].x = pixX0;	pvtx[1].y = pixY1;	pvtx[1].z = fZ;
			pvtx[2].x = pixX1;	pvtx[2].y = pixY1;	pvtx[2].z = fZ;
			pvtx[3].x = pixX1;	pvtx[3].y = pixY0;	pvtx[3].z = fZ;
			ptex[0].s = fU0;	ptex[0].t = fV0;
			ptex[1].s = fU0;	ptex[1].t = fV1;
			ptex[2].s = fU1;	ptex[2].t = fV1;
			ptex[3].s = fU1;	ptex[3].t = fV0;
			pcol[0] = glcol;	pcol[1] = glcol;	pcol[2] = glcol;	pcol[3] = glcol;
			(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
			(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
			(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );

			// Advance to next char
			pixX0 += pixFontAdvancer;
		}
	}
}


//------------------------------------------------------------------------------
// CDrawPort::PutTextExChs
// Explain:  
// Date : 2005-03-03(오후 1:49:37) Lee Ki-hwan
//------------------------------------------------------------------------------
void CDrawPort::PutTextExChs( const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend,
								FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const
{
	// Skip drawing if text falls above or below draw port
	if( pixY0 > GetHeight() || pixX0 > GetWidth() )
		return;

	// Check if string even exists
	INDEX	ctMaxChars = strText.Length();
	if( ctMaxChars <= 0 )
		return;

	GFXColor	glcol( colBlend );
	GFXColor	glShadow( colShadow );
	const int	nTextureCount = _pUIFontTexMgr->GetFontTextureCount();
	const PIX	pixFontWidth = _pUIFontTexMgr->GetFontWidth();
	const PIX	pixFontWidth2Byte = _pUIFontTexMgr->GetFontWidth2Byte();
	const PIX	pixFontUVOffset = _pUIFontTexMgr->GetFontWidth2Byte();
	const PIX	pixFontHeight = _pUIFontTexMgr->GetFontHeight();
	const PIX	pixFontAdvancer = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	const PIX	pixFontAdvancer2Byte = pixFontWidth2Byte + _pUIFontTexMgr->GetFontSpacing();

	int			nSequentialCode, nTextureID, nU, nV;
	FLOAT		fU0, fV0, fU1, fV1, fInvU, fInvV;
	PIX			pixX1, pixY1;
	PIX			pixStartX = pixX0;

	_pUIFontTexMgr->GetInvUV( fInvU, fInvV );

	// Loop thru chars
	for( INDEX iChar = 0; iChar < ctMaxChars; iChar++ )
	{
		// Get current char
		char	cCurrent = strText[iChar];

		if( (unsigned char)cCurrent >= GBK_HI_START) //  Hi start 0xa1
		{ 
			if( (unsigned char)cCurrent <= GBK_HI_END) // Hi End
			{
				if( (unsigned char)strText[iChar + 1] >= GBK_LOW_START ) // Low start
				{
					nSequentialCode = ( (unsigned char)cCurrent - GBK_HI_START ) * GBK_LOW_BYTE_COUNT		// Hi
									+ ( (unsigned char)strText[iChar + 1] - GBK_LOW_START )	// Low
									+ ASCII_CHARACTER_COUNT;
				}
				else return;
			}
			else return;

			nTextureID = nSequentialCode / GBK_PAGE_PER_CHARACTER;

			if( nTextureID >= nTextureCount )
				break;

			// Texture coordinate
			nU = ( nSequentialCode % GBK_COUNT ) * pixFontUVOffset;
			nV = ( ( nSequentialCode % GBK_PAGE_PER_CHARACTER ) / GBK_COUNT ) * pixFontUVOffset;
			fU0 = nU * fInvU;
			fV0 = nV * fInvV;
			fU1 = ( nU + pixFontWidth2Byte ) * fInvU;
			fV1 = ( nV + pixFontHeight ) * fInvV;

			pixX1 = pixX0 + pixFontWidth2Byte;
			pixY1 = pixY0 + pixFontHeight;

			// Shadow
			if( bShadow )
			{
				// Prepare arrays
				INDEX		iStart = _avtxText[nTextureID].Count();
				GFXVertex	*pvtx = _avtxText[nTextureID].Push( 4 );
				GFXTexCoord	*ptex = _atexText[nTextureID].Push( 4 );
				GFXColor	*pcol = _acolText[nTextureID].Push( 4 );
				UWORD		*pelm = _auwText[nTextureID].Push( 6 );

				// Add char
				pvtx[0].x = pvtx[1].x = pixX0 + 1;
				pvtx[2].x = pvtx[3].x = pixX1 + 1;
				pvtx[0].y = pvtx[3].y = pixY0 + 1;
				pvtx[1].y = pvtx[2].y = pixY1 + 1;
				pvtx[0].z = pvtx[1].z = pvtx[2].z = pvtx[3].z = fZ;
				ptex[0].s = fU0;	ptex[0].t = fV0;
				ptex[1].s = fU0;	ptex[1].t = fV1;
				ptex[2].s = fU1;	ptex[2].t = fV1;
				ptex[3].s = fU1;	ptex[3].t = fV0;
				pcol[0] = glShadow;	pcol[1] = glShadow;	pcol[2] = glShadow;	pcol[3] = glShadow;
				(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
				(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
				(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );
			}

			// Prepare arrays
			INDEX		iStart = _avtxText[nTextureID].Count();
			GFXVertex	*pvtx = _avtxText[nTextureID].Push( 4 );
			GFXTexCoord	*ptex = _atexText[nTextureID].Push( 4 );
			GFXColor	*pcol = _acolText[nTextureID].Push( 4 );
			UWORD		*pelm = _auwText[nTextureID].Push( 6 );

			// Add char
			pvtx[0].x = pixX0;	pvtx[0].y = pixY0;	pvtx[0].z = fZ;
			pvtx[1].x = pixX0;	pvtx[1].y = pixY1;	pvtx[1].z = fZ;
			pvtx[2].x = pixX1;	pvtx[2].y = pixY1;	pvtx[2].z = fZ;
			pvtx[3].x = pixX1;	pvtx[3].y = pixY0;	pvtx[3].z = fZ;
			ptex[0].s = fU0;	ptex[0].t = fV0;
			ptex[1].s = fU0;	ptex[1].t = fV1;
			ptex[2].s = fU1;	ptex[2].t = fV1;
			ptex[3].s = fU1;	ptex[3].t = fV0;
			pcol[0] = glcol;	pcol[1] = glcol;	pcol[2] = glcol;	pcol[3] = glcol;
			(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
			(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
			(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );

			// Advance to next char
			pixX0 += pixFontAdvancer2Byte;
			iChar++;
		}
		else
		{
			// Texture coordinate
			nU = ( ( cCurrent - 0x1e ) % 42 ) * pixFontUVOffset;
			nV = ( ( cCurrent - 0x1e ) / 42 ) * pixFontUVOffset;
			fU0 = nU * fInvU;
			fV0 = nV * fInvV;
			fU1 = ( nU + pixFontWidth ) * fInvU;
			fV1 = ( nV + pixFontHeight ) * fInvV;

			pixX1 = pixX0 + pixFontWidth;
			pixY1 = pixY0 + pixFontHeight;

			// Shadow
			if( bShadow )
			{
				// Prepare arrays
				INDEX		iStart = _avtxText[0].Count();
				GFXVertex	*pvtx = _avtxText[0].Push( 4 );
				GFXTexCoord	*ptex = _atexText[0].Push( 4 );
				GFXColor	*pcol = _acolText[0].Push( 4 );
				UWORD		*pelm = _auwText[0].Push( 6 );

				// Add char
				pvtx[0].x = pvtx[1].x = pixX0 + 1;
				pvtx[2].x = pvtx[3].x = pixX1 + 1;
				pvtx[0].y = pvtx[3].y = pixY0 + 1;
				pvtx[1].y = pvtx[2].y = pixY1 + 1;
				pvtx[0].z = pvtx[1].z = pvtx[2].z = pvtx[3].z = fZ;
				ptex[0].s = fU0;	ptex[0].t = fV0;
				ptex[1].s = fU0;	ptex[1].t = fV1;
				ptex[2].s = fU1;	ptex[2].t = fV1;
				ptex[3].s = fU1;	ptex[3].t = fV0;
				pcol[0] = glShadow;	pcol[1] = glShadow;	pcol[2] = glShadow;	pcol[3] = glShadow;
				(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
				(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
				(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );
			}

			// Prepare arrays
			INDEX		iStart = _avtxText[0].Count();
			GFXVertex	*pvtx = _avtxText[0].Push( 4 );
			GFXTexCoord	*ptex = _atexText[0].Push( 4 );
			GFXColor	*pcol = _acolText[0].Push( 4 );
			UWORD		*pelm = _auwText[0].Push( 6 );

			// Add char
			pvtx[0].x = pixX0;	pvtx[0].y = pixY0;	pvtx[0].z = fZ;
			pvtx[1].x = pixX0;	pvtx[1].y = pixY1;	pvtx[1].z = fZ;
			pvtx[2].x = pixX1;	pvtx[2].y = pixY1;	pvtx[2].z = fZ;
			pvtx[3].x = pixX1;	pvtx[3].y = pixY0;	pvtx[3].z = fZ;
			ptex[0].s = fU0;	ptex[0].t = fV0;
			ptex[1].s = fU0;	ptex[1].t = fV1;
			ptex[2].s = fU1;	ptex[2].t = fV1;
			ptex[3].s = fU1;	ptex[3].t = fV0;
			pcol[0] = glcol;	pcol[1] = glcol;	pcol[2] = glcol;	pcol[3] = glcol;
			(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
			(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
			(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );

			// Advance to next char
			pixX0 += pixFontAdvancer;
		}
	}
}

//------------------------------------------------------------------------------
// CDrawPort::PutTextExThai
// Explain: display thai font
// Date : 2005-09-10 wooss
//------------------------------------------------------------------------------
void CDrawPort::PutTextExThai( const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend,
								FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const
{
	// Skip drawing if text falls above or below draw port
	if( pixY0 > GetHeight() || pixX0 > GetWidth() )
		return;

	// Check if string even exists
	INDEX	ctMaxChars = strText.Length();
	if( ctMaxChars <= 0 )
		return;

	GFXColor	glcol( colBlend );
	GFXColor	glShadow( colShadow );
	const int	nTextureCount = _pUIFontTexMgr->GetFontTextureCount();
	const PIX	pixFontWidth = _pUIFontTexMgr->GetFontWidth();
	const PIX	pixFontWidth2Byte = _pUIFontTexMgr->GetFontWidth2Byte();
	const PIX	pixFontUVOffset = _pUIFontTexMgr->GetFontWidth2Byte();
	const PIX	pixFontHeight = _pUIFontTexMgr->GetFontHeight();
		  PIX	pixFontAdvancer = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	const PIX	pixFontAdvancer2Byte = pixFontWidth2Byte + _pUIFontTexMgr->GetFontSpacing();

	int			nU, nV;
	FLOAT		fU0, fV0, fU1, fV1, fInvU, fInvV,thai_fV1;
	PIX			pixX1, pixY1 , thai_Y0;
	PIX			pixStartX = pixX0;
	PIX			pixStartY = pixY0;

	_pUIFontTexMgr->GetInvUV( fInvU, fInvV );
	// Loop thru chars
	for( INDEX iChar = 0; iChar < ctMaxChars; iChar++ )
	{
		// Get current char
		unsigned char	cCurrent = strText[iChar];
		unsigned char	pre_cCurrent,next_cCurrent;
		int				tv_height=0;
		{
			// If at end of current line
			if( cCurrent == '\n' || cCurrent == '\r' )
			{
				continue;
			}
			else if( cCurrent == ' ' )
			{
				// Advance to next char
				pixX0 += pixFontAdvancer;
				continue;
			}
			
			// Texture coordinate
			nU = ( ( cCurrent - 0x1e ) % 21 ) * pixFontUVOffset;
			nV = ( ( cCurrent - 0x1e ) / 21 ) * pixFontUVOffset * 2; // wooss 050929 
						
			pixFontAdvancer = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
			thai_fV1	=	0; // wooss 051001
			thai_Y0		=	0;
			if(cCurrent >= 0xa1 && cCurrent <= 0xfb){
				pixFontAdvancer = _pUIFontTexMgr->GetFontWidthThai(cCurrent) +_pUIFontTexMgr->GetFontSpacing();
				thai_Y0 = 2;
				thai_fV1 = 4 ; //태국어 V길이를 3 더한다...( 높이가 더 길다...성조 첨가로)				
				if(cCurrent == 0xd3){
					if(iChar==0) continue;
					pre_cCurrent = strText[iChar-1];
					if((pre_cCurrent>=0xa1 && pre_cCurrent<=0xcf)||(pre_cCurrent>=0xe7 && pre_cCurrent <= 0xec)){ // pre_cCurrent 050924
						pixX0-=4; // 4픽셀 앞으로
					} else continue; 
					
				}else if(cCurrent == 0xed ||cCurrent == 0xd1 || (cCurrent>=0xd4 && cCurrent<=0xda)){		// vowel display //wooss 050924 d3->d4
					if(iChar==0) continue;
					pre_cCurrent = strText[iChar-1];
					if(pre_cCurrent>=0xa1 && pre_cCurrent <= 0xcf) {
						pixX0-=7; // 겹치는 모음 출력시 7픽셀 앞으로
						pixFontAdvancer = _pUIFontTexMgr->GetFontWidthThai(cCurrent);
					} else continue;
				} 
				// tone indication display 
				else if( cCurrent>=0xe7 && cCurrent<=0xec) {
					if(iChar==0) continue;
					pre_cCurrent = strText[iChar-1];
					next_cCurrent = strText[iChar+1];
					if(pre_cCurrent == 0xed || (pre_cCurrent == 0xd1) || (pre_cCurrent>=0xd4 && pre_cCurrent<=0xd7)){ // pre_cCurrent 050924
						tv_height=(pixFontHeight/4);
					//	pixY0-=tv_height;
						
					} else if( pre_cCurrent >= 0xd8 && pre_cCurrent <= 0xda) ;
						else if(pre_cCurrent < 0xa1 || pre_cCurrent > 0xcf) // pre_cCurrent 050924
							continue;
							else if (next_cCurrent == 0xd3) {
								tv_height=(pixFontHeight/4);
							//	pixY0-=tv_height;
							}
					pixX0-=7; // 7픽셀 당겨서 찍는다
					pixFontAdvancer -= _pUIFontTexMgr->GetFontSpacing();
					
				}
			}

			fU0 = nU * fInvU;
			fV0 = nV * fInvV;
		
			fU1 = ( nU + pixFontWidth2Byte ) * fInvU;
			fV1 = ( nV + pixFontHeight+thai_fV1) * fInvV;

			pixX1 = pixX0 + pixFontWidth2Byte;
			
			pixY0 = pixStartY - thai_Y0 -tv_height;
			pixY1 = pixY0 + pixFontHeight+thai_fV1;

//			fU1 = ( nU + pixFontWidth2Byte ) * fInvU;
//			fV1 = ( nV + pixFontHeight) * fInvV;
//
//			pixX1 = pixX0 + pixFontWidth2Byte;
//			
//			pixY0 = pixStartY - thai_Y0 - tv_height;
//			pixY1 = pixY0 + pixFontHeight;


			// Shadow
			if( bShadow )
			{
				// Prepare arrays
				INDEX		iStart = _avtxText[0].Count();
				GFXVertex	*pvtx = _avtxText[0].Push( 4 );
				GFXTexCoord	*ptex = _atexText[0].Push( 4 );
				GFXColor	*pcol = _acolText[0].Push( 4 );
				UWORD		*pelm = _auwText[0].Push( 6 );

				// Add char
				pvtx[0].x = pvtx[1].x = pixX0 + 1;
				pvtx[2].x = pvtx[3].x = pixX1 + 1;
				pvtx[0].y = pvtx[3].y = pixY0 + 1;
				pvtx[1].y = pvtx[2].y = pixY1 + 1;
				pvtx[0].z = pvtx[1].z = pvtx[2].z = pvtx[3].z = fZ;
				ptex[0].s = fU0;	ptex[0].t = fV0;
				ptex[1].s = fU0;	ptex[1].t = fV1;
				ptex[2].s = fU1;	ptex[2].t = fV1;
				ptex[3].s = fU1;	ptex[3].t = fV0;
				pcol[0] = glShadow;	pcol[1] = glShadow;	pcol[2] = glShadow;	pcol[3] = glShadow;
				(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
				(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
				(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );
			}

			// Prepare arrays
			INDEX		iStart = _avtxText[0].Count();
			GFXVertex	*pvtx = _avtxText[0].Push( 4 );
			GFXTexCoord	*ptex = _atexText[0].Push( 4 );
			GFXColor	*pcol = _acolText[0].Push( 4 );
			UWORD		*pelm = _auwText[0].Push( 6 );

			// Add char
			pvtx[0].x = pixX0;	pvtx[0].y = pixY0;	pvtx[0].z = fZ;
			pvtx[1].x = pixX0;	pvtx[1].y = pixY1;	pvtx[1].z = fZ;
			pvtx[2].x = pixX1;	pvtx[2].y = pixY1;	pvtx[2].z = fZ;
			pvtx[3].x = pixX1;	pvtx[3].y = pixY0;	pvtx[3].z = fZ;
			ptex[0].s = fU0;	ptex[0].t = fV0;
			ptex[1].s = fU0;	ptex[1].t = fV1;
			ptex[2].s = fU1;	ptex[2].t = fV1;
			ptex[3].s = fU1;	ptex[3].t = fV0;
			pcol[0] = glcol;	pcol[1] = glcol;	pcol[2] = glcol;	pcol[3] = glcol;
			(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
			(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
			(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );

			// Advance to next char
//			if(cCurrent >= 0x41 && cCurrent <= 0x5a) pixX0 += pixFontWidth2Byte-2;
//			else if (cCurrent >= 0x80) pixX0 += pixFontAdvancer;
			pixX0 += pixFontAdvancer;
			pixY0+=tv_height;
		}
	}
}


//------------------------------------------------------------------------------
// CDrawPort::PutTextExJap
// Explain: display Japan font
// Date : 2005-10-21 wooss
//------------------------------------------------------------------------------
void CDrawPort::PutTextExJap( const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend,
								FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const
{
	// Skip drawing if text falls above or below draw port
	if( pixY0 > GetHeight() || pixX0 > GetWidth() )
		return;

	// Check if string even exists
	INDEX	ctMaxChars = strText.Length();
	if( ctMaxChars <= 0 )
		return;

	GFXColor	glcol( colBlend );
	GFXColor	glShadow( colShadow );
	const int	nTextureCount = _pUIFontTexMgr->GetFontTextureCount();
	const PIX	pixFontWidth = _pUIFontTexMgr->GetFontWidth();
	const PIX	pixFontWidth2Byte = _pUIFontTexMgr->GetFontWidth2Byte();
	const PIX	pixFontUVOffset = _pUIFontTexMgr->GetFontWidth2Byte();
	const PIX	pixFontHeight = _pUIFontTexMgr->GetFontHeight();
	const PIX	pixFontAdvancer = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	const PIX	pixFontAdvancer2Byte = pixFontWidth2Byte + _pUIFontTexMgr->GetFontSpacing();

	int			nSequentialCode, nTextureID, nU, nV;
	FLOAT		fU0, fV0, fU1, fV1, fInvU, fInvV;
	PIX			pixX1, pixY1;
	PIX			pixStartX = pixX0;

	_pUIFontTexMgr->GetInvUV( fInvU, fInvV );

	// wooss 051102
	// pageWords - 이전 페이지의 글자 누적수
	// markStart - 현재 페이지의 시작 위치(코드페이지상)
//	int pageWords[9]	=	{189,652,803,930,0,0,1023,1117}; // 81 - alphabet_num 82 ~ 89 start pos 
	int pageWords[8]	=	{0xbd,0x160,0x1e7,0x276,0x276,0x276,0x2d3,0x331}; // 81 - alphabet_num 82 ~ 89 start pos 
	int markStart[9]	=	{0x40,0x4f,0x40,0x40,0,0,0x40,0x9f,0x40}; 
	int markEnd[9]		=	{0xfc,0xf1,0xd6,0xbe,0,0,0x9c,0xfc,0xfc};
	//		End - Start +1 	 0xbd,0xa3,0x97,0x7f,0,0,0x5d,0x5e

	// Loop thru chars
	for( INDEX iChar = 0; iChar < ctMaxChars; iChar++ )
	{
		// Get current char
		unsigned char	cCurrent = strText[iChar];

		// If at end of current line
		if( cCurrent == '\n' || cCurrent == '\r' )
		{
			continue;
		}
		else if( cCurrent == ' ' )
		{
			// Advance to next char
			pixX0 += pixFontAdvancer;
			continue;
		}

		if(cCurrent >= 0xa1 && cCurrent <= 0xdf){
			// Texture coordinate
			nU = ( ( cCurrent - 0x1e - 0x21) % 42 ) * pixFontUVOffset; // 0x21 : 가타가나 중간의 31문자칸을 빼준다. wooss 051028
			nV = ( ( cCurrent - 0x1e - 0x21) / 42 ) * pixFontUVOffset; // 0x1e : texture상에서 00~1D까지는 출력이 안되므로....
			fU0 = nU * fInvU;										   
			fV0 = nV * fInvV;
			fU1 = ( nU + pixFontWidth2Byte ) * fInvU;
			fV1 = ( nV + pixFontHeight ) * fInvV;

			pixX1 = pixX0 + pixFontWidth2Byte;
			pixY1 = pixY0 + pixFontHeight;

			// Shadow
			if( bShadow )
			{
				// Prepare arrays
				INDEX		iStart = _avtxText[0].Count();
				GFXVertex	*pvtx = _avtxText[0].Push( 4 );
				GFXTexCoord	*ptex = _atexText[0].Push( 4 );
				GFXColor	*pcol = _acolText[0].Push( 4 );
				UWORD		*pelm = _auwText[0].Push( 6 );

				// Add char
				pvtx[0].x = pvtx[1].x = pixX0 + 1;
				pvtx[2].x = pvtx[3].x = pixX1 + 1;
				pvtx[0].y = pvtx[3].y = pixY0 + 1;
				pvtx[1].y = pvtx[2].y = pixY1 + 1;
				pvtx[0].z = pvtx[1].z = pvtx[2].z = pvtx[3].z = fZ;
				ptex[0].s = fU0;	ptex[0].t = fV0;
				ptex[1].s = fU0;	ptex[1].t = fV1;
				ptex[2].s = fU1;	ptex[2].t = fV1;
				ptex[3].s = fU1;	ptex[3].t = fV0;
				pcol[0] = glShadow;	pcol[1] = glShadow;	pcol[2] = glShadow;	pcol[3] = glShadow;
				(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
				(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
				(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );
			}

			// Prepare arrays
			INDEX		iStart = _avtxText[0].Count();
			GFXVertex	*pvtx = _avtxText[0].Push( 4 );
			GFXTexCoord	*ptex = _atexText[0].Push( 4 );
			GFXColor	*pcol = _acolText[0].Push( 4 );
			UWORD		*pelm = _auwText[0].Push( 6 );

			// Add char
			pvtx[0].x = pixX0;	pvtx[0].y = pixY0;	pvtx[0].z = fZ;
			pvtx[1].x = pixX0;	pvtx[1].y = pixY1;	pvtx[1].z = fZ;
			pvtx[2].x = pixX1;	pvtx[2].y = pixY1;	pvtx[2].z = fZ;
			pvtx[3].x = pixX1;	pvtx[3].y = pixY0;	pvtx[3].z = fZ;
			ptex[0].s = fU0;	ptex[0].t = fV0;
			ptex[1].s = fU0;	ptex[1].t = fV1;
			ptex[2].s = fU1;	ptex[2].t = fV1;
			ptex[3].s = fU1;	ptex[3].t = fV0;
			pcol[0] = glcol;	pcol[1] = glcol;	pcol[2] = glcol;	pcol[3] = glcol;
			(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
			(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
			(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );

			// Advance to next char
			pixX0 += pixFontAdvancer;
			
		} else
		if( cCurrent & 0x80 )
		{
			nSequentialCode = ASCII_CHARACTER_COUNT+KATA_CHARACTER_COUNT;
			unsigned char	cNext = strText[iChar+1];	
			switch(cCurrent)
			{
				case 0x81:
					nSequentialCode+=cNext-markStart[cCurrent-0x81];
					break;
				case 0x82:
				case 0x83:
				case 0x84:
				case 0x87:
				case 0x88:
				case 0x89:
					nSequentialCode+=cNext-markStart[cCurrent-0x81]+pageWords[cCurrent-0x82];
					break;

				default :
					if(cCurrent < 0xa0)
						nSequentialCode+=cNext-0x40+pageWords[7]+(cCurrent-0x89)*0xbd;
					else if( cCurrent > 0xdf && cCurrent < 0xeb)
						nSequentialCode+=cNext-0x40+pageWords[7]+(cCurrent-0xc9)*0xbd;
					else if( cCurrent > 0xec && cCurrent < 0xef)
						nSequentialCode+=cNext-0x40+pageWords[7]+(cCurrent-0xca)*0xbd;
					else if( cCurrent > 0xf9 && cCurrent < 0xfd)
						nSequentialCode+=cNext-0x40+pageWords[7]+(cCurrent-0xd5)*0xbd;
					break;
			}
			
			nTextureID = nSequentialCode / 1764;

			if( nTextureID >= nTextureCount )
				break;

			// Texture coordinate
			nU = ( nSequentialCode % 42 ) * pixFontUVOffset;
			nV = ( ( nSequentialCode % 1764 ) / 42 ) * pixFontUVOffset;
			fU0 = nU * fInvU;
			fV0 = nV * fInvV;
			fU1 = ( nU + pixFontWidth2Byte ) * fInvU;
			fV1 = ( nV + pixFontHeight ) * fInvV;

			pixX1 = pixX0 + pixFontWidth2Byte;
			pixY1 = pixY0 + pixFontHeight;

			// Shadow
			if( bShadow )
			{
				// Prepare arrays
				INDEX		iStart = _avtxText[nTextureID].Count();
				GFXVertex	*pvtx = _avtxText[nTextureID].Push( 4 );
				GFXTexCoord	*ptex = _atexText[nTextureID].Push( 4 );
				GFXColor	*pcol = _acolText[nTextureID].Push( 4 );
				UWORD		*pelm = _auwText[nTextureID].Push( 6 );

				// Add char
				pvtx[0].x = pvtx[1].x = pixX0 + 1;
				pvtx[2].x = pvtx[3].x = pixX1 + 1;
				pvtx[0].y = pvtx[3].y = pixY0 + 1;
				pvtx[1].y = pvtx[2].y = pixY1 + 1;
				pvtx[0].z = pvtx[1].z = pvtx[2].z = pvtx[3].z = fZ;
				ptex[0].s = fU0;	ptex[0].t = fV0;
				ptex[1].s = fU0;	ptex[1].t = fV1;
				ptex[2].s = fU1;	ptex[2].t = fV1;
				ptex[3].s = fU1;	ptex[3].t = fV0;
				pcol[0] = glShadow;	pcol[1] = glShadow;	pcol[2] = glShadow;	pcol[3] = glShadow;
				(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
				(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
				(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );
			}

			// Prepare arrays
			INDEX		iStart = _avtxText[nTextureID].Count();
			GFXVertex	*pvtx = _avtxText[nTextureID].Push( 4 );
			GFXTexCoord	*ptex = _atexText[nTextureID].Push( 4 );
			GFXColor	*pcol = _acolText[nTextureID].Push( 4 );
			UWORD		*pelm = _auwText[nTextureID].Push( 6 );

			// Add char
			pvtx[0].x = pixX0;	pvtx[0].y = pixY0;	pvtx[0].z = fZ;
			pvtx[1].x = pixX0;	pvtx[1].y = pixY1;	pvtx[1].z = fZ;
			pvtx[2].x = pixX1;	pvtx[2].y = pixY1;	pvtx[2].z = fZ;
			pvtx[3].x = pixX1;	pvtx[3].y = pixY0;	pvtx[3].z = fZ;
			ptex[0].s = fU0;	ptex[0].t = fV0;
			ptex[1].s = fU0;	ptex[1].t = fV1;
			ptex[2].s = fU1;	ptex[2].t = fV1;
			ptex[3].s = fU1;	ptex[3].t = fV0;
			pcol[0] = glcol;	pcol[1] = glcol;	pcol[2] = glcol;	pcol[3] = glcol;
			(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
			(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
			(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );

			// Advance to next char
			pixX0 += pixFontAdvancer2Byte;
			iChar++;
		}
		else {
			// Texture coordinate
			nU = ( ( cCurrent - 0x1e ) % 42 ) * pixFontUVOffset;
			nV = ( ( cCurrent - 0x1e ) / 42 ) * pixFontUVOffset;
			fU0 = nU * fInvU;
			fV0 = nV * fInvV;
			fU1 = ( nU + pixFontWidth ) * fInvU;
			fV1 = ( nV + pixFontHeight ) * fInvV;

			pixX1 = pixX0 + pixFontWidth;
			pixY1 = pixY0 + pixFontHeight;

			// Shadow
			if( bShadow )
			{
				// Prepare arrays
				INDEX		iStart = _avtxText[0].Count();
				GFXVertex	*pvtx = _avtxText[0].Push( 4 );
				GFXTexCoord	*ptex = _atexText[0].Push( 4 );
				GFXColor	*pcol = _acolText[0].Push( 4 );
				UWORD		*pelm = _auwText[0].Push( 6 );

				// Add char
				pvtx[0].x = pvtx[1].x = pixX0 + 1;
				pvtx[2].x = pvtx[3].x = pixX1 + 1;
				pvtx[0].y = pvtx[3].y = pixY0 + 1;
				pvtx[1].y = pvtx[2].y = pixY1 + 1;
				pvtx[0].z = pvtx[1].z = pvtx[2].z = pvtx[3].z = fZ;
				ptex[0].s = fU0;	ptex[0].t = fV0;
				ptex[1].s = fU0;	ptex[1].t = fV1;
				ptex[2].s = fU1;	ptex[2].t = fV1;
				ptex[3].s = fU1;	ptex[3].t = fV0;
				pcol[0] = glShadow;	pcol[1] = glShadow;	pcol[2] = glShadow;	pcol[3] = glShadow;
				(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
				(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
				(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );
			}

			// Prepare arrays
			INDEX		iStart = _avtxText[0].Count();
			GFXVertex	*pvtx = _avtxText[0].Push( 4 );
			GFXTexCoord	*ptex = _atexText[0].Push( 4 );
			GFXColor	*pcol = _acolText[0].Push( 4 );
			UWORD		*pelm = _auwText[0].Push( 6 );

			// Add char
			pvtx[0].x = pixX0;	pvtx[0].y = pixY0;	pvtx[0].z = fZ;
			pvtx[1].x = pixX0;	pvtx[1].y = pixY1;	pvtx[1].z = fZ;
			pvtx[2].x = pixX1;	pvtx[2].y = pixY1;	pvtx[2].z = fZ;
			pvtx[3].x = pixX1;	pvtx[3].y = pixY0;	pvtx[3].z = fZ;
			ptex[0].s = fU0;	ptex[0].t = fV0;
			ptex[1].s = fU0;	ptex[1].t = fV1;
			ptex[2].s = fU1;	ptex[2].t = fV1;
			ptex[3].s = fU1;	ptex[3].t = fV0;
			pcol[0] = glcol;	pcol[1] = glcol;	pcol[2] = glcol;	pcol[3] = glcol;
			(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
			(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
			(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );

			// Advance to next char
			pixX0 += pixFontAdvancer;
		}
	}

}

//------------------------------------------------------------------------------
// CDrawPort::PutTextExBrz
// Explain: display Brzil font
// Date : 2006-11-02 eons
//------------------------------------------------------------------------------
void CDrawPort::PutTextExBrz(const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend,
							 FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const
{
	// Skip drawing if text falls above or below draw port
	if( pixY0 > GetHeight() || pixX0 > GetWidth() )
		return;

	// Check if string even exists
	INDEX	ctMaxChars = strText.Length();
	if( ctMaxChars <= 0 )
		return;

	GFXColor	glcol( colBlend );
	GFXColor	glShadow( colShadow );
	const int	nTextureCount = _pUIFontTexMgr->GetFontTextureCount();
	const PIX	pixFontWidth = _pUIFontTexMgr->GetFontWidth();
	const PIX	pixFontWidth2Byte = _pUIFontTexMgr->GetFontWidth2Byte();
	const PIX	pixFontUVOffset = _pUIFontTexMgr->GetFontWidth2Byte();
	const PIX	pixFontHeight = _pUIFontTexMgr->GetFontHeight();
		  PIX	pixFontAdvancer = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	const PIX	pixFontAdvancer2Byte = pixFontWidth2Byte + _pUIFontTexMgr->GetFontSpacing();

	int			nU, nV;
	FLOAT		fU0, fV0, fU1, fV1, fInvU, fInvV;
	PIX			pixX1, pixY1;
	PIX			pixStartX = pixX0;
	PIX			pixStartY = pixY0;

	_pUIFontTexMgr->GetInvUV( fInvU, fInvV );
	// Loop thru chars
	for( INDEX iChar = 0; iChar < ctMaxChars; iChar++ )
	{
		// Get current char
		unsigned char	cCurrent = strText[iChar];
//		unsigned char	pre_cCurrent,next_cCurrent;
		
		{
			// If at end of current line
			if( cCurrent == '\n' || cCurrent == '\r' )
			{
				continue;
			}
			else if( cCurrent == ' ' )
			{
				// Advance to next char
				pixX0 += pixFontAdvancer;
				continue;
			}
			
			// Texture coordinate
			// 
			nU = ( ( cCurrent - 0x1e ) % GBK_COUNT ) * pixFontUVOffset;
			nV = ( ( cCurrent - 0x1e ) / GBK_COUNT ) * pixFontUVOffset * 2; // wooss 050929 

			fU0 = nU * fInvU;
			fV0 = nV * fInvV;
		
			fU1 = ( nU + pixFontWidth2Byte ) * fInvU;
			fV1 = ( nV + pixFontHeight + cBrzFontOffsetV ) * fInvV;

			pixX1 = pixX0 + pixFontWidth2Byte;
			pixY1 = pixY0 + pixFontHeight + cBrzFontOffsetV;

			// Shadow
			if( bShadow )
			{
				// Prepare arrays
				INDEX		iStart = _avtxText[0].Count();
				GFXVertex	*pvtx = _avtxText[0].Push( 4 );
				GFXTexCoord	*ptex = _atexText[0].Push( 4 );
				GFXColor	*pcol = _acolText[0].Push( 4 );
				UWORD		*pelm = _auwText[0].Push( 6 );

				// Add char
				pvtx[0].x = pvtx[1].x = pixX0 + 1;
				pvtx[2].x = pvtx[3].x = pixX1 + 1;
				pvtx[0].y = pvtx[3].y = pixY0 + 1;
				pvtx[1].y = pvtx[2].y = pixY1 + 1;
				pvtx[0].z = pvtx[1].z = pvtx[2].z = pvtx[3].z = fZ;
				ptex[0].s = fU0;	ptex[0].t = fV0;
				ptex[1].s = fU0;	ptex[1].t = fV1;
				ptex[2].s = fU1;	ptex[2].t = fV1;
				ptex[3].s = fU1;	ptex[3].t = fV0;
				pcol[0] = glShadow;	pcol[1] = glShadow;	pcol[2] = glShadow;	pcol[3] = glShadow;
				(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
				(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
				(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );
			}

			// Prepare arrays
			INDEX		iStart = _avtxText[0].Count();
			GFXVertex	*pvtx = _avtxText[0].Push( 4 );
			GFXTexCoord	*ptex = _atexText[0].Push( 4 );
			GFXColor	*pcol = _acolText[0].Push( 4 );
			UWORD		*pelm = _auwText[0].Push( 6 );

			// Add char
			pvtx[0].x = pixX0;	pvtx[0].y = pixY0;	pvtx[0].z = fZ;
			pvtx[1].x = pixX0;	pvtx[1].y = pixY1;	pvtx[1].z = fZ;
			pvtx[2].x = pixX1;	pvtx[2].y = pixY1;	pvtx[2].z = fZ;
			pvtx[3].x = pixX1;	pvtx[3].y = pixY0;	pvtx[3].z = fZ;
			ptex[0].s = fU0;	ptex[0].t = fV0;
			ptex[1].s = fU0;	ptex[1].t = fV1;
			ptex[2].s = fU1;	ptex[2].t = fV1;
			ptex[3].s = fU1;	ptex[3].t = fV0;
			pcol[0] = glcol;	pcol[1] = glcol;	pcol[2] = glcol;	pcol[3] = glcol;
			(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
			(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
			(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );

			pixX0 += pixFontAdvancer;
		}
	}
}
//------------------------------------------------------------------------------
// CDrawPort::PutTextExRus
// Explain: display Russia font
// Date : 
//------------------------------------------------------------------------------
void CDrawPort::PutTextExRus(const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend,
							 FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const
{
//	CDrawPort *pD = (CDrawPort *)this;
//	pD->PutTextRus(strText, pixX0, pixY0, colBlend);

	// check API and adjust position for D3D by half pixel
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);

	// skip drawing if text falls above or below draw port
	if( pixY0>GetHeight() || pixX0>GetWidth()) return;
	// check if string even exists
	if( strText.Length()<=0) return;
	char acTmp[7]; // needed for strtoul()
	char *pcDummy; 
	INDEX iRet;

	// cache char and texture dimensions
	const FLOAT fTextScalingX   = dp_fTextScaling*dp_fTextAspect;
	const SLONG fixTextScalingX = FloatToInt(fTextScalingX  *65536.0f);
	const SLONG fixTextScalingY = FloatToInt(dp_fTextScaling*65536.0f);
	const PIX pixCharHeight     = dp_FontData->fd_pixCharHeight-1;
	const PIX pixScaledHeight   = (pixCharHeight*fixTextScalingY)>>16;
	const PIX pixShadowSpacing  = dp_FontData->fd_pixShadowSpacing;
	const FLOAT fItalicAdjustX  = fTextScalingX * (pixScaledHeight)*0.2f;  // 20% slanted
	const FLOAT fBoldAdjustX    = fTextScalingX * (dp_FontData->fd_pixCharWidth)*0.2f;  // 20% fat (extra light mayonnaise:)
	const BOOL bFixedWidth = dp_FontData->fd_ulFlags & FNF_FIXED;
	const BOOL bCellBased  = dp_FontData->fd_ulFlags & FNF_CELLBASED;
	const FLOAT fDropShadowX = dp_fTextShadow * dp_fTextScaling*dp_fTextAspect;
	const FLOAT fDropShadowY = dp_fTextShadow * dp_fTextScaling;

	// prepare font texture
	gfxSetTextureWrapping( GFX_REPEAT, GFX_REPEAT);
	CTextureData &td = *dp_FontData->fd_ptdTextureData;
	ASSERT( td.td_ctFineMipLevels<2);  // font texture cannot have mipmaps!

	// calculate and apply correction factor
	FLOAT fCorrectionU = 1.0f / td.GetPixWidth();
	FLOAT fCorrectionV = 1.0f / td.GetPixHeight();
	INDEX ctMaxChars = (INDEX)strlen(strText);
	// determine text color
	GFXColor glcolDefault(colBlend);
	GFXColor glcol = glcolDefault;
	ULONG ulAlphaDefault = (colBlend&CT_AMASK)>>CT_ASHIFT;;  // for flasher

	// prepare some text control and output vars
	FLOAT fBold   = (dp_ulTextFlags&DPTF_BOLD)   ? fBoldAdjustX   : 0;
	FLOAT fItalic = (dp_ulTextFlags&DPTF_ITALIC) ? fItalicAdjustX : 0;
	INDEX iFlash  = 0;
	ULONG ulAlpha = ulAlphaDefault;
	TIME  tmFrame = _pGfx->gl_tvFrameTime.GetSeconds();
	BOOL  bParse  = !(dp_ulTextFlags&(DPTF_PRINTSPECIALCODES|DPTF_IGNORESPECIALCODES));

	// cell/advancer for fixed width and old font (default)
	PIX pixAdvancer  = ((dp_FontData->fd_pixCharWidth *fixTextScalingX)>>16) + dp_pixTextCharSpacing - pixShadowSpacing;
	PIX pixCellWidth = dp_FontData->fd_pixCharWidth;

	// loop thru chars
	PIX pixStartX = pixX0;
	INDEX ctCharsPrinted=0, ctShadowsPrinted=0;
	for( INDEX iChar=0; iChar<ctMaxChars; iChar++)
	{
		// get current char
		unsigned char chrCurrent = strText[iChar];

		// if at end of current line
		if( chrCurrent=='\n') {
			// advance to next line
			pixX0  = pixStartX;
			pixY0 += pixScaledHeight + dp_pixTextLineSpacing - pixShadowSpacing;
			if( pixY0>GetHeight()) break;
			// skip to next char
			continue;
		}
		// special char encountered and allowed?
		else if( chrCurrent=='^' && !(dp_ulTextFlags&DPTF_PRINTSPECIALCODES)) {
			// get next char
			chrCurrent = strText[++iChar];
			COLOR col;
			switch( chrCurrent)
			{
			// color change?
			case 'c':
				strncpy( acTmp, &strText[iChar+1], 6);
				iRet = FindByte( 0, (UBYTE*)&strText[iChar+1], 6);
				iChar+=iRet;
				if( !bParse || iRet<6) continue;
				acTmp[6] = '\0'; // terminate string
				col = strtoul( acTmp, &pcDummy, 16) <<8;
				glcol.Set( col|glcol.a); // do color change but keep original alpha
				continue;
			// alpha change?
			case 'a':
				strncpy( acTmp, &strText[iChar+1], 2);
				iRet = FindByte( 0, (UBYTE*)&strText[iChar+1], 2);
				iChar+=iRet;
				if( !bParse || iRet<2) continue;
				acTmp[2] = '\0'; // terminate string
				ulAlpha = strtoul( acTmp, &pcDummy, 16);
				continue;
			// flash?
			case 'f':
				chrCurrent = strText[++iChar];
				if( bParse) iFlash = 1+ 2* Clamp( (INDEX)(chrCurrent-'0'), 0L, 9L);
				continue;
			// reset all?
			case 'r':
				fBold   = 0;
				fItalic = 0;
				iFlash  = 0;
				glcol   = glcolDefault;
				ulAlpha = ulAlphaDefault;
				continue;
			// simple codes ...
			case 'o':  bParse = bParse && gfx_bDecoratedText;  continue;  // allow console override settings?
			case 'b':  if( bParse) fBold   = fBoldAdjustX;     continue;  // bold?
			case 'i':  if( bParse) fItalic = fItalicAdjustX;   continue;  // italic?
			case 'C':  glcol   = glcolDefault;    continue;  // color reset?
			case 'A':  ulAlpha = ulAlphaDefault;  continue;  // alpha reset?
			case 'B':  fBold   = 0;  continue;  // no bold?
			case 'I':  fItalic = 0;  continue;  // italic?
			case 'F':  iFlash  = 0;  continue;  // no flash?
			default:   break;
			} // unrecognized special code or just plain ^
			if( chrCurrent!='^') { iChar--; break; }
		}
		// ignore tab
		else if( chrCurrent=='\t') continue;

		// if character is not defined (could be missing small letters in font)
		if( !dp_FontData->IsCharDefined(chrCurrent)) {
			char chrUpper = _toupper(chrCurrent);
			if( (UBYTE)chrUpper==0xFF) chrUpper = (char)0x9F;
			// if its upper version is defined and font uses small caps
			if( dp_FontData->IsCharDefined(chrUpper) && dp_FontData->fd_bSmallCaps) chrCurrent = chrUpper;
		}

		// fetch char props
		const CFontCharData &fcdCurrent = dp_FontData->fd_fcdFontCharData[chrCurrent];
		const PIX pixCharX = fcdCurrent.fcd_pixXOffset;
		const PIX pixCharY = fcdCurrent.fcd_pixYOffset;
		const PIX pixCharStart = fcdCurrent.fcd_pixStart;
		const PIX pixCharEnd   = fcdCurrent.fcd_pixEnd;
		if( !bCellBased) pixCellWidth = pixCharEnd;
		const PIX pixScaledWidth = (pixCellWidth*fixTextScalingX)>>16;

		// determine corresponding char width and position adjustments
		PIX pixXA; // adjusted starting X location of printout
		if( bFixedWidth) {
			// for fixed font
			pixXA = pixX0 - ((pixCharStart*fixTextScalingX)>>16)
						+ (((pixScaledWidth<<16) - ((pixCharEnd-pixCharStart)*fixTextScalingX) +0x10000) >>17);
		} else {
			// for proportional font
			pixXA = pixX0 - ((pixCharStart*fixTextScalingX)>>16);
			pixAdvancer = (((pixCharEnd-pixCharStart)*fixTextScalingX)>>16) +dp_pixTextCharSpacing - pixShadowSpacing;
		}
		// out of screen (left) ?
		if( pixXA>GetWidth() || (pixXA+pixCharEnd)<0) {
			// skip to next char
			pixX0 += pixAdvancer;
			continue; 
		}

		// adjust alpha for flashing
		if( iFlash>0) glcol.a = ulAlpha*(sin(iFlash*tmFrame)*0.5f+0.5f);
		else glcol.a = ulAlpha; 

		// prepare coordinates for screen and texture
		const FLOAT fX0 = pixXA;  const FLOAT fX1 = fX0 +pixScaledWidth;
		const FLOAT fY0 = pixY0;  const FLOAT fY1 = fY0 +pixScaledHeight;
		const FLOAT fU0 = pixCharX *fCorrectionU;  const FLOAT fU1 = (pixCharX+pixCellWidth)  *fCorrectionU;
		const FLOAT fV0 = pixCharY *fCorrectionV;  const FLOAT fV1 = (pixCharY+ ( chrCurrent != 'i' ? pixCharHeight : pixCharHeight - 1 ) ) *fCorrectionV;

		// add drop shadow
		if( fDropShadowX) {
			GFXColor glShdCol = 0;
			glShdCol.a = glcol.a;

			INDEX		iStart = _avtxText[0].Count();
			GFXVertex	*pvtx = _avtxText[0].Push( 4 );
			GFXTexCoord	*ptex = _atexText[0].Push( 4 );
			GFXColor	*pcol = _acolText[0].Push( 4 );
			UWORD		*pelm = _auwText[0].Push( 6 );

			pvtx[0].x = fX0 +fDropShadowX+fItalic;        pvtx[0].y = fY0 +fDropShadowY;  pvtx[0].z = fZ;
			pvtx[1].x = fX0 +fDropShadowX;                pvtx[1].y = fY1 +fDropShadowY;  pvtx[1].z = fZ;
			pvtx[2].x = fX1 +fDropShadowX+fBold;          pvtx[2].y = fY1 +fDropShadowY;  pvtx[2].z = fZ;
			pvtx[3].x = fX1 +fDropShadowX+fBold+fItalic;  pvtx[3].y = fY0 +fDropShadowY;  pvtx[3].z = fZ;
			ptex[0].s = fU0;  ptex[0].t = fV0;
			ptex[1].s = fU0;  ptex[1].t = fV1;
			ptex[2].s = fU1;  ptex[2].t = fV1;
			ptex[3].s = fU1;  ptex[3].t = fV0;
			pcol[0] = glShdCol;
			pcol[1] = glShdCol;
			pcol[2] = glShdCol;
			pcol[3] = glShdCol;

			(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
			(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
			(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );
			ctShadowsPrinted++;
		}

		INDEX		iStart = _avtxText[0].Count();
		GFXVertex	*pvtx = _avtxText[0].Push( 4 );
		GFXTexCoord	*ptex = _atexText[0].Push( 4 );
		GFXColor	*pcol = _acolText[0].Push( 4 );
		UWORD		*pelm = _auwText[0].Push( 6 );

		// add char
		pvtx[0].x = fX0+fItalic;        pvtx[0].y = fY0;  pvtx[0].z = fZ;
		pvtx[1].x = fX0;                pvtx[1].y = fY1;  pvtx[1].z = fZ;
		pvtx[2].x = fX1+fBold;          pvtx[2].y = fY1;  pvtx[2].z = fZ;
		pvtx[3].x = fX1+fBold+fItalic;  pvtx[3].y = fY0;  pvtx[3].z = fZ;
		ptex[0].s = fU0;  ptex[0].t = fV0;
		ptex[1].s = fU0;  ptex[1].t = fV1;
		ptex[2].s = fU1;  ptex[2].t = fV1;
		ptex[3].s = fU1;  ptex[3].t = fV0;
		pcol[0] = glcol;
		pcol[1] = glcol;
		pcol[2] = glcol;
		pcol[3] = glcol;
		(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
		(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
		(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );

		// advance to next char
		pixX0 += pixAdvancer;
		ctCharsPrinted++;
	}

	// adjust vertex arrays size according to chars that really got printed out
	ctCharsPrinted += ctShadowsPrinted;
}

void CDrawPort::PutTextCharExKor( const char *pText, int nLength, PIX pixX0, PIX pixY0, const COLOR colBlend,
									FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const
{
	// Skip drawing if text falls above or below draw port
	if( pixY0 > GetHeight() || pixX0 > GetWidth() )
		return;

	// Check if string even exists
	INDEX	ctMaxChars;
	if( nLength == 0 ) ctMaxChars = strlen( pText );
	else ctMaxChars = nLength;
	if( ctMaxChars <= 0 )
		return;

	GFXColor	glcol( colBlend );
	GFXColor	glShadow( colShadow );
	const int	nTextureCount = _pUIFontTexMgr->GetFontTextureCount();
	const PIX	pixFontWidth = _pUIFontTexMgr->GetFontWidth();
	const PIX	pixFontWidth2Byte = _pUIFontTexMgr->GetFontWidth2Byte();
	const PIX	pixFontUVOffset = _pUIFontTexMgr->GetFontWidth2Byte();
	const PIX	pixFontHeight = _pUIFontTexMgr->GetFontHeight();
	const PIX	pixFontAdvancer = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	const PIX	pixFontAdvancer2Byte = pixFontWidth2Byte + _pUIFontTexMgr->GetFontSpacing();

	int			nSequentialCode, nTextureID, nU, nV;
	FLOAT		fU0, fV0, fU1, fV1, fInvU, fInvV;
	PIX			pixX1, pixY1;
	PIX			pixStartX = pixX0;

	_pUIFontTexMgr->GetInvUV( fInvU, fInvV );

	// Loop thru chars
	for( INDEX iChar = 0; iChar < ctMaxChars; iChar++ )
	{
		// Get current char
		char	cCurrent = pText[iChar];

		// If at end of current line
		if( cCurrent == '\n' || cCurrent == '\r' )
		{
			continue;
		}
		else if( cCurrent == ' ' )
		{
			// Advance to next char
			pixX0 += pixFontAdvancer;
			continue;
		}

		if( cCurrent & 0x80 )
		{
			nSequentialCode = ( (unsigned char)cCurrent - 0x81 ) * 0xBE +
								( (unsigned char)pText[iChar + 1] - 0x41 ) + 98;
			nTextureID = nSequentialCode / 1764;

			if( nTextureID >= nTextureCount )
				break;

			// Texture coordinate
			nU = ( nSequentialCode % 42 ) * pixFontUVOffset;
			nV = ( ( nSequentialCode % 1764 ) / 42 ) * pixFontUVOffset;
			fU0 = nU * fInvU;
			fV0 = nV * fInvV;
			fU1 = ( nU + pixFontWidth2Byte ) * fInvU;
			fV1 = ( nV + pixFontHeight ) * fInvV;

			pixX1 = pixX0 + pixFontWidth2Byte;
			pixY1 = pixY0 + pixFontHeight;

			// Shadow
			if( bShadow )
			{
				// Prepare arrays
				INDEX		iStart = _avtxText[nTextureID].Count();
				GFXVertex	*pvtx = _avtxText[nTextureID].Push( 4 );
				GFXTexCoord	*ptex = _atexText[nTextureID].Push( 4 );
				GFXColor	*pcol = _acolText[nTextureID].Push( 4 );
				UWORD		*pelm = _auwText[nTextureID].Push( 6 );

				// Add char
				pvtx[0].x = pvtx[1].x = pixX0 + 1;
				pvtx[2].x = pvtx[3].x = pixX1 + 1;
				pvtx[0].y = pvtx[3].y = pixY0 + 1;
				pvtx[1].y = pvtx[2].y = pixY1 + 1;
				pvtx[0].z = pvtx[1].z = pvtx[2].z = pvtx[3].z = fZ;
				ptex[0].s = fU0;	ptex[0].t = fV0;
				ptex[1].s = fU0;	ptex[1].t = fV1;
				ptex[2].s = fU1;	ptex[2].t = fV1;
				ptex[3].s = fU1;	ptex[3].t = fV0;
				pcol[0] = glShadow;	pcol[1] = glShadow;	pcol[2] = glShadow;	pcol[3] = glShadow;
				(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
				(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
				(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );
			}

			// Prepare arrays
			INDEX		iStart = _avtxText[nTextureID].Count();
			GFXVertex	*pvtx = _avtxText[nTextureID].Push( 4 );
			GFXTexCoord	*ptex = _atexText[nTextureID].Push( 4 );
			GFXColor	*pcol = _acolText[nTextureID].Push( 4 );
			UWORD		*pelm = _auwText[nTextureID].Push( 6 );

			// Add char
			pvtx[0].x = pixX0;	pvtx[0].y = pixY0;	pvtx[0].z = fZ;
			pvtx[1].x = pixX0;	pvtx[1].y = pixY1;	pvtx[1].z = fZ;
			pvtx[2].x = pixX1;	pvtx[2].y = pixY1;	pvtx[2].z = fZ;
			pvtx[3].x = pixX1;	pvtx[3].y = pixY0;	pvtx[3].z = fZ;
			ptex[0].s = fU0;	ptex[0].t = fV0;
			ptex[1].s = fU0;	ptex[1].t = fV1;
			ptex[2].s = fU1;	ptex[2].t = fV1;
			ptex[3].s = fU1;	ptex[3].t = fV0;
			pcol[0] = glcol;	pcol[1] = glcol;	pcol[2] = glcol;	pcol[3] = glcol;
			(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
			(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
			(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );

			// Advance to next char
			pixX0 += pixFontAdvancer2Byte;
			iChar++;
		}
		else
		{
			// Texture coordinate
			nU = ( ( cCurrent - 0x1e ) % 42 ) * pixFontUVOffset;
			nV = ( ( cCurrent - 0x1e ) / 42 ) * pixFontUVOffset;
			fU0 = nU * fInvU;
			fV0 = nV * fInvV;
			//fU1 = ( nU + pixFontWidth+1 ) * fInvU;
			fU1 = ( nU + pixFontWidth ) * fInvU;
			fV1 = ( nV + pixFontHeight ) * fInvV;

			pixX1 = pixX0 + pixFontWidth;
			pixY1 = pixY0 + pixFontHeight;

			// Shadow
			if( bShadow )
			{
				// Prepare arrays
				INDEX		iStart = _avtxText[0].Count();
				GFXVertex	*pvtx = _avtxText[0].Push( 4 );
				GFXTexCoord	*ptex = _atexText[0].Push( 4 );
				GFXColor	*pcol = _acolText[0].Push( 4 );
				UWORD		*pelm = _auwText[0].Push( 6 );

				// Add char
				pvtx[0].x = pvtx[1].x = pixX0 + 1;
				pvtx[2].x = pvtx[3].x = pixX1 + 1;
				pvtx[0].y = pvtx[3].y = pixY0 + 1;
				pvtx[1].y = pvtx[2].y = pixY1 + 1;
				pvtx[0].z = pvtx[1].z = pvtx[2].z = pvtx[3].z = fZ;
				ptex[0].s = fU0;	ptex[0].t = fV0;
				ptex[1].s = fU0;	ptex[1].t = fV1;
				ptex[2].s = fU1;	ptex[2].t = fV1;
				ptex[3].s = fU1;	ptex[3].t = fV0;
				pcol[0] = glShadow;	pcol[1] = glShadow;	pcol[2] = glShadow;	pcol[3] = glShadow;
				(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
				(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
				(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );
			}

			// Prepare arrays
			INDEX		iStart = _avtxText[0].Count();
			GFXVertex	*pvtx = _avtxText[0].Push( 4 );
			GFXTexCoord	*ptex = _atexText[0].Push( 4 );
			GFXColor	*pcol = _acolText[0].Push( 4 );
			UWORD		*pelm = _auwText[0].Push( 6 );

			// Add char
			pvtx[0].x = pixX0;	pvtx[0].y = pixY0;	pvtx[0].z = fZ;
			pvtx[1].x = pixX0;	pvtx[1].y = pixY1;	pvtx[1].z = fZ;
			pvtx[2].x = pixX1;	pvtx[2].y = pixY1;	pvtx[2].z = fZ;
			pvtx[3].x = pixX1;	pvtx[3].y = pixY0;	pvtx[3].z = fZ;
			ptex[0].s = fU0;	ptex[0].t = fV0;
			ptex[1].s = fU0;	ptex[1].t = fV1;
			ptex[2].s = fU1;	ptex[2].t = fV1;
			ptex[3].s = fU1;	ptex[3].t = fV0;
			pcol[0] = glcol;	pcol[1] = glcol;	pcol[2] = glcol;	pcol[3] = glcol;
			(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
			(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
			(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );

			// Advance to next char
			pixX0 += pixFontAdvancer;
		}
	}
}

void CDrawPort::PutTextCharExCht( const char *pText, int nLength, PIX pixX0, PIX pixY0, const COLOR colBlend,
									FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const
{
	// Skip drawing if text falls above or below draw port
	if( pixY0 > GetHeight() || pixX0 > GetWidth() )
		return;

	// Check if string even exists
	INDEX	ctMaxChars;
	if( nLength == 0 ) ctMaxChars = strlen( pText );
	else ctMaxChars = nLength;
	if( ctMaxChars <= 0 )
		return;

	GFXColor	glcol( colBlend );
	GFXColor	glShadow( colShadow );
	const int	nTextureCount = _pUIFontTexMgr->GetFontTextureCount();
	const PIX	pixFontWidth = _pUIFontTexMgr->GetFontWidth();
	const PIX	pixFontWidth2Byte = _pUIFontTexMgr->GetFontWidth2Byte();
	const PIX	pixFontUVOffset = _pUIFontTexMgr->GetFontWidth2Byte();
	const PIX	pixFontHeight = _pUIFontTexMgr->GetFontHeight();
	const PIX	pixFontAdvancer = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	const PIX	pixFontAdvancer2Byte = pixFontWidth2Byte + _pUIFontTexMgr->GetFontSpacing();

	int			nSequentialCode, nTextureID, nU, nV;
	FLOAT		fU0, fV0, fU1, fV1, fInvU, fInvV;
	PIX			pixX1, pixY1;
	PIX			pixStartX = pixX0;

	_pUIFontTexMgr->GetInvUV( fInvU, fInvV );

	// Loop thru chars
	for( INDEX iChar = 0; iChar < ctMaxChars; iChar++ )
	{
		// Get current char
		char	cCurrent = pText[iChar];

		// If at end of current line
		if( cCurrent == '\n' || cCurrent == '\r' )
		{
			continue;
		}
		else if( cCurrent == ' ' )
		{
			// Advance to next char
			pixX0 += pixFontAdvancer;
			continue;
		}

		if( cCurrent & 0x80 )
		{
			if( (unsigned char)cCurrent >= 0xC9 )
			{
				if( (unsigned char)pText[iChar + 1] < 0xA1 )
				{
					nSequentialCode = ( (unsigned char)cCurrent - 0xC9 + 0x26 ) * 0x9D +
								( (unsigned char)pText[iChar + 1] - 0x40 ) + 98;
				}
				else
				{
					nSequentialCode = ( (unsigned char)cCurrent - 0xC9 + 0x26 ) * 0x9D +
								( (unsigned char)pText[iChar + 1] - 0xA1 + 0x3F ) + 98;
				}
			}
			else
			{ 
				if( (unsigned char)pText[iChar + 1] < 0xA1 )
				{
					nSequentialCode = ( (unsigned char)cCurrent - 0xA1 ) * 0x9D +
								( (unsigned char)pText[iChar + 1] - 0x40 ) + 98;
				}
				else
				{
					nSequentialCode = ( (unsigned char)cCurrent - 0xA1 ) * 0x9D +
								( (unsigned char)pText[iChar + 1] - 0xA1 + 0x3F ) + 98;
				}
			}
			nTextureID = nSequentialCode / 1764;

			if( nTextureID >= nTextureCount )
				break;

			// Texture coordinate
			nU = ( nSequentialCode % 42 ) * pixFontUVOffset;
			nV = ( ( nSequentialCode % 1764 ) / 42 ) * pixFontUVOffset;
			fU0 = nU * fInvU;
			fV0 = nV * fInvV;
			fU1 = ( nU + pixFontWidth2Byte ) * fInvU;
			fV1 = ( nV + pixFontHeight ) * fInvV;

			pixX1 = pixX0 + pixFontWidth2Byte;
			pixY1 = pixY0 + pixFontHeight;

			// Shadow
			if( bShadow )
			{
				// Prepare arrays
				INDEX		iStart = _avtxText[nTextureID].Count();
				GFXVertex	*pvtx = _avtxText[nTextureID].Push( 4 );
				GFXTexCoord	*ptex = _atexText[nTextureID].Push( 4 );
				GFXColor	*pcol = _acolText[nTextureID].Push( 4 );
				UWORD		*pelm = _auwText[nTextureID].Push( 6 );

				// Add char
				pvtx[0].x = pvtx[1].x = pixX0 + 1;
				pvtx[2].x = pvtx[3].x = pixX1 + 1;
				pvtx[0].y = pvtx[3].y = pixY0 + 1;
				pvtx[1].y = pvtx[2].y = pixY1 + 1;
				pvtx[0].z = pvtx[1].z = pvtx[2].z = pvtx[3].z = fZ;
				ptex[0].s = fU0;	ptex[0].t = fV0;
				ptex[1].s = fU0;	ptex[1].t = fV1;
				ptex[2].s = fU1;	ptex[2].t = fV1;
				ptex[3].s = fU1;	ptex[3].t = fV0;
				pcol[0] = glShadow;	pcol[1] = glShadow;	pcol[2] = glShadow;	pcol[3] = glShadow;
				(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
				(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
				(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );
			}

			// Prepare arrays
			INDEX		iStart = _avtxText[nTextureID].Count();
			GFXVertex	*pvtx = _avtxText[nTextureID].Push( 4 );
			GFXTexCoord	*ptex = _atexText[nTextureID].Push( 4 );
			GFXColor	*pcol = _acolText[nTextureID].Push( 4 );
			UWORD		*pelm = _auwText[nTextureID].Push( 6 );

			// Add char
			pvtx[0].x = pixX0;	pvtx[0].y = pixY0;	pvtx[0].z = fZ;
			pvtx[1].x = pixX0;	pvtx[1].y = pixY1;	pvtx[1].z = fZ;
			pvtx[2].x = pixX1;	pvtx[2].y = pixY1;	pvtx[2].z = fZ;
			pvtx[3].x = pixX1;	pvtx[3].y = pixY0;	pvtx[3].z = fZ;
			ptex[0].s = fU0;	ptex[0].t = fV0;
			ptex[1].s = fU0;	ptex[1].t = fV1;
			ptex[2].s = fU1;	ptex[2].t = fV1;
			ptex[3].s = fU1;	ptex[3].t = fV0;
			pcol[0] = glcol;	pcol[1] = glcol;	pcol[2] = glcol;	pcol[3] = glcol;
			(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
			(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
			(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );

			// Advance to next char
			pixX0 += pixFontAdvancer2Byte;
			iChar++;
		}
		else
		{
			// Texture coordinate
			nU = ( ( cCurrent - 0x1e ) % 42 ) * pixFontUVOffset;
			nV = ( ( cCurrent - 0x1e ) / 42 ) * pixFontUVOffset;
			fU0 = nU * fInvU;
			fV0 = nV * fInvV;
			fU1 = ( nU + pixFontWidth ) * fInvU;
			fV1 = ( nV + pixFontHeight ) * fInvV;

			pixX1 = pixX0 + pixFontWidth;
			pixY1 = pixY0 + pixFontHeight;

			// Shadow
			if( bShadow )
			{
				// Prepare arrays
				INDEX		iStart = _avtxText[0].Count();
				GFXVertex	*pvtx = _avtxText[0].Push( 4 );
				GFXTexCoord	*ptex = _atexText[0].Push( 4 );
				GFXColor	*pcol = _acolText[0].Push( 4 );
				UWORD		*pelm = _auwText[0].Push( 6 );

				// Add char
				pvtx[0].x = pvtx[1].x = pixX0 + 1;
				pvtx[2].x = pvtx[3].x = pixX1 + 1;
				pvtx[0].y = pvtx[3].y = pixY0 + 1;
				pvtx[1].y = pvtx[2].y = pixY1 + 1;
				pvtx[0].z = pvtx[1].z = pvtx[2].z = pvtx[3].z = fZ;
				ptex[0].s = fU0;	ptex[0].t = fV0;
				ptex[1].s = fU0;	ptex[1].t = fV1;
				ptex[2].s = fU1;	ptex[2].t = fV1;
				ptex[3].s = fU1;	ptex[3].t = fV0;
				pcol[0] = glShadow;	pcol[1] = glShadow;	pcol[2] = glShadow;	pcol[3] = glShadow;
				(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
				(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
				(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );
			}

			// Prepare arrays
			INDEX		iStart = _avtxText[0].Count();
			GFXVertex	*pvtx = _avtxText[0].Push( 4 );
			GFXTexCoord	*ptex = _atexText[0].Push( 4 );
			GFXColor	*pcol = _acolText[0].Push( 4 );
			UWORD		*pelm = _auwText[0].Push( 6 );

			// Add char
			pvtx[0].x = pixX0;	pvtx[0].y = pixY0;	pvtx[0].z = fZ;
			pvtx[1].x = pixX0;	pvtx[1].y = pixY1;	pvtx[1].z = fZ;
			pvtx[2].x = pixX1;	pvtx[2].y = pixY1;	pvtx[2].z = fZ;
			pvtx[3].x = pixX1;	pvtx[3].y = pixY0;	pvtx[3].z = fZ;
			ptex[0].s = fU0;	ptex[0].t = fV0;
			ptex[1].s = fU0;	ptex[1].t = fV1;
			ptex[2].s = fU1;	ptex[2].t = fV1;
			ptex[3].s = fU1;	ptex[3].t = fV0;
			pcol[0] = glcol;	pcol[1] = glcol;	pcol[2] = glcol;	pcol[3] = glcol;
			(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
			(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
			(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );

			// Advance to next char
			pixX0 += pixFontAdvancer;
		}
	}
}

void CDrawPort::PutTextCharExThai( const char *pText, int nLength, PIX pixX0, PIX pixY0, const COLOR colBlend,
									FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const
{
	// Skip drawing if text falls above or below draw port
	if( pixY0 > GetHeight() || pixX0 > GetWidth() )
		return;

	// Check if string even exists
	INDEX	ctMaxChars;
	if( nLength == 0 ) ctMaxChars = strlen( pText );
	else ctMaxChars = nLength;
	if( ctMaxChars <= 0 )
		return;

	GFXColor	glcol( colBlend );
	GFXColor	glShadow( colShadow );
	const int	nTextureCount = _pUIFontTexMgr->GetFontTextureCount();
	const PIX	pixFontWidth = _pUIFontTexMgr->GetFontWidth();
	const PIX	pixFontWidth2Byte = _pUIFontTexMgr->GetFontWidth2Byte();
	const PIX	pixFontUVOffset = _pUIFontTexMgr->GetFontWidth2Byte();
	const PIX	pixFontHeight = _pUIFontTexMgr->GetFontHeight();
		  PIX	pixFontAdvancer = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	const PIX	pixFontAdvancer2Byte = pixFontWidth2Byte + _pUIFontTexMgr->GetFontSpacing();

	int			nU, nV;
	FLOAT		fU0, fV0, fU1, fV1, fInvU, fInvV,thai_fV1;
	PIX			pixX1, pixY1 , thai_Y0;
	PIX			pixStartX = pixX0;
	PIX			pixStartY = pixY0;
	
	_pUIFontTexMgr->GetInvUV( fInvU, fInvV );
	// Loop thru chars
	for( INDEX iChar = 0; iChar < ctMaxChars; iChar++ )
	{
		// Get current char
		unsigned char	cCurrent = pText[iChar];
		unsigned char	pre_cCurrent,next_cCurrent;
		int				tv_height=0;
		{
			
			// Texture coordinate
			nU = ( ( cCurrent - 0x1e ) % 21 ) * pixFontUVOffset;
			nV = ( ( cCurrent - 0x1e ) / 21 ) * pixFontUVOffset * 2; // wooss 050929

			pixFontAdvancer = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
			thai_fV1	=	0; // wooss 051001
			thai_Y0		=	0;
			if(cCurrent >= 0xa1 && cCurrent <= 0xfb){
				pixFontAdvancer = _pUIFontTexMgr->GetFontWidthThai(cCurrent) +_pUIFontTexMgr->GetFontSpacing();
				thai_Y0	 = 2;
				thai_fV1 = 4 ; //태국어 V길이를 3 더한다...( 높이가 더 길다...성조 첨가로)
				if(cCurrent == 0xd3){
					if(iChar==0) continue;
					pre_cCurrent = pText[iChar-1];
					if((pre_cCurrent>=0xa1 && pre_cCurrent<=0xcf)||(pre_cCurrent>=0xe7 && pre_cCurrent <= 0xec)){ // pre_cCurrent 050924
						pixX0-=4; // 4픽셀 앞으로
					} else continue; 
					
				}else if(cCurrent == 0xed || cCurrent == 0xd1 || (cCurrent>=0xd4 && cCurrent<=0xda)){		// vowel display //wooss 050924 d3->d4
					if(iChar==0) continue;
					pre_cCurrent = pText[iChar-1];
					if(pre_cCurrent>=0xa0 && pre_cCurrent <= 0xcf){
						pixX0-=7; // 겹치는 모음 출력시 7픽셀 앞으로
						pixFontAdvancer = _pUIFontTexMgr->GetFontWidthThai(cCurrent);
					} else continue;
				} 
				// tone indication display 
				else if( cCurrent>=0xe7 && cCurrent<=0xec) {
					if(iChar==0) continue;
					pre_cCurrent = pText[iChar-1];
					next_cCurrent = pText[iChar+1];
					if(pre_cCurrent == 0xed || (pre_cCurrent == 0xd1) || (pre_cCurrent>=0xd4 && pre_cCurrent<=0xd7)){ // pre_cCurrent 050924
						tv_height=(pixFontHeight/4);
					//	pixY0-=tv_height;
												
					} else if( pre_cCurrent >= 0xd8 && pre_cCurrent <= 0xda) ;
						else if(pre_cCurrent < 0xa1 || pre_cCurrent > 0xcf) continue;// pre_cCurrent 050924
							else if (next_cCurrent == 0xd3) {
								tv_height=(pixFontHeight/4);
								//	pixY0-=tv_height;
							}
					pixX0-=7; // 7픽셀 당겨서 찍는다
					pixFontAdvancer -= _pUIFontTexMgr->GetFontSpacing(); 
				}
				
			}

			fU0 = nU * fInvU;
			fV0 = nV * fInvV;
		
			fU1 = ( nU + pixFontWidth2Byte ) * fInvU;
			fV1 = ( nV + pixFontHeight +thai_fV1) * fInvV;

			pixX1 = pixX0 + pixFontWidth2Byte;
						
			pixY0 = pixStartY - thai_Y0 -tv_height;
			pixY1 = pixY0 + pixFontHeight+thai_fV1;

//			fU1 = ( nU + pixFontWidth2Byte ) * fInvU;
//			fV1 = ( nV + pixFontHeight) * fInvV;
//
//			pixX1 = pixX0 + pixFontWidth2Byte;
//			
//			pixY0 = pixStartY - thai_Y0 - tv_height;
//			pixY1 = pixY0 + pixFontHeight;
			// Shadow
			if( bShadow )
			{
				// Prepare arrays
				INDEX		iStart = _avtxText[0].Count();
				GFXVertex	*pvtx = _avtxText[0].Push( 4 );
				GFXTexCoord	*ptex = _atexText[0].Push( 4 );
				GFXColor	*pcol = _acolText[0].Push( 4 );
				UWORD		*pelm = _auwText[0].Push( 6 );

				// Add char
				pvtx[0].x = pvtx[1].x = pixX0 + 1;
				pvtx[2].x = pvtx[3].x = pixX1 + 1;
				pvtx[0].y = pvtx[3].y = pixY0 + 1;
				pvtx[1].y = pvtx[2].y = pixY1 + 1;
				pvtx[0].z = pvtx[1].z = pvtx[2].z = pvtx[3].z = fZ;
				ptex[0].s = fU0;	ptex[0].t = fV0;
				ptex[1].s = fU0;	ptex[1].t = fV1;
				ptex[2].s = fU1;	ptex[2].t = fV1;
				ptex[3].s = fU1;	ptex[3].t = fV0;
				pcol[0] = glShadow;	pcol[1] = glShadow;	pcol[2] = glShadow;	pcol[3] = glShadow;
				(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
				(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
				(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );
			}

			// Prepare arrays
			INDEX		iStart = _avtxText[0].Count();
			GFXVertex	*pvtx = _avtxText[0].Push( 4 );
			GFXTexCoord	*ptex = _atexText[0].Push( 4 );
			GFXColor	*pcol = _acolText[0].Push( 4 );
			UWORD		*pelm = _auwText[0].Push( 6 );

			// Add char
			pvtx[0].x = pixX0;	pvtx[0].y = pixY0;	pvtx[0].z = fZ;
			pvtx[1].x = pixX0;	pvtx[1].y = pixY1;	pvtx[1].z = fZ;
			pvtx[2].x = pixX1;	pvtx[2].y = pixY1;	pvtx[2].z = fZ;
			pvtx[3].x = pixX1;	pvtx[3].y = pixY0;	pvtx[3].z = fZ;
			ptex[0].s = fU0;	ptex[0].t = fV0;
			ptex[1].s = fU0;	ptex[1].t = fV1;
			ptex[2].s = fU1;	ptex[2].t = fV1;
			ptex[3].s = fU1;	ptex[3].t = fV0;
			pcol[0] = glcol;	pcol[1] = glcol;	pcol[2] = glcol;	pcol[3] = glcol;
			(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
			(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
			(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );

			// Advance to next char
//			if(cCurrent >= 0x41 && cCurrent <= 0x5a) pixX0 += pixFontWidth2Byte-2;
//			else if (cCurrent >= 0x80) pixX0 += pixFontAdvancer;
			pixX0 += pixFontAdvancer;
			pixY0+=tv_height;
			
		}
	}
	
}


void CDrawPort::PutTextCharExJap( const char *pText, int nLength, PIX pixX0, PIX pixY0, const COLOR colBlend,
									FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const
{
	// Skip drawing if text falls above or below draw port
	if( pixY0 > GetHeight() || pixX0 > GetWidth() )
		return;

	// Check if string even exists
	INDEX	ctMaxChars;
	if( nLength == 0 ) ctMaxChars = strlen( pText );
	else ctMaxChars = nLength;
	if( ctMaxChars <= 0 )
		return;

	GFXColor	glcol( colBlend );
	GFXColor	glShadow( colShadow );
	const int	nTextureCount = _pUIFontTexMgr->GetFontTextureCount();
	const PIX	pixFontWidth = _pUIFontTexMgr->GetFontWidth();
	const PIX	pixFontWidth2Byte = _pUIFontTexMgr->GetFontWidth2Byte();
	const PIX	pixFontUVOffset = _pUIFontTexMgr->GetFontWidth2Byte();
	const PIX	pixFontHeight = _pUIFontTexMgr->GetFontHeight();
	const PIX	pixFontAdvancer = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	const PIX	pixFontAdvancer2Byte = pixFontWidth2Byte + _pUIFontTexMgr->GetFontSpacing();

	int			nSequentialCode, nTextureID, nU, nV;
	FLOAT		fU0, fV0, fU1, fV1, fInvU, fInvV;
	PIX			pixX1, pixY1;
	PIX			pixStartX = pixX0;

	_pUIFontTexMgr->GetInvUV( fInvU, fInvV );

		// wooss 051102 
	// pageWords - 이전 페이지의 글자 누적수
	// markStart - 현재 페이지의 시작 위치(코드페이지상)
//	int pageWords[9]	=	{189,652,803,930,0,0,1023,1117}; // 81 - alphabet_num 82 ~ 89 start pos 
	int pageWords[8]	=	{0xbd,0x160,0x1e7,0x276,0x276,0x276,0x2d3,0x331}; // 81 - alphabet_num 82 ~ 89 start pos 
	int markStart[9]	=	{0x40,0x4f,0x40,0x40,0,0,0x40,0x9f,0x40}; 
	int markEnd[9]		=	{0xfc,0xf1,0xd6,0xbe,0,0,0x9c,0xfc,0xfc};
	//		End - Start +1 	 0xbd,0xa3,0x97,0x7f,0,0,0x5d,0x5e
	
	// Loop thru chars
	for( INDEX iChar = 0; iChar < ctMaxChars; iChar++ )
	{
		// Get current char
		unsigned char	cCurrent = pText[iChar];

		// If at end of current line
		if( cCurrent == '\n' || cCurrent == '\r' )
		{
			continue;
		}
		else if( cCurrent == ' ' )
		{
			// Advance to next char
			pixX0 += pixFontAdvancer;
			continue;
		}

		if(cCurrent >= 0xa1 && cCurrent <= 0xdf){
			// Texture coordinate
			nU = ( ( cCurrent - 0x1e - 0x21) % 42 ) * pixFontUVOffset; // 0x21 : 가타가나 중간의 31문자칸을 빼준다. wooss 051028
			nV = ( ( cCurrent - 0x1e - 0x21) / 42 ) * pixFontUVOffset; // 0x1e : texture상에서 00~1D까지는 출력이 안되므로....
			fU0 = nU * fInvU;										   
			fV0 = nV * fInvV;
			fU1 = ( nU + pixFontWidth2Byte ) * fInvU;
			fV1 = ( nV + pixFontHeight ) * fInvV;

			pixX1 = pixX0 + pixFontWidth2Byte;
			pixY1 = pixY0 + pixFontHeight;

			// Shadow
			if( bShadow )
			{
				// Prepare arrays
				INDEX		iStart = _avtxText[0].Count();
				GFXVertex	*pvtx = _avtxText[0].Push( 4 );
				GFXTexCoord	*ptex = _atexText[0].Push( 4 );
				GFXColor	*pcol = _acolText[0].Push( 4 );
				UWORD		*pelm = _auwText[0].Push( 6 );

				// Add char
				pvtx[0].x = pvtx[1].x = pixX0 + 1;
				pvtx[2].x = pvtx[3].x = pixX1 + 1;
				pvtx[0].y = pvtx[3].y = pixY0 + 1;
				pvtx[1].y = pvtx[2].y = pixY1 + 1;
				pvtx[0].z = pvtx[1].z = pvtx[2].z = pvtx[3].z = fZ;
				ptex[0].s = fU0;	ptex[0].t = fV0;
				ptex[1].s = fU0;	ptex[1].t = fV1;
				ptex[2].s = fU1;	ptex[2].t = fV1;
				ptex[3].s = fU1;	ptex[3].t = fV0;
				pcol[0] = glShadow;	pcol[1] = glShadow;	pcol[2] = glShadow;	pcol[3] = glShadow;
				(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
				(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
				(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );
			}

			// Prepare arrays
			INDEX		iStart = _avtxText[0].Count();
			GFXVertex	*pvtx = _avtxText[0].Push( 4 );
			GFXTexCoord	*ptex = _atexText[0].Push( 4 );
			GFXColor	*pcol = _acolText[0].Push( 4 );
			UWORD		*pelm = _auwText[0].Push( 6 );

			// Add char
			pvtx[0].x = pixX0;	pvtx[0].y = pixY0;	pvtx[0].z = fZ;
			pvtx[1].x = pixX0;	pvtx[1].y = pixY1;	pvtx[1].z = fZ;
			pvtx[2].x = pixX1;	pvtx[2].y = pixY1;	pvtx[2].z = fZ;
			pvtx[3].x = pixX1;	pvtx[3].y = pixY0;	pvtx[3].z = fZ;
			ptex[0].s = fU0;	ptex[0].t = fV0;
			ptex[1].s = fU0;	ptex[1].t = fV1;
			ptex[2].s = fU1;	ptex[2].t = fV1;
			ptex[3].s = fU1;	ptex[3].t = fV0;
			pcol[0] = glcol;	pcol[1] = glcol;	pcol[2] = glcol;	pcol[3] = glcol;
			(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
			(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
			(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );

			// Advance to next char
			pixX0 += pixFontAdvancer;
			
		} else
		if( cCurrent & 0x80 )
		{
			nSequentialCode = ASCII_CHARACTER_COUNT+KATA_CHARACTER_COUNT;
			unsigned char	cNext = pText[iChar+1];	
			switch(cCurrent)
			{
				case 0x81:
					nSequentialCode+=cNext-markStart[cCurrent-0x81];
					break;
				case 0x82:
				case 0x83:
				case 0x84:
				case 0x87:
				case 0x88:
				case 0x89:
					nSequentialCode+=cNext-markStart[cCurrent-0x81]+pageWords[cCurrent-0x82];
					break;

				default :
 					if(cCurrent < 0xa0)
						nSequentialCode+=cNext-0x40+pageWords[7]+(cCurrent-0x89)*0xbd;
					else if( cCurrent > 0xdf && cCurrent < 0xeb)
						nSequentialCode+=cNext-0x40+pageWords[7]+(cCurrent-0xc9)*0xbd;
					else if( cCurrent > 0xec && cCurrent < 0xef)
						nSequentialCode+=cNext-0x40+pageWords[7]+(cCurrent-0xca)*0xbd;
					else if( cCurrent > 0xf9 && cCurrent < 0xfd)
						nSequentialCode+=cNext-0x40+pageWords[7]+(cCurrent-0xd5)*0xbd;
					break;
			}
			nTextureID = nSequentialCode / 1764;

			if( nTextureID >= nTextureCount )
				break;

			// Texture coordinate
			nU = ( nSequentialCode % 42 ) * pixFontUVOffset;
			nV = ( ( nSequentialCode % 1764 ) / 42 ) * pixFontUVOffset;
			fU0 = nU * fInvU;
			fV0 = nV * fInvV;
			fU1 = ( nU + pixFontWidth2Byte ) * fInvU;
			fV1 = ( nV + pixFontHeight ) * fInvV;

			pixX1 = pixX0 + pixFontWidth2Byte;
			pixY1 = pixY0 + pixFontHeight;

			// Shadow
			if( bShadow )
			{
				// Prepare arrays
				INDEX		iStart = _avtxText[nTextureID].Count();
				GFXVertex	*pvtx = _avtxText[nTextureID].Push( 4 );
				GFXTexCoord	*ptex = _atexText[nTextureID].Push( 4 );
				GFXColor	*pcol = _acolText[nTextureID].Push( 4 );
				UWORD		*pelm = _auwText[nTextureID].Push( 6 );

				// Add char
				pvtx[0].x = pvtx[1].x = pixX0 + 1;
				pvtx[2].x = pvtx[3].x = pixX1 + 1;
				pvtx[0].y = pvtx[3].y = pixY0 + 1;
				pvtx[1].y = pvtx[2].y = pixY1 + 1;
				pvtx[0].z = pvtx[1].z = pvtx[2].z = pvtx[3].z = fZ;
				ptex[0].s = fU0;	ptex[0].t = fV0;
				ptex[1].s = fU0;	ptex[1].t = fV1;
				ptex[2].s = fU1;	ptex[2].t = fV1;
				ptex[3].s = fU1;	ptex[3].t = fV0;
				pcol[0] = glShadow;	pcol[1] = glShadow;	pcol[2] = glShadow;	pcol[3] = glShadow;
				(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
				(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
				(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );
			}

			// Prepare arrays
			INDEX		iStart = _avtxText[nTextureID].Count();
			GFXVertex	*pvtx = _avtxText[nTextureID].Push( 4 );
			GFXTexCoord	*ptex = _atexText[nTextureID].Push( 4 );
			GFXColor	*pcol = _acolText[nTextureID].Push( 4 );
			UWORD		*pelm = _auwText[nTextureID].Push( 6 );

			// Add char
			pvtx[0].x = pixX0;	pvtx[0].y = pixY0;	pvtx[0].z = fZ;
			pvtx[1].x = pixX0;	pvtx[1].y = pixY1;	pvtx[1].z = fZ;
			pvtx[2].x = pixX1;	pvtx[2].y = pixY1;	pvtx[2].z = fZ;
			pvtx[3].x = pixX1;	pvtx[3].y = pixY0;	pvtx[3].z = fZ;
			ptex[0].s = fU0;	ptex[0].t = fV0;
			ptex[1].s = fU0;	ptex[1].t = fV1;
			ptex[2].s = fU1;	ptex[2].t = fV1;
			ptex[3].s = fU1;	ptex[3].t = fV0;
			pcol[0] = glcol;	pcol[1] = glcol;	pcol[2] = glcol;	pcol[3] = glcol;
			(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
			(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
			(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );

			// Advance to next char
			pixX0 += pixFontAdvancer2Byte;
			iChar++;
		}
		else {
			// Texture coordinate
			nU = ( ( cCurrent - 0x1e ) % 42 ) * pixFontUVOffset;
			nV = ( ( cCurrent - 0x1e ) / 42 ) * pixFontUVOffset;
			fU0 = nU * fInvU;
			fV0 = nV * fInvV;
			fU1 = ( nU + pixFontWidth ) * fInvU;
			fV1 = ( nV + pixFontHeight ) * fInvV;

			pixX1 = pixX0 + pixFontWidth;
			pixY1 = pixY0 + pixFontHeight;

			// Shadow
			if( bShadow )
			{
				// Prepare arrays
				INDEX		iStart = _avtxText[0].Count();
				GFXVertex	*pvtx = _avtxText[0].Push( 4 );
				GFXTexCoord	*ptex = _atexText[0].Push( 4 );
				GFXColor	*pcol = _acolText[0].Push( 4 );
				UWORD		*pelm = _auwText[0].Push( 6 );

				// Add char
				pvtx[0].x = pvtx[1].x = pixX0 + 1;
				pvtx[2].x = pvtx[3].x = pixX1 + 1;
				pvtx[0].y = pvtx[3].y = pixY0 + 1;
				pvtx[1].y = pvtx[2].y = pixY1 + 1;
				pvtx[0].z = pvtx[1].z = pvtx[2].z = pvtx[3].z = fZ;
				ptex[0].s = fU0;	ptex[0].t = fV0;
				ptex[1].s = fU0;	ptex[1].t = fV1;
				ptex[2].s = fU1;	ptex[2].t = fV1;
				ptex[3].s = fU1;	ptex[3].t = fV0;
				pcol[0] = glShadow;	pcol[1] = glShadow;	pcol[2] = glShadow;	pcol[3] = glShadow;
				(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
				(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
				(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );
			}

			// Prepare arrays
			INDEX		iStart = _avtxText[0].Count();
			GFXVertex	*pvtx = _avtxText[0].Push( 4 );
			GFXTexCoord	*ptex = _atexText[0].Push( 4 );
			GFXColor	*pcol = _acolText[0].Push( 4 );
			UWORD		*pelm = _auwText[0].Push( 6 );

			// Add char
			pvtx[0].x = pixX0;	pvtx[0].y = pixY0;	pvtx[0].z = fZ;
			pvtx[1].x = pixX0;	pvtx[1].y = pixY1;	pvtx[1].z = fZ;
			pvtx[2].x = pixX1;	pvtx[2].y = pixY1;	pvtx[2].z = fZ;
			pvtx[3].x = pixX1;	pvtx[3].y = pixY0;	pvtx[3].z = fZ;
			ptex[0].s = fU0;	ptex[0].t = fV0;
			ptex[1].s = fU0;	ptex[1].t = fV1;
			ptex[2].s = fU1;	ptex[2].t = fV1;
			ptex[3].s = fU1;	ptex[3].t = fV0;
			pcol[0] = glcol;	pcol[1] = glcol;	pcol[2] = glcol;	pcol[3] = glcol;
			(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
			(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
			(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );

			// Advance to next char
			pixX0 += pixFontAdvancer;
		}
	}		
	
}

//------------------------------------------------------------------------------
// CDrawPort::PutTextCharExBrz
// Explain: display Brzil font
// Date : 2006-11-02 eons
//------------------------------------------------------------------------------
void CDrawPort::PutTextCharExBrz(const char *pText, int nLength, PIX pixX0, PIX pixY0, const COLOR colBlend,
								FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const
{
	// Skip drawing if text falls above or below draw port
	if( pixY0 > GetHeight() || pixX0 > GetWidth() )
		return;

	// Check if string even exists
	INDEX	ctMaxChars;
	if( nLength == 0 ) ctMaxChars = strlen( pText );
	else ctMaxChars = nLength;
	if( ctMaxChars <= 0 )
		return;

	GFXColor	glcol( colBlend );
	GFXColor	glShadow( colShadow );
	const int	nTextureCount = _pUIFontTexMgr->GetFontTextureCount();
	const PIX	pixFontWidth = _pUIFontTexMgr->GetFontWidth();
	const PIX	pixFontWidth2Byte = _pUIFontTexMgr->GetFontWidth2Byte();
	const PIX	pixFontUVOffset = _pUIFontTexMgr->GetFontWidth2Byte();
	const PIX	pixFontHeight = _pUIFontTexMgr->GetFontHeight();
		  PIX	pixFontAdvancer = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	const PIX	pixFontAdvancer2Byte = pixFontWidth2Byte + _pUIFontTexMgr->GetFontSpacing();

	int			nU, nV;
	FLOAT		fU0, fV0, fU1, fV1, fInvU, fInvV;
	PIX			pixX1, pixY1;
	PIX			pixStartX = pixX0;
	PIX			pixStartY = pixY0;
	
	_pUIFontTexMgr->GetInvUV( fInvU, fInvV );
	// Loop thru chars
	for( INDEX iChar = 0; iChar < ctMaxChars; iChar++ )
	{
		// Get current char
		unsigned char	cCurrent = pText[iChar];
//		unsigned char	pre_cCurrent,next_cCurrent;

		{
			
			// Texture coordinate
			nU = ( ( cCurrent - 0x1e ) % GBK_COUNT ) * pixFontUVOffset;
			nV = ( ( cCurrent - 0x1e ) / GBK_COUNT ) * pixFontUVOffset * 2; // wooss 050929

			fU0 = nU * fInvU;
			fV0 = nV * fInvV;
		
			fU1 = ( nU + pixFontWidth2Byte ) * fInvU;
			fV1 = ( nV + pixFontHeight + cBrzFontOffsetV ) * fInvV;

			pixX1 = pixX0 + pixFontWidth2Byte;
			pixY1 = pixY0 + pixFontHeight + cBrzFontOffsetV;

			// Shadow
			if( bShadow )
			{
				// Prepare arrays
				INDEX		iStart = _avtxText[0].Count();
				GFXVertex	*pvtx = _avtxText[0].Push( 4 );
				GFXTexCoord	*ptex = _atexText[0].Push( 4 );
				GFXColor	*pcol = _acolText[0].Push( 4 );
				UWORD		*pelm = _auwText[0].Push( 6 );

				// Add char
				pvtx[0].x = pvtx[1].x = pixX0 + 1;
				pvtx[2].x = pvtx[3].x = pixX1 + 1;
				pvtx[0].y = pvtx[3].y = pixY0 + 1;
				pvtx[1].y = pvtx[2].y = pixY1 + 1;
				pvtx[0].z = pvtx[1].z = pvtx[2].z = pvtx[3].z = fZ;
				ptex[0].s = fU0;	ptex[0].t = fV0;
				ptex[1].s = fU0;	ptex[1].t = fV1;
				ptex[2].s = fU1;	ptex[2].t = fV1;
				ptex[3].s = fU1;	ptex[3].t = fV0;
				pcol[0] = glShadow;	pcol[1] = glShadow;	pcol[2] = glShadow;	pcol[3] = glShadow;
				(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
				(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
				(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );
			}

			// Prepare arrays
			INDEX		iStart = _avtxText[0].Count();
			GFXVertex	*pvtx = _avtxText[0].Push( 4 );
			GFXTexCoord	*ptex = _atexText[0].Push( 4 );
			GFXColor	*pcol = _acolText[0].Push( 4 );
			UWORD		*pelm = _auwText[0].Push( 6 );

			// Add char
			pvtx[0].x = pixX0;	pvtx[0].y = pixY0;	pvtx[0].z = fZ;
			pvtx[1].x = pixX0;	pvtx[1].y = pixY1;	pvtx[1].z = fZ;
			pvtx[2].x = pixX1;	pvtx[2].y = pixY1;	pvtx[2].z = fZ;
			pvtx[3].x = pixX1;	pvtx[3].y = pixY0;	pvtx[3].z = fZ;
			ptex[0].s = fU0;	ptex[0].t = fV0;
			ptex[1].s = fU0;	ptex[1].t = fV1;
			ptex[2].s = fU1;	ptex[2].t = fV1;
			ptex[3].s = fU1;	ptex[3].t = fV0;
			pcol[0] = glcol;	pcol[1] = glcol;	pcol[2] = glcol;	pcol[3] = glcol;
			(ULONG&)pelm[0] = ( ( iStart + 2 ) << 16 ) | ( iStart + 1 );
			(ULONG&)pelm[2] = ( ( iStart + 0 ) << 16 ) | ( iStart + 0 );
			(ULONG&)pelm[4] = ( ( iStart + 3 ) << 16 ) | ( iStart + 2 );

			// Advance to next char
			pixX0 += pixFontAdvancer;
		}
	}
}

//------------------------------------------------------------------------------
// CDrawPort::PutTextCharExRus
// Explain: display Russia font
// Date : 
//------------------------------------------------------------------------------
void CDrawPort::PutTextCharExRus(const char *pText, int nLength, PIX pixX0, PIX pixY0, const COLOR colBlend,
								FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const
{
	CTString strText = pText;
	CDrawPort *pD = (CDrawPort *)this;
	pD->PutTextExRus(strText, pixX0, pixY0, colBlend, fZ, bShadow, colShadow);
//	pD->PutTextRus(strText, pixX0, pixY0, colBlend);
}

void CDrawPort::PutTextExCX( const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend,
								FLOAT fZ, BOOL bShadow, const COLOR colShadow )
{
	PIX pixOffset = GetTextWidth2(strText) / 2;
	PutTextEx( strText, pixX0 - pixOffset, pixY0, colBlend, fZ, bShadow, colShadow );
}

void CDrawPort::PutTextExRX( const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend,
								FLOAT fZ, BOOL bShadow, const COLOR colShadow )
{
	PIX pixOffset = GetTextWidth2(strText) - 1;
	PutTextEx( strText, pixX0 - pixOffset, pixY0, colBlend, fZ, bShadow, colShadow );
}

void CDrawPort::PutTextCharExCX( const char *pText, int nLength, PIX pixX0, PIX pixY0, const COLOR colBlend,
									FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const
{
	if (nLength == 0)
		nLength = strlen(pText);
#if defined (G_THAI)
	PIX pixOffset = FindThaiLen(pText) / 2;
#elif defined (G_RUSSA) 
	PIX pixOffset = GetTextWidth(pText) / 2;
#elif defined (G_MAL) || defined (G_HONGKONG) 
	PIX pixOffset = nLength * (_pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing()) / 2;
	if (g_bIsMalEng)
		pixOffset = nLength * (_pUIFontTexMgr->GetFontWidth()) / 2;
#elif defined (G_USA) || defined (G_GERMAN)  || defined (G_EUROPE3) || defined (G_EUROPE2)
	PIX pixOffset = nLength * (_pUIFontTexMgr->GetFontWidth()) / 2;
#else
	PIX pixOffset = nLength * (_pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing()) / 2;
#endif 
	PutTextCharEx( pText, nLength, pixX0 - pixOffset, pixY0, colBlend, fZ, bShadow, colShadow );
}

void CDrawPort::PutTextCharExRX( const char *pText, int nLength, PIX pixX0, PIX pixY0, const COLOR colBlend,
									FLOAT fZ, BOOL bShadow, const COLOR colShadow )
{
	if (nLength == 0)
		nLength = strlen(pText);
#if defined (G_THAI)
	PIX pixOffset = FindThaiLen(pText) - 1;
#elif defined (G_RUSSIA)
	PIX pixOffset = GetTextWidth(pText) - 1;
#else
	PIX pixOffset = nLength * (_pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing()) - 1;
#endif
	PutTextCharEx( pText, nLength, pixX0 - pixOffset, pixY0, colBlend, fZ, bShadow, colShadow );
}

void CDrawPort::EndTextEx( BOOL bDepthTest )
{
	// Check API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE );

#if defined G_RUSSIA
	CTextureData *ptd = (CTextureData*)dp_FontData->fd_ptdTextureData;
	InitTextureData( ptd, FALSE, 203, bDepthTest );
#endif

	// Render all elements
	for( INDEX iText = 0; iText < FONT_MAX; iText++ )
	{
		INDEX ctElements = _auwText[iText].Count();
		if( ctElements > 0 )
		{

#if !defined G_RUSSIA
			_pUIFontTexMgr->InitTexture( iText, bDepthTest );
#endif
			gfxFlushTextElements( ctElements, iText );			
			_avtxText[iText].PopAll();
			_atexText[iText].PopAll();
			_acolText[iText].PopAll();
			_auwText[iText].PopAll();
		}
	}
}

void CDrawPort::AddBtnTexture( const int nTexID, const FLOAT fI0, const FLOAT fJ0, const FLOAT fI1, const FLOAT fJ1, 
								const FLOAT fU0, const FLOAT fV0, const FLOAT fU1, const FLOAT fV1,
								const COLOR col ) const
{
	TRACKMEM(Mem, "Gfx");
	const GFXColor glCol(col);
	const INDEX iStart = _avtxBtn[nTexID].Count();
	GFXVertex   *pvtx = _avtxBtn[nTexID].Push(4);
	GFXTexCoord *ptex = _atexBtn[nTexID].Push(4);
	GFXColor    *pcol = _acolBtn[nTexID].Push(4);
	UWORD       *pelm = _auwBtn[nTexID].Push(6);
	pvtx[0].x = fI0;  pvtx[0].y = fJ0;  pvtx[0].z = 0;
	pvtx[1].x = fI0;  pvtx[1].y = fJ1;  pvtx[1].z = 0;
	pvtx[2].x = fI1;  pvtx[2].y = fJ1;  pvtx[2].z = 0;
	pvtx[3].x = fI1;  pvtx[3].y = fJ0;  pvtx[3].z = 0;
	ptex[0].s = fU0;  ptex[0].t = fV0;
	ptex[1].s = fU0;  ptex[1].t = fV1;
	ptex[2].s = fU1;  ptex[2].t = fV1;
	ptex[3].s = fU1;  ptex[3].t = fV0;
	pcol[0] = glCol;
	pcol[1] = glCol;
	pcol[2] = glCol;
	pcol[3] = glCol;
	(ULONG&)pelm[0] = ((iStart+2)<<16) | (iStart+1);
	(ULONG&)pelm[2] = ((iStart+0)<<16) | (iStart+0);
	(ULONG&)pelm[4] = ((iStart+3)<<16) | (iStart+2);
}

void CDrawPort::AddBtnTexture( const int nTexID, const FLOAT fI0, const FLOAT fJ0, const FLOAT fI1, const FLOAT fJ1, 
								const UIRectUV uv, const COLOR col ) const
{
	AddBtnTexture(nTexID, fI0, fJ0, fI1, fJ1, uv.U0, uv.V0, uv.U1, uv.V1, col);
}

void CDrawPort::FlushBtnRenderingQueue( int nBtnType, const ULONG ulPBT )
{
	if(nBtnType == UBET_GUILD_MARK || nBtnType >= UBET_TYPE_MAX)
		return;

	// Check API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE );

	// Render all elements
	for( INDEX iBtn = 0; iBtn < MAX_BTN; iBtn++ )
	{
		INDEX ctElements = _auwBtn[iBtn].Count();
		if( ctElements > 0 )
		{
			_pUIBtnTexMgr->InitTexture( nBtnType, iBtn, ulPBT );
			gfxFlushBtnElements( ctElements, iBtn );

			_avtxBtn[iBtn].PopAll();
			_atexBtn[iBtn].PopAll();
			_acolBtn[iBtn].PopAll();
			_auwBtn[iBtn].PopAll();
		}
	}
}
// yjpark     -->|


/**********************************************************
 * Routines for putting and getting textures strictly in 2D
 */

void CDrawPort::PutTexture( class CTextureObject *pTO, const PIXaabbox2D &boxScreen,
														const COLOR colBlend/*=0xFFFFFFFF*/, 
														const ULONG ulPBT/*=PBT_BLEND*/) const
{
	PutTexture( pTO, boxScreen, colBlend, colBlend, colBlend, colBlend, ulPBT);
}

void CDrawPort::PutTexture( class CTextureObject *pTO, const PIXaabbox2D &boxScreen,
														const MEXaabbox2D &boxTexture, const COLOR colBlend/*=0xFFFFFFFF*/,
														const ULONG ulPBT/*=PBT_BLEND*/) const
{
	PutTexture( pTO, boxScreen, boxTexture, colBlend, colBlend, colBlend, colBlend, ulPBT);
}

void CDrawPort::PutTexture( class CTextureObject *pTO, const PIXaabbox2D &boxScreen,
														const COLOR colUL, const COLOR colUR, const COLOR colDL, const COLOR colDR,
														const ULONG ulPBT/*=PBT_BLEND*/) const
{
	MEXaabbox2D boxTexture( MEX2D(0,0), MEX2D(pTO->GetWidth(), pTO->GetHeight()));
	PutTexture( pTO, boxScreen, boxTexture, colUL, colUR, colDL, colDR, ulPBT);
}

// complete put texture routine
void CDrawPort::PutTexture( class CTextureObject *pTO,
														const PIXaabbox2D &boxScreen, const MEXaabbox2D &boxTexture,
														const COLOR colUL, const COLOR colUR, const COLOR colDL, const COLOR colDR,
														const ULONG ulPBT/*=PBT_BLEND*/) const
{
	// extract screen and texture coordinates
	const PIX pixI0 = boxScreen.Min()(1);  const PIX pixI1 = boxScreen.Max()(1);
	const PIX pixJ0 = boxScreen.Min()(2);  const PIX pixJ1 = boxScreen.Max()(2);

	// if whole texture is out of drawport, skip it (just to reduce OpenGL call overhead)
	if( pixI0>GetWidth() || pixJ0>GetHeight() || pixI1<0 || pixJ1<0) return;

	// check API and adjust position for D3D by half pixel
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);
	FLOAT fI0 = pixI0;  FLOAT fI1 = pixI1;
	FLOAT fJ0 = pixJ0;  FLOAT fJ1 = pixJ1;

	// prepare texture
	gfxSetTextureWrapping( GFX_REPEAT, GFX_REPEAT);
	CTextureData *ptd = (CTextureData*)pTO->GetData();
	ptd->SetAsCurrent(pTO->GetFrame());

	// setup rendering mode and prepare arrays
	const enum PredefinedBlendType pbt = (const enum PredefinedBlendType)ulPBT;
	gfxDisableDepthTest();
	gfxSetBlendType(pbt);
	gfxResetArrays();
	GFXVertex   *pvtx = _avtxCommon.Push(4);
	GFXTexCoord *ptex = _atexCommon[0].Push(4);
	GFXColor    *pcol = _acolCommon.Push(4);

	// extract texture coordinates and apply correction factor
	const PIX pixWidth  = ptd->GetPixWidth();
	const PIX pixHeight = ptd->GetPixHeight();
	FLOAT fCorrectionU, fCorrectionV;
	(SLONG&)fCorrectionU = (127-FastLog2(pixWidth))  <<23; // fCorrectionU = 1.0f / ptd->GetPixWidth() 
	(SLONG&)fCorrectionV = (127-FastLog2(pixHeight)) <<23; // fCorrectionV = 1.0f / ptd->GetPixHeight() 
	FLOAT fU0 = (boxTexture.Min()(1)>>ptd->td_iFirstMipLevel) *fCorrectionU;
	FLOAT fU1 = (boxTexture.Max()(1)>>ptd->td_iFirstMipLevel) *fCorrectionU;
	FLOAT fV0 = (boxTexture.Min()(2)>>ptd->td_iFirstMipLevel) *fCorrectionV;
	FLOAT fV1 = (boxTexture.Max()(2)>>ptd->td_iFirstMipLevel) *fCorrectionV;

	// if not tiled
	const BOOL bTiled = Abs(fU0-fU1)>1 || Abs(fV0-fV1)>1;
	if( !bTiled) {
		// slight adjust for sub-pixel precision
		fU0 += +0.25f *fCorrectionU;
		fU1 += -0.25f *fCorrectionU;
		fV0 += +0.25f *fCorrectionV;
		fV1 += -0.25f *fCorrectionV;
	}
	// prepare colors
	const GFXColor glcolUL(colUL);
	const GFXColor glcolUR(colUR);
	const GFXColor glcolDL(colDL);
	const GFXColor glcolDR(colDR);

	// prepare coordinates of the rectangle
	pvtx[0].x = fI0;  pvtx[0].y = fJ0;  pvtx[0].z = 0;
	pvtx[1].x = fI0;  pvtx[1].y = fJ1;  pvtx[1].z = 0;
	pvtx[2].x = fI1;  pvtx[2].y = fJ1;  pvtx[2].z = 0;
	pvtx[3].x = fI1;  pvtx[3].y = fJ0;  pvtx[3].z = 0;
	ptex[0].s = fU0;  ptex[0].t = fV0;
	ptex[1].s = fU0;  ptex[1].t = fV1;
	ptex[2].s = fU1;  ptex[2].t = fV1;
	ptex[3].s = fU1;  ptex[3].t = fV0;
	pcol[0] = glcolUL;
	pcol[1] = glcolDL;
	pcol[2] = glcolDR;
	pcol[3] = glcolUR;
	gfxFlushQuads();
}



// prepares texture and rendering arrays
void CDrawPort::InitTexture( class CTextureObject *pTO, const BOOL bClamp/*=FALSE*/,
														 const ULONG ulPBT/*=PBT_BLEND*/, const BOOL bDepthTest/*= FALSE*/) const
{
	// prepare
	if( pTO!=NULL) {
		// has texture
		CTextureData *ptd = (CTextureData*)pTO->GetData();
		GfxWrap eWrap = GFX_REPEAT;
		if( bClamp) eWrap = GFX_CLAMP;
		gfxSetTextureWrapping( eWrap, eWrap);
		ptd->SetAsCurrent(pTO->GetFrame());
	} else {
		// no texture
		gfxDisableTexture();
	}
	// setup rendering mode and prepare arrays
	const enum PredefinedBlendType pbt = (const enum PredefinedBlendType)ulPBT;
	gfxSetBlendType(pbt);

	if( bDepthTest )
	{
		gfxEnableDepthWrite();
		gfxEnableDepthTest();
	}
	else
		gfxDisableDepthTest();

	gfxResetArrays();
}

void CDrawPort::InitTextureData( class CTextureData *pTD, const BOOL bClamp, const ULONG ulPBT,
									const BOOL bDepthTest ) const
{
	if( pTD != NULL )
	{
		GfxWrap	eWrap = GFX_REPEAT;
		if( bClamp) eWrap = GFX_CLAMP;
		gfxSetTextureWrapping( eWrap, eWrap );
		pTD->SetAsCurrent();
#if defined (G_RUSSIA)
		CDrawPort *pD = (CDrawPort *)this;
		pD->StockCurrentTextureData(pTD);
#endif	// end g_russia
	}
	else
		gfxDisableTexture();

	const enum PredefinedBlendType	pbt = (const enum PredefinedBlendType)ulPBT;
	gfxSetBlendType(pbt);

	if( bDepthTest )
	{
		gfxEnableDepthWrite();
		gfxEnableDepthTest();
	}
	else
		gfxDisableDepthTest();

	gfxResetArrays();
}


// adds one full texture to rendering queue
void CDrawPort::AddTexture( const FLOAT fI0, const FLOAT fJ0, const FLOAT fI1, const FLOAT fJ1, const COLOR col) const
{
	TRACKMEM(Mem, "Gfx");
	const GFXColor glCol(col);
	const INDEX iStart = _avtxCommon.Count();
	GFXVertex   *pvtx = _avtxCommon.Push(4);
	GFXTexCoord *ptex = _atexCommon[0].Push(4);
	GFXColor    *pcol = _acolCommon.Push(4);
	UWORD       *pelm = _auwCommonElements.Push(6);
	pvtx[0].x = fI0;  pvtx[0].y = fJ0;  pvtx[0].z = 0;
	pvtx[1].x = fI0;  pvtx[1].y = fJ1;  pvtx[1].z = 0;
	pvtx[2].x = fI1;  pvtx[2].y = fJ1;  pvtx[2].z = 0;
	pvtx[3].x = fI1;  pvtx[3].y = fJ0;  pvtx[3].z = 0;
	ptex[0].s = 0;    ptex[0].t = 0;
	ptex[1].s = 0;    ptex[1].t = 1;
	ptex[2].s = 1;    ptex[2].t = 1;
	ptex[3].s = 1;    ptex[3].t = 0;
	pcol[0] = glCol;  pcol[1] = glCol;  pcol[2] = glCol;  pcol[3] = glCol;
	(ULONG&)pelm[0] = ((iStart+2)<<16) | (iStart+1);
	(ULONG&)pelm[2] = ((iStart+0)<<16) | (iStart+0);
	(ULONG&)pelm[4] = ((iStart+3)<<16) | (iStart+2);
}


// adds one part of texture to rendering queue
void CDrawPort::AddTexture( const FLOAT fI0, const FLOAT fJ0, const FLOAT fI1, const FLOAT fJ1, 
							const FLOAT fU0, const FLOAT fV0, const FLOAT fU1, const FLOAT fV1,
							const COLOR col, FLOAT fZ ) const
{
	TRACKMEM(Mem, "Gfx");
	const GFXColor glCol(col);
	const INDEX iStart = _avtxCommon.Count();
	GFXVertex   *pvtx = _avtxCommon.Push(4);
	GFXTexCoord *ptex = _atexCommon[0].Push(4);
	GFXColor    *pcol = _acolCommon.Push(4);
	UWORD       *pelm = _auwCommonElements.Push(6);
	pvtx[0].x = fI0;  pvtx[0].y = fJ0;  pvtx[0].z = fZ;
	pvtx[1].x = fI0;  pvtx[1].y = fJ1;  pvtx[1].z = fZ;
	pvtx[2].x = fI1;  pvtx[2].y = fJ1;  pvtx[2].z = fZ;
	pvtx[3].x = fI1;  pvtx[3].y = fJ0;  pvtx[3].z = fZ;
	ptex[0].s = fU0;  ptex[0].t = fV0;
	ptex[1].s = fU0;  ptex[1].t = fV1;
	ptex[2].s = fU1;  ptex[2].t = fV1;
	ptex[3].s = fU1;  ptex[3].t = fV0;
	pcol[0] = glCol;
	pcol[1] = glCol;
	pcol[2] = glCol;
	pcol[3] = glCol;
	(ULONG&)pelm[0] = ((iStart+2)<<16) | (iStart+1);
	(ULONG&)pelm[2] = ((iStart+0)<<16) | (iStart+0);
	(ULONG&)pelm[4] = ((iStart+3)<<16) | (iStart+2);
}

void CDrawPort::AddTexture( const FLOAT fI0, const FLOAT fJ0, const FLOAT fI1, const FLOAT fJ1, 
							const UIRectUV uv, const COLOR col, FLOAT fZ /*= 0 */ ) const
{
	AddTexture(fI0, fJ0, fI1, fJ1, uv.U0, uv.V0, uv.U1, uv.V1, col, fZ);
}

// adds one triangle to rendering queue
void CDrawPort::AddTriangle( const FLOAT fI0, const FLOAT fJ0,
							 const FLOAT fI1, const FLOAT fJ1,
							 const FLOAT fI2, const FLOAT fJ2, const COLOR col, const float fz) const
{
	const GFXColor glCol(col);
	const INDEX iStart = _avtxCommon.Count();
	GFXVertex   *pvtx = _avtxCommon.Push(3);
	GFXTexCoord *ptex = _atexCommon[0].Push(3);
	GFXColor    *pcol = _acolCommon.Push(3);
	UWORD       *pelm = _auwCommonElements.Push(3);
	pvtx[0].x = fI0;  pvtx[0].y = fJ0;  pvtx[0].z = fz;
	pvtx[1].x = fI1;  pvtx[1].y = fJ1;  pvtx[1].z = fz;
	pvtx[2].x = fI2;  pvtx[2].y = fJ2;  pvtx[2].z = fz;
	pcol[0] = glCol;
	pcol[1] = glCol;
	pcol[2] = glCol;
	(ULONG&)pelm[0] = ((iStart+1)<<16) | (iStart+0);
	pelm[2] = iStart+2;
}

void CDrawPort::AddQuadrangle( const FLOAT fI0, const FLOAT fJ0, const FLOAT fI1, const FLOAT fJ1, const COLOR col, const FLOAT fz /*= 0 */ ) const
{
	const GFXColor glCol(col);
	const INDEX iStart = _avtxCommon.Count();
	GFXVertex   *pvtx = _avtxCommon.Push(4);
	GFXTexCoord *ptex = _atexCommon[0].Push(4);
	GFXColor    *pcol = _acolCommon.Push(4);
	UWORD       *pelm = _auwCommonElements.Push(6);

	// left, top
	pvtx[0].x = fI0;  pvtx[0].y = fJ0;  pvtx[0].z = fz;

	// left, bottom
	pvtx[1].x = fI0;  pvtx[1].y = fJ1;  pvtx[1].z = fz;

	// right, bottom
	pvtx[2].x = fI1;  pvtx[2].y = fJ1;  pvtx[2].z = fz;

	// right, top
	pvtx[3].x = fI1;  pvtx[3].y = fJ0;  pvtx[3].z = fz;

	pcol[0] = glCol;
	pcol[1] = glCol;
	pcol[2] = glCol;
	pcol[3] = glCol;

	(ULONG&)pelm[0] = ((iStart+2)<<16) | (iStart+1);
	(ULONG&)pelm[2] = ((iStart+0)<<16) | (iStart+0);
	(ULONG&)pelm[4] = ((iStart+3)<<16) | (iStart+2);
}

// adds one textured quad (up-left start, counter-clockwise)
void CDrawPort::AddTexture( const FLOAT fI0, const FLOAT fJ0, const FLOAT fU0, const FLOAT fV0, const COLOR col0,
							const FLOAT fI1, const FLOAT fJ1, const FLOAT fU1, const FLOAT fV1, const COLOR col1,
							const FLOAT fI2, const FLOAT fJ2, const FLOAT fU2, const FLOAT fV2, const COLOR col2,
							const FLOAT fI3, const FLOAT fJ3, const FLOAT fU3, const FLOAT fV3, const COLOR col3) const
{
	TRACKMEM(Mem, "Gfx");
	const GFXColor glCol0(col0);
	const GFXColor glCol1(col1);
	const GFXColor glCol2(col2);
	const GFXColor glCol3(col3);
	const INDEX iStart = _avtxCommon.Count();
	GFXVertex   *pvtx = _avtxCommon.Push(4);
	GFXTexCoord *ptex = _atexCommon[0].Push(4);
	GFXColor    *pcol = _acolCommon.Push(4);
	UWORD       *pelm = _auwCommonElements.Push(6);
	pvtx[0].x = fI0;  pvtx[0].y = fJ0;  pvtx[0].z = 0;
	pvtx[1].x = fI1;  pvtx[1].y = fJ1;  pvtx[1].z = 0;
	pvtx[2].x = fI2;  pvtx[2].y = fJ2;  pvtx[2].z = 0;
	pvtx[3].x = fI3;  pvtx[3].y = fJ3;  pvtx[3].z = 0;
	ptex[0].s = fU0;  ptex[0].t = fV0;
	ptex[1].s = fU1;  ptex[1].t = fV1;
	ptex[2].s = fU2;  ptex[2].t = fV2;
	ptex[3].s = fU3;  ptex[3].t = fV3;
	pcol[0] = glCol0;
	pcol[1] = glCol1;
	pcol[2] = glCol2;
	pcol[3] = glCol3;
	(ULONG&)pelm[0] = ((iStart+2)<<16) | (iStart+1);
	(ULONG&)pelm[2] = ((iStart+0)<<16) | (iStart+0);
	(ULONG&)pelm[4] = ((iStart+3)<<16) | (iStart+2);
}

void CDrawPort::AddTexture( const FLOAT fI0, const FLOAT fJ0, const FLOAT fI1, const FLOAT fJ1, 
							const FLOAT fU0, const FLOAT fV0, const FLOAT fU1, const FLOAT fV1,
							const FLOAT fU2, const FLOAT fV2, const FLOAT fU3, const FLOAT fV3, const COLOR col ) const
{
	TRACKMEM(Mem, "Gfx");
	const GFXColor glCol(col);
	const INDEX iStart = _avtxCommon.Count();
	GFXVertex   *pvtx = _avtxCommon.Push(4);
	GFXTexCoord *ptex = _atexCommon[0].Push(4);
	GFXColor    *pcol = _acolCommon.Push(4);
	UWORD       *pelm = _auwCommonElements.Push(6);
	pvtx[0].x = fI0;  pvtx[0].y = fJ0;  pvtx[0].z = 0;
	pvtx[1].x = fI0;  pvtx[1].y = fJ1;  pvtx[1].z = 0;
	pvtx[2].x = fI1;  pvtx[2].y = fJ1;  pvtx[2].z = 0;
	pvtx[3].x = fI1;  pvtx[3].y = fJ0;  pvtx[3].z = 0;
	ptex[0].s = fU0;  ptex[0].t = fV0;
	ptex[1].s = fU1;  ptex[1].t = fV1;
	ptex[2].s = fU2;  ptex[2].t = fV2;
	ptex[3].s = fU3;  ptex[3].t = fV3;
	pcol[0] = glCol;
	pcol[1] = glCol;
	pcol[2] = glCol;
	pcol[3] = glCol;
	(ULONG&)pelm[0] = ((iStart+2)<<16) | (iStart+1);
	(ULONG&)pelm[2] = ((iStart+0)<<16) | (iStart+0);
	(ULONG&)pelm[4] = ((iStart+3)<<16) | (iStart+2);
}

// renders all textures from rendering queue and flushed rendering arrays
void CDrawPort::FlushRenderingQueue(void) const
{ 
	gfxFlushElements(); 
	gfxResetArrays(); 
}



// blends screen with accumulation color
void CDrawPort::BlendScreen(void)
{
	if( dp_ulBlendingA==0) return;

	ULONG fix1oA = 65536 / dp_ulBlendingA;
	ULONG ulRA = (dp_ulBlendingRA*fix1oA)>>16;
	ULONG ulGA = (dp_ulBlendingGA*fix1oA)>>16;
	ULONG ulBA = (dp_ulBlendingBA*fix1oA)>>16;
	ULONG ulA  = ClampUp( dp_ulBlendingA, 255UL);
	COLOR colBlending = RGBAToColor( ulRA,ulGA,ulBA, ulA);
																		
	// blend drawport (thru z-buffer because of elimination of pixel artefacts)
	gfxEnableDepthTest();
	gfxDisableDepthWrite();
	gfxEnableBlend();
	gfxBlendFunc( GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
	gfxDisableAlphaTest();
	gfxDisableTexture();
	// prepare color
	GFXColor glcol(colBlending);

	// set arrays
	gfxResetArrays();
	GFXVertex   *pvtx = _avtxCommon.Push(4);
	GFXTexCoord *ptex = _atexCommon[0].Push(4);
	GFXColor    *pcol = _acolCommon.Push(4);
	const INDEX iW = GetWidth();
	const INDEX iH = GetHeight();
	pvtx[0].x =  0;  pvtx[0].y =  0;  pvtx[0].z = 0.01f;
	pvtx[1].x =  0;  pvtx[1].y = iH;  pvtx[1].z = 0.01f;
	pvtx[2].x = iW;  pvtx[2].y = iH;  pvtx[2].z = 0.01f;
	pvtx[3].x = iW;  pvtx[3].y =  0;  pvtx[3].z = 0.01f;
	pcol[0] = glcol;
	pcol[1] = glcol;
	pcol[2] = glcol;
	pcol[3] = glcol;
	gfxFlushQuads();
	// reset accumulation color
	dp_ulBlendingRA = 0;
	dp_ulBlendingGA = 0;
	dp_ulBlendingBA = 0;
	dp_ulBlendingA  = 0;
}

int CDrawPort::GetTextSectionWidth(char* strString, int nSectionEnd, BOOL bIsPassWordBox)
{
	if(nSectionEnd > strlen(strString))
		return -1;

	std::string strText = "";

	if(bIsPassWordBox)
	{
		for(int i=0; i<nSectionEnd; i++)
		{
			strText += "*";
		}
	}
	else
	{
		strText = strString;
	}

	extern CFontData *_pfdDefaultFont;

	if(dp_FontData != _pfdDefaultFont)
		SetFont(_pfdDefaultFont);

	return GetTextWidth(strText.c_str());
}

int CDrawPort::CheckShowCharLength(char* strString, int nWidth)
{
	int nStrLength = strlen(strString);
	int nStrWidth = GetTextSectionWidth(strString, nStrLength, FALSE);
	if(nStrWidth <= nWidth)
		return nStrLength;

	for(int i=nStrLength; i >= 0; i--)
	{
		if(nStrWidth - GetTextSectionWidth(&strString[i], strlen(&strString[i]), FALSE) <= nWidth )
		{
			return i;
		}
	}

	return nStrLength;
}

void CDrawPort::PutTextRus(const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend)
{
	CTextureData* curTexture = stCurrentTextureData;
	FlushRenderingQueue();
	PutText(strText, pixX0, pixY0, colBlend);
	InitTextureData(curTexture);
}
