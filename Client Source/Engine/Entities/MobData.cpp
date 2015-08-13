#include "stdh.h"

#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Entities/MobData.h>
#include <Engine/Entities/EntityClass.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Ska/StringTable.h>
#include <Engine/World/World.h>
#include <Engine/Network/CNetwork.h> // WSS_DRATAN_SEIGEWARFARE 2007/08/08
#include <Engine/Interface/UIManager.h>
#include <Engine/Secure/FileSecure.h>	// [2012/07/18 : Sora]  파일 보안코드 추가
/*
 *  Constructor.
 */
CMobData::CMobData(void)
	: mob_ulZoneFlag(0)
	, mob_ulExtraFlag(0)
{
	mob_bBoss		= FALSE;
	Owner_Index	= -1;

	channel_flag = (short)0xFFFF;
}

/*
 *  Destructor.
 */
CMobData::~CMobData(void) 
{
	mob_ulZoneFlag	= 0;
	mob_ulExtraFlag	= 0;
}

CTString CMobData::GetAnimAttackNameByID(int iAttackID)
{
	ASSERT(iAttackID > 0 && "Invalid Attack Animation Number");

	// === Attack Animation 규칙 ===
	// Attack##		--> ##는 숫자.
	// 예) Attack01, Attack02...
	CTString strAttackName;
	strAttackName.PrintF(TRANS("%s%02d"), GetAnimAttackName(), iAttackID);
	return strAttackName;
}

bool CMobData::LoadZoneData( const char* ZNFileName )
{
	int iReadBytes = 0;
#define LOADINT(d)			iReadBytes = fread(&d, sizeof(int), 1, fp);
#define LOADSHORT(d)		iReadBytes = fread(&d, sizeof(short), 1, fp);
#define LOADCHAR(d)			iReadBytes = fread(&d, sizeof(char), 1, fp);
#define LOADFLOAT(d)		iReadBytes = fread(&d, sizeof(float), 1, fp);
#define LOADSTR(d)			{ int iLen; LOADINT(iLen); iReadBytes = fread(&d, iLen, 1, fp); }
	// 존 이동 플래그 정보를 화일로부터 읽어들임.
	if(ZNFileName)
	{
		FILE *fp		= NULL;
		if ((fp = fopen(ZNFileName, "rb")) == NULL) 
		{
			MessageBox(NULL, "File is not Exist.", "error!", MB_OK);
			return false;
		}
		int nCount = 0;
		int nIndex;
		LOADINT(nCount);
		ASSERT(nCount > 0 && "Invalid Zone Move Npc Data");

		for (int i = 0; i < nCount; ++i)
		{
			iReadBytes = fread(&nIndex,	sizeof(int), 1, fp);
			
			CMobData* pData = CMobData::getData(nIndex);
			
			if (pData == NULL)
				return false;

			iReadBytes = fread(&pData->mob_ulZoneFlag,	sizeof(ZONEFLAGS), 1, fp);	// Zone Flag
			iReadBytes = fread(&pData->mob_ulExtraFlag,	sizeof(ZONEFLAGS), 1, fp);	// Extra Flag			
		}

		fclose(fp);
	}
	//////////////////////////////////////////////////////////////////////////	
#undef LOADINT
#undef LOADCHAR
#undef LOADFLOAT
#undef LOADSTR

	return true;
}


bool CMobData::loadEx(const char* FileName)
{
	FILE*	fp = NULL;

	fp = fopen(FileName, "rb");

	if (fp == NULL)
		return false;

	fread(&_nSize, sizeof(int), 1, fp);

	if (_nSize <= 0)
	{
		fclose(fp);
		return false;
	}

	stNpc* pdata = new stNpc[_nSize];
	fread(pdata, sizeof(stNpc) * _nSize, 1, fp);
	fclose(fp);

	for (int i = 0; i < _nSize; i++)
	{
		CMobData* ptmp = new CMobData;
		memcpy(ptmp, &pdata[i], sizeof(stNpc));
		if (_mapdata.insert(std::make_pair(ptmp->getindex(), ptmp)).second == false)
		{
			delete ptmp;
			ptmp = NULL;
		}

		_vecdata.push_back(ptmp);
	}

	m_dummy = new CMobData; // 더미데이타 생성
	
	memset(m_dummy, 0, sizeof(stNpc));

	if (pdata != NULL)
	{
		delete[] pdata;
		pdata = NULL;
	}

	SetSDNpc();

	return true;
}

