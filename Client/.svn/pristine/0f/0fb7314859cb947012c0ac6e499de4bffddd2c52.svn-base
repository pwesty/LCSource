
#ifndef	UITALK_H_
#define	UITALK_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// #include <Engine/Interface/UIEditBox.h>
// #include <Engine/Interface/UIListBox.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Interface/UIGroup.h>
#include <Engine/Interface/UITrackPopup.h>
 
//------------------------------------------------------------------------------
// CUITalk
// Explain:  
// Date : 2005-05-19,Author: Lee Ki-hwan
// History: 
//------------------------------------------------------------------------------
class CUITalk : public CUIWindow
{
public:
	
	// Control
	CUIButton				m_btnClose;
	CUIButton					m_btnMember;
	CUIButton					m_btnColor;
	CUIEditBox				m_ebInput;
	CUIListBox				m_lbTalkList;
	
	// Data	
	CMemberInfo				m_miMyInfo;
	std::vector<CTString>		m_vecTarget;			// 대화상대 정보 
	std::vector<CTString> 		m_vecTalkList;		// 대화 내용


	// Region of each part
	UIRect					m_rcTitle;								// Region of title
	UIRect					m_rcInputBox;					// Region of input box

	// UV of each part
	UIRectUV3				m_rt3BackT;								// UV of upper left background
		
	UIRectUV3				m_rt3BackMT;								// UV of middle left background
	UIRectUV3				m_rt3BackMM;								// UV of middle middle background
	UIRectUV3				m_rt3BackMB;								// UV of middle right background
	
	UIRectUV3				m_rt3BackMST;							// UV of middle left background
	UIRectUV3				m_rt3BackMSM;							// UV of middle middle background
	UIRectUV3				m_rt3BackMSB;							// UV of middle right background

	UIRectUV3				m_rt3BackM;								// UV of middle left background
	UIRectUV3				m_rt3BackL;								// UV of lower left background
	
	UIRectUV3				m_rt3InputBox;

	CTString				m_strTitle;
	
	int						m_nBoardHeight;
	
	int						m_nWhichUI;

	CUITrackPopup			m_tpTargetList;
	CUITrackPopup			m_tpColor;

	CUIButton				m_btnColorList[9];

	COLOR					m_nColIndex;

	int						m_nMakeCharIndex;
	int						m_nChatIndex;

	//CUITrackPopup			m_tpImoticon;
	// Messenger [12/8/2006 KwonYongDae]

	int						m_nClientIndex;		// 클라이언트에서 사용하는 채팅 창에 대한 Index
	int						m_nTargetCharIndex;
public :
	CUITalk();
	virtual ~CUITalk();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void SetFocus( BOOL bVisible );
	void Clear();
	void Close();
	void Create( CUIWindow *pParentWnd, int nWhichUI );
	void CreateTrackPopup( float fTexWidth, float fTexHeight);
	void Open( CTString strName );
	//  [12/8/2006 KwonYongDae]
	void Open( int clientIndex, const CMemberInfo targetInfo );

	void RenderRectUV3( int nUnitSize, int nX, int nY, int nWidth, int nHeight, UIRectUV3 rtRectUV3 );
	void Render();
	void RenderBackground( int nX, int nY );
	
	void AddTalkListString( CTString strName, CTString strMessage, bool bFocus = true, COLOR colDesc = 0xF2F2F2FF );
	void AddTalkListString( CTString &strDesc, COLOR colDesc = 0xF2F2F2FF );
	void AddErrorTalk( CTString &strDesc, COLOR colDesc = 0xE28769FF );
	void AddTalkTarget( CTString strName);
	void DeleteTalkTarget(CTString strName);
	bool IsExistTarget( CTString strName );
	void ReloadTalkList();
	
	void SetTargetCondition( int nCharIndex, eCondition eState );
	void SetIndex(int nMakeCharIndex, int nChatIndex) { m_nMakeCharIndex =nMakeCharIndex; m_nChatIndex =nChatIndex;};
	// Network
	void SendChatMessageFiltering( CTString stMessage );
	void SendChatMessage( CTString stMessage );
	
	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	WMSG_RESULT	KeyMessage( MSG *pMsg );
	WMSG_RESULT	IMEMessage( MSG *pMsg );
	WMSG_RESULT	CharMessage( MSG *pMsg );

	BOOL	IsEditBoxFocused() { return m_ebInput.IsFocused(); }
	void	KillFocusEditBox() { m_ebInput.SetFocus( FALSE ); }
	
	//  [12/8/2006 KwonYongDae]
	void	SetClientIndex( int clientIndex ) { m_nClientIndex = clientIndex; }
	void	SetServerIndex( int serverIndex ) { m_nChatIndex = serverIndex; }
	int		GetClientIndex() { return m_nClientIndex; }
	int		GetServerIndex() { return m_nChatIndex; }
	int		GetTargetIndex() { return m_nTargetCharIndex; }
	
	
};

#endif	// UITALK_H_