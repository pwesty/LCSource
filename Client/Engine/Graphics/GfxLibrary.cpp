#include "stdh.h"

#include <Engine/Graphics/GfxLibrary.h>

#include <Engine/Base/Translation.h>
#include <Engine/Base/ErrorReporting.h>
#include <Engine/Base/Console.h>
#include <Engine/Base/Shell.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Base/Statistics_internal.h>
#include <Engine/Base/ListIterator.inl>
#include <Engine/Math/Functions.h>
#include <Engine/Math/AABBox.h>
#include <Engine/Models/Normals.h>
#include <Engine/Sound/SoundLibrary.h>
#include <Engine/Templates/Stock_CModelData.h>
#include <Engine/Templates/Stock_CShader.h>
#include <Engine/Terrain/Terrain.h>

#include <Engine/Graphics/ShadowMap.h>
#include <Engine/Graphics/Texture.h>
#include <Engine/Graphics/Raster.h>
#include <Engine/Graphics/ViewPort.h>
#include <Engine/Graphics/DrawPort.h>
#include <Engine/Graphics/Font.h>
#include <Engine/Graphics/MultiMonitor.h>

#include <Engine/Templates/DynamicStackArray.h>
#include <Engine/Templates/DynamicStackArray.cpp>
#include <Engine/Templates/DynamicContainer.cpp>
#include <Engine/Templates/Stock_CTextureData.h>
#include <Engine/Templates/Stock_CMesh.h>
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#include <Engine/Effect/EffectCommon.h>
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
#include <Engine/Network/Web.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIOption.h>

extern cWeb g_web;
extern HWND	_hwndMain;
//	김영환 전역 설정 값 가져오기
extern BOOL _bClientApp;

// control for partial usage of compiled vertex arrays
extern BOOL CVA_b2D     = FALSE;
extern BOOL CVA_bWorld  = FALSE;
extern BOOL CVA_bModels = FALSE;    

// common element arrays
CStaticStackArray<GFXVertex>    _avtxCommon;      
CStaticStackArray<GFXNormal>    _anorCommon;      
CStaticStackArray<GFXColor>     _acolCommon;      
CStaticStackArray<GFXTexCoord>  _atexCommon[GFX_USETEXUNITS];   
CStaticStackArray<UWORD>    _auwCommonQuads;   // predefined array for rendering quads thru triangles in glDrawElements()
CStaticStackArray<UWORD> _auwCommonElements;

// yjpark |<--
// Text element arrays
CStaticStackArray<GFXVertex>	_avtxText[FONT_MAX];
CStaticStackArray<GFXColor>		_acolText[FONT_MAX];
CStaticStackArray<GFXTexCoord>	_atexText[FONT_MAX];
CStaticStackArray<UWORD>		_auwText[FONT_MAX];

// Button element arrays
CStaticStackArray<GFXVertex>	_avtxBtn[MAX_BTN];
CStaticStackArray<GFXColor>		_acolBtn[MAX_BTN];
CStaticStackArray<GFXTexCoord>	_atexBtn[MAX_BTN];
CStaticStackArray<UWORD>		_auwBtn[MAX_BTN];
// yjpark     -->|

// global texture parameters
CTexParams _tpGlobal[GFX_MAXTEXUNITS];  

// pointer to global graphics library object
extern CGfxLibrary *_pGfx = NULL;

// string for debug output
extern CTString _strDebug="";
extern PIX _pixDebugStringX=0, _pixDebugStringY=0;
extern COLOR _colDebugString=C_WHITE|CT_OPAQUE;

// forced texture upload quality (0 = default, 16 = force 16-bit, 32 = force 32-bit)
extern INDEX _iTexForcedQuality = 0;

extern PIX _fog_pixSizeH;
extern PIX _fog_pixSizeL;
extern PIX _haze_pixSize;

// control for partial usage of compiled vertex arrays
extern BOOL CVA_b2D;
extern BOOL CVA_bWorld;
extern BOOL CVA_bModels;

// gamma control
static FLOAT _fLastBrightness, _fLastContrast, _fLastGamma;
static FLOAT _fLastBiasR, _fLastBiasG, _fLastBiasB;
static INDEX _iLastLevels;
static UWORD _auwGammaTable[256*3];

// tables for occlusion queries
extern UINT  *_puiOcclusionQueryIDs = NULL; 
extern SLONG *_pslOcclusionQueryFrames = NULL; // 0=free, >0=in use, <0=visible
extern ULONG *_pulOcclusionQueryOwners = NULL; // array of void pointers
extern INDEX _ctUsedOcclusionQueries = 0;    

// flag for scene rendering in progress (i.e. between 1st lock in frame & swap-buffers)
static BOOL GFX_bRenderingScene = FALSE;
// ID of the last drawport that has been locked
static ULONG GFX_ulLastDrawPortID = 0;  

// last size of vertex buffers
extern INDEX _iLastVertexBufferSize = 0;
// pretouch flag
extern BOOL _bNeedPretouch;

// system gamma table
extern UWORD _auwSystemGammaTable[256*3] = {0};


// flat texture
extern CTextureData *_ptdFlat = NULL;
static ULONG _ulWhite = 0xFFFFFFFF;


// texture/shadow control
extern INDEX tex_iNormalQuality    = 00;      // 0=optimal, 1=16bit, 2=32bit, 3=compressed (1st num=opaque tex, 2nd=alpha tex)
extern INDEX tex_iAnimationQuality = 11;      // 0=optimal, 1=16bit, 2=32bit, 3=compressed (1st num=opaque tex, 2nd=alpha tex)
extern INDEX tex_iNormalSize     = 9;         // log2 of texture area /2 for max texture size allowed
extern INDEX tex_iAnimationSize  = 7; 
extern INDEX tex_iEffectSize     = 8; 
extern INDEX tex_bDynamicMipmaps = FALSE;     // how many mipmaps will be bilineary filtered (0-15)
extern INDEX tex_iDithering      = 0;         // 0=none, 1-3=low, 4-7=medium, 8-10=high
extern INDEX tex_bFineEffect = FALSE;         // 32bit effect? (works only if base texture hasn't been dithered)
extern INDEX tex_bFineFog = TRUE;             // should fog be 8/32bit? (or just plain 4/16bit)
extern INDEX tex_iFogSize = 7;                // limit fog texture size 
extern INDEX tex_iFiltering = 0;              // -6 - +6; negative = sharpen, positive = blur, 0 = none
extern INDEX tex_iEffectFiltering = +4;       // filtering of fire effect textures
extern INDEX tex_bProgressiveFilter = FALSE;  // filter mipmaps in creation time (not afterwards)
extern INDEX tex_bColorizeMipmaps = FALSE;    // DEBUG: colorize texture's mipmap levels in various colors
extern INDEX tex_bCompressGrayscale     = TRUE;   // compress grayscale textures
extern INDEX tex_bCompressAlphaChannel  = FALSE;  // use interpolated alpha channel for compressed translucent textures
extern INDEX tex_bAlternateCompression  = FALSE;  // compress opaque texture as translucent (this is a fix for GF1/2/3)
extern INDEX tex_bAggressiveCompression = TRUE;   // compress "constant" textures
extern INDEX ter_bShowCharacterShadow = TRUE;	// 지형에 캐릭터 그림자를 그릴것인가?

extern INDEX shd_iStaticSize  = 8;    
extern INDEX shd_iDynamicSize = 8;    
extern INDEX shd_iAllowDynamic = 1;     // 0=disallow, 1=allow on polys w/o 'NoDynamicLights' flag, 2=allow unconditionally
extern FLOAT shd_fCacheSize = 8;        // in megabytes

extern FLOAT shd_tmFlushDelay = 30;     // in seconds (maximum)
extern INDEX shd_bFineQuality = TRUE; 
extern INDEX shd_iFiltering = 3;        // >0 = blurring, 0 = no filtering
extern INDEX shd_iDithering = 1;        // 0=none, 1,2=low, 3,4=medium, 5=high
extern INDEX shd_bDynamicMipmaps = TRUE;
extern INDEX shd_bCacheAll   = FALSE;   // cache all shadowmap at the level loading time (careful - memory eater!)
extern INDEX shd_bAllowFlats = TRUE;    // allow optimization of single-color shadowmaps
extern INDEX shd_iForceFlats = 0;       // force all shadowmaps to be flat (internal!) - 0=don't, 1=w/o overbrighting, 2=w/ overbrighting
extern INDEX shd_bShowFlats  = FALSE;   // colorize flat shadows
extern INDEX shd_bColorize   = FALSE;   // colorize shadows by size (gradieng from red=big to green=little)


// OpenGL control
extern INDEX ogl_iTextureCompressionType  = 1;    // 0=none, 1=default (ARB), 2=S3TC, 3=FXT1, 4=old S3TC
extern INDEX ogl_bUseCompiledVertexArrays = 101;  // =XYZ; X=2D, Y=world, Z=models
extern INDEX ogl_bAllowQuadArrays = FALSE;
extern INDEX ogl_bExclusive = TRUE;
extern INDEX ogl_bGrabDepthBuffer = FALSE;
extern INDEX ogl_iMaxBurstSize = 0;        // unlimited
extern INDEX ogl_bTruformLinearNormals = TRUE;
extern INDEX ogl_bAlternateClipPlane = FALSE; // signal when user clip plane requires a texture unit
extern INDEX ogl_iFinish = 1;              // 0=never, 1=before rendering of next frame, 2=at the end of this frame, 3=at projection change
extern INDEX ogl_bHardFinish = FALSE;      // read from z-buffer after finish to be sure everything has been rendered

// Direct3D control
extern INDEX d3d_bUseHardwareTnL = TRUE;
extern INDEX d3d_bAlternateDepthReads = FALSE;  // should check delayed depth reads at the end of current frame (FALSE) or at begining of the next (TRUE)
extern INDEX d3d_iVertexBuffersSize   = 512;    // KBs reserved for vertex buffers
extern INDEX d3d_iVertexRangeTreshold = 99;     // minimum vertices in buffer that triggers range optimization
extern INDEX d3d_bFastUpload = TRUE;            // use internal format conversion routines
extern INDEX d3d_iMaxBurstSize = 0;             // 0=unlimited
extern INDEX d3d_iFinish = 0;


// API common controls
extern INDEX gap_iUseTextureUnits = 4;
extern INDEX gap_iTextureFiltering  = 22;       // bilinear by default
extern INDEX gap_iTextureAnisotropy = 2;        // 1=isotropic, 2=min anisotropy
extern FLOAT gap_fTextureLODBias    = -0.2f;
extern INDEX gap_bAggressiveAnisotropy = FALSE; // use anisotropic filtering for single-mipmap textures
extern INDEX gap_bOptimizeStateChanges = TRUE;
extern INDEX gap_iOptimizeDepthReads = 1;       // 0=imediately, 1=after frame, 2=every 0.1 seconds
extern INDEX gap_iOptimizeClipping   = 2;       // 0=no, 1=mirror plane only, 2=mirror and view frustum
extern INDEX gap_bAllowGrayTextures = TRUE;
extern INDEX gap_bUseOcclusionCulling = 11;     // =XY; X=lens-flares, Y=models (if not disabled)
extern INDEX gap_iSwapInterval = 0;
extern INDEX gap_iRefreshRate  = 0;
extern INDEX gap_iDithering = 2;        // 16-bit dithering: 0=none, 1=no alpha, 2=all
extern INDEX gap_bForceTruform = 0;     // 0 = only for models that allow truform, 1=for every model
extern INDEX gap_iTruformLevel = 3;     // 0 = no tesselation
extern INDEX gap_iDepthBits = 0;        // 0 (as color depth), 16, 24 or 32

// models control
extern INDEX mdl_bRenderDetail     = TRUE;
extern INDEX mdl_bRenderSpecular   = TRUE;
extern INDEX mdl_bRenderReflection = TRUE;
//안태훈 수정 시작	//(Modify Worldbase Overbright to NonOver)(0.1)
extern INDEX mdl_bAllowOverbright  = TRUE;
//안태훈 수정 끝	//(Modify Worldbase Overbright to NonOver)(0.1)
extern INDEX mdl_iShadowQuality = 1;
extern FLOAT mdl_fLODMul = 1.0f;
extern FLOAT mdl_fLODAdd = 0.0f;
extern INDEX mdl_iLODDisappear = 1; // 0=never, 1=ignore bias, 2=with bias
extern INDEX mdl_bShowOccludees = FALSE;
extern INDEX mdl_bShowTriangles = FALSE;
extern INDEX mdl_bShowStrips    = FALSE;
extern INDEX mdl_bCreateStrips  = TRUE;
extern INDEX mdl_bTruformWeapons = FALSE;

// ska controls
ENGINE_API extern FLOAT ska_fLODMul = 1.0f;
ENGINE_API extern FLOAT ska_fLODAdd = 0.0f;
extern INDEX ska_bShowSkeleton     = FALSE;
extern INDEX ska_bShowCollision    = FALSE;
extern INDEX ska_bShowAllFramesBox = FALSE;
extern INDEX ska_bShowActiveBones  = FALSE;
extern INDEX ska_bRenderMesh       = TRUE;  // for profiling
extern INDEX ska_bAnimateMesh      = TRUE;  // for profiling
extern INDEX ska_bTransformMesh    = TRUE;  // for profiling
extern INDEX ska_bMorphMesh        = TRUE;  // for profiling
extern INDEX ska_bCalcBoneTransf   = TRUE;  // for profiling
extern INDEX ska_iMaxWeightsPerVertex  = 4;
extern INDEX ska_bUseHardwareShaders   = TRUE;
extern INDEX ska_bAllowBonesAdjustment = TRUE;
extern INDEX ska_bReplaceSmcWithBmc    = FALSE;
//안태훈 수정 시작	//(Easy Use World Editor)(0.1)
extern INDEX ska_bShowTriangles		   = FALSE;
extern INDEX g_bShowTerrain            = TRUE;
extern INDEX g_bShowBSP                = TRUE;
extern INDEX g_bShowModel              = TRUE;
extern INDEX g_bShowWater              = TRUE;
extern INDEX g_iShowReflectionMap		   = 0;
extern INDEX g_bReflection			   = TRUE;
//안태훈 수정 끝	//(Easy Use World Editor)(0.1)
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
extern INDEX ska_bShowSkaTag		   = FALSE;
extern INDEX ska_bShowAttachedTag      = FALSE;
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
extern INDEX login_bSkipSelect		   = FALSE;

