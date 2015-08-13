#ifndef __PTYPE_CHAR_STATUS_H__
#define __PTYPE_CHAR_STATUS_H__

#include "ptype_base.h"
enum
{
	eArti_Str,
	eArti_Dex,
	eArti_Int,
	eArti_Con,
	eArti_MAX,
};

enum
{
	MSG_SUB_UPDATE_FIRST_MONENY,			// 돈(나스) 변화
	MSG_SUB_UPDATE_FIRST_MONENY_IN_STASH,	// 창고 돈(나스) 변화
	MSG_SUB_UPDATE_MONENY,				// 돈(나스) 변화
	MSG_SUB_UPDATE_MONENY_IN_STASH,		// 창고 돈(나스) 변화

	MSG_SUB_UPDATE_INVEN_LIST,			// 가방 아이템 리스트 (클라에서는 기존 데이터를 지우고 무조건 이 값으로 변경)

	MSG_SUB_UPDATE_ADD_ITEM,			// 1개의 아이템이 신규로 추가됨
	MSG_SUB_UPDATE_DELETE_ITEM,			// 1개의 아이템 삭제
	MSG_SUB_UPDATE_CHANGE_ITEM_COUNT,	// 1개의 아이템의 갯수가 변화됨
	MSG_SUB_UPDATE_CHANGE_ITEM_COUNT_WITH_SWAP,	// 1개의 아이템의 갯수가 변화됨

	MSG_SUB_UPDATE_CHANGE_ITEM_PLUS,
	MSG_SUB_UPDATE_CHANGE_ITEM_PLUS_2,
	MSG_SUB_UPDATE_CHANGE_ITEM_FLAG,
	MSG_SUB_UPDATE_CHANGE_ITEM_USED,
	MSG_SUB_UPDATE_CHANGE_ITEM_USED_2,
	MSG_SUB_UPDATE_CHANGE_ITEM_WEAR_POS,
	MSG_SUB_UPDATE_CHANGE_ITEM_OPTION,
	MSG_SUB_UPDATE_CHANGE_ITEM_SOCKET,
	MSG_SUB_UPDATE_CHANGE_ITEM_ORIGIN_VAR,
	MSG_SUB_UPDATE_CHANGE_ITEM_COMPOSITE_ITEM,

	MSG_SUB_UPDATE_NAS_DESC_REASON,			// 돈 감소 사유

	MSG_SUB_UPDATE_OLD_TIMER_ITEM,
	MSG_SUB_UPDATE_ERASE_ALL_ITEM,

	MSG_SUB_UPDATE_WEAR,					// 장비 장착
	MSG_SUB_UPDATE_WEAR_TAKEOFF,			// 장비 탈착
	MSG_SUB_UPDATE_WEAR_LIST,				// 장비 리스트
	MSG_SUB_UPDATE_WEAR_CHANGE_INFO,		// 장비 정보 변경

	MSG_SUB_UPDATE_COST_WEAR,				// 코스튬 장착
	MSG_SUB_UPDATE_COST_WEAR_TAKEOFF,		// 코스튬 탈착
	MSG_SUB_UPDATE_COST_WEAR_LIST,			// 코스튬 장비 리스트

	MSG_SUB_UPDATE_COST_WEAR_SUIT,			// 코스튬 한벌의상 장착

	MSG_SUB_UPDATE_CHANGE_WEAR_ITEM_USED,	// 장착 아이템 USE 업데이트
	MSG_SUB_UPDATE_CHANGE_WEAR_ITEM_PLUS,	// 장착 아이템 PLUS 업데이트
	MSG_SUB_UPDATE_CHANGE_WEAR_ITEM_FLAG,	// 장착 아이템 FLAG 업데이트

	MSG_SUB_UPDATE_STASH_PASSWORD_FLAG,		// 창고 패스워드 유무

#ifdef DURABILITY
	MSG_SUB_UPDATE_ITEM_DURABILITY_FOR_INVENTORY,
	MSG_SUB_UPDATE_ITEM_DURABILITY_FOR_WEAR,
#endif

	MSG_SUB_UPDATE_STATUS = 100,		// 캐릭터 정보(모두)
	MSG_SUB_UPDATE_AT,
	MSG_SUB_UPDATE_APPEAR_PC,
	MSG_SUB_UPDATE_APPEAR_NPC,
	MSG_SUB_UPDATE_APPEAR_PET,
	MSG_SUB_UPDATE_APPEAR_ELEMENTAL,
	MSG_SUB_UPDATE_APPEAR_APET,

