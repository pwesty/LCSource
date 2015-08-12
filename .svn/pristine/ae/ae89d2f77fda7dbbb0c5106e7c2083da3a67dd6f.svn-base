#include "stdhdrs.h"

#include "CmdMsg.h"
#include "Server.h"
#include "../ShareLib/DBCmd.h"
#include "Exp.h"
#include "WarCastle.h"
#include "Log.h"
#include "../ShareLib/packetType/ptype_appear.h"
#include "../ShareLib/packetType/ptype_old_do_friend.h"

void AppearMsg(CNetMsg::SP& msg, CCharacter* _ch, bool bNew, bool bAction)
{
	if (_ch == NULL || _ch->m_bVisible == false)
	{
		msg->Init();
		return ;
	}

	if (_ch->m_maxHP <= 0)
		_ch->m_maxHP = 1;

	switch (_ch->m_type)
	{
	case MSG_CHAR_PC:
		{
			CPC* ch = TO_PC(_ch);
			UpdateClient::appearPc* packet = reinterpret_cast<UpdateClient::appearPc*>(msg->m_buf);
			packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
			packet->subType = MSG_SUB_UPDATE_APPEAR_PC;

			packet->bNew = bNew;
			packet->charType = GET_CHAR_TYPE(ch);
			packet->charIndex = ch->m_index;
			memcpy(packet->charName, ch->GetName(), sizeof(packet->charName));
			packet->charName[MAX_CHAR_NAME_LENGTH] = '\0';
			packet->job = ch->m_job;
			packet->job2 = ch->m_job2;
			packet->hairStyle = ch->m_hairstyle;
			packet->faceStyle = ch->m_facestyle;
			packet->x = GET_X(ch);
			packet->z = GET_Z(ch);
			packet->h = GET_H(ch);
			packet->r = GET_R(ch);
			packet->yLayer = GET_YLAYER(ch);
			packet->hp = ch->m_hp;
			packet->maxHp = ch->m_maxHP;
			packet->playerState = ch->GetPlayerState();
			packet->pkPenalty = ch->m_pkPenalty;
			packet->pkName = ch->GetPKName();
			packet->plusEffect = ch->m_plusEffect;

			for (int i = COSTUME2_WEARING_START, c = 0; i < COSTUME2_WEARING_START+MAX_COSTUME_WEARING; ++i, ++c)
			{
				if (ch->m_wearInventory.wearItemInfo[i])
					packet->costume2[c] = ch->m_wearInventory.wearItemInfo[i]->getDBIndex();
				else
					packet->costume2[c] = -1;
			}

			for (int i = WEARING_SHOW_START; i <= WEARING_BOOTS; i++)
			{
				if (ch->m_wearInventory.wearItemInfo[i])
				{
					packet->wearingShowList[i].dbIndex = ch->m_wearInventory.wearItemInfo[i]->getDBIndex();
					packet->wearingShowList[i].plus = ch->m_wearInventory.wearItemInfo[i]->getPlus();
				}
				else
				{
					packet->wearingShowList[i].dbIndex = -1;
					packet->wearingShowList[i].plus = 0;
				}
			}

			if ( ch->m_wearInventory.wearItemInfo[WEARING_BACKWING] )
			{
				packet->backwing.dbIndex = ch->m_wearInventory.wearItemInfo[WEARING_BACKWING]->getDBIndex();
				packet->backwing.plus = ch->m_wearInventory.wearItemInfo[WEARING_BACKWING]->getPlus();
			}
			else
			{
				packet->backwing.dbIndex = -1;
				packet->backwing.plus = 0;
			}

			for (int i = WEARING_ACCESSORY1, c = 0; i <= WEARING_ACCESSORY3; ++i, ++c)
			{
				if (ch->m_wearInventory.wearItemInfo[i])
					packet->accessory[c] = ch->m_wearInventory.wearItemInfo[i]->getDBIndex();
				else
					packet->accessory[c] = -1;
			}

			if (ch->m_personalshop)
			{
				packet->personalshop.type = ch->m_personalshop->GetType();
				memcpy(packet->personalshop.name, ch->m_personalshop->GetName(), sizeof(packet->personalshop.name));
				packet->personalshop.name[PS_MAX_SHOPNAME] = '\0';
			}
			else
			{
				packet->personalshop.type = PST_NOSHOP;
				packet->personalshop.name[0] = '\0';
			}

			if (ch->m_guildInfo && ch->m_guildInfo->guild())
			{
				packet->guildinfo.index = ch->m_guildInfo->guild()->index();
				memcpy(packet->guildinfo.name, ch->m_guildInfo->guild()->name(), sizeof(packet->guildinfo.name));
				packet->guildinfo.name[MAX_GUILD_NAME_LENGTH] = '\0';
				packet->guildinfo.pos = ch->m_guildInfo->pos();
				packet->guildinfo.guildMark = ch->m_guildInfo->GetGuildMark();

#ifdef DEV_GUILD_MARK
				CGuild* pGuild = ch->m_guildInfo->guild();
				packet->guildinfo.guildMark_row = pGuild->GetGuildMarkRow();
				packet->guildinfo.guildMark_col = pGuild->GetGuildMarkCol();
				packet->guildinfo.guildMark_bg_row = pGuild->GetBackgroundRow();
				packet->guildinfo.guildMark_bg_col = pGuild->GetBackgroundCol();;
#endif
			}
			else
			{
				packet->guildinfo.index = -1;
				packet->guildinfo.name[0] = '\0';
				packet->guildinfo.pos = -1;
				packet->guildinfo.guildMark = 0;

#ifdef DEV_GUILD_MARK
				packet->guildinfo.guildMark_row = -1;
				packet->guildinfo.guildMark_col = -1;
				packet->guildinfo.guildMark_bg_row = -1;
				packet->guildinfo.guildMark_bg_col = -1;
#endif
			}

			packet->changeIndex = ch->m_changeIndex;
			packet->fame = ch->m_fame;
			packet->mapAttr = ch->GetMapAttr();
			packet->merac_join_flag = ch->GetJoinFlag(ZONE_MERAC);
			packet->dratan_join_flag = ch->GetJoinFlag(ZONE_DRATAN);

			CPet* pet = ch->GetPet();
			if (pet && ch->m_wearInventory.wearItemInfo[WEARING_PET])
			{
				if (pet->IsMount())
					packet->pet.grade = pet->GetPetTypeGrade();
				else
					packet->pet.grade = 0;

				packet->pet.color = pet->GetPetColor();
			}
			else
			{
				packet->pet.grade = 0;
				packet->pet.color = 0;
			}

			packet->evocationIndex = ch->m_evocationIndex;

			if (ch->IsSetPlayerState(PLAYER_STATE_SUPPORTER))
				packet->isPlayerStateSupporter = true;
			else
				packet->isPlayerStateSupporter = false;

			packet->pressCorps = ch->m_bPressCorps;
			packet->expedLabel = ch->GetExpedLabel();		//원정대 표식(PC)

			// 현재 스킬을 쓰고 있다면, 지속 시간이 길다면
			if (ch->m_currentSkill && ch->m_currentSkill->m_state == SKILL_CON_FIRE)
			{
				// AppearMsg를 보냈으므로 유저의 초당 체크에서 skillfireMsg를 보낸다.
				ch->m_bCheckAppear = true;
			}

			packet->currentTitle = ch->m_nCurrentTitle;

			CAPet *apet = ch->GetAPet();
			if( apet && apet->IsMount() )
				packet->mountApetIndex = apet->m_index;
			else
				packet->mountApetIndex = 0;

			packet->petStashManagerEffect = ch->m_petStashManager.getEffect();
			packet->syndicate.type = ch->getSyndicateType();
			packet->syndicate.grade = ch->m_syndicateManager.getSyndicateGrade(ch->getSyndicateType());

			ch->m_assist.AppendAssistToNetStructMsg(packet->assistInfo);

#ifdef PREMIUM_CHAR
			packet->isPremiumChar = ch->m_premiumChar.isActive();
#endif

			msg->setSize(sizeof(UpdateClient::appearPc) + (packet->assistInfo.count * sizeof(tag_assistinfo::tag_list)));
		}
		break;

	case MSG_CHAR_NPC:
		{
			CNPC* ch = TO_NPC(_ch);
			if (ch->m_idNum <= 0 || ch->m_disableTime > 0)
			{
				msg->Init();
				return;
			}

			UpdateClient::appearNpc* packet = reinterpret_cast<UpdateClient::appearNpc*>(msg->m_buf);
			packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
			packet->subType = MSG_SUB_UPDATE_APPEAR_NPC;

			if (ch->m_assist.m_avAddition.attrdef > 0 || ch->m_assist.m_avAddition.attrdef_item > 0)
				ch->m_attrdef = ch->m_assist.getAttrDef();
			else
				ch->m_attrdef = ch->m_proto->m_attrdef;

			if (ch->m_assist.m_avAddition.attratt > 0 || ch->m_assist.m_avAddition.attratt_item > 0)
				ch->m_attratt = ch->m_assist.getAttrAtt();
			else
				ch->m_attratt = ch->m_proto->m_attratt;

			packet->bNew = bNew;
			packet->charType = GET_CHAR_TYPE(ch);
			packet->charVirtualIndex = ch->m_index;
			packet->charDbIndex = ch->m_idNum;
			packet->x = GET_X(ch);
			packet->z = GET_Z(ch);
			packet->h = GET_H(ch);
			packet->r = GET_R(ch);
			packet->yLayer = GET_YLAYER(ch);
			packet->hp = ch->m_hp;
			packet->maxHp = ch->m_maxHP;
			packet->attrdef = ch->m_attrdef;
			packet->attratt = ch->m_attratt;

			packet->mapAttr = ch->GetMapAttr();
			packet->expedLabel = ch->GetExpedLabel();		//원정대 표식(NPC)

			if( ch->GetOwner() && !ch->Check_MobFlag(STATE_MONSTER_PARASITE) )
			{
				packet->ownerIndex = ch->GetOwner()->m_index;

#ifdef SYSTEM_MONSTER_MERCENARY_CARD_EX
				if( ch->Check_MobFlag(STATE_MONSTER_MERCENARY))
					packet->mercenarySize = ch->m_pMercenaryClassData->nSize;
				else
					packet->mercenarySize = 0.0;
#else
				packet->mercenarySize = 0.0;
#endif
			}
			else
			{
				packet->ownerIndex = 0;
				packet->mercenarySize = 0.0;
			}

			ch->m_assist.AppendAssistToNetStructMsg(packet->assistInfo);

			msg->setSize(sizeof(UpdateClient::appearNpc) + (packet->assistInfo.count * sizeof(tag_assistinfo::tag_list)));
		}
		break;

	case MSG_CHAR_PET:
		{
			CPet* ch = TO_PET(_ch);

			UpdateClient::appearPet* packet = reinterpret_cast<UpdateClient::appearPet*>(msg->m_buf);
			packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
			packet->subType = MSG_SUB_UPDATE_APPEAR_PET;

			packet->bNew = bNew;
			packet->charType = GET_CHAR_TYPE(ch);
			packet->charIndex = ch->m_index;

			if (ch->GetOwner())
			{
				packet->ownerIndex = ch->GetOwner()->m_index;
				memcpy(packet->ownerName, ch->GetOwner()->GetName(), sizeof(packet->ownerName));
				packet->ownerName[MAX_CHAR_NAME_LENGTH] = '\0';
			}
			else
			{
				packet->ownerIndex = 0;
				packet->ownerName[0] = '\0';
			}

			packet->grade = ch->GetPetTypeGrade();
			packet->x = GET_X(ch);
			packet->z = GET_Z(ch);
			packet->h = GET_H(ch);
			packet->r = GET_R(ch);
			packet->yLayer = GET_YLAYER(ch);
			packet->hp = ch->m_hp;
			packet->maxHp = ch->m_maxHP;
			packet->mapAttr = ch->GetMapAttr();
			memcpy(packet->name, ch->m_name, sizeof(packet->name));
			packet->name[MAX_CHAR_NAME_LENGTH] = '\0';
			packet->color = ch->GetPetColor();
			packet->turnToNpc = ch->GetPetTurnToNpc();
			packet->turnToNpcSize = ch->GetPetTurnToNpcSize();

			msg->setSize(sizeof(UpdateClient::appearPet));
		}
		break;

	case MSG_CHAR_ELEMENTAL:
		{
			CElemental* ch = TO_ELEMENTAL(_ch);
			if (ch->GetOwner() == NULL)
			{
				msg->Init();
				return;
			}

			UpdateClient::appearElemental* packet = reinterpret_cast<UpdateClient::appearElemental*>(msg->m_buf);
			packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
			packet->subType = MSG_SUB_UPDATE_APPEAR_ELEMENTAL;

			packet->bNew = bNew;
			packet->charType = GET_CHAR_TYPE(ch);
			packet->charIndex = ch->m_index;
			packet->elementalType = ch->GetElementalType();
			packet->ownerIndex = ch->GetOwner()->m_index;
			memcpy(packet->ownerName, ch->GetOwner()->GetName(), sizeof(packet->ownerName));
			packet->x = GET_X(ch);
			packet->z = GET_Z(ch);
			packet->h = GET_H(ch);
			packet->r = GET_R(ch);
			packet->yLayer = GET_YLAYER(ch);
			packet->hp = ch->m_hp;
			packet->maxHp = ch->m_maxHP;
			packet->mapAttr = ch->GetMapAttr();

			// 소환수 크기를 크게 할 것인가? : 엘리멘탈 리스트로 전직하면 커짐
			packet->isBigSize = (ch->GetOwner()->m_job2 == JOB_2ND_ELEMENTALLIST);

			ch->m_assist.AppendAssistToNetStructMsg(packet->assistInfo);

			msg->setSize(sizeof(UpdateClient::appearElemental) + (packet->assistInfo.count * sizeof(tag_assistinfo::tag_list)));
		}
		break;

	case MSG_CHAR_APET:
		{
			CAPet* ch = TO_APET(_ch);

			UpdateClient::appearApet* packet = reinterpret_cast<UpdateClient::appearApet*>(msg->m_buf);
			packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
			packet->subType = MSG_SUB_UPDATE_APPEAR_APET;

			packet->bNew = bNew;
			packet->charType = GET_CHAR_TYPE(ch);
			packet->charIndex = ch->m_index;
			packet->dbIndex = ch->m_pProto->Index();
			packet->level = ch->m_level;
			memcpy(packet->name, ch->m_name, sizeof(packet->name));
			packet->name[MAX_CHAR_NAME_LENGTH] = '\0';
			packet->isMount = ch->m_bMount;
			packet->transSate = ch->m_cTransSate;
			packet->isAction = bAction;

			if( ch->GetOwner() )
			{
				packet->ownerIndex = ch->GetOwner()->m_index;
				memcpy(packet->ownerName, ch->GetOwner()->m_name, sizeof(packet->ownerName));
				packet->ownerName[MAX_CHAR_NAME_LENGTH] = '\0';
			}
			else
			{
				packet->ownerIndex = 0;
				packet->ownerName[0] = '\0';
			}

			packet->protoType = ch->m_pProto->m_Type;
			packet->x = GET_X(ch);
			packet->z = GET_Z(ch);
			packet->h = GET_H(ch);
			packet->r = GET_R(ch);
			packet->yLayer = GET_YLAYER(ch);;

			for( int i = 0; i < APET_WEARPOINT; ++i)
			{
				if (ch->m_wearing[i])
				{
					packet->weaponList[i].dbIndex = ch->m_wearing[i]->getDBIndex();
					packet->weaponList[i].plus = ch->m_wearing[i]->getPlus();
				}
				else
				{
					packet->weaponList[i].dbIndex = -1;
					packet->weaponList[i].plus = 0;
				}
			}

			packet->hp = ch->m_hp;
			packet->maxHp = ch->m_maxHP;
			packet->mapAttr = ch->GetMapAttr();

			msg->setSize(sizeof(UpdateClient::appearApet));
		}
		break;

	default:
		msg->Init();
		break;
	}
}

void NameChangeGuildMemberMsg(CNetMsg::SP& msg, int index, const char* name, char job )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_GUILDMEMBERNAMECHANGE
				<< index
				<< name
				<< job;
}

void NameChangeFriendMsg(CNetMsg::SP& msg, int index, const char* name, char job )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_FRIENDNAMECHANGE
				<< index
				<< name
				<< job;
}
void NameChangeRepMsg(CNetMsg::SP& msg, MSG_EX_NAMECHANGE_ERROR_TYPE type, const char* name, char bguild)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_NAMECHANGE
				<< (unsigned char) type
				<< name
				<< bguild;
}

void CashItemBalanceRepMsg(CNetMsg::SP& msg, MSG_EX_CASHITEM_ERROR_TYPE errorcode, int cashBalance)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASHITEM
				<< (unsigned char) MSG_EX_CASHITEM_BALANCE_REP
				<< (unsigned char) errorcode
				<< cashBalance;
}

void CashItemPurchaseRepMsg(CNetMsg::SP& msg, MSG_EX_CASHITEM_ERROR_TYPE errorcode, int cashBalance)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASHITEM
				<< (unsigned char) MSG_EX_CASHITEM_PURCHASE_REP
				<< (unsigned char) errorcode
				<< cashBalance;
}

void CashItemBringRepMsg(CNetMsg::SP& msg, MSG_EX_CASHITEM_ERROR_TYPE errorcode, bool bPresent)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASHITEM;
	if( bPresent )
		RefMsg(msg) << (unsigned char) MSG_EX_CASHITEM_GIFT_RECV_REP;
	else
		RefMsg(msg) << (unsigned char) MSG_EX_CASHITEM_BRING_REP;

	RefMsg(msg) << (unsigned char) errorcode;
}

void CashItemPurchaseListRepMsg(CNetMsg::SP& msg, MSG_EX_CASHITEM_ERROR_TYPE errorcode, int count, int idx[], int ctid[])
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASHITEM
				<< (unsigned char) MSG_EX_CASHITEM_PURCHASELIST_REP
				<< (unsigned char) errorcode
				<< count;

	if(count == 0) return;

	for(int i = 0; i < count; i++)
	{
		RefMsg(msg) << idx[i]
					<< ctid[i];
	}
}

void DisappearMsg(CNetMsg::SP& msg, CCharacter* ch)
{
	if (ch == NULL || ch->m_bVisible == false)
	{
		msg->Init();
		return ;
	}

	UpdateClient::charDisappear* packet = reinterpret_cast<UpdateClient::charDisappear*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_DISAPPERAR;
	packet->charType = GET_CHAR_TYPE(ch);
	packet->charIndex = ch->m_index;

	msg->setSize(sizeof(UpdateClient::charDisappear));
}

void DisappearMsgForClientEffect(CNetMsg::SP& msg, CCharacter* ch, int owner_index)
{
	if (ch == NULL || ch->m_bVisible == false)
	{
		msg->Init();
		return ;
	}

	UpdateClient::charDisappearForClientEffect* packet = reinterpret_cast<UpdateClient::charDisappearForClientEffect*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_DISAPPERAR_FOR_CLIENT_EFFECT;
	packet->charIndex = ch->m_index;
	packet->owner_index = owner_index;
	msg->setSize(sizeof(UpdateClient::charDisappearForClientEffect));
}

void DisappearAllMsg(CNetMsg::SP& msg)
{
	UpdateClient::charDisappearAll* packet = reinterpret_cast<UpdateClient::charDisappearAll*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_DISAPPERAR_ALL;
	msg->setSize(sizeof(UpdateClient::charDisappearAll));
}

void TargetClearMsg(CNetMsg::SP& msg, int targetIndex)
{
	UpdateClient::targetClear* packet = reinterpret_cast<UpdateClient::targetClear*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_TARGET_CLEAR;
	packet->targetIndex = targetIndex;				//targetIndex가 -1이면 내가 들고 있는 타겟을 비교하지않고 무조껀 해제 시킨다.
	msg->setSize(sizeof(UpdateClient::targetClear));
}

void StatusMsg(CNetMsg::SP& msg, CPC* ch)
{
	UpdateClient::charStatus* packet = reinterpret_cast<UpdateClient::charStatus*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_STATUS;

	// 옵션에 의한 스탯 상승 수치 적용
	int str = ch->m_statpt_str + ch->m_opStr + ch->m_artiStr;
	int dex = ch->m_statpt_dex + ch->m_opDex + ch->m_artiDex;
	int intl = ch->m_statpt_int + ch->m_opInt + ch->m_artiInt;
	int con = ch->m_statpt_con + ch->m_opCon + ch->m_artiCon;

	int attack = 0;
	int magic = 0;
	int defense = 0;
	int resist = 0;

	int attack_diff = 0;
	int magic_diff = 0;
	int defense_diff = 0;
	int resist_diff = 0;

	extern int base_stat[JOBCOUNT][4];
	extern int nBaseHitTypeProb[2][6];

	MSG_DAMAGE_TYPE meleetype;

	// 물리회피계산, 물리명중계산
	if(ch->m_wearInventory.wearItemInfo[WEARING_WEAPON])
	{
		if(ch->m_wearInventory.wearItemInfo[WEARING_WEAPON]->IsRangeWeapon())
			meleetype = MSG_DAMAGE_RANGE;
		else
			meleetype = MSG_DAMAGE_MELEE;
	}
	else
		meleetype = MSG_DAMAGE_MELEE;

	magic = ch->m_int * 10 / 15 + ch->m_eqMagic + ch->m_opMagic;

	defense = ch->m_eqDefense + ch->m_opDMelee + ch->m_opDRange;
	resist = ch->m_eqResist + ch->m_opResist;

	if (ch->m_wearInventory.wearItemInfo[WEARING_WEAPON] && ch->m_wearInventory.wearItemInfo[WEARING_WEAPON]->IsRangeWeapon())
		attack += ch->m_eqRange + ch->m_opRange;
	else
		attack += ch->m_eqMelee + ch->m_opMelee;

	attack += ch->m_avPassiveAddition.attack + attack * ch->m_avPassiveRate.attack / SKILL_RATE_UNIT;
	magic += ch->m_avPassiveAddition.magic + magic * ch->m_avPassiveRate.magic / SKILL_RATE_UNIT;
	defense += ch->m_avPassiveAddition.defense + defense * ch->m_avPassiveRate.defense / SKILL_RATE_UNIT;
	resist += ch->m_avPassiveAddition.resist + resist * ch->m_avPassiveRate.resist / SKILL_RATE_UNIT;
	int addattack80 = ch->m_avPassiveAddition.attack80;
	int rateattack80 = attack * ch->m_avPassiveRate.attack80 / SKILL_RATE_UNIT;
	if (addattack80 > 80)
		addattack80 = 80;
	if (rateattack80 > 80)
		rateattack80 = 80;
	attack += addattack80 + rateattack80;

// NPC 공격력 증가 표시는 무조건 합니다.
	attack += ch->m_avPassiveAddition.npcAttack + attack * ch->m_avPassiveAddition.npcAttack / SKILL_RATE_UNIT ;
	attack += ch->m_avPassiveAddition.npcMagic + magic * ch->m_avPassiveAddition.npcMagic / SKILL_RATE_UNIT ;

	attack_diff = attack;
	magic_diff = magic;
	defense_diff = defense;
	resist_diff = resist;

	addattack80 = ch->m_assist.m_avAddition.attack80;
	rateattack80 = attack * ch->m_assist.m_avRate.attack80 / SKILL_RATE_UNIT;
	if (addattack80 > 80)
		addattack80 = 80;
	if (rateattack80 > 80)
		rateattack80 = 80;
	attack += addattack80 + rateattack80;

	// 버프효과 표시
	if (ch->m_wearInventory.wearItemInfo[WEARING_WEAPON] && ch->m_wearInventory.wearItemInfo[WEARING_WEAPON]->IsRangeWeapon())
	{
		attack += ch->m_assist.m_avAddition.attack + attack * ch->m_assist.m_avRate.attack / SKILL_RATE_UNIT
				  + ch->m_assist.m_avAddition.attack_dam_range
				  +  attack * ch->m_assist.m_avRate.attack_dam_range / SKILL_RATE_UNIT;
	}
	else
	{
		attack += ch->m_assist.m_avAddition.attack + attack * ch->m_assist.m_avRate.attack / SKILL_RATE_UNIT
				  + ch->m_assist.m_avAddition.attack_dam_melee
				  + attack * ch->m_assist.m_avRate.attack_dam_melee / SKILL_RATE_UNIT;
	}

	magic += ch->m_assist.m_avAddition.magic + magic * ch->m_assist.m_avRate.magic / SKILL_RATE_UNIT;
	defense += ch->m_assist.m_avAddition.defense + defense * ch->m_assist.m_avRate.defense / SKILL_RATE_UNIT;
	resist += ch->m_assist.m_avAddition.resist + resist * ch->m_assist.m_avRate.resist / SKILL_RATE_UNIT;

	if(ch->isWarZone())
	{
		defense += ch->m_assist.m_avAddition.war_defence;
		resist += ch->m_assist.m_avAddition.war_resist;
	}

// NPC 공격력 증가 표시는 무조건 합니다.
	attack += ch->m_assist.m_avAddition.npcAttack + attack * ch->m_assist.m_avRate.npcAttack / SKILL_RATE_UNIT;
	magic += ch->m_assist.m_avAddition.npcMagic + magic * ch->m_assist.m_avRate.npcMagic / SKILL_RATE_UNIT;

	// 아이템 페널티 관련
	int adef = ch->GetAttackLevel() - ch->m_level;
	int ddef = ch->GetDefenseLevel() - ch->m_level;

	if (adef >= 5 && adef <= 8)
	{
		attack	/= 2;
		magic	/= 2;
		attack_diff /= 2;
		magic_diff /= 2;
	}
	else if (adef >= 9 && adef <= 12)
	{
		attack = attack * 3 / 10;
		magic = magic * 3 / 10;
		attack_diff = attack_diff * 3 / 10;
		magic_diff = magic_diff * 3 / 10;
	}
	else if (adef >= 13)
	{
		attack /= 10;
		magic /= 10;
		attack_diff /= 10;
		magic_diff /= 10;
	}

	if (ddef >= 6 && ddef <= 10)
	{
		defense /= 2;
		defense_diff /= 2;
	}
	else if (ddef >= 11 && ddef <= 15)
	{
		defense = defense * 3 / 10;
		defense_diff = defense_diff * 3 / 10;
	}
	else if (ddef >= 16)
	{
		defense /= 10;
		defense_diff /= 10;
	}

	attack_diff = attack - attack_diff;
	magic_diff = magic - magic_diff;
	defense_diff = defense - defense_diff;
	resist_diff = resist - resist_diff;

	if (ch->m_wearInventory.wearItemInfo[WEARING_WEAPON] && ch->m_wearInventory.wearItemInfo[WEARING_WEAPON]->IsRangeWeapon())
		attack += ch->GetRangeLevelBonus();
	else
		attack += ch->GetMeleeLevelBonus();
	magic += ch->GetMagicLevelBonus();
	defense += ch->GetDefenseLevelBonus();
	resist += ch->GetResistLevelBonus();

	if (ch->m_wearInventory.wearItemInfo[WEARING_WEAPON] && ch->m_wearInventory.wearItemInfo[WEARING_WEAPON]->m_itemProto == NULL)
	{
		// TODO : DEBUGING용
		char debug_buf[MAX_STRING_LENGTH] = {0,};
		sprintf(debug_buf, "ERR:ITEM:PROTO:%s:%s:%d", (const char *)ch->m_name, (const char *)ch->m_nick, ch->m_wearInventory.wearItemInfo[WEARING_WEAPON]->getDBIndex());
		LOG_INFO(debug_buf);
	}
	else
	{
		if (ch->m_wearInventory.wearItemInfo[WEARING_WEAPON] && ch->m_wearInventory.wearItemInfo[WEARING_WEAPON]->GetUsingStat() == USING_DEX)
			attack += ch->m_dex * 10 / 15;
		else
			attack += ch->m_str * 10 / 15;
	}

	// 강신 적용 : 물리방어
	switch (ch->m_evocationIndex)
	{
	case EVOCATION_HELLOUND:
		defense = defense * 10 / 11;
		break;
	case EVOCATION_ELENEN:
		defense = defense * 10 / 7;
		break;
	}

	// 강신 적용 : 마공
	switch (ch->m_evocationIndex)
	{
	case EVOCATION_HELLOUND:
		magic = magic;
		break;

	case EVOCATION_ELENEN:
		magic = magic * 10 / 13;
		break;
	}

	// 강신 적용 : 마법방어
	switch (ch->m_evocationIndex)
	{
	case EVOCATION_HELLOUND:
		resist = resist;
		break;
	case EVOCATION_ELENEN:
		resist = resist * 13 / 10;
		break;
	}

	float walkSpeed = ch->m_walkSpeed;
	float runSpeed = ch->GetRunSpeed();
	float flySpeed = ch->m_flySpeed;

	if(ch->m_pZone->m_index == ZONE_DRATAN_CASTLE_DUNGEON && IS_PC(ch))
	{
		CDratanCastle * pCastle = CDratanCastle::CreateInstance();
		if(pCastle->m_dvd.GetZone() && pCastle->m_dvd.GetEnvRate() >= 90)
		{
			walkSpeed = walkSpeed + walkSpeed*0.1;
			runSpeed = runSpeed + runSpeed*0.1;
			flySpeed = flySpeed + flySpeed*0.1;
		}
	}

	char mountPet = 0;
	char colorType = 0;
	if (ch->m_wearInventory.wearItemInfo[WEARING_PET])
	{
		CPet* pet = ch->GetPet();
		if (pet)
		{
			if (pet->IsMount())
			{
				mountPet = pet->GetPetTypeGrade();
				walkSpeed = pet->m_walkSpeed;
				runSpeed = pet->GetRunSpeed();
				colorType = pet->GetPetColor();
			}
		}
	}

	///////////////////////////////////////////////////////
	float avoid_magic_org = 0.0, avoid_magic = 0.0;			// 마법회피
	float avoid_melee_org = 0.0, avoid_melee = 0.0;			// 물리회피
	float fhitrate_magic_org = 0.0, fhitrate_magic = 0.0;	// 마법명중
	float fhitrate_melee_org = 0.0, fhitrate_melee = 0.0;	// 물리명중


	avoid_melee = ch->GetAvoid(ch, meleetype);
	fhitrate_melee = ch->GetHitrate(ch, meleetype);

	switch (meleetype)
	{
	case MSG_DAMAGE_MELEE:
		avoid_melee += ch->m_opMeleeAvoid + ch->m_avPassiveAddition.Avoid_2034;
		avoid_melee += ch->m_avPassiveAddition.avoid + (avoid_melee * ch->m_avPassiveRate.avoid / SKILL_RATE_UNIT);
		fhitrate_melee += ch->m_opMeleeHitRate + ch->m_avPassiveAddition.HitRate_2033;
		fhitrate_melee += ch->m_avPassiveAddition.hitrate + (fhitrate_melee * ch->m_avPassiveRate.hitrate / SKILL_RATE_UNIT);
		break;
	case MSG_DAMAGE_RANGE:
		avoid_melee += ch->m_opRangeAvoid + ch->m_avPassiveAddition.Avoid_2034;
		avoid_melee += ch->m_avPassiveAddition.avoid + (avoid_melee * ch->m_avPassiveRate.avoid / SKILL_RATE_UNIT);
		fhitrate_melee += ch->m_opRangeHitRate + ch->m_avPassiveAddition.HitRate_2033;
		fhitrate_melee += ch->m_avPassiveAddition.hitrate + (fhitrate_melee * ch->m_avPassiveRate.hitrate / SKILL_RATE_UNIT);
		break;
	default:
		break;
	}

	avoid_melee_org = avoid_melee;			// 물리회피계산
	avoid_melee += ch->m_assist.m_avAddition.avoid + (avoid_melee * ch->m_assist.m_avRate.avoid / SKILL_RATE_UNIT);

	fhitrate_melee_org = fhitrate_melee;	// 물리명중계산
	fhitrate_melee += ch->m_assist.m_avAddition.hitrate + (fhitrate_melee * ch->m_assist.m_avRate.hitrate / SKILL_RATE_UNIT);

	// 마법회피계산
	avoid_magic = ch->GetAvoid(ch, MSG_DAMAGE_MAGIC);
	avoid_magic += ch->m_opResistAvoid + ch->m_avPassiveAddition.Avoid_2034;
	avoid_magic += ch->m_avPassiveAddition.magicavoid + (avoid_magic * ch->m_avPassiveRate.magicavoid / SKILL_RATE_UNIT);

	avoid_magic_org = avoid_magic;
	avoid_magic += ch->m_assist.m_avAddition.magicavoid + (avoid_magic * ch->m_assist.m_avRate.magicavoid / SKILL_RATE_UNIT);

	// 마법명중계산
	fhitrate_magic = ch->GetHitrate(ch, MSG_DAMAGE_MAGIC);
	fhitrate_magic += ch->m_opMagicHitRate + ch->m_avPassiveAddition.HitRate_2033;
	fhitrate_magic += ch->m_avPassiveAddition.magichitrate + (fhitrate_magic * ch->m_avPassiveRate.magichitrate / SKILL_RATE_UNIT);

	fhitrate_magic_org = fhitrate_magic;
	fhitrate_magic += ch->m_assist.m_avAddition.magichitrate + (fhitrate_magic * ch->m_assist.m_avRate.magichitrate / SKILL_RATE_UNIT);

	// 크리티컬, 데들리
	int critical = 0, deadly = 0;
	int critical_org = 0, deadly_org = 0;
	int nOffenseDexBase = 0;
	nOffenseDexBase  = base_stat[ch->m_job][1];
	critical = nBaseHitTypeProb[0][4];
	deadly = nBaseHitTypeProb[0][5];

	critical += (ch->m_dex - nOffenseDexBase) * 9 / 10;
	deadly += (ch->m_dex - nOffenseDexBase) * 6 / 10;

	critical += critical * ch->m_opAttackCritical / 100;
	deadly += deadly * ch->m_opAttackDeadly / 100;

	critical += ch->m_avPassiveAddition.critical + (critical * ch->m_avPassiveRate.critical / SKILL_RATE_UNIT);
	critical_org = critical;
	critical += ch->m_assist.m_avAddition.critical + (critical * ch->m_assist.m_avRate.critical / SKILL_RATE_UNIT);

	deadly += ch->m_avPassiveAddition.deadly + (deadly * ch->m_avPassiveRate.deadly / SKILL_RATE_UNIT);
	deadly_org = deadly;
	deadly += ch->m_assist.m_avAddition.deadly + (deadly * ch->m_assist.m_avRate.deadly / SKILL_RATE_UNIT);
	////////////////////////////////////////////////////////////////////////////////////

	if (ch->m_assist.m_avAddition.attrdef > 0 || ch->m_assist.m_avAddition.attrdef_item > 0)
	{
		ch->m_attrdef = ch->m_assist.getAttrDef();
	}
	else ch->m_attrdef = ch->m_opAttrdef;

	if (ch->m_assist.m_avAddition.attratt > 0 || ch->m_assist.m_avAddition.attratt_item > 0)
	{
		ch->m_attratt = ch->m_assist.getAttrAtt();
	}
	else ch->m_attratt = ch->m_opAttratt;

	packet->level = ch->m_level;
	packet->exp = ch->m_exp;
	packet->levelupExp = GetLevelupExp(ch->m_level);
	packet->hp = ch->m_hp;
	packet->maxHp = ch->m_maxHP;
	packet->maxHpOrg = ch->m_maxHP_org;
	packet->mp = ch->m_mp;
	packet->maxMp = ch->m_maxMP;
	packet->maxMpOrg = ch->m_maxMP_org;
	packet->attrDef = ch->m_attrdef;
	packet->attrAtt = ch->m_attratt;
	packet->str = str;
	packet->dex = dex;
	packet->intl = intl;
	packet->con	= con;
	packet->opStr = ch->m_opStr;
	packet->opDex = ch->m_opDex;
	packet->opInt = ch->m_opInt;
	packet->opCon = ch->m_opCon;
	packet->attack = attack;
	packet->attack_diff = attack_diff; // (증감값)
	packet->magic = magic;
	packet->magic_diff = magic_diff; // (증감값)
	packet->defense = defense;
	packet->defense_diff = defense_diff; // (증감값)
	packet->resist = resist;
	packet->resist_diff = resist_diff; // (증감값)
	packet->avoid_melee = avoid_melee;	// 물리회피(최종)
	packet->avoid_melee_org = avoid_melee_org; 	// 물리회피(추가되는거 뺀 값)
	packet->avoid_magic = avoid_magic;	// 마법회피(최종)
	packet->avoid_magic_org = avoid_magic_org; // 마법회피(추가되는거 뺀 값)
	packet->fhitrate_melee = fhitrate_melee;
	packet->fhitrate_melee_org = fhitrate_melee_org;
	packet->fhitrate_magic = fhitrate_magic;
	packet->fhitrate_magic_org = fhitrate_magic_org;

	packet->critical = critical;
	packet->critical_org = critical_org;
	packet->deadly = deadly;
	packet->deadly_org = deadly_org;

	packet->skillPoint = ch->m_skillPoint;
	packet->walkSpeed = walkSpeed;

	packet->party_arti[eArti_Str] = ch->m_artiStr;
	packet->party_arti[eArti_Dex] = ch->m_artiStr;
	packet->party_arti[eArti_Int] = ch->m_artiStr;
	packet->party_arti[eArti_Con] = ch->m_artiStr;

	if( ch->IsSetPlayerState(PLAYER_STATE_FLYING))
	{
		packet->run_or_fly_speed = flySpeed;
		packet->run_or_fly_speed_org = ch->m_flySpeed; // (추가되는거 뺀 값)
	}
	else
	{
		packet->run_or_fly_speed = runSpeed;
		packet->run_or_fly_speed_org = ch->m_runSpeed_org; // (추가되는거 뺀 값)
	}

	packet->attackSpeed = ch->m_attackSpeed;
	packet->attackSpeedOrg = ch->m_attackSpeed_org;  // (추가되는거 뺀 값)

	packet->magicSpeed = ch->m_magicSpeed;
	packet->pkName = ch->GetPKName();
	packet->pkPenalty = ch->m_pkPenalty;
	packet->pkCount  = ch->m_pkCount;
	packet->fame = ch->m_fame;
	packet->attackRange = ch->m_attackRange;

	packet->joinForMerac = ch->GetJoinFlag(ZONE_MERAC);
	packet->joinForDratan = ch->GetJoinFlag(ZONE_DRATAN);

	packet->skillSpeed = ch->m_skillSpeed;
	packet->mapAttr = ch->GetMapAttr();
	packet->mountPet = mountPet;
	packet->colorType = colorType;
	packet->evocationIndex = ch->m_evocationIndex;
	packet->remainEvocation = ch->GetRemainEvocation(true);

	packet->bPressCorps = ch->m_bPressCorps;
	packet->prob = ch->m_opDecreaseSkillDelay + ch->m_skillCooltime;
	packet->opDecreaseSkillMP = ch->m_opDecreaseSkillMP;

	packet->ep = ch->m_ep;
	packet->maxEp = ch->m_maxEP;

	packet->opMoney_buy_addition = ch->m_avPassiveAddition.money_buy;
	packet->opMoney_buy_rate = ch->m_avPassiveRate.money_buy;
	packet->opMoney_sell_addition = ch->m_avPassiveAddition.money_sell;
	packet->opMoney_sell_rate = ch->m_avPassiveRate.money_sell;

	msg->setSize(sizeof(UpdateClient::charStatus));
}

void AtMsg(CNetMsg::SP& msg, CPC* ch)
{
	UpdateClient::charAtMsg* packet = reinterpret_cast<UpdateClient::charAtMsg*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_AT;

	packet->charIndex = ch->m_index;
	memcpy(packet->charName, ch->GetName(), sizeof(packet->charName));
	packet->charName[MAX_CHAR_NAME_LENGTH] = '\0';
	packet->job = ch->m_job;
	packet->job2 = ch->m_job2;
	packet->hairstyle = ch->m_hairstyle;
	packet->facestyle = ch->m_facestyle;
	packet->zoneIndex = ch->m_pZone->m_index;
	packet->areaIndex = ch->m_pArea->m_index;
	packet->x = GET_X(ch);
	packet->z = GET_Z(ch);
	packet->h = GET_H(ch);
	packet->r = GET_R(ch);
	packet->yLayer = GET_YLAYER(ch);
	packet->userIndex = ch->m_desc->m_index;
	packet->guildoutdate = ch->m_guildoutdate;
	packet->plusEffect = ch->m_plusEffect;
	packet->mapAttr = ch->GetMapAttr();

	ch->m_secretkey = GetRandom(10,100);
	packet->key = (ch->m_index + ch->m_secretkey) << 1;
	packet->currentTitle = ch->m_nCurrentTitle;

#ifdef NO_CHATTING
	packet->chatingFlag = ch->m_nflag;
#endif

#ifdef ENABLE_SUBJOB
	packet->jobSub = ch->m_jobSub;
#endif

	msg->setSize(sizeof(UpdateClient::charAtMsg));
}

void SayMsg(CNetMsg::SP& msg, MSG_CHAT_TYPE chatType, int sender_index, const char* sender_name, const char* receiver_name, const char* chat,int nGroup)
{
	msg->Init(MSG_CHAT);
	RefMsg(msg) << (unsigned char)chatType
				<< sender_index
				<< sender_name
				<< receiver_name
				<< chat
				<< nGroup;
}

void SayChannelMsg( CNetMsg::SP& msg, unsigned char chattype, int sender_index ,const char* sender_name, unsigned char channel, const char* chat  )
{
	msg->Init(MSG_CHAT);
	RefMsg(msg) << chattype
				<< sender_index
				<< sender_name
				<< channel
				<< chat ;
}

void FailMsg(CNetMsg::SP& msg, MSG_FAIL_TYPE failtype)
{
	msg->Init(MSG_FAIL);
	RefMsg(msg) << (unsigned char)failtype;
}

void DamageMsg(CNetMsg::SP& msg, CCharacter* ch, CCharacter* tch, MSG_DAMAGE_TYPE damageType, int skillidx, int damage, char flag, int holyItemIndex)
{
	UpdateClient::charDamage* packet = reinterpret_cast<UpdateClient::charDamage*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_DAMAGE;

	packet->charType = GET_CHAR_TYPE(ch);
	packet->charIndex = ch->m_index;
	packet->damageType = damageType;
	packet->skillIndex = skillidx;
	packet->targetType = GET_CHAR_TYPE(tch);
	packet->targetIndex = tch->m_index;
	packet->targetHp = tch->m_hp;
	packet->targetMp = tch->m_mp;
	packet->damage = damage;
	packet->holyitemIndex = holyItemIndex;

	// 공격속도
	if (IS_NPC(tch) && TO_NPC(tch)->m_proto->CheckFlag(NPC_LORD_SYMBOL))
	{
		packet->attackSpeed = 12;
	}
	else
	{
		switch (damageType)
		{
		case MSG_DAMAGE_MELEE:
		case MSG_DAMAGE_RANGE:
		case MSG_DAMAGE_MAGIC:
			packet->attackSpeed = ch->m_attackSpeed;
			break;
		default:
			packet->attackSpeed = 0;
			break;
		}
	}

	packet->flag = flag;

	msg->setSize(sizeof(UpdateClient::charDamage));
}

void WearingMsg(CNetMsg::SP& msg, CPC* ch, char wearpos, int item_db_index, int item_plus)
{
	// 안 보이면 무시
	if (!ch->m_bVisible)
	{
		msg->Init();
		return ;
	}

	msg->Init(MSG_WEARING);

	RefMsg(msg) << ch->m_index		// char_index
				<< wearpos
				<< item_db_index;
// 050223 : bs : plus 효과를 위한 정보 추가
	RefMsg(msg) << item_plus;
// --- 050223 : bs : plus 효과를 위한 정보 추가
}

// 아이템 정보 전송 시 공통 적인 부분을 이 함수 안에 추가한다..
void ItemPublicMsg(CNetMsg::SP& msg, CItem * pItem, bool bOption)
{
	// bOption : 아래의 메세지 전송 시 옵션 정보가 마지막 위치가 아닌,
	//           중간에 위치해 있는 경우는 bOption을 false로 사용하여,
	//           옵션 전송을 사용하지 않는다.
	//           ** 추후 시간이 남는다면^^ 아래의 메세지에서
	//              옵션 위치 맞춰서 이곳에서 전송할 수 있도록 수정하면 좋겠다.
	// MSG_PERSONALSHOP_SELL_LIST
	// MSG_STASH - MSG_STASH_LIST_REP
	// MSG_TRADEAGENT - MSG_TRADEAGENT_REG_LIST_REP
	//                  MSG_TRADEAGENT_SEARCH_REP
	//                  MSG_TRADEAGENT_CALCLIST_REP

	if (bOption)
	{
		if (pItem->IsOriginItem())
		{
			// 옵션
			RefMsg(msg) << (char)pItem->GetOriginOptionCnt();
			int i = 0;
			for (i = 0; i < MAX_ORIGIN_OPTION; i++)
			{
				if (pItem->GetOriginIndex(i) > -1)
				{
					RefMsg(msg) << (char)pItem->GetOriginIndex(i)
								<< (int)pItem->GetOriginLevel(i);
					RefMsg(msg) << pItem->getOriginVar(i);
				}
			}

			// 스킬
			RefMsg(msg) << (char)pItem->GetBelongType()					// 귀속 설정
						<< (char)pItem->GetOriginSkillCnt();
			for (i = MAX_ORIGIN_OPTION; i < MAX_ORIGIN; i++)
			{
				if (pItem->GetOriginIndex(i) > -1)
				{
					RefMsg(msg) << pItem->GetOriginIndex(i)
								<< (char)pItem->GetOriginLevel(i);
				}
			}
		}
		else
		{
			RefMsg(msg) << (char)pItem->m_nOption;

			int j = 0;
			for (j = 0; j < pItem->m_nOption; j++)
			{
				RefMsg(msg) << (char)pItem->m_option[j].m_type
							<< (int)pItem->m_option[j].m_level;
			}
		}
	}

	// 추가되는 데이타는 아래에 추가한다.
	RefMsg(msg) << pItem->getPlus_2();
}

void ItemProlongMsg(CNetMsg::SP& msg, int itemindex, int useTime, bool bprolong)
{
	msg->Init(MSG_ITEM);

	RefMsg(msg) << (unsigned char)MSG_ITEM_USE_PROLONG
				<< itemindex
				<< useTime
				<< (unsigned char) bprolong;
}

void ItemTakeMsg(CNetMsg::SP& msg, CCharacter* ch, CItem* item)
{
	msg->Init(MSG_ITEM);

	RefMsg(msg) << (unsigned char)MSG_ITEM_TAKE
				<< (char)ch->m_type
				<< ch->m_index
				<< item->getVIndex();
}

#ifdef DEV_LETS_PARTYTIME
void ItemPartyTypeUseMsg( CNetMsg::SP& msg, int itemIndex ,const char* nick )
{
	msg->Init(MSG_ITEM);
	RefMsg(msg) << (unsigned char)MSG_ITEM_LETSPARTY_USE
				<< itemIndex
				<< nick;
}
#endif //DEV_LETS_PARTYTIME

void ItemDropMsg(CNetMsg::SP& msg, CCharacter* ch, CItem* item)
{
	msg->Init(MSG_ITEM);

	RefMsg(msg) << (unsigned char)MSG_ITEM_DROP
				<< item->getVIndex()
				<< item->getDBIndex()
				<< item->Count()
				<< GET_X(item)
				<< GET_Z(item);

	// 싱글던전 itemdrop
	if (ch->m_pZone->IsGuildRoom() || ch->m_pZone->IsPersonalDungeon())
		RefMsg(msg) << item->m_pos.m_h + 1.0f;
	else
		RefMsg(msg) << (GET_H(item) + 1.0f);

	RefMsg(msg) << GET_R(item)
				<< GET_YLAYER(item)
				<< GET_CHAR_TYPE(ch)
				<< ch->m_index;

	if( IS_NPC(ch) )
	{
		if( DEAD(ch) )
			RefMsg(msg) << (char)0;
		else
			RefMsg(msg) << (char)1;
	}
}

void ItemAppearMsg(CNetMsg::SP& msg, CItem* item)
{
	msg->Init(MSG_ITEM);

	RefMsg(msg) << (unsigned char)MSG_ITEM_APPEAR
				<< item->getVIndex()
				<< item->getDBIndex()
				<< item->Count()
				<< GET_X(item)
				<< GET_Z(item)
				<< (GET_H(item) + 1.0f)
				<< GET_R(item)
				<< GET_YLAYER(item);
}

void ItemDisappearMsg(CNetMsg::SP& msg, int vIndex)
{
	msg->Init(MSG_ITEM);

	RefMsg(msg) << (unsigned char)MSG_ITEM_DISAPPEAR
				<< vIndex;
}

void ItemUpgradeRepMsg(CNetMsg::SP& msg, char result, bool isRune, char nPlusNum)
{
	msg->Init(MSG_ITEM);

	RefMsg(msg) << (unsigned char)MSG_ITEM_UPGRADE_REP
				<< result;

	// 146레벨 이상 부터는 적용되는 제련석(룬)은
	// 룬의 여부와 +,-되는 수치를 추가 전송한다.
	RefMsg(msg) << (char)isRune;

	if (isRune)
		RefMsg(msg) << nPlusNum;
}

void ItemRefineRepMsg(CNetMsg::SP& msg, char result)
{
	msg->Init(MSG_ITEM);

	RefMsg(msg) << (unsigned char)MSG_ITEM_REFINE_REP
				<< result;
}

void ItemOptionAddRepMsg(CNetMsg::SP& msg, CItem* item, MSG_ITEM_OPTION_ADD_RESULT result)
{
	msg->Init(MSG_ITEM);

	RefMsg(msg) << (unsigned char)MSG_ITEM_OPTION_ADD_REP
				<< (char)result;
}

void ItemOptionDelRepMsg(CNetMsg::SP& msg, CItem* item, MSG_ITEM_OPTION_DEL_RESULT result)
{
	msg->Init(MSG_ITEM);

	RefMsg(msg) << (unsigned char)MSG_ITEM_OPTION_DEL_REP
				<< (char)result;
}

void ItemProcessRepMsg(CNetMsg::SP& msg, MSG_ITEM_PROCESS_RESULT result)
{
	msg->Init(MSG_ITEM);

	RefMsg(msg) << (unsigned char) MSG_ITEM_PROCESS_REP
				<< (char)result;
}

void ItemMakeRepMsg(CNetMsg::SP& msg, MSG_ITEM_MAKE_RESULT result)
{
	msg->Init(MSG_ITEM);

	RefMsg(msg) << (unsigned char) MSG_ITEM_MAKE_REP
				<< (char)result;
}

void ItemMixRepMsg(CNetMsg::SP& msg, MSG_ITEM_MIX_RESULT result)
{
	msg->Init(MSG_ITEM);

	RefMsg(msg) << (unsigned char) MSG_ITEM_MIX_REP
				<< (char) result;
}

void ItemArcaneRepMsg(CNetMsg::SP& msg, MSG_ITEM_ARCANE_RESULT result)
{
	msg->Init(MSG_ITEM);

	RefMsg(msg) << (unsigned char) MSG_ITEM_ARCANE_REP
				<< (char) result;
}

void ItemGetMsg(CNetMsg::SP& msg, CPC* ch, CItem* item)
{
	msg->Init(MSG_ITEM);

	RefMsg(msg) << (unsigned char) MSG_ITEM_GET
				<< ch->GetName()
				<< item->m_itemProto->getItemIndex()
				<< item->Count();
}

void ItemCheckCompotion(CNetMsg::SP& msg, unsigned short tab, unsigned short invenIndex , int index )
{
	msg->Init(MSG_ITEM);

	RefMsg(msg) << (unsigned char)MSG_ITEM_CHECK_COMPOSITION
				<< tab << invenIndex << index;
}

void DBCharEndMsg(CNetMsg::SP& msg, int remain0, int remain1, unsigned int nSeed)
{
	msg->Init(MSG_DB);
	RefMsg(msg) << (unsigned char)MSG_DB_CHAR_END
				<< remain0
				<< remain1
				<< nSeed;
}
void DBCharExistMsg(CNetMsg::SP& msg, int index, const char* name, const char* nick, char job, char job2, char hairstyle, char facestyle, int level, LONGLONG ex, int sp, int hp, int maxhp, int mp, int maxmp, int wear[MAX_WEARING], int plus[MAX_WEARING], int nDeleteDelayRemain)
{
	if(ex > GetLevelupExp(level))
	{
		ex = GetLevelupExp(level);
	}
	int i;

	msg->Init(MSG_DB);

	if (hp > maxhp)
		hp = maxhp;
	if (mp > maxmp)
		mp = maxmp;

	// 캐릭터 정보

	RefMsg(msg) << (unsigned char)MSG_DB_CHAR_EXIST
				<< index;
	if( (nick != NULL) && (strcmp(nick, "") != 0) )
		RefMsg(msg) << nick;
	else
		RefMsg(msg) << name;
	RefMsg(msg) << job
				<< job2
				<< hairstyle
				<< facestyle
				<< level
				<< ex
				<< GetLevelupExp(level)
				<< sp
				<< hp
				<< maxhp
				<< mp
				<< maxmp;

	for (i = WEARING_SHOW_START; i <= WEARING_BOOTS; i++)
	{
		RefMsg(msg) << wear[i - WEARING_SHOW_START];
// 050223 : bs : plus 효과를 위한 정보 추가
		RefMsg(msg) << plus[i - WEARING_SHOW_START];
// --- 050223 : bs : plus 효과를 위한 정보 추가
	}
	RefMsg(msg) << wear[WEARING_BACKWING];
	RefMsg(msg) << plus[WEARING_BACKWING];

	RefMsg(msg) << nDeleteDelayRemain;
}

void DBSuccessMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_DB);
	RefMsg(msg) << (unsigned char)MSG_DB_SUCCESS;
}

void DBOtherServerMsg(CNetMsg::SP& msg, int zone, const char* ip, int port)
{
	msg->Init(MSG_DB);
	RefMsg(msg) << (unsigned char)MSG_DB_OTHER_SERVER
				<< zone
				<< ip
				<< port;
}

void DBOKMsg(CNetMsg::SP& msg, int zone)
{
	msg->Init(MSG_DB);
	RefMsg(msg) << (unsigned char)MSG_DB_OK
				<< zone;
}

void MsgrRepWhisperNotFoundMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sindex, const char* sname)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_WHISPER_NOTFOUND
				<< sindex << sname;
}

void ChatWhisperMsg(CNetMsg::SP& msg, int sindex, const char* sname, const char* rname, const char* chat)
{
	msg->Init(MSG_CHAT);
	RefMsg(msg) << (unsigned char)MSG_CHAT_WHISPER
				<< sindex << sname
				<< rname
				<< chat;
}

void MsgrRepWhisperRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sindex, const char* sname, const char* rname, const char* chat)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_WHISPER_REP
				<< sindex << sname
				<< rname
				<< chat;
}

void SysWhisperNotFoundMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_SYS);
	RefMsg(msg) << (unsigned char)MSG_SYS_WHISPER_NOTFOUND;
}

void MsgrRepMessengerChatRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sindex, const char* sname, const char* rname, const char* chat)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_MESSENGERCHAT_REP
				<< sindex << sname
				<< rname
				<< chat;
}

void MsgrRepMessengerNotFoundMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sindex, const char* sname)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_MESSENGERCHAT_NOTFOUND
				<< sindex << sname;
}

void ChatMessengerMsg(CNetMsg::SP& msg, int sindex, const char* sname, const char* rname, const char* chat)
{
	msg->Init(MSG_CHAT);
	RefMsg(msg) << (unsigned char)MSG_CHAT_MESSENGER
				<< sindex << sname
				<< rname
				<< chat;
}

void SysFriendNotFoundMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_SYS);
	RefMsg(msg) << (unsigned char)MSG_SYS_FRIEND_NOTFOUND;
}

void MsgrFriendChatReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int sindex, const char* sname, const char* rname, const char* chat)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< (int)-1
				<< (unsigned char)MSG_MSGR_MESSENGERCHAT_REQ
				<< sindex << sname
				<< rname
				<< chat;
}

void HelperNameChangeReqMsg(CNetMsg::SP& msg, char bguild, int index, const char* reqname)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_NAME_CHANGE_REQ
				<< bguild
				<< index
				<< reqname;
}

void BlockPCRepMsg(CNetMsg::SP& msg, MSG_EX_MESSENGER_ERROR_TYPE errcode, int charIndex, const char* name)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_MESSENGER
				<< (unsigned char) MSG_EX_MESSENGER_BLOCK
				<< (unsigned char) errcode
				<< charIndex
				<< name;
}

void HelperFriendSetConditionMsg(CNetMsg::SP& msg, int chaindex, int condition, int bReply, CPC* pc)
{
	CFriend* pfriend = NULL;
	if( pc )
		pfriend = pc->m_Friend;

	if(bReply==-1 && !pfriend)
	{
		msg->Init();
		return;
	}
	if(pfriend && pfriend->GetFriendSum() < 1)
	{
		msg->Init();
		return;
	}

	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_FRIEND_SET_CONDITION_REQ
				<< chaindex
				<< condition;

	RefMsg(msg) << bReply;

	if(bReply == -1)
	{
		int index;
		int sum;

		sum = pfriend->GetFriendSum();

		CNetMsg::SP IndexMsg(new CNetMsg);
		IndexMsg->Init();
		int blockSum = 0;

		for(int j=0; j< FRIEND_MAX_MEMBER; j++)
		{
			if(pfriend->GetFriendMember(j))
			{
				index = pfriend->GetFriendMember(j)->GetChaIndex();
				if( index != -1 )
				{
					if( !pc->IsBlockPC(index, pfriend->GetFriendMember(j)->GetName() ) )
					{
						RefMsg(IndexMsg) << index;
					}
					else
						blockSum++;
				}
			}
		}

		RefMsg(msg) << sum - blockSum
					<< IndexMsg;
	}
}

void BlockPCListMsg(CNetMsg::SP& msg, CDescriptor* d)
{
	CPC* pc = d->m_pChar;

	if( pc )
	{
		msg->Init(MSG_EXTEND);
		RefMsg(msg) << MSG_EX_MESSENGER
					<< (unsigned char) MSG_EX_MESSENGER_BLOCK_LIST;

		int count = pc->m_listBlockPC.size();

		RefMsg(msg) << count;

		map_listblock_t::iterator it = pc->m_listBlockPC.begin();
		map_listblock_t::iterator endit = pc->m_listBlockPC.end();
		for(; it != endit; ++it)
		{
			CBlockPC* blockPC = it->second;

			RefMsg(msg) << blockPC->GetIndex()
						<< blockPC->GetName();
		}

		SEND_Q(msg, d);
	}
}

void ChatOneMessengerMsg(CNetMsg::SP& msg, int sindex, const char* sname, int rindex, const char* rname, int chatcolor, const char* chat)
{
	// sindex(n) sname(str) rindex(n) rname(str) chatcolor(n) chat(str)
	// sindex(n) rindex(n) chatcolor(n) char(str)
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_MESSENGER
				<< (unsigned char)MSG_EX_MESSENGER_ONE_VS_ONE
				<< sindex
				<< sname
				<< rindex
//		<< rname
				<< chatcolor
				<< chat;
}

void ChatOneOffLineMessengerMsg(CNetMsg::SP& msg, int sindex, int rindex )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_MESSENGER
				<< (unsigned char)MSG_EX_MESSENGER_ONE_OFF_LINE
				<< sindex
				<< rindex;
}

void FriendListMsg(CNetMsg::SP& msg, CDescriptor* d)
{
	CPC* pc = d->m_pChar;

	CFriendMember* pfriendmember;

	if(pc)
	{
		CFriend* pFriend = pc->m_Friend;

		if(pFriend)
		{
			int cnt = pFriend->GetFriendSum();
			msg->Init(MSG_FRIEND);
			RefMsg(msg) << (unsigned char)MSG_FRIEND_MEMBERLIST
						<< cnt;
			for (int i = 0; i < cnt; i++)
			{
				pfriendmember = pFriend->GetFriendMember(i);
				if (pfriendmember)
				{
					RefMsg(msg) << pfriendmember->GetChaIndex()
								<< pfriendmember->GetName()
								<< pfriendmember->GetJob()
								<< pfriendmember->GetCondition()
								<< pfriendmember->GetGroup()
								;
				}
			}
			SEND_Q(msg, d);
		}
	}
}

void MsgrConnectMsg(CNetMsg::SP& msg, int version, int server, int subno, int count, int* zones)
{
	msg->Init(MSG_MSGR_CONNECT);
	RefMsg(msg) << version
				<< server
				<< subno
				<< count;
	int i;
	for (i = 0; i < count; i++)
		RefMsg(msg) << zones[i];
}

void MsgrWhisperReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int sindex, const char* sname, const char* rname, const char* chat)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< (int)-1
				<< (unsigned char)MSG_MSGR_WHISPER_REQ
				<< sindex << sname
				<< rname
				<< chat;
}

#ifdef GMTOOL
void MsgGmKickIDCommandReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int thisServer, int thisSubno, int charindex, const char* id)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char) MSG_MSGR_GMTOOL_KICKID_REQ
				<< thisServer
				<< thisSubno
				<< charindex		// 지엠툴 캐릭터 인덱스
				<< id;
}

void MsgGmKickIDCommandRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, char success, int charindex, const char* id)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< -1
				<< (unsigned char)MSG_MSGR_GMTOOL_KICKID_REP
				<< success
				<< charindex
				<< id;
}

void MsgGmKickCommandReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int thisServer, int thisSubno, int charindex, const char* name)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char) MSG_MSGR_GMTOOL_KICK_REQ
				<< thisServer
				<< thisSubno
				<< charindex		// 지엠툴 캐릭터 인덱스
				<< name;
}

void MsgGmKickCommandRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, char success, int charindex, const char* name)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< -1
				<< (unsigned char)MSG_MSGR_GMTOOL_KICK_REP
				<< success
				<< charindex
				<< name;
}

void MsgrGmToolChatMonitorReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int gmcharindex, const char* name, const char* onoff)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_GMTOOL_CHATMONITOR_REQ
				<< gmcharindex
				<< name
				<< onoff;
}

void MsgrGmToolChatMonitorRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, char success, int gmcharindex, const char* id)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_GMTOOL_CHATMONITOR_REP
				<< success
				<< gmcharindex
				<< id;
}

void MsgrNoticeGmChatMonitorMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int thisServer, int thisSub, int thisZone, const char* chat, int charindex, const char* name, unsigned char chattype)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_GMTOOL_CHATMONITOR_NOTICE
				<< thisServer
				<< thisSub
				<< thisZone
				<< chat
				<< charindex
				<< name
				<< chattype;
}

void MsgrNoticeGmChatMonitorPartyMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int thisServer, int thisSub, int thisZone, const char* chat, int bossindex, const char* name, int charindex, const char* charname)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_GMTOOL_CHATMONITOR_NOTICE_PARTY
				<< thisServer
				<< thisSub
				<< thisZone
				<< chat
				<< bossindex
				<< name
				<< charindex
				<< charname;
}

void MsgrNoticeGmChatMonitorGuildMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int thisServer, int thisSub, int thisZone, const char* chat, int guildindex, const char* name, int charindex, const char* charname)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_GMTOOL_CHATMONITOR_NOTICE_GUILD
				<< thisServer
				<< thisSub
				<< thisZone
				<< chat
				<< guildindex
				<< name
				<< charindex
				<< charname;
}

void MsgrGmToolChatMonitorOffMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_GMTOOL_CHATMONITOR_OFF
				<< charindex;
}

void MsgrGmToolChatMonitorPartyOffMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int bossindex)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_GMTOOL_CHATMONITOR_PARTY_OFF
				<< bossindex;
}

void MsgrGmToolChatMonitorGuildOffMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int guildindex)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_GMTOOL_CHATMONITOR_GUILD_OFF
				<< guildindex;
}

void MsgrNoticeGmChatMonitorWhisperMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int thisServer, int thisSub, int thisZone, const char* chat, int charindex, const char* name, unsigned char chattype, const char* sender)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_GMTOOL_CHATMONITOR_NOTICE_WHISPER
				<< thisServer
				<< thisSub
				<< thisZone
				<< chat
				<< charindex
				<< name
				<< chattype
				<< sender;
}

void MsgGMSilenceReq(CNetMsg::SP& msg, int seq, int serverno, int subno, int gmcharindex, const char* charname, int sec)
{
	msg->Init(MSG_MSGR_REQ);

	RefMsg(msg) << seq
				<< serverno
				<< subno
				<< -1
				<< (unsigned char)MSG_MSGR_GMTOOL_SILENCE_REQ
				<< gmcharindex
				<< charname
				<< sec;
}

void MsgGMSilenceRep(CNetMsg::SP& msg, int seq, int thisServer, int thisSub, int success, int gmcharindex, const char* name)
{
	msg->Init(MSG_MSGR_REP);

	RefMsg(msg) << seq
				<< 1
				<< 1
				<< -1
				<< (unsigned char)MSG_MSGR_GMTOOL_SILENCE_REP
				<< thisServer
				<< thisSub
				<< success
				<< gmcharindex
				<< name;
}

void MsgrRepGMToolWhisperNotFoundMsg(CNetMsg::SP& msg, int nSeq, int nServer, int nSubno, int nZone, int charindex, const char* name)
{
	msg->Init(MSG_MSGR_REP);

	RefMsg(msg) << nSeq
				<< nServer
				<< nSubno
				<< nZone
				<< (unsigned char)MSG_MSGR_GMTOOL_WHISPER_NOTFOUND
				<< charindex
				<< name;
}

void GMSayMsg(CNetMsg::SP& msg, int server, int sub, int charindex, const char* name, const char* chat, unsigned char chattype)
{
	msg->Init(MSG_GM_CHAT);
	RefMsg(msg) << server
				<< sub
				<< charindex
				<< name
				<< chat
				<< chattype;
}

void MsgrRepGMWhisperNotFoundMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sindex)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_GM_WHISPER_NOTFOUND
				<< sindex;
}

void MsgExtraGMCommandReq(CNetMsg::SP& msg, int serverno, int subno, MSG_MSGR_TYPE msgtype)
{
	msg->Init(MSG_MSGR_REQ);

	RefMsg(msg) << -1
				<< serverno
				<< subno
				<< 0
				<< (unsigned char)msgtype;
}

void MsgLoopCountRep(CNetMsg::SP& msg, int serverno, int subno, int thisServer, int thisSub, const char* loopbuf)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << -1
				<< serverno
				<< subno
				<< 0
				<< (unsigned char)MSG_MSGR_GM_LOOPCOUNT_REP
				<< thisServer
				<< thisSub
				<< loopbuf;
}

void ChatGMWhisperMsg(CNetMsg::SP& msg, int sindex, const char* sname, const char* rname, const char* chat)
{
	msg->Init(MSG_CHAT);
	RefMsg(msg) << (unsigned char)MSG_CHAT_GMTOOL
				<< sindex << sname
				<< rname
				<< chat;
}
void MsgrRepGMWhisperRepMsg(CNetMsg::SP& msg, int nSeq, int server, int subno, int zone, int index, const char*  name, const char* chat, int thisServer, int thisSub, int sindex)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << nSeq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_GM_WHISPER_REP
				<< index
				<< name
				<< chat
				<< thisServer
				<< thisSub
				<< sindex;
}

void MsgrGMWhisperReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int sindex, const char* sname, const char* rname, const char* chat)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< (int)-1
				<< (unsigned char)MSG_MSGR_GM_WHISPER_REQ
				<< sindex << sname
				<< rname
				<< chat;
}

void MsgrGMWhisperReqMsg(CNetMsg::SP& msg, int seq, int server, int sub, const char* chat, int charindex, const char* name, int serverno, int subno, const char* receiver)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< sub
				<< -1
				<< (unsigned char)MSG_MSGR_GMTOOL_WHIPER_REQ
				<< chat
				<< charindex
				<< name
				<< serverno
				<< subno
				<< receiver;
}

void MsgrRepGmToolWhisperRepMsg(CNetMsg::SP& msg, int seq, int server, int sub, int zone, int sindex, const char* sname, const char* rname, const char *chat)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< sub
				<< zone
				<< (unsigned char)MSG_MSGR_GMTOOL_WHISPER_REP
				<< sindex << sname
				<< rname
				<< chat;
}

void MsgrGMShutdownReqMsg(CNetMsg::SP& msg, int server, int sub, int remain, const char* chat)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << -1
				<< server
				<< sub
				<< -1
				<< (unsigned char)MSG_MSGR_GMTOOL_SHUTDOWN
				<< remain
				<< chat;
}

#endif // GMTOOL

void MsgrChannelChatReqMsg(CNetMsg::SP& msg, int server, int sender_index, const char*  name, const char* chat, unsigned char chatType)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << -1
				<< server
				<< -1
				<< -1
				<< (unsigned char)MSG_MSGR_CHANNEL_CHAT
				<< sender_index
				<< name
				<< chat
				<< (unsigned char) gserver->m_subno
				<< chatType ;
}

void SysFullInventoryMsg(CNetMsg::SP& msg, char tab)
{
	msg->Init(MSG_SYS);
	RefMsg(msg) << (unsigned char)MSG_SYS_FULL_INVENTORY
				<< tab;
}

void SysMsg(CNetMsg::SP& msg, MSG_SYS_TYPE systype)
{
	msg->Init(MSG_SYS);
	RefMsg(msg) << (unsigned char)systype;
}

void SysCannotDuplcationMsg(CNetMsg::SP& msg, int newIndex, int curIndex)
{
	msg->Init(MSG_SYS);
	RefMsg(msg) << (unsigned char)MSG_SYS_CANNOT_DUPLICATION
				<< newIndex
				<< curIndex;
}

void SysBloodItemMsg(CNetMsg::SP& msg, int itemdbindex)
{
	msg->Init(MSG_SYS);
	RefMsg(msg) << (unsigned char)MSG_SYS_BLOODITEM
				<< itemdbindex;
}

void EnvWeatherMsg(CNetMsg::SP& msg, int zoneNo)
{
	msg->Init(MSG_ENV);
	RefMsg(msg) << (unsigned char)MSG_ENV_WEATHER
				<< gserver->m_zones[zoneNo].m_weather;
}

void ExchangeReqMsg(CNetMsg::SP& msg, MSG_EXCHANGE_REQ_TYPE reqType, CPC* src, CPC* dest)
{
	msg->Init(MSG_EXCHANGE);

	RefMsg(msg) << (unsigned char)MSG_EXCHANGE_REQ
				<< (unsigned char)reqType;
	if (reqType == MSG_EXCHANGE_REQ_REQ)
	{
		RefMsg(msg) << src->m_index
					<< src->GetName()
					<< dest->m_index
					<< dest->GetName();
	}
}

void ExchangeItemMsg(CNetMsg::SP& msg, MSG_EXCHANGE_ITEM_ACTION action, CItem* item, LONGLONG count, GoldType_t nasCount)
{
	msg->Init(MSG_EXCHANGE);

	RefMsg(msg) << (unsigned char)MSG_EXCHANGE_ITEM
				<< (unsigned char)action
				<< item->getVIndex();
	switch (action)
	{
	case MSG_EXCHANGE_ITEM_DEL_SRC:
	case MSG_EXCHANGE_ITEM_DEL_DEST:
		return ;
	default:
		RefMsg(msg) << count;
		break;
	}

	switch (action)
	{
	case MSG_EXCHANGE_ITEM_ADD:
	case MSG_EXCHANGE_ITEM_ADD_SRC:
	case MSG_EXCHANGE_ITEM_ADD_DEST:
		RefMsg(msg) << item->getDBIndex()
			<< item->getPlus()
			<< item->getFlag()
			<< item->getUsed()
			<< item->getUsed_2()
			<< item->getNowDurability()
			<< item->getMaxDurability();

		ItemPublicMsg(msg, item);
		ItemSocketMsg(msg, item);
		RefMsg(msg) << nasCount;
		break;
	default:
		RefMsg(msg) << nasCount;
		break;
	}
}

void GMWhoAmIMsg(CNetMsg::SP& msg, CPC* ch)
{
	msg->Init(MSG_GM);
	RefMsg(msg) << (unsigned char)MSG_GM_WHOAMI
				<< (unsigned char)ch->m_admin;
}

void MsgrNoticeMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, const char* chat)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_NOTICECHAT
				<< chat;
}

void MsgrUpdateRevMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int rev, const char* chat, int date)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_UPDATE_REV
		<< rev
		<< chat
		<< date;
}

void MsgrLogoutReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, const char* id)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_LOGOUT_REQ
				<< id;
}

void MsgrLogoutRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, char success, const char* id)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_LOGOUT_REP
				<< success
				<< id;
}

void MsgrEndGameReqMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << (int)-1
				<< (int)-1
				<< (int)-1
				<< (int)-1
				<< (unsigned char)MSG_MSGR_ENDGAME_REQ;
}

void MsgrRebootReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_REBOOT_REQ;
}

void MsgrObserverMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, const char* chat)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_OBSERVER
				<< chat;
}

void MsgrNpcNoticeMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int npcIndex, const char* chat)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_NPC_NOTICECHAT
				<< npcIndex
				<< chat;
}

#ifdef NEW_DOUBLE_GM_AUTO
void MsgrDoubleEventReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int percent[], int start[], int end[])
{
	int i;
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char) MSG_MSGR_DOUBLE_EVENT_REQ
				<< charindex
				<< cmd;

	for(i = 0; i < 6; i++)
	{
		RefMsg(msg) << percent[i];
	}

	for(i = 0; i < 6; i++)
	{
		RefMsg(msg) << start[i];
	}

	for(i = 0; i < 6; i++)
	{
		RefMsg(msg) << end[i];
	}
}
#else
void MsgrDoubleEventReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int percent[])
{
	int i;
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char) MSG_MSGR_DOUBLE_EVENT_REQ
				<< charindex
				<< cmd;

	for(i = 0; i < 6; i++)
	{
		RefMsg(msg) << percent[i];
	}
}
#endif

void MsgrDoubleEventRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int cmd)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char) MSG_MSGR_DOUBLE_EVENT_REP
				<< charindex
				<< gserver->m_serverno
				<< gserver->m_subno
				<< cmd;
#ifdef NEW_DOUBLE_GM_AUTO
	RefMsg(msg) << gserver->m_bDoubleNasPercent
				<< gserver->m_bDoubleNasGetPercent
				<< gserver->m_bDoubleExpPercent
				<< gserver->m_bDoubleSpPercent
				<< gserver->m_bDoubleProducePercent
				<< gserver->m_bDoubleProduceNum;
	int i;
	for(i = 0; i < 6; i++)
	{
		RefMsg(msg) << gserver->m_iDoubleGMStart[i];
	}
	for(i = 0; i < 6; i++)
	{
		RefMsg(msg) << gserver->m_iDoubleGMEnd[i];
	}
#else
	RefMsg(msg) << gserver->m_bDoubleNasPercent
				<< gserver->m_bDoubleNasGetPercent
				<< gserver->m_bDoubleExpPercent
				<< gserver->m_bDoubleSpPercent
				<< gserver->m_bDoubleProducePercent
				<< gserver->m_bDoubleProduceNum;
#endif
}

#ifdef NEW_DOUBLE_EVENT_AUTO
#ifdef NEW_DOUBLE_EVENT_AUTO_TIME
void MsgrDoubleEventAutoReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, char cmd, int start[], int end[])
#else
void MsgrDoubleEventAutoReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, char cmd)
#endif // NEW_DOUBLE_EVENT_AUTO_TIME
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char) MSG_MSGR_DOUBLE_EVENT_AUTO_REQ
				<< charindex
				<< cmd;
#ifdef NEW_DOUBLE_EVENT_AUTO_TIME
	int i;
	for (i = 0; i < 6; i++)
	{
		RefMsg(msg) << start[i];
	}
	for (i = 0; i < 6; i++)
	{
		RefMsg(msg) << end[i];
	}
#endif // NEW_DOUBLE_EVENT_AUTO_TIME
}

#ifdef NEW_DOUBLE_EVENT_AUTO_TIME
void MsgrDoubleEventAutoRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, char cmd, char state, int start[], int end[])
#else
void MsgrDoubleEventAutoRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, char cmd, char state)
#endif
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char) MSG_MSGR_DOUBLE_EVENT_AUTO_REP
				<< charindex
				<< gserver->m_serverno
				<< gserver->m_subno
				<< cmd
				<< state;
#ifdef NEW_DOUBLE_EVENT_AUTO_TIME
	int i;
	for (i = 0; i < 6; i++)
	{
		RefMsg(msg) << gserver->m_iDoubleEventStart[i];
	}
	for (i = 0; i < 6; i++)
	{
		RefMsg(msg) << gserver->m_iDoubleEventEnd[i];
	}

#endif // NEW_DOUBLE_EVENT_AUTO_TIME
}
#endif // NEW_DOUBLE_EVENT_AUTO

void MsgrDoubleExpEventReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int expPercent)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char) MSG_MSGR_DOUBLE_EXP_EVENT_REQ
				<< charindex
				<< cmd
				<< expPercent;
}

void MsgrDoubleExpEventRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int expPercent)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char) MSG_MSGR_DOUBLE_EXP_EVENT_REP
				<< charindex
				<< gserver->m_serverno
				<< gserver->m_subno
				<< cmd
				<< expPercent;
}

void MsgrValentineEventReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, bool bStart, int thisServer, int thisSubno, int charIndex)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char) MSG_MSGR_VALENTINE_EVENT_REQ
				<< (char) bStart
				<< thisServer
				<< thisSubno
				<< charIndex;
}

void MsgrWhiteDayEventReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, bool bStart, int thisServer, int thisSubno, int charIndex)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char) MSG_MSGR_WHITEDAY_EVENT_REQ
				<< (char) bStart
				<< thisServer
				<< thisSubno
				<< charIndex;
}

void QuickSlotAddMsg(CNetMsg::SP& msg, CPC* ch, char page, char slot)
{
	msg->Init(MSG_QUICKSLOT);

	RefMsg(msg) << (unsigned char)MSG_QUICKSLOT_ADD
				<< page
				<< slot;

	switch (ch->m_quickSlot[(int)page].m_slotType[(int)slot])
	{
	case QUICKSLOT_TYPE_SKILL:
		RefMsg(msg) << (char)ch->m_quickSlot[(int)page].m_slotType[(int)slot]
					<< ch->m_quickSlot[(int)page].m_skillType[(int)slot];
		break;

	case QUICKSLOT_TYPE_ACTION:
		RefMsg(msg) << (char)ch->m_quickSlot[(int)page].m_slotType[(int)slot]
					<< ch->m_quickSlot[(int)page].m_actionType[(int)slot];
		break;

	case QUICKSLOT_TYPE_ITEM:
		if (ch->m_quickSlot[(int)page].m_item[(int)slot])
		{
			RefMsg(msg) << (char)ch->m_quickSlot[(int)page].m_slotType[(int)slot]
						<< (unsigned short)ch->m_quickSlot[(int)page].m_item[(int)slot]->tab()
						<< (unsigned short)ch->m_quickSlot[(int)page].m_item[(int)slot]->getInvenIndex();
		}
		else
			RefMsg(msg) << (char)QUICKSLOT_TYPE_EMPTY;
		break;
	case QUICKSLOT_TYPE_ITEM_WEAR:
		if (ch->m_quickSlot[(int)page].m_item[(int)slot])
		{
			int wearItemType = -1;
			int wearItemValue = -1;

			if(ch->m_quickSlot[(int)page].m_item[slot]->getWearPos() == COSTUME2_WEARING_SUIT)
			{
				wearItemType = ITEM_COSTUM_SUIT;
				wearItemValue = (int)ch->m_quickSlot[(int)page].m_item[(int)slot]->getDBIndex();
			}

			else if(ch->m_quickSlot[(int)page].m_item[slot]->m_itemProto->getItemFlag() & ITEM_FLAG_COSTUME2)
			{
				wearItemType = ITEM_COSTUM;
				wearItemValue = (int)ch->m_quickSlot[(int)page].m_item[(int)slot]->getWearPos();
			}

			else
			{
				wearItemType = ITEM_NORMAL;
				wearItemValue = (int)ch->m_quickSlot[(int)page].m_item[(int)slot]->getWearPos();
			}


			RefMsg(msg) << (char)ch->m_quickSlot[(int)page].m_slotType[(int)slot]
						<< wearItemType
						<< wearItemValue;
		}
		break;

	case QUICKSLOT_TYPE_EMPTY:
	default:
		RefMsg(msg) << (char)ch->m_quickSlot[(int)page].m_slotType[(int)slot];
		break;
	}
}

void QuickSlotListMsg(CNetMsg::SP& msg, CPC* ch, char page)
{
	int slot;

	msg->Init(MSG_QUICKSLOT);

	RefMsg(msg) << (unsigned char)MSG_QUICKSLOT_LIST
				<< page;

	for (slot=0; slot < QUICKSLOT_MAXSLOT; slot++)
	{
		switch (ch->m_quickSlot[(int)page].m_slotType[(int)slot])
		{
		case QUICKSLOT_TYPE_SKILL:
			RefMsg(msg) << (char)ch->m_quickSlot[(int)page].m_slotType[(int)slot]
						<< ch->m_quickSlot[(int)page].m_skillType[(int)slot];
			break;

		case QUICKSLOT_TYPE_ACTION:
			RefMsg(msg) << (char)ch->m_quickSlot[(int)page].m_slotType[(int)slot]
						<< ch->m_quickSlot[(int)page].m_actionType[(int)slot];
			break;

		case QUICKSLOT_TYPE_ITEM_WEAR:
			{
				int wearItemType = -1;
				int wearItemValue = -1;

				if(ch->m_quickSlot[(int)page].m_item[slot]->getWearPos() == COSTUME2_WEARING_SUIT)
				{
					wearItemType = ITEM_COSTUM_SUIT;
					wearItemValue = (int)ch->m_quickSlot[(int)page].m_item[(int)slot]->getDBIndex();
				}

				else if(ch->m_quickSlot[(int)page].m_item[slot]->m_itemProto->getItemFlag() & ITEM_FLAG_COSTUME2)
				{
					wearItemType = ITEM_COSTUM;
					wearItemValue = (int)ch->m_quickSlot[(int)page].m_item[(int)slot]->getWearPos();
				}

				else
				{
					wearItemType = ITEM_NORMAL;
					wearItemValue = (int)ch->m_quickSlot[(int)page].m_item[(int)slot]->getWearPos();
				}

				RefMsg(msg) << (char)ch->m_quickSlot[(int)page].m_slotType[(int)slot]
							<< wearItemType
							<< wearItemValue;
				break;
			}
		case QUICKSLOT_TYPE_ITEM:
			if (ch->m_quickSlot[(int)page].m_item[(int)slot])
			{
				RefMsg(msg) << (char)ch->m_quickSlot[(int)page].m_slotType[(int)slot]
							<< (unsigned short)ch->m_quickSlot[(int)page].m_item[(int)slot]->tab()
							<< (unsigned short)ch->m_quickSlot[(int)page].m_item[(int)slot]->getInvenIndex();
			}
			else
				RefMsg(msg) << (char)QUICKSLOT_TYPE_EMPTY;
			break;

		case QUICKSLOT_TYPE_EMPTY:
		default:
			RefMsg(msg) << (char)ch->m_quickSlot[(int)page].m_slotType[(int)slot];
			break;
		}
	}
}

void PartyInviteMsg(CNetMsg::SP& msg, char cPartyType1, char cPartyType2, char cPartyType3, int nBossIndex, const char* strBossName)
{
	msg->Init(MSG_PARTY);
	RefMsg(msg) << (unsigned char)MSG_PARTY_INVITE
				<< cPartyType1
				<< cPartyType2
				<< cPartyType3
				<< nBossIndex
				<< strBossName;
}

void PartyMsg(CNetMsg::SP& msg, MSG_PARTY_TYPE subtype)
{
	msg->Init(MSG_PARTY);
	RefMsg(msg) << (unsigned char)subtype;
}

void PartyAddMsg(CNetMsg::SP& msg, int nCharIndex, const char* strCharName, CPC* tch, char isboss)
{
	msg->Init(MSG_PARTY);
	RefMsg(msg) << (unsigned char)MSG_PARTY_ADD
				<< isboss
				<< nCharIndex
				<< strCharName;
	if (tch)
	{
		RefMsg(msg) << tch->m_job
					<< tch->m_job2
					<< tch->m_level
					<< tch->m_hp << tch->m_maxHP
					<< tch->m_mp << tch->m_maxMP
					<< GET_X(tch)
					<< GET_Z(tch)
					<< GET_YLAYER(tch);
		if( tch->m_pZone )
		{
			RefMsg(msg) << tch->m_pZone->m_index;
		}
		else
		{
			RefMsg(msg) << (int)0;
		}
	}
	else
	{
		RefMsg(msg) << (char)0
					<< (char)0
					<< (int)0				//level
					<< (int)1 << (int)1
					<< (int)1 << (int)1
					<< (int)0
					<< (int)0
					<< (int)0
					<< (int)0;
	}
}

void PartyDelMsg(CNetMsg::SP& msg, bool bKick, int nTargetIndex)
{
	msg->Init(MSG_PARTY);
	if (bKick)
		RefMsg(msg) << (unsigned char)MSG_PARTY_KICK;
	else
		RefMsg(msg) << (unsigned char)MSG_PARTY_QUIT;
	RefMsg(msg) << nTargetIndex;
}

void PartyInfoMsg(CNetMsg::SP& msg, CPC* ch, bool isOnline)
{
	UpdateClient::partyInfo* packet = reinterpret_cast<UpdateClient::partyInfo*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_PARTY_INFO;

	packet->charIndex = ch->m_index;
	packet->level = ch->m_level;
	packet->hp = ch->m_hp;
	packet->maxHp = ch->m_maxHP;
	packet->mp = ch->m_mp;
	packet->maxMp = ch->m_maxMP;
	packet->x = GET_X(ch);
	packet->z = GET_Z(ch);
	packet->yLayer = GET_YLAYER(ch);
	packet->zoneIndex = ch->m_pZone->m_index;
	packet->isOnline = isOnline;
	msg->setSize(sizeof(UpdateClient::partyInfo));
}

void PartyErrorMsg(CNetMsg::SP& msg, MSG_PARTY_ERROR_TYPE partyerror)
{
	msg->Init(MSG_PARTY);
	RefMsg(msg) << (unsigned char)MSG_PARTY_ERROR
				<< (unsigned char)partyerror;
}

void PartyAssistInfoMsg(CNetMsg::SP& msg, CPC* tch)
{
	msg->Init(MSG_PARTY);
	RefMsg(msg) << (unsigned char)MSG_PARTY_ASSISTINFO;
	if (tch)
		tch->m_assist.AppendAssistToNetMsg(msg);
	else
	{
		RefMsg(msg) << (LONGLONG)0
					<< (char)0;
	}
}

void PartyChangeBoss(CNetMsg::SP& msg, const char* bossname, int nNewBossIndex, const char* newname, bool bMandate)
{
	msg->Init(MSG_PARTY);
	RefMsg(msg) << (unsigned char) MSG_PARTY_CHANGEBOSS
				<< bossname
				<< nNewBossIndex
				<< newname
				<< (char)bMandate;
}

void PartyTypeinfo(CNetMsg::SP& msg, char cTypeAll, char cType1, char cType2)
{
	msg->Init(MSG_PARTY);
	RefMsg(msg) << (unsigned char) MSG_PARTY_TYPEINFO
				<< cTypeAll
				<< cType1
				<< cType2;
}

void PartyChangeType(CNetMsg::SP& msg, char cPartyType, char cDiviType,char cAllOneSet)
{
	msg->Init(MSG_PARTY);
	RefMsg(msg) << (unsigned char) MSG_PARTY_CHANGETYPE
				<< cPartyType
				<< cDiviType
				<< cAllOneSet;
}
void PartyEndPartyStartMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_PARTY);
	RefMsg(msg) << (unsigned char)MSG_PARTY_ENDPARTY_START;
}

void ConnConnectMsg(CNetMsg::SP& msg, int version, int server, int subno, int count, int* zones)
{
	msg->Init(MSG_CONN_CONNECT);
	RefMsg(msg) << version
				<< server
				<< subno
				<< gserver->m_serverAddr
				<< gserver->m_serverPort
				<< gserver->m_hardcore_flag_in_gameserver
				<< count;
	int i;
	for (i = 0; i < count; i++)
		RefMsg(msg) << zones[i];
}

void ConnLoginMsg(CNetMsg::SP& msg, CDescriptor* d)
{
	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char)MSG_CONN_LOGIN_REQ
				<< d->m_idname
				<< d->m_passwd
				<< gserver->m_serverno
				<< gserver->m_subno
				<< (unsigned char)d->m_playmode
				<< d->getHostString();
}

void ConnLogoutMsg(CNetMsg::SP& msg, const char* id)
{
	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char)MSG_CONN_LOGOUT_REQ
				<< id;
}

void ConnPlayingMsg(CNetMsg::SP& msg, CDescriptor* d, MSG_LOGIN_TYPE mode)
{
	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char)MSG_CONN_PLAYING
				<< d->m_idname
				<< (unsigned char)mode
				<< d->m_pChar->m_pZone->m_index;
}

void NPCRegenMsg(CNetMsg::SP& msg, CNPC* npc, int entityIndex)
{
	msg->Init(MSG_NPC_REGEN);

	RefMsg(msg) << npc->m_index
				<< npc->m_idNum
				<< GET_X(npc)
				<< GET_Z(npc)
				// TODO : 높이맵에 따른 height 값 조정
				<< npc->m_pos.m_h //GET_H(npc)
				<< GET_R(npc)
				<< GET_YLAYER(npc)
				<< entityIndex;
}

void MsgrPlayerCountReq(CNetMsg::SP& msg, int server, int subno, int charidx)
{
	msg->Init(MSG_MSGR_REQ);

	RefMsg(msg) << 0
				<< server
				<< subno
				<< (int)-1
				<< (unsigned char)MSG_MSGR_PLAYER_COUNT_REQ
				<< charidx;
}

void MsgrPlayerCountRep(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charidx, int countZone, int* countPC, int* countProduce, int* countShop, int* countPlay)
{
	msg->Init(MSG_MSGR_REP);

	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_PLAYER_COUNT_REP
				<< charidx
				<< countZone;

	int i;
	for (i = 0; i < gserver->m_numZone; i++)
	{
		if (countPC[i] == 0)
			continue ;

		RefMsg(msg) << gserver->m_zones[i].m_index
					<< countPC[i]
					<< countProduce[i]
					<< countShop[i]
					<< countPlay[i];
	}
}

void AssistAddMsg(CNetMsg::SP& msg, CCharacter* ch, int itemidx, int index, char level, int remain
				  , int remainCount
				 )
{
	msg->Init(MSG_ASSIST);

	if ( itemidx < 0 )
		itemidx = -1;

	RefMsg(msg) << (unsigned char)MSG_ASSIST_ADD
				<< GET_CHAR_TYPE(ch)
				<< ch->m_index
				<< itemidx
				<< index
				<< level
				<< remain
				<< remainCount
				;
}

void AssistModifyMsg(CNetMsg::SP& msg, CCharacter* ch, int itemidx, int index, char level, int remain, int remainCount)
{
	msg->Init(MSG_ASSIST);

	if (itemidx < 0)
		itemidx = -1;

	RefMsg(msg) << (unsigned char)MSG_ASSIST_MODIFY
				<< GET_CHAR_TYPE(ch)
				<< ch->m_index
				<< itemidx
				<< index
				<< level
				<< remain
				<< remainCount;
}

void AssistDelMsg(CNetMsg::SP& msg, CCharacter* ch, int itemidx, int index)
{
	msg->Init(MSG_ASSIST);

	RefMsg(msg) << (unsigned char)MSG_ASSIST_DEL
				<< GET_CHAR_TYPE(ch)
				<< ch->m_index
				<< itemidx
				<< index;
}

void AssistListMsg(CNetMsg::SP& msg, CPC* ch)
{
	msg->Init(MSG_ASSIST);

	RefMsg(msg) << (unsigned char)MSG_ASSIST_LIST;
	ch->m_assist.AppendAssistToNetMsg(msg);
}

// 몹 상태 메시지
void CharStatusMsg(CNetMsg::SP& msg, CCharacter* ch, LONGLONG state2)
{
	switch (ch->m_type)
	{
	case MSG_CHAR_PC:
		{
			UpdateClient::charStatusPc* packet = reinterpret_cast<UpdateClient::charStatusPc*>(msg->m_buf);
			packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
			packet->subType = MSG_SUB_UPDATE_CHAR_STATUS_PC;

			packet->charIndex = ch->m_index;
			packet->hp = ch->m_hp;
			packet->maxHp = ch->m_maxHP;
			packet->mp = ch->m_mp;
			packet->maxMp = ch->m_maxMP;

			CPC* pc = TO_PC(ch);

			packet->pkPenalty = pc->m_pkPenalty;
			packet->pkName = pc->GetPKName();

			packet->state = ch->m_assist.m_state;
			packet->state2 = state2;

			msg->setSize(sizeof(UpdateClient::charStatusPc));
		}
		break;

	case MSG_CHAR_NPC:
		{
			UpdateClient::charStatusNpc* packet = reinterpret_cast<UpdateClient::charStatusNpc*>(msg->m_buf);
			packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
			packet->subType = MSG_SUB_UPDATE_CHAR_STATUS_NPC;

			packet->charIndex = ch->m_index;
			packet->hp = ch->m_hp;
			packet->maxHp = ch->m_maxHP;
			packet->mp = ch->m_mp;
			packet->maxMp = ch->m_maxMP;

			packet->state = ch->m_assist.m_state;
			packet->state2 = state2;

			CNPC* pNPC = TO_NPC(ch);
			if( pNPC->GetOwner() && pNPC == pNPC->GetOwner()->GetSummonNpc(SUMMON_NPC_TYPE_MERCENARY) &&
				pNPC->GetMercenaryClassData() )
			{
				packet->ownerIndex = pNPC->GetOwner()->m_index;
				packet->classIndex = pNPC->GetMercenaryClassData()->nClassIdx;
				packet->melee = pNPC->m_eqMelee;
				packet->defence = pNPC->m_eqDefense;
				packet->magic = pNPC->m_eqMagic;
				packet->resist = pNPC->m_eqResist;
			}

			msg->setSize(sizeof(UpdateClient::charStatusNpc));
		}
		break;

	case MSG_CHAR_PET:
		{
			UpdateClient::charStatusPet* packet = reinterpret_cast<UpdateClient::charStatusPet*>(msg->m_buf);
			packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
			packet->subType = MSG_SUB_UPDATE_CHAR_STATUS_PET;

			packet->charIndex = ch->m_index;
			packet->hp = ch->m_hp;
			packet->maxHp = ch->m_maxHP;
			packet->mp = ch->m_mp;
			packet->maxMp = ch->m_maxMP;

			packet->state = ch->m_assist.m_state;
			packet->state2 = state2;

			msg->setSize(sizeof(UpdateClient::charStatusPet));
		}
		break;

	case MSG_CHAR_ELEMENTAL:
		{
			UpdateClient::charStatusElemental* packet = reinterpret_cast<UpdateClient::charStatusElemental*>(msg->m_buf);
			packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
			packet->subType = MSG_SUB_UPDATE_CHAR_STATUS_ELEMENTAL;

			packet->charIndex = ch->m_index;
			packet->hp = ch->m_hp;
			packet->maxHp = ch->m_maxHP;
			packet->mp = ch->m_mp;
			packet->maxMp = ch->m_maxMP;

			packet->state = ch->m_assist.m_state;
			packet->state2 = state2;

			msg->setSize(sizeof(UpdateClient::charStatusElemental));
		}
		break;
	}
}

void RebirthMsg(CNetMsg::SP& msg, CCharacter* ch)
{
	msg->Init(MSG_PC_REBIRTH);

	RefMsg(msg) << GET_CHAR_TYPE(ch)
				<< ch->m_index
				<< ch->m_hp << ch->m_maxHP
				<< ch->m_mp << ch->m_maxMP;
}

void EffectEtcMsg(CNetMsg::SP& msg, CCharacter* ch, MSG_EFFECT_ETC_TYPE effectType)
{
	msg->Init(MSG_EFFECT);

	RefMsg(msg) << (unsigned char) MSG_EFFECT_ETC
				<< GET_CHAR_TYPE(ch)
				<< ch->m_index
				<< (unsigned char)effectType;
}

void EffectSkillMsg(CNetMsg::SP& msg, CCharacter* ch, const CSkillProto* proto)
{
	msg->Init(MSG_EFFECT);

	RefMsg(msg) << (unsigned char) MSG_EFFECT_SKILL
				<< proto->m_index
				<< GET_CHAR_TYPE(ch)
				<< ch->m_index;
}

void EffectProduceMsg(CNetMsg::SP& msg, CCharacter* ch, CCharacter* tch, MSG_PRODUCE_KIND kind)
{
	msg->Init(MSG_EFFECT);

	RefMsg(msg) << (unsigned char) MSG_EFFECT_PRODUCE
				<< (char)kind
				<< GET_CHAR_TYPE(ch)
				<< ch->m_index
				<< GET_CHAR_TYPE(tch)
				<< tch->m_index
				<< tch->m_hp;
}

void EffectItemMsg(CNetMsg::SP& msg, CCharacter* ch, CItem* item)
{
	msg->Init(MSG_EFFECT);

	RefMsg(msg) << (unsigned char) MSG_EFFECT_ITEM
				<< GET_CHAR_TYPE(ch)
				<< ch->m_index
				<< item->m_itemProto->getItemIndex();
}

void EffectFireReadyMsg(CNetMsg::SP& msg, int extra, int count, int* effectNPC, float* x, float* z, float* h)
{
	msg->Init(MSG_EFFECT);
	RefMsg(msg) << (unsigned char) MSG_EFFECT_FIRE
				<< extra
				<< count;

	int i;
	for(i = 0 ; i < count; ++i)
	{
		RefMsg(msg) << effectNPC[i] << x[i] << z[i] << h[i];
	}
}

void ConnRebootReqMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_CONN_REQ);

	RefMsg(msg) << (unsigned char)MSG_CONN_REBOOT_REQ;
}

void ConnPreopenGiftMsg(CNetMsg::SP& msg, int userindex, int charindex, bool bCancel)
{
	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char)MSG_CONN_PREOPEN_GIFT
				<< userindex
				<< charindex;
	if (bCancel)
		RefMsg(msg) << (char)1;
	else
		RefMsg(msg) << (char)0;
}

void ConnCashItemPresentHistoryReq(CNetMsg::SP& msg, bool bSend, int userindex, int charindex, int y, char m, char d)
{
	msg->Init(MSG_CONN_REQ);
	if( bSend )
		RefMsg(msg) << (unsigned char) MSG_CONN_CASHITEM_GIFT_SENDHISTORY;
	else
		RefMsg(msg) << (unsigned char) MSG_CONN_CASHITEM_GIFT_RECVHISTORY;

	RefMsg(msg) << userindex
				<< charindex
				<< y
				<< m
				<< d;
}

void ConnCashItemGiftReqMsg(CNetMsg::SP& msg, int sendUserIndex, int sendCharIndex, const char* sendCharName, const char* sendMsg, int recvuserIndex, int recvcharIndex, const char* recvcharName, int count, int idx[], int ctid[])
{
	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char) MSG_CONN_CASHITEM_GIFT
				<< sendUserIndex
				<< sendCharIndex
				<< sendCharName
				<< sendMsg
				<< recvuserIndex
				<< recvcharIndex
				<< recvcharName
				<< count;

	for(int  i = 0; i < count; i++ )
	{
		RefMsg(msg) << idx[i]
					<< ctid[i];
	}
}

void ConnCashItemGiftRecvListReqMsg(CNetMsg::SP& msg, int userIndex, int charIndex )
{
	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char) MSG_CONN_CASHITEM_GIFT_RECVLIST
				<< userIndex
				<< charIndex;
}

void ConnCashItemPurchaseHistoryReq(CNetMsg::SP& msg, int userindex, int charindex, int y, char m, char d)
{
	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char) MSG_CONN_CASHITEM_PURCHASEHISTORY_REQ
				<< userindex
				<< charindex
				<< y
				<< m
				<< d;
}

void ConnCashItemPurchaselistReq(CNetMsg::SP& msg, int userindex, int charindex)
{
	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char) MSG_CONN_CASHITEM_PURCHASELIST_REQ
				<< userindex
				<< charindex;
}
void ConnCashItemBringReq(CNetMsg::SP& msg, bool bPresent, int userindex, int charindex, int count, int idx[], int ctid[])
{
	msg->Init(MSG_CONN_REQ);
	if( bPresent )
		RefMsg(msg) << (unsigned char) MSG_CONN_CASHITEM_GIFT_RECV;
	else
		RefMsg(msg) << (unsigned char) MSG_CONN_CASHITEM_BRING_REQ;

	RefMsg(msg) << userindex
				<< charindex
				<< count;

	for(int i = 0; i < count; i++)
	{
		RefMsg(msg) << idx[i]
					<< ctid[i];
	}
}

void ConnCashItemBalanceReq(CNetMsg::SP& msg, int userindex, const char* idname)
{
	// userindex, idname
	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char) MSG_CONN_CASHITEM_BALANCE_REQ
				<< userindex
				<< idname;
}

void ConnCashItemPurchaseReqMsg(CNetMsg::SP& msg, int userindex, const char* idname, int charindex, int cash, int count, int ctid[], int requestCash)
{
	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char) MSG_CONN_CASHITEM_PURCHASE_REQ
				<< userindex
				<< idname
				<< charindex
				<< cash;

	int i;
	// 물품 구매 요청이 오면
	// 한정 판매 물품인지 구분하여 한정 판매 물품이면 분류하여 뒤에 붙인다.
	std::vector<int> limitList;
	CNetMsg ctlist;
	ctlist.Init();

	for(i = 0; i < count; i++)
	{
		std::set<int>::iterator it = gserver->m_limitCT.find(ctid[i]);
		if( it != gserver->m_limitCT.end() )
		{
			limitList.push_back(ctid[i]);
		}
		else
		{
			ctlist << ctid[i];
		}
	}
	int lcount = limitList.size();
	count -= lcount;
	RefMsg(msg) << count;
	RefMsg(msg) << ctlist;

	RefMsg(msg) << lcount;

	std::vector<int>::iterator vit = limitList.begin();
	std::vector<int>::iterator vendit = limitList.end();
	for(; vit != vendit; ++vit)
	{
		RefMsg(msg) << (int)(*vit);
	}

#ifdef USE_TENTER_BILLING
	RefMsg(msg) << requestCash;
#endif // USE_TENTER_BILLING
}

void ExpSPMsg(CNetMsg::SP& msg, LONGLONG exp, int sp)
{
	msg->Init(MSG_EXP_SP);

	RefMsg(msg) << exp
				<< sp;
}

void QuestPCListMsg(CNetMsg::SP& msg, CPC* pc)
{
	msg->Init(MSG_QUEST);

	int i;

	RefMsg(msg) << (unsigned char) MSG_QUEST_PC_LIST
				<< pc->m_questList.GetCountRun();

	CQuest* pQuest;
	CQuest* pQuestNext = pc->m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
	while ((pQuest = pQuestNext))
	{
		if (pQuest->GetQuestProto()->m_index == 682 || pQuest->GetQuestProto()->m_index == 683)
		{
			if (pc->getSyndicateType() > 0)
			{
				pQuestNext = pc->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
				continue;
			}
		}

		pQuestNext = pc->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
		RefMsg(msg) << pQuest->GetQuestIndex();
		for (i = 0; i <	QUEST_MAX_CONDITION; i++)
			RefMsg(msg) << pQuest->GetQuestValue(i);
	}
}

void QuestCompleteListMsg(CNetMsg::SP& msg, CPC* pc)
{
	msg->Init(MSG_QUEST);

	RefMsg(msg) << (unsigned char) MSG_QUEST_DONE_LIST
				<< pc->m_questList.GetCountDone();

	CQuest* pQuest;
	CQuest* pQuestNext = pc->m_questList.GetNextQuest(NULL, QUEST_STATE_DONE);
	while ((pQuest = pQuestNext))
	{
		pQuestNext = pc->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_DONE);
		RefMsg(msg) << pQuest->GetQuestIndex();
	}
}

void QuestAbandonListMsg(CNetMsg::SP& msg, CPC* pc)
{
	msg->Init(MSG_QUEST);

	RefMsg(msg) << (unsigned char) MSG_QUEST_ABANDON_LIST
				<< pc->m_questList.GetCountAbandon();

	CQuest* pQuest;
	CQuest* pQuestNext = pc->m_questList.GetNextQuest(NULL, QUEST_STATE_ABANDON);
	while ((pQuest = pQuestNext))
	{
		pQuestNext = pc->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_ABANDON);
		RefMsg(msg) << pQuest->GetQuestIndex();
	}
}

void QuestNPCListMsg(CNetMsg::SP& msg, int total, int* flag, int* index)
{
	msg->Init(MSG_QUEST);

	RefMsg(msg) << (unsigned char) MSG_QUEST_NPC_LIST
				<< total;

	int i;
	for (i=0; i < total; i++)
	{
		RefMsg(msg) << index[i]
					<< (char)flag[i];
	}
}

void QuestCreateListMsg(CNetMsg::SP& msg, int count, int* index)
{
	msg->Init(MSG_QUEST);

	RefMsg(msg) << (unsigned char) MSG_QUEST_CREATE_LIST
				<< count;

	int i;
	for (i=0; i < count; i++)
	{
		RefMsg(msg) << index[i];
	}
}

void QuestStartMsg(CNetMsg::SP& msg, CQuest* quest)
{
	msg->Init(MSG_QUEST);

	RefMsg(msg) << (unsigned char)MSG_QUEST_START
				<< quest->GetQuestIndex();
	RefMsg(msg) << quest->GetQuestProto()->m_failValue;
}

void QuestCompleteMsg(CNetMsg::SP& msg, CQuest* quest)
{
	msg->Init(MSG_QUEST);

	RefMsg(msg) << (unsigned char) MSG_QUEST_COMPLETE
				<< quest->GetQuestIndex();
}

void QuestStatusMsg(CNetMsg::SP& msg, CQuest* quest)
{
	msg->Init(MSG_QUEST);

	RefMsg(msg) << (unsigned char)MSG_QUEST_STATUS
				<< quest->GetQuestIndex();

	int i;
	for (i=0; i < QUEST_MAX_CONDITION; i++)
		RefMsg(msg) << quest->GetQuestValue(i);
}

void QuestPrizeMsg(CNetMsg::SP& msg, CQuest* quest)
{
	msg->Init(MSG_QUEST);

	RefMsg(msg) << (unsigned char) MSG_QUEST_PRIZE
				<< quest->GetQuestIndex();
}

void QuestGiveUpMsg(CNetMsg::SP& msg, CQuest* quest)
{
	msg->Init(MSG_QUEST);

	RefMsg(msg) << (unsigned char) MSG_QUEST_GIVEUP
				<< quest->GetQuestIndex();
}

void QuestGiveUpMsg(CNetMsg::SP& msg, int questIndex)
{
	msg->Init(MSG_QUEST);

	RefMsg(msg) << (unsigned char) MSG_QUEST_GIVEUP
				<< questIndex;
}

void QuestForceGiveUpMsg(CNetMsg::SP& msg, CQuest* quest)
{
	msg->Init(MSG_QUEST);

	RefMsg(msg) << (unsigned char) MSG_QUEST_FORCE_GIVEUP
				<< quest->GetQuestIndex();
}

void QuestFailMsg(CNetMsg::SP& msg, CQuest* quest)
{
	msg->Init(MSG_QUEST);

	RefMsg(msg) << (unsigned char) MSG_QUEST_FAIL
				<< quest->GetQuestIndex();
}

void QuestErrorMsg(CNetMsg::SP& msg, MSG_QUEST_ERROR_TYPE type)
{
	msg->Init(MSG_QUEST);

	RefMsg(msg) << (unsigned char) MSG_QUEST_ERROR
				<< (unsigned char) type;
}

void QuestPD4ErrorMsg(CNetMsg::SP& msg, MSG_QUEST_PD4_ERROR_TYPE type)
{
	msg->Init(MSG_QUEST);

	RefMsg(msg) << (unsigned char) MSG_QUEST_PD4_ERROR
				<< (unsigned char) type;
}

void QuestRestoreAbandonMsg(CNetMsg::SP& msg, int nRestoreCount, int nRestoreIndex[])
{
	msg->Init(MSG_QUEST);
	RefMsg(msg) << (unsigned char) MSG_QUEST_RESTORE_ABANDON
				<< nRestoreCount;
	for (int i=0; i < nRestoreCount; i++)
		RefMsg(msg) << nRestoreIndex[i];
}

void StatPointRemainMsg(CNetMsg::SP& msg, CPC* ch)
{
	msg->Init(MSG_STATPOINT);
	RefMsg(msg) << (unsigned char)MSG_STATPOINT_REMAIN
				<< ch->m_statpt_remain;
}

void StatPointUseMsg(CNetMsg::SP& msg, CPC* ch, MSG_STATPOINT_USE_TYPE type, int value)
{
	msg->Init(MSG_STATPOINT);
	RefMsg(msg) << (unsigned char)MSG_STATPOINT_USE
				<< (unsigned char)type
				<< value
				<< ch->m_statpt_remain;
}

void StatPointErrorMsg(CNetMsg::SP& msg, MSG_STATPOINT_ERROR_TYPE errcode)
{
	msg->Init(MSG_STATPOINT);
	RefMsg(msg) << (unsigned char)MSG_STATPOINT_ERROR
				<< (char)errcode;
}

void WarpStartMsg(CNetMsg::SP& msg, CPC* ch)
{
	msg->Init(MSG_WARP);
	RefMsg(msg) << (unsigned char)MSG_WARP_START
				<< ch->m_index;
}

void WarpCancelMsg(CNetMsg::SP& msg, CPC* ch)
{
	msg->Init(MSG_WARP);
	RefMsg(msg) << (unsigned char)MSG_WARP_CANCEL
				<< ch->m_index;
}

void WarpEndMsg(CNetMsg::SP& msg, CPC* ch)
{
	msg->Init(MSG_WARP);
	RefMsg(msg) << (unsigned char)MSG_WARP_END
				<< ch->m_index;
}

void WarpErrorMsg(CNetMsg::SP& msg, MSG_WARP_ERROR_TYPE subtype, const char *name)
{
	msg->Init(MSG_WARP);
	RefMsg(msg) << (unsigned char) MSG_WARP_ERROR
				<< (unsigned char) subtype
				<< name;
}

void WarpReqIngMsg(CNetMsg::SP& msg, MSG_WARP_TYPE subtype)
{
	msg->Init(MSG_WARP);
	RefMsg(msg) << (unsigned char) subtype;
}

void WarpReqMsg(CNetMsg::SP& msg, CPC* ch, MSG_WARP_TYPE subtype)
{
	msg->Init(MSG_WARP);
	RefMsg(msg) << (unsigned char) subtype
				<< ch->GetName();
}

void PKItemSealMsg(CNetMsg::SP& msg, CItem* item)
{
	msg->Init(MSG_PK);
	RefMsg(msg) << (unsigned char)MSG_PK_ITEMSEAL
				<< (unsigned short)item->tab()
				<< (unsigned short)item->getInvenIndex()
				<< item->getVIndex()
				<< item->getFlag();
}

void PKRecoverItemSealMsg(CNetMsg::SP& msg, CItem* item)
{
	msg->Init(MSG_PK);
	RefMsg(msg) << (unsigned char)MSG_PK_RECOVER_ITEMSEAL
				<< (unsigned short)item->tab()
				<< (unsigned short)item->getInvenIndex()
				<< item->getVIndex()
				<< item->getFlag();
}

void PKErrorMsg(CNetMsg::SP& msg, char errcode)
{
	msg->Init(MSG_PK);
	RefMsg(msg) << (unsigned char)MSG_PK_ERROR
				<< errcode;
}

void EventErrorMsg(CNetMsg::SP& msg, MSG_EVENT_ERROR_TYPE errcode)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_ERROR
				<< (unsigned char)errcode;
}

void PersonalShopErrorMsg(CNetMsg::SP& msg, MSG_PERSONALSHOP_ERROR_TYPE errcode)
{
	msg->Init(MSG_PERSONALSHOP);
	RefMsg(msg) << (unsigned char)MSG_PERSONALSHOP_ERROR
				<< (unsigned char)errcode;
}

void PersonalShopSellStartMsg(CNetMsg::SP& msg, CPC* ch)
{
	msg->Init(MSG_PERSONALSHOP);
	RefMsg(msg) << (unsigned char)MSG_PERSONALSHOP_SELL_START
				<< ch->m_index
				<< ch->m_personalshop->GetType()
				<< ch->m_personalshop->GetName();
}

void PersonalShopSellListMsg(CNetMsg::SP& msg, CPC* ch)
{
	CPersonalShop* ps = ch->m_personalshop.get();

	if (ps == NULL)
	{
		msg->Init();
		return ;
	}

	char normalcount = ps->GetNormalCount();

	msg->Init(MSG_PERSONALSHOP);
	RefMsg(msg) << (unsigned char)MSG_PERSONALSHOP_SELL_LIST
				<< ch->m_index
				<< ps->GetType()
				<< normalcount;

	if (normalcount)
	{
		int i;
		int pos = -1;

		for (i = 0; i < normalcount; i++)
		{
			pos = ps->GetNextNormalItem(pos);
			if (pos == -1)
			{
				msg->Init();
				return ;
			}

			int itemindex = ps->GetNormalItemIndex(pos);
			CItem* pitem = ch->m_inventory.FindByVirtualIndex(itemindex);
			if (pitem == NULL)
			{
				msg->Init();
				return ;
			}

			LONGLONG count = ps->GetNormalItemCount(pos);
			LONGLONG price = ps->GetNormalItemPrice(pos);
			RefMsg(msg) << itemindex
						<< pitem->getDBIndex()
						<< pitem->getPlus()
						<< pitem->getFlag()
						<< pitem->getNowDurability()
						<< pitem->getMaxDurability();
			if (pitem->IsOriginItem())
			{
				RefMsg(msg) << (char)pitem->GetOriginOptionCnt();
				for (int j = 0; j < MAX_ORIGIN_OPTION; j++)
				{
					if (pitem->GetOriginIndex(j) > -1)
					{
						RefMsg(msg) << (char)pitem->GetOriginIndex(j)
									<< (int)pitem->GetOriginLevel(j);
						RefMsg(msg) << pitem->getOriginVar(j);
					}
				}
			}
			else
			{
				RefMsg(msg) << (char)pitem->m_nOption;
				int j;
				for (j = 0; j < pitem->m_nOption; j++)
				{
					RefMsg(msg) << (char)pitem->m_option[j].m_type
								<< (int)pitem->m_option[j].m_level;
				}
			}
			RefMsg(msg) << count
						<< price
						<< pitem->getUsed()
						<< pitem->getUsed_2();

			if (pitem->IsOriginItem())
			{
				RefMsg(msg) << (char)pitem->GetBelongType()					// 귀속 설정
							<< (char)pitem->GetOriginSkillCnt();
				for (int j = MAX_ORIGIN_OPTION; j < MAX_ORIGIN; j++)
				{
					if (pitem->GetOriginIndex(j) > -1)
					{
						RefMsg(msg) << pitem->GetOriginIndex(j)
									<< (char)pitem->GetOriginLevel(j);
					}
				}
			}
			ItemPublicMsg(msg, pitem, false);
			ItemSocketMsg(msg, pitem);
		}
	}

	if (ps->GetType() & PST_PACKAGE)
	{
		char itemcount = ps->GetPackageCount();
		RefMsg(msg) << ps->GetPackagePrice()
					<< itemcount;
		int i;
		int pos = -1;
		for (i = 0; i < itemcount; i++)
		{
			pos = ps->GetNextPackageItem(pos);
			if (pos == -1)
			{
				msg->Init();
				return ;
			}

			int itemindex = ps->GetPackageItemIndex(pos);
			CItem* pitem = ch->m_inventory.FindByVirtualIndex(itemindex);
			if (pitem == NULL)
			{
				msg->Init();
				return ;
			}

			LONGLONG count = ps->GetPackageItemCount(pos);
			RefMsg(msg) << itemindex
						<< pitem->getDBIndex()
						<< pitem->getPlus()
						<< pitem->getFlag()
						<< pitem->getNowDurability()
						<< pitem->getMaxDurability();
			if (pitem->IsOriginItem())
			{
				RefMsg(msg) << (char)pitem->GetOriginOptionCnt();
				for (int j = 0; j < MAX_ORIGIN_OPTION; j++)
				{
					if (pitem->GetOriginIndex(j) > -1)
					{
						RefMsg(msg) << (char)pitem->GetOriginIndex(j)
									<< (int)pitem->GetOriginLevel(j);
						RefMsg(msg) << pitem->getOriginVar(j);
					}
				}
			}
			else
			{
				RefMsg(msg) << (char)pitem->m_nOption;
				int j;
				for (j = 0; j < pitem->m_nOption; j++)
				{
					RefMsg(msg) << (char)pitem->m_option[j].m_type
								<< (int)pitem->m_option[j].m_level;
				}
			}

			RefMsg(msg) << count
						<< pitem->getUsed()
						<< pitem->getUsed_2();

			if (pitem->IsOriginItem())
			{
				RefMsg(msg) << (char)pitem->GetBelongType()
							<< (char)pitem->GetOriginSkillCnt();
				for (int j = MAX_ORIGIN_OPTION; j < MAX_ORIGIN; j++)
				{
					if (pitem->GetOriginIndex(j) > -1)
					{
						RefMsg(msg) << pitem->GetOriginIndex(j)
									<< (char)pitem->GetOriginLevel(j);
					}
				}
			}
			ItemPublicMsg(msg, pitem, false);
			ItemSocketMsg(msg, pitem);
		}
	}
}

void PersonalShopChangeMsg(CNetMsg::SP& msg, CPC* ch)
{
	msg->Init(MSG_PERSONALSHOP);
	RefMsg(msg) << (unsigned char)MSG_PERSONALSHOP_CHANGE;
	RefMsg(msg) << ch->m_index;
	if (ch->m_personalshop)
		RefMsg(msg) << ch->m_personalshop->GetType();
	else
		RefMsg(msg) << (char)PST_NOSHOP;
}

void PersonalShopBuyMsg(CNetMsg::SP& msg, CNetMsg::SP& buymsg, CPC* tch)
{
	msg->Init(buymsg);
	msg->MoveFirst();
	RefMsg(msg) << (unsigned char)MSG_PERSONALSHOP_BUY;
	RefMsg(msg) << tch->m_index;
}

void RightAttackMsg(CNetMsg::SP& msg, CPC* target, MSG_RIGHT_ATTACK_TYPE type)
{
	msg->Init(MSG_RIGHT_ATTACK);

	RefMsg(msg) << (unsigned char) type
				<< GET_CHAR_TYPE(target)
				<< target->m_index;
}

void GuildErrorMsg(CNetMsg::SP& msg, MSG_GUILD_ERROR_TYPE errcode, CGuild * guild)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_ERROR
				<< (char)errcode;

	switch( errcode )
	{
	case MSG_NEW_GUILD_ERROR_ADJUST_OK:
	case MSG_GUILD_ERROR_KICK_OK:
	case MSG_GUILD_ERROR_OUT_OK:
		{
			if( guild )
			{
				RefMsg(msg) << guild->m_nCountTotalGradeEx
							<< guild->m_nCountRushCaptain+guild->m_nCountSupportCaptain+guild->m_nCountReconCaptain;
			}
			else
			{
				RefMsg(msg) << 0 << 0;
			}
			break;
		}
	default:
		break;
	}
}

void GuildInfoMsg(CNetMsg::SP& msg, CPC* pc)
{
	// TODO : GUILD : 트리 구성시 트리 정보 보내기
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_INFO;
	if (pc->m_guildInfo)
	{
		CGuild* guild = pc->m_guildInfo->guild();
		RefMsg(msg) << guild->index()
					<< guild->name()
					<< guild->level()
					<< pc->m_guildInfo->pos()
					<< pc->m_guildInfo->GetGuildMark();

		RefMsg(msg) << pc->m_guildInfo->GetPositionName();

		if( guild->m_passiveSkillList.Find( 444 ) )
		{
			RefMsg(msg) << guild->m_passiveSkillList.Find( 444 )->m_level;
		}
		else
		{
			RefMsg(msg) << (char)0;
		}
#ifdef DEV_GUILD_MARK
		RefMsg(msg) << (char)guild->GetGuildMarkRow()
					<< (char)guild->GetGuildMarkCol()
					<< (char)guild->GetBackgroundRow()
					<< (char)guild->GetBackgroundCol();
#endif
		RefMsg(msg) << guild->getGuildKick()->getKickStatus();
	}
	else
	{
		RefMsg(msg) << (int)-1;
	}
}

void GuildListMsg(CNetMsg::SP& msg, CPC* pc)
{
	if (pc->m_guildInfo == NULL)
	{
		msg->Init();
		return ;
	}

	// 길드 있을때
	CGuild* guild = pc->m_guildInfo->guild();

	int i;
	CGuildMember* member;

	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_MEMBERLIST;
	RefMsg(msg) << guild->index()
				<< guild->membercount();
	for (i = 0; i < GUILD_MAX_MEMBER; i++)
	{
		member = guild->member(i);
		if (member)
		{
			RefMsg(msg) << member->charindex()
						<< member->GetName()
						<< member->pos()
						<< member->online();
		}
	}
}

void HelperShutdownMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_SHUTDOWN;
}

void HelperRepWhisperNotFoundMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sindex, const char* sname)
{
	msg->Init(MSG_HELPER_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_HELPER_WHISPER_NOTFOUND
				<< sindex << sname;
}

void HelperRepWhisperRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sindex, const char* sname, const char* rname, const char* chat)
{
	msg->Init(MSG_HELPER_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_HELPER_WHISPER_REP
				<< sindex << sname
				<< rname
				<< chat;
}

void HelperGuildCreateReqMsg(CNetMsg::SP& msg, CPC* ch, const char* name)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_CREATE_REQ
				<< ch->m_index
				<< ch->GetName()
				<< name;
}

void GuildOnlineMsg(CNetMsg::SP& msg, CGuildMember* member)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_ONLINE
				<< member->guild()->index()
				<< member->charindex()
				<< member->GetName()
				<< member->online();
}

void HelperConnectMsg(CNetMsg::SP& msg, int version, int server, int subno, int count, int* zones)
{
	msg->Init(MSG_HELPER_CONNECT);
	RefMsg(msg) << version
				<< server
				<< subno
				<< count;
	int i;
	for (i = 0; i < count; i++)
		RefMsg(msg) << zones[i];
}

void HelperGuildOnline(CNetMsg::SP& msg, CGuildMember* member)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_ONLINE
				<< member->guild()->index()
				<< member->charindex()
				<< member->online();
}

void HelperGuildLoadReq(CNetMsg::SP& msg, const char* idname, int charindex, int channel, int zoneindex )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_GUILD_LOAD_REQ
				<< idname
				<< charindex
				<< channel
				<< zoneindex;
}

void HelperGuildLevelUpReqMsg(CNetMsg::SP& msg, int guildindex, int charindex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_LEVELUP_REQ
				<< guildindex
				<< charindex;
}

void GuildLevelInfoMsg(CNetMsg::SP& msg, int guildindex, int guildlevel)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_LEVELINFO
				<< guildindex
				<< guildlevel;
}

void HelperGuildBreakUpReqMsg(CNetMsg::SP& msg, CPC* boss)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_BREAKUP_REQ
				<< boss->m_index
				<< boss->m_guildInfo->guild()->index();
}

void GuildBreakUpNotifyMsg(CNetMsg::SP& msg, int guildindex, const char* guildname)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_BREAKUP_NOTIFY
				<< guildindex
				<< guildname;
}

void GuildRegistCancelMsg(CNetMsg::SP& msg, bool bTarget)
{
	char target = (bTarget) ? 1 : 0;
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_REGIST_CANCEL
				<< (char)target;
}

void HelperGuildMemberAddReqMsg(CNetMsg::SP& msg, int guildindex, int targetindex, int requester, const char* name)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_MEMBER_ADD_REQ
				<< guildindex
				<< targetindex
				<< requester
				<< name;
}

void GuildMemberAddMsg(CNetMsg::SP& msg, int guildindex, int charindex, const char* name)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_MEMBER_ADD
				<< guildindex
				<< charindex
				<< name;
}

void HelperGuildOutReqMsg(CNetMsg::SP& msg, int guildindex, int charindex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_MEMBER_OUT_REQ
				<< guildindex
				<< charindex;
}

void GuildMemberOutMsg(CNetMsg::SP& msg, int guildindex, int charindex, const char* name)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_MEMBER_OUT
				<< guildindex
				<< charindex
				<< name;
}

void HelperGuildKickReqMsg(CNetMsg::SP& msg, int guildindex, int bossindex, int charindex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_MEMBER_KICK_REQ
				<< guildindex
				<< bossindex
				<< charindex;
}

void GuildMemberKickMsg(CNetMsg::SP& msg, int guildindex, int bossindex, int charindex, const char* name)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_MEMBER_KICK
				<< guildindex
				<< bossindex
				<< charindex
				<< name;
}

void GuildInfoChangeMsg(CNetMsg::SP& msg, int charindex, int guildindex, const char* name, MSG_GUILD_POSITION_TYPE pos, CPC* ch)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_INFO_CHANGE
				<< charindex
				<< guildindex
				<< name
				<< pos;
	if(ch && ch->m_guildInfo)
	{
		RefMsg(msg) << ch->m_guildInfo->GetGuildMark();

#ifdef DEV_GUILD_MARK
		if(ch->m_guildInfo->guild())
		{
			RefMsg(msg) << ch->m_guildInfo->guild()->GetGuildMarkRow()
						<< ch->m_guildInfo->guild()->GetGuildMarkCol()
						<< ch->m_guildInfo->guild()->GetBackgroundRow()
						<< ch->m_guildInfo->guild()->GetBackgroundCol();
		}
#endif
	}
}

void HelperGuildChangeBossReqMsg(CNetMsg::SP& msg, int guildindex, int current, int change)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_CHANGE_BOSS_REQ
				<< guildindex
				<< current
				<< change;
}

void GuildChangeBossMsg(CNetMsg::SP& msg, int guildindex, int current, int change)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_CHANGE_BOSS
				<< guildindex
				<< current
				<< change;
}

void HelperGuildAppointOfficerReqMsg(CNetMsg::SP& msg, int guildindex, int bossindex, int charindex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_APPOINT_OFFICER_REQ
				<< guildindex
				<< bossindex
				<< charindex;
}

void GuildAppointOfficerMsg(CNetMsg::SP& msg, int guildindex, int charindex)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_APPOINT_OFFICER
				<< guildindex
				<< charindex;
}

void HelperGuildChat(CNetMsg::SP& msg, int guildindex, int charindex, const char* charname, const char* chat)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_CHAT
				<< guildindex
				<< charindex
				<< charname
				<< chat;
}

void HelperGuildFireOfficerReqMsg(CNetMsg::SP& msg, int guildindex, int bossindex, int charindex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_FIRE_OFFICER_REQ
				<< guildindex
				<< bossindex
				<< charindex;
}

void HelperGuildBattleReqMsg(CNetMsg::SP& msg, int guildindex1, int guildindex2, int prize, int zone, int time)
{
	msg->Init(MSG_HELPER_COMMAND);

	RefMsg(msg) << MSG_HELPER_GUILD_BATTLE_REQ
				<< guildindex1
				<< guildindex2
				<< prize
				<< zone
				<< time;
}

void HelperGuildBattleStopReqMsg(CNetMsg::SP& msg, int guildindex1, int guildindex2)
{
	msg->Init(MSG_HELPER_COMMAND);

	RefMsg(msg) << MSG_HELPER_GUILD_BATTLE_STOP_REQ
				<< guildindex1
				<< guildindex2;
}

void HelperGuildBattlePeaceReqMsg(CNetMsg::SP& msg, CGuild* g)
{
	msg->Init(MSG_HELPER_COMMAND);

	RefMsg(msg) << MSG_HELPER_GUILD_BATTLE_PEACE_REQ
				<< g->index();
}

void HelperGuildBattleKillReqMsg(CNetMsg::SP& msg, int of_guildindex, int df_guildindex)
{
	msg->Init(MSG_HELPER_COMMAND);

	RefMsg(msg) << MSG_HELPER_GUILD_BATTLE_KILL_REQ
				<< of_guildindex
				<< df_guildindex;
}
//0503 kwon
void HelperEventMoonStoneUpdateReqMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_HELPER_COMMAND);

	RefMsg(msg) << MSG_HELPER_EVENT_MOONSTONE_UPDATE_REQ;
}

void HelperEventMoonStoneJackPotReqMsg(CNetMsg::SP& msg, int chaindex)
{
	msg->Init(MSG_HELPER_COMMAND);

	RefMsg(msg) << MSG_HELPER_EVENT_MOONSTONE_JACKPOT_REQ
				<< chaindex;
}

void GuildFireOfficerMsg(CNetMsg::SP& msg, int guildindex, int charindex)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_FIRE_OFFICER
				<< guildindex
				<< charindex;
}

void GuildBattleReqReqMsg(CNetMsg::SP& msg, int guildindex, const char* name, int prize, int time)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char) MSG_GUILD_BATTLE_REQ_REQ
				<< guildindex
				<< name
				<< prize
				<< time;
}

void GuildBattleReqRejectMsg(CNetMsg::SP& msg, int reject_charindex)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char) MSG_GUILD_BATTLE_REQ_REJECT
				<< reject_charindex;
}

void GuildBattleReqAccpetMsg(CNetMsg::SP& msg, int guildindex1, const char* name1, int guildindex2, const char* name2, int prize, int zone, int time)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char) MSG_GUILD_BATTLE_REQ_ACCEPT
				<< guildindex1
				<< name1
				<< guildindex2
				<< name2
				<< prize
				<< zone
				<< time / 10;
}

void GuildBattleStopReqMsg(CNetMsg::SP& msg, int guildindex, const char* name)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char) MSG_GUILD_BATTLE_STOP_REQ
				<< guildindex
				<< name;
}

void GuildBattleStopRejectMsg(CNetMsg::SP& msg, int reject_charindex)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char) MSG_GUILD_BATTLE_STOP_REJECT
				<< reject_charindex;
}

void GuildBattleEndMsg(CNetMsg::SP& msg, int winner_guildindex, int guildindex1, const char* name1, int guildindex2, const char* name2, int prize)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char) MSG_GUILD_BATTLE_END
				<< winner_guildindex
				<< guildindex1
				<< name1
				<< guildindex2
				<< name2
				<< prize;
}

void GuildBattleStatusMsg(CNetMsg::SP& msg, int guildindex1, const char* name1, int killcount1, int guildindex2, const char* name2, int killcount2, int battletime, int battleZone)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char) MSG_GUILD_BATTLE_STATUS
				<< guildindex1
				<< name1
				<< killcount1
				<< guildindex2
				<< name2
				<< killcount2
				<< battletime / 10
				<< battleZone;
}

void GuildBattleErrMsg(CNetMsg::SP& msg, MSG_GUILD_EROOR_BATTLE_TYPE type)
{
	msg->Init(MSG_GUILD);

	RefMsg(msg) << (unsigned char) MSG_GUILD_ERROR
				<< (unsigned char) MSG_GUILD_ERROR_BATTLE
				<< (char) type;
}

void HelperCharDelMsg(CNetMsg::SP& msg, int charindex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_CHAR_DEL
				<< charindex;
}

void ChangeErrMsg(CNetMsg::SP& msg, MSG_CHANGE_ERR_TYPE type)
{
	msg->Init (MSG_CHANGE);

	RefMsg(msg) << (unsigned char) MSG_CHANGE_ERR
				<< (unsigned char) type;
}

void ChangeStartMsg(CNetMsg::SP& msg, CPC* ch)
{
	msg->Init (MSG_CHANGE);

	RefMsg(msg) << (unsigned char) MSG_CHANGE_START
				<< ch->m_index
				<< ch->m_changeIndex;
}

void ChangeStopMsg(CNetMsg::SP& msg, CPC* ch)
{
	msg->Init (MSG_CHANGE);

	RefMsg(msg) <<(unsigned char) MSG_CHANGE_STOP
				<< ch->m_index;
}

void UIPlusEffectRep(CNetMsg::SP& msg, CPC* ch)
{
	msg->Init(MSG_UI);

	RefMsg(msg) << (unsigned char) MSG_UI_PLUS_EFFECT_REP
				<< ch->m_plusEffect
				<< ch->m_index;
}

void TeachTeacherListMsg(CNetMsg::SP& msg, CPC* ch, MSG_TEACH_TEACHER_LIST_TYPE type, CPC** list)
{
	msg->Init(MSG_TEACH);

	RefMsg(msg) << (unsigned char) MSG_TEACH_TEACHER_LIST
				<< (unsigned char) type;

	if (type == MSG_TEACH_TEACHER_LIST_SHOW)
	{
		int i;
		for (i=0; i < 20; i++)
		{
			if (list[i])
			{
				RefMsg(msg) << list[i]->m_index
							<< list[i]->GetName()
							<< list[i]->m_job;
				RefMsg(msg) << list[i]->m_job2;
				//0627
				RefMsg(msg) << list[i]->m_fame
							<< list[i]->m_cntTeachingStudent
							<< list[i]->m_cntCompleteStudent
							<< list[i]->m_cntFailStudent;
				RefMsg(msg) << (unsigned char)list[i]->m_teachTime[0];
				RefMsg(msg) << (unsigned char)list[i]->m_teachTime[1];
			}
			else
				RefMsg(msg) << -1;
		}
	}
}

void HelperTeachLimitTimeCheck(CNetMsg::SP& msg, int charindex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_TEACHER_SYSTEM_RENEWER
				<< (unsigned char)MSG_HELPER_TEACH_RENEWER_TIME_LIMIT_REQ
				<< charindex;
}

void HelperTeacherGiftAddReqMsg(CNetMsg::SP& msg, int teacherIdx, int count)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_TEACHER_SYSTEM_RENEWER
				<< (unsigned char)MSG_HELPER_TEACH_RENEWER_TEACHER_GIFT_REQ
				<< (unsigned char)MSG_HELPER_TEACH_RENEWER_GIFT_CNT_ADD
				<< teacherIdx
				<< count;
}
void HelperTeacherGiftReqMsg(CNetMsg::SP& msg, int charindex )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_TEACHER_SYSTEM_RENEWER
				<< (unsigned char)MSG_HELPER_TEACH_RENEWER_TEACHER_GIFT_REQ
				<< (unsigned char)MSG_HELPER_TEACH_RENEWER_GIFT
				<< charindex;
}

void HelperTeachStudentSucMsg(CNetMsg::SP& msg, int studentIdx, int teacherIdx)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_TEACHER_SYSTEM_RENEWER
				<< (unsigned char)MSG_HELPER_TEACH_RENEWER_STUDENT_SUC_REQ
				<< studentIdx
				<< teacherIdx;
}

void HelperTeachMsg(CNetMsg::SP& msg, int teachidx, char bTeacher, unsigned char noticetime1, unsigned char noticetime2)
{
	msg->Init(MSG_HELPER_COMMAND);

	RefMsg(msg) << MSG_HELPER_TEACHER_REQ
				<< teachidx
				<< bTeacher;
	RefMsg(msg) << noticetime1
				<< noticetime2;
}

void HelperTeacherGiveup(CNetMsg::SP& msg, int teachidx, int studentidx, int teachType, int giveupCnt)
{
	msg->Init(MSG_HELPER_COMMAND);

	RefMsg(msg) << MSG_HELPER_TEACHER_GIVEUP_REQ
				<< teachidx
				<< studentidx;
	RefMsg(msg) << teachType
				<< giveupCnt;
}

void HelperTeachRegisterMsg(CNetMsg::SP& msg, int teachidx, bool bteacher, int studentidx)
{
	msg->Init(MSG_HELPER_COMMAND);

	RefMsg(msg) << MSG_HELPER_TEACHER_REGISTER_REQ
				<< teachidx
				<< (char) bteacher
				<< studentidx;
}

void HelperTeacherSuperstoneRecieve(CNetMsg::SP& msg, int charindex)
{
	msg->Init(MSG_HELPER_COMMAND);

	RefMsg(msg) << MSG_HELPER_TEACHER_SUPERSTONERECIEVE
				<< charindex;
}

void HelperTeachTimeover(CNetMsg::SP& msg, int flag, int teachidx, int studentidx)
{
	msg->Init(MSG_HELPER_COMMAND);

	RefMsg(msg) << MSG_HELPER_TEACHER_TIMEOVER_REQ
				<< flag
				<< teachidx
				<< studentidx;
}

void HelperTeachLoadReq(CNetMsg::SP& msg, const char* idname, int charindex)
{
	msg->Init(MSG_HELPER_COMMAND);

	RefMsg(msg) << MSG_HELPER_TEACHER_LOAD_REQ
				<< idname
				<< charindex;
}

void HelperTeachFameUpReqMsg(CNetMsg::SP& msg, int teachidx, int studentidx, const char* studentname, int fame)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_TEACHER_FAMEUP_REQ
				<< teachidx
				<< fame;
}

void HelperGiftCharReq(CNetMsg::SP& msg, int sendUserIdx, int sendCharIdx, const char* recvCharName, const char* sendMsg, int count, int idx[], int ctid[] )
{
	msg->Init(MSG_HELPER_COMMAND);

	RefMsg(msg) << MSG_HELPER_GIFT_RECVCHARNAME_REQ
				<< sendUserIdx
				<< sendCharIdx
				<< recvCharName
				<< sendMsg
				<< count;

	for(int i = 0; i < count; i++)
	{
		RefMsg(msg) << idx[i]
					<< ctid[i];
	}
}

void TeachTeacherReqReqMsg(CNetMsg::SP& msg, CPC* ch)
{
	msg->Init(MSG_TEACH);

	RefMsg(msg) << (unsigned char) MSG_TEACH_TEACHER_REQ
				<< (unsigned char) MSG_TEACH_TEACHER_REQ_REQ
				<< ch->m_index
				<< ch->GetName();
}

void TeachTeacherReqRejectMsg(CNetMsg::SP& msg, CPC* ch)
{
	msg->Init(MSG_TEACH);

	RefMsg(msg) << (unsigned char) MSG_TEACH_TEACHER_REQ
				<< (unsigned char) MSG_TEACH_TEACHER_REQ_REJECT
				<< ch->m_index
				<< ch->GetName();
}

void TeachTeacherReqAcceptMsg(CNetMsg::SP& msg, CPC* teacher, CPC* student)
{
	msg->Init(MSG_TEACH);

	RefMsg(msg) << (unsigned char) MSG_TEACH_TEACHER_REQ
				<< (unsigned char) MSG_TEACH_TEACHER_REQ_ACCEPT
				<< teacher->m_index
				<< teacher->GetName()
				<< student->m_index
				<< student->GetName();
	RefMsg(msg) << teacher->m_cntTeachingStudent
				<< teacher->m_cntCompleteStudent
				<< teacher->m_cntFailStudent;
}

void TeachEndMsg(CNetMsg::SP& msg, int teacher_index, const char* teacher_name, int studetn_index, const char* student_name, MSG_TEACH_END_TYPE type, int fame)
{
	msg->Init(MSG_TEACH);

	RefMsg(msg) << (unsigned char) MSG_TEACH_END
				<< teacher_index
				<< teacher_name
				<< studetn_index
				<< student_name
				<< (unsigned char) type;
#if defined( EVENT_TEACH ) || defined ( EVENT_CHILDERN_DAY )
	RefMsg(msg) << (char) 1;
#else
	RefMsg(msg) << (char) 0;
#endif
	RefMsg(msg) << fame;
}

void TeachInfoMsg(CNetMsg::SP& msg, CPC* ch)
{
	msg->Init(MSG_TEACH);

	RefMsg(msg) << (unsigned char) MSG_TEACH_INFO
				<< (char)((ch->m_bTeacher) ? 1 : 0)
				<< ch->m_teachType;

	if (ch->m_teachType == MSG_TEACH_TEACHER_TYPE)
	{
		RefMsg(msg) << ch->m_cntTeachingStudent
					<< ch->m_cntCompleteStudent
					<< ch->m_cntFailStudent;
		int i;
		for (i=0; i < TEACH_MAX_STUDENTS; i++)
		{
			if (ch->m_teachIdx[i] != -1)
			{
				RefMsg(msg) << ch->m_teachIdx[i]
							<< ch->m_teachLevel[i]
							<< ch->m_teachName[i]
							<< ch->m_teachJob[i];
				RefMsg(msg) << ch->m_teachJob2[i];
			}
			else
				RefMsg(msg) << -1;
		}
	}
	else if (ch->m_teachType == MSG_TEACH_STUDENT_TYPE)
	{
		RefMsg(msg) << ch->m_teachTime[0];
		if (ch->m_teachIdx[0] != -1)
		{
			RefMsg(msg) << ch->m_teachIdx[0]
						<< ch->m_teachLevel[0]
						<< ch->m_teachName[0]
						<< ch->m_teachJob[0];
			RefMsg(msg) << ch->m_teachJob2[0];
			//0627
			CPC* tpc = PCManager::instance()->getPlayerByCharIndex(ch->m_teachIdx[0]);
			if(tpc)
			{
				RefMsg(msg) << tpc->m_fame
							<< tpc->m_cntTeachingStudent
							<< tpc->m_cntCompleteStudent
							<< tpc->m_cntFailStudent;

				// 자신이 학생이면 idx[1]을 선생의 명성수치로, idx[2]를 선생의 양성중이 초보로, time[1]을 완료인원, time[2]을 실패인원으로 쓴다.
				ch->m_teachIdx[1] = tpc->m_fame;
				ch->m_teachIdx[2] = tpc->m_cntTeachingStudent;
				ch->m_teachTime[1] = tpc->m_cntCompleteStudent;
				ch->m_teachTime[2] = tpc->m_cntFailStudent;
			}
			else //접속해 있지 않은 스승의 정보를 어찌 알리오..
			{
				// 자신이 학생이면 idx[1]을 선생의 명성수치로, idx[2]를 선생의 양성중이 초보로, time[1]을 완료인원, time[2]을 실패인원으로 쓴다.
				RefMsg(msg) << ch->m_teachIdx[1]
							<< ch->m_teachIdx[2]
							<< ch->m_teachTime[1]
							<< ch->m_teachTime[2];
			}
		}
		else
			RefMsg(msg) << -1;
	}
}
void TeachTeacherGiveUPMsg(CNetMsg::SP& msg, CPC* teacher, CPC* student, int giveupCnt, int teacherFame)
{
	msg->Init(MSG_TEACH);

	RefMsg(msg) << (unsigned char) MSG_TEACH_TEACHER_REQ
				<< (unsigned char) MSG_TEACH_TEACHER_GIVEUP;
	if( teacher != NULL )
	{
		RefMsg(msg) << teacher->m_index
					<< teacher->GetName()
					<< student->m_index
					<< student->GetName()
					<< giveupCnt;
	}
	else
	{
		RefMsg(msg) << -1
					<< student->m_index
					<< student->GetName()
					<< giveupCnt;
	}

	if( giveupCnt == -1) // 후견인이다.
	{
		RefMsg(msg) << teacher->m_cntTeachingStudent
					<< teacher->m_cntCompleteStudent
					<< teacher->m_cntFailStudent;
	}
}

void TeachTeacherGiftMsg(CNetMsg::SP& msg, MSG_TEACH_RENEWER_GIFT_ERROR_TYPE type, CPC* ch, int count)
{
	msg->Init(MSG_TEACH);

	RefMsg(msg) << (unsigned char) MSG_TEACH_RENEWER_TEACH_GIFT
				<< (unsigned char) type
				<< ch->m_index
				<< count;
}
void TeachLoginMsg(CNetMsg::SP& msg, CPC* ch)
{
	msg->Init(MSG_TEACH);
	RefMsg(msg) << (unsigned char) MSG_TEACH_LOGIN
				<< ch->m_teachType
				<< ch->m_index
				<< ch->GetName();
}

void TeachStudentLevelUPMsg(CNetMsg::SP& msg, CPC* ch)
{
	msg->Init(MSG_TEACH);
	RefMsg(msg) << (unsigned char) MSG_TEACH_STUDENT_LEVELUP
				<< ch->m_index
				<< ch->m_level
				<< ch->GetName()
				<< ch->m_job
				<< ch->m_job2;
}

void TeachErrMsg(CNetMsg::SP& msg, MSG_TEACH_ERR_TYPE type)
{
	msg->Init(MSG_TEACH);

	RefMsg(msg) << (unsigned char) MSG_TEACH_ERR
				<< (unsigned char) type;
}

void ItemProcessNPCMsg(CNetMsg::SP& msg, int itemdbindex, int count, MSG_ITEM_PROCESS_NPC_ERROR_TYPE errcode)
{
	msg->Init(MSG_ITEM);
	RefMsg(msg) << (unsigned char)MSG_ITEM_PROCESS_NPC
				<< itemdbindex
				<< count
				<< errcode;
}

void ItemAddBoosterMsg(CNetMsg::SP& msg, MSG_ITEM_ADD_BOOSTER_ERROR_TYPE errcode)
{
	msg->Init(MSG_ITEM);
	RefMsg(msg) << (unsigned char)MSG_ITEM_ADD_BOOSTER
				<< errcode;
}

void ItemMixWarItemMsg(CNetMsg::SP& msg, MSG_ITEM_MIX_WARITEM_ERROR_TYPE errcode)
{
	msg->Init(MSG_ITEM);
	RefMsg(msg) << (unsigned char)MSG_ITEM_MIX_WARITEM
				<< errcode;
}

void CashItemMoonstoneStartRepMsg(CNetMsg::SP& msg, MSG_EX_CASHITEM_MOONSTONE_ERROR_TYPE errorcode, char grade)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASHITEM
				<< (unsigned char) MSG_EX_CASHITEM_MOONSTONE_START
				<< (unsigned char) errorcode
				<< grade;
}

void CashItemGiftRecvNoticeRepMsg(CNetMsg::SP& msg, char bGiftExist)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASHITEM
				<< (unsigned char) MSG_EX_CASHITEM_GIFT_NOTICE
				<< bGiftExist;
}

void CashItemGiftSendRepMsg(CNetMsg::SP& msg, MSG_EX_CASHITEM_ERROR_TYPE errCode)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASHITEM
				<< (unsigned char) MSG_EX_CASHITEM_GIFT_REP
				<< (unsigned char) errCode;
}

void CashItemGiftHistoryRepMsg(CNetMsg::SP& msg, bool bSend, MSG_EX_CASHITEM_ERROR_TYPE errCode )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASHITEM;

	if( bSend )
		RefMsg(msg) << (unsigned char) MSG_EX_CASHITEM_GIFT_SENDHISTORY_REP;
	else
		RefMsg(msg) << (unsigned char) MSG_EX_CASHITEM_GIFT_RECVHISTORY_REP;

	RefMsg(msg) << (unsigned char) errCode;
}

void CashItemGiftRecvListRepMsg(CNetMsg::SP& msg, unsigned char listflag, CNetMsg::SP& recvMsg)
{
	int idx, ctid, count, i;
	long sendDate;
	CLCString strMsg(MAX_GIFT_MESSAGE_LENGTH + 1);
	CLCString sendName( MAX_CHAR_NAME_LENGTH + 1);

	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASHITEM
				<< (unsigned char) MSG_EX_CASHITEM_GIFT_RECVLIST_REP;

	if( listflag & ( 1 << 2 ) )
	{
		listflag = listflag &~ (1 << 2);
		listflag |= (1 << 0);
		RefMsg(msg) << listflag;
		RefMsg(msg) << (int)0;
		return;
	}
	else
	{
		RefMsg(msg) << listflag;
		RefMsg(recvMsg) >> count;
		RefMsg(msg) << count;

		for(i = 0; i < count; i++)
		{
			RefMsg(recvMsg) >> idx
							>> ctid
							>> sendDate
							>> sendName
							>> strMsg;

			RefMsg(msg) << idx
						<< ctid
						<< sendDate
						<< sendName
						<< strMsg;
		}
	}
}

void WarpTeleportMsg(CNetMsg::SP& msg, int pos, CPC* ch)
{
	msg->Init(MSG_WARP);
	RefMsg(msg) << (unsigned char)MSG_WARP_TELEPORT
				<< pos
				<< GET_X(ch)
				<< GET_Z(ch)
				<< GET_H(ch)
				<< GET_R(ch)
				<< GET_YLAYER(ch);
}

void MsgrEventReqMsg(CNetMsg::SP& msg, MSG_MSGR_TYPE msgType, int seq, int server, int subno, int zone, int charindex, int cmd, int drop)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char) msgType
				<< charindex
				<< cmd
				<< drop;
}

void MsgrEventRepMsg(CNetMsg::SP& msg, MSG_MSGR_TYPE msgType, int seq, int server, int subno, int zone, int charindex, int cmd, int drop)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char) msgType
				<< charindex
				<< gserver->m_serverno
				<< gserver->m_subno
				<< cmd
				<< drop;
}

void MsgrEventReqMsg(CNetMsg::SP& msg, int server, int subno, int zone, int thisServer, int thisSubno, int charIndex, const char* eventcmd, int flag)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << (int)-1
				<< server
				<< subno
				<< zone
				<< (unsigned char) MSG_MSGR_EVENT_REQ
				<< thisServer
				<< thisSubno
				<< charIndex
				<< eventcmd
				<< flag;
}

void MsgrMessengerChatMsg( CNetMsg::SP& msg, MSG_MSGR_TYPE subtype,
						   int makeCharIndex, int chatIndex, int chatColor, int charIndex, CLCString chat )
{
	msg->Init(MSG_MSGR_REQ);

	RefMsg(msg) << -1
				<< gserver->m_serverno
				<< -1
				<< -1
				<< (unsigned char) subtype
				<< makeCharIndex
				<< chatIndex
				<< chatColor
				<< charIndex
				<< chat;
}

void MsgrMessengerChatMsg( CNetMsg::SP& msg, MSG_MSGR_TYPE subtype,
						   int makeCharIndex, int chatIndex, int chatColor, const char* charName, CLCString chat )
{
	msg->Init(MSG_MSGR_REQ);

	RefMsg(msg) << -1
				<< gserver->m_serverno
				<< -1
				<< -1
				<< (unsigned char) subtype
				<< makeCharIndex
				<< chatIndex
				<< chatColor
				<< charName
				<< chat;
}

void WarpPromptMsg(CNetMsg::SP& msg, int zoneindex, int pos)
{
	msg->Init(MSG_WARP);
	RefMsg(msg) << (unsigned char)MSG_WARP_PROMPT
				<< zoneindex
				<< pos;
}

void MsgrChatLordMsg(CNetMsg::SP& msg, int index, const char* name, const char* chatmsg)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << -1
				<< gserver->m_serverno
				<< gserver->m_subno
				<< -1
				<< (unsigned char)MSG_MSGR_CHAT_LORD
				<< index
				<< name
				<< chatmsg;
}

void GuildWarGetTimeMsg(CNetMsg::SP& msg, MSG_GUILD_WAR_ERROR_TYPE errcode, CWarCastle* castle)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_WAR_GET_TIME
				<< errcode;

	if (castle)
	{
		struct tm tmNext;
		castle->GetNextWarTime(&tmNext, true);

		RefMsg(msg) << castle->GetOwnerGuildIndex()
					<< castle->GetOwnerGuildName()
					<< (char)tmNext.tm_mon
					<< (char)tmNext.tm_mday
					<< (char)tmNext.tm_hour
					<< (char)tmNext.tm_min;
	}
	else
	{
		RefMsg(msg) << 0
					<< ""
					<< 0
					<< 0
					<< 0
					<< 0;
	}
}

void HelperWarNoticeTimeMsg(CNetMsg::SP& msg, int zoneindex, char month, char day, char hour, char min)
{
	msg->Init(MSG_HELPER_REQ);
	RefMsg(msg) << -1
				<< gserver->m_serverno
				<< -1
				<< -1
				<< (unsigned char)MSG_HELPER_WAR_NOTICE_TIME
				<< zoneindex
				<< month
				<< day
				<< hour
				<< min;
}

void GuildWarNoticeTimeMsg(CNetMsg::SP& msg, int zoneindex, char month, char day, char hour, char min)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_WAR_NOTICE_TIME
				<< zoneindex
				<< month
				<< day
				<< hour
				<< min;
}

void GuildWarNoticeTimeRemainMsg(CNetMsg::SP& msg, int zoneindex, char min)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_WAR_NOTICE_TIME_REMAIN
				<< zoneindex
				<< min;
}

void HelperWarNoticeRemainMsg(CNetMsg::SP& msg, int zoneindex, char remain)
{
	msg->Init(MSG_HELPER_REQ);
	RefMsg(msg) << -1
				<< gserver->m_serverno
				<< -1
				<< -1
				<< (unsigned char)MSG_HELPER_WAR_NOTICE_TIME_REMAIN
				<< zoneindex
				<< remain;
}

void HelperWarNoticeStartMsg(CNetMsg::SP& msg, int zoneindex, int remainSec)
{
	msg->Init(MSG_HELPER_REQ);
	RefMsg(msg) << -1
				<< gserver->m_serverno
				<< -1
				<< -1
				<< (unsigned char)MSG_HELPER_WAR_NOTICE_START
				<< zoneindex
				<< remainSec;
}

void GuildWarNoticeStartMsg(CNetMsg::SP& msg, int zoneindex, int remainSec)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_WAR_START
				<< zoneindex
				<< remainSec;
}

void GuildWarJoinAttackGuildMsg(CNetMsg::SP& msg, MSG_GUILD_WAR_ERROR_TYPE errcode, CWarCastle* castle)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_WAR_JOIN_ATTACK_GUILD
				<< errcode;

	if (castle)
	{
		struct tm tmNext;
		castle->GetNextWarTime(&tmNext, true);

		RefMsg(msg) << (char)tmNext.tm_mon
					<< (char)tmNext.tm_mday
					<< (char)tmNext.tm_hour
					<< (char)tmNext.tm_min;
	}
	else
	{
		RefMsg(msg) << 0
					<< 0
					<< 0
					<< 0;
	}
}

void GuildWarJoinDefenseGuildMsg(CNetMsg::SP& msg, MSG_GUILD_WAR_ERROR_TYPE errcode, CWarCastle* castle)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_WAR_JOIN_DEFENSE_GUILD
				<< errcode;

	if (castle)
	{
		struct tm tmNext;
		castle->GetNextWarTime(&tmNext, true);

		RefMsg(msg) << (char)tmNext.tm_mon
					<< (char)tmNext.tm_mday
					<< (char)tmNext.tm_hour
					<< (char)tmNext.tm_min;
	}
	else
	{
		RefMsg(msg) << 0
					<< 0
					<< 0
					<< 0;
	}
}

void GuildWarJoinAttackCharMsg(CNetMsg::SP& msg, MSG_GUILD_WAR_ERROR_TYPE errcode, CWarCastle* castle)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_WAR_JOIN_ATTACK_CHAR
				<< errcode;

	if (castle)
	{
		struct tm tmNext;
		castle->GetNextWarTime(&tmNext, true);

		RefMsg(msg) << (char)tmNext.tm_mon
					<< (char)tmNext.tm_mday
					<< (char)tmNext.tm_hour
					<< (char)tmNext.tm_min;
	}
	else
	{
		RefMsg(msg) << 0
					<< 0
					<< 0
					<< 0;
	}
}

void GuildWarJoinDefenseCharMsg(CNetMsg::SP& msg, MSG_GUILD_WAR_ERROR_TYPE errcode, CWarCastle* castle)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_WAR_JOIN_DEFENSE_CHAR
				<< errcode;

	if (castle)
	{
		struct tm tmNext;
		castle->GetNextWarTime(&tmNext, true);

		RefMsg(msg) << (char)tmNext.tm_mon
					<< (char)tmNext.tm_mday
					<< (char)tmNext.tm_hour
					<< (char)tmNext.tm_min;
	}
	else
	{
		RefMsg(msg) << 0
					<< 0
					<< 0
					<< 0;
	}
}

void HelperWarJoinAttackGuildMsg(CNetMsg::SP& msg, int zoneindex, int guildindex)
{
	msg->Init(MSG_HELPER_REQ);
	RefMsg(msg) << -1
				<< gserver->m_serverno
				<< -1
				<< -1
				<< (unsigned char)MSG_HELPER_WAR_JOIN_ATTACK_GUILD
				<< zoneindex
				<< guildindex;
}

void HelperWarJoinDefenseGuildMsg(CNetMsg::SP& msg, int zoneindex, int guildindex)
{
	msg->Init(MSG_HELPER_REQ);
	RefMsg(msg) << -1
				<< gserver->m_serverno
				<< -1
				<< -1
				<< (unsigned char)MSG_HELPER_WAR_JOIN_DEFENSE_GUILD
				<< zoneindex
				<< guildindex;
}

void HelperWarNoticeStartAttackCastleMsg(CNetMsg::SP& msg, int zoneindex, int remainSec, CGuild* g1, CGuild* g2, CGuild* g3)
{
	msg->Init(MSG_HELPER_REQ);
	RefMsg(msg) << -1
				<< gserver->m_serverno
				<< -1
				<< -1
				<< (unsigned char)MSG_HELPER_WAR_NOTICE_START_ATTACK_CASTLE
				<< zoneindex
				<< remainSec;

	if (g1)		RefMsg(msg) << g1->index() << g1->name();
	else		RefMsg(msg) << 0 << "";
	if (g2)		RefMsg(msg) << g2->index() << g2->name();
	else		RefMsg(msg) << 0 << "";
	if (g3)		RefMsg(msg) << g3->index() << g3->name();
	else		RefMsg(msg) << 0 << "";
}

void GuildWarPointMsg(CNetMsg::SP& msg, CPC* pc, CWarCastle* castle)
{
	int remainsec = castle->GetRemainWarTime();
	int guildindex[3] = {-1, -1, -1};
	char guildname[3][MAX_GUILD_NAME_LENGTH + 1] = {"", "", ""};
	int guildpoint[3] = {0, 0, 0};
	castle->GetTop3AttackGuild(guildindex, guildname, guildpoint);
	int myguildpoint = -1;
	int defensepoint = -1;
	if (pc->m_guildInfo && pc->m_guildInfo->guild() && castle->GetState() == WCSF_WAR_FIELD)
	{
		myguildpoint = castle->GetGuildPoint(pc->m_guildInfo->guild()->index());
		defensepoint = castle->GetGuildPoint(castle->GetOwnerGuildIndex());
	}
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_WAR_POINT
				<< remainsec;
	int i;
	for (i = 0; i < 3; i++)
	{
		RefMsg(msg) << guildindex[i]
					<< guildname[i]
					<< guildpoint[i];
	}
	RefMsg(msg) << myguildpoint
				<< defensepoint;
}

void GuildWarCastleStateMsg(CNetMsg::SP& msg, int zoneindex, CPC* pc, CWarCastle* castle)
{
	int state = castle->GetState();
	int remainsec = castle->GetRemainWarTime();

	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_WAR_CASTLE_STATE
				<< zoneindex
				<< state
				<< castle->GetGateState()
				<< remainsec;

	if (zoneindex != ZONE_DRATAN)
	{
		int guildindex[3] = {-1, -1, -1};
		char guildname[3][MAX_GUILD_NAME_LENGTH + 1] = {"", "", ""};
		int guildpoint[3] = {0, 0, 0};

		castle->GetTop3AttackGuild(guildindex, guildname, guildpoint);

		int myguildpoint = -1;
		int defensepoint = -1;
		if (pc->m_guildInfo && pc->m_guildInfo->guild() && castle->GetState() == WCSF_WAR_FIELD)
		{
			myguildpoint = castle->GetGuildPoint(pc->m_guildInfo->guild()->index());
			defensepoint = castle->GetGuildPoint(castle->GetOwnerGuildIndex());
		}

		int i;
		for (i = 0; i < 3; i++)
		{
			RefMsg(msg) << guildindex[i]
						<< guildname[i]
						<< guildpoint[i];
		}
		RefMsg(msg) << myguildpoint
					<< defensepoint;
	}
}

void HelperWarNoticeRemainFieldTimeMsg(CNetMsg::SP& msg, int zoneindex, int remainSec)
{
	msg->Init(MSG_HELPER_REQ);
	RefMsg(msg) << -1
				<< gserver->m_serverno
				<< -1
				<< -1
				<< (unsigned char)MSG_HELPER_WAR_NOTICE_REMAIN_FIELD_TIME
				<< zoneindex
				<< remainSec;
}

void GuildWarNoticeStartCastleMsg(CNetMsg::SP& msg, int zoneindex, int remainSec, int guildindex1, const char* guildname1, int guildindex2, const char* guildname2, int guildindex3, const char* guildname3)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_WAR_NOTICE_START_CASTLE
				<< zoneindex
				<< remainSec
				<< guildindex1
				<< guildname1
				<< guildindex2
				<< guildname2
				<< guildindex3
				<< guildname3;
}

void GuildWarNoticeRemainFieldTimeMsg(CNetMsg::SP& msg, int zoneindex, int remainSec)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_WAR_NOTICE_REMAIN_FIELD_TIME
				<< zoneindex
				<< remainSec;
}

void GuildWarGateStateMsg(CNetMsg::SP& msg, int oldstate, int newstate)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_WAR_GATE_STATE
				<< oldstate
				<< newstate;
}

void HelperWarNoticeEndMsg(CNetMsg::SP& msg, int zoneindex, char bWinDefense, int ownerindex, const char* ownername, int charindex, const char* charname, int nextMonth, int nextDay, int nextHour, int wDay)
{
	msg->Init(MSG_HELPER_REQ);
	RefMsg(msg) << -1
				<< gserver->m_serverno
				<< -1
				<< -1
				<< (unsigned char)MSG_HELPER_WAR_NOTICE_END
				<< zoneindex;
	if (bWinDefense)
		RefMsg(msg) << (char)1;
	else
		RefMsg(msg) << (char)0;
	RefMsg(msg) << ownerindex
				<< ownername
				<< charindex
				<< charname
				<< nextMonth
				<< nextDay
				<< nextHour
				<< wDay;
}

void GuildWarEndMsg(CNetMsg::SP& msg, int zoneindex, char bWinDefense, int ownerindex, const char* ownername, int charindex, const char* charname, int nextMonth, int nextDay, int nextHour, int wDay)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_WAR_END
				<< zoneindex
				<< bWinDefense
				<< ownerindex
				<< ownername
				<< charindex
				<< charname
				<< nextMonth
				<< nextDay
				<< nextHour
				<< wDay;
}

void GuildWarSetTimeRepMsg(CNetMsg::SP& msg, MSG_GUILD_WAR_ERROR_TYPE errcode, char month, char day, char hour, char min)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_WAR_SET_TIME_REP
				<< errcode
				<< month
				<< day
				<< hour
				<< min;
}

void GuildWarInvalidCommandMsg(CNetMsg::SP& msg, MSG_GUILD_TYPE reqtype)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_WAR_INVALID_COMMAND
				<< (unsigned char)reqtype;
}

void GuildStashHistoryRepMsg(CNetMsg::SP& msg, MSG_GUILD_STASH_ERROR_TYPE errcode, int month[7], int day[7], LONGLONG money[7])
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_STASH_HISTORY_REP
				<< errcode;
	int i;
	for (i = 0; i < 7; i++)
	{
		RefMsg(msg) << month[i]
					<< day[i]
					<< money[i];
	}
}

void GuildStashViewRepMsg(CNetMsg::SP& msg, MSG_GUILD_STASH_ERROR_TYPE errcode, LONGLONG money)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_STASH_VIEW_REP
				<< errcode
				<< money;
}

void GuildStashTakeRepMsg(CNetMsg::SP& msg, MSG_GUILD_STASH_ERROR_TYPE errcode)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_STASH_TAKE_REP
				<< errcode;
}

void HelperGuildStashHistoryReqMsg(CNetMsg::SP& msg, int guildindex, int charindex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_STASH_HISTORY_REQ
				<< guildindex
				<< charindex;
}

void HelperGuildStashViewReqMsg(CNetMsg::SP& msg, int guildindex, int charindex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_STASH_VIEW_REQ
				<< guildindex
				<< charindex;
}

void HelperGuildStashTakeReqMsg(CNetMsg::SP& msg, int guildindex, int charindex, LONGLONG money)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_STASH_TAKE_REQ
				<< guildindex
				<< charindex
				<< money;
}

void HelperGuildStashRollbackMsg(CNetMsg::SP& msg, int guildindex, LONGLONG money)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_STASH_ROLLBACK
				<< guildindex
				<< money;
}

void HelperGuildStashSaveTaxReqMsg(CNetMsg::SP& msg, int guildIndex, int zoneindex, LONGLONG taxItem, LONGLONG taxProduct)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_STASH_SAVE_TAX_REQ
				<< guildIndex
				<< zoneindex
				<< taxItem
				<< taxProduct;
}

void EventTreasureBoxMsg(CNetMsg::SP& msg, MSG_EVENT_TREASUREBOX_TYPE type, int arg)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char) MSG_EVENT_TREASUREBOX
				<< (unsigned char) type
				<< arg;

	if(type ==MSG_EVENT_TREASUREBOX_TRY_REP)
	{
		if(arg == (int)MSG_EVENT_TREASUREBOX_10LEVEL)
		{
			RefMsg(msg) << (unsigned char) MSG_EVENT_CHANGE_13LEVEL_WEAPON
						<< (int) 1
						<< (unsigned char) MSG_EVENT_CHANGE_NAS
						<< (int) 30000
						<< (unsigned char) MSG_EVENT_CHANGE_CANDY
						<< (int) 5;
		}
		else if(arg == (int)MSG_EVENT_TREASUREBOX_14LEVEL)
		{
			RefMsg(msg) << (unsigned char) MSG_EVENT_CHANGE_ATTACK_POTION
						<< (int) 10
						<< (unsigned char) MSG_EVENT_CHANGE_NAS
						<< (int) 50000
						<< (unsigned char) MSG_EVENT_CHANGE_CANDY
						<< (int) 10;
		}
		else if(arg == (int)MSG_EVENT_TREASUREBOX_18LEVEL)
		{
			RefMsg(msg) << (unsigned char) MSG_EVENT_CHANGE_21LEVEL_WEAPON
						<< (int) 1
						<< (unsigned char) MSG_EVENT_CHANGE_NAS
						<< (int) 80000
						<< (unsigned char) MSG_EVENT_CHANGE_CANDY
						<< (int) 15;
		}
		else if(arg == (int)MSG_EVENT_TREASUREBOX_22LEVEL)
		{
			RefMsg(msg) << (unsigned char) MSG_EVENT_CHANGE_DEFENSE_POTION
						<< (int) 15
						<< (unsigned char) MSG_EVENT_CHANGE_NAS
						<< (int) 100000
						<< (unsigned char) MSG_EVENT_CHANGE_CANDY
						<< (int) 20;
		}
		else if(arg == (int)MSG_EVENT_TREASUREBOX_26LEVEL)
		{
			RefMsg(msg) << (unsigned char) MSG_EVENT_CHANGE_29LEVEL_WEAPON
						<< (int) 1
						<< (unsigned char) MSG_EVENT_CHANGE_NAS
						<< (int) 150000
						<< (unsigned char) MSG_EVENT_CHANGE_CANDY
						<< (int) 30;
		}
		else if(arg == (int)MSG_EVENT_TREASUREBOX_30LEVEL)
		{
#ifdef LC_TLD
			RefMsg(msg) << (unsigned char) MSG_EVENT_CHANGE_BLUE_HORSE
						<< (int) 1
#else
			RefMsg(msg) << (unsigned char) MSG_EVENT_CHANGE_ATTACK_POTION
						<< (int) 20
#endif // LC_TLD
						<< (unsigned char) MSG_EVENT_CHANGE_NAS
						<< (int) 200000
#ifdef LC_TLD
						<< (unsigned char) MSG_EVENT_CHANGE_PINK_DRAGON
						<< (int) 1;
#else
						<< (unsigned char) MSG_EVENT_CHANGE_CANDY
						<< (int) 40;
#endif // LC_TLD
		}
	}
}

//0627
void EventSuperStoneMsg(CNetMsg::SP& msg, MSG_EVENT_SUPERSTONE_TYPE type)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char) MSG_EVENT_SUPERSTONE
				<< (unsigned char) type;
}

void SysEnableSuperStoneMsg(CNetMsg::SP& msg, int fame)
{
	msg->Init(MSG_SYS);
	RefMsg(msg) << (unsigned char)MSG_SYS_CAN_RECEIVE_SUPERSTONE
				<< fame;
}

void NoviceNotifyMsg(CNetMsg::SP& msg, const char* name)
{
	msg->Init(MSG_SYS);
	RefMsg(msg) << (unsigned char)MSG_SYS_APPEAR_NOVICE
				<< name;
}

//0704
void NoticeInfoMsg(CNetMsg::SP& msg, int notice)
{
	msg->Init(MSG_SYS);
	RefMsg(msg) << (unsigned char)MSG_SYS_NOTICE
				<< notice;
}

void HelperPetCreateReqMsg(CNetMsg::SP& msg, int owner, char typeGrade)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PET_CREATE_REQ
				<< owner
				<< typeGrade;
}

// TODO : DELETE
void HelperPetDeleteReqMsg(CNetMsg::SP& msg, int index, int owner)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PET_DELETE_REQ
				<< index
				<< owner;
}

void ExPetStatusMsg(CNetMsg::SP& msg, CPet* pet)
{
	int nRemainGetRebirth = pet->GetRemainRebirthTime();
	float x = 0.0f;
	float z = 0.0f;
	float h = 0.0f;
	float r = 0.0f;
	char y = 0;
	unsigned short mapAttr;
	mapAttr = MATT_UNWALKABLE;

	if (IS_IN_CELL(pet))
	{
		x = GET_X(pet);
		z = GET_Z(pet);
		h = GET_H(pet);
		r = GET_R(pet);
		y = GET_YLAYER(pet);
		mapAttr = pet->GetMapAttr();
	}

	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_PET_STATUS
				<< pet->m_index
				<< pet->GetPetTypeGrade()
				<< pet->m_level
				<< pet->m_exp
				<< pet->GetNeedExp()
				<< pet->m_hp
				<< pet->m_maxHP
				<< pet->GetAbilityPoint()
				<< pet->GetHungryPoint()
				<< (int)PET_MAX_HUNGRY
				<< pet->GetSympathyPoint()
				<< (int)PET_MAX_SYMPATHY
				<< x
				<< z
				<< h
				<< r
				<< y
				<< mapAttr
				<< nRemainGetRebirth
				<< pet->m_name;
}

void ExPetMountMsg(CNetMsg::SP& msg, int charindex, char pettype)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_PET_MOUNT
				<< charindex
				<< pettype;
}

void ExPetLearnMsg(CNetMsg::SP& msg, int skillindex, char level, MSG_EX_PET_LEARN_ERROR_TYPE errcode)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_PET_LEARN
				<< skillindex
				<< level
				<< errcode;
}

void ExPetSkillListMsg(CNetMsg::SP& msg, CPet* pet)
{
	if (pet == NULL)
	{
		msg->Init();
		return ;
	}
	CSkillList& list = *(pet->GetSkillList());

	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_PET_SKILLLIST
				<< pet->m_index
				<< list.count();

	void* p = list.GetHeadPosition();
	while (p)
	{
		CSkill* skill = list.GetNext(p);
		if (skill)
		{
			RefMsg(msg) << skill->m_proto->m_index
						<< skill->m_level;
		}
	}
}

void ExPetResetSkillMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_PET_RESET_SKILL;
}

void ExPetSellInfoMsg(CNetMsg::SP& msg, CPet* pet)
{
	if (pet == NULL || pet->GetOwner() == NULL)
	{
		msg->Init();
		return ;
	}

	int nRemainGetRebirth = pet->GetRemainRebirthTime();

	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_PET_SELL_INFO
				<< pet->GetOwner()->m_index
				<< pet->m_index
				<< pet->GetPetTypeGrade()
				<< pet->m_level
				<< pet->m_exp
				<< pet->GetNeedExp()
				<< pet->m_hp
				<< pet->m_maxHP
				<< pet->GetAbilityPoint()
				<< pet->GetHungryPoint()
				<< (int)PET_MAX_HUNGRY
				<< pet->GetSympathyPoint()
				<< (int)PET_MAX_SYMPATHY
				<< nRemainGetRebirth
				<< pet->m_name;
}

void ExPetChangeMountMsg(CNetMsg::SP& msg, MSG_EX_PET_CHANGE_MOUNT_ERROR_TYPE errcode)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_PET_CHANGE_MOUNT
				<< errcode;
}

void ExPetCommandMsg(CNetMsg::SP& msg, int petindex, int commandindex, char targettype, int targetindex)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_PET_COMMAND
				<< petindex
				<< commandindex
				<< targettype
				<< targetindex;
}

void ExPetMixItemMsg(CNetMsg::SP& msg, MSG_EX_PET_MIX_ITEM_ERROR_TYPE errcode)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_PET_MIX_ITEM
				<< errcode;
}

void ExPetLevelupMsg(CNetMsg::SP& msg, CPet* pet)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_PET_LEVELUP
				<< pet->m_index
				<< pet->GetPetTypeGrade()
				<< pet->m_level
				<< pet->GetPetColor();
}

void ExAPetStatusMsg(CNetMsg::SP& msg, CAPet* pet)
{
	float x = 0.0f;
	float z = 0.0f;
	float h = 0.0f;
	float r = 0.0f;
	char y = 0;
	unsigned short mapAttr;
	mapAttr = MATT_UNWALKABLE;

	if (IS_IN_CELL(pet))
	{
		x = GET_X(pet);
		z = GET_Z(pet);
		h = GET_H(pet);
		r = GET_R(pet);
		y = GET_YLAYER(pet);
		mapAttr = pet->GetMapAttr();
	}

	int	nEqAttack = 0;
	if( pet->m_wearing[APET_WEAR_WEAPON] )
	{
		if( pet->m_wearing[APET_WEAR_WEAPON]->IsRangeWeapon() )
		{
			nEqAttack = pet->m_eqRange + pet->m_opRange;
		}
		else
		{
			nEqAttack = pet->m_eqMelee + pet->m_opMelee;
		}
	}

	// 계산이 필요한 표시부분
	int nAttack = pet->m_nAttack + nEqAttack;	// 공격력
	nAttack += pet->m_avPassiveAddition.attack + (nAttack * pet->m_avPassiveRate.attack / SKILL_RATE_UNIT);
	// 마법 공격력
	int nMAttack = pet->m_nMagicAttack + pet->m_eqMagic + pet->m_opMagic;
	nMAttack += pet->m_avPassiveAddition.magic + (nMAttack * pet->m_avPassiveRate.magic / SKILL_RATE_UNIT);
	// 방어
	int nDeffence = pet->m_nDefence + pet->m_eqDefense + pet->m_opDMelee;
	nDeffence += pet->m_avPassiveAddition.defense + (nDeffence * pet->m_avPassiveRate.defense / SKILL_RATE_UNIT);
	// 마법 방어
	int nMDeffence = pet->m_nMagicDefence + pet->m_eqResist + pet->m_opResist;
	nMDeffence += pet->m_avPassiveAddition.resist + (nMDeffence * pet->m_avPassiveRate.resist / SKILL_RATE_UNIT);
	// 물리 명중
	int nHitPoint = (int)( pet->m_nHitPoint + pet->m_dex + pet->m_opMeleeHitRate );
	nHitPoint += pet->m_avPassiveAddition.hitrate + (nHitPoint * pet->m_avPassiveRate.hitrate / SKILL_RATE_UNIT);

	// 물리 회피
	int nAvoid = (int)( pet->m_nAvoid + ( pet->m_dex * 0.25 ) + pet->m_opMeleeAvoid );
	nAvoid += pet->m_avPassiveAddition.avoid + (nAvoid * pet->m_avPassiveRate.avoid / SKILL_RATE_UNIT);

	if(pet->m_hp > (pet->m_maxHP + pet->GetOwner()->m_opJewelElementPetHPUP))
	{
		pet->m_hp = pet->m_maxHP + pet->GetOwner()->m_opJewelElementPetHPUP;
	}

	UpdateClient::attackPetStatus* packet = reinterpret_cast<UpdateClient::attackPetStatus*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_CHAR_STATUS_APET;

	packet->charIndex = pet->m_index;
	memcpy(packet->charName, pet->m_name, sizeof(packet->charName));
	packet->charName[MAX_CHAR_NAME_LENGTH] = '\0';
	packet->level = pet->m_level;
	packet->protoType = pet->m_pProto->m_Type;
	packet->isMount = pet->m_bMount;
	packet->transSate = pet->m_cTransSate;

	packet->coolTime = pet->GetCooltime();
	packet->accExp = pet->GetAccExp();

	packet->exp = pet->m_exp;
	packet->needExp = pet->GetNeedExp();
	packet->hp = pet->m_hp;
	packet->maxHp = pet->m_maxHP + pet->GetOwner()->m_opJewelElementPetHPUP;
	packet->mp = pet->m_mp;
	packet->maxMp = pet->m_maxMP;
	packet->faith = pet->GetFaith();
	packet->maxFaith = pet->m_pProto->m_nMaxFaith;
	packet->stamina = pet->GetStamina();
	packet->maxStamina = pet->m_pProto->m_nMaxStm;
	packet->str = pet->m_str + pet->m_nPlusStr + pet->m_opStr;
	packet->addStr = pet->m_nPlusStr + pet->m_opStr;
	packet->con = pet->m_con + pet->m_nPlusCon + pet->m_opCon;
	packet->addCon = pet->m_nPlusCon + pet->m_opCon;
	packet->dex = pet->m_dex + pet->m_nPlusDex + pet->m_opDex;
	packet->addDex = pet->m_nPlusDex + pet->m_opDex;
	packet->intl = pet->m_int + pet->m_nPlusInt + pet->m_opInt;
	packet->addIntl = pet->m_nPlusInt + pet->m_opInt;
	packet->remainStat = pet->m_nRemainStat;
	packet->attack = nAttack + pet->m_nJewelAttack;
	packet->magicAttack = nMAttack + pet->m_nJewelMagicAttack;
	packet->defence = nDeffence;
	packet->maxDefence = nMDeffence;
	packet->hitPoint = nHitPoint;
	packet->avoid = nAvoid;
	packet->x = x;
	packet->z = z;
	packet->h = h;
	packet->r = r;
	packet->yLayer = y;
	packet->mapAttr = mapAttr;
	msg->setSize(sizeof(UpdateClient::attackPetStatus));
}

void ExAPetFuntionMsg( CNetMsg::SP& msg, MSG_EX_ATTACK_PET_ERRORTYPE subtype, CAPet* apet, unsigned char error )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_ATTACK_PET
				<< (unsigned char)subtype;

	switch( subtype )
	{
	case MSG_SUB_SKILLLIST :
		{
			if( apet )
			{
				RefMsg(msg) << apet->m_skillList.count() + apet->m_passiveSkillList.count() ;

				void* pos = apet->m_skillList.GetHeadPosition();
				while ( pos )
				{
					CSkill* pSkill = apet->m_skillList.GetNext(pos);
					RefMsg(msg) << pSkill->m_proto->m_index
								<< pSkill->m_level;
				}

				pos = apet->m_passiveSkillList.GetHeadPosition();
				while ( pos )
				{
					CSkill* pSkill = apet->m_passiveSkillList.GetNext(pos);
					RefMsg(msg) << pSkill->m_proto->m_index
								<< pSkill->m_level;
				}
			}
		}
		break;
	case MSG_SUB_DELETE_EQUIP:
		{
			if( apet )
			{
				RefMsg(msg) << apet->m_name;
			}
			return;
		}
		break;

	case MSG_SUB_MOUNT_REP:
		{
			RefMsg(msg) << error << (int) apet->m_bMount ;
			RefMsg(msg) << apet->GetOwner()->m_index;
			RefMsg(msg) << apet->m_index;
		}
		break;

	case MSG_SUB_SKILLLEAN:
	case MSG_SUB_REBIRTH:
	case MSG_SUB_LEVELUP:
	case MSG_SUB_EVOLUTION:
	default:
		break;
	}

	RefMsg(msg) << error;
}

void ItemAPetWearMsg( CNetMsg::SP& msg, char wear_pos, CItem* item1, CItem* item2 )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_ATTACK_PET
				<< (unsigned char)MSG_SUB_ITEM_WEAR
				<< (char)wear_pos;

	if (item1)
	{
		RefMsg(msg) << (int)item1->getDBIndex();
	}
	else
	{
		RefMsg(msg) << (int)-1;
	}

	if (item2)
	{
		RefMsg(msg) << (int)item2->getDBIndex();
	}
	else
	{
		RefMsg(msg) << (int)-1;
	}
}

void ItemAPetWearingMsg( CNetMsg::SP& msg, CAPet* apet )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_ATTACK_PET
				<< (unsigned char)MSG_SUB_ITEM_WEARING
				<< apet->GetOwner()->m_index
				<< apet->m_index;

	for( int i=0; i<APET_WEARPOINT; i++)
	{
		if (apet->m_wearing[i])
		{
			RefMsg(msg) << (int)apet->m_wearing[i]->getDBIndex();
			RefMsg(msg) << apet->m_wearing[i]->getPlus();
		}
		else
		{
			RefMsg(msg) << (int)-1;
			RefMsg(msg) << (int)0;
		}
	}
}

void AddItemExAPetMsg( CNetMsg::SP& msg, CItem* addItem )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_ATTACK_PET
				<< (unsigned char) MSG_SUB_ADDITEM_MSG
				<< addItem->getWearPos()
				<< addItem->getDBIndex()
				<< addItem->getPlus()
				<< addItem->getFlag()
				<< addItem->getUsed()
				<< addItem->getUsed_2();

	ItemPublicMsg(msg, addItem);
}

void ExApetSellInfo( CNetMsg::SP& msg, CAPet* apet )
{
	UpdateClient::attackPetSmallStatus* packet = reinterpret_cast<UpdateClient::attackPetSmallStatus*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_APET_SMALL_STATUS;

	packet->charIndex = apet->m_index;
	memcpy(packet->charName, apet->m_name, sizeof(packet->charName));
	packet->charName[MAX_CHAR_NAME_LENGTH] = '\0';
	packet->level = apet->m_level;
	packet->str = apet->m_str + apet->m_nPlusStr + apet->m_opStr;
	packet->con = apet->m_con + apet->m_nPlusCon + apet->m_opCon;
	packet->dex = apet->m_dex + apet->m_nPlusDex + apet->m_opDex;
	packet->intl = apet->m_int + apet->m_nPlusInt + apet->m_opInt;

	packet->accExp = apet->GetAccExp();
	packet->coolTime = apet->GetCooltime();

	msg->setSize(sizeof(UpdateClient::attackPetSmallStatus));
}

void APetUseMsg( CNetMsg::SP& msg , unsigned char errorMsg )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_ATTACK_PET
				<< (unsigned char)MSG_SUB_EXPUSE
				<< errorMsg;
}

void APetAIListMsg(CNetMsg::SP& msg, CAPet* apet)
{
	if (!apet) return;

	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_ATTACK_PET
				<< (unsigned char)MSG_SUB_AI_LIST_NTF
				<< apet->m_nAISlot
				<< apet->m_nAICount;

	for (int i = 0; i < apet->m_nAISlot; i++)
	{
		if (apet->IsEmptyAILine(i) == false)
		{
			RefMsg(msg) << i
						<< apet->m_tAIData[i].nItemNum
						<< apet->m_tAIData[i].nSkillIdx
						<< apet->m_tAIData[i].nSkillType
						<< apet->m_tAIData[i].cUse;
		}
	}
}

void APetAIOnOffMsg(CNetMsg::SP& msg, CAPet* apet, unsigned char ucError)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_ATTACK_PET
				<< (unsigned char)MSG_SUB_AI_ONOFF_REP;

	if( apet )
		RefMsg(msg) << apet->m_cAI;
	else
		RefMsg(msg) << 0;

	RefMsg(msg) << ucError;
}

#ifdef RANKER_NOTICE
void ConnRankerReqMsg(CNetMsg::SP& msg, int userindex, int charindex)
{
	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char)MSG_CONN_RANKER_REQ
				<< userindex
				<< charindex;
}
#endif// RANKER_NOTICE

void ExCastleMapRecentMsg(CNetMsg::SP& msg, CWarCastle* castle, CPC* pc)
{
	msg->Init(MSG_UNKNOWN);
	int zoneindex = castle->GetZoneIndex();
	CZone* pZone = gserver->FindZone(zoneindex);
	if (pZone == NULL)
		return ;

	float lordx = -1.0f;
	float lordz = -1.0f;
	int lordhp = 0;
	int lordmaxhp = 0;

	int towercount = 0;

	// 권좌
	CNPC* lord = castle->GetLordSymbol();
	if (lord)
	{
		lordx = GET_X(lord);
		lordz = GET_Z(lord);
		lordhp = lord->m_hp;
		lordmaxhp = lord->m_maxHP;
	}

	// 수호탑
	towercount = castle->GetGateNPCCount();

	// 리젠 위치
	int joinflag = pc->GetJoinFlag(castle->GetZoneIndex());
	int pos = castle->GetRegenPoint(joinflag, pc);
	float regenx = (pZone->m_zonePos[pos][1] + pZone->m_zonePos[pos][3]) / 2 / 2.0f;
	float regenz = (pZone->m_zonePos[pos][2] + pZone->m_zonePos[pos][4]) / 2 / 2.0f;

	// 시그널
	int flag = -1;
	int senderindex = -1;
	float signalx = 0.0f;
	float signalz = 0.0f;
	GUILD_SIGNAL* guildsignal = NULL;
	if (pc->m_guildInfo && pc->m_guildInfo->guild())
		guildsignal = pc->m_guildInfo->guild()->guildSignal();

	if (guildsignal)
	{
		flag = guildsignal->flag;
		senderindex = guildsignal->senderIndex;
		signalx = guildsignal->x;
		signalz = guildsignal->z;
	}

	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASTLE_MAP_RECENT
				<< lordx
				<< lordz
				<< lordhp
				<< lordmaxhp
				<< towercount;

	int i;
	for (i = 0; i < towercount; i++)
	{
		CNPC* tower = TO_NPC(castle->GetGateNPC(i));
		if( tower != NULL && tower->m_proto != NULL )
		{
			RefMsg(msg) << tower->m_proto->m_index
						<< GET_X(tower)
						<< GET_Z(tower)
						<< tower->m_hp
						<< tower->m_maxHP;
		}
		else
		{
			RefMsg(msg) << 0
						<< -1
						<< -1
						<< 0
						<< 0;
		}
	}

	RefMsg(msg) << regenx
				<< regenz
				<< flag
				<< senderindex
				<< signalx
				<< signalz;
}

void ExCastleMapSignalMsg(CNetMsg::SP& msg, MSG_GUILD_POSITION_TYPE flag, CPC* sender, float x, float z)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASTLE_MAP_SIGNAL
				<< x
				<< z
				<< flag
				<< sender->m_index;
}

void ExElementalStatusMsg(CNetMsg::SP& msg, CElemental* elemental)
{
	UpdateClient::elementalStatus* packet = reinterpret_cast<UpdateClient::elementalStatus*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_ELEMENTAL_STATUS;

	packet->charIndex = elemental->m_index;
	packet->charType = elemental->GetElementalType();
	packet->remainTime = elemental->GetRemainTime();
	packet->hp = elemental->m_hp;
	packet->maxHp = elemental->m_maxHP;
	packet->attackSpeed = elemental->m_attackSpeed;
	packet->magicSpeed = elemental->m_magicSpeed;
	packet->skillSpeed = elemental->m_skillSpeed;
	packet->walkSpeed = elemental->m_walkSpeed;
	packet->runSpeed = elemental->GetRunSpeed();
	packet->attackRange = elemental->m_attackRange;
	packet->x = GET_X(elemental);
	packet->z = GET_Z(elemental);
	packet->h = GET_H(elemental);
	packet->r = GET_R(elemental);
	packet->yLayer = GET_YLAYER(elemental);
	packet->mapAttr = elemental->GetMapAttr();

	elemental->m_assist.AppendAssistToNetStructMsg(packet->assistinfo);

	msg->setSize(sizeof(UpdateClient::elementalStatus) + (packet->assistinfo.count * sizeof(tag_assistinfo::tag_list)));
}

void ExElementalDeleteMsg(CNetMsg::SP& msg, CElemental* elemental)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_ELEMENTAL_DELETE
				<< elemental->m_index;
}

void ExEvocationStartMsg(CNetMsg::SP& msg, CPC* ch)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_EVOCATION_START
				<< ch->m_index
				<< ch->m_evocationIndex;
}

void ExEvocationStopMsg(CNetMsg::SP& msg, CPC* ch)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_EVOCATION_STOP
				<< ch->m_index;
}

void ExPetChangeItemMsg(CNetMsg::SP& msg, MSG_EX_PET_CHANGE_ITEM_ERROR_TYPE errcode)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_PET_CHANGE_ITEM
				<< (int)errcode;
}

void SysCannotWearDeadPetMsg(CNetMsg::SP& msg, int nPetIndex, int nRemainRebirth)
{
	msg->Init(MSG_SYS);
	RefMsg(msg) << (unsigned char)MSG_SYS_CANNOT_WEAR_DEADPET
				<< nPetIndex
				<< nRemainRebirth;
}

void ExPetRebirthMsg(CNetMsg::SP& msg, int nPetIndex, MSG_EX_PET_REBIRTH_ERROR_TYPE errcode)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_PET_REBIRTH
				<< nPetIndex
				<< errcode;
}

void ExPartyRecallNoticeMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_PARTY_RECALL
				<< MSG_EX_PARTY_RECALL_NOTICE;
}

void ExPartyRecallPromptMsg(CNetMsg::SP& msg, int charindex, const char* charname)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_PARTY_RECALL
				<< MSG_EX_PARTY_RECALL_PROMPT
				<< charindex
				<< charname;
}

void ExPartyRecallCancelMsg(CNetMsg::SP& msg, int requestcharindex, const char* requestcharname, int targetcharindex, const char* targetcharname)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_PARTY_RECALL
				<< MSG_EX_PARTY_RECALL_CANCEL
				<< requestcharindex
				<< requestcharname
				<< targetcharindex
				<< targetcharname;
}

#ifdef EVENT_PACKAGE_ITEM
void ConnCouponConfirm(CNetMsg::SP& msg, int charindex, const char* coupon)
{
	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char)MSG_CONN_COUPON_CONFIRM
				<< charindex
				<< coupon;
}

#ifdef EVENT_PACKAGE_ITEM_DBLOG

void ConnCouponUse(CNetMsg::SP& msg, int charindex, int cIndex, int type, CLCString idname, CLCString nick)
{
	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char)MSG_CONN_COUPON_USE
				<< charindex
				<< cIndex
				<< type
				<< idname
				<< nick
				<< gserver->m_serverno;
}

#else // EVENT_PACKAGE_ITEM_DBLOG

void ConnCouponUse(CNetMsg::SP& msg, int charindex, int cIndex, int type)
{
	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char)MSG_CONN_COUPON_USE
				<< charindex
				<< cIndex
				<< type;
}
#endif // EVENT_PACKAGE_ITEM_DBLOG

#endif

void HelperPartyMemberChangeJobMsg(CNetMsg::SP& msg, int nBossIndex, int nCharIndex, char job1, char job2)
{
	msg->Init(MSG_HELPER_REQ);
	RefMsg(msg) << -1
				<< gserver->m_serverno
				<< gserver->m_subno
				<< (int)-1
				<< (unsigned char)MSG_HELPER_PARTY_MEMBER_CHANGE_JOB
				<< nBossIndex
				<< nCharIndex
				<< job1
				<< job2;
}

void HelperPartyChatMsg(CNetMsg::SP& msg, int nBossIndex, int nCharIndex, const char* strName, const char* strChat)
{
	msg->Init(MSG_HELPER_REQ);
	RefMsg(msg) << -1
				<< gserver->m_serverno
				<< gserver->m_subno
				<< (int)-1
				<< (unsigned char)MSG_HELPER_PARTY_CHAT
				<< nBossIndex
				<< nCharIndex
				<< strName
				<< strChat;
}

void HelperPartyRecallPromptMsg(CNetMsg::SP& msg, int nBossIndex, int nReqIndex, const char* strReqName, char cIsInCastle, int nGuildIndex, bool bUseContinent, int zoneID, CPos& pos)
{
	msg->Init(MSG_HELPER_REQ);
	RefMsg(msg) << -1
				<< gserver->m_serverno
				<< gserver->m_subno
				<< (int)-1
				<< (unsigned char)MSG_HELPER_PARTY_RECALL_PROMPT
				<< nBossIndex
				<< nReqIndex
				<< strReqName
				<< cIsInCastle
				<< nGuildIndex
				<< bUseContinent
				<< zoneID
				<< pos.m_x
				<< pos.m_z
				<< pos.m_yLayer;
}

void HelperPartyRecallConfirmMsg(CNetMsg::SP& msg, int nBossIndex, int nReqIndex, const char* strReqName, int nRepIndex, const char* strRepName, char yesno)
{
	msg->Init(MSG_HELPER_REQ);
	RefMsg(msg) << -1
				<< gserver->m_serverno
				<< gserver->m_subno
				<< (int)-1
				<< (unsigned char)MSG_HELPER_PARTY_RECALL_CONFIRM
				<< nBossIndex
				<< nReqIndex
				<< strReqName
				<< nRepIndex
				<< strRepName
				<< yesno;
}

void HelperPartyRecallProcMsg(CNetMsg::SP& msg, int nBossIndex, int nCharIndex, int nZone, CPos* pos, char cIsInCastle, int nGuildIndex)
{
	msg->Init(MSG_HELPER_REQ);
	RefMsg(msg) << -1
				<< gserver->m_serverno
				<< gserver->m_subno
				<< (int)-1
				<< (unsigned char)MSG_HELPER_PARTY_RECALL_PROC
				<< nBossIndex
				<< nCharIndex
				<< nZone
				<< pos->m_x
				<< pos->m_z
				<< pos->m_yLayer
				<< cIsInCastle
				<< nGuildIndex;
}

void HelperPartyInviteReqMsg(CNetMsg::SP& msg, int nBossIndex, const char* strBossName, int nBossLevel, int nTargetIndex, char partyType)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_INVITE_REQ
				<< nBossIndex
				<< strBossName
				<< nBossLevel
				<< nTargetIndex
				<< partyType;
}

void HelperPartyInviteRepMsg(CNetMsg::SP& msg, int nBossIndex, const char* strBossName, int nTargetIndex, const char* strTargetName, char partyType, MSG_HELPER_PARTY_ERROR_TYPE errcode, int nBossLevel )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_INVITE_REP
				<< nBossIndex
				<< strBossName
				<< nTargetIndex
				<< strTargetName
				<< partyType
				<< errcode;
	RefMsg(msg) << nBossLevel;
}

void HelperPartyRecallConfirmFail(CNetMsg::SP& msg)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_PARTY_RECALL << MSG_EX_PARTY_RECALL_CONFIRM_FAIL;
}

void HelperPartyAllowReqMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex, const char* strTargetName, int nTargetLevel )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_ALLOW_REQ
				<< nBossIndex
				<< nTargetIndex
				<< strTargetName;
	RefMsg(msg) << nTargetLevel;
}

void HelperPartyRejectReqMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_REJECT_REQ
				<< nBossIndex
				<< nTargetIndex;
}

void HelperPartyQuitReqMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_QUIT_REQ
				<< nBossIndex
				<< nTargetIndex;
}

void HelperPartyKickReqMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_KICK_REQ
				<< nBossIndex
				<< nTargetIndex;
}

void HelperPartyChangeBossReqMsg(CNetMsg::SP& msg, int nBossIndex, const char* strTargetName)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_CHANGE_BOSS_REQ
				<< nBossIndex
				<< strTargetName;
}

void HelperPartyChangeTypeReqMsg(CNetMsg::SP& msg, int nBossIndex, char partytype,char cDiviType,char cAllOneSet)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_CHANGE_TYPE_REQ
				<< nBossIndex
				<< partytype
				<< cDiviType
				<< cAllOneSet;
}
void HelperPartyEndPartyReqMsg(CNetMsg::SP& msg, int nBossIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_END_PARTY_REQ
				<< nBossIndex;
}

void HelperPartyBreakReqMsg(CNetMsg::SP& msg, int nBossIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_BREAK_REQ
				<< nBossIndex;
}

void HelperPartyOffline( CNetMsg::SP& msg, int nBossIndex, int targetIndex )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_OFFLINE
				<< nBossIndex
				<< targetIndex;
}

void HelperPartyOnline( CNetMsg::SP& msg, int nBossIndex, int targetIndex , int targetLevel )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_ONLINE
				<< nBossIndex
				<< targetIndex
				<< targetLevel;
}

void MsgrEventOXSetStartMsg(CNetMsg::SP& msg, int nGMCharIndex, int yy, int mm, int dd, int hh, int min)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << -1
				<< gserver->m_serverno
				<< gserver->m_subno
				<< -1
				<< (unsigned char)MSG_MSGR_EVENT_OX
				<< MSG_MSGR_EVENT_OX_SET_START
				<< nGMCharIndex
				<< yy
				<< mm
				<< dd
				<< hh
				<< min;
}

void MsgrEventOXEndMsg(CNetMsg::SP& msg, int nGMCharIndex)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << -1
				<< -1
				<< -1
				<< -1
				<< (unsigned char)MSG_MSGR_EVENT_OX
				<< MSG_MSGR_EVENT_OX_END
				<< nGMCharIndex;
}

void MsgrEventOXQuizMsg(CNetMsg::SP& msg, int nGMCharIndex, int nQuizNo, int nSec)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << -1
				<< -1
				<< -1
				<< -1
				<< (unsigned char)MSG_MSGR_EVENT_OX
				<< MSG_MSGR_EVENT_OX_QUIZ
				<< nGMCharIndex
				<< nQuizNo
				<< nSec;
}

void EventOXQuizMsg(CNetMsg::SP& msg, MSG_EVENT_OXQUIZ_TYPE subtype)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_OXQUIZ
				<< subtype;
}

void EventOXQuizQuizMsg(CNetMsg::SP& msg, int nQuizNo, int nSec, const char* strQuiz)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_OXQUIZ
				<< MSG_EVENT_OXQUIZ_QUIZ
				<< nQuizNo
				<< nSec
				<< strQuiz;
}

void EventOXQuizAnswerMsg(CNetMsg::SP& msg, int nQuizNo, bool bAnswer)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_OXQUIZ
				<< MSG_EVENT_OXQUIZ_ANSWER
				<< nQuizNo;
	if (bAnswer)
		RefMsg(msg) << (char)1;
	else
		RefMsg(msg) << (char)0;
}

void MsgrRecommendMsg(CNetMsg::SP& msg, int nGMCharIndex, int nRecommendServer)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << (int)-1
				<< (int)-1
				<< (int)-1
				<< (int)-1
				<< (unsigned char)MSG_MSGR_RECOMMEND
				<< nGMCharIndex
				<< nRecommendServer;
}

void EventRecommendSetMsg(CNetMsg::SP& msg)
{
	char cFlag = (gserver->m_bRecommend) ? 1 : 0;

	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_RECOMMEND
				<< MSG_EVENT_RECOMMEND_SET
				<< cFlag;
}

void EventRecommendPotionMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_RECOMMEND
				<< MSG_EVENT_RECOMMEND_POTION;
}

void MsgrRecommendMoonstoneMsg(CNetMsg::SP& msg, int nCharIndex, const char* strCharName)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << (int)-1
				<< gserver->m_serverno
				<< gserver->m_subno
				<< (int)-1
				<< (unsigned char)MSG_MSGR_RECOMMEND_MOONSTONE
				<< nCharIndex
				<< strCharName;
}

void EventRecommendMoonstoneMsg(CNetMsg::SP& msg, int nCharIndex, const char* strCharName)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_RECOMMEND
				<< MSG_EVENT_RECOMMEND_MOONSTONE
				<< nCharIndex
				<< strCharName;
}

void HelperPartyMatchRegMemberReqMsg(CNetMsg::SP& msg, int nCharIndex, const char* strCharName, int nLevel, int nZone, char nJob, char nPartyType)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_MATCH_REG_MEMBER_REQ
				<< nCharIndex
				<< strCharName
				<< nLevel
				<< nZone
				<< nJob
				<< nPartyType;
}

void HelperPartyMatchRegPartyReqMsg(CNetMsg::SP& msg, int nBossIndex, const char* strBossName, int nBossLevel, int nZone, int nJobFlag, char cLimitLevel, const char* strComment)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_MATCH_REG_PARTY_REQ
				<< nBossIndex
				<< strBossName
				<< nBossLevel
				<< nZone
				<< nJobFlag
				<< cLimitLevel
				<< strComment;
}

void ExPartyMatchRegMemberRepMsg(CNetMsg::SP& msg, MSG_EX_PARTY_MATCH_ERROR_TYPE errcode)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_PARTY_MATCH
				<< MSG_EX_PARTY_MATCH_REG_MEMBER_REP
				<< errcode;
}

void ExPartyMatchRegPartyRepMsg(CNetMsg::SP& msg, MSG_EX_PARTY_MATCH_ERROR_TYPE errcode)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_PARTY_MATCH
				<< MSG_EX_PARTY_MATCH_REG_PARTY_REP
				<< errcode;
}

void ExPartyMatchMemberListRepMsg(CNetMsg::SP& msg, int nPageNo, int nCharLevel, std::vector<CPartyMatchMember*>& list)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_PARTY_MATCH
				<< MSG_EX_PARTY_MATCH_MEMBER_LIST_REP
				<< nPageNo
				<< (int)list.size();

	std::vector<CPartyMatchMember*>::iterator it = list.begin();
	std::vector<CPartyMatchMember*>::iterator endit = list.end();
	for(; it != endit; ++it)
	{
		CPartyMatchMember* pMatchMember = *(it);
		char cLevelDiff = (ABS(nCharLevel - pMatchMember->GetLevel()) > PARTY_MATCHING_LIMIT_LEVEL) ? 1 : 0;
		RefMsg(msg) << pMatchMember->GetIndex()
					<< pMatchMember->GetName()
					<< cLevelDiff
					<< pMatchMember->GetZone()
					<< pMatchMember->GetJob()
					<< pMatchMember->GetPartyType();
	}
}

void ExPartyMatchPartyListRepMsg(CNetMsg::SP& msg, int nPageNo, int nCharLevel, std::vector<CPartyMatchParty*>& list)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_PARTY_MATCH
				<< MSG_EX_PARTY_MATCH_PARTY_LIST_REP
				<< nPageNo
				<< (int)list.size();

	std::vector<CPartyMatchParty*>::iterator it = list.begin();
	std::vector<CPartyMatchParty*>::iterator endit = list.end();
	for(; it != endit; ++it)
	{
		CPartyMatchParty* pMatchParty = *(it);
		char cLevelDiff = (ABS(nCharLevel - pMatchParty->GetBossLevel()) > PARTY_MATCHING_LIMIT_LEVEL) ? 1 : 0;
		char cLimitLevel = (pMatchParty->IsLimitLevel()) ? 1 : 0;
		RefMsg(msg) << pMatchParty->GetBossIndex()
					<< cLevelDiff
					<< pMatchParty->GetZone()
					<< pMatchParty->GetMemberCount()
					<< pMatchParty->GetJobFlag()
					<< pMatchParty->GetPartyType()
					<< cLimitLevel
					<< pMatchParty->GetComment();
	}
}

void HelperPartyMatchDelReqMsg(CNetMsg::SP& msg, int nCharIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_MATCH_DEL_REQ
				<< nCharIndex;
}

void ExPartyMatchDelRepMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_PARTY_MATCH
				<< MSG_EX_PARTY_MATCH_DEL_REP;
}

void HelperPartyMatchInviteReqMsg(CNetMsg::SP& msg, int nBossIndex, const char* strBossName, int nBossLevel, int nCharIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_MATCH_INVITE_REQ
				<< nBossIndex
				<< strBossName
				<< nBossLevel
				<< nCharIndex
				<< (char)0;
}

void HelperPartyMatchInviteRepMsg(CNetMsg::SP& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, int nBossIndex, const char* strBossName, int nCharIndex, const char* strCharName, char cPartyType)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_MATCH_INVITE_REP
				<< nErrorCode
				<< nBossIndex
				<< strBossName
				<< nCharIndex
				<< strCharName
				<< cPartyType;
}

void ExPartyMatchInviteRepMsg(CNetMsg::SP& msg, MSG_EX_PARTY_MATCH_ERROR_TYPE nErrorCode, char cPartyType, int nBossIndex, const char* strBossName, int nCharIndex, const char* strCharName)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_PARTY_MATCH
				<< MSG_EX_PARTY_MATCH_INVITE_REP
				<< nErrorCode
				<< cPartyType
				<< nBossIndex
				<< strBossName
				<< nCharIndex
				<< strCharName;
}

void HelperPartyMatchJoinReqMsg(CNetMsg::SP& msg, int nBossIndex, int nCharIndex, const char* strCharName, int nCharLevel, char cCharJob)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_MATCH_JOIN_REQ
				<< nBossIndex
				<< nCharIndex
				<< strCharName
				<< nCharLevel
				<< cCharJob;
}

void ExPartyMatchJoinRepMsg(CNetMsg::SP& msg, MSG_EX_PARTY_MATCH_ERROR_TYPE nErrorCode, char cPartyType, int nBossIndex, const char* strBossName, int nReqCharIndex, const char* strReqCharName, char cReqCharJob)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_PARTY_MATCH
				<< MSG_EX_PARTY_MATCH_JOIN_REP
				<< nErrorCode
				<< cPartyType
				<< nBossIndex
				<< strBossName
				<< nReqCharIndex
				<< strReqCharName
				<< cReqCharJob;
}
void HelperPartyMatchJoinAllowReqMsg(CNetMsg::SP& msg, int nBossIndex, int nCharIndex, int nLevel, int reqCharLevel )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_MATCH_JOIN_ALLOW_REQ
				<< nBossIndex
				<< nCharIndex;
	RefMsg(msg) << nLevel;
	RefMsg(msg) << reqCharLevel;
}

void HelperPartyMatchJoinRejectReqMsg(CNetMsg::SP& msg, int nJoinCharIndex, int nRejectCharIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_MATCH_JOIN_REJECT_REQ
				<< nJoinCharIndex
				<< nRejectCharIndex;
}

void HelperPartyMatchMemberChangeInfoMsg(CNetMsg::SP& msg, int nCharIndex, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_TYPE nType, const char* strCharName, int nLevel, int nZone)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO
				<< nCharIndex
				<< nType;

	switch (nType)
	{
	case MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_NAME:
		RefMsg(msg) << strCharName;
		break;

	case MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_LEVEL:
		RefMsg(msg) << nLevel;
		break;

	case MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_ZONE:
		RefMsg(msg) << nLevel
					<< nZone;
		break;
	}
}

void SysExpireItemcompositeMsg(CNetMsg::SP& msg, int nItemDBIndex)
{
	msg->Init(MSG_SYS);
	RefMsg(msg) << (unsigned char)MSG_SYS_EXPIRE_ITEMCOMPOSITE
				<< nItemDBIndex;
}

void EventWorldcupVoteMsg(CNetMsg::SP& msg, int nCountry, MSG_EVENT_WORLDCUP_ERROR_TYPE nErrorCode)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_WORLDCUP
				<< MSG_EVENT_WORLDCUP_VOTE
				<< nCountry
				<< nErrorCode;
}

void EventWorldcupGiftMsg(CNetMsg::SP& msg, int nRank, MSG_EVENT_WORLDCUP_ERROR_TYPE nErrorCode)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_WORLDCUP
				<< MSG_EVENT_WORLDCUP_GIFT
				<< nRank
				<< nErrorCode;
}

void MsgrEventGoldenballVoteMsg(CNetMsg::SP& msg, int nGMCharIndex, const char* strTeam1, const char* strTeam2, int nYear, int nMonth, int nDay, int nHour, int nMinute)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << (int)-1
				<< (int)-1
				<< (int)-1
				<< (int)-1
				<< (unsigned char)MSG_MSGR_EVENT_GOLDENBALL
				<< MSG_MSGR_EVENT_GOLDENBALL_VOTE
				<< nGMCharIndex
				<< strTeam1
				<< strTeam2
				<< nYear
				<< nMonth
				<< nDay
				<< nHour
				<< nMinute;
}

void MsgrEventGoldenballGiftMsg(CNetMsg::SP& msg, int nGMCharIndex, const char* strTeam1, int nTeam1Score, const char* strTeam2, int nTeam2Score, int nYear, int nMonth, int nDay, int nHour, int nMinute, time_t timeEndVote)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << (int)-1
				<< (int)-1
				<< (int)-1
				<< (int)-1
				<< (unsigned char)MSG_MSGR_EVENT_GOLDENBALL
				<< MSG_MSGR_EVENT_GOLDENBALL_GIFT
				<< nGMCharIndex
				<< strTeam1
				<< nTeam1Score
				<< strTeam2
				<< nTeam2Score
				<< nYear
				<< nMonth
				<< nDay
				<< nHour
				<< nMinute
				<< (int)timeEndVote;
}

void MsgrEventGoldenballEndMsg(CNetMsg::SP& msg, int nGMCharIndex)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << (int)-1
				<< (int)-1
				<< (int)-1
				<< (int)-1
				<< (unsigned char)MSG_MSGR_EVENT_GOLDENBALL
				<< MSG_MSGR_EVENT_GOLDENBALL_END
				<< nGMCharIndex;
}

void EventGoldenballVoteStartMsg(CNetMsg::SP& msg, const char* strTeam1, const char* strTeam2, int nYear, int nMonth, int nDay, int nHour, int nMin)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_GOLDENBALL
				<< MSG_EVENT_GOLDENBALL_VOTE_START
				<< strTeam1
				<< strTeam2
				<< nYear
				<< nMonth
				<< nDay
				<< nHour
				<< nMin;
}

void EventGoldenballVoteEndMsg(CNetMsg::SP& msg, const char* strTeam1, const char* strTeam2)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_GOLDENBALL
				<< MSG_EVENT_GOLDENBALL_VOTE_END
				<< strTeam1
				<< strTeam2;
}

void EventGoldenballGiftStartMsg(CNetMsg::SP& msg, const char* strTeam1, int nTeam1Score, const char* strTeam2, int nTeam2Score, int nYear, int nMonth, int nDay, int nHour, int nMin)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_GOLDENBALL
				<< MSG_EVENT_GOLDENBALL_GIFT_START
				<< strTeam1
				<< nTeam1Score
				<< strTeam2
				<< nTeam2Score
				<< nYear
				<< nMonth
				<< nDay
				<< nHour
				<< nMin;
}

void EventGoldenballGiftEndMsg(CNetMsg::SP& msg, const char* strTeam1, const char* strTeam2)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_GOLDENBALL
				<< MSG_EVENT_GOLDENBALL_GIFT_END
				<< strTeam1
				<< strTeam2;
}

void EventGoldenballVoteMsg(CNetMsg::SP& msg, int nTeam1Score, int nTeam2Score, MSG_EVENT_GOLDENBALL_ERROR_TYPE nErrorCode)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_GOLDENBALL
				<< MSG_EVENT_GOLDENBALL_VOTE
				<< nTeam1Score
				<< nTeam2Score
				<< nErrorCode;
}

void EventGoldenballGiftMsg(CNetMsg::SP& msg, MSG_EVENT_GOLDENBALL_ERROR_TYPE nErrorCode)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_GOLDENBALL
				<< MSG_EVENT_GOLDENBALL_GIFT
				<< nErrorCode;
}

void EventRain2006Msg(CNetMsg::SP& msg, MSG_EVENT_RAIN_2006_ERROR_TYPE nErrorCode)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_RAIN_2006
				<< nErrorCode;
}

void ItemLendWeaponMsg(CNetMsg::SP& msg, int nItemDBIndex, MSG_ITEM_LEND_WEAPON_ERROR_TYPE nErrorCode)
{
	msg->Init(MSG_ITEM);
	RefMsg(msg) << (unsigned char)MSG_ITEM_LEND_WEAPON
				<< nItemDBIndex
				<< nErrorCode;
}

void SysSeparateItemMsg(CNetMsg::SP& msg, int nItemDBIndex)
{
	msg->Init(MSG_SYS);
	RefMsg(msg) << (unsigned char)MSG_SYS_SEPARATE_ITEM
				<< nItemDBIndex;
}

void EventCollectBugMsg(CNetMsg::SP& msg, MSG_EVENT_COLLECT_BUG_TYPE nType, int nData)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_COLLECT_BUG
				<< nType
				<< nData;
}

void QuestCollectMsg(CNetMsg::SP& msg, int nNPCIndex, int nCharIndex, int nItemIndex)
{
	msg->Init(MSG_QUEST);
	RefMsg(msg) << (unsigned char)MSG_QUEST_COLLECT
				<< nNPCIndex
				<< nCharIndex
				<< nItemIndex;
}

void EventChuseok2006Msg(CNetMsg::SP& msg, MSG_EVENT_CHUSEOK_2006_TYPE nErrorcode)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_CHUSEOK_2006
				<< nErrorcode;
}

void MsgrLoginServerMsg(CNetMsg::SP& msg, int nCode)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << (int)-1
				<< (int)-1
				<< (int)-1
				<< (int)-1
				<< (unsigned char)MSG_MSGR_LOGIN_SERVER
				<< nCode;
}

void SysRaidmobRegenMsg(CNetMsg::SP& msg, int nNPCIndex, float x, float z)
{
	msg->Init(MSG_SYS);
	RefMsg(msg) << (unsigned char)MSG_SYS_RAIDMOB_REGEN
				<< nNPCIndex
				<< x
				<< z;
}

void ExHairChangeMsg(CNetMsg::SP& msg, CPC* pPC )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_HAIR_CHANGE
				<< pPC->m_index
				<< pPC->m_hairstyle;
	if( pPC->m_wearInventory.wearItemInfo[WEARING_HELMET] )
	{
		RefMsg(msg) << pPC->m_wearInventory.wearItemInfo[WEARING_HELMET]->m_itemProto->getItemIndex();
	}
}

void ExPlayerStateChangeMsg(CNetMsg::SP& msg, CPC* pPC)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_PLAYER_STATE_CHANGE
				<< pPC->m_index
				<< pPC->GetPlayerState();
}

void SysChangeLuckyBoxMsg(CNetMsg::SP& msg, int nItemDBIndex, int nUseDBIndex)
{
	msg->Init(MSG_SYS);
	RefMsg(msg) << (unsigned char)MSG_SYS_CHANGE_LUCKY_BOX
				<< nItemDBIndex
				<< nUseDBIndex;
}

void SysChangeRaidBoxMsg(CNetMsg::SP& msg, int nItemDBIndex, int nUseDBIndex)
{
	msg->Init(MSG_SYS);
	RefMsg(msg) << (unsigned char)MSG_SYS_CHANGE_RAID_BOX
				<< nItemDBIndex
				<< nUseDBIndex;
}

void EventValentine2007ErrorMsg(CNetMsg::SP& msg, MSG_EVENT_VALENTINE_TYPE nErrorcode)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_VALENTINE_2007
				<< nErrorcode;
}

void EventValentine2007Msg(CNetMsg::SP& msg, MSG_EVENT_VALENTINE_TYPE nErrorcode, int extra)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_VALENTINE_2007
				<< nErrorcode
				<< extra;
}

#ifdef FREE_PK_SYSTEM
void FreePKMsg(CNetMsg::SP& msg)
{
	msg->Init( MSG_PK );
	RefMsg(msg) << (unsigned char)MSG_PK_FREE;
}
#endif //FREE_PK_SYSTEM

void EventWhiteday2007Msg(CNetMsg::SP& msg, MSG_EVENT_WHITEDAY_2007_TYPE nMsg)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_WHITEDAY_2007
				<< nMsg;
}

void EventEggsHunt2007ErrorMsg(CNetMsg::SP& msg, MSG_EVENT_EGGS_HUNT_2007_TYPE nMsg)
{
	msg->Init( MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_EGGS_HUNT_2007
				<< nMsg;
}

void EventGomdori2007Msg(CNetMsg::SP& msg, MSG_EVENT_GOMDORI_2007_TYPE nType)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_GOMDORI_2007
				<< (unsigned char)nType;
}

void EventGomdori2007ResultMsg(CNetMsg::SP& msg, MSG_EVENT_GOMDORI_2007_TYPE nResult, char cWinCount, char cUserSel, char cNPCSel)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_GOMDORI_2007
				<< (unsigned char)nResult
				<< cWinCount
				<< cUserSel
				<< cNPCSel;
}

void EventGomdori2007EndMsg(CNetMsg::SP& msg, char cWinCount, char cPromptType, int nExtra1, int nExtra2)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_GOMDORI_2007
				<< (unsigned char)MSG_EVENT_GOMDORI_2007_END
				<< cWinCount
				<< cPromptType
				<< nExtra1
				<< nExtra2;
}

void EventGomdori2007SelectGiftMsg(CNetMsg::SP& msg, int nItemDBIndex)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_GOMDORI_2007
				<< (unsigned char)MSG_EVENT_GOMDORI_2007_SELECT_GIFT
				<< nItemDBIndex;
}

void EventGomdori2007ViewStatusMsg(CNetMsg::SP& msg, int nCount, int* nStatus)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_GOMDORI_2007
				<< (unsigned char)MSG_EVENT_GOMDORI_2007_VIEW_STATUS
				<< nCount;
	int i;
	for (i = 0; i < nCount; i++)
		RefMsg(msg) << nStatus[i];
}

void ConnEventGomdori2007CountMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char)MSG_CONN_EVENT_GOMDORI_2007
				<< (unsigned char)MSG_CONN_EVENT_GOMDORI_2007_COUNT;
}

void ConnEventGomdori2007StatusMsg(CNetMsg::SP& msg, int nCharIndex)
{
	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char)MSG_CONN_EVENT_GOMDORI_2007
				<< (unsigned char)MSG_CONN_EVENT_GOMDORI_2007_STATUS
				<< nCharIndex;
}

#ifdef GMTOOL
void ConnGMToolCommand(CNetMsg::SP& msg, int gmindex, const char* gmname, const char* gmcommand)
{
	msg->Init( MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char)MSG_CONN_GMTOOL_GOMMAND
				<< gmindex
				<< gmname
				<< gmcommand;
}

void ConnGMToolChatMonitor(CNetMsg::SP& msg, int charindex, const char* name, unsigned char chattype, int server, int sub, const char* chat)
{
	msg->Init( MSG_CONN_REQ );
	RefMsg(msg) << (unsigned char)MSG_CONN_GMTOOL_CHATMONITOR
				<< charindex
				<< name
				<< chattype
				<< server
				<< sub
				<< chat;
}

void ConnGMToolChatting(CNetMsg::SP& msg, int server, int sub, int gmindex, const char* gmname, const char* name, const char* chat)
{
	msg->Init( MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char)MSG_CONN_GMTOOL_CHATTING
				<< server
				<< sub
				<< gmindex
				<< gmname
				<< name
				<< chat;
}
#endif // GMTOOL

void PetNameChange( CNetMsg::SP& msg, MSG_EX_PET_CHANGE_NAME_ERROR_TYPE err, int petidx, const char* strPetName )
{
	msg->Init(MSG_EXTEND );
	RefMsg(msg) << MSG_EX_PET_CHANGE_NAME
				<< (unsigned char)err
				<< petidx
				<< strPetName;
}

void HelperPetNameChange( CNetMsg::SP& msg, int charindex, int petindex, const char* strPetName )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_PET_NAME_CHANGE
				<< charindex
				<< petindex
				<< strPetName;
}

#ifdef DOUBLE_ITEM_DROP
void MsgrDoubleItemEventRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int itemPercent)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char) MSG_MSGR_DOUBLE_ITEM_EVENT_REP
				<< charindex
				<< gserver->m_serverno
				<< gserver->m_subno
				<< cmd
				<< itemPercent;
}

void MsgrDoubleItemEventReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int itemPercent)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char) MSG_MSGR_DOUBLE_ITEM_EVENT_REQ
				<< charindex
				<< cmd
				<< itemPercent;
}
#endif

#if defined( EVENT_SUMMER_VACATION_2007_RESULT )
void EventSummerVacationInchenMsg( CNetMsg::SP& msg, MSG_EVENT_SUMMER_VACATION_2007_TYPE vacationType, int itemindex )
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_SUMMER_VACATION_2007
				//<< (unsigned char)MSG_EVENT_SUMMER_VACATION_2007_INCHEN
				<< vacationType
				<< itemindex;
}
void EventSummerVacation2007FigureMsg( CNetMsg::SP& msg, MSG_EVENT_SUMMER_VACATION_2007_TYPE vacationType, int figuretype, int itemindex, int count )
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_SUMMER_VACATION_2007
				//<< (unsigned char)MSG_EVENT_SUMMER_VACATION_2007_PAPER_FIGURES
				<< vacationType
				<< figuretype
				<< itemindex
				<< count;
}
#endif// defined( EVENT_SUMMER_VACATION_2007_RESULT )

#ifdef DOUBLE_PET_EXP
#ifdef DOUBLE_PET_EXP_AUTO
void MsgrDoublePetExpEventReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int PetExpPercent, long start, long end)
#else
void MsgrDoublePetExpEventReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int PetExpPercent)
#endif
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char) MSG_MSGR_DOUBLE_PET_EXP_EVENT_REQ
				<< charindex
				<< cmd
				<< PetExpPercent;
#ifdef DOUBLE_PET_EXP_AUTO
	RefMsg(msg) << start
				<< end;
#endif
}
#ifdef DOUBLE_PET_EXP_AUTO
void MsgrDoublePetExpEventRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int PetExpPercent, long start, long end)
#else
void MsgrDoublePetExpEventRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int PetExpPercent)
#endif
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char) MSG_MSGR_DOUBLE_PET_EXP_EVENT_REP
				<< charindex
				<< gserver->m_serverno
				<< gserver->m_subno
				<< cmd
				<< PetExpPercent;
#ifdef DOUBLE_PET_EXP_AUTO
	RefMsg(msg) << start
				<< end;
#endif
}
#endif // DOUBLE_PET_EXP

#ifdef DOUBLE_ATTACK
void MsgrDoubleAttackEventReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int AttackPercent)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char) MSG_MSGR_DOUBLE_ATTACK_EVENT_REQ
				<< charindex
				<< cmd
				<< AttackPercent;
}
void MsgrDoubleAttackEventRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int AttackPercent)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char) MSG_MSGR_DOUBLE_ATTACK_EVENT_REP
				<< charindex
				<< gserver->m_serverno
				<< gserver->m_subno
				<< cmd
				<< AttackPercent;
}
#endif // DOUBLE_ATTACK

void GuildInclineEstablishMsg( CNetMsg::SP& msg, char guildincline )
{
	msg->Init( MSG_GUILD );
	RefMsg(msg) << (unsigned char)MSG_NEW_GUILD_INCLINE_ESTABLISH
				<< guildincline;
}

void GuildNewInfo( CNetMsg::SP& msg, CPC* ch, int avelevel, int guildpoint, int usepoint )
{
#if defined (LC_GAMIGO)
	// -- imso
	if (!ch->m_guildInfo->guild()->boss())
	{
		GAMELOG << init("GuildNewInfo", ch)
				<< "guild_index " << delim << ch->m_guildInfo->guild()->index() << delim
				<< "guild_name " << delim << ch->m_guildInfo->guild()->name() << end;
	}
#endif

	msg->Init( MSG_GUILD );
	RefMsg(msg) << (unsigned char)MSG_NEW_GUILD_INFO
				<< ch->m_guildInfo->guild()->name()
				<< ch->m_guildInfo->guild()->level()
				<< ch->m_guildInfo->guild()->boss()->GetName()
				<< ch->m_guildInfo->guild()->membercount()
				<< ch->m_guildInfo->guild()->maxmember()
				<< avelevel
				<< ch->m_guildInfo->guild()->GetGuildPoint()
				<< ch->m_guildInfo->GetcumulatePoint();
#ifdef DEV_GUILD_MARK
	RefMsg(msg) << (char)ch->m_guildInfo->guild()->GetGuildMarkRow()
				<< (char)ch->m_guildInfo->guild()->GetGuildMarkCol()
				<< (char)ch->m_guildInfo->guild()->GetBackgroundRow()
				<< (char)ch->m_guildInfo->guild()->GetBackgroundCol()
				<< (int)ch->m_guildInfo->guild()->GetMarkTimeConvertGameTime();
#endif
//	int landcount = ch->m_guildInfo->guild()->GetLandCount();
//	if( ch->m_guildInfo->guild()->GetLandCount() > 0 )
//	{
//		RefMsg(msg) << landcount;
//		int* land = ch->m_guildInfo->guild()->GetLand();
//		memset(land, -1, sizeof(land) );
//		for( int i = 0 ; i < landcount; i++ )
//		{
//			RefMsg(msg) << land[i];
//		}
//
//	}
//
}

void GuildPointInfo(CNetMsg::SP& msg, int guildpoint)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_GP_INFO
				<< guildpoint;
}

void GuildMemberListRep( CNetMsg::SP& msg, int membercount, int* membercharindex, int* cumulatePoint,  const char CharName[][MAX_CHAR_NAME_LENGTH  + 1], const char PositionName[][GUILD_POSITION_NAME+1],  char* job, char* job2, int* level, int* position, CGuild* guild )
{
	msg->Init( MSG_GUILD );
	RefMsg(msg) << (unsigned char)MSG_NEW_GUILD_MEMBERLIST
				<< membercount	;
	for( int i = 0; i < membercount; i++ )
	{
		CGuildMember* guildmember = guild->findmember( membercharindex[i] );

		if( guildmember )
		{
			RefMsg(msg) << CharName[i]
						<< PositionName[i]
						<< level[i]
						<< guildmember->online();

			if( guildmember->online() != 0 )
			{
				RefMsg(msg) << guildmember->GetZoneNum();
			}
			else
			{
				RefMsg(msg) << -1;
			}
			RefMsg(msg) << cumulatePoint[i];
			RefMsg(msg) << job[i]
						<< job2[i]
						<< guildmember->pos();
		}
	}

	if( guild)
	{
		RefMsg(msg) << guild->m_nCountTotalGradeEx
					<< guild->m_nCountRushCaptain+guild->m_nCountSupportCaptain+guild->m_nCountReconCaptain;
	}
	else
	{
		RefMsg(msg) << 0 << 0;
	}
}

#ifdef DEV_GUILD_STASH
void GuildNewManageRep( CNetMsg::SP& msg, int membercount, int* membercharindex, int* contributeExp, int* contributeFame, const char CharName[][MAX_CHAR_NAME_LENGTH  + 1], const char PositionName[][GUILD_POSITION_NAME+1], int* level, int* position, char* stashAuth, CGuild* guild, char first )
#else
void GuildNewManageRep( CNetMsg::SP& msg, int membercount, int* membercharindex, int* contributeExp, int* contributeFame, const char CharName[][MAX_CHAR_NAME_LENGTH  + 1], const char PositionName[][GUILD_POSITION_NAME+1], int* level, int* position, CGuild* guild, char first )
#endif // DEV_GUILD_STASH
{
	msg->Init( MSG_GUILD );
	RefMsg(msg) << (unsigned char)MSG_NEW_GUILD_MANAGE
				<< first
				<< membercount	;
	for( int i = 0; i < membercount; i++ )
	{
		CGuildMember* guildmember = guild->findmember( membercharindex[i] );
		if( guildmember )
		{
			if( guildmember->GetPC() )
			{
				RefMsg(msg) << guildmember->pos()
							<< guildmember->GetName()
							<< guildmember->GetPositionName()
							<< guildmember->GetPC()->m_level
							<< contributeExp[i]
							<< contributeFame[i]
							<< guildmember->charindex();
#ifdef DEV_GUILD_STASH
				RefMsg(msg) << stashAuth[i];			//TODO:: 주의
#endif //DEV_GUILD_STASH
			}
			else
			{
				RefMsg(msg) << position[i]
							<< CharName[i]
							<< PositionName[i]
							<< level[i]
							<< contributeExp[i]
							<< contributeFame[i]
							<< membercharindex[i];
#ifdef DEV_GUILD_STASH
				RefMsg(msg) << stashAuth[i];
#endif //DEV_GUILD_STASH
			}
		}
	}

	if( guild )
	{
		RefMsg(msg) << guild->m_nCountTotalGradeEx
					<< guild->m_nCountRushCaptain+guild->m_nCountSupportCaptain+guild->m_nCountReconCaptain;
	}
	else
	{
		RefMsg(msg) << 0 << 0;
	}
}

void GuildNewNotify( CNetMsg::SP& msg, const char* title, const char* text )
{
	msg->Init( MSG_GUILD );
	RefMsg(msg) << (unsigned char)MSG_NEW_GUILD_NOTICE
				<< title
				<< text;
}

void GuildNewNotifyTrans( CNetMsg::SP& msg, const char* guildname, const char* title, const char* text )
{
	msg->Init( MSG_GUILD );
	RefMsg(msg) << (unsigned char)MSG_NEW_GUILD_NOTICE_TRANSMISSION
				<< guildname
				<< title
				<< text;
}
void GuildSkillListRepMsg(CNetMsg::SP& msg, int active_count,	int active_skill_index[],	int active_skill_level[], int active_skill_cooltime[],
						  int passive_count,	int passive_skill_index[],	int passive_skill_level[],
						  int etc_count,		int etc_skill_index[],		int etc_skill_level[])
{
	msg->Init( MSG_GUILD );
	RefMsg(msg) << (unsigned char)MSG_NEW_GUILD_SKILL;
	int i;
	RefMsg(msg) << GUILD_ACTIVE_SKILL_START
				<< active_count;
	for(i = 0; i < active_count; i++)
	{
		RefMsg(msg) << active_skill_index[i]
					<< active_skill_level[i]
					<< active_skill_cooltime[i];
	}
	RefMsg(msg) << GUILD_PASSIVE_SKILL_START
				<< passive_count;
	for(i = 0; i < passive_count; i++)
	{
		RefMsg(msg) << passive_skill_index[i]
					<< passive_skill_level[i];
	}
	RefMsg(msg) << GUILD_ETC_SKILL_START
				<< etc_count;
	for(i = 0; i < etc_count; i++)
	{
		RefMsg(msg) << etc_skill_index[i]
					<< etc_skill_level[i];
	}
}

void GuildSkillLearnMsg(CNetMsg::SP& msg, CSkill* skill)
{
	msg->Init(MSG_GUILD);

	RefMsg(msg) << (unsigned char)MSG_NEW_GUILD_SKILL_LEARN;
	if(skill && skill->m_proto)
	{
		if(skill->m_proto->IsActive())
			RefMsg(msg) << (unsigned char)SKILL_TYPE_ACTIVE;
		else if(skill->m_proto->IsPassive())
			RefMsg(msg) << (unsigned char)SKILL_TYPE_PASSIVE;
		else
			RefMsg(msg) << (unsigned char)SKILL_TYPE_ETC;
	}
	RefMsg(msg) << skill->m_proto->m_index
				<< skill->m_level;
}

void HelperGuildInclineEstablishReqMsg( CNetMsg::SP& msg, CPC* ch, char guildincline )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_GUILD_INCLINE_ESTABLISH_REQ
				<< ch->m_guildInfo->guild()->index()
				<< ch->m_index
				<< guildincline;
}

void HelperGuildMemberAdjust( CNetMsg::SP& msg, CPC* ch, int charindex, const char* strPositionName, int contributeExp, int contributFame, int pos )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_GUILD_MEMBER_ADJUST_REQ
				<< ch->m_guildInfo->guild()->index()
				<< ch->m_index
				<< charindex
				<< strPositionName
				<< contributeExp
				<< contributFame;
	RefMsg(msg) << pos;
}

void HelperNewGuildInfo( CNetMsg::SP& msg, CPC* ch )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_NEW_GUILD_INFO_REQ
				<< ch->m_guildInfo->guild()->index()
				<< ch->m_index;
}

void HelperNewGuildMemberList( CNetMsg::SP& msg, CPC* ch )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_NEW_GUILD_MEMBERLIST_REQ
				<< ch->m_guildInfo->guild()->index()
				<< ch->m_index;
}
void HelperNewGuildManage( CNetMsg::SP& msg, CPC* ch )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_NEW_GUILD_MANAGE_REQ
				<< ch->m_guildInfo->guild()->index()
				<< ch->m_index;
}
void HelperNewGuildNotice( CNetMsg::SP& msg, CPC* ch )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_NEW_GUILD_NOTICE_REQ
				<< ch->m_guildInfo->guild()->index()
				<< ch->m_index;
}
void HelperNewGuildNoticeUpdate( CNetMsg::SP& msg, CPC* ch, const char* title, const char* text )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_NEW_GUILD_NOTICE_UPDATE_REQ
				<< ch->m_guildInfo->guild()->index()
				<< ch->m_index
				<< title
				<< text;
}

void HelperNewGuildNoticeTrans( CNetMsg::SP& msg, CPC* ch )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_NEW_GUILD_NOTICE_TRANSMISSION_REQ
				<< ch->m_guildInfo->guild()->index()
				<< ch->m_index;
}

void HelperNewGuildSkillList( CNetMsg::SP& msg, CPC* ch )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_NEW_GUILD_NOTICE_SKILLLIST_REQ
				<< ch->m_guildInfo->guild()->index()
				<< ch->m_index;
}

void HelperSaveGuildPointMsg(CNetMsg::SP& msg, int guildindex, int guildpoint )
{
	msg->Init( MSG_HELPER_COMMAND );

	RefMsg(msg) << MSG_HELPER_SAVE_GUILD_POINT
				<< guildindex
				<< guildpoint;
}

void HelperSaveGuildMemberPointMsg( CNetMsg::SP& msg, int guildindex, int charindex, int GuildMemberPoint )
{
	msg->Init( MSG_HELPER_COMMAND );

	RefMsg(msg) << MSG_HELPER_SAVE_GUILD_MEMBER_POINT
				<< guildindex
				<< charindex
				<< GuildMemberPoint;
}

//void HelperSaveGuildSkillMsg( CNetMsg::SP& msg, CGuild* guild )
//{
//	char level[1024];	// for temp string
//	char index[1024];	// for temp string
//
//	msg->Init( MSG_HELPER_COMMAND );
//	RefMsg(msg) << MSG_HELPER_SAVE_GUILD_SKILL
//		<< guild->index()
//		<< guild->m_passiveSkillList.GetIndexString(index)
//		<< guild->m_passiveSkillList.GetLevelString(level);
//}

void HelperNewGuildPointUpdate( CNetMsg::SP& msg, int charindex, int guildindex, int point )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_UPDATE_GUILD_POINT
				<< charindex
				<< guildindex
				<< point;
}

void HelperGuildLoadReq(CNetMsg::SP& msg, int charindex )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_GUILD_NOTICE
				<< charindex;
}

void HelperGuildSkillLearnMsg( CNetMsg::SP& msg, int gp, CGuild* guild, int skilltype )
{
	char level[1024];	// for temp string
	char index[1024];	// for temp string
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_SKILL_LEARN
				<< guild->index()
				<< gp;
	switch(skilltype)
	{
	case 0:
		RefMsg(msg) << skilltype
					<< guild->m_activeSkillList.GetIndexString(index)
					<< guild->m_activeSkillList.GetLevelString(level);
		break;
	case 1:
		RefMsg(msg) << skilltype
					<< guild->m_passiveSkillList.GetIndexString(index)
					<< guild->m_passiveSkillList.GetLevelString(level);
		break;
	case 2:
		RefMsg(msg) << skilltype
					<< guild->m_etcSkillList.GetIndexString(index)
					<< guild->m_etcSkillList.GetLevelString(level);
		break;
	}
}

void HelperGuildSkillLearnSendMemberMsg( CNetMsg::SP& msg, int guild_index, int skill_type, int skill_index, int skill_level)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_SKILL_LEARN_SEND_MEMBER
		<< guild_index
		<< skill_type
		<< skill_index
		<< skill_level;
}

void ConnGuildPointRankerReqMsg( CNetMsg::SP& msg, int guildindex, int charindex )
{
	msg->Init( MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char)MSG_CONN_GUILD_POINT_RANKER_REQ
				<< guildindex
				<< charindex;
}

void GuildPointRankingMsg( CNetMsg::SP& msg, int nCharindex, int nGuildindex, int nRanking )
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_NEW_GUILD_POINT_RANKING
				<< nCharindex
				<< nGuildindex
				<< nRanking;
}

void HelperInsertGuardMsg(CNetMsg::SP& msg, CPC * ch, CItem * item)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_INSERT_GUARD
				<< ch->m_pZone->m_index
				<< ch->m_pos.m_x
				<< ch->m_pos.m_z
				<< ch->m_pos.m_h
				<< item->m_itemProto->getItemIndex();
}

void CastleCristalRespondStartMsg(CNetMsg::SP& msg, CPC * ch)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASTLE_WAR
				<< (unsigned char)MSG_CASTLE_CRISTAL_RESPOND_START
				<< ch->m_index;
}

void CastleCristalRespondEndMsg(CNetMsg::SP& msg, CPC * ch)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASTLE_WAR
				<< (unsigned char)MSG_CASTLE_CRISTAL_RESPOND_END
				<< ch->m_index
				<< ch->GetName();
}

void CastleCristalRespondFailMsg(CNetMsg::SP& msg, CPC * ch)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASTLE_WAR
				<< (unsigned char)MSG_CASTLE_CRISTAL_RESPOND_FAIL
				<< ch->m_index;
}

void CastleTowerListMsg(CNetMsg::SP& msg, CDratanCastle * pCastle)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASTLE_WAR
				<< (unsigned char)MSG_CASTLE_TOWER_CONTRAL_LIST;
	for(int i=0; i<7; i++)
	{
		RefMsg(msg) << pCastle->GetTowerStatus(i);
	}
}

void HelperCastleTowerAddMsg(CNetMsg::SP& msg, int zoneidx, CDratanCastle * pCastle)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_CASTLE_TOWER_ADD
				<< zoneidx;
	for(int i=0; i<7; i++)
	{
		RefMsg(msg) << pCastle->GetTowerStatus(i);
	}
}

void CastleTowerAddMsg(CNetMsg::SP& msg, CDratanCastle * pCastle)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASTLE_WAR
				<< (unsigned char)MSG_CASTLE_TOWER_CONTRAL;
}

void CastleTowerReinforceListMsg(CNetMsg::SP& msg, char type, CDratanCastle * pCastle)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASTLE_WAR
				<< (unsigned char)MSG_CASTLE_TOWER_REINFORCE_LIST
				<< type
				<< pCastle->GetReinforceStep((int)type);
}

void CastleTowerReinforceMsg(CNetMsg::SP& msg, char type, CDratanCastle * pCastle)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASTLE_WAR
				<< (unsigned char)MSG_CASTLE_TOWER_REINFORCE
				<< type
				<< pCastle->GetReinforceStep((int)type);
}

void HelperCastleReinforceMsg(CNetMsg::SP& msg, char type, char step, int zoneidx)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_CASTLE_TOWER_REINFORCE
				<< zoneidx
				<< type
				<< step;
}

void CastleTowerRepaireListMsg(CNetMsg::SP& msg, int idx, LONGLONG money)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASTLE_WAR
				<< (unsigned char)MSG_CASTLE_TOWER_REPAIRE_LIST
				<< idx
				<< money;
}

void CastleTowerRepaireMsg(CNetMsg::SP& msg, int idx, LONGLONG money)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASTLE_WAR
				<< (unsigned char)MSG_CASTLE_TOWER_REPAIRE
				<< idx
				<< money;
}

void CastleTowerWarpListMsg(CNetMsg::SP& msg, int * aliveindex, char total)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASTLE_WAR
				<< (unsigned char)MSG_CASTLE_TOWER_WARP_LIST
				<< total;
	for(int i = 0; i < total; i++)
	{
		RefMsg(msg) << aliveindex[i];
	}
}

void CastleTowerRebrithInstallMsg(CNetMsg::SP& msg, int qindex, int gindex, const char * gname, int list_index)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASTLE_WAR
				<< (unsigned char)MSG_CASTLE_QUARTERS_INSTALL
				<< qindex
				<< gindex
				<< gname
				<< list_index;
}

void HeplerCastleRebrithInstallMsg(CNetMsg::SP& msg, int qindex, int gindex, const char * gname)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_CASTLE_QUARTERS_INSTALL
				<< qindex
				<< gindex
				<< gname;
}

void CastleTowerQuartersCrushMsg(CNetMsg::SP& msg, int index)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASTLE_WAR
				<< (unsigned char)MSG_CASTLE_QUARTERS_CRUSH
				<< index;
}

void CastletowerQuartersListMsg(CNetMsg::SP& msg, CDratanCastle * pCastle)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASTLE_WAR
				<< (unsigned char)MSG_CASTLE_QUARTERS_LIST;
	for(int i=0; i<7; i++)
	{
		if(pCastle->m_pRebrithNPC[i] == NULL || pCastle->m_pRebrithNPC[i]->m_proto == NULL)
		{
			RefMsg(msg) << -1 << -1 << "";
		}
		else
		{
			RefMsg(msg) << pCastle->m_pRebrithNPC[i]->m_proto->m_index
						<< pCastle->m_nRebrithGuild[i]
						<< pCastle->m_strRebrithGuild[i];
		}
	}
}

void WaitTimeMsg(CNetMsg::SP& msg, int wait_time)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASTLE_WAR
				<< (unsigned char)MSG_CASTLE_WAIT_TIME
				<< wait_time;
}

void CastleErrorMsg(CNetMsg::SP& msg, MSG_EX_CASTLE_ERROR_TYPE error)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASTLE_WAR
				<< (unsigned char)MSG_EX_CASTLE_ERROR
				<< (unsigned char)error;
}

void CastleErrorMsg(CNetMsg::SP& msg, unsigned char error)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASTLE_WAR
				<< (unsigned char)MSG_EX_CASTLE_ERROR
				<< (unsigned char)error;
}


void EventOpenAdultServerMsg(CNetMsg::SP& msg, MSG_EVENT_OPEN_ADULT_SERVER_TYPE type)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_OPEN_ADULT_SERVER
				<< type;
}

void HelperPetColorChange( CNetMsg::SP& msg, int charindex, int petindex, char petCorlorType )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_PET_COLOR_CHANGE
				<< charindex
				<< petindex
				<< petCorlorType;
}

void PetChangeColor( CNetMsg::SP& msg, int petindex, char petColorType, int ownerindex, char petMount )
{
	msg->Init( MSG_EXTEND );
	RefMsg(msg) << MSG_EX_PET_CHANGE_COLOR
				<< petindex
				<< petColorType
				<< ownerindex
				<< petMount;
}

#ifdef EVENT_DROPITEM
void MsgrEventDropItemReqMsg( CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, char type, int npcidx, int itemidx, int prob, int thisServer, int thisSub)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char) MSG_MSGR_EVENT_DROPITEM_REQ
				<< charindex
				<< type
				<< npcidx
				<< itemidx
				<< prob
				<< thisServer
				<< thisSub;
}

void MsgrEventDropItemRepMsg( CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, char type, int npcidx, int itemidx, int prob, int thisServer, int thisSub)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char) MSG_MSGR_EVENT_DROPITEM_REP
				<< charindex
				<< type
				<< npcidx
				<< itemidx
				<< prob
				<< thisServer
				<< thisSub;
}
#endif // EVENT_DROPITEM

void EventHalloween2007Msg( CNetMsg::SP& msg, MSG_EVENT_HALLOWEEN_2007_TYPE type, unsigned char error )
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_HALLOWEEN_2007
				<< (unsigned char)type
				<< error;
}

void EventHalloween2007Msg( CNetMsg::SP& msg, MSG_EVENT_HALLOWEEN_2007_TYPE type )
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_HALLOWEEN_2007
				<< (unsigned char)type;
}

void HelperHalloween2007Msg( CNetMsg::SP& msg, int char_index )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_HALLOWEEN_2007
				<< char_index;
}

void EventNewMoonStoneMsg( CNetMsg::SP& msg, MSG_EVENT_NEW_MOONSTONE_TYPE type)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_NEW_MOONSTONE
				<< (unsigned char)type;
}

void CastleOwnerInfoMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASTLE_WAR
				<< (unsigned char)MSG_CASTLE_OWNER_INFO;

	int index = 0;
	if( CWarCastle::GetCastleObject(ZONE_DRATAN) != 0 )
	{
		index = CWarCastle::GetCastleObject(ZONE_DRATAN)->GetOwnerGuildIndex();
	}
	RefMsg(msg) << index;

	if( CWarCastle::GetCastleObject(ZONE_MERAC) != 0 )
	{
		index = CWarCastle::GetCastleObject(ZONE_MERAC)->GetOwnerGuildIndex();
	}
	RefMsg(msg) << index;
}

void DVDInfoMsg(CPC * pPC, CNetMsg::SP& msg, int nMobRate, int nEnvRate )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_DVD
				<< (unsigned char)MSG_DUNGEON_INFO
				<< nMobRate
				<< nEnvRate ;
}
void DVDManagementMsg( CNetMsg::SP& msg, MSG_DVD_INFO_SUBTYPE subtype )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_DVD
				<< (unsigned char) MSG_MANAGEMENT
				<< (unsigned char) subtype;
}

void DVDDungeonEnterMsg( CNetMsg::SP& msg, MSG_DVD_INFO_SUBTYPE subtype )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_DVD
				<< (unsigned char) MSG_DUNGEON_ENTER
				<< (unsigned char) subtype;
}

void DVDDungeonNormalChangeNoticeMsg( CNetMsg::SP& msg )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_DVD
		<< (unsigned char) MSG_CHANGE_NORMAL_NOTICE;
}

void DVDDungeonChangeToOwnerMsg( CNetMsg::SP& msg )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_DVD
		<< (unsigned char) MSG_CHANGE_NORMAL_NOTICE_TO_OWNER;
}

void HelperDVDRateChangeMsg(CNetMsg::SP& msg, MSG_DVD_INFO_SUBTYPE subtype, int nRate, int mode)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_DVD_RATE_CHANGE
				<< (unsigned char) subtype
				<< nRate
				<< mode;
}

void HelperDVDNormalChangeNoticeMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_DVD_NORMAL_CHANGE_NOTICE;
}

void HelperDVDNormalChangeTimeMsg(CNetMsg::SP& msg, int time)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_DVD_NORMAL_CHANGE_NOTICE
		<< time;
}

void GuildNameColorStateMsg( CNetMsg::SP& msg , CPC* ch)
{
	unsigned char GuildNameColor = 0;
	if( ch->m_guildInfo != NULL )
	{
		if( CWarCastle::GetCastleObject(ZONE_MERAC) != 0 )
		{
			if( ch->m_guildInfo->guild()->index() == CWarCastle::GetCastleObject(ZONE_MERAC)->GetOwnerGuildIndex() )
			{
				GuildNameColor = 1;
			}
		}
		if( CWarCastle::GetCastleObject(ZONE_DRATAN) != 0 )
		{
			if( ch->m_guildInfo->guild()->index() == CWarCastle::GetCastleObject(ZONE_DRATAN)->GetOwnerGuildIndex() )
			{
				GuildNameColor = 2;
			}
		}
	}

	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_DVD
				<< (unsigned char) MSG_GUILD_NAME_COLOR
				<< ch->m_index
				<< GuildNameColor;
}

void ConnEventXmas2007Msg( CNetMsg::SP& msg, MSG_CONN_EVENT_XMAS_2007_TYPE subtype )
{
	msg->Init( MSG_CONN_REQ );
	RefMsg(msg) << (unsigned char) MSG_CONN_EVENT_XMAS_2007
				<< (unsigned char) subtype;
}

void EventXmas2007Msg(CNetMsg::SP& msg, MSG_EVENT_XMAS_2007_TYPE subtype )
{
	msg->Init( MSG_EVENT );
	RefMsg(msg) << (unsigned char)MSG_EVENT_XMAS_2007
				<< (unsigned char)subtype;
}

void EventXmasPuzzleMsg( CNetMsg::SP& msg, MSG_EVENT_XMAS_2007_TYPE subtype, int result)
{
	msg->Init( MSG_EVENT );
	RefMsg(msg) << (unsigned char)MSG_EVENT_XMAS_2007
				<< (unsigned char)subtype;

	if (result > -1)
		RefMsg(msg) << (unsigned char)result;
}
void EventXmasPuzzleRes( CNetMsg::SP& msg, unsigned char result, unsigned char winCount, unsigned char userChoice, unsigned char serverChoice)
{
	msg->Init( MSG_EVENT );
	RefMsg(msg) << (unsigned char)MSG_EVENT_XMAS_2007
				<< (unsigned char)MSG_EVENT_XMAS_PUZZLE_SELECT_REP
				<< (unsigned char)result
				<< (unsigned char)winCount
				<< (unsigned char)userChoice
				<< (unsigned char)serverChoice;
}

void EventXmasPuzzleEnd( CNetMsg::SP& msg, unsigned char winCount, int giftIndex, LONGLONG giftCount )
{
	msg->Init( MSG_EVENT );
	RefMsg(msg) << (unsigned char)MSG_EVENT_XMAS_2007
				<< (unsigned char)MSG_EVENT_XMAS_PUZZLE_END_REP
				<< (unsigned char)winCount
				<< (int)giftIndex
				<< (LONGLONG)giftCount;
}

void HelperPetTurnToNPCMsg( CNetMsg::SP& msg, int charindex, int petindex, int toNpc, int size )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_PET_TURNTO_NPC
				<< charindex
				<< petindex
				<< toNpc
				<< size;
}

void PetTurnToNPCMsg( CNetMsg::SP& msg, int petindex, int toNpc, int ownerindex, int size )
{
	msg->Init( MSG_EXTEND );
	RefMsg(msg) << MSG_EX_PET_TURNTO_NPC
				<< petindex
				<< toNpc
				<< ownerindex
				<< size;
}

#ifdef GM_EXP_LIMIT_AUTO
void MsgrSetExpLimitMsg( CNetMsg::SP& msg, int server,  int charindex, int nLimitStart, int nLimitEnd, int start[], int end[] )
#else
void MsgrSetExpLimitMsg( CNetMsg::SP& msg, int server,  int charindex, int nLimit )
#endif // GM_EXP_LIMIT_AUTO
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << 0
				<< server
				<< -1
				<< -1
				<< (unsigned char) MSG_MSGR_SET_EXPLIMIT
				<< charindex;

#ifdef GM_EXP_LIMIT_AUTO
	RefMsg(msg) << nLimitStart
				<< nLimitEnd;
	int i;
	for(i = 0; i < 6; i++)
	{
		RefMsg(msg) << start[i];
	}
	for(i = 0; i < 6; i++)
	{
		RefMsg(msg) << end[i];
	}
#else // GM_EXP_LIMIT_AUTO
	RefMsg(msg) << nLimit;
#endif // GM_EXP_LIMIT_AUTO
}

void HelperComboGotoWaitRoomPrompt(CNetMsg::SP& msg, int bossindex, const char* bossname)
{
	msg->Init(MSG_HELPER_REQ);
	RefMsg(msg) << -1
				<< gserver->m_serverno
				<< gserver->m_subno
				<< (int)-1
				<< (unsigned char)MSG_HELPER_COMBO_GOTO_COMBO_PROMPT
				<< bossindex
				<< bossname;
}

void HelperRecallToWaitRoomPrompt(CNetMsg::SP& msg, int bossindex)
{
	msg->Init(MSG_HELPER_REQ);
	RefMsg(msg) << -1
				<< gserver->m_serverno
				<< gserver->m_subno
				<< (int)-1
				<< (unsigned char)MSG_HELPER_COMBO_RECALL_TO_COMBO_PROMPT
				<< bossindex;
}

void HelperCreateComboMsg(CNetMsg::SP& msg, int subserver, int charindex, int areaindex, int comboindex)
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_CREATE_COMBO
				<< subserver
				<< charindex
				<< areaindex
				<< comboindex;
}

#if defined (SAKURA_EVENT_2008_REWARD)
void EventSakuraEvent2008Msg( CNetMsg::SP& msg, MSG_EVNET_SAKURA_2008_TYPE type )
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_SAKURA_2008
				<< (unsigned char)type;
}
#endif //defined (SAKURA_EVENT_2008_REWARD)

#ifdef UPGRADE_EVENT_AUTO
void MsgrUpgradeEventReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, char cmd, int prob, long tStart, long tEnd)
#else // UPGRADE_EVENT_AUTO
void MsgrUpgradeEventReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, char cmd, int prob)
#endif // UPGRADE_EVENT_AUTO
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char) MSG_MSGR_UPGRADE_EVENT_REQ
				<< charindex
				<< cmd
				<< prob;
#ifdef UPGRADE_EVENT_AUTO
	RefMsg(msg) << tStart
				<< tEnd;
#endif // UPGRADE_EVENT_AUTO
}
#ifdef UPGRADE_EVENT_AUTO
void MsgrUpgradeEventRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, char cmd, long start, long end)
#else
void MsgrUpgradeEventRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, char cmd)
#endif
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char) MSG_MSGR_UPGRADE_EVENT_REP
				<< charindex
				<< gserver->m_serverno
				<< gserver->m_subno
				<< cmd
				<< g_nUpgradeProb;
#ifdef UPGRADE_EVENT_AUTO
	RefMsg(msg) << start
				<< end;
#endif
}

#ifdef CHAOSBALL
void ChaosBallMsg(CNetMsg::SP& msg, MSG_EX_CHAOSBALL_ERRORTYPE error)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CHAOSBALL
				<< (unsigned char)error;
}
#endif

void HelperAttackPetMsg( CNetMsg::SP& msg , unsigned char subtype, int ownerIndex )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_APET
				<< subtype
				<< ownerIndex;
}

void HelperEventPhoenixMsg(CNetMsg::SP& msg, int nUserIdx, int nCharIdx  )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EVENT_PHOENIX
				<< nUserIdx
				<< nCharIdx;
}

void EventPhoenixMsg(CNetMsg::SP& msg, MSG_EVENT_PHOENIX_ERROR_TYPE eError)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char) MSG_EVENT_PHOENIX
				<< (unsigned char) eError;
}

void EventPhoenix_SuccessMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char) MSG_EVENT_PHOENIX_SUCCESS;
}

#ifdef EXTREME_CUBE
void HelperCubeStateReqMsg( CNetMsg::SP& msg, int charindex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_CUBESTATE_REQ
				<< charindex;
}

void HelperCubeStatePersonalReqMsg( CNetMsg::SP& msg, int charindex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_CUBESTATE_PERSONAL_REQ
				<< charindex;
}

void HelperAddCubePointMsg(CNetMsg::SP& msg, int ofguildindex, int ofcubepoint, long lastcubepoint)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_ADDCUBEPOINT
				<< ofguildindex
				<< ofcubepoint
				<< lastcubepoint;
}

void HelperAddCubePointPersonalMsg(CNetMsg::SP& msg, int ofcharidx ,int ofcubepoint, long lastCubePoint)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_ADDCUBEPOINT_PERSONAL
				<< ofcharidx
				<< ofcubepoint
				<< lastCubePoint;
}

void HelperGuildCubeNoticeMsg(CNetMsg::SP& msg, int type)
{
	msg->Init(MSG_HELPER_REQ);
	RefMsg(msg) << -1
				<< gserver->m_serverno
				<< -1
				<< -1
				<< (unsigned char) MSG_HELPER_GUILDCUBETIME_NOTICE
				<< type;
}

void ExtremeCubeErrorMsg(CNetMsg::SP& msg, MSG_EX_EXTREME_CUBE_ERROR_TYPE errortype)
{
	msg->Init( MSG_EXTEND );
	RefMsg(msg) << MSG_EX_EXTREME_CUBE
				<< (unsigned char)MSG_EX_EXTREME_CUBE_ERROR
				<< (unsigned char)errortype;
}

void ExtremeCubeMsg(CNetMsg::SP& msg, MSG_EX_EXTREME_CUBE_TYPE type)
{
	msg->Init( MSG_EXTEND );
	RefMsg(msg) << MSG_EX_EXTREME_CUBE
				<< (unsigned char ) type;
}
#ifdef EXTREME_CUBE_VER2
void HelperCubeRewardPersonalRollBackMsg(CNetMsg::SP& msg, int updatetime, int charindex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_CUBEREWARD_PERSONAL_ROLLBACK
				<< updatetime
				<< charindex;
}

void HelperCubeRewardPersonalReqMsg( CNetMsg::SP& msg, int charindex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_CUBEREWARD_PERSONAL_REQ
				<< charindex;
}
#endif // EXTREME_CUBE_VER2
#endif // EXTREME_CUBE

void InitSSkillMsg(CNetMsg::SP& msg, MSG_EX_INIT_SSkill_ERROR_TYPE type)
{
	msg->Init( MSG_EXTEND );
	RefMsg(msg) << MSG_EX_INIT_SSKILL
				<< (char)type;
}

// 정산 리스트 정보
void TradeAgentCalcListRepMsg(CNetMsg::SP& msg,int nCharIndex, int nMaxPageNo, int nPageNo, int nItemCount,int* pItemSerial,int* pItemPlus,int* pItemFlag,S_ITEMOPTION* pItemOption,int* pQuantity,LONGLONG* ptotalmoney,int* pState, int* pPassDay,CLCString *pCharname, int* pItemOrigin0, int* pItemOrigin1, int* pItemOrigin2, int* pItemOrigin3, int* pItemOrigin4, int* pItemOrigin5)
{
	msg->Init( MSG_TRADEAGENT );
	RefMsg(msg) << MSG_TRADEAGENT_CALCLIST_REP
				<< nCharIndex
				<< nMaxPageNo
				<< nPageNo
				<< nItemCount;

	for( int i = 0; i < nItemCount; i++ )
	{
		int nItemPlus = 0;

		int nItemPlus2 = 0;
		CItemProto* pItem = gserver->m_itemProtoList.FindIndex(pItemSerial[i]);
		if( pItem && pItem->getItemTypeIdx() == ITYPE_ACCESSORY && ( pItem->getItemSubTypeIdx() == IACCESSORY_PET || pItem->getItemSubTypeIdx() == IACCESSORY_ATTACK_PET)  )
		{
			nItemPlus = pItemPlus[i];
			nItemPlus2 = 0;
		}
		else
		{
			nItemPlus = (pItemPlus[i] & 0x0000ffff);
			nItemPlus2 = ((pItemPlus[i] >> 16) & 0x0000ffff);
		}

		RefMsg(msg) << pItemSerial[i]
					<< nItemPlus
					<< pItemFlag[i];

		if (pItem && pItem->IsOriginItem())
		{
			RefMsg(msg) << pItem->GetOriginOptionCnt();
			for (int l = 0; l < MAX_ORIGIN_OPTION; l++)
			{
				if (pItem->GetOriginIndex(l) > -1)
				{
					RefMsg(msg) << (char)pItem->GetOriginIndex(l)
								<< (int)pItem->GetOriginLevel(l);
					switch(l)
					{
					case 0:
						if (pItemOrigin0[i] == 0) pItemOrigin0[i] = 100;
						RefMsg(msg) << pItemOrigin0[i];
						break;
					case 1:
						if (pItemOrigin1[i] == 0) pItemOrigin1[i] = 100;
						RefMsg(msg) << pItemOrigin1[i];
						break;
					case 2:
						if (pItemOrigin2[i] == 0) pItemOrigin2[i] = 100;
						RefMsg(msg) << pItemOrigin2[i];
						break;
					case 3:
						if (pItemOrigin3[i] == 0) pItemOrigin3[i] = 100;
						RefMsg(msg) << pItemOrigin3[i];
						break;
					case 4:
						if (pItemOrigin4[i] == 0) pItemOrigin4[i] = 100;
						RefMsg(msg) << pItemOrigin4[i];
						break;
					case 5:
						if (pItemOrigin5[i] == 0) pItemOrigin5[i] = 100;
						RefMsg(msg) << pItemOrigin5[i];
						break;
					default:
						RefMsg(msg) << 0;
						break;
					}
				}
			}
		}
		else
		{
			RefMsg(msg) << pItemOption[i].m_nOptionCnt;
			for(int j=0; j < pItemOption[i].m_nOptionCnt; j++)
			{
				RefMsg(msg) << (char)pItemOption[i].m_Option_type[j]
							<< (int)pItemOption[i].m_Option_level[j];
			}
		}
		RefMsg(msg) << pQuantity[i]
					<< ptotalmoney[i]
					<< pState[i]
					<< pPassDay[i]
					<< pCharname[i];

		if (pItem && pItem->IsOriginItem())
		{
			RefMsg(msg) << (char)pItem->GetBelongType()
						<< (char)pItem->GetOriginSkillCnt();
			for (int l = MAX_ORIGIN_OPTION; l < MAX_ORIGIN; l++)
			{
				if (pItem->GetOriginIndex(l) > -1)
				{
					RefMsg(msg) << pItem->GetOriginIndex(l)
								<< (char)pItem->GetOriginLevel(l);
				}
			}
		}
		RefMsg(msg) << nItemPlus2;
	}
}

//정산
void TradeAgentCalculateRepMsg(CNetMsg::SP& msg,int nCharIndex)
{
	msg->Init( MSG_TRADEAGENT );
	RefMsg(msg) << MSG_TRADEAGENT_CALCULATE_REP
				<< nCharIndex;
}

//체크 정산
void TradeAgentCheckCalcRepMsg(CNetMsg::SP& msg,int nCharIndex,int nIsBeCalcdata)
{
	msg->Init( MSG_TRADEAGENT );
	RefMsg(msg) << MSG_TRADEAGENT_CHECKCALC_REP
				<< nCharIndex
				<< nIsBeCalcdata;
}

//정산 알림
void TradeAgentNeedCalcMotifyMsg(CNetMsg::SP& msg,int nCharIndex,unsigned char ucState,CLCString stItemname,int nItemCount)
{
	msg->Init( MSG_TRADEAGENT );
	RefMsg(msg) << MSG_TRADEAGENT_NEEDCALC_NOTIFY
				<< nCharIndex
				<< ucState
				<< stItemname
				<< nItemCount;
}

#ifdef DISCONNECT_HACK_CHARACTER  // 핵 프로그램 유저 방출
void HelperDisconnectHackCharacter(CNetMsg::SP& msg, int nUserIndex, const char* user_id, int nCharIndex, const char* char_id, int nHack_type, float delay)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_DISCONNECT_HACK_CHARACTER
				<< nUserIndex
				<< user_id
				<< nCharIndex
				<< char_id
				<< nHack_type
				<< delay;
}
#endif // DISCONNECT_HACK_CHARACTER

//에러
void ExpedErrorMsg(CNetMsg::SP& msg, unsigned char errorType)
{
	msg->Init(MSG_EXPEDITION);
	RefMsg(msg) << MSG_EXPED_ERROR
				<< (unsigned char)errorType;
}

void HelperExpedCreateReqMsg(CNetMsg::SP& msg, int nBossIndex,CLCString BossName)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EXPED_CREATE_REQ
				<< nBossIndex
				<< BossName;
}

void HelperExpedCreateResultMsg(CNetMsg::SP& msg, int nResult, int nBossIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EXPED_CREATE_RESULT
				<< nResult
				<< nBossIndex;
}

void HelperExpedInviteReqMsg(CNetMsg::SP& msg, int nBossIndex, CLCString strBossName, int nTargetIndex,CLCString strTargetName)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EXPED_INVITE_REQ
				<< nBossIndex
				<< strBossName
				<< nTargetIndex
				<< strTargetName;
}

void HelperExpedAllowReqMsg(CNetMsg::SP& msg, int nBossIndex, int nAllowIndex, CLCString strAllowCharName, int nAllowLevel )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EXPED_ALLOW_REQ
				<< nBossIndex
				<< nAllowIndex
				<< strAllowCharName;
	RefMsg(msg) << nAllowLevel;
}

void HelperExpedRejectReqMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EXPED_REJECT_REQ
				<< nBossIndex
				<< nTargetIndex;
}

void HelperExpedQuitReqMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex, int nQuitMode)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EXPED_QUIT_REQ
				<< nBossIndex
				<< nTargetIndex
				<< nQuitMode;
}

void HelperExpedKickReqMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EXPED_KICK_REQ
				<< nBossIndex
				<< nTargetIndex;
}

void HelperExpedChangeBossReqMsg(CNetMsg::SP& msg, int nBossIndex, int nChangeMode, int nNewBossIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EXPED_CHANGEBOSS_REQ
				<< nBossIndex
				<< nChangeMode
				<< nNewBossIndex;
}

void HelperExpedChangeTypeReqMsg(CNetMsg::SP& msg, int nBossIndex, char cExpedType,char cDiviType)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EXPED_CHANGETYPE_REQ
				<< nBossIndex
				<< cExpedType
				<< cDiviType;
}

void HelperExpedEndExpedMsg(CNetMsg::SP& msg, int nBossIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EXPED_ENDEXPED_REQ
				<< nBossIndex;
}

void HelperExpedSetMBossReqMsg(CNetMsg::SP& msg, int nBossIndex, int nNewMBossIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EXPED_SETMBOSS_REQ
				<< nBossIndex
				<< nNewMBossIndex;
}

void HelperExpedResetMBossReqMsg(CNetMsg::SP& msg, int nBossIndex, int nNewMBossIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EXPED_RESETMBOSS_REQ
				<< nBossIndex
				<< nNewMBossIndex;
}

void HelperExpedMoveGroupReqMsg(CNetMsg::SP& msg, int nBossIndex, int nSourceGroup, int nMoveCharIndex, int nTargetGroup, int nTargetListindex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EXPED_MOVEGROUP_REQ
				<< nBossIndex
				<< nSourceGroup
				<< nMoveCharIndex
				<< nTargetGroup
				<< nTargetListindex;
}

void HelperExpendOfflineMsg( CNetMsg::SP& msg, int nBossIndex, int nTargetIndex )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EXPEND_OFFLINE
				<< nBossIndex
				<< nTargetIndex;
}

void HelperExpendOnlineMsg( CNetMsg::SP& msg, int nBossIndex, int nTargetIndex, int nTargetLevel )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EXPEND_ONLINE
				<< nBossIndex
				<< nTargetIndex
				<< nTargetLevel;
}


//파티 전환 응답
void ExpedCreateRepMsg(CNetMsg::SP& msg, char cExpedType1, char cExpedType2, char cExpedType3, int nBossIndex,CLCString BossCharName,int nMemberCount,int *pCharIdex,CLCString* pCharName,int* pGroupType,int* pMemberType,int* pSetLabelType,int* pQuitType)
{
	msg->Init(MSG_EXPEDITION);
	RefMsg(msg) << MSG_CREATE_REP
				<< cExpedType1
				<< cExpedType2
				<< cExpedType3
				<< nBossIndex
				<< BossCharName
				<< nMemberCount;

	for(int i=0; i < nMemberCount; i++)
	{
		RefMsg(msg) << pCharIdex[i]
					<< pCharName[i]
					<< pGroupType[i]
					<< pMemberType[i];
	}
}

//초대 전달
void ExpedInviteRepMsg(CNetMsg::SP& msg, char cExpedType1, char cExpedType2, char cExpedType3, int nBossIndex, CLCString strBossName)
{
	msg->Init(MSG_EXPEDITION);
	RefMsg(msg) << MSG_INVITE_REP
				<< cExpedType1
				<< cExpedType2
				<< cExpedType3
				<< nBossIndex
				<< strBossName;
}

void ExpedAddMsg(CNetMsg::SP& msg, int nCharIndex, const char* strCharName,int nCharGroup,int nCharMember, int nCharListIndex, CPC* tch)
{
	msg->Init(MSG_EXPEDITION);
	RefMsg(msg) << MSG_EXPED_ADD
				<< nCharIndex
				<< strCharName
				<< nCharGroup
				<< nCharMember
				<< nCharListIndex;

	if (tch)
	{
		RefMsg(msg) << tch->m_job
					<< tch->m_job2
					<< tch->m_level
					<< tch->m_hp << tch->m_maxHP
					<< tch->m_mp << tch->m_maxMP
					<< GET_X(tch)
					<< GET_Z(tch)
					<< GET_YLAYER(tch)
					<< tch->m_pZone->m_index;
	}
	else
	{
		RefMsg(msg) << (char)0
					<< (char)0
					<< (int)0				//level
					<< (int)1 << (int)1
					<< (int)1 << (int)1
					<< (int)0
					<< (int)0
					<< (int)0
					<< (int)0;
	}
}

void ExpedInfoMsg(CNetMsg::SP& msg, CPC* ch, int nGroup, bool isOnline)
{
	UpdateClient::expedInfoForChar* packet = reinterpret_cast<UpdateClient::expedInfoForChar*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_EXPED_INFO;

	packet->charIndex = ch->m_index;
	packet->group = nGroup;
	packet->level = ch->m_level;
	packet->hp = ch->m_hp;
	packet->maxHp = ch->m_maxHP;
	packet->mp = ch->m_mp;
	packet->maxMp = ch->m_maxMP;
	packet->x = GET_X(ch);
	packet->z = GET_Z(ch);
	packet->yLayer = GET_YLAYER(ch);
	packet->zoneIndex = ch->m_pZone->m_index;
	packet->isOnline = isOnline;
	msg->setSize(sizeof(UpdateClient::expedInfoForChar));
}

void ExpedQuitRepMsg(CNetMsg::SP& msg, int nTargetIndex,int nQuitMode)
{
	msg->Init(MSG_EXPEDITION);
	RefMsg(msg) << MSG_QUIT_REP
				<< nTargetIndex
				<< nQuitMode;		// 정상,비정상 구분
}
void ExpedQuitReqMsg(CNetMsg::SP& msg, int nQuitMode)
{
	msg->Init(MSG_EXPEDITION);
	RefMsg(msg) << (unsigned char) MSG_QUIT_REQ
				<< nQuitMode;		// 정상,비정상 구분
}

void ExpendOfflineMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_EXPEDITION);
	RefMsg(msg) << (unsigned char) MSG_EXPEND_OFFLINE;
}

void ExpedKickMsg(CNetMsg::SP& msg, int nTargetIndex)
{
	msg->Init(MSG_EXPEDITION);
	RefMsg(msg) << MSG_KICK_REP;
	RefMsg(msg) << nTargetIndex;
}

void ExpedMsg(CNetMsg::SP& msg, int nSubtype)
{
	msg->Init(MSG_EXPEDITION);
	RefMsg(msg) << nSubtype;
}

void ExpedRejectReqMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_EXPEDITION);
	RefMsg(msg) << (unsigned char) MSG_REJECT_REQ;
}

//해체 시작
void ExpedEndExpedStartMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_EXPEDITION);
	RefMsg(msg) << MSG_ENDEXPED_START;
}

void ExpedChangeBoss(CNetMsg::SP& msg, int nBossIndex,  int nNewBossIndex, int nChangeMode)
{
	msg->Init(MSG_EXPEDITION);
	RefMsg(msg) << MSG_CHANGEBOSS_REP
				<< nBossIndex
				<< nNewBossIndex
				<< nChangeMode;			// 수동,자동
}

void ExpedTypeinfo(CNetMsg::SP& msg, char cType1, char cType2, char cType3)
{
	msg->Init(MSG_EXPEDITION);
	RefMsg(msg) << MSG_EXPED_TYPEINFO
				<< cType1
				<< cType2
				<< cType3;
}

void ExpedChangeType(CNetMsg::SP& msg, char cExpedType, char cDiviType)
{
	msg->Init(MSG_EXPEDITION);
	RefMsg(msg) << MSG_CHANGETYPE_REP
				<< cExpedType
				<< cDiviType;
}

void ExpedSetMbossMsg(CNetMsg::SP& msg, int nNewMBossIndex)
{
	msg->Init(MSG_EXPEDITION);
	RefMsg(msg) << MSG_SETMBOSS_REP
				<< nNewMBossIndex;
}

void ExpedResetMbossMsg(CNetMsg::SP& msg, int nNewMBossIndex)
{
	msg->Init(MSG_EXPEDITION);
	RefMsg(msg) << MSG_RESETMBOSS_REP
				<< nNewMBossIndex;
}

void ExpedMoveGroupRepMsg(CNetMsg::SP& msg,int nSourceGroup, int nMoveCharIndex, int nTargetGroup, int nTargetListindex)
{
	msg->Init(MSG_EXPEDITION);
	RefMsg(msg) << MSG_MOVEGROUP_REP
				<< nSourceGroup
				<< nMoveCharIndex
				<< nTargetGroup
				<< nTargetListindex;
}

void ExpedSetLabelRepMsg(CNetMsg::SP& msg,int nType, int nMode, int nLabel,int nDestIndex)
{
	msg->Init(MSG_EXPEDITION);
	RefMsg(msg) << MSG_SET_LABEL_REP
				<< nType
				<< nMode
				<< nLabel
				<< nDestIndex;
}

bool ExpedViewDail_InvenMsg(CNetMsg::SP& msg, CPC* ch)
{
	wearItem_search_t vec;
	ch->m_wearInventory.searchWearItem(vec);

	msg->Init(MSG_EXPEDITION);
	RefMsg(msg) << MSG_VIEWDETAIL_INVEN
				<< (int)vec.size();

	wearItem_search_t::iterator it = vec.begin();
	wearItem_search_t::iterator endit = vec.end();
	for (; it != endit; ++it)
	{
		wearItem_search_pair_t& p = (*it);
		CItem* item = p.pItem;

		RefMsg(msg) << (int)item->getVIndex()
					<< (int)item->getDBIndex()
					<< item->getWearPos()
					<< (int)item->getPlus()
					<< (int)item->getFlag()
					<< (int)item->getUsed()
					<< (int)item->getUsed_2()
					<< (LONGLONG)item->Count();			// count

		ItemPublicMsg(msg, item);
	}

	return true;
}

void ExpedViewDail_ExPetMsg(CNetMsg::SP& msg, CPet* pet)
{
	int nRemainGetRebirth = pet->GetRemainRebirthTime();

	msg->Init(MSG_EXPEDITION);
	RefMsg(msg) << MSG_VIEWDETAIL_PET
				<< pet->m_index
				<< pet->GetPetTypeGrade()
				<< pet->m_level
				<< pet->m_exp
				<< pet->GetNeedExp()
				<< pet->m_hp
				<< pet->m_maxHP
				<< pet->GetAbilityPoint()
				<< pet->GetHungryPoint()
				<< (int)PET_MAX_HUNGRY
				<< pet->GetSympathyPoint()
				<< (int)PET_MAX_SYMPATHY
				<< nRemainGetRebirth
				<< pet->m_name;
}

void ExpedViewDail_ExAPetMsg(CNetMsg::SP& msg, CAPet* pet)
{
	int	nEqAttack = 0;
	if( pet->m_wearing[APET_WEAR_WEAPON] )
	{
		if( pet->m_wearing[APET_WEAR_WEAPON]->IsRangeWeapon() )
		{
			nEqAttack = pet->m_eqRange + pet->m_opRange;
		}
		else
		{
			nEqAttack = pet->m_eqMelee + pet->m_opMelee;
		}
	}

	msg->Init(MSG_EXPEDITION);
	RefMsg(msg) << MSG_VIEWDETAIL_APET
				<< (unsigned char) MSG_SUB_STAT
				<< pet->m_index
				<< pet->m_name
				<< pet->m_level
				<< pet->m_str + pet->m_nPlusStr + pet->m_opStr
				<< pet->m_con + pet->m_nPlusCon + pet->m_opCon
				<< pet->m_dex + pet->m_nPlusDex + pet->m_opDex
				<< pet->m_int + pet->m_nPlusInt + pet->m_opInt;
}

void ExpedSearchTriggerItemMsg(CNetMsg::SP& msg, int nSubtype)
{
	msg->Init(MSG_EXPEDITION);
	RefMsg(msg) << nSubtype;
}

void ExpedAddSysMsg(CNetMsg::SP& msg, const char* strCharName)
{
	msg->Init(MSG_EXPEDITION);
	RefMsg(msg) << MSG_EXPED_ADD_SYSMSG
				<< strCharName;
}

void FactoryListMsg(CNetMsg::SP& msg, CPC* ch, int nSkillIdx)
{
	if (!ch || nSkillIdx < 0)
		return;

	CNetMsg::SP tmpMsg(new CNetMsg);
	int nCount = 0;
	CFactoryList::set_t& list = ch->m_listFactory.getList();
	CFactoryList::set_t::iterator it = list.begin();
	CFactoryList::set_t::iterator endit = list.end();
	for(; it != endit; ++it)
	{
		CFactoryProto* p = *(it);

		if (p->GetSealType() == nSkillIdx)
		{
			RefMsg(tmpMsg) << p->GetIndex();
			nCount++;
		}
	}
	RefMsg(msg) << nCount << tmpMsg;
}

void FactoryLearnMsg(CNetMsg::SP& msg, CFactoryProto * pFactory)
{
	if (!pFactory)
		return;

	msg->Init(MSG_FACTORY);
	RefMsg(msg) << (unsigned char)MSG_FACTORY_LEARN
				<< pFactory->GetIndex();
}

void FactoryErrorMsg(CNetMsg::SP& msg, MSG_FACTORY_ERROR_TYPE errorcode)
{
	msg->Init(MSG_FACTORY);
	RefMsg(msg) << (unsigned char)MSG_FACTORY_ERROR
				<< (unsigned char)errorcode;
}

void FactoryMakeMsg(CNetMsg::SP& msg, CPC* ch, CFactoryProto * pFactory)
{
	if (!ch || !pFactory)
		return;

	msg->Init(MSG_FACTORY);
	RefMsg(msg) << (unsigned char)MSG_FACTORY_ITEM_MAKE
				<< pFactory->GetIndex()
				<< pFactory->GetSealType()
				<< ch->GetSealExp(pFactory->GetSealType());
}

void TriggerEvnetMsg(CNetMsg::SP& msg, MSG_EX_TRIGGER_EVENT_TYPE type, int touch_ID, int Play_ID, int bContinue)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_TRIGGER_EVENT
				<< (unsigned char) type
				<< touch_ID
				<< Play_ID
				<< bContinue;
}

void HelperTriggerEvnetMsg(CNetMsg::SP& msg, int nZoneIdx, int nAreaIdx, int nSaveInfo)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_TRIGGER_EVENT
				<< nZoneIdx
				<< nAreaIdx
				<< nSaveInfo;
}

void HelperDeleteRaidCharacterMsg(CNetMsg::SP& msg, int nCharIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_DELETE_RAID_CHAR
				<< nCharIndex;
}

void HelperRaidInfoMsg(CNetMsg::SP& msg, int nCharIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_RAID_INFO
				<< nCharIndex;
}

void RaidInfoMsg(CNetMsg::SP& msg, int nRaidCount, int* nZoneNum, int* nRoomNum)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_RAID_INFO
				<< nRaidCount;

	// 지정된 원정대 레이드의 개수에 따라 정보가 추가 됨.
	for (int i=0; i < nRaidCount; i++)
	{
		RefMsg(msg) << nZoneNum[i]
					<< nRoomNum[i];
	}
}

//에러
void RaidErrorMsg(CNetMsg::SP& msg, unsigned char errorType,int nErrData1, int nErrData2)
{
	msg->Init(MSG_RAID);
	RefMsg(msg) << MSG_RAID_ERROR
				<< (unsigned char)errorType
				<< nErrData1
				<< nErrData2;
}

//게임 서버 내부 사용
void RaidInzoneQuitReq(CNetMsg::SP& msg, int nGoZone,int nRebirth)
{
	msg->Init(MSG_RAID);
	RefMsg(msg) << (unsigned char)MSG_RAID_INZONE_QUIT
				<< nGoZone
				<< nRebirth;
}

void HelperRaidInzoneGetRoomNoReqMsg(CNetMsg::SP& msg, int nCharindex, int nZoneNo, int nBossIndex,int nJoinType)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_INZONE_GET_ROOMNO_REQ
				<< nCharindex
				<< nZoneNo
				<< nBossIndex
				<< nJoinType;
}

void HelperRaidInzoneGameDataClearReqMsg(CNetMsg::SP& msg, int nZone)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_INZONE_GAMEDATA_CLEAR_REQ
				<< nZone;
}

void HelperRaidInzoneSetNo(CNetMsg::SP& msg, int nNewBosIndex, int nOldBossIndex, int nZoneNum, int nJoinType, int difficulty )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_INZONE_SET_ROOMNO
				<< nNewBosIndex
				<< nOldBossIndex
				<< nZoneNum
				<< nJoinType;
	RefMsg(msg) << difficulty;
}


void AffinityRewardNoticeMsg(CNetMsg::SP& msg, int noticeCount, int* npcidx)
{
	msg->Init( MSG_EXTEND );
	RefMsg(msg) << MSG_EX_AFFINITY
				<< (unsigned char)MSG_EX_AFFINITY_REWARD_NOTICE
				<< noticeCount;

	int i;
	for(i = 0 ; i < noticeCount ; ++i)
		RefMsg(msg) << npcidx[i];
}

void AffinityListInfoMsg(CNetMsg::SP& msg, CPC* ch)
{
	map_affinitylist_t* list = ch->m_affinityList.GetList();

	int count = list->size();
	msg->Init( MSG_EXTEND );
	RefMsg(msg) << MSG_EX_AFFINITY
				<< (unsigned char)MSG_EX_AFFINITY_LISTINFO
				<< count;

	map_affinitylist_t::iterator it = list->begin();
	map_affinitylist_t::iterator endit = list->end();
	for( ; it != endit; ++it)
	{
		CAffinity* affinity = it->second;
		RefMsg(msg) << affinity->m_affinityidx
					<< affinity->m_point
					<< affinity->m_proto->m_maxvalue;
	}
}

void AffinityAddInfoMsg(CNetMsg::SP& msg, int affinityidx, int maxpoint)
{
	msg->Init( MSG_EXTEND );
	RefMsg(msg) << MSG_EX_AFFINITY
				<< (unsigned char)MSG_EX_AFFINITY_ADDINFO
				<< affinityidx
				<< maxpoint;
}

void AffinityUpdatePointMsg(CNetMsg::SP& msg, int affinityidx, int point, bool bBoostered, int bonus)
{
	msg->Init( MSG_EXTEND );
	RefMsg(msg) << MSG_EX_AFFINITY
				<< (unsigned char)MSG_EX_AFFINITY_UPDATE_POINT
				<< affinityidx
				<< point
				<< bonus;
	/*친화도 부스터 적용 표시를 위한 boolean값*/
	RefMsg(msg) << bBoostered;
}

void AffinityErrorMsg(CNetMsg::SP& msg, MSG_EX_AFFINITY_ERROR_TYPE errortype)
{
	msg->Init( MSG_EXTEND );
	RefMsg(msg) << MSG_EX_AFFINITY
				<< (unsigned char)MSG_EX_AFFINITY_ERROR
				<< (unsigned char)errortype;
}

void AffinityMsg(CNetMsg::SP& msg, MSG_EX_AFFINITY_TYPE type)
{
	msg->Init( MSG_EXTEND );
	RefMsg(msg) << MSG_EX_AFFINITY
				<< (unsigned char)type;
}


void SubHelperConnectMsg(CNetMsg::SP& msg, int version, int server, int subno, int count, int* zones)
{
	msg->Init(MSG_SUBHELPER_CONNECT);
	RefMsg(msg) << version
				<< server
				<< subno
				<< count;
	int i;
	for (i = 0; i < count; i++)
		RefMsg(msg) << zones[i];
}

void SubHelperShutdownMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << MSG_SUBHELPER_SHUTDOWN;
}

void NpcPortalLoadingEndUseMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_LOADING_END;
	RefMsg(msg) << MSG_EX_LODING_END_NPC_PORTAL_USE;
}

void NpcPortalListMsg(CNetMsg::SP& msg, int zoneNum)
{
	CZone* pZone = gserver->FindZone(zoneNum);

	if( pZone == NULL ||
			(pZone->m_area->m_NpcPortal_MobList.empty() &&
			 pZone->m_area->m_NpcPortal_NpcList.empty()) )
	{
		NpcPortalListErrorMsg( msg, MSG_NPC_PORTAL_ERROR_LIST);
		return;
	}
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_NPC_PORTAL_SCROLL;
	RefMsg(msg) << MSG_NPC_PORTAL_LIST;

	int nCount = pZone->m_area->m_NpcPortal_NpcList.size();
	RefMsg(msg) << nCount;

	if( nCount > 0 )
	{
		set_int_t::iterator it = pZone->m_area->m_NpcPortal_NpcList.begin();
		set_int_t::iterator endit = pZone->m_area->m_NpcPortal_NpcList.end();
		for(; it != endit; ++it)
		{
			RefMsg(msg) << (*it);
		}
	}

	nCount = pZone->m_area->m_NpcPortal_MobList.size();
	RefMsg(msg) << nCount;

	if( nCount > 0)
	{
		set_int_t::iterator it = pZone->m_area->m_NpcPortal_MobList.begin();
		set_int_t::iterator endit = pZone->m_area->m_NpcPortal_MobList.end();
		for(; it != endit; ++it)
		{
			RefMsg(msg) << (*it);
		}
	}
}

void NpcPortalListErrorMsg(CNetMsg::SP& msg, int ErrorType)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_NPC_PORTAL_SCROLL;
	RefMsg(msg) << MSG_NPC_PORTAL_ERROR
				<< ErrorType;
}


void NewTutorialMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_TUTORIAL;
}

void LuckyDrawBoxErrorMsg(CNetMsg::SP& msg, int ErrorType)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_LUCKYDRAW_BOX;
	RefMsg(msg) << ErrorType;
}

void HelperNSCreateCardUse(CNetMsg::SP& msg, MSG_HELPER_NS_CARD_TYPE subtype, int userindex, int charindex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_NS_CARD
				<< (unsigned char)subtype
				<< userindex
				<< charindex;
}

void DBNSCreateMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_DB);
	RefMsg(msg) << (unsigned char)MSG_DB_NSCREATE;
}

void PartyRecallErrorMsg(CNetMsg::SP& msg, MSG_EX_PARTY_RECALL_TYPE type)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << (long)MSG_EX_PARTY_RECALL;
	RefMsg(msg) << (long)type;
}

void TitleItemUseMsg(CNetMsg::SP& msg, int titleindex, int endtime)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_TITLE_SYSTEM
				<< MSG_EX_TITLE_SYSTEM_INSERT_SUCCESS
				<< titleindex
				<< endtime;
}

void TitleItemCantUseMsg(CNetMsg::SP& msg, int titleindex)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_TITLE_SYSTEM
				<< MSG_EX_TITLE_SYSTEM_ALREADY_HAVE_TITLE
				<< titleindex;
}
void SubHelperTitleSystemInsertReq(CNetMsg::SP& msg, int char_index, int title_index, int endtime, int tab, int dbIndex, CLCString serial)
{
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << MSG_SUBHELPER_TITLESYSTEM_INSERT_TITLE_REQ
				<< char_index
				<< title_index
				<< endtime
				<< tab
				<< dbIndex;
	RefMsg(msg) << serial;
}

void TitleSystemListRep(CNetMsg::SP& msg, MSG_EX_TITLE_SYSTEM_TYPE type, int count)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_TITLE_SYSTEM
				<< type;
	if (count != 0)
	{
		RefMsg(msg) << count;
	}
}

void SubHelperTitleSystemCheckTimeReq(CNetMsg::SP& msg, int char_index, CTitleNode* temp)
{
	int count = 0;
	CTitleNode* head;
	head = temp;
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << MSG_SUBHELPER_TITLESYSTEM_CHECK_EXPIRE_TITLE_REQ
				<< char_index;

	while(head)
	{
		if((head->m_title->m_endtime < gserver->getNowSecond()) && (head->m_title->m_endtime != -1))
			count++;
		head = head->m_next;
	}
	RefMsg(msg) << count;
	head = temp;
	while(head)
	{
		if((head->m_title->m_endtime < gserver->getNowSecond()) && (head->m_title->m_endtime != -1))
		{
			RefMsg(msg) << head->m_title->m_proto->m_index;
		}
		head = head->m_next;
	}
}

void TitleSystemMsg(CNetMsg::SP& msg, MSG_EX_TITLE_SYSTEM_TYPE type, int title_index, int char_index)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_TITLE_SYSTEM
				<< type
				<< title_index
				<< char_index;
}

void SubHelperTitleSystemDeleteReq(CNetMsg::SP& msg, int char_index, int title_index)
{
	// 사용자 수동삭제
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << MSG_SUBHELPER_TITLE_SYSTEM_TITLE_DELETE_REQ
				<< char_index
				<< title_index;
}
void SubHelperTitleSystemTitleDelReq(CNetMsg::SP& msg, int char_index, int title_index)
{
	// 기간만료로 인한 자동삭제
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << MSG_SUBHELPER_TITLE_SYSTEM_TITLE_AUTO_DELETE
				<< char_index
				<< title_index;
}

// [selo: 101105] 종료 시간 강제 변경
void TitleSysstemForceChangeTimeMsg(CNetMsg::SP& msg, MSG_EX_TITLE_SYSTEM_TYPE type, int title_index, int endtime)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_TITLE_SYSTEM
				<< type
				<< title_index
				<< endtime;
}

void TakeAgainQuestItemMsg(CNetMsg::SP& msg, MSG_EX_TAKE_AGAIN_QUEST_ITEM_TYPE type)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_TAKE_AGAIN_QUEST_ITEM
				<< type;
}

void HelperPartyDataInitReqMsg(CNetMsg::SP& msg)
{
	// 게임서버 다운 시 헬퍼파티 정보 초기화.
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_INIT_REQ;
}

#ifdef LACARETTE_SYSTEM

void ConnLacaretteTokenReadyMsg(CNetMsg::SP& msg, int userIndex, int charIndex)
{
	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char)MSG_CONN_LACARETTE_SYSTEM
				<< (unsigned char)MSG_CONN_LACARETTE_TOKEN_READY_REQ
				<< userIndex
				<< charIndex;
}

void ConnLacaretteTokenMsg(CNetMsg::SP& msg, int userIndex, int charIndex)
{
	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char)MSG_CONN_LACARETTE_SYSTEM
				<< (unsigned char)MSG_CONN_LACARETTE_TOKEN_REQ
				<< userIndex
				<< charIndex;
}

void ConnLacaretteTokenAddMag(CNetMsg::SP& msg, int userIndex, int charIndex, int bAdd)
{
	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char)MSG_CONN_LACARETTE_SYSTEM
				<< (unsigned char)MSG_CONN_LACARETTE_TOKEN_WRITE
				<< userIndex
				<< charIndex
				<< bAdd;
}

void ConnLacaretteRepeatMsg(CNetMsg::SP& msg, int userIndex, int charIndex, int cosArrNum, int cosIndex, int tokenIndex)
{
	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char)MSG_CONN_LACARETTE_SYSTEM
				<< (unsigned char)MSG_CONN_LACARETTE_RETTE_REQ
				<< userIndex
				<< charIndex
				<< cosArrNum
				<< cosIndex
				<< tokenIndex;
}
void ConnLacaretteRepeatAddMsg(CNetMsg::SP& msg, int userIndex, int charIndex, int giveItemIdx, int bAdd)
{
	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char)MSG_CONN_LACARETTE_SYSTEM
				<< (unsigned char)MSG_CONN_LACARETTE_RETTE_WRITE
				<< userIndex
				<< charIndex
				<< giveItemIdx
				<< bAdd;
}

void ConnLacaretteUIMsg(CNetMsg::SP& msg, int userIndex, int charIndex, int cosArrNum, int cosIndex, int tokenIndex)
{
	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char) MSG_CONN_LACARETTE_SYSTEM
				<< (unsigned char) MSG_CONN_LACARETTE_UI_REQ
				<< userIndex
				<< charIndex;
}

void lacaretteTokenReadyErrorMSg(CNetMsg::SP& msg, MSG_EX_LACARETTE_TOKEN_ERROR_TYPE type )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_LACARETTE_SYSTEM
				<< (unsigned char)MSG_EX_LACARETTE_TOKEN_READY_REP
				<< (unsigned char)type;
}

void lacaretteTokenErrorMSg(CNetMsg::SP& msg, MSG_EX_LACARETTE_TOKEN_ERROR_TYPE type )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_LACARETTE_SYSTEM
				<< (unsigned char)MSG_EX_LACARETTE_TOKEN_REP
				<< (unsigned char)type;
}

void lacaretteRetteErrorMSg(CNetMsg::SP& msg, MSG_EX_LACARETTE_RETTE_ERROR_TYPE type )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_LACARETTE_SYSTEM
				<< (unsigned char)MSG_EX_LACARETTE_RETTE_REP
				<< (unsigned char)type;
}
void lacaretteRetteErrorSucMSg(CNetMsg::SP& msg, int giveitemArrNum, int giveitemIndex)//, int courseArrNum, int courseIndex )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_LACARETTE_SYSTEM
				<< (unsigned char)MSG_EX_LACARETTE_RETTE_REP
				<< (unsigned char)MSG_EX_LACARETTE_RETTE_ERROR_SUC
				<< giveitemArrNum
				<< giveitemIndex;
	//<< courseArrNum
	//<< courseIndex;
}
void lacaretteResultErrorMSg(CNetMsg::SP& msg, MSG_EX_LACARETTE_RESULT_ERROR_TYPE type )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_LACARETTE_SYSTEM
				<< (unsigned char)MSG_EX_LACARETTE_RESULT_REP
				<< (unsigned char)type;
}
void lacaretteResultErrorSucMSg(CNetMsg::SP& msg, int repeat)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_LACARETTE_SYSTEM
				<< (unsigned char)MSG_EX_LACARETTE_RESULT_REP
				<< (unsigned char)MSG_EX_LACARETTE_RESULT_ERROR_SUC
				<< repeat;
}
void lacaretteUIrepMsg(CNetMsg::SP& msg, int repeat)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_LACARETTE_SYSTEM
				<< (unsigned char)MSG_EX_LACARETTE_UI_REP
				<< repeat;
}
void lacaretteJackpotMsg(CNetMsg::SP& msg, int itemIndex, const char* chName )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_LACARETTE_SYSTEM
				<< (unsigned char)MSG_EX_LACARETTE_JACKPOT
				<< itemIndex
				<< chName;
}
#endif

void ItemWearCostumeMsg(CNetMsg::SP& msg, char wear_pos, CItem* item1, CItem* item2)
{
	msg->Init(MSG_ITEM);

	RefMsg(msg) << (unsigned char)MSG_ITEM_WEAR_COSTUME
				<< (char)wear_pos;

	if (item1)
	{
		RefMsg(msg) << (int)item1->getVIndex()
					<< (int)item1->getDBIndex();
	}
	else
	{
		RefMsg(msg) << (int)-1
					<< (int)-1;
	}

	if (item2)
	{
		RefMsg(msg) << (int)item2->getVIndex()
					<< (int)item2->getDBIndex();
	}
	else
	{
		RefMsg(msg) << (int)-1
					<< (int)-1;
	}
}
void CostumeWearingMsg(CNetMsg::SP& msg, CPC* ch, char wear_pos, int item_db_index)
{
	if (!ch->m_bVisible)
	{
		msg->Init();
		return ;
	}
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_COSTUME2
				<< (unsigned char)MSG_EX_COSTUME2_WEARING
				<< ch->m_index
				<< wear_pos
				<< item_db_index;
}

void SysExpireCostumeItem(CNetMsg::SP& msg, int index)
{
	msg->Init(MSG_SYS);
	RefMsg(msg) << (unsigned char)MSG_SYS_COSTUME2_ITEM_EXPIRED
				<< index;
}

void CostumeWearingSuitMsg(CNetMsg::SP& msg, CPC* ch)
{
	if(ch == NULL)
	{
		msg->Init();
		return ;
	}

	if (!ch->m_bVisible)
	{
		msg->Init();
		return ;
	}
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_COSTUME2
				<< (unsigned char)MSG_EX_COSTUME2_WEARING_SUIT
				<< ch->m_index
				<< (int) COSTUME2_SUIT_MAX_WEARING;

	int pos = -1;
	for( int i = 0; i < COSTUME2_SUIT_MAX_WEARING; i++ )
	{
		pos = ch->GetSuitWearingPos(i);
		RefMsg(msg) << pos;
		pos += COSTUME2_WEARING_START;
		if( ch->m_wearInventory.wearItemInfo[pos] )
		{
			RefMsg(msg) << ch->m_wearInventory.wearItemInfo[pos]->getDBIndex();
		}
		else
		{
			RefMsg(msg) << -1;
		}
	}
}

void prepareSuitMsg(CNetMsg::SP& msg, CPC* ch)
{
	UpdateClient::doItemWearCostumeSuit* packet = reinterpret_cast<UpdateClient::doItemWearCostumeSuit*>(msg->m_buf);

	CItem* item = ch->m_wearInventory.getCosWearSuitItem();

	if(item == NULL)
	{
		return ;
	}

	packet->DBIndex = item->getDBIndex();
	packet->VIndex = item->getVIndex();

	for(int i = 0, j = 0; i < MAX_COSTUME_WEARING; i++)
	{
		if(ch->m_wearInventory.wearItemInfo[COSTUME2_WEARING_START + i] != NULL)
		{
			ch->m_wearInventory.makeItemInfo(&packet->info_list[j], ch->m_wearInventory.wearItemInfo[COSTUME2_WEARING_START + i]);
			j++;
		}
	}
}

#if defined(EVENT_WORLDCUP_2010)  || defined(EVENT_WORLDCUP_2010_TOTO) || defined(EVENT_WORLDCUP_2010_TOTO_STATUS) || defined(EVENT_WORLDCUP_2010_TOTO_GIFT) || defined(EVENT_WORLDCUP_2010_ATTENDANCE) || defined(EVENT_WORLDCUP_2010_KOREA)
void EventWorldcup2010TradeErrorMsg(CNetMsg::SP& msg, MSG_EVENT_WORLDCUP2010_ERROR_TYPE eError, int tradeType)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_WORLDCUP_2010
		<< (unsigned char)MSG_EVENT_WORLDCUP2010_TRADE_REP
		<< (unsigned char)eError
		<< tradeType;
}

void EventWorldcup2010ErrorMsg(CNetMsg::SP& msg, MSG_EVENT_WORLDCUP2010_TYPE repType, MSG_EVENT_WORLDCUP2010_ERROR_TYPE eError)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_WORLDCUP_2010
		<< (unsigned char)repType
		<< (unsigned char)eError;
}

void EventWorldcup2010TOTOStatusErrorMsg(CNetMsg::SP& msg, MSG_EVENT_WORLDCUP2010_ERROR_TYPE eError, int resultType, int itemindex)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_WORLDCUP_2010
		<< (unsigned char)MSG_EVENT_WORLDCUP2010_TOTO_STATUS_REP
		<< (unsigned char)eError
		<< resultType
		<< itemindex;
}

void EventWorldcup2010KoreaErrorMsg(CNetMsg::SP& msg, MSG_EVENT_WORLDCUP2010_ERROR_TYPE eError)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_WORLDCUP_2010
		<< (unsigned char)MSG_EVENT_WORLDCUP2010_KOREA_REP
		<< (unsigned char)eError;
}

void HelperWorldcup2010TOTORepMsg(CNetMsg::SP& msg, int charindex, int CountryItemIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EVENT_WORLDCUP2010
		<< (unsigned char)MSG_HELPER_WORLDCUP2010_TOTO_REQ
		<< charindex
		<< CountryItemIndex;
}
void HelperWorldcup2010TOTOStatusRepMsg(CNetMsg::SP& msg, int charindex, int resultType)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EVENT_WORLDCUP2010
		<< (unsigned char)MSG_HELPER_WORLDCUP2010_TOTO_STATUS_REQ
		<< charindex
		<< resultType;
}
void HelperWorldcup2010TOTOGiftRepMsg(CNetMsg::SP& msg, int charindex,int restore)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EVENT_WORLDCUP2010
		<< (unsigned char)MSG_HELPER_WORLDCUP2010_TOTO_GIFT_REQ
		<< charindex
		<< restore;
}
void HelperWorldcup2010AttendanceRepMsg(CNetMsg::SP& msg, int charindex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EVENT_WORLDCUP2010
		<< (unsigned char)MSG_HELPER_WORLDCUP2010_ATTENDANCE_REQ
		<< charindex;
}
void HelperWorldcup2010KoreaRepMsg(CNetMsg::SP& msg, int charindex, int type)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EVENT_WORLDCUP2010
		<< (unsigned char)MSG_HELPER_WORLDCUP2010_KOREA_REQ
		<< charindex
		<< type;
}
#endif


void MsgrDungeonTimeReqMsg(CNetMsg::SP& msg, MSG_MSGR_TYPE type, int arg1, int arg2)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << (int)-1
				<< (int)-1
				<< (int)-1
				<< (int)-1
				<< (unsigned char)type
				<< arg1
				<< arg2;
}

void DungeonTimeNoticeMsg(CNetMsg::SP& msg, MSG_EX_DUNGEONTIME_TYPE type, int start, int end, int exp)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << (int)MSG_EX_DUNGEONTIME
				<< (unsigned char)type;
	if (type != MSG_EX_DUNGEONTIME_END)
	{
		RefMsg(msg) << start
					<< end
					<< exp;
	}
}

void MsgrReloadCatalogMsg( CNetMsg::SP& msg , MSG_MSGR_TYPE type )
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << (int)-1
				<< (int)-1
				<< (int)-1
				<< (int)-1
				<< (unsigned char)type;
}

void MsgrPopupNoticeMsg( CNetMsg::SP& msg , int seq, int server, int subno, int zone, int html_num, int runtime, time_t start_time )
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_POPUP_NOTICE
		<< html_num
		<< runtime
		<< (int)start_time;
}

void MsgrPopupNoticeCloseMsg( CNetMsg::SP& msg , int seq, int server, int subno, int zone, int html_num )
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_POPUP_NOTICE_CLOSE
		<< html_num;
}

void MsgrPopupNoticeClearMsg( CNetMsg::SP& msg , int seq, int server, int subno, int zone )
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_POPUP_NOTICE_CLEAR;
}

void SocketMsg(CNetMsg::SP& msg, MSG_EX_SOCKET_TYPE type)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_SOCKET
				<< (unsigned char)type;
}

void SocketErrorMsg(CNetMsg::SP& msg, MSG_EX_SOCKET_ERROR_TYPE errortype)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_SOCKET
				<< (unsigned char)MSG_EX_SOCKET_ERROR
				<< (char)errortype;
}
void ItemSocketMsg(CNetMsg::SP& msg, const CItem* item)
{
	if(item->m_itemProto->getItemFlag() & ITEM_FLAG_SOCKET)
	{
		for(int i = 0; i < MAX_SOCKET_COUNT; i++)
		{
			RefMsg(msg) << item->m_socketList.GetJewelAt(i);
		}
	}
}

void tutorialrenewerMsg(CNetMsg::SP& msg, int index)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_TUTORIAL_RENEWER_2010
				<< index;
}

void RaidSceneMsg(CNetMsg::SP& msg, int TodoOutput, int object_type, int object_index, int object_data)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_RAID_SCENE
				<< TodoOutput
				<< object_type
				<< object_index
				<< object_data;
}

void SendOutputMsg(CNetMsg::SP& msg, int type, int index, int data)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_RAID_SCENE
				<< MSG_EX_RAID_SCENE_SEND_OUTPUT
				<< type
				<< index
				<< data;
}

void SendRaidSceneObjectStateMsg(CNetMsg::SP& msg, CPC* ch)
{
	if (ch == NULL || DEAD(ch))
	{
		msg->Init();
		return ;
	}

	CArea* pArea;
	pArea = ch->m_pArea;

	if(pArea == NULL)
	{
		msg->Init();
		return ;
	}

	int nDifficulty = pArea->m_RaidDifficulty;
	if(nDifficulty == -1)
	{
		msg->Init();
		return;
	}

	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_RAID_SCENE
				<< MSG_EX_RAID_SCENE_SEND_OBJECT_ALL_STATE;

	int nCount = pArea->m_RaidInfoList[nDifficulty].m_objectlist.GetCountRow();
	RefMsg(msg) << pArea->m_RaidInfoList[nDifficulty].m_objectlist.GetZoneNo()
				<< nCount;

	for(int i = 0; i < nCount; i++)
	{
		RefMsg(msg) << pArea->m_RaidInfoList[nDifficulty].m_objectlist.GetObject(i)
					<< pArea->m_RaidInfoList[nDifficulty].m_objectlist.GetObjectIndex(i)
					<< pArea->m_RaidInfoList[nDifficulty].m_objectlist.GetObjectState(i);
	}
}

void SendRaidSceneObjectRefreshMsg(CNetMsg::SP& msg, CPC* ch, int object_type, int index, int state)
{
	if(ch == NULL || DEAD(ch))
	{
		msg->Init();
		return ;
	}
	if(ch->m_pArea == NULL)
	{
		msg->Init();
		return ;
	}
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_RAID_SCENE
				<< MSG_EX_RAID_SCENE_SEND_OBJECT_REFRESH
				<< object_type
				<< index
				<< state;
}

void SendRaidScenePadoxSkillAction(CNetMsg::SP& msg)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_RAID_SCENE
				<< MSG_EX_RAID_SCENE_SEND_PADOX_SKILL;
}
void SendRaidSceneCountDown(CNetMsg::SP& msg, int count)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_RAID_SCENE
				<< MSG_EX_RAID_SCENE_SEND_COUNT_DOWN
				<< count;
}

#ifdef ENABLE_SUBJOB
void SubJobMsg( CNetMsg::SP& msg, MSG_EX_SUBJOB_TYPE type )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_SUBJOB
				<< type;
}

void SubJobErrorMsg( CNetMsg::SP& msg, int error )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_SUBJOB
				<< MSG_EX_SUBJOB_ERROR_FAIL
				<< error;
}
#endif //ENABLE_SUBJOB

void CastllanErrorMsg( CNetMsg::SP& msg, MSG_EX_CASTLLAN_ERROR_TYPE errorType )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASTLLAN
				<< (unsigned char) MSG_EX_CASTLLAN_ERROR
				<< (unsigned char) errorType;
}

#ifdef DEV_EVENT_PROMOTION2
void EventPromotion2RepErrorMsg(CNetMsg::SP& msg, int errorType)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char) MSG_EVENT_PROMOTION2
				<< (unsigned char) MSG_EVENT_PROMOTION2_COUPON_USE_REP
				<< (unsigned char) errorType;
}

void ConnPromotion2CouponUseReqMsg(CNetMsg::SP& msg, int userIndex, int charIndex, const char* couponNum )
{
	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char) MSG_CONN_EVENT_PROMOTION2
				<< (unsigned char) MSG_CONN_PROMOTION2_COUPON_USE_REQ
				<< userIndex
				<< charIndex
				<< couponNum;
}

void ConnPromotion2CouponItemReqMsg(CNetMsg::SP& msg, int userIndex, int charIndex, int keyIndex )
{
	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char) MSG_CONN_EVENT_PROMOTION2
				<< (unsigned char) MSG_CONN_PROMOTION2_COUPON_ITEM_REQ
				<< userIndex
				<< charIndex
				<< keyIndex;
}

#endif
void MonsterMercenarySummonMsg(CNetMsg::SP& msg, int index, char enable )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_MONSTER_MERCENARY
				<< (unsigned char) MSG_EX_MERCENARY_SUMMON
				<< index
				<< enable; // 1 소환,   -1 소환 해지
}

void MonsterMercenaryErrorMsg(CNetMsg::SP& msg, MSG_EX_MONSTER_MERCENARY_ERROR_TYPE errorType )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_MONSTER_MERCENARY
				<< (unsigned char) MSG_EX_MERCENARY_ERROR
				<< (unsigned char) errorType;
}

void MsgCashshopTypeListRep(CNetMsg::SP& msg, int t_count, int type_list[], int st_count, int subtype_list[])
{
	int i;
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASHITEM
				<< (unsigned char)MSG_EX_CASHITEM_TYPE_LIST_REP
				<< (unsigned char)TYPE_LIST_START
				<< t_count;
	for(i = 0; i < t_count; i++)
	{
		RefMsg(msg) << type_list[i];
	}
	RefMsg(msg) << (unsigned char)SUBTYPE_LIST_START
				<< st_count;
	for(i = 0; i < st_count; i++)
	{
		RefMsg(msg) << subtype_list[i];
	}
}

void MsgCashshopListRep(CNetMsg::SP& msg, int search_depth, int ct_count, int ctid[], int depth[], int remain_count[], int price[], int now_page, int total_page)
{
	int i = 0;
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASHITEM
				<< (unsigned char)MSG_EX_CASHITEM_LIST_REP
				<< search_depth
				<< (unsigned char)ct_count;
	for(i = 0; i < ct_count; i++)
	{
		RefMsg(msg) << ctid[i]
					<< depth[i]
					<< remain_count[i]
					<< price[i];
	}
	RefMsg(msg) << now_page
				<< total_page;
}

void MsgCashshopRecommandRep(CNetMsg::SP& msg, int ct_count, int ctid[], int type[], int remain_count[], int price[])
{
	int i = 0;
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASHITEM
				<< (unsigned char)MSG_EX_CASHITEM_RECOMMAND_REP
				<< (unsigned char)ct_count;
	for(i = 0; i < ct_count; i++)
	{
		RefMsg(msg) << ctid[i]
					<< type[i]
					<< remain_count[i]
					<< price[i];
	}
}

void MsgCashshopWishlistRep(CNetMsg::SP& msg, int ct_count, int ctid[], int type[], int remain_count[], int price[])
{
	int i = 0;
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASHITEM
				<< (unsigned char)MSG_EX_CASHITEM_WISHLIST_REP
				<< (unsigned char)ct_count;
	for(i = 0; i < ct_count; i++)
	{
		RefMsg(msg) << ctid[i]
					<< type[i]
					<< remain_count[i]
					<< price[i];
	}
}

void MsgCashshopWishlistSaveRep(CNetMsg::SP& msg, int errorcode, int ctid)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASHITEM
				<< (unsigned char)MSG_EX_CASHITEM_WISHLIST_SAVE_REP
				<< (unsigned char)errorcode
				<< ctid;
}

void MsgCashshopWishlistDelRep(CNetMsg::SP& msg, int errorcode, int ctid)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASHITEM
				<< (unsigned char)MSG_EX_CASHITEM_WISHLIST_DEL_REP
				<< (unsigned char)errorcode
				<< ctid;
}

void ConnWishlistReq(CNetMsg::SP& msg, CPC* pc)
{
	if(!pc)
		return ;
	if(!pc->m_desc)
		return ;

	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char)MSG_CONN_WISHLIST
				<< (unsigned char)MSG_WISHLIST_TYPE_LIST_REQ
				<< pc->m_desc->m_index
				<< pc->m_index;
}

void ConnWishlistSaveReq(CNetMsg::SP& msg, CPC* pc, int ctid)
{
	if(!pc)
		return ;
	if(!pc->m_desc)
		return ;

	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char)MSG_CONN_WISHLIST
				<< (unsigned char)MSG_WISHLIST_TYPE_SAVE_REQ
				<< pc->m_desc->m_index
				<< pc->m_index
				<< ctid;
}

void ConnWishlistDelReq(CNetMsg::SP& msg, CPC* pc, int ctid)
{
	if(!pc)
		return ;
	if(!pc->m_desc)
		return ;

	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char)MSG_CONN_WISHLIST
				<< (unsigned char)MSG_WISHLIST_TYPE_DEL_REQ
				<< pc->m_desc->m_index
				<< pc->m_index
				<< ctid;
}

void MsgCashShopLock(CNetMsg::SP& msg, MSG_EX_CASHITEM_TYPE type)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_CASHITEM
				<< (unsigned char) type;
}

// [selo: 110104] 메시지 박스
void MsgMessageBox(CNetMsg::SP& msg, MSG_EX_MSGBOX_TYPE type)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_MSGBOX
				<< (unsigned char) type;
}

void LCBallUseMsg(CNetMsg::SP& msg, char errorCode , int nItemIndex )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_LCBALL
				<< (char) MSG_EX_LCBALL_USE
				<< errorCode
				<< nItemIndex;
}

void LCBallSuccessMsg(CNetMsg::SP& msg, CLCString& strName, int nItemIndex)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_LCBALL
				<< (char) MSG_EX_LCBALL_MSG
				<< strName
				<< nItemIndex;
}

void SubHelperLCBallinfoMsg( CNetMsg::SP& msg, int charIndex, int coinIndex, char courseIndex )
{
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << (int)MSG_SUBHELPER_LCBALL_INFO
				<< charIndex
				<< coinIndex
				<< courseIndex;
}

void SubHelperLCBalluseMsg( CNetMsg::SP& msg, int charIndex, int coinIndex, char courseIndex )
{
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << (int)MSG_SUBHELPER_LCBALL_USE
				<< charIndex
				<< coinIndex
				<< courseIndex;
}

void SubHelperLCBalluse_successMsg( CNetMsg::SP& msg, char error, CLCString name, int coinIndex, char courseIndex, char cOrder , int itemIndex  )
{
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << (int)MSG_SUBHELPER_LCBALL_USE_SUCCESS
				<< error
				<< coinIndex
				<< courseIndex
				<< cOrder
				<< name
				<< itemIndex;
}

void EventEnchantRate(CNetMsg::SP& msg, unsigned char IsStart, int nEnchantRate)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_ENCHANT_RATE
				<< IsStart
				<< nEnchantRate;
}

#ifdef EVENT_USER_COMEBACK
void ConnUserComebackGiftReqMsg(CNetMsg::SP& msg, int userindex, int charindex)
{
	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char)MSG_CONN_EVENT_USER_COMEBACK
				<< userindex
				<< charindex;
}

void EventUserComebackErrorMsg(CNetMsg::SP& msg, MSG_EVENT_USER_COMEBACK_ERROR_TYPE errortype )
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char) MSG_EVENT_USER_COMEBACK
				<< (unsigned char) errortype;
}
#endif

#ifdef EVENT_CHAR_BIRTHDAY
void HelperCharBirthdayReqMsg(CNetMsg::SP& msg, MSG_EVENT_CHAR_BIRTHDAY_TYPE subtype, int charindex )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_CHAR_BIRTHDAY
				<< (unsigned char) subtype
				<< charindex;
}

void EventCharBirthdayErrorMsg(CNetMsg::SP& msg, MSG_EVENT_CHAR_BIRTHDAY_ERROR_TYPE errortype)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char) MSG_EVENT_CHAR_BIRTHDAY
				<< (unsigned char) MSG_EVENT_CHAR_BIRTHDAY_GIFT_REP
				<< (unsigned char) errortype;
}
#endif
#ifdef EVENT_EUR2_PROMOTION
void ConnEUR2PromotionMsg(CNetMsg::SP& msg, int CharIndex, int UserIndex, CLCString& Key, unsigned char bInvenSpace, unsigned char bWeight)
{
	msg->Init(MSG_CONN_REQ);
	RefMsg(msg) << (unsigned char)MSG_CONN_EVENT_EUR2_PROMOTION
				<< CharIndex
				<< UserIndex
				<< Key
				<< bInvenSpace
				<< bWeight;
}

void EventEUR2PromotionMsg(CNetMsg::SP& msg, MSG_EVENT_EUR2_PROMOTION_ERROR_TYPE type)
{
	msg->Init(MSG_EVENT);
	RefMsg(msg) << (unsigned char)MSG_EVENT_EUR2_PROMOTION
				<< (unsigned char)type;
}
#endif

void NpcNoticeMsg(CNetMsg::SP& msg, MSG_CHAT_TYPE chatType, int npcIndex, const char* chat)
{
	char dummy[10] = "sora";	// 이전 클라이언트 메세지와 연결을 위해 더미 추가

	msg->Init(MSG_CHAT);
	RefMsg(msg) << (unsigned char)chatType
				<< npcIndex
				<< dummy
				<< dummy
				<< chat;
}
#ifdef DEV_GUILD_MARK
void HelperGuildMarkRegistReqMsg(CNetMsg::SP& msg, int GuildIndex, int CharIndex, char gm_row, char gm_col, char bg_row, char bg_col, unsigned short tab, unsigned short invenIndex, int markTime, const char* itemserial)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_NEW_GUILD_MARK_REGIST
				<< GuildIndex
				<< CharIndex
				<< gm_row
				<< gm_col
				<< bg_row
				<< bg_col
				<< tab
				<< invenIndex
				<< markTime // 헬퍼로 보내는 markTime은 실제 현실시간이다.
				<< itemserial;
}

void GuildMarkEditFinMsg(CNetMsg::SP& msg, char gm_row, char gm_col, char bg_row, char bg_col, int markTime)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_NEW_GUILD_MARK_EDIT_FIN
				<< gm_row
				<< gm_col
				<< bg_row
				<< bg_col
				<< markTime;
}

void GuildMarkExpireMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_NEW_GUILD_MARK_EXPIRE;
}
#endif

#ifdef DEV_GUILD_STASH
void GuildStashListMsg( CNetMsg::SP& msg , CLCString& timeStamp, int capacity, GoldType_t money, int itemCount )
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char) MSG_NEW_GUILD_STASH_LIST
				<< timeStamp
				<< capacity
				<< money
				<< itemCount;
}

void GuildStashErrorMsg( CNetMsg::SP& msg, unsigned char subtype, char error )
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char) subtype
				<< error;
}

void GuildStashLogMsg( CNetMsg::SP& msg , int logCount )
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char) MSG_NEW_GUILD_STASH_LOG
				<< logCount;
}

void HelperGuildStashCreateMsg( CNetMsg::SP& msg , int guildIndex, int charIndex )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_STASH_CREATE
				<< guildIndex
				<< charIndex;
}

void HelperGuildStashListMsg( CNetMsg::SP& msg , int guildIndex, int charIndex )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_STASH_LIST
				<< guildIndex
				<< charIndex;
}

void HelperGuildStashKeepMsg( CNetMsg::SP& msg , int guildIndex, int charIndex, LONGLONG keeyMoney, int itemCount )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_STASH_KEEP
				<< guildIndex
				<< charIndex
				<< keeyMoney
				<< itemCount;
}

void HelperGuildStashKeepErrorMsg( CNetMsg::SP& msg , int charIndex, int guildIndex, char error, LONGLONG money )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_STASH_KEEP_ERROR
				<< charIndex
				<< guildIndex
				<< error
				<< money;
}
void HelperGuildStashTakeMsg( CNetMsg::SP& msg , int guildIndex, int charIndex, LONGLONG takeMoney, int itemCount )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_STASH_TAKE
				<< guildIndex
				<< charIndex
				<< takeMoney
				<< itemCount;
}
void HelperGuildStashTakeErrorMsg(  CNetMsg::SP& msg , int charIndex, int guildIndex, char error, LONGLONG money )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_STASH_TAKE_ERROR
				<< charIndex
				<< guildIndex
				<< error
				<< money;
}
void HelperGuildStashLogMsg( CNetMsg::SP& msg , int charIndex, int guildIndex, int lastLogIdx )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_STASH_LOG
				<< guildIndex
				<< charIndex
				<< lastLogIdx;
}

#endif // DEV_GUILD_STASH

void SysRaidZoneRemain(CNetMsg::SP& msg, int remain)
{
	msg->Init(MSG_SYS);
	RefMsg(msg) << (unsigned char)MSG_SYS_ENTER_RAID_ZONE_REMAIN
				<< remain;
}

void HelperAutoResetRaid(CNetMsg::SP& msg, int index, int zone, int area[][2])
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_AUTO_RESET_RAID
				<< zone
				<< index;

	int i;
	for(i = 0; i < index; i++)
	{
		RefMsg(msg) << area[i][0]
					<< area[i][1];
	}
}

#ifdef WARCASTLE_STATE_CHANNEL_CYNC
void HelperWarCastleStateCync(CNetMsg::SP& msg)
{
	if( gserver->m_subno != WAR_CASTLE_SUBNUMBER_MERAC && gserver->m_subno != WAR_CASTLE_SUBNUMBER_DRATAN )
		return;

	// 메라크 공성 확인.
	msg->Init(MSG_HELPER_REQ);
	RefMsg(msg) << -1
				<< gserver->m_serverno
				<< -1
				<< -1
				<< (unsigned char)MSG_HELPER_WARCASTLE_STATE_CYNC;

	CWarCastle* pMercWar = CWarCastle::GetCastleObject(ZONE_MERAC);
	if( pMercWar )
		RefMsg(msg) << pMercWar->GetState();
	else
		RefMsg(msg) << WCSF_NORMAL;

	CWarCastle * pDratanWar = CWarCastle::GetCastleObject(ZONE_DRATAN);
	if( pDratanWar )
		RefMsg(msg) << pDratanWar->GetState();
	else
		RefMsg(msg) << WCSF_NORMAL;
}
#endif // WARCASTLE_STATE_CHANNEL_CYNC

void MsgrRoyalRumbleStart(CNetMsg::SP& msg, int seq, int server, int subno, int zone, MSG_MSGR_TYPE type, int cmd)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)type
				<< cmd;
}

void MsgrRoyalRumbleWinner(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int leveltype, int charindex, CLCString nick)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_WARGROUND_WINNER
				<< leveltype
				<< charindex
				<< nick;
}

void RoyalRumbleNotice(CNetMsg::SP& msg, unsigned char command)
{
	// command 0:신청시간입니다. 1:5분남았습니다. 2:종료되었습니다.
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_ROYAL_RUMBLE
				<< (unsigned char)MSG_EX_ROYAL_RUMBLE_NOTICE
				<< (unsigned char)command;
}

void RoyalRumbleRegistMenu(CNetMsg::SP& msg, unsigned char command)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_ROYAL_RUMBLE
				<< (unsigned char)MSG_EX_ROYAL_RUMBLE_REG_MENU
				<< (unsigned char)command;
}

void RoyalRumbleRegist(CNetMsg::SP& msg, unsigned char command)
{
	// command 0:성공, 1:돈없음, 2:선수등록이 불가능 합니다. 3:이미 등록하였습니다.
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_ROYAL_RUMBLE
				<< (unsigned char)MSG_EX_ROYAL_RUMBLE_PLAYER_REP
				<< command;
}

void RoyalRumbleUnRegist(CNetMsg::SP& msg, unsigned char command)
{
	// command 0:취소 성공, 1:취소할 수 있는 시간이 아닙니다, 2:등록자 명단에 없습니다.
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_ROYAL_RUMBLE
				<< (unsigned char)MSG_EX_ROYAL_RUMBLE_PLAYER_UNREGIST_REP
				<< command;
}

void RoyalRumbleRegistResult(CNetMsg::SP& msg, unsigned char command)
{
	// command 0:no status, 1:selected, 2:no selected, 3:Game draw
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_ROYAL_RUMBLE
				<< (unsigned char)MSG_EX_ROYAL_RUMBLE_PLAYER_REG_RESULT
				<< command;
}

void RoyalRumbleGoZone(CNetMsg::SP& msg)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_ROYAL_RUMBLE
				<< (unsigned char)MSG_EX_ROYAL_RUMBLE_GO_ZONE;
}

void RoyalRumbleEnterNextPlayer(CNetMsg::SP& msg, int second)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_ROYAL_RUMBLE
				<< (unsigned char)MSG_EX_ROYAL_RUMBLE_ENTER_NEXT_PLAYER
				<< second;
}

void RoyalRumbleHold(CNetMsg::SP& msg, unsigned char boolean)
{
	// (s->c) (uc) boolean 0:unhold 1:hold
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_ROYAL_RUMBLE
				<< (unsigned char)MSG_EX_ROYAL_RUMBLE_HOLD
				<< boolean;
}

void RoyalRumbleWinner(CNetMsg::SP& msg, int leveltype, int charindex, CLCString nick)
{
	// (s->c) (n)leveltype 0:rookie, 1:senior, 2:master, (n)character index (str)nick
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_ROYAL_RUMBLE
				<< (unsigned char)MSG_EX_ROYAL_RUMBLE_WINNER
				<< leveltype
				<< charindex
				<< nick;
}

void RoyalRumbleReward(CNetMsg::SP& msg, unsigned char command)
{
	// (s->c) (uc)command 0: 보상 대상자가 아닙니다, 1:이미 보상을 받았습니다.
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_ROYAL_RUMBLE
				<< (unsigned char)MSG_EX_ROYAL_RUMBLE_REWARD_REP
				<< command;
}

void RoyalRumbleWarPoint(CNetMsg::SP& msg, int warpoint, int waraccpoint)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_ROYAL_RUMBLE
				<< (unsigned char)MSG_EX_ROYAL_RUMBLE_POINT_REP
				<< warpoint
				<< waraccpoint;
}
void RoyalRumbleTimeReq(CNetMsg::SP& msg, int time)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_ROYAL_RUMBLE
				<< (unsigned char)MSG_EX_ROYAL_RUMBLE_TIME_REP
				<< time;
}

void RoyalRumbleLeftCount(CNetMsg::SP& msg, int count)
{
	if(count < 0)
	{
		msg->Init();
		return ;
	}
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_ROYAL_RUMBLE
				<< (unsigned char)MSG_EX_ROYAL_RUMBLE_LEFT_COUNT
				<< count;
}
void RoyalRumbleTimeNotice(CNetMsg::SP& msg, unsigned char command, int remain)
{
	// command 0: 20분 남음, 1:10분남음, 2:5분 남음, 3:종료
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_ROYAL_RUMBLE
				<< (unsigned char) MSG_EX_ROYAL_RUMBLE_TIME_NOTICE
				<< command;
	RefMsg(msg) << remain;
}

void RoyalRumbleNotfoundItem(CNetMsg::SP& msg)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_ROYAL_RUMBLE
				<< (unsigned char)MSG_EX_ROYAL_RUMBLE_NOTFOUND_ITEM;
}
void RoyalRumbleKillPlayer(CNetMsg::SP& msg, CPC* opc, CPC* dpc)
{
	if(opc == NULL || dpc == NULL)
	{
		msg->Init();
		return ;
	}
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_ROYAL_RUMBLE
				<< (unsigned char)MSG_EX_ROYAL_RUMBLE_KILL_PLAYER
				<< opc->GetName()
				<< dpc->GetName();
}

#ifdef SYSTEM_TREASURE_MAP
void MsgrSetTreasureMap( CNetMsg::SP& msg, int server, int charindex, const char* arg)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << 0
				<< server
				<< -1
				<< -1
				<< (unsigned char) MSG_MSGR_SET_TREASURE_MAP
				<< charindex
				<< arg;
}

#ifdef SYSTEM_TREASURE_MAP_GAMIGO
void MsgTreaserMapKeyExchangeError( CNetMsg::SP& msg, MSG_EX_TRS_MAP_KEY_EXCHANGE_ERROR_TYPE errorType )
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_TREASURE_MAP_SYSTEM_KEY_EXCHANGE
				<< (unsigned char) errorType;
}
#endif // SYSTEM_TREASURE_MAP_GAMIGO
#endif // SYSTEM_TREASURE_MAP

#ifdef GM_COMMAND_NOTICE
void MsgSetGMCommandNotice( CNetMsg::SP& msg, MSG_GM_COMMAND_NOTICE_TYPE Type, CLCString gm, CLCString user )
{
	msg->Init(MSG_SYS);
	RefMsg(msg) << (unsigned char)MSG_SYS_GMCOMMAND_NOTICE
				<< (unsigned char)Type
				<< gm
				<< user;
}
#endif

#ifdef GUILD_MEMBER_KICK_JOIN_DELAY
void HelperGuildKickOutDateUpdateReqMsg(CNetMsg::SP& msg, int charindex, int outDate )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_MEMBER_KICK_OUTDATE_UPDATE_REQ
				<< charindex
				<< outDate;
}
#endif

void HelperStuentListRefresh(CNetMsg::SP& msg, int teacherIndex )
{
	msg->Init(MSG_HELPER_REQ);
	RefMsg(msg) << -1
				<< gserver->m_serverno
				<< -1
				<< -1
				<< (unsigned char)MSG_HELPER_TEACH_STUDENT_LIST_SYCN
				<< teacherIndex;
}

void HelperTeacherStudentListCyncReq(CNetMsg::SP& msg, CPC* pc)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_TEACHER_STUDENT_LIST_CYNC_REQ
				<< pc->m_desc->m_idname
				<< pc->m_index;
}

void SubHelperLevelup(CNetMsg::SP& msg, int char_index)
{
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << MSG_SUBHELPER_LEVELUP
				<< char_index;
}

void SubHelperRankingRefreshEx(CNetMsg::SP& msg, int char_index)
{
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << MSG_SUBHELPER_RANKING_EX_REFRESH
				<< char_index;
}

void SubHelperRankingListReqEx(CNetMsg::SP& msg, unsigned char type, unsigned char subType, int char_index)
{
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << MSG_SUBHELPER_RANKING_EX_LIST
				<< type
				<< subType
				<< char_index;
}

void SubHelperRankingSearchReqEx(CNetMsg::SP& msg, unsigned char type, unsigned char subType, int char_index, CLCString name)
{
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << MSG_SUBHELPER_RANKING_EX_SEARCH
				<< type
				<< subType
				<< char_index
				<< name;
}

void SubHelperRankingRewardReqEx(CNetMsg::SP& msg, int endTime)
{
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << MSG_SUBHELPER_RANKING_EX_REWARD
				<< endTime;
}

void RankingEXList(CNetMsg::SP& msg, int listType, unsigned char type, unsigned char subtype)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_RANKING_SYSTEM_EX
				<< listType
				<< type
				<< subtype;
}

void RankingEXRefresh(CNetMsg::SP& msg)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_RANKING_SYSTEM_EX
				<< MSG_EX_RANKING_REFRESH;
}

void RankingEXError(CNetMsg::SP& msg, unsigned char type)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_RANKING_SYSTEM_EX
				<< MSG_EX_RANKING_ERROR
				<< type;
}

#ifdef REFORM_PK_PENALTY_201108 // 2011-08 PK 패널티 리폼
void MsgPKPenaltyRewardRep(CNetMsg::SP& msg, int rewardNum, int errorType)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_PKPENALTY_REFORM
				<< (unsigned char) MSG_EX_PKPENALTY_REFORM_REWARD_REP
				<< rewardNum
				<< (unsigned char) errorType;
}

void MsgPKPenaltyRewardInfoRep(CNetMsg::SP& msg, int rewardFlag)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_PKPENALTY_REFORM
				<< (unsigned char) MSG_EX_PKPENALTY_REFORM_REWARD_INFO
				<< rewardFlag;
}

void MsgSubHelperPKPenaltyRewardReq(CNetMsg::SP& msg, int charindex, int rewardNum, int titleIndex, int endTime )
{
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << MSG_SUBHELPER_PKPENALTY_REFORM
				<< (unsigned char) MSG_SUBHELPER_PKPENALTY_REFORM_REWARD_REQ
				<< charindex
				<< rewardNum
				<< titleIndex
				<< endTime;
}

void MsgSubHelperPKPenaltyRewardInfoReq(CNetMsg::SP& msg, int charindex)
{
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << MSG_SUBHELPER_PKPENALTY_REFORM
				<< (unsigned char) MSG_SUBHELPER_PKPENALTY_REFORM_REWARD_INFO_REQ
				<< charindex;
}
#endif // REFORM_PK_PENALTY_201108 // 2011-08 PK 패널티 리폼

#ifdef NOTICE_SYSTEM
void SubHelperUserNoticeRegMsg(CNetMsg::SP& msg, int _characterIndex, CLCString _name, CLCString _contents)
{
	msg->Init(MSG_SUBHELPER_NOTICE);
	RefMsg(msg) << MSG_SUBHELPER_USER_NOTICE_REG_REQ
		<< _characterIndex
		<< _name
		<< _contents;
}

void UserNoticePrintMsg(CNetMsg::SP& msg, CLCString _name, CLCString _contents)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_USER_NOTICE
		<< MSG_EX_USER_NOTICE_PRINT
		<< _name
		<< _contents;
}

void UserNoticeRegRepMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_USER_NOTICE
		<< MSG_EX_USER_NOTICE_REG_REP;
}

void UserNoticeErrorMsg(CNetMsg::SP& msg, int _errorType)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_USER_NOTICE
		<< MSG_EX_USER_NOTICE_ERROR
		<< _errorType;
}

#endif // NOTICE_SYSTEM

void HelperGuildMasterKickReq(CNetMsg::SP& msg, int _guildIndex, int _requstCharIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_MASTER_KICK_REQ
				<< _guildIndex
				<< _requstCharIndex;
}

void HelperGuildMasterKickCancelReq(CNetMsg::SP& msg, int _guildIndex, int _requestCharIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_MASTER_KICK_CANCEL_REQ
				<< _guildIndex
				<< _requestCharIndex;
}

void GuildMasterKickRep(CNetMsg::SP& msg, int _guildIndex, CLCString _currentBoss)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_MASTER_KICK_REP
				<< _guildIndex
				<< _currentBoss;
}

void GuildMasterKickCancelRep(CNetMsg::SP& msg, int _guildIndex)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_MASTER_KICK_CANCEL_REP
				<< _guildIndex;
}

void GuildMasterKickStatus(CNetMsg::SP& msg, int _guildIndex, int _status)
{
	msg->Init(MSG_GUILD);
	RefMsg(msg) << (unsigned char)MSG_GUILD_MASTER_KICK_STATUS
				<< _guildIndex
				<< _status;
}

void HelperGuildMasterKickReset(CNetMsg::SP& msg, int _charIndex, int _guildIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_MASTER_KICK_RESET
				<< _charIndex
				<< _guildIndex;
}

void EventActiveListMsg(CNetMsg::SP& msg, std::map<int, CEventInfo*> * _eventList)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_EVENT_AUTOMATION_SYSTEM
				<< (unsigned char) MSG_EX_EVENT_AUTOMATION_LIST
				<< (int)_eventList->size();

	std::map<int, CEventInfo*>::iterator itr;
	std::map<int, CEventInfo*>::iterator itrEnd = _eventList->end();
	for( itr=_eventList->begin(); itr!=itrEnd; itr++)
		RefMsg(msg) << itr->first;
}

void EventActiveUpdateMsg(CNetMsg::SP& msg, unsigned char _subtype, int _eventIndex)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_EVENT_AUTOMATION_SYSTEM
				<< _subtype
				<< _eventIndex;
}

void MsgrEventActiveUpdateMsg(CNetMsg::SP& msg, MSG_EX_EVENT_AUTOMATION_SYSTEM_TYPE _subtype, int _eventIndex, int _notice)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << -1
				<< -1
				<< -1
				<< -1
				<< (unsigned char) MSG_MSGR_EVENT_AUTOMATION
				<< (unsigned char) _subtype
				<< _eventIndex
				<< _notice;
}

void JumpingResultMsg(CNetMsg::SP& msg, MSG_EX_JUMPING_SYSTEM_TYPE _subtype, int _value)
{
	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_JUMPING_SYSTEM
				<< (unsigned char)_subtype;

	if ( _subtype == MSG_EX_JUMPING_ERROR_NOT_EXIST_CHAR )
	{
		RefMsg(msg) << _value;
	}
}

void HelperGuildMasterChange(CNetMsg::SP& msg, int _newBossIndex, int _guildIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_MASTER_CHANGE
				<< _newBossIndex
				<< _guildIndex;
}
void SubHelperCharSlotExtUse(CNetMsg::SP& msg, int _userIndex, int _charIndex, int _tab, int _invenIndex, CLCString _serial, int _days)
{
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << MSG_SUBHELPER_EXT_CHAR_SLOT_ITEM_USE_REQ
				<< _userIndex
				<< _charIndex
				<< _tab
				<< _invenIndex
				<< _serial
				<< _days;
}

void SubHelperCharSlotExtUseResult(CNetMsg::SP& msg, int _userIndex)
{
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << MSG_SUBHELPER_EXT_CHAR_SLOT_ITEM_USE_ROLLBACK
				<< _userIndex;
}

void ItemUseCharSlotExt(CNetMsg::SP& msg, unsigned char cmd)
{
	msg->Init(MSG_ITEM);
	RefMsg(msg) << (unsigned char)MSG_ITEM_USE_CHAR_SLOT_EXT
				<< cmd;
}

void DBCharSlotExtTime(CNetMsg::SP& msg, long time)
{
	msg->Init(MSG_DB);
	RefMsg(msg) << (unsigned char)MSG_DB_CHAR_SLOT_TIME
				<< time;
}

void RecoverExpSp(CNetMsg::SP& msg, LONGLONG recoverEXP, LONGLONG recoverSP)
{
	msg->Init(MSG_RECOVER_EXP_SP);
	RefMsg(msg) << recoverEXP << recoverSP;
}

void SysNpcPortalExpiredMsg(CNetMsg::SP& msg, int itemIndex)
{
	msg->Init(MSG_SYS);
	RefMsg(msg) << (unsigned char)MSG_SYS_NPC_PORTAL_EXPIRED
				<< itemIndex;
}

void ReformerSystemRep(CNetMsg::SP& msg, unsigned char cmdType_, CItem *pReformItem_)
{
	if (cmdType_ == MSG_EX_REFORMER_SYSTEM_REFORM_SUCCESS_REP && pReformItem_ == NULL)
	{
		ReformerSystemRep(msg, MSG_EX_REFORMER_SYSTEM_EXCEPTION_ERROR_REP);
		return ;
	}

	msg->Init(MSG_EXTEND);
	RefMsg(msg) << MSG_EX_REFORMER_SYSTEM
				<< cmdType_;

	if (pReformItem_ != NULL)
	{
		// 리폼에 성공한 아이템 정보 보내기
		RefMsg(msg) << (unsigned short)pReformItem_->tab()
					<< (unsigned short)pReformItem_->getInvenIndex()
					<< pReformItem_->getVIndex()
					<< pReformItem_->getDBIndex();
	}
}

void MsgrRoyalRumbleSettingReq(CNetMsg::SP& msg, const int min, const int max, const int startHour, const int charindex)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << -1
				<< -1
				<< -1
				<< (int)-1
				<< (unsigned char)MSG_MSGR_ROYAL_RUMBLE_SETTING
				<< min
				<< max
				<< startHour
				<< charindex;
}

void MsgrSendMsgToCharacterReq(CNetMsg::SP& msg, const int charindex, const char* message)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << -1
				<< -1
				<< -1
				<< (int)-1
				<< (unsigned char)MSG_MSGR_SEND_MSG_TO_CHAR
				<< charindex
				<< message;
}
void NoticeFortuneMsg(CNetMsg::SP& msg, int ret, const int item_index, int skill_index, int skill_level)
{
	msg->Init(MSG_ITEM);

	RefMsg(msg) << (unsigned char)MSG_ITEM_COSTUME_SUIT_FORTUNE
				<< ret
				<< item_index
				<< skill_index
				<< skill_level;
}
