#include "StdH.h"

#include <Engine/Entities/SkillTree.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Entities/Skill.h>
#include <Engine/Util.h>

#define DEF_EXCLUSION_SKILL_MAX	(8)
#define DEF_2ND_JOB_MAX			(3)	// 0은 전직 안한 상태.

stSkillTreeRow* CSkillTree::m_SkillTree[9][3];
int	CSkillTree::m_nSkillCount[9][3];

void CSkillTree::Destroy()
{
	int i, j;
	for( i = 0; i < TOTAL_JOB; ++i )
	{
		for( j = 0; j < 3; ++j )
		{
			SAFE_ARRAY_DELETE(m_SkillTree[i][j]);
		}
	}
}

int CSkillTree::LoadSkillTreeDataFromFile(const char* FileName)
{
	FILE *fp		= NULL;
	if ((fp = fopen(FileName, "rb")) == NULL) 
	{
		MessageBox(NULL, "File is not Exist.", "error!", MB_OK);
		return -1;
	}

	fflush(fp);

	int	i, j;
	int iLastSkillIndex	= 0; //스킬 갯수.
	int iLength		= -1;
	int iReadBytes	= 0;

	//////////////////////////////////////////////////////////////////////////	
	// MACRO DEFINITION
	//////////////////////////////////////////////////////////////////////////	
#define LOADINT(d)			iReadBytes = fread(&d, sizeof(int), 1, fp);

	int count = 0;

	for( i = 0; i < TOTAL_JOB; ++i )
	{
		for( j = 0; j < DEF_2ND_JOB_MAX; ++j )
		{
			LOADINT(count);

			m_SkillTree[i][j] = new stSkillTreeRow[count];
			m_nSkillCount[i][j] = DEF_SKILL_COL * count;

			iReadBytes = fread(m_SkillTree[i][j], sizeof(stSkillTreeRow) * count, 1, fp);

			if( iReadBytes <= 0 )
			{
				fclose(fp);
				return iReadBytes;
			}
		}
	}

#undef LOADINT

	fclose(fp);
	
#ifdef G_THAI
	ChangeExclusionData();
#endif // G_THAI

#ifdef VER_TEST
	ErrDataOutput();
#endif // VER_TEST

	return iReadBytes;
}

#ifdef G_THAI
void CSkillTree::ChangeExclusionData()
{
	int i, j, idx, exclusion;

	int nExclusionSkill[DEF_EXCLUSION_SKILL_MAX] = {1086, 1087, 1088, 1089, 1090, 1091, 1538, 1698};

	for (i = 0; i < TOTAL_JOB; ++i)
	{
		for (j = 0; j < DEF_2ND_JOB_MAX; ++j)
		{
			for (idx = 0; idx < m_nSkillCount[i][j]; ++idx)
			{
				for (exclusion = 0; exclusion < DEF_EXCLUSION_SKILL_MAX; ++exclusion)
				{
					if (m_SkillTree[i][j][idx / DEF_SKILL_COL].index[idx % DEF_SKILL_COL] == nExclusionSkill[exclusion])
					{
#ifdef VER_TEST
						CSkill	&rSelSkill = _pNetwork->GetSkillData( m_SkillTree[i][j][idx/DEF_SKILL_COL].index[idx%DEF_SKILL_COL] );
						LOG_DEBUG("[%s[%d]] 제거.", rSelSkill.GetName(), m_SkillTree[i][j][idx/DEF_SKILL_COL].index[idx%DEF_SKILL_COL]);
#endif // VER_TEST
						m_SkillTree[i][j][idx / DEF_SKILL_COL].index[idx % DEF_SKILL_COL] = 0;
					}
				}
			}
		}
	}
}
#endif // G_THAI

#ifdef VER_TEST
void CSkillTree::ErrDataOutput()
{
	int idx;
	int i, j;

	CJobInfo* pInfo = CJobInfo::getSingleton();

	if (pInfo == NULL)
		return;

	for(i = 0; i < TOTAL_JOB; ++i)
	{
		for (j = 0; j < DEF_2ND_JOB_MAX; ++j)
		{
			for( idx = 0; idx < m_nSkillCount[i][j]; ++idx )
			{
				if ( m_SkillTree[i][j][idx/8].index[idx%8] <= 0 )
					continue;
	
				CSkill	&rSelSkill = _pNetwork->GetSkillData( m_SkillTree[i][j][idx/8].index[idx%8] );
	
				if( rSelSkill.GetJob() != i || rSelSkill.GetJob2() != j )
				{
					LOG_DEBUG("[%s[%d]]의 직업이 [%s]로 설정 되어있습니다.",
						rSelSkill.GetName(), m_SkillTree[i][j][idx/8].index[idx%8], pInfo->GetName(rSelSkill.GetJob(), rSelSkill.GetJob2()) );
				}
			}
		}
	}
}
#endif // VER_TEST