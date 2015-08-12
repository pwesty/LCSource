#include "stdh.h"

// 헤더 정리. [12/3/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/LocalDefine.h>
#include "UIMail.h"


// ----------------------------------------------------------------------------
// Name : CUIWareHouse()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIMail::CUIMail()
{
	m_TabStatus = TABSTATUS::LIST;
	m_Page = 0;
	m_bCheckClaim = FALSE;
	m_ItemCount = 0;
	m_bMoneyOver = 0;
	initMoney();
	m_NpcUID = -1;
	m_SelectedIndex = -1;
	//SendMailItemInit();
	m_bSending = FALSE;
	m_SendMailItemOnSlot = 0;
}

// ----------------------------------------------------------------------------
// Name : ~CUIWareHouse()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIMail::~CUIMail()
{
#ifndef POSTSOCKETSYSTEM
	return;
#endif 
	initstring();
	SetSendMailString(FALSE);
}

void CUIMail::init()
{
#ifdef POSTSOCKETSYSTEM
	m_TabStatus = TABSTATUS::LIST;
	m_Page = 0;
	CMail::getInstance()->InitMailData();
	m_bCheckClaim = FALSE;
	m_ItemCount = 0;
	m_bMoneyOver = 0;
	initMoney();
	m_NpcUID = -1;
	m_SelectedIndex = -1;

	m_fNPCPosX = 0.0;
	m_fNPCPosZ = 0.0;

	m_UIOpenMail.CloseMail();
	initstring();
	CMail::getInstance()->InitSendMailItem();
	SendMailItemBtnInit();
	m_bSending = FALSE;
	SetSending(TRUE);
#endif 
}


void CUIMail::SetFocusReciver(BOOL mode)
{
	m_ebSendMailReciver.SetFocus(mode);
}

void CUIMail::initstring()
{
	m_ebSendMailReciver.ResetString();
	m_ebSendMailReciver.SetFocus(FALSE);
	m_ebSendMailTitle.ResetString();
	m_ebSendMailTitle.SetFocus(FALSE);
	m_mebSendMailText.ResetString();
	m_mebSendMailText.SetFocus(FALSE);
}
void CUIMail::initMoney()
{
	m_Money = 0;
}
void CUIMail::SetSendMailString(BOOL mode)
{
	m_ebSendMailReciver.SetEnable(mode);
	m_ebSendMailTitle.SetEnable(mode);
	m_mebSendMailText.SetEnable(mode);
}
void CUIMail::SendMailItemBtnInit()
{
	for(int i = 0; i < ITEMBUTTONMAX; ++i)
	{
		m_btnSendMailItem[i].InitBtn();
	}
	m_ItemCount = 0;
}
void CUIMail::SendMailItemBtnInitIndex(int index)
{
	m_btnSendMailItem[index].InitBtn();
	--m_ItemCount;
}

void CUIMail::SendMailItemBtnInitUniIndex(int uniindex)
{
	for(int i = 0; i < ITEMBUTTONMAX; ++i)
	{
		if(m_btnSendMailItem[i].GetItemUniIndex() == uniindex)
		{
			m_btnSendMailItem[i].InitBtn();
			--m_ItemCount;
		}
	}
}

BOOL CUIMail::CheckSendMail()
{
	//받을 사람 제목 내용중 하나라도 빈 내용이 있으면 보내지 않는다.
	CTString reciver = m_ebSendMailReciver.GetString();
	if( strlen(reciver.str_String)<=0 )
	{
		_pNetwork->ClientSystemMessage(_S( 5656, "받을 분의 이름이 입력되지 않았습니다."), SYSMSG_ERROR);
		return FALSE;
	}	
	else if( strlen(m_ebSendMailTitle.GetString())<=0 )
	{
		_pNetwork->ClientSystemMessage(_S( 5657, "편지의 제목이 입력되지 않았습니다."), SYSMSG_ERROR);
		return FALSE;
	}
	return TRUE;
}

void CUIMail::OpenMailListReq(LONG npcuid, FLOAT npcposx, FLOAT npcposz, BOOL bRequest)
{
	CUIManager* pUIManager = CUIManager::getSingleton();
#if defined (POSTSOCKETSYSTEM)
	if (!IsVisible())
	{
		if (bRequest  && !m_bSending)
		{
			PostSystem::MyMailListRequest(npcuid);
			init();
			m_NpcUID = npcuid;
			m_fNPCPosX = npcposx;
			m_fNPCPosZ = npcposz;
			m_bSending = TRUE;
		}
		else if (!bRequest && m_bSending)
		{
			CDrawPort* pDrawPort = pUIManager->GetDrawPort();
			int	nX = ( pDrawPort->dp_MinI + pDrawPort->dp_MaxI ) / 2 - m_nWidth / 2;
			int	nY = ( pDrawPort->dp_MinJ + pDrawPort->dp_MaxJ ) / 2 - m_nHeight / 2;

			SetPos( nX, nY );
			m_TabStatus = TABSTATUS::LIST;
			m_Page = 0;
			m_bSending = FALSE;
			pUIManager->GetMail()->OpenMailListRep();
		}
	}
#endif//POSTSOCKETSYSTEM
}

void CUIMail::OpenMailListRep()
{
#if defined (POSTSOCKETSYSTEM)
	if (!IsVisible())
	{
		CUIManager* pUIManager = CUIManager::getSingleton();
		pUIManager->RearrangeOrder( UI_MAIL, TRUE );
	}
#endif//POSTSOCKETSYSTEM
}

void CUIMail::CloseMailList()
{
#if defined (POSTSOCKETSYSTEM)
	init();
	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->RearrangeOrder( UI_MAIL, FALSE );
	pUIManager->CloseMessageBox( MSGCMD_SENDMAIL_COUNT_INPUT );
	pUIManager->CloseMessageBox( MSGCMD_SENDMAIL_COUNT_ADD );
	pUIManager->CloseMessageBox( MSGCMD_SENDMAIL_MONEY_INPUT );

#endif
}

void CUIMail::SetTab(TABSTATUS::TABSTATUS tabstatus)
{
	m_TabStatus = tabstatus;
}

void CUIMail::ReplySet()
{
#if defined (POSTSOCKETSYSTEM)
	MAILELEMENT&	OpenMailData = CMail::getInstance()->GetMailElement();

	m_TabStatus = TABSTATUS::SEND;
	m_ebSendMailReciver.SetString(OpenMailData.senderName.str_String);
	CTString temStr;
	temStr.PrintF("%s%s",_S(5646, "답신"),OpenMailData.title);
	m_ebSendMailTitle.SetString(temStr.str_String);
#endif //POSTSOCKETSYSTEM
}

CUIMail::CUIOpenMail* CUIMail::GetOpenMail()
{
	return &m_UIOpenMail;
}

BOOL CUIMail::CheckClaim()
{
	if(!CheckSendMail())
		return FALSE;
	if(m_ItemCount <= 0)
	{
		_pNetwork->ClientSystemMessage(_S( 5660, "대금 청구 편지에는 아이템이 존재해야 합니다."), SYSMSG_ERROR);
		return FALSE;
	}
	if (m_Money < 1)
	{
		_pNetwork->ClientSystemMessage(_S( 5661, "금액을 적지 않아서 대금청구 메시지를 보낼 수 없습니다."), SYSMSG_ERROR);
		return FALSE;
	}
	return TRUE;
}
//아이템이 편지에는 첨부되어 있으나 인벤토리에는 없을 때 실패
BOOL CUIMail::CheckDeletItem()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	for(int i = 0; i < ITEMBUTTONMAX; ++i)
	{
		if(!m_btnSendMailItem[i].IsEmpty())
			if(pUIManager->GetInventory()->GetItemIndex(m_btnSendMailItem[i].GetItemTab(),m_btnSendMailItem[i].GetItemRow(),m_btnSendMailItem[i].GetItemCol()) == -1)
			{
				_pNetwork->ClientSystemMessage(_S( 5697, "인벤토리에 첨부된 아이템이 없습니다."), SYSMSG_ERROR);
				return FALSE;
			}
	}
	return TRUE;
}

int CUIMail::GetItemCount()
{
	return m_ItemCount;
}

BOOL CUIMail::CheckSendMailItemAdd()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CItemData ItemData = _pNetwork->GetItemData(pUIManager->GetHoldBtn().GetIndex());
	int flag = ItemData.GetFlag();
	if(pUIManager->GetHoldBtn().GetItemIndex() == 19)
	{
		return FALSE;
	}
	//ITEM_FLAG_CASH, ITEM_FLAG_NO_STASH, ITEM_FLAG_ABS, ITEM_FLAG_ZONEMOVE_DEL, ITEM_FLAG_QUEST, ITEM_FLAG_CASTLLAN, ITEM_FLAG_COSTUME2
	//중에 하나라도 걸리면 실패
	else if(flag & ITEM_FLAG_CASH  || flag & ITEM_FLAG_NO_STASH  || flag & ITEM_FLAG_ABS  ||
		flag & ITEM_FLAG_ZONEMOVE_DEL  || flag & ITEM_FLAG_QUEST  || flag & ITEM_FLAG_CASTLLAN  || flag & ITEM_FLAG_COSTUME2)
	{
		return FALSE;
	}
	else if(pUIManager->GetHoldBtn().GetItemWearType() != -1)
	{
		return FALSE;
	}
	return TRUE;
}
CUIButtonEx* CUIMail::GetMailListItemIcon()
{
	return m_btnListMailItem;
}
void CUIMail::SetMoneyOver(BOOL bOver)
{
	m_bMoneyOver = bOver;
}
BOOL CUIMail::GetMoneyOver()
{
	return m_bMoneyOver;
}
LONG CUIMail::GetNPCUID()
{
	return m_NpcUID;
}

const FLOAT CUIMail::GetNPCPosX()
{
	return m_fNPCPosX;
}
const FLOAT CUIMail::GetNPCPosZ()
{
	return m_fNPCPosZ;
}
void CUIMail::SetSending(BOOL mode)
{
	m_btnConfirm.SetEnable(mode);
}
BOOL CUIMail::GetSending()
{
	return m_btnConfirm.IsEnabled();
}
void CUIMail::MailStatusCommand()
{
#if defined(POSTSOCKETSYSTEM)
	CUIManager* pUIManager = CUIManager::getSingleton();
	STATUS::STATUS status = CMail::getInstance()->GetStatus();

	switch(status)
	{
	case STATUS::post_system_mail_send_ok:
		initstring();
		SendMailItemBtnInit();
		initMoney();
		SetFocusReciver(TRUE);
		if(!GetSending())
			SetSending(TRUE);
		if(pUIManager->GetInventory()->IsLocked())
			pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_SENDMAIL );
		CMail::getInstance()->SetSTatus(STATUS::done);
		break;
	case STATUS::post_system_mail_send_err:
		if(!GetSending())
			SetSending(TRUE);
		if(pUIManager->GetInventory()->IsLocked())
			pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_SENDMAIL );
		CMail::getInstance()->SetSTatus(STATUS::done);
		break;
	case STATUS::post_system_mail_read:
		//CMail::getInstance()->GetMailData().SetOpenMail(selectmailitr);
		m_UIOpenMail.OpenOpenMail();
		CMail::getInstance()->SetSTatus(STATUS::done);
	    break;
	case STATUS::post_system_mail_del:
		pUIManager->GetMail()->GetOpenMail()->CloseMail();
		CMail::getInstance()->SetSTatus(STATUS::done);
	    break;
	case STATUS::post_system_mail_itemrecv:
		{
			CUIButtonEx* btn = m_UIOpenMail.GetOpenMailItemButtonEX();
			for(int i = 0; i < ITEMBUTTONMAX; ++i)
			{
				btn[i].SetEnable(TRUE);
			}

		}
		break;
	default:
	    break;
	}
#endif //POSTSOCKETSYSTEM
}




void CUIMail::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
#if defined (POSTSOCKETSYSTEM)
	CUIManager* pUIManager = CUIManager::getSingleton();

	switch( nCommandCode )
	{
	case MSGCMD_SENDMAIL_COUNT_INPUT:
		{
			int inputnum = atoi(strInput.str_String);
			if (m_btnSendMailItem[m_SendMailItemOnSlot].GetItemCount() < inputnum)
			{
				_pNetwork->ClientSystemMessage(_S( 5011, "아이템 개수가 부족합니다. 아이템 개수를 확인해 주세요."), SYSMSG_ERROR);

				CTString		strMessage;
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 4288, "아이템" ), UMBS_OK | UMBS_INPUTBOX,
					UI_MAIL, MSGCMD_SENDMAIL_COUNT_INPUT );
				const char	*szItemName = _pNetwork->GetItemName(m_btnSendMailItem[m_SendMailItemOnSlot].GetItemIndex());
				strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );
				MsgBoxInfo.AddString( strMessage );
				pUIManager->CreateMessageBox( MsgBoxInfo );

			}
			else if(bOK && strInput.IsInteger())
			{
				m_btnSendMailItem[m_SendMailItemOnSlot].SetItemCount(atoi(strInput.str_String));

				CMail::getInstance()->GetSendItemData(m_SendMailItemOnSlot).itemCount = atoi(strInput.str_String);
			}
			else if(!strInput.IsInteger())
			{
				_pNetwork->ClientSystemMessage(_S( 5694, "개수 입력은 숫자만 가능합니다."), SYSMSG_ERROR);
			}
		}
		break;
	case MSGCMD_SENDMAIL_COUNT_ADD:
		{
			int inputnum = m_btnSendMailItem[m_SendMailItemOnSlot].GetItemCount()+atoi(strInput.str_String);
			if (m_btnSendMailItem[m_SendMailItemOnSlot].GetItemCount() < inputnum)
			{
				_pNetwork->ClientSystemMessage(_S( 5011, "아이템 개수가 부족합니다. 아이템 개수를 확인해 주세요."), SYSMSG_ERROR);

				CTString		strMessage;
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 4288, "아이템" ), UMBS_OK | UMBS_INPUTBOX,
					UI_MAIL, MSGCMD_SENDMAIL_COUNT_ADD );
				const char	*szItemName = _pNetwork->GetItemName(m_btnSendMailItem[m_SendMailItemOnSlot].GetItemIndex());
				strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );
				MsgBoxInfo.AddString( strMessage );
				pUIManager->CreateMessageBox( MsgBoxInfo );
			}
			else if(bOK && strInput.IsInteger())
			{
				m_btnSendMailItem[m_SendMailItemOnSlot].SetItemCount(m_btnSendMailItem[m_SendMailItemOnSlot].GetItemCount()+atoi(strInput.str_String));

				CMail::getInstance()->GetSendItemData(m_SendMailItemOnSlot).itemCount = atoi(strInput.str_String);
			}
			else if(!strInput.IsInteger())
			{
				_pNetwork->ClientSystemMessage(_S( 5694, "개수 입력은 숫자만 가능합니다."), SYSMSG_ERROR);
			}
		}
		break;
	case MSGCMD_SENDMAIL_MONEY_INPUT:
		{
			if(bOK && strInput.IsInteger())
			{
				LONGLONG tmepMoney = _atoi64(strInput);
				if(tmepMoney > 1000000000000)
				{
					_pNetwork->ClientSystemMessage(_S( 5692, "입력할 수 있는 나스 한도가 초과 되었습니다. 확인 후 이용 부탁합니다."), SYSMSG_ERROR);
					m_Money = 0;
					break;
				}			
				m_Money = tmepMoney;
				if (m_Money > _pNetwork->MyCharacterInfo.money)
					SetMoneyOver(TRUE);
				else
					SetMoneyOver(FALSE);
			}
		}
		break;
	case MSGCMD_DELETMAIL_CHECK:
		{
			// Delete는 Open Menu에서만 수행할 수 있다
			if (bOK)
			{
				PostSystem::DeletetoMailRequest(CMail::getInstance()->GetMailElement().serverIndex);
			}
		}
	default:
		break;
	}
