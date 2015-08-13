#include "stdhdrs.h"

#include "Server.h"
#include "../ShareLib/DBCmd.h"
#include "CmdMsg.h"
#include "Log.h"

#include "TrigerSet.h"

// CTrigger 시작
CTrigger::CTrigger()
{
	m_index = 0;
	m_triggerset_type = 0;
	m_trigger_touchID = 0;
	m_trigger_playID = 0;
	m_trigger_type = 0;
	m_group_index = 0;
	m_nSaveLevel = 0;

	m_bUsed = false;
	m_nType_sub = 0;
	m_bContinue = 0;
	m_bDomino = 0;
	m_bPlayOnePC = 0;
	m_TriggerTime = 0;

	m_nItem_index_ItemComposition = 0;
	m_nMeterial_IndexCount = 0;
	memset(m_nMeterial_index, 0, sizeof(m_nMeterial_index));
	memset(m_nMeterial_count, 0, sizeof(m_nMeterial_count));

	m_ItemDrop_index = 0;
	m_ItemDrop_percent = 0;

	m_Killed_Npc_Index = 0;
	m_Killed_Npc_Count = 0;
	m_NeedItem_IndexCount = 0;
	memset(m_NeedItem_Index, 0, sizeof(m_NeedItem_Index));
	memset(m_NeedItem_Count, 0, sizeof(m_NeedItem_Count));

	m_nStringIndex = 0;

	m_nSummonNpc_indexCount = 0;
	memset(m_nSummonNpc_index, 0, sizeof(m_nSummonNpc_index));
	memset(m_nSummonNpc_count, 0, sizeof(m_nSummonNpc_count));

	m_nUseDebuf_indexCount = 0;
	memset(m_nUseDebuf_index, 0, sizeof(m_nUseDebuf_index));

	m_Item_Index_removeNPC = 0;
	m_KilledNpc_Index_ForRemoveNpc = 0;
	m_KillCount = 0;

	m_TimeLimitSec = 0;

	m_nQuestIndex = 0;

	m_nGetItem_TotalCount = 0;
	memset(m_nGetItem_Index, 0, sizeof(m_nGetItem_Index));
	memset(m_nGetItem_Count, 0, sizeof(m_nGetItem_Count));
	memset(m_nGetItem_Prob, 0, sizeof(m_nGetItem_Prob));

	m_nMainDialogueString = 0;
	m_nNextDialogueString1 = 0;
	m_nNextDialogueString2 = 0;
	m_nNextDialogueString3 = 0;
	m_nNextDialogueString4 = 0;
	m_nNextDialogueString5 = 0;
	m_nNextDialogueTriggerID1 = 0;
	m_nNextDialogueTriggerID2 = 0;
	m_nNextDialogueTriggerID3 = 0;
	m_nNextDialogueTriggerID4 = 0;
	m_nNextDialogueTriggerID5 = 0;

	m_nTargetType = 0;
	m_nEffectNPCIndex = 0;
}

CTrigger::~CTrigger()
{
	m_index = 0;
	m_triggerset_type = 0;
	m_trigger_touchID = 0;
	m_trigger_playID = 0;
	m_trigger_type = 0;
	m_group_index = 0;
	m_nSaveLevel = 0;

	m_bUsed = false;
	m_nType_sub = 0;
	m_bContinue = 0;
	m_bDomino = 0;
	m_bPlayOnePC = 0;
	m_TriggerTime = 0;

	m_nItem_index_ItemComposition = 0;
	m_nMeterial_IndexCount = 0;
	memset(m_nMeterial_index, 0, sizeof(m_nMeterial_index));
	memset(m_nMeterial_count, 0, sizeof(m_nMeterial_count));

	m_ItemDrop_index = 0;
	m_ItemDrop_percent = 0;

	m_Killed_Npc_Index = 0;
	m_Killed_Npc_Count = 0;
	m_NeedItem_IndexCount = 0;
	memset(m_NeedItem_Index, 0, sizeof(m_NeedItem_Index));
	memset(m_NeedItem_Count, 0, sizeof(m_NeedItem_Count));

	m_nStringIndex = 0;

	m_nSummonNpc_indexCount = 0;
	memset(m_nSummonNpc_index, 0, sizeof(m_nSummonNpc_index));
	memset(m_nSummonNpc_count, 0, sizeof(m_nSummonNpc_count));

	m_nUseDebuf_indexCount = 0;
	memset(m_nUseDebuf_index, 0, sizeof(m_nUseDebuf_index));

	m_Item_Index_removeNPC = 0;
	m_KilledNpc_Index_ForRemoveNpc = 0;
	m_KillCount = 0;

	m_TimeLimitSec = 0;

	m_nQuestIndex = 0;

	m_nGetItem_TotalCount = 0;
	memset(m_nGetItem_Index, 0, sizeof(m_nGetItem_Index));
	memset(m_nGetItem_Count, 0, sizeof(m_nGetItem_Count));
	memset(m_nGetItem_Prob, 0, sizeof(m_nGetItem_Prob));

	m_nMainDialogueString = 0;
	m_nNextDialogueString1 = 0;
	m_nNextDialogueString2 = 0;
	m_nNextDialogueString3 = 0;
	m_nNextDialogueString4 = 0;
	m_nNextDialogueString5 = 0;
	m_nNextDialogueTriggerID1 = 0;
	m_nNextDialogueTriggerID2 = 0;
	m_nNextDialogueTriggerID3 = 0;
	m_nNextDialogueTriggerID4 = 0;
	m_nNextDialogueTriggerID5 = 0;

	m_nTargetType = 0;
	m_nEffectNPCIndex = 0;
}

