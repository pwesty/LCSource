#include "stdh.h"
#include "UIQuestAccept.h"
#include <Engine/GameDataManager/GameDataManager.h>

#include <vector>
#include <algorithm>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/GlobalDefinition.h>

#include <Engine/Interface/UIPetTraining.h>
#include <Engine/Interface/UIShop.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Contents/Base/UIQuestNew.h>
#include <Engine/Contents/function/UIPortalNew.h>
#include <Engine/Interface/UISiegeWarfare.h>
#include <Engine/Interface/UIWareHouse.h>
#include <Engine/Interface/UIGuild.h>
#include <Engine/Interface/UIInitJob.h>
#include <Engine/Interface/UIRefine.h>
#include <Engine/Contents/Base/UIChangeWeaponNew.h>
#include <Engine/Interface/UIPlayerInfo.h>
#include <Engine/Interface/UIProcessNPC.h>
#include <Engine/Interface/UISkillLearn.h>
#include <Engine/Interface/UIProduct.h>
#include <Engine/Contents/Base/UIQuestComplete.h>

//--------------------------------------------------------------
// 수락창 닫기
class CmdQuestAcceptClose : public Command
{
public:
	CmdQuestAcceptClose() : pUI_(NULL) {}
	void setData(CUIQuestAccept* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->close();
	}
private:
	CUIQuestAccept* pUI_;
};

//--------------------------------------------------------------
// 수락 버튼
class CmdQuestAcceptOk : public Command
{
public:
	CmdQuestAcceptOk() : pUI_(NULL) {}
	void setData(CUIQuestAccept* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
		{
			GAMEDATAMGR()->GetQuest()->SendAcceptReq( pUI_->GetAcceptIdx(), pUI_->GetQuestItem());
		}
	}
private:
	CUIQuestAccept* pUI_;
};

//--------------------------------------------------------------
// npc 메뉴로 돌아감
class CmdQuestReturnNpcMenu : public Command
{
public:
	CmdQuestReturnNpcMenu() : pUI_(NULL) {}
	void setData(CUIQuestAccept* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
		{
			pUI_->ReturnToNPCMenu();
		}
	}
private:
	CUIQuestAccept* pUI_;
};

//////////////////////////////////////////////////////////////////////////
// 퀘스트 수락창

CUIQuestAccept::CUIQuestAccept()
	: m_pbtnClose(NULL)
	, m_pmoveArea(NULL)
	, m_plistContent(NULL)
	, m_ptbName(NULL)
	, m_pbtnAccept(NULL)
	, m_pbtnCancle(NULL)
	, m_pbtnPrev(NULL)
	, m_pDesign(NULL)
	, m_plistScroll(NULL)
	, m_bDrag(false)
	, m_nOriX(0), m_nOriY(0)
	, m_nMouseX(0)
	, m_nMouseY(0)
	, m_nTargetIndex(0)
	, m_pQuestItem(NULL)
	, m_nQuestIdx(-1)
{
	for (int i = 0; i < eICON_MAX; i++)
	{
		m_pImageArr[i] = NULL;
	}
	setInherit(false);
}

CUIQuestAccept::~CUIQuestAccept()
{
	SAFE_DELETE(m_pDesign);
	Destroy();
}


