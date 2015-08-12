// ----------------------------------------------------------------------------
//  File : UIAD.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UIAD_H_
#define	UIAD_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UICheckButton.h>
#include <Engine/Interface/UIEditBox.h>

// Define size of skill learn
#define	AD_WIDTH		103
#define	AD_HEIGHT		52

// Logo Count
#define AD_LOGO_CNT		2

enum ANI_STATE {
	ANI_IDLE,
	ANI_APPEAR,
	ANI_APPEAR_ON,
	ANI_APPEAR_END,
	ANI_DISAPPEAR,
	ANI_DISAPPEAR_ON,	
	ANI_END,
	
};

enum ANI_TYPE {
	SLOW_APPEAR,
};

// ----------------------------------------------------------------------------
// Name : CUIAD
// Desc :
// ----------------------------------------------------------------------------
class CUIAD : public CUIWindow
{
protected:

	CTextureData			*m_ptdNakoTitle;			// Texture of window
	
	// Rect AD.tex
	UIRectUV				m_uvAD[AD_LOGO_CNT];					// uv of advertisement 1
	UIRect					m_rcAD[AD_LOGO_CNT];					// rect of advertiesment 1
	UIRect					m_rcAD_ani;								// rect of ani

	// ani state
	ANI_STATE				m_eAniState;	
	ANI_TYPE				m_eAniType;

	// increase
	float					m_fDelay;

	// play ani num
	INDEX					m_selAni;

	// play list queue
	std::vector<INDEX>		m_listQ;

	// m_delayTime			
	INDEX					m_startT;
	INDEX					m_delayT;

public:
	CUIAD();
	~CUIAD();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );	

	// PlayLC
	void PlayLC(ANI_TYPE aniType);
	
	// Select Logo 
	RESULT	SelectLogo();

	inline INDEX TimeGap() { return _pTimer->GetHighPrecisionTimer().GetSeconds() - m_startT; }
	
	inline void AddListQ(INDEX Idx) { m_listQ.push_back(Idx) ; }
	inline void ClearListQ() { m_listQ.clear() ; }

protected:
	void OnRender( CDrawPort* pDraw );
	void OnUpdate( float fElapsedTime );
};


#endif	// UIAD_H_

