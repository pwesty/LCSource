#include "StdH.h"

#include <Engine/Interface/UIInternalClasses.h>
#include "SimplePlayerInfoUI.h"

#include <Engine/Contents/function/SimplePlayerMenuUI.h>
#include <Common/Packet/ptype_premium_char.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Help/Util_Help.h>

extern INDEX g_iCountry;

class CSimplePlayerInfoEPSkillFire	: public Command
{
public:
	void execute()
	{
		if (_pNetwork->MyCharacterInfo.ep >= 50000)
			_pNetwork->SendEnergySkillMessage();
	}
};

CSimplePlayerInfoUI::CSimplePlayerInfoUI()
	: m_pBtnEp(NULL)
	, m_pImgEpGage(NULL)
	, m_pImgSoulBack(NULL)
	, m_pListDungeon(NULL)
	, m_pImgDTPBack(NULL)
	, m_pAniDungeonTime(NULL)
	, m_pImgBaseBack(NULL)
	, m_pImgEPBack(NULL)
{
	setInherit(false);

	int		i;

	for (i = 0; i < eMAX; ++i)
	{
		m_pProgressBar[i] = NULL;
		m_pText[i] = NULL;
	}

	for (i = 0; i < 10; ++i)
		m_pImgSoulGage[i] = NULL;

	m_rcMouseEventArea.SetRect(0, 0, 279, 88);
}

CSimplePlayerInfoUI::~CSimplePlayerInfoUI()
{
	m_pBtnEp = NULL;
	m_pImgEpGage = NULL;
	m_pListDungeon = NULL;
	m_pImgDTPBack = NULL;
	m_pAniDungeonTime = NULL;
	m_pImgBaseBack = NULL;
	m_pImgEPBack = NULL;

	int		i;

	for (i = 0; i < eMAX; ++i)
	{
		m_pProgressBar[i] = NULL;
		m_pText[i] = NULL;
	}

	m_pImgSoulBack = NULL;

	for (i = 0; i < 10; ++i)
		m_pImgSoulGage[i] = NULL;
}

void CSimplePlayerInfoUI::initialize()
{
	Hide(FALSE);
	UIMGR()->RearrangeOrder(UI_SIMPLE_PLAYER_INFO, TRUE);

	m_pImgBaseBack = (CUIImageArray*)findUI("img_back");
	m_pImgSoulBack = (CUIImageArray*)findUI("img_ns_soul_gage_back");

	if (m_pImgEPBack = (CUIImageArray*)findUI("img_ep_back"))
		m_pImgEpGage = (CUIImage*)m_pImgEPBack->findUI("img_ep_gage");

	m_pProgressBar[eHP] = (CUIProgressBar*)findUI("pb_hp");
	m_pProgressBar[eMP] = (CUIProgressBar*)findUI("pb_mp");
	m_pProgressBar[eEXP] = (CUIProgressBar*)findUI("pb_exp");

	if (g_iCountry != RUSSIA)
	{
		CUIText* pText = NULL;
		if (pText = (CUIText*)findUI("text_hp"))
			pText->SetText(CTString("HP"));
		
		if (pText = (CUIText*)findUI("text_mp"))
			pText->SetText(CTString("MP"));

		if (pText = (CUIText*)findUI("text_exp"))
			pText->SetText(CTString("EXP"));
	}

	m_pText[eHP] = (CUIText*)findUI("text_hp_value");
	m_pText[eMP] = (CUIText*)findUI("text_mp_value");
	m_pText[eEXP] = (CUIText*)findUI("text_exp_value");

	if (m_pBtnEp = (CUIButton*)findUI("btn_monster_energy"))
	{
		CSimplePlayerInfoEPSkillFire* pCmd = new CSimplePlayerInfoEPSkillFire;
		m_pBtnEp->SetCommandUp(pCmd);
	}

	
	
	m_pAniDungeonTime = (CUISpriteAni*)findUI("img_dungeon_time");

	CTString strTemp;
	int		i;
	for (i = 0; i < 10; ++i)
	{
		strTemp.PrintF("img_soul_%d", i);
		m_pImgSoulGage[i] = (CUIImage*)findUI(strTemp);
	}

	if (m_pImgDTPBack = (CUIImageSplit*)findUI("img_dungeon_time_popup_back"))
		m_pListDungeon = (CUIList*)m_pImgDTPBack->findUI("list_dungeon");

	int nGageGap = 12;
	int nBtnGap = 3;

	int nGageH = 21;
	int nBtnW = 27;

	for (i = 0; i < eEP_MAX; ++i)
	{
		m_uvEPGage[i].SetUV(30, 80 + (i * (nGageGap + nGageH)), 196, 101 + (i * (nGageGap + nGageH)));
		m_uvEPBtn_Idle[i].SetUV(245 + (i * (nBtnGap + nBtnW)), 9, 272 + (i * (nBtnGap + nBtnW)), 37);
		m_uvEPBtn_Click[i].SetUV(245 + (i * (nBtnGap + nBtnW)), 44, 272 + (i * (nBtnGap + nBtnW)), 72);
	}

	StopDungeonTime();
}