// shader controls 
extern INDEX sha_bColorizeShaders  = FALSE;
extern INDEX sha_iOptimizeShaders  = 2;
extern INDEX sha_iResetConstRegisters = 0;
extern INDEX dbg_bNormalizeShaderNormals = TRUE;

// terrain controls
extern INDEX ter_bUseHardwareShaders = TRUE;
extern INDEX ter_bUseTopMap          = TRUE;
extern INDEX ter_bUpdateTopMapOnEdit = FALSE;
extern INDEX ter_bLerpVertices       = TRUE;  // not active yet
extern INDEX ter_bOptimizeRendering  = TRUE;  // not active yet
extern INDEX ter_bRenderTerrain      = TRUE;  // for profiling
extern INDEX ter_bUpdateTerrain      = TRUE;  // for profiling
extern INDEX ter_bShowQuadTree       = FALSE; // for debuging
extern INDEX ter_iShowQuadTreeLev    = 0;     // for debuging
extern INDEX ter_bShowVertices       = FALSE; // for debuging
extern INDEX ter_bShowLerpTargets    = FALSE; // for debuging
extern INDEX ter_bDisableViewer      = FALSE; // for debuging
extern INDEX ter_bShowTriangles      = FALSE; // for debuging
extern INDEX ter_iRecordRayCasting   = FALSE; // for debuging
extern INDEX ter_iAttributeRange	 = 40;		//Terrain에 그려질 Attribute맵의 크기를 설정 :Su-won





// rendering control
extern INDEX wld_bAlwaysAddAll         = FALSE;
extern INDEX wld_bRenderMirrors        = TRUE;
extern INDEX wld_bRenderEmptyBrushes   = TRUE;
extern INDEX wld_bRenderShadowMaps     = TRUE;
extern INDEX wld_bRenderTextures       = TRUE;
extern INDEX wld_bRenderDetailPolygons = TRUE;
extern INDEX wld_bTextureLayers        = 111;
extern INDEX wld_bShowTriangles        = FALSE;
extern INDEX wld_bShowDetailTextures   = FALSE;
extern INDEX wld_iDetailRemovingBias   = 3;
extern FLOAT wld_fEdgeOffsetI          = 0.0f; //0.125f;
extern FLOAT wld_fEdgeAdjustK          = 1.0f; //1.0001f;
extern INDEX wld_bExcludeEditorModels = FALSE; // exclude editor models from worlds
										
extern INDEX gfx_bRenderView       = TRUE;
extern INDEX gfx_bRenderWorld      = TRUE;
extern INDEX gfx_bRenderParticles  = TRUE;
extern INDEX gfx_bRenderModels     = TRUE;
extern INDEX gfx_bRenderFog        = TRUE;
extern INDEX gfx_bRenderReflection = FALSE;
extern INDEX gfx_iLensFlareQuality = 3;   // 0=none, 1=corona only, 2=corona and reflections, 3=corona, reflections and glare 

extern INDEX gfx_bDecoratedText   = TRUE;
extern INDEX gfx_bClearScreen = FALSE;
extern FLOAT gfx_tmProbeDecay = 30.0f;   // seconds
extern INDEX gfx_iProbeSize   = 256;     // in KBs

extern INDEX gfx_ctMonitors = 0;
extern INDEX gfx_bMultiMonDisabled = FALSE;
extern INDEX gfx_bDisableMultiMonSupport = TRUE;
extern INDEX gfx_bSeparateTripleHead = FALSE;   // render 3-views instead of one for triplehead modes
extern INDEX gfx_bDisableWindowsKeys = TRUE;

extern INDEX wed_bIgnoreTJunctions = FALSE;
extern INDEX wed_bUseBaseForReplacement = FALSE;

extern INDEX dbg_bShowFreeMemory = FALSE;
FLOAT _tmLastPeak = 0.0f;
extern INDEX dbg_bShowDeletedObjectsFromStock = FALSE;
// gamma table control
extern FLOAT gfx_fBrightness = 0.0f;    // -0.9 - 0.9
extern FLOAT gfx_fContrast   = 1.0f;    //  0.1 - 1.9
extern FLOAT gfx_fGamma      = 1.0f;    //  0.1 - 9.0
extern FLOAT gfx_fBiasR  = 1.0f;        //  0.0 - 1.0
extern FLOAT gfx_fBiasG  = 1.0f;        //  0.0 - 1.0
extern FLOAT gfx_fBiasB  = 1.0f;        //  0.0 - 1.0
extern INDEX gfx_iLevels = 256;         //    2 - 256

// stereo rendering control
extern INDEX gfx_iStereo = 0;                  // 0=off, 1=red/cyan
extern INDEX gfx_bStereoInvert = FALSE;        // is left eye RED or CYAN
extern INDEX gfx_iStereoOffset = 10;           // view offset (or something:)
extern FLOAT gfx_fStereoSeparation =  0.25f;   // distance between eyes

// 'supported' console variable flags
static INDEX sys_bHasTextureCompression = 0;
static INDEX sys_bHasTextureAnisotropy = 0;
static INDEX sys_bHasAdjustableGamma = 0;
static INDEX sys_bHasTextureLODBias = 0;
static INDEX sys_bHasMultitexturing = 0;
static INDEX sys_bHas32bitTextures = 0;
static INDEX sys_bHasSwapInterval = 0;
static INDEX sys_bHasHardwareTnL = 1;
static INDEX sys_bHasTruform = 0;
static INDEX sys_bHasCVAs = 0;
static INDEX sys_bUsingOpenGL = 0;
extern INDEX sys_bUsingDirect3D = 0;


/*
 * Low level hook flags
 */
#define WH_KEYBOARD_LL 13

#if	!defined(WINDOW_SDK_70A)
#define LLKHF_EXTENDED 0x00000001
#define LLKHF_INJECTED 0x00000010
#define LLKHF_ALTDOWN  0x00000020
#define LLKHF_UP       0x00000080

#define LLMHF_INJECTED 0x00000001


/*
 * Structure used by WH_KEYBOARD_LL
 */
typedef struct tagKBDLLHOOKSTRUCT {
		DWORD   vkCode;
		DWORD   scanCode;
		DWORD   flags;
		DWORD   time;
		DWORD   dwExtraInfo;
} KBDLLHOOKSTRUCT, FAR *LPKBDLLHOOKSTRUCT, *PKBDLLHOOKSTRUCT;

#endif	// !defined(WINDOW_SDK_70A)

static HHOOK _hLLKeyHook = NULL;

LRESULT CALLBACK LowLevelKeyboardProc (INT nCode, WPARAM wParam, LPARAM lParam)
{
	// By returning a non-zero value from the hook procedure, the
	// message does not get passed to the target window
	KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *) lParam;
	BOOL bControlKeyDown = 0;

	switch (nCode)
	{
			case HC_ACTION:
			{
					// Check to see if the CTRL key is pressed
					bControlKeyDown = GetAsyncKeyState (VK_CONTROL) >> ((sizeof(SHORT) * 8) - 1);
					
					// Disable CTRL+ESC
					if (pkbhs->vkCode == VK_ESCAPE && bControlKeyDown)
							return 1;

					// Disable ALT+TAB
					if (pkbhs->vkCode == VK_TAB && pkbhs->flags & LLKHF_ALTDOWN)
							return 1;

					// Disable ALT+ESC
					if (pkbhs->vkCode == VK_ESCAPE && pkbhs->flags & LLKHF_ALTDOWN)
							return 1;

					break;
			}

			default:
					break;
	}
	return CallNextHookEx (_hLLKeyHook, nCode, wParam, lParam);
} 

void DisableWindowsKeys(void)
{
	//if( _hLLKeyHook!=NULL) UnhookWindowsHookEx(_hLLKeyHook);
	//_hLLKeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, &LowLevelKeyboardProc, NULL, GetCurrentThreadId());

	INDEX iDummy;
	SystemParametersInfo(SPI_SETSCREENSAVERRUNNING, TRUE, &iDummy, 0);
}

void EnableWindowsKeys(void)
{
	INDEX iDummy;
	SystemParametersInfo(SPI_SETSCREENSAVERRUNNING, FALSE, &iDummy, 0);
	// if( _hLLKeyHook!=NULL) UnhookWindowsHookEx(_hLLKeyHook);
}


// texture size reporting

static CTString ReportQuality( INDEX iQuality)
{
	if( iQuality==0) return "optimal";
	if( iQuality==1) return "16-bit";
	if( iQuality==2) return "32-bit";
	if( iQuality==3) return "compressed";
	ASSERTALWAYS( "Invalid texture quality.");
	return "?";
}


static void TexturesInfo(void)
{
	UpdateTextureSettings();
	INDEX ctNo04O=0, ctNo64O=0, ctNoMXO=0;
	PIX   pixK04O=0, pixK64O=0, pixKMXO=0;
	SLONG slKB04O=0, slKB64O=0, slKBMXO=0;
	INDEX ctNo04A=0, ctNo64A=0, ctNoMXA=0;
	PIX   pixK04A=0, pixK64A=0, pixKMXA=0;
	SLONG slKB04A=0, slKB64A=0, slKBMXA=0;

	// walk thru all textures on stock
	{FOREACHINDYNAMICCONTAINER( _pTextureStock->st_ctObjects, CTextureData, ittd)
	{ // get texture info
		CTextureData &td = *ittd;
		BOOL  bAlpha   = td.td_ulFlags&TEX_ALPHACHANNEL;
		INDEX ctFrames = td.td_ctFrames;
		SLONG slBytes  = td.GetUsedMemory();
		ASSERT( slBytes>=0);
		// get texture size
		PIX pixTextureSize = td.GetPixWidth() * td.GetPixHeight();
		PIX pixMipmapSize  = pixTextureSize;
		if( td.td_ctFineMipLevels>1) pixMipmapSize = pixMipmapSize *4/3;
		// increase corresponding counters
		if( pixTextureSize<4096) {
			if( bAlpha) { pixK04A+=pixMipmapSize;  slKB04A+=slBytes;  ctNo04A+=ctFrames; }
			else        { pixK04O+=pixMipmapSize;  slKB04O+=slBytes;  ctNo04O+=ctFrames; }
		} else if( pixTextureSize<=65536) {
			if( bAlpha) { pixK64A+=pixMipmapSize;  slKB64A+=slBytes;  ctNo64A+=ctFrames; }
			else        { pixK64O+=pixMipmapSize;  slKB64O+=slBytes;  ctNo64O+=ctFrames; }
		} else {
			if( bAlpha) { pixKMXA+=pixMipmapSize;  slKBMXA+=slBytes;  ctNoMXA+=ctFrames; }
			else        { pixKMXO+=pixMipmapSize;  slKBMXO+=slBytes;  ctNoMXO+=ctFrames; }
		}
	}}

	// report
	const PIX pixNormDim = sqrt((double)TS.ts_pixNormSize);
	const PIX pixAnimDim = sqrt((double)TS.ts_pixAnimSize);
	const PIX pixEffDim  = 1L<<tex_iEffectSize;
	CTString strTmp;
	strTmp = tex_bFineEffect ? "32-bit" : "16-bit";
	CPrintF( "\n");
	CPrintF( "Normal-opaque textures quality:         %s\n", ReportQuality(TS.ts_iNormQualityO));
	CPrintF( "Normal-translucent textures quality:    %s\n", ReportQuality(TS.ts_iNormQualityA));
	CPrintF( "Animation-opaque textures quality:      %s\n", ReportQuality(TS.ts_iAnimQualityO));
	CPrintF( "Animation-translucent textures quality: %s\n", ReportQuality(TS.ts_iAnimQualityA));
	CPrintF( "Effect textures quality:                %s\n", strTmp);
	CPrintF( "\n");
	CPrintF( "Max allowed normal texture area size:    %3dx%d\n", pixNormDim, pixNormDim);
	CPrintF( "Max allowed animation texture area size: %3dx%d\n", pixAnimDim, pixAnimDim);
	CPrintF( "Max allowed effect texture area size:    %3dx%d\n", pixEffDim,  pixEffDim);

	CPrintF( "\n");
	CPrintF( "Opaque textures memory usage:\n");
	CPrintF( "     <64 pix: %3d frames use %6.1f Kpix in %5d KB\n", ctNo04O, pixK04O/1024.0f, slKB04O/1024);
	CPrintF( "  64-256 pix: %3d frames use %6.1f Kpix in %5d KB\n", ctNo64O, pixK64O/1024.0f, slKB64O/1024);
	CPrintF( "    >256 pix: %3d frames use %6.1f Kpix in %5d KB\n", ctNoMXO, pixKMXO/1024.0f, slKBMXO/1024);
	CPrintF( "Translucent textures memory usage:\n");
	CPrintF( "     <64 pix: %3d frames use %6.1f Kpix in %5d KB\n", ctNo04A, pixK04A/1024.0f, slKB04A/1024);
	CPrintF( "  64-256 pix: %3d frames use %6.1f Kpix in %5d KB\n", ctNo64A, pixK64A/1024.0f, slKB64A/1024);
	CPrintF( "    >256 pix: %3d frames use %6.1f Kpix in %5d KB\n", ctNoMXA, pixKMXA/1024.0f, slKBMXA/1024);
	CPrintF("\n- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n\n");
}



// reformat an extensions string to cross multiple lines
extern CTString ReformatExtensionsString( CTString strUnformatted)
{
	CTString strTmp, strDst = "\n";
	char *pcSrc = (char*)(const char*)strUnformatted;
	FOREVER {
		char *pcSpace = strchr( pcSrc, ' ');
		if( pcSpace==NULL) break;
		*pcSpace = 0;
		strTmp.PrintF( "    %s\n", pcSrc);
		strDst += strTmp;
		*pcSpace = ' ';
		pcSrc = pcSpace +1;
	}
	if(strDst=="\n") {
		strDst = "none\n";
	}
	// done
	return strDst;
}



// printout OpenGL extensions list
static void ListOpenGLExtensions(void)
{
	CPrintF("\n");
	CPrintF("- Published extensions: %s", ReformatExtensionsString(_pGfx->go_strExtensions));
	if( _pGfx->go_strWinExtensions != "") CPrintF("%s", ReformatExtensionsString(_pGfx->go_strWinExtensions));
	CPrintF("\n- Supported extensions: %s\n", ReformatExtensionsString(_pGfx->go_strSupportedExtensions));
	CPrintF("\n");
}



