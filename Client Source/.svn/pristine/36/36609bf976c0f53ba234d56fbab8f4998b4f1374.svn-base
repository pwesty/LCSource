#include "stdh.h"

#include <Engine/Interface/UIInternalClasses.h>
#include "UIMapOption.h"
#include <Engine/Help/Util_Help.h>
#include <Engine/Interface/UIRadar.h>

extern INDEX g_iCountry;

CUIMapOption::CUIMapOption()
{
	setInherit(false);

	for (int i = 0; i < eNT_MAX; ++i)
	{
		m_pCheckBtn[i] = NULL;
	}

	m_bInit = false;
}

CUIMapOption::~CUIMapOption()
{
	
}

bool CUIMapOption::IsCheck( eNPC_TYPE eType )
{
	if (m_pCheckBtn[eType] == NULL)
		return false;

	return m_pCheckBtn[eType]->IsChecked() ? true : false;
}

UIRectUV CUIMapOption::GetUV( eNPC_TYPE eType )
{
	UIRectUV uv;

	if (m_pCheckBtn[eType] == NULL)
		return uv;

	uv = m_pImgIcon[eType]->GetUV();

	return uv;
}

BOOL CUIMapOption::IsFlag( int nType, int npcIndex )
{
	CMobData* pData = CMobData::getData(npcIndex);

	// 상점, 길드, 친화도, 공간이동, 창고, 거래대행, 캐릭터 관리, 애완동물 조련사,
	// 신비한 석상, 스킬 마스터, 공성, 그외NPC

	switch((eNPC_TYPE)nType)
	{
	case eNT_SHOP:
		return pData->IsShopper();
	case eNT_STASH:
		return pData->IsWareHouse();
	case eNT_TELEPORT:
		return pData->IsZoneMoving();
	case eNT_GUILD:
		return pData->IsGuild();
	case eNT_AFFINITY:
		return pData->IsAffinityNPC();
	case eNT_AUCTION:
		return pData->IsAuctionChar();
	case eNT_CHAR_MANAGER:
		return pData->IsResetStat();
	case eNT_JOB_MASTER:
		return pData->IsSkillMaster();
	case eNT_WAR:
		return pData->IsWarCastle();
	case eNT_PET_MANAGER:
		{
			int idx[] = {253, 1032, 1191, 1201, 1256, 1257, 1258, 1260};
			for (int i = 0; i < 8; ++i)
			{
				if (npcIndex == idx[i])
					return TRUE;
			}
			return FALSE;
		}
	case eNT_EXPRESS:
		if (npcIndex == 1374)
			return TRUE;
		return FALSE;
	case eNT_ELSE_NPC:
		return TRUE;

	}
	return TRUE;
}

void CUIMapOption::Init()
{
	CDrawPort* pDraw = UIMGR()->GetDrawPort();

	if (pDraw == NULL)
		return;

	const char* strID[eNT_MAX] = {"quest_q", "quest_a", "shop", "stash", "teleport", "guild", "affinity",
		"auction", "char_manager", "job_master", "war", "pet_manager", "express", "else",
		"monster", "guild_group", "party", "fellowship"};

	CTString strTmp;

	CUIText* pTmpText = NULL;
	int		 nWidth = 0;

	for (int i = 0; i < eNT_MAX; ++i)
	{
		strTmp.PrintF("cb_%s", strID[i]);
		m_pCheckBtn[i] = (CUICheckButton*)findUI(strTmp.str_String);
		if (m_pCheckBtn[i] != NULL)
			m_pCheckBtn[i]->SetCheck(TRUE);

		strTmp.PrintF("img_%s", strID[i]);
		m_pImgIcon[i] = (CUIImage*)findUI(strTmp.str_String);

		// 사이즈 조절.
		strTmp.PrintF("text_%s", strID[i]);
		pTmpText = (CUIText*)findUI(strTmp.str_String);

		if (pTmpText == NULL)
			continue;

		strTmp = pTmpText->getText();

		if (g_iCountry != RUSSIA)
		{
			if (nWidth < pDraw->GetTextWidth2(strTmp))
				nWidth = pDraw->GetTextWidth2(strTmp);
		}
		else
		{
			extern CFontData *_pfdDefaultFont;

			int nNoFixedWidth = UTIL_HELP()->GetNoFixedWidth(_pfdDefaultFont, strTmp.str_String);
			if (nWidth < nNoFixedWidth)
				nWidth = nNoFixedWidth;
		}
	}

	int nGap = 20;
	SetWidth(m_pImgIcon[0]->GetWidth() + m_pCheckBtn[0]->GetWidth() + nWidth + nGap);

	if (CUIImageSplit* pSplit = (CUIImageSplit*)findUI("img_back"))
	{
		pSplit->SetWidth(GetWidth());
		pSplit->UpdateSplit();
	}

	SetPos(UIMGR()->GetRadar()->GetPosX() - m_nWidth, m_nPosY);
	updatePosition(true);

	m_bInit = true;
}

void CUIMapOption::Hide( BOOL bHide )
{
	if (m_bInit == false)
		Init();

	m_bHide = bHide;
}