bool CMobData::LoadChannelData( const char* FileName )
{
	FILE *fp		= NULL;

	fp = fopen(FileName, "rb");

	if (fp == NULL)
		return false;

	int nSize = 0;

	fread(&nSize, sizeof(int), 1, fp);

	if (nSize > 0)
	{
		struct stChannel
		{
			int npcIndex;
			int channel[16];
		};

		stChannel* pChannel = new stChannel[nSize];
		fread(pChannel, sizeof(stChannel) * nSize, 1, fp);
		fclose(fp);

		for (int i = 0; i < nSize; ++i)
		{
			CMobData* pMD = CMobData::getData(pChannel[i].npcIndex);

			if (pMD == NULL)
				continue;

			short channel = 0;

			for (int j = 0; j < 16; ++j)
			{
				if (pChannel[i].channel[j] > 0)
					channel |= (pChannel[i].channel[j] << j);
			}

			pMD->SetChannelFlag(channel);
		}

		SAFE_ARRAY_DELETE(pChannel);
	}
	else
	{
		fclose(fp);

		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : penEntity - 
//			MD - 
// Output : int
//-----------------------------------------------------------------------------
int CMobData::SetMobDataToNPC(CEntity* penEntity, CMobData* MD, const char* szMobName)
{
	if (MD == NULL)
		return -1;

	ASSERT(penEntity != NULL && "Invalid Entity Pointer!");
	ASSERT(MD->GetMobIndex() != -1 && "Invalid Mob Index!");
	
	if(MD->IsPeaceful())
	{
		penEntity->SetFirstExtraFlagOn(ENF_EX1_NPC);
		penEntity->SetFirstExtraFlagOn(ENF_EX1_PEACEFUL);
	}

	if(MD->IsZoneMoving())	
	{
		// WSS_DRATAN_SIEGEWARFARE 2007/10/14 -------------------------------->>
		BOOL bSkip = FALSE;
		if( MD->IsCastleTower() )
		{
			// 워프 타워의 경우 공성측의 경우 일반 몹으로 처리함
			if(!(_pNetwork->MyCharacterInfo.sbJoinFlagDratan == WCJF_OWNER ||
				 _pNetwork->MyCharacterInfo.sbJoinFlagDratan == WCJF_DEFENSE_GUILD) )
			{
				bSkip = TRUE;				
			}
		}
		// -------------------------------------------------------------------<<

		if( !bSkip )
		{
			// 존 이동 장치 플래그가 켜져있는 경우, 존 이동 플래그를 넣어줌.
			penEntity->SetFlagOn(ENF_ZONEMOVING);
			penEntity->SetFirstExtraFlagOn(ENF_EX1_NPC);

			CEntityProperty &epPropertyZoneFlag	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_ZONEFLAGS_EX, 237));		// Zone Flag
			ENTITYPROPERTY( &*penEntity, epPropertyZoneFlag.ep_slOffset, ZONEFLAGS)		= MD->GetZoneMovingFlag();

			CEntityProperty &epPropertyExtraFlag	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_ZONEFLAGS_EX, 238));	// Extra Flag
			ENTITYPROPERTY( &*penEntity, epPropertyExtraFlag.ep_slOffset, ZONEFLAGS)	= MD->GetExtraFlag();
		}
	}
	else if(MD->IsResource())							// 생산물품
	{
		// 광물은 반드시 NPC가 아니어야함.
		penEntity->SetFirstExtraFlagOn(ENF_EX1_PRODUCTION);
		penEntity->SetFirstExtraFlagOff(ENF_EX1_NPC);
	}
	else if( MD->IsSkillMaster() || MD->IsSSkillMaster() || MD->IsNPC() )		// NPC
	{
		penEntity->SetFirstExtraFlagOn(ENF_EX1_NPC);
	}
	else
	{
		penEntity->SetFlagOn(ENF_ALIVE);
	}

	CEntityProperty &epPropertyNpcIndex	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 91));	// Enemy Index
	ENTITYPROPERTY( &*penEntity, epPropertyNpcIndex.ep_slOffset, INDEX)		= MD->GetMobIndex();

	// NOTE : 리젠 타임도 복사해줘야 함(월드 에디터에서만 필요함)
	//CEntityProperty &epPropertyNpcIndex	= pdecDLLClass->dec_aepProperties[3];		// Regen Time
	//ENTITYPROPERTY( &*penEntity, epPropertyNpcIndex.ep_slOffset, INDEX)		= MD->GetIndex();

	CEntityProperty &epPropertyAlready	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_BOOL, 250));	// Initialized?
	ENTITYPROPERTY( &*penEntity, epPropertyAlready.ep_slOffset, BOOL)	= TRUE;
	
	//-----------------------------------------------------------------------------
	// 애니메이션 정보를 설정함.
	//-----------------------------------------------------------------------------
	CEntityProperty &epPropertyWalk		= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 100));	// Walk Animation
	ENTITYPROPERTY( &*penEntity, epPropertyWalk.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(MD->GetAnimWalkName());
	
	CEntityProperty &epPropertyAttack	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 101));	// Attack Animation
	ENTITYPROPERTY( &*penEntity, epPropertyAttack.ep_slOffset, INDEX)	= ska_GetIDFromStringTable(MD->GetAnimAttackName());
	
	CEntityProperty &epPropertyIdle		= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 102));	// Idle Animation
	ENTITYPROPERTY( &*penEntity, epPropertyIdle.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(MD->GetAnimDefaultName());
	
	CEntityProperty &epPropertyDeath	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 103));	// Death Animation
	ENTITYPROPERTY( &*penEntity, epPropertyDeath.ep_slOffset, INDEX)	= ska_GetIDFromStringTable(MD->GetAnimDeathName());
	
	CEntityProperty &epPropertyWound	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 104));	// Wound Animation
	ENTITYPROPERTY( &*penEntity, epPropertyWound.ep_slOffset, INDEX)	= ska_GetIDFromStringTable(MD->GetAnimWoundName());
	
	CEntityProperty &epPropertyBox		= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 105));	// Normal Box
	ENTITYPROPERTY( &*penEntity, epPropertyBox.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(MD->GetNormalBoxName());

	CEntityProperty &epPropertyRun		= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 107));	// Run Animation
	ENTITYPROPERTY( &*penEntity, epPropertyRun.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(MD->GetAnimRunName());

	CEntityProperty &epPropertyStretch	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 5));	// Model stretch Scale
	ENTITYPROPERTY( &*penEntity, epPropertyStretch.ep_slOffset, FLOAT)	= MD->GetScale();
	
	CEntityProperty &epPropertyAttack2	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 106));	// Attack Animation 2
	ENTITYPROPERTY( &*penEntity, epPropertyAttack2.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(MD->GetAnimAttack2Name());

	CEntityProperty &epPropertyIdle2		= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 108));	// Idle Animation
	ENTITYPROPERTY( &*penEntity, epPropertyIdle2.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(MD->GetAnimDefault2Name());

	CEntityProperty &epPropertyAttack3	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 109));	// Attack Animation 2
	ENTITYPROPERTY( &*penEntity, epPropertyAttack3.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(MD->GetAnimAttack3Name());

	CEntityProperty &epPropertyAttack4	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 110));	// Attack Animation 2
	ENTITYPROPERTY( &*penEntity, epPropertyAttack4.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(MD->GetAnimAttack4Name());

	CEntityProperty &epPropertyName		= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_STRING, 200));	// Mob Name
	ENTITYPROPERTY( &*penEntity, epPropertyName.ep_slOffset, CTString)	= szMobName;

	// FIXME : 싱글던젼에서만 쓰임...ㅡ.ㅡ
	if( MD->IsBoss() )
	{
		CEntityProperty &epPropertyBoss	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_BOOL, 160));	// Is Boss Mob?
		ENTITYPROPERTY( &*penEntity, epPropertyBoss.ep_slOffset, BOOL)	= MD->IsBoss();
	}
	
	CDLLEntityClass *pdecDLLBaseClass	= penEntity->GetClass()->ec_pdecDLLClass->dec_pdecBase;
	CDLLEntityClass *pdecDLLBaseBaseClass	= pdecDLLBaseClass->dec_pdecBase;

	CEntityProperty &epPropertyWalkSpeed		= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 10);	// NPC Walk Speed
	ENTITYPROPERTY( &*penEntity, epPropertyWalkSpeed.ep_slOffset, FLOAT)			= MD->GetWalkSpeed();	

	CEntityProperty &epPropertyAttackRunSpeed	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 12);	// NPC Attack Run Speed
	ENTITYPROPERTY( &*penEntity, epPropertyAttackRunSpeed.ep_slOffset, FLOAT)		= MD->GetRunSpeed();

	CEntityProperty &epPropertyCloseRunSpeed	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 14);	// NPC Close Run Speed
	ENTITYPROPERTY( &*penEntity, epPropertyCloseRunSpeed.ep_slOffset, FLOAT)		= MD->GetRunSpeed();	

	CEntityProperty &epPropertyAttackDistance	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 20);	// Attack Distance
	ENTITYPROPERTY( &*penEntity, epPropertyAttackDistance.ep_slOffset, FLOAT)		= MD->GetAttackDistance() * 1.5f;

	CEntityProperty &epPropertyHealth	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 143);	// NPC Health
	ENTITYPROPERTY( &*penEntity, epPropertyHealth.ep_slOffset, FLOAT)		= MD->GetHealth();
	
	// SetMobDataToNPC 전에 SetMobData()에서 Entity의 현재 HP가 갱신이 되어 있습니다.
	//((CLiveEntity*)penEntity)->SetHealth(MD->GetHealth());
	// --> MD가 아닌, Entity의 HP를 갱신합시다.
	CEntityProperty &epPropertyCurHealth	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 202);	// NPC Health
	INDEX MobHp = ENTITYPROPERTY( &*penEntity, epPropertyCurHealth.ep_slOffset, INDEX);

	((CLiveEntity*)penEntity)->SetHealth(MobHp);

	if (MD->GetMobIndex() == LORD_SYMBOL_INDEX) // 권좌는 hp따라 모델이 다르다. 현재 모델이 어떤 타입인지만 설정한다. 이 시점에서 모델은 이미 정해져 있다.
	{ // type -1 (사용하지 않는 m_nPlayActionNum 변수를 type으로 사용한다.)

		CEntityProperty &epPropertyPlayActionNum = *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 224);	// NPC PlayerActionNum

		if (MobHp <= MD->GetHealth() * 0.25f) // type 0
		{
			ENTITYPROPERTY( &*penEntity, epPropertyPlayActionNum.ep_slOffset, INDEX) = 0;
		}
		else if (MobHp <= MD->GetHealth() * 0.50f) // type 1
		{
			ENTITYPROPERTY( &*penEntity, epPropertyPlayActionNum.ep_slOffset, INDEX) = 1;
		}
		else if (MobHp <= MD->GetHealth() * 0.75f) // type 2
		{
			ENTITYPROPERTY( &*penEntity, epPropertyPlayActionNum.ep_slOffset, INDEX) = 2;
		}
	}

	float fStopDistance		= MD->GetAttackDistance() - 0.1f;
	float fCloseDistance	= MD->GetAttackDistance();
	if(fStopDistance < 0.0f)	fStopDistance	= 0.0f;
	if(fCloseDistance < 0.0f)	fCloseDistance	= 0.0f;

	CEntityProperty &epPropertyStopDistance		= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 24);	// Attack Distance
	ENTITYPROPERTY( &*penEntity, epPropertyStopDistance.ep_slOffset, FLOAT)			= fStopDistance;

	CEntityProperty &epPropertyCloseDistance	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 21);	// Attack Distance
	ENTITYPROPERTY( &*penEntity, epPropertyCloseDistance.ep_slOffset, FLOAT)		= fCloseDistance;
	
	return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *  Constructor.
 */