// printout extensive OpenGL/Direct3D info to console
static void GAPInfo(void)
{
	// check API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);
	CPrintF( "\n");

	// in case of driver hasn't been initialized yet
	if( (_pGfx->go_hglRC==NULL && _pGfx->gl_pd3dDevice==NULL) || eAPI==GAT_NONE) {
		// be brief, be quick
		CPrintF( TRANS("Display driver hasn't been initialized.\n\n"));
		return;
	}

	// report API
	CPrintF( "- Graphics API: ");
	if( eAPI==GAT_OGL) CPrintF( "OpenGL\n");
	else CPrintF( "Direct3D\n");
	// and number of adapters
	CPrintF( "- Adapters found: %d\n", _pGfx->gl_gaAPI[eAPI].ga_ctAdapters);
	CPrintF( "\n");

	// report renderer
	CDisplayAdapter &da = _pGfx->gl_gaAPI[eAPI].ga_adaAdapter[_pGfx->gl_iCurrentAdapter];
	if( eAPI==GAT_OGL) CPrintF( "- Vendor:   %s\n", da.da_strVendor);
	CPrintF( "- Renderer: %s\n", da.da_strRenderer);
	CPrintF( "- Version:  %s\n", da.da_strVersion);
	CPrintF( "\n");

	// Z-buffer depth
	CPrintF( "- Z-buffer precision: ");
	if( _pGfx->gl_ctDepthBits==0) CPrintF( "default\n");
	else CPrintF( "%d bits\n", _pGfx->gl_ctDepthBits);

	// 32-bit textures
	CPrintF( "- 32-bit textures: ");
	if( _pGfx->gl_ulFlags & GLF_32BITTEXTURES) CPrintF( "supported\n");
	else CPrintF( "not supported\n");
	// grayscale textures
	CPrintF( "- Grayscale textures: ");
	if( gap_bAllowGrayTextures) CPrintF( "allowed\n");
	else CPrintF( "not allowed\n");
	// report maximum texture dimension
	CPrintF( "- Max texture dimension: %d pixels\n", _pGfx->gl_pixMaxTextureDimension);

	// report multitexturing capabilities
	CPrintF( "- Multi-texturing: ");
	if( _pGfx->gl_ctRealTextureUnits<2) CPrintF( "not supported\n");
	else {
		if( gap_iUseTextureUnits>1) CPrintF( "enabled (using %d texture units)\n", gap_iUseTextureUnits);
		else CPrintF( "disabled\n");
		CPrintF( "- Texture units: %d", _pGfx->gl_ctRealTextureUnits);
		if( _pGfx->gl_ctTextureUnits < _pGfx->gl_ctRealTextureUnits)  {
			CPrintF(" (%d can be used)\n", _pGfx->gl_ctTextureUnits); 
		} else CPrintF("\n"); 
	}

	// report texture anisotropy degree
	if( _pGfx->gl_iMaxTextureAnisotropy>=2) {
		CPrintF( "- Texture anisotropy: %d of %d\n", _tpGlobal[0].tp_iAnisotropy, _pGfx->gl_iMaxTextureAnisotropy);
	} else CPrintF( "- Anisotropic texture filtering: not supported\n");

	// report texture LOD bias range
	const FLOAT fMaxLODBias = _pGfx->gl_fMaxTextureLODBias;
	if( fMaxLODBias>0) {
		CPrintF( "- Texture LOD bias: %.1f of +/-%.1f\n", _pGfx->gl_fTextureLODBias, fMaxLODBias);
	} else CPrintF( "- Texture LOD biasing: not supported\n");

	// OpenGL only stuff ...
	if( eAPI==GAT_OGL) 
	{
		// report truform tessellation
		CPrintF( "- Truform tessellation: ");
		if( _pGfx->gl_iMaxTessellationLevel>0) {
			if( _pGfx->gl_iTessellationLevel>0) {
				CPrintF( "enabled ");
				if( gap_bForceTruform) CPrintF( "(for all models)\n");
				else CPrintF( "(only for Truform-ready models)\n");
				CTString strNormalMode = ogl_bTruformLinearNormals ? "linear" : "quadratic";
				CPrintF( "- Tesselation level: %d of %d (%s normals)\n", _pGfx->gl_iTessellationLevel, _pGfx->gl_iMaxTessellationLevel, strNormalMode);
			} else CPrintF( "disabled\n");
		} else CPrintF( "not supported\n");

		// report current swap interval (only if fullscreen)
		if( _pGfx->gl_ulFlags&GLF_FULLSCREEN) {
			// report current swap interval
			CPrintF( "- Swap interval: ");
			if( _pGfx->gl_ulFlags&GLF_VSYNC) {
				GLint gliWaits = pwglGetSwapIntervalEXT();
				if( gliWaits>=0) {
					ASSERT( gliWaits==_pGfx->gl_iSwapInterval);
					CPrintF( "%d frame(s)\n", gliWaits);
				} else CPrintF( "not readable\n");
			} else CPrintF( "not adjustable\n");
		}

		// compiled vertex arrays support
		CPrintF( "- Compiled Vertex Arrays: ");
		if( _pGfx->gl_ulFlags & GLF_EXT_COMPILEDVERTEXARRAY) {
			extern BOOL CVA_b2D;
			extern BOOL CVA_bWorld;
			extern BOOL CVA_bModels;    
			if( ogl_bUseCompiledVertexArrays) {
				CTString strSep="";
				CPrintF( "enabled (for ");
				if( CVA_bWorld)  { CPrintF( "world");               strSep="/"; }
				if( CVA_bModels) { CPrintF( "%smodels",    strSep); strSep="/"; }
				if( CVA_b2D)     { CPrintF( "%sparticles", strSep); }
				CPrintF( ")\n");
			} else CPrintF( "disabled\n");
		} else CPrintF( "not supported\n");

		// report texture compression type
		CPrintF( "- Supported texture compression system(s): ");
		if( !(_pGfx->gl_ulFlags&GLF_TEXTURECOMPRESSION)) CPrintF( "none\n");
		else {
			CTString strSep="";
			if( _pGfx->gl_ulFlags & GLF_EXTC_ARB)    { CPrintF( "ARB");                strSep=", "; }
			if( _pGfx->gl_ulFlags & GLF_EXTC_S3TC)   { CPrintF( "%sS3TC",     strSep); strSep=", "; }
			if( _pGfx->gl_ulFlags & GLF_EXTC_FXT1)   { CPrintF( "%sFXT1",     strSep); strSep=", "; }
			if( _pGfx->gl_ulFlags & GLF_EXTC_LEGACY) { CPrintF( "%sold S3TC", strSep); }
			CPrintF( "\n- Current texture compression system: ");
			switch( ogl_iTextureCompressionType) {
			case 0:   CPrintF( "none\n");         break;
			case 1:   CPrintF( "ARB wrapper\n");  break;
			case 2:   CPrintF( "S3TC\n");         break;
			case 3:   CPrintF( "FXT1\n");         break;
			default:  CPrintF( "old S3TC\n");     break;
			}
		}

		// visibility test support
		CPrintF( "- Occlusion culling: ");
		if( _pGfx->HasOcclusionCulling()) {
			switch( gap_bUseOcclusionCulling) {
			case  0:  CPrintF("disabled\n");                   break;
			case  1:  CPrintF("enabled (for models)\n");       break;
			case 10:  CPrintF("enabled (for lens-flares)\n");  break;
			case 11:  CPrintF("enabled\n");                    break;
			default:  CPrintF("undetermined\n");               break;
			}
		} else CPrintF( "not supported\n");

	}

	// Direct3D only stuff
	if( eAPI==GAT_D3D)
	{
		// HW T&L
		CPrintF( "- Hardware T&L: ");
		if( _pGfx->gl_ulFlags&GLF_D3D_HASHWTNL) {
			if( _pGfx->gl_ctMaxStreams<GFX_MINSTREAMS) CPrintF( "cannot be used\n");
			else if( _pGfx->gl_ulFlags&GLF_D3D_USINGHWTNL) CPrintF( "enabled (%d streams)\n", _pGfx->gl_ctMaxStreams);
			else CPrintF( "disabled\n");
		} else CPrintF( "not present\n");

		// report vtx/idx buffers size
		extern SLONG SizeFromVertices_D3D( INDEX ctVertices);
		const SLONG slMemoryUsed = SizeFromVertices_D3D(_pGfx->gl_ctVertices);
		CPrintF( "- Vertex buffer size: %.1f KB (%d vertices)\n", slMemoryUsed/1024.0f, _pGfx->gl_ctVertices);

		// N-Patches tessellation (Truform)
		CPrintF( "- N-Patches: ");
		if( _pGfx->gl_iMaxTessellationLevel>0) {
			if( !(_pGfx->gl_ulFlags&GLF_D3D_USINGHWTNL)) CPrintF( "not possible with SW T&L\n");
			else if( _pGfx->gl_iTessellationLevel>0) {
				CPrintF( "enabled ");
				if( gap_bForceTruform) CPrintF( "(for all models)\n");
				else CPrintF( "(only for Truform-ready models)\n");
				CPrintF( "- Tesselation level: %d of %d\n", _pGfx->gl_iTessellationLevel, _pGfx->gl_iMaxTessellationLevel);
			} else CPrintF( "disabled\n");
		} else CPrintF( "not supported\n");

		// texture compression
		CPrintF( "- Texture compression: ");
		if( _pGfx->gl_ulFlags&GLF_TEXTURECOMPRESSION) CPrintF( "supported\n");
		else CPrintF( "not supported\n");

		// custom clip plane support
		CPrintF( "- Custom clip plane: ");
		if( _pGfx->gl_ulFlags&GLF_D3D_CLIPPLANE) CPrintF( "supported\n");
		else CPrintF( "not supported\n");

		// color buffer writes enable/disable support
		CPrintF( "- Color masking: ");
		if( _pGfx->gl_ulFlags&GLF_D3D_COLORWRITES) CPrintF( "supported\n");
		else CPrintF( "not supported\n");

		// depth (Z) bias support
		CPrintF( "- Depth biasing: ");
		if( _pGfx->gl_ulFlags&GLF_D3D_ZBIAS) CPrintF( "supported\n");
		else CPrintF( "not supported\n");

		// visibility test support
		CPrintF( "- Occlusion culling: ");
		if( _pGfx->HasOcclusionCulling()) { 
			if( !gap_bUseOcclusionCulling) CPrintF( "disabled\n");
			else CPrintF( "enabled\n");
		} else CPrintF( "not supported\n");

		// current swap interval (only if fullscreen)
		if( _pGfx->gl_ulFlags&GLF_FULLSCREEN) {
			CPrintF( "- Swap interval: ");
			if( _pGfx->gl_ulFlags&GLF_VSYNC) {
				CPrintF( "%d frame(s)\n", _pGfx->gl_iSwapInterval);
			} else CPrintF( "not adjustable\n");
		}
	}
}


// update console system vars
extern void UpdateGfxSysCVars(void)
{
	sys_bHasTextureCompression = 0;
	sys_bHasTextureAnisotropy = 0;
	sys_bHasAdjustableGamma = 0;
	sys_bHasTextureLODBias = 0;
	sys_bHasMultitexturing = 0;
	sys_bHas32bitTextures = 0;
	sys_bHasSwapInterval = 0;
	sys_bHasHardwareTnL = 1;
	sys_bHasTruform = 0;
	sys_bHasCVAs = 1;
	sys_bUsingOpenGL = 0;
	sys_bUsingDirect3D = 0;
	if( _pGfx->gl_iMaxTextureAnisotropy>1) sys_bHasTextureAnisotropy = 1;
	if( _pGfx->gl_fMaxTextureLODBias>0) sys_bHasTextureLODBias = 1;
	if( _pGfx->gl_ctTextureUnits>1) sys_bHasMultitexturing = 1;
	if( _pGfx->gl_iMaxTessellationLevel>0) sys_bHasTruform = 1;
	if( _pGfx->gl_ulFlags & GLF_TEXTURECOMPRESSION) sys_bHasTextureCompression = 1;
	if( _pGfx->gl_ulFlags & GLF_ADJUSTABLEGAMMA) sys_bHasAdjustableGamma = 1;
	if( _pGfx->gl_ulFlags & GLF_32BITTEXTURES) sys_bHas32bitTextures = 1;
	if( _pGfx->gl_ulFlags & GLF_VSYNC) sys_bHasSwapInterval = 1;
	if( _pGfx->gl_eCurrentAPI==GAT_OGL && !(_pGfx->gl_ulFlags&GLF_EXT_COMPILEDVERTEXARRAY)) sys_bHasCVAs = 0;
	if( _pGfx->gl_eCurrentAPI==GAT_D3D && !(_pGfx->gl_ulFlags&GLF_D3D_HASHWTNL)) sys_bHasHardwareTnL = 0;
	if( _pGfx->gl_eCurrentAPI==GAT_OGL) sys_bUsingOpenGL = 1;
	if( _pGfx->gl_eCurrentAPI==GAT_D3D) sys_bUsingDirect3D = 1;
}

	 
	 
// determine whether texture or shadowmap needs probing
extern BOOL ProbeMode( CTimerValue tvLast)
{
	// probing off ?
	if( !_pGfx->gl_bAllowProbing) return FALSE;
	if( gfx_tmProbeDecay<1) {
		gfx_tmProbeDecay = 0;  
		return FALSE;
	}
	// clamp and determine probe mode
	if( gfx_tmProbeDecay>999) gfx_tmProbeDecay = 999;
	CTimerValue tvNow  = _pTimer->GetHighPrecisionTimer();
	const TIME tmDelta = (tvNow-tvLast).GetSeconds();
	if( tmDelta>gfx_tmProbeDecay) return TRUE;
	return FALSE;
}
	 


// uncache all cached shadow maps
extern void UncacheShadows(void)
{
	// mute all sounds
	_pSound->Mute();
					
	CListHead &lhOriginal = _pGfx->gl_lhCachedShadows;
	// while there is some shadow in main list
	while( !lhOriginal.IsEmpty()) {
		CShadowMap &sm = *LIST_HEAD( lhOriginal, CShadowMap, sm_lnInGfx);
		sm.Uncache();
	}
	// mark that we need pretouching
	_bNeedPretouch = TRUE;
}


