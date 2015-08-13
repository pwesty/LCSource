#if !defined(AFX_ZONEFLAGDIALOG_H__A2E45207_DB5F_4F22_8C59_B3E00A5EED8E__INCLUDED_)
#define AFX_ZONEFLAGDIALOG_H__A2E45207_DB5F_4F22_8C59_B3E00A5EED8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZoneFlagDialog.h : header file
//

//#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CZoneFlagDialog dialog


class CZoneFlagDialog : public CDialog
{
// Construction
public:
	CZoneFlagDialog(CWnd* pParent = NULL);   // standard constructor
	ZONEFLAGS	GetZoneFlag();
	void	SetZoneFlag(ZONEFLAGS ulZoneFlag);
	ZONEFLAGS	GetExtraFlag();
	void	SetExtraFlag(ZONEFLAGS ulExtraFlag);

// Dialog Data
	//{{AFX_DATA(CZoneFlagDialog)
	enum { IDD = IDD_ZONE_FLAG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZoneFlagDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	ZONEFLAGS		m_ulZoneFlag;		// Zone Moving Flag
	ZONEFLAGS		m_ulExtraFlag;		// Zone Extra Flag

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CZoneFlagDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZONEFLAGDIALOG_H__A2E45207_DB5F_4F22_8C59_B3E00A5EED8E__INCLUDED_)
