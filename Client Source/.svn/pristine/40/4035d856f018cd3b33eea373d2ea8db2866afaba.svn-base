// PersistentSymbolToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PersistentSymbolTool.h"
#include "PersistentSymbolToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPersistentSymbolToolDlg dialog

CPersistentSymbolToolDlg::CPersistentSymbolToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPersistentSymbolToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPersistentSymbolToolDlg)
	m_strStatus = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPersistentSymbolToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPersistentSymbolToolDlg)
	DDX_Text(pDX, IDC_EDIT_STATUS, m_strStatus);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPersistentSymbolToolDlg, CDialog)
	//{{AFX_MSG_MAP(CPersistentSymbolToolDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ENCODE, OnButtonEncode)
	ON_BN_CLICKED(IDC_BUTTON_DECODE, OnButtonDecode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPersistentSymbolToolDlg message handlers

BOOL CPersistentSymbolToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPersistentSymbolToolDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPersistentSymbolToolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

#include <Engine/Base/PersistentSymbolsEncode.h>
void CPersistentSymbolToolDlg::EncodeFile(const char* srcFile, const char* dstFile)
{
	FILE *fp = fopen(srcFile, "rb");
	unsigned char *bufSrc = new unsigned char[1024*1024];
	memset(bufSrc, 0, 1024*1024);
	fread(bufSrc, 1024*1024, 1, fp);
	int len = strlen((char*)bufSrc);
	fclose(fp);
	unsigned char *bufDst = new unsigned char[len];
	memset(bufDst, 0, len);
	
	EncodePersistentSymbols(bufDst, 0, bufSrc, len);
	
	fp = fopen(dstFile, "wb");
	fwrite(&len, 4, 1, fp);
	fwrite(bufDst, len, 1, fp);
	fclose(fp);
	delete[] bufSrc;
	delete[] bufDst;
}

void CPersistentSymbolToolDlg::OnButtonEncode() 
{
	CFileDialog dlgFileOpen(TRUE, NULL, NULL, OFN_HIDEREADONLY, NULL);
	if(dlgFileOpen.DoModal() == IDOK)
	{
		CFileDialog dlgFileSave(FALSE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, NULL);
		if(dlgFileSave.DoModal() == IDOK)
		{
			try
			{
#if 1
				EncodeFile(dlgFileOpen.GetPathName().GetBuffer(0), dlgFileSave.GetPathName().GetBuffer(0));
#else
				FILE *fp = fopen(dlgFileOpen.GetPathName().GetBuffer(0), "rb");
				unsigned char *bufSrc = new unsigned char[1024*1024];
				memset(bufSrc, 0, 1024*1024);
				fread(bufSrc, 1024*1024, 1, fp);
				int len = strlen((char*)bufSrc);
				fclose(fp);
				unsigned char *bufDst = new unsigned char[len];
				memset(bufDst, 0, len);

				EncodePersistentSymbols(bufDst, bufSrc, len);
				
				fp = fopen(dlgFileSave.GetPathName().GetBuffer(0), "wb");
				fwrite(&len, 4, 1, fp);
				fwrite(bufDst, len, 1, fp);
				fclose(fp);
				delete[] bufSrc;
				delete[] bufDst;
#endif
			}
			catch(...)
			{
				m_strStatus = "Somthing is wrong.";
				UpdateData(FALSE);
			}
		}
	}
}

void CPersistentSymbolToolDlg::OnButtonDecode() 
{
	CFileDialog dlgFileOpen(TRUE, NULL, NULL, OFN_HIDEREADONLY, NULL);
	if(dlgFileOpen.DoModal() == IDOK)
	{
		CFileDialog dlgFileSave(FALSE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, NULL);
		if(dlgFileSave.DoModal() == IDOK)
		{
			try
			{
				FILE *fp = fopen(dlgFileOpen.GetPathName().GetBuffer(0), "rb");
				int len = 0;
				fread(&len, 4, 1, fp);
				unsigned char *bufSrc = new unsigned char[len];
				fread(bufSrc, len, 1, fp);
				fclose(fp);
				char *bufDst = new char[len + 1024];
				memset(bufDst, 0, len+1024);

				DecodePersistentSymbols((unsigned char*)bufDst, 0, bufSrc, len);
				
				fp = fopen(dlgFileSave.GetPathName().GetBuffer(0), "w");
				fwrite(bufDst, len, 1, fp);
				fclose(fp);
				delete[] bufSrc;
				delete[] bufDst;
			}
			catch(...)
			{
				m_strStatus = "Somthing is wrong.";
				UpdateData(FALSE);
			}
		}
	}
}
