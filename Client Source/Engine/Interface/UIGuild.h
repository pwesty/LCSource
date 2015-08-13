// ----------------------------------------------------------------------------
//  File : UIGuild.h
//  Desc : Created by Dongmin
// ----------------------------------------------------------------------------

#ifndef	UIGUILD_H_
#define	UIGUILD_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIWindow.h>
#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UIListBox.h>
#include <Engine/Interface/UIEditBox.h>
#include <Engine/Interface/UIMultiEditBox.h>
#include <Engine/Interface/UIWebBoard.h>
#include <Engine/Interface/UIComboBox.h>
#include <vector>
#include <Engine/Interface/UIDrawFigure.h>	//WSS_NEW_GUILD_SYSTEM 070611
#include <Engine/LocalDefine.h>				//WSS_NEW_GUILD_SYSTEM 070611
#include <Engine/Interface/UIListBoxEx.h>
#include <Engine/Interface/UIGuildArrangeMemberCont.h>
#include <Common/Packet/ptype_old_do_guild.h>


// Define text position
#define	GUILD_TITLE_TEXT_OFFSETX		25
#define	GUILD_TITLE_TEXT_OFFSETY		5

// Define size of Guild
#define	GUILD_WIDTH						216
#define	GUILD_HEIGHT					284

// Define member level
enum eMemberLevel
{
	GUILD_MEMBER_BOSS			= 0,		// 단장
	GUILD_MEMBER_VICE_BOSS		= 1,		// 부단장
	GUILD_MEMBER_MEMBER			= 2,		// 단원
	GUILD_MEMBER_RUSH_CAPTAIN	= 3,		// 돌격대장
	GUILD_MEMBER_SUPPORT_CAPTAIN= 4,		// 지원대장
	GUILD_MEMBER_RECON_CAPTAIN	= 5,		// 정찰대장
	GUILD_MEMBER_RUSH			= 6,		// 돌격대원
	GUILD_MEMBER_SUPPORT		= 7,		// 지원대원
	GUILD_MEMBER_RECON			= 8,		// 정찰대원
	GUILD_MEMBER_TOTAL			= 9,		// TOTAL
	GUILD_MEMBER_NOMEMBER	= -1,		// 멤버 아님
};

#define	MAX_APPLICANTS			(10)		// 최대 가입 신청자.
#define GUILD_SKILL_VIEW_MAX	(5)			// 길드 스킬 창에 보이는 최대 스킬 갯수
// WSS_NEW_GUILD_SYSTEM 070716 ------------------------------->>
#if defined(G_KOR)
	#define MAX_GUILDINFO_TAB	(6)
#else
	#define MAX_GUILDINFO_TAB	(5)			//해외로컬은 길드 게시판 사용 안함 사용시 (6)으로 수정
	#define LOCAL_NEW_GUILD					//해외 로컬
#endif
#define LIMIT_GUILD_LEVEL	(6)
// Define Guild Info Tab
enum eGUILDNEW
{
	NEW_GUILD_INFO				= 0,		// 길드 정보
	NEW_GUILD_MEMBER_INFO		= 1,		// 길드원 정보
	NEW_GUILD_SKILL				= 2,		// 길드 스킬
#ifndef LOCAL_NEW_GUILD
	NEW_GUILD_BOARD,						// 길드 게시판
#endif
	NEW_GUILD_NOTICE,					// 길드 공지사항
	NEW_GUILD_MANAGE,					// 길드 관리		
};

enum eGUILD_MEMBER_LIST
{
	eGML_NAME = 0,
	eGML_TITLE,
	eGML_LV,
	eGML_CLASS,
	eGML_LOCAL,
	eGML_CONTRIBUTION,
	eGML_POINT,
	eGML_MAX
};
class CUIIcon;
// -----------------------------------------------------------<<

