304
%{
#include "StdH.h"
#include "EntitiesMP/TacticsHolder.h"
#include <Engine/GlobalDefinition.h>
#include <Engine/Network/MessageDefine.h>
%}

uses "EntitiesMP/Enemy";
uses "EntitiesMP/EnemyBase";
uses "EntitiesMP/BasicEffects";

enum EnemySpawnerType {
  0 EST_SIMPLE          "Simple",           // spawns on trigger
  1 EST_RESPAWNER       "Respawner",        // respawn after death
  2 EST_DESTROYABLE     "Destroyable",      // spawns untill killed
  3 EST_TRIGGERED       "Triggered",        // spawn one group on each trigger
  4 EST_TELEPORTER      "Teleporter",       // teleport the target instead copying it - usable only once
  5 EST_RESPAWNERBYONE  "OBSOLETE - Don't use!",  // respawn only one (not entire group) after death
  6 EST_MAINTAINGROUP   "MaintainGroup",    // respawn by need to maintain the number of active enemies
  7 EST_RESPAWNGROUP    "RespawnerByGroup", // respawn the whole group when it's destroyed
  8 EST_NPCAPPEARACTION "Appear view action" // npc appear action is viewing type
};

event ENetSpawnEntity {
  ULONG ulEntityID,
  BOOL  bCopy,
  FLOAT3D vPosition,
  ANGLE3D vOrientation,
};

class CEnemySpawner: CRationalEntity {
name      "Enemy Spawner";
thumbnail "Thumbnails\\EnemySpawner.tbn";
features  "HasName", "HasTarget", "IsTargetable", "HasRaidObject", "RaidEvent";

properties:

/*** Export Data***/
	3 CTString m_strName          "Name" 'N' = "Enemy spawner",
	200 BOOL	m_bRaidObject	"This entity is RaidObject" = FALSE,		// 레이드 오브젝트 설정
	201 INDEX m_RaidEventType		"Raid Event Type" = 0,			// 레이드 이벤트 타입
/******************/
  1 CEntityPointer m_penTarget  "Template Target" 'T'  COLOR(C_BLUE|0x20),        // template entity to duplicate
  2 CTString m_strDescription = "",
  
  6 RANGE m_fInnerCircle        "Circle inner" 'V' = 0.0f,    // inner circle for creation
  7 RANGE m_fOuterCircle        "Circle outer" 'B' = 0.0f,    // outer circle for creation
  9 FLOAT m_tmDelay             "Delay initial" 'W' = 0.0f,      // how long to delay before spawning
 16 FLOAT m_tmSingleWait        "Delay single" 'O' = 0.1f,    // delay inside one group
  5 FLOAT m_tmGroupWait         "Delay group" 'G' = 0.1f,     // delay between two groups
 17 INDEX m_ctGroupSize         "Count group"  = 1,
  8 INDEX m_ctTotal             "Count total" 'C' = 1 features(EPROPF_NETSEND),        // max. number of spawned enemies
 13 CEntityPointer m_penPatrol  "Patrol target" 'P'  COLOR(C_lGREEN|0xFF),          // for spawning patrolling 
 15 enum EnemySpawnerType m_estType "Type"  'Y' = EST_SIMPLE,      // type of spawner
 18 BOOL m_bTelefrag "Telefrag" 'F' = FALSE,                  // telefrag when spawning
 19 BOOL m_bSpawnEffect "SpawnEffect" 'S' = TRUE, // show effect and play sound
 20 BOOL m_bDoubleInSerious "Double in serious mode" = FALSE,
 21 CEntityPointer m_penSeriousTarget  "Template for Serious"  COLOR(C_RED|0x20),
 22 BOOL m_bFirstPass = TRUE features(EPROPF_NETSEND),
 
 50 CSoundObject m_soSpawn,    // sound channel
 51 INDEX m_iInGroup=0 features(EPROPF_NETSEND),        // in group counter for loops
 52 INDEX m_iEnemiesTriggered=0 features(EPROPF_NETSEND),  // number of times enemies triggered the spawner on death

