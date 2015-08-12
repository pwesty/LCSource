// ----------------------------------------------------------------------------
//  File : UIGuildMark.h
//  Desc : Created by sora
// ----------------------------------------------------------------------------

#ifndef	UIGUILDMARK_H_
#define	UIGUILDMARK_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIBase.h>
#include <Engine/Interface/UIWindow.h>
#include <Engine/Interface/UIButton.h>


#define		DEF_GM_PER_PAGE		24	// 한페이지에 표시되는 아이템수

class CUIGuildMarkIcon : public CUIBase
{
public:
	CUIGuildMarkIcon();
	~CUIGuildMarkIcon();

	void Create( CUIBase* pParent, int x, int y, int width, int height );
	void setTexture();
	CUIBase* Clone();
	

	void setZ(float fZ)		{ m_fZ = fZ; }

	void OnRender(CDrawPort* pDraw);

	void CalcUV(SBYTE row, SBYTE col, bool bFront);
	void RemoveData();
	bool GetSetting(bool bFront)	{ return bFront == true ? m_bSetFront : m_bSetBG; }

protected:

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);

private:
	bool	m_bSetBG;
	bool	m_bSetFront;
	float	m_fZ;
	UIRectUV m_UV;
	UIRectUV m_UVBG;

	CTextureData* m_ptdMarkBG;
};

class CUIGuildMark : public CUIWindow
{
protected:
	CTextureData		*m_ptdAddTexture;

	UIRect				m_rcTitle;
	CUIRectSurface		m_Background;
	CUIRectSurface		m_BtnTab;

	UIRectUV			m_rtSelectedBtn;

	CUIButton			m_btnInitGuildMark;
	CUIButton			m_btnRegisterGuildMark;
	CUIButton			m_btnCancel;
	CUIButton			m_btnGuildMarkPrev;
	CUIButton			m_btnGuildMarkNext;
	CUIButton			m_btnGuildMarkBGPrev;
	CUIButton			m_btnGuildMarkBGNext;

	int					m_selectedGuildMark;
	int					m_selectedGuildMarkBG;

	int					m_guildMarkPage;
	int					m_guildMarkBGPage;

	CUIGuildMarkIcon*			m_pIconMyGuildMark;
	CUIGuildMarkIcon*			m_pIconsGM[DEF_GM_PER_PAGE];
	CUIGuildMarkIcon*			m_pIconsGMBG[DEF_GM_PER_PAGE];

	void	CalcGuildMarkUV( CTextureData* textureData, SBYTE row, SBYTE col, UIRectUV* rectUV );
	void	SetGuildMarkPage( int page );
	void	SetGuildMarkBGPage( int page );
	void	ConfirmGuildMarkItem();
public:
	CUIGuildMark();
	~CUIGuildMark();

	void	InitGuidMark();
	void	OpenGuildMarkEdit();
	void	CloseGuildMarkEdit();
	BOOL	IsPopUpMsgExist();

	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	Render();
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	WMSG_RESULT	MouseMessage( MSG *pMsg );
};

#endif