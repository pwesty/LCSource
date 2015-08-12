
#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/GameState.h>
#include <Engine/Base/Input.h>
#include <Engine/Sound/SoundLibrary.h>
#include <Engine/Help/Util_Help.h>
#include <Engine/LoginJobInfo.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/GameStageManager/StageMgr.h>
#include <Engine/Contents/function/SimplePlayerInfoUI.h>

#include "BackImageManager.h"
#include "ServerSelect.h"
#include "UIServerSelect.h"
#include "CharacterSelect.h"
#include "UICharacterSelect.h"
#include "UIHardCoreWarning.h"

#define DEF_SLOT_COUNT (4)	// 슬롯 리스트당 4개의 캐릭터를 보여줄수 있다.
#define DEF_SLOT_MAX   (8)	// 슬롯 맥스는 8개.
#define DEF_MAX_NAME_LEN	(16)
#define DEF_CHAR_MODEL_ID	24

class CmdSelCharExitGame : public Command
{
public:
	CmdSelCharExitGame() : m_pWnd(NULL)	{}
	void setData(CUICharacterSelect* pWnd)	{ m_pWnd = pWnd;	}
	void execute()
	{
 		if (m_pWnd)
 			m_pWnd->LogOut();
	}

private:
	CUICharacterSelect* m_pWnd;
};

class CmdSelPressOk : public Command
{
public:
	CmdSelPressOk() : m_pWnd(NULL)	{}
	void setData(CUICharacterSelect* pWnd)	{ m_pWnd = pWnd;	}
	void execute()
	{
		if (m_pWnd)
			m_pWnd->PressOkBtn();
	}
private:
	CUICharacterSelect* m_pWnd;
};

class CmdSelCharDelete : public Command
{
public:
	CmdSelCharDelete() : m_pWnd(NULL), m_nSelectIdx(-1) {}
	void setData(CUICharacterSelect* pWnd, int nSelectIdx)	
	{
		m_pWnd = pWnd;
		m_nSelectIdx = nSelectIdx;
	}
	void execute()
	{
		if (m_pWnd != NULL)
		{
			m_pWnd->DeleteCharacterMsgBox(m_nSelectIdx);
		}
	}

private:
	CUICharacterSelect* m_pWnd;
	int	m_nSelectIdx;
};

class CmdSelCharChangeServer : public Command
{
public:
	CmdSelCharChangeServer() {}

	void execute()
	{
		GameDataManager* pGameData = GameDataManager::getSingleton();

		if (pGameData == NULL)
			return;

		CCharacterSelect* pCharSel = pGameData->GetCharSelData();

		if (pCharSel == NULL)
			return;

		pCharSel->Return2ServerSelectSend();
	}
};

class CmdSelCharModel : public Command
{
public:
	CmdSelCharModel() : m_pWnd(NULL), m_nSelectIdx(-1) {}
	void setData(CUICharacterSelect* pWnd, int nSelIdx)	{ m_pWnd = pWnd; m_nSelectIdx = nSelIdx;	}
	void execute()
	{
		if (m_pWnd)
			m_pWnd->SetCharacterModel(m_nSelectIdx);
	}
private:
	CUICharacterSelect* m_pWnd;
	int m_nSelectIdx;
};

CUICharacterSelect::CUICharacterSelect()
	: m_pTextSlotInfoTitle(NULL)
	, m_pTbSlotInfoDesc(NULL)
	, m_pBtnPressOk(NULL)
	, m_pBtnServerChange(NULL)
	, m_pBtnLogout(NULL)
	, m_nDeleteCharIdx(-1)
	, m_nRenderModelOldIdx(-1)
	, m_ulExpansoinTime(0)
	, m_nPlayAnimId(0)
	, m_IsWear(false)
{
	int i;

	for (i = 0; i < eSLOT_LIST_MAX; ++i)
	{
		m_pListCharacterSlot[i] = NULL;
		m_pUIBase[i]	= NULL;
	}

}

void CUICharacterSelect::initialize()
{
	CUIButton* pBtn = NULL;

	if (m_pBtnLogout = (CUIButton*)findUI("btn_exit_game"))
	{
		CmdSelCharExitGame* pCmd = new CmdSelCharExitGame;
		pCmd->setData(this);
		m_pBtnLogout->SetCommandUp(pCmd);
	}

	if (m_pBtnServerChange = (CUIButton*)findUI("btn_server_change"))
	{
		CmdSelCharChangeServer* pCmd = new CmdSelCharChangeServer;
		m_pBtnServerChange->SetCommandUp(pCmd);
	}
	
	if (m_pBtnPressOk = (CUIButton*)findUI("btn_press_ok"))
	{
		CmdSelPressOk* pCmd = new CmdSelPressOk;
		pCmd->setData(this);
		m_pBtnPressOk->SetCommandUp(pCmd);
	}

	m_pListCharacterSlot[eSLOT_LIST_LEFT] = (CUIList*)findUI("list_character_slot_left");
	m_pListCharacterSlot[eSLOT_LIST_RIGHT] = (CUIList*)findUI("list_character_slot_right");

	m_pUIBase[0]	= findUI("left_ui");
	m_pUIBase[1]	= findUI("right_ui");

	m_pTextSlotInfoTitle = (CUIText*)findUI("text_expansion_title");
	m_pTbSlotInfoDesc = (CUITextBox*)findUI("tb_expansion_desc");
}

