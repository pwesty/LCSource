#include "StdH.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIWindow.h>
#include <Engine/Graphics/DrawPort.h>
#include <Engine/Interface/UITextureManager.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIImageBox.h>

#define BTN_SIZE_CORPS_WIDTH	19
#define BTN_SIZE_CORPS_HEIGHT	12

CUIImageBox::CUIImageBox()
: m_eImageType( IT_GENERAL )
, m_bHighlight( FALSE )
, m_colHighlight( 0x808080FF )
, m_bShowPopup( FALSE )
, m_bEnablePopup( FALSE )
, m_strInfo( CTString("") )
, m_nPopWidth( 0 )
, m_eUBType(UBET_ITEM)
{
	m_ptdPopup = NULL;
	m_ptdCorps = NULL;
}

CUIImageBox::~CUIImageBox()
{
	Destroy();

	STOCK_RELEASE( m_ptdPopup );

	STOCK_RELEASE( m_ptdCorps );
}

void CUIImageBox::ResetImageBox()
{
	m_eImageType = IT_GENERAL;
	m_bHighlight = FALSE;
	m_colHighlight = 0x808080FF;
	m_bShowPopup = FALSE;
	m_bEnablePopup = FALSE;
	m_strInfo = CTString("");
	m_vecString.clear();
	m_nPopWidth = 0;
	m_colText = 0;
}

void CUIImageBox::Render()
{
	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );

	int nX0 = nX + m_rtRenderRegion.Left;
	int nX1 = nX + m_rtRenderRegion.Right;
	int nY0 = nY + m_rtRenderRegion.Top;
	int nY1 = nY + m_rtRenderRegion.Bottom;
	float fU0 = m_rtUV.U0 + (m_rtUV.U1 - m_rtUV.U0) * m_rtRenderRegion.Left / float(m_nWidth);
	float fU1 = m_rtUV.U0 + (m_rtUV.U1 - m_rtUV.U0) * m_rtRenderRegion.Right / float(m_nWidth);
	float fV0 = m_rtUV.V0 + (m_rtUV.V1 - m_rtUV.V0) * m_rtRenderRegion.Top / float(m_nHeight);
	float fV1 = m_rtUV.V0 + (m_rtUV.V1 - m_rtUV.V0) * m_rtRenderRegion.Bottom / float(m_nHeight);

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	if(m_eImageType == IT_GENERAL)
	{
		pDrawPort->InitTextureData( m_ptdBaseTexture );
		pDrawPort->AddTexture(nX0, nY0, nX1, nY1,
											fU0, fV0,
											fU1, fV1,
											0xFFFFFFFF );
		pDrawPort->FlushRenderingQueue();
	}
	else if (m_eImageType == IT_CORPS)
	{
		pDrawPort->InitTextureData( m_ptdCorps );
		pDrawPort->AddTexture(nX0, nY0, nX1, nY1,
											fU0, fV0,
											fU1, fV1,
											0xFFFFFFFF );
		pDrawPort->FlushRenderingQueue();
	}
	else
	{
		UIBtnExType bet;
		switch(m_eImageType)
		{
			case IT_EXP:
			case IT_MONEY:
			case IT_ITEM:
			case IT_SP:		// [090617: selo] SP 이미지 타입
			case IT_RVR_POINT:
				bet = UBET_ITEM;
				break;
			case IT_SKILL:
			case IT_SSKILL:
			case IT_AFFINITY: // 친화도 개편2 친화도 타입 추가 [2/7/2013 Ranma]
				bet = UBET_SKILL;
				break;
			case IT_ACTION:
				bet = UBET_ACTION;
				break;
			case IT_AFFINITY_MONSTER:
				bet = UBET_COMBO;				
				break;
		}
		pDrawPort->AddBtnTexture(m_nTextureID, nX0, nY0, nX1, nY1,
											fU0, fV0,
											fU1, fV1,
											0xFFFFFFFF );
		pDrawPort->FlushBtnRenderingQueue( bet, PBT_BLEND );

		if(m_bHighlight)
		{
			pDrawPort->AddBtnTexture(m_nTextureID, nX0, nY0, nX1, nY1,
												fU0, fV0,
												fU1, fV1,
												m_colHighlight );

			// Render all button elements
			pDrawPort->FlushBtnRenderingQueue( bet, PBT_ADD );
		}
	}
}

