
#include "stdh.h"

// 헤더 정리. [12/3/2009 rumist]
#include <map>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIWildPetInfo.h>
#include <iostream>
#include <Engine/Info/MyInfo.h>
#include <Engine/Interface/UIInventory.h>

//#define DEL_PET_AI_EDIT

// UI Renewal [9/16/2009 rumist]
extern INDEX g_iXPosInWPetInfo;
extern INDEX g_iYPosInWPetInfo;

CUIWildPetInfo::CUIWildPetInfo()
{
	m_bTitle = FALSE;
	m_bAIActive = FALSE;
	m_bSlideUI = FALSE;
	m_nTebnum = WILDPET_INFO_SLOT;

	for (int nslot = 0; nslot < WILDPET_AI_MAXSLOT; nslot++)
	{
		m_sAISlot[nslot].m_bActive = FALSE;
		m_sAISlot[nslot].m_nSillType = -1;
		m_sAISlot[nslot].m_nActionSlot = -1;
		m_sAISlot[nslot].m_nTargetSlot = -1;
	}
	m_ptdButtonTexture = NULL;
}
CUIWildPetInfo::~CUIWildPetInfo()
{
	Destroy();

	STOCK_RELEASE(m_ptdButtonTexture);

	int		i;

	for (i = 0; i < WILDPET_AI_MAXSLOT; ++i)
	{
		SAFE_DELETE(m_pIconAITarget[i]);
		SAFE_DELETE(m_pIconAIAction[i]);
	}

	for (i = 0; i < WILDPET_WEAR_TOTAL; ++i)
		SAFE_DELETE(m_pIconWearing[i]);

	for (i = 0; i < 8; ++i)
		SAFE_DELETE(m_pIconSkill[i]);

	deleteSkill();
}

static int m_nSelSkillIndex = -1;
static int m_nShowItem = -1;
static int m_nShowSkill = -1;
static int m_nAIShowItem = -1;
static int m_nAIShowSkill = -1;

void CUIWildPetInfo::Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Create skill learn texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\new_PetInfo.tex" ) );
	m_ptdButtonTexture = CreateTexture(CTString("Data\\Interface\\CommonBtn.tex"));
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_rtsBace.AddRectSurface(UIRect(0,0,nWidth,37),UIRectUV(239,389,511,425,fTexWidth,fTexHeight));
	m_rtsBace.AddRectSurface(UIRect(0,37,nWidth,nHeight-10),UIRectUV(239,425,511,502,fTexWidth,fTexHeight));
	m_rtsBace.AddRectSurface(UIRect(0,nHeight-10,nWidth,nHeight),UIRectUV(239,502,511,511,fTexWidth,fTexHeight));

	m_rtsAIBace.AddRectSurface(UIRect(273,118,397,400),UIRectUV(525,0,649,282,fTexWidth,fTexHeight));

	m_rtTitle.SetRect(0,0,nWidth,37);

	m_rtInfoBace.SetUV(0,0,235,357,fTexWidth,fTexHeight);

	m_rtSkillBace.SetUV(277,0,511,356,fTexWidth,fTexHeight);

	m_rtAIONOFF.SetRect(48, 177, 73, 202);

	m_btnAIEdit.Create(this, CTString(""), 219, 177, 25, 25);
	m_btnAIEdit.SetUV(UBS_IDLE, 114, 375, 138, 399, fTexWidth, fTexHeight);
	m_btnAIEdit.SetUV(UBS_CLICK, 142, 375, 166, 399, fTexWidth, fTexHeight);
	m_btnAIEdit.CopyUV(UBS_IDLE, UBS_ON);
	m_btnAIEdit.CopyUV(UBS_IDLE, UBS_DISABLE);
	
	int		nAISlot, i;
	for( nAISlot = 0; nAISlot < WILDPET_AI_MAXSLOT; nAISlot++)
	{
		m_pIconAITarget[nAISlot] = new CUIIcon;
		m_pIconAITarget[nAISlot]->Create(this, 297, 141+(nAISlot * 39), 32, 32, UI_WILDPET_INFO, UBET_ITEM);

		m_rtAITarget[nAISlot].SetRect(297,141+(nAISlot*39),297+32,141+(nAISlot*39)+32);
		m_pIconAIAction[nAISlot] = new CUIIcon;
		m_pIconAIAction[nAISlot]->Create(this, 348, 141+(nAISlot * 39), 32, 32, UI_WILDPET_INFO, UBET_SKILL);
		m_rtAIAction[nAISlot].SetRect(348,141+(nAISlot*39),348+32,141+(nAISlot*39)+32);
	}

	m_rvAccuback.SetBoxUV(m_ptdBaseTexture,20, WRect(5, 175, 227, 218));
	m_rvAccuback.SetBoxPos(UIRect(35,212,257,388));
	m_rvAccublank.SetUV(60, 50, 220, 65,fTexWidth, fTexHeight);
	// Button Texture

	fTexWidth = m_ptdButtonTexture->GetPixWidth();
	fTexHeight = m_ptdButtonTexture->GetPixHeight();
	

	for (i = 0; i < WILDPET_SLOT_TOTAL; i++)
	{
		m_btTabSlot[i].Create(this,CTString( "" ),10,41+(i*(3+100)),21,100);
		m_btTabSlot[i].SetUV(UBS_IDLE,86,0,107,101,fTexWidth,fTexHeight);
		m_btTabSlot[i].SetUV(UBS_CLICK,59,0,80,101,fTexWidth,fTexHeight);
		m_btTabSlot[i].CopyUV(UBS_CLICK,UBS_ON);
		m_btTabSlot[i].CopyUV(UBS_IDLE,UBS_DISABLE);
	}

	m_btTabSlot[WILDPET_INFO_SLOT].SetText(_S(4204, "기본정보"),TRUE);
	m_btTabSlot[WILDPET_INFO_SLOT].SetBtnState(UBS_ON);
	m_btTabSlot[WILDPET_SKILL_SLOT].SetText(_S(4205, "스킬정보"),TRUE);

	m_btnClose.Create(this,CTString(""),247,4,16,16);
	m_btnClose.SetUV(UBS_IDLE,211,33,226,48,fTexWidth,fTexHeight);
	m_btnClose.SetUV(UBS_CLICK,229,33,244,48,fTexWidth,fTexHeight);
	m_btnClose.CopyUV(UBS_IDLE,UBS_ON);
	m_btnClose.CopyUV(UBS_IDLE,UBS_DISABLE);


	m_btnPoint[WILDPET_STR].Create(this,CTString(""),126,215,15,15);
	m_btnPoint[WILDPET_STR].SetUV(UBS_IDLE,121,49,135,63,fTexWidth,fTexHeight);
	m_btnPoint[WILDPET_STR].SetUV(UBS_CLICK,138,49,152,63,fTexWidth,fTexHeight);
	m_btnPoint[WILDPET_STR].CopyUV(UBS_IDLE,UBS_ON);
	m_btnPoint[WILDPET_STR].CopyUV(UBS_IDLE,UBS_DISABLE);

	m_btnPoint[WILDPET_CON].Create(this,CTString(""),234,215,15,15);
	m_btnPoint[WILDPET_CON].SetUV(UBS_IDLE,121,49,135,63,fTexWidth,fTexHeight);
	m_btnPoint[WILDPET_CON].SetUV(UBS_CLICK,138,49,152,63,fTexWidth,fTexHeight);
	m_btnPoint[WILDPET_CON].CopyUV(UBS_IDLE,UBS_ON);
	m_btnPoint[WILDPET_CON].CopyUV(UBS_IDLE,UBS_DISABLE);

	m_btnPoint[WILDPET_DEX].Create(this,CTString(""),126,237,15,15);
	m_btnPoint[WILDPET_DEX].SetUV(UBS_IDLE,121,49,135,63,fTexWidth,fTexHeight);
	m_btnPoint[WILDPET_DEX].SetUV(UBS_CLICK,138,49,152,63,fTexWidth,fTexHeight);
	m_btnPoint[WILDPET_DEX].CopyUV(UBS_IDLE,UBS_ON);
	m_btnPoint[WILDPET_DEX].CopyUV(UBS_IDLE,UBS_DISABLE);

	m_btnPoint[WILDPET_INT].Create(this,CTString(""),234,237,15,15);
	m_btnPoint[WILDPET_INT].SetUV(UBS_IDLE,121,49,135,63,fTexWidth,fTexHeight);
	m_btnPoint[WILDPET_INT].SetUV(UBS_CLICK,138,49,152,63,fTexWidth,fTexHeight);
	m_btnPoint[WILDPET_INT].CopyUV(UBS_IDLE,UBS_ON);
	m_btnPoint[WILDPET_INT].CopyUV(UBS_IDLE,UBS_DISABLE);
		
	m_sbSkillList.Create( this, 246, 74, 10, 307 );
	m_sbSkillList.CreateButtons( TRUE, 10, 10, 0, 0, 10 );
	m_sbSkillList.SetScrollPos( 0 );
	m_sbSkillList.SetScrollRange( 0 );
	m_sbSkillList.SetItemsPerPage( 8 );
	// Up button
	m_sbSkillList.SetUpUV( UBS_IDLE, 156,33,165,42, fTexWidth, fTexHeight );
	m_sbSkillList.SetUpUV( UBS_CLICK, 168,33,177,42,  fTexWidth, fTexHeight );
	m_sbSkillList.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbSkillList.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_sbSkillList.SetDownUV( UBS_IDLE, 156, 45, 165, 54, fTexWidth, fTexHeight );
	m_sbSkillList.SetDownUV( UBS_CLICK, 168, 45, 177, 54, fTexWidth, fTexHeight );
	m_sbSkillList.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbSkillList.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbSkillList.SetBarTopUV( 185, 32, 194, 40, fTexWidth, fTexHeight );
	m_sbSkillList.SetBarMiddleUV( 185, 40, 194, 62, fTexWidth, fTexHeight );
	m_sbSkillList.SetBarBottomUV( 185, 62, 194, 70, fTexWidth, fTexHeight );
	// Wheel region
	m_sbSkillList.SetWheelRect( -205, 0, 215, 307 );

