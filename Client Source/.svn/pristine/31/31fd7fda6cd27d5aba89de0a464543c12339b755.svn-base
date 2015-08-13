// ***************************************************************
//  UIResurrection   version:  1.0   ? date: 28/10/2009
//  -------------------------------------------------------------
//	Author 	: rumist
//  Desc	: Player Resurrection Scroll.
//  -------------------------------------------------------------
//  Copyright (C) 2009 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef __UIRESURRECTION_H__
#define __UIRESURRECTION_H__

#ifdef PRAGMA_ONCE
	#pragma once
#endif

class CUIResurrectionMsgBox : public CUIWindow
// : public CUIMessageBox
{
public:

// attribute
protected:
	// surface.
	CUIRectSurface			m_rtsBase;
	// resurrection base surface
	UIRectUV				m_rtResurrectionBase;
	// UV of each part
	UIRectUV		m_rtBackTopL;					// UV of top background
	UIRectUV		m_rtBackTopM;					// UV of top background
	UIRectUV		m_rtBackTopR;					// UV of top background
	
	UIRectUV		m_rtBackMiddleL;				// UV of middle background
	UIRectUV		m_rtBackMiddleM;				// UV of middle background
	UIRectUV		m_rtBackMiddleR;				// UV of middle background

	UIRectUV		m_rtBackBottomL;					// UV of bottom background
	UIRectUV		m_rtBackBottomM;					// UV of bottom background
	UIRectUV		m_rtBackBottomR;					// UV of bottom background

	// resurrection title bar.
	UIRect					m_rcTitle;
	// resurrection base texture.
	CTextureData*			m_ptdButtonTexture;
	// buttons.
	CUIButton				m_btnOk;
	CUIButton				m_btnCancel;
	// tooltip box.
	CUIDrawBox				m_bxToolTip;
	// tooltip string.
	CTString				m_strToolTip;
	// put string.
	CTString				m_strMainMsg;
	// flags.
	BOOL					m_bShowToolTip;	

	// ¸¾¿¡ ¾Èµë. ¼öÁ¤ÇÒ ¿¹Á¤. [11/19/2009 rumist]
	CUIMultiLineText		m_msgText;
	__int64					m_nStartTime;
	__int64					m_nTime;					
	BOOL					m_bTimeOutBtnMessage;
	LONG					m_lResurrectionItemIndex;
public:
// functions.
protected:
public:
	//////////////////////////////////////////////////////////////////////////
	// constructor & destructor.
	CUIResurrectionMsgBox();
	virtual ~CUIResurrectionMsgBox();

	//////////////////////////////////////////////////////////////////////////
	// overriding functions.
	// create
	void			Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	// render
	void			Render();
	// adjust position.
	void			ResetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void			AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	
	// message procedure.
	WMSG_RESULT		MouseMessage(MSG *pMsg );
	
	//////////////////////////////////////////////////////////////////////////
	// custom functions.
	ENGINE_API void	OpenResurrectionMsgBox( INDEX nTime = 300, BOOL bDefault = FALSE );
	ENGINE_API void	CloseResurrectionMsgBox();

	const BOOL		IsCheckItemInInven( INDEX nItemIdx ) const;

	BOOL			IsOpenResurrectionUI() const;

	BOOL			SetMsgTimer( __int64 nTime, BOOL bTimeOutBtnMessage = TRUE );
	LONG			GetResurrectionItemID() {	return m_lResurrectionItemIndex;	}
};


#endif //__UIRESURRECTION_H__
