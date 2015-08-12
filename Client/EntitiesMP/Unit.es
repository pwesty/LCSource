10653
%{
#include "StdH.h"
%}

enum wPetAIState {
	0	WPET_AI_NONE	"",
	1	WPET_AI_STOP	"",
	2	WPET_AI_ACTION		"",
	3	WPET_AI_NORMALATTACK	"",
	4	WPET_AI_USESKILL	"",
	5	WPET_AI_FOLLOW	"",
};

enum EventType
{
	0 EVENT_NONE		"None",			// 아무것도...
	1 EVENT_MOBCREATE	"Mob Created",	// 몹이 생성되고 나서
	2 EVENT_MOBDESTROY	"Mob Destroyed",// 몹이 죽고 나서.
	3 EVENT_MOBCLICK	"Mob Clicked",	// 몹을 클릭했을때...
};

enum DestinationType {
	0 DT_PLAYERCURRENT "",    // go to where player is now
	1 DT_PLAYERSPOTTED "",    // go to where player was last seen
	2 DT_PATHTEMPORARY "",    // go to navigation marker - temporary, only until you spot player again
	3 DT_PATHPERSISTENT "",    // go to navigation marker - until you really get there
};

enum TargetType {
	0 TT_NONE "",   // no target
	1 TT_SOFT "",   // soft target - only spoted player but not heavily angry at him
	2 TT_HARD "",   // hard target - player has provoked enemy and it is very angry
};

// NOTE : 주의!!! CEntityPointer를 부모 클래스에 선언하게 되면, 
// NOTE : 컴파일은 되지만, 소멸자에서 문제가 발생함.  원인 모름~~~ㅡ_ㅡ;;;
// 캐릭터, 에너미, 애완동물, 소환수의 Base가 되는 클래스.
class export CUnit: CMovableModelEntity 
{
	name      "Unit";
	thumbnail "";
	features  "HasName", "IsTargetable";
	
properties :	
//	1 CEntityPointer m_penWatcher,      // watcher
	4 enum TargetType m_ttTarget = TT_NONE, // type of target	
	200	CTString		m_strName	" Name" = "Unit",		
	143 FLOAT m_fMaxHealth = -1.0f features(EPROPF_NETSEND),      // must set this because of crosshair colorizing
	201 INDEX			m_nMaxiHealth = 0,
	202 INDEX			m_nCurrentHealth = 0,	
	205 INDEX			m_nMobLevel = 0,
	206 INDEX			m_nPreHealth = -1,				
	209 INDEX			m_nMobDBIndex = -1,				
	95 FLOAT m_fActivityRange "Activity Range" = 0.0f,
	88 RANGE m_fAttackRadius "Radius of attack" 'A' = 10000.0f, // attack sphere range radius
	53 enum DestinationType m_dtDestination =  DT_PLAYERCURRENT, // type of current desired position

	// moving/attack properties - CAN BE SET
	// these following must be ordered exactly like this for GetProp() to function
	10 FLOAT m_fWalkSpeed = 1.0f,                  // walk speed
	11 ANGLE m_aWalkRotateSpeed = AngleDeg(10.0f), // walk rotate speed
	12 FLOAT m_fAttackRunSpeed = 1.0f,                // attack run speed
	13 ANGLE m_aAttackRotateSpeed = AngleDeg(10.0f),  // attack rotate speed
	14 FLOAT m_fCloseRunSpeed = 1.0f,                 // close run speed
	15 ANGLE m_aCloseRotateSpeed = AngleDeg(10.0f),   // close rotate speed
	20 FLOAT m_fAttackDistance = 3.0f,          // attack distance mode
	21 FLOAT m_fCloseDistance = 0.0f,           // close distance mode(Singt로 가정)
	22 FLOAT m_fAttackFireTime = 2.0f,           // attack distance fire time
	23 FLOAT m_fCloseFireTime = 2.0f,            // close distance fire time
	24 FLOAT m_fStopDistance = 0.5f,             // stop moving toward enemy if closer than stop distance
	25 FLOAT m_fIgnoreRange = 50.0f,            // cease attack if enemy farther
	133 FLOAT m_fMoveTime = 0.0f,
	55 FLOAT m_fMoveSpeed = 0.0f,
	56 ANGLE m_aRotateSpeed = 0.0f,
	54 FLOAT m_fMoveFrequency = 0.0f,
	17 RANGE m_fSenseRange "Sense Range" = 0.0f,       // immediately spots any player that gets closer than this
	28 FLOAT m_fViewAngle "View angle" 'V' = 360.0f, // view frustum angle for spotting players 
	29 FLOAT m_fFallHeight "Fall height" = 8.0f,
	31 FLOAT m_fStepHeight "Step height" = -1.0f,