// refresh (uncache and eventually cache) all cached shadow maps
extern void CacheShadows(void);
static void RecacheShadows(void)
{
	// mute all sounds
	_pSound->Mute();
	UncacheShadows();
	if( shd_bCacheAll) CacheShadows();
	else CPrintF( TRANS("All shadows uncached.\n"));
}

extern void ReloadMeshes(void)
{
	// mute all sounds
	_pSound->Mute();

	// loop thru mesh stock
	{FOREACHINDYNAMICCONTAINER( _pMeshStock->st_ctObjects, CMesh, itmsh) {
		CMesh &msh = *itmsh;
		msh.Reload();
	}}

	CPrintF( TRANS("All meshes reloaded.\n"));
}

// reload all textures that were loaded
extern void ReloadTextures(void)
{
	// mute all sounds
	_pSound->Mute();

	// update texture settings
	UpdateTextureSettings();
	// loop thru texture stock
	{FOREACHINDYNAMICCONTAINER( _pTextureStock->st_ctObjects, CTextureData, ittd) {
		CTextureData &td = *ittd;
		td.Reload();
		td.td_tpLocal.Clear();
	}}

	// reset fog/haze texture
	_fog_pixSizeH = 0;
	_fog_pixSizeL = 0;
	_haze_pixSize = 0;

	// reinit flat texture
	ASSERT( _ptdFlat!=NULL);
	_ptdFlat->td_tpLocal.Clear();
	_ptdFlat->Unbind();
	_ptdFlat->td_ulFlags = TEX_ALPHACHANNEL | TEX_32BIT | TEX_STATIC;
	_ptdFlat->td_mexWidth  = 1;
	_ptdFlat->td_mexHeight = 1;
	_ptdFlat->td_iFirstMipLevel  = 0;
	_ptdFlat->td_ctFineMipLevels = 1;
	_ptdFlat->td_slFrameSize = 1*1* BYTES_PER_TEXEL;    
	_ptdFlat->td_ctFrames = 1;
	_ptdFlat->td_ulInternalFormat = TS.ts_tfRGBA8;
	_ptdFlat->td_pulFrames = &_ulWhite;
	_ptdFlat->SetAsCurrent();

	/*
	// reset are renderable textures, too
	CListHead &lhOriginal = _pGfx->gl_lhRenderTextures;
	while( !lhOriginal.IsEmpty()) {
		CRenderTexture &rt = *LIST_HEAD( lhOriginal, CRenderTexture, rt_lnInGfx);
		rt.Reset();
	}
	*/

	{FOREACHINLIST(CTerrain, tr_lnLoadedTerrains, _pGfx->gl_lhLoadedTerrains, ittr) {
		// reload terrain textures
		TR_BindTerrainTextures(ittr);
	}
	}

	// mark that we need pretouching
	_bNeedPretouch = TRUE;
	CPrintF( TRANS("All textures reloaded.\n"));
}


extern void ClearVertexAndPixelPrograms(void)
{
	// is shader stock is valid
	if(_pShaderStock!=NULL) {
		// for each shader in stock
		INDEX ctshd = _pShaderStock->st_ctObjects.Count();
		for(INDEX ishd=0;ishd<ctshd;ishd++) {
			CShader &shd = _pShaderStock->st_ctObjects[ishd];
			// Delete precached vertex and pixel program handles
			shd.DeleteHandles();
		}
	}

	extern void RM_ClearShaders(void);
	RM_ClearShaders();
	extern void TR_ClearTerrainShaders(void);
	TR_ClearTerrainShaders();
}


// refreshes all textures and shadow maps
static void RefreshTextures(void)
{
	// refresh
	ReloadTextures();
	RecacheShadows();
}


static void RefreshShaders(void)
{
	extern void ClearVertexAndPixelPrograms(void);
	ClearVertexAndPixelPrograms();
	CPrintF("All shaders uncached\n");
}

// reload all models that were loaded
static void ReloadModels(void)
{
	// mute all sounds
	_pSound->Mute();
	// loop thru model stock
	{FOREACHINDYNAMICCONTAINER( _pModelStock->st_ctObjects, CModelData, itmd) {
		CModelData &md = *itmd;
		md.Reload();
	}}
	// mark that we need pretouching
	_bNeedPretouch = TRUE;
	// all done
	CPrintF( TRANS("All models reloaded.\n"));
}



/*
 * Construct uninitialized gfx library.
 */
CGfxLibrary::CGfxLibrary(void)
{
	// reset some variables to default
	gl_iFrameNumber = 1;
	gl_slAllowedUploadBurst = 0;
	gl_bAllowProbing = FALSE;

	gl_iSwapInterval = 1234;
	gl_pixMaxTextureDimension = 8192;
	gl_ctTextureUnits = 0;
	gl_ctRealTextureUnits = 0;
	gl_fTextureLODBias = 0;
	gl_fMaxTextureLODBias = 0;   
	gl_iMaxTextureAnisotropy = 0;
	gl_iMaxTessellationLevel = 0;
	gl_iTessellationLevel = 0;
	gl_ulFlags = NONE;
	
	// no driver loaded
	_iGfxAPI = -1;  // just for some color.h routines
	gl_eCurrentAPI = GAT_NONE;
	gl_hiDriver = NONE;
	go_hglRC = NONE;
	gl_ctDriverChanges = 0;

	// DX8 not loaded either
	gl_pD3D = NONE;
	gl_pd3dDevice = NULL;
	gl_d3dColorFormat = (D3DFORMAT)NONE;
	gl_d3dDepthFormat = (D3DFORMAT)NONE;
	gl_pvpActive = NULL;
	gl_ctMaxStreams = 0;
	gl_dwVertexShader = 0;
	gl_dwPixelShader  = 0;
	gl_pd3dIdx = NULL;
	gl_pd3dVtx = NULL;
	gl_pd3dNor = NULL;
	gl_pd3dWgh = NULL;
	for( INDEX i=0; i<GFX_MAXLAYERS; i++) gl_pd3dCol[i] = gl_pd3dTex[i] = NULL;
	gl_ctVertices = 0;
	gl_ctIndices  = 0;

	// reset profiling counters
	gl_ctWorldElements     = 0;
	gl_ctModelElements     = 0;
	gl_ctParticleTriangles = 0;
	gl_ctTotalElements     = 0;

	gl_bCompressedTexture = TRUE;

	// init flat texture
	_ptdFlat = new CTextureData;
	_ptdFlat->td_ulFlags = TEX_ALPHACHANNEL | TEX_32BIT | TEX_STATIC;

	// prepare some quad elements
	extern void AddQuadElements( const INDEX ctQuads);
	AddQuadElements(1024); // should be enough (at least for a start)

	// reset GFX API function pointers
	GFX_SetFunctionPointers( (INDEX)GAT_NONE);
}


/*
 * Destruct (and clean up).
 */
CGfxLibrary::~CGfxLibrary()
{
	extern void EnableWindowsKeys(void);
	EnableWindowsKeys();
	// free common arrays
	_avtxCommon.Clear();
	_anorCommon.Clear();
	_acolCommon.Clear();
	for( INDEX i=0; i<GFX_USETEXUNITS; i++) _atexCommon[i].Clear();
	_auwCommonElements.Clear();
	_auwCommonQuads.Clear();
	// stop current display mode
	StopDisplayMode();
	// safe release of flat texture
	ASSERT( _ptdFlat!=NULL);
	_ptdFlat->td_pulFrames = NULL;
	delete _ptdFlat;
	_ptdFlat = NULL;
}



#define SM_CXVIRTUALSCREEN  78 
#define SM_CYVIRTUALSCREEN  79 
#define SM_CMONITORS        80 