 60 CEntityPointer m_penTacticsHolder  "Tactics Holder",
 61 BOOL m_bTacticsAutostart           "Tactics autostart" = TRUE,
//강동민 수정 시작 싱글 던젼 작업	07.23
 80	INDEX	m_iMobType				   "Mob Type"	= 0,
 81 BOOL	m_bAngleFix					"Angle Fix" = FALSE,
 82 BOOL	m_bShowSpawnEffect		"Show Spawn Effect" = FALSE,
 83 CTString m_strAnimation			"Entity action animation" = "",
//강동민 수정 끝 싱글 던젼 작업		07.23
	{
		CEntityPointer m_penAction;
	}

 

components:

  1 editor model   MODEL_ENEMYSPAWNER     "Data\\Models\\Editor\\EnemySpawner.mdl",
  2 editor texture TEXTURE_ENEMYSPAWNER   "Data\\Models\\Editor\\EnemySpawner.tex",
  3 class   CLASS_BASIC_EFFECT  "Classes\\BasicEffect.ecl",


functions:

  void ~CEnemySpawner(void)
  {
	  ReleaseChche(ECT_CLASS, CLASS_BASIC_EFFECT);
  }

  void Precache(void)
  {
    PrecacheClass(CLASS_BASIC_EFFECT, BET_TELEPORT);
  }


	const CTString &GetDescription(void) const
	{
		((CTString&)m_strDescription).PrintF("-><none>");
	
		if (m_penTarget!=NULL) 
		{
			((CTString&)m_strDescription).PrintF("->%s", m_penTarget->GetName());
			if (m_penSeriousTarget!=NULL) 
			{
				((CTString&)m_strDescription).PrintF("->%s, %s", 
				m_penTarget->GetName(), m_penSeriousTarget->GetName());
			}
		}
		
		((CTString&)m_strDescription) = EnemySpawnerType_enum.NameForValue(INDEX(m_estType)) + m_strDescription;
		
		return m_strDescription;
	}


	// check if one template is valid for this spawner
	BOOL CheckTemplateValid(CEntity *pen)
	{
		if (pen==NULL || !IsDerivedFromClass(pen, &CEnemyBase_DLLClass)) {
			return FALSE;
		}
		
		if (m_estType==EST_TELEPORTER) {
			return !(((CEnemyBase&)*pen).m_bTemplate);
		} else {
			return ((CEnemyBase&)*pen).m_bTemplate;
		}
	}

	BOOL IsTargetValid(SLONG slPropertyOffset, CEntity *penTarget)
	{
	    if( slPropertyOffset == offsetof(CEnemySpawner, m_penTarget))
	    {
			return CheckTemplateValid(penTarget);
		}
		else if( slPropertyOffset == offsetof(CEnemySpawner, m_penPatrol))
		{
			return (penTarget!=NULL && IsDerivedFromClass(penTarget, &CEnemyMarker_DLLClass));
		}
		else if( slPropertyOffset == offsetof(CEnemySpawner, m_penSeriousTarget))
		{
			return CheckTemplateValid(penTarget);
		}   
		else if( slPropertyOffset == offsetof(CEnemySpawner, m_penTacticsHolder))
		{
			return IsOfClass( penTarget, &CTacticsHolder_DLLClass);
		}   
		
		return CEntity::IsTargetValid(slPropertyOffset, penTarget);
	}


	/* Fill in entity statistics - for AI purposes only */
	BOOL FillEntityStatistics(EntityStats *pes)
	{
	    if (m_penTarget==NULL) { return FALSE; }

	    m_penTarget->FillEntityStatistics(pes);
	    pes->es_ctCount = m_ctTotal;
	    pes->es_strName += " (spawned)";

	    if (m_penSeriousTarget!=NULL) {
		pes->es_strName += " (has serious)";
		}
		return TRUE;
	}

	virtual BOOL IsEnemySpawner(void) const
	{
		return TRUE;
	}

	BOOL EntityIsAnimation(void)
	{
		if (m_penAction != NULL)
		{
			if (!m_penAction->IsAnimationPlaying(ska_GetIDFromStringTable(m_strAnimation)))
			{
				// IdleAnimation으로 전환
				((CEnemyBase*)((CEntity*)m_penAction))->m_bAction = FALSE;
				((CUnit*)((CEntity*)m_penAction))->StandingAnim();
				return FALSE;
			}
		}

		return TRUE;
	}

