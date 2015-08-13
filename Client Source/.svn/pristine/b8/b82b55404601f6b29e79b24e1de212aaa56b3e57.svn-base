// ----------------------------------------------------------------------------
//  File : UIMonsterMercenary.h
//  Desc : 
// ----------------------------------------------------------------------------

#ifndef	UIMONSTERMERCENARY_H_
#define	UIMONSTERMERCENARY_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#define UI_MONSTER_MERCENARY_WIDTH		208
#define UI_MONSTER_MERCENARY_HEIGHT		44
#define UI_MONSTER_MERCENARY_INFO_WIDTH	178

// [2010/10/20 : Sora] 몬스터 용병 카드
class CUIMonsterMercenary : public CUIWindow	// 용병 기본정보 창
{
public:
	CUIMonsterMercenary();
	~CUIMonsterMercenary();

	CTextureData		*m_ptdAddTexture;
	BOOL				m_bPopupShown;
	INDEX				m_nItemIndex;
	int					m_nTexID;				

	UIRect				m_rcTitle;
	UIRect				m_rcHP;
	UIRectUV			m_rtBackground;
	UIRectUV			m_rtHP;
	UIRectUV			m_rtMobImg;
	CUIDrawBox			m_bxPopupInfo;
	CUIRectString		m_rsMercenaryInfo;
	CUIButton			m_btnClose;

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	void	ToggleMonsterMercenary( int toggle );
	CTString GetMonsterGrade( int grade );
	void	SetMonsterMercenaryInfo();
	void	SetUseItemData( INDEX index );

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	ENGINE_API BOOL ReceiveMercenaryMessage( CNetworkMessage* istr, SLONG hp, SLONG maxhp, SLONG mp, SLONG maxmp );
};

#endif