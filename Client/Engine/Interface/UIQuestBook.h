// ----------------------------------------------------------------------------
//  File : UIQuestBook.h
//  Desc : Created by Dongmin
// ----------------------------------------------------------------------------

#ifndef	UIQUESTBOOK_H_
#define	UIQUESTBOOK_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// Define text position
#define	QUESTBOOK_TITLE_TEXT_OFFSETX		25 
#define	QUESTBOOK_TITLE_TEXT_OFFSETY		5

// Define size of message box
#define	QUESTBOOK_TOP_HEIGHT				26
#define	QUESTBOOK_DESC_HEIGHT				164
#define	QUESTBOOK_BOTTOM_HEIGHT				7
#define QUESTBOOK_WIDTH						311
#define	QUESTBOOK_HEIGHT					290

class CItems;

// ----------------------------------------------------------------------------
// Name : CUIQuestBook
// Desc :
// ----------------------------------------------------------------------------
class CUIQuestBook : public CUIWindow
{
public:
	struct sQuestInfo
	{
		sQuestInfo()
		{
			iQuestIndex		= -1;
			strQuestTitle	= CTString("");
			iQuestType		= -1;
			iQuestScale		= -1;
			dOldTime		= -1;			
			dRemainTime		= -1;
		}

		// [090708: selo] 비교 연산자 오버로딩
		inline const bool operator== (const sQuestInfo& rhs)
		{
			return iQuestIndex == rhs.iQuestIndex;
		}		
		// [090709: selo] 정렬위한 연산자 오버로딩
		inline const bool operator< (const sQuestInfo& rhs)
		{
			return iQuestIndex < rhs.iQuestIndex;
		}

		CTString	strQuestTitle;
		int			iQuestIndex;
		int			iQuestType;			// [090601: selo] 퀘스트 정렬 타입(일반, 시나리오, 외전, 나이트 쉐도우)
		int			iQuestScale;		// [090603: selo] 퀘스트 규모(일반, 파티, 원정대)
		DOUBLE		dOldTime;			// [090609: selo] 시간제한 퀘스트 한 프레임 전의 시간		
		DOUBLE		dRemainTime;		// [090609: selo] 시간제한 퀘스트 남은 시간(초)
	};

	struct FindQuest
	{
		int iQuestIndex;
		FindQuest(sQuestInfo& info) : iQuestIndex(info.iQuestIndex) {};
		bool operator() (const sQuestInfo& info)
		{
			if(info.iQuestIndex == iQuestIndex)
			{
				return true;
			}
			else
				return false;
		}
	};	

	const int				m_nUIIndex;

	int						m_nSelectedQuestIndex;					// 선택된 퀘스트 인덱스.
	CUIButton				m_btnClose;								// Close button
	CUIButton				m_btnOK;								// 확인
	CUIButton				m_btnCancel;							// 닫기
	CUIButton				m_btnGiveUp;							// 퀘스트 포기
	CUIButton				m_btnAccept;							// 수락
	CUIButton				m_btnReserve;							// 보류	
	CUIButton				m_btnDeny;								// 거절
	CUIButton				m_btnProceedPop;						// 파핑버튼
	CUIButton				m_btnCompletePop;						// 파핑버튼
	CUIButton				m_btnPrev;								// [090526: selo] 확장팩 기획에 따른 NPC 메뉴 리스트로 돌아가는 버튼
	CUIComboBox				m_cmbSort;								// [090601: selo] 퀘스트 리스트 정렬을 위한 콤보 버튼
	CUIListBoxEx			m_lbDescription;						// List box of description	
	CUIListBoxEx			m_lbQuestList;							// List box of Quest
	CTString				m_strTitle;								// Title of message box	
	BOOL					m_bTitleBarClick;						// If title bar is clicked or not

	// Region of each part
	UIRect					m_rcTitle;								// Region of title bar

	// UV of each part
	UIRectUV				m_rtTopL;								// UV of top background
	UIRectUV				m_rtTopM;								// UV of top background
	UIRectUV				m_rtTopR;								// UV of top background
	UIRectUV				m_rtMiddleScrollL;						// UV of middle background with scroll bar
	UIRectUV				m_rtMiddleScrollM;						// UV of middle background with scroll bar
	UIRectUV				m_rtMiddleScrollR;						// UV of middle background with scroll bar	
	UIRectUV				m_rtMiddleGapL;							// UV of middle gap background
	UIRectUV				m_rtMiddleGapM;							// UV of middle gap background
	UIRectUV				m_rtMiddleGapR;							// UV of middle gap background
	UIRectUV				m_rtBottomL;							// UV of bottom background
	UIRectUV				m_rtBottomM;							// UV of bottom background
	UIRectUV				m_rtBottomR;							// UV of bottom background
	UIRectUV				m_rtPopingExtended;
	UIRectUV				m_rtPopingClosed;

