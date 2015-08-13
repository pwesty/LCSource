/*
 * File:	ximalctex.cpp
 * Purpose:	Platform Independent LastChaos TEX Image Class Loader and Writer
 * 13/Jun/2012 sykim70 - www.barunsongames.com
 */

#include "ximalctex.h"
#include <string>
using namespace std;

#if CXIMAGE_SUPPORT_LC_TEX

#include "xmemfile.h"

#define SAFE_FREE(buf) if (buf) { free(buf); buf = NULL; }

void LOG_PRINT(char *pFormat, ...)
{
	va_list va = NULL;
	va_start(va, pFormat);

	CHAR* buf = NULL;
	UINT bufLen = 128;
	while (1)
	{
		buf = (CHAR*)malloc(bufLen);
		int ret = _vsnprintf(buf, bufLen, pFormat, va);
		if (ret != -1)
			break;
		bufLen *= 2;
		buf = (CHAR*)realloc(buf, bufLen);
	}

	va_end(va);

	if (buf)
		OutputDebugString(buf);
	
	free(buf);
}
#define LOG LOG_PRINT
//#define LOG 0

//////////////////////////////////////////////////////////////////////////

template<class Type>
__forceinline Type Min(const Type a, const Type b)
{
	return a > b ? b : a;
}

template<class Type>
__forceinline Type ClampDn(const Type x, const Type dnlimit)
{
	return x >= dnlimit ? x : dnlimit;
}

// returns accurate values only for integers that are power of 2
__forceinline LONG FastLog2(LONG x)
{
	return LONG(log((double)x) / log(2.0));
}

// retrives number of mip-maps for the given bitmap size
__forceinline INT GetNoOfMipmaps(LONG pixWidth, LONG pixHeight)
{
	return FastLog2(Min(pixWidth, pixHeight)) + 1;
}

//////////////////////////////////////////////////////////////////////////

typedef ULONG  COLOR;  // color is always in 32 bit true-color format

// CT RGBA masks and shifts
#define CT_RMASK  0xFF000000UL
#define CT_GMASK  0x00FF0000UL
#define CT_BMASK  0x0000FF00UL
#define CT_AMASK  0x000000FFUL
#define CT_RSHIFT 24
#define CT_GSHIFT 16
#define CT_BSHIFT  8
#define CT_ASHIFT  0

// combine CroTeam COLOR format from separate R,G and B color components
__forceinline COLOR RGBAToColor( BYTE const ubR, BYTE const ubG, BYTE const ubB, BYTE const ubA) {
  return ((ULONG)ubR<<CT_RSHIFT) | ((ULONG)ubG<<CT_GSHIFT)
       | ((ULONG)ubB<<CT_BSHIFT) | ((ULONG)ubA<<CT_ASHIFT);
}

__forceinline ULONG ByteSwap(ULONG ul)
{
	return( ((ul << 24)            ) |
            ((ul << 8) & 0x00FF0000) |
            ((ul >> 8) & 0x0000FF00) |
            ((ul >> 24)            ) );
}

//////////////////////////////////////////////////////////////////////////

struct DXTColBlock
{
	WORD col0;
	WORD col1;
	BYTE row[4];      // no bit fields - use bytes
};

struct DXTAlphaBlockExplicit
{
	WORD row[4];
};

struct DXTAlphaBlock3BitLinear
{
	BYTE alpha0;
	BYTE alpha1;
	BYTE stuff[6];
};

// use cast to struct instead of RGBA_MAKE as struct is
//  much
struct Color8888
{
	BYTE r;   // change the order of names to change the 
	BYTE g;   //  order of the output ARGB or BGRA, etc...
	BYTE b;   //  Last one is MSB, 1st is LSB.
	BYTE a;
};

struct Color565
{
	unsigned nBlue  : 5;    // order of names changes
	unsigned nGreen : 6;    // byte order of output to 32 bit
	unsigned nRed   : 5;
};

