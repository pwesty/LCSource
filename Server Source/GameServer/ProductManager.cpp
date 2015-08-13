#include "stdhdrs.h"

#include "ProductManager.h"
#include "Log.h"
#include "Server.h"
#include "../ShareLib/DBCmd.h"

CProductMgr::CProductMgr():
	m_count(0)
	, m_data(NULL)
{	}

CProductMgr::~CProductMgr()
{
	removeMemory();
}

void CProductMgr::removeMemory()
{
	if( m_data )
	{
		delete [] m_data;
		m_data = NULL;
	}
	m_count = 0;
}

int CProductMgr::setMemory(int _nCnt)
{
	if( _nCnt < 0 )
		return -1;

	removeMemory();
	m_count = _nCnt;
	m_data = new CProductData[m_count];
	return 1;
}

CProductData * CProductMgr::getData(int _index)
{
	if( !m_data )
		return NULL;
	int i=0;
	int count = getCount();
	for( i=0; i<count; i++ )
	{
		if( m_data[i].getIndex()==_index )
			return &m_data[i];
	}
	return NULL;
}

CProductItemData ** CProductMgr::getProductItemList(int _itemIndex)
{
	if( !m_data )
		return NULL;

	int i=0;
	int count = getCount();
	for( i=0; i<count; i++ )
	{
		if( m_data[i].getIndex()==_itemIndex )
			return m_data[i].getProductItemList();
	}
	return NULL;
}

int CProductMgr::load()
{
	removeMemory();

	CDBCmd cmd;
	cmd.Init(&gserver->m_dbdata);
	std::string select_product_query = "SELECT * FROM t_product ORDER BY a_index";
	cmd.SetQuery(select_product_query);
	cmd.Open();
	cmd.MoveFirst();
	if( cmd.m_nrecords < 1 )
		return -1;

	setMemory(cmd.m_nrecords);

	int index, delay;
	CLCString productIndex(128);
	CLCString productMini(128);
	CLCString productMaxi(128);
	CLCString productProb(128);

	int i=0;
	for(i=0; i<m_count; i++)
	{
		if( !cmd.GetRec("a_index",					index)
				|| !cmd.GetRec("a_delay_msec",			delay)
				|| !cmd.GetRec("a_product_item_idx",	productIndex)
				|| !cmd.GetRec("a_product_item_mini",	productMini)
				|| !cmd.GetRec("a_product_item_maxi",	productMaxi)
				|| !cmd.GetRec("a_product_item_prob",	productProb)
		  )
		{
			removeMemory();
			return -1;
		}

		m_data[i].setData(index, delay, productIndex, productMini, productMaxi, productProb);
		cmd.MoveNext();
	}
	return 1;
}
//