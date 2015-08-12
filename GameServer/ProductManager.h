#ifndef _HEADER_CRAFTING_SYSTEM_PRODUCT_MANAGER_
#define _HEADER_CRAFTING_SYSTEM_PRODUCT_MANAGER_

#include "Product.h"

class CProductMgr
{
public:
	CProductMgr();
	~CProductMgr();

	int load();
	CProductData * getData(int _index);
	CProductItemData ** getProductItemList(int _index);

	int getCount()
	{
		return m_count;
	}

private:
	int  setMemory(int _nCnt);
	void removeMemory();

private:
	int m_count;
	CProductData * m_data;
};

#endif
//
