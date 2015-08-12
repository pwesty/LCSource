#include "StdH.h"
#include <Engine/Network/CNetwork.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UISkillToolTip.h>
#include <Engine/Entities/SkillTree.h>
#include <Engine/Contents/Base/UICharacterInfoNew.h>
#include <Engine/Interface/UIQuickSlot.h>
#include "UISkillNew.h"
#include <Engine/Info/MyInfo.h>

#define DEF_SUB_BACK_GAPX	(21)
#define DEF_SUB_BACK_GAPY	(116)
#define DEF_SUB_BACK_UV_R	(381)
#define DEF_SUB_BACK_UV_B	(475)
#define DEF_SUB_BACK_SY		(668)

#define DEF_EARTH_SUMMON	(293)
#define DEF_FIRE_SUMMON		(294)
#define DEF_WATER_SUMMON	(300)
#define DEF_WIND_SUMMON		(301)
#define DEF_VITAL_CONVERSION (307)

#define DEF_EARTH_REINFORCEMENT		(297)
#define DEF_FIRE_REINFORCEMENT		(298)
#define DEF_WATER_REINFORCEMENT		(302)
#define DEF_WIND_REINFORCEMENT		(303)

// --------------------------------------------------
#define DEF_SATISFIED_EFFECT_COUNT		(-1)
#define DEF_SATISFIED_EFFECT_ELAPSED	(1000)

#define DEF_SKILL_LVUP_EFFECT_COUNT		(1)
#define DEF_SKILL_LVUP_EFFECT_ELAPSED	(150)


class CmdCloseBtn : public Command
{
public:
	CmdCloseBtn() : m_pWnd(NULL) {}
	void setData(CUISkillNew* pWnd) { m_pWnd = pWnd; }
	void execute()
	{
		if( m_pWnd )
			m_pWnd->CloseUI();
	}
private:
	CUISkillNew* m_pWnd;
};

class CmdSkillLearn : public Command
{
public :
	CmdSkillLearn() : m_pWnd(NULL), m_slSkillIndex(-1), m_nCurTab(0), m_nCurPos(0), m_nNpcVirIdx(-1) {}
	void setData(CUISkillNew* pWnd, SLONG idx, int nCurTab, int nCurPos, int nNpcIdx) 
	{ 
		m_pWnd = pWnd;
		m_slSkillIndex = idx;
		m_nCurTab	= nCurTab;
		m_nCurPos	= nCurPos;
		m_nNpcVirIdx = nNpcIdx;
	}
	void execute()
	{
		if( m_pWnd != NULL && m_slSkillIndex > 0 )
		{
			_pNetwork->SendSkillLearn(m_slSkillIndex, m_nNpcVirIdx);
			m_pWnd->SetLvUpItem(m_nCurTab, m_nCurPos, m_slSkillIndex);
		}
	}
private:
	SLONG		  m_slSkillIndex;
	CUISkillNew*  m_pWnd;
	int			  m_nCurTab;
	int			  m_nCurPos;
	int			  m_nNpcVirIdx;
};

class CmdSkillFire : public Command
{
public : 
	CmdSkillFire() : m_slSkillIndex(-1) {}
	void setData(SLONG idx) { m_slSkillIndex = idx; }
	void execute()
	{
		if( m_slSkillIndex > 0 )
			CUIManager::getSingleton()->GetCharacterInfo()->UseSkill(m_slSkillIndex);
	}
private:
	SLONG m_slSkillIndex;
};

CUISkillNew::CUISkillNew()
	: m_pTab(NULL)
	, m_pDragArea(NULL)
	, m_bSkillLearnMode(false)
	, m_nOriX(0)
	, m_nOriY(0)
	, m_bDrag(false)
	, m_ulSkillLVUpEffectStartTime(0)
	, m_bSkillLevelupEffect(false)
	, m_nCurTab(0)
	, m_nCurPos(0)
	, m_nMouseX(0)
	, m_nMouseY(0)
	, m_ulSatisfiedEffectStart(0.0f)
	, m_bSatisfiedEffect(true)
	, m_nSatisfiedEffectCount(DEF_SATISFIED_EFFECT_COUNT)
	, m_nSkillLevelupEffectCount(DEF_SKILL_LVUP_EFFECT_COUNT)
	, m_nNpcVirIdx(-1)
{
	setInherit(false);

	int i;
	for( i = 0; i < eTAB_MAX; ++i )
		m_pArray[i] = NULL;

	m_ulSkillLVUpEffectElapsTime = DEF_SKILL_LVUP_EFFECT_ELAPSED;
	m_ulSatisfiedEffectElaps = DEF_SATISFIED_EFFECT_ELAPSED;

	m_nSatisfiedEffectTempCount = m_nSatisfiedEffectCount;
	m_nSkillLVUpEffectTempCount = m_nSkillLevelupEffectCount;
}