inline void GetColorBlockColors(DXTColBlock* pBlock, Color8888* col_0, Color8888* col_1, 
                                Color8888* col_2, Color8888* col_3)
{
	// There are 4 methods to use - see the Time_ functions.
	// 1st = shift = does normal approach per byte for color comps
	// 2nd = use freak variable bit field color565 for component extraction
	// 3rd = use super-freak DWORD adds BEFORE shifting the color components
	//  This lets you do only 1 add per color instead of 3 BYTE adds and
	//  might be faster
	// Call RunTimingSession() to run each of them & output result to txt file


	// freak variable bit structure method
	// normal math
	// This method is fastest

	Color565* pCol;

	pCol = (Color565*)&(pBlock->col0);
	col_0->a = 0xFF;
	col_0->r = pCol->nRed;   col_0->r = (col_0->r<<3) | (col_0->r>>2);   // shift to full precision
	col_0->g = pCol->nGreen; col_0->g = (col_0->g<<2) | (col_0->g>>4);
	col_0->b = pCol->nBlue;  col_0->b = (col_0->b<<3) | (col_0->b>>2);

	pCol = (Color565*)&(pBlock->col1);
	col_1->a = 0xFF;
	col_1->r = pCol->nRed;   col_1->r = (col_1->r<<3) | (col_1->r>>2);
	col_1->g = pCol->nGreen; col_1->g = (col_1->g<<2) | (col_1->g>>4);
	col_1->b = pCol->nBlue;  col_1->b = (col_1->b<<3) | (col_1->b>>2);

	if (pBlock->col0 > pBlock->col1)
	{
		// Four-color block: derive the other two colors.    
		// 00 = color_0, 01 = color_1, 10 = color_2, 11 = color_3
		// These two bit codes correspond to the 2-bit fields 
		// stored in the 64-bit block.
		col_2->r = ((WORD)col_0->r * 2 + (WORD)col_1->r) / 3;  // no +1 for rounding as bits have been shifted to 888
		col_2->g = ((WORD)col_0->g * 2 + (WORD)col_1->g) / 3;
		col_2->b = ((WORD)col_0->b * 2 + (WORD)col_1->b) / 3;
		col_2->a = 0xFF;
		col_3->r = ((WORD)col_0->r + (WORD)col_1->r * 2) / 3;
		col_3->g = ((WORD)col_0->g + (WORD)col_1->g * 2) / 3;
		col_3->b = ((WORD)col_0->b + (WORD)col_1->b * 2) / 3;
		col_3->a = 0xFF;
	}
	else
	{
		// Three-color block: derive the other color.
		// 00 = color_0,  01 = color_1,  10 = color_2,  
		// 11 = transparent.
		// These two bit codes correspond to the 2-bit fields 
		// stored in the 64-bit block. 
		// explicit for each component, unlike some refrasts...

		col_2->r = ((WORD)col_0->r + (WORD)col_1->r) / 2;
		col_2->g = ((WORD)col_0->g + (WORD)col_1->g) / 2;
		col_2->b = ((WORD)col_0->b + (WORD)col_1->b) / 2;
		col_2->a = 0xFF;

		col_3->r = 0x00;    // random color to indicate alpha
		col_3->g = 0xFF;
		col_3->b = 0xFF;
		col_3->a = 0x00;
	}
}

inline void DecodeColorBlock(DWORD* pImPos, DXTColBlock* pColorBlock, int width,  // width is width of image in pixels
                             DWORD* col_0, DWORD* col_1, DWORD* col_2, DWORD* col_3)
{
	// bit masks = 00000011, 00001100, 00110000, 11000000
	const DWORD masks[] = { 3<<0, 3<<2, 3<<4, 3<<6 };
	const int   shift[] = { 0, 2, 4, 6 };

	// r steps through lines in y
	for (int r = 0; r < 4; r++) { // no width*4 as DWORD ptr inc will *4
		// width * 4 bytes per pixel per line
		// each j dxtc row is 4 lines of pixels
		// pImPos = (DWORD*)((DWORD)pBase + i*16 + (r+j*4) * m_nWidth * 4 );
		// n steps through pixels
		for (int n = 0; n < 4; n++) {
			const DWORD bits = (pColorBlock->row[r] & masks[n]) >> shift[n];
			switch (bits) {
			case 0: *pImPos = *col_0; break;
			case 1: *pImPos = *col_1; break;
			case 2: *pImPos = *col_2; break;
			case 3: *pImPos = *col_3; break;
			default: /*ASSERT(FALSE);*/ break;
			}
			// next texel
			pImPos++;
		}
		// next row
		pImPos += width - 4;
	}
}

inline void DecodeAlphaExplicit(DWORD* pImPos, DXTAlphaBlockExplicit* pAlphaBlock, int width)
{
	// decodes to 32 bit format only
	WORD wrd;
	Color8888 col;
	col.r = col.g = col.b = 0;

	for (int row = 0; row < 4; row++) {
		// pImPow += pImPos += width-4 moves to next row down
		wrd = pAlphaBlock->row[row];
		for (int pixel = 0; pixel < 4; pixel++) {
			// zero the alpha bits of image pixel
			*pImPos &= 0x00FFFFFF;
			col.a = wrd & 0x000F;   // get only low 4 bits
			col.a = col.a | (col.a<<4);
			*pImPos |= *((DWORD*)&col); // or the bits into the prev. nulled alpha
			wrd >>= 4;    // move next bits to lowest 4
			pImPos++;     // move to next pixel in the row
		}
		// next row
		pImPos += width-4;
	}
}

WORD      gAlphas[8];
BYTE      gBits[4][4];
Color8888 gACol[4][4];