void CSimplePlayerInfoUI::UpdatePlayerBaseInfo()
{
	UpdateHPMP();
	UpdateEXP();
	UpdateEP();
	UpdateSoul();

	SetPlayerName();
	SetPlayerLevel();
	SetPlayerFace();
}

void CSimplePlayerInfoUI::UpdateHPMP()
{
	if (m_pProgressBar[eHP] == NULL || m_pProgressBar[eMP] == NULL)
		return;

	if (m_pText[eHP] == NULL || m_pText[eMP] == NULL)
		return;

	int nHP = _pNetwork->MyCharacterInfo.hp;
	int nMaxHP = _pNetwork->MyCharacterInfo.maxHP;

	int nMP = _pNetwork->MyCharacterInfo.mp;
	int nMaxMP = _pNetwork->MyCharacterInfo.maxMP;

	CTString strTemp;

	m_pProgressBar[eHP]->SetProgressRange(0, nMaxHP);
	m_pProgressBar[eHP]->OffsetProgressPos(nHP);

	m_pProgressBar[eMP]->SetProgressRange(0, nMaxMP);
	m_pProgressBar[eMP]->OffsetProgressPos(nMP);

	strTemp.PrintF("%d/%d", nHP, nMaxHP);
	m_pText[eHP]->SetText(strTemp);

	strTemp.PrintF("%d/%d", nMP, nMaxMP);
	m_pText[eMP]->SetText(strTemp);
}

void CSimplePlayerInfoUI::UpdateEXP()
{
	if (m_pProgressBar[eEXP] == NULL)
		return;

	SQUAD sqExp = _pNetwork->MyCharacterInfo.curExp;
	SQUAD sqMaxExp = _pNetwork->MyCharacterInfo.needExp;

	m_pProgressBar[eEXP]->SetProgressRange(0, sqMaxExp);
	m_pProgressBar[eEXP]->OffsetProgressPos(sqExp);

	if (m_pText[eEXP] == NULL)
		return;

	CTString strTemp;

	strTemp.PrintF("%.2f%%", (float)((float)sqExp / (float)sqMaxExp) * 100.f);
	m_pText[eEXP]->SetText(strTemp);
}