void CUIQuestAccept::UpdateQuest()
{
	if(m_nQuestIdx == -1)
		return;

	int		i;

	CQuestDynamicData* pQuestDD = CQuestSystem::Instance().Create( m_nQuestIdx );

	// 타입 아이콘
	int nType[eICON_MAX];
	int nToolTipIdx[eICON_MAX];
	nType[eQUEST_TYPE1] = pQuestDD->GetQuestType1();
	nType[eQUEST_TYPE2] = pQuestDD->GetQuestType2();
	nType[eQUEST_SCALE] = pQuestDD->GetQuestPartyScale();

	nToolTipIdx[eQUEST_TYPE1] = nType1StringIdx[nType[eQUEST_TYPE1]];
	nToolTipIdx[eQUEST_TYPE2] = nType2StringIdx[nType[eQUEST_TYPE2]];
	nToolTipIdx[eQUEST_SCALE] = nScaleStringIdx[nType[eQUEST_SCALE]];

	CTString strTooltip;
	COLOR	tempColor;

	for (i = 0; i < eICON_MAX; i++)
	{		
		if(m_pImageArr[i] != NULL)
		{
			m_pImageArr[i]->SetRenderIdx(nType[i]);
			strTooltip.PrintF(_S(nToolTipIdx[i], "타입 툴팁"));
			m_pImageArr[i]->setTooltip(strTooltip.str_String);
		}
	}	

	// 퀘스트 이름
	if (m_ptbName != NULL)
	{
		tempColor = m_ptbName->GetTextColor();
		m_ptbName->SetAlignTextV(eALIGN_V_CENTER);
		m_ptbName->SetText( CTString(pQuestDD->GetName()), tempColor );
		m_ptbName->StringEllipsis(2);

		if (m_ptbName->IsEllipsis() == true)
		{
			m_ptbName->setTooltip(CTString(pQuestDD->GetName()));
		}
		else
		{
			m_ptbName->setTooltip(CTString(""));
		}
		m_ptbName->Hide(FALSE);
	}

	// List 내용
	if (m_plistContent == NULL)
		return;

	m_plistContent->DeleteAllListItem();
	if (m_plistScroll)
		m_plistScroll->SetScrollPos(0);

	CUITextBox* pTextBox = NULL;
	CUITextBox* pTempTb = NULL;
	CUIText* pText = NULL;
	CUIImage* pImg = NULL;
	CUIIcon* pIcon = NULL;
	CTString strTemp;
	CUIListItem* plistItem = NULL;

	// 1. Summury
	pTextBox = (CUITextBox*)m_pDesign->CloneSummury();

	if (pTextBox != NULL)
	{		
		pTextBox->SetText(CTString(pQuestDD->GetDesc3()));
		int nCnt = pTextBox->GetLineCount();

		for (i = 0; i < nCnt; ++i)
		{			
			pText = (CUIText*)m_pDesign->CloneSummuryText();

			if(pText != NULL)
			{
				strTemp = pTextBox->GetLineString(i);
				pText->SetText(strTemp);
				m_plistContent->AddListItem(pText);
			}
		}

		SAFE_DELETE(pTextBox);
		pText = (CUIText*)m_pDesign->CloneSpace();
		if (pText != NULL)
		{
			m_plistContent->AddListItem(pText);
		}
	}
	
	// 2. Npc
	plistItem = (CUIListItem*)m_pDesign->CloneTitle();

	if (plistItem != NULL)
	{
		// Name
		if (pQuestDD->GetStartType() == QSTART_NPC)
		{
			CMobData* pNpc = CMobData::getData(pQuestDD->GetStartData());

			strTemp.PrintF("%s", pNpc->GetName());
		}
		else
		{
			strTemp.PrintF(_S(457, "시스템"));
		}

		pText = (CUIText*)plistItem->findUI("str_copytitle");

		if (pText != NULL)
			pText->SetText(strTemp);

		m_plistContent->AddListItem(plistItem);
	}

	// 3. StartDesc
	pTextBox = (CUITextBox*)m_pDesign->CloneDesc();

	if (pTextBox != NULL)
	{
		pTextBox->SetText(CTString(pQuestDD->GetDesc()));

		int nCnt = pTextBox->GetLineCount();

		for (i = 0; i < nCnt; ++i)
		{			
			pText = (CUIText*)m_pDesign->CloneDescText();

			if(pText != NULL)
			{
				strTemp = pTextBox->GetLineString(i);
				pText->SetText(strTemp);
				m_plistContent->AddListItem(pText);
			}
		}

		SAFE_DELETE(pTextBox);
		pText = (CUIText*)m_pDesign->CloneSpace();
		if (pText != NULL)
		{
			m_plistContent->AddListItem(pText);
		}
	}

	// 4. 수행가능 레벨
	plistItem = (CUIListItem*)m_pDesign->CloneTitle();

	if (plistItem != NULL)
	{
		strTemp.PrintF(_S( 1704, "수행가능 레벨" ));

		pText = (CUIText*)plistItem->findUI("str_copytitle");

		if (pText != NULL)
			pText->SetText(strTemp);
		
		m_plistContent->AddListItem(plistItem);
	}
	
	// 5. 레벨 값	
	pTextBox = (CUITextBox*)m_pDesign->CloneDesc();

	if (pTextBox != NULL)
	{
		if(pQuestDD->GetNeedMinLevel() == pQuestDD->GetNeedMaxLevel())
		{
			strTemp.PrintF(_S( 1705, "레벨 %d" ), pQuestDD->GetNeedMinLevel());
		}
		else if(pQuestDD->GetNeedMaxLevel()==999)
		{
			strTemp.PrintF(_S( 5667, "레벨 : %d ~ MAX" ), pQuestDD->GetNeedMinLevel());
		}
		else
		{
			strTemp.PrintF(_S( 1706, "레벨 %d ~ 레벨 %d" ), pQuestDD->GetNeedMinLevel(), pQuestDD->GetNeedMaxLevel());
		}

		pTextBox->SetText(strTemp);
		int nCnt = pTextBox->GetLineCount();

		for (i = 0; i < nCnt; ++i)
		{			
			pText = (CUIText*)m_pDesign->CloneDescText();
			if(pText != NULL)
			{
				strTemp = pTextBox->GetLineString(i);
				pText->SetText(strTemp);
				m_plistContent->AddListItem(pText);
			}
		}
		SAFE_DELETE(pTextBox);
		pText = (CUIText*)m_pDesign->CloneSpace();
		if (pText != NULL)
		{
			m_plistContent->AddListItem(pText);
		}
	}
	
	// 6. 퀘스트 보상	
	
	// 고정보상
	if(pQuestDD->IsPrizeExist())
	{
		// 보상 타이틀
		plistItem = (CUIListItem*)m_pDesign->CloneTitle();

		if (plistItem != NULL)
		{
			pText = (CUIText*)plistItem->findUI("str_copytitle");

			if (pText != NULL)
				pText->SetText(pQuestDD->GetPrizeDesc( 0 ));
						
			m_plistContent->AddListItem(plistItem);
		}

		// 보상 아이템		
		CUIImageBox* pImgBox = new CUIImageBox;
		int Count = pQuestDD->GetCountPrizeDesc();

		for( INDEX i = 1; i < Count; ++i )
		{
			plistItem = (CUIListItem*)m_pDesign->CloneListItem();

			if (plistItem == NULL)
				continue;

			// 아이템 설명
			CTString strPrize = pQuestDD->GetPrizeDesc( i );
			bool bIcon = true;
			UIBtnExType eBtnType = UBET_ITEM;

			pTextBox = (CUITextBox*)plistItem->findUI("tb_copyComplete");

			if (pTextBox != NULL)
			{
				tempColor = pTextBox->GetTextColor();
				pTextBox->SetText(strPrize, tempColor, TRUE);
			}

			// 아이콘인지, 이미지 인지 결정
			switch (pQuestDD->GetPrizeType(i-1) )
			{
			case QPRIZE_ITEM:
				eBtnType = UBET_ITEM;
				break;
			case QPRIZE_SKILL:
			case QPRIZE_SSKILL:
				eBtnType = UBET_SKILL;
				break;
			default:
				bIcon = false;
				break;
			}
			
			pIcon = (CUIIcon*)plistItem->findUI("icon_copyItem");
			pImg = (CUIImage*)plistItem->findUI("img_copyitem");

			// 아이템
			if (bIcon == true)
			{			
				if (pIcon != NULL)
				{
					pIcon->setData(eBtnType, pQuestDD->GetPrizeIndex(i-1));
					pIcon->Hide(FALSE);
				}
			}
			else
			{
				if (pImg != NULL)
				{					
					pImgBox->SetImageByType(CUIImageBox::eImageType(pQuestDD->GetPrizeType(i-1)), pQuestDD->GetPrizeIndex(i-1), pQuestDD->GetSyndicateType());
					UIRectUV pUv = pImgBox->GetUV();
					pImg->setTexData( _pUIBtnTexMgr->GetTex( UIBtnExType(pImgBox->GetBtnType()), pImgBox->GetTextureID() ));					
					pImg->SetUV(pUv.U0, pUv.V0, pUv.U1, pUv.V1);
					pImg->Hide(FALSE);
				}
			}

			m_plistContent->AddListItem(plistItem);
		}

		if (pImgBox)
		{
			delete pImgBox;
			pImgBox = NULL;
		}
	}

	//선택 보상
	if(pQuestDD->IsOptionPrizeExist())
	{
		// 보상 타이틀
		plistItem = (CUIListItem*)m_pDesign->CloneTitle();

		if (plistItem != NULL)
		{
			pText = (CUIText*)plistItem->findUI("str_copytitle");

			if (pText != NULL)
				pText->SetText(pQuestDD->GetOptionPrizeDesc( 0 ));
			
			m_plistContent->AddListItem(plistItem);
		}

		// 보상 아이템		
		CUIImageBox* pImgBox = new CUIImageBox;
		int Count = pQuestDD->GetCountOptionPrizeDesc();

		for( INDEX i = 1; i < Count; ++i )
		{
			plistItem = (CUIListItem*)m_pDesign->CloneListItem();

			if (plistItem == NULL)
				continue;

			// 아이템 설명
			CTString strPrize = pQuestDD->GetOptionPrizeDesc( i );
			bool bIcon = true;
			UIBtnExType eBtnType = UBET_ITEM;

			pTextBox = (CUITextBox*)plistItem->findUI("tb_copyComplete");

			if (pTextBox != NULL)
			{
				tempColor = pTextBox->GetTextColor();
				pTextBox->SetText(strPrize, tempColor, TRUE);
			}

			// 아이콘인지, 이미지 인지 결정
			switch (pQuestDD->GetOptionPrizeType(i-1) )
			{
			case QPRIZE_ITEM:
				eBtnType = UBET_ITEM;
				break;
			case QPRIZE_SKILL:
			case QPRIZE_SSKILL:
				eBtnType = UBET_SKILL;
				break;
			default:
				bIcon = false;
				break;
			}

			pIcon = (CUIIcon*)plistItem->findUI("icon_copyItem");
			pImg = (CUIImage*)plistItem->findUI("img_copyitem");

			// 아이템
			if (bIcon == true)
			{			
				if (pIcon != NULL)
				{
					pIcon->setData(eBtnType, pQuestDD->GetOptionPrizeIndex(i-1));
					pIcon->Hide(FALSE);
				}
			}
			else
			{
				if (pImg != NULL)
				{					
					pImgBox->SetImageByType(CUIImageBox::eImageType(pQuestDD->GetOptionPrizeType(i-1)), pQuestDD->GetOptionPrizeIndex(i-1), pQuestDD->GetSyndicateType());
					UIRectUV pUv = pImgBox->GetUV();
					pImg->setTexData( _pUIBtnTexMgr->GetTex( UIBtnExType(pImgBox->GetBtnType()), pImgBox->GetTextureID() ));					
					pImg->SetUV(pUv.U0, pUv.V0, pUv.U1, pUv.V1);
					pImg->Hide(FALSE);
				}
			}
			m_plistContent->AddListItem(plistItem);
		}

		if (pImgBox)
		{
			delete pImgBox;
			pImgBox = NULL;
		}
	}

	int nMaxItem = m_plistContent->getListItemCount();
	m_plistContent->UpdateScroll(nMaxItem);
	m_plistContent->UpdateList();

	int nShowCnt = m_plistContent->GetItemShowNum();

	if (m_plistScroll != NULL)
		m_plistScroll->SetItemsPerPage(nShowCnt);
}

