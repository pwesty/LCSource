222
%{
#include "StdH.h"
#include "EntitiesMP/EnemyBase.h"
#include "EntitiesMP/EnemySpawner.h"
#include "EntitiesMP/Trigger.h"
//#include "EntitiesMP/Woman.h"
//#include "EntitiesMP/BigHead.h"
//#include "EntitiesMP/CrateRider.h"

#include <Engine/Sound/SoundData.h>
%}


enum MusicType {
	0 MT_LIGHT  "light",
	1 MT_MEDIUM "medium",
	2 MT_HEAVY  "heavy",
	3 MT_EVENT  "event",
	4 MT_CONTINUOUS  "continuous",
};

event EChangeMusic {
	enum MusicType mtType,
	CTFileName fnMusic,
	FLOAT fVolume,
	BOOL bForceStart,
};

event ENetChangeMusic {
	enum MusicType mtType,
	INDEX iBossID,
	INDEX iCounterID,
};

%{
#define MUSIC_VOLUMEMIN   0.02f     // minimum volume (considered off)
#define MUSIC_VOLUMEMAX   0.98f     // maximum volume (considered full)

float FadeInFactor(TIME fFadeTime)
{
	return (float) pow(MUSIC_VOLUMEMAX/MUSIC_VOLUMEMIN, 1/(fFadeTime/_pTimer->TickQuantum));
}
float FadeOutFactor(TIME fFadeTime)
{
	return (float) pow(MUSIC_VOLUMEMIN/MUSIC_VOLUMEMAX, 1/(fFadeTime/_pTimer->TickQuantum));
}
%}

class CMusicHolder : CRationalEntity {
name      "MusicHolder";
thumbnail "Thumbnails\\MusicHolder.tbn";
features "HasName", "IsTargetable", "IsImportant";

properties:
	1 CTString m_strName     "" = "MusicHolder",
	2 FLOAT m_fScoreMedium "Score Medium" = 100.0f,
	3 FLOAT m_fScoreHeavy  "Score Heavy"  = 1000.0f,

 10 CTFileName m_fnMusic0 "Music Light" 'M' = CTFILENAME("") features(EPROPF_NETSEND),
 11 CTFileName m_fnMusic1 "Music Medium"    = CTFILENAME("") features(EPROPF_NETSEND),
 12 CTFileName m_fnMusic2 "Music Heavy"     = CTFILENAME("") features(EPROPF_NETSEND),
 13 CTFileName m_fnMusic3                   = CTFILENAME("") features(EPROPF_NETSEND),  // event music
 14 CTFileName m_fnMusic4                   = CTFILENAME("") features(EPROPF_NETSEND),  // continuous music

