#ifndef  __GAME_SHOP_H__
#define  __GAME_SHOP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCharacter;

class CShop  
{
public:
	// DB Info
	//CNPC*		m_keeper;
	int			m_keeperIdx;
	
	float		m_x;
	float		m_z;
	float		m_r;
	float		m_h;
	int			m_yLayer;

	int			m_sellRate;
	int			m_buyRate;
	int*		m_itemDBIdx;
	int			m_itemCount;

	CShop()
	{
		//m_keeper = NULL;
		m_keeperIdx = -1;
		m_x = m_z = m_r = m_h = 0.0f;
		m_yLayer = 0;
		m_sellRate = m_buyRate = 0;
		m_itemDBIdx = 0;
		m_itemCount = 0;
	}

	~CShop()
	{
		if (m_itemDBIdx)
			delete[] m_itemDBIdx;
		m_itemDBIdx = 0;
	}

	// shop 에서 아이템 있는지 확인
	bool ThersIsItem(int itemDBIdx)
	{
		int i;
		for(i=0; i < m_itemCount; i++)
		{
			if (m_itemDBIdx[i] == itemDBIdx)
				return true;
		}

		return false;
	}
};

#endif // end of define header
