// ----------------------------------------------------------------------------
//  File : UIAutoHelp.h
//  Desc : Created by cpp2angel
// ----------------------------------------------------------------------------

#ifndef	UIAUTOHELP_H_
#define	UIAUTOHELP_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#define AUTOHELO_SHOW_TIME			8000			// (m sec) 지역이름이 표시 되는 시간 

#if defined(G_BRAZIL)
	#define INTERVAL_TIME				30000			// 자동 도움말이 무시 되는 시간
#else
	#define INTERVAL_TIME				15000			// 자동 도움말이 무시 되는 시간
#endif

#define RND_HELP_TIME				20000

#define RND_HELP_LEVEL_LOW			1	
#define RND_HELP_LEVEL_HIGH			5
#define RND_HELP_LEVEL_MAX			100				// 070820_ttos: 브라질 요청 레벨 제한 없음

// Define notice delay time
#ifdef	IMPROV1107_NOTICESYSTEM
#define	CHATMSG_NOTICE_DELAY		3000			// milliseconds
#define	CHATMSG_NOTICE_FADEOUT		2000
#else
#define	CHATMSG_NOTICE_DELAY		8000			// milliseconds
#define	CHATMSG_NOTICE_FADEOUT		7000
#endif
#define	CHATMSG_NOTICE_FADETIME		( CHATMSG_NOTICE_DELAY - CHATMSG_NOTICE_FADEOUT )
#define CLASSIFICATION_TIME			60*60*1000			//등급표시 시간 1시간마다 


// ----------------------------------------------------------------------------
// Name : _SAutoHelpInfo
// Desc : struct
// ----------------------------------------------------------------------------
struct _SAutoHelpInfo
{
	CTString	m_strMessage;		// 출력 메세지
	int			m_iEndLevel;		// 레벨 제한 
	int			m_iStartLevel;		// 레벨 제한 
	int			m_Kindred;			// 적용 종족
	BOOL		m_bJustOnce;		// 한번만 하는 겁니다.
	BOOL		m_bActive;			// 활성화 중입니다.


	void Clear ()
	{
		//SetInfo ( CTString (""), -1, -1, -1, FALSE );
		m_iEndLevel		= -1;			// 레벨 제한 
		m_iStartLevel	= -1;
		m_Kindred		= -1;			// 적용 종족
		m_bJustOnce		= FALSE;
		m_bActive		= FALSE;
		m_strMessage.Clear();
	}

	void SetInfo ( CTString strMessage, int iStartLevel = -1, 
					int iLevel = -1, int Kindred = -1, BOOL bJustOnce = FALSE )
	{
		m_strMessage	= strMessage;		// 출력 메세지
		m_iEndLevel		= iLevel;			// 레벨 제한 
		m_iStartLevel	= iStartLevel;
		m_Kindred		= Kindred;			// 적용 종족
		m_bJustOnce		= bJustOnce;
		m_bActive		= FALSE;
			
	}
	
	bool CheckInfo ();
};

typedef struct _SGMNotice
{
	CTString	strGMNotice;
	COLOR		colGMTextColor;
	__int64		i64GMNoticeTime;
	__int64		i64GMFadeTime;
	BOOL		bCompleteVisible;
	UIRect		rcBackground;

}SGMNotice;

// ----------------------------------------------------------------------------
// Name : AUTO_HELP_INDEX
// Desc : enum
// ----------------------------------------------------------------------------
enum AUTO_HELP_INDEX
{
	AU_MOB_KILL,			// 몬스터를 죽였을 때
	AU_ITEM_DROP,			// 아이템이 떨어졌을 경우
	AU_PICK_ITEM ,			// 바닥에 떨어진 아이템을 주었을 때
	AU_GET_SKILL_POINT,		// 숙력도를 얻을 때
	AU_MOVE_DRATAN,			// 드라탄으로 이동
	
	
	AU_LEVEL_UP,			// 레벨업
	AU_LEVEL_UP_QUEST2,		// Quest ...
	AU_LEVEL_UP_QUEST8,
	AU_LEVEL_UP_QUEST9,
	AU_LEVEL_UP_QUEST10,
	AU_LEVEL_UP_QUEST16,
	AU_LEVEL_UP_QUEST24,
	AU_LEVEL_UP_STRUP,
	
	AU_RANDOM_HELP_START,						// 13
	AU_RANDOM_HELP = AU_RANDOM_HELP_START, 
	AU_RANDOM_HELP_END = 31,					// 29
	
