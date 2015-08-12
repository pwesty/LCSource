#include "../Addr.h"
#include "Zone.h"

CZone::CZone()
{
	((void(*)())Addr::CZone::Constructor)();
}
CZone::~CZone()
{
	((void(*)())Addr::CZone::DeConstructor)();
}
bool CZone::LoadZone(int ZoneID)
{
	return ((bool(*)(CZone*, int))Addr::CZone::LoadZone)(this, ZoneID);
}
bool CZone::LoadNPC(void)
{
	return ((bool(*)(CZone*))Addr::CZone::LoadNPC)(this);
}
bool CZone::LoadShop(void)
{
	return ((bool(*)(CZone*))Addr::CZone::LoadShop)(this);
}
CShop * CZone::FindShop(int ShopID)
{
	return ((CShop *(*)(CZone*, int))Addr::CZone::FindShop)(this, ShopID);
}
void CZone::ChangeWeather(void)
{
	return ((void(*)(CZone*))Addr::CZone::ChangeWeather)(this);
}
void CZone::ChangeTaxRate(int Tax1, int Tax2)
{
	return ((void(*)(CZone*,int,int))Addr::CZone::ChangeTaxRate)(this, Tax1, Tax2);
}
int CZone::FindEmptyArea(void)
{
	return ((int(*)(CZone*))Addr::CZone::FindEmptyArea)(this);
}
int CZone::SetEnableArea(void)
{
	return ((int(*)(CZone*))Addr::CZone::SetEnableArea)(this);
}
void CZone::SetDisableArea(int AreaID)
{
	return ((void(*)(CZone*, int))Addr::CZone::SetDisableArea)(this, AreaID);
}
bool CZone::IsPersonalDungeon(void)
{
	return ((bool(*)(CZone*))Addr::CZone::IsPersonalDungeon)(this);
}
bool CZone::IsPartyRecallZone(void)
{
	return ((bool(*)(CZone*))Addr::CZone::IsPartyRecallZone)(this);
}
bool CZone::IsSummonZone(void)
{
	return ((bool(*)(CZone*))Addr::CZone::IsSummonZone)(this);
}
bool CZone::IsGuildRoom(void)
{
	return ((bool(*)(CZone*))Addr::CZone::IsGuildRoom)(this);
}
bool CZone::IsOXQuizRoom(void)
{
	return ((bool(*)(CZone*))Addr::CZone::IsOXQuizRoom)(this);
}
bool CZone::IsComboZone(void)
{
	return ((bool(*)(CZone*))Addr::CZone::IsComboZone)(this);
}
bool CZone::IsExtremeCube(void)
{
	return ((bool(*)(CZone*))Addr::CZone::IsExtremeCube)(this);
}
bool CZone::IsPkZone(void)
{
	return ((bool(*)(CZone*))Addr::CZone::IsExtremeCube)(this);
}
bool CZone::IsWarGroundZone(void)
{
	return ((bool(*)(CZone*))Addr::CZone::IsExtremeCube)(this);
}
bool CZone::IsPersonalRaid(void)
{
	return ((bool(*)(CZone*))Addr::CZone::IsExtremeCube)(this);
}
bool CZone::IsExpedRaidZone(void)
{
	return ((bool(*)(CZone*))Addr::CZone::IsExtremeCube)(this);
}
bool CZone::IsPartyRaidZone(void)
{
	return ((bool(*)(CZone*))Addr::CZone::IsPartyRaidZone)(this);
}
bool CZone::IsWarZone(int a, int b)
{
	return ((bool(*)(CZone*, int, int))Addr::CZone::IsWarZone)(this,a,b);
}
int CZone::FindComboArea(int a)
{
	return ((bool(*)(CZone*, int))Addr::CZone::FindComboArea)(this,a);
}
int CZone::GetExtra(int a, int b)
{
	return ((bool(*)(CZone*, int, int))Addr::CZone::GetExtra)(this,a,b);
}
bool CZone::Load_NpcRegenRaidList(void)
{
	return ((bool(*)(CZone*))Addr::CZone::Load_NpcRegenRaidList)(this);
}
bool CZone::IsFieldZone(void)
{
	return ((bool(*)(CZone*))Addr::CZone::IsFieldZone)(this);
}