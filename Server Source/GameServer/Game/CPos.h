#ifndef __CPOSITION_H__
#define __CPOSITION_H__

class CPos
{
public:
	float	m_x;
	float	m_z;
	float	m_h;
	float	m_r;
	char	m_yLayer;

	CPos()
	{
		m_x = m_z = m_h = m_r = 0.0f;
		m_yLayer = 0;
	}

	CPos(float x, float z, float h, float r, int yLayer)
	{
		m_x = x;
		m_z = z;
		m_h = h;
		m_r = r;
		m_yLayer = yLayer;
	}

	CPos(const CPos& src)
	{
		m_x = src.m_x;
		m_z = src.m_z;
		m_h = src.m_h;
		m_r = src.m_r;
		m_yLayer = src.m_yLayer;
	}

	CPos& operator = (const CPos& src)
	{
		m_x = src.m_x;
		m_z = src.m_z;
		m_h = src.m_h;
		m_r = src.m_r;
		m_yLayer = src.m_yLayer;

		return *this;
	}
};

#endif