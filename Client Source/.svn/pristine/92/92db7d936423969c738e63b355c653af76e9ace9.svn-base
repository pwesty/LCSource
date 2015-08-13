// DlgPgTexture.cpp : implementation file
//

#include "stdafx.h"
#include "DlgPgTexture.h"

#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DIFFERENT_TEXTURE "Different\ntextures"
#define NO_TEXTURE "No texture"
#define NO_SELECTION "Empty\nselection"

/////////////////////////////////////////////////////////////////////////////
// CDlgPgTexture property page

IMPLEMENT_DYNCREATE(CDlgPgTexture, CPropertyPage)

CDlgPgTexture::CDlgPgTexture() : CPropertyPage(CDlgPgTexture::IDD)
{
	//{{AFX_DATA_INIT(CDlgPgTexture)
	m_strTextureFile = _T("");
	m_fOffsetV = 0.0f;
	m_fOffsetU = 0.0f;
	m_radioTexture = -1;
	m_fRotationU = 0.0f;
	m_fRotationV = 0.0f;
	m_fStretchU = 0.0f;
	m_fStretchV = 0.0f;
	m_strTextureDim = _T("");
	//}}AFX_DATA_INIT

  m_ctrlCombineColor.SetPickerType(  CColoredButton::PT_MFC);
}

CDlgPgTexture::~CDlgPgTexture()
{
}