void CUICharacterSelect::Reset()
{
	m_nDeleteCharIdx = -1;
	m_nRenderModelOldIdx = -1;
	m_ulExpansoinTime = 0;
}

void CUICharacterSelect::Lock( BOOL bLock )
{
	if (m_pBtnPressOk)
		m_pBtnPressOk->SetEnable(!bLock);

	if (m_pBtnServerChange)
		m_pBtnServerChange->SetEnable(!bLock);

	if (m_pBtnLogout)
		m_pBtnLogout->SetEnable(!bLock);
}

void CUICharacterSelect::OpenUI()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	SetVisible(TRUE);
	Hide(FALSE);

	UpdateCharacterList();

	pUIManager->RearrangeOrder(UI_CHARACTER_SELECT, TRUE);
		
	if (_pNetwork->m_iServerType == SERVER_TYPE_HARDCORE)
	{
		GameDataManager* pGameData = GameDataManager::getSingleton();

		if (pGameData == NULL)
			return;

		CCharacterSelect* pCharSelData = pGameData->GetCharSelData();

		if (pCharSelData == NULL)
			return;

		UBYTE ubType = pCharSelData->GetHardCoreMsgType();

		pUIManager->GetHardCoreWarning()->OpenUI((eWarningType)ubType);

		pCharSelData->SetHardCoreMsgType(0);
	}	
}

void CUICharacterSelect::CloseUI()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	SetVisible(FALSE);
	Hide(TRUE);

	Reset();

	CUITooltipMgr::getSingleton()->clearTooltip();

	Lock(FALSE);
	pUIManager->RearrangeOrder(UI_CHARACTER_SELECT, FALSE);
}

void CUICharacterSelect::UpdateCharacterList()
{
	if (CEntity* penMarker = _pNetwork->ga_World.EntityFromID(24))
		penMarker->en_RenderType = CEntity::RT_SKAEDITORMODEL;

	if (m_pListCharacterSlot[eSLOT_LIST_LEFT] == NULL || m_pListCharacterSlot[eSLOT_LIST_RIGHT] == NULL)
		return;

	if (m_nRenderModelOldIdx >= 0)
		_pGameState->ClearModelEffect(m_nRenderModelOldIdx, LOGIN_MODEL_TYPE_SELECTUI);

	Reset();

	CUIManager* pUIManager = CUIManager::getSingleton();

	GameDataManager* pGameData = GameDataManager::getSingleton();

	if (pGameData == NULL)
		return;

	CCharacterSelect* pCharSelData = pGameData->GetCharSelData();

	if (pCharSelData == NULL)
		return;

	m_ulExpansoinTime = pCharSelData->GetExpansionTime();

	CUIListItem* pItem = NULL;
	CUIListItem* pTmpItem = NULL;

	int i, nCharCount = _pGameState->GetExistCharNum();
	int nItemCnt = 0;
	int nSlotListType = 0;
	
	for (i = 0; i < DEF_SLOT_MAX; ++i)
	{
		nSlotListType = i / DEF_SLOT_COUNT;

		if (nSlotListType >= eSLOT_LIST_MAX)
			continue;

		pItem = m_pListCharacterSlot[nSlotListType]->GetListItemTemplate();

		if (pItem == NULL)
			continue;

		nItemCnt = m_pListCharacterSlot[nSlotListType]->getListItemCount();

		if ((i % DEF_SLOT_COUNT) >= nItemCnt)
			m_pListCharacterSlot[nSlotListType]->AddListItem((CUIListItem*)pItem->Clone());

		pTmpItem = (CUIListItem*)m_pListCharacterSlot[nSlotListType]->GetListItem(i % DEF_SLOT_COUNT);

		if (pTmpItem == NULL)
			continue;

		if (i < nCharCount)
		{
			CmdSelCharModel* pCmd = new CmdSelCharModel;
			pCmd->setData(this, i);
			pTmpItem->SetCommand(pCmd);

			CmdSelPressOk* pCmdOk = new CmdSelPressOk;
			pCmdOk->setData(this);
			pTmpItem->SetCommandDBL(pCmdOk);	// 더블클릭시 게임 시작 가능 하도록 하기 위해,,
		}

		SetTextData(pTmpItem, i);
		SetImageData(pTmpItem, i);
		SetButtonData(pTmpItem, i);
		SetTooltipData(pTmpItem, i);
	}

	for (i = 0; i < eSLOT_LIST_MAX; ++i)
	{
		m_pListCharacterSlot[i]->UpdateList();
	}

	if (nCharCount > 0)
	{
		m_pListCharacterSlot[eSLOT_LIST_LEFT]->setCurSel(0);
		SetCharacterModel(0);
	}
	else
		m_pListCharacterSlot[eSLOT_LIST_LEFT]->setCurSel(-1);

	m_pListCharacterSlot[eSLOT_LIST_RIGHT]->setCurSel(-1);

	UpdateSlotInfo();
	UpdateServerInfo();

	int nPreviewWidth = pUIManager->GetWidth();
	int nPreviewHeight = pUIManager->GetHeight();
}

