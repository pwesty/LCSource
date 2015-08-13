#include "AutoBuffer.h"
#include "stdhdrs.h"
#include "Server.h"
#include "dofuncAdmin.h"

AutoBuffSystem::AutoBuffSystem() : m_bOn(true), lastTime(0), m_iLoopTimeSeconds(600),
m_bInit(false)
{
}

AutoBuffSystem::~AutoBuffSystem() {
}

bool AutoBuffSystem::Init() {
	
	CDBCmd DBBuffs;
	DBBuffs.Init(&gserver->m_dbdata);

	std::string sql = "SELECT * FROM t_autobuffs WHERE enabled=1";

	DBBuffs.SetQuery(sql);
	if (!DBBuffs.Open())
		return false;

	if (DBBuffs.m_nrecords == 0)
	{
		LOG_INFO("No Active Buffs Found");
		m_bOn = false;
		return false;
	}

	while(DBBuffs.MoveNext())
	{
		BuffData b;
		DBBuffs.GetRec("level", b.level);
		DBBuffs.GetRec("id", b.skillID);
		m_buffs.push_back(b);
	}

	m_bInit = true;

	return true;
}

void AutoBuffSystem::TurnOff() {
	m_bOn = false;
}

void AutoBuffSystem::TurnOn() {
	m_bOn = true;
}

void AutoBuffSystem::TickSystem() {
	if(!m_bOn)
		return;

	// Get the current time
	time_t now;
	time(&now);

	int dif = difftime(now, lastTime);
	
	if(dif < m_iLoopTimeSeconds)
		return;

	char* buffer = (char*)malloc(256);
	memset(buffer, 0, 256);

	std::vector<std::string> strs;

	for(int i = 0; i < m_buffs.size(); i++) {
		sprintf(buffer, "%d %d", m_buffs[i].skillID, m_buffs[i].level);
		do_GMGodBless(0, buffer, strs);
		memset(buffer, 0, 256);
	}

	free(buffer);

	lastTime = now;
}

void AutoBuffSystem::ReloadTable() {
	m_bOn = false;
	m_buffs.clear();
	Init();
	m_bOn = true;
}