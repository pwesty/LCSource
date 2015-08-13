
#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIManager.h>
#include "UIIcon.h"
#include <Common/header/def_item_collection.h>
#include <Engine/Info/MyInfo.h>
#include <Engine/Help/ItemHelp.h>
//#include <Engine/Interface/UIGuildMark.h>

#define DEF_HIGHLIGHT_TIME 500
#define DEF_DISABLE_SKILL_OFFSET	3
#define	COMBO_BTN_SIZE	52

CUIIcon::CUIIcon()
	: m_pItems(NULL)
	, m_pItemEtc(NULL)
	, m_nIndex(-1)
	, m_nSkillDisable(0)
	, m_color(DEF_UI_COLOR_WHITE)
	, m_TexID(-1)
	, m_bRef(true)
	, m_pAttribute(NULL)
	, m_pStack(NULL)
	, m_pStackError(NULL)
	, m_pPlus(NULL)
	, m_pJewelGrade(NULL)
	, m_bCooltime(false)
	, m_BtnType(UBET_TYPE_MAX)
	, m_BtnEtctype(eETC_TYPE_NONE)
	, m_pToggle(NULL)
	, m_bWearTab(false)
	, m_bSpecialSkill(false)
	, m_eWhichUI(UI_NONE)
	, m_nQuickSlotID(-1)
	, m_nStartTime(0)
	, m_nQuickSlotWearType(0)
	, m_nCashIndex(-1)
	, m_bSuit(false)
	, m_nPetKind(-1)
{
#ifdef UI_TOOL
	m_TexID	   = 0;
	m_nRow	   = 0;
	m_nCol	   = 0;
#endif // UI_TOOL
	m_nSize	   = 0;
	m_pCmdDrag = NULL;
	m_bSelectIcon = FALSE;
	m_bCustomShowInfo = false;
	setType(eUI_CONTROL_ICON);
	m_bHighlightMode = false;
	m_bHighlight = false;
	m_dwAniStartTime = 0;

#ifdef DURABILITY
	m_bDurItem = false;
#endif	//	DURABILITY

	for (int i = 0; i < eIMG_TYPE_MAX; ++i)
		m_pImage[i] = NULL;

	setInherit(false);
}

CUIIcon::~CUIIcon()
{
	Destroy();
	SAFE_DELETE(m_pCmdDrag);

	clearIconData();	
}


void CUIIcon::initialize()
{
}

void CUIIcon::Create( CUIWindow* pParent, int x, int y, int width, int height, int nWhich, int nBtnType, int nQuickID )
{
	initialize();
	InitPos(x, y, width, height);
	setParent(pParent);
	SetWhichUI(nWhich);
	m_BtnType = (UIBtnExType)nBtnType;
	m_nQuickSlotID = nQuickID;
}

void CUIIcon::setInfo( UIBtnExType type, SBYTE cTexID, int row, int col )
{
#ifdef UI_TOOL
	m_nRow	   = row;
	m_nCol     = col;
#endif // UI_TOOL

	m_BtnType	= type;
	m_TexID		= cTexID;

	setIconUV(row, col);
}

