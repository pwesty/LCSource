#if !defined(AFX_DLGCPPFORCEPROCESSPROPERTY_H__04F2740E_9DA2_4667_B826_1FDDDE322722__INCLUDED_)
#define AFX_DLGCPPFORCEPROCESSPROPERTY_H__04F2740E_9DA2_4667_B826_1FDDDE322722__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCPPForceProcessProperty.h : header file
//
#include <vector>
#include <Engine/Effect/CForce.h>
#include <Engine/Effect/CParticlesProcessForce.h>

/////////////////////////////////////////////////////////////////////////////
// CDlgCPPForceProcessProperty dialog
inline CString GetForceName(CForce &force)
{
	switch(force.GetType())
	{
	case FT_ZERO:					return "Zero";
	case FT_DIRECTION:				return "Direction";
	case FT_POINT:					return "Point";
	case FT_GRAVITY_POINT:			return "GravityPoint";
	case FT_INVERSE_GRAVITY_POINT:	return "InverseGravityPoint";
	}
	return "None";
}

class CDlgCPPForceProcessProperty : public CDialog
{
// Construction
public:
	CDlgCPPForceProcessProperty(CWnd* pParent = NULL);   // standard constructor

	void Clear();
	void Refresh();

	CParticlesProcessForce GetParticlesProcess()
	{
		CParticlesProcessForce ppf;
		for(int i=0; i<m_vectorForce.size(); ++i)
		{
			ppf.AddForce(m_vectorForce[i]);
		}
		UpdateData(SAVETOOBJECT);
		return ppf;
	}
	void SetParticlesProcess(CParticlesProcessForce &ppf)
	{
		m_vectorForce.clear();
		m_lbAddedForce.ResetContent();
		for(int i=0; i<ppf.GetForceCount(); ++i)
		{
			m_vectorForce.push_back(ppf.GetForce(i));
			m_lbAddedForce.AddString( GetForceName( const_cast<CForce&>(ppf.GetForce(i)) ) );
		}
		UpdateData(SAVETOFORM);
	}

// Dialog Data
	//{{AFX_DATA(CDlgCPPForceProcessProperty)
	enum { IDD = IDD_CPP_FORCE_PROCESS_PROPERTY };
	CListBox	m_lbAddedForce;
	//}}AFX_DATA

	std::vector<CForce> m_vectorForce;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCPPForceProcessProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCPPForceProcessProperty)
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnDel();
	afx_msg void OnBtnModify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCPPFORCEPROCESSPROPERTY_H__04F2740E_9DA2_4667_B826_1FDDDE322722__INCLUDED_)
