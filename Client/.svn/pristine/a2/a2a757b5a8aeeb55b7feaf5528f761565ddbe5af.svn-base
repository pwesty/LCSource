
// ----------------------------------------------------------------------------
//  File : UISelectResource.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	GUILDWARPORTAL_H_
#define	GUILDWARPORTAL_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// #include <Engine/Interface/UIButton.h>
// #include <Engine/Interface/UIEditBox.h>
// #include <Engine/Interface/UIListBox.h>
// #include <map>
// #include <Engine/Effect/CEffectGroupManager.h>
// #include <Engine/Effect/CEffectGroup.h>

// Define text position
#define	GUILDWARPORTAL_TITLE_TEXT_OFFSETX		25
#define	GUILDWARPORTAL_TITLE_TEXT_OFFSETY		5

// Define size of skill learn
#define	GUILDWARPORTAL_WIDTH		171
#define	GUILDWARPORTAL_HEIGHT		214

// ----------------------------------------------------------------------------
// Name : CUIGuildWarPortal
// Desc :
// ----------------------------------------------------------------------------
class CUIGuildWarPortal : public CUIWindow
{
public:

protected:
	CUIButton				m_btnClose;						// Close button

	// Region of each part
	UIRect					m_rcTitle;						// Region of title bar

	// UV of each part
	UIRectUV				m_rtBackTop;					// UV of top background
	UIRectUV				m_rtBackMiddle1;				// UV of middle background
	UIRectUV				m_rtBackMiddle2;				// UV of middle background
	UIRectUV				m_rtBackBottom;					// UV of bottom background	

	int						m_iClientNPCIndex;
	
	std::vector<CUIButton>	m_vectorResourceList;
	int						m_bPrePortal;					// 이동전인 상태 
	int						m_nPortal;						// 

	LONG					m_lLeftTime;				// 공성 진행 시간 
	TIME					m_tmLeftTime;				// 공성 남은 시간 

	int m_nNpcType;
	std::map<SLONG, CEffectGroup *> m_mapEG;

	// WSS_DRATAN_SEIGEWARFARE 2007/08/07
	int						m_btNpcAvailable;			// 드라탄 공성 사용 가능 타워(bit flag)
	int						m_nSelPotalIdx;
		
public:
	CUIGuildWarPortal();
	~CUIGuildWarPortal();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void	PortalCancel(  BOOL bMessage = TRUE );	// 워프 중인 상태를 취소한다.
	void	PrePortal( int nPotal );
	void	ResetGuildWarPortal();
	
	// Open
	ENGINE_API void	OpenGuildWarPortal( int npcIndex );	
	
	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );	

	// WSS_DRATAN_SEIGEWARFARE 2007/08/07
	ENGINE_API void	OpenGuildWarPortalDratan();
	void SetDratanPortalFlag(int npcIndex );
	ENGINE_API inline void SetPortalIdx(int npcIdx) { m_nSelPotalIdx = npcIdx; }
};


#endif	// GUILDWARPORTAL_H_