void CUIIcon::setData( CItems* pItems, bool bRef )
{
	if (pItems == NULL)
		return;

	if (m_bRef == false)
		SAFE_DELETE(m_pItems);

	m_nIndex = pItems->Item_Index;
	m_pItems = pItems;
	m_bRef = bRef;

	m_BtnType = UBET_ITEM;
	m_BtnEtctype = eETC_TYPE_NONE;
	m_TexID = m_pItems->ItemData->GetIconTexID();

	int nRow = m_pItems->ItemData->GetIconTexRow();
	int nCol = m_pItems->ItemData->GetIconTexCol();

	setIconUV(nRow, nCol);

	CItemData*	pItemData = m_pItems->ItemData;

	// 플러스 수치 표시
	{
		if (pItemData != NULL && pItemData->GetItemIndex() > 0)
		{
			if (m_pItems->Item_Plus > 0 && 
				((pItemData->GetType() == CItemData::ITEM_ETC && 
					pItemData->GetSubType() != CItemData::ITEM_ETC_MONSTER_MERCENARY_CARD && 
					pItemData->GetSubType() != 10) || 
				 (pItemData->GetType() == CItemData::ITEM_ONCEUSE && 
					pItemData->GetSubType() != CItemData::ITEM_ETC_EVENT &&
					pItemData->GetSubType() != CItemData::ITEM_SUB_BOX && 
					pItemData->GetSubType() != CItemData::ITEM_SUB_ETC)))
			{ 
				// 월드컵 카드로 인해 조건 추가, 일회용 상자 제외
				if (m_pPlus == NULL)
				{
					m_pPlus = UISUPPORT()->clone_txt_plus();
					addChild(m_pPlus);
				}

				CTString tv_plus;
				tv_plus.PrintF("+%d", m_pItems->Item_Plus);

				m_pPlus->SetText(tv_plus);
				m_pPlus->Hide(FALSE);
			}
			else
			{
				if (m_pPlus != NULL)
					m_pPlus->Hide(TRUE);
			}
		}		
	}

	// 보석 등급 추가
	{
	    if (pItemData->GetType() == CItemData::ITEM_ETC &&
			(pItemData->GetSubType() == CItemData::ITEM_ETC_JEWEL ||
			 pItemData->GetSubType() == CItemData::ITEM_ETC_CHAOSJEWEL))
	    {
	        int nGrade = pItemData->GetNum0() - 1;

			if (m_pJewelGrade != NULL)
				deleteChild(m_pJewelGrade);
			
			m_pJewelGrade = UISUPPORT()->clone_jewelgrade(nGrade);
			addChild(m_pJewelGrade);

			m_pJewelGrade->Hide(FALSE);
	    }
		else
		{
			if (m_pJewelGrade != NULL)
				m_pJewelGrade->Hide(TRUE);
		}
	}

	// Stack
	setStack();

	// 봉인
	{
		if (pItems->IsFlag(FLAG_ITEM_COMPOSITION) && pItemData->IsFlag(ITEM_FLAG_COMPOSITE) == false)
			setImage(eIMG_TYPE_COMPOSITION, true);
		else
			setImage(eIMG_TYPE_COMPOSITION, false);
	}

	// 대여
	{
		if (pItems->IsFlag(FLAG_ITEM_LENT))
			setImage(eIMG_TYPE_RENT, true);
		else
			setImage(eIMG_TYPE_RENT, false);
	}

	if (pItemData->IsFlag(ITEM_FLAG_TOGGLE))
	{
		if (m_pToggle == NULL)
		{
			m_pToggle = UISUPPORT()->clone_toggle();
			addChild(m_pToggle);
		}
	}

	// 내구도
#ifdef DURABILITY
	{
		if ( pItemData->IsFlag( ITEM_FLAG_DURABILITY) )
		{
			setImage(eIMG_TYPE_DURABILITY, true);
			setImageHide(eIMG_TYPE_DURABILITY, pItems->IsZeroDurability() ? FALSE : TRUE);
			m_bDurItem = true;
		}
		else
		{
			setImage(eIMG_TYPE_DURABILITY, false);
			m_bDurItem = false;
		}
	}
#endif	//	DURABILITY

	// 사용 제한 추가
	if ( pItemData->IsFlag( ITEM_FLAG_QUEST) )
		setImage(eIMG_TYPE_RESTRITION, true);
	else
		setImage(eIMG_TYPE_RESTRITION, false);
	
	SetToggle(pItems->GetToggle());
}

void CUIIcon::setDataEtc( eUIBTN_ETC_TYPE type, int nIndex, SQUAD llCount )
{
	m_TexID = 0;
	int nTexRow = 0;
	int nTexCol = 0;

	m_BtnType = UBET_ITEM;
	m_BtnEtctype = type;

	if (m_bRef == false)
	{
		SAFE_DELETE(m_pItemEtc);
	}

	m_bRef = false;
	m_pItemEtc = new stItemEtc;
	m_pItemEtc->nIndex = nIndex;
	m_pItemEtc->llEtc = llCount;

	switch(type)
	{
	case eETC_TYPE_NAS:
		{
			// 나스는 0, 0, 0
			m_TexID = 0;
			nTexRow = 0;
			nTexCol = 0;
		}
		break;
	case eETC_TYPE_EXP:
		{
			m_TexID = 1;
			nTexRow = 15;
			nTexCol = 5;
		}
		break;
	case eETC_TYPE_SP:
		{
#if defined (G_RUSSIA)
			m_TexID = 12;
			nTexRow = 0;
			nTexCol = 2;
#else
			m_TexID = 9;
			nTexRow = 12;
			nTexCol = 11;
#endif			
		}
		break;
	case eETC_TYPE_THEME:
		{
			m_TexID = 19;
			nTexRow = 7;
			nTexCol = 10;
		}
		break;
	}

	setIconUV(nTexRow, nTexCol);
}