void CMobData::SetNoTranslate()
{
	char buff[MAX_PATH];
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->IsNotTranslated( TRANS_NAME, transFlag ) )
	{
		sprintf( buff, "[%d] : npc name", index );
		SetName(buff);
	}

	if( pUIManager->IsNotTranslated( TRANS_DESC, transFlag ) )
	{
		sprintf( buff, "[%d] : npc desc", index );
		SetDesc(buff);
	}
}

void CMobData::ClearNoTranslate()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->IsNotTranslated( TRANS_NAME, transFlag ) )
		name = "";
	if( pUIManager->IsNotTranslated( TRANS_DESC, transFlag ) )
		desc = "";
}

void CMobData::SetTranslateFlag( int flag )
{
	transFlag |= flag;
}

// WSS_DRATAN_SEIGEWARFARE 2007/08/08 ---------------------------------------->>
// 드라탄 마스터 타워 관려 수정...
// NPC로 되면 말표시가 되고 관련 창을 생성할 수 있고, NPC가 아니면 
// CheckTarget을 통해 공격여부를 판단할 수 있다 
BOOL CMobData::IsNPC() const
{
	// 제외 사항 설정
	// NPC_CASTLE_TOWER가 설정되어 있으면 기본적으로 NPC로 설정하지 않음
	// 공성의 경우 수성과 공성에 따라 설정값이 다르기 때문에...
	if( (flag&NPC_CASTLE_TOWER) )
	{
		int tRes = IsMyQuarter();
		if (tRes != QUARTER_NONE )
		{
			if ( tRes == QUARTER_INSTALL || tRes == QUARTER_MINE) return TRUE;
			else if ( tRes == QUARTER_ATTACK ) return FALSE; 
		}
		if (_pNetwork->MyCharacterInfo.sbJoinFlagDratan!=WCJF_OWNER)
			return FALSE;
		else return TRUE;	
	}		
	else 
		return (flag & NPC_MASK);	
}
// ---------------------------------------------------------------------------<<

// WSS_DRATAN_SEIGEWARFARE 2007/08/13------------------------>>
int CMobData::IsMyQuarter() const
{
	std::map<int,int>::const_iterator tFound	= _pNetwork->MyCharacterInfo.mQuarter.find(index);
	
	if( tFound == _pNetwork->MyCharacterInfo.mQuarter.end() ) return QUARTER_NONE; // 부활진지가 아님
	// 두번 조회 필요없음.  [2/24/2011 rumist]
	int  tIdx = tFound->second;
	if (tIdx==-1)
		return QUARTER_INSTALL;	// 설치	
	else if ( tIdx == _pNetwork->MyCharacterInfo.lGuildIndex)
		return QUARTER_MINE;	// 나의 진지

	return QUARTER_ATTACK;		// 공격
}

void CMobData::SetSDNpc()
{
	CMobData::_map::iterator	iter = CMobData::_mapdata.begin();
	CMobData::_map::iterator	eiter = CMobData::_mapdata.end();

	for (;iter != eiter; ++iter)
	{
		CMobData* pNpc = (*iter).second;

		if (pNpc == NULL)
			continue;

		CTString strAttackAnim;
		// FIXME : 죄다 하드 코딩~~~~
		// FIXME : 구울만 공격 애니메이션이 4개여서...
		// FIXME : 매직넘버 제거!!!
		// 구울의 경우.
		if( pNpc->getindex() == 193 )
		{			
			strAttackAnim.PrintF( "%s_att%02d", "ghoulworker", 3 );
			strcpy( pNpc->motion_Attack3, strAttackAnim );
			strAttackAnim.PrintF( "%s_att%02d", "ghoulworker", 4 );
			strcpy( pNpc->motion_Attack4, strAttackAnim );
		}
		else if( pNpc->getindex() == 194 )
		{
			strAttackAnim.PrintF( "%s_att_%02d", "ghoulsucker", 3 );
			strcpy( pNpc->motion_Attack3, strAttackAnim );
			strAttackAnim.PrintF( "%s_att_%02d", "ghoulsucker", 4 );
			strcpy( pNpc->motion_Attack4, strAttackAnim );
		}
		else if( pNpc->getindex() == 195 )
		{
			strAttackAnim.PrintF( "%s_att%02d", "ghoulanker", 3 );
			strcpy( pNpc->motion_Attack3, strAttackAnim );
			strAttackAnim.PrintF( "%s_att%02d", "ghoulanker", 4 );
			strcpy( pNpc->motion_Attack4, strAttackAnim );
		}
		else
		{
			// FIXME : 싱글던젼을 위해서만 필요한 부분....ㅡ.ㅡ
			strcpy( pNpc->motion_Attack3, pNpc->motion_Attack );
			strcpy( pNpc->motion_Attack4, pNpc->motion_Attack2 );
		}

		// FIXME : 보스 몹인지 인덱스로 확인 후에 플래그를 켜줌...ㅡ.ㅡ		
		// NOTE : 201 -> 싱글던젼에 보스 몹 카미라.
		if( pNpc->getindex() == 201 )
		{
			pNpc->mob_bBoss	= TRUE;
		}

		// stl 은 배열도 정렬할 수 있다. 아래와 같이 코드 수정. [2/17/2011 rumist]
		std::sort( pNpc->fireDelay, pNpc->fireDelay+pNpc->fireDelayCount);
	}
}


// ----------------------------------------------------------<<
