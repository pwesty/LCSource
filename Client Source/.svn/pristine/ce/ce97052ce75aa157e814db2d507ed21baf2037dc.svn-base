// ***************************************************************
//  UIFortune   version:  1.0   ? date: 2013/02/13
//  -------------------------------------------------------------
//	Author 	: sykim70
//  Desc	: Fortune System.
//  -------------------------------------------------------------
//  Copyright (C) 2013 - All Rights Reserved
// ***************************************************************

#ifndef UIFORTUNE_H_
#define UIFORTUNE_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIImageBox.h>

class CUIFortune : public CUIWindow
{
public:
	enum
	{
		FORTUNE_WIDTH	= 410,
		FORTUNE_HEIGHT	= 410,
		MAX_CARD = 7,
	};

protected:
	// surface.
	CUIRectSurface m_rtsBase;

	// title.
	UIRect m_rtTitle;
	CTString m_strTitle;
	// button.
	CUIButton m_btnClose;
	CUIButton m_btnOK;
	
	UIRect m_rtHighlightView;
	UIRectUV m_rtHighlight;
	
	CUIImageBox m_imgCard[MAX_CARD];

	CUIListBox m_lbDescription;

	// flags.
	BOOL m_bPickTitle;

	// item data
	SWORD m_nTab;
	SWORD m_nIdx;

	bool m_bSlow;
	DWORD m_delta;
	DWORD m_prevTick;
	int m_offset;
	bool m_bMove;

	int m_nFortuneSkillCount;
	std::vector<stFortune> m_vecFortuneSkill;

	int m_nFortuneIndex;

public:
	CUIFortune();
	~CUIFortune();

	//////////////////////////////////////////////////////////////////////////
	// overriding func. (virtual func.)
	// create.
	void Create(CUIWindow* pParentWnd, int nX, int nY, int nWidth, int nHeight);
	// Render.
	void Render();
	// Adjust Position.
	void ResetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ);
	void AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ);
	// Message pump.
	WMSG_RESULT MouseMessage(MSG* pMsg);
	BOOL CloseWindowByEsc();
	// Message Handler.
	void MsgBoxCommand(int nCommandCode, BOOL bOK, CTString& strInput);

	//////////////////////////////////////////////////////////////////////////
	// User Definition func.
	void OpenFortune(int nTab, int inven_idx);
	void ToggleVisible();
	void OpenFortune();
	void CloseFortune();
	void ResetFortune();
	void SetFortune(int nItemIndex, int nSkillIndex, int nSkillLevel);
};

#endif //UIFORTUNE_H_
