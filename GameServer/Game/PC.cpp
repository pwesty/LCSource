#include "../Addr.h"
#include "Character.h"

void CPC::SendStatus()
{
	return ((void(*)(CPC *))Addr::PC::SendStatus)(this);
}

void CPC::SendElementalStatus()
{
	return ((void(*)(CPC *))Addr::PC::SendElementalStatus)(this);
}

void CPC::SendInventory(char resultArrange)
{
	return ((void(*)(CPC *,char))Addr::PC::SendInventory)(this, resultArrange);
}

void CPC::SendQuickSlot()
{
	return ((void(*)(CPC *))Addr::PC::SendQuickSlot)(this);
}

void CPC::Send(CNetMsg& msg)
{
	return ((void(*)(CPC *, CNetMsg&))Addr::PC::Send)(this, msg);
}

//bool CPC::AddMoney(LONGLONG money)
//{
//	return ((bool(*)(CPC *, LONGLONG))Addr::PC::AddMoney)(this, money);
//}

bool CPC::AddExpSP(LONGLONG exp, int sp, bool bUseEvent, bool arg1, bool arg2)
{
	return ((bool(*)(CPC *, LONGLONG, int, bool, bool, bool))Addr::PC::AddExpSP)(this, exp, sp, bUseEvent, arg1, arg2);
}

void CPC::Restore()
{
	return ((void(*)(CPC *))Addr::PC::Restore)(this);
}

void CPC::InitStat()
{
	return ((void(*)(CPC *))Addr::PC::InitStat)(this);
}

/*
	Description
		create item and give it to pc
		
	Param
		IN		int			itemIndex		// item db index
		IN		int			plus			// item plus
		IN		int			flag			// item flag
		IN		LONGLONG	itemNum			// item count 
		IN		bool		ndrop			// can drop field
		IN		bool		nOtion			// can plus option

	Return
		type bool
		if item create and give success, return true, otherwise return false
*/
bool CPC::GiveItem(int itemIndex, int plus, int flag, LONGLONG itemNum, bool bdrop, bool bOption)
{
	return ((bool(*)(CPC *, int, int, int, LONGLONG, bool, bool))Addr::PC::GiveItem)(this, itemIndex, plus, flag, itemNum, bdrop, bOption);
}

void CPC::LevelUp(bool bSendEffect)
{
	return ((void(*)(CPC *, bool))Addr::PC::LevelUp)(this, bSendEffect);
}

void CPC::RemoveItemFromQuickSlot(CItem* item)
{
	return ((void(*)(CPC *,CItem*))Addr::PC::RemoveItemFromQuickSlot)(this,item);
}

void CPC::RemoveSkillFromQuickSlot(CSkill* pSkill)
{
	return ((void(*)(CPC *,CSkill*))Addr::PC::RemoveSkillFromQuickSlot)(this,pSkill);
}

void CPC::SetMaxWeight(bool bReset)
{
	return ((void(*)(CPC *,bool))Addr::PC::SetMaxWeight)(this,bReset);
}

bool CPC::CheckWeight(LONGLONG weight, bool bSendWarning, bool bCheckWeight, bool bCalcStatus)
{
	return ((bool(*)(CPC *, LONGLONG, bool, bool, bool))Addr::PC::CheckWeight)(this, weight, bSendWarning, bCheckWeight, bCalcStatus);
}

void CPC::RecoverPC()
{
	return ((void(*)(CPC *))Addr::PC::RecoverPC)(this);
}

void CPC::GoMemPos(int slot)
{
	return ((void(*)(CPC *, int))Addr::PC::GoMemPos)(this,slot);
}

void CPC::CalcStatus(bool bSend)
{
	return ((void(*)(CPC *,bool))Addr::PC::SendElementalStatus)(this,bSend);
}

bool CPC::UseStatPoint(MSG_STATPOINT_USE_TYPE type, int* val)
{
	return ((bool(*)(CPC *,MSG_STATPOINT_USE_TYPE, int*))Addr::PC::UseStatPoint)(this,type,val);
}

void CPC::ApplyItemValue(bool bSend)
{
	return ((void(*)(CPC *,bool))Addr::PC::ApplyItemValue)(this, bSend);
}

// 착용 아이템 중 무기, 방어구에 옵션을 붙일 수 있도록 선택
int CPC::SettingItemOption()
{
	return ((int(*)(CPC *))Addr::PC::SettingItemOption)(this);
}

