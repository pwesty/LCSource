#ifndef __PTYPE_APPEAR_H__
#define __PTYPE_APPEAR_H__

#include "ptype_base.h"
#include "ptype_char_status.h"

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace UpdateClient
{
struct appearPc : public pTypeBase
{
	bool			bNew;
	char			charType;
	int				charIndex;
	char			charName[MAX_CHAR_NAME_LENGTH + 1];
	unsigned char	job;
	unsigned char	job2;
	char			hairStyle;
	char			faceStyle;
	float			x;
	float			z;
	float			h;
	float			r;
	char			yLayer;
	int				hp;
	int				maxHp;
	int				playerState;
	int				pkPenalty;
	int				pkName;
	char			plusEffect;

	int				costume2[8];

	struct tag_wearingshow
	{
		int			dbIndex;
		int			plus;
	} wearingShowList[WEARING_BOOTS + 1];

	struct tag_backwing
	{
		int			dbIndex;
		int			plus;
	} backwing;

	int				accessory[3];

	struct tag_personalshop
	{
		char		type;
		char		name[PS_MAX_SHOPNAME + 1];
	} personalshop;

	struct tag_guildinfo
	{
		int				index;
		char			name[MAX_GUILD_NAME_LENGTH + 1];
		unsigned short	pos;
		char			guildMark;
#ifdef DEV_GUILD_MARK
		char			guildMark_row;
		char			guildMark_col;
		char			guildMark_bg_row;
		char			guildMark_bg_col;
#endif
	} guildinfo;

	int				changeIndex;
	int				fame;
	unsigned short  mapAttr;
	char			merac_join_flag;
	char			dratan_join_flag;

	struct tag_pet
	{
		char		grade;
		char		color;
	} pet;

	int				evocationIndex;
	bool			isPlayerStateSupporter;
	char			pressCorps;
	char			expedLabel;
	int				currentTitle;
	int				mountApetIndex;
	int				petStashManagerEffect;

	struct tag_syndicate
	{
		char		type;
		char		grade;
	} syndicate;

#ifdef PREMIUM_CHAR
	bool			isPremiumChar;			// true = preimum char
#endif

	struct tag_assistinfo	assistInfo;
};

struct appearNpc : public pTypeBase
{
	bool			bNew;
	char			charType;
	int				charVirtualIndex;
	int				charDbIndex;
	float			x;
	float			z;
	float			h;
	float			r;
	char			yLayer;
	int				hp;
	int				maxHp;
	unsigned char	attrdef;
	unsigned char	attratt;
	unsigned short	mapAttr;
	int				expedLabel;
	int				ownerIndex;
	float			mercenarySize;

	struct tag_assistinfo	assistInfo;
};

struct appearPet : public pTypeBase
{
	bool			bNew;
	char			charType;
	int				charIndex;
	int				ownerIndex;
	char			ownerName[MAX_CHAR_NAME_LENGTH + 1];
	char			grade;
	float			x;
	float			z;
	float			h;
	float			r;
	char			yLayer;
	int				hp;
	int				maxHp;
	unsigned short	mapAttr;
	char			name[MAX_CHAR_NAME_LENGTH + 1];
	char			color;
	int				turnToNpc;
	int				turnToNpcSize;
};

struct appearElemental : public pTypeBase
{
	bool			bNew;
	char			charType;
	int				charIndex;
	char			elementalType;
	int				ownerIndex;
	char			ownerName[MAX_CHAR_NAME_LENGTH + 1];
	float			x;
	float			z;
	float			h;
	float			r;
	char			yLayer;
	int				hp;
	int				maxHp;
	unsigned short  mapAttr;
	bool			isBigSize;

	struct tag_assistinfo	assistInfo;
};

struct appearApet : public pTypeBase
{
	bool			bNew;
	char			charType;
	int				charIndex;
	int				dbIndex;
	int				level;
	char			name[MAX_CHAR_NAME_LENGTH + 1];
	bool			isMount;
	char			transSate;
	bool			isAction;
	int				ownerIndex;
	char			ownerName[MAX_CHAR_NAME_LENGTH + 1];
	int				protoType;
	float			x;
	float			z;
	float			h;
	float			r;
	char			yLayer;

	struct tag_weapon
	{
		int			dbIndex;
		int			plus;
	} weaponList[APET_WEARPOINT];

	int				hp;
	int				maxHp;
	unsigned short  mapAttr;
};

#ifdef PREMIUM_CHAR
	struct changePremiumCharFlag : public pTypeBase 
	{
		int			charIndex;
		bool		flag;			// true = premium char
	};

#ifndef _CLIENT_
	inline void makeChangePremiumCharFlag(CNetMsg::SP& msg, int charIndex, bool flag)
	{
		UpdateClient::changePremiumCharFlag* packet = reinterpret_cast<UpdateClient::changePremiumCharFlag*>(msg->m_buf);
		packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
		packet->subType = MSG_SUB_UPDATE_PREMIUM_CHAR_CHANGE_FLAG;
		packet->charIndex = charIndex;
		packet->flag = flag;
		msg->setSize(sizeof(UpdateClient::changePremiumCharFlag));
	}
#endif
#endif

}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