void CSimplePlayerInfoUI::UpdateEP()
{
	if (m_pImgEpGage == NULL || m_pBtnEp == NULL)
		return;

	int nEpLv = 0;
	float fRatio = 0.f;
	int nCurEp = _pNetwork->MyCharacterInfo.ep;

	int nNeedEp[eEP_MAX] = {50000, 60000, 70000, 70000, 100000};
	int nCumulativeEp[eEP_MAX]	= {50000, 110000, 180000, 250000, 350000};

	for (int i = 0; i < eEP_MAX; ++i)
	{
		if (nCurEp >= nCumulativeEp[eEP_LV4])
		{
			nEpLv = eEP_LV4;
			fRatio = 1.f;
			break;
		}

		if (nCurEp > 0 && nCurEp < nCumulativeEp[i])
		{
			if (i == 0)
				fRatio = nCurEp / (float)nCumulativeEp[i];
			else
				fRatio = (nCurEp - nCumulativeEp[i - 1]) / (float)nNeedEp[i];

			nEpLv = i;
			break;
		}
	}

	UIRectUV uvTemp = m_uvEPGage[nEpLv];
	uvTemp.U1 = m_uvEPGage[nEpLv].U0 + ((m_uvEPGage[nEpLv].U1 - m_uvEPGage[nEpLv].U0) * fRatio);

	m_pImgEpGage->SetSize(uvTemp.U1 - uvTemp.U0, m_pImgEpGage->GetHeight());
	m_pImgEpGage->SetUV(uvTemp);

	if (nCurEp < nNeedEp[0])
		m_pBtnEp->SetEnable(FALSE);
	else
		m_pBtnEp->SetEnable(TRUE);

	if (nCurEp < nCumulativeEp[eEP_LV4])
	{
		if (nEpLv > 0 && nEpLv < eEP_MAX)
			nEpLv -= 1;
	}

	m_pBtnEp->SetUV(UBS_IDLE, m_uvEPBtn_Idle[nEpLv]);
	m_pBtnEp->SetUV(UBS_ON, m_uvEPBtn_Idle[nEpLv]);
	m_pBtnEp->SetUV(UBS_CLICK, m_uvEPBtn_Click[nEpLv]);
}

void CSimplePlayerInfoUI::UpdateSoul()
{
	if (m_pImgSoulBack == NULL)
		return;

	if (_pNetwork->MyCharacterInfo.job != NIGHTSHADOW)
	{
		m_pImgSoulBack->Hide(TRUE);
		return;
	}

	m_pImgSoulBack->Hide(FALSE);
	
	for (int i = 0; i < 10; ++i)
	{
		if (i < _pNetwork->MyCharacterInfo.sbSoulCount)
			m_pImgSoulGage[i]->Hide(FALSE);
		else
			m_pImgSoulGage[i]->Hide(TRUE);
	}
}

void CSimplePlayerInfoUI::SetPlayerName()
{
	CUIText* pText = (CUIText*)findUI("text_player_name");

	if (pText == NULL)
		return;

	pText->SetText(_pNetwork->MyCharacterInfo.name);
}

void CSimplePlayerInfoUI::SetPlayerLevel()
{
	CUIText* pText = (CUIText*)findUI("text_lv");

	if (pText == NULL)
		return;

	CTString strTemp;

	strTemp.PrintF("%d", _pNetwork->MyCharacterInfo.level);
	pText->SetText(strTemp);
}

void CSimplePlayerInfoUI::SetPlayerFace()
{
	CUIImageArray* pImage = (CUIImageArray*)findUI("img_char_face");

	if (pImage == NULL)
		return;

	int nJob = _pNetwork->MyCharacterInfo.job;

	if (nJob < 0 || nJob > TOTAL_JOB)
		nJob = TOTAL_JOB;

	pImage->SetRenderIdx(nJob);
}

void CSimplePlayerInfoUI::StopDungeonTime()
{
	if (m_pAniDungeonTime == NULL)
		return;

	m_pAniDungeonTime->StopAni();
	m_pAniDungeonTime->Hide(TRUE);
}

void CSimplePlayerInfoUI::StartDungeonTime()
{
	if (m_pAniDungeonTime == NULL)
		return;

	m_pAniDungeonTime->SetDelayTime(300);
	m_pAniDungeonTime->PlayAni();
	m_pAniDungeonTime->Hide(FALSE);
}