void CTrigger::CopyTriggerDate(CTrigger* trigger)
{
	m_index				= trigger->m_index;
	m_triggerset_type	= trigger->m_triggerset_type;
	m_trigger_touchID	= trigger->m_trigger_touchID;
	m_trigger_playID	= trigger->m_trigger_playID;
	m_trigger_type		= trigger->m_trigger_type;
	m_trigger_data      = trigger->m_trigger_data;
	m_nSaveLevel		= trigger->m_nSaveLevel;
	m_group_index		= trigger->m_group_index;

	m_bUsed				= trigger->m_bUsed;
	m_nType_sub			= trigger->m_nType_sub;
	m_bContinue			= trigger->m_bContinue;
	m_bDomino			= trigger->m_bDomino;
	m_bPlayOnePC		= trigger->m_bPlayOnePC;
	m_TriggerTime		= trigger->m_TriggerTime;

	m_nItem_index_ItemComposition = trigger->m_nItem_index_ItemComposition;
	m_nMeterial_IndexCount		= trigger->m_nMeterial_IndexCount;
	memcpy(m_nMeterial_index, trigger->m_nMeterial_index, sizeof(m_nMeterial_index));
	memcpy(m_nMeterial_count, trigger->m_nMeterial_count, sizeof(m_nMeterial_count));

	m_ItemDrop_index		= trigger->m_ItemDrop_index;
	m_ItemDrop_percent		= trigger->m_ItemDrop_percent;

	m_Killed_Npc_Index		= trigger->m_Killed_Npc_Index;
	m_Killed_Npc_Count		= trigger->m_Killed_Npc_Count;
	m_NeedItem_IndexCount	= trigger->m_NeedItem_IndexCount;
	memcpy(m_NeedItem_Index, trigger->m_NeedItem_Index, sizeof(m_NeedItem_Index));
	memcpy(m_NeedItem_Count, trigger->m_NeedItem_Count, sizeof(m_NeedItem_Count));

	m_nStringIndex			= trigger->m_nStringIndex;

	m_nSummonNpc_indexCount = trigger->m_nSummonNpc_indexCount;
	memcpy(m_nSummonNpc_index, trigger->m_nSummonNpc_index, sizeof(m_nSummonNpc_index));
	memcpy(m_nSummonNpc_count, trigger->m_nSummonNpc_count, sizeof(m_nSummonNpc_count));

	m_nUseDebuf_indexCount	= trigger->m_nUseDebuf_indexCount;
	memcpy(m_nUseDebuf_index, trigger->m_nUseDebuf_index, sizeof(m_nUseDebuf_index));

	m_Item_Index_removeNPC	= trigger->m_Item_Index_removeNPC;
	m_KilledNpc_Index_ForRemoveNpc = trigger->m_KilledNpc_Index_ForRemoveNpc;
	m_KillCount				= trigger->m_KillCount;

	m_TimeLimitSec			= trigger->m_TimeLimitSec;

	m_nQuestIndex			= trigger->m_nQuestIndex;

	m_nGetItem_TotalCount	= trigger->m_nGetItem_TotalCount;
	memcpy(m_nGetItem_Index, trigger->m_nGetItem_Index, sizeof(m_nGetItem_Index));
	memcpy(m_nGetItem_Count, trigger->m_nGetItem_Count, sizeof(m_nGetItem_Count));
	memcpy(m_nGetItem_Prob, trigger->m_nGetItem_Prob, sizeof(m_nGetItem_Prob));

	m_nMainDialogueString = trigger->m_nMainDialogueString;
	m_nNextDialogueString1 = trigger->m_nNextDialogueString1;
	m_nNextDialogueString2 = trigger->m_nNextDialogueString2;
	m_nNextDialogueString3 = trigger->m_nNextDialogueString3;
	m_nNextDialogueString4 = trigger->m_nNextDialogueString4;
	m_nNextDialogueString5 = trigger->m_nNextDialogueString5;
	m_nNextDialogueTriggerID1 = trigger->m_nNextDialogueTriggerID1;
	m_nNextDialogueTriggerID2 = trigger->m_nNextDialogueTriggerID2;
	m_nNextDialogueTriggerID3 = trigger->m_nNextDialogueTriggerID3;
	m_nNextDialogueTriggerID4 = trigger->m_nNextDialogueTriggerID4;
	m_nNextDialogueTriggerID5 = trigger->m_nNextDialogueTriggerID5;

	m_strEffectName = trigger->m_strEffectName;
	m_nTargetType = trigger->m_nTargetType;
	m_nEffectNPCIndex = trigger->m_nEffectNPCIndex;
}

// CTrigger_List 시작
CTrigger_List::CTrigger_List()
{
	m_nCount = 0;
	m_Trigger = NULL;

	m_pArea_UseTrigerEvent = NULL;

	m_flagTriggerEvent = 0;
	m_nNPC963_KilledCount = 0;
}

CTrigger_List::~CTrigger_List()
{
	if(m_Trigger)
		delete[] m_Trigger;
	m_Trigger = NULL;

	m_pArea_UseTrigerEvent = NULL;

	m_flagTriggerEvent = 0;
	m_nNPC963_KilledCount = 0;
}