#endif //POSTSOCKETSYSTEM
}
BOOL CUIMail::IsEditBoxFocused()
{
	return (m_ebSendMailReciver.IsFocused() ||
		m_ebSendMailTitle.IsFocused() ||
		m_mebSendMailText.IsFocused());
}

void CUIMail::KillFocusEditBox()
{
	m_ebSendMailReciver.SetFocus(FALSE);
	m_ebSendMailTitle.SetFocus(FALSE);
	m_mebSendMailText.SetFocus(FALSE);
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIMail::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
#ifndef POSTSOCKETSYSTEM
	return;
#endif 
#if defined (POSTSOCKETSYSTEM)
	m_pParent = (CUIBase*)pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	m_UIOpenMail.Create(this, nX+MAINBACKGROUNDWIDTH+10, nY, OPENBACKGROUNDWIDTH, OPENBACKGROUNDHEIGHT);

	m_ptdBaseTexture = CreateTexture(CTString("Data\\Interface\\postNcraft.tex"));
	FLOAT fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_rcMainTitle.SetRect( 0, 0, 395, 30 );
	m_rcListTab.SetRect(8, 33, 120, 54);
// [12/5/2012 박훈] 빌라 요청으로 인한 우편시스템 보내기 버튼 삭제(추후 우편NPC 제거 예정)
#if !(defined G_BRAZIL) && !(defined G_MAXICO)
	m_rcSendTab.SetRect(122, 33, 234, 54);
#endif
	m_rcListMailPage.SetRect(164, 371, 231, 390);

	for(int i = 0;i < PAGEMAILMAX; ++i)
	{
		m_rcListMailIcon[i].SetRect(13, 60+(38*i), 46, 93+(38*i));
		m_rcListMailInfo[i].SetRect(48, 60+(38*i), 381, 93+(38*i));
		m_btnListMailItem[i].Create(this, m_rcListMailIcon[i].Left, m_rcListMailIcon[i].Top, m_rcListMailIcon[i].GetWidth(), m_rcListMailIcon[i].GetHeight(), UI_MAIL, UBET_ITEM);
	}

	m_rcSendMailTextBackground.SetRect(8, 55, 388, 282);
	
	m_rcSendMailCheckClaim.SetRect(8, 290, 20, 302);
	m_rcSendMailCheckSend.SetRect(89, 290, 101, 302);

	m_rcSendMailMoneyIcon.SetRect(188, 287, 206, 305);

	m_rcSendMailItemBackground.SetRect(8, 309, 388, 393);
	
	for(int i = 0; i < ITEMBUTTONMAX; ++i)
	{
		int plusWidth = i/ITEMBUTTONLINEMAX*39;
		int plusHeight = i%ITEMBUTTONLINEMAX*71;


// 		m_rcSendMailItemEmpty[i].SetRect(39+plusWidth, 315+plusHeight, 73+plusWidth, 349+plusHeight);
// 		m_btnSendMailItem[i].Create(this, 39+plusWidth, 315+plusHeight, 34, 34,UI_MAIL,UBET_ITEM);
		m_rcSendMailItemEmpty[i].SetRect(MAINBACKGROUNDWIDTH/2-17, 334, MAINBACKGROUNDWIDTH/2+17, 368);
		m_btnSendMailItem[i].Create(this, m_rcSendMailItemEmpty[i].Left, m_rcSendMailItemEmpty[i].Top, m_rcSendMailItemEmpty[i].GetWidth(), m_rcSendMailItemEmpty[i].GetHeight(),UI_MAIL,UBET_ITEM);
	}
	

	m_ebSendMailReciver.Create( this, 126, 63, 258, 17, 40);
	m_ebSendMailReciver.SetReadingWindowUV( 329, 121, 415, 138, fTexWidth, fTexHeight );
	m_ebSendMailReciver.SetCandidateUV( 329, 121, 415, 138, fTexWidth, fTexHeight );
	
	m_ebSendMailTitle.Create(this, 126, 84, 258, 17, 40);
	m_ebSendMailTitle.SetReadingWindowUV( 329, 121, 415, 138, fTexWidth, fTexHeight );
	m_ebSendMailTitle.SetCandidateUV( 329, 121, 415, 138, fTexWidth, fTexHeight );

	m_btnMoney.Create(this, _s(""), m_rcSendMailMoneyIcon.Left, m_rcSendMailMoneyIcon.Top, m_rcSendMailMoneyIcon.GetWidth(), m_rcSendMailMoneyIcon.GetHeight());
	m_btnMoney.SetUV(UBS_IDLE, 420, 170, 438, 188, fTexWidth,fTexHeight);
	m_btnMoney.SetUV(UBS_CLICK, 420, 170, 438, 188, fTexWidth,fTexHeight);
	m_btnMoney.CopyUV(UBS_IDLE, UBS_DISABLE);
	m_btnMoney.CopyUV(UBS_IDLE, UBS_ON);

	m_mebSendMailText.Create(this, 16, 108, 361, 167, 122);

	initstring();
	SetSendMailString(TRUE);
	//선택 테두리
	m_rtSelectBorderUL.SetUV(363,222,363+11,222+11,fTexWidth,fTexHeight);
	m_rtSelectBorderUM.SetUV(363+11,222,429-11,222+11,fTexWidth,fTexHeight);
	m_rtSelectBorderUR.SetUV(429-11,222,429,222+11,fTexWidth,fTexHeight);
	
	m_rtSelectBorderML.SetUV(363,222+11,363+11,256-11,fTexWidth,fTexHeight);
	m_rtSelectBorderMM.SetUV(363+11,222+11,429-11,256-11,fTexWidth,fTexHeight);
	m_rtSelectBorderMR.SetUV(429-11,222+11,429,256-11,fTexWidth,fTexHeight);
	
	m_rtSelectBorderLL.SetUV(363,256-11,363+11,256,fTexWidth,fTexHeight);
	m_rtSelectBorderLM.SetUV(363+11,256-11,429-11,256,fTexWidth,fTexHeight);
	m_rtSelectBorderLR.SetUV(429-11,256-11,429,256,fTexWidth,fTexHeight);	

	m_rcSendMailReciver.SetRect(125, 63, 382, 80);
	m_rcSendMailTitle.SetRect(125, 84, 382, 101);
	m_rcSendMailMoney.SetRect(212, 287, 388,304);
	m_rcSendMailText.SetRect(16, 108, 370, 275);
	

	m_rtBrownGradationRectL.SetUV(329, 121, 330, 138 ,fTexWidth, fTexHeight);
	m_rtBrownGradationRectM.SetUV(330, 121, 414, 138 ,fTexWidth, fTexHeight);
	m_rtBrownGradationRectR.SetUV(414, 121, 415, 138 ,fTexWidth, fTexHeight);

	m_rtItemCapIcon.SetUV(292, 156, 326, 190, fTexWidth, fTexHeight);
	m_rtEmptyMailIcon.SetUV(469, 216, 503, 250, fTexWidth, fTexHeight);
	m_rtTabSelect.SetUV( 193, 194, 305, 215, fTexWidth, fTexHeight );
	m_rtTab.SetUV( 193, 219, 305, 240, fTexWidth, fTexHeight );

	m_rtBrownRectL.SetUV(328, 155, 329, 190, fTexWidth, fTexHeight);
	m_rtBrownRectM.SetUV(330, 155, 413, 190, fTexWidth, fTexHeight);
	m_rtBrownRectR.SetUV(414, 155, 415, 190, fTexWidth, fTexHeight);

	m_rtEmptyItemIcon.SetUV( 292, 120, 326, 154, fTexWidth, fTexHeight );

	m_rtMainBackgroundUL.SetUV(0, 0, 120, 40, fTexWidth, fTexHeight);
	m_rtMainBackgroundUM.SetUV(120, 0, MAINBACKGROUNDWIDTHTEX - 120, 40, fTexWidth, fTexHeight);
	m_rtMainBackgroundUR.SetUV(MAINBACKGROUNDWIDTHTEX -120, 0 , MAINBACKGROUNDWIDTHTEX, 40, fTexWidth, fTexHeight);

	m_rtMainBackgroundML.SetUV(0, 40, 120, MAINBACKGROUNDHEIGHTTEX -10, fTexWidth, fTexHeight );
	m_rtMainBackgroundMM.SetUV(120, 40, MAINBACKGROUNDWIDTHTEX - 120, MAINBACKGROUNDHEIGHTTEX -10, fTexWidth, fTexHeight);
	m_rtMainBackgroundMR.SetUV(MAINBACKGROUNDWIDTHTEX -120, 40 , MAINBACKGROUNDWIDTHTEX, MAINBACKGROUNDHEIGHTTEX -10, fTexWidth, fTexHeight);
	
	m_rtMainBackgroundLL.SetUV(0, MAINBACKGROUNDHEIGHTTEX -10, 120, MAINBACKGROUNDHEIGHTTEX, fTexWidth, fTexHeight );
	m_rtMainBackgroundLM.SetUV(120, MAINBACKGROUNDHEIGHTTEX -10, MAINBACKGROUNDWIDTHTEX - 120, MAINBACKGROUNDHEIGHTTEX, fTexWidth, fTexHeight);
	m_rtMainBackgroundLR.SetUV(MAINBACKGROUNDWIDTHTEX -120, MAINBACKGROUNDHEIGHTTEX -10 , MAINBACKGROUNDWIDTHTEX, MAINBACKGROUNDHEIGHTTEX, fTexWidth, fTexHeight);

	m_rcListMailBackground.SetRect(8, 55, 388, 364);

	m_rtBrownBackgroundU.SetUV(412, 0, 512, 1, fTexWidth, fTexHeight);
	
	m_rtBrownBackgroundML.SetUV(412, 0, 414, 52, fTexWidth, fTexHeight);
	m_rtBrownBackgroundMM.SetUV(415, 30, 510, 51, fTexWidth, fTexHeight);
	m_rtBrownBackgroundMR.SetUV(510, 0, 512, 52, fTexWidth, fTexHeight);
	
	m_rtBrownBackgroundL.SetUV(412, 52, 512, 53, fTexWidth, fTexHeight);

	m_rtMoneyIcon.SetUV(420, 170, 438, 188, fTexWidth,fTexHeight);
	m_rtCheckboxOn.SetUV(407, 194, 422, 206, fTexWidth,fTexHeight);
	m_rtCheckboxOff.SetUV(426, 194, 438, 206, fTexWidth,fTexHeight);

	m_rtGreenBackguoundU.SetUV(412,56,512,57, fTexWidth,fTexHeight);
	
	m_rtGreenBackguoundML.SetUV(412, 57, 413, 108, fTexWidth,fTexHeight);
	m_rtGreenBackguoundMM.SetUV(413, 57, 511, 108, fTexWidth,fTexHeight);
	m_rtGreenBackguoundMR.SetUV(511, 57, 512, 108, fTexWidth,fTexHeight);
	
	m_rtGreenBackguoundL.SetUV(412, 108, 512, 109, fTexWidth,fTexHeight);

	m_btnFirst.Create(this, _s(""), 131, 371, 13, 19);
	m_btnFirst.SetUV(UBS_IDLE, 444, 170, 457, 189, fTexWidth, fTexHeight);
	m_btnFirst.SetUV(UBS_CLICK, 444, 193, 457, 212,fTexWidth, fTexHeight);
	m_btnFirst.CopyUV(UBS_IDLE, UBS_DISABLE);
	m_btnFirst.CopyUV(UBS_IDLE, UBS_ON);

	m_btnLeft.Create(this, _s(""), 146,	371, 13, 19);
	m_btnLeft.SetUV(UBS_IDLE, 459, 170, 472, 189,fTexWidth,fTexHeight);
	m_btnLeft.SetUV(UBS_CLICK, 459, 193, 472, 212,fTexWidth,fTexHeight);
	m_btnLeft.CopyUV(UBS_IDLE, UBS_DISABLE);
	m_btnLeft.CopyUV(UBS_IDLE, UBS_ON);
	
	m_btnRight.Create(this, _s(""), 236, 371, 13, 19);
	m_btnRight.SetUV(UBS_IDLE, 474, 170, 487, 189,fTexWidth,fTexHeight);
	m_btnRight.SetUV(UBS_CLICK, 474, 193, 487, 212,fTexWidth,fTexHeight);
	m_btnRight.CopyUV(UBS_IDLE, UBS_DISABLE);
	m_btnRight.CopyUV(UBS_IDLE, UBS_ON);
	
	m_btnLast.Create(this, _s(""), 251, 371, 13, 19);
	m_btnLast.SetUV(UBS_IDLE, 489, 170, 502, 189,fTexWidth,fTexHeight);
	m_btnLast.SetUV(UBS_CLICK, 489, 193, 502, 212,fTexWidth,fTexHeight);
	m_btnLast.CopyUV(UBS_IDLE, UBS_DISABLE);
	m_btnLast.CopyUV(UBS_IDLE, UBS_ON);

	m_btnConfirm.Create(this, _S(1417, "확인"), 66, 399, 122, 20);
	m_btnConfirm.SetUV(UBS_IDLE, 420, 120, 502, 140, fTexWidth, fTexHeight);
	m_btnConfirm.SetUV(UBS_CLICK, 420, 144, 502, 164, fTexWidth, fTexHeight);
	m_btnConfirm.CopyUV(UBS_IDLE, UBS_DISABLE);
	m_btnConfirm.CopyUV(UBS_IDLE, UBS_ON);

	m_btnCancel.Create(this, _S(1419,"취소"),207, 399, 122, 20);
	m_btnCancel.SetUV(UBS_IDLE, 420, 120, 502, 140, fTexWidth, fTexHeight);
	m_btnCancel.SetUV(UBS_CLICK, 420, 144, 502, 164, fTexWidth, fTexHeight);
	m_btnCancel.CopyUV(UBS_IDLE, UBS_DISABLE);
	m_btnCancel.CopyUV(UBS_IDLE, UBS_ON);

	m_btnClose.Create(this, _s(""),374, 4, 15, 15);
	m_btnClose.SetUV(UBS_IDLE, 310, 209, 323, 222, fTexWidth, fTexHeight);
	m_btnClose.SetUV(UBS_CLICK, 310, 194, 323, 207, fTexWidth, fTexHeight);
	m_btnClose.CopyUV(UBS_IDLE, UBS_DISABLE);
	m_btnClose.CopyUV(UBS_IDLE, UBS_ON);
#endif //(POSTSOCKETSYSTEM)
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIMail::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI ) / 2 - GetWidth(), ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIMail::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}


// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIMail::Render()
{
#ifdef POSTSOCKETSYSTEM
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();
	MailStatusCommand();


	pDrawPort->InitTextureData(m_ptdBaseTexture);

	int	nX, nY, i;

	//////////////////////////////////////////////////////////////////////////
	// Main Background begin
	nX = m_nPosX;
	nY = m_nPosY;
	pDrawPort->AddTexture(nX,nY,nX+120,nY+40,
		m_rtMainBackgroundUL.U0,m_rtMainBackgroundUL.V0,m_rtMainBackgroundUL.U1,m_rtMainBackgroundUL.V1,0xFFFFFFFF);
	pDrawPort->AddTexture(nX+120,nY,nX+MAINBACKGROUNDWIDTH-120,nY+40,
		m_rtMainBackgroundUM.U0,m_rtMainBackgroundUM.V0,m_rtMainBackgroundUM.U1,m_rtMainBackgroundUM.V1,0xFFFFFFFF);
	pDrawPort->AddTexture(nX+MAINBACKGROUNDWIDTH-120,nY,nX+MAINBACKGROUNDWIDTH,nY+40,
		m_rtMainBackgroundUR.U0,m_rtMainBackgroundUR.V0,m_rtMainBackgroundUR.U1,m_rtMainBackgroundUR.V1,0xFFFFFFFF);
	
	pDrawPort->AddTexture(nX,nY+40,nX+120,nY+MAINBACKGROUNDHEIGHT-10,
		m_rtMainBackgroundML.U0,m_rtMainBackgroundML.V0,m_rtMainBackgroundML.U1,m_rtMainBackgroundML.V1,0xFFFFFFFF);
	pDrawPort->AddTexture(nX+120,nY+40,nX+MAINBACKGROUNDWIDTH-120,nY+MAINBACKGROUNDHEIGHT-10,
		m_rtMainBackgroundMM.U0,m_rtMainBackgroundMM.V0,m_rtMainBackgroundMM.U1,m_rtMainBackgroundMM.V1,0xFFFFFFFF);
	pDrawPort->AddTexture(nX+MAINBACKGROUNDWIDTH-120,nY+40,nX+MAINBACKGROUNDWIDTH,nY+MAINBACKGROUNDHEIGHT-10,
		m_rtMainBackgroundMR.U0,m_rtMainBackgroundMR.V0,m_rtMainBackgroundMR.U1,m_rtMainBackgroundMR.V1,0xFFFFFFFF);
	
	pDrawPort->AddTexture(nX,nY+MAINBACKGROUNDHEIGHT-10,nX+120,nY+MAINBACKGROUNDHEIGHT,
		m_rtMainBackgroundLL.U0,m_rtMainBackgroundLL.V0,m_rtMainBackgroundLL.U1,m_rtMainBackgroundLL.V1,0xFFFFFFFF);
	pDrawPort->AddTexture(nX+120,nY+MAINBACKGROUNDHEIGHT-10,nX+MAINBACKGROUNDWIDTH-120,nY+MAINBACKGROUNDHEIGHT,
		m_rtMainBackgroundLM.U0,m_rtMainBackgroundLM.V0,m_rtMainBackgroundLM.U1,m_rtMainBackgroundLM.V1,0xFFFFFFFF);
	pDrawPort->AddTexture(nX+MAINBACKGROUNDWIDTH-120,nY+MAINBACKGROUNDHEIGHT-10,nX+MAINBACKGROUNDWIDTH,nY+MAINBACKGROUNDHEIGHT,
		m_rtMainBackgroundLR.U0,m_rtMainBackgroundLR.V0,m_rtMainBackgroundLR.U1,m_rtMainBackgroundLR.V1,0xFFFFFFFF);

	// Main Background end
	//////////////////////////////////////////////////////////////////////////
	m_btnConfirm.Render();
	m_btnCancel.Render();
	m_btnClose.Render();

	switch(m_TabStatus)
	{
	case TABSTATUS::LIST:
		{
			//////////////////////////////////////////////////////////////////////////
			// List background rendering begin
				//메일 리스트 탭
			pDrawPort->AddTexture(nX + m_rcListTab.Left, nY + m_rcListTab.Top, nX + m_rcListTab.Right, nY + m_rcListTab.Bottom,
				m_rtTabSelect.U0, m_rtTabSelect.V0, m_rtTabSelect.U1, m_rtTabSelect.V1, 0xFFFFFFFF);
			
			CTString tempstring = _S( 5649, "받은 편지" );
			pDrawPort->PutTextExCX( tempstring, nX + m_rcListTab.Left + m_rcListTab.GetWidth()/2, nY + m_rcListTab.Top + 5);
// [12/5/2012 박훈] 빌라 요청으로 인한 우편시스템 보내기 버튼 삭제(추후 우편NPC 제거 예정)
#if !(defined G_BRAZIL) && !(defined G_MAXICO)
			pDrawPort->AddTexture(nX + m_rcSendTab.Left, nY + m_rcSendTab.Top, nX + m_rcSendTab.Right, nY + m_rcSendTab.Bottom,
				m_rtTab.U0, m_rtTab.V0, m_rtTab.U1, m_rtTab.V1, 0xFFFFFFFF);

			tempstring = _S( 5650, "보낼 편지" );
			pDrawPort->PutTextExCX( tempstring, nX + m_rcSendTab.Left + m_rcSendTab.GetWidth()/2, nY + m_rcSendTab.Top + 5);
#endif
			//백그라운드
			pDrawPort->AddTexture(nX + m_rcListMailBackground.Left, nY + m_rcListMailBackground.Top, nX + m_rcListMailBackground.Right, nY+m_rcListMailBackground.Top+1,
				m_rtBrownBackgroundU.U0, m_rtBrownBackgroundU.V0, m_rtBrownBackgroundU.U1, m_rtBrownBackgroundU.V1, 0xFFFFFFFF);

			pDrawPort->AddTexture(nX + m_rcListMailBackground.Left, nY + m_rcListMailBackground.Top, nX + m_rcListMailBackground.Left+1, nY+m_rcListMailBackground.Bottom,
				m_rtBrownBackgroundML.U0, m_rtBrownBackgroundML.V0, m_rtBrownBackgroundML.U1, m_rtBrownBackgroundML.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nX + m_rcListMailBackground.Left+1, nY + m_rcListMailBackground.Top+1, nX + m_rcListMailBackground.Right -1, nY+m_rcListMailBackground.Bottom-1,
				m_rtBrownBackgroundMM.U0, m_rtBrownBackgroundMM.V0, m_rtBrownBackgroundMM.U1, m_rtBrownBackgroundMM.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nX + m_rcListMailBackground.Right-1, nY + m_rcListMailBackground.Top, nX + m_rcListMailBackground.Right, nY+m_rcListMailBackground.Bottom,
				m_rtBrownBackgroundMR.U0, m_rtBrownBackgroundMR.V0, m_rtBrownBackgroundMR.U1, m_rtBrownBackgroundMR.V1, 0xFFFFFFFF);

			pDrawPort->AddTexture(nX + m_rcListMailBackground.Left, nY + m_rcListMailBackground.Bottom-1, nX + m_rcListMailBackground.Right, nY+m_rcListMailBackground.Bottom,
				m_rtBrownBackgroundL.U0, m_rtBrownBackgroundL.V0, m_rtBrownBackgroundL.U1, m_rtBrownBackgroundL.V1, 0xFFFFFFFF);

			for(i = 0; i < PAGEMAILMAX; ++i)
			{
				pDrawPort->AddTexture(nX + m_rcListMailIcon[i].Left, nY + m_rcListMailIcon[i].Top, nX + m_rcListMailIcon[i].Right, nY + m_rcListMailIcon[i].Bottom,
					m_rtEmptyMailIcon.U0, m_rtEmptyMailIcon.V0, m_rtEmptyMailIcon.U1, m_rtEmptyMailIcon.V1, 0xFFFFFFFF);
				pDrawPort->AddTexture(nX + m_rcListMailInfo[i].Left, nY + m_rcListMailInfo[i].Top, nX + m_rcListMailInfo[i].Left+1, nY + m_rcListMailInfo[i].Bottom,
					m_rtBrownRectL.U0, m_rtBrownRectL.V0, m_rtBrownRectL.U1, m_rtBrownRectL.V1, 0xFFFFFFFF);
				pDrawPort->AddTexture(nX + m_rcListMailInfo[i].Left+1, nY + m_rcListMailInfo[i].Top, nX + m_rcListMailInfo[i].Right-1, nY + m_rcListMailInfo[i].Bottom,
					m_rtBrownRectM.U0, m_rtBrownRectM.V0, m_rtBrownRectM.U1, m_rtBrownRectM.V1, 0xFFFFFFFF);
				pDrawPort->AddTexture(nX + m_rcListMailInfo[i].Right-1, nY + m_rcListMailInfo[i].Top, nX + m_rcListMailInfo[i].Right, nY + m_rcListMailInfo[i].Bottom,
					m_rtBrownRectR.U0, m_rtBrownRectR.V0, m_rtBrownRectR.U1, m_rtBrownRectR.V1, 0xFFFFFFFF);
			}
			// List background rendering end
			//////////////////////////////////////////////////////////////////////////


			CMailDataList& mailList = CMail::getInstance()->GetMailDataList();
			if(!mailList.empty())
			{
				// Mail Info box end
				int tailNum = PAGEMAILMAX;
				int bodyNum = mailList.size()/PAGEMAILMAX;
				
				if (m_Page == bodyNum)
					tailNum = mailList.size()%PAGEMAILMAX;

				// List loop 시작
				//MAILELEMENT&	UsingMailData = CMail::getInstance()->GetMailElement();

				INDEX iStart = m_Page*PAGEMAILMAX;
				INDEX iEnd = m_Page*PAGEMAILMAX + tailNum;
				INDEX iCount = 0;

				for(INDEX i = iStart; i < iEnd; ++i, ++iCount)
				{
					if(m_SelectedIndex == iCount)
					{
						// Select box 이미지는 가운데 색상에 Alpha처리가 되어 있지 않으므로, 먼저 그려야 한다.
						// Select box render begin
						pDrawPort->AddTexture(nX+m_rcListMailIcon[iCount].Left-3,
												nY + m_rcListMailIcon[iCount].Top-2,
												nX+m_rcListMailIcon[iCount].Left-3+11,
												nY+m_rcListMailIcon[iCount].Top-2+11,
												m_rtSelectBorderUL.U0,m_rtSelectBorderUL.V0,m_rtSelectBorderUL.U1,m_rtSelectBorderUL.V1,0xFFFFFFFF);
						pDrawPort->AddTexture(nX+m_rcListMailIcon[iCount].Left-3+11,
												nY + m_rcListMailIcon[iCount].Top-2,
												nX+m_rcListMailInfo[iCount].Right+3-11,
												nY+m_rcListMailIcon[iCount].Top-2+11,
												m_rtSelectBorderUM.U0,m_rtSelectBorderUM.V0,m_rtSelectBorderUM.U1,m_rtSelectBorderUM.V1,0xFFFFFFFF);
						pDrawPort->AddTexture(nX+m_rcListMailInfo[iCount].Right+3-11,
												nY + m_rcListMailIcon[iCount].Top-2,
												nX+m_rcListMailInfo[iCount].Right+3,
												nY+m_rcListMailIcon[iCount].Top-2+11,
												m_rtSelectBorderUR.U0,m_rtSelectBorderUR.V0,m_rtSelectBorderUR.U1,m_rtSelectBorderUR.V1,0xFFFFFFFF);
						pDrawPort->AddTexture(nX+m_rcListMailIcon[iCount].Left-3,
												nY + m_rcListMailIcon[iCount].Top-2+11,
												nX+m_rcListMailIcon[iCount].Left-3+11,
												nY + m_rcListMailIcon[iCount].Bottom+2-11,
												m_rtSelectBorderML.U0,m_rtSelectBorderML.V0,m_rtSelectBorderML.U1,m_rtSelectBorderML.V1,0xFFFFFFFF);
						pDrawPort->AddTexture(nX+m_rcListMailIcon[iCount].Left-3+11,
												nY + m_rcListMailIcon[iCount].Top-2+11,
												nX+m_rcListMailInfo[iCount].Right+3-11,
												nY + m_rcListMailIcon[iCount].Bottom+2-11,
												m_rtSelectBorderMM.U0,m_rtSelectBorderMM.V0,m_rtSelectBorderMM.U1,m_rtSelectBorderMM.V1,0xFFFFFFFF);
						pDrawPort->AddTexture(nX+m_rcListMailInfo[iCount].Right+3-11,
												nY + m_rcListMailIcon[iCount].Top-2+11,
												nX+m_rcListMailInfo[iCount].Right+3,
												nY + m_rcListMailIcon[iCount].Bottom+2-11,
												m_rtSelectBorderMR.U0,m_rtSelectBorderMR.V0,m_rtSelectBorderMR.U1,m_rtSelectBorderMR.V1,0xFFFFFFFF);
						pDrawPort->AddTexture(nX+m_rcListMailIcon[iCount].Left-3,
												nY + m_rcListMailIcon[iCount].Bottom+2-11,
												nX+m_rcListMailIcon[iCount].Left-3+11,
												nY + m_rcListMailIcon[iCount].Bottom+2,
												m_rtSelectBorderLL.U0,m_rtSelectBorderLL.V0,m_rtSelectBorderLL.U1,m_rtSelectBorderLL.V1,0xFFFFFFFF);
						pDrawPort->AddTexture(nX+m_rcListMailIcon[iCount].Left-3+11,
												nY + m_rcListMailIcon[iCount].Bottom+2-11,
												nX+m_rcListMailInfo[iCount].Right+3-11,
												nY + m_rcListMailIcon[iCount].Bottom+2,
												m_rtSelectBorderLM.U0,m_rtSelectBorderLM.V0,m_rtSelectBorderLM.U1,m_rtSelectBorderLM.V1,0xFFFFFFFF);
						pDrawPort->AddTexture(nX+m_rcListMailInfo[iCount].Right+3-11,
												nY + m_rcListMailIcon[iCount].Bottom+2-11,
												nX+m_rcListMailInfo[iCount].Right+3,
												nY + m_rcListMailIcon[iCount].Bottom+2,
												m_rtSelectBorderLR.U0,m_rtSelectBorderLR.V0,m_rtSelectBorderLR.U1,m_rtSelectBorderLR.V1,0xFFFFFFFF);
						// Select box render end
						//인포 박스를 그위에 다시 하나 그려 준다.
						// Mail Info box begin
						pDrawPort->AddTexture(nX + m_rcListMailInfo[iCount].Left,
													nY + m_rcListMailInfo[iCount].Top,
													nX + m_rcListMailInfo[iCount].Left+1,
													nY + m_rcListMailInfo[iCount].Bottom,
													m_rtBrownRectL.U0, m_rtBrownRectL.V0, m_rtBrownRectL.U1, m_rtBrownRectL.V1, 0xFFFFFFFF);
						pDrawPort->AddTexture(nX + m_rcListMailInfo[iCount].Left+1,
													nY + m_rcListMailInfo[iCount].Top,
													nX + m_rcListMailInfo[iCount].Right-1,
													nY + m_rcListMailInfo[iCount].Bottom,
													m_rtBrownRectM.U0, m_rtBrownRectM.V0, m_rtBrownRectM.U1, m_rtBrownRectM.V1, 0xFFFFFFFF);
						pDrawPort->AddTexture(nX + m_rcListMailInfo[iCount].Right-1,
													nY + m_rcListMailInfo[iCount].Top,
													nX + m_rcListMailInfo[iCount].Right,
													nY + m_rcListMailInfo[iCount].Bottom,
													m_rtBrownRectR.U0, m_rtBrownRectR.V0, m_rtBrownRectR.U1, m_rtBrownRectR.V1, 0xFFFFFFFF);
					}

					// Mail Info box end
					pDrawPort->FlushRenderingQueue();
					pDrawPort->EndTextEx();

					int iconindex = 731;
					if (mailList[i].iconIndex != -1)
					{
						iconindex = mailList[i].iconIndex;
					}
					//아이템 아이콘 나오게 하는 부분이 들어간다.
					m_btnListMailItem[iCount].SetItemInfo(-1,-1,-1,iconindex,-1,-1);
					if(mailList[i].mailType == 1 || mailList[i].mailType == 3)
					{
						m_btnListMailItem[iCount].SetItemColor(0xFFFFFFFF);
					}
					else if(mailList[i].mailType == 2)
					{
						m_btnListMailItem[iCount].SetItemColor(0x6F6F6FFF);
					}
					m_btnListMailItem[iCount].Render();
					pDrawPort->FlushBtnRenderingQueue(UBET_ITEM);
					pDrawPort->InitTextureData(m_ptdBaseTexture);

					if(mailList[i].mailType == 3)
						pDrawPort->PutTextExCX( _S(5645,"반송"), nX + m_rcListMailIcon[iCount].Left + 17,nY+m_rcListMailIcon[iCount].Top+10);

					CTString data = mailList[i].title;
					data.TrimRight(24);
					pDrawPort->PutTextEx( data, nX + m_rcListMailInfo[iCount].Left + 8, nY + m_rcListMailInfo[iCount].Top + 10);
					data = mailList[i].senderName;
					data.TrimRight(16);
					pDrawPort->PutTextEx( data, nX + m_rcListMailInfo[iCount].Left + 198, nY + m_rcListMailInfo[iCount].Top + 10);
					int lastTime = 0;
					if(mailList[i].mailType == 1 || mailList[i].mailType == 3)
					{
						lastTime = NOOPENTIME - mailList[i].time;
					}
					else if(mailList[i].mailType == 2)
					{
						lastTime = OPENTIME - mailList[i].time;
					}
					if(lastTime/DAYTIME > 0)
						data.PrintF( _S( 2511, "%d일" ), lastTime/DAYTIME );
					else if(lastTime/HOURTIME > 0)
						data.PrintF( _S( 2512, "%d시간" ), lastTime/DAYTIME );
					else if(lastTime/HALFTIME > 0)
						data.PrintF( _S( 5647, "30분 이상" ));
					else
						data.PrintF( _S( 5648, "30분 미만" ));
					
					pDrawPort->PutTextExRX( data, nX + m_rcListMailInfo[iCount].Left + 327, nY + m_rcListMailInfo[iCount].Top + 10);
				}
			}
			pDrawPort->AddTexture(nX+m_rcListMailPage.Left, nY+m_rcListMailPage.Top, nX+m_rcListMailPage.Left+1, nY+m_rcListMailPage.Bottom,
				m_rtBrownGradationRectL.U0, m_rtBrownGradationRectL.V0, m_rtBrownGradationRectL.U1, m_rtBrownGradationRectL.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nX+m_rcListMailPage.Left+1, nY+m_rcListMailPage.Top, nX+m_rcListMailPage.Right-1, nY+m_rcListMailPage.Bottom,
				m_rtBrownGradationRectM.U0, m_rtBrownGradationRectM.V0, m_rtBrownGradationRectM.U1, m_rtBrownGradationRectM.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nX+m_rcListMailPage.Right-1, nY+m_rcListMailPage.Top, nX+m_rcListMailPage.Right, nY+m_rcListMailPage.Bottom,
				m_rtBrownGradationRectR.U0, m_rtBrownGradationRectR.V0, m_rtBrownGradationRectR.U1, m_rtBrownGradationRectR.V1, 0xFFFFFFFF);
			CTString data;
			data.PrintF("%d",m_Page+1);
			pDrawPort->PutTextExCX(data, nX + m_rcListMailPage.Left + 33, nY + m_rcListMailPage.Top + 2);
			
			m_btnFirst.Render();
			m_btnLeft.Render();
			m_btnRight.Render();
			m_btnLast.Render();
		}
		break;
	case TABSTATUS::SEND:
		{
				//메일 쓰기 탭
			pDrawPort->AddTexture(nX + m_rcListTab.Left, nY + m_rcListTab.Top, nX + m_rcListTab.Right, nY + m_rcListTab.Bottom,
				m_rtTab.U0, m_rtTab.V0, m_rtTab.U1, m_rtTab.V1, 0xFFFFFFFF);
			CTString tempstring = _S( 5649, "받은 편지" );
			pDrawPort->PutTextExCX( tempstring, nX + m_rcListTab.Left + m_rcListTab.GetWidth()/2, nY + m_rcListTab.Top + 5);
// [12/5/2012 박훈] 빌라 요청으로 인한 우편시스템 보내기 버튼 삭제(추후 우편NPC 제거 예정)
#if !(defined G_BRAZIL) && !(defined G_MAXICO)
			pDrawPort->AddTexture(nX + m_rcSendTab.Left, nY + m_rcSendTab.Top, nX + m_rcSendTab.Right, nY + m_rcSendTab.Bottom,
				m_rtTabSelect.U0, m_rtTabSelect.V0, m_rtTabSelect.U1, m_rtTabSelect.V1, 0xFFFFFFFF);

			tempstring = _S( 5650, "보낼 편지" );
			pDrawPort->PutTextExCX( tempstring, nX + m_rcSendTab.Left + m_rcSendTab.GetWidth()/2, nY + m_rcSendTab.Top + 5);
#endif
//상부 백그라운드 (편지내용)			
			pDrawPort->AddTexture(nX + m_rcSendMailTextBackground.Left, nY + m_rcSendMailTextBackground.Top, nX + m_rcSendMailTextBackground.Right, nY+m_rcSendMailTextBackground.Top+1,
				m_rtBrownBackgroundU.U0, m_rtBrownBackgroundU.V0, m_rtBrownBackgroundU.U1, m_rtBrownBackgroundU.V1, 0xFFFFFFFF);
			
			pDrawPort->AddTexture(nX + m_rcSendMailTextBackground.Left, nY + m_rcSendMailTextBackground.Top, nX + m_rcSendMailTextBackground.Left+1, nY+m_rcSendMailTextBackground.Bottom,
				m_rtBrownBackgroundML.U0, m_rtBrownBackgroundML.V0, m_rtBrownBackgroundML.U1, m_rtBrownBackgroundML.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nX + m_rcSendMailTextBackground.Left + 1, nY + m_rcSendMailTextBackground.Top + 1, nX + m_rcSendMailTextBackground.Right -1, nY+m_rcSendMailTextBackground.Bottom-1,
				m_rtBrownBackgroundMM.U0, m_rtBrownBackgroundMM.V0, m_rtBrownBackgroundMM.U1, m_rtBrownBackgroundMM.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nX + m_rcSendMailTextBackground.Right-1, nY + m_rcSendMailTextBackground.Top, nX + m_rcSendMailTextBackground.Right, nY+m_rcSendMailTextBackground.Bottom,
				m_rtBrownBackgroundMR.U0, m_rtBrownBackgroundMR.V0, m_rtBrownBackgroundMR.U1, m_rtBrownBackgroundMR.V1, 0xFFFFFFFF);
			
			pDrawPort->AddTexture(nX + m_rcSendMailTextBackground.Left, nY + m_rcSendMailTextBackground.Bottom-1, nX + m_rcSendMailTextBackground.Right, nY+m_rcSendMailTextBackground.Bottom,
				m_rtBrownBackgroundL.U0, m_rtBrownBackgroundL.V0, m_rtBrownBackgroundL.U1, m_rtBrownBackgroundL.V1, 0xFFFFFFFF);
//하부 백그라운드(첨부아이템)
			pDrawPort->AddTexture(nX + m_rcSendMailItemBackground.Left, nY + m_rcSendMailItemBackground.Top, nX + m_rcSendMailItemBackground.Right, nY+m_rcSendMailItemBackground.Top+1,
				m_rtBrownBackgroundU.U0, m_rtBrownBackgroundU.V0, m_rtBrownBackgroundU.U1, m_rtBrownBackgroundU.V1, 0xFFFFFFFF);
			
			pDrawPort->AddTexture(nX + m_rcSendMailItemBackground.Left, nY + m_rcSendMailItemBackground.Top, nX + m_rcSendMailItemBackground.Left+1, nY+m_rcSendMailItemBackground.Bottom,
				m_rtBrownBackgroundML.U0, m_rtBrownBackgroundML.V0, m_rtBrownBackgroundML.U1, m_rtBrownBackgroundML.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nX + m_rcSendMailItemBackground.Left + 1, nY + m_rcSendMailItemBackground.Top + 1, nX + m_rcSendMailItemBackground.Right -1, nY+m_rcSendMailItemBackground.Bottom-1,
				m_rtBrownBackgroundMM.U0, m_rtBrownBackgroundMM.V0, m_rtBrownBackgroundMM.U1, m_rtBrownBackgroundMM.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nX + m_rcSendMailItemBackground.Right-1, nY + m_rcSendMailItemBackground.Top, nX + m_rcSendMailItemBackground.Right, nY+m_rcSendMailItemBackground.Bottom,
				m_rtBrownBackgroundMR.U0, m_rtBrownBackgroundMR.V0, m_rtBrownBackgroundMR.U1, m_rtBrownBackgroundMR.V1, 0xFFFFFFFF);
			
			pDrawPort->AddTexture(nX + m_rcSendMailItemBackground.Left, nY + m_rcSendMailItemBackground.Bottom-1, nX + m_rcSendMailItemBackground.Right, nY+m_rcSendMailItemBackground.Bottom,
				m_rtBrownBackgroundL.U0, m_rtBrownBackgroundL.V0, m_rtBrownBackgroundL.U1, m_rtBrownBackgroundL.V1, 0xFFFFFFFF);

			//받는이
			pDrawPort->AddTexture(nX+m_rcSendMailReciver.Left, nY+m_rcSendMailReciver.Top, nX+m_rcSendMailReciver.Left+1, nY+m_rcSendMailReciver.Bottom,
				m_rtBrownGradationRectL.U0, m_rtBrownGradationRectL.V0, m_rtBrownGradationRectL.U1, m_rtBrownGradationRectL.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nX+m_rcSendMailReciver.Left+1, nY+m_rcSendMailReciver.Top, nX+m_rcSendMailReciver.Right-1, nY+m_rcSendMailReciver.Bottom,
				m_rtBrownGradationRectM.U0, m_rtBrownGradationRectM.V0, m_rtBrownGradationRectM.U1, m_rtBrownGradationRectM.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nX+m_rcSendMailReciver.Right-1, nY+m_rcSendMailReciver.Top, nX+m_rcSendMailReciver.Right, nY+m_rcSendMailReciver.Bottom,
				m_rtBrownGradationRectR.U0, m_rtBrownGradationRectR.V0, m_rtBrownGradationRectR.U1, m_rtBrownGradationRectR.V1, 0xFFFFFFFF);
			tempstring = _S( 5654, "받는 사람" );
			pDrawPort->PutTextExCX( tempstring, nX + m_rcSendMailReciver.Left -50, nY + m_rcSendMailReciver.Top + 1);
			tempstring = _S( 341, "제목" );
			pDrawPort->PutTextExCX( tempstring, nX + m_rcSendMailTitle.Left -50, nY + m_rcSendMailTitle.Top + 1);


			//제목
			pDrawPort->AddTexture(nX+m_rcSendMailTitle.Left, nY+m_rcSendMailTitle.Top, nX+m_rcSendMailTitle.Left+1, nY+m_rcSendMailTitle.Bottom,
				m_rtBrownGradationRectL.U0, m_rtBrownGradationRectL.V0, m_rtBrownGradationRectL.U1, m_rtBrownGradationRectL.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nX+m_rcSendMailTitle.Left+1, nY+m_rcSendMailTitle.Top, nX+m_rcSendMailTitle.Right-1, nY+m_rcSendMailTitle.Bottom,
				m_rtBrownGradationRectM.U0, m_rtBrownGradationRectM.V0, m_rtBrownGradationRectM.U1, m_rtBrownGradationRectM.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nX+m_rcSendMailTitle.Right-1, nY+m_rcSendMailTitle.Top, nX+m_rcSendMailTitle.Right, nY+m_rcSendMailTitle.Bottom,
				m_rtBrownGradationRectR.U0, m_rtBrownGradationRectR.V0, m_rtBrownGradationRectR.U1, m_rtBrownGradationRectR.V1, 0xFFFFFFFF);
			//돈
			pDrawPort->AddTexture(nX+m_rcSendMailMoney.Left, nY+m_rcSendMailMoney.Top, nX+m_rcSendMailMoney.Left+1, nY+m_rcSendMailMoney.Bottom,
				m_rtBrownGradationRectL.U0, m_rtBrownGradationRectL.V0, m_rtBrownGradationRectL.U1, m_rtBrownGradationRectL.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nX+m_rcSendMailMoney.Left+1, nY+m_rcSendMailMoney.Top, nX+m_rcSendMailMoney.Right-1, nY+m_rcSendMailMoney.Bottom,
				m_rtBrownGradationRectM.U0, m_rtBrownGradationRectM.V0, m_rtBrownGradationRectM.U1, m_rtBrownGradationRectM.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nX+m_rcSendMailMoney.Right-1, nY+m_rcSendMailMoney.Top, nX+m_rcSendMailMoney.Right, nY+m_rcSendMailMoney.Bottom,
				m_rtBrownGradationRectR.U0, m_rtBrownGradationRectR.V0, m_rtBrownGradationRectR.U1, m_rtBrownGradationRectR.V1, 0xFFFFFFFF);
			//내용
			pDrawPort->AddTexture(nX + m_rcSendMailText.Left, nY + m_rcSendMailText.Top, nX + m_rcSendMailText.Right, nY+m_rcSendMailText.Top+1,
				m_rtGreenBackguoundU.U0, m_rtGreenBackguoundU.V0, m_rtGreenBackguoundU.U1, m_rtGreenBackguoundU.V1, 0xFFFFFFFF);
			
			pDrawPort->AddTexture(nX + m_rcSendMailText.Left, nY + m_rcSendMailText.Top, nX + m_rcSendMailText.Left+1, nY+m_rcSendMailText.Bottom,
				m_rtGreenBackguoundML.U0, m_rtGreenBackguoundML.V0, m_rtGreenBackguoundML.U1, m_rtGreenBackguoundML.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nX + m_rcSendMailText.Left+1, nY + m_rcSendMailText.Top+1, nX + m_rcSendMailText.Right -1, nY+m_rcSendMailText.Bottom-1,
				m_rtGreenBackguoundMM.U0, m_rtGreenBackguoundMM.V0, m_rtGreenBackguoundMM.U1, m_rtGreenBackguoundMM.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(nX + m_rcSendMailText.Right-1, nY + m_rcSendMailText.Top, nX + m_rcSendMailText.Right, nY+m_rcSendMailText.Bottom,
				m_rtGreenBackguoundMR.U0, m_rtGreenBackguoundMR.V0, m_rtGreenBackguoundMR.U1, m_rtGreenBackguoundMR.V1, 0xFFFFFFFF);
			
			pDrawPort->AddTexture(nX + m_rcSendMailText.Left, nY + m_rcSendMailText.Bottom-1, nX + m_rcSendMailText.Right, nY+m_rcSendMailText.Bottom,
				m_rtGreenBackguoundL.U0, m_rtGreenBackguoundL.V0, m_rtGreenBackguoundL.U1, m_rtGreenBackguoundL.V1, 0xFFFFFFFF);
//			청구 시스템이 1차 개발 내용에 포함 되지 않음 때문에 완전 제거
// 			if(m_bCheckClaim)
// 			{
// 				pDrawPort->AddTexture(nX+m_rcSendMailCheckClaim.Left, nY+m_rcSendMailCheckClaim.Top-1, nX+m_rcSendMailCheckClaim.Right+3, nY+m_rcSendMailCheckClaim.Bottom,
// 					m_rtCheckboxOn.U0, m_rtCheckboxOn.V0, m_rtCheckboxOn.U1, m_rtCheckboxOn.V1, 0xFFFFFFFF);
// 				
// 				pDrawPort->AddTexture(nX+m_rcSendMailCheckSend.Left, nY+m_rcSendMailCheckSend.Top, nX+m_rcSendMailCheckSend.Right, nY+m_rcSendMailCheckSend.Bottom,
// 					m_rtCheckboxOff.U0, m_rtCheckboxOff.V0, m_rtCheckboxOff.U1, m_rtCheckboxOff.V1, 0xFFFFFFFF);
// 			}
// 			else
// 			{
// 				pDrawPort->AddTexture(nX+m_rcSendMailCheckClaim.Left, nY+m_rcSendMailCheckClaim.Top, nX+m_rcSendMailCheckClaim.Right, nY+m_rcSendMailCheckClaim.Bottom,
// 					m_rtCheckboxOff.U0, m_rtCheckboxOff.V0, m_rtCheckboxOff.U1, m_rtCheckboxOff.V1, 0xFFFFFFFF);
// 				pDrawPort->AddTexture(nX+m_rcSendMailCheckSend.Left, nY+m_rcSendMailCheckSend.Top-1, nX+m_rcSendMailCheckSend.Right+3, nY+m_rcSendMailCheckSend.Bottom,
// 					m_rtCheckboxOn.U0, m_rtCheckboxOn.V0, m_rtCheckboxOn.U1, m_rtCheckboxOn.V1, 0xFFFFFFFF);
// 			}
// 			tempstring = _S(5653,"청구");
// 			pDrawPort->PutTextEx( tempstring, nX + m_rcSendMailCheckClaim.Left +20, nY + m_rcSendMailCheckClaim.Top);
// 			tempstring = _S(3090,"보내기");
// 			pDrawPort->PutTextEx( tempstring, nX + m_rcSendMailCheckSend.Left +20, nY + m_rcSendMailCheckSend.Top);
 			tempstring.PrintF( "%I64d", m_Money);
			CUIManager::getSingleton()->InsertCommaToString( tempstring );
			if(m_bMoneyOver)
				pDrawPort->PutTextExRX(tempstring, nX + m_rcSendMailMoney.Right - 3, nY + m_rcSendMailMoney.Top + 1,0xFF0000FF);
			else
				pDrawPort->PutTextExRX(tempstring, nX + m_rcSendMailMoney.Right - 3, nY + m_rcSendMailMoney.Top + 1);

			
			pDrawPort->AddTexture(nX+m_rcSendMailMoneyIcon.Left, nY+m_rcSendMailMoneyIcon.Top, nX+m_rcSendMailMoneyIcon.Right, nY+m_rcSendMailMoneyIcon.Bottom,
				m_rtMoneyIcon.U0, m_rtMoneyIcon.V0, m_rtMoneyIcon.U1, m_rtMoneyIcon.V1, 0xFFFFFFFF);


			for(i = 0; i<ITEMBUTTONMAX; ++i)
			{
				pDrawPort->AddTexture(nX+m_rcSendMailItemEmpty[i].Left, nY+m_rcSendMailItemEmpty[i].Top, nX+m_rcSendMailItemEmpty[i].Right, nY+m_rcSendMailItemEmpty[i].Bottom,
					m_rtEmptyItemIcon.U0, m_rtEmptyItemIcon.V0, m_rtEmptyItemIcon.U1, m_rtEmptyItemIcon.V1, 0xFFFFFFFF);
			}
			m_btnMoney.Render();

			pDrawPort->FlushRenderingQueue();
			pDrawPort->EndTextEx();
			
			for(i = 0; i < ITEMBUTTONMAX; ++i)
			{
					if( !m_btnSendMailItem[i].IsEmpty() )
					{
						m_btnSendMailItem[i].Render();
						pDrawPort->FlushBtnRenderingQueue(UBET_ITEM);
						m_btnSendMailItem[i].RenderInfoPopup();
						pDrawPort->FlushRenderingQueue();
					}	
			}
			

			//편리한 addtexture ㅋㅋㅋㅋ
			//pDrawPort->AddTexture(nX+.Left, nY+.Top, nX+.Right, nY+.Bottom,
			//	.U0, .V0, .U1, .V1, 0xFFFFFFFF);

 			pDrawPort->InitTextureData(m_ptdBaseTexture);
 
 
 			m_ebSendMailReciver.Render();
 			m_ebSendMailTitle.Render();
  			pDrawPort->FlushRenderingQueue();
 			pDrawPort->EndTextEx();
 
 			m_mebSendMailText.Render();

		}
	    break;
	default:
	    break;
	}
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();
	m_UIOpenMail.Render();
#endif //POSTSOCKETSYSTEM

}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIMail::MouseMessage( MSG *pMsg )
{
#if defined (POSTSOCKETSYSTEM)
	WMSG_RESULT	wmsgResult;
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Title bar
	static BOOL	bTitleBarClick = FALSE;

	// Item clicked
	static BOOL	bLButtonDownInMEB = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	CMailDataList& mailList = CMail::getInstance()->GetMailDataList();
	
	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;
				
				Move( ndX, ndY );
				
				return WMSG_SUCCESS;
			}
			if( IsInside( nX, nY ) ){
				SetFocus(TRUE);
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();
				
				if(bLButtonDownInMEB == TRUE && m_mebSendMailText.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					m_mebSendMailText.SetFocus(TRUE);
					m_ebSendMailReciver.SetFocus(FALSE);
					m_ebSendMailTitle.SetFocus(FALSE);
					return WMSG_SUCCESS;
				}
				for(int i = 0; i < ITEMBUTTONMAX; ++i)
				{
					if( !m_btnSendMailItem[i].IsEmpty() )
					{
						if(m_btnSendMailItem[i].MouseMessage( pMsg) != WMSG_FAIL)
						{
							return WMSG_SUCCESS;
						}
					}
				}
			}
			// Move board
			
			if(m_UIOpenMail.IsInside(nX,nY) && m_UIOpenMail.IsVisible())
			{
				if(m_UIOpenMail.MouseMessage(pMsg)!= WMSG_FAIL)
				{
					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				SetFocus ( TRUE );
			
				nOldX = nX;		nOldY = nY;

				// Title bar
				if( IsInsideRect( nX, nY, m_rcMainTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				else if(IsInsideRect(nX,nY,m_rcListTab))
				{
					m_TabStatus = TABSTATUS::LIST;
				}
// [12/5/2012 박훈] 빌라 요청으로 인한 우편시스템 보내기 버튼 삭제(추후 우편NPC 제거 예정)
#if !(defined G_BRAZIL) && !(defined G_MAXICO)
				else if(IsInsideRect(nX,nY,m_rcSendTab))
				{
					m_TabStatus = TABSTATUS::SEND;
				}
#endif
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					int a  = 0;
				}
				else if(m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL)
				{
					
				}

				if(m_TabStatus == TABSTATUS::LIST)
				{
					if( m_btnFirst.MouseMessage( pMsg ) != WMSG_FAIL )
					{

					}
					else if(m_btnLeft.MouseMessage( pMsg ) != WMSG_FAIL)
					{

					}
					else if(m_btnRight.MouseMessage( pMsg ) != WMSG_FAIL)
					{

					}
					else if(m_btnLast.MouseMessage( pMsg ) != WMSG_FAIL)
					{

					}
					else if(m_btnConfirm.MouseMessage( pMsg ) != WMSG_FAIL)
					{
						
					}

					for(int i = 0; i < PAGEMAILMAX; ++i)
					{
						if (IsInsideRect( nX, nY, m_rcListMailIcon[i]) || IsInsideRect( nX, nY, m_rcListMailInfo[i]) )
						{
							m_SelectedIndex = i;
						}
					}
					
				}
				else if(m_TabStatus == TABSTATUS::SEND)
				{
					if( m_ebSendMailReciver.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						m_mebSendMailText.SetFocus(FALSE);
						m_ebSendMailReciver.SetFocus(TRUE);
						m_ebSendMailReciver.SetFocus(TRUE);
						m_ebSendMailTitle.SetFocus(FALSE);
					}
					else if(m_ebSendMailTitle.MouseMessage( pMsg ) != WMSG_FAIL)
					{
						m_mebSendMailText.SetFocus(FALSE);
						m_ebSendMailReciver.SetFocus(FALSE);
						m_ebSendMailTitle.SetFocus(TRUE);
					}
					else if(m_btnMoney.MouseMessage( pMsg ) != WMSG_FAIL)
					{
					}
					else if(m_mebSendMailText.MouseMessage( pMsg ) != WMSG_FAIL)
					{
						bLButtonDownInMEB = TRUE;
						m_mebSendMailText.SetFocus(TRUE);
						m_ebSendMailReciver.SetFocus(FALSE);
						m_ebSendMailTitle.SetFocus(FALSE);
					}
					else if(m_btnConfirm.MouseMessage( pMsg ) != WMSG_FAIL)
					{

					}
					else if(IsInsideRect(nX, nY,m_rcSendMailCheckClaim))
					{
						m_bCheckClaim = TRUE;
						initMoney();
						SendMailItemBtnInit();
					}
					else if(IsInsideRect(nX, nY,m_rcSendMailCheckSend))
					{
						m_bCheckClaim = FALSE;
						initMoney();
						SendMailItemBtnInit();
					}
				}
				pUIManager->RearrangeOrder( UI_MAIL, TRUE );
				return WMSG_SUCCESS;
			}
			if(m_UIOpenMail.IsInside(nX,nY) && m_UIOpenMail.IsVisible())
			{
				if(m_UIOpenMail.MouseMessage(pMsg)!= WMSG_FAIL)
				{
					return WMSG_SUCCESS;
				}
			}
			bLButtonDownInMEB = TRUE;
		}
		break;

	case WM_LBUTTONUP:
		{
			bTitleBarClick = FALSE;
			bLButtonDownInMEB = FALSE;

			if( IsInside( nX, nY ) )
			{
				if( pUIManager->GetHoldBtn().IsEmpty() )
				{
					// Title bar
					bTitleBarClick = FALSE;
					
					if( !IsFocused() )
						return WMSG_FAIL;
					if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							CloseMailList();
						return WMSG_SUCCESS;
					}
					else if(( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL)
					{
						if( wmsgResult == WMSG_COMMAND )
							CloseMailList();
						return WMSG_SUCCESS;
					}
					if(m_TabStatus == TABSTATUS::LIST)
					{
						if(( wmsgResult = m_btnFirst.MouseMessage( pMsg ) ) != WMSG_FAIL)
						{
							m_Page = 0;
							m_SelectedIndex = -1;
							return WMSG_SUCCESS;
						}
						else if(( wmsgResult = m_btnLeft.MouseMessage( pMsg ) ) != WMSG_FAIL)
						{
							if(m_Page > 0)
								--m_Page;
							m_SelectedIndex = -1;
							return WMSG_SUCCESS;
						}
						else if(( wmsgResult = m_btnRight.MouseMessage( pMsg ) ) != WMSG_FAIL)
						{
							if(mailList.empty())
								return WMSG_SUCCESS;
							int tailNum = mailList.size()%PAGEMAILMAX;
							int bodyNum = mailList.size()/PAGEMAILMAX;
							if(bodyNum > MAXPAGE)
								bodyNum = MAXPAGE;
							if(tailNum==0)
							{
								if(m_Page < bodyNum-1)
									++m_Page;
							}
							else
							{
								if(m_Page < bodyNum)
									++m_Page;
							}
							m_SelectedIndex = -1;
							return WMSG_SUCCESS;
						}
						else if(( wmsgResult = m_btnLast.MouseMessage( pMsg ) ) != WMSG_FAIL)
						{
							if(mailList.empty())
								return WMSG_SUCCESS;
							int tailNum = mailList.size()%PAGEMAILMAX;
							int bodyNum = mailList.size()/PAGEMAILMAX;
							if(bodyNum > MAXPAGE)
								bodyNum = MAXPAGE;
							if(tailNum==0)
							{
								m_Page = bodyNum -1;
							}
							else
							{
								m_Page = bodyNum;
							}
							m_SelectedIndex = -1;
							return WMSG_SUCCESS;
						}
						else if(( wmsgResult = m_btnConfirm.MouseMessage( pMsg ) ) != WMSG_FAIL)
						{
							if(m_SelectedIndex > 0 && wmsgResult == WMSG_COMMAND)
							{
								INDEX serverIndex = CMail::getInstance()->GetMailElementNCount(m_Page * PAGEMAILMAX + m_SelectedIndex).serverIndex;
								PostSystem::ReadtoMailRequest(serverIndex);
							}
							return WMSG_SUCCESS;
						}
						
					}
					else if(m_TabStatus == TABSTATUS::SEND)
					{
						if( m_ebSendMailReciver.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							
						}
						else if(m_ebSendMailTitle.MouseMessage( pMsg ) != WMSG_FAIL)
						{
							
						}
						else if(m_btnMoney.MouseMessage( pMsg ) != WMSG_FAIL)
						{
							if(!pUIManager->DoesMessageBoxExist(MSGCMD_SENDMAIL_MONEY_INPUT))
							{
								CUIMsgBox_Info MsgBoxInfo;
								MsgBoxInfo.SetMsgBoxInfo( _S(1762, "나스"), UMBS_OK| UMBS_INPUTBOX,
									UI_MAIL, MSGCMD_SENDMAIL_MONEY_INPUT, 250);
								MsgBoxInfo.AddString( _S(5663, "나스를 입력해 주십시오.") );
								MsgBoxInfo.SetInputBox( 2, 15, 40, 130 );
								MsgBoxInfo.m_nInputPosY += 4;
								pUIManager->CreateMessageBox( MsgBoxInfo );
							}
						}
						else if(m_mebSendMailText.MouseMessage( pMsg ) != WMSG_FAIL)
						{
							
						}
						else if(( wmsgResult = m_btnConfirm.MouseMessage( pMsg ) ) != WMSG_FAIL)
						{
							if(wmsgResult == WMSG_COMMAND)
							{
								//메일 보내기 조건 체크 함수 만들자...
								if(CheckSendMail() || (m_bCheckClaim && CheckClaim()))
								{
									if(CheckDeletItem() == FALSE)
										return WMSG_SUCCESS;
									CTString reciver(m_ebSendMailReciver.GetString());
									CTString title(m_ebSendMailTitle.GetString());
									CTString text(m_mebSendMailText.GetString());
									SWORD billReqFlag = 0;
									SWORD type = 0;
//청구가 온되면 타입이 바뀐다.
// 									if (m_bCheckClaim)
// 									{
// 										type = 1;
// 									}
									INDEX iconIndex = -1;
									for(int i = 0; i < ITEMBUTTONMAX; ++i)
									{
										if(m_btnSendMailItem[i].GetItemIndex() > 0)
										{
											iconIndex = m_btnSendMailItem[i].GetItemIndex();
											break;
										}
									}
									PostSystem::SendMailMessage(reciver,
																title,
																text,
																m_Money,
																billReqFlag,
																type,
																iconIndex,
																m_ItemCount,
																m_NpcUID);
									SetSending(FALSE);
								}
								
								int a = 0;
							}
						}
						return WMSG_SUCCESS;
						
						
					}
				}
				else //if( pUIManager->GetHoldBtn().IsEmpty() )
				{
					if( IsInside( nX, nY ) )
					{
						// If holding button is item and is from exchange
						if( pUIManager->GetHoldBtn().GetBtnType() == UBET_ITEM &&
							pUIManager->GetHoldBtn().GetWhichUI() == UI_INVENTORY)
						{

							//IsInsideRect 체크는 루프이용해서 i로 돌림
							for(int i = 0; i < ITEMBUTTONMAX; ++i)
							{
								if( IsInsideRect( nX, nY, m_rcSendMailItemEmpty[i]) && CheckSendMailItemAdd())
								{
									CItemData& ItemData = _pNetwork->GetItemData(pUIManager->GetHoldBtn().GetIndex());
									if (m_btnSendMailItem[i].GetItemUniIndex() == pUIManager->GetHoldBtn().GetItemUniIndex())
									{
										if( ( ItemData.GetFlag() & ITEM_FLAG_COUNT ) && pUIManager->GetHoldBtn().GetItemCount() > 1 )
										{
											if(!pUIManager->DoesMessageBoxExist(MSGCMD_SENDMAIL_COUNT_ADD))
											{
												// Ask quantity
												CTString		strMessage;
												CUIMsgBox_Info	MsgBoxInfo;
												MsgBoxInfo.SetMsgBoxInfo( _S( 4288, "아이템" ), UMBS_OK | UMBS_INPUTBOX,
													UI_MAIL, MSGCMD_SENDMAIL_COUNT_ADD );
												const char	*szItemName = _pNetwork->GetItemName(pUIManager->GetHoldBtn().GetItemIndex());
												strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );
												MsgBoxInfo.AddString( strMessage );
												CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
												m_SendMailItemOnSlot = i;
											}
										}
									}
									else
									{
										if(m_btnSendMailItem[i].IsEmpty())
											++m_ItemCount;
										m_btnSendMailItem[i].CopyItemInfo(pUIManager->GetHoldBtn());
										//SENDMAILITEMLIST

										ReceiveItemData& mailItemData = CMail::getInstance()->GetSendItemData(i);
										mailItemData.UniqueIndex = pUIManager->GetHoldBtn().GetItemUniIndex();
										mailItemData.itemCount = pUIManager->GetHoldBtn().GetItemCount();

										if( ( ItemData.GetFlag() & ITEM_FLAG_COUNT ) && pUIManager->GetHoldBtn().GetItemCount() > 1 )
										{
											if(!pUIManager->DoesMessageBoxExist(MSGCMD_SENDMAIL_COUNT_INPUT))
											{
												// Ask quantity
												CTString		strMessage;
												CUIMsgBox_Info	MsgBoxInfo;
												MsgBoxInfo.SetMsgBoxInfo( _S( 4288, "아이템" ), UMBS_OK | UMBS_INPUTBOX,
													UI_MAIL, MSGCMD_SENDMAIL_COUNT_INPUT );
												const char	*szItemName = _pNetwork->GetItemName(pUIManager->GetHoldBtn().GetItemIndex());
												strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );
												MsgBoxInfo.AddString( strMessage );
												CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
												m_SendMailItemOnSlot = i;
											}
										}
									}
									if(m_bCheckClaim)
									{
										initMoney();
										SetMoneyOver(FALSE);
									}
								}
								else
								{
									_pNetwork->ClientSystemMessage(_S( 5662, "해당 아이템은 우편으로 보낼 수 없습니다."), SYSMSG_ERROR);
								}
							}
						}
						// Reset holding button
						pUIManager->ResetHoldBtn();
						return WMSG_SUCCESS;
					}
				}
			}
			if(m_UIOpenMail.IsInside(nX,nY) && m_UIOpenMail.IsVisible())
			{
				if(m_UIOpenMail.MouseMessage(pMsg)!= WMSG_FAIL)
				{
					return WMSG_SUCCESS;
				}
			}
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) ){
				SetFocus(TRUE);
				if(m_TabStatus == TABSTATUS::LIST)
				{
					for(int i = 0; i < PAGEMAILMAX; ++i)
					{
						if (IsInsideRect( nX, nY, m_rcListMailIcon[i]) || IsInsideRect( nX, nY, m_rcListMailInfo[i]) )
						{
							m_SelectedIndex = i;
							INDEX SelectCount = m_Page * PAGEMAILMAX + m_SelectedIndex;

							if (mailList.size() > SelectCount)
							{
								INDEX serverIndex = CMail::getInstance()->GetMailElementNCount(m_Page * PAGEMAILMAX + m_SelectedIndex).serverIndex;
								PostSystem::ReadtoMailRequest(serverIndex);
								break;
							}
						}
					}
				}
				else if(m_TabStatus == TABSTATUS::SEND)
				{
					for(int i = 0; i < ITEMBUTTONMAX; ++i)
					{
						if (IsInsideRect(nX,nY, m_rcSendMailItemEmpty[i]))
						{
#if !(defined G_MAXICO) && !(defined G_BRAZIL)	
							SendMailItemBtnInitIndex(i);
#endif
						}
					}
				}


				return WMSG_SUCCESS;
			}
			else if(m_UIOpenMail.IsInside(nX,nY) && m_UIOpenMail.IsVisible())
			{
				if(m_UIOpenMail.MouseMessage(pMsg)!= WMSG_FAIL)
				{
					return WMSG_SUCCESS;
				}
			}
			else
				SetFocus(FALSE);

		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				return WMSG_SUCCESS;
				// Top Scroll bar
