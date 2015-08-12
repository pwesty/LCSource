#include "stdh.h"

#include <Engine/Graphics/Texture.h>

#include <Engine/Base/Stream.h>
#include <Engine/Base/Timer.h>
#include <Engine/Base/Console.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Math/Functions.h>
#include <Engine/Graphics/GfxLibrary.h>
#include <Engine/Graphics/ImageInfo.h>
#include <Engine/Graphics/TextureEffects.h>

#include <Engine/Templates/DynamicArray.h>
#include <Engine/Templates/DynamicArray.cpp>
#include <Engine/Templates/Stock_CtextureData.h>
#include <Engine/Templates/StaticArray.cpp>

#include <Engine/Base/Statistics_internal.h>

#ifdef KALYDO
#include <Kalydo/KRFReadLib/Include/KRFReadLib.h>
#endif

#include <3rdparty/lpng163/png.h>

// track texture memory allocated in main heap
#define TRACKTEX_HEAP() TRACKMEM(mem, "Textures (heap)")
// track texture memory allocated in hardware
#define TRACKTEX_HW() TRACKMEM(mem, "Textures (hardware)")


// (texture compression needs this)
#if defined(_MSC_VER) && (_MSC_VER < 1600)
#	include <Engine/Graphics/dxtlib.h>
#	pragma comment(lib, "nvDXTlib.lib")
#endif	// _MSC_VER


// cvars
extern INDEX tex_iNormalQuality;
extern INDEX tex_iAnimationQuality;
extern INDEX tex_iNormalSize;
extern INDEX tex_iAnimationSize;
extern INDEX tex_iEffectSize;

extern INDEX tex_iDithering;
extern INDEX tex_iFiltering;       

extern FLOAT gfx_tmProbeDecay;


// singleton object for texture settings
struct TextureSettings TS = {0};

#ifdef KALYDO
CTString CTextureData::strDefaultTexturePath = "data\\defaults\\default.tex";
#endif

#define TEXFMT_NONE 0

// determine (or assume) support for OpenGL and Direct3D texture internal formats
extern void DetermineSupportedTextureFormats( GfxAPIType eAPI)
{
	if( eAPI==GAT_OGL) 
	{
		TS.ts_tfRGB8   = GL_RGB8;
		TS.ts_tfRGBA8  = GL_RGBA8;
		TS.ts_tfRGB5   = GL_RGB5;
		TS.ts_tfRGBA4  = GL_RGBA4;
		TS.ts_tfRGB5A1 = GL_RGB5_A1;
		TS.ts_tfLA8    = GL_LUMINANCE8_ALPHA8;
		TS.ts_tfL8     = GL_LUMINANCE8;
		TS.ts_tfA8     = GL_ALPHA8;
	}
	if( eAPI==GAT_D3D) 
	{
		extern D3DFORMAT FindClosestFormat_D3D(D3DFORMAT d3df);
		TS.ts_tfRGBA8  = FindClosestFormat_D3D(D3DFMT_A8R8G8B8);
		TS.ts_tfRGB8   = FindClosestFormat_D3D(D3DFMT_X8R8G8B8);
		TS.ts_tfRGB5   = FindClosestFormat_D3D(D3DFMT_R5G6B5);
		TS.ts_tfRGB5A1 = FindClosestFormat_D3D(D3DFMT_A1R5G5B5);
		TS.ts_tfRGBA4  = FindClosestFormat_D3D(D3DFMT_A4R4G4B4);
		TS.ts_tfLA8    = FindClosestFormat_D3D(D3DFMT_A8L8);
		TS.ts_tfL8     = FindClosestFormat_D3D(D3DFMT_L8);
		TS.ts_tfA8     = FindClosestFormat_D3D(D3DFMT_A8);
	}
}


// update all relevant texture parameters
extern void UpdateTextureSettings(void)
{
	// determine API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);
	
	// set texture formats and compression
	TS.ts_tfRGB8 = TS.ts_tfRGBA8 = NONE;
	TS.ts_tfRGB5 = TS.ts_tfRGBA4 = TS.ts_tfRGB5A1 = NONE;
	TS.ts_tfLA8  = TS.ts_tfL8 = TS.ts_tfA8 = NONE;
	DetermineSupportedTextureFormats(eAPI);
	
	// readout supported texture compression
	INDEX iTCType = 0;
	const ULONG ulGfxFlags = _pGfx->gl_ulFlags;
	const BOOL bAnyTC = (ulGfxFlags & GLF_TEXTURECOMPRESSION); 
	const BOOL bARBC  = (ulGfxFlags & GLF_EXTC_ARB);
	const BOOL bS3TC  = (ulGfxFlags & GLF_EXTC_S3TC);
	const BOOL bFXT1  = (ulGfxFlags & GLF_EXTC_FXT1);
	const BOOL bOldTC = (ulGfxFlags & GLF_EXTC_LEGACY);
	
	// clamp and adjust texture compression type
	if( eAPI==GAT_OGL && bAnyTC)
	{ // OpenGL
		extern INDEX ogl_iTextureCompressionType;  // 0=none, 1=default (ARB), 2=S3TC, 3=FXT1, 4=legacy S3TC
		INDEX &iTC = ogl_iTextureCompressionType;
		iTC = Clamp( iTC, 0L, 4L);
		if( iTC==3 && !bFXT1) iTC = 2;
		if( iTC==2 && !bS3TC) iTC = 3;
		if((iTC==2 || iTC==3) && !(bFXT1 || bS3TC))  iTC = 1;
		if( iTC==1 && !bARBC)  iTC = 4;
		if( iTC==4 && !bOldTC) iTC = 0; // khm ... 
		iTCType = iTC;   // set it
	}
	// Direct3D (just force DXTC - it's the only one)
	if( eAPI==GAT_D3D && bAnyTC) iTCType = 5;
	
	// set members
	switch( iTCType) 
	{
	case 1:  // ARB
		TS.ts_tfRGBc   = GL_COMPRESSED_RGB_ARB;
		TS.ts_tfRGBcA  = GL_COMPRESSED_RGBA_ARB;
		TS.ts_tfRGBcAc = GL_COMPRESSED_RGBA_ARB;
		if( bS3TC)  TS.ts_tfRGBcAc = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		else if( bOldTC) TS.ts_tfRGBcAc = GL_COMPRESSED_RGB4_COMPRESSED_ALPHA4_S3TC;
		break;  
	case 2:  // S3TC
		TS.ts_tfRGBc   = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
		TS.ts_tfRGBcA  = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		TS.ts_tfRGBcAc = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;  
	case 3:  // FXT1
		TS.ts_tfRGBc   = GL_COMPRESSED_RGB_FXT1_3DFX;
		TS.ts_tfRGBcA  = GL_COMPRESSED_RGBA_FXT1_3DFX;
		TS.ts_tfRGBcAc = GL_COMPRESSED_RGBA_FXT1_3DFX;
		break;  
	case 4:  // LEGACY
		TS.ts_tfRGBc   = GL_COMPRESSED_RGB_S3TC;
		TS.ts_tfRGBcA  = GL_COMPRESSED_RGB4_ALPHA4_S3TC;
		TS.ts_tfRGBcAc = GL_COMPRESSED_RGB4_COMPRESSED_ALPHA4_S3TC;
		break;
	case 5:  // DXTC
		extern D3DFORMAT FindClosestFormat_D3D(D3DFORMAT d3df);
		TS.ts_tfRGBc   = D3DFMT_DXT1;
		TS.ts_tfRGBcA  = FindClosestFormat_D3D(D3DFMT_DXT3);
		TS.ts_tfRGBcAc = FindClosestFormat_D3D(D3DFMT_DXT5);
		break;
	default: // none
		TS.ts_tfRGBc   = NONE;
		TS.ts_tfRGBcA  = NONE;
		TS.ts_tfRGBcAc = NONE;
		ASSERT(FALSE);
		break;
	}
	
	// clamp texture quality
	INDEX iMinQuality = 0;
	INDEX iMaxQuality = iTCType>0 ? 3 : 2;
	if( !(_pGfx->gl_ulFlags&GLF_32BITTEXTURES)) iMinQuality = iMaxQuality = 1;
	TS.ts_iNormQualityO  = Clamp( (INDEX)(tex_iNormalQuality   /10), iMinQuality, iMaxQuality); 
	TS.ts_iNormQualityA  = Clamp( (INDEX)(tex_iNormalQuality   %10), iMinQuality, iMaxQuality); 
	TS.ts_iAnimQualityO  = Clamp( (INDEX)(tex_iAnimationQuality/10), iMinQuality, iMaxQuality); 
	TS.ts_iAnimQualityA  = Clamp( (INDEX)(tex_iAnimationQuality%10), iMinQuality, iMaxQuality); 
	tex_iNormalQuality    = TS.ts_iNormQualityO*10 + TS.ts_iNormQualityA;
	tex_iAnimationQuality = TS.ts_iAnimQualityO*10 + TS.ts_iAnimQualityA;
	// clamp texture size
	tex_iNormalSize    = Clamp( tex_iNormalSize,    5L, 11L);
	tex_iAnimationSize = Clamp( tex_iAnimationSize, 5L,  9L);
	TS.ts_pixNormSize = 1L<<(tex_iNormalSize   *2);
	TS.ts_pixAnimSize = 1L<<(tex_iAnimationSize*2);
	
	// determine maximum texel-byte ratio
	INDEX iOQ = tex_iNormalQuality /10; if( iOQ==0) iOQ=2; else if( iOQ==3) iOQ=0;
	INDEX iAQ = tex_iNormalQuality %10; if( iAQ==0) iAQ=2; else if( iAQ==3) iAQ=0;
	INDEX iTexMul = 2 * Max(iOQ,iAQ);   if( iTexMul==0) iTexMul=1;
	if( tex_iNormalSize<=6 && iTexMul==1) iTexMul = 2;
	if( tex_iNormalSize<=5 && iTexMul==2) iTexMul = 4;
	TS.ts_iMaxBytesPerTexel = iTexMul;
}


// internal routines for texture::read routine


// test mipmap if it can be equilized
#define EQUALIZER_TRESHOLD 3
static BOOL IsEqualized( ULONG *pulMipmap, INDEX pixMipSize)
{
	// determine components and calc averages
	COLOR col;
	ULONG ulR=0, ulG=0, ulB=0;
	for( INDEX iPix=0; iPix<pixMipSize; iPix++) 
	{
		col  = ByteSwap(pulMipmap[iPix]);
		ulR += (col&CT_RMASK)>>CT_RSHIFT;
		ulG += (col&CT_GMASK)>>CT_GSHIFT;
		ulB += (col&CT_BMASK)>>CT_BSHIFT;
	}
	ulR /= pixMipSize;
	ulG /= pixMipSize;
	ulB /= pixMipSize;
	const ULONG ulLoEdge = 127-EQUALIZER_TRESHOLD;
	const ULONG ulHiEdge = 128+EQUALIZER_TRESHOLD;
	BOOL bEqulized = FALSE;
	if( ulR>ulLoEdge && ulR<ulHiEdge &&
		ulG>ulLoEdge && ulG<ulHiEdge &&
		ulB>ulLoEdge && ulB<ulHiEdge) bEqulized = TRUE;
	return bEqulized;
}


// test mipmap if it can be transparent
#define TRANS_TRESHOLD 7
static BOOL IsTransparent( ULONG *pulMipmap, INDEX pixMipSize)
{
	COLOR col;
	ULONG ulA;
	// determine transparency
	for( INDEX iPix=0; iPix<pixMipSize; iPix++) 
	{
		col = ByteSwap(pulMipmap[iPix]);
		ulA = (col&CT_AMASK)>>CT_ASHIFT;
		if( ulA>TRANS_TRESHOLD && ulA<(255-TRANS_TRESHOLD)) return FALSE;
	}
	// transparent!
	return TRUE;
}


// test mipmap whether it is grayscaled
static BOOL IsGray( ULONG *pulMipmap, INDEX pixMipSize)
{
	// loop thru texels
	for( INDEX iPix=0; iPix<pixMipSize; iPix++) 
	{
		COLOR col = ByteSwap(pulMipmap[iPix]);
		if( !IsGray(col)) return FALSE; // colored
	} // grayscaled
	return TRUE;
}



/*****************************************
* Implementation of CTextureData routines
*/
CTextureData::CTextureData()
{
	td_ulFlags = NONE;
	td_mexWidth  = 0;
	td_mexHeight = 0;
	td_tvLastDrawn = 0I64;
	td_iFirstMipLevel  = 0;
	td_ctFineMipLevels = 0;
	
	td_ctFrames    = 0;
	td_slFrameSize = 0;
	td_ulInternalFormat = TEXFMT_NONE;
	td_ulProbeObject = NONE;
	td_ulObject = NONE;
	td_pulFrames = NULL;
	
	td_pubBuffer1      = NULL; // reset effect buffers
	td_pubBuffer2      = NULL;
	td_pixBufferWidth  = 0;
	td_pixBufferHeight = 0;
	td_ptdBaseTexture  = NULL; // no base texture by default
	td_ptegEffect      = NULL; // no effect data
	
	td_iRenderFrame = -1;
	CAnimData::DefaultAnimation();
}


CTextureData::~CTextureData()
{
	Clear();
}


// converts mip level to the one of allowed by texture
INDEX CTextureData::ClampMipLevel( FLOAT fMipFactor) const
{
	INDEX res = (INDEX)fMipFactor;
	INDEX iLastMipLevel = GetNoOfMipmaps( GetPixWidth(), GetPixHeight()) -1 +td_iFirstMipLevel;
	res = Clamp( res, td_iFirstMipLevel, iLastMipLevel);
	return( res);
}