void CUICharacterSelect::SetTextData( CUIListItem* pItem, int idx )
{
	if (pItem == NULL)
		return;

	CJobInfo* pInfo = CJobInfo::getSingleton();

	if (pInfo == NULL)
		return;

	CUIImageArray* pImgArray = NULL;

	CTString strClass;
	CTString strName;
	CTString strLevel;
	CTString strGuild;

	CUIText* pText = NULL;
	CUITextBox* pTBClass = NULL;

	SLONG tv_DeleteTime = 0;

	if (idx < _pGameState->GetExistCharNum())
	{
		int	nJob = _pGameState->m_SlotInfo[idx].job;
		int nJob2 = _pGameState->m_SlotInfo[idx].job2;
	
#if		defined(G_GERMAN) || defined(G_USA)
		strClass = GetJobName(nJob, nJob2);
#else
		strClass = pInfo->GetName(nJob, nJob2);
#endif
		strName  = _pGameState->m_SlotInfo[idx].name;
		strLevel.PrintF("Lv %d", _pGameState->m_SlotInfo[idx].level);
		strGuild = _pGameState->m_SlotInfo[idx].strGuildName;

		tv_DeleteTime = _pGameState->m_SlotInfo[idx].m_time;
	}

	if (pTBClass = (CUITextBox*)pItem->findUI("tb_class_name"))
	{
		CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
		ULONG ulWidth = pDrawPort->GetTextWidth2(strClass);

		if (ulWidth > pTBClass->GetWidth())		// 러시아의 경우 텍스트 길이가 width를 초과하는 경우
			pTBClass->SetStartXY(0, 0);			// 개행 되어 지도록 하기위해.
		else
			pTBClass->SetStartXY(0, 5);	// 길이가 넘어가지 않는 다면 중앙에 출력.

		pTBClass->SetText(strClass);
	}

	if (pText = (CUIText*)pItem->findUI("text_name"))
		pText->SetText(strName);

	if (pText = (CUIText*)pItem->findUI("text_level"))
	{
		if (strLevel.IsEmpty() == TRUE)
		{
			strLevel = _S(140, "캐릭터 생성");

			if (idx >= DEF_SLOT_COUNT)
			{
				if (m_ulExpansoinTime <= 0)
				{
					strLevel = _S(6092, "잠긴 슬롯");
				}
			}
		}
		pText->SetText(strLevel);
	}

	if (pText = (CUIText*)pItem->findUI("text_guild"))
	{
		if (tv_DeleteTime > 0)
		{
			pText->SetText(_S(6196, "삭제 대기시간"));
			pText->setFontColor(DEF_UI_COLOR_RED);
		}
		else
			pText->SetText(strGuild);
	}

	if (pText = (CUIText*)pItem->findUI("text_del_time"))
	{
		if (tv_DeleteTime > 0)
		{
			CTString strTmp;

			int tv_hour;
			tv_hour = tv_DeleteTime / DEF_DATE_ONE_HOUR;

			strTmp.PrintF(_S(2512, "%d시간"), tv_hour);
			pText->Hide(FALSE);
			pText->SetText(strTmp);
		}
		else
			pText->Hide(TRUE);
	}
}

void CUICharacterSelect::SetImageData( CUIListItem* pItem, int idx )
{
	if (pItem == NULL)
		return;

	CUIImageArray* pImgArray = NULL;

	int nJob = -1;
	SLONG tv_DeleteTime = 0;
	eCHAR_SLOT_STATE eType = eSLOT_STATE_NONE;

	if (idx < _pGameState->GetExistCharNum())	// 캐릭터가 있다.
	{
		tv_DeleteTime = _pGameState->m_SlotInfo[idx].m_time;
		nJob = _pGameState->m_SlotInfo[idx].job;
	}
	else
	{
		eType = eSLOT_STATE_EMPTY;

		if (idx >= DEF_SLOT_COUNT)
		{
			if (m_ulExpansoinTime <= 0)
				eType = eSLOT_STATE_LOCK;
		}
	}

	if (pImgArray = (CUIImageArray*)pItem->findUI("img_array_char_slot"))
		pImgArray->SetRenderIdx((int)eType);

	if (pImgArray = (CUIImageArray*)pItem->findUI("img_array_class"))
	{
		if (eType == eSLOT_STATE_NONE)
		{
			if (tv_DeleteTime > 0)	// 0~8까지 캐릭터 클래스 이미지. 9는 해골 이미지.
				pImgArray->SetRenderIdx(DEF_SLOT_MAX + 1); 
			else
				pImgArray->SetRenderIdx(nJob);
		}
		else
			pImgArray->SetRenderIdx(-1);
	}
}

