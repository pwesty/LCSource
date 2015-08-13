//
// Community 관련 Receive 처리

#include "StdH.h"
#include <Engine/Network/CNetwork.h>
#include "SessionState.h"
#include <Common/CommonDef.h>
#include <Common/Packet/ptype_old_do_guild.h>
#include <Common/Packet/ptype_old_do_friend.h>
#include <Common/Packet/ptype_gps.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIMessenger.h>
#include <Engine/Interface/UIMessageBox.h>
#include <Engine/interface/UIGuild.h>
#include <Engine/Contents/Base/ChattingUI.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/function/gps.h>
#include <Engine/Contents/Base/UIMsgBoxMgr.h>
#include <Engine/Info/MyInfo.h>

DECLARE_MSG_UPDATE(GuildSkillLearn);
DECLARE_MSG_UPDATE(FriendError);
DECLARE_MSG_UPDATE(FriendRegRequest);
DECLARE_MSG_UPDATE(FriendRegCancel);
DECLARE_MSG_UPDATE(FriendCondition);
DECLARE_MSG_UPDATE(FriendRegNorify);
DECLARE_MSG_UPDATE(FriendDeleteNorify);

DECLARE_MSG_UPDATE(GPSAddTarget);
DECLARE_PACKET(GPSError);
DECLARE_MSG_UPDATE(GPSTargetMoveInfo);
DECLARE_MSG_UPDATE(GPSArtifactInfo);


void CSessionState::reg_packet_comm()
{
	REG_PACKET_UPDATE(MSG_GUILD, MSG_NEW_GUILD_SKILL_LEARN, GuildSkillLearn);
	REG_PACKET_UPDATE(MSG_FRIEND, MSG_FRIEND_ERROR, FriendError);
	REG_PACKET_UPDATE(MSG_FRIEND, MSG_FRIEND_REGIST_REQUEST, FriendRegRequest);
	REG_PACKET_UPDATE(MSG_FRIEND, MSG_FRIEND_REGIST_CANCEL, FriendRegCancel);
	
	REG_PACKET_UPDATE(MSG_FRIEND, MSG_FRIEND_SET_CONDITION, FriendCondition);
	
	REG_PACKET_UPDATE(MSG_FRIEND, MSG_FRIEND_REGIST_MEMBER_NOTIFY, FriendRegNorify);
	REG_PACKET_UPDATE(MSG_FRIEND, MSG_FRIEND_DELETE_MEMBER_NOTIFY, FriendDeleteNorify);

	REG_PACKET_UPDATE(MSG_GPS, MSG_SUB_GPS_TARGET_INFO, GPSAddTarget);
	REG_PACKET(MSG_GPS, MSG_SUB_GPS_ERROR, GPSError);
	REG_PACKET_UPDATE(MSG_GPS, MSG_SUB_GPS_TARGET_MOVE_INFO, GPSTargetMoveInfo);
	REG_PACKET_UPDATE(MSG_GPS, MSG_SUB_ARTIFACT_GPS_DATA, GPSArtifactInfo);
}

IMPLEMENT_MSG_UPDATE(GuildSkillLearn)
{
	UpdateClient::doNewGuildSkillLearnToMemberMsg* pPack = reinterpret_cast<UpdateClient::doNewGuildSkillLearnToMemberMsg*>(istr->GetBuffer());

	UIMGR()->GetGuild()->UpdateGuildSkillLearnMessage(*pPack);
}

IMPLEMENT_MSG_UPDATE(FriendError)
{
	ResponseClient::doFriendError* pPack = reinterpret_cast<ResponseClient::doFriendError*>(istr->GetBuffer());

	UIMGR()->GetMessenger()->ErrorMsg(pPack->errorCode);
}

IMPLEMENT_MSG_UPDATE(FriendRegRequest)
{
	CUIManager* pUIMgr = UIMGR();

	if (!pUIMgr->GetMessenger()->IsUseLevel())
		return;
	
	pUIMgr->RearrangeOrder( UI_MESSENGER, TRUE );

	ResponseClient::doFriendRegRep* pPack = reinterpret_cast<ResponseClient::doFriendRegRep*>(istr->GetBuffer());

	if (_pNetwork->MyCharacterInfo.index == pPack->requesterindex)
	{
		CTString strMessage;
		strMessage.PrintF( _S( 1622, "%s님께 친구등록을 요청하고 있습니다. 취소하시겠습니까?" ), pPack->name );
		pUIMgr->GetMessenger()->Message( MSGCMD_MESSENGER_ADD_WAIT, _S(2996, "친구등록 요청"), strMessage, UMBS_CANCEL );
	}
	else
	{
		pUIMgr->GetMessenger()->RegistMemberReq(pPack->requesterindex, pPack->name);
	}
}

IMPLEMENT_MSG_UPDATE(FriendRegCancel)
{
	CUIManager* pUIMgr = UIMGR();

	pUIMgr->CloseMessageBox( MSGCMD_MESSENGER_ADD_REP  );
	pUIMgr->CloseMessageBox( MSGCMD_MESSENGER_ADD_WAIT  );
	pUIMgr->GetChattingUI()->AddSysMessage( _S(2082,"상대방이 친구등록을 취소하였습니다." ));			
}

IMPLEMENT_MSG_UPDATE(FriendCondition)
{
	ResponseClient::doFriendSetCondition* pPack = reinterpret_cast<ResponseClient::doFriendSetCondition*>(istr->GetBuffer());

	UIMGR()->GetMessenger()->SetMemberCondition( pPack->requester, (eCondition)pPack->condition );
}

IMPLEMENT_MSG_UPDATE(FriendRegNorify)
{
	ResponseClient::doFriendAddNotify* pPack = reinterpret_cast<ResponseClient::doFriendAddNotify*>(istr->GetBuffer());

	UIMGR()->GetMessenger()->AddFriendList( pPack->index, 0, pPack->name, 
								(eJob)pPack->job, (eCondition)pPack->condition, false );
}

IMPLEMENT_MSG_UPDATE(FriendDeleteNorify)
{
	ResponseClient::doFriendDelNotify* pPack = reinterpret_cast<ResponseClient::doFriendDelNotify*>(istr->GetBuffer());

	UIMGR()->GetMessenger()->DeleteMember(pPack->delIndex);
}

IMPLEMENT_MSG_UPDATE(GPSAddTarget)
{	
	UpdateClient::GPSAddTarget* pPack = reinterpret_cast<UpdateClient::GPSAddTarget*>(istr->GetBuffer());

	GAMEDATAMGR()->GetGPS()->addTarget(pPack->charIndex, pPack->targetName);
}

IMPLEMENT_PACKET(GPSError)
{
	ResponseClient::GPSError* pPack = reinterpret_cast<ResponseClient::GPSError*>(istr->GetBuffer());

	switch (pPack->errorCode)
	{
	case ERROR_GPS_NOT_FOUND_USER:
		MSGBOX_OK(_S(6280, "위치 추적"), _S(6201, "입력한 캐릭터는 존재하지 않습니다. 다시 확인 후 입력 해 주세요"));
		break;
	}

}

IMPLEMENT_MSG_UPDATE(GPSTargetMoveInfo)
{
	UpdateClient::GPSTargetMoveInfo* pPack = reinterpret_cast<UpdateClient::GPSTargetMoveInfo*>(istr->GetBuffer());

	GAMEDATAMGR()->GetGPS()->setInfo(*pPack);
}

IMPLEMENT_MSG_UPDATE(GPSArtifactInfo)
{
	UpdateClient::ArtifactGPSData* pPack = reinterpret_cast<UpdateClient::ArtifactGPSData*>(istr->GetBuffer());

	GAMEDATAMGR()->GetGPS()->SetFindItemInfo(*pPack);
}