	std::vector<CUIButton>	m_vectorBtnDynamicPrize;				// 선택 보상	
	std::vector<sQuestInfo>	m_vectorProceedQuestList;
	std::vector<sQuestInfo>	m_vectorCompleteQuestList;
	std::vector<sQuestInfo>	m_vectorCategoryQuestList[4];			// [090603: selo] 카테고리 별로 퀘스트를 나눈 리스트
	std::vector<sQuestInfo> m_vectorRaidMessageList;				// [090708: selo] 레이드 퀘스트 리스트

	std::list<int>			m_listSelectedQuestIndex;				// [090602: selo] 선택되어 있는 퀘스트들의 인덱스 모음
	
	CItems*					m_pQuestItem;
	INDEX					m_nTargetIndex;
	INDEX					m_nTargetVirIndex;
	SBYTE					m_nTargetUIType;
	FLOAT					m_fNpcX, m_fNpcZ;
	int						m_nSelectPrizeFirstIndex;
	int						m_nSelectPrizeCount;
	BOOL					m_bProceedListExtended;
	BOOL					m_bCompleteListExtended;
	BOOL					m_bCategoryListExtended[QCATEGORY_SUPPLEMENT];	// [090603: selo] 카테고리별 리스트들이 펼쳐져 있는지 확인
	BOOL					m_bRaidMessageListExtended;				// [090708: selo] 레이드 퀘스트 리스트가 펼쳐져 있는지 확인
	int						m_iNextQuestIndex;
		
	// 2009. 05. 27 김정래
	// 확장팩 관련 NPC 메뉴로 돌아가기 위한 NPC Index 저장
	INDEX					m_nPrevNPCIndex;
	SBYTE				    m_nPrevNPCUIType;

	// 2009. 06. 01 김정래
	// 퀘스트 북 정렬 타입 (퀘스트 리스트 에서만 사용한다.)
	SBYTE					m_nSortType;

	// 2009. 06. 01 김정래
	// 퀘스트를 포기하는 동안 메시지 처리를 못 하게 하는 플래그
	BOOL					m_bLockQuestList;
	
	// [090818: selo]
	// 퀘스트 공지 시작 시간
	DOUBLE					m_dQuestNoticeStartTime;
	
	// [090820: selo]
	// 퀘스트 공지 두번 안나오게 하기 위해 
	// 이미 발생된 퀘스트 공지를 저장해 둔다	
	CStaticArray<int>		m_aAlreadyShowQuestNotice;
	
	// [090907: selo]
	// 레이드 시작, 끝 관련
	DOUBLE					m_dRaidRemainTime;	// 레이드 남은 시간
	BOOL					m_bIsRaidNow;		// 레이드 중 인지 판단 플래그
	DOUBLE					m_dRaidRemainTimeRecvTime;	// 레이드 남은 시간을 받은 시간
	