inline void DecodeAlpha3BitLinear(DWORD* pImPos, DXTAlphaBlock3BitLinear* pAlphaBlock, int width)
{
	gAlphas[0] = pAlphaBlock->alpha0;
	gAlphas[1] = pAlphaBlock->alpha1;

	// 8-alpha or 6-alpha block?    
	if (gAlphas[0] > gAlphas[1]) {
		// 8-alpha block:  derive the other 6 alphas.    
		// 000 = alpha_0, 001 = alpha_1, others are interpolated
		gAlphas[2] = (6 * gAlphas[0] + 1 * gAlphas[1] + 3) / 7;    // bit code 010
		gAlphas[3] = (5 * gAlphas[0] + 2 * gAlphas[1] + 3) / 7;    // Bit code 011    
		gAlphas[4] = (4 * gAlphas[0] + 3 * gAlphas[1] + 3) / 7;    // Bit code 100    
		gAlphas[5] = (3 * gAlphas[0] + 4 * gAlphas[1] + 3) / 7;    // Bit code 101
		gAlphas[6] = (2 * gAlphas[0] + 5 * gAlphas[1] + 3) / 7;    // Bit code 110    
		gAlphas[7] = (1 * gAlphas[0] + 6 * gAlphas[1] + 3) / 7;    // Bit code 111
	}    
	else {
		// 6-alpha block:  derive the other alphas.    
		// 000 = alpha_0, 001 = alpha_1, others are interpolated
		gAlphas[2] = (4 * gAlphas[0] + 1 * gAlphas[1] + 2) / 5;    // Bit code 010
		gAlphas[3] = (3 * gAlphas[0] + 2 * gAlphas[1] + 2) / 5;    // Bit code 011    
		gAlphas[4] = (2 * gAlphas[0] + 3 * gAlphas[1] + 2) / 5;    // Bit code 100    
		gAlphas[5] = (1 * gAlphas[0] + 4 * gAlphas[1] + 2) / 5;    // Bit code 101
		gAlphas[6] = 0;                                           // Bit code 110
		gAlphas[7] = 255;                                         // Bit code 111
	}

	// Decode 3-bit fields into array of 16 BYTES with same value

	// first two rows of 4 pixels each:
	// pRows = (Alpha3BitRows*) & ( pAlphaBlock->stuff[0] );
	const DWORD mask = 0x00000007;    // bits = 00 00 01 11
	DWORD bits = *((DWORD*)&(pAlphaBlock->stuff[0]));

	gBits[0][0] = (BYTE)(bits&mask);  bits >>= 3;
	gBits[0][1] = (BYTE)(bits&mask);  bits >>= 3;
	gBits[0][2] = (BYTE)(bits&mask);  bits >>= 3;
	gBits[0][3] = (BYTE)(bits&mask);  bits >>= 3;
	gBits[1][0] = (BYTE)(bits&mask);  bits >>= 3;
	gBits[1][1] = (BYTE)(bits&mask);  bits >>= 3;
	gBits[1][2] = (BYTE)(bits&mask);  bits >>= 3;
	gBits[1][3] = (BYTE)(bits&mask);

	// now for last two rows:
	bits = *((DWORD*)&(pAlphaBlock->stuff[3]));   // last 3 bytes
	gBits[2][0] = (BYTE)(bits&mask);  bits >>= 3;
	gBits[2][1] = (BYTE)(bits&mask);  bits >>= 3;
	gBits[2][2] = (BYTE)(bits&mask);  bits >>= 3;
	gBits[2][3] = (BYTE)(bits&mask);  bits >>= 3;
	gBits[3][0] = (BYTE)(bits&mask);  bits >>= 3;
	gBits[3][1] = (BYTE)(bits&mask);  bits >>= 3;
	gBits[3][2] = (BYTE)(bits&mask);  bits >>= 3;
	gBits[3][3] = (BYTE)(bits&mask);

	// decode the codes into alpha values
	int row, pixel;
	for (row = 0; row < 4; row++) {
		for (pixel = 0; pixel < 4; pixel++) {
			gACol[row][pixel].a = (BYTE)gAlphas[gBits[row][pixel]];
			//ASSERT(gACol[row][pixel].r==0);
			//ASSERT(gACol[row][pixel].g==0);
			//ASSERT(gACol[row][pixel].b==0);
		}
	}

	// write out alpha values to the image bits
	for (row = 0; row < 4; row++) {
		// pImPow += pImPos += width-4 moves to next row down
		for (pixel = 0; pixel < 4; pixel++) {
			// zero the alpha bits of image pixel
			*pImPos &= 0x00FFFFFF;
			*pImPos |= *((DWORD*)&(gACol[row][pixel])); // or the bits into the prev. nulled alpha
			pImPos++;
		}
		// next row
		pImPos += width-4;
	}
}

void DecompressDXT1(BYTE* pubCompressed, ULONG* pulExpanded, LONG pixWidth, LONG pixHeight)
{
	// decompresses to 32 bit format 0xARGB
	//ASSERT((pixWidth%4)==0 && (pixHeight%4)==0);
	int xblocks = pixWidth / 4;
	int yblocks = pixHeight / 4;
	for (int j = 0; j < yblocks; j++) {
		// 8 bytes per block
		DXTColBlock* pBlock = (DXTColBlock*)((DWORD)pubCompressed + j * xblocks * 8);
		for (int i = 0; i < xblocks; i++) {
			Color8888 col_0, col_1, col_2, col_3;
			GetColorBlockColors(pBlock, &col_0, &col_1, &col_2, &col_3);
			// now decode the color block into the bitmap bits
			DWORD* pImPos = (DWORD*)((DWORD)pulExpanded + i * 16 + (j * 4) * pixWidth * 4);
			DecodeColorBlock(pImPos, pBlock, pixWidth, (DWORD*)&col_0, (DWORD*)&col_1, (DWORD*)&col_2, (DWORD*)&col_3);
			// next block
			pBlock++;
		}
	}
}