// ----------------------------------------------------------------------------
// Name : CUIGuild
// Desc :
// ----------------------------------------------------------------------------
class CUIGuild : public CUIWindow
{
protected:	
	// Guild State
	enum eGuildState
	{
		GUILD_REQ,
		GUILD_CREATE,		// 생성
		GUILD_UPGRADE,		// 승급
		GUILD_DESTROY,		// 해체
		GUILD_MANAGER,		// 관리
		GUILD_MANAGER_NEW,	// 신규 길드 시스템 WSS_NEW_GUILD_SYSTEM 070704
	};
	
	// Define guild tab
	enum GUILD_TAB
	{
		GUILD_TAB_NONE		= -1,
		GUILD_TAB_MEMBER	= 0,		// 멤버 목록
		GUILD_TAB_TREE		= 1,		// 길드 트리
		GUILD_TAB_TOTAL		= 2,
	};

	// internal structure
	struct sGuildMember
	{
		sGuildMember()
		{
			lIndex			= -1;
			eRanking		= GUILD_MEMBER_MEMBER;
			bOnline			= FALSE;
		}

		~sGuildMember()
		{
			lIndex			= -1;
			eRanking		= GUILD_MEMBER_MEMBER;			
			bOnline			= FALSE;
		}

		bool operator<(const sGuildMember &other) const
		{
			if( eRanking < other.eRanking )
				return true;
			return false;
		}
		bool operator>(const sGuildMember &other) const
		{			
			return other.operator < (*this);
		}

		SLONG			lIndex;
		CTString		strMemberName;		// 멤버명?
		int				eRanking;			// 순위?
		BOOL			bOnline;			// 접속중?

	};

	// Function Object
	struct FindMember
	{
		SLONG lIndex;
		FindMember(sGuildMember& info) : lIndex(info.lIndex) {};
		bool operator() (const sGuildMember& info)
		{
			if(info.lIndex == lIndex)
			{
				return true;
			}
			else
				return false;
		}
	};

	std::vector<sGuildMember>	m_vectorMemberList;			// 단원 목록
	
	// Controls
	CUIButton				m_btnClose;						// Close button
	CUIButton				m_btnOK;						// OK button
	CUIButton				m_btnCancel;					// Cancel button

	CUIButton				m_btnChangeBoss;				// 단장 이임
	CUIButton				m_btnAccept;					// 부단장 임명(appointment)
	CUIButton				m_btnReject;					// 부단장 해임(dismissal)
	CUIButton				m_btnMemberFire;				// 단원 퇴출
	CUIButton				m_btnExit;						// 닫기

	// Region of each part
	UIRect					m_rcTitle;						// Region of title bar
	UIRect					m_rcTab;

	// UV of each part
	UIRectUV				m_rtBackground;					// UV of background		
	UIRectUV				m_rtBackTop;					// UV of top background
	UIRectUV				m_rtBackMiddle1;				// UV of middle background
	UIRectUV				m_rtBackMiddle2;				// UV of middle background
	UIRectUV				m_rtBackManagerTop;				//
	UIRectUV				m_rtBackManagerMiddle;			//
	UIRectUV				m_rtBackManagerBottom;			//
	UIRectUV				m_rtBackBottom;					// UV of bottom background
	UIRectUV				m_rtInputBoxL;					// UV of left region of input box
	UIRectUV				m_rtInputBoxM;					// UV of middle region of input box
	UIRectUV				m_rtInputBoxR;					// UV of right region of input box

	UIRectUV				m_rtTab;						// UV of section tab

	CUIListBox				m_lbGuildDesc;					// List box of guild description
	CUIListBox				m_lbMemberList;					// List box of guild member list
	CUIListBox				m_lbApplicantList;				// List box of guild new user list

	// edit box of guild name
	CUIEditBox				m_ebGuildName;

protected:
	int						m_nCurrentTab;					// Current tab
	eGuildState				m_eGuildState;