/* Initialize library for application main window. */
void CGfxLibrary::Init(void)
{
	ASSERT( this!=NULL);

	// report desktop settings
	CPrintF(TRANS("Desktop settings...\n"));

	HDC hdc = GetDC(NULL); 
	SLONG slBPP = GetDeviceCaps(hdc, PLANES) * GetDeviceCaps(hdc, BITSPIXEL); 
	ReleaseDC(NULL, hdc);  
	gfx_ctMonitors = GetSystemMetrics(SM_CMONITORS);
	gfx_bMultiMonDisabled = FALSE;

	CPrintF(TRANS("  Color Depth: %dbit\n"), slBPP);
	CPrintF(TRANS("  Screen: %dx%d\n"), GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	CPrintF(TRANS("  Virtual screen: %dx%d\n"), GetSystemMetrics(SM_CXVIRTUALSCREEN), GetSystemMetrics(SM_CYVIRTUALSCREEN));
	CPrintF(TRANS("  Monitors directly reported: %d\n"), gfx_ctMonitors);
	CPrintF("\n");
 
	// we will never allow glide splash screen
	putenv( "FX_GLIDE_NO_SPLASH=1");

	// declare some console vars
	_pShell->DeclareSymbol("user void MonitorsOn(void);",  &MonitorsOn);
	_pShell->DeclareSymbol("user void MonitorsOff(void);", &MonitorsOff);

	_pShell->DeclareSymbol("user void GAPInfo(void);", &GAPInfo);
	_pShell->DeclareSymbol("user void ListOpenGLExtensions(void);", &ListOpenGLExtensions);
	_pShell->DeclareSymbol("user void TexturesInfo(void);",    &TexturesInfo);
	_pShell->DeclareSymbol("user void UncacheShadows(void);",  &UncacheShadows);
	_pShell->DeclareSymbol("user void RecacheShadows(void);",  &RecacheShadows);
	_pShell->DeclareSymbol("user void RefreshTextures(void);", &RefreshTextures);
	_pShell->DeclareSymbol("user void ReloadModels(void);",    &ReloadModels);
	_pShell->DeclareSymbol("user void ReloadMeshes(void);",    &ReloadMeshes);
	_pShell->DeclareSymbol("user void RefreshShaders(void);",  &RefreshShaders);
	

	_pShell->DeclareSymbol("persistent user INDEX ogl_bUseCompiledVertexArrays;", &ogl_bUseCompiledVertexArrays);
	_pShell->DeclareSymbol("persistent user INDEX ogl_bExclusive;", &ogl_bExclusive);
	_pShell->DeclareSymbol("persistent user INDEX ogl_bAllowQuadArrays;",   &ogl_bAllowQuadArrays);
	_pShell->DeclareSymbol("persistent user INDEX ogl_iTextureCompressionType;", &ogl_iTextureCompressionType);
	_pShell->DeclareSymbol("persistent user INDEX ogl_iMaxBurstSize;",    &ogl_iMaxBurstSize);
	_pShell->DeclareSymbol("persistent user INDEX ogl_bGrabDepthBuffer;", &ogl_bGrabDepthBuffer);
	_pShell->DeclareSymbol("persistent user INDEX ogl_bHardFinish;", &ogl_bHardFinish);
	_pShell->DeclareSymbol("persistent user INDEX ogl_iFinish;",     &ogl_iFinish);

	_pShell->DeclareSymbol("persistent user INDEX ogl_bTruformLinearNormals;", &ogl_bTruformLinearNormals);
	_pShell->DeclareSymbol("persistent user INDEX ogl_bAlternateClipPlane;",   &ogl_bAlternateClipPlane);

	_pShell->DeclareSymbol("persistent user INDEX d3d_bUseHardwareTnL;", &d3d_bUseHardwareTnL);
	_pShell->DeclareSymbol("persistent user INDEX d3d_iMaxBurstSize;",   &d3d_iMaxBurstSize);
	_pShell->DeclareSymbol("persistent user INDEX d3d_iVertexRangeTreshold;", &d3d_iVertexRangeTreshold);
	_pShell->DeclareSymbol("persistent user INDEX d3d_bAlternateDepthReads;", &d3d_bAlternateDepthReads);

	_pShell->DeclareSymbol("persistent user INDEX d3d_iFinish;", &d3d_iFinish);
	_pShell->DeclareSymbol("persistent user INDEX d3d_iVertexBuffersSize;", &d3d_iVertexBuffersSize);
	_pShell->DeclareSymbol("persistent      INDEX d3d_bFastUpload;", &d3d_bFastUpload);

																																		
	_pShell->DeclareSymbol("persistent user INDEX gap_iUseTextureUnits;",   &gap_iUseTextureUnits);
	_pShell->DeclareSymbol("persistent user INDEX gap_iTextureFiltering;",  &gap_iTextureFiltering);
	_pShell->DeclareSymbol("persistent user INDEX gap_iTextureAnisotropy;", &gap_iTextureAnisotropy);
	_pShell->DeclareSymbol("persistent user FLOAT gap_fTextureLODBias;",    &gap_fTextureLODBias);
	_pShell->DeclareSymbol("persistent user INDEX gap_bAggressiveAnisotropy;", &gap_bAggressiveAnisotropy);
	_pShell->DeclareSymbol("persistent user INDEX gap_bAllowGrayTextures;",    &gap_bAllowGrayTextures);
	_pShell->DeclareSymbol("persistent user INDEX gap_bUseOcclusionCulling;",  &gap_bUseOcclusionCulling);
	_pShell->DeclareSymbol("persistent user INDEX gap_bOptimizeStateChanges;", &gap_bOptimizeStateChanges);
	_pShell->DeclareSymbol("persistent user INDEX gap_iOptimizeDepthReads;",   &gap_iOptimizeDepthReads);
	_pShell->DeclareSymbol("persistent user INDEX gap_iOptimizeClipping;",     &gap_iOptimizeClipping);
	_pShell->DeclareSymbol("persistent user INDEX gap_iSwapInterval;", &gap_iSwapInterval);
	_pShell->DeclareSymbol("persistent user INDEX gap_iRefreshRate;",  &gap_iRefreshRate);
	_pShell->DeclareSymbol("persistent user INDEX gap_iDithering;", &gap_iDithering);
	_pShell->DeclareSymbol("persistent user INDEX gap_iDepthBits;", &gap_iDepthBits);
	_pShell->DeclareSymbol("persistent user INDEX gap_bForceTruform;", &gap_bForceTruform);
	_pShell->DeclareSymbol("persistent user INDEX gap_iTruformLevel;", &gap_iTruformLevel);
	_pShell->DeclareSymbol("                INDEX mdl_bTruformWeapons;", &mdl_bTruformWeapons);

	_pShell->DeclareSymbol("           user INDEX gfx_bRenderModels;",    &gfx_bRenderModels);
	_pShell->DeclareSymbol("persistent user INDEX mdl_bRenderDetail;",     &mdl_bRenderDetail);
	_pShell->DeclareSymbol("persistent user INDEX mdl_bRenderSpecular;",   &mdl_bRenderSpecular);
	_pShell->DeclareSymbol("persistent user INDEX mdl_bRenderReflection;", &mdl_bRenderReflection);
	_pShell->DeclareSymbol("persistent user INDEX mdl_bAllowOverbright;",  &mdl_bAllowOverbright);
	_pShell->DeclareSymbol("persistent user FLOAT mdl_fLODMul;", &mdl_fLODMul);
	_pShell->DeclareSymbol("persistent user FLOAT mdl_fLODAdd;", &mdl_fLODAdd);
	_pShell->DeclareSymbol("persistent user INDEX mdl_iLODDisappear;", &mdl_iLODDisappear);
	_pShell->DeclareSymbol("persistent user INDEX mdl_iShadowQuality;",     &mdl_iShadowQuality);
	_pShell->DeclareSymbol("           user INDEX mdl_bShowOccludees;", &mdl_bShowOccludees);
	_pShell->DeclareSymbol("           user INDEX mdl_bShowTriangles;", &mdl_bShowTriangles);
	_pShell->DeclareSymbol("           user INDEX mdl_bCreateStrips;",  &mdl_bCreateStrips);
	_pShell->DeclareSymbol("           user INDEX mdl_bShowStrips;",    &mdl_bShowStrips);
	
	_pShell->DeclareSymbol("persistent user FLOAT ska_fLODMul;", &ska_fLODMul);
	_pShell->DeclareSymbol("persistent user FLOAT ska_fLODAdd;", &ska_fLODAdd);
	_pShell->DeclareSymbol("           user INDEX ska_bShowSkeleton;",  &ska_bShowSkeleton);
	_pShell->DeclareSymbol("           user INDEX ska_bShowCollision;", &ska_bShowCollision);
	_pShell->DeclareSymbol("           user INDEX ska_bShowAllFramesBox;", &ska_bShowAllFramesBox);
	_pShell->DeclareSymbol("           user INDEX ska_bShowActiveBones;",  &ska_bShowActiveBones);
	_pShell->DeclareSymbol("           user INDEX ska_bRenderMesh;",    &ska_bRenderMesh);
	_pShell->DeclareSymbol("           user INDEX ska_bAnimateMesh;",   &ska_bAnimateMesh);
	_pShell->DeclareSymbol("           user INDEX ska_bMorphMesh;",     &ska_bMorphMesh);
	_pShell->DeclareSymbol("           user INDEX ska_bCalcBoneTransf;", &ska_bCalcBoneTransf);
	_pShell->DeclareSymbol("           user INDEX ska_bTransformMesh;", &ska_bTransformMesh);
	_pShell->DeclareSymbol("persistent user INDEX ska_bUseHardwareShaders;", &ska_bUseHardwareShaders);
	_pShell->DeclareSymbol("persistent user INDEX ska_iMaxWeightsPerVertex;", &ska_iMaxWeightsPerVertex);
	_pShell->DeclareSymbol("persistent user INDEX ska_bAllowBonesAdjustment;", &ska_bAllowBonesAdjustment);
	_pShell->DeclareSymbol("                INDEX ska_bReplaceSmcWithBmc;",&ska_bReplaceSmcWithBmc);
//안태훈 수정 시작	//(Easy Use World Editor)(0.1)
	_pShell->DeclareSymbol("           user INDEX ska_bShowTriangles;",&ska_bShowTriangles);
	_pShell->DeclareSymbol("           user INDEX g_bShowTerrain;",&g_bShowTerrain);
	_pShell->DeclareSymbol("           user INDEX g_bShowBSP;",&g_bShowBSP);
	_pShell->DeclareSymbol("           user INDEX g_bShowModel;",&g_bShowModel);
	_pShell->DeclareSymbol("           user INDEX g_bShowWater;",&g_bShowWater);
	_pShell->DeclareSymbol("           user INDEX g_iShowReflectionMap;",&g_iShowReflectionMap);
	_pShell->DeclareSymbol("           user INDEX g_bReflection;",&g_bReflection);
//안태훈 수정 끝	//(Easy Use World Editor)(0.1)
	_pShell->DeclareSymbol("persistent user INDEX login_bSkipSelect;",&login_bSkipSelect);	
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
	_pShell->DeclareSymbol("           user INDEX ska_bShowSkaTag;", &ska_bShowSkaTag);
	_pShell->DeclareSymbol("           user INDEX ska_bShowAttachedTag;", &ska_bShowAttachedTag);
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)

	
	_pShell->DeclareSymbol("           user INDEX sha_bColorizeShaders;", &sha_bColorizeShaders);
	_pShell->DeclareSymbol("           user INDEX dbg_bNormalizeShaderNormals;", &dbg_bNormalizeShaderNormals);
	_pShell->DeclareSymbol("persistent user INDEX sha_iResetConstRegisters;", &sha_iResetConstRegisters);
	_pShell->DeclareSymbol("persistent user INDEX sha_iOptimizeShaders;",&sha_iOptimizeShaders);

	_pShell->DeclareSymbol("           user INDEX ter_bShowQuadTree;",   &ter_bShowQuadTree);
	_pShell->DeclareSymbol("           user INDEX ter_bRenderTerrain;",  &ter_bRenderTerrain);
	_pShell->DeclareSymbol("           user INDEX ter_bUpdateTerrain;",  &ter_bUpdateTerrain);
	_pShell->DeclareSymbol("           user INDEX ter_iShowQuadTreeLev;",&ter_iShowQuadTreeLev);
	_pShell->DeclareSymbol("           user INDEX ter_bShowVertices;",   &ter_bShowVertices);
	_pShell->DeclareSymbol("           user INDEX ter_bShowLerpTargets;",&ter_bShowLerpTargets);
	_pShell->DeclareSymbol("           user INDEX ter_bDisableViewer;",  &ter_bDisableViewer);
	_pShell->DeclareSymbol("           user INDEX ter_bShowTriangles;",  &ter_bShowTriangles);
	_pShell->DeclareSymbol("                INDEX ter_bLerpVertices;",   &ter_bLerpVertices);         // not active yet
	_pShell->DeclareSymbol("           user INDEX ter_bOptimizeRendering;", &ter_bOptimizeRendering);
	_pShell->DeclareSymbol("           user INDEX ter_iRecordRayCasting; ", &ter_iRecordRayCasting);
	_pShell->DeclareSymbol("persistent user INDEX ter_bUseHardwareShaders;", &ter_bUseHardwareShaders);
	_pShell->DeclareSymbol("persistent user INDEX ter_bUseTopMap;", &ter_bUseTopMap);
	_pShell->DeclareSymbol("persistent user INDEX ter_bUpdateTopMapOnEdit;", &ter_bUpdateTopMapOnEdit);
	_pShell->DeclareSymbol("           user INDEX ter_iAttributeRange; ", &ter_iAttributeRange);
	

	_pShell->DeclareSymbol("persistent user FLOAT gfx_tmProbeDecay;", &gfx_tmProbeDecay);
	_pShell->DeclareSymbol("persistent user INDEX gfx_iProbeSize;",   &gfx_iProbeSize);
	_pShell->DeclareSymbol("persistent user INDEX gfx_bClearScreen;",   &gfx_bClearScreen);
	_pShell->DeclareSymbol("persistent user INDEX gfx_bDecoratedText;", &gfx_bDecoratedText);
	_pShell->DeclareSymbol("persistent user INDEX gfx_bSeparateTripleHead;",     &gfx_bSeparateTripleHead);
	_pShell->DeclareSymbol("persistent user INDEX gfx_bDisableMultiMonSupport;", &gfx_bDisableMultiMonSupport);
	_pShell->DeclareSymbol("persistent user INDEX gfx_bDisableWindowsKeys;",     &gfx_bDisableWindowsKeys);
	_pShell->DeclareSymbol("     const user INDEX gfx_bMultiMonDisabled;", &gfx_bMultiMonDisabled);
	_pShell->DeclareSymbol("     const user INDEX gfx_ctMonitors;",        &gfx_ctMonitors);

	_pShell->DeclareSymbol("persistent user INDEX tex_iNormalQuality;",    &tex_iNormalQuality);
	_pShell->DeclareSymbol("persistent user INDEX tex_iAnimationQuality;", &tex_iAnimationQuality);
	_pShell->DeclareSymbol("persistent user INDEX tex_bFineEffect;",       &tex_bFineEffect);
	_pShell->DeclareSymbol("persistent user INDEX tex_bFineFog;",          &tex_bFineFog);
	_pShell->DeclareSymbol("persistent user INDEX tex_iNormalSize;",    &tex_iNormalSize);
	_pShell->DeclareSymbol("persistent user INDEX tex_iAnimationSize;", &tex_iAnimationSize);
	_pShell->DeclareSymbol("persistent user INDEX tex_iEffectSize;",    &tex_iEffectSize);
	_pShell->DeclareSymbol("persistent user INDEX tex_iFogSize;",       &tex_iFogSize);
	_pShell->DeclareSymbol("persistent user INDEX tex_bCompressGrayscale;",     &tex_bCompressGrayscale);
	_pShell->DeclareSymbol("persistent user INDEX tex_bCompressAlphaChannel;",  &tex_bCompressAlphaChannel);
	_pShell->DeclareSymbol("persistent user INDEX tex_bAlternateCompression;",  &tex_bAlternateCompression);
	_pShell->DeclareSymbol("persistent user INDEX tex_bAggressiveCompression;", &tex_bAggressiveCompression);
	_pShell->DeclareSymbol("persistent user INDEX tex_bDynamicMipmaps;", &tex_bDynamicMipmaps);
	_pShell->DeclareSymbol("persistent user INDEX tex_iDithering;", &tex_iDithering);
	_pShell->DeclareSymbol("persistent user INDEX tex_iFiltering;", &tex_iFiltering);
	_pShell->DeclareSymbol("persistent user INDEX tex_iEffectFiltering;",   &tex_iEffectFiltering);
	_pShell->DeclareSymbol("persistent user INDEX tex_bProgressiveFilter;", &tex_bProgressiveFilter);
	_pShell->DeclareSymbol("           user INDEX tex_bColorizeMipmaps;",   &tex_bColorizeMipmaps);

	_pShell->DeclareSymbol("persistent user INDEX shd_iStaticSize;",   &shd_iStaticSize);
	_pShell->DeclareSymbol("persistent user INDEX shd_iDynamicSize;",  &shd_iDynamicSize);
	_pShell->DeclareSymbol("persistent user INDEX shd_bFineQuality;",  &shd_bFineQuality);
	_pShell->DeclareSymbol("persistent user INDEX shd_iAllowDynamic;", &shd_iAllowDynamic);
	_pShell->DeclareSymbol("persistent user INDEX shd_bDynamicMipmaps;", &shd_bDynamicMipmaps);
	_pShell->DeclareSymbol("persistent user INDEX shd_iFiltering;", &shd_iFiltering);
	_pShell->DeclareSymbol("persistent user INDEX shd_iDithering;", &shd_iDithering);
	_pShell->DeclareSymbol("persistent user FLOAT shd_tmFlushDelay;", &shd_tmFlushDelay);
	_pShell->DeclareSymbol("persistent user FLOAT shd_fCacheSize;",   &shd_fCacheSize);
	_pShell->DeclareSymbol("persistent user INDEX shd_bCacheAll;",    &shd_bCacheAll);
	_pShell->DeclareSymbol("persistent user INDEX shd_bAllowFlats;", &shd_bAllowFlats);
	_pShell->DeclareSymbol("persistent      INDEX shd_iForceFlats;", &shd_iForceFlats);
	_pShell->DeclareSymbol("           user INDEX shd_bShowFlats;",  &shd_bShowFlats);
	_pShell->DeclareSymbol("           user INDEX shd_bColorize;",   &shd_bColorize);
	
	_pShell->DeclareSymbol("           user INDEX gfx_bRenderParticles;", &gfx_bRenderParticles);
	_pShell->DeclareSymbol("           user INDEX gfx_bRenderFog;",       &gfx_bRenderFog);
	_pShell->DeclareSymbol("           user INDEX gfx_bRenderWorld;",     &gfx_bRenderWorld);
	_pShell->DeclareSymbol("           user INDEX gfx_bRenderView;",      &gfx_bRenderView);
	_pShell->DeclareSymbol("persistent user INDEX gfx_iLensFlareQuality;", &gfx_iLensFlareQuality);
	_pShell->DeclareSymbol("persistent user INDEX wld_bTextureLayers;", &wld_bTextureLayers);
	_pShell->DeclareSymbol("persistent user INDEX wld_bRenderMirrors;", &wld_bRenderMirrors);
	_pShell->DeclareSymbol("persistent user FLOAT wld_fEdgeOffsetI;",   &wld_fEdgeOffsetI);
	_pShell->DeclareSymbol("persistent user FLOAT wld_fEdgeAdjustK;",   &wld_fEdgeAdjustK);
	_pShell->DeclareSymbol("persistent user INDEX wld_iDetailRemovingBias;", &wld_iDetailRemovingBias);
	_pShell->DeclareSymbol("           user INDEX wld_bRenderEmptyBrushes;", &wld_bRenderEmptyBrushes);
	_pShell->DeclareSymbol("           user INDEX wld_bRenderShadowMaps;",   &wld_bRenderShadowMaps);
	_pShell->DeclareSymbol("           user INDEX wld_bRenderTextures;",     &wld_bRenderTextures);
	_pShell->DeclareSymbol("           user INDEX wld_bRenderDetailPolygons;", &wld_bRenderDetailPolygons);
	_pShell->DeclareSymbol("           user INDEX wld_bShowTriangles;",        &wld_bShowTriangles);
	_pShell->DeclareSymbol("           user INDEX wld_bShowDetailTextures;",   &wld_bShowDetailTextures);
	_pShell->DeclareSymbol("                INDEX wld_bExcludeEditorModels;",  &wld_bExcludeEditorModels);

	_pShell->DeclareSymbol("           user INDEX wed_bIgnoreTJunctions;", &wed_bIgnoreTJunctions);
	_pShell->DeclareSymbol("persistent user INDEX wed_bUseBaseForReplacement;", &wed_bUseBaseForReplacement);

	_pShell->DeclareSymbol("persistent user INDEX dbg_bShowFreeMemory;", &dbg_bShowFreeMemory);
	_pShell->DeclareSymbol("persistent user INDEX dbg_bShowDeletedObjectsFromStock;", &dbg_bShowDeletedObjectsFromStock);

	_pShell->DeclareSymbol("persistent user FLOAT gfx_fBrightness;", &gfx_fBrightness);
	_pShell->DeclareSymbol("persistent user FLOAT gfx_fContrast;",   &gfx_fContrast);
	_pShell->DeclareSymbol("persistent user FLOAT gfx_fGamma;",      &gfx_fGamma);
	_pShell->DeclareSymbol("persistent user FLOAT gfx_fBiasR;",  &gfx_fBiasR);
	_pShell->DeclareSymbol("persistent user FLOAT gfx_fBiasG;",  &gfx_fBiasG);
	_pShell->DeclareSymbol("persistent user FLOAT gfx_fBiasB;",  &gfx_fBiasB);
	_pShell->DeclareSymbol("persistent user INDEX gfx_iLevels;", &gfx_iLevels);

	_pShell->DeclareSymbol("persistent user INDEX gfx_iStereo;", &gfx_iStereo);
	_pShell->DeclareSymbol("persistent user INDEX gfx_bStereoInvert;", &gfx_bStereoInvert);
	_pShell->DeclareSymbol("persistent user INDEX gfx_iStereoOffset;", &gfx_iStereoOffset);
	_pShell->DeclareSymbol("persistent user FLOAT gfx_fStereoSeparation;", &gfx_fStereoSeparation);

	_pShell->DeclareSymbol( "INDEX sys_bHasTextureCompression;", &sys_bHasTextureCompression);
	_pShell->DeclareSymbol( "INDEX sys_bHasTextureAnisotropy;", &sys_bHasTextureAnisotropy);
	_pShell->DeclareSymbol( "INDEX sys_bHasAdjustableGamma;", &sys_bHasAdjustableGamma);
	_pShell->DeclareSymbol( "INDEX sys_bHasTextureLODBias;", &sys_bHasTextureLODBias);
	_pShell->DeclareSymbol( "INDEX sys_bHasMultitexturing;", &sys_bHasMultitexturing);
	_pShell->DeclareSymbol( "INDEX sys_bHas32bitTextures;", &sys_bHas32bitTextures);
	_pShell->DeclareSymbol( "INDEX sys_bHasSwapInterval;", &sys_bHasSwapInterval);
	_pShell->DeclareSymbol( "INDEX sys_bHasHardwareTnL;", &sys_bHasHardwareTnL);
	_pShell->DeclareSymbol( "INDEX sys_bHasTruform;", &sys_bHasTruform);
	_pShell->DeclareSymbol( "INDEX sys_bHasCVAs;", &sys_bHasCVAs);
	_pShell->DeclareSymbol( "INDEX sys_bUsingOpenGL;",   &sys_bUsingOpenGL);
	_pShell->DeclareSymbol( "INDEX sys_bUsingDirect3D;", &sys_bUsingDirect3D);

	// initialize gfx APIs support
	InitAPIs();

}