void CUIImageBox::PopupRender()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	int	nX, nY;
	GetAbsPos( nX, nY );
	
	int nX0 = nX + m_rtRenderRegion.Left;
	int nX1 = nX + m_rtRenderRegion.Right;
	int nY0 = nY + m_rtRenderRegion.Top;
	int nY1 = nY + m_rtRenderRegion.Bottom;

	int rtRenderRegionMiddle = nX + (m_rtRenderRegion.Left + m_rtRenderRegion.Right) / 2;
	int nRegionHeight = m_rtRenderRegion.Bottom - m_rtRenderRegion.Top;
		
	if ( m_bShowPopup )	
	{
		pDrawPort->InitTextureData( m_ptdPopup );
		
		int nAlineSize	= m_vecString.size();
		int nPopHeight	= 10 - _pUIFontTexMgr->GetLineSpacing() + (nAlineSize+1) * _pUIFontTexMgr->GetLineHeight();
		int nPopX = rtRenderRegionMiddle - (m_nPopWidth/2);
		int nPopY = nY0 - nPopHeight;
	
		if( nPopX < pUIManager->GetMinI() )
		{
			nPopX = pUIManager->GetMinI();
		}
		else if (nPopX + m_nPopWidth > pUIManager->GetMaxI())
		{
			nPopX = pUIManager->GetMaxI() - m_nPopWidth;
		}
		
		if (nPopY < pUIManager->GetMinJ())
		{
			nPopY = nY + nRegionHeight;
		}
		
		m_bxPopupInfo.SetBoxPos(WRect(nPopX-nX, nPopY-nY, nPopX-nX+m_nPopWidth, nPopY-nY+nPopHeight ));
		m_bxPopupInfo.Render(nX,nY);

		std::vector<CTString>::iterator b = m_vecString.begin();
		
		for (int i = 0; b != m_vecString.end(); b++, i++)
		{
			pDrawPort->PutTextEx( *b, nPopX+20, nPopY+6+(i*14), m_colText );
		}
		pDrawPort->FlushRenderingQueue();
		pDrawPort->EndTextEx();		
	}
}

void CUIImageBox::SetPopupInfo( CTString strInfo, BOOL bPopup, COLOR Color, int nPopWidth )
{	
	// Set Popup
	if (bPopup)
	{
		m_strInfo = strInfo;
		m_bEnablePopup = bPopup;
		m_nPopWidth = nPopWidth + 40;
		m_colText = Color;
		StringSplit(m_vecString, m_strInfo, nPopWidth);
		InitPopupImageBox();
	}
}

