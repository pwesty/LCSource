#include "stdhdrs.h"

#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "Exp.h"
#include "doFunc.h"
#include "Battle.h"
#include "WarCastle.h"
#include "Log.h"
#include "../ShareLib/packetType/ptype_old_do_skill.h"
#include "../ShareLib/packetType/ptype_old_extend.h"

extern void DelAttackList(CCharacter* ch);
extern int levelup_hp[JOBCOUNT][4];
extern int levelup_mp[JOBCOUNT][4];

////////////////////
// CCharacter member
CCharacter::CCharacter()
	: m_name(MAX_CHAR_NAME_LENGTH + 1)
{
	m_type = MSG_CHAR_UNKNOWN;

	m_index = -1;
	m_level = 0;
	m_exp = 0;
	m_skillPoint = 0;

	m_maxHP = 0;
	m_hp = 0;
	m_maxMP = 0;
	m_mp = 0;

	m_recoverHP = 0;
	m_recoverMP = 0;
	m_attackRange = 0.0f;

	m_str = 0;
	m_dex = 0;
	m_int = 0;
	m_con = 0;

	m_eqMelee = 0;
	m_eqRange = 0;
	m_eqMagic = 0;
	m_eqDefense = 0;
	m_eqResist = 0;

	m_attackSpeed = 0;
	m_magicSpeed = 0;
	m_skillSpeed = 0;
	m_statPall = 0;
	// << 071226 kjban add
	m_statPall_per = 0;
	// >>

	m_walkSpeed = 0.0f;
	m_runSpeed = 0.0f;
	m_recoverPulse = 0;

	m_pos = CPos(0, 0, 0, 0, 0);

	m_cellX = NO_CELL;
	m_cellZ = NO_CELL;

	m_assist.Init(this);

	m_attackList = NULL;

	m_pCellPrev = NULL;
	m_pCellNext = NULL;
	m_pZone = NULL;
	m_pArea = NULL;

	m_bVisible = true;

	m_currentSkill = NULL;
	m_linkSource = NULL;
	m_linkTarget = NULL;

	m_artiStr = 0;
	m_artiDex = 0;
	m_artiInt = 0;
	m_artiCon = 0;
	m_artiExp = 0;
	m_artiGold = 0;
	
	m_opStr = 0;
	m_opDex = 0;
	m_opInt = 0;
	m_opCon = 0;

	m_opMelee = 0;
	m_opRange = 0;
	m_opMeleeHitRate = 0;
	m_opRangeHitRate =0;

	m_opDMelee = 0;
	m_opDRange = 0;
	m_opMeleeAvoid = 0;
	m_opRangeAvoid =0;

	m_opMagic = 0;
	m_opMagicHitRate = 0;
	m_opResist = 0;
	m_opResistAvoid = 0;

	m_opExpRate = 0;
	m_opSPRate = 0;

	m_opAttratt = 0;
	m_opAttrdef = 0;

	memset(&m_avPassiveAddition, 0, sizeof(m_avPassiveAddition));
	memset(&m_avPassiveRate, 0, sizeof(m_avPassiveRate));

	m_hackAttackCount = PULSE_HACK_ATTACK_COUNT;	// 어택메세지 온 수 저장
	m_AttackServerTime = 0;	// 공속에 따른 최대 어택 시간
	m_AttackClientTime = 0; // 클라이언트가 보낸 어택시간
	m_lastAttackPulse = 0;

	m_pulseDisable = 0;
#ifdef NEW_ACCERY_ADD
	m_AddProb	= 0;
#endif // NEW_ACCERY_ADD
	cooltime_2142 = 0;			// 건강의 물약
	cooltime_2143 = 0;			// 지력의 물약
	MobScroll = 0;

	m_cooltime_Competition = 0;	// 대전용 아이템

	m_nFinalHitrate = 0;
	m_nFinalAvoidrate = 0;

	m_flySpeed = 15.0f;		// 날기 속도
	m_attacktype = ATTACK_TYPE_NORMAL;
	m_attackcount = 0;

	// 원정대 표식
	m_nExpedLabel = -1;

	m_ep = 0;
	m_maxEP = 350000;
	m_skillCooltime = 0; // 0 ~ 100%
#ifdef BUGFIX_DEAD_AGAIN_DEAD
	m_deadTime = 0;
#endif
	m_decreaseDBufTimeRate = 0;
	m_tradeAgentRegCount = 0;

	m_syndicateType = 0;
}

CCharacter::~CCharacter()
{
	m_type = MSG_CHAR_UNKNOWN;
	m_index = -1;
	DelAttackList(this);
}

void CCharacter::SetVisible()
{
	if (m_bVisible)
	{
		// 보이는 상태이면
		// 공격 받지 않게 하고
		DelAttackList(this);
		{
			// 주변에 유저에게 사라진다고 알리고
			CNetMsg::SP rmsg(new CNetMsg);
			DisappearMsg(rmsg, this);
			m_pArea->SendToCell(rmsg, this);
		}
		// 상태 토글
		m_bVisible = false;
		if (IS_PC(this))
			TO_PC(this)->m_bImmortal = true;
	}
	else
	{
		// 안보이는 상태이면
		// 상태 토글
		m_bVisible = !m_bVisible;
		TO_PC(this)->m_bImmortal = false;
		{
			// 주변 유저에게 나타남을 알리고
			CNetMsg::SP rmsg(new CNetMsg);
			AppearMsg(rmsg, this, true);
			m_pArea->SendToCell(rmsg, this);
		}

		if( TO_PC(this)->m_guildInfo && TO_PC(this)->m_guildInfo->guild() )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildPointRankingMsg( rmsg, TO_PC(this)->m_index, TO_PC(this)->m_guildInfo->guild()->index(), TO_PC(this)->m_guildInfo->guild()->GetGuildPointRanking() );
			m_pArea->SendToCell( rmsg, this );
		}
		if( TO_PC(this) )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildNameColorStateMsg(rmsg, TO_PC(this) );
			m_pArea->SendToCell( rmsg, this );
		}
	}
}

void CCharacter::SendDisappearAllInCell(bool bSendOtherDisappear)
{
	{
		CNetMsg::SP rmsg(new CNetMsg);
		// 타캐릭터에게 사라짐 알림
		DisappearMsg(rmsg, this);
		if (IS_PC(this))
			m_pArea->SendToCell(rmsg, this);
		else
			m_pArea->SendToCell(rmsg, this, true);
	}

	if (!IS_PC(this))
		return ;

	if (!bSendOtherDisappear)
		return ;

	CPC* pc = TO_PC(this);

	CNetMsg::SP rmsg(new CNetMsg);
	DisappearAllMsg(rmsg);
	SEND_Q(rmsg, pc->m_desc);
}

void CCharacter::SendRemoveAllInCellForClientEffect(CPC* owner)
{
	CNetMsg::SP rmsg(new CNetMsg);
	DisappearMsgForClientEffect(rmsg, this, owner->m_index);
	m_pArea->SendToCell(rmsg, this, true);
}

bool CCharacter::CanApplySkill(const CSkillProto* proto, const CSkillLevelProto* levelproto)
{
	if (proto == NULL)
		return false;
	if (levelproto == NULL)
		return false;

	// 적용조건 검사 : state
	if ((proto->m_applyState & SCT_DEATH) && !DEAD(this))
		return false;
	if (!(proto->m_applyState & SCT_DEATH) && DEAD(this))
		return false;

	if (IS_PC(this))
	{
		CPC* pc = TO_PC(this);
		if (proto->m_applyState & SCT_WEAPON)
		{
			if (pc->m_wearInventory.wearItemInfo[WEARING_WEAPON] == NULL)
				return false;
			if (!pc->m_wearInventory.wearItemInfo[WEARING_WEAPON]->IsWeaponType())
				return false;
			int sub = pc->m_wearInventory.wearItemInfo[WEARING_WEAPON]->m_itemProto->getItemSubTypeIdx();
			if ((proto->m_applyWeaponType0 >= 0 || proto->m_applyWeaponType1 >= 0) && (sub != proto->m_applyWeaponType0 && sub != proto->m_applyWeaponType1))
				return false;
		}
		if (proto->m_applyState & SCT_WEAPON && pc->m_wearInventory.wearItemInfo[WEARING_WEAPON] == NULL)
			return false;
		if (!(proto->m_applyState & SCT_STAND) || !(proto->m_applyState & SCT_SITDOWN))
		{
			// 둘다 꺼져 있는 경우는 둘다 켜져 있는 경우와 동일
			if ((proto->m_applyState & SCT_SITDOWN) || (proto->m_applyState & SCT_STAND))
			{
				if (proto->m_applyState & SCT_SITDOWN && !pc->IsSetPlayerState(PLAYER_STATE_SITDOWN))
					return false;
				if (proto->m_applyState & SCT_STAND && pc->IsSetPlayerState(PLAYER_STATE_SITDOWN))
					return false;
			}
		}
		if (proto->m_applyState & SCT_SHIELD && pc->m_wearInventory.wearItemInfo[WEARING_SHIELD] == NULL)
			return false;

		if (proto->m_index == 433	// 건강의 물약
				&& cooltime_2142 > gserver->getNowSecond())
		{
			return false;
		}

		if (proto->m_index == 434	// 지력의 물약
				&& cooltime_2143 > gserver->getNowSecond())
		{
			return false;
		}

		if( proto->m_index == 465 // 광속
				&& m_cooltime_Competition > gserver->getNowSecond() )
		{
			return false;
		}
	}

	if ((proto->m_applyState & SCT_PEACEZONE) && IsInPeaceZone(false))
		return false;

	// 적용조건 검사 : 효과
	int i;
	for (i = 0; i < MAX_SKILL_USE_MAGIC; i++)
	{
		if (levelproto->m_appMagicIndex[i] < 0)
			continue ;
		if (!m_assist.Find(levelproto->m_appMagicIndex[i], levelproto->m_appMagicLevel[i]))
			return false;
	}

	return true;
}

bool CCharacter::CanMove()
{
	if (m_assist.m_state & AST_STONE)
		return false;
	if (m_assist.m_state & AST_HOLD)
		return false;
	if (m_assist.m_state & AST_STURN)
		return false;
	if (m_assist.m_state & AST_SLEEP)
		return false;
	if (m_assist.m_state & AST_FREEZE)
		return false;
	return true;
}

bool CCharacter::CanAttack()
{
	if (m_assist.m_state & AST_STONE)
		return false;
	if (m_assist.m_state & AST_STURN)
		return false;
	if (m_assist.m_state & AST_SLEEP)
		return false;
	if (m_assist.m_state & AST_FAKEDEATH)
		return false;
	if (m_assist.m_state & AST_FREEZE)
		return false;
	if(m_assist.m_state & AST_SOUL_TOTEM_BUFF || m_assist.m_state & AST_SOUL_TOTEM_ATTK ) // 토템은 공격 불가 스킬만 사용 가능
		return false;
	if(IS_PC(this))
	{
		if(m_assist.m_state & AST_SAFEGUARD && this->m_bFirstDoIt == false)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_DO_NOT_ATTACK_DO_MOVE);
			SEND_Q(rmsg, TO_PC(this)->m_desc);
			return false;
		}
	}
	return true;
}

