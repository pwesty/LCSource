// ----------------------------------------------------------------------------
//  File : UITargetInfo.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UITARGETINFO_H_
#define	UITARGETINFO_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


// #include <Engine/Interface/UIWindow.h>


// Define position
//#define	TARGETINFO_NAME_SY				5
#define	TARGETINFO_NAME_SY				2


// Define size of player information
//#define	TARGETINFO_WIDTH				140
//#define	TARGETINFO_HEIGHT				43
#define	TARGETINFO_WIDTH				197		// [2010/09/08 : Sora] 75 + 67 + 55 = 197임
#define	TARGETINFO_HEIGHT			57
#define	TARGETINFO_BAR_WIDTH			120

#define TARGETINFO_MAX_INFO_LINE 5


// ----------------------------------------------------------------------------
// Name : CUITargetInfo
// Desc :
// ----------------------------------------------------------------------------
class CUITargetInfo : public CUIWindow
{
protected:
	// Information
#ifdef NEW_CHAO_SYS
	COLOR				m_colNameColor[19];				// Colors of target name
#else
	COLOR				m_colNameColor[13];				// Colors of target name
#endif

	COLOR				m_colCurNameColor;				// Current color of target name
	BOOL				m_bShowBuff;					// If buff of target is shown or not
	BOOL				m_bShowPkHp;					// WSS_PK_TARGET_HP 070726

	// Region of each part
	UIRect				m_rcTitle;						// Title
	UIRect				m_rcHP;							// HP
	UIRect				m_rcHPBack;						// Background of HP
	UIRect				m_rcInfoMark;
	UIRect				m_rcLv;							// Level mark

	// UV of each part
	UIRectUV			m_rtBackground;					// Background
	UIRectUV			m_rtBackground_new_L;			// Background
	UIRectUV			m_rtBackground_new_M;			// Background
	UIRectUV			m_rtBackground_new_R;			// Background
	UIRectUV			m_rtNPCBackground;

	// [091111: selo] 몹 정보도 파티 몹처럼 표시한다
	UIRectUV			m_rtMobBackground_L;
	UIRectUV			m_rtMobBackground_M;
	UIRectUV			m_rtMobBackground_R;

	// npc new background tab add [8/30/2010 rumist]
	UIRectUV			m_rtNPCBackground_L;
	UIRectUV			m_rtNPCBackground_M;
	UIRectUV			m_rtNPCBackground_R;

	// 속성 관련
	UIRectUV			m_rtAttributeIconAtt[eICON_ATTR_MAX];
	UIRectUV			m_rtAttributeIconDef[eICON_ATTR_MAX];
	UIRect				m_rcAttributeIconAtt;
	UIRect				m_rcAttributeIconDef;
	
	BOOL				m_bShowAttrIcon;
	
	UBYTE				m_ubAttIdx; // 현재 공격속성 인덱스
	UBYTE				m_ubAttLv; // 현재 공격속성 레벨
	UBYTE				m_ubDefIdx; // 현재 방어속성 인덱스
	UBYTE				m_ubDefLv; // 현재 공격속성 레벨

	CTextureData*		m_ptdAttributeTexture;// 속성 시스템

	FLOAT				m_fHPOffset;
	UIRectUV			m_rtMobBackground;
	UIRectUV			m_rtHP;							// HP
	UIRectUV			m_rtHPBack;						// Background of HP
	UIRectUV			m_rtInfoMarkUV;
	UIRectUV			m_rtLvUV;

	// Tool Tip	
	UIRectUV				m_rtInfoUL;								// UV of upper left region of information
	UIRectUV				m_rtInfoUM;								// UV of upper middle region of information
	UIRectUV				m_rtInfoUR;								// UV of upper right region of information
	UIRectUV				m_rtInfoML;								// UV of middle left region of information
	UIRectUV				m_rtInfoMM;								// UV of middle middle region of information
	UIRectUV				m_rtInfoMR;								// UV of middle right region of information
	UIRectUV				m_rtInfoLL;								// UV of lower left region of information
	UIRectUV				m_rtInfoLM;								// UV of lower middle region of information
	UIRectUV				m_rtInfoLR;								// UV of lower right region of information
	

	BOOL					m_bShowInfo;
	BOOL					m_bSyndicateTooltip;

	UIRect					m_rcInfo;
	int						m_nCurInfoLines;

	CTString				m_strInfo[TARGETINFO_MAX_INFO_LINE];		// information string
	COLOR					m_colInfo[TARGETINFO_MAX_INFO_LINE];		// Color of information string

	UIRect				m_rcSyndicateMark;
	CUIRectSurface		m_rsSyndicateMark;
	CTString			m_strSyndicateType;
	CTString			m_strSyndicateGrade;
protected:
	// Update info of target
	void	UpdateHPInfo();
	void	SetTargetSyndicateTooltip();

	// 현재 타겟의 속성 인덱스 체크
	void	CheckAttrIdx();
	void	SetTargetAttrTooltip(UBYTE ubAttr, UBYTE ubAttrLv, int nStringID);
	void	UpdateShowInfo();
public:
	CUITargetInfo();
	~CUITargetInfo();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();
	void	RenderInfo();

	// Buff
	void	SetTargetBuff( BOOL bMe );
	// WSS_PK_TARGET_HP 070726--------------------->>
	// Show Target Character HP
	inline void	ShowPKTargetHP( BOOL bOn) { m_bShowPkHp = bOn; }
	// --------------------------------------------<<

	// Get color of name
	COLOR	&GetNameColor( int nIndex ) { return m_colNameColor[nIndex]; }

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	BOOL	CloseWindowByEsc();

	void	RenderAttribute(CDrawPort* pDrawPort);
};


#endif	// UITARGETINFO_H_