void CUIImageBox::SetImageByType(eImageType type, int index, int nSyndiType /* = 0 */)
{
	FLOAT fTexWidth;
	FLOAT fTexHeight;
	int nTexRow, nTexCol;
	int nTextureID;
	
	switch(type)
	{
	case IT_EXP://hardcoding	
		{
			nTextureID = 1;
			nTexRow = 15;
			nTexCol = 5;
			m_eUBType = UBET_ITEM;
		} break;
	case IT_SP:	// [090617: selo] SP 이미지 추가 하드코딩
		{
			// only russia sp icon [9/9/2010 rumist]
#if defined (G_RUSSIA)
			nTextureID = 12;
			nTexRow = 0;
			nTexCol = 2;
			m_eUBType = UBET_ITEM;
#else
			nTextureID = 9;
			nTexRow = 12;
			nTexCol = 11;
			m_eUBType = UBET_ITEM;
#endif			
		} break;
	case IT_MONEY:
	case IT_ITEM:
		{
			if(IT_MONEY == type) index = 19;//hardcoding
			CItemData*	pItemData = _pNetwork->GetItemData( index );
			nTextureID = pItemData->GetIconTexID();
			nTexRow = pItemData->GetIconTexRow();
			nTexCol = pItemData->GetIconTexCol();
			m_eUBType = UBET_ITEM;
		} break;
	case IT_SKILL:
		{
			CSkill	&rSkillData = _pNetwork->GetSkillData( index );
			nTextureID = rSkillData.GetIconTexID();
			nTexRow = rSkillData.GetIconTexRow();
			nTexCol = rSkillData.GetIconTexCol();
			m_eUBType = UBET_SKILL;
		} break;
	case IT_SSKILL:// Special Skill
		{
			CSpecialSkill* pSSkillData = CSpecialSkill::getData(index);
			if (pSSkillData == NULL)
				return;
			nTextureID = pSSkillData->GetIconTexID();
			nTexRow = pSSkillData->GetIconTexRow();
			nTexCol = pSSkillData->GetIconTexCol();
			m_eUBType = UBET_SKILL;
		} break;
	case IT_ACTION:
		{
			CAction* pActionData = CAction::getData(index);
			if (pActionData == NULL)
				return;
			nTextureID = pActionData->GetTexID();
			nTexRow = pActionData->GetTexRow();
			nTexCol = pActionData->GetTexCol();
			m_eUBType = UBET_ACTION;
		} break;
	case IT_RVR_POINT:
		{
			nTextureID = 19;
			nTexRow = 2;
			nTexCol = 3;
			m_eUBType = UBET_ITEM;

			// 딜라문 npc면
			if (nSyndiType == 2)	
				nTexCol = 4;
		} break;
	case IT_CORPS:
		{
			InitCorpsImageBox();
			SetImageForCorps( type, index );
			return;
		} break;

	case IT_AFFINITY: // 친화도 개편2 이미지 박스에 친화도 추가 [2/6/2013 Ranma]
		{
			CAffinityData*	rAffinityData = _pNetwork->GetAffinityData();
					
			nTextureID = rAffinityData->GetAffinityDataByIndex(index)->nIconTextureID;
			nTexRow = rAffinityData->GetAffinityDataByIndex(index)->nIconTextureRow;
			nTexCol = rAffinityData->GetAffinityDataByIndex(index)->nIconTextureCol;
			m_eUBType = UBET_SKILL;
		} break;

	case IT_AFFINITY_MONSTER: // 친화도 개편2 이미지 박스에 친화도 추가 [2/6/2013 Ranma]
		{
			CAffinityData* pTempAffinityData = _pNetwork->GetAffinityData();
				
			// 맵은 키값으로 소팅이 되기 때문에 값이 뒤에서부터 들어가 있다. 그래서 뒤에서 부터 값을 가져도록 하자 최대한 근접하게 [2/7/2013 Ranma]
			CAffinityData::mapAffReverseIter itermapBegin = pTempAffinityData->mapAffinity_Data.rbegin();
			CAffinityData::mapAffReverseIter itermapEnd = pTempAffinityData->mapAffinity_Data.rend();
			CAffinityData::mapAffReverseIter itermapP = itermapBegin;
			
			for (; itermapP != itermapEnd; itermapP++)
			{
				int pInfoIndex = (*itermapP).first;

				int nNPCIndex = _pNetwork->GetAffinityData()->GetAdminNPCIndex(pInfoIndex);
							
				std::vector<CAffinityData::_AffinityMonsterInfo>::iterator b, e;
				b = _pNetwork->GetAffinityData()->GetAffinityData(nNPCIndex)->vecMonsterList.begin();
				e = _pNetwork->GetAffinityData()->GetAffinityData(nNPCIndex)->vecMonsterList.end();
				for (int i = 0; b != e; b++, i++)
				{
					if ( b->index ==  index)
					{
						nTextureID = b->TextureID;
						nTexRow = b->TextureRow;
						nTexCol = b->TextureCol;
						m_eUBType = UBET_COMBO;

						fTexWidth = _pUIBtnTexMgr->GetTexWidth( UIBtnExType(m_eUBType), nTextureID );
						fTexHeight = _pUIBtnTexMgr->GetTexHeight( UIBtnExType(m_eUBType), nTextureID );
						m_ptdBaseTexture = _pUIBtnTexMgr->GetTex( UIBtnExType(m_eUBType), nTextureID );
						m_ptdBaseTexture->AddReference();
						
						int	nUVSX = 50 * nTexCol;
						int	nUVSY = 50 * nTexRow;
						SetUV( nUVSX, nUVSY, nUVSX + 50, nUVSY + 50, fTexWidth, fTexHeight );
						
						m_eImageType = type;
						m_nTextureID = nTextureID;
						return;
					}
				}
			}
		} break;

	default:
		{
			ASSERTALWAYS("Somthing wrong");
		} break;
	}

	fTexWidth = _pUIBtnTexMgr->GetTexWidth( UIBtnExType(m_eUBType), nTextureID );
	fTexHeight = _pUIBtnTexMgr->GetTexHeight( UIBtnExType(m_eUBType), nTextureID );
	m_ptdBaseTexture = _pUIBtnTexMgr->GetTex( UIBtnExType(m_eUBType), nTextureID );
	m_ptdBaseTexture->AddReference();

	int	nUVSX = BTN_SIZE * nTexCol;
	int	nUVSY = BTN_SIZE * nTexRow;
	SetUV( nUVSX, nUVSY, nUVSX + BTN_SIZE, nUVSY + BTN_SIZE, fTexWidth, fTexHeight );

	m_eImageType = type;
	m_nTextureID = nTextureID;
}