 20 FLOAT m_fVolume0  "Volume Light" 'V' = 1.0f features(EPROPF_NETSEND),
 21 FLOAT m_fVolume1  "Volume Medium"    = 1.0f features(EPROPF_NETSEND),
 22 FLOAT m_fVolume2  "Volume Heavy"     = 1.0f features(EPROPF_NETSEND),
 23 FLOAT m_fVolume3                     = 1.0f features(EPROPF_NETSEND),  // event volume
 24 FLOAT m_fVolume4                     = 1.0f features(EPROPF_NETSEND),  // continuous volume

// internals

100 CEntityPointer m_penBoss,    // current boss if any
102 CEntityPointer m_penCounter,   // enemy counter for wave-fight progress display
104 INDEX m_ctEnemiesInWorld = 0,   // count of total enemies in world
105 INDEX m_iRespawnMarker = -1 features(EPROPF_NETSEND),    // respawn marker for coop
106 INDEX m_ctSecretsInWorld = 0,   // count of total secrets in world
101 FLOAT m_tmFade = 1.0f,    // music cross-fade speed
103 enum MusicType m_mtCurrentMusic = MT_LIGHT features(EPROPF_NETSEND), // current active channel
109 INDEX m_ctTreasuresInWorld = 0,   // count of total treasures in world

// for cross-fade purposes
110 FLOAT m_fCurrentVolume0a  = 1.0f features(EPROPF_NETSEND),
210 FLOAT m_fCurrentVolume0b  = 1.0f features(EPROPF_NETSEND),
111 FLOAT m_fCurrentVolume1a  = 1.0f features(EPROPF_NETSEND),
211 FLOAT m_fCurrentVolume1b  = 1.0f features(EPROPF_NETSEND),
112 FLOAT m_fCurrentVolume2a  = 1.0f features(EPROPF_NETSEND),
212 FLOAT m_fCurrentVolume2b  = 1.0f features(EPROPF_NETSEND),
113 FLOAT m_fCurrentVolume3a  = 1.0f features(EPROPF_NETSEND),
213 FLOAT m_fCurrentVolume3b  = 1.0f features(EPROPF_NETSEND),
114 FLOAT m_fCurrentVolume4a  = 1.0f features(EPROPF_NETSEND),
214 FLOAT m_fCurrentVolume4b  = 1.0f features(EPROPF_NETSEND),

// the music channels
120 CSoundObject m_soMusic0a features(EPROPF_NETSEND),
220 CSoundObject m_soMusic0b features(EPROPF_NETSEND),
121 CSoundObject m_soMusic1a features(EPROPF_NETSEND),
221 CSoundObject m_soMusic1b features(EPROPF_NETSEND),
122 CSoundObject m_soMusic2a features(EPROPF_NETSEND),
222 CSoundObject m_soMusic2b features(EPROPF_NETSEND),
123 CSoundObject m_soMusic3a features(EPROPF_NETSEND),
223 CSoundObject m_soMusic3b features(EPROPF_NETSEND),
124 CSoundObject m_soMusic4a features(EPROPF_NETSEND),
224 CSoundObject m_soMusic4b features(EPROPF_NETSEND),

// next free subchannel markers (all starts at subchannel 1(b), first switch goes to subchannel 0(a))
130 INDEX m_iSubChannel0 = 1 features(EPROPF_NETSEND),
131 INDEX m_iSubChannel1 = 1 features(EPROPF_NETSEND),
132 INDEX m_iSubChannel2 = 1 features(EPROPF_NETSEND),
133 INDEX m_iSubChannel3 = 1 features(EPROPF_NETSEND),
134 INDEX m_iSubChannel4 = 1 features(EPROPF_NETSEND),

150 FLOAT m_tmLastUpdate = -1 features(EPROPF_NETSEND),
151 INDEX m_iFirstTime   = TRUE,

	{
		// array of enemies that make fuss
		CDynamicContainer<CEntity> m_cenFussMakers;
		// array of all enemies in the world
		CStaticStackArray<ULONG> m_aulAutoAimables;
		CAutoPrecacheSound m_aps0;
		CAutoPrecacheSound m_aps1;
		CAutoPrecacheSound m_aps2;
	}

components:
	1 editor model   MODEL_MARKER     "Data\\Models\\Editor\\MusicHolder.mdl",
	2 editor texture TEXTURE_MARKER   "Data\\Models\\Editor\\MusicHolder.tex"


functions:
	
	void Precache(void)
	{
		if(m_fnMusic0!="") {
			m_aps0.Precache(m_fnMusic0);
		}
		if(m_fnMusic1!="") {
			m_aps1.Precache(m_fnMusic1);
		}
		if(m_fnMusic2!="") {
			m_aps2.Precache(m_fnMusic2);
		}
	}