	MSG_SUB_UPDATE_DISAPPERAR,
	MSG_SUB_UPDATE_DISAPPERAR_FOR_CLIENT_EFFECT,
	MSG_SUB_UPDATE_DISAPPERAR_ALL,		//클라이언트에게 캐릭터를 기준으로 주위에 있는 모든 것을 지우라고 하는 명령 일반적인 DISAPPEAR와는 다름
	MSG_SUB_UPDATE_TARGET_CLEAR,

	MSG_SUB_UPDATE_ELEMENTAL_STATUS,
	MSG_SUB_UPDATE_EXPED_INFO,
	MSG_SUB_UPDATE_PARTY_INFO,

	MSG_SUB_UPDATE_CHAR_STATUS_PC,
	MSG_SUB_UPDATE_CHAR_STATUS_NPC,
	MSG_SUB_UPDATE_CHAR_STATUS_PET,
	MSG_SUB_UPDATE_CHAR_STATUS_ELEMENTAL,
	MSG_SUB_UPDATE_CHAR_STATUS_APET,

	MSG_SUB_UPDATE_DAMAGE,
	MSG_SUB_UPDATE_APET_SMALL_STATUS,

	MSG_SUB_UPDATE_LEVELUP_INFO,

	MSG_SUB_UPDATE_TARGET_HP_INFO,

	MSG_SUB_UPDATE_HOLYWATER_STATE,

#ifdef PREMIUM_CHAR
	MSG_SUB_UPDATE_PREMIUM_CHAR_INFO,
	MSG_SUB_UPDATE_PREMIUM_CHAR_JUMP_COUNT,
	MSG_SUB_UPDATE_PREMIUM_CHAR_END,

	MSG_SUB_UPDATE_PREMIUM_CHAR_CHANGE_FLAG,		// 프리미엄 캐릭터 상태 변경
#endif

	MSG_SUB_UPDATE_ATTR_ITEM_USE,
	MSG_SUB_UPDATE_ATTR_STATE_FOR_NPC,

	MSG_SUB_UPDATE_CHECK_NOTICE,			//공지 확인 깜박임
	MSG_SUB_UPDATE_CHECK_POPUP_NOTICE,		//팝업 공지 확인
	MSG_SUB_UPDATE_CLOSE_POPUP_NOTICE,		//팝옵 공지 종료
	MSG_SUB_UPDATE_CLEAR_POPUP_NOTICE,		//팝업 전체 종료

	MSG_SUB_UPDATE_ARTIFACT_SYSMSG,
	MSG_SUB_UPDATE_ARTIFACT_FIND_USER_COUNT_MSG,

	MSG_SUB_REQUEST_GUILD_RECALL,			//서버에서 길드원들에게 길드방 리콜 확인 여부 요청 업데이트 패킷
};

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
struct tag_assistinfo // 이 구조체를 포함하는 구조체에서는 항상 맨 마지막에 사용할것 (0배열때문)
{
	LONGLONG		state;
	int				count;
	struct tag_list
	{
		int			index;
		int			dbIndex;
		char		level;
		int			remain;
		int			remainCount;
	} list[0];
};

namespace UpdateClient
{
struct oldTimerItem : public pTypeBase
{
	int memposTime;		// 메모리 스크롤
	int stashextTime;	// 개인창고 확장
};

struct stashPasswordFlag : public pTypeBase
{
	unsigned char		flag;		// 0 - 없음 , 1 - 있음
};

struct charStatus : public pTypeBase
{
	int				level;
	LONGLONG		exp;
	LONGLONG		levelupExp;
	int				hp;
	int				maxHp;
	int				maxHpOrg;
	int				mp;
	int				maxMp;
	int				maxMpOrg;
	unsigned char	attrDef;
	unsigned char	attrAtt;
	int				str;
	int				dex;
	int				intl;
	int				con;
	int				opStr;
	int				opDex;
	int				opInt;
	int				opCon;
	int				attack;
	int				attack_diff;
	int				magic;
	int				magic_diff;
	int				defense;
	int				defense_diff;
	int				resist;
	int				resist_diff;
	int				avoid_melee;
	int				avoid_melee_org;
	int				avoid_magic;
	int				avoid_magic_org;
	int				fhitrate_melee;
	int				fhitrate_melee_org;
	int				fhitrate_magic;
	int				fhitrate_magic_org;
	int				critical;
	int				critical_org;
	int				deadly;
	int				deadly_org;
	LONGLONG		skillPoint;
	float			walkSpeed;

	float			run_or_fly_speed;
	float			run_or_fly_speed_org;