	CTString				m_strGuildName;					// Name of Guild
	CTString				m_strBossName;					// Name of Boss
	CTString				m_strViceBoss[2];				// Name of first vice boss
	int						m_iGuildIndex;
	int						m_iGuildLevel;					// Level of Guild
	int						m_iNumOfMember;					// Number of member
	int						m_iUserRanking;					// Ranking of user
	BOOL					m_bGuildJoinReq;				//

	BOOL					m_bChackNpc;					//카오마을 NPC 길드 마스터 체크
	// WSS_NEW_GUILD_SYSTEM 070608 ------------------------->>

	// 길드 정보------------------------------------>>
	// 길드 시스템 개편 추가 변수
	INDEX					m_iSelTab;						// 선택한 텝(0 ~ 5)
	INDEX					m_iGuildAverageLevel;			// 길드 평균 레벨
	INDEX					m_iGuildOwnLand;				// 길드 소유 영지
	INDEX					m_iGuildTotalPoint;				// 길드 총 포인트
	INDEX					m_iGuildMyPoint;				// 나의 기여 포인트		
	INDEX					m_iNumOfMaxMember;				// Number of Max Member 

	CTString				m_strRemoteGuildJoinTaget;		// 원격 길드 가입 대상 이름

	// 렌더링 관련 추가 변수
	// Region of each part
	UIRect					m_rcTitleNew;					// Region of title bar
	UIRect					m_rcWindowNew;					// Region of Entire Window
	UIRect					m_rcTabNew[MAX_GUILDINFO_TAB];

	// UV of each part
	UIRectUV				m_uvTabNew;						// UV of section tab
	UIRectUV				m_uvInfoTabTL;					// UV of top left background
	UIRectUV				m_uvInfoTabTR;					// UV of top right background
	UIRectUV				m_uvInfoTabBL;					// UV of Bottom left background
	UIRectUV				m_uvInfoTabBRL;					// UV of Bottom Right1 background
	UIRectUV				m_uvInfoTabBRR;					// UV of Bottom Right2 background

	// UV of Lines
	UIRectUV				m_uvLineV;						// UV of vertical line
	UIRectUV				m_uvLineH;						// UV of Horizon line

	// Common Box
	CUIDrawBox				m_bxBox1;						// Box(1) - 옅은 색 박스
	CUIDrawBox				m_bxBox1_1;						// Box(1) - 옅은 색 박스 밑줄 없는거
	CUIDrawBox				m_bxBox1_2;						// Box(1) - 옅은 색 박스 윗줄 없는거
	CUIDrawBox				m_bxBox2;						// Box(2) - 짙은 색 박스
	CUIDrawBox				m_bxBackGroundBox;				// Box of Top Background
	CUIDrawBox				m_bxBackGroundBox2;				// Box of Bottom Background

	// ListBox
	CUIListBox				m_lbMemberAllianceList;			// List box of Alliance member list
	CUIListBox				m_lbMemberHostilityList;		// List box of Alliance member list

	// Buttons
	CUIButton				m_btnEdit;						// 편집 버튼
	CUIButton				m_btnCloseNew;					// X 버튼
	CUIButton				m_btnExitNew;					// 닫기 버튼
	CUIButton				m_btnGuildMarkBack;				// 길드 마크 바탕 버튼
	// --------------------------------------------<<

	// 길드원 정보 -------------------------------->>	
	
	CUIListBoxEx			m_lbGuildMemberList;			// Guild Member List
	INDEX					m_iOnlineMembers;				// 접속한 길드원
					
	// --------------------------------------------<<

	// 길드 스킬 ---------------------------------->>
	UIRect					m_rcGuildSkillList;
	CUIButton				m_btnGetSkill;					// 습득하기
	CUIScrollBar			m_sbGuildSkillBar;				// Guild Skill Scroll Bar
	INDEX					m_iGuildSkillPos;				// Guild Skill Position
	BOOL					m_bIsSelList;					// 길드 스킬 체크 여부
	CUIListBox				m_lbGuildSkillDesc;
	