void CUIIcon::setData( UIBtnExType type, int nIndex, int nVIndex )
{
	switch (type)
	{
	case UBET_ITEM:
		setItem(nIndex, nVIndex);
		break;
	case UBET_SKILL:
		setSkill(nIndex);
		break;
	case UBET_QUEST:
		setQuest(nIndex);
		break;
	case UBET_ACTION:
		setAction(nIndex);
		break;
	case UBET_COMBO:
		setMCombo(nIndex);
		break;
	}
}

void CUIIcon::setSkill( int nSkill_idx, bool bSpecial, bool bDisable )
{
	if (m_nIndex == nSkill_idx)
		return;

	int		nRow = 0, nCol = 0;

	m_nIndex = nSkill_idx;
	m_BtnType = UBET_SKILL;
	m_bSpecialSkill = bSpecial;

	if (bDisable == true)
		m_nSkillDisable = DEF_DISABLE_SKILL_OFFSET;
	else
		m_nSkillDisable = 0;

	if (m_bSpecialSkill == false)
	{
		CSkill& rSkill = _pNetwork->GetSkillData(m_nIndex);

		m_TexID = rSkill.GetIconTexID();
		nRow = rSkill.GetIconTexRow();
		nCol = rSkill.GetIconTexCol();

		int nLv = MY_INFO()->GetSkillLevel(nSkill_idx);

		if (nLv > 0)
		{
			int attr_attack = rSkill.GetAttrAtt(nLv - 1);
			int attr_defence = rSkill.GetAttrDef(nLv - 1);

			if (attr_attack > 0)
			{
				m_pAttribute = UISUPPORT()->clone_attribute(attr_attack, true);
				addChild(m_pAttribute);
			}
			else if (attr_defence > 0)
			{
				m_pAttribute = UISUPPORT()->clone_attribute(attr_defence, false);
				addChild(m_pAttribute);
			}
		}

		// toggle
		{
			if (rSkill.GetFlag() & SF_TOGGLE)
			{
				if (m_pToggle == NULL)
				{
					m_pToggle = UISUPPORT()->clone_toggle();
					addChild(m_pToggle);
					m_pToggle->Hide(TRUE);
				}
			}
			else
			{
				if (m_pToggle != NULL)
					SetToggle(false);
			}
		}

		m_nPetKind = rSkill.Skill_Data.job;
	}
	else
	{
		CSpecialSkill*	pSkill = CSpecialSkill::getData(m_nIndex);

		m_TexID = pSkill->GetIconTexID();
		nRow = pSkill->GetIconTexRow();
		nCol = pSkill->GetIconTexCol();
	}

	setIconUV(nRow, nCol);
}

void CUIIcon::setCash( int nCashIdx )
{
	m_BtnType = UBET_ITEM;

	if (m_bRef == false)
	{
		SAFE_DELETE(m_pItems);
	}

	m_pItems = new CItems;
	m_bRef = false;

	int tv_index = _pNetwork->GetCashShopData().GetCashShopData(nCashIdx)->m_vItemDataArray[0].m_itemIndex;
	// Set texture data
	CItemData*	pItemData = _pNetwork->GetItemData( tv_index );	
	m_nIndex = tv_index;
	m_pItems->Item_Index = tv_index;
	m_pItems->ItemData = pItemData;

	m_TexID = pItemData->GetIconTexID();
	int	nTexRow = pItemData->GetIconTexRow();
	int	nTexCol = pItemData->GetIconTexCol();

	setIconUV(nTexRow, nTexCol);

	m_nCashIndex = nCashIdx;
}

void CUIIcon::copyItem( CUIIcon* pIcon )
{
	if (pIcon == NULL)
		return;

	switch (pIcon->getBtnType())
	{
	case UBET_ITEM:
		{
			if (pIcon->isRef() == false)
			{
				CItems* pCopy = new CItems;
				memcpy(pCopy, pIcon->getItems(), sizeof(CItems));

				setData(pCopy, false);
			}
			else
			{
				setData(pIcon->getItems());
			}
		}
		break;
	default:
		{
			setData(pIcon->getBtnType(), pIcon->getIndex());
		}
		break;
	}

	m_nCashIndex = pIcon->GetCashIndex();
}

void CUIIcon::cloneItem( CUIIcon* pIcon )
{
	if (pIcon == NULL)
		return;


	switch (pIcon->getBtnType())
	{
	case UBET_ITEM:
		{
			CItems* pCopy = new CItems;
			memcpy(pCopy, pIcon->getItems(), sizeof(CItems));

			setData(pCopy, false);
		}
		break;
	default:
		{
			setData(pIcon->getBtnType(), pIcon->getIndex());
		}
		break;
	}
}