// 				if( m_sbTopScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
// 				{
// 					m_nCurRow = m_sbTopScrollBar.GetScrollPos();
// 					return WMSG_SUCCESS;
// 				}				
			}
		}
		break;
	}
#endif //(POSTSOCKETSYSTEM)
	return WMSG_FAIL;
}
WMSG_RESULT	CUIMail::KeyMessage(MSG *pMsg )
{
	if( !IsFocused() ) return WMSG_FAIL;
	
	WMSG_RESULT	wmsgResult;

	if( pMsg->wParam == VK_TAB )
	{
		if(m_ebSendMailReciver.IsFocused())
		{
			m_ebSendMailReciver.SetFocus(FALSE);
			m_ebSendMailTitle.SetFocus(TRUE);
			m_mebSendMailText.SetFocus(FALSE);
		}
		else if(m_ebSendMailTitle.IsFocused())
		{
			m_ebSendMailTitle.SetFocus(FALSE);
			m_mebSendMailText.SetFocus(TRUE);
			m_ebSendMailReciver.SetFocus(FALSE);
		}	
		else if(m_mebSendMailText.IsFocused())
		{
			m_mebSendMailText.SetFocus(FALSE);
			m_ebSendMailReciver.SetFocus(TRUE);
			m_ebSendMailTitle.SetFocus(FALSE);
		}
		return WMSG_SUCCESS;
	}
	else if(pMsg->wParam == VK_RETURN)
	{
		if(m_ebSendMailReciver.IsFocused())
		{
			m_ebSendMailReciver.SetFocus(FALSE);
			m_ebSendMailTitle.SetFocus(TRUE);
			m_mebSendMailText.SetFocus(FALSE);
			return WMSG_SUCCESS;
		}
		else if(m_ebSendMailTitle.IsFocused())
		{
			m_ebSendMailTitle.SetFocus(FALSE);
			m_mebSendMailText.SetFocus(TRUE);
			m_ebSendMailReciver.SetFocus(FALSE);
			return WMSG_SUCCESS;
		}
	}
	if(m_ebSendMailReciver.IsFocused())
	{
		if( m_ebSendMailReciver.KeyMessage( pMsg ) != WMSG_FAIL )
			return WMSG_SUCCESS; 
	}
	else if(m_ebSendMailTitle.IsFocused())
	{
		if(pMsg->wParam == VK_OEM_7)
			return WMSG_SUCCESS; 
		if( m_ebSendMailTitle.KeyMessage( pMsg ) != WMSG_FAIL )
			return WMSG_SUCCESS; 
	}
	else if(m_mebSendMailText.IsFocused())
	{
		if(pMsg->wParam == VK_OEM_7)
			return WMSG_SUCCESS; 
		if(m_mebSendMailText.GetAllStringLength()+2 <= 255)
		{
			if( ( wmsgResult = m_mebSendMailText.KeyMessage( pMsg ) ) != WMSG_FAIL )
				return WMSG_SUCCESS; 
		}
		else if (pMsg->wParam == VK_BACK)
		{
			if( ( m_mebSendMailText.KeyMessage( pMsg ) ) != WMSG_FAIL )
			{
				return WMSG_SUCCESS; 
			}
		}

	}
	return WMSG_FAIL;
}

