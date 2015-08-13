#ifndef UI_LCE_SYSTEM_ITEM_LIST_H_
#define UI_LCE_SYSTEM_ITEM_LIST_H_
#ifdef	PRAGMA_ONCE
#pragma once
#endif

enum eLCE_ITEM_STRING
{
	LCE_STRING_NAME = 0,		// 아이템 이름
	LCE_STRING_NUM,				// 아이템일때 갯수
	LCE_STRING_AMOUNT,			// 나스일때 갯수
	LCE_STRING_SENDER,			// 보낸 사람
	LCE_STRING_TYPE,			// 등록 타입
	LCE_STRING_DATE_START,		// 등록 시간
	LCE_STRING_DATE_END,		// 등록 만료 시간
	LCE_STRING_MAX,
};

enum eLCE_ITEM_STRING_COLOR
{
	LCE_COLOR_NAME = 0,			// 아이템 이름
	LCE_COLOR_NUM,				// 아이템일때 갯수
	LCE_COLOR_AMOUNT,			// 나스일때 갯수
	LCE_COLOR_SENDER,			// 보낸 사람
	LCE_COLOR_TYPE,				// 등록 타입
	LCE_COLOR_DATE,			// 등록 시간
	LCE_COLOR_MAX,				
};

enum eLCE_Item_List_UI_Info
{
	COLUM_X_GAP = 3,
	COLUM_START_NAME_GAP	= BTN_SIZE + COLUM_X_GAP,
	CULUM_X_WIDTH_ITME_NAME = 258,
	CULUM_X_WIDTH_QUANTITY	= 51,
	CULUM_X_WIDTH_AMOUNT	= 90,
	CULUM_X_WIDTH_SENDER	= 100,
	CULUM_X_WIDTH_TYPE		= 79,
	CULUM_X_WIDTH_TIME		= 121,
	LCE_UI_ITEMLIST_COLUM_ITEM_NAME		= 0,
	LCE_UI_ITEMLIST_COLUM_QUANTITY		= LCE_UI_ITEMLIST_COLUM_ITEM_NAME + COLUM_X_GAP + CULUM_X_WIDTH_ITME_NAME,
	LCE_UI_ITEMLIST_COLUM_AMOUNT		= LCE_UI_ITEMLIST_COLUM_QUANTITY + COLUM_X_GAP + CULUM_X_WIDTH_QUANTITY,
	LCE_UI_ITEMLIST_COLUM_SENDER		= LCE_UI_ITEMLIST_COLUM_AMOUNT + COLUM_X_GAP + CULUM_X_WIDTH_AMOUNT,
	LCE_UI_ITEMLIST_COLUM_TYPE			= LCE_UI_ITEMLIST_COLUM_SENDER + COLUM_X_GAP + CULUM_X_WIDTH_SENDER,
	LCE_UI_ITEMLIST_COLUM_TIME			= LCE_UI_ITEMLIST_COLUM_TYPE + COLUM_X_GAP + CULUM_X_WIDTH_TYPE,
};

class CUIExpressItemList : public CUIWindow
{
public:
	CUIExpressItemList();
	~CUIExpressItemList();
	//----------------------------------------------------------------
	void		SetString( eLCE_ITEM_STRING type, CTString strString );
	void		SetStringColor( eLCE_ITEM_STRING_COLOR type, COLOR color );

	int			GetBtnPosX()	{ return m_pIcon->GetPosX(); }
	int			GetBtnPosY()	{ return m_pIcon->GetPosY(); }
	CUIIcon*	GetBtn()		{ return m_pIcon; }
	// 나스 아이템으로 셋팅
	void		SetNasItem(bool bNas) { m_bNas = bNas; }
		
	// 아이템 리스트 리셋
	void		InitItemList();

	//-----------------------------------------------------------------
	void		Create( CUIWindow* pParentWnd, int nX, int nY, int nWidth, int nHeight );

	void		Render();

	// Msg Proc
	WMSG_RESULT MouseMessage(MSG* pMsg);

private:
	// 스트링 리셋
	void			_ListStringInit();
	// 스트링 컬러 리셋
	void			_ListColorInit();
	// 스트링 위치 리셋
	void			_StringPosInit();

	CUIIcon*		m_pIcon;		// 익스프레스 아이템
	CTString		m_strString[LCE_STRING_MAX];
	COLOR			m_color[LCE_COLOR_MAX];

	//------------------------------------------------------------------
	// 스트링 위치
	int				m_nItemNamePosX;
	int				m_nItemNamePosY;
	int				m_nItemNumPosX;
	int				m_nItemNumPosY;
	int				m_nAmountPosX;
	int				m_nAmountPosY;
	int				m_nSenderPosX;
	int				m_nSenderPosY;
	int				m_nTypePosX;
	int				m_nTypePosY;
	int				m_nStartTimePosX;
	int				m_nStartTimePosY;
	int				m_nEndTimePosX;
	int				m_nEndTimePosY;

	int				m_nNasAmountPosX;
	bool			m_bNas;		// 나스인지 아이템 인지
};

#	endif	// UI_LCE_SYSTEM_ITEM_LIST_H_