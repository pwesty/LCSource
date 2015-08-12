#include "stdh.h"

#include <Engine/Entities/EntityClass.h>
#include <Engine/Entities/EntityProperties.h>
#include <Common/header/EventTrade.h>
#include "TradeItem.h"

#include <Engine/Util.h>

std::vector<stEventTrade*> CTradeItem::m_vecTradeItem;
stEventTrade* CTradeItem::_data;

int CTradeItem::LoadTradeItemLod( const char* szFileName )
{
	FILE *fp		= NULL;
	if ((fp = fopen(szFileName, "rb")) == NULL) 
	{
		MessageBox(NULL, "File is not Exist.", "error!", MB_OK);
		return -1;
	}

	fflush(fp);

	int i, nMax = 0;
	int nLastIndex = 0;
	int nReadByte = 0;

	nReadByte = fread(&nLastIndex, sizeof(int), 1, fp);
	nReadByte = fread(&nMax, sizeof(int), 1, fp);

	ASSERT(nReadByte > 0 && "Invalid TradeItem Data");

	if (nMax <= 0)
	{
		fclose(fp);
		return 0;
	}

	_data = new stEventTrade[nMax];

	nReadByte = fread(_data, sizeof(stEventTrade), nMax, fp);

	fclose(fp);

	for (i = 0; i < nMax; ++i)
	{
		m_vecTradeItem.push_back(&_data[i]);
	}

	return nMax;
}

void CTradeItem::Destroy()
{
	SAFE_ARRAY_DELETE(_data);

	m_vecTradeItem.clear();
}