void CUIQuestAccept::open( int nQuestIdx, CItems* pItem)
{
	if (nQuestIdx == -1) return;

	SetVisible(TRUE);
	Hide(FALSE);

	m_nQuestIdx = nQuestIdx;
	
	if (m_pQuestItem == NULL)
	{
	    m_pQuestItem = pItem;
	}

	BOOL bEnable = TRUE;

	if (71 == m_nPrevNPCIndex || m_nPrevNPCIndex < 0)
		bEnable = FALSE;
	
	if (m_pbtnPrev != NULL)
	{
		m_pbtnPrev->SetEnable(bEnable);
	}

	UpdateQuest();

	CQuestSystem::Instance().DeleteCurrentRequest();
	CUIManager::getSingleton()->RearrangeOrder( UI_QUEST_ACCEPT, TRUE );
}

void CUIQuestAccept::Clear()
{
	m_nQuestIdx = -1;
	m_nTargetIndex = -1;
	m_nTargetUIType = -1;
	m_nPrevNPCIndex = -1;
	m_nPrevNPCUIType = -1;
	m_pQuestItem = NULL;
}

void CUIQuestAccept::close()
{
	SetVisible(FALSE);
	Hide(TRUE);

	Clear();
	CUITooltipMgr::getSingleton()->setData(NULL);
	CUIManager* pUIManager = CUIManager::getSingleton();
	GAMEDATAMGR()->GetQuest()->ClearQuest();

	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_QUEST );
	pUIManager->RearrangeOrder( UI_QUEST_ACCEPT, FALSE );
}