//	m_abtnWearing[WILDPET_WEAR_HEAD].Create(this,129,120,32,32,UI_WILDPET_INFO,UBET_ITEM);
//	m_abtnWearing[WILDPET_WEAR_BODY].Create(this,129,159,32,32,UI_WILDPET_INFO,UBET_ITEM);
//	m_abtnWearing[WILDPET_WEAR_WEAPON].Create(this,88,139,32,32,UI_WILDPET_INFO,UBET_ITEM);
//	m_abtnWearing[WILDPET_WEAR_ACCE].Create(this,171,140,32,32,UI_WILDPET_INFO,UBET_ITEM);

	for (i = 0; i < WILDPET_WEAR_TOTAL; ++i)
	{
		m_pIconWearing[i] = new CUIIcon;
		m_pIconWearing[i]->initialize();
		m_pIconWearing[i]->SetWhichUI(UI_WILDPET_INFO);
		m_pIconWearing[i]->setParent(this);
	}
	m_pIconWearing[WILDPET_WEAR_HEAD]->InitPos(129, 120, 32, 32);
	m_pIconWearing[WILDPET_WEAR_BODY]->InitPos(129, 159, 32, 32);
	m_pIconWearing[WILDPET_WEAR_WEAPON]->InitPos(88, 139, 32, 32);
	m_pIconWearing[WILDPET_WEAR_ACCE]->InitPos(171, 140, 32, 32);

	for (int sSlot = 0; sSlot < 8; sSlot++)
	{
//		m_abtnSkill[sSlot].Create(this,42,75+(sSlot*39),32,32,UI_WILDPET_INFO,UBET_SKILL);
		m_pIconSkill[sSlot] = new CUIIcon;
		m_pIconSkill[sSlot]->initialize();
		m_pIconSkill[sSlot]->InitPos(42, 75+(sSlot * 39), 32, 32);
		m_pIconSkill[sSlot]->setParent(this);
		//m_abtnSkill[sSlot]->setData(UBET_SKILL, )
		m_pIconSkill[sSlot]->SetWhichUI(UI_WILDPET_INFO);
	}

	for( nAISlot = 0; nAISlot < WILDPET_AI_MAXSLOT; nAISlot++)
	{
		m_cbtnAISlotActive[nAISlot].Create(this, 279, 150+(nAISlot*39), 15, 13, CTString(""), TRUE);
		m_cbtnAISlotActive[nAISlot].SetUV(UCBS_NONE, 139, 75, 153, 87, fTexWidth, fTexHeight);
		m_cbtnAISlotActive[nAISlot].SetUV(UCBS_CHECK, 119, 75, 133, 87, fTexWidth, fTexHeight);
		m_cbtnAISlotActive[nAISlot].CopyUV(UCBS_NONE, UCBS_CHECK_DISABLE);
		m_cbtnAISlotActive[nAISlot].CopyUV(UCBS_NONE, UCBS_NONE_DISABLE);
		m_cbtnAISlotActive[nAISlot].SetCheck(FALSE);		
	}

	m_btnAIMake.Create(this, _S(4021,"편집 완료"), 295, 374, 81, 22);
	m_btnAIMake.SetUV(UBS_IDLE, 113, 0, 182, 21, fTexWidth, fTexHeight);
	m_btnAIMake.SetUV(UBS_CLICK, 186, 0, 255, 21, fTexWidth, fTexHeight);
	m_btnAIMake.CopyUV( UBS_IDLE, UBS_ON );
	m_btnAIMake.CopyUV( UBS_IDLE, UBS_DISABLE );

	// 임시
	m_nAISlotCount = 3;

	m_btnAccumulateUse.Create(this, _S(5013, "사용하기"),150, 216, 100, 22);
	m_btnAccumulateUse.SetUV(UBS_IDLE, 113, 0, 183, 22, fTexWidth, fTexHeight);
	m_btnAccumulateUse.SetUV(UBS_CLICK, 186, 0, 256, 22, fTexWidth, fTexHeight);
	m_btnAccumulateUse.CopyUV(UBS_IDLE, UBS_ON);
	m_btnAccumulateUse.CopyUV(UBS_IDLE, UBS_DISABLE);

}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIWildPetInfo::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void CUIWildPetInfo::ResetSavePosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );

	g_iXPosInWPetInfo = GetPosX();
	g_iYPosInWPetInfo = GetPosY();
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIWildPetInfo::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}
void CUIWildPetInfo::ToggleVisible()
{
	BOOL	bVisible = !IsVisible();

	if(bVisible)
	{
		SetPos( g_iXPosInWPetInfo, g_iYPosInWPetInfo );
		OpenWildPetInfo();
	}else
	{
		g_iXPosInWPetInfo = GetPosX();
		g_iYPosInWPetInfo = GetPosY();
		CloseWildPetInfo();
	}

}

void CUIWildPetInfo::OpenWildPetInfo( )
{
	// If this is already exist
	if( IsVisible() )
		return;

	ObjInfo* pInfo = ObjInfo::getSingleton();

	if (pInfo->GetMyApetInfo() != NULL)
		m_Petdata = CWildPetData::getData(pInfo->GetMyApetInfo()->m_nIndex);

	CUIManager::getSingleton()->RearrangeOrder( UI_WILDPET_INFO, TRUE );

	// 켜짐과 동시에 포커스를 가져간다. [5/30/2011 rumist]
	SetFocus( TRUE );

}

void CUIWildPetInfo::CloseWildPetInfo()
{
	PetWearItemReSet();
	PetAIReSet();
	m_Petdata = NULL; 
	CUIManager::getSingleton()->RearrangeOrder( UI_WILDPET_INFO, FALSE );

	int		i;

	for(i = 0; i < WILDPET_WEAR_TOTAL; ++i)
		m_pIconWearing[i]->CloseProc();

	for (i = 0; i < 8; ++i)
		m_pIconSkill[i]->CloseProc();

	for (i = 0; i < WILDPET_AI_MAXSLOT; ++i)
	{
		m_pIconAITarget[i]->CloseProc();
		m_pIconAIAction[i]->CloseProc();
	}
}

void CUIWildPetInfo::AIClear()
{
	for (int nslot = 0; nslot < WILDPET_AI_MAXSLOT; nslot++)
	{
		m_sAISlot[nslot].m_bActive = FALSE;
		m_sAISlot[nslot].m_nSillType = -1;
		m_sAISlot[nslot].m_nActionSlot = -1;
		m_sAISlot[nslot].m_nTargetSlot = -1;

		m_cbtnAISlotActive[nslot].SetCheck(FALSE);
		
		m_pIconAITarget[nslot]->clearIconData();
		m_pIconAIAction[nslot]->clearIconData();
	}

}

void CUIWildPetInfo::Render()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();

	if (pInfo->GetMyApetInfo() == NULL)
		return;

	if(pInfo->GetMyApetInfo()->bIsActive == FALSE)
		return;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set skill learn texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	m_rtsBace.SetPos(m_nPosX,m_nPosY);
	m_rtsBace.RenderRectSurface(pDrawPort,0xFFFFFFFF);

	//Text
	CTString strTemp;
	strTemp.PrintF(_S(2173, "펫 정보"));
	pDrawPort->PutTextExCX(strTemp, m_nPosX+WILDPETINFO_WIDTH/2, m_nPosY+15);

	pDrawPort->FlushRenderingQueue();

	pDrawPort->InitTextureData( m_ptdButtonTexture );

	for(int i = 0; i < WILDPET_SLOT_TOTAL; i++)
	{
		m_btTabSlot[i].Render();
	}

	// [2010/10/06 : Sora] 종료 버튼 렌더 위치 수정(팝업 정보 위에 표시되는 버그 확인)
	m_btnClose.Render();

	pDrawPort->FlushRenderingQueue();

	
	if(m_nTebnum == WILDPET_INFO_SLOT)
	{
		InfoRender();		
	}
	else if(m_nTebnum == WILDPET_SKILL_SLOT)
	{
		SkillRender();
	}

	if(m_bSlideUI)
	{
		AIPopupRender();		
	}
	
	// Render all elements
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();

}
#define COLOR_RED RGBAToColor(255,0,0,255)
#define COLOR_YELLOW RGBAToColor(255,255,0,255)