// routine that adds one-frame to texture from one picture
void CTextureData::AddFrame_t( const CImageInfo *pII)
{
	// check for supported image format
	ASSERT( pII->ii_BitsPerPixel==24 || pII->ii_BitsPerPixel==32);
	if( pII->ii_BitsPerPixel!=24 && pII->ii_BitsPerPixel!=32) 
	{
		throw( TRANS("Only 24-bit and 32-bit pictures can be processed."));
	}
	const PIX pixWidth  = pII->ii_Width;
	const PIX pixHeight = pII->ii_Height;
	const BOOL bAlphaChannel = td_ulFlags & TEX_ALPHACHANNEL;
	
	// frame that is about to be added must have the same dimensions as the texture
	ASSERT( pixWidth  == GetPixWidth());
	ASSERT( pixHeight == GetPixHeight());
	if( pixWidth  != GetPixWidth())  throw( TRANS("Incompatible frame width."));
	if( pixHeight != GetPixHeight()) throw( TRANS("Incompatible frame height."));
	
	// add memory for new frame
	SLONG slFramesSize = td_slFrameSize * td_ctFrames;
	GrowMemory( (void**)&td_pulFrames, slFramesSize + td_slFrameSize);
	
	// add new frame to the end of the previous texture frames
	const PIX pixMipSize = pixWidth*pixHeight;
	ULONG *pulCurrentFrame = td_pulFrames + slFramesSize/BYTES_PER_TEXEL;
	
	if( bAlphaChannel) 
	{
		// has alpha channel - do simple copying
		memcpy( pulCurrentFrame, pII->ii_Picture, pixMipSize*4);
	}
	else 
	{
		// hasn't got alpha channel - do conversion from 24-bit bitmap to 32-bit format
		AddAlphaChannel( pII->ii_Picture, pulCurrentFrame, pixMipSize);
	}
	// make mipmaps (in place!)
	MakeMipmaps( pulCurrentFrame, pixWidth,pixHeight);
	
	// re-check texture for transparency
	if( td_ulFlags & TEX_TRANSPARENT) 
	{
		if( !IsTransparent( pulCurrentFrame, pixMipSize)) td_ulFlags &= ~TEX_TRANSPARENT; 
	}
	// re-check texture for equalization
	if( td_ulFlags & TEX_EQUALIZED) 
	{
		const INDEX ctLastPixels   = Max(pixWidth,pixHeight) / Min(pixWidth,pixHeight);
		ULONG *pulLastMipMap = pulCurrentFrame + td_slFrameSize/BYTES_PER_TEXEL - ctLastPixels;
		if( !IsEqualized( pulLastMipMap, ctLastPixels)) td_ulFlags &= ~TEX_EQUALIZED;
	}
	
	// increase number of frames
	td_ctFrames++;
}


// routine that creates one-frame-texture from loaded picture thru image info structure
void CTextureData::Create_t( const CImageInfo *pII, MEX mexWanted, INDEX ctFineMips, ULONG ulFlags/*=NONE*/)
{
	TRACKTEX_HEAP();
	
	// check for supported image format
	ASSERT( pII->ii_BitsPerPixel==24 || pII->ii_BitsPerPixel==32);
	if( pII->ii_BitsPerPixel!=24 && pII->ii_BitsPerPixel!=32) 
	{
		throw( TRANS("Only 24-bit and 32-bit pictures can be processed."));
	}
	
	// get picture data
	PIX pixSizeU = pII->ii_Width;
	PIX pixSizeV = pII->ii_Height;
	
	// check maximum supported texture dimension
	if( pixSizeU>MAX_MEX || pixSizeV>MAX_MEX) throw( TRANS("At least one of texture dimensions is too large."));
	
	// determine physical (maximum) number of mip-levels
	INDEX iSizeULog2 = FastLog2(pixSizeU);
	INDEX iSizeVLog2 = FastLog2(pixSizeV);
	ASSERT( (1UL<<iSizeULog2)==pixSizeU && (1UL<<iSizeVLog2)==pixSizeV);
	
	// dimension in mexels must not be smaller than the one in pixels
	ASSERT( pixSizeU<=mexWanted);
	
	// determine mip index from mex size and alpha channel presence
	td_iFirstMipLevel = FastLog2( mexWanted/pixSizeU);
	const BOOL bAlphaChannel = (pII->ii_BitsPerPixel==32);
	
	// initiate proper flags
	if( pixSizeU<4 || pixSizeV<4) ulFlags &= ~(TEX_COMPRESS|TEX_COMPRESSALPHA); // min allowed dimension of compressed texture is 4x4
	td_ulFlags  = bAlphaChannel ? TEX_ALPHACHANNEL : NONE;
	td_ulFlags |= ulFlags;
	
	// initialize general TextureData members
	td_ctFrames  = 0;
	td_mexWidth  = pixSizeU<<td_iFirstMipLevel;
	td_mexHeight = pixSizeV<<td_iFirstMipLevel;
	
	// create all mip levels (either bilinear or downsampled)
	INDEX ctMipLevels  = GetNoOfMipmaps( pixSizeU, pixSizeV);
	td_ctFineMipLevels = Min( ctFineMips, ctMipLevels);
	// get frame size (includes only one mip-map)
	td_slFrameSize = GetMipmapOffset( 15, pixSizeU, pixSizeV) *BYTES_PER_TEXEL;
	
	// try to assume that texture is transparent and equilized, until proven otherwize (in AddFrame)
	if( td_ulFlags & TEX_ALPHACHANNEL) td_ulFlags |= TEX_TRANSPARENT;
	if( td_ctFineMipLevels > 1)        td_ulFlags |= TEX_EQUALIZED;  
	
	// allocate small ammount of memory just for Realloc sake
	td_pulFrames = (ULONG*)AllocMemory(16);
	AddFrame_t( pII);
}



// returns dimension of effect buffers and size in bytes (of one, not both)
static ULONG GetEffectBufferSize( CTextureData *pTD)
{
	ASSERT( pTD->td_ptegEffect!=NULL);
	PIX pixWidth  = pTD->td_pixBufferWidth; 
	PIX pixHeight = pTD->td_pixBufferHeight;
	
	ULONG ulSize = pixWidth*pixHeight *sizeof(UBYTE);
	// eventual adjustment for water effect type
	if( pTD->td_ptegEffect->IsWater()) ulSize = pixWidth*(pixHeight+2) *sizeof(SWORD);
	return ulSize;
}


// initializes td_pixBufferWidth & td_pixBufferHeight
static void InitEffectBufferDimensions( CTextureData *pTD)
{
	// initialize as default
	PIX pixWidth  = pTD->GetPixWidth(); 
	PIX pixHeight = pTD->GetPixHeight();
	
	// if water effect type
	if( pTD->td_ptegEffect->IsWater()) 
	{
		// adjust size for water type effect (width or height must be 64)
		if( pixWidth > pixHeight) 
		{
			pixHeight = (PIX)((FLOAT)pixHeight/pixWidth  *64.0f);
			pixWidth  = 64;
		}
		else 
		{
			pixWidth  = (PIX)((FLOAT)pixWidth /pixHeight *64.0f);
			pixHeight = 64;
		}
	}
	// set 
	pTD->td_pixBufferWidth  = pixWidth;
	pTD->td_pixBufferHeight = pixHeight;
}


// free effect buffers' memory
static void FreeEffectBuffers( CTextureData *pTD)
{
	if( pTD->td_pubBuffer1 != NULL) 
	{
		FreeMemory( pTD->td_pubBuffer1);
		pTD->td_pubBuffer1 = NULL;
	}
	if( pTD->td_pubBuffer2 != NULL) 
	{
		FreeMemory( pTD->td_pubBuffer2);
		pTD->td_pubBuffer2 = NULL;
	}
}


// allocates and resets effect buffers
static ULONG AllocEffectBuffers( CTextureData *pTD)
{
	TRACKTEX_HEAP();
	// free if already allocated
	FreeEffectBuffers( pTD);
	// determine size of effect buffers 
	const ULONG ulSize = GetEffectBufferSize( pTD);
	// allocate and reset buffers (memory walling!)
	pTD->td_pubBuffer1 = (UBYTE*)AllocMemory( ulSize+8);
	pTD->td_pubBuffer2 = (UBYTE*)AllocMemory( ulSize+8);
	memset( pTD->td_pubBuffer1, 0, ulSize);
	memset( pTD->td_pubBuffer2, 0, ulSize);
	return ulSize;
}


// creates new effect texture with one frame
void CTextureData::CreateEffectTexture( PIX pixWidth, PIX pixHeight, MEX mexWidth,
									   CTextureData *ptdBaseTexture, ULONG ulGlobalEffect)
{
	ptdBaseTexture->MarkUsed();
	Clear();
	CAnimData::DefaultAnimation();
	
	// determine mip index from mex size
	td_iFirstMipLevel = FastLog2( mexWidth/pixWidth);
	// fill some of TextureData members
	td_ulFlags   = TEX_STATIC;
	td_mexWidth  = pixWidth <<td_iFirstMipLevel;
	td_mexHeight = pixHeight<<td_iFirstMipLevel;
	td_ctFrames  = 1;
	td_pulFrames = NULL;
	// remember base texture
	td_ptdBaseTexture = ptdBaseTexture;
	
	// allocate texture effect global
	td_ptegEffect = new CTextureEffectGlobal( this, ulGlobalEffect);
	
	// allocate and reset effect buffers
	InitEffectBufferDimensions(this);
	AllocEffectBuffers(this);
}



// this promotes 16bit internal format to corresponding 32bit
static ULONG PromoteTo32bitFormat( ULONG ulFormat)
{
	if( ulFormat==TS.ts_tfRGB5) return TS.ts_tfRGB8;
	if( ulFormat==TS.ts_tfRGBA4 || ulFormat==TS.ts_tfRGB5A1) return TS.ts_tfRGBA8;
	return ulFormat;
}


// returns format in what texture will be uploaded (regarding console vars)
static ULONG DetermineInternalFormat( CTextureData *pTD)
{
	// not needed for compressed textures
	ASSERT( !(pTD->td_ulFlags&TEX_COMPRESSED));
	if( pTD->td_ulFlags&TEX_COMPRESSED) return NONE;
	
	// clamp some cvars
	extern INDEX tex_bCompressGrayscale;      if( tex_bCompressGrayscale)     tex_bCompressGrayscale     = 1;
	extern INDEX tex_bCompressAlphaChannel;   if( tex_bCompressAlphaChannel)  tex_bCompressAlphaChannel  = 1;
	extern INDEX tex_bAlternateCompression;   if( tex_bAlternateCompression)  tex_bAlternateCompression  = 1;
	extern INDEX tex_bAggressiveCompression;  if( tex_bAggressiveCompression) tex_bAggressiveCompression = 1;
	
	// cache some vars
	const PIX pixTexWidth  = pTD->GetPixWidth();
	const PIX pixTexHeight = pTD->GetPixHeight();
	const PIX pixTexSize   = pixTexWidth * pixTexHeight;
	extern INDEX gap_bAllowGrayTextures;
	const BOOL bGrayTexture  = gap_bAllowGrayTextures && (pTD->td_ulFlags&TEX_GRAY);
	const BOOL bAlphaChannel = pTD->td_ulFlags & TEX_ALPHACHANNEL;
	const BOOL bHighQuality  = pTD->td_ulFlags & TEX_32BIT;
	const BOOL bCanCompress  = pixTexWidth>=4 && pixTexHeight>=4
		&& (!(pTD->td_ulFlags&TEX_CONSTANT) || tex_bAggressiveCompression);
	
	// choose internal texture format for alpha textures
	INDEX iQuality;
	ULONG ulFormat;
	if( bAlphaChannel)
	{
		iQuality = pTD->td_ctFrames>1 ? TS.ts_iAnimQualityA : TS.ts_iNormQualityA;
		ASSERT( iQuality>=0 && iQuality<=3);
		ulFormat = TS.ts_tfRGBA4;
		if( iQuality==2 || (iQuality==0 && bHighQuality)) ulFormat = TS.ts_tfRGBA8;
		if( bGrayTexture) ulFormat = TS.ts_tfLA8;
		// handle case of forced internal format (for texture cration process only!)
		if( _iTexForcedQuality==16) ulFormat = TS.ts_tfRGBA4;
		else if( _iTexForcedQuality==32) ulFormat = TS.ts_tfRGBA8;
		// do eventual adjustment of transparent textures
		if( (pTD->td_ulFlags&TEX_TRANSPARENT) && ulFormat==TS.ts_tfRGBA4) ulFormat = TS.ts_tfRGB5A1;
	}
	
	// choose internal texture format for opaque textures
	else
	{
		iQuality = pTD->td_ctFrames>1 ? TS.ts_iAnimQualityO : TS.ts_iNormQualityO;
		ASSERT( iQuality>=0 && iQuality<=3);
		ulFormat = TS.ts_tfRGB5;
		if( iQuality==2 || (iQuality==0 && bHighQuality)) ulFormat = TS.ts_tfRGB8;
		if( bGrayTexture) ulFormat = TS.ts_tfL8;
		// handle case of forced internal format (for texture cration process only!)
		if( _iTexForcedQuality==16) ulFormat = TS.ts_tfRGB5;
		else if( _iTexForcedQuality==32) ulFormat = TS.ts_tfRGB8;
	}
	
	// adjust format to compressed if needed and allowed
	if( iQuality==3 && bCanCompress && _iTexForcedQuality==0)
	{ 
		// compress opaque/transparent
		// (strange one: D3DFMT_A1R5G5B5 instead of TS.ts_tfRGB5A1 is because OpenGL
		//  erroneously treats transparent textures as fully opaque!)
		if( ulFormat==TS.ts_tfRGB8 || ulFormat==TS.ts_tfRGB5 || ulFormat==D3DFMT_A1R5G5B5  
			|| (ulFormat==TS.ts_tfL8 && tex_bCompressGrayscale)) 
		{
			// do NV fix if needed (compress opaque textures as transparent)
			if( tex_bAlternateCompression) ulFormat = TS.ts_tfRGBcA;
			else ulFormat = TS.ts_tfRGBc;
		}
		// compress translucent
		else if( ulFormat==TS.ts_tfRGBA8 || ulFormat==TS.ts_tfRGBA4 
			|| (ulFormat==TS.ts_tfLA8 && tex_bCompressGrayscale)) 
		{
			if( tex_bCompressAlphaChannel) ulFormat = TS.ts_tfRGBcAc;  // interpolated alpha
			else ulFormat = TS.ts_tfRGBcA;
		}
	}
	
	// all done
	return ulFormat;
}



