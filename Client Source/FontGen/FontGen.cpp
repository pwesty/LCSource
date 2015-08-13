#include "stdafx.h"
#include "resource.h"
#include "FontGen.h"
#include <Engine/Templates/Stock_CTextureData.h>

#define CONFIG_VERSION 3

static HWND _hWnd;
static HINSTANCE _hInstance;
static CImageInfo _iiFont;
static CImageInfo _iiGrid;
static CDrawPort *_pdpDrawPort = NULL;
static CViewPort *_pvpViewPort = NULL;
static CTextureObject _toPreview;
static CTextureData *_ptdGrid = NULL;
static CFontData _fdFont;
static CFontData *_pfdCurrentFont = NULL;
static const INDEX _aiFontSizes[] = {8,9,10,11,12,14,16,18,20,22,24,26,28,36,48,72};
static const INDEX _aiTexSizes[] = {64,128,256,512,1024,2048};
static CTString _strCharTable;
static BOOL _bShowTexture = FALSE;
static BOOL _bInitialized = FALSE;
static BOOL _bShowGrid = TRUE;
static CTFileName _fnCharacterTable;


extern void GenerateFont_t(CFontData &fdFont,CImageInfo &iiDst, CImageInfo &iiGrid, const CTString &strName, const INDEX iFontSize,
                           INDEX iLastChar, INDEX iFirstChar, INDEX iAlignH, INDEX iAlignV, INDEX iPaddingX, INDEX iPaddingY, 
                           INDEX iWidthAdd, INDEX iHeightAdd, ULONG ubFlags,INDEX ctShadow);

extern UBYTE _aubCharTable[256] = ""; // table of characters that will be included in tex

static void RefreshCanvas()
{
  HWND hCanvas = GetDlgItem(_hWnd,IDC_CANVAS);
  InvalidateRect(hCanvas,NULL,FALSE);
  UpdateWindow(hCanvas);
}

static void UpdatePreviewButton(void)
{
  if(_bShowTexture) {
    SetDlgItemText(_hWnd,IBC_SHOW_TEXTURE,"Preview");
  } else {
    SetDlgItemText(_hWnd,IBC_SHOW_TEXTURE,"Texture");
  }
}

static ULONG GetFontFlags(void)
{
  ULONG ulFlags = 0;
  if(IsDlgButtonChecked(_hWnd,ICB_BOLD) == BST_CHECKED) ulFlags|=FNT_BOLD;
  if(IsDlgButtonChecked(_hWnd,ICB_ITALIC) == BST_CHECKED) ulFlags|=FNT_ITALIC;
  if(IsDlgButtonChecked(_hWnd,ICB_UNDERLINE) == BST_CHECKED) ulFlags|=FNT_UNDERLINE;
  if(IsDlgButtonChecked(_hWnd,ICB_STRIKE_OUT) == BST_CHECKED) ulFlags|=FNT_STRIKEOUT;
  if(IsDlgButtonChecked(_hWnd,ICB_ANTIALIASED) == BST_CHECKED) ulFlags|=FNT_ANTIALIASED;
  if(IsDlgButtonChecked(_hWnd,ICB_HAS_SHADOW) == BST_CHECKED) ulFlags|=FNT_HAS_SHADOW;
  return ulFlags;
}

static void SetFontFlags(ULONG ulFlags)
{
  CheckDlgButton(_hWnd,ICB_BOLD,BST_UNCHECKED);
  CheckDlgButton(_hWnd,ICB_ITALIC,BST_UNCHECKED);
  CheckDlgButton(_hWnd,ICB_UNDERLINE,BST_UNCHECKED);
  CheckDlgButton(_hWnd,ICB_STRIKE_OUT,BST_UNCHECKED);
  CheckDlgButton(_hWnd,ICB_ANTIALIASED,BST_UNCHECKED);
  CheckDlgButton(_hWnd,ICB_HAS_SHADOW,BST_UNCHECKED);

  if(ulFlags&FNT_BOLD) CheckDlgButton(_hWnd,ICB_BOLD,BST_CHECKED);
  if(ulFlags&FNT_ITALIC) CheckDlgButton(_hWnd,ICB_ITALIC,BST_CHECKED);
  if(ulFlags&FNT_UNDERLINE) CheckDlgButton(_hWnd,ICB_UNDERLINE,BST_CHECKED);
  if(ulFlags&FNT_STRIKEOUT) CheckDlgButton(_hWnd,ICB_STRIKE_OUT,BST_CHECKED);
  if(ulFlags&FNT_ANTIALIASED) CheckDlgButton(_hWnd,ICB_ANTIALIASED,BST_CHECKED);
  if(ulFlags&FNT_HAS_SHADOW) CheckDlgButton(_hWnd,ICB_HAS_SHADOW,BST_CHECKED);
}

