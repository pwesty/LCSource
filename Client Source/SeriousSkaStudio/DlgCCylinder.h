#if !defined(AFX_DLGCCYLINDER_H__871D7CFD_C6D2_414E_AB60_96EBCF0E6CCF__INCLUDED_)
#define AFX_DLGCCYLINDER_H__871D7CFD_C6D2_414E_AB60_96EBCF0E6CCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCCylinder.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCCylinder dialog

class CDlgCCylinder : public CDialog
{
// Construction
public:
	CDlgCCylinder(CWnd* pParent = NULL);   // standard constructor

	inline void SetPropertyFrom(CCylinderSpace &cs)
	{
		m_bDoubleSpace = FALSE;
		m_fX = cs.GetCenter()(1);
		m_fY = cs.GetCenter()(2);
		m_fZ = cs.GetCenter()(3);
		m_fInner = cs.GetRadius();
		m_fOuter = 0;
		m_fHeight = cs.GetHeight();
	}
	inline void SetPropertyTo(CCylinderSpace &cs)
	{
		cs.SetCenter(FLOAT3D(m_fX, m_fY, m_fZ));
		cs.SetRadius(m_fInner);
		cs.SetHeight(m_fHeight);
	}

	inline void SetPropertyFrom(CCylinderDoubleSpace &cds)
	{
		m_bDoubleSpace = TRUE;
		m_fX = cds.GetCenter()(1);
		m_fY = cds.GetCenter()(2);
		m_fZ = cds.GetCenter()(3);
		m_fInner = cds.GetRadiusInner();
		m_fOuter = cds.GetRadiusOuter();
		m_fHeight = cds.GetHeight();
	}
	inline void SetPropertyTo(CCylinderDoubleSpace &cds)
	{
		cds.SetCenter(FLOAT3D(m_fX, m_fY, m_fZ));
		cds.SetRadius(m_fInner, m_fOuter);
		cds.SetHeight(m_fHeight);
	}
// Dialog Data
	//{{AFX_DATA(CDlgCCylinder)
	enum { IDD = IDD_CYLINDER };
	CStatic	m_stOuter;
	CEdit	m_edOuter;
	float	m_fHeight;
	float	m_fX;
	float	m_fY;
	float	m_fZ;
	CString	m_strInner;
	float	m_fInner;
	float	m_fOuter;
	//}}AFX_DATA

	BOOL	m_bDoubleSpace;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCCylinder)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCCylinder)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#define DLGCYLINDER(cyl) \
	CDlgCCylinder dlg(this);\
	dlg.SetPropertyFrom(cyl);\
	if( dlg.DoModal() == IDOK )\
	{\
		dlg.SetPropertyTo(cyl);\
	}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCCYLINDER_H__871D7CFD_C6D2_414E_AB60_96EBCF0E6CCF__INCLUDED_)