	// --------------------------------------------<<

	// 길드 게시판 -------------------------------->>
	CUIWebBoard*			m_pWebBoard;					// 길드 게시판에서 사용(이전 게시판에서)
	CUIDrawBox				m_bxBox3;						// Box(3) - 녹색 박스
	CUIDrawBox				m_bxBox4;						// Box(4) - 테두리없는 검정박스

	// Controls	
	CUIButton			m_btnPrev;							// Prev button
	CUIButton			m_btnNext;							// Next button
	CUIButton			m_btnList;							// List button
	CUIButton			m_btnWrite;							// Write button
	CUIButton			m_btnReply;							// Reply button
	CUIButton			m_btnModify;						// Modifu button
	CUIButton			m_btnDelete;						// Delete button
	CUIButton			m_btnPage[10];						// Buttons of page
	CUIButton			m_btnSearch;						// Search button
	CUIEditBox			m_ebSearch;							// Search edit box
	CUIComboBox			m_cmbSearch;						// Combobox of search

	// List type
	CUIListBox			m_lbListContent;					// Listbox of list content ( col : 0-No, 1-Subject, 2-Writer, 3-Date, 4-Hit )	

	// Read type
	CUIListBox			m_lbReadContent;					// Listbox of reading content	

	// Write type
	CUIEditBox			m_ebWriteSubject;					// Subject edit box in writing

	// Write ...
	CUIMultiEditBox		m_mebContent;						// 쓰기 내용
	
	// -------------------------------------------->>

	// 길드 공지 ---------------------------------->>
	// 공지 입력	
	CUIEditBox				m_ebNoticeTitle;					// 제목
	CUIMultiEditBox			m_mebNoticeContent;					// 내용
	CUIButton				m_btnNotice;						// 공지 버튼
	CUIButton				m_btnNoticeCorrect;					// 수정 버튼	
	CUIButton				m_btnUpdateNotice;					// 작성완료 버튼
	BOOL					m_bEnableCorrect;					// 수정 여부
	// --------------------------------------------<<

	// 길드 관리 ---------------------------------->>
	std::vector<INDEX>		m_vManageMemberIndex;			// 멤버 인덱스(list box의 순서와 같음)
	CUIListBox				m_lbManageMemberList;			// 설정 리스트
	CUIButton				m_btnChangeBossNew;				// 단장 이임
	CUIButton				m_btnAcceptNew;					// 부단장 임명
	CUIButton				m_btnRejectNew;					// 부단장 해임
	CUIButton				m_btnMemberFireNew;				// 멤버 퇴출
	CUIButton				m_btnChangeSetting;				// 설정 변경

