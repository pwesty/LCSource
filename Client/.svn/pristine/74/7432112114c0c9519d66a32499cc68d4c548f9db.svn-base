// ----------------------------------------------------------------------------
//  File : UINickName.h
//  Desc : NickName interface
//  code : 
// ----------------------------------------------------------------------------

#ifndef	UINICKNAME_H_
#define	UINICKNAME_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIWindow.h>
#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UIListBox.h>
#include <Engine/Interface/UIEditBox.h>
#include <Engine/Interface/UIScrollBar.h>
#include <Engine/Interface/UIRectString.h>
#include <vector>
#include <Entities/ItemData.h>

#define NICKNAME_WIDTH		251
#define NICKNAME_HEIGHT		285
#define NICK_NAME_MAX		50

class CUINickName : public CUIWindow  
{
protected:
	struct NickListData 
	{
		INDEX		NickAccessor;	// 호칭 인덱스(호칭 데이터 접근자)
		LONG		lDate;			// 만료 날짜
		INDEX		NickItemIndex;	// 호칭 아이템 인덱스
		CUIIcon*	pIconNick;		// 호칭 버튼

		NickListData() : NickAccessor(0), lDate(0), NickItemIndex(0) {}

		NickListData(INDEX l, LONG r, INDEX i) : NickAccessor(l), lDate(r), NickItemIndex(i) {}
		INDEX GetNickAccessor(void) { return NickAccessor; }
	};

	UIRectUV			m_rtBackGround;				// BackGround UV
	UIRectUV			m_rtSelectedBackGround;		// Select Box UV

	UIRect				m_rtCloseBtn;				// X Button Rect
	UIRect				m_rtSelectBtn;				// Select Button Rect
	UIRect				m_rtCancelBtn;				// Cancel Button Rect
	UIRect				m_rtDeleteBtn;				// Delete Button Rect
	UIRect				m_rcTitle;					// Title Rect
	UIRect				m_rtButtonAll;				// NickName Button Rect (5개의 호칭 버튼이 그려지는 전체 공간)
	UIRect				m_rtTextAll;				// Text(NickName, EndDate) Rect (5개의 호칭 이름, 만료기간이 그려지는 전체 공간)
	UIRect				m_rtBtn[5];					// NickName Button Rect (5개의 호칭 버튼 각각의 공간)
	UIRect				m_rtText[5];				// Text Rect (5개의 호칭 이름, 만료기간이 그려지는 각각의 공간)
		
	CTextureData		*m_ptdButtonTexture;		// Texture
	
	CUIButton			m_btnClose;					// X Button 
	CUIButton			m_btnDelete;				// Delete Button
	CUIButton			m_btnCancel;				// Cancel Button
	CUIButton			m_btnSelect;				// Select Button
	
	CUIScrollBar		m_sbNickNameListBox;		// Scroll Bar

	BOOL				m_bTitleBarClick;			// Title Bar
	
	SLONG				m_sbNickNameListBoxNumber;	// 선택된 리스트 박스 인덱스
	INDEX				m_nNickNameListSize;		// 리스트의 총 갯수
	
	int					m_nScrollBarPos;			// Scroll Bar Position
	int					m_nCurInfoLines;			// 멀티 라인
	
	CTString			m_strItemInfo[2];							// Item information string
	COLOR				m_colItemInfo[2];							// Color of item information string
	
	std::vector<NickListData> m_NickList;			// NickName Data를 저장하기 위한 list

	
	
public:
	CUINickName();
	~CUINickName();
	void		Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	void		Clear();							// 호칭창 초기화
	
	void		AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void		ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void		CloseNickName();

	void		Render();				// Render
	void		RenderListButton();		// Button Render
	void		RenderText();			// Text (NickName, EndDate) Render
	void		RenderSelectedTexture();	// Select Box Render

	WMSG_RESULT MouseMessage( MSG *pMsg );	// Mouse Message

	void		MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );	// Message Box Command

	void		Open();							// Open

	CTString	CulEndDate( INDEX iIndex );		// EndDate

	void		AddNickNameList( INDEX iIndex, LONG lDate );		// Add NickName
	void		DelNickNameList( INDEX iIndex );	// Delete NickName
	void		SortNickNameData();

	void		SetBtnPos();									// Set Button Position

	void		SetCancelButtonState( BOOL b) {	m_btnCancel.SetEnable(b); }	// Cancel Button State(select button, delete button, cancel button) State
	
	void		AddNickNameText( CTString &strItemInfo, COLOR colItemInfo ,int maxLine=2, int maxChars=20);	// 호칭 선택 창 텍스트 추가

	static bool SortCompare(const NickListData& pNickData1, const NickListData& pNickData2)
	{
		int nLIndex = CNickNameData::getData(pNickData1.NickAccessor)->GetItemIndex();
		int nRIndex = CNickNameData::getData(pNickData2.NickAccessor)->GetItemIndex();

		std::string strL = CItemData::getData(nLIndex)->GetName();
		std::string strR = CItemData::getData(nRIndex)->GetName();

		return strL < strR ? true : false; 
	}
	
	//  [3/25/2010 kiny8216] 호칭 이름을 아이템 lod에서 불러오도록 변경
	CTString	GetName(INDEX iIndex);
	CTString	GetDesc(INDEX iIndex);
	// 랭킹시스템에서 호칭을 받아서 호칭 만료 시간이 변경 될 수 있다.
	void		ChangeTitleTime ( INDEX iIndex, LONG iDate );
	int			GetTitleIndex(int nItemIndex);
};

#endif // !defined(AFX_UINICKNAME_H__379A2562_EA30_4320_8038_73C1035B9C78__INCLUDED_)