WMSG_RESULT CUIMail::CharMessage(MSG *pMsg )
{
	if( !IsFocused() ) return WMSG_FAIL;
	

	if(m_ebSendMailReciver.IsFocused())
	{
		if( m_ebSendMailReciver.CharMessage( pMsg ) != WMSG_FAIL )
			return WMSG_SUCCESS; 
	}
	else if(m_ebSendMailTitle.IsFocused())
	{
		if(pMsg->wParam == 39)
			return WMSG_SUCCESS; 

		if( m_ebSendMailTitle.CharMessage( pMsg ) != WMSG_FAIL )
			return WMSG_SUCCESS; 
		
	}
	else if(m_mebSendMailText.IsFocused())
	{
		if(pMsg->wParam == 39)
		{
			return WMSG_SUCCESS; 
		}
		if(m_mebSendMailText.GetAllStringLength()+1 <= 255)
		{
			if( m_mebSendMailText.CharMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS; 
		}
		return WMSG_SUCCESS; 
	}
	return WMSG_FAIL;

}
WMSG_RESULT	CUIMail::IMEMessage(MSG *pMsg )
{
	if( !IsFocused() ) return WMSG_FAIL;
	

	if(m_ebSendMailReciver.IsFocused())
	{
		if( m_ebSendMailReciver.IMEMessage( pMsg ) != WMSG_FAIL )
			return WMSG_SUCCESS; 
	}
	else if(m_ebSendMailTitle.IsFocused())
	{
		if( m_ebSendMailTitle.IMEMessage( pMsg ) != WMSG_FAIL )
			return WMSG_SUCCESS; 
	}
	else if(m_mebSendMailText.IsFocused())
	{
		if( m_mebSendMailText.GetAllStringLength()+2 <= 255)
		{
			if( m_mebSendMailText.IMEMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS; 
		}
	}
	return WMSG_FAIL;
}




//메일 보기 UI

CUIMail::CUIOpenMail::CUIOpenMail()
{
	SetVisible(FALSE);
	SetFocus(FALSE);
}

// ----------------------------------------------------------------------------
// Name : ~CUIWareHouse()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIMail::CUIOpenMail::~CUIOpenMail()
{
	CloseMail();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIMail::CUIOpenMail::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
#if defined (POSTSOCKETSYSTEM)
	m_pParent = (CUIBase*)pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	m_ptdBaseTexture = CreateTexture(CTString("Data\\Interface\\postNcraft.tex"));
	FLOAT fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_rcMainTitle.SetRect( 0, 0, 425, 22 );

	m_rcOpenMailBackground.SetRect(7, 27, 387, 292);
	m_rcOpenMailItemBackground.SetRect(7, 318, 387, 401);

	m_rcOpenMailReciver.SetRect(124, 36, 382,53);
	m_rcOpenMailTitle.SetRect(124,57,382,74);
	m_rcOpenMailText.SetRect(15, 78, 382, 281);
	
 	for(int i = 0; i < ITEMBUTTONMAX; ++i)
 	{
		int plusWidth = i/ITEMBUTTONLINEMAX*39;
		int plusHeight = i%ITEMBUTTONLINEMAX*71;
		//m_rcOpenMailItemEmpty[i].SetRect(15+plusWidth, 323+plusHeight, 49+plusWidth, 357+plusHeight);
		//m_btnOpenMailItem[i].Create(this, 15+plusWidth, 323+plusHeight, 34, 34,UI_MAIL,UBET_ITEM);
		m_rcOpenMailItemEmpty[i].SetRect(OPENBACKGROUNDWIDTH/2-17, 342, OPENBACKGROUNDWIDTH/2+17, 376);
		m_btnOpenMailItem[i].Create(this, m_rcOpenMailItemEmpty[i].Left, m_rcOpenMailItemEmpty[i].Top,
			m_rcOpenMailItemEmpty[i].GetWidth(), m_rcOpenMailItemEmpty[i].GetHeight(),UI_MAIL,UBET_ITEM);
 	}

	m_rcOpenMailMoneyIcon.SetRect(7, 405, 25, 423);
	m_rcOpenMailMoney.SetRect(31, 405, 207, 422);
	//메인 배경
	m_rtMainBackgroundUL.SetUV(0, 120, 26, 120 + 22,fTexWidth,fTexHeight);
	m_rtMainBackgroundUM.SetUV(26, 120, OPENBACKGROUNDWIDTHTEX - 26, 120 + 22,fTexWidth,fTexHeight);
	m_rtMainBackgroundUR.SetUV(OPENBACKGROUNDWIDTHTEX - 26, 120, OPENBACKGROUNDWIDTHTEX, 120 + 22,fTexWidth,fTexHeight);
	
	m_rtMainBackgroundML.SetUV(0, 120 + 22, 26, 120 + OPENBACKGROUNDHEIGHTTEX - 8,fTexWidth,fTexHeight);
	m_rtMainBackgroundMM.SetUV(26, 120 + 22, OPENBACKGROUNDWIDTHTEX - 26, 120 + OPENBACKGROUNDHEIGHTTEX - 8,fTexWidth,fTexHeight);
	m_rtMainBackgroundMR.SetUV(OPENBACKGROUNDWIDTHTEX - 26, 120 + 22, OPENBACKGROUNDWIDTHTEX, 120 + OPENBACKGROUNDHEIGHTTEX - 8,fTexWidth,fTexHeight);
	
	m_rtMainBackgroundLL.SetUV(0, 120 + OPENBACKGROUNDHEIGHTTEX - 8, 26, 120 + OPENBACKGROUNDHEIGHTTEX,fTexWidth,fTexHeight);
	m_rtMainBackgroundLM.SetUV(26, 120 + OPENBACKGROUNDHEIGHTTEX - 8, OPENBACKGROUNDWIDTHTEX - 26, 120 + OPENBACKGROUNDHEIGHTTEX,fTexWidth,fTexHeight);
	m_rtMainBackgroundLR.SetUV(OPENBACKGROUNDWIDTHTEX - 26, 120 + OPENBACKGROUNDHEIGHTTEX - 8, OPENBACKGROUNDWIDTHTEX, 120 + OPENBACKGROUNDHEIGHTTEX,fTexWidth,fTexHeight);
	//갈색
	m_rtBrownBackgroundU.SetUV(412, 0, 512, 1, fTexWidth, fTexHeight);
	
	m_rtBrownBackgroundML.SetUV(412, 0, 414, 52, fTexWidth, fTexHeight);
	m_rtBrownBackgroundMM.SetUV(415, 30, 510, 51, fTexWidth, fTexHeight);
	m_rtBrownBackgroundMR.SetUV(510, 0, 512, 52, fTexWidth, fTexHeight);
	
	m_rtBrownBackgroundL.SetUV(412, 52, 512, 53, fTexWidth, fTexHeight);

	//녹색
	m_rtGreenBackguoundU.SetUV(412,56,512,57, fTexWidth,fTexHeight);
	
	m_rtGreenBackguoundML.SetUV(412, 57, 413, 108, fTexWidth,fTexHeight);
	m_rtGreenBackguoundMM.SetUV(413, 57, 511, 108, fTexWidth,fTexHeight);
	m_rtGreenBackguoundMR.SetUV(511, 57, 512, 108, fTexWidth,fTexHeight);
	
	m_rtGreenBackguoundL.SetUV(412, 108, 512, 109, fTexWidth,fTexHeight);

	m_rtBrownGradationRectL.SetUV(329, 121, 330, 138 ,fTexWidth, fTexHeight);
	m_rtBrownGradationRectM.SetUV(330, 121, 414, 138 ,fTexWidth, fTexHeight);
	m_rtBrownGradationRectR.SetUV(414, 121, 415, 138 ,fTexWidth, fTexHeight);

	m_rtBrownRectL.SetUV(328, 155, 329, 190, fTexWidth, fTexHeight);
	m_rtBrownRectM.SetUV(330, 155, 413, 190, fTexWidth, fTexHeight);
	m_rtBrownRectR.SetUV(414, 155, 415, 190, fTexWidth, fTexHeight);

	m_rtItemCapIcon.SetUV(292, 156, 326, 190, fTexWidth, fTexHeight);
	m_rtEmptyItemIcon.SetUV( 292, 120, 326, 154, fTexWidth, fTexHeight );

	m_btnMoney.Create(this, _s(""), 7, 405, 18, 18);
	m_btnMoney.SetUV(UBS_IDLE, 420, 170, 438, 188, fTexWidth, fTexHeight);
	m_btnMoney.SetUV(UBS_CLICK, 420, 170, 438, 188, fTexWidth, fTexHeight);
	m_btnMoney.CopyUV(UBS_IDLE, UBS_DISABLE);
	m_btnMoney.CopyUV(UBS_IDLE, UBS_ON);

	m_btnClose.Create(this, _S(1417, "확인"), 265, 403, 119, 21);
	m_btnClose.SetUV(UBS_IDLE, 420, 120, 502, 140, fTexWidth, fTexHeight);
	m_btnClose.SetUV(UBS_CLICK, 420, 144, 502, 164, fTexWidth, fTexHeight);
	m_btnClose.CopyUV(UBS_IDLE, UBS_DISABLE);
	m_btnClose.CopyUV(UBS_IDLE, UBS_ON);

	m_btnDelete.Create(this, _S(338, "삭제"), 265, 295, 119, 21);
	m_btnDelete.SetUV(UBS_IDLE, 420, 120, 502, 140, fTexWidth, fTexHeight);
	m_btnDelete.SetUV(UBS_CLICK, 420, 144, 502, 164, fTexWidth, fTexHeight);
	m_btnDelete.CopyUV(UBS_IDLE, UBS_DISABLE);
	m_btnDelete.CopyUV(UBS_IDLE, UBS_ON);

	m_btnRetun.Create(this, _S(5645, "반송"), 7, 295, 119, 21);
	m_btnRetun.SetUV(UBS_IDLE, 420, 120, 502, 140, fTexWidth, fTexHeight);
	m_btnRetun.SetUV(UBS_CLICK, 420, 144, 502, 164, fTexWidth, fTexHeight);
	m_btnRetun.CopyUV(UBS_IDLE, UBS_DISABLE);
	m_btnRetun.CopyUV(UBS_IDLE, UBS_ON);
	//일단 늘 비활성화 반송기능 개발이후에 작동하자
	m_btnRetun.SetEnable(FALSE);

	m_btnReply.Create(this, _S(5674, "답신"), 136, 295, 119, 21);
	m_btnReply.SetUV(UBS_IDLE, 420, 120, 502, 140, fTexWidth, fTexHeight);
	m_btnReply.SetUV(UBS_CLICK, 420, 144, 502, 164, fTexWidth, fTexHeight);
	m_btnReply.CopyUV(UBS_IDLE, UBS_DISABLE);
	m_btnReply.CopyUV(UBS_IDLE, UBS_ON);


	m_mebOpenMailText.Create(this, m_rcOpenMailText.Left, m_rcOpenMailText.Top, m_rcOpenMailText.GetWidth(), m_rcOpenMailText.GetHeight(), 122);
	m_mebOpenMailText.SetEnableInput(TRUE);
	m_mebOpenMailText.SetEnable(TRUE);
	m_mebOpenMailText.ResetString();
#endif //(POSTSOCKETSYSTEM)
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIMail::CUIOpenMail::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI ) / 2 - GetWidth(), ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIMail::CUIOpenMail::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}


// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIMail::CUIOpenMail::Render()
{
#ifdef POSTSOCKETSYSTEM
	if( IsVisible() == FALSE )
		return;
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();
	
	int	nX, nY, i;
	nX = GetAbsPosX();
	nY = GetAbsPosY();

	pDrawPort->InitTextureData(m_ptdBaseTexture);


	pDrawPort->AddTexture(nX,nY,nX+26,nY+22,
		m_rtMainBackgroundUL.U0,m_rtMainBackgroundUL.V0,m_rtMainBackgroundUL.U1,m_rtMainBackgroundUL.V1,0xFFFFFFFF);
	pDrawPort->AddTexture(nX+26,nY,nX+OPENBACKGROUNDWIDTH-26,nY+22,
		m_rtMainBackgroundUM.U0,m_rtMainBackgroundUM.V0,m_rtMainBackgroundUM.U1,m_rtMainBackgroundUM.V1,0xFFFFFFFF);
	pDrawPort->AddTexture(nX+OPENBACKGROUNDWIDTH-26,nY,nX+OPENBACKGROUNDWIDTH,nY+22,
		m_rtMainBackgroundUR.U0,m_rtMainBackgroundUR.V0,m_rtMainBackgroundUR.U1,m_rtMainBackgroundUR.V1,0xFFFFFFFF);
	
	pDrawPort->AddTexture(nX,nY+22,nX+26,nY+OPENBACKGROUNDHEIGHT-8,
		m_rtMainBackgroundML.U0,m_rtMainBackgroundML.V0,m_rtMainBackgroundML.U1,m_rtMainBackgroundML.V1,0xFFFFFFFF);
	pDrawPort->AddTexture(nX+26,nY+22,nX+OPENBACKGROUNDWIDTH-26,nY+OPENBACKGROUNDHEIGHT-8,
		m_rtMainBackgroundMM.U0,m_rtMainBackgroundMM.V0,m_rtMainBackgroundMM.U1,m_rtMainBackgroundMM.V1,0xFFFFFFFF);
	pDrawPort->AddTexture(nX+OPENBACKGROUNDWIDTH-26,nY+22,nX+OPENBACKGROUNDWIDTH,nY+OPENBACKGROUNDHEIGHT-8,
		m_rtMainBackgroundMR.U0,m_rtMainBackgroundMR.V0,m_rtMainBackgroundMR.U1,m_rtMainBackgroundMR.V1,0xFFFFFFFF);
	
	pDrawPort->AddTexture(nX,nY+OPENBACKGROUNDHEIGHT-8,nX+26,nY+OPENBACKGROUNDHEIGHT,
		m_rtMainBackgroundLL.U0,m_rtMainBackgroundLL.V0,m_rtMainBackgroundLL.U1,m_rtMainBackgroundLL.V1,0xFFFFFFFF);
	pDrawPort->AddTexture(nX+26,nY+OPENBACKGROUNDHEIGHT-8,nX+OPENBACKGROUNDWIDTH-26,nY+OPENBACKGROUNDHEIGHT,
		m_rtMainBackgroundLM.U0,m_rtMainBackgroundLM.V0,m_rtMainBackgroundLM.U1,m_rtMainBackgroundLM.V1,0xFFFFFFFF);
	pDrawPort->AddTexture(nX+OPENBACKGROUNDWIDTH-26,nY+OPENBACKGROUNDHEIGHT-8,nX+OPENBACKGROUNDWIDTH,nY+OPENBACKGROUNDHEIGHT,
		m_rtMainBackgroundLR.U0,m_rtMainBackgroundLR.V0,m_rtMainBackgroundLR.U1,m_rtMainBackgroundLR.V1,0xFFFFFFFF);
	//상부 백그라운드 (편지내용)			
	pDrawPort->AddTexture(nX + m_rcOpenMailBackground.Left, nY + m_rcOpenMailBackground.Top, nX + m_rcOpenMailBackground.Right, nY+m_rcOpenMailBackground.Top+1,
		m_rtBrownBackgroundU.U0, m_rtBrownBackgroundU.V0, m_rtBrownBackgroundU.U1, m_rtBrownBackgroundU.V1, 0xFFFFFFFF);
	
	pDrawPort->AddTexture(nX + m_rcOpenMailBackground.Left, nY + m_rcOpenMailBackground.Top, nX + m_rcOpenMailBackground.Left+1, nY+m_rcOpenMailBackground.Bottom,
		m_rtBrownBackgroundML.U0, m_rtBrownBackgroundML.V0, m_rtBrownBackgroundML.U1, m_rtBrownBackgroundML.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX + m_rcOpenMailBackground.Left + 1, nY + m_rcOpenMailBackground.Top + 1, nX + m_rcOpenMailBackground.Right -1, nY+m_rcOpenMailBackground.Bottom-1,
		m_rtBrownBackgroundMM.U0, m_rtBrownBackgroundMM.V0, m_rtBrownBackgroundMM.U1, m_rtBrownBackgroundMM.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX + m_rcOpenMailBackground.Right-1, nY + m_rcOpenMailBackground.Top, nX + m_rcOpenMailBackground.Right, nY+m_rcOpenMailBackground.Bottom,
		m_rtBrownBackgroundMR.U0, m_rtBrownBackgroundMR.V0, m_rtBrownBackgroundMR.U1, m_rtBrownBackgroundMR.V1, 0xFFFFFFFF);
	
	pDrawPort->AddTexture(nX + m_rcOpenMailBackground.Left, nY + m_rcOpenMailBackground.Bottom-1, nX + m_rcOpenMailBackground.Right, nY+m_rcOpenMailBackground.Bottom,
		m_rtBrownBackgroundL.U0, m_rtBrownBackgroundL.V0, m_rtBrownBackgroundL.U1, m_rtBrownBackgroundL.V1, 0xFFFFFFFF);
	//하부 백그라운드(첨부아이템)
	pDrawPort->AddTexture(nX + m_rcOpenMailItemBackground.Left, nY + m_rcOpenMailItemBackground.Top, nX + m_rcOpenMailItemBackground.Right, nY+m_rcOpenMailItemBackground.Top+1,
		m_rtBrownBackgroundU.U0, m_rtBrownBackgroundU.V0, m_rtBrownBackgroundU.U1, m_rtBrownBackgroundU.V1, 0xFFFFFFFF);
	
	pDrawPort->AddTexture(nX + m_rcOpenMailItemBackground.Left, nY + m_rcOpenMailItemBackground.Top, nX + m_rcOpenMailItemBackground.Left+1, nY+m_rcOpenMailItemBackground.Bottom,
		m_rtBrownBackgroundML.U0, m_rtBrownBackgroundML.V0, m_rtBrownBackgroundML.U1, m_rtBrownBackgroundML.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX + m_rcOpenMailItemBackground.Left + 1, nY + m_rcOpenMailItemBackground.Top + 1, nX + m_rcOpenMailItemBackground.Right -1, nY+m_rcOpenMailItemBackground.Bottom-1,
		m_rtBrownBackgroundMM.U0, m_rtBrownBackgroundMM.V0, m_rtBrownBackgroundMM.U1, m_rtBrownBackgroundMM.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX + m_rcOpenMailItemBackground.Right-1, nY + m_rcOpenMailItemBackground.Top, nX + m_rcOpenMailItemBackground.Right, nY+m_rcOpenMailItemBackground.Bottom,
		m_rtBrownBackgroundMR.U0, m_rtBrownBackgroundMR.V0, m_rtBrownBackgroundMR.U1, m_rtBrownBackgroundMR.V1, 0xFFFFFFFF);
	
	pDrawPort->AddTexture(nX + m_rcOpenMailItemBackground.Left, nY + m_rcOpenMailItemBackground.Bottom-1, nX + m_rcOpenMailItemBackground.Right, nY+m_rcOpenMailItemBackground.Bottom,
		m_rtBrownBackgroundL.U0, m_rtBrownBackgroundL.V0, m_rtBrownBackgroundL.U1, m_rtBrownBackgroundL.V1, 0xFFFFFFFF);
	//보낸 사람
	pDrawPort->AddTexture(nX+m_rcOpenMailReciver.Left, nY+m_rcOpenMailReciver.Top, nX+m_rcOpenMailReciver.Left+1, nY+m_rcOpenMailReciver.Bottom,
		m_rtBrownGradationRectL.U0, m_rtBrownGradationRectL.V0, m_rtBrownGradationRectL.U1, m_rtBrownGradationRectL.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX+m_rcOpenMailReciver.Left+1, nY+m_rcOpenMailReciver.Top, nX+m_rcOpenMailReciver.Right-1, nY+m_rcOpenMailReciver.Bottom,
		m_rtBrownGradationRectM.U0, m_rtBrownGradationRectM.V0, m_rtBrownGradationRectM.U1, m_rtBrownGradationRectM.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX+m_rcOpenMailReciver.Right-1, nY+m_rcOpenMailReciver.Top, nX+m_rcOpenMailReciver.Right, nY+m_rcOpenMailReciver.Bottom,
		m_rtBrownGradationRectR.U0, m_rtBrownGradationRectR.V0, m_rtBrownGradationRectR.U1, m_rtBrownGradationRectR.V1, 0xFFFFFFFF);
	CTString tempstring = _S( 5655, "보낸 사람" );
	pDrawPort->PutTextExCX( tempstring, nX + m_rcOpenMailReciver.Left -50, nY + m_rcOpenMailReciver.Top + 1);
	tempstring = _S( 341, "제목" );
	pDrawPort->PutTextExCX( tempstring, nX + m_rcOpenMailTitle.Left -50, nY + m_rcOpenMailTitle.Top + 1);
	//제목
	pDrawPort->AddTexture(nX+m_rcOpenMailTitle.Left, nY+m_rcOpenMailTitle.Top, nX+m_rcOpenMailTitle.Left+1, nY+m_rcOpenMailTitle.Bottom,
		m_rtBrownGradationRectL.U0, m_rtBrownGradationRectL.V0, m_rtBrownGradationRectL.U1, m_rtBrownGradationRectL.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX+m_rcOpenMailTitle.Left+1, nY+m_rcOpenMailTitle.Top, nX+m_rcOpenMailTitle.Right-1, nY+m_rcOpenMailTitle.Bottom,
		m_rtBrownGradationRectM.U0, m_rtBrownGradationRectM.V0, m_rtBrownGradationRectM.U1, m_rtBrownGradationRectM.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX+m_rcOpenMailTitle.Right-1, nY+m_rcOpenMailTitle.Top, nX+m_rcOpenMailTitle.Right, nY+m_rcOpenMailTitle.Bottom,
		m_rtBrownGradationRectR.U0, m_rtBrownGradationRectR.V0, m_rtBrownGradationRectR.U1, m_rtBrownGradationRectR.V1, 0xFFFFFFFF);

	//돈
	pDrawPort->AddTexture(nX+m_rcOpenMailMoney.Left, nY+m_rcOpenMailMoney.Top, nX+m_rcOpenMailMoney.Left+1, nY+m_rcOpenMailMoney.Bottom,
		m_rtBrownGradationRectL.U0, m_rtBrownGradationRectL.V0, m_rtBrownGradationRectL.U1, m_rtBrownGradationRectL.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX+m_rcOpenMailMoney.Left+1, nY+m_rcOpenMailMoney.Top, nX+m_rcOpenMailMoney.Right-1, nY+m_rcOpenMailMoney.Bottom,
		m_rtBrownGradationRectM.U0, m_rtBrownGradationRectM.V0, m_rtBrownGradationRectM.U1, m_rtBrownGradationRectM.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX+m_rcOpenMailMoney.Right-1, nY+m_rcOpenMailMoney.Top, nX+m_rcOpenMailMoney.Right, nY+m_rcOpenMailMoney.Bottom,
		m_rtBrownGradationRectR.U0, m_rtBrownGradationRectR.V0, m_rtBrownGradationRectR.U1, m_rtBrownGradationRectR.V1, 0xFFFFFFFF);

	//내용
	pDrawPort->AddTexture(nX + m_rcOpenMailText.Left, nY + m_rcOpenMailText.Top, nX + m_rcOpenMailText.Right, nY+m_rcOpenMailText.Top+1,
		m_rtGreenBackguoundU.U0, m_rtGreenBackguoundU.V0, m_rtGreenBackguoundU.U1, m_rtGreenBackguoundU.V1, 0xFFFFFFFF);
	
	pDrawPort->AddTexture(nX + m_rcOpenMailText.Left, nY + m_rcOpenMailText.Top, nX + m_rcOpenMailText.Left+1, nY+m_rcOpenMailText.Bottom,
		m_rtGreenBackguoundML.U0, m_rtGreenBackguoundML.V0, m_rtGreenBackguoundML.U1, m_rtGreenBackguoundML.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX + m_rcOpenMailText.Left+1, nY + m_rcOpenMailText.Top+1, nX + m_rcOpenMailText.Right -1, nY+m_rcOpenMailText.Bottom-1,
		m_rtGreenBackguoundMM.U0, m_rtGreenBackguoundMM.V0, m_rtGreenBackguoundMM.U1, m_rtGreenBackguoundMM.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX + m_rcOpenMailText.Right-1, nY + m_rcOpenMailText.Top, nX + m_rcOpenMailText.Right, nY+m_rcOpenMailText.Bottom,
		m_rtGreenBackguoundMR.U0, m_rtGreenBackguoundMR.V0, m_rtGreenBackguoundMR.U1, m_rtGreenBackguoundMR.V1, 0xFFFFFFFF);
	
	pDrawPort->AddTexture(nX + m_rcOpenMailText.Left, nY + m_rcOpenMailText.Bottom-1, nX + m_rcOpenMailText.Right, nY+m_rcOpenMailText.Bottom,
		m_rtGreenBackguoundL.U0, m_rtGreenBackguoundL.V0, m_rtGreenBackguoundL.U1, m_rtGreenBackguoundL.V1, 0xFFFFFFFF);


	//플레쉬랜더 큐 구조때문에 한 포문에 돌릴수가 없다.
	for( i = 0; i < ITEMBUTTONMAX; ++i)
	{
		pDrawPort->AddTexture(nX+m_rcOpenMailItemEmpty[i].Left, nY+m_rcOpenMailItemEmpty[i].Top, nX+m_rcOpenMailItemEmpty[i].Right, nY+m_rcOpenMailItemEmpty[i].Bottom,
			m_rtEmptyItemIcon.U0, m_rtEmptyItemIcon.V0, m_rtEmptyItemIcon.U1, m_rtEmptyItemIcon.V1, 0xFFFFFFFF);
	}

	CTString data;
	MAILELEMENT& SelectedMail = CMail::getInstance()->GetMailElement();

	data = SelectedMail.senderName;
	//7자 넘으면 전부 지운다.
	pDrawPort->PutTextEx( data, nX + m_rcOpenMailReciver.Left + 2, nY + m_rcOpenMailReciver.Top );
	data = SelectedMail.title;
	pDrawPort->PutTextEx( data, nX + m_rcOpenMailTitle.Left + 2, nY + m_rcOpenMailTitle.Top );
	data.PrintF( "%I64d", SelectedMail.money);
	CUIManager::getSingleton()->InsertCommaToString( data );
	pDrawPort->PutTextEx( data, nX + m_rcOpenMailMoney.Left + 2, nY + m_rcOpenMailMoney.Top );
	if(SelectedMail.iconIndex == -1)
	{
		m_btnRetun.SetEnable(FALSE);
	}
// [12/5/2012 박훈] 빌라 요청으로 인한 우편시스템 보내기 버튼 삭제(추후 우편NPC 제거 예정)
#if !(defined G_BRAZIL) && !(defined G_MAXICO)
	m_btnRetun.Render();
	m_btnReply.Render();
#endif
	m_btnDelete.Render();
	m_btnClose.Render();
	m_btnMoney.Render();
	
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();

	for( i = 0; i < ITEMBUTTONMAX; ++i)
 	{
		ReceiveItemData& ItemData = CMail::getInstance()->GetRecvItemData(i);

		if (ItemData.itemIndex > 0)
			CopyiteminfoFromMail(ItemData, i);
		else
			m_btnOpenMailItem[i].InitBtn();

		if( !m_btnOpenMailItem[i].IsEmpty() )
		{
			m_btnOpenMailItem[i].Render();
			pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );
			m_btnOpenMailItem[i].RenderInfoPopup();
			pDrawPort->FlushRenderingQueue();
		}
	}
	

	m_mebOpenMailText.Render();
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();
#endif //POSTSOCKETSYSTEM
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIMail::CUIOpenMail::MouseMessage( MSG *pMsg )
{
#if defined (POSTSOCKETSYSTEM)
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Item clicked
	static BOOL	bLButtonDownInItem = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );
	
	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
			{
				// Move board
				SetFocus(TRUE);
				if(m_mebOpenMailText.MouseMessage( pMsg ) != WMSG_FAIL)
				{
							
				}
				for(int i = 0; i < ITEMBUTTONMAX; ++i)
				{
					if( !m_btnOpenMailItem[i].IsEmpty() )
					{
						if(m_btnOpenMailItem[i].MouseMessage( pMsg) != WMSG_FAIL)
						{
							return WMSG_SUCCESS;
						}
					}
				}
				return WMSG_SUCCESS;
			}
			else
				SetFocus(FALSE);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				SetFocus(TRUE);
				if( m_btnRetun.MouseMessage( pMsg ) != WMSG_FAIL)
				{
				}
				else if(m_btnReply.MouseMessage( pMsg ) != WMSG_FAIL)
				{
				}
				else if(m_btnDelete.MouseMessage( pMsg ) != WMSG_FAIL)
				{
				}
				else if(m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL)
				{
				}
				else if(m_btnMoney.MouseMessage(pMsg) != WMSG_FAIL)
				{
				}
				pUIManager->RearrangeOrder( UI_MAIL, TRUE );

				return WMSG_SUCCESS;
			}
			else
				SetFocus(FALSE);
		}
		break;

	case WM_LBUTTONUP:
		{
			if( IsInside( nX, nY ) )
			{
				MAILELEMENT& MailElement = CMail::getInstance()->GetMailElement();
				
				if( m_btnRetun.MouseMessage( pMsg ) != WMSG_FAIL)
				{
				}
// [12/5/2012 박훈] 빌라 요청으로 인한 우편시스템 보내기 버튼 삭제(추후 우편NPC 제거 예정)
#if !(defined G_BRAZIL) && !(defined G_MAXICO)
				else if(m_btnReply.MouseMessage( pMsg ) != WMSG_FAIL)
				{
					pUIManager->GetMail()->ReplySet();
				}
#endif
				else if(m_btnDelete.MouseMessage( pMsg ) != WMSG_FAIL)
				{
					BOOL bItemEmpty = TRUE;
					for(int lol = 0 ;lol < ITEMBUTTONMAX;++lol)
					{
						if (!m_btnOpenMailItem[lol].IsEmpty())
						{
							bItemEmpty = FALSE;
							break;
						}
					}

					if(MailElement.money > 0 || !bItemEmpty)
					{
						if(!pUIManager->DoesMessageBoxExist(MSGCMD_DELETMAIL_CHECK))
						{
							CTString		strMessage;
							CUIMsgBox_Info	MsgBoxInfo;
							MsgBoxInfo.SetMsgBoxInfo( _S( 4102, "경고!" ), UMBS_OKCANCEL,
								UI_MAIL, MSGCMD_DELETMAIL_CHECK);
							strMessage.PrintF( _S( 5658, "아이템이나 돈이 남아있습니다. 정말로 Message를 삭제하시겠습니까?" ));
							MsgBoxInfo.AddString( strMessage );
							CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
						}
						
					}
					else
					{
						if (pUIManager->DoesMessageBoxExist(MSGCMD_DELETMAIL_CHECK))
							pUIManager->CloseMessageBox(MSGCMD_DELETMAIL_CHECK);

						PostSystem::DeletetoMailRequest(MailElement.serverIndex);
					}
				}
				else if(m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL)
				{
					CloseMail();
				}
				else if(m_btnMoney.MouseMessage(pMsg) != WMSG_FAIL)
				{
					PostSystem::GetItemsfromMailRequest(MailElement.serverIndex, 10, 19, MailElement.money, pUIManager->GetMail()->GetNPCUID(), 1);
				}
				
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				SetFocus(TRUE);
				for(int i = 0; i < ITEMBUTTONMAX; ++i)
				{
					if (IsInsideRect(nX,nY, m_rcOpenMailItemEmpty[i]) && !m_btnOpenMailItem[i].IsEmpty() && m_btnOpenMailItem[i].IsEnabled())
					{
						MAILELEMENT& MailElement = CMail::getInstance()->GetMailElement();
						PostSystem::GetItemsfromMailRequest(MailElement.serverIndex,
															i,
															m_btnOpenMailItem[i].GetIndex(),
															m_btnOpenMailItem[i].GetItemCount(),
															pUIManager->GetMail()->GetNPCUID(),
															0);
					}
					m_btnOpenMailItem[i].SetEnable(FALSE);
				}
				return WMSG_SUCCESS;
			}
			else
				SetFocus(FALSE);
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				return WMSG_SUCCESS;
				// Top Scroll bar
// 				if( m_sbTopScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
// 				{
// 					m_nCurRow = m_sbTopScrollBar.GetScrollPos();
// 					return WMSG_SUCCESS;
// 				}				
			}
		}
		break;
	}