	// 수정 윈도우
	BOOL					m_bApplySettingOn;				// 설정 변경창 상태
	CUIButton				m_btnApplySetting;				// 설정 적용
	CUIButton				m_btnApplySettingClose;			// 설정 적용 닫기
	CUIEditBox				m_ebChangePositionName;			// 직위명 변경
	CUIEditBox				m_ebChangePayExp;				// 상납 경힘치 포인트 
	CUIEditBox				m_ebChangePayFame;				// 상납 명성치 포인트
	CUICheckButton			m_ckGuildStashPermission;		// 길드 창고 사용여부 [6/24/2011 rumist]

	
	int						m_nSelSkillTab;					// 선택된 길드 스킬 탭 ( 0 : Passive Guild Skill, 1 : Active Guild Skill )
	CTString				m_strSkillTabPopupInfo;			// 탭 팝업 정보 (액티브, 패시ㅡ)
	UIRect					m_rcSkillTab[2];				// 길드 스킬 탭
	UIRect					m_rcSkillBtn[5];				// 길드 스킬 버튼 위치
	UIRect					m_rcSkillTabPopupInfo;			// 탭 팝업 정보
	UIRectUV				m_rtSelBoxUV;					// 선택된 길드 스킬 UV
	UIRectUV				m_rtSelSideTabUV;				// 선택된 세로 탭
	UIRectUV				m_rtUnSelSideTabUV;				// 선택 안된 세로 탭
	UIRectUV				m_rtPopupUL;					// 길드 스킬 탭 팝업 UV
	UIRectUV				m_rtPopupUM;					// 길드 스킬 탭 팝업 UV
	UIRectUV				m_rtPopupUR;					// 길드 스킬 탭 팝업 UV
	UIRectUV				m_rtPopupML;					// 길드 스킬 탭 팝업 UV
	UIRectUV				m_rtPopupMM;					// 길드 스킬 탭 팝업 UV
	UIRectUV				m_rtPopupMR;					// 길드 스킬 탭 팝업 UV
	UIRectUV				m_rtPopupLL;					// 길드 스킬 탭 팝업 UV
	UIRectUV				m_rtPopupLM;					// 길드 스킬 탭 팝업 UV
	UIRectUV				m_rtPopupLR;					// 길드 스킬 탭 팝업 UV
	UIRectUV				m_rtPassiveTabIconUV[2];		// Passive Tab Icon UV
	UIRectUV				m_rtActiveTabIconUV[2];			// Active Tab Icon UV
	CTextureData			*m_ptdSelBoxTexture;			// 선택된 길드 스킬 Texture
	CUIButton				m_btnUseSkill;					// 길드 스킬 사용하기 버튼
	CUIListBox				m_lbUseInfo;					// 스킬 사용 정보 표시 박스
	CUIListBox				m_lbLearnInfo;					// 스킬 습득 정보 표시 박스
	// --------------------------------------------<<
	// [1/3/2011 kiny8216] NEW_CORPS
	CUIComboBox				m_cmbCorps;							// 군단 선택 콤보 박스
	INDEX					m_iCorpsMember;						// 일반직위 멤버 수
	INDEX					m_iCorpsBoss;						// 대장직위 멤버 수
	CTString				m_strCorps[GUILD_MEMBER_TOTAL];		// 군단 스트링
	// [1/11/2011 kiny8216] GUILD_SKILL_FIX
	CUIIcon*				m_pIconsGuildSkillEx[GUILD_SKILL_VIEW_MAX];	// 길드 스킬 버튼
	std::vector<CSkill>		m_vecGuildPassiveSkill;						// 길드 패시브 스킬
	std::vector<CSkill>		m_vecGuildActiveSkill;						// 길드 액티브 스킬

	LONG					m_guildMarkTime;					// [sora] GUILD_MARK
	CTString				m_strGuildMarkTime;
	CUIGuildMarkIcon*		m_pIconGuildMark;

	// 정렬을 위해 추가.
	ContGuild				m_ContGuild;

	INDEX			m_iSyndiType;

	// -------------------------------------------------------<<

	// Position of target npc
	FLOAT					m_fNpcX, m_fNpcZ;

	// Command
	void	PressOKBtn();

	// Description
	void	GetGuildDesc( BOOL bShow = TRUE );
	void	AddGuildDescString( CTString &strDesc, COLOR colDesc = 0xF2F2F2FF );

	// 길드 자체에 대한 처리
	void	CreateGuild();			// 길드 생성
	void	UpgradeGuild();			// 길드 승급
	void	DestroyGuild();			// 길드 해체
	void	GoGuildZone();			// 길드 전용 공간으로...

	// 멤버에 대한 처리
	void	ChangeBoss();			// 단장 이임
	void	AddViceBoss();			// 부단장 임명
	void	DelViceBoss();			// 부단장 해임
	void	MemberFire();			// 단원 퇴출
	
