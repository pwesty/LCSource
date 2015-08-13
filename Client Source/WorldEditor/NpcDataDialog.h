#if !defined(AFX_NPCDATADIALOG_H__52918EB4_2D38_4189_A059_9A360346B6C9__INCLUDED_)
#define AFX_NPCDATADIALOG_H__52918EB4_2D38_4189_A059_9A360346B6C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NpcDataDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNpcDataDialog dialog

class CNpcDataDialog : public CDialog
{
private:	
	typedef struct __tagNPCWithEntity
	{
		inline void Clear(void) 
		{
			iNpcIndex	= -1;
			pMobData	= NULL;
		};
		__tagNPCWithEntity()
		{
			iNpcIndex	= -1;
			pEntity		= NULL;
			pMobData	= NULL;
			fPosX		= 0.0f;
			fPosZ		= 0.0f;
			fPosY		= 0.0f;
			iPosY		= 0;
			fAngle		= 0.0f;
			iRegenTime	= 0;
			iTotalNum	= -1;
		}
		
		int			iNpcIndex;		// NPC INDEX
		CMobData*	pMobData;		// Mob Data
		CEntity*	pEntity;		// 엔티티 포인터.
		float		fPosX;
		float		fPosZ;
		float		fPosY;
		int			iPosY;
		float		fAngle;			// Angle
		int			iRegenTime;
		int			iTotalNum;
	} NPCWithEntity;
	
	CDynamicStackArray<struct __tagNPCWithEntity>	m_paNPCWithEntity;
	
	//----------------------------------------------------
	// 엔티티 처리와 관련된 작업.
	//----------------------------------------------------
	BOOL			InsertEntityList(int iNpcIndex, int iWorldNum, CEntity* pEnt, int YLayer = -1, int iRegenTime = 1800, int iTotalNum = -1);	

	BOOL			ScanEntities();	
	void			DestroyEntities();

	CEntity*		CreateEntityByYLayer(CMobData& MD, float XPos, int YLayer, float YPos, float ZPos, float fAngle, int iRegenTime, int iTotalNum);

	//----------------------------------------------------
	// DB의 레코드 처리하는 부분.
	//----------------------------------------------------
	void			InsertNpcData(CEntity* pEnt, int iWorldNum);
	void			RefreshNpcData(int iWorldNum, int iNpcIndex = -1, int iNpcType = 0, int iYLayer = -1);
	void			RefreshShopData(int iWorldNum, int iNpcIndex = -1, int iNpcType = 0, int iYLayer = -1);
	BOOL			DeleteNpcData(int iWorldNum, int iNpcIndex = -1, int iNpcType = 0, int iYLayer = -1);	
	BOOL			DeleteShopData(int iWorldNum, int iNpcIndex = -1, int iNpcType = 0, int iYLayer = -1);

public:
	void			BackupData();
	void			RestoreData();

// Construction
public:	
	CNpcDataDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNpcDataDialog)
	enum { IDD = IDD_NPC_DIALOG };
	CComboBox	m_ctlcbFindByYLayer;
	CComboBox	m_ctlcbFindByName;
	CComboBox	m_ctlcbFindByType;
	CListCtrl	m_ctlNpcGrid;
	BOOL		m_bShowEnemyModel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNpcDataDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNpcDataDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeServerlist();
	afx_msg void OnNpcDataDelete();
	afx_msg void OnDestroy();	
	afx_msg void OnEnemyUpdate();
	virtual void OnOK();
	afx_msg void OnNpcGenerate();	
	afx_msg void OnSelchangeFindByYlayer();
	afx_msg void OnSelchangeFindByName();
	afx_msg void OnSelchangeFindByType();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnImportDbData();
	afx_msg void OnExportDbData();
	afx_msg void OnEnemyEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NPCDATADIALOG_H__52918EB4_2D38_4189_A059_9A360346B6C9__INCLUDED_)
