#include "stdh.h"

// 헤더정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Contents/Base/UIExpedition.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Contents/Base/UIPartyNew.h>
#include <Engine/Object/ActorMgr.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/Party.h>

//-----------------------------------
class CmdManageClose : public Command
{
public:
	CmdManageClose() : pUI_(NULL) {}
	void setData(CUIExpManage* pUI)
	{ 
		pUI_ = pUI;
	}
	void execute()	{
		if (pUI_)	{
			pUI_->closeUI();
		}
	}
private:
	CUIExpManage*	pUI_;
};

//-----------------------------------
class CmdManageAdd : public Command
{
public:
	CmdManageAdd() : pUI_(NULL) {}
	void setData(CUIExpManage* pUI)
	{ 
		pUI_ = pUI;
	}
	void execute()	{
		if (pUI_)	{
			pUI_->PressAddBtn();
		}
	}
private:
	CUIExpManage*	pUI_;
};

//-----------------------------------
class CmdManageMenu : public Command
{
public:
	CmdManageMenu() : pUI_(NULL), nGroup_(0), nPos_(0) {}
	void setData(CUIExpManage* pUI, int nGroup, int nPos)
	{ 
		pUI_ = pUI;
		nGroup_ = nGroup;
		nPos_ = nPos;
	}
	void execute()	{
		if (pUI_)	{
			pUI_->ShowManageMenu(nGroup_, nPos_);
		}
	}
private:
	CUIExpManage*	pUI_;
	int nGroup_;
	int nPos_;
};

//-----------------------------------
class CmdDragBase : public Command
{
public:
	CmdDragBase() : pUI_(NULL), _pBase(NULL), nGroup_(0), nPos_(0) {}
	void setData(CUIExpManage* pUI, CUIBase* pBase, int nGroup, int nPos)	
	{
		pUI_ = pUI;
		_pBase = pBase;
		nGroup_ = nGroup;
		nPos_ = nPos;
	}
	void execute()
	{
		if (pUI_ != NULL)
		{
			if (pUI_->GetBtnMove() == false)
				return;

			pUI_->SetSelectBtnGroup(nGroup_);
			pUI_->SetSelectBtnPos(nPos_);

			if (_pBase != NULL)
				UIMGR()->SetHoldBtn(_pBase);
		}
	}
private:
	CUIExpManage* pUI_;
	CUIBase*	_pBase;
	int nGroup_;
	int nPos_;
};