void CUISkillNew::initialize()
{
	CUIButton* btnClose = (CUIButton*)findUI("btn_close");
	if( btnClose )
	{
		CmdCloseBtn* pCmd = new CmdCloseBtn;
		pCmd->setData(this);
		btnClose->SetCommandUp(pCmd);
	}

	m_pTab = (CUITab*)findUI("tab_skill");

	m_pDragArea = (CUIText*)findUI("text_drag_area");

	InitArrayData();

	UIRectUV uv;

	uv.SetUV(1269, 117, 1284, 139);
	m_rsArrow.AddRectSurface(UIRect(0, 0, 15, 22), uv);	// 화살표 시작.

	uv.SetUV(1269, 121, 1284, 141);
	m_rsArrow.AddRectSurface(UIRect(0, 0, 15, 81), uv);	// 화살표 중간.

	uv.SetUV(1269, 121, 1284, 141);
	m_rsArrow.AddRectSurface(UIRect(0, 0, 15, 69), uv);	// 화살표 중간 ( 끝에 화살촉이 붙을수 있는 중간 ).

	uv.SetUV(1218, 141, 1233, 162);
	m_rsArrow.AddRectSurface(UIRect(0, 0, 15, 20), uv); // 완전한 화살표

	uv.SetUV(1317, 117, 1332, 139);
	m_rsArrowBlend.AddRectSurface(UIRect(0, 0, 15, 22), uv);	// 화살표 시작.

	uv.SetUV(1317, 121, 1332, 141);
	m_rsArrowBlend.AddRectSurface(UIRect(0, 0, 15, 81), uv);	// 화살표 중간.

	uv.SetUV(1317, 121, 1332, 141);
	m_rsArrowBlend.AddRectSurface(UIRect(0, 0, 15, 69), uv);	// 화살표 중간 ( 끝에 화살촉이 붙을수 있는 중간 ).

	uv.SetUV(1292, 141, 1307, 162);
	m_rsArrowBlend.AddRectSurface(UIRect(0, 0, 15, 20), uv);	// 완전한 화살표

	//------------------------------------------------------------------------------------------------------------<<
	uv.SetUV(1481, 43, 1525, 101);
	m_rsSkillLvUpEffect.AddRectSurface(UIRect(0, 0, 44, 58), uv);	// 액티브 스킬 레벨업 이펙트.

	uv.SetUV(1532, 43, 1575, 101);
	m_rsSkillLvUpEffect.AddRectSurface(UIRect(0, 0, 44, 58), uv);	// 패시브 스킬 레벨업 이펙트.
}


void CUISkillNew::InitArrayData()
{
	Reset();

	m_pArray[eTAB_NORMAL] = (CUIArray*)findUI("array_class_0");
	m_pArray[eTAB_JOB2ND_01] = (CUIArray*)findUI("array_class_1");
	m_pArray[eTAB_JOB2ND_02] = (CUIArray*)findUI("array_class_2");

	CUIArrayItem* pTmpItem;
	CUIIcon* pIcon;
	CUIText* pText;

	int state, max = 0;
	int i;
	for( state = 0; state < eTAB_MAX; ++state )
	{
		if( m_pArray[state] == NULL )
			continue;

		max = m_pArray[state]->GetArrayChildCount();

		for( i = 0; i < max; ++i )
		{
			pTmpItem = (CUIArrayItem*)m_pArray[state]->getChildAt(i);
			
			if( pTmpItem == NULL )
				continue;

			pIcon = (CUIIcon*)pTmpItem->findUI("icon_skill");

			if( pIcon != NULL )
				pIcon->clearIconData();

			pText = (CUIText*)pTmpItem->findUI("text_skill_level");

			if( pText != NULL )
				pText->SetText(CTString("0/0"));

			SetChildVisible(pTmpItem, false);
		}
	}

	for	(i = 0; i < eTAB_MAX; ++i)
	{
		if( m_pArray[i] != NULL )
			m_pArray[i]->ResetArray();
	}
}

void CUISkillNew::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}
void CUISkillNew::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CUISkillNew::OpenUI()
{
	if(IsVisible() == TRUE || GetHide() == FALSE )
	{
		CloseUI();
		return;
	}

	SetVisible(TRUE);
	Hide(FALSE);

	UpdateSkillInfo();
	SetRightBackGround();
	SetTabString();
	UpdateSPointAndMoney();

	CUIManager::getSingleton()->RearrangeOrder(UI_SKILL_NEW, TRUE);

	m_bSatisfiedEffect = true;
}

void CUISkillNew::OpenUI( int iMobIndex, int iMobVirIdx, BOOL bHasQuest, FLOAT fX, FLOAT fZ )
{
	if(IsVisible() == TRUE || GetHide() == FALSE )
	{
		//Reset();
		m_bSkillLearnMode = true;
		UpdateSkillData();
		return;
	}

	m_bSkillLearnMode = true;
	m_nNpcVirIdx = iMobVirIdx;
	OpenUI();
}

void CUISkillNew::CloseUI()
{
	Hide(TRUE);
	SetVisible(FALSE);

	Reset();
	CUIManager::getSingleton()->RearrangeOrder(UI_SKILL_NEW, FALSE);
}

void CUISkillNew::Reset()
{
	m_bSkillLearnMode = false;
// 	InitArrayData();

	if (m_pTab != NULL)
		m_pTab->ResetTabPos();
	
	m_vecSatisfiedSkill.clear();

	if (CUITabPage* pPage = (CUITabPage*)findUI("page_class_0"))
	{
		if (CUIImage* pImg = (CUIImage*)pPage->findUI("img_levelup_effect"))
		{
			pImg->Hide(TRUE);
		}
	}

	m_bSkillLevelupEffect = false;
	m_nSkillLVUpEffectTempCount = m_nSkillLevelupEffectCount;

	m_bSatisfiedEffect = false;
	m_nSatisfiedEffectTempCount = m_nSatisfiedEffectCount;

	m_nNpcVirIdx = -1;
}