void CUIQuestAccept::initialize()
{
	// design
	m_pDesign = new CUIQuestDesign;
	CUIManager::getSingleton()->LoadXML("questDesign.xml", (CUIBase*)m_pDesign);

	m_pbtnClose		= (CUIButton*)findUI("btn_close");

	if (m_pbtnClose != NULL)
	{
		CmdQuestAcceptClose* pCmd = new CmdQuestAcceptClose;
		pCmd->setData(this);
		m_pbtnClose->SetCommandUp(pCmd);
	}

	m_pmoveArea		= (CUIText*)findUI("move_title");
	m_ptbName		= (CUITextBox*)findUI("tb_name");
	m_plistContent	= (CUIList*)findUI("list_info");
	m_pbtnAccept	= (CUIButton*)findUI("btn_ok");

	if (m_pbtnAccept != NULL)
	{
		CmdQuestAcceptOk* pCmd = new CmdQuestAcceptOk;
		pCmd->setData(this);
		m_pbtnAccept->SetCommandUp(pCmd);
	}

	m_pbtnPrev		= (CUIButton*)findUI("btn_giveup");

	if (m_pbtnPrev != NULL)
	{
		CmdQuestReturnNpcMenu* pCmd = new CmdQuestReturnNpcMenu;
		pCmd->setData(this);
		m_pbtnPrev->SetCommandUp(pCmd);
	}

	m_pbtnCancle	= (CUIButton*)findUI("btn_cancle");

	if (m_pbtnCancle != NULL)
	{
		CmdQuestAcceptClose* pCmd = new CmdQuestAcceptClose;
		pCmd->setData(this);
		m_pbtnCancle->SetCommandUp(pCmd);
	}

	CTString strImageName[eICON_MAX] = { "iarr_type1", "iarr_type2", "iarr_scale"};

	for (int i = 0; i < eICON_MAX; i++)
	{
		m_pImageArr[i] = (CUIImageArray*)findUI(strImageName[i]);
	}

	m_plistScroll = (CUIScrollBar*)findUI("list_scroll");
}

