// PersistentSymbolToolDlg.h : header file
//

#if !defined(AFX_PERSISTENTSYMBOLTOOLDLG_H__68FAE505_1BA4_429C_8F06_587DA7610F9A__INCLUDED_)
#define AFX_PERSISTENTSYMBOLTOOLDLG_H__68FAE505_1BA4_429C_8F06_587DA7610F9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPersistentSymbolToolDlg dialog

class CPersistentSymbolToolDlg : public CDialog
{
// Construction
public:
	CPersistentSymbolToolDlg(CWnd* pParent = NULL);	// standard constructor

	static void EncodeFile(const char* srcFile, const char* dstFile);

// Dialog Data
	//{{AFX_DATA(CPersistentSymbolToolDlg)
	enum { IDD = IDD_PERSISTENTSYMBOLTOOL_DIALOG };
	CString	m_strStatus;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPersistentSymbolToolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPersistentSymbolToolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonEncode();
	afx_msg void OnButtonDecode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PERSISTENTSYMBOLTOOLDLG_H__68FAE505_1BA4_429C_8F06_587DA7610F9A__INCLUDED_)