bool CTrigger_List::Load_Trigger()
{
	// 1. 일단 전체 trigger를 받아온다.
	CDBCmd dbcmd;
	dbcmd.Init(&gserver->m_dbdata);
	dbcmd.SetQuery("SELECT a_index, a_triggerset_type, a_trigger_flag, a_trigger_touchID, a_trigger_playID, a_trigger_type, a_trigger_data, a_save_level, a_group_index FROM t_trigger_set ORDER BY a_index");
	if (!dbcmd.Open())
	{
		GAMELOG << init("Load_Trigger step 1")
				<< end;
		return false;
	}

	m_nCount = dbcmd.m_nrecords;
	m_Trigger = new CTrigger[m_nCount];
	int i;

	if(!dbcmd.MoveFirst())
	{
		GAMELOG << init("Load_Trigger : ZERO TRIGGERS")
				<< end;
		return true;
	}

	for (i=0; i < m_nCount; i++)
	{
		dbcmd.GetRec("a_index",				m_Trigger[i].m_index);
		dbcmd.GetRec("a_triggerset_type",	m_Trigger[i].m_triggerset_type);
		dbcmd.GetRec("a_trigger_touchID",	m_Trigger[i].m_trigger_touchID);
		dbcmd.GetRec("a_trigger_playID",	m_Trigger[i].m_trigger_playID);
		dbcmd.GetRec("a_trigger_type",		m_Trigger[i].m_trigger_type);
		dbcmd.GetRec("a_trigger_data",		m_Trigger[i].m_trigger_data);
		dbcmd.GetRec("a_save_level",		m_Trigger[i].m_nSaveLevel);
		dbcmd.GetRec("a_group_index",		m_Trigger[i].m_group_index);

		int triggerProtoFlag = 0;
		dbcmd.GetRec("a_trigger_flag",		triggerProtoFlag);

		m_Trigger[i].m_bContinue	= (triggerProtoFlag & TRIGGER_PROTO_FLAG_CONTINUE)		?	1 : 0;
		m_Trigger[i].m_bDomino		= (triggerProtoFlag & TRIGGER_PROTO_FLAG_DOMINO)		?	1 : 0;
		m_Trigger[i].m_bPlayOnePC	= (triggerProtoFlag & TRIGGER_PROTO_FLAG_PLAY_ONE_PC)	?	1 : 0;

		dbcmd.MoveNext();
	}

	// 2. 랜덤 그룹으로 묶여있는 trigger의 종류 가짓 수를 확인한다.
	dbcmd.SetQuery("SELECT DISTINCT a_group_index FROM t_trigger_set ");
	if (!dbcmd.Open())
	{
		GAMELOG << init("Load_Trigger step 2")
				<< end;
		return false;
	}

	// 1을 빼는 이유는 group_index == 0 이면 그룹이 아닌 단독 trigger이라서 제외시키기 때문
	int nCount_randomGroup = dbcmd.m_nrecords - 1;
	int j, number = 1;

	// 3. trigger 가짓 수를 확인 후 같은 랜덤 그룹의 trigger는 섞어준다.
	for (j=0; j < nCount_randomGroup; j++)
	{
		char buf[1000];
		strcpy(buf, "SELECT a_index, a_trigger_type, a_trigger_data FROM t_trigger_set WHERE a_group_index =");
		IntCat(buf, number);
		strcat(buf, " ORDER BY a_index");
		dbcmd.SetQuery(buf);

		if (!dbcmd.Open())
		{
			GAMELOG << init("Load_Trigger step 3_")
					<< number
					<< end;
			return false;
		}

		int nCount_triggers = dbcmd.m_nrecords;
		CTrigger* RandomGroup_List = new CTrigger[nCount_triggers];

		if(!dbcmd.MoveFirst())
		{
			GAMELOG << init("Load_Trigger : ZERO GROUP TRIGGER ")
					<< number
					<< end;
			number++;
			continue;
		}

		// 임시로 만든 RandomGroup_List에 동일한 그룹을 담는다.
		for (i=0; i < nCount_triggers; i++)
		{
			dbcmd.GetRec("a_index",				RandomGroup_List[i].m_index);
			dbcmd.GetRec("a_trigger_type",		RandomGroup_List[i].m_trigger_type);
			dbcmd.GetRec("a_trigger_data",		RandomGroup_List[i].m_trigger_data);

			dbcmd.MoveNext();
		}

		// 100번을 swap으로 서로 바꾸어준다.
		for (i=0; i < 100; i++)
		{
			int swapTriggerNum1 = GetRandom(0, nCount_triggers - 1);
			int swapTriggerNum2 = GetRandom(0, nCount_triggers - 1);

			if(swapTriggerNum1 == swapTriggerNum2)
				continue;

			// 두 trigger를 섞는 함수
			Swap_Trigger(&RandomGroup_List[swapTriggerNum1], &RandomGroup_List[swapTriggerNum2]);
		}

		int k=0;
		// 섞은 임시의 RandomGroup_List를 m_Trigger_List의 해당 trigger에 찾아서 넣어준다.
		for (i=0; i < nCount_triggers; i++)
		{
			// 전체 trigger list에서
			for (k=0; k < m_nCount; k++)
			{
				// 찾았으면 데이터를 바꾸어준다.
				if ( m_Trigger[k].m_index == RandomGroup_List[i].m_index )
				{
					m_Trigger[k].m_trigger_type		= RandomGroup_List[i].m_trigger_type;
					m_Trigger[k].m_trigger_data		= RandomGroup_List[i].m_trigger_data;
					break;
				}
			}
		}

		delete[] RandomGroup_List;
		RandomGroup_List = NULL;

		number++;		// 다음 랜덤 그룹
	} // for

	CLCString arg(255);
	char tmpBuf[MAX_STRING_LENGTH] = {0,};
	// 4. trigger의 m_trigger_data를 type에 맞추어서 쪼개어 저장한다.
	for (i=0; i < m_nCount; i++)
	{
		arg = m_Trigger[i].m_trigger_data;

		switch(m_Trigger[i].m_trigger_type)
		{
		case TRIGGER_ITEM_COMPOSITION:
			arg = AnyOneArg(arg, tmpBuf);
			m_Trigger[i].m_nItem_index_ItemComposition = atoi(tmpBuf);
			arg = AnyOneArg(arg, tmpBuf);
			m_Trigger[i].m_nMeterial_IndexCount = atoi(tmpBuf);
			for( j=0; j < m_Trigger[i].m_nMeterial_IndexCount; j++ )
			{
				arg = AnyOneArg(arg, tmpBuf);
				m_Trigger[i].m_nMeterial_index[j] = atoi(tmpBuf);
				arg = AnyOneArg(arg, tmpBuf);
				m_Trigger[i].m_nMeterial_count[j] = atoi(tmpBuf);
			}
			break;

		case TRIGGER_ITEM_DROP:
			arg = AnyOneArg(arg, tmpBuf);
			m_Trigger[i].m_ItemDrop_index = atoi(tmpBuf);
			arg = AnyOneArg(arg, tmpBuf);
			m_Trigger[i].m_ItemDrop_percent = atoi(tmpBuf);
			break;

		case TRIGGER_OPEN_THE_DOOR:
			arg = AnyOneArg(arg, tmpBuf);
			m_Trigger[i].m_nType_sub = atoi(tmpBuf);
			if ( m_Trigger[i].m_nType_sub == TRIGGER_SUBTYPE_NONE_OPEN_THE_DOOR )
			{
			}
			else if ( m_Trigger[i].m_nType_sub == TRIGGER_SUBTYPE_NPC_OPEN_THE_DOOR )
			{
				arg = AnyOneArg(arg, tmpBuf);
				m_Trigger[i].m_Killed_Npc_Index = atoi(tmpBuf);
				arg = AnyOneArg(arg, tmpBuf);
				m_Trigger[i].m_Killed_Npc_Count = atoi(tmpBuf);
			}
			else if ( m_Trigger[i].m_nType_sub == TRIGGER_SUBTYPE_ITEM_OPEN_THE_DOOR )
			{
				arg = AnyOneArg(arg, tmpBuf);
				m_Trigger[i].m_NeedItem_IndexCount = atoi(tmpBuf);
				for ( j=0; j < m_Trigger[i].m_NeedItem_IndexCount; j++ )
				{
					arg = AnyOneArg(arg, tmpBuf);
					m_Trigger[i].m_NeedItem_Index[j] = atoi(tmpBuf);
					arg = AnyOneArg(arg, tmpBuf);
					m_Trigger[i].m_NeedItem_Count[j] = atoi(tmpBuf);
				}
			}
			break;

		case TRIGGER_START_RAID:
			break;

		case TRIGGER_MESSAGE_PRINT:
			arg = AnyOneArg(arg, tmpBuf);
			m_Trigger[i].m_nStringIndex = atoi(tmpBuf);
			break;

		case TRIGGER_SUMMON_NPC:
			arg = AnyOneArg(arg, tmpBuf);
			m_Trigger[i].m_nSummonNpc_indexCount = atoi(tmpBuf);
			for( j=0; j < m_Trigger[i].m_nSummonNpc_indexCount; j++ )
			{
				arg = AnyOneArg(arg, tmpBuf);
				m_Trigger[i].m_nSummonNpc_index[j] = atoi(tmpBuf);
				arg = AnyOneArg(arg, tmpBuf);
				m_Trigger[i].m_nSummonNpc_count[j] = atoi(tmpBuf);
			}
			break;

		case TRIGGER_USE_DEBUF:
			arg = AnyOneArg(arg, tmpBuf);
			m_Trigger[i].m_nUseDebuf_indexCount = atoi(tmpBuf);
			for( j=0; j < m_Trigger[i].m_nUseDebuf_indexCount; j++ )
			{
				arg = AnyOneArg(arg, tmpBuf);
				m_Trigger[i].m_nUseDebuf_index[j] = atoi(tmpBuf);
			}
			break;

		case TRIGGER_ACTIVE_OBJECT:
			break;

		case TRIGGER_REMOVE_NPC:
			arg = AnyOneArg(arg, tmpBuf);
			m_Trigger[i].m_nType_sub = atoi(tmpBuf);
			if ( m_Trigger[i].m_nType_sub == TRIGGER_SUBTYPE_NONE_REMOVE_NPC )
			{
			}
			else if ( m_Trigger[i].m_nType_sub == TRIGGER_SUBTYPE_ITEM_REMOVE_NPC )
			{
				arg = AnyOneArg(arg, tmpBuf);
				m_Trigger[i].m_Item_Index_removeNPC = atoi(tmpBuf);
				arg = AnyOneArg(arg, tmpBuf);
				m_Trigger[i].m_KilledNpc_Index_ForRemoveNpc = atoi(tmpBuf);
				arg = AnyOneArg(arg, tmpBuf);
				m_Trigger[i].m_KillCount = atoi(tmpBuf);
			}
			break;

		case TRIGGER_TIME_LIMIT:
			arg = AnyOneArg(arg, tmpBuf);
			m_Trigger[i].m_TimeLimitSec = atol(tmpBuf);
			break;

		case TRIGGER_QUEST_COMPLETE:
			arg = AnyOneArg(arg, tmpBuf);
			m_Trigger[i].m_nQuestIndex = atoi(tmpBuf);
			break;

		case TRIGGER_ITEM_GET_INVEN:
			arg = AnyOneArg(arg, tmpBuf);
			m_Trigger[i].m_nGetItem_TotalCount = atoi(tmpBuf);
			for (j=0; j<m_Trigger[i].m_nGetItem_TotalCount; j++)
			{
				arg = AnyOneArg(arg, tmpBuf);
				m_Trigger[i].m_nGetItem_Index[j] =  atoi(tmpBuf);
			}
			for (j=0; j<m_Trigger[i].m_nGetItem_TotalCount; j++)
			{
				arg = AnyOneArg(arg, tmpBuf);
				m_Trigger[i].m_nGetItem_Count[j] =  atoi(tmpBuf);
			}
			for (j=0; j<m_Trigger[i].m_nGetItem_TotalCount; j++)
			{
				arg = AnyOneArg(arg, tmpBuf);
				m_Trigger[i].m_nGetItem_Prob[j] =  atoi(tmpBuf);
			}
			break;

		case TRIGGER_CONSECUTIVE_DIALOGUE:
			arg = AnyOneArg(arg, tmpBuf);
			m_Trigger[i].m_nMainDialogueString = atoi(tmpBuf);
			arg = AnyOneArg(arg, tmpBuf);
			m_Trigger[i].m_nNextDialogueString1 = atoi(tmpBuf);
			arg = AnyOneArg(arg, tmpBuf);
			m_Trigger[i].m_nNextDialogueString2 = atoi(tmpBuf);
			arg = AnyOneArg(arg, tmpBuf);
			m_Trigger[i].m_nNextDialogueString3 = atoi(tmpBuf);
			arg = AnyOneArg(arg, tmpBuf);
			m_Trigger[i].m_nNextDialogueString4 = atoi(tmpBuf);
			arg = AnyOneArg(arg, tmpBuf);
			m_Trigger[i].m_nNextDialogueString5 = atoi(tmpBuf);
			arg = AnyOneArg(arg, tmpBuf);
			m_Trigger[i].m_nNextDialogueTriggerID1 = atoi(tmpBuf);
			arg = AnyOneArg(arg, tmpBuf);
			m_Trigger[i].m_nNextDialogueTriggerID2 = atoi(tmpBuf);
			arg = AnyOneArg(arg, tmpBuf);
			m_Trigger[i].m_nNextDialogueTriggerID3 = atoi(tmpBuf);
			arg = AnyOneArg(arg, tmpBuf);
			m_Trigger[i].m_nNextDialogueTriggerID4 = atoi(tmpBuf);
			arg = AnyOneArg(arg, tmpBuf);
			m_Trigger[i].m_nNextDialogueTriggerID5 = atoi(tmpBuf);
			break;

		case TRIGGER_ACTIVE_EFFECT:
			arg = AnyOneArg(arg, tmpBuf);
			m_Trigger[i].m_strEffectName = tmpBuf;
			arg = AnyOneArg(arg, tmpBuf);
			m_Trigger[i].m_nTargetType = atoi(tmpBuf);
			arg = AnyOneArg(arg, tmpBuf);
			m_Trigger[i].m_nEffectNPCIndex = atoi(tmpBuf);
			break;

		default:
			GAMELOG << init("Load_Trigger Error : switch() ")
					<< m_Trigger[i].m_trigger_type << delim
					<< end;
			break;
		}	// switch
	}	// for

	return true;
}