// set new display mode
BOOL CGfxLibrary::SetDisplayMode( enum GfxAPIType eAPI, INDEX iAdapter, PIX pixSizeI, PIX pixSizeJ,
																	enum DisplayDepth eColorDepth)
{
	// some safeties
	ASSERT( pixSizeI>0 && pixSizeJ>0);

	// determine new API
	GfxAPIType eNewAPI = eAPI;
	if( eNewAPI==GAT_CURRENT) eNewAPI = gl_eCurrentAPI;
	
	// shutdown old and startup new API, and mode and ... stuff, you know!
	StopDisplayMode();
	BOOL bRet = StartDisplayMode( eNewAPI, iAdapter, pixSizeI, pixSizeJ, eColorDepth);
	if( !bRet) return FALSE; // didn't make it?

	// update some info
	gl_iCurrentAdapter = gl_gaAPI[gl_eCurrentAPI].ga_iCurrentAdapter = iAdapter;
	gl_dmCurrentDisplayMode.dm_pixSizeI = pixSizeI;
	gl_dmCurrentDisplayMode.dm_pixSizeJ = pixSizeJ;
	gl_dmCurrentDisplayMode.dm_ddDepth  = eColorDepth;

	// prepare texture formats for this display mode
	extern void DetermineSupportedTextureFormats( GfxAPIType eAPI);
	DetermineSupportedTextureFormats(gl_eCurrentAPI);
	
	// made it! (eventually disable windows system keys)
	if( gfx_bDisableWindowsKeys) DisableWindowsKeys();

	return TRUE;
}


// set display mode to original desktop display mode and default ICD driver
BOOL CGfxLibrary::ResetDisplayMode( enum GfxAPIType eAPI/*=GAT_CURRENT*/)
{
	// determine new API
	GfxAPIType eNewAPI = eAPI;
	if( eNewAPI==GAT_CURRENT) eNewAPI = gl_eCurrentAPI;

	// shutdown old and startup new API, and mode and ... stuff, you know!
	StopDisplayMode();
	BOOL bRet = StartDisplayMode( eNewAPI, 0, 0, 0, DISPD_DEFAULT);
	if( !bRet) return FALSE; // didn't make it?

	// update some info
	gl_iCurrentAdapter = 0;
	gl_dmCurrentDisplayMode.dm_pixSizeI = 0;
	gl_dmCurrentDisplayMode.dm_pixSizeJ = 0;
	gl_dmCurrentDisplayMode.dm_ddDepth  = DISPD_DEFAULT;

	// prepare texture formats for this display mode
	extern void DetermineSupportedTextureFormats( GfxAPIType eAPI);
	DetermineSupportedTextureFormats(gl_eCurrentAPI);

	// made it!
	EnableWindowsKeys();

	return TRUE;
}



// startup gfx API and set given display mode
BOOL CGfxLibrary::StartDisplayMode( enum GfxAPIType eAPI, INDEX iAdapter, PIX pixSizeI, PIX pixSizeJ,
																		enum DisplayDepth eColorDepth)
{
	// reinit gamma table
	_fLastBrightness = 999;
	_fLastContrast   = 999;
	_fLastGamma      = 999;
	_iLastLevels = 999;
	_fLastBiasR  = 999;
	_fLastBiasG  = 999;
	_fLastBiasB  = 999;

	// prepare
	BOOL bSuccess;
	ASSERT( iAdapter>=0);
	const BOOL bFullScreen = (pixSizeI>0 && pixSizeJ>0);
	gl_ulFlags &= GLF_ADJUSTABLEGAMMA;
	gl_ctDriverChanges++;
	GFX_bRenderingScene = FALSE;
	GFX_ulLastDrawPortID = 0;  
	gl_iTessellationLevel = 0;
	gl_ctRealTextureUnits = 0;
	_iLastVertexBufferSize = 0;

	// OpenGL driver ?
	if( eAPI==GAT_OGL)
	{
		// disable multimonitor support if it can interfere with OpenGL
		MonitorsOff();
		if( bFullScreen) {
			// set windows mode to fit same size
			bSuccess = CDS_SetMode( pixSizeI, pixSizeJ, eColorDepth);
			if( !bSuccess) return FALSE;
		} else {
			// reset windows mode
			CDS_ResetMode();
		}
		// startup OpenGL
		bSuccess = InitDriver_OGL(iAdapter!=0);
		// try to setup sub-driver
		if( !bSuccess) {
			// reset windows mode and fail
			CDS_ResetMode();
			return FALSE;
		} // made it
		gl_eCurrentAPI = GAT_OGL;
		gl_iSwapInterval = 1234; // need to reset
	 _iGfxAPI = 0;  // just for some color.h routines
	}

	// DirectX driver ?
	else if( eAPI==GAT_D3D)
	{
		// startup D3D
		bSuccess = InitDriver_D3D();
		if( !bSuccess) return FALSE; // what, didn't make it?
		bSuccess = InitDisplay_D3D( iAdapter, pixSizeI, pixSizeJ, eColorDepth);
		if( !bSuccess) return FALSE;
		// made it
		gl_eCurrentAPI = GAT_D3D;
	 _iGfxAPI = 1;  // just for some color.h routines
	}

	// no driver
	else
	{
		ASSERT( eAPI==GAT_NONE); 
		gl_eCurrentAPI = GAT_NONE;
	 _iGfxAPI = -1;  // just for some color.h routines
	}

	// initialize on first child window
	gl_iFrameNumber = 1;
	gl_pvpActive = NULL;
	bFullScreen ? gl_ulFlags|=GLF_FULLSCREEN : gl_ulFlags&=~GLF_FULLSCREEN;
	// mark that some things needs to be reinitialized
	gl_fTextureLODBias = 0.0f;

	// set function pointers
	GFX_SetFunctionPointers( (INDEX)gl_eCurrentAPI);

	// flag the need for context
	gl_ulFlags |= GLF_INITONNEXTWINDOW;

	// allocate and reset occlusion queries
	_pslOcclusionQueryFrames = (SLONG*)AllocMemory( 2* GFX_MAXOCCQUERIES *sizeof(SLONG));
	_pulOcclusionQueryOwners = (ULONG*)&_pslOcclusionQueryFrames[GFX_MAXOCCQUERIES];
	StoreLongs( 0, (ULONG*)_pslOcclusionQueryFrames, 2*GFX_MAXOCCQUERIES);
	_ctUsedOcclusionQueries = 0;

	// all done
	return TRUE;
}


// Stop display mode and shutdown API
void CGfxLibrary::StopDisplayMode(void)
{
	// release all cached shadows and models' arrays
	extern void Models_ClearVertexArrays(void);
	extern void UncacheShadows(void);
	Models_ClearVertexArrays();
	UncacheShadows();

	extern void ReleaseShadowTexture();
	ReleaseShadowTexture();

	extern void ReleaseRefractionTexture();
	ReleaseRefractionTexture();
	extern void ReleaseReflectionTexture();
	ReleaseReflectionTexture();
	// sehan
	extern void ReleaseBloomTexture();
	ReleaseBloomTexture();
	// sehan end

	{FOREACHINLIST(CTerrain, tr_lnLoadedTerrains, _pGfx->gl_lhLoadedTerrains, ittr) {
		// reload terrain textures
		TR_UnbindTerrainTextures(ittr);
	}
	}

	// unbind textures
	if( _pTextureStock!=NULL) {
		{FOREACHINDYNAMICCONTAINER( _pTextureStock->st_ctObjects, CTextureData, ittd) {
			CTextureData &td = *ittd;
			td.td_tpLocal.Clear();
			td.Unbind();
		}}
	}

	// unbind fog, haze and flat texture
	extern ULONG _fog_ulTexture;
	extern ULONG _haze_ulTexture;
	gfxDeleteTexture( _fog_ulTexture); 
	gfxDeleteTexture( _haze_ulTexture);
	ASSERT( _ptdFlat!=NULL);
	_ptdFlat->td_tpLocal.Clear();
	_ptdFlat->Unbind();

	// delete all vertex buffers held by SKA models
	if( _pMeshStock!=NULL) {
		{FOREACHINDYNAMICCONTAINER( _pMeshStock->st_ctObjects, CMesh, itmsh) {
			CMesh &msh = *itmsh;
			msh.Unbind();
		}}
	}
	// also delete morph buffers used by SKA models
	extern void DeleteMorphedDynamicBuffer(void);
	DeleteMorphedDynamicBuffer();
	
	// shutdown API
	if( gl_eCurrentAPI==GAT_OGL)
	{ // OpenGL
		EndDriver_OGL();
		MonitorsOn();       // re-enable multimonitor support if disabled
		CDS_ResetMode();
	}
	else if( gl_eCurrentAPI==GAT_D3D)
	{ // Direct3D
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
//		Finalize_EffectSystem();
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
		EndDriver_D3D();
		MonitorsOn();
	}
	else
	{ // none
		ASSERT( gl_eCurrentAPI==GAT_NONE);
	}

	// free occlusion tests memory
	if( _pslOcclusionQueryFrames!=NULL) {
		FreeMemory(_pslOcclusionQueryFrames);
		_pslOcclusionQueryFrames = NULL;
		_pulOcclusionQueryOwners = NULL;
		_ctUsedOcclusionQueries  = 0;
	}

	// free driver DLL
	if( gl_hiDriver!=NONE) FreeLibrary(gl_hiDriver);
	gl_hiDriver = NONE;

	// reset some vars
	_iGfxAPI = -1;  // just for some color.h routines
	gl_eCurrentAPI = GAT_NONE;
	gl_ctRealTextureUnits = 0;
	gl_pvpActive = NULL;
	gl_ulFlags &= GLF_ADJUSTABLEGAMMA;

	// reset function pointers
	GFX_SetFunctionPointers( (INDEX)GAT_NONE);
}



// prepare current viewport for rendering
BOOL CGfxLibrary::SetCurrentViewport(CViewPort *pvp)
{
	if( gl_eCurrentAPI==GAT_OGL)  return SetCurrentViewport_OGL(pvp);
	if( gl_eCurrentAPI==GAT_D3D)  return SetCurrentViewport_D3D(pvp);
	if( gl_eCurrentAPI==GAT_NONE) return TRUE;
	ASSERTALWAYS( "SetCurrenViewport: Wrong API!");
	return FALSE;
}


