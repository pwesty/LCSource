#ifndef UIEVENT_POPUP_H_
#	define UIEVENT_POPUP_H_
#	ifdef PRAGMA_ONCE
#		pragma once
#	endif	// PRAGMA_ONCE

#include <Engine/Interface/UIWindow.h>
#include <Engine/Interface/UIRectString.h>

// [2011/01/18 : Sora] 출석 이벤트
class CUIEventPopUp : public CUIWindow
{
protected:

	enum eBoxPos
	{
		BOXPOS_TOPLEFT = 0,
		BOXPOS_TOPCENTER,
		BOXPOS_TOPRIGHT,
		BOXPOS_MIDLEFT,
		BOXPOS_MIDCENTER,
		BOXPOS_MIDRIGHT,
		BOXPOS_BOTLEFT,
		BOXPOS_BOTCENTER,
		BOXPOS_BOTRIGHT,
		BOXPOS_MAX,
	};


	CTextureData		*m_ptdNumberTexture;

	CTString			m_strTitle;

	int					m_nPercent;
	CUIRectString		m_rsNoticePopUpString;
	CUIRectSurface		m_rtNoticePopUpBack;

	void	SetBackGroundSize( int nWidth, int nHeight );
public:

	CUIEventPopUp();
	~CUIEventPopUp();

	BOOL	IsEmpty() { return m_rsNoticePopUpString.IsEmpty(); }
	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();


	void	SetPos(int nPosX, int nPosY);
	void	SetPosX( int nX );
	void	SetPosY( int nY );
	void	Move( int ndX, int ndY );

	void	InitPosition();

	void	SetTitle( CTString strTitle ) { m_strTitle = strTitle; }
	void	SetPercent( int percent ) { m_nPercent = percent; }
	void	SetNoticeMessage( CTString strNotice, COLOR color = 0xFFFFFFFF );
	void	ClearNoticePopUp();
};

#endif // UIEVENT_POPUP_H_