void CUIWildPetInfo::InfoRender()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();

	if (pInfo->GetMyApetInfo() == NULL)
		return;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	pDrawPort->AddTexture(m_nPosX+30,m_nPosY+37,m_nPosX+264,m_nPosY+393,
		m_rtInfoBace.U0,m_rtInfoBace.V0,m_rtInfoBace.U1,m_rtInfoBace.V1, 0xFFFFFFFF);
	
	UIRectUV temTex;
	
	if (pInfo->GetMyApetInfo()->m_nType == 2)
	{
		
		temTex.SetUV(517,417,738,511,fTexWidth,fTexHeight);
		pDrawPort->AddTexture(m_nPosX+35,m_nPosY+112,m_nPosX+256,m_nPosY+206,
		temTex.U0,temTex.V0,temTex.U1,temTex.V1, 0xFFFFFFFF);
	}else if (pInfo->GetMyApetInfo()->m_nType == 3)
	{

		temTex.SetUV(745,417,966,511,fTexWidth,fTexHeight);
		pDrawPort->AddTexture(m_nPosX+35,m_nPosY+112,m_nPosX+256,m_nPosY+206,
		temTex.U0,temTex.V0,temTex.U1,temTex.V1, 0xFFFFFFFF);
	}
	

	//Text
	CTString strTemp;

	int tempboxheight = 15;
	strTemp.PrintF(_S(71, "이름"));
	pDrawPort->PutTextEx(strTemp, m_nPosX+41, m_nPosY+49);
	pDrawPort->PutTextExCX(pInfo->GetMyApetInfo()->m_strName.c_str(), m_nPosX+170,m_nPosY+49);

	strTemp.PrintF(_S(73, "레벨"));
	pDrawPort->PutTextEx(strTemp, m_nPosX+41, m_nPosY+69);
	strTemp.PrintF("%d",pInfo->GetMyApetInfo()->m_nLevel);
	pDrawPort->PutTextExCX(strTemp, m_nPosX+107,m_nPosY+69);

	strTemp.PrintF(_S(4206, "종족"));
	pDrawPort->PutTextEx(strTemp, m_nPosX+141, m_nPosY+69);
	
	if (pInfo->GetMyApetInfo()->m_nType == 1)
	{
		strTemp.PrintF(_S(4207, "인간형"));
	}else if(pInfo->GetMyApetInfo()->m_nType == 2)
	{
		strTemp.PrintF(_S(4208, "동물형"));
	}else if(pInfo->GetMyApetInfo()->m_nType == 3)
	{
		strTemp.PrintF(_S(5269, "데몬형"));
	}
	pDrawPort->PutTextExCX(strTemp, m_nPosX+214,m_nPosY+69);

	strTemp.PrintF(_S(89, "경험치"));
	pDrawPort->PutTextEx(strTemp, m_nPosX+41, m_nPosY+89);
	strTemp.PrintF("%I64d / %I64d", pInfo->GetMyApetInfo()->m_exp, pInfo->GetMyApetInfo()->m_next_exp);
	pDrawPort->PutTextExCX(strTemp, m_nPosX+170,m_nPosY+89);

	// 충성도
	// [090824 sora 충성도값 %에서 수치로 변경]
	
	FLOAT temFaithResult = (FLOAT)pInfo->GetMyApetInfo()->m_nFaith/(FLOAT)pInfo->GetMyApetInfo()->m_nMaxFaith *100.0f;
	if(temFaithResult > 60)
	{
		m_rtFaithImage.SetUV(69,405,87,423,fTexWidth,fTexHeight);
	}else if(temFaithResult > 20)
	{
		m_rtFaithImage.SetUV(92,405,110,423,fTexWidth,fTexHeight);
	}else
	{
		m_rtFaithImage.SetUV(115,405,133,423,fTexWidth,fTexHeight);
	}
	strTemp.PrintF(_S(4209, "충성도"));
	pDrawPort->PutTextExCX(strTemp,m_nPosX+234,m_nPosY+116);
	pDrawPort->AddTexture(m_nPosX+224,m_nPosY+130,m_nPosX+244,m_nPosY+150,
		m_rtFaithImage.U0,m_rtFaithImage.V0,m_rtFaithImage.U1,m_rtFaithImage.V1,0xFFFFFFFF);
	
	strTemp.PrintF("%d/%d",pInfo->GetMyApetInfo()->m_nFaith, pInfo->GetMyApetInfo()->m_nMaxFaith);	
	pDrawPort->PutTextExCX(strTemp,m_nPosX+234,m_nPosY+152);


	// 스테미너
	// [090824 sora 배고픔값 %에서 수치로 변경]
	FLOAT temStmResult = (FLOAT)pInfo->GetMyApetInfo()->m_nStm/(FLOAT)pInfo->GetMyApetInfo()->m_nMaxStm *100.0f;
	if(temStmResult > 60)
	{
		m_rtStmImage.SetUV(0,405,18,423,fTexWidth,fTexHeight);
	}else if(temStmResult > 20)
	{
		m_rtStmImage.SetUV(23,405,41,423,fTexWidth,fTexHeight);
	}else
	{
		m_rtStmImage.SetUV(46,405,64,423,fTexWidth,fTexHeight);
	}
	
	strTemp.PrintF(_S(4210, "배고품"));
	pDrawPort->PutTextExCX(strTemp,m_nPosX+56,m_nPosY+116);
	pDrawPort->AddTexture(m_nPosX+46,m_nPosY+130,m_nPosX+66,m_nPosY+150,
		m_rtStmImage.U0,m_rtStmImage.V0,m_rtStmImage.U1,m_rtStmImage.V1,0xFFFFFFFF);
	
	strTemp.PrintF("%d/%d",pInfo->GetMyApetInfo()->m_nStm, pInfo->GetMyApetInfo()->m_nMaxStm);
	pDrawPort->PutTextExCX(strTemp,m_nPosX+56,m_nPosY+152);
	
	if (!(m_Petdata->nFlag & WILDPET_FLAG_EXP ))
	{
		m_btnAIEdit.Render();
		if (m_bAIActive)
		{
			
			m_rvAIONOFF.SetUV(2, 375,26,399,fTexWidth,fTexHeight);
			
		}else
		{
			
			m_rvAIONOFF.SetUV(58, 375, 82, 399, fTexWidth, fTexHeight);
		}
		pDrawPort->AddTexture(m_rtAIONOFF.Left+m_nPosX, m_rtAIONOFF.Top+m_nPosY, m_rtAIONOFF.Right+m_nPosX, m_rtAIONOFF.Bottom+m_nPosY,
										m_rvAIONOFF.U0, m_rvAIONOFF.V0, m_rvAIONOFF.U1, m_rvAIONOFF.V1,0xFFFFFFFF);

	}

	// Render all elements
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();
	
	for(int i = 0; i < WILDPET_WEAR_TOTAL; i++)
	{
		if(pInfo->GetMyApetInfo()->m_nPetWearIndex[i] < 0) continue;
		m_pIconWearing[i]->Render(pDrawPort);
	}
	// Render all button elements
	
	if (m_Petdata->nFlag & WILDPET_FLAG_EXP )
	{
		AccumulateInfoRender();
		return;
	}

	//--------------------------------

	strTemp.PrintF(_S(4406, "힘"));

	pDrawPort->PutTextEx(strTemp, m_nPosX+41, m_nPosY+216);
	strTemp.PrintF("%d(%d)",pInfo->GetMyApetInfo()->m_nStr,pInfo->GetMyApetInfo()->m_nStrPlus);
	pDrawPort->PutTextEx(strTemp, m_nPosX+41+38, m_nPosY+216);

	strTemp.PrintF(_S(4409, "체질"));

	pDrawPort->PutTextEx(strTemp, m_nPosX+146, m_nPosY+216);
	strTemp.PrintF("%d(%d)",pInfo->GetMyApetInfo()->m_nCon,pInfo->GetMyApetInfo()->m_nConPlus);
	pDrawPort->PutTextEx(strTemp, m_nPosX+146+38, m_nPosY+216);

	strTemp.PrintF(_S(4407, "민첩"));

	pDrawPort->PutTextEx(strTemp, m_nPosX+41, m_nPosY+238);
	strTemp.PrintF("%d(%d)",pInfo->GetMyApetInfo()->m_nDex,pInfo->GetMyApetInfo()->m_nDexPlus);
	pDrawPort->PutTextEx(strTemp, m_nPosX+41+38, m_nPosY+238);

	strTemp.PrintF(_S(4408, "지혜"));

	pDrawPort->PutTextEx(strTemp, m_nPosX+146, m_nPosY+238);
	strTemp.PrintF("%d(%d)",pInfo->GetMyApetInfo()->m_nInt,pInfo->GetMyApetInfo()->m_nIntPlus);
	pDrawPort->PutTextEx(strTemp, m_nPosX+146+38, m_nPosY+238);

	strTemp.PrintF(_S(84, "성장포인트"));
	pDrawPort->PutTextEx(strTemp, m_nPosX+41, m_nPosY+258);
	strTemp.PrintF("%d",pInfo->GetMyApetInfo()->m_nLevelupPoint);
	pDrawPort->PutTextExCX(strTemp, m_nPosX+150+38, m_nPosY+258);



	COLOR	temColor = 0xFFFFFFFF;

	// 배고품 정도에 따라 패널티가 주어짐
	if(temFaithResult < 20)
	{
		temColor = COLOR_RED;
	}else
	{
		temColor = 0xFFFFFFFF;
	}

	strTemp.PrintF(_S(85, "공격력"));
	pDrawPort->PutTextEx(strTemp, m_nPosX+41, m_nPosY+281);
	strTemp.PrintF("%d",pInfo->GetMyApetInfo()->m_nWildPetState[STATE_ATTACK]);
	pDrawPort->PutTextExCX(strTemp, m_nPosX+150+38, m_nPosY+281,temColor);
	
	strTemp.PrintF(_S(86, "마법공격력"));
	pDrawPort->PutTextEx(strTemp, m_nPosX+41, m_nPosY+299);
	strTemp.PrintF("%d",pInfo->GetMyApetInfo()->m_nWildPetState[STATE_MAGIC_ATTACK]);
	pDrawPort->PutTextExCX(strTemp, m_nPosX+150+38, m_nPosY+299,temColor);

	
	strTemp.PrintF(_S(87, "방어력"));
	pDrawPort->PutTextEx(strTemp, m_nPosX+41, m_nPosY+317);
	strTemp.PrintF("%d",pInfo->GetMyApetInfo()->m_nWildPetState[STATE_DEFENCE]);
		pDrawPort->PutTextExCX(strTemp, m_nPosX+150+38, m_nPosY+317,temColor);

	strTemp.PrintF(_S(88, "마법방어력"));
	pDrawPort->PutTextEx(strTemp, m_nPosX+41, m_nPosY+335);
	strTemp.PrintF("%d",pInfo->GetMyApetInfo()->m_nWildPetState[STATE_MAGIC_DEFENCE]);
		pDrawPort->PutTextExCX(strTemp, m_nPosX+150+38, m_nPosY+335,temColor);


	// 배고품 정도에 따라 패널티가 주어짐
	if(temStmResult < 20)
	{
		temColor = COLOR_RED;
	}else
	{
		temColor = 0xFFFFFFFF;
	}

	strTemp.PrintF(_S(4211, "명중도"));
	pDrawPort->PutTextEx(strTemp, m_nPosX+41, m_nPosY+353);
	strTemp.PrintF("%d",pInfo->GetMyApetInfo()->m_nWildPetState[STATE_HITPOINT]);
	pDrawPort->PutTextExCX(strTemp, m_nPosX+150+38, m_nPosY+353,temColor);

	strTemp.PrintF(_S(4212, "회피"));
	pDrawPort->PutTextEx(strTemp, m_nPosX+41, m_nPosY+371);
	strTemp.PrintF("%d",pInfo->GetMyApetInfo()->m_nWildPetState[STATE_AVOID]);
	pDrawPort->PutTextExCX(strTemp, m_nPosX+150+38, m_nPosY+371,temColor);

	// Render all elements
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();

	pDrawPort->InitTextureData(m_ptdButtonTexture);

	for( int nPoint = 0; nPoint < POINT_END; nPoint++ )
	{
		m_btnPoint[nPoint].Render();
	}

	// Render all elements
	pDrawPort->FlushRenderingQueue();
}