static CTString GetStringFromControl(INDEX iControlID)
{
  char strText[4096];
  GetDlgItemText(_hWnd,iControlID,strText,4095);
  return strText;
}

static void SetStringToControl(INDEX iControlID, const CTString &strText)
{
  SetDlgItemText(_hWnd,iControlID,(const char*)strText);
}

static INDEX GetIntFromControl(INDEX iControlID)
{
  CTString strText = GetStringFromControl(iControlID);
  INDEX iRet = atoi((const char*)strText);
  return iRet;
}

static void SetControlInt(INDEX iControlID, INT iValue)
{
  char strText[64] = {0,};
  itoa(iValue,strText,10);
  SetDlgItemText(_hWnd,iControlID,strText);
}

static void SetComboString(INDEX iControlID, const CTString &strText)
{
  HWND hWnd = GetDlgItem(_hWnd,iControlID);
  INDEX iString = SendMessage(hWnd,CB_FINDSTRINGEXACT,(WPARAM) -1,(LPARAM)(const char*)strText);
  SendMessage(hWnd,CB_SETCURSEL,(WPARAM)iString,0);

}

static void SetComboInt(INDEX iControlID, const INT iValue)
{
	char strText[64] = {0,};
	itoa(iValue,strText,10);
	SetComboString(iControlID,strText);
}


static INDEX GetComboIndex(INDEX iControlID)
{
  HWND hWnd = GetDlgItem(_hWnd,iControlID);
  return SendMessage(hWnd,CB_GETCURSEL,0,0);
}

static void SetComboIndex(INDEX iControlID, INDEX iIndex)
{
  HWND hWnd = GetDlgItem(_hWnd,iControlID);
  SendMessage(hWnd,CB_SETCURSEL,(WPARAM) iIndex,0);
#if _DEBUG
  INDEX ctItems = SendMessage(hWnd,CB_GETCOUNT,0,0);
  INDEX iSelected = SendMessage(hWnd,CB_GETCURSEL,0,0);
  int a=0;
#endif
}


static CTString GetFontName(void)
{
  char strFontName[MAX_PATH] = "";
  INDEX iSelected = SendDlgItemMessage(_hWnd,ICB_FONT_NAMES,CB_GETCURSEL,0,0);
  SendDlgItemMessage(_hWnd,ICB_FONT_NAMES,CB_GETLBTEXT,(WPARAM) iSelected, (LPARAM) (LPCSTR) strFontName);
  return strFontName;
}

static void GenerateFont(void)
{
  if(!_bInitialized) return;
  try {
    _iiFont.Clear();
    _iiGrid.Clear();

    _iiFont.ii_Width = GetIntFromControl(ICB_TEX_WIDTH);
    _iiFont.ii_Height = GetIntFromControl(ICB_TEX_HEIGHT);
    _iiFont.ii_BitsPerPixel = 32;

    _iiGrid.ii_Width = _iiFont.ii_Width;
    _iiGrid.ii_Height = _iiFont.ii_Height;
    _iiGrid.ii_BitsPerPixel = _iiFont.ii_BitsPerPixel;

    SLONG slSize = _iiFont.ii_Width*_iiFont.ii_Height * _iiFont.ii_BitsPerPixel/8;
    _iiFont.ii_Picture = (UBYTE*)AllocMemory(slSize);
    _iiGrid.ii_Picture = (UBYTE*)AllocMemory(slSize);

    memset(_iiFont.ii_Picture,0,slSize);
    memset(_iiGrid.ii_Picture,0,slSize);

    CTString strFontName = GetFontName();
    if(strFontName.Length() == 0) {
      throw("No font selected");
    }


    ULONG ulFlags = GetFontFlags();
    INDEX iFontSize = GetIntFromControl(IEC_FONT_SIZE);
    INDEX iFirstChar = GetIntFromControl(IEC_FIRST_CHAR);
    INDEX iLastChar = GetIntFromControl(IEC_LAST_CHAR);
    INDEX iAlignH  = GetComboIndex(IDC_ALIGN_H);
    INDEX iAlignV  = GetComboIndex(IDC_ALIGN_V);
    INDEX iPaddingX = GetIntFromControl(IEC_PADDINGX);
    INDEX iPaddingY = GetIntFromControl(IEC_PADDINGY);
    INDEX iWidthAdd = GetIntFromControl(IEC_WIDTH_ADD);
    INDEX iHeightAdd = GetIntFromControl(IEC_HEIGHT_ADD);
    INDEX ctShadows = GetIntFromControl(IEC_SHADOW_PASSES);
    
    if(ulFlags&FNT_HAS_SHADOW) {
      iPaddingX+=ctShadows;
      iPaddingY+=ctShadows;
      iWidthAdd+=ctShadows;
      iHeightAdd+=ctShadows;
    }

    _pfdCurrentFont = NULL;
    GenerateFont_t(_fdFont,_iiFont,_iiGrid,strFontName,iFontSize,iLastChar,iFirstChar,iAlignH,iAlignV,iPaddingX,iPaddingY,iWidthAdd,iHeightAdd,ulFlags,ctShadows);
    _pfdCurrentFont = &_fdFont;

    RefreshCanvas();
  } catch (char *strErr) {
    MessageBox(_hWnd,strErr,0,0);
  }
}