	// count enemies in current world
	void CountEnemies(void)
	{
		m_ctEnemiesInWorld = 0;
		m_ctSecretsInWorld = 0;
		// for each entity in the world
		{FOREACHINDYNAMICCONTAINER(GetWorld()->wo_cenEntities, CEntity, iten) {
			CEntity *pen = iten;
			// if enemybase
			if (IsDerivedFromClass(pen, "Enemy Base")) {
				CEnemyBase *penEnemy = (CEnemyBase *)pen;
				// if not template
				if( !penEnemy->m_bTemplate )
					//&& !IsOfClass( pen, "BigHead"))
				 //&& !IsOfClass( pen, "CrateRider")) 
				{
					// count one
					m_ctEnemiesInWorld++;
					AddEnemyKillData(penEnemy, NULL);

					/*
		      // if this is a woman kamikaze carrier, add another one to count
		      if (IsOfClass(pen, "Woman")) 
			  {
			      if (((CWoman *)&*pen)->m_bKamikazeCarrier) { m_ctEnemiesInWorld++; }
		      }
			  */
				}
			// if spawner
			} else if (IsDerivedFromClass(pen, "Enemy Spawner")) {
				CEnemySpawner *penSpawner = (CEnemySpawner *)pen;
				BOOL bDoNotCount = FALSE;
				if( penSpawner->m_penTarget) {
					//if( IsOfClass( penSpawner->m_penTarget, "BigHead"))
					// || IsOfClass( penSpawner->m_penTarget, "CrateRider")) 
					//{
					//	bDoNotCount = TRUE;
					//}
				}
				// if not teleporting
				if (penSpawner->m_estType!=EST_TELEPORTER && !bDoNotCount) {
					AddSpawnerKillData(penSpawner, penSpawner->m_ctTotal);
					// add total count
					m_ctEnemiesInWorld+=penSpawner->m_ctTotal;
					// if this spawner points to a woman kamikaze carrier template, increase count once more
					if (penSpawner->m_penTarget) 
					{
						/*
						if (IsOfClass(penSpawner->m_penTarget, "Woman")) 
						{
							if (((CWoman *)&*penSpawner->m_penTarget)->m_bKamikazeCarrier) { m_ctEnemiesInWorld+=penSpawner->m_ctTotal; }
						}
						*/
					}
				}
			// if trigger
			} else if (IsDerivedFromClass(pen, "Trigger")) {
				CTrigger *penTrigger = (CTrigger *)pen;
				/*
				// if has score
				if (penTrigger->m_fScore>0) {
					// it counts as a secret
					m_ctSecretsInWorld++;
				}
				*/
			}
		}}
	}

	// check for stale fuss-makers
	void CheckOldFussMakers(void)
	{
		TIME tmNow = _pTimer->CurrentTick();
		TIME tmTooOld = tmNow-10.0f;
		CDynamicContainer<CEntity> cenOldFussMakers;
		// for each fussmaker
		{FOREACHINDYNAMICCONTAINER(m_cenFussMakers, CEntity, itenFussMaker) {
			CEnemyBase & enFussMaker = (CEnemyBase&)*itenFussMaker;
			// if haven't done fuss for too long
			if (enFussMaker.m_tmLastFussTime<tmTooOld) {
				// add to old fuss makers
				cenOldFussMakers.Add(&enFussMaker);
			}
		}}
		// for each old fussmaker
		{FOREACHINDYNAMICCONTAINER(cenOldFussMakers, CEntity, itenOldFussMaker) {
			CEnemyBase &enOldFussMaker = (CEnemyBase&)*itenOldFussMaker;
			// remove from fuss
			enOldFussMaker.RemoveFromFuss();
		}}
	}
	
	// get total score of all active fuss makers
	INDEX GetFussMakersScore(void) {
		INDEX iScore = 0;
		{FOREACHINDYNAMICCONTAINER(m_cenFussMakers, CEntity, itenFussMaker) {
			CEnemyBase &enFussMaker = (CEnemyBase&)*itenFussMaker;
			//iScore += enFussMaker.m_iScore;
		}}
		return iScore;
	}