bool CCharacter::CanSpell()
{
	if (m_assist.m_state & AST_STONE)
		return false;
	if (m_assist.m_state & AST_SILENT)
		return false;
	if (m_assist.m_state & AST_STURN)
		return false;
	if (m_assist.m_state & AST_SLEEP)
		return false;
	if (m_assist.m_state & AST_FAKEDEATH)
		return false;
	if (m_assist.m_state & AST_FREEZE)
		return false;
	return true;
}

void CCharacter::ApplyAssistData(ASSISTVALUE* add, ASSISTVALUE* rate)
{
	m_attackSpeed -= (char)add->attackspeed;
	m_attackSpeed -= (char)((int)m_attackSpeed * rate->attackspeed / SKILL_RATE_UNIT);
	m_magicSpeed += (char)add->magicspeed;
	m_magicSpeed += (char)((int)m_magicSpeed * rate->magicspeed / SKILL_RATE_UNIT);
	m_skillSpeed += rate->skillspd;
	m_skillCooltime += rate->SkillCooltime;
	m_skillCooltime += add->SkillCooltime;
	m_statPall += add->statpall;

	this->m_str += add->statpall;
	if( this->m_str < 1) this->m_str = 1;
	this->m_dex += add->statpall;
	if( this->m_dex < 1) this->m_dex = 1;
	this->m_int += add->statpall;
	if( this->m_int < 1) this->m_int = 1;
	this->m_con += add->statpall;
	if( this->m_con < 1) this->m_con = 1;

	// < 071226 kjban add
	m_statPall_per += add->statpall_per;
	this->m_str += this->m_str*add->statpall_per/100;
	if( this->m_str < 1) this->m_str = 1;
	this->m_dex += this->m_dex*add->statpall_per/100;
	if( this->m_dex < 1) this->m_dex = 1;
	this->m_int += this->m_int*add->statpall_per/100;
	if( this->m_int < 1) this->m_int = 1;
	this->m_con += this->m_con*add->statpall_per/100;
	if( this->m_con < 1) this->m_con = 1;
	// >

	SetRunSpeed(GetRunSpeed() + add->movespeed / 10.0f);
	SetRunSpeed(GetRunSpeed() + GetRunSpeed() * rate->movespeed / SKILL_RATE_UNIT);

	m_maxHP += add->maxhp;
	m_maxHP += m_maxHP * rate->maxhp / SKILL_RATE_UNIT;

	if(IS_PC(this))
	{
		if(TO_PC(this)->isWarZone() == true)
		{
			m_maxHP += add->war_max_hp;
		}
	}

	int addmaxhp450 = add->maxhp450;
	int ratemaxhp450 = m_maxHP * rate->maxhp450 / SKILL_RATE_UNIT;;
	if (addmaxhp450 > 450)
		addmaxhp450 = 450;
	if (ratemaxhp450 > 450)
		ratemaxhp450 = 450;
	m_maxHP += addmaxhp450 + ratemaxhp450;

	m_maxMP += add->maxmp;
	m_maxMP += m_maxMP * rate->maxmp / SKILL_RATE_UNIT;

	// 060227 : bs : 유료 HP,MP 확장
	if (rate->hcCashMaxHPUp > 0)
	{
		m_maxHP += m_maxHP * rate->hcCashMaxHPUp / SKILL_RATE_UNIT;
	}
	if (rate->hcCashMaxMPUp > 0)
	{
		m_maxMP += m_maxMP * rate->hcCashMaxMPUp / SKILL_RATE_UNIT;
	}

	if( gserver->isActiveEvent(A_EVENT_HALLOWEEN) )
	{
		m_maxHP += m_maxHP * rate->hcEventHalloweenMaxHP / SKILL_RATE_UNIT;
		//m_runSpeed += m_runSpeed * rate->hcEventHalloweenSpeed / SKILL_RATE_UNIT;
		SetRunSpeed(GetRunSpeed() + GetRunSpeed() * rate->hcEventHalloweenSpeed / SKILL_RATE_UNIT);
	}

	m_attackRange += add->attackdist;
	m_attackRange += m_attackRange * rate->attackdist / SKILL_RATE_UNIT;
}

bool CCharacter::IsInPeaceZone(bool bBlockAsPeace)
{
	if (!m_pArea)
		return true;
	unsigned short mapAttr = m_pArea->GetAttr(GET_YLAYER(this), GET_X(this), GET_Z(this));

	if(mapAttr & MATT_FREEPKZONE)
		return false;
	else if(mapAttr & MATT_PEACE || mapAttr & MATT_PRODUCT_PUBLIC || mapAttr & MATT_PRODUCT_PRIVATE)
		return true;
	else if(mapAttr & MATT_UNWALKABLE)
		return bBlockAsPeace;
	else
		return false;
}

bool CCharacter::IsInRaidZone()
{
	if (!m_pArea)
		return true;

	switch( m_pArea->m_zone->m_index )
	{
	case ZONE_ALTER_OF_DARK:
	case ZONE_AKAN_TEMPLE:
	case ZONE_TARIAN_DUNGEON:
		return true;
	}
	return false;
}

void CCharacter::applyAuraSkill( CCharacter* ch, int& auraCount, int level, CAssistData* outData )
{
	int skillIndex = 0;
	// npc가 오오라 디버프가 이미 걸려 있는지 확인
	// 위에 있는 MT_ASSIST, MST_ASSIST_AURA_DARKNESS 등등 타입으로 찾는 것은 불가능하다.
	// 위의 타입은 나이트쉐도우 자신에게 거는 버프인 오오라고 여기에 스킬 인덱스로 되어 있는 것은 대상에게 들어가는 디버프이다.
	if (ch && outData->m_proto && !ch->m_assist.FindBySkillIndex(898) && !ch->m_assist.FindBySkillIndex(899))
	{
		// notHelp 오러는 캐릭터 버프와 몬스터 디버프 스킬 두개가 있다.
		// 캐릭터 버프로 몬스터에게 걸릴 디버프가 결정된다.
		switch(outData->m_proto->m_index)
		{
		// 오러오브다크니스
		case 764:
			skillIndex = 898;
			break;

		// 오러오브위크니스
		case 765:
			if(IS_PET(ch))
			{
				CPet* pet = TO_PET(ch);
				if(pet->GetOwner()->m_index == this->m_index)
				{
					return;
				}
			}
			else if(IS_APET(ch))
			{
				CAPet* apet = TO_APET(ch);
				if(apet->GetOwner()->m_index == this->m_index)
				{
					return;
				}
			}

			skillIndex = 899;
			break;

		//오러오브일루젼
		case 766:
			if(IS_NPC(ch))
				return;

			skillIndex = 900;
			break;

		default:
			skillIndex = 0;
			break;
		}

		if (skillIndex > 0)
		{
			CSkill * skill = gserver->m_skillProtoList.Create(skillIndex, level) ;
			bool apply = false;
			// 캐릭터로 스킬을 걸면 npc가 공격하므로 npc가 npc에게 버프를 걸도록 설정
			if(skill)
			{
				ApplySkill(ch, ch, skill, -2, apply);
				auraCount++;
			}
		}
	}

	// 버프가 이미 걸려 있는 npc를 찾아도 오오라 카운트는 올려준다.
	else
		auraCount++;
}

void CCharacter::addItemCoolTime( int item_index )
{
	std::map<int, int>::iterator it = this->m_itemCoolMap.find(item_index);
	if(it != this->m_itemCoolMap.end())
		it->second = gserver->getNowSecond();
	else
		this->m_itemCoolMap.insert(std::map<int, int>::value_type(item_index, gserver->getNowSecond()));
}

bool CCharacter::checkItemCoolTime( int item_index, CSkill* skill )
{
	//아이템 스킬 쿨타임 체크 한 후 아직 사용 시간이 안되어 있으면 return 0;
	std::map<int, int>::iterator it = this->m_itemCoolMap.find(item_index);
	if(it != this->m_itemCoolMap.end())
	{
		if(it->second + (skill->m_proto->m_reuseTime / 10) > gserver->getNowSecond())
			return false;
	}

	return true;
}

