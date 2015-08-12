/*
 * File:	ximalctex.h
 * Purpose:	LastChaos TEX Image Class Loader and Writer
 */
/* ============================================================
 * CxImageLCTEX (c) 13/Jun/2012 sykim70 - www.barunsongames.com
 * ============================================================
 */
#if !defined(__ximaLCTEX_h)
#define __ximaLCTEX_h

#include "ximage.h"

#if CXIMAGE_SUPPORT_LC_TEX

class CxImageLCTEX: public CxImage
{
public:
	CxImageLCTEX(): CxImage(CXIMAGE_FORMAT_LC_TEX) {}

//	bool Load(const TCHAR * imageFileName){ return CxImage::Load(imageFileName,CXIMAGE_FORMAT_LC_TEX);}
//	bool Save(const TCHAR * imageFileName){ return CxImage::Save(imageFileName,CXIMAGE_FORMAT_LC_TEX);}
	bool Decode(CxFile* hFile);
	bool Decode(FILE* hFile) { CxIOFile file(hFile); return Decode(&file); }

#if CXIMAGE_SUPPORT_ENCODE
	bool Encode(CxFile* hFile);
	bool Encode(FILE* hFile) { CxIOFile file(hFile); return Encode(&file); }
#endif // CXIMAGE_SUPPORT_ENCODE
};

#endif

#endif