void CUICharacterSelect::SetButtonData( CUIListItem* pItem, int idx )
{
	if (pItem == NULL)
		return;

	CUIButton* pDeleteButton = (CUIButton*)pItem->findUI("btn_char_delete");
	CUIButton* pDeleteCancelBtn = (CUIButton*)pItem->findUI("btn_char_del_cancel");

	if (pDeleteButton == NULL || pDeleteCancelBtn == NULL)
		return;

	if (idx < _pGameState->GetExistCharNum())
	{
		SLONG slDelTime = _pGameState->m_SlotInfo[idx].m_time;

		CmdSelCharDelete* pCmd = new CmdSelCharDelete;
		pCmd->setData(this, idx);

		if (slDelTime > 0)
		{
			pDeleteButton->Hide(TRUE);
			pDeleteCancelBtn->Hide(FALSE);
			
			pDeleteCancelBtn->SetCommandUp(pCmd);
		}
		else
		{
			pDeleteButton->Hide(FALSE);
			pDeleteCancelBtn->Hide(TRUE);

			pDeleteButton->SetCommandUp(pCmd);
		}
	}
	else
	{
		pDeleteButton->Hide(TRUE);
		pDeleteCancelBtn->Hide(TRUE);
	}
}

void CUICharacterSelect::SetTooltipData( CUIListItem* pItem, int idx )
{
	if (pItem == NULL)
		return;

	CTString strTooltip;

	if (idx >= _pGameState->GetExistCharNum())
	{
		strTooltip = _S(6093, "캐릭터 생성");

		if (idx >= DEF_SLOT_COUNT)
		{
			if (m_ulExpansoinTime <= 0)
			{
				strTooltip = _S(6094, "잠긴 슬롯");
			}
		}
	}

	pItem->setTooltip(strTooltip.str_String);
}

void CUICharacterSelect::UpdateSlotInfo()
{
	if (m_pTextSlotInfoTitle == NULL || m_pTbSlotInfoDesc == NULL)
		return;

	BOOL bHide = TRUE;

	if (m_ulExpansoinTime > 0)
	{
		bHide = FALSE;

		CTString strTmp = "";

		if (m_ulExpansoinTime >= DEF_DATE_ONE_DAY)
			strTmp.PrintF( _S( 2511, "%d일"), m_ulExpansoinTime / DEF_DATE_ONE_DAY);
		else if (m_ulExpansoinTime >= DEF_DATE_ONE_HOUR)
			strTmp.PrintF( _S( 2512, "%d시간"), m_ulExpansoinTime / DEF_DATE_ONE_HOUR);
		else
			strTmp.PrintF( _S( 2513, "%d분"), m_ulExpansoinTime / DEF_DATE_ONE_MIN);

		CTString strText = getText(5707);

		m_pTextSlotInfoTitle->SetText(strText + strTmp);
	}

	m_pTextSlotInfoTitle->Hide(bHide);
	m_pTbSlotInfoDesc->Hide(bHide);
}

void CUICharacterSelect::UpdateServerInfo()
{
	int nRecentServer = _pNetwork->m_iServerGroup;
	int nRecentChannel = _pNetwork->m_iServerCh;

	CUIManager* pUIManager = CUIManager::getSingleton();

	CTString strServer = pUIManager->GetServerSelect()->GetServerName(nRecentServer);
	CTString strChannel;

	strChannel.PrintF("-%d", nRecentChannel);

	strChannel += pUIManager->GetServerSelect()->GetChannelSubString(nRecentServer, nRecentChannel);

	CUIText* pText = NULL;

	if (pText = (CUIText*)findUI("text_server"))
		pText->SetText(strServer);

	if (pText = (CUIText*)findUI("text_channel"))
		pText->SetText(strServer + strChannel);
}

void CUICharacterSelect::DeleteCharacter(CTString strSecurity)
{
	if (m_nDeleteCharIdx < 0 && m_nDeleteCharIdx >= _pGameState->GetExistCharNum())
		return;

	if( _pNetwork->m_bSendMessage == TRUE )
		return;

	GameDataManager* pGameData = GameDataManager::getSingleton();

	if (pGameData == NULL)
		return;

	CCharacterSelect* pCharSelData = pGameData->GetCharSelData();

	if (pCharSelData == NULL)
		return;

	ULONG ulCharIdx = _pGameState->m_SlotInfo[m_nDeleteCharIdx].index;
	BOOL tv_chk = FALSE;

	if (_pGameState->m_SlotInfo[m_nDeleteCharIdx].m_time > 0)
		tv_chk = TRUE;	// 삭제 대기시간이 남아 있는 상태에서 삭제 취소 버튼을 눌렀을 경우 BOOL값.
	
	pCharSelData->DeleteCharacter(ulCharIdx, strSecurity, tv_chk);
}