void CDlgPgTexture::DoDataExchange(CDataExchange* pDX)
{
  if( theApp.m_bDisableDataExchange) return;

  CPropertyPage::DoDataExchange(pDX);

  // mark that property page has been modified
  SetModified( TRUE);

  // obtain document
  CWorldEditorDoc* pDoc = theApp.GetDocument();
  if( pDoc == NULL)  return;

  // polygon mode must be on
  if( pDoc->GetEditingMode() != POLYGON_MODE)  return;

  // if dialog is recieving data and control windows are valid
  if( (pDX->m_bSaveAndValidate == FALSE) && IsWindow( m_comboScroll.m_hWnd) )
  {
	  m_comboScroll.ResetContent();
    for(INDEX iScroll=0; iScroll<256; iScroll++)
    {
      CTString strScrollName = pDoc->m_woWorld.wo_attTextureTransformations[iScroll].tt_strName;
      if( strScrollName != CTString("") ) m_comboScroll.AddString( strScrollName);
    }
	  m_comboBlend.ResetContent();
    for(INDEX iBlend=0; iBlend<256; iBlend++)
    {
      CTString strBlendName = pDoc->m_woWorld.wo_atbTextureBlendings[iBlend].tb_strName;
      if( strBlendName != CTString("") ) m_comboBlend.AddString( strBlendName);
    }

    m_radioTexture = pDoc->m_iTexture;
    m_udTextureData.MarkUpdated();
    BOOL bSelectionExists = pDoc->m_selPolygonSelection.Count() != 0;
    // controls for editing polygon mapping can exist only while one polygon is selected
    GetDlgItem( IDC_STATIC_ROTATION_SHEAR)->EnableWindow( bSelectionExists);
    GetDlgItem( IDC_STATIC_STRETCH)->EnableWindow( bSelectionExists);
    GetDlgItem( IDC_ROTATION_U)->EnableWindow( bSelectionExists);
    GetDlgItem( IDC_ROTATION_V)->EnableWindow( bSelectionExists);
    GetDlgItem( IDC_STATIC_OFFSET)->EnableWindow( bSelectionExists);
    GetDlgItem( IDC_OFFSET_U)->EnableWindow( bSelectionExists);
    GetDlgItem( IDC_OFFSET_V)->EnableWindow( bSelectionExists);
    GetDlgItem( IDC_STRETCH_U)->EnableWindow( bSelectionExists);
    GetDlgItem( IDC_STRETCH_V)->EnableWindow( bSelectionExists);
    GetDlgItem( IDC_STATIC_BLEND)->EnableWindow( bSelectionExists);
    GetDlgItem( IDC_BLEND_TYPE)->EnableWindow( bSelectionExists);
    GetDlgItem( IDC_STATIC_SCROLL)->EnableWindow( bSelectionExists);
    GetDlgItem( IDC_SCROLL_COMBO)->EnableWindow( bSelectionExists);
    GetDlgItem( IDC_TEXTURE_FILE_T)->EnableWindow( bSelectionExists);
    GetDlgItem( IDC_BROWSE_TEXTURE)->EnableWindow( bSelectionExists);
    GetDlgItem( IDC_REMOVE_TEXTURE)->EnableWindow( bSelectionExists);
    GetDlgItem( IDC_CLAMP_U)->EnableWindow( bSelectionExists);
    GetDlgItem( IDC_CLAMP_V)->EnableWindow( bSelectionExists);
    GetDlgItem( IDC_REFLECTIVE)->EnableWindow( bSelectionExists);
    GetDlgItem( IDC_AFTER_SHADOW)->EnableWindow( bSelectionExists);
    GetDlgItem( IDC_PREVIEW_FRAME)->EnableWindow( bSelectionExists);
	  m_ctrlCombineColor.EnableWindow( bSelectionExists);

    m_comboScroll.EnableWindow( bSelectionExists);
    m_comboBlend.EnableWindow( bSelectionExists);

    GetDlgItem( IDC_RADIO_TEXTURE)->EnableWindow( TRUE);
    GetDlgItem( IDC_TEXTURE_2)->EnableWindow( TRUE);
    GetDlgItem( IDC_TEXTURE_3)->EnableWindow( TRUE);

	GetDlgItem( IDC_TEX_TRANS1)->EnableWindow( bSelectionExists);
	GetDlgItem( IDC_TEX_TRANS2)->EnableWindow( bSelectionExists);
	GetDlgItem( IDC_TEX_TRANS3)->EnableWindow( bSelectionExists);
	
    if( !bSelectionExists)
    {
      m_strTextureFile = NO_SELECTION;
      m_strTextureDim = "";
    }
    else
    {
      UBYTE ubFirstScroll;
      BOOL bSameScroll = TRUE;
      UBYTE ubFirstBlend;
      BOOL bSameBlend = TRUE;

      UBYTE ubFlagsOn = MAX_UBYTE;
      UBYTE ubFlagsOff = MAX_UBYTE;
      GetDlgItem( IDC_PREVIEW_FRAME)->EnableWindow( TRUE);
      INDEX ctPolygons = 0;
      // for each of the selected polygons
      FOREACHINDYNAMICCONTAINER(pDoc->m_selPolygonSelection, CBrushPolygon, itbpo)
      {
        CBrushPolygon &bpo = *itbpo;
        ubFlagsOn &= bpo.bpo_abptTextures[pDoc->m_iTexture].s.bpt_ubFlags;
        ubFlagsOff &= ~bpo.bpo_abptTextures[pDoc->m_iTexture].s.bpt_ubFlags;

        CMappingDefinitionUI mdui;
        bpo.bpo_abptTextures[pDoc->m_iTexture].bpt_mdMapping.ToUI( mdui);
        CTextureData *ptdTexture = (CTextureData *) bpo.bpo_abptTextures[pDoc->m_iTexture].bpt_toTexture.GetData();
        if( ptdTexture == NULL)
        {
          GetDlgItem( IDC_PREVIEW_FRAME)->EnableWindow( FALSE);
          m_strTextureFile = NO_TEXTURE;
          m_strTextureDim = "";
        }
        else
        {
          CTString strTexture = ptdTexture->GetName();
          if( ctPolygons == 0)
          {
        	  m_ctrlCombineColor.SetColor( bpo.bpo_abptTextures[pDoc->m_iTexture].s.bpt_colColor);
            m_strTextureFile = strTexture;
            m_strTextureDim = ptdTexture->GetDescription();
          }
          else
          {
        	  if( m_ctrlCombineColor.GetColor() != bpo.bpo_abptTextures[pDoc->m_iTexture].s.bpt_colColor)
            {
              m_ctrlCombineColor.SetMixedColor();
            }
            if( m_strTextureFile != CString( strTexture))
            {
              GetDlgItem( IDC_PREVIEW_FRAME)->EnableWindow( FALSE);
              m_strTextureFile = DIFFERENT_TEXTURE;
              m_strTextureDim = "";
            }
          }
        }
        if( ctPolygons == 0)
        {
          m_fOffsetU = mdui.mdui_fUOffset;    m_bOffsetU = TRUE;
          m_fOffsetV = mdui.mdui_fVOffset;    m_bOffsetV = TRUE;
          m_fRotationU = mdui.mdui_aURotation;m_bRotationU = TRUE;
          m_fRotationV = mdui.mdui_aVRotation;m_bRotationV = TRUE;
          m_fStretchU = mdui.mdui_fUStretch;  m_bStretchU = TRUE;
          m_fStretchV = mdui.mdui_fVStretch;  m_bStretchV = TRUE;

          ubFirstScroll = bpo.bpo_abptTextures[pDoc->m_iTexture].s.bpt_ubScroll;
          ubFirstBlend = bpo.bpo_abptTextures[pDoc->m_iTexture].s.bpt_ubBlend;
        }
        else
        {
          if( m_fOffsetU != mdui.mdui_fUOffset)    m_bOffsetU = FALSE;
          if( m_fOffsetV != mdui.mdui_fVOffset)    m_bOffsetV = FALSE;
          if( m_fRotationU != mdui.mdui_aURotation) m_bRotationU = FALSE;
          if( m_fRotationV != mdui.mdui_aVRotation) m_bRotationV = FALSE;
          if( m_fStretchU != mdui.mdui_fUStretch)  m_bStretchU = FALSE;
          if( m_fStretchV != mdui.mdui_fVStretch)  m_bStretchV = FALSE;

          if( bpo.bpo_abptTextures[pDoc->m_iTexture].s.bpt_ubScroll != ubFirstScroll) bSameScroll = FALSE;
          if( bpo.bpo_abptTextures[pDoc->m_iTexture].s.bpt_ubBlend != ubFirstBlend) bSameBlend = FALSE;
        }
        ctPolygons++;
      }

      if( bSameScroll) m_comboScroll.SetCurSel( ubFirstScroll); else m_comboScroll.SetCurSel(-1);
      if( bSameBlend) m_comboBlend.SetCurSel( ubFirstBlend); else m_comboBlend.SetCurSel(-1);
      // set states of all flags using on and off masks
      if((ubFlagsOn & BPTF_CLAMPU) && !(ubFlagsOff & BPTF_CLAMPU)) m_checkClampU.SetCheck( 1);
      else if(!(ubFlagsOn & BPTF_CLAMPU) && (ubFlagsOff & BPTF_CLAMPU)) m_checkClampU.SetCheck( 0);
      else m_checkClampU.SetCheck( 2);
      if((ubFlagsOn & BPTF_CLAMPV) && !(ubFlagsOff & BPTF_CLAMPV)) m_checkClampV.SetCheck( 1);
      else if(!(ubFlagsOn & BPTF_CLAMPV) && (ubFlagsOff & BPTF_CLAMPV)) m_checkClampV.SetCheck( 0);
      else m_checkClampV.SetCheck( 2);
      if((ubFlagsOn & BPTF_REFLECTION) && !(ubFlagsOff & BPTF_REFLECTION)) m_checkReflective.SetCheck( 1);
      else if(!(ubFlagsOn & BPTF_REFLECTION) && (ubFlagsOff & BPTF_REFLECTION)) m_checkReflective.SetCheck( 0);
      else m_checkReflective.SetCheck( 2);
      if((ubFlagsOn & BPTF_AFTERSHADOW) && !(ubFlagsOff & BPTF_AFTERSHADOW)) m_checkAfterShadow.SetCheck( 1);
      else if(!(ubFlagsOn & BPTF_AFTERSHADOW) && (ubFlagsOff & BPTF_AFTERSHADOW)) m_checkAfterShadow.SetCheck( 0);
      else m_checkAfterShadow.SetCheck( 2);

		pDoc->m_bTextureTrans[0] = m_checkTexTrans1.GetCheck();
		pDoc->m_bTextureTrans[1] = m_checkTexTrans2.GetCheck();
		pDoc->m_bTextureTrans[2] = m_checkTexTrans3.GetCheck();
    }
  }

	//{{AFX_DATA_MAP(CDlgPgTexture)
	DDX_Control(pDX, IDC_TEX_TRANS3, m_checkTexTrans3);
	DDX_Control(pDX, IDC_TEX_TRANS2, m_checkTexTrans2);
	DDX_Control(pDX, IDC_TEX_TRANS1, m_checkTexTrans1);
	DDX_Control(pDX, IDC_AFTER_SHADOW, m_checkAfterShadow);
	DDX_Control(pDX, ID_COMBINE_COLOR, m_ctrlCombineColor);
	DDX_Control(pDX, IDC_BLEND_TYPE, m_comboBlend);
	DDX_Control(pDX, IDC_REFLECTIVE, m_checkReflective);
	DDX_Control(pDX, IDC_CLAMP_V, m_checkClampV);
	DDX_Control(pDX, IDC_CLAMP_U, m_checkClampU);
	DDX_Control(pDX, IDC_SCROLL_COMBO, m_comboScroll);
	DDX_Text(pDX, IDC_TEXTURE_FILE_T, m_strTextureFile);
	DDX_Radio(pDX, IDC_RADIO_TEXTURE, m_radioTexture);
	DDX_Text(pDX, IDC_TEXTURE_DIM_T, m_strTextureDim);
	//}}AFX_DATA_MAP

  DDX_SkyFloat(pDX, IDC_OFFSET_U, m_fOffsetU, m_bOffsetU);
  DDX_SkyFloat(pDX, IDC_OFFSET_V, m_fOffsetV, m_bOffsetV);
  DDX_SkyFloat(pDX, IDC_STRETCH_U, m_fStretchU, m_bStretchU);
  DDX_SkyFloat(pDX, IDC_STRETCH_V, m_fStretchV, m_bStretchV);
  DDX_SkyFloat(pDX, IDC_ROTATION_U, m_fRotationU, m_bRotationU);
  DDX_SkyFloat(pDX, IDC_ROTATION_V, m_fRotationV, m_bRotationV);

  // if dialog is giving data and control windows are valid
  if( (pDX->m_bSaveAndValidate != FALSE) && IsWindow( m_comboScroll.m_hWnd) )
  {
    m_udTextureData.MarkUpdated();
    // paste new active texture over polygon selection
    if( (m_strTextureFile != DIFFERENT_TEXTURE) &&
        (m_strTextureFile != NO_SELECTION) )
    {
      if( m_strTextureFile == NO_TEXTURE)
      {
        pDoc->PasteTextureOverSelection_t( CTString(""));
      }
      else
      {
        pDoc->PasteTextureOverSelection_t( CTString( m_strTextureFile));
      }
    }
    // obtain masks for setting and clearing texture flags
    ULONG ulBitsToClear = MAX_ULONG;
    ULONG ulBitsToSet = 0;
    if( m_checkClampU.GetCheck() == 0) ulBitsToClear &= ~BPTF_CLAMPU;
    if( m_checkClampU.GetCheck() == 1) ulBitsToSet   |=  BPTF_CLAMPU;
    if( m_checkClampV.GetCheck() == 0) ulBitsToClear &= ~BPTF_CLAMPV;
    if( m_checkClampV.GetCheck() == 1) ulBitsToSet   |=  BPTF_CLAMPV;
    if( m_checkReflective.GetCheck()  == 0) ulBitsToClear &= ~BPTF_REFLECTION;
    if( m_checkReflective.GetCheck()  == 1) ulBitsToSet   |=  BPTF_REFLECTION;
    if( m_checkAfterShadow.GetCheck() == 0) ulBitsToClear &= ~BPTF_AFTERSHADOW;
    if( m_checkAfterShadow.GetCheck() == 1) ulBitsToSet   |=  BPTF_AFTERSHADOW;
    // for each of the selected polygons
    FOREACHINDYNAMICCONTAINER(pDoc->m_selPolygonSelection, CBrushPolygon, itbpo)
    {
      CBrushPolygon &bpo = *itbpo;
      bpo.bpo_abptTextures[pDoc->m_iTexture].s.bpt_ubFlags &= ulBitsToClear;
      bpo.bpo_abptTextures[pDoc->m_iTexture].s.bpt_ubFlags |= ulBitsToSet;

      CMappingDefinitionUI mdui;
      bpo.bpo_abptTextures[pDoc->m_iTexture].bpt_mdMapping.ToUI( mdui);
      if( m_bOffsetU) mdui.mdui_fUOffset = m_fOffsetU;
      if( m_bOffsetV) mdui.mdui_fVOffset = m_fOffsetV;
      if( m_bRotationU) mdui.mdui_aURotation = m_fRotationU;
      if( m_bRotationV) mdui.mdui_aVRotation = m_fRotationV;
      if( m_bStretchU) mdui.mdui_fUStretch = m_fStretchU;
      if( m_bStretchV) mdui.mdui_fVStretch = m_fStretchV;
      bpo.bpo_abptTextures[pDoc->m_iTexture].bpt_mdMapping.FromUI( mdui);

      INDEX iScroll = m_comboScroll.GetCurSel();
      if( iScroll != -1) bpo.bpo_abptTextures[pDoc->m_iTexture].s.bpt_ubScroll = (UBYTE)iScroll;
      INDEX iBlend = m_comboBlend.GetCurSel();
      if( iBlend != -1)  bpo.bpo_abptTextures[pDoc->m_iTexture].s.bpt_ubBlend = (UBYTE)iBlend;

      if( m_ctrlCombineColor.IsColorValid())
      {
        bpo.bpo_abptTextures[pDoc->m_iTexture].s.bpt_colColor = m_ctrlCombineColor.GetColor();
      }
    }
    pDoc->UpdateAllViews( NULL);
    pDoc->SetModifiedFlag( TRUE);
  }

  // set file name of texture for thumbnail window
  CString strWindowText;
  GetDlgItem( IDC_TEXTURE_FILE_T)->GetWindowText( strWindowText);
  m_wndViewTexture.m_strTexture = strWindowText;
  if( IsWindow( m_wndViewTexture.m_hWnd))
  {
    m_wndViewTexture.Invalidate( FALSE);
  }
}


