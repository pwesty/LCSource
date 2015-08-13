#include "stdafx.h"
#include "FontGen.h"
#include <Engine/Templates/Stock_CTextureData.h>

extern void WriteFont_t(CFontData &fdFont, const CImageInfo &iiFont, const CImageInfo &iiGrid, const CTFileName &fnFont);
#define INVALID_CHARACTER 31


SINT _aiCharWidth[256]; // width of each character
SINT _iCharHeight = 0;
extern UBYTE _aubCharTable[256];
CFontCharData _afcdCharData[256];

static void MyDrawLine(CImageInfo &iiDst, PIX pixX0, PIX pixY0, PIX pixX1, PIX pixY1)
{
  PIX pixWidth = pixX1 - pixX0;
  PIX pixHeight = pixY1 - pixY0;
  PIX pixStepX = 0;
  PIX pixStepY = 0;
  PIX ctpixs = 0;

  // Only horizontal and vertical lines suported
  if(pixWidth!=0) {
    ASSERT(pixHeight==0);
    ctpixs = pixWidth;
    pixStepX=1;
  }
  if(pixHeight!=0) {
    ASSERT(pixWidth==0);
    ctpixs = pixHeight;
    pixStepY=1;
  }

  PIX pixFirst = pixX0 + iiDst.ii_Width*pixY0;
  PIX *ppixData = (PIX*)iiDst.ii_Picture;
  PIX *ppix = &ppixData[pixFirst];
  for(INDEX ipix=0;ipix<ctpixs;ipix++) {
    (*ppix) = 0xFF000000;
    ppix+=pixStepX;
    ppix+=pixStepY * iiDst.ii_Width;
  }
}

static void DrawCharCell(CImageInfo &iiDst, CFontCharData &fcd, PIX pixCharHeight)
{
  PIXaabbox2D rcCell;
  rcCell.minvect = PIX2D(fcd.fcd_pixXOffset, fcd.fcd_pixYOffset);
  rcCell.maxvect = PIX2D(fcd.fcd_pixXOffset + fcd.fcd_pixEnd, fcd.fcd_pixYOffset + pixCharHeight);

  if(rcCell.minvect(1)>=iiDst.ii_Width  || rcCell.minvect(1)<0 ||
     rcCell.maxvect(1)>=iiDst.ii_Width  || rcCell.maxvect(1)<0 ||
     rcCell.minvect(2)>=iiDst.ii_Height || rcCell.minvect(2)<0 ||
     rcCell.maxvect(2)>=iiDst.ii_Height || rcCell.maxvect(2)<0 ) {
    // ASSERT(FALSE);
    return;
  }
/*
  ASSERT(rcCell.minvect(1) < iiDst.ii_Width && rcCell.minvect(1) >= 0);
  ASSERT(rcCell.maxvect(1) < iiDst.ii_Width && rcCell.maxvect(1) >= 0);

  ASSERT(rcCell.minvect(2) < iiDst.ii_Height && rcCell.minvect(2) >= 0);
  ASSERT(rcCell.maxvect(2) < iiDst.ii_Height  && rcCell.maxvect(2) >= 0);
*/
  // lower
  MyDrawLine(iiDst,rcCell.minvect(1), rcCell.minvect(2), rcCell.maxvect(1), rcCell.minvect(2));
  // upper
  MyDrawLine(iiDst,rcCell.minvect(1), rcCell.maxvect(2), rcCell.maxvect(1), rcCell.maxvect(2));
  // left
  MyDrawLine(iiDst,rcCell.minvect(1), rcCell.minvect(2), rcCell.minvect(1), rcCell.maxvect(2));
  // right
  MyDrawLine(iiDst,rcCell.maxvect(1), rcCell.minvect(2), rcCell.maxvect(1), rcCell.maxvect(2));
}

