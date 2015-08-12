#include "StdH.h"
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIMessageBox.h>
#include <Engine/Interface/UIMonsterCombo.h>
#include "SessionState.h"
#include <Common/Packet/ptype_old_extend.h>
#include <Common/Packet/ptype_old_do_monstercombo.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Login/CharacterSelect.h>
#include <Engine/LoginJobInfo.h>
#include <Engine/Contents/function/attendance.h>
#include <Engine/Contents/function/News.h>

bool CSessionState::RecvExtend(CNetworkMessage* istr)
{
	bool bRet = true;
	pTypeThirdBase* pBase = reinterpret_cast<pTypeThirdBase*>(istr->GetBuffer());

	switch (pBase->subType)
	{
	case MSG_EX_RESTART:
		recvExRestart(istr);
		break;
	
	case MSG_EX_MONSTERCOMBO:
		recvExMonsterCombo(istr);
		break;

	case MSG_EX_ATTENDANCE_EXP_SYSTEM:
		ReceiveAttendanceMessage(istr);
		break;
	case MSG_EX_NOTICE:
		ReceiveNewsMessage(istr);
		break;
	default:
		bRet = false;
		break;
	}

	return bRet;
}

void CSessionState::recvExRestart( CNetworkMessage* istr )
{
	_pNetwork->SetDelivery(true);
	_pNetwork->SendReceiveRestartGame();

	ResponseClient::exRestart* pRecv = reinterpret_cast<ResponseClient::exRestart*>(istr->GetBuffer());

	extern BOOL _bLoginProcess;
	_bLoginProcess = TRUE;
	_pNetwork->bMoveCharacterSelectUI = TRUE;
	CLoginJobInfo::getSingleton()->LoginModelCreate();

	if (pRecv->thirdType == 1)
	{
		GameDataManager* pGame = GameDataManager::getSingleton();

		if (pGame != NULL)
		{
			pGame->GetCharSelData()->SetHardCoreMsgType(1); // 0 일반 접속, 1 캐릭터가 죽은 뒤 캐릭터 선택창으로 보내짐
		}
	}
}

void CSessionState::recvExMonsterCombo( CNetworkMessage* istr )
{
	pTypeThirdBase* pBase = reinterpret_cast<pTypeThirdBase*>(istr->GetBuffer());
	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(pBase->thirdType)
	{
	case  MSG_EX_MONSTERCOMBO_EDIT_CONTEXT_REP:
		{
			ResponseClient::MCEditContext* pPack = reinterpret_cast<ResponseClient::MCEditContext*>(istr->GetBuffer());

			int* pList = NULL;

			if (pPack->count > 0)
			{
				pList = new int[pPack->count];
				memcpy(pList, &pPack->list[0], sizeof(int) * pPack->count);
			}				

			for(int i = 0; i < pPack->count; i++)
			{
				pUIManager->GetCombo()->SetComboList(i, pList[i]);
			}
			pUIManager->GetCombo()->SetComboCount(pPack->count);
			pUIManager->GetCombo()->SetActionChack(TRUE);
			pUIManager->GetCombo()->OpenMonsterCombo(TRUE,_pNetwork->MyCharacterInfo.x,_pNetwork->MyCharacterInfo.z);

			SAFE_ARRAY_DELETE(pList);
		}
		break;
	case MSG_EX_MONSTERCOMBO_GOTO_COMBO_PROMPT:
		{
			ResponseClient::MCGotoWaitRoomPrompt* pPack = reinterpret_cast<ResponseClient::MCGotoWaitRoomPrompt*>(istr->GetBuffer());

			CUIMsgBox_Info	MsgBoxInfo;
			CTString tv_str;

			pUIManager->GetCombo()->SetBossIndex(pPack->boosIndex);
			if(pUIManager->DoesMessageBoxExist(MSGCMD_EX_MONSTERCOMBO_GOTO_COMBO_PROMPT)) return ;
			MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인" ), UMBS_YESNO, UI_NONE, MSGCMD_EX_MONSTERCOMBO_GOTO_COMBO_PROMPT );
			tv_str.PrintF(_S(4049,"몬스터 콤보 %d Nas 입장료를 내고 입장하시겠습니까?"), pPack->nas);
			MsgBoxInfo.AddString(tv_str);
			pUIManager->CreateMessageBox( MsgBoxInfo );
		}
		break;
	case MSG_EX_MONSTERCOMBO_MISSION_COMPLETE:
		{
			ResponseClient::MCMissionComplete* pPack = reinterpret_cast<ResponseClient::MCMissionComplete*>(istr->GetBuffer());

			pUIManager->GetCombo()->StageComplete(pPack->stage, pPack->success);
		}
		break;
	case MSG_EX_MONSTERCOMBO_NOTICE_STAGE:  // stage(n) 시작시 stage 넘버메시지 전달
		{
			ResponseClient::MCNoticeStage* pPack = reinterpret_cast<ResponseClient::MCNoticeStage*>(istr->GetBuffer());
			
			pUIManager->GetCombo()->SetStageNum(pPack->stage);
			pUIManager->GetCombo()->SetSysImage(SYS_STAGE,TRUE);				
		}
		break;
	case MSG_EX_MONSTERCOMBO_ERROR:
		{
			ResponseClient::MCErrorMsg* pPack = reinterpret_cast<ResponseClient::MCErrorMsg*>(istr->GetBuffer());
			
			pUIManager->GetCombo()->RecComboErrorMessage(pPack->errorCode);
		}
		break;
	}

}


void CSessionState::ReceiveAttendanceMessage( CNetworkMessage* istr )
{
	GAMEDATAMGR()->GetAttendance()->RecvAttendanceMsg(istr);
}

void CSessionState::ReceiveNewsMessage(CNetworkMessage* istr)
{
	GAMEDATAMGR()->GetNews()->RecvMsg(istr);
}