void CUISkillNew::UpdateSkillInfo()
{
	{
		m_vecSatisfiedSkill.clear();
		m_nSatisfiedEffectTempCount = m_nSatisfiedEffectCount;
	}

	if( m_pTab == NULL )
		return;
	
	CUIArrayItem* pArrayItem;
	CUIArrayItem* pTmpArrayItem;

	int i;
	int Skillidx;
	int job = _pNetwork->MyCharacterInfo.job;
	int nMyChar2ndJob = _pNetwork->MyCharacterInfo.job2;

	int dataMax = 0;
	int arrayItemCount = 0;

	int	setting_count = 1;

	if (nMyChar2ndJob == 0)
		setting_count = 3;

	for (int sj = 0; sj < setting_count; ++sj)
	{
		dataMax = CSkillTree::m_nSkillCount[job][sj];	// 기본 직업 카운트.

		if (nMyChar2ndJob > 0)
			dataMax += CSkillTree::m_nSkillCount[job][nMyChar2ndJob];
		
		if( m_pArray[sj] == NULL )
			continue;

		pArrayItem = m_pArray[sj]->GetArrayItemTemplate();

		for( i = 0; i < dataMax; ++i )
		{
			arrayItemCount = m_pArray[sj]->GetArrayChildCount();

			if( i >= arrayItemCount )
				m_pArray[sj]->AddArrayItem((CUIArrayItem*)pArrayItem->Clone());

			pTmpArrayItem = m_pArray[sj]->GetArrayItem(i);

			if( pTmpArrayItem == NULL )
				continue;

			Skillidx = GetSkillIndex(job, sj, i);

			if (IsSummonSkill(Skillidx) == true || Skillidx <= 0)
			{
				SetChildVisible( pTmpArrayItem, false);
				continue;
			}
			else if (Skillidx > _pNetwork->ga_World.wo_aSkillData.Count() )
			{
				LOG_DEBUG("스킬 인덱스가 저장된 값보다 큽니다.");
				continue;
			}

			pTmpArrayItem->Hide(FALSE);
			SetSkillTree(pTmpArrayItem, Skillidx, sj, i);

			CheckArrow(Skillidx, sj, i);
		}
		m_pArray[sj]->UpdateItem();
	}
}

void CUISkillNew::SetSkillTree( CUIArrayItem* pItem, int idx, int nTab, int nPos )
{
	if( pItem == NULL || idx <= 0 )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	CUIImage*  pBaseImgPassive;
	CUIImage*  pBaseImgActive;
	CUIButton* pBtn;
	CUIIcon*   pIcon;
	CUIImage*  pImg;
	CUIText*   pText;

	CSkill		 skill;

	skill = _pNetwork->GetSkillData(idx);

	pBtn = (CUIButton*)pItem->findUI("btn_plus");
	pIcon = (CUIIcon*)pItem->findUI("icon_skill");
	pText = (CUIText*)pItem->findUI("text_skill_level");
	pBaseImgPassive = (CUIImage*)pItem->findUI("img_base_passive");
	pBaseImgActive = (CUIImage*)pItem->findUI("img_base_active");

	if (pImg = (CUIImage*)pItem->findUI("img_skill_passive"))
		pImg->Hide(TRUE);

	if (pImg = (CUIImage*)pItem->findUI("img_skill_active"))
		pImg->Hide(TRUE);

	if ( skill.GetType() == CSkill::ST_PASSIVE )
	{
		pBaseImgPassive->Hide(FALSE);
		pBaseImgActive->Hide(TRUE);
	}
	else
	{
		pBaseImgPassive->Hide(TRUE);
		pBaseImgActive->Hide(FALSE);
	}

	SetIconData(pIcon, idx, nTab);
	SetButtonData(pBtn, idx, nTab, nPos);
	SetTextData(pText, idx, nTab);
	SetImageData(idx, nTab, nPos);
}

void CUISkillNew::SetIconData( CUIIcon* pIcon, int nSkillIndex, int nTab )
{
	if( pIcon == NULL )
		return;

	pIcon->Hide(FALSE);
	pIcon->clearIconData();
	
	CUIManager* pUIManager = CUIManager::getSingleton();

	int		maxLv		= 0;
	bool	bPassive	= false;
	SBYTE	sbSkillLv	= 0;
	int		nJob		= 0;	

	CSkill& skill = _pNetwork->GetSkillData(nSkillIndex);
	sbSkillLv = MY_INFO()->GetSkillLevel(nSkillIndex);
	maxLv	  = skill.GetMaxLevel();
	nJob	  = _pNetwork->MyCharacterInfo.job;

	if( skill.GetType() == CSkill::ST_PASSIVE )
		bPassive = true;

	int satisfied = 0;
	
	if( sbSkillLv != maxLv )
		 satisfied= CUISkillToolTip::getSingleton()->IsSatisfiedSkill(nSkillIndex, sbSkillLv + 1, FALSE, IsReinforcementSkill(nSkillIndex));

	
	bool	bDisable;
	
	if (sbSkillLv == maxLv)
		bDisable = false;
	else if (nTab > 0)
		bDisable = true;
	else
	{
		if (m_bSkillLearnMode == true)
		{
			if (satisfied == 0)	// 스킬을 배울수 있는 상태.
				bDisable = false;
			else
				bDisable = true;
		}
		else
		{
			if (sbSkillLv > 0)
				bDisable = false;
			else
				bDisable = true;
		}
	}

	pIcon->setSkill(nSkillIndex, false, bDisable);
	pIcon->SetWhichUI(UI_SKILL_NEW);

	// 스킬 배우기 모드가 아님.
	if (m_bSkillLearnMode == false)
	{
		// 액티브 스킬이며, 스킬 레벨이 1이상이라면 커맨드 등록.
		if (bPassive == false && sbSkillLv > 0 )
		{
			if (CmdSkillFire* pCmd = new CmdSkillFire)
			{
				pCmd->setData(nSkillIndex);
				pIcon->SetCommandDBL(pCmd);
			}

			{
				CmdDragIcon* pCmd = new CmdDragIcon;
				pCmd->setData(pIcon);
				pIcon->SetCommand(pCmd);
			}

			pIcon->SetToggle(skill.GetToggle());
		}
	}
}

