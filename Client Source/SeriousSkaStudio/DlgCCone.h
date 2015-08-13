#if !defined(AFX_DLGCCONE_H__8F32702F_C8DC_475C_8046_8F5238548565__INCLUDED_)
#define AFX_DLGCCONE_H__8F32702F_C8DC_475C_8046_8F5238548565__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCCone.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCCone dialog

class CDlgCCone : public CDialog
{
// Construction
public:
	CDlgCCone(CWnd* pParent = NULL);   // standard constructor

	inline void SetPropertyFrom(CConeSpace &cs)
	{
		m_bDoubleSpace = FALSE;
		m_fX = cs.GetCenter()(1);
		m_fY = cs.GetCenter()(2);
		m_fZ = cs.GetCenter()(3);
		m_fHotspot = cs.GetAngle();
		m_fFalloff = 0;
		m_fHeightUpper = cs.GetHeightUpper();
		m_fHeightLower = cs.GetHeightLower();
		m_fCloseRange = cs.GetCloseRange();
		m_fLerpRatio = cs.GetLerpRatio();
	}
	inline void SetPropertyTo(CConeSpace &cs)
	{
		cs.SetCenter(FLOAT3D(m_fX, m_fY, m_fZ));
		cs.SetAngle(m_fHotspot);
		cs.SetHeight(m_fHeightUpper, m_fHeightLower);
		cs.SetGroupValue(m_fCloseRange, m_fLerpRatio);
	}

	inline void SetPropertyFrom(CConeDoubleSpace &cds)
	{
		m_bDoubleSpace = TRUE;
		m_fX = cds.GetCenter()(1);
		m_fY = cds.GetCenter()(2);
		m_fZ = cds.GetCenter()(3);
		m_fHotspot = cds.GetHotspot();
		m_fFalloff = cds.GetFalloff();
		m_fHeightUpper = cds.GetHeightUpper();
		m_fHeightLower = cds.GetHeightLower();
		m_fCloseRange = cds.GetCloseRange();
		m_fLerpRatio = cds.GetLerpRatio();
	}
	inline void SetPropertyTo(CConeDoubleSpace &cds)
	{
		cds.SetCenter(FLOAT3D(m_fX, m_fY, m_fZ));
		cds.SetAngle(m_fHotspot, m_fFalloff);
		cds.SetHeight(m_fHeightUpper, m_fHeightLower);
		cds.SetGroupValue(m_fCloseRange, m_fLerpRatio);
	}
// Dialog Data
	//{{AFX_DATA(CDlgCCone)
	enum { IDD = IDD_CONE };
	CEdit	m_edFalloff;
	CStatic	m_stFalloff;
	float	m_fX;
	float	m_fY;
	float	m_fZ;
	CString	m_strHotspot;
	float	m_fHotspot;
	float	m_fFalloff;
	float	m_fHeightLower;
	float	m_fHeightUpper;
	float	m_fLerpRatio;
	float	m_fCloseRange;
	//}}AFX_DATA

	BOOL	m_bDoubleSpace;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCCone)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCCone)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#define DLGCONE(cone) \
	CDlgCCone dlg(this);\
	dlg.SetPropertyFrom(cone);\
	if( dlg.DoModal() == IDOK )\
	{\
		dlg.SetPropertyTo(cone);\
	}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCCONE_H__8F32702F_C8DC_475C_8046_8F5238548565__INCLUDED_)