BEGIN_MESSAGE_MAP(CDlgPgTexture, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgPgTexture)
	ON_BN_CLICKED(IDC_BROWSE_TEXTURE, OnBrowseTexture)
	ON_BN_CLICKED(IDC_RADIO_TEXTURE, OnRadioTexture)
	ON_BN_CLICKED(IDC_TEXTURE_2, OnTexture2)
	ON_BN_CLICKED(IDC_TEXTURE_3, OnTexture3)
	ON_BN_CLICKED(IDC_REMOVE_TEXTURE, OnRemoveTexture)
	ON_CBN_SELCHANGE(IDC_SCROLL_COMBO, OnSelchangeScrollCombo)
	ON_WM_DROPFILES()
	ON_CBN_SELCHANGE(IDC_BLEND_TYPE, OnSelchangeBlendType)
	ON_CBN_DROPDOWN(IDC_SCROLL_COMBO, OnDropdownScrollCombo)
	ON_CBN_DROPDOWN(IDC_BLEND_TYPE, OnDropdownBlendType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPgTexture message handlers

BOOL CDlgPgTexture::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

  // obtain document
  CWorldEditorDoc* pDoc = theApp.GetDocument();
  m_ctrlCombineColor.SetDialogPtr( this);
  m_checkReflective.SetDialogPtr(  this);
  m_checkAfterShadow.SetDialogPtr( this);
  m_checkClampV.SetDialogPtr( this);
  m_checkClampU.SetDialogPtr( this);

  m_checkTexTrans1.SetDialogPtr( this);
  m_checkTexTrans2.SetDialogPtr( this);
  m_checkTexTrans3.SetDialogPtr( this);

  PIX pixLeft = 0;
  PIX pixTop = 30;
  m_wndViewTexture.Create(NULL, NULL, WS_BORDER|WS_VISIBLE,
                          CRect( pixLeft, pixTop, pixLeft+64, pixTop+64),
                          this, IDW_SHOW_TEXTURE);
  DragAcceptFiles();
  if( pDoc == NULL)
  {
    m_radioTexture = 0;
  }
  else
  {
    m_radioTexture = pDoc->m_iTexture;
  }

  m_checkTexTrans1.SetCheck(1);

  return TRUE;
}