// ----------------------------------
class CmdViewClose : public Command
{
public:
	CmdViewClose() : pUI_(NULL) {}
	void setData(CUIViewDetail* pUI)
	{ 
		pUI_ = pUI;
	}
	void execute()	{
		if (pUI_)	{
			pUI_->closeUI();
		}
	}
private:
	CUIViewDetail*	pUI_;
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CUIExpManage::CUIExpManage()
	: m_slDestIndex(-1)
	, m_slSeletedBtnGroup(-1)
	, m_slSeletedBtnPos(-1)
	, m_bCanUseBtn(true)
	, m_ptdMessageBoxTexture(NULL)
	, m_pMoveArea(NULL)
	, m_pstrTitle(NULL)
	, m_bDrag(false)
	, m_nOriX(0), m_nOriY(0)
	, m_nMouseX(0), m_nMouseY(0)
{
	m_strDestName = "";

	int i;
	for (i = 0; i < eGROUP_MAX; ++i)
	{
		m_pStrText[i] = NULL;
	}

	for (i = 0; i < eBTN_MAX; ++i)
	{
		m_pBtn[i] = NULL;
		m_pList[i] = NULL;
	}

	setInherit(false);
}

CUIExpManage::~CUIExpManage()
{
	Destroy();

	STOCK_RELEASE(m_ptdMessageBoxTexture);
}

BOOL CUIExpManage::SetCommand(SLONG slPosition, BOOL bIsMyGroup /* = FALSE */, BOOL bIsExpeditionLeader /* = FALSE */)
{
	BOOL	bRet	= FALSE;
	m_tpList.ResetAllStrings();

	switch(slPosition)
	{
	case MSG_EXPED_MEMBERTYPE_BOSS: //원정대장
		m_tpList.AddMenuList(_S(4578, "지휘관 위임"), 0xc2bac5FF, MANAGE_CONNAMD_CHANGE_LEADER);
		m_tpList.AddMenuList(_S(4579, "부대장 임명"), 0xc2bac5FF, MANAGE_CONNAMD_SET_SUBLEADER);
		m_tpList.AddMenuList(_S(4580, "부대장 임명 해제"), 0xc2bac5FF, MANAGE_CONNAMD_RESET_SUBLEADER);
		m_tpList.AddMenuList(_S(4581, "원정대 추방"), 0xc2bac5FF, MANAGE_CONNAMD_KICK_MEMBER);
		m_tpList.AddMenuList(_S(4582, "살펴보기"), 0xc2bac5FF, MANAGE_CONNAMD_VIEW_MEMBER);
		bRet		= TRUE;
		break;

	case MSG_EXPED_MEMBERTYPE_MBOSS: //원정부대장
		{
			if(!bIsExpeditionLeader) // 원정대 부대장은 원정대장을 살펴볼 수 없다
			{
				m_tpList.AddMenuList(_S(4582, "살펴보기"), 0xc2bac5FF, MANAGE_CONNAMD_VIEW_MEMBER);

				if(bIsMyGroup)	// 원정대 부대장은 자기 그룹만 추방 가능
				{
					m_tpList.AddMenuList(_S(4583, "원정대 추방"), 0xc2bac5FF, MANAGE_CONNAMD_KICK_MEMBER);
				}

				bRet		= TRUE;
			}
		}
		break;

	default:
		break;
	}

	return bRet;
}

void CUIExpManage::RunCommand(INDEX iCommand)
{
	CUIMsgBox_Info	MsgBoxInfo;
	CTString strMessage;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(iCommand)
	{
	case MANAGE_CONNAMD_CHANGE_LEADER:
		{
			//기존 메시지박스가 있으면 닫아준다
			if(pUIManager->DoesMessageBoxExist(MSGCMD_EXPEDITION_SET_BOSS))
				pUIManager->CloseMessageBox(MSGCMD_EXPEDITION_SET_BOSS);

			MsgBoxInfo.SetMsgBoxInfo( _S(4493, "원정대" ), UMBS_YESNO, UI_PARTY, MSGCMD_EXPEDITION_SET_BOSS );

			strMessage.PrintF( _S(4584, "%s님께 원정대장을 위임하시겠습니까?" ), m_strDestName );

			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
		}
		break;
	case MANAGE_CONNAMD_SET_SUBLEADER:
		{
			//기존 메시지박스가 있으면 닫아준다
			if(pUIManager->DoesMessageBoxExist(MSGCMD_EXPEDITION_SET_MBOSS))
				pUIManager->CloseMessageBox(MSGCMD_EXPEDITION_SET_MBOSS);

			MsgBoxInfo.SetMsgBoxInfo( _S(4493, "원정대" ), UMBS_YESNO, UI_PARTY, MSGCMD_EXPEDITION_SET_MBOSS );

			strMessage.PrintF( _S(4585, "%s님을 부대장으로 임명하시겠습니까?" ), m_strDestName );

			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
		}
		break;
	case MANAGE_CONNAMD_RESET_SUBLEADER:
		{
			//기존 메시지박스가 있으면 닫아준다
			if(pUIManager->DoesMessageBoxExist(MSGCMD_EXPEDITION_RESET_MBOSS))
				pUIManager->CloseMessageBox(MSGCMD_EXPEDITION_RESET_MBOSS);

			MsgBoxInfo.SetMsgBoxInfo( _S(4493, "원정대" ), UMBS_YESNO, UI_PARTY, MSGCMD_EXPEDITION_RESET_MBOSS );

			strMessage.PrintF( _S(4586, "%s님을 부대장에서 해임하시겠습니까?" ), m_strDestName );

			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
		}
		break;
	case MANAGE_CONNAMD_KICK_MEMBER:
		{
			//기존 메시지박스가 있으면 닫아준다
			if(pUIManager->DoesMessageBoxExist(MSGCMD_EXPEDITION_KICK))
				pUIManager->CloseMessageBox(MSGCMD_EXPEDITION_KICK);

			MsgBoxInfo.SetMsgBoxInfo( _S(4493, "원정대" ), UMBS_YESNO, UI_PARTY, MSGCMD_EXPEDITION_KICK );

			strMessage.PrintF( _S(4587, "%s님을 추방하시겠습니까?" ), m_strDestName );

			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
		}
		break;
	case MANAGE_CONNAMD_VIEW_MEMBER:
		{
			GAMEDATAMGR()->GetPartyInfo()->SendExpeditionViewDetail(m_slDestIndex, m_strDestName);
		}
		break;
	default:
		break;
	}
}

void CUIExpManage::ClearExpManage()
{
	m_slDestIndex = -1;
	m_slSeletedBtnGroup = -1;
	m_slSeletedBtnPos = -1;
	m_strDestName = "";
	m_bCanUseBtn = true;

	m_tpList.Hide();
}

void CUIExpManage::OnPostRender(CDrawPort* pDraw)
{
	if (IsVisible() == FALSE)
		return;

	pDraw->InitTextureData(m_ptdMessageBoxTexture);
	m_tpList.Render();
	pDraw->FlushRenderingQueue();
	pDraw->EndTextEx();
}

void CUIExpManage::initialize()
{
	m_ptdMessageBoxTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	float fTexWidth = m_ptdMessageBoxTexture->GetPixWidth();
	float fTexHeight = m_ptdMessageBoxTexture->GetPixHeight();

	m_tpList.m_rtBackUL.SetUV( 164, 45, 171, 63, fTexWidth, fTexHeight );
	m_tpList.m_rtBackUM.SetUV( 174, 45, 176, 63, fTexWidth, fTexHeight );
	m_tpList.m_rtBackUR.SetUV( 179, 45, 186, 63, fTexWidth, fTexHeight );
	m_tpList.m_rtBackML.SetUV( 164, 55, 171, 58, fTexWidth, fTexHeight );
	m_tpList.m_rtBackMM.SetUV( 174, 55, 176, 58, fTexWidth, fTexHeight );
	m_tpList.m_rtBackMR.SetUV( 179, 55, 186, 58, fTexWidth, fTexHeight );
	m_tpList.m_rtBackLL.SetUV( 164, 60, 171, 68, fTexWidth, fTexHeight );
	m_tpList.m_rtBackLM.SetUV( 174, 60, 176, 68, fTexWidth, fTexHeight );
	m_tpList.m_rtBackLR.SetUV( 179, 60, 186, 68, fTexWidth, fTexHeight );

	m_tpList.Create( NULL, 0,  0, 100, 200, _pUIFontTexMgr->GetLineHeight(), 6, 3, 1, TRUE, TRUE );
	m_tpList.SetScrollBar( FALSE );
	m_tpList.SetSelBar( 100, _pUIFontTexMgr->GetLineHeight()+2, 187, 46, 204, 61, fTexWidth, fTexHeight );
	m_tpList.SetOverColor( 0xF8E1B5FF );
	m_tpList.SetSelectColor( 0xF8E1B5FF );
	m_tpList.Hide();

	m_pMoveArea = (CUIImage*)findUI("move_area");

	int i;
	CTString strListID[eGROUP_MAX] = { "list_group1", "list_group2", "list_group3", "list_group4"};
	CTString strGroupTitleID[eGROUP_MAX] = { "str_group1", "str_group2", "str_group3", "str_group4"};
	CTString strTemp = "";

	for (i = 0; i < eGROUP_MAX; i++)
	{
		m_pList[i] = (CUIList*)findUI(strListID[i]);
		m_pStrText[i] = (CUIText*)findUI(strGroupTitleID[i]);

		if (m_pStrText[i] != NULL)
		{
			strTemp.PrintF(_S(4494, "그룹%d"), i + 1);
			m_pStrText[i]->SetText(strTemp);
		}
	}

	CTString strBtnID[eBTN_MAX] = { "btn_close", "btn_add", "btn_cancel" };
	for (i = 0; i < eBTN_MAX; ++i)
	{
		m_pBtn[i] = (CUIButton*)findUI(strBtnID[i]);
	}

	if (m_pBtn[eBTN_CLOSE] != NULL)
	{
		CmdManageClose* pCmd = new CmdManageClose;
		pCmd->setData(this);
		m_pBtn[eBTN_CLOSE]->SetCommandUp(pCmd);
	}

	if (m_pBtn[eBTN_ADD] != NULL)
	{
		CmdManageAdd* pCmd = new CmdManageAdd;
		pCmd->setData(this);
		m_pBtn[eBTN_ADD]->SetCommandUp(pCmd);
	}

	if (m_pBtn[eBTN_CANCEL] != NULL)
	{
		CmdManageClose* pCmd = new CmdManageClose;
		pCmd->setData(this);
		m_pBtn[eBTN_CANCEL]->SetCommandUp(pCmd);
	}
}

void CUIExpManage::UpdateUI()
{
	if (IsVisible() == FALSE)
		return;

	Party* pParty = GAMEDATAMGR()->GetPartyInfo();

	if (pParty == NULL)
		return;

	int i, j;

	for (i = 0; i < eGROUP_MAX; ++i)
	{
		if (m_pList[i] == NULL)
			return;
	}

	CUIListItem*	pItem = NULL;
	CUIImageArray*	pImageArr = NULL;
	CUIText*		pText = NULL;
	CUIImage*		pImg = NULL;

	for (i = 0; i < EXPEDITION_GROUP_MAX; ++i)
	{
		for (j = 0; j < EXPEDITION_MEMBER_PER_GROUP; ++j)
		{
			bool bEmpty = pParty->IsExpedetionDataExist(i, j);

			pItem = (CUIListItem*)m_pList[i]->GetListItem(j);

			if (pItem == NULL)
				continue;

			if (bEmpty == true)
			{
				pItem->Hide(FALSE);
				
				// 직책 아이콘
				pImageArr = (CUIImageArray*)pItem->findUI("ia_pos");

				if (pImageArr != NULL)
				{
					SBYTE sbPos = pParty->GetExpeditionMemberPosition(i, j);

					if (sbPos >= MSG_EXPED_MEMBERTYPE_NORMAL)
					{
						pImageArr->Hide(TRUE);
					}
					else if (sbPos >= 0)
					{
						pImageArr->Hide(FALSE);
						pImageArr->SetRenderIdx(sbPos);
					}					
				}

				BOOL bOnline = pParty->GetExpeditionMemberOnline(i, j);
				SBYTE Job = pParty->GetExpeditionMemberJob(i, j);
				int nWidth = 0;
				// 직업 HP 바
				pImageArr = (CUIImageArray*)pItem->findUI("ia_job");
				pImg = (CUIImage*)pItem->findUI("img_hpRect");

				if (pImageArr != NULL)
				{
					if (pImg != NULL)
					{
						nWidth = calcHpBar(i, j, pImg->GetWidth());
					}

					pImageArr->SetRenderIdx(Job);
					pImageArr->SetImageWidth(Job, nWidth);					
				}
				
				// 파티원 이름
				pText = (CUIText*)pItem->findUI("str_name");

				if (pText != NULL)
				{
					COLOR color = UIMGR()->GetParty()->GetJobStringColor(Job);
					CTString strTemp;
					strTemp.PrintF("(%d)", pParty->GetExpeditionMemberLevel(i, j));

					if (bOnline == FALSE)
					{
						 color = 0x777777FF;
						 strTemp.PrintF("(off)");
					}

					pText->SetText(pParty->GetExpeditionMemberName(i, j) + strTemp);
					pText->setFontColor(color);
				}
				
				{
					CmdManageMenu* pCmd = new CmdManageMenu;
					pCmd->setData(this, i, j);
					pItem->SetCommandR(pCmd);
				}

				{
					CmdDragBase* pCmd = new CmdDragBase;
					pCmd->setData(this, (CUIBase*)pItem, i, j);
					pItem->SetCommandDrag(pCmd);
				}				
			}
			else
			{
				pItem->Hide(TRUE);
			}
		}
	}

}

void CUIExpManage::openUI()
{
	SetVisible(TRUE);
	Hide(FALSE);

	UpdateUI();

	UIMGR()->RearrangeOrder(UI_EXPEDITION, TRUE);
}

void CUIExpManage::closeUI()
{
	SetVisible(FALSE);
	Hide(TRUE);

	UIMGR()->RearrangeOrder(UI_EXPEDITION, FALSE);
}

WMSG_RESULT CUIExpManage::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
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

WMSG_RESULT CUIExpManage::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pMoveArea && m_pMoveArea->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
	}

	CUIManager::getSingleton()->RearrangeOrder( UI_EXPEDITION, TRUE );

	return WMSG_FAIL;
}