void CUIQuestAccept::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2 + 22, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 + 22 );
}

void CUIQuestAccept::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

WMSG_RESULT CUIQuestAccept::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_nMouseX = x;
	m_nMouseY = y;

	if( m_bDrag && ( pMsg->wParam & MK_LBUTTON ) )
	{
		int ndX = x - m_nOriX;
		int ndY = y - m_nOriY;

		m_nOriX = x;
		m_nOriY = y;

		Move( ndX, ndY );
		return WMSG_SUCCESS;
	}

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager::getSingleton()->SetMouseCursorInsideUIs();

	return WMSG_FAIL;
}

WMSG_RESULT CUIQuestAccept::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pmoveArea && m_pmoveArea->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
	}

	CUIManager::getSingleton()->RearrangeOrder( UI_QUEST_ACCEPT, TRUE );

	return WMSG_FAIL;
}

WMSG_RESULT CUIQuestAccept::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	CUIManager::getSingleton()->ResetHoldBtn();
	return WMSG_FAIL;
}

void CUIQuestAccept::ReturnToNPCMenu()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	int nNpcIndex = -1;
	int nTargetUIType = -1;

	nNpcIndex = GetPrevNpcIdx();
	nTargetUIType = GetPrevNpcUIType();

	close();

	m_nTargetIndex = nNpcIndex;
	RequestQuest( nNpcIndex, m_nTargetVirIndex, nTargetUIType, m_fNpcX, m_fNpcZ );
}

