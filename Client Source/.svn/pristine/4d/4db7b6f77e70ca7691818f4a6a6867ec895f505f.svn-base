
#ifndef __DXTLIB_H__
#define __DXTLIB_H__
#ifdef PRAGMA_ONCE
  #pragma once
#endif


/*********************************************************************NVMH2****
Path:  C:\Dev\devrel\Nv_sdk_4\Dx8_private\PhotoShop\dxtlib
File:  dxtlib.h

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

******************************************************************************/


enum
{
  dSaveButton = 1,
  dCancelButton = 2,

	dDXT1 = 10,
	dTextureFormatFirst = dDXT1,

	dDXT1a = 11,  // DXT1 with one bit alpha
	dDXT3  = 12,  // explicit alpha
	dDXT5  = 13,  // interpolated alpha

	d4444 = 14,   // a4 r4 g4 b4
	d1555 = 15,   // a1 r5 g5 b5
	d565  = 16,   // a0 r5 g6 b5
	d8888 = 17,   // a8 r8 g8 b8
	d888  = 18,   // a0 r8 g8 b8
	d555  = 19,   // a0 r5 g5 b5
  d8    = 20,   // paletted

  dNVHS = 21,   // signed HILO TODO
  dNVHU = 22,   // unsighed HILO TODO

	dTextureFormatLast = dNVHU,

  // 3d viewing options
  d3DPreviewButton = 300, 

  dViewDXT1 = 200,
  dViewDXT2 = 201,
  dViewDXT3 = 202,
  dViewDXT5 = 203,
  dViewA4R4G4B4 = 204,
  dViewA1R5G5B5 = 205,
  dViewR5G6B5   = 206,
  dViewA8R8G8B8 = 207,

  dGenerateMipMaps = 30,
  dMIPMapSourceFirst = dGenerateMipMaps,
	dSpecifyMipMaps = 31,
	dUseExistingMipMaps = 32,
	dNoMipMaps = 33,
  dMIPMapSourceLast = dNoMipMaps,
  dSpecifiedMipMaps = 39,

  // MIP filters
  dMIPFilterBox = 133,
  dMIPFilterFirst = dMIPFilterBox,
  dMIPFilterCubic = 134,
  dMIPFilterFullDFT = 135,
  dMIPFilterKaiser = 136,
  dMIPFilterLinearLightKaiser = 137,
  dMIPFilterLast = dMIPFilterLinearLightKaiser,

  dShowDifferences = 40,
  dShowFiltering = 41,
  dShowMipMapping = 42,
  dShowAnisotropic = 43,

  dChangeClearColorButton = 50,
  dViewXBOX1c = 51,
  dViewXBOX1a = 52,
  dDitherColor = 53,

  dLoadBackgroundImageButton = 54,
  dUseBackgroundImage = 55,

  dBinaryAlpha = 56,
  dAlphaBlending = 57,
  dFadeColor = 58,
  dFadeAlpha = 59,

  dFadeToColorButton = 60,
  dAlphaBorder = 61,
  dBorder = 62,
  dBorderColorButton = 63,
	dNormalMap = 64,
	dDuDvMap = 65,
  dDitherEachMIPLevel = 66,
  dGreyScale = 67,

  dZoom = 70,

	dTextureType2D = 80,
	dTextureTypeFirst = dTextureType2D,

	dTextureTypeCube   = 81,
	dTextureTypeImage  = 82,
//dTextureTypeVolume = 83,  to be added
	dTextureTypeLast   = dTextureTypeImage,

  dFadeAmount  = 90,
  dFadeToAlpha = 91,
  dFadeToDelay = 92,

  dAskToLoadMIPMaps = 400,
  dShowAlphaWarning = 401,
  dShowPower2Warning = 402,

  dAdvancedBlendingButton = 500,
  dUserSpecifiedFadingAmounts = 501
};



#ifndef TRGBA
#define TRGBA
typedef	struct	
{
	BYTE rgba[4];
} rgba_t;
#endif

#ifndef TPIXEL
#define TPIXEL
union tPixel
{
  unsigned long u;
  rgba_t c;
};
#endif


// Windows handle for our plug-in (seen as a dynamically linked library):
extern HANDLE hDllInstance;
class CMyD3DApplication;

typedef enum RescaleOption
{
  RESCALE_NONE,             // no rescale
  RESCALE_NEAREST_POWER2,   // rescale to nearest power of two
  RESCALE_BIGGEST_POWER2,   // rescale to next bigger power of 2
  RESCALE_SMALLEST_POWER2,  // rescale to next smaller power of 2
} RescaleOption;


