// ----------------------------------------------------------------------------
//  File : UISignBoard.h
//  Desc : Created by cpp2angel
// ----------------------------------------------------------------------------

#ifndef	UISINGBOARD_H_
#define	UISINGBOARD_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// #include <Engine/Interface/UIWindow.h>

#define JUNO_COUNT					5			
#define DRATAN_COUNT				11			
#define ALL_SIGNBOARD_COUNT			DRATAN_COUNT + JUNO_COUNT

#define SHOW_TIME					5000
#define SIGNBOARD_SY				120


class ENGINE_API CUISignBoard
{
protected:
	BOOL		m_bStop;					// 정지 상태인감?
	int			m_nWidth;
	int			m_nHeight;

	CTString	m_strLocalName;				// 표시할 지역 이름 
	float		m_fShowTime;
	int			m_iAlpha;
	int			m_FadeInOp;
	int			m_FadeOutOp;
	int			m_op;
	TIME		m_tmStartTime;
	
	BOOL		m_bVisible;
	BOOL		m_bText;
	int			m_nTextureIndex;	

	UIRectUV	m_rtTexts[ALL_SIGNBOARD_COUNT];
	UIRectUV	m_rtZones[6];
	UIRectUV	m_rtCurrent;
	
	CTextureData	*m_ptdBaseTexture;			// Texture of window

	FLOAT		m_fTexWidth;
	FLOAT		m_fTexHeight;

public:
	CUISignBoard();
	~CUISignBoard();

	void Clear ();
	
	void Create( int iCountry );

	void ShowSingBoard ( int nIndex, SLONG slZoneIndex = -1 );
	void Render();

	BOOL LoadSignBoardTexture( int nZoneIndex, int nLocalIndex );

};

ENGINE_API void ShowSignBoard ( int nIndex );
ENGINE_API extern CUISignBoard*	_UISignBoard;


#endif	// UISINGBOARD_H_

