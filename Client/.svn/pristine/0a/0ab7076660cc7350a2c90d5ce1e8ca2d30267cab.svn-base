#if !defined(AFX_DLGCSPHERE_H__F757EFB0_AB0F_462C_8985_76EBFE4986E6__INCLUDED_)
#define AFX_DLGCSPHERE_H__F757EFB0_AB0F_462C_8985_76EBFE4986E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCSphere.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCSphere dialog

class CDlgCSphere : public CDialog
{
// Construction
public:
	CDlgCSphere(CWnd* pParent = NULL);   // standard constructor

	inline void SetPropertyFrom(CSphereSpace &ss)
	{
		m_bDoubleSpace = FALSE;
		m_fX = ss.GetCenter()(1);
		m_fY = ss.GetCenter()(2);
		m_fZ = ss.GetCenter()(3);
		m_fInnerRadius = ss.GetRadius();
		m_fOuterRadius = 0;
	}
	inline void SetPropertyTo(CSphereSpace &ss)
	{
		ss.SetRadius(FLOAT3D(m_fX, m_fY, m_fZ), m_fInnerRadius);
	}

	inline void SetPropertyFrom(CSphereDoubleSpace &sds)
	{
		m_bDoubleSpace = TRUE;
		m_fX = sds.GetCenter()(1);
		m_fY = sds.GetCenter()(2);
		m_fZ = sds.GetCenter()(3);
		m_fInnerRadius = sds.GetRadiusInner();
		m_fOuterRadius = sds.GetRadiusOuter();
	}
	inline void SetPropertyTo(CSphereDoubleSpace &sds)
	{
		sds.SetRadius(FLOAT3D(m_fX, m_fY, m_fZ), m_fInnerRadius, m_fOuterRadius);
	}

// Dialog Data
	//{{AFX_DATA(CDlgCSphere)
	enum { IDD = IDD_SPHERE };
	CEdit	m_edOuter;
	CStatic	m_stOuter;
	float	m_fOuterRadius;
	float	m_fX;
	float	m_fY;
	float	m_fZ;
	float	m_fInnerRadius;
	CString	m_strInner;
	//}}AFX_DATA

	BOOL	m_bDoubleSpace;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCSphere)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCSphere)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#define DLGSPHERE(sphere) \
	CDlgCSphere dlgSphere(this);\
	dlgSphere.SetPropertyFrom(sphere);\
	if( dlgSphere.DoModal() == IDOK )\
	{\
		dlgSphere.SetPropertyTo(sphere);\
	}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCSPHERE_H__F757EFB0_AB0F_462C_8985_76EBFE4986E6__INCLUDED_)