void CUIQuestAccept::OpenWindow( BOOL bHasQuestList )
{
	OBFUSCATE();

	int iMobIndex = m_nTargetIndex;
	int iMobVirIndex = m_nTargetVirIndex;
	// [090527: selo] 몹인덱스를 찾지 못하는 경우는 퀘스트 북에서 이전을 눌렀을때 발생한다.
	// 그리하여 PrevNPCIndex 로 다시 얻어온다.
	if( -1 == iMobIndex )
		iMobIndex = m_nPrevNPCIndex;

	CUIManager* pUIManager = CUIManager::getSingleton();

	CMobData* MD = CMobData::getData(iMobIndex);

	if (_pNetwork->IsRvrZone() && _pNetwork->MyCharacterInfo.iSyndicateType != MD->GetSyndicateType())
		return;

	switch( m_nTargetUIType )
	{
	case UI_PORTAL:
		{
			pUIManager->GetPortal()->OpenDratanDungeonMsgBox();
		}
		break;
	case UI_SHOP:
		{
			pUIManager->GetShop()->OpenShop( iMobIndex, iMobVirIndex, bHasQuestList, m_fNpcX, m_fNpcZ );
		}
		break;		
	case UI_WAREHOUSE:
		{			
			pUIManager->GetWareHouse()->SetNPCPos( iMobIndex, m_fNpcX, m_fNpcZ );
			pUIManager->GetWareHouse()->OpenWareHouse(bHasQuestList);
		}
		break;
	case UI_GUILD:
		{
			pUIManager->GetGuild()->OpenGuild( iMobIndex, bHasQuestList, _pNetwork->MyCharacterInfo.lGuildPosition, _pNetwork->MyCharacterInfo.lGuildLevel );
		}
		break;
	case UI_SIEGE_WARFARE:
		{
			pUIManager->GetSiegeWarfare()->OpenSiegeWarfare();
		}
		break;
	case UI_REFINE:
		{
			pUIManager->GetRefine()->OpenRefine( iMobIndex, bHasQuestList, m_fNpcX, m_fNpcZ );
		}
		break;
	case UI_QUEST:
		{
			pUIManager->GetQuest()->OpenQuest( iMobIndex, iMobVirIndex, bHasQuestList, m_fNpcX, m_fNpcZ );
		}
		break;
	case UI_SKILLLEARN:
		{
			pUIManager->GetSkillLearn()->OpenSkillLearn( iMobIndex, iMobVirIndex, bHasQuestList, m_fNpcX, m_fNpcZ );
		}
		break;
	case UI_CHANGEWEAPON:
		{
			pUIManager->GetChangeWeapon()->OpenChangeWeapon( iMobIndex, bHasQuestList, m_fNpcX, m_fNpcZ );
		}
		break;
	case UI_INITJOB:
		{
			pUIManager->GetInitJob()->OpenInitJob( iMobIndex, iMobVirIndex, bHasQuestList, m_fNpcX, m_fNpcZ );
		}
		break;
	case UI_PETTRAINING:	// 애완동물 조련사.
		{
			pUIManager->GetPetTraining()->OpenPetTraining( iMobIndex, bHasQuestList, m_fNpcX, m_fNpcZ );
		}
		break;
	case UI_PROCESSNPC:
		{
			pUIManager->GetProcessNPC()->OpenProcessNPC( );
		}
		break;
	case UI_PRODUCTNPC:
		{
			pUIManager->GetProductNPC()->OpenProductNPC(iMobIndex, iMobVirIndex, m_fNpcX, m_fNpcZ );
		}
		break;
	}
}

