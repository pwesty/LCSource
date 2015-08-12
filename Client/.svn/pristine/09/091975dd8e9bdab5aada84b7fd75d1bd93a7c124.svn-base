
#ifndef	UIMESSENGER_H_
#define	UIMESSENGER_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIGroup.h>
#include <Engine/Interface/UITrackPopup.h>
#include <Engine/GlobalDefinition.h>

// Define size of messenger
//#define	MESSENGER_WIDTH					180
//#define	MESSENGER_HEIGHT				291
#define	MESSENGER_WIDTH					288
#define	MESSENGER_HEIGHT				308

enum eGroup
{
	GROUP_INSERT,		//그룹 추가
	GROUP_DELETE,		//그룹 삭제
	GROUP_RENAME,		//그룹명 변경
};

enum eMemberMenu
{
	MM_GROUP_CHANGE,	//그룹 이동
	MM_CHAT,			//대화하기
	MM_DELETE,			//삭제
	MM_BLOCK,			//차단
	MM_TELEPORT			//텔레포트
};
//------------------------------------------------------------------------------
// CUIMessenger
// Explain:  
// Date : 2005-05-19,Author: Lee Ki-hwan
// History: 
//------------------------------------------------------------------------------
class CUIMessenger : public CUIWindow
{
protected:
	// Controls
	CUIButton				m_btmSetState;				//상태변화 버튼
	
	CUIButton				m_btnClose;					//닫기 버튼
	CUIButton				m_btnGroup;					//"그룹관리" 버튼
	CUIButton				m_btnBlock;					//"차단" 버튼
	CUIButton				m_btnInsert;				//"추가" 버튼
	CUIButton				m_btnDelete;				//"삭제" 버튼
	
	CUIScrollBar			m_sbMemberList;				//스크롤

	CTString				m_strTitle;					// 타이틀바 이름

	// Information of friend & guild
	CMemberInfo				m_MyInfo;					// 내 정보
	int						m_nTarIndex;				// 상대방 인덱

	
	// Region of each part
	UIRect					m_rcTitle;								// Region of title
	
	// 크기 변경 가능 영역
	UIRect					m_rcResizeLeft;							// Region of left resizing
	UIRect					m_rcResizeRight;						// Region of right resizing
	UIRect					m_rcResizeBottomL;						// Region of left bottom resizing
	UIRect					m_rcResizeBottomM;						// Region of middle bottom resizing
	UIRect					m_rcResizeBottomR;						// Region of right bottom resizing
	
	//Tab
	UIRect					m_rcFriendTab;				//친구탭 영역
	UIRect					m_rcBlockTab;				//차단탭 영역
	bool					m_bFriendTab;				//활성화된 탭( true=>친구, false=>차단)

	// UV of each part
	UIRectUV3				m_rt3BackT;								// UV of upper left background
		
	UIRectUV3				m_rt3BackMT;								// UV of middle left background
	UIRectUV3				m_rt3BackMM;								// UV of middle middle background
	UIRectUV3				m_rt3BackMB;								// UV of middle right background
	
	UIRectUV3				m_rt3Tab;				// UV of Tab

	UIRectUV3				m_rt3BackMST;							// UV of middle left background
	UIRectUV3				m_rt3BackMSM;							// UV of middle middle background
	UIRectUV3				m_rt3BackMSB;							// UV of middle right background

	UIRectUV3				m_rt3BackM;								// UV of middle left background
	UIRectUV3				m_rt3BackL;								// UV of lower left background
	
	int						m_nBoardHeight;				//그룹을 표시할 부분의 높이

	CUITrackPopup			m_tpSetMyState;				//내상태변경 툴팁
	CUITrackPopup			m_tpMemberMenu;				//멤버 우클릭시 나타나는 툴팁
	CUITrackPopup			m_tpBlock;					//차단 멤버 우클리시 나타나는 톨팁
	
//그룹 관련 멤버
protected:
	std::vector<CUIGroup*>	m_vecGroup;					//그룹 리스트
	CUIGroup				m_mgBlock;					//차단 멤버 그룹

	CUITrackPopup			m_tpGroup;					//"그룹관리" 버튼 클릭시 나타나는 툴팁
	CUITrackPopup			m_tpGroupMenu;				//그룹 우클릭시 나타나는 툴팁
	CUITrackPopup			m_tpGroupList;				//툴팁에서 사용하는 그룹 리스트

	float					m_fTexWidth, m_fTexHeight;	//텍스쳐의 크기

	UIRect					m_rcBoard;					//그룹을 표시할 영역
		
	int						m_nSelectGroup;				//현재 선택한 그룹(vector<> 상의 인덱스)
	int						m_nTalkCount;				//실행중인 채팅창의 수
	CMemberInfo				m_miSelectMember;			//현재 선택한 멤버
	
	bool					m_bSelect;					//멤버를 클릭?
	bool					m_bDragging;				//드래그중?
	bool					m_bDrop;					//드래그중인 멤버를 드랍?

	int						m_nX, m_nY;					//마우스의 위치(멤버 드래그시 사용)
	int						m_nFriendScrollPos;			//친구 탭에서의 스크롤 위치
	int						m_nBlockScrollPos;			//차단 탭에서의 스크롤 위치

	int						m_nActiveMenu;				//그룹 리스트(m_tpGroupList)를 나타낼 툴팁
														// 0 => m_tpGroupMenu, 1 =>m_tpGroup
	bool					m_bPremiumChar;