	CTextureData	*m_ptdCommonBtnTexture;

protected:
	void	AcceptQuest();				// 퀘스트 수락
	void	ReserveQuest();				// 퀘스트 보류
	void	DenyQuest();				// 퀘스트 거절
	void	GiveUpQuest();				// 퀘스트 포기
	void	PrizeQuest();				// 퀘스트 보상
	// 2009. 05. 26 김정래
	// 확장팩 관련 NPC 메뉴 리스트로 돌아오기
	

public:
	CUIQuestBook( int nUIIndex );
	~CUIQuestBook();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );	
	bool	AddDescString( CTString &strMessage, const COLOR colDesc, BOOL bSelectable = FALSE, BOOL bPrizeDesc = FALSE );	// [090812: selo] 보상 설명 처리 추가
	void	InitQuestBook();
	void	CloseQuestBook();
	void	OpenQuestBook( int iQuestIndex = -1, CItems* pQuestItem = NULL );	
	void	ToggleVisible();

	// 퀘스트 목록에 추가
	BOOL	AddToQuestList( int iQuestIndex, BOOL bComplete );

	// 퀘스트 목록에서 제거
	BOOL	DelFromQuestList( int iQuestIndex, BOOL bComplete );

	// 퀘스트 목록을 갱신합니다.
	void	RefreshQuestList();

	// 퀘스트 목록을 모두 지웁니다.
	void	ClearQuestList();

	// 퀘스트 내용을 갱신합니다.
	void	RefreshQuestContent(INDEX questIndex);
	
	INDEX	GetProceedQuestCount()		{ return m_vectorProceedQuestList.size();	}
	INDEX	GetCompleteQuestCount()		{ return m_vectorCompleteQuestList.size();	}
	INDEX	GetCurrentQuest()			{ return m_nSelectedQuestIndex;				}

	// 2009. 05. 27 김정래
	// 퀘스트 Index 정보 가져오기
	INDEX	GetProceedQuestIndex(INDEX index);
	INDEX	GetCompleteQuestIndex(INDEX index);	

	// 2009. 05. 28 김정래
	// NPC 메뉴로 돌아가기 위한 변수 관련
	void	SetPrevNPCIndex(INDEX index){ m_nPrevNPCIndex = index;					}
	INDEX	GetPrevNPCIndex()			{ return m_nPrevNPCIndex;					}
	void	SetPrevNPCUIType(SBYTE type){ m_nPrevNPCUIType = type;					}
	SBYTE	GetPrevNPCUIType()			{ return m_nPrevNPCUIType;					}


	//npc index 관련
	void	SetTargetIndex(INDEX index)	{ m_nTargetIndex = index;					}
	INDEX	GetTargetIndex()			{ return m_nTargetIndex;					}

	//quest 보류 관련
	void	SetNextQuestIndex(int index)	{ m_iNextQuestIndex = index;			}
	int		GetNextQuestIndex()				{ return m_iNextQuestIndex;				}

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	ResetSavePosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	// add large message box. [8/2/2010 rumist]
	void	MsgBoxLCommand(int nCommandCode, int nResult );
	// add open tutorial window [8/2/2010 rumist]
	void	OpenTutorialWindow();
		
	void	OpenWindow( BOOL bHasQuestList );


	//notice 정보
	void	RenderNotice();
	void	CreateNotice();
	void	SetNoticeInfo(const char *szString);
	
	ENGINE_API void	RequestQuest( int nNpcIndex, int nNpcVirIndex, SBYTE sbUIType, FLOAT fX, FLOAT fZ );
	void	SendQuestReq( int nNpcIndex );
	void	SendQuestCancel( int nQuestIndex );
	void	SendQuestStart( int nQuestIndex );
	void	SendQuestPrize( int nQuestIndex, INDEX iNpcIndex, INDEX iOptionItemIndex, INDEX iOptionItemPlus );

	// [090810: selo] 
	// 제목이 길어지면 ... 으로 변경하여 준다
	static CTString MakeTitleString(CTString strTitle, int iSplitPos);

	// [090708: selo] 
	// 레이드 퀘스트 관련 함수들
	void	AddRaidMessage(const int& iQuestIndex);
	void	RemoveRaidMessage(const int& iQuestIndex);
	void	RemoveRaidMessageAll(void);
	void	RefreshQuestContentByRaid(const int& iQuestIndex);
	bool	isRaidMessage(const int& iQuestIndex);

	// 2009. 05. 29 김정래
	// 확장팩 퀘스트 확장 관련 함수들 추가		
	void	CreateQuestListNew( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	RefreshQuestListNew();
	void	RenderQuestListNew();
	WMSG_RESULT	MsgProcQuestListNew( MSG *pMsg );

	// 2009. 06. 02 김정래
	// 전체화면에 표시하기 위해 체크해둔 퀘스트 관련 함수
	void	AddSelectedQuest(const int& iQuestIndex);
	void	RemoveSelectedQuest(const int& iQuestIndex);
	bool	IsSelectedQuest(const int& iQuestIndex);
	void	ClearSelectedQuestList();			// [090803 sora] 선택된 퀘스트 리스트 clear
	const std::list<int>& GetSelectedQuestList() { return m_listSelectedQuestIndex; }

	// 2009. 06. 03 김정래
	// 퀘스트 리스트 소트
	void	SortQuestListByCondition();
	void	SortQuestListByType();
	
	// 2009. 06. 03 김정래
	// 퀘스트가 완료 상황인지 판단
	bool	IsCompleteQuest(int nQuestIndex);
	
	// 2009 .06. 08 김정래
	// 타임어택 퀘스트 중 제일 먼저 수락 받은 퀘스트의 초시간을 반환한다
	DOUBLE	GetTimeAttackRemainTime();

	// 2009. 06. 09 김정래
	// 타임어택 퀘스트의 남은 시간을 설정한다.
	void	SetQuestRemainTime(int iQuestIndex, int iRemainTime);

	// [090818: selo]
	// 퀘스트 공지를 띄운 후 흐른 시간을 알려준다.
	DOUBLE	GetQuestNoticeElapsedTime();

	// [090907: selo]
	// 레이드 시작과 끝 처리	
	void	RaidEnd(void);
	void	SetRaidRemainTime(INDEX iRemainTime);
	DOUBLE	GetRaidReminTime(void);
	BOOL	IsRaidNow(void);

	static void TalkWithNPC();

	// 2009. 05. 27 김정래
	// 메시지 박스에 퀘스트 리스트를 추가하기
	static void	AddQuestListToMessageBoxL(const int& iMessageBoxType);

	// 2009. 05. 27 김정래
	// 퀘스트를 선택하여 선택 결과에 맞는 퀘스트 북을 열게 한다.
	static void SelectQuestFromMessageBox(const int& iClassifierIndex);
	// 퀘스트 type 정보 가져오기
	static SBYTE GetQuestType(const INDEX& iQuestIndex);

	// esc closing support [5/30/2011 rumist]
	BOOL	CloseWindowByEsc()						{ CloseQuestBook();	return TRUE;		}
};

#endif	// UIQUESTBOOK_H_