int CPC::CanWarp()
{
	return ((int(*)(CPC *))Addr::PC::CanWarp)(this);
}

CSkill* CPC::FindSkill(int index)
{
	return ((CSkill*(*)(CPC *, int))Addr::PC::FindSkill)(this, index);
}

//void CPC::ApplyPassive()
//{
//	return ((void(*)(CPC *))Addr::PC::ApplyPassive)(this);
//}

//int CPC::GetAttackLevel() const
//{
//	return ((int(*)(const CPC *))Addr::PC::GetAttackLevel)(this);
//}

/*int CPC::GetDefenseLevel() const
{
	return ((int(*)(const CPC *))Addr::PC::GetDefenseLevel)(this);
}

MSG_DAMAGE_TYPE CPC::GetAttackType(const CSkillProto* proto) const
{
	return ((MSG_DAMAGE_TYPE(*)(const CPC *,const CSkillProto*))Addr::PC::SendElementalStatus)(this,proto);
}*/

int CPC::GetPKName()
{
	return ((int(*)(CPC *))Addr::PC::GetPKName)(this);
}

//bool CPC::CanMove()
//{
//	return ((bool(*)(CPC *))Addr::PC::CanMove)(this);
//}
//
//bool CPC::CanAttack()
//{
//	return ((bool(*)(CPC *))Addr::PC::CanAttack)(this);
//}
//
//bool CPC::CanSpell()
//{
//	return ((bool(*)(CPC *))Addr::PC::CanSpell)(this);
//}

void CPC::do_QuestGiveUp(CPC* ch, CQuest* quest)
{
	return ((void(*)(CPC *))Addr::PC::do_QuestGiveUp)(this);
}

//bool AddToInventory(CPC* ch, CItem* item, bool bSendWarning, bool bCheckWeight, int row, int col)
//{
//
//}
//
//bool AddToPreInventory(CPC* ch, CItem* item, bool bSendWarning, bool bCheckWeight, int tab, int row, int col)
//{
//
//}
//
//bool DecreaseFromInventory(CPC* ch, CItem* item, LONGLONG count)
//{
//
//}
//
//bool RemoveFromInventory(CPC* ch, int tab, int row, int col, bool bFreeMem, bool removeQuickSlot)
//{
//
//}
//
//bool RemoveFromInventory(CPC* ch, CItem* item, bool bFreeMem, bool removeQuickSlot)
//{
//
//}
//
////0608
//void CheckQuestCondition(CPC* ch, CItem* item, LONGLONG itemSum)
//{
//
//}

bool CPC::CheckDungeonData(int zone)
{
	return ((bool(*)(CPC *, int))Addr::PC::CheckDungeonData)(this, zone);
}

int CPC::GetJoinFlag(int zoneindex)
{
	return ((int(*)(CPC *, int))Addr::PC::GetJoinFlag)(this, zoneindex);
}

void CPC::SetJoinFlag(int zoneindex, int value)
{
	return ((void(*)(CPC *, int, int))Addr::PC::SetJoinFlag)(this, zoneindex, value);
}

bool CPC::InitSkill(CSkillList *skillList, bool isPassive)
{
	return ((bool(*)(CPC *,CSkillList *,bool))Addr::PC::InitSkill)(this,skillList,isPassive);
}

bool CPC::InitSSkill()
{
	return ((bool(*)(CPC *))Addr::PC::InitSSkill)(this);
}
bool CPC::IsCombatMode()
{
	return ((bool(*)(CPC *))Addr::PC::IsCombatMode)(this);
}

void CPC::UpdatePetValue()
{
	return ((void(*)(CPC *))Addr::PC::UpdatePetValue)(this);
}

CPet* CPC::GetPet(int index)
{
	return ((CPet*(*)(CPC *,int))Addr::PC::GetPet)(this,index);
}

void CPC::DelPet(int petIndex)
{
	return ((void(*)(CPC *,int))Addr::PC::DelPet)(this,petIndex);
}

bool CPC::CanPvP(CCharacter* target, bool bIgnoreInvisible)
{
	return ((bool(*)(CPC *,CCharacter*,bool))Addr::PC::CanPvP)(this,target, bIgnoreInvisible);
}

void CPC::CancelChange()
{
	return ((void(*)(CPC *))Addr::PC::CancelChange)(this);
}