void CTrigger_List::Swap_Trigger( CTrigger* trigger1, CTrigger* trigger2 )
{
	int temp_type;
	CLCString temp_data;

	temp_type	= trigger1->m_trigger_type;
	temp_data	= trigger1->m_trigger_data;

	trigger1->m_trigger_type	= trigger2->m_trigger_type;
	trigger1->m_trigger_data	= trigger2->m_trigger_data;

	trigger2->m_trigger_type	= temp_type;
	trigger2->m_trigger_data	= temp_data;
}

bool CTrigger_List::ResetTriggerInfo(bool bDelete)
{
	int i;

	for (i=0; i < m_nCount; i++)
	{
		m_Trigger[i].SetTriggerTime(0);		// 시간 초기화
		m_Trigger[i].m_bUsed = false;		// 활성화 초기화
	}

	m_flagTriggerEvent = 0;					// 트리거 플래그 초기화
	m_nNPC963_KilledCount = 0;				// 예배당 1차 963npc 숫자 세는 것 초기화

	// 초기화 시킨 것 저장해야 함
	if(bDelete && !SaveTriggerInfo(TRIGGER_SAVE_ALTER_OF_DARK_NONE))
	{
		GAMELOG << init("SaveTriggerInfo Error : INTO ResetTriggerInfo() ")
				<< end;
		return false;
	}

	// 모든 유저를 몬트샤인 마을로 이동시킨다.
	// 현재는 몬트샤인 마을로 이동시키는데 각 존마다 다른 곳으로 조절하고 싶으면 수정하자
	if(m_pArea_UseTrigerEvent)
		m_pArea_UseTrigerEvent->GotoAllClient_StartPoint();

	return true;
}