void CCharacter::setSyndicateType( int syndicateType )
{
	if(IS_NPC(this))
	{
		CNPC* npc = TO_NPC(this);
		if(npc->GetOwner() != NULL)
		{
			m_syndicateType = TO_NPC(this)->GetOwner()->getSyndicateType();
			return;
		}
	}
	m_syndicateType = syndicateType;
}
bool CCharacter::IsEnemy(CCharacter* tch)
{
	if (this == tch)
		return false;

	//뭐가 됐던 rvr존에서는 결사대타입이 같으면 아군
	if( (this->m_pZone != NULL && this->m_pZone->isRVRZone() == true) &&
		(tch->m_pZone != NULL && tch->m_pZone->isRVRZone() == true) )
	{
		if(this->getSyndicateType() == tch->getSyndicateType())
			return false;
		else
			return true;
	}
	
	switch (m_type)
	{
	case MSG_CHAR_PC:
		switch (tch->m_type)
		{
		case MSG_CHAR_NPC:
			// PC가 수호병을 공격할 때에는 공성 지역내에 있어야 적으로 인식한다
			if (TO_NPC(tch)->m_proto->CheckFlag(NPC_CASTLE_GUARD))
			{
				if (GetMapAttr() & MATT_WAR && IS_ATTACK_TEAM(TO_PC(this)->GetJoinFlag(m_pZone->m_index)))
					return true;
				else
					return false;
			}

			if(TO_NPC(tch)->GetOwner() &&  TO_NPC(tch)->GetOwner() == TO_PC(this) )
			{
				if( !TO_NPC(tch)->Check_MobFlag(STATE_MONSTER_PARASITE) )
					return false;
			}

			return true;

		case MSG_CHAR_PC:
			{
				CPC* ch = TO_PC(this);
				CPC* target = TO_PC(tch);

				// 같은 파티는 적이 될 수 없음 : 파티가 가장 높은 우선순위를 가짐
				if (ch->IsParty() && target->IsParty() && ch->m_party == target->m_party)
					return false;
				if( ch->IsExped() && target->IsExped() && ch->m_Exped == target->m_Exped )
					return false;

				// 같은 길드는 적이 될 수 없음 -> 결사대가 다르다면 적이 될수도 있음
				if (ch->m_guildInfo && target->m_guildInfo && ch->m_guildInfo->guild() == target->m_guildInfo->guild())
				{
					// 쥬노마을 FREEPKZONE 에서는 같은 길드도 공격가능
					if(ch->m_pZone && ((ch->m_pZone->m_index == ZONE_START) || (ch->m_pZone->IsWarGroundZone()))
							&& (ch->GetMapAttr() & MATT_FREEPKZONE)
							&& ch->IsSetPlayerState(PLAYER_STATE_PKMODE)
							&& target->m_pZone && ((target->m_pZone->m_index == ZONE_START) || (ch->m_pZone->IsWarGroundZone()))
							&& (target->GetMapAttr() & MATT_FREEPKZONE)
							&& target->IsSetPlayerState(PLAYER_STATE_PKMODE))
					{
						return true;
					}

					//드라탄 PVP 경기장에서는 같은 길드도 공격이 가능하도록 한다.
					if( ch->IsSetPlayerState(PLAYER_STATE_PKMODE)
							&& ch->m_pArea->m_zone->m_index == ZONE_PK_TOURNAMENT
							&& target->IsSetPlayerState(PLAYER_STATE_PKMODE)
							&& target->m_pArea->m_zone->m_index == ZONE_PK_TOURNAMENT)
					{
						return true;
					}
#ifdef EXTREME_CUBE
					if(!gserver->m_extremeCube.IsGuildCubeTime())
					{
						// 파티큐브일 때
						// 큐브 안에서는 같은 길드라도 공격가능
						if(ch->m_pZone != NULL && !ch->m_pZone->IsExtremeCube())
							return false;

						if(target->m_pZone !=NULL && !target->m_pZone->IsExtremeCube())
							return false;

						return true;
					}
#endif // EXTREME_CUBE

					return false;
				}
				// 길드전
				if (ch->m_guildInfo && (ch->m_guildInfo->guild()->battleState() == GUILD_BATTLE_STATE_ING)
						&& target->m_guildInfo && (target->m_guildInfo->guild()->battleState() == GUILD_BATTLE_STATE_ING))
				{
					if (ch->m_guildInfo->guild()->battleIndex() == target->m_guildInfo->guild()->index() &&
							target->m_guildInfo->guild()->battleIndex() == ch->m_guildInfo->guild()->index())
					{
						return true;
					}
				}

				// 둘다 공성 참여자 이면
				// 서로의 참여 플래그가 적대적이면 적이다
#ifdef CHANGE_WARCASTLE_SETTING
				int nZoneIdx = -1;
				CWarCastle* castle = CWarCastle::GetCastleObject(ZONE_DRATAN);
				if (castle && castle->GetState() != WCSF_NORMAL)
				{
					nZoneIdx = ZONE_DRATAN;
				}
				else
				{
					castle = CWarCastle::GetCastleObject(ZONE_MERAC);
					if (castle && castle->GetState() != WCSF_NORMAL)
					{
						nZoneIdx = ZONE_MERAC;
					}
				}
				if ( nZoneIdx > 0 )
				{
					// 서로 적대 관게일 경우 전장 내, 전장 밖 모두 공격 가능하다.
					if ( (IS_DEFENSE_TEAM(ch->GetJoinFlag(nZoneIdx)) && IS_ATTACK_TEAM(target->GetJoinFlag(nZoneIdx))) ||
							( IS_ATTACK_TEAM(ch->GetJoinFlag(nZoneIdx)) && target->GetJoinFlag(nZoneIdx) != WCJF_NONE) )
					{
						return true;
					}
					// 공격자가 공수성이면, 3세력 공격 가능하다.
#ifdef CHECK_CASTLE_AREA
					// 공성존이고, 공성 지역 내이다.
					if ((ch->m_pZone->m_index == nZoneIdx && tch->m_pZone->m_index == nZoneIdx) &&
							(ch->GetMapAttr() & MATT_WAR || ch->m_pZone->IsWarZone((int)ch->m_pos.m_x, (int)ch->m_pos.m_z)) &&
							(tch->GetMapAttr() & MATT_WAR || tch->m_pZone->IsWarZone((int)tch->m_pos.m_x, (int)tch->m_pos.m_z)) )
#else
					if ( ch->m_pZone->m_index == nZoneIdx && tch->m_pZone->m_index == nZoneIdx && ch->GetMapAttr() == tch->GetMapAttr() && ch->GetMapAttr() & MATT_WAR )
#endif // CHECK_CASTLE_AREA
					{
						// 공,수성 vs 3세력
						if ( (IS_DEFENSE_TEAM(ch->GetJoinFlag(nZoneIdx)) && target->GetJoinFlag(nZoneIdx) == WCJF_NONE ) ||
								( IS_ATTACK_TEAM(ch->GetJoinFlag(nZoneIdx)) && target->GetJoinFlag(nZoneIdx) == WCJF_NONE) )
							return true;
					}

#else // CHANGE_WARCASTLE_SETTING
				CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
				if (castle && castle->GetState() != WCSF_NORMAL)
				{
#ifdef CHECK_CASTLE_AREA
					if ((ch->GetMapAttr() & MATT_WAR || ch->m_pZone->IsWarZone((int)ch->m_pos.m_x, (int)ch->m_pos.m_z))
							&& (tch->GetMapAttr() & MATT_WAR || tch->m_pZone->IsWarZone((int)tch->m_pos.m_x, (int)tch->m_pos.m_z)))
					{
						if ( (IS_DEFENSE_TEAM(ch->GetJoinFlag(ch->m_pZone->m_index)) && IS_ATTACK_TEAM(target->GetJoinFlag(ch->m_pZone->m_index))) ||
								( IS_ATTACK_TEAM(ch->GetJoinFlag(ch->m_pZone->m_index)) && target->GetJoinFlag(ch->m_pZone->m_index) != WCJF_NONE)  )
							return true;

						if( ch->GetJoinFlag(ch->m_pZone->m_index) != WCJF_NONE )
							return true;

						if( target->GetJoinFlag(ch->m_pZone->m_index) != WCJF_NONE)
							return false;
					}
#else
					if (ch->GetMapAttr() == tch->GetMapAttr() && ch->GetMapAttr() & MATT_WAR)
					{
						if ( (IS_DEFENSE_TEAM(ch->GetJoinFlag(ch->m_pZone->m_index)) && IS_ATTACK_TEAM(target->GetJoinFlag(ch->m_pZone->m_index))) ||
								( IS_ATTACK_TEAM(ch->GetJoinFlag(ch->m_pZone->m_index)) && target->GetJoinFlag(ch->m_pZone->m_index) != WCJF_NONE)  )
							return true;
					}

#endif // CHECK_CASTLE_AREA
#endif // CHANGE_WARCASTLE_SETTING
				}
//#endif
				/////////////////////////////////////////////
				// BANGWALL : 2005-07-18 오전 11:38:17
				// Comment : freepkzone

				if ( ch->GetMapAttr() == tch->GetMapAttr() && ch->GetMapAttr() & MATT_FREEPKZONE )
				{
					if(tch->m_level <= PKMODE_LIMIT_LEVEL)
						return false;
					
					return true;
				}

				if (ch->IsSetPlayerState(PLAYER_STATE_PKMODE))
				{
					bool bSkipLevel = false;

#ifdef CHANGE_WARCASTLE_SETTING
					// 공성전 지역에서 공성 진행시에는 레벨 검사 안함
					if (ch->m_pZone->m_index == tch->m_pZone->m_index && ch->GetMapAttr() == tch->GetMapAttr() && ch->GetMapAttr() & MATT_WAR)
#else	// CHANGE_WARCASTLE_SETTING
					if (ch->m_pZone->m_index == tch->m_pZone->m_index && ch->m_pZone->m_index == CWarCastle::GetCurSubServerCastleZoneIndex() && ch->GetMapAttr() == tch->GetMapAttr() && ch->GetMapAttr() & MATT_WAR)
#endif	// CHANGE_WARCASTLE_SETTING
					{
						CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
						if (castle && castle->GetState() != WCSF_NORMAL)
						{
							bSkipLevel = true;
						}
					}

					if(!bSkipLevel && tch->m_level <= PKMODE_LIMIT_LEVEL )
					{
#ifdef FREE_PK_SYSTEM
						if( gserver->m_bFreePk)
							return true;
#endif // FREE_PK_SYSTEM
						return false;
					}
					else
						return true;
				}

				if (ch->IsSetPlayerState(PLAYER_STATE_RAMODE))
				{
					if (IsRaList(ch, target))
						return true;
					else
						return false;
				}

				return false;
			}
			break;

		case MSG_CHAR_PET:
			{
				CPet* tpet = TO_PET(tch);
				if (!tpet->GetOwner())
					return false;
				else
					return IsEnemy(tpet->GetOwner());
			}
			break;

		case MSG_CHAR_ELEMENTAL:
			{
				CElemental* tElemental = TO_ELEMENTAL(tch);
				if (!tElemental->GetOwner())
					return false;
				else
					return IsEnemy(tElemental->GetOwner());
			}
			break;

		case MSG_CHAR_APET :
			{
				CAPet* apet = TO_APET(tch);
				if (!apet->GetOwner())
					return false;
				else
					return IsEnemy(apet->GetOwner());
			}
			break;

		default:
			return false;
		}
		break;

	case MSG_CHAR_NPC:
		{
			CPC* tpc = NULL;
			switch (tch->m_type)
			{
			case MSG_CHAR_PC:
				tpc = TO_PC(tch);
				break;
			case MSG_CHAR_PET:
				tpc = TO_PET(tch)->GetOwner();
				break;
			case MSG_CHAR_ELEMENTAL:
				tpc = TO_ELEMENTAL(tch)->GetOwner();
				break;
			case MSG_CHAR_APET :
				tpc = TO_APET(tch)->GetOwner();
				break;
			// 일반 몬스터가 테이밍된 몬스터를 적으로 인식
			case MSG_CHAR_NPC :
				{
					CNPC* target_npc = TO_NPC(tch);
					
					if (target_npc == NULL)
						return false;

					if(target_npc->m_proto->CheckFlag(NPC_PEACEFUL) != 0)
						return false;

					if( target_npc->GetOwner() )
						tpc = TO_NPC(tch)->GetOwner();
					else if( TO_NPC(this)->Check_MobFlag( STATE_MONSTER_TOTEM_ATTK )
						|| TO_NPC(this)->Check_MobFlag( STATE_MONSTER_TOTEM_ITEM_ATTK ) )
						return true;
				}
				break;
			default:
				tpc = NULL;
				break;
			}

			// 여기로 넘어오면 테이밍 npc가 일반 npc를 타겟으로 상대하는 중이다.
			// tpc가 없고 npc 이면 일반 몬스터 이므로 적이다.
			if ( tpc == NULL && IS_NPC(tch) && TO_NPC(this)->Check_MobFlag( STATE_MONSTER_TAMING ) )
			{
				// 자기 자신이면 적이 아님
				if ( this == tch )
				{
					return false;
				}

				// 다른 몬스터는 적
				else
				{
					return true;
				}
			}

			if ( tpc == NULL
					&& (( IS_NPC(tch) && TO_NPC(this)->Check_MobFlag( STATE_MONSTER_MERCENARY ))
						|| TO_NPC(this)->Check_MobFlag( STATE_MONSTER_TRAP )
						|| TO_NPC(this)->Check_MobFlag( STATE_MONSTER_SUICIDE ) ) )
			{
				if(this == tch )
				{
					return false;
				}
				else
				{
					CPC* owner = TO_NPC(this)->GetOwner();
					if( owner == NULL )
						return false;
					int zoneindex = owner->m_pZone->m_index;
					CWarCastle* castle = CWarCastle::GetCastleObject(zoneindex);
					if( castle != NULL && castle->GetState() != WCSF_NORMAL && owner->GetMapAttr() & MATT_WAR
							&& ( IS_ATTACK_TEAM( owner->GetJoinFlag( owner->m_pZone->m_index ) )
								 || IS_DEFENSE_TEAM( owner->GetJoinFlag( owner->m_pZone->m_index ) ) ) )
					{
						if( IS_ATTACK_TEAM(owner->GetJoinFlag( owner->m_pZone->m_index) ) )// 공성 측이라면
						{
							if( TO_NPC(tch)->m_proto->CheckFlag(NPC_WARCASTLE) )
								return true;
						}
					}
					else
						return true;
				}
			}

			if (tpc)
			{
				// 수호병은 공성측만 적으로 인식
				CNPC* npc_ = TO_NPC(this);
				if (npc_->m_proto->CheckFlag(NPC_CASTLE_GUARD | NPC_CASTLE_TOWER))
				{
					if (IS_ATTACK_TEAM(tpc->GetJoinFlag(tpc->m_pZone->m_index)))
						return true;
					else
						return false;
				}

				// 테이밍된 npc는 pc를 아군으로 인식, 힐 스킬을 사용해준다.
				if ( npc_->Check_MobFlag( STATE_MONSTER_TAMING )  )
				{
					return false;
				}

				if( npc_->Check_MobFlag( STATE_MONSTER_MERCENARY )
						|| npc_->Check_MobFlag( STATE_MONSTER_TRAP )
						|| npc_->Check_MobFlag( STATE_MONSTER_SUICIDE ) )
				{
					CPC* owner = npc_->GetOwner();

					if( !owner )
						return false;

					if( owner == tpc )
						return false;

					if( IsRaList(owner, tpc) ) // 오너와 정당방위 PC라면 적이다.
						return true;

					if( npc_->Check_MobFlag( STATE_MONSTER_TRAP) || npc_->Check_MobFlag( STATE_MONSTER_SUICIDE ) )
					{
						if( owner->IsSetPlayerState( PLAYER_STATE_PKMODE) && tpc->IsSetPlayerState(PLAYER_STATE_PKMODE ) )
							return true;
					}

					int zoneindex = owner->m_pZone->m_index;
					CWarCastle* castle = CWarCastle::GetCastleObject(zoneindex);

					if( castle != NULL && castle->GetState() != WCSF_NORMAL && owner->GetMapAttr() & MATT_WAR
							&& ( IS_ATTACK_TEAM( owner->GetJoinFlag( owner->m_pZone->m_index ) )
								 || IS_DEFENSE_TEAM( owner->GetJoinFlag( owner->m_pZone->m_index ) ) ) ) // 오너가 공성에 참여해 있다면
					{
						if( IS_ATTACK_TEAM(owner->GetJoinFlag( owner->m_pZone->m_index) ) )// 공성 측이라면
						{
							if( IS_DEFENSE_TEAM( tpc->GetJoinFlag(owner->m_pZone->m_index) )  )
								return true;
						}
						else if(IS_DEFENSE_TEAM( owner->GetJoinFlag(owner->m_pZone->m_index) ) ) // 수성 측이라면
						{
							if( IS_ATTACK_TEAM( tpc->GetJoinFlag(owner->m_pZone->m_index) )  )
								return true;
						}
					}
					return false;
				}
				else if( npc_->Check_MobFlag( STATE_MONSTER_TOTEM_ITEM_ATTK ) )
				{
					CPC* owner = npc_->GetOwner();

					if(owner == NULL)
						return true;

					if(owner == tpc)
						return false;
					else if(owner->IsEnemy(tpc) == false)
						return false;
				}
				return true;
			}
			else
				return false;
		}
		break;
	case MSG_CHAR_PET:
		{
			CPet* pet = TO_PET(this);
			if (!pet->GetOwner())
				return false;
			else
				return pet->GetOwner()->IsEnemy(tch);
		}
		break;

	case MSG_CHAR_ELEMENTAL:
		{
			CElemental* pElemental = TO_ELEMENTAL(this);
			if (!pElemental->GetOwner())
				return false;
			else
				return pElemental->GetOwner()->IsEnemy(tch);
		}
		break;

	case MSG_CHAR_APET :
		{
			CAPet* apet = TO_APET(this);
			if( apet )
			{
				return apet->GetOwner()->IsEnemy(tch);
			}
		}

	default:
		return false;
	}
}

bool CCharacter::CanVisible(CCharacter* tch)
{
	if (this == tch)
		return true;

	if (!tch->IsInvisible())
		return true;

	switch (m_type)
	{
	case MSG_CHAR_PC:
		switch (tch->m_type)
		{
		case MSG_CHAR_PC:
			{
				CPC* pc = TO_PC(this);
				CPC* tpc = TO_PC(tch);
				if (pc->IsParty() && tpc->IsParty() && pc->m_party == tpc->m_party)
					return true;
				else
					return false;
			}
			break;

		case MSG_CHAR_NPC:
			return false;

		default:
			return false;
		}
		break;

	case MSG_CHAR_NPC:
		switch (tch->m_type)
		{
		case MSG_CHAR_PC:
			if (tch->m_level + 5 - m_level <= 0)
				return true;
			else
				return false;
			break;

		case MSG_CHAR_NPC:
			return true;
			break;

		default:
			return false;
		}
		break;

	default:
		return false;
	}
}

void CCharacter::CancelInvisible()
{
	m_assist.CancelInvisible();
	if (m_currentSkill && m_currentSkill->m_proto->m_index == 131 && m_currentSkill->m_state == SKILL_CON_READY)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSkillCancelMsg(rmsg, this);
		m_pArea->SendToCell(rmsg, this, true);
		m_currentSkill->Cancel(this);
		m_currentSkill = NULL;
	}
}