	// attack temporary -> DO NOT USE
	60 FLOAT m_fShootTime = 0.0f,                // time when entity will try to shoot on enemy
	61 FLOAT m_fDamageConfused = 0.0f,           // damage amount when entity shoot concentration is spoiled 
	//84 CEntityPointer m_penDeathTarget "Death target" 'D',                 // death target
	//85 enum EventEType m_eetDeathType  "Death event type" 'F' = EET_TRIGGER, // death event type

	40 FLOAT	m_fBlowUpAmount = 0.0f,             // damage in minus for blow up
	41 INDEX	m_fBodyParts = 4,                   // number of spawned body parts
	42 FLOAT	m_fDamageWounded = 0.0f,            // damage amount to be wounded
	224 INDEX	m_nPlayActionNum = -1,
	52 FLOAT3D	m_vDesiredPosition = FLOAT3D(0,0,0), 
//	211 BOOL	m_bRunning = FALSE,
	213	BOOL	m_bSkilling = FALSE,//
	214	INDEX	m_nCurrentSkillNum = -1,		
	43 FLOAT	m_fSkillSpeed = 1.0f,	 
	215 FLOAT	m_fAttackLengthMul = 1.0f,
	216 FLOAT	m_fAttackSpeedMul =  1.0f,	
	217	INDEX	m_idCurrentSkillAnim = -1,
	204 BOOL	m_bAttack = FALSE,

	191 BOOL m_bWasKilled = FALSE features(EPROPF_NETSEND),
	193 BOOL	m_bUseAI			= FALSE,	// 소환수가 알아서 움직임.
	194	BOOL	m_bAIStart			= FALSE,	// AI 작동 시작
	
	156	FLOAT	m_tmSkillStartTime	= 0.0f,
	157	FLOAT   m_tmSkillAnimTime = 0.0f,
	158	FLOAT	m_fSkillAnimTime = 0.0f,
	116 INDEX	m_bKillEnemy = FALSE,

	232 FLOAT3D m_vMyPositionTmp = FLOAT3D(0.0f, 0.0f, 0.0f),	
	112 INDEX	m_enAttackerID = 0,

//---------------------------------------------------------------------------------
	// 플레이어의 소유물인 경우에만 해당.
	117 BOOL	m_bStuned			= FALSE,		// 버프 처리를 위한 부분.
	118 BOOL	m_bHold				= FALSE,
	119 BOOL	m_bCannotUseSkill	= FALSE,
//---------------------------------------------------------------------------------
 {
	CSelectedEntities		m_dcEnemies;
	FLOAT		m_fAttackSpeed;
	ANGLE3D		m_aDesiredRotation;
 }
        
components:
        
functions:

	virtual void OnInitialize(const CEntityEvent &eeInput)
	{
		CMovableModelEntity::OnInitialize(eeInput);
	};

	virtual void OnEnd(void) 
	{		
		m_dcEnemies.Clear();
		CMovableModelEntity::OnEnd();		
	};

	virtual void DeathNow() {}
	virtual void SkillNow() {}	
	virtual void PreSkillNow() {}	
	virtual void AttackNow() {}	
	virtual void ActionNow() {}	
	virtual void SetAttackSpeed(SBYTE attackspeed) {}
	virtual void StopNow() {}
	virtual BOOL SetTargetEntity(CEntity *penPlayer)	{ return TRUE; }
	virtual BOOL SetKillTargetEntity(CEntity *penKill)	{ return FALSE; }
	virtual BOOL SetEnemyEntity(CEntity *penEnemy, int type) { return TRUE; }
	virtual void SetNoTargetEntity(void) {}
	virtual void StopandTeleport() {}
	virtual void MoveNow() {}
	virtual void SendActEvent(void) {}

	virtual BOOL CheckTarget(CEntity* penTarget) { return FALSE; }
	virtual BOOL UseSkill( int iSkillIndex ) { return FALSE; }

	// 이동할수 있는가???
	virtual BOOL IsMovable() { return TRUE; }
	virtual BOOL IsAvailableRide()		{ return FALSE;	}
	virtual void StopMove()
	{
		if( _pNetwork->m_bSingleMode )
		{
			_pNetwork->SendStopMessage(this, GetPlacement());
		}
		m_vDesiredPosition = GetPlacement().pl_PositionVector;//이동하려는 지점을 자신이 서있는 곳으로 셋팅.
		m_vMyPositionTmp = GetPlacement().pl_PositionVector;
		StopRotating();
		StopTranslating();	
	}
	
	// ----------------------------------------------------------------------------
	// Name : 
	// Desc : 공통으로 쓰이는 함수를 빼놓은것...
	// ----------------------------------------------------------------------------

	// overridable function for access to different properties of derived classes (flying/diving)
	virtual FLOAT &GetProp(FLOAT &m_fBase)
	{
		return m_fBase;
	}

	// overridable function to get range for switching to another player
	virtual FLOAT GetThreatDistance(void)
	{
		// closer of close or stop range
		return Max(GetProp(m_fCloseDistance), GetProp(m_fStopDistance));
	}

	// calculate delta to given entity
	FLOAT3D CalcDelta(CEntity *penEntity) 
	{
		ASSERT(penEntity!=NULL);
		// find vector from you to target
		return penEntity->GetPlacement().pl_PositionVector - GetPlacement().pl_PositionVector;
	};

	// calculate distance to given entity
	FLOAT CalcDist(CEntity *penEntity) 
	{
		return CalcDelta(penEntity).Length();
	};

	// calculate delta to given entity in current gravity plane
	FLOAT3D CalcPlaneDelta(CEntity *penEntity) 
	{
		ASSERT(penEntity!=NULL);
		FLOAT3D vPlaneDelta;
		// find vector from you to target in XZ plane
		GetNormalComponent(
			penEntity->GetPlacement().pl_PositionVector - GetPlacement().pl_PositionVector,
			en_vGravityDir, vPlaneDelta);
		return vPlaneDelta;
	};

	// calculate distance to given entity in current gravity plane
	FLOAT CalcPlaneDist(CEntity *penEntity)
	{
		return CalcPlaneDelta(penEntity).Length();
	};

	// get cos of angle in direction
	FLOAT GetFrustumAngle(const FLOAT3D &vDir)
	{
		// find front vector
		FLOAT3D vFront = -GetRotationMatrix().GetColumn(3);
		// make dot product to determine if you can see target (view angle)
		return (vDir/vDir.Length())%vFront;
	}
	
	// get cos of angle in direction in current gravity plane
	FLOAT GetPlaneFrustumAngle(const FLOAT3D &vDir)
	{
		FLOAT3D vPlaneDelta;
		// find vector from you to target in XZ plane
		GetNormalComponent(vDir, en_vGravityDir, vPlaneDelta);
		// find front vector
		FLOAT3D vFront = -GetRotationMatrix().GetColumn(3);
		FLOAT3D vPlaneFront;
		GetNormalComponent(vFront, en_vGravityDir, vPlaneFront);
		// make dot product to determine if you can see target (view angle)
		vPlaneDelta.SafeNormalize();
		vPlaneFront.SafeNormalize();
		return vPlaneDelta%vPlaneFront;
	}

