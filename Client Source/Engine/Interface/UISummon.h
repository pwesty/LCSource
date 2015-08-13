// ----------------------------------------------------------------------------
//  File : UISummon.h
//  Desc : Created by Dongmin
// ----------------------------------------------------------------------------

#ifndef	UISUMMON_H_
#define	UISUMMON_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


// #include <Engine/Interface/UIWindow.h>
// #include <Engine/Interface/UIButton.h>


// Define position
#define	SUMMON_NAME_SY				5


// Define size of player information
#define	SUMMON_WIDTH				178
#define	SUMMON_HEIGHT				44
#define	SUMMON_BAR_WIDTH			107

#define SUMMON_START_X_GAP			105 // ui플레이어 인포의 초상화 부분 부터 시작 해야 되므로 초상화의 넓이를 수동으로 가지고 있는다.

class CEntity;

// ----------------------------------------------------------------------------
// Name : CUISummon
// Desc :
// ----------------------------------------------------------------------------
class CUISummon : public CUIWindow
{
protected:
	// Region of each part
	UIRect				m_rcTitle;						// Title
	UIRect				m_rcHP;							// HP
	UIRect				m_rcHPBack;						// Background of HP
	UIRect				m_rcTime;						// Time
	UIRect				m_rcTimeBack;					// Background of Time
	UIRect				m_rcSummonImage;
	UIRect				m_rcSkill;
	UIRect				m_rcToolTip;

	// UV of each part
	UIRectUV			m_rtBackground1;				// Background	
	UIRectUV			m_rtHP;							// HP
	UIRectUV			m_rtHPBack;						// Background of HP	
	UIRectUV			m_rtTime;						// Time
	UIRectUV			m_rtTimeBack;					// Background of Time

	UIRectUV			m_rtInfoL;						// UV of left region of information
	UIRectUV			m_rtInfoM;						// UV of middle region of information
	UIRectUV			m_rtInfoR;						// UV of right region of information

	UIRectUV			m_rtSummon[5];
	CUIButton			m_btnCommand[4];
	
	CUIIcon*			m_pIconSkill;
	
	int					m_nSummonType;
	INDEX				m_nOldCommand;
	INDEX				m_nCurrentCommand;

	BOOL				m_bShowToolTip;							// If tool tip is shown or not
	CTString			m_strToolTip;							// String of tool tip
	
	CEntity*			m_penEntity;
	int					m_nIndex;	
	int					m_nLeftTime;					// 남은 시간
	int					m_nMaxTime;						// 최대 시간
	TIME				m_tmLeftTime;					// 타이머 용 임시 변수
	BOOL				m_bSetMaxTime;					// 최대 시간 설정 유무.

	const	int			m_nUIIndex;
	
protected:
	// Update info of target
	void	UpdateHPInfo();
	void	UpdateTimeInfo();	

	void	AddSkill( int iSkillIndex );
	void	UseSkill( int nIndex );	
	void	ShowToolTip( BOOL bShow, int nToolTipID = -1 );

public:
	CUISummon( int nUIIndex );
	~CUISummon();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();	

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	void	SetSummonType( int iType );
	ENGINE_API void	SetCommand( INDEX iCommand );	
	ENGINE_API INDEX&	GetCommand();
	void	ResetSummon();
	void	WarpOfResetSummon();

	BOOL	StartSkillDelay( int nIndex );

	void	SetSummonEntity( CEntity* pEntity )		{ m_penEntity = pEntity; }
	ENGINE_API CEntity*	GetSummonEntity()			{ return m_penEntity;	}

	void	SetSummonIndex( int iIndex )			{ m_nIndex = iIndex;	}
	int		GetSummonIndex()	const				{ return m_nIndex;		}	

	void	SetLeftTime( int nLeftTime );
	void	SetMaxTime( int nMaxTime );

	void	UseSkill();
};


#endif	// UISUMMON_H_

