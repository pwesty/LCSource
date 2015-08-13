/*********************************************************************NVMH2****
Path:  C:\Dev\devrel\Nv_sdk_4\Direct3D\Decompress_DXTC
File:  Image_DXTC.cpp

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/

#include "stdh.h"

#include <Engine/Graphics/GfxLibrary.h>


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





inline void GetColorBlockColors( DXTColBlock *pBlock, Color8888 *col_0, Color8888 *col_1, 
                                 Color8888 *col_2, Color8888 *col_3)
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

  Color565 *pCol;

  pCol = (Color565*)&(pBlock->col0);
  col_0->a = 0xFF;
  col_0->r = pCol->nRed;    col_0->r = (col_0->r<<3) | (col_0->r>>2);   // shift to full precision
  col_0->g = pCol->nGreen;  col_0->g = (col_0->g<<2) | (col_0->g>>4);
  col_0->b = pCol->nBlue;   col_0->b = (col_0->b<<3) | (col_0->b>>2);

  pCol = (Color565*)&(pBlock->col1);
  col_1->a = 0xFF;
  col_1->r = pCol->nRed;    col_1->r = (col_1->r<<3) | (col_1->r>>2);
  col_1->g = pCol->nGreen;  col_1->g = (col_1->g<<2) | (col_1->g>>4);
  col_1->b = pCol->nBlue;   col_1->b = (col_1->b<<3) | (col_1->b>>2);

  if( pBlock->col0 > pBlock->col1)
  {
    // Four-color block: derive the other two colors.    
    // 00 = color_0, 01 = color_1, 10 = color_2, 11 = color_3
    // These two bit codes correspond to the 2-bit fields 
    // stored in the 64-bit block.
    col_2->r = ((WORD)col_0->r *2 + (WORD)col_1->r) /3;  // no +1 for rounding as bits have been shifted to 888
    col_2->g = ((WORD)col_0->g *2 + (WORD)col_1->g) /3;
    col_2->b = ((WORD)col_0->b *2 + (WORD)col_1->b) /3;
    col_2->a = 0xFF;
    col_3->r = ((WORD)col_0->r + (WORD)col_1->r *2) /3;
    col_3->g = ((WORD)col_0->g + (WORD)col_1->g *2) /3;
    col_3->b = ((WORD)col_0->b + (WORD)col_1->b *2) /3;
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

    col_2->r = ((WORD)col_0->r + (WORD)col_1->r) /2;
    col_2->g = ((WORD)col_0->g + (WORD)col_1->g) /2;
    col_2->b = ((WORD)col_0->b + (WORD)col_1->b) /2;
    col_2->a = 0xFF;

    col_3->r = 0x00;    // random color to indicate alpha
    col_3->g = 0xFF;
    col_3->b = 0xFF;
    col_3->a = 0x00;
  }
}



inline void DecodeColorBlock( DWORD *pImPos, DXTColBlock *pColorBlock, int width,  // width is width of image in pixels
                              DWORD *col_0, DWORD *col_1, DWORD *col_2, DWORD *col_3)
{
  // bit masks = 00000011, 00001100, 00110000, 11000000
  const DWORD masks[] = { 3<<0, 3<<2, 3<<4, 3<<6 };
  const int   shift[] = { 0, 2, 4, 6 };

  // r steps through lines in y
  for( int r=0; r<4; r++) { // no width*4 as DWORD ptr inc will *4
    // width * 4 bytes per pixel per line
    // each j dxtc row is 4 lines of pixels
    // pImPos = (DWORD*)((DWORD)pBase + i*16 + (r+j*4) * m_nWidth * 4 );
    // n steps through pixels
    for( int n=0; n<4; n++) {
      const DWORD bits = (pColorBlock->row[r] & masks[n]) >> shift[n];
      switch(bits) {
      case 0:  *pImPos = *col_0;  break;
      case 1:  *pImPos = *col_1;  break;
      case 2:  *pImPos = *col_2;  break;
      case 3:  *pImPos = *col_3;  break;
      default:  ASSERT(FALSE);    break;
      }
      // next texel
      pImPos++;
    }
    // next row
    pImPos += width-4;
  }
}



inline void DecodeAlphaExplicit( DWORD *pImPos, DXTAlphaBlockExplicit *pAlphaBlock, int width)
{
  // decodes to 32 bit format only
  WORD wrd;
  Color8888 col;
  col.r = col.g = col.b = 0;

  for( int row=0; row<4; row++) {
    // pImPow += pImPos += width-4 moves to next row down
    wrd = pAlphaBlock->row[row];
    for( int pixel=0; pixel<4; pixel++) {
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

inline void DecodeAlpha3BitLinear( DWORD *pImPos, DXTAlphaBlock3BitLinear *pAlphaBlock, int width)
{
  gAlphas[0] = pAlphaBlock->alpha0;
  gAlphas[1] = pAlphaBlock->alpha1;
  
  // 8-alpha or 6-alpha block?    
  if( gAlphas[0] > gAlphas[1]) {
    // 8-alpha block:  derive the other 6 alphas.    
    // 000 = alpha_0, 001 = alpha_1, others are interpolated
    gAlphas[2] = (6 * gAlphas[0] + 1 * gAlphas[1] +3) / 7;    // bit code 010
    gAlphas[3] = (5 * gAlphas[0] + 2 * gAlphas[1] +3) / 7;    // Bit code 011    
    gAlphas[4] = (4 * gAlphas[0] + 3 * gAlphas[1] +3) / 7;    // Bit code 100    
    gAlphas[5] = (3 * gAlphas[0] + 4 * gAlphas[1] +3) / 7;    // Bit code 101
    gAlphas[6] = (2 * gAlphas[0] + 5 * gAlphas[1] +3) / 7;    // Bit code 110    
    gAlphas[7] = (1 * gAlphas[0] + 6 * gAlphas[1] +3) / 7;    // Bit code 111
  }    
  else {
    // 6-alpha block:  derive the other alphas.    
    // 000 = alpha_0, 001 = alpha_1, others are interpolated
    gAlphas[2] = (4 * gAlphas[0] + 1 * gAlphas[1] +2) / 5;    // Bit code 010
    gAlphas[3] = (3 * gAlphas[0] + 2 * gAlphas[1] +2) / 5;    // Bit code 011    
    gAlphas[4] = (2 * gAlphas[0] + 3 * gAlphas[1] +2) / 5;    // Bit code 100    
    gAlphas[5] = (1 * gAlphas[0] + 4 * gAlphas[1] +2) / 5;    // Bit code 101
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
  for( row=0; row<4; row++) {
    for( pixel=0; pixel<4; pixel++) {
      gACol[row][pixel].a = (BYTE)gAlphas[gBits[row][pixel]];
      ASSERT( gACol[row][pixel].r==0);
      ASSERT( gACol[row][pixel].g==0);
      ASSERT( gACol[row][pixel].b==0);
    }
  }

  // write out alpha values to the image bits
  for( row=0; row<4; row++) {
    // pImPow += pImPos += width-4 moves to next row down
    for( pixel=0; pixel<4; pixel++) {
      // zero the alpha bits of image pixel
      *pImPos &= 0x00FFFFFF;
      *pImPos |= *((DWORD*)&(gACol[row][pixel])); // or the bits into the prev. nulled alpha
      pImPos++;
    }
    // next row
    pImPos += width-4;
  }
}



extern void DecompressDXT1( UBYTE *pubCompressed, ULONG *pulExpanded, PIX pixWidth, PIX pixHeight)
{
  // decompresses to 32 bit format 0xARGB
  ASSERT( (pixWidth%4)==0 && (pixHeight%4)==0);
  int xblocks = pixWidth  /4;
  int yblocks = pixHeight /4;
  for( int j=0; j<yblocks; j++) {
    // 8 bytes per block
    DXTColBlock *pBlock = (DXTColBlock*)((DWORD)pubCompressed + j*xblocks *8);
    for( int i=0; i<xblocks; i++) {
      Color8888 col_0, col_1, col_2, col_3;
      GetColorBlockColors( pBlock, &col_0, &col_1, &col_2, &col_3);
      // now decode the color block into the bitmap bits
      DWORD *pImPos = (DWORD*)((DWORD)pulExpanded + i*16 + (j*4) * pixWidth*4);
      DecodeColorBlock( pImPos, pBlock, pixWidth,
                        (DWORD*)&col_0, (DWORD*)&col_1, (DWORD*)&col_2, (DWORD*)&col_3);
      // next block
      pBlock++;
    }
  }
}



extern void DecompressDXT3( UBYTE *pubCompressed, ULONG *pulExpanded, PIX pixWidth, PIX pixHeight)
{
  ASSERT( (pixWidth%4)==0 && (pixHeight%4)==0);
  int xblocks = pixWidth  /4;
  int yblocks = pixHeight /4;
  for( int j=0; j<yblocks; j++) {
    // 8 bytes per block
    // 1 block for alpha, 1 block for color
    DXTColBlock *pBlock = (DXTColBlock*)((DWORD)pubCompressed + j*xblocks *16);
    for( int i=0; i<xblocks; i++) {
      // Get alpha block
      DXTAlphaBlockExplicit *pAlphaBlock = (DXTAlphaBlockExplicit*)pBlock;
      ((DXTAlphaBlockExplicit*&)pBlock)++;
      // Get color block & colors
      Color8888 col_0, col_1, col_2, col_3;
      GetColorBlockColors( pBlock, &col_0, &col_1, &col_2, &col_3);
      // Decode the color block into the bitmap bits
      DWORD *pImPos = (DWORD*)((DWORD)pulExpanded + i*16 + (j*4) * pixWidth*4);
      DecodeColorBlock( pImPos, pBlock, pixWidth,
                        (DWORD*)&col_0, (DWORD*)&col_1, (DWORD*)&col_2, (DWORD*)&col_3);
      // Overwrite the previous alpha bits with the alpha block
      DecodeAlphaExplicit( pImPos, pAlphaBlock, pixWidth);
      // next block
      pBlock++;
    }
  }
}



extern void DecompressDXT5( UBYTE *pubCompressed, ULONG *pulExpanded, PIX pixWidth, PIX pixHeight)
{
  ASSERT( (pixWidth%4)==0 && (pixHeight%4)==0);
  int xblocks = pixWidth  /4;
  int yblocks = pixHeight /4;
  for( int j=0; j<yblocks; j++) {
    // 8 bytes per block
    // 1 block for alpha, 1 block for color
    DXTColBlock *pBlock = (DXTColBlock*)((DWORD)pubCompressed + j*xblocks *16);
    for( int i=0; i<xblocks; i++) {
      // Get alpha block
      DXTAlphaBlock3BitLinear *pAlphaBlock = (DXTAlphaBlock3BitLinear*)pBlock;
      ((DXTAlphaBlock3BitLinear*&)pBlock)++;
      // Get color block & colors
      Color8888 col_0, col_1, col_2, col_3;
      GetColorBlockColors( pBlock, &col_0, &col_1, &col_2, &col_3);
      // Decode the color block into the bitmap bits
      DWORD *pImPos = (DWORD*)((DWORD)pulExpanded + i*16 + (j*4) * pixWidth*4);
      DecodeColorBlock( pImPos, pBlock, pixWidth,
                        (DWORD*)&col_0, (DWORD*)&col_1, (DWORD*)&col_2, (DWORD*)&col_3);
      // Overwrite the previous alpha bits with the alpha block
      DecodeAlpha3BitLinear( pImPos, pAlphaBlock, pixWidth);
      // next block
      pBlock++;
    }
  }
}
