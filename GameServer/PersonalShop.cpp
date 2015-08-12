#include "stdhdrs.h"

#include "Server.h"
#include "PersonalShop.h"

CPersonalShop::CPersonalShop(char type, const char* name)
	: m_name(PS_MAX_SHOPNAME + 1)
{
	m_type = type;
	m_name = name;

	memset(m_normalitemindex, -1, sizeof(int) * PS_MAX_NORMAL_ITEM);
	memset(m_normalCount, 0, sizeof(LONGLONG) * PS_MAX_NORMAL_ITEM);
	memset(m_normalPrice, 0, sizeof(LONGLONG) * PS_MAX_NORMAL_ITEM);
	memset(m_packageitemindex, -1, sizeof(int) * PS_MAX_PACKAGE_ITEM);
	memset(m_packageCount, 0, sizeof(LONGLONG) * PS_MAX_PACKAGE_ITEM);
	m_packagePrice = 0;
}

bool CPersonalShop::AddItem(bool bNormal, int itemindex, LONGLONG count, LONGLONG price)
{
	if( count < 0 || price < 0 )
		return false;

	int i;

	if (bNormal)
	{
		for (i = 0; i < PS_MAX_NORMAL_ITEM; i++)
		{
			if (m_normalitemindex[i] == -1)
				break;
		}
		if (i == PS_MAX_NORMAL_ITEM)
			return false;
		m_normalitemindex[i] = itemindex;
		m_normalCount[i] = count;
		m_normalPrice[i] = price;
		return true;
	}

	else
	{
		for (i = 0; i < PS_MAX_PACKAGE_ITEM;  i++)
		{
			if (m_packageitemindex[i] == -1)
				break;
		}
		if (i == PS_MAX_PACKAGE_ITEM)
			return false;
		m_packageitemindex[i] = itemindex;
		m_packageCount[i] = count;
		m_packagePrice = price;
		return true;
	}
}

int CPersonalShop::FindItem(bool bNormal, int itemindex)
{
	int i;
	if (bNormal)
	{
		for (i = 0; i < PS_MAX_NORMAL_ITEM; i++)
		{
			if (m_normalitemindex[i] == itemindex)
				return i;
		}
		return -1;
	}
	else
	{
		for (i = 0; i < PS_MAX_PACKAGE_ITEM; i++)
		{
			if (m_packageitemindex[i] == itemindex)
				return i;
		}
		return -1;
	}
}

void CPersonalShop::RemoveNormalItem(int itemindex, LONGLONG count)
{
	int i;
	for (i = 0; i < PS_MAX_NORMAL_ITEM; i++)
	{
		if (m_normalitemindex[i] == itemindex)
		{
			m_normalCount[i] -= count;
			if (m_normalCount[i] <= 0)
			{
				m_normalitemindex[i] = -1;
				m_normalCount[i] = 0;
				m_normalPrice[i] = 0;
			}
			if (GetNormalCount() == 0 && GetPackageCount() == 0)
				m_type = PST_NOSHOP;
			return ;
		}
	}
}

void CPersonalShop::RemovePackage()
{
	memset(m_packageitemindex, -1, sizeof(int) * PS_MAX_PACKAGE_ITEM);
	memset(m_packageCount, 0, sizeof(LONGLONG) * PS_MAX_PACKAGE_ITEM);
	m_packagePrice = 0;
	if (GetNormalCount() == 0)
		m_type = PST_NOSHOP;
	else
		m_type = PST_SELL | PST_PREMIUM;
}

char CPersonalShop::GetNormalCount()
{
	char ret = 0;
	int i;
	for (i = 0; i < PS_MAX_NORMAL_ITEM; i++)
	{
		if (m_normalitemindex[i] != -1)
			ret++;
	}
	return ret;
}

int CPersonalShop::GetNextNormalItem(int pos)
{
	pos++;
	if (pos < 0)
		return -1;
	while (pos < PS_MAX_NORMAL_ITEM)
	{
		if (m_normalitemindex[pos] != -1)
			return pos;
		else
			pos++;
	}

	return -1;
}

char CPersonalShop::GetPackageCount()
{
	char ret = 0;
	int i;
	for (i = 0; i < PS_MAX_PACKAGE_ITEM; i++)
	{
		if (m_packageitemindex[i] != -1)
			ret++;
	}
	return ret;
}

int CPersonalShop::GetNextPackageItem(int pos)
{
	pos++;
	if (pos < 0)
		return -1;
	while (pos < PS_MAX_PACKAGE_ITEM)
	{
		if (m_packageitemindex[pos] != -1)
			return pos;
		else
			pos++;
	}

	return -1;
}
//