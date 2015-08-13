// UIFortune.cpp: implementation of the CUIFortune class.
//
//////////////////////////////////////////////////////////////////////
#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIShop.h>
#include <vector>
#include <Engine/Interface/UIFortune.h>
#include <Engine/Interface/UIMultList.h>
#include <Common/Packet/ptype_old_do_item.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// Name : CUIFortune()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIFortune::CUIFortune()
	: m_bPickTitle(FALSE)
	, m_nTab(-1)
	, m_nIdx(-1)
	, m_bMove(false)
	, m_nFortuneSkillCount(0)
{
}

// ----------------------------------------------------------------------------
// Name : ~CUIFortune()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIFortune::~CUIFortune()
{
	ResetFortune();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc : 
// ----------------------------------------------------------------------------
void CUIFortune::Create(CUIWindow* pParentWnd, int nX, int nY, int nWidth, int nHeight)
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Create Texture.
	m_ptdBaseTexture = CreateTexture(CTString("Data\\Interface\\fortune.tex"));
	// Get Texture size.
	FLOAT fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// Set size of UI on this texture.
	m_rtsBase.AddRectSurface(UIRect(0,0,nWidth,nHeight),UIRectUV(0,0,410,410,fTexWidth,fTexHeight));

	m_rtTitle.SetRect(0, 0, nWidth, 37);
	m_strTitle = _S(5868, "포츈 코스튬");

	m_btnClose.Create(this, CTString(""), 390, 4, 16, 16);
	m_btnClose.SetUV(UBS_IDLE, 411, 0, 411+16, 0+16, fTexWidth, fTexHeight);
	m_btnClose.SetUV(UBS_ON, 427, 0, 427+16, 0+16, fTexWidth, fTexHeight);
	m_btnClose.SetUV(UBS_CLICK, 443, 0, 443+16, 0+16, fTexWidth, fTexHeight);
	m_btnClose.CopyUV(UBS_IDLE, UBS_DISABLE);

	m_btnOK.Create(this, _S(191, "확인"), 149, 377, 108, 21);
	m_btnOK.SetUV(UBS_IDLE, 0, 411, 0+108, 411+21, fTexWidth, fTexHeight);
	m_btnOK.SetUV(UBS_ON, 0, 433, 0+108, 433+21, fTexWidth, fTexHeight);
	m_btnOK.SetUV(UBS_CLICK, 0, 455, 0+108, 455+21, fTexWidth, fTexHeight);
	m_btnOK.CopyUV(UBS_IDLE, UBS_DISABLE);

	m_rtHighlightView.SetRect(179, 67, 179+52, 67+97);
	m_rtHighlight.SetUV(413, 27, 413+52, 27+97, fTexWidth, fTexHeight);

	const POINT pos[MAX_CARD] = {
		{  33, 111 },
		{  85, 103 },
		{ 137,  95 },
		{ 189,  87 },
		{ 241,  95 },
		{ 293, 103 },
		{ 345, 111 }
	};
	for (int i = 0; i < MAX_CARD; i++)
	{
		m_imgCard[MAX_CARD-1-i].Create(this, pos[i].x, pos[i].y, 32, 32);
		m_imgCard[MAX_CARD-1-i].SetRenderRegion(0, 0, 32, 32);
	}

	int _nMsgBoxLineHeight = _pUIFontTexMgr->GetFontHeight() + 4;//8;

	// List box
	m_lbDescription.Create(this, 15, 201, 368, 159, _nMsgBoxLineHeight, 4, 4, 1, FALSE);
	m_lbDescription.CreateScroll(TRUE, 0, 2, 10, 156, 10, 10, 0, 0, 10);
	m_lbDescription.SetColumnPosX(0, 180, TEXT_CENTER);
	// Up button
	m_lbDescription.SetScrollUpUV(UBS_IDLE, 471, 0, 471+10, 0+10, fTexWidth, fTexHeight);
	m_lbDescription.SetScrollUpUV(UBS_ON, 481, 0, 481+10, 0+10, fTexWidth, fTexHeight);
	m_lbDescription.SetScrollUpUV(UBS_CLICK, 491, 0, 491+10, 0+10, fTexWidth, fTexHeight);
	m_lbDescription.CopyScrollUpUV(UBS_IDLE, UBS_DISABLE);
	// Down button
	m_lbDescription.SetScrollDownUV(UBS_IDLE, 471, 11, 471+10, 11+10, fTexWidth, fTexHeight);
	m_lbDescription.SetScrollDownUV(UBS_ON, 481, 11, 481+10, 11+10, fTexWidth, fTexHeight);
	m_lbDescription.SetScrollDownUV(UBS_CLICK, 491, 11, 491+10, 11+10, fTexWidth, fTexHeight);
	m_lbDescription.CopyScrollDownUV(UBS_IDLE, UBS_DISABLE);
	// Bar button
	m_lbDescription.SetScrollBarTopUV(460, 0, 460+10, 0+8, fTexWidth, fTexHeight);
	m_lbDescription.SetScrollBarMiddleUV(460, 0+8, 460+10, 16, fTexWidth, fTexHeight);
	m_lbDescription.SetScrollBarBottomUV(460, 16, 460+10, 16+8, fTexWidth, fTexHeight);
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc : 
// ----------------------------------------------------------------------------
void CUIFortune::Render()
{
	int		i, j;
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	pDrawPort->InitTextureData(m_ptdBaseTexture);
	m_rtsBase.SetPos(m_nPosX, m_nPosY);
	m_rtsBase.RenderRectSurface(pDrawPort, 0xFFFFFFFF);

	m_btnClose.Render();
	m_btnOK.Render();
	m_lbDescription.Render();

	pDrawPort->FlushRenderingQueue();

	pDrawPort->PutTextExCX(m_strTitle, m_nPosX+205, m_nPosY+15);
	pDrawPort->EndTextEx();

	DWORD tick = timeGetTime();
	if (m_bMove && (tick - m_prevTick) > m_delta)
	{
		if (m_bSlow && m_delta < 500)
			m_delta *= 2;

		m_prevTick = tick;
		m_offset++;

		for( i = 0; i < MAX_CARD && i < m_nFortuneSkillCount; i++)
		{
			int id = (i+m_offset) % m_nFortuneSkillCount;
			m_imgCard[i].SetImageByType(CUIImageBox::IT_SKILL, m_vecFortuneSkill[id].skill_index);
			if (i == 3 && id == m_nFortuneIndex)
			{
				m_bMove = false;
				m_btnOK.SetEnable(TRUE);
				m_btnClose.SetEnable(TRUE);
				
				const int width = 300;
				std::vector<CTString> vecOutput;
				extern void StringSplit(std::vector<CTString>& vecOutput, CTString strInput, ULONG ulColumnWidth);
				CSkill& skill = _pNetwork->GetSkillData(m_vecFortuneSkill[m_nFortuneIndex].skill_index);
				m_lbDescription.AddString(0, CTString(skill.GetName()), 0xBBC200FF, TRUE, NULL, TRUE);
				vecOutput.clear();
				CTString strText;
				strText.PrintF(_S(5894, "조언: %s"), _S(m_vecFortuneSkill[m_nFortuneIndex].string_index, "조언"));
				StringSplit(vecOutput, strText, width);
				for( j = 0; j < vecOutput.size(); j++ )
					m_lbDescription.AddString(0, vecOutput[j], 0xBD6F3EFF);
				vecOutput.clear();
				StringSplit(vecOutput, CTString(skill.GetDescription()), width);
				for( j = 0; j < vecOutput.size(); j++ )
					m_lbDescription.AddString(0, vecOutput[j], 0xBBBDBAFF);
			}
		}
	}

	for( i = 0; i < MAX_CARD; i++)
		m_imgCard[i].Render();

	if (!m_bMove)
	{
		pDrawPort->InitTextureData(m_ptdBaseTexture);
		pDrawPort->AddTexture(m_nPosX + m_rtHighlightView.Left, m_nPosY + m_rtHighlightView.Top,
			m_nPosX + m_rtHighlightView.Right, m_nPosY + m_rtHighlightView.Bottom,
			m_rtHighlight.U0, m_rtHighlight.V0, m_rtHighlight.U1, m_rtHighlight.V1,
			0xFFFFFFFF);
		pDrawPort->FlushRenderingQueue();
	}
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc : 
// ----------------------------------------------------------------------------
void CUIFortune::ResetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ)
{
	SetPos((pixMaxI + pixMinI - GetWidth()) / 2, (pixMaxJ + pixMinJ - GetHeight()) / 2);
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc : 
// ----------------------------------------------------------------------------
void CUIFortune::AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ)
{
	if (m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ)
		ResetPosition(pixMinI, pixMinJ, pixMaxI, pixMaxJ);
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc : 
// ----------------------------------------------------------------------------
WMSG_RESULT CUIFortune::MouseMessage(MSG* pMsg)
{
	WMSG_RESULT	wmsgResult;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD(pMsg->lParam);
	int	nY = HIWORD(pMsg->lParam);

	switch (pMsg->message)
	{
	case WM_MOUSEMOVE:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			if (IsInside(nX, nY))
				pUIManager->SetMouseCursorInsideUIs();

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Move UI window.
			if (m_bPickTitle && (pMsg->wParam & MK_LBUTTON))
			{
				nOldX = nX;	nOldY = nY;
				Move(ndX, ndY);
				return WMSG_SUCCESS;
			}
			else if (m_btnClose.MouseMessage(pMsg) != WMSG_FAIL)
				return WMSG_SUCCESS;
			else if (m_btnOK.MouseMessage(pMsg) != WMSG_FAIL)
				return WMSG_SUCCESS;
			else if (m_lbDescription.MouseMessage(pMsg) != WMSG_FAIL)
				return WMSG_SUCCESS;
			else
				return WMSG_SUCCESS;
			break;
		}

	case WM_LBUTTONDOWN:
		{
			if (IsInside(nX, nY))
			{
				CUIManager* pUIManager = CUIManager::getSingleton();

				nOldX = nX;
				nOldY = nY;

				// Close button
				if (m_btnClose.MouseMessage(pMsg) != WMSG_FAIL)
				{
					// Nothing
				}
				else if (IsInsideRect(nX, nY, m_rtTitle))
				{
					m_bPickTitle = TRUE;
				}
				else if (m_btnOK.MouseMessage(pMsg) != WMSG_FAIL)
				{
				}
				else if (m_lbDescription.MouseMessage(pMsg) != WMSG_FAIL)
				{
				}
				else
				{
				}
				pUIManager->RearrangeOrder(UI_FORTUNE, TRUE);
				return WMSG_SUCCESS;
			}
			break;
		}	

	case WM_LBUTTONUP:
		{
			m_bPickTitle = FALSE;

			if (IsInside(nX, nY))
			{
				// if donation view isn't focused.
				if (!IsFocused())
					return WMSG_FAIL;

				// Close button
				if ((wmsgResult = m_btnClose.MouseMessage(pMsg)) != WMSG_FAIL)
				{
					if (wmsgResult == WMSG_COMMAND)
						CloseFortune();
					return WMSG_SUCCESS;
				}
				else if ((wmsgResult = m_btnOK.MouseMessage(pMsg)) != WMSG_FAIL)
				{
					if (wmsgResult == WMSG_COMMAND)
						CloseFortune();
					return WMSG_SUCCESS;
				}
				else if (m_lbDescription.MouseMessage(pMsg) != WMSG_FAIL)
				{
					return WMSG_SUCCESS;
				}
			}
			break;
		}
	case WM_MOUSEWHEEL:
		{
			if (IsInside(nX, nY))
			{
				if (m_lbDescription.MouseMessage(pMsg) != WMSG_FAIL)
				{
				}
				return WMSG_SUCCESS;
			}
			break;
		}
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : CloseWindowByEsc()
// Desc : 
// ----------------------------------------------------------------------------
BOOL CUIFortune::CloseWindowByEsc()
{
	if (!m_btnClose.IsEnabled())
		return FALSE;
	CloseFortune();
	return TRUE;
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc : 
// ----------------------------------------------------------------------------
void CUIFortune::MsgBoxCommand(int nCommandCode, BOOL bOK, CTString& strInput)
{
	if (nCommandCode == MSGCMD_FORTUNE)
	{
		if (bOK)
		{
			// Send message
			CNetworkMessage nmMessage;
			RequestClient::doItemMakeFortune* packet = reinterpret_cast<RequestClient::doItemMakeFortune*>(nmMessage.nm_pubMessage);
			packet->type = MSG_ITEM;
			packet->subType = MSG_ITEM_COSTUME_SUIT_FORTUNE;
			packet->tab = m_nTab;
			packet->invenIndex = m_nIdx;
			nmMessage.setSize( sizeof(*packet) );

			_pNetwork->SendToServerNew( nmMessage );

			// Show fortune UI
			OpenFortune();
		}
	}
}

// ----------------------------------------------------------------------------
// Name : OpenFortune()
// Desc : 
// ----------------------------------------------------------------------------
void CUIFortune::OpenFortune(int nTab, int inven_idx)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// If this is already exist.
	if (pUIManager->DoesMessageBoxExist(MSGCMD_FORTUNE) || IsVisible())
		return;

	CItems* pItems = &_pNetwork->MySlotItem[nTab][inven_idx];
	if (pItems->ItemData->GetFortuneIndex() <= 0)
		return;

	m_nTab = (SWORD)nTab;
	m_nIdx = (SWORD)inven_idx;

	CUIMsgBox_Info MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo(_S(5868, "포츈 코스튬"), UMBS_OKCANCEL, UI_FORTUNE, MSGCMD_FORTUNE);
	MsgBoxInfo.AddString(_S(5869, "코스튬에 깃든 운명을 지금 확인해 보시겠습니까? 한번 결정된 운명은 다시 되돌릴 수 없습니다."));
	pUIManager->CreateMessageBox(MsgBoxInfo);
}

// ----------------------------------------------------------------------------
// Name : ToggleVisible()
// Desc : 
// ----------------------------------------------------------------------------
void CUIFortune::ToggleVisible()
{
	BOOL bVisible = !IsVisible();
	if (bVisible)
		OpenFortune();
	else
		CloseFortune();
}

// ----------------------------------------------------------------------------
// Name : OpenFortune()
// Desc : 
// ----------------------------------------------------------------------------
void CUIFortune::OpenFortune()
{
	if (IsVisible() == TRUE)
		return;

	int		i;
	m_bSlow = false;
	m_delta = 80;
	m_prevTick = timeGetTime();
	m_offset = 0;

	m_bMove = true;
	m_btnOK.SetEnable(FALSE);
	m_btnClose.SetEnable(FALSE);

	CItems*		pItems = &_pNetwork->MySlotItem[m_nTab][m_nIdx];
	CItemData*	pItemData = pItems->ItemData;

	if(pItemData == NULL)
		return;

	if (pItemData->GetFortuneIndex() > 0)
	{
		m_nFortuneSkillCount = CFortuneData::getsize();

		CFortuneData::_map::iterator	iter = CFortuneData::_mapdata.begin();
		CFortuneData::_map::iterator	eiter = CFortuneData::_mapdata.end();

		for (;iter != eiter; ++iter)
		{
			stFortune* pinfo = (*iter).second;

			if (pinfo)
			{
				m_vecFortuneSkill.push_back(*pinfo);
			}				
		}

		for( i = 0; i < MAX_CARD; i++)
			m_imgCard[i].SetImageByType(CUIImageBox::IT_SKILL, m_vecFortuneSkill[i].skill_index);

		CUIManager::getSingleton()->RearrangeOrder(UI_FORTUNE, TRUE);
	}	
}

// ----------------------------------------------------------------------------
// Name : CloseFortune()
// Desc : 
// ----------------------------------------------------------------------------
void CUIFortune::CloseFortune()
{
	ResetFortune();
	CUIManager::getSingleton()->RearrangeOrder(UI_FORTUNE, FALSE);
}

// ----------------------------------------------------------------------------
// Name : ResetFortune()
// Desc : 
// ----------------------------------------------------------------------------
void CUIFortune::ResetFortune()
{
	m_bPickTitle = FALSE;
	m_nTab = -1;
	m_nIdx = -1;
	m_bMove = false;
	m_nFortuneSkillCount = 0;
	m_vecFortuneSkill.clear();
	m_nFortuneIndex = -1;
	m_lbDescription.ResetAllStrings();
}

// ----------------------------------------------------------------------------
// Name : SetFortune()
// Desc : 
// ----------------------------------------------------------------------------
void CUIFortune::SetFortune(int nItemIndex, int nSkillIndex, int nSkillLevel)
{
	if (nItemIndex == -1 && nSkillIndex == -1)
	{
		CloseFortune();
	}
	else
	{
		for (int i = 0; i < m_nFortuneSkillCount; i++)
		{
			if (m_vecFortuneSkill[i].skill_index == nSkillIndex &&
				m_vecFortuneSkill[i].skill_level == nSkillLevel)
			{
				m_bSlow = true;
				m_nFortuneIndex = i;
				break;
			}
		}
	}
}