	// wooss 060328 add thai_auto_help
	AU_NO_TOXICOSIS_S = 32,							// 3시간마다 한번씩 휴식을 취할 것을 권장
	AU_NO_TOXICOSIS_E = 35,
	AU_NO_TOXICOSIS_VACATION_S = 36,					// 방학중 하루종일 게임만 하는 유저들을 선도하는 내용
	AU_NO_TOXICOSIS_VACATION_E = 39,

	AUTOHELP_COUNT,			
};

#define		RANDON_STRING_COUNT		1

// ----------------------------------------------------------------------------
// Name : CUIAutoHelp
// Desc : class
// ----------------------------------------------------------------------------
class ENGINE_API CUIAutoHelp
{
protected:
	CTextureData	*m_ptdBaseTexture;					// Texture of window
	CTextureData	*m_ptdClassification;				// 게임 등급 표시

	BOOL			m_bStop;							// 정지 상태인감?
	float			m_fShowTime;						// 보여 지고 있는 시간
	__int64			m_tmStartTime;						// 보이기 시작 한 시간
	
	__int64			m_tmEndTime;						// 보이고 난 후 저장 된 시간 
	__int64			m_tmClassification;					// 등급표시 저장시간
	__int64			m_tmCurTime;						// 등급표시 관련 현재 시간

	// Additional autohelp for thai - wooss 060328 ------------------------------->>
	__int64			m_tmCheckTime;						// time check for thai
	__int64			m_bChecked;							// time check for thai
	int				m_iNoticeTime;
	// ---------------------------------------------------------------------------<<

	BOOL			m_bVisible;							// 화면에 보여 주고 있는가?
	int				m_nActiveIndex;						// 현제 보여 주고 있는 도움말 Index

	_SAutoHelpInfo	m_AutoHelpInfo[AUTOHELP_COUNT];
	BOOL			m_bCondition[AUTOHELP_COUNT];

	UIRect			m_rcNotice;							// Region of notice
	UIRect			m_rcGMNotice;						// Region of notice
	TIME			m_tmGMNoticeTime;					// 보이기 시작 한 시간

#if	defined(IMPROV1107_NOTICESYSTEM)
	std::vector<SGMNotice>	m_vGMNotice;
	BOOL					m_bmovGMNotice;
	__int64					m_i64movGMNotice;
#endif
	CTString		m_strGMNotice;
	COLOR			m_colGMTextColor;
	BOOL			m_bShowGMNotice;
	BOOL			m_bShowClassification;				//등급표시
	UIRect			m_rcClassification;
	UIRectUV		m_rtClassification;					//등급표시
	
	int				m_nKillMobIndex;
	BOOL			m_RndHelp;

#if		!defined(_UI_LIB_RENEWAL_)
	UIRectUV		m_rtNoticeL;						// UV of notice left region
	UIRectUV		m_rtNoticeC;						// UV of notice center region
	UIRectUV		m_rtNoticeR;						// UV of notice right region
#else	// _UI_LIB_RENEWAL_
	CUIImage*		m_pImgNotice[3];					// left, center, right
#endif	// _UI_LIB_RENEWAL_

public:

	CUIAutoHelp();
	~CUIAutoHelp();

	void Clear();
	
	// Create
	void Create();

	// Render
	void Render();
	void RenderGMNotice ();
	void ClassificationShowRender();

	void ShowAutoHelp ( int nIndex );
	void SetInfo ( DWORD dwAutoHelpInfo );
	
	void SetGMNotice ( CTString strGMNotice, COLOR colText = 0xFFBA13FF );
	COLOR GetGMNoticeColor(){ return m_colGMTextColor;	}
	BOOL IsShowGMNotice()	{ return m_bShowGMNotice;	}
	void ClearGMNNotice();

	void SetKillMonIndex ( int nIndex )
	{
		m_nKillMobIndex = nIndex;
	}

	int	GetKillMonIndex () const 
	{
		return m_nKillMobIndex;
	}

	void GetLvGuidString(int lv);
	void LearnTheSkill(int lv);
	void SetStatpoint(int old, int cur);
	int  GetUpStatpoint()	{ return m_nUpStat;	}
	void initialize();

private:
	int m_nUpStat;
};

ENGINE_API extern CUIAutoHelp*	_UIAutoHelp;

#endif	// UISINGBOARD_H_