bool CTrigger_List::SaveTriggerInfo(int saveInfo, int roomNumber)
{
	// 트리거 DB 저장을 위해 헬퍼로 전송
	if(m_pArea_UseTrigerEvent && m_pArea_UseTrigerEvent->m_zone)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperTriggerEvnetMsg(rmsg, m_pArea_UseTrigerEvent->m_zone->m_index, roomNumber, saveInfo);
		SEND_Q(rmsg, gserver->m_helper);
	}
	else
		return false;

	return true;
}

bool CTrigger_List::LoadTriggerInfo(int triggerSetType)
{
	int i, curCount = 0;
	int nCount = gserver->m_CTriggerDBInfo.m_nCount;

	// 트리거 세트 타입이 없는 경우이므로 스킵
	if(triggerSetType == 0)
		return false;

	// 먼저 소속된 트리거의 개수를 확인한다.
	for (i=0; i < nCount; i++)
	{
		if (gserver->m_CTriggerDBInfo.m_Trigger[i].m_triggerset_type == triggerSetType)
		{
			curCount++;
		}
	}

	m_Trigger = new CTrigger[curCount];
	curCount = 0;

	// 소속된 트리거만 가져온다.
	for (i=0; i < nCount; i++)
	{
		if (gserver->m_CTriggerDBInfo.m_Trigger[i].m_triggerset_type == triggerSetType)
		{
			m_Trigger[curCount].CopyTriggerDate(&gserver->m_CTriggerDBInfo.m_Trigger[i]);
			curCount++;
		}
	}

	// 가져온 트리거 숫자를 넣어준다.
	m_nCount = curCount;

	return true;
}

void CTrigger_List::LoadTriggerSaveInfo(int roomNumber)
{
	// 가져온 트리거가 이미 활성화가 된 것을 업데이트 한다.
	// 처음 서버 부팅시에만 중간 저장한 데이터를 불러오므로 직접 DB 연결한다.
	int j, nCount;
	CDBCmd dbcmd;
	dbcmd.Init(&gserver->m_dbTrigger);
	dbcmd.SetQuery("SELECT a_zone_index, a_room_index, a_save_info FROM t_trigger_saveinfo ORDER BY a_index");
	if (!dbcmd.Open())
	{
		GAMELOG << init("LoadTriggerInfo step 1")
				<< end;
		return;
	}

	nCount = dbcmd.m_nrecords;

	if(!dbcmd.MoveFirst())
	{
		GAMELOG << init("LoadTriggerInfo : ZERO SAVES")
				<< end;
		return;
	}

	while (nCount > 0)
	{
		int zoneIndex = 0, roomIndex = 0, nSaveInfo = 0;
		dbcmd.GetRec("a_zone_index",	zoneIndex);
		dbcmd.GetRec("a_room_index",	roomIndex);
		dbcmd.GetRec("a_save_info",		nSaveInfo);

		// 해당 zone과 area에 대한 데이터를 찾으면
		if(m_pArea_UseTrigerEvent->m_zone->m_index == zoneIndex && roomNumber == roomIndex)
		{
			// 존에 따라 저장된 트리거 활성화 부분이 다르다.
			switch(zoneIndex)
			{
			case ZONE_ALTER_OF_DARK:
				// saveTriggerInfo에 따라 트리거 활성화를 시켜준다.
				for (j=0; j < m_nCount; j++)
				{
					if(m_Trigger[j].m_nSaveLevel <= nSaveInfo)
						m_Trigger[j].m_bUsed = true;
				}

				// 그리고 암흑의 제단 네임드가 죽었으면, 전에 죽었다고 체크해준다.
				switch(nSaveInfo)
				{
				case TRIGGER_SAVE_ALTER_OF_DARK_1002:
					Set_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH1002_BEFORE);
					break;

				case TRIGGER_SAVE_ALTER_OF_DARK_1003:
					Set_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH1002_BEFORE);
					Set_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH1003_BEFORE);
					break;

				case TRIGGER_SAVE_ALTER_OF_DARK_1018:
					Set_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH1002_BEFORE);
					Set_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH1003_BEFORE);
					Set_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH1018_BEFORE);
					break;
				}
				break;
			}
			break;
		}

		dbcmd.MoveNext();
		nCount--;
	}
}

