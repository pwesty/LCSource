#if !defined(AFX_DLGEXPORTBSPATTRIBUTEMAP_H__4DDCCB35_D4F5_4C10_984F_2B1448ABCA7C__INCLUDED_)
#define AFX_DLGEXPORTBSPATTRIBUTEMAP_H__4DDCCB35_D4F5_4C10_984F_2B1448ABCA7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgExportBspAttributeMap.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgExportBspAttributeMap dialog
#include <vector>

class CDlgExportBspAttributeMap : public CDialog
{
// Construction
public:
	CDlgExportBspAttributeMap(CWnd* pParent = NULL);   // standard constructor

	CTFileName	m_fnFileName;

// Dialog Data
	//{{AFX_DATA(CDlgExportBspAttributeMap)
	enum { IDD = IDD_EXPORT_BSPATTRIBUTEMAP };
	int		m_nHeight;
	int		m_nWidth;
	int		m_nSelFloor;
	CString	m_strFileName;
	BOOL	m_bFieldAttrMap;	
	//}}AFX_DATA
	std::vector<int>	m_vectorPriority;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgExportBspAttributeMap)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgExportBspAttributeMap)
	afx_msg void OnBspattributemapOpen();
	virtual void OnOK();
	afx_msg void OnButtonPriority();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEXPORTBSPATTRIBUTEMAP_H__4DDCCB35_D4F5_4C10_984F_2B1448ABCA7C__INCLUDED_)