	void EntityAppearStart(void)
	{
		SetFlagOn(ENF_PROPSCHANGED);

		CPlacement3D plEntity = GetPlacement();
		CMobData* MD = CMobData::getData(m_iMobType);
		
		m_penAction = _pNetwork->ga_World.CreateEntity_t(plEntity, CLASS_ENEMY, -1, TRUE);
		m_penAction->InitAsSkaModel();
		m_penAction->SetSkaModel(MD->GetMobSmcFileName());
		CMobData::SetMobDataToNPC(m_penAction, MD, CMobData::getData(m_iMobType)->GetName());
		m_penAction->Initialize(); //ska_GetIDFromStringTable(m_strAnimation);
		((CEnemyBase*)((CEntity*)m_penAction))->m_bAction = TRUE; // action만 애니메이션 되도록
		((CEnemy*)((CEntity*)m_penAction))->m_idMob_Action = ska_GetIDFromStringTable(m_strAnimation); // 대상 애니메이션 ID 설정
		((CEnemy*)((CEntity*)m_penAction))->m_dwAniMationFlag = AN_REMOVE_ON_END;	// 대상 애니메이션 flag 설정
		m_penAction->SetFlagOn(ENF_INVISIBLE); // Action을 자연스럽게 하기 위해, Action ani 호출시 off해 준다.
	}

	void EntityAppearEnd(void)
	{
		SetFlagOff(ENF_PROPSCHANGED);
		if (m_penAction)
		{
			m_penAction->Destroy(FALSE);
			m_penAction = NULL;
		}
	}