void GenerateFontTexture_t(CImageInfo &iiDst, CImageInfo &iiGrid, const CTString &bf_strName, const INDEX bf_iFontSize,
                           INDEX bf_iLastChar, INDEX bf_iFirstChar, INDEX iAlignH, INDEX iAlignV,
                           INDEX iPaddingX, INDEX iPaddingY, INDEX iWidthAdd, INDEX iHeightAdd,
                           ULONG ulFlags, INDEX ctShadows)
{

  memset(&_aiCharWidth[0],0,sizeof(_aiCharWidth));
  memset(&_afcdCharData[0],0,sizeof(_afcdCharData));

  _aubCharTable[INVALID_CHARACTER] = TRUE;

  // Extra padding if the font is in italic!
  if(ulFlags&FNT_ITALIC) iPaddingX += 2;

  // Get the Desktop handle
  HWND hWnd = GetDesktopWindow();
  if(hWnd == NULL) {
    throw("Could not get handle to Desktop window");
  }
  // Get device content
  HDC hDC = GetDC(hWnd);
  if(hDC == NULL) {
    throw("Could not get DC from Desktop");
  }

  // Create a compatible memory DC
  HDC hMemDC = CreateCompatibleDC(hDC);
  if(hMemDC == NULL) {
    throw("Could not create compatible DC");
  }

  // Create and fill the structure containing the information about the DIBitmap
  BITMAPINFO bInfo;
  memset(&bInfo, 0, sizeof(BITMAPINFO));
  bInfo.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
  bInfo.bmiHeader.biWidth       = iiDst.ii_Width;
  bInfo.bmiHeader.biHeight      = iiDst.ii_Height;
  bInfo.bmiHeader.biPlanes      = 1;
  bInfo.bmiHeader.biBitCount    = iiDst.ii_BitsPerPixel;
  bInfo.bmiHeader.biCompression = BI_RGB;

  // Create a DIBSurface
  BYTE *pbData=NULL;
  HBITMAP bmpDibSurface = CreateDIBSection(hMemDC, &bInfo, DIB_RGB_COLORS, (VOID **)&pbData, NULL, 0);
  if(bmpDibSurface == NULL) throw("Could not create DIB section");

  // Bind it to the newly created memDC
  HBITMAP bmpOldBitmap = (HBITMAP)SelectObject(hMemDC, bmpDibSurface);

  // Select the background color (Black) and the text color (White)
  SetBkColor(hMemDC, RGB(0, 0, 0));
  SetTextColor(hMemDC, RGB(255, 255, 255));

  // Prepare params for font creation
  BOOL bItalic = ulFlags&FNT_ITALIC;
  BOOL bUnderline = ulFlags&FNT_UNDERLINE;
  BOOL bStrikeOut = ulFlags&FNT_STRIKEOUT;
  UINT uiBold = FW_NORMAL;
  UINT uiAntialiased = NONANTIALIASED_QUALITY;
  if(ulFlags&FNT_BOLD) uiBold = FW_SEMIBOLD;
  if(ulFlags&FNT_ANTIALIASED) uiAntialiased = ANTIALIASED_QUALITY;

  // Create font
  HFONT hFont = CreateFont(-(INT)bf_iFontSize,0,0,0,uiBold, bItalic, bUnderline,bStrikeOut,DEFAULT_CHARSET,
                            OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,uiAntialiased,FF_DONTCARE|DEFAULT_PITCH,
                            (const char*)bf_strName);

  // Check if there was an error creating the font
  if(hFont == NULL) {
    throw("Could not create font");
  }
  HFONT hFontOld = (HFONT)SelectObject(hMemDC, hFont);

  TEXTMETRIC textMetric;
  if(!GetTextMetrics(hMemDC, &textMetric)) {
    throw("Could not get font metrics");
  }

  // INT nHeight = -MulDiv(bf_iFontSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);

  // Calculate the different parameters
  _iCharHeight = bf_iFontSize + bf_iFontSize / 3;
  SINT iTexWidth   = bInfo.bmiHeader.biWidth;
  SINT iTexHeight  = bInfo.bmiHeader.biHeight;

  // Set character height
  FLOAT bf_fCharHeight = FLOAT(_iCharHeight); // !!!!!!!

  
  // Set character widths
  if(!GetCharWidth32(hMemDC, 0, 255, _aiCharWidth)) {
    throw("Could not get character width");
  }

  // 25A1

  // Print all the chars on the bitmap
  INT iCurrent = bf_iFirstChar;
  for(INT iy=_iCharHeight+iPaddingY; (iy<iTexHeight)&&(iCurrent<bf_iLastChar); iy+=(_iCharHeight+iPaddingY)) {
    //for(INT ix=_aiCharWidth[iCurrent]+iPaddingX; (ix<iTexWidth)&&(iCurrent<bf_iLastChar);ix+=(_aiCharWidth[iCurrent]+iPaddingX))
    INT ix=0;
    while(TRUE) {
      if(_aubCharTable[iCurrent]==TRUE) {
        ix+=(_aiCharWidth[iCurrent]+iPaddingX);
      }
      if((ix>=iTexWidth)||(iCurrent>=bf_iLastChar)) {
        break;
      }
      // if character is in table
      if(_aubCharTable[iCurrent]==TRUE) {
        CHAR chCurrent = (CHAR)iCurrent;
        INDEX iPosX = ix-_aiCharWidth[iCurrent]-iPaddingX;
        INDEX iPosY = iy-_iCharHeight-iPaddingY;
        
        PIX pixPosX = iPosX;
        PIX pixPosY = iPosY;
        
        // Center horisontal align
        if(iAlignH==1) {
          pixPosX+=iWidthAdd/2;
        // right align
        } else if(iAlignH==2) {
          pixPosX+=iWidthAdd;
        }

        // Aligh center verticaly
        if(iAlignV==1) {
          pixPosY+=iHeightAdd/2;
        // Align down
        } else if(iAlignV==2){
          pixPosY+=iHeightAdd;
        }

        TextOut(hMemDC, pixPosX, pixPosY, &chCurrent, 1);

        _afcdCharData[iCurrent].fcd_pixXOffset = iPosX;
        _afcdCharData[iCurrent].fcd_pixYOffset = iPosY;
        _afcdCharData[iCurrent].fcd_pixStart = 0;
        _afcdCharData[iCurrent].fcd_pixEnd = _aiCharWidth[iCurrent] + iWidthAdd;
        if(_iCharHeight+iHeightAdd < iiDst.ii_Height) {
          DrawCharCell(iiGrid, _afcdCharData[iCurrent], _iCharHeight + iHeightAdd);
        } else {
          ASSERT(FALSE);
        }
      }
      iCurrent++;
    }
  }

  // for each char
  for(INDEX ich=0;ich<256;ich++) {
    // that is not in table or is out of specified chars
    BOOL bInvalidChar = !_aubCharTable[ich] || (ich<bf_iFirstChar || ich>=bf_iLastChar);
    // marke as excluded char 
    if(ich!=10 && ich!=13 && bInvalidChar) {
      // map as invalid character
      _afcdCharData[ich].fcd_pixStart   =  _afcdCharData[INVALID_CHARACTER].fcd_pixStart;
      _afcdCharData[ich].fcd_pixEnd     =  _afcdCharData[INVALID_CHARACTER].fcd_pixEnd;
      _afcdCharData[ich].fcd_pixXOffset =  _afcdCharData[INVALID_CHARACTER].fcd_pixXOffset;
      _afcdCharData[ich].fcd_pixYOffset =  _afcdCharData[INVALID_CHARACTER].fcd_pixYOffset;
    }
  }
  
  // Flush the GDI command cache before proceding further
  GdiFlush();

  // Copy font image to bitmap font texture
  INDEX ctpix = iiDst.ii_Width*iiDst.ii_Height;
  SLONG slSize = ctpix * iiDst.ii_BitsPerPixel/8;

  memcpy(&iiDst.ii_Picture[0],pbData,slSize);

  // calc shadow modulo
  INDEX iShadowModulo = -iiDst.ii_Width + 1;

  // if font has shadow
  if(ulFlags&FNT_HAS_SHADOW) {
    // for each pixel 
    for(INDEX ipix=0;ipix<ctpix;ipix++) {
      UBYTE &ubRed   = iiDst.ii_Picture[ipix*4];
      UBYTE &ubAlpha = iiDst.ii_Picture[ipix*4+3];
      ubAlpha = ClampUp(ubAlpha + ubRed,255);


      for(INDEX isp=0;isp<ctShadows;isp++) {
        PIX pixShadowPix = ipix + iShadowModulo*(isp+1);
        // if shadow pixel is inside texture
        if(pixShadowPix>=0 && pixShadowPix<ctpix) {
          UBYTE &ubShadowAlpha = iiDst.ii_Picture[pixShadowPix*4+3];
          ubShadowAlpha = ClampUp(ubShadowAlpha + ubRed,255);
        }
      }
    }
  } else {
    // for each pixel 
    for(INDEX ipix=0;ipix<ctpix;ipix++) {
      UBYTE &ubRed   = iiDst.ii_Picture[ipix*4];
      UBYTE &ubGreen = iiDst.ii_Picture[ipix*4+1];
      UBYTE &ubBlue  = iiDst.ii_Picture[ipix*4+2];
      UBYTE &ubAlpha = iiDst.ii_Picture[ipix*4+3];
      ubAlpha = ubRed;
      ubRed = 0xFF;
      ubGreen = 0xFF;
      ubBlue = 0xFF;
    }
  }

  FlipBitmap( &iiDst.ii_Picture[0], &iiDst.ii_Picture[0], iiDst.ii_Width, iiDst.ii_Height, 2, TRUE);

  // CleanUP the resources used!
  SelectObject(hMemDC, hFontOld);
  DeleteObject(hFont);
  SelectObject(hMemDC, bmpOldBitmap);
  DeleteObject(bmpDibSurface);
  DeleteDC(hMemDC);
  ReleaseDC(hWnd, hDC);
}