void CUIWildPetInfo::SkillRender()
{
    CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

    pDrawPort->InitTextureData(m_ptdBaseTexture);

    pDrawPort->AddTexture(m_nPosX + 30, m_nPosY + 37, m_nPosX + 264, m_nPosY + 393,
                          m_rtSkillBace.U0, m_rtSkillBace.V0, m_rtSkillBace.U1, m_rtSkillBace.V1, 0xFFFFFFFF);

    int	nX = 46, nY = 75;
    int	iRowE;
    int nCount = 0;
    int nSelectID = -1;
    map_Icon::iterator	iter;
    map_Icon::iterator	iterSelect;
    map_Icon::iterator	iterBegin;
    map_Icon::iterator	iterEnd;

    LONG nSkillIconPos = m_sbSkillList.GetScrollPos();

    int nIconPos;

    iterBegin	= m_mapIconSkills.begin();
    iterEnd		= m_mapIconSkills.end();
    nIconPos	= nSkillIconPos;
    iterSelect	= m_mapIconSkills.find(m_nSelSkillIndex);

    if (iterSelect != iterEnd)
    {
        nSelectID = std::distance(iterBegin, iterSelect);
    }

    //Render Select OutLine
    iRowE = nIconPos + 4;

    pDrawPort->FlushRenderingQueue();


    pDrawPort->InitTextureData(m_ptdButtonTexture);
    m_sbSkillList.SetCurItemCount(m_mapIconSkills.size());

    m_sbSkillList.Render();

    // Render all elements
    pDrawPort->FlushRenderingQueue();

    // Render Skill Info
    nY = 76;

    iter = iterBegin;

	for( nCount = 0; nCount < nIconPos; iter++, nCount++ );

    for (nCount = 0; (iter != iterEnd) && (nCount < 8); iter++, nCount++, nY += 39)
    {
        if (iter->second->IsEmpty()) continue;

        LONG nSkillIndex = iter->second->getIndex();
        CSkill&	rSkill = _pNetwork->GetSkillData(nSkillIndex);
        SBYTE	sbLevel = MY_INFO()->GetPetSkillLevel(0, nSkillIndex);
        BOOL	bDelay = MY_INFO()->GetPetSkillDelay(0, nSkillIndex);
        CTString tempLv;

#if defined G_RUSSIA
        tempLv.PrintF("%s %d", _S(4414, "LV"), sbLevel);
#else
        tempLv.PrintF("Lv %d", sbLevel);
#endif
        m_pIconSkill[nCount]->setData(UBET_SKILL, nSkillIndex);

		if (bDelay == TRUE)
			m_pIconSkill[nCount]->setCooltime(true);
        
		m_pIconSkill[nCount]->Render(pDrawPort);

        pDrawPort->PutTextExCX(rSkill.GetName(), m_nPosX + 150, m_nPosY + nY, 0xC5C5C5FF);
        pDrawPort->PutTextExCX(tempLv, m_nPosX + 150, m_nPosY + nY + 15, 0xC5C5C5FF);

    }

    pDrawPort->EndTextEx();
}

