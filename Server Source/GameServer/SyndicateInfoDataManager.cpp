#include "stdhdrs.h"

#include "SyndicateInfoDataManager.h"
#include "../ShareLib/packetType/ptype_syndicate.h"

//////////////////////////////////////////////////////////////////
//SyndicateInfoDataManager
SyndicateInfoDataManager::SyndicateInfoDataManager() {};
SyndicateInfoDataManager::~SyndicateInfoDataManager() {};
SyndicateInfoDataManager* SyndicateInfoDataManager::instance()
{
	static SyndicateInfoDataManager __instance;
	return &__instance;
}

void SyndicateInfoDataManager::setJewelPoint(int syndicateType, int JewelPoint)
{
	info_[syndicateType].jewelPoint_ = JewelPoint;
}

int SyndicateInfoDataManager::getJewelPoint(int syndicateType)
{
	return info_[syndicateType].jewelPoint_;
}

void SyndicateInfoDataManager::setJoinCount(int syndicateType, int joinCount)
{
	info_[syndicateType].joinCount_ = joinCount;
}

int	SyndicateInfoDataManager::getJoinCount(int syndicateType)
{
	return info_[syndicateType].joinCount_;
}

void SyndicateInfoDataManager::setKingName( int syndicateType, const char* kingName )
{
	strncpy(info_[syndicateType].kingName, kingName, MAX_CHAR_NAME_LENGTH);
	info_[syndicateType].kingName[MAX_CHAR_NAME_LENGTH] = '\0';
}

char* SyndicateInfoDataManager::getKingName( int syndicateType )
{
	return info_[syndicateType].kingName;
}
void SyndicateInfoDataManager::sendKingNameInfo( CDescriptor* dest )
{
	if(dest == NULL)
	{
		//전체 유저에게?
	}
	else
	{
		int syndicateType = dest->m_pChar->getSyndicateType();

		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::SyndicateKingNameUpdate* packet = reinterpret_cast<UpdateClient::SyndicateKingNameUpdate*>(rmsg->m_buf);
		packet->type = MSG_RVR;
		packet->subType = MSG_UPDATE_KINGNAME;
		packet->syndicateType = syndicateType;
		strncpy(packet->kingName, this->getKingName(syndicateType), MAX_CHAR_NAME_LENGTH);
		packet->kingName[MAX_CHAR_NAME_LENGTH] = '\0';

		rmsg->setSize(sizeof(UpdateClient::SyndicateKingNameUpdate));
		SEND_Q(rmsg, dest);
	}
}

void SyndicateInfoDataManager::sendJewelPercentageInfo( CDescriptor* dest )
{
	if(dest == NULL)
	{
		//전체 유저에게?
	}
	else
	{
		//카이룩스를 보내주면 나머지 퍼센트는 자동적으로 나오므로 서버는 카이룩스의 퍼센테이지만 보낸다.
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::SyndicatePercentageData* packet = reinterpret_cast<UpdateClient::SyndicatePercentageData*>(rmsg->m_buf);

		int kPoint = this->getJewelPoint(SYNDICATE::eSYNDICATE_KAILUX);
		int dPoint = this->getJewelPoint(SYNDICATE::eSYNDICATE_DEALERMOON);

		packet->type = MSG_RVR;
		packet->subType = MSG_UPDATE_SYNDICATE_PERCENTAGE;
		packet->syndicateType = SYNDICATE::eSYNDICATE_KAILUX;
		packet->percentage = (float)kPoint / (kPoint + dPoint) * 100;

		rmsg->setSize(sizeof(UpdateClient::SyndicatePercentageData));
		SEND_Q(rmsg, dest);
	}
}

void SyndicateInfoDataManager::sendSyndicateInfo( CDescriptor* dest )
{
	if(dest == NULL)
	{
		//전체 유저에게?
	}
	else
	{
		if(dest->m_pChar->getSyndicateType() == SYNDICATE::eSYNDICATE_KAILUX)
		{
			char* kailuxKingName = NULL;
			kailuxKingName = SyndicateInfoDataManager::instance()->getKingName(SYNDICATE::eSYNDICATE_KAILUX);

			CNetMsg::SP rmsg(new CNetMsg);
			UpdateClient::RVRSyndicateInfoOfServer* packet = reinterpret_cast<UpdateClient::RVRSyndicateInfoOfServer*>(rmsg->m_buf);

			packet->type = MSG_RVR;
			packet->subType = MSG_RVR_SYNDICATE_INFO_SERVER;
			packet->syndicateType = SYNDICATE::eSYNDICATE_KAILUX;

			if(kailuxKingName != NULL)
			{
				strncpy(packet->kingName, kailuxKingName, MAX_CHAR_NAME_LENGTH);
				packet->kingName[MAX_CHAR_NAME_LENGTH] = '\0';
			}
			else
				memset(packet->kingName, 0x00, sizeof(packet->kingName));

			packet->skillIndex = 1756;
			packet->skillLevel = dest->m_pChar->m_syndicateManager.getSyndicateGrade(dest->m_pChar->getSyndicateType());

			rmsg->setSize(sizeof(UpdateClient::RVRSyndicateInfoOfServer));
			SEND_Q(rmsg, dest);
		}
		else if(dest->m_pChar->getSyndicateType() == SYNDICATE::eSYNDICATE_DEALERMOON)
		{
			char* DealerMoonKingName = NULL;
			DealerMoonKingName = SyndicateInfoDataManager::instance()->getKingName(SYNDICATE::eSYNDICATE_DEALERMOON);

			CNetMsg::SP rmsg(new CNetMsg);
			UpdateClient::RVRSyndicateInfoOfServer* packet = reinterpret_cast<UpdateClient::RVRSyndicateInfoOfServer*>(rmsg->m_buf);

			packet->type = MSG_RVR;
			packet->subType = MSG_RVR_SYNDICATE_INFO_SERVER;
			packet->syndicateType = SYNDICATE::eSYNDICATE_DEALERMOON;

			if(DealerMoonKingName != NULL)
			{
				strncpy(packet->kingName, DealerMoonKingName, MAX_CHAR_NAME_LENGTH);
				packet->kingName[MAX_CHAR_NAME_LENGTH] = '\0';
			}
			else
				memset(packet->kingName, 0x00, sizeof(packet->kingName));

			packet->skillIndex = 1757;
			packet->skillLevel = dest->m_pChar->m_syndicateManager.getSyndicateGrade(dest->m_pChar->getSyndicateType());

			rmsg->setSize(sizeof(UpdateClient::RVRSyndicateInfoOfServer));
			SEND_Q(rmsg, dest);
		}
	}
}

void SyndicateInfoDataManager::setKingPoint( int syndicateType, LONGLONG point )
{
	info_[syndicateType].kingPoint = point;
}

LONGLONG SyndicateInfoDataManager::getKingPoint( int syndicateType )
{
	return info_[syndicateType].kingPoint;
}

void SyndicateInfoDataManager::setKingIndex( int syndicateType, int charIndex )
{
	info_[syndicateType].kingCharIndex = charIndex;
}

int SyndicateInfoDataManager::getKingIndex( int syndicateType )
{
	return info_[syndicateType].kingCharIndex;
}