static void ClearCharTable(void) {
  memset(&_aubCharTable[0],1,sizeof(_aubCharTable));
  SetDlgItemText(_hWnd,IDC_CHARACTER_TABLE,"");
  _aubCharTable[10] = 0;
  _aubCharTable[13] = 0;
  _fnCharacterTable = CTString("");
}

static void LoadCharTable(const CTFileName &fnCharTable) {
  if(fnCharTable.Length() == 0) {
    ClearCharTable();
    return;
  }

  try {
    SetDlgItemText(_hWnd,IDC_CHARACTER_TABLE,(const char*)(fnCharTable.FileName() + fnCharTable.FileExt()));
    _strCharTable.Load_t(fnCharTable);
    INDEX iLen = _strCharTable.Length();
    memset(&_aubCharTable[0],0,sizeof(_aubCharTable));

    for(INDEX ic=0;ic<iLen;ic++) {
      unsigned char ch = _strCharTable[ic];
      _aubCharTable[ch] = 1;
    }
    _aubCharTable[10] = 0;
    _aubCharTable[13] = 0;
    _fnCharacterTable = fnCharTable;
  } catch(char *strErr) {
    MessageBox(_hWnd,strErr,0,0);
  }
}

static void SaveSettings(const CTFileName &fnConfgin)
{
  if(!_bInitialized) return;
  try {
    CTFileStream ostr;
    ostr.Create_t(fnConfgin);

    INDEX iVersion=CONFIG_VERSION;
    ULONG ulFlags = GetFontFlags();
    INDEX iFontSize = GetIntFromControl(IEC_FONT_SIZE);
    INDEX iFirstChar = GetIntFromControl(IEC_FIRST_CHAR);
    INDEX iLastChar = GetIntFromControl(IEC_LAST_CHAR);
    INDEX iAlignH  = GetComboIndex(IDC_ALIGN_H);
    INDEX iAlignV  = GetComboIndex(IDC_ALIGN_V);
    INDEX iPaddingX = GetIntFromControl(IEC_PADDINGX);
    INDEX iPaddingY = GetIntFromControl(IEC_PADDINGY);
    INDEX iWidthAdd = GetIntFromControl(IEC_WIDTH_ADD);
    INDEX iHeightAdd = GetIntFromControl(IEC_HEIGHT_ADD);
    INDEX ctShadows = GetIntFromControl(IEC_SHADOW_PASSES);
    INDEX iTexWidth = GetIntFromControl(ICB_TEX_WIDTH);
    INDEX iTexHeight = GetIntFromControl(ICB_TEX_HEIGHT);
    const CTString strFontName = GetStringFromControl(ICB_FONT_NAMES);
    const CTString strSampleText = GetStringFromControl(IEC_SAMPLE_TEXT);

  
    ostr<<iVersion;
    ostr<<ulFlags;
    ostr<<iFontSize;
    ostr<<iFirstChar;
    ostr<<iLastChar;
    ostr<<iAlignH;
    ostr<<iAlignV;
    ostr<<iPaddingX;
    ostr<<iPaddingY;
    ostr<<iWidthAdd;
    ostr<<iHeightAdd;
    ostr<<ctShadows;
    ostr<<iTexWidth;
    ostr<<iTexHeight;
    ostr<<strFontName;
    ostr<<strSampleText;
    ostr<<_bShowTexture;
    ostr<<_bShowGrid;
    ostr<<_fnCharacterTable;
  } catch(char *strErr) {
    MessageBox(_hWnd,strErr,0,0);
  }
}

