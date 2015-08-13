#include <boost/format.hpp>
#include "stdhdrs.h"

#include "Server.h"
#include "attendanceManager.h"
#include "Character.h"
#include "../ShareLib/DBCmd.h"
#include "DBManager.h"
#include "CmdMsg.h"
#include "SyndicateInfoDataManager.h"
#include "../ShareLib/packetType/ptype_attendance.h"

CAttendanceManager::CAttendanceManager( CPC* pc )
	: _owner(pc)
	, m_acc_count(0)
	, m_max_acc(0)
	, m_last_reward_point(0)
	, m_isAttendance(false)
	, m_isUseAssure(false)
	, m_isFirst(false)
	, m_sysMsg(0)
{
}

CAttendanceManager::~CAttendanceManager()
{

}

void CAttendanceManager::setAttendance( bool isAttendance )
{
	this->m_isAttendance = isAttendance;
}

bool CAttendanceManager::getAttendance()
{
	return this->m_isAttendance;
}

void CAttendanceManager::setUseAssure( bool isUseAssure )
{
	this->m_isUseAssure = isUseAssure;
}

bool CAttendanceManager::getUseAssure()
{
	return this->m_isUseAssure;
}

void CAttendanceManager::load(MYSQL* conn)
{
	CDBCmd cmd;
	cmd.Init(conn);
	std::string query = boost::str(boost::format("select * from t_attendance_exp_system where a_char_index = %d and a_last_date = CURDATE()") % _owner->m_index);
	cmd.SetQuery(query);

	//출석 했다면
	if(cmd.Open() == true && cmd.MoveNext() == true)
	{
		cmd.GetRec("a_acc_count", m_acc_count);
		cmd.GetRec("a_max_acc", m_max_acc);
		cmd.GetRec("a_last_reward_point", m_last_reward_point);

		setAttendance(true);
	}
	//출석하지 않았다면
	else
	{
		query = boost::str(boost::format("SELECT * FROM t_attendance_exp_system WHERE a_char_index = %d and a_last_date >=  CURDATE() - INTERVAL 1 DAY") % _owner->m_index);
		cmd.SetQuery(query);

		//출석하지 않았는데 전날 출석을 했다면
		if(cmd.Open() == true && cmd.MoveNext() == true)
		{
			cmd.GetRec("a_acc_count", m_acc_count);
			cmd.GetRec("a_max_acc", m_max_acc);
			cmd.GetRec("a_last_reward_point", m_last_reward_point);
		}
		//보험 아이템을 사용하고 있는 중이라면
		else if(m_isUseAssure == true)
		{
			query = boost::str(boost::format("SELECT * FROM t_attendance_exp_system where a_char_index = %d") % _owner->m_index);
			cmd.SetQuery(query);
			if(cmd.Open() == true && cmd.MoveNext() == true)
			{
				cmd.GetRec("a_acc_count", m_acc_count);
				cmd.GetRec("a_max_acc", m_max_acc);
				cmd.GetRec("a_last_reward_point", m_last_reward_point);
			}
		}
		//보험 아이템을 사용하지 않은 상태에서 출석하지 않았는데 전날 출석도 하지 않았다면 해당 유저의 출석 시스템을 모두 초기화
		else
		{
			query = boost::str(boost::format("SELECT a_acc_count, a_max_acc FROM t_attendance_exp_system WHERE a_char_index = %d") % _owner->m_index);
			cmd.SetQuery(query);
			if(cmd.Open() == true && cmd.MoveFirst() == true)
			{
				query = boost::str(boost::format("UPDATE t_attendance_exp_system SET a_acc_count = 0, a_max_acc = 0, a_last_reward_point = 0 WHERE a_char_index = %d") % _owner->m_index);
				cmd.SetQuery(query);
				cmd.Update();
			}
			else
			{
				m_isFirst = true;
			}
		}
	}
}

void CAttendanceManager::checkAttendance()
{
	//출석이 되지 않은 상태라면
	if(m_isAttendance == false && m_isUseAssure == false)
	{
		if( _owner->m_assist.FindBySkillIndex(1060) == 0)
		{
			bool bApply;
			CSkill * pSkill = gserver->m_skillProtoList.Create(1060,1);
			if( pSkill )
			{
				ApplySkill(_owner, _owner, pSkill, -1, bApply);
				if( !bApply && pSkill )
				{
					delete pSkill;
					pSkill = NULL;
				}
			}
		}
	}

	char	reward_exp[6] = { 0, 0 , 3, 4, 5, 10 };
	int		reward_idx = m_acc_count > 5 ? 5 : m_acc_count;
	_owner->m_nAttendance_exp = reward_exp[reward_idx];

	CNetMsg::SP rmsg(new CNetMsg);
	UpdateClient::makeAttendanceInfo(rmsg, m_acc_count, _owner->m_nAttendance_exp);
	SEND_Q(rmsg, _owner->m_desc);

	GAMELOG << init("ATTENDANCE EXP SYSTEM", _owner ) ;
	GAMELOG << m_acc_count << delim << reward_exp[reward_idx] << "%" << end;
}

