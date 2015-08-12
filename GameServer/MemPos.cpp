#include "stdhdrs.h"

#include "Server.h"
#include "MemPos.h"
#include "gameserver_config.h"

CMemPosData::CMemPosData(int zone, float x, float z, char ylayer, const char* comment)
	: m_comment(MEMPOS_COMMENT_LENGTH + 1)
{
	m_zone = zone;
	m_x = (int)x;
	m_z = (int)z;
	m_ylayer = ylayer;
	m_comment = comment;
}

CMemPos::CMemPos()
{
	m_count = 0;
	memset(m_data, 0, sizeof(CMemPosData*) * MAX_MEMPOS);
}

CMemPos::~CMemPos()
{
	for (int i = 0; i < MAX_MEMPOS; i++)
	{
		if (m_data[i])
		{
			delete m_data[i];
			m_data[i] = NULL;
		}
	}
	m_count = 0;
}

bool CMemPos::Write(int slot, int zone, float x, float z, char ylayer, const char* comment)
{
	if (slot < 0 || slot >= MAX_MEMPOS)
		return false;

	if (m_data[slot])
	{
		// °»½Å
		m_data[slot]->m_zone = zone;
		m_data[slot]->m_x = (int)x;
		m_data[slot]->m_z = (int)z;
		m_data[slot]->m_ylayer = ylayer;
		m_data[slot]->m_comment = comment;
	}
	else
	{
		m_count++;
		m_data[slot] = new CMemPosData(zone, x, z, ylayer, comment);
	}

	return true;
}