	// change given music channel
	void ChangeMusicChannel(enum MusicType mtType, const CTFileName &fnNewMusic, FLOAT fNewVolume)
	{
		// Get real file name
		CTFileName fnmExpanded;
		if(fnNewMusic!="") {
			ExpandFilePath(EFP_READ, fnNewMusic, fnmExpanded);
		}

		INDEX &iSubChannel = (&m_iSubChannel0)[mtType];

		BOOL bChangeMusic = TRUE;
		// change music only if fn is diff from one currently playing
		CSoundObject &soMusicCurrent = (&m_soMusic0a)[mtType*2+iSubChannel];
		const CSoundData *sdSound = soMusicCurrent.so_psdLink;
		if(sdSound!=NULL) {
			if(sdSound->ser_FileName==fnmExpanded) {
				bChangeMusic = FALSE;
			}
		}

		if(bChangeMusic) {
			// take next sub-channel if needed
			if (fnmExpanded!="") {
				iSubChannel = (iSubChannel+1)%2;
			}
		}

		// find channel's variables
		FLOAT &fVolume = (&m_fVolume0)[mtType];
		CSoundObject &soMusic = (&m_soMusic0a)[mtType*2+iSubChannel];
		FLOAT &fCurrentVolume = (&m_fCurrentVolume0a)[mtType*2+iSubChannel];

		// setup looping/non looping flags
		ULONG ulFlags = SOF_MUSIC;
		if( mtType!=MT_EVENT) { ulFlags |= SOF_LOOP; } // |SOF_NONGAME;

		// remember volumes
		fVolume = fNewVolume;

		// start new music file if needed
		if (fnmExpanded!="" && bChangeMusic) {
			// CPrintF("PlaySound( %s )\n", (const char*)fnmExpanded);
			PlaySound( soMusic, fnmExpanded, ulFlags);
			// initially, not playing
			fCurrentVolume = MUSIC_VOLUMEMIN;
			soMusic.Pause();
			soMusic.SetVolume(fCurrentVolume*fVolume);
		}
	}

	// fade out one channel
	void FadeOutChannel(INDEX iChannel, INDEX iSubChannel)
	{
		// find channel's variables
		FLOAT &fVolume = (&m_fVolume0)[iChannel];
		CSoundObject &soMusic = (&m_soMusic0a)[iChannel*2+iSubChannel];
		FLOAT &fCurrentVolume = (&m_fCurrentVolume0a)[iChannel*2+iSubChannel];

		// do nothing, if music is not playing
		if( !soMusic.IsPlaying()) { return; }

		// do nothing, if music is already paused
		if( soMusic.IsPaused()) { return; }

		// if minimum volume reached 
		if( fCurrentVolume<MUSIC_VOLUMEMIN) {
			// pause music
			soMusic.Pause();
		} else {
			// music isn't even faded yet, so continue on fading it out
			fCurrentVolume *= FadeOutFactor( m_tmFade);
			soMusic.SetVolume(fCurrentVolume*fVolume);
		}
	}

	// fade in one channel
	void FadeInChannel(INDEX iChannel, INDEX iSubChannel)
	{
		// find channel's variables
		FLOAT &fVolume = (&m_fVolume0)[iChannel];
		CSoundObject &soMusic = (&m_soMusic0a)[iChannel*2+iSubChannel];
		FLOAT &fCurrentVolume = (&m_fCurrentVolume0a)[iChannel*2+iSubChannel];

		// do nothing, if music is not playing
		if( !soMusic.IsPlaying()) { return; }

		// resume music if needed
		if( soMusic.IsPaused()) {
			soMusic.Resume();
		}
		// fade in music if needed
		if( fCurrentVolume<MUSIC_VOLUMEMAX) {
			fCurrentVolume *= FadeInFactor( m_tmFade);
			fCurrentVolume = ClampUp( fCurrentVolume, 1.0f);
		}
		soMusic.SetVolume(fCurrentVolume*fVolume);
	}

