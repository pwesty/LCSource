#ifndef __MEMPOS1_H__
#define __MEMPOS1_H__

class CMemPosData
{
public:
	int		m_zone;
	float	m_x;
	float	m_z;
	char	m_ylayer;
	CLCString m_comment;

	CMemPosData(int zone, float x, float z, char ylayer, const char* comment);
};

class CMemPos
{
public:
	int				m_count;
	CMemPosData*	m_data[MAX_MEMPOS];

	CMemPos();
	~CMemPos();

	bool Write(int slot, int zone, float x, float z, char ylayer, const char* comment);
};

#endif

