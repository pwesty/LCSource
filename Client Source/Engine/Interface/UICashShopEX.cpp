#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Entities/CashShopData.h>
#include <Engine/Network/Web.h>
#include <Engine/Interface/UICashShopEX.h>
#include <Engine/Interface/UIMessenger.h>
#include <Engine/JobInfo.h>

#if		defined(G_USA) || defined(G_RUSSIA) || defined(G_BRAZIL)
#include <Engine/Interface/UIOption.h>
#endif	

#define MODEL_TREASURE	("Data\\Item\\Common\\ITEM_treasure02.smc")

extern cWeb			g_web;

extern ENGINE_API INDEX sam_bFullScreenActive;

#define KIT_SLOT_TOTAL						25


const INDEX _uiCashSizeX = 750, _uiCashSizeY = 550;
const INDEX InvenbtnSlicegap = 15;
const INDEX _RankCount = 5; // 추천 아이템 랭크 갯수
const INDEX _GoodsListCount = 8; // 상품 목록 표시 갯수(페이지당)
const INDEX _PurchaseListCount = 25;
const INDEX _PurchaseGetCount = 10; // 구매한 상품 중 한번에 인벤으로 가져올 수 있는 최대 갯수
extern INDEX g_iCountry;

const INDEX _PreviewX = 140, _PreviewY = 226, _RTWidth = 366, _RTHeight = 226; // 캐릭터를 표현하는 Preivew Screen 위치

CCashTypeNode::CCashTypeNode(CCashTypeNode* pNode, CTString strName, CCashTypeNode::CTNINFO _ctnInfo)
{
	m_Parents = pNode;
	m_strName = strName;
	m_CTNINFO = _ctnInfo;
	m_SubList.Clear();

	switch(_ctnInfo)
	{
	case CCashTypeNode::CTN_CATALOG:
		{
			AddNode(CTString("NONE"), CCashTypeNode::CTN_FIRST);
			GetSubNode(0).AddNode(CTString("NONE"), CCashTypeNode::CTN_SECOND);
		}
		break;
	case CCashTypeNode::CTN_FIRST:
		{
			AddNode(CTString("NONE"), CCashTypeNode::CTN_SECOND);
		}
		break;
	}
}

void CCashTypeNode::Create(CCashTypeNode* pNode, CTString strName, CCashTypeNode::CTNINFO _ctnInfo)
{
	m_Parents = pNode;
	m_strName = strName;
	m_CTNINFO = _ctnInfo;
	m_SubList.Clear();

	switch(_ctnInfo)
	{
	case CCashTypeNode::CTN_CATALOG:
		{
			AddNode(CTString("NONE"), CCashTypeNode::CTN_FIRST);
			GetSubNode(0).AddNode(CTString("NONE"), CCashTypeNode::CTN_SECOND);
		}
		break;
	case CCashTypeNode::CTN_FIRST:
		{
			AddNode(CTString("NONE"), CCashTypeNode::CTN_SECOND);
		}
		break;
	}
}

void CCashTypeNode::AddNode(CTString strName, CCashTypeNode::CTNINFO _ctnInfo)
{
	m_SubList.Add(CCashTypeNode(this, strName, _ctnInfo));
}

//////////////////////////////////////////////////////////////////////////
// 캐시 샵 용 Popup 클래스 시작
// ----------------------------------------------------------------------------
// Name : CCashShopPopup::CCashShopPopup()
// Desc : 
// ----------------------------------------------------------------------------
CUICashShopEX::CCashShopPopup::CCashShopPopup()
{

}

// ----------------------------------------------------------------------------
// Name : CCashShopPopup::~CCashShopPopup()
// Desc : 
// ----------------------------------------------------------------------------
CUICashShopEX::CCashShopPopup::~CCashShopPopup()
{
	int		i;
	SAFE_DELETE(m_pIconPackage);

	for (i = 0; i < 5; ++i)
		SAFE_DELETE(m_PackageItems[i]);
}

void CUICashShopEX::CCashShopPopup::PopupClose(void)
{
	m_edGiftContent.SetFocus(FALSE);
	m_edCharacterName.SetFocus(FALSE);
	m_edGiftContent.IsFocused();

	SetVisible(FALSE);
}