	// spawn new entity
	// 새로운 엔티티 생성.
	void SpawnEntity(BOOL bCopy) 
	{
		SetFlagOn(ENF_PROPSCHANGED);
		if (!_pNetwork->IsServer()) 
		{
			return;
		}
		
		// 싱글 모드 일때...
		if(_pNetwork->m_bSingleMode && m_iMobType != 0 && _cmiComm. IsNetworkOn())
		{
			FLOAT fOuterCircle = ClampDn(m_fOuterCircle, 0.0f);
			FLOAT fInnerCircle = ClampUp(m_fInnerCircle, fOuterCircle);
			
			// calculate new position
			// 새로운 좌표를 계산함.
			FLOAT fR = fInnerCircle + FRnd()*(fOuterCircle-fInnerCircle);
			FLOAT fA = FRnd()*360.0f;
			CPlacement3D pl(FLOAT3D(CosFast(fA)*fR, 0.05f, SinFast(fA)*fR), ANGLE3D(0, 0, 0));
			pl.RelativeToAbsolute(GetPlacement());
			CPlacement3D plSpawner = GetPlacement();
			
			// teleport back
			//pen->Teleport(pl, m_bTelefrag);
			
			CEntity *penEntity = NULL;
			
			penEntity = _pNetwork->ga_World.CreateEntity_t(pl, CLASS_ENEMY, -1, TRUE);
			penEntity->InitAsSkaModel();
			//mt.mob_iClientIndex	= penEntity->en_ulID;
			//penEntity->SetSkaModel(MD->GetMobSmcFileName());				
			
			INDEX iEntityID = penEntity->en_ulID;
			
			CEnemyBase *peb = ((CEnemyBase*)penEntity);
			AddEnemyKillData(peb, this);
			MarkSpawnerSpawned(this);
			
			if (m_estType==EST_RESPAWNER || m_estType==EST_MAINTAINGROUP || m_estType==EST_RESPAWNGROUP) 
			{
				peb->m_penSpawnerTarget = this;
			}
			
			// Mob Respawn Message
			if(_cmiComm. IsNetworkOn())
			{
				CNetworkMessage nmMobSpawn(MSG_NPC_REGEN);
				INDEX iIndex	= -1;
				INDEX iYLayer	= 0;
				INDEX iMobType	= m_iMobType;
				nmMobSpawn << iIndex;						// Index
				nmMobSpawn << iMobType;						// DB Index
				nmMobSpawn << pl.pl_PositionVector(1);		// Pos X
				nmMobSpawn << pl.pl_PositionVector(3);		// Pos Z
				nmMobSpawn << pl.pl_PositionVector(2);		// Pos H
				if(m_bAngleFix)
				{
					nmMobSpawn << plSpawner.pl_OrientationAngle(1);	// Angle
				}
				else
				{
					nmMobSpawn << pl.pl_OrientationAngle(1);	// Angle
				}
				nmMobSpawn << iYLayer;						// Y Layer
				nmMobSpawn << iEntityID;					// EntityID
				_pNetwork->SendToServerNew(nmMobSpawn);

				// spawn teleport effect
				if (m_bShowSpawnEffect) 
				{
					ESpawnEffect ese;
					ese.colMuliplier = C_WHITE|CT_OPAQUE;
					ese.betType = BET_TELEPORT;
					ese.vNormal = FLOAT3D(0,1,0);					
					FLOAT fEntitySize = 3.0f;
					ese.vStretch = FLOAT3D(fEntitySize, fEntitySize, fEntitySize);
					CEntityPointer penEffect = CreateEntity(pl, CLASS_BASIC_EFFECT, WLD_AUTO_ENTITY_ID,FALSE);
					penEffect->Initialize(ese,FALSE);
				}
			}
		}
		// 싱글 모드가 아닐때...		
		else
		{
			extern BOOL _bWorldEditorApp;
			if(m_iMobType != 0 && _bWorldEditorApp)
			{
				CEntity *penEntity;
				CMobData* MD = CMobData::getData(m_iMobType);

				FLOAT fOuterCircle = ClampDn(m_fOuterCircle, 0.0f);
				FLOAT fInnerCircle = ClampUp(m_fInnerCircle, fOuterCircle);

				// calculate new position
				// 새로운 좌표를 계산함.
				FLOAT fR = fInnerCircle + FRnd()*(fOuterCircle-fInnerCircle);
				FLOAT fA = FRnd()*360.0f;
				CPlacement3D pl(FLOAT3D(CosFast(fA)*fR, 0.05f, SinFast(fA)*fR), ANGLE3D(0, 0, 0));
				pl.RelativeToAbsolute(GetPlacement());

				penEntity			= _pNetwork->ga_World.CreateEntity_t(pl, CLASS_ENEMY, -1, TRUE);
				penEntity->InitAsSkaModel();

				// FIXME : NPC 리젠과 중복되는 부분.
				penEntity->SetSkaModel(MD->GetMobSmcFileName());
			
				CMobData::SetMobDataToNPC(penEntity, MD, CMobData::getData(m_iMobType)->GetName());

				// 생성될때 애니메이션이 필요한 에너미들...
				const int iWanderingZombie	= 35;		// 방황하는 좀비
				const int iZombie			= 51;		// 좀비
				const int iDamd				= 142;		// 뎀드(스펠링 맞나???)
				// FIXME : 임시적으로 작업한 부분.
				if( m_iMobType == iWanderingZombie || m_iMobType == iZombie || m_iMobType == iDamd )		// 좀비 & 뎀드
				{
					CEntityProperty &epPropertyStart = *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 99));	// Walk Animation
					ENTITYPROPERTY( &*penEntity, epPropertyStart.ep_slOffset, INDEX)		= ska_GetIDFromStringTable("m_zm_up01");
				}
				penEntity->Initialize();

				CEnemyBase *peb = ((CEnemyBase*)penEntity);
				AddEnemyKillData(peb, this);
				MarkSpawnerSpawned(this);

				if (m_estType==EST_RESPAWNER || m_estType==EST_MAINTAINGROUP || m_estType==EST_RESPAWNGROUP) 
				{
					peb->m_penSpawnerTarget = this;
				}
			}
			else
			{
				if (((CEntity*)m_penTarget) == NULL) 
				{
					return;
				}

				// spawn new entity if of class basic enemy
				if (CheckTemplateValid(m_penTarget)) 
				{
					CEntity *pen = NULL;
					
					if (bCopy) 
					{
						// copy template entity
						// 엔티티를 복사해서...
						pen = GetWorld()->CopyEntityInWorld( *m_penTarget,
						CPlacement3D(FLOAT3D(-32000.0f+FRnd()*200.0f, -32000.0f+FRnd()*200.0f, 0), ANGLE3D(0, 0, 0)),TRUE,WLD_AUTO_ENTITY_ID,FALSE );

						// change needed properties
						pen->End();
						CEnemyBase *peb = ((CEnemyBase*)pen);

						AddEnemyKillData(peb, this);
						MarkSpawnerSpawned(this);

						peb->m_bTemplate = FALSE;
						if (m_estType==EST_RESPAWNER /*|| m_estType==EST_RESPAWNERBYONE*/|| m_estType==EST_MAINTAINGROUP || m_estType==EST_RESPAWNGROUP) 
						{
							peb->m_penSpawnerTarget = this;
						}
						
						if (m_penPatrol!=NULL) 
						{
							peb->m_penMarker = m_penPatrol;
						}
						
						pen->Initialize(EVoid(),FALSE);
					} 
					else 
					{
						pen = m_penTarget;
						m_penTarget = NULL;
					}
      
					// adjust circle radii to account for enemy size
					FLOAT fEntityR = 0;
					if (pen->en_pciCollisionInfo!=NULL) 
					{
						fEntityR = pen->en_pciCollisionInfo->GetMaxFloorRadius();
					}
					
					FLOAT fOuterCircle = ClampDn(m_fOuterCircle-fEntityR, 0.0f);
					FLOAT fInnerCircle = ClampUp(m_fInnerCircle+fEntityR, fOuterCircle);

					// calculate new position
					// 새로운 좌표를 계산함.
					FLOAT fR = fInnerCircle + FRnd()*(fOuterCircle-fInnerCircle);
					FLOAT fA = FRnd()*360.0f;
					CPlacement3D pl(FLOAT3D(CosFast(fA)*fR, 0.05f, SinFast(fA)*fR), ANGLE3D(0, 0, 0));
					pl.RelativeToAbsolute(GetPlacement());
//강동민 수정 시작 싱글 던젼 작업	07.29
					if(m_bAngleFix)
					{
						CPlacement3D plSpawner = GetPlacement();
						pl.pl_OrientationAngle(1) = plSpawner.pl_OrientationAngle(1);
					}
//강동민 수정 끝 싱글 던젼 작업		07.29

					// teleport back
					pen->Teleport(pl, m_bTelefrag);

					// spawn teleport effect
					if (m_bSpawnEffect) 
					{
						ESpawnEffect ese;
						ese.colMuliplier = C_WHITE|CT_OPAQUE;
						ese.betType = BET_TELEPORT;
						ese.vNormal = FLOAT3D(0,1,0);
						FLOATaabbox3D box;
						pen->GetBoundingBox(box);
						FLOAT fEntitySize = box.Size().MaxNorm()*2;
						ese.vStretch = FLOAT3D(fEntitySize, fEntitySize, fEntitySize);
						CEntityPointer penEffect = CreateEntity(pl, CLASS_BASIC_EFFECT,WLD_AUTO_ENTITY_ID,FALSE);
						penEffect->Initialize(ese,FALSE);
					}

					  // tell clients to spawn an entity with a given id
					  ENetSpawnEntity eNetSpawn;
					  eNetSpawn.ulEntityID = pen->en_ulID;
					  eNetSpawn.bCopy = bCopy;
					  eNetSpawn.vPosition = pl.pl_PositionVector;
					  eNetSpawn.vOrientation = pl.pl_OrientationAngle;
					  SendEvent(eNetSpawn,TRUE);
			    }    
			}
		}
	};

	// spawn new entity
	void SpawnEntity_net(ENetSpawnEntity eNetSpawn) 
	{
		SetFlagOn(ENF_PROPSCHANGED);
		
		if (_pNetwork->IsServer()) 
		{
			return;
		}
		
		if (m_penSeriousTarget!=NULL && GetSP()->sp_gdGameDifficulty==CSessionProperties::GD_EXTREME && GetSP()->sp_bSinglePlayer) {
			m_penTarget = m_penSeriousTarget;
		}
		
		if (!CheckTemplateValid(m_penTarget)) 
		{
			return;
		}
		
		CEntity *pen = NULL;
		CPlacement3D pl(eNetSpawn.vPosition,eNetSpawn.vOrientation);

		if (eNetSpawn.bCopy) 
		{
			// copy template entity
			pen = GetWorld()->CopyEntityInWorld( *m_penTarget, pl, TRUE, eNetSpawn.ulEntityID,FALSE );

			// change needed properties
			pen->End();
			CEnemyBase *peb = ((CEnemyBase*)pen);
			peb->m_bTemplate = FALSE;
			if (m_estType==EST_RESPAWNER /*|| m_estType==EST_RESPAWNERBYONE*/|| m_estType==EST_MAINTAINGROUP || m_estType==EST_RESPAWNGROUP) 
			{
				peb->m_penSpawnerTarget = this;
			}
			
			if (m_penPatrol!=NULL) {
				peb->m_penMarker = m_penPatrol;
			}
			
			pen->Initialize(EVoid(),FALSE);
		} 
		else 
		{
			pen = m_penTarget;
			m_penTarget = NULL;
		}
    
		// spawn teleport effect
		if (m_bSpawnEffect) 
		{
			ESpawnEffect ese;
			ese.colMuliplier = C_WHITE|CT_OPAQUE;
			ese.betType = BET_TELEPORT;
			ese.vNormal = FLOAT3D(0,1,0);
			FLOATaabbox3D box;
			pen->GetBoundingBox(box);
			FLOAT fEntitySize = box.Size().MaxNorm()*2;
			ese.vStretch = FLOAT3D(fEntitySize, fEntitySize, fEntitySize);
			CEntityPointer penEffect = CreateEntity(pl, CLASS_BASIC_EFFECT,WLD_AUTO_ENTITY_ID,FALSE);
			penEffect->Initialize(ese,FALSE);
		}
	};


	// Handle an event, return false if the event is not handled
	BOOL HandleEvent(const CEntityEvent &ee)
	{
	    if (ee.ee_slEvent==EVENTCODE_ETrigger)
		{
			ETrigger eTrigger = ((ETrigger &) ee);
			if( IsDerivedFromClass( eTrigger.penCaused, &CEnemyBase_DLLClass) && (m_estType==EST_MAINTAINGROUP || m_estType==EST_RESPAWNGROUP || 
				m_estType==EST_RESPAWNER || m_estType==EST_TRIGGERED)) 
			{
				// Respawn By Group인 경우 죽은 에너미의 갯수를 카운트 하는듯...
				m_iEnemiesTriggered++;
			}
		} 
		else if (ee.ee_slEvent==EVENTCODE_ENetSpawnEntity) 
		{
			if (!_pNetwork->IsServer()) 
			{
				ENetSpawnEntity eNetSpawn = ((ENetSpawnEntity &) ee);
				SpawnEntity_net(eNetSpawn);
			}
		}

		return CRationalEntity::HandleEvent(ee);
	}


	// returns bytes of memory used by this object
	SLONG GetUsedMemory(void)
	{
		// initial
		SLONG slUsedMemory = sizeof(CEnemySpawner) - sizeof(CRationalEntity) + CRationalEntity::GetUsedMemory();
		// add some more
		slUsedMemory += m_strDescription.Length();
		slUsedMemory += m_strName.Length();
		slUsedMemory += 1* sizeof(CSoundObject);
		return slUsedMemory;
	}