void CUISkillNew::SetTextData( CUIText* pText, int nSkillIndex, int nTab )
{
	if (pText == NULL)
		return;
	
	pText->Hide(FALSE);

	CUIManager* pUIManager = CUIManager::getSingleton();

	int		maxLv			= 0;
	SBYTE	sbSkillLv		= 0;
	CSkill	skill;

	skill	  = _pNetwork->GetSkillData(nSkillIndex);
	sbSkillLv = MY_INFO()->GetSkillLevel(nSkillIndex);
	maxLv	  = skill.GetMaxLevel();

	COLOR colText = DEF_UI_COLOR_WHITE;

	int satisfied = 0;

	if( sbSkillLv != maxLv )
		satisfied= CUISkillToolTip::getSingleton()->IsSatisfiedSkill(nSkillIndex, sbSkillLv + 1, FALSE, IsReinforcementSkill(nSkillIndex));
	else
		satisfied= CUISkillToolTip::getSingleton()->IsSatisfiedSkill(nSkillIndex, sbSkillLv, FALSE, IsReinforcementSkill(nSkillIndex));

	if (sbSkillLv == 0 || nTab > 0)	// 스킬레벨이 0이라면 안보여줌.
	{
		pText->Hide(TRUE);
		return;
	}
	else
	{
		if (m_bSkillLearnMode == true)
		{
			if (maxLv != sbSkillLv)
			{
				if (satisfied != 0)	// 스킬을 배울수 없는 상태.
					colText = 0x808080FF;
			}
			else
				colText = 0x00FF21FF;
		}
	}

	CTString strTmp;

	pText->setFontColor(colText);

	strTmp.PrintF("%d/%d", sbSkillLv, maxLv);
	pText->SetText(strTmp);

}

void CUISkillNew::SetButtonData( CUIButton* pBtn, int nSkillIndex, int nTab, int nPos )
{
	if ( m_bSkillLearnMode == false || pBtn == NULL)
	{
		pBtn->Hide(TRUE);
		return;
	}

	pBtn->SetEnable(FALSE);
	pBtn->Hide(FALSE);

	int nJob	= _pNetwork->MyCharacterInfo.job;

	CUIManager* pUIManager = CUIManager::getSingleton();

	int		maxLv			= 0;
	SBYTE	sbSkillLv		= 0;

	CSkill		 skill;

	skill	  = _pNetwork->GetSkillData(nSkillIndex);
	sbSkillLv = MY_INFO()->GetSkillLevel(nSkillIndex);
	maxLv	  = skill.GetMaxLevel();

	if (nTab > 0  )	// 0번탭은 전직안한 클레스
	{
		pBtn->Hide(TRUE);
		return;
	}

	if (sbSkillLv == maxLv)
	{
		pBtn->Hide(TRUE);
		return;
	}

	int satisfied = 0;

	if( sbSkillLv != maxLv )
		satisfied= CUISkillToolTip::getSingleton()->IsSatisfiedSkill(nSkillIndex, sbSkillLv + 1, FALSE, IsReinforcementSkill(nSkillIndex));
	else
		satisfied= CUISkillToolTip::getSingleton()->IsSatisfiedSkill(nSkillIndex, sbSkillLv, FALSE, IsReinforcementSkill(nSkillIndex));

	if (satisfied > 0)
	{
		pBtn->SetEnable(FALSE);
	}
	else if (satisfied == 0)
	{
		pBtn->SetEnable(TRUE);

		if (CmdSkillLearn* pCmd = new CmdSkillLearn)
		{
			if (m_pArray[nTab] != NULL)
			{
				pCmd->setData(this, nSkillIndex, nTab, nPos, m_nNpcVirIdx);
				pBtn->SetCommandUp(pCmd);
			}
		}
	}
}


