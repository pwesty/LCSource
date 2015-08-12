#if !defined(AFX_DLGEXPORTBSPHEIGHTMAP_H__F6375A52_0254_4901_91C9_C5C59861D6A3__INCLUDED_)
#define AFX_DLGEXPORTBSPHEIGHTMAP_H__F6375A52_0254_4901_91C9_C5C59861D6A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgExportBspHeightMap.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgExportBspHeightMap dialog

class CDlgExportBspHeightMap : public CDialog
{
// Construction
public:
	CDlgExportBspHeightMap(CWnd* pParent = NULL);   // standard constructor

	CTFileName	m_fnFileName;

// Dialog Data
	//{{AFX_DATA(CDlgExportBspHeightMap)
	enum { IDD = IDD_EXPORT_BSPHEIGHTMAP };
	CString	m_strFileName;
	int		m_nWidth;
	int		m_nHeight;
	int		m_nSelFloor;
	BOOL	m_bFieldAttrMap;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgExportBspHeightMap)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgExportBspHeightMap)
	afx_msg void OnBspheightmapOpen();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEXPORTBSPHEIGHTMAP_H__F6375A52_0254_4901_91C9_C5C59861D6A3__INCLUDED_)