// Lock a drawport for drawing
BOOL CGfxLibrary::LockDrawPort( CDrawPort *pdpToLock)
{
	// check API
	ASSERT( gl_eCurrentAPI==GAT_OGL || gl_eCurrentAPI==GAT_D3D || gl_eCurrentAPI==GAT_NONE);

	// don't set if same as last
	const ULONG ulThisDrawPortID = pdpToLock->GetID();
	if( GFX_ulLastDrawPortID==ulThisDrawPortID && gap_bOptimizeStateChanges) {
		// just set projection
		pdpToLock->SetOrtho();
		return TRUE;
	}

	// OpenGL ...
	if( gl_eCurrentAPI==GAT_OGL)
	{ 
		// pass drawport dimensions to OpenGL
		const PIX pixMinSI = pdpToLock->dp_ScissorMinI;
		const PIX pixMaxSI = pdpToLock->dp_ScissorMaxI;
		const PIX pixMinSJ = pdpToLock->dp_Raster->ra_Height - pdpToLock->dp_ScissorMaxJ;
		const PIX pixMaxSJ = pdpToLock->dp_Raster->ra_Height - pdpToLock->dp_ScissorMinJ;
		pglViewport( pixMinSI, pixMinSJ, pixMaxSI-pixMinSI, pixMaxSJ-pixMinSJ);
		pglScissor(  pixMinSI, pixMinSJ, pixMaxSI-pixMinSI, pixMaxSJ-pixMinSJ);
		OGL_CHECKERROR;
	}
	// Direct3D ...
	else if( gl_eCurrentAPI==GAT_D3D)
	{ 
		// set viewport
		PIX pixMinSI = pdpToLock->dp_ScissorMinI;
		PIX pixMaxSI = pdpToLock->dp_ScissorMaxI;
		PIX pixMinSJ = pdpToLock->dp_ScissorMinJ;
		PIX pixMaxSJ = pdpToLock->dp_ScissorMaxJ;
		D3DVIEWPORT8 d3dViewPort = { pixMinSI, pixMinSJ, pixMaxSI-pixMinSI, pixMaxSJ-pixMinSJ, 0,1 };
		if( pixMaxSI<=pixMinSI || pixMaxSJ<=pixMinSJ) {
			d3dViewPort.X = 0;
			d3dViewPort.Y = 0;
			d3dViewPort.Width  = 1;
			d3dViewPort.Height = 1;
		}
		HRESULT hr = gl_pd3dDevice->SetViewport( &d3dViewPort);
		D3D_CHECKERROR(hr);
		// update for wrapper
		extern FLOAT GFX_fMinDepthRange;
		extern FLOAT GFX_fMaxDepthRange;
		GFX_fMinDepthRange = 0.0f;
		GFX_fMaxDepthRange = 1.0f;
	}

	// mark and set default projection
	GFX_ulLastDrawPortID = ulThisDrawPortID;
	pdpToLock->SetOrtho();
	return TRUE;
}



// Unlock a drawport after drawing
void CGfxLibrary::UnlockDrawPort( CDrawPort *pdpToUnlock)
{
	// check API
	ASSERT( gl_eCurrentAPI==GAT_OGL || gl_eCurrentAPI==GAT_D3D || gl_eCurrentAPI==GAT_NONE);
	// eventually signalize that scene rendering has ended
}


/////////////////////////////////////////////////////////////////////
// Window canvas functions

/* Create a new window canvas. */
void CGfxLibrary::CreateWindowCanvas(void *hWnd, CViewPort **ppvpNew, CDrawPort **ppdpNew)
{

	RECT rectWindow;	// rectangle for the client area of the window

	// get the dimensions from the window
	GetClientRect( (HWND)hWnd, &rectWindow);
	PIX pixWidth  = rectWindow.right  - rectWindow.left;
	PIX pixHeight = rectWindow.bottom - rectWindow.top;

	*ppvpNew = NULL;
	*ppdpNew = NULL;
	// create a new viewport
	if (*ppvpNew = new CViewPort( pixWidth, pixHeight, (HWND)hWnd)) {
		// and it's drawport
		*ppdpNew = &(*ppvpNew)->vp_Raster.ra_MainDrawPort;
	} else {
		delete *ppvpNew;
		*ppvpNew = NULL;
	}
}

/* Destroy a window canvas. */
void CGfxLibrary::DestroyWindowCanvas(CViewPort *pvpOld) {
	// delete the viewport
	delete pvpOld;
}

/////////////////////////////////////////////////////////////////////
// Work canvas functions

#define WorkCanvasCLASS "WorkCanvas Window"
static BOOL _bClassRegistered = FALSE;

/* Create a work canvas. */
void CGfxLibrary::CreateWorkCanvas(PIX pixWidth, PIX pixHeight, CDrawPort **ppdpNew)
{
	// 김영환 : 클라이언트에서 사용안함 아마 툴에서 하용할 듯.
	// must have dimensions
	ASSERT (pixWidth>0 || pixHeight>0);

	if (!_bClassRegistered) {
		_bClassRegistered = TRUE;
		WNDCLASS wc;

		// must have owndc for opengl and dblclks to give to parent
		wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wc.lpfnWndProc = DefWindowProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = NULL;
		wc.hIcon = NULL;
		wc.hCursor = LoadCursor( NULL, IDC_ARROW );
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = WorkCanvasCLASS;
		RegisterClass(&wc);
	}

	// create a window
	HWND hWnd = ::CreateWindowEx(
	  0,
	  WorkCanvasCLASS,
	  "",   // title
		WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_POPUP,
	  0,0,
	  pixWidth, pixHeight,  // window size
	  NULL,
	  NULL,
	  NULL, //hInstance,
	  NULL);
	ASSERT(hWnd != NULL);

	*ppdpNew = NULL;
	CViewPort *pvp;
	CreateWindowCanvas(hWnd, &pvp, ppdpNew);
}

/* Destroy a work canvas. */
void CGfxLibrary::DestroyWorkCanvas(CDrawPort *pdpOld)
{
	CViewPort *pvp = pdpOld->dp_Raster->ra_pvpViewPort;
	HWND hwnd = pvp->vp_hWndParent;
	DestroyWindowCanvas(pvp);
	//	김영환 클라이언트에서 사용안하는 함수 툴에서 사용할 듯
	if(_bClientApp)
		return; 
	::DestroyWindow(hwnd);
}




// some vars for probing
extern INDEX _ctProbeTexs = 0;
extern INDEX _ctProbeShdU = 0;
extern INDEX _ctProbeShdB = 0;
extern INDEX _ctFullShdU  = 0;
extern SLONG _slFullShdUBytes = 0;
static void AdjustGammaRamp( CViewPort *pvp);
static FLOAT _fMinAvailPhys = UpperLimit(1.0f);


//안태훈 수정 시작	//(For Win98)(0.1)
//////////////////////////////////////////////////////////
//  Function Name  
//      GetOSVersionType
//
//  Parameters 
//
//  Return Values
//      설치된 Windows 버전을 return
//      -1: Failed
//      1 : Windows 95
//      2 : Windows 98
//      3 : Windows ME
//      4 : Windows NT
//      5 : Windows 2000
//      6 : Windows XP
static int WINAPI GetOSVersionType() 
{
    UINT nOSVersion;
    OSVERSIONINFOEX osvi;
    BOOL bOsVersionInfoEx;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    
    if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
    {
        osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
        if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
            return -1;
    }
    
    switch (osvi.dwPlatformId)
    {
    case VER_PLATFORM_WIN32_NT:
        // Test for the product.
        if ( osvi.dwMajorVersion <= 4 )
            nOSVersion = 4;
        else if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
            nOSVersion = 5;
        else if( bOsVersionInfoEx )  
            nOSVersion = 6;
        break;
    case VER_PLATFORM_WIN32_WINDOWS:
        if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
            nOSVersion = 1;
        else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
            nOSVersion = 2;
        else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
            nOSVersion = 3;
        break;
    }
    
    return nOSVersion; 
}
//안태훈 수정 끝	//(For Win98)(0.1)
 
/*
 * swap buffers in a viewport
 */