CElemental* CPC::SummonElemental(char elementalType)
{
	return ((CElemental*(*)(CPC *, char))Addr::PC::SummonElemental)(this, elementalType);
}

void CPC::UnsummonElemental(CElemental* elemental)
{
	return ((void(*)(CPC *,CElemental*))Addr::PC::UnsummonElemental)(this, elemental);
}

bool CPC::CanSummonElemental(char newElementalType, char* duplicationType, int* nRemainTimeSummon, bool* errElementalType)
{
	return ((bool(*)(CPC *, char, char*, int*, bool*))Addr::PC::CanSummonElemental)(this,newElementalType,duplicationType,nRemainTimeSummon,errElementalType);
}

CElemental* CPC::GetElemental(int elementalindex)
{
	return ((CElemental*(*)(CPC *, int))Addr::PC::GetElemental)(this, elementalindex);
}

void CPC::Evocation(int type)
{
	return ((void(*)(CPC *, int))Addr::PC::Evocation)(this, type);
}

void CPC::Unevocation()
{
	return ((void(*)(CPC *))Addr::PC::Unevocation)(this);
}

int CPC::GetRemainEvocation(bool bSec)
{
	return ((int(*)(CPC *, bool))Addr::PC::GetRemainEvocation)(this, bSec);
}

void CPC::ProcDisconnect(bool bDisconnect, bool bGoto)
{
	return ((void(*)(CPC *, bool, bool))Addr::PC::ProcDisconnect)(this, bDisconnect, bGoto);
}

void CPC::OutputDBItemLog()
{
	return ((void(*)(CPC *))Addr::PC::OutputDBItemLog)(this);
}

void CPC::ChangeName(const char* name)
{
	return ((void(*)(CPC *, const char*))Addr::PC::ChangeName)(this, name);
}

bool CPC::AddBlockPC(int charIndex, const char* name)
{
	return ((bool(*)(CPC *,int, const char*))Addr::PC::AddBlockPC)(this, charIndex, name);
}

void CPC::ReleaseBlockPC(int charIndex, CLCString& name)
{
	return ((void(*)(CPC *, int, CLCString&))Addr::PC::ReleaseBlockPC)(this, charIndex, name);
}

bool CPC::IsBlockPC(int charIndex, const char* name)
{
	return ((bool(*)(CPC *, int, const char*))Addr::PC::IsBlockPC)(this, charIndex, name);
}

void CPC::GetBlockListString(CLCString& blockIndexList, CLCString& blockNameList)
{
	return ((void(*)(CPC *,CLCString&,CLCString&))Addr::PC::GetBlockListString)(this,blockIndexList,blockNameList);
}

//float CPC::GetHitrate(CCharacter* df, MSG_DAMAGE_TYPE type)
//{
//	return ((float(*)(CPC *, CCharacter*, MSG_DAMAGE_TYPE))Addr::PC::GetHitrate)(this, df, type);
//}
//
//float CPC::GetAvoid(CCharacter* of, MSG_DAMAGE_TYPE type)
//{
//	return ((float(*)(CPC *, CCharacter*, MSG_DAMAGE_TYPE))Addr::PC::GetAvoid)(this, of, type);
//}

void CPC::UpdateAPetValue(int vla)
{
	return ((void(*)(CPC *, int))Addr::PC::UpdateAPetValue)(this, vla);
}

CAPet* CPC::GetAPet(int index)
{
	return ((CAPet*(*)(CPC *, int))Addr::PC::GetAPet)(this, index);
}

void CPC::DelAPet(int petIndex)
{
	return ((void(*)(CPC *, int))Addr::PC::DelAPet)(this, petIndex);
}

void CPC::ApplyOriginOption(CItem * pItem)
{
	return ((void(*)(CPC *,CItem *))Addr::PC::ApplyOriginOption)(this,pItem);
}

void CPC::ApplySetItemValue(CItem * pItem)
{
	return ((void(*)(CPC *,CItem *))Addr::PC::ApplySetItemValue)(this,pItem);
}

void CPC::RemoveSetItemValue(CItem * pItem)
{
	return ((void(*)(CPC *,CItem *))Addr::PC::RemoveSetItemValue)(this,pItem);
}

void CPC::CountSetItem(CItem* pItem, bool bPlus /* = true */)
{
	return ((void(*)(CPC *,CItem*, bool))Addr::PC::CountSetItem)(this,pItem,bPlus);
}