	char			attackSpeed;
	char			attackSpeedOrg;

	char			magicSpeed;
	char			pkName;
	int				pkPenalty;
	int				pkCount;
	int				fame;
	float			attackRange;

	char			joinForMerac;
	char			joinForDratan;

	int				skillSpeed;
	unsigned short	mapAttr;
	char			mountPet;
	char			colorType;
	int				evocationIndex;
	int				remainEvocation;

	char			bPressCorps;
	int				prob;
	int				opDecreaseSkillMP;

	int				ep;
	int				maxEp;

	int				party_arti[eArti_MAX];

	int				opMoney_buy_addition;
	int				opMoney_buy_rate;
	int				opMoney_sell_addition;
	int				opMoney_sell_rate;

	//int				adminLevel;
};

struct charAtMsg : public pTypeBase
{
	int				charIndex;
	char			charName[MAX_CHAR_NAME_LENGTH + 1];
	unsigned char	job;
	unsigned char	job2;
	unsigned char	hairstyle;
	unsigned char	facestyle;
	int				zoneIndex;
	int				areaIndex;
	float			x;
	float			z;
	float			h;
	float			r;
	char			yLayer;
	int				userIndex;
	int				guildoutdate;
	char			plusEffect;
	unsigned short	mapAttr;

	int				key;
	int				currentTitle;

#ifdef NO_CHATTING
	int				chatingFlag;
#endif

#ifdef ENABLE_SUBJOB
	int				jobSub;
#endif

	//int				adminLevel;
};

struct elementalStatus : public pTypeBase
{
	int				charIndex;
	char			charType;
	int				remainTime;
	int				hp;
	int				maxHp;
	char			attackSpeed;
	char			magicSpeed;
	int				skillSpeed;
	float			walkSpeed;
	float			runSpeed;
	float			attackRange;
	float			x;
	float			z;
	float			h;
	float			r;
	char			yLayer;
	unsigned short  mapAttr;

	tag_assistinfo	assistinfo;
};

struct expedInfoForChar : public pTypeBase
{
	int				charIndex;
	int				group;
	int				level;
	int				hp;
	int				maxHp;
	int				mp;
	int				maxMp;
	float			x;
	float			z;
	char			yLayer;
	int				zoneIndex;
	bool			isOnline;
};

struct partyInfo : public pTypeBase
{
	int				charIndex;
	int				level;
	int				hp;
	int				maxHp;
	int				mp;
	int				maxMp;
	float			x;
	float			z;
	char			yLayer;
	int				zoneIndex;
	bool			isOnline;
};

struct attackPetStatus : public pTypeBase
{
	int				charIndex;
	char			charName[MAX_CHAR_NAME_LENGTH + 1];
	int				level;
	int				protoType;
	bool			isMount;
	char			transSate;

	int				coolTime;
	LONGLONG		accExp;

	LONGLONG		exp;
	LONGLONG		needExp;
	int				hp;
	int				maxHp;
	int				mp;
	int				maxMp;
	int				faith;
	int				maxFaith;
	int				stamina;
	int				maxStamina;
	int				str;
	int				addStr;
	int				con;
	int				addCon;
	int				dex;
	int				addDex;
	int				intl;
	int				addIntl;
	int				remainStat;
	int				attack;
	int				magicAttack;
	int				defence;
	int				maxDefence;
	int				hitPoint;
	int				avoid;
	float			x;
	float			z;
	float			h;
	float			r;
	char			yLayer;
	unsigned short	mapAttr;
};

struct charStatusPc : public pTypeBase
{
	int			charIndex;
	int			hp;
	int			maxHp;
	int			mp;
	int			maxMp;
	int			pkPenalty;
	char		pkName;
	LONGLONG	state;
	LONGLONG	state2;

	int			adminLevel;
};

struct charStatusNpc : public pTypeBase
{
	int			charIndex;
	int			hp;
	int			maxHp;
	int			mp;
	int			maxMp;
	LONGLONG	state;
	LONGLONG	state2;

	int			ownerIndex;
	int			classIndex;
	int			melee;
	int			defence;
	int			magic;
	int			resist;
};

struct charStatusPet : public pTypeBase
{
	int			charIndex;
	int			hp;
	int			maxHp;
	int			mp;
	int			maxMp;
	LONGLONG	state;
	LONGLONG	state2;
};

struct charStatusElemental : public pTypeBase
{
	int			charIndex;
	int			hp;
	int			maxHp;
	int			mp;
	int			maxMp;
	LONGLONG	state;
	LONGLONG	state2;
};

struct charDamage : public pTypeBase
{
	char		damageType;