#endif //POSTSOCKETSYSTEM
	return WMSG_FAIL;
}

WMSG_RESULT	CUIMail::CUIOpenMail::KeyMessage(MSG *pMsg )
{
	return WMSG_FAIL;
}
WMSG_RESULT CUIMail::CUIOpenMail::CharMessage(MSG *pMsg )
{
	return WMSG_FAIL;
}
WMSG_RESULT	CUIMail::CUIOpenMail::IMEMessage(MSG *pMsg )
{
	return WMSG_FAIL;
}
		
BOOL CUIMail::CUIOpenMail::IsEditBoxFocused()
{
	return FALSE;
}
void CUIMail::CUIOpenMail::KillFocusEditBox()
{

}

void CUIMail::CUIOpenMail::OpenOpenMail()
{
#if defined(POSTSOCKETSYSTEM)
	//해당 함수가 사용 되는 부분은 메일을 선택하고 서버에 선택 메일 오픈 요청에 대한 응답이 오픈하라 로 왔을때 이다.
	//때문에 openmailitr은 Null인 경우 없다.
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();
	m_mebOpenMailText.ResetString();
	MAILELEMENT& MailElement = CMail::getInstance()->GetMailElement();
	
	CTString mainstr =  MailElement.text.str_String;
	char cSeparator = 13;
	CTString ctsSeparator;
	ctsSeparator.PrintF("%c",cSeparator);

 	for(INDEX cutpos = mainstr.FindSubstr(ctsSeparator); cutpos != -1; cutpos = mainstr.FindSubstr(ctsSeparator))
 	{
 		CTString addstr;
 		mainstr.Split(cutpos+2, addstr, mainstr);
 		m_mebOpenMailText.AddString(addstr.str_String);
 	}
	m_mebOpenMailText.AddString(mainstr.str_String);
	
	int	nX = GetPosX();
	int	nY = GetPosY();

	SetPos( nX, nY );
	SetVisible(TRUE);
	SetFocus(TRUE);
#endif //POSTSOCKETSYSTEM	
}
void CUIMail::CUIOpenMail::CloseMail()
{
	//int		max = CMail::getInstance()->GetRecvItemDataCount();
	for(int lol = 0 ;lol < ITEMBUTTONMAX;++lol)
	{
		m_btnOpenMailItem[lol].InitBtn();

		//if( lol < max )
			CMail::getInstance()->GetRecvItemData(lol).init();
	}
	SetVisible(FALSE);
	SetFocus(FALSE);
}

