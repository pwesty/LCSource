#include "../Addr.h"
#include "Character.h"

void CCharacter::SetVisible()
{
	return ((void(*)(CCharacter *))Addr::Character::SetVisible)(this);
}

void CCharacter::Move(float arg1, float arg2, float arg3, float arg4, int arg5)
{
	return ((void(*)(CCharacter *,float, float, float, float, int))Addr::Character::Move)(this, arg1, arg2, arg3, arg4, arg5);
}

void CCharacter::AddEP(int arg1)
{
	return ((void(*)(CCharacter *, int))Addr::Character::AddEP)(this, arg1);
}

void CCharacter::SendDisappearAllInCell(bool bSendOtherDisappear)
{
	return ((void(*)(CCharacter *, bool))Addr::Character::SendDisappearAllInCell)(this, bSendOtherDisappear);
}

bool CCharacter::CanApplySkill(const CSkillProto *proto, const CSkillLevelProto *levelproto)
{
	return ((bool(*)(CCharacter *, const CSkillProto *, const CSkillLevelProto *))Addr::Character::CanApplySkill)(this, proto, levelproto);
}

bool CCharacter::CanMove()
{
	return ((bool(*)(CCharacter *))Addr::Character::CanMove)(this);
}

bool CCharacter::CanAttack()
{
	return ((bool(*)(CCharacter *))Addr::Character::CanAttack)(this);
}

bool CCharacter::CanSpell()
{
	return ((bool(*)(CCharacter *))Addr::Character::CanSpell)(this);
}

/*void ApplyAssistData(ASSISTVALUE* add, ASSISTVALUE* rate)
{

}*/

bool CCharacter::IsInPeaceZone(bool bBlockAsPeace)
{
	return ((bool(*)(CCharacter *, bool))Addr::Character::IsInPeaceZone)(this, bBlockAsPeace);
}

bool CCharacter::IsEnemy(CCharacter * tch)
{
	return ((bool(*)(CCharacter *, CCharacter *))Addr::Character::IsEnemy)(this, tch);
}

bool CCharacter::CanVisible(CCharacter * tch)
{
	return ((bool(*)(CCharacter *, CCharacter *))Addr::Character::CanVisible)(this, tch);
}

void CCharacter::CancelInvisible()
{
	return ((void(*)(CCharacter *))Addr::Character::CancelInvisible)(this);
}

//int CCharacter::GetMeleeLevelBonus()  const
//{
//	return ((int(*)(CCharacter *))Addr::Character::GetMeleeLevelBonus)(this);
//}
//
//int CCharacter::GetRangeLevelBonus()  const
//{
//	return ((int(*)(CCharacter *))Addr::Character::GetRangeLevelBonus)(this);
//}
//
//int CCharacter::GetMagicLevelBonus()  const
//{
//	return ((int(*)(CCharacter *))Addr::Character::GetMagicLevelBonus)(this);
//}

void CCharacter::CancelDamageLink()
{
	return ((void(*)(CCharacter *))Addr::Character::CancelDamageLink)(this);
}

unsigned char CCharacter::GetMapAttr()
{
	return ((unsigned char(*)(CCharacter *))Addr::Character::GetMapAttr)(this);
}

void CCharacter::ApplyPassiveSkill(CSkill *skill, int param)
{
	return ((void(*)(CCharacter *, CSkill *, int))Addr::Character::CancelDamageLink)(this,skill,param);
}

bool CCharacter::CheckHackAttack(CPC *pPC)
{
	return ((bool(*)(CCharacter *, CPC *))Addr::Character::CheckHackAttack)(this, pPC);
}

void CCharacter::SetDisableTime(int sec)
{
	return ((void(*)(CCharacter *, int))Addr::Character::SetDisableTime)(this, sec);
}

bool CCharacter::IsDisable()
{
	return ((bool(*)(CCharacter *))Addr::Character::IsDisable)(this);	
}

bool CCharacter::ChekAttackType()
{
	return ((bool(*)(CCharacter *))Addr::Character::ChekAttackType)(this);
}