	// determine if you can see something in given direction
	BOOL IsInFrustum(CEntity *penEntity, FLOAT fCosHalfFrustum) 
	{
		// get direction to the entity
		FLOAT3D vDelta = CalcDelta(penEntity);
		// find front vector
		FLOAT3D vFront = -GetRotationMatrix().GetColumn(3);
		// make dot product to determine if you can see target (view angle)
		FLOAT fDotProduct = (vDelta/vDelta.Length())%vFront;
		return fDotProduct >= fCosHalfFrustum;
	};

	// determine if you can see something in given direction in current gravity plane
	BOOL IsInPlaneFrustum(CEntity *penEntity, FLOAT fCosHalfFrustum) 
	{
		// get direction to the entity
		FLOAT3D vPlaneDelta = CalcPlaneDelta(penEntity);
		// find front vector
		FLOAT3D vFront = -GetRotationMatrix().GetColumn(3);
		FLOAT3D vPlaneFront;
		GetNormalComponent(vFront, en_vGravityDir, vPlaneFront);
		// make dot product to determine if you can see target (view angle)
		vPlaneDelta.SafeNormalize();
		vPlaneFront.SafeNormalize();
		FLOAT fDot = vPlaneDelta%vPlaneFront;
		return fDot >= fCosHalfFrustum;
	};

	// cast a ray to entity checking only for brushes
	BOOL IsVisible(CEntity *penEntity) 
	{
		ASSERT(penEntity!=NULL);
		// get ray source and target
		FLOAT3D vSource, vTarget;
		GetPositionCastRay(this, penEntity, vSource, vTarget);

		// cast the ray
		CCastRay crRay(this, vSource, vTarget);
		crRay.cr_ttHitModels = CCastRay::TT_NONE;     // check for brushes only
		crRay.cr_bHitTranslucentPortals = FALSE;
		en_pwoWorld->CastRay(crRay);

		// if hit nothing (no brush) the entity can be seen
		return (crRay.cr_penHit==NULL);     
	};

	// cast a ray to entity checking all
	BOOL IsVisibleCheckAll(CEntity *penEntity) 
	{
		ASSERT(penEntity!=NULL);
		// get ray source and target
		FLOAT3D vSource, vTarget;
		GetPositionCastRay(this, penEntity, vSource, vTarget);

		// cast the ray
		CCastRay crRay(this, vSource, vTarget);
		crRay.cr_ttHitModels = CCastRay::TT_COLLISIONBOX;   // check for model collision box
		crRay.cr_bHitTranslucentPortals = FALSE;
		en_pwoWorld->CastRay(crRay);

		// if the ray hits wanted entity
		return crRay.cr_penHit==penEntity;
	};

	// calc distance to entity in one plane (relative to owner gravity)
	FLOAT CalcDistanceInPlaneToDestination(void) 
	{
		// find vector from you to target in XZ plane
		FLOAT3D vNormal;
		GetNormalComponent(m_vDesiredPosition - GetPlacement().pl_PositionVector, en_vGravityDir, vNormal);
		return vNormal.Length();
	};


	// ----------------------------------------------------------------------------
	// Name : 
	// Desc : 이동과 관련된 공통으로 쓰이는 부분.
	// ----------------------------------------------------------------------------

