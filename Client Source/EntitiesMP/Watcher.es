700
%{
#include "StdH.h"

#include "EntitiesMP/EnemyBase.h"
%}

// input parameter for watcher
event EWatcherInit 
{
	CEntityID eidOwner,        // who owns it
};

// entity is seen
event EWatch 
{
	CEntityID eidSeen,
};

class export CWatcher : CRationalEntity 
{
name      "Watcher";
thumbnail "";
features  "CanBePredictable","NotSentOverNet";

properties:

1 CEntityPointer m_penOwner,  // entity which owns it
2 FLOAT m_tmDelay = 6.0f,     // delay between checking moments - set depending on distance of closest player

20 FLOAT m_fClosestPlayer = UpperLimit(0.0f),  // distance from closest player to owner of this watcher
21 INDEX m_iPlayerToCheck = 0,   // sequence number for checking next player in each turn

31 INDEX m_iOwnerID = -1  features(EPROPF_NETSEND),

components:

functions:

	class CEnemyBase *GetOwner(void)
	{
		ASSERT(m_penOwner!=NULL);
		return (CEnemyBase*)&*m_penOwner;
	}

	// find one player number by random(플레이어를 랜덤하게 찾아낸다.)
	// NOTE : 안쓰임.
	INDEX GetRandomPlayer(void)
	{
		INDEX i;
		INDEX ctMaxPlayers		= GetOwner()->en_pwoWorld->m_vectorTargetNPC.size();
		INDEX ctActivePlayers	= 0;
		for(i=0; i < ctMaxPlayers; i++)
		{
			CEntity* pEntity = GetOwner()->en_pwoWorld->m_vectorTargetNPC[i];
			if(pEntity != NULL)			
			{
				ctActivePlayers++;
			}
		}

		// if none
		if (ctActivePlayers==0) 
		{
			// return first index anyway
			return 0;
		}

		// choose one by random(그중에 한넘을 골라서.)
		INDEX iChosenActivePlayer = IRnd()%ctActivePlayers;

		// find its physical index(물리적 인덱스를 찾고)
		INDEX iActivePlayer = 0;
		for(i=0; i<ctMaxPlayers; i++)
		{
			CEntity* pEntity = GetOwner()->en_pwoWorld->m_vectorTargetNPC[i];
			if(pEntity != NULL)
			{
				if (iActivePlayer == iChosenActivePlayer)
				{
					return i;
				}
				iActivePlayer++;
			}
		}
		ASSERT(FALSE);
		return 0;

		/*
		// get maximum number of players in game
		INDEX ctMaxPlayers = GetMaxPlayers();
		// find actual number of players
		INDEX ctActivePlayers = 0;
		{for(INDEX i=0; i<ctMaxPlayers; i++) {
			if (GetPlayerEntity(i)!=NULL) {
				ctActivePlayers++;
			}
		}}
		// if none
		if (ctActivePlayers==0) {
			// return first index anyway
			return 0;
		}


		// choose one by random(그중에 한넘을 골라서.)
		INDEX iChosenActivePlayer = IRnd()%ctActivePlayers;

		// find its physical index(물리적 인덱스를 찾고)
		INDEX iActivePlayer = 0;
		{for(INDEX i=0; i<ctMaxPlayers; i++) 
		{
			if (GetPlayerEntity(i)!=NULL) 
			{
				if (iActivePlayer==iChosenActivePlayer) 
				{
					return i;
				}
				iActivePlayer++;
			}
		}}
		ASSERT(FALSE);
		return 0;
		*/
	}

	// find closest player
	CEntity *FindClosestPlayer(void)
	{
		CEntity *penClosestPlayer = NULL;
		FLOAT fClosestPlayer = UpperLimit(0.0f);

		// 싱글 모드가 아니라면...(이 경우가 필요한가???)
		for(std::vector<CEntity*>::iterator iter = GetOwner()->en_pwoWorld->m_vectorTargetNPC.begin();
			iter != GetOwner()->en_pwoWorld->m_vectorTargetNPC.end();
			++iter)
		{
			if ((*iter)!=NULL && (*iter)->GetFlags()&ENF_ALIVE && !((*iter)->GetFlags()&ENF_INVISIBLE)) 
			{
				// calculate distance to player
				FLOAT fDistance = ((*iter)->GetPlacement().pl_PositionVector - m_penOwner->GetPlacement().pl_PositionVector).Length();
				// update if closer
				if (fDistance < fClosestPlayer) 
				{
					fClosestPlayer = fDistance;
					penClosestPlayer = (*iter);
				}
			}
		}		
		// if no players found
		if (penClosestPlayer==NULL) 
		{
			// behave as if very close - must check for new ones
			fClosestPlayer = 10.0f;
		}
		m_fClosestPlayer = fClosestPlayer;
		return penClosestPlayer;
	}