void CUIIcon::setImage( eIMG_TYPE eType, bool bOn )
{
	if (bOn == true)
	{
		if (m_pImage[eType] == NULL)
		{
			m_pImage[eType] = UISUPPORT()->clone_Image(eType);
			addChild(m_pImage[eType]);
		}

		m_pImage[eType]->Hide(FALSE);		
	}
	else
	{
		if (m_pImage[eType] != NULL)
			deleteChild(m_pImage[eType]);

		m_pImage[eType] = NULL;
	}
}

void CUIIcon::setImageHide( eIMG_TYPE eType, BOOL bHide )
{
	if (m_pImage[eType] != NULL)
		m_pImage[eType]->Hide(bHide);
}

void CUIIcon::clearIconData()
{
	// 데이터 초기화 시 툴팁에 있다면, 삭제한다.
	HideTooltip();

	m_nIndex = -1;
	m_nCashIndex = -1;

	m_bSelectIcon = FALSE;
	m_bHighlightMode = false;
	m_bHighlight = false;
	m_bCooltime = false;
#ifdef DURABILITY
	m_bDurItem = false;
#endif	//	DURABILITY

	m_TexID = -1;
	m_nSkillDisable = 0;
	
	switch (m_BtnType)
	{
	case UBET_ITEM:
		{
			if (m_bRef == false)
			{
				SAFE_DELETE(m_pItems);
				m_bRef = true;
			}
			else
			{
				m_pItems = NULL;
			}

			m_TexID = -1;

			if (m_pStack != NULL)
			{
				deleteChild(m_pStack);
				m_pStack = NULL;
			}

			if (m_pStackError != NULL)
			{
				deleteChild(m_pStackError);
				m_pStackError = NULL;
			}

			if (m_pPlus != NULL)
			{
				deleteChild(m_pPlus);
				m_pPlus = NULL;
			}

			if (m_pJewelGrade != NULL)
			{
				deleteChild(m_pJewelGrade);
				m_pJewelGrade = NULL;
			}

			for (int i = 0; i < eIMG_TYPE_MAX; ++i)
			{
				if (m_pImage[i] != NULL)
				{
					deleteChild(m_pImage[i]);
					m_pImage[i] = NULL;
				}
			}

			SAFE_DELETE(m_pItemEtc);
			m_BtnEtctype = eETC_TYPE_NONE;
		}
		break;

	case UBET_SKILL:
		{
			if (m_pAttribute != NULL)
			{
				deleteChild(m_pAttribute);
				m_pAttribute = NULL;
			}

			if (m_pToggle != NULL)
			{
				deleteChild(m_pToggle);
				m_pToggle = NULL;
			}

			m_nPetKind = -1;
		}
		break;

	case UBET_COMBO:
		{
			if (m_bRef == false)
			{
				SAFE_DELETE(m_pItems);
				m_bRef = true;
			}
			else
			{
				m_pItems = NULL;
			}
		}
		break;
	}

	m_BtnType = UBET_TYPE_MAX;
}

// --------------------------------------------------------------------

void CUIIcon::OnRender( CDrawPort* pDraw )
{
	int	nX, nY;
	
	GetAbsPos( nX, nY );

	if (m_BtnType < UBET_SKILL || m_BtnType >= UBET_TYPE_MAX)
		return;

	switch (m_BtnType)
	{
	case UBET_ITEM:
	case UBET_SKILL:
		{
			if (m_TexID > -1)
			{
				if (m_bCooltime == false)
					renderDefault(nX, nY, pDraw);
				else
					renderCooltime(nX, nY, pDraw);

				//if( m_bSkillDelay && ( !m_bSpecial || m_betType == UBET_ITEM ) )
			}
		}
		break;
	default:
		renderDefault(nX, nY, pDraw);
		break;
	}

	if (m_bHighlight)
		renderDefault(nX, nY, pDraw, DEF_UI_COLOR_WHITE, 0.f, PBT_ADD);
}

void CUIIcon::renderDefault( int x, int y, CDrawPort* pDraw, COLOR color, float delta, const ULONG ulPBT )
{
	pDraw->AddBtnTexture( m_TexID + m_nSkillDisable, x - delta, y - delta, 
		x + m_nWidth + delta, y + m_nHeight + delta,
		m_uv, color );

	pDraw->FlushBtnRenderingQueue( m_BtnType, ulPBT );
}