bool CTrigger_List::SyncForClient_TriggerInfo(CPC* ch)
{
	if ( ch == NULL )
		return false;

	int totalCount = m_nCount;
	int i, usedCount =0;

	// 우선 MessagePrint 타입의 트리거중 활성화가 된 것을 따로 보내주면서
	// 활성화된 트리거 개수를 구한다.
	for (i=0; i < totalCount; i++)
	{
		// MESSAGE_PRINT의 데이터는 퀘스트 인덱스이다.
		if (m_Trigger[i].m_bUsed && m_Trigger[i].m_trigger_type == TRIGGER_MESSAGE_PRINT && m_Trigger[i].m_nStringIndex > 0)
		{
			// 퀘스트 인덱스중 이미 완료한 것이면 보낼 필요가 없다.
			bool bSendMsg = false;

			// 최종 보스를 잡으면 메세지를 보내지 않는다.
			if (Check_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH1018_BEFORE))
				bSendMsg = false;

			// 두 번째 네임드를 잡았으면 lv3의 퀘스트만 보내준다.
			else if (Check_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH1003_BEFORE))
			{
				if(m_Trigger[i].m_nSaveLevel == TRIGGER_SAVE_ALTER_OF_DARK_1018)
					bSendMsg = true;
			}

			// 첫 번째 네임드를 잡았으면 lv2의 퀘스트만 보내준다.
			else if (Check_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH1002_BEFORE))
			{
				if(m_Trigger[i].m_nSaveLevel == TRIGGER_SAVE_ALTER_OF_DARK_1003)
					bSendMsg = true;
			}

			// 네임드를 못잡았으면 lv1의 퀘스트만 보내준다.
			else
			{
				if(m_Trigger[i].m_nSaveLevel == TRIGGER_SAVE_ALTER_OF_DARK_1002)
					bSendMsg = true;
			}

			if (bSendMsg)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_MESSAGE_PRINT, 0, 0, 0);
				RefMsg(rmsg) << m_Trigger[i].m_nStringIndex;
				SEND_Q(rmsg, ch->m_desc);
			}
		}

		// 트리거가 발동한 것이 아니면 스킵, 오브젝트 활성화가 아니면 스킵
		if(!m_Trigger[i].m_bUsed || m_Trigger[i].m_trigger_playID <= 0)
			continue;

		// touch와 play가 다르면 메세지에 touchID도 추가해야 한다.
		if (m_Trigger[i].m_trigger_touchID != m_Trigger[i].m_trigger_playID)
		{
			usedCount++;
		}

		usedCount++;
	}

	// 활성화된 트리거가 없으면 메세지를 보내지 않는다.
	if (usedCount == 0)
		return false;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_TRIGGER_EXIST_AREA, 0, 0, 0);

		// 활성화된 개수 메세지에 넣기
		RefMsg(rmsg) << usedCount;

		// 활성화된 트리거의 ID를 모두 보낸다.
		for (i=0; i < totalCount; i++)
		{
			// 트리거가 발동한 것이 아니면 스킵, 오브젝트 활성화가 아니면 스킵
			if(!m_Trigger[i].m_bUsed || m_Trigger[i].m_trigger_playID <= 0)
				continue;

			RefMsg(rmsg) << m_Trigger[i].m_trigger_playID;

			// touch와 play가 다르면 메세지에 touchID도 추가해야 한다.
			if (m_Trigger[i].m_trigger_touchID != m_Trigger[i].m_trigger_playID)
			{
				RefMsg(rmsg) << m_Trigger[i].m_trigger_touchID;
			}
		}

		SEND_Q(rmsg, ch->m_desc);
	}

	return true;
}