// routine that performs texture conversion to current texture format (version 4)
static void Convert( CTextureData *pTD)
{
	// skip effect textures
	if( pTD->td_ptegEffect != NULL) return;
	TRACKTEX_HEAP();
	
	// determine dimensions 
	PIX pixWidth     = pTD->GetPixWidth();
	PIX pixHeight    = pTD->GetPixHeight();
	PIX pixMipSize   = pixWidth * pixHeight;
	PIX pixFrameSize = GetMipmapOffset( 15, pixWidth, pixHeight);
	// allocate memory for new texture
	ULONG *pulFramesNew = (ULONG*)AllocMemory( pixFrameSize*pTD->td_ctFrames *BYTES_PER_TEXEL);
	UWORD *puwFramesOld = (UWORD*)pTD->td_pulFrames;
	ASSERT( puwFramesOld!=NULL);
	
	// determine alpha channel presence
	BOOL bHasAlphaChannel = pTD->td_ulFlags & TEX_ALPHACHANNEL;
	
	// unpack texture from 16-bit RGBA4444 or RGBA5551 format to RGBA8888 32-bit format
	UBYTE r,g,b,a;
	// for each frame
	for( INDEX iFr=0; iFr<pTD->td_ctFrames; iFr++)
	{ // get addresses of current frames (new and old)
		PIX pixFrameOffset = iFr * pixFrameSize;
		// for each pixel
		for( INDEX iPix=0; iPix<pixMipSize; iPix++)
		{ // read 16-bit pixel
			UWORD uwPix = puwFramesOld[pixFrameOffset+iPix];
			// unpack it
			if( bHasAlphaChannel) 
			{
				// with alpha channel
				r = (uwPix & 0xF000) >>8;
				g = (uwPix & 0x0F00) >>4;
				b = (uwPix & 0x00F0) >>0;
				a = (uwPix & 0x000F) <<4;
				// adjust strength
				r |= r>>4; g |= g>>4; b |= b>>4; a |= a>>4;
			}
			else 
			{
				// without alpha channel
				r = (uwPix & 0xF800) >>8;
				g = (uwPix & 0x07C0) >>3;
				b = (uwPix & 0x003E) <<2;
				a = 0xFF;
				// adjust strength
				r |= r>>5; g |= g>>5; b |= b>>5;
			}
			
			// pack it back to 32-bit
			ULONG ulPix = RGBAToColor(r,g,b,a);
			// store 32-bit pixel
			pulFramesNew[pixFrameOffset+iPix] = ByteSwap(ulPix);
		}
	}
	
	// free old memory
	FreeMemory( pTD->td_pulFrames);
	// remember new texture parameters
	pTD->td_pulFrames   = pulFramesNew;
	pTD->td_slFrameSize = pixFrameSize *BYTES_PER_TEXEL;
}


// remove mipmaps from texture that are not needed (exceeds maximum supported dimension)
static void RemoveOversizedMipmaps( CTextureData *pTD)
{
	// if this is an effect texture, leave as it is
	if( pTD->td_ptegEffect != NULL) return;
	pTD->td_ulFlags &= ~TEX_DISPOSED;
	
	// determine and clamp to max allowed texture dimension and size
	PIX pixClampAreaSize = (pTD->td_ctFrames>1) ? TS.ts_pixAnimSize : TS.ts_pixNormSize;
	// constant textures doesn't need clamping to area, but still must be clamped to max HW dimension!
	if( pTD->td_ulFlags & TEX_CONSTANT) pixClampAreaSize = 4096*4096; 
	
	// determine dimensions of finest mip-map and number of mip-maps to skip
	PIX pixSizeU = pTD->GetPixWidth();
	PIX pixSizeV = pTD->GetPixHeight();
	INDEX ctSkipMips = ClampTextureSize( pixClampAreaSize, _pGfx->gl_pixMaxTextureDimension, pixSizeU, pixSizeV);
	
	// check for mip overhead
	INDEX ctMips = GetNoOfMipmaps( pixSizeU, pixSizeV);
	ASSERT( ctMips>ctSkipMips);
	while( ctMips<=ctSkipMips) ctSkipMips--;
	
	// done here if no need to remove mip-maps
	if( ctSkipMips==0) return;
	TRACKTEX_HEAP();
	
	SLONG slRemovedMipsSize, slOldFrameSize, slNewFrameSize;
	UBYTE *pubNewFrames, *pubOldFrame, *pubNewFrame;
	
	// determine memory size and allocate memory for rest mip-maps
	// (compressed texture is handled differently)
	if( pTD->td_ulFlags & TEX_COMPRESSED) 
	{
		// cannot remove mipmaps if one dimension ends up less 4 pixels
		ASSERT( (pixSizeU>>ctSkipMips)>=4 && (pixSizeV>>ctSkipMips)>=4);
		if( (pixSizeU>>ctSkipMips)<4 || (pixSizeV>>ctSkipMips)<4) return;
		pubOldFrame = (UBYTE*)pTD->td_pulFrames;
		for( INDEX iMip=0; iMip<ctSkipMips; iMip++) 
		{
			const SLONG slMipSize = *(ULONG*)pubOldFrame;
			pubOldFrame += slMipSize+4;
		}
		slOldFrameSize = pTD->td_slFrameSize;   
		slNewFrameSize = slOldFrameSize - ((SLONG)pubOldFrame - (SLONG)pTD->td_pulFrames);
		pubNewFrames = (UBYTE*)AllocMemory( slNewFrameSize * pTD->td_ctFrames);
		pubNewFrame  = pubNewFrames;
	}
	// non-compressed textures
	else 
	{
		slRemovedMipsSize = GetMipmapOffset( ctSkipMips, pixSizeU, pixSizeV) *BYTES_PER_TEXEL;
		slOldFrameSize = pTD->td_slFrameSize;   
		slNewFrameSize = slOldFrameSize - slRemovedMipsSize;
		pubNewFrames = (UBYTE*)AllocMemory( slNewFrameSize * pTD->td_ctFrames);
		pubOldFrame  = ((UBYTE*)pTD->td_pulFrames) + slRemovedMipsSize;
		pubNewFrame  = pubNewFrames;
	}
	
	// copy only needed mip-maps from each frame
	for( INDEX iFr=0; iFr<pTD->td_ctFrames; iFr++) 
	{
		memcpy( pubNewFrame, pubOldFrame, slNewFrameSize);
		pubNewFrame += slNewFrameSize;
		pubOldFrame += slOldFrameSize;
	}
	
	// free old frames memory
	FreeMemory( pTD->td_pulFrames);
	// adjust texture parameters
	pTD->td_pulFrames       = (ULONG*)pubNewFrames;
	pTD->td_slFrameSize     = slNewFrameSize;
	pTD->td_iFirstMipLevel += ctSkipMips;
	pTD->td_ctFineMipLevels = ClampDn( pTD->td_ctFineMipLevels-ctSkipMips, 1L);
	
	// mark that this texture had some mip maps disposed
	pTD->td_ulFlags |= TEX_DISPOSED;
}

//안태훈 수정 시작	//(Encode Data)(0.1)
#define TEX_DATA_VER 1
#define CODE_TEX_1 17
#define CODE_TEX_2 02
#define CODE_TEX_3 41
#define CODE_TEX_4 01
#define CODE_TEX_5 6

template<class Type>
inline Type EncodeSimpleTex(Type val, UBYTE &ubChecker)
{
	if(sizeof(Type) == 4)
	{
		//ULONG ulChecker = ((ubChecker += (UBYTE)CODE_1) << 24)
						//| ((ubChecker += (UBYTE)CODE_2) << 16)
						//| ((ubChecker += (UBYTE)CODE_3) << 8 )
						//| ((ubChecker += (UBYTE)CODE_4) << 0 );
		ULONG ulChecker = 0;
		ubChecker += (UBYTE)CODE_TEX_1;
		ulChecker |= ULONG(ubChecker) << 24;
		ubChecker += (UBYTE)CODE_TEX_2;
		ulChecker |= ULONG(ubChecker) << 16;
		ubChecker += (UBYTE)CODE_TEX_3;
		ulChecker |= ULONG(ubChecker) << 8;
		ubChecker += (UBYTE)CODE_TEX_4;
		ulChecker |= ULONG(ubChecker) << 0;
		ubChecker += CODE_TEX_5;
		return Type( val ^ ulChecker );
	}
	else return val;
}
template<class Type>
inline Type DecodeSimpleTex(Type val, UBYTE &ubChecker)
{
	if(sizeof(Type) == 4)
	{
		//ULONG ulChecker = ((ubChecker += (UBYTE)CODE_1) << 24)
						//| ((ubChecker += (UBYTE)CODE_2) << 16)
						//| ((ubChecker += (UBYTE)CODE_3) << 8 )
						//| ((ubChecker += (UBYTE)CODE_4) << 0 );
		ULONG ulChecker = 0;
/*
		ubChecker += 61;
		ulChecker |= ULONG(ubChecker) << 24;
		ulChecker |= ULONG(ubChecker) << 16;
		ulChecker |= ULONG(ubChecker) << 8;
		ulChecker |= ULONG(ubChecker) << 0;
*/
		//error version

		ubChecker += (UBYTE)CODE_TEX_1;
		ulChecker |= ULONG(ubChecker) << 24;
		ubChecker += (UBYTE)CODE_TEX_2;
		ulChecker |= ULONG(ubChecker) << 16;
		ubChecker += (UBYTE)CODE_TEX_3;
		ulChecker |= ULONG(ubChecker) << 8;
		ubChecker += (UBYTE)CODE_TEX_4;
		ulChecker |= ULONG(ubChecker) << 0;
		ubChecker += CODE_TEX_5;
		return Type( val ^ ulChecker );
	}
	else return val;
}

//안태훈 수정 끝	//(Encode Data)(0.1)