void CUIIcon::renderCooltime( int x, int y, CDrawPort* pDraw )
{
	DOUBLE dStartTime = 0;
	DOUBLE dCoolTime;
	DOUBLE dReUseTime = 0.0;

	if (m_BtnType == UBET_SKILL)
	{
		if (m_nIndex > 0)
			dStartTime = _pNetwork->GetSkillData(m_nIndex).Skill_Data.Skill_StartTime;

		dReUseTime = MY_INFO()->GetReuseSkill(m_nIndex);
	}
	else if (m_BtnType == UBET_ITEM)
	{
		if (m_pItems != NULL)
			dStartTime = m_pItems->ItemData->StartTime;

		dReUseTime = MY_INFO()->GetReuseItem(m_nIndex);
	}

	dCoolTime = ItemHelp::GetCoolTime(dReUseTime, dStartTime);

	if (dCoolTime <= 0.0)
	{
		m_bCooltime = false;

		if (m_BtnType == UBET_ITEM)		
			m_pItems->SetSkillDelay(FALSE);
		else if (m_BtnType == UBET_SKILL)
		{
			if (m_nPetKind == PET_JOB)
				MY_INFO()->SetPetSkillDelay(MY_PET_INFO()->lIndex, m_nIndex, false);
			else if (m_nPetKind == WILDPET_JOB)
				MY_INFO()->SetPetSkillDelay(0, m_nIndex, false);
			else
				MY_INFO()->SetSkillDelay(m_nIndex, false);				
		}
	}

	if(  dCoolTime < 0.5 )
	{
		float fDelta;

		fDelta = dCoolTime * 10;

		if( fDelta < 0 || fDelta > 5 ) 
		{
			fDelta = 0;
		}
		// Add render regions
		renderDefault(x, y, pDraw, DEF_UI_COLOR_WHITE, fDelta);

		// Add render regions
// 		pDraw->AddBtnTexture( m_nTextureID, nX - fDelta, nY - fDelta, nX + m_nWidth + fDelta *2, nY + m_nHeight + fDelta * 2,
// 			m_rtUV.U0, m_rtUV.V0, m_rtUV.U1, m_rtUV.V1,
// 			0x808080FF );

		// Render all button elements
//		pDraw->FlushBtnRenderingQueue( m_betType, PBT_ADD ); 		
	}
	else
	{
		renderDefault(x, y, pDraw, DEF_UI_COLOR_COOL_BG);

		// Delay animation
		FLOAT	fDelayRatio = dCoolTime / dReUseTime;
		FLOAT	fSizeOffset = FLOAT(BTN_SIZE / 2) * fDelayRatio;
		FLOAT	fUOffset = ( m_uv.U1 - m_uv.U0 ) * 0.5f * fDelayRatio;
		FLOAT	fVOffset = ( m_uv.V1 - m_uv.V0 ) * 0.5f * fDelayRatio;
		FLOAT	fX0 = FLOAT(x) + fSizeOffset;
		FLOAT	fY0 = FLOAT(y) + fSizeOffset;
		FLOAT	fX1 = FLOAT(x + m_nWidth) - fSizeOffset;
		FLOAT	fY1 = FLOAT(y + m_nHeight) - fSizeOffset;

		// Add render regions
		pDraw->AddBtnTexture( m_TexID, fX0, fY0, fX1, fY1,
			m_uv.U0 + fUOffset, m_uv.V0 + fVOffset,
			m_uv.U1 - fUOffset, m_uv.V1 - fVOffset,
			DEF_UI_COLOR_COOL_FRONT );

		// Render all button elements
		pDraw->FlushBtnRenderingQueue( m_BtnType, PBT_ADD );
	}
}

// void CUIIcon::renderAttribute(int x, int y, CDrawPort* pDraw)
// {
//     pDraw->InitTextureData(m_ptdAttrTexture);
// 
//     // 공격 속성 아이콘
//     if (m_nAtt > 0)
//     {
//         pDraw->AddTexture(nX + 16, nY + 16, nX + 32, nY + 32,
//                               m_rtAttributeIconAtt[m_nAtt].U0, m_rtAttributeIconAtt[m_nAtt].V0,
//                               m_rtAttributeIconAtt[m_nAtt].U1, m_rtAttributeIconAtt[m_nAtt].V1, 0xFFFFFFFF);
//     }
// 
//     // 방어 속성 아이콘
//     if (m_nDef > 0)
//     {
//         pDraw->AddTexture(nX + 16, nY + 16, nX + 32, nY + 32,
//                               m_rtAttributeIconDef[m_nDef].U0, m_rtAttributeIconDef[m_nDef].V0,
//                               m_rtAttributeIconDef[m_nDef].U1, m_rtAttributeIconDef[m_nDef].V1, 0xFFFFFFFF);
//     }
// 
//     pDraw->FlushRenderingQueue();
// }

