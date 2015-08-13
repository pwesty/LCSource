#if !defined(AFX_MERCHANTDIALOG_H__2AF86374_52BA_4D51_B9B8_763B2B58B8B0__INCLUDED_)
#define AFX_MERCHANTDIALOG_H__2AF86374_52BA_4D51_B9B8_763B2B58B8B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MerchantDialog.h : header file
//

#include <afxtempl.h>
#include <Engine/GlobalDefinition.h>
#include <vector>

/////////////////////////////////////////////////////////////////////////////
// CMerchantDialog dialog


class CMerchantDialog : public CDialog
{
// Construction
public:
	CMerchantDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMerchantDialog)
	enum { IDD = IDD_MERCHANT_DATA_DIALOG };
	CComboBox	m_ctlFindByType;
	CComboBox	m_ctlFindBySubType;
	CListCtrl	m_ctlItemListOfNPC;
	CListCtrl	m_ctlItemList;
	CString	m_strShopperName;
	BOOL	m_bHealer;
	BOOL	m_bKnight;
	BOOL	m_bMage;
	BOOL	m_bRogue;
	BOOL	m_bTitan;
	BOOL	m_bSorcerer;
	int		m_iBuyRate;
	int		m_iSellRate;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMerchantDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMerchantDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemToNpc();
	afx_msg void OnItemToNpcAll();
	afx_msg void OnItemFromNpc();
	afx_msg void OnItemFromNpcAll();
	afx_msg void OnSelchangeFindByType();
	afx_msg void OnSelchangeFindBySubtype();
	afx_msg void OnSelchangeFindByRace();
	afx_msg void OnMerchantUpdate();
	afx_msg void OnMerchantRefresh();
	afx_msg void OnSelchangeMerchantTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingMerchantTab(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnMerchantMakefile();
	afx_msg void OnTitan();
	afx_msg void OnKnight();
	afx_msg void OnHealer();
	afx_msg void OnMage();
	afx_msg void OnRogue();
	afx_msg void OnSorcerer();		
	afx_msg void OnItemchangedItemListSell(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnCheck2();
	afx_msg void OnChangeCountry();
	afx_msg void OnCopyCountry();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	
	typedef struct _tagSellItems
	{
		_tagSellItems()
		{
			iIndex		= -1;
			lNational	= 0;
		};
		int		iIndex;
		ULONG	lNational;
	}SellItems;

	// Function Object
	struct FindItems
	{
		int iIndex;
		FindItems(SellItems& item) : iIndex(item.iIndex) {};
		bool operator() (const SellItems& item)
		{
			if(item.iIndex == iIndex)
			{
				return true;
			}
			else
				return false;
		}
	};

	void	RefreshItemList(CListCtrl& listctrl, BOOL bTotal, int iZoneFlag = ZONEALL, int iItemIdx = -1, int iNational = 0, int iType = -1, int iSubType = -1, int iRace = -1);
	void	RefreshItemListOfNPC(int iMerchantIdx);
	BOOL	UpdateItemListOfNPC(int iMerchantIdx, int iItemIdx, int iNational);
	BOOL	DeleteItemListOfNPC(int iMerchantIdx);
	BOOL	ShowWindowTabs(CPtrList& ptrList, BOOL bShow = FALSE);

	void	SetNationalFlag();
	void	GetNationalFlag();

	BOOL	SetTradeRate(int iSellRate, int iBuyRate);	
	void	GetTradeRate();

	std::vector<SellItems>	m_vectorSellItems;
	//std::vector<int>	m_vectorTotalItems;
public:
	int		m_iSelectedShopper;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MERCHANTDIALOG_H__2AF86374_52BA_4D51_B9B8_763B2B58B8B0__INCLUDED_)
