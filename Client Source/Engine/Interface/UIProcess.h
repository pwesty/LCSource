
// ----------------------------------------------------------------------------
//  File : UIProcess.h
//  Desc : Created by 이기환
// ----------------------------------------------------------------------------

#ifndef	UIProcess_H
#define	UIProcess_H
#ifdef	PRAGMA_ONCE 
	#pragma once
#endif

#include <Engine/Interface/UIListBox.h>

// Define max char and line of strings
#define	MAX_PROCESS_STRING			4
#define	PROCESS_STRING_CHAR_WIDTH	170

// Define text position
#define	PROCESS_TITLE_TEXT_OFFSETX	25
#define	PROCESS_TITLE_TEXT_OFFSETY	5
#define	PROCESS_DESC_TEXT_SX		18
#define	PROCESS_DESC_TEXT_SY		33

// Define size of Process
#define	PROCESS_TOP_HEIGHT			339
#define	PROCESS_BOTTOM_HEIGHT		41

#define	PROCESS_WIDTH				216
#define	PROCESS_HEIGHT				380

#define PROCESS_SLOT_ROW			4
#define PROCESS_SLOT_ROW_TOTAL		30


// 네트워크 메세지를 위한 데이터 구조체 
struct CNeedItems 
{	
	CItemData*	ItemData;
	SWORD		MatTab;
	SWORD		inven_idx;
	__int64		llCount;

	CNeedItems() : ItemData(NULL) {}
};



// ----------------------------------------------------------------------------
// Name : CUIProcess
// Desc : 제조 
// ----------------------------------------------------------------------------
class CUIProcess : public CUIWindow
{
protected:
	
// Controls...
	
	// Button
	CUIButton				m_btnClose;							// 닫기 버튼 
	CUIButton				m_btnOK;							// 가공 버튼 
	CUIButton				m_btnCancel;						// 취소 버튼 
	
	// Skill buttons
	std::vector< CUIIcon* > m_vecIcons;							// 가공 아이템

	// Etc ...
	CUIScrollBar			m_sbProcessItem;					// 가공 아이템 창 스크롤 바
	CUIListBox				m_lbPreconditionDesc;				// 필요 조건 설명 리스트 박스
	
	
//	Process Item Info

	int						m_nProcessText;						// 가공 문서 아이템 인덱스
	SWORD					m_nTab;
	SLONG					m_nInvenIdx;

	int						m_nSelectProcessItem;				// 현재 선택된 가공물
	CTString				m_StrProcessType;					// 가공 타입
	int						m_nProcessItemCount;				// 가공문서에 맞는 가공품 갯수
	
	BOOL					m_bSatisfied;						// 조건이 충분한가?
	
	CNeedItems				m_NeedItems[MAX_MAKE_ITEM_MATERIAL];// 필요 아이템 정보

	int						m_nNeedItemCount;					// 필요한 아이템 종류의 수
		
// Region of each part
	UIRect					m_rcTitle;							// Region of title bar
	UIRect					m_rcIcons;							// Region of icons
	UIRect					m_rcTab;							// Region of tab
	UIRect					m_rcItem;
	UIRect					m_rcDesc;
	
// UV of each part
	UIRectUV				m_rtBackgroundTop;					// UV of background UP
	UIRectUV				m_rtBackgroundBtm;					// UV of background DOWN
	UIRectUV				m_rtSelOutline;						// UV of outline of selected button

// Network ...
	BOOL					m_bWaitProcessResult;				// Wait Message

public:
	CUIProcess();
	~CUIProcess();

	
// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	
// Render
	void	Render();

// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	
// Open & close Process
	void	Clear ();
	void	InitProcess();

	ENGINE_API	void	OpenProcess( int nItemIndex, int nTab, int inven_idx );
	void				CloseProcess();

// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

// Network message functions ( receive )
	void	ProcessRep( SBYTE sbResult );
	
// Send ...
	void	SendProcessReq();

// etc ...
	void	SelectItem ( int nIndex = -1 );
	void	AddString ( CTString& strText, COLOR colText = 0xffffffff );

private:
	
	void	clearIcon();
};


#endif	// UIProcess

