#ifndef	UICHANGEEQUIPMENT_H_
#define	UICHANGEEQUIPMENT_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

enum eChangeEquipState
{
	CHANGEEQUIP_WEAPON = 0,
	CHANGEEQUIP_ARMOR,
	CHANGEEQUIP_MAX
};

enum eGUIDE_STATE
{
	eGUIDE_REGISTER_TRADE_ITEM = 0,
	eGUIDE_TRADE_FOR_NEED_ITEM,
	eGUIDE_ALL_READY,
	eGUIDE_MAX
};

class CUIChangeEquipment : public CUIWindow
{
public:
	CUIChangeEquipment();

	void	initialize();
	
	void	open(eChangeEquipState eType, float x, float z);
	void	close();

	void	UpdateUI();
	void	OnUpdate(float fDeltaTime, ULONG ElapsedTime);

	// Command functions
	void	PressOK();
	void	ClickListItem();
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	ShowSystemMsg(int nErrorCode);

	// Msg Proc
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

	BOOL	CloseWindowByEsc()	{ close(); return TRUE; }

private:
	// UI ¼ÂÆÃ
	void	clearIcon(int nType);
	void	updateGuide(eGUIDE_STATE eType, int nCount = 0);
	bool	refreshString();
	bool	refreshListItem();
	bool	refreshListInfo();
	bool	checkValidItem(int nItemIdx);
	int		getSelectIdx(int nSelIdx);
	int		getConsumeTokenCnt(int nItemIdx);

	void	SendChangeEquipReq();

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
		eTEXT_LISTNAME,
		eTEXT_MAX
	};

	enum
	{
		eLIST_ITEM = 0,
		eLIST_INFO,
		eLIST_MAX
	};

	enum
	{
		eICON_ITEM = 0,
		eICON_SUB,
		eICON_MAX
	};

	FLOAT		m_fNpcX, m_fNpcZ;
	bool		m_bDrag;
	int			m_nOriX, m_nOriY;
		
	eChangeEquipState	m_eChangeEquipState;
	eGUIDE_STATE		m_eGuideState;
	int			m_nGoldTokenCount;

	CUIButton*	m_pbtn[eBUTTON_MAX];
	CUIText*	m_pText[eTEXT_MAX];
	CUIList*	m_plist[eLIST_MAX];
	CUIIcon*	m_pIcon[eICON_MAX];
	CUITextBox* m_ptbContent;
	CUIScrollBar* m_psbList;
};


#endif	// UICHANGEEQUIPMENT_H_