int CCharacter::GetMeleeLevelBonus() const
{
	return (m_type == MSG_CHAR_PC && TO_PC(this)->m_job2) ? MAX((m_level - JOB_2ND_LEVEL), 0) : 0;
}

int CCharacter::GetRangeLevelBonus() const
{
	return (m_type == MSG_CHAR_PC && TO_PC(this)->m_job2) ? MAX((m_level - JOB_2ND_LEVEL), 0) : 0;
}

int CCharacter::GetMagicLevelBonus() const
{
	return (m_type == MSG_CHAR_PC && TO_PC(this)->m_job2) ? MAX((m_level - JOB_2ND_LEVEL), 0) : 0;
}

void CCharacter::CancelDamageLink()
{
	m_assist.CureAssist(MST_ASSIST_DAMAGELINK, 99);
}

unsigned short CCharacter::GetMapAttr()
{
	return (m_pArea) ? m_pArea->GetAttr(GET_YLAYER(this), GET_X(this), GET_Z(this)) : MATT_UNWALKABLE;
}

void CCharacter::ApplyPassiveSkill(CSkill* skill, int param)
{
	// 패시브 없는 캐릭은 스킵
	switch (m_type)
	{
	case MSG_CHAR_APET:

	case MSG_CHAR_PC:
	case MSG_CHAR_ELEMENTAL:
		break;

	case MSG_CHAR_PET:
	case MSG_CHAR_NPC:
	default:
		return ;
	}

	if (skill == NULL)
		return ;

	const CSkillProto* sp;
	const CSkillLevelProto* slp;
	const CMagicProto* mp;
	const CMagicLevelProto* mlp;
	int i;

	sp = skill->m_proto;
	if (sp == NULL)
		return ;
	slp = sp->Level(skill->m_level);
	if (slp == NULL)
		return ;

	// 필요 스탯 검사 : PC
	if (IS_PC(this))
	{
		CPC* pc = TO_PC(this);
		const CSkillLevelProto* level1 = sp->Level(1);
		if (!level1)
			return ;
		if (pc ->m_statpt_str < level1->m_learnstr)
			return ;
		if (pc ->m_statpt_dex < level1->m_learndex)
			return ;
		if (pc ->m_statpt_int < level1->m_learnint)
			return ;
		if (pc ->m_statpt_con < level1->m_learncon)
			return ;
	}

	// 적용 조건 검사
	if (!CanApplySkill(sp, slp))
		return ;

	for (i = 0; i < MAX_SKILL_MAGIC; i++)
	{
		mp = slp->m_magic[i];
		if (mp == NULL)
			return ;
		mlp = mp->Level(slp->m_magicLevel[i]);
		if (mlp == NULL)
			return ;

		int realPowerValue = mlp->m_nPowerValue * param / 100;
		int realHitrateValue = mlp->m_nHitrateValue * param / 100;

		// 효과 적용
#define APPVAL(v)		{ \
						switch (mp->m_damagetype) \
						{ \
						case MDT_ADDITION: \
							m_avPassiveAddition.v += realPowerValue; \
							break; \
						case MDT_RATE: \
							m_avPassiveRate.v += realPowerValue; \
							break; \
						} \
					}

		// 방어 관련은 소환수들도 적용
		switch (mp->m_type)
		{
		case MT_STAT:
			switch (mp->m_subtype)
			{
			case MST_STAT_ATTACK:
				APPVAL(attack);
				break;
			case MST_STAT_DEFENSE:
				APPVAL(defense);
				break;
			case MST_STAT_MAGIC:
				APPVAL(magic);
				break;
			case MST_STAT_RESIST:
				APPVAL(resist);
				break;
			case MST_STAT_HITRATE:
				APPVAL(hitrate);
				break;
			case MST_STAT_AVOID:
				APPVAL(avoid);
				break;
			case MST_STAT_CRITICAL:
				APPVAL(critical);
				break;
			case MST_STAT_ATTACKSPD:
				APPVAL(attackspeed);
				break;
			case MST_STAT_MAGICSPD:
				APPVAL(magicspeed);
				break;
			case MST_STAT_MOVESPD:
				APPVAL(movespeed);
				break;
			case MST_STAT_RECOVERHP:
				APPVAL(recoverhp);
				break;
			case MST_STAT_RECOVERMP:
				APPVAL(recovermp);
				break;
			case MST_STAT_MAXHP:
				APPVAL(maxhp);
				break;
			case MST_STAT_MAXMP:
				APPVAL(maxmp);
				break;
			case MST_STAT_DEADLY:
				APPVAL(deadly);
				break;
			case MST_STAT_MAGICHITRATE:
				APPVAL(magichitrate);
				break;
			case MST_STAT_MAGICAVOID:
				APPVAL(magicavoid);
				break;
			case MST_STAT_ATTACKDIST:
				APPVAL(attackdist);
				break;
			case MST_STAT_ATTACK_MELEE:
				APPVAL(attack_dam_melee);
				break;
			case MST_STAT_ATTACK_RANGE:
				APPVAL(attack_dam_range);
				break;
			case MST_STAT_HITRATE_SKILL:
				APPVAL(hitrate_skill);
				break;
			case MST_STAT_ATTACK_80:
				APPVAL(attack80);
				break;
			case MST_STAT_MAXHP_450:
				APPVAL(maxhp450);
				break;
			case MST_STAT_SKILLSPD:
				APPVAL(skillspd);
				break;
			case MST_STAT_HARD:
				APPVAL(hard);
				break;
			case MST_STAT_NPCATTACK:
				APPVAL(npcAttack);
				break;
			case MST_STAT_NPCMAGIC:
				APPVAL(npcMagic);
				break;
			case MST_STAT_SKILLCOOLTIME:
				APPVAL(SkillCooltime);
				break;
#ifdef ASSIST_DECREASE_SKILL_MP
			case MST_STAT_DECREASE_MANA_SPEND:
				APPVAL(decreaseskillmp);
				break;
#endif

			case MST_STAT_VALOR:
				{
					if (IS_PC(this))
					{
						CPC* pc = TO_PC(this);
						if (pc->IsParty())
							m_avPassiveAddition.attack += realPowerValue * pc->m_party->GetNearPartyMemberCount(pc) / 10;
						else if( pc->IsExped() && pc->m_Exped->GetNearExpeditionMemberCount(pc) > 1 )
						{
							const CExpedMember *member = pc->m_Exped->GetMemberByCharIndex(pc->m_index);
							int group_type = 0;
							if( member )
								group_type = member->GetGroupType();
							m_avPassiveAddition.attack += realPowerValue * pc->m_Exped->GetGroupMemberCount(group_type) / 10;
						}
					}
				}
				break;
			case MST_STAT_STATPALL:
				APPVAL(statpall);
				break;
			// << 071211 kjban edit
			case MST_STAT_ATTACK_PER:
				{
					switch (mp->m_damagetype)
					{
					case MDT_ADDITION:
						m_avPassiveAddition.attack += this->m_eqMelee * realPowerValue / 100;
						m_avPassiveAddition.attack += this->m_eqRange * realPowerValue / 100;
						m_avPassiveAddition.magic += this->m_eqMagic * realPowerValue / 100;
						break;
					case MDT_RATE:
						m_avPassiveRate.attack += this->m_eqMelee * realPowerValue / 100;
						m_avPassiveRate.attack += this->m_eqRange * realPowerValue / 100;
						m_avPassiveRate.magic += this->m_eqMagic * realPowerValue / 100;
						break;
					}
				}
				break;

			case MST_STAT_DEFENSE_PER:
				{
					switch (mp->m_damagetype)
					{
					case MDT_ADDITION:
						m_avPassiveAddition.defense += this->m_eqDefense * realPowerValue / 100;
						break;
					case MDT_RATE:
						m_avPassiveRate.defense += this->m_eqDefense * realPowerValue / 100;
						break;
					}
				}
				break;

			case MST_STAT_STATPALL_PER:
				m_avPassiveAddition.statpall_per += realPowerValue;
				break;

			case MST_STAT_STR:
				{
					COption::ApplyOptionValue( this, OPTION_STR_UP, realPowerValue, NULL );
					//APPVAL(stat_str);
				}
				break;
			case MST_STAT_DEX:
				{
					COption::ApplyOptionValue( this, OPTION_DEX_UP, realPowerValue, NULL );
					//APPVAL(stat_dex);
				}
				break;
			case MST_STAT_INT:
				{
					COption::ApplyOptionValue( this, OPTION_INT_UP, realPowerValue, NULL );
					//APPVAL(stat_int);
				}
				break;
			case MST_STAT_CON:
				{
					COption::ApplyOptionValue( this, OPTION_CON_UP, realPowerValue, NULL );
					//APPVAL(stat_con);
				}
				break;
			case MST_STAT_STRONG:
				{
					APPVAL(stronger_skill);
				}
			}
			break;
		// >>
		case MT_OTHER:
			switch (mp->m_subtype)
			{
			case MST_OTHER_REFLEX:
				APPVAL(hcReflex);
				m_avPassiveAddition.hcReflexProb = realHitrateValue;
				break;
			case MST_OTHER_AFFINITY_QUEST:
				APPVAL(affinity_quest);
				break;
			case MST_OTHER_AFFINITY_MONSTER:
				APPVAL(affinity_monster);
				break;
			case MST_OTHER_AFFINITY_ITEM:
				APPVAL(affinity_item);
				break;
			case MST_OTHER_QUEST_EXP:
				APPVAL(quest_exp);
				break;
			case MST_OTHER_GUILD_PARTY_EXP:
				APPVAL(guild_party_exp);
				break;
			case MST_OTHER_GUILD_PARTY_SP:
				APPVAL(guild_party_sp);
				break;
			}
			break;

// OLD ATTRIBUTE VARIABLE REUSE, Elenoa 2013.01.09
		case MT_ATTRIBUTE:
			if (mp->m_subtype < 0 || mp->m_subtype > AT_LIGHT)
				break;

			switch (mp->m_damagetype)
			{
			case MDT_ADDITION: /* attr attack */
				m_avPassiveAddition.attratt =
					AT_MIX(mp->m_subtype, mlp->m_nPowerValue);
				break;

			case MDT_RATE: /* attr defend */
				m_avPassiveAddition.attrdef =
					AT_MIX(mp->m_subtype, mlp->m_nPowerValue);
				break;
			}
			break;

		case MT_REDUCE:
			switch (mp->m_subtype)
			{
			case MST_REDUCE_MELEE:
				APPVAL(reduceMelee);
				m_avPassiveAddition.reduceMeleeProb += realHitrateValue;
				break;
			case MST_REDUCE_RANGE:
				APPVAL(reduceRange);
				m_avPassiveAddition.reduceRangeProb += realHitrateValue;
				break;
			case MST_REDUCE_MAGIC:
				APPVAL(reduceMagic);
				m_avPassiveAddition.reduceMagicProb += realHitrateValue;
				break;
#ifdef ASSIST_REDUCE_SKILL
			case MST_REDUCE_SKILL:
				APPVAL(reduceSkill);
				m_avPassiveAddition.reduceSkillProb += realHitrateValue;
				break;
#endif
			}
			break;

		case MT_IMMUNE:
			switch (mp->m_subtype)
			{
			case MST_IMMUNE_BLIND:
				m_avPassiveAddition.immune_blind = true;
				break;
			}
			break;

		case MT_MONEY:
			switch (mp->m_subtype)
			{
			case MST_MONEY_BUY:
				APPVAL(money_buy);
				break;
			case MST_MONEY_SELL:
				APPVAL(money_sell);
				break;
			case MST_MONEY_NAS:
				APPVAL(money_nas);
				break;
			}
		}
#undef APPVAL
	}
}