void CUICharacterSelect::DeleteCharacterMsgBox(int nDelIdx)
{
	m_nDeleteCharIdx = nDelIdx;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if (m_nDeleteCharIdx < 0 && m_nDeleteCharIdx >= _pGameState->GetExistCharNum())
	{
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 241, "캐릭터 선택 오류" ), UMBS_OK,
			UI_CHARACTER_SELECT, MSGCMD_SELCHAR_ERROR );
		MsgBoxInfo.AddString( _S( 242, "캐릭터를 선택하지 않으셨습니다." ) );
		pUIManager->CreateMessageBox( MsgBoxInfo );

		return;
	}
	else
	{
		BOOL tv_chk = FALSE;

		// FIXME : 캐릭터 삭제 메세지 정의할것.
		pUIManager->CloseMessageBox( MSGCMD_DELETE_CHARACTER );

		CUIMsgBox_Info	MsgBoxInfo;
#if defined G_KOR || defined G_THAI
		int tv_line;
		// 주민번호 -> 주민번호 뒤 7자리 [10/29/2009 rumist]
		MsgBoxInfo.SetMsgBoxInfo( _S( 240, "캐릭터 삭제" ), UMBS_OK | UMBS_INPUTPASSWORD,
			UI_CHARACTER_SELECT, MSGCMD_DELETE_CHARACTER, 260 );

		if(_pGameState->m_SlotInfo[m_nDeleteCharIdx].m_time > 0) 
			tv_chk = TRUE;

		if( !tv_chk )
		{
			// [2012/07/05 : Sora]  캐릭터 슬롯 확장
			if( m_nDeleteCharIdx >= 4 &&  m_ulExpansoinTime == 0 )
			{
				MsgBoxInfo.SetMsgBoxInfo( _S( 42, "오류 발생" ), UMBS_OK, UI_CHARACTER_SELECT, MSGCMD_SELCHAR_ERROR );
				MsgBoxInfo.AddString( _S( 5703, "해당 기능은 캐릭터 확장 슬롯 아이템 사용 시 가능 합니다." ) );
				pUIManager->CreateMessageBox( MsgBoxInfo );
				
				return;
			}

			MsgBoxInfo.AddString( _S( 2904, "아이디를 입력하세요.\n(삭제요청 후 24시간이 지나면 자동삭제)" ) );
			MsgBoxInfo.AddString( CTString( " " ) );
			tv_line = 3;
		}
		else 
		{
			MsgBoxInfo.AddString( _S( 2905, "캐릭 삭제를 취소하시겠습니까?" ) );
			tv_line = 2;

		}

		MsgBoxInfo.AddString( CTString( " " ) );

		// 주민번호 -> 주민번호 뒤 7자리 [10/29/2009 rumist]
		//MsgBoxInfo.AddStringEx( _S( 2906, "아이디 입력"),tv_line,0,0xFF9156FF);
		MsgBoxInfo.SetInputBox(tv_line,10,DEF_MAX_NAME_LEN+1);
		pUIManager->CreateMessageBox( MsgBoxInfo );
		Lock(TRUE);
#else // G_KOR

		MsgBoxInfo.SetMsgBoxInfo( _S( 240, "캐릭터 삭제" ), UMBS_YESNO,
			UI_CHARACTER_SELECT, MSGCMD_DELETE_CHARACTER );

		tv_chk = FALSE;

		if(_pGameState->m_SlotInfo[m_nDeleteCharIdx].m_time > 0) 
			tv_chk = TRUE;

		if( !tv_chk )
		{
			// [2012/07/05 : Sora]  캐릭터 슬롯 확장
			if( m_nDeleteCharIdx >= 4 &&  m_ulExpansoinTime == 0 )
			{
				pUIManager->CloseMessageBox( MSGCMD_NULL );

				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 42, "오류 발생" ), UMBS_OK, UI_CHARACTER_SELECT, MSGCMD_SELCHAR_ERROR );
				MsgBoxInfo.AddString( _S( 5703, "해당 기능은 캐릭터 확장 슬롯 아이템 사용 시 가능 합니다." ) );
				pUIManager->CreateMessageBox( MsgBoxInfo );

				return;
			}

			MsgBoxInfo.AddString( _S( 2904, "캐릭터 삭제는 삭제요청 후 24시간이 지나야 삭제가 됩니다. 단, 시간안에 취소 가능" ) );
			MsgBoxInfo.AddString( CTString( " " ) );
		}
		else 
		{
			MsgBoxInfo.AddString( _S( 2905, "캐릭 삭제를 취소하시겠습니까?" ) );
		}

		pUIManager->CreateMessageBox( MsgBoxInfo );
#endif // G_KOR
	}
}

void CUICharacterSelect::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_DELETE_CHARACTER:
		if( bOK )
		{
			_pSound->Mute();
			DeleteCharacter(strInput);
#ifdef RESTRICT_SOUND
			extern BOOL _bNTKernel;
			//if( _bNTKernel )
			_pSound->UpdateSounds();
#endif
		}
		else
		{
			Lock(FALSE);
		}
		break;
	}
}

