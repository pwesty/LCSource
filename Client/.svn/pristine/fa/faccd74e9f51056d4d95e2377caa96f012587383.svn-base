#include "StdH.h"
#include <set>
#include <Engine/Build.h>
#include <Engine/Base/Console.h>
#include <Engine/Base/CRCTable.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Base/Shell.h>
#include <Engine/Base/Statistics_internal.h>
#include <Engine/Base/ListIterator.inl>
#include <Engine/Base/FileName.h>
#include <Engine/Base/Timer.h>
#include <Engine/Graphics/DrawPort.h>
#include <Engine/Rendering/Render_internal.h>
#include <Engine/Sound/SoundLibrary.h>
#include <Engine/Math/Float.h>
#include <Engine/Brushes/BrushArchive.h>
#include <Engine/Models/ModelObject.h>
#include <Engine/Ska/ModelInstance.h>

#include <Engine/Entities/InternalClasses.h>
#include <Engine/Entities/Precaching.h>
#include <Engine/Entities/ShadingInfo.h>
#include <Engine/Entities/EntityCollision.h>
#include <Engine/Entities/LastPositions.h>
#include <Engine/Entities/OptionData.h>
#include <Engine/World/WorldEntityHashing.h>
#include <Engine/Network/Server.h>
#include <Engine/Network/SessionState.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Compression.h>
#include <Engine/Network/PlayerSource.h>
#include <Engine/Network/PlayerTarget.h>
#include <Engine/Network/CommunicationInterface.h>
#include <Engine/Network/NetworkProfile.h>
#include <Engine/Network/LevelChange.h>
#include <Engine/Network/EntityHashing.h>
#include <Engine/Network/tcpipconnection.h>
#include <Engine/Network/MessageDefine.h>

#include <Engine/Templates/Stock_CFontData.h>
#include <Engine/Templates/Stock_CModelData.h>
#include <Engine/Templates/Stock_CAnimData.h>
#include <Engine/Templates/Stock_CTextureData.h>
#include <Engine/Templates/Stock_CSoundData.h>
#include <Engine/Templates/Stock_CEntityClass.h>
#include <Engine/Templates/Stock_CAnimSet.h>
#include <Engine/Templates/Stock_CMesh.h>
#include <Engine/Templates/Stock_CShader.h>
#include <Engine/Templates/Stock_CSkeleton.h>
#include <Engine/Templates/Stock_CModelInstance.h>
#include <Engine/Templates/DynamicContainer.cpp>
#include <Engine/Templates/DynamicArray.cpp>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Templates/StaticStackArray.cpp>

#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/interface/UIManager.h>
#include <Engine/interface/UISpinButton.h>
#include <Engine/interface/UIMessageBox.h>
#include <Engine/Interface/UIProcess.h>			// 이기환 추가 ( 12. 6 )
#include <Engine/Interface/UISiegeWarfareDoc.h>
#include <Engine/Interface/UIGuild.h>
#include <Engine/Interface/UISummon.h>
#include <Engine/Interface/UITeleport.h>
#include <Engine/Interface/UIQuickSlot.h>
#include <Engine/Interface/UIShop.h>
#include <Engine/Contents/Base/ChattingUI.h>		// yjpark
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIProduct.h>
#include <Engine/Interface/UIGamble.h>
#include <Engine/Interface/UIWildPetInfo.h>
#include <Engine/Interface/UISiegeWarfareNew.h>
#include <Engine/Interface/UIMix.h>
#include <Engine/Interface/UIReformSystem.h>
#include <Engine/Interface/UIGuildWarPortal.h>
#include <Engine/Contents/Base/UIPartyNew.h>
#include <Engine/Interface/UICompound.h>
#include <Engine/Contents/Base/UIChangeWeaponNew.h>
#include <Engine/Interface/UISocketSystem.h>
#include <Engine/Contents/Base/UIQuestNew.h>
#include <Engine/Contents/Base/UIQuestAccept.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/Quest.h>
#include <Engine/Contents/Base/Party.h>

#include <Engine/Effect/CTerrainEffect.h>	// yjpark
#include <Engine/JobInfo.h>
#include <Engine/PetInfo.h>
#include <Engine/SlaveInfo.h>
#include <Engine/LocalDefine.h>
#include <Engine/Info/MyInfo.h>
#include <Engine/Help/ItemHelp.h>
// WSS_NPROTECT 070402 --------------------------------->>
#ifndef NO_GAMEGUARD
//	#include <NPGameLib.h>
	#include <Engine\GameState.h>
//	extern ENGINE_API CNPGameLib npgl;
#endif
// -----------------------------------------------------<<
#include <Engine/GameState.h>
#include <Common/Packet/ptype_old_do_item.h>
#include <Common/Packet/ptype_old_do_exapet.h>
#include <Common/Packet/ptype_old_do_guild.h>
#include <Common/Packet/ptype_old_do_pk.h>
#include <Common/Packet/ptype_old_do_reform_system.h>
#include <Common/Packet/ptype_old_do_stash.h>
#include <Common/Packet/ptype_old_mempos.h>
#include <Common/Packet/ptype_old_do_action.h>
#include <Common/Packet/ptype_old_do_attack.h>
#include <Common/Packet/ptype_old_do_changejob.h>
#include <Common/Packet/ptype_old_do_move.h>
#include <Common/Packet/ptype_old_do_skill.h>
#include <Common/Packet/ptype_old_do_sskill.h>
#include <Engine/Object/ActorMgr.h>
#include <Engine/Loading.h>
#include <Common/Packet/ptype_old_do_friend.h>
#include <Common/Packet/ptype_old_extend.h>
#include <Common/Packet/ptype_attendance.h>
#include <Common/Packet/ptype_old_login.h>
#include <common/Packet/ptype_premium_char.h>

#define MAX_MOVE_LIST		(20)
#define	DEF_SKILL_SEND_DELAY	200

// pointer to global instance of the only game object in the application
CNetworkLibrary *_pNetwork= NULL;

extern BOOL _bNeedPretouch;
extern BOOL _bMultiPlayer = FALSE;
extern INDEX _ctEntities = 0;
extern BOOL _bSendEnumRequest = FALSE;
extern BOOL _bEnumerating = FALSE;
extern LevelChangePhase _lphCurrent = LCP_NOCHANGE;
extern BOOL con_bCapture; 
extern CTString con_strCapture;

static CWorld *_pwoCurrentWorld = NULL;
static FLOAT _bStartDemoRecordingNextTime = FALSE;
static FLOAT _bStopDemoRecordingNextTime = FALSE;
static INDEX dem_iRecordedNumber = 0;
extern INDEX dem_iAnimFrame = -1;
static INDEX dem_iMotionBlurInterFrames = 0;

// network control
extern INDEX net_bNetworkGame = FALSE;
extern INDEX net_bMmoGame = FALSE;					// by seo-40225
extern INDEX ser_iRememberBehind = 0;
extern INDEX ser_bClientsMayPause = TRUE;
extern INDEX ser_bEnumeration  = TRUE;
//extern INDEX ser_bHeartbeatGameSpy = TRUE;
extern FLOAT ser_tmKeepAlive = 0.1f;
extern FLOAT ser_tmPingUpdate = 3.0f;
extern INDEX ser_bWaitFirstPlayer = 0;
extern INDEX ser_iMaxAllowedBPS = 10000;
extern CTString ser_strIPMask = "";
extern CTString ser_strNameMask = "";
extern INDEX ser_bInverseBanning = FALSE;
extern CTString ser_strMOTD = "";
extern FLOAT ser_fAckDropRate = 0.0f;
extern FLOAT ser_fPositionTreshold = 0.05f;
extern FLOAT ser_fOrientationTreshold = 0.15f;
extern INDEX ser_iSkipTicks = 0;
extern INDEX ser_iMaxBufferSize = SERVER_MAX_BUFFER_SIZE;
extern INDEX ser_iMaxPendingData = 0;
extern INDEX ser_iMaxTotalData = 0;
extern INDEX cli_iSendBehind = 3;
extern INDEX cli_iBufferActions = 10;
extern INDEX cli_iMaxBPS = 75000;
extern INDEX cli_iMinBPS = 0;
extern INDEX cli_iFPSLimit = 30;
extern ENGINE_API INDEX  cli_iCurrentFPS = 10000;

extern FLOAT cli_fTickDropRate = 0.0f;
extern INDEX net_iCompression = 1;
extern INDEX net_bLookupHostNames = FALSE;
extern INDEX net_bReportPackets = FALSE;
extern INDEX net_bReportServerTraffic = FALSE;
extern INDEX net_bReportClientTraffic = TRUE;//FALSE;
extern INDEX net_bReportDemoTraffic = FALSE;
extern INDEX net_iDemoQuality = 128;
extern INDEX net_bLocalPrediction = TRUE;
extern INDEX net_iMaxSendRetries = 10;
extern FLOAT net_fSendRetryWait = 0.5f;
extern INDEX net_bReportTraffic = FALSE;
extern INDEX net_bReportICMPErrors = FALSE;
extern INDEX net_bReportMiscErrors = FALSE;
extern INDEX net_bLerping       = TRUE;
extern INDEX net_iGraphBuffer = 100;
extern INDEX net_bDumpStreamBlocks = 0;
extern INDEX net_bDumpConnectionInfo = 0;

extern INDEX net_iPort = 25600;
extern CTString net_strLocalHost = "";
extern CTString net_strLocationCode = "";
extern CTString net_strConnectPassword = "";
extern CTString net_strAdminPassword = "";
extern CTString net_strVIPPassword = "";
extern CTString net_strObserverPassword = "";
extern INDEX net_iVIPReserve = 0;
extern INDEX net_iMaxObservers = 16;
extern INDEX net_iMaxClients = 0;
extern FLOAT net_tmConnectionTimeout = 60.0f;
extern FLOAT net_tmProblemsTimeout = 5.0f;
extern FLOAT net_tmDisconnectTimeout = 60.0f;
extern INDEX net_bReportCRC = FALSE;
extern FLOAT net_fDropPackets = 0.0f;
extern INDEX net_iHashCompartments = 50;
extern INDEX net_iHashSlotsPerComp = 20;
extern INDEX net_iHashStep = 5;

extern INDEX ent_bReportSpawnInWall = FALSE;
extern FLOAT cmd_tmTick = 0.0f;
extern CTString cmd_cmdOnTick = "";
extern CTString cmd_strChatSender = "";
extern CTString cmd_strChatMessage = "";
extern CTString cmd_cmdOnChat = "";
extern INDEX net_ctChatMessages = 0;  // counter for incoming chat messages

extern CPacketBufferStats _pbsSend;
extern CPacketBufferStats _pbsRecv;

class CGatherCRC {
public:
  BOOL bOld;
  CGatherCRC();
  ~CGatherCRC();
};

CGatherCRC::CGatherCRC() {
  bOld = CRCT_bGatherCRCs;
}

CGatherCRC::~CGatherCRC() {
  CRCT_bGatherCRCs = bOld;
}

// precache control
extern INDEX _precache_NONE      = PRECACHE_NONE;
extern INDEX _precache_SMART     = PRECACHE_SMART;
extern INDEX _precache_ALL       = PRECACHE_ALL;
extern INDEX _precache_PARANOIA  = PRECACHE_PARANOIA;
extern INDEX gam_iPrecachePolicy = _precache_ALL;
extern INDEX _precache_bNowPrecaching = FALSE;

extern INDEX dbg_bBreak = FALSE;
extern INDEX gam_bPretouch = FALSE;

extern FLOAT phy_fCollisionCacheAhead  = 5.0f;
extern FLOAT phy_fCollisionCacheAround = 1.5f;
extern INDEX phy_bNonZoningClipMove = TRUE;
extern INDEX phy_bNonZoningClipRay = TRUE;

extern INDEX shd_bCacheAll;
extern void RendererInfo(void);
extern void ClearRenderer(void);
extern void DumpSkaStringToIDTable(void);
extern void DumpShellHashTable(void);

// cache all shadowmaps now
extern void CacheShadows(void)
{
	// mute all soundsf
	_pSound->Mute();
	CWorld *pwo = (CWorld*)_pShell->GetINDEX("pwoCurrentWorld");
	if( pwo!=NULL) {
		pwo->wo_baBrushes.CacheAllShadowmaps();
		CPrintF( TRANS("All shadows recached"));
		if( shd_bCacheAll) CPrintF(".\n");
		else CPrintF( TRANS(", but not for long.\n(precache all shadows function is disabled)\n"));
	}
	// mark that we need pretouching
	_bNeedPretouch = TRUE;
}

// check if a name or IP matches a mask
extern BOOL MatchesBanMask(const CTString &strString, const CTString &strMask)
{
	CTString strRest = strMask;
	CTString strLine;
	while(strRest!="") {
		strLine = strRest;
		strLine.OnlyFirstLine();
		strRest.RemovePrefix(strLine);
		strRest.DeleteChar(0);
		
		if (strString.Matches(strLine)) {
			return TRUE;
		}
	}
	return FALSE;
}

extern CTString RemoveSubstring(const CTString &strFull, const CTString &strSub);

static void AddIPMask(const CTString &strIP)
{
	ser_strIPMask+= strIP+"\n";
}

static void RemIPMask(const CTString &strIP)
{
	ser_strIPMask = RemoveSubstring(ser_strIPMask, strIP+"\n");
}

static void AddNameMask(const CTString &strName)
{
	ser_strNameMask += strName+"\n";
}

static void RemNameMask(const CTString &strName)
{
	ser_strNameMask = RemoveSubstring(ser_strNameMask, strName+"\n");
}

static void StartDemoRecording(void)
{
	_bStartDemoRecordingNextTime = TRUE;
}

static void StopDemoRecording(void)
{
	_bStopDemoRecordingNextTime = TRUE;
}

void ReinitializeLevel(const CTString &strWorld) {
	if (strWorld.Length() > 0) {
		CWorld wldTemp;
		try {
			wldTemp.Load_t(CTString("Levels\\") + strWorld + CTString(".wld"));
			CPrintF("Reinitialization successfull\n");
		} catch (char *strError) {
			CPrintF("Reinitialization failed. Error: %s\n", strError);
		}
	}
	extern void FreeUnusedStock(void);
	FreeUnusedStock();
}

void DumpPlayer(INDEX iEntityID)
{
	CEntity* penEntity;
	if (!_pNetwork->ga_World.EntityExists(iEntityID,penEntity)) {
		CPrintF(TRANS("Entity with ID %X does not exist!\n"),iEntityID);
		return;
	} if (!penEntity->IsPlayer()) {
		CPrintF(TRANS("Entity with ID %X is not a player!\n"),iEntityID);
		return;
	} else {
		((CPlayerEntity*)penEntity)->DumpToConsole();
	}       
}

void DumpAllPlayers(INDEX iVerbose)
{
	for(INDEX iplt=0; iplt<_pNetwork->ga_srvServer.srv_apltPlayers.Count(); ++iplt) {
		CPlayerTarget &plt = _pNetwork->ga_srvServer.srv_apltPlayers[iplt];
		if (plt.plt_bActive) {
			if (!iVerbose) {
				CPrintF(TRANS("Entity ID: %X, Name: %s\n"),plt.plt_penPlayerEntity->en_ulID,plt.plt_penPlayerEntity->en_pcCharacter.pc_strName);
			} else {
				plt.plt_penPlayerEntity->DumpToConsole();
				CPrintF("\n");
			}
		}
	}
}

void DumpClassReferences(void)
{
	ULONG aulClasses[2048] = {0};
	
	CPrintF(TRANS("Number of entities of each class:\n\n"));
	FOREACHINDYNAMICCONTAINER(_pNetwork->ga_World.wo_cenEntities, CEntity, iten) {
		CEntity* penEntity = iten;
		ULONG ulClassID = penEntity->en_pecClass->ec_pdecDLLClass->dec_iID;
		if ((ulClassID < 2048) && (ulClassID >= 0)) {
			aulClasses[ulClassID]++;
		}
	}
  
	for (INDEX iClass=0;iClass < 2048;++iClass) {
		if (aulClasses[iClass]==0) {
			continue;
		}
		EntityClassEntry* peceEntry = FindClassInContainer(iClass);
		CPrintF(TRANS("%d  \t%d  \t"),iClass,aulClasses[iClass]);
		if (peceEntry != NULL) {
			CPrintF(TRANS("%s\n"),peceEntry->ece_fnmEntityClass);
		} else {
			CPrintF(TRANS("N/A\n"));
		}
	}
}

void DumpEntityHashEfficiency(void)
{
	_pNetwork->ga_World.wo_ehEntityHashContainer.ReportEfficiency();
}

void DumpWorldEntityHashEfficiency(void)
{
	_pNetwork->ga_World.wo_whWorldEntityContainer.ReportEfficiency();
}

// bVerbose == 0 - dump entity id, number of references and entity class name
// bVerbose == 1 - for each reference dump it's holder's name
// bVerbose == 2 - also dump info on pending events
void DumpEntityInfo(INDEX iVerbose,BOOL bAll) 
{
	if (bAll) {
		CPrintF(TRANS("All entities list:\n"));
		FOREACHINDYNAMICCONTAINER(_pNetwork->ga_World.wo_cenAllEntities, CEntity, iten) {
			CEntity* penEntity = iten;
			CPrintF(TRANS("  EntityID: %X     References: %d    Class: %s\n"),iten->en_ulID,iten->en_ctReferences,iten->en_pecClass->GetName());
			if (iVerbose>=1) {
				_pNetwork->ga_World.DumpReferenceInfo(penEntity);
				
				if (iVerbose>=2) {
					if (IsDerivedFromClass(penEntity,"MovableEntity")) {
						INDEX ctStates = ((CRationalEntity*)penEntity)->en_stslStateStack.Count();
						for(INDEX iState=ctStates-1; iState>=0; iState--) {
							SLONG slState = ((CRationalEntity*)penEntity)->en_stslStateStack[iState];
							CPrintF("  Event pending: 0x%08x %s\n", slState, 
								((CRationalEntity*)penEntity)->en_pecClass->ec_pdecDLLClass->HandlerNameForState(slState));
						};
					}
				}
			}
		}
	} else {
		CPrintF(TRANS("Existing entities list:\n"));
		FOREACHINDYNAMICCONTAINER(_pNetwork->ga_World.wo_cenEntities, CEntity, iten) {
			CEntity* penEntity = iten;
			CPrintF(TRANS("  EntityID: %X     References: %d    Class: %s\n"),iten->en_ulID,iten->en_ctReferences,iten->en_pecClass->GetName());
			if (iVerbose>=1) {
				_pNetwork->ga_World.DumpReferenceInfo(penEntity);
				
				if (iVerbose>=2) {
					if (IsDerivedFromClass(penEntity,"MovableEntity")) {
						INDEX ctStates = ((CRationalEntity*)penEntity)->en_stslStateStack.Count();
						for(INDEX iState=ctStates-1; iState>=0; iState--) {
							SLONG slState = ((CRationalEntity*)penEntity)->en_stslStateStack[iState];
							CPrintF("  Event pending: 0x%08x %s\n", slState, 
								((CRationalEntity*)penEntity)->en_pecClass->ec_pdecDLLClass->HandlerNameForState(slState));
						};
					}
				}
			}
		}
	}
}

void DumpServerStats() {
	if (_pNetwork != NULL) {
		_pNetwork->ga_srvServer.DumpStats();
	}
}

static void DumpEntity(INDEX ulEntityID)
{
	CEntity* penEntity = NULL;
	
	if (!_pNetwork->ga_World.EntityExists(ulEntityID,penEntity)) {
		CPrintF(TRANS("Entity with ID %X does not exist!\n"),ulEntityID);
		return;
	} else {
		CPrintF(TRANS("Entity ID: %X\n"),ulEntityID);
		CPrintF(TRANS("   Entity class: %s\n"),penEntity->en_pecClass->GetName());
		CPrintF(TRANS("   Flags:  0x%X    Physics flags: 0x%X\n"),penEntity->en_ulFlags,penEntity->en_ulPhysicsFlags);
		if (penEntity->IsSentOverNet()) {
			CPrintF(TRANS("   IsSentOverNet = TRUE\n"));
		} else {
			CPrintF(TRANS("   IsSentOverNet = FALSE\n"));
		}
		if (penEntity->IsFlagOn(ENF_ALIVE)) {
			CPrintF(TRANS("   ENF_ALIVE = TRUE\n"));
		} else {
			CPrintF(TRANS("   ENF_ALIVE = FALSE\n"));
		}
		CPrintF(TRANS("   Placement: x: %6.2f, y: %6.2f, z:%6.2f\n"),penEntity->en_plPlacement.pl_PositionVector(1)
			,penEntity->en_plPlacement.pl_PositionVector(2),penEntity->en_plPlacement.pl_PositionVector(3));
		
		switch (penEntity->en_RenderType) {
		case CEntity::RT_ILLEGAL        : { CPrintF(TRANS("   RenderType: RT_ILLEGAL\n")); break;}
		case CEntity::RT_NONE           : { CPrintF(TRANS("   RenderType: RT_NONE\n")); break;}
		case CEntity::RT_MODEL          : { CPrintF(TRANS("   RenderType: RT_MODEL\n")); break;}
		case CEntity::RT_BRUSH          : { CPrintF(TRANS("   RenderType: RT_BRUSH\n")); break;}
		case CEntity::RT_EDITORMODEL    : { CPrintF(TRANS("   RenderType: RT_EDITORMODEL\n")); break;}
		case CEntity::RT_VOID           : { CPrintF(TRANS("   RenderType: RT_VOID\n")); break;}
		case CEntity::RT_FIELDBRUSH     : { CPrintF(TRANS("   RenderType: RT_FIELDBRUSH\n")); break;}
		case CEntity::RT_SKAMODEL       : { CPrintF(TRANS("   RenderType: RT_SKAMODEL\n")); break;}
		case CEntity::RT_SKAEDITORMODEL : { CPrintF(TRANS("   RenderType: RT_SKAEDITORMODEL\n")); break;}
		case CEntity::RT_TERRAIN        : { CPrintF(TRANS("   RenderType: RT_TERRAIN\n")); break;}
		};
		_pNetwork->ga_World.DumpReferenceInfo(penEntity);
	}       
}         

static void NetworkInfo(void)
{
	CPrintF("*Network library information:\n");
	CPrintF("Entities existing: %d\n", _ctEntities);
	
	CPrintF("Server:\n");
	if (_pNetwork->ga_srvServer.srv_bActive) {
		CPrintF("  last processed tick: %g\n", _pNetwork->ga_srvServer.srv_tmLastProcessedTick);
		CPrintF("  last processed sequence: %d\n", _pNetwork->ga_srvServer.srv_iLastProcessedSequence);
		CPrintF("  players:\n");
		for(INDEX iplt=0; iplt<_pNetwork->ga_srvServer.srv_apltPlayers.Count(); ++iplt) {
			CPlayerTarget &plt = _pNetwork->ga_srvServer.srv_apltPlayers[iplt];
			if (plt.plt_bActive) {
				CPrintF("    %2d(%2d):'%s'@client%2d: (%dact)\n", 
					iplt, plt.plt_Index, RemoveSpecialCodes(plt.plt_pcCharacter.GetNameForPrinting()),
					plt.plt_iClient, plt.plt_abReceived.GetCount());
			}
		}
		CPrintF("  clients:\n");
		for(INDEX iSession=0; iSession<_pNetwork->ga_srvServer.srv_assoSessions.Count(); ++iSession) {
			CSessionSocket &sso = _pNetwork->ga_srvServer.srv_assoSessions[iSession];
			if (sso.sso_bActive) {
				CPrintF("  %2d:'%s'\n", iSession, _cmiComm.Server_GetClientName(iSession)),
					CPrintF("    buffer: %dblk=%dk\n", 
					sso.sso_nsBuffer.GetUsedBlocks(),
					sso.sso_nsBuffer.GetUsedMemory()/1024);
				CPrintF("    state:");
				if (sso.sso_iDisconnectedState>0) {
					CPrintF("    disconnecting");
				} else if (sso.sso_bSendStream) {
					CPrintF("    connected");
				} else {
					CPrintF("    connecting");
				}
				CPrintF("\n");
			}
		}
	} else {
		CPrintF("  not a server\n");
	}
	CPrintF("Session state:\n");
	CPrintF("  buffer: (%dblk)%dk\n", 
		_pNetwork->ga_sesSessionState.ses_nsGameStream.GetUsedBlocks(),
		_pNetwork->ga_sesSessionState.ses_nsGameStream.GetUsedMemory()/1024);
	CPrintF("  last processed tick: %g\n", _pNetwork->ga_sesSessionState.ses_tmLastProcessedTick);
	CPrintF("  last processed sequence: %d\n", _pNetwork->ga_sesSessionState.ses_iLastProcessedSequence);
	CPrintF("  level change: %d\n", _pNetwork->ga_sesSessionState.ses_ubNumLevelChanges);
	for(INDEX iplt=0; iplt<_pNetwork->ga_srvServer.srv_apltPlayers.Count(); iplt++) {
		CPlayerTarget &plt = _pNetwork->ga_srvServer.srv_apltPlayers[iplt];
		if (plt.plt_bActive) {
			ULONG ulID = -1;
			if (plt.plt_penPlayerEntity!=NULL) {
				ulID = plt.plt_penPlayerEntity->en_ulID;
			}
			CPrintF("  player %2d (ID:%d)\n", iplt, ulID);
		}
	}	
	if (TIMER_PROFILING) {		
		CTString strNetProfile;		
		_pfNetworkProfile.Report(strNetProfile);		
		CPrintF(strNetProfile);		
	}	
}

static void ListPlayers(void)
{
	CPrintF("player list:\n");
	if (!_pNetwork->ga_srvServer.srv_bActive) {
		CPrintF("  <not a server>\n");
		return;
	}
	
	CPrintF("  client# name\n");
	CPrintF("  ----------------------\n");
	for(INDEX iplt=0; iplt<_pNetwork->ga_srvServer.srv_apltPlayers.Count(); iplt++) {
		CPlayerTarget &plt = _pNetwork->ga_srvServer.srv_apltPlayers[iplt];
		if (plt.plt_bActive) {
			CPrintF("     %-2d   %s\n", plt.plt_iClient, RemoveSpecialCodes(plt.plt_pcCharacter.GetNameForPrinting()));
		}
	}
	CPrintF("  ----------------------\n");
}

static void KickClient(INDEX iClient, const CTString &strReason)
{
	if (!_pNetwork->IsServer()) {
		CPrintF( TRANS("Only server can kick people!\n"));
		return;
	}
	iClient = Clamp(iClient, INDEX(0), INDEX(NET_MAXGAMECOMPUTERS));
	if (!_pNetwork->ga_srvServer.srv_assoSessions[iClient].IsActive()) {
		CPrintF(TRANS("Client not connected!\n"));
		return;
	}
	if (iClient == 0) {
		CPrintF(TRANS("Can't kick local client!\n"));
		return;
	}
	CPrintF( TRANS("Kicking %d with explanation '%s'...\n"), iClient, strReason);
	_pNetwork->ga_srvServer.SendDisconnectMessage(iClient, "Admin: "+strReason);
}

static void KickByName(const CTString &strName, const CTString &strReason)
{
	if (!_pNetwork->IsServer()) {
		CPrintF( TRANS("Only server can kick people!\n"));
		return;
	} 
	for(INDEX iplt=0; iplt<_pNetwork->ga_srvServer.srv_apltPlayers.Count(); iplt++) {
		CPlayerTarget &plt = _pNetwork->ga_srvServer.srv_apltPlayers[iplt];
		if (plt.plt_bActive && plt.plt_pcCharacter.GetNameForPrinting().Undecorated().Matches(strName)) {
			KickClient(plt.plt_iClient, strReason);
		}
	}
}

static void Admin(const CTString &strCommand)
{
/* //0522 kwon 삭제.
CNetworkMessage nm(MSG_ADMIN_COMMAND);
nm<<net_strAdminPassword<<strCommand;
_pNetwork->SendToServerReliable(nm);
	*/
}

// Print to file or to console
extern void FCPrintF(CTFileStream *pstrm,const char *strFormat, ...);

// Write stock info
extern void StockInfoToStream(CTFileStream *pstrm)
{
	// find memory used by shadowmap (both cached and uploaded)
	INDEX ctCachedShadows=0, ctDynamicShadows=0, ctFlatShadows=0;
	SLONG slStaticMemory=0,  slDynamicMemory=0,  slUploadMemory=0;
	SLONG slShdBytes=0,  slSlackMemory=0,    slFlatMemory=0;
	INDEX ct256=0, ct128=0, ct64=0, ct32=0, ct16=0; 
	SLONG sl256Memory=0, sl128Memory=0, sl64Memory=0, sl32Memory=0, sl16Memory=0; 
	
	if( _pGfx!=NULL)
	{
		FLOAT fSlackRatio;
		FOREACHINLIST( CShadowMap, sm_lnInGfx, _pGfx->gl_lhCachedShadows, itsm)
		{ // get polygon size in pixels (used portion of shadowmap)
			SLONG slStaticSize, slDynamicSize, slUploadSize;
			BOOL bIsFlat = itsm->GetUsedMemory( slStaticSize, slDynamicSize, slUploadSize, fSlackRatio);
			SLONG slTotalSize = slDynamicSize+slUploadSize;
			if( bIsFlat) {
				slStaticMemory += 4;
				slTotalSize    += 4;
				slFlatMemory   += slStaticSize;
				ctFlatShadows++;
			} else {
				slStaticMemory += slStaticSize;
				slTotalSize    += slStaticSize;
				if( slTotalSize>0) ctCachedShadows++;
			}
			if( slDynamicSize>0) {
				slDynamicMemory += slDynamicSize;
				ctDynamicShadows++;
			}
			slUploadMemory  += slUploadSize;
			slShdBytes  += slTotalSize + sizeof(CShadowMap);
			slSlackMemory   += slTotalSize*fSlackRatio;
			
			if( !bIsFlat) { // by size ...
				if(      slStaticSize>128*1024) { ct256++; sl256Memory+=slTotalSize; }
				else if( slStaticSize> 64*1024) { ct128++; sl128Memory+=slTotalSize; }
				else if( slStaticSize> 32*1024) { ct64++;  sl64Memory +=slTotalSize; }
				else if( slStaticSize> 16*1024) { ct32++;  sl32Memory +=slTotalSize; }
				else if( slStaticSize> 0)       { ct16++;  sl16Memory +=slTotalSize; }
			}
		}
		// report shadowmap memory usage (if any)
		if( slShdBytes>0) {
			FCPrintF(pstrm, "\nCached shadowmaps:\n");
			FCPrintF(pstrm, "    Total: %d in %d KB with %d%% (%d KB) of slack space\n", ctCachedShadows, slShdBytes/1024, slSlackMemory*100/slShdBytes, slSlackMemory/1024);
			FCPrintF(pstrm, "   Static: %d KB\n", slStaticMemory/1024);
			FCPrintF(pstrm, "   Upload: %d KB\n", slUploadMemory/1024);
			FCPrintF(pstrm, "  Dynamic: %d in %d KB\n", ctDynamicShadows, slDynamicMemory/1024);
			if( ctCachedShadows<1) ctCachedShadows=1; // for percentage calc
			FCPrintF(pstrm, "    Flats: %d (%d%%) with %d KB saved\n", ctFlatShadows, ctFlatShadows*100/ctCachedShadows, slFlatMemory/1024);
			FCPrintF(pstrm, "of size:\n");
			FCPrintF(pstrm, "    >128K: %4d in %d KB\n", ct256, sl256Memory/1024);
			FCPrintF(pstrm, "  128-64K: %4d in %d KB\n", ct128, sl128Memory/1024);
			FCPrintF(pstrm, "   64-32K: %4d in %d KB\n", ct64,  sl64Memory /1024);
			FCPrintF(pstrm, "   32-16K: %4d in %d KB\n", ct32,  sl32Memory /1024);
			FCPrintF(pstrm, "    <=16K: %4d in %d KB\n", ct16,  sl16Memory /1024);
		}
	}
	
	// report world stats
	INDEX ctEntities=0, ctShadowLayers=0, ctPolys=0,    ctPlanes=0,   ctEdges=0,    ctVertices=0, ctSectors=0;
	SLONG slEntBytes=0, slLyrBytes=0,     slPlyBytes=0, slPlnBytes=0, slEdgBytes=0, slVtxBytes=0, slSecBytes=0;
	SLONG slCgrBytes=0;
	
	CWorld *pwo = NULL;
	// if pwoCurrentWorld exists
	if(_pShell->GetSymbol("pwoCurrentWorld",TRUE)!=NULL) {
		pwo = (CWorld*)_pShell->GetINDEX("pwoCurrentWorld");
	}
	if( pwo!=NULL)
	{
		// report count of and memory used by entities
		FOREACHINDYNAMICCONTAINER( pwo->wo_cenEntities, CEntity, iten) {
			ctEntities++;
			slEntBytes += iten->GetUsedMemory();
		}
		
		// report shadow layers and world geometry memory usage
		FOREACHINDYNAMICARRAY( pwo->wo_baBrushes.ba_abrBrushes, CBrush3D, itbr) // for all brush entities in the world
		{ 
			// skip brush without entity
			if( itbr->br_penEntity==NULL) continue; 
			
			// for each mip
			FOREACHINLIST( CBrushMip, bm_lnInBrush, itbr->br_lhBrushMips, itbm) 
			{ 
				// for each sector in the brush mip
				FOREACHINDYNAMICARRAY( itbm->bm_abscSectors, CBrushSector, itbsc) 
				{
					// add sector class memory usage to polygons memory
					ctSectors++;
					slSecBytes += itbsc->GetUsedMemory();
					
					// add each vertex and working vertex in sector
					ctVertices += itbsc->bsc_abvxVertices.Count();
					FOREACHINSTATICARRAY( itbsc->bsc_abvxVertices, CBrushVertex,   itbvx) slVtxBytes += itbvx->GetUsedMemory();
					FOREACHINSTATICARRAY( itbsc->bsc_awvxVertices, CWorkingVertex, itwvx) slVtxBytes += 32; // aligned to 32 bytes!
					
					// add each plane and working plane in sector
					ctPlanes += itbsc->bsc_abplPlanes.Count();
					FOREACHINSTATICARRAY( itbsc->bsc_abplPlanes, CBrushPlane,   itbpl) slPlnBytes += itbpl->GetUsedMemory();
					FOREACHINSTATICARRAY( itbsc->bsc_awplPlanes, CWorkingPlane, itwpl) slPlnBytes += sizeof(CWorkingPlane);
					
					// add each edge and working edge in sector
					ctEdges += itbsc->bsc_abedEdges.Count();
					FOREACHINSTATICARRAY( itbsc->bsc_abedEdges, CBrushEdge,   itbed) slEdgBytes += itbed->GetUsedMemory();
					FOREACHINSTATICARRAY( itbsc->bsc_awedEdges, CWorkingEdge, itwed) slEdgBytes += sizeof(CWorkingEdge);
					
					// for each polygon in sector
					ctPolys += itbsc->bsc_abpoPolygons.Count();
					FOREACHINSTATICARRAY( itbsc->bsc_abpoPolygons, CBrushPolygon, itbpo) { 
						CBrushPolygon &bpo = *itbpo;
						slPlyBytes += bpo.GetUsedMemory();
						// count in the shadow layers (if any)
						if( bpo.bpo_smShadowMap.bsm_lhLayers.IsEmpty()) continue; // skip polygon without shadowmap
						ctShadowLayers += bpo.bpo_smShadowMap.GetShadowLayersCount();
						slLyrBytes += bpo.bpo_smShadowMap.GetUsedMemory();
					}
				}
			}
		} // add in memory used by collision grid
		extern SLONG  GetCollisionGridMemory( CCollisionGrid *pcg);
		slCgrBytes += GetCollisionGridMemory( pwo->wo_pcgCollisionGrid);
	}
	
	// stock info
	const DOUBLE dToMB = 1.0/1024.0/1024.0;
	const FLOAT fTexBytes = dToMB * _pTextureStock->CalculateUsedMemory();
	const FLOAT fClsBytes = dToMB * _pEntityClassStock->CalculateUsedMemory();
	const FLOAT fMdlBytes = dToMB * _pModelStock->CalculateUsedMemory();
	const FLOAT fSndBytes = dToMB * _pSoundStock->CalculateUsedMemory();
	const FLOAT fMshBytes = dToMB * _pMeshStock->CalculateUsedMemory();
	const FLOAT fAstBytes = dToMB * _pAnimSetStock->CalculateUsedMemory();
	const FLOAT fShaBytes = dToMB * _pShaderStock->CalculateUsedMemory();
	const FLOAT fSkaBytes = dToMB * _pSkeletonStock->CalculateUsedMemory();
	const FLOAT fSmcBytes = dToMB * _pModelInstanceStock->CalculateUsedMemory();
	
	extern INDEX _ctExistingModelInstances;
	FCPrintF(pstrm, "\nStock information:\n");
	FCPrintF(pstrm, "     Textures: %5d (%5.2f MB)\n", _pTextureStock->GetTotalCount(), fTexBytes);
	FCPrintF(pstrm, "   ShadowMaps: %5d (%5.2f MB)\n", ctCachedShadows, slShdBytes*dToMB);
	FCPrintF(pstrm, "     Entities: %5d (%5.2f MB)\n", ctEntities,      slEntBytes*dToMB);
	FCPrintF(pstrm, "       Sounds: %5d (%5.2f MB)\n", _pSoundStock->GetTotalCount(), fSndBytes);
	FCPrintF(pstrm, "\n");
	FCPrintF(pstrm, "      Sectors: %5d (%5.2f MB)\n", ctSectors,  slSecBytes*dToMB);
	FCPrintF(pstrm, "       Planes: %5d (%5.2f MB)\n", ctPlanes,   slPlnBytes*dToMB);
	FCPrintF(pstrm, "        Edges: %5d (%5.2f MB)\n", ctEdges,    slEdgBytes*dToMB);
	FCPrintF(pstrm, "     Polygons: %5d (%5.2f MB)\n", ctPolys,    slPlyBytes*dToMB);
	FCPrintF(pstrm, "     Vertices: %5d (%5.2f MB)\n", ctVertices, slVtxBytes*dToMB);
	FCPrintF(pstrm, " ShadowLayers: %5d (%5.2f MB)\n", ctShadowLayers, slLyrBytes*dToMB);
	FCPrintF(pstrm, "\n");
	FCPrintF(pstrm, "      Classes: %5d (%5.2f MB)\n", _pEntityClassStock->GetTotalCount(),fClsBytes);
	FCPrintF(pstrm, "       Models: %5d (%5.2f MB)\n", _pModelStock->GetTotalCount(),    fMdlBytes);
	FCPrintF(pstrm, "       Meshes: %5d (%5.2f MB)\n", _pMeshStock->GetTotalCount(),     fMshBytes);
	FCPrintF(pstrm, "    Skeletons: %5d (%5.2f MB)\n", _pSkeletonStock->GetTotalCount(), fSkaBytes);
	FCPrintF(pstrm, "     AnimSets: %5d (%5.2f MB)\n", _pAnimSetStock->GetTotalCount(),  fAstBytes);
	FCPrintF(pstrm, "      Shaders: %5d (%5.2f MB)\n", _pShaderStock->GetTotalCount(),   fShaBytes);
	FCPrintF(pstrm, "         Smcs: %5d (%5.2f MB) (%d)\n", _pModelInstanceStock->GetTotalCount(), fSmcBytes, _ctExistingModelInstances);
	FCPrintF(pstrm, "\n");
	FCPrintF(pstrm, "CollisionGrid: %.2f MB\n", slCgrBytes*dToMB);
	FCPrintF(pstrm, "--------------\n");
	FCPrintF(pstrm, "        Total: %.2f MB\n", fTexBytes+fSndBytes+fMdlBytes+fMshBytes+fSkaBytes+fAstBytes+fShaBytes+fSmcBytes
		+ (slShdBytes+slEntBytes+slSecBytes+slPlnBytes+slEdgBytes+slPlyBytes+slVtxBytes+slLyrBytes+slCgrBytes)*dToMB);
	FCPrintF(pstrm, "\n");
}

static void StockInfo(void)
{
	StockInfoToStream(NULL);
}

extern void StockDumpToStream(CTFileStream *pstrm)
{
	pstrm->PutLine_t("Animations:");
	_pAnimStock->DumpMemoryUsage_t(*pstrm);
	pstrm->PutLine_t("Textures:");
	_pTextureStock->DumpMemoryUsage_t(*pstrm);
	pstrm->PutLine_t("Models:");
	_pModelStock->DumpMemoryUsage_t(*pstrm);
	pstrm->PutLine_t("Meshes:");
	_pMeshStock->DumpMemoryUsage_t(*pstrm);
	pstrm->PutLine_t("Skeletons:");
	_pSkeletonStock->DumpMemoryUsage_t(*pstrm);
	pstrm->PutLine_t("Animsets:");
	_pAnimSetStock->DumpMemoryUsage_t(*pstrm);
	pstrm->PutLine_t("Shaders:");
	_pShaderStock->DumpMemoryUsage_t(*pstrm);
	pstrm->PutLine_t("ModelInstances:");
	_pModelInstanceStock->DumpMemoryUsage_t(*pstrm);
	pstrm->PutLine_t("Sounds:");
	_pSoundStock->DumpMemoryUsage_t(*pstrm);
	pstrm->PutLine_t("Classes:");
	_pEntityClassStock->DumpMemoryUsage_t(*pstrm);
}

static void StockDump(void)
{
	try {
		CTFileStream strm;
		CTFileName fnm = CTString("Temp\\StockDump.txt");
		strm.Create_t(fnm);
		// Dump stock to file
		StockDumpToStream(&strm);
		CPrintF("Dumped to '%s'\n", CTString(fnm));
	} catch (char *strError) {
		CPrintF("Error: %s\n", strError);
	}
}

// free all unused stocks
extern void FreeUnusedStock(void)
{
	// free all unused stocks
	_pEntityClassStock->FreeUnused();
	_pModelStock->FreeUnused();
	_pSoundStock->FreeUnused();
	_pFontStock->FreeUnused();
	_pModelInstanceStock->FreeUnused();
	_pTextureStock->FreeUnused();
	_pAnimStock->FreeUnused();
	_pMeshStock->FreeUnused();
	_pSkeletonStock->FreeUnused();
	_pAnimSetStock->FreeUnused();
	_pShaderStock->FreeUnused();
}

CNetworkLibrary::CNetworkLibrary(void) 
	: ga_IsServer(FALSE)               // is not server
	, ga_bFirstMainLoop(TRUE)          // MainLoop will be run for the first time	
	, ga_bDemoRec(FALSE)               // not recording demo
	, ga_bDemoPlay(FALSE)              // not playing demo
	, ga_bDemoPlayFinished(FALSE)      // demo not finished
	, ga_srvServer(*new CServer)
	, ga_sesSessionState(*new CSessionState)
#ifdef NDEBUG
	, m_ubGMLevel( 0 )
#else
	, m_ubGMLevel( 1 )
#endif
	, ga_tvDemoTimerLastTime((__int64)0)
	, m_nSkillSendTime(0)
{
	// EDIT : BS : 패킷 암호화
#ifdef CRYPT_NET_MSG
	CNM_InitKeyValue(&cnmKey);
#ifndef CRYPT_NET_MSG_MANUAL
	CNM_InitKeyValue(&cnmKeyServer);
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#else
	cnmKey = CNM_INIT_KEY;
#endif // #ifdef CRYPT_NET_MSG
#ifdef CRYPT_NET_MSG
	CNM_InitKeyValue(&subHelperKey);
#ifndef CRYPT_NET_MSG_MANUAL
	CNM_InitKeyValue(&subHelperKeyServer);
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#else
	subHelperKey = CNM_INIT_KEY;
#endif // #ifdef CRYPT_NET_MSG
	// EDIT : BS : BEGIN
	m_tickSendMoveList = 0;
	// EDIT : BS : END

	TRACKMEM(Mem,"Network");
	ga_aplsPlayers.New(NET_MAXLOCALPLAYERS);
	
	// default demo syncronization is real-time, with 1:1 playback speed
	ga_fDemoSyncRate = DEMOSYNC_REALTIME;
	ga_fDemoRealTimeFactor = 1.0f;
	ga_fGameRealTimeFactor = 1.0f;
	ga_pubDefaultState = NULL;
	ga_slDefaultStateSize = 0;
	memset(ga_aubDefaultProperties, 0, sizeof(ga_aubDefaultProperties));
	ga_pubCRCList = NULL;
	ga_slCRCList = 0;
	
	ga_csNetwork.cs_iIndex = 2000;
	ga_fEnumerationProgress = 0;
	ga_bEnumerationChange = FALSE;
	ga_aulBandwidthGraph.Clear();
	ga_aulBandwidthGraph.New(net_iGraphBuffer);
	ga_ubNumLevelChanges = 0;
	
	memset(ga_aubProperties, 0, NET_MAXSESSIONPROPERTIES);
	//0105
	//IsTcpServer = FALSE;		// by seo-40225
	m_bReadyStart = FALSE;
	wo_aJewelData.Clear();
	wo_iNumOfJewelGradeData = -1;
	wo_iNumOfSkill = -1;			// yjpark
	m_bSingleMode = FALSE;
	
	wo_dwEnemyCount			= 0;
	wo_dwKilledEnemyCount	= 0;
	// set check point variable
	wo_stCheckPoint.m_iCheckFlag = 0;
	wo_stCheckPoint.m_iMaxCheckFlag = 0;
	wo_stCheckPoint.m_iQuestIndex	= 0;

	MyCharacterInfo.StatPoint = 0;	
	MyCharacterInfo.sbItemEffectOption = 0;
	
	// 케릭터와 연관된 Pet 관련 데이터 초기화
	MyCharacterInfo.bPetRide = FALSE; 
	MyCharacterInfo.iPetType = -1;
	MyCharacterInfo.iPetAge = -1;
	MyCharacterInfo.bWildPetRide = FALSE;
	MyCharacterInfo.secretkey = 0;
	MyCharacterInfo.pktitle = 0;

	// WSS_VISIBLE 070511
	MyCharacterInfo.m_ModelColor = NULL;
	MyCharacterInfo.ubGuildNameColor =0;	//[071123: Su-won] DRATAN_SIEGE_DUNGEON
	// WSS_GUILDMASTER 070511
	MyCharacterInfo.sbGuildRank = 0;
	MyCharacterInfo.bConsensus = FALSE;	// WSS_DRATAN_SEIGEWARFARE 2007/08/01	
	MyCharacterInfo.sbSoulCount = 0;
	MyCharacterInfo.EntranceType = CURRENT_ENTER_NORMAL;

	MyCharacterInfo.eMorphStatus	= MyChaInfo::eMORPH_END;

	m_uiSendedTime = 0;
	m_bSendMessage = FALSE;

	m_iServerGroup = -1;
	m_iServerCh	= -1;
	m_iServerType = -1;
	
	m_bIsPvp = FALSE;

	int		i;
	for( i = 0; i < WEAR_TOTAL; ++i )
	{	
		MyWearItem[i].Init();
	}

	for( i = 0; i < WEAR_COSTUME_TOTAL; ++i )
	{
		MyWearCostItem[i].Init();
	}

	ga_bGuildWar = FALSE;
	
	// [7/15/2010 kiny8216] MONSTER_ENERGY_IGNITION_SYSTEM
	MyCharacterInfo.ep		= 0;
	MyCharacterInfo.maxEP	= 0;

	MyCharacterInfo.iGP = 0;
	MyCharacterInfo.iStudentGiveUpCnt = 0;

	// 한벌의상 added by sam 110131
	MyCharacterInfo.bOneSuit = FALSE;
	MyCharacterInfo.iOneSuitDBIndex = -1;
	MyCharacterInfo.iOneSuitUniIndex = -1;
	MyCharacterInfo.bpkSysRewardLate = -1;//한번도 받지 않았으면 -1
	MyCharacterInfo.iSyndicateType = 0;	// 무소속으로 초기화.
	MyCharacterInfo.iSyndicateGrade = 0;
	MyCharacterInfo.lSyndicateAccPoint = 0;
	MyCharacterInfo.iHitEffectType = 0;
	MyCharacterInfo.useTotem = false;

	memset(m_iNetworkResponse, 0, sizeof(int) * MSG_END);
	memset(m_iNetworkResponseEx, 0, sizeof(int) * MSG_EX_END);

	// 서버타임 초기화
	slServerTimeGap = 0;
	slServerTime = 0;
		
	bMoveCharacterSelectUI = FALSE;

	// rvr존 진입 여부.
	m_bIsRvr = FALSE;
}

/*
* Destructor.
*/
CNetworkLibrary::~CNetworkLibrary(void)
{
	wo_aMakeItemData.Clear();
	wo_aLacaretteData.Clear();
	
	wo_aJewelData.Clear();
	wo_iNumOfJewelGradeData = -1;
	wo_iNumOfSkill = -1;			// yjpark
	// clear the global world
	ga_World.Clear();
	
	wo_dwEnemyCount			= 0;
	wo_dwKilledEnemyCount	= 0;
	// set check point variable
	wo_stCheckPoint.m_iCheckFlag = 0;
	wo_stCheckPoint.m_iMaxCheckFlag = 0;
	wo_stCheckPoint.m_iQuestIndex	= 0;
	delete &ga_sesSessionState;
	delete &ga_srvServer;
}

/*
* Initialize game management.
*/
void CNetworkLibrary::Init(const CTString &strGameID)
{
	TRACKMEM(Mem,"Network");
	// remember the game ID
	CMessageDispatcher::Init(strGameID);
	
	// add shell symbols
	_pShell->DeclareSymbol("user INDEX dbg_bBreak;", &dbg_bBreak);
	_pShell->DeclareSymbol("persistent user INDEX gam_bPretouch;", &gam_bPretouch);
	
	_pShell->DeclareSymbol("user INDEX dem_iRecordedNumber;",     &dem_iRecordedNumber);
	_pShell->DeclareSymbol("user void StartDemoRecording(void);", &StartDemoRecording);
	_pShell->DeclareSymbol("user void StopDemoRecording(void);",  &StopDemoRecording);
	_pShell->DeclareSymbol("user void NetworkInfo(void);",  &NetworkInfo);
	_pShell->DeclareSymbol("user void ReinitializeLevel(CTString);",&ReinitializeLevel);
	_pShell->DeclareSymbol("user void DumpClassReferences(void);",  &DumpClassReferences);  
	_pShell->DeclareSymbol("user void DumpEntityHashEfficiency(void);",  &DumpEntityHashEfficiency);
	_pShell->DeclareSymbol("user void DumpWorldEntityHashEfficiency(void);",  &DumpWorldEntityHashEfficiency);  
	_pShell->DeclareSymbol("user void DumpEntity(INDEX);",  &DumpEntity);
	_pShell->DeclareSymbol("user void DumpEntityInfo(INDEX,INDEX);",  &DumpEntityInfo);
	_pShell->DeclareSymbol("user void DumpPlayer(INDEX);",  &DumpPlayer);  
	_pShell->DeclareSymbol("user void DumpAllPlayers(INDEX);",  &DumpAllPlayers);  
	_pShell->DeclareSymbol("user void DumpServerStats();",  &DumpServerStats);
	_pShell->DeclareSymbol("user void StockInfo(void);",    &StockInfo);
	_pShell->DeclareSymbol("user void StockDump(void);",    &StockDump);
	_pShell->DeclareSymbol("user void DumpSkaStringToIDTable(void);", &DumpSkaStringToIDTable);
	_pShell->DeclareSymbol("user void DumpShellHashTable(void);", &DumpShellHashTable);
	_pShell->DeclareSymbol("user void RendererInfo(void);", &RendererInfo);
	_pShell->DeclareSymbol("user void ClearRenderer(void);",   &ClearRenderer);
	_pShell->DeclareSymbol("user void CacheShadows(void);",    &CacheShadows);
	_pShell->DeclareSymbol("user void KickClient(INDEX, CTString);", &KickClient);
	_pShell->DeclareSymbol("user void KickByName(CTString, CTString);", &KickByName);
	_pShell->DeclareSymbol("user void ListPlayers(void);", &ListPlayers);
	_pShell->DeclareSymbol("user void Admin(CTString);", &Admin);	
	_pShell->DeclareSymbol("user void AddIPMask(CTString);", &AddIPMask);
	_pShell->DeclareSymbol("user void RemIPMask(CTString);", &RemIPMask);
	_pShell->DeclareSymbol("user void AddNameMask(CTString);", &AddNameMask);
	_pShell->DeclareSymbol("user void RemNameMask(CTString);", &RemNameMask);	
	_pShell->DeclareSymbol("user FLOAT dem_tmTimer;",         &ga_fDemoTimer);
	_pShell->DeclareSymbol("user FLOAT dem_fSyncRate;",       &ga_fDemoSyncRate);
	_pShell->DeclareSymbol("user FLOAT dem_fRealTimeFactor;", &ga_fDemoRealTimeFactor);
	_pShell->DeclareSymbol("user FLOAT gam_fRealTimeFactor;", &ga_fGameRealTimeFactor);
	_pShell->DeclareSymbol("user INDEX dem_iAnimFrame;", &dem_iAnimFrame);
	_pShell->DeclareSymbol("user INDEX dem_iMotionBlurInterFrames;", &dem_iMotionBlurInterFrames);
	
	_pShell->DeclareSymbol("user const FLOAT cmd_tmTick;", &cmd_tmTick);
	_pShell->DeclareSymbol("persistent user CTString cmd_cmdOnTick;", &cmd_cmdOnTick);
	_pShell->DeclareSymbol("user CTString cmd_strChatSender ;", &cmd_strChatSender );
	_pShell->DeclareSymbol("user CTString cmd_strChatMessage;", &cmd_strChatMessage);
	_pShell->DeclareSymbol("persistent user CTString cmd_cmdOnChat;", &cmd_cmdOnChat);
	
	_pShell->DeclareSymbol("user INDEX net_ctChatMessages;", &net_ctChatMessages);
	
	_pShell->DeclareSymbol("persistent user INDEX ent_bReportSpawnInWall;", &ent_bReportSpawnInWall);
	
	_pShell->DeclareSymbol("user INDEX net_bSendEnumRequest;",   &_bSendEnumRequest);
	_pShell->DeclareSymbol("user INDEX net_bDumpStreamBlocks;",   &net_bDumpStreamBlocks);
	_pShell->DeclareSymbol("user INDEX net_bDumpConnectionInfo;", &net_bDumpConnectionInfo);
	_pShell->DeclareSymbol("user INDEX net_iPort;", &net_iPort);
	_pShell->DeclareSymbol("persistent user CTString net_strLocalHost;", &net_strLocalHost);
	_pShell->DeclareSymbol("persistent user CTString net_strLocationCode;", &net_strLocationCode);
	_pShell->DeclareSymbol("user CTString net_strVIPPassword;", &net_strVIPPassword);
	
	_pShell->DeclareSymbol("user CTString net_strObserverPassword;", &net_strObserverPassword);
	_pShell->DeclareSymbol("user INDEX net_iVIPReserve;", &net_iVIPReserve);
	_pShell->DeclareSymbol("user INDEX net_iMaxObservers;", &net_iMaxObservers);
	_pShell->DeclareSymbol("user INDEX net_iMaxClients;", &net_iMaxClients);
	_pShell->DeclareSymbol("user CTString net_strConnectPassword;", &net_strConnectPassword);
	_pShell->DeclareSymbol("user CTString net_strAdminPassword;", &net_strAdminPassword);
	_pShell->DeclareSymbol("user FLOAT net_tmConnectionTimeout;", &net_tmConnectionTimeout);
	_pShell->DeclareSymbol("user FLOAT net_tmProblemsTimeout;", &net_tmProblemsTimeout);
	_pShell->DeclareSymbol("user FLOAT net_tmDisconnectTimeout;", &net_tmDisconnectTimeout);
	_pShell->DeclareSymbol("user INDEX net_bReportCRC;", &net_bReportCRC);
	_pShell->DeclareSymbol("user INDEX ser_iRememberBehind;", &ser_iRememberBehind);
	_pShell->DeclareSymbol("persistent user INDEX net_bLookupHostNames;",    &net_bLookupHostNames);
	_pShell->DeclareSymbol("persistent user INDEX net_iCompression ;",       &net_iCompression);
	_pShell->DeclareSymbol("persistent user INDEX net_bReportPackets;", &net_bReportPackets);
	_pShell->DeclareSymbol("INDEX net_bReportServerTraffic;", &net_bReportServerTraffic);
	_pShell->DeclareSymbol("INDEX net_bReportClientTraffic;", &net_bReportClientTraffic);
	_pShell->DeclareSymbol("INDEX net_bReportDemoTraffic;", &net_bReportDemoTraffic);
	_pShell->DeclareSymbol("INDEX net_iDemoQuality;", &net_iDemoQuality);
	_pShell->DeclareSymbol("INDEX net_bLocalPrediction;", &net_bLocalPrediction); 
	_pShell->DeclareSymbol("persistent user INDEX net_iMaxSendRetries;", &net_iMaxSendRetries);
	_pShell->DeclareSymbol("persistent user FLOAT net_fSendRetryWait;", &net_fSendRetryWait);
	_pShell->DeclareSymbol("persistent user INDEX net_bReportTraffic;", &net_bReportTraffic);
	_pShell->DeclareSymbol("persistent user INDEX net_bReportICMPErrors;", &net_bReportICMPErrors);
	_pShell->DeclareSymbol("persistent user INDEX net_bReportMiscErrors;", &net_bReportMiscErrors);
	_pShell->DeclareSymbol("user INDEX net_bLerping;",       &net_bLerping);
	_pShell->DeclareSymbol("persistent user INDEX ser_bClientsMayPause;", &ser_bClientsMayPause);
	_pShell->DeclareSymbol("persistent user INDEX ser_bEnumeration;",      &ser_bEnumeration);
//#if _GAMESPY
//	_pShell->DeclareSymbol("persistent user INDEX ser_bHeartbeatGameSpy;", &ser_bHeartbeatGameSpy);
//#endif
	
	_pShell->DeclareSymbol("user INDEX net_bNetworkGame;", &net_bNetworkGame);  
	
	
	
	_pShell->DeclareSymbol("persistent user FLOAT ser_tmKeepAlive;", &ser_tmKeepAlive);
	_pShell->DeclareSymbol("persistent user FLOAT ser_tmPingUpdate;", &ser_tmPingUpdate);
	_pShell->DeclareSymbol("persistent user INDEX ser_bWaitFirstPlayer;", &ser_bWaitFirstPlayer);
	_pShell->DeclareSymbol("persistent user INDEX ser_iMaxAllowedBPS;", &ser_iMaxAllowedBPS);
	_pShell->DeclareSymbol("persistent user CTString ser_strIPMask;", &ser_strIPMask);
	_pShell->DeclareSymbol("persistent user CTString ser_strNameMask;", &ser_strNameMask);
	_pShell->DeclareSymbol("persistent user INDEX ser_bInverseBanning;", &ser_bInverseBanning);
	_pShell->DeclareSymbol("persistent user CTString ser_strMOTD;", &ser_strMOTD);
	_pShell->DeclareSymbol("user FLOAT ser_fAckDropRate;", &ser_fAckDropRate);
	_pShell->DeclareSymbol("persistent user FLOAT ser_fPositionTreshold;", &ser_fPositionTreshold);
	_pShell->DeclareSymbol("persistent user FLOAT ser_fOrientationTreshold;", &ser_fOrientationTreshold);
	_pShell->DeclareSymbol("user INDEX ser_iSkipTicks;", &ser_iSkipTicks);
	_pShell->DeclareSymbol("persistent user INDEX ser_iMaxBufferSize;", &ser_iMaxBufferSize);
	_pShell->DeclareSymbol("     const user INDEX ser_iMaxPendingData;", &ser_iMaxPendingData);
	_pShell->DeclareSymbol("     const user INDEX ser_iMaxTotalData;", &ser_iMaxTotalData);
	
	_pShell->DeclareSymbol("INDEX net_iHashCompartments;", &net_iHashCompartments);
	_pShell->DeclareSymbol("INDEX net_iHashSlotsPerComp;", &net_iHashSlotsPerComp);
	_pShell->DeclareSymbol("INDEX net_iHashStep;", &net_iHashStep);
	
	_pShell->DeclareSymbol("persistent user INDEX cli_iSendBehind;", &cli_iSendBehind);
	_pShell->DeclareSymbol("persistent user INDEX cli_iMaxBPS;",     &cli_iMaxBPS);
	_pShell->DeclareSymbol("persistent user INDEX cli_iMinBPS;",     &cli_iMinBPS);
	_pShell->DeclareSymbol("persistent user FLOAT cli_fTickDropRate;", &cli_fTickDropRate);
	_pShell->DeclareSymbol("persistent user INDEX cli_iFPSLimit;", &cli_iFPSLimit);
	
	_pShell->DeclareSymbol("user FLOAT net_fLimitLatencySend;",   &_pbsSend.pbs_fLatencyLimit);
	_pShell->DeclareSymbol("user FLOAT net_fLimitLatencyRecv;",   &_pbsRecv.pbs_fLatencyLimit);
	_pShell->DeclareSymbol("user FLOAT net_fLatencyVariationSend;", &_pbsSend.pbs_fLatencyVariation);
	_pShell->DeclareSymbol("user FLOAT net_fLatencyVariationRecv;", &_pbsRecv.pbs_fLatencyVariation);
	_pShell->DeclareSymbol("user FLOAT net_fLimitBandwidthSend;", &_pbsSend.pbs_fBandwidthLimit);
	_pShell->DeclareSymbol("user FLOAT net_fLimitBandwidthRecv;", &_pbsRecv.pbs_fBandwidthLimit);
	_pShell->DeclareSymbol("user FLOAT net_fDropPackets;", &net_fDropPackets);
	_pShell->DeclareSymbol("persistent user INDEX net_iGraphBuffer;", &net_iGraphBuffer);
	_pShell->DeclareSymbol("user const INDEX precache_NONE;",     &_precache_NONE);
	_pShell->DeclareSymbol("user const INDEX precache_SMART;",    &_precache_SMART);
	_pShell->DeclareSymbol("user const INDEX precache_ALL;",      &_precache_ALL);
	_pShell->DeclareSymbol("user const INDEX precache_PARANOIA;", &_precache_PARANOIA);
	_pShell->DeclareSymbol("persistent user INDEX gam_iPrecachePolicy;", &gam_iPrecachePolicy);
	
	_pShell->DeclareSymbol("user FLOAT phy_fCollisionCacheAhead;",  &phy_fCollisionCacheAhead);
	_pShell->DeclareSymbol("user FLOAT phy_fCollisionCacheAround;", &phy_fCollisionCacheAround);
	
	_pShell->DeclareSymbol("user INDEX phy_bNonZoningClipMove;", &phy_bNonZoningClipMove);
	_pShell->DeclareSymbol("user INDEX phy_bNonZoningClipRay ;", &phy_bNonZoningClipRay );
	
	_pShell->DeclareSymbol("INDEX pwoCurrentWorld;", &_pwoCurrentWorld);

	// WSS_DRATAN_SEIGEWARFARE 2007/08/14 -------------------------------->>
	// 390 ~ 399 공성 부활진지 초기화
	for(int i=0;i<10;i++)
	{	
		_pNetwork->MyCharacterInfo.mQuarter[390+i] = -1;
		_pNetwork->MyCharacterInfo.mQuarterName[390+i] = CTString("");
	}
}





/*
* Start a peer-to-peer game session.
*
* remember to keep this routine up to date with CNetworkLibrary::Read()
*/
void CNetworkLibrary::StartPeerToPeer_t(const CTString &strSessionName,
										const CTFileName &fnmWorld, ULONG ulSpawnFlags, 
										INDEX ctMaxPlayers, BOOL bWaitAllPlayers,
										void *pvSessionProperties) // throw char *
{
	TRACKMEM(Mem,"Network");
	// remove all pending sounds
	_pSound->Flush();
	CallProgressHook_t(0.f);
	
	// go on
#ifdef _DEBUG
	CPrintF( TRANS("Starting session: '%s'\n"), strSessionName);
	CPrintF( TRANS("  level: '%s'\n"), (const char*) fnmWorld);
	CPrintF( TRANS("  spawnflags: %08x\n"), ulSpawnFlags);
	CPrintF( TRANS("  max players: %d\n"), ctMaxPlayers);
	CPrintF( TRANS("  waiting: %d\n"), bWaitAllPlayers);
#endif
	
	CGatherCRC gc;
	
	// if starting in network
	if (_cmiComm.IsNetworkEnabled()) {
		CPrintF( TRANS("  network is on\n"));
		// start gathering CRCs
		FreeUnusedStock();
		InitCRCGather();
		
	} else {
		CPrintF( TRANS("  network is off\n"));
	}
	
	BOOL bLock = TRUE;

	if (_pNetwork->bMoveCharacterSelectUI == TRUE)
		bLock = FALSE;
	// access to the list of handlers must be locked
	CTSingleLock slHooks(&_pTimer->tm_csHooks, bLock);
	// synchronize access to network
	CTSingleLock slNetwork(&ga_csNetwork, bLock);
	
	ga_strSessionName = strSessionName;
	ga_bLocalPause = FALSE;
	
	ga_ubNumLevelChanges++;
	ga_sesSessionState.ses_ubNumLevelChanges = ga_ubNumLevelChanges;
	
	ga_sesSessionState.ses_ulSpawnFlags = ulSpawnFlags;
	
	memcpy(ga_aubProperties, pvSessionProperties, sizeof(CSessionProperties));
	ga_IsServer = TRUE;
	
	// remember the world filename
	ga_fnmWorld = fnmWorld;
	ga_fnmNextLevel = CTString("");
	try {
		// load the world
		_pTimer->SetCurrentTick(0.0f);  // must have timer at 0 while loading
//		ga_World.Load_t(fnmWorld);
		
		//wooss 050822
		extern BOOL _bWorldEditorApp;
		ga_World.Load_t(fnmWorld);
		
		// delete all entities that don't fit given spawn flags
		ga_World.FilterEntitiesBySpawnFlags(ga_sesSessionState.ses_ulSpawnFlags);
	} catch(char *) {
		ga_IsServer = FALSE;
		ga_fnmWorld = CTString("");
		_cmiComm.Server_Close();
		_cmiComm.Client_Close();
		//StopProgress();
		CLoadingImage::getSingleton()->StopLoading();
		throw;
	}
	// remember the world pointer
	_pShell->SetINDEX("pwoCurrentWorld", (INDEX)&ga_World);
	
	CallProgressHook_t(0.0f);
	if (_pNetwork->bMoveCharacterSelectUI == FALSE)
	{
		// initialize server
		try {
			//! 서버 초기화.
			ga_srvServer.Start_t();
		} catch (char *) {
			ga_World.Clear();
			//StopProgress();
			CLoadingImage::getSingleton()->StopLoading();
			throw;
		}
	}
	ga_IsServer = TRUE;
	CallProgressHook_t(1.0f);
	
	CallProgressHook_t(0.0f);
	if (_pNetwork->bMoveCharacterSelectUI == FALSE)
	{
		// initialize session state
		try {
			//! 세션 스테이트 초기화 및 서버 스타트.
			ga_sesSessionState.Start_t(-1);
		} catch (char *strError) {
			(void)strError;
			ga_srvServer.Stop();
			ga_World.Clear();
			//StopProgress();
			CLoadingImage::getSingleton()->StopLoading();
			throw;
		}
	}
	CallProgressHook_t(1.0f);
	
	// remember maximum number of players
	ga_sesSessionState.ses_ctMaxPlayers = ctMaxPlayers;
	ga_sesSessionState.ses_bWaitAllPlayers = bWaitAllPlayers;
	
	// time speed is normal by default
	ga_sesSessionState.ses_fRealTimeFactor = 1.0f;
	
	// eventually cache all shadowmaps in world (memory eater!)
	if( shd_bCacheAll) ga_World.wo_baBrushes.CacheAllShadowmaps();
	// flush stale caches
	FreeUnusedStock();
	// mark that pretouching is required
	_bNeedPretouch = TRUE;
	
	FinishCRCGather();
	CPrintF( TRANS("  started.\n"));
	//StopProgress();
	CLoadingImage::getSingleton()->StopLoading();
	// mute sounds for one second
	_pSound->Mute(1);
}

/*
 * Save the game.
 */
void CNetworkLibrary::Save_t(const CTFileName &fnmGame) // throw char *
{
  // synchronize access to network
  CTSingleLock slNetwork(&ga_csNetwork, TRUE);

  // mute all sounds
  _pSound->Mute(1);

  // must be server
  if (!ga_IsServer) 
  {
    if (IsPlayingDemo()) 
	{
      throw "";
    } 
	else 
	{
      throw TRANS("Cannot save game - not a server!\n");
    }
  }
  
#if _DEBUG
  CTimerValue tvLoadStart = _pTimer->GetHighPrecisionTimer();
#endif
  extern INDEX gam_iMaxSaveGameSize;

  // create the file
  CTFileStream strmFile;
  strmFile.Create_t(fnmGame);

  if(gam_iMaxSaveGameSize>0) {
    strmFile.SetFileSize_t(gam_iMaxSaveGameSize);
  }

  // write game to stream
  strmFile.WriteID_t("GAME");
  ga_sesSessionState.Write_t(&strmFile);
  strmFile.WriteID_t("GEND");   // game end
  

  
  

#if _DEBUG
  CTimerValue tvLoadEnd = _pTimer->GetHighPrecisionTimer();
  FLOAT fLoadTime = (tvLoadEnd-tvLoadStart).GetSeconds();
  SLONG slGameSize = strmFile.GetPos_t();
  CPrintF("Time to save game %.2f, save game size is %.2f KB\n",fLoadTime,slGameSize/1024.0f);
#endif

  // Close file
  strmFile.Close();
}

/*
 * Load the game.
 *
 * remember to keep this routine up to date with CNetworkLibrary::StartPeerToPeer()
 */
void CNetworkLibrary::Load_t(const CTFileName &fnmGame) // throw char *
{
  // remove all pending sounds
  _pSound->Flush();
  

  CallProgressHook_t(0.f);

  // access to the list of handlers must be locked
  CTSingleLock slHooks(&_pTimer->tm_csHooks, TRUE);
  // synchronize access to network
  CTSingleLock slNetwork(&ga_csNetwork, TRUE);
  CGatherCRC gc;


  ga_bLocalPause = FALSE;

  // open the file
  CTFileStream strmFile;
  strmFile.Open_t(fnmGame);

  // if starting in network
  if (_cmiComm.IsNetworkEnabled()) {
    // start gathering CRCs
    InitCRCGather();
  }

  // initialize server
  ga_srvServer.Start_t();
  ga_IsServer = TRUE;
  ga_fnmNextLevel = CTString("");
  memset(ga_aubProperties, 0, NET_MAXSESSIONPROPERTIES);

  strmFile.ExpectID_t("GAME");
  // read session state
  try {
    ga_sesSessionState.Start_t(-1);
    ga_sesSessionState.Read_t(&strmFile);
    strmFile.ExpectID_t("GEND");   // game end
  } catch(char *) {
    //StopProgress();
	CLoadingImage::getSingleton()->StopLoading();
    ga_srvServer.Stop();
    ga_IsServer = FALSE;
    throw;
  }
  
  // remember the world pointer
  _pShell->SetINDEX("pwoCurrentWorld", (INDEX)&ga_World);

  // set time and pause for server from the saved game
  ga_ubNumLevelChanges++;
  ga_sesSessionState.ses_ubNumLevelChanges = ga_ubNumLevelChanges;

  ga_srvServer.srv_tmLastProcessedTick = ga_sesSessionState.ses_tmLastProcessedTick;
  ga_srvServer.srv_iLastProcessedSequence = ga_sesSessionState.ses_iLastProcessedSequence;
  ga_srvServer.srv_bPause = ga_sesSessionState.ses_bPause;
  ga_srvServer.srv_bGameFinished = ga_sesSessionState.ses_bGameFinished;
  // start sending stream to local state
  ga_srvServer.srv_assoSessions[0].sso_bSendStream = TRUE;
  ga_srvServer.srv_assoSessions[0].sso_iLastSentSequence = ga_srvServer.srv_iLastProcessedSequence;

  // if the names match, check if it's already active
  for (INDEX iTarget=0;iTarget<NET_MAXGAMEPLAYERS;iTarget++) {
    if (ga_srvServer.srv_apltPlayers[iTarget].IsActive()) {
      // and if it is, skip it (someone is already playing with it)
      ga_srvServer.srv_apltPlayers[iTarget].Deactivate();
    }
  }

  // eventually cache all shadowmaps in world (memory eater!)
  if( shd_bCacheAll) ga_World.wo_baBrushes.CacheAllShadowmaps();
  // flush stale caches
  FreeUnusedStock();
  // mark that pretouching is required
  _bNeedPretouch = TRUE;

  FinishCRCGather();
  //StopProgress();
  CLoadingImage::getSingleton()->StopLoading();
}

/*
 * Save a debugging game.
 */
void CNetworkLibrary::DebugSave(void)
{
  // try to save game
  try {
    Save_t(CTString("Save\\Debug.sav"));
  // if not successful
  } catch (char *strError){
    FatalError("Cannot save debug game:\n%s", strError);
  }
}


/*
 * Join a running multi-player game.
 */
void CNetworkLibrary::JoinSession_t(const CNetworkSession &nsSesssion, INDEX ctLocalPlayers) // throw char *
{
  TRACKMEM(Mem,"Network");
  // remove all pending sounds
  _pSound->Flush();
  CallProgressHook_t(0.f);

  // report session addres
  CPrintF( TRANS("Joining session at: '%s'\n"), nsSesssion.ns_strAddress);

  ga_bLocalPause = FALSE;

  // access to the list of handlers must be locked
  CTSingleLock slHooks(&_pTimer->tm_csHooks, TRUE);
  // synchronize access to network
  CTSingleLock slNetwork(&ga_csNetwork, TRUE);
  // start gathering CRCs
  CGatherCRC gc;
  InitCRCGather();



  // set session name and server address
  ga_strSessionName = nsSesssion.ns_strSession;
  ga_strServerAddress = nsSesssion.ns_strAddress;
  ga_fnmNextLevel = CTString("");
  ga_fnmWorld = CTString("");
  memset(ga_aubProperties, 0, NET_MAXSESSIONPROPERTIES);

  ga_IsServer = FALSE;

  CallProgressHook_t(0.0f);
  // initialize session state
  try {
    ga_sesSessionState.Start_t(ctLocalPlayers);
  } catch(char *) {
    ga_World.Clear();
    FreeUnusedStock();
    //StopProgress();
	CLoadingImage::getSingleton()->StopLoading();
    throw;
  }

  // remember the world pointer
  _pShell->SetINDEX("pwoCurrentWorld", (INDEX)&ga_World);

  // eventually cache all shadowmaps in world (memory eater!)
  if( shd_bCacheAll) ga_World.wo_baBrushes.CacheAllShadowmaps();
  // flush stale caches
  FreeUnusedStock();
  // mark that pretouching is required
  _bNeedPretouch = TRUE;

  // run main loop to let session state process messages from server
  MainLoop();

  CPrintF("  joined\n");
  //StopProgress();
  CLoadingImage::getSingleton()->StopLoading();
}

/* Start playing a demo. */
/*
void CNetworkLibrary::StartDemoPlay_t(const CTFileName &fnDemo)  // throw char *
{
  // remove all pending sounds
  _pSound->Flush();
  StartProgress();


  // access to the list of handlers must be locked
  CTSingleLock slHooks(&_pTimer->tm_csHooks, TRUE);
  // synchronize access to network
  CTSingleLock slNetwork(&ga_csNetwork, TRUE);
  ga_bLocalPause = FALSE;

  // open the file
  ga_strmDemoPlay.Open_t(fnDemo);

  // remember that playing demo
  ga_bDemoPlay = TRUE;
  ga_bDemoPlayFinished = FALSE;

  // create session name from demo name
  CTString strSessionName = CTString("Demo: ")+fnDemo;
  ga_strSessionName = strSessionName;

  ga_IsServer = FALSE;
  // initialize server
  try {
    // read initial info from stream
    ga_strmDemoPlay.ExpectID_t("DEMO");

    // write world filename and spawn flags
    ga_strmDemoPlay>>ga_fnmWorld;
    ga_strmDemoPlay>>ga_sesSessionState.ses_ulSpawnFlags;
    // write sessin properties
    ga_strmDemoPlay.Read_t(_pNetwork->ga_aubProperties, NET_MAXSESSIONPROPERTIES);

    ga_World.Load_t(ga_fnmWorld);
    // delete all entities that don't fit given spawn flags
    ga_World.FilterEntitiesBySpawnFlags(ga_sesSessionState.ses_ulSpawnFlags);

    // set overdue timers in just loaded world to be due in current time
    ga_World.AdjustLateTimers(ga_sesSessionState.ses_tmLastProcessedTick);
    
    _pNetwork->ga_World.wo_ulNextEntityID = 0x10000001;

    ga_sesSessionState.Read_t(&ga_strmDemoPlay,TRUE);

    _pNetwork->ga_strmDemoPlay.ExpectID_t("DTTM");
    ga_strmDemoPlay>>ga_tmNextDemoTick;

    extern TIME _tmLocalTick;
    _tmLocalTick = ga_tmNextDemoTick;
    ga_fDemoTimer = ga_tmNextDemoTick;
    ga_sesSessionState.ses_bRestartLocalTime = TRUE;
  } catch(char *) {
    ga_strmDemoPlay.Close();
    ga_bDemoPlay = FALSE;
    StopProgress();
    throw;
  }

  // eventually cache all shadowmaps in world (memory eater!)
  if( shd_bCacheAll) ga_World.wo_baBrushes.CacheAllShadowmaps();
  // flush stale caches
  FreeUnusedStock();
  // mark that pretouching is required
  _bNeedPretouch = TRUE;

  // remember the world pointer
  _pShell->SetINDEX("pwoCurrentWorld", (INDEX)&ga_World);

  // demo synchronization starts at the beginning initially
  ga_tvDemoTimerLastTime = _pTimer->GetHighPrecisionTimer();

  // run main loop to let server process messages from host
  MainLoop();
  StopProgress();
}
	*/

/*
BOOL CNetworkLibrary::IsDemoPlayFinished(void)
{
  return ga_bDemoPlay && ga_bDemoPlayFinished;
}
*/
BOOL CNetworkLibrary::IsPlayingDemo(void)
{
  return ga_bDemoPlay;
}

/*
void CNetworkLibrary::StartDemoRec_t(const CTFileName &fnDemo) // throw char *
{
  // synchronize access to network
  CTSingleLock slNetwork(&ga_csNetwork, TRUE);

  // if already recording
  if (ga_bDemoRec) {
    // error
    throw TRANS("Already recording a demo!");
  }

  // create the file
  ga_strmDemoRec.Create_t(fnDemo);

  // write initial info to stream
  ga_strmDemoRec.WriteID_t("DEMO");

  // write world filename and spawn flags
  ga_strmDemoRec<<ga_fnmWorld;
  ga_strmDemoRec<<ga_sesSessionState.ses_ulSpawnFlags;
  // write sessin properties
  ga_strmDemoRec.Write_t(_pNetwork->ga_aubProperties, NET_MAXSESSIONPROPERTIES);
  // write gamestate
  ga_sesSessionState.Write_t(&ga_strmDemoRec,TRUE);

  // remember that recording demo
  ga_bDemoRec = TRUE;
}

void CNetworkLibrary::StopDemoRec(void)
{
  // synchronize access to network
  CTSingleLock slNetwork(&ga_csNetwork, TRUE);

  // if not recording
  if (!ga_bDemoRec) {
    // do nothing
    return;
  }
  // write terminal info to the stream
  ga_strmDemoRec.WriteID_t("DEND");   // game end
  // close the file
  ga_strmDemoRec.Close();
  // remember that not recording demo
  ga_bDemoRec = FALSE;
}


BOOL CNetworkLibrary::IsRecordingDemo(void)
{
  return ga_bDemoRec;
}
*/
BOOL CNetworkLibrary::IsNetworkEnabled(void)
{ 
  return _cmiComm.IsNetworkEnabled();
}




// pause/unpause game
void CNetworkLibrary::TogglePause(void)
{
  ga_sesSessionState.ses_bWantPause = !ga_sesSessionState.ses_bWantPause;
}

// test if game is paused
BOOL CNetworkLibrary::IsPaused(void)
{
  if (this==NULL) {
    return TRUE; 
  }
  return ga_sesSessionState.ses_bWantPause;
}

// test if having connnection problems (not getting messages from server regulary)
BOOL CNetworkLibrary::IsConnectionStable(void)
{
  // if this is the server or network is not enabled
  if (IsServer() || !_cmiComm.IsNetworkEnabled() || ga_bDemoPlay) {
    // it is always stable
    return TRUE;
  }

  // check when last message was received.
  return (_pTimer->GetHighPrecisionTimer()-ga_sesSessionState.ses_tvMessageReceived).GetSeconds()<net_tmProblemsTimeout;
}
// test if completely disconnected and why
BOOL CNetworkLibrary::IsDisconnected(void)
{
  return ga_sesSessionState.ses_strDisconnected!="";
}

const CTString &CNetworkLibrary::WhyDisconnected(void)
{
  return ga_sesSessionState.ses_strDisconnected;
}

// set/get server side pause (for single player only)
void CNetworkLibrary::SetLocalPause(BOOL bPause)
{
  ga_bLocalPause = bPause;
}

BOOL CNetworkLibrary::GetLocalPause(void)
{
  if (this==NULL) {
    return TRUE;
  }
  return ga_bLocalPause;
}

// get server/client name and address
void CNetworkLibrary::GetHostName(CTString &strName, CTString &strAddress)
{
  _cmiComm.GetHostName(strName, strAddress);
}

// mark that the game has finished -- called from AI
void CNetworkLibrary::SetGameFinished(void)
{
  // remove all pending sounds
  _pSound->Flush();
  ga_tvGameFinished = _pTimer->GetHighPrecisionTimer();
  ga_sesSessionState.ses_bGameFinished = TRUE;
  if (IsServer()) {
    ga_srvServer.srv_bGameFinished = TRUE;

    INDEX iActiveSessions = ga_srvServer.ActiveRemoteSessions();
    if (iActiveSessions > 0) {
      ga_sesSessionState.ses_tmLastProcessedTick += _pTimer->TickQuantum;
      ga_srvServer.srv_tmLastProcessedTick += _pTimer->TickQuantum;
      _pNetwork->ga_srvServer.StartNewTick(ga_sesSessionState.ses_tmLastProcessedTick);
      _pNetwork->ga_srvServer.srv_embSendBuffer.RequestTickAcknowledge(ga_sesSessionState.ses_tmLastProcessedTick,iActiveSessions);
    }
  }
}

BOOL CNetworkLibrary::IsGameFinished(void)
{
  return ga_sesSessionState.ses_bGameFinished;
}

// manipulation with realtime factor for slower/faster time -- called from AI
void CNetworkLibrary::SetRealTimeFactor(FLOAT fSpeed)
{
  ga_sesSessionState.ses_fRealTimeFactor = fSpeed;
}

FLOAT CNetworkLibrary::GetRealTimeFactor(void)
{
  return ga_sesSessionState.ses_fRealTimeFactor;
}

// test if game is waiting for more players to connect
BOOL CNetworkLibrary::IsWaitingForPlayers(void)
{
  // if game mode does not include waiting for players
  if (!ga_sesSessionState.ses_bWaitAllPlayers) {
    // not waiting
    return FALSE;
  }
  // if server
  if (IsServer()) {
    // check number of players on server
    return ga_srvServer.GetPlayersCount()<ga_sesSessionState.ses_ctMaxPlayers;
  // if not server
  } else {
    // check number of players in session
    return ga_sesSessionState.GetPlayersCount()<ga_sesSessionState.ses_ctMaxPlayers;
  }
}
// test if game is waiting for server
BOOL CNetworkLibrary::IsWaitingForServer(void)
{
  return ga_sesSessionState.ses_bWaitingForServer;
}


/*
 * Stop currently running game.
 */
void CNetworkLibrary::StopGame(void)
{
	TRACKMEM(Mem,"Network");
	// remove all pending sounds and finish rendering
	_pSound->Flush();
	_pGfx->Flush();
	
	CPrintF( TRANS("stopping game.\n"));
	BOOL bLock = TRUE;

	if (_pNetwork->bMoveCharacterSelectUI == TRUE)
		bLock = FALSE;
	// access to the list of handlers must be locked
	CTSingleLock slHooks(&_pTimer->tm_csHooks, bLock);
	// synchronize access to network
	CTSingleLock slNetwork(&ga_csNetwork, bLock);
	
	/*
	// stop demo recording if active
	StopDemoRec();
	
	// if playing demo
	if (ga_bDemoPlay) {
		// close the demo file
		ga_strmDemoPlay.Close();
		// remember that not playing demo
		ga_bDemoPlay = FALSE;
		ga_bDemoPlayFinished = FALSE;
	}
	*/

	LeavePet( (CPlayerEntity*)CEntity::GetPlayerEntity(0) );
	ClearPetList();
	MY_PET_INFO()->Init();

	ACTORMGR()->RemoveAll();
	
	if (_pNetwork->bMoveCharacterSelectUI == FALSE)
	{
		// stop session
		ga_sesSessionState.Stop();

		// stop server
		if (ga_IsServer) {
			ga_srvServer.Stop();
			ga_IsServer = FALSE;
		}

		ga_strSessionName = "";
	}
	else
	{
		_pNetwork->ga_srvServer.srv_apltPlayers.Clear();
		_pNetwork->ga_srvServer.srv_apltPlayers.New(NET_MAXGAMEPLAYERS);

		INDEX iPlayer = 0;
		{FOREACHINSTATICARRAY(_pNetwork->ga_srvServer.srv_apltPlayers, CPlayerTarget, itplt) {
			itplt->plt_Index = iPlayer;
			iPlayer++;
		}}
	}	
	
	ga_World.Clear();	
	
	// free default state if existing
	if (ga_pubDefaultState!=NULL) {
		FreeMemory(ga_pubDefaultState);
		ga_pubDefaultState = NULL;
		ga_slDefaultStateSize = 0;
		memset(ga_aubDefaultProperties, 0, sizeof(ga_aubDefaultProperties));
	}
	if (ga_pubCRCList!=NULL) {
		FreeMemory(ga_pubCRCList);
		ga_pubCRCList = NULL;
		ga_slCRCList = 0;
	}
	
	{
		MEMTRACK_SETFLAGS(mem,MTF_NOSTACKTRACE);
		ga_aplsPlayers.Clear();
		ga_aplsPlayers.New(NET_MAXLOCALPLAYERS);
	}
	// remember the world pointer
	_pShell->SetINDEX("pwoCurrentWorld", (INDEX)NULL);
	
	ClearEntityClassContainer();
	
	FreeUnusedStock();
	
	// rewind the timer
	_pTimer->SetCurrentTick(0.0f);
}

// initiate level change
void CNetworkLibrary::ChangeLevel(
								  const CTFileName &fnmNextLevel, BOOL bRemember, INDEX iUserData)
{
	ShowCursor(FALSE);
	// synchronize access to network
	CTSingleLock slNetwork(&ga_csNetwork, TRUE);
	
	// if not currently changing
	if (_lphCurrent==LCP_NOCHANGE) 
	{
		// initiate change
		ga_fnmNextLevel = fnmNextLevel;
		ga_bNextRemember = bRemember;
		ga_iNextLevelUserData = iUserData;
		_lphCurrent = LCP_INITIATED;
	}
}

// really do the level change
void CNetworkLibrary::ChangeLevel_internal(void)
{
	_pNetwork->MyCharacterInfo.itemEffect.Clear();
	_pNetwork->MyCharacterInfo.eMorphStatus = MyChaInfo::eMORPH_END;

	_cmiComm.m_inbuf.ClearBuffer();
	//TODO : 다른 캐릭터들의 item effect도 처리해야함.
	ga_ubNumLevelChanges++;
	ga_sesSessionState.ses_ubNumLevelChanges = ga_ubNumLevelChanges;
	
	{for (INDEX iClient=0;iClient<NET_MAXGAMECOMPUTERS;++iClient) 
	{
		CSessionSocket &sso = ga_srvServer.srv_assoSessions[iClient];
		if (sso.IsActive() && sso.sso_bSendStream) {
			sso.sso_ubNumLevelChanges++;
		}
	}}
	
	// remove all pending sounds
	_pSound->Flush();
	
	TRACKMEM(Mem,"Network");
	BOOL bMultiplayer = FALSE;
	extern BOOL _bReinitEntitiesWhileCopying;
	_bReinitEntitiesWhileCopying = FALSE;
	
	if (ga_srvServer.ActiveRemoteSessions() > 0) {
		bMultiplayer = TRUE;
	}
	
	if (ga_srvServer.srv_bRunning) {
		ga_srvServer.StopNetProcess(); 
	}
	
	CallProgressHook_t(0.f);
	
	// find all entities that are to cross to next level
	CEntitySelection* psenToCross = new CEntitySelection;
	{FOREACHINDYNAMICCONTAINER(ga_World.wo_cenEntities, CEntity, iten) {
		if (iten->en_ulFlags&ENF_CROSSESLEVELS) {
			psenToCross->Select(*iten);
		}
	}}
	
	// copy them to a temporary world
	CWorld* pwldTemp = new CWorld;
	CEntitySelection* psenInTemp = new CEntitySelection;
	pwldTemp->CopyEntities(ga_World, *psenToCross,
		*psenInTemp, CPlacement3D(FLOAT3D(0,0,0), ANGLE3D(0,0,0)));
	
	
	// remember characters for all player targets and disable them
	CPlayerCharacter apc[NET_MAXGAMEPLAYERS];
	BOOL abWasActive[NET_MAXGAMEPLAYERS];
	INDEX aiClientIndex[NET_MAXGAMEPLAYERS];
	CPlayerAction apaActions[NET_MAXGAMEPLAYERS][2];
	{for(INDEX i=0; i<NET_MAXGAMEPLAYERS; i++) {
		CPlayerTarget &plt = ga_srvServer.srv_apltPlayers[i];
		abWasActive[i] = plt.IsActive();
		aiClientIndex[i] = plt.plt_iClient;
		if (plt.IsActive()) {
			apc[i] = plt.plt_penPlayerEntity->en_pcCharacter;
			apaActions[i][0] = plt.plt_paLastAction;
			apaActions[i][1] = plt.plt_paPreLastAction;
			plt.plt_penPlayerEntity = NULL;
			plt.Deactivate();
		}
	}}
	
	// destroy all entities that will cross level
	ga_World.DestroyEntities(*psenToCross);
	delete psenToCross;
	
	// if should remember old levels
	if (ga_bNextRemember) {
		// remember current level
		ga_sesSessionState.RememberCurrentLevel(ga_fnmWorld);
	}
	
	CGatherCRC gc;
	
	ACTORMGR()->RemoveAll();
	ga_World.Clear();
	
	
	// if starting in network
	//if (_cmiComm.IsNetworkEnabled()) {
    // start gathering CRCs
    FreeUnusedStock();
    InitCRCGather();
	//}
	
	/* //0522 kwon 삭제.
	if (bMultiplayer) {
    // create base info to be sent
    extern CTString ser_strMOTD;
    CNetworkMessage nmLevelChange(MSG_CHANGE_LEVEL);
    nmLevelChange<<ser_strMOTD;
    nmLevelChange<<ga_fnmNextLevel;
    nmLevelChange<<_pNetwork->ga_sesSessionState.ses_ulSpawnFlags;
    nmLevelChange.Write(_pNetwork->ga_aubProperties, NET_MAXSESSIONPROPERTIES);
	
	  
		
		  // notify active clients of level change
		  for( INDEX iClient=1; iClient<ga_srvServer.srv_assoSessions.Count(); iClient++) {
		  if (ga_srvServer.srv_assoSessions[iClient].IsActive()) {
		  CPrintF("Sent levelchange to client: %d\n",iClient);
		  _pNetwork->SendToClientReliable(iClient, nmLevelChange);
		  }
		  }
		  _cmiComm.Server_Update();
		  }
	*/
	// remember original world filename
	CTFileName fnmOldWorld = ga_fnmWorld;
	// try to
	try {
		// load the new world
		_pTimer->SetCurrentTick(0.0f);  // must have timer at 0 while loading	
		ga_World.Load_t(ga_fnmNextLevel);
		// delete all entities that don't fit given spawn flags
		ga_World.FilterEntitiesBySpawnFlags(ga_sesSessionState.ses_ulSpawnFlags);
		// if failed
	} catch(char *strError) {
		// report error
		CPrintF(TRANS("Cannot change level:\n%s"), strError);
		// try to
		try {
			// load the old world
			ga_fnmNextLevel = fnmOldWorld;
			ga_World.Load_t(ga_fnmNextLevel);
			// delete all entities that don't fit given spawn flags
			ga_World.FilterEntitiesBySpawnFlags(ga_sesSessionState.ses_ulSpawnFlags);
			// if that fails
		} catch (char *strError2) {
			//StopProgress();
			CLoadingImage::getSingleton()->StopLoading();
			// fatal error
			FatalError(
				TRANS("Cannot change level because:\n%s\n"
				"and cannot go back to original one because:\n%s"), strError, strError2);
			return;
		}
	}
	
	
	// remember the world filename
	ga_fnmWorld = ga_fnmNextLevel;
	// remember the world pointer
	_pShell->SetINDEX("pwoCurrentWorld", (INDEX)&ga_World);
	
	// set overdue timers in just loaded world to be due in current time
	ga_World.AdjustLateTimers(ga_sesSessionState.ses_tmLastProcessedTick);
	
	// copy entities from temporary world into new one
	CEntitySelection* psenCrossed = new CEntitySelection;
	ga_World.CopyEntities(*pwldTemp, *psenInTemp, 
		*psenCrossed, CPlacement3D(FLOAT3D(0,0,0), ANGLE3D(0,0,0)));
	delete psenInTemp;
	delete psenCrossed;
	delete pwldTemp;

	CallProgressHook_t(0.0f);
	// precache data needed by entities
	if( gam_iPrecachePolicy>=PRECACHE_SMART) {
		ga_World.PrecacheEntities_t();
	}
	CallProgressHook_t(1.0f);

	// restore pointers to entities for all active player targets
	{for(INDEX i=0; i<NET_MAXGAMEPLAYERS; ++i) {
		CPlayerTarget &plt = ga_srvServer.srv_apltPlayers[i];
		if (abWasActive[i]) {
			plt.Activate(aiClientIndex[i]);
			plt.plt_paLastAction    = apaActions[i][0];
			plt.plt_paPreLastAction = apaActions[i][1];
			plt.AttachEntity(ga_World.FindEntityWithCharacter(apc[i]));
			plt.plt_penPlayerEntity->SetWeaponAndAnimator();
		}
	}}
	
	_bReinitEntitiesWhileCopying = TRUE;
	
	if (bMultiplayer) {
		ga_srvServer.srv_embSendBuffer.Clear();
		ga_srvServer.InitServerNetProcess();
	}
	
	// for each player
	{for( INDEX iPlayer=0; iPlayer<NET_MAXGAMEPLAYERS; ++iPlayer) {
		CPlayerTarget &plt = _pNetwork->ga_srvServer.srv_apltPlayers[iPlayer];
		if (plt.plt_bActive) {
			// add one dummy action
			CPlayerAction pa;
			pa.Clear();
			pa.pa_aRotation = plt.plt_paLastAction.pa_aRotation;
			pa.pa_aViewRotation = plt.plt_paLastAction.pa_aViewRotation;
			plt.plt_abReceived.AddAction(pa);
		}
	}}
	
	// flush stale caches
	FreeUnusedStock();
	// mark that pretouching is required
	_bNeedPretouch = TRUE;
	
	CTerrainEffect::SetTerrain( NULL );		// yjpark
	
	FinishCRCGather();
//	Sleep(200);
	//StopProgress();
	CLoadingImage::getSingleton()->StopLoading();
	
	// mute sounds for one second
	_pSound->Mute(1);
}

// really do the level change
void CNetworkLibrary::ChangeLevel_client_internal_t(void)
{
  // remove all pending sounds
  _pSound->Flush();

  TRACKMEM(Mem,"Network");
  CallProgressHook_t(0.f);

  EPreLevelChange ePreChange;
  ePreChange.iUserData = _pNetwork->ga_iNextLevelUserData;
  ga_sesSessionState.SendLevelChangeNotification(ePreChange);
  CEntity::HandleSentEvents();

  CGatherCRC gc;
  BOOL bPaused = _pNetwork->GetLocalPause();
  _pNetwork->SetLocalPause(TRUE);
  // start gathering CRCs
  ga_World.Clear();
  FreeUnusedStock();
  InitCRCGather();

  // remember original world filename
  CTFileName fnmOldWorld = ga_fnmWorld;
  
  // disable all player targets
  {for(INDEX i=0; i<NET_MAXGAMEPLAYERS; ++i) {
    CPlayerTarget &plt = ga_srvServer.srv_apltPlayers[i];
    if (plt.IsActive()) {
      plt.plt_penPlayerEntity = NULL;
      plt.Deactivate();
    }
  }} 

  // load the new world
  _pTimer->SetCurrentTick(0.0f);  // must have timer at 0 while loading
  ga_World.Load_t(ga_fnmNextLevel);
  // delete all entities that don't fit given spawn flags
  ga_World.FilterEntitiesBySpawnFlags(ga_sesSessionState.ses_ulSpawnFlags);

  // remember the world filename
  ga_fnmWorld = ga_fnmNextLevel;
  // remember the world pointer
  _pShell->SetINDEX("pwoCurrentWorld", (INDEX)&ga_World);

  // set overdue timers in just loaded world to be due in current time
  ga_World.AdjustLateTimers(ga_sesSessionState.ses_tmLastProcessedTick);

  ga_sesSessionState.ses_bWaitingForServer = TRUE;

  // flush stale caches
  FreeUnusedStock();
  // mark that pretouching is required
  _bNeedPretouch = TRUE;

  FinishCRCGather();
/* //0522 kwon 삭제.
    // send data request
  CPrintF(TRANS("Sending full game state request\n"));
  CNetworkMessage nmRequestGameState(MSG_REQ_GAMESTATE);
  _pNetwork->SendToServerReliable(nmRequestGameState);
  _cmiComm.Client_Update();
  _cmiComm.Client_Update();
  _cmiComm.Client_Update();
  _cmiComm.Client_Update();
  _cmiComm.Client_Update();
  // wait for server's response
  CTMemoryStream strmMessage;

  _pNetwork->ga_sesSessionState.WaitStream_t(strmMessage, "data", MSG_REP_GAMESTATE);

  CTMemoryStream strmGameState;
  CzlibCompressor comp;
  comp.UnpackStream_t(strmMessage, strmGameState);

  // get the number of server level changes, and store it
  strmGameState>>ga_sesSessionState.ses_ubNumLevelChanges;

  // set proper ID sequence for entities created by this machine - used to separate 
  // server and client generated IDs
  if (!_pNetwork->IsServer()) {
    _pNetwork->ga_World.wo_ulNextEntityID = 0x10000001;
  }
  ga_sesSessionState.Read_t(&strmGameState,TRUE);

  _lphCurrent = LCP_CHANGED;
  EPostLevelChange ePostChange;
  ePostChange.iUserData = _pNetwork->ga_iNextLevelUserData;
  ga_sesSessionState.SendLevelChangeNotification(ePostChange);
  CEntity::HandleSentEvents();
  _lphCurrent = LCP_NOCHANGE;

  _pNetwork->SetLocalPause(bPaused);
  StopProgress();

  ga_sesSessionState.ses_tvMessageReceived = _pTimer->GetHighPrecisionTimer(); 

  SendActionsToServer();
  _cmiComm.Client_Update();
*/
}



// split the rcon response string into lines and send one by one to the client
static void SendAdminResponse(ULONG ulAdr, UWORD uwPort, ULONG ulCode, const CTString &strResponse)
{
  CTString str = strResponse;
  INDEX iLineCt = 0;
/* //0522 kwon 삭제.
  while (str!="") {
    CTString strLine = str;
    strLine.OnlyFirstLine();
    str.RemovePrefix(strLine);
    str.DeleteChar(0);
    if (strLine.Length()>0) { 
      CNetworkMessage nm(MSG_EXTRA);
      nm<<CTString(0, "log %u %d %s\n", ulCode, iLineCt++, strLine);
      _pNetwork->SendBroadcast(nm, ulAdr, uwPort);
    }
  }
*/
}


/*
 * Main loop.
 */
void CNetworkLibrary::MainLoop(void)
{
  TRACKMEM(Mem,"Network");

#if !defined(_DEBUG) && !defined(KALYDO) && !defined(G_KOR) && !defined(VER_TEST)
  BOOL bIsDebuggerPresent = FALSE;
  __asm {
	  // TIB(Thread Information Block)의 위치 얻기
	  mov		eax, fs:[0x18]
	  // TIB에서 0x30 위치에 해당하는 것이 debugging과 관련된 구조체에 대한 포인터임
	  mov		eax, dword ptr [eax+0x30]
	  // 해당 구조체에서 두번째 WORD에 저장된 값이 현재 프로세스가 디버깅 중인지에 대한 값이다.
	  movzx	eax, byte ptr [eax+2]
	  // 결과 리턴
	  mov		bIsDebuggerPresent, eax
  }
  if (bIsDebuggerPresent) {
	  static int counter = 0;
	  counter++;
	  if (counter == 10) {
		  _pGameState->Running() = FALSE;
		  _pGameState->QuitScreen() = FALSE;
	  }
  }
#endif

  // synchronize access to network
  CTSingleLock slNetwork(&ga_csNetwork, TRUE);
  if (ga_bFirstMainLoop) {
    ga_tvLastMainLoop = _pTimer->GetHighPrecisionTimer();
    ga_bFirstMainLoop = FALSE;
  }
/* //0311 삭제
  //! 1명이상 플레이 하고 있다면,
  // update network state variable (to control usage of some cvars that cannot be altered in mulit-player mode)
  _bMultiPlayer = (_pNetwork->ga_sesSessionState.GetPlayersCount() > 1);
*/
  //! 월드 체인지.
  // if should change world
  if (_lphCurrent==LCP_SIGNALLED) {
    // really do the level change here
    ChangeLevel_internal();
    _lphCurrent=LCP_CHANGED;
  }
/* //0311 삭제
  if (_bStartDemoRecordingNextTime) {
    if (!_pNetwork->IsServer()) {
      CPrintF("Demos can be recorded only on server computer or in single player mode.\n");
    } else {
      ga_srvServer.srv_tmNextAvailableDemoTimeSlot = _pTimer->GetHighPrecisionTimer().GetSeconds();
      _bStartDemoRecordingNextTime = 0.0f;

      if (!ga_bDemoRec) {
        try {
          CTString strName;
          strName.PrintF("Temp\\Recorded%02d.dem", (INDEX)dem_iRecordedNumber);
          StartDemoRec_t(strName);
          dem_iRecordedNumber+=1;
        } catch(char *strError) {
          CPrintF(TRANS("Demo recording error: %s\n"), strError);
        }
      }
    }
  }
  if (_bStopDemoRecordingNextTime) {
    _bStopDemoRecordingNextTime = 0.0f;
    if (ga_bDemoRec) {
      StopDemoRec();
    }
  }
*/
  _sfStats.StartTimer(CStatForm::STI_MAINLOOP);


  TIME tmBefore;
  CTimerValue tvNewMainLoop = _pTimer->GetHighPrecisionTimer();
  static CTimerValue tvLastAction = -1.0f;
  if (tvLastAction.GetSeconds() < 0) {
    tvLastAction = _pTimer->GetHighPrecisionTimer();
  }

  /*
  {CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();
  if (ga_bDemoPlay) {
    ga_sesSessionState.SetLerpFactor(CTimerValue(ga_fDemoTimer));
  } 
  */
  /*else {
    ga_sesSessionState.SetLerpFactor(tvNow);
  }}
  */
/*
  if (_pNetwork->IsPlayingDemo() && !_pNetwork->ga_bDemoPlayFinished) {
    int ctUpdates=0;
    while (_pNetwork->ga_fDemoTimer > (_pNetwork->ga_tmNextDemoTick) && !_pNetwork->ga_bDemoPlayFinished) {
      ga_sesSessionState.RunDemoTick();
      ctUpdates++;
    }
    if (ctUpdates) {
      ga_sesSessionState.HandleTimers(_pNetwork->ga_tmNextDemoTick - _pTimer->TickQuantum);
      ga_sesSessionState.HandleMovers_client();
    }
  }
*/
  //0109
  //ga_fGameRealTimeFactor:게임 시간 가속인자 =1
  //ses_fRealTimeFactor : 특수효과에 대하여 시간을 느리게 하거나 빠르게 하는 인자 =1
  //tvLastAction : 마지막 액션을 한 시간
  //tvNewMainLoop : 네트워크 메인loop에 들어온 시간.
  extern BOOL _bWorldEditorApp;
  TIME fPeriod = _pTimer->TickQuantum/(ga_fGameRealTimeFactor*ga_sesSessionState.ses_fRealTimeFactor);
  double dDeltaTime = tvNewMainLoop.GetSeconds() - ga_tvLastMainLoop.GetSeconds();

  // NOTE : 캐릭터가 제자리에서 못움직이던 문제 때문에...
  // NOTE : 원인은 dDeltaTime이 음수값이어서 그랬음...
  // NOTE : 월드 로딩이 오래걸리거나, 문제가 생길경우에...
  if( !ga_bFirstMainLoop && dDeltaTime < 0 )
  {
	  ga_tvLastMainLoop = tvNewMainLoop.GetSeconds();
	  dDeltaTime = 0.0f;
  }

  ULONG ulNumTicks = (dDeltaTime) / _pTimer->TickQuantum;

  dDeltaTime = tvNewMainLoop.GetSeconds() - tvLastAction.GetSeconds();

  // NOTE : 캐릭터가 제자리에서 못움직이던 문제 때문에...
  // NOTE : 원인은 dDeltaTime이 음수값이어서 그랬음...
  // NOTE : 월드 로딩이 오래걸리거나, 문제가 생길경우에...
  if( dDeltaTime < 0 )
  {
	  tvLastAction = tvNewMainLoop.GetSeconds();
	  dDeltaTime = 0.0f;
  }

  ULONG ulActions = (dDeltaTime) / fPeriod;
  ULONG ulClampedTicks = Clamp(ulNumTicks,(ULONG)0, (ULONG)2);
  ULONG ulClampedActions = Clamp(ulActions, (ULONG)0, (ULONG)10);
  extern INDEX net_bReportMiscErrors;
  if (net_bReportMiscErrors) 
  {
	  if ((ulNumTicks - ulClampedTicks) > 50) 
	  {
		  CPrintF(TRANS("Warning: main loop time clamp by %ld ticks!\n"),ulNumTicks-ulClampedTicks);
	  }
	  
	  if (ga_IsServer || _bWorldEditorApp) 
	  {
		  if ((ulClampedActions - ulActions) > 50) 
		  {
			  CPrintF(TRANS("Warning: main loop actions clamp by %ld ticks!\n"),ulClampedActions - ulActions);
		  }
	  }
  }

  INDEX ctTicks = 0;

  // handle messages for session state
  if (!ga_bDemoPlay) 
  {
    if (ga_IsServer) 
	{
      if (_cmiComm.Client_Update() == FALSE) 
	  {
        _sfStats.StopTimer(CStatForm::STI_MAINLOOP);
        ga_sesSessionState.Stop();
        return;
      } 
      if ((ga_IsServer || _bWorldEditorApp) && _cmiComm.cci_bClientInitialized) 
	  {
        while (ctTicks < ulClampedActions) 
		{
		  //0109 
		  //ulActions는 지난번 액션후 밀린 액션 갯수
		  //ulClampedActions는 처리 허용 액션갯수.최대 10개.
          // make actions packet for all local players and send to server
          SendActionsToServer();
          ctTicks++;
        }
        tvLastAction += ulActions*fPeriod;
      }
  
      if (_cmiComm.Client_Update() == FALSE) 
	  {
        ga_sesSessionState.Stop();
        _sfStats.StopTimer(CStatForm::STI_MAINLOOP);
        return;
      }
    }
  }
   
  // if this is server computer
  if (ga_IsServer) 
  {
    ctTicks=0;
    // handle server messages
    _cmiComm.Server_Update();

    while (ctTicks < ulClampedTicks) 
	{
      ga_srvServer.ServerLoop();     
      ctTicks++;
    }

    _cmiComm.Server_Update();
    ga_srvServer.HandleAll();
  }
/* //0311 삭제
//! 클라이언트 업데이트.
    if (_cmiComm.Client_Update() == FALSE) {
      ga_sesSessionState.Stop();
      _sfStats.StopTimer(CStatForm::STI_MAINLOOP);
      return;
    }
*/
//! 세션 loop
    ga_sesSessionState.SessionStateLoop();
/* //0311 삭제     
//! 또 클라이언트 업데이트.
    if (_cmiComm.Client_Update() == FALSE) {
      ga_sesSessionState.Stop();
      _sfStats.StopTimer(CStatForm::STI_MAINLOOP);
      return;
    }
//  }  
 */
  if (IsServer() ) {
    _cmiComm.Server_Update();
  }  

  ga_tvLastMainLoop += ulNumTicks * _pTimer->TickQuantum;

  
  // let server process game stream
  tmBefore = _pTimer->GetRealTimeTick();
  _pTimer->SetLerp(0.0f);

  if (IsServer()) {
    _cmiComm.Server_Update();
    ga_srvServer.HandleAll();
	//! 서버로 부터 오는 게임 스트림을 처리한다. 로컬클라이언트.
    // process the game stream coming from the server
    ga_sesSessionState.ProcessGameStream();
  }
/* //0311 삭제
//! tick 데이타를 클라이언트로 보낸다. 
  // sent tick data to the clients only once, and after the last tick has been processed
  if (ulClampedTicks>0 && ga_IsServer && !ga_bDemoPlay && ga_srvServer.srv_bActive) {
    ga_srvServer.ServerNetProcess();     
    _cmiComm.Server_Update();
  }
*/
  ga_sesSessionState.ses_tmLastUpdated = _pTimer->GetRealTimeTick();

  TIME tmAfter = _pTimer->GetRealTimeTick();
  ga_sesSessionState.ses_bKeepingUpWithTime = (tmAfter-tmBefore)<=_pTimer->TickQuantum*2.01f;

  CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();
  // set the lerping factor for current frame
  if (!ga_bDemoPlay) {
    ga_sesSessionState.SetLerpFactor(tvNow);
//안태훈 수정 시작	//(Add Sun Moon Entity and etc)(0.2)
	g_fGWTime += (float)(( tvNow - ga_tvDemoTimerLastTime ).GetSeconds()) * RWTOneSec2GWTSec * g_fGWTimeMul;
	if( g_fGWTime > (FLOAT)GAMETIME_ONEDAY_SECOND ){
		g_fGWTime -= (FLOAT)GAMETIME_ONEDAY_SECOND * int(g_fGWTime / (FLOAT)GAMETIME_ONEDAY_SECOND);															// yjpark     -->|
	}
//안태훈 수정 끝	//(Add Sun Moon Entity and etc)(0.2)
  } else {
    ga_sesSessionState.SetLerpFactor(CTimerValue(ga_fDemoTimer));
  }
  ga_tvDemoTimerLastTime = tvNow;
/* //0311 삭제
  // if playing a demo
  if (ga_bDemoPlay) {
    // if synchronizing by real time
    if (ga_fDemoSyncRate==DEMOSYNC_REALTIME) {
      // if server is keeping up
      if (ga_sesSessionState.ses_bKeepingUpWithTime) {
        // add passed time with slow/fast factor
        ga_fDemoTimer += FLOAT((tvNow-ga_tvDemoTimerLastTime).GetSeconds())
          *ga_fDemoRealTimeFactor*ga_sesSessionState.ses_fRealTimeFactor;
      }
    // if synchronizing is stopped
    } else if (ga_fDemoSyncRate==DEMOSYNC_STOP) {
      // don't step
      NOTHING;
    // if synchronizing by given steps
    } else {
      // just add the step
      dem_iMotionBlurInterFrames = Clamp( dem_iMotionBlurInterFrames, 0L, 99L);
      const FLOAT fInterFrames = (dem_iAnimFrame<0) ? 1.0f : (dem_iMotionBlurInterFrames+1);
      ga_fDemoTimer += ga_fDemoRealTimeFactor / (ga_fDemoSyncRate*fInterFrames);
    }
  }
  // remember the demo timer
*/
/*#if _GAMESPY
  // do services for gamespy querying
  //GameSpy_ServerHandle();
#endif*/

  // if network
  if (_cmiComm.IsNetworkEnabled()) {
    // repeat
    FOREVER {
      CNetworkMessage nmReceived;

      ULONG ulFrom;
      UWORD uwPort;
      BOOL bHasMsg = ReceiveBroadcast(nmReceived, ulFrom, uwPort);
      // if there are no more messages
      if (!bHasMsg) {
        // finish
        break;
      }
/*
//! 서버는 이밑으로 안들어가~
      // if this message is not valid rcon message or a server enumeration request
      if (nmReceived.GetType() != MSG_EXTRA && nmReceived.GetType() != MSG_REQ_ENUMSERVERS) {
        // skip it
        continue;
      }

      // if requesting enumeration and this is server, and the server is visible and the game is not finished
      if (nmReceived.GetType()==MSG_REQ_ENUMSERVERS && IsServer() && ser_bEnumeration && !IsGameFinished()) {
      


        CTString strGameType;
        // get function that will provide us the info about gametype
        CShellSymbol *pss = _pShell->GetSymbol("GetCurrentGameTypeName",  TRUE);
        // if none
        if (pss==NULL) {
          // just give dummy info
          strGameType = "N/A";
        // if found
        } else {
          // get the info
          CTString (*pFunc)(void) = (CTString (*)(void))pss->ss_pvValue;
          strGameType = pFunc();
        }

        // create response
        CNetworkMessage nmEnum(MSG_SERVERINFO);
        nmEnum<<ga_strSessionName;
        nmEnum<<ga_World.wo_strName;
        nmEnum<<ga_srvServer.GetPlayersCount();
        nmEnum<<ga_sesSessionState.ses_ctMaxPlayers;
        nmEnum<<strGameType;
        nmEnum<<_strModName;
        nmEnum<<_SE_VER_STRING;
        nmEnum<<0.0f; // ping
      

        // send it back
        SendBroadcast(nmEnum,INADDR_BROADCAST,uwPort);
      } else {
        // get the string from the message
        CTString strMsg;
        nmReceived>>strMsg;

        // if this is server
        if (IsServer()) {
          // accept requests
          if (!strMsg.RemovePrefix("rcmd ")) {
            continue;
          }
          ULONG ulCode;
          char strPass[80];
          char strCmd[256];
          strMsg.ScanF("%u \"%80[^\"]\"%256[^\n]", &ulCode, strPass, strCmd);
          CTString strAdr = AddressToString(ulFrom);

          if (net_strAdminPassword=="" || net_strAdminPassword!=strPass) {
            CPrintF(TRANS("Server: Client '%s', Wrong password for remote administration.\n"), (const char*)strAdr);
            continue;
          }

          CPrintF(TRANS("Server: Client '%s', Admin cmd: %s\n"), (const char*)strAdr, strCmd);

          con_bCapture = TRUE;
          con_strCapture = "";
          _pShell->Execute(CTString(strCmd)+";");

          CTString strResponse = CTString(">")+strCmd+"\n"+con_strCapture;
          SendAdminResponse(ulFrom, uwPort, ulCode, strResponse);
          con_bCapture = FALSE;
          con_strCapture = "";
        }
      }
*/
    }
  }
  _sfStats.StopTimer(CStatForm::STI_MAINLOOP);
}
//! 이함수는 로컬 플레이어를 위한것.
// make actions packet for local players and send to server
void CNetworkLibrary::SendActionsToServer(void)
{
	// make the packet
	CNetworkMessage nmAction((UBYTE)MSG_ACTIONS);
	
	// for all local players on this machine
	for(INDEX ipls=0; ipls<ga_aplsPlayers.Count(); ++ipls) 
	{
		CPlayerSource &pls = ga_aplsPlayers[ipls];
		// create action packet if the player exists
		pls.WriteActionPacket(nmAction);
	}
	// send the packet
	//0130 1line 지우기
	//0203
	if (_pNetwork->IsServer())
	{
		SendToServer(nmAction);
	}
	
	if (_cmiComm.Client_Update() == FALSE) 
	{
		ga_sesSessionState.Stop();
		return;
	}
}

/* Get count of local players. */
INDEX CNetworkLibrary::GetLocalPlayerCount(void)
{
  INDEX iCount = 0;
  // for all local players on this machine
  for(INDEX ipls=0; ipls<ga_aplsPlayers.Count(); ++ipls) {
    CPlayerSource &pls = ga_aplsPlayers[ipls];
    // create action packet if the player exists
    if (pls.IsActive()) {
      iCount++;
    }
  }
  return iCount;
}

/* Get player entity for a given local player. */
CEntity *CNetworkLibrary::GetLocalPlayerEntity(CPlayerSource *ppls)
{
  // synchronize access to network
  CTSingleLock slNetwork(&ga_csNetwork, TRUE);

  // get the index of the player target in game state
  INDEX iPlayerTarget = ppls->pls_Index;
  // if player is not added
  if (iPlayerTarget<0) {
    // no entity
    return NULL;
  // if player is added
  } else {
    // get the entity from player target
    CPlayerTarget &plt = ga_srvServer.srv_apltPlayers[iPlayerTarget];
    CPlayerEntity *pen = plt.plt_penPlayerEntity;
    return pen;
  }
}

/* Get player entity for a given player by name. */
CEntity *CNetworkLibrary::GetPlayerEntityByName(const CTString &strName)
{
  // synchronize access to network
  CTSingleLock slNetwork(&ga_csNetwork, TRUE);

  // for each player in game
  CStaticArray<CPlayerTarget> &aplt = ga_srvServer.srv_apltPlayers;
  for(INDEX iplt = 0; iplt<aplt.Count(); ++iplt) {
    // if it is active and has that name
    if (aplt[iplt].IsActive()
      &&aplt[iplt].plt_penPlayerEntity->en_pcCharacter.GetName()==strName) {
      // return it
      return aplt[iplt].plt_penPlayerEntity;
    }
  }
  // else not found
  return NULL;
}

/* Get number of entities with given name. */
INDEX CNetworkLibrary::GetNumberOfEntitiesWithName(const CTString &strName)
{
  INDEX ctEntities = 0;
  {FOREACHINDYNAMICCONTAINER(ga_World.wo_cenEntities, CEntity, iten) {
    if (iten->GetName()==strName) {
      ctEntities++;
    }
  }}
  return ctEntities;
}

/* Get n-th entity with given name. */
CEntity *CNetworkLibrary::GetEntityWithName(const CTString &strName, INDEX iEntityWithThatName)
{
  INDEX ctEntities = 0;
  CEntity *pen = NULL;
  {FOREACHINDYNAMICCONTAINER(ga_World.wo_cenEntities, CEntity, iten) {
    if (iten->GetName()==strName) {
      if (ctEntities==iEntityWithThatName) {
        pen = iten;
        break;
      }
      ctEntities++;
    }
  }}
  return pen;
}

/* Get number of entities with given name. */
INDEX CNetworkLibrary::GetNumberOfPlayerEntitiesWithName(const CTString &strName)
{
  INDEX ctEntities = 0;
  {FOREACHINDYNAMICCONTAINER(ga_World.wo_cenEntities, CEntity, iten) {
    if (iten->IsPlayer() && iten->GetName()==strName) {
      ctEntities++;
    }
  }}
  return ctEntities;
}

/* Get n-th player entity with given name. */
CPlayerEntity *CNetworkLibrary::GetNthPlayerEntityWithName(const CTString &strName, INDEX iEntityWithThatName)
{
  INDEX ctEntities = 0;
  CEntity *pen = NULL;
  {FOREACHINDYNAMICCONTAINER(ga_World.wo_cenEntities, CEntity, iten) {
    if (iten->IsPlayer() && iten->GetName()==strName) {      
      if (ctEntities==iEntityWithThatName) {
        pen = iten;
        break;
      }
      ctEntities++;
    }
  }}
  return (CPlayerEntity*)pen;
}

/* Get n-th player entity. */
CPlayerEntity *CNetworkLibrary::GetNthPlayerEntity(INDEX iEntity)
{
  INDEX ctEntities = 0;
  CEntity *pen = NULL;
  {FOREACHINDYNAMICCONTAINER(ga_World.wo_cenEntities, CEntity, iten) {
    if (iten->IsPlayer()) {      
      if (ctEntities==iEntity) {
        pen = iten;
        break;
      }
      ctEntities++;
    }
  }}
  return (CPlayerEntity*)pen;
}

/* Get n-th entity of given class. */
CEntity *CNetworkLibrary::GetEntityOfClass(const CTString &strClass, INDEX iEntityOfThatClass)
{
  INDEX ctEntities = 0;
  CEntity *pen = NULL;
  {FOREACHINDYNAMICCONTAINER(ga_World.wo_cenEntities, CEntity, iten) {
    if (IsOfClass(iten, strClass)) {
      pen = iten;
      if (ctEntities==iEntityOfThatClass) {
        break;
      }
      ctEntities++;
    }
  }}
  return pen;
}

// wooss 070305 ------------------------------------------------->>
// kw : WSS_WHITEDAY_2007
CEntity* CNetworkLibrary::GetEntityByTypeIndex(SBYTE sbCharType, SLONG slCharIndex)
{
	CEntity* penEntity=NULL;

	if( _pNetwork->MyCharacterInfo.index == slCharIndex )
	{
		penEntity = (CPlayerEntity*)CEntity::GetPlayerEntity(0);
	}
	else
	{
		ObjectBase* pObject = ACTORMGR()->GetObject((eOBJ_TYPE)sbCharType, slCharIndex);

		if (pObject != NULL)
		{
			penEntity = pObject->GetEntity();
		}
	}

	return penEntity;
}

// --------------------------------------------------------------<<

// wooss 070305 ------------------------------------------------->>
// kw		:	WSS_EVENT_LOD
// Argument	:	Server Country Code
// return	: 	Client Country Code
	
	//Clinet Define country
	//KOREA					0		// 한국
	//TAIWAN				1		// 대만
	//CHINA					2		// 중국
	//THAILAND				3		// 태국
	//TAIWAN2				4		// 대만 천하대란 wooss 050929
	//JAPAN					5		// 일본 wooss 051123
	//MALAYSIA    			6
	//USA					7		// 미국
	//BRAZIL				8		// 브라질
	//HONGKONG				9		// 홍콩

	//Sever Define country
	//LC_KOR		0	// 국내
	//LC_TWN		1	// 대만 - 혁명
	//LC_TWN2		2	// 대만 - 천하대란, LC_TWN과 동시에 설정해야 함
	//LC_CHN		3	// 중국
	//LC_TLD		4	// 태국
	//LC_TLD_ENG	5	// 태국 영어
	//LC_JPN		6 	// 일본
	//LC_MAL		7 	// 말레이시아
	//LC_MAL_ENG	8 	// 말레이시아 영어 
	//LC_USA		9	// 미국
	//LC_BRZ		10	// 브라질
	//LC_HBK		11	// 홍콩

int CNetworkLibrary::ReturnCCC(int iSCC)
{
	extern INDEX g_iCountry;

	int iCCC;
	switch(iSCC)
	{
		case 0:
			iCCC = KOREA;
			break;
		case 4:
		case 5:
			iCCC = THAILAND;
			break;
		case 9:
			iCCC = USA;
			break;
		case 10:
			iCCC = BRAZIL;
			break;
		case 13:
			iCCC = g_iCountry;
			break;
		case 14://FRANCE_SPAIN_CLOSEBETA_NA_20081124
			iCCC = SPAIN;
			break;
		case 15:
			iCCC = FRANCE;
			break;
		case 16:
			iCCC = POLAND;
			break;
		case 17:
			iCCC = RUSSIA;
			break;
		case 19:
			iCCC = ITALY;
			break;
		case 20:
			iCCC = MEXICO;
			break;
		default:
			iCCC = -1;
			break;
	}

	return iCCC;
}
// --------------------------------------------------------------<<



/* Test if a given player is local to this computer. */
BOOL CNetworkLibrary::IsPlayerLocal(CEntity *pen)
{
  return GetPlayerSource(pen)!=NULL;
}
// get player source for a given player if it is local to this computer
CPlayerSource *CNetworkLibrary::GetPlayerSource(CEntity *pen)
{
  // synchronize access to network
  CTSingleLock slNetwork(&ga_csNetwork, TRUE);

  // for all local player on this machine
  {FOREACHINSTATICARRAY(ga_aplsPlayers, CPlayerSource, itpls) {
    // get the index of the player target in game state
    INDEX iPlayerTarget = itpls->pls_Index;
    // if player is added
    if (iPlayerTarget>=0) {
      // get the player target
      CPlayerTarget &plt = ga_srvServer.srv_apltPlayers[iPlayerTarget];
      // if it is that one
      if (plt.plt_penPlayerEntity == pen) {
        // return it
        return itpls;
      }
    }
  }}

  // if not found, it is not local
  return NULL;
}

// get game time in currently running game
TIME CNetworkLibrary::GetGameTime(void)
{
  return ga_sesSessionState.ses_tmLastProcessedTick;
}

/*
 * Add a new client to game.
 */
CPlayerSource *CNetworkLibrary::AddPlayer_t(CPlayerCharacter &pcCharacter)  // throw char *
{
  // synchronize access to network
  CTSingleLock slNetwork(&ga_csNetwork, TRUE);
//  CPrintF( TRANS("Adding player: '%s'\n"), RemoveSpecialCodes(pcCharacter.GetNameForPrinting()));

  // for all local clients on this machine
	FOREACHINSTATICARRAY(ga_aplsPlayers, CPlayerSource, itcls) 
	{
		if(itcls->IsActive())
		{
			itcls->Stop();
		}
    // if client is not active
		if (!itcls->IsActive()) 
		{
      // activate it
      itcls->Start_t(pcCharacter);
//      CPrintF( TRANS("  done.\n"));
      return &itcls.Current();
    }
  }
  // number of local clients is limited with NET_MAXLOCALCLIENTS
  ASSERTALWAYS("Adding too much local clients!");
  throw TRANS("Cannot add more local clients");
  return NULL;
}

/* Get session properties for current game. */
void *CNetworkLibrary::GetSessionProperties(void)
{
  // synchronize access to network (no need for that one!)
  //CTSingleLock slNetwork(&ga_csNetwork, TRUE);
  return ga_aubProperties;
}

void CNetworkLibrary::SendChat(ULONG ulFrom, ULONG ulTo, const CTString &strMessage)
{
  // if the string is too long
  if (strlen(strMessage)>256) {
    // refuse it
    return;
  }

  // just make the message and send it to server
//  CNetworkMessage nm(MSG_CHAT_IN);
//  nm<<ulFrom;
//  nm<<ulTo;

// 채팅				: chat_type(uc) sender_index(n) sender_name(str) receiver_index(n) receiver_name(str) chat(str)												
  ULONG recvIndex = -1;
  CTString	strRecvName="";
  /*
  	for(INDEX ipl=0; ipl<_pNetwork->ga_srvServer.srv_actCha.Count(); ipl++) 
	{
		CCharacterTarget &ct = _pNetwork->ga_srvServer.srv_actCha[ipl];
						
		if (ct.cha_strName == strRecvName) 
		{
			recvIndex = ct.cha_Index;
			break;
		}
	}
	if(recvIndex==-1)//만약 찾지 못했다면,
	{
		return;
	}
  */
  CNetworkMessage nm((UBYTE)MSG_CHAT);
  nm<< (unsigned char)MSG_CHAT_SAY; //0528 kwon
  nm<< MyCharacterInfo.index;
  nm<< MyCharacterInfo.name;
  nm<< strRecvName;
  nm<<strMessage;
  SendToServerNew(nm);
}

// save current version of engine
void CNetworkLibrary::WriteVersion_t(CTStream &strm)
{
  strm.WriteID_t("BUIV"); // build version
  strm<<INDEX(_SE_BUILD_MAJOR);
}

// load version of engine saved in file and check against current
void CNetworkLibrary::CheckVersion_t(CTStream &strm, BOOL bAllowReinit, BOOL &bNeedsReinit)
{
  // if not saved
  if (strm.PeekID_t()!=CChunkID("BUIV")) { // build version
    // behave as if everything is ok (for old versions)
    bNeedsReinit = FALSE;
    return;
  }
  strm.ExpectID_t("BUIV"); // build version
  // read the saved one
  INDEX iSaved;
  strm>>iSaved;
  // get current one
  INDEX iCurrent = _SE_BUILD_MAJOR;
  // if current version is an internal build
  if (iCurrent==0) {
    // it is never forced to reinit
    bNeedsReinit = FALSE;
    return;
  }

  // if current version is older than the saved one
  if (iCurrent<iSaved) {
    // it cannot be reinitialized
    ThrowF_t(TRANS("File '%s' was saved by a newer version of engine, it cannot be loaded"),
      strm.GetDescription());
    return;
  }

  // if current version is same as the saved one
  if (iCurrent==iSaved) {
    // all ok
    bNeedsReinit = FALSE;
    return;
  }

  // if current version is newer than the saved one
  if (iCurrent>iSaved) {
    // it should be reinitialized
    bNeedsReinit = TRUE;

    // if it may not be reinitialized
    if (!bAllowReinit) {
      ThrowF_t(TRANS("File '%s' was saved by an older version of engine, it cannot be loaded"),
        strm.GetDescription());
    }
    return;
  }
  // this should not happen
  ASSERT(FALSE);
  bNeedsReinit = FALSE;
  return;
}

// add a value to the netgraph
void CNetworkLibrary::AddNetGraphValue(enum NetGraphEntryType nget, FLOAT fLatency)
{
  net_iGraphBuffer = Clamp(net_iGraphBuffer, INDEX(20), INDEX(1000));

  // make sure the netgraph has wanted number of values
  if (ga_angeNetGraph.Count()!=net_iGraphBuffer) {
    ga_angeNetGraph.Clear();
    ga_angeNetGraph.New(net_iGraphBuffer);
    memset(&ga_angeNetGraph[0], 0, ga_angeNetGraph.Count()*sizeof(ga_angeNetGraph[0]));
  }

  // scroll the values in the netgraph by one value
  memmove(&ga_angeNetGraph[1], &ga_angeNetGraph[0], (ga_angeNetGraph.Count()-1)*sizeof(ga_angeNetGraph[0]));

  // add the new value
  ga_angeNetGraph[0].nge_ngetType = nget;
  ga_angeNetGraph[0].nge_fLatency = fLatency;
}

 
// add a value to the bandwidth graph
void CNetworkLibrary::AddBandwidthGraphValue(ULONG ulBandwidth)
{
  net_iGraphBuffer = Clamp(net_iGraphBuffer, INDEX(20), INDEX(1000));

  // make sure the netgraph has wanted number of values
  if (ga_aulBandwidthGraph.Count()!=net_iGraphBuffer) {
    ga_aulBandwidthGraph.Clear();
    ga_aulBandwidthGraph.New(net_iGraphBuffer);
    memset(&ga_aulBandwidthGraph[0], 0, ga_aulBandwidthGraph.Count()*sizeof(ga_aulBandwidthGraph[0]));
  }

  // scroll the values in the netgraph by one value
  memmove(&ga_aulBandwidthGraph[1], &ga_aulBandwidthGraph[0], (ga_aulBandwidthGraph.Count()-1)*sizeof(ga_aulBandwidthGraph[0]));

  // add the new value
  ga_aulBandwidthGraph[0] = ulBandwidth;
}
  
//! 게임이 아직 실행되지 않았을때는 브로드캐스트 메시지만 받는다.
// handle broadcast messages (server enumeration)
void CNetworkLibrary::GameInactive(void)
{
//#if _GAMESPY
//	GameSpy_EnumHandle();
//#endif

  // if no network
  if (!_cmiComm.IsNetworkEnabled()) {
    // do not handle
    return;
  }

  // repeat
  FOREVER {
    CNetworkMessage nmReceived;

    ULONG ulFrom;
    UWORD uwPort;
    BOOL bHasMsg = ReceiveBroadcast(nmReceived, ulFrom, uwPort);
    // if there are no more messages
    if (!bHasMsg) {
      // finish
      break;
    }
  }
}

void CNetworkLibrary::InitCRCGather(void)
{
  CRCT_ResetActiveList();
  CRCT_bGatherCRCs = TRUE;  
  
  // clear the container that holds pairs of entity class ID & entity class*
  ClearEntityClassContainer();

  CRCT_AddFile_t(CTString("Classes\\Player.ecl"));

  // add the Player to entity class contianer
  EntityClassEntry* peceEntry = new EntityClassEntry;
  peceEntry->ece_fnmEntityClass = CTString("Classes\\Player.ecl");
  peceEntry->ece_iClassID = 401;
  _cEntityClassContainer.Add(peceEntry);

  try {
    _pEntityClassStock->Release(_pEntityClassStock->Obtain_t(CTString("Classes\\Player.ecl")));
  } catch (char *strError) {
    CPrintF("WARNING: %s", strError);
  }

}

// finish gathering of file CRCs to CRC table (call for server only!)
void CNetworkLibrary::FinishCRCGather(void)
{
  try {
    // make the list
    CTMemoryStream strmCRC;
    CRCT_MakeFileList_t(strmCRC);
    // remember it
    strmCRC.SetPos_t(0);
    ga_slCRCList = strmCRC.GetStreamSize();
	if (ga_pubCRCList != NULL)
		FreeMemory(ga_pubCRCList);
    ga_pubCRCList = (UBYTE*)AllocMemory(ga_slCRCList);
    strmCRC.Read_t(ga_pubCRCList, ga_slCRCList);
    // remember its CRC
    strmCRC.SetPos_t(0);
    ga_ulCRC = CRCT_MakeCRCForFiles_t(strmCRC);
  } catch (char *strError) {
    CPrintF(TRANS("Warning, cannot get CRCs: %s\n"), strError);
  }
}

/* Start numeration of existing sessions. */
void CNetworkLibrary::EnumSessionsStart(BOOL bInternet)
{
  _bEnumerating = TRUE;
  // clear old list
  FORDELETELIST(CNetworkSession, ns_lnNode, ga_lhEnumeratedSessions, itns) {
    delete &*itns;
  }
	/*
// gamespy enumeration
#if _GAMESPY 
  // make sure network is on
  if (!_cmiComm.IsNetworkEnabled()) {
  _cmiComm.PrepareForUse(TRUE, FALSE); // have to enumerate as server
  }
  // request enumeration
  GameSpy_EnumTrigger(bInternet);
// Croteam enumeration 
#else
	*/
  // request enumeration
  _bSendEnumRequest = TRUE;
  // make sure network is on
  if (!_cmiComm.IsNetworkEnabled()) {
    _cmiComm.PrepareForUse(/*network*/TRUE, /*client*/TRUE); 
  }
  if (!ga_srvServer.srv_bActive) {
    ga_srvServer.Start_t();
  }
//#endif
}

/* Continue numeration of existing sessions. */
void CNetworkLibrary::EnumSessionsContinue()
{
/* //0522 kwon 삭제.
  // we will send enumeration requests two times per second
  const TIME tmRequestInterval = 0.75f;
  static TIME tmLastRequest = -1.0f;
  TIME tmNow = _pTimer->GetHighPrecisionTimer().GetSeconds();
  // if we have never enumerated before, or a enough time has passed since last request
  if (tmLastRequest<0 || ((tmNow-tmLastRequest) >= tmRequestInterval)) {
    // send enumeration request
    tmLastRequest = tmNow;
    CNetworkMessage nmEnum(MSG_REQ_ENUMSERVERS);
    SendBroadcast(nmEnum,INADDR_BROADCAST,net_iPort);
  }
  ga_bEnumerationChange = FALSE;
  // repeat
  FOREVER {
    CNetworkMessage nmReceived;
    // manage input/output buffers
    _cmiComm.Server_Update();
    ULONG ulFrom; UWORD uwPort;
    BOOL bHasMsg = ReceiveBroadcast(nmReceived, ulFrom,uwPort);
    // if there are no more messages
    if (!bHasMsg) {
      // finish
      break;
    } 
    // received a response to an enumeration request
    if (nmReceived.GetType()==MSG_SERVERINFO) {
      BOOL bFound = FALSE;
      
      //char strPort[8];
      //itoa(uwPort,strPort,10);
      //CTString strAddress = AddressToString(ulFrom) + CTString(":") + CTString(strPort);
      // create a new session
      CNetworkSession &nsNew = *new CNetworkSession;      

      // read it
      nmReceived>>nsNew.ns_strSession;
      nmReceived>>nsNew.ns_strWorld;
      nmReceived>>nsNew.ns_ctPlayers;
      nmReceived>>nsNew.ns_ctMaxPlayers;
      nmReceived>>nsNew.ns_strGameType;
      nmReceived>>nsNew.ns_strMod;
      nmReceived>>nsNew.ns_strVer;
      nmReceived>>nsNew.ns_tmPing;


      // try the find the server that responded in the list of existing sessions 
      FOREACHINLIST(CNetworkSession, ns_lnNode, ga_lhEnumeratedSessions, itns) {
        CNetworkSession &ns = *itns;

        BOOL bSameAddr = FALSE;
          if ((ns.ns_ulAddress == ulFrom) && (ns.ns_uwPort == uwPort)) {
            bSameAddr = TRUE;
          }
        
        // if the server is found
        if (bSameAddr) {
          // if there was a change in the number of players, flag it
          if (ns.ns_ctPlayers != nsNew.ns_ctPlayers) {
            ga_bEnumerationChange = TRUE;
          }
          //update it's data
          ns.ns_strSession = nsNew.ns_strSession;
          ns.ns_strWorld = nsNew.ns_strWorld;
          ns.ns_ctPlayers = nsNew.ns_ctPlayers;
          ns.ns_ctMaxPlayers = nsNew.ns_ctMaxPlayers;
          ns.ns_strGameType = nsNew.ns_strGameType;
          ns.ns_strMod = nsNew.ns_strMod;
          ns.ns_strVer = nsNew.ns_strVer;
          ns.ns_tmPing = nsNew.ns_tmPing;

            ns.ns_tmLastSeen = tmNow;
          bFound = TRUE;
          delete &nsNew;
          break;
        }
      }

      // if the server was not listed already, add it
      if (!bFound) {
        char strPort[8];
        itoa(uwPort,strPort,10);
        CTString strAddress = AddressToString(ulFrom) + CTString(":") + CTString(strPort);    
        nsNew.ns_ulAddress = ulFrom;
        nsNew.ns_uwPort = uwPort;
        nsNew.ns_tmLastSeen = tmNow;
        nsNew.ns_strAddress = strAddress;
        ga_lhEnumeratedSessions.AddTail(nsNew.ns_lnNode);            
        ga_bEnumerationChange = TRUE;
      }
    }
  }

  // run through the list and delete stale sessions
  FORDELETELIST(CNetworkSession, ns_lnNode, ga_lhEnumeratedSessions, itns) {
    CNetworkSession &ns = *itns;
    if (ns.ns_tmLastSeen < 0.0f || ((tmNow - ns.ns_tmLastSeen) > 2*tmRequestInterval)) {
      delete &*itns;
      ga_bEnumerationChange = TRUE;
    }
  }
  */
}


/* Stop numeration of existing sessions. */
void CNetworkLibrary::EnumSessionsStop()
{
#if !_GAMESPY
  if (ga_srvServer.srv_bActive) {
    ga_srvServer.Stop();
  }

  _cmiComm.Unprepare();
  _bEnumerating = FALSE;
#endif
}

//0105
void CNetworkLibrary::SendMessagetoServer(char *msg)
{
	char Buf[1024];
	
	sprintf(Buf, "say %s\n", msg);
	_tcpip.SendNetMessage(Buf, TRUE);
}
//..

// ----------------------------------------------------------------------------
// Name : GetWearingEffect()
// Desc : 착용중인 아이템의 이펙트 정보(이름)를 가져온다.
// ----------------------------------------------------------------------------
CTString CNetworkLibrary::GetWearingEffect(eEquipment Type, INDEX subType)
{
	CTString strEffectName = CTString("");

	if (_pNetwork->MyWearItem[Type].IsEmptyItem() == FALSE)
	{
		CItems* pItem = &_pNetwork->MyWearItem[Type];

		switch(subType)
		{
		case 0: // pItemData->ArmorEffectName
			{
				strEffectName = pItem->ItemData->GetArmorEffectName();
			}
			break;
		case 1: // pItemData->MissileShotEffect
			{
				strEffectName = pItem->ItemData->GetMissileShotEffect();
			}
			break;
		case 2: // pItemData->ShotHitEffect
			{
				strEffectName = pItem->ItemData->GetShotHitEffect();
			}
			break;
		}
	}

	return strEffectName;
}

void CNetworkLibrary::DropItem(int tabId, int inven_idx, SQUAD cnt)
{
	CItems& rItems = MySlotItem[tabId][inven_idx];
	//0616 kwon 입고있는 아이템은 드롭 못하게 하기.
	if(rItems.Item_Wearing != -1)
	{
//		CPrintF(TRANS("Can't Drop because this Item is Wearing... \n"));
		return;
	}

	CNetworkMessage nmMessage;
	RequestClient::doItemThrow* packet = reinterpret_cast<RequestClient::doItemThrow*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_THROW;
	packet->tab = rItems.Item_Tab;
	packet->invenIndex = rItems.InvenIndex;
	packet->virtualIndex = rItems.Item_UniIndex;
	packet->count = cnt;
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew(nmMessage);
}

void CNetworkLibrary::SwapItem(int tabId, int inven_idx, int tabId2, int inven_idx2)
{
	//CItems* pItems = &MySlotItem[tabId][inven_idx]; //source
	//CItems* pItems2 = &MySlotItem[tabId][inven_idx2];//target
	CNetworkMessage nmMessage;
	RequestClient::doItemSwap* packet = reinterpret_cast<RequestClient::doItemSwap*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_SWAP;
	packet->stab = tabId;
	packet->sinvenIndex = inven_idx;
	packet->ttab = tabId2;
	packet->tinvenIndex = inven_idx2;
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew(nmMessage);
//	CPrintF(TRANS("Send MSG_ITEM_SWAP : ITEM [%d][%d][%d] -> [%d][%d][%d]  Swaped!!! \n"),tabId,rowId,colId,tabId,rowId2,colId2);
}

void CNetworkLibrary::DivideItem( SWORD nTabS, SWORD nIdxS, SWORD nTabT, SWORD nIdxT, int count )
{
	CNetworkMessage nmMessage;
	RequestClient::doItemDivide* packet = reinterpret_cast<RequestClient::doItemDivide*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_DIVIDE;
	packet->tab = nTabS;
	packet->invenIndex = nIdxS;
	packet->ttab = nTabT;
	packet->tinvenIndex = nIdxT;
	packet->divide_count = count;
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew(nmMessage);
}

void CNetworkLibrary::ArrangeItem(int tab)
{				 
	CNetworkMessage nmItem((UBYTE)MSG_ITEM); 	
	nmItem << (SBYTE)MSG_ITEM_ARRANGE;								
	nmItem << (SBYTE)tab;
	SendToServerNew(nmItem);

	CUIManager::getSingleton()->GetInventory()->ClearAllItems( tab );
}

// ----------------------------------------------------------------------------
// Name : UpgradeItem()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::UpgradeItem( SLONG nWearPos, SLONG VirIndex, SLONG slPlus,
									SWORD nTab2, SWORD inven_idx2, SLONG slLevel, 
									INDEX iProtect, SWORD nTab3, SWORD inven_idx3 )
{				 
	CNetworkMessage nmMessage;
	RequestClient::doItemUpgrade* packet = reinterpret_cast<RequestClient::doItemUpgrade*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_UPGRADE_REQ;
	packet->wearPos = nWearPos;
	packet->vIndex = VirIndex;
	packet->tab_2 = nTab2;
	packet->invenIndex_2 = inven_idx2;
	
	packet->tab_3 = nTab3;
	packet->invenIndex_3 = inven_idx3;

	packet->plus = slPlus;
	packet->level = slLevel;
	packet->runeItemDBIndex = iProtect;

	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew(nmMessage);
}

// ----------------------------------------------------------------------------
// Name : UpgradeItem()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::ItemLevelDown(SWORD inven_idx1, SWORD inven_idx2)
{
	CNetworkMessage nmItem((UBYTE)MSG_ITEM);
	nmItem << (SBYTE)MSG_ITEM_LEVELDOWN;
	nmItem << inven_idx1;
	nmItem << inven_idx2;
	SendToServerNew(nmItem);
}

// ----------------------------------------------------------------------------
// Name : OptionAddItem()
// Desc : 블러드 아이템 & 정화 옵션 추가
// ----------------------------------------------------------------------------
void CNetworkLibrary::OptionAddItem(UWORD slWearPos, int WearVirIdx, SWORD nTab, SWORD inven_idx, int JemVirIdx)
{
	CNetworkMessage nmMessage;
	RequestClient::doItemAddOption* packet = reinterpret_cast<RequestClient::doItemAddOption*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_OPTION_ADD_REQ;
	packet->weapon_wearPos = slWearPos;
	packet->weapon_virtualIndex = WearVirIdx;
	packet->jam_tab = nTab;
	packet->jam_invenIndex = inven_idx;
	packet->jam_virtualIndex = JemVirIdx;
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew( nmMessage );
}

// ----------------------------------------------------------------------------
// Name : OptionDelItem()
// Desc : 블러드 아이템 & 정화 옵션 제거
// ----------------------------------------------------------------------------
void CNetworkLibrary::OptionDelItem(UWORD slWearPos, int WearVirIdx, SWORD nTab, SWORD inven_idx, int JemVirIdx)
{
	CNetworkMessage nmMessage;
	RequestClient::doItemDelOption* packet = reinterpret_cast<RequestClient::doItemDelOption*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_OPTION_DEL_REQ;
	packet->weapon_wearPos = slWearPos;
	packet->weapon_virtualIndex = WearVirIdx;
	packet->jam_tab = nTab;
	packet->jam_invenIndex = inven_idx;
	packet->jam_virtualIndex = JemVirIdx;
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew( nmMessage );
}

// ----------------------------------------------------------------------------
// Name : RefineReq()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::RefineReq( SWORD nTab, SWORD inven_idx )
{
	CNetworkMessage nmMessage;
	RequestClient::doItemRefin* packet = reinterpret_cast<RequestClient::doItemRefin*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_REFINE_REQ;
	packet->tab = nTab;
	packet->invenIndex = inven_idx;
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew( nmMessage );
}

// 이기환 수정 시작 ( 12. 6 ) : CNeedItems 배열 형태로 변환
// ----------------------------------------------------------------------------
// Name : ProcessReq()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::ProcessReq( SWORD nTab, SWORD inven_idx, 
								 SLONG slResultIndex, SLONG slResultCount, 
								 CNeedItems* NeedItems )
{
	CNetworkMessage nmMessage;
	RequestClient::doItemProcess* packet = reinterpret_cast<RequestClient::doItemProcess*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_PROCESS_REQ;
	packet->useitem_tab = nTab;
	packet->useitem_invenIndex = inven_idx;
	packet->resultItemDBIndex = slResultIndex;
	packet->listCount = slResultCount;

	for (int i = 0; i < slResultCount; ++i)
	{
		packet->list[i].tab =			NeedItems[i].MatTab;
		packet->list[i].invenIndex =	NeedItems[i].inven_idx;
		packet->list[i].count =   (int)NeedItems[i].llCount;
	}

	nmMessage.setSize( sizeof(*packet) + (sizeof(packet->list[0]) * slResultCount) );

	SendToServerNew( nmMessage );
}

// ----------------------------------------------------------------------------
// Name : ProductReq()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::ProductReq( SWORD nTab, SWORD inven_idx, 
								 SLONG slResultIndex, SLONG slResultCount, 
								 CNeedItems* NeedItems )
{
	CNetworkMessage nmMessage;
	RequestClient::doItemMake* packet = reinterpret_cast<RequestClient::doItemMake*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_MAKE_REQ;
	packet->useitem_tab = nTab;
	packet->useitem_invenIndex = inven_idx;
	packet->resultItemDBIndex = slResultIndex;
	packet->listCount = slResultCount;

	for (int i = 0; i < slResultCount; ++i)
	{
		packet->list[i].dbIndex = NeedItems[i].ItemData->GetItemIndex();
		packet->list[i].count = (int)NeedItems[i].llCount;
	}

	nmMessage.setSize( sizeof(*packet) + (sizeof(packet->list[0]) * slResultCount));

	SendToServerNew( nmMessage );

}
// 이기환 수정 끝 ( 12. 6 )

// 이기환 수정 시작 ( 12. 8 ) : 조합
// ----------------------------------------------------------------------------
// Name : MixReq()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::MixReq( SWORD nTab, SWORD inven_idx, SWORD* arrTab, SWORD* arrIdx )
{
	CNetworkMessage nmMessage;
	RequestClient::doItemMix* packet = reinterpret_cast<RequestClient::doItemMix*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_MIX_REQ;
	packet->useitem_tab = nTab;
	packet->useitem_invenIndex = inven_idx;

	for (int i = 0; i < MIX_ITEM_SLOT_COUNT; ++i)
	{
		packet->list[i].tab = arrTab[i];
		packet->list[i].invenIndex = arrIdx[i];
	}

	nmMessage.setSize( sizeof(*packet) + (sizeof(packet->list[0]) * MIX_ITEM_SLOT_COUNT));

	SendToServerNew( nmMessage );
}
// 이기환 수정 끝 (04.12.18)


//------------------------------------------------------------------------------
// CNetworkLibrary::CompoundReq
// Explain:  
// Date : 2005-01-12,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::CompoundReq( SWORD nTab, SWORD nInvenIdx, SWORD* arrTab, SWORD* arrIdx )
{
	CNetworkMessage nmMessage;
	RequestClient::doItemArcane* packet = reinterpret_cast<RequestClient::doItemArcane*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_ARCANE_REQ;
	packet->useitem_tab = nTab;
	packet->useitem_invenIndex = nInvenIdx;

	for (int i = 0 ; i < COMPOUND_ITEM_SLOT_COUNT; ++i)
	{
		packet->list[i].tab = arrTab[i];
		packet->list[i].invenIndex = arrIdx[i];
	}

	nmMessage.setSize( sizeof(*packet) + (sizeof(packet->list[0]) * COMPOUND_ITEM_SLOT_COUNT) );

	SendToServerNew( nmMessage );
}
// ----------------------------------------------------------------------------
// Name : SendProlongMessage() wooss 050816
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendProlongMessage(int tabId, int inven_idx)
{
	CItems* pItems = &MySlotItem[tabId][inven_idx];

	if(pItems->Item_Index ==-1) //비어있는 슬롯이다.
		return;
	
	CNetworkMessage nmMessage;
	RequestClient::doItemUse* packet = reinterpret_cast<RequestClient::doItemUse*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_USE_PROLONG;
	packet->tab = tabId;
	packet->invenIndex = inven_idx;
	packet->virtualIndex = pItems->Item_UniIndex;
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew(nmMessage);
}

// ----------------------------------------------------------------------------
// Name : SendCashItemMessage() wooss 060105
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendCashItemMessage(int nType, INDEX searchType, INDEX ReqPage)
{
	CNetworkMessage nmCash((UBYTE)MSG_EXTEND);
	nmCash << (ULONG)MSG_EX_CASHITEM;
	nmCash << (UBYTE)nType;

	switch(nType)
	{
	case MSG_EX_CASHITEM_LIST_REQ:
		{
			nmCash << (ULONG)searchType;

			if (ReqPage < 1)
			{
				nmCash << (ULONG)(1);
			}
			else
			{
				nmCash << (ULONG)ReqPage;
			}
		}
		break;
	case MSG_EX_CASHITEM_RECOMMAND_REQ:
		{
			nmCash << (ULONG)searchType;
		}
		break;
	case MSG_EX_CASHITEM_WISHLIST_SAVE_REQ:
		{
			nmCash << (ULONG)searchType; // 여기서는 CTID이다.
		}
	}

	SendToServerNew(nmCash);
}

void CNetworkLibrary::SendCashItemMessage(int nType)
{
//	int tv_i,nCnt;

	CUIManager* pUIManager = CUIManager::getSingleton();

	CNetworkMessage nmItem((UBYTE)MSG_EXTEND);
	nmItem << (ULONG)MSG_EX_CASHITEM;
	nmItem << (UBYTE)nType;

	switch(nType)
	{
		case MSG_EX_CASHITEM_SHOP_OPEN_REQ: // 2010.11 캐시샵 오픈 요청
		case MSG_EX_CASHITEM_WISHLIST_REQ: // 소망상자 리스트 요청
		case MSG_EX_CASHITEM_BALANCE_REQ :
		case MSG_EX_CASHITEM_CUPON_REQ : // 보유 쿠폰 리스트 요청
			SendToServerNew(nmItem);
			break;

		case MSG_EX_CASHITEM_PURCHASE_REQ :
		case MSG_EX_CASHITEM_PURCHASE_WITH_COUPON_REQ :
			break;

		case MSG_EX_CASHITEM_BRING_REQ :
			{
				// CashshopEX 리뉴얼

// 				CUIButtonEx tv_btn;
// 				for(tv_i=0 ,nCnt = 0; tv_i< INVEN_SLOT_TOTAL; tv_i++){
// 					tv_btn = pUIManager -> GetCashShop()->m_abtnInvenItems[tv_i];
// 					if(!tv_btn.IsEmpty())  nCnt++;
// 				}
// 				nmItem<<(ULONG)nCnt;
// 
// 				for(tv_i=0 ; tv_i<INVEN_SLOT_TOTAL ; tv_i++){
// 					tv_btn= pUIManager->GetCashShop()->m_abtnInvenItems[tv_i];
// 					if(tv_btn.IsEmpty()	)
// 						continue;
// 					nmItem<<(ULONG)tv_btn.GetItemUniIndex();
// 					nmItem<<(ULONG)tv_btn.GetCashIndex();
// 				}
// 				SendToServerNew(nmItem);
		
			}			
			break;

		case MSG_EX_CASHITEM_PURCHASELIST_REQ :
			SendToServerNew(nmItem);

			break;

		case MSG_EX_CASHITEM_PURCHASEHISTORY_REQ :
			{
// 				std::vector<CTString> tv_vecStr;
// 				int tv_curSel;
// 				// YEAR
// 				tv_vecStr = pUIManager->GetCashShop()->m_cbYear.GetVecString();
// 				tv_curSel = pUIManager->GetCashShop()->m_cbYear.GetCurSel();
// 				nmItem << (ULONG)atoi(tv_vecStr[tv_curSel].str_String);
// 				// MONTH
// 				tv_vecStr = pUIManager->GetCashShop()->m_cbMonth.GetVecString();
// 				tv_curSel = pUIManager->GetCashShop()->m_cbMonth.GetCurSel();
// 				nmItem << (UBYTE)atoi(tv_vecStr[tv_curSel].str_String);
// 				// DAY
// 				tv_vecStr = pUIManager->GetCashShop()->m_cbDay.GetVecString();
// 				tv_curSel = pUIManager->GetCashShop()->m_cbDay.GetCurSel();
// 				nmItem << (UBYTE)atoi(tv_vecStr[tv_curSel].str_String);
// 
// 				SendToServerNew(nmItem);
			}
			break;

		//선물 관련 :Su-won		|-------------------------------------------->
		case MSG_EX_CASHITEM_GIFT_REQ :			// 선물 보내기 요청
			{
				// CashshopEX 리뉴얼

				//charName(str) Msg(str) count(n) idx(n) ctid(n)

// 				nmItem<<pUIManager->GetCashShop()->m_ebChar.GetString();
// 				nmItem<<pUIManager->GetCashShop()->m_ebGiftMessage.GetString();
// 
// 				CUIButtonEx tv_btn;
// 				for(tv_i=0 ,nCnt = 0; tv_i< INVEN_SLOT_TOTAL; tv_i++){
// 					tv_btn = pUIManager -> GetCashShop()->m_abtnInvenItems[tv_i];
// 					if(!tv_btn.IsEmpty())  nCnt++;
// 				}
// 				nmItem<<(ULONG)nCnt;
// 
// 				for(tv_i=0 ; tv_i<INVEN_SLOT_TOTAL ; tv_i++){
// 					tv_btn= pUIManager->GetCashShop()->m_abtnInvenItems[tv_i];
// 					if(tv_btn.IsEmpty()	)
// 						continue;
// 					nmItem<<(ULONG)tv_btn.GetItemUniIndex();
// 					nmItem<<(ULONG)tv_btn.GetCashIndex();
// 				}
// 				SendToServerNew(nmItem);
			}
			break;

		case MSG_EX_CASHITEM_GIFT_SENDHISTORY_REQ:  // 보낸 선물 내역 리스트 요청 : y(n) m(c) d(c)
			{
// 				std::vector<CTString> tv_vecStr;
// 				int tv_curSel;
// 				// YEAR
// 				tv_vecStr = pUIManager->GetCashShop()->m_cbGiftYear.GetVecString();
// 				tv_curSel = pUIManager->GetCashShop()->m_cbGiftYear.GetCurSel();
// 				nmItem << (ULONG)atoi(tv_vecStr[tv_curSel].str_String);
// 				// MONTH
// 				tv_vecStr = pUIManager->GetCashShop()->m_cbGiftMonth.GetVecString();
// 				tv_curSel = pUIManager->GetCashShop()->m_cbGiftMonth.GetCurSel();
// 				nmItem << (UBYTE)atoi(tv_vecStr[tv_curSel].str_String);
// 				// DAY
// 				tv_vecStr = pUIManager->GetCashShop()->m_cbGiftDay.GetVecString();
// 				tv_curSel = pUIManager->GetCashShop()->m_cbGiftDay.GetCurSel();
// 				nmItem << (UBYTE)atoi(tv_vecStr[tv_curSel].str_String);
// 
// 				SendToServerNew(nmItem);
			}
			break;

		case MSG_EX_CASHITEM_GIFT_RECVHISTORY_REQ:  // 받은 선물 내역 리스트 요청 : y(n) m(c) d(c)
			{
// 				std::vector<CTString> tv_vecStr;
// 				int tv_curSel;
// 				// YEAR
// 				tv_vecStr = pUIManager->GetCashShop()->m_cbGiftYear.GetVecString();
// 				tv_curSel = pUIManager->GetCashShop()->m_cbGiftYear.GetCurSel();
// 				nmItem << (ULONG)atoi(tv_vecStr[tv_curSel].str_String);
// 				// MONTH
// 				tv_vecStr = pUIManager->GetCashShop()->m_cbGiftMonth.GetVecString();
// 				tv_curSel = pUIManager->GetCashShop()->m_cbGiftMonth.GetCurSel();
// 				nmItem << (UBYTE)atoi(tv_vecStr[tv_curSel].str_String);
// 				// DAY
// 				tv_vecStr = pUIManager->GetCashShop()->m_cbGiftDay.GetVecString();
// 				tv_curSel = pUIManager->GetCashShop()->m_cbGiftDay.GetCurSel();
// 				nmItem << (UBYTE)atoi(tv_vecStr[tv_curSel].str_String);
// 
// 				SendToServerNew(nmItem);
			}
			break;
		case MSG_EX_CASHITEM_GIFT_RECVLIST_REQ:  // 받은 선물 리스트 요청
			{
				SendToServerNew(nmItem);
			}
			break;
		case MSG_EX_CASHITEM_GIFT_RECV_REQ:		// 받은 선물 인벤으로 이동 요청 : count(n) idx(c) ctid(c)
			{
// 				CUIButtonEx tv_btn;
// 				for(tv_i=0 ,nCnt = 0; tv_i< INVEN_SLOT_TOTAL; tv_i++){
// 					tv_btn = pUIManager -> GetCashShop()->m_abtnInvenItems[tv_i];
// 					if(!tv_btn.IsEmpty())  nCnt++;
// 				}
// 				nmItem<<(ULONG)nCnt;
// 
// 				for(tv_i=0 ; tv_i<INVEN_SLOT_TOTAL ; tv_i++){
// 					tv_btn= pUIManager->GetCashShop()->m_abtnInvenItems[tv_i];
// 					if(tv_btn.IsEmpty()	)
// 						continue;
// 					nmItem<<(ULONG)tv_btn.GetItemUniIndex();
// 					nmItem<<(ULONG)tv_btn.GetCashIndex();
// 				}
// 				SendToServerNew(nmItem);
			}
			break;
		//선물 관련 :Su-won		<--------------------------------------------|
	}
}

// ----------------------------------------------------------------------------
// Name : SendChangeMyName() wooss 050816
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendChangMyName(CTString strMsg)
{

	CNetworkMessage nmItem((UBYTE)MSG_EXTEND);
		nmItem << (ULONG)MSG_EX_NAMECHANGE;
		nmItem << (ULONG)CHANGE_MY_NAME_ITEM;
		nmItem << strMsg;
	SendToServerNew(nmItem);

}


// ----------------------------------------------------------------------------
// Name : SendChangeGuildName() wooss 050816
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendChangGuildName(CTString strMsg)
{
	CNetworkMessage nmItem((UBYTE)MSG_EXTEND);
		nmItem << (ULONG)MSG_EX_NAMECHANGE;
		nmItem << (ULONG)CHANGE_GUILD_NAME_ITEM;
		nmItem << strMsg;
	SendToServerNew(nmItem);
}


// ----------------------------------------------------------------------------
// Name : SendExSlotMessage() wooss 050816
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendExSlotMessage(int slotNum)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	int tabId, inven_idx;

	pUIManager->GetInventory()->GetUseItemSlotInfo(tabId, inven_idx);
	CItems* pItems = &MySlotItem[tabId][inven_idx];

	pUIManager->CloseMessageBox(MSGCMD_USE_SLOT_ITEM);
	
	if(pItems->Item_Index ==-1) //비어있는 슬롯이다.
		return;

	if(slotNum>0) 
		slotNum--;

	CNetworkMessage nmMessage;
	RequestClient::doItemUse* packet = reinterpret_cast<RequestClient::doItemUse*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_USE;
	packet->tab = tabId;
	packet->invenIndex = inven_idx;
	packet->virtualIndex = pItems->Item_UniIndex;
	packet->extra_1 = slotNum;
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew( nmMessage );
}
// ----------------------------------------------------------------------------
// Name : SendItemWearingMSG()
// Desc : 
// ----------------------------------------------------------------------------
bool CNetworkLibrary::SendItemWearingMSG(SBYTE sbType, SBYTE sbWearPos, SWORD nTab, SWORD inven_idx, char take, int nVirIndex)
{
	CUIManager* pUIMgr = CUIManager::getSingleton();

	pUIMgr->ResetHoldBtn();

	if (pUIMgr->GetInventory()->IsLockedArrange() == TRUE)
		return false;

	pUIMgr->GetQuickSlot()->SetWearingLock(TRUE);

	if (_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING)
	{ // 비행 모드에서는 장비 착용을 할 수 없습니다.
		return false;
	}

	CEntity* pEntity = CEntity::GetPlayerEntity( 0 );
	CPlayerEntity* pPlayerEntity = (CPlayerEntity*)pEntity;

	CTString strSysmessage;

	if (pPlayerEntity->IsTransforming() && sbWearPos != WEAR_PET)
	{	// 강신 중에는 장비를 착용 및 탈착 할 수 없다
		strSysmessage.PrintF(_S(5179, "강신 중에는 이용할 수 없습니다."));
		ClientSystemMessage(strSysmessage, SYSMSG_ERROR);
		return false;
	}
	else if (pPlayerEntity->IsPolymophing() && sbWearPos != WEAR_PET)
	{	// 변신 중에는 장비를 착용 및 탈착 할 수 없다
		strSysmessage.PrintF(_S(5181, "변신 중에는 이용할 수 없습니다."));
		ClientSystemMessage(strSysmessage, SYSMSG_ERROR);
		return false;
	}

	if (sbType == MSG_ITEM_WEAR_COSTUME)
	{
		CNetworkMessage nmMessage;
		RequestClient::doItemWearCostume* packet = reinterpret_cast<RequestClient::doItemWearCostume*>(nmMessage.nm_pubMessage);
		packet->type = MSG_ITEM;
		packet->subType = MSG_ITEM_WEAR_COSTUME;
		packet->tab = nTab;
		packet->invenIndex = inven_idx;
		packet->wearPos = sbWearPos;
		nmMessage.setSize( sizeof(*packet) );

		SendToServerNew( nmMessage );
	}
	else if (sbType == MSG_ITEM_WEAR_COSTUME_TAKEOFF)
	{
		CNetworkMessage nmMessage;
		RequestClient::doItemWearCostumeTakeOff* packet = reinterpret_cast<RequestClient::doItemWearCostumeTakeOff*>(nmMessage.nm_pubMessage);
		packet->type = MSG_ITEM;
		packet->subType = MSG_ITEM_WEAR_COSTUME_TAKEOFF;
		packet->tab = nTab;
		packet->invenIndex = inven_idx;
		packet->vIndex = nVirIndex;
		packet->wearPos = sbWearPos;
		nmMessage.setSize( sizeof(*packet) );

		SendToServerNew( nmMessage );
	}
	else if (sbType == MSG_ITEM_WEAR_COSTUME_SUIT)
	{
		CNetworkMessage nmMessage;
		RequestClient::doItemWearCostumeSuit* packet = reinterpret_cast<RequestClient::doItemWearCostumeSuit*>(nmMessage.nm_pubMessage);
		packet->type = MSG_ITEM;
		packet->subType = MSG_ITEM_WEAR_COSTUME_SUIT;
		packet->tab = nTab;
		packet->invenIndex = inven_idx;
		nmMessage.setSize( sizeof(*packet) );

		SendToServerNew( nmMessage );
	}
	else if (sbType == MSG_ITEM_WEAR_COSTUME_SUIT_TAKEOFF)
	{
		CNetworkMessage nmMessage;
		RequestClient::doItemWearCostumeSuitTakeOff* packet = reinterpret_cast<RequestClient::doItemWearCostumeSuitTakeOff*>(nmMessage.nm_pubMessage);
		packet->type = MSG_ITEM;
		packet->subType = MSG_ITEM_WEAR_COSTUME_SUIT_TAKEOFF;
		packet->tab = nTab;
		packet->invenIndex = inven_idx;
		nmMessage.setSize( sizeof(*packet) );

		SendToServerNew( nmMessage );
	}
	else if (sbType == MSG_ITEM_WEAR_TAKE_OFF)
	{
		CNetworkMessage nmMessage;
		RequestClient::doItemWearTakeOff* packet = reinterpret_cast<RequestClient::doItemWearTakeOff*>(nmMessage.nm_pubMessage);
		packet->type = MSG_ITEM;
		packet->subType = MSG_ITEM_WEAR_TAKE_OFF;
		packet->wearPos = sbWearPos;
		packet->tab = nTab;
		packet->invenIndex = inven_idx;
		nmMessage.setSize( sizeof(*packet) );

		SendToServerNew( nmMessage );
	}
	else
	{
		CNetworkMessage nmMessage;
		RequestClient::doItemWear* packet = reinterpret_cast<RequestClient::doItemWear*>(nmMessage.nm_pubMessage);
		packet->type = MSG_ITEM;
		packet->subType = MSG_ITEM_WEAR;
		packet->wearPos = sbWearPos;
		packet->tab = nTab;
		packet->invenIndex = inven_idx;
		nmMessage.setSize( sizeof(*packet) );

		SendToServerNew( nmMessage );
	}
	
	return true;
}


// ----------------------------------------------------------------------------
// Name : UseSlotItem()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::UseSlotItem( int tabId, int inven_idx, SBYTE sbWearType )
{
	CItems* pItems = &MySlotItem[tabId][inven_idx];
	
	if(pItems->Item_Index ==-1) //비어있는 슬롯이다.
		return;

	// 아이템 쿨타임 검사
	DOUBLE dStartTime = 0;
	DOUBLE dCoolTime;
	DOUBLE dReUseTime = 0.0;

	dStartTime = pItems->ItemData->StartTime;
	dReUseTime = MY_INFO()->GetReuseItem(pItems->Item_Index);
	dCoolTime = ItemHelp::GetCoolTime(dReUseTime, dStartTime);

	if (dCoolTime > 0.0)
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();
	char castlewar = pItems->ItemData->GetCastleWar();

	if (castlewar != CItemData::eANYWHERE)
	{
		bool bError = false;
		int nErrorSting = 7064;
		
		bool bMeracJoin = _pNetwork->MyCharacterInfo.zoneNo == 7 && _pNetwork->MyCharacterInfo.sbJoinFlagMerac != WCJF_NONE && _pNetwork->ga_bGuildWar == TRUE ? true : false;
		bool bDratanJoin = _pNetwork->MyCharacterInfo.zoneNo == 4 && _pNetwork->MyCharacterInfo.sbJoinFlagDratan != WCJF_NONE && pUIManager->GetSiegeWarfareNew()->GetWarState() == TRUE ? true : false;

		switch (castlewar)
		{
		case CItemData::eSIEGEAREA_ALL:	//	1	전체 공성에서만 사용 가능
		    {
		        if (bMeracJoin == false && bDratanJoin == false)
		        {
		            bError = true;
		            nErrorSting = 7064;
		        }
		    }
		    break;

		case CItemData::eSIEGEAREA_MERAC:	//	2	메라크 공성에서만 사용 가능
		    {
		        if (bMeracJoin == false)
		        {
		            bError = true;
		            nErrorSting = 7075;
		        }
		    }
		    break;

		case CItemData::eSIEGEAREA_DRATAN:	//	3	드라탄 공성에서만 사용 가능
		    {
		        if (bDratanJoin == false)
		        {
		            bError = true;
		            nErrorSting = 7076;
		        }
		    }
		    break;
		}
		
		if (bError == true)
		{
			ClientSystemMessage( _S( nErrorSting, "공성 지역에서만 사용 할 수 있습니다." ), SYSMSG_ERROR );		
			return;
		}
	}

	if( pItems->ItemData->GetType() == CItemData::ITEM_WEAPON || 
		pItems->ItemData->GetType() == CItemData::ITEM_SHIELD ||
		pItems->ItemData->GetType() == CItemData::ITEM_ACCESSORY )
	{
		if (_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING)
		{ // 비행 모드에서는 장비 착용을 할 수 없습니다.
			return;
		}
		// NOTE : 테섭에서 애완동물 탈것을 장착할수 없도록 하는 부분...
		// 애완동물 일때...
		if( (pItems->ItemData->GetSubType() == CItemData::ACCESSORY_PET || pItems->ItemData->GetSubType() == CItemData::ACCESSORY_WILDPET)
			&& pItems->Item_Wearing == -1 )
		{
			const INDEX iPetIndex = MySlotItem[tabId][inven_idx].Item_Plus;
			CNetworkLibrary::sPetInfo	TempPet;
			TempPet.lIndex				= iPetIndex;
			std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
				std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
			if( iter != _pNetwork->m_vectorPetList.end() )
			{
				INDEX iPetType;
				INDEX iPetAge;
				_pNetwork->CheckPetType( (*iter).sbPetTypeGrade, iPetType, iPetAge );				
				const BOOL bPetRide = PetInfo().IsRide(iPetType, iPetAge);
				
				// 타야하는 말일경우...
				if( bPetRide )
				{
					//ClientSystemMessage( _S( 294, "착용 할수 없는 아이템입니다." ), SYSMSG_ERROR );
					
					// 공격중이거나 스킬 사용중일때는 바꿀수 없음.
					if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsAttacking() )
					{
						ClientSystemMessage( _S(2579, "공격중일때는 애완동물을 탈 수 없습니다." ), SYSMSG_ERROR );		
						return;
					}
					
					if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsPolymophing() )
					{
						ClientSystemMessage( _S( 2574, "변신중일때는 애완동물을 탈 수 없습니다." ), SYSMSG_ERROR );		
						return;						
					}
					
					if( _pNetwork->MyCharacterInfo.nEvocationIndex > 0 )
					{
						ClientSystemMessage( _S(2580, "강신중일때는 애완동물을 탈 수 없습니다." ), SYSMSG_ERROR );		
						return;
					}
					
					// 공성중이고 공성지역 내에서는 애완동물을 소환할 수 없음
					if( _pNetwork->MyCharacterInfo.sbAttributePos & MATT_WAR && _pUISWDoc->IsWar() )
					{
						ClientSystemMessage( _S( 2583, "공성중에는 애완동물을 탈 수 없습니다." ), SYSMSG_ERROR );		
						return;
					}					
					
					if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
					{
						ClientSystemMessage( _S(2581, "스킬 사용중일때는 애완동물을 탈 수 없습니다." ), SYSMSG_ERROR );		
						return;
					}

					if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSitting() )
					{
						return;
					}

					pUIManager->SetCSFlagOn(CSF_PETRIDING);
				}
			}
		}

		if (pUIManager->IsCSFlagOnElapsed(CSF_ITEMWEARING, 5000)) // 아이템 착용 및 탈착을 시도 중입니다.
		{
			ClientSystemMessage( _S( 305, "장비를 착용할 수 없습니다." ), SYSMSG_ERROR );
			return;
		}

		//SET_ITEM_ADD				//[ttos_2009_5_22]: 세트 아이템 적용
		if (pItems->ItemData->GetFlag()&ITEM_FLAG_ORIGIN)
		{
			if (pItems->GetItemBelong() != -1 && !pItems->IsFlag(FLAG_ITEM_BELONG))
			{
				if(pUIManager->DoesMessageBoxExist(MSGCMD_ITEM_BELONG)) 
				{
					return;
				}
				CUIMsgBox_Info	MsgBoxInfo;
				CTString	strMessage[2];
				strMessage[0] = _S(4102,"경고!");
				strMessage[1] = _S(4657, "아이템을 착용하면 귀속되어 거래와 창고보관이 불가능하게 됩니다. 착용하시겠습니까?");
				MsgBoxInfo.SetMsgBoxInfo(strMessage[0], UMBS_OKCANCEL, UI_NONE, MSGCMD_ITEM_BELONG	);
				MsgBoxInfo.AddString(strMessage[1]);
				if (pUIManager->CreateMessageBox(MsgBoxInfo))
					pUIManager->GetMessageBox(MSGCMD_ITEM_BELONG)->SetInvenUseItemInfo( tabId, inven_idx );
				return;
			}
		}

		// 무기 일때...
		if( pItems->ItemData->GetType() == CItemData::ITEM_WEAPON )
		{
			// 공격중이거나 스킬 사용중일때는 바꿀수 없음.
			if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsAttacking() )
			{
				ClientSystemMessage( _S( 1324, "공격중일때는 무기를 교체할 수 없습니다." ), SYSMSG_ERROR );		
				return;
			}
			
			if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
			{
				ClientSystemMessage( _S( 1325, "스킬 사용중일때는 무기를 교체할 수 없습니다." ), SYSMSG_ERROR );		
				return;
			}
		}

		SBYTE sbType = (SBYTE)MSG_ITEM_WEAR;	// 아이템 메시지 타입
		SBYTE sbWear = -1;

		// Item Flag 확인
		if( pItems->ItemData->GetFlag() & ITEM_FLAG_COSTUME2 ) { 
			// 코스튬2 시스템 아이템
			if( pItems->ItemData->GetSubType() == CItemData::ITEM_SHIELD_ONE_SUIT )
				sbType = (SBYTE)MSG_ITEM_WEAR_COSTUME_SUIT;
			else
				sbType = (SBYTE)MSG_ITEM_WEAR_COSTUME;
		}

		if( pItems->ItemData->GetType() == CItemData::ITEM_ACCESSORY && 
			!(pItems->ItemData->GetSubType() == CItemData::ACCESSORY_PET || pItems->ItemData->GetSubType() == CItemData::ACCESSORY_WILDPET))
		{
			// 인벤토리의 빈자리를 확인합니다.
			for(int iWearPos = WEAR_ACCESSORY1; iWearPos <= WEAR_ACCESSORY3; ++iWearPos)
			{
				if(!_pNetwork->MyWearItem[iWearPos].IsEmptyItem() == FALSE)
				{
					sbWear = iWearPos;
					break;
				}
			}
		}
		else
		{
			sbWear = pItems->ItemData->GetWearingPosition();
		}
		// Send msg
		if( SendItemWearingMSG( sbType, sbWear, pItems->Item_Tab, pItems->InvenIndex, 0 ) ) 
		{
			pUIManager->SetCSFlagOnElapsed(CSF_ITEMWEARING, _pTimer->GetLerpedCurrentTick()*1000);
		}
		//}
	}
	else if(pItems->ItemData->GetType() == CItemData::ITEM_ONCEUSE)
	{
		switch( pItems->ItemData->GetSubType() )
		{
	
		case CItemData::ITEM_SUB_WARP:			// 이동
			{
				if( pItems->ItemData->GetWarpType() == 0 )			// 리턴 스크롤
				{
					if( pUIManager->IsCSFlagOn( CSF_CANNOT_TELEPORT_MASK ) )
					{
						pUIManager->GetTeleport()->ShowTeleportError();
						return;
					}
				}
				else if( pItems->ItemData->GetWarpType() == 1 )	// 메모리 스크롤
				{
					// [KH_070315] 프리미엄 메모리스크롤 관련 추가
					if(pItems->Item_Index == PRIMIUM_TELEPORT)
						pUIManager->GetTeleportPrimium()->OpenTeleport();
					else
						pUIManager->GetTeleport()->OpenTeleport();
					return;
				}			
			}
			break;
		
		// 이기환 수정 시작 ( 12. 6 ) : 생산 시스템
		case CItemData::ITEM_SUB_PROCESS_DOC:	// 가공 문서
			{
				if ( pUIManager->GetProduct()->IsVisible() || pUIManager->GetMix()->IsVisible() ||
					pUIManager->GetCompound()->IsVisible() )
				{
					return;
				}

				pUIManager->GetProcess()->OpenProcess( pItems->Item_Index, tabId, inven_idx );
				return;
			}
			break;
		case CItemData::ITEM_SUB_MAKE_POTION_DOC:	// 가공 문서
		case CItemData::ITEM_SUB_MAKE_TYPE_DOC:	// 제작 문서
			{
				if ( pUIManager->GetProcess()->IsVisible() || pUIManager->GetMix()->IsVisible() ||
					pUIManager->GetCompound()->IsVisible() )
				{
					return;
				}

				pUIManager->GetProduct()->OpenProduct( pItems->Item_Index, tabId, inven_idx );
				return;
			}
			break;
		// 이기환 수정 끝 ( 12. 10 )
		// Date : 2005-01-12,   By Lee Ki-hwan
		case CItemData::ITEM_SUB_BOX : // 상자 ( 조합 )
			{
				switch ( pItems->ItemData->GetBoxType() )
				{
				case CItemData::BOX_REMAKE: // 아이템 조합 ( 재활의 상자 )
					if ( pUIManager->GetProcess()->IsVisible() ||
						pUIManager->GetProduct()->IsVisible() ||
						pUIManager->GetCompound()->IsVisible() )
					{
						return;
					}
					
					pUIManager->GetMix()->OpenMix( pItems->Item_Index, tabId, inven_idx );
					return;
					
				case CItemData::BOX_ARCANE:
					if ( pUIManager->GetProcess()->IsVisible() || 
						pUIManager->GetProduct()->IsVisible() ||
						pUIManager->GetMix()->IsVisible() )
					{
						return;
					}

					pUIManager->GetCompound()->OpenCompound( pItems->Item_Index, tabId, inven_idx );
					return;
				
				
				}
			}
			break;
		// Date : 2005-01-12,   By Lee Ki-hwan
		case  CItemData::ITEM_SUB_CHANGE_DOC:	
			{
			
				if(!ga_srvServer.srv_apltPlayers[0].plt_penPlayerEntity->CheckChangeCondition(pItems->ItemData->GetNum2(), pItems->ItemData->GetNum3()))
				{
					return;
				}
				
				// 변신 스크롤 사용
				_pNetwork->MyCharacterInfo.eMorphStatus		= MyChaInfo::eMORPH_TRANSFORMATION_BEGIN;
			}
			break;
			// 퀘스트 스크롤.
		case CItemData::ITEM_SUB_QUEST_SCROLL:
			{
				const int iQuestIndex = pItems->ItemData->GetNum0();
				if( CQuestSystem::Instance().CanIDoQuest( iQuestIndex ) )
				{
					if( CQuestSystem::Instance().GetDynamicDataByQuestIndex( iQuestIndex ) == NULL )
					{						
						pUIManager->GetInventory()->Lock( TRUE, TRUE, LOCK_QUEST );
						pUIManager->GetQuestAccept()->open( iQuestIndex, pItems );
					}
					else
					{
						CTString strSysMessage;
						strSysMessage.PrintF( _S( 1744, "이미 수행중인 퀘스트입니다."  ) );		
						_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );
					}
				}
				else
				{
					CTString strSysMessage;
					strSysMessage.PrintF( _S( 1745, "조건이 맞지 않습니다."  ) );		
					_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );
				}
				return;
			}
			break;

		case CItemData::ITEM_SUB_TARGET:
			{
				ObjInfo* pInfo = ObjInfo::getSingleton();

				if( pInfo->IsTargetActive(eTARGET) == FALSE )
						return;
				
				LONG iMobClientIndex = pInfo->GetTargetServerIdx(eTARGET);
				SBYTE cTargetType = pInfo->GetTargetType(eTARGET);

				if( !pUIManager->GetQuickSlot()->StartSkillDelay( pItems->ItemData->GetNum0() ) )
				{
					if( !pUIManager->GetInventory()->StartSkillDelay( pItems->ItemData->GetNum0() ) )
						return;
				}
				
				SendtargetItemUse(pItems->Item_Tab, pItems->InvenIndex, pItems->Item_UniIndex, cTargetType, iMobClientIndex);
				return;
			}
			break;

		case CItemData::ITEM_SUB_EXPRESS_REMOTE:
			{
			}
			break;

		default:
			break;
		}

		SendItemUse(pItems->Item_Tab, pItems->InvenIndex, pItems->Item_UniIndex, 0);		
	}
	else if ( pItems->ItemData->GetType() == CItemData::ITEM_POTION )
	{
		CItemData*	pItemData = _pNetwork->GetItemData( pItems->Item_Index );
		
		//2012/11/13 jeil 부스터 아이탬 중복 사용 체크 	
		if(_pUIBuff->IsBuff(9189) && pItemData->GetItemIndex() == 9189)
		{	
			CTString strSysMessage;
			strSysMessage.PrintF( _S(5816,"친화도 부스터는 중복 사용이 불가능 합니다.") );		
			_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );
			return;
		}		
		if( !pUIManager->GetQuickSlot()->StartSkillDelay( pItems->ItemData->GetNum0() ) )
		{
			if( !pUIManager->GetInventory()->StartSkillDelay( pItems->ItemData->GetNum0() ) )
				return;
		}
		
		// item 중복 체크
		if ( pItemData->GetFlag() & ITEM_FLAG_DUPLICATION && _pUIBuff->IsBuff( pItemData->GetItemIndex() ) )
		{
			if ( pUIManager->DoesMessageBoxExist( MSGCMD_ITEM_DUPLICATION_CHECK ) )
			{
				pUIManager->CloseMessageBox( MSGCMD_ITEM_DUPLICATION_CHECK );
			}
			
			CTString strMessage, strTitle;
			CUIMsgBox_Info msgBoxInfo;						
			strTitle.PrintF(_S( 191, "확인" ));
			msgBoxInfo.SetMsgBoxInfo( strTitle, UMBS_OKCANCEL, UI_NONE ,MSGCMD_ITEM_DUPLICATION_CHECK);
			strMessage.PrintF( _S( 6076, "이미 %s와 동일한 효과가 적용되어 있습니다. 정말로 사용 하시겠습니까?"), _pNetwork->GetItemName( pItems->Item_Index ) );
			msgBoxInfo.AddString(strMessage);

			if ( pUIManager )
			{
				pUIManager->CreateMessageBox( msgBoxInfo );
				pUIManager->GetMessageBox(MSGCMD_ITEM_DUPLICATION_CHECK)->SetInvenUseItemInfo( tabId, inven_idx );
				return;
			}
		}
		SendItemUse(pItems->Item_Tab, pItems->InvenIndex, pItems->Item_UniIndex, 0);
	}
	else if (pItems->ItemData->GetType() == CItemData::ITEM_ETC) // 기타 아이템
	{
		if (pItems->ItemData->GetSubType() == CItemData::ITEM_ETC_PRODUCT && _pNetwork->MyWearItem[WEAR_PET].IsEmptyItem() == FALSE)
		{ // 생산물
			SendItemUse(pItems->Item_Tab, pItems->InvenIndex, pItems->Item_UniIndex, 0);
		}
		else if (pItems->ItemData->GetFlag() & ITEM_FLAG_TRIGGER || pItems->ItemData->GetSubType() == CItemData::ITEM_ETC_SKILL) // [2010/10/20 : Sora] 서브타입을 비트로 체크하던 부분을 수정
		{ // [090810: selo] 트리거 아이템인 경우 , // 스킬 습득 일 경우
			SendItemUse(pItems->Item_Tab, pItems->InvenIndex, pItems->Item_UniIndex, 0);
		}
		else if ( pItems->ItemData->GetSubType() == CItemData::ITEM_ETC_MONSTER_MERCENARY_CARD )	// [2010/10/20 : Sora] 몬스터 용병 카드
		{
			if( !pUIManager->GetQuickSlot()->StartSkillDelay( pItems->ItemData->GetNum0() ) )
			{
				if( !pUIManager->GetInventory()->StartSkillDelay( pItems->ItemData->GetNum0() ) )
					return;
			}

			SendItemUse(pItems->Item_Tab, pItems->InvenIndex, pItems->Item_UniIndex, 0);
		}
	}

	//2012/11/09 jeil 친화도 부스터 아이탬 체크 하기 
}

/****   WareHouse *********************************************************/
//-----------------------------------------------------------------------------
// Purpose: SendWareHouseChangePassword
// Input  :
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendWareHouseChangePassword(const CTString& strOld, const CTString& strNew)
{	
	CNetworkMessage nmWareHouse((UBYTE)MSG_STASH);
	nmWareHouse << (UBYTE)MSG_STASH_CHANGE_PASSWORD_REQ;
	nmWareHouse << strOld;
	nmWareHouse << strNew;
	SendToServerNew(nmWareHouse);	
}

//-----------------------------------------------------------------------------
// Purpose: SendWareHouseIsSetPassword
// Input  :
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendWareHouseIsSetPassword()
{
	CNetworkMessage nmWareHouse((UBYTE)MSG_STASH);
	nmWareHouse << (UBYTE)MSG_STASH_ISSETPASSWORD;	
	SendToServerNew(nmWareHouse);
}

//-----------------------------------------------------------------------------
// Purpose: SendWareHouseSeal
// Input  : 창고 잠금 요청.
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendWareHouseSeal()
{
	CNetworkMessage nmWareHouse((UBYTE)MSG_STASH);
	nmWareHouse << (UBYTE)MSG_STASH_SEAL;	
	SendToServerNew(nmWareHouse);
}

//-----------------------------------------------------------------------------
// Purpose: SendWareHouseSetupPassWord
// Input  :
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendWareHouseSetupPassWord( const CTString& strPW, const CTString& strOldPW )
{
#ifdef	STASH_PASSWORD
	CNetworkMessage nmMessage;
	RequestClient::doStashChangePassword* packet = reinterpret_cast<RequestClient::doStashChangePassword*>(nmMessage.nm_pubMessage);
	packet->type = MSG_STASH;
	packet->subType = MSG_STASH_CHANGE_PASSWORD;
	int nSize = strOldPW.Length();
	if (nSize > 0)
	{
		memset(packet->old_password, 0, MAX_STASH_PASSWORD_LENGTH+1);
		nSize = strOldPW.Length();
		if (nSize > MAX_STASH_PASSWORD_LENGTH)
			nSize = MAX_STASH_PASSWORD_LENGTH;
		memcpy(packet->old_password, strOldPW.str_String, nSize);
	}

	memset(packet->new_password, 0, MAX_STASH_PASSWORD_LENGTH+1);
	nSize = strPW.Length();
	if (nSize > MAX_STASH_PASSWORD_LENGTH)
		nSize = MAX_STASH_PASSWORD_LENGTH;
	memcpy(packet->new_password, strPW.str_String, nSize);
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew( nmMessage );
#endif	// STASH_PASSWORD
}

//-----------------------------------------------------------------------------
// Purpose: SendWareHouseDeletePassWord
// Input  :
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendWareHouseDeletePassWord( const CTString& strID )
{
#ifdef	STASH_PASSWORD
	CNetworkMessage nmMessage;
	RequestClient::doStashDeletePassword* packet = reinterpret_cast<RequestClient::doStashDeletePassword*>(nmMessage.nm_pubMessage);
	packet->type = MSG_STASH;
	packet->subType = MSG_STASH_DELETE_PASSWORD;

	memset(packet->identity, 0, A_CID_LENGTH+1);
	int nSize = strID.Length();
	if (nSize > A_CID_LENGTH)
		nSize = A_CID_LENGTH;
	memcpy(packet->identity, strID.str_String, nSize);

	nmMessage.setSize( sizeof(*packet) );
	SendToServerNew( nmMessage );
#endif	// STASH_PASSWORD
}

/****   Guild   ***********************************************************/
// ----------------------------------------------------------------------------
// Name : GuildJoin()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::GuildJoin( SLONG slGuildIndex, SLONG slSrcIndex, SLONG slDestIndex )			// 가입 신청
{
	CNetworkMessage nmGuild((UBYTE)MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_REGIST_REQ;
	nmGuild << slGuildIndex;
	nmGuild << slSrcIndex;
	nmGuild << slDestIndex;
	SendToServerNew(nmGuild);
}

// ----------------------------------------------------------------------------
// Name : GuildQuit()
// Desc : 길드 탈퇴
// ----------------------------------------------------------------------------
void CNetworkLibrary::GuildQuit( )
{
	CNetworkMessage nmGuild((UBYTE)MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_OUT_REQ;
	nmGuild << (LONG)_pNetwork->MyCharacterInfo.lGuildIndex;
	SendToServerNew(nmGuild);
}

// ----------------------------------------------------------------------------
// Name : GuildCreate()
// Desc : 길드 생성
// ----------------------------------------------------------------------------
void CNetworkLibrary::GuildCreate( const CTString &strGuildName )		// 길드 생성
{
	CNetworkMessage nmGuild((UBYTE)MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_CREATE;
	//nmGuild << _pNetwork->MyCharacterInfo.index;
	nmGuild << strGuildName;
	SendToServerNew(nmGuild);
}

// ----------------------------------------------------------------------------
// Name : GuildUpgrade()
// Desc : 길드 승급
// ----------------------------------------------------------------------------
void CNetworkLibrary::GuildUpgrade()							// 길드 승급
{
	CNetworkMessage nmGuild((UBYTE)MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_LEVELUP;	
	SendToServerNew(nmGuild);
}

// ----------------------------------------------------------------------------
// Name : GuildDestroy()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::GuildDestroy()							// 길드 해체
{
	// 길드 해체 메시지 보내기.
	CNetworkMessage nmGuild((UBYTE)MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_BREAKUP;	
	SendToServerNew(nmGuild);
}

// ----------------------------------------------------------------------------
// Name : GuildChangeBoss()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::GuildChangeBoss( SLONG slDestIndex )		// 단장 이임
{
	// 길드 해체 메시지 보내기.
	CNetworkMessage nmGuild((UBYTE)MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_CHANGE_BOSS;	
	nmGuild << (LONG)_pNetwork->MyCharacterInfo.lGuildIndex;
	nmGuild << _pNetwork->MyCharacterInfo.index;
	nmGuild << slDestIndex;
	SendToServerNew(nmGuild);
}

// ----------------------------------------------------------------------------
// Name : GuildAddViceBoss()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::GuildAddViceBoss( SLONG slDestIndex )		// 부단장 임명
{
	// 길드 해체 메시지 보내기.
	CNetworkMessage nmGuild((UBYTE)MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_APPOINT_OFFICER;	
	nmGuild << (LONG)_pNetwork->MyCharacterInfo.lGuildIndex;
	nmGuild << slDestIndex;
	SendToServerNew(nmGuild);
}

// ----------------------------------------------------------------------------
// Name : GuildDelViceBoss()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::GuildDelViceBoss( SLONG slDestIndex )		// 부단장 해임
{
	// 길드 해체 메시지 보내기.
	CNetworkMessage nmGuild((UBYTE)MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_FIRE_OFFICER;	
	nmGuild << (LONG)_pNetwork->MyCharacterInfo.lGuildIndex;
	nmGuild << slDestIndex;
	SendToServerNew(nmGuild);
}

// ----------------------------------------------------------------------------
// Name : GuildMemberFire()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::GuildMemberFire( SLONG slDestIndex )		// 멤버 퇴출
{
	// 길드 해체 메시지 보내기.
	CNetworkMessage nmGuild((UBYTE)MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_KICK;	
	nmGuild << (LONG)_pNetwork->MyCharacterInfo.lGuildIndex;
	nmGuild << slDestIndex;
	SendToServerNew(nmGuild);
}

// ----------------------------------------------------------------------------
// Name : GuildApplicantAccept()
// Desc : 요청자에 대해서 가입 승인하기.
// ----------------------------------------------------------------------------
void CNetworkLibrary::GuildApplicantAccept( SLONG slDestIndex )	// 가입 승인
{
	CNetworkMessage nmGuild((UBYTE)MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_REGIST_ALLOW;
	nmGuild << (LONG)_pNetwork->MyCharacterInfo.lGuildIndex;
	nmGuild << slDestIndex;
	SendToServerNew(nmGuild);
}

// ----------------------------------------------------------------------------
// Name : GuildApplicantReject()
// Desc : 요청자에 대해서 가입 거부하기.
// ----------------------------------------------------------------------------
void CNetworkLibrary::GuildApplicantReject( SBYTE sbWhoCancel )	// 가입 거부
{
	// 길드 해체 메시지 보내기.
	CNetworkMessage nmGuild((UBYTE)MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_REGIST_CANCEL;	
	nmGuild << sbWhoCancel;
	SendToServerNew(nmGuild);
}


// Guild Battle

//------------------------------------------------------------------------------
// CNetworkLibrary::GBReq
// Explain: 길드 전투 요청
// Date : 2005-03-18(오후 3:16:42) Lee Ki-hwan
// Edit History 
// Date : 2005-04-15(오후 7:49:53), By Lee Ki-hwan : SLONG nTime 추가
//------------------------------------------------------------------------------
void CNetworkLibrary::GBReq( SLONG nCharIndex, SLONG nPrize, SLONG nTime )
{
	CNetworkMessage nmGuild((UBYTE)MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_BATTLE_REQ_REQ;	
	nmGuild << nCharIndex;
	nmGuild << nPrize;
	nmGuild << nTime;

	SendToServerNew(nmGuild);

}


//------------------------------------------------------------------------------
// CNetworkLibrary::GBReqReject
// Explain: 길드 전투 요청 거절
// Date : 2005-03-18(오후 3:16:45) Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::GBReqReject()
{
	CNetworkMessage nmGuild((UBYTE)MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_BATTLE_REQ_REJECT;	
	SendToServerNew(nmGuild);
}


//------------------------------------------------------------------------------
// CNetworkLibrary::GBReqAccept
// Explain: 길드 전투 수락
// Date : 2005-03-18(오후 3:16:47) Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::GBReqAccept()
{
	CNetworkMessage nmGuild((UBYTE)MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_BATTLE_REQ_ACCEPT;	
	SendToServerNew(nmGuild);
}


//------------------------------------------------------------------------------
// CNetworkLibrary::GBStopReqSend
// Explain: 길드 전투 중지 요청 
// Date : 2005-03-18(오후 3:16:50) Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::GBStopReq( SLONG nCharIndex )
{
	CNetworkMessage nmGuild((UBYTE)MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_BATTLE_STOP_REQ;
	nmGuild << nCharIndex;
	SendToServerNew(nmGuild);
}


//------------------------------------------------------------------------------
// CNetworkLibrary::GBStopReqReject
// Explain:  길드 전투 중지 요청 거절 
// Date : 2005-03-18(오후 3:16:52) Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::GBStopReqReject()	
{
	CNetworkMessage nmGuild((UBYTE)MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_BATTLE_STOP_REJECT;	
	SendToServerNew(nmGuild);
}


//------------------------------------------------------------------------------
// CNetworkLibrary::GBStopReqAccept
// Explain:  길드 전투 중지 요청 수락 
// Date : 2005-03-18(오후 3:30:03) Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::GBStopReqAccept()	
{
	CNetworkMessage nmGuild((UBYTE)MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_BATTLE_STOP_ACCEPT;	
	SendToServerNew(nmGuild);
}

/****   Teach      ***********************************************************/
// ----------------------------------------------------------------------------
// Name : TeachTeacherRegister()
// Desc :
// ----------------------------------------------------------------------------
void CNetworkLibrary::TeachTeacherRegister(SBYTE sbStartPlayTime, SBYTE sbEndPlayTime)			// 선생 목록에 추가
{
	CNetworkMessage nmTeach((UBYTE)MSG_TEACH);
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_LIST;	
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_LIST_UP;
	nmTeach << (SBYTE)sbStartPlayTime;
	nmTeach << (SBYTE)sbEndPlayTime;
	SendToServerNew(nmTeach);
}

// ----------------------------------------------------------------------------
// Name : TeachTeacherCancelRegister()
// Desc :
// ----------------------------------------------------------------------------
void CNetworkLibrary::TeachTeacherCancelRegister()		// 선생 목록에서 빼줘
{
	CNetworkMessage nmTeach((UBYTE)MSG_TEACH);
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_LIST;	
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_LIST_DN;
	SendToServerNew(nmTeach);
}

// ----------------------------------------------------------------------------
// Name : TeachRefreshTeacherList()
// Desc : 후견인 목록 갱신
// ----------------------------------------------------------------------------
void CNetworkLibrary::TeachRefreshTeacherList()
{
	CNetworkMessage nmTeach((UBYTE)MSG_TEACH);
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_LIST;	
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_LIST_SHOW;
	SendToServerNew(nmTeach);
}

// ----------------------------------------------------------------------------
// Name : TeachTeacherRequest()
// Desc : 후견인 신청
// ----------------------------------------------------------------------------
void CNetworkLibrary::TeachTeacherRequest( SLONG slCharIndex, CTString& strName )
{
	CNetworkMessage nmTeach((UBYTE)MSG_TEACH);
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_REQ;	
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_REQ_REQ;	
	nmTeach << (LONG)slCharIndex;
	nmTeach << strName;
	SendToServerNew(nmTeach);
}

// ----------------------------------------------------------------------------
// Name : TeachTeacherReject()
// Desc : 후견인 신청 취소.
// ----------------------------------------------------------------------------
void CNetworkLibrary::TeachTeacherReject( BOOL bStudent, SLONG slCharIndex, CTString& strName )
{
	// 선생 요청 거절	: char_type(uc) reject_charindex(n) reject_charname(str)
	CNetworkMessage nmTeach((UBYTE)MSG_TEACH);
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_REQ;	
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_REQ_REJECT;

	/*
	if( !bStudent )
		nmTeach << (UBYTE)MSG_TEACH_TEACHER_TYPE;
	else
		nmTeach << (UBYTE)MSG_TEACH_STUDENT_TYPE;
		*/
	nmTeach << slCharIndex;
	nmTeach << strName;
	SendToServerNew(nmTeach);
}

// ----------------------------------------------------------------------------
// Name : TeachTeacherAccept()
// Desc : 후견인 신청 확인.
// ----------------------------------------------------------------------------
void CNetworkLibrary::TeachTeacherAccept( SLONG slTeacherIndex, CTString& strTeacherName, SLONG slStudentIndex, CTString& strStudentName )
{
	// 선생 허락		: teacher_index(n) teacher_name(str) student_index(n) student_name(str)
	CNetworkMessage nmTeach((UBYTE)MSG_TEACH);
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_REQ;
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_REQ_ACCEPT;
	nmTeach << slTeacherIndex;
	nmTeach << strTeacherName;
	nmTeach << slStudentIndex;
	nmTeach << strStudentName;
	SendToServerNew(nmTeach);
}

// ----------------------------------------------------------------------------
// Name : TeachTeacherGiveUp()
// Desc :
// ----------------------------------------------------------------------------
void CNetworkLibrary::TeachTeacherGiveUp( SLONG slTeacherIndex, CTString& strTeacherName, SLONG slStudentIndex, CTString& strStudentName )
{
	// 선생 허락		: teacher_index(n) teacher_name(str) student_index(n) student_name(str)
	CNetworkMessage nmTeach((UBYTE)MSG_TEACH);
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_REQ;
	nmTeach << (UBYTE)MSG_TEACH_TEACHER_GIVEUP;
	nmTeach << slTeacherIndex;
	nmTeach << strTeacherName;
	nmTeach << slStudentIndex;
	nmTeach << strStudentName;
	SendToServerNew(nmTeach);
}

// ----------------------------------------------------------------------------
// Name : ChangeJobReq()
// Desc : 전직 요청
// ----------------------------------------------------------------------------
void CNetworkLibrary::ChangeJobReq( SBYTE sbJob, int nNpcVirIdx )
{
	CNetworkMessage nmChangeJob;
	RequestClient::changeJob* packet = reinterpret_cast<RequestClient::changeJob*>(nmChangeJob.nm_pubMessage);
	packet->type = MSG_CHANGEJOB;
	packet->subType = MSG_CHANGEJOB_REQ;
	packet->npcIndex = nNpcVirIdx;
	packet->job = sbJob;
	nmChangeJob.setSize( sizeof(*packet) );

	SendToServerNew( nmChangeJob );
}

// ----------------------------------------------------------------------------
// Name : ChangeJobGiveUp()
// Desc : 직업 포기
// ----------------------------------------------------------------------------
void CNetworkLibrary::ChangeJobGiveUp(int nNpcVirIdx)
{
	CNetworkMessage nmChangeJob;
	RequestClient::changeJobReset* packet = reinterpret_cast<RequestClient::changeJobReset*>(nmChangeJob.nm_pubMessage);
	packet->type = MSG_CHANGEJOB;
	packet->subType = MSG_CHANGEJOB_RESET_REQ;
	packet->npcIndex = nNpcVirIdx;
	nmChangeJob.setSize( sizeof(*packet) );

	SendToServerNew( nmChangeJob );
}

// ----------------------------------------------------------------------------
// Name : ChangeWeaponReq()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::ChangeWeaponReq( SWORD nTab, SWORD inven_idx, LONG lItemVirIndex, LONG lTradeIndex, LONG lChangeType, LONG lTokenVirIndex, LONG lTokenCount )
{
	CNetworkMessage nmMessage;
	RequestClient::doItemChangeWeapon* packet = reinterpret_cast<RequestClient::doItemChangeWeapon*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_CHANGEWEAPON_REQ;
	packet->tab = nTab;
	packet->invenIndex = inven_idx;
	packet->virtualIndex = lItemVirIndex;
	packet->exchangeDBIndex = lTradeIndex;
	packet->changeType = lChangeType;
	packet->goldTokenItemVIndex = lTokenVirIndex;
	packet->goldTokenCount = lTokenCount;
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew( nmMessage );
}

// ----------------------------------------------------------------------------
// Name : ChangeWeaponEventReq()
// Desc : 2판 4판 무기 교체 이벤트
// ----------------------------------------------------------------------------
void CNetworkLibrary::ChangeWeaponEventReq( LONG lItemIndex, LONG lChangeType )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->GetChangeWeapon()->GetCashItem()) {
		
		int tv_tab, inven_idx;
		pUIManager->GetInventory()->GetUseItemSlotInfo(tv_tab, inven_idx);
		
		CItems* pItems = &MySlotItem[tv_tab][inven_idx];

		if(pItems->Item_Index ==-1) //비어있는 슬롯이다.
			return;

		CNetworkMessage nmMessage;
		RequestClient::doItemUse* packet = reinterpret_cast<RequestClient::doItemUse*>(nmMessage.nm_pubMessage);
		packet->type = MSG_ITEM;
		packet->subType = MSG_ITEM_USE;
		packet->tab = pItems->Item_Tab;
		packet->invenIndex = pItems->InvenIndex;
		packet->virtualIndex = pItems->Item_UniIndex;
		packet->extra_1 = lChangeType;
		nmMessage.setSize( sizeof(*packet) );

		SendToServerNew( nmMessage );
	
				
	} else {
		// MSG_EVENT_CHANGE_WITHOUT_OPTION,			// 2판4판 리뉴얼 무기 교체 이벤트 : old_itemindex(n) new_subtype(n)
		CNetworkMessage nmChangeWeapon((UBYTE)MSG_EVENT);
		nmChangeWeapon << (UBYTE)MSG_EVENT_CHANGE_WITHOUT_OPTION;			
		nmChangeWeapon << lItemIndex;
		nmChangeWeapon << lChangeType;
		SendToServerNew(nmChangeWeapon);
	}
}

// ----------------------------------------------------------------------------
// Name : StatReset()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::StatReset( int iStr, int iDex, int iInt, int iCon )
{
	LONG lStr = iStr;
	LONG lDex = iDex;
	LONG lInt = iInt;
	LONG lCon = iCon;

	// 포인트 초기화
	CNetworkMessage nmStat((UBYTE)MSG_STATPOINT);
	nmStat << (UBYTE)MSG_STATPOINT_RESET;
	nmStat << lStr;
	nmStat << lDex;
	nmStat << lInt;
	nmStat << lCon;
	SendToServerNew( nmStat );
}

// ----------------------------------------------------------------------------
// Name : SendChangeWeaponEvent()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendChangeWeaponEvent()
{
	if (_pNetwork->MyWearItem[WEAR_WEAPON].IsEmptyItem() == FALSE)
	{
		CTString strSysMessage;
		strSysMessage.PrintF( _S( 1326, "교체할 무기를 장착하여 주십시오." ) );		
		_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );
		return;
	}
	
	const int iWeaponType = _pNetwork->MyWearItem[WEAR_WEAPON].ItemData->GetSubType();
	
	// 생산도구를 장착하구 있을때...
	if(iWeaponType == CItemData::ITEM_WEAPON_MINING || 
		iWeaponType == CItemData::ITEM_WEAPON_GATHERING || 
		iWeaponType == CItemData::ITEM_WEAPON_CHARGE)
	{
		CTString strSysMessage;
		strSysMessage.PrintF( _S( 1327, "생산 도구는 교체할 수 없습니다." ) );		
		_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );
		return;
	}
		
	CNetworkMessage nmChangeWeapon((UBYTE)MSG_EVENT);
	nmChangeWeapon << (UBYTE)MSG_EVENT_CHANGEWEAPON;
	SendToServerNew( nmChangeWeapon );
}

// ----------------------------------------------------------------------------
// Name : SendMoonStoneStartReq()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendMoonStoneStartReq()
{
	CNetworkMessage	nmMoonStone( (UBYTE)MSG_EVENT );

	nmMoonStone << (SBYTE)MSG_EVENT_NEW_MOONSTONE;
	nmMoonStone << (SBYTE)MSG_EVENT_NEW_MOONSTONE_START_REQ;
	SendToServerNew( nmMoonStone );
}

// ----------------------------------------------------------------------------
// Name : SendMoonStoneTryReq()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendMoonStoneTryReq()
{
	CNetworkMessage	nmMoonStone( (UBYTE)MSG_EVENT );

	nmMoonStone << (SBYTE)MSG_EVENT_NEW_MOONSTONE;
	nmMoonStone << (SBYTE)MSG_EVENT_NEW_MOONSTONE_TRY_REQ;
	nmMoonStone << (ULONG)CUIManager::getSingleton()->GetGamble()->GetUsedMoonStoneIndex();
	SendToServerNew( nmMoonStone );
}

// ----------------------------------------------------------------------------
// Name : SendMoonStoneResultReq()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendMoonStoneResultReq()
{
	CNetworkMessage	nmMoonStone( (UBYTE)MSG_EVENT );

	nmMoonStone << (SBYTE)MSG_EVENT_NEW_MOONSTONE;
	nmMoonStone << (SBYTE)MSG_EVENT_NEW_MOONSTONE_RESULT_REQ;
	SendToServerNew( nmMoonStone );
}

// [071122: Su-won] NEW_MOONSTONE
void CNetworkLibrary::SendMoonStoneMix(INDEX iMoonStone, INDEX iMoonStoneBox)
{
	CNetworkMessage	nmMoonStone( (UBYTE)MSG_EVENT );

	nmMoonStone << (SBYTE)MSG_EVENT_NEW_MOONSTONE;
	nmMoonStone << (SBYTE)MSG_EVENT_NEW_MOONSTONE_MIX_REQ;
	nmMoonStone << iMoonStone;
	nmMoonStone << iMoonStoneBox;

	SendToServerNew( nmMoonStone );
}

//////////////////////////////////////////////////////////////////////////
// Name : SendMoonStoneTryReq()	
// Desc :
// Date : [6/20/2006] , Wooss
//////////////////////////////////////////////////////////////////////////
void CNetworkLibrary::SendCashMoonStoneReward()
{
	CNetworkMessage nmItem((UBYTE)MSG_EXTEND);
	nmItem	<< (ULONG)MSG_EX_CASHITEM;
	nmItem	<< (UBYTE)MSG_EX_CASHITEM_MOONSTONE_STOP;
	SendToServerNew(nmItem);
}

// ypark |<--
/****   Exchange   ***********************************************************/
void CNetworkLibrary::ExchangeReq_Req( SLONG slDestIndex, CTString &strDestName )
{
	// Send network message
	CNetworkMessage	nm( (UBYTE)MSG_EXCHANGE );
	nm << (SBYTE)MSG_EXCHANGE_REQ;
	nm << (SBYTE)MSG_EXCHANGE_REQ_REQ;
	nm << MyCharacterInfo.index;
	nm << MyCharacterInfo.name;
	nm << slDestIndex;
	nm << strDestName;

	SendToServerNew( nm );
}

void CNetworkLibrary::ExchangeReq_Rep()
{
	// Send network message
	CNetworkMessage	nm( (UBYTE)MSG_EXCHANGE );
	nm << (SBYTE)MSG_EXCHANGE_REQ;
	nm << (SBYTE)MSG_EXCHANGE_REQ_REP;

	SendToServerNew( nm );
}

void CNetworkLibrary::ExchangeReq_Rej()
{
	// Send network message
	CNetworkMessage	nm( (UBYTE)MSG_EXCHANGE );
	nm << (SBYTE)MSG_EXCHANGE_REQ;
	nm << (SBYTE)MSG_EXCHANGE_REQ_REJECT;

	SendToServerNew( nm );
}

void CNetworkLibrary::ExchangeReq_Ready()
{
	// Send network message
	CNetworkMessage	nm( (UBYTE)MSG_EXCHANGE );
	nm << (SBYTE)MSG_EXCHANGE_REQ;
	nm << (SBYTE)MSG_EXCHANGE_REQ_READY;

	SendToServerNew( nm );
}

void CNetworkLibrary::ExchangeReq_Ok()
{
	// Send network message
	CNetworkMessage	nm( (UBYTE)MSG_EXCHANGE );
	nm << (SBYTE)MSG_EXCHANGE_REQ;
	nm << (SBYTE)MSG_EXCHANGE_REQ_OK;

	SendToServerNew( nm );
}

void CNetworkLibrary::ExchangeItem_Add( int nUniIndex, SQUAD llCount )
{
	// Send network message
	CNetworkMessage	nm( (UBYTE)MSG_EXCHANGE );
	nm << (SBYTE)MSG_EXCHANGE_ITEM;
	nm << (SBYTE)MSG_EXCHANGE_ITEM_ADD;
	nm << (SLONG)nUniIndex;
	nm << llCount;

	SendToServerNew( nm );
}

void CNetworkLibrary::ExchangeItem_Del( int nUniIndex, SQUAD llCount )
{
	// Send network message
	CNetworkMessage	nm( (UBYTE)MSG_EXCHANGE );
	nm << (SBYTE)MSG_EXCHANGE_ITEM;
	nm << (SBYTE)MSG_EXCHANGE_ITEM_DEL;
	nm << (SLONG)nUniIndex;
	nm << llCount;

	SendToServerNew( nm );
}

void CNetworkLibrary::AddQuickSlot( int nPage, int nSlotNum, int nSlotType, int nData0, int nData1 )
{
	// Send network message
	CNetworkMessage	nm( (UBYTE)MSG_QUICKSLOT );
	nm << (SBYTE)MSG_QUICKSLOT_ADD;
	nm << (SBYTE)nPage;
	nm << (SBYTE)nSlotNum;
	nm << (SBYTE)nSlotType;

	if( nSlotType == 2 )
	{
		nm << (SWORD)nData0;
		nm << (SWORD)nData1;
	}
	else if( nSlotType >= 0 )
	{
		nm << (SLONG)nData0;
	}

	SendToServerNew( nm );
}

void CNetworkLibrary::SwapQuickSlot( int nPage1, int nSlot1, int nPage2, int nSlot2 )
{
	// Send network message
	CNetworkMessage	nm( (UBYTE)MSG_QUICKSLOT );
	nm << (SBYTE)MSG_QUICKSLOT_SWAP;
	nm << (SBYTE)nPage1;
	nm << (SBYTE)nSlot1;
	nm << (SBYTE)nPage2;
	nm << (SBYTE)nSlot2;

	SendToServerNew( nm );
}

// TO-KR-T20090903-005 관련 파티 신청 수정. [11/27/2009 rumist]
// void CNetworkLibrary::PartyInvite( SBYTE sbType, SLONG slIndex )
// {
// 	// Send network message
// 	CNetworkMessage	nm( MSG_PARTY );
// 	nm << (SBYTE)MSG_PARTY_INVITE;
// 	nm << sbType;
// 	nm << slIndex;
// 
// 	SendToServerNew( nm );
// }
void CNetworkLibrary::PartyInvite( SBYTE sbType, SLONG slIndex, CTString strName )
{
	// Send network message
	CNetworkMessage	nm( (UBYTE)MSG_PARTY );
	nm << (SBYTE)MSG_PARTY_INVITE;
	nm << sbType;
	nm << slIndex;
	nm << strName;

	SendToServerNew( nm );
}


void CNetworkLibrary::PartyAllow()
{
	CNetworkMessage	nm( (UBYTE)MSG_PARTY );
	nm << (SBYTE)MSG_PARTY_ALLOW;

	SendToServerNew( nm );
}

void CNetworkLibrary::PartyReject()
{
	CNetworkMessage	nm( (UBYTE)MSG_PARTY );
	nm << (SBYTE)MSG_PARTY_REJECT;

	SendToServerNew( nm );
}

void CNetworkLibrary::PartyQuit()
{
	CNetworkMessage	nm( (UBYTE)MSG_PARTY );
	nm << (SBYTE)MSG_PARTY_QUIT;

	SendToServerNew( nm );
}

void CNetworkLibrary::PartyKick( SLONG slIndex )
{
	CNetworkMessage	nm( (UBYTE)MSG_PARTY );
	nm << (SBYTE)MSG_PARTY_KICK;
	nm << slIndex;

	SendToServerNew( nm );
}

void CNetworkLibrary::ItemPlusEffectReq( SBYTE sbOption )
{
	CNetworkMessage	nm( (UBYTE)MSG_UI );
	nm << (SBYTE)MSG_UI_PLUS_EFFECT_REQ;
	nm << sbOption;

	SendToServerNew( nm );
}
// ypark -->|

// ----------------------------------------------------------------------------
// Name : FindTargetsInRange()
// Desc : 
// ----------------------------------------------------------------------------
// FIXME : 퍼포먼스 측정이 필요한 부분.
// FIXME : 소환수 및 싱글던젼의 경우 속도에서 문제가 발생할수 있는 함수임.
void CNetworkLibrary::FindTargetsInRange(
								 CEntity* pPlayer, 								// 타겟.
								 CEntity* pCenter,
								 CSelectedEntities &cen, 								 
								 FLOAT fFallOffRange, 
								 INDEX iMaxEnemies,
								 FLOAT fAngle, 								 
								 INDEX iSkillIndex,
								 INDEX iTargetType )
{
	INDEX iCount		= 0;
	FLOAT3D vCenter		= pCenter->GetPlacement().pl_PositionVector;
	const FLOATaabbox3D &boxRange = FLOATaabbox3D(vCenter, fFallOffRange);

	CEntity* penPlEntity;
	CPlayerEntity* penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = (CPlayerEntity*) penPlEntity;

	// for each entity in the world of this entity
	FOREACHINDYNAMICCONTAINER(pPlayer->en_pwoWorld->wo_cenEntities, CEntity, iten) 
	{
		// if it is zoning brush entity
		if (iten->en_RenderType == CEntity::RT_BRUSH && (iten->en_ulFlags&ENF_ZONING)) 
		{
			// get first mip in its brush
			CBrushMip *pbm = iten->en_pbrBrush->GetFirstMip();
			// if the mip doesn't touch the box
			if (!pbm->bm_boxBoundingBox.HasContactWith(boxRange)) 
			{
				// skip it
				continue;
			}

			// for all sectors in this mip
			FOREACHINDYNAMICARRAY(pbm->bm_abscSectors, CBrushSector, itbsc) 
			{
				// if the sector doesn't touch the box
				if (!itbsc->bsc_boxBoundingBox.HasContactWith(boxRange)) 
				{
					// skip it
					continue;
				}

				// for all entities in the sector
				{FOREACHDSTOFSRC(itbsc->bsc_rsEntities, CEntity, en_rdSectors, pen)
					
					// SKA 모델만 컨테이너에 추가합니다.
					if ((pen->en_RenderType==CEntity::RT_SKAMODEL)
						&& (pen->GetFlags() & ENF_ALIVE)
						&& boxRange.HasContactWith(
						FLOATaabbox3D(pen->GetPlacement().pl_PositionVector, pen->en_fSpatialClassificationRadius)))
					{
						// if it has collision box
						if (pen->en_pciCollisionInfo!=NULL) 
						{
							// for each sphere
							FOREACHINSTATICARRAY(pen->en_pciCollisionInfo->ci_absSpheres, CMovingSphere, itms) 
							{
								// project it
								itms->ms_vRelativeCenter0 = itms->ms_vCenter * pen->en_mRotation + pen->en_plPlacement.pl_PositionVector;

								// if the sphere touches the range
								if (boxRange.HasContactWith(FLOATaabbox3D(itms->ms_vRelativeCenter0, itms->ms_fR))) 
								{
									// add it to container
									if (!cen.IsMember(pen)) 
									{
										FLOAT3D vHitPos;

										if ( (pen != pCenter) )
										{
											// 최대 갯수를 넘지 않도록 하고...
											if(iCount >= iMaxEnemies)
											{
												return;
											}

											// 내가 나한테 쓴 경우...
											if( pPlayer == pCenter )
											{
												FLOAT3D vDelta = pen->GetPlacement().pl_PositionVector - pPlayer->GetPlacement().pl_PositionVector;
												vDelta(2)	= 0.0f;
												FLOAT fLength = vDelta.Length();
												if( fLength <= fFallOffRange )
												{
													if( penPlayerEntity->CheckAttackTarget(iSkillIndex, pen, fLength) )
													{
														cen.Add(pen);
														iCount++;
													}
												}
											}
											// 범위 제한이 없을때...( 이 경우에는 플레이어 중심이어야만 함!!! )
											else if( iTargetType != CSkill::STT_TARGET_RECT && 
													fAngle != 360.0f )
											{
												// 플레이어와 각 엔티티 사이의 각도를 계산하여 처리함.
												// 플레이어의 위치와 엔티티의 위치의 벡터를 구하고,
												// 그 값을 Heading으로 변환함.
												FLOAT3D vNormal;
												FLOAT	fLength;
												//AnglesToDirectionVector(pPlayer->GetPlacement().pl_OrientationAngle, vNormal);
												vNormal = vCenter - pPlayer->GetPlacement().pl_PositionVector;
												vNormal(2) = 0.0f;
												vNormal.Normalize();
												FLOAT3D vDelta = pen->GetPlacement().pl_PositionVector - pPlayer->GetPlacement().pl_PositionVector;
												vDelta(2) = 0.0f;
												fLength	= vDelta.Length();
												vDelta.Normalize();

												if( fLength <= fFallOffRange )
												{
													// FIXME : 느린 루틴임.
													// FIXME : 테이블을 이용할것.
													//ANGLE aDelta = GetRelativeHeading(vDelta);
													float fDelta = acos((vNormal%vDelta)/(vNormal.Length() * vDelta.Length()));
													ANGLE aDelta = AngleRad(fDelta);

													if(fmod(fabs(aDelta), 360.0f) < fAngle)
													{
														if( penPlayerEntity->CheckAttackTarget( iSkillIndex, pen, fLength ) )
														{
															cen.Add(pen);
															iCount++;
														}													
													}
												}
											}
											// 타겟 중심.
											else if( iTargetType == CSkill::STT_TARGET_RANGE )
											{
												FLOAT3D vDelta = pen->GetPlacement().pl_PositionVector - pCenter->GetPlacement().pl_PositionVector;
												vDelta(2)	= 0.0f;
												if( vDelta.Length() <= fFallOffRange )
												{
													if( penPlayerEntity->CheckAttackTarget( iSkillIndex, pen, vDelta.Length() ) )
													{
														cen.Add(pen);
														iCount++;
													}												
												}
											}
											// 일직선 사각형 영역일 경우...
											else if( iTargetType == CSkill::STT_TARGET_RECT )											
											{
												FLOAT3D vNormal;												
												vNormal = vCenter - pPlayer->GetPlacement().pl_PositionVector;
												vNormal(2) = 0.0f;
												vNormal.Normalize();

												FLOAT fLength = 0.0f;
												FLOAT3D vDelta = pen->GetPlacement().pl_PositionVector - pPlayer->GetPlacement().pl_PositionVector;												
												vDelta(2) = 0.0f;
												fLength = vDelta.Length();
												vDelta.Normalize();												

												float fDelta	= acos((vNormal%vDelta)/(vNormal.Length() * vDelta.Length()));
												float fDist		= fLength * sin(fDelta);
												float fDist2	= fLength * cos(fDelta);
												float fDot		= vDelta % vNormal;

												if( fDot <= 0 )
												{
													goto next_entity;
												}
												
												if( fabs(fDist) <= 0.5f && fabs(fDist2) <= fFallOffRange )
												{
													if( penPlayerEntity->CheckAttackTarget( iSkillIndex, pen, fLength ) )
													{
														cen.Add(pen);
														iCount++;
													}												
												}
											}
										}										
									}
									goto next_entity;
								} 
							}						
						}						
					}
					next_entity:;
				ENDFOR}
			}
		}
	}
}

void CNetworkLibrary::FindTargetsInRangeEx(CEntity* pCenter, CSelectedEntities& cen, FLOAT fFallOffRange, INDEX iMaxEnemies)
{
	INDEX iCount = 0;
	FLOAT3D vCenter = pCenter->GetPlacement().pl_PositionVector;
	const FLOATaabbox3D &boxRange = FLOATaabbox3D(vCenter, fFallOffRange);

	// for each entity in the world of this entity
	FOREACHINDYNAMICCONTAINER(pCenter->en_pwoWorld->wo_cenAllEntities, CEntity, iten)
	{
		// if it is zoning brush entity
		if (iten->en_RenderType == CEntity::RT_BRUSH && (iten->en_ulFlags & ENF_ZONING))
		{
			// get first mip in its brush
			CBrushMip *pbm = iten->en_pbrBrush->GetFirstMip();
			// if the mip doesn't touch the box
			if (!pbm->bm_boxBoundingBox.HasContactWith(boxRange))
			{	// skip it
				continue;
			}

			// for all sectors in this mip
			FOREACHINDYNAMICARRAY(pbm->bm_abscSectors, CBrushSector, itbsc)
			{
				// if the sector doesn't touch the box
				if (!itbsc->bsc_boxBoundingBox.HasContactWith(boxRange))
				{// skip it
					continue;
				}

				// for all entities in the sector
				{FOREACHDSTOFSRC(itbsc->bsc_rsEntities, CEntity, en_rdSectors, pen)
					// SKA 모델만 컨테이너에 추가합니다.
					if ((pen->en_RenderType == CEntity::RT_SKAMODEL) && (pen->GetFlags() & ENF_ALIVE) &&
						boxRange.HasContactWith(FLOATaabbox3D(pen->GetPlacement().pl_PositionVector, pen->en_fSpatialClassificationRadius)))
					{
						// if it has collision box
						if (pen->en_pciCollisionInfo != NULL)
						{
							// for each sphere
							FOREACHINSTATICARRAY(pen->en_pciCollisionInfo->ci_absSpheres, CMovingSphere, itms)
							{
								// project it
								itms->ms_vRelativeCenter0 = itms->ms_vCenter * pen->en_mRotation + pen->en_plPlacement.pl_PositionVector;
								// if the sphere touches the range
								if (boxRange.HasContactWith(FLOATaabbox3D(itms->ms_vRelativeCenter0, itms->ms_fR)))
								{
									// add it to container
									if (!cen.IsMember(pen))
									{
										FLOAT3D vHitPos;

										if (pen != pCenter)
										{
											FLOAT3D vDelta = pen->GetPlacement().pl_PositionVector - pCenter->GetPlacement().pl_PositionVector;
											vDelta(2) = 0.0f;

											if (vDelta.Length() <= fFallOffRange)
											{
												ObjInfo* pInfo = ObjInfo::getSingleton();

												//if (((CUnit*)INFO()->_WildPetInfo.pet_pEntity)->CheckTarget(pen))	// 현재 등록된 펫의 AI 조건
												if (pInfo->GetMyApetInfo() != NULL && pInfo->GetMyApetInfo()->GetEntity()) // 예외 처리
												{
													if (((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CheckEntityOfTarget(pInfo->GetMyApetInfo()->m_pEntity, pen))
													{
														cen.Add(pen);
														iCount++;
														
														if (iCount >= iMaxEnemies)
															return;
													}
												}
												else{
													return;
												}
											}
										}
									}
									goto next_ent;
								}
							}
						}
					}
					next_ent:;
				ENDFOR}
			}
		}
	}
}

// 멀티 공격가능한지를 판단합니다.
// ----------------------------------------------------------------------------
// Name : CheckSkillAttack()
// Desc : 
// ----------------------------------------------------------------------------
BOOL CNetworkLibrary::CheckSkillAttack( INDEX iSkillIndex, CEntity* pEntity )
{
	// 이쪽으로 iSkillIndex 가 -1이 들어오면 안됨.
	if( iSkillIndex == -1 )
		return FALSE;

	CEntity* penPlEntity;
	CPlayerEntity* penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = (CPlayerEntity*) penPlEntity;
	
	BOOL bForHelp		= FALSE;	
	CSkill &rSkillData	= GetSkillData( iSkillIndex );
	bForHelp		= rSkillData.GetFlag() & SF_FORHELP;
	const int iJob2		= rSkillData.GetJob2();

	BOOL bIsCharacter	= pEntity->IsCharacter();
	BOOL bIsMob			= pEntity->IsEnemy();
	BOOL bIsPet			= pEntity->IsPet();
	BOOL bIsSummon		= pEntity->IsSlave();
	BOOL bIsPlayer		= pEntity->IsPlayer();

	// 애완동물을 타고 있을때 쓸수 있는 스킬일 경우.
	if( _pNetwork->MyCharacterInfo.bPetRide && !( ( iJob2 == 2 || iJob2 == 3 ) ) )
	{
		return FALSE;
	}
	
	// HELP 스킬은 내 애완동물
	if( bForHelp )
	{
		if( bIsPlayer )
			return TRUE;
		else if( pEntity->GetFirstExFlags() & ( ENF_EX1_CURRENT_SLAVE | ENF_EX1_CURRENT_PET ) )
			return TRUE;
		else if( bIsPet || bIsSummon || bIsCharacter )
		{	
			return !((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CheckNormalAttack( pEntity, 0.0f );
		}		
	}
	else
	{
		// 자기가 자기한테 공격할수는 없음.
		if( bIsPlayer )
		{
			return FALSE;
		}
		// 그 이외의 경우.
		else
		{
			return ((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CheckNormalAttack( pEntity, 0.0f );
		}
	}
	return FALSE;
}

// NOTE : 공격자 목록이 20개를 넘거나, 시간이 초과한경우에는 메시지를 보낼것...
// ----------------------------------------------------------------------------
// Name : AddAttackList()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::AddAttackList( UBYTE ubAttackType, INDEX iAttackIndex, UBYTE ubTargetType, INDEX iTargetIndex )
{
	sAttackInfo TempAttack;
	TempAttack.ubAttackType	= ubAttackType;
	TempAttack.iAttackIndex	= iAttackIndex;	
	TempAttack.iTargetIndex	= iTargetIndex;
	TempAttack.ubTargetType	= ubTargetType;

	if( ubTargetType == MSG_CHAR_NPC )
	{
		m_vectorAttackNPCList.push_back(TempAttack);
	}
	else if( ubTargetType == MSG_CHAR_ELEMENTAL )
	{
		m_vectorAttackSummonList.push_back(TempAttack);
	}
	else if( ubTargetType == MSG_CHAR_PET )
	{
		m_vectorAttackPetList.push_back(TempAttack);
	}
	else
	{
		m_vectorAttackPCList.push_back(TempAttack);
	}
}

// ----------------------------------------------------------------------------
// Name : SendAttackList()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendAttackList()
{
	int i;
	if( !m_vectorAttackNPCList.empty() ) // NPC가 NPC를 공격하려고 할때...	
	{
		INDEX iCount	= m_vectorAttackNPCList.size();
		CNetworkMessage nmAttack;
		RequestClient::doPDAttack* packet = reinterpret_cast<RequestClient::doPDAttack*>(nmAttack.nm_pubMessage);
		packet->type = MSG_PD_ATTACK;
		packet->subType = 0;
				
		std::vector<sAttackInfo>::iterator it	= m_vectorAttackNPCList.begin();
		std::vector<sAttackInfo>::iterator end	= m_vectorAttackNPCList.end();		

		packet->tIndex = (*it).iTargetIndex;
		packet->tCharType = MSG_CHAR_NPC;		
		packet->attackType = (*it).ubAttackType;
		packet->multicount = iCount;
			
		for( i = 0; it != end; ++it, i++ )
		{
			INDEX iIndex = (*it).iAttackIndex;
			packet->list[i].index = iIndex;
		}

		int nSize = sizeof(RequestClient::doPDAttack::tag_multi) * i;	
		nmAttack.setSize( sizeof(*packet) + nSize);

		SendToServerNew(nmAttack);

		if( !m_vectorAttackNPCList.empty() )
			m_vectorAttackNPCList.clear();
	}

	if( !m_vectorAttackSummonList.empty() ) // NPC가 소환수를 공격하려고 할때...
	{
		INDEX iCount	= m_vectorAttackSummonList.size();
		CNetworkMessage nmAttack;
		RequestClient::doPDAttack* packet = reinterpret_cast<RequestClient::doPDAttack*>(nmAttack.nm_pubMessage);
		packet->type = MSG_PD_ATTACK;
		packet->subType = 0;

		std::vector<sAttackInfo>::iterator it	= m_vectorAttackSummonList.begin();
		std::vector<sAttackInfo>::iterator end	= m_vectorAttackSummonList.end();
		
		packet->tIndex = (*it).iTargetIndex;
		packet->tCharType = MSG_CHAR_ELEMENTAL;		
		packet->attackType = (*it).ubAttackType;
		packet->multicount = iCount;

		for( i = 0; it != end; ++it, i++ )
		{
			INDEX iIndex = (*it).iAttackIndex;
			packet->list[i].index = iIndex;
		}
	
		int nSize = sizeof(RequestClient::doPDAttack::tag_multi) * i;	
		nmAttack.setSize( sizeof(*packet) + nSize);

		SendToServerNew(nmAttack);

		if( !m_vectorAttackSummonList.empty() )
			m_vectorAttackSummonList.clear();
	}

	if( !m_vectorAttackPetList.empty() )// NPC가 Pet을 공격하려고 할때...
	{
		INDEX iCount	= m_vectorAttackPetList.size();
		CNetworkMessage nmAttack;
		RequestClient::doPDAttack* packet = reinterpret_cast<RequestClient::doPDAttack*>(nmAttack.nm_pubMessage);
		packet->type = MSG_PD_ATTACK;
		packet->subType = 0;

		std::vector<sAttackInfo>::iterator it	= m_vectorAttackPetList.begin();
		std::vector<sAttackInfo>::iterator end	= m_vectorAttackPetList.end();

		packet->tIndex = (*it).iTargetIndex;
		packet->tCharType = MSG_CHAR_PET;		
		packet->attackType = (*it).ubAttackType;
		packet->multicount = iCount;

		for( i = 0; it != end; ++it, i++ )
		{
			INDEX iIndex = (*it).iAttackIndex;
			packet->list[i].index = iIndex;
		}

		int nSize = sizeof(RequestClient::doPDAttack::tag_multi) * i;	
		nmAttack.setSize( sizeof(*packet) + nSize);

		SendToServerNew(nmAttack);

		if( !m_vectorAttackPetList.empty() )
			m_vectorAttackPetList.clear();
	}
	
	if( !m_vectorAttackPCList.empty() ) // NPC가 PC를 공격하려고 할때...
	{
		INDEX iCount	= m_vectorAttackPCList.size();
		CNetworkMessage nmAttack;
		RequestClient::doPDAttack* packet = reinterpret_cast<RequestClient::doPDAttack*>(nmAttack.nm_pubMessage);
		packet->type = MSG_PD_ATTACK;
		packet->subType = 0;

		std::vector<sAttackInfo>::iterator it	= m_vectorAttackPCList.begin();
		std::vector<sAttackInfo>::iterator end	= m_vectorAttackPCList.end();
		
		packet->tIndex = (*it).iTargetIndex;
		packet->tCharType = MSG_CHAR_PC;		
		packet->attackType = (*it).ubAttackType;
		packet->multicount = iCount;			
				
		for( i = 0; it != end; ++it, i++ )
		{
			INDEX iIndex = (*it).iAttackIndex;
			packet->list[i].index = iIndex;
		}

		int nSize = sizeof(RequestClient::doPDAttack::tag_multi) * i;	
		nmAttack.setSize( sizeof(*packet) + nSize);

		SendToServerNew(nmAttack);

		CPrintF("Send Attack List : %d\n", iCount );

		if( !m_vectorAttackPCList.empty() )
			m_vectorAttackPCList.clear();
	}
	//ClearAttackList();
}

// ----------------------------------------------------------------------------
// Name : ClearAttackList()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::ClearAttackList()
{
	if( !m_vectorAttackPCList.empty() )
		m_vectorAttackPCList.clear();

	if( !m_vectorAttackSummonList.empty() )
		m_vectorAttackSummonList.clear();

	if( !m_vectorAttackPetList.empty() )
		m_vectorAttackPetList.clear();

	if( !m_vectorAttackNPCList.empty() )
		m_vectorAttackNPCList.clear();
}

// ----------------------------------------------------------------------------
// Name : AddMoveList()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::AddMoveList( INDEX iIndex, FLOAT fX, FLOAT fZ, FLOAT fH, FLOAT fAngle )
{	
	sMoveInfo TempMove;
	TempMove.iIndex		= iIndex;
	TempMove.fX			= fX;
	TempMove.fZ			= fZ;
	TempMove.fH			= fH;
	TempMove.fAngle		= fAngle;
	m_vectorMoveList.push_back(TempMove);

	// 20개를 넘어가면... 서버로 메세지를 보냄.
	if( m_vectorMoveList.size() >= MAX_MOVE_LIST )
	{
		SendMoveList();	
	}
}

// EDIT : BS : BEGIN
void CNetworkLibrary::AddMoveList(CEntity &en)
{
	if (en.IsEnemy())
	{
		DWORD curTickCount = GetTickCount();
		// EDIT : BS : 시간 딜레이
		if (en.m_tickSendPDMove == 0 || curTickCount - en.m_tickSendPDMove >= 500)
		{
			en.m_tickSendPDMove = curTickCount;

			CPlacement3D pl		= en.GetLerpedPlacement();

			// 같은 넘이 있으면 좌표만 수정
			bool bAdd = true;
			if (!m_vectorMoveList.empty())
			{
				INDEX iCount		= m_vectorMoveList.size();	
				std::vector<sMoveInfo>::iterator it		= m_vectorMoveList.begin();
				std::vector<sMoveInfo>::iterator end	= m_vectorMoveList.end();
				for( ; it != end; ++it )
				{
					if ((*it).iIndex == en.GetNetworkID())
					{
						(*it).fX			= pl.pl_PositionVector(1);
						(*it).fZ			= pl.pl_PositionVector(3);
						(*it).fH			= pl.pl_PositionVector(2);
						(*it).fAngle		= pl.pl_OrientationAngle(1);
						bAdd = false;
						break;
					}
				}
			}

			if (bAdd)
			{
				sMoveInfo TempMove;
				TempMove.iIndex		= en.GetNetworkID();
				TempMove.fX			= pl.pl_PositionVector(1);
				TempMove.fZ			= pl.pl_PositionVector(3);
				TempMove.fH			= pl.pl_PositionVector(2);
				TempMove.fAngle		= pl.pl_OrientationAngle(1);
				m_vectorMoveList.push_back(TempMove);
			}

			// 20개를 넘어가면... 서버로 메세지를 보냄.
			if( m_vectorMoveList.size() >= MAX_MOVE_LIST )
			{
				SendMoveList(true);	
			}
		}
	}
}
// EDIT : BS : END

// ----------------------------------------------------------------------------
// Name : SendMoveList()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendMoveList(bool bForce)
{
	if( m_vectorMoveList.empty() )
		return;

// EDIT : BS : BEGIN
	DWORD tickCur = GetTickCount();
	// EDIT : BS : 시간 딜레이
	// 리스트가 가득 안 차고 최근 보낸지 0.5초가 안 지났으면 모은다
	if (!bForce
		&& m_vectorMoveList.size() < MAX_MOVE_LIST
		&& m_tickSendMoveList != 0
		&& tickCur - m_tickSendMoveList < 500)
		return ;
	m_tickSendMoveList = tickCur;
// EDIT : BS : END
	int count;
	//MSG_PD_MOVE,				//무브				: movetype(uc) speed(f) x(f) z(f) h(f) r(f) y(n) multicount(c) multiindex(n:multicount)
	CNetworkMessage nmMove;
	RequestClient::moveForPernalDungeon* packet = reinterpret_cast<RequestClient::moveForPernalDungeon*>(nmMove.nm_pubMessage);
	packet->type = MSG_PD_MOVE;
	packet->subType = 0;
	packet->moveType = MSG_MOVE_STOP;

	std::set<INDEX> tset;

	std::vector<sMoveInfo>::iterator it		= m_vectorMoveList.begin();
	std::vector<sMoveInfo>::iterator end	= m_vectorMoveList.end();
	for( count = 0; it != end; ++it, ++count)
	{
		if (tset.insert((*it).iIndex).second == false)
			continue;

		packet->list[count].multiIndex = (*it).iIndex;
		packet->list[count].x = (*it).fX;
		packet->list[count].z = (*it).fZ;
		packet->list[count].h = (*it).fH + 0.5f;
		packet->list[count].r = (*it).fAngle;
		packet->list[count].ylayer = MyCharacterInfo.yLayer;
	}

	packet->multicount = tset.size();
	int nSize = sizeof(RequestClient::moveForPernalDungeon::tag_multi) * count;

	nmMove.setSize(sizeof(*packet) + nSize);
	SendToServerNew(nmMove);

	CPrintF("Send Move List : %d\n", count );
	ClearMoveList();
}

// ----------------------------------------------------------------------------
// Name : ClearMoveList()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::ClearMoveList()
{
	if( !m_vectorMoveList.empty() )
		m_vectorMoveList.clear();
}

// ----------------------------------------------------------------------------
// Name : AddRegenList()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::AddRegenList(INDEX iIndex, INDEX iMobType, FLOAT fX, FLOAT fY, FLOAT fZ, FLOAT fAngle, INDEX iYLayer, INDEX iEntityID)
{
	sRegenInfo TempRegen;
	TempRegen.iIndex		= iIndex;
	TempRegen.iMobType		= iMobType;
	TempRegen.fX			= fX;
	TempRegen.fY			= fY;
	TempRegen.fZ			= fZ;
	TempRegen.fAngle		= fAngle;
	TempRegen.iYLayer		= iYLayer;
	TempRegen.iEntityID		= iEntityID;
	m_vectorRegenList.push_back(TempRegen);
}

// ----------------------------------------------------------------------------
// Name : SendRegenList()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendRegenList()
{
	if( m_vectorRegenList.empty() )
		return;
	
	INDEX iCount		= m_vectorRegenList.size();	

	std::vector<sRegenInfo>::iterator it = m_vectorRegenList.begin();
	std::vector<sRegenInfo>::iterator end = m_vectorRegenList.end();

	for(;it != end; ++it)
	{	
		CNetworkMessage nmMobSpawn((UBYTE)MSG_NPC_REGEN);	
		nmMobSpawn << (*it).iIndex;						// Index
		nmMobSpawn << (*it).iMobType;					// DB Index
		nmMobSpawn << (*it).fX;							// Pos X
		nmMobSpawn << (*it).fZ;							// Pos Z
		nmMobSpawn << (*it).fY;							// Pos H						
		nmMobSpawn << (*it).fAngle;						// Angle
		nmMobSpawn << (*it).iYLayer;					// Y Layer
		nmMobSpawn << (*it).iEntityID;					// EntityID
		_pNetwork->SendToServerNew(nmMobSpawn);
	}
	
	ClearRegenList();
}

// ----------------------------------------------------------------------------
// Name : ClearRegenList()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::ClearRegenList()
{
	if( !m_vectorRegenList.empty() )
		m_vectorRegenList.clear();
}

// ----------------------------------------------------------------------------
// Name : AddLegitList()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::AddLegitList(int Index)
{
	if (!SearchLegitList(Index))
	{
		m_vectorLegitList.push_back(Index);
	}
}

// ----------------------------------------------------------------------------
// Name : SearchLegitList()
// Desc : 
// ----------------------------------------------------------------------------
BOOL CNetworkLibrary::SearchLegitList(int Index, BOOL bDel/* = FALSE*/)
{
	if (!m_vectorLegitList.empty())
	{
		std::vector<int>::iterator tmpItr;

		tmpItr = std::find(m_vectorLegitList.begin(), m_vectorLegitList.end(), Index);

		if (tmpItr != m_vectorLegitList.end())
		{
			if (bDel) m_vectorLegitList.erase(tmpItr);
			return TRUE;
		}
	}

	return FALSE;
}

// ----------------------------------------------------------------------------
// Name : DelLegitList()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::DelLegitList(int Index)
{
	SearchLegitList(Index, TRUE);
}

// ----------------------------------------------------------------------------
// Name : AddAffinityRewardNPC()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::AddAffinityRewardNPC(int iIndex)
{
	// 이미 리스트에 있으면 그냥 리턴
	if( std::count(m_listAffinityRewardNPCList.begin(), m_listAffinityRewardNPCList.end(), iIndex) )
		return;

	m_listAffinityRewardNPCList.push_back(iIndex);	
}

// ----------------------------------------------------------------------------
// Name : RemoveAffinityRewardNPC()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::RemoveAffinityRewardNPC(int iIndex)
{
	m_listAffinityRewardNPCList.remove(iIndex);	
}

// ----------------------------------------------------------------------------
// Name : ClearAffinityRewardNPC()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::ClearAffinityRewardNPC()
{
	m_listAffinityRewardNPCList.clear();
	m_listAffinityRewardNPCList.resize(0);	
}

// ----------------------------------------------------------------------------
// Name : GetAffinityRewardNPCListCount()
// Desc : 
// ----------------------------------------------------------------------------
const int CNetworkLibrary::GetAffinityRewardNPCListCount()
{
	return m_listAffinityRewardNPCList.size();
}

// ----------------------------------------------------------------------------
// Name : GetAffinityRewardNPCList()
// Desc : 
// ----------------------------------------------------------------------------
const std::list<int>& CNetworkLibrary::GetAffinityRewardNPCList()
{
	return m_listAffinityRewardNPCList;
}

//안태훈 수정 시작	//(Zone Change System)(0.1)
// ----------------------------------------------------------------------------
// Name : GoZone()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::GoZone(int zone, int extra,int npcIdx )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// (eons) 신전 과 필드 이동시 지역표시 문제로 지역 번호 저장
	_pNetwork->MyCharacterInfo.LocalNo = extra;
	
	if(pUIManager->IsInstantZone(zone)) // [sora] 인존으로 이동할 경우
	{	
		RaidInzoneJoinReq(zone);
		return;
	}
	
	if(pUIManager->IsPlayInZone()) // [sora] 인스턴트존내부에서 나갈경우
	{
		// [090709: selo] 레이드에서 나갈때는 퀘스트 북의 레이드 메시지를 비운다
		GAMEDATAMGR()->GetQuest()->RemoveRaidMessageAll();
		
		RaidInzoneQuitReq(zone, extra);
		
		return;
	}
	
	const int iJob = _pNetwork->MyCharacterInfo.job;

	INFO()->TargetClear();

	pUIManager->SetCSFlagOn(CSF_TELEPORT);

	//----------------------------------------------------
	CNetworkMessage nmZone;
	RequestClient::moveGoZone* packet = reinterpret_cast<RequestClient::moveGoZone*>(nmZone.nm_pubMessage);
	packet->type = MSG_GO_ZONE;
	packet->subType = 0;
	packet->zone = zone;
	packet->extra = extra;
	packet->npcIndex = npcIdx;
	nmZone.setSize(sizeof(*packet));
	SendToServerNew(nmZone);
	// (eons) 신전 과 필드 이동시 지역표시 문제로 지역 번호 저장
	_pNetwork->MyCharacterInfo.LocalNo = extra;
}

//안태훈 수정 시작	//(Teleport System)(0.1)
void CNetworkLibrary::WriteCurrentPos(int slot, const char *szComment)
{
	CNetworkMessage nmMemPos;
	RequestClient::memposWrite* packet = reinterpret_cast<RequestClient::memposWrite*>(nmMemPos.nm_pubMessage);
	packet->type = MSG_MEMPOS;
	packet->subType = MSG_MEMPOS_WRITE;
	packet->slot = slot;
	memcpy(packet->comment, szComment, MEMPOS_COMMENT_LENGTH + 1);
	nmMemPos.setSize( sizeof(*packet) );

	_pNetwork->SendToServerNew(nmMemPos);
}
//안태훈 수정 끝	//(Teleport System)(0.1)

//안태훈 수정 시작	//(GM Command)(0.1)
void CNetworkLibrary::SendWhoAmI()
{
	CNetworkMessage nmGM((UBYTE)MSG_GM);
	nmGM << (SBYTE)MSG_GM_WHOAMI;
	SendToServerNew(nmGM);
}

void CNetworkLibrary::SendGMCommand(const char *szCommand)
{
	CNetworkMessage nmGM((UBYTE)MSG_GM);
	nmGM << (SBYTE)MSG_GM_COMMAND;
	nmGM << CTString(szCommand);
	SendToServerNew(nmGM);
}
//안태훈 수정 끝	//(GM Command)(0.1)
//안태훈 수정 끝	//(Game Manager Command)(0.1)
//-----------------------------------------------------------------------------
// NOTE : 구출 NPC의 정보를 파티로 표시하기 위한 부분이며,
// NOTE : 현재는 파티멤버로 반드시 1명만 되도록 되어있음.
// NOTE : 추후, 변경할것.
// ----------------------------------------------------------------------------
// Name : AddRescueNPC()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::AddRescueNPC(CEntity* pEntity)
{
	if(pEntity->IsEnemy())
	{				
		CEntityProperty	&epPropertyNpcIndex	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 91));	// Enemy Index
		const INDEX		iMobIndex = ENTITYPROPERTY( &*pEntity, epPropertyNpcIndex.ep_slOffset, INDEX);
		CMobData*		MD	= CMobData::getData(iMobIndex);
		FLOAT3D			vPos = pEntity->en_plPlacement.pl_PositionVector;
		GAMEDATAMGR()->GetPartyInfo()->PartyAddMember( 0, 0, CTString(MD->GetName()), 0, 0, MD->GetLevel(),
												MD->GetHealth(), MD->GetHealth(), MD->GetMP(), MD->GetMP(),
												vPos( 1 ), vPos( 3 ), 0, _pNetwork->MyCharacterInfo.zoneNo );
	}
}

// ----------------------------------------------------------------------------
// Name : UpdateRescueNPC()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::UpdateRescueNPC(CEntity* pEntity, int iHP, int iMP)
{
	if(pEntity->IsEnemy())
	{				
		CEntityProperty	&epPropertyNpcIndex	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 91));	// Enemy Index
		const INDEX		iMobIndex = ENTITYPROPERTY( &*pEntity, epPropertyNpcIndex.ep_slOffset, INDEX);
		CMobData*		MD	= CMobData::getData(iMobIndex);
		FLOAT3D			vPos = pEntity->en_plPlacement.pl_PositionVector;
		GAMEDATAMGR()->GetPartyInfo()->PartyMemberInfo( 0, MD->GetLevel(), iHP, MD->GetHealth(), iMP, MD->GetMP(),
												vPos( 1 ), vPos( 3 ), 0, _pNetwork->MyCharacterInfo.zoneNo );		
	}
}

// ----------------------------------------------------------------------------
// Name : EndRescueNPC()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::EndRescueNPC()
{
	if(GAMEDATAMGR()->GetPartyInfo()->GetMemberCount())
		GAMEDATAMGR()->GetPartyInfo()->PartyEnd();
}

//-----------------------------------------------------------------------------
// Purpose: 목록에 있는 아이템을 대여 합니다.
// Input  : iShopID - 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendLeaseItem(int iItemIdx)
{
	CNetworkMessage nmMessage;
	RequestClient::doItemLendWeapon* packet = reinterpret_cast<RequestClient::doItemLendWeapon*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_LEND_WEAPON;
	packet->itemDBIndex = iItemIdx;
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew( nmMessage );
}
//-----------------------------------------------------------------------------
// Purpose: 목록에 있는 이벤트 아이템을 구입합니다. (2007 가정의 달 이벤트)
// Input  : iShopID - 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendEventItem(int iItemIdx, int iItemcont)
{
	CNetworkMessage nmItem((UBYTE)MSG_EVENT);
	nmItem << (SBYTE)MSG_EVENT_CHILDRENSDAY_2007;
	nmItem << (LONG)MSG_EVENT_CHILDRENSDAY_2007_REQ;
	nmItem << (LONG)iItemIdx;						// ITEM INDEX
	nmItem << (LONG)iItemcont;
	SendToServerNew(nmItem);	
}

//-----------------------------------------------------------------------------
// Purpose: 목록에 있는 아이템을 구입합니다.
// Input  : iShopID - 
//-----------------------------------------------------------------------------
void CNetworkLibrary::BuyItem(int iShopID, int iNumOfItem, __int64 iTotalPrice)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	CNetworkMessage nmMessage;
	RequestClient::doItemBuy* packet = reinterpret_cast<RequestClient::doItemBuy*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_BUY;
	packet->npcIndex = iShopID;
	packet->clientPrice = iTotalPrice;
	packet->buyCount = iNumOfItem;

	for (int i = 0; i < iNumOfItem; ++i)
	{
		SLONG	slIndex = pUIManager->GetShop()->GetTradeItem(i)->Item_Index;
		SQUAD	llItemCount = pUIManager->GetShop()->GetTradeItem(i)->Item_Sum;

		packet->list[i].dbIndex = slIndex;
		packet->list[i].count = llItemCount;
	}

	nmMessage.setSize( sizeof(*packet) + (sizeof(packet->list[0]) * iNumOfItem) );

	SendToServerNew(nmMessage);	
//	CPrintF("Send MSG_ITEM_BUY, Shop ID : %d, Total Item Count : %d, Total Price : %ld\n", iShopID, iNumOfItem, iSumPrice);	
}

//-----------------------------------------------------------------------------
// Purpose: 목록에 있는 아이템을 판매합니다.
// Input  : iShopID - 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SellItem(int iShopID, int iNumOfItem, __int64 iTotalPrice)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	CNetworkMessage nmMessage;
	RequestClient::doItemSell* packet = reinterpret_cast<RequestClient::doItemSell*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_SELL;
	packet->npcIndex = iShopID;
	packet->clientPrice = iTotalPrice;
	packet->sellCount = iNumOfItem;

	for (int i = 0; i < iNumOfItem; ++i)
	{
		SWORD	nTab = pUIManager->GetShop()->GetTradeItem(i)->Item_Tab;
		SWORD	nIndex = pUIManager->GetShop()->GetTradeItem(i)->InvenIndex;
		SQUAD	llCnt = pUIManager->GetShop()->GetTradeItem(i)->Item_Sum;

		if(_pNetwork->m_ubGMLevel > 1)
			CPrintF("Sell Tab:%d,Idx:%d,Cnt:%d\n", nTab, nIndex, llCnt);

		packet->list[i].tab = nTab;
		packet->list[i].invenIndex = nIndex;
		packet->list[i].count = llCnt;
	}

	nmMessage.setSize( sizeof(*packet) + (sizeof(packet->list[0]) * iNumOfItem) );

	SendToServerNew(nmMessage);
//	CPrintF("Send MSG_ITEM_SELL, Shop ID : %d, Total Item Count : %d, Total Price : %ld\n", iShopID, iNumOfItem, iSumPrice);
}
//-----------------------------------------------------------------------------
// Purpose: 잡화상 이용 주문서 목록에 있는 아이템을 구입합니다.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::FieldShopBuyItem( int iNumOfItem, __int64 iTotalPrice)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	CNetworkMessage nmMessage;
	RequestClient::doItemGroceryItemBuy* packet = reinterpret_cast<RequestClient::doItemGroceryItemBuy*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_USE_GROCERY_BUY;
	packet->clientPrice = iTotalPrice;
	packet->buyCount = iNumOfItem;

	for (int i = 0; i < iNumOfItem; ++i)
	{
		SLONG	slIndex = pUIManager->GetShop()->GetTradeItem(i)->Item_Index;
		SQUAD	llItemCount = pUIManager->GetShop()->GetTradeItem(i)->Item_Sum;

		packet->list[i].itemDBIndex = slIndex;
		packet->list[i].count = llItemCount;
	}

	nmMessage.setSize( sizeof(*packet) + (sizeof(packet->list[0]) * iNumOfItem) );

	SendToServerNew( nmMessage );
//	CPrintF("Send MSG_ITEM_BUY, Shop ID : %d, Total Item Count : %d, Total Price : %ld\n", iShopID, iNumOfItem, iSumPrice);	
}
//-----------------------------------------------------------------------------
// Purpose: 잡화상 이용 주문서 목록에 있는 아이템을 판매합니다.
// Input  :  
//-----------------------------------------------------------------------------
void CNetworkLibrary::FieldShopSellItem(int iNumOfItem, __int64 iTotalPrice)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	CNetworkMessage nmMessage;
	RequestClient::doItemGroceryItemSell* packet = reinterpret_cast<RequestClient::doItemGroceryItemSell*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_USE_GROCERY_SELL;
	packet->clientPrice = iTotalPrice;
	packet->sellCount = iNumOfItem;

	for (int i = 0; i < iNumOfItem; ++i)
	{
		SWORD	nTab = pUIManager->GetShop()->GetTradeItem(i)->Item_Tab;
		SWORD	nIndex = pUIManager->GetShop()->GetTradeItem(i)->InvenIndex;
		int		nCnt = (int)pUIManager->GetShop()->GetTradeItem(i)->Item_Sum;

		packet->list[i].tab = nTab;
		packet->list[i].invenIndex = nIndex;
		packet->list[i].count = nCnt;
	}

	nmMessage.setSize( sizeof(*packet) + (sizeof(packet->list[0]) * iNumOfItem) );

	SendToServerNew( nmMessage );
}


//0616 kwon
void CNetworkLibrary::SendChatMessage(int index, CTString &strRecvName, CTString &strMessage, SLONG nExpChatType)
{
	CNetworkMessage nm((UBYTE)MSG_CHAT);
	nm << (unsigned char)index;
	nm << MyCharacterInfo.index;
	nm << MyCharacterInfo.name;
	nm << strRecvName;
	nm << strMessage;
	nm << nExpChatType;
	
	SendToServerNew(nm);
}

//0701 kwon
void CNetworkLibrary::SendStopMessage(CEntity *pEntity, CPlacement3D MyPlacement)
{
	if( !pEntity )
		return;

	if(_cmiComm. IsNetworkOn())
	{	
		CNetworkMessage nmPlayerMove;
		RequestClient::moveForNormal* packet = reinterpret_cast<RequestClient::moveForNormal*>(nmPlayerMove.nm_pubMessage);
		packet->type = MSG_MOVE;
		packet->subType = 0;
		packet->charType = pEntity->GetNetworkType();
		packet->moveType = MSG_MOVE_STOP;
		packet->index = pEntity->GetNetworkID();
		packet->speed = 0.0f;
		packet->x = MyPlacement.pl_PositionVector(1);
		packet->z = MyPlacement.pl_PositionVector(3);
		packet->h = MyPlacement.pl_PositionVector(2);
		packet->r = MyPlacement.pl_OrientationAngle(1);
		packet->ylayer = MyCharacterInfo.yLayer;

		nmPlayerMove.setSize(sizeof(*packet));

		SendToServerNew(nmPlayerMove);	
	}
}

SLONG CNetworkLibrary::SearchClientChaIndex(ULONG Index)//이건 클라이언트 인덱스를 찾는 함수.
{
	SLONG	cha_index = -1;

	int		i;

	for (i = 0; i < eOBJ_MAX; ++i)
	{
		ObjectBase* pObject = ACTORMGR()->GetObject((eOBJ_TYPE)i, Index);

		if (pObject != NULL)
		{
			cha_index = pObject->GetCIndex();
			break;
		}
	}

	return cha_index;
}

//-----------------------------------------------------------------------------
// Purpose: 네트워크 ID값으로 엔티티를 얻습니다.
// Input  : 
//-----------------------------------------------------------------------------
BOOL CNetworkLibrary::SearchEntityByNetworkID( long lIndex, SBYTE sbType, CEntity* &pEntity )
{
	// 캐릭터에 대해서...
	if (lIndex == MyCharacterInfo.index)
	{
		pEntity = CEntity::GetPlayerEntity(0);
		return TRUE;
	}
	else
	{
		int		i;

		for (i = 0; i < eOBJ_MAX; ++i)
		{
			ObjectBase* pObject = ACTORMGR()->GetObject((eOBJ_TYPE)i, lIndex);

			if (pObject != NULL)
			{			
				return _pNetwork->ga_World.EntityExists(pObject->GetCIndex(), pEntity);
			}
		}
	}

	return FALSE;
}

void CNetworkLibrary::SendPickMessage( CEntity* pEntity, ULONG ItemIndex, BOOL bLayerCheck )
{
	if(_cmiComm. IsNetworkOn())
	{
		if( bLayerCheck )
		{
			ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_ITEM, ItemIndex);

			if (pObject != NULL)
			{
				if( abs( pObject->GetyLayer() - MyCharacterInfo.yLayer ) > 1 )
					return;
			}
		}

		CUIManager* pUIManager = CUIManager::getSingleton();
		if( pUIManager->GetReformSystem()->IsVisible() ) // 리폼 시스템 UI가 열려 있을 경우 단축키 막음 [9/6/2012 Ranma]
			return;
		if( pUIManager->GetSocketSystem()->IsVisible() ) // 소켓 시스템 UI가 열려 있을 경우 단축키 막음 Ranma
			return;

		CNetworkMessage nmMessage;
		RequestClient::doItemTake* packet = reinterpret_cast<RequestClient::doItemTake*>(nmMessage.nm_pubMessage);
		packet->type = MSG_ITEM;
		packet->subType = MSG_ITEM_TAKE;
		packet->char_type = pEntity->GetNetworkType();
		packet->char_index = pEntity->GetNetworkID();
		packet->virtualIndex = ItemIndex;
		nmMessage.setSize( sizeof(*packet) );

		SendToServerNew(nmMessage);	
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : plPlacement - 
//			speed - 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendMoveMessage(CEntity *pEntity, CPlacement3D plPlacement, SLONG speed)
{
	if( !pEntity )
		return;

	if(_cmiComm. IsNetworkOn())
	{
		CNetworkMessage nmPlayerMove;
		RequestClient::moveForNormal* packet = reinterpret_cast<RequestClient::moveForNormal*>(nmPlayerMove.nm_pubMessage);
		packet->type = MSG_MOVE;
		packet->subType = 0;
		packet->charType = pEntity->GetNetworkType();

		// [2012/08/22 : Sora] 나이트 쉐도우 비행중에는 비행 메시지를 보내도록 수정
		UBYTE movetype;
		if ( ( pEntity->IsPlayer() ) && 
			( _pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING ) )
		{
			movetype = MSG_MOVE_FLY;
		}
		else
		{
			if(speed < 2.0f)
				movetype = MSG_MOVE_WALK;
			else
				movetype = MSG_MOVE_RUN;
		}

		packet->moveType = movetype;
		packet->index = pEntity->GetNetworkID();
		packet->speed = speed;
		packet->x = plPlacement.pl_PositionVector(1);
		packet->z = plPlacement.pl_PositionVector(3);
		packet->h = plPlacement.pl_PositionVector(2);
		packet->r = plPlacement.pl_OrientationAngle(1);
		packet->ylayer = MyCharacterInfo.yLayer;

		nmPlayerMove.setSize(sizeof(*packet));

		SendToServerNew(nmPlayerMove);
		// 길드 전쟁중 포탈을 타려고 하는데 이동을  하면 취소 (중지)
		CUIManager::getSingleton()->GetGuildWarPortal()->PortalCancel();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendAttackSymbol()
{
	CNetworkMessage nmGuildWar( (UBYTE)MSG_GUILD );
	nmGuildWar << (UBYTE)MSG_GUILD_WAR_ATTACK_SYMBOL;
	SendToServerNew(nmGuildWar);
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : Index - 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendAttackMessage(CEntity *pAttackerEntity, CEntity *pTargetEntity, BOOL bPvp) 
{
	if( !pAttackerEntity || !pTargetEntity )
	{
		return;
	}
	if(_cmiComm. IsNetworkOn())
	{
		CNetworkMessage nmPlayerAttack;
		RequestClient::doAttack* packet = reinterpret_cast<RequestClient::doAttack*>(nmPlayerAttack.nm_pubMessage);
		packet->type = MSG_ATTACK;
		packet->subType = 0;
		packet->aCharType = pAttackerEntity->GetNetworkType();
		packet->aIndex = pAttackerEntity->GetNetworkID();		
		packet->tCharType = pTargetEntity->GetNetworkType();
		packet->tIndex = pTargetEntity->GetNetworkID();
		packet->multicount = 0;
		packet->list[0].index = 0;

		nmPlayerAttack.setSize( sizeof(*packet) );

		SendToServerNew(nmPlayerAttack);
		//-------------------------------------------------------
	}
}

//-----------------------------------------------------------------------------
// Purpose: Booster 업글.
// Input  : Index - 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendBoosterUpgrade(LONG itemServerIndex)
{
	CNetworkMessage nmMessage;
	RequestClient::doItemAddBooster* packet = reinterpret_cast<RequestClient::doItemAddBooster*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_ADD_BOOSTER;
	packet->boostitem_vitualIndex = itemServerIndex;
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew( nmMessage );
}

//-----------------------------------------------------------------------------
// Purpose: NPC 통해서 가공하기
// Input  : Index - 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendNPCProcessMessage(LONG processItemDBIndex, LONG count)
{
	//MSG_ITEM_PROCESS_NPC,		// NPC를 통해 가공		: processItemDBIndex(n) count(n) errcode(n:s)
	if(!_cmiComm.IsNetworkOn()) return;

	CNetworkMessage nmMessage;
	RequestClient::doItemProcessNpc* packet = reinterpret_cast<RequestClient::doItemProcessNpc*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_PROCESS_NPC;
	packet->itemDBIndex = processItemDBIndex;
	packet->count = count;
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew( nmMessage );
}

//-----------------------------------------------------------------------------
// Purpose: 생산하기.
// Input  : Index - 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendSelectProduceMessage(LONG npc_client_index, LONG item_db_index)
{
	// 생산 관련		: producekind(c) attackchartype(c) attackIndex(n) targetchartype(c) targetindex(n)
	if(!_cmiComm.IsNetworkOn()) return;

	if( item_db_index >= 0 )
	{
		CNetworkMessage nmPlayerProduce((UBYTE)MSG_SELECT_PRODUCE);
		nmPlayerProduce << npc_client_index;
		nmPlayerProduce << item_db_index;
		SendToServerNew(nmPlayerProduce);	
	}
	else
	{
		CNetworkMessage nmPlayerProduce( (UBYTE)MSG_RANDOM_PRODUCE );
		nmPlayerProduce << npc_client_index;
			SendToServerNew(nmPlayerProduce);
	}
}


//---------------------------------------------------------------------->>
// Name : SendSelectProduceMessage()
// Desc : Produce some collect goods
// Date : [7/20/2006] , Wooss
//----------------------------------------------------------------------<<

void CNetworkLibrary::SendCollectProduceMessage(LONG npc_client_index)
{
	if(!_cmiComm.IsNetworkOn()) return;

	CNetworkMessage nmQuest((UBYTE)MSG_QUEST);
	nmQuest << (UBYTE)MSG_QUEST_COLLECT;
	nmQuest << npc_client_index;

	SendToServerNew(nmQuest);

	
}


void CNetworkLibrary::SendProduceMessage(ULONG Index, SBYTE sbKind)
{
	// 생산 관련		: producekind(c) attackchartype(c) attackIndex(n) targetchartype(c) targetindex(n)
/*	if(_cmiComm. IsNetworkOn())
	{		
		CNetworkMessage nmPlayerAttack(MSG_PRODUCE);
		nmPlayerAttack << (UBYTE)sbKind;
		nmPlayerAttack << (UBYTE)MSG_CHAR_PC;						
		nmPlayerAttack << MyCharacterInfo.index;
		nmPlayerAttack << (UBYTE)MSG_CHAR_NPC;
		
		CEntity* penEntity;
		INDEX ipl;
		for(ipl=0; ipl<ga_srvServer.srv_amtMob.Count(); ++ipl) 
		{
			CMobTarget &mt = ga_srvServer.srv_amtMob[ipl];
			if (mt.mob_iClientIndex == Index) 
			{																			
				if (ga_World.EntityExists(mt.mob_iClientIndex,penEntity)) 
				{
					nmPlayerAttack<< mt.mob_Index;
					if(_pNetwork->m_ubGMLevel > 1)
						CPrintF("Send Produce Message : index: %d ClientID: %d \n", mt.mob_Index,Index );
				}
				break;
			}
		}
		SendToServerNew(nmPlayerAttack);	
	}
	*/
}

//-----------------------------------------------------------------------------
// Purpose: 배열내에 있는 모든 엔티티에 대해서 Attack Message를 보냅니다(싱글 모드에서만 사용됨)
// Input  : &dcEntities - 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendAttackMessageInContainer(CSelectedEntities &dcEntities)
{
	if(_pNetwork->m_bSingleMode)
	{	
		for( ENTITIES_ITERATOR it = dcEntities.vectorSelectedEntities.begin();
		it != dcEntities.vectorSelectedEntities.end(); ++it )
		{
			CEntity &en = *(*it);
// EDIT : BS : BEGIN
//			if( en.IsEnemy() || en.IsSlave() || en.IsPet() )
//			{
//				CPlacement3D pl		= en.GetLerpedPlacement();
//				_pNetwork->AddMoveList( 
//					en.GetNetworkID(),
//					pl.pl_PositionVector(1), 
//					pl.pl_PositionVector(3), 
//					pl.pl_PositionVector(2),
//					pl.pl_OrientationAngle(1) );
//			}			
			_pNetwork->AddMoveList(en);
// EDIT : BS : END
		}
		
		_pNetwork->SendMoveList();
	}
	
	if(_cmiComm. IsNetworkOn())
	{
		CNetworkMessage nmPlayerAttack;
		RequestClient::doAttack* packet = reinterpret_cast<RequestClient::doAttack*>(nmPlayerAttack.nm_pubMessage);
		packet->type = MSG_ATTACK;
		packet->subType = 0;
		packet->aCharType = MSG_CHAR_PC;
		packet->aIndex = MyCharacterInfo.index;		
		packet->tCharType = MSG_CHAR_NPC;

		int nMobCount = dcEntities.Count() - 1;
						
		BOOL bFirst = FALSE;
		int i;
		ENTITIES_ITERATOR it = dcEntities.vectorSelectedEntities.begin();		
		// for each entity in container
		for( i = 0; it != dcEntities.vectorSelectedEntities.end(); ++it, i++ )
		{
			CEntity &en = *(*it);
			INDEX iMobIndex = en.GetNetworkID();
			if(!bFirst)
			{
				bFirst = TRUE;					
				packet->tIndex = iMobIndex;
				packet->attackType = 0;
				packet->multicount = nMobCount;
				continue;
			}
			packet->list[i].index = iMobIndex;
		}
		
		int nSize = sizeof(RequestClient::doAttack::tag_multi) * i;	
		nmPlayerAttack.setSize( sizeof(*packet) + nSize );

		SendToServerNew(nmPlayerAttack);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 스킬 사용 메세지를 보내줌.
// Input  : nSkillIndex - 
//			nTargetIndex - 
//			bFire - 
//-----------------------------------------------------------------------------
// FIXME : 코드 정리가 필요함.
void CNetworkLibrary::SendSkillMessage(int nSkillIndex, CEntity *pEntity, int nTargetIndex, BOOL bFire, UBYTE ubMove/*=0*/)
{
	ASSERT( pEntity != NULL && "Invalid Entity Pointer" );
	if( pEntity == NULL )
		return;

	CSkill &SkillData = GetSkillData( nSkillIndex );//0807
	ObjInfo* pInfo = ObjInfo::getSingleton();

	// FIXME : 애완동물을 타고 있는 상태에서 스킬을 쓰려고 할때...
	if( pEntity->IsPlayer() 
		&& _pNetwork->MyCharacterInfo.bPetRide 
		&& SkillData.GetJob() == PET_JOB )
	{	
		SendPetSkillMessage( nSkillIndex, NULL, pEntity, bFire );
		return;
	}

	RequestClient::skillReady* Readypacket = NULL;
	RequestClient::skillFire* Firepacket = NULL;
	CTString strSysMessage;

	if( pEntity->IsPlayer() && pInfo->GetMyApetInfo() != NULL && pInfo->GetMyApetInfo()->GetEntity() && SkillData.GetJob() == WILDPET_JOB )
	{
		CSkill &SkillData = GetSkillData( nSkillIndex );

		CNetworkMessage nmSkill;
		CEntity* pEn = pInfo->GetMyApetInfo()->GetEntity();

		if (pEn != NULL)
		{
			if(bFire)
			{
				Firepacket = reinterpret_cast<RequestClient::skillFire*>(nmSkill.nm_pubMessage);
				Firepacket->type = MSG_SKILL;
				Firepacket->subType = MSG_SKILL_FIRE;

				Firepacket->skillIndex = nSkillIndex;
				Firepacket->cMoveChar = 0;
				Firepacket->charType = MSG_CHAR_WILDPET;
				Firepacket->charIndex = pInfo->GetMyApetInfo()->GetSIndex();
				Firepacket->targetType = pEn->GetNetworkType();
				Firepacket->targetIndex = pInfo->GetMyApetInfo()->GetSIndex();
				Firepacket->listCount = 0;
				nmSkill.setSize( sizeof(*Firepacket) );

				strSysMessage.PrintF( _S( 297, "%s 스킬을 사용합니다." ), SkillData.GetName() );
			}
			else
			{
				if (CheckSendSkill(nSkillIndex) == false)
					return;

				Readypacket = reinterpret_cast<RequestClient::skillReady*>(nmSkill.nm_pubMessage);
				Readypacket->type = MSG_SKILL;
				Readypacket->subType = MSG_SKILL_READY;

				Readypacket->skillIndex = nSkillIndex;
				Readypacket->cMoveChar = 0;
				Readypacket->charType = MSG_CHAR_WILDPET;				
				Readypacket->charIndex = pInfo->GetMyApetInfo()->GetSIndex();
				Readypacket->targetType = pEn->GetNetworkType();
				Readypacket->targetIndex = pInfo->GetMyApetInfo()->GetSIndex();
				Readypacket->nDummySkillSpeed = 0;
				nmSkill.setSize( sizeof(*Readypacket) );

				strSysMessage.PrintF( _S( 298, "%s 스킬을 스펠합니다." ), SkillData.GetName() );
			}

			ClientSystemMessage( strSysMessage);
			
			SendToServerNew(nmSkill);
		}
		return;
	}	

	if(_cmiComm. IsNetworkOn())
	{	
		if( _pNetwork->m_bSingleMode && pEntity->IsEnemy() )
		{
			CPlacement3D pl		= pEntity->GetLerpedPlacement();
			_pNetwork->AddMoveList( 
				pEntity->GetNetworkID(),
				pl.pl_PositionVector(1), 
				pl.pl_PositionVector(3), 
				pl.pl_PositionVector(2),
				pl.pl_OrientationAngle(1) );		
			_pNetwork->SendMoveList();
		}

		CEntity *penPlEntity = NULL;
		CPlayerEntity *penPlayerEntity = NULL;
		FLOAT3D plVector = FLOAT3D( 0.0f, 0.0f, 0.0f );
		ANGLE3D plAngle = ANGLE3D( 0.0f, 0.0f, 0.0f );
		SBYTE sbLayer = 0;

		if ( nSkillIndex == 401 && ubMove == 1 ) // 대쉬
		{
			penPlEntity = CEntity::GetPlayerEntity(0);
			penPlayerEntity = (CPlayerEntity*)penPlEntity;
			CPlacement3D pl = penPlayerEntity->GetPlacement();

			plVector = pl.pl_PositionVector;
			plAngle = pl.pl_OrientationAngle;

			int		i;

			for (i = 0; i < eOBJ_MAX; ++i)
			{
				ObjectBase* pObject = ACTORMGR()->GetObject((eOBJ_TYPE)i, nTargetIndex);

				if (pObject != NULL)
				{
					sbLayer = pObject->GetyLayer();
					break;
				}
			}
		}
				
		CNetworkMessage nmPlayerSkill;

		if(bFire)
		{
#if defined (G_USA)
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetSkillCancel(FALSE);
#endif
			Firepacket = reinterpret_cast<RequestClient::skillFire*>(nmPlayerSkill.nm_pubMessage);
			Firepacket->type = MSG_SKILL;
			Firepacket->subType = MSG_SKILL_FIRE;

			Firepacket->skillIndex = nSkillIndex;
			Firepacket->charType = MSG_CHAR_PC;
			Firepacket->charIndex = MyCharacterInfo.index;
			Firepacket->targetType = pEntity->GetNetworkType();
			Firepacket->targetIndex = nTargetIndex;
			Firepacket->listCount = 0;
			Firepacket->nDummySkillSpeed = 0;
			Firepacket->cMoveChar = ubMove;
			Firepacket->fx = plVector(1);
			Firepacket->fz = plVector(3);
			Firepacket->fh = plVector(2);
			Firepacket->fr = plAngle(1);
			Firepacket->cYlayer = sbLayer;

			nmPlayerSkill.setSize( sizeof(*Firepacket) );
			
			strSysMessage.PrintF( _S( 297, "%s 스킬을 사용합니다." ), SkillData.GetName() );
		}
		else
		{
			if (CheckSendSkill(nSkillIndex) == false)
				return;

#if defined (G_USA)
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetSkillCancel(TRUE);
#endif
			Readypacket = reinterpret_cast<RequestClient::skillReady*>(nmPlayerSkill.nm_pubMessage);
			Readypacket->type = MSG_SKILL;
			Readypacket->subType = MSG_SKILL_READY;

			Readypacket->skillIndex = nSkillIndex;
			Readypacket->charType = MSG_CHAR_PC;
			Readypacket->charIndex = MyCharacterInfo.index;
			Readypacket->targetType = pEntity->GetNetworkType();
			Readypacket->targetIndex = nTargetIndex;
			Readypacket->nDummySkillSpeed = 0;
			Readypacket->cMoveChar = ubMove;
			Readypacket->fx = plVector(1);
			Readypacket->fz = plVector(3);
			Readypacket->fh = plVector(2);
			Readypacket->fr = plAngle(1);
			Readypacket->cYlayer = sbLayer;
			nmPlayerSkill.setSize( sizeof(*Readypacket) );

			CUIManager::getSingleton()->SetCSFlagOn(CSF_SKILLREADY);
			strSysMessage.PrintF( _S( 298, "%s 스킬을 스펠합니다." ), SkillData.GetName() );
		}
		
		ClientSystemMessage( strSysMessage);

		SendToServerNew(nmPlayerSkill);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : 
//			
//-----------------------------------------------------------------------------
// FIXME : 코드 정리가 필요함.
// FIXME : SendSkillMessage 계열들 정리 필요.
void CNetworkLibrary::SendPetSkillMessage(int nSkillIndex, CEntity *pSourceEntity, CEntity *pTargetEntity, BOOL bFire)
{	
	CSkill &SkillData = GetSkillData( nSkillIndex );
	CTString strSysMessage;

	CNetworkMessage nmPetSkill;
	RequestClient::skillReady* Readypacket = NULL;
	RequestClient::skillFire* Firepacket = NULL;
	CPetTargetInfom* pPetInfo = INFO()->GetMyPetInfo();

	if(bFire)
	{
		Firepacket = reinterpret_cast<RequestClient::skillFire*>(nmPetSkill.nm_pubMessage);
		Firepacket->type = MSG_SKILL;
		Firepacket->subType = MSG_SKILL_FIRE;

		Firepacket->skillIndex = nSkillIndex;
		Firepacket->cMoveChar = 0;
		Firepacket->charType = MSG_CHAR_PET;
		Firepacket->charIndex = pPetInfo->lIndex;
		Firepacket->targetType = pTargetEntity->GetNetworkType();
		Firepacket->targetIndex = pTargetEntity->GetNetworkID();
		Firepacket->listCount = 0;
		nmPetSkill.setSize( sizeof(*Firepacket) );

		strSysMessage.PrintF( _S( 297, "%s 스킬을 사용합니다." ), SkillData.GetName() );
	}
	else
	{
		if (CheckSendSkill(nSkillIndex) == false)
			return;

		Readypacket = reinterpret_cast<RequestClient::skillReady*>(nmPetSkill.nm_pubMessage);
		Readypacket->type = MSG_SKILL;
		Readypacket->subType = MSG_SKILL_READY;

		Readypacket->skillIndex = nSkillIndex;
		Readypacket->cMoveChar = 0;
		Readypacket->charType = MSG_CHAR_PET;
		Readypacket->charIndex = pPetInfo->lIndex;
		Readypacket->targetType = pTargetEntity->GetNetworkType();
		Readypacket->targetIndex = pTargetEntity->GetNetworkID();
		Readypacket->nDummySkillSpeed = 0;
		nmPetSkill.setSize( sizeof(*Readypacket) );

		strSysMessage.PrintF( _S( 298, "%s 스킬을 스펠합니다." ), SkillData.GetName() );
	}

	ClientSystemMessage( strSysMessage);

	SendToServerNew(nmPetSkill);
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : 
//			
//-----------------------------------------------------------------------------
// FIXME : 코드 정리가 필요함.
// FIXME : SendSkillMessage 계열들 정리 필요.
void CNetworkLibrary::SendSlaveSkillMessage(int nSkillIndex, CEntity *pSourceEntity, CEntity *pTargetEntity, BOOL bFire)
{
	CSkill &SkillData = GetSkillData( nSkillIndex );
	CTString strSysMessage;

	CNetworkMessage nmSkill;
	RequestClient::skillReady* Readypacket = NULL;
	RequestClient::skillFire* Firepacket = NULL;

	if(bFire)
	{
		Firepacket = reinterpret_cast<RequestClient::skillFire*>(nmSkill.nm_pubMessage);
		Firepacket->type = MSG_SKILL;
		Firepacket->subType = MSG_SKILL_FIRE;

		Firepacket->skillIndex = nSkillIndex;
		Firepacket->cMoveChar = 0;
		Firepacket->charType = pSourceEntity->GetNetworkType();
		Firepacket->charIndex = pSourceEntity->GetNetworkID();
		Firepacket->targetType = pTargetEntity->GetNetworkType();
		Firepacket->targetIndex = pTargetEntity->GetNetworkID();
		Firepacket->listCount = 0;
		nmSkill.setSize( sizeof(*Firepacket) );

		strSysMessage.PrintF( _S( 297, "%s 스킬을 사용합니다." ), SkillData.GetName() );
	}
	else
	{
		if (CheckSendSkill(nSkillIndex) == false)
			return;

		Readypacket = reinterpret_cast<RequestClient::skillReady*>(nmSkill.nm_pubMessage);
		Readypacket->type = MSG_SKILL;
		Readypacket->subType = MSG_SKILL_READY;

		Readypacket->skillIndex = nSkillIndex;
		Readypacket->cMoveChar = 0;
		Readypacket->charType = pSourceEntity->GetNetworkType();
		Readypacket->charIndex = pSourceEntity->GetNetworkID();
		Readypacket->targetType = pTargetEntity->GetNetworkType();
		Readypacket->targetIndex = pTargetEntity->GetNetworkID();
		Readypacket->nDummySkillSpeed = 0;
		nmSkill.setSize( sizeof(*Readypacket) );

		strSysMessage.PrintF( _S( 298, "%s 스킬을 스펠합니다." ), SkillData.GetName() );
	}

	ClientSystemMessage( strSysMessage);

	SendToServerNew(nmSkill);
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendPetCommandMessage( int nSkillIndex, CEntity *pSourceEntity )
{
	// MSG_EX_PET_COMMAND,			// 펫 사교동작		: pet_index(n) command_skill_index(n) targettype(c) targetindex(n)
	ASSERT( pSourceEntity != NULL && "Invalid Entity Pointer" );
	if( pSourceEntity == NULL )
		return;	

	if( !pSourceEntity->IsPet() )
		return;

	CSkill &SkillData = _pNetwork->GetSkillData(nSkillIndex);

	const int iSkillType = SkillData.GetType();

	// 펫 커맨드 스킬만!!!
	if( iSkillType != CSkill::ST_PET_COMMAND )
	{
		return;
	}

	CNetworkMessage nmPet( (UBYTE)MSG_EXTEND );	
	nmPet << (LONG)MSG_EX_PET_COMMAND;
	nmPet << pSourceEntity->GetNetworkID();	
	nmPet << (LONG)nSkillIndex;
	nmPet << pSourceEntity->GetNetworkType();
	nmPet << pSourceEntity->GetNetworkID();
	SendToServerNew(nmPet);
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : nSkillIndex - 
//			&dcEntities - 
//-----------------------------------------------------------------------------
// FIXME : 코드 정리가 필요함.
// FIXME : SendSlaveSkillMessageInContainer() 소화수의 스킬 메세지.
void CNetworkLibrary::SendSkillMessageInContainer(int nSkillIndex, CSelectedEntities &dcEntities, BOOL bFire)
{
	INDEX iMobCount	= dcEntities.Count();

	CPrintF( "Send Skill Attack : %d\n", iMobCount );
	if(_cmiComm. IsNetworkOn() && iMobCount > 0)
	{
		if(_pNetwork->m_bSingleMode)
		{
			for( ENTITIES_ITERATOR it = dcEntities.vectorSelectedEntities.begin();
				it != dcEntities.vectorSelectedEntities.end(); ++it )
			{
				CEntity &en = *(*it);
				_pNetwork->AddMoveList(en);
			}
			_pNetwork->SendMoveList();		
		}

		CSkill &SkillData = GetSkillData( nSkillIndex );//0807
		CTString strSysMessage;
		RequestClient::skillReady* Readypacket = NULL;
		RequestClient::skillFire* Firepacket = NULL;

		CNetworkMessage nmPlayerSkill;

		UBYTE ubCharType = (UBYTE)MSG_CHAR_PC;		
		int CharIndex;
		// FIXME : 펫을 타고 있는 경우.
		if( _pNetwork->MyCharacterInfo.bPetRide )
		{
			ubCharType = (UBYTE)MSG_CHAR_PET;
			CharIndex = MY_PET_INFO()->lIndex;
		}
		else
		{
			ubCharType = (UBYTE)MSG_CHAR_PC;
			CharIndex = MyCharacterInfo.index;
		}

		ENTITIES_ITERATOR it = dcEntities.vectorSelectedEntities.begin();
		ENTITIES_ITERATOR it_End = dcEntities.vectorSelectedEntities.end();
		INDEX iIndex		= -1;
		INDEX nTargetType	= 1;

		if(bFire)
		{
#if defined (G_USA)
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetSkillCancel(FALSE);
#endif
			Firepacket = reinterpret_cast<RequestClient::skillFire*>(nmPlayerSkill.nm_pubMessage);
			Firepacket->type = MSG_SKILL;
			Firepacket->subType = MSG_SKILL_FIRE;

			Firepacket->skillIndex = nSkillIndex;
			Firepacket->charType = ubCharType;
			Firepacket->charIndex = CharIndex;
			Firepacket->cMoveChar = 0;

			BOOL bFirst = FALSE;
			int i, Size;
			for( i = 0; it != it_End; ++it)
			{
				CEntity &en			= *(*it);
				nTargetType			= en.GetNetworkType();
				iIndex				= en.GetNetworkID();

				if(!bFirst)
				{
					bFirst = TRUE;					
					Firepacket->targetType = nTargetType;
					Firepacket->targetIndex = iIndex;
					Firepacket->listCount = iMobCount - 1;// 타겟의 갯수.
					continue;
				}
				Firepacket->list[i].mtargettype = nTargetType;
				Firepacket->list[i].mtargetindex = iIndex;
				++i;
			}
			Size = sizeof(RequestClient::skillFire::tag_list) * i;
			nmPlayerSkill.setSize( sizeof(*Firepacket) + Size );
			
			strSysMessage.PrintF( _S( 297, "%s 스킬을 사용합니다." ), SkillData.GetName() );
		}
		else
		{
			if (CheckSendSkill(nSkillIndex) == false)
				return;

#if defined (G_USA)
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetSkillCancel(TRUE);
#endif
			Readypacket = reinterpret_cast<RequestClient::skillReady*>(nmPlayerSkill.nm_pubMessage);
			Readypacket->type = MSG_SKILL;
			Readypacket->subType = MSG_SKILL_READY;

			Readypacket->skillIndex = nSkillIndex;
			Readypacket->cMoveChar = 0;
			Readypacket->charType = ubCharType;
			Readypacket->charIndex = CharIndex;

			for( ; it != it_End; ++it )
			{
				CEntity &en			= *(*it);
				nTargetType			= en.GetNetworkType();
				iIndex				= en.GetNetworkID();

				Readypacket->targetType = nTargetType;
				Readypacket->targetIndex = iIndex;
			}			
					
			nmPlayerSkill.setSize( sizeof(*Readypacket) );

			CUIManager::getSingleton()->SetCSFlagOn(CSF_SKILLREADY);
			strSysMessage.PrintF( _S( 298, "%s 스킬을 스펠합니다." ), SkillData.GetName() );
		}

		ClientSystemMessage( strSysMessage);

		SendToServerNew(nmPlayerSkill);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : nSkillIndex - 
//			&dcEntities - 
//-----------------------------------------------------------------------------
// FIXME : 코드 정리가 필요함.
// FIXME : SendSlaveSkillMessageInContainer() 소화수의 스킬 메세지.
void CNetworkLibrary::SendSlaveSkillMessageInContainer(int nSkillIndex, CEntity *pSourceEntity, CSelectedEntities &dcEntities, BOOL bFire)
{
	if( pSourceEntity == NULL )
		return;

	INDEX iMobCount	= dcEntities.Count();

	CPrintF( "Send Skill Attack : %d\n", iMobCount );
	if(_cmiComm. IsNetworkOn() && iMobCount > 0)
	{
		if(_pNetwork->m_bSingleMode)
		{
			for( ENTITIES_ITERATOR it = dcEntities.vectorSelectedEntities.begin();
				it != dcEntities.vectorSelectedEntities.end(); ++it )
			{
				CEntity &en = *(*it);
				_pNetwork->AddMoveList(en);
			}
			_pNetwork->SendMoveList();		
		}

		CSkill &SkillData = GetSkillData( nSkillIndex );//0807
		CTString strSysMessage;
		RequestClient::skillReady* Readypacket = NULL;
		RequestClient::skillFire* Firepacket = NULL;

		CNetworkMessage nmPlayerSkill;
				
		ENTITIES_ITERATOR it = dcEntities.vectorSelectedEntities.begin();
		ENTITIES_ITERATOR it_End = dcEntities.vectorSelectedEntities.end();
		INDEX iIndex		= -1;
		INDEX nTargetType	= 1;

		if(bFire)
		{
#if defined (G_USA)
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetSkillCancel(FALSE);
#endif
			Firepacket = reinterpret_cast<RequestClient::skillFire*>(nmPlayerSkill.nm_pubMessage);
			Firepacket->type = MSG_SKILL;
			Firepacket->subType = MSG_SKILL_FIRE;

			Firepacket->skillIndex = nSkillIndex;
			Firepacket->cMoveChar = 0;
			Firepacket->charType = pSourceEntity->GetNetworkType();
			Firepacket->charIndex = pSourceEntity->GetNetworkID();			

			BOOL bFirst = FALSE;
			int i, Size;
			for( i = 0; it != it_End; ++it)
			{
				CEntity &en			= *(*it);
				nTargetType			= en.GetNetworkType();
				iIndex				= en.GetNetworkID();

				if(!bFirst)
				{
					bFirst = TRUE;					
					Firepacket->targetType = nTargetType;
					Firepacket->targetIndex = iIndex;
					Firepacket->listCount = iMobCount - 1;// 타겟의 갯수.
					continue;
				}
				Firepacket->list[i].mtargettype = nTargetType;
				Firepacket->list[i].mtargetindex = iIndex;
				++i;
			}
			Size = sizeof(RequestClient::skillFire::tag_list) * i;
			nmPlayerSkill.setSize( sizeof(*Firepacket) + Size );

			strSysMessage.PrintF( _S( 297, "%s 스킬을 사용합니다." ), SkillData.GetName() );
		}
		else
		{
			if (CheckSendSkill(nSkillIndex) == false)
				return;

#if defined (G_USA)
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetSkillCancel(TRUE);
#endif
			Readypacket = reinterpret_cast<RequestClient::skillReady*>(nmPlayerSkill.nm_pubMessage);
			Readypacket->type = MSG_SKILL;
			Readypacket->subType = MSG_SKILL_READY;

			Readypacket->skillIndex = nSkillIndex;
			Readypacket->cMoveChar = 0;
			Readypacket->charType = pSourceEntity->GetNetworkType();
			Readypacket->charIndex = pSourceEntity->GetNetworkID();

			for( ; it != it_End; ++it )
			{
				CEntity &en			= *(*it);
				nTargetType			= en.GetNetworkType();
				iIndex				= en.GetNetworkID();

				Readypacket->targetType = nTargetType;
				Readypacket->targetIndex = iIndex;
			}			

			nmPlayerSkill.setSize( sizeof(*Readypacket) );

			strSysMessage.PrintF( _S( 298, "%s 스킬을 스펠합니다." ), SkillData.GetName() );
		}

		ClientSystemMessage( strSysMessage);

		SendToServerNew(nmPlayerSkill);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendCancelSkillMessage()
{
	UBYTE ubCharType;
	int nIdex;
	// FIXME : 펫을 타고 있는 경우.
	if( _pNetwork->MyCharacterInfo.bPetRide )
	{
		nIdex = MY_PET_INFO()->lIndex;
		ubCharType = (UBYTE)MSG_CHAR_PET;
	}
	else
	{
		nIdex = MyCharacterInfo.index;
		ubCharType = (UBYTE)MSG_CHAR_PC;
	}

	CNetworkMessage nmPlayerSkill;
	RequestClient::skillCancel* packet = reinterpret_cast<RequestClient::skillCancel*>(nmPlayerSkill.nm_pubMessage);
	packet->type = MSG_SKILL;
	packet->subType = MSG_SKILL_CANCEL;
	packet->charType = ubCharType;
	packet->charIndex = nIdex;

	nmPlayerSkill.setSize( sizeof(*packet) );
	SendToServerNew(nmPlayerSkill);
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : plPlacement - 
//			camera_angle - 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SetMyPosition(CPlacement3D plPlacement, FLOAT camera_angle)
{
	MyCharacterInfo.x = plPlacement.pl_PositionVector(1);
	MyCharacterInfo.h = plPlacement.pl_PositionVector(2);
	MyCharacterInfo.z = plPlacement.pl_PositionVector(3);
	MyCharacterInfo.r = plPlacement.pl_OrientationAngle(1);
	MyCharacterInfo.camera_angle = camera_angle;
}

void CNetworkLibrary::ClientSystemMessage( CTString &strSysMessage, int nSysType )
{
	CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( strSysMessage, nSysType );
}

void CNetworkLibrary::SendRebirthMessage()
{	
	UIMGR()->SetCSFlagOn(CSF_TELEPORT);

	if(_cmiComm. IsNetworkOn())
	{
		CNetworkMessage nmRebirth((UBYTE)MSG_PC_REBIRTH); 			
		SendToServerNew(nmRebirth);	
	}
}

//wooss 050805
//확장된 리버스 메시지 아이템 사용여부와 부활 장소 
void CNetworkLibrary::SendRebirthMessageEx(int nIndex ,BOOL bUse ,BOOL bHere)
{	
	UIMGR()->SetCSFlagOn(CSF_TELEPORT);

	if(_cmiComm. IsNetworkOn())
	{
		CNetworkMessage nmRebirth((UBYTE)MSG_PC_REBIRTH);
		nmRebirth << (ULONG)nIndex;
		nmRebirth << (UBYTE)bUse;
		nmRebirth << (UBYTE)bHere;
		SendToServerNew(nmRebirth);	
	}
}

//wooss 050808
void CNetworkLibrary::SendWarpItemMessage(UBYTE nmWarpType, CTString sCharName,BOOL bAllow)
{
	CNetworkMessage nmWarp((UBYTE)MSG_WARP);

	switch(nmWarpType)
	{
		case MSG_WARP_TO_REQ :
			nmWarp << (UBYTE)MSG_WARP_TO_REQ;
			nmWarp << sCharName;
			SendToServerNew(nmWarp);
			break;

		case MSG_WARP_TAKE_REQ :
			nmWarp << (UBYTE)MSG_WARP_TAKE_REQ;
			nmWarp << sCharName;
			SendToServerNew(nmWarp);
			break;

		case MSG_WARP_TO :
		
			nmWarp << (UBYTE)MSG_WARP_TO;
			nmWarp << sCharName;
			nmWarp << (UBYTE)bAllow;
			SendToServerNew(nmWarp);
			break;

		case MSG_WARP_TAKE :
		
			nmWarp << (UBYTE)MSG_WARP_TAKE;
			nmWarp << sCharName;
			nmWarp << (UBYTE)bAllow;
			SendToServerNew(nmWarp);
			break;

		case MSG_WARP_CANCEL :
			_pNetwork->SendWarpCancel();
			break;

		case MSG_WARP_TO_REQING :
			nmWarp << (UBYTE)MSG_WARP_TO_REQING;
			nmWarp << sCharName;
			SendToServerNew(nmWarp);
			break;
		case MSG_WARP_TAKE_REQING :
			nmWarp << (UBYTE)MSG_WARP_TAKE_REQING;
			nmWarp << sCharName;
			SendToServerNew(nmWarp);
			break;
	
	}
}

void CNetworkLibrary::DeleteAllMob()
{
	ACTORMGR()->RemoveAll();
}

// ----------------------------------------------------------------------------
// Name : SendActionMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendActionMessage(SBYTE action_type, SBYTE action_id, SBYTE state, CEntityPointer epTarget )
{
	if(_cmiComm. IsNetworkOn())
	{
		int targetIdx = 0;
		CNetworkMessage nmAction;
		RequestClient::action* packet = reinterpret_cast<RequestClient::action*>(nmAction.nm_pubMessage);
		packet->type = MSG_ACTION;
		packet->subType = 0;
		packet->typevalue = action_type;
		packet->index = action_id;

		//물뿌리기 액션일 때는 타겟 인덱스 전달
		if( action_id ==42 && epTarget )
			targetIdx = epTarget->GetNetworkID();

		packet->targetIndex = targetIdx;

		nmAction.setSize( sizeof(*packet) );

		SendToServerNew(nmAction);		
	}
}

// ----------------------------------------------------------------------------
// Name : SendQuestMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendQuestMessage(UBYTE msgQuestType, INDEX data)
{
	CNetworkMessage nmQuest((UBYTE)MSG_QUEST);
	nmQuest << (UBYTE)msgQuestType;
	nmQuest << SLONG(data);

	SendToServerNew(nmQuest);
}

// ----------------------------------------------------------------------------
// Name : SendQuestPrizeMessage()
// Desc : 퀘스트 보상 메시지.
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendQuestPrizeMessage(UBYTE msgType, INDEX iQuestIndex, INDEX iNpcIndex, INDEX iOptionItemIndex, INDEX iOptionItemPlust)
{
	CNetworkMessage nmQuest((UBYTE)MSG_QUEST);
	nmQuest << (UBYTE)MSG_QUEST_PRIZE;
	nmQuest << SLONG(iQuestIndex);
	nmQuest << SLONG(iNpcIndex);
	nmQuest << SLONG(iOptionItemIndex);
	nmQuest << SLONG(iOptionItemPlust);

	SendToServerNew(nmQuest);
}

void CNetworkLibrary::SendUseStatPoint( UBYTE ubStatType )
{
	CNetworkMessage nmStat((UBYTE)MSG_STATPOINT);
	nmStat << (UBYTE)MSG_STATPOINT_USE;
	nmStat << ubStatType;

	SendToServerNew(nmStat);
}

//-----------------------------------------------------------------------------
// Purpose: 일반 스킬 배우기
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendSkillLearn( SLONG slIndex, int NpcVirIdx )
{
	CNetworkMessage nmSkillLearn;
	RequestClient::skillLearn* packet = reinterpret_cast<RequestClient::skillLearn*>(nmSkillLearn.nm_pubMessage);
	packet->type = MSG_SKILL;
	packet->subType = MSG_SKILL_LEARN;
	packet->npcIndex = NpcVirIdx;
	packet->skillIndex = slIndex;
	
	nmSkillLearn.setSize( sizeof(*packet) );
	SendToServerNew(nmSkillLearn);
}

//-----------------------------------------------------------------------------
// Purpose: 특수 스킬 배우기
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendSSkillLearn( SLONG slIndex, int NpcVirIdx )
{
	CNetworkMessage nmSSkillLearn;
	RequestClient::sskill* packet = reinterpret_cast<RequestClient::sskill*>(nmSSkillLearn.nm_pubMessage);
	packet->type = MSG_SSKILL;
	packet->subType = MSG_SSKILL_LEARN;
	packet->sskillIndex = slIndex;
	packet->npcIndex = NpcVirIdx;
	
	nmSSkillLearn.setSize( sizeof(RequestClient::sskill) );
	SendToServerNew(nmSSkillLearn);
}

void CNetworkLibrary::SendTeleportWrite( UBYTE sendMSG, UBYTE ubSlot, CTString &strComment )
{
	CNetworkMessage nmMemPos;
	RequestClient::memposWrite* packet = reinterpret_cast<RequestClient::memposWrite*>(nmMemPos.nm_pubMessage);
	packet->type = sendMSG;
	packet->subType = MSG_MEMPOS_WRITE;
	packet->slot = ubSlot;
	memcpy(packet->comment, strComment.str_String, MEMPOS_COMMENT_LENGTH + 1);
	nmMemPos.setSize( sizeof(*packet) );

	_pNetwork->SendToServerNew(nmMemPos);
}

// [KH_070316] 프리미엄 메모리 관련 변경 ( UBYTE ubSlot ) -> ( UBYTE sendMSG, UBYTE ubSlot )
void CNetworkLibrary::SendTeleportMove( UBYTE sendMSG, UBYTE ubSlot )
{
	CNetworkMessage nmMemPos;
	RequestClient::memposWrite* packet = reinterpret_cast<RequestClient::memposWrite*>(nmMemPos.nm_pubMessage);
	packet->type = sendMSG;
	packet->subType = MSG_MEMPOS_MOVE;
	packet->slot = ubSlot;
	nmMemPos.setSize( sizeof(*packet) );

	_pNetwork->SendToServerNew(nmMemPos);
}

void CNetworkLibrary::SendWarpTeleport( int iTeleportIndex )
{
	LONG lTeleportIndex = iTeleportIndex;
	CNetworkMessage nmWarp( (UBYTE)MSG_WARP );
	nmWarp << (UBYTE)MSG_WARP_TELEPORT;
	nmWarp << lTeleportIndex;
	SendToServerNew( nmWarp );
}

void CNetworkLibrary::SendWarpCancel()
{
	CNetworkMessage nmWarp( (UBYTE)MSG_WARP );
	nmWarp << (UBYTE)MSG_WARP_CANCEL;

	SendToServerNew( nmWarp );
}

//-----------------------------------------------------------------------------
// Purpose: HP와 MP의 패널티 회복
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendRecoverHPMP( SBYTE sbHPCount, SBYTE sbMPCount )
{
	CNetworkMessage nmRecoverHP( (UBYTE)MSG_PK );
	nmRecoverHP << (UBYTE)MSG_PK_RECOVER_HPMP;
	nmRecoverHP << sbHPCount;
	nmRecoverHP << sbMPCount;
	SendToServerNew( nmRecoverHP );
}

//-----------------------------------------------------------------------------
// Purpose: 아이템 봉인 해제
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendRecoverItemSeal( SWORD nTab, SWORD nIdx, SLONG slIndex)
{
	CNetworkMessage nmMessage;
	RequestClient::doPKRecoverItemSealed* packet = reinterpret_cast<RequestClient::doPKRecoverItemSealed*>(nmMessage.nm_pubMessage);
	packet->type = MSG_PK;
	packet->subType = MSG_PK_RECOVER_ITEMSEAL;
	packet->tab = nTab;
	packet->invenIndex = nIdx;
	packet->virtualIndex = slIndex;
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew( nmMessage );
}

//-----------------------------------------------------------------------------
// Purpose: 이벤트 보상받기
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendEventPrize()
{
	CNetworkMessage nmEventPrize( (UBYTE)MSG_EVENT );

	nmEventPrize << (UBYTE)MSG_EVENT_LATTO;
	nmEventPrize << (UBYTE)MSG_EVENT_LATTO_CHANGE_LUCKYBAG_REQ;
	
	SendToServerNew( nmEventPrize );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendEventTreasureList()
{
	CNetworkMessage nmEvent( (UBYTE)MSG_EVENT );
	nmEvent << (UBYTE)MSG_EVENT_TREASUREBOX;
	nmEvent << (UBYTE)MSG_EVENT_TREASUREBOX_TRY_REQ;
	nmEvent << _pNetwork->MyCharacterInfo.index;
	_pNetwork->SendToServerNew( nmEvent );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendEventOpenTreasure()
{
	CNetworkMessage nmEvent( (UBYTE)MSG_EVENT );
	nmEvent << (UBYTE)MSG_EVENT_TREASUREBOX;
	nmEvent << (UBYTE)MSG_EVENT_TREASUREBOX_OPEN_REQ;
	nmEvent << _pNetwork->MyCharacterInfo.index;
	_pNetwork->SendToServerNew( nmEvent );
}

void CNetworkLibrary::SendEventNewyear(int tv_event)
{
	CNetworkMessage nmEvent( (UBYTE)MSG_EVENT );

	switch(tv_event)
	{
		case EVENT_NEWYEAR1_REWARD:
			nmEvent << (UBYTE)MSG_EVENT_XMAS_2005;
			_pNetwork->SendToServerNew( nmEvent );
			
			break;
	
		case EVENT_NEWYEAR2_TIMECHECK:
			nmEvent << (UBYTE)MSG_EVENT_NEWYEAR_2006_CHECK;
			_pNetwork->SendToServerNew( nmEvent );

			break;

		case EVENT_NEWYEAR2_REWARD:
			nmEvent << (UBYTE)MSG_EVENT_NEWYEAR_2006_GOODS;
			_pNetwork->SendToServerNew( nmEvent );

			break;
	}
}

// [KH_070413] 스승의날 이벤트 관련 추가
void CNetworkLibrary::SendEventMaster()
{
	CNetworkMessage nmEvent( (UBYTE)MSG_EVENT );
	nmEvent << (UBYTE)MSG_EVENT_TEACH_2007;
	_pNetwork->SendToServerNew( nmEvent );
}

void CNetworkLibrary::SendFindFriend(int tv_event,void * strInput)
{
	CNetworkMessage nmEvent( (UBYTE)MSG_EVENT );
	nmEvent << (UBYTE)tv_event;
	switch(tv_event)
	{
		case MSG_EVENT_SEARCHFRIEND_ADD_CHECK :
			nmEvent << *((CTString *)strInput);
			break;
		case MSG_EVENT_SEARCHFRIEND_ADDSELECT_CHECK :	
			{
				ULONG tv_idx = *((ULONG*)strInput);
				nmEvent << tv_idx;
			}
			break;
		case MSG_EVENT_SEARCHFRIEND_TIME_CHECK :	
			break;
		case MSG_EVENT_SEARCHFRIEND_GOODS_CHECK :
			{
				ULONG tv_idx = *((ULONG*)strInput);
				nmEvent << tv_idx;
			}

			break;
	}
	_pNetwork->SendToServerNew( nmEvent );		

}

void CNetworkLibrary::DelMobTarget(ULONG ClientIndex)
{
	// 서버 인덱스를 갖고 있다면, 그것으로 지우자!!
	ObjectBase* pObject = ACTORMGR()->GetObjectByCIndex(eOBJ_MOB, ClientIndex);

	if (pObject != NULL)
	{
		//타겟 이펙트 없앰...
		CUIManager::getSingleton()->StopTargetEffect( pObject->GetSIndex() );
		ACTORMGR()->RemoveObject(eOBJ_MOB, pObject->GetSIndex());
	}
}

void CNetworkLibrary::DelChaTarget(ULONG ClientIndex)
{
	ObjectBase* pObject = ACTORMGR()->GetObjectByCIndex(eOBJ_CHARACTER, ClientIndex);

	if (pObject != NULL)
	{
		//타겟 이펙트 없앰...
		CUIManager::getSingleton()->StopTargetEffect( pObject->GetSIndex() );
		ACTORMGR()->RemoveObject(eOBJ_CHARACTER, pObject->GetSIndex());
	}
}

void CNetworkLibrary::DelWildPetTarget(ULONG ulSIndex)
{
	ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_WILDPET, ulSIndex);

	if (pObject != NULL)
	{
		CWildPetTarget* pTarget = static_cast< CWildPetTarget* >(pObject);

		CUIManager* pUIManager = CUIManager::getSingleton();

		//타겟 이펙트 없앰...
		pUIManager->StopTargetEffect(pTarget->GetSIndex());

		if (pTarget->m_nOwnerIndex == _pNetwork->MyCharacterInfo.index)
		{
			pUIManager->RearrangeOrder(UI_WILDPETTARGETINFO,FALSE);
			INFO()->SetMyApet(NULL);
			pUIManager->GetWildPetInfo()->AIClear();
			pUIManager->GetQuickSlot()->RemoveWildPetSkill();
		}

		ACTORMGR()->RemoveObject(eOBJ_WILDPET, pObject->GetSIndex());
	}
}

void CNetworkLibrary::ResetMobStatus(ULONG ClientIndex)
{
	ObjectBase* pObject = ACTORMGR()->GetObjectByCIndex(eOBJ_MOB, ClientIndex);
	
	if (pObject != NULL)
	{
		CMobTarget* pTarget = static_cast< CMobTarget* >(pObject);
		pTarget->ResetStatus();
	}
}

void CNetworkLibrary::ResetChaStatus(ULONG ClientIndex)
{
	ObjectBase* pObject = ACTORMGR()->GetObjectByCIndex(eOBJ_CHARACTER, ClientIndex);

	if (pObject != NULL)
	{
		CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);
		pTarget->ResetStatus();
	}
}

// Ranking 관련
//-----------------------------------------------------------------------------
// Purpose: 랭킹 리스트를 요청함.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::Ranking_RequestList( SBYTE sbJob )
{
	SBYTE sbReqJob	= sbJob;
	CNetworkMessage nmQuest((UBYTE)MSG_QUEST);
	nmQuest << (UBYTE)MSG_QUEST_PD4_RANK_VIEW_REQ;
	nmQuest << sbReqJob;
	_pNetwork->SendToServerNew(nmQuest);
}

//-----------------------------------------------------------------------------
// Purpose: 보상 리스트를 요청함.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::Ranking_RequestPrizeList()
{
	CNetworkMessage nmQuest((UBYTE)MSG_QUEST);
	nmQuest << (UBYTE)MSG_QUEST_PD4_RANK_REWARD_RANK_REQ;	
	_pNetwork->SendToServerNew(nmQuest);
}

//-----------------------------------------------------------------------------
// Purpose: 보상을 요청함.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::Ranking_Prize()
{
	CNetworkMessage nmQuest((UBYTE)MSG_QUEST);
	nmQuest << (UBYTE)MSG_QUEST_PD4_REWARD;	
	_pNetwork->SendToServerNew(nmQuest);	
}

//-----------------------------------------------------------------------------
// Purpose: 직업과 해당 무기의 아이템 정보를 갖고서, 무기가 2번째 타입(전직)인지를 체크
//-----------------------------------------------------------------------------
BOOL CNetworkLibrary::IsExtensionState( int iJob, CItemData& ID )
{
	if( ID.GetType() == CItemData::ITEM_WEAPON )
	{
		if( ID.GetSubType() == CJobInfo::getSingleton()->GetSkillWeponType( iJob, 1 ) )
			return TRUE;		
	}
	return FALSE;
}

// Messenger...
//------------------------------------------------------------------------------
// CNetworkLibrary::MgrRegistReq
// Explain:  
// Date : 2005-05-19,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrRegistReq( int nCharIndex, CTString strCharName )
{
	CNetworkMessage nmMessage;
	RequestClient::doFriendRegReq* packet = reinterpret_cast<RequestClient::doFriendRegReq*>(nmMessage.nm_pubMessage);
	packet->type = MSG_FRIEND;
	packet->subType = MSG_FRIEND_REGIST_REQUEST;
	packet->requesterindex = nCharIndex;
#if		(_MSC_VER > 1200)
	sprintf_s(packet->name, MAX_CHAR_NAME_LENGTH, "%s", strCharName.str_String);
#else
	sprintf(packet->name, "%s", strCharName.str_String);
#endif
	nmMessage.setSize(sizeof(*packet));

	SendToServerNew(nmMessage);
}


//------------------------------------------------------------------------------
// CNetworkLibrary::MgrRegistAllow
// Explain:  
// Date : 2005-05-19,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrRegistAllow( int nCharIndex, CTString strReqCharName )
{	
	CNetworkMessage nmMessage;
	RequestClient::doFriendRegAllow* packet = reinterpret_cast<RequestClient::doFriendRegAllow*>(nmMessage.nm_pubMessage);
	packet->type = MSG_FRIEND;
	packet->subType = MSG_FRIEND_REGIST_ALLOW;
	packet->charIndex = nCharIndex;
	nmMessage.setSize(sizeof(*packet));

	SendToServerNew(nmMessage);
}


//------------------------------------------------------------------------------
// CNetworkLibrary::MgrSetCondition
// Explain:  
// Date : 2005-05-19,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrSetCondition( int nCharIndex, int nCondition )
{
	CNetworkMessage nmMessage;
	RequestClient::doFriendSetCondition* packet = reinterpret_cast<RequestClient::doFriendSetCondition*>(nmMessage.nm_pubMessage);
	packet->type = MSG_FRIEND;
	packet->subType = MSG_FRIEND_SET_CONDITION;
	packet->charIndex = nCharIndex;
	packet->condition = nCondition;
	nmMessage.setSize(sizeof(*packet));

	SendToServerNew(nmMessage);
}


void CNetworkLibrary::MgrDeleteMember( int nCharIndex, int nTargetIndex, CTString strName )
{
	CNetworkMessage nmMessage;
	RequestClient::doFriendDeleteMember* packet = reinterpret_cast<RequestClient::doFriendDeleteMember*>(nmMessage.nm_pubMessage);
	packet->type = MSG_FRIEND;
	packet->subType = MSG_FRIEND_DELETE_MEMBER;
	packet->requestIndex = nCharIndex;
	packet->targetIndex = nTargetIndex;
	strcpy(packet->name, strName);
	nmMessage.setSize(sizeof(*packet));

	SendToServerNew(nmMessage);
}

void CNetworkLibrary::MgrRegistCancel(int nCharIndex, CTString strReqName)
{
	CNetworkMessage nmMessage;
	RequestClient::doFriendRegCancel* packet = reinterpret_cast<RequestClient::doFriendRegCancel*>(nmMessage.nm_pubMessage);
	packet->type = MSG_FRIEND;
	packet->subType = MSG_FRIEND_REGIST_CANCEL;
	packet->charIndex = nCharIndex;
	nmMessage.setSize(sizeof(*packet));

	SendToServerNew(nmMessage);
}

void CNetworkLibrary::MgrFriendDeleteBlock( int nCharIndex )
{
	CNetworkMessage nmMessage;
	RequestClient::doFriendDeleteBlock* packet = reinterpret_cast<RequestClient::doFriendDeleteBlock*>(nmMessage.nm_pubMessage);
	packet->type = MSG_FRIEND;
	packet->subType = MSG_FRIEND_DELETE_BLOCK;
	packet->charIndex = nCharIndex;
	nmMessage.setSize(sizeof(*packet));

	SendToServerNew(nmMessage);
}

void CNetworkLibrary::SendFriendCatting( int nCharIndex, CTString strName, CTString strTargetName, CTString strMessage )
{
	CNetworkMessage nmFriend( (UBYTE)MSG_CHAT );
	nmFriend << (UBYTE)MSG_CHAT_MESSENGER;

	nmFriend << (ULONG)nCharIndex;
	nmFriend << strName;
	nmFriend << strTargetName;	
	nmFriend << strMessage;
		
	SendToServerNew(nmFriend);
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SetTimeReq
// Explain:  
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::SetTimeReq(int nDay, int nHour, int nZone /* = -1  */)
{
	CNetworkMessage nmGuildWar( (UBYTE)MSG_GUILD );
	nmGuildWar << (UBYTE)MSG_GUILD_WAR_SET_TIME_REQ;

	nmGuildWar << (ULONG)nDay;
	nmGuildWar << (ULONG)nHour;
	nmGuildWar << (ULONG)nZone;
		
	SendToServerNew(nmGuildWar);
}

//------------------------------------------------------------------------------
// CNetworkLibrary::GetTimeReq
// Explain:  
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::GetTimeReq()
{
	CNetworkMessage nmGuildWar( (UBYTE)MSG_GUILD );
	nmGuildWar << (UBYTE)MSG_GUILD_WAR_GET_TIME;

	SendToServerNew(nmGuildWar);
}

//------------------------------------------------------------------------------
// CNetworkLibrary::AttackReq
// Explain:  
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::AttackReq()
{
	CNetworkMessage nmGuildWar( (UBYTE)MSG_GUILD );
	nmGuildWar << (UBYTE)MSG_GUILD_WAR_JOIN_ATTACK_GUILD;

	SendToServerNew(nmGuildWar);
}

//------------------------------------------------------------------------------
// CNetworkLibrary::DefenseGuildReq
// Explain:  
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::DefenseGuildReq()
{
	CNetworkMessage nmGuildWar( (UBYTE)MSG_GUILD );
	nmGuildWar << (UBYTE)MSG_GUILD_WAR_JOIN_DEFENSE_GUILD;

	SendToServerNew(nmGuildWar);
}

//------------------------------------------------------------------------------
// CNetworkLibrary::AttackCharReq
// Explain:  
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::AttackCharReq()
{
	CNetworkMessage nmGuildWar( (UBYTE)MSG_GUILD );
	nmGuildWar << (UBYTE)MSG_GUILD_WAR_JOIN_ATTACK_CHAR;

	SendToServerNew(nmGuildWar);
}

//------------------------------------------------------------------------------
// CNetworkLibrary::AttackGuildReq
// Explain:  
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::AttackGuildReq()
{
	CNetworkMessage nmGuildWar( (UBYTE)MSG_GUILD );
	nmGuildWar << (UBYTE)MSG_GUILD_WAR_JOIN_DEFENSE_CHAR;

	SendToServerNew(nmGuildWar);
}

void CNetworkLibrary::WarItemMixReq( SWORD* arrTab, SWORD* arrIdx )
{
	CNetworkMessage nmMessage;
	RequestClient::doItemMixWar* packet = reinterpret_cast<RequestClient::doItemMixWar*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_MIX_WARITEM;
	packet->tab_1 = arrTab[0];
	packet->invenIndex_1 = arrIdx[0];
	packet->tab_2 = arrTab[1];
	packet->invenIndex_2 = arrIdx[1];
	packet->tab_3 = arrTab[2];
	packet->invenIndex_3 = arrIdx[2];
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew( nmMessage );
}

//-----------------------------------------------------------------------------
// Purpose: 애완 동물을 호출함.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::CallPet( LONG lIndex )
{
	CNetworkMessage nmPet( (UBYTE)MSG_EXTEND );	
	nmPet << (LONG)MSG_EX_PET_CALL;
	nmPet << lIndex;
	SendToServerNew(nmPet);
}

//-----------------------------------------------------------------------------
// Purpose: 펫 타입 정보를 얻습니다.
// Input  : 서버에서 오는 것과 클라이언트에서 쓰이던게 달라서... 필요한 부분...
//-----------------------------------------------------------------------------
void CNetworkLibrary::CheckPetType( SBYTE sbPetTypeGrade, INDEX &iPetType, INDEX &iPetAge )
{	
	// MASK를 하드 코딩함...
	iPetType	= ((sbPetTypeGrade & 0xF0) >> 4) - 1;
	iPetAge		= (sbPetTypeGrade & 0x0F) - 1;
}

//-----------------------------------------------------------------------------
// Purpose: 캐릭터가 펫을 타도록 처리합니다.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::RidePet( CEntity *pCharacter, CEntity *pPet, INDEX iPetType )
{
	if( !pCharacter /*|| !pPet || !bRide */)
		return;

	LeavePet(pCharacter);

	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->RidingPet( pCharacter, iPetType );
}

//-----------------------------------------------------------------------------
// Purpose: 캐릭터가 펫에서 내리도록 처리합니다.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::LeavePet( CEntity *pCharacter )
{
	if( !pCharacter )
		return;

	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->LeavingPet( pCharacter );	
}

// author : rumist [12/20/2010 rumist]
//-----------------------------------------------------------------------------
// Purpose: 캐릭터가 펫을 타도록 처리합니다.
// Input  : 
//-----------------------------------------------------------------------------
void	CNetworkLibrary::RideWildPet(CEntity *pCharacter, CEntity* pWildPet, CTString strFileName )
{
	if( (NULL == pCharacter) || (NULL == pWildPet) )
		return;

	LeaveWildPet( pCharacter );

	(static_cast<CPlayerEntity*>(CEntity::GetPlayerEntity(0)))->RidingWildPet( pCharacter, pWildPet, strFileName );
}

//-----------------------------------------------------------------------------
// Purpose: 캐릭터가 펫에서 내리도록 처리합니다.
// Input  : 
//-----------------------------------------------------------------------------
void	CNetworkLibrary::LeaveWildPet(CEntity *pCharacter )
{
	if( !pCharacter )
		return;
	
	(static_cast<CPlayerEntity*>(CEntity::GetPlayerEntity(0)))->LeavingWildPet( pCharacter );	
}

//-----------------------------------------------------------------------------
// Purpose: 펫 정보를 갱신합니다.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::UpdatePetTargetInfo( INDEX iPetIndex )
{
	CPetTargetInfom* pPetInfo = INFO()->GetMyPetInfo();

	if( !pPetInfo->bIsActive || pPetInfo->lIndex != iPetIndex )
		return;

	CNetworkLibrary::sPetInfo	TempPet;
	TempPet.lIndex				= iPetIndex;
	std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
		std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
	if( iter != _pNetwork->m_vectorPetList.end() )
	{		
		pPetInfo->iLevel		= (*iter).lLevel;
		pPetInfo->fHealth		= (*iter).lHP;
		pPetInfo->fMaxHealth	= (*iter).lMaxHP;
		pPetInfo->fMaxHungry	= (*iter).lMaxHungry;
		pPetInfo->fHungry		= (*iter).lHungry;
		pPetInfo->lAbility		= (*iter).lAbility;
		pPetInfo->strNameCard	= (*iter).strNameCard;

		INDEX iPetType	= -1;
		INDEX iPetAge	= -1;
		_pNetwork->CheckPetType( (*iter).sbPetTypeGrade, iPetType, iPetAge );
		pPetInfo->iAge			= iPetAge;
		pPetInfo->iType			= iPetType;

		const BOOL bPetRide = PetInfo().IsRide(iPetType, iPetAge);
		if( bPetRide )
		{
			// 마운트 상태에서 애완동물의 배고픔이 0이라면 움직이지 못함.
			if( (*iter).lHungry <= 0 )
				CUIManager::getSingleton()->SetCSFlagOn( CSF_MOUNT_HUNGRY );
			else			
				CUIManager::getSingleton()->SetCSFlagOff( CSF_MOUNT_HUNGRY );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 펫 정보를 클리어합니다.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::ClearPetList()
{
	if(!_pNetwork->m_vectorPetList.empty())
	{		
		_pNetwork->m_vectorPetList.clear();
	}
}

//------------------------------------------------------------------------------
// CNetworkLibrary::LearnPetSkill
// Explain:  
// Date : 2005-09-27,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::LearnPetSkill( LONG nSkillIndex )
{
	CNetworkMessage nmPet( (UBYTE)MSG_EXTEND );	
	nmPet << (LONG)MSG_EX_PET_LEARN;
	nmPet << (LONG)nSkillIndex;

	SendToServerNew(nmPet);
}

void CNetworkLibrary::LearnWildPetSkill( LONG nSkillIndex )
{
	CNetworkMessage nmWildPet( (UBYTE)MSG_EXTEND );
	
	nmWildPet << (LONG)MSG_EX_ATTACK_PET;
	nmWildPet << (UBYTE)MSG_SUB_SKILLLEAN;
	nmWildPet << (LONG)nSkillIndex;

	SendToServerNew(nmWildPet);
}

// ----------------------------------------------------------------------------
// Name : SendPetChangeRide()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendPetChangeRide()
{
	CNetworkMessage	nmPet( (UBYTE)MSG_EXTEND );
	nmPet << (LONG)MSG_EX_PET_CHANGE_MOUNT;
	
	CUIManager::getSingleton()->SetCSFlagOn(CSF_PETRIDING);
	SendToServerNew( nmPet );
}

// ----------------------------------------------------------------------------
// Name : SendPetSkillInit()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendPetSkillInit()
{
	CNetworkMessage	nmPet( (UBYTE)MSG_EXTEND );
	nmPet << (LONG)MSG_EX_PET_RESET_SKILL;
		
	SendToServerNew( nmPet );
}

// ----------------------------------------------------------------------------
// Name : SendPetDestruction()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendPetDestruction()
{
	CNetworkMessage nmPet( (UBYTE)MSG_EXTEND );
	nmPet << (LONG)MSG_EX_PET_CHANGE_ITEM;

	SendToServerNew( nmPet );
}

// ----------------------------------------------------------------------------
// Name : SendPetItemMix()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendPetItemMix( SLONG slPetItemIndex, SLONG slmethod )
{
	CNetworkMessage nmPet( (UBYTE)MSG_EXTEND );
	nmPet << (LONG)MSG_EX_PET_MIX_ITEM;   // 조합 메세지
	nmPet << slPetItemIndex;   // 제작 유니크 아이템 인덱스
	nmPet << slmethod;		// 제작 유형 0, 1

	SendToServerNew( nmPet );
}

// ----------------------------------------------------------------------------
// Name : SendPetItemMix()
// Desc : 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendPetRebirth( SLONG slPetIndex )
{
	CNetworkMessage nmPet( (UBYTE)MSG_EXTEND );
	nmPet << (LONG)MSG_EX_PET_REBIRTH;   // 조합 메세지
	nmPet << slPetIndex;   // 제작 유니크 아이템 인덱스

	SendToServerNew( nmPet );
}

void CNetworkLibrary::SendWildPetRebirth( int nTab, int nIdx, int nNpcIdx )
{
	CNetworkMessage nmMessage;
	RequestClient::doExApetRebirth* packet = reinterpret_cast<RequestClient::doExApetRebirth*>(nmMessage.nm_pubMessage);
	packet->type = MSG_EXTEND;
	packet->subType = htonl(MSG_EX_ATTACK_PET);
	packet->thirdType = MSG_SUB_REBIRTH;
	packet->tab = nTab;
	packet->invenIndex = nIdx;
	packet->npcIndex = nNpcIdx;
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew( nmMessage );
}

void CNetworkLibrary::SendPetitemInfo(SLONG slOwnerIndex,int nitemIndex)
{
	CNetworkMessage nmpet((UBYTE)MSG_EXTEND);

	nmpet << (ULONG)MSG_EX_ATTACK_PET;
	nmpet << (UBYTE)MSG_SUB_APET_INFO;
	nmpet << slOwnerIndex;
	nmpet << (INDEX)nitemIndex;

	SendToServerNew(nmpet);
}

// 060306 wooss Party Recall CashItem
//------------------------------------------------------------------------------
// CNetworkLibrary::SendChuseokExchange
// Explain:  
// Date : 060306 wooss
//------------------------------------------------------------------------------
void CNetworkLibrary::SendPartyRecallConfirm(CTString tmp_str,LONG tmp_idx,BOOL tmp_answer)
{
	// 리콜 확인 메시지 YES/NO
	CNetworkMessage nmItem((UBYTE)MSG_EXTEND); 	
	nmItem << (LONG)MSG_EX_PARTY_RECALL;
	nmItem << (LONG)MSG_EX_PARTY_RECALL_CONFIRM;
	nmItem << tmp_idx;
	nmItem << tmp_str;
	nmItem << (UBYTE)tmp_answer;
	_pNetwork->SendToServerNew(nmItem);

	if (tmp_answer) // 리콜 승낙일때만
	{
		CUIManager::getSingleton()->SetCSFlagOn( CSF_TELEPORT );
	}
}


void CNetworkLibrary::SendMovingGuildWarArea()
{
	UIMGR()->SetCSFlagOn(CSF_TELEPORT);

	CNetworkMessage nmWarp( (UBYTE)MSG_WARP );
	nmWarp << (UBYTE)MSG_WARP_PROMPT;
	
	nmWarp << MyCharacterInfo.lWarpZone;
	nmWarp << MyCharacterInfo.lWarpPos;

	SendToServerNew(nmWarp);
	
	// WSS_DRATAN_SIEGEWARFARE 071011
	if(!CUIManager::getSingleton()->GetSiegeWarfareNew()->GetWarState())
		_pUISWDoc->SetDealy();
}


bool CNetworkLibrary::IsLord() 
{
	// 내 길드가 성주 길드이고 내가 그 길드의 길드장이면 성주
	if( ( MyCharacterInfo.sbJoinFlagMerac == WCJF_OWNER || MyCharacterInfo.sbJoinFlagDratan == WCJF_OWNER) 
		&& ( MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS ) )
		return true;
	return false;
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendGuildStashHistroyReq
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::SendGuildStashHistroyReq()
{
	CNetworkMessage nmMessage( (UBYTE)MSG_GUILD );
	nmMessage << (UBYTE)MSG_GUILD_STASH_HISTORY_REQ;
	
	SendToServerNew(nmMessage);
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendGuildStashViewReq
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::SendGuildStashViewReq()
{
	CNetworkMessage nmMessage( (UBYTE)MSG_GUILD );
	nmMessage << (UBYTE)MSG_GUILD_STASH_VIEW_REQ;
	
	SendToServerNew(nmMessage);

}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendGuildStashTakeReq
// Explain:  
// Date : 2005-09-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::SendGuildStashTakeReq( LONGLONG llMoney )
{
	CNetworkMessage nmMessage( (UBYTE)MSG_GUILD );
	nmMessage << (UBYTE)MSG_GUILD_STASH_TAKE_REQ;
	
	nmMessage << llMoney;
	
	SendToServerNew(nmMessage);
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendChuseokUpgrade
// Explain:  
// Date : 2005-09-08,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::SendChuseokUpgrade()
{
	CNetworkMessage nmMessage( (UBYTE)MSG_EVENT );
	nmMessage << (UBYTE)MSG_EVENT_CHUSEOK_UPGRADE;
	
	SendToServerNew(nmMessage);
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendChuseokExchange
// Explain:  
// Date : 2005-09-08,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::SendChuseokExchange()
{
	CNetworkMessage nmMessage( (UBYTE)MSG_EVENT );
	nmMessage << (UBYTE)MSG_EVENT_CHUSEOK_EXCHANGE;
	
	SendToServerNew(nmMessage);
}

///////////////////////////////////////////////////////////////////////////////////
// 2006 추석 이벤트 :Su-won			|--------------------------------------------->
	
//송편 만들기
void CNetworkLibrary::Send2006ChuseokRicecakeMake()
{
	CNetworkMessage	nmMessage( (UBYTE)MSG_EVENT );

	nmMessage << (UBYTE)MSG_EVENT_CHUSEOK_2006;
	nmMessage << (ULONG)MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE;

	SendToServerNew( nmMessage );
}

//오색 송편 만들기
void CNetworkLibrary::Send2006ChuseokRainbowMake()
{
	CNetworkMessage	nmMessage( (UBYTE)MSG_EVENT );

	nmMessage << (UBYTE)MSG_EVENT_CHUSEOK_2006;
	nmMessage << (ULONG)MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE;

	SendToServerNew( nmMessage );
}

//오색송편 보상품으로 교환하기
void CNetworkLibrary::Send2006ChuseokExchange()
{
	CNetworkMessage	nmMessage( (UBYTE)MSG_EVENT );

	nmMessage << (UBYTE)MSG_EVENT_CHUSEOK_2006;
	nmMessage << (ULONG)MSG_EVENT_CHUSEOK_2006_GIFT;

	SendToServerNew( nmMessage );
}	
// 2006 추석 이벤트 :Su-won			<---------------------------------------------|
///////////////////////////////////////////////////////////////////////////////////

// 2006 X-Mase event [12/12/2006 Theodoric]
void CNetworkLibrary::Send2006XMasEvent( SLONG nCakeCount) 
{
	CNetworkMessage	nmMessage( (UBYTE)MSG_EVENT );

	nmMessage << (UBYTE)MSG_EVENT_XMAS_2006;
	nmMessage << nCakeCount;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendCouponItemReq
// Explain:  
// Date : 060118 wooss japan package item
//------------------------------------------------------------------------------
void CNetworkLibrary::SendCouponItemReq(CTString strNum)
{
	CNetworkMessage nmMessage( (UBYTE)MSG_EVENT );
	
	extern INDEX g_iCountry;
	if( g_iCountry == TURKEY )
	{
		nmMessage << (UBYTE)MSG_EVENT_EUR2_PROMOTION;
	}
	else
	{
		nmMessage << (UBYTE)MSG_EVENT_PACKAGE_ITEM_GIVE;
	}
	
	nmMessage << strNum;
	
	SendToServerNew(nmMessage);
}


//------------------------------------------------------------------------------
// CNetworkLibrary::SendConnectItemReq
// Explain:  
// Date : 060223 wooss japan connect event
//------------------------------------------------------------------------------
void CNetworkLibrary::SendConnectItemReq()
{
	CNetworkMessage nmMessage( (UBYTE)MSG_EVENT );
	nmMessage << (UBYTE)MSG_EVENT_OPENBETA_CONN;
		
	SendToServerNew(nmMessage);
}
//------------------------------------------------------------------------------
// CNetworkLibrary::SendClothesExchange
// Explain:  
// Date : 060321 eons japan clothes exchange event
//------------------------------------------------------------------------------
void CNetworkLibrary::SendClothesExchange( int nValue )
{
	CNetworkMessage nmMessage( (UBYTE)MSG_EVENT );
	nmMessage << (UBYTE)MSG_EVENT_SAKURABUD;
	nmMessage << (UBYTE)nValue;

	SendToServerNew( nmMessage );
}

void CNetworkLibrary::SendRainyDayExchange()
{
	CNetworkMessage nmMessage( (UBYTE)MSG_EVENT );
	nmMessage << (UBYTE)MSG_EVENT_RAIN_2006;
	SendToServerNew( nmMessage );
}


void CNetworkLibrary::Send2p4pLetter( LONG lGameIndex )
{
	CNetworkMessage nmMessage( (UBYTE)MSG_EVENT );
	nmMessage << (UBYTE)MSG_EVENT_2PAN4PAN_LETTER;
	nmMessage << (LONG)lGameIndex;
	
	SendToServerNew(nmMessage);
}

//-----------------------------------------------------------------------------
// Purpose: 타운으로 귀환합니다.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendPetWarpTown()
{
	CNetworkMessage nmPet( (UBYTE)MSG_EXTEND );	
	nmPet << (LONG)MSG_EX_PET_WARPTOWN;	
	SendToServerNew(nmPet);
}


//------------------------------------------------------------------------------
// CNetworkLibrary::MandateBossReq
// Explain:  
// Date : 2005-10-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::MandateBossReq( CTString strMandateCharName )
{
	CNetworkMessage	nmMessage( (UBYTE)MSG_PARTY );
	nmMessage << (SBYTE)MSG_PARTY_CHANGEBOSS;
	nmMessage << (CTString) strMandateCharName;
		
	SendToServerNew( nmMessage );
}

//-----------------------------------------------------------------------------
// Purpose: 로그인 메세지를 서버로 보냅니다.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendLoginMessage(CTString& strID, CTString& strPW, ULONG ulVersion)
{
	// EDIT : BS : 070413 : 신규 패킷 암호화
	// EDIT : BS : 패킷 암호화 : 암호화 키 초기화
#ifdef CRYPT_NET_MSG
	CNM_InitKeyValue(&cnmKey);
#ifndef CRYPT_NET_MSG_MANUAL
	CNM_InitKeyValue(&cnmKeyServer);
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#else
	cnmKey = CNM_INIT_KEY;
#endif // #ifdef CRYPT_NET_MSG

#ifdef	VER_TEST
	int nOther = 0, nVtm = ulVersion, nLocal;
	{
		// 다른 서버에 접속하기
		std::string strFullPath = _fnmApplicationPath.FileDir();
		strFullPath += "sl.txt";

		nOther	= GetPrivateProfileInt( "OTHER_SERVER", "OTHER", 0, strFullPath.c_str() );
		if (nOther > 0)
		{
			nVtm	= GetPrivateProfileInt( "OTHER_SERVER", "VTM", ulVersion, strFullPath.c_str() );
			nLocal	= GetPrivateProfileInt( "OTHER_SERVER", "LOCAL", 0, strFullPath.c_str() );
		}
		
	}
#endif	// VER_TEST

	CNetworkMessage nmMessage;
	RequestClient::LoginFromClient* packet = reinterpret_cast<RequestClient::LoginFromClient*>(nmMessage.nm_pubMessage);

	// 로그인 서버에 접속 후, 새로운 서버에 접속을 시도합니다.
	packet->type = MSG_LOGIN;
	packet->subType = MSG_LOGIN_NEW;
	packet->mode = 0;
	packet->version = VERSION_FOR_CLIENT;

#if	(_MSC_VER > 1200)
	strncpy_s(packet->id, MAX_ID_NAME_LENGTH, strID.str_String, strID.Length());
	strncpy_s(packet->pw, MAX_PWD_LENGTH, strPW.str_String, strPW.Length());
#else
	strncpy(packet->id, strID.str_String, strID.Length());
	strncpy(packet->pw, strPW.str_String, strPW.Length());
#endif

	// new Version serialization 060710
	// national code 
	extern INDEX g_iCountry;
	INDEX tv_idx;
	switch(g_iCountry)
	{
	case KOREA :		tv_idx = 0;		break;	
	
	case BRAZIL:		tv_idx = 10;	break;
	case MEXICO:		tv_idx = 20;	break;
	
	case GERMANY:		tv_idx = 13;	break;
	case SPAIN:			tv_idx = 14;	break;
	case FRANCE:		tv_idx = 15;	break;
	case POLAND:		tv_idx = 16;	break;	
	case ITALY:			tv_idx = 19;	break;
	case USA:			tv_idx = 9;		break;

	case RUSSIA:		tv_idx = 17;	break;
	
	case THAILAND :		tv_idx = 4;		break;
	default:			
		tv_idx = g_iCountry;	// ENGLAND
		break;
	}
#ifdef	VER_TEST
	if (nOther > 0)
		packet->nation = (UBYTE)nLocal;
	else 
		packet->nation = (UBYTE)tv_idx;
#else	// VER_TEST
	packet->nation = (UBYTE)tv_idx;
#endif	// VER_TEST
	
	// EDIT : BS : 070413 : 신규 패킷 암호화
	// EDIT : BS : 070413 : 신규 패킷 암호화
#ifdef CRYPT_NET_MSG
#ifndef CRYPT_NET_MSG_MANUAL
	ULONG ulSeed = (ULONG)CNM_MakeSeedForClient(strID, strPW, GetTickCount());
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#endif // #ifdef CRYPT_NET_MSG

	nmMessage.setSize(sizeof(*packet));

	SendToServerNew(nmMessage, TRUE);
	m_uiSendedTime		= (unsigned int(_pTimer->GetLerpedCurrentTick()*1000)); //timeGetTime();
	m_bSendMessage		= TRUE;

	// EDIT : BS : 070413 : 신규 패킷 암호화
#ifdef CRYPT_NET_MSG
#ifndef CRYPT_NET_MSG_MANUAL
	CNM_MakeKeyFromSeed(&_pNetwork->cnmKey, (unsigned int)ulSeed);
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#endif // #ifdef CRYPT_NET_MSG
}

void CNetworkLibrary::SendSecurityMessage(CTString& strSecurity)
{

	// 로그인 서버에 접속 후, 보안 카드 체크 시도합니다.
	CNetworkMessage nmSecurityCardNew((UBYTE)MSG_EXTEND);			// 보안카드 메시지 보내기.			
	nmSecurityCardNew << (LONG)MSG_EX_SECURE_CARD;
	for(int i=0; i<4; i++)
	{
		nmSecurityCardNew << (UBYTE)strSecurity[i];	    
	}
	SendToServerNew(nmSecurityCardNew);
	m_uiSendedTime		= timeGetTime();
	m_bSendMessage		= TRUE;
			
}
//-----------------------------------------------------------------------------
// Purpose: 슈퍼고제.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendSuperStoneReq()
{
	CNetworkMessage msg((UBYTE)MSG_EVENT);
	msg << (SBYTE)MSG_EVENT_SUPERSTONE;
	msg << (SBYTE)MSG_EVENT_SUPERSTONE_REQ;
	SendToServerNew(msg);
}

//-----------------------------------------------------------------------------
// Purpose: 캐릭터 삭제.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendDeleteCharacter(ULONG ulIndex,BYTE del)
{	
	CNetworkMessage nmChaDel((UBYTE)MSG_MENU); //캐릭
	nmChaDel << (unsigned char)MSG_MENU_DEL;
	nmChaDel << ulIndex;
	nmChaDel << del;

	SendToServerNew(nmChaDel,TRUE);	
	m_uiSendedTime		= (unsigned int(_pTimer->GetLerpedCurrentTick()*1000)); timeGetTime();
	m_bSendMessage		= TRUE;
}

//-----------------------------------------------------------------------------
// Purpose: 캐릭터 삭제.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendDeleteCharacter(ULONG ulIndex, CTString& strSecuNum, BYTE del)
{	
	CNetworkMessage nmChaDel((UBYTE)MSG_MENU); //캐릭
	nmChaDel << (unsigned char)MSG_MENU_DEL;
	nmChaDel << ulIndex;
	nmChaDel << strSecuNum;
	nmChaDel << del;
	
		
	SendToServerNew(nmChaDel,TRUE);	
	m_uiSendedTime		= (unsigned int(_pTimer->GetLerpedCurrentTick()*1000)); //timeGetTime();
	m_bSendMessage		= TRUE;
}

//-----------------------------------------------------------------------------
// Purpose: 게임 시작.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendGameStart(ULONG ulIndex)
{
	/////스타트
	CNetworkMessage nmChaStart((UBYTE)MSG_MENU); 
	nmChaStart << (unsigned char)MSG_MENU_START;
	nmChaStart << ulIndex;
	SendToServerNew(nmChaStart,TRUE);	
	m_uiSendedTime		= (unsigned int(_pTimer->GetLerpedCurrentTick()*1000)); //timeGetTime();
	m_bSendMessage		= TRUE;
}

//-----------------------------------------------------------------------------
// Purpose: 몬스터 변신 종료.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendStopChange()
{
	CNetworkMessage nmStopChange((UBYTE)MSG_CHANGE); 	
	nmStopChange << (unsigned char)MSG_CHANGE_CANCEL;
	SendToServerNew(nmStopChange);
}

//-----------------------------------------------------------------------------
// Purpose: 캐릭터 생성.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendCreateCharacter( CTString& strName, UBYTE ubJob, UBYTE ubHair, UBYTE ubFace )
{
	CNetworkMessage nmChaNew((UBYTE)MSG_MENU);			//캐릭 생성.
	nmChaNew << (unsigned char)MSG_MENU_NEW;	  	
	nmChaNew << strName;
	nmChaNew << (UBYTE)ubJob;
	nmChaNew << (UBYTE)ubHair;
	nmChaNew << (UBYTE)ubFace;
    _pNetwork->SendToServerNew(nmChaNew,TRUE);	
	_pNetwork->m_uiSendedTime		= (unsigned int(_pTimer->GetLerpedCurrentTick()*1000)); //timeGetTime();
	_pNetwork->m_bSendMessage		= TRUE;
} 

//-----------------------------------------------------------------------------
// Purpose: 아이템 사용 메세지.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendItemUse( SWORD nTab, SWORD nInvenIdx, LONG lIndex, LONG lEtc )
{
	// 아이템을 통해서 퀘스트를 얻으려 할 경우
	CNetworkMessage nmMessage;
	RequestClient::doItemUse* packet = reinterpret_cast<RequestClient::doItemUse*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_USE;
	packet->tab = nTab;
	packet->invenIndex = nInvenIdx;
	packet->virtualIndex = lIndex;
	packet->extra_1 = lEtc;
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew( nmMessage );
}

//-----------------------------------------------------------------------------
// Purpose: 타겟이 필요한 아이템 사용 메세지.
// Input  : 
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendtargetItemUse( SWORD nTab, SWORD nInvenIdx, LONG lIndex, SBYTE sbType, LONG lMobIndex )
{
	// 아이템을 통해서 퀘스트를 얻으려 할 경우
	CNetworkMessage nmMessage;
	RequestClient::doItemTarget* packet = reinterpret_cast<RequestClient::doItemTarget*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_TARGET;
	packet->tab = nTab;
	packet->invenIndex = nInvenIdx;
	packet->virtualIndex = lIndex;
	packet->charType = sbType;
	packet->charIndex = lMobIndex;	
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew( nmMessage );

}

//-----------------------------------------------------------------------------
// Purpose: 아이템 사용 메세지( 합성 )
// Date  :  060523 wooss
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendMixItemUse( SWORD nTab, SWORD nInvenIdx, LONG lMixItemidx, LONG lCashItemIdx, LONG lgameItemIdx)
{
	CNetworkMessage nmMessage;
	RequestClient::doItemUse* packet = reinterpret_cast<RequestClient::doItemUse*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_USE;
	packet->tab = nTab;
	packet->invenIndex = nInvenIdx;
	packet->virtualIndex = lMixItemidx;
	packet->extra_1 = lCashItemIdx;
	packet->extra_2 = lgameItemIdx;
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew( nmMessage );
}

//-----------------------------------------------------------------------------
// Purpose: 아이템 사용 메세지( 분리 )
// Date  :  060627 wooss
//-----------------------------------------------------------------------------
void CNetworkLibrary::SendUnMixItemUse( SWORD nTab, SWORD nIdx, LONG lMixItemidx, LONG lCashItemIdx)
{
	CNetworkMessage nmMessage;
	RequestClient::doItemUse* packet = reinterpret_cast<RequestClient::doItemUse*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_USE;
	packet->tab = nTab;
	packet->invenIndex = nIdx;
	packet->virtualIndex = lMixItemidx;
	packet->extra_1 = lCashItemIdx;
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew( nmMessage );
}


//-----------------------------------------------------------------------------
// Purpose: 소환수 생성.
// Input  : 플레이어의 경우에만 해당하는 것 같음...
//-----------------------------------------------------------------------------
void CNetworkLibrary::CreateSlave( int iOwnerIndex, CEntity* pOwnerEntity, int iType )
{
	if(pOwnerEntity == NULL )
		return;
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendEvent24
// Explain: 2pan4pan event message  
// Date : 2005-10-31,Author: wooss
//------------------------------------------------------------------------------

void CNetworkLibrary::SendEvent24(int sel,SBYTE eventType)
{
	CNetworkMessage	nmMessage( (UBYTE)MSG_EVENT);
	
	switch(eventType)
	{
	case MSG_EVENT_2PAN4PAN_GOODS_CHECK:
		nmMessage << (UBYTE)eventType;	
		break;
	case MSG_EVENT_2PAN4PAN_GOODS_REQ:
		nmMessage << (UBYTE)eventType;
		break;
	case MSG_EVENT_2PAN4PAN_MONEY_REQ:
		nmMessage << (UBYTE)eventType;
		nmMessage << (ULONG)sel;
		break;
	case MSG_EVENT_2PAN4PAN_BOX_CHECK:
		nmMessage << (UBYTE)eventType;
		break;
	case MSG_EVENT_2PAN4PAN_BOX_REQ:
		nmMessage << (UBYTE)eventType;
		break;
	}
	
	SendToServerNew( nmMessage );


//	MSG_EVENT_2PAN4PAN_GOODS_CHECK,				// 2판4판 리뉴얼 아이템 리스트 요청 : count(n:server) wooss 051031
//	MSG_EVENT_2PAN4PAN_GOODS_REQ,				// 2판4판 리뉴얼 아이템 받기 요청 :
//	MSG_EVENT_2PAN4PAN_GOODS_REP,				// 2판4판 리뉴얼 아이템 받기 응답 : MSG_EVENT_2PAN4PAN_GOODS_ERROR_TYPE(n) count(n) [itemdbindex(n) itemcount(ll)]*count

//	MSG_EVENT_2PAN4PAN_MONEY_REQ,				// 2판4판 머니 카드 교환 요청 : type(n)
												// type : 1 - 맞고, 2 - 포커
//	MSG_EVENT_2PAN4PAN_MONEY_REP,				// 2판4판 머니 카드 교환 응답 : success(n)
												// success : 0 - 실패, 1 - 성공

//	MSG_EVENT_2PAN4PAN_BOX_CHECK,				// 2판4판 상자 교환 검사	: flag(n:server)
												// flag : 0 - 없음, 1 - 있음
//	MSG_EVENT_2PAN4PAN_BOX_REQ,					// 2판4판 상자 교환 요청	: 
//	MSG_EVENT_2PAN4PAN_BOX_REP,					// 2판4판 상자 교환 응답	: MSG_EVENT_2PAN4PAN_BOX_ERROR_TYPE(n) count(n)  [itemdbindex(n) itemcount(ll)]*count

}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendCastleMapRecent
// Explain:  
// Date : 2005-10-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::SendCastleMapRecent()
{
	CNetworkMessage	nmMessage( (UBYTE)MSG_EXTEND );
	nmMessage << (LONG)MSG_EX_CASTLE_MAP_RECENT;
			
	SendToServerNew( nmMessage );

}


//------------------------------------------------------------------------------
// CNetworkLibrary::SendCastleMapSignal
// Explain:  
// Date : 2005-10-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::SendCastleMapSignal( FLOAT fX, FLOAT fY, LONG lSenderFlag, LONG lSenderIndex )
{
	CNetworkMessage	nmMessage( (UBYTE)MSG_EXTEND );
	nmMessage << (LONG)MSG_EX_CASTLE_MAP_SIGNAL;

	nmMessage << (FLOAT)fX;
	nmMessage << (FLOAT)fY;
	nmMessage << (LONG)lSenderFlag;
	nmMessage << (LONG)lSenderIndex;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendGameStart
// Explain: 로딩이 완료되면 서버에 MSG_START_GAME를 보낸다.
// * 현재 로딩되고 있는 zone 번호는 초기화
// Date : 2005-11-03,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::SendGameStart()
{
	SetLoadingZone( -1 );
	
	CNetworkMessage nmStartGame((UBYTE)MSG_START_GAME);
	_pNetwork->SendToServerNew(nmStartGame,TRUE);	
	
	_cmiComm.Server_Update();
}


//------------------------------------------------------------------------------
// CNetworkLibrary::SetLoadingZone
// Explain: 서버에서 MSG_DB_OK메세지를 받으면 존번호를 저장하여 
// 존 로딩이 완료 된 후에 MSG_START_GAME를 보낸다.
// Date : 2005-11-03,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::SetLoadingZone( LONG lZone )
{
	m_lLoadingZone = lZone;
}


//------------------------------------------------------------------------------
// CNetworkLibrary::GetLoadingZone
// Explain: 로딩되고 있는 존의 번호를 얻어 온다.
// * 로딩을 하고 있지 않을때는 -1
// Date : 2005-11-03,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
LONG CNetworkLibrary::GetLoadingZone()
{
	return m_lLoadingZone;
}

//------------------------------------------------------------------------------
// CNetworkLibrary::GetLoadingZone
// Explain: 로딩되고 있는 존의 번호를 얻어 온다.
// * 로딩을 하고 있지 않을때는 -1
// Date : 2005-11-03,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CNetworkLibrary::SendNetCafeOpenBox()
{
	CNetworkMessage	nmMessage( (UBYTE)MSG_EVENT );
	nmMessage << (UBYTE)MSG_EVENT_PCBANG;
	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendPartyRegistNormal
// Explain: 파티 매칭 일반 등록 요청
// Date : 2006-05-09(오후 5:14:07), By eons
//------------------------------------------------------------------------------
void CNetworkLibrary::SendPartyRegistNormal( int nType )
{
	CNetworkMessage		nmMessage( (UBYTE)MSG_EXTEND );
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH;
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH_REG_MEMBER_REQ;

	nmMessage << (UBYTE)nType;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendPartyRegistParty
// Explain: 파티 매칭 파티 등록 요청
// Date : 2006-05-09(오후 5:14:07), By eons
//------------------------------------------------------------------------------
void CNetworkLibrary::SendPartyRegistParty( CTString strComment, DWORD JobFlag, int LimitLevel )
{
	CNetworkMessage	nmMessage( (UBYTE)MSG_EXTEND );
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH;
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH_REG_PARTY_REQ;
	nmMessage << strComment;
	nmMessage << (ULONG)JobFlag;
	nmMessage << (SBYTE)LimitLevel;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendPartyRegistParty
// Explain: 파티 매칭 파티 등록 요청
// Date : 2006-05-13(오후 1:22:33), By eons
//------------------------------------------------------------------------------
void CNetworkLibrary::SendPartyListReq( int nPage, int nJob, int nLimitLv, int nPtType, BOOL bState )
{
	CNetworkMessage nmMessage( (UBYTE)MSG_EXTEND );
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH;

	if( bState ) // 파티 팀 리스트 요청
	{
		nmMessage << (ULONG)MSG_EX_PARTY_MATCH_PARTY_LIST_REQ;
	}
	else// 파티 멤버 리스트 요청
	{
		nmMessage << (ULONG)MSG_EX_PARTY_MATCH_MEMBER_LIST_REQ;
	}

	nmMessage << (SLONG)nPage;
	nmMessage << (SBYTE)nJob;
	nmMessage << (SBYTE)nLimitLv;
	nmMessage << (SBYTE)nPtType;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendPartyRegistDelete
// Explain: 파티 매칭 파티 등록 삭제 요청
// Date : 2006-05-15(오전 9:54:51), By eons
//------------------------------------------------------------------------------
void CNetworkLibrary::SendPartyRegistDelete()
{
	CNetworkMessage nmMessage( (UBYTE)MSG_EXTEND );
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH;
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH_DEL_REQ;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendPartyInviteReq
// Explain: 파티 매칭 일반 초대 요청
// Date : 2006-05-15(오전 9:54:51), By eons
//------------------------------------------------------------------------------
void CNetworkLibrary::SendPartyInviteReq( int nCharIndex )
{
	CNetworkMessage nmMessage( (UBYTE)MSG_EXTEND );
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH;
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH_INVITE_REQ;
	nmMessage << (SLONG)nCharIndex;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendPartyJoinReq
// Explain: 파티 매칭 파티 조인 요청
// Date : 2006-05-15(오후 1:39:55), By eons
//------------------------------------------------------------------------------
void CNetworkLibrary::SendPartyJoinReq( int nBossIndex )
{
	CNetworkMessage nmMessage( (UBYTE)MSG_EXTEND );
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH;
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH_JOIN_REQ;
	nmMessage << (SLONG)nBossIndex;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendPartyJoinAllow
// Explain: 파티 매칭 파티 조인 허락
// Date : 2006-05-15(오후 1:28:11), By eons
//------------------------------------------------------------------------------
void CNetworkLibrary::SendPartyJoinAllow( int nReqIndex )
{
	CNetworkMessage nmMessage( (UBYTE)MSG_EXTEND );
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH;
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH_JOIN_ALLOW;
	nmMessage << (SLONG)nReqIndex;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendPartyJoinReject
// Explain: 파티 매칭 파티 조인 거절
// Date : 2006-05-15(오후 1:28:11), By eons
//------------------------------------------------------------------------------
void CNetworkLibrary::SendPartyJoinReject( int nReqIndex )
{
	CNetworkMessage nmMessage( (UBYTE)MSG_EXTEND );
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH;
	nmMessage << (ULONG)MSG_EX_PARTY_MATCH_JOIN_REJECT;
	nmMessage << (SLONG)nReqIndex;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendWorldCupEvent
// Explain: WorldCup Event 1
// Date : 2006-06-01, wooss
//------------------------------------------------------------------------------
void CNetworkLibrary::SendWorldCupEvent( int msgCmd, int iSelIdx )
{
	CNetworkMessage	nmMessage( (UBYTE)MSG_EVENT );
	
	switch(msgCmd)
	{
		case MSGCMD_WORLDCUP_EVENT1_EXCHANGE :
		{
			nmMessage << (UBYTE)MSG_EVENT_WORLDCUP;
			nmMessage << (LONG)MSG_EVENT_WORLDCUP_VOTE;
			nmMessage << (LONG)iSelIdx;
		}
		break;
		case MSGCMD_WORLDCUP_EVENT1_RECEIVE :
		{
			nmMessage << (UBYTE)MSG_EVENT_WORLDCUP;
			nmMessage << (LONG)MSG_EVENT_WORLDCUP_GIFT;
			nmMessage << (LONG)iSelIdx;
		}
		break;

		
	}
	
	SendToServerNew( nmMessage );

}

//------------------------------------------------------------------------------
// CNetworkLibrary::SendWorldCupGoldenBallEvent
// Explain: Golden ball event
// Date : 2006-06-01(오후 8:05:27), By eons
//------------------------------------------------------------------------------
void CNetworkLibrary::SendWorldCupGoldenBallEvent( int nSendType, int nTeamAScore, int nTeamBScore )
{
	CNetworkMessage	nmMessage( (UBYTE)MSG_EVENT );

	nmMessage << (UBYTE)MSG_EVENT_GOLDENBALL;
	nmMessage << (LONG)nSendType;

	if( nSendType == MSG_EVENT_GOLDENBALL_VOTE )
	{
		nmMessage << (LONG)nTeamAScore;
		nmMessage << (LONG)nTeamBScore;
	}

	SendToServerNew( nmMessage );
}


//------------------------------------------------------------------------------
// CNetworkLibrary::MgrOneVsOneConnect
// Explain: OneOnOneChatMessenger
// Date : 2006-12-11(오후 8:05:27), By KwonYongDae
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrOneVsOneConnect( int nCharIndex, int nOtherCharIndex, CTString strChat )
{
	CNetworkMessage	nmMessage( (UBYTE)MSG_EXTEND );
	
	nmMessage << (LONG)MSG_EX_MESSENGER;
	nmMessage << (BYTE)MSG_EX_MESSENGER_ONE_VS_ONE;
	nmMessage << (LONG)nCharIndex;
	nmMessage << (LONG)nOtherCharIndex;
	nmMessage << strChat;

	SendToServerNew( nmMessage );
}

//============================================================================================================
// CNetworkLibrary::SendBuddhismEvent
// Explain : Buddhism event
// Date : 2006-06-27(오전 10:44:09), By eons
//============================================================================================================
void CNetworkLibrary::SendBuddhismEvent( BYTE nType )
{
	CNetworkMessage nmMessage( (UBYTE)MSG_EVENT );

	nmMessage << (UBYTE)MSG_EVENT_TLD_BUDDHIST;
	nmMessage << (BYTE)nType;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::MgrFriendInvite
// Explain: 친구를 대화에 초대한다
// nCharIndex - 채팅창 생성 캐릭 인덱스, ChatIndex -채팅창 인덱스, strTargetName -초대할 캐릭 이름
// Date : 2006-6-1 Su-won
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrFriendInvite( int nCharIndex, int nChatIndex, int nTargetIndex)
{
	CNetworkMessage	nmMessage( (UBYTE)MSG_EXTEND );
	
	nmMessage << (LONG)MSG_EX_MESSENGER;
	nmMessage << (BYTE)MSG_EX_MESSENGER_INVITE;
	nmMessage << (LONG)nCharIndex;
	nmMessage << (LONG)nChatIndex;
	nmMessage << (LONG)nTargetIndex;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::MgrFriendOut
// Explain: 친구가 대화창에서 나감.
// nCharIndex - 채팅창 생성 캐릭 인덱스, nChatIndex -채팅창 인덱스
// Date : 2006-6-1 Su-won
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrFriendOut( int nCharIndex, int nChatIndex )
{
	CNetworkMessage	nmMessage( (UBYTE)MSG_EXTEND );
	
	nmMessage << (LONG)MSG_EX_MESSENGER;
	nmMessage << (BYTE)MSG_EX_MESSENGER_OUT;
	nmMessage << (LONG)nCharIndex;
	nmMessage << (LONG)nChatIndex;
	
	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::MgrFriendChat
// Explain: 친구가 대화창에서 나감.
// nCharIndex - 채팅창 생성 캐릭 인덱스, nChatIndex -채팅창 인덱스, strChat -채팅 내용
// Date : 2006-6-1 Su-won
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrFriendChat( int nCharIndex, int nChatIndex, CTString strChat)
{
	CNetworkMessage	nmMessage( (UBYTE)MSG_EXTEND );
	
	nmMessage << (LONG)MSG_EX_MESSENGER;
	nmMessage << (BYTE)MSG_EX_MESSENGER_CHAT;
	nmMessage << (LONG)nCharIndex;
	nmMessage << (LONG)nChatIndex;
	nmMessage << strChat;
	
	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::MgrBlock
// Explain: 친구를 차단시킴.
// strTarget -차단대상 캐릭 이름
// Date : 2006-6-2 Su-won
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrBlock( CTString strTarget)
{
	CNetworkMessage	nmMessage( (UBYTE)MSG_EXTEND );

	nmMessage << (LONG)MSG_EX_MESSENGER
			  << (BYTE)MSG_EX_MESSENGER_BLOCK
			  << strTarget;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::MgrUnBlock
// Explain: 차단된 캐릭을 해제함.
// strTarget -차단대상 캐릭 이름
// Date : 2006-6-9 Su-won
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrUnBlock( int nCharIndex )
{
	CNetworkMessage	nmMessage( (UBYTE)MSG_EXTEND );

	nmMessage << (LONG)MSG_EX_MESSENGER
			  << (BYTE)MSG_EX_MESSENGER_RELEASE_BLOCK
			  << (LONG)nCharIndex;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::MgrGroupAdd
// Explain: 메신저에 그룹을 추가.
// strName -추가할 그룹 이름
// Date : 2006-6-1 Su-won
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrGroupAdd( CTString strName)
{
	CNetworkMessage	nmMessage( (UBYTE)MSG_EXTEND );
	
	nmMessage << (LONG)MSG_EX_MESSENGER
			  << (BYTE)MSG_EX_MESSENGER_GROUP_ADD
			  << strName;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::MgrGroupDel
// Explain: 메신저에 그룹을 삭제함.
// nGroupIndex -삭제할 그룹 인덱스
// Date : 2006-6-1 Su-won
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrGroupDel( int nGroupIndex)
{
	CNetworkMessage	nmMessage( (UBYTE)MSG_EXTEND );
	
	nmMessage << (LONG)MSG_EX_MESSENGER;
	nmMessage << (BYTE)MSG_EX_MESSENGER_GROUP_DEL;
	nmMessage << (LONG)nGroupIndex;
	
	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::MgrGroupRename
// Explain: 메신저에 그룹 이름을 변경함.
// nGroupIndex -변경할 그룹 인덱스, strNewName -변경할 이름.
// Date : 2006-6-1 Su-won
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrGroupRename( int nGroupIndex, CTString strNewName)
{
	CNetworkMessage	nmMessage( (UBYTE)MSG_EXTEND );
	
	nmMessage << (LONG)MSG_EX_MESSENGER;
	nmMessage << (BYTE)MSG_EX_MESSENGER_GROUP_CHANGE;
	nmMessage << (LONG)nGroupIndex;
	nmMessage << strNewName;
	
	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::MgrGroupMove
// Explain: 메신저에 친구의 그룹을 변경함.
// nGroupIndex -변경할 그룹 인덱스, strCharName -그룹을 이동할 캐릭 이름.
// Date : 2006-6-1 Su-won
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrGroupMove( int nGroupIndex, int nCharIndex)
{
	CNetworkMessage	nmMessage( (UBYTE)MSG_EXTEND );
	
	nmMessage << (LONG)MSG_EX_MESSENGER;
	nmMessage << (BYTE)MSG_EX_MESSENGER_GROUP_MOVE;
	nmMessage << (LONG)nGroupIndex;
	nmMessage << (LONG)nCharIndex;

	SendToServerNew( nmMessage );
}

//------------------------------------------------------------------------------
// CNetworkLibrary::MgrSetChatColor
// Explain: 채팅글의 색깔 설정
// colChat -변경할 색
// Date : 2006-6-7 Su-won
//------------------------------------------------------------------------------
void CNetworkLibrary::MgrSetChatColor( int nColIndex)
{
	CNetworkMessage	nmMessage( (UBYTE)MSG_EXTEND );
	
	nmMessage << (LONG)MSG_EX_MESSENGER;
	nmMessage << (BYTE)MSG_EX_MESSENGER_CHATCOLOR_CHAGNE;
	nmMessage << (LONG)nColIndex;

	SendToServerNew( nmMessage );
}

//============================================================================================================
// CNetworkLibrary::SendBuyCollectBox
// Explain : 곤충 채집 상자 구입
// Date : 2006-06-28(오후 4:21:15), By eons
//============================================================================================================
void CNetworkLibrary::SendBuyCollectBox( void )
{
	CNetworkMessage	nmMessage( (UBYTE)MSG_EVENT );

	nmMessage << (UBYTE)MSG_EVENT_COLLECT_BUG;
	nmMessage << (ULONG)MSG_EVENT_COLLECT_BUG_BUY_BOX_REQ;

	SendToServerNew( nmMessage );	
}

//============================================================================================================
// CNetworkLibrary::SendGiftCollectBox
// Explain : 곤충 채집 상자 보상 요청
// Date : 2006-06-28(오후 4:23:19), By eons
//============================================================================================================
void CNetworkLibrary::SendGiftCollectBox( void )
{
	CNetworkMessage	nmMessage( (UBYTE)MSG_EVENT );

	nmMessage << (UBYTE)MSG_EVENT_COLLECT_BUG;
	nmMessage << (ULONG)MSG_EVENT_COLLECT_BUG_GIFT_REQ;

	SendToServerNew( nmMessage );	
}

//============================================================================================================
// CNetworkLibrary::SendDropInsect
// Explain : 곤충 아이템 드롭
// Date : 2006-06-28(오후 4:24:31), By eons
//============================================================================================================
void CNetworkLibrary::SendDropInsect( int nInsect )
{
	CNetworkMessage nmMessage( (UBYTE)MSG_EVENT );

	nmMessage << (UBYTE)MSG_EVENT_COLLECT_BUG;
	nmMessage << (ULONG)MSG_EVENT_COLLECT_BUG_DROP;
	nmMessage << (LONG)nInsect;

	SendToServerNew( nmMessage );
}


//---------------------------------------------------------------------->>
// Name : SetCheckPointData
// Desc : for s-dungeon check point (use touch field)
// Date : [7/7/2006] , Wooss
//----------------------------------------------------------------------<<
void CNetworkLibrary::SetCheckPointData(int questIdx,int chkNumFlag, int maxChkflag)
{
	if(questIdx != -1)
		wo_stCheckPoint.m_iQuestIndex	= questIdx;
	if(chkNumFlag != -1)
		wo_stCheckPoint.m_iCheckFlag	|= chkNumFlag;
	if(maxChkflag != -1)
		wo_stCheckPoint.m_iMaxCheckFlag = maxChkflag;
	
}

//==============================================================================================
// CNetworkLibrary::SendUsaOpenBetaGift
// Expalin : 미국 오픈 베타 선물 지급( 7일간 )
// Date : [11/16/2006] , eons
//==============================================================================================
void CNetworkLibrary::SendUsaOpenBetaGift(void)
{
	CNetworkMessage nmMessage((UBYTE)MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_OPEN_BETA_GIFT;

	SendToServerNew(nmMessage);
}

//=============================================================================================
// CNetworkLibrary::SendSaveBingoItem
// Explain : 러브러브 이벤트(초코상자 아이템 저장 요청)
//
//=============================================================================================
void CNetworkLibrary::SendSaveBingoItem(BYTE nPos, int nGiftItemIndex, int nBoxItemIndex)
{
	CNetworkMessage nmMessage((UBYTE)MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_VALENTINE_2007;
	nmMessage << (ULONG)MSG_EVENT_VALENTINE_2007_BINGO_ITEM_ARRANGE;
	nmMessage << (BYTE)nPos;
	nmMessage << (LONG)nGiftItemIndex;
	nmMessage << (LONG)nBoxItemIndex;

	SendToServerNew(nmMessage);
}

//=============================================================================================
// CNetworkLibrary::SendSaveBingoItem
// Explain : 러브러브 이벤트(초코상자 아이템 저장 요청)
//
//=============================================================================================
void CNetworkLibrary::SendReqGiftChocoBox(int nUniItemIndex)
{
	CNetworkMessage nmMessage((UBYTE)MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_VALENTINE_2007;
	nmMessage << (ULONG)MSG_EVENT_VALENTINE_2007_BINGO_GIFT;
	nmMessage << (LONG)nUniItemIndex;

	SendToServerNew(nmMessage);
}

// wooss 070307 ---------------------------------------->>
// kw : WSS_WHITEDAY_2007 
void CNetworkLibrary::SendWhiteday2007(UBYTE ubType)
{
	CNetworkMessage nmMessage((UBYTE)MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_WHITEDAY_2007;
	nmMessage << (ULONG)MSG_EVENT_WHITEDAY_2007_EXCHANGE_REQ;
	nmMessage << (UBYTE)ubType;

	SendToServerNew(nmMessage);
}

void CNetworkLibrary::SendWhiteday2007Letter(int itemIdx,COLOR colMsg,CTString tStr)
{
	CNetworkMessage nmMessage((UBYTE)MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_WHITEDAY_2007;
	nmMessage << (ULONG)MSG_EVENT_WHITEDAY_2007_LETTER_REQ;
	nmMessage << (ULONG)itemIdx;
	nmMessage << (ULONG)colMsg;
	nmMessage << tStr;

	SendToServerNew(nmMessage);
}
// -----------------------------------------------------<<

// WSS_GUILDMASTER 070416 ----------------------------------------->>

// 길드포인트 리스트 요청 
void CNetworkLibrary::SendRequestParentsday(int reqIdx)
{
	CNetworkMessage nmMessage((UBYTE)MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_PARENTSDAY_2007;
	nmMessage << (ULONG)reqIdx;
	
	SendToServerNew(nmMessage);
}

// 길드 포인트 등록
void CNetworkLibrary::SendParentsdayAddPoint(int charIdx)
{
	CNetworkMessage nmMessage((UBYTE)MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_PARENTSDAY_2007;
	nmMessage << (ULONG)MSG_EVENT_PARENTSDAY_2007_ADD_POINT;
	nmMessage << (ULONG)charIdx;
		
	SendToServerNew(nmMessage);
}
// ----------------------------------------------------------------<<	
/*
#ifndef NO_GAMEGUARD
// WSS_NPROTECT 070402 --------------------------------->>

void CNetworkLibrary::SendnProtectAuth2(DWORD dwArg)
{
	GG_AUTH_DATA m_AuthData;
	int msgLen = sizeof(GG_AUTH_DATA);
	memcpy(&m_AuthData,(PVOID)dwArg,msgLen);
	CNetworkMessage nmItem(MSG_EXTEND);
	nmItem << (ULONG)MSG_EX_NPROTECT;
	nmItem << m_AuthData.dwIndex;
	nmItem << m_AuthData.dwValue1;
	nmItem << m_AuthData.dwValue2;
	nmItem << m_AuthData.dwValue3;			
	_pNetwork->SendToServerNew(nmItem);

//	CPrintF("Send Index  : %x\n", m_AuthData.dwIndex);
// 	CPrintF("Send Value1 : %x\n", m_AuthData.dwValue1);
// 	CPrintF("Send Value2 : %x\n", m_AuthData.dwValue2);
//	CPrintF("Send Value3 : %x\n", m_AuthData.dwValue3);
}

BOOL CNetworkLibrary::Init_nProtect()
{	
	// npgl 초기화 에러 메시지 출력
	DWORD dwResult = npgl.Init();
	if ( dwResult != NPGAMEMON_SUCCESS )
	{
		CTString msg,errStr;
		switch(dwResult)
		{
			// TODO : 메시지 출력(p18)
		case NPGAMEMON_ERROR_EXIST:
			errStr = _S(3321,"게임가드가 실행 중입니다. 잠시 후나 재부팅 후에 다시 실행해 보시기 바랍니다.");
			break;
		case NPGAMEMON_ERROR_GAME_EXIST:
			errStr = _S(3322,"게임이 중복 실행되었거나 게임가드가 이미 실행 중 입니다. 게임 종료 후 다시 실행해 보시기 바랍니다,");
			break;
		case NPGAMEMON_ERROR_INIT:
			errStr = _S(3323,"게임가드 초기화 에러입니다. 재부팅 후 다시 실행해 보거나 충돌할 수 있는 다른 프로그램들을 종료한 후 실행해 보시기 바랍니다.");
			break;
		case NPGAMEMON_ERROR_AUTH_GAMEGUARD:
		case NPGAMEMON_ERROR_NFOUND_GG:			
		case NPGAMEMON_ERROR_AUTH_INI:		
		case NPGAMEMON_ERROR_NFOUND_INI:
			errStr = _S(3324,"게임가드 파일이 없거나 변조되었습니다. 게임가드 셋업 파일을 설치해보시기 바랍니다.");
			break;
		case NPGAMEMON_ERROR_CRYPTOAPI:
			errStr = _S(3325,"윈도우의 일부 시스템 파일이 손상되었습니다. 인터넷 익스플로러(IE)를 다시 설치해 보시기 바랍니다.");
			break;
		case NPGAMEMON_ERROR_EXECUTE:
			errStr = _S(3326,"게임가드 실행에 실패했습니다. 게임가드 셋업 파일을 다시 설치해 보시기 바랍니다.");
			break;
		case NPGAMEMON_ERROR_ILLEGAL_PRG:
			errStr = _S(3327,"불법 프로그램이 발견되었습니다. 불필요한 프로그램을 종료한 후 다시 실행해 보시기 바랍니다.");
//			break;
		case NPGMUP_ERROR_ABORT:
			errStr = _S(3328,"게임가드 업데이트를 취소하셨습니다. 접속이 계속되지 않을 경우 인터넷 및 개인 방화벽 설정을 조정해 보시기 바랍니다.");
			break;
		case NPGMUP_ERROR_CONNECT:			
		case HOOK_TIMEOUT:
			errStr = _S(3329,"바이러스나 스파이웨어로 인해 후킹이 실패하였습니다. 최신백신을 받으신 후 컴퓨터 전체검사를 해 주십시오");
			break;
		case NPGAMEMON_ERROR_GAMEGUARD:
			errStr = _S(3330,"게임가드 초기화 에러 또는 구버젼의 게임가드 파일입니다. 게임가드 셋업 파일을 다시 설치하고 게임을 실행해 주십시오");
			break;
		case NPGMUP_ERROR_PARAM:
			errStr = _S(3331,"ini 파일이 없거나 변조되었습니다. 게임가드 셋업 파일을 설치하면 해결할 수 있습니다.");
			break;
		case NPGMUP_ERROR_INIT:
			errStr = _S(3332,"npgmup.des 초기화 에러입니다. 게임가드 폴더를 삭제후 다시 게임 실행해 주십시오.");
			break;
		case NPGMUP_ERROR_DOWNCFG:
			errStr = _S(3333,"게임가드 업데이트 서버 접속에 실패하였습니다. 잠시 후 재시도 해보거나, 개인 방화벽이 있다면 설정을 조정해 보시기 바랍니다.");
			break;
		case NPGMUP_ERROR_AUTH:
			errStr = _S(3334,"게임가드 업데이트를 완료하지 못 했습니다. 바이러스 백신을 일시 중지시킨 후 재시도 해보시거나, PC관리 프로그램을 사용하시면 설정을 조정해 보시기 바랍니다.");
			break;
		case NPGAMEMON_ERROR_NPSCAN:
			errStr = _S(3335,"바이러스 및 해킹툴 검사 모듈 로딩에 실패 했습니다. 메모리 부족이거나 바이러스에 의한 감염일 수 있습니다.");			
			break;
		default:
			errStr = _S(3336,"게임가드 실행 중 에러가 발생하였습니다. 게임 폴더 안의 GameGuard폴더에 있는 *erl 파일을 버그레포팅시 첨부하여 주시기 바랍니다.");
			break;			
		}
		msg.PrintF(_S(3342,"게임가드 에러 : %lu"),dwResult);
		MessageBox(NULL,errStr.str_String,msg,MB_OK);
		
		// 게임 종료
		return FALSE;
	}	

	return TRUE;
}


BOOL CNetworkLibrary::NPGameMonCallbackErrChk(DWORD dwMsg, DWORD dwArg)
{
	BOOL tAppExit = FALSE;
	extern ENGINE_API CTString g_szHackMsg;

	switch(dwMsg)
	{
		// TODO : 메시지 처리 (p21)		
		case NPGAMEMON_COMM_ERROR :
		case NPGAMEMON_COMM_CLOSE :
			tAppExit = TRUE;
			break;
		case NPGAMEMON_INIT_ERROR :
			tAppExit = TRUE;
			g_szHackMsg.PrintF(_S(3337,"게임가드 초기화 에러 : %lu"), dwArg);
			break;
		case NPGAMEMON_SPEEDHACK :
			tAppExit = TRUE;
			g_szHackMsg = _S(3338,"스피드핵이 감지되었습니다.");
			break;						
		case NPGAMEMON_GAMEHACK_KILLED :
			tAppExit = TRUE;
			g_szHackMsg.PrintF(_S(3339,"게임핵이 발견되었습니다.\r\n%s"),npgl.GetInfo());
			break;
		case NPGAMEMON_GAMEHACK_DETECT :
			tAppExit = TRUE;
			g_szHackMsg.PrintF(_S(3340,"게임핵이 발견되었습니다.\r\n%s"),npgl.GetInfo());
			break;
		case NPGAMEMON_GAMEHACK_DOUBT :
			tAppExit = TRUE;
			g_szHackMsg.PrintF(_S(3341,"게임이나 게임가드가 변조되었습니다.\r\n%s"),npgl.GetInfo());
			break;
		case NPGAMEMON_CHECK_CSAUTH2 :
			SendnProtectAuth2(dwArg);
			break;
	}

	return tAppExit;
}

void CNetworkLibrary::SetHwnd_nProtect(HWND _hwndMain)
{
	npgl.SetHwnd(_hwndMain);
}

void CNetworkLibrary::Check_nProtect()
{
	if( npgl.Check() != NPGAMEMON_SUCCESS )
	{
		// 게임 종료
		_pGameState->Running()		= FALSE;
		_pGameState->QuitScreen()	= FALSE;
	}
}

#endif
  /**/
// -----------------------------------------------------<<

// WSS_MINIGAME 070420 -------------------------------------------->>

void CNetworkLibrary::SendMinigameDefaultMessage(UBYTE eventType, UBYTE subType)
{
	// MSG_EVENT_GOMDORI_2007_CHECKITEM
	// MSG_EVENT_GOMDORI_2007_START
	// MSG_EVENT_GOMDORI_2007_CONTINUE
	// MSG_EVENT_GOMDORI_2007_END
	// MSG_EVENT_GOMDORI_2007_VIEW_STATUS

	CNetworkMessage nmMessage((UBYTE)MSG_EVENT);
	nmMessage << eventType;	
	nmMessage << subType;
	SendToServerNew(nmMessage);
}

void CNetworkLibrary::SendMinigameSelect(UBYTE eventType, UBYTE subType, UBYTE cSelect)
{
	CNetworkMessage nmMessage((UBYTE)MSG_EVENT);
	nmMessage << eventType;	
	nmMessage << subType;
	nmMessage << cSelect;
	SendToServerNew(nmMessage);
}

void CNetworkLibrary::SendMinigameSelectGift(ULONG cSelect)
{
	CNetworkMessage nmMessage((UBYTE)MSG_EVENT);
	nmMessage << (UBYTE)MSG_EVENT_GOMDORI_2007;	
	nmMessage << (UBYTE)MSG_EVENT_GOMDORI_2007_SELECT_GIFT;
	nmMessage << (ULONG)cSelect;
	SendToServerNew(nmMessage);
}

// ----------------------------------------------------------------<<

/************************************************************************/
/* Name : SendFlowerTreeSupport                                         */
/************************************************************************/
void CNetworkLibrary::SendFlowerTreeReq(UBYTE subType)
{
	CNetworkMessage nmMessage((UBYTE)MSG_EVENT);
	nmMessage << (UBYTE)MSG_EVENT_FLOWERTREE_2007;
	nmMessage << subType;
	SendToServerNew(nmMessage);
}

// [070613: Su-won] 
// 무기,방어구 교환 카드 사용 요청
void CNetworkLibrary::UseChangeWeaponItem( SWORD nTab, SWORD inven_idx, LONG lItemIndex, LONG lChangeType, LONG lTradeItemIndex )
{
	int tv_tab, tv_idx;
	CUIManager::getSingleton()->GetInventory()->GetUseItemSlotInfo(tv_tab, tv_idx);
	
	CItems* pItems = &MySlotItem[tv_tab][tv_idx];

	if(pItems->Item_Index ==-1) //비어있는 슬롯이다.
		return;
	
	CNetworkMessage nmMessage;
	RequestClient::doItemUseExchangeEquip* packet = reinterpret_cast<RequestClient::doItemUseExchangeEquip*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_EXCHANGE_EQUIP_REQ;
	packet->tab_1 = nTab;
	packet->invenIndex_1 = inven_idx;
	packet->tab_2 = tv_tab;
	packet->invenIndex_2 = tv_idx;
	packet->virtualIndex = lItemIndex;
	packet->changeType = lChangeType;
	packet->exchangeDBIndex = lTradeItemIndex;

	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew( nmMessage );
}

// [070613: Su-won] 
// 펫 명찰 아이템 사용 요청
void CNetworkLibrary::SendPetNameChageReq( int nPetIndex, CTString strPetName )
{
	CNetworkMessage nmMessage( (UBYTE)MSG_EXTEND );
	nmMessage << (ULONG)MSG_EX_PET_CHANGE_NAME;
	
	nmMessage << (SLONG)nPetIndex;
	nmMessage << strPetName;

	SendToServerNew( nmMessage );
}

// [070705: Su-won] EVENT_SUMMBER_2007
// 2007 여름 이벤트: 종이 접기 요청
void CNetworkLibrary::SendPaperFoldingReq(int nItemIndex0, int nItemIndex1, int nItemIndex2)
{
	CNetworkMessage nmMessage((UBYTE)MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_SUMMER_VACATION_2007;
	nmMessage << (UBYTE)MSG_EVENT_SUMMER_VACATION_2007_PAPER_FIGURES;
	nmMessage << (LONG)nItemIndex0;
	nmMessage << (LONG)nItemIndex1;
	nmMessage << (LONG)nItemIndex2;

	SendToServerNew(nmMessage);
}

// [070705: Su-won] EVENT_SUMMBER_2007
// 2007 여름 이벤트: 색종이 조합 요청
void CNetworkLibrary::SendPaperBlendingReq(int nItemIndex)
{
	CNetworkMessage nmMessage((UBYTE)MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_SUMMER_VACATION_2007;
	nmMessage << (UBYTE)MSG_EVENT_SUMMER_VACATION_2007_INCHEN;
	nmMessage << (LONG)nItemIndex;
	
	SendToServerNew(nmMessage);
}
////////////////////////////////////////////////////////////////////////////////////////////
// [070807: Su-won] EVENT_ADULT_OPEN
// 성인서버 오픈 이벤트 관련 보상 요청
//
// Parameter:
// iEvent - 이벤트 번호( 0:매직카드 이벤트, 1:전직 이벤트, 2:연금술 이벤트)
// tv_tab, tv_row, tv_col - 연금술에 사용할 장비의 인벤 위치. 연금술 이벤트에만 사용
//
void CNetworkLibrary::SendAdultOpenEventReq( int iEvent, int tv_tab, int tv_row, int tv_col)
{
	CNetworkMessage nmMessage((UBYTE)MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_OPEN_ADULT_SERVER;
	
	switch( iEvent )
	{
		//매직카드 이벤트
	case 0:
		{
			nmMessage << (UBYTE)MSG_EVENT_OPEN_ADULT_SERVER_MAGIC_REQ;
		}
		break;
		//전직 이벤트
	case 1:
		{
			nmMessage << (UBYTE)MSG_EVENT_OPEN_ADULT_SERVER_CHABGEJOB_EXCHANGE;
		}
		break;
		//연금술 이벤트
	case 2:
		{
			nmMessage << (UBYTE)MSG_EVENT_OPEN_ADULT_SERVER_UPGRADE_REQ;

			nmMessage << (LONG)tv_tab;
			nmMessage << (LONG)tv_row;
			nmMessage << (LONG)tv_col;
		}
		break;
	}

	SendToServerNew(nmMessage);
}
// [070807: Su-won] EVENT_ADULT_OPEN
////////////////////////////////////////////////////////////////////////////////////////////

// WSS_TG2007 2007/09/14 ------------------------------------------------------------>>
void CNetworkLibrary::SendTG2007ScreenshotReq()
{
	CNetworkMessage nmMessage((UBYTE)MSG_EVENT);
	nmMessage << (UBYTE)MSG_EVENT_SSHOT_2007;
	nmMessage << (UBYTE)MSG_EVENT_SSHOT_2007_RECIVE;	
	SendToServerNew(nmMessage);
}

void CNetworkLibrary::SendTG2007RichYearReq(int subType)
{
	CNetworkMessage nmMessage((UBYTE)MSG_EVENT);
	nmMessage << (UBYTE)MSG_EVENT_RICHYEAR;
	nmMessage << (UBYTE)subType;	
	SendToServerNew(nmMessage);
}
// ----------------------------------------------------------------------------------<<
//============================================================================================================
// CNetworkLibrary::SendBJMono2007Req
// SK BJ mono Event
//============================================================================================================
void CNetworkLibrary::SendBJMono2007Req(UBYTE subType, ULONG ulNum)
{
	CNetworkMessage nmMessage((UBYTE)MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_BJMONO_2007;
	nmMessage << subType;

	if (subType==MSG_EVENT_BJMONO_2007_CHANGE_TICKET_REQ)
	{
		nmMessage << ulNum;
	}

	SendToServerNew(nmMessage);
}

//============================================================================================================
// CNetworkLibrary::SendLC1000DayCapUpgradeReq
// 
//============================================================================================================
void CNetworkLibrary::SendLC1000DayHatUpgradeReq(SWORD nIdx, LONG lItemIndex)
{
	CNetworkMessage nmMessage((UBYTE)MSG_EVENT);
	
	nmMessage << (UBYTE)MSG_EVENT_LC_1000DAY;
	nmMessage << (UBYTE)MSG_EVENT_LC_1000DAY_HAT_UPDATE;
//	nmMessage << sbRow << sbCol;
	nmMessage << lItemIndex;

	SendToServerNew(nmMessage);
}

//============================================================================================================
// CNetworkLibrary::SendLC1000DayTakeHatReq
//
//============================================================================================================
void CNetworkLibrary::SendLC1000DayTakeHatReq()
{
	CNetworkMessage nmMessage((UBYTE)MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_LC_1000DAY;
	nmMessage << (UBYTE)MSG_EVENT_LC_1000DAY_HAT;

	SendToServerNew(nmMessage);
}

//============================================================================================================
// CNetworkLibrary::SendTakeFireCracker
//
//============================================================================================================
void CNetworkLibrary::SendTakeFireCracker()
{
	CNetworkMessage nmMessage((UBYTE)MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_LC_1000DAY;
	nmMessage << (UBYTE)MSG_EVENT_LC_1000DAY_FIRECRACKER;

	SendToServerNew(nmMessage);
}

//============================================================================================================
// CNetworkLibrary::SendHalloween2007Req
//
//============================================================================================================
void CNetworkLibrary::SendHalloween2007Req(UBYTE subType)
{
	CNetworkMessage nmMessage((UBYTE)MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_HALLOWEEN_2007;
	nmMessage << (UBYTE)subType;

	SendToServerNew(nmMessage);
}

//[071123: Su-won] DRATAN_SIEGE_DUNGEON
void CNetworkLibrary::SendDratanSiegeDungeonMSG(UBYTE ubSubType, UBYTE ubSubSubType, int nValue)
{
	if (ubSubType == MSG_DUNGEON_ENTER && ubSubSubType == MSG_DUNGEON_GO)
	{
		UIMGR()->SetCSFlagOn(CSF_TELEPORT);
	}

	CNetworkMessage nmMessage( (UBYTE)MSG_EXTEND );
	nmMessage<< (LONG)MSG_EX_DVD;
	nmMessage<< ubSubType;
	if( ubSubType == MSG_MANAGEMENT || ubSubType == MSG_DUNGEON_ENTER)
	{
		nmMessage<< ubSubSubType;

		if( ubSubSubType == MSG_MANAGEMENT_MANAGER_ENV_CHANGE ||
			ubSubSubType == MSG_MANAGEMENT_MANAGER_MONSTER_CHANGE ||
			ubSubSubType == MSG_MANAGEMENT_MANAGER_TAX_CHANGE ||
			ubSubSubType == MSG_MANAGEMENT_MANAGER_HUNTER_TAX_CHANGE ) 
		{
			nmMessage<< (LONG)nValue;
		}
	}
	
	SendToServerNew(nmMessage);
}

// ttos : 판매대행 상인
void CNetworkLibrary::SendCashPersonShop(UBYTE subType)
{
	CNetworkMessage nmMessage((UBYTE)MSG_EXTEND);
	
	nmMessage << (LONG)MSG_EX_ALTERNATE_MERCHANT;
	nmMessage << (UBYTE)subType;

	SendToServerNew(nmMessage);
}

//============================================================================================================
// CNetworkLibrary::SendXMAS2007DecoReq
//
//============================================================================================================
void CNetworkLibrary::SendXMAS2007DecoReq(UBYTE subType)
{
	CNetworkMessage nmMessage((UBYTE)MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_XMAS_2007;
	nmMessage << subType;

	SendToServerNew(nmMessage);
}

//============================================================================================================
// CNetworkLibrary::SendRestartGame
// 웹 실행버전으로 인한 재시작 요청 메세지
//============================================================================================================
void CNetworkLibrary::SendRestartGame()
{
	CNetworkMessage nmMessage((UBYTE)MSG_EXTEND);
	nmMessage << (ULONG)MSG_EX_RESTART;
	nmMessage << (SBYTE)0;

	SendToServerNew( nmMessage );
}

void CNetworkLibrary::SendReceiveRestartGame()
{
	CNetworkMessage nmMessage((UBYTE)MSG_EXTEND);
	nmMessage << (ULONG)MSG_EX_RESTART;
	nmMessage << (SBYTE)1;

	SendToServerNew( nmMessage );
}

//============================================================================================================
// CNetworkLibrary::SendUseGoDungeon
// 던전이동 주문서 사용
//============================================================================================================
void CNetworkLibrary::SendUseGoDungeon(INDEX iItemIndex, INDEX iZone, INDEX iExtra)
{
	UIMGR()->SetCSFlagOn(CSF_TELEPORT);

	CNetworkMessage nmMessage;
	RequestClient::doItemUseWarpDoc* packet = reinterpret_cast<RequestClient::doItemUseWarpDoc*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_USE_WARPDOC;
	packet->virtualIndex = iItemIndex;
	packet->zone = iZone;
	packet->extra = iExtra;
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew( nmMessage );
}

//============================================================================================================
// CNetworkLibrary::SendPresscorpsMessage
// 라카기자단 혜택 관련 (확성기 기능 메세지 전송) 외치기!
//============================================================================================================
void CNetworkLibrary::SendPresscorpsMessage(ULONG ulItemIndex, CTString Sendstr)
{
	if(Sendstr.Length() == 0)
	{
		return;
	}
		
	CTString tmpString = _pNetwork->MyCharacterInfo.name + ": " + Sendstr;

	CNetworkMessage nmMessage;
	RequestClient::doItemUsePressCorps* packet = reinterpret_cast<RequestClient::doItemUsePressCorps*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_USE_PRESSCORPS;
	packet->virtuaIndex = ulItemIndex;

	strcpy(packet->str, tmpString.str_String);
	packet->str[tmpString.Length()] = '\0';

	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew( nmMessage );
}

//============================================================================================================
// CNetworkLibrary::SendUpdatePlayTime
// 플레이 시간 10분간격으로 업데이트 메세지 전달
//============================================================================================================
void CNetworkLibrary::SendUpdatePlayTime(ULONG nTime)
{
	CNetworkMessage nmMessage((UBYTE)MSG_EXTEND);

	nmMessage << (ULONG)MSG_EX_UPDATE_PLAYTIME;
	nmMessage << (ULONG)nTime;

	_pNetwork->SendToServerNew(nmMessage);
}

void CNetworkLibrary::SendItemDelete(SWORD iTab, SWORD inven_idx, INDEX UniIndex, SQUAD count)
{
	CNetworkMessage nmMessage;
	RequestClient::doItemDelete* packet = reinterpret_cast<RequestClient::doItemDelete*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_DELETE;
	packet->tab = iTab;
	packet->invenIndex = inven_idx;
	packet->virualIndex = UniIndex;
	nmMessage.setSize( sizeof(*packet) );

	_pNetwork->SendToServerNew(nmMessage);
}

void CNetworkLibrary::SendUseWildPetItem(int posId, int tabId, int inven_idx, int item_index)
{
	CNetworkMessage nmMessage;
	RequestClient::doExApetItemWear* packet = reinterpret_cast<RequestClient::doExApetItemWear*>(nmMessage.nm_pubMessage);
	packet->type = MSG_EXTEND;
	packet->subType = htonl(MSG_EX_ATTACK_PET);
	packet->thirdType = MSG_SUB_ITEM_WEAR;
	packet->tab = tabId;
	packet->invenIndex = inven_idx;
	packet->itemIdx = item_index;
	packet->wearPos = posId;
	nmMessage.setSize( sizeof(*packet) );

	_pNetwork->SendToServerNew(nmMessage);
}

//////////////////////////////////////////////////////////////////////////
// 피닉스 캐릭터 가능 여부 확인

void CNetworkLibrary::SendPhoenixCharacterCondition()
{
	CNetworkMessage nmMessage((UBYTE)MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_PHOENIX;

	_pNetwork->SendToServerNew(nmMessage);
}

//////////////////////////////////////////////////////////////////////////
// 피닉스 캐릭터 신청

void CNetworkLibrary::SendCreatePhoenixCharacter()
{
	CNetworkMessage nmMessage((UBYTE)MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_PHOENIX_SUCCESS;

	_pNetwork->SendToServerNew(nmMessage);
}


//////////////////////////////////////////////////////////////////////////
//============================================================================================================
// CNetworkLibrary::SendInitSSkillReq
// 스페셜 스킬 초기화 요청
//============================================================================================================
void CNetworkLibrary::SendInitSSkillReq()
{
	CNetworkMessage nmMessage((UBYTE)MSG_EXTEND);
	nmMessage << (ULONG)MSG_EX_INIT_SSKILL;
	_pNetwork->SendToServerNew(nmMessage);
}

//============================================================================================================
// CNetworkLibrary::SendUsingSpeedHack
// 스피드 핵 사용자 서버에 통보
//============================================================================================================
void CNetworkLibrary::SendInput()  // SendUsingSpeedHack --> SendInput
{
	CNetworkMessage nmMessage((UBYTE)MSG_EXTEND);
	nmMessage << (ULONG)MSG_EX_DISCONNECT_HACK_CHARACTER;
	nmMessage << (FLOAT)_pTimer->tmTimeDelay;
	_pNetwork->SendToServerNew(nmMessage);
}

//[ttos_2009_3_18]: 하나포스 및 SK브로드밴드 가입자 이벤트
void CNetworkLibrary::SendHanaposEvent(int nNpcVirIdx)
{
	CNetworkMessage nmMessage;
	RequestClient::skillHanaroEvent* packet = reinterpret_cast<RequestClient::skillHanaroEvent*>(nmMessage.nm_pubMessage);
	packet->type = MSG_SKILL;
	packet->subType = MSG_SKILL_HANARO_EVENT;
	packet->npcIndex = nNpcVirIdx;
	
	nmMessage.setSize( sizeof(*packet) );
	SendToServerNew(nmMessage);
}

void CNetworkLibrary::SendEventRequital(int nRequital)
{
	CNetworkMessage nmMessage((UBYTE)MSG_EVENT);

	nmMessage << (UBYTE)MSG_EVENT_REQUITAL;
	nmMessage << (SLONG)nRequital;

	_pNetwork->SendToServerNew(nmMessage);
}

//=========================================================================
// 초보 지원사 및 중저레벨 지원사 스킬 사용 요청(고레벨 지원사도 같이 사용)
//=========================================================================
void CNetworkLibrary::SendSupportSkill(UBYTE ubMsg)
{
	CNetworkMessage nm;
	RequestClient::skillUseEP* packet = reinterpret_cast<RequestClient::skillUseEP*>(nm.nm_pubMessage);
	packet->type = MSG_SKILL;
	packet->subType = ubMsg;	
	nm.setSize( sizeof(*packet) );
	SendToServerNew(nm);
}

//============================================================================================================
// void SendEventMsg()는 이벤트 메세지 함수가 계속 늘어나는것을 회피하기 위하여 통합 처리를 하도록 합시다.
// CNetwork class에서 이벤트 전송 파라미터를 받아서 한번에 SendEventMsg()로 전송하도록 합시다.
// 전송 파라미터는 각 변수 타입 별로, ULONG, UBYTE, CTString 3가지 타입만으로 전송 처리가 용의하므로(type casting),
// 전송 파라미터 변수는 static_Array<ULONG>, static_array<UBYTE>, static_array<CTString>으로 합시다.
// 이벤트 메세지 패킷 프로토콜을 서버와 논의하여 맞추도록 합시다.
// 이해 안되면 바보~
//============================================================================================================
void CNetworkLibrary::SendEventMsg(UBYTE ubEventType)
{
	CNetworkMessage nmMessage((UBYTE)MSG_EVENT);

	nmMessage << ubEventType;
	_pNetwork->SendToServerNew(nmMessage);
}

//============================================================================================================
// CNetworkLibrary::SendRaidObjectEvent()
// 레이드 시스템 오브젝트 활성화 메세지(클릭 및 진입)
//============================================================================================================
void CNetworkLibrary::SendRaidObjectEvent(ULONG iObjectID)
{
	CNetworkMessage nmMessage((UBYTE)MSG_EXTEND);

	nmMessage << (ULONG)MSG_EX_TRIGGER_EVENT;
	nmMessage << iObjectID;

	_pNetwork->SendToServerNew(nmMessage);
}

//////////////////////////////////////////////////////////////////////////
// [sora] RAID_SYSTEM
//////////////////////////////////////////////////////////////////////////

// [sora] 원정대로 전환 요청
void CNetworkLibrary::ExpeditionCreateReq()
{
	CNetworkMessage	nm( (UBYTE)MSG_EXPEDITION );
	nm << (SBYTE)MSG_CREATE_REQ;

	SendToServerNew( nm );
}

// [sora] 원정대 초대
void CNetworkLibrary::ExpeditionInviteReq( SLONG slIndex )
{
	// Send network message
	CNetworkMessage	nm( (UBYTE)MSG_EXPEDITION );
	nm << (SBYTE)MSG_INVITE_REQ;
	nm << slIndex;

	SendToServerNew( nm );
}

// [sora] 원정대 초대 수락
void CNetworkLibrary::ExpeditionAllowReq()
{
	CNetworkMessage	nm( (UBYTE)MSG_EXPEDITION );
	nm << (SBYTE)MSG_ALLOW_REQ;

	SendToServerNew( nm );
}

// [sora] 원정대 초대 거절
void CNetworkLibrary::ExpeditionRejectReq()
{
	CNetworkMessage	nm( (UBYTE)MSG_EXPEDITION );
	nm << (SBYTE)MSG_REJECT_REQ;

	SendToServerNew( nm );
}

// [sora] 원정대 탈퇴
void CNetworkLibrary::ExpeditionQuitReq()
{
	CNetworkMessage	nm( (UBYTE)MSG_EXPEDITION );
	nm << (SBYTE)MSG_QUIT_REQ;
	nm << (SLONG)MSG_EXPED_QUITMODE_NORMAL;

	SendToServerNew( nm );
}

// [sora] 원정대 추방
void CNetworkLibrary::ExpeditionKickReq( SLONG slIndex )
{
	CNetworkMessage	nm( (UBYTE)MSG_EXPEDITION );
	nm << (SBYTE)MSG_KICK_REQ;
	nm << slIndex;

	SendToServerNew( nm );
}

// [sora] 원정대 해체
void CNetworkLibrary::ExpeditionEndReq()
{
	CNetworkMessage	nm( (UBYTE)MSG_EXPEDITION );
	nm << (SBYTE)MSG_ENDEXPED_REQ;

	SendToServerNew( nm );
}

// [sora] 원정대 분배방식 변경
void CNetworkLibrary::ExpeditionChangeDivisionTypeReq(UBYTE msgType, SBYTE sbExpedType, SBYTE sbDiviType)
{
	CNetworkMessage	nm( msgType );
	nm << (SBYTE)MSG_CHANGETYPE_REQ;
	nm << sbDiviType;
	nm << sbExpedType;

	SendToServerNew( nm );
}

// [sora] 원정대장 변경
void CNetworkLibrary::ExpeditionChangeLeaderReq( SLONG slIndex )
{
	CNetworkMessage	nm( (UBYTE)MSG_EXPEDITION );
	nm << (SBYTE)MSG_CHANGEBOSS_REQ;
	nm << (SLONG)MSG_EXPED_CHANGEBOSS_MANUAL;
	nm << slIndex;

	SendToServerNew( nm );
}

// [sora] 원정 부대장 변경
void CNetworkLibrary::ExpeditionChangeSubLeaderReq(BOOL bIsSubLeader, SLONG slIndex )
{
	CNetworkMessage	nm( (UBYTE)MSG_EXPEDITION );
	if(bIsSubLeader)
	{
		nm << (SBYTE)MSG_SETMBOSS_REQ;	
	}
	else
	{
		nm << (SBYTE)MSG_RESETMBOSS_REQ;
	}
	nm << slIndex;

	SendToServerNew( nm );
}

// [sora] 그룹 변경
void CNetworkLibrary::ExpeditionChangeGroupReq(SLONG slGroupSrc, SLONG slIndex, SLONG slGroupDesc, SLONG slPos )
{
	CNetworkMessage	nm( (UBYTE)MSG_EXPEDITION );
	nm << (SBYTE)MSG_MOVEGROUP_REQ;
	nm << slGroupSrc;
	nm << slIndex;
	nm << slGroupDesc;
	nm << slPos;

	SendToServerNew( nm );
}

// [sora] 멤버 추가
void CNetworkLibrary::ExpeditionAddCharReq( CTString &strCharName )
{
	CNetworkMessage	nm( (UBYTE)MSG_EXPEDITION );
	nm << (SBYTE)MSG_ADDMEMBER_REQ;
	nm << strCharName;

	SendToServerNew( nm );
}


// [sora] 살펴보기
void CNetworkLibrary::ExpeditionViewDetailReq( SLONG slGroup, SLONG slIndex )
{
	CNetworkMessage	nm( (UBYTE)MSG_EXPEDITION );
	nm << (SBYTE)MSG_VIEWDETAIL_REQ;
	nm << slGroup;
	nm << slIndex;

	SendToServerNew( nm );
}


// [sora] 표식 설정, 해제
void CNetworkLibrary::ExpeditionSetLabelReq(SLONG slCharType, SLONG slMode, SLONG slLabelType, SLONG slIndex)
{
	CNetworkMessage	nm( (UBYTE)MSG_EXPEDITION );
	nm << (SBYTE)MSG_SET_LABEL_REQ;
	nm << slCharType;
	nm << slMode;
	nm << slLabelType;
	nm << slIndex;

	SendToServerNew( nm );
}
//------------------------------------------------------------------------------
// CUIParty::ExpeditionCollectQuestItemReq()
// Explain: [sora] 원정대 퀘스트 아이템 수집
//------------------------------------------------------------------------------		 
void CNetworkLibrary::ExpeditionCollectQuestItemReq(SLONG slIndex)
{
	CNetworkMessage	nm( (UBYTE)MSG_EXPEDITION );
	nm << (SBYTE)MSG_QUESTITEM_CHECK_REQ;
	nm << slIndex;

	SendToServerNew( nm );
}

// [sora] 파티 해체
void CNetworkLibrary::PartyEndReq()
{
	CNetworkMessage	nm( (UBYTE)MSG_PARTY );
	nm << (SBYTE)MSG_PARTY_ENDPARTY_REQ;

	SendToServerNew( nm );
}


// [sora] 인던 초기화
void CNetworkLibrary::InitInZoneReq()
{
	CNetworkMessage	nm( (UBYTE)MSG_PARTY );
	nm << (SBYTE)MSG_PARTY_INZONE_CLEAR_REQ;

	SendToServerNew( nm );
}

// [sora] 인존 입장 요청
void CNetworkLibrary::RaidInzoneJoinReq(SLONG slZoneNo, SLONG exParam)
{
	UIMGR()->SetCSFlagOn(CSF_TELEPORT);

	CNetworkMessage	nm( (UBYTE)MSG_RAID );
	nm << (SBYTE)MSG_RAID_INZONE_JOIN;
	nm << slZoneNo;

	if (exParam >= 0)
	{
		nm << exParam;
	}

	SendToServerNew( nm );
}

// [sora] 인존 나가기 요청
// [100125 sora] 인존 나갈때 존번호를 지정할 수 있도록 수정
void CNetworkLibrary::RaidInzoneQuitReq(SLONG slZoneNo /* = -1 */, SLONG slExtraNo)
{
	UIMGR()->SetCSFlagOn(CSF_TELEPORT);

	CNetworkMessage	nm( (UBYTE)MSG_RAID );
	nm << (SBYTE)MSG_RAID_INZONE_QUIT;
	nm << (SLONG)1;
	nm << (SLONG)0;
	nm << slZoneNo;
	nm << slExtraNo;

	SendToServerNew( nm );
}
//////////////////////////////////////////////////////////////////////////
void CNetworkLibrary::SendMakeItemList(ULONG sealtype, UBYTE UItype)
{
	CNetworkMessage	nmMessage((UBYTE)MSG_FACTORY);

	nmMessage << (UBYTE)MSG_FACTORY_ITEM_LIST;
	nmMessage << (UBYTE)UItype;		// 0: NPC에서 요청 1: 만들기에서 요청
	nmMessage << (ULONG)sealtype;

	_pNetwork->SendToServerNew(nmMessage);

}
//////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
//  Name 	: SendAffinityConnectReq()
//  Desc 	: 친구 요청.
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendAffinityConnectReq( SLONG npcIdx )
{
	CNetworkMessage nm( (UBYTE)MSG_EXTEND );
	nm << (ULONG)MSG_EX_AFFINITY;
	nm << (UBYTE)MSG_EX_AFFINITY_CONNECT_REQ;
	nm << (SLONG)npcIdx;

	SendToServerNew( nm );
}

// ----------------------------------------------------------------------------
//  Name 	: SendAffinityShopReq()
//  Desc 	: request shop used permission.
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendAffinityShopReq( SLONG npcIdx )
{
	CNetworkMessage nm( (UBYTE)MSG_EXTEND );
	nm << (ULONG)MSG_EX_AFFINITY;
	nm << (UBYTE)MSG_EX_AFFINITY_SHOPUSE_REQ;
	nm << (SLONG) npcIdx;

	SendToServerNew( nm );
}

// ----------------------------------------------------------------------------
//  Name 	: SendAffinityGiftInfoReq()
//  Desc 	: request next present information.
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendAffinityGiftInfoReq( SLONG npcIdx )
{
	// 친화도 개편2 보상 요청 [2/5/2013 Ranma]
	CNetworkMessage nm( (UBYTE)MSG_EXTEND );
	nm << (ULONG)MSG_EX_AFFINITY;
	nm << (UBYTE)MSG_EX_AFFINITY_GIFTINFO_REQ;
	nm << (SLONG)npcIdx;

	SendToServerNew( nm );
}

// ----------------------------------------------------------------------------
//  Name 	: SendAffinityGiftReq()
//  Desc 	: request to take a present.
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendAffinityGiftReq( SLONG npcIdx )
{
	CNetworkMessage nm( (UBYTE)MSG_EXTEND );
	nm << (ULONG)MSG_EX_AFFINITY;
	nm << (UBYTE)MSG_EX_AFFINITY_GIFT_REQ;
	nm << (SLONG)npcIdx;

	SendToServerNew( nm );
}

// ----------------------------------------------------------------------------
//  Name 	: SendAffinityInfoTabReq()
//  Desc 	: 친화도 개편2 친화도 보상 정보 요청
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendAffinityInfoTabReq( SLONG npcIdx )
{
	CNetworkMessage nm( (UBYTE)MSG_EXTEND );
	nm << (ULONG)MSG_EX_AFFINITY;
	nm << (UBYTE)MSG_EX_AFFINITY_INFOTAB_REQ;
	nm << (SLONG)npcIdx;
	
	SendToServerNew( nm );
}

// ----------------------------------------------------------------------------
//  Name 	: ClearAffinityData()
//  Desc 	: 
// ---------------------------------------------------------------------------- 
void CNetworkLibrary::ClearAffinityData()
{
	AffinityInfo.count = 0;
	AffinityInfo.point = 0;
	AffinityInfo.mapAffinityList.clear();
}

void CNetworkLibrary::SendNPCPortalGoReq(SLONG slIndex)
{
	CNetworkMessage nm((UBYTE)MSG_EXTEND);

	nm << (SLONG) MSG_EX_NPC_PORTAL_SCROLL;
	nm << (SLONG) MSG_NPC_PORTAL_GO;
	nm << (SLONG) slIndex;

	SendToServerNew(nm);
}


void CNetworkLibrary::SendNPCPortalLocationReq(SLONG slIndex)
{
	CNetworkMessage nm((UBYTE)MSG_EXTEND);
	nm << (SLONG) MSG_EX_NPC_PORTAL_SCROLL;
	nm << (SLONG) MSG_NPC_PORTAL_LOCATION;
	nm << (SLONG) slIndex;

	SendToServerNew(nm);
}

// ----------------------------------------------------------------------------
//  Name 	: SendRaidInfoReq()
//  Desc 	: 자신이 속해있는 레이드 정보를 요청
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendRaidInfoReq()
{
	CNetworkMessage	nm( (UBYTE)MSG_EXTEND );
	nm << (ULONG)MSG_EX_RAID_INFO;

	SendToServerNew( nm );
}

// ----------------------------------------------------------------------------
//  Name 	: SendCheckComposition()
//  Desc 	: 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendCheckComposition(SWORD nTab, SWORD nInvenIdx, SLONG slUniIndex)
{
	CNetworkMessage nmMessage;
	RequestClient::doItemCheckComposition* packet = reinterpret_cast<RequestClient::doItemCheckComposition*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_CHECK_COMPOSITION;
	packet->tab = nTab;
	packet->invenIndex = nInvenIdx;
	packet->virtualIndex = slUniIndex;
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew( nmMessage );
}

// ----------------------------------------------------------------------------
//  Name 	: SendNickNameMessage()
//  Desc 	: 호칭 관련 메시지 보내기
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendNickNameMessage( SLONG nType, SLONG iNickIndex )
{
	CNetworkMessage nmNick((UBYTE)MSG_EXTEND);
	nmNick << (SLONG)MSG_EX_TITLE_SYSTEM;
	nmNick << nType;
	
	switch(nType)
	{
	//case MSG_EX_TITLE_SYSTEM_LIST_REQ:			// 호칭 리스트 요청
	case MSG_EX_TITLE_SYSTEM_TITLE_SELECT_REQ:	// 호칭 선택
	case MSG_EX_TITLE_SYSTEM_TITLE_CANCEL_REQ:	// 호칭 취소
	case MSG_EX_TITLE_SYSTEM_TITLE_DELETE_REQ:	// 호칭 삭제
		{
			nmNick << iNickIndex;
		}
		break;
	}

	SendToServerNew(nmNick);
}


// ----------------------------------------------------------------------------
// [100208: selo]
//  Name 	: SendRestoreAbandonQuest()
//  Desc 	: 포기한 퀘스트 복구 요청
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendRestoreAbandonQuest(SQUAD llMoney)
{
	CNetworkMessage nm((UBYTE)MSG_QUEST);
	nm << (UBYTE)MSG_QUEST_RESTORE_ABANDON;
	nm << llMoney;

	SendToServerNew(nm);
}

// ----------------------------------------------------------------------------
// [100208: selo]
//  Name 	: SendTakeAgainQuestItem()
//  Desc 	: 퀘스트 아이템 다시 받기
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendTakeAgainQuestItem()
{
	CNetworkMessage nm((UBYTE)MSG_EXTEND);
	nm << (SLONG)MSG_EX_TAKE_AGAIN_QUEST_ITEM;

	SendToServerNew(nm);
}

// ----------------------------------------------------------------------------
// [100324: sora]
//  Name 	: SendLacaretteTokenReq()
//  Desc 	: 토큰 받기
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendLacaretteTokenReq()
{
	CNetworkMessage nm((UBYTE)MSG_EXTEND);
	nm << (SLONG)MSG_EX_LACARETTE;
	nm << (UBYTE)MSG_EX_LACARETTE_TOKEN_REQ;

	SendToServerNew(nm);
}

// ----------------------------------------------------------------------------
// [100324: sora]
//  Name 	: SendLacaretteRetteReq()
//  Desc 	: 라카렛 결과 요청
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendLacaretteRetteReq( SLONG courseNum, SLONG couseIndex, SLONG tokenIndex )
{
	CNetworkMessage nm((UBYTE)MSG_EXTEND);
	nm << (SLONG)MSG_EX_LACARETTE;
	nm << (UBYTE)MSG_EX_LACARETTE_RETTE_REQ;
	nm << (SLONG)courseNum;
	nm << (SLONG)couseIndex;
	nm << (SLONG)tokenIndex;

	SendToServerNew(nm);
}

// ----------------------------------------------------------------------------
// [100324: sora]
//  Name 	: SendLacaretteUseCountReq()
//  Desc 	: 라카렛 이용 횟수 요청
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendLacaretteUseCountReq()
{
	CNetworkMessage nm((UBYTE)MSG_EXTEND);
	nm << (SLONG)MSG_EX_LACARETTE;
	nm << (UBYTE)MSG_EX_LACARETTE_UI_REQ;

	SendToServerNew(nm);
}

// ----------------------------------------------------------------------------
// [100325: sora]
//  Name 	: SendLacaretteResultReq()
//  Desc 	: 라카렛 당첨 아이템 지급요청
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendLacaretteResultReq( INDEX itemIndex )
{
	CNetworkMessage nm((UBYTE)MSG_EXTEND);
	nm << (SLONG)MSG_EX_LACARETTE;
	nm << (UBYTE)MSG_EX_LACARETTE_RESULT_REQ;
	nm << (SLONG)itemIndex;

	SendToServerNew(nm);
}

void CNetworkLibrary::SendLacaretteCloseReq()
{
	CNetworkMessage nm((UBYTE)MSG_EXTEND);
	nm << (SLONG)MSG_EX_LACARETTE;
	nm << (UBYTE)MSG_EX_LACARETTE_UI_COLSE;

	SendToServerNew(nm);
}

void CNetworkLibrary::SendLacaretteTokenReadyReq()
{
	CNetworkMessage nm((UBYTE)MSG_EXTEND);
	nm << (SLONG)MSG_EX_LACARETTE;
	nm << (UBYTE)MSG_EX_LACARETTE_TOKEN_READY_REQ;

	SendToServerNew(nm);
}

// ------------------------------------------------------------------
//  [5/13/2010 selo0530] ($E_WC2010)
// 
//	Name : SendWorldCup2010_Event
//  Desc : 2010 남아공 월드컵 Event Message
//  Info : 
//		Trade : iParam(tradeType - 1. 축구공 -> 황금 축구공, 2. 황금 축구공 -> 축제 상자)
//		Status Req : iParam(resultType - 1. 우승 국가 응모 전 현황 확인	2. 응모 현황 확인)
//		Toto Req : iParam(nItemIndex1 - 국기 아이템 인덱스)
//		Gift Req : iParam(null) [토토 보상 요청]
//		Attendance Req : iParam(null) [응원 출석]
//		SupportCard Req : iParam(supportType) [응원카드 보상 Type]
// ------------------------------------------------------------------
void CNetworkLibrary::SendWorldCup2010_Event(MSG_EVENT_WORLDCUP2010_TYPE e_Type, INDEX iParam /* = 0 */)
{
	CNetworkMessage nmEvent((UBYTE)MSG_EVENT);

	nmEvent << (UBYTE)MSG_EVENT_WORLDCUP_2010;
	nmEvent << (UBYTE)e_Type;

	if (iParam > 0)
	{
		nmEvent << iParam;
	}

	SendToServerNew(nmEvent);
}

// ----------------------------------------------------------------------------
//  [2010/06/30 : Sora]
//  Name 	: SendOwnerGuildBuffReq()
//  Desc 	: 성주 길드 버프 부여 요청
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendOwnerGuildBuffReq()
{
	CNetworkMessage nmGuild((UBYTE)MSG_GUILD);

	nmGuild << (UBYTE)MSG_CASTLE_BUFF;
	SendToServerNew(nmGuild);
}

// ----------------------------------------------------------------------------
// [7/2/2010 kiny8216] ATTENDANCE_SYSTEM : 출석 시스템
//  Name 	: SendAttendanceReq()
//  Desc 	: 누적 출석일수 요청
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendAttendanceReq( INDEX subType )
{
	CNetworkMessage nm;

	switch(subType)
	{
	case 0: // 누적 현황
		{
			RequestClient::AttendanceCheck* packet = reinterpret_cast<RequestClient::AttendanceCheck*>(nm.nm_pubMessage);
			packet->type = MSG_EXTEND;
			packet->subType = MSG_EX_ATTENDANCE_EXP_SYSTEM;
			packet->thirdType = MSG_SUB_ATTENDANCE_CHECK_REQ;
			nm.setSize( sizeof(*packet) );
		}
		break;

	case 1: // 보상 받기
		{
			RequestClient::AttendanceReward* packet = reinterpret_cast<RequestClient::AttendanceReward*>(nm.nm_pubMessage);
			packet->type = MSG_EXTEND;
			packet->subType = MSG_EX_ATTENDANCE_EXP_SYSTEM;
			packet->thirdType = MSG_SUB_ATTENDANCE_REWARD_REQ;
			nm.setSize( sizeof(*packet) );
		}
		break;

	case 2: // 출석 보험 아이템 해제
		{
			RequestClient::AttendanceAssureOk* packet = reinterpret_cast<RequestClient::AttendanceAssureOk*>(nm.nm_pubMessage);
			packet->type = MSG_EXTEND;
			packet->subType = MSG_EX_ATTENDANCE_EXP_SYSTEM;
			packet->thirdType = MSG_SUB_ATTENDANCE_ASSURE_CANCEL_OK_REQ;
			nm.setSize( sizeof(*packet) );
		}
		break;
	}

	SendToServerNew(nm);
}

// ----------------------------------------------------------------------------
// [7/15/2010 kiny8216] MONSTER_ENERGY_IGNITION_SYSTEM
//  Name 	: SendEnergySkillMessage()
//  Desc 	: 몬스터 에너지 스킬 사용 메시지
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendEnergySkillMessage()
{
	CNetworkMessage nm;
	RequestClient::skillUseEP* packet = reinterpret_cast<RequestClient::skillUseEP*>(nm.nm_pubMessage);
	packet->type = MSG_SKILL;
	packet->subType = MSG_SKILL_USEEP;	
	nm.setSize( sizeof(*packet) );
	SendToServerNew(nm);
}

// ----------------------------------------------------------------------------
// [100901] 아칸사원 레이드 Scene Message
//  Name 	: SendRaidScene()
//  Desc 	: 월드 안에 존재하는 Scene Active로 설정된 오브젝트가 보내는 메시지(TouchField, ItemObjects)
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendRaidScene(INDEX ObjType, INDEX Id, INDEX iData)
{
	CNetworkMessage nm((UBYTE)MSG_EXTEND);
	const INDEX iOBJECT_TYPE_TODO = 0;
	nm << (SLONG)MSG_EX_RAID_SCENE;
	nm << iOBJECT_TYPE_TODO;
	nm << ObjType;
	nm << Id;
	nm << iData;

	SendToServerNew(nm);
}

// ----------------------------------------------------------------------------
// [8/31/2010 kiny8216] 후견인 시스템 개편
//  Name 	: TeachGiftReq()
//  Desc 	: 후견인 보상 요청
// ----------------------------------------------------------------------------
void CNetworkLibrary::TeachGiftReq()
{
	CNetworkMessage nmTeach((UBYTE)MSG_TEACH);
	nmTeach << (UBYTE)MSG_TEACH_RENEWER_TEACH_GIFT;	
	SendToServerNew(nmTeach);
}

// ----------------------------------------------------------------------------
// [9/29/2010 kiny8216] 성주 코스튬
//  Name 	: LordCostumeReq()
//  Desc 	: 성주 코스튬 요청
// ----------------------------------------------------------------------------
void CNetworkLibrary::LordCostumeReq()
{
	CNetworkMessage nmTeach((UBYTE)MSG_EXTEND);
	nmTeach << (INDEX)MSG_EX_CASTLLAN;
	nmTeach << (UBYTE)MSG_EX_CASTLLAN_GIVE_ITEM_REQ;
	SendToServerNew(nmTeach);
}
// 성주 길드 코스튬 [1/21/2011 ldy1978220]
void CNetworkLibrary::LordGuildCostumeReq()
{
	CNetworkMessage nmTeach((UBYTE)MSG_EXTEND);
	nmTeach << (INDEX)MSG_EX_CASTLLAN;
	nmTeach << (UBYTE)MSG_EX_CASTLLAN_GIVE_ITEM_GUILD_COSTUME_REQ;
	SendToServerNew(nmTeach);
}

void CNetworkLibrary::LordGuildTitleReq()
{
	CNetworkMessage nmTeach((UBYTE)MSG_EXTEND);
	nmTeach << (INDEX)MSG_EX_CASTLLAN;
	nmTeach << (UBYTE)MSG_EX_CASTLLAN_GIVE_ITEM_GUILD_TITLE_REQ;
	SendToServerNew(nmTeach);
}

// ----------------------------------------------------------------------------
//  [2010/08/25 : Sora] ADD_SUBJOB
//  Name 	: SendSubJobRegister()
//  Desc 	: 보조직업 등록 요청
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendSubJobRegister(SLONG subJobCode)
{
	CNetworkMessage nm((UBYTE)MSG_EXTEND);
	nm << (SLONG)MSG_EX_SUBJOB;
	nm << (SBYTE)MSG_EX_SUBJOB_REGIST;
	nm << (SLONG)subJobCode;
	SendToServerNew(nm);
}

//------------------------------------------------------------------------------
// [10/6/2010 kiny8216] 쿠폰 프로모션 이벤트
// Name		:  SendPromotionEventReq()
// Desc		:  쿠폰 프로모션 이벤트 아이템 요청
//------------------------------------------------------------------------------
void CNetworkLibrary::SendPromotionEventReq(CTString strNum)
{
	CNetworkMessage nmMessage( (UBYTE)MSG_EVENT );
	nmMessage << (UBYTE)MSG_EVENT_PROMOTION2;
	nmMessage << (UBYTE)MSG_EVENT_PROMOTION2_COUPON_USE_REQ;
	nmMessage << strNum;
	
	SendToServerNew(nmMessage);
}

//added by sam 10/11/11
void CNetworkLibrary::SendRankingLsit ( INDEX iIndex, INDEX iPageIndex )
{
	CNetworkMessage nmMessage( (UBYTE)MSG_EXTEND );
	nmMessage << (INDEX)MSG_EX_RANKING_SYSTEM;
	nmMessage << (UBYTE)MSG_EX_RANKING_LIST_REQ;
	nmMessage << iIndex;	
	nmMessage << iPageIndex;
	SendToServerNew(nmMessage);
}

void CNetworkLibrary::SendGuildWarTimeMenu()
{
	CNetworkMessage nmGuild((UBYTE)MSG_GUILD);

	nmGuild << (UBYTE)MSG_GUILD_WAR_SET_TIME_MENU;
	SendToServerNew(nmGuild);
}

void CNetworkLibrary::SendGuildWarTimeReq()
{
	CNetworkMessage nmGuildWar( (UBYTE)MSG_GUILD );
	nmGuildWar << (UBYTE)MSG_GUILD_WAR_GET_TIME_UNI_REQ;
		
	SendToServerNew(nmGuildWar);
}

void CNetworkLibrary::SendFaceOffMessage( UBYTE ubType, UBYTE ubFaceType, UBYTE ubHairType )
{
	CNetworkMessage nmFaceOff((UBYTE)MSG_EXTEND);
	nmFaceOff << (INDEX)MSG_EX_FACEOFF;
	nmFaceOff << (UBYTE)ubType;
	
	if ( ubType == MSG_EX_FACEOFF_REQ )
	{
		nmFaceOff << ubHairType;
		nmFaceOff << ubFaceType;
	}

	SendToServerNew( nmFaceOff );
}

void CNetworkLibrary::SendWildPetMountReq(const BOOL bMount )
{
	CNetworkMessage	nmWildPetMount( (UBYTE)MSG_EXTEND );
	nmWildPetMount << (ULONG)MSG_EX_ATTACK_PET;
	nmWildPetMount << (SBYTE)MSG_SUB_MOUNT_REQ;
	nmWildPetMount << (SBYTE)( bMount?0:1);

	CUIManager::getSingleton()->SetCSFlagOn(CSF_PETRIDING);

	SendToServerNew( nmWildPetMount );
}

// ----------------------------------------------------------------------------
// [2011/01/18 : Sora] 출석 이벤트 2011
//  Name 	: SendAttendanceRewardReq()
//  Desc 	: 출석 이벤트 개인 보상
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendAttendanceRewardReq()
{
	CNetworkMessage nmEvent((UBYTE)MSG_EVENT);
	nmEvent << (UBYTE)MSG_EVENT_ATTENDANCE_2011;
	nmEvent << (UBYTE)MSG_EVENT_ATTENDANCE_2011_INDIVIDUAL_REWARD_REQ;

	SendToServerNew(nmEvent);
}

// ----------------------------------------------------------------------------
// [2011/02/09 : Sora] 퀘스트 아이템 다시 받기
//  Name 	: SendRestoreQuestItem()
//  Desc 	: 완료된 퀘스트의 보상아이템을 다시 받는다
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendRestoreQuestItem( SLONG questIndex )
{
	CNetworkMessage nm((UBYTE)MSG_QUEST);
	nm << (UBYTE)MSG_QUEST_ITEM_REQ;
	nm << questIndex;

	SendToServerNew(nm);
}

// ----------------------------------------------------------------------------
//  Name 	: SendComebackMessage()
//  Desc 	: 휴면 계정 보상 이벤트 메시지
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendComebackMessage()
{
	CNetworkMessage nmComeback( (UBYTE)MSG_EVENT );
	nmComeback << (UBYTE)MSG_EVENT_USER_COMEBACK;

	SendToServerNew( nmComeback );
}
// ----------------------------------------------------------------------------
//  Name 	: SendBirthdayMessage()
//  Desc 	: 생일 선물 관련 메시지
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendBirthdayMessage( INDEX index )
{
	CNetworkMessage nmBirthday( (UBYTE)MSG_EVENT );
	nmBirthday << (UBYTE)MSG_EVENT_CHAR_BIRTHDAY;

	if ( index == EVENT_BIRTHDAY_GIFT )
	{	// 선물 요청
		nmBirthday << (SBYTE)MSG_EVENT_CHAR_BIRTHDAY_GIFT_REQ;
	}
	else if ( index == EVENT_BIRTHDAY_INFO )
	{	// 정보 요청
		nmBirthday << (SBYTE)MSG_EVENT_CHAR_BIRTHDAY_BDAY_REQ;
	}

	SendToServerNew( nmBirthday );
}

// royal rumble [4/19/2011 rumist]
// ----------------------------------------------------------------------------
//  Name 	: SendRoyalRumbleJoinReq()
//  Desc 	: 로얄 럼블 참가 신청
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendRoyalRumbleJoinReq()
{
	CNetworkMessage nmRoyal( (UBYTE)MSG_EXTEND );
	nmRoyal << (INDEX)MSG_EX_ROYAL_RUMBLE;
	nmRoyal << (UBYTE)MSG_EX_ROYAL_RUMBLE_PLAYER_REQ;

	SendToServerNew( nmRoyal );
}
// ----------------------------------------------------------------------------
//  Name 	: SendRoyalRUmbleRejectReq()
//  Desc 	: 로얄 럼블 참가 신청 취소
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendRoyalRumbleRejectReq()
{
	CNetworkMessage nmRoyal( (UBYTE)MSG_EXTEND );
	nmRoyal << (INDEX)MSG_EX_ROYAL_RUMBLE;
	nmRoyal << (UBYTE)MSG_EX_ROYAL_RUMBLE_PLAYER_UNREGIST_REQ;

	SendToServerNew( nmRoyal );
}

// ----------------------------------------------------------------------------
//  Name 	: SendRoyalRumbleRewardReq()
//  Desc 	: 로얄 럼블 보상 신청
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendRoyalRumbleRewardReq()
{
	CNetworkMessage nmRoyal( (UBYTE)MSG_EXTEND );
	nmRoyal << (INDEX)MSG_EX_ROYAL_RUMBLE;
	nmRoyal << (UBYTE)MSG_EX_ROYAL_RUMBLE_REWARD_REQ;

	SendToServerNew( nmRoyal );
}
// ----------------------------------------------------------------------------
//  Name 	: SendRoyalRumblePointReq()
//  Desc 	: 로얄 럼블 전장 포인트 확인.
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendRoyalRumblePointReq()
{
	CNetworkMessage nmRoyal( (UBYTE)MSG_EXTEND );
	nmRoyal << (INDEX)MSG_EX_ROYAL_RUMBLE;
	nmRoyal << (UBYTE)MSG_EX_ROYAL_RUMBLE_POINT_REQ;

	SendToServerNew( nmRoyal );
}

// ----------------------------------------------------------------------------
//  Name 	: SendRoyalRumbleNextTimeReq()
//  Desc 	: 로얄 럼블 다음 경기 일정 조회.
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendRoyalRumbleNextTimeReq()
{
	CNetworkMessage nmRoyal( (UBYTE)MSG_EXTEND );
	nmRoyal << (INDEX)MSG_EX_ROYAL_RUMBLE;
	nmRoyal << (UBYTE)MSG_EX_ROYAL_RUMBLE_TIME_REQ;

	SendToServerNew( nmRoyal );
}

// ----------------------------------------------------------------------------
//  Name 	: SendRoyalRumbleStartReq()
//  Desc 	: 로얄럼블 대기실 이동.
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendRoyalRumbleStartReq()
{
	UIMGR()->SetCSFlagOn(CSF_TELEPORT);

	CNetworkMessage nmRoyal( (UBYTE)MSG_EXTEND );
	nmRoyal << (INDEX)MSG_EX_ROYAL_RUMBLE;
	nmRoyal << (UBYTE)MSG_EX_ROYAL_RUMBLE_GO_ZONE;

	SendToServerNew( nmRoyal );
}

// ----------------------------------------------------------------------------
//  Name 	: SendMasterStoneUseReq()
//  Desc 	: 마스터 스톤 사용 요청.
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendMasterStoneUseReq(SWORD nMStoneTab, SWORD ubMStoneIdx, SLONG slMStone, 
						SWORD nTargetTab, SWORD ubItemTgtIdx, SLONG slItemTgt)
{
	CNetworkMessage nmMessage;
	RequestClient::doItemUseMasterStoneUSA* packet = reinterpret_cast<RequestClient::doItemUseMasterStoneUSA*>(nmMessage.nm_pubMessage);
	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_MASTERSTONE_USE;
	packet->tab_1 = nMStoneTab;
	packet->invenIndex_1 = ubMStoneIdx;
	packet->itemDBIndex_1 = slMStone;
	packet->tab_2 = nTargetTab;
	packet->invenIndex_2 = ubItemTgtIdx;
	packet->itemDBIndex_2 = slItemTgt;
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew( nmMessage );
}

// ----------------------------------------------------------------------------
//  Name 	: SendGuildMarkWndOpenReq()
//  Desc 	: 길드마크 창 오픈 요청
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendGuildMarkWndOpenReq()
{
	CNetworkMessage nmGuild((UBYTE)MSG_GUILD);
	nmGuild << (UBYTE)MSG_NEW_GUILD_MARK_EDIT_WND;
	SendToServerNew(nmGuild);
}

// ----------------------------------------------------------------------------
//  Name 	: SendGuildMarkEditEndReq()
//  Desc 	: 길드마크 편집완료
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendGuildMarkEditEndReq( SBYTE gm_row, SBYTE gm_col, SBYTE bg_row, SBYTE bg_col, SWORD nTab, SWORD nInvenIdx )
{
	CNetworkMessage nmMessage;
	RequestClient::doNewGuildSkillMarkEditFinish* packet = reinterpret_cast<RequestClient::doNewGuildSkillMarkEditFinish*>(nmMessage.nm_pubMessage);
	packet->type = MSG_GUILD;
	packet->subType = MSG_NEW_GUILD_MARK_EDIT_FIN;
	packet->gm_row = gm_row;
	packet->gm_col = gm_col;
	packet->bg_row = bg_row;
	packet->bg_col = bg_col;
	packet->tab = nTab;
	packet->invenIndex = nInvenIdx;
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew(nmMessage);
}

// ----------------------------------------------------------------------------
//  Name 	: SendRankingListEx()
//  Desc 	: 랭킹 리스트 요청
// ----------------------------------------------------------------------------

void CNetworkLibrary::SendRankingListEx(UCHAR ucType, UCHAR ucSubType)
{
	CNetworkMessage	nmRanking((UBYTE)MSG_EXTEND);

	nmRanking << (LONG)MSG_EX_RANKING_SYSTEM_EX;
	nmRanking << (UCHAR)MSG_EX_RANKING_LIST;
	nmRanking << (UCHAR)ucType;
	nmRanking << (UCHAR)ucSubType;

	SendToServerNew(nmRanking);
}

// ----------------------------------------------------------------------------
//  Name 	: SendRankingListEx()
//  Desc 	: 랭킹 검색 리스트 요청
// ----------------------------------------------------------------------------

void CNetworkLibrary::SendRankingSearchListEx(UCHAR ucType, UCHAR ucSubType, CTString pcstrSearch)
{
	CNetworkMessage	nmRanking((UBYTE)MSG_EXTEND);

	nmRanking << (LONG)MSG_EX_RANKING_SYSTEM_EX;
	nmRanking << (UCHAR)MSG_EX_RANKING_SEARCH;
	nmRanking << (UCHAR)ucType;
	nmRanking << (UCHAR)ucSubType;
	nmRanking << pcstrSearch;

	SendToServerNew(nmRanking);
}

void CNetworkLibrary::pkPenaltyReformRewardReq(LONG titlenum)
{
	CNetworkMessage nmPKReward((UBYTE)MSG_EXTEND);
	nmPKReward << (LONG)MSG_EX_PKPENALTY_REFORM;
	nmPKReward << (UBYTE)MSG_EX_PKPENALTY_REFORM_REWARD_REQ;
	nmPKReward << (LONG)titlenum;
	SendToServerNew(nmPKReward);

}

// ----------------------------------------------------------------------------
//  Name 	: SendEventKrathongReq()
//  Desc 	: 크라통 이벤트 재료교환 요청
// ----------------------------------------------------------------------------

void CNetworkLibrary::SendEventKrathongReq()
{
	CNetworkMessage	nmKrathong((UBYTE)MSG_EVENT);
	nmKrathong << (UBYTE)MSG_EVENT_LOI_KRATHONG;

	SendToServerNew(nmKrathong);
}

// ----------------------------------------------------------------------------
//	[2011/11/14 : Sora]
//  Name 	: SendKBRewardReq()
//  Desc 	: 태국 국왕 탄생일 이벤트 아이템 교환 요청 
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendKBRewardReq( UCHAR commandNum )
{
	CNetworkMessage nmEvent((UBYTE)MSG_EVENT);
	nmEvent << (UCHAR)MSG_EVENT_KB_REWARD;
	nmEvent << (UCHAR)commandNum;	// 0은 탄생일 선물상자로 교환 1은 의상상자로 교환

	SendToServerNew(nmEvent);
}


// ----------------------------------------------------------------------------
//  Name 	: SendUserNotice()
//  Desc 	: 유저 공지 메세지 전송
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendUserNotice(CTString strMessage)
{
	CNetworkMessage	nmUserNotice(MSG_EXTEND);
	nmUserNotice << (LONG)MSG_EX_USER_NOTICE;
	nmUserNotice << (LONG)MSG_EX_USER_NOTICE_REG_REQ;
	nmUserNotice << (CTString)strMessage;

	SendToServerNew(nmUserNotice);
}

// ----------------------------------------------------------------------------
//  Name 	: SendGuildMasterKickReq()
//  Desc 	: 길드 마스터를 추방 신청한다
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendGuildMasterKickReq(INDEX idxGuild)
{
	CNetworkMessage nmGuildMKick(MSG_GUILD);
	nmGuildMKick << (UCHAR)MSG_GUILD_MASTER_KICK_REQ;
	nmGuildMKick << (INDEX)idxGuild;
	SendToServerNew(nmGuildMKick);
}

// ----------------------------------------------------------------------------
//  Name 	: SendGuildMasterKickCancelReq()
//  Desc 	: 길드 마스터가 추방에 이의제기한다
// ----------------------------------------------------------------------------
void CNetworkLibrary::SendGuildMasterKickCancelReq(INDEX idxGuild)
{
	CNetworkMessage nmGuildMKick(MSG_GUILD);
	nmGuildMKick << (UCHAR)MSG_GUILD_MASTER_KICK_CANCEL_REQ;
	nmGuildMKick << (INDEX)idxGuild;
	SendToServerNew(nmGuildMKick);
}

void CNetworkLibrary::SendGuildRemoteJoinReq(CTString strTargetName, LONG lType)
{
	CNetworkMessage nmGuild(MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_REMOTE_JOIN_REQ;
	nmGuild << strTargetName;
	nmGuild << MyCharacterInfo.name;
	nmGuild << lType;
	SendToServerNew(nmGuild);
}

void CNetworkLibrary::SendGuildRemoteJoinOKReq(CTString strTargetName, LONG lType)
{
	CNetworkMessage nmGuild(MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_REMOTE_JOIN_OK;
	nmGuild << strTargetName;
	nmGuild << lType;
	SendToServerNew(nmGuild);
}

void CNetworkLibrary::SendGuildRemoteJoinNOReq()
{
	CNetworkMessage nmGuild(MSG_GUILD);
	nmGuild << (UBYTE)MSG_GUILD_REMOTE_JOIN_NO;
	SendToServerNew(nmGuild);
}

void CNetworkLibrary::SendGuildRecallReq()
{
	CNetworkMessage nmMessage;
	pTypeBase* packet = reinterpret_cast<pTypeBase*>(nmMessage.nm_pubMessage);
	packet->type = MSG_GUILD;
	packet->subType = MSG_GUILD_ROOM_RECALL;	
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew( nmMessage );
}
//	FD_TEST 김영환 : 기능함수 추가
void	CNetworkLibrary::Set_MyChar_MorphStatus_EVOCATION_CONVERTING()
{
	MyCharacterInfo.eMorphStatus		= CNetworkLibrary::MyChaInfo::eMORPH_EVOCATION_CONVERTING;
	return;
}

void	CNetworkLibrary::Set_MyChar_MorphStatus_EVOCATION()
{
	MyCharacterInfo.eMorphStatus		= CNetworkLibrary::MyChaInfo::eMORPH_EVOCATION;
}

void	CNetworkLibrary::Set_MyChar_MorphStatus_MORPH_END()
{
	MyCharacterInfo.eMorphStatus		= CNetworkLibrary::MyChaInfo::eMORPH_END;
}

bool	CNetworkLibrary::Is_MyChar_MorphStatus_MORPH_END()
{
	if(MyCharacterInfo.eMorphStatus	== CNetworkLibrary::MyChaInfo::eMORPH_END)
		return true;
	return false;
}

void	CNetworkLibrary::Set_MyChar_MorphStatus_TRANSFORMATION()
{
	MyCharacterInfo.eMorphStatus		= CNetworkLibrary::MyChaInfo::eMORPH_TRANSFORMATION;
}

void	CNetworkLibrary::Set_MyChar_MorphStatus_EVOCATION_BEGIN()
{
	MyCharacterInfo.eMorphStatus		= CNetworkLibrary::MyChaInfo::eMORPH_EVOCATION_BEGIN;
}

int		CNetworkLibrary::Get_MyChar_MorphStatus()
{
	return MyCharacterInfo.eMorphStatus;
}

bool	CNetworkLibrary::Get_MyChar_Attack(int targetID,LONG	targetHP) // TO.DO HP
{
	CNetworkLibrary::sPetInfo	TempPet;
	TempPet.lIndex				= targetID;
	std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
		std::find_if(m_vectorPetList.begin(), m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
	if( iter != m_vectorPetList.end() )
	{
		(*iter).lHP = targetHP;
		return	true;
	}
	return false;
}

bool CNetworkLibrary::IsAttackMe( int targetID )
{
	if( MyCharacterInfo.index == targetID )
		return true;
	
	CNetworkLibrary::sPetInfo	TempPet;
	TempPet.lIndex				= targetID;
	std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
		std::find_if(m_vectorPetList.begin(), m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
	if( iter != m_vectorPetList.end() )
		return true;

	// 소서러 정령 처리
	ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_SLAVE, targetID);

	if (pObject != NULL)
	{
		return true;
	}

	return false;
}

void	CNetworkLibrary::Set_MyChar_Effect(int item_E, int statusEffect)
{
	MyCharacterInfo.itemEffect.Refresh(NULL, item_E);
	MyCharacterInfo.statusEffect.Refresh(NULL, (CStatusEffect::eRelation)statusEffect);
}

SBYTE	CNetworkLibrary::Get_MyChar_faceStyle()
{
	return MyCharacterInfo.faceStyle;
}

SBYTE	CNetworkLibrary::Get_MyChar_hairStyle()
{
	return MyCharacterInfo.hairStyle;
}

UBYTE	CNetworkLibrary::Get_MyChar_Job()
{
	return MyCharacterInfo.job;
}

void CNetworkLibrary::SendPetAccumulateUse(SBYTE scType, LONG lIndex)
{
	CNetworkMessage nmPetAccumulate((UBYTE)MSG_EXTEND);
	nmPetAccumulate << (ULONG)MSG_EX_ATTACK_PET;
	nmPetAccumulate << (SBYTE)MSG_SUB_EXPUSE;
	nmPetAccumulate << scType;
	nmPetAccumulate << lIndex;
	
	SendToServerNew(nmPetAccumulate);
}

void CNetworkLibrary::SendReformItemReq( UCHAR reformerGrade, SWORD reformerTab, SWORD reformerIdx, 
	SWORD magnifierTab, SWORD magnifierIdx, ULONG magnifierCount, SWORD reformItemTab, SWORD reformItemIdx, int nNPCVIdx )
{
	CNetworkMessage nmMessage;
	RequestClient::doReform* packet = reinterpret_cast<RequestClient::doReform*>(nmMessage.nm_pubMessage);
	packet->type = MSG_EXTEND;
	packet->subType = htonl(MSG_EX_REFORMER_SYSTEM);
	packet->thirdType = 0;
	packet->npcIndex = nNPCVIdx;
	packet->reformer_tab = reformerTab;
	packet->reformer_invenIndex = reformerIdx;
	packet->magnifire_tab = magnifierTab;
	packet->magnifire_invenIndex = magnifierIdx;
	packet->reform_tab = reformItemTab;
	packet->reform_invenIndex = reformItemIdx;
	packet->grade = reformerGrade;
	packet->count = magnifierCount;
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew( nmMessage );
}

void CNetworkLibrary::ResetSkillDelayTime() // 스킬 쿨타임 초기화
{
	FOREACHINSTATICARRAY(ga_World.wo_aSkillData, CSkill, iter)
	{
		iter->ResetStartTime();
		iter->SetToggle(false);		// 토글도 꺼준다.
	}
}
void CNetworkLibrary::ResetItemDelayTime() // 아이템 쿨타임 초기화
{
	CItemData::_map::iterator	iter = CItemData::_mapdata.begin();
	CItemData::_map::iterator	eiter = CItemData::_mapdata.end();

	for (; iter != eiter; ++iter)
	{	
		CItemData* pItem = (*iter).second;
	
		if (pItem == NULL)
			continue;

		pItem->StartTime = 0.0f;
	}
}

void CNetworkLibrary::SendReqServerTime()
{
	CNetworkMessage nmServerTime(MSG_EXTEND);
	nmServerTime << (ULONG)MSG_EX_SERVER_TIME;
	
	SendToServerNew(nmServerTime);
}

void CNetworkLibrary::RestartGame()
{
	MyCharacterInfo.money = 0;
	MyCharacterInfo.sbSoulCount = 0;
	MyCharacterInfo.bExtension = FALSE;
	MyCharacterInfo.nEvocationIndex = 0;
	MyCharacterInfo.statusEffect.Reset();
	MyCharacterInfo.itemEffect.Clear();
	MyCharacterInfo.iHitEffectType = 0;
	MyCharacterInfo.eMorphStatus = MyChaInfo::eMORPH_END;
	MyCharacterInfo.useTotem = false;

	ClearAffinityData();
	// 친화도 보상 NPC 리스트 클리어
	ClearAffinityRewardNPC();

	MonsterMercenaryInfo.Init();

	// leave wild pet
	LeaveWildPet( (CPlayerEntity*)CEntity::GetPlayerEntity(0) );
	ClearPetList();

	INFO()->clear();
	
	SetRvrZone(FALSE); // RVR 존 셋팅 초기화

	ResetSkillDelayTime();//스킬 딜레이 타임 초기화
	ResetItemDelayTime();// 아이템 딜레이 타임 초기

	m_vectorLegitList.clear();
}

bool CNetworkLibrary::CheckSendSkill(INDEX nIndex)
{
	__int64	_cur = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

	// 같은 스킬은 정해진 시간내에 중복 사용 금지.
	if (m_nLastSkillIndex == nIndex &&
		(_cur - m_nSkillSendTime) < DEF_SKILL_SEND_DELAY)
	{
		CUIManager* pUIMgr = UIMGR();

		pUIMgr->GetChattingUI()->AddSysMessage(_S(5550, "이미 사용중입니다."), SYSMSG_ERROR);
		pUIMgr->SetCSFlagOff(CSF_SKILLREADY);
		pUIMgr->CancelSkill(FALSE, TRUE);
		return false;
	}

	m_nSkillSendTime = _cur;
	m_nLastSkillIndex = nIndex;	

	return true;
}

void CNetworkLibrary::SendClickObject( int charIndex )
{
	if (_pGameState->GetRestartGameValue() == TRUE)
		return;

 	CNetworkMessage nmMessage;
	RequestClient::doSetTargetInfo* packet = reinterpret_cast<RequestClient::doSetTargetInfo*>(nmMessage.nm_pubMessage);
	packet->type = MSG_EXTEND;
	packet->subType = MSG_EX_SET_TARGET;
	packet->charIndex = charIndex;

	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew( nmMessage );
}

void CNetworkLibrary::SendPremiumCharItemUse( SWORD tab, SWORD invenIdx, int VirtualIdx )
{
#ifdef PREMIUM_CHAR
	CNetworkMessage nmMessage;
	RequestClient::premiumChar_UseItem* packet = reinterpret_cast<RequestClient::premiumChar_UseItem*>(nmMessage.nm_pubMessage);
	packet->type = MSG_PREMIUM_CHAR;
	packet->subType = MSG_SUB_PREMIUM_CHAR_USE_ITEM;
	packet->tab = tab;
	packet->invenIndex = invenIdx;
	packet->virtualIndex = VirtualIdx;
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew( nmMessage );
#endif	//	PREMIUM_CHAR
}

void CNetworkLibrary::SendPremiumCharJumpReq( CTString strCharName )
{
#ifdef PREMIUM_CHAR
	CNetworkMessage nmMessage;
	RequestClient::premiumChar_JumpToChar* packet = reinterpret_cast<RequestClient::premiumChar_JumpToChar*>(nmMessage.nm_pubMessage);
	packet->type = MSG_PREMIUM_CHAR;
	packet->subType = MSG_SUB_PREMIUM_CHAR_JUMP_TO_CHAR;
#if	!defined(WORLD_EDITOR)
	strcpy_s(packet->toCharName, MAX_CHAR_NAME_LENGTH, strCharName.str_String);	
#else
	strcpy(packet->toCharName, strCharName.str_String);
#endif	// WORLD_EDITOR
	nmMessage.setSize( sizeof(*packet) );

	SendToServerNew( nmMessage );
#endif	//	PREMIUM_CHAR
}
