
#ifndef		NPC_DATA_H_
#define		NPC_DATA_H_

#include "def_base.h"

#define DEF_SMC_LENGTH 128
#define DEF_ANI_LENGTH 64

#pragma pack(push, 1)

struct stNpc : public stTbl_base
{
	int level, hp, mp, flag, flag1, attackSpeed;
	float walkSpeed, runSpeed, scale, attackArea, size;
	char skillmaster;
	char sskillmaster;
	int effect[5];
	char attackType;
	char fireDelayCount;
	float fireDelay[4];
	char fireObject;
	float fireSpeed;
	int skill0_index;
	char skill0_level;
	int skill1_index;
	char skill1_level;
	int	rvr_grade;
	int rvr_value;
	float fBound;
	char fileSMC[DEF_SMC_LENGTH];
	char motion_Idle[DEF_ANI_LENGTH];
	char motion_Walk[DEF_ANI_LENGTH];
	char motion_Dam[DEF_ANI_LENGTH];
	char motion_Attack[DEF_ANI_LENGTH];
	char motion_Die[DEF_ANI_LENGTH];
	char motion_Run[DEF_ANI_LENGTH];
	char motion_Idle2[DEF_ANI_LENGTH];
	char motion_Attack2[DEF_ANI_LENGTH];

	char fireEffect0[DEF_ANI_LENGTH];
	char fireEffect1[DEF_ANI_LENGTH];
	char fireEffect2[DEF_ANI_LENGTH];
};

struct stNpcHelp : public stTbl_base
{
	int zoneNum;

	// NpcIndex 는 하위 24비트
	int getNpcIndex() {
		return (index & 0x00ffffff);
	}
};

#pragma pack(pop)

#endif		// NPC_DATA_H_