WMSG_RESULT CUIWildPetInfo::MouseMessage(MSG *pMsg)
{
	WMSG_RESULT	wmsgResult;
	// Extended button clicked
	static BOOL	bLButtonDownInBtn = FALSE;
	static INDEX clickWearpos = -1;
	static INDEX clickSkillpos = -1;
	static INDEX clickAITargetpos = -1;
	static INDEX clickAIActionpos = -1;
	
	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			if( IsInside( nX, nY ) )
				pUIManager->SetMouseCursorInsideUIs();

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Hold item button
			if (pUIManager->GetDragIcon() == NULL && bLButtonDownInBtn && 
				(pMsg->wParam& MK_LBUTTON) && (ndX != 0 || ndY != 0))
			{
				// Close message box of inventory
				pUIManager->CloseMessageBox( MSGCMD_DROPITEM );

				if(clickWearpos != -1)
				{
					pUIManager->SetHoldBtn(m_pIconWearing[clickWearpos]);
				
					clickWearpos = -1;
				}
				else if (clickSkillpos != -1)
				{	
					if( _pNetwork->GetSkillData( m_pIconSkill[clickSkillpos]->getIndex() ).GetType() != CSkill::ST_PET_SKILL_PASSIVE )
						pUIManager->SetHoldBtn(m_pIconSkill[clickSkillpos]);

					clickSkillpos = -1;
				}
				else if (clickAITargetpos != -1 && m_bSlideUI)
				{
					pUIManager->SetHoldBtn(m_pIconAITarget[clickAITargetpos]);
				}
				else if (clickAIActionpos != -1 && m_bSlideUI)
				{
					pUIManager->SetHoldBtn(m_pIconAIAction[clickAIActionpos]);
				}

				bLButtonDownInBtn = FALSE;
			}
			// Move skill learn
			if( m_bTitle && ( pMsg->wParam & MK_LBUTTON ) )
			{
				
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			if(IsInside(nX,nY))
			{
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if(m_nTebnum == WILDPET_INFO_SLOT)
				{
					for( int iWear = 0; iWear < WILDPET_WEAR_TOTAL; iWear++ )
					{
						if( m_pIconWearing[iWear]->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							m_nShowItem = iWear;
							return WMSG_SUCCESS;
						}
					}

					if (m_Petdata->nFlag & WILDPET_FLAG_EXP )
					{
						m_btnAccumulateUse.MouseMessage(pMsg);
					}
					else
					{
						for( int nPoint = 0; nPoint < POINT_END; nPoint++ )
						{
							m_btnPoint[nPoint].MouseMessage(pMsg);
						}
					}			
				}
				else if(m_nTebnum == WILDPET_SKILL_SLOT)				
				{
					if( ( m_sbSkillList.MouseMessage( pMsg ) ) != WMSG_FAIL )
						return WMSG_SUCCESS;

					for (int nSkillnum = 0; nSkillnum < 8; nSkillnum++)
					{
						if(m_pIconSkill[nSkillnum]->MouseMessage(pMsg) != WMSG_FAIL)
						{
							m_nShowSkill = nSkillnum;
							return WMSG_SUCCESS;
						}						
					}
				}

				m_btnAIEdit.MouseMessage(pMsg);

				for( int ntab = 0; ntab < WILDPET_SLOT_TOTAL; ntab++ )
				{
					m_btTabSlot[ntab].MouseMessage(pMsg);
				}

			}else if(IsInsideRect(nX,nY,UIRect(273,118,397,400)) && m_bSlideUI)
			{
				for (int nSlot = 0; nSlot < m_nAISlotCount; nSlot++)
				{
					if (m_pIconAIAction[nSlot]->MouseMessage(pMsg) != WMSG_FAIL)
					{
						m_nAIShowSkill = nSlot;
						return WMSG_SUCCESS;
					} 
					else if(m_pIconAITarget[nSlot]->MouseMessage(pMsg) != WMSG_FAIL)
					{
						m_nAIShowItem = nSlot;
						return WMSG_SUCCESS;
					}

					m_cbtnAISlotActive[nSlot].MouseMessage(pMsg);
				}

				m_btnAIMake.MouseMessage(pMsg);
			}
		}break;
	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;
				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( IsInsideRect( nX, nY, m_rtTitle ) )
				{
					m_bTitle = TRUE;
				}			

				for( int ntab = 0; ntab < WILDPET_SLOT_TOTAL; ntab++ )
				{
					m_btTabSlot[ntab].MouseMessage(pMsg);
				}
				
				if(m_nTebnum == WILDPET_INFO_SLOT)
				{

					if (m_Petdata->nFlag & WILDPET_FLAG_EXP )
					{
						m_btnAccumulateUse.MouseMessage(pMsg);
					}
					else
					{
						for( int nPoint = 0; nPoint < POINT_END; nPoint++ )
						{
							m_btnPoint[nPoint].MouseMessage(pMsg);
						}
					}			

					for( int iWear = 0; iWear < WILDPET_WEAR_TOTAL; iWear++ )
					{
						if( m_pIconWearing[iWear]->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							clickWearpos = iWear;
							bLButtonDownInBtn = TRUE;					
							return WMSG_SUCCESS;
						}
					}
				}
				else if(m_nTebnum == WILDPET_SKILL_SLOT)
				{
					if( ( m_sbSkillList.MouseMessage( pMsg ) ) != WMSG_FAIL )
						return WMSG_SUCCESS;

					for (int nSkillnum = 0; nSkillnum < 8; nSkillnum++)
					{
						if(m_pIconSkill[nSkillnum]->MouseMessage(pMsg) != WMSG_FAIL)
						{
							clickSkillpos = nSkillnum;
							bLButtonDownInBtn = TRUE;
							return WMSG_SUCCESS;
						}
					}
				}

				m_btnAIEdit.MouseMessage(pMsg);
				
				CUIManager::getSingleton()->RearrangeOrder( UI_WILDPET_INFO, TRUE );
				return WMSG_SUCCESS;

			}
			else if(IsInsideRect(nX,nY,UIRect(273,118,397,400)) && m_bSlideUI)
			{
				m_btnAIMake.MouseMessage(pMsg);

				for (int nSlot = 0; nSlot < m_nAISlotCount; nSlot++)
				{
					if (m_pIconAITarget[nSlot]->MouseMessage(pMsg) != WMSG_FAIL)
					{
						clickAITargetpos = nSlot;
						clickAIActionpos = -1;
						bLButtonDownInBtn = TRUE;
						return WMSG_SUCCESS;
					}

					if (m_pIconAIAction[nSlot]->MouseMessage(pMsg) != WMSG_FAIL)
					{
						clickAIActionpos = nSlot;
						clickAITargetpos = -1;
						bLButtonDownInBtn = TRUE;
						return WMSG_SUCCESS;
					}

					m_cbtnAISlotActive[nSlot].MouseMessage(pMsg);
				}

				CUIManager::getSingleton()->RearrangeOrder(UI_WILDPET_INFO, TRUE);
				return WMSG_SUCCESS;
			}
			
		}break;
	case WM_LBUTTONUP:
		{
			m_bTitle = FALSE;
			clickSkillpos = -1;
			clickWearpos = -1;
			bLButtonDownInBtn = FALSE;
			CUIManager* pUIManager = CUIManager::getSingleton();			

			if (pUIManager->GetDragIcon() != NULL)
			{
				CUIIcon* pDrag = pUIManager->GetDragIcon();				

				if(m_nTebnum == WILDPET_INFO_SLOT && IsInside(nX,nY))
				{
					if (pDrag->getBtnType() == UBET_ITEM &&
						pDrag->GetWhichUI() == UI_INVENTORY)
					{
						CItems* pItems = pDrag->getItems();

						if (pItems == NULL)
							return WMSG_FAIL;

						CItemData* ptemItem = pItems->ItemData;
						_pNetwork->SendUseWildPetItem(ptemItem->GetWearingPosition(), 
									pItems->Item_Tab, pItems->InvenIndex, pItems->Item_Index);
					}
					pUIManager->ResetHoldBtn();
				}
				else if (IsInsideRect(nX,nY,UIRect(273,118,397,400)) && m_bSlideUI)
				{
					int nSlotnum = 0;

					for ( nSlotnum = 0; nSlotnum < m_nAISlotCount; nSlotnum++)
					{
						if (IsInsideRect(nX,nY,m_rtAITarget[nSlotnum]))
						{
							if (m_pIconAITarget[nSlotnum]->IsEmpty() == false && 
								m_pIconAITarget[nSlotnum]->getIndex() == pDrag->getIndex())
							{
								pUIManager->ResetHoldBtn();
								return WMSG_SUCCESS;
							}

							if (pDrag->getBtnType() == UBET_ITEM &&
								pDrag->GetWhichUI() == UI_INVENTORY )
							{
								CItems* pItems = pDrag->getItems();

								if (pItems == NULL)
									return WMSG_FAIL;

								CItemData* pCurItem = pItems->ItemData;

								if (pCurItem->GetSubType() == CItemData::ITEM_PET_AI)
								{
									if (CheckAIItem(pCurItem, nSlotnum) == TRUE)
									{
										m_pIconAITarget[nSlotnum]->setData(UBET_ITEM, pDrag->getIndex());
									}
									else
									{
										ErrorMassage(MSG_APET_ERR_WRONG_AI);
									}									
									break;
								}								
							}
						}
						else if (IsInsideRect(nX,nY,m_rtAIAction[nSlotnum]))
						{
							if (m_pIconAIAction[nSlotnum]->IsEmpty() == false &&
								m_pIconAIAction[nSlotnum]->getIndex() == pDrag->getIndex())
							{
								pUIManager->ResetHoldBtn();
								return WMSG_SUCCESS;
							}

							if (pDrag->getBtnType() == UBET_SKILL &&
								pDrag->GetWhichUI() == UI_WILDPET_INFO )
							{
								if ( CheckAIAction(pDrag, nSlotnum) == TRUE)
								{
									m_pIconAIAction[nSlotnum]->setData(UBET_SKILL, pDrag->getIndex());
								}
								else
								{
									ErrorMassage(MSG_APET_ERR_WRONG_AI);
								}									
								break;
							}
							else if (pDrag->getBtnType() == UBET_ACTION &&
									 pDrag->GetWhichUI() == UI_CHARACTERINFO )
							{
								CAction* curAction = CAction::getData(pDrag->getIndex());

								if (curAction == NULL)
									break;

								if (curAction->GetType() == 5)
								{
									if ( CheckAIAction(pDrag, nSlotnum) == TRUE)
									{
										m_pIconAIAction[nSlotnum]->setData(UBET_ACTION, pUIManager->GetDragIcon()->getIndex());
									}
									else
									{
										ErrorMassage(MSG_APET_ERR_WRONG_AI);
									}									
									break;
								}								
							}
						}
					}

					if (clickAIActionpos != -1 && m_bSlideUI)
					{
						m_pIconAIAction[clickAIActionpos]->clearIconData();
						clickAIActionpos = -1;
					} 
					else if(clickAITargetpos != -1 && m_bSlideUI)
					{
						m_pIconAITarget[clickAITargetpos]->clearIconData();
						clickAITargetpos = -1;
					}
					pUIManager->ResetHoldBtn();
					return WMSG_SUCCESS;
				}
				else
				{
					if (clickAIActionpos != -1 && m_bSlideUI)
					{
						m_pIconAIAction[clickAIActionpos]->clearIconData();
						clickAIActionpos = -1;
					} 
					else if(clickAITargetpos != -1 && m_bSlideUI)
					{
						m_pIconAITarget[clickAITargetpos]->clearIconData();
						clickAITargetpos = -1;
					}
				}
			}
			else if( IsInside( nX, nY ) )
			{
				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						CloseWildPetInfo();
					}
					return WMSG_SUCCESS;
				}

				if(m_nTebnum == WILDPET_INFO_SLOT)
				{
					if (m_Petdata->nFlag & WILDPET_FLAG_EXP )
					{

						if(m_btnAccumulateUse.MouseMessage(pMsg) == WMSG_COMMAND && !IsWildPetCoolTime())
						{
							if (pUIManager->DoesMessageBoxExist(MSGCMD_USE_PET_ACCUMULATE))
								return WMSG_SUCCESS;

							if (pUIManager->GetInventory()->IsLocked() == TRUE ||
								pUIManager->GetInventory()->IsLockedArrange() == TRUE)
							{
								pUIManager->GetInventory()->ShowLockErrorMessage();
								return WMSG_SUCCESS;
							}

							pUIManager->GetInventory()->Lock(TRUE, TRUE, LOCK_EXP_PET);

							CUIMsgBox_Info	MsgBoxInfo;
							CTString strMsg;

							MsgBoxInfo.SetMsgBoxInfo(_S(5632, "경험치 사용"),UMBS_SELECTBOX,UI_NONE,MSGCMD_USE_PET_ACCUMULATE);
							strMsg.PrintF(_S(5633, "축척된 경험치를 사용 하시겠습니까?"));
							MsgBoxInfo.AddString(strMsg);
							strMsg.PrintF(_S(5634, "(축적된 경험치 만큼 경험치를 획득하게 됩니다. 단, 재사용시 4시간의 대기시간이 필요합니다.)"));
							MsgBoxInfo.AddString(strMsg);

							pUIManager->CreateMessageBox(MsgBoxInfo);
						}
					}else
					{
						if (m_btnAIEdit.MouseMessage(pMsg) == WMSG_COMMAND)
						{
#ifndef DEL_PET_AI_EDIT
							if(m_bSlideUI)
							{
								PetAIReSet();
								m_bSlideUI = FALSE;
							}else m_bSlideUI = TRUE;
#endif
						}
						
						if (IsInsideRect(nX,nY,m_rtAIONOFF))
						{
							if (m_bAIActive)
							{
								SendAIActive(FALSE);
							}
							else
							{
								SendAIActive(TRUE);								
							}
						}

					
						for( int nPoint = 0; nPoint < POINT_END; nPoint++ )
						{
							if(m_btnPoint[nPoint].MouseMessage( pMsg ) == WMSG_COMMAND )
							{
								sendWildPetPointUP((UBYTE)nPoint);
								return WMSG_SUCCESS;
							}						
						}
					}
				}
				else if(m_nTebnum == WILDPET_SKILL_SLOT)				
				{
					if( ( m_sbSkillList.MouseMessage( pMsg ) ) != WMSG_FAIL )
						return WMSG_SUCCESS;

					for (int nSkillnum = 0; nSkillnum < 8; nSkillnum++)
					{
						if (m_pIconSkill[nSkillnum]->MouseMessage(pMsg) == WMSG_SUCCESS)
						{
							const int iSkillIndex = m_pIconSkill[nSkillnum]->getIndex();
							if (iSkillIndex != -1 && 
								MY_INFO()->GetPetSkillDelay(0, iSkillIndex) == false)
							{
								UseSkill( iSkillIndex );
							}
						}
					}
				}		

				if (m_nTebnum == WILDPET_INFO_SLOT)
				{
					if(m_btTabSlot[WILDPET_SKILL_SLOT].MouseMessage( pMsg ) == WMSG_COMMAND)
					{
						m_nTebnum = WILDPET_SKILL_SLOT;
						m_btTabSlot[WILDPET_INFO_SLOT].MouseMessage( pMsg );
					}
				}
				else if (m_nTebnum == WILDPET_SKILL_SLOT)
				{
					if(m_btTabSlot[WILDPET_INFO_SLOT].MouseMessage( pMsg ) == WMSG_COMMAND)
					{
						m_nTebnum = WILDPET_INFO_SLOT;
						m_btTabSlot[WILDPET_SKILL_SLOT].MouseMessage( pMsg );
					}
				}

				return WMSG_SUCCESS;
			}
			else if (IsInsideRect(nX,nY,UIRect(273,118,397,400)))
			{				
				if (m_btnAIMake.MouseMessage(pMsg) == WMSG_COMMAND)
				{
					SendAIList();
					return WMSG_SUCCESS;
				}
			}
		}break;
	case WM_LBUTTONDBLCLK:
		{
			if(IsInside(nX,nY))
			{
				if( m_nTebnum == WILDPET_INFO_SLOT )
				{
					for( int iWear = 0; iWear < WILDPET_WEAR_TOTAL; iWear++ )
					{
						if( m_pIconWearing[iWear]->MouseMessage( pMsg ) == WMSG_COMMAND )
						{
							// Close message box of inventory
							CUIManager::getSingleton()->CloseMessageBox( MSGCMD_DROPITEM );

							_pNetwork->SendUseWildPetItem(iWear,-1,-1,MY_APET_INFO()->m_nPetWearIndex[iWear]);
							
							return WMSG_SUCCESS;
						}
					}
				}
				else if( m_nTebnum == WILDPET_SKILL_SLOT )
				{
					if( ( m_sbSkillList.MouseMessage( pMsg ) ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}

				return WMSG_SUCCESS;
			}
		}break;
	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				if(m_nTebnum == WILDPET_SKILL_SLOT)
				{
					if( ( m_sbSkillList.MouseMessage( pMsg ) ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				return WMSG_SUCCESS;
			}
		}break;
	}
	return WMSG_FAIL;

}

BOOL CUIWildPetInfo::IsLearnSkill(int nSkillIndex )
{
	if (m_mapIconSkills.find(nSkillIndex) != m_mapIconSkills.end())
	{
		return TRUE;
	}
	
	return FALSE;
}

void CUIWildPetInfo::AddSkill(int nSkillIndex, SBYTE sbSkillLevel )
{
	// 새로운 스킬을 추가한다
	CUIIcon* pIcon = new CUIIcon();

	pIcon->initialize();
	pIcon->setData(UBET_SKILL, nSkillIndex);
	map_Icon::value_type tmpVal( nSkillIndex, pIcon ); 
	
	m_mapIconSkills.insert( tmpVal );
}

void CUIWildPetInfo::deleteSkill()
{
	map_Icon::iterator	iter;

	for (iter = m_mapIconSkills.begin(); iter != m_mapIconSkills.end(); ++iter)
	{
		delete (*iter).second;
		(*iter).second = NULL;
	}
}

void CUIWildPetInfo::SkillClear()
{
	deleteSkill();

	m_mapIconSkills.clear();

	for (int sSlot = 0; sSlot < 8; sSlot++)
	{
		m_pIconSkill[sSlot]->clearIconData();
	}
}

void CUIWildPetInfo::UseSkill( int nIndex )
{	
	if (m_bAIActive)
	{
		SendAIActive(FALSE);
	}

	// 스킬 사용 버그. [7/20/2010 rumist]	
	//UIButton_map::iterator	iter;	
	map_Icon::iterator	iterEnd;	
	map_Icon::iterator	iterSel;
	iterEnd	= m_mapIconSkills.end();
	iterSel	= m_mapIconSkills.find( nIndex );

	if( iterSel == iterEnd )
		return;

	LONG nSkillIndex = iterSel->second->getIndex();	
	CSkill& rSkill = _pNetwork->GetSkillData( nSkillIndex );
	ObjInfo* pInfo = ObjInfo::getSingleton();

	if (pInfo->GetMyApetInfo() == NULL)
		return;

	if( rSkill.GetType() == CSkill::ST_PET_SKILL_PASSIVE )
		return;

	// 실제 스킬 레벨은 0부터 시작.
	if (pInfo->GetMyApetInfo()->m_nMP < rSkill.GetNeedMP(MY_INFO()->GetPetSkillLevel(0, nSkillIndex) - 1))
	{
		CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( _S( 320, "MP가 부족합니다."), SYSMSG_ERROR );
		return;
	}

	if( pInfo->GetMyApetInfo()->m_pEntity ) // 예외 처리
	{
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->UseWildSkill( pInfo->GetMyApetInfo()->m_pEntity, nIndex );	
	}
	
}

// Author by RUMIST [12/20/2010 rumist]
BOOL CUIWildPetInfo::IsUsableSkill(LONG nSkillIndex )
{
	// 스킬 사용 버그. [7/20/2010 rumist]	
	//UIButton_map::iterator	iter;	
	map_Icon::iterator	iterEnd;	
	map_Icon::iterator	iterSel;
	iterEnd	= m_mapIconSkills.end();
	iterSel	= m_mapIconSkills.find( nSkillIndex );

	if( iterSel == iterEnd )
		return FALSE;

	LONG lSkillIndex = iterSel->second->getIndex();	
	CSkill& rSkill = _pNetwork->GetSkillData( lSkillIndex );
	// 실제 스킬 레벨은 0부터 시작.
	if (MY_APET_INFO()->m_nMP < rSkill.GetNeedMP(MY_INFO()->GetPetSkillLevel(0, lSkillIndex) - 1))
	{
		return FALSE;
	}
	
	return TRUE;
}

void CUIWildPetInfo::sendWildPetPointUP(UBYTE ubPoint)
{
	CNetworkMessage nmMessage(MSG_EXTEND);

	nmMessage << (ULONG)MSG_EX_ATTACK_PET;
	nmMessage << (SBYTE)MSG_SUB_STATUP;
	nmMessage << (UBYTE)ubPoint;

	_pNetwork->SendToServerNew(nmMessage);
}

void CUIWildPetInfo::sendSkillList()
{
	CNetworkMessage nmMessage(MSG_EXTEND);

	nmMessage << (ULONG)MSG_EX_ATTACK_PET;
	nmMessage << (SBYTE)MSG_SUB_SKILLLIST;

	_pNetwork->SendToServerNew(nmMessage);

}

void CUIWildPetInfo::PetWearItemReSet()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();

	for(int i = 0; i < WILDPET_WEAR_TOTAL; i++)
	{
		if(pInfo->GetMyApetInfo() != NULL && pInfo->GetMyApetInfo()->m_nPetWearIndex[i] < 0)
		{
			m_pIconWearing[i]->clearIconData();
		}
	}
}