BOOL CDlgPgTexture::OnIdle(LONG lCount)
{
  CWorldEditorDoc* pDoc = theApp.GetDocument();
  if( (pDoc == NULL) || !IsWindow(m_hWnd)) return FALSE;

  // if polygon selection have been changed update dialog
  if( !pDoc->m_chSelections.IsUpToDate( m_udTextureData))
  {
    UpdateData( FALSE);
  }
  return TRUE;
}

void CDlgPgTexture::OnBrowseTexture()
{
  char achrDirectory[256], achrMessage[256];

//   if( OnRadioTexture() == 0)
//   {
//     strcpy( achrDirectory, "Hyper texture directory");
//     strcpy( achrMessage, "Browse hyper texture");
//   }
//   else
//   {
//     strcpy( achrDirectory, "Texture directory");
//     strcpy( achrMessage, "Browse texture");
//   }

  OnRadioTexture();
  strcpy( achrDirectory, "Hyper texture directory");
  strcpy( achrMessage, "Browse hyper texture");


  CTFileName fnTexture = _EngineGUI.BrowseTexture( CTString(""), achrDirectory, achrMessage);
  if( fnTexture != "")
  {
    GetDlgItem( IDC_TEXTURE_FILE_T)->SetWindowText( fnTexture);
  }
  // to reflect data change
	UpdateData( TRUE);
}

