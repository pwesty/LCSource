#include <boost/format.hpp>
#include "stdhdrs.h"

#ifdef PREMIUM_CHAR

#include "../ShareLib/DBCmd.h"
#include "../ShareLib/packetType/ptype_appear.h"
#include "../ShareLib/packetType/ptype_premium_char.h"
#include "Descriptor.h"
#include "Character.h"
#include "DBManager.h"
#include "Server.h"
#include "PremiumChar.h"

int PremiumChar::resetJumpCountTime = 0;

PremiumChar::PremiumChar(CPC* owner)
	: isActive_(false)
	, owner_(owner)
	, premiumType_(PREMIUM_CHAR_TYPE_FIRST)
	, expireTime_(0)
	, jumpCount_(0)
	, jumpTime_(0)
{

}

PremiumChar::~PremiumChar()
{

}

void PremiumChar::load( MYSQL* mysql_conn )
{
	CDBCmd dbChar;
	dbChar.Init(mysql_conn);

	std::string query = boost::str(boost::format(
		"SELECT  a_premiumType, a_expireTime, a_jumpCount, a_jumpTime "
		"FROM t_premiumchar WHERE a_charIndex = %1% LIMIT 1") % owner_->m_index);
	dbChar.SetQuery(query);
	if (dbChar.Open() == false)
	{
		this->reset();
		return;
	}

	if (dbChar.m_nrecords <= 0)
	{
		// 데이터가 없는 경우
		query = boost::str(boost::format(
			"INSERT INTO t_premiumchar(a_charIndex, a_premiumType, a_expireTime, a_jumpCount, a_jumpTime)"
			"VALUES(%1%, 0, 0, 0, 0)") % owner_->m_index);
		dbChar.SetQuery(query);
		dbChar.Update();

		this->reset();
		return;
	}

	// 데이터 읽기
	dbChar.MoveNext();
	dbChar.GetRec("a_premiumType", this->premiumType_);
	dbChar.GetRec("a_expireTime", this->expireTime_);
	dbChar.GetRec("a_jumpCount", this->jumpCount_);
	dbChar.GetRec("a_jumpTime", this->jumpTime_);

	if (this->expireTime_ == 0)
	{
		// 아이템을 사용하지 않은 경우
		this->reset();
	}
	else
	{
		this->isActive_ = true;

		// 중요 : 로딩시 expire 검사는 Server::CharPrePlay()에서 정보를 내려준 후 바로 검사
	}
}

void PremiumChar::setExpireTime( int t )
{
	expireTime_ = t;
}

void PremiumChar::setJumpCount( int c )
{
	jumpCount_ = c;

	if (jumpCount_ == 0)
	{
		jumpTime_ = 0;
	}
	else
	{
		jumpTime_ = gserver->getNowSecond();
	}

	CNetMsg::SP rmsg(new CNetMsg);
	UpdateClient::makePremiumCharJumpCount(rmsg, jumpCount_);
	SEND_Q(rmsg, owner_->m_desc);
}

void PremiumChar::sendInfo()
{
	CNetMsg::SP rmsg(new CNetMsg);
	UpdateClient::makePremiumCharInfo(rmsg, premiumType_, expireTime_, jumpCount_);
	SEND_Q(rmsg, owner_->m_desc);
}

void PremiumChar::sendEnd()
{
	CNetMsg::SP rmsg(new CNetMsg);
	UpdateClient::makePremiumCharEnd(rmsg);
	SEND_Q(rmsg, owner_->m_desc);

	owner_->SendStatus();
}

// 매 초마다 체크하는 부분
void PremiumChar::checkExpireTime( int chk_t )
{
	if (expireTime_ == 0)
		return;

	if (expireTime_ >= chk_t)
		return;

	// 로그를 남기는 작업이므로, 메모리 데이터를 초기화(reset()) 하기 이전에 호출할것
	this->saveResetLog();

	// 시간 만료
	this->reset();
	this->sendEnd();
	this->saveNow();
}

void PremiumChar::reset()
{
	isActive_ = false;

	premiumType_ = PREMIUM_CHAR_TYPE_NONE;
	expireTime_ = 0;
	jumpCount_ = 0;
	jumpTime_ = 0;

	// 지역의 다른 캐릭터에게 전달
	CNetMsg::SP rmsg(new CNetMsg);
	UpdateClient::makeChangePremiumCharFlag(rmsg, owner_->m_index, this->isActive_);
	owner_->m_pZone->m_area->SendToAllClient(rmsg);
}

void PremiumChar::save( std::vector<std::string>& vec )
{
	std::string query;
	this->getSaveQuery(query);

	vec.push_back(query);
}

void PremiumChar::getSaveQuery( std::string& query )
{
	query = boost::str(boost::format(
		"UPDATE t_premiumchar SET "
		"a_premiumType = %1%,"
		"a_expireTime = %2%,"
		"a_jumpCount = %3%,"
		"a_jumpTime = %4% WHERE a_charIndex = %5%")
		% this->premiumType_
		% this->expireTime_
		% this->jumpCount_
		% this->jumpTime_
		% owner_->m_index);
}

void PremiumChar::saveNow()
{
	std::string query;
	this->getSaveQuery(query);

	DBManager::instance()->pushQuery(owner_->m_desc->m_index, query);
}

void PremiumChar::saveSetLog()
{
	// DB에 로그 남기기
	std::string query = boost::str(boost::format(
		"INSERT INTO t_premiumchar_log VALUES(%1%, %2%, %3%, %4%, %5%)")
		% owner_->m_index
		% this->getPremiumType()
		% gserver->getNowSecond()
		% this->getExpireTime()
		% 0);
	DBManager::instance()->pushQuery(owner_->m_desc->m_index, query);

	LOG_INFO("PREMIUM_CHAR_SET - charIndex : %d : type : %d : start : %d : end : %d",
		owner_->m_index,
		this->getPremiumType(),
		gserver->getNowSecond(),
		this->getExpireTime());

}

void PremiumChar::saveResetLog()
{
	// DB에 로그 남기기
	std::string query = boost::str(boost::format(
		"INSERT INTO t_premiumchar_log VALUES(%1%, %2%, %3%, %4%, %5%)")
		% owner_->m_index
		% this->getPremiumType()
		% 0
		% this->getExpireTime()
		% gserver->getNowSecond());
	DBManager::instance()->pushQuery(owner_->m_desc->m_index, query);

	LOG_INFO("PREMIUM_CHAR_RESET - charIndex : %d : type : %d : end : %d : resettime : %d",
		owner_->m_index,
		this->getPremiumType(),
		this->getExpireTime(),
		gserver->getNowSecond());
}

void PremiumChar::checkJumpCount()
{
	if (this->jumpTime_ <= this->resetJumpCountTime)
		this->setJumpCount(0);
}

void PremiumChar::setActive()
{
	isActive_ = true;

	// 지역의 다른 캐릭터에게 전달
	CNetMsg::SP rmsg(new CNetMsg);
	UpdateClient::makeChangePremiumCharFlag(rmsg, owner_->m_index, this->isActive_);
	owner_->m_pZone->m_area->SendToAllClient(rmsg);
}

#endif
