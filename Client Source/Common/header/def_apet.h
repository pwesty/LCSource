
#ifndef		APET_DATA_H_
#define		APET_DATA_H_

#include "def_base.h"

#define DEF_SMCFILE_LENGTH 64
#define DEF_APET_ANI_LENGTH 32
#define DEF_APET_NAME_LENGTH 20

#define DEF_MAX_EVOLUTION	4
#define DEF_MAX_ACCEXP		1

#pragma pack(push, 1)

struct stEvolutionData
{
	int	evLevel;
	int	stm;
	int faith;
	int evPetIdx;

	stEvolutionData()
	{
		evLevel = 0;
		stm = 0;
		faith = 0;
		evPetIdx = 0;
	}
};

struct stAccExpData
{
	int nMaxAccParam1;
	int nMaxAccParam2;
	int nAccRateParam1;
	int nAccRateParam2;

	stAccExpData()
	{
		nMaxAccParam1 = 0;
		nMaxAccParam2 = 0;
		nAccRateParam1 = 0;
		nAccRateParam2 = 0;
	}
};

struct stApet : public stTbl_base
{
	char	name[DEF_APET_NAME_LENGTH];
	int		type;
	int		nItemIndex;
	int		nAISlot;
	int		nMount[2];
	int		nSummonSkill[2];
	int		nFlag;
	char	smcFile[2][DEF_SMCFILE_LENGTH];
	char	strIdle1[2][DEF_APET_ANI_LENGTH];
	char	strIdle2[2][DEF_APET_ANI_LENGTH];
	char	strAttack1[2][DEF_APET_ANI_LENGTH];
	char	strAttack2[2][DEF_APET_ANI_LENGTH];
	char	strDamage[2][DEF_APET_ANI_LENGTH];
	char	strDie[2][DEF_APET_ANI_LENGTH];
	char	strWalk[2][DEF_APET_ANI_LENGTH];
	char	strRun[2][DEF_APET_ANI_LENGTH];
	char	strLevelup[2][DEF_APET_ANI_LENGTH];
	stEvolutionData	evdata[DEF_MAX_EVOLUTION];
	stAccExpData	accexpData[DEF_MAX_ACCEXP];

	void SetEvolution(int pos, stEvolutionData* pData)
	{
		if (pData == NULL)
			return;

		if (pos < 0 || pos >= DEF_MAX_EVOLUTION)
			return;

		evdata[pos].evLevel = pData->evLevel;
		evdata[pos].stm = pData->stm;
		evdata[pos].faith = pData->faith;
		evdata[pos].evPetIdx = pData->evPetIdx;
	}

	void SetAccexp(int pos, stAccExpData* pData)
	{
		if (pData == NULL)
			return;

		if (pos < 0 || pos >= DEF_MAX_ACCEXP)
			return;

		accexpData[pos].nMaxAccParam1 = pData->nMaxAccParam1;
		accexpData[pos].nMaxAccParam2 =	pData->nMaxAccParam2;
		accexpData[pos].nAccRateParam1 = pData->nAccRateParam1;
		accexpData[pos].nAccRateParam2 =	 pData->nAccRateParam2;
	}
};

#pragma pack(pop)
#endif		// APET_DATA_H_