
// ----------------------------------------------------------------------------
//  File : UIProduct.h
//  Desc : Created by 이기환
// ----------------------------------------------------------------------------

#ifndef	UIPRODUCT_H_
#define	UIPRODUCT_H_
#ifdef	PRAGMA_ONCE 
	#pragma once
#endif

#include <Engine/Interface/UIListBoxEx.h>
#include <Engine/Entities/ItemData.h>
#include <Engine/Interface/UIProcess.h>

// Define max char and line of strings
#define	MAX_PRODUCT_STRING			4
#define	PRODUCT_STRING_CHAR_WIDTH	170

// Define text position
#define	PRODUCT_TITLE_TEXT_OFFSETX	25
#define	PRODUCT_TITLE_TEXT_OFFSETY	5
#define	PRODUCT_DESC_TEXT_SX		18
#define	PRODUCT_DESC_TEXT_SY		33

// Define size of Product
#define	PRODUCT_TOP_HEIGHT			339
#define	PRODUCT_BOTTOM_HEIGHT		41

#define	PRODUCT_WIDTH				216
#define	PRODUCT_HEIGHT				380

#define PRODUCT_SLOT_ROW			4
#define PRODUCT_SLOT_ROW_TOTAL		30

// ----------------------------------------------------------------------------
// Name : CUIProduct
// Desc : 제조 
// ----------------------------------------------------------------------------



class CUIProduct : public CUIWindow
{
protected:
	
// Controls...
	
	// Button
	CUIButton				m_btnClose;							// 닫기 버튼 
	CUIButton				m_btnOK;							// 가공 버튼 
	CUIButton				m_btnCancel;						// 취소 버튼 
	
	// Skill buttons
	std::vector<CUIIcon*>	m_vecIcons;							// 가공 아이템

	// Etc ...
	CUIScrollBar			m_sbProductItem;					// 가공 아이템 창 스크롤 바
	CUIListBox				m_lbPreconditionDesc;				// 필요 조건 설명 리스트 박스
	
	
//	Product Item Info

	int						m_nProductText;						// 가공 문서 아이템 인덱스
	SWORD					m_nTab;
	SWORD					m_nInvenIdx;

	int						m_nSelectProductItem;				// 현재 선택된 가공물
	CTString				m_StrProductType;					// 가공 타입
	int						m_nProductItemCount;				// 가공문서에 맞는 가공품 갯수
	
	BOOL					m_bSatisfied;						// 조건이 충분한가?
	
	CNeedItems				m_NeedItems[MAX_MAKE_ITEM_MATERIAL];// 필요 아이템 정보

	int						m_nNeedItemCount;					// 필요한 아이템 종류의 수
	int						m_nMakeItemCount;			
		
// Region of each part
	UIRect					m_rcTitle;							// Region of title bar
	UIRect					m_rcIcons;							// Region of icons
	UIRect					m_rcTab;							// Region of tab
	UIRect					m_rcItem;
	UIRect					m_rcDesc;

// UV of each part
	UIRectUV				m_rtBackgroundTop;					// UV of background UP
	UIRectUV				m_rtBackgroundBtm;					// UV of background DOWN
	UIRectUV				m_rtSelOutline;						// UV of outline of selected button

// Network ...
	BOOL					m_bWaitProductResult;				// Wait Message

public:
	CUIProduct();
	~CUIProduct();

	
// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	
// Render
	void	Render();

// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	
// Open & close Product
	void	Clear ();
	void	InitProduct();

	ENGINE_API	void	OpenProduct( int nItemIndex, SWORD nTab, SWORD inven_idx );
	void				CloseProduct();
	


// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	
// Network message functions ( receive )
	void	ProductRep( SBYTE sbResult );
	
// Send ...
	void	SendProductReq();

// etc ...
	void	SelectItem ( int nIndex = -1 );
	void	AddString ( CTString& strText, COLOR colText = 0xffffffff );
};


// ----------------------------------------------------------------------------
// Name : CUIProduct2
// Desc : 아이템 제작 확장
// ----------------------------------------------------------------------------
class CUIProduct2: public CUIWindow
{
protected:

	struct sResultItem 
	{
		
		int nitemIndex;
		int nSubType;
		UQUAD lExp;
		CMakeItemData* pItemData;

		sResultItem()
		{
			nitemIndex = -1;
			nSubType	= -1;
			lExp		= -1;
			pItemData = NULL;
		}
		~sResultItem()
		{
			nitemIndex = -1;
			nSubType	= -1;
			lExp		= -1;
			pItemData = NULL;
		}
		bool operator<(const sResultItem &other) const
		{
			if( nSubType < other.nSubType )
			{
				return true;
			}
			else if( nSubType == other.nSubType && lExp < other.lExp )
			{
				return true;
			}

			return false;
		}
				
	};
	CTextureData	*m_ptdaddTexture;
	CUIButton		m_btnClose;
	CUIButton		m_btnMake;
	CUIIcon*		m_pIconsMake;
	CUIIcon*		m_pIconsStuff[5];
	CUIDrawBox		m_bxBackground;
	CUIListBoxEx	m_lbMakeList;

	CMakeItemData*	pSelItemData;
	CTString*		m_pSubTypeString;
	CTString*		m_pstrSealType;
	INDEX			m_nSealType;
	int				m_nSelFactoryIndex;

	BOOL			m_bItemMaking;

	std::vector<INDEX> m_vctMakeLearn;		// 자신이 배운 제작 리스트 저장 공간

public:
	CUIProduct2();
	~CUIProduct2();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void	ListReset();
	// Render
	void	Render();

	void	AddMakeItemList(INDEX nListIndex);
	BOOL	IsLearn(INDEX nIndex);
	void	SetSealType(INDEX sealtype) { m_nSealType = sealtype; }
	UBYTE	GetSealType() { return m_nSealType; }
	void	OpenProduct2(INDEX sealtype);
	void	CloseProduct2();
	void	SetList(CUIListBoxEx* lbList, INDEX sealtype);
	void	SetStuffItem();

	void	SendMakeItem(UBYTE ubType, ULONG lindex);

	void	SetItemMaking(BOOL bMaking);
	void	ProgressBarRender();
	CTString	GetSealTypeName(int nSealtype)	{ return m_pstrSealType[nSealtype-656];}
	

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
};

class CUIProductNPC: public CUIWindow
{
protected:

	CTextureData	*m_ptdaddTexture;
	CUIButton	m_btnClose;
	CUIButton	m_btnLearn;
	CUIButton	m_btnCancel;
	CUIDrawBox		m_bxBackground;

	CUIListBoxEx	m_lbLearnList;
	CMakeItemData*	pSelItemData;

	INDEX			m_nSealType;
	FLOAT			m_fNpcX, m_fNpcZ;
	int				m_nNpcVirIdx;
	int				m_nSealNum;		// 증표획득 확인 창에 사용할 값 저장

public:
	CUIProductNPC();
	~CUIProductNPC();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Render
	void	Render();

	void	Reset();

	void	OpenProductList(INDEX sealtype);
	void	OpenProductNPC(int iMobIndex, int iMobVirIdx, FLOAT npcx, FLOAT npcz);

	void	CloseProductNPC();

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	void MsgBoxLCommand( int nCommandCode, int nResult );
	void MsgBoxCommand(int nCommandCode, BOOL bOK, CTString &strInput );
	void SendFactoryLearn(int nIndex);

};

#endif	// UIProduct_H_