void DecompressDXT3(BYTE *pubCompressed, ULONG *pulExpanded, LONG pixWidth, LONG pixHeight)
{
	//ASSERT( (pixWidth%4)==0 && (pixHeight%4)==0);
	int xblocks = pixWidth / 4;
	int yblocks = pixHeight / 4;
	for (int j = 0; j < yblocks; j++) {
		// 8 bytes per block
		// 1 block for alpha, 1 block for color
		DXTColBlock* pBlock = (DXTColBlock*)((DWORD)pubCompressed + j * xblocks * 16);
		for (int i = 0; i < xblocks; i++) {
			// Get alpha block
			DXTAlphaBlockExplicit* pAlphaBlock = (DXTAlphaBlockExplicit*)pBlock;
			((DXTAlphaBlockExplicit*&)pBlock)++;
			// Get color block & colors
			Color8888 col_0, col_1, col_2, col_3;
			GetColorBlockColors(pBlock, &col_0, &col_1, &col_2, &col_3);
			// Decode the color block into the bitmap bits
			DWORD* pImPos = (DWORD*)((DWORD)pulExpanded + i * 16 + (j * 4) * pixWidth * 4);
			DecodeColorBlock(pImPos, pBlock, pixWidth, (DWORD*)&col_0, (DWORD*)&col_1, (DWORD*)&col_2, (DWORD*)&col_3);
			// Overwrite the previous alpha bits with the alpha block
			DecodeAlphaExplicit(pImPos, pAlphaBlock, pixWidth);
			// next block
			pBlock++;
		}
	}
}