void CUIIcon::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	if (m_pItems != NULL)
	{
		if (m_bCooltime == false)
		{
			if (m_pItems->GetSkillDelay() == TRUE)
				m_bCooltime = true;
		}

		// 내구도
#ifdef DURABILITY
		{
			if ( m_bDurItem == true)
				setImageHide(eIMG_TYPE_DURABILITY, m_pItems->IsZeroDurability() ? FALSE : TRUE);
		}
#endif	//	DURABILITY

		// 사용 제한 체크
		if (IsRestritionItem() == true)
			setImageHide(eIMG_TYPE_RESTRITION, FALSE);
		else
			setImageHide(eIMG_TYPE_RESTRITION, TRUE);

	}
		
	if (m_bHighlightMode)
	{
		DWORD nCurrent = timeGetTime();

		if ( nCurrent > m_dwAniStartTime + DEF_HIGHLIGHT_TIME )
		{
			m_bHighlight = !m_bHighlight;
			m_dwAniStartTime = nCurrent;
		}		
	}
}

void CUIIcon::setIconUV( int nRow, int nCol )
{
	if (m_BtnType < UBET_SKILL || m_BtnType >= UBET_TYPE_MAX)
		return;

	int	nUVSX = BTN_SIZE;
	int	nUVSY = BTN_SIZE;

	nUVSX *= nCol;
	nUVSY *= nRow;

	FLOAT	fTexWidth = _pUIBtnTexMgr->GetTexWidth( m_BtnType, m_TexID );
	FLOAT	fTexHeight = _pUIBtnTexMgr->GetTexHeight( m_BtnType, m_TexID );

	m_uv.SetUV(nUVSX, nUVSY, nUVSX + BTN_SIZE, nUVSY + BTN_SIZE, fTexWidth, fTexHeight);
}

WMSG_RESULT CUIIcon::OnCloseProc()
{
	TOOLTIPMGR()->hideUI(this);

	// 다른 컨트롤에도 처리되도록 FAIL 리턴
	return WMSG_FAIL;
}

// --------------------------------------------------------------------

void CUIIcon::setCount( __int64 count )
{
	if (m_pItems)
		m_pItems->Item_Sum = count;

	setStack();
}

CUIBase* CUIIcon::Clone()
{
	CUIIcon* pIcon = new CUIIcon(*this);

	pIcon->initialize();

	if (m_bRef == false)
	{
		if (m_pItems != NULL)
		{
			CItems* p = new CItems(*m_pItems);
			pIcon->setData(p, false);
		}
	}

	if (m_pAttribute != NULL)
		pIcon->setPointAttr( new CUIBase(*m_pAttribute) );
	if (m_pStack != NULL)
 		pIcon->setPointStack(new CUIImageFont(*m_pStack));
	if (m_pStackError != NULL)
 		pIcon->setPointStackError(new CUIBase(*m_pStackError));
	if (m_pPlus != NULL)
 		pIcon->setPointPlus(new CUIText(*m_pPlus));
	if (m_pJewelGrade != NULL)
 		pIcon->setPointJewel(new CUIBase(*m_pJewelGrade));
	if (m_pToggle != NULL)
 		pIcon->setPointToggle(new CUISpriteAni(*m_pToggle));
	
	for (int i = 0; i < eIMG_TYPE_MAX; ++i)
	{
		if (m_pImage[i] != NULL)
 			pIcon->setPointImage((eIMG_TYPE)i, new CUIImage(*m_pImage[i]));
	}

	return pIcon;
}

void CUIIcon::OnEnter( UINT16 x, UINT16 y )
{
	m_bEnter = true;

	if (m_bCustomShowInfo == false)
		CUITooltipMgr::getSingleton()->setData(this);

	if (m_pCmdOnEnter != NULL)
		m_pCmdOnEnter->execute();
}

void CUIIcon::OnLeave( UINT16 x, UINT16 y )
{
	m_bEnter = false;

	if (m_bCustomShowInfo == false)
		CUITooltipMgr::getSingleton()->hideUI(this);

	CUIBase::OnLeave(x, y);
}

WMSG_RESULT CUIIcon::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide == TRUE)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (pMsg->wParam & MK_LBUTTON && CUIManager::getSingleton()->GetDragIcon() == NULL)
	{
		if (m_pCmdDrag)
			m_pCmdDrag->execute();
	}

	return WMSG_FAIL;
}