procedures:
	Main() {
		// init as nothing
		InitAsEditorModel();
		SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
		SetCollisionFlags(ECF_IMMATERIAL);

		// set appearance
		SetModel(MODEL_ENEMYSPAWNER);
		SetModelMainTexture(TEXTURE_ENEMYSPAWNER);

		SetFlagOff(ENF_PROPSCHANGED);
		SetFlagOn(ENF_MARKDESTROY);
		SetFlagOn(ENF_NONETCONNECT);

		m_penAction = NULL;

		if (m_tmSingleWait<=0.0f) { m_tmSingleWait=0.05f; }
		if (m_tmGroupWait<=0.0f) { m_tmGroupWait=0.05f; }
    
		// set range
		if (m_fInnerCircle > m_fOuterCircle) {
		  m_fInnerCircle = m_fOuterCircle;
		}

		if (m_estType==EST_RESPAWNERBYONE) {
		  m_estType=EST_MAINTAINGROUP;
		}

	    // check target
		if (m_penTarget!=NULL) {
			if( !IsDerivedFromClass( m_penTarget, &CEnemyBase_DLLClass)) {
				WarningMessage("Target '%s' is of wrong class!", m_penTarget->GetName());
				m_penTarget = NULL;
			}
		}
		
		if (m_penSeriousTarget!=NULL) {
			if( !IsDerivedFromClass(m_penSeriousTarget, &CEnemyBase_DLLClass)) {
				WarningMessage("Target '%s' is of wrong class!", m_penSeriousTarget->GetName());
				m_penSeriousTarget = NULL;
			}
		}

		// never start ai in wed
		autowait(_pTimer->TickQuantum);

		// destroy self if this is a multiplayer-only spawner, and flags indicate no extra enemies
		if ( !GetSP()->sp_bUseExtraEnemies && !GetSP()->sp_bSinglePlayer && !(GetSpawnFlags()&SPF_SINGLEPLAYER)) {
			Destroy();
			return;
		}

		if (m_bDoubleInSerious && GetSP()->sp_gdGameDifficulty==CSessionProperties::GD_EXTREME && GetSP()->sp_bSinglePlayer) {
			m_ctGroupSize*=2;
			m_ctTotal*=2;
		}
		
		if (m_penSeriousTarget!=NULL && GetSP()->sp_gdGameDifficulty==CSessionProperties::GD_EXTREME && GetSP()->sp_bSinglePlayer) {
			m_penTarget = m_penSeriousTarget;
		}

		if (m_estType==EST_MAINTAINGROUP) {
			m_iEnemiesTriggered = m_ctGroupSize;
		}

		m_bFirstPass = TRUE;

		wait() {
			on(EBegin) : {
				SetFlagOn(ENF_PROPSCHANGED);

				if(m_estType==EST_SIMPLE) {
					call Simple();
				} else if(m_estType==EST_TELEPORTER) {
					call Teleporter();
				} else if(m_estType==EST_RESPAWNER /*|| m_estType==EST_RESPAWNERBYONE*/|| m_estType==EST_TRIGGERED || m_estType==EST_RESPAWNGROUP) {
					call Respawner();
				} else if(m_estType==EST_MAINTAINGROUP) {
					m_ctGroupSize = 1;
					call Respawner();
				} else if(m_estType==EST_DESTROYABLE) {
					call Destroyable();
				} else if (m_estType == EST_NPCAPPEARACTION) {
					call SpawnerAppearAction();
				}
			}
			on(EDeactivate) : {
				if (_pNetwork->IsServer()) {
					stop;
				} else {
					resume;
				}
			}
			on(EStop) : {
				if (_pNetwork->IsServer()) {
					stop;
				} else {
					resume;
				}
			}
			on(EEnd) : {
				if (_pNetwork->IsServer()) {
					stop;
				} else {
					resume;
				}
			}
		}

		Destroy();
		return;
	};

	// spawn one group of entities
	SpawnGroup() 
	{
		SetFlagOn(ENF_PROPSCHANGED);
		// no enemies in group yet
		m_iInGroup = 0;
		// repeat forever
		
		while(TRUE) 
		{
			// spawn one enemy
			SpawnEntity(TRUE);

			// count total enemies spawned
			m_ctTotal--;
			// if no more left
			if (m_ctTotal<=0) 
			{
				// finish entire spawner
				return EEnd();
			}

			// count enemies in group
			m_iInGroup++;
			// decrease the needed count
			
			if (m_iEnemiesTriggered>0 && m_estType==EST_RESPAWNGROUP) 
			{
				if (!m_bFirstPass) 
				{
					m_iEnemiesTriggered--;
				}
			} 
			else if (m_iEnemiesTriggered>0) 
			{
				m_iEnemiesTriggered--;
			}

			// if entire group spawned
			if (m_iInGroup>=m_ctGroupSize) 
			{
				if (!(m_estType==EST_MAINTAINGROUP && m_iEnemiesTriggered>0)) 
				{
					// finish
					return EReturn();
				}
			}

			// wait between two entities in group
			wait(m_tmSingleWait) {
				on (EBegin) : { resume; }
				on (ETimer) : { stop; }
				otherwise() : { pass; }
			}
		}
	}

	// simple spawner
	Simple()
	{
	    // wait to be triggered
	    wait() {
			on (EBegin) : { resume; }
			on (ETrigger) : { stop; };
			on (EStart) : { stop; };
			otherwise() : { pass; }
		}

		// if should delay
		if (m_tmDelay>0) {
			// wait delay
			autowait(m_tmDelay);
		}

		// repeat
		while(TRUE) {
			// spawn one group
			autocall SpawnGroup() EReturn;
			// delay between groups
			autowait(m_tmGroupWait);
		}
	}

	// teleports the template
	Teleporter()
	{
	    // wait to be triggered
	    wait() {
			on (EBegin) : { resume; }
			on (ETrigger) : { stop; };
			on (EStart) : { stop; };
			otherwise() : { pass; }
		}

		// if should delay
		if (m_tmDelay>0) {
			// wait delay
			autowait(m_tmDelay);
		}

		// teleport it
		SpawnEntity(FALSE);

		// end the spawner
		return EEnd();
	}

	// respawn enemies when killed
	Respawner()
	{
		// repeat
		while(TRUE) 
		{
			// wait to be triggered
			wait() 
			{
				on (EBegin) : 
				{				 
					if (!m_bFirstPass && m_iEnemiesTriggered>0) 
					{
						stop;
					}
					resume;
				}
				on (ETrigger) : { stop; };
				on (EStart) : { stop; };
				otherwise() : { pass; }
			}
     
			// if should delay - only once, on beginning
			if (m_tmDelay>0 && m_bFirstPass) {
				// initial delay
				autowait(m_tmDelay);
			}

			if (m_estType==EST_RESPAWNGROUP) {
				if (m_bFirstPass) {
					autocall SpawnGroup() EReturn;
				} else if (m_iEnemiesTriggered>=m_ctGroupSize) {
					if (m_tmGroupWait>0) { autowait(m_tmGroupWait); }
					autocall SpawnGroup() EReturn;
				}
			} else if (TRUE) {
				// spawn one group
				if (m_tmGroupWait>0 && !m_bFirstPass) { autowait(m_tmGroupWait); }
				autocall SpawnGroup() EReturn;
			}

			// if should continue respawning by one
			/*if (m_estType==EST_RESPAWNERBYONE) {
				// set group size to 1
				if (m_tmGroupWait>0 && !m_bFirstPass) { autowait(m_tmGroupWait); }
				m_ctGroupSize = 1;
			}*/

			// if should continue maintaining group
			if (m_estType==EST_MAINTAINGROUP) {
				// set group size to 1
				m_ctGroupSize = 1;
			}

			// never do an initial delay again - set FirstPass to FALSE
			m_bFirstPass = FALSE;

			// wait a bit to recover
			autowait(0.1f);
		}
	}

	DestroyableInactive()
	{
		waitevent() EActivate;
		jump DestroyableActive();
	}

	DestroyableActiveSpawning()
	{
		// repeat
		while(TRUE) {
			// spawn one group
			autocall SpawnGroup() EReturn;
			// delay between groups
			autowait(m_tmGroupWait);
		}
	}

	DestroyableActive()
	{
		autocall DestroyableActiveSpawning() EDeactivate;
		jump DestroyableInactive();
	}

	// spawn new entities until you are stopped
	Destroyable()
	{
		// start in inactive state and do until stopped
		autocall DestroyableInactive() EStop;
		// finish
		return EEnd();
	}

	SpawnerAppearAction()
	{
		wait()
		{
			on (EBegin) : {
				EntityIsAnimation();
				resume;
			}
			on (EStart) : {
				// 출현 액션을 처리한다.
				EntityAppearStart();
				resume;
			}
			on (EStop) : {
				// 출현 액션 처리한 엔티티 및 데이터를 제거 하고 끝
				EntityAppearEnd();
				stop;
			}
		}
		return EEnd();
	}
};