WMSG_RESULT CUIExpManage::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;


	if (UIMGR()->GetDragBase() != NULL)
	{
		Party* pParty = GAMEDATAMGR()->GetPartyInfo();
		int i, j;

		if (pParty == NULL)
			return WMSG_FAIL;
		
		CUIListItem* pItem = NULL;

		for (i = 0; i < EXPEDITION_GROUP_MAX; ++i)
		{
			for (j = 0; j < EXPEDITION_MEMBER_PER_GROUP; ++j)
			{
				if (m_pList[i] != NULL)
					pItem = (CUIListItem*)m_pList[i]->GetListItem(j);

				if (pItem == NULL)
					continue;

				if (pItem->IsInside(x, y) == TRUE)
				{
					if (pParty->IsExpedetionDataExist(i, j) == true)
					{
						CTString strTemp;

						if (pParty->GetExpeditionMemberOnline(i, j) == FALSE)
							strTemp.PrintF(_S(4590, "오프라인 멤버는 그룹을 이동할 수 없습니다."));
						else
							strTemp.PrintF(_S(4589, "이미 %s가 지정되어 있습니다."), pParty->GetExpeditionMemberName(i, j));

						UIMGR()->GetChattingUI()->AddSysMessage(strTemp);
					}
					else
					{
						SetBtnMove(false);
						pParty->SendChangeGroup(m_slSeletedBtnGroup, m_slSeletedBtnPos, i, j);
					}

					CUIManager::getSingleton()->ResetHoldBtn();
					return WMSG_SUCCESS;
				}
			}
		}
	}	

	CUIManager::getSingleton()->ResetHoldBtn();
	return WMSG_FAIL;
}

WMSG_RESULT CUIExpManage::MouseMessage( MSG *pMsg )
{
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			if(m_tpList.MouseMessage(pMsg) != WMSG_FAIL)
			{
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if(m_tpList.MouseMessage(pMsg) != WMSG_FAIL)
			{
				INDEX iIndex = m_tpList.GetCurSel();
				RunCommand(m_tpList.GetCommandNum(iIndex));
				m_tpList.Hide();

				return WMSG_SUCCESS;
			}
			else
			{
				m_tpList.Hide();
			}
		}
		break;
	}	

	return CUIBase::MouseMessage(pMsg);
}

void CUIExpManage::PressAddBtn()
{
	CUIMsgBox_Info MsgBoxInfo;
	CTString strTemp;
	MsgBoxInfo.SetMsgBoxInfo( _S(4577, "대원추가"), UMBS_OKCANCEL | UMBS_INPUTBOX, UI_PARTY, MSGCMD_EXPEDITION_INVITE_BY_NAME);	
	MsgBoxInfo.AddString( _S(4588, "추가할 캐릭터의 이름을 입력해 주세요.") );

	UIMGR()->CreateMessageBox( MsgBoxInfo );
}

int CUIExpManage::calcHpBar( int nGroup, int nPos, int nWidth )
{
	Party* pParty = GAMEDATAMGR()->GetPartyInfo();

	if (pParty == NULL)
		return 0;

	if (m_pList[nGroup] == NULL)
		return 0;

	if (pParty->IsExpedetionDataExist(nGroup, nPos) == false)
		return 0;

	if (pParty->GetExpeditionMemberOnline(nGroup, nPos) == FALSE)
		return 0;

	FLOAT fRatio = 0.0f;
	fRatio = (FLOAT)pParty->GetExpeditionMemberHp(nGroup, nPos) / (FLOAT)pParty->GetExpeditionMemberMaxHp(nGroup, nPos);

	if (fRatio > 1.0f)
		fRatio = 1.0f;
	else if (fRatio < 0.0f)
		fRatio = 0.0f;

	int nCurValue = 0;
	nCurValue = nWidth * fRatio;

	return nCurValue;
}