WMSG_RESULT CUIIcon::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (m_bSelectIcon == TRUE)
	{
		if (m_pImage[eIMG_TYPE_SELECT] == NULL)
		{
			m_pImage[eIMG_TYPE_SELECT] = UISUPPORT()->clone_Image(eIMG_TYPE_SELECT);
			addChild(m_pImage[eIMG_TYPE_SELECT]);
		}

		m_pImage[eIMG_TYPE_SELECT]->Hide(FALSE);
	}

	if (m_pCmd)
		m_pCmd->execute();

	return WMSG_FAIL;
}

WMSG_RESULT CUIIcon::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (m_pCmdUp)
		m_pCmdUp->execute();

	return WMSG_FAIL;
}

WMSG_RESULT CUIIcon::OnLButtonDBLClick( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (m_pToggle != NULL && m_pToggle->GetHide() == FALSE && m_BtnType == UBET_ITEM)
	{
		if (m_eWhichUI != UI_QUICKSLOT &&
			(m_eWhichUI < UI_INVENTORY || m_eWhichUI > UI_QUICKSLOT3))
		{
			UIMGR()->GetChattingUI()->AddSysMessage(_S(7035, "활성화 된 아이템은 옮길 수 없습니다."));
			return WMSG_FAIL;
		}
	}

	if (m_pCmdDBL)
		m_pCmdDBL->execute();

	return WMSG_COMMAND;
}

void CUIIcon::HideTooltip()
{
	CUITooltipMgr::getSingleton()->hideUI(this);
}

void CUIIcon::SetHighLightMode( bool bOnOff )
{
	if (bOnOff == true)
	{
		m_dwAniStartTime = timeGetTime();
	}
	else
	{
		m_dwAniStartTime = 0;
		m_bHighlight = false;
	}

	m_bHighlightMode = bOnOff;
}

void CUIIcon::SetToggle( bool bToggle )
{
	if (m_pToggle != NULL)
	{
		if (bToggle == true)
			m_pToggle->PlayAni();
		else
			m_pToggle->StopAni();

		m_pToggle->Hide(bToggle ? FALSE : TRUE);
	}
}

// --------------------------------------------------------------------

void CUIIcon::setItem(int nIndex, int nVIndex)
{
    m_nIndex = nIndex;
    m_BtnType = UBET_ITEM;
	m_BtnEtctype = eETC_TYPE_NONE;

	if (m_bRef == false)
	{
		SAFE_DELETE(m_pItems);
	}

	m_pItems = new CItems;
	m_bRef = false;
	m_pItems->Item_UniIndex = nVIndex;

    CItemData*	pItemData = _pNetwork->GetItemData(nIndex);
    m_pItems->ItemData = pItemData;

    int nRow, nCol;

    if (nIndex == NAS_INDEX)
    {
        m_TexID	= NAS_TEX_ID;
        nRow		= NAS_TEX_ROW;
        nCol		= NAS_TEX_COL;
    }
    else
    {
        // Set texture data
        m_TexID = pItemData->GetIconTexID();
        nRow = pItemData->GetIconTexRow();
        nCol = pItemData->GetIconTexCol();

// 		if (!(pItemData->GetFlag() & ITEM_FLAG_QUEST) && pItemData->GetSetItemIndex() > 0)
// 		{
// 			m_bSetItem = TRUE;
// 		}
// 		else
// 		{
// 			m_bSetItem = FALSE;
// 		}
    }

    setIconUV(nRow, nCol);
}

void CUIIcon::setQuest(int nIndex)
{
	m_nIndex = nIndex;

	m_BtnType = UBET_QUEST;
	m_TexID = 0;

	int	nRow = ( nIndex - 1 ) / 4;
	int	nCol = ( nIndex - 1 ) % 4;

	if( nIndex == 1000 ) // 지불 아이콘으로 사용하는 Quest버튼 : 열라 하드...
	{
		nRow = 0;
		nCol = 4;
	}
	// 펫 관련 퀘스트...
	else if( nIndex == 4000 || nIndex == 4003 || nIndex == 4005)
	{
		nRow = 1;
		nCol = 1;
	}
	// 펫 관련 퀘스트...
	else if( nIndex == 4001 || nIndex == 4004 || nIndex == 4006)
	{
		nRow = 1;
		nCol = 1;
	}
	else if (nIndex == NEWS_WEB)
	{
		nRow = 0;
		nCol = 5;
	}
	else if (nIndex == NEWS_NORMAL)
	{
		nRow = 1;
		nCol = 5;
	}
	// 이벤트 버튼의 이미지.
	else if( nIndex >= EVENT_START_INDEX )
	{
		nRow = 1;
		nCol = 4;
	}

	setIconUV(nRow, nCol);
}