// reads 32/24-bit texture from file and eventually converts it to 8-bit pixel format
void CTextureData::Read_t( CTStream *inFile)
{
	TRACKTEX_HEAP();
	
	// reset texture (blank all except some flags)
	Clear();
	
	// determine API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);
	
	// determine driver context presence (must have at least 1 texture unit!)
	const BOOL bHasContext = (_pGfx->gl_ctRealTextureUnits>0);

	CTString& strDesc = inFile->GetDescription();

	// read version
	INDEX iVersion;
	inFile->ExpectID_t( "TVER");

	if (inFile->is_png() == true)
	{
		Read_PNG(inFile);
		return;
	}

	*inFile >> iVersion;
//안태훈 수정 시작	//(Encode Data)(0.1)
	INDEX iNewVer = (iVersion & 0xFFFF0000) >> 16;
	iVersion &= 0x0000FFFF;
//안태훈 수정 끝	//(Encode Data)(0.1)

	if(iVersion == 5)
	{
		iVersion = 4;
		iNewVer = 5;
	}
	
	// check currently supported versions
	if(iVersion!=4 && iVersion!=3)
		throw( TRANS("Invalid texture format version."));

	// mark if this texture was loaded form the old format
	if( iVersion==3) td_ulFlags |= TEX_WASOLD;
	BOOL bResetEffectBuffers = FALSE;
	BOOL bFramesLoaded = FALSE;
	BOOL bAlphaChannel = FALSE;
	BOOL bCompressed   = FALSE;
	SLONG slCompressedFrameSize = 0;
	
	// loop trough file and react according to chunk ID
	do
	{
		// obtain chunk id
		CChunkID idChunk = inFile->GetID_t();
		
		// if this is chunk containing texture data
		if( idChunk == CChunkID("TDAT"))
		{
//안태훈 수정 시작	//(Encode Data)(0.1)
			if(iNewVer == TEX_DATA_VER || iNewVer == 5)
			{
				UBYTE checker = iNewVer;
				// read data describing texture
				INDEX ctMipLevels;
				ULONG ulFlags=0;
				*inFile >> td_mexWidth;			td_mexWidth = DecodeSimpleTex(td_mexWidth, checker);
				*inFile >> td_iFirstMipLevel;	td_iFirstMipLevel = DecodeSimpleTex(td_iFirstMipLevel, checker);
				*inFile >> td_mexHeight;		td_mexHeight = DecodeSimpleTex(td_mexHeight, checker);
				*inFile >> td_ctFineMipLevels;	td_ctFineMipLevels = DecodeSimpleTex(td_ctFineMipLevels, checker);
				*inFile >> ulFlags;				ulFlags = DecodeSimpleTex(ulFlags, checker);
				*inFile >> td_ctFrames;			td_ctFrames = DecodeSimpleTex(td_ctFrames, checker);
				//char buf[1024] = {0};
				//sprintf(buf, "%d, %d, %d, %d, %d, %d"
					//, td_mexWidth, td_iFirstMipLevel, td_mexHeight, td_ctFineMipLevels, ulFlags, td_ctFrames);
				//OutputDebugString(buf);
				if( iVersion!=4) *inFile >> ctMipLevels;
				if( iVersion!=4) *inFile >> td_slFrameSize;
				// combine flags
				td_ulFlags |= ulFlags;
				bAlphaChannel = td_ulFlags & TEX_ALPHACHANNEL;
				bCompressed   = td_ulFlags & TEX_COMPRESSED;
				// set number of mip levels and frame size
				// ASSERT( td_ctFineMipLevels>0);
				td_ctFineMipLevels = ClampDn( td_ctFineMipLevels, 1L);
				td_slFrameSize = GetMipmapOffset( 15, GetPixWidth(), GetPixHeight()) * BYTES_PER_TEXEL;
			}
			else	//old
			{
				// read data describing texture
				ULONG ulFlags=0;
				INDEX ctMipLevels;
				*inFile >> ulFlags;
				*inFile >> td_mexWidth;
				*inFile >> td_mexHeight;
				*inFile >> td_ctFineMipLevels;
				if( iVersion!=4) *inFile >> ctMipLevels;
				*inFile >> td_iFirstMipLevel;
				if( iVersion!=4) *inFile >> td_slFrameSize;
				*inFile >> td_ctFrames;
				// combine flags
				td_ulFlags |= ulFlags;
				bAlphaChannel = td_ulFlags & TEX_ALPHACHANNEL;
				bCompressed   = td_ulFlags & TEX_COMPRESSED;
				// set number of mip levels and frame size
				// ASSERT( td_ctFineMipLevels>0);
				td_ctFineMipLevels = ClampDn( td_ctFineMipLevels, 1L);
				if( iVersion==4) td_slFrameSize = GetMipmapOffset( 15, GetPixWidth(), GetPixHeight()) * BYTES_PER_TEXEL;
			}
//안태훈 수정 끝	//(Encode Data)(0.1)
		}
		// if this is chunk containing compressed frames
		else if( idChunk == CChunkID("FRMC"))
		{
			ASSERT(bCompressed);
			*inFile >> slCompressedFrameSize;
			const SLONG slCompressedTexSize = td_ctFrames * slCompressedFrameSize;
			// if no driver is present
			if( !bHasContext) 
			{
				// just skip the frames
				inFile->Seek_t( slCompressedTexSize, CTStream::SD_CUR);
			}
			else 
			{
				// load frames!
				td_pulFrames = (ULONG*)AllocMemory(slCompressedTexSize);
				inFile->Read_t( td_pulFrames, slCompressedTexSize);
				bFramesLoaded = TRUE;  // mark that frames have been loaded
			}
			// update frame size      
			td_slFrameSize = slCompressedFrameSize;
		}
		// if this is chunk containing raw frames
		else if( idChunk == CChunkID("FRMS")) 
		{ 
			ASSERT( !bCompressed);
			// if no driver is present and texture is not static
			if( !bHasContext) // hmmm ...  && !(td_ulFlags&TEX_STATIC)
			{ // determine frames' size
				SLONG slSkipSize = td_slFrameSize;
				if( iVersion==4) 
				{
					slSkipSize = GetPixWidth()*GetPixHeight();
					if( bAlphaChannel) slSkipSize *=4;
					else slSkipSize *=3;
				} 
				// just seek over frames (skip 'em)
				inFile->Seek_t( slSkipSize*td_ctFrames, CTStream::SD_CUR);
				continue;
			}
			// calculate texture size for corresponding texture format and allocate memory
			SLONG slTexSize = td_slFrameSize * td_ctFrames;
			td_pulFrames = (ULONG*)AllocMemory(slTexSize);
			// if older version
			if( iVersion==3) 
			{
				// alloc memory block and read mip-maps
				inFile->Read_t( td_pulFrames, slTexSize);
			} 
			// if current version
			else 
			{
				PIX pixFrameSizeOnDisk = GetPixWidth()*GetPixHeight();
				for( INDEX iFr=0; iFr<td_ctFrames; iFr++)
				{ // loop thru frames
					ULONG *pulCurrentFrame = td_pulFrames + (iFr * td_slFrameSize/BYTES_PER_TEXEL);
					if( bAlphaChannel) 
					{
						// read texture with alpha channel from file
						inFile->Read_t( pulCurrentFrame, pixFrameSizeOnDisk *4);
					}
					else 
					{
						// read texture without alpha channel from file
						inFile->Read_t( pulCurrentFrame, pixFrameSizeOnDisk *3);
						// add opaque alpha channel
						AddAlphaChannel( (UBYTE*)pulCurrentFrame, pulCurrentFrame, pixFrameSizeOnDisk);
					}
				}
			} // mark that frames have been loaded
			bFramesLoaded = TRUE;
		}
		// if this is chunk containing texture animation data
		else if( idChunk == CChunkID("ANIM"))
		{
			// read corresponding animation(s)
			CAnimData::Read_t( inFile);
		}
		// if this is chunk containing base texture name
		else if( idChunk == CChunkID("BAST"))
		{
			CTFileName fnBaseTexture;
			// read file name of base texture
			*inFile >> fnBaseTexture;
			// if there is base texture, obtain it
			if( fnBaseTexture != "") 
			{
				// must not be the same as base texture
				CTFileName fnTex = inFile->GetDescription();
				if( fnTex == fnBaseTexture) 
				{
					// generate exception
					ThrowF_t( TRANS("Texture \"%s\" has same name as its base texture."), (CTString&)fnTex);
				}
				else 
				{
					// obtain base texture and force it to be static by default
					td_ptdBaseTexture = _pTextureStock->Obtain_t(fnBaseTexture);
					td_ptdBaseTexture->Force(TEX_STATIC);
				}
			} 
		}
		// if this is chunk containing saved effect buffers
		else if( idChunk == CChunkID("FXBF")) 
		{ // skip chunk in old versions
			bResetEffectBuffers = TRUE;
			if( iVersion!=4) 
			{
				inFile->Seek_t( 2* GetPixWidth()*GetPixHeight() *sizeof(SWORD), CTStream::SD_CUR);
			}
			else 
			{
				ASSERT( td_pixBufferWidth>0 && td_pixBufferHeight>0);
				ULONG ulSize = AllocEffectBuffers(this);
				if( td_ptegEffect->IsWater()) ulSize*=2;
				inFile->Seek_t( 2*ulSize, CTStream::SD_CUR);
			}
		}
		else if( idChunk == CChunkID("FXB2")) 
		{ // read effect buffers
			ASSERT( td_pixBufferWidth>0 && td_pixBufferHeight>0);
			ULONG ulSize = AllocEffectBuffers(this);
			inFile->Read_t( td_pubBuffer1, ulSize);
			inFile->Read_t( td_pubBuffer2, ulSize);
		}
		// if this is chunk containing effect data
		else if( idChunk == CChunkID("FXDT"))
		{ // read effect class
			ULONG ulGlobalEffect;
			*inFile >> ulGlobalEffect;
			// read effect buffer dimensions
			if( iVersion==4) *inFile >> td_pixBufferWidth;
			if( iVersion==4) *inFile >> td_pixBufferHeight;
			// allocate memory for texture effect struct
			td_ptegEffect = new CTextureEffectGlobal(this, ulGlobalEffect);
			// skip global properties for old format effect texture
			if( iVersion!=4) inFile->Seek_t( 64*sizeof(char), CTStream::SD_CUR);
			// read count of effect sources
			INDEX ctEffectSources;
			*inFile >> ctEffectSources;
			// add requested number of members to effect source array
			CTextureEffectSource *pEffectSources = td_ptegEffect->teg_atesEffectSources.New( ctEffectSources);
			
			// read whole dynamic array of effect sources
			FOREACHINDYNAMICARRAY( td_ptegEffect->teg_atesEffectSources, CTextureEffectSource, itEffectSource)
			{
				// read type of effect source
				*inFile >> (ULONG) itEffectSource->tes_ulEffectSourceType;
				// read structure holding effect source properties
				inFile->Read_t( &itEffectSource->tes_tespEffectSourceProperties, sizeof(struct TextureEffectSourceProperties));
				// remember pointer to global effect
				itEffectSource->tes_ptegGlobalEffect = td_ptegEffect;
				// read count of effect pixels
				INDEX ctEffectSourcePixels;
				*inFile >> ctEffectSourcePixels;
				// if there are any effect pixels
				if (ctEffectSourcePixels>0) 
				{
					// alocate needed ammount of members
					itEffectSource->tes_atepPixels.New( ctEffectSourcePixels);
					// read all effect pixels in one block
					inFile->Read_t( &itEffectSource->tes_atepPixels[0], sizeof(struct TextureEffectPixel)*ctEffectSourcePixels);
				}
			}
			// allocate memory for effect frame buffer
			SLONG slFrameSize = GetMipmapOffset( 15, GetPixWidth(), GetPixHeight()) *BYTES_PER_TEXEL;
			td_pulFrames = (ULONG*)AllocMemory( slFrameSize);
			// remember once again new frame size just for the sake of old effect textures
			td_slFrameSize = slFrameSize;
			// mark that effect texture needs to be static
			td_ulFlags |= TEX_STATIC;
		}
		// if this is chunk containing data about detail texture
		else if( idChunk == CChunkID("DTLT"))
		{ // skip chunk (this is here only for compatibility reasons)
			CTFileName fnTmp;
			*inFile >> fnTmp;
		}
		else
		{
			ThrowF_t( TRANS("Unrecognizable chunk ID (\"%s\") found while reading texture \"%s\"."),
				(char*)idChunk, (CTString&)inFile->GetDescription() );
		}
	}
	// until we didn't reach end of file
	while( !inFile->AtEOF());
	
	// if this is an effect texture
	if( td_ptegEffect!=NULL) 
	{
		// reset effect buffers if needed
		if( bResetEffectBuffers) 
		{
			InitEffectBufferDimensions(this);
			AllocEffectBuffers(this);
		} // force effect textures to be static and upload once right now
		td_ulFlags |= TEX_STATIC;
		if( bHasContext) SetAsCurrent();
		return;
	}
	
	// done here if frames weren't loaded
	ASSERT( !bResetEffectBuffers);
	if( !bFramesLoaded) 
	{
		return;
	}
	
	// handle compressed textures differently
	//_pGfx->gl_bCompressedTexture = FALSE;		// test - yjpark
	if( bCompressed)
	{
		ASSERT( bHasContext);
		RemoveOversizedMipmaps(this);
		const PIX pixWidth  = GetPixWidth();
		const PIX pixHeight = GetPixHeight();
		const BOOL bStatic  = td_ulFlags & TEX_STATIC;
		// static texture needs to be decompressed
		ULONG *pulExpanded = NULL;
		SLONG slExpandSize = 0;
		if( bStatic /*|| !_pGfx->gl_bCompressedTexture*/ ) 
		{
			const BOOL bAlphaCh = (td_ulFlags & TEX_ALPHACHANNEL);
			const BOOL bComprAl = (td_ulFlags & TEX_COMPRESSEDALPHA) && bAlphaCh;
			const PIX pixExpandSize = GetMipmapOffset( 15, pixWidth,pixHeight);
			slExpandSize = pixExpandSize * BYTES_PER_TEXEL;
			pulExpanded  = (ULONG*)AllocMemory(slExpandSize*td_ctFrames);
			// loop thru frames
			for( INDEX iFr=0; iFr<td_ctFrames; iFr++)
			{ // determine frame offset and upload texture frame
				UBYTE *pubFrameCo = (UBYTE*)(td_pulFrames+1) + (iFr*td_slFrameSize); // +1 to skip length of mipmap!
				ULONG *pulFrameEx = pulExpanded + (iFr*pixExpandSize);
				extern void DecompressDXT1( UBYTE *pubCompressed, ULONG *pulExpanded, PIX pixWidth, PIX pixHeight);
				extern void DecompressDXT3( UBYTE *pubCompressed, ULONG *pulExpanded, PIX pixWidth, PIX pixHeight);
				extern void DecompressDXT5( UBYTE *pubCompressed, ULONG *pulExpanded, PIX pixWidth, PIX pixHeight);
				if( bComprAl) DecompressDXT5( pubFrameCo, pulFrameEx, pixWidth,pixHeight);
				else if( bAlphaCh) DecompressDXT3( pubFrameCo, pulFrameEx, pixWidth,pixHeight);
				else               DecompressDXT1( pubFrameCo, pulFrameEx, pixWidth,pixHeight);
				// mipmaps must be created in memory
				MakeMipmaps( pulFrameEx, pixWidth,pixHeight);
			}
		}

		//if( _pGfx->gl_bCompressedTexture )
		{
			// upload
			const BOOL bUploaded = UploadCompressed();
			if( !bUploaded) throw( TRANS("Cannot upload compressed texture."));

			// assign uncompressed frames if texture is static
			if( bStatic) 
			{
				ASSERT( slExpandSize>0 && pulExpanded!=NULL);
				td_slFrameSize = slExpandSize;
				td_pulFrames   = pulExpanded;
			}
			
			// over and out
			return;
		}
		//else
		//{
		//	ASSERT( slExpandSize>0 && pulExpanded!=NULL);
		//	td_slFrameSize = slExpandSize;
		//	td_pulFrames   = pulExpanded;
		//	td_ulFlags &= ~TEX_COMPRESSED;
		//}
	}
	
	// if texture is in old format, convert it to current format
	if( iVersion==3) Convert(this);
	PIX pixWidth     = GetPixWidth();
	PIX pixHeight    = GetPixHeight();
	PIX pixMipSize   = pixWidth*pixHeight;
	PIX pixFrameSize = td_slFrameSize/BYTES_PER_TEXEL;
	INDEX iFrame;
	
	// test first mipmap for transparency (i.e. is one bit of alpha channel enough?)
	// (must test it before filtering and/or mipmap reduction gets to this texture)
	if( bAlphaChannel && !(td_ulFlags&TEX_TRANSPARENT)) 
	{
		td_ulFlags |= TEX_TRANSPARENT; 
		for( iFrame=0; iFrame<td_ctFrames; iFrame++) 
		{
			ULONG *pulCurrentFrame = td_pulFrames + iFrame*pixFrameSize;
			if( !IsTransparent( pulCurrentFrame, pixMipSize)) 
			{
				// no need to test other frames if one found that isn't gray
				td_ulFlags &= ~TEX_TRANSPARENT; 
				break; 
			}
		}
	}
	
	// generate texture mip-maps for each frame (in version 4 they're no longer kept in file)
	// and eventually adjust texture saturation, do filtering and/or dithering
	tex_iFiltering = Clamp( tex_iFiltering, -6L, +6L);
	INDEX iTexFilter = tex_iFiltering;
	if( (td_ulFlags&TEX_CONSTANT)) iTexFilter = 0; // don't filter constants and textures for exporting
	if( iTexFilter) td_ulFlags |= TEX_FILTERED;
	// make mipmaps
	for( iFrame=0; iFrame<td_ctFrames; iFrame++) 
	{ 
		ULONG *pulCurrentFrame = td_pulFrames + iFrame*pixFrameSize;
		MakeMipmaps( pulCurrentFrame, pixWidth,pixHeight, iTexFilter);
	}
	
	// remove mipmaps from texture that are not needed and update texture size
	RemoveOversizedMipmaps(this);
	
	// do some additional mipmap adjustments if needed
	pixWidth     = GetPixWidth();
	pixHeight    = GetPixHeight();
	pixMipSize   = pixWidth*pixHeight;
	pixFrameSize = td_slFrameSize/BYTES_PER_TEXEL;
	
	// eventually colorize mipmaps
	extern INDEX tex_bColorizeMipmaps;
	if( tex_bColorizeMipmaps && !(td_ulFlags&TEX_CONSTANT)) 
	{
		td_ulFlags |=  TEX_COLORIZED;
		td_ulFlags &= ~TEX_EQUALIZED; // colorized textures cannot be equilized
		for( iFrame=0; iFrame<td_ctFrames; iFrame++) 
		{
			ULONG *pulCurrentFrame = td_pulFrames + iFrame*pixFrameSize;
			ColorizeMipmaps( 1, pulCurrentFrame, pixWidth, pixHeight);
		}
	} // if not colorized, test if texture is gray
	else 
	{
		td_ulFlags |= TEX_GRAY; 
		for( iFrame=0; iFrame<td_ctFrames; iFrame++) 
		{
			ULONG *pulCurrentFrame = td_pulFrames + iFrame*pixFrameSize;
			if( !IsGray( pulCurrentFrame, pixMipSize)) 
			{
				// no need to test other frames if one found that isn't gray
				td_ulFlags &= ~TEX_GRAY; 
				break; 
			}
		}
	} // test texture for equality (i.e. determine if texture could be discardable in shade mode when in lowest mipmap)
	if( td_ctFineMipLevels>1 && !(td_ulFlags&TEX_EQUALIZED)) 
	{ 
		td_ulFlags |= TEX_EQUALIZED;
		const INDEX ctLastPixels = Max(pixWidth,pixHeight) / Min(pixWidth,pixHeight);
		ULONG *pulLastMipMap;
		for( iFrame=0; iFrame<td_ctFrames; iFrame++) 
		{
			// get last mipmap pointer of current frame
			ULONG *pulCurrentFrame = td_pulFrames + iFrame*pixFrameSize;
			pulLastMipMap = pulCurrentFrame + td_slFrameSize/BYTES_PER_TEXEL - ctLastPixels;
			if( !IsEqualized( pulLastMipMap, ctLastPixels)) 
			{
				// no need to test other frames if one found that isn't equalized
				td_ulFlags &= ~TEX_EQUALIZED;
				break;
			}
		}
	}
	// prepare dithering type
	td_ulInternalFormat = DetermineInternalFormat(this);
	tex_iDithering = Clamp( tex_iDithering, 0L, 10L); 
	INDEX iDitherType = 0;
	if( !(td_ulFlags&(TEX_STATIC|TEX_CONSTANT))) 
	{ // only non-static-constant textures can be dithered
		extern INDEX AdjustDitheringType_OGL( GLenum eFormat, INDEX iDitheringType);
		if( eAPI==GAT_OGL) iDitherType = AdjustDitheringType_OGL( (GLenum)td_ulInternalFormat, tex_iDithering);
		extern INDEX AdjustDitheringType_D3D( D3DFORMAT eFormat, INDEX iDitheringType);
		if( eAPI==GAT_D3D) iDitherType = AdjustDitheringType_D3D( (D3DFORMAT)td_ulInternalFormat, tex_iDithering);
	}
	// eventually dither texture
	if( iDitherType!=0) 
	{
		td_ulFlags |= TEX_DITHERED;
		for( iFrame=0; iFrame<td_ctFrames; iFrame++) 
		{
			ULONG *pulCurrentFrame = td_pulFrames + iFrame*pixFrameSize;
			DitherMipmaps( iDitherType, pulCurrentFrame, pulCurrentFrame, pixWidth, pixHeight);
		}
	}
	// upload texture if not static and API is active
	// (or maybe better not - this could cause reloading due to force() after obtain())
	if( bHasContext && !(td_ulFlags&TEX_STATIC)) SetAsCurrent();
}