CTextureData *GetGridTexture(void)
{
  return NULL;
}

extern void GenerateFont_t(CFontData &fdFont,CImageInfo &iiDst, CImageInfo &iiGrid, const CTString &strName, const INDEX iFontSize,
                           INDEX iLastChar, INDEX iFirstChar, INDEX iAlignH, INDEX iAlignV, INDEX iPaddingX, INDEX iPaddingY,
                           INDEX iWidthAdd, INDEX iHeightAdd, ULONG ulFlags, INDEX ctShadows)
{
  // Prepare font data
  GenerateFontTexture_t(iiDst,iiGrid,strName,iFontSize,iLastChar,iFirstChar, iAlignH, iAlignV, iPaddingX,iPaddingY,iWidthAdd,iHeightAdd,ulFlags,ctShadows);

  // get char with max width
  INDEX iMaxWidth = 0;
  INDEX  ich;
  for(ich=0;ich<256;ich++) {
    if(_aubCharTable[ich]==TRUE) {
      iMaxWidth = Max((INDEX)_aiCharWidth[ich],iMaxWidth);
    }
  }

  fdFont.Clear();
  fdFont.fd_pixCharWidth = iMaxWidth;
  fdFont.fd_pixCharHeight = _iCharHeight + iHeightAdd;
  if(ulFlags&FNT_HAS_SHADOW) {
    fdFont.fd_pixShadowSpacing = ctShadows;
  } else {
    fdFont.fd_pixShadowSpacing = 0;
  }
  fdFont.fd_ulFlags = NONE;
  for(ich=0;ich<256;ich++) {
    fdFont.fd_fcdFontCharData[ich] = _afcdCharData[ich];
  }

  CTFileName fnFont = (CTString)"Temp\\TempFont.fnt";
  WriteFont_t(fdFont,iiDst,iiGrid,fnFont);
  fnFont = fnFont.NoExt();
  // delete temp files
  // CTDeleteFile(fnFont + ".tex");
  CTDeleteFile(fnFont + ".tga");
  CTDeleteFile(fnFont + "G.tga");
  CTDeleteFile(fnFont + ".fnt");
}