	// set desired rotation and translation to go/orient towards desired position
	// and get the resulting movement type
	virtual ULONG SetDesiredMovementEx(void) 
	{
		ULONG ulFlags = 0;

		// get delta to desired position
		FLOAT3D vDelta = m_vDesiredPosition - GetPlacement().pl_PositionVector;
		vDelta(2) = 0.0f;

		// if we may rotate
		if (m_aRotateSpeed>0.0f) 
		{
			// get desired heading orientation
			FLOAT3D vDir = vDelta;
			vDir.SafeNormalize();
			ANGLE aWantedHeadingRelative = GetRelativeHeading(vDir);

			// normalize it to [-180,+180] degrees
			aWantedHeadingRelative = NormalizeAngle(aWantedHeadingRelative);

			ANGLE aHeadingRotation;
			// if desired position is left
			if (aWantedHeadingRelative < -m_aRotateSpeed*m_fMoveFrequency) 
			{
				// start turning left
				aHeadingRotation = -m_aRotateSpeed;
			// if desired position is right
			} 
			else if (aWantedHeadingRelative > m_aRotateSpeed*m_fMoveFrequency) 
			{
				// start turning right
				aHeadingRotation = +m_aRotateSpeed;
			// if desired position is more-less ahead
			} 
			else 
			{
				// keep just the adjusting fraction of speed 
				aHeadingRotation = aWantedHeadingRelative/m_fMoveFrequency;
			}
			// start rotating
			SetDesiredRotation(ANGLE3D(aHeadingRotation, 0, 0));
			
			if (Abs(aHeadingRotation)>1.0f) 
			{
				ulFlags |= MF_ROTATEH;				
			}

		// if we may not rotate
		} 
		else 
		{
			// stop rotating
			SetDesiredRotation(ANGLE3D(0, 0, 0));			
		}

		// FIXME : Player.es의 이동 루틴을 복사해옴.
		// if we may move(이동중이야...)
		if ( m_fMoveSpeed > 0.0f ) 
		{
			//0703 kwon 높이맵이 적용되기 전까지는 평면이동을 하게 했다.
			FLOAT3D vDelta = m_vDesiredPosition - GetPlacement().pl_PositionVector;
			vDelta(2) = 0.0f;

			FLOAT length = vDelta.Length();
			FLOAT deltaX = m_vDesiredPosition(1) - GetPlacement().pl_PositionVector(1);
			FLOAT deltaZ = m_vDesiredPosition(3) - GetPlacement().pl_PositionVector(3);

			FLOAT num = length/m_fMoveSpeed;	

			// 한번에 갈수 있는 거리라면...
			if(num < 1.0f)
			{
				//이전에 메시지 보낼때와 비교해서 0.5보다 적게 갔다면,
				FLOAT3D vDelta = GetPlacement().pl_PositionVector - m_vMyPositionTmp;
				vDelta(2) = 0.0f;

				if(vDelta.Length() < 0.5f)
				{
					//y값을 고려하지 않은 거리 계산.
					FLOAT flength = Sqrt(deltaX * deltaX + deltaZ * deltaZ);
					
					if(flength < 0.5f)
					{
						if(!_pNetwork->m_bSingleMode){
							StopMove();
						}
						return 0;
					}					
				}

				CPlacement3D plPlacement;
				plPlacement.pl_OrientationAngle = ANGLE3D(GetPlacement().pl_OrientationAngle(1),0,0);
				plPlacement.pl_PositionVector = m_vDesiredPosition;
				
				// 메세지 보내고 나도 이동.
				if( !_pNetwork->m_bSingleMode )
				{
					_pNetwork->SendMoveMessage(this, plPlacement, m_fMoveSpeed);
				}
				m_vMyPositionTmp = GetPlacement().pl_PositionVector;

				return SetDesiredMovement();
			}

			// 한번에 갈수 없는 거리라면...

			deltaX = deltaX/num;
			deltaZ = deltaZ/num;
			
			FLOAT3D	vRayHit;
			CPlacement3D plPlacement;
			plPlacement.pl_OrientationAngle = ANGLE3D(0,0,0);
			plPlacement.pl_PositionVector = FLOAT3D(GetPlacement().pl_PositionVector(1)+deltaX, 300.0f,GetPlacement().pl_PositionVector(3)+deltaZ);	
			
			FLOAT fMaxY = -9999999.0f;
			BOOL bFloorHitted = FALSE;
			
			FLOAT3D vSource = plPlacement.pl_PositionVector;
			FLOAT3D vTarget = vSource;
			vTarget(2) -= 1000.0f;
			CCastRay crRay( this, vSource, vTarget);
			crRay.cr_ttHitModels = CCastRay::TT_NONE; // CCastRay::TT_FULLSEETHROUGH;
			crRay.cr_bHitTranslucentPortals = TRUE;
			crRay.cr_bPhysical = TRUE;
			GetWorld()->CastRay(crRay);
			if( (crRay.cr_penHit != NULL) && (crRay.cr_vHit(2) > fMaxY)) 
			{
				fMaxY = crRay.cr_vHit(2);
				bFloorHitted = TRUE;
			}

			if( bFloorHitted)
			{
				plPlacement.pl_PositionVector(2) += fMaxY-plPlacement.pl_PositionVector(2)+0.01f;
			}
				
			plPlacement.pl_OrientationAngle(1) = GetPlacement().pl_OrientationAngle(1);

			// 메세지 보내고 나도 이동.
			if( !_pNetwork->m_bSingleMode )
			{
				_pNetwork->SendMoveMessage(this, plPlacement, m_fMoveSpeed);
			}
			m_vMyPositionTmp = GetPlacement().pl_PositionVector;			
						
			return SetDesiredMovement();
			// if we may not move
		}
		else 
		{			
			// stop translating
			SetDesiredTranslation(FLOAT3D(0, 0, 0));			
		}
		return ulFlags;
	};



