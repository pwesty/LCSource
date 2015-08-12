#ifndef _HEADER_SYSTEM_CRAFTING_PRODUCTDATA_
#define _HEADER_SYSTEM_CRAFTING_PRODUCTDATA_

#define PRODUC_DATA_MAX 5

class CProductItemData
{
public:
	CProductItemData();
	~CProductItemData() {}

	int getIndex()
	{
		return m_index;
	}
	int getMinimum()
	{
		return m_minimum;
	}
	int getMaximum()
	{
		return m_maximum;
	}
	int getProb()
	{
		return m_prob;
	}

	void setData(int _index, int _mini, int _maxi, int _prob)
	{
		m_index = _index;
		m_minimum = _mini;
		m_maximum = _maxi;
		m_prob = _prob;
	}

private:
	int	m_index;
	int	m_minimum;
	int	m_maximum;
	int m_prob;
};

class CProductData
{
public:
	CProductData();
	CProductData(int _index, int _delayMSec, const char * _strIndex, const char * _strMini, const char * _strMaxi, const char * _strProb);
	~CProductData();

	int getIndex()
	{
		return m_index;
	}
	int getDelayMSec()
	{
		return m_delayMSec;
	}
	int getCount()
	{
		return m_count;
	}
	CProductItemData ** getProductItemList()
	{
		return ( CProductItemData **)&m_productItem;
	}
	CProductItemData * getProductItem(int _pos);

	int  setData(int _index, int _delayMSec, const char * _strIndex, const char * _strMini, const char * _strMaxi, const char * _strProb);
	int  setProductData(const char * _strIndex, const char * _strMini, const char * _strMaxi, const char * _strProb);
	int  setProductItemData(int _pos, int _index, int _mini, int _maxi, int _prob);
	void setDelayMSec(int _var)
	{
		m_delayMSec = _var;
	}

private:
	int  getMaxCount()
	{
		return PRODUC_DATA_MAX;
	}
	void setMemory(int _nCnt);
	void removeMemory();

private:
	int	m_index;
	int m_delayMSec;
	int m_count;
	CProductItemData * m_productItem;
};
#endif
//
