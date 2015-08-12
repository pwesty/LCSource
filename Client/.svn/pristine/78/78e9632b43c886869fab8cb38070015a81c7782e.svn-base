#include "StdH.h"
#include "resource.h"
#include "DlgRecursiveFileBrowser.h"

#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDlgRecursiveFileBrowser dialog


CDlgRecursiveFileBrowser::CDlgRecursiveFileBrowser(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRecursiveFileBrowser::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRecursiveFileBrowser)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgRecursiveFileBrowser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRecursiveFileBrowser)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

void CDlgRecursiveFileBrowser::Update(void)
{
  UpdateData(FALSE);
}

void CDlgRecursiveFileBrowser::SetMessage(INDEX iControlID, const char *strFormat, ...)
{
  CWnd *pCtrl = GetDlgItem(iControlID);
  ASSERT(pCtrl!=NULL);
  va_list arg;
  va_start(arg, strFormat);
  CTString strMessage;
  strMessage.VPrintF(strFormat, arg);

  pCtrl->SetWindowText((const char*)strMessage);
}



BEGIN_MESSAGE_MAP(CDlgRecursiveFileBrowser, CDialog)
	//{{AFX_MSG_MAP(CDlgRecursiveFileBrowser)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRecursiveFileBrowser message handlers

void CDlgRecursiveFileBrowser::OnClose() 
{
  // Cancel conversion
  extern BOOL _bCanceled;
  _bCanceled = TRUE;
	CDialog::OnClose();
}