void CSimplePlayerInfoUI::SetDungeonList(SLONG slExp)
{
	if (m_pImgDTPBack == NULL || m_pListDungeon == NULL)
		return;

	int nMax = m_pListDungeon->getListItemCount();

	CUIListItem* pItem = NULL;
	CUIText* pText = NULL;

	int nStrIndex[14] = { 343/*1 벨피스트 신전*/, 345/*3 프로키온 신전*/, 1201/*9 마르가둠*/, 2368/*12 망각의 신전*/, 4946/*21 테오스의 무덤*/,
		4947/*17, 18, 19, 20 루스트 투룸*/, 5146/*37 트리비아 계곡*/, 4319/*29 에보니탄광*/, 4317/*30 미스티협곡*/, 4395/*31 플로라임 동굴*/,
		4509/*34 예배당*/, 4510/*35 암흑의 제단*/, 5065/*36 아칸사원*/, 5817/*41 람슬로 지하 연구실*/ 
	};
	int nMaxWidth = 0;

	CDrawPort* pDrawPort = UIMGR()->GetDrawPort();

	for (int i = 0; i < nMax; ++i)
	{
		pItem = (CUIListItem*)m_pListDungeon->GetListItem(i);
		
		if (pItem == NULL)
			continue;

		pText = (CUIText*)pItem->findUI("text_dungeon_name");

		if (pText == NULL)
			continue;

		if (i == 0)
		{
			CTString strTemp;

			if (slExp > 0)
				strTemp.PrintF(CTString("%s(%d%%)"), _S(4959, "던전 몬스터 경험치를 추가로 획득"), slExp);
			else
				strTemp = _S(4959, "던전 몬스터 경험치를 추가로 획득");

			pText->SetText(strTemp);
			pText->setFontColor(0xE18600FF);
		}
		else if (i == 1)
		{
			pText->SetText(_S(4960, "던전 타임!"));
			pText->setFontColor(0xA3A1A3FF);
		}
		else
		{
			pText->SetText(_S(nStrIndex[i-2], "던전 이름"));
			pText->setFontColor(DEF_UI_COLOR_WHITE);
		}

		if (pDrawPort != NULL)
		{
			if (g_iCountry != RUSSIA)
			{
				int nCurWidth = pDrawPort->GetTextWidth2(pText->getText());

				if (nMaxWidth < nCurWidth)
					nMaxWidth = nCurWidth;
			}
			else
			{
				extern CFontData *_pfdDefaultFont;

				int nCurWidth = UTIL_HELP()->GetNoFixedWidth(_pfdDefaultFont, (char*)pText->getText());

				if (nMaxWidth < nCurWidth)
					nMaxWidth = nCurWidth;
			}
		}
	}

	// 14는 갭.
	m_pImgDTPBack->SetWidth(nMaxWidth + 14);
	m_pImgDTPBack->UpdateSplit();

	m_pListDungeon->UpdateList();
}

WMSG_RESULT CSimplePlayerInfoUI::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (m_pAniDungeonTime != NULL && m_pAniDungeonTime->IsInside(x, y) == TRUE)
	{
		if (m_pImgDTPBack != NULL)
		{
			m_pImgDTPBack->Hide(FALSE);
			m_pImgDTPBack->SetPos(x, y);

			UIMGR()->RearrangeOrder(UI_SIMPLE_PLAYER_INFO, TRUE);
		}
	}
	else
	{
		if (m_pImgDTPBack != NULL)
			m_pImgDTPBack->Hide(TRUE);
	}

	return WMSG_FAIL;
}

WMSG_RESULT CSimplePlayerInfoUI::OnRButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInsideRect(x, y, m_rcMouseEventArea) == TRUE)
	{
		UIMGR()->GetSimplePlayerMenuUI()->SetMainMenu();
		UIMGR()->GetSimplePlayerMenuUI()->SetPos(x, y);
	}

	return WMSG_FAIL;
}

WMSG_RESULT CSimplePlayerInfoUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInsideRect(x, y, m_rcMouseEventArea) == TRUE)
	{
		CEntity			*penPlEntity;
		CPlayerEntity	*penPlayerEntity;
		penPlEntity = CEntity::GetPlayerEntity( 0 );
		penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);
		if (penPlayerEntity != NULL)
			penPlayerEntity->SetTargetMe();
	}

	return WMSG_FAIL;
}

void CSimplePlayerInfoUI::SetPremiumChar( char szType )
{
	if (m_pImgBaseBack == NULL || m_pImgSoulBack == NULL || m_pImgEPBack == NULL)
		return;

	if (szType > PREMIUM_CHAR_TYPE_FIRST)
		szType = PREMIUM_CHAR_TYPE_FIRST;

	m_pImgBaseBack->SetRenderIdx(szType);
	m_pImgSoulBack->SetRenderIdx(szType);
	m_pImgEPBack->SetRenderIdx(szType);
}