void CGfxLibrary::SwapBuffers( CViewPort *pvp, const BOOL bWaitForRetrace/*=FALSE*/)
{
	// check API
	ASSERT( gl_eCurrentAPI==GAT_OGL || gl_eCurrentAPI==GAT_D3D || gl_eCurrentAPI==GAT_NONE);

	// safety check
	ASSERT( gl_pvpActive!=NULL);
	if( pvp!=gl_pvpActive) {
		ASSERTALWAYS( "Swapping viewport that was not last drawn to!");
		return;
	}

	_sfStats.StartTimer(CStatForm::STI_SWAPBUFFERS);

	// eventual printout of debug string
	if( _strDebug!="" && _pfdConsoleFont!=NULL) {
		CDrawPort &dp = pvp->vp_Raster.ra_MainDrawPort;
		dp.SetAsCurrent();
		dp.SetFont( _pfdConsoleFont);
		dp.PutText( _strDebug, _pixDebugStringX+1, _pixDebugStringY+1, C_BLACK|200);
		dp.PutText( _strDebug, _pixDebugStringX,   _pixDebugStringY,  _colDebugString);
	//_strDebug = "";
	}
	
	// eventual printout of free memory
	if( dbg_bShowFreeMemory && _pfdDisplayFont!=NULL) {

		MEMORYSTATUS ms;
		GlobalMemoryStatus(&ms);
		CTString strFreeMem;
		#define KB (1024.0f)
		#define MB (KB*KB)

		FLOAT fAvailPhys = ms.dwAvailPhys/KB;
		if(fAvailPhys<_fMinAvailPhys) {
			_fMinAvailPhys = fAvailPhys;
			_tmLastPeak = _pTimer->GetHighPrecisionTimer().GetSeconds();
		}

		if (ms.dwAvailPhys>=MB) {
			strFreeMem.PrintF("free: %.2fMB", fAvailPhys/KB);
		} else {
			strFreeMem.PrintF("free: %.2fkB", fAvailPhys);
		}
		if (_fMinAvailPhys>3145) {
			strFreeMem += CTString(0,"\npeak: %.2fMB",_fMinAvailPhys/KB);      
		} else {      
			FLOAT tmDelta = _pTimer->GetHighPrecisionTimer().GetSeconds()-_tmLastPeak;
			INDEX iBlink = (INDEX)(4.0f*tmDelta);
			if (tmDelta<5.0f && iBlink%2) {
				strFreeMem += CTString(0,"\n^cff0000WARNING! ^cffffffpeak: %.2fMB",_fMinAvailPhys/KB);        
			} else {
				strFreeMem += CTString(0,"\n^cffffffWARNING! ^cffffffpeak: %.2fMB",_fMinAvailPhys/KB);
			}
		}
		// if peak lower then 3 mb and new peak occurs, flash for five seconds

		CDrawPort &dp = pvp->vp_Raster.ra_MainDrawPort;
		dp.SetAsCurrent();
		dp.SetFont( _pfdDisplayFont);
		dp.SetTextScaling(0.5f);
		dp.PutText( strFreeMem, 50, 30, C_WHITE|CT_OPAQUE);
	}
																													
	// optimize memory used by cached shadow maps and update shadowmap counters
	ReduceShadows();

	// remove old occlusion queries
	extern void RemoveOldOcclusionQueries(void);
	RemoveOldOcclusionQueries();

	// check and eventually adjust texture filtering and LOD biasing
	gfxSetTextureFiltering( gap_iTextureFiltering, gap_iTextureAnisotropy);
	gfxSetTextureBiasing( gap_fTextureLODBias);

	// clamp some cvars
	gap_iDithering = Clamp( gap_iDithering, 0L, 2L);
	gap_iSwapInterval = Clamp( gap_iSwapInterval, 0L, 3L);
	gap_iOptimizeClipping = Clamp( gap_iOptimizeClipping, 0L, 2L);
	gap_iTruformLevel = Clamp( gap_iTruformLevel, 0L, gl_iMaxTessellationLevel);
	ogl_iFinish = Clamp( ogl_iFinish, 0L, 3L);
	d3d_iFinish = Clamp( d3d_iFinish, 0L, 3L);
	if( ogl_bHardFinish) ogl_bHardFinish = TRUE;

	// clamp occlusion culling usage
	const INDEX bCullModels  = gap_bUseOcclusionCulling%10;
	const INDEX bCullFlares  = gap_bUseOcclusionCulling/10;
	gap_bUseOcclusionCulling = bCullModels ? 1 : 0;
	if( bCullFlares) gap_bUseOcclusionCulling += 10;

	// OpenGL  
	if( gl_eCurrentAPI==GAT_OGL)
	{
		// force finishing of all rendering operations (if required)
		if( ogl_iFinish==2) gfxFinish(ogl_bHardFinish);

		// check state of swap interval extension usage
		if( gl_ulFlags & GLF_VSYNC) {
			if( gl_iSwapInterval != gap_iSwapInterval) {
				gl_iSwapInterval = gap_iSwapInterval;
				pwglSwapIntervalEXT( gl_iSwapInterval);
			}
			// check for forced wait-for-vertical-retrace
			static BOOL bLastWaitForRetrace = FALSE;
			if( gl_iSwapInterval==0 && !bLastWaitForRetrace!=!bWaitForRetrace) {
				const INDEX iInterval = bWaitForRetrace ? 1 : 0;
				pwglSwapIntervalEXT( gl_iSwapInterval);
				bLastWaitForRetrace = bWaitForRetrace;
			}
		}

		// swap buffers
		CTempDC tdc(pvp->vp_hWnd);
		pwglSwapBuffers(tdc.hdc);

		// force finishing of all rendering operations (if required)
		if( ogl_iFinish==3) gfxFinish(ogl_bHardFinish);

		// reset CVA usage if ext is not present
		if( !(gl_ulFlags&GLF_EXT_COMPILEDVERTEXARRAY)) ogl_bUseCompiledVertexArrays = 0;
	}

	// Direct3D
	else if( gl_eCurrentAPI==GAT_D3D)
	{
		// force finishing of all rendering operations (if required)
		if( d3d_iFinish==2) gfxFinish(FALSE);

		// end scene rendering
		HRESULT hr;
		if( GFX_bRenderingScene) {
			hr = gl_pd3dDevice->EndScene(); 
			D3D_CHECKERROR(hr);
		}
		CDisplayMode dm;
		GetCurrentDisplayMode(dm);
		ASSERT( (dm.dm_pixSizeI==0 && dm.dm_pixSizeJ==0) || (dm.dm_pixSizeI!=0 && dm.dm_pixSizeJ!=0));

//안태훈 수정 시작	//(For Win98)(0.1)
		//if(GetOSVersionType() > 3)
		{
			// PC can swap in windowed mode
			if( dm.dm_pixSizeI==0 || dm.dm_pixSizeJ==0)
			{ // windowed mode
				if( pvp->vp_pSwapChain != NULL)
				{
					if (g_web.IsWebHandle())
					{
						int iW, iH;
						RECT rcMain;
						GetClientRect(_hwndMain, &rcMain);
						iW = rcMain.right;
						iH = rcMain.bottom;

						int nWebPosX, nWebPosY, nWebWidth, nWebHeight;
						g_web.GetPos(nWebPosX, nWebPosY);
						g_web.GetSize(nWebWidth, nWebHeight);

						RECT rcTop  = {0, 0, iW, nWebPosY};   
						RECT rcBottom   = {0, nWebPosY + nWebHeight, iW, iH};   
						RECT rcLeft = {0, nWebPosY, nWebPosX, nWebPosY + nWebHeight};   
						RECT rcRight    = {nWebPosX + nWebWidth, nWebPosY, iW, nWebPosY + nWebHeight};

						hr = pvp->vp_pSwapChain->Present(&rcTop, &rcTop, _hwndMain, NULL);
						if (!FAILED(hr)) {
							pvp->vp_pSwapChain->Present(&rcBottom, &rcBottom, _hwndMain, NULL);
						}
						if (!FAILED(hr)) {
							pvp->vp_pSwapChain->Present(&rcLeft, &rcLeft, _hwndMain, NULL);
						}
						if (!FAILED(hr)) {
							pvp->vp_pSwapChain->Present(&rcRight, &rcRight, _hwndMain, NULL);
						}
					}
					else
					{
						hr = pvp->vp_pSwapChain->Present( NULL, NULL, NULL, NULL);
					}
				}
			} else
			{ // full screen mode
				if (g_web.IsWebHandle())
				{
					int iW, iH;
					RECT rcMain;
					GetClientRect(_hwndMain, &rcMain);
					iW = rcMain.right;
					iH = rcMain.bottom;

					int nWebPosX, nWebPosY, nWebWidth, nWebHeight;
					g_web.GetPos(nWebPosX, nWebPosY);
					g_web.GetSize(nWebWidth, nWebHeight);

					RECT rcTop  = {0, 0, iW, nWebPosX};   
					RECT rcBottom   = {0, nWebPosY + nWebHeight, iW, iH};   
					RECT rcLeft = {0, nWebPosY, nWebPosX, nWebHeight};   
					RECT rcRight    = {nWebPosX + nWebWidth, nWebPosY, iW, nWebPosY + nWebHeight};

					hr = gl_pd3dDevice->Present(&rcTop, &rcTop, _hwndMain, NULL);  
					if (!FAILED(hr)) {
						hr = gl_pd3dDevice->Present(&rcBottom, &rcBottom, _hwndMain, NULL);  
					}
					if (!FAILED(hr)) {
						hr = gl_pd3dDevice->Present(&rcLeft, &rcLeft, _hwndMain, NULL);
					}
					if (!FAILED(hr)) {
						hr = gl_pd3dDevice->Present(&rcRight, &rcRight, _hwndMain, NULL);
					}
				}
				else
				{
					hr = gl_pd3dDevice->Present( NULL, NULL, NULL, NULL);
				}
			} // done swapping


			//D3D_CHECKERROR(hr);
			// Is it possible to restore this device?
			// 임의 검사를 위해서는 IDirect3DDevice::TestCooperativeLevel()를 호출해야한다.
 			if (FAILED(hr))
 			{
				if (D3DERR_DEVICELOST == hr)
				{
					for (;;)
					{
						if (D3DERR_DEVICELOST == hr)
						{ // 디바이스를 소실했고, 복구가 불가능 상태
							Sleep(500);
							hr = gl_pd3dDevice->TestCooperativeLevel(); // 복구 가능 상태까지 대기 한다.
						}
						else if (D3DERR_DEVICENOTRESET == hr)
						{ // 디바이스를 소실했지만, 복구가 가능한 경우
							if( CUIManager::isCreated() == true )
							{
								CUIManager::getSingleton()->GetOption()->ResetDisplay(); // 복구 하자.
								break;
							}
						}
					}
				}
			}
		}
//안태훈 수정 끝	//(For Win98)(0.1)

		// force finishing of all rendering operations (if required)
		if( d3d_iFinish==3) gfxFinish(FALSE);

		// eventually reset vertex buffer if something got changed
		if( _iLastVertexBufferSize!=d3d_iVertexBuffersSize
		|| (gl_iTessellationLevel<1 && gap_iTruformLevel>0)
		|| (gl_iTessellationLevel>0 && gap_iTruformLevel<1)) {
			extern void SetupVertexArrays_D3D( INDEX ctVertices);
			extern void SetupIndexArray_D3D( INDEX ctVertices);
			extern DWORD SetupShader_D3D( ULONG ulStreamsMask);
			SetupShader_D3D(NONE); 
			SetupVertexArrays_D3D(0); 
			SetupIndexArray_D3D(0);
			d3d_iVertexBuffersSize = (d3d_iVertexBuffersSize+3) & (~3); // round to 4
			d3d_iVertexBuffersSize = Clamp( d3d_iVertexBuffersSize, 64L, 2048L);
			extern INDEX VerticesFromSize_D3D( SLONG slSizeKB);
			const INDEX ctVertices = VerticesFromSize_D3D(d3d_iVertexBuffersSize);
			SetupVertexArrays_D3D(ctVertices); 
			SetupIndexArray_D3D(3*ctVertices);
		 _iLastVertexBufferSize = d3d_iVertexBuffersSize;
		} 

	}

	// update tessellation level
	gl_iTessellationLevel = gap_iTruformLevel;

	// must reset drawport and rendering status for subsequent locks
	extern void DrawPort_NoCurrentDrawPort(void);
	DrawPort_NoCurrentDrawPort();
	GFX_ulLastDrawPortID = 0;  
	GFX_bRenderingScene  = FALSE;

	// clear viewport if needed
	if( gfx_bClearScreen) {
		pvp->vp_Raster.ra_MainDrawPort.SetAsCurrent();
		pvp->vp_Raster.ra_MainDrawPort.Fill( C_BLACK|CT_OPAQUE);
	//pvp->vp_Raster.ra_MainDrawPort.FillZBuffer(ZBUF_BACK);  // for debugging purposes only (makes lens-flares to stop working!)
	}

	// reset frustum/ortho matrix, too
	extern BOOL  GFX_bViewMatrix;
	extern FLOAT GFX_fLastL, GFX_fLastR, GFX_fLastT, GFX_fLastB, GFX_fLastN, GFX_fLastF;
	GFX_fLastL = GFX_fLastR = GFX_fLastT = GFX_fLastB = GFX_fLastN = GFX_fLastF = 0;
	GFX_bViewMatrix = TRUE;

	// set maximum allowed upload ammount
	gfx_iProbeSize = Clamp( gfx_iProbeSize, 1L, 16384L);
	gl_slAllowedUploadBurst = gfx_iProbeSize *1024; 
	_ctProbeTexs = 0;
	_ctProbeShdU = 0;
	_ctProbeShdB = 0;
	_ctFullShdU  = 0;
	_slFullShdUBytes = 0;

	// keep time when swap buffer occured and maintain counter of frames for temporal coherence checking
	gl_tvFrameTime = _pTimer->GetHighPrecisionTimer();
	gl_iFrameNumber++;
	// reset profiling counters
	gl_ctWorldElements     = 0;
	gl_ctModelElements     = 0;
	gl_ctParticleTriangles = 0;
	gl_ctTotalElements     = 0;

	// re-adjust multi-texturing support
	gap_iUseTextureUnits = Clamp( gap_iUseTextureUnits, 1L, gl_ctTextureUnits);
	ASSERT( gap_iUseTextureUnits>=1 && gap_iUseTextureUnits<=GFX_MAXTEXUNITS);

	// re-get usage of compiled vertex arrays
	CVA_b2D     = ogl_bUseCompiledVertexArrays /100;
	CVA_bWorld  = ogl_bUseCompiledVertexArrays /10 %10;
	CVA_bModels = ogl_bUseCompiledVertexArrays %10;    
	ogl_bUseCompiledVertexArrays = 0;
	if( CVA_b2D)     ogl_bUseCompiledVertexArrays += 100;
	if( CVA_bWorld)  ogl_bUseCompiledVertexArrays += 10;
	if( CVA_bModels) ogl_bUseCompiledVertexArrays += 1;

	// adjust gamma ramp table (well, if supported)
	AdjustGammaRamp(pvp);

	// all done
 _sfStats.StopTimer(CStatForm::STI_SWAPBUFFERS);
}



// get array of all supported display modes
CDisplayMode *CGfxLibrary::EnumDisplayModes( INDEX &ctModes, enum GfxAPIType eAPI/*=GAT_CURRENT*/, INDEX iAdapter/*=0*/)
{
	if( eAPI==GAT_CURRENT) eAPI = gl_eCurrentAPI;
	if( iAdapter==0) iAdapter = gl_iCurrentAdapter;
	CDisplayAdapter *pda = &gl_gaAPI[eAPI].ga_adaAdapter[iAdapter];
	ctModes = pda->da_ctDisplayModes;
	return &pda->da_admDisplayModes[0];
}


// Lock a raster for drawing.
BOOL CGfxLibrary::LockRaster( CRaster *praToLock)
{
	// don't do this! it can break sync consistency in entities!
	// SetFPUPrecision(FPT_24BIT); 
	ASSERT( praToLock->ra_pvpViewPort!=NULL);
	BOOL bRes = SetCurrentViewport( praToLock->ra_pvpViewPort);
	if( bRes) {
		// must signal to picky Direct3D
		if( gl_eCurrentAPI==GAT_D3D && !GFX_bRenderingScene) {  
			HRESULT hr = gl_pd3dDevice->BeginScene(); 
			D3D_CHECKERROR(hr);
			bRes = (hr==D3D_OK);
		} // mark it
		GFX_bRenderingScene = TRUE;
	} // done
	return bRes;
}


// Unlock a raster after drawing.
void CGfxLibrary::UnlockRaster( CRaster *praToUnlock)
{
	// don't do this! it can break sync consistency in entities!
	// SetFPUPrecision(FPT_53BIT);
	ASSERT( GFX_bRenderingScene);
}



// has this driver support for S3TC/DXT1 compression? (not FXT1!)
BOOL CGfxLibrary::HasCompression(void)
{
	// check API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D);
	// OpenGL: ARB & non-legacy-ST3C is required
	if( eAPI==GAT_OGL && (gl_ulFlags&GLF_EXTC_ARB) && (gl_ulFlags&GLF_EXTC_S3TC)) return TRUE;
	// Direct3D: just plain compression support is needed
	if( eAPI==GAT_D3D && (gl_ulFlags&GLF_TEXTURECOMPRESSION)) return TRUE;
	// no API - no compression
	return FALSE;
}



// generates gamma table and returns true if gamma table has been changed
static void AdjustGammaRamp( CViewPort *pvp)
{
	// if gamma adjustment is not supported
	if( !(_pGfx->gl_ulFlags & GLF_ADJUSTABLEGAMMA)) {
		// just reset settings to default
		gfx_fBrightness = 0;
		gfx_fGamma = gfx_fContrast = 1;
		gfx_fBiasR = gfx_fBiasG = gfx_fBiasB = 1;
		gfx_iLevels = 256;
		return;
	}

	// if nothing changed
	if( _fLastBrightness==gfx_fBrightness && _fLastContrast==gfx_fContrast
	 && _fLastGamma     ==gfx_fGamma      && _iLastLevels  ==gfx_iLevels
	 && _fLastBiasR==gfx_fBiasR && _fLastBiasG==gfx_fBiasG && _fLastBiasB==gfx_fBiasB) {
		// skip adjustment
		return;
	}

	// clamp values
	gfx_fBrightness = Clamp( gfx_fBrightness, -0.8f, 0.8f);
	gfx_fContrast   = Clamp( gfx_fContrast,    0.2f, 4.0f);
	gfx_fGamma      = Clamp( gfx_fGamma,  0.2f, 4.0f);    
	gfx_iLevels = Clamp( gfx_iLevels, 2L,  256L);
	gfx_fBiasR  = Clamp( gfx_fBiasR, 0.0f, 2.0f);
	gfx_fBiasG  = Clamp( gfx_fBiasG, 0.0f, 2.0f);
	gfx_fBiasB  = Clamp( gfx_fBiasB, 0.0f, 2.0f);
	// update
	_fLastBrightness = gfx_fBrightness;
	_fLastContrast   = gfx_fContrast;
	_fLastGamma      = gfx_fGamma;
	_iLastLevels = gfx_iLevels;
	_fLastBiasR  = gfx_fBiasR;
	_fLastBiasG  = gfx_fBiasG;
	_fLastBiasB  = gfx_fBiasB;
	
	// fill in the table
	INDEX i;
	const FLOAT fLevels   = 256.0f * 256.0f/gfx_iLevels;
	const FLOAT f1oLevels = 1.0f / fLevels;
	const FLOAT f1oGamma  = 1.0f / gfx_fGamma;
	const FLOAT f1o65535  = 1.0f / 65535.0f;
	for( i=0; i<256; i++) {
		FLOAT fVal = i/255.0f;
		fVal  = (FLOAT)pow(fVal,f1oGamma);                                // gamma
		fVal  = (fVal-0.5f)*gfx_fContrast +0.5f;                          // contrast
		fVal += gfx_fBrightness;                                          // brightness
		fVal  = ((INDEX)(fVal*65535.0f*f1oLevels)) *fLevels *f1o65535;    // levels
		const FLOAT fR = Clamp( fVal*gfx_fBiasR, 0.0f, 1.0f);             // bias
		const FLOAT fG = Clamp( fVal*gfx_fBiasG, 0.0f, 1.0f);
		const FLOAT fB = Clamp( fVal*gfx_fBiasB, 0.0f, 1.0f);
		_auwGammaTable[i+0]   = (UWORD)(fR*65535);
		_auwGammaTable[i+256] = (UWORD)(fG*65535);
		_auwGammaTable[i+512] = (UWORD)(fB*65535);
	}

	// apply!

	if( _pGfx->gl_eCurrentAPI==GAT_OGL) {
		// OpenGL
		ASSERT( pvp!=NULL);
		CTempDC tdc(pvp->vp_hWnd);
		SetDeviceGammaRamp( tdc.hdc, &_auwGammaTable[0]);
	}
	else if( _pGfx->gl_eCurrentAPI==GAT_D3D) {
		// Direct3D
		_pGfx->gl_pd3dDevice->SetGammaRamp( D3DSGR_NO_CALIBRATION, (D3DGAMMARAMP*)&_auwGammaTable[0]);
	}

}