void DecompressDXT5(BYTE* pubCompressed, ULONG* pulExpanded, LONG pixWidth, LONG pixHeight)
{
	//ASSERT((pixWidth%4)==0 && (pixHeight%4)==0);
	int xblocks = pixWidth / 4;
	int yblocks = pixHeight / 4;
	for (int j = 0; j < yblocks; j++) {
		// 8 bytes per block
		// 1 block for alpha, 1 block for color
		DXTColBlock* pBlock = (DXTColBlock*)((DWORD)pubCompressed + j * xblocks * 16);
		for (int i = 0; i < xblocks; i++) {
			// Get alpha block
			DXTAlphaBlock3BitLinear* pAlphaBlock = (DXTAlphaBlock3BitLinear*)pBlock;
			((DXTAlphaBlock3BitLinear*&)pBlock)++;
			// Get color block & colors
			Color8888 col_0, col_1, col_2, col_3;
			GetColorBlockColors(pBlock, &col_0, &col_1, &col_2, &col_3);
			// Decode the color block into the bitmap bits
			DWORD* pImPos = (DWORD*)((DWORD)pulExpanded + i * 16 + (j * 4) * pixWidth * 4);
			DecodeColorBlock(pImPos, pBlock, pixWidth, (DWORD*)&col_0, (DWORD*)&col_1, (DWORD*)&col_2, (DWORD*)&col_3);
			// Overwrite the previous alpha bits with the alpha block
			DecodeAlpha3BitLinear(pImPos, pAlphaBlock, pixWidth);
			// next block
			pBlock++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////

#define BYTES_PER_TEXEL 4   // all textures in engine are 4 bytes per pixel

// td_ulFlags bits
// (on disk and in memory)
#define TEX_ALPHACHANNEL    (1UL<<0)   // texture has alpha channel (for old version support)
#define TEX_32BIT           (1UL<<1)   // texture needs to be in 32-bit quality uploaded if can
#define TEX_COMPRESSED      (1UL<<2)   // texture is stored on disk in compressed format
#define TEX_TRANSPARENT     (1UL<<3)   // only one bit of alpha channel is enough (internal format GL_RGB5_A1)
#define TEX_EQUALIZED       (1UL<<4)   // texture has 128-gray last mipmap (i.e. can be discarded in shade mode)
#define TEX_COMPRESSEDALPHA (1UL<<5)   // compressed texture is using interpoladed alpha (DXT5) rather than fixed one (DXT3)
// (only in memory)
#define TEX_STATIC          (1UL<<8)   // remain loaded after being bound (i.e. uploaded - for base textures)
#define TEX_CONSTANT        (1UL<<9)   // cannot be changed (no mip-map disposing, no LOD biasing, no colorizing, nothing!)
#define TEX_GRAY            (1UL<<10)  // grayscale texture
#define TEX_COMPRESS        (1UL<<16)  // this texture will need to be compressed when saved
#define TEX_COMPRESSALPHA   (1UL<<17)  // this texture will have alpha interpolated when compressed
#define TEX_SINGLEMIPMAP    (1UL<<18)  // set if last uploading was in single-mipmap
#define TEX_PROBED          (1UL<<19)  // set if last binding was as probe-texture
// (flags that shows if texture mipmaps has been changed)
#define TEX_DISPOSED        (1UL<<20)  // largest mip-map(s) has been left-out
#define TEX_DITHERED        (1UL<<21)  // dithering has been applied on this texture
#define TEX_FILTERED        (1UL<<22)  // flitering has been applied on this texture
#define TEX_COLORIZED       (1UL<<24)  // mipmaps has been colorized on this texture
#define TEX_WASOLD          (1UL<<30)  // loaded from old format (version 3)

#define TEX_DATA_VER 1
#define CODE_TEX_1 17
#define CODE_TEX_2 02
#define CODE_TEX_3 41
#define CODE_TEX_4 01
#define CODE_TEX_5 6

template<class Type>
inline Type DecodeSimpleTex(Type val, BYTE& ubChecker)
{
	if (sizeof(Type) != 4)
		return val;
	ULONG ulChecker = 0;
	ubChecker += (BYTE)CODE_TEX_1;
	ulChecker |= (ULONG)ubChecker << 24;
	ubChecker += (BYTE)CODE_TEX_2;
	ulChecker |= (ULONG)ubChecker << 16;
	ubChecker += (BYTE)CODE_TEX_3;
	ulChecker |= (ULONG)ubChecker << 8;
	ubChecker += (BYTE)CODE_TEX_4;
	ulChecker |= (ULONG)ubChecker << 0;
	ubChecker += CODE_TEX_5;
	return Type(val ^ ulChecker);
}

// retrives memory offset of a specified mip-map or a size of all mip-maps (IN PIXELS!)
// (zero offset means first, i.e. largest mip-map)
LONG GetMipmapOffset(INT iMipLevel, LONG pixWidth, LONG pixHeight)
{
	LONG pixTexSize = 0;
	LONG pixMipSize = pixWidth * pixHeight;
	INT iMips = GetNoOfMipmaps(pixWidth, pixHeight);
	iMips = Min(iMips, iMipLevel);
	while (iMips > 0)
	{
		pixTexSize += pixMipSize;
		pixMipSize >>= 2;
		iMips--;
	}
	return pixTexSize;
}

// adds 8-bit opaque alpha channel to 24-bit bitmap (in place supported)
void AddAlphaChannel(BYTE *pubSrcBitmap, ULONG *pulDstBitmap, LONG pixSize, BYTE *pubAlphaBitmap = NULL)
{
	BYTE ubR, ubG, ubB, ubA = 255;
	// loop backwards thru all bitmap pixels
	for (INT iPix = (pixSize-1); iPix >= 0; iPix--)
	{
		ubR = pubSrcBitmap[iPix * 3 + 0];
		ubG = pubSrcBitmap[iPix * 3 + 1];
		ubB = pubSrcBitmap[iPix * 3 + 2];
		if (pubAlphaBitmap != NULL) ubA = pubAlphaBitmap[iPix];
		else ubA = 255; // for the sake of forced RGBA internal formats!
		pulDstBitmap[iPix] = ByteSwap(RGBAToColor(ubR,ubG,ubB,ubA));
	}
}

// routine that performs texture conversion to current texture format (version 4)
void Convert(ULONG*& td_pulFrames, LONG& td_slFrameSize, LONG pixWidth, LONG pixHeight, ULONG td_ulFlags, INT td_ctFrames)
{
	// determine dimensions 
	LONG pixMipSize   = pixWidth * pixHeight;
	LONG pixFrameSize = GetMipmapOffset(15, pixWidth, pixHeight);
	// allocate memory for new texture
	ULONG *pulFramesNew = (ULONG*)malloc(pixFrameSize * td_ctFrames * BYTES_PER_TEXEL);
	UWORD *puwFramesOld = (UWORD*)td_pulFrames;
	
	// determine alpha channel presence
	BOOL bHasAlphaChannel = td_ulFlags & TEX_ALPHACHANNEL;
	
	// unpack texture from 16-bit RGBA4444 or RGBA5551 format to RGBA8888 32-bit format
	BYTE r,g,b,a;
	// for each frame
	for (INT iFr = 0; iFr < td_ctFrames; iFr++)
	{ // get addresses of current frames (new and old)
		LONG pixFrameOffset = iFr * pixFrameSize;
		// for each pixel
		for (INT iPix = 0; iPix < pixMipSize; iPix++)
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
	free(td_pulFrames);
	// remember new texture parameters
	td_pulFrames   = pulFramesNew;
	td_slFrameSize = pixFrameSize * BYTES_PER_TEXEL;
}

inline CxFile& operator>>(CxFile& file, BYTE&  v) { file.Read(&v, sizeof(v), 1); return file; }
inline CxFile& operator>>(CxFile& file, int&   v) { file.Read(&v, sizeof(v), 1); return file; }
inline CxFile& operator>>(CxFile& file, ULONG& v) { file.Read(&v, sizeof(v), 1); return file; }
inline CxFile& operator>>(CxFile& file, LONG&  v) { file.Read(&v, sizeof(v), 1); return file; }
inline CxFile& operator>>(CxFile& file, FLOAT& v) { file.Read(&v, sizeof(v), 1); return file; }

inline CxFile& operator<<(CxFile& file, const BYTE&  v) { file.Write(&v, sizeof(v), 1); return file; }
inline CxFile& operator<<(CxFile& file, const int&   v) { file.Write(&v, sizeof(v), 1); return file; }
inline CxFile& operator<<(CxFile& file, const ULONG& v) { file.Write(&v, sizeof(v), 1); return file; }
inline CxFile& operator<<(CxFile& file, const LONG&  v) { file.Write(&v, sizeof(v), 1); return file; }
inline CxFile& operator<<(CxFile& file, const FLOAT& v) { file.Write(&v, sizeof(v), 1); return file; }

bool ReadID(CxFile* hFile, string& id)
{
	char buf[8];
	memset(buf, 0, 8);
	if (hFile->Read(buf, 1, 4) == 0)
		return false;
	id = buf;
	return true;
}

bool ExpectID(CxFile* hFile, const char* szID)
{
	string id;
	if (!ReadID(hFile, id))
		return false;
	if (id != szID)
		return false;
	return true;
}

bool ReadFilename(CxFile* hFile, string& filename)
{
	if (!ExpectID(hFile, "DFNM"))
		return false;
	ULONG len;
	*hFile >> len;
	if (len)
	{
		char* buf = (char*)malloc(len+1);
		if (buf == NULL)
			return false;
		if (hFile->Read(buf, 1, len) == 0)
		{
			free(buf);
			return false;
		}
		buf[len] = 0;
		filename = buf;
		free(buf);
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////
#if CXIMAGE_SUPPORT_DECODE
////////////////////////////////////////////////////////////////////////////////
bool CxImageLCTEX::Decode(CxFile* hFile)
{
	if (hFile == NULL)
		return false;
	
	int x, y, y2, Height, Width;
	BYTE *pixels;
	
	string id;
	int iVersion, iNewVer;

	ULONG td_ulFlags = 0;			// see defines
	LONG  td_mexWidth = 0;			// texture dimensions
	LONG  td_mexHeight = 0;			//
	INT   td_iFirstMipLevel = 0;	// the highest quality mip level
	LONG  td_ctFineMipLevels = 0;	// number of bilineary created mip levels
	LONG  td_slFrameSize = 0;		// sum of sizes of all mip-maps for one frame
	INT   td_ctFrames = 0;			// number of different frames

	ULONG* td_pulFrames = NULL;		// all frames with their mip-maps and private palettes
	//BYTE*  td_pubBuffer1 = NULL;	// buffers for effect textures
	//BYTE*  td_pubBuffer2 = NULL;	//
	//LONG   td_pixBufferWidth = 0;	// effect buffer dimensions
	//LONG   td_pixBufferHeight = 0;	//

	cx_try
	{
		// TVER
		id = "TVER";
		if (!ExpectID(hFile, id.c_str()))
			cx_throw("Error: Not a PCX file");
		LOG("%s\n", id.c_str());
		*hFile >> iVersion;
		LOG("  Version:0x%08x\n", iVersion);

		iNewVer = (iVersion & 0xFFFF0000) >> 16;
		iVersion &= 0x0000FFFF;

		if (iVersion == 5)
		{
			iVersion = 4;
			iNewVer = 5;
		}
		LOG("  Version:%d, NewVer:%d\n", iVersion, iNewVer);

		if (iVersion != 4 && iVersion != 3)
			cx_throw("Error: Invalid texture format version.");

		// mark if this texture was loaded form the old format
		if (iVersion == 3) td_ulFlags |= TEX_WASOLD;
		//BOOL bResetEffectBuffers = FALSE;
		//BOOL bFramesLoaded = FALSE;
		BOOL bAlphaChannel = FALSE;
		BOOL bCompressed   = FALSE;

		while (!hFile->Eof())
		{
			if (!ReadID(hFile, id))
				break;
			LOG("%s\n", id.c_str());

			if (id == "TDAT")
			{
				if (iNewVer == TEX_DATA_VER || iNewVer == 5)
				{
					UCHAR checker = iNewVer;
					// read data describing texture
					int ctMipLevels;
					ULONG ulFlags = 0;
					*hFile >> td_mexWidth;			td_mexWidth = DecodeSimpleTex(td_mexWidth, checker);
					*hFile >> td_iFirstMipLevel;	td_iFirstMipLevel = DecodeSimpleTex(td_iFirstMipLevel, checker);
					*hFile >> td_mexHeight;			td_mexHeight = DecodeSimpleTex(td_mexHeight, checker);
					*hFile >> td_ctFineMipLevels;	td_ctFineMipLevels = DecodeSimpleTex(td_ctFineMipLevels, checker);
					*hFile >> ulFlags;				ulFlags = DecodeSimpleTex(ulFlags, checker);
					*hFile >> td_ctFrames;			td_ctFrames = DecodeSimpleTex(td_ctFrames, checker);
					if (iVersion != 4) *hFile >> ctMipLevels;
					if (iVersion != 4) *hFile >> td_slFrameSize;
					// combine flags
					td_ulFlags |= ulFlags;
					bAlphaChannel = td_ulFlags & TEX_ALPHACHANNEL;
					bCompressed   = td_ulFlags & TEX_COMPRESSED;
					// set number of mip levels and frame size
					td_ctFineMipLevels = ClampDn(td_ctFineMipLevels, 1L);
					Width  = td_mexWidth  >> td_iFirstMipLevel;
					Height = td_mexHeight >> td_iFirstMipLevel;
					td_slFrameSize = GetMipmapOffset(15, Width, Height) * BYTES_PER_TEXEL;
				}
				else	//old
				{
					// read data describing texture
					ULONG ulFlags = 0;
					int ctMipLevels;
					*hFile >> ulFlags;
					*hFile >> td_mexWidth;
					*hFile >> td_mexHeight;
					*hFile >> td_ctFineMipLevels;
					if (iVersion != 4) *hFile >> ctMipLevels;
					*hFile >> td_iFirstMipLevel;
					if (iVersion != 4) *hFile >> td_slFrameSize;
					*hFile >> td_ctFrames;
					// combine flags
					td_ulFlags |= ulFlags;
					bAlphaChannel = td_ulFlags & TEX_ALPHACHANNEL;
					bCompressed   = td_ulFlags & TEX_COMPRESSED;
					// set number of mip levels and frame size
					td_ctFineMipLevels = ClampDn(td_ctFineMipLevels, 1L);
					Width  = td_mexWidth  >> td_iFirstMipLevel;
					Height = td_mexHeight >> td_iFirstMipLevel;
					if (iVersion == 4) td_slFrameSize = GetMipmapOffset(15, Width, Height) * BYTES_PER_TEXEL;
				}
				info.nNumFrames = td_ctFrames;
				if (info.bGetAllFrames) {
					DestroyFrames();
					ppFrames = new CxImage*[info.nNumFrames];
					for(int frameIdx = 0; frameIdx < info.nNumFrames; frameIdx++){
						ppFrames[frameIdx] = NULL;
					}
				}
				LOG("  Flags:0x%08x\n", td_ulFlags);
				LOG("  Width:%d, Height:%d\n", td_mexWidth, td_mexHeight);
				LOG("  FirstMipLevel:%d, FineMipLevels:%d\n", td_iFirstMipLevel, td_ctFineMipLevels);
				LOG("  Frames:%d, FrameSize:%d\n", td_ctFrames, td_slFrameSize);
				LOG("  PixWidth:%d, PixHeight:%d\n", Width, Height);
			}
			else if (id == "FRMC")
			{
				if (!bCompressed)
					cx_throw("Error: bCompressed == FALSE, but FRMC.");

				LONG slCompressedFrameSize;
				*hFile >> slCompressedFrameSize;
				const LONG slCompressedTexSize = td_ctFrames * slCompressedFrameSize;
				// load frames!
				td_pulFrames = (ULONG*)malloc(slCompressedTexSize);
				hFile->Read(td_pulFrames, slCompressedTexSize, 1);
				//bFramesLoaded = TRUE;  // mark that frames have been loaded
				// update frame size
				td_slFrameSize = slCompressedFrameSize;
			}
			else if (id == "FRMS")
			{
				if (bCompressed)
					cx_throw("Error: bCompressed == TRUE, but FRMS.");

				// calculate texture size for corresponding texture format and allocate memory
				LONG slTexSize = td_slFrameSize * td_ctFrames;
				LOG("  TexSize:%d\n", slTexSize);
				td_pulFrames = (ULONG*)malloc(slTexSize);
				// if older version
				if (iVersion == 3)
				{
					// alloc memory block and read mip-maps
					hFile->Read(td_pulFrames, slTexSize, 1);
					Convert(td_pulFrames, td_slFrameSize, Width, Height, td_ulFlags, td_ctFrames);
				}
				// if current version
				else 
				{
					LONG pixFrameSizeOnDisk = Width * Height;
					for (INT iFr = 0; iFr < td_ctFrames; iFr++)
					{ // loop thru frames
						ULONG* pulCurrentFrame = td_pulFrames + (iFr * td_slFrameSize / BYTES_PER_TEXEL);
						if (bAlphaChannel)
						{
							LOG("  [%d] PixFrameSizeOnDisk*4:%d\n", iFr, pixFrameSizeOnDisk * 4);
							// read texture with alpha channel from file
							hFile->Read(pulCurrentFrame, pixFrameSizeOnDisk * 4, 1);
						}
						else 
						{
							LOG("  [%d] PixFrameSizeOnDisk*3:%d\n", iFr, pixFrameSizeOnDisk * 3);
							// read texture without alpha channel from file
							hFile->Read(pulCurrentFrame, pixFrameSizeOnDisk * 3, 1);
							// add opaque alpha channel
							AddAlphaChannel((BYTE*)pulCurrentFrame, pulCurrentFrame, pixFrameSizeOnDisk);
						}
					}
				} // mark that frames have been loaded
				//bFramesLoaded = TRUE;
			}
			else if (id == "ANIM")
			{
				if (!ExpectID(hFile, "ADAT"))
					break;
				LOG("  ADAT\n");

				int iNumberOfAnims;
				*hFile >> iNumberOfAnims;
				LOG("  NumberOfAnims:%d\n", iNumberOfAnims);

				char szName[32];
				float fSecsPerFrame;
				int iNumberOfFrames = 0;
				for (int i = 0; i < iNumberOfAnims; i++)
				{
					hFile->Read(szName, 32, 1);
					*hFile >> fSecsPerFrame;
					*hFile >> iNumberOfFrames;
					if (iNumberOfFrames > 0)
						hFile->Seek(iNumberOfFrames * 4, SEEK_CUR);
					if (info.bGetAllFrames && i == 0)
					{
						info.dwFrameDelay = (DWORD)(fSecsPerFrame * 100);
					}
					LOG("  [%d] Name:%s, SecsPerFrame:%.3f, NumberOfFrames:%d\n", i, szName, fSecsPerFrame, iNumberOfFrames);
				}
			}
			else if (id == "BAST")
			{
				cx_throw("Not implemented");
			}
			else if (id == "FXBF")
			{
				cx_throw("Not implemented");
			}
			else if (id == "FXB2")
			{
				cx_throw("Not implemented");
			}
			else if (id == "FXDT")
			{
				cx_throw("Not implemented");
			}
			else if (id == "DTLT")
			{
				cx_throw("Not implemented");
			}
			else
			{
				cx_throw("Error: Unknown ID.");
			}
		}

		if (bCompressed)
		{
			const LONG pixWidth  = Width;
			const LONG pixHeight = Height;
			// static texture needs to be decompressed
			ULONG *pulExpanded = NULL;
			LONG slExpandSize = 0;

			const BOOL bAlphaCh = (td_ulFlags & TEX_ALPHACHANNEL);
			const BOOL bTranspa = (td_ulFlags & TEX_TRANSPARENT);
			const BOOL bComprAl = (td_ulFlags & TEX_COMPRESSEDALPHA) && bAlphaCh;
			const LONG pixExpandSize = GetMipmapOffset(15, pixWidth, pixHeight);
			slExpandSize = pixExpandSize * BYTES_PER_TEXEL;
			pulExpanded  = (ULONG*)malloc(slExpandSize * td_ctFrames);
			// loop thru frames
			for (INT iFr = 0; iFr < td_ctFrames; iFr++)
			{ // determine frame offset and upload texture frame
				BYTE*  pubFrameCo = (BYTE*)(td_pulFrames+1) + (iFr*td_slFrameSize); // +1 to skip length of mipmap!
				ULONG* pulFrameEx = pulExpanded + (iFr*pixExpandSize);
				if (bComprAl)
					DecompressDXT5(pubFrameCo, pulFrameEx, pixWidth, pixHeight);
				else if (bAlphaCh && !bTranspa)
					DecompressDXT3(pubFrameCo, pulFrameEx, pixWidth, pixHeight);
				else
					DecompressDXT1(pubFrameCo, pulFrameEx, pixWidth, pixHeight);
			}

			// upload
			//const BOOL bUploaded = UploadCompressed();
			//if( !bUploaded) throw( TRANS("Cannot upload compressed texture."));
			free(td_pulFrames);

			// assign uncompressed frames if texture is static
			//ASSERT( slExpandSize>0 && pulExpanded!=NULL);
			td_slFrameSize = slExpandSize;
			td_pulFrames   = pulExpanded;
		}

#if CXIMAGE_SUPPORT_ALPHA
		if (bAlphaChannel)
		{
			Create(Width, Height, 24, CXIMAGE_FORMAT_LC_TEX);
			AlphaCreate();
		}
		else
#endif //CXIMAGE_SUPPORT_ALPHA
		{
			Create(Width, Height, 24, CXIMAGE_FORMAT_LC_TEX);
		}

		int frameIdx = info.nFrame;
		int frameEnd = info.nFrame+1;
		if (info.bGetAllFrames)
		{
			frameIdx = 0;
			frameEnd = info.nNumFrames;
		}
		for (; frameIdx < frameEnd; frameIdx++)
		{
			// Convert the image
			ULONG* pulCurrentFrame = td_pulFrames + (frameIdx * td_slFrameSize / BYTES_PER_TEXEL);
			for (y = 0; y < Height; y++)
			{
				if (info.nEscape)
					cx_throw("Cancelled"); // <vho> - cancel decoding

				y2 = Height - 1 - y;
				pixels = (BYTE*)(pulCurrentFrame + y * Width);

#if CXIMAGE_SUPPORT_ALPHA
				if (bAlphaChannel)
				{
					for (x = 0; x < Width; x++)
					{
						SetPixelColor(x, y2, RGB(pixels[4*x], pixels[4*x+1], pixels[4*x+2]));
						AlphaSet(x, y2, pixels[4*x+3]);
					}
					continue;
				}
				else
#endif //CXIMAGE_SUPPORT_ALPHA
				{
					for (x = 0; x < Width; x++)
					{
						SetPixelColor(x, y2, RGB(pixels[4*x], pixels[4*x+1], pixels[4*x+2]));
					}
					continue;
				}
			}

			if (info.bGetAllFrames)
			{
				ppFrames[frameIdx] = new CxImage(*this);
				ppFrames[frameIdx]->SetRetreiveAllFrames(false);
			}
		}
	}
	cx_catch
	{
		if (strcmp(message, ""))
			strncpy(info.szLastError, message, 255);
		SAFE_FREE(td_pulFrames);
		return false;
	}
	SAFE_FREE(td_pulFrames);
	return true;
}
////////////////////////////////////////////////////////////////////////////////
#endif //CXIMAGE_SUPPORT_DECODE
////////////////////////////////////////////////////////////////////////////////
#if CXIMAGE_SUPPORT_ENCODE
////////////////////////////////////////////////////////////////////////////////
bool CxImageLCTEX::Encode(CxFile* hFile)
{
	return false;	// not implement
}
////////////////////////////////////////////////////////////////////////////////
#endif // CXIMAGE_SUPPORT_ENCODE

#endif // CXIMAGE_SUPPORT_LC_TEX