typedef struct CompressionOptions
{
    RescaleOption bRescaleImageToPower2; 
    bool  bMipMapsInImage;    // mip have been loaded in during read
    short MipMapType;         // dNoMipMaps, dSpecifyMipMaps, dUseExistingMipMaps, dGenerateMipMaps
    short SpecifiedMipMaps;   // if dSpecifyMipMaps is set (number of mipmaps to generate)

    short MIPFilterType;      // for MIP maps
    /* for MIPFilterType, specify one of:
      dMIPFilterBox 
      dMIPFilterCubic 
      dMIPFilterFullDFT 
      dMIPFilterKaiser 
      dMIPFilterLinearLightKaiser 
    */

    bool bBinaryAlpha;         // zero or one alpha channel

    bool bNormalMap;           // Is a normal Map
    bool bDuDvMap;             // Is a DuDv (EMBM) map

    bool   bAlphaBorder;       // make an alpha border
    bool   bBorder;            // make a color border
    tPixel BorderColor;        // color of border

    bool   bFadeColor;         // fade color over MIP maps
    bool   bFadeAlpha;         // fade alpha over MIP maps
    tPixel FadeToColor;        // color to fade to
    int    FadeToAlpha;        // alpha value to fade to (0-255)
    int    FadeToDelay;        // start fading after 'n' MIP maps
    int    FadeAmount;         // percentage of color to fade in

    bool bDitherColor;         // enable dithering during 16 bit conversion
    bool bDitherEachMIPLevel;  // enable dithering during 16 bit conversion for each MIP level (after filtering)
    bool bGreyScale;           // treat image as a grey scale


	short TextureType;        // regular decal, cube or volume  
	/* for TextureType, specify one of:
    dTextureType2D 
    dTextureTypeCube 
    dTextureTypeImage 
  */

	short TextureFormat;	    
  /* for TextureFormat, specify one of:
    dDXT1, 
    dDXT1a, 
    dDXT3, 
    dDXT5, 
    d4444, 
    d1555, 	
    d565,	
    d8888, 	
    d888, 
    d555, 
    not supported yet dNVHS, dNVHU
  */
  bool bSwapRGB;           // swap color positions R and G

} CompressionOptions;



typedef HRESULT (*MIPcallback)(void * data, int miplevel, DWORD size);
// call back
// pointer to data
// mip level
// size of chunk



/*
   Compresses an image with a user supplied callback with the data for each MIP level created
   Only supports input of RGB 24 or ARGB 32 bpp
*/
HRESULT nvDXTcompress( unsigned char *raw_data, // pointer to data (24 or 32 bit)
                       unsigned long w, // width  in texels
                       unsigned long h, // height in texels
                       DWORD byte_pitch,
                       CompressionOptions *options,
                       DWORD planes, // 3 or 4
                       MIPcallback callback = 0);   // callback for generated levels
// if callback is == 0 (or not specified), then WriteDTXnFile is called with all file info
//
// You must write the routines (or provide stubs)
// void WriteDTXnFile(count, buffer);
// void ReadDTXnFile(count, buffer);
// 
void WriteDTXnFile( DWORD count, void *buffer);
void ReadDTXnFile(  DWORD count, void *buffer);


#define DXTERR_INPUT_POINTER_ZERO -1
#define DXTERR_DEPTH_IS_NOT_3_OR_4 -2
#define DXTERR_NON_POWER_2 -3


/* example

LPDIRECT3DTEXTURE8 pCurrentTexture = 0; 

HRESULT LoadAllMipSurfaces(void * data, int iLevel)
{
    HRESULT hr;
    LPDIRECT3DSURFACE8 psurf;
    D3DSURFACE_DESC sd;
    D3DLOCKED_RECT lr;
       
    hr = pCurrentTexture->GetSurfaceLevel(iLevel, &psurf);
    
    if (FAILED(hr))
        return hr;
    psurf->GetDesc(&sd);
    
    
    hr = pCurrentTexture->LockRect(iLevel, &lr, NULL, 0);
    if (FAILED(hr))
        return hr;
    
    memcpy(lr.pBits, data, sd.Size);
    
    hr = pCurrentTexture->UnlockRect(iLevel);
    
    ReleasePpo(&psurf);
    
    return 0;
}
       

    hr = D3DXCreateTexture(m_pd3dDevice, Width, Height, nMips,  0,   D3DFMT_DXT3,  D3DPOOL_MANAGED, &pCurrentTexture);
    nvDXTcompress(raw_data, Width, Height, DXT3, true, 4, LoadAllMipSurfaces);

*/

/* src_format
  dDXT1 
	dDXT1a   // DXT1 with one bit alpha
	dDXT3    // explicit alpha
	dDXT5    // interpolated alpha

	d4444   // a4 r4 g4 b4
	d1555   // a1 r5 g5 b5
	d565    // a0 r5 g6 b5
	d8888   // a8 r8 g8 b8
	d888    // a0 r8 g8 b8
	d555    // a0 r5 g5 b5
  dNVHS   // signed HILO
  dNVHU   // unsighed HILO
  d8      // paletted
*/

unsigned char *nvDXTdecompress( int &w, int &h, int &depth, int &total_width, int &rowBytes, int &src_format);

enum ColorFormat
{
	COLOR_RGB,
	COLOR_ARGB,
	COLOR_BGR,
	COLOR_BGRA,
	COLOR_RGBA,
	COLOR_ABGR,
};

#endif