void CDlgPgTexture::OnRemoveTexture()
{
  GetDlgItem( IDC_TEXTURE_FILE_T)->SetWindowText( NO_TEXTURE);
  // apply data change
	UpdateData( TRUE);
}

void CDlgPgTexture::OnRadioTexture()
{
  CWorldEditorDoc* pDoc = theApp.GetDocument();
  if( pDoc == NULL)  return;
  pDoc->m_iTexture = 0;
	UpdateData( FALSE);

  m_checkTexTrans1.SetCheck(1);
  m_checkTexTrans2.SetCheck(0);
  m_checkTexTrans3.SetCheck(0);
  
  pDoc->m_bTextureTrans[0] = TRUE;
  pDoc->m_bTextureTrans[1] = FALSE;
  pDoc->m_bTextureTrans[2] = FALSE;
}

void CDlgPgTexture::OnTexture2()
{
  CWorldEditorDoc* pDoc = theApp.GetDocument();
  if( pDoc == NULL)  return;
  pDoc->m_iTexture = 1;
	UpdateData( FALSE);

  m_checkTexTrans1.SetCheck(0);
  m_checkTexTrans2.SetCheck(1);
  m_checkTexTrans3.SetCheck(0);
  
  pDoc->m_bTextureTrans[0] = FALSE;
  pDoc->m_bTextureTrans[1] = TRUE;
  pDoc->m_bTextureTrans[2] = FALSE;
}