void CUISkillNew::SetImageData( int nSkillIndex, int nTab, int nPos )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	int		maxLv		= 0;
	SBYTE	sbSkillLv	= 0;
	bool	bPassive	= false;

	CSkill& skill  = _pNetwork->GetSkillData(nSkillIndex);
	sbSkillLv = MY_INFO()->GetSkillLevel(nSkillIndex);
	maxLv	  = skill.GetMaxLevel();

	if( skill.GetType() == CSkill::ST_PASSIVE )
		bPassive = true;
		
	if (nTab > 0)
		return;

	int satisfied = 0;

	if ( sbSkillLv != maxLv)
		satisfied = CUISkillToolTip::getSingleton()->IsSatisfiedSkill(nSkillIndex, sbSkillLv + 1, FALSE, IsReinforcementSkill(nSkillIndex));
	
	stSatisfiedSkill satisfiedSkill;

	satisfiedSkill.nPos = nPos;
	satisfiedSkill.bPassive = bPassive;

	if (satisfied != 0)
		satisfiedSkill.bShow = false;
	else
		satisfiedSkill.bShow = true;

	if (m_bSkillLearnMode == true || sbSkillLv == maxLv)
		satisfiedSkill.bShow = false;

	m_vecSatisfiedSkill.push_back(satisfiedSkill);
}


void CUISkillNew::UpdateSkillData(bool bReset /*= false*/)
{
	UpdateSkillInfo();
	UpdateSPointAndMoney();

	if (bReset == true)
	{
		SetRightBackGround();
		SetTabString();
		m_bSatisfiedEffect = true;
	}
}

void CUISkillNew::UpdateSPointAndMoney(int sp /* = -1 */)
{
	CUIText* pText = NULL;

	if ( pText = (CUIText*)findUI("text_skill_point") )
	{
		CTString strTmp;
		COLOR	 colNas = CUIManager::getSingleton()->GetNasColor(_pNetwork->MyCharacterInfo.sp);

		if (sp > 0)
			strTmp.PrintF("%d", sp / 10000);
		else
			strTmp.PrintF("%d", _pNetwork->MyCharacterInfo.sp / 10000);

		CUIManager::getSingleton()->InsertCommaToString( strTmp );

		int nSp = CalcLearnSkillPoint();

		CTString strTmp2;
		strTmp2.PrintF("%d", nSp);

		CUIManager::getSingleton()->InsertCommaToString( strTmp2 );

		strTmp.PrintF("%s / %s", strTmp, strTmp2);

		pText->SetText(strTmp);
		pText->setFontColor(colNas);
	}

	if ( pText = (CUIText*)findUI("text_money") )
	{
		CTString strTmp;
		COLOR	 colNas = CUIManager::getSingleton()->GetNasColor(_pNetwork->MyCharacterInfo.money);

		strTmp.PrintF( "%I64d", _pNetwork->MyCharacterInfo.money );
		CUIManager::getSingleton()->InsertCommaToString( strTmp );

		pText->SetText(strTmp);
		pText->setFontColor(colNas);
	}
}

bool CUISkillNew::CheckArrow( int SKillIndex, int _2nd_job, int num )
{
	if (IsReinforcementSkill(SKillIndex) == TRUE)
		return false;

	int		nLearnSkillIndex[3];
	int		nParentPos;
	SBYTE   sbParentSkillLv = 0;
	CUIImage* pImg_t, *pImg_b;
	CSkill& rSkill = _pNetwork->GetSkillData(SKillIndex);
	UIRectUV uv1, uv2;
	UIRect	 rc1, rc2;

	int		nSj = _pNetwork->MyCharacterInfo.job2;
	int		nJob = _pNetwork->MyCharacterInfo.job;

	CUIManager* pUIManager = CUIManager::getSingleton();

	// ???
	for( int i = 0; i < 3; i++ )
	{
		nLearnSkillIndex[i] = rSkill.GetLearnSkillIndex( 0, i );
	}

	if (nLearnSkillIndex[0] > 0)
	{
		nParentPos = CheckCols( nLearnSkillIndex[0], _2nd_job, num );

		sbParentSkillLv = MY_INFO()->GetSkillLevel(nLearnSkillIndex[0]);

		if( nParentPos < 0 )
			return false;

		int	 arrowPos = nParentPos;
		bool bStart = true;

		while (arrowPos < num)
		{
			// Setting!!
			CUIArrayItem* pItem = NULL;

			if (m_pArray[_2nd_job] != NULL && nSj == 0 && CSkillTree::m_nSkillCount[nJob][0] > num)
				pItem = m_pArray[_2nd_job]->GetArrayItem(arrowPos);
			else if (m_pArray[0] != NULL && nSj > 0)
				pItem = m_pArray[0]->GetArrayItem(arrowPos);
			else
				pItem = m_pArray[_2nd_job]->GetArrayItem(arrowPos);

			if( pItem == NULL )
				return false;

			pImg_t = (CUIImage*)pItem->findUI("img_arrow_top");
			pImg_b = (CUIImage*)pItem->findUI("img_arrow_bot");
			
			if( pImg_t == NULL || pImg_b == NULL )
				return false;

			pImg_t->Hide(TRUE);
			pImg_b->Hide(TRUE);

			if (bStart == true)
			{
				// 화살표 시작!!
				bStart = false;
				
				if (sbParentSkillLv > 0)
				{
					if ( arrowPos + DEF_SKILL_COL == num )
						m_rsArrow.GetRectAndUV(rc1, uv1, eARROW_PERFECT);
					else
						m_rsArrow.GetRectAndUV(rc1, uv1, eARROW_START);
				}
				else
				{
					if ( arrowPos + DEF_SKILL_COL == num )
						m_rsArrowBlend.GetRectAndUV(rc1, uv1, eARROW_PERFECT);
					else
						m_rsArrowBlend.GetRectAndUV(rc1, uv1, eARROW_START);
				}

				pImg_b->SetUV(uv1);
				pImg_b->SetWidth(rc1.Right);
				pImg_b->SetHeight(rc1.Bottom);

				pImg_b->Hide(FALSE);
			}
			else if ((arrowPos + DEF_SKILL_COL) == num)
			{
				// 화살표 끝
				if (sbParentSkillLv > 0)
				{
					m_rsArrow.GetRectAndUV(rc1, uv1, eARROW_MID_BAR_2);
					m_rsArrow.GetRectAndUV(rc2, uv2, eARROW_PERFECT);
				}
				else
				{
					m_rsArrowBlend.GetRectAndUV(rc1, uv1, eARROW_MID_BAR_2);
					m_rsArrowBlend.GetRectAndUV(rc2, uv2, eARROW_PERFECT);
				}

				pImg_t->SetUV(uv1);
				pImg_t->SetWidth(rc1.Right);
				pImg_t->SetHeight(rc1.Bottom);

				pImg_b->SetUV(uv2);
				pImg_b->SetWidth(rc2.Right);
				pImg_b->SetHeight(rc2.Bottom);

				pImg_t->Hide(FALSE);
				pImg_b->Hide(FALSE);
			}
			else
			{
				// 화살표 중간

				if (sbParentSkillLv > 0)
					m_rsArrow.GetRectAndUV(rc1, uv1, eARROW_MID_BAR);
				else
					m_rsArrowBlend.GetRectAndUV(rc1, uv1, eARROW_MID_BAR);

				pImg_t->SetUV(uv1);
				pImg_t->SetWidth(rc1.Right);
				pImg_t->SetHeight(rc1.Bottom);

				pImg_t->Hide(FALSE);
			}
			arrowPos += DEF_SKILL_COL;
		}
	}
	return true;
}

int CUISkillNew::CheckCols( int nLearnSkill, int _2nd_job, int pos )
{
	if ( IsReinforcementSkill(nLearnSkill) == TRUE )
		return -1;

	int job = _pNetwork->MyCharacterInfo.job;
	int nSj = _pNetwork->MyCharacterInfo.job2;

	int parentPos = pos;

	while (parentPos > 0)
	{
		parentPos -= DEF_SKILL_COL;

		int SkillIdx = GetSkillIndex(job, _2nd_job, parentPos);

		if (nLearnSkill == SkillIdx)
		{
			// find!!
			return parentPos;
		}
	}

	// 
	LOG_DEBUG( "아무것도 없으면 에런디..." );

	return -1;
}

int CUISkillNew::GetSkillIndex( int job, int job2, int num )
{
	int idx = num;

	if ( num >= CSkillTree::m_nSkillCount[job][job2] )
	{
		idx -= CSkillTree::m_nSkillCount[job][0];
		job2 = _pNetwork->MyCharacterInfo.job2;
	}

	return CSkillTree::m_SkillTree[job][job2][idx/8].index[idx%8];
}

void CUISkillNew::SetChildVisible( CUIArrayItem* pItem, bool bVisible )
{
	if( pItem == NULL )
		return;

	int i, max = pItem->getChildCount();

	for( i = 0; i < max; ++i )
	{
		if ( CUIBase* pBase = (CUIBase*)pItem->getChildAt(i) )
			pBase->Hide(bVisible ? FALSE : TRUE);
	}
}

void CUISkillNew::LearnSkillMessage( SLONG slIndex, SBYTE sbIsNew, SBYTE sbLevel )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	CSkill		&rSkill = _pNetwork->GetSkillData( slIndex );

	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 270, "스킬" ), UMBS_OK, UI_SKILLLEARN, MSGCMD_SKILLLEARN_NOTIFY );
	strMessage.PrintF( _S( 277, "%s 스킬을 습득하였습니다" ), rSkill.GetName() );
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );

	MY_INFO()->SetSkill(slIndex, (int)sbLevel);

	if( sbIsNew == 0 )
	{
		//pUIManager->GetQuickSlot()->UpdateSkillLevel( slIndex, sbLevel );
	}
	else
		pUIManager->GetCharacterInfo()->AddSkill( slIndex, sbLevel );

	UpdateSkillData();

	// effect start!!
	{
		m_bSkillLevelupEffect = true;
		m_nSkillLVUpEffectTempCount = m_nSkillLevelupEffectCount;
	}
}

WMSG_RESULT CUISkillNew::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	CUIManager::getSingleton()->ResetHoldBtn();
	return WMSG_FAIL;
}

WMSG_RESULT CUISkillNew::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pDragArea && m_pDragArea->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
	}

	CUIManager::getSingleton()->RearrangeOrder(UI_SKILL_NEW, TRUE);

	return WMSG_FAIL;
}

WMSG_RESULT CUISkillNew::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
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

void CUISkillNew::SetRightBackGround()
{
	CUIImage* pImg = (CUIImage*)findUI("img_sub_back_right");

	if (pImg == NULL)
		return;

	int job = _pNetwork->MyCharacterInfo.job;

	UIRectUV uv;

	uv = pImg->GetAbsUV();

	uv.U0 = ((job % 5) * (DEF_SUB_BACK_UV_R + DEF_SUB_BACK_GAPX));
	uv.V0 = ((job / 5) * DEF_SUB_BACK_GAPY) + ((job / 5) * DEF_SUB_BACK_UV_B ) + DEF_SUB_BACK_SY;	// tex 데이타 첫줄에 5개, 두번째 4개 있으므로,,,,
	uv.U1 = uv.U0 + DEF_SUB_BACK_UV_R;
	uv.V1 = uv.V0 + DEF_SUB_BACK_UV_B;

	pImg->SetUV(uv);
}

void CUISkillNew::SetTabString()
{
	int job = _pNetwork->MyCharacterInfo.job;
	int _2ndJob = _pNetwork->MyCharacterInfo.job2;

	CUICheckButton* pCheck0 = NULL;
	CUICheckButton* pCheck1 = NULL;
	CUICheckButton* pCheck2 = NULL;

	pCheck0 = (CUICheckButton*)findUI("cb_class_0");
	pCheck1 = (CUICheckButton*)findUI("cb_class_1");
	pCheck2 = (CUICheckButton*)findUI("cb_class_2");

	if (pCheck0 == NULL || pCheck1 == NULL || pCheck2 == NULL)
		return;

	CJobInfo* pInfo = CJobInfo::getSingleton();

	pCheck0->SetText(pInfo->GetName(job, 0));
	pCheck0->Hide(FALSE);
	pCheck0->setEdge(true);

	pCheck1->SetText(pInfo->GetName(job, 1));
	pCheck1->Hide(FALSE);
	pCheck1->setEdge(true);

	pCheck2->SetText(pInfo->GetName(job, 2));
	pCheck2->Hide(FALSE);
	pCheck2->setEdge(true);

	if ( _2ndJob > 0 )
	{
		pCheck0->SetText(pInfo->GetName(job, _2ndJob));
		pCheck1->Hide(TRUE);
		pCheck2->Hide(TRUE);
	}
}

void CUISkillNew::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
 	UpdateLevelupEffect(ElapsedTime);

	UpdateSatisfiedSkill(ElapsedTime);
}

void CUISkillNew::SetSkillCoolTime( int idx )
{
	if (m_pArray[eTAB_NORMAL] == NULL)
		return;

	CUIArrayItem* pItem;
	CUIIcon*	  pIcon;

	int i, max = m_pArray[eTAB_NORMAL]->GetArrayChildCount();
	int job = _pNetwork->MyCharacterInfo.job;
	int job2 = 0;
	int nPos = 0;
	int nIndex = -1;

	for (i = 0; i < max; ++i)
	{
		int nidx = i;

		if ( i >= CSkillTree::m_nSkillCount[job][0] )
		{
			nidx -= CSkillTree::m_nSkillCount[job][0];
			job2 = _pNetwork->MyCharacterInfo.job2;
		}

		nIndex = CSkillTree::m_SkillTree[job][job2][nidx/8].index[nidx%8];

		if ( nIndex == idx )
		{
			pItem = m_pArray[eTAB_NORMAL]->GetArrayItem(i);

			if (pItem == NULL)
				continue;

			pIcon = (CUIIcon*)pItem->findUI("icon_skill");

			if (pIcon == NULL)
				break;

			pIcon->setCooltime(true);

			break;
		}
	}
	
}

bool CUISkillNew::IsSummonSkill( int nSkillIdx )
{
	int job = _pNetwork->MyCharacterInfo.job;
	int job2 = _pNetwork->MyCharacterInfo.job2;

	if (job != SORCERER || job2 != 2)
		return false;

	if( nSkillIdx == DEF_EARTH_SUMMON ||
		nSkillIdx == DEF_FIRE_SUMMON  ||
		nSkillIdx == DEF_WATER_SUMMON ||
		nSkillIdx == DEF_WIND_SUMMON  ||
		nSkillIdx == DEF_VITAL_CONVERSION ) 	// 스페셜리스트로 전직한 후 특정 스킬 처리.
		return true;

	return false;
}

BOOL CUISkillNew::IsReinforcementSkill( int nSkillIdx )
{
	int job = _pNetwork->MyCharacterInfo.job;
	int job2 = _pNetwork->MyCharacterInfo.job2;

	if (job != SORCERER || job2 != 2)
		return FALSE;

	if( nSkillIdx == DEF_EARTH_REINFORCEMENT ||
		nSkillIdx == DEF_FIRE_REINFORCEMENT  ||
		nSkillIdx == DEF_WATER_REINFORCEMENT ||
		nSkillIdx == DEF_WIND_REINFORCEMENT ) 	// 스페셜리스트로 전직한 후 특정 스킬 처리.
		return TRUE;

	return FALSE;
}

void CUISkillNew::UpdateLevelupEffect(ULONG Time)
{
	if (m_bSkillLevelupEffect == false)
		return;

	BOOL  bHide = FALSE;

	if (m_ulSkillLVUpEffectStartTime + m_ulSkillLVUpEffectElapsTime > Time)
		bHide = FALSE;
	else if (m_ulSkillLVUpEffectStartTime + (m_ulSkillLVUpEffectElapsTime * 2) > Time)
		bHide = TRUE;
	else
	{
		m_ulSkillLVUpEffectStartTime = Time;
		bHide = TRUE;

		if (m_nSkillLVUpEffectTempCount > 0)
			m_nSkillLVUpEffectTempCount--;
		else if (m_nSkillLVUpEffectTempCount == 0)
			m_bSkillLevelupEffect = false;
	}

	if (CUITabPage* pPage = (CUITabPage*)findUI("page_class_0"))
	{
		CUIImage* pImg = (CUIImage*)pPage->findUI("img_levelup_effect");
		CUIArrayItem* pItem = m_pArray[eTAB_NORMAL]->GetArrayItem(m_nCurPos);

		if (pImg == NULL)
			return;

		if (bHide == TRUE || pItem == NULL)
		{
			pImg->Hide(bHide);
			return;
		}

		CSkill skill = _pNetwork->GetSkillData(m_nCurIndex);

		UIRectUV uv;
		UIRect	 rc;

		if (skill.GetType() == CSkill::ST_PASSIVE)
			m_rsSkillLvUpEffect.GetRectAndUV(rc, uv, eSKILL_PASSIVE);
		else
			m_rsSkillLvUpEffect.GetRectAndUV(rc, uv, eSKILL_ACTIVE);

		pImg->SetUV(uv);

		int x, y;

		pItem->GetPos(x, y);

		pImg->SetPos(x, y);
		pImg->Hide(bHide);

	}	
}

void CUISkillNew::UpdateSatisfiedSkill(ULONG Time)
{
	if (m_bSatisfiedEffect == false)
		return;

	BOOL  bHide = FALSE;

	if (m_ulSatisfiedEffectStart + m_ulSatisfiedEffectElaps > Time)
		bHide = FALSE;
	else if (m_ulSatisfiedEffectStart + (m_ulSatisfiedEffectElaps * 2) > Time)
		bHide = TRUE;
	else
	{
		m_ulSatisfiedEffectStart = Time;
		bHide = TRUE;
		
		if (m_nSatisfiedEffectTempCount > 0)
			m_nSatisfiedEffectTempCount--;
		else if (m_nSatisfiedEffectTempCount == 0)
			m_bSatisfiedEffect = false;
	}

	vecIter bIt = m_vecSatisfiedSkill.begin();
	vecIter eIt = m_vecSatisfiedSkill.end();

	for( ; bIt != eIt; ++bIt )
	{
		if ( (&bIt) == NULL )
			continue;

		CUIArrayItem* pItem = (CUIArrayItem*)m_pArray[eTAB_NORMAL]->GetArrayItem(bIt->nPos);

		if (pItem == NULL)
			continue;

		CUIImage* pImg = NULL;
		
		if (bIt->bPassive == true)
			pImg = (CUIImage*)pItem->findUI("img_skill_passive");
		else
			pImg = (CUIImage*)pItem->findUI("img_skill_active");

		if (pImg == NULL)
			continue;

		if (bIt->bShow == false)
		{
			pImg->Hide(TRUE);
			continue;
		}

		pImg->Hide(bHide);
	}
}

void CUISkillNew::SetToggle( int nIndex )
{
	if (GetHide() == TRUE)
		return;

	CSkill& rSkill = _pNetwork->GetSkillData(nIndex);

	if ( !(rSkill.GetFlag() & SF_TOGGLE) )
		return;

	int i, max = m_pArray[0]->GetArrayChildCount();

	CUIArrayItem* pItem = NULL;
	CUIIcon* pIcon = NULL;

	for (i = 0; i < max; ++i)
	{
		pItem = m_pArray[0]->GetArrayItem(i);

		if (pItem == NULL)
			continue;

		pIcon = (CUIIcon*)pItem->findUI("icon_skill");

		if (pIcon == NULL)
			continue;

		if (pIcon->getIndex() == nIndex)
		{
			pIcon->SetToggle(rSkill.GetToggle());
			break;
		}
	}
}

int CUISkillNew::CalcLearnSkillPoint()
{
	int nCalcSp = 0;
	int nSkillIndex = 0;
	int job = _pNetwork->MyCharacterInfo.job;
	int sj = _pNetwork->MyCharacterInfo.job2;

	int nMax = CSkillTree::m_nSkillCount[job][0];	// 기본 직업 스킬 카운트.

	if (sj > 0)
		nMax += CSkillTree::m_nSkillCount[job][sj];

	for (int i = 0; i < nMax; ++i)
	{
		nSkillIndex = GetSkillIndex(job, sj, i);

		CSkill &rSkill = _pNetwork->GetSkillData(nSkillIndex);

		int nCurLv = MY_INFO()->GetSkillLevel(nSkillIndex);
		int nMaxLv = rSkill.GetMaxLevel();

		if (nMaxLv == nCurLv)
			continue;

		for (int j = nCurLv; j < nMaxLv; ++j)
		{
			if (nMaxLv == nCurLv)
				break;

			nCalcSp += rSkill.GetLearnSP(j);
		}
	}

	return nCalcSp;
}

#ifdef VER_TEST	// GM명령어로 인한 이펙트 처리.
void CUISkillNew::SetSatisfiedEffect( int nElapsed, int nCount)
{
	if ( nElapsed > 0 )
		m_ulSatisfiedEffectElaps = nElapsed;

	m_nSatisfiedEffectCount = nCount;
	m_nSatisfiedEffectTempCount = m_nSatisfiedEffectCount;
}

void CUISkillNew::SetSkillLVUpEffect( int nElapsed, int nCount)
{
	if ( nElapsed > 0 )
		m_ulSkillLVUpEffectElapsTime = nElapsed;

	m_nSkillLevelupEffectCount = nCount;
	m_nSkillLVUpEffectTempCount = m_nSkillLevelupEffectCount;
}

#endif // VER_TEST