bool CCharacter::CheckHackAttack(CPC* pPC)
{
	if (pPC == NULL)
		return false;
	switch (m_type)
	{
	case MSG_CHAR_PC:
	case MSG_CHAR_PET:
		break;

	default:
		return false;
	}

	m_hackAttackCount--;
	m_AttackServerTime += m_attackSpeed;
	m_AttackClientTime += gserver->m_pulse - m_lastAttackPulse  + PULSE_HACK_ATTACK_THRESHOLD ;

	if( m_hackAttackCount < 1 )
	{
		if( m_AttackClientTime < m_AttackServerTime )
		{
			GAMELOG << init("HACK ATTACK", pPC)
					<< "CHAR TYPE" << delim
					<< m_type << delim
					<< (gserver->m_pulse - m_lastAttackPulse) << delim
					<< m_AttackClientTime << delim
					<< m_AttackServerTime << end;

			if (pPC->m_desc->IncreaseHackCount(1))
				return true;
		}
		m_hackAttackCount = PULSE_HACK_ATTACK_COUNT;
		m_AttackClientTime = 0;
		m_AttackServerTime = 0;
	}

	m_lastAttackPulse = gserver->m_pulse;

	return false;
}

void CCharacter::SetDisableTime(int sec)
{
	if (m_pulseDisable > gserver->m_pulse + (sec * PULSE_REAL_SEC))
		return ;
	m_pulseDisable = gserver->m_pulse + (sec * PULSE_REAL_SEC);
}

bool CCharacter::IsDisable()
{
	return (m_pulseDisable > gserver->m_pulse);
}

bool CCharacter::ChekAttackType()
{
	// true 공속검사 , false 검사 안함
	if( m_attacktype == ATTACK_TYPE_NORMAL )
	{
		m_attackcount = 0;
		return true;
	}
	else if ( m_attacktype == ATTACK_TYPE_DOUBLE )
	{
		m_attackcount++;
		if( m_attackcount < ATTACK_TYPE_DOUBLE )
			return false;
		m_attackcount = 0;
		return true;
	}
	else if ( m_attacktype == ATTACK_TYPE_INFINITY )
	{
		return false;
	}

	return true;
}

void CCharacter::SetExpedLabel(int nType)
{
	m_nExpedLabel = nType;
}