WMSG_RESULT CUICharacterSelect::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	int i;
	int nOldSelectIdx = -1;
	int	nHitTest	  = -1;

	for (i = 0; i < eSLOT_LIST_MAX; ++i)
	{
		if (m_pListCharacterSlot[i] == NULL)
			continue;

		nHitTest = m_pListCharacterSlot[i]->HitTest(x, y);

		if (nHitTest != -1 && (nHitTest + (i*DEF_SLOT_COUNT) >= _pGameState->GetExistCharNum()))
		{
			m_pListCharacterSlot[eSLOT_LIST_LEFT]->setCurSel(nOldSelectIdx);
			return WMSG_SUCCESS;
		}

		if (m_pListCharacterSlot[i]->getCurSel() != -1)
			nOldSelectIdx = m_pListCharacterSlot[i]->getCurSel() + (i * DEF_SLOT_COUNT);

		m_pListCharacterSlot[i]->setCurSel(nHitTest);
	}

	if (m_pListCharacterSlot[eSLOT_LIST_LEFT]->getCurSel() == -1 &&
		m_pListCharacterSlot[eSLOT_LIST_RIGHT]->getCurSel() == -1)
	{
		m_pListCharacterSlot[nOldSelectIdx / DEF_SLOT_COUNT]->setCurSel(nOldSelectIdx % DEF_SLOT_COUNT);
		return WMSG_SUCCESS;
	}

	return WMSG_FAIL;
}

WMSG_RESULT CUICharacterSelect::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	int	nHitTest = -1;
	int i;

	for (i = 0; i < eSLOT_LIST_MAX; ++i)
	{
		if (m_pListCharacterSlot[i] == NULL)
			continue;

		nHitTest = m_pListCharacterSlot[i]->HitTest(x, y);

		if (nHitTest != -1 && (nHitTest + (i*DEF_SLOT_COUNT) >= _pGameState->GetExistCharNum()))
		{
			if (nHitTest + (i*DEF_SLOT_COUNT) >= DEF_SLOT_COUNT)
			{
				if (m_ulExpansoinTime > 0)
				{
					m_pListCharacterSlot[i]->setCurSel(nHitTest);
					PressOkBtn();
				}
			}
			else
			{
				m_pListCharacterSlot[i]->setCurSel(nHitTest);
				PressOkBtn();
			}
		}
	}

	return WMSG_FAIL;
}

WMSG_RESULT CUICharacterSelect::OnLButtonDBLClick( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	int	nHitTest = -1;
	int i;

	for (i = 0; i < eSLOT_LIST_MAX; ++i)
	{
		if (m_pListCharacterSlot[i] == NULL)
			continue;

		nHitTest = m_pListCharacterSlot[i]->HitTest(x, y);

		// 더블클릭 이벤트 발생시 빈 슬롯일 경우 마우스 메시지 child로 안보냄.( 빈슬롯 선택 되는 문제 때문에 )
		if (nHitTest != -1 && (nHitTest + (i*DEF_SLOT_COUNT) >= _pGameState->GetExistCharNum()))
			return WMSG_SUCCESS;
	}

	return WMSG_FAIL;
}

void CUICharacterSelect::LogOut()
{
	_cmiComm.Reconnect(_cmiComm.cci_szAddr, _cmiComm.cci_iPort);
	_pGameState->ClearModelEffect(m_nRenderModelOldIdx, LOGIN_MODEL_TYPE_SELECTUI);
	STAGEMGR()->SetNextStage(eSTAGE_LOGIN);
}

void CUICharacterSelect::StartGame(int nSelectIdx)
{
	// 	if( PopUpMsgBoxExist() )
	// 		return;

	ULONG	ulStartIndex;
	ulStartIndex	= _pGameState->m_SlotInfo[nSelectIdx].index;

	if(_pNetwork->m_bSendMessage)
		return;

	_pNetwork->SendGameStart(ulStartIndex);

	CUIManager* pUIManager = CUIManager::getSingleton();

	if (_pNetwork->m_iServerType == SERVER_TYPE_HARDCORE)
		pUIManager->GetHardCoreWarning()->CloseUI();

	Lock(TRUE);

	LOG_DEBUG("CUICharacterSelect::StartGame()");
	UIMGR()->GetSimplePlayerInfo()->SetDungeonList(0);
}

void CUICharacterSelect::CreateCharacter()
{
	_pGameState->ClearModelEffect(m_nRenderModelOldIdx, LOGIN_MODEL_TYPE_SELECTUI);
	STAGEMGR()->SetNextStage(eSTAGE_CREATECHAR);
}

void CUICharacterSelect::PressOkBtn()
{
	if (m_pBtnPressOk != NULL && m_pBtnPressOk->GetBtnState() == UBS_DISABLE)
		return;	// 더블 클릭으로 게임 접속시 한번 더 눌리는 경우가 발생. 예외처리 해둠.

	if (m_pListCharacterSlot[eSLOT_LIST_LEFT] == NULL || m_pListCharacterSlot[eSLOT_LIST_LEFT] == NULL)
		return;

	int iSelectedSlot = m_pListCharacterSlot[eSLOT_LIST_LEFT]->getCurSel();

	if (m_pListCharacterSlot[eSLOT_LIST_RIGHT]->getCurSel() >= 0)
	{
		iSelectedSlot = m_pListCharacterSlot[eSLOT_LIST_RIGHT]->getCurSel();

		if (iSelectedSlot >= 0)
			iSelectedSlot += DEF_SLOT_COUNT;
		else
			return;
	}

	if (iSelectedSlot >= _pGameState->GetExistCharNum())	// 빈 슬롯일 경우 CreateCharacter
	{
		if (iSelectedSlot < DEF_SLOT_COUNT )
			CreateCharacter();
		else
		{
			if (m_ulExpansoinTime > 0)
				CreateCharacter();
			
			// 잠겨있는 슬롯을 더블클릭 할 경우. 예외처리 
		}
		return;
	}

	if (_pGameState->m_SlotInfo[iSelectedSlot].m_time > 0 || 
		_pGameState->m_SlotInfo[iSelectedSlot].sbMoveState != eMOVE_STATE_NORMAL)	// 선택한 캐릭터가 삭제 대기중이거나 서버 이전중이 아니라면,,
		return;

	StartGame(iSelectedSlot);
}