static void read_png_data_callback( png_structp png_ptr, png_byte* raw_data, png_size_t read_length) 
{
	CTStream* handle = (CTStream*)png_get_io_ptr(png_ptr);
	handle->Read_t(raw_data, read_length);
}

void CTextureData::Read_PNG( CTStream* inFile )
{
//	png_byte color_type;
	png_byte bit_depth;

	png_structp png_ptr;
	png_infop info_ptr;
//	int number_of_passes;

	char header[8];    // 8 is the maximum size that can be checked

	/* open file and test for it being a png */
	memcpy(header, inFile->strm_pubBufferBegin, 8);

	if (png_sig_cmp((png_const_bytep)header, 0, 8))
	{
		CPrintF("[read_png_file] File %s is not recognized as a PNG file", inFile->GetDescription());
		return;
	}

	/* initialize stuff */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr)
	{
		CPrintF("[read_png_file] png_create_read_struct failed");
		return;
	}

	info_ptr = png_create_info_struct(png_ptr);
	
	if (!info_ptr)
	{
		CPrintF("[read_png_file] png_create_info_struct failed");
		return;
	}

// 	if (setjmp(png_jmpbuf(png_ptr)))
// 	{
// 		CPrintF("[read_png_file] Error during init_io");
// 		return;
// 	}

	inFile->SetPos_t(0);
	png_set_read_fn(png_ptr, inFile, read_png_data_callback);

//	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

 	td_mexWidth = png_get_image_width(png_ptr, info_ptr);
 	td_mexHeight = png_get_image_height(png_ptr, info_ptr);
// 	color_type = png_get_color_type(png_ptr, info_ptr);
 	bit_depth = png_get_bit_depth(png_ptr, info_ptr);
// 
// 	number_of_passes = png_set_interlace_handling(png_ptr);
 	png_read_update_info(png_ptr, info_ptr);

	/* read file */
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		CPrintF("[read_png_file] Error during read_image");
		return;
	}

	td_ulFlags = TEX_FILTERED | TEX_32BIT | TEX_ALPHACHANNEL;
	td_ctFrames = 1;
	td_ctFineMipLevels = 1;
	td_iFirstMipLevel = 0;

	SLONG slFrameSize = GetPixWidth() * GetPixHeight() * bit_depth;
	td_pulFrames = (ULONG*)AllocMemory( slFrameSize);
	
	td_ulInternalFormat = DetermineInternalFormat(this);

	const png_size_t row_size = png_get_rowbytes(png_ptr, info_ptr);
	png_uint_32 i;
	for (i = 0; i < td_mexHeight; i++) 
	{
		png_read_row(png_ptr, (png_bytep)(td_pulFrames + (i * td_mexWidth)), NULL);
	}

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	if (!(td_ulFlags&TEX_STATIC))
		SetAsCurrent();
}

// writes texutre to file
void CTextureData::Write_t( CTStream *outFile)   // throw char *
{
	TRACKTEX_HEAP();
	
	// cannot write textures that have been mangled somehow
	if( td_ptegEffect==NULL && IsModified()) throw( TRANS("Cannot write texture that has modified frames."));
	
	// must not have base texture with same name
	if( td_ptdBaseTexture != NULL) 
	{
		CTFileName fnTex = outFile->GetDescription();
		if( fnTex == td_ptdBaseTexture->GetName()) 
		{
			ThrowF_t( TRANS("Texture \"%s\" has same name as its base texture."), (CTString&)fnTex);
		}
	}
	
//안태훈 수정 시작	//(Encode Data)(0.1)
	// write version
	INDEX iVersion = 4;
	outFile->WriteID_t("TVER");
	INDEX iSaveVer = ((TEX_DATA_VER << 16) | 4);
	*outFile << iSaveVer;
	
	// isolate required flags
	const BOOL bCompress      = td_ulFlags & TEX_COMPRESS;
	const BOOL bCompressAlpha = td_ulFlags & TEX_COMPRESSALPHA;
	const BOOL bAlphaChannel  = td_ulFlags & TEX_ALPHACHANNEL;
	const BOOL bTransparent   = td_ulFlags & TEX_TRANSPARENT;
	ULONG ulFlags = td_ulFlags & (TEX_ALPHACHANNEL | TEX_32BIT | TEX_COMPRESSED | TEX_COMPRESSEDALPHA
		|  TEX_EQUALIZED | TEX_TRANSPARENT | TEX_STATIC | TEX_CONSTANT);
	if( bCompress)      ulFlags |= TEX_COMPRESSED;
	if( bCompressAlpha) ulFlags |= TEX_COMPRESSEDALPHA;
	
	// write chunk containing texture data
	outFile->WriteID_t( CChunkID("TDAT"));
	UBYTE checker = TEX_DATA_VER;
	// write data describing texture
	*outFile << EncodeSimpleTex(td_mexWidth, checker);
	*outFile << EncodeSimpleTex(td_iFirstMipLevel, checker);
	*outFile << EncodeSimpleTex(td_mexHeight, checker);
	*outFile << EncodeSimpleTex(td_ctFineMipLevels, checker);
	*outFile << EncodeSimpleTex(ulFlags, checker);
	*outFile << EncodeSimpleTex(td_ctFrames, checker);
//안태훈 수정 끝	//(Encode Data)(0.1)
	
	// if global effect struct exists in texture, don't save frames
	if( td_ptegEffect==NULL)
	{ 
		ASSERT( td_ctFrames>0);
		if( bCompress) 
		{
			// write chunk containing compressed frames
			SLONG slCompressedFrameSize;
			outFile->WriteID_t( CChunkID("FRMC"));
			UBYTE *pubCompressed = Compress( slCompressedFrameSize, bCompressAlpha);
			ASSERT( pubCompressed!=NULL);
			if( pubCompressed==NULL) throw( TRANS("Cannot compress texture."));
			*outFile << slCompressedFrameSize; // frame size store once for all frames
			outFile->Write_t( pubCompressed, slCompressedFrameSize*td_ctFrames);
			FreeMemory( pubCompressed);
		}
		else 
		{
			// write chunk containing raw frames
			ASSERT( td_pulFrames!=NULL);
			outFile->WriteID_t( CChunkID("FRMS"));
			const PIX pixFrSize = GetPixWidth()*GetPixHeight();
			// eventually prepare temp buffer in case of frames without alpha channel
			UBYTE *pubTmp = NULL;
			if( !bAlphaChannel) pubTmp = (UBYTE*)AllocMemory( pixFrSize*3);
			// write frames without mip-maps (just write the largest one)
			for( INDEX iFr=0; iFr<td_ctFrames; iFr++ )
			{ // determine write params
				ULONG *pulCurrentFrame = td_pulFrames + (iFr * td_slFrameSize/BYTES_PER_TEXEL);
				if( bAlphaChannel) 
				{ // write frame with alpha channel
					outFile->Write_t( pulCurrentFrame, pixFrSize *4);
				}
				else 
				{ // write frame without alpha channel
					RemoveAlphaChannel( pulCurrentFrame, pubTmp, pixFrSize);
					outFile->Write_t( pubTmp, pixFrSize *3);
				}
			}
			// no need for temp buffer anymore
			if( pubTmp!=NULL) FreeMemory(pubTmp);
		}
	}
	// if exists global effect struct in texture
	else 
	{ // write chunk containing effect data
		outFile->WriteID_t( CChunkID("FXDT"));
		// write effect class
		*outFile << td_ptegEffect->teg_ulEffectType;
		// write effect buffer dimensions
		*outFile << td_pixBufferWidth;
		*outFile << td_pixBufferHeight;
		// write count of effect sources
		*outFile << td_ptegEffect->teg_atesEffectSources.Count();
		
		// write whole dynamic array of effect sources
		FOREACHINDYNAMICARRAY(td_ptegEffect->teg_atesEffectSources, CTextureEffectSource, itEffectSource)
		{ // write type of effect source
			*outFile << itEffectSource->tes_ulEffectSourceType;
			// write structure holding effect source properties
			outFile->Write_t( &itEffectSource->tes_tespEffectSourceProperties, sizeof( struct TextureEffectSourceProperties));
			INDEX ctEffectSourcePixels = itEffectSource->tes_atepPixels.Count();
			// write count of effect pixels
			*outFile << ctEffectSourcePixels;
			// if there are any effect pixels
			if( ctEffectSourcePixels>0) 
			{
				// write all effect pixels in one block
				outFile->Write_t( &itEffectSource->tes_atepPixels[0], sizeof(struct TextureEffectPixel)*ctEffectSourcePixels);
			}
		}
		// if effect buffers are valid
		if( td_pubBuffer1!=NULL && td_pubBuffer2!=NULL)
		{ // write chunk containing effect buffers
			outFile->WriteID_t( CChunkID("FXB2"));
			ULONG ulSize = GetEffectBufferSize(this);
			// write effect buffers
			outFile->Write_t( td_pubBuffer1, ulSize);
			outFile->Write_t( td_pubBuffer2, ulSize);
		}
	}
	// write chunk containing texture animation data
	outFile->WriteID_t( CChunkID("ANIM"));
	// write corresponding animation(s)
	CAnimData::Write_t( outFile);
	
	// if this texture has base texture
	if( td_ptdBaseTexture != NULL)
	{ // write chunk containing base texture file name
		outFile->WriteID_t( CChunkID("BAST"));
		// write file name of base texture
		*outFile << td_ptdBaseTexture->GetName();
	}
	
	// reset the need for compression and mark if texture has been compressed
	td_ulFlags &= ~TEX_COMPRESS;
}


// export finest mipmap of one texture's frame to imageinfo
void CTextureData::Export_t( class CImageInfo &iiExportedImage, INDEX iFrame)
{
	TRACKTEX_HEAP();
	
	// check for right frame number and non-effect texture type
	ASSERT( iFrame<td_ctFrames && td_ptegEffect==NULL);
	if( iFrame>=td_ctFrames) throw( TRANS("Texture frame that is to be exported doesn't exist."));
	
	// reload without modifications
	const ULONG ulOriginalFlags = td_ulFlags;
	Force( TEX_STATIC|TEX_CONSTANT);
	ASSERT( td_pulFrames!=NULL);
	
	// prepare miplevel and mipmap offset
	const PIX pixWidth  = GetPixWidth();
	const PIX pixHeight = GetPixHeight();
	const BOOL bAlphaCh = td_ulFlags & TEX_ALPHACHANNEL;
	// export header to image info structure
	iiExportedImage.Clear();
	iiExportedImage.ii_Width  = pixWidth;
	iiExportedImage.ii_Height = pixHeight;
	iiExportedImage.ii_BitsPerPixel = bAlphaCh ? 32 : 24;
	
	// prepare the texture for exporting (with or without alpha channel)
	ULONG *pulFrame = td_pulFrames + td_slFrameSize*iFrame/BYTES_PER_TEXEL;
	PIX  pixMipSize = pixWidth*pixHeight;
	iiExportedImage.ii_Picture = (UBYTE*)AllocMemory( pixMipSize * iiExportedImage.ii_BitsPerPixel/8);
	// export frame
	if( bAlphaCh) CopyLongs( pulFrame, (ULONG*)iiExportedImage.ii_Picture, pixMipSize);
	else RemoveAlphaChannel( pulFrame, (UBYTE*)iiExportedImage.ii_Picture, pixMipSize);
	
	// reload as it was
	td_ulFlags = ulOriginalFlags;
	Reload();
}