int CCharacter::GetExpedLabel()
{
	return m_nExpedLabel;
}
void CPC::ApplyTitle()
{
	int idx;
	switch (this->m_job)
	{
	case JOB_TITAN:
	case JOB_KNIGHT:
	case JOB_HEALER:
	case JOB_MAGE:
	case JOB_ROGUE:
	case JOB_SORCERER:
	case JOB_NIGHTSHADOW:
#ifdef EX_ROGUE
	case JOB_EX_ROGUE:
#endif // EX_ROGUE
#ifdef EX_MAGE
	case JOB_EX_MAGE:
#endif
		idx = this->m_job;
		break;
	default:
		return;
		break;
	}

	int title_count = 0;
	title_count = this->m_titleList.m_nCount;
	// 만약 title이 없으면 return;
	if (title_count == 0)
		return ;

	CTitle* temp;
	temp = this->m_titleList.Find(this->m_nCurrentTitle);
	if (temp == NULL)
		return ;

	int ntype = 0;
	int nvalue = 0;

	COptionProto* proto;

	for(int i = 0; i < MAX_TITLE_OPTION; i++)
	{
		if( temp->m_proto->m_option[i][TITLE_SYSTEM_OPTION_NUM] != -1 )
		{
			ntype = temp->m_proto->m_option[i][TITLE_SYSTEM_OPTION_NUM];
			proto = gserver->m_optionProtoList.FindProto(ntype);
			if(proto == NULL)
				return ;
			nvalue = proto->m_levelValue[temp->m_proto->m_option[i][TITLE_SYSTEM_OPTION_LEVEL]-1];

			switch(ntype)
			{
			case OPTION_STR_UP:
				m_opStr += nvalue;
				m_str += nvalue;
				m_maxHP += nvalue * levelup_hp[idx][0];
				break;
			case OPTION_DEX_UP:
				m_opDex += nvalue;
				m_dex += nvalue;
				break;
			case OPTION_INT_UP:
				m_opInt += nvalue;
				m_int += nvalue;
				m_maxMP += nvalue * levelup_mp[idx][2];
				break;
			case OPTION_CON_UP:
				m_opCon += nvalue;
				m_con += nvalue;
				m_maxHP += nvalue * levelup_hp[idx][3];
				m_maxMP += nvalue * levelup_mp[idx][3];
				break;
			case OPTION_HP_UP:
				m_maxHP += nvalue;
				break;
			case OPTION_MP_UP:
				m_maxMP += nvalue;
				break;
			case OPTION_DAMAGE_UP:
				m_opMelee += nvalue;
				m_opRange += nvalue;
				break;
			case OPTION_MELEE_DAMAGE_UP:
				m_opMelee += nvalue;
				break;
			case OPTION_RANGE_DAMAGE_UP:
				m_opRange += nvalue;
				break;
			case OPTION_MELEE_HIT_UP:
				m_opMeleeHitRate += nvalue;
				break;
			case OPTION_RANGE_HIT_UP:
				m_opRangeHitRate += nvalue;
				break;
			case OPTION_DEFENSE_UP:
				m_opDMelee += nvalue;
				m_opDRange += nvalue;
				break;
			case OPTION_MELEE_DEFENSE_UP:
				m_opDMelee += nvalue;
				break;
			case OPTION_RANGE_DEFENSE_UP:
				m_opDRange += nvalue;
				break;
			case OPTION_MELEE_AVOID_UP:
				m_opMeleeAvoid += nvalue;
				break;
			case OPTION_RANGE_AVOID_UP:
				m_opRangeAvoid += nvalue;
				break;
			case OPTION_MAGIC_UP:
				m_opMagic += nvalue;
				break;
			case OPTION_MAGIC_HIT_UP:
				m_opMagicHitRate += nvalue;
				break;
			case OPTION_RESIST_UP:
				m_opResist += nvalue;
				break;
			case OPTION_RESIST_AVOID_UP:
				m_opResistAvoid += nvalue;
				break;
			case OPTION_ALL_DAMAGE_UP:
				m_opMelee += nvalue;
				m_opRange += nvalue;
				m_opMagic += nvalue;
				break;
			case OPTION_ALL_HIT_UP:
				m_opMeleeHitRate += nvalue;
				m_opRangeHitRate += nvalue;
				m_opMagicHitRate += nvalue;
				break;
			case OPTION_ALL_DEFENSE_UP:
				m_opDMelee += nvalue;
				m_opDRange += nvalue;
				m_opResist += nvalue;
				break;
			case OPTION_ALL_AVOID_UP:
				m_opMeleeAvoid += nvalue;
				m_opRangeAvoid += nvalue;
				m_opResistAvoid += nvalue;
				break;
			case OPTION_NOT_USED_24:
				break;
			case OPTION_NOT_USED_25:
				break;
////////// 여기서 부터는 공성 조합 아이템용 옵션 //////////
			case OPTION_MIX_STR:
				break;
			case OPTION_MIX_DEX:
				break;
			case OPTION_MIX_INT:
				break;
			case OPTION_MIX_CON:
				break;
			case OPTION_MIX_ATTACK:
				break;
			case OPTION_MIX_MAGIC:
				break;
			case OPTION_MIX_DEFENSE:
				break;
			case OPTION_MIX_RESIST:
				break;
			case OPTION_MIX_STURN:
				break;
			case OPTION_MIX_BLOOD:
				break;
			case OPTION_MIX_MOVE:
				break;
			case OPTION_MIX_POISON:
				break;
			case OPTION_MIX_SLOW:
				break;
////////// 여기까지 부터는 공성 조합 아이템용 옵션 //////////
			case OPTION_DOWN_LIMITLEVEL: //// 아이템에 발린 옵션 사용불가
				break;
			case OPTION_INCREASE_INVEN:
				m_opIncreaseInven += nvalue;
				break;
			case OPTION_STEAL_MP:
				m_opMPSteal += nvalue;
				break;
			case OPTION_STEAL_HP:
				m_opHPSteal += nvalue;
				break;
			case OPTION_ATTACK_BLIND:
				if (m_opAttackBlind < nvalue) m_opAttackBlind = nvalue;
				break;
			case OPTION_ATTACK_POISON:
				if (m_opAttackPoison < nvalue) m_opAttackPoison = nvalue;
				break;
			case OPTION_ATTACK_CRITICAL:
				m_opAttackCritical += nvalue;
				break;
			case OPTION_RECOVER_HP:
				m_opRecoverHP += nvalue;
				break;
			case OPTION_RECOVER_MP:
				m_opRecoverMP += nvalue;
				break;
			case OPTION_DECREASE_SKILL_DELAY:
				m_opDecreaseSkillDelay += nvalue;
				break;
			case OPTION_DECREASE_SKILL_MP:
				m_opDecreaseSkillMP += nvalue;
				break;
			case OPTION_RESIST_STONE:
				m_opResistStone += nvalue;
				if(m_opResistStone > 100)
					m_opResistStone = 100;
				break;
			case OPTION_RESIST_STURN:
				m_opResistSturn += nvalue;
				if(m_opResistSturn > 100)
					m_opResistSturn = 100;
				break;
			case OPTION_RESIST_SILENT:
				m_opResistSilent += nvalue;
				if(m_opResistSilent > 100)
					m_opResistSilent = 100;
				break;
			case OPTION_BLOCKING:
				m_opBlocking += nvalue;
				break;
			case OPTION_MOVESPEED:
				//m_runSpeed += m_runSpeed * nvalue / 100;
				SetRunSpeed(GetRunSpeed() + GetRunSpeed() * nvalue / 100);
				break;
			case OPTION_FLYSPEED:
				m_flySpeed += (float)nvalue / 10.0f;
				break;
			case OPTION_ATTACK_DEADLY:
				m_opAttackDeadly += nvalue;
				break;
			case OPTION_STR_UP_RATE:
				nvalue = m_dbStr * nvalue / 100;
				m_opStr += nvalue;
				m_str += nvalue;
				m_maxHP += nvalue * levelup_hp[idx][0];
				break;
			case OPTION_DEX_UP_RATE:
				nvalue = m_dbDex * nvalue / 100;
				m_opDex += nvalue;
				m_dex += nvalue;
				break;
			case OPTION_INT_UP_RATE:
				nvalue = m_dbInt * nvalue / 100;
				m_opInt += nvalue;
				m_int += nvalue;
				m_maxMP += nvalue * levelup_mp[idx][2];
				break;
			case OPTION_CON_UP_RATE:
				nvalue = m_dbCon * nvalue / 100;
				m_opCon += nvalue;
				m_con += nvalue;
				m_maxHP += nvalue * levelup_hp[idx][3];
				m_maxMP += nvalue * levelup_mp[idx][3];
				break;
			case OPTION_HP_UP_RATE:
				nvalue = m_dbHP * nvalue / 100;
				m_maxHP += nvalue;
				break;
			case OPTION_MP_UP_RATE:
				nvalue = m_dbMP * nvalue / 100;
				m_maxMP += nvalue;
				break;
			case OPTION_WEAPON_UP_RATE:
				if(m_wearInventory.wearItemInfo[WEARING_WEAPON])
				{
					if(m_wearInventory.wearItemInfo[WEARING_WEAPON]->IsRangeWeapon())
					{
						m_opRange += m_wearInventory.wearItemInfo[WEARING_WEAPON]->m_itemProto->getItemNum0() * nvalue / 100;
					}
					else
					{
						m_opMelee += m_wearInventory.wearItemInfo[WEARING_WEAPON]->m_itemProto->getItemNum0() * nvalue / 100;
					}

					m_opMagic += m_wearInventory.wearItemInfo[WEARING_WEAPON]->m_itemProto->getItemNum1() * nvalue / 100;
				}
				break;
			case OPTION_ARMOR_UP_RATE:
				{
					const int amor_count = 7;
					static int armors[] = {WEARING_HELMET, WEARING_ARMOR_UP, WEARING_ARMOR_DOWN, WEARING_SHIELD, WEARING_GLOVE, WEARING_BOOTS, WEARING_BACKWING };

					int i;
					CItem *item;
					int deffence = 0, deffence2 = 0;
					for (i = 0; i < amor_count; i++)
					{
						item = m_wearInventory.wearItemInfo[armors[i]];
						if (item && item->IsArmorType() && !(item->getFlag() & FLAG_ITEM_SEALED))
						{
							deffence += item->m_itemProto->getItemNum0();
							deffence2 += item->m_itemProto->getItemNum1();
						}
					}

					m_opDMelee += deffence * nvalue / 100;
					m_opDRange += deffence * nvalue / 100;
					m_opResist += deffence2 * nvalue / 100;
				}
				break;
			case OPTION_MELEE_HIT_UP_RATE:
				nvalue = (int)GetHitrate(this, MSG_DAMAGE_MELEE) * nvalue / 100;
				m_opMeleeHitRate += nvalue;
				m_opRangeHitRate += nvalue;
				break;
			case OPTION_MAGIC_HIT_UP_RATE:
				nvalue = (int)GetHitrate(this, MSG_DAMAGE_MAGIC) * nvalue / 100;
				m_opMagicHitRate += nvalue;
				break;
			case OPTION_MELEE_AVOID_RATE:
				nvalue = (int)GetAvoid(this, MSG_DAMAGE_MELEE) * nvalue / 100;
				m_opMeleeAvoid += nvalue;
				m_opRangeAvoid += nvalue;
				break;
			case OPTION_MAGIC_AVOID_RATE:
				nvalue = (int)GetAvoid(this, MSG_DAMAGE_MAGIC) * nvalue / 100;
				m_opResistAvoid += nvalue;
				break;
			case OPTION_RECOVER_HP_RATE:
				nvalue = m_recoverHP * nvalue / 100;
				m_opRecoverHP += nvalue;
				break;
			case OPTION_RECOVER_MP_RATE:
				nvalue = m_recoverMP * nvalue / 100;
				m_opRecoverMP += nvalue;
				break;
			case OPTION_EXP_UP_RATE:
				m_opExpRate += nvalue;
				break;
			case OPTION_SP_UP_RATE:
				m_opSPRate += nvalue;
				break;
			case OPTION_APET_ELEMENT_ATTUP:
				if(GetAPet())
				{
					GetAPet()->m_opMelee += nvalue;
					GetAPet()->m_opRange += nvalue;
				}
				break;
			case OPTION_ALL_STAT_UP:
				m_opStr += nvalue;
				m_opDex += nvalue;
				m_opInt += nvalue;
				m_opCon += nvalue;
				break;
			case OPTION_PVP_DAMAGE_ABSOLB:
				m_absorbPVPDamageRate += nvalue;
				break;
			case OPTION_DEBUF_DECR_TIME:
				m_decreaseDBufTimeRate += nvalue;
				break;
			case OPTION_RECOVER_HP_NOTRATE:
				m_opRecoverHP_NoRate += nvalue;
				break;
			case OPTION_RECOVER_MP_NOTRATE:
				m_opRecoverMP_NoRate += nvalue;
				break;
			case OPTION_INCREASE_STRONG:
				m_opStrong += nvalue;
				break;
			case OPTION_INCREASE_HARD:
				m_plusHard += nvalue;
				break;
			case OPTION_INCREASE_HP:
				m_maxHP += nvalue;
				break;
			default:
				break;
			}
		}
	}
}

