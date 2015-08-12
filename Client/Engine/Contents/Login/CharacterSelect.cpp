
#include "StdH.h"

#include <Engine/Interface/UIManager.h>
#include <Engine/Network/NetworkMessage.h>
#include <Engine/network/CNetwork.h>

#include <Engine/Contents/Login/CharacterSelect.h>

CCharacterSelect::CCharacterSelect()
	: m_ulSlotExpansion(0)
	, m_ubHardCoreMsgType(0)
{

}

CCharacterSelect::~CCharacterSelect()
{

}

void CCharacterSelect::SetExpansionTime( ULONG ulTime )
{
	m_ulSlotExpansion = ulTime;
}

void CCharacterSelect::DeleteCharacter( ULONG ulCharIdx, CTString strSecuNum, BOOL bDelete )
{
	if (strSecuNum.IsEmpty() == FALSE)
		_pNetwork->SendDeleteCharacter(ulCharIdx,strSecuNum, bDelete);
	else
		_pNetwork->SendDeleteCharacter(ulCharIdx,bDelete);
}

void CCharacterSelect::Return2ServerSelectSend()
{
	// 서버선택창으로 이동해도 되나요?

	CNetworkMessage nmPacket((UBYTE)MSG_MENU);
	nmPacket << (unsigned char)MSG_RETURN_TO_SELECT_CHANNEL; 

	_pNetwork->SendToServerNew(nmPacket,TRUE);
	UIMGR()->SetCSFlagOn(CSF_TELEPORT);
}
