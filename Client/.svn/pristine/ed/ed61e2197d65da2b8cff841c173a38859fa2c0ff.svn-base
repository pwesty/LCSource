// ***************************************************************
//  UIAffinityInfo   version:  1.0   ? date: 05/28/2009
//  -------------------------------------------------------------
//	Author 	: rumist
//  Desc	: show NPC affinity system informations.
//  -------------------------------------------------------------
//  Copyright (C) 2009 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef UIITEMPRODUCT_h_
#define UIITEMPRODUCT_h_

#include <vector>

class CUIIcon;

class CUIItemProduct : public CUIWindow
{
public:
	enum __tagITEMPRODUCT_UI
	{
		ITEMPRODUCT_WIDTH		= 251,
		ITEMPRODUCT_HEIGHT		= 284,
		ITEMPRODUCT_ITEMCOUNT	= 6
	};

// attribute.
protected:
	typedef		std::vector< CUIIcon* >	vec_Icon;
	typedef		vec_Icon::iterator		vec_IconIter;

	// surface.
	CUIRectSurface			m_rtsBase;
	// title rect.
	UIRect					m_rtTitle;
	// skill area rect.
	UIRect					m_rtArea;

	// texture container.
	CTextureData*			m_ptdButtonTexture;
	// button.
	CUIButton				m_btnClose;
	// scroll bar.
	CUIScrollBar			m_sbProductList;

	CUIDrawBox				m_bxToolTip;

	CTString				m_strToopTip;

	// data container.
	vec_Icon				m_vecProduct;

	// flags.
	BOOL					m_bShowToolTip;
	BOOL					m_bPickTitle;

protected:
	// not used func.
	void					_toggleVisible();
	const	ULONG			_getUIType() const;

public:
	CUIItemProduct();
	~CUIItemProduct();

	//////////////////////////////////////////////////////////////////////////
	// overriding func.
	// Create
	void					Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	
	// Render
	void					Render();
	
	// Adjust position
	void					ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void					AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT				MouseMessage( MSG *pMsg );
	BOOL					CloseWindowByEsc()										{ CloseItemProduct(); return TRUE;	}

	//////////////////////////////////////////////////////////////////////////
	// custom func.
	void					OpenItemProduct();
	void					CloseItemProduct();
	void					ItemProductReset();
	// if rendering type is expert, setting data from network library.
	void					SetExpertData(int nIndex);


};

#endif //UIAFFINITYINFO_h_