void CUICharacterSelect::ChangeServer()
{
	if (GameDataManager* pGameData = GameDataManager::getSingleton())
	{
		_pGameState->ClearModelEffect(m_nRenderModelOldIdx, LOGIN_MODEL_TYPE_SELECTUI);

		if (CServerSelect* pServerSelect = pGameData->GetServerData())
		{
			pServerSelect->ConnectToServer(_cmiComm.cci_szAddr, _cmiComm.cci_iPort);
			CBackImageManager::getSingleton()->SetRenderTex(CBackImageManager::eTEX_LOGIN_BACK);
		}

		_pGameState->DeleteSelectModel();
	}
}

void CUICharacterSelect::SetCharacterModel(int nSelIdx)
{
	if (nSelIdx <= 0 && nSelIdx >= _pGameState->GetExistCharNum())
		return;

	if (nSelIdx == m_nRenderModelOldIdx)
		return;

	if (m_nRenderModelOldIdx >= 0)
	{
		_pGameState->ClearModelEffect(m_nRenderModelOldIdx, LOGIN_MODEL_TYPE_SELECTUI);
	}

	CEntity* pEntity = _pGameState->GetModelEntity(FALSE, nSelIdx);

	if (pEntity == NULL)
		return;

	if (CEntity* penMarker = _pNetwork->ga_World.EntityFromID(24))
		penMarker->en_RenderType = CEntity::RT_SKAMODEL;
	
	bool bDelete = false;
	COLOR colModel = 0xFFFFFFFF;

	if( _pGameState->m_SlotInfo[nSelIdx].m_time > 0 )
	{
		bDelete = true;
		colModel = 0x000000FF;	// 캐릭터 삭제중이라면 모델을 까맣게 표시.
	}

	pEntity->GetModelInstance()->SetModelColor(colModel);

	int nMotion = GetMotionIdx(bDelete);
	_pGameState->SelectSlot(nSelIdx, nMotion);

	{	// 무기나 방패 착용 확인.

		CEntity* pSelEntity = _pGameState->GetModelEntity(FALSE, nSelIdx);

		if (pSelEntity == NULL)
			return;

		SLONG iWeapon = _pGameState->m_SlotInfo[nSelIdx].wear[WEAR_WEAPON];
		SLONG iShield = _pGameState->m_SlotInfo[nSelIdx].wear[WEAR_SHIELD];
		int iJob = _pGameState->m_SlotInfo[nSelIdx].job;

		if (iWeapon > 0)	// 착용 무기가 있다면 해제.
		{
			CItemData* pID = _pNetwork->GetItemData(iWeapon);

			if (pID != NULL)
				_pGameState->TakeOffArmor(pSelEntity->GetModelInstance(), *pID);

			_pGameState->SetItemEffect(pSelEntity, iJob, iWeapon, -1, WEAR_WEAPON);

			m_IsWear = false;
		}

		if (iShield > 0)
		{
			CItemData* pID = _pNetwork->GetItemData(iShield);

			if (pID != NULL)
				_pGameState->TakeOffArmor(pSelEntity->GetModelInstance(), *pID);

			_pGameState->SetItemEffect(pSelEntity, iJob, iShield, -1, WEAR_SHIELD);

			m_IsWear = false;
		}
	}

	m_nRenderModelOldIdx = nSelIdx;
}

void CUICharacterSelect::OnUpdatePosition()
{
	// 해상도에 따라서 UI정렬 offset 값 세팅
	if (m_pUIBase[0] == NULL || m_pUIBase[1] == NULL)
		return;

	CBackImageManager* pBackManager = CBackImageManager::getSingleton();
	CUIManager* pUIManager = CUIManager::getSingleton();

	int x = 0, y = 0;
	int w, h;

	w = pUIManager->GetWidth();
	h = pUIManager->GetHeight();

	SetSize(w, h);	// 최상위 window 사이즈를 조절해줘야 child로 있는 UIBase그룹이 정렬이 됨.

	pBackManager->GetArrangeOffset(x, y);

	m_pUIBase[0]->SetPos(x, y);
	m_pUIBase[1]->SetPos(x, y);
}

void CUICharacterSelect::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	EquipWear();
}