void CTrigger_List::TriggerTimerCheck()
{
	int i;
	time_t curtime;
	time(&curtime);

	if(!m_pArea_UseTrigerEvent)
		return;

	for(i=0; i < m_nCount; i++)
	{
		// 1. 타이머를 사용하는 트리거이면
		if(m_Trigger[i].GetTriggerTime() > 0)
		{
			// 처음 타이머가 시작된 시간과 지금의 시간의 차이
			long delayTime = 0;
			delayTime = curtime - m_Trigger[i].GetTriggerTime();

			switch(m_Trigger[i].m_trigger_type)
			{
			case TRIGGER_TIME_LIMIT:
				// 예정된 트리거 발동시간을 넘어가면
				if (delayTime >= m_Trigger[i].m_TimeLimitSec)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_TIME_LIMIT, m_Trigger[i].m_trigger_touchID, m_Trigger[i].m_trigger_playID, m_Trigger[i].m_bContinue);
					RefMsg(rmsg) << 0;		// 0으로 보내주는 것은 제한 시간이 다 되어 실패헸다는 것을 알리는 방법임
					m_pArea_UseTrigerEvent->SendToAllClient(rmsg);

					ResetTriggerInfo(false);	// 트리거 리스트 초기화
				}

				// 시간이 아직 남았으면
				else
				{
					// 남은 시간을 보내준다.
					long remainTime = m_Trigger[i].m_TimeLimitSec - delayTime;
					CNetMsg::SP rmsg(new CNetMsg);
					TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_TIME_LIMIT, m_Trigger[i].m_trigger_touchID, m_Trigger[i].m_trigger_playID, m_Trigger[i].m_bContinue);
					RefMsg(rmsg) << remainTime;		// 남은 시간을 area의 모든 클라이언트에게 보내준다.
					m_pArea_UseTrigerEvent->SendToAllClient(rmsg);
				}
				break;

			default:
				break;
			}
		}

		// 2. 하드 코딩
		switch( m_Trigger[i].m_triggerset_type )
		{
		case TRIGGERSET_ALTER_OF_DARK:
			// 2-1. 지정한 네임드 Npc가 죽었는지 체크, 네임드가 이미 죽었으면 스킵
			// 암흑의 제단 1차 네임드
			if (Check_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH1)
					&& m_Trigger[i].m_trigger_type == TRIGGER_TIME_LIMIT)
			{
				// 네임드 죽었다는 것 체크를 해제
				Reset_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH1);

				// 타이머 시간초기화
				if (m_Trigger[i].GetTriggerTime() > 0)
					m_Trigger[i].SetTriggerTime(0);

				{
					// 지정된 문을 열어준다.
					CNetMsg::SP rmsg(new CNetMsg);
					TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_OPEN_THE_DOOR, 15219, 15219, 0);
					m_pArea_UseTrigerEvent->SendToAllClient(rmsg);
				}

				{
					// 문의 오브젝트 활성화를 없앤다.
					CNetMsg::SP rmsg(new CNetMsg);
					TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ACTIVE_OBJECT, 15219, 15703, 0);
					m_pArea_UseTrigerEvent->SendToAllClient(rmsg);
				}

				{
					// 메시지를 프린트 한다.
					CNetMsg::SP rmsg(new CNetMsg);
					TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_MESSAGE_PRINT, 15219, 0, 0);
					RefMsg(rmsg) << 325;
					m_pArea_UseTrigerEvent->SendToAllClient(rmsg);
				}

				{
					CNetMsg::SP rmsg(new CNetMsg);
					TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_TIME_LIMIT, 0, 0, 0);
					RefMsg(rmsg) << -1;		// -1 로 보내주는 것은 제한 시간을 해제
					m_pArea_UseTrigerEvent->SendToAllClient(rmsg);
				}

				// 문을 열어주고 뒤늦게 들어 온 캐릭터도 문을 지나가기 위해 관련 플래그를 체크한다.
				Set_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH1_DOOR_SAVE);
			}

			// 2-2. 암흑의 재단 3차 네임드
			else if (Check_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH2))
			{
				// 네임드 죽었다는 것 체크를 해제
				Reset_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH2);

				{
					// 지정된 문을 열어준다.
					CNetMsg::SP rmsg(new CNetMsg);
					TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_OPEN_THE_DOOR, 15224, 15224, 0);
					m_pArea_UseTrigerEvent->SendToAllClient(rmsg);
				}

				{
					// 문의 오브젝트 활성화를 없앤다.
					CNetMsg::SP rmsg(new CNetMsg);
					TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ACTIVE_OBJECT, 15219, 15710, 0);
					m_pArea_UseTrigerEvent->SendToAllClient(rmsg);
				}

				// 문을 열어주고 뒤늦게 들어 온 캐릭터도 문을 지나가기 위해 관련 플래그를 체크한다.
				Set_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH2_DOOR_SAVE);
			}

			// 2-3. 암흑의 제단 3차 네임드 몬스터 소환, 중간 저장으로 전에 안죽었는지도 체크
			if (!Check_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_SUMMON1)
					&& !Check_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH1003_BEFORE))
			{
				if (m_Trigger[i].m_trigger_touchID == 15379 && m_Trigger[i].m_bUsed == true)
					Set_TriggerFlag(TRIGGER_FLAG_ACTIVE_OBJECT1);
				else if (m_Trigger[i].m_trigger_touchID == 15380 && m_Trigger[i].m_bUsed == true)
					Set_TriggerFlag(TRIGGER_FLAG_ACTIVE_OBJECT2);
				else if (m_Trigger[i].m_trigger_touchID == 15381 && m_Trigger[i].m_bUsed == true)
					Set_TriggerFlag(TRIGGER_FLAG_ACTIVE_OBJECT3);
				else if (m_Trigger[i].m_trigger_touchID == 15382 && m_Trigger[i].m_bUsed == true)
					Set_TriggerFlag(TRIGGER_FLAG_ACTIVE_OBJECT4);
			}

			// 2-4. 암흑의 제단 3차 네임드 NPC 소환 여부
			if (!Check_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_SUMMON1)
					&& Check_TriggerFlag(TRIGGER_FLAG_ACTIVE_OBJECT1)
					&& Check_TriggerFlag(TRIGGER_FLAG_ACTIVE_OBJECT2)
					&& Check_TriggerFlag(TRIGGER_FLAG_ACTIVE_OBJECT3)
					&& Check_TriggerFlag(TRIGGER_FLAG_ACTIVE_OBJECT4))
			{
				CNPC* pNPC = gserver->m_npcProtoList.Create(1003, NULL);
				if (pNPC)
				{
					GET_X(pNPC)				= 482;
					GET_Z(pNPC)				= 697;
					GET_R(pNPC)				= 0;//GetRandom(0, (int) (PI_2 * 10000)) / 10000;
					GET_YLAYER(pNPC)		= 0;

					pNPC->m_regenX			= GET_X(pNPC);
					pNPC->m_regenZ			= GET_Z(pNPC);
					pNPC->m_regenY			= GET_YLAYER(pNPC);

					pNPC->m_recoverPulse	= gserver->m_pulse;

					pNPC->InitPointsToMax();
					pNPC->m_disableTime	= time(0) + 3;
					pNPC->m_postregendelay = gserver->m_pulse + (PULSE_REAL_SEC * 3);

					m_pArea_UseTrigerEvent->AddNPC(pNPC);

					int		nCellX;
					int		nCellZ;
					m_pArea_UseTrigerEvent->PointToCellNum(GET_X(pNPC), GET_Z(pNPC), &nCellX, &nCellZ);
					m_pArea_UseTrigerEvent->CharToCell(pNPC, GET_YLAYER(pNPC), nCellX, nCellZ);

					{
						CNetMsg::SP rmsg(new CNetMsg);
						AppearMsg(rmsg, pNPC, true);
						m_pArea_UseTrigerEvent->SendToCell(rmsg, GET_YLAYER(pNPC), nCellX, nCellZ);
					}

					pNPC->m_aipulse		= gserver->m_pulse;
					pNPC->m_delay		= PULSE_REAL_SEC * 3;

					// 소환 되었다고 플래그 체크
					Set_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_SUMMON1);

					{
						// 소환되는 제단의 트리거 활성화
						CNetMsg::SP rmsg(new CNetMsg);
						TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ACTIVE_OBJECT, 15377, 15377, 0);
						m_pArea_UseTrigerEvent->SendToAllClient(rmsg);
					}

					// 뒤늦게 들어 온 캐릭터도 소환이 이미 되었다는 정보를 주기 위해 관련 플래그를 체크한다.
					Set_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_SUMMON_1003);
				}
				else
				{
					GAMELOG << init("TRIGGER_ERROR : do_TriggerEvent_Hardcording 2")
							<< "npc index" << delim
							<< 1003
							<< end;
				}
			}

			// 2-5. 지정한 네임드 Npc가 죽었으면 하드코딩한 문을 늦게 들어온 캐릭터도 지나가기 위해 활성화 시켜둔다.
			// 2-5-1. 암흑의 제단 1차 네임드 npc 죽으면 열리는 문 활성화
			if (m_Trigger[i].m_trigger_touchID == 15219 && m_Trigger[i].m_bUsed == false && Check_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH1_DOOR_SAVE))
			{
				m_Trigger[i].m_bUsed = true;
			}
			// 2-5-2. 암흑의 제단 3차 네임드 npc 죽으면 열리는 문 활성화
			if (m_Trigger[i].m_trigger_touchID == 15224 && m_Trigger[i].m_bUsed == false && Check_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH2_DOOR_SAVE))
			{
				m_Trigger[i].m_bUsed = true;
			}
			// 2-5-3. 암흑의 제단 3차 네임드 npc 소환하는 제단 활성화
			if (m_Trigger[i].m_trigger_touchID == 15377 && m_Trigger[i].m_bUsed == false && Check_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_SUMMON_1003))
			{
				m_Trigger[i].m_bUsed = true;
			}
			break;

		case TRIGGERSET_CAPPELLA_1:
			// 4-3. 예배당 1차 npc963 죽은 count가 12를 넘으면
			if (m_nNPC963_KilledCount >= 12)
			{
				CNPC* pNPC = gserver->m_npcProtoList.Create(960, NULL);
				if (pNPC)
				{
					GET_X(pNPC)				= 996;
					GET_Z(pNPC)				= 680;
					GET_R(pNPC)				= 0;
					GET_YLAYER(pNPC)		= 0;

					pNPC->m_regenX			= GET_X(pNPC);
					pNPC->m_regenZ			= GET_Z(pNPC);
					pNPC->m_regenY			= GET_YLAYER(pNPC);

					pNPC->m_recoverPulse	= gserver->m_pulse;

					pNPC->InitPointsToMax();
					pNPC->m_disableTime	= time(0) + 3;
					pNPC->m_postregendelay = gserver->m_pulse + (PULSE_REAL_SEC * 3);

					m_pArea_UseTrigerEvent->AddNPC(pNPC);

					int		nCellX;
					int		nCellZ;
					m_pArea_UseTrigerEvent->PointToCellNum(GET_X(pNPC), GET_Z(pNPC), &nCellX, &nCellZ);
					m_pArea_UseTrigerEvent->CharToCell(pNPC, GET_YLAYER(pNPC), nCellX, nCellZ);

					{
						CNetMsg::SP rmsg(new CNetMsg);
						AppearMsg(rmsg, pNPC, true);
						m_pArea_UseTrigerEvent->SendToCell(rmsg, GET_YLAYER(pNPC), nCellX, nCellZ);
					}

					pNPC->m_aipulse		= gserver->m_pulse;
					pNPC->m_delay		= PULSE_REAL_SEC * 3;

					{
						// 소환되는 제단의 트리거 활성화
						CNetMsg::SP rmsg(new CNetMsg);
						TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_ACTIVE_OBJECT, 4114, 4114, 0);
						m_pArea_UseTrigerEvent->SendToAllClient(rmsg);
					}

					// count 센 것은 초기화 시킨다.
					m_nNPC963_KilledCount = 0;
				}
				else
				{
					GAMELOG << init("TRIGGER_ERROR : do_TriggerEvent_Hardcording 1")
							<< "npc index" << delim
							<< 1003
							<< end;
				}
			}
			break;
		} // switch
	} // for

	// 초당 되돌려야 하는 트리거는 초기화 시킨다.
	// 1. 암흑의 제단 3차 네임드 몬스터 소환못하면 오브젝트 활성화를 되돌린다.
	if (!Check_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_SUMMON1)
			&& !Check_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH1003_BEFORE))
	{
		for(i=0; i < m_nCount; i++)
		{
			// 1. 암흑의 제단 3차 네임드 몬스터 소환 초기화
			if(m_Trigger[i].m_trigger_touchID == 15379 && m_Trigger[i].m_bUsed == true)
			{
				m_Trigger[i].m_bUsed = false;
				Reset_TriggerFlag(TRIGGER_FLAG_ACTIVE_OBJECT1);
				CNetMsg::SP rmsg(new CNetMsg);
				TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_RESTORE_OBJECT, 15379, 15379, m_Trigger[i].m_bContinue);
				m_pArea_UseTrigerEvent->SendToAllClient(rmsg);
			}
			else if(m_Trigger[i].m_trigger_touchID == 15380 && m_Trigger[i].m_bUsed == true)
			{
				m_Trigger[i].m_bUsed = false;
				Reset_TriggerFlag(TRIGGER_FLAG_ACTIVE_OBJECT2);
				CNetMsg::SP rmsg(new CNetMsg);
				TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_RESTORE_OBJECT, 15380, 15380, m_Trigger[i].m_bContinue);
				m_pArea_UseTrigerEvent->SendToAllClient(rmsg);
			}
			else if(m_Trigger[i].m_trigger_touchID == 15381 && m_Trigger[i].m_bUsed == true)
			{
				m_Trigger[i].m_bUsed = false;
				Reset_TriggerFlag(TRIGGER_FLAG_ACTIVE_OBJECT3);
				CNetMsg::SP rmsg(new CNetMsg);
				TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_RESTORE_OBJECT, 15381, 15381, m_Trigger[i].m_bContinue);
				m_pArea_UseTrigerEvent->SendToAllClient(rmsg);
			}
			else if(m_Trigger[i].m_trigger_touchID == 15382 && m_Trigger[i].m_bUsed == true)
			{
				m_Trigger[i].m_bUsed = false;
				Reset_TriggerFlag(TRIGGER_FLAG_ACTIVE_OBJECT4);
				CNetMsg::SP rmsg(new CNetMsg);
				TriggerEvnetMsg(rmsg, MSG_EX_TRIGGER_EVENT_RESTORE_OBJECT, 15382, 15382, m_Trigger[i].m_bContinue);
				m_pArea_UseTrigerEvent->SendToAllClient(rmsg);
			}
		}
	}
}

void CTrigger_List::InitTriggerList()
{
	m_nCount = 0;
	m_Trigger = NULL;

	m_pArea_UseTrigerEvent = NULL;

	m_flagTriggerEvent = 0;
	m_nNPC963_KilledCount = 0;
}

// CNpc_Regen_Raid Class  시작
// 레이드에서 NPC 리젠을 구역별로 제어하기 위한 클래스
CNpc_Regen_Raid::CNpc_Regen_Raid()
{
	m_nCount_regen = 0;
}

CNpc_Regen_Raid::~CNpc_Regen_Raid()
{
	m_nCount_regen = 0;
	delete[] m_regenList_InstantDoungen;
}