void CUIIcon::setAction(int nIndex)
{
	m_nIndex = nIndex;
	m_BtnType = UBET_ACTION;

	CAction* pActionData = CAction::getData(nIndex);
	m_TexID = pActionData->GetTexID();

	int nRow = pActionData->GetTexRow();
	int nCol = pActionData->GetTexCol();

	setIconUV(nRow, nCol);
}

void CUIIcon::setAffinity( int nIndex )
{
	m_nIndex = nIndex;
	m_BtnType = UBET_SKILL;

	CAffinityData* pData = _pNetwork->GetAffinityData();
	CAffinityData::_AffinityInfo* pInfo = pData->GetAffinityDataByIndex(nIndex);

	if (pInfo == NULL)
		return;

	m_TexID = pInfo->nIconTextureID;

	int nRow = pInfo->nIconTextureRow;
	int nCol = pInfo->nIconTextureCol;

	setIconUV(nRow, nCol);
}

void CUIIcon::setMCombo( int nIndex )
{
    m_nIndex = nIndex;
    m_BtnType = UBET_COMBO;

    if (m_bRef == false)
    {
        SAFE_DELETE(m_pItems);
    }

    m_pItems = new CItems;
    m_bRef = false;

    CMissionCase* MC = CMissionCase::getDataSeq(nIndex);

	m_TexID = MC->texture_id;
    int nRow = MC->texture_row;
    int nCol = MC->texture_col;

    int nUVSX = COMBO_BTN_SIZE - 2;
    int nUVSY = COMBO_BTN_SIZE - 2;

    int	nTexRow = nRow;
    int	nTexCol = nCol;

    nUVSX *= nTexCol;
    nUVSY *= nTexRow;

    FLOAT	fTexWidth = _pUIBtnTexMgr->GetTexWidth(UBET_COMBO, MC->texture_id);
    FLOAT	fTexHeight = _pUIBtnTexMgr->GetTexHeight(UBET_COMBO, MC->texture_id);
    m_uv.SetUV(nUVSX, nUVSY, nUVSX + COMBO_BTN_SIZE - 2, nUVSY + COMBO_BTN_SIZE - 2, fTexWidth, fTexHeight);
}

void CUIIcon::setStack()
{
	if (m_pItems == NULL)
		return;

	if (m_nIndex == 19)		// 나스라면
		return;

	CItemData*	pItemData = m_pItems->ItemData;

	if (pItemData != NULL && (pItemData->GetFlag() & ITEM_FLAG_COUNT))
	{
		if (m_pItems->Item_Sum > pItemData->GetStack())
		{
			if (m_pStackError == NULL)
			{
				m_pStackError = (CUIBase*)UISUPPORT()->clone_stack_error();
				addChild(m_pStack);
			}

			m_pStackError->Hide(FALSE);
		}
		else
		{
			if (m_pItems->Item_Sum > 1)
			{
				if (m_pStack == NULL)
				{
					m_pStack = UISUPPORT()->clone_stack();
					addChild(m_pStack);
				}

				CTString strTmp;
				strTmp.PrintF("%I64d", m_pItems->Item_Sum);

				m_pStack->setString(strTmp);
				m_pStack->Hide(FALSE);
			}
			else
			{
				if (m_pStack != NULL)
					m_pStack->Hide(TRUE);
			}
		}
	}
	else
	{
		if (m_pStack != NULL)
			m_pStack->Hide(TRUE);

		if (m_pStackError != NULL)
			m_pStackError->Hide(TRUE);
	}
}

bool CUIIcon::IsRestritionItem()
{
	if(m_pItems == NULL)
		return false;

	CItemData*	pItemData = m_pItems->ItemData;

	if( pItemData != NULL && pItemData->GetFlag() & ITEM_FLAG_QUEST )
	{
		int	nZoneNo = pItemData->GetRestrictionZoneNo();

		if( nZoneNo == _pNetwork->MyCharacterInfo.zoneNo )
		{
			FLOAT3D		restrictionPos;
			FLOAT		fRadius = (FLOAT)pItemData->GetRestrictionRadius();
			if( 0 == fRadius )
				fRadius = 20.0f;

			restrictionPos(1) = (FLOAT)pItemData->GetRestrictionPosX();
			restrictionPos(2) = (FLOAT)pItemData->GetRestrictionPosY();
			restrictionPos(3) = (FLOAT)pItemData->GetRestrictionPosZ();

			FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - restrictionPos(1);
			FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - restrictionPos(3);
			if( fDiffX * fDiffX + fDiffZ * fDiffZ > fRadius )
				return true;
		}
		else
			return true;
	}

	return false;
}
