#ifndef	UICHANGEWEAPON_H_
#define	UICHANGEWEAPON_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

enum eChangeWeaponState
{
	CHANGEWEAPON_WEAPON = 0,
	CHANGEWEAPON_SHIELD,
	CHANGEWEAPON_MAX
};

class CUIChangeWeapon : public CUIWindow
{
public:
	CUIChangeWeapon();

	void	initialize();

	ENGINE_API	void	OpenChangeWeapon(int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ );
	void	open(eChangeWeaponState eType);
	void	close();

	void	UpdateUI();
	void	OnUpdate(float fDeltaTime, ULONG ElapsedTime);
		
	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Command functions
	void	PressOK();
	void	ClickListItem();
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );

	// Cash Item Use Check wooss 051011
	void	SetCashItem(BOOL bChk)	{	m_bCashItemChk = bChk;	} 
	BOOL	GetCashItem()			{	return m_bCashItemChk;	}

	// Msg Proc
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

	BOOL	CloseWindowByEsc()	{ close(); return TRUE; }
private:
	SQUAD	CalculateNeedNas( int iWeaponLevel );
	SQUAD	CalculateNeedNasS( int iWeaponLevel );
	void	SendChangeWeaponReq();
	void	ShowSystemMsg(int errorType);
	// UI ¼ÂÆÃ
	void	clearIcon();
	void	updateNas();
	bool	refreshString();
	bool	refreshListItem();
	bool	refreshListInfo();
	bool	checkValidItem(int nItemIdx);
	int		getSelectIdx(int nSelIdx);

	enum
	{
		eBUTTON_CLOSE = 0,
		eBUTTON_OK,
		eBUTTON_NO,
		eBUTTON_MAX
	};

	enum
	{
		eTEXT_MOVE = 0,
		eTEXT_TITLE,
		eTEXT_NAS,
		eTEXT_LISTNAME,
		eTEXT_MAX
	};

	enum
	{
		eLIST_ITEM = 0,
		eLIST_INFO,
		eLIST_MAX
	};

	FLOAT		m_fNpcX, m_fNpcZ;
	bool		m_bDrag;
	int			m_nOriX, m_nOriY;
	BOOL		m_bCashItemChk;						// item check
	__int64		m_llNas;
	eChangeWeaponState	m_eChangeWeaponState;

	CUIButton*	m_pbtn[eBUTTON_MAX];
	CUIText*	m_pText[eTEXT_MAX];
	CUIList*	m_plist[eLIST_MAX];
	CUIIcon*	m_piconItem;
	CUIScrollBar* m_psbList;
};

#endif	// UICHANGEWEAPON_H_