	//void	ApplicantReject();		// 가입 거부

public:
	CUIGuild();
	~CUIGuild();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Open guild
	void	OpenGuild( int iMobIndex, BOOL bHasQuest, int iUserRanking, int iGuildLevel );
	void	OpenGuildManager( int iUserRanking );
	void	ResetGuild();
	void	ClearMemberList();

	void	JoinGuild( LONG lGuildIndex, LONG lChaIndex, const CTString& strName, INDEX iSyndiType );		// 길드 가입
	void	QuitGuild();		// 길드 탈퇴

	// 멤버 목록 관리
	void	AddToMemberList( LONG lIndex, const CTString& strName, int eLevel = GUILD_MEMBER_MEMBER, BOOL bOnline = TRUE );		// 멤버 목록에 추가
	void	DelFromMemberList( LONG lIndex );
	void	ChangeMemberLevel( LONG lIndex, int eLevel );
	void	ChangeMemberOnline( LONG lIndex, BOOL bOnline );

	// 멤버 목록 갱신
	// NOTE : 인터페이스를 열때는 반드시 갱신되어야 하는 부분이므로 TRUE로 설정하고,
	// NOTE : 보이는 상태일때만 갱신하고자 한다면 FALSE로 설정할것.
	void	RefreshMemberList( BOOL bInit = FALSE );
	void	RefreshApplicantList( BOOL bInit = FALSE);

	// 지원자에 대한 처리
	void	ApplicantAccept( LONG lIndex );		// 가입 승인

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	WMSG_RESULT	KeyMessage( MSG *pMsg );
	WMSG_RESULT	IMEMessage( MSG *pMsg );
	WMSG_RESULT	CharMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );

	// WSS_NEW_GUILD_SYSTEM 070702
	// 불필요해서 주석 처리
	// Set focus