	// notify owner that a player has been seen
	void SendWatchEvent(CEntity *penPlayer)
	{
		EWatch eWatch;
		eWatch.eidSeen = penPlayer;
		m_penOwner->SendEvent(eWatch);
	}

	void CheckIfPlayerVisible(void)
	{
		// if the owner is blind
		if( GetOwner()->m_bBlind) 
		{
			// don't even bother checking
			return;
		}
		if(GetOwner()->en_pwoWorld->m_vectorTargetNPC.empty())
		{
			return;
		}

		// get maximum number of players in game
		INDEX ctPlayers		= GetOwner()->en_pwoWorld->m_vectorTargetNPC.size();

		// find first one after current sequence
		CEntity *penEntity	= NULL;
		m_iPlayerToCheck = (m_iPlayerToCheck+1)%ctPlayers;
		INDEX iFirstChecked = m_iPlayerToCheck;
		FOREVER 
		{
			penEntity = GetOwner()->en_pwoWorld->m_vectorTargetNPC[m_iPlayerToCheck];
			if (penEntity!=NULL) 
			{
				break;
			}
			m_iPlayerToCheck++;
			m_iPlayerToCheck%=ctPlayers;
			if (m_iPlayerToCheck==iFirstChecked) 
			{
				return; // we get here if there are no players at all
			}
		}

		// if this one is dead or invisible
		if (!(penEntity->GetFlags()&ENF_ALIVE) || (penEntity->GetFlags()&ENF_INVISIBLE)) 
		{
			// do nothing
			return;
		}

		// if inside view angle and visible
		if (GetOwner()->SeeEntity(penEntity, Cos(GetOwner()->m_fViewAngle/2.0f)))
		{
			// send event to owner
			SendWatchEvent(penEntity);
		}
	};

	// set new watch time
	void SetWatchDelays(void)
	{
		const FLOAT tmMinDelay = 0.1f;   // delay at closest distance
		const FLOAT tmSeeDelay = 5.0f;   // delay at see distance
		const FLOAT tmTick = _pTimer->TickQuantum;
		FLOAT fSeeDistance  = GetOwner()->m_fIgnoreRange;
		FLOAT fNearDistance = Min(GetOwner()->m_fStopDistance, GetOwner()->m_fCloseDistance);

		// if closer than near distance
		if (m_fClosestPlayer<=fNearDistance) 
		{
			// always use minimum delay
			m_tmDelay = tmMinDelay;
		// if further than near distance
		} 
		else 
		{
			// interpolate between near and see
			m_tmDelay = tmMinDelay+
				(m_fClosestPlayer-fNearDistance)*(tmSeeDelay-tmMinDelay)/(fSeeDistance-fNearDistance);
			// round to nearest tick
			m_tmDelay = floor(m_tmDelay/tmTick)*tmTick;
		}
	};

	// watch
	void Watch(void)
	{
		// remember original distance
		FLOAT fOrgDistance = m_fClosestPlayer;

		CEntity *penClosest = NULL;
			// find closest player(가장 가까운 플레이어 찾기)
			penClosest = FindClosestPlayer();

		FLOAT fSeeDistance  = GetOwner()->m_fIgnoreRange;
		FLOAT fStopDistance  = Max(fSeeDistance*1.5f, GetOwner()->m_fActivityRange);

		// if players exited enemy's scope(플레이어가 에너미의 영역에 있다면...)
		if (fOrgDistance<fStopDistance && m_fClosestPlayer>=fStopDistance) 
		{
			// stop owner
			m_penOwner->SendEvent(EStop());
		// if players entered enemy's scope(플레이어가 에너미의 영역으로 들어왔다면...)
		} 
		else if (fOrgDistance>=fStopDistance && m_fClosestPlayer<fStopDistance) 
		{
			// start owner
			m_penOwner->SendEvent(EStart());
		}

		// if the closest player is close enough to be seen
		// 가장 가까운 플레이어가 보일정도록 가까이 있다면...
		if (m_fClosestPlayer<fSeeDistance) 
		{
			// check for seeing any of the players
			CheckIfPlayerVisible();
		}

		// if the closest player is inside sense range
		// 가장 가까운 플레이어가 감지 범위내에 있을때...
		FLOAT fSenseRange = GetOwner()->m_fSenseRange;
		if (penClosest!=NULL && fSenseRange>0 && m_fClosestPlayer<fSenseRange) 
		{
			// detect it immediately
			SendWatchEvent(penClosest);
		}
	
		// set new watch time
		SetWatchDelays();
	};