static void LoadSettings(const CTFileName &fnConfig)
{
  if(!_bInitialized) return;
  if(!FileExists(fnConfig)) return;

  try {
    CTFileStream istr;
    istr.Open_t(fnConfig,CTFileStream::OM_READ);

    INDEX iVersion;
    ULONG ulFlags;
    INDEX iFontSize;
    INDEX iFirstChar;
    INDEX iLastChar;
    INDEX iAlignH;
    INDEX iAlignV;
    INDEX iPaddingX;
    INDEX iPaddingY;
    INDEX iWidthAdd;
    INDEX iHeightAdd;
    INDEX ctShadows;
    INDEX iTexWidth;
    INDEX iTexHeight;
    CTString strFontName;
    CTString strSampleText;

    istr>>iVersion;
    if(iVersion!=CONFIG_VERSION) {
      WarningMessage("Invalid config version.\nExpected Ver \"%d\" but found \"%d\"\n", CONFIG_VERSION, iVersion);
      return;
    }

    istr>>ulFlags;
    istr>>iFontSize;
    istr>>iFirstChar;
    istr>>iLastChar;
    istr>>iAlignH;
    istr>>iAlignV;
    istr>>iPaddingX;
    istr>>iPaddingY;
    istr>>iWidthAdd;
    istr>>iHeightAdd;
    istr>>ctShadows;
    istr>>iTexWidth;
    istr>>iTexHeight;
    istr>>strFontName;
    istr>>strSampleText;
    istr>>_bShowTexture;
    istr>>_bShowGrid;
    istr>>_fnCharacterTable;

    UpdatePreviewButton();
    SetFontFlags(ulFlags);
    SetControlInt(IEC_FONT_SIZE,iFontSize);
    SetControlInt(IEC_FIRST_CHAR,iFirstChar);
    SetControlInt(IEC_LAST_CHAR,iLastChar);
    SetComboIndex(IDC_ALIGN_H,iAlignH);
    SetComboIndex(IDC_ALIGN_V,iAlignV);
    SetControlInt(IEC_PADDINGX,iPaddingX);
    SetControlInt(IEC_PADDINGY,iPaddingY);
    SetControlInt(IEC_WIDTH_ADD,iWidthAdd);
    SetControlInt(IEC_HEIGHT_ADD,iHeightAdd);
    SetControlInt(IEC_SHADOW_PASSES,ctShadows);

    SetComboInt(ICB_TEX_WIDTH,iTexWidth);
    SetComboInt(ICB_TEX_HEIGHT,iTexHeight);
    SetComboString(ICB_FONT_NAMES,strFontName);
    SetStringToControl(IEC_SAMPLE_TEXT,strSampleText);

    LoadCharTable(_fnCharacterTable);
    GenerateFont();
  } catch(char *strErr) {
    WarningMessage("%s", strErr);
  }
}

