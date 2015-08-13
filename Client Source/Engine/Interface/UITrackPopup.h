// ----------------------------------------------------------------------------
//  File : UITrackPopup.h
//  Desc : Created by cpp2angel
// ----------------------------------------------------------------------------

#ifndef	UITRACKPOPUP_H_
#define	UITRACKPOPUP_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIListBox.h>

//------------------------------------------------------------------------------
// class CUITrackPopup 
// Explain:  
// Date : 2005-05-16,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
class CUITrackPopup : public CUIListBox
{
protected:
	
public : // 9개나 만들 필요가 ... 구찮다.. 난중에 만들장..~
	// Tool Tip	
	UIRectUV		m_rtBackUL;				// UV of upper left region of Back				
	UIRectUV		m_rtBackUM;				// UV of upper middle region of Back				
	UIRectUV		m_rtBackUR;				// UV of upper right region of Back				
	UIRectUV		m_rtBackML;				// UV of middle left region of Back				
	UIRectUV		m_rtBackMM;				// UV of middle middle region of Back				
	UIRectUV		m_rtBackMR;				// UV of middle right region of Back				
	UIRectUV		m_rtBackLL;				// UV of lower left region of Back				
	UIRectUV		m_rtBackLM;				// UV of lower middle region of Back				
	UIRectUV		m_rtBackLR;				// UV of lower right region of Back				
	
	int				m_nMaxCharCount;														// [sora] 설정된 command수
	std::vector<int> m_vecCommandList;														// [sora] 설정한 command값 list
	CTString		m_strLongestStringInMenu;
public:
	CUITrackPopup();
	virtual ~CUITrackPopup();
	void Render();
	
	void AddMenuList( CTString strText, COLOR colText = 0xc2bac5FF , int nCommandNum = -1);		// [sora] 추가하는 메뉴의 command값을 입력할 수 있도록 수정
	void DeleteMenuList( int nCol, int nIndex );
	
	// russia resizing [8/31/2010 rumist]
	void Resize( int nStrLength = 0 );
	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	void ResetAllStrings();

	void Hide()	{	SetEnable( FALSE ); SetVisible( FALSE ); SetCurSel( -1 );	}
	void Show()	{	SetEnable( TRUE ); SetVisible( TRUE );		}
	int	 GetCommandNum(int nIndex);																// [sora] 지정한 메뉴의 commandnum값을 반환
	
};


#endif	// UISCROLLBAR_H_

//DEL 