	// this is called directly from enemybase to check if another player has come too close
	CEntity *CheckCloserPlayer(CEntity *penCurrentTarget, FLOAT fRange)
	{
		// if the owner is blind
		if( GetOwner()->m_bBlind) {
			// don't even bother checking
			return NULL;
		}

		CEntity *penClosestPlayer = NULL;
		FLOAT fClosestPlayer = 
			(penCurrentTarget->GetPlacement().pl_PositionVector-m_penOwner->GetPlacement().pl_PositionVector).Length();
		fClosestPlayer = Min(fClosestPlayer, fRange);  // this is maximum considered range

		/*
		if(!_pNetwork->m_bSingleMode)
		{
		// for all other players
			for (INDEX iPlayer=0; iPlayer<GetMaxPlayers(); iPlayer++) 
			{
			CEntity *penPlayer = GetPlayerEntity(iPlayer);
				if (penPlayer==NULL || penPlayer==penCurrentTarget) 
				{
				continue;
			}
			// if player is alive and visible
				if ((penPlayer->GetFlags()&ENF_ALIVE) && !(penPlayer->GetFlags()&ENF_INVISIBLE)) 
				{
				// calculate distance to player
				FLOAT fDistance = 
					(penPlayer->GetPlacement().pl_PositionVector-m_penOwner->GetPlacement().pl_PositionVector).Length();
				// if closer than current and you can see him
				if (fDistance<fClosestPlayer && 
						GetOwner()->SeeEntity(penPlayer, Cos(GetOwner()->m_fViewAngle/2.0f))) 
					{
					// update
					fClosestPlayer = fDistance;
					penClosestPlayer = penPlayer;
				}
			}
		}
		}
		else
		{
		*/

		for(std::vector<CEntity*>::iterator iter = GetOwner()->en_pwoWorld->m_vectorTargetNPC.begin();
		iter != GetOwner()->en_pwoWorld->m_vectorTargetNPC.end();
		++iter)
		{
			if((*iter == NULL || (*iter) == penCurrentTarget))
			{
				continue;
			}

			if ((*iter)->GetFlags()&ENF_ALIVE && !((*iter)->GetFlags()&ENF_INVISIBLE)) 
			{
				// calculate distance to player
				FLOAT fDistance = ((*iter)->GetPlacement().pl_PositionVector - m_penOwner->GetPlacement().pl_PositionVector).Length();
				// update if closer
				if (fDistance < fClosestPlayer && 
					GetOwner()->SeeEntity((*iter), Cos(GetOwner()->m_fViewAngle/2.0f)))
				{
					fClosestPlayer = fDistance;
					penClosestPlayer = (*iter);
				}
			}
		}

		/*
			CEntity *penPlayer = GetPlayerEntity(0);
			// if player is alive and visible
			if ((penPlayer->GetFlags()&ENF_ALIVE) && !(penPlayer->GetFlags()&ENF_INVISIBLE)) 
			{
				// calculate distance to player
				FLOAT fDistance = 
					(penPlayer->GetPlacement().pl_PositionVector-m_penOwner->GetPlacement().pl_PositionVector).Length();

				// if closer than current and you can see him
				if (fDistance<fClosestPlayer && 
						GetOwner()->SeeEntity(penPlayer, Cos(GetOwner()->m_fViewAngle/2.0f))) 
				{
					// update
					fClosestPlayer = fDistance;
					penClosestPlayer = penPlayer;
				}
			}
			*/
		//}

		return penClosestPlayer;
	}