void CPC::TitleCurrentSkillCure(int titleIndex , bool blistDelete )
{
	if(titleIndex == -1 || m_nCurrentTitle == TITLE_SYSTEM_NO_TITLE )
		return;

	if( m_nCurrentTitle != titleIndex )
		return;

	int title = m_nCurrentTitle;
	m_nCurrentTitle = TITLE_SYSTEM_NO_TITLE;

	CalcStatus(true);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_TITLE_EXPIRED);
		RefMsg(rmsg) << 1 << title;
		SEND_Q(rmsg, m_desc);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		TitleSystemMsg(rmsg, MSG_EX_TITLE_SYSTEM_TITLE_EXPIRED, title, m_index);
		SEND_Q(rmsg, m_desc);
		m_pArea->SendToCell(rmsg, this);
	}

	GAMELOG << init("TITLE CURRENT DELETE", this) << delim
			<< "TITLE INDEX" << delim << title << end;

	if(blistDelete == true )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SubHelperTitleSystemDeleteReq(rmsg, m_index, title);
		SEND_Q(rmsg, gserver->m_subHelper);
	}
}

void CPC::CastllanTitleDelete(int castleZoneindex, bool bExclude, char bCastellan)
{
	CTitleNode* node = m_titleList.m_head;
	while(node)
	{
		if ( node->m_title->m_proto->m_castleNum > 0)
		{
			bool bDelete = false;
			CTitle * title = node->m_title;
			if(bExclude)		// castleZoneindex 이외의 것을 지움
			{
				if( castleZoneindex == -1 || title->m_proto->m_castleNum != castleZoneindex )
				{
					bDelete = true;
				}
				else
				{
					// 같은 castle 호칭이면 지위검사를 한다.
					CItemProto* proto = gserver->m_itemProtoList.FindIndex(title->m_proto->m_itemidx);
					if(proto && (proto->getItemFlag() & ITEM_FLAG_CASTLLAN) && ((proto->getItemNum3() >> 16 & 0xffff) !=  !bCastellan) )
					{
						bDelete = true;
					}
				}
			}
			else					// castleZoneindex 동일한 것을 지움
			{
				if( title->m_proto->m_castleNum == castleZoneindex )
				{
					bDelete = true;
				}
			}

			if(bDelete)
			{
				if( m_nCurrentTitle == title->m_proto->m_index )
					TitleCurrentSkillCure(title->m_proto->m_index, false);

				CNetMsg::SP rmsg(new CNetMsg);
				SubHelperTitleSystemDeleteReq(rmsg, m_index, title->m_proto->m_index);
				SEND_Q(rmsg, gserver->m_subHelper);
			}
		}
		node = node->m_next;
	}
}


bool CPC::AddExpUseGP(LONGLONG exp)
{
	if(m_level <= MAX_LEVEL)
	{
		m_exp += exp;
		GAMELOG << init("GUILD SKILL USE GP CHANGE TO EXP", this) << "ADD EXP : 30000" << end;
	}
	if(m_level >= MAX_LEVEL && m_exp > GetLevelupExp(m_level))
		m_exp = GetLevelupExp(m_level);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExpSPMsg(rmsg, exp, 0);
		SEND_Q(rmsg, m_desc);
	}

	int zoneindex = this->m_pZone->m_index;

	if(GetLevelupExp(m_level) <= m_exp && m_level < MAX_LEVEL)
	{
		do
		{
#ifdef GER_LOG
			GAMELOGGEM << init( 0, "CHAR_LEVLE_UP")
					   << LOG_VAL("account-id", this->m_desc->m_idname ) << blank
					   << LOG_VAL("character-id", this->m_desc->m_pChar->m_name ) << blank
					   << LOG_VAL("level", m_level+1 ) << blank
					   << endGer;
#endif // GER_LOG
			GAMELOG << init("LEVEL UP", this)
					<< m_level
					<< " ==> "
					<< m_level + 1
					<< end;

			m_exp -= GetLevelupExp(m_level);
			m_level++;

			{
				CNetMsg::SP rmsg(new CNetMsg);
				SubHelperLevelup(rmsg, this->m_index);
				SEND_Q(rmsg, gserver->m_subHelper);
			}

			// stat point 증가
			if (m_job2)
			{
				m_statpt_remain += 3;
				if ((m_level - JOB_2ND_LEVEL) % 2 == 0)
					m_statpt_remain += 1;
			}
			else
				m_statpt_remain++;

			{
				// STAT POINT
				CNetMsg::SP rmsg(new CNetMsg);
				StatPointRemainMsg(rmsg, this);
				SEND_Q(rmsg, m_desc);
			}

			// 레벨업 효과
			LevelUp(true);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperPartyMatchMemberChangeInfoMsg(rmsg, m_index, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_LEVEL, "", m_level, 0);
				SEND_Q(rmsg, gserver->m_helper);
			}
		}
		while(GetLevelupExp(m_level) <= m_exp);
		CalcStatus(true);

		Restore();
	}
	if( zoneindex != this->m_pZone->m_index)
		return true;

	return false;
}

void CCharacter::AddEP(int ep_ )
{
	if( ep_ > 0 )
	{
		int useSkill[3]= {1063, 1064, 1065};
		for(int i = 0; i < 3; i++)
		{
			if(this->m_assist.FindBySkillIndex(useSkill[i]))
				return ;
		}
	}

	if (ep_ > 8000)
		ep_ = 8000;

	m_ep += ep_ ;

	if(m_ep > m_maxEP)
		m_ep = m_maxEP;

	if(m_ep < 0)
		m_ep = 0;
}

// [101214: selo] 보상 아이템 드롭 수정
bool CPC::CheckInvenForQuestPrize(const CQuestProto* pQuestProto, const int& optItemIndex)
{
	if( NULL == m_desc )
		return false;

	if( NULL == pQuestProto )
		return false;

	CItemProto* pItemProto = NULL;
	int totalPrizeSpace_Normal = 0;
	for( int i = 0; i < QUEST_MAX_PRIZE; i++ )
	{
		if (pQuestProto->m_prizeType[i] != QPRIZE_ITEM)
			continue;

		pItemProto = gserver->m_itemProtoList.FindIndex(pQuestProto->m_prizeIndex[i]);

		if( !pItemProto )
			return false;

		if (this->m_inventory.FindByDBIndex(pQuestProto->m_prizeIndex[i], 0, 0) == NULL)
			continue;

		++totalPrizeSpace_Normal;
	}

	// 옵션 아이템
	if(optItemIndex != -1)
	{
		for (int j =0; j < QUEST_MAX_OPTPRIZE; j++)
		{
			if (pQuestProto->m_optPrizeType[j] != QPRIZE_ITEM)
				continue;

			if(pQuestProto->m_optPrizeIndex[j] != optItemIndex)
				continue;

			pItemProto = gserver->m_itemProtoList.FindIndex(optItemIndex);

			if( !pItemProto )
				return false;

			if (this->m_inventory.FindByDBIndex(optItemIndex, 0, 0) == NULL)
				continue;

			++totalPrizeSpace_Normal;
		}
	}

	if (this->m_inventory.getEmptyCount() < totalPrizeSpace_Normal)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgMessageBox(rmsg, MSG_EX_MSGBOX_INVEN_FULL);
		SEND_Q(rmsg, m_desc);

		return false;
	}

	return true;
}

bool CPC::CheckInvenForProduceNoramlPrize(const int& itemdbIndex, const int& grade, const int& boost)
{
	if( NULL == m_desc )
		return false;

	CItemProto* pItemProto = gserver->m_itemProtoList.FindIndex(itemdbIndex);
	if( !pItemProto )
		return false;

	int temp = 0;

	int count = 1;

	if( 0 != boost )
	{
		LONGLONG countOfGrade[5] = {150, 86, 55, 38, 30};
		count = countOfGrade[grade] / 2 * boost / 100;
	}
	else
	{
		if (gserver->m_bDoubleEvent)
			count = gserver->m_bDoubleProduceNum;
	}

	if( (pItemProto->getItemFlag() & ITEM_FLAG_COUNT) && (this->m_inventory.FindByDBIndex(itemdbIndex, 0, 0) != NULL) )
	{
		temp = 0;
	}
	else if (this->m_inventory.getEmptyCount() < 1 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgMessageBox(rmsg, MSG_EX_MSGBOX_INVEN_FULL);
		SEND_Q(rmsg, m_desc);

		return false;
	}

	return true;
}

bool CPC::CheckInvenForProduceRandomPrize()
{
	// [101216: selo]
	// 현재까지의 상황으로 Normal 인벤으로만 들어가기에 Normal 인벤의 공간으로만 비교
	// 그리고 무게는 모두 1로 가정함
	if( this->m_inventory.getEmptyCount() < 1 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgMessageBox(rmsg, MSG_EX_MSGBOX_INVEN_FULL);
		SEND_Q(rmsg, m_desc);

		return false;
	}

	int count = 1;

	if (gserver->m_bDoubleEvent)
		count = gserver->m_bDoubleProduceNum;

	return true;
}

bool CPC::CheckInvenForProcessPrize(CItemProto* pItemProto)
{
	return CheckInvenForPrize(pItemProto, 0, 0);
}

bool CPC::CheckInvenForTresureBoxPrize(CItem* pItem)
{
	if( !pItem )
		return false;

	CItemProto* pItemProto = gserver->m_itemProtoList.FindIndex(pItem->getDBIndex());

	return CheckInvenForPrize(pItemProto, pItem->getPlus(), pItem->getFlag());
}