	CTextureData*			m_ptdMenuTexture;			//툴팁에 사용될 화살표를 위한 텍스쳐
	UIRectUV				m_rtArrow;					//화살표 텍스쳐 좌표
	
	UIRectUV				m_rtTab;					//텝 텍스쳐 좌표	
			
public:
	CUIMessenger();
	virtual ~CUIMessenger();

	void	Clear();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	CreateTrackPopup( float fTexWidth, float fTexHeight );
	void	CreateGroup( float fTexWidth, float fTexHeight );

	// Toggle visible
	void	ToggleVisible();

	// Render
	void	Render();
	void	RenderBackground( int nX, int nY );
	void	RenderMyInfo( int nX, int nY );
	void	RenderGroup( int nX, int nY );
	void	RenderTab(int nX, int nY);
	void	RenderSelectMember(int nX, int nY);
	void	RenderMenuArrow(int nX, int nY, int nWidth, int nHeight, int nLine);
	
	void	RenderRectUV3( int nUnitSize, int nX, int nY, int nWidth, int nHeight, UIRectUV3 rtRectUV3 );

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	ResetSavePosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Reset
	void	ReSizing( int nResizeRange, int nWidth, int nHeight );
		
	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	
	//wooss 051005
	void	SetMyInfo( int nCharIndex, CTString strName, eJob nJob=TOTAL_JOB, eCondition eState = ONLINE );
	void	SetMyState( eCondition eState );
	
	
	// Member Control
	void AddFriendList( int nCharIndex, int nGroupIndex, CTString strName, eJob nJob, eCondition eState = ONLINE, bool bBlock=false );
	void AddFriendList( CMemberInfo miMember,  bool bBlock );
	void DeleteMember();
	void DeleteMember( int nCharIndex );
	//void SetMemberCondition( eCondition eState );
	void SetMemberCondition( int nCharIndex, eCondition eState );
	eCondition GetMemberCondition(int nCharIndex);
	void SetMemberMenu(eMemberMenu eMenu, int nSubMenu);
	void SetBlock( int nCharIndex, CTString strName, bool bBlock);

	// Group
	void AddGroup(int nGroupIndex, CTString strName);
	void DeleteGroup();
	void RenameGroup(int nGroupIndex, CTString strNewName);
	void SetGroup(eGroup eState, int nSubMenu);
	void SetDrop( bool bDrop ) { m_bDrop =bDrop; };
	void SetDragging( bool bDragging ) { m_bDragging =bDragging; };
	void ChangeGroup(int nGroup);
	int FindGroup(int nGroupIndex);
	int FindGroup(CTString strName);
	CMemberInfo FindMember(CTString strName);
	CMemberInfo FindBlockMember(CTString strName);
	bool IsDrop() { return m_bDrop; };
	CMemberInfo GetSelectMember() { return m_miSelectMember; };
	void SetChat(int nMakeCharIndex, int nChatIndex, CTString strName);
	void AddChatMember(int nMakeCharIndex, int nChatIndex, CTString strName);
	void DeleteChatMember(int nMakeCharIndex, int nChatIndex, CTString strName);
	void SetDefaultGroupName(CTString strName);
	void Block(BYTE cError, int nCharIndex, CTString strName);
	void UnBlock(BYTE cError, int nCharIndex, CTString strName);
	
	// 친구메뉴 업데이트
	void UpdateMemberMenu();

	// 프리미엄 캐릭터
	void SetPremiumBenefit(bool bUse);
	
	//	Network
	void RegistMemberReq( CTString strName );	// 친구등록 요청
	void RegistMemberReq( int nCharIndex, CTString strName );	// 친구등록 요청
	void RegistAllow();		// 친구 등록 수락 
	//void RevTalk( int nSendIndex, CTString strSendName, CTString strRecvName, CTString strMessage );
	void RevTalk( int nMakeCharIndex, int nChatIndex, CTString strSendName, CTString strChat, int nColIndex );
	
	
	void ErrorMsg( int nError );

	//void Message( int nCommandCode, CTString strMessage, DWORD dwStyle );
	void Message( int nCommandCode, CTString strTitle, CTString strMessage, DWORD dwStyle );
	void ErrorMessage( CTString strErrorMessage );
	void CloseAllMessageBox();

	void ResizeScrollBar();
	void DeleteMemberRep();

	void Reset();
	void ReSetFriendList();
	bool ReadyOpenTalk( CMemberInfo miMemberInfo, bool bFocus = true );
	int OpenTalk( int nMakeCharIndex, int nChatIndex, CTString strName);
	void CloseTalk( int nIndex );
	void TalkErrorMessage( CTString strMessage );

	CMemberInfo GetMyInfo() 
	{ 
		return m_MyInfo;
	}

	bool IsUseLevel();
	
	std::vector<CUIGroup*>& GetGroup(void) { return m_vecGroup; }

	//  [12/8/2006 KwonYongDae]
	int OpenTalk( int nChatIndex, const CMemberInfo targetInfo );
	void RevOneVsOneTalk( int nSenderIndex, int nResiverIndex, CTString strSendName, CTString strChat, int nColIndex );

	BOOL	CloseWindowByEsc()			{ ToggleVisible();	return TRUE;		}	
};


#endif	// UIMESSENGER_H_