void CAttendanceManager::finish()
{
	CDBCmd cmd;
	cmd.Init(&gserver->m_dbchar);

	m_acc_count++;

	if(m_acc_count > m_max_acc)
		m_max_acc = m_acc_count;
	//DB Thread에 던지기
	if(m_isFirst == true)
	{
		std::string query = boost::str(boost::format("INSERT INTO t_attendance_exp_system ( a_char_index, a_last_date, a_acc_count, a_last_reward_point ) VALUES ( %d, NOW(), %d, 0 )") % _owner->m_index % m_acc_count);
		DBManager::instance()->pushQuery(0, query);
	}
	else
	{
		std::string query = boost::str(boost::format("UPDATE t_attendance_exp_system SET a_last_date = NOW() , a_acc_count = %d, a_max_acc = %d WHERE a_char_index = %d") % m_acc_count % m_max_acc % _owner->m_index);
		DBManager::instance()->pushQuery(0, query);
	}
	//


	char	reward_exp[6] = { 0, 0 , 3, 4, 5, 10 };
	int		reward_idx = m_acc_count > 5 ? 5 : m_acc_count;

	CNetMsg::SP rmsg(new CNetMsg);
	UpdateClient::makeAttendanceFinish(rmsg, m_acc_count, reward_exp[reward_idx]);
	SEND_Q(rmsg, _owner->m_desc);
	
	_owner->m_nAttendance_exp = reward_exp[reward_idx];

	//출석 시 rvr 포인트도 준다.
	int JewelPoint_K = SyndicateInfoDataManager::instance()->getJewelPoint(SYNDICATE::eSYNDICATE_KAILUX);
	int JewelPoint_D = SyndicateInfoDataManager::instance()->getJewelPoint(SYNDICATE::eSYNDICATE_DEALERMOON);

	if(_owner->getSyndicateType() ==  SYNDICATE::eSYNDICATE_KAILUX)
	{
		if(JewelPoint_K > JewelPoint_D)
		{
			_owner->m_syndicateManager.increaseSyndicatePoint(10);
		}
		else
		{
			_owner->m_syndicateManager.increaseSyndicatePoint(100);
		}
	}
	else if(_owner->getSyndicateType() == SYNDICATE::eSYNDICATE_DEALERMOON)
	{
		if(JewelPoint_K > JewelPoint_D)
		{
			_owner->m_syndicateManager.increaseSyndicatePoint(100);
		}
		else
		{
			_owner->m_syndicateManager.increaseSyndicatePoint(10);
		}
	}

	GAMELOG << init("ATTENDANCE EXP SYSTEM", _owner ) ;
	GAMELOG << "SKILL CHECK" << delim;
	GAMELOG << m_acc_count << delim << reward_exp[reward_idx] << "%" << end;

	setAttendance(true);
}

void CAttendanceManager::setAccCount( int accCount )
{
	m_acc_count = accCount;
}

int CAttendanceManager::getAccCount()
{
	return this->m_acc_count;
}

void CAttendanceManager::setMaxAcc( int maxAcc )
{
	m_max_acc = maxAcc;
}

int CAttendanceManager::getMaxAcc()
{
	return this->m_max_acc;
}

void CAttendanceManager::setLastRewardPoint( int last_reward_point )
{
	m_last_reward_point = last_reward_point;
}

int CAttendanceManager::getLastRewardPoint()
{
	return this->m_last_reward_point;
}