void CUIExpManage::ShowManageMenu( int nGroup, int nPos )
{
	Party* pParty = GAMEDATAMGR()->GetPartyInfo();

	if (pParty == NULL)
		return;

	if(pParty->IsExpedetionDataExist(nGroup, nPos) == true)
	{
		m_tpList.SetPos(m_nMouseX, m_nMouseY);

		// 내정보 획득
		int slMyPosition = pParty->GetExpeditionMyPosition();
		int slMyGroup, slMyPos;
		pParty->SearchExpeditionMember(_pNetwork->MyCharacterInfo.index ,slMyGroup, slMyPos);

		//  상대 정보 획득
		BOOL bIsMyGroup = ( nGroup == slMyGroup );
		BOOL bIsExpeditionLeader = ( MSG_EXPED_MEMBERTYPE_BOSS == pParty->GetExpeditionMemberPosition(nGroup, nPos));
		m_strDestName = pParty->GetExpeditionMemberName(nGroup, nPos);
		m_slDestIndex = pParty->GetExpeditionMemberIndex(nGroup, nPos);

		if(SetCommand(slMyPosition, bIsMyGroup, bIsExpeditionLeader))
			m_tpList.Show();
		else
			m_tpList.Hide();
	}
	else
	{
		m_tpList.Hide();
	}
}

