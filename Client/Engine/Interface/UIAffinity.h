// ***************************************************************
//  UIAffinity   version:  1.0   ? date: 06/03/2009
//  -------------------------------------------------------------
//	Author 	: rumist
//  Desc	: NPC Affinity System.
//  -------------------------------------------------------------
//  Copyright (C) 2009 - All Rights Reserved
// ***************************************************************
//  Modified : sykim70
// ***************************************************************

#ifndef UIAFFINITY_H_
#define UIAFFINITY_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

class CUIAffinity : public CUIWindow
{
// attribute.
public:
	enum	__tagAffinity_UI
	{
		NPCAFFINITYDONATION_WIDTH	= 410,
		NPCAFFINITYDONATION_HEIGHT	= 240,
		AFFINITY_POINT_LIMIT		= 50000,
		DONATION_SLOT_ROW			= 2,
		DONATION_SLOT_COL			= 6,
		CONTROL_MAX					= 3
	};
protected:
	// surface.
	CUIRectSurface				m_rtsBase;
	// title rect.
	UIRect						m_rtTitle;
	UIRect						m_rtItemSlot;
	// texture container.
	CTextureData*				m_ptdButtonTexture;
	// button.
	CUIButton					m_btnClose;
	CUIButton					m_btnOK;
	CUIButton					m_btnCancel;

	CUIDrawBox					m_bxToolTip;
	CTString					m_strToolTip;

	// data container.
	int							m_nAffinityPoint;

	// npc data.
	SLONG						m_slNPCIdx;
	SLONG						m_slNPCVIdx;
	FLOAT						m_fNpcZ;
	FLOAT						m_fNpcX;

	// flags.
	BOOL						m_bShowToolTip;
	BOOL						m_bPickTitle;
protected:
	void						_sendDonateMessage();

	int m_nAffinityIndex;
	CUIMultiList m_mlItems;

	class Control
	{
	protected:
		bool bEnable;
		CUIButton btnPlus;
		CUIButton btnMinus;
		CUIButton btnMax;
	public:
		void SetEnable(bool flag = true) { bEnable = flag; }
		void Create(CUIWindow *pParentWnd, int nX, int nY, FLOAT fTexWidth, FLOAT fTexHeight);
		void Render();
		WMSG_RESULT MouseMessage(MSG* pMsg, int& nBtn);
	};
	Control m_pCtrl[CONTROL_MAX];
	int m_nControlButtonDown;

	struct Item
	{
		INDEX index;
		int count;
		Item(INDEX index)
		{
			this->index = index;
			this->count = 0;
		}
	};
	std::vector<Item> m_vecItem;

public:
	CUIAffinity();
	~CUIAffinity();
	
	//////////////////////////////////////////////////////////////////////////
	// overriding func.( virtual func. )
	// create.
	void						Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	// Render.
	void						Render();
	// Adjust Position.
	void						ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void						AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	// Message pump.
	WMSG_RESULT					MouseMessage( MSG *pMsg );
	BOOL						CloseWindowByEsc()		{ CloseDonation();	return TRUE;	}
	// Message Handler.
	void						MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void						MsgBoxLCommand( int nCommandCode, int nResult );
	
	//////////////////////////////////////////////////////////////////////////
	// User Definition func.
	ENGINE_API void				OpenAffinity( int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ );
	void						ToggleVisible();
	void						OpenDonation();
	void						CloseDonation();
	void						DonationReset();
	void						OpenShop( const int npcIdx );
	void						CloseAllUI();

	const BOOL					IsOpenUI() const;
	const FLOAT					GetNPCPosX() const			{ return m_fNpcX; }
	const FLOAT					GetNPCPosZ() const			{ return m_fNpcZ; }
	const SLONG					GetNPCIndex() const			{ return m_slNPCIdx; }	// [2010/12/07 : Sora] 친화도 개선 2차
};

#endif //UIAFFINITY_H_
