// DlgProperties.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "DlgProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgProperties dialog


CDlgProperties::CDlgProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProperties::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProperties)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProperties)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP


  // if dialog is giving data
  if( pDX->m_bSaveAndValidate) {
    CComboBox *cbGapi = ((CComboBox*)GetDlgItem(IDC_TB_GAPI));
    CTextBox *tbFov = ((CTextBox*)GetDlgItem(IDC_TB_FOV));

    ASSERT(cbGapi!=NULL);
    ASSERT(tbFov!=NULL);
    INDEX iApiIndex = cbGapi->GetCurSel();
    const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
    GfxAPIType eNewAPI;
    if(iApiIndex==0) {
      eNewAPI = GAT_OGL;
    } else if(iApiIndex==1) {
      eNewAPI = GAT_D3D;
    }
    if(eNewAPI!=eAPI) {
      _pGfx->ResetDisplayMode(eNewAPI);
    }
    char strFOV[64];
    tbFov->GetWindowText(strFOV,64);
    theApp.m_fFOV = atof(strFOV);
    if(theApp.m_fFOV==0.0f) {
//안태훈 수정 시작	//(Easy Use Ska Studio)(0.1)
		theApp.m_fFOV = 60.0f;
//안태훈 수정 끝	//(Easy Use Ska Studio)(0.1)
    }
  }
}

BOOL CDlgProperties::OnInitDialog() 
{
  CDialog::OnInitDialog();
  CComboBox *cbGapi = ((CComboBox*)GetDlgItem(IDC_TB_GAPI));
  CTextBox *tbFov = ((CTextBox*)GetDlgItem(IDC_TB_FOV));
  ASSERT(cbGapi!=NULL);
  ASSERT(tbFov!=NULL);

  const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
  ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);

  cbGapi->ResetContent();
  cbGapi->AddString("OpenGL");
  cbGapi->AddString("DirectX");
  if(eAPI==GAT_OGL) {
    cbGapi->SetCurSel(0);
  } else if(eAPI==GAT_D3D) {
    cbGapi->SetCurSel(1);
  }

  // FOV
  CTString strFov(0,"%g",theApp.m_fFOV);
  tbFov->SetWindowText((const char*)strFov);
  return TRUE;
}


BEGIN_MESSAGE_MAP(CDlgProperties, CDialog)
	//{{AFX_MSG_MAP(CDlgProperties)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProperties message handlers
