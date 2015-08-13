#include "stdhdrs.h"

#include "Product.h"

CProductItemData::CProductItemData() :
	m_index(-1)
	, m_minimum(-1)
	, m_maximum(-1)
	, m_prob(-1)
{	}

CProductData::CProductData() :
	m_index(-1)
	, m_delayMSec(-1)
	, m_count(-1)
	, m_productItem(NULL)
{	}

CProductData::CProductData(int _index, int _delayMSec, const char * _strIndex, const char * _strMini, const char * _strMaxi, const char * _strProb)
{
	setData( _index, _delayMSec, _strIndex, _strMini, _strMaxi, _strProb);
}

CProductData::~CProductData()
{
	removeMemory();
}
void CProductData::setMemory(int _nCnt)
{
	removeMemory();
	m_count = _nCnt;
	m_productItem = new CProductItemData[m_count];
}

void CProductData::removeMemory()
{
	if( m_productItem )
	{
		delete [] m_productItem;
		m_productItem =NULL;
	}
	m_count = 0;
}

CProductItemData * CProductData::getProductItem(int _pos)
{
	if( !m_productItem || _pos < 0 || _pos >= getCount() )
		return NULL;

	return &m_productItem[_pos];
}

int  CProductData::setData(int _index, int _delayMSec, const char * _strIndex, const char * _strMini, const char * _strMaxi, const char * _strProb)
{
	if( setProductData(_strIndex, _strMini, _strMaxi, _strProb) < 0 )
		return -1;

	m_index = _index;
	m_delayMSec = _delayMSec;
	return 1;
}

int  CProductData::setProductData(const char * _strIndex, const char * _strMini, const char * _strMaxi, const char * _strProb)
{
	if( !_strIndex || !_strMini || !_strMaxi || !_strProb )
		return -1;

	const char * argIndex = _strIndex;
	const char * argMini  = _strMini;
	const char * argMaxi  = _strMaxi;
	const char * argProb  = _strProb;

	char strIndex[64];
	char strMini[64];
	char strMaxi[64];
	char strProb[64];
	int nIndex[PRODUC_DATA_MAX];
	int nMini[PRODUC_DATA_MAX];
	int nMaxi[PRODUC_DATA_MAX];
	int nProb[PRODUC_DATA_MAX];
	int index;
	int mini;
	int maxi;
	int prob;

	m_count = 0;

	int i=0;
	int count = getMaxCount();
	for(i=0; i<count; i++)
	{
		argIndex = AnyOneArg(argIndex, strIndex);
		argMini  = AnyOneArg(argMini,  strMini);
		argMaxi  = AnyOneArg(argMaxi,  strMaxi);
		argProb  = AnyOneArg(argProb,  strProb);

		index = atoi(strIndex);
		mini  = atoi(strMini);
		maxi  = atoi(strMaxi);
		prob  = atoi(strProb);

		if( index > 0 && prob > 0 && mini >= 0 && maxi >= mini )
		{
			nIndex[m_count] = index;
			nMini[m_count]  = mini;
			nMaxi[m_count]  = maxi;
			nProb[m_count]  = prob;
			m_count++;
		}
	}

	if( m_count > 0 )
	{
		setMemory(m_count);
		for(i=0; i<m_count; i++)
		{
			if( setProductItemData(i, nIndex[i], nMini[i], nMaxi[i], nProb[i] ) < 0 )
			{
				removeMemory();
				return -1;
			}
		}
	}
	return 1;
}

int CProductData::setProductItemData(int _pos, int _index, int _mini, int _maxi, int _prob)
{
	if( !m_productItem || _pos < 0 || _pos >= m_count )
		return -1;

	m_productItem[_pos].setData(_index, _mini, _maxi, _prob);
	return 1;
}
//