CUIButtonEx* CUIMail::CUIOpenMail::GetOpenMailItemButtonEX()
{
	return m_btnOpenMailItem;
}


void	CUIMail::CUIOpenMail::CopyiteminfoFromMail(RecvItemDataArray::iterator itr, int i)
{
	m_btnOpenMailItem[i].SetItemInfo(-1, -1, -1, itr->itemIndex, -1, -1);
	m_btnOpenMailItem[i].SetItemOptionData(1,itr->option1,itr->optionLevel1, itr->originOptionVar[0]);
	m_btnOpenMailItem[i].SetItemOptionData(2,itr->option2,itr->optionLevel2, itr->originOptionVar[1]);
	m_btnOpenMailItem[i].SetItemOptionData(3,itr->option3,itr->optionLevel3, itr->originOptionVar[2]);
	m_btnOpenMailItem[i].SetItemOptionData(4,itr->option4,itr->optionLevel4, itr->originOptionVar[3]);
	m_btnOpenMailItem[i].SetItemOptionData(5,itr->option5,itr->optionLevel5, itr->originOptionVar[4]);
	m_btnOpenMailItem[i].SetItemUsed(itr->limitTime1);
	m_btnOpenMailItem[i].SetItemUsed2(itr->limitTime2);
	m_btnOpenMailItem[i].SetItemPlus(itr->plus);
	m_btnOpenMailItem[i].SetItemFlag(itr->flag);
	m_btnOpenMailItem[i].SetItemCount(itr->itemCount);
}