	// set desired rotation and translation to go/orient towards desired position
	// and get the resulting movement type
	virtual ULONG SetDesiredMovement(void) 
	{
		ULONG ulFlags = 0;
/*
		// get delta to desired position
		FLOAT3D vDelta = m_vDesiredPosition - GetPlacement().pl_PositionVector;

		// if we may rotate
		if (m_aRotateSpeed>0.0f) 
		{
			// get desired heading orientation
			FLOAT3D vDir = vDelta;
			vDir.SafeNormalize();
			ANGLE aWantedHeadingRelative = GetRelativeHeading(vDir);

			// normalize it to [-180,+180] degrees
			aWantedHeadingRelative = NormalizeAngle(aWantedHeadingRelative);

			ANGLE aHeadingRotation;
			// if desired position is left
			if (aWantedHeadingRelative < -m_aRotateSpeed*m_fMoveFrequency) 
			{
				// start turning left
				aHeadingRotation = -m_aRotateSpeed;
			// if desired position is right
			} 
			else if (aWantedHeadingRelative > m_aRotateSpeed*m_fMoveFrequency) 
			{
				// start turning right
				aHeadingRotation = +m_aRotateSpeed;
			// if desired position is more-less ahead
			} 
			else 
			{
				// keep just the adjusting fraction of speed 
				aHeadingRotation = aWantedHeadingRelative/m_fMoveFrequency;
			}
			// start rotating
			SetDesiredRotation(ANGLE3D(aHeadingRotation, 0, 0));
			
			if (Abs(aHeadingRotation)>1.0f) 
			{
				ulFlags |= MF_ROTATEH;
			}

		// if we may not rotate
		} 
		else 
		{
			// stop rotating
			SetDesiredRotation(ANGLE3D(0, 0, 0));			
		}
*/
		ulFlags = SetDesiredRotate();

		// if we may move(이동중이야...)
		if (m_fMoveSpeed > 0.0f ) 
		{
			FLOAT3D vDeltaPlane = m_vDesiredPosition - GetPlacement().pl_PositionVector;
			vDeltaPlane(2) = 0.0f;

			FLOAT fLength = vDeltaPlane.Length();

			// determine translation speed
			//FLOAT3D vTranslation(0.0f, 0.0f, 0.0f);
			//vTranslation(3) = -m_fMoveSpeed;			
			
			/*
			if( !m_bUseAI && 
				(fLength > m_fMoveSpeed && 
				fLength > 1.0f ) )
			{
				m_fMoveSpeed = fLength;
			}
			*/			
			
			vDeltaPlane.Normalize();

			if( fLength < m_fMoveSpeed * m_fMoveFrequency )
			{
				// m_fMoveSpeed = fLength;
				m_fMoveSpeed = fLength / m_fMoveFrequency;
			}

			FLOAT3D vSpeed = vDeltaPlane*m_fMoveSpeed;				
			FLOAT fTestLength = vSpeed.Length();
			SetDesiredTranslation(vSpeed);
			ulFlags |= MF_MOVEZ;
			return ulFlags;
		} 
		else 
		{
			// stop translating
			SetDesiredTranslation(FLOAT3D(0, 0, 0));			
		}
		return ulFlags;
	};