void CUIExpManage::UpdateMemberInfo( int nGroup, int nPos )
{
	if (IsVisible() == FALSE)
		return;

	Party* pParty = GAMEDATAMGR()->GetPartyInfo();

	if (pParty == NULL)
		return;

	if (m_pList[nGroup] == NULL)
		return;

	CUIListItem*	pItem = NULL;
	CUIImageArray*	pImageArr = NULL;
	CUIImage*		pImg = NULL;
	
	bool bEmpty = pParty->IsExpedetionDataExist(nGroup, nPos);
	pItem = (CUIListItem*)m_pList[nGroup]->GetListItem(nPos);

	if (pItem == NULL)
		return;
		
	if (bEmpty == true)
	{
		BOOL bOnline = pParty->GetExpeditionMemberOnline(nGroup, nPos);
	    SBYTE Job = pParty->GetExpeditionMemberJob(nGroup, nPos);
	    int nWidth = 0;
	    // 직업 HP 바
	    pImageArr = (CUIImageArray*)pItem->findUI("ia_job");
	    pImg = (CUIImage*)pItem->findUI("img_hpRect");

	    if (pImageArr != NULL)
	    {
	        if (pImg != NULL)
	        {
	            nWidth = calcHpBar(nGroup, nPos, pImg->GetWidth());
	        }

	        pImageArr->SetRenderIdx(Job);
	        pImageArr->SetImageWidth(Job, nWidth);
	    }
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CUIViewDetail::CUIViewDetail()
	: m_pIaBack(NULL)
	, m_pbtnClose(NULL)
	, m_pstrName(NULL)
	, m_pMoveArea(NULL)
{
	for (int i = 0; i < WEAR_TOTAL; i++)
	{
		m_pIcon[i] = NULL;
	}

	setInherit(false);
}

void CUIViewDetail::ClearViewDetail()
{
	m_sPetInfo.lIndex			= -1;
	m_sPetInfo.sbPetTypeGrade	= -1;
	m_sPetInfo.lLevel			= -1;
	m_sPetInfo.llExp			= -1;
	m_sPetInfo.llNeedExp		= -1;
	m_sPetInfo.lHP				= -1;
	m_sPetInfo.lMaxHP			= 0;
	m_sPetInfo.lAbility		= 0;
	m_sPetInfo.lHungry			= -1;
	m_sPetInfo.lMaxHungry		= -1;
	m_sPetInfo.lSympathy		= -1;
	m_sPetInfo.lMaxSympathy	= -1;
	m_sPetInfo.lRemainRebirth	= 0;
	m_sPetInfo.strNameCard		= CTString("");

	m_sWildPetInfo.pet_index = -1;
	m_sWildPetInfo.pet_name = CTString("");
	m_sWildPetInfo.pet_level = -1;
	m_sWildPetInfo.pet_str = -1;
	m_sWildPetInfo.pet_con = -1;
	m_sWildPetInfo.pet_dex = -1;
	m_sWildPetInfo.pet_int = -1;

	for (int i = 0; i < WEAR_TOTAL; ++i)
	{
		if (m_pIcon[i] != NULL)
			m_pIcon[i]->clearIconData();
	}
}

void CUIViewDetail::initialize()
{
	m_pIaBack = (CUIImageArray*)findUI("ia_back");
	m_pstrName = (CUIText*)findUI("str_name");
	m_pMoveArea = (CUIImage*)findUI("move_area");
	m_pbtnClose = (CUIButton*)findUI("btn_close");

	if (m_pbtnClose != NULL)
	{
		CmdViewClose* pCmd = new CmdViewClose;
		pCmd->setData(this);
		m_pbtnClose->SetCommandUp(pCmd);
	}

	CTString strIconId;
	for (int i = 0; i < WEAR_TOTAL; ++i)
	{
		strIconId.PrintF("icon_wear%d", i);
		m_pIcon[i] = (CUIIcon*)findUI(strIconId);

		if (m_pIcon[i] != NULL)
			m_pIcon[i]->clearIconData();
	}
}

void CUIViewDetail::openUI()
{
	SetVisible(TRUE);
	Hide(FALSE);

	UIMGR()->RearrangeOrder(UI_EXPEDITION_VIEW, TRUE);
}

void CUIViewDetail::closeUI()
{
	SetVisible(FALSE);
	Hide(TRUE);

	ClearViewDetail();
	UIMGR()->RearrangeOrder(UI_EXPEDITION_VIEW, FALSE);
}

WMSG_RESULT CUIViewDetail::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

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

WMSG_RESULT CUIViewDetail::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pMoveArea && m_pMoveArea->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
	}

	CUIManager::getSingleton()->RearrangeOrder( UI_EXPEDITION_VIEW, TRUE );
	return WMSG_FAIL;
}

WMSG_RESULT CUIViewDetail::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	CUIManager::getSingleton()->ResetHoldBtn();
	return WMSG_FAIL;
}

void CUIViewDetail::SetViewItem( int nWearPos, CItems* pItem )
{
	if (nWearPos < 0 || nWearPos >= WEAR_TOTAL)
		return;

	if (m_pIcon[nWearPos] == NULL)
		return;

	m_pIcon[nWearPos]->Hide(FALSE);
	m_pIcon[nWearPos]->SetWhichUI(UI_EXPEDITION_VIEW);
	m_pIcon[nWearPos]->setData(pItem, false);
}

void CUIViewDetail::SetCharInfo( CTString strCharInfo, SBYTE sbJob )
{
	if (m_pstrName != NULL)
		m_pstrName->SetText(strCharInfo);


	if (sbJob < 0 || sbJob >= TOTAL_JOB)
		return;

	if (m_pIaBack != NULL)
		m_pIaBack->SetRenderIdx(sbJob);
}