static CTFileName SelectCharTable(void)
{
  try {
    char strFileName[MAX_PATH+1] = "";
    OPENFILENAME ofn;
    memset(&ofn,0,sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = _hWnd;
    ofn.hInstance = _hInstance;
    ofn.lpstrFilter = " (*.tbl)\0*.tbl\0";
    ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
    ofn.lpstrFile  = strFileName;
    ofn.nMaxFile  = MAX_PATH;
    if(GetOpenFileName(&ofn)) {
      CTFileName fnFullFileName = (CTString)ofn.lpstrFile;
      fnFullFileName.RemoveApplicationPath_t();
      return fnFullFileName;
    } else {
      return CTString("");
    }
  } catch(char *) {
    return CTString("");
  }
}

extern void WriteFont_t(CFontData &fdFont, const CImageInfo &iiFont, const CImageInfo &iiGrid, const CTFileName &fnFont)
{
  MEX inMex = Max(iiFont.ii_Height,iiFont.ii_Width);
  INDEX ctMipmaps = 0;// GetNoOfMipmaps(iiFont.ii_Width,iiFont.ii_Height);

  CTFileName fnName = fnFont.NoExt();
  try {
    fnName.RemoveApplicationPath_t();
  } catch(char *) {
  }
  // save font tga file
  iiFont.SaveTGA_t(fnName + ".tga");
  iiGrid.SaveTGA_t(fnName + "G.tga");
  // create tex file from saved targa
  CreateTexture_t( fnName+".tga",  fnName+".tex",  inMex, ctMipmaps, TEX_CONSTANT);
  CreateTexture_t( fnName+"G.tga", fnName+"G.tex", inMex, ctMipmaps);
  
  // set texture source to saved font
  fdFont.fd_ptdTextureData = _pTextureStock->Obtain_t(fnName+".tex");
  fdFont.fd_ptdTextureData->Reload();
  fdFont.fd_ptdTextureData->Force(TEX_CONSTANT); // don't mess with created textures
  fdFont.fd_fnTexture = fnName+".tex";

  _ptdGrid = _pTextureStock->Obtain_t(fnName + "G.tex");
  if(_ptdGrid!=NULL) {
    _ptdGrid->Reload();
    _ptdGrid->Force(TEX_CONSTANT); // don't mess with created textures
  }

  // finaly save fnt file
  CTFileStream ostr;
  ostr.Create_t(fnName + ".fnt");
  fdFont.Write_t(&ostr);
}

static void SaveFont()
{
  try {
    if(_iiFont.ii_Picture==NULL) {
      throw("Font not generated");
    }
    char strFileName[MAX_PATH+1] = "";
    OPENFILENAME ofn;
    memset(&ofn,0,sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = _hWnd;
    ofn.hInstance = _hInstance;
    ofn.lpstrFilter = "Font files (*.fnt)\0*.fnt\0";
    ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
    ofn.lpstrFile  = strFileName;
    ofn.nMaxFile  = MAX_PATH;
    if(GetSaveFileName(&ofn)) {
      CTFileName fnFullFileName = (CTString)ofn.lpstrFile;
      WriteFont_t(_fdFont,_iiFont,_iiGrid,fnFullFileName);
    }
  } catch(char *strErr) {
    MessageBox(_hWnd,strErr,0,0);
  }
}

int APIENTRY ProcEnumFonts(LOGFONT *plf, TEXTMETRIC *ptm , DWORD dwType, LPARAM lpData)
{
  SendDlgItemMessage(_hWnd,ICB_FONT_NAMES,CB_ADDSTRING,0,(LPARAM) (LPCTSTR) plf->lfFaceName);
  return TRUE;
}

static void PaintScreen()
{
  if(_pdpDrawPort==NULL) {
    return;
  }
  _pdpDrawPort->SetAsCurrent();
  // clear browsing window
  _pdpDrawPort->FillZBuffer( ZBUF_BACK);
  _pdpDrawPort->Fill( C_GRAY | CT_OPAQUE);

  if(_pfdCurrentFont!=NULL && _pfdCurrentFont->fd_ptdTextureData!=NULL && _ptdGrid!=NULL) {
    PIX pixWidth = _pfdCurrentFont->fd_ptdTextureData->GetPixWidth();
    PIX pixHeight = _pfdCurrentFont->fd_ptdTextureData->GetPixHeight();

    PIXaabbox2D rectPict;
    rectPict = PIXaabbox2D( PIX2D(0, 0), PIX2D(pixWidth, pixHeight));
    if(_bShowTexture) {
      // Show texture
      _toPreview.SetData(_pfdCurrentFont->fd_ptdTextureData);
      _pdpDrawPort->PutTexture(&_toPreview,rectPict);
      // if grid must be shown
      if(_bShowGrid) {
        // Show character grid
        _toPreview.SetData(_ptdGrid);
        _pdpDrawPort->PutTexture(&_toPreview,rectPict);
      }
      // Show texture grid
      _pdpDrawPort->DrawLine(0,0,pixWidth,0,0xFFFF00FF);
      _pdpDrawPort->DrawLine(0,0,0,pixHeight,0xFFFF00FF);
      _pdpDrawPort->DrawLine(0,pixHeight,pixWidth,pixHeight,0xFFFF00FF);
      _pdpDrawPort->DrawLine(pixWidth,0,pixWidth,pixHeight,0xFFFF00FF);
      _toPreview.SetData(NULL);
    } else {
      char strSampleText[2048];
      GetDlgItemText(_hWnd,IEC_SAMPLE_TEXT,strSampleText,2047);
      _pdpDrawPort->SetFont( _pfdCurrentFont);
      _pdpDrawPort->PutText( strSampleText, 0, 0, 0xFFFFFFFF);
    }
  }

  if(_pvpViewPort!=NULL) {
    // swap it
    _pvpViewPort->SwapBuffers();
  }
}

LRESULT APIENTRY CanvasProc(HWND hWnd, UINT iMsg , WPARAM wParam, LPARAM lParam) {
  if(iMsg==WM_PAINT) {
    PaintScreen();
  }
  return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

static void ResetConfig(void)
{
  SetWindowText(GetDlgItem(_hWnd,IEC_SAMPLE_TEXT),"!\"$#%&'()*+,-./\r\n0123456789:;<=>?\r\n@ABDEFGHIJKLMNOP\r\nQRSTUVWXYZ[\\]_{\r\n|}~\r\nThe quick brown fox jumps over the lazy dog\r\nThe quick brown fox jumps over the lazy dog\r\nThe quick brown fox jumps over the lazy dog\r\nThe quick brown fox jumps over the lazy dog\r\nThe quick brown fox jumps over the lazy dog");
  SetWindowText(GetDlgItem(_hWnd,IEC_FIRST_CHAR),"0");//"32");
  SetWindowText(GetDlgItem(_hWnd,IEC_LAST_CHAR),"255");//"126");
  SetWindowText(GetDlgItem(_hWnd,IEC_FONT_SIZE),"14");
  SetComboIndex(IDC_ALIGN_H,0);
  SetComboIndex(IDC_ALIGN_V,0);
  SetWindowText(GetDlgItem(_hWnd,IEC_PADDINGX),"2");
  SetWindowText(GetDlgItem(_hWnd,IEC_PADDINGY),"0");
  SetWindowText(GetDlgItem(_hWnd,IEC_WIDTH_ADD),"0");
  SetWindowText(GetDlgItem(_hWnd,IEC_HEIGHT_ADD),"1");
  SetWindowText(GetDlgItem(_hWnd,IEC_SHADOW_PASSES),"1");
}

static void Initialize(HWND hWnd)
{
  _hWnd = hWnd;

  ResetConfig();
 
  SE_InitEngine("");

  // Get device content
  HDC hDC = GetDC(hWnd);
  if(hDC==NULL) {
    ASSERT(FALSE);
    return;
  }

  EnumFonts(hDC,NULL,(FONTENUMPROC)ProcEnumFonts,0);
  SendDlgItemMessage(_hWnd,ICB_FONT_NAMES,CB_SETCURSEL,(WPARAM) 0,0);

  char strText[32] = {0,};

  INDEX ctFontSizes = sizeof(_aiFontSizes) / sizeof(INDEX);
  for(INDEX ifs=0;ifs<ctFontSizes;ifs++) {
    itoa(_aiFontSizes[ifs],strText,10);
    SendDlgItemMessage(_hWnd,ICB_FONT_SIZES,CB_ADDSTRING,0,(LPARAM) (LPCTSTR) strText);
  }
  SendDlgItemMessage(_hWnd,ICB_FONT_SIZES,CB_SETCURSEL,(WPARAM)4, 0);

  // Align
  SendDlgItemMessage(_hWnd,IDC_ALIGN_H,CB_ADDSTRING,0,(LPARAM) (LPCTSTR) "H");
  SendDlgItemMessage(_hWnd,IDC_ALIGN_H,CB_ADDSTRING,0,(LPARAM) (LPCTSTR) "C");
  SendDlgItemMessage(_hWnd,IDC_ALIGN_H,CB_ADDSTRING,0,(LPARAM) (LPCTSTR) "V");
  SendDlgItemMessage(_hWnd,IDC_ALIGN_V,CB_ADDSTRING,0,(LPARAM) (LPCTSTR) "H");
  SendDlgItemMessage(_hWnd,IDC_ALIGN_V,CB_ADDSTRING,0,(LPARAM) (LPCTSTR) "C");
  SendDlgItemMessage(_hWnd,IDC_ALIGN_V,CB_ADDSTRING,0,(LPARAM) (LPCTSTR) "V");
  SetComboIndex(IDC_ALIGN_H,1);
  SetComboIndex(IDC_ALIGN_V,1);

  INDEX ctTexSizes = sizeof(_aiTexSizes) / sizeof(INDEX);
  for(INDEX its=0;its<ctTexSizes;its++) {
    itoa(_aiTexSizes[its],strText,10);
    SendDlgItemMessage(_hWnd,ICB_TEX_WIDTH,CB_ADDSTRING,0,(LPARAM) (LPCTSTR) strText);
    SendDlgItemMessage(_hWnd,ICB_TEX_HEIGHT,CB_ADDSTRING,0,(LPARAM) (LPCTSTR) strText);
  }
  SendDlgItemMessage(_hWnd,ICB_TEX_WIDTH,CB_SETCURSEL,(WPARAM)2, 0);
  SendDlgItemMessage(_hWnd,ICB_TEX_HEIGHT,CB_SETCURSEL,(WPARAM)2, 0);

  ReleaseDC(hWnd, hDC);

  HWND hWndCanvas = GetDlgItem(hWnd,IDC_CANVAS);
  ASSERT(hWndCanvas!=NULL);

  ClearCharTable();
  SetWindowLong(hWndCanvas,GWL_WNDPROC,(LONG)CanvasProc);
  _pGfx->ResetDisplayMode(GAT_OGL);
 	_pGfx->CreateWindowCanvas(hWndCanvas, &_pvpViewPort, &_pdpDrawPort);
  UpdateWindow(hWndCanvas);
  _bInitialized = TRUE;
  GenerateFont();

  LoadSettings((CTString)"Temp\\GenFont.cfg");
}

BOOL CALLBACK MainDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch(uMsg) {
    case WM_INITDIALOG:
      Initialize(hDlg);
    return TRUE;
    case WM_CLOSE:
      EndDialog(hDlg, 0);
      SaveSettings((CTString)"Temp\\GenFont.cfg");
    return TRUE;
    case WM_COMMAND:
      switch(LOWORD(wParam)) {
        case ICB_BOLD:
        case ICB_ITALIC:
        case ICB_UNDERLINE:
        case ICB_STRIKE_OUT:
        case ICB_ANTIALIASED:
        case ICB_HAS_SHADOW:
          GenerateFont();
        return TRUE;
        case ICB_FONT_NAMES:
        case ICB_TEX_HEIGHT:
        case ICB_TEX_WIDTH:
        case IDC_ALIGN_H:
        case IDC_ALIGN_V:
          if(HIWORD(wParam) == CBN_SELCHANGE) {
            GenerateFont();
          }
        return TRUE;
        case ICB_FONT_SIZES:
          if((HIWORD(wParam)==CBN_EDITCHANGE) || (HIWORD(wParam)==CBN_SELCHANGE)) {
            GenerateFont();
          }
        return TRUE;
        case IEC_FIRST_CHAR:
        case IEC_LAST_CHAR:
        case IEC_SHADOW_PASSES:
        case IEC_PADDINGX:
        case IEC_PADDINGY:
        case IEC_WIDTH_ADD:
        case IEC_HEIGHT_ADD:
        case IEC_FONT_SIZE:
          if(HIWORD(wParam) == EN_CHANGE) {
            GenerateFont();
          }
        return TRUE;
        case IEC_SAMPLE_TEXT:
          if(HIWORD(wParam) == EN_CHANGE) {
            RefreshCanvas();
          }
        return TRUE;
        case IBC_LOAD_CHAR_TABLE:
          LoadCharTable(SelectCharTable());
          GenerateFont();
        return TRUE;
        case IBC_CLEAR_CHAR_TABLE:
          ClearCharTable();
          GenerateFont();
        return TRUE;

        case IBC_SAVE_FONT:
          SaveFont();
        return TRUE;
        case IBC_SHOW_TEXTURE:
          _bShowTexture =!_bShowTexture;
          UpdatePreviewButton();
          RefreshCanvas();
        return TRUE;
        case IBC_SHOW_GRID:
          _bShowGrid =!_bShowGrid;
          RefreshCanvas();
        return TRUE;
      }
    return FALSE;
  }
  return FALSE;
}

void Run()
{
  DialogBox(_hInstance,MAKEINTRESOURCE(IDD_MAIN), NULL,(DLGPROC) MainDlgProc);
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
  _hInstance = hInstance;
  CTSTREAM_BEGIN {
    Run();
  } CTSTREAM_END;

	return 1;
}