void CUICharacterSelect::EquipWear()
{
	if (m_IsWear == true)
		return;

	if (m_nRenderModelOldIdx < 0 || m_nRenderModelOldIdx >= DEF_SLOT_MAX)
		return;

	int nJob = _pGameState->m_SlotInfo[m_nRenderModelOldIdx].job;

	if (_pGameState->IsPlayAnim(FALSE, m_nRenderModelOldIdx, nJob, m_nPlayAnimId) == true)
		return;

	SLONG iWeapon = _pGameState->m_SlotInfo[m_nRenderModelOldIdx].wear[WEAR_WEAPON];
	SLONG iShield = _pGameState->m_SlotInfo[m_nRenderModelOldIdx].wear[WEAR_SHIELD];
	int nPlus = -1;

	CEntity* pEntity = _pGameState->GetModelEntity(FALSE, m_nRenderModelOldIdx);

	if (iWeapon > 0)
	{
		CItemData* pID = _pNetwork->GetItemData(iWeapon);

		if (pID != NULL || pEntity != NULL)
			_pGameState->WearingArmor(pEntity->GetModelInstance(), *pID);

		nPlus = _pGameState->m_SlotInfo[m_nRenderModelOldIdx].itemPlus[WEAR_WEAPON];
		_pGameState->SetItemEffect(pEntity, nJob, iWeapon, nPlus, WEAR_WEAPON);
	}
	
	if (iShield > 0)
	{
		CItemData* pID = _pNetwork->GetItemData(iShield);

		if (pID != NULL || pEntity != NULL)
			_pGameState->WearingArmor(pEntity->GetModelInstance(), *pID);

		nPlus = _pGameState->m_SlotInfo[m_nRenderModelOldIdx].itemPlus[WEAR_SHIELD];
		_pGameState->SetItemEffect(pEntity, nJob, iShield, nPlus, WEAR_SHIELD);
	}
	
	m_IsWear = true;
}

WMSG_RESULT CUICharacterSelect::OnKeyMessage( MSG* pMsg )
{
	if( pMsg->wParam == VK_RETURN )
	{	
		PressOkBtn();
		return WMSG_SUCCESS;
	}

	return WMSG_FAIL;
}

int CUICharacterSelect::GetMotionIdx(bool bDelete)
{
	srand(time(NULL));

	int nMotion[]	= {ANIM_SOCIAL_0, ANIM_SOCIAL_1, ANIM_SOCIAL_3, ANIM_SOCIAL_4, ANIM_SOCIAL_6, ANIM_SOCIAL_8, ANIM_SOCIAL_10};
	int nDelMotion[] = {ANIM_SOCIAL_2, ANIM_SOCIAL_5, ANIM_SOCIAL_7};
	int nCount = 7;	// 일반 소셜모션 갯수.
	int nMotionIdx = 0;

	if (bDelete == true)
		nCount = 3;	// 지우고 있는 상태일 경우.

	nMotionIdx = rand() % nCount;

	if (bDelete == true)
		m_nPlayAnimId = nDelMotion[nMotionIdx];
	else
		m_nPlayAnimId = nMotion[nMotionIdx];

	return m_nPlayAnimId;
}

CTString CUICharacterSelect::GetJobName( int nJob, int n2ndJob /* =0 */)
{
	if (nJob < 0 && n2ndJob < 0)
		return CTString("");

	int class_[TOTAL_JOB][3] = 
	{
		{ 6230, 6239, 6247 },	// 타이탄
		{ 6231, 6240, 6248 },	// 나이트
		{ 6232, 6241, 6249 },	// 힐러
		{ 6233, 6242, 6250 },	// 메이지
		{ 6234, 6243, 6251 },	// 로그
		{ 6235, 6244, 6252 },	// 소서러
		{ 6236, 6236, 6236 },	// 나이트셰도우
#ifdef CHAR_EX_ROGUE
		{ 6237, 6245, 6253 },	// EX로그
#endif
#ifdef CHAR_EX_MAGE
		{ 6238, 6246, 6254 },	// EX메이지
#endif
	};

	return _S(class_[nJob][n2ndJob], "");
}

void CUICharacterSelect::DeleteModel()
{
	if (m_pListCharacterSlot[eSLOT_LIST_LEFT] == NULL || m_pListCharacterSlot[eSLOT_LIST_LEFT] == NULL)
		return;

	// 현재 선택되어진 모델 삭제.
	int iSelectedSlot = m_pListCharacterSlot[eSLOT_LIST_LEFT]->getCurSel();

	if (m_pListCharacterSlot[eSLOT_LIST_RIGHT]->getCurSel() >= 0)
	{
		iSelectedSlot = m_pListCharacterSlot[eSLOT_LIST_RIGHT]->getCurSel();

		if (iSelectedSlot >= 0)
			iSelectedSlot += DEF_SLOT_COUNT;
		else
			return;
	}

	// 로그인 전용 캐릭터 삭제
	_pGameState->ClearModelEffect(iSelectedSlot, LOGIN_MODEL_TYPE_SELECTUI);
	_pGameState->DeleteSelectModel();
	_pGameState->ClearCharacterSlot();
	CLoginJobInfo::getSingleton()->LoginModelDelete();
}