void CDlgPgTexture::OnTexture3()
{
  CWorldEditorDoc* pDoc = theApp.GetDocument();
  if( pDoc == NULL)  return;
  pDoc->m_iTexture = 2;
	UpdateData( FALSE);

  m_checkTexTrans1.SetCheck(0);
  m_checkTexTrans2.SetCheck(0);
  m_checkTexTrans3.SetCheck(1);
  
  pDoc->m_bTextureTrans[0] = FALSE;
  pDoc->m_bTextureTrans[1] = FALSE;
  pDoc->m_bTextureTrans[2] = TRUE;
}

void CDlgPgTexture::OnSelchangeScrollCombo()
{
  UpdateData( TRUE);
}

void CDlgPgTexture::OnSelchangeBlendType()
{
  UpdateData( TRUE);
}

void CDlgPgTexture::OnDropFiles(HDROP hDropInfo)
{
  if( m_strTextureFile == NO_SELECTION)
  {
    return;
  }

  INDEX iNoOfFiles = DragQueryFile( hDropInfo, 0xFFFFFFFF, NULL, 0);

  if( iNoOfFiles != 1)
  {
    AfxMessageBox( "You can drop only one file at a time.");
    return;
  }

	// buffer for dropped file name
  char chrFile[ 256];
  // place dropped file name into buffer
  DragQueryFile( hDropInfo, 0, chrFile, 256);
  // create file name from buffer
  CTFileName fnDropped = CTString(chrFile);
  // if it is not texture, report error
  if( fnDropped.FileExt() != ".tex" )
  {
    AfxMessageBox( "You can only drop textures here.");
    return;
  }
  try
  {
    fnDropped.RemoveApplicationPath_t();
    GetDlgItem( IDC_TEXTURE_FILE_T)->SetWindowText( fnDropped);
    // apply data change
	  UpdateData( TRUE);
  }
  catch( char *err_str)
  {
    AfxMessageBox( err_str);
  }
}

BOOL CDlgPgTexture::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
  {
    UpdateData( TRUE);
    UpdateData( FALSE);
    // the message is handled
    return TRUE;
  }
	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CDlgPgTexture::OnDropdownScrollCombo()
{
  m_comboScroll.SetDroppedWidth(256);
}

void CDlgPgTexture::OnDropdownBlendType()
{
  m_comboBlend.SetDroppedWidth(256);
}