// force texture to be re-loaded (if needed) in corresponding manner
void CTextureData::Force( ULONG ulTexFlags) 
{
	ASSERT( td_ctFrames>0);
	const BOOL bFlushed = td_pulFrames==NULL;
	const BOOL bAltered = td_ulFlags & (TEX_DISPOSED|TEX_DITHERED|TEX_FILTERED|TEX_COLORIZED);
	const BOOL bReload  = (bFlushed && (ulTexFlags&TEX_STATIC)) || (bAltered && (ulTexFlags&TEX_CONSTANT));
	td_ulFlags |= ulTexFlags & (TEX_STATIC|TEX_CONSTANT);
	// reload if required
	if( bReload) 
	{
		// go on ...
		Reload();
	}
}



// some compression routines statics
static UBYTE *_pubDXTFrames;
static SLONG _slDXTFramesOffset;

// just to keep nvDXTlib happy
void WriteDTXnFile( DWORD count, void *buffer) { count = 0; }
void ReadDTXnFile(  DWORD count, void *buffer) { count = 0; }


// copy each mipmap to our memory
static HRESULT MIPCallBack( void *data, int iMipLevel, DWORD dwSize)
{
	*(DWORD*)&_pubDXTFrames[_slDXTFramesOffset] = dwSize;
	_slDXTFramesOffset += 4;
	memcpy( &_pubDXTFrames[_slDXTFramesOffset], data, dwSize); 
	_slDXTFramesOffset += dwSize;
	return 0;
}


// compress texture (returns pointer to compressed frames)
UBYTE *CTextureData::Compress( SLONG &slCompressedFrameSize, BOOL bCompressAlpha)
{
#if defined(_MSC_VER) && (_MSC_VER < 1600)
	TRACKTEX_HEAP();
	
	const PIX pixTexWidth  = GetPixWidth();
	const PIX pixTexHeight = GetPixHeight();
	const BOOL bTransparent  = td_ulFlags & TEX_TRANSPARENT;
	const BOOL bAlphaChannel = td_ulFlags & TEX_ALPHACHANNEL;
	
	// dimension must be at least 4 pixels
	ASSERT( pixTexWidth>=4 && pixTexHeight>=4);
	if( pixTexWidth<4 || pixTexHeight<4) return FALSE;
	
	// determine texture format
	INDEX iFormat = dDXT1;
	if( bTransparent) iFormat = dDXT1a;
	else if( bAlphaChannel) iFormat = bCompressAlpha ? dDXT5 : dDXT3; 
	
	// set compressor options
	CompressionOptions co;
	memset( &co, 0, sizeof(co));
	co.bMipMapsInImage = FALSE;            
	co.MIPFilterType   = dMIPFilterBox;
	co.MipMapType      = dGenerateMipMaps; 
	co.TextureType     = dTextureType2D;
	co.TextureFormat   = iFormat;
	
	// for compressed frame(s), assume the same memory ammont as uncompressed (will be shrunk at the end!)
	_pubDXTFrames = (UBYTE*)AllocMemory(td_ctFrames*td_slFrameSize);
	_slDXTFramesOffset = 0;
	
	// need temp frame for intermediate color conversion
	UBYTE *pubFrameARGB = (UBYTE*)AllocMemory(td_slFrameSize);
	
	// loop thru frames
	const PIX pixFrameSize = td_slFrameSize / BYTES_PER_TEXEL;
	for( INDEX iFrame=0; iFrame<td_ctFrames; iFrame++) 
	{
		// compress (mipmaps will be created)
		ULONG *pulSrcFrame = td_pulFrames + pixFrameSize*iFrame;
		abgr2argb( pulSrcFrame, (ULONG*)pubFrameARGB, pixFrameSize);
		nvDXTcompress( pubFrameARGB, pixTexWidth, pixTexHeight, pixTexWidth*4, &co, 4, &MIPCallBack);
	}
	// calc size of one frame
	slCompressedFrameSize = _slDXTFramesOffset / td_ctFrames;
	
	// free memory if compression didn't make it
	if( slCompressedFrameSize==0) 
	{
		FreeMemory(_pubDXTFrames);
		_pubDXTFrames = NULL;
	}
	else 
	{
		// just shrink to exact compressed size
		ShrinkMemory( (void **)&_pubDXTFrames, _slDXTFramesOffset);  
	}
	
	// release temp frame memory
	FreeMemory( pubFrameARGB);
	return _pubDXTFrames;
#else	// _MSC_VER
	return NULL;
#endif	// _MSC_VER
}



// has texture been stored in compressed format
inline BOOL CTextureData::IsCompressed(void)
{
	return (td_ulInternalFormat==TS.ts_tfRGBc)
		|| (td_ulInternalFormat==TS.ts_tfRGBcA)
		|| (td_ulInternalFormat==TS.ts_tfRGBcAc);
}


// upload texture from compressed format
BOOL CTextureData::UploadCompressed(void)
{
	// check whether texture can be uploaded (i.e. driver supports compressed textures)
	if( !_pGfx->HasCompression()) return FALSE;
	TRACKTEX_HEAP();
	
	// internal texture format is, of course, compressed
	const BOOL bAlphaChannel  = td_ulFlags & TEX_ALPHACHANNEL;
	const BOOL bTransparent   = td_ulFlags & TEX_TRANSPARENT;
	const BOOL bCompressAlpha = td_ulFlags & TEX_COMPRESSEDALPHA;
	if( bTransparent || !bAlphaChannel) { td_ulInternalFormat = TS.ts_tfRGBc; }
	else { td_ulInternalFormat = bCompressAlpha ? TS.ts_tfRGBcAc : TS.ts_tfRGBcA; }
	
	// skip if current hardware doesn't support interpolated (compressed) alpha channel
	if((TS.ts_tfRGBcA == TS.ts_tfRGBcAc) && bCompressAlpha) return FALSE;
	
	{
		TRACKTEX_HW();
	
	// generate texture for gfx API if needed
	if( td_ulObject==NONE) 
	{
		// check whether frames are present
		ASSERT( td_pulFrames!=NULL && td_pulFrames[0]!=0xDEADBEEF); 
		if( td_ctFrames>1) 
		{ // animation textures
			td_pulObjects = (ULONG*)AllocMemory( td_ctFrames *sizeof(td_ulObject));
			for( INDEX i=0; i<td_ctFrames; i++) 
			{
				gfxGenerateTexture( td_pulObjects[i]);
			}
		}
		else 
		{ // single-frame textures
			gfxGenerateTexture( td_ulObject);
		}
	}
	
	// upload frame-by-frame
	td_tpLocal.tp_bSingleMipmap = (td_ctFineMipLevels<2);
	ULONG *pulTexObject = (td_ctFrames>1) ? td_pulObjects : &td_ulObject;
	for( INDEX iFr=0; iFr<td_ctFrames; iFr++)
	{ // determine frame offset and upload texture frame
		UBYTE *pubCurrentFrame = (UBYTE*)td_pulFrames + (iFr*td_slFrameSize);
		gfxSetTexture( pulTexObject[iFr], td_tpLocal);
		extern BOOL gfxUploadCompressedTexture( UBYTE *pubMipmaps, PIX pixWidth, PIX pixHeight, SLONG slSize, ULONG ulFormat);
		const BOOL bUploaded = gfxUploadCompressedTexture( pubCurrentFrame, GetPixWidth(), GetPixHeight(), 
			/* check */                                        td_slFrameSize, td_ulInternalFormat);
			if( !bUploaded) return FALSE;
	}
	}// track hw
	
	// free frames memory - it's all now up there, in the driver
	FreeMemory( td_pulFrames);
	td_pulFrames = NULL;
	return TRUE;
}