ENGINE_API void CUIQuestAccept::RequestQuest( int nNpcIndex, int nNpcVirIndex, SBYTE sbUIType, FLOAT fX, FLOAT fZ )
{
	// If requesting quest is exist
	if( CQuestSystem::Instance().GetCurrentRequest() )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(sbUIType)
	{
	case UI_SHOP:
		{
			if(pUIManager->DoesMessageBoxLExist( MSGLCMD_SHOP_REQ ))
				return;
		}
		break;
	case UI_REFINE:
		{
			if(pUIManager->DoesMessageBoxLExist( MSGLCMD_REFINE_REQ ))
				return;
		}
		break;
	case UI_SKILLLEARN:
		{
			if( pUIManager->DoesMessageBoxLExist( MSGLCMD_SKILLLEARN_REQ ) )
				return;
			if( pUIManager->DoesMessageBoxLExist( MSGLCMD_SSKILLLEARN_REQ ) )
				return;
		}
		break;
	case UI_PROCESSNPC:
		{
			if (pUIManager->DoesMessageBoxLExist( MSGLCMD_PROCESSNPC_REQ ))
				return;
		}
		break;
	case UI_QUEST:
		{
			if (pUIManager->IsUIVisible(sbUIType))
				return;

			if( pUIManager->DoesMessageBoxLExist( MSGLCMD_QUEST_REQ ) || IsVisible() )
				return;
		}
		break;
	case UI_WAREHOUSE:
		{			
			if(pUIManager->DoesMessageBoxLExist( MSGLCMD_WAREHOUSE_REQ ))
				return;
		}
		break;
	case UI_GUILD:
		{
			if(pUIManager->DoesMessageBoxLExist( MSGLCMD_GUILD_REQ ))
				return;
		}
		break;
	case UI_CHANGEWEAPON:
		{
			if( pUIManager->DoesMessageBoxLExist( MSGLCMD_CHANGEWEAPON_REQ ))
				return;
		}
		break;
	case UI_INITJOB:
		{
			if( pUIManager->DoesMessageBoxLExist( MSGLCMD_INITJOB_REQ ))
				return;
		}
		break;
	case UI_SIEGE_WARFARE:
		{
			if(pUIManager->DoesMessageBoxLExist( MSGLCMD_SIEGE_WARFARE ))
				return;
		}
		break;
	case UI_PETTRAINING:	// 애완동물 조련사.
		{
			if(pUIManager->DoesMessageBoxLExist( MSGLCMD_PETTRAINING_REQ ))
				return;
		}
		break;
	case UI_PRODUCTNPC:
		{
			if (pUIManager->DoesMessageBoxLExist(MSGLCMD_PRODUCETNPC_VOUCHER))
			{
				return;
			}

			if (pUIManager->DoesMessageBoxLExist(MSGLCMD_PRODUCETNPC_LEARN))
			{
				return;
			}
		}
		break;
	case UI_PORTAL:
		{
			if (pUIManager->IsUIVisible(sbUIType))
				return;

			if (pUIManager->DoesMessageBoxLExist(MSGLCMD_DRATAN_SIEGE_DUNGEON_REQ) || IsVisible())
				return;
		}
		break;
	}

	CMobData* MD = CMobData::getData(nNpcIndex);
	m_nTargetIndex	= nNpcIndex;
	m_nTargetVirIndex = nNpcVirIndex;
	m_nTargetUIType = sbUIType;

	// Set position of target npc
	m_fNpcX = fX;
	m_fNpcZ = fZ;

	GAMEDATAMGR()->GetQuest()->SendQuestReq( nNpcIndex );
	GAMEDATAMGR()->GetQuest()->ClearQuest();

	CUIQuestComplete* pUIComplete = pUIManager->GetQuestBookComplete();

	if (pUIComplete == NULL)
		return;
	
	pUIComplete->SetTargetIndex(nNpcIndex);
	pUIComplete->SetTargetUIType(sbUIType);
	pUIComplete->SetTargetVirIndex(nNpcVirIndex);
	pUIComplete->SetNpcPosX(fX);
	pUIComplete->SetNpcPosZ(fZ);
}