	// fade one channel in or out
	void CrossFadeOneChannel(enum MusicType mtType)
	{
		INDEX iSubChannelActive = (&m_iSubChannel0)[mtType];
		INDEX iSubChannelInactive = (iSubChannelActive+1)%2;
		// if it is current channel
		if (mtType==m_mtCurrentMusic) {
			// fade in active subchannel
			FadeInChannel(mtType, iSubChannelActive);
			// fade out inactive subchannel
			FadeOutChannel(mtType, iSubChannelInactive);
		// if it is not current channel
		} else {
			// fade it out
			FadeOutChannel(mtType, 0);
			FadeOutChannel(mtType, 1);
		}
	}
	
procedures:
	// initialize music
 // initialize music
  Main(EVoid) {

    // init as model
    InitAsEditorModel();
    SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
    SetCollisionFlags(ECF_IMMATERIAL);

    // set appearance
    SetModel(MODEL_MARKER);
    SetModelMainTexture(TEXTURE_MARKER);
    SetFlagOn(ENF_CLIENTHANDLING);
    SetFlagOff(ENF_NONETCONNECT);

    // wait for game to start
    autowait(_pTimer->TickQuantum);

    if (_pNetwork->IsServer()) {
      // prepare initial music channel values
      ChangeMusicChannel(MT_LIGHT,        m_fnMusic0, m_fVolume0);
      ChangeMusicChannel(MT_MEDIUM,       m_fnMusic1, m_fVolume1);
      ChangeMusicChannel(MT_HEAVY,        m_fnMusic2, m_fVolume2);
      ChangeMusicChannel(MT_EVENT,        m_fnMusic3, m_fVolume3);
      ChangeMusicChannel(MT_CONTINUOUS,   m_fnMusic4, m_fVolume4);

      // start with light music
      m_mtCurrentMusic = MT_LIGHT;
      m_fCurrentVolume0a = MUSIC_VOLUMEMAX*0.98f;
      m_tmFade = 0.01f;
      CrossFadeOneChannel(MT_LIGHT);
    }

    // must react after enemyspawner and all enemies, but before player for proper enemy counting
    // (total wait is two ticks so far)
    autowait(_pTimer->TickQuantum);

    // count enemies in current world
    CountEnemies();

    // main loop
    while(TRUE) {
      // wait a bit
      wait(0.1f) {
        on (ETimer) : {
          if (m_iFirstTime && !_pNetwork->IsServer()) {
            // prepare initial music channel values
            if(m_mtCurrentMusic==MT_LIGHT) {
              ChangeMusicChannel(MT_LIGHT,        m_fnMusic0, m_fVolume0);
            } else if(m_mtCurrentMusic==MT_MEDIUM) {
              ChangeMusicChannel(MT_MEDIUM,       m_fnMusic1, m_fVolume1);
            } else if(m_mtCurrentMusic==MT_HEAVY) {
              ChangeMusicChannel(MT_HEAVY,        m_fnMusic2, m_fVolume2);
            } else if(m_mtCurrentMusic==MT_EVENT) {
              ChangeMusicChannel(MT_EVENT,        m_fnMusic3, m_fVolume3);
            } else if(m_mtCurrentMusic==MT_CONTINUOUS) {
              ChangeMusicChannel(MT_CONTINUOUS,   m_fnMusic4, m_fVolume4);
            }

            // fade all channels
            CrossFadeOneChannel(MT_LIGHT);
            CrossFadeOneChannel(MT_MEDIUM);
            CrossFadeOneChannel(MT_HEAVY);
            CrossFadeOneChannel(MT_EVENT);
            CrossFadeOneChannel(MT_CONTINUOUS);

            /*// start with light music
            m_mtCurrentMusic = MT_LIGHT;
            m_fCurrentVolume0a = MUSIC_VOLUMEMAX*0.98f;
            m_tmFade = 0.01f;
            CrossFadeOneChannel(MT_LIGHT);*/
            
            CountEnemies();

            m_iFirstTime = FALSE;
          }
          stop;
        };
        // if music is to be changed
        on (EChangeMusic ecm) : { 
          // change parameters
          ChangeMusicChannel(ecm.mtType, ecm.fnMusic, ecm.fVolume);
          if(ecm.mtType==MT_LIGHT) {
            m_fnMusic0 = ecm.fnMusic;
          } else if(ecm.mtType==MT_MEDIUM) {
            m_fnMusic1 = ecm.fnMusic;
          } else if(ecm.mtType==MT_HEAVY) {
            m_fnMusic2 = ecm.fnMusic;
          } else if(ecm.mtType==MT_EVENT) {
            m_fnMusic3 = ecm.fnMusic;
          } else if(ecm.mtType==MT_CONTINUOUS) {
            m_fnMusic4 = ecm.fnMusic;
          }
          // if force started
          if (ecm.bForceStart) {
            // set as current music
            m_mtCurrentMusic = ecm.mtType;
          }
          // stop waiting
          stop;
        }
        on (ENetChangeMusic encm) : {
          if (!_pNetwork->IsServer()) {
            m_mtCurrentMusic = encm.mtType;
            //CPrintF("Music change\n");
            // setup fade speed depending on music type           
            CEntity* penEntity;
            if (encm.iBossID == -1) {
              m_penBoss = NULL;
            } else {
              _pNetwork->ga_World.EntityExists(encm.iBossID,penEntity);
              m_penBoss = penEntity;
            }
            if (encm.iCounterID == -1) {
              m_penCounter = NULL;
            } else {
              _pNetwork->ga_World.EntityExists(encm.iCounterID,penEntity);
              m_penCounter = penEntity;
            }

          }
          stop;
        }
      }
      if (_pNetwork->IsServer()) {
        // check fuss
        CheckOldFussMakers();
        // get total score of all active fuss makers
        FLOAT fFussScore = GetFussMakersScore();
        MusicType mtOldMusic = m_mtCurrentMusic;

        // if event is on
        if (m_mtCurrentMusic==MT_EVENT) {
          // if event has ceased playing
          if (!m_soMusic3a.IsPlaying() && !m_soMusic3b.IsPlaying()) {
            // switch to light music
            m_mtCurrentMusic=MT_LIGHT;
          }
        }
        // if heavy fight is on
        if (m_mtCurrentMusic==MT_HEAVY) {
          // if no more fuss
          if (fFussScore<=0.0f) {
            // switch to no fight
            m_mtCurrentMusic=MT_LIGHT;
          }
        // if medium fight is on
        } else if (m_mtCurrentMusic==MT_MEDIUM) {
          // if no more fuss
          if (fFussScore<=0.0f) {
            // switch to no fight
            m_mtCurrentMusic=MT_LIGHT;
          // if larger fuss
          } else if (fFussScore>=m_fScoreHeavy) {
            // switch to heavy fight
            m_mtCurrentMusic=MT_HEAVY;
          }
        // if no fight is on
        } else if (m_mtCurrentMusic==MT_LIGHT) {
          // if heavy fuss
          if (fFussScore>=m_fScoreHeavy) {
            // switch to heavy fight
            m_mtCurrentMusic=MT_HEAVY;
          // if medium fuss
          } else if (fFussScore>=m_fScoreMedium) {
            // switch to medium fight
            m_mtCurrentMusic=MT_MEDIUM;
          }
        }

        FLOAT tmNow = _pTimer->GetHighPrecisionTimer().GetSeconds();
        if ((tmNow - m_tmLastUpdate) > 4*_pTimer->TickQuantum) {       
          ENetChangeMusic encm;
          encm.mtType = m_mtCurrentMusic;
          if (((CEntity*)m_penBoss) == NULL) {
            encm.iBossID = -1;
          } else {
            encm.iBossID = m_penBoss->en_ulID;
          }
          if (((CEntity*)m_penCounter) == NULL) {
            encm.iCounterID = -1;
          } else {
            encm.iCounterID = m_penCounter->en_ulID;
          }
          SendEvent(encm,TRUE);
          m_tmLastUpdate = tmNow;
        }
      }

      // setup fade speed depending on music type
      if (m_mtCurrentMusic==MT_LIGHT) {
        m_tmFade = 2.0f;
      } else if (m_mtCurrentMusic==MT_MEDIUM) {
        m_tmFade = 1.0f;
      } else if (m_mtCurrentMusic==MT_HEAVY) {
        m_tmFade = 1.0f;
      } else if (m_mtCurrentMusic==MT_EVENT || m_mtCurrentMusic==MT_CONTINUOUS) {
        m_tmFade = 0.5f;
      }

      // fade all channels
      CrossFadeOneChannel(MT_LIGHT);
      CrossFadeOneChannel(MT_MEDIUM);
      CrossFadeOneChannel(MT_HEAVY);
      CrossFadeOneChannel(MT_EVENT);
      CrossFadeOneChannel(MT_CONTINUOUS);
    }
    return;
  }
};