// ----------------------------------------------------------------------------
// Name : CCashShopPopup::Create()
// Desc : 
// ----------------------------------------------------------------------------
void CUICashShopEX::CCashShopPopup::Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight)
{
	//m_pParent = (CUIBase*)pParentWnd;
	SetPos(nX, nY);
	SetSize(nWidth, nHeight);

	// Create Texture
	m_ptdBaseTexture = CreateTexture(CTString("Data\\Interface\\Cashshop.tex"));
	FLOAT fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// Purchase
	m_BgPurchasePopup.AddRectSurface(UIRect(0,0,30,22), UIRectUV(48,490,78,512, fTexWidth, fTexHeight));
	m_BgPurchasePopup.AddRectSurface(UIRect(30,0,270,22), UIRectUV(78,490,149,512, fTexWidth, fTexHeight));
	m_BgPurchasePopup.AddRectSurface(UIRect(270,0,300,22), UIRectUV(149,490,179,512, fTexWidth, fTexHeight));

	m_BgPurchasePopup.AddRectSurface(UIRect(0,22,30,338), UIRectUV(0,46,30,124, fTexWidth, fTexHeight));
	m_BgPurchasePopup.AddRectSurface(UIRect(30,22,270,338), UIRectUV(30,46,270,124, fTexWidth, fTexHeight));
	m_BgPurchasePopup.AddRectSurface(UIRect(270,22,300,338), UIRectUV(482,46,512,124, fTexWidth, fTexHeight));

	m_BgPurchasePopup.AddRectSurface(UIRect(0,338,30,360), UIRectUV(0,124,30,146, fTexWidth, fTexHeight));
	m_BgPurchasePopup.AddRectSurface(UIRect(30,338,270,360), UIRectUV(30,124,270,146, fTexWidth, fTexHeight));
	m_BgPurchasePopup.AddRectSurface(UIRect(270,338,300,360), UIRectUV(482,124,512,146, fTexWidth, fTexHeight));

	// Purchase black box
	m_BgPurchasePopup.AddRectSurface(UIRect(8,27,38,52), UIRectUV(294,153,324,178, fTexWidth, fTexHeight));
	m_BgPurchasePopup.AddRectSurface(UIRect(38,27,262,52), UIRectUV(324,153,354,178, fTexWidth, fTexHeight));
	m_BgPurchasePopup.AddRectSurface(UIRect(262,27,292,52), UIRectUV(354,153,384,178, fTexWidth, fTexHeight));

	m_BgPurchasePopup.AddRectSurface(UIRect(8,52,38,274), UIRectUV(294,178,324,204, fTexWidth, fTexHeight));
	m_BgPurchasePopup.AddRectSurface(UIRect(38,52,262,274), UIRectUV(324,178,354,204, fTexWidth, fTexHeight));
	m_BgPurchasePopup.AddRectSurface(UIRect(262,52,292,274), UIRectUV(354,178,384,204, fTexWidth, fTexHeight));

	m_BgPurchasePopup.AddRectSurface(UIRect(8,274,38,299), UIRectUV(294,204,324,229, fTexWidth, fTexHeight));
	m_BgPurchasePopup.AddRectSurface(UIRect(38,274,262,299), UIRectUV(324,204,354,229, fTexWidth, fTexHeight));
	m_BgPurchasePopup.AddRectSurface(UIRect(262,274,292,299), UIRectUV(354,204,384,229, fTexWidth, fTexHeight));

	// Purchase blue box
	m_BgPurchasePopup.AddRectSurface(UIRect(12,31,42,56), UIRectUV(391,153,421,178, fTexWidth, fTexHeight));
	m_BgPurchasePopup.AddRectSurface(UIRect(42,31,258,56), UIRectUV(421,153,451,178, fTexWidth, fTexHeight));
	m_BgPurchasePopup.AddRectSurface(UIRect(258,31,288,56), UIRectUV(451,153,481,178, fTexWidth, fTexHeight));

	m_BgPurchasePopup.AddRectSurface(UIRect(12,56,42,247), UIRectUV(391,178,421,204, fTexWidth, fTexHeight));
	m_BgPurchasePopup.AddRectSurface(UIRect(42,56,258,247), UIRectUV(421,178,451,204, fTexWidth, fTexHeight));
	m_BgPurchasePopup.AddRectSurface(UIRect(258,56,288,247), UIRectUV(451,178,481,204, fTexWidth, fTexHeight));

	m_BgPurchasePopup.AddRectSurface(UIRect(12,247,42,272), UIRectUV(391,204,421,229, fTexWidth, fTexHeight));
	m_BgPurchasePopup.AddRectSurface(UIRect(42,247,258,272), UIRectUV(421,204,451,229, fTexWidth, fTexHeight));
	m_BgPurchasePopup.AddRectSurface(UIRect(258,247,288,272), UIRectUV(451,204,481,229, fTexWidth, fTexHeight));

	// Purchase black edit box
	m_BgPurchasePopup.AddRectSurface(UIRect(170,275,288,295), UIRectUV(39,236,157,256, fTexWidth, fTexHeight));

	// Gift
	m_BgGiftPopup.AddRectSurface(UIRect(0,0,30,22), UIRectUV(48,490,78,512, fTexWidth, fTexHeight));
	m_BgGiftPopup.AddRectSurface(UIRect(30,0,270,22), UIRectUV(78,490,149,512, fTexWidth, fTexHeight));
	m_BgGiftPopup.AddRectSurface(UIRect(270,0,300,22), UIRectUV(149,490,179,512, fTexWidth, fTexHeight));

	m_BgGiftPopup.AddRectSurface(UIRect(0,22,30,181), UIRectUV(0,46,30,124, fTexWidth, fTexHeight));
	m_BgGiftPopup.AddRectSurface(UIRect(30,22,270,181), UIRectUV(30,46,270,124, fTexWidth, fTexHeight));
	m_BgGiftPopup.AddRectSurface(UIRect(270,22,300,181), UIRectUV(482,46,512,124, fTexWidth, fTexHeight));

	m_BgGiftPopup.AddRectSurface(UIRect(0,181,30,203), UIRectUV(0,124,30,146, fTexWidth, fTexHeight));
	m_BgGiftPopup.AddRectSurface(UIRect(30,181,270,203), UIRectUV(30,124,270,146, fTexWidth, fTexHeight));
	m_BgGiftPopup.AddRectSurface(UIRect(270,181,300,203), UIRectUV(482,124,512,146, fTexWidth, fTexHeight));

	// Gift black box
	m_BgGiftPopup.AddRectSurface(UIRect(8,72,38,97), UIRectUV(294,153,324,178, fTexWidth, fTexHeight));
	m_BgGiftPopup.AddRectSurface(UIRect(38,72,262,97), UIRectUV(324,153,354,178, fTexWidth, fTexHeight));
	m_BgGiftPopup.AddRectSurface(UIRect(262,72,292,97), UIRectUV(354,153,384,178, fTexWidth, fTexHeight));

	m_BgGiftPopup.AddRectSurface(UIRect(8,97,38,146), UIRectUV(294,178,324,204, fTexWidth, fTexHeight));
	m_BgGiftPopup.AddRectSurface(UIRect(38,97,262,146), UIRectUV(324,178,354,204, fTexWidth, fTexHeight));
	m_BgGiftPopup.AddRectSurface(UIRect(262,97,292,146), UIRectUV(354,178,384,204, fTexWidth, fTexHeight));

	m_BgGiftPopup.AddRectSurface(UIRect(8,146,38,171), UIRectUV(294,204,324,229, fTexWidth, fTexHeight));
	m_BgGiftPopup.AddRectSurface(UIRect(38,146,262,171), UIRectUV(324,204,354,229, fTexWidth, fTexHeight));
	m_BgGiftPopup.AddRectSurface(UIRect(262,146,292,171), UIRectUV(354,204,384,229, fTexWidth, fTexHeight));

	// Gift blue box
	m_BgGiftPopup.AddRectSurface(UIRect(12,96,42,121), UIRectUV(391,153,421,178, fTexWidth, fTexHeight));
	m_BgGiftPopup.AddRectSurface(UIRect(42,96,258,121), UIRectUV(421,153,451,178, fTexWidth, fTexHeight));
	m_BgGiftPopup.AddRectSurface(UIRect(258,96,288,121), UIRectUV(451,153,481,178, fTexWidth, fTexHeight));

	m_BgGiftPopup.AddRectSurface(UIRect(12,121,42,140), UIRectUV(391,178,421,204, fTexWidth, fTexHeight));
	m_BgGiftPopup.AddRectSurface(UIRect(42,121,258,140), UIRectUV(421,178,451,204, fTexWidth, fTexHeight));
	m_BgGiftPopup.AddRectSurface(UIRect(258,121,288,140), UIRectUV(451,178,481,204, fTexWidth, fTexHeight));

	m_BgGiftPopup.AddRectSurface(UIRect(12,140,42,165), UIRectUV(391,204,421,229, fTexWidth, fTexHeight));
	m_BgGiftPopup.AddRectSurface(UIRect(42,140,258,165), UIRectUV(421,204,451,229, fTexWidth, fTexHeight));
	m_BgGiftPopup.AddRectSurface(UIRect(258,140,288,165), UIRectUV(451,204,481,229, fTexWidth, fTexHeight));

	// Gift black edit box (143 * 20)
	m_BgGiftPopup.AddRectSurface(UIRect(149,25,179,45), UIRectUV(39,236,69,256, fTexWidth, fTexHeight));
	m_BgGiftPopup.AddRectSurface(UIRect(179,25,262,45), UIRectUV(69,236,127,256, fTexWidth, fTexHeight));
	m_BgGiftPopup.AddRectSurface(UIRect(262,25,292,45), UIRectUV(127,236,157,256, fTexWidth, fTexHeight));

	// Package
	m_BgPackagePopup.AddRectSurface(UIRect(0,0,30,22), UIRectUV(48,490,78,512, fTexWidth, fTexHeight));
	m_BgPackagePopup.AddRectSurface(UIRect(30,0,270,22), UIRectUV(78,490,149,512, fTexWidth, fTexHeight));
	m_BgPackagePopup.AddRectSurface(UIRect(270,0,300,22), UIRectUV(149,490,179,512, fTexWidth, fTexHeight));

	m_BgPackagePopup.AddRectSurface(UIRect(0,22,30,310), UIRectUV(0,46,30,124, fTexWidth, fTexHeight));
	m_BgPackagePopup.AddRectSurface(UIRect(30,22,270,310), UIRectUV(30,46,270,124, fTexWidth, fTexHeight));
	m_BgPackagePopup.AddRectSurface(UIRect(270,22,300,310), UIRectUV(482,46,512,124, fTexWidth, fTexHeight));

	m_BgPackagePopup.AddRectSurface(UIRect(0,310,30,332), UIRectUV(0,124,30,146, fTexWidth, fTexHeight));
	m_BgPackagePopup.AddRectSurface(UIRect(30,310,270,332), UIRectUV(30,124,270,146, fTexWidth, fTexHeight));
	m_BgPackagePopup.AddRectSurface(UIRect(270,310,300,332), UIRectUV(482,124,512,146, fTexWidth, fTexHeight));

	// Package black box
	m_BgPackagePopup.AddRectSurface(UIRect(8,27,38,52), UIRectUV(294,153,324,178, fTexWidth, fTexHeight));
	m_BgPackagePopup.AddRectSurface(UIRect(38,27,262,52), UIRectUV(324,153,354,178, fTexWidth, fTexHeight));
	m_BgPackagePopup.AddRectSurface(UIRect(262,27,292,52), UIRectUV(354,153,384,178, fTexWidth, fTexHeight));

	m_BgPackagePopup.AddRectSurface(UIRect(8,52,38,274), UIRectUV(294,178,324,204, fTexWidth, fTexHeight));
	m_BgPackagePopup.AddRectSurface(UIRect(38,52,262,274), UIRectUV(324,178,354,204, fTexWidth, fTexHeight));
	m_BgPackagePopup.AddRectSurface(UIRect(262,52,292,274), UIRectUV(354,178,384,204, fTexWidth, fTexHeight));

	m_BgPackagePopup.AddRectSurface(UIRect(8,274,38,299), UIRectUV(294,204,324,229, fTexWidth, fTexHeight));
	m_BgPackagePopup.AddRectSurface(UIRect(38,274,262,299), UIRectUV(324,204,354,229, fTexWidth, fTexHeight));
	m_BgPackagePopup.AddRectSurface(UIRect(262,274,292,299), UIRectUV(354,204,384,229, fTexWidth, fTexHeight));

	// Package blue box
	m_BgPackagePopup.AddRectSurface(UIRect(12,32,42,57), UIRectUV(391,153,421,178, fTexWidth, fTexHeight));
	m_BgPackagePopup.AddRectSurface(UIRect(42,32,258,57), UIRectUV(421,153,451,178, fTexWidth, fTexHeight));
	m_BgPackagePopup.AddRectSurface(UIRect(258,32,288,57), UIRectUV(451,153,481,178, fTexWidth, fTexHeight));

	m_BgPackagePopup.AddRectSurface(UIRect(12,57,42,61), UIRectUV(391,178,421,204, fTexWidth, fTexHeight));
	m_BgPackagePopup.AddRectSurface(UIRect(42,57,258,61), UIRectUV(421,178,451,204, fTexWidth, fTexHeight));
	m_BgPackagePopup.AddRectSurface(UIRect(258,57,288,61), UIRectUV(451,178,481,204, fTexWidth, fTexHeight));

	m_BgPackagePopup.AddRectSurface(UIRect(12,61,42,86), UIRectUV(391,204,421,229, fTexWidth, fTexHeight));
	m_BgPackagePopup.AddRectSurface(UIRect(42,61,258,86), UIRectUV(421,204,451,229, fTexWidth, fTexHeight));
	m_BgPackagePopup.AddRectSurface(UIRect(258,61,288,86), UIRectUV(451,204,481,229, fTexWidth, fTexHeight));

	m_BgPackagePopup.AddRectSurface(UIRect(12,93,42,118), UIRectUV(391,153,421,178, fTexWidth, fTexHeight));
	m_BgPackagePopup.AddRectSurface(UIRect(42,93,258,118), UIRectUV(421,153,451,178, fTexWidth, fTexHeight));
	m_BgPackagePopup.AddRectSurface(UIRect(258,93,288,118), UIRectUV(451,153,481,178, fTexWidth, fTexHeight));

	m_BgPackagePopup.AddRectSurface(UIRect(12,118,42,269), UIRectUV(391,178,421,204, fTexWidth, fTexHeight));
	m_BgPackagePopup.AddRectSurface(UIRect(42,118,258,269), UIRectUV(421,178,451,204, fTexWidth, fTexHeight));
	m_BgPackagePopup.AddRectSurface(UIRect(258,118,288,269), UIRectUV(451,178,481,204, fTexWidth, fTexHeight));

	m_BgPackagePopup.AddRectSurface(UIRect(12,269,42,294), UIRectUV(391,204,421,229, fTexWidth, fTexHeight));
	m_BgPackagePopup.AddRectSurface(UIRect(42,269,258,294), UIRectUV(421,204,451,229, fTexWidth, fTexHeight));
	m_BgPackagePopup.AddRectSurface(UIRect(258,269,288,294), UIRectUV(451,204,481,229, fTexWidth, fTexHeight));

	m_PackageItems.New(5);

	m_BgPackagePopup.AddRectSurface(UIRect(18,42,52,76), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_pIconPackage = new CUIIcon;
	m_pIconPackage->Create(this, 19, 43, 32, 32, -1, UBET_ITEM);
	m_BgPackagePopup.AddRectSurface(UIRect(18,101,52,135), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_PackageItems[0] = new CUIIcon;
	m_PackageItems[0]->Create(this, 19, 102, 32, 32, -1, UBET_ITEM);
	m_BgPackagePopup.AddRectSurface(UIRect(18,139,52,172), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_PackageItems[1] = new CUIIcon;
	m_PackageItems[1]->Create(this, 19, 140, 32, 32, -1, UBET_ITEM);
	m_BgPackagePopup.AddRectSurface(UIRect(18,177,52,211), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_PackageItems[2] = new CUIIcon;
	m_PackageItems[2]->Create(this, 19, 178, 32, 32, -1, UBET_ITEM);
	m_BgPackagePopup.AddRectSurface(UIRect(18,215,52,249), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_PackageItems[3] = new CUIIcon;
	m_PackageItems[3]->Create(this, 19, 216, 32, 32, -1, UBET_ITEM);
	m_BgPackagePopup.AddRectSurface(UIRect(18,253,52,287), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_PackageItems[4] = new CUIIcon;
	m_PackageItems[4]->Create(this, 19, 254, 32, 32, -1, UBET_ITEM);

	m_PackageScroll.Create(this, 276,94,9,199);
	m_PackageScroll.CreateButtons(TRUE, 9,7,0,0,10);
	m_PackageScroll.SetScrollPos(0);
	m_PackageScroll.SetCurItemCount(0);
	m_PackageScroll.SetItemsPerPage(5);
	// Up button
	m_PackageScroll.SetUpUV(UBS_IDLE, 12,488,21,495,fTexWidth,fTexHeight);
	m_PackageScroll.SetUpUV(UBS_CLICK, 22,488,31,495,fTexWidth,fTexHeight);
	m_PackageScroll.CopyUpUV(UBS_IDLE, UBS_ON);
	m_PackageScroll.CopyUpUV(UBS_CLICK, UBS_DISABLE);
	// Down button
	m_PackageScroll.SetDownUV(UBS_IDLE, 12,496,21,503,fTexWidth,fTexHeight);
	m_PackageScroll.SetDownUV(UBS_CLICK, 22,496,31,503,fTexWidth,fTexHeight);
	m_PackageScroll.CopyDownUV(UBS_IDLE, UBS_ON);
	m_PackageScroll.CopyDownUV(UBS_CLICK, UBS_DISABLE);
	// Bar button
	m_PackageScroll.SetBarTopUV( 0, 488, 9, 494, fTexWidth, fTexHeight );
	m_PackageScroll.SetBarMiddleUV( 0, 494, 9, 506, fTexWidth, fTexHeight );
	m_PackageScroll.SetBarBottomUV( 0, 506, 9, 512, fTexWidth, fTexHeight );
	// wheel
	m_PackageScroll.SetWheelRect(-263, 0, 262, 199);

	// gift
	m_cbFriends.Create(this, 149,47,143,20,127,4,13,13,10,_pUIFontTexMgr->GetFontHeight()+5, 0, 0);
	m_cbFriends.SetBackUV(39,236,157,256,fTexWidth,fTexHeight);
	m_cbFriends.SetDownBtnUV(165,236,178,249,fTexWidth,fTexHeight);
	m_cbFriends.SetUpBtnUV(165,252,178,265,fTexWidth,fTexHeight);
	m_cbFriends.SetDropListUV(294,153,384,229,fTexWidth,fTexHeight);

	m_cbFriends.CreateScroll(TRUE,9,7,0,0,10);
	
	m_cbFriends.SetScrollUpUV(UBS_IDLE, 12,488,21,495,fTexWidth,fTexHeight);
	m_cbFriends.SetScrollUpUV(UBS_CLICK, 22,488,31,495,fTexWidth,fTexHeight);
	m_cbFriends.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_cbFriends.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	
	m_cbFriends.SetScrollBarTopUV( 0, 488, 9, 494, fTexWidth, fTexHeight );
	m_cbFriends.SetScrollBarMiddleUV( 0, 494, 9, 506, fTexWidth, fTexHeight );
	m_cbFriends.SetScrollBarBottomUV( 0, 506, 9, 512, fTexWidth, fTexHeight );

	m_cbFriends.SetScrollDownUV(UBS_IDLE, 12,496,21,503,fTexWidth,fTexHeight);
	m_cbFriends.SetScrollDownUV(UBS_CLICK, 22,496,31,503,fTexWidth,fTexHeight);
	m_cbFriends.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_cbFriends.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );

	m_edCharacterName.Create( this, 149, 25, 143, 20, 16 );
	m_edCharacterName.SetReadingWindowUV( 391, 153, 481, 229, fTexWidth, fTexHeight );
	m_edCharacterName.SetCandidateUV( 391, 153, 481, 229, fTexWidth, fTexHeight );
//276,69
	m_edGiftContent.Create(this, 12,96,276,69,3);

	m_btnConfirm.Create(this, _S(2380, "결제"), 0,0,108,21);
	m_btnConfirm.SetNewType(TRUE);
	m_btnConfirm.SetRTSurface(UBS_IDLE, UIRect(0,0,InvenbtnSlicegap,21), UIRectUV(249,235,264,256,fTexWidth,fTexHeight)); // L
	m_btnConfirm.SetRTSurface(UBS_IDLE, UIRect(InvenbtnSlicegap,0,108-InvenbtnSlicegap,21), UIRectUV(264,235,299,256,fTexWidth,fTexHeight)); // M
	m_btnConfirm.SetRTSurface(UBS_IDLE, UIRect(108-InvenbtnSlicegap,0,108,21), UIRectUV(299,235,314,256,fTexWidth,fTexHeight)); // R
	m_btnConfirm.SetRTSurface(UBS_CLICK, UIRect(0,0,InvenbtnSlicegap,21), UIRectUV(249,258,264,279,fTexWidth,fTexHeight)); // L
	m_btnConfirm.SetRTSurface(UBS_CLICK, UIRect(InvenbtnSlicegap,0,108-InvenbtnSlicegap,21), UIRectUV(264,258,299,279,fTexWidth,fTexHeight)); // M
	m_btnConfirm.SetRTSurface(UBS_CLICK, UIRect(108-InvenbtnSlicegap,0,108,21), UIRectUV(299,258,314,279,fTexWidth,fTexHeight)); // R
	m_btnConfirm.CopyRTSurface(UBS_IDLE, UBS_DISABLE);
	m_btnConfirm.CopyRTSurface(UBS_IDLE, UBS_ON);

	m_btnCancel.Create(this, _S(139, "취소"), 0,0,108,21);
	m_btnCancel.SetNewType(TRUE);
	m_btnCancel.SetRTSurface(UBS_IDLE, UIRect(0,0,InvenbtnSlicegap,21), UIRectUV(249,235,264,256,fTexWidth,fTexHeight)); // L
	m_btnCancel.SetRTSurface(UBS_IDLE, UIRect(InvenbtnSlicegap,0,108-InvenbtnSlicegap,21), UIRectUV(264,235,299,256,fTexWidth,fTexHeight)); // M
	m_btnCancel.SetRTSurface(UBS_IDLE, UIRect(108-InvenbtnSlicegap,0,108,21), UIRectUV(299,235,314,256,fTexWidth,fTexHeight)); // R
	m_btnCancel.SetRTSurface(UBS_CLICK, UIRect(0,0,InvenbtnSlicegap,21), UIRectUV(249,258,264,279,fTexWidth,fTexHeight)); // L
	m_btnCancel.SetRTSurface(UBS_CLICK, UIRect(InvenbtnSlicegap,0,108-InvenbtnSlicegap,21), UIRectUV(264,258,299,279,fTexWidth,fTexHeight)); // M
	m_btnCancel.SetRTSurface(UBS_CLICK, UIRect(108-InvenbtnSlicegap,0,108,21), UIRectUV(299,258,314,279,fTexWidth,fTexHeight)); // R
	m_btnCancel.CopyRTSurface(UBS_IDLE, UBS_DISABLE);
	m_btnCancel.CopyRTSurface(UBS_IDLE, UBS_ON);

	m_btnClose.Create(this, _s(""), 0,0,16,16);
	m_btnClose.SetUV(UBS_IDLE, 122,258,138,274, fTexWidth, fTexHeight);
	m_btnClose.SetUV(UBS_CLICK, 140,258,156,274, fTexWidth, fTexHeight);
	m_btnClose.CopyUV(UBS_IDLE, UBS_DISABLE);
	m_btnClose.CopyUV(UBS_IDLE, UBS_ON);

	SetVisible(FALSE);
}

// ----------------------------------------------------------------------------
// Name : CCashShopPopup::ResetPosition()
// Desc : 
// ----------------------------------------------------------------------------
void CUICashShopEX::CCashShopPopup::ResetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ)
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : CCashShopPopup::AdjustPosition()
// Desc : 
// ----------------------------------------------------------------------------
void CUICashShopEX::CCashShopPopup::AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ)
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : CCashShopPopup::OpenPopup()
// Desc : 
// ----------------------------------------------------------------------------
void CUICashShopEX::CCashShopPopup::OpenPopup(CSPopupType type)
{
	if (m_GoodsDatas.Count() < 1)
	{
		return;
	}

	m_Type = type;
	SetVisible(TRUE);
	SetFocus(TRUE);

	if (m_Type == CSP_Package)
	{
		SetWidth(300);
		SetHeight(332);
	}
	else if (m_Type == CSP_Gift)
	{
		SetWidth(300);
		SetHeight(203);
	}
	else if (m_Type == CSP_Purchase)
	{
		SetWidth(300);
		SetHeight(360);
	}

	ResetPosition(m_pixMinI, m_pixMinJ, m_pixMaxI, m_pixMaxJ);

	switch(m_Type)
	{
	case CSP_Package:
		{
			m_btnCancel.SetPos(97, 304);
			m_btnCancel.SetText(_S(870, "닫기"));
			m_btnClose.SetPos(260,4);
			m_pIconPackage->clearIconData();
			int i;
			for (i=0; i<5; ++i)
			{
				m_PackageItems[i]->clearIconData();
			}

			SetPackageItems();
		}
		break;
	case CSP_Gift:
		{
			m_btnConfirm.SetPos(74,174);
			m_btnConfirm.SetText(_S(3090, "보내기"));
			m_btnCancel.SetPos(185,174);
			m_btnCancel.SetText(_S(139, "취소"));
			m_btnClose.SetPos(260,4);

			AddFriendsList();
			m_edCharacterName.ResetString();
			m_edCharacterName.SetFocus(TRUE);
			m_edGiftContent.ResetString();
			m_edGiftContent.SetFocus(FALSE);
		}
		break;
	case CSP_Purchase:
		{
			m_btnConfirm.SetPos(36,329);
			m_btnConfirm.SetText(_S(2380, "결제"));
			m_btnCancel.SetPos(161,329);
			m_btnCancel.SetText(_S(139, "취소"));
			m_btnClose.SetPos(260,4);
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : CCashShopPopup::Render()
// Desc : 
// ----------------------------------------------------------------------------
void CUICashShopEX::CCashShopPopup::Render()
{
	if( IsVisible() == FALSE )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	pDrawPort->InitTextureData(m_ptdBaseTexture);

	switch(m_Type)
	{
	case CSP_Package:
		{
			int i;
			m_BgPackagePopup.SetPos(m_nPosX, m_nPosY);
			m_BgPackagePopup.RenderRectSurface(pDrawPort, 0xFFFFFFFF);

			m_btnCancel.Render();
			m_btnClose.Render();
			m_PackageScroll.Render();
			
			pDrawPort->PutTextExCX(_S(5307, "패키지 정보"), m_nPosX+150, m_nPosY+4, 0xFFFFFFFF);

			CCashShopData::CASH_SHOP_DATA* pCashData = _pNetwork->GetCashShopData().GetCashShopData(m_pIconPackage->GetCashIndex());
			pDrawPort->PutTextEx(pCashData->m_itemName, m_nPosX+77,m_nPosY+54, 0xFFFFFFFF);

			int tmpY = 113;

			for (i = 0; i < 5; ++i)
			{
				if (!m_PackageItems[i]->IsEmpty())
				{
					CTString strName = _pNetwork->GetItemName(m_PackageItems[i]->getIndex());
					pDrawPort->PutTextEx(strName, m_nPosX+77,m_nPosY+tmpY, 0xFFFFFFFF);
					tmpY += 37;
				}
			}

			pDrawPort->FlushRenderingQueue();
			pDrawPort->EndTextEx();

			m_pIconPackage->Render(pDrawPort);

			for (i = 0; i < 5; ++i)
			{
				if (m_PackageItems[i]->IsEmpty() == false)
					m_PackageItems[i]->Render(pDrawPort);
			}
		}
		break;
	case CSP_Gift:
		{
			m_BgGiftPopup.SetPos(m_nPosX, m_nPosY);
			m_BgGiftPopup.RenderRectSurface(pDrawPort, 0xFFFFFFFF);

			m_btnConfirm.Render();
			m_btnCancel.Render();
			m_btnClose.Render();

			pDrawPort->PutTextExCX(_S(3104, "선물 받을 친구"), m_nPosX+150, m_nPosY+4, 0xFFFFFFFF);
			pDrawPort->PutTextExRX(_S(5214, "캐릭터 명"), m_nPosX+142,m_nPosY+28,0xFFFFFFFF);
			pDrawPort->PutTextExRX(_S(5215, "친구목록"), m_nPosX+142,m_nPosY+50,0xFFFFFFFF);
			pDrawPort->PutTextEx(_S(3101, "선물메시지"), m_nPosX+17,m_nPosY+79,0xFFFFFFFF);

			m_edCharacterName.Render();
			pDrawPort->FlushRenderingQueue();

			m_edGiftContent.Render();
			pDrawPort->FlushRenderingQueue();

			// 100 byte제한으로 인해 MultiEditBox에서 IME완성이 안되서 제한 없앰(입력 라인 제한으로 변경)
			CTString temp;
			//temp.PrintF("(%d/100 Byte)", m_edGiftContent.GetAllStringLength());
			temp = _S(5317, "(입력 3줄 제한)");
			pDrawPort->PutTextExRX( temp, m_nPosX+285, m_nPosY+79, 0xFFFFFFFF );

			pDrawPort->EndTextEx();
			pDrawPort->FlushBtnRenderingQueue(UBET_ITEM);

			pDrawPort->InitTextureData(m_ptdBaseTexture);
			m_cbFriends.Render();
			pDrawPort->FlushRenderingQueue();
			pDrawPort->EndTextEx();

			// Reading window
			if( m_edCharacterName.DoesShowReadingWindow() )
			{
				// Set texture
				pDrawPort->InitTextureData( m_ptdBaseTexture );

				// Reading window
				m_edCharacterName.RenderReadingWindow();

				// Render all elements
				pDrawPort->FlushRenderingQueue();

				// Flush all render text queue
				pDrawPort->EndTextEx();
			}
		}
		break;
	case CSP_Purchase:
		{
			m_BgPurchasePopup.SetPos(m_nPosX, m_nPosY);
			m_BgPurchasePopup.RenderRectSurface(pDrawPort, 0xFFFFFFFF);

			m_btnConfirm.Render();
			m_btnCancel.Render();
			m_btnClose.Render();

			pDrawPort->PutTextExCX(_S(5304, "구매 결제 창"), m_nPosX+150, m_nPosY+4, 0xFFFFFFFF);
			pDrawPort->PutTextExCX(_S(2392, "구매한 물품"), m_nPosX+103,m_nPosY+40,0xFFFFFFFF);
			pDrawPort->PutTextExCX(_S(2397, "가격"), m_nPosX+252,m_nPosY+40,0xFFFFFFFF);

			//21, 63
			//252,63

			int i;
			INDEX iTotalCash = 0;
			int tmpY = 63;

			for (i=0; i<m_GoodsDatas.Count(); ++i)
			{
				pDrawPort->PutTextEx(m_GoodsDatas[i]->m_itemName, m_nPosX+21, m_nPosY+tmpY, 0xFFFFFFFF);
				CTString strPrice;
				strPrice.PrintF("%d", m_GoodsDatas[i]->m_cash);
				iTotalCash += m_GoodsDatas[i]->m_cash;
				pUIManager->InsertCommaToString(strPrice);
				pDrawPort->PutTextExCX(strPrice, m_nPosX+252, m_nPosY+tmpY, 0xFFFFFFFF);
				tmpY += 20;
			}//283,279

			pDrawPort->PutTextEx(_S(5216, "총 구입 금액"), m_nPosX+20,m_nPosY+279, 0xFFFFFFFF);
			
			CTString strTotal;
			strTotal.PrintF("%d", iTotalCash);
			pUIManager->InsertCommaToString(strTotal);
			pDrawPort->PutTextExRX(strTotal, m_nPosX+283, m_nPosY+279, 0xFFFFFFFF);

			pDrawPort->FlushRenderingQueue();
			pDrawPort->EndTextEx();
			pDrawPort->FlushBtnRenderingQueue(UBET_ITEM);
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : CCashShopPopup::MouseMessage()
// Desc : 
// ----------------------------------------------------------------------------
WMSG_RESULT CUICashShopEX::CCashShopPopup::MouseMessage(MSG *pMsg)
{
	if (!IsVisible())
	{
		return WMSG_FAIL;
	}

	WMSG_RESULT wmsgResult;
	// Item clicked
	static BOOL bLButtonDownInItem = FALSE;
	
	// Mouse point
	static int nOldX, nOldY;
	int nX = LOWORD(pMsg->lParam);
	int nY = HIWORD(pMsg->lParam);

	switch(pMsg->message)
	{
	case WM_MOUSEMOVE:
		{
			if (IsInside(nX, nY))
			{
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();
				
				m_btnClose.MouseMessage(pMsg);
				
				if (m_Type == CSP_Package)
				{
					m_btnCancel.MouseMessage(pMsg);

					if ((wmsgResult = m_PackageScroll.MouseMessage(pMsg)) != WMSG_FAIL)
					{
						if (wmsgResult == WMSG_COMMAND)
						{
							SetPackageItems(m_PackageScroll.GetScrollPos());
						}
					}

					m_pIconPackage->MouseMessage(pMsg);
					int i;
					for (i = 0; i < 5; ++i)
					{
						if (!m_PackageItems[i]->IsEmpty())
						{
							m_PackageItems[i]->MouseMessage(pMsg);
						}
					}
				}
				else if (m_Type == CSP_Gift)
				{
					m_btnConfirm.MouseMessage(pMsg);
					m_btnCancel.MouseMessage(pMsg);
					m_cbFriends.MouseMessage(pMsg);
				}
				else if (m_Type == CSP_Purchase)
				{
					m_btnConfirm.MouseMessage(pMsg);
					m_btnCancel.MouseMessage(pMsg);
				}

				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if (m_Type == CSP_Gift)
			{
				if ((wmsgResult = m_cbFriends.MouseMessage(pMsg)) != WMSG_FAIL)
				{
					if (wmsgResult == WMSG_COMMAND)
					{
						if (m_cbFriends.GetCurSel() > 0)
						{
							std::vector<CTString> tv_vecStr = m_cbFriends.GetVecString();
							m_edCharacterName.ResetString();
							m_edCharacterName.SetString(tv_vecStr[m_cbFriends.GetCurSel()].str_String);
						}
					}

					return WMSG_SUCCESS;
				}
			}

			if (IsInside(nX, nY))
			{
				SetFocus(TRUE);

				if (m_btnClose.MouseMessage(pMsg) != WMSG_FAIL)
				{
					return WMSG_SUCCESS;
				}

				if (m_Type == CSP_Package)
				{
					if (m_btnCancel.MouseMessage(pMsg) != WMSG_FAIL)
					{
						return WMSG_SUCCESS;
					}
					else if ((wmsgResult = m_PackageScroll.MouseMessage(pMsg)) != WMSG_FAIL)
					{
						if (wmsgResult == WMSG_COMMAND)
						{
							SetPackageItems(m_PackageScroll.GetScrollPos());
						}

						return WMSG_SUCCESS;
					}
				}
				else if (m_Type == CSP_Gift)
				{
					if (m_btnConfirm.MouseMessage(pMsg) != WMSG_FAIL)
					{
						return WMSG_SUCCESS;
					}
					else if(m_btnCancel.MouseMessage(pMsg) != WMSG_FAIL)
					{
						return WMSG_SUCCESS;
					}
					else if (m_edCharacterName.MouseMessage(pMsg) != WMSG_FAIL)
					{
						m_edCharacterName.SetFocus(TRUE);
						m_edGiftContent.SetFocus(FALSE);
						return WMSG_SUCCESS;
					}
					else if (m_edGiftContent.MouseMessage(pMsg) != WMSG_FAIL)
					{
						m_edCharacterName.SetFocus(FALSE);
						m_edGiftContent.SetFocus(TRUE);
						return WMSG_SUCCESS;
					}
				}
				else if (m_Type == CSP_Purchase)
				{
					if (m_btnConfirm.MouseMessage(pMsg) != WMSG_FAIL)
					{
						return WMSG_SUCCESS;
					}
					else if (m_btnCancel.MouseMessage(pMsg) != WMSG_FAIL)
					{
						return WMSG_SUCCESS;
					}
				}
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			if (IsInside(nX, nY))
			{
				if (m_btnClose.MouseMessage(pMsg) != WMSG_FAIL)
				{
					PopupClose();
					return WMSG_SUCCESS;
				}

				if (m_Type == CSP_Package)
				{
					if (m_btnCancel.MouseMessage(pMsg) != WMSG_FAIL)
					{
						PopupClose();
						return WMSG_SUCCESS;
					}
					else if ((wmsgResult = m_PackageScroll.MouseMessage(pMsg)) != WMSG_FAIL)
					{
						return WMSG_SUCCESS;
					}
				}
				else if (m_Type == CSP_Gift)
				{
					if (m_btnConfirm.MouseMessage(pMsg) != WMSG_FAIL)
					{
						CTString strName = m_edCharacterName.GetString();
						if (strName.Length() > 0)
						{
							CUIManager::getSingleton()->GetCashShopEX()->SendGiftReq();
						}
						return WMSG_SUCCESS;
					}
					else if(m_btnCancel.MouseMessage(pMsg) != WMSG_FAIL)
					{
						PopupClose();
						return WMSG_SUCCESS;
					}
					else if (m_edCharacterName.MouseMessage(pMsg) != WMSG_FAIL)
					{
						return WMSG_SUCCESS;
					}
					else if (m_edGiftContent.MouseMessage(pMsg) != WMSG_FAIL)
					{
						return WMSG_SUCCESS;
					}
					else if (m_cbFriends.MouseMessage(pMsg) != WMSG_FAIL)
					{
						return WMSG_SUCCESS;
					}
				}
				else if (m_Type == CSP_Purchase)
				{
					if (m_btnConfirm.MouseMessage(pMsg) != WMSG_FAIL)
					{
						SendPurchaseReq();
						return WMSG_SUCCESS;
					}
					else if (m_btnCancel.MouseMessage(pMsg) != WMSG_FAIL)
					{
						PopupClose();
						return WMSG_SUCCESS;
					}
				}
			}
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			if( m_Type != CSP_Package )
				break;

			if( IsInside(nX, nY) == FALSE )
				break;

			CUIManager* pUIManager = CUIManager::getSingleton();

			for (int i=0; i<5; ++i)
			{
				if (m_PackageItems[i]->IsEmpty() == true)
					continue;

				if( m_PackageItems[i]->MouseMessage(pMsg) == WMSG_FAIL )
					continue;

				// 장비 아이템이면 더블클릭으로 입어 볼 수 있다.
				UIMGR()->GetCashShopEX()->SetWearingModel(m_PackageItems[i], m_pIconPackage->GetCashIndex());
			}
		}
		break;
	case WM_MOUSEWHEEL:
		{
			if (m_Type == CSP_Package)
			{
				if (IsInside(nX,nY))
				{
					if ((wmsgResult = m_PackageScroll.MouseMessage(pMsg)) != WMSG_FAIL)
					{
						SetPackageItems(m_PackageScroll.GetScrollPos());
					}
				}
			}
		}
		break;
	}

	return WMSG_SUCCESS;
}

// ----------------------------------------------------------------------------
// Name : CCashShopPopup::KeyMessage()
// Desc : 
// ----------------------------------------------------------------------------
WMSG_RESULT CUICashShopEX::CCashShopPopup::KeyMessage(MSG *pMsg)
{
	if( !IsFocused() || m_Type != CSP_Gift) return WMSG_FAIL;
	
	WMSG_RESULT	wmsgResult;

	if( pMsg->wParam == VK_TAB )
	{
		if( m_edCharacterName.IsFocused() )
		{
			m_edCharacterName.SetFocus( FALSE );
			m_edGiftContent.SetFocus( TRUE );
		}
		else if( m_edGiftContent.IsFocused() )
		{
			m_edGiftContent.SetFocus( FALSE );
			m_edCharacterName.SetFocus( TRUE );
		}

		return WMSG_SUCCESS;
	}

	if( m_edCharacterName.IsFocused() )
	{
		if( ( wmsgResult = m_edCharacterName.KeyMessage( pMsg ) ) != WMSG_FAIL )
			return WMSG_SUCCESS; 
	}

	if( m_edGiftContent.IsFocused() )
	{
		if( ( wmsgResult = m_edGiftContent.KeyMessage( pMsg ) ) != WMSG_FAIL )
			return WMSG_SUCCESS; 
	}
		
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : CCashShopPopup::IMEMessage()
// Desc : 
// ----------------------------------------------------------------------------
WMSG_RESULT CUICashShopEX::CCashShopPopup::IMEMessage(MSG *pMsg)
{
	if( !IsFocused() || m_Type != CSP_Gift ) return WMSG_FAIL;

	WMSG_RESULT wmsg_Result;

	//if( m_edCharacterName.IsFocused() )
	if ((wmsg_Result = m_edCharacterName.IMEMessage(pMsg)) != WMSG_FAIL)
	{
		return wmsg_Result;
	}
	else if ((wmsg_Result = m_edGiftContent.IMEMessage( pMsg )) != WMSG_FAIL)
	{
		return wmsg_Result;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : CCashShopPopup::CharMessage()
// Desc : 
// ----------------------------------------------------------------------------
WMSG_RESULT CUICashShopEX::CCashShopPopup::CharMessage(MSG *pMsg)
{
	if( !IsFocused() || m_Type != CSP_Gift) return WMSG_FAIL;
	
	WMSG_RESULT wmsg_Result;

	if( (wmsg_Result = m_edCharacterName.CharMessage( pMsg )) != WMSG_FAIL )
	{
		return wmsg_Result; 
	}
	else if ((wmsg_Result = m_edGiftContent.CharMessage( pMsg )) != WMSG_FAIL)
	{
		return wmsg_Result;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : CCashShopPopup::SetPackageItems()
// Desc : 
// ----------------------------------------------------------------------------
void CUICashShopEX::CCashShopPopup::SetPackageItems(void)
{
	if (m_GoodsDatas.Count() > 0)
	{
		int i;
		CCashShopData::CASH_SHOP_DATA* tmpData = m_GoodsDatas[0];
// 		m_pIconPackage.SetItemInfo(-1, -1, -1, -1, -1,
// 						tmpData->m_shopItemIndex, 0, tmpData->m_itemName, tmpData->m_itemDesc, tmpData->m_itemListCnt, tmpData->m_typeIndex);
		m_pIconPackage->SetWhichUI(UI_CASHSHOP_EX);
		m_pIconPackage->setData(UBET_ITEM, tmpData->m_vItemDataArray[0].m_itemIndex);
		m_pIconPackage->setCash(tmpData->m_shopItemIndex);
		m_PackageScroll.SetCurItemCount(tmpData->m_itemListCnt);
		m_PackageScroll.SetScrollPos(0);

		for (i=0; i<tmpData->m_itemListCnt && i<5; ++i)
		{	
			m_PackageItems[i]->SetWhichUI(UI_CASHSHOP_EX);
			m_PackageItems[i]->setData(UBET_ITEM, tmpData->m_vItemDataArray[i].m_itemIndex);
			m_PackageItems[i]->setCount(tmpData->m_vItemDataArray[i].m_itemCnt);
			CItems* pItems = m_PackageItems[i]->getItems();
			pItems->Item_Flag = tmpData->m_vItemDataArray[i].m_flag;
			pItems->Item_Plus = tmpData->m_vItemDataArray[i].m_plus;
		}
	}
}

// ----------------------------------------------------------------------------
// Name : CCashShopPopup::SetPackageItems()
// Desc : 
// ----------------------------------------------------------------------------
void CUICashShopEX::CCashShopPopup::SetPackageItems(INDEX iPos)
{
	if (m_GoodsDatas.Count() > 0)
	{
		int i;
		CCashShopData::CASH_SHOP_DATA* tmpData = m_GoodsDatas[0];

		for (i = 0; i < 5; ++i)
		{
			m_PackageItems[i]->clearIconData();
		}

		for (i = 0; (i + iPos) < tmpData->m_itemListCnt && i < 5; ++i)
		{
			m_PackageItems[i]->setData(UBET_ITEM, tmpData->m_vItemDataArray[i+iPos].m_itemIndex);
			m_PackageItems[i]->setCount((__int64)tmpData->m_vItemDataArray[i+iPos].m_itemCnt);
			CItems* pItems = m_PackageItems[i]->getItems();
			pItems->Item_Flag = tmpData->m_vItemDataArray[i+iPos].m_flag;
			pItems->Item_Plus = tmpData->m_vItemDataArray[i+iPos].m_plus;			
		}
	}
}

// ----------------------------------------------------------------------------
// Name : CCashShopPopup::SendPurchaseReq()
// Desc : 
// ----------------------------------------------------------------------------
void CUICashShopEX::CCashShopPopup::SendPurchaseReq(void)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	CNetworkMessage nmItem((UBYTE)MSG_EXTEND);
	nmItem << (ULONG)MSG_EX_CASHITEM;
	nmItem << (UBYTE)MSG_EX_CASHITEM_PURCHASE_REQ;

	nmItem<<(ULONG)pUIManager->GetCashShopEX()->GetMyCash();

	nmItem<<(ULONG)m_GoodsDatas.Count();

	ULONG ulTotal = 0;

	for (int i=0; i<m_GoodsDatas.Count(); ++i)
	{
		nmItem << (ULONG)m_GoodsDatas[i]->m_shopItemIndex;
		ulTotal += m_GoodsDatas[i]->m_cash;
	}

	nmItem << ulTotal;
	pUIManager->GetCashShopEX()->SetLoadDataState(FALSE);
	_pNetwork->SendNetworkMessage(nmItem);
}

// ----------------------------------------------------------------------------
// Name : CCashShopPopup::AddFriendsList()
// Desc : 
// ----------------------------------------------------------------------------
void CUICashShopEX::CCashShopPopup::AddFriendsList(void)
{
	m_cbFriends.ResetStrings();
	m_cbFriends.AddString(_s("----"));
	std::vector<CUIGroup*>& tmpGroup = CUIManager::getSingleton()->GetMessenger()->GetGroup();

	std::vector<CUIGroup*>::iterator tmpItr;

	for (tmpItr = tmpGroup.begin(); tmpItr!=tmpGroup.end(); ++tmpItr)
	{
		CUIGroup* group = (*tmpItr);

		for (int i=0; i<group->m_vecMember.size(); ++i)
		{
			m_cbFriends.AddString(group->m_vecMember[i].m_strName);
		}
	}
}

// ----------------------------------------------------------------------------
// Name : CUICashShopEX()
// Desc : 생성자
// ----------------------------------------------------------------------------
CUICashShopEX::CUICashShopEX(const char* pcXMLPath) : CUIWindow(pcXMLPath)
{
	m_Background.Clear();
	m_RecommandBoxList.New(_RankCount);
	m_GoodsBoxList.New(_GoodsListCount);
	m_WishBoxList.New(_GoodsListCount);
	m_PurchaseItemList.New(_PurchaseListCount);
	m_PurchaseGetItemList.New(_PurchaseGetCount);
	m_RevGiftList.Clear();
	m_ViewFrameType = VFT_GoodsList;
	m_SelCategoryTab = CSC_HOTandNew;
	m_OverCategoryTab = CSC_NONE;
	m_SelMainTab = CS_IrisShop;

//	m_ModelRenderTarget = new CRenderTexture();
//	m_ModelRenderTarget->Init(_PreviewX,_PreviewY);
	m_ModelRenderTarget = NULL;

	m_SelectedCount = 0; // 최대 _PurchaseGetCount 까지
	m_SelectedGoods = -1;
	m_SelectedRecommand = -1;
	m_SelectedIven = -1;
	m_SelectedEquip = -1;
	m_SelectedGetPurchase = -1;
	m_SelectedWishList = -1;
	m_GoodsPrice = 0;
	m_iRClickMenuIndex = 0;
	m_bCashShopLock = FALSE;
	m_bLoadingData = FALSE;
	m_bPackageInfo = FALSE;
	m_bShowPurchaseOver = FALSE;
	m_bPreviewDrag = false;
}

// ----------------------------------------------------------------------------
// Name : ~CUICashShopEX()
// Desc : 소멸자
// ----------------------------------------------------------------------------
CUICashShopEX::~CUICashShopEX()
{
	int		i;

	for (i = 0; i < _PurchaseGetCount; ++i)
		SAFE_DELETE(m_PurchaseGetItemList[i]);

	m_PurchaseGetItemList.Clear();
	m_PurchaseItemList.Clear();
	m_WishBoxList.Clear();
	m_GoodsBoxList.Clear();
	m_RecommandBoxList.Clear();

	//if (m_ModelRenderTarget)
	//{
	//	delete m_ModelRenderTarget;
	//	m_ModelRenderTarget = NULL;
	//}

	for (i = 0; i < MAX_CASHINVEN; ++i)
		SAFE_DELETE(m_pIconInven[i]);

	for (i = 0; i < MAX_CASHEQUIP; ++i)
		SAFE_DELETE(m_pIconEquip[i]);

	for (i = 0; i < 5; ++i)
		SAFE_DELETE(m_pIconRevGift[i]);
}

// ----------------------------------------------------------------------------
// Name : InitRenderTarget()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::InitRenderTarget(void)
{
}

// ----------------------------------------------------------------------------
// Name : DestroyRenderTarget()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::DestroyRenderTarget(void)
{
	CUIManager::getSingleton()->DestroyRenderTarget();
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
	m_Popup.SetPosition(pixMinI, pixMinJ, pixMaxI, pixMaxJ);
	g_web.SetWebPosition(500, 400);
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	/*if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )*/ // CashShop UI의 경우, 무조건 센터로 가져가야 한다.
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );

	m_Popup.SetPosition(pixMinI, pixMinJ, pixMaxI, pixMaxJ);
	g_web.SetWebPosition(500, 400);
}

TiXmlElement* CUICashShopEX::FromXmltoCashData(CCashTypeNode& CashNode, TiXmlElement* pNode, CCashTypeNode::CTNINFO NodeType)
{
	if (pNode)
	{
		TiXmlElement* pElement = pNode;
		INDEX iRefIndex = 1;

		while (pElement != NULL)
		{
			const char* strIndex = pElement->Attribute("INDEX");
			INDEX iIndex = atoi(strIndex);

			switch(NodeType)
			{
			case CCashTypeNode::CTN_FIRST:
				{
					CashNode.AddNode(_S(iIndex, ""), CCashTypeNode::CTN_FIRST);
					TiXmlElement* pChildElement = pElement->FirstChildElement("SECOND");
					
					if (pChildElement != NULL)
					{
						FromXmltoCashData(CashNode.GetSubNode(iRefIndex), pChildElement, CCashTypeNode::CTN_SECOND);
					}
				}
				break;
			case CCashTypeNode::CTN_SECOND:
				{
					CashNode.AddNode(_S(iIndex, ""), CCashTypeNode::CTN_SECOND);
				}
				break;
			}

			pElement = (TiXmlElement*)pElement->NextSibling();
			iRefIndex++;
		}
	}

	return pNode;
}

void CUICashShopEX::InitCatalogNameNodes(void)
{
	bool bFileLoaded = false;

	if (m_strXMLFileName.Length() > 0) {
		TiXmlDocument doc;

		if (doc.LoadFile(m_strXMLFileName, TIXML_DEFAULT_ENCODING))
		{
			bFileLoaded = true;
			TiXmlHandle docH(&doc);
			TiXmlElement* pCashTypeElement = docH.FirstChild("CASHTYPE").ToElement();

			INDEX iRefIndex = 0;

			if (pCashTypeElement)
			{
				TiXmlElement* pCatalogElement = pCashTypeElement->FirstChildElement("CATALOG");
				INDEX iCatalog = 0;
				while (pCatalogElement != NULL)
				{
					const char* strIndex = pCatalogElement->Attribute("INDEX");
					INDEX iIndex = atoi(strIndex);

					m_CatalogNamesNodes[iCatalog].Create(NULL, _S(iIndex, ""), CCashTypeNode::CTN_CATALOG);
					TiXmlElement* pChildElement = pCatalogElement->FirstChildElement("FIRST");

					if (pChildElement != NULL)
					{
						FromXmltoCashData(m_CatalogNamesNodes[iCatalog], pChildElement, CCashTypeNode::CTN_FIRST);
					}

					pCatalogElement = (TiXmlElement*)pCatalogElement->NextSibling();
					iCatalog++;
				}
			}
		}
	}

	if (!bFileLoaded)
	{
		INDEX iCount = 1;
		m_CatalogNamesNodes[CSC_HOTandNew].Create(NULL, _S(5229, "HOT&NEW"), CCashTypeNode::CTN_CATALOG);
		m_CatalogNamesNodes[CSC_HOTandNew].AddNode(_S(5236, "신상품"), CCashTypeNode::CTN_FIRST);
		m_CatalogNamesNodes[CSC_HOTandNew].AddNode(_S(5237, "특가"), CCashTypeNode::CTN_FIRST);
		m_CatalogNamesNodes[CSC_HOTandNew].AddNode(_S(5238, "인기상품"), CCashTypeNode::CTN_FIRST);

		m_CatalogNamesNodes[CSC_Platinum].Create(NULL, _S(5230, "플래티늄"), CCashTypeNode::CTN_CATALOG);

		m_CatalogNamesNodes[CSC_SpendGoods].Create(NULL, _S(5231, "소비상품"), CCashTypeNode::CTN_CATALOG);
		m_CatalogNamesNodes[CSC_SpendGoods].AddNode(_S(5239, "캐릭터 성장"), CCashTypeNode::CTN_FIRST);
		m_CatalogNamesNodes[CSC_SpendGoods].GetSubNode(iCount).AddNode(_S(5248, "경험치"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_SpendGoods].GetSubNode(iCount).AddNode(_S(5249, "숙련도"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_SpendGoods].GetSubNode(iCount).AddNode(_S(5250, "확률"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_SpendGoods].GetSubNode(iCount).AddNode(_S(5251, "혼합"), CCashTypeNode::CTN_SECOND); iCount++;
		m_CatalogNamesNodes[CSC_SpendGoods].AddNode(_S(5240, "능력치 강화"), CCashTypeNode::CTN_FIRST);
		m_CatalogNamesNodes[CSC_SpendGoods].GetSubNode(iCount).AddNode(_S(5252, "HP 확장"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_SpendGoods].GetSubNode(iCount).AddNode(_S(5253, "MP 확장"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_SpendGoods].GetSubNode(iCount).AddNode(_S(5254, "공격력"), CCashTypeNode::CTN_SECOND); iCount++;
		m_CatalogNamesNodes[CSC_SpendGoods].AddNode(_S(5241, "물약"), CCashTypeNode::CTN_FIRST);
		m_CatalogNamesNodes[CSC_SpendGoods].GetSubNode(iCount).AddNode(_S(5255, "HP 회복"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_SpendGoods].GetSubNode(iCount).AddNode(_S(5256, "MP 회복"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_SpendGoods].GetSubNode(iCount).AddNode(_S(5257, "듀얼 회복"), CCashTypeNode::CTN_SECOND); iCount = 1;

		m_CatalogNamesNodes[CSC_EquipGoods].Create(NULL, _S(5232, "장비상품"), CCashTypeNode::CTN_CATALOG);
		m_CatalogNamesNodes[CSC_EquipGoods].AddNode(_S(5242, "장비"), CCashTypeNode::CTN_FIRST);
		m_CatalogNamesNodes[CSC_EquipGoods].GetSubNode(iCount).AddNode(_S(5258, "무기"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_EquipGoods].GetSubNode(iCount).AddNode(_S(5259, "갑옷"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_EquipGoods].GetSubNode(iCount).AddNode(_S(5274, "액세서리"), CCashTypeNode::CTN_SECOND); iCount++;
		m_CatalogNamesNodes[CSC_EquipGoods].AddNode(_S(5243, "장비 강화"), CCashTypeNode::CTN_FIRST);
		m_CatalogNamesNodes[CSC_EquipGoods].GetSubNode(iCount).AddNode(_S(5261, "결합주문서"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_EquipGoods].GetSubNode(iCount).AddNode(_S(5262, "소켓"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_EquipGoods].GetSubNode(iCount).AddNode(_S(5263, "블러드"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_EquipGoods].GetSubNode(iCount).AddNode(_S(5275, "제련"), CCashTypeNode::CTN_SECOND); iCount++;
		m_CatalogNamesNodes[CSC_EquipGoods].AddNode(_S(5260, "코스튬"), CCashTypeNode::CTN_FIRST);
		m_CatalogNamesNodes[CSC_EquipGoods].GetSubNode(iCount).AddNode(_S(43, "타이탄"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_EquipGoods].GetSubNode(iCount).AddNode(_S(44, "기사"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_EquipGoods].GetSubNode(iCount).AddNode(_S(45, "힐러"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_EquipGoods].GetSubNode(iCount).AddNode(_S(46, "메이지"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_EquipGoods].GetSubNode(iCount).AddNode(_S(47, "로그"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_EquipGoods].GetSubNode(iCount).AddNode(_S(48, "소서러"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_EquipGoods].GetSubNode(iCount).AddNode(_S(4410, "나이트셰도우"), CCashTypeNode::CTN_SECOND); iCount = 1;

		m_CatalogNamesNodes[CSC_Avata].Create(NULL, _S(5233, "아바타"), CCashTypeNode::CTN_CATALOG);
		m_CatalogNamesNodes[CSC_Avata].AddNode(_S(5244, "펫 상품"), CCashTypeNode::CTN_FIRST);
		m_CatalogNamesNodes[CSC_Avata].GetSubNode(iCount).AddNode(_S(5264, "펫"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_Avata].GetSubNode(iCount).AddNode(_S(5265, "펫 아이템"), CCashTypeNode::CTN_SECOND); iCount++;
		m_CatalogNamesNodes[CSC_Avata].AddNode(_S(5245, "용병"), CCashTypeNode::CTN_FIRST);
		m_CatalogNamesNodes[CSC_Avata].GetSubNode(iCount).AddNode(_S(5266, "몬스터 용병"), CCashTypeNode::CTN_SECOND); iCount = 1;

		m_CatalogNamesNodes[CSC_ServiceGoods].Create(NULL, _S(5234, "서비스상품"), CCashTypeNode::CTN_CATALOG);
		m_CatalogNamesNodes[CSC_ServiceGoods].AddNode(_S(5246, "편의 상품"), CCashTypeNode::CTN_FIRST);
		m_CatalogNamesNodes[CSC_ServiceGoods].GetSubNode(iCount).AddNode(_S(5267, "주문서"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_ServiceGoods].GetSubNode(iCount).AddNode(_S(5268, "카드"), CCashTypeNode::CTN_SECOND); iCount = 1;
		m_CatalogNamesNodes[CSC_ServiceGoods].AddNode(_S(5247, "기타"), CCashTypeNode::CTN_FIRST);

		m_CatalogNamesNodes[CSC_PackageGoods].Create(NULL, _S(5235, "패키지상품"), CCashTypeNode::CTN_CATALOG);
		m_CatalogNamesNodes[CSC_PackageGoods].AddNode(_S(5239, "캐릭터 성장"), CCashTypeNode::CTN_FIRST);
		m_CatalogNamesNodes[CSC_PackageGoods].GetSubNode(iCount).AddNode(_S(5248, "경험치"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_PackageGoods].GetSubNode(iCount).AddNode(_S(5249, "숙련도"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_PackageGoods].GetSubNode(iCount).AddNode(_S(5250, "확률"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_PackageGoods].GetSubNode(iCount).AddNode(_S(5251, "혼합"), CCashTypeNode::CTN_SECOND); iCount++;
		m_CatalogNamesNodes[CSC_PackageGoods].AddNode(_S(5240, "능력치 강화"), CCashTypeNode::CTN_FIRST);
		m_CatalogNamesNodes[CSC_PackageGoods].GetSubNode(iCount).AddNode(_S(5252, "HP 확장"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_PackageGoods].GetSubNode(iCount).AddNode(_S(5253, "MP 확장"), CCashTypeNode::CTN_SECOND); iCount++;
		m_CatalogNamesNodes[CSC_PackageGoods].AddNode(_S(5241, "물약"), CCashTypeNode::CTN_FIRST);
		m_CatalogNamesNodes[CSC_PackageGoods].GetSubNode(iCount).AddNode(_S(5255, "HP 회복"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_PackageGoods].GetSubNode(iCount).AddNode(_S(5256, "MP 회복"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_PackageGoods].GetSubNode(iCount).AddNode(_S(5257, "듀얼 회복"), CCashTypeNode::CTN_SECOND); iCount++;
		m_CatalogNamesNodes[CSC_PackageGoods].AddNode(_S(5260, "코스튬"), CCashTypeNode::CTN_FIRST);
		m_CatalogNamesNodes[CSC_PackageGoods].GetSubNode(iCount).AddNode(_S(43, "타이탄"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_PackageGoods].GetSubNode(iCount).AddNode(_S(44, "기사"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_PackageGoods].GetSubNode(iCount).AddNode(_S(45, "힐러"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_PackageGoods].GetSubNode(iCount).AddNode(_S(46, "메이지"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_PackageGoods].GetSubNode(iCount).AddNode(_S(47, "로그"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_PackageGoods].GetSubNode(iCount).AddNode(_S(48, "소서러"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_PackageGoods].GetSubNode(iCount).AddNode(_S(4410, "나이트셰도우"), CCashTypeNode::CTN_SECOND); iCount++;
		m_CatalogNamesNodes[CSC_PackageGoods].AddNode(_S(5243, "장비 강화"), CCashTypeNode::CTN_FIRST);
		m_CatalogNamesNodes[CSC_PackageGoods].GetSubNode(iCount).AddNode(_S(5261, "결합주문서"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_PackageGoods].GetSubNode(iCount).AddNode(_S(5275, "제련"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_PackageGoods].GetSubNode(iCount).AddNode(_S(5262, "소켓"), CCashTypeNode::CTN_SECOND); iCount++;
		m_CatalogNamesNodes[CSC_PackageGoods].AddNode(_S(5244, "펫 상품"), CCashTypeNode::CTN_FIRST);
		m_CatalogNamesNodes[CSC_PackageGoods].GetSubNode(iCount).AddNode(_S(5264, "펫"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_PackageGoods].GetSubNode(iCount).AddNode(_S(5265, "펫 아이템"), CCashTypeNode::CTN_SECOND); iCount++;
		m_CatalogNamesNodes[CSC_PackageGoods].AddNode(_S(5245, "용병"), CCashTypeNode::CTN_FIRST);
		m_CatalogNamesNodes[CSC_PackageGoods].AddNode(_S(5246, "편의 상품"), CCashTypeNode::CTN_FIRST);
		m_CatalogNamesNodes[CSC_PackageGoods].GetSubNode(iCount).AddNode(_S(5267, "주문서"), CCashTypeNode::CTN_SECOND);
		m_CatalogNamesNodes[CSC_PackageGoods].GetSubNode(iCount).AddNode(_S(5268, "카드"), CCashTypeNode::CTN_SECOND); iCount++;
		m_CatalogNamesNodes[CSC_PackageGoods].AddNode(_S(5247, "기타"), CCashTypeNode::CTN_FIRST);
	}
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight)
{
	CUIWindow::Create(pParentWnd, nX, nY, _uiCashSizeX, _uiCashSizeY);

	InitCatalogNameNodes();
	// Create Texture
	m_ptdBaseTexture = CreateTexture(CTString("Data\\Interface\\Cashshop.tex"));
	FLOAT fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT fTexHeight = m_ptdBaseTexture->GetPixHeight();

	BasicBackgroundInit(fTexWidth, fTexHeight); // Backgroudn surface init..
	MainTabFrameInit(fTexWidth, fTexHeight);
	CategoryFrameInit(fTexWidth, fTexHeight);
	ViewFrameInit(fTexWidth, fTexHeight);
	RecommandFrameInit(fTexWidth, fTexHeight);
	PreviewFrameInit(fTexWidth, fTexHeight);
	InvenFrameInit(fTexWidth, fTexHeight);

	m_Popup.Create(this, 0, 0, 0, 0);
	UIRect rtAnim = UIRect(86,327,110,351);
	m_LoadingAnim.Base.SetRectEx(UIRect(218,339,242,363), UIRectUV(86,327,110,351,fTexWidth,fTexHeight));
	m_LoadingAnim.NumberOfFrames = 11;
	m_LoadingAnim.speed = 0.3f;
	for (int i=0; i<11; ++i)
	{
		m_LoadingAnim.AddRectSurface(UIRect(218,339,242,363), UIRectUV(rtAnim.Left, rtAnim.Top, rtAnim.Right, rtAnim.Bottom, fTexWidth,fTexHeight));
		rtAnim.Left += 24;
		rtAnim.Right += 24;
	}

	m_LoadingBackBox.AddRectSurface(UIRect(217,338,230,351), UIRectUV(294,153,307,166,fTexWidth,fTexHeight));
	m_LoadingBackBox.AddRectSurface(UIRect(230,338,243,351), UIRectUV(371,153,384,166,fTexWidth,fTexHeight));
	m_LoadingBackBox.AddRectSurface(UIRect(217,351,230,364), UIRectUV(294,216,307,229,fTexWidth,fTexHeight));
	m_LoadingBackBox.AddRectSurface(UIRect(230,351,243,364), UIRectUV(371,216,384,229,fTexWidth,fTexHeight));
}

// ----------------------------------------------------------------------------
// Name : OpenCashShopEX()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::OpenCashShopEX()
{
	if (m_bCashShopLock)
	{
		return;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	pUIManager->InitRenderTarget();
	g_web.SetPosition(pDrawPort->dp_MinI, pDrawPort->dp_MinJ, pDrawPort->dp_MaxI, pDrawPort->dp_MaxJ);

	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->HUD_SetModelData(HUD_CHARACTER_TYPE, -1);

	int i;

	m_SelectedGoods = -1;
	m_SelectedIven = -1;
	m_SelectedRecommand = -1;
	m_SelectedEquip = -1;
	m_SelectedGetPurchase = -1;
	m_SelectedGift = -1;
	m_SelectedWishList = -1;
	m_iEquipDropCashIndex = 0;
	m_GoodsPrice = 0;
	m_hasCash = 0;
	m_iRClickMenuIndex = 0;
	m_iCurrentPage = 1;
	m_iMaxPage = 1;
	m_SelCategoryTab = CSC_HOTandNew;
	m_LoadingAnim.StartAnim();
	m_bLoadingData = FALSE;

	for (i = 0; i < MAX_CASHINVEN; ++i)
	{
		m_pIconInven[i]->clearIconData();
	}

	for (i = 0; i < MAX_CASHEQUIP; ++i)
	{
		m_pIconEquip[i]->clearIconData();
	}
	
	ClearGoodsList();
	ClearRecommandList();
	ClearWishList();
	ClearPurchaseItemList();

	pUIManager->RearrangeOrder(UI_CASHSHOP_EX, TRUE);
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUICashShopEX::MouseMessage(MSG *pMsg)
{
	if (m_Popup.IsVisible())
	{
		return m_Popup.MouseMessage(pMsg);
	}

	WMSG_RESULT wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Item clicked
	static BOOL bLButtonDownInItem = FALSE;

	// Mouse point
	static int nOldX, nOldY;
	int nX = LOWORD(pMsg->lParam);
	int nY = HIWORD(pMsg->lParam);

	if (m_bLoadingData)
	{
		switch(pMsg->message)
		{
		case WM_MOUSEMOVE:
			{
				m_btnClose.MouseMessage(pMsg);
				m_btnInven[BIT_Exit].MouseMessage(pMsg);
			}
			break;
		case WM_LBUTTONDOWN:
			{
				if (m_btnClose.MouseMessage(pMsg) != WMSG_FAIL)
				{
					return WMSG_SUCCESS;
				}
				else if (m_btnInven[BIT_Exit].MouseMessage(pMsg) != WMSG_FAIL)
				{
					return WMSG_SUCCESS;
				}
			}
			break;
		case WM_LBUTTONUP:
			{
				if ((wmsgResult = m_btnClose.MouseMessage(pMsg)) != WMSG_FAIL)
				{
					if (wmsgResult == WMSG_COMMAND)
					{
						// closing cashshopEX
						CUIManager::getSingleton()->RearrangeOrder(UI_CASHSHOP_EX, FALSE);
					}

					return WMSG_SUCCESS;
				}
				else if (m_btnInven[BIT_Exit].MouseMessage(pMsg) != WMSG_FAIL)
				{
					CUIManager::getSingleton()->RearrangeOrder(UI_CASHSHOP_EX, FALSE);
				}
			}
			break;
		}

		return WMSG_SUCCESS;
	}

	switch(pMsg->message)
	{
	case WM_MOUSEMOVE:
		{
			int ndX = nX - nOldX;
			int ndY = nY - nOldY;
			wmsgResult = WMSG_FAIL;

			CUIManager* pUIManager = CUIManager::getSingleton();

			// Hold item button
			if (pUIManager->GetDragIcon() == NULL && bLButtonDownInItem && 
				(pMsg->wParam & MK_LBUTTON) && (ndX != 0 || ndY != 0))
			{
				int nOffset = BTN_SIZE / 2;
				m_bMoveStartInven = false;

				if (m_SelectedGoods >= 0)
				{
					if (m_ViewFrameType == VFT_GoodsList)
					{
						pUIManager->SetHoldBtn(m_GoodsBoxList[m_SelectedGoods].pIconGoods);

						bLButtonDownInItem = FALSE;
						m_SelectedGoods = -1;
					}
					else if (m_ViewFrameType == VFT_PurchaseList)
					{
						pUIManager->SetHoldBtn(m_PurchaseItemList[m_SelectedGoods].pIconGoods);

						bLButtonDownInItem = FALSE;
						m_SelectedGoods = -1;
					}
					else if (m_ViewFrameType == VFT_WishList)
					{
						pUIManager->SetHoldBtn(m_WishBoxList[m_SelectedGoods].pIconGoods);

						bLButtonDownInItem = FALSE;
						m_SelectedGoods = -1;
					}
				}
				else if (m_SelectedRecommand >= 0)
				{
					if (m_ViewFrameType == VFT_GoodsList)
					{
						pUIManager->SetHoldBtn(m_RecommandBoxList[m_SelectedRecommand].pIconGoods);

						bLButtonDownInItem = FALSE;
						m_SelectedRecommand = -1;
					}
				}
				else if (m_SelectedEquip >= 0)
				{
					if (m_ViewFrameType == VFT_GoodsList || m_ViewFrameType == VFT_WishList)
					{
						pUIManager->SetHoldBtn(m_pIconEquip[m_SelectedEquip]);
						m_bPreviewDrag = true;
// 						m_iEquipDropCashIndex = m_iEquipCashIndex[m_SelectedEquip];
// 
// 						if (m_iEquipDropCashIndex > 0)
// 						{
// 							for (int i=0; i<MAX_CASHEQUIP; ++i)
// 							{
// 								if (m_iEquipCashIndex[i] == m_iEquipDropCashIndex)
// 								{
// 									m_pIconEquip[i]->clearIconData();
// 									SetTakeOffModel(i+1);
// 									m_iEquipCashIndex[i] = 0;
// 								}
// 							}
// 						}
// 						else
// 						{
// 							m_pIconEquip[m_SelectedEquip]->clearIconData();
// 							SetTakeOffModel(m_SelectedEquip+1);
// 						}

						bLButtonDownInItem = FALSE;
						//m_SelectedEquip = -1;
					}
				}
				else if (m_SelectedIven >= 0)
				{
					if (m_ViewFrameType == VFT_GoodsList || m_ViewFrameType == VFT_WishList)
					{
						pUIManager->SetHoldBtn(m_pIconInven[m_SelectedIven]);
						m_bMoveStartInven = true;
						//CCashShopData::CASH_SHOP_DATA* csdInfo = _pNetwork->GetCashShopData().GetCashShopData(m_pIconInven[m_SelectedIven]->GetCashIndex());
						//if (csdInfo->m_cash > 0)
						//{
						//	m_GoodsPrice -= csdInfo->m_cash;
						//}
						//m_pIconInven[m_SelectedIven]->clearIconData();
						bLButtonDownInItem = FALSE;						
					}
				}
			}

			m_tpCommand.MouseMessage(pMsg);
			m_btnClose.MouseMessage(pMsg);

			m_bPackageInfo = FALSE; // 패키지 버튼에 마우스 오버시 돋보기 커서로 변경

			for (int i=0; i<CSF_Total; ++i)
			{
				if (WMSG_FAIL != MouseMoveLoop(pMsg, static_cast<CashShopFrame>(i)))
				{
					wmsgResult = WMSG_SUCCESS;
				}
			}

			if (IsInside(nX, nY))
			{
				if (m_bPackageInfo)
				{
					pUIManager->SetMouseCursorInsideUIs(UMCT_ZOOMIN);
				}
				else
				{
					pUIManager->SetMouseCursorInsideUIs();
				}
			}

			if (wmsgResult == WMSG_SUCCESS)
			{
				return wmsgResult;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if (m_tpCommand.MouseMessage(pMsg) != WMSG_FAIL)
			{
				return WMSG_SUCCESS;
			}
			else
			{
				m_tpCommand.Hide();
			}

			if (IsInside(nX, nY))
			{
				CUIManager* pUIManager = CUIManager::getSingleton();

				if( !pUIManager->DoesMessageBoxExist(MSGCMD_CASH_EX_ERROR) )
				{
					pUIManager->RearrangeOrder(UI_CASHSHOP_EX, TRUE);
				}

				nOldX = nX; nOldY = nY;
				int i;

				if (m_btnClose.MouseMessage(pMsg) != WMSG_FAIL)
				{

				}
				else if (m_ViewFrameType == VFT_GoodsList && (wmsgResult = m_cbCategoryType[CST_First].MouseMessage(pMsg)) != WMSG_FAIL)
				{
					if (wmsgResult == WMSG_COMMAND)
					{
						m_cbCategoryType[CST_Second].ResetStrings();
						m_cbCategoryType[CST_Second].AddString(_s("----"));
						SetSubComboBox(m_SelCategoryTab, m_cbCategoryType[CST_First].GetCurSel(), 0);
						m_cbCategoryType[CST_Second].SetCurSel(0);
					}

					return WMSG_SUCCESS;
				}
				else if (m_ViewFrameType == VFT_GoodsList && (wmsgResult = m_cbCategoryType[CST_Second].MouseMessage(pMsg)) != WMSG_FAIL)
				{
					if (wmsgResult == WMSG_COMMAND)
					{

					}

					return WMSG_SUCCESS;
				}
				else if (IsInsideRect(nX, nY, m_CashShopFrame[CSF_MainTab]))
				{
					for (i=0; i<CS_Total; ++i)
					{
						if (m_SelMainTab != static_cast<CS_MainTab>(i) &&
							m_BtnMainTab[i].MouseMessage(pMsg) != WMSG_FAIL)
						{
							m_BtnMainTab[m_SelMainTab].SetBtnState(UBS_IDLE);

							if (CS_PurchaseInfo == i) // 구매정보 요청
							{
								m_bLoadingData = TRUE;
								_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_PURCHASELIST_REQ);
							}
							else if (CS_GotTheGift == i) // 받은 선물 요청
							{
								m_bLoadingData = TRUE;
								_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_GIFT_RECVLIST_REQ);
							}
							else if (CS_WishPage == i) // 소망 상자요청
							{
								m_bLoadingData = TRUE;
								_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_WISHLIST_REQ);
							}
							else if (CS_IrisShop == i)
							{
								if (!m_bLoadingData)
								{
									m_ViewFrameType = VFT_GoodsList;
									m_SelMainTab = static_cast<CS_MainTab>(i);
								}
							}
						}
					}
				}
				else if (IsInsideRect(nX, nY, m_CashShopFrame[CSF_Category]))
				{
					for (i=0; i<CSC_Total; ++i)
					{
						if ((m_SelCategoryTab != static_cast<CashShopCategory>(i) || m_SelMainTab != CS_IrisShop) &&
							m_BtnCategory[i].MouseMessage(pMsg) != WMSG_FAIL)
						{
							m_BtnCategory[m_SelCategoryTab].SetBtnState(UBS_IDLE);
							m_SelCategoryTab = static_cast<CashShopCategory>(i);
							m_bLoadingData = TRUE;
							_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_LIST_REQ, (i+1)*10000);
							return WMSG_SUCCESS;
						}
					}

					if (m_BtnSearch.MouseMessage(pMsg) != WMSG_FAIL)
					{
						return WMSG_SUCCESS;
					}
				}
				else if (IsInsideRect(nX, nY, m_CashShopFrame[CSF_View]))
				{
					if (m_ViewFrameType == VFT_GoodsList)
					{
						for (i=0; i<8; ++i)
						{
							if (m_GoodsBoxList[i].GoodsData != NULL)
							{
								if (m_GoodsBoxList[i].pIconGoods->MouseMessage(pMsg) != WMSG_FAIL)
								{
									m_SelectedGoods = i;
									bLButtonDownInItem = TRUE;
									return WMSG_SUCCESS;
								}

								if (m_GoodsBoxList[i].GoodsData->IsPackageEx())
								{
									if (m_GoodsBoxList[i].btnPkgInfo.MouseMessage(pMsg) != WMSG_FAIL)
									{
										return WMSG_SUCCESS;
									}
								}
							}
						}

						for (i=0; i<VPT_Total; ++i)
						{
							if (m_BtnPage[i].MouseMessage(pMsg) != WMSG_FAIL)
							{
								return WMSG_SUCCESS;
							}
						}
					}
					else if (m_ViewFrameType == VFT_PurchaseList)
					{
						for (i=0; i<_PurchaseListCount; ++i)
						{
							if (!m_PurchaseItemList[i].pIconGoods->IsEmpty())
							{
								if (m_PurchaseItemList[i].pIconGoods->MouseMessage(pMsg) != WMSG_FAIL)
								{
									if (!m_PurchaseItemList[i].bSelected)
									{
										m_SelectedGoods = i;
										bLButtonDownInItem = TRUE;
									}
									return WMSG_SUCCESS;
								}
							}
						}

						for (i=0; i<_PurchaseGetCount; ++i)
						{
							if (!m_PurchaseGetItemList[i]->IsEmpty())
							{
								if (m_PurchaseGetItemList[i]->MouseMessage(pMsg) != WMSG_FAIL)
								{
									m_SelectedGetPurchase = i;
									bLButtonDownInItem = TRUE;
									return WMSG_SUCCESS;
								}
							}
						}

						if (m_BtnMove.MouseMessage(pMsg) != WMSG_FAIL)
						{
							return WMSG_SUCCESS;
						}
						else if (m_BtnGift.MouseMessage(pMsg) != WMSG_FAIL)
						{
							return WMSG_SUCCESS;
						}
						else if (m_BtnGet.MouseMessage(pMsg) != WMSG_FAIL)
						{
							return WMSG_SUCCESS;
						}
					}
					else if (m_ViewFrameType == VFT_GiftList)
					{
						if (IsInsideRect(nX, nY, m_rtGiftDataList))
						{
							int nSel = (nY - m_rtGiftDataList.Top - m_nPosY) / 53;
							
							if (!m_pIconRevGift[nSel]->IsEmpty())
							{
								m_SelectedGift = nSel+m_GiftScroll.GetScrollPos();
								m_edGiftContent.ResetString();
								CTString strMessage = m_RevGiftList[m_GiftScroll.GetScrollPos()+nSel].strMessage;
								pUIManager->InitCurInfoLines();
								pUIManager->AddItemInfoString(strMessage, 0xFFFFFFFF, 10, 32);

								for (i=0; i<pUIManager->m_nCurInfoLines; ++i)
								{
									m_edGiftContent.AddString(pUIManager->m_strItemInfo[i].str_String);
								}
							}
							// 선물 목록 선택
							return WMSG_SUCCESS;
						}
						else if (m_BtnGet.MouseMessage(pMsg) != WMSG_FAIL)
						{
							return WMSG_SUCCESS;
						}
						else if ((wmsgResult =m_GiftScroll.MouseMessage(pMsg)) != WMSG_FAIL)
						{
							if (wmsgResult == WMSG_COMMAND)
							{
								SetRevGiftDatas(m_GiftScroll.GetScrollPos());
							}
							return WMSG_SUCCESS;
						}
					}
					else if (m_ViewFrameType == VFT_WishList)
					{
						for (i=0; i<8; ++i)
						{
							if (m_WishBoxList[i].GoodsData != NULL)
							{
								if (m_WishBoxList[i].pIconGoods->MouseMessage(pMsg) != WMSG_FAIL)
								{
									m_SelectedGoods = i;
									bLButtonDownInItem = TRUE;
									return WMSG_SUCCESS;
								}

								if (m_WishBoxList[i].GoodsData->IsPackageEx())
								{
									if (m_WishBoxList[i].btnPkgInfo.MouseMessage(pMsg) != WMSG_FAIL)
									{
										return WMSG_SUCCESS;
									}
								}
							}
						}
					}
				}
				else if (IsInsideRect(nX, nY, m_CashShopFrame[CSF_Recommand]))
				{
					if (m_ViewFrameType == VFT_GoodsList || m_ViewFrameType == VFT_WishList)
					{
						for (i=0; i<5; ++i)
						{
							if (m_RecommandBoxList[i].GoodsData != NULL)
							{
								if (m_RecommandBoxList[i].pIconGoods->MouseMessage(pMsg) != WMSG_FAIL)
								{
									m_SelectedRecommand = i;
									bLButtonDownInItem = TRUE;
									return WMSG_SUCCESS;
								}
							}
						}
					}
				}
				else if (IsInsideRect(nX, nY, m_CashShopFrame[CSF_Preview]))
				{
					if (m_ViewFrameType == VFT_GoodsList || m_ViewFrameType == VFT_WishList)
					{
						for (i=0; i<MAX_CASHEQUIP; ++i)
						{
							if (!m_pIconEquip[i]->IsEmpty())
							{
								if (m_pIconEquip[i]->MouseMessage(pMsg) != WMSG_FAIL)
								{
									m_SelectedEquip = i;
									bLButtonDownInItem = TRUE;
									return WMSG_SUCCESS;
								}
							}
						}
					}

					if (m_btnRestore.MouseMessage(pMsg) != WMSG_FAIL)
					{
						return WMSG_SUCCESS;
					}
				}
				else if (IsInsideRect(nX, nY, m_CashShopFrame[CSF_Inven]))
				{
					if (m_ViewFrameType == VFT_GoodsList || m_ViewFrameType == VFT_WishList)
					{
						for (i=0; i<MAX_CASHINVEN; ++i)
						{
							if (!m_pIconInven[i]->IsEmpty())
							{
								if (m_pIconInven[i]->MouseMessage(pMsg) != WMSG_FAIL)
								{
									m_SelectedIven = i;
									bLButtonDownInItem = TRUE;
									return WMSG_SUCCESS;
								}
							}
						}
					}

					for (i=0; i<BIT_Total; ++i)
					{
						if (m_btnInven[i].MouseMessage(pMsg) != WMSG_FAIL)
						{
							return WMSG_SUCCESS;
						}
					}
				}

				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			bLButtonDownInItem = FALSE;
			m_SelectedIven = -1;
			m_SelectedGoods = -1;
			m_SelectedRecommand = -1;

			int nTmpEquipIndex = m_SelectedEquip;
			m_SelectedEquip = -1;

			if (m_bPreviewDrag == true)
			{
				m_bPreviewDrag = false;

				m_iEquipDropCashIndex = m_iEquipCashIndex[nTmpEquipIndex];

				if (m_iEquipDropCashIndex > 0)
				{
					for (int i=0; i<MAX_CASHEQUIP; ++i)
					{
						if (m_iEquipCashIndex[i] == m_iEquipDropCashIndex)
						{
							m_pIconEquip[i]->clearIconData();
							SetTakeOffModel(i+1);
							m_iEquipCashIndex[i] = 0;
						}
					}
				}
				else
				{
					m_pIconEquip[nTmpEquipIndex]->clearIconData();
					SetTakeOffModel(nTmpEquipIndex+1);
				}

				
				return WMSG_FAIL;
			}

			if(m_tpCommand.MouseMessage(pMsg) != WMSG_FAIL)
			{
				INDEX iIndex = m_tpCommand.GetCurSel();
				RunRClickMenu(m_tpCommand.GetCommandNum(iIndex));
				m_tpCommand.Hide();
				return WMSG_SUCCESS;
			}
			else
			{
				m_tpCommand.Hide();
				m_iRClickMenuIndex = 0;
			}

			CUIManager* pUIManager = CUIManager::getSingleton();

			if (pUIManager->GetDragIcon() == NULL)
			{
				if (!IsFocused())
				{
					return WMSG_SUCCESS; //WMSG_FAIL;
				}

				int i;

				if ((wmsgResult = m_btnClose.MouseMessage(pMsg)) != WMSG_FAIL)
				{
					if (wmsgResult == WMSG_COMMAND)
					{
						// closing cashshopEX
						pUIManager->RearrangeOrder(UI_CASHSHOP_EX, FALSE);
					}

					return WMSG_SUCCESS;
				}
				else if (m_ViewFrameType == VFT_GoodsList && m_cbCategoryType[CST_First].MouseMessage(pMsg) != WMSG_FAIL)
				{
					return WMSG_SUCCESS;
				}
				else if (m_ViewFrameType == VFT_GoodsList && m_cbCategoryType[CST_Second].MouseMessage(pMsg) != WMSG_FAIL)
				{
					return WMSG_SUCCESS;
				}
				else if (IsInsideRect(nX, nY, m_CashShopFrame[CSF_MainTab]))
				{ // 탭형태이기 때문에 굳이 Button Up 처리가 필요하지 않을 듯 함.
				}
				else if (IsInsideRect(nX, nY, m_CashShopFrame[CSF_Category]))
				{
					if (m_BtnSearch.MouseMessage(pMsg) != WMSG_FAIL)
					{
						// send search command
						if (m_ViewFrameType == VFT_GoodsList)
						{
							m_bLoadingData = TRUE;
							_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_LIST_REQ, GetSearchType());
						}
						return WMSG_SUCCESS;
					}
				}
				else if (IsInsideRect(nX, nY, m_CashShopFrame[CSF_View]))
				{
					if (m_ViewFrameType == VFT_GoodsList)
					{
						for (i=0; i<8; ++i)
						{
							if (m_GoodsBoxList[i].GoodsData != NULL)
							{
								if (m_GoodsBoxList[i].pIconGoods->MouseMessage(pMsg) != WMSG_FAIL)
								{
									return WMSG_SUCCESS;
								}

								if (m_GoodsBoxList[i].GoodsData->IsPackageEx())
								{
									if (m_GoodsBoxList[i].btnPkgInfo.MouseMessage(pMsg) != WMSG_FAIL)
									{
										// 아이템 구성 정보를 보자.(세트 상품)
										m_Popup.ClearGoodsData();
										m_Popup.AddGoodsData(m_GoodsBoxList[i].GoodsData);
										m_Popup.OpenPopup(CCashShopPopup::CSP_Package);
										return WMSG_SUCCESS;
									}
								}
							}
						}

						for (i=0; i<VPT_Total; ++i)
						{
							if (m_BtnPage[i].MouseMessage(pMsg) != WMSG_FAIL)
							{
								// send page move
								if (VPT_Begin == i)
								{
									if (m_iCurrentPage > 1)
									{
										m_bLoadingData = TRUE;
										_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_LIST_REQ, m_CurrentPageType);
									}
								}
								else if (VPT_Front == i)
								{
									if (m_iCurrentPage < m_iMaxPage)
									{
										m_bLoadingData = TRUE;
										_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_LIST_REQ, m_CurrentPageType, m_iCurrentPage+1);
									}
								}
								else if (VPT_Back == i)
								{
									if (m_iCurrentPage > 1)
									{
										m_bLoadingData = TRUE;
										_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_LIST_REQ, m_CurrentPageType, m_iCurrentPage-1);
									}
								}
								else if (VPT_End == i)
								{
									if (m_iCurrentPage < m_iMaxPage)
									{
										m_bLoadingData = TRUE;
										_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_LIST_REQ, m_CurrentPageType, m_iMaxPage);
									}
								}
								return WMSG_SUCCESS;
							}
						}
					}
					else if (m_ViewFrameType == VFT_PurchaseList)
					{
						for (i=0; i<_PurchaseListCount; ++i)
						{
							if (!m_PurchaseItemList[i].pIconGoods->IsEmpty())
							{
								if (m_PurchaseItemList[i].pIconGoods->MouseMessage(pMsg) != WMSG_FAIL)
								{
									return WMSG_SUCCESS;
								}
							}
						}

						for (i=0; i<_PurchaseGetCount; ++i)
						{
							if (!m_PurchaseGetItemList[i]->IsEmpty())
							{
								if (m_PurchaseGetItemList[i]->MouseMessage(pMsg) != WMSG_FAIL)
								{
									return WMSG_SUCCESS;
								}
							}
						}

						if (m_BtnMove.MouseMessage(pMsg) != WMSG_FAIL) // 모두 옮기기
						{
							MovePurchaseGoods();
							return WMSG_SUCCESS;
						}
						else if (m_BtnGift.MouseMessage(pMsg) != WMSG_FAIL)
						{
							m_Popup.ClearGoodsData();
							AddGoodsBringData();
							m_Popup.OpenPopup(CCashShopPopup::CSP_Gift);
							return WMSG_SUCCESS;
						}
						else if (m_BtnGet.MouseMessage(pMsg) != WMSG_FAIL)
						{
							SendBringReq();
							return WMSG_SUCCESS;
						}
					}
					else if (m_ViewFrameType == VFT_GiftList)
					{
						if (m_BtnGet.MouseMessage(pMsg) != WMSG_FAIL)
						{
							if (m_SelectedGift >= 0)
							{
								SendGetRevGiftItem(m_SelectedGift);
							}
							// 선물 가져오기
							return WMSG_SUCCESS;
						}
						else if (m_GiftScroll.MouseMessage(pMsg) != WMSG_FAIL)
						{
							// 
							return WMSG_SUCCESS;
						}
					}
					else if (m_ViewFrameType == VFT_WishList)
					{
						for (i=0; i<8; ++i)
						{
							if (m_WishBoxList[i].GoodsData != NULL)
							{
								if (m_WishBoxList[i].pIconGoods->MouseMessage(pMsg) != WMSG_FAIL)
								{
									return WMSG_SUCCESS;
								}

								if (m_WishBoxList[i].GoodsData->IsPackageEx())
								{
									if (m_WishBoxList[i].btnPkgInfo.MouseMessage(pMsg) != WMSG_FAIL)
									{
										// 아이템 구성 정보를 보자.(세트 상품)
										m_Popup.ClearGoodsData();
										m_Popup.AddGoodsData(m_WishBoxList[i].GoodsData);
										m_Popup.OpenPopup(CCashShopPopup::CSP_Package);
										return WMSG_SUCCESS;
									}
								}
							}
						}
					}
				}
				else if (IsInsideRect(nX, nY, m_CashShopFrame[CSF_Recommand]))
				{
					if (m_ViewFrameType == VFT_GoodsList || m_ViewFrameType == VFT_WishList)
					{
						for (i=0; i<_RankCount; ++i)
						{
							if (m_RecommandBoxList[i].GoodsData != NULL)
							{
								if (m_RecommandBoxList[i].pIconGoods->MouseMessage(pMsg) != WMSG_FAIL)
								{
									return WMSG_SUCCESS;
								}
							}
						}
					}
				}
				else if (IsInsideRect(nX, nY, m_CashShopFrame[CSF_Preview]))
				{
					if (m_ViewFrameType == VFT_GoodsList || m_ViewFrameType == VFT_WishList)
					{
						for (i=0; i<MAX_CASHEQUIP; ++i)
						{
							if (!m_pIconEquip[i]->IsEmpty())
							{
								if (m_pIconEquip[i]->MouseMessage(pMsg) != WMSG_FAIL)
								{
									return WMSG_SUCCESS;
								}
							}
						}
					}

					if (m_btnRestore.MouseMessage(pMsg) != WMSG_FAIL)
					{
						// initialize preview model
						for (i=0; i<(MAX_CASHEQUIP+1); ++i)
						{
							SetTakeOffModel(i);
							if (i>0)
							{
								m_iEquipCashIndex[i-1] = 0;
								m_pIconEquip[i-1]->clearIconData();
							}
						}

						return WMSG_SUCCESS;
					}
				}
				else if (IsInsideRect(nX, nY, m_CashShopFrame[CSF_Inven]))
				{
					if (m_ViewFrameType == VFT_GoodsList || m_ViewFrameType == VFT_WishList)
					{
						for (i=0; i<MAX_CASHINVEN; ++i)
						{
							if (!m_pIconInven[i]->IsEmpty())
							{
								if (m_pIconInven[i]->MouseMessage(pMsg) != WMSG_FAIL)
								{
									return WMSG_SUCCESS;
								}
							}
						}
					}

					for (i=0; i<BIT_Total; ++i)
					{
						if (m_btnInven[i].MouseMessage(pMsg) != WMSG_FAIL)
						{
							// execute Inven command
							switch(i)
							{
							case BIT_Clear: // 장바구니 비우기
								{
									for (int j=0; j<MAX_CASHINVEN; ++j)
									{
										m_pIconInven[j]->clearIconData();
									}
									m_GoodsPrice = 0;
								}
								break;
							case BIT_PurchaseItem: // 아이템 구매하기
								{
									m_Popup.ClearGoodsData();
									for (i=0; i<MAX_CASHINVEN; ++i)
									{
										if (!m_pIconInven[i]->IsEmpty())
										{
											m_Popup.AddGoodsData(_pNetwork->GetCashShopData().GetCashShopData(m_pIconInven[i]->GetCashIndex()));
										}
									}
									m_Popup.OpenPopup(CCashShopPopup::CSP_Purchase);
								}
								break;
							case BIT_CashCharge: // 캐쉬 충전
								{
#if defined(G_BRAZIL)
									{
										if ( IsFullScreen( sam_bFullScreenActive))
										{												 
											pUIManager->GetOption()->ChangeWindowMode();
											pUIManager->DestroyRenderTarget();
											pUIManager->InitRenderTarget();
										}										
										
										if (g_iCountry == BRAZIL)
											ShellExecute( NULL, "open", "https://pt.gamigo.com/lcsa/pt/cashshop/login", NULL,NULL, SW_SHOWNORMAL );
										else  //if (g_iCountry == MEXICO)
											ShellExecute( NULL, "open", "https://es.gamigo.com/lcsa/es/cashshop/login", NULL,NULL, SW_SHOWNORMAL );
									}
#elif defined(G_USA)
									{
										if ( IsFullScreen( sam_bFullScreenActive))
										{												 
											 pUIManager->GetOption()->ChangeWindowMode();
											 pUIManager->DestroyRenderTarget();
											 pUIManager->InitRenderTarget();
										}										
										ShellExecute( NULL, "open", "https://en.gamigo.com/lcus/en/cashshop/index/", NULL,NULL, SW_SHOWNORMAL );
									}
#elif defined(G_RUSSIA )
									{
										if ( IsFullScreen( sam_bFullScreenActive))
										{												 
											 pUIManager->GetOption()->ChangeWindowMode();
											 pUIManager->DestroyRenderTarget();
											 pUIManager->InitRenderTarget();
										}										
										ShellExecute( NULL, "open", "https://ru.gamigo.com/cashshop/", NULL,NULL, SW_SHOWNORMAL );										
									}
									// sam -->|
#elif defined(G_GERMAN)
									{
										CNetworkMessage nmMessage(MSG_EXTEND);
										nmMessage << (INDEX)MSG_EX_CASH_AUTHENTICATION_CODE;

										_pNetwork->SendToServerNew( nmMessage );
									}
#else
									{
										g_web.SetWebMoveWindow();
										g_web.SendWebPageOpenMsg(TRUE);

										CTString strCallUrl;
#if defined G_KOR // 국내는 웹 결제 페이지를 호출시, 파트너 코드를 파라미터로 보낸다.
										extern BOOL g_bAutoLogin;
										if (g_bAutoLogin)
										{
											strCallUrl.PrintF(_S(5318, "http://lastchaos.barunsongames.com/cash_shop/cash_login.asp?Partner=%s"), _pNetwork->m_strUserCID);
										}
										else
										{
											strCallUrl.PrintF(_S(5318, "http://lastchaos.barunsongames.com/cash_shop/cash_login.asp?Partner=%s"), CTString("or"));
										}
										g_web.SetWebUrl(std::string(strCallUrl.str_String));
#else
										strCallUrl = _S(5318, "http://lastchaos.barunsongames.com/cash_shop/cash_login.asp");
										g_web.SetWebUrl(std::string(strCallUrl.str_String));
#endif
									}
#endif
								}
								break;
							case BIT_Exit: // 종료
								{
									pUIManager->RearrangeOrder(UI_CASHSHOP_EX, FALSE);
								}
								break;
							}

							return WMSG_SUCCESS;
						}
					}
				}
			}
			else
			{
				if (IsInside(nX, nY))
				{
					CUIIcon* pDrag = pUIManager->GetDragIcon();

					if (pDrag->getBtnType() == UBET_ITEM && pDrag->GetWhichUI() == UI_CASHSHOP_EX)
					{
						if (IsInsideRect(nX, nY, m_CashShopFrame[CSF_Preview]))
						{ // 장비 아이템만 등록이 되어야 한다.
							if (m_ViewFrameType == VFT_GoodsList || m_ViewFrameType == VFT_WishList)
							{
								INDEX iCashIndex = 0;
								CCashShopData::CASH_SHOP_DATA* tmpData = NULL;

								if (pDrag->GetCashIndex() < 0)
								{
									tmpData = _pNetwork->GetCashShopData().GetCashShopData(m_iEquipDropCashIndex);
									iCashIndex = m_iEquipDropCashIndex;
								}
								else
								{
									tmpData = _pNetwork->GetCashShopData().GetCashShopData(pDrag->GetCashIndex());
								}

								if (tmpData != NULL)
								{
									if (tmpData->IsPackageEx())
									{
										SetPackageWearingModel(pDrag, iCashIndex);
									}
									else if (pDrag->IsSuit() == true)
									{ // UniIndex에 한벌의상 아이템 Index가 등록되었다.
										SetSuitWearingModel(pDrag->getIndex(), iCashIndex);
									}
									else
									{
										SetWearingModel(pDrag);
									}

									if (m_pIconInven[m_SelectedIven] != NULL && 
										pDrag == m_pIconInven[m_SelectedIven])
									{
										CCashShopData::CASH_SHOP_DATA* csdInfo = _pNetwork->GetCashShopData().GetCashShopData(m_pIconInven[m_SelectedIven]->GetCashIndex());
										if (csdInfo != NULL && csdInfo->m_cash > 0)
										{
											m_GoodsPrice -= csdInfo->m_cash;
										}
										m_pIconInven[m_SelectedIven]->clearIconData();

										m_SelectedIven = -1;
									}
								}
							}
						}
						else if (IsInsideRect(nX, nY, m_PurchaseGetItemRect))
						{
							if (m_ViewFrameType == VFT_PurchaseList)
							{
								for (int i=0; i<_PurchaseGetCount; ++i)
								{
									if (m_PurchaseGetItemList[i]->IsEmpty())
									{
										m_PurchaseGetItemList[i]->copyItem(pDrag);

										for (int j=0; j<_PurchaseListCount; ++j)
										{
											CItems* pPurchaseItems = m_PurchaseItemList[j].pIconGoods->getItems();
											CItems* pGetItems = m_PurchaseGetItemList[i]->getItems();
											
											if (pPurchaseItems != NULL && pGetItems != NULL &&
												pPurchaseItems->Item_UniIndex == pGetItems->Item_UniIndex)
											{
												m_PurchaseItemList[j].bSelected = TRUE;
												m_PurchaseItemList[j].bEnable = FALSE;
												break;
											}
										}
										break;
									}
								}
							}
						}
						// 인벤에서 선택된 아이템은 다시 인벤에 세팅 되면 안됨.
						else if (m_bMoveStartInven == false && IsInsideRect(nX, nY, m_btnExInvenRect))
						{ 
							// 캐시 인벤토리로 등록 한다.
							if (m_ViewFrameType == VFT_GoodsList || m_ViewFrameType == VFT_WishList)
							{
								for (int i=0; i<MAX_CASHINVEN; ++i)
								{
									if (m_pIconInven[i]->IsEmpty())
									{
										CCashShopData::CASH_SHOP_DATA* csdInfo = NULL;
										if (m_iEquipDropCashIndex > 0)
										{
											csdInfo = _pNetwork->GetCashShopData().GetCashShopData(m_iEquipDropCashIndex);
											m_pIconInven[i]->setCash(csdInfo->m_shopItemIndex);											
										}
										else
										{
											m_pIconInven[i]->copyItem(pDrag);
											csdInfo = _pNetwork->GetCashShopData().GetCashShopData(m_pIconInven[i]->GetCashIndex());
										}

										if (csdInfo->m_cash > 0)
										{
											m_GoodsPrice += csdInfo->m_cash;
										}
										break;
									}
								}
							}
						}
					}
				}

				pUIManager->ResetHoldBtn();
				m_iEquipDropCashIndex = 0;
			}
		}
		break;
	case WM_RBUTTONDOWN:
		{
			if (IsInside(nX, nY))
			{
				int i;
				INDEX iItem = 0;

				if (m_ViewFrameType == VFT_GoodsList)
				{
					for (i=0; i<_RankCount; ++i)
					{
						if (m_RecommandBoxList[i].GoodsData != NULL)
						{
							UIRect rtTmpBox = m_RecommandBoxList[i].BoxSurface.m_RectSurfaceArray[0].m_RT;

							if (IsInsideRect(nX, nY, rtTmpBox))
							{
								m_tpCommand.SetPos(nX-m_nPosX, nY-m_nPosY);
								SetRClickMenu(m_RecommandBoxList[i].GoodsData->m_shopItemIndex);
								return WMSG_SUCCESS;
							}
						}
					}

					for (i=0; i<8; ++i)
					{
						if (m_GoodsBoxList[i].GoodsData != NULL)
						{
							UIRect rtTmpBox = m_GoodsBoxList[i].BoxSurface.m_RectSurfaceArray[0].m_RT;

							if (IsInsideRect(nX, nY, rtTmpBox))
							{
								m_tpCommand.SetPos(nX-m_nPosX, nY-m_nPosY);
								SetRClickMenu(m_GoodsBoxList[i].GoodsData->m_shopItemIndex);
								return WMSG_SUCCESS;
							}
						}
					}
				}
				else if (m_ViewFrameType == VFT_WishList)
				{
					for (i=0; i<8; ++i)
					{
						if (m_WishBoxList[i].GoodsData != NULL)
						{
							UIRect rtTmpBox = m_WishBoxList[i].BoxSurface.m_RectSurfaceArray[0].m_RT;

							if (IsInsideRect(nX, nY, rtTmpBox))
							{
								m_tpCommand.SetPos(nX-m_nPosX, nY-m_nPosY);
								SetRClickMenu(m_WishBoxList[i].GoodsData->m_shopItemIndex);
								return WMSG_SUCCESS;
							}
						}
					}
				}

				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			int i, j;

			if (IsInside(nX, nY))
			{
				if (IsInsideRect(nX, nY, m_CashShopFrame[CSF_View]))
				{
					if (m_ViewFrameType == VFT_GoodsList)
					{
						for (i=0; i<8; ++i)
						{
							if (m_GoodsBoxList[i].GoodsData != NULL)
							{
								if (m_GoodsBoxList[i].pIconGoods->MouseMessage(pMsg) != WMSG_FAIL)
								{
									for (j=0; j<MAX_CASHINVEN; ++j)
									{
										if (m_pIconInven[j]->IsEmpty())
										{
											m_pIconInven[j]->copyItem(m_GoodsBoxList[i].pIconGoods);
											CCashShopData::CASH_SHOP_DATA* csdInfo = _pNetwork->GetCashShopData().GetCashShopData(m_pIconInven[j]->GetCashIndex());
											if (csdInfo->m_cash > 0)
											{
												m_GoodsPrice += csdInfo->m_cash;
											}
											return WMSG_SUCCESS;
										}
									}
								}
							}
						}
					}
					else if (m_ViewFrameType == VFT_PurchaseList)
					{
						for (i=0; i<_PurchaseListCount; ++i)
						{
							if (!m_PurchaseItemList[i].pIconGoods->IsEmpty())
							{
								if (m_PurchaseItemList[i].pIconGoods->MouseMessage(pMsg) != WMSG_FAIL)
								{
									if (!m_PurchaseItemList[i].bSelected)
									{
										for (j=0; j<_PurchaseGetCount; ++j)
										{
											if (m_PurchaseGetItemList[j]->IsEmpty())
											{
												m_PurchaseGetItemList[j]->copyItem(m_PurchaseItemList[i].pIconGoods);
												// 옮겨진 물품 설정
												m_PurchaseItemList[i].bSelected = TRUE;
												m_PurchaseItemList[i].bEnable = FALSE;
												return WMSG_SUCCESS;
											}
										}
									}
								}
							}
						}

						for (i=0; i<_PurchaseGetCount; ++i)
						{
							if (!m_PurchaseGetItemList[i]->IsEmpty())
							{
								if (m_PurchaseGetItemList[i]->MouseMessage(pMsg) != WMSG_FAIL)
								{
									for (j=0; j<_PurchaseListCount; ++j)
									{
										CItems* pPurchaseItems = m_PurchaseItemList[j].pIconGoods->getItems();
										CItems* pGetItems = m_PurchaseGetItemList[i]->getItems();

										if (pPurchaseItems != NULL && pGetItems != NULL &&
											pPurchaseItems->Item_UniIndex == pGetItems->Item_UniIndex)
										{
											m_PurchaseItemList[j].bSelected = FALSE;
											m_PurchaseItemList[j].bEnable = FALSE;
										}
									}
									m_PurchaseGetItemList[i]->clearIconData();
									ArrangePurchaseGetGoods();
									return WMSG_SUCCESS;
								}
							}
						}
					}
					else if (m_ViewFrameType == VFT_WishList)
					{
						for (i=0; i<8; ++i)
						{
							if (m_WishBoxList[i].GoodsData != NULL)
							{
								if (m_WishBoxList[i].pIconGoods->MouseMessage(pMsg) != WMSG_FAIL)
								{
									for (j=0; j<MAX_CASHINVEN; ++j)
									{
										if (m_pIconInven[j]->IsEmpty())
										{
											m_pIconInven[j]->copyItem(m_WishBoxList[i].pIconGoods);
											CCashShopData::CASH_SHOP_DATA* csdInfo = 
												_pNetwork->GetCashShopData().GetCashShopData(m_pIconInven[j]->GetCashIndex());
											if (csdInfo->m_cash > 0)
											{
												m_GoodsPrice += csdInfo->m_cash;
											}
											return WMSG_SUCCESS;
										}
									}
								}
							}
						}
					}
				}
				else if (IsInsideRect(nX, nY, m_CashShopFrame[CSF_Recommand]) && m_ViewFrameType == VFT_GoodsList)
				{
					for (i=0; i<_RankCount; ++i)
					{
						if (m_RecommandBoxList[i].GoodsData != NULL)
						{
							if (m_RecommandBoxList[i].pIconGoods->MouseMessage(pMsg) != WMSG_FAIL)
							{
								for (j=0; j<MAX_CASHINVEN; ++j)
								{
									if (m_pIconInven[j]->IsEmpty())
									{
										m_pIconInven[j]->copyItem(m_RecommandBoxList[i].pIconGoods);
										CCashShopData::CASH_SHOP_DATA* csdInfo = 
											_pNetwork->GetCashShopData().GetCashShopData(m_pIconInven[j]->GetCashIndex());
										if (csdInfo->m_cash > 0)
										{
											m_GoodsPrice += csdInfo->m_cash;
										}
										return WMSG_SUCCESS;
									}
								}
							}
						}
					}
				}
				else if (IsInsideRect(nX, nY, m_CashShopFrame[CSF_Preview]))
				{
					for (i=0; i<MAX_CASHEQUIP; ++i)
					{
						if (!m_pIconEquip[i]->IsEmpty())
						{
							if (m_pIconEquip[i]->MouseMessage(pMsg) != WMSG_FAIL)
							{
								SetTakeOffModel(i+1);
								m_iEquipCashIndex[i] = 0;
								m_pIconEquip[i]->clearIconData();
								return WMSG_SUCCESS;
							}
						}
					}
				}
				else if (IsInsideRect(nX, nY, m_btnExInvenRect))
				{
					for (i=0; i<MAX_CASHINVEN; ++i)
					{
						if (m_pIconInven[i]->MouseMessage(pMsg) != WMSG_FAIL)
						{
							if (!m_pIconInven[i]->IsEmpty())
							{
								CCashShopData::CASH_SHOP_DATA* csdInfo = 
									_pNetwork->GetCashShopData().GetCashShopData(m_pIconInven[i]->GetCashIndex());
								if (csdInfo->m_cash > 0)
								{
									m_GoodsPrice -= csdInfo->m_cash;
								}
								m_pIconInven[i]->clearIconData();
								return WMSG_SUCCESS;
							}
						}
					}
				}

				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_MOUSEWHEEL:
		{
			if (m_ViewFrameType == VFT_GiftList)
			{
				if (IsInside(nX, nY))
				{
					if (m_GiftScroll.MouseMessage(pMsg) != WMSG_FAIL)
					{
						SetRevGiftDatas(m_GiftScroll.GetScrollPos());
						return WMSG_SUCCESS;
					}
				}
			}
		}
		break;
	}

	return WMSG_SUCCESS; //WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : KeyMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUICashShopEX::KeyMessage(MSG *pMsg)
{
	if (m_Popup.IsVisible())
	{
		return m_Popup.KeyMessage(pMsg);
	}
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : CharMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUICashShopEX::CharMessage(MSG *pMsg)
{
	if (m_Popup.IsVisible())
	{
		return m_Popup.CharMessage(pMsg);
	}
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : IMEMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUICashShopEX::IMEMessage(MSG *pMsg)
{
	if (m_Popup.IsVisible())
	{
		return m_Popup.IMEMessage(pMsg);
	}
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::MsgBoxCommand(int nCommandCode, BOOL bOK, CTString &strInput)
{

}
// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::MsgBoxCommand(int nCommandCode, BOOL bOK, CTString &strInput ,CTString &strConfirm)
{

}
// ----------------------------------------------------------------------------
// Name : MsgBoxLCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::MsgBoxLCommand(int nCommandCode, int nResult)
{

}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::Render()
{
	int i = 0;
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	pDrawPort->InitTextureData(m_ptdBaseTexture);
	m_Background.SetPos(m_nPosX, m_nPosY);
	m_Background.RenderRectSurface(pDrawPort, 0xFFFFFFFF);

	m_btnClose.Render();

/// CashShop Frame MainTab begin() ///////////////////////////////////////////////////////////////////////////////////////
	for (i=0; i<CS_Total; ++i)
	{
		m_BtnMainTab[i].Render();
	}
///  CashShop Frame MainTab end() ////////////////////////////////////////////////////////////////////////////////////////
///	CashShop Frame Category begin() //////////////////////////////////////////////////////////////////////////////////////
	for (i=0; i<CSC_Total; ++i)
	{
		m_BtnCategory[i].Render();

		if (m_SelCategoryTab == static_cast<CashShopCategory>(i))
		{
			UIRect rtTmpBox = UIRect(m_BtnCategory[i].GetAbsPosX(), m_BtnCategory[i].GetAbsPosY(), 
				m_BtnCategory[i].GetAbsPosX()+60, m_BtnCategory[i].GetAbsPosY()+60);

			pDrawPort->AddTexture(rtTmpBox.Left, rtTmpBox.Top, rtTmpBox.Right, rtTmpBox.Bottom,
				m_UVSelCategory.U0, m_UVSelCategory.V0, m_UVSelCategory.U1, m_UVSelCategory.V1, 0xFFFFFFFF);
		}
		else if (m_OverCategoryTab == static_cast<CashShopCategory>(i))
		{
			UIRect rtTmpBox = UIRect(m_BtnCategory[i].GetAbsPosX(), m_BtnCategory[i].GetAbsPosY(), 
				m_BtnCategory[i].GetAbsPosX()+60, m_BtnCategory[i].GetAbsPosY()+60);

			pDrawPort->AddTexture(rtTmpBox.Left, rtTmpBox.Top, rtTmpBox.Right, rtTmpBox.Bottom,
				m_UVSelCategory.U0, m_UVSelCategory.V0, m_UVSelCategory.U1, m_UVSelCategory.V1, 0xFFFFFFFF);
		}
	}

	if (m_OverCategoryTab >= 0)
	{
		pDrawPort->PutTextEx(m_CatalogNamesNodes[m_OverCategoryTab].GetName(), m_nPosX+21, m_nPosY+147, 0xFFE97FFF);
	}
	else
	{
		pDrawPort->PutTextEx(m_CatalogNamesNodes[m_SelCategoryTab].GetName(), m_nPosX+21, m_nPosY+147, 0xFFFFFFFF);
	}

	m_BtnSearch.Render();
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();

	pDrawPort->InitTextureData(m_ptdBaseTexture);
///	CashShop Frame Category end() ////////////////////////////////////////////////////////////////////////////////////////
///	CashShop Frame View begin() ////////////////////////////////////////////////////////////////////////////////////////
	switch(m_ViewFrameType)
	{
	case VFT_GoodsList:
		{
			m_blueBoxGoodsList.SetPos(m_nPosX, m_nPosY);
			m_blueBoxGoodsList.RenderRectSurface(pDrawPort, 0xFFFFFFFF);

			for (i=0; i<8; ++i)
			{
				m_GoodsBoxList[i].BoxSurface.SetPos(m_nPosX, m_nPosY);
				m_GoodsBoxList[i].BoxSurface.RenderRectSurface(pDrawPort, 0xFFFFFFFF);

				if (m_GoodsBoxList[i].GoodsData == NULL)
				{
					UIRect rtTmpBox = m_GoodsBoxList[i].BoxSurface.m_RectSurfaceArray[0].m_RT;
					pDrawPort->AddTexture(m_nPosX+rtTmpBox.Left+9, m_nPosY+rtTmpBox.Top+35, m_nPosX+rtTmpBox.Left+41, m_nPosY+rtTmpBox.Top+67,
						m_UVbtnEXLock.U0, m_UVbtnEXLock.V0, m_UVbtnEXLock.U1, m_UVbtnEXLock.V1, 0xFFFFFFFF);
				}
				else
				{
					int nTmpX, nTmpY;
					UIRect rtTmpBox = m_GoodsBoxList[i].BoxSurface.m_RectSurfaceArray[0].m_RT;
					nTmpX = m_GoodsBoxList[i].BoxSurface.m_RectSurfaceArray[0].m_RT.Left + m_nPosX + 45;
					nTmpY = m_GoodsBoxList[i].BoxSurface.m_RectSurfaceArray[0].m_RT.Top + m_nPosY + 35;

					if (m_GoodsBoxList[i].GoodsData->IsPackageEx())
					{
						m_GoodsBoxList[i].btnPkgInfo.Render();
						nTmpX += 24;
					}
					else
					{
						if (m_GoodsBoxList[i].GoodsData->m_itemListCnt > 0)
						{
							CTString strCount;
							CCashShopData::ITEM_DATA tmpData = m_GoodsBoxList[i].GoodsData->m_vItemDataArray[0];
							strCount.PrintF(CTString("X %d"), tmpData.m_itemCnt);
							pDrawPort->PutTextEx(strCount, m_nPosX+rtTmpBox.Left+45, m_nPosY+rtTmpBox.Top+57, 0xFFFFFFFF); // 아이템 갯수
						}
					}

					if (m_GoodsBoxList[i].GoodsData->IsNew()) // 신상품
					{
						pDrawPort->AddTexture(nTmpX, nTmpY, nTmpX+22, nTmpY+11,
							m_uvFlag[CFI_New].U0, m_uvFlag[CFI_New].V0, m_uvFlag[CFI_New].U1, m_uvFlag[CFI_New].V1, 0xFFFFFFFF);
						nTmpX += (22+1);
					}

					if (m_GoodsBoxList[i].GoodsData->IsPopular()) // 인기 상품
					{
						pDrawPort->AddTexture(nTmpX, nTmpY, nTmpX+22, nTmpY+11,
							m_uvFlag[CFI_Popular].U0, m_uvFlag[CFI_Popular].V0, m_uvFlag[CFI_Popular].U1, m_uvFlag[CFI_Popular].V1, 0xFFFFFFFF);
						nTmpX += (22+1);
					}

					if (m_GoodsBoxList[i].GoodsData->IsDiscount()) // 특가 상품
					{
						pDrawPort->AddTexture(nTmpX, nTmpY, nTmpX+22, nTmpY+11,
							m_uvFlag[CFI_Discount].U0, m_uvFlag[CFI_Discount].V0, m_uvFlag[CFI_Discount].U1, m_uvFlag[CFI_Discount].V1, 0xFFFFFFFF);
						nTmpX += (22+1);
					}

					pDrawPort->PutTextEx(m_GoodsBoxList[i].GoodsData->m_itemName, 
						m_nPosX+rtTmpBox.Left+21, m_nPosY+rtTmpBox.Top+10, 0xFFFFFFFF); // 아이템 이름
					CTString strPrice, strRemain, strCash;
					strCash.PrintF("%d", m_GoodsBoxList[i].GoodsData->m_cash);
					pUIManager->InsertCommaToString(strCash);
					strPrice.PrintF(_S(255, "가격: %s"), strCash);

					if (m_GoodsBoxList[i].GoodsData->m_limitCnt > 0)
					{
						strRemain.PrintF(_S(2890, "남은 수량: %d"), m_GoodsBoxList[i].GoodsData->m_limitCnt);
					}
					else if (m_GoodsBoxList[i].GoodsData->m_limitCnt == 0)
					{
						strRemain = _S(2891, "남은 수량 : 품절");
					}

					pDrawPort->PutTextExRX(strPrice, m_nPosX+rtTmpBox.Left+203, m_nPosY+rtTmpBox.Top+41, 0xFFFFFFFF); // 상품 가격
					pDrawPort->PutTextExRX(strRemain, m_nPosX+rtTmpBox.Left+203, m_nPosY+rtTmpBox.Top+54, 0xFFFFFFFF); // 남은 수량
				}
			}

			for (i=0; i<VPT_Total; ++i)
			{
				m_BtnPage[i].Render();
			}

			m_BlackBoxPage.SetPos(m_nPosX, m_nPosY);
			m_BlackBoxPage.RenderRectSurface(pDrawPort, 0xFFFFFFFF);
			CTString strPage;
			strPage.PrintF("%d / %d", m_iCurrentPage, m_iMaxPage);
			pDrawPort->PutTextExCX(strPage, m_nPosX+233, m_nPosY+514, 0xFFFFFFFF);

			pDrawPort->FlushRenderingQueue();
			pDrawPort->EndTextEx();

			for (i=0; i<8; ++i)
			{
				if (m_GoodsBoxList[i].GoodsData != NULL)
				{
					m_GoodsBoxList[i].pIconGoods->Render(pDrawPort);
				}
			}
		}
		break;
	case VFT_PurchaseList:
		{
			m_blueBoxPurchase.SetPos(m_nPosX, m_nPosY);
			m_blueBoxPurchase.RenderRectSurface(pDrawPort, 0xFFFFFFFF);
			
			m_BtnMove.Render();
			m_BtnGet.Render();
			m_BtnGift.Render();
//345
			pDrawPort->PutTextExCX(_S(2392, "구매한 물품"), m_nPosX+124, m_nPosY+218, 0xFFFFFFFF);
			pDrawPort->PutTextExCX(_S(2393, "옮길 물품"), m_nPosX+345, m_nPosY+218, 0xFFFFFFFF);

			pDrawPort->FlushRenderingQueue();
			pDrawPort->EndTextEx();

			for (i=0; i<_PurchaseListCount; ++i)
			{
				if (!m_PurchaseItemList[i].pIconGoods->IsEmpty())
				{
					m_PurchaseItemList[i].pIconGoods->Render(pDrawPort);

					if (m_PurchaseItemList[i].bSelected)
					{
						int tmpX, tmpY;
						m_PurchaseItemList[i].pIconGoods->GetAbsPos(tmpX, tmpY);
						pDrawPort->AddTexture(tmpX, tmpY, tmpX+32, tmpY+32,
							m_UVSelCategory.U0, m_UVSelCategory.V0, m_UVSelCategory.U1, m_UVSelCategory.V1, 0xFFFFFFFF);
					}
				}
			}

			for (i=0; i<_PurchaseGetCount; ++i)
			{
				if (!m_PurchaseGetItemList[i]->IsEmpty())
				{
					m_PurchaseGetItemList[i]->Render(pDrawPort);
				}
			}

			pDrawPort->InitTextureData(m_ptdBaseTexture);

			for (i=0; i<_PurchaseListCount; ++i)
			{
				int tmpX, tmpY;
				if (m_PurchaseItemList[i].bSelected)
				{
					m_PurchaseItemList[i].pIconGoods->GetAbsPos(tmpX, tmpY);
					pDrawPort->AddTexture(tmpX, tmpY, tmpX+32, tmpY+32,
						m_UVSelCategory.U0, m_UVSelCategory.V0, m_UVSelCategory.U1, m_UVSelCategory.V1, 0xFFFFFFFF);
				}
				else if (m_PurchaseItemList[i].bEnable)
				{
					m_PurchaseItemList[i].pIconGoods->GetAbsPos(tmpX, tmpY);
					pDrawPort->AddTexture(tmpX, tmpY, tmpX+32, tmpY+32,
						m_UVSelCategory.U0, m_UVSelCategory.V0, m_UVSelCategory.U1, m_UVSelCategory.V1, 0xFFE97FFF);					
				}
			}

			pDrawPort->FlushRenderingQueue();
		}
		break;
	case VFT_GiftList:
		{
			m_blueBoxGoodsList.SetPos(m_nPosX, m_nPosY);
			m_blueBoxGoodsList.RenderRectSurface(pDrawPort, 0xFFFFFFFF);
			m_BlackBoxRevGiftList.SetPos(m_nPosX, m_nPosY);
			m_BlackBoxRevGiftList.RenderRectSurface(pDrawPort, 0xFFFFFFFF);

			m_BtnGet.Render();
			m_GiftScroll.Render();
			pDrawPort->PutTextExCX(_S(3098, "받은 선물함"), m_nPosX+124, m_nPosY+218, 0xFFFFFFFF);
			pDrawPort->PutTextExCX(_S(3101, "선물 메시지"), m_nPosX+345, m_nPosY+218, 0xFFFFFFFF);

			pDrawPort->FlushRenderingQueue();
			pDrawPort->EndTextEx();
			m_edGiftContent.Render();

			for (i=0; i<5; ++i)
			{
				if (!m_pIconRevGift[i]->IsEmpty())
				{
					m_pIconRevGift[i]->Render(pDrawPort);
				}
			}
			pDrawPort->FlushBtnRenderingQueue(UBET_ITEM);

			for (i=0; i<5; ++i)
			{
				if (!m_pIconRevGift[i]->IsEmpty())
				{
					INDEX iRef = m_GiftScroll.GetScrollPos() + i;
					CTString nTemp;
					struct tm *tmDate =localtime((time_t*)&m_RevGiftList[iRef].ulDate);
					nTemp.PrintF(_S( 3091, "%d년 %d월 %d일" ), tmDate->tm_year+1900, tmDate->tm_mon+1, tmDate->tm_mday);
					pDrawPort->PutTextEx(m_RevGiftList[iRef].strName, 61+m_nPosX,m_pIconRevGift[i]->GetAbsPosY()+4, 0xFFFFFFFF);
					pDrawPort->PutTextEx(nTemp,61+m_nPosX, m_pIconRevGift[i]->GetAbsPosY()+19, 0xFFFFFFFF);
				}
			}
			pDrawPort->EndTextEx();
			pDrawPort->InitTextureData(m_ptdBaseTexture);
			
			if ((m_SelectedGift-m_GiftScroll.GetScrollPos()) >= 0 && (m_SelectedGift-m_GiftScroll.GetScrollPos()) < 5)
			{
				INDEX iRef = m_SelectedGift-m_GiftScroll.GetScrollPos();
				if (!m_pIconRevGift[iRef]->IsEmpty())
				{
					UIRect rtTmpBox = UIRect(m_pIconRevGift[iRef]->GetAbsPosX(), m_pIconRevGift[iRef]->GetAbsPosY(), 
						m_pIconRevGift[iRef]->GetAbsPosX()+197, m_pIconRevGift[iRef]->GetAbsPosY()+34);

					pDrawPort->AddTexture(rtTmpBox.Left, rtTmpBox.Top, rtTmpBox.Right, rtTmpBox.Bottom,
						m_UVSelCategory.U0, m_UVSelCategory.V0, m_UVSelCategory.U1, m_UVSelCategory.V1, 0xFFFFFFFF);
				}
			}

			pDrawPort->FlushRenderingQueue();
		}
		break;
	case VFT_WishList:
		{
			m_blueBoxGoodsList.SetPos(m_nPosX, m_nPosY);
			m_blueBoxGoodsList.RenderRectSurface(pDrawPort, 0xFFFFFFFF);

			for (i=0; i<8; ++i)
			{
				m_WishBoxList[i].BoxSurface.SetPos(m_nPosX, m_nPosY);
				m_WishBoxList[i].BoxSurface.RenderRectSurface(pDrawPort, 0xFFFFFFFF);

				if (m_WishBoxList[i].GoodsData == NULL)
				{
					UIRect rtTmpBox = m_WishBoxList[i].BoxSurface.m_RectSurfaceArray[0].m_RT;
					pDrawPort->AddTexture(m_nPosX+rtTmpBox.Left+9, m_nPosY+rtTmpBox.Top+35, m_nPosX+rtTmpBox.Left+41, m_nPosY+rtTmpBox.Top+67,
						m_UVbtnEXLock.U0, m_UVbtnEXLock.V0, m_UVbtnEXLock.U1, m_UVbtnEXLock.V1, 0xFFFFFFFF);
				}
				else
				{
					int nTmpX, nTmpY;
					UIRect rtTmpBox = m_WishBoxList[i].BoxSurface.m_RectSurfaceArray[0].m_RT;
					if (m_WishBoxList[i].GoodsData->IsPackageEx())
					{
						m_WishBoxList[i].btnPkgInfo.Render();
					}

					nTmpX = m_WishBoxList[i].BoxSurface.m_RectSurfaceArray[0].m_RT.Left + m_nPosX + 45;
					nTmpY = m_WishBoxList[i].BoxSurface.m_RectSurfaceArray[0].m_RT.Top + m_nPosY + 35;

					if (m_WishBoxList[i].GoodsData->IsPackageEx())
					{
						m_WishBoxList[i].btnPkgInfo.Render();
						nTmpX += 24;
					}
					else
					{
						if (m_WishBoxList[i].GoodsData->m_itemListCnt > 0)
						{
							CTString strCount;
							CCashShopData::ITEM_DATA tmpData = m_WishBoxList[i].GoodsData->m_vItemDataArray[0];
							strCount.PrintF(CTString("X %d"), tmpData.m_itemCnt);
							pDrawPort->PutTextEx(strCount, m_nPosX+rtTmpBox.Left+45, m_nPosY+rtTmpBox.Top+57, 0xFFFFFFFF); // 아이템 갯수
						}
					}

					if (m_WishBoxList[i].GoodsData->IsNew()) // 신상품
					{
						pDrawPort->AddTexture(nTmpX, nTmpY, nTmpX+22, nTmpY+11,
							m_uvFlag[CFI_New].U0, m_uvFlag[CFI_New].V0, m_uvFlag[CFI_New].U1, m_uvFlag[CFI_New].V1, 0xFFFFFFFF);
						nTmpX += (22+2);
					}

					if (m_WishBoxList[i].GoodsData->IsPopular()) // 인기 상품
					{
						pDrawPort->AddTexture(nTmpX, nTmpY, nTmpX+22, nTmpY+11,
							m_uvFlag[CFI_Popular].U0, m_uvFlag[CFI_Popular].V0, m_uvFlag[CFI_Popular].U1, m_uvFlag[CFI_Popular].V1, 0xFFFFFFFF);
						nTmpX += (22+2);
					}

					if (m_WishBoxList[i].GoodsData->IsDiscount()) // 특가 상품
					{
						pDrawPort->AddTexture(nTmpX, nTmpY, nTmpX+22, nTmpY+11,
							m_uvFlag[CFI_Discount].U0, m_uvFlag[CFI_Discount].V0, m_uvFlag[CFI_Discount].U1, m_uvFlag[CFI_Discount].V1, 0xFFFFFFFF);
						nTmpX += (22+2);
					}

					pDrawPort->PutTextEx(m_WishBoxList[i].GoodsData->m_itemName, 
						m_nPosX+rtTmpBox.Left+21, m_nPosY+rtTmpBox.Top+10, 0xFFFFFFFF); // 아이템 이름
					CTString strPrice, strRemain, strCash;
					strCash.PrintF("%d", m_WishBoxList[i].GoodsData->m_cash);
					pUIManager->InsertCommaToString(strCash);
					strPrice.PrintF(_S(255, "가격: %s"), strCash);
					strRemain.PrintF(_S(2890, "남은 수량: %d"), m_WishBoxList[i].GoodsData->m_limitCnt);
					pDrawPort->PutTextExRX(strPrice, m_nPosX+rtTmpBox.Left+203, m_nPosY+rtTmpBox.Top+46, 0xFFFFFFFF); // 상품 가격
				}
			}

			pDrawPort->FlushRenderingQueue();
			pDrawPort->EndTextEx();

			for (i=0; i<8; ++i)
			{
				if (m_WishBoxList[i].GoodsData != NULL)
				{
					m_WishBoxList[i].pIconGoods->Render(pDrawPort);
				}
			}
		}
		break;
	}

	pDrawPort->InitTextureData(m_ptdBaseTexture);
///	CashShop Frame View end() ////////////////////////////////////////////////////////////////////////////////////////
///	CashShop Frame Recommand begin() /////////////////////////////////////////////////////////////////////////////////////
	for (i=0; i<_RankCount; ++i)
	{
		m_RecommandBoxList[i].BoxSurface.SetPos(m_nPosX, m_nPosY);
		m_RecommandBoxList[i].BoxSurface.RenderRectSurface(pDrawPort, 0xFFFFFFFF);

		if (m_RecommandBoxList[i].GoodsData == NULL)
		{
			UIRect rtTmpBox = m_RecommandBoxList[i].BoxSurface.m_RectSurfaceArray[0].m_RT;
			pDrawPort->AddTexture(m_nPosX+rtTmpBox.Left+32, m_nPosY+rtTmpBox.Top+7, m_nPosX+rtTmpBox.Left+64, m_nPosY+rtTmpBox.Top+39,
				m_UVbtnEXLock.U0, m_UVbtnEXLock.V0, m_UVbtnEXLock.U1, m_UVbtnEXLock.V1, 0xFFFFFFFF);
		}
		else
		{
			UIRect rtTmpBox = m_RecommandBoxList[i].BoxSurface.m_RectSurfaceArray[0].m_RT;
			CTString strPrice;
			strPrice.PrintF(CTString("%d"), m_RecommandBoxList[i].GoodsData->m_cash);
			pUIManager->InsertCommaToString(strPrice);
			pDrawPort->PutTextExCX(strPrice, m_nPosX+rtTmpBox.Left+35, m_nPosY+rtTmpBox.Top+47, 0xFFFFFFFF);
		}
	}
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();

	for (i=0; i<_RankCount; ++i)
	{
		if (m_RecommandBoxList[i].GoodsData != NULL)
		{
			m_RecommandBoxList[i].pIconGoods->Render(pDrawPort);
		}
	}

/// CashShop Frame Recommand end() ///////////////////////////////////////////////////////////////////////////////////////
///	CashShop Frame Preview begin() ///////////////////////////////////////////////////////////////////////////////////////
	// Preview character begin

	//if (m_ModelRenderTarget != NULL)
	if (pUIManager->GetRenderTarget() != NULL)
	{
		m_ModelRenderTarget = pUIManager->GetRenderTarget();
		pDrawPort->Unlock();
		m_ModelRenderTarget->Begin();
		m_ModelRenderTarget->Clear(0x00000000);
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->HUD_DrawModel(pDrawPort, 0, 0, _PreviewX, _PreviewY, HUD_CHARACTER_TYPE, -3.2f, -1.0f);
		m_ModelRenderTarget->End();
		pDrawPort->Lock();
		// Preview character end

		FLOAT fU = _PreviewX/(FLOAT)_RTWidth;

		pDrawPort->InitTextureData(&m_ModelRenderTarget->rt_tdTexture);
		pDrawPort->AddTexture(m_nPosX+m_rtPreview.Left, m_nPosY+m_rtPreview.Top, m_nPosX+m_rtPreview.Right, m_nPosY+m_rtPreview.Bottom,
			0.0f,0.0f,fU,1.0f,0xFFFFFFFF);
		pDrawPort->FlushRenderingQueue();
		m_ModelRenderTarget = NULL;
	}

	for (i=0; i<MAX_CASHEQUIP; ++i)
	{
		if (!m_pIconEquip[i]->IsEmpty())
		{
			m_pIconEquip[i]->Render(pDrawPort);
		}
	}

	pDrawPort->InitTextureData(m_ptdBaseTexture);
	m_btnRestore.Render();
/// CashShop Frame Preview end() /////////////////////////////////////////////////////////////////////////////////////////
/// CashShop Frame Iventory begin() //////////////////////////////////////////////////////////////////////////////////////
	pDrawPort->PutTextExRX(_S(2389, "상품 금액"),  m_nPosX+630,m_nPosY+392, 0xFFFFFFFF);
	pDrawPort->PutTextExRX(_S(2390, "보유 캐쉬"), m_nPosX+630,m_nPosY+418, 0xFFFFFFFF);

	CTString strHasCash, strPrice;

	strHasCash.PrintF("%I64d", m_hasCash);
	pUIManager->InsertCommaToString(strHasCash);
	strPrice.PrintF("%I64d", m_GoodsPrice);
	pUIManager->InsertCommaToString(strPrice);

	pDrawPort->PutTextExRX(strPrice, m_nPosX+730,m_nPosY+392, 0xFFFFFFFF);
	pDrawPort->PutTextExRX(strHasCash, m_nPosX+730,m_nPosY+418, 0xFFFFFFFF);

	for (i=0; i<BIT_Total; ++i)
	{
		m_btnInven[i].Render();
	}
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();

	for (i=0; i<MAX_CASHINVEN; ++i)
	{
		if (!m_pIconInven[i]->IsEmpty())
		{
			m_pIconInven[i]->Render(pDrawPort);
		}
	}

/// CashShop Frame Iventory end() //////////////////////////////////////////////////////////////////////////////////////
	
	pDrawPort->InitTextureData(m_ptdBaseTexture);

	for (i=0; i<CST_Total; ++i)
	{
		m_cbCategoryType[i].Render();
	}

	m_tpCommand.Render();
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();

	pDrawPort->InitTextureData(m_ptdBaseTexture);
	if (m_bLoadingData)
	{
		m_LoadingAnim.SetPos(m_nPosX, m_nPosY);
		m_LoadingBackBox.SetPos(m_nPosX,m_nPosY);
		m_LoadingBackBox.RenderRectSurface(pDrawPort, 0xFFFFFFFF);
		m_LoadingAnim.Render(pDrawPort, 0xFFFFFFFF);
	}
	pDrawPort->FlushRenderingQueue();
	m_Popup.Render();
}
// ----------------------------------------------------------------------------
// Name : BasicBackgroundInit()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::BasicBackgroundInit(FLOAT fTexWidth, FLOAT fTexHeight)
{
	// T_L, T_M, T_R
	m_Background.AddRectSurface(UIRect(0,0,190,42), UIRectUV(0,0,189,41,fTexWidth,fTexHeight)); // T_L
	m_Background.AddRectSurface(UIRect(190,0,558,42), UIRectUV(190,0,320,41,fTexWidth,fTexHeight)); // T_M
	m_Background.AddRectSurface(UIRect(558,0,750,42), UIRectUV(321,0,511,41,fTexWidth,fTexHeight)); // T_R
	// M_L, M_M, M_R
	m_Background.AddRectSurface(UIRect(0,42,190,523), UIRectUV(0,42,189,119,fTexWidth,fTexHeight)); // M_L
	m_Background.AddRectSurface(UIRect(190,42,558,523), UIRectUV(190,42,320,119,fTexWidth,fTexHeight)); // M_M
	m_Background.AddRectSurface(UIRect(558,42,750,523), UIRectUV(321,42,511,119,fTexWidth,fTexHeight)); // M_R
	// B_L, B_M, B_R
	m_Background.AddRectSurface(UIRect(0,523,190,550), UIRectUV(0,120,189,145,fTexWidth,fTexHeight)); // B_L
	m_Background.AddRectSurface(UIRect(190,523,558,550), UIRectUV(190,120,320,145,fTexWidth,fTexHeight)); // B_M
	m_Background.AddRectSurface(UIRect(558,523,750,550), UIRectUV(321,120,511,145,fTexWidth,fTexHeight)); // B_R

	m_UVbtnEXLock.SetUV(288,291,320,323,fTexWidth,fTexHeight);

	m_CashShopFrame[CSF_MainTab].SetRect(7,36,462,58);
	m_CashShopFrame[CSF_Category].SetRect(7,58,462,168);
	m_CashShopFrame[CSF_View].SetRect(7,173,462,542);
	m_CashShopFrame[CSF_Recommand].SetRect(466,36,545,542);
	m_CashShopFrame[CSF_Preview].SetRect(549,36,743,300);
	m_CashShopFrame[CSF_Inven].SetRect(549,303,743,542);
	m_btnExInvenRect.SetRect(553,308,739,385);

	m_btnClose.Create(this, CTString(""), 716,3,16,16);
	m_btnClose.SetUV(UBS_IDLE, 122,258,138,274, fTexWidth, fTexHeight);
	m_btnClose.SetUV(UBS_CLICK, 140,258,156,274, fTexWidth, fTexHeight);
	m_btnClose.CopyUV(UBS_IDLE, UBS_DISABLE);
	m_btnClose.CopyUV(UBS_IDLE, UBS_ON);

	m_UVbtnEXInfo[CUIManager::UV_UL].SetUV(294,153,301,160, fTexWidth, fTexHeight);
	m_UVbtnEXInfo[CUIManager::UV_UM].SetUV(301,153,377,160, fTexWidth, fTexHeight);
	m_UVbtnEXInfo[CUIManager::UV_UR].SetUV(377,153,384,160, fTexWidth, fTexHeight);

	m_UVbtnEXInfo[CUIManager::UV_ML].SetUV(294,160,301,222, fTexWidth, fTexHeight);
	m_UVbtnEXInfo[CUIManager::UV_MM].SetUV(301,160,377,222, fTexWidth, fTexHeight);
	m_UVbtnEXInfo[CUIManager::UV_MR].SetUV(377,160,384,222, fTexWidth, fTexHeight);

	m_UVbtnEXInfo[CUIManager::UV_LL].SetUV(294,222,301,229, fTexWidth, fTexHeight);
	m_UVbtnEXInfo[CUIManager::UV_LM].SetUV(301,222,377,229, fTexWidth, fTexHeight);
	m_UVbtnEXInfo[CUIManager::UV_LR].SetUV(377,222,384,229, fTexWidth, fTexHeight);

	m_tpCommand.Create(this, 0, 0, 100, 200, _pUIFontTexMgr->GetLineHeight(), 6, 3, 1, TRUE, TRUE);
	m_tpCommand.SetScrollBar(FALSE);
	m_tpCommand.SetSelBar(100, _pUIFontTexMgr->GetLineHeight()+2, 287,421,341,475, fTexWidth,fTexHeight);
	m_tpCommand.SetOverColor(0xF8E1B5FF);
	m_tpCommand.SetSelectColor(0xF8E1B5FF);
	m_tpCommand.Hide();

	m_tpCommand.m_rtBackUL.SetUV(294,153,301,160, fTexWidth, fTexHeight);
	m_tpCommand.m_rtBackUM.SetUV(301,153,377,160, fTexWidth, fTexHeight);
	m_tpCommand.m_rtBackUR.SetUV(377,153,384,160, fTexWidth, fTexHeight);
	m_tpCommand.m_rtBackML.SetUV(294,160,301,222, fTexWidth, fTexHeight);
	m_tpCommand.m_rtBackMM.SetUV(301,160,377,222, fTexWidth, fTexHeight);
	m_tpCommand.m_rtBackMR.SetUV(377,160,384,222, fTexWidth, fTexHeight);
	m_tpCommand.m_rtBackLL.SetUV(294,222,301,229, fTexWidth, fTexHeight);
	m_tpCommand.m_rtBackLM.SetUV(301,222,377,229, fTexWidth, fTexHeight);
	m_tpCommand.m_rtBackLR.SetUV(377,222,384,229, fTexWidth, fTexHeight);
}

// ----------------------------------------------------------------------------
// Name : MainTabFrameInit()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::MainTabFrameInit(FLOAT fTexWidth, FLOAT fTexHeight)
{
	int i;

	m_BtnMainTab[CS_IrisShop].Create(this,_S(2572, "아이리스 상점"), 7,36,113,22);
	m_BtnMainTab[CS_PurchaseInfo].Create(this,_S(2386, "구매 정보"), 121,36,113,22);
	m_BtnMainTab[CS_GotTheGift].Create(this,_S(3088, "받은 선물"), 235,36,113,22);
	m_BtnMainTab[CS_WishPage].Create(this, _S(5218, "소망상자"), 349,36,113,22);

	for (i=0; i<CS_Total; ++i)
	{
		m_BtnMainTab[i].SetUV(UBS_IDLE, 170, 452, 283, 474, fTexWidth, fTexHeight);
		m_BtnMainTab[i].SetUV(UBS_CLICK, 170, 428, 283, 450, fTexWidth, fTexHeight);
		m_BtnMainTab[i].CopyUV(UBS_IDLE, UBS_ON);
		m_BtnMainTab[i].CopyUV(UBS_IDLE, UBS_DISABLE);
	}

	m_BtnMainTab[CS_IrisShop].SetBtnState(UBS_CLICK);
}

// ----------------------------------------------------------------------------
// Name : CategoryFrameInit()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::CategoryFrameInit(FLOAT fTexWidth, FLOAT fTexHeight)
{
	// Category black box
	// T_L, T_M, T_R
	m_Background.AddRectSurface(UIRect(7,58,37,83), UIRectUV(294,153,323,177,fTexWidth,fTexHeight)); // T_L
	m_Background.AddRectSurface(UIRect(37,58,432,83), UIRectUV(324,153,353,177,fTexWidth,fTexHeight)); // T_M
	m_Background.AddRectSurface(UIRect(432,58,462,83), UIRectUV(354,153,384,177,fTexWidth,fTexHeight)); // T_R
	// M_L, M_M, M_R
	m_Background.AddRectSurface(UIRect(7,83,37,110), UIRectUV(294,178,323,203,fTexWidth,fTexHeight)); // M_L
	m_Background.AddRectSurface(UIRect(37,83,432,110), UIRectUV(324,178,353,203,fTexWidth,fTexHeight)); // M_M
	m_Background.AddRectSurface(UIRect(432,83,462,110), UIRectUV(354,178,384,203,fTexWidth,fTexHeight)); // M_R
	// B_L, B_M, B_R
	m_Background.AddRectSurface(UIRect(7,110,37,133), UIRectUV(294,204,323,229,fTexWidth,fTexHeight)); // B_L
	m_Background.AddRectSurface(UIRect(37,110,432,133), UIRectUV(324,204,353,229,fTexWidth,fTexHeight)); // B_M
	m_Background.AddRectSurface(UIRect(432,110,462,133), UIRectUV(354,204,384,229,fTexWidth,fTexHeight)); // B_R

	// Search black box
	m_Background.AddRectSurface(UIRect(7,137,37,162), UIRectUV(294,153,323,177,fTexWidth,fTexHeight)); // T_L
	m_Background.AddRectSurface(UIRect(37,137,432,162), UIRectUV(324,153,353,177,fTexWidth,fTexHeight)); // T_M
	m_Background.AddRectSurface(UIRect(432,137,462,162), UIRectUV(354,153,384,177,fTexWidth,fTexHeight)); // T_R

	m_Background.AddRectSurface(UIRect(7,143,37,168), UIRectUV(294,204,323,229,fTexWidth,fTexHeight)); // B_L
	m_Background.AddRectSurface(UIRect(37,143,432,168), UIRectUV(324,204,353,229,fTexWidth,fTexHeight)); // B_M
	m_Background.AddRectSurface(UIRect(432,143,462,168), UIRectUV(354,204,384,229,fTexWidth,fTexHeight)); // B_R

	m_Background.AddRectSurface(UIRect(15,142,133,162), UIRectUV(39,236,157,256,fTexWidth,fTexHeight));

	m_BtnCategory[CSC_HOTandNew].Create(this, CTString(""), 15,66,60,60);
	m_BtnCategory[CSC_HOTandNew].SetUV(UBS_IDLE, 0,357,60,417,fTexWidth,fTexHeight);
	m_BtnCategory[CSC_HOTandNew].CopyUV(UBS_IDLE, UBS_CLICK);
	m_BtnCategory[CSC_HOTandNew].CopyUV(UBS_IDLE, UBS_DISABLE);
	m_BtnCategory[CSC_HOTandNew].CopyUV(UBS_IDLE, UBS_ON);

	m_BtnCategory[CSC_Platinum].Create(this, CTString(""), 78,66,60,60);
	m_BtnCategory[CSC_Platinum].SetUV(UBS_IDLE, 189,357,249,417,fTexWidth,fTexHeight);
	m_BtnCategory[CSC_Platinum].CopyUV(UBS_IDLE, UBS_CLICK);
	m_BtnCategory[CSC_Platinum].CopyUV(UBS_IDLE, UBS_DISABLE);
	m_BtnCategory[CSC_Platinum].CopyUV(UBS_IDLE, UBS_ON);

	m_BtnCategory[CSC_SpendGoods].Create(this, CTString(""), 141,66,60,60);
	m_BtnCategory[CSC_SpendGoods].SetUV(UBS_IDLE, 0,420,59,479,fTexWidth,fTexHeight);
	m_BtnCategory[CSC_SpendGoods].CopyUV(UBS_IDLE, UBS_CLICK);
	m_BtnCategory[CSC_SpendGoods].CopyUV(UBS_IDLE, UBS_DISABLE);
	m_BtnCategory[CSC_SpendGoods].CopyUV(UBS_IDLE, UBS_ON);

	m_BtnCategory[CSC_EquipGoods].Create(this, CTString(""), 204,66,60,60);
	m_BtnCategory[CSC_EquipGoods].SetUV(UBS_IDLE, 63,420,123,479,fTexWidth,fTexHeight);
	m_BtnCategory[CSC_EquipGoods].CopyUV(UBS_IDLE, UBS_CLICK);
	m_BtnCategory[CSC_EquipGoods].CopyUV(UBS_IDLE, UBS_DISABLE);
	m_BtnCategory[CSC_EquipGoods].CopyUV(UBS_IDLE, UBS_ON);

	m_BtnCategory[CSC_Avata].Create(this, CTString(""), 267,66,60,60);
	m_BtnCategory[CSC_Avata].SetUV(UBS_IDLE, 63,357,122,417,fTexWidth,fTexHeight);
	m_BtnCategory[CSC_Avata].CopyUV(UBS_IDLE, UBS_CLICK);
	m_BtnCategory[CSC_Avata].CopyUV(UBS_IDLE, UBS_DISABLE);
	m_BtnCategory[CSC_Avata].CopyUV(UBS_IDLE, UBS_ON);

	m_BtnCategory[CSC_ServiceGoods].Create(this, CTString(""), 330,66,60,60);
	m_BtnCategory[CSC_ServiceGoods].SetUV(UBS_IDLE, 126,357,186,417,fTexWidth,fTexHeight);
	m_BtnCategory[CSC_ServiceGoods].CopyUV(UBS_IDLE, UBS_CLICK);
	m_BtnCategory[CSC_ServiceGoods].CopyUV(UBS_IDLE, UBS_DISABLE);
	m_BtnCategory[CSC_ServiceGoods].CopyUV(UBS_IDLE, UBS_ON);

	m_BtnCategory[CSC_PackageGoods].Create(this, CTString(""), 393,66,60,60);
	m_BtnCategory[CSC_PackageGoods].SetUV(UBS_IDLE, 252,357,312,417,fTexWidth,fTexHeight);
	m_BtnCategory[CSC_PackageGoods].CopyUV(UBS_IDLE, UBS_CLICK);
	m_BtnCategory[CSC_PackageGoods].CopyUV(UBS_IDLE, UBS_DISABLE);
	m_BtnCategory[CSC_PackageGoods].CopyUV(UBS_IDLE, UBS_ON);

	m_cbCategoryType[CST_First].Create(this, 180,143,118,20,102,4,13,13,10, _pUIFontTexMgr->GetFontHeight()+5, 0, 0);
	m_cbCategoryType[CST_First].SetBackUV(39,236,157,256,fTexWidth,fTexHeight);
	m_cbCategoryType[CST_First].SetDownBtnUV(165,236,178,249,fTexWidth,fTexHeight);
	m_cbCategoryType[CST_First].SetUpBtnUV(165,252,178,265,fTexWidth,fTexHeight);
	m_cbCategoryType[CST_First].SetDropListUV(294,153,384,229,fTexWidth,fTexHeight);
	m_cbCategoryType[CST_Second].Create(this, 300,143,118,20,102,4,13,13,10, _pUIFontTexMgr->GetFontHeight()+5, 0, 0);
	m_cbCategoryType[CST_Second].SetBackUV(39,236,157,256,fTexWidth,fTexHeight);
	m_cbCategoryType[CST_Second].SetDownBtnUV(165,236,178,249,fTexWidth,fTexHeight);
	m_cbCategoryType[CST_Second].SetUpBtnUV(165,252,178,265,fTexWidth,fTexHeight);
	m_cbCategoryType[CST_Second].SetDropListUV(294,153,384,229,fTexWidth,fTexHeight);

	m_BtnSearch.Create(this, CTString(""), 422,143,35,20);
	m_BtnSearch.SetUV(UBS_IDLE, 130,428,165,448, fTexWidth, fTexHeight);
	m_BtnSearch.SetUV(UBS_CLICK, 130,450,165,470,fTexWidth, fTexHeight);
	m_BtnSearch.CopyUV(UBS_IDLE, UBS_DISABLE);
	m_BtnSearch.CopyUV(UBS_IDLE, UBS_ON);

	m_UVSelCategory.SetUV(287,421,341,475,fTexWidth,fTexHeight);
}

// ----------------------------------------------------------------------------
// Name : ViewFrameInit()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::ViewFrameInit(FLOAT fTexWidth, FLOAT fTexHeight)
{
	int i, j;

	// Goods list black box
	m_Background.AddRectSurface(UIRect(7,173,37,198), UIRectUV(294,153,323,178,fTexWidth,fTexHeight)); // T_L
	m_Background.AddRectSurface(UIRect(37,173,432,198), UIRectUV(324,153,353,178,fTexWidth,fTexHeight)); // T_M
	m_Background.AddRectSurface(UIRect(432,173,462,198), UIRectUV(354,153,384,178,fTexWidth,fTexHeight)); // T_R
	// M_L, M_M, M_R
	m_Background.AddRectSurface(UIRect(7,198,37,517), UIRectUV(294,178,323,203,fTexWidth,fTexHeight)); // M_L
	m_Background.AddRectSurface(UIRect(37,198,432,517), UIRectUV(324,178,353,203,fTexWidth,fTexHeight)); // M_M
	m_Background.AddRectSurface(UIRect(432,198,462,517), UIRectUV(354,178,384,203,fTexWidth,fTexHeight)); // M_R
	// B_L, B_M, B_R
	m_Background.AddRectSurface(UIRect(7,517,37,542), UIRectUV(294,204,323,229,fTexWidth,fTexHeight)); // B_L
	m_Background.AddRectSurface(UIRect(37,517,432,542), UIRectUV(324,204,353,229,fTexWidth,fTexHeight)); // B_M
	m_Background.AddRectSurface(UIRect(432,517,462,542), UIRectUV(354,204,384,229,fTexWidth,fTexHeight)); // B_R

	// Goods list blue box
	m_blueBoxGoodsList.AddRectSurface(UIRect(12,179,42,203), UIRectUV(391,153,421,178,fTexWidth,fTexHeight)); // T_L
	m_blueBoxGoodsList.AddRectSurface(UIRect(42,179,428,203), UIRectUV(421,153,451,178,fTexWidth,fTexHeight)); // T_M
	m_blueBoxGoodsList.AddRectSurface(UIRect(428,179,458,203), UIRectUV(451,153,481,178,fTexWidth,fTexHeight)); // T_R

	m_blueBoxGoodsList.AddRectSurface(UIRect(12,203,42,511), UIRectUV(391,178,421,204,fTexWidth,fTexHeight)); // M_L
	m_blueBoxGoodsList.AddRectSurface(UIRect(42,203,428,511), UIRectUV(421,178,451,204,fTexWidth,fTexHeight)); // M_M
	m_blueBoxGoodsList.AddRectSurface(UIRect(428,203,458,511), UIRectUV(451,178,481,204,fTexWidth,fTexHeight)); // M_R

	m_blueBoxGoodsList.AddRectSurface(UIRect(12,511,42,536), UIRectUV(391,204,421,229,fTexWidth,fTexHeight)); // B_L
	m_blueBoxGoodsList.AddRectSurface(UIRect(42,511,428,536), UIRectUV(421,204,451,229,fTexWidth,fTexHeight)); // B_M
	m_blueBoxGoodsList.AddRectSurface(UIRect(428,511,458,536), UIRectUV(451,204,481,229,fTexWidth,fTexHeight)); // B_R

	// purchase blue box
	// left
	m_blueBoxPurchase.AddRectSurface(UIRect(17,187,47,212), UIRectUV(391,153,421,178,fTexWidth,fTexHeight)); // T_L
	m_blueBoxPurchase.AddRectSurface(UIRect(47,187,200,212), UIRectUV(421,153,451,178,fTexWidth,fTexHeight)); // T_M
	m_blueBoxPurchase.AddRectSurface(UIRect(200,187,230,212), UIRectUV(451,153,481,178,fTexWidth,fTexHeight)); // T_R

	m_blueBoxPurchase.AddRectSurface(UIRect(17,212,47,503), UIRectUV(391,178,421,204,fTexWidth,fTexHeight)); // M_L
	m_blueBoxPurchase.AddRectSurface(UIRect(47,212,200,503), UIRectUV(421,178,451,204,fTexWidth,fTexHeight)); // M_M
	m_blueBoxPurchase.AddRectSurface(UIRect(200,212,230,503), UIRectUV(451,178,481,204,fTexWidth,fTexHeight)); // M_R

	m_blueBoxPurchase.AddRectSurface(UIRect(17,503,47,528), UIRectUV(391,204,421,229,fTexWidth,fTexHeight)); // B_L
	m_blueBoxPurchase.AddRectSurface(UIRect(47,503,200,528), UIRectUV(421,204,451,229,fTexWidth,fTexHeight)); // B_M
	m_blueBoxPurchase.AddRectSurface(UIRect(200,503,230,528), UIRectUV(451,204,481,229,fTexWidth,fTexHeight)); // B_R
	// right
	m_blueBoxPurchase.AddRectSurface(UIRect(238,187,268,212), UIRectUV(391,153,421,178,fTexWidth,fTexHeight)); // T_L
	m_blueBoxPurchase.AddRectSurface(UIRect(268,187,421,212), UIRectUV(421,153,451,178,fTexWidth,fTexHeight)); // T_M
	m_blueBoxPurchase.AddRectSurface(UIRect(421,187,451,212), UIRectUV(451,153,481,178,fTexWidth,fTexHeight)); // T_R

	m_blueBoxPurchase.AddRectSurface(UIRect(238,212,268,503), UIRectUV(391,178,421,204,fTexWidth,fTexHeight)); // M_L
	m_blueBoxPurchase.AddRectSurface(UIRect(268,212,421,503), UIRectUV(421,178,451,204,fTexWidth,fTexHeight)); // M_M
	m_blueBoxPurchase.AddRectSurface(UIRect(421,212,451,503), UIRectUV(451,178,481,204,fTexWidth,fTexHeight)); // M_R

	m_blueBoxPurchase.AddRectSurface(UIRect(238,503,268,528), UIRectUV(391,204,421,229,fTexWidth,fTexHeight)); // B_L
	m_blueBoxPurchase.AddRectSurface(UIRect(268,503,421,528), UIRectUV(421,204,451,229,fTexWidth,fTexHeight)); // B_M
	m_blueBoxPurchase.AddRectSurface(UIRect(421,503,451,528), UIRectUV(451,204,481,229,fTexWidth,fTexHeight)); // B_R

	// purchase item box
	m_blueBoxPurchase.AddRectSurface(UIRect(30,261,64,295), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_blueBoxPurchase.AddRectSurface(UIRect(30,261,64,295), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));

	UIRect rtBox;
	rtBox.SetRect(30,261,64,295);

	for (i=0; i<5; ++i)
	{
		UIRect rtTempBox = rtBox;

		if (i>0)
		{
			rtTempBox.Top += (38*i);
			rtTempBox.Bottom += (38*i);
		}

		for (j=0; j<5; ++j)
		{
			if (j>0)
			{
				rtTempBox.Left += 38;
				rtTempBox.Right += 38;
			}

			m_blueBoxPurchase.AddRectSurface(rtTempBox, UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
			m_PurchaseItemList[(i*5)+j].pIconGoods->Create(this, rtTempBox.Left+1, rtTempBox.Top+1, 32, 32, UI_CASHSHOP_EX, UBET_ITEM);
		}
	}

	rtBox.SetRect(252,261,286,295);

	for (i=0; i<2; ++i)
	{
		UIRect rtTempBox = rtBox;

		if (i>0)
		{
			rtTempBox.Top += (38*i);
			rtTempBox.Bottom += (38*i);
		}

		for (j=0; j<5; ++j)
		{
			if (j>0)
			{
				rtTempBox.Left += 38;
				rtTempBox.Right += 38;
			}

			m_blueBoxPurchase.AddRectSurface(rtTempBox, UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
			m_PurchaseGetItemList[(i*5)+j] = new CUIIcon;
			m_PurchaseGetItemList[(i*5)+j]->Create(this, rtTempBox.Left+1, rtTempBox.Top+1, 32, 32, UI_CASHSHOP_EX, UBET_ITEM);
		}
	}

	// black edit box
	m_BlackBoxPage.AddRectSurface(UIRect(200,509,234,529), UIRectUV(39,236,73,256,fTexWidth,fTexHeight));
	m_BlackBoxPage.AddRectSurface(UIRect(234,509,268,529), UIRectUV(123,236,157,256,fTexWidth,fTexHeight));

	//m_BlackBoxRevGiftList.AddRectSurface(UIRect(17,253,229,523), UIRectUV(,,,fTexWidth,fTexHeight)); // 30 25
	m_BlackBoxRevGiftList.AddRectSurface(UIRect(17,253,47,278), UIRectUV(294,153,323,178,fTexWidth,fTexHeight)); // T_L
	m_BlackBoxRevGiftList.AddRectSurface(UIRect(47,253,199,278), UIRectUV(324,153,353,178,fTexWidth,fTexHeight)); // T_M
	m_BlackBoxRevGiftList.AddRectSurface(UIRect(199,253,229,278), UIRectUV(354,153,384,178,fTexWidth,fTexHeight)); // T_R
	// M_L, M_M, M_R
	m_BlackBoxRevGiftList.AddRectSurface(UIRect(17,278,47,498), UIRectUV(294,178,323,203,fTexWidth,fTexHeight)); // M_L
	m_BlackBoxRevGiftList.AddRectSurface(UIRect(47,278,199,498), UIRectUV(324,178,353,203,fTexWidth,fTexHeight)); // M_M
	m_BlackBoxRevGiftList.AddRectSurface(UIRect(199,278,229,498), UIRectUV(354,178,384,203,fTexWidth,fTexHeight)); // M_R
	// B_L, B_M, B_R
	m_BlackBoxRevGiftList.AddRectSurface(UIRect(17,498,47,523), UIRectUV(294,204,323,229,fTexWidth,fTexHeight)); // B_L
	m_BlackBoxRevGiftList.AddRectSurface(UIRect(47,498,199,523), UIRectUV(324,204,353,229,fTexWidth,fTexHeight)); // B_M
	m_BlackBoxRevGiftList.AddRectSurface(UIRect(199,498,229,523), UIRectUV(354,204,384,229,fTexWidth,fTexHeight)); // B_R

	m_BlackBoxRevGiftList.AddRectSurface(UIRect(20,255,54,289), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	
	for (i = 0; i < 5; ++i)
		m_pIconRevGift[i] = new CUIIcon;

	m_pIconRevGift[0]->Create(this, 21, 256, 32, 32, UI_CASHSHOP_EX, UBET_ITEM);
	m_BlackBoxRevGiftList.AddRectSurface(UIRect(20,313,54,347), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_pIconRevGift[1]->Create(this, 21, 314, 32, 32, UI_CASHSHOP_EX, UBET_ITEM);
	m_BlackBoxRevGiftList.AddRectSurface(UIRect(20,371,54,405), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_pIconRevGift[2]->Create(this, 21, 372, 32, 32, UI_CASHSHOP_EX, UBET_ITEM);
	m_BlackBoxRevGiftList.AddRectSurface(UIRect(20,429,54,463), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_pIconRevGift[3]->Create(this, 21, 430, 32, 32, UI_CASHSHOP_EX, UBET_ITEM);
	m_BlackBoxRevGiftList.AddRectSurface(UIRect(20,487,54,521), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_pIconRevGift[4]->Create(this, 21, 488, 32, 32, UI_CASHSHOP_EX, UBET_ITEM);

	m_rtGiftDataList.SetRect(20,255,217,521);

	m_BlackBoxRevGiftList.AddRectSurface(UIRect(238,253,268,278), UIRectUV(294,153,323,178,fTexWidth,fTexHeight)); // T_L
	m_BlackBoxRevGiftList.AddRectSurface(UIRect(268,253,420,278), UIRectUV(324,153,353,178,fTexWidth,fTexHeight)); // T_M
	m_BlackBoxRevGiftList.AddRectSurface(UIRect(420,253,450,278), UIRectUV(354,153,384,178,fTexWidth,fTexHeight)); // T_R
	// M_L, M_M, M_R
	m_BlackBoxRevGiftList.AddRectSurface(UIRect(238,278,268,322), UIRectUV(294,178,323,222,fTexWidth,fTexHeight)); // M_L
	m_BlackBoxRevGiftList.AddRectSurface(UIRect(268,278,420,322), UIRectUV(324,178,353,222,fTexWidth,fTexHeight)); // M_M
	m_BlackBoxRevGiftList.AddRectSurface(UIRect(420,278,450,322), UIRectUV(354,178,384,222,fTexWidth,fTexHeight)); // M_R
	// B_L, B_M, B_R
	m_BlackBoxRevGiftList.AddRectSurface(UIRect(238,322,268,347), UIRectUV(294,204,323,229,fTexWidth,fTexHeight)); // B_L
	m_BlackBoxRevGiftList.AddRectSurface(UIRect(268,322,420,347), UIRectUV(324,204,353,229,fTexWidth,fTexHeight)); // B_M
	m_BlackBoxRevGiftList.AddRectSurface(UIRect(420,322,450,347), UIRectUV(354,204,384,229,fTexWidth,fTexHeight)); // B_R

	m_edGiftContent.Create(this, 238,253,212,94,4);
	m_edGiftContent.SetEnableInput(TRUE);

	rtBox.SetRect(17,186,229,260);

	// 소망 상자 리스트도 디자인이 같으므로 여기서 셋팅한다.
	// 이것은 가로 배열
	for (i=0; i<4; ++i)
	{
		UIRect rtTempBox = rtBox;

		if (i>0)
		{
			rtTempBox.Top += (81*i);
			rtTempBox.Bottom += (81*i);
		}

		for (j=0; j<2; ++j)
		{
			if (j>0)
			{
				rtTempBox.Left += 222;
				rtTempBox.Right += 222;
			}

			m_GoodsBoxList[(i*2)+j].BoxSurface.AddRectSurface(rtTempBox, UIRectUV(0,153,213,228,fTexWidth,fTexHeight));
			/*m_GoodsBoxList[(i*2)+j].BoxSurface.AddRectSurface(UIRect(rtTempBox.Left+8,rtTempBox.Top+34,rtTempBox.Left+42,rtTempBox.Top+68),
				UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
			m_GoodsBoxList[(i*2)+j].BoxSurface.AddRectSurface(UIRect(rtTempBox.Right-59,rtTempBox.Bottom-32, rtTempBox.Right-36, rtTempBox.Bottom-13),
				UIRectUV(39,236,62,256,fTexWidth,fTexHeight));
			m_GoodsBoxList[(i*2)+j].BoxSurface.AddRectSurface(UIRect(rtTempBox.Right-36,rtTempBox.Bottom-32,rtTempBox.Right-13,rtTempBox.Bottom-13),
				UIRectUV(133,236,157,256,fTexWidth,fTexHeight));*/

			m_GoodsBoxList[(i*2)+j].pIconGoods->Create(this, rtTempBox.Left + 9, rtTempBox.Top + 35, 32, 32, UI_CASHSHOP_EX, UBET_ITEM);
			m_GoodsBoxList[(i*2)+j].btnPkgInfo.Create(this, CTString(""), rtTempBox.Left+45, rtTempBox.Top+35, 22, 34);
			m_GoodsBoxList[(i*2)+j].btnPkgInfo.SetUV(UBS_IDLE, 262,478,284,512, fTexWidth, fTexHeight);
			m_GoodsBoxList[(i*2)+j].btnPkgInfo.SetUV(UBS_CLICK, 288,478,310,512, fTexWidth, fTexHeight);
			m_GoodsBoxList[(i*2)+j].btnPkgInfo.CopyUV(UBS_IDLE, UBS_DISABLE);
			m_GoodsBoxList[(i*2)+j].btnPkgInfo.CopyUV(UBS_IDLE, UBS_ON);

			// 소망상자에 Copy
			m_WishBoxList[(i*2)+j].BoxSurface = m_GoodsBoxList[(i*2)+j].BoxSurface;
			// 소망상자 buttonex
			m_WishBoxList[(i*2)+j].pIconGoods->Create(this, rtTempBox.Left + 9, rtTempBox.Top + 35, 32, 32, UI_CASHSHOP_EX, UBET_ITEM);
			// 소망상자 패키지 내용보기 버튼
			m_WishBoxList[(i*2)+j].btnPkgInfo.Create(this, CTString(""), rtTempBox.Left+45, rtTempBox.Top+35, 22, 34);
			m_WishBoxList[(i*2)+j].btnPkgInfo.SetUV(UBS_IDLE, 262,478,284,512, fTexWidth, fTexHeight);
			m_WishBoxList[(i*2)+j].btnPkgInfo.SetUV(UBS_CLICK, 288,478,310,512, fTexWidth, fTexHeight);
			m_WishBoxList[(i*2)+j].btnPkgInfo.CopyUV(UBS_IDLE, UBS_DISABLE);
			m_WishBoxList[(i*2)+j].btnPkgInfo.CopyUV(UBS_IDLE, UBS_ON);
		}
	}

	m_BtnPage[VPT_Begin].Create(this, CTString(""), 167,509,14,20);
	m_BtnPage[VPT_Begin].SetUV(UBS_IDLE, 184,236,198,256,fTexWidth,fTexHeight);
	m_BtnPage[VPT_Begin].SetUV(UBS_CLICK, 184,259,198,279,fTexWidth,fTexHeight);
	m_BtnPage[VPT_Begin].CopyUV(UBS_IDLE, UBS_DISABLE);
	m_BtnPage[VPT_Begin].CopyUV(UBS_IDLE, UBS_ON);

	m_BtnPage[VPT_Back].Create(this, CTString(""), 182,509,14,20);
	m_BtnPage[VPT_Back].SetUV(UBS_IDLE, 199,236,213,256,fTexWidth,fTexHeight);
	m_BtnPage[VPT_Back].SetUV(UBS_CLICK, 199,259,213,279,fTexWidth,fTexHeight);
	m_BtnPage[VPT_Back].CopyUV(UBS_IDLE, UBS_DISABLE);
	m_BtnPage[VPT_Back].CopyUV(UBS_IDLE, UBS_ON);

	m_BtnPage[VPT_Front].Create(this, CTString(""), 272,509,14,20);
	m_BtnPage[VPT_Front].SetUV(UBS_IDLE, 214,236,228,256,fTexWidth,fTexHeight);
	m_BtnPage[VPT_Front].SetUV(UBS_CLICK, 214,259,228,279,fTexWidth,fTexHeight);
	m_BtnPage[VPT_Front].CopyUV(UBS_IDLE, UBS_DISABLE);
	m_BtnPage[VPT_Front].CopyUV(UBS_IDLE, UBS_ON);

	m_BtnPage[VPT_End].Create(this, CTString(""), 287,509,14,20);
	m_BtnPage[VPT_End].SetUV(UBS_IDLE, 229,236,243,256,fTexWidth,fTexHeight);
	m_BtnPage[VPT_End].SetUV(UBS_CLICK, 229,259,243,279,fTexWidth,fTexHeight);
	m_BtnPage[VPT_End].CopyUV(UBS_IDLE, UBS_DISABLE);
	m_BtnPage[VPT_End].CopyUV(UBS_IDLE, UBS_ON);

	m_PurchaseGetItemRect.SetRect(252,261,438,333);
	m_PurchaseItemRect.SetRect(30,261,216,447);

	m_BtnMove.Create(this, _S(5219, "모두 옮기기"), 71,463,96,21);
	m_BtnMove.SetNewType(TRUE);
	m_BtnMove.SetRTSurface(UBS_IDLE, UIRect(0,0,InvenbtnSlicegap,21), UIRectUV(249,235,264,256,fTexWidth,fTexHeight)); // L
	m_BtnMove.SetRTSurface(UBS_IDLE, UIRect(InvenbtnSlicegap,0,96-InvenbtnSlicegap,21), UIRectUV(264,235,299,256,fTexWidth,fTexHeight)); // M
	m_BtnMove.SetRTSurface(UBS_IDLE, UIRect(96-InvenbtnSlicegap,0,96,21), UIRectUV(299,235,314,256,fTexWidth,fTexHeight)); // R
	m_BtnMove.SetRTSurface(UBS_CLICK, UIRect(0,0,InvenbtnSlicegap,21), UIRectUV(249,258,264,279,fTexWidth,fTexHeight)); // L
	m_BtnMove.SetRTSurface(UBS_CLICK, UIRect(InvenbtnSlicegap,0,96-InvenbtnSlicegap,21), UIRectUV(264,258,299,279,fTexWidth,fTexHeight)); // M
	m_BtnMove.SetRTSurface(UBS_CLICK, UIRect(96-InvenbtnSlicegap,0,96,21), UIRectUV(299,258,314,279,fTexWidth,fTexHeight)); // R
	m_BtnMove.CopyRTSurface(UBS_IDLE, UBS_DISABLE);
	m_BtnMove.CopyRTSurface(UBS_IDLE, UBS_ON);

	m_BtnGet.Create(this, _S(2383, "가져오기"), 299,363,96,21);
	m_BtnGet.SetNewType(TRUE);
	m_BtnGet.CopyRTSurface(UBS_IDLE, m_BtnMove.GetRectSurface(UBS_IDLE));
	m_BtnGet.CopyRTSurface(UBS_CLICK, m_BtnMove.GetRectSurface(UBS_CLICK));
	m_BtnGet.CopyRTSurface(UBS_IDLE, UBS_DISABLE);
	m_BtnGet.CopyRTSurface(UBS_IDLE, UBS_ON);

	m_BtnGift.Create(this, _S(3089, "선물하기"), 299,394,96,21);
	m_BtnGift.SetNewType(TRUE);
	m_BtnGift.CopyRTSurface(UBS_IDLE, m_BtnMove.GetRectSurface(UBS_IDLE));
	m_BtnGift.CopyRTSurface(UBS_CLICK, m_BtnMove.GetRectSurface(UBS_CLICK));
	m_BtnGift.CopyRTSurface(UBS_IDLE, UBS_DISABLE);
	m_BtnGift.CopyRTSurface(UBS_IDLE, UBS_ON);

	m_uvFlag[CFI_New].SetUV(183, 501, 205, 512, fTexWidth, fTexHeight);
	m_uvFlag[CFI_Popular].SetUV(208, 501, 230, 512, fTexWidth, fTexHeight);
	m_uvFlag[CFI_Discount].SetUV(233, 501, 255, 512, fTexWidth, fTexHeight);

	m_GiftScroll.Create(this, 217,255,9,266);
	m_GiftScroll.CreateButtons(TRUE, 9,7,0,0,10);
	m_GiftScroll.SetScrollPos(0);
	m_GiftScroll.SetCurItemCount(0);
	m_GiftScroll.SetItemsPerPage(5);
	// Up button
	m_GiftScroll.SetUpUV(UBS_IDLE, 12,488,21,495,fTexWidth,fTexHeight);
	m_GiftScroll.SetUpUV(UBS_CLICK, 22,488,31,495,fTexWidth,fTexHeight);
	m_GiftScroll.CopyUpUV(UBS_IDLE, UBS_ON);
	m_GiftScroll.CopyUpUV(UBS_CLICK, UBS_DISABLE);
	// Down button
	m_GiftScroll.SetDownUV(UBS_IDLE, 12,496,21,503,fTexWidth,fTexHeight);
	m_GiftScroll.SetDownUV(UBS_CLICK, 22,496,31,503,fTexWidth,fTexHeight);
	m_GiftScroll.CopyDownUV(UBS_IDLE, UBS_ON);
	m_GiftScroll.CopyDownUV(UBS_CLICK, UBS_DISABLE);
	// Bar button
	m_GiftScroll.SetBarTopUV( 0, 488, 9, 494, fTexWidth, fTexHeight );
	m_GiftScroll.SetBarMiddleUV( 0, 494, 9, 506, fTexWidth, fTexHeight );
	m_GiftScroll.SetBarBottomUV( 0, 506, 9, 512, fTexWidth, fTexHeight );
	// wheel
	m_GiftScroll.SetWheelRect(-198, 0, 197, 266);
}

// ----------------------------------------------------------------------------
// Name : RecommandFrameInit()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::RecommandFrameInit(FLOAT fTexWidth, FLOAT fTexHeight)
{
	int i;
	// Recommand black box
	m_Background.AddRectSurface(UIRect(466,36,496,61), UIRectUV(294,153,323,177,fTexWidth,fTexHeight)); // T_L
	m_Background.AddRectSurface(UIRect(496,36,515,61), UIRectUV(324,153,353,177,fTexWidth,fTexHeight)); // T_M
	m_Background.AddRectSurface(UIRect(515,36,545,61), UIRectUV(354,153,384,177,fTexWidth,fTexHeight)); // T_R

	m_Background.AddRectSurface(UIRect(466,61,496,517), UIRectUV(294,178,323,203,fTexWidth,fTexHeight)); // M_L
	m_Background.AddRectSurface(UIRect(496,61,515,517), UIRectUV(324,178,353,203,fTexWidth,fTexHeight)); // M_M
	m_Background.AddRectSurface(UIRect(515,61,545,517), UIRectUV(354,178,384,203,fTexWidth,fTexHeight)); // M_R

	m_Background.AddRectSurface(UIRect(466,517,496,542), UIRectUV(294,204,323,229,fTexWidth,fTexHeight)); // B_L
	m_Background.AddRectSurface(UIRect(496,517,515,542), UIRectUV(324,204,353,229,fTexWidth,fTexHeight)); // B_M
	m_Background.AddRectSurface(UIRect(515,517,545,542), UIRectUV(354,204,384,229,fTexWidth,fTexHeight)); // B_R

	// Best Item layer
	m_Background.AddRectSurface(UIRect(467,37,544,114), UIRectUV(1,274,77,350,fTexWidth,fTexHeight));

	UIRect recBoxRect;
	recBoxRect.SetRect(470,120,541,189);

	for (i=0; i<_RankCount; ++i)
	{
		if (i > 0)
		{
			recBoxRect.Top += (17+69);
			recBoxRect.Bottom += (17+69);
		}

		m_RecommandBoxList[i].BoxSurface.AddRectSurface(recBoxRect,
			UIRectUV(218,153,289,222,fTexWidth, fTexHeight));
		m_RecommandBoxList[i].BoxSurface.AddRectSurface(UIRect(recBoxRect.Left+32,recBoxRect.Top+7,recBoxRect.Right-7,recBoxRect.Bottom-30),
			UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
		m_RecommandBoxList[i].BoxSurface.AddRectSurface(UIRect(recBoxRect.Left+7,recBoxRect.Top+45,recBoxRect.Left+37,recBoxRect.Bottom-6),
			UIRectUV(39,236,69,256,fTexWidth, fTexHeight));
		m_RecommandBoxList[i].BoxSurface.AddRectSurface(UIRect(recBoxRect.Right-36,recBoxRect.Top+45,recBoxRect.Right-7,recBoxRect.Bottom-6),
			UIRectUV(128,236,157,256,fTexWidth, fTexHeight));

		m_RecommandBoxList[i].pIconGoods->Create(this, recBoxRect.Left + 32, recBoxRect.Top + 7, 32, 32, UI_CASHSHOP_EX, UBET_ITEM);
	}

	m_RecommandBoxList[0].BoxSurface.AddRectSurface(UIRect(480,130,490,156), UIRectUV(93,294,104,321,fTexWidth, fTexHeight)); //1
	m_RecommandBoxList[1].BoxSurface.AddRectSurface(UIRect(478,215,495,242), UIRectUV(118,294,135,321,fTexWidth, fTexHeight)); //2
	m_RecommandBoxList[2].BoxSurface.AddRectSurface(UIRect(474,300,497,327), UIRectUV(143,294,166,321,fTexWidth, fTexHeight)); //3
	m_RecommandBoxList[3].BoxSurface.AddRectSurface(UIRect(475,385,497,412), UIRectUV(172,294,194,321,fTexWidth, fTexHeight)); //4
	m_RecommandBoxList[4].BoxSurface.AddRectSurface(UIRect(478,471,495,498), UIRectUV(202,294,219,321,fTexWidth, fTexHeight)); //5
}

// ----------------------------------------------------------------------------
// Name : PreviewFrameInit()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::PreviewFrameInit(FLOAT fTexWidth, FLOAT fTexHeight)
{
	// Preview black box
	m_Background.AddRectSurface(UIRect(549,36,579,61), UIRectUV(294,153,324,178,fTexWidth,fTexHeight)); // T_L
	m_Background.AddRectSurface(UIRect(579,36,713,61), UIRectUV(324,153,354,178,fTexWidth,fTexHeight)); // T_M
	m_Background.AddRectSurface(UIRect(713,36,743,61), UIRectUV(354,153,384,178,fTexWidth,fTexHeight)); // T_R

	m_Background.AddRectSurface(UIRect(549,61,579,275), UIRectUV(294,178,324,204,fTexWidth,fTexHeight)); // M_L
	m_Background.AddRectSurface(UIRect(579,61,713,275), UIRectUV(324,178,354,204,fTexWidth,fTexHeight)); // M_M
	m_Background.AddRectSurface(UIRect(713,61,743,275), UIRectUV(354,178,384,204,fTexWidth,fTexHeight)); // M_R

	m_Background.AddRectSurface(UIRect(549,275,579,300), UIRectUV(294,204,324,229,fTexWidth,fTexHeight)); // B_L
	m_Background.AddRectSurface(UIRect(579,275,713,300), UIRectUV(324,204,354,229,fTexWidth,fTexHeight)); // B_M
	m_Background.AddRectSurface(UIRect(713,275,743,300), UIRectUV(354,204,384,229,fTexWidth,fTexHeight)); // B_R

	// Preview bg box
	m_Background.AddRectSurface(UIRect(595,40,739,295), UIRectUV(368,257,512,512,fTexWidth,fTexHeight));
	m_rtPreview = UIRect(597,42,597+_PreviewX,42+_PreviewY);

	// Preview Equip box
	for (int i = 0; i < MAX_CASHEQUIP; ++i)
		m_pIconEquip[i] = new CUIIcon;

	m_Background.AddRectSurface(UIRect(555,41,589,75), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_pIconEquip[0]->Create(this, 556, 42, 32, 32, UI_CASHSHOP_EX, UBET_ITEM);
	m_Background.AddRectSurface(UIRect(555,85,589,119), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_pIconEquip[1]->Create(this, 556, 86, 32, 32, UI_CASHSHOP_EX, UBET_ITEM);
	m_Background.AddRectSurface(UIRect(555,129,589,162), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_pIconEquip[2]->Create(this, 556, 130, 32, 32, UI_CASHSHOP_EX, UBET_ITEM);
	m_Background.AddRectSurface(UIRect(555,173,589,206), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_pIconEquip[3]->Create(this, 556, 174, 32, 32, UI_CASHSHOP_EX, UBET_ITEM);
	m_Background.AddRectSurface(UIRect(555,217,589,250), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_pIconEquip[4]->Create(this, 556, 218, 32, 32, UI_CASHSHOP_EX, UBET_ITEM);
	m_Background.AddRectSurface(UIRect(555,261,589,295), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_pIconEquip[5]->Create(this, 556, 262, 32, 32, UI_CASHSHOP_EX, UBET_ITEM);

	m_btnRestore.Create(this, _S(5220, "원래대로 돌리기"), 603,268,129,21);
	m_btnRestore.SetNewType(TRUE);
	m_btnRestore.SetRTSurface(UBS_IDLE, UIRect(0,0,InvenbtnSlicegap,21), UIRectUV(249,235,264,256,fTexWidth,fTexHeight)); // L
	m_btnRestore.SetRTSurface(UBS_IDLE, UIRect(InvenbtnSlicegap,0,129-InvenbtnSlicegap,21), UIRectUV(264,235,299,256,fTexWidth,fTexHeight)); // M
	m_btnRestore.SetRTSurface(UBS_IDLE, UIRect(129-InvenbtnSlicegap,0,129,21), UIRectUV(299,235,314,256,fTexWidth,fTexHeight)); // R
	m_btnRestore.SetRTSurface(UBS_CLICK, UIRect(0,0,InvenbtnSlicegap,21), UIRectUV(249,258,264,279,fTexWidth,fTexHeight)); // L
	m_btnRestore.SetRTSurface(UBS_CLICK, UIRect(InvenbtnSlicegap,0,129-InvenbtnSlicegap,21), UIRectUV(264,258,299,279,fTexWidth,fTexHeight)); // M
	m_btnRestore.SetRTSurface(UBS_CLICK, UIRect(129-InvenbtnSlicegap,0,129,21), UIRectUV(299,258,314,279,fTexWidth,fTexHeight)); // R
	m_btnRestore.CopyRTSurface(UBS_IDLE, UBS_DISABLE);
	m_btnRestore.CopyRTSurface(UBS_IDLE, UBS_ON);
}

// ----------------------------------------------------------------------------
// Name : InvenFrameInit()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::InvenFrameInit(FLOAT fTexWidth, FLOAT fTexHeight)
{
	int i;

	// Cash Inven black box
	m_Background.AddRectSurface(UIRect(549,303,579,328), UIRectUV(294,153,323,177,fTexWidth,fTexHeight)); // T_L
	m_Background.AddRectSurface(UIRect(579,303,713,328), UIRectUV(324,153,353,177,fTexWidth,fTexHeight)); // T_M
	m_Background.AddRectSurface(UIRect(713,303,743,328), UIRectUV(354,153,384,177,fTexWidth,fTexHeight)); // T_R

	m_Background.AddRectSurface(UIRect(549,328,579,517), UIRectUV(294,178,323,203,fTexWidth,fTexHeight)); // M_L
	m_Background.AddRectSurface(UIRect(579,328,713,517), UIRectUV(324,178,353,203,fTexWidth,fTexHeight)); // M_M
	m_Background.AddRectSurface(UIRect(713,328,743,517), UIRectUV(354,178,384,203,fTexWidth,fTexHeight)); // M_R

	m_Background.AddRectSurface(UIRect(549,517,579,542), UIRectUV(294,204,323,229,fTexWidth,fTexHeight)); // B_L
	m_Background.AddRectSurface(UIRect(579,517,713,542), UIRectUV(324,204,353,229,fTexWidth,fTexHeight)); // B_M
	m_Background.AddRectSurface(UIRect(713,517,743,542), UIRectUV(354,204,384,229,fTexWidth,fTexHeight)); // B_R

	// Cash Inven blue box
	m_Background.AddRectSurface(UIRect(553,308,583,333), UIRectUV(391,153,421,178,fTexWidth,fTexHeight)); // T_L
	m_Background.AddRectSurface(UIRect(583,308,709,333), UIRectUV(421,153,451,178,fTexWidth,fTexHeight)); // T_M
	m_Background.AddRectSurface(UIRect(709,308,738,333), UIRectUV(451,153,481,178,fTexWidth,fTexHeight)); // T_R

	m_Background.AddRectSurface(UIRect(553,333,583,511), UIRectUV(391,178,421,204,fTexWidth,fTexHeight)); // M_L
	m_Background.AddRectSurface(UIRect(583,333,709,511), UIRectUV(421,178,451,204,fTexWidth,fTexHeight)); // M_M
	m_Background.AddRectSurface(UIRect(709,333,738,511), UIRectUV(451,178,481,204,fTexWidth,fTexHeight)); // M_R

	m_Background.AddRectSurface(UIRect(553,511,583,536), UIRectUV(391,204,421,229,fTexWidth,fTexHeight)); // B_L
	m_Background.AddRectSurface(UIRect(583,511,709,536), UIRectUV(421,204,451,229,fTexWidth,fTexHeight)); // B_M
	m_Background.AddRectSurface(UIRect(709,511,738,536), UIRectUV(451,204,481,229,fTexWidth,fTexHeight)); // B_R

	// Cash Inven goods box
	for (i = 0; i < 10; ++i)
		m_pIconInven[i] = new CUIIcon;

	m_Background.AddRectSurface(UIRect(557,312,591,346), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_pIconInven[0]->Create(this, 558, 313, 32, 32, UI_CASHSHOP_EX, UBET_ITEM);
	m_Background.AddRectSurface(UIRect(593,312,627,346), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_pIconInven[1]->Create(this, 594, 313, 32, 32, UI_CASHSHOP_EX, UBET_ITEM);
	m_Background.AddRectSurface(UIRect(629,312,663,346), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_pIconInven[2]->Create(this, 630, 313, 32, 32, UI_CASHSHOP_EX, UBET_ITEM);
	m_Background.AddRectSurface(UIRect(665,312,699,346), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_pIconInven[3]->Create(this, 666, 313, 32, 32, UI_CASHSHOP_EX, UBET_ITEM);
	m_Background.AddRectSurface(UIRect(701,312,735,346), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_pIconInven[4]->Create(this, 702, 313, 32, 32, UI_CASHSHOP_EX, UBET_ITEM);

	m_Background.AddRectSurface(UIRect(557,348,591,382), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_pIconInven[5]->Create(this, 558, 349, 32, 32, UI_CASHSHOP_EX, UBET_ITEM);
	m_Background.AddRectSurface(UIRect(593,348,627,382), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_pIconInven[6]->Create(this, 594, 349, 32, 32, UI_CASHSHOP_EX, UBET_ITEM);
	m_Background.AddRectSurface(UIRect(629,348,663,382), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_pIconInven[7]->Create(this, 630, 349, 32, 32, UI_CASHSHOP_EX, UBET_ITEM);
	m_Background.AddRectSurface(UIRect(665,348,699,382), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_pIconInven[8]->Create(this, 666, 349, 32, 32, UI_CASHSHOP_EX, UBET_ITEM);
	m_Background.AddRectSurface(UIRect(701,348,735,382), UIRectUV(0,236,34,270,fTexWidth,fTexHeight));
	m_pIconInven[9]->Create(this, 702, 349, 32, 32, UI_CASHSHOP_EX, UBET_ITEM);

	// Cash Edit Box Bg
	m_Background.AddRectSurface(UIRect(634,388,735,408), UIRectUV(39,236,157,256,fTexWidth,fTexHeight));
	m_Background.AddRectSurface(UIRect(634,414,735,434), UIRectUV(39,236,157,256,fTexWidth,fTexHeight));

	

	m_btnInven[BIT_Clear].Create(this, _S(5221, "장바구니 비우기"), 564,439,164,21);
	m_btnInven[BIT_PurchaseItem].Create(this, _S(2385, "아이템 구매"), 564,464,164,21);
	m_btnInven[BIT_CashCharge].Create(this, _S(2387, "캐쉬 충전"), 564,488,164,21);
	m_btnInven[BIT_Exit].Create(this, _S(4117, "종료"), 564,511,164,21);

	for (i=0; i<BIT_Total; ++i)
	{
		m_btnInven[i].SetNewType(TRUE);
		m_btnInven[i].SetRTSurface(UBS_IDLE, UIRect(0,0,InvenbtnSlicegap,21), UIRectUV(249,235,264,256,fTexWidth,fTexHeight)); // L
		m_btnInven[i].SetRTSurface(UBS_IDLE, UIRect(InvenbtnSlicegap,0,164-InvenbtnSlicegap,21), UIRectUV(264,235,299,256,fTexWidth,fTexHeight)); // M
		m_btnInven[i].SetRTSurface(UBS_IDLE, UIRect(164-InvenbtnSlicegap,0,164,21), UIRectUV(299,235,314,256,fTexWidth,fTexHeight)); // R
		m_btnInven[i].SetRTSurface(UBS_CLICK, UIRect(0,0,InvenbtnSlicegap,21), UIRectUV(249,258,264,279,fTexWidth,fTexHeight)); // L
		m_btnInven[i].SetRTSurface(UBS_CLICK, UIRect(InvenbtnSlicegap,0,164-InvenbtnSlicegap,21), UIRectUV(264,258,299,279,fTexWidth,fTexHeight)); // M
		m_btnInven[i].SetRTSurface(UBS_CLICK, UIRect(164-InvenbtnSlicegap,0,164,21), UIRectUV(299,258,314,279,fTexWidth,fTexHeight)); // R
		m_btnInven[i].CopyRTSurface(UBS_IDLE, UBS_DISABLE);
		m_btnInven[i].CopyRTSurface(UBS_IDLE, UBS_ON);
	}

	m_btnInven[BIT_Clear].SetBtnState(UBS_ON);
}

// ----------------------------------------------------------------------------
// Name : MouseMoveLoop()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUICashShopEX::MouseMoveLoop(MSG *pMsg, CashShopFrame type)
{
	WMSG_RESULT wmsgResult = WMSG_FAIL;
	int i;

	int nX = LOWORD(pMsg->lParam);
	int nY = HIWORD(pMsg->lParam);

	switch(type)
	{
	case CSF_MainTab:
		{
			for (i=0; i<CS_Total; ++i)
			{
				if (m_SelMainTab != static_cast<CS_MainTab>(i) &&
					m_BtnMainTab[i].MouseMessage(pMsg) != WMSG_FAIL)
				{
					wmsgResult = WMSG_SUCCESS;
				}
			}
		}
		break;
	case CSF_Category:
		{
			m_OverCategoryTab = CSC_NONE;

			for (i=0; i<CSC_Total; ++i)
			{
				if (m_BtnCategory[i].MouseMessage(pMsg) != WMSG_FAIL)
				{
					wmsgResult = WMSG_SUCCESS;
					m_OverCategoryTab = static_cast<CashShopCategory>(i);
				}
			}

			for (i=0; i<CST_Total; ++i)
			{
				if (m_cbCategoryType[i].MouseMessage(pMsg) != WMSG_FAIL)
				{
					wmsgResult = WMSG_SUCCESS;
				}
			}

			if (m_BtnSearch.MouseMessage(pMsg) != WMSG_FAIL)
			{
				wmsgResult = WMSG_SUCCESS;
			}
		}
		break;
	case CSF_View:
		{
			if (m_ViewFrameType == VFT_GoodsList)
			{
				for (i=0; i<8; ++i)
				{
					if (m_GoodsBoxList[i].GoodsData != NULL)
					{
						if (m_GoodsBoxList[i].pIconGoods->MouseMessage(pMsg) != WMSG_FAIL)
						{
							wmsgResult = WMSG_SUCCESS;
						}

						if (m_GoodsBoxList[i].GoodsData->IsPackageEx())
						{
							if (m_GoodsBoxList[i].btnPkgInfo.MouseMessage(pMsg) != WMSG_FAIL)
							{
								m_bPackageInfo = TRUE;
								wmsgResult = WMSG_SUCCESS;
							}
						}
					}
				}

				for (i=0; i<VPT_Total; ++i)
				{
					if (m_BtnPage[i].MouseMessage(pMsg) != WMSG_FAIL)
					{
						wmsgResult = WMSG_SUCCESS;
					}
				}
			}
			else if (m_ViewFrameType == VFT_PurchaseList)
			{
				if (m_BtnMove.MouseMessage(pMsg) != WMSG_FAIL)
				{
					// 옮겨질 아이템을 예비로 찾자.
					if (!m_bShowPurchaseOver)
					{
						MovePurchaseGoods(TRUE);
						m_bShowPurchaseOver = TRUE;
					}
					wmsgResult = WMSG_SUCCESS;
				}
				else
				{
					if (m_bShowPurchaseOver)
					{
						for (i=0; i<_PurchaseListCount; ++i)
						{
							m_PurchaseItemList[i].bEnable = FALSE;
						}

						m_bShowPurchaseOver = FALSE;
					}
				}

				if (m_BtnGet.MouseMessage(pMsg) != WMSG_FAIL)
				{
					wmsgResult = WMSG_SUCCESS;
				}
				if (m_BtnGift.MouseMessage(pMsg) != WMSG_FAIL)
				{
					wmsgResult = WMSG_SUCCESS;
				}

				for (i=0; i<_PurchaseListCount; ++i)
				{
					if (m_PurchaseItemList[i].pIconGoods->MouseMessage(pMsg) != WMSG_FAIL)
					{
						wmsgResult = WMSG_SUCCESS;
					}
				}

				for (i=0; i<_PurchaseGetCount; ++i)
				{
					if (m_PurchaseGetItemList[i]->MouseMessage(pMsg) != WMSG_FAIL)
					{
						wmsgResult = WMSG_SUCCESS;
					}
				}
			}
			else if (m_ViewFrameType == VFT_WishList)
			{
				for (i=0; i<_GoodsListCount; ++i)
				{
					if (m_WishBoxList[i].GoodsData != NULL)
					{
						if (m_WishBoxList[i].pIconGoods->MouseMessage(pMsg) != WMSG_FAIL)
						{
							wmsgResult = WMSG_SUCCESS;
						}

						if (m_WishBoxList[i].GoodsData->IsPackageEx())
						{
							if (m_WishBoxList[i].btnPkgInfo.MouseMessage(pMsg) != WMSG_FAIL)
							{
								m_bPackageInfo = TRUE;
								wmsgResult = WMSG_SUCCESS;
							}
						}
					}
				}
			}
			else if (m_ViewFrameType == VFT_GiftList)
			{
				if ((wmsgResult = m_GiftScroll.MouseMessage(pMsg)) != WMSG_FAIL)
				{
					if (wmsgResult == WMSG_COMMAND)
					{
						SetRevGiftDatas(m_GiftScroll.GetScrollPos());
					}
					wmsgResult = WMSG_SUCCESS;
				}
				else if (m_BtnGet.MouseMessage(pMsg) != WMSG_FAIL)
				{
					wmsgResult = WMSG_SUCCESS;
				}

				for (i=0; i<5; ++i)
				{
					if (!m_pIconRevGift[i]->IsEmpty())
					{
						if (m_pIconRevGift[i]->MouseMessage(pMsg) != WMSG_FAIL)
						{
							wmsgResult = WMSG_SUCCESS;
						}
					}
				}
			}
		}
		break;
	case CSF_Recommand:
		{
			for (i=0; i<_RankCount; ++i)
			{
				if (m_RecommandBoxList[i].GoodsData != NULL)
				{
					if (m_RecommandBoxList[i].pIconGoods->MouseMessage(pMsg) != WMSG_FAIL)
					{
						wmsgResult = WMSG_SUCCESS;
					}
				}
			}
		}
		break;
	case CSF_Preview:
		{
			for (i=0; i<MAX_CASHEQUIP; ++i)
			{
				if (!m_pIconEquip[i]->IsEmpty())
				{
					if (m_pIconEquip[i]->MouseMessage(pMsg) != WMSG_FAIL)
					{
						wmsgResult = WMSG_SUCCESS;
					}
				}
			}

			if (m_btnRestore.MouseMessage(pMsg) != WMSG_FAIL)
			{
				wmsgResult = WMSG_SUCCESS;
			}
		}
		break;
	case CSF_Inven:
		{
			for (i=0; i<MAX_CASHINVEN; ++i)
			{
				if (!m_pIconInven[i]->IsEmpty())
				{
					if (m_pIconInven[i]->MouseMessage(pMsg) != WMSG_FAIL)
					{
						wmsgResult = WMSG_SUCCESS;
					}
				}
			}

			for (i=0; i<BIT_Total; ++i)
			{
				if (m_btnInven[i].MouseMessage(pMsg) != WMSG_FAIL)
				{
					wmsgResult = WMSG_SUCCESS;
				}
			}
		}
		break;
	}

	return wmsgResult;
}

// ----------------------------------------------------------------------------
// Name : SetMyCash()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::SetMyCash(SQUAD myCash)
{
	m_hasCash = myCash;
}

// ----------------------------------------------------------------------------
// Name : ClearGoodsList()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::ClearGoodsList(void)
{
	for (int i=0; i<_GoodsListCount; ++i)
	{
		m_GoodsBoxList[i].GoodsData = NULL;
	}
}

// ----------------------------------------------------------------------------
// Name : ClearRecommandList()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::ClearRecommandList()
{
	for (int i=0; i<_RankCount; ++i)
	{
		m_RecommandBoxList[i].GoodsData = NULL;
	}
}

// ----------------------------------------------------------------------------
// Name : ClearWishList()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::ClearWishList()
{
	for (int i=0; i<_GoodsListCount; ++i)
	{
		m_WishBoxList[i].GoodsData = NULL;
	}
}

// ----------------------------------------------------------------------------
// Name : ClearPurchaseItemList()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::ClearPurchaseItemList()
{
	int i;
	for (i=0; i<_PurchaseListCount; ++i)
	{
		m_PurchaseItemList[i].Clear();
	}

	for (i=0; i<_PurchaseGetCount; ++i)
	{
		m_PurchaseGetItemList[i]->clearIconData();
	}
}

// ----------------------------------------------------------------------------
// Name : SetGoodsList()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::SetGoodsList(CNetworkMessage *istr)
{
	ULONG ulSearchType; // 검색 종류
	UBYTE ubCatalogCount; // 상품 갯수 0 ~ 8
	ULONG ulCTID; // 상품 고유 색인
	SLONG slRemainCount; // 한정 상품 남은 갯수
	ULONG ulPrice; // 상품 가격
	ULONG ulCurrentPage; // 현재 페이지
	ULONG ulEndPage; // 전체 페이지 수

	(*istr) >> ulSearchType >> ubCatalogCount;

	m_CurrentPageType = ulSearchType;
	ClearGoodsList(); // 페이지 상품 정보 클리어

	for (int i=0; i<ubCatalogCount; ++i)
	{
		(*istr) >> ulCTID >> ulSearchType >> slRemainCount >> ulPrice;

		m_GoodsBoxList[i].GoodsData = _pNetwork->GetCashShopData().GetCashShopData(ulCTID);
		CCashShopData::CASH_SHOP_DATA* tmpData = _pNetwork->GetCashShopData().GetCashShopData(ulCTID);

		if (m_GoodsBoxList[i].GoodsData != NULL)
		{
			m_GoodsBoxList[i].GoodsData->m_cash = ulPrice;
			m_GoodsBoxList[i].GoodsData->m_type = ulSearchType;
			m_GoodsBoxList[i].GoodsData->m_limitCnt = slRemainCount;

			m_GoodsBoxList[i].pIconGoods->setCash(tmpData->m_shopItemIndex);			
		}
	}

	(*istr) >> ulCurrentPage >> ulEndPage;

	m_iCurrentPage = ulCurrentPage;
	m_iMaxPage = ulEndPage;
	m_ViewFrameType = VFT_GoodsList;
	m_bLoadingData = FALSE;
	if (m_SelMainTab != CS_IrisShop)
	{
		m_BtnMainTab[m_SelMainTab].SetBtnState(UBS_IDLE);
		m_BtnMainTab[CS_IrisShop].SetBtnState(UBS_CLICK);
		m_SelMainTab = CS_IrisShop;
	}

	INDEX iCategory = m_CurrentPageType / 10000;
	INDEX iFirstSubType = (m_CurrentPageType - (iCategory * 10000)) / 100;
	INDEX iSecondSubType = (m_CurrentPageType - (iCategory * 10000 + iFirstSubType * 100));

	m_cbCategoryType[CST_First].ResetStrings();
	m_cbCategoryType[CST_Second].ResetStrings();
	m_cbCategoryType[CST_First].AddString(_s("----"));
	m_cbCategoryType[CST_Second].AddString(_s("----"));	
	SetComboBox(static_cast<CashShopCategory>(iCategory-1));
	SetSubComboBox(static_cast<CashShopCategory>(iCategory-1), iFirstSubType, iSecondSubType);
	m_cbCategoryType[CST_First].SetCurSel(iFirstSubType);
	m_cbCategoryType[CST_Second].SetCurSel(iSecondSubType);
	m_SelCategoryTab = static_cast<CashShopCategory>(iCategory-1);
}

// ----------------------------------------------------------------------------
// Name : SetRecommandList()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::SetRecommandList(CNetworkMessage *istr)
{
	UBYTE ubCatalogCount; // 상품 갯수 0 ~ 5
	ULONG ulCTID; // 상품 고유 색인
	SLONG slRemainCount; // 한정 상품 남은 갯수
	ULONG ulPrice; // 상품 가격
	ULONG ulSearchType;

	(*istr) >> ubCatalogCount;

	ClearRecommandList();

	for (int i=0; i<ubCatalogCount; ++i)
	{
		(*istr) >> ulCTID >> ulSearchType >> slRemainCount >> ulPrice;

		m_RecommandBoxList[i].GoodsData = _pNetwork->GetCashShopData().GetCashShopData(ulCTID);
		CCashShopData::CASH_SHOP_DATA* tmpData = _pNetwork->GetCashShopData().GetCashShopData(ulCTID);

		if (m_RecommandBoxList[i].GoodsData != NULL)
		{
			m_RecommandBoxList[i].GoodsData->m_cash = ulPrice;
			m_RecommandBoxList[i].GoodsData->m_type = ulSearchType;
			m_RecommandBoxList[i].GoodsData->m_limitCnt = slRemainCount;

			m_RecommandBoxList[i].pIconGoods->setCash(tmpData->m_shopItemIndex);
		}
	}
}

// ----------------------------------------------------------------------------
// Name : SetWishList()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::SetWishList(CNetworkMessage *istr)
{
	UBYTE ubCatalogCount; // 상품 갯수 0 ~ 8
	ULONG ulCTID; // 상품 고유 색인
	SLONG slRemainCount; // 한정 상품 남은 갯수
	ULONG ulPrice; // 상품 가격
	ULONG ulSearchType;

	(*istr) >> ubCatalogCount;

	ClearWishList(); // 상품 정보 클리어

	for (int i=0; i<ubCatalogCount; ++i)
	{
		(*istr) >> ulCTID >> ulSearchType >> slRemainCount >> ulPrice;

		m_WishBoxList[i].GoodsData = _pNetwork->GetCashShopData().GetCashShopData(ulCTID);
		CCashShopData::CASH_SHOP_DATA* tmpData = _pNetwork->GetCashShopData().GetCashShopData(ulCTID);

		if (m_WishBoxList[i].GoodsData != NULL)
		{
			m_WishBoxList[i].GoodsData->m_cash = ulPrice;
			m_WishBoxList[i].GoodsData->m_type = ulSearchType;
			m_WishBoxList[i].GoodsData->m_limitCnt = slRemainCount;

			m_WishBoxList[i].pIconGoods->setCash(tmpData->m_shopItemIndex);
		}
	}

	m_ViewFrameType = VFT_WishList;
	m_SelMainTab = CS_WishPage;
	m_bLoadingData = FALSE;
}

// ----------------------------------------------------------------------------
// Name : ClearCashType()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::ClearCashType(void)
{
	int i;

	for (i=0; i<CSC_Total; ++i)
	{
		INDEX iCount = m_CashSubTypeInfo[i].FirstSubTypeIndexList.Count();

		for (int j=0; j<iCount; ++j)
		{
			m_CashSubTypeInfo[i].FirstSubTypeIndexList[j].Clear();
		}

		m_CashSubTypeInfo[i].FirstSubTypeIndexList.Clear();

		if (i==0)
		{
			CS_FirstSubType& tmpCTTypeInfo1 = m_CashSubTypeInfo[i].FirstSubTypeIndexList.Push(); // 신상품
			tmpCTTypeInfo1.iFirstSubType = 1;
			CS_FirstSubType& tmpCTTypeInfo2 = m_CashSubTypeInfo[i].FirstSubTypeIndexList.Push(); // 특가
			tmpCTTypeInfo2.iFirstSubType = 2;
			CS_FirstSubType& tmpCTTypeInfo3 = m_CashSubTypeInfo[i].FirstSubTypeIndexList.Push(); // 인기상품
			tmpCTTypeInfo3.iFirstSubType = 3;
		}
	}
}

// ----------------------------------------------------------------------------
// Name : SetCashType()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::SetCashType(CNetworkMessage *istr)
{
	UBYTE ubStart;
	ULONG ulSubTypeCount;
	int i, j;

	// Category는 고정이므로 나중에 생각하자.
	// Category type 
	//(*istr) >> ulMax;

	ClearCashType(); // 초기화 한다.

	(*istr) >> ubStart; // 무조건 0
	// first subtype
	(*istr) >> ulSubTypeCount;

	for (i=0; i<ulSubTypeCount; ++i)
	{
		ULONG ulFirstSubTypeIndex;

		(*istr) >> ulFirstSubTypeIndex;

		INDEX iCategory = ulFirstSubTypeIndex / 10000;
		ASSERT(iCategory > 0 && "is not category");
		INDEX iFirstSubType = (ulFirstSubTypeIndex - (iCategory * 10000)) / 100;
		//ASSERT(iFirstSubType > 1 && "is not first subtype"); // 1은 서버에서 none type으로 정의 되어 있다.
		if (iFirstSubType == 0) continue; // none type은 가지지 않는다.

		CS_FirstSubType& tmpCTTypeInfo = m_CashSubTypeInfo[iCategory-1].FirstSubTypeIndexList.Push();
		tmpCTTypeInfo.iFirstSubType = iFirstSubType;
	}

	// 여기까지 first subtype 구성..

	// Second subtype
	(*istr) >> ubStart; // 무조건 1

	(*istr) >> ulSubTypeCount;

	for (i=0; i<ulSubTypeCount; ++i)
	{
		ULONG ulSecondSubTypeCount;

		(*istr) >> ulSecondSubTypeCount;

		INDEX iCategory = ulSecondSubTypeCount / 10000;
		ASSERT(iCategory > 0 && "is not category");
		INDEX iFirstSubType = (ulSecondSubTypeCount - (iCategory * 10000)) / 100;
		//ASSERT(iFirstSubType > 1 && "is not first subtype"); // 1은 서버에서 none type으로 정의 되어 있다.
		if (iFirstSubType == 0) continue; // none type은 가지지 않는다.

		INDEX iSecondSubType = (ulSecondSubTypeCount - (iCategory * 10000 + iFirstSubType * 100));
		if (iSecondSubType == 0) continue; // none type은 가지지 않는다.
		//ASSERT(iSecondSubType > 1 && "is not second subtype"); // 1은 서버에서 none type으로 정의 되어 있다.

		for (j=0; j<m_CashSubTypeInfo[iCategory-1].FirstSubTypeIndexList.Count(); ++j)
		{
			if (m_CashSubTypeInfo[iCategory-1].FirstSubTypeIndexList[j].iFirstSubType == iFirstSubType)
			{
				m_CashSubTypeInfo[iCategory-1].FirstSubTypeIndexList[j].SecondSubTypeIndexList.Add(iSecondSubType);
				break;
			}
		}
	}

	if (!IsVisible())
	{
		 // 여기서 오픈 한다.
		OpenCashShopEX();
		m_bLoadingData = TRUE;
	}
}

// ----------------------------------------------------------------------------
// Name : GetSearchType()
// Desc :
// ----------------------------------------------------------------------------
INDEX CUICashShopEX::GetSearchType(void)
{
	INDEX SearchType, FirstType = 0, SecondType = 0;

	if (m_cbCategoryType[CST_First].GetCurSel() > 0)
	{
		if (m_CashSubTypeInfo[m_SelCategoryTab].FirstSubTypeIndexList.Count() > 0)
		{
			FirstType = m_CashSubTypeInfo[m_SelCategoryTab].FirstSubTypeIndexList[m_cbCategoryType[CST_First].GetCurSel()-1].iFirstSubType;

			if (m_cbCategoryType[CST_Second].GetCurSel() > 0)
			{
				if (m_CashSubTypeInfo[m_SelCategoryTab].FirstSubTypeIndexList[m_cbCategoryType[CST_First].GetCurSel()-1].SecondSubTypeIndexList.Count() > 0)
				{
					SecondType = m_CashSubTypeInfo[m_SelCategoryTab].FirstSubTypeIndexList[m_cbCategoryType[CST_First].GetCurSel()-1].SecondSubTypeIndexList[m_cbCategoryType[CST_Second].GetCurSel()-1];
				}
			}
		}
	}

	SearchType = (m_SelCategoryTab+1)*10000 + FirstType*100 + SecondType;

	return SearchType;
}

// ----------------------------------------------------------------------------
// Name : SetPurchaseItemList()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::SetPurchaseItemList(CNetworkMessage *istr)
{
	ULONG ulCount;
	ULONG ulIndex;
	ULONG ulCTID;

	ClearPurchaseItemList();

	(*istr) >> ulCount;
	ulCount = (ulCount>KIT_SLOT_TOTAL)?KIT_SLOT_TOTAL:ulCount;

	for( int i = 0; i<ulCount; ++i){
		(*istr) >> ulIndex;
		(*istr) >> ulCTID;
		SetCashIndexToBtn(ulCTID,ulIndex);
	}

	m_ViewFrameType = VFT_PurchaseList;
	m_BtnMainTab[m_SelMainTab].SetBtnState(UBS_IDLE);
	m_BtnMainTab[CS_PurchaseInfo].SetBtnState(UBS_CLICK);
	m_SelMainTab = CS_PurchaseInfo;
	m_bLoadingData = FALSE;
}

// ----------------------------------------------------------------------------
// Name : SetCashIndexToBtn()
// Desc :
// ----------------------------------------------------------------------------
BOOL CUICashShopEX::SetCashIndexToBtn(int nCashIndex,int nUniIndex)
{
	CCashShopData& CD = _pNetwork->GetCashShopData();
	
	CCashShopData::CASH_SHOP_DATA* tv_shop = CD.GetCashShopData(nCashIndex);
	
	if (tv_shop != NULL)
	{
		// 구매 정보 버튼을 추가
		for (int i=0; i<_PurchaseListCount; ++i)
		{
			if (m_PurchaseItemList[i].pIconGoods->IsEmpty())
			{
				m_PurchaseItemList[i].bSelected = FALSE;
				m_PurchaseItemList[i].bEnable = FALSE;
				m_PurchaseItemList[i].pIconGoods->setCash(tv_shop->m_shopItemIndex);

				CItems* pItems = m_PurchaseItemList[i].pIconGoods->getItems();

				if (pItems != NULL)
					pItems->Item_UniIndex = nUniIndex;

				return TRUE;
			}
		}
	}

	return FALSE;
}

// ----------------------------------------------------------------------------
// Name : SetRClickMenu()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::SetRClickMenu(INDEX iItem)
{
	m_iRClickMenuIndex = iItem;
	CCashShopData::CASH_SHOP_DATA* tmpData = _pNetwork->GetCashShopData().GetCashShopData(m_iRClickMenuIndex);

	m_tpCommand.ResetAllStrings();
	m_tpCommand.AddMenuList(_S(5224, "아이템 구매하기"), 0xc2bac5FF, CSC_ItemPurchase);
	if (tmpData->m_itemListCnt == 1) // 패키지 상품은 패키지 상품 안에서 입는다.
	{
		CItemData* pCD = _pNetwork->GetItemData(tmpData->m_vItemDataArray[0].m_itemIndex);
		INDEX iJob    = _pNetwork->MyCharacterInfo.job;

		if ( (pCD->GetType() == CItemData::ITEM_WEAPON || pCD->GetType() == CItemData::ITEM_SHIELD) && pCD->CanUse(iJob) && !( pCD->GetWearingPosition() == WEAR_HELMET && (CTString)pCD->GetItemSmcFileName() == MODEL_TREASURE )
			&& (pCD->GetType() == CItemData::ITEM_SHIELD /*&& pCD->GetSubType() != CItemData::ITEM_SHIELD_ONE_SUIT*/ && pCD->GetSubType() != CItemData::ITEM_SHIELD_BACKWING))
		{ // 한벌 의상과 날개도 입을 수 없다.
			m_tpCommand.AddMenuList(_S(5225, "코스튬 입기"), 0xc2bac5FF, CSC_CostumeWear);
		}
	}

	m_tpCommand.AddMenuList(_S(5226, "장바구니에 담기"), 0xc2bac5FF, CSC_AddInven);
	if (m_ViewFrameType == VFT_GoodsList)
	{
		m_tpCommand.AddMenuList(_S(5227, "소망 상자에 담기"), 0xc2bac5FF, CSC_AddWishBox);
	}
	else if (m_ViewFrameType == VFT_WishList)
	{
		m_tpCommand.AddMenuList(_S(5228, "소망 상자에서 삭제"), 0xc2bac5FF, CSC_AddWishBox);
	}
	m_tpCommand.AddMenuList(_S(139, "취소"), 0xc2bac5FF, CSC_Cancel);
	m_tpCommand.Show();
}

// ----------------------------------------------------------------------------
// Name : RunRClickMenu()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::RunRClickMenu(INDEX iType)
{
	int i;
	CCashShopData::CASH_SHOP_DATA* tmpData = _pNetwork->GetCashShopData().GetCashShopData(m_iRClickMenuIndex);

	switch(iType)
	{
	case CSC_ItemPurchase:
		{
			// 아이템 구매하기
			m_Popup.ClearGoodsData();
			m_Popup.AddGoodsData(tmpData);
			m_Popup.OpenPopup(CCashShopPopup::CSP_Purchase);
		}
		break;
	case CSC_CostumeWear:
		{
			// Preview 캐릭터 장비 입히기
			CUIIcon* pIcon = new CUIIcon;
			pIcon->setCash(tmpData->m_shopItemIndex);

			SetWearingModel(pIcon);
			SAFE_DELETE(pIcon);
		}
		break;
	case CSC_AddInven:
		{
			// 장바구니에 담기
			for (i=0; i<MAX_CASHINVEN; ++i)
			{
				if (m_pIconInven[i]->IsEmpty())
				{
					m_pIconInven[i]->setCash(tmpData->m_shopItemIndex);
					m_GoodsPrice += tmpData->m_cash;
					break;
				}
			}
		}
		break;
	case CSC_AddWishBox:
		{
			// 소망상자에 담기
			if (m_ViewFrameType == VFT_GoodsList)
			{
				SendWishItems(tmpData->m_shopItemIndex);
			}
			else if (m_ViewFrameType == VFT_WishList)
			{
				SendWishItems(tmpData->m_shopItemIndex, FALSE);
			}
		}
		break;
	case CSC_Cancel:
		{

		}
		break;
	}

	m_iRClickMenuIndex = 0;
}

// ----------------------------------------------------------------------------
// Name : SetComboBox()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::SetComboBox(CashShopCategory categorytype)
{
	int i;

	switch(categorytype)
	{
	case CSC_HOTandNew:
		{
			for (i=1; i<m_CatalogNamesNodes[categorytype].GetNodeCount(); ++i)
			{
				m_cbCategoryType[CST_First].AddString(m_CatalogNamesNodes[categorytype].GetSubNode(i).GetName());
			}
		}
		break;
	case CSC_Platinum:
		{
		}
		break;
	case CSC_SpendGoods:
	case CSC_EquipGoods:
	case CSC_Avata:
	case CSC_ServiceGoods:
	case CSC_PackageGoods:
		{
			for (i=0; i<m_CashSubTypeInfo[categorytype].FirstSubTypeIndexList.Count(); ++i)
			{
				INDEX iRef = m_CashSubTypeInfo[categorytype].FirstSubTypeIndexList[i].iFirstSubType;
				m_cbCategoryType[CST_First].AddString(m_CatalogNamesNodes[categorytype].GetSubNode(iRef).GetName());
			}
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : SetSubComboBox()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::SetSubComboBox(CashShopCategory categorytype, INDEX iType, INDEX iSubType)
{
	if (iType < 1 || iType > m_CashSubTypeInfo[categorytype].FirstSubTypeIndexList.Count())
	{
		return;
	}

	int i;
	INDEX iSub;
	INDEX iRef;
	INDEX iTmpType = iType-1;

	iSub = m_CashSubTypeInfo[categorytype].FirstSubTypeIndexList[iTmpType].SecondSubTypeIndexList.Count();

	switch(categorytype)
	{
	case CSC_HOTandNew:
		{
		}
		break;
	case CSC_Platinum:
		{
		}
		break;
	case CSC_SpendGoods:
	case CSC_EquipGoods:
	case CSC_Avata:
	case CSC_ServiceGoods:
	case CSC_PackageGoods:
		{
			for (i=0; i<iSub; ++i)
			{
				iRef = m_CashSubTypeInfo[categorytype].FirstSubTypeIndexList[iTmpType].SecondSubTypeIndexList[i];
				m_cbCategoryType[CST_Second].AddString(m_CatalogNamesNodes[categorytype].GetSubNode(iType).GetSubNode(iRef).GetName());
			}
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : SetWearingModel()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::SetWearingModel(CUIIcon* srcBtn, INDEX iCashIndex)
{
	INDEX iItemIndex = 0;
	CCashShopData::CASH_SHOP_DATA* tmpData = NULL;

	if (srcBtn->GetCashIndex() < 0)
	{
		iItemIndex = srcBtn->getIndex();
	}
	else if (srcBtn->GetCashIndex() > 0)
	{
		tmpData = _pNetwork->GetCashShopData().GetCashShopData(srcBtn->GetCashIndex());

		if (!tmpData->IsPackageEx())
		{
			iItemIndex = tmpData->m_vItemDataArray[0].m_itemIndex;
		}
	}

	if (iItemIndex < 1)
	{
		return;
	}

	CItemData*	pItemData = _pNetwork->GetItemData(iItemIndex);

	INDEX iJob, iRef, iType;
	iJob = _pNetwork->MyCharacterInfo.job;

	if (!pItemData->CanUse(iJob) || ( pItemData->GetWearingPosition() == WEAR_HELMET && (CTString)pItemData->GetItemSmcFileName() == MODEL_TREASURE))
	{
		// 착용할 수 있는 직업군이 아니거나 메시 변화가 없는 헬멧임
		return;
	}

	if (pItemData->GetType() == CItemData::ITEM_WEAPON || pItemData->GetType() == CItemData::ITEM_SHIELD)
	{
		if (pItemData->GetType() == CItemData::ITEM_WEAPON)
		{
			//방패를 무조건 벗자
			if (m_iEquipCashIndex[6] > 0)
			{
				CItemData*	pItemData = _pNetwork->GetItemData(iItemIndex);

				if (pItemData->GetSubType() != CItemData::ITEM_WEAPON_KNIFE)
				{
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->HUD_WearingModel(WEAR_SHIELD,_pNetwork->MyCharacterInfo.job,0,FALSE);
					m_iEquipCashIndex[6] = 0;
				}
			}
			iRef = 0;
			iType = WEAR_WEAPON;
			m_pIconEquip[5]->cloneItem(srcBtn);
			m_pIconEquip[5]->SetWhichUI(UI_CASHSHOP_EX);
			m_iEquipCashIndex[5] = iCashIndex;
		}
		else
		{
			iRef = pItemData->GetSubType() + 1;

			if (pItemData->GetSubType() == CItemData::ITEM_SHIELD_HEAD) {
				iType = WEAR_HELMET;
				m_pIconEquip[0]->cloneItem(srcBtn);
				m_pIconEquip[0]->SetWhichUI(UI_CASHSHOP_EX);
				m_iEquipCashIndex[0] = iCashIndex;
			}
			else if (pItemData->GetSubType() == CItemData::ITEM_SHIELD_COAT) {
				iType = WEAR_JACKET;
				m_pIconEquip[1]->cloneItem(srcBtn);
				m_pIconEquip[1]->SetWhichUI(UI_CASHSHOP_EX);
				m_iEquipCashIndex[1] = iCashIndex;
			}
			else if (pItemData->GetSubType() == CItemData::ITEM_SHIELD_PANTS) {
				iType = WEAR_PANTS;
				m_pIconEquip[2]->cloneItem(srcBtn);
				m_pIconEquip[2]->SetWhichUI(UI_CASHSHOP_EX);
				m_iEquipCashIndex[2] = iCashIndex;
			}
			else if (pItemData->GetSubType() == CItemData::ITEM_SHIELD_GLOVE) {
				iType = WEAR_GLOVES;
				m_pIconEquip[3]->cloneItem(srcBtn);
				m_pIconEquip[3]->SetWhichUI(UI_CASHSHOP_EX);
				m_iEquipCashIndex[3] = iCashIndex;
			}
			else if (pItemData->GetSubType() == CItemData::ITEM_SHIELD_SHOES) {
				iType = WEAR_BOOTS;
				m_pIconEquip[4]->cloneItem(srcBtn);
				m_pIconEquip[4]->SetWhichUI(UI_CASHSHOP_EX);
				m_iEquipCashIndex[4] = iCashIndex;
			}
			else if (pItemData->GetSubType() == CItemData::ITEM_SHIELD_SHIELD) {
				if (!m_pIconEquip[5]->IsEmpty())
				{
					INDEX iTmpItemIndex;

					if (m_pIconEquip[5]->GetCashIndex() < 0)
					{
						iTmpItemIndex = m_pIconEquip[5]->getIndex();
					}
					else
					{
						tmpData = _pNetwork->GetCashShopData().GetCashShopData(m_pIconEquip[5]->GetCashIndex());
						iTmpItemIndex = tmpData->m_vItemDataArray[0].m_itemIndex;
					}
					CItemData*	pTmpItemData = _pNetwork->GetItemData(iTmpItemIndex);
					
					if (pTmpItemData->GetSubType() != CItemData::ITEM_WEAPON_KNIFE)
					{
						m_iEquipCashIndex[5] = 0;
						m_pIconEquip[5]->clearIconData();
						((CPlayerEntity*)CEntity::GetPlayerEntity(0))->HUD_WearingModel(WEAR_WEAPON,_pNetwork->MyCharacterInfo.job,0,FALSE);
					}
				}
				iType = WEAR_SHIELD;
				m_iEquipCashIndex[6] = iItemIndex; // 방패만 dbitemindex를 쓴다.
			}
			else if (pItemData->GetSubType() == CItemData::ITEM_SHIELD_ONE_SUIT)
			{ 
				SetSuitWearingModel(pItemData->GetItemIndex(), srcBtn->getIndex());
				return;
			}
		}

		if (pItemData->GetType() == CItemData::ITEM_SHIELD && pItemData->GetSubType() == CItemData::ITEM_SHIELD_BACKWING)
		{ // 날개는 입을 수 없다.
			return;
		}

		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->HUD_WearingModel(iType,iJob,iItemIndex,FALSE);
	}
}

// ----------------------------------------------------------------------------
// Name : SetTakeOffModel()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::SetTakeOffModel(INDEX iRef)
{
	if (iRef > 0)
	{
		INDEX iType = WEAR_HELMET;
		
		if (iRef == 1)
		{
			iType = WEAR_HELMET;
		}
		else if (iRef == 2)
		{
			iType = WEAR_JACKET;
		}
		else if (iRef == 3)
		{
			iType = WEAR_PANTS;
		}
		else if (iRef == 4)
		{
			iType = WEAR_GLOVES;
		}
		else if (iRef == 5)
		{
			iType = WEAR_BOOTS;
		}
		else if (iRef == 6)
		{ // 무기를 벗을 때는 방패도 벗는다.
			iRef = 0; // 0번은 무기
			iType = WEAR_WEAPON; // 방패 아이템은 버린다.//iType = WEAR_SHIELD;
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->HUD_WearingModel(WEAR_SHIELD,_pNetwork->MyCharacterInfo.job,0,FALSE);
			m_iEquipCashIndex[6] = 0;
		}

		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->HUD_WearingModel(iType,_pNetwork->MyCharacterInfo.job,0,FALSE);
	}
/*	else
	{ // 무기 0 --> 방패 6
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->HUD_WearingModel(6,WEAR_SHIELD,_pNetwork->MyCharacterInfo.job,0);
	}*/
}

// ----------------------------------------------------------------------------
// Name : CompletePurchase()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::CompletePurchase()
{
	// Inven clear
	int i;
	for (i=0; i<MAX_CASHINVEN; ++i)
	{
		m_pIconInven[i]->clearIconData();
	}
	m_GoodsPrice = 0;
	// 구매를 완료 하였으므로 구매 정보를 보여줌
	m_bLoadingData = TRUE;
	_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_PURCHASELIST_REQ, m_CurrentPageType, m_iCurrentPage);

	// Popup 종료
	m_Popup.PopupClose();
}

// ----------------------------------------------------------------------------
// Name : CompleteBringItems()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::CompleteBringItems()
{
	int i;

	for (i=0; i<_PurchaseGetCount; ++i)
	{
		m_PurchaseGetItemList[i]->clearIconData();
	}

	m_Popup.PopupClose();
	m_bLoadingData = TRUE;
	_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_PURCHASELIST_REQ);
}

// ----------------------------------------------------------------------------
// Name : ArrangePurchaseGetGoods()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::ArrangePurchaseGetGoods()
{
	int i;

	for (i=0; i<_PurchaseGetCount-1; ++i)
	{
		if (m_PurchaseGetItemList[i]->IsEmpty() && !m_PurchaseGetItemList[i+1]->IsEmpty())
		{
			m_PurchaseGetItemList[i]->copyItem(m_PurchaseGetItemList[i+1]);
			m_PurchaseGetItemList[i+1]->clearIconData();
		}
	}
}

// ----------------------------------------------------------------------------
// Name : MovePurchaseGoods()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::MovePurchaseGoods(BOOL bOver)
{
	int i, j;

	for (i=0; i<_PurchaseGetCount; ++i)
	{
		if (m_PurchaseGetItemList[i]->IsEmpty())
		{
			for (j=0; j<_PurchaseListCount; ++j)
			{
				if ((!bOver &&!m_PurchaseItemList[j].pIconGoods->IsEmpty() && !m_PurchaseItemList[j].bSelected) ||
					(bOver &&!m_PurchaseItemList[j].pIconGoods->IsEmpty() && !m_PurchaseItemList[j].bSelected && !m_PurchaseItemList[j].bEnable))
				{
					if (!bOver)
					{
						m_PurchaseGetItemList[i]->copyItem(m_PurchaseItemList[j].pIconGoods);
						m_PurchaseItemList[j].bSelected = TRUE;
						m_PurchaseItemList[j].bEnable = FALSE;
					}
					else
					{
						m_PurchaseItemList[j].bEnable = TRUE;
					}
					break;
				}
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : SendBringReq()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::SendBringReq()
{
	ULONG ulCount=0;
	int		i;
	for( i = 0; i < _PurchaseGetCount; ++i )
	{
		if (!m_PurchaseGetItemList[i]->IsEmpty())
		{
			++ulCount;
		}
	}

	if (ulCount < 1)
	{
		return;
	}

	CNetworkMessage nmItem((UBYTE)MSG_EXTEND);
	nmItem << (ULONG)MSG_EX_CASHITEM;
	nmItem << (UBYTE)MSG_EX_CASHITEM_BRING_REQ;

	nmItem << ulCount;

	for( i = 0; i < _PurchaseGetCount; ++i )
	{
		if (!m_PurchaseGetItemList[i]->IsEmpty())
		{
			CItems* pItems = m_PurchaseGetItemList[i]->getItems();

			nmItem << (ULONG)pItems->Item_UniIndex;
			nmItem << (ULONG)m_PurchaseGetItemList[i]->GetCashIndex();
		}
	}
	m_bLoadingData = TRUE;
	_pNetwork->SendNetworkMessage(nmItem);
}

// ----------------------------------------------------------------------------
// Name : SendGiftReq()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::SendGiftReq(void)
{
	ULONG ulCount=0;
	int		i;
	for( i = 0; i < _PurchaseGetCount; ++i )
	{
		if (!m_PurchaseGetItemList[i]->IsEmpty())
		{
			++ulCount;
		}
	}

	if (ulCount < 1)
	{
		return;
	}

	CNetworkMessage nmItem((UBYTE)MSG_EXTEND);
	nmItem << (ULONG)MSG_EX_CASHITEM;
	nmItem << (UBYTE)MSG_EX_CASHITEM_GIFT_REQ;

	nmItem << m_Popup.GetCharacterName().GetString();
	nmItem << m_Popup.GetGiftContent().GetString();

	nmItem << ulCount;

	for( i = 0; i < _PurchaseGetCount; ++i )
	{
		if (!m_PurchaseGetItemList[i]->IsEmpty())
		{
			CItems* pItems = m_PurchaseGetItemList[i]->getItems();

			nmItem << (ULONG)pItems->Item_UniIndex;
			nmItem << (ULONG)m_PurchaseGetItemList[i]->GetCashIndex();
		}
	}
	m_bLoadingData = TRUE;
	_pNetwork->SendNetworkMessage(nmItem);
}

// ----------------------------------------------------------------------------
// Name : SendWishItems()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::SendWishItems(ULONG ulCTID, BOOL bCommit)
{
	if (ulCTID > 0)
	{
		CNetworkMessage nmItem((UBYTE)MSG_EXTEND);
		nmItem << (ULONG)MSG_EX_CASHITEM;
		if (bCommit)
		{
			nmItem << (UBYTE)MSG_EX_CASHITEM_WISHLIST_SAVE_REQ; // 등록
		}
		else
		{
			nmItem << (UBYTE)MSG_EX_CASHITEM_WISHLIST_DEL_REQ; // 삭제			
		}

		nmItem << ulCTID;

		m_bLoadingData = TRUE;
		_pNetwork->SendNetworkMessage(nmItem);
	}
}

// ----------------------------------------------------------------------------
// Name : AddGoodsBringData()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::AddGoodsBringData()
{
	int i;

	for (i=0; i<_PurchaseGetCount; ++i)
	{
		CCashShopData::CASH_SHOP_DATA* tmpData = 
			_pNetwork->GetCashShopData().GetCashShopData(m_PurchaseGetItemList[i]->GetCashIndex());
		if (tmpData != NULL)
		{
			m_Popup.AddGoodsData(tmpData);
		}
	}
}

// ----------------------------------------------------------------------------
// Name : RevWishList()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::RevWishList(CNetworkMessage* istr, BOOL bCommit)
{
	UBYTE ubError;
	ULONG ulCTID;
	(*istr) >> ubError;
	(*istr) >> ulCTID;

	m_bLoadingData = FALSE;

	if (ubError > 0)
	{
		switch(ubError)
		{
		case 1:
			{
				if (bCommit)
				{
					Message(MSGCMD_CASH_EX_ERROR, _S(5270, "소망상자"), _S(5271, "더 이상 소망 상자에 아이템을 담을 수 없습니다."), UMBS_OK);
				}
				else
				{
					Message(MSGCMD_CASH_EX_ERROR, _S(5270, "소망상자"), _S(5272, "소망 상자에 존재하지 않는 상품입니다."), UMBS_OK);
				}
			}
			break;
		case 2:
			{
				Message(MSGCMD_CASH_EX_ERROR, _S(5270, "소망상자"), _S(5273, "시스템 오류입니다. 운영자에게 문의하십시오."), UMBS_OK);
			}
			break;
		}
	}
	else
	{
		if (bCommit)
		{
			Message(MSGCMD_CASH_EX_ERROR, _S(5270, "소망상자"), _S(5315, "소망상자 담기를 완료하였습니다"), UMBS_OK);
		}

		if (m_ViewFrameType == VFT_WishList)
		{
			for (int i=0; i<8; ++i)
			{
				if (bCommit)
				{
					if (m_WishBoxList[i].GoodsData == NULL)
					{
						CCashShopData::CASH_SHOP_DATA* tmpData = _pNetwork->GetCashShopData().GetCashShopData(ulCTID);
						m_WishBoxList[i].GoodsData = tmpData;
						m_WishBoxList[i].pIconGoods->setCash(tmpData->m_shopItemIndex);
						break;
					}
				}
				else
				{
					if (m_WishBoxList[i].GoodsData != NULL)
					{
						if (m_WishBoxList[i].GoodsData->m_shopItemIndex == ulCTID)
						{
							m_WishBoxList[i].GoodsData = NULL;
							m_WishBoxList[i].pIconGoods->clearIconData();
						}
					}

					if (i<7 && m_WishBoxList[i].GoodsData == NULL)
					{
						m_WishBoxList[i].GoodsData = m_WishBoxList[i+1].GoodsData;
						m_WishBoxList[i].pIconGoods->copyItem(m_WishBoxList[i+1].pIconGoods);

						m_WishBoxList[i+1].GoodsData = NULL;
						m_WishBoxList[i+1].pIconGoods->clearIconData();
					}
				}
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : SetRevGiftDatas()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::SetRevGiftDatas(INDEX iPos)
{
	if (m_RevGiftList.Count() > 0)
	{
		int i,j;

		for (i=0,j=iPos; j<m_RevGiftList.Count() && i<5; ++i)
		{
			if (i < m_RevGiftList.Count())
			{
				CCashShopData::CASH_SHOP_DATA* tmpData = m_RevGiftList[j+i].GoodsData;

				if (tmpData != NULL)
				{
					m_pIconRevGift[i]->setCash(tmpData->m_shopItemIndex);
				}
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : RevGiftList()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::RevGiftList(CNetworkMessage* istr)
{ // 5개씩 받고 있다.
	UBYTE listFlag;
	ULONG ulCount, ulIndex, ulCTID, ulDate;
	CTString strChar, strMessage;
	int i;

	(*istr) >> listFlag;
	(*istr) >> ulCount;

	if (listFlag & 0x01) // 시작
	{
		m_RevGiftList.Clear();
		m_SelectedGift = -1;
		m_edGiftContent.ResetString();
		for (i=0; i<5; ++i)
		{
			m_pIconRevGift[i]->clearIconData();
		}
	}

	for (i=0; i<ulCount; ++i)
	{
		(*istr) >> ulIndex >> ulCTID >> ulDate;
		(*istr) >> strChar >> strMessage;
		CS_GiftData tmpData;
		tmpData.ulUniIndex = ulIndex;
		tmpData.ulDate = ulDate;
		tmpData.strName = strChar;
		tmpData.strMessage = strMessage;
		tmpData.GoodsData = _pNetwork->GetCashShopData().GetCashShopData(ulCTID);
		
		if (tmpData.GoodsData)
		{
			m_RevGiftList.Add(tmpData);
		}
	}

	if (listFlag & 0x02 || ulCount == 0) // End
	{
		SetRevGiftDatas(0);
		m_GiftScroll.SetScrollPos(0);
		m_GiftScroll.SetCurItemCount(m_RevGiftList.Count());
		m_ViewFrameType = VFT_GiftList;
		m_SelMainTab = CS_GotTheGift;
		m_bLoadingData = FALSE;
	}
}

// ----------------------------------------------------------------------------
// Name : SendGetRevGiftItem()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::SendGetRevGiftItem(INDEX iPos)
{
	CNetworkMessage nmItem((UBYTE)MSG_EXTEND);
	nmItem << (ULONG)MSG_EX_CASHITEM;
	nmItem << (UBYTE)MSG_EX_CASHITEM_GIFT_RECV_REQ;

	nmItem << (ULONG)1; // 현재 선물 가져오기는 하나씩만 가져온다.
	nmItem << m_RevGiftList[iPos].ulUniIndex;
	nmItem << m_RevGiftList[iPos].GoodsData->m_shopItemIndex;

	m_bLoadingData = TRUE;
	_pNetwork->SendNetworkMessage(nmItem);
}

// ----------------------------------------------------------------------------
// Name : RevResultGetGift()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::RevResultGetGift(void)
{
	m_bLoadingData = TRUE;
	_pNetwork->SendCashItemMessage(MSG_EX_CASHITEM_GIFT_RECVLIST_REQ);
}

// ----------------------------------------------------------------------------
// Name : ForcedExit()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::ForcedExit()
{
	if (g_web.IsWebHandle())
	{
		HWND hTmp = g_web.GetWebHandle();

		g_web.CloseWebPage(hTmp);
		ShowWindow(hTmp, SW_HIDE);
		UpdateWindow(hTmp);		
	}

	if (IsVisible())
	{
		if (m_Popup.IsVisible())
		{
			m_Popup.PopupClose();
		}
		CUIManager::getSingleton()->RearrangeOrder(UI_CASHSHOP_EX, FALSE);
	}

	m_bCashShopLock = TRUE;
}

// ----------------------------------------------------------------------------
// Name : Message()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::Message( int nCommandCode, CTString strTitle, CTString strMessage, DWORD dwStyle )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	if( pUIManager->DoesMessageBoxExist( nCommandCode ) )
		return;

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( strTitle, dwStyle, UI_CASHSHOP_EX, nCommandCode ); 
	
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : SetPackageWearingModel()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::SetPackageWearingModel(CUIIcon* srcBtn, INDEX iCashIndex)
{
	INDEX iItemIndex = 0;
	INDEX iInCashIndex = 0;
	CCashShopData::CASH_SHOP_DATA* tmpData = NULL;
	int i=0;
	CUIIcon kIcon;

	if (iCashIndex > 0)
	{
		iInCashIndex = iCashIndex;
	}
	else
	{
		iInCashIndex = srcBtn->GetCashIndex();
	}

	tmpData = _pNetwork->GetCashShopData().GetCashShopData(iInCashIndex);

	if (tmpData)
	{
		for (i=0; i<tmpData->m_vItemDataArray.size(); ++i)
		{
			iItemIndex = tmpData->m_vItemDataArray[i].m_itemIndex;

			CItemData*	pItemData = _pNetwork->GetItemData(iItemIndex);
			kIcon.setData(UBET_ITEM, iItemIndex);
			CItems* pItems = kIcon.getItems();
			pItems->Item_Sum = tmpData->m_vItemDataArray[i].m_itemCnt;
			pItems->Item_Flag = tmpData->m_vItemDataArray[i].m_flag;
			pItems->Item_Plus = tmpData->m_vItemDataArray[i].m_plus;
			
			SetWearingModel(&kIcon, iInCashIndex);
		}
	}
}

// ----------------------------------------------------------------------------
// Name : SetSuitWearingModel()
// Desc :
// ----------------------------------------------------------------------------
void CUICashShopEX::SetSuitWearingModel(INDEX iItem, INDEX iCash)
{
	INDEX iItemIndex = iItem;
	INDEX iCashIndex = iCash;

	CItemData* pItemMainData = _pNetwork->GetItemData(iItemIndex);

	// Suit 파츠 갯수는 5가지
	const INDEX suitCount = 5;
	CUIIcon kIcon;

	for (int i=0; i<5; ++i)
	{
		CItemData* pItemData = _pNetwork->GetItemData(pItemMainData->GetNeedItemCount(i));
		// UniIndex에 한벌의상 아이템 인덱스를 등록
		kIcon.setData(UBET_ITEM, pItemData->GetItemIndex());
		kIcon.setCount(1);
//		kIcon.SetSuitIndex(iItemIndex);

		SetWearingModel(&kIcon, iCashIndex);
	}
}