void CUIMail::CUIOpenMail::CopyiteminfoFromMail(ReceiveItemData& ItemData, int i)
{
	m_btnOpenMailItem[i].SetItemInfo(-1, -1, -1, ItemData.itemIndex, -1, -1);
	m_btnOpenMailItem[i].SetItemOptionData(1,ItemData.option1,ItemData.optionLevel1, ItemData.originOptionVar[0]);
	m_btnOpenMailItem[i].SetItemOptionData(2,ItemData.option2,ItemData.optionLevel2, ItemData.originOptionVar[1]);
	m_btnOpenMailItem[i].SetItemOptionData(3,ItemData.option3,ItemData.optionLevel3, ItemData.originOptionVar[2]);
	m_btnOpenMailItem[i].SetItemOptionData(4,ItemData.option4,ItemData.optionLevel4, ItemData.originOptionVar[3]);
	m_btnOpenMailItem[i].SetItemOptionData(5,ItemData.option5,ItemData.optionLevel5, ItemData.originOptionVar[4]);
	m_btnOpenMailItem[i].SetItemUsed(ItemData.limitTime1);
	m_btnOpenMailItem[i].SetItemUsed2(ItemData.limitTime2);
	m_btnOpenMailItem[i].SetItemPlus(ItemData.plus);
	m_btnOpenMailItem[i].SetItemFlag(ItemData.flag);
	m_btnOpenMailItem[i].SetItemCount(ItemData.itemCount);
}