void CAttendanceManager::reward()
{
	if( m_isFirst == false )
	{
		bool bReward = false;
		int nGiveItemCount = 0;
		for(int i = 0; i < 7; i++)
		{
			std::vector<RewardEventItem*> items;
			if (gserver->m_rewardItemList.getRewardItems( i, items ) == -1)
				continue;

			RewardEventItem *rewardItem = items[0];
			if(rewardItem->GetRewardDate() > m_last_reward_point && rewardItem->GetRewardDate() <= m_max_acc)
			{
				nGiveItemCount++;
			}
		}
		if(_owner->m_inventory.getEmptyCount() < nGiveItemCount)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeAttendanceRewardMsg(rmsg, ATTENDANCE_ERROR_INVEN_FULL);
			SEND_Q(rmsg, _owner->m_desc);
			return;
		}
		for(int i = 0 ; i < 7; i++ )
		{
			std::vector<RewardEventItem*> items;
			gserver->m_rewardItemList.getRewardItems( i, items );
			RewardEventItem *rewardItem = items[0];
			if(rewardItem->GetRewardDate() > m_last_reward_point && rewardItem->GetRewardDate() <= m_max_acc)
			{
				// 아이템 이벤확인 및 아이템 주기
				CItem * pItem = gserver->m_itemProtoList.CreateItem(
					rewardItem->GetIndex(),
					-1,
					rewardItem->GetPlus(),
					rewardItem->GetFlag(),
					rewardItem->GetCount()
					);

				if (_owner->m_inventory.addItem(pItem))
				{
					GAMELOG << init("ATTENDANCE EXP SYSTEM", _owner ) << "REWARD" << delim << m_last_reward_point << delim
						<< rewardItem->GetRewardDate() << "/" << m_max_acc << itemlog(pItem) << end;

					bReward = true;
					m_last_reward_point = rewardItem->GetRewardDate();
				}
				else // 실패
				{
					delete pItem;

					if ( bReward )
					{
						std::string query = boost::str(boost::format("UPDATE t_attendance_exp_system SET a_last_reward_point = %d WHERE a_char_index = %d") % m_last_reward_point % _owner->m_index);
						DBManager::instance()->pushQuery(0, query);
					}
					return;
				}
			}
		}
		/**
		* 버그 수정 ( 기존의 365일이 넘은 유저의 경우 리셋을 시키지 않았던 문제 수정.)
		* date : 2011.11.14
		*/
		if( m_max_acc >= 365 )
		{
			bReward = true;
		}

		if( bReward )
		{
			// 보상 아이템 지급일 기준 365일 이상일 경우 리셋 시킴.
			int nResetType = 0;
			if( 365 <= m_last_reward_point )
			{
				nResetType = 1;
			}
			//DB Thread에 던짐
			std::string query;
			if(nResetType == 1)
			{
				query = boost::str(boost::format("UPDATE t_attendance_exp_system SET a_acc_count = 0, a_max_acc = 0, a_last_reward_point = 0 WHERE a_char_index = %d ") % _owner->m_index);
			}
			else
			{
				query = boost::str(boost::format("UPDATE t_attendance_exp_system SET a_last_reward_point = %d WHERE a_char_index = %d") % m_last_reward_point % _owner->m_index);
			}
			DBManager::instance()->pushQuery(0, query);
		}
		else
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeAttendanceRewardMsg(rmsg, ATTENDANCE_ERROR_NO_REWARD);
			SEND_Q(rmsg, _owner->m_desc);
		}
	}
	else  // 받을 보상이 없음 DB 자체에 기록이 없는경우 subtype 3 - nAccCount:1 ( 받을보상이 없음 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeAttendanceRewardMsg(rmsg, ATTENDANCE_ERROR_NO_REWARD);
		SEND_Q(rmsg, _owner->m_desc);
	}
}

void CAttendanceManager::assureOk()
{
	if(m_isUseAssure == false)
	{
		LOG_ERROR("HACKING?. Do not use assureItem. charIndex[%d]", _owner->m_index);
		_owner->m_desc->Close("Do not use assureItem.");
		return;
	}

	m_isUseAssure = false;
	_owner->m_assist.CureBySkillIndex(1771);
	checkAttendance();

	std::string query;

	if(m_isAttendance == true)
	{
		query = boost::str(boost::format("UPDATE t_attendance_exp_system SET a_last_date = (CURDATE()) WHERE a_char_index = %d") % _owner->m_index);
	}
	else
	{
		query = boost::str(boost::format("UPDATE t_attendance_exp_system SET a_last_date = (CURDATE() - INTERVAL 1 DAY) WHERE a_char_index = %d") % _owner->m_index);
	}
	
	DBManager::instance()->pushQuery(0, query);

	m_sysMsg = ATTENDANCE_SYS_MSG_CASE1;
	sendSysMsg();
}

void CAttendanceManager::sendSysMsg()
{
	switch (m_sysMsg)
	{
	case NONE:
		break;
	case ATTENDANCE_SYS_MSG_CASE1:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			UpdateClient::makeAttendanceSysMsg(rmsg, ATTENDANCE_SYS_MSG_END_OK);
			SEND_Q(rmsg, _owner->m_desc);

			break;
		}
		
	case ATTENDANCE_SYS_MSG_CASE2:
		{			
			{
				CNetMsg::SP rmsg(new CNetMsg);
				UpdateClient::makeAttendanceSysMsg(rmsg, ATTENDANCE_SYS_MSG_TIME_END);
				SEND_Q(rmsg, _owner->m_desc);
			}
			
			{
				CNetMsg::SP rmsg(new CNetMsg);
				UpdateClient::makeAttendanceSysMsg(rmsg, ATTENDANCE_SYS_MSG_END_OK);
				SEND_Q(rmsg, _owner->m_desc);
			}
			break;
		}
		
	case ATTENDANCE_SYS_MSG_CASE3:
		{
			{
				CNetMsg::SP rmsg(new CNetMsg);
				UpdateClient::makeAttendanceSysMsg(rmsg, ATTENDANCE_SYS_MSG_TIME_END);
				SEND_Q(rmsg, _owner->m_desc);
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				UpdateClient::makeAttendanceSysMsg(rmsg, ATTENDANCE_SYS_MSG_TIME_END_INIT_ATTENDANCE);
				SEND_Q(rmsg, _owner->m_desc);

			}
			break;
		}
	}
}

void CAttendanceManager::setSysMsg( int sysMsg )
{
	m_sysMsg = sysMsg;
}

int CAttendanceManager::getSysMsg()
{
	return m_sysMsg;
}