	virtual ULONG SetDesiredRotate(void) 
	{
		ULONG ulFlags = 0;

		// get delta to desired position
		FLOAT3D vDelta = m_vDesiredPosition - GetPlacement().pl_PositionVector;
		vDelta(2) = 0.0f;

		// if we may rotate(회전할때... 적과의 각도를 계산해서 회전~)
		if (m_aRotateSpeed>0.0f) 
		{
			// get desired heading orientation
			FLOAT3D vDir = vDelta;
			vDir.SafeNormalize();
			ANGLE aWantedHeadingRelative = GetRelativeHeading(vDir);

			// normalize it to [-180,+180] degrees
			aWantedHeadingRelative = NormalizeAngle(aWantedHeadingRelative);

			ANGLE aHeadingRotation;
			// if desired position is left
			if (aWantedHeadingRelative < -m_aRotateSpeed*m_fMoveFrequency) 
			{
				// start turning left
				aHeadingRotation = -m_aRotateSpeed;
			// if desired position is right
			} 
			else if (aWantedHeadingRelative > m_aRotateSpeed*m_fMoveFrequency) 
			{
				// start turning right
				aHeadingRotation = +m_aRotateSpeed;
			// if desired position is more-less ahead
			} 
			else 
			{
				// keep just the adjusting fraction of speed 
				aHeadingRotation = aWantedHeadingRelative/m_fMoveFrequency;
			}
			// start rotating(회전하기~)
			SetDesiredRotation(ANGLE3D(aHeadingRotation, 0, 0));
			
			if (Abs(aHeadingRotation)>1.0f) 
			{			
				ulFlags |= MF_ROTATEH;
			}

		// if we may not rotate
		} 
		else 
		{
			// stop rotating
			SetDesiredRotation(ANGLE3D(0, 0, 0));
		}
		return ulFlags;
	};	

	// ----------------------------------------------------------------------------
	// Name : 
	// Desc : 
	// ----------------------------------------------------------------------------
	
	// stop moving entity
	void StopMoving() 
	{
		StopRotating();
		StopTranslating();
	};

	// stop desired rotation
	void StopRotating() 
	{
		SetDesiredRotation(ANGLE3D(0, 0, 0));
	};

	// stop desired translation
	void StopTranslating() 
	{
		SetDesiredTranslation(FLOAT3D(0.0f, 0.0f, 0.0f));
	};

	// ----------------------------------------------------------------------------
	// Name : 
	// Desc : 
	// ----------------------------------------------------------------------------

	virtual void StandingAnim(void) {};	
	virtual void WalkingAnim(void) {};
	virtual void RunningAnim(void) {};
	virtual void RotatingAnim(void) {};
	virtual void AttackAnim(void) {};
	virtual void HungryAnim(void) {}; // 배고픔 동작
	virtual void ActEventAnim(void) {}; // 대기 동작 활성화(배고픔에서 벗어 날때 및 대기동작 변경)

	// ----------------------------------------------------------------------------
	// Name : 
	// Desc : 
	// ----------------------------------------------------------------------------
	// check if an entity is valid for being your new enemy
	BOOL IsValidForEnemy(CEntity *penPlayer)
	{
		return 
			penPlayer!=NULL && 
			// NOTE : 에너미가 지들끼리 싸울수 있도록 하기 위한 부분.
			//IsDerivedFromClass( penPlayer, &CPlayer_DLLClass) &&
			penPlayer->GetFlags()&ENF_ALIVE;
	}
	
procedures:	
  Main()
  {
	InitAsVoid();	
  }
};