// set texture to be as current for accelerator and eventually upload it to accelerator's memory
void CTextureData::SetAsCurrent( INDEX iFrameNo/*=0*/, BOOL bForceUpload/*=FALSE*/)
{
	TRACKTEX_HEAP();
	
	// check API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);
	
	// skip non-existing texture
	// ASSERT( iFrameNo<td_ctFrames);
	if( iFrameNo>=td_ctFrames) return;
	
	// get initial parameters
	BOOL bNeedUpload = bForceUpload;
	BOOL bNoDiscard  = TRUE;
	PIX  pixWidth  = GetPixWidth();
	PIX  pixHeight = GetPixHeight();
	
	// eventually re-adjust LOD bias
	extern FLOAT _fCurrentLODBias;
	const FLOAT fWantedLODBias = _pGfx->gl_fTextureLODBias;
	extern void UpdateLODBias( const FLOAT fLODBias);
	if( td_ulFlags&TEX_CONSTANT) 
	{
		// non-adjustable textures don't tolerate positive LOD bias
		if( _fCurrentLODBias>0) UpdateLODBias(0);
		else if( _fCurrentLODBias>fWantedLODBias) UpdateLODBias(fWantedLODBias);
	} 
	else if( td_ulFlags&TEX_EQUALIZED) 
	{
		// equilized textures don't tolerate negative LOD bias
		if( _fCurrentLODBias<0) UpdateLODBias(0);
		else if( _fCurrentLODBias<fWantedLODBias) UpdateLODBias(fWantedLODBias);
	}
	else if( _fCurrentLODBias != fWantedLODBias) 
	{
		// all other textures must take LOD bias into account
		UpdateLODBias( fWantedLODBias);
	}
	
	// determine probing
	extern BOOL ProbeMode( CTimerValue tvLast);
	BOOL bUseProbe = ProbeMode(td_tvLastDrawn);
	
	// if we have an effect texture
	if( td_ptegEffect!=NULL)
	{ 
		ASSERT( iFrameNo==0); // effect texture must have only one frame
		// get max allowed effect texture dimension
		PIX pixClampAreaSize = 1L<<16L;
		tex_iEffectSize = Clamp( tex_iEffectSize, 4L, 8L);
		if( !(td_ulFlags&TEX_CONSTANT)) pixClampAreaSize = 1L<<(tex_iEffectSize*2);
		INDEX iWantedMipLevel = td_iFirstMipLevel
			+ ClampTextureSize( pixClampAreaSize, _pGfx->gl_pixMaxTextureDimension, pixWidth, pixHeight);
		// check whether wanted mip level is beyond last mip-level
		iWantedMipLevel = ClampMipLevel( iWantedMipLevel);
		
		// default adjustment for mapping
		pixWidth  >>= iWantedMipLevel-td_iFirstMipLevel;
		pixHeight >>= iWantedMipLevel-td_iFirstMipLevel;
		ASSERT( pixWidth>0 && pixHeight>0);
		
		// eventually adjust water effect texture size (if larger than base)
		if( td_ptegEffect->IsWater()) 
		{
			INDEX iMipDiff = Min( FastLog2(td_ptdBaseTexture->GetPixWidth())  - FastLog2(pixWidth),
				FastLog2(td_ptdBaseTexture->GetPixHeight()) - FastLog2(pixHeight));
			iWantedMipLevel = iMipDiff;
			if( iMipDiff<0) 
			{
				pixWidth  >>= (-iMipDiff);
				pixHeight >>= (-iMipDiff);
				iWantedMipLevel = 0;
				ASSERT( pixWidth>0 && pixHeight>0);
			}
		}
		// if current frame size differs from the previous one
		SLONG slFrameSize = GetMipmapOffset( 15, pixWidth, pixHeight) *BYTES_PER_TEXEL;
		if( td_pulFrames==NULL || td_slFrameSize!=slFrameSize) 
		{
			// (re)allocate the frame buffer
			if( td_pulFrames!=NULL) FreeMemory( td_pulFrames);
			td_pulFrames = (ULONG*)AllocMemory( slFrameSize);
			td_slFrameSize = slFrameSize;
			bNoDiscard = FALSE;
		}
		
		// if not calculated for this tick (must be != to test for time rewinding)
		if( td_ptegEffect->teg_updTexture.LastUpdateTime() != _pTimer->CurrentTick()) 
		{
			// discard eventual cached frame and calculate new frame
			MarkChanged();
			td_ptegEffect->Animate();
			bNeedUpload = TRUE;
			// make sure that effect and base textures are static
//안태훈 수정 시작	//(Create Effect Texture Bug Fix)(0.1)
//			ASSERT( td_ulFlags & TEX_STATIC);
//			ASSERT( td_ptdBaseTexture->td_ulFlags & TEX_STATIC);
			Force(TEX_STATIC);
			td_ptdBaseTexture->Force(TEX_STATIC);
//안태훈 수정 끝	//(Create Effect Texture Bug Fix)(0.1)
			// copy some flags from base texture to effect texture
			td_ulFlags |= td_ptdBaseTexture->td_ulFlags & (TEX_ALPHACHANNEL|TEX_TRANSPARENT|TEX_GRAY);
			// render effect texture
			td_ptegEffect->Render( iWantedMipLevel, pixWidth, pixHeight);
			// determine internal format
			ULONG ulNewFormat;
			if( td_ulFlags&TEX_GRAY) 
			{
				if( td_ulFlags&TEX_ALPHACHANNEL) ulNewFormat = TS.ts_tfLA8;
				else ulNewFormat = TS.ts_tfL8;
			}
			else 
			{
				if( td_ulFlags&TEX_TRANSPARENT) ulNewFormat = TS.ts_tfRGB5A1;
				else if( td_ulFlags&TEX_ALPHACHANNEL) ulNewFormat = TS.ts_tfRGBA4;
				else ulNewFormat = TS.ts_tfRGB5;
			}
			// effect texture can be in 32-bit quality only if base texture hasn't been dithered
			extern INDEX tex_bFineEffect;      
			if( tex_bFineEffect && (td_ptdBaseTexture->td_ulFlags&TEX_DITHERED)) 
			{
				ulNewFormat = PromoteTo32bitFormat(ulNewFormat);
			}
			// internal format changed? - must discard!
			if( td_ulInternalFormat!=ulNewFormat) 
			{
				td_ulInternalFormat = ulNewFormat;
				bNoDiscard = FALSE;
			}
		}
		// effect texture cannot have probing
		bUseProbe = FALSE;
	}
	
	// skip if internal format hasn't been determined yet
	if( td_ulInternalFormat==TEXFMT_NONE) return;
	
	// prepare effect cvars
	extern INDEX tex_bDynamicMipmaps;
	extern INDEX tex_iEffectFiltering; 
	if( tex_bDynamicMipmaps) tex_bDynamicMipmaps = 1;
	tex_iEffectFiltering = Clamp( tex_iEffectFiltering, -6L, +6L);
	
	// determine whether texture has single mipmap (effect textures are treated differently)
	if( td_ptegEffect!=NULL) td_tpLocal.tp_bSingleMipmap = !tex_bDynamicMipmaps;
	else td_tpLocal.tp_bSingleMipmap = (td_ctFineMipLevels<2);
	
	// effect texture might need dynamic mipmaps creation
	if( bNeedUpload && td_ptegEffect!=NULL) 
	{
		_sfStats.StartTimer(CStatForm::STI_EFFECTRENDER);
		const INDEX iTexFilter = td_ptegEffect->IsWater() ? NONE : tex_iEffectFiltering;  // don't filter water textures
		if( td_tpLocal.tp_bSingleMipmap) 
		{ // no mipmaps?
			if( iTexFilter!=NONE) FilterBitmap( iTexFilter, td_pulFrames, td_pulFrames, pixWidth, pixHeight);
		}
		else 
		{ // mipmaps!
			MakeMipmaps( td_pulFrames, pixWidth, pixHeight, iTexFilter);
		} // done with effect
		_sfStats.StopTimer(CStatForm::STI_EFFECTRENDER);
	} 
	
	// if not already generated, generate bind number(s) and force upload
	const PIX pixTextureSize = pixWidth*pixHeight;
	if( td_ulObject==NONE) 
	{
		// check whether frames are present
		ASSERT( td_pulFrames!=NULL && td_pulFrames[0]!=0xDEADBEEF); 
		if( td_ctFrames>1) 
		{
			// animation textures
			td_pulObjects = (ULONG*)AllocMemory( td_ctFrames *sizeof(td_ulObject));
			for( INDEX i=0; i<td_ctFrames; i++) gfxGenerateTexture( td_pulObjects[i]);
		}
		else 
		{
			// single-frame textures
			gfxGenerateTexture( td_ulObject);
		}
		// generate probe texture 
		ASSERT( td_ulProbeObject==NONE);
		if( td_ptegEffect==NULL && pixTextureSize>16*16) gfxGenerateTexture( td_ulProbeObject);
		// must do initial uploading
		bNeedUpload = TRUE;
		bNoDiscard  = FALSE;
	}
	
	// constant textures cannot be probed either
	if( td_ulFlags&TEX_CONSTANT) gfxDeleteTexture(td_ulProbeObject);
	if( td_ulProbeObject==NONE)  bUseProbe = FALSE;
	
	// update statistics if not updated already for this frame
	extern INDEX _iStatsMode;
	if( _iStatsMode>1 && td_iRenderFrame!=_pGfx->gl_iFrameNumber) 
	{
		td_iRenderFrame = _pGfx->gl_iFrameNumber;
		// determine size and update
		SLONG slBytes = pixWidth*pixHeight * gfxGetFormatPixRatio(td_ulInternalFormat);
		if( !td_tpLocal.tp_bSingleMipmap) slBytes = slBytes *4/3;
		_sfStats.IncrementCounter( CStatForm::SCI_TEXTUREBINDS, 1);
		_sfStats.IncrementCounter( CStatForm::SCI_TEXTUREBINDBYTES, slBytes);
	}
	
	// if needs to be uploaded
	if( bNeedUpload)
	{ 
		// check whether frames are present
		ASSERT( td_pulFrames!=NULL && td_pulFrames[0]!=0xDEADBEEF);
		TRACKTEX_HW();
		
		// must discard uploaded texture if single mipmap flag has been changed
		const BOOL bLastSingleMipmap = td_ulFlags & TEX_SINGLEMIPMAP;
		bNoDiscard = (bNoDiscard && bLastSingleMipmap==td_tpLocal.tp_bSingleMipmap);
		// update flag
		if( td_tpLocal.tp_bSingleMipmap) td_ulFlags |= TEX_SINGLEMIPMAP;
		else td_ulFlags &= ~TEX_SINGLEMIPMAP;
		
		// upload all texture frames
		ASSERT( td_ulInternalFormat!=TEXFMT_NONE);
		ULONG *pulTexObject = (td_ctFrames>1) ? td_pulObjects : &td_ulObject;
		for( INDEX iFr=0; iFr<td_ctFrames; iFr++)
		{ // determine frame offset and upload texture frame
			ULONG *pulCurrentFrame = td_pulFrames + (iFr * td_slFrameSize/BYTES_PER_TEXEL);
			const ULONG ulOldObject = pulTexObject[iFr];
			gfxSetTexture( pulTexObject[iFr], td_tpLocal);
			gfxUploadTexture( pulCurrentFrame, pixWidth, pixHeight, td_ulInternalFormat, bNoDiscard);
		}
		// upload probe texture if exist
		if( td_ulProbeObject!=NONE) 
		{
			PIX pixProbeWidth  = pixWidth;
			PIX pixProbeHeight = pixHeight;
			ULONG *pulProbeFrame = td_pulFrames;
			GetMipmapOfSize( 16*16, pulProbeFrame, pixProbeWidth, pixProbeHeight);
			gfxSetTexture( td_ulProbeObject, td_tpLocal);
			gfxUploadTexture( pulProbeFrame, pixProbeWidth, pixProbeHeight, TS.ts_tfRGBA4, FALSE);
		}
		// clear local texture parameters because we need to correct later texture settings
		td_tpLocal.Clear();
		// free frames' memory if allowed
		if(!(td_ulFlags&TEX_STATIC)) 
		{
			FreeMemory( td_pulFrames);
			td_pulFrames = NULL;
		}
		// done uploading
		ASSERT( td_ulObject!=NONE);
		return;
	}
	
	// do special case for animated textures when parameters re-initialization is required
	if( td_ctFrames>1 && !td_tpLocal.IsEqual(_tpGlobal[0])) 
	{
		// must reset local texture parameters for each frame of animated texture
		for( INDEX iFr=0; iFr<td_ctFrames; iFr++) 
		{
			td_tpLocal.Clear();
			gfxSetTexture( ((ULONG*)td_ulObject)[iFr], td_tpLocal);
		}
	} 
	
	// set corresponding probe or texture frame as current
	ULONG ulTexObject = td_ulObject; // single-frame
	if( td_ctFrames>1) ulTexObject = ((ULONG*)td_ulObject)[iFrameNo]; // animation
	if( bUseProbe) 
	{
		// set probe if burst value doesn't allow real texture
		if( _pGfx->gl_slAllowedUploadBurst<0) 
		{  
			CTexParams tpTmp = td_tpLocal;
			ASSERT( td_ulProbeObject!=NONE);
			gfxSetTexture( td_ulProbeObject, tpTmp);
			return;
		}
		// reduce allowed burst value
		_pGfx->gl_slAllowedUploadBurst -= pixWidth*pixHeight *4; // assume 32-bit textures (don't ask driver!)
	} 
	// set real texture and mark that this texture has been drawn
	gfxSetTexture( ulTexObject, td_tpLocal);
	MarkDrawn();
	
	// debug check
	ASSERT( td_ulObject!=NONE);
}



// unbind texture from accelerator's memory
void CTextureData::Unbind(void)
{
	// reset mark
	td_tvLastDrawn = 0I64;
	// only if bound
	if( td_ulObject==NONE) 
	{
		ASSERT( td_ulProbeObject==NONE);
		return;
	}
	// free frame number(s)
	if( td_ctFrames>1) 
	{
		// animation
		for( INDEX iFrame=0; iFrame<td_ctFrames; iFrame++) gfxDeleteTexture( td_pulObjects[iFrame]);
		FreeMemory(td_pulObjects);
		td_pulObjects = NULL;
	}
	else 
	{
		// single-frame
		gfxDeleteTexture(td_ulObject);
	}
	// free probe texture
	gfxDeleteTexture(td_ulProbeObject);
}


// free memory allocated for texture (if any)
void CTextureData::Clear(void)
{
	// unbind texture from OpenGL or Direct3D memory
	Unbind();
	
	// free allocated memory and reset pointer
	if( td_pulFrames!=NULL && td_slFrameSize!=0) 
	{
		FreeMemory( td_pulFrames);
		td_pulFrames = NULL;
		td_slFrameSize = 0;
	}
	
	// free memory allocated for texture effect buffers
	FreeEffectBuffers(this);
	
	// release base texture if it exists
	if( td_ptdBaseTexture != NULL) 
	{
		_pTextureStock->Release( td_ptdBaseTexture);
		td_ptdBaseTexture = NULL;
	}
	// free global effect data if it exists
	if( td_ptegEffect != NULL) 
	{
		delete td_ptegEffect;
		td_ptegEffect = NULL;
	}
	
	// reset texture parameters
	td_tpLocal.Clear();
	// clear animation
	CAnimData::Clear();
	
	// reset variables (but keep some flags)
	td_ctFrames = 0;
	td_mexWidth  = 0;
	td_mexHeight = 0;
	td_tvLastDrawn = 0I64;
	td_iFirstMipLevel  = 0;
	td_ctFineMipLevels = 0;
	td_pixBufferWidth  = 0;
	td_pixBufferHeight = 0;
	td_ulInternalFormat = TEXFMT_NONE;
	td_iRenderFrame = -1;
	td_ulFlags &= TEX_CONSTANT|TEX_STATIC;
}


#ifdef KALYDO
static void KCPTextureDownloaded(const char* fileName, TKResult result, void* id)
{
	switch (result)
	{
	case KR_OK:
		{
			SLS* pSLS = new SLS();
			pSLS->pTarget = reinterpret_cast<CSerial*>(id);
			pSLS->pTargetFilePath = fileName;
			g_deqLoadData.push_back( pSLS );
		}
		break;
	case KR_DOWNLOAD_FAILED:
	case KR_FILE_CORRUPT:
		krfRequestKCPFile(fileName, &KCPTextureDownloaded, id);
	//default:
		// unknown error!
	}
}

void CTextureData::Load_t(const CTFileName &fnFileName)
{
  ASSERT(!IsUsed());
  // mark that you have changed
  MarkChanged();
  // 근데 이게 확실한가?? 호출 매커니즘의 정확한 해명이 필요할 거 같다.
  TKResult tkResult = KR_OK;
  tkResult = krfRequestKCPFile( fnFileName, NULL, NULL );
  // if file exist in local disk.
  if( KR_OK == tkResult )
  {
	// open a stream
	CTFileStream istrFile;
	istrFile.Open_t(fnFileName);
	// read object from stream
	Read_t(&istrFile);
	// if still here (no exceptions raised)
	// remember filename
	ser_FileName = fnFileName;
  }
  else
  {
	CPrintF("Request file to kcp : %s\n", fnFileName );
	//?????????????
	CTFileStream istrFile;
	istrFile.Open_t( strDefaultTexturePath );
	Read_t(&istrFile);
	ser_FileName = fnFileName;
	if( tkResult == KR_FILE_NOT_AVAILABLE )
	{
		MarkUsed();
	}
	tkResult = krfRequestKCPFile(fnFileName, &KCPTextureDownloaded, this);
	if( KR_FILE_NOT_FOUND == tkResult )
	{
		CPrintF("[Load_t] Texture File Not Found in kalydo...\n" );
	}
	else if( KR_IO_PENDING == tkResult )
	{
		CPrintF("[Load_t] Texture File already request...\n" );
	}
	else
	{
		;
	}
  }  
}



void CTextureData::Load_Delay_t(const CTFileName &fnFileName)
{
  // mark that you have changed
  MarkChanged();
  // 근데 이게 확실한가?? 호출 매커니즘의 정확한 해명이 필요할 거 같다.

  //if( kfileExists( fnFileName ) )
	// open a stream
	CTFileStream istrFile;
	istrFile.Open_t(fnFileName);
	// read object from stream
	Read_t(&istrFile);
	// if still here (no exceptions raised)
	// remember filename
	ser_FileName = fnFileName;
	MarkUnused();
}


void CTextureData::Reload(void)
{
	/* if not found, */
	TRACKMEM(Mem, strrchr((const char*)ser_FileName, '.'));

	// mark that you have changed
	MarkChanged();

	CTFileName fnmOldName = ser_FileName;
	Clear();

	TKResult tkResult = krfRequestKCPFile( fnmOldName, NULL, NULL );
	if( KR_OK == tkResult )
	{
		// try to
		try {
		  // open a stream
		  CTFileStream istrFile;
		  istrFile.Open_t(fnmOldName);
		  // read object from stream
		  Read_t(&istrFile);

		// if there is some error while reloading
		} catch (char *strError) {
		  // quit the application with error explanation
		  FatalError(TRANS("Cannot reload file '%s':\n%s"), (CTString&)fnmOldName, strError);
		}

		// if still here (no exceptions raised)
		// remember filename
		ser_FileName = fnmOldName;
	}
	else
	{
		// it is already request in load_t()
		CPrintF("Request file to kcp : %s\n", fnmOldName );
		//?????????????
		CTFileStream istrFile;
		istrFile.Open_t( strDefaultTexturePath );
		Read_t(&istrFile);
		ser_FileName = fnmOldName;
	}
}

#else	// KALYDO

void CTextureData::Reload()
{
	CTFileName	strFile = ser_FileName;
	Clear();
	ser_FileName = strFile;
	CSerial::Reload();
}

#endif	// KALYDO



/*******************************************
* Implementation of CTextureObject routines
*/
CTextureObject::CTextureObject(void)
{
}
// copy from another object of same class
void CTextureObject::Copy(CTextureObject &toOther)
{
	CAnimObject::Copy(toOther);
}

void CTextureObject::Read_t( CTStream *istrFile)  // throw char *
{
	CAnimObject::Read_t( istrFile);
}