	char		charType;
	char		targetType;
	int			charIndex;
	int			targetIndex;

	int			skillIndex;
	int			targetHp;
	int			targetMp;
	int			damage;
	char		attackSpeed;
	char		flag;
	int			holyitemIndex;
};

struct charDisappear : public pTypeBase
{
	char		charType;
	int			charIndex;
};

struct charDisappearForClientEffect : public pTypeBase
{
	int			charIndex;
	int			owner_index;
};

struct charDisappearAll : public pTypeBase
{
};

struct targetClear : public pTypeBase
{
	int targetIndex;
};

struct attackPetSmallStatus : public pTypeBase
{
	int				charIndex;
	char			charName[MAX_CHAR_NAME_LENGTH + 1];
	int				level;
	int				str;
	int				con;
	int				dex;
	int				intl;

	int				coolTime;
	LONGLONG		accExp;
};

struct LevelupInfo : public pTypeBase
{
	int level;
};

struct charHpInfo : public pTypeBase
{
	int charIndex;
	int hp;
	int maxHp;
};

struct holyWaterState : public pTypeBase
{
	int charIndex;
	int holyWaterItemIndex;
};

struct AttrItemUse : public pTypeBase
{
	char state_type;			//0 NONE, 1 ATTACK, 2DEFENCE
	unsigned char attr;			//attribute
};

struct AttrNpcStateInfo : public pTypeBase
{
	int npc_index;
	char state_type;			//0 NONE, 1 ATTACK, 2DEFENCE
	unsigned char attr;			//attribute
};

struct GuildRoomRecall : public pTypeBase
{
};

#ifndef _CLIENT_
inline void makeOldTimerItem(CNetMsg::SP& msg, int mempos, int stashext)
{
	oldTimerItem* packet = reinterpret_cast<oldTimerItem*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_OLD_TIMER_ITEM;
	packet->memposTime = mempos;
	packet->stashextTime = stashext;
	msg->setSize(sizeof(oldTimerItem));
}

inline void makeStashPasswordFlag(CNetMsg::SP& msg, unsigned char flag)
{
	stashPasswordFlag* packet = reinterpret_cast<stashPasswordFlag*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_STASH_PASSWORD_FLAG;
	packet->flag = flag;
	msg->setSize(sizeof(stashPasswordFlag));
}

inline void makeLevelupInfo(CNetMsg::SP& msg, int level)
{
	LevelupInfo* packet = reinterpret_cast<LevelupInfo*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_LEVELUP_INFO;
	packet->level = level;
	msg->setSize(sizeof(LevelupInfo));
}
inline void CharHpInfoMsg(CNetMsg::SP& msg, int charIndex, int maxHp, int hp)
{
	charHpInfo* packet = reinterpret_cast<charHpInfo*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_TARGET_HP_INFO;
	packet->charIndex = charIndex;
	packet->maxHp = maxHp;
	packet->hp = hp;

	msg->setSize(sizeof(charHpInfo));
}
inline void holyWaterStateMsg(CNetMsg::SP& msg, int charIndex, int holyWaterItemIndex)
{
	holyWaterState* packet = reinterpret_cast<holyWaterState*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_HOLYWATER_STATE;
	packet->charIndex = charIndex;
	packet->holyWaterItemIndex = holyWaterItemIndex;
	msg->setSize(sizeof(holyWaterState));
}
inline void AttrItemUseMsg(CNetMsg::SP& msg, int type, unsigned char attr)
{
	AttrItemUse* packet = reinterpret_cast<AttrItemUse*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_ATTR_ITEM_USE;	//127
	packet->state_type = type;
	packet->attr = attr;
	msg->setSize(sizeof(AttrItemUse));
}
inline void AttrNpcStateMsg(CNetMsg::SP& msg, int type, unsigned char attr, int npc_index)
{
	AttrNpcStateInfo* packet = reinterpret_cast<AttrNpcStateInfo*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_ATTR_STATE_FOR_NPC;	//128
	packet->state_type = type;
	packet->attr = attr;
	packet->npc_index = npc_index;
	msg->setSize(sizeof(AttrNpcStateInfo));
}
inline void GuildRoomRecallRequestMsg(CNetMsg::SP& msg)
{
	GuildRoomRecall* packet = reinterpret_cast<GuildRoomRecall*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_REQUEST_GUILD_RECALL;
	msg->setSize(sizeof(GuildRoomRecall));
}
#endif
}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif

