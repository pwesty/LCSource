#if !defined(AFX_DLGTERRAINSOUND_H__8B8FBA74_0724_4C65_A084_65957DDF3DBB__INCLUDED_)
#define AFX_DLGTERRAINSOUND_H__8B8FBA74_0724_4C65_A084_65957DDF3DBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTerrainSound.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTerrainSound dialog

class CDlgTerrainSound : public CDialog
{
// Construction
public:
	int GetSoundIndex() { return m_nSoundIndex; }
	CDlgTerrainSound(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTerrainSound)
	enum { IDD = IDD_TERRAIN_SOUND };
	CComboBox	m_ctrlSurfaceType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTerrainSound)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nSoundIndex;
	// Generated message map functions
	//{{AFX_MSG(CDlgTerrainSound)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTERRAINSOUND_H__8B8FBA74_0724_4C65_A084_65957DDF3DBB__INCLUDED_)
