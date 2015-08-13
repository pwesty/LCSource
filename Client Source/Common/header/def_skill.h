
#ifndef		SKILL_DATA_H_
#define		SKILL_DATA_H_

#include "def_base.h"

#define		DEF_SKILL_DEFAULT_LENGTH 40
#define		SSKILL_MAX_LEVEL	5	// Special Skill 최대 레벨

#pragma pack(push, 1)
struct stSkill : public stTbl_base
{
	int		job;
	int		job2;
	char	type;
	int		flag;
	char	maxLevel;
	float	appRange;
	float	fireRange;
	float	fireRange2;
	char	targetType;
	int		useState;
	int		useWeaponType0;
	int		useWeaponType1;
	int		useMagicIndex1;
	char	useMagicLevel1;
	int		useMagicIndex2;
	char	useMagicLevel2;
	int		useMagicIndex3;
	char	useMagicLevel3;
	int		appState;
	int		readyTime;
	int		stillTime;
	int		fireTime;
	int		reuseTime;
	int		sorcererflag;
	int		apet_index;
};

struct stSkillLevel
{
	int needHP;
	int needMP;
	int needGP;
	int durtime;
	int dummypower;
	int needItemIndex1;
	int needItemCount1;
	int needItemIndex2;
	int needItemCount2;
	int learnLevel;
	int learnSP;
	int learnSkillIndex1;
	char learnSkillLevel1;
	int learnSkillIndex2;
	char learnSkillLevel2;
	int learnSkillIndex3;
	char learnSkillLevel3;
	int learnItemIndex1;
	int learnItemCount1;
	int learnItemIndex2;
	int learnItemCount2;
	int learnItemIndex3;
	int learnItemCount3;
	int appMagicIndex1;
	char appMagicLevel1;
	int appMagicIndex2;
	char appMagicLevel2;
	int appMagicIndex3;
	char appMagicLevel3;
	int magicIndex1;
	char magicLevel1;
	int magicIndex2;
	char magicLevel2;
	int magicIndex3;
	char magicLevel3;
	int learnstr;
	int learndex;
	int learnint;
	int learncon;
	int learnGP;
	int targetNum;

	char attack_subtype;
	char attack_power;
	char defense_subtype;
	char defense_power;
};

struct stSkillClientDataBase
{
	char cd_fot;
	float cd_fos;
	float cd_ox;
	float cd_oz;
	float cd_oh;
	char cd_oc;
	char cd_fdc;
	float cd_fd0;
	float cd_fd1;
	float cd_fd2;
	float cd_fd3;
	float cd_dd;
	int client_icon_texid;
	int client_icon_row;
	int client_icon_col;	
	char cd_fot2;
	float cd_fos2;
	float cd_ox2;
	float cd_oz2;
	float cd_oh2;
	char cd_oc2;
	char cd_fdc2;
	float cd_fd4;
	float cd_fd5;
	float cd_fd6;
	float cd_fd7;
	float cd_dd2;
	int cd_soul_consum;
};

struct stSkillClientDataTool : public stSkillClientDataBase
{
	char cd_ra[DEF_SKILL_DEFAULT_LENGTH];
	char cd_re[DEF_SKILL_DEFAULT_LENGTH];
	char cd_sa[DEF_SKILL_DEFAULT_LENGTH];
	char cd_fa[DEF_SKILL_DEFAULT_LENGTH];
	char cd_fe0[DEF_SKILL_DEFAULT_LENGTH];
	char cd_fe1[DEF_SKILL_DEFAULT_LENGTH];
	char cd_fe2[DEF_SKILL_DEFAULT_LENGTH];
	char cd_ra2[DEF_SKILL_DEFAULT_LENGTH];
	char cd_re2[DEF_SKILL_DEFAULT_LENGTH];
	char cd_sa2[DEF_SKILL_DEFAULT_LENGTH];
	char cd_fa2[DEF_SKILL_DEFAULT_LENGTH];
	char cd_fe3[DEF_SKILL_DEFAULT_LENGTH];
	char cd_fe4[DEF_SKILL_DEFAULT_LENGTH];
	char cd_fe5[DEF_SKILL_DEFAULT_LENGTH];
	char cd_fe_after[DEF_SKILL_DEFAULT_LENGTH];
	char cd_fe_after2[DEF_SKILL_DEFAULT_LENGTH];

	stSkillClientDataTool() { Clear(); }
	void Clear() { memset(this, 0, sizeof(*this)); }
	void CopyFrom(stSkillClientDataTool* src) { memcpy(this, src, sizeof(*this)); }
};

struct stSkillClientData : public stSkillClientDataBase
{
	std::string strCd_ra;
	std::string strCd_re;
	std::string strCd_sa;
	std::string strCd_fa;
	std::string strCd_fe0;
	std::string strCd_fe1;
	std::string strCd_fe2;
	std::string strCd_ra2;
	std::string strCd_re2;
	std::string strCd_sa2;
	std::string strCd_fa2;
	std::string strCd_fe3;
	std::string strCd_fe4;
	std::string strCd_fe5;
	std::string strCd_fe_after;
	std::string strCd_fe_after2;

	void CopyFrom(stSkillClientDataTool* src) {
		memcpy(this, src, sizeof(stSkillClientDataBase));

		strCd_ra			= src->cd_ra;
		strCd_re			= src->cd_re;
		strCd_sa			= src->cd_sa;
		strCd_fa			= src->cd_fa;
		strCd_fe0			= src->cd_fe0;
		strCd_fe1			= src->cd_fe1;
		strCd_fe2			= src->cd_fe2;
		strCd_ra2			= src->cd_ra2;
		strCd_re2			= src->cd_re2;
		strCd_sa2			= src->cd_sa2;
		strCd_fa2			= src->cd_fa2;
		strCd_fe3			= src->cd_fe3;
		strCd_fe4			= src->cd_fe4;
		strCd_fe5			= src->cd_fe5;
		strCd_fe_after		= src->cd_fe_after;
		strCd_fe_after2		= src->cd_fe_after2;
	}
};

struct stSpecailSkill : public stTbl_base
{
	int		type;
	int		maxLevel;
	int		preference;
	int		needLevel[SSKILL_MAX_LEVEL];
	int		needSP[SSKILL_MAX_LEVEL];
	int		needSSkill;
	int		needSSkillLevel;
	int		textureID;
	int		textureRow;
	int		textureCol;	
};

struct stSSkillClient : public stSpecailSkill
{
	char	name[50];
	char	desc[255];

	void CopyFrom(stSpecailSkill* src) {
		memcpy(this, src, sizeof(stSpecailSkill));
	}
};
#pragma pack(pop)
#endif		// SKILL_DATA_H_