void CUIWildPetInfo::ReceiveWearItemData(CNetworkMessage *istr)
{
	SBYTE nWearpos;
	ULONG nItemIndex, nItemPlus, nItemPlus2 = 0;
	ULONG nFlag,nUsed,nUsed2;
	SBYTE	sbOptionCount;
	SBYTE	sbOptionType;
	LONG	lOptionLevel;

	(*istr) >> nWearpos;
	(*istr) >> nItemIndex;
	(*istr) >> nItemPlus;
	(*istr) >> nFlag;
	(*istr) >> nUsed;
	(*istr) >> nUsed2;
	(*istr) >> sbOptionCount;

// 	m_pIconWearing[nWearpos].SetItemInfo( -1, -1, nItemIndex, -1, nWearpos );
// 	m_pIconWearing[nWearpos].SetItemIndex(nItemIndex);
// 	m_pIconWearing[nWearpos].SetItemPlus(nItemPlus);
// 	m_pIconWearing[nWearpos].SetItemFlag(nFlag);
// 	m_pIconWearing[nWearpos].SetItemUsed(nUsed);
// 	m_pIconWearing[nWearpos].SetItemUsed2(nUsed2);

	CItemData* pItemData = _pNetwork->GetItemData(nItemIndex);

	CItems* pItems = new CItems;
	pItems->Item_Index = nItemIndex;
	pItems->Item_Plus = nItemPlus;
	pItems->Item_Wearing = nWearpos;
	pItems->Item_Used = nUsed;
	pItems->Item_Used2 = nUsed2;

	pItems->ItemData = CItemData::getData(nItemIndex);

	//레어 아이템일때...
	if( pItemData->GetFlag() & ITEM_FLAG_RARE )
	{
		//옵션 개수가 0이면 미감정 레어아이템
		if( sbOptionCount ==0)
		{
			pItems->SetRareIndex(0);
		}		
		else
		{
			//감정된 레어아이템이면...
			CUIManager::getSingleton()->SetRareOption(istr, *pItems);
		}
	}
	//레어 아이템이 아니면.....
	else
	{
		LONG lOriginOptionVar = ORIGIN_VAR_DEFAULT;

		for( SBYTE sbOption = 0; sbOption < sbOptionCount; sbOption++ )
		{
			(*istr) >> sbOptionType;
			(*istr) >> lOptionLevel;

			if ( pItemData->GetFlag() & ITEM_FLAG_ORIGIN )
			{
				(*istr) >> lOriginOptionVar;
			}

			pItems->SetOptionData(sbOption, sbOptionType, lOptionLevel, lOriginOptionVar);
		}
	}

	if ( pItemData->GetFlag() & ITEM_FLAG_ORIGIN)
	{
		SBYTE sbBelong, sbSkillcont;
		LONG lSkillIndex;
		SBYTE sbSkillLevel;

		(*istr) >> sbBelong;
		(*istr) >> sbSkillcont;

		pItems->Item_Belong = sbBelong;

		for (SBYTE sbSkillpos = 0; sbSkillpos < sbSkillcont; sbSkillpos++)
		{
			(*istr) >> lSkillIndex;
			(*istr) >> sbSkillLevel;

			pItems->SetItemSkill(sbSkillpos, lSkillIndex, sbSkillLevel);
		}
		
	}

	(*istr) >> nItemPlus2;
	pItems->SetItemPlus2(nItemPlus2);
	
	m_pIconWearing[nWearpos]->setData(pItems, false);
}

void CUIWildPetInfo::AddWildPetInfo( sPetItem_Info sPetInfo )
{
	sPetItem_Info TempData;
	TempData = sPetInfo;
	int nPetIndex = TempData.pet_index;

	WildPetInfo_map::iterator iter = m_mapWildPetInfo.find( nPetIndex );
	
	if( iter != m_mapWildPetInfo.end() )	//이미 가지고 있는 펫 데이터이면 업데이트...
		iter->second =TempData;
	else										//가지고 있지 않는 펫 데이터이면 추가...
	{
		WildPetInfo_map::value_type tmpVal( nPetIndex, TempData );
		m_mapWildPetInfo.insert( tmpVal );
	}
}
BOOL CUIWildPetInfo::GetWildPetInfo(int nPetIndex, sPetItem_Info& WildPetInfo )
{
	WildPetInfo_map::iterator PetInfoiter = m_mapWildPetInfo.find( nPetIndex );

	if( PetInfoiter != m_mapWildPetInfo.end() )
	{
		WildPetInfo = PetInfoiter->second;
		return TRUE;
	}else return FALSE;
}

void CUIWildPetInfo::AIPopupRender()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData(m_ptdBaseTexture);
	
	m_rtsAIBace.SetPos(m_nPosX,m_nPosY);
	m_rtsAIBace.RenderRectSurface(pDrawPort,0xFFFFFFFF);

	//Text
	CTString strTemp;
	strTemp.PrintF(_S(4373, "펫 AI 설정"));
	pDrawPort->PutTextExCX(strTemp, m_nPosX+335, m_nPosY+123);

	pDrawPort->FlushRenderingQueue();

	int nAISot;
	for( nAISot = 0; nAISot < m_nAISlotCount; nAISot++)
	{	
		if (m_pIconAITarget[nAISot]->IsEmpty() == false)
		{
			m_pIconAITarget[nAISot]->Render(pDrawPort);
		}
		
	}
	
	for( nAISot = 0; nAISot < m_nAISlotCount; nAISot++)
	{
		if (m_pIconAIAction[nAISot]->getBtnType() == UBET_SKILL && 
			m_pIconAIAction[nAISot]->IsEmpty() == false)
			m_pIconAIAction[nAISot]->Render(pDrawPort);
	}

	for( nAISot = 0; nAISot < m_nAISlotCount; nAISot++)
	{
		if (m_pIconAIAction[nAISot]->getBtnType() == UBET_ACTION && 
			m_pIconAIAction[nAISot]->IsEmpty() == false)
			m_pIconAIAction[nAISot]->Render(pDrawPort);
	}

	pDrawPort->InitTextureData(m_ptdButtonTexture);

	for( nAISot = 0; nAISot < m_nAISlotCount; nAISot++ )
	{
		m_cbtnAISlotActive[nAISot].Render();
	}

	m_btnAIMake.Render();	

	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();	
}

void CUIWildPetInfo::AddAIData(int nSlotnum, sWildPet_AI_Slot sAIData)
{
	m_sAISlot[nSlotnum] = sAIData;
	
	m_cbtnAISlotActive[nSlotnum].SetCheck(m_sAISlot[nSlotnum].m_bActive);

}

void CUIWildPetInfo::ErrorMassage(UBYTE errorcode)
{
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;

	switch(errorcode)
	{
	case MSG_APET_ERROR_OK:
		{
		//	strMessage = _S(1970, "성공");
			PetAIReSet();
			AIPopupClose();
			return;
		}break;
	case MSG_APET_ERROR:
		{
			strMessage = _S(1971, "실패");
		}break;
	case MSG_APET_ERR_NOT_HAVE_ITEM:
		{
			strMessage = _S(2907, "이용 가능한 아이템이 아닙니다.");

		}break;
	case MSG_APET_ERR_WRONG_AI:
		{
			strMessage = _S(318, "타겟이 잘못되었습니다.");

		}break;
	}

	PetAIReSet();

	MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_WILDPET_INFO, UI_NONE );
	MsgBoxInfo.AddString( strMessage );
	CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
}

void CUIWildPetInfo::SendAIList()
{
	if (m_nAISlotCount <= 0)
	{
		return;
	}

	int temSlotcount = 0;
	INDEX nSlot;
	for( nSlot = 0; nSlot < m_nAISlotCount; nSlot++)
	{
		if (m_pIconAITarget[nSlot]->IsEmpty() == false && 
			m_pIconAIAction[nSlot]->IsEmpty() == false)
		{
			temSlotcount++;
		}
	}

	CNetworkMessage nmMessage(MSG_EXTEND);

	nmMessage << (ULONG)MSG_EX_ATTACK_PET;
	nmMessage << (SBYTE)MSG_SUB_AI_UPDATE_REQ;
	nmMessage << m_nAISlotCount;
	nmMessage << (INDEX)temSlotcount;

	for( nSlot = 0; nSlot < m_nAISlotCount; nSlot++)
	{
		if (m_pIconAITarget[nSlot]->IsEmpty() == true && 
			m_pIconAIAction[nSlot]->IsEmpty() == true)
		{
			continue;
		}
		
		nmMessage << nSlot;
		nmMessage << (ULONG)m_pIconAITarget[nSlot]->getIndex();
		

		if (m_pIconAIAction[nSlot]->getBtnType() == UBET_SKILL ||
			m_pIconAIAction[nSlot]->getBtnType() == UBET_ACTION)
		{
			nmMessage << (ULONG)m_pIconAIAction[nSlot]->getBtnType();
			nmMessage << (ULONG)m_pIconAIAction[nSlot]->getIndex();
		}
		else
		{
			nmMessage << (ULONG) -1;
			nmMessage << (ULONG) -1;
		}
		
		UBYTE bCheckd;
		if (m_cbtnAISlotActive[nSlot].IsChecked())
		{
			bCheckd = TRUE;
		}else bCheckd = FALSE;

		nmMessage << bCheckd;
	}


	_pNetwork->SendToServerNew(nmMessage);
}

void CUIWildPetInfo::SendAIActive(SBYTE bActive)
{
	CNetworkMessage nmMessage(MSG_EXTEND);

	nmMessage << (ULONG)MSG_EX_ATTACK_PET;
	nmMessage << (SBYTE)MSG_SUB_AI_ONOFF_REQ;
	nmMessage << bActive;

	_pNetwork->SendToServerNew(nmMessage);
}

void CUIWildPetInfo::PetAIReSet()
{
	for (int nSlot = 0; nSlot < WILDPET_AI_MAXSLOT; nSlot++)
	{
		m_cbtnAISlotActive[nSlot].SetCheck(FALSE);
		m_pIconAITarget[nSlot]->clearIconData();
		m_pIconAIAction[nSlot]->clearIconData();

		m_cbtnAISlotActive[nSlot].SetCheck(m_sAISlot[nSlot].m_bActive);

		if (m_sAISlot[nSlot].m_nTargetSlot > 0)
		{
			m_pIconAITarget[nSlot]->setData(UBET_ITEM, m_sAISlot[nSlot].m_nTargetSlot);
		}

		if (m_sAISlot[nSlot].m_nActionSlot > 0)
		{
			if(m_sAISlot[nSlot].m_nSillType == UBET_SKILL)
			{
				if(IsLearnSkill(m_sAISlot[nSlot].m_nActionSlot))
				{
					m_pIconAIAction[nSlot]->setData(UBET_SKILL, m_sAISlot[nSlot].m_nActionSlot);
				}
			}else
			{
				m_pIconAIAction[nSlot]->setData(UBET_ACTION, m_sAISlot[nSlot].m_nActionSlot);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//	RemoveSlot
//	Hold Btn정보로 슬롯의 Button을 초기화 한다.
//////////////////////////////////////////////////////////////////////////
void CUIWildPetInfo::RemoveSlot(CUIIcon* pIcon)
{
	switch(pIcon->getBtnType())
	{
	case UBET_ITEM:
		{
			for (int i=0; i<WILDPET_AI_MAXSLOT; ++i)
			{
				if (m_pIconAITarget[i]->getIndex() == pIcon->getIndex())
				{
					m_pIconAITarget[i]->clearIconData();
					break;
				}
			}
		}
		break;
	case UBET_SKILL:
	case UBET_ACTION:
		{
			for (int i=0; i<WILDPET_AI_MAXSLOT; ++i)
			{
				if (m_pIconAIAction[i]->getIndex() == pIcon->getIndex())
				{
					m_pIconAIAction[i]->clearIconData();
					break;
				}
			}
		}
		break;
	}
}

void CUIWildPetInfo::AccumulateInfoRender()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_rvAccuback.Render(m_nPosX,m_nPosY);
	
	//Text
	CTString strTemp;
	ObjInfo* pInfo = ObjInfo::getSingleton();

	strTemp.PrintF(_S(5635, "경험치 축적 정보"));
	pDrawPort->PutTextEx(strTemp, m_nPosX+45, m_nPosY+220, 0xffb64bFF);

	strTemp.PrintF(_S(5636, "경험치 축적 률"));
	pDrawPort->PutTextEx(strTemp, m_nPosX+45, m_nPosY+242);

	pDrawPort->AddTexture(m_nPosX+45,m_nPosY+260,m_nPosX+246,m_nPosY+275,
		m_rvAccublank.U0,m_rvAccublank.V0,m_rvAccublank.U1,m_rvAccublank.V1, 0xFFFFFFFF);
	
	FLOAT ntempresult = m_Petdata->accexpData[0].nAccRateParam1 + (pInfo->GetMyApetInfo()->m_nLevel * m_Petdata->accexpData[0].nAccRateParam2)/100.0f ;
	strTemp.PrintF("%.2f%%", ntempresult);
		
	pDrawPort->PutTextExCX(strTemp, m_nPosX+150, m_nPosY+260);
		

	strTemp.PrintF(_S(5637, "경험치 축적 가능량"));
	pDrawPort->PutTextEx(strTemp, m_nPosX+45, m_nPosY+282);

	pDrawPort->AddTexture(m_nPosX+45,m_nPosY+300,m_nPosX+246,m_nPosY+315,
		m_rvAccublank.U0,m_rvAccublank.V0,m_rvAccublank.U1,m_rvAccublank.V1, 0xFFFFFFFF);

	FLOAT temresult = (float)m_Petdata->accexpData[0].nMaxAccParam1 + (float)(pInfo->GetMyApetInfo()->m_nLevel * m_Petdata->accexpData[0].nMaxAccParam2) / 100.0f;
	// 부동소수점 연산으로 인한 계산오차가 발생하므로 연산식 수정 [1/13/2012 ldy1978220]
	SQUAD lAccuresult = _pNetwork->MyCharacterInfo.needExp * (int)(temresult * 100) / 10000;
	
	strTemp.PrintF("%I64d",lAccuresult);

	pDrawPort->PutTextExCX(strTemp, m_nPosX+150, m_nPosY+300);

	strTemp.PrintF(_S(5638, "축척 된 경험치 량"));
	pDrawPort->PutTextEx(strTemp, m_nPosX+45, m_nPosY+322);

	pDrawPort->AddTexture(m_nPosX+45,m_nPosY+340,m_nPosX+246,m_nPosY+355,
		m_rvAccublank.U0,m_rvAccublank.V0,m_rvAccublank.U1,m_rvAccublank.V1, 0xFFFFFFFF);

	sPetItem_Info temPetitem;
	GetWildPetInfo(pInfo->GetMyApetInfo()->m_nIdxServer, temPetitem);

	strTemp.PrintF("%I64d", temPetitem.pet_accexp);
	pDrawPort->PutTextExCX(strTemp, m_nPosX+150, m_nPosY+340);
		
	// Render all elements
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();

	pDrawPort->InitTextureData(m_ptdButtonTexture);

	if(IsWildPetCoolTime())
	{
		strTemp.PrintF(_S(5644, "쿨타임"));

		pDrawPort->PutTextExRX(strTemp, m_nPosX + 250, m_nPosY + 190, 0xFF2626FF);

		INDEX temCooltime = temPetitem.pet_cooltime - (unsigned int)_pTimer->GetLerpedCurrentTick();

		int iSec = temCooltime % 60;
		temCooltime /= 60;
		
		int iMin = temCooltime % 60;
		int iHour = temCooltime /= 60;
		CTString temtime;

		strTemp.Clear();

		if (iHour > 0)
		{
			temtime.PrintF(_S(2512,"%d시간"),iHour);
			strTemp += temtime;
		}
		if (iMin > 0)
		{
			temtime.PrintF(_S(2513, "%d분"),iMin);
			strTemp += temtime;
		}
		if (iSec > 0)
		{
			temtime.PrintF(_S(2514,"%d초"),iSec);
			strTemp += temtime;
	
		}
		pDrawPort->PutTextExRX(strTemp, m_nPosX + 250, m_nPosY + 220, 0xFF2626FF);
	}else
	{
		if (temPetitem.pet_accexp > 0)
		{
			if (m_btnAccumulateUse.GetBtnState() == UBS_DISABLE)
			{
				m_btnAccumulateUse.SetEnable(TRUE);
			}
			
		}else
		{
			if (m_btnAccumulateUse.GetBtnState() != UBS_DISABLE)
			{
				m_btnAccumulateUse.SetEnable(FALSE);
			}
			
		}
		m_btnAccumulateUse.Render();
	}
	pDrawPort->FlushRenderingQueue();

	pDrawPort->EndTextEx();

}

BOOL CUIWildPetInfo::IsWildPetCoolTime()
{
	sPetItem_Info temPetitem;
	GetWildPetInfo(MY_APET_INFO()->m_nIdxServer, temPetitem);

	if (temPetitem.pet_cooltime/* - (unsigned int)_pTimer->GetLerpedCurrentTick()*/ > 0)
	{
		return TRUE;
	}
	return FALSE;

}

BOOL CUIWildPetInfo::CheckAIItem( CItemData* pAIItem, int nSlot )
{
	if (pAIItem == NULL)
		return FALSE;

	if (m_pIconAIAction[nSlot]->IsEmpty() != false)
		return TRUE;
	
	int nTargetType = pAIItem->GetPetAISet3();
	int nActionType = GetActionAIType(m_pIconAIAction[nSlot]);

	return CheckAIType(nTargetType, nActionType);
}

BOOL CUIWildPetInfo::CheckAIAction( CUIIcon* pIcon, int nSlot )
{
	if (pIcon == NULL)
		return FALSE;

	if (m_pIconAITarget[nSlot]->IsEmpty() != false)
		return TRUE;

	CItemData* pCurItem = _pNetwork->GetItemData(m_pIconAITarget[nSlot]->getIndex());

	int nTargetType = pCurItem->GetPetAISet3();
	int nActionType = GetActionAIType(pIcon);
	
	return CheckAIType(nTargetType, nActionType);
}

BOOL CUIWildPetInfo::CheckAIType( int nAIType, int nActionType )
{
	switch (nAIType)
	{
	case CItemData::PET_AI_OWNER_ATTACK_ENEMY:// 주인이 공격하는 적
	case CItemData::PET_AI_ENEMY_ATTACK_OWNER:// 주인을 공격하는 적
	case CItemData::PET_AI_ENEMY_ATTACK_PET:// 펫을 공격하는 적
	case CItemData::PET_AI_ENEMY:// 적
		{
			if (nActionType == AI_TARGET_ENEMY)
				return TRUE;
		}
		break;
	case CItemData::PET_AI_PET:// 펫
		{
			if (nActionType == AI_TARGET_PET)
				return TRUE;
		}
		break;
	case CItemData::PET_AI_OWNER:// 주인
		{
			if (nActionType == AI_TARGET_OWNER)
				return TRUE;
		}
		break;
	case CItemData::PET_AI_ITEM:// 아이템
		{
			if (nActionType == AI_TARGET_ITEM)
				return TRUE;
		}
		break;
	}

	return FALSE;
}

int CUIWildPetInfo::GetActionAIType( CUIIcon* pIcon )
{
	if (pIcon == NULL)
		return AI_NONE;

	if(pIcon->getBtnType() == UBET_SKILL)
	{
		CSkill SkillData = _pNetwork->GetSkillData(pIcon->getIndex());

		if(SkillData.IsNeedTarget() == TRUE)
		{
			BOOL bHelp = SkillData.GetFlag() & SF_FORHELP;

			if (bHelp != FALSE)
				return AI_TARGET_OWNER;
			else
				return AI_TARGET_ENEMY;
		}
		else
		{
			return AI_TARGET_PET;
		}
	}
	else if (pIcon->getBtnType() == UBET_ACTION)
	{
		CAction* curAction =	CAction::getData(pIcon->getIndex());

		if (curAction == NULL)
			return AI_NONE;

		switch(curAction->GetIndex())
		{
		case 44/*[P2]공격*/:
			return AI_TARGET_ENEMY;
		case 45/*[P2]줍기*/:
			return AI_TARGET_ITEM;
		}
	}

	return AI_NONE;
}

void CUIWildPetInfo::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	int		i;
	int		nIndex;
	bool	bDelay;

	for (i = 0; i < WILDPET_AI_MAXSLOT; ++i)
	{
		if (m_pIconAIAction[i]->IsEmpty() == false)
		{
			nIndex = m_pIconAIAction[i]->getIndex();
			bDelay = MY_INFO()->GetPetSkillDelay(0, nIndex);

			if (bDelay)
				m_pIconAIAction[i]->setCooltime(true);
		}
	}
}