	// this is called directly from enemybase to attack multiple players (for really big enemies)
	// NOTE : 안쓰임.
	CEntity *CheckAnotherPlayer(CEntity *penCurrentTarget)
	{
		/*
		// if the owner is blind, or no current target
		if( GetOwner()->m_bBlind || penCurrentTarget==NULL) 
		{
			// don't even check
			return NULL;
		}

		// get allowed distance
		CEntity *penClosestPlayer = NULL;
		FLOAT fCurrentDistance = 
			(penCurrentTarget->GetPlacement().pl_PositionVector-m_penOwner->GetPlacement().pl_PositionVector).Length();
		FLOAT fRange = fCurrentDistance*1.5f;

		// find a random offset to start searching
		//INDEX iOffset = GetRandomPlayer();
		INDEX iOffset = 0;

		// for all other players
		INDEX ctPlayers = GetMaxPlayers();
		for (INDEX iPlayer=0; iPlayer<ctPlayers; iPlayer++) 
		{
			CEntity *penPlayer = GetPlayerEntity((iPlayer+iOffset)%ctPlayers);
			if (penPlayer==NULL || penPlayer==penCurrentTarget) 
			{
				continue;
			}
			// if player is alive and visible
			if ((penPlayer->GetFlags()&ENF_ALIVE) && !(penPlayer->GetFlags()&ENF_INVISIBLE)) 
			{
				// calculate distance to player
				FLOAT fDistance = 
					(penPlayer->GetPlacement().pl_PositionVector-m_penOwner->GetPlacement().pl_PositionVector).Length();
				// if inside allowed range and visible
				if (fDistance<fRange && 
						GetOwner()->SeeEntity(penPlayer, Cos(GetOwner()->m_fViewAngle/2.0f))) 
				{
					// attack that one
					return penPlayer;
				}
			}
		}
		return penCurrentTarget;
		*/
		return NULL;
	}


	// returns bytes of memory used by this object
	SLONG GetUsedMemory(void)
	{
		return( sizeof(CWatcher) - sizeof(CRationalEntity) + CRationalEntity::GetUsedMemory());
	}

procedures:
	Main(EWatcherInit eInit) 
	{
		// remember the initial parameters
		ASSERT((CEntity*)eInit.eidOwner!=NULL);
		m_penOwner = eInit.eidOwner;

		m_iOwnerID = m_penOwner->en_ulID;

		// init as nothing
		InitAsVoid();
		SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
		SetCollisionFlags(ECF_IMMATERIAL);

//강동민 수정 시작 싱글 던젼 작업	07.29
		if(!_pNetwork->m_bSingleMode)
		{
			// if in flyover game mode(Flyover 게임 모드)
			if (GetSP()->sp_gmGameMode == CSessionProperties::GM_FLYOVER) 
			{
				// go to dummy mode
				jump Dummy();
				// NOTE: must not destroy self, because owner has a pointer
			}
		}
//강동민 수정 끝 싱글 던젼 작업		07.29

		// generate random number of player to check next(에너미 생성될때만 Watcher가 생성되는듯...) 
		// (to provide even distribution of enemies among players)
		m_iPlayerToCheck = GetRandomPlayer()-1;
		//m_iPlayerToCheck = -1;

		// start in disabled state
		autocall Inactive() EEnd;

		// cease to exist(에너미가 죽었을때...)
		Destroy();

		return;
	};

	// watching
	Active() 
	{
		// repeat
		while (TRUE) 
		{
			// check all players
			Watch();
			// wait for given delay
			wait(m_tmDelay) 
			{
				on (EBegin) : { resume; }
				on (ETimer) : { stop; }		// 지정된 시간만큼 기다렸을때...
				// stop looking
				on (EStop) : { jump Inactive(); }
				// force re-checking if receiving start or teleport
				on (EStart) : { stop; }
				on (ETeleport) : { stop; }
			}
		}
	};

	// not watching
	Inactive(EVoid) 
	{
		wait() 
		{
			on (EBegin) : { resume; }
			on (EStart) : { jump Active(); }
		}
	};

	// dummy mode
	Dummy(EVoid)
	{
		// ignores all events forever
		wait() {
			on (EBegin) : { resume; }
			otherwise() : { resume; };
		};
	}
};
