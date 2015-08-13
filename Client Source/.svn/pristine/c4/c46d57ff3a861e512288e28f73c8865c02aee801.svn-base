#include "StdH.h"
#include "gps.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Contents/Base/UIMsgBoxMgr.h>
#include <Common/Packet/ptype_gps.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Contents/function/UIGps.h>
#include <Engine/Object/ActorMgr.h>

class CmdOpenGPS : public Command
{
public:
	void execute() {
		UIMGR()->GetGPS()->Open();
	}
};

class CmdUseNoGPS : public Command
{
public:
	void execute() {
		int nTab, nIdx;
		UIMGR()->GetInventory()->GetUseItemSlotInfo(nTab, nIdx);
		_pNetwork->UseSlotItem(nTab, nIdx, -1);
	}
};

GPS::GPS()
	: m_msgID(-1)
	, m_nTargetIdx(-1)
	, m_bPreVisible(false)
	, m_nRelicPosCnt(0)
{
	m_tarInfo.isSameZone = false;
}

void GPS::clear()
{
	m_nTargetIdx = -1;
	m_strTarget = "";
	RelicPosClear();
}

void GPS::open( bool bGPS )
{
	if (m_msgID > 0)
		MSGBOXMGR()->Close(m_msgID);

	if (bGPS == true)
	{
		// 이미 사용중이라면
		if (_pUIBuff->IsSkillBuff(1759) == TRUE)
		{
			m_msgID = MSGBOXMGR()->CreateMsgBox(_S(6280, "위치 추적"), 
				_S(6198, "이미 점술사의 마법구를 이용하여 위치를 추적 중에 ..."),
				eMSG_YESNO, true, new CmdOpenGPS);
		}
		else
		{
			m_msgID = MSGBOXMGR()->CreateMsgBox(_S(6280, "위치 추적"), 
				_S(6197, "점술사의 마법구를 사용하게 되면 신비한 힘으로 상대..."),
				eMSG_YESNO, true, new CmdOpenGPS);
		}
	}
	else
	{
		// 이미 사용중이라면
		if (_pUIBuff->IsSkillBuff(1760) == TRUE)
		{
			m_msgID = MSGBOXMGR()->CreateMsgBox(_S(6281, "위치 추적 방해"), 
				_S(6204, "이미 자신의 위치를 숨기고 있습니다. 다시 사용 ..."),
				eMSG_YESNO, true, new CmdUseNoGPS);
		}
		else
		{
			m_msgID = MSGBOXMGR()->CreateMsgBox(_S(6281, "위치 추적 방해"), 
				_S(6203, "어둠에 휩싸여 자신의 위치를 숨기고 혼란을 주어 ..."),
				eMSG_YESNO, true, new CmdUseNoGPS);
		}
	}
}

void GPS::close()
{
	// 초기화
	m_tarInfo.isSameZone = false;
	m_nTargetIdx = -1;
	m_strTarget = "";

	if (m_msgID > 0)
		MSGBOXMGR()->Close(m_msgID);

	m_msgID = -1;

	CUIManager* pUIMgr = UIMGR();
	pUIMgr->GetChattingUI()->AddSysMessage(_S(6202, "점술사의 마법구 힘이 사라져 더 이상 추적 할 수 없습니다."), SYSMSG_NORMAL);

	pUIMgr->GetCompass()->Close();
}

void GPS::findUser(const char* strName)
{
	if (strName != NULL)
	{
		int tab, idx;
		UIMGR()->GetInventory()->GetUseItemSlotInfo(tab, idx);

		CNetworkMessage nmMessage;
		RequestClient::GPSAddTarget* packet = reinterpret_cast<RequestClient::GPSAddTarget*>(nmMessage.nm_pubMessage);
		packet->type = MSG_GPS;
		packet->subType = MSG_SUB_GPS_ADD_TARGET_REQ;
		packet->tab = (short)tab;
		packet->invenIndex = idx;
		strcpy(packet->UserName, strName);
		nmMessage.setSize(sizeof(*packet));

		_pNetwork->SendToServerNew(nmMessage);
	}
}

void GPS::addTarget( int nIdx, const char* tarName )
{
	m_nTargetIdx = nIdx;
	m_strTarget = tarName;

	// 열려 있는 경우에만 메세지 추가
	if (UIMGR()->GetGPS()->GetHide() == FALSE)
		MSGBOX_OK(_S(6280, "위치 추적"), _S(6200, "입력한 플레이어의 위치 추적을 시작합니다"));

	UIMGR()->GetGPS()->Close();
	UIMGR()->GetCompass()->Open();	
}

void GPS::setInfo( UpdateClient::GPSTargetMoveInfo& tarInfo )
{
	m_tarInfo = tarInfo;
		
	if (m_tarInfo.isSameZone != m_bPreVisible)
	{
		m_bPreVisible = m_tarInfo.isSameZone;

		UIMGR()->GetCompass()->showCompass(m_bPreVisible);
	}

	// calc
	if (m_bPreVisible == true)
	{
		FLOAT3D dir = FLOAT3D(m_tarInfo.x - _pNetwork->MyCharacterInfo.x,
							0.f, 
							m_tarInfo.z - _pNetwork->MyCharacterInfo.z);

		FLOAT fLength = dir.Length();		

		ANGLE3D	angle;
		DirectionVectorToAnglesNoSnap(dir, angle);

		CUICompass* pCompass = UIMGR()->GetCompass();
		if (pCompass != NULL)
		{
			pCompass->setAngle(_pNetwork->MyCharacterInfo.camera_angle - angle(1));

			if (ACTORMGR()->GetObject(eOBJ_CHARACTER, m_nTargetIdx) != NULL)
				pCompass->setCover(CUICompass::eCOVER_GREEN);
			else
				pCompass->setCover(CUICompass::eCOVER_RED);
		}
		
	}
}

void GPS::SetFindItemInfo(UpdateClient::ArtifactGPSData& Info)
{
	m_mapRelicPos.clear();
	m_nRelicPosCnt = Info._count;

	if (m_nRelicPosCnt <= 0)
		return;
	
	for (int i = 0; i < m_nRelicPosCnt; ++i)
	{
		stFindItemInfo stInfo;
		stInfo.fX = Info.info[i].x;
		stInfo.fY = Info.info[i].h;
		stInfo.fZ = Info.info[i].z;
		stInfo.yLayer = Info.info[i].yLayer;
		stInfo.nCharIdx = Info.info[i].char_index;
		m_mapRelicPos.insert(std::make_pair(Info.info[i].char_index, stInfo));
	}			
}

void GPS::RelicPosClear()
{
	m_mapRelicPos.clear();
	m_nRelicPosCnt = 0;
}
