#ifndef	__UI_NOTICE_H__
#define	__UI_NOTICE_H__

#include <Engine/Contents/Base/Notice.h>

class CUIList;
class CUIText;
class CUIListItem;
class CUIScrollBar;

class CUINoticeGroup : public CUIWindow
{
public:

	enum GROUP_TYPE
	{
		NONE_GROUP = -1,
		EVENT = 0,
		QUEST_ONCE,
		QUEST_UNLIMITED,
		GROUP_MAX,
	};

	CUINoticeGroup();
	~CUINoticeGroup();

	void	initialize();
	virtual CUIBase*	Clone();

	void	UpdateSlot();
	void	SetStringColor(CUIListItem* pItem, COLOR col = 0xFFFFFFFF);
		
	void	Open(GROUP_TYPE eType, int x, int y);
	void	Close();

	void	SetGroup(GROUP_TYPE eType = QUEST_ONCE);
	void	SetFixUi(bool bFix)						{ m_bFixUi = bFix; }
	bool	GetFixUi()								{ return m_bFixUi; }

private:
	GROUP_TYPE	m_eType;
	CUIList*	m_pNoticeList;
	CUIText*	m_pstrTitle;
	CUIScrollBar* m_pScroll;
	bool		m_bFixUi;
};

class CUINoticeNew : public CUIWindow
{
public:
	CUINoticeNew();
	~CUINoticeNew();

	void	initialize();
	
	void	UpdateBtn();
	void	PressOK(Notice::eNoticeType iNoticeType, int nIndex);

	void	SetIconBlink(CUINoticeGroup::GROUP_TYPE eType, bool bClick);
	void	SetIconBlink(Notice::eNoticeType eType, bool bBlink);
	
	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	Clear();
	
//////////////////////////////////////////////////////////////////////////
	// [2011/01/18 : Sora] 출석 이벤트
	void	ShowAttendanceMessage( SLONG attendanceRate );
	void	ShowAttendanceNotice( SLONG attendanceRate, SLONG enchantRate, SLONG expSpRate );
	BOOL	IsAttendanceEventOn() { return m_bAttendanceEvent; }

	// used party item. [4/25/2011 rumist]
	void	ShowUsedPartyItemNotice( const INDEX& nItemIdx, const CTString& strNickName );

	// royal rumble notice [4/26/2011 rumist]
	void	ShowRoyalRumbleNotice( const UBYTE uType );
	void	ShowRoyalRumbleWinner( const INDEX& iLevel, const CTString& strNickName );
	void	ShowRoyalRumbleNextPlayerTime( const INDEX iLeftTime );

private:
	BOOL			m_bAttendanceEvent;
	CUIList*		m_pList;
	CUINoticeGroup* m_pGroup;
	CUINoticeGroup* m_pDesign;
};


#endif	// UINOTICE_H_