void CTextureObject::Write_t( CTStream *ostrFile)  // throw char *
{
	CAnimObject::Write_t( ostrFile);
}
MEX CTextureObject::GetWidth(void) const
{ return ((CTextureData*)ao_AnimData)->GetWidth();  };
MEX CTextureObject::GetHeight(void) const
{ return ((CTextureData*)ao_AnimData)->GetHeight(); };
ULONG CTextureObject::GetFlags(void) const
{ return ((CTextureData*)ao_AnimData)->GetFlags();  };

/****************************************
* Implementation of independent routines
*/
#define EQUAL_SUB_STR( str) (strnicmp( ld_line, str, strlen(str))==0)

void ProcessScript_t( const CTFileName &inFileName) // throw char *
{
	CTFileStream File;
	char ld_line[128];
	char err_str[256];
	FLOAT fTextureWidthMeters = 2.0f;
	INDEX TexMipmaps = (MAX_MEX_LOG2-2);
	CTextureData tex;
	CListHead FrameNamesList;
	INDEX NoOfDataFound = 0;
	BOOL bForce32bit = FALSE;
	BOOL bCompressed = FALSE;
	BOOL bCompressAlpha = FALSE;
	
	File.Open_t( inFileName, CTStream::OM_READ);    // open script file for text reading
	
	FOREVER
	{
		do 
		{
			File.GetLine_t( ld_line, 128);
		}	while( (strlen( ld_line)==0) || (ld_line[0]==';'));
		
		_strupr( ld_line);
		
		// specified width of texture
		if( EQUAL_SUB_STR( "TEXTURE_WIDTH")) 
		{
			sscanf( ld_line, "TEXTURE_WIDTH %g", &fTextureWidthMeters);
			NoOfDataFound ++;
		}
		// how many mip-map levels will texture have
		else if( EQUAL_SUB_STR( "TEXTURE_MIPMAPS")) 
		{
			sscanf( ld_line, "TEXTURE_MIPMAPS %d", &TexMipmaps);
		}
		// should texture be forced to keep 32-bit quality even that 16-bit textures are set
		else if( EQUAL_SUB_STR( "TEXTURE_32BIT")) 
		{
			bForce32bit = TRUE;
		}
		// should texture be compressed on disk
		else if( EQUAL_SUB_STR( "TEXTURE_COMPRESSED")) 
		{
			bCompressed = TRUE;
		}
		// should texture alpha channel be compressed also
		else if( EQUAL_SUB_STR( "TEXTURE_COMPRESSALPHA")) 
		{
			bCompressAlpha = TRUE;
		}
		
		// Key-word "ANIM_START" starts loading of Animation Data object
		else if( EQUAL_SUB_STR( "ANIM_START")) 
		{
			tex.LoadFromScript_t( &File, &FrameNamesList);
			NoOfDataFound ++;
		}
		// Key-word "END" ends infinite loop and script loading is over
		else if( EQUAL_SUB_STR( "END")) break;
		
		// if none of known key-words isn't recognised, throw error
		else 
		{
			sprintf( err_str,
				TRANS("Unidentified key-word found (line: \"%s\") or unexpected end of file reached."), ld_line);
			throw( err_str);
		}
	}
	if( NoOfDataFound != 2) 
	{
		throw( TRANS("Required key-word(s) has not been specified in script file:\nTEXTURE_WIDTH and/or ANIM_START"));
	}
	
	// Now we will create texture file form read script data
	CImageInfo   inPic;
	CTFileName   outFileName;
	CTFileStream outFile;
	
	// load first picture
	CFileNameNode *pFirstFNN = LIST_HEAD( FrameNamesList, CFileNameNode, cfnn_Node);
	inPic.LoadAnyGfxFormat_t( CTString(pFirstFNN->cfnn_FileName));
	
	// determine type (compressed overrides 32-bit)
	ULONG ulFlags = NONE;             // default
	if( bForce32bit) ulFlags |= TEX_32BIT;  // 32-bit
	if( bCompressed) ulFlags |= TEX_COMPRESS;
	if( bCompressAlpha) ulFlags |= TEX_COMPRESSALPHA;
	// create texture with one frame
	tex.Create_t( &inPic, MEX_METERS(fTextureWidthMeters), TexMipmaps, ulFlags);
	inPic.Clear();
	
	// process rest of the frames in animation (if any)
	BOOL bFirst = TRUE;
	FOREACHINLIST( CFileNameNode, cfnn_Node, FrameNamesList, it1) 
	{
		if( bFirst) 
		{  // skip first picture since it has already been done
			bFirst = FALSE;
			continue;
		} // add picture as next frame in texture
		inPic.LoadAnyGfxFormat_t( CTString(it1->cfnn_FileName));
		tex.AddFrame_t( &inPic);
		inPic.Clear();
	}
	// save texture
	outFileName = inFileName.FileDir() + inFileName.FileName() + ".TEX";
	tex.Save_t( outFileName);
	
	// clear list
	FORDELETELIST( CFileNameNode, cfnn_Node, FrameNamesList, itDel) delete &itDel.Current();
}


void CreateTexture_t( const CTFileName &inFileName, const CTFileName &outFileName,
					 MEX inMex, INDEX inMipmaps, ULONG ulFlags/*=NONE*/)
{
	if( inFileName.FileExt()==".SCR") 
	{
		// input is a script file
		ProcessScript_t( inFileName);
	}
	else 
	{
		// input is a picture file (PCX or TGA)
		CAnimData anim;
		CTextureData tex;
		CImageInfo inPic;
		// mex must be specified and valid
		if( inMex<=0) throw( TRANS("Invalid or unspecified mexel units."));
		// load picture
		inPic.LoadAnyGfxFormat_t( inFileName);
		// create texture
		tex.Create_t( &inPic, inMex, inMipmaps, ulFlags);
		// no more need for picture - get out!
		inPic.Clear();
		// save texture to file
		tex.Save_t( outFileName);
	}
}


void CreateTexture_t( const CTFileName &inFileName, MEX inMex, INDEX inMipmaps, ULONG ulFlags/*=NONE*/)
{
	CTFileName outFileName = inFileName.FileDir() + inFileName.FileName() + ".TEX";
	CreateTexture_t( inFileName, outFileName, inMex, inMipmaps, ulFlags);
}


// reference counting (override from CAnimData)
void CTextureData::RemReference_internal(void)
{
	_pTextureStock->Release(this);
}


// obtain texture and set it for this object
void CTextureObject::SetData_t(const CTFileName &fnmTexture) // throw char *
{
	// if the filename is empty
	if (fnmTexture=="") 
	{
		// release current texture
		SetData(NULL);
		
		// if the filename is not empty
	}
	else 
	{
		// obtain it (adds one reference)
		CTextureData *ptd = _pTextureStock->Obtain_t(fnmTexture);
		// set it as data (adds one more reference, and remove old reference)
		SetData(ptd);
		// release it (removes one reference)
		_pTextureStock->Release(ptd);
		// total reference count +1+1-1 = +1 for new data -1 for old data
	}
}


// get filename of texture or empty string if no texture
const CTFileName &CTextureObject::GetName(void)
{
	static const CTFileName strDummy(CTString(""));
	// if there is some texture
	if (ao_AnimData!=NULL) 
	{
		// get texture filename
		return ao_AnimData->GetName();
		// if there is no texture
	}
	else 
	{
		// get empty string
		return strDummy;
	}
}



// check if this kind of objects is auto-freed
BOOL CTextureData::IsAutoFreed(void)
{
	// cannot be
	return FALSE;
}



// get amount of memory used by this object
SLONG CTextureData::GetUsedMemory(void)
{
	// readout texture object
	ULONG ulTexObject = td_ulObject;
	if( td_ctFrames>1) ulTexObject = td_pulObjects[0];
	
	// add structure size and anim block size
	SLONG slUsed = sizeof(*this) + CAnimData::GetUsedMemory()-sizeof(CAnimData);
	// add effect buffers and static memory if exist
	if( td_pubBuffer1!=NULL) slUsed += 2* GetEffectBufferSize(this); // two buffers
	if( (td_ulFlags&TEX_STATIC) && td_pulFrames!=NULL) slUsed += td_ctFrames*td_slFrameSize;
	
	// add eventual uploaded size and finito
	const SLONG slUploadSize = gfxGetTextureSize( ulTexObject, !td_tpLocal.tp_bSingleMipmap);
	return slUsed + td_ctFrames*slUploadSize; 
}



// get texel from texture's largest mip-map
COLOR CTextureData::GetTexel( MEX mexU, MEX mexV)
{
	// if the texture is not static
	if( !(td_ulFlags&TEX_STATIC) && !(td_ulFlags&TEX_CONSTANT)) 
	{
		// print warning
		ASSERTALWAYS("GetTexel: Texture needs to be static and constant.");
		CPrintF("GetTexel: '%s' was not static and/or constant!\n", (const char*)GetName());
		// workaround: make sure that the texture is static
		Force(TEX_STATIC|TEX_CONSTANT);
	}
	
	// convert dimensions to pixels
	PIX pixU = mexU >>td_iFirstMipLevel;
	PIX pixV = mexV >>td_iFirstMipLevel;
	pixU &= GetPixWidth()-1;
	pixV &= GetPixHeight()-1;
	ASSERT(pixU>=0 && pixU<GetPixWidth());
	ASSERT(pixV>=0 && pixV<GetPixHeight());
	// read texel from texture
	return ByteSwap( *(ULONG*)(td_pulFrames + pixV*GetPixWidth() + pixU));
}


// copy (and eventually convert to floats) one row from texture to an array (iChannel is 1=R,2=G,3=B,4=A)
void CTextureData::FetchRow( PIX pixRow, void *pvDst, INDEX iChannel/*=4*/, BOOL bConvertToFloat/*=TRUE*/)
{
	// if the texture is not static
	if( !(td_ulFlags&TEX_STATIC) && !(td_ulFlags&TEX_CONSTANT)) 
	{
		// print warning
		ASSERTALWAYS("FetchRow: Texture needs to be static and constant.");
		CPrintF("FetchRow: '%s' was not static and/or constant!\n", (const char*)GetName());
		// workaround: make sure that the texture is static
		Force(TEX_STATIC|TEX_CONSTANT);
	}
	
	// determine row offset and loop thru row pixels
	ULONG *pulSrc = td_pulFrames + pixRow*GetPixWidth();
	for( INDEX iCol=0; iCol<GetPixWidth(); iCol++) 
	{
		const UBYTE ubPix = ((UBYTE*)pulSrc)[iCol*4 +iChannel-1];
		if( bConvertToFloat) ((FLOAT*)pvDst)[iCol] = NormByteToFloat(ubPix);
		else                 ((UBYTE*)pvDst)[iCol] = ubPix;
	}
}


// get pointer to one row of texture
ULONG *CTextureData::GetRowPointer( PIX pixRow)
{
	// texture must not be compressed
	ASSERT( !(td_ulFlags&TEX_COMPRESSED));
	// if the texture is not static
	if( !(td_ulFlags&TEX_STATIC) && !(td_ulFlags&TEX_CONSTANT)) 
	{
		// print warning
		ASSERTALWAYS("GetRowPointer: Texture needs to be static and constant.");
		CPrintF("GetRowPointer: '%s' was not static and/or constant!\n", (const char*)GetName());
		// workaround: make sure that the texture is static
		Force(TEX_STATIC|TEX_CONSTANT);
	}
	// done
	return (td_pulFrames + pixRow*GetPixWidth());
}


// get string description of texture size, mips and parameters
CTString CTextureData::GetDescription(void)
{
	// get all parameters
	MEX mexSizeU = GetWidth();
	MEX mexSizeV = GetHeight();
	PIX pixSizeU = GetPixWidth();
	MEX pixSizeV = GetPixHeight();
	FLOAT fSizeU = METERS_MEX( mexSizeU);
	FLOAT fSizeV = METERS_MEX( mexSizeV);
	INDEX ctFineMips  = GetNoOfFineMips();
	INDEX ctTotalMips = GetNoOfMips();
	
	// print size and mips
	CTString strSizeM;
	if (fSizeU==int(fSizeU) && fSizeV==int(fSizeV)) 
	{
		strSizeM.PrintF("%dx%dm", int(fSizeU), int(fSizeV));
	}
	else 
	{
		strSizeM.PrintF("%.2fx%.2fm", fSizeU, fSizeV);
	}
	CTString str;
	str.PrintF( "%s(%dx%d) %d/%d", strSizeM, pixSizeU, pixSizeV, ctFineMips, ctTotalMips);
	
	// print flags
	CTString strFlags = "";
	if( td_ulFlags & TEX_WASOLD)       strFlags += "!";
	if( td_ulFlags & TEX_TRANSPARENT)  strFlags += "T";
	if( td_ulFlags & TEX_ALPHACHANNEL) strFlags += "A";
	if( td_ulFlags & TEX_EQUALIZED)    strFlags += "=";
	if( td_ulFlags & TEX_32BIT)        strFlags += "H";  // "high quality"
	if( td_ulFlags & TEX_COMPRESSED)   strFlags += "C";  // compressed on disk (and in memory, of course)
	else if( IsCompressed())           strFlags += "c";  // compressed only in memory
	if( td_ptegEffect!=NULL)           strFlags += "E";  // effect textures
	// (flags in memory)
	if( td_ulFlags & TEX_CONSTANT)     strFlags += "*";
	if( td_ulFlags & TEX_GRAY)         strFlags += "g";
	if( td_ulFlags & TEX_DISPOSED)     strFlags += "x";  // cut
	if( td_ulFlags & TEX_STATIC) 
	{
		if( td_ulObject) strFlags += "S";   // uploaded
		else             strFlags += "s";   // not uploaded
	} 
	
	// if there are any flags, add blank before flags
	if( strFlags!="") str = CTString(" ") + str;
	
	CAnimInfo aiInfo;
	GetAnimInfo( 0, aiInfo);
	CTString strAnims = "";
	if (ad_NumberOfAnims>1 || aiInfo.ai_NumberOfFrames>1) 
	{
		strAnims.PrintF(" %d(%d)anim", ad_NumberOfAnims, aiInfo.ai_NumberOfFrames);
	}
	
	// return combined string
	return str+strFlags+strAnims;
}