bool CPC::CheckInvenForPrize(int nItemIndex, int plus, int flag, int count)
{
	CItemProto* pItemProto = gserver->m_itemProtoList.FindIndex(nItemIndex);

	return CheckInvenForPrize(pItemProto, plus, flag, count);
}

bool CPC::CheckInvenForPrize(CItemProto* pItemProto, int plus, int flag, int count)
{
	if( NULL == pItemProto )
		return false;

	int temp = 0;

	if( (pItemProto->getItemFlag() & ITEM_FLAG_COUNT) && (this->m_inventory.FindByDBIndex(pItemProto->getItemIndex(), plus, flag) != NULL) )
	{
		temp = 0;
	}
	else if(this->m_inventory.getEmptyCount() < 1 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgMessageBox(rmsg, MSG_EX_MSGBOX_INVEN_FULL);
		SEND_Q(rmsg, m_desc);

		return false;
	}

	return true;
}

bool CPC::CheckCastellanType(int& zonenum, char& castellan)
{
	if(!m_guildInfo)
		return false;
	if(!m_guildInfo->guild())
		return false;

	if(m_index == meracCastle.GetOwnerCharIndex())
	{
		zonenum = meracCastle.GetZoneIndex();
		castellan = 1;
		return true;
	}
	else if(m_index == CDratanCastle::CreateInstance()->GetOwnerCharIndex())
	{
		zonenum = CDratanCastle::CreateInstance()->GetZoneIndex();
		castellan = 1;
		return true;
	}
	else if(m_guildInfo->guild()->index() == meracCastle.GetOwnerGuildIndex())
	{
		zonenum = meracCastle.GetZoneIndex();
		castellan = 0;
		return true;
	}
	else if(m_guildInfo->guild()->index() == CDratanCastle::CreateInstance()->GetOwnerGuildIndex())
	{
		zonenum = CDratanCastle::CreateInstance()->GetZoneIndex();
		castellan = 0;
		return true;
	}
	return false;
}

const char* CPC::GetName()
{
	if( this->IsNick() )
		return this->m_nick;
	else
		return this->m_name;
}

bool CPC::IsNick()
{
	if (this->m_nick != NULL && strcmp(this->m_nick, "") != 0)
		return true;
	else
		return false;
}
//

void CAPet::AddFaith( int faith )
{
	m_nFaith += faith;

	if (m_nFaith < 0)
		m_nFaith = 0;
	else if (m_nFaith > m_pProto->m_nMaxFaith)
		m_nFaith = m_pProto->m_nMaxFaith;
}

void CAPet::AddStamina( int stm )
{
	m_nStm += stm;

	if (m_nStm < 0)
		m_nStm=0;
	else if (m_nStm > m_pProto->m_nMaxStm)
		m_nStm = m_pProto->m_nMaxStm;
}

void CAPet::resetStashTimer()
{
	m_nPetStashSeconds = gserver->m_nowseconds;
}

void CPet::resetStashTimer()
{
	m_petStashHungrySeconds = gserver->m_nowseconds;
	m_petStashSeconds = gserver->m_nowseconds;
}

void CPC::recalcByItem( CItem* pItem )
{
	for (int page = 0; page < QUICKSLOT_PAGE_NUM; ++page)
	{
		for (int slot = 0; slot < QUICKSLOT_MAXSLOT; ++slot)
		{
			if (m_quickSlot[page].m_slotType[slot] != QUICKSLOT_TYPE_ITEM)
				continue;

			if (m_quickSlot[page].m_item[slot] != pItem)
				continue;

			m_quickSlot[page].m_item[slot] = pItem;

			CNetMsg::SP rmsg(new CNetMsg);
			QuickSlotAddMsg(rmsg, this, page, slot);
			SEND_Q(rmsg, this->m_desc);
		}
	}
}

void CPC::ChangeQuickSlot( CItem* item, int itemSlotType )
{
	if(item == NULL)
	{
		return;
	}

	for (int i = 0; i < QUICKSLOT_PAGE_NUM; i++)
	{
		for (int j = 0; j < QUICKSLOT_MAXSLOT; j++)
		{
			if(m_quickSlot[i].m_item[j] == item)
			{
				m_quickSlot[i].m_slotType[j] = itemSlotType;

				{
					CNetMsg::SP rmsg(new CNetMsg);
					QuickSlotAddMsg(rmsg, this, i, j);
					SEND_Q(rmsg, this->m_desc);
				}
			}
		}
	}
}

#ifdef XTRAP
void CPC::xTrapSessionInit()
{
	XTrap_S_SessionInit( 120, CSFILE_NUM, gserver->m_XTrapMap, m_xtrapSessionBuf );// ITS-5545 가미고 요청에 의한 진단시간 수정 (300 -> 120 )
}
#endif

CPC* CNPC::GetOwner()
{
	if(m_owner != NULL)
	{
		if( m_owner->m_type == MSG_CHAR_UNKNOWN || m_owner->m_index == -1 )
			m_owner = NULL;
	}

	return m_owner;
}

void CPC::SendToggleState( int index, bool toggle, int toggle_type)
{
	CNetMsg::SP rmsg(new CNetMsg);
	ResponseClient::makeToggleMsg(rmsg, index, toggle, toggle_type);
	SEND_Q(rmsg, this->m_desc);
}

void CPC::toggleOff( int index, int toggle_type )
{
	switch(toggle_type)
	{
	case TOGGLE_SKILL:
		{
			CSkill* skill = this->FindSkill(index);
			if (skill == NULL)
				return;

			skill->m_toggle = false;

			this->SendToggleState(index, skill->m_toggle, TOGGLE_SKILL);
			break;
		}
	case TOGGLE_ITEM:
		{
			CItem* item = this->m_inventory.FindByVirtualIndex(index);
			if (item == NULL)
				return;

			item->m_toggle = false;

			this->SendToggleState(index, item->m_toggle, TOGGLE_ITEM);
			break;
		}
	}
}

void CPC::changeToggleState( int index, int toggle_type )
{
	switch(toggle_type)
	{
	case TOGGLE_SKILL:
		{
			CSkill* skill = this->FindSkill(index);
			if(skill == NULL)
				return;

			if(skill->m_toggle == false)
				skill->m_toggle = true;
			else
				skill->m_toggle = false;

			this->SendToggleState(index, skill->m_toggle, TOGGLE_SKILL);
			break;
		}		
	case TOGGLE_ITEM:
		{
			CItem* item = this->m_inventory.FindByVirtualIndex(index);
			if(item == NULL)
				return;

			if(item->m_toggle == false)
				item->m_toggle = true;
			else
				item->m_toggle = false;

			this->SendToggleState(index, item->m_toggle, TOGGLE_ITEM);
			break;
		}
	}
}

void CPC::RemovePetSkillFromQuickSlot()
{
	int i, j;
	for (i=0; i < QUICKSLOT_PAGE_NUM; i++)
	{
		for (j=0; j < QUICKSLOT_MAXSLOT; j++)
		{
			CSkill* skill = gserver->m_skillProtoList.Create(m_quickSlot[i].m_skillType[j]);
			if(skill == NULL)
				continue;

			if(skill->m_proto->getJob() == JOB_PET)
			{
				m_quickSlot[i].m_skillType[j] = -1;
				m_quickSlot[i].m_slotType[j] = QUICKSLOT_TYPE_EMPTY;

				CNetMsg::SP rmsg(new CNetMsg);
				QuickSlotAddMsg(rmsg, this, (char)i, (char)j);
				SEND_Q(rmsg, m_desc);
			}
		}
	}
}

void CPC::RemoveApetSkillFromQuickSlot()
{
	int i, j;
	for (i=0; i < QUICKSLOT_PAGE_NUM; i++)
	{
		for (j=0; j < QUICKSLOT_MAXSLOT; j++)
		{
			CSkill* skill = gserver->m_skillProtoList.Create(m_quickSlot[i].m_skillType[j]);
			if(skill == NULL)
				continue;

			if(skill->m_proto->getJob() == JOB_APET)
			{
				m_quickSlot[i].m_skillType[j] = -1;
				m_quickSlot[i].m_slotType[j] = QUICKSLOT_TYPE_EMPTY;

				CNetMsg::SP rmsg(new CNetMsg);
				QuickSlotAddMsg(rmsg, this, (char)i, (char)j);
				SEND_Q(rmsg, m_desc);
			}
		}
	}
}

void CPC::SendHolyWaterStateMsg( CItem* item )
{
	this->holy_water_item = item;
	int holyWaterIndex = -1;
	
	if(item != NULL)
		holyWaterIndex = this->holy_water_item->getDBIndex();

	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::holyWaterStateMsg(rmsg, this->m_index, holyWaterIndex);
		this->m_pArea->SendToCell(rmsg, this, true);
	}
}

bool CPC::isWarZone()
{
	if(this->m_pArea == NULL)
		return false;

	if(this->m_pArea->m_zone == NULL)
		return false;

	int zone_index = this->m_pArea->m_zone->m_index;

	CWarCastle* castle = NULL;

	//공성 지역에 존재하는 캐릭터 및 가능 레벨 체크
	if(zone_index == ZONE_MERAC)
	{
		castle = CWarCastle::GetCastleObject(ZONE_MERAC);
		if(castle == NULL)
			return false;
	}
	else if (zone_index == ZONE_DRATAN)
	{
		castle = CWarCastle::GetCastleObject(ZONE_DRATAN);
		if(castle == NULL)
			return false;
	}
	else
	{
		return false;
	}

	if(castle->GetState() != WCSF_WAR_CASTLE)
		return false;

	if(this->GetJoinFlag(this->m_pZone->m_index) != 0)
	{
		if ( this->GetMapAttr() & MATT_WAR || this->m_pZone->IsWarZone(GET_X(this), GET_Z(this)) == true ) 
		{
			return true;
		}
	}

	return false;
}

void CPC::RemoveAllGuildSkillFromQuickSlot()
{
	int i, j;
	for (i=0; i < QUICKSLOT_PAGE_NUM; i++)
	{
		for (j=0; j < QUICKSLOT_MAXSLOT; j++)
		{
			const CSkillProto* proto = gserver->m_skillProtoList.Find(m_quickSlot[i].m_skillType[j]);
			if(proto == NULL)
				continue;

			if(proto->m_flag & SF_GUILD)
			{
				m_quickSlot[i].m_skillType[j] = -1;
				m_quickSlot[i].m_slotType[j] = QUICKSLOT_TYPE_EMPTY;

				CNetMsg::SP rmsg(new CNetMsg);
				QuickSlotAddMsg(rmsg, this, (char)i, (char)j);
				SEND_Q(rmsg, m_desc);
			}
		}
	}
}