void CUIImageBox::SetImageForCorps( eImageType type, int index )
{
	SetSize( BTN_SIZE_CORPS_WIDTH, BTN_SIZE_CORPS_HEIGHT );
	SetRenderRegion( 0, 0, BTN_SIZE_CORPS_WIDTH, BTN_SIZE_CORPS_HEIGHT );
	
	m_rtUV = m_rtCorpsMark[index];

	m_eImageType = type;
}

WMSG_RESULT	CUIImageBox::MouseMessage( MSG *pMsg )
{
	if( !IsEnabled() )
		return WMSG_FAIL;

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if ( IsInside(nX, nY) && m_bEnablePopup )
			{
				m_bShowPopup = TRUE;
				return WMSG_SUCCESS;
			}
			m_bShowPopup = FALSE;
		}
		break;
	}
	return WMSG_FAIL;
}

void CUIImageBox::InitCorpsImageBox()
{
	InitPopupImageBox();

	// Set Corps Mark
	m_ptdCorps	= CreateTexture( CTString( "Data\\Interface\\new_interface.tex") );
	FLOAT fTexWidth		= m_ptdCorps->GetPixWidth();
	FLOAT fTexHeight	= m_ptdCorps->GetPixHeight();
	m_rtCorpsMark[CT_BOSS].SetUV( 385, 305, 405, 316, fTexWidth, fTexHeight );
	m_rtCorpsMark[CT_VICE_BOSS].SetUV( 407, 305, 427, 316, fTexWidth, fTexHeight );
	m_rtCorpsMark[CT_RUSH_CAPTAIN].SetUV( 451, 305, 471, 316, fTexWidth, fTexHeight );
	m_rtCorpsMark[CT_SUPPORT_CAPTAIN].SetUV( 429, 305, 449, 316, fTexWidth, fTexHeight );
	m_rtCorpsMark[CT_RECON_CAPTAIN].SetUV( 363, 305, 383, 316, fTexWidth, fTexHeight );
	m_rtCorpsMark[CT_RUSH].SetUV( 297, 305, 317, 316, fTexWidth, fTexHeight );
	m_rtCorpsMark[CT_SUPPORT].SetUV( 319, 305, 339, 316, fTexWidth, fTexHeight );
	m_rtCorpsMark[CT_RECON].SetUV( 341, 305, 361, 316, fTexWidth, fTexHeight );
	m_rtCorpsMark[CT_MEMBER].SetUV( 275, 305, 295, 316, fTexWidth, fTexHeight );
	//=================================================
}

void CUIImageBox::InitPopupImageBox()
{
	//=================================================
	// Set Popup
	m_ptdPopup = CreateTexture( CTString( "Data\\Interface\\CommonBtn.tex") );
	FLOAT fTexWidth  = m_ptdPopup->GetPixWidth();
	FLOAT fTexHeight = m_ptdPopup->GetPixHeight();
	m_bxPopupInfo.SetBoxUV(m_ptdPopup,20,WRect(0,137,141,227));
	//=================================================
} 

void CUIImageBox::StringSplit(std::vector<CTString>& vecOutput, CTString strInput, ULONG ulColumnWidth)
{
	extern void StringSplit(std::vector<CTString>& vecOutput, CTString strInput, ULONG ulColumnWidth);
	StringSplit(vecOutput, strInput, ulColumnWidth);
}

UIRectUV CUIImageBox::GetUV()
{
	UIRectUV uv = m_rtUV;
#ifdef UI_TOOL
	std::string strImg = getTexString();
	if(strImg.empty() == false)
	{
		float fTexW, fTexH;
		CTextureData* pTex = _pTextureStock->Obtain_t( strImg.c_str() );
		fTexW = pTex->GetPixWidth();
		fTexH = pTex->GetPixHeight();

		uv.U0 = m_rtUV.U0 * fTexW;
		uv.V0 = m_rtUV.V0 * fTexH;
		uv.U1 = m_rtUV.U1 * fTexW;
		uv.V1 = m_rtUV.V1 * fTexH;
	}
#endif // UI_TOOL
	 return uv;
}