//	void	SetFocus( BOOL bVisible );
			
	// Edit box focus
	BOOL	IsEditBoxFocused();
	void	KillFocusEditBox();

	// For using name change card  060120
	void	SetMemberName(int charIndex,CTString strName) ;
	
	int GetGuildMemberCount() const 
	{
		return m_vectorMemberList.size();
	}

	int GetUserRanking()
	{
		return m_iUserRanking;
	}

	int GetGuildLevel()
	{
		return m_iGuildLevel;
	}

	// WSS_NEW_GUILD_SYSTEM 070716 -------------------------------------->>
	void	InitNewGuildSystem();
	void	CreateNew( CUIWindow *pParentWnd, int nX, int nY);
	void	OpenGuildManagerNew();
	
	void	RenderNew();
	void	RenderNewGuildInfo(int nx,int ny);
	void	RenderNewGuildMemberInfo(int nx,int ny);
	void	RenderNewGuildNoticeInput();
	void	RenderNewGuildManage(int nx,int ny);
	void	RenderNewGuildManagePopup();
	void	RenderNewGuildBoard(int nx,int ny);
	void	RenderNewGuildBoardList();
	void	RenderNewGuildBoardRead();
	void	RenderNewGuildBoardWrite();	
		
	// 길드 멤버 정보 추가
	void	AddGuildMemberInfo(clsGuildMemberNew tMemberInfo);
	void	AddGuildManageInfo(clsGuildMemberNew tMemberInfo);
	CTString SetPosName(CTString tName);

	// 길드 게시판
	void	GetBoardListContent();
	void	GetBoardReadContent();	

	// 길드 관리 설정 관련 전체 버튼 세팅
	void	SetManagePopup(BOOL bEnable);
	void	ResetManagePopupString();
	// 설정 수정 항목 체크
	BOOL	CheckDataValidation();
	
	// Message Send
	void	SendRequestGuildTab(int iTabNum);
	void	SendChangeGuildInclination(UBYTE ubIncline);
	void	SendAdjustGuildMemberInfo( int charIdx, CTString posName ,int expPer, int famePer, int corps = -1, UBYTE ubStashAuth = 0 );
	void	SendUpdateGuildNotice( CTString strTitle, CTString strContents);
	void	SendRequestGuildNotice();	
	void	SendLearnGuildSkill(int skillIdx); 
	// Message Receive
	void	ReceiveNewGuildMessage(UBYTE ubType,CNetworkMessage* istr);
	void	UpdateGuildSkillLearnMessage(UpdateClient::doNewGuildSkillLearnToMemberMsg& SkillInfo);

	// Mouse Message
	WMSG_RESULT	MouseMessageNew( MSG *pMsg );
	WMSG_RESULT	MouseMessageNewBoard( MSG *pMsg );

	// Encode Notice String
	// \r\n MultiBox에 사용으로 스트링 함수 수정
	void EncodeNoticeString(CTString& tStr);
	void DecodeNoticeString(CTString& tStr);

	// Adjust Guild Member Info 
	void AdjustGuildMemberInfo();
	// Guild Skill Setup
	void SetGuildSkillList();
	// Guild Skill Check
	ENGINE_API int GetGuildSkillLevel(int skillIdx);
	// Set Guild Skill Member Extend
	void ResetGuildMaxMember(int guildSkillLevel);
	// Set Guild Position Name
	CTString ResetPosName(CTString tPosName,int ulPosition);

	void RenderNewGuildSkillExtend(int nX,int nY);			// 확장 길드 스킬창 렌더
	
	void ReceiveGuildSkillExtend(CNetworkMessage* istr);	// 확장 길드 스킬 정보 받기 (Server->Client) : (int)skill_type, (int)count, (int)skill_index, (int)skill_level
	
	void SetGuildSkillInfo();								// 확장 길드 스킬 창 사용 정보, 습득 정보 설정
	void SetSkillPopupInfo(int nTab);

	int	GetGuildSkillCount();								// 길드 스킬 총 갯수 얻기
	CUIIcon* GetSkillButton(int nPos);					// 길드 스킬 버튼 얻기
	CSkill* GetGuildSkill(int nPos);						// (스킬창에서 클릭 시 포지션으로) 길드 스킬 얻기
	BOOL CheckUseGuildSkill(int nSkillIndex);				// 길드 스킬 사용 조건 체크
	BOOL StartGuildSkillDelay( int nSkillIndex );			// 길드 스킬 딜레이 설정
	INDEX GetSelectedPositon();								// 콤보박스에서 선택된 부대 정보 얻기
	void SetSkillBtnInfo();									// 길드 스킬 버튼 정보 설정
	BOOL GetSkillDelay( INDEX index );						// 스킬 딜레이 얻기
	DOUBLE GetReuseTime( INDEX SkillIndex );				// 재사용시간 얻기
	DOUBLE GetCoolTime( DOUBLE ReuseTime, DOUBLE StartTime );	// 쿨타임 얻기
	void SetCorpsInfo( INDEX memberCount, INDEX bossCount );	// 길드의 부대 정보 설정
	void InitString();										// 부대 관련 스트링 설정
	void SetCorpsComboBox();								// 부대 설정 콤보 박스 설정
	void ReceiveCorpsInfo(CNetworkMessage* istr);			// 길드 부대 정보 받기
	// --------------------------------------------------------------------<<

	void		SetRemoteGuildJoinTaget(CTString &strTargetName)		{ m_strRemoteGuildJoinTaget = strTargetName;	}
	CTString	GetRemoteGuildJoinTaget()								{ return m_strRemoteGuildJoinTaget;				}
	void		ArrangeMemList(const int eType);
	CUICheckButton* m_pCbMemberArrange[eGML_MAX];
	BOOL		IsOnline(CTString strName);
	int			GetMemberNetIdx( CTString strName );
	WMSG_RESULT OpenGuildPop(int nMemberIdx, int nX, int nY);

	void		ResetArrangeState();
	void		ClearGuildSkill();
	void		ClearGuildSkillCooltime();
};

#endif	// UIGUILD_H_