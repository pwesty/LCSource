// RaidData.cpp: implementation of the CRaidData class.
//
//////////////////////////////////////////////////////////////////////
#include "stdhdrs.h"

#include "../ShareLib/DBCmd.h"
#include "Server.h"
#include "Log.h"
#include "RaidData.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "Battle.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRaidDataProto::CRaidDataProto()
{
	m_nDBIndex = -1;
	memset(m_nPreCond, -1, sizeof(m_nPreCond));
	m_nRaidNo = -1;
	m_nSceneNo = -1;
	strcpy(m_strName, "NODATA");
	m_TodoProto = NULL;
	memset(m_OutputProto, 0, sizeof(CRaidObjectDataProto*)* 5);
	m_nDifficulty = 0;
	m_nRepeat = 0;
}

CRaidDataProto::~CRaidDataProto()
{
	int i;
	m_TodoProto = NULL;
	for(i = 0; i < 5; i++)
	{
		if(m_OutputProto[i])
		{
			m_OutputProto[i] = NULL;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// CRaidObjectData Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRaidObjectDataProto::CRaidObjectDataProto()
{
	m_nAssignZone = -1;
	m_nDBIndex = -1;
	m_nObjectData = -1;
	m_nObjectIndex = -1;
	m_nObjectType = -1;
	strcpy(m_strObjectName, "NODATA");
	m_flag = 0;
}

CRaidObjectDataProto::~CRaidObjectDataProto()
{
}

//////////////////////////////////////////////////////////////////////
// CRaidZoneData Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRaidZoneDataProto::CRaidZoneDataProto()
{
	strcpy(m_strName, "NODATA");
	m_nDBIndex = -1;
	m_nRaidNo = -1;
	m_nDistNo = -1;
	m_nDistSubNo = -1;
	m_nLeft = -1;
	m_nTop = -1;
	m_nRight = -1;
	m_nBottom = -1;
}

CRaidZoneDataProto::~CRaidZoneDataProto()
{
}

CRaidDataProtoList::CRaidDataProtoList()
{
	m_nCount = 0;
	m_RaidDataProto = NULL;
}

CRaidDataProtoList::~CRaidDataProtoList()
{
	if(m_RaidDataProto)
	{
		delete[] m_RaidDataProto;
		m_RaidDataProto = NULL;
	}
}

CRaidObjectDataProtoList::CRaidObjectDataProtoList()
{
	m_RaidObjectDataProto = NULL;
}

CRaidObjectDataProtoList::~CRaidObjectDataProtoList()
{
	if(m_RaidObjectDataProto)
	{
		delete[] m_RaidObjectDataProto;
		m_RaidObjectDataProto = NULL;
	}
}

CRaidZoneDataProtoList::CRaidZoneDataProtoList()
{
	m_RaidZoneDataProto = NULL;
}

CRaidZoneDataProtoList::~CRaidZoneDataProtoList()
{
	if(m_RaidZoneDataProto)
	{
		delete[] m_RaidZoneDataProto;
		m_RaidZoneDataProto = NULL;
	}
}

CRaidInfo::CRaidInfo()
{
}
CRaidInfo::~CRaidInfo()
{
}

CRaidData::CRaidData()
{
	m_nState = 0;
	m_proto = NULL;
	m_Todo = NULL;
	for(int i = 0; i < 5; i++)
	{
		m_Output[i] = NULL;
	}
}
CRaidData::~CRaidData()
{
}

bool CRaidObjectDataProtoList::Load()
{
	CDBCmd cmd;
	CLCString sql(1024);
	sql.Format("SELECT * FROM t_raid_object_data ORDER BY a_index ASC");
	cmd.Init(&gserver->m_dbdata);
	cmd.SetQuery(sql);
	if(!cmd.Open())
	{
		GAMELOG << init("SYS_ERR") << "CANNOT OPEN t_raid_object_data TABLE" << end;
		return false;
	}

	if(!cmd.MoveFirst())
	{
		GAMELOG << init("SYS_ERR") << "NOT FOUND DATA IN t_raid_object_data TABLE" << end;
		return false;
	}

	m_nCount = cmd.m_nrecords;
	m_RaidObjectDataProto = new CRaidObjectDataProto[m_nCount];

	int i = 0;
	int nIndex;
	int nAssignZone;
	int nTodoOutput;
	int nObjectType;
	int nObjectIndex;
	int nObjectData;
	int nFlag = 0;
	CLCString strName(50);

	do
	{
		cmd.GetRec("a_index", nIndex);
		cmd.GetRec("a_assign_zone", nAssignZone);
		cmd.GetRec("a_TodoOutput", nTodoOutput);
		cmd.GetRec("a_object_type", nObjectType);
		cmd.GetRec("a_object_index", nObjectIndex);
		cmd.GetRec("a_object_data", nObjectData);
		cmd.GetRec("a_object_name", strName);
		cmd.GetRec("a_flag", nFlag);

		m_RaidObjectDataProto[i].SetIndex(nIndex);
		m_RaidObjectDataProto[i].SetAssignZone(nAssignZone);
		m_RaidObjectDataProto[i].SetObjectType(nObjectType);
		m_RaidObjectDataProto[i].SetObjectIndex(nObjectIndex);
		m_RaidObjectDataProto[i].SetObjectData(nObjectData);
		m_RaidObjectDataProto[i].SetName(strName);
		m_RaidObjectDataProto[i].SetTodoOutput(nTodoOutput);
		m_RaidObjectDataProto[i].SetFlag(nFlag);

		map_.insert(map_t::value_type(m_RaidObjectDataProto[i].GetIndex(), &m_RaidObjectDataProto[i]));

		i++;
	}
	while(cmd.MoveNext());

	return true;
}

CRaidObjectDataProto* CRaidObjectDataProtoList::Find(int index)
{
	map_t::iterator it = map_.find(index);
	return (it != map_.end()) ? it->second : NULL;
}

bool CRaidDataProtoList::Load()
{
	CDBCmd cmd;
	CLCString sql(1024);
	sql.Format("SELECT * FROM t_raid_data ORDER BY a_index ASC");
	cmd.Init(&gserver->m_dbdata);
	cmd.SetQuery(sql);
	if(!cmd.Open())
	{
		GAMELOG << init("SYS_ERR") << "CANNOT OPEN t_raid_data TABLE" << end;
		return false;
	}

	if(!cmd.MoveFirst())
	{
		GAMELOG << init("SYS_ERR") << "NOT FOUND DATA IN t_raid_data TABLE" << end;
		return false;
	}

	m_nCount = cmd.m_nrecords;
	m_RaidDataProto = new CRaidDataProto[m_nCount];

	int i = 0;
	int loop = 0;
	int nIndex;
	int nRaidNo;
	int nSceneNo;
	int nTodoIdx;
	int nOutput[5];
	int nPreCond[3];
	int nDifficulty;
	int nRepeat = 0;
	CLCString strName(100);

	do
	{
		cmd.GetRec("a_index", nIndex);
		cmd.GetRec("a_raid_no", nRaidNo);
		cmd.GetRec("a_scene_no", nSceneNo);
		cmd.GetRec("a_name", strName);
		cmd.GetRec("a_todo_idx", nTodoIdx);
		cmd.GetRec("a_output1", nOutput[0]);
		cmd.GetRec("a_output2", nOutput[1]);
		cmd.GetRec("a_output3", nOutput[2]);
		cmd.GetRec("a_output4", nOutput[3]);
		cmd.GetRec("a_output5", nOutput[4]);
		cmd.GetRec("a_precond_idx1", nPreCond[0]);
		cmd.GetRec("a_precond_idx2", nPreCond[1]);
		cmd.GetRec("a_precond_idx3", nPreCond[2]);
		cmd.GetRec("a_difficulty", nDifficulty);
		cmd.GetRec("a_repeat", nRepeat);

		m_RaidDataProto[i].SetIndex(nIndex);
		m_RaidDataProto[i].SetRaidNo(nRaidNo);
		m_RaidDataProto[i].SetSceneNo(nSceneNo);
		m_RaidDataProto[i].SetName(strName);
		m_RaidDataProto[i].SetTodo(nTodoIdx);
		m_RaidDataProto[i].SetDifficulty(nDifficulty);
		m_RaidDataProto[i].SetRepeat(nRepeat);

		for(loop = 0; loop < 5; loop++)
		{
			m_RaidDataProto[i].SetOutput(loop, nOutput[loop]);
		}
		for(loop = 0; loop < 3; loop++)
		{
			m_RaidDataProto[i].SetPreCond(loop, nPreCond[loop]);
		}

		map_.insert(map_t::value_type(m_RaidDataProto[i].GetIndex(), &m_RaidDataProto[i]));

		i++;
	}
	while(cmd.MoveNext());

	return true;
}

CRaidDataProto* CRaidDataProtoList::Find(int nDBIndex)
{
	map_t::iterator it = map_.find(nDBIndex);
	return (it != map_.end()) ? it->second : NULL;
}

CRaidDataProto* CRaidDataProtoList::FindUseArray(int nArrayIndex)
{
	return &m_RaidDataProto[nArrayIndex];
}

bool CRaidZoneDataProtoList::Load()
{
	CDBCmd cmd;
	CLCString sql(1024);
	sql.Format("SELECT * FROM t_raid_zone_data ORDER BY a_index ASC");
	cmd.Init(&gserver->m_dbdata);
	cmd.SetQuery(sql);
	if(!cmd.Open())
	{
		GAMELOG << init("SYS_ERR") << "CANNOT OPEN t_raid_zone_data TABLE" << end;
		return false;
	}

	if(!cmd.MoveFirst())
	{
		GAMELOG << init("SYS_ERR") << "NOT FOUND DATA IN t_raid_zone_data TABLE" << end;
		return false;
	}

	m_nCount = cmd.m_nrecords;
	m_RaidZoneDataProto= new CRaidZoneDataProto[m_nCount];

	int i = 0;
	int nIndex;
	int nRaidNo;
	int nDistNo;
	int nDistSubNo;
	int nLeft;
	int nTop;
	int nRight;
	int nBottom;
	CLCString strName(100);

	do
	{
		cmd.GetRec("a_index", nIndex);
		cmd.GetRec("a_raid_no", nRaidNo);
		cmd.GetRec("a_district_no", nDistNo);
		cmd.GetRec("a_district_sub_no", nDistSubNo);
		cmd.GetRec("a_district_name", strName);
		cmd.GetRec("a_left", nLeft);
		cmd.GetRec("a_top", nTop);
		cmd.GetRec("a_right", nRight);
		cmd.GetRec("a_bottom", nBottom);

		m_RaidZoneDataProto[i].SetIndex(nIndex);
		m_RaidZoneDataProto[i].SetRaidNo(nRaidNo);
		m_RaidZoneDataProto[i].SetDistNo(nDistNo);
		m_RaidZoneDataProto[i].SetDistSubNo(nDistSubNo);
		m_RaidZoneDataProto[i].SetName(strName);
		m_RaidZoneDataProto[i].SetLeftPos(nLeft);
		m_RaidZoneDataProto[i].SetTopPos(nTop);
		m_RaidZoneDataProto[i].SetRightPos(nRight);
		m_RaidZoneDataProto[i].SetBottomPos(nBottom);

		map_.insert(map_t::value_type(m_RaidZoneDataProto[i].GetIndex(), &m_RaidZoneDataProto[i]));

		i++;
	}
	while(cmd.MoveNext());

	return true;
}

CRaidZoneDataProto* CRaidZoneDataProtoList::Find(int index)
{
	map_t::iterator it = map_.find(index);
	return (it != map_.end()) ? it->second : NULL;
}

bool CRaidInfo::Load()
{
	if(!m_RaidObjectDataProtoList.Load())	// m_RaidDataProtoList를 Load 하기전 m_RaidObjectDataProtoList먼저 Load 하여야 한다.
		return false;
	if(!m_RaidDataProtoList.Load())
		return false;
//	if(!m_RaidZoneDataProtoList.Load())
//		return false;
	return true;
}
//////////////////////////////////////////////////////////////////////
// CAreaRaidInfoList Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAreaRaidInfoList::CAreaRaidInfoList()
{
	m_RaidData = NULL;
	m_nCount = 0;
	m_nSavePoint = 0;
	m_pArea = NULL;
}

CAreaRaidInfoList::~CAreaRaidInfoList()
{
	if(!m_RaidData)
	{
		delete[] m_RaidData;
		m_RaidData = NULL;
	}
	m_pArea = NULL;
}
CRaidObjectData::CRaidObjectData()
{
//	m_nStateCount = -1;
	m_nState = -1;
	m_proto = NULL;
}
CRaidObjectData::~CRaidObjectData()
{
	m_proto = NULL;
}

void CRaidDataProto::SetTodo(int index)
{
	m_TodoProto = gserver->m_RaidInfo.m_RaidObjectDataProtoList.Find(index);
}

void CRaidDataProto::SetOutput(int index, int val)
{
	m_OutputProto[index] = gserver->m_RaidInfo.m_RaidObjectDataProtoList.Find(val);
}

CRaidObjectDataProto* CRaidDataProto::GetOutput(int idx)
{
	return m_OutputProto[idx];
}
bool CAreaRaidInfoList::LoadRaidInfo(int ZoneIndex, int difficulty)
{
	//존번호도 받아왔고.. m_RaidData를 가져온다.
	int nCount = 0;
	int i;
	CRaidDataProto* proto = NULL;
	nCount = gserver->m_RaidInfo.m_RaidDataProtoList.GetZoneCount(ZoneIndex, difficulty);
	if(nCount > 0)
	{
		m_RaidData = new CRaidData[nCount];
		SetCount(nCount);
	}

	else
		return false;
	// 로딩을 하고..
	int index = 0;
	int raidDataCount = gserver->m_RaidInfo.m_RaidDataProtoList.GetCount();
	for(i = 0; i < raidDataCount; i++)
	{
		proto = gserver->m_RaidInfo.m_RaidDataProtoList.FindUseArray(i);
		if(proto->GetRaidNo() == ZoneIndex && proto->GetDifficulty() == difficulty)
		{
			m_RaidData[index].SetProto(proto);
			CRaidObjectData* todo;
			todo = new CRaidObjectData;
			todo->SetProto(m_RaidData[index].GetProto()->GetTodo());
			todo->SetAttr();
			m_RaidData[index].SetTodo(todo);

			for(int j = 0; j < 5; j++)
			{
				CRaidObjectData* output;
				output = new CRaidObjectData;
				output->SetProto(m_RaidData[index].GetProto()->GetOutput(j));
				if(output->GetProto() != NULL)
				{
					output->SetAttr();
					m_RaidData[index].SetOutput(j, output);
				}
				else
					m_RaidData[index].SetOutput(j, NULL);
			}
			index++;
		}
	}

	if(!m_objectlist.Load(ZoneIndex))
	{
		//TODO :: 데이터가 없지만, 로컬에서 ERROR로 인식할 수 있기 때문에 임시로 true 처리함.
		//return false;
		return true;
	}

	return true;
}

int CRaidDataProtoList::GetZoneCount(int ZoneIndex, int nDifficulty)
{
	int ret = 0;	//return 값
	int i;
	for(i = 0; i < m_nCount; i++)
	{
		if(m_RaidDataProto[i].GetRaidNo() == ZoneIndex && m_RaidDataProto[i].GetDifficulty() == nDifficulty)
		{
			ret++;
		}
	}
	return ret;
}

void CRaidObjectData::SetAttr()
{
	if(this->GetProto() == NULL)
		return ;

	int nTodoOutput = this->GetProto()->GetTodoOutput();
	int nObjectType = this->GetProto()->GetObjectType();

	if(nTodoOutput == OBJECT_TYPE_TODO)
	{
		switch(nObjectType)
		{
		case START_RAID:
		case TOUCH_FIELD:
		case TAKE_ITEM:
		case ALL_IN_AREA:
		case MOVE_IN_AREA:
		case MOVE_OUT_AREA:
		case COUNT_DOWN:
		case KILL_NPC:
			SetState(0);
			break;
		case NO_IN_AREA:
			SetState(1);
		default:
			break;
		}
	}
	else if(nTodoOutput == OBJECT_TYPE_OUTPUT)
	{
		switch(nObjectType)
		{
		case TOUCH_FIELD:
		case SUMMON_EXTRA:
		case SUMMON_DISTRICT:
		case SUMMON_MBOSS:
		case SUMMON_BOSS:
		case GROUND_EFFECT_ON:
		case GROUND_EFFECT_OFF:
		case TOGGLE_GATE:
		case CREATE_ITEM_OBJECT:
		case DEBUFF_SKILL:
		case END_RAID:
		case PAUSE_AREA:
		case BUFF_SKILL:
		case TOGGLE_PORTAL:
		case TOGGLE_CAMERA:
		case COUNT_DOWN_START:
		case SAVE_POINT:
		case NPC_SAY:
		case APPLY_SKILL:
		case CURE_SKILL:
			SetState(0);
			break;
		case MODEL_HOLDER_PADOX:
			SetState(1);
		default:
			break;
		}
	}
}

void CAreaRaidInfoList::ResetRaidInfo(int ZoneIndex)
{
	int nCount = GetCount();
	for(int i = 0; i < nCount; i++)
	{
		m_RaidData[i].SetState(0);
		m_RaidData[i].GetTodo()->SetAttr();
		for(int j = 0; j < 5; j++)
		{
			if(m_RaidData[i].GetOutput(j))
			{
				m_RaidData[i].GetOutput(j)->SetAttr();
			}
		}
	}

	m_objectlist.ObjectReset();
	this->m_nSavePoint = 0;
}

int CAreaRaidInfoList::LoadSavePointRaidInfo(int ZoneIndex, int roomindex)
{
	CDBCmd cmd;
	cmd.Init(&gserver->m_dbTrigger);
	CLCString sql(1024);
	if(this->m_pArea == NULL)
	{
		return 0;
	}

	sql.Format("SELECT * FROM t_trigger_saveinfo WHERE a_zone_index = %d and a_room_index = %d and a_difficulty = %d", ZoneIndex, roomindex, this->m_pArea->m_RaidDifficulty);
	cmd.SetQuery(sql);
	if(!cmd.Open())
	{
		GAMELOG << init("NOT FOUND TABLE") << "LOST charDB Table : t_trigger_saveinfo" << end;

		return 0;
	}
	if(!cmd.MoveFirst())
	{
		GAMELOG << init("NOT FOUND RAID SAVED DATA") << "ZONE No. : " << ZoneIndex  << delim << "ROOM No. : " << roomindex << end;
		m_nSavePoint = 0;
		return 0;
	}

	int nSaveInfo;
	cmd.GetRec("a_save_info", nSaveInfo);	// Save는 DB의 index
	m_nSavePoint = nSaveInfo;

	//여기서 세팅을 해주어야 한다.*/
	int nCount = GetCount();
	for(int i = 0; i < nCount; i++)
	{
		//loop를 돌면서 현재 SAVEPOINT를 찾는다.
		if(m_RaidData[i].GetProto()->GetSceneNo() <= nSaveInfo)
		{
			m_RaidData[i].SetState(RAID_DATA_STATE_COMPLETE);
			m_RaidData[i].GetTodo()->SetState(1);
			for(int j = 0; j < 5; j++)
			{
				if(m_RaidData[i].GetOutput(j) != NULL)
				{
					m_RaidData[i].GetOutput(j)->SetState(1);

					int objecttype = m_RaidData[i].GetOutput(j)->GetProto()->GetObjectType();
					int objectindex = m_RaidData[i].GetOutput(j)->GetProto()->GetObjectIndex();

					switch(objecttype)
					{
					case TOUCH_FIELD:
						m_objectlist.SetObjectState(OBJECT_TOUCH_FIELD, objectindex, 1);
						break;
					case TOGGLE_GATE:
						m_objectlist.SetObjectState(OBJECT_GATE, objectindex, 1);
						break;
					case TOGGLE_PORTAL:
						m_objectlist.SetObjectState(OBJECT_PORTAL, objectindex, 1);
						break;
					case TAKE_ITEM:
						m_objectlist.SetObjectState(OBJECT_ITEM, objectindex, 0);
						break;
					case MODEL_HOLDER_PADOX:
						m_objectlist.SetObjectState(OBJECT_PADOX_MODEL, objectindex, 0);
						break;
					default:
						break;
					}
				}
			}
		}
	}
	return 1;
}
CRaidObjectList::CRaidObjectList()
{
	m_nCountRow = 0;
	m_nZoneNo = -1;
	m_nObject = NULL;
	m_nState = NULL;
	m_nObjectIndex = NULL;
}
CRaidObjectList::~CRaidObjectList()
{
	if(m_nObject)
	{
		delete[] m_nObject;
		m_nObject = NULL;
	}
	if(m_nState)
	{
		delete[] m_nState;
		m_nState = NULL;
	}
	if(m_nObjectIndex)
	{
		delete[] m_nObjectIndex;
		m_nObjectIndex = NULL;
	}
}

int	CRaidObjectList::GetObject(int nArrayIndex)
{
	if(nArrayIndex >= m_nCountRow)
	{
		return -1;
	}
	return m_nObject[nArrayIndex];
}

int	CRaidObjectList::GetObjectIndex(int nArrayIndex)
{
	if(nArrayIndex >= m_nCountRow)
	{
		return -1;
	}
	return m_nObjectIndex[nArrayIndex];
}

int	CRaidObjectList::GetObjectState(int nArrayIndex)
{
	if(nArrayIndex >= m_nCountRow)
	{
		return -1;
	}
	return m_nState[nArrayIndex];
}

bool CRaidObjectList::Load(int ZoneIndex)
{
	m_nZoneNo = ZoneIndex;
	CDBCmd cmd;
	cmd.Init(&gserver->m_dbdata);

	CLCString sql(1024);
	sql.Format("SELECT * FROM t_raid_object_list WHERE a_zone_num = %d", ZoneIndex);
	cmd.SetQuery(sql);

	if(!cmd.Open())
	{
		GAMELOG << init("ERROR CANNOT OPEN t_raid_object_list") << end;
		return false;
	}

	if(!cmd.MoveFirst())
	{
		GAMELOG << init("CANNOT FIND DATA t_raid_object_list") << "ZONE NUMBER : " << ZoneIndex << end;
		m_nCountRow = 0;
	}
	else
	{
		m_nCountRow = cmd.m_nrecords;
	}

	int i = 0;
	if(m_nCountRow > 0)
	{
		m_nObject		= new int[m_nCountRow];
		m_nObjectIndex	= new int[m_nCountRow];
		m_nState		= new int[m_nCountRow];

		do
		{
			cmd.GetRec("a_object", m_nObject[i]);
			cmd.GetRec("a_object_index", m_nObjectIndex[i]);
			m_nState[i] = 0;
			i++;
		}
		while(cmd.MoveNext());
	}
	return true;
}

void CRaidObjectList::ObjectReset()
{
	int i;
	for(i = 0; i < m_nCountRow; i++)
	{
		m_nState[i] = 0;
	}
}

void CRaidObjectList::SetObjectState(int object, int index, int state)
{
	int i;
	for(i = 0; i < m_nCountRow; i++)
	{
		if( (m_nObject[i] == object) && (m_nObjectIndex[i] == index) )
		{
			m_nState[i] = state;
			break;
		}
	}
}

void CAreaRaidInfoList::ExcuteTodo(int object_type, int object_index, int object_data, CPC* ch)
{
	int i, j, ai;
	bool bTrigger0 = false;
	bool bTrigger1 = false;
	bool bTrigger2 = false;
	int nPCCount = 0;
	CPC* pc = NULL;
	int id;
	for(i = 0; i < m_nCount; i++)
	{
		if( m_RaidData[i].GetTodo()->GetProto()->GetObjectType() == object_type &&
				m_RaidData[i].GetTodo()->GetProto()->GetObjectIndex() == object_index )
		{
			// 조건검사 필요 object_data와 precondition 확인할 것.
			if( m_RaidData[i].GetTodo()->GetProto()->GetObjectData() == -1 &&
					m_RaidData[i].GetProto()->GetPreCond(0) == -1 &&
					m_RaidData[i].GetProto()->GetPreCond(1) == -1 &&
					m_RaidData[i].GetProto()->GetPreCond(2) == -1 )					// 조건 없음.
			{
				if(m_pArea == NULL)
					return ;

				if(m_pArea->m_zone->m_index == ZONE_AKAN_TEMPLE && m_RaidData[i].GetTodo()->GetProto()->GetObjectType() == TOUCH_FIELD)
				{
					for(ai = 0; ai < m_nCount; ai++)
					{
						if( (m_RaidData[ai].GetProto()->GetIndex() == 6 && m_RaidData[ai].GetState() == 1 && object_index == 2673)
								|| (m_RaidData[ai].GetProto()->GetIndex() == 28 && m_RaidData[ai].GetState() == 1 && object_index == 2673)
						  )
						{
							id = ai;
							bTrigger0 = true;
						}
						if( (m_RaidData[ai].GetProto()->GetIndex() == 7 && m_RaidData[ai].GetState() == 0 && object_index == 2673)
								|| (m_RaidData[ai].GetProto()->GetIndex() == 29 && m_RaidData[ai].GetState() == 0 && object_index == 2673)
						  )
						{
							bTrigger1 = true;
							break;
						}
					}

					if(bTrigger0 && bTrigger1)
					{
						float left = 712;
						float top = 475;
						float right = 785;
						float bottom = 537;
						for(ai = 0; ai < 8; ai++)
						{
							pc = m_pArea->m_AkanPCList[ai];
							if(pc != NULL)
							{
								if(left >= GET_X(pc) && right <= GET_X(pc) && top >= GET_Z(pc) && bottom <= GET_Z(pc))
								{
									nPCCount++;
								}
							}
							pc = NULL;
						}

						if(nPCCount == 0)
						{
							m_pArea->m_nCountDown = -1;
							m_RaidData[id].SetState(0);
						}
					}
				}
////////////////////////////////////////////////////////////////////////////////////////////////
				if(m_pArea->m_zone->m_index == ZONE_AKAN_TEMPLE && m_RaidData[i].GetTodo()->GetProto()->GetObjectType() == TOUCH_FIELD)
				{
					bTrigger1 = false;
					bTrigger2 = false;
					nPCCount = 0;
					pc = NULL;
					//int id;
					for(ai = 0; ai < m_nCount; ai++)
					{
						if( (m_RaidData[ai].GetProto()->GetIndex() == 7 && m_RaidData[ai].GetState() == 1)
								|| (m_RaidData[ai].GetProto()->GetIndex() == 29 && m_RaidData[ai].GetState() == 1)
						  )
						{
							bTrigger1 = true;
						}
						if( (m_RaidData[ai].GetProto()->GetIndex() == 10 && m_RaidData[ai].GetState() == 0)
								|| (m_RaidData[ai].GetProto()->GetIndex() == 32 && m_RaidData[ai].GetState() == 0)
						  )
						{
							bTrigger2 = true;
							break;
						}
					}
					if(bTrigger1 && bTrigger2)
					{
						float left = 712;
						float top = 475;
						float right = 785;
						float bottom = 537;
						for(ai = 0; ai < 8; ai++)
						{
							pc = m_pArea->m_AkanPCList[ai];
							if(pc != NULL)
							{
								if(left <= GET_X(pc) && right >= GET_X(pc) && top <= GET_Z(pc) && bottom >= GET_Z(pc))
								{
									nPCCount++;
								}
							}
							pc = NULL;
						}

						if(nPCCount == 0)
						{
							m_pArea->m_nDelayOpenGate = gserver->getNowSecond()+5;	//5초 뒤 문열림.
						}
					}
				}
////////////////////////////////////////////////////////////////////////////////////////////////
				if( m_RaidData[i].GetState() == 0 || m_RaidData[i].GetProto()->GetRepeat() == 1	)
				{
					m_RaidData[i].SetState(1);
					m_RaidData[i].GetTodo()->SetState(1);

					for(j = 0; j < 5; j++)
					{
						if(m_RaidData[i].GetOutput(j) != NULL)
						{
							m_RaidData[i].GetOutput(j)->SetState(1);
							CNetMsg::SP rmsg(new CNetMsg);
							RaidSceneMsg(rmsg, OBJECT_TYPE_OUTPUT, m_RaidData[i].GetOutput(j)->GetProto()->GetObjectType(),
										 m_RaidData[i].GetOutput(j)->GetProto()->GetObjectIndex(),
										 m_RaidData[i].GetOutput(j)->GetProto()->GetObjectData() );
							do_ExRaidScene(ch, rmsg);
						}
					}
					if(m_RaidData[i].GetTodo()->GetProto()->GetObjectType() == NO_IN_AREA)
					{
						if(m_pArea->m_zone->m_index == ZONE_AKAN_TEMPLE)
						{
							m_RaidData[i].SetState(0);
							for(int ai = 0; ai < m_nCount; ai++)
							{
								if( m_RaidData[ai].GetProto()->GetIndex() == 6
										|| m_RaidData[ai].GetProto()->GetIndex() == 7
										|| m_RaidData[ai].GetProto()->GetIndex() == 8
										|| m_RaidData[ai].GetProto()->GetIndex() == 28
										|| m_RaidData[ai].GetProto()->GetIndex() == 29
										|| m_RaidData[ai].GetProto()->GetIndex() == 30
								  )
								{
									m_RaidData[ai].SetState(0); // 대기 상태로 바꾸기
									m_RaidData[ai].GetTodo()->SetAttr();
									for(j = 0; j < 5; j++)
									{
										if(m_RaidData[ai].GetOutput(j) != NULL)
										{
											m_RaidData[ai].GetOutput(j)->SetAttr();
										}
									}
								}
							}

							CNPC* npc;
							CNPC* npcNext = m_pArea->m_npcList;
							while ((npc = npcNext))
							{
								if(!npc)
									break;
								npcNext = npc->m_pNPCNext;
								if(!npc->m_proto)
									continue;

								if( npc->m_idNum == 1115
										|| npc->m_idNum == 1116
										|| npc->m_idNum == 1170
										|| npc->m_idNum == 1171
								  )
								{
									if(npc->m_hp > 0)
									{
										npc->SendDisappearAllInCell(true);
										DelAttackList(npc);
										m_pArea->CharFromCell(npc, true);
										m_pArea->DelNPC(npc);
									}
								}
							}
						}
					}
				}
			}
			else																	// 조건 있음.
			{
				// 조건이 있으면 조건이 완료 되었는지 검사할 것.

				int precond1 = -1;
				int precond2 = -1;
				int precond3 = -1;
				int CondCount = 0;

				if( m_RaidData[i].GetTodo()->GetProto()->GetObjectData() != -1 )
				{
					CondCount++;
				}
				if( m_RaidData[i].GetProto()->GetPreCond(0) != -1 )
				{
					CondCount++;
				}
				if( m_RaidData[i].GetProto()->GetPreCond(1) != -1 )
				{
					CondCount++;
				}
				if( m_RaidData[i].GetProto()->GetPreCond(2) != -1 )
				{
					CondCount++;
				}

				precond1 = m_RaidData[i].GetProto()->GetPreCond(0);
				precond2 = m_RaidData[i].GetProto()->GetPreCond(1);
				precond3 = m_RaidData[i].GetProto()->GetPreCond(2);
				if(precond1 != -1)
				{
					for(j = 0; j < m_nCount; j++)
					{
						if( m_RaidData[j].GetProto()->GetIndex() == precond1 &&
								m_RaidData[j].GetState() == 1)
						{
							CondCount--;
							break;
						}
					}
				}

				if(precond2 != -1)
				{
					for(j = 0; j < m_nCount; j++)
					{
						if( m_RaidData[j].GetProto()->GetIndex() == precond2 &&
								m_RaidData[j].GetState() == 1)
						{
							CondCount--;
							break;
						}
					}
				}

				if(precond3 != -1)
				{
					for(j = 0; j < m_nCount; j++)
					{
						if( m_RaidData[j].GetProto()->GetIndex() == precond2 &&
								m_RaidData[j].GetState() == 1)
						{
							CondCount--;
							break;
						}
					}
				}

				if(object_type == KILL_NPC)
				{
					m_RaidData[i].GetTodo()->SetState(m_RaidData[i].GetTodo()->GetState()+1);
				}

				if(m_RaidData[i].GetTodo()->GetProto()->GetObjectData() == m_RaidData[i].GetTodo()->GetState())
				{
					CondCount--;
				}

				if(CondCount == 0)
				{
					// 조건 만족 Output을 실행하라!!!!
					int condition = m_RaidData[i].GetState();
					if(condition == 0)
					{
						m_RaidData[i].SetState(1);
						m_RaidData[i].GetTodo()->SetState(1);
						for(j = 0; j < 5; j++)
						{
							if(m_RaidData[i].GetOutput(j) != NULL)
							{
								m_RaidData[i].GetOutput(j)->SetState(1);
								CNetMsg::SP rmsg(new CNetMsg);
								RaidSceneMsg(rmsg, OBJECT_TYPE_OUTPUT, m_RaidData[i].GetOutput(j)->GetProto()->GetObjectType(),
											 m_RaidData[i].GetOutput(j)->GetProto()->GetObjectIndex(),
											 m_RaidData[i].GetOutput(j)->GetProto()->GetObjectData() );
								do_ExRaidScene(ch, rmsg);
							}
						}
					}
					else
						return ;
				}
			}
		}
	}
}

void CAreaRaidInfoList::SetTheObjectState(int object, int index, int state)
{
	m_objectlist.SetObjectState(object, index, state);
}

void CAreaRaidInfoList::ExcuteOutput(int type, int index, int data, CPC* ch)
{
	int i, j;
	switch(type)
	{
	case APPLY_SKILL:
		{
			for(i = 0; i < m_nCount; i++)
			{
				for(j = 0; j < 5; j++)
				{
					if( m_RaidData[i].GetOutput(j) != NULL &&
							m_RaidData[i].GetOutput(j)->GetProto()->GetObjectType() == type &&
							m_RaidData[i].GetOutput(j)->GetProto()->GetObjectIndex() == index )
					{
						m_RaidData[i].GetOutput(j)->SetState(1);
						// find skill and applyskill
						CSkill * pSkill = gserver->m_skillProtoList.Create(index, data);
						if( pSkill != NULL )
						{
							bool bApply;
							ApplySkill(ch, ch, pSkill, -1, bApply);

							if( !bApply && pSkill )
							{
								delete pSkill;
								pSkill = NULL;
							}
						}
					}
				}
			}
		}
		break;
	case CURE_SKILL:
		{
			for(i = 0; i < m_nCount; i++)
			{
				for(j = 0; j < 5; j++)
				{
					if( m_RaidData[i].GetOutput(j) != NULL &&
							m_RaidData[i].GetOutput(j)->GetProto()->GetObjectType() == type &&
							m_RaidData[i].GetOutput(j)->GetProto()->GetObjectIndex() == index )
					{
						m_RaidData[i].GetOutput(j)->SetState(1);
						// find skill and cure skill
						ch->m_assist.CureBySkillIndex( index );
					}
				}
			}
		}
		break;
	case NPC_SAY:
		{
			for(i = 0; i < m_nCount; i++)
			{
				for(j = 0; j < 5; j++)
				{
					if( m_RaidData[i].GetOutput(j) != NULL &&
							m_RaidData[i].GetOutput(j)->GetProto()->GetObjectType() == type &&
							m_RaidData[i].GetOutput(j)->GetProto()->GetObjectIndex() == index )
					{
						m_RaidData[i].GetOutput(j)->SetState(1);
						CNetMsg::SP rmsg(new CNetMsg);
						SendOutputMsg(rmsg, NPC_SAY, index, data);
						m_pArea->SendToAllClient(rmsg);
					}
				}
			}
		}
		break;
	case MODEL_HOLDER_PADOX:
		{
			for(i = 0; i < m_nCount; i++)
			{
				for(j = 0; j < 5; j++)
				{
					if( m_RaidData[i].GetOutput(j) != NULL &&
							m_RaidData[i].GetOutput(j)->GetProto()->GetObjectType() == type &&
							m_RaidData[i].GetOutput(j)->GetProto()->GetObjectIndex() == index )
					{
						int object = OBJECT_PADOX_MODEL;
						m_RaidData[i].GetOutput(j)->SetState(data);
						m_objectlist.SetObjectState(object, index, data);
						CNetMsg::SP rmsg(new CNetMsg);
						SendOutputMsg(rmsg, MODEL_HOLDER_PADOX, index, data);
						m_pArea->SendToAllClient(rmsg);
					}
				}
			}
		}
		break;
	case SAVE_POINT:
		{
			for(i = 0; i < m_nCount; i++)
			{
				for(j = 0; j < 5; j++)
				{
					if( m_RaidData[i].GetOutput(j) != NULL &&
							m_RaidData[i].GetOutput(j)->GetProto()->GetObjectType()  == type &&
							m_RaidData[i].GetOutput(j)->GetProto()->GetObjectIndex() == index )
					{
						m_RaidData[i].GetOutput(j)->SetState(1);

						CLCString sql(1024);
						CDBCmd cmd;
						int nDBIndex = 0;
						sql.Format("SELECT * FROM t_trigger_saveinfo WHERE a_zone_index = %d and a_room_index = %d and a_difficulty = %d", ch->m_pZone->m_index, ch->m_nJoinInzone_RoomNo, this->m_pArea->m_RaidDifficulty);
						cmd.Init(&gserver->m_dbTrigger);
						cmd.SetQuery(sql);

						if(!cmd.Open())
						{
							GAMELOG << init("CANNOT RAID SAVE") << "ZONE No. : " << ch->m_pZone->m_index << delim
									<< "ROOM No. : " << ch->m_nJoinInzone_RoomNo << end;
							return ;
						}
						m_nSavePoint = data;
						if(!cmd.MoveFirst())
						{
							// 데이터가 없으므로 insert 한다.
							sql.Format("INSERT INTO t_trigger_saveinfo(a_zone_index, a_room_index, a_save_info, a_save_time, a_difficulty) VALUES(%d, %d, %d, now(), %d)",
									   ch->m_pZone->m_index, ch->m_nJoinInzone_RoomNo, data, this->m_pArea->m_RaidDifficulty);
							cmd.SetQuery(sql);
							if(!cmd.Update())
							{
								DBLOG << init("DB INSERT ERROR") << sql << end;
								return ;
							}
						}
						else
						{
							cmd.GetRec("a_index", nDBIndex);
							// 데이터가 있으므로 update 한다.
							sql.Format("UPDATE t_trigger_saveinfo SET a_save_info = %d, a_save_time = now(), a_difficulty = %d WHERE a_index = %d", data, this->m_pArea->m_RaidDifficulty, nDBIndex );
							cmd.SetQuery(sql);
							if(!cmd.Update())
							{
								DBLOG << init("DB INSERT ERROR") << sql << end;
								return ;
							}
						}
					}
				}
			}
		}
		break;
	case COUNT_DOWN_START:
		{
			m_pArea->m_nCountDown = data;
			m_pArea->m_nObjectIndex = index;
			for(i = 0; i < m_nCount; i++)
			{
				for(j = 0; j < 5; j++)
				{
					if( m_RaidData[i].GetOutput(j) != NULL &&
							m_RaidData[i].GetOutput(j)->GetProto()->GetObjectType()  == type &&
							m_RaidData[i].GetOutput(j)->GetProto()->GetObjectIndex() == index )
					{
						m_RaidData[i].GetOutput(j)->SetState(1);
					}
				}
			}
		}
		break;
	case BUFF_SKILL:
		{
			CNPC* npc;
			CNPC* npcNext = m_pArea->m_npcList;
			bool bFind = false;
			while ((npc = npcNext))
			{
				npcNext = npc->m_pNPCNext;
				if(npc->m_idNum == data)
				{
					bFind = true;
					break;
				}
			}
			if(bFind == true)
			{
				CSkill* skill = new CSkill(gserver->m_skillProtoList.Find(index), 1);

				bool bApply;
				ApplySkill(npc, npc, skill, -1, bApply);
				delete skill;
			}

			for(i = 0; i < m_nCount; i++)
			{
				for(j = 0; j < 5; j++)
				{
					if( m_RaidData[i].GetOutput(j) != NULL &&
							m_RaidData[i].GetOutput(j)->GetProto()->GetObjectType()  == type &&
							m_RaidData[i].GetOutput(j)->GetProto()->GetObjectIndex() == index )
					{
						m_RaidData[i].GetOutput(j)->SetState(1);
					}
				}
			}
		}
		break;
	case PAUSE_AREA:
		{
			for(i = 0; i < m_nCount; i++)
			{
				for(j = 0; j < 5; j++)
				{
					if(m_RaidData[i].GetOutput(j) != NULL)
					{
						if( m_RaidData[i].GetOutput(j) != NULL &&
								m_RaidData[i].GetOutput(j)->GetProto()->GetObjectIndex() == index &&
								m_RaidData[i].GetOutput(j)->GetProto()->GetObjectType() == type )
						{
							m_RaidData[i].GetOutput(j)->SetState(1);
							CNetMsg::SP rmsg(new CNetMsg);
							SendOutputMsg(rmsg, type, index, 1);
							m_pArea->SendToAllClient(rmsg);
							m_pArea->m_nPauseIndex = index;
							m_pArea->m_nPauseMobActivity = gserver->getNowSecond() + data - 1;
						}
					}
				}
			}
		}
		break;
	case DEBUFF_SKILL:
		{
			CNPC* npc;
			CNPC* npcNext = m_pArea->m_npcList;
			bool bFind = false;
			while ((npc = npcNext))
			{
				npcNext = npc->m_pNPCNext;
				if(npc->m_idNum == data)
				{
					bFind = true;
					break;
				}
			}
			if(bFind == true)
			{
				npc->m_assist.CureBySkillIndex(index);
			}

			for(i = 0; i < m_nCount; i++)
			{
				for(j = 0; j < 5; j++)
				{
					if( m_RaidData[i].GetOutput(j) != NULL &&
							m_RaidData[i].GetOutput(j)->GetProto()->GetObjectType()  == type &&
							m_RaidData[i].GetOutput(j)->GetProto()->GetObjectIndex() == index )
					{
						m_RaidData[i].GetOutput(j)->SetState(1);
					}
				}
			}
		}
		break;
	case CREATE_ITEM_OBJECT:
		{
			for(i = 0; i < m_nCount; i++)
			{
				for(j = 0; j < 5; j++)
				{
					if(m_RaidData[i].GetOutput(j) != NULL)
					{
						if( m_RaidData[i].GetOutput(j)->GetProto()->GetObjectType() == type &&
								m_RaidData[i].GetOutput(j)->GetProto()->GetObjectIndex() == index )
						{
							m_RaidData[i].GetOutput(j)->SetState(data);
							CNetMsg::SP rmsg(new CNetMsg);
							type = OBJECT_ITEM;
							SendOutputMsg(rmsg, type, index, data);
							m_pArea->SendToAllClient(rmsg);
						}
					}
				}
			}
		}
		break;
	case TOGGLE_PORTAL:
	case TOGGLE_GATE:
	case TOUCH_FIELD:
		{
			for(i = 0; i < m_nCount; i++)
			{
				for(j = 0; j < 5; j++)
				{
					if(m_RaidData[i].GetOutput(j) != NULL)
					{
						if( m_RaidData[i].GetOutput(j)->GetProto()->GetObjectType() == type &&
								m_RaidData[i].GetOutput(j)->GetProto()->GetObjectIndex() == index )
						{
							m_RaidData[i].GetOutput(j)->SetState(data);
							if(type != TOUCH_FIELD)
							{
								if(m_pArea->m_zone->m_index == ZONE_AKAN_TEMPLE && type == TOGGLE_GATE && index == 2667)
								{
									if (data == 0)		//닫힘.(255)
									{
										m_pArea->m_zone->m_attrMap[0].m_attr[708][501] |= MATT_UNWALKABLE;
										m_pArea->m_zone->m_attrMap[0].m_attr[708][502] |= MATT_UNWALKABLE;
										m_pArea->m_zone->m_attrMap[0].m_attr[708][503] |= MATT_UNWALKABLE;
										m_pArea->m_zone->m_attrMap[0].m_attr[708][504] |= MATT_UNWALKABLE;
										m_pArea->m_zone->m_attrMap[0].m_attr[708][505] |= MATT_UNWALKABLE;
										m_pArea->m_zone->m_attrMap[0].m_attr[709][501] |= MATT_UNWALKABLE;
										m_pArea->m_zone->m_attrMap[0].m_attr[709][502] |= MATT_UNWALKABLE;
										m_pArea->m_zone->m_attrMap[0].m_attr[709][503] |= MATT_UNWALKABLE;
										m_pArea->m_zone->m_attrMap[0].m_attr[709][504] |= MATT_UNWALKABLE;
										m_pArea->m_zone->m_attrMap[0].m_attr[709][505] |= MATT_UNWALKABLE;
									}
									else if (data == 1)	//열림 (0)
									{
										m_pArea->m_zone->m_attrMap[0].m_attr[708][501] |= MATT_WALKABLE;
										m_pArea->m_zone->m_attrMap[0].m_attr[708][502] |= MATT_WALKABLE;
										m_pArea->m_zone->m_attrMap[0].m_attr[708][503] |= MATT_WALKABLE;
										m_pArea->m_zone->m_attrMap[0].m_attr[708][504] |= MATT_WALKABLE;
										m_pArea->m_zone->m_attrMap[0].m_attr[708][505] |= MATT_WALKABLE;
										m_pArea->m_zone->m_attrMap[0].m_attr[709][501] |= MATT_WALKABLE;
										m_pArea->m_zone->m_attrMap[0].m_attr[709][502] |= MATT_WALKABLE;
										m_pArea->m_zone->m_attrMap[0].m_attr[709][503] |= MATT_WALKABLE;
										m_pArea->m_zone->m_attrMap[0].m_attr[709][504] |= MATT_WALKABLE;
										m_pArea->m_zone->m_attrMap[0].m_attr[709][505] |= MATT_WALKABLE;
									}
								}

								{
									CNetMsg::SP rmsg(new CNetMsg);
									SendOutputMsg(rmsg, type, index, data);
									m_pArea->SendToAllClient(rmsg);
								}
							}
							int object;
							if(type == TOUCH_FIELD)
								object = OBJECT_TOUCH_FIELD;
							else if(type == TOGGLE_GATE)
								object = OBJECT_GATE;
							else if(type == TOGGLE_PORTAL)
								object = OBJECT_PORTAL;
							else object = -1;

							if (object != -1)
								m_objectlist.SetObjectState(object, index, data);
						}
					}
				}
			}
		}
		break;
	case SUMMON_EXTRA:
		{
			CNPC* npc;
			int RegenCount = 0;
			int LevelCount = 0;
			int i, j;

			if(m_pArea->m_zone->m_index == ZONE_AKAN_TEMPLE)
			{
				LevelCount = 10;
			}

			for(i = 0; i < LevelCount; i++)
			{
				RegenCount = m_pArea->m_zone->m_CNpcRegenRaidList[i].m_nCount_regen;

				for(j = 0; j < RegenCount; j++)
				{
					if(m_pArea->m_zone->m_CNpcRegenRaidList[i].m_regenList_InstantDoungen[j].m_npcIdx == index)
					{
						//찾았으니깐 리스트 좌표대로 소환할 것.
						npc = gserver->m_npcProtoList.Create(m_pArea->m_zone->m_CNpcRegenRaidList[i].m_regenList_InstantDoungen[j].m_npcIdx, NULL);

						if(!npc)
							continue;

						GET_X(npc)		= m_pArea->m_zone->m_CNpcRegenRaidList[i].m_regenList_InstantDoungen[j].m_pos_x;
						GET_Z(npc)		= m_pArea->m_zone->m_CNpcRegenRaidList[i].m_regenList_InstantDoungen[j].m_pos_z;
						GET_YLAYER(npc)	= m_pArea->m_zone->m_CNpcRegenRaidList[i].m_regenList_InstantDoungen[j].m_y_layer;
						GET_R(npc)		= m_pArea->m_zone->m_CNpcRegenRaidList[i].m_regenList_InstantDoungen[j].m_pos_r;

						npc->m_regenX = GET_X(npc);
						npc->m_regenZ = GET_Z(npc);
						npc->m_regenY = GET_YLAYER(npc);

						if(!npc->m_proto->CheckFlag(NPC_PEACEFUL))
						{
							if( m_pArea->GetAttr(npc->m_regenY, npc->m_regenX, npc->m_regenZ) & MATT_UNWALKABLE ||
									m_pArea->GetAttr(npc->m_regenY, npc->m_regenX, npc->m_regenZ) & MATT_PEACE )
							{
								GAMELOG << init("AKAN TEMPLE REGEN error")
										<< "NPC" << delim
										<< npc->m_name << delim
										<< npc->m_proto->m_index << delim
										<< "ZONE" << delim
										<< ch->m_pZone->m_index << delim
										<< "YLAYER" << delim
										<< npc->m_regenY << delim
										<< "X" << delim
										<< npc->m_regenX << delim
										<< "Z" << delim
										<< npc->m_regenZ
										<< end;
								delete npc;
								continue;
							}
						}

						int cx, cz;
						m_pArea->AddNPC(npc);
						m_pArea->PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
						m_pArea->CharToCell(npc, GET_YLAYER(npc), cx, cz);

						{
							CNetMsg::SP rmsg(new CNetMsg);
							AppearMsg(rmsg, npc, true);
							m_pArea->SendToCell(rmsg, GET_YLAYER(npc), cx, cz);
						}
					}
				}
			}
			for(i = 0; i < m_nCount; i++)
			{
				for(j = 0; j < 5; j++)
				{
					if( m_RaidData[i].GetOutput(j) != NULL &&
							m_RaidData[i].GetOutput(j)->GetProto()->GetObjectType()  == type &&
							m_RaidData[i].GetOutput(j)->GetProto()->GetObjectIndex() == index )
					{
						m_RaidData[i].GetOutput(j)->SetState(1);
					}
				}
			}
		}
		break;
	case SUMMON_DISTRICT:
		{
			bool bAttack = false;
			for(i = 0; i < m_nCount; i++)
			{
				for(j = 0; j < 5; j++)
				{
					if( m_RaidData[i].GetOutput(j) != NULL &&
							m_RaidData[i].GetOutput(j)->GetProto()->GetObjectType()  == type &&
							m_RaidData[i].GetOutput(j)->GetProto()->GetObjectIndex() == index )
					{
						m_RaidData[i].GetOutput(j)->SetState(1);
						int flag = m_RaidData[i].GetOutput(j)->GetProto()->GetFlag();
						if(flag & OBJECT_DATA_FLAG_AUTO_ATTACK)
							bAttack = true;
					}
				}
			}

			CNPC* npc;
			int i, RegenCount;

			RegenCount = m_pArea->m_zone->m_CNpcRegenRaidList[data].m_nCount_regen;

			for(i = 0; i < RegenCount; i++)
			{
				npc = gserver->m_npcProtoList.Create(m_pArea->m_zone->m_CNpcRegenRaidList[data].m_regenList_InstantDoungen[i].m_npcIdx, NULL);

				if(!npc)
					continue;

				GET_X(npc)		= m_pArea->m_zone->m_CNpcRegenRaidList[data].m_regenList_InstantDoungen[i].m_pos_x;
				GET_Z(npc)		= m_pArea->m_zone->m_CNpcRegenRaidList[data].m_regenList_InstantDoungen[i].m_pos_z;
				GET_YLAYER(npc)	= m_pArea->m_zone->m_CNpcRegenRaidList[data].m_regenList_InstantDoungen[i].m_y_layer;
				GET_R(npc)		= m_pArea->m_zone->m_CNpcRegenRaidList[data].m_regenList_InstantDoungen[i].m_pos_r;

				npc->m_regenX = GET_X(npc);
				npc->m_regenZ = GET_Z(npc);
				npc->m_regenY = GET_YLAYER(npc);

				if(!npc->m_proto->CheckFlag(NPC_PEACEFUL))
				{
					if( m_pArea->GetAttr(npc->m_regenY, npc->m_regenX, npc->m_regenZ) & MATT_UNWALKABLE ||
							m_pArea->GetAttr(npc->m_regenY, npc->m_regenX, npc->m_regenZ) & MATT_PEACE )
					{
#ifdef WIN32
						GAMELOG << init("RAID DATA REGEN error")
								<< "NPC" << delim
								<< npc->m_name << delim
								<< npc->m_proto->m_index << delim
								<< "ZONE" << delim
								<< ch->m_pZone->m_index << delim
								<< "YLAYER" << delim
								<< npc->m_regenY << delim
								<< "X" << delim
								<< npc->m_regenX << delim
								<< "Z" << delim
								<< npc->m_regenZ
								<< end;
#endif
						delete npc;
						continue;
					}
				}

				int cx, cz;
				m_pArea->AddNPC(npc);
				m_pArea->PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
				m_pArea->CharToCell(npc, GET_YLAYER(npc), cx, cz);

				{
					CNetMsg::SP rmsg(new CNetMsg);
					AppearMsg(rmsg, npc, true);
					m_pArea->SendToCell(rmsg, GET_YLAYER(npc), cx, cz);
				}

				if(bAttack)
					AddAttackList(ch, npc, HATE_FIRST_ATTACK);
			}
		}
		break;
	case SUMMON_MBOSS:
	case SUMMON_BOSS:
		{
			CNPC* npc;
			int i, j;
			int LevelCount = 0;
			int RegenCount = 0;
			if(m_pArea->m_zone->m_index == ZONE_AKAN_TEMPLE)
			{
				LevelCount = 10;
			}
			for(i = 0; i < LevelCount; i++)
			{
				RegenCount = m_pArea->m_zone->m_CNpcRegenRaidList[i].m_nCount_regen;
				for(j = 0; j < RegenCount; j++)
				{
					if(m_pArea->m_zone->m_CNpcRegenRaidList[i].m_regenList_InstantDoungen[j].m_npcIdx == index)
					{
						npc = gserver->m_npcProtoList.Create(m_pArea->m_zone->m_CNpcRegenRaidList[i].m_regenList_InstantDoungen[j].m_npcIdx, NULL);

						if(!npc)
							continue;

						GET_X(npc)		= m_pArea->m_zone->m_CNpcRegenRaidList[i].m_regenList_InstantDoungen[j].m_pos_x;
						GET_Z(npc)		= m_pArea->m_zone->m_CNpcRegenRaidList[i].m_regenList_InstantDoungen[j].m_pos_z;
						GET_YLAYER(npc)	= m_pArea->m_zone->m_CNpcRegenRaidList[i].m_regenList_InstantDoungen[j].m_y_layer;
						GET_R(npc)		= m_pArea->m_zone->m_CNpcRegenRaidList[i].m_regenList_InstantDoungen[j].m_pos_r;

						npc->m_regenX = GET_X(npc);
						npc->m_regenZ = GET_Z(npc);
						npc->m_regenY = GET_YLAYER(npc);

						if(!npc->m_proto->CheckFlag(NPC_PEACEFUL))
						{
							if( m_pArea->GetAttr(npc->m_regenY, npc->m_regenX, npc->m_regenZ) & MATT_UNWALKABLE ||
									m_pArea->GetAttr(npc->m_regenY, npc->m_regenX, npc->m_regenZ) & MATT_PEACE )
							{
								GAMELOG << init("AKAN TEMPLE REGEN error")
										<< "NPC" << delim
										<< npc->m_name << delim
										<< npc->m_proto->m_index << delim
										<< "ZONE" << delim
										<< ch->m_pZone->m_index << delim
										<< "YLAYER" << delim
										<< npc->m_regenY << delim
										<< "X" << delim
										<< npc->m_regenX << delim
										<< "Z" << delim
										<< npc->m_regenZ
										<< end;
								delete npc;
								continue;
							}
						}

						int cx, cz;
						m_pArea->AddNPC(npc);
						m_pArea->PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
						m_pArea->CharToCell(npc, GET_YLAYER(npc), cx, cz);
						if( npc->m_idNum == 1116
								|| npc->m_idNum == 1128
								|| npc->m_idNum == 1171
								|| npc->m_idNum == 1182
						  )
						{
							CNetMsg::SP rmsg(new CNetMsg);
							AppearMsg(rmsg, npc, true);
							m_pArea->SendToCell(rmsg, GET_YLAYER(npc), cx, cz);
						}
						else
						{
							m_pArea->m_pBossAppearNPC = npc;
						}
					}
				}
			}
			for(i = 0; i < m_nCount; i++)
			{
				for(j = 0; j < 5; j++)
				{
					if( m_RaidData[i].GetOutput(j) != NULL &&
							m_RaidData[i].GetOutput(j)->GetProto()->GetObjectType()  == type &&
							m_RaidData[i].GetOutput(j)->GetProto()->GetObjectIndex() == index )
					{
						m_RaidData[i].GetOutput(j)->SetState(1);
					}
				}
			}
		}
		break;
	case GROUND_EFFECT_ON:
		{
			m_pArea->m_GroundEffect2.Start();
			for(i = 0; i < m_nCount; i++)
			{
				for(j = 0; j < 5; j++)
				{
					if( m_RaidData[i].GetOutput(j) != NULL &&
							m_RaidData[i].GetOutput(j)->GetProto()->GetObjectType()  == type &&
							m_RaidData[i].GetOutput(j)->GetProto()->GetObjectIndex() == index )
					{
						m_RaidData[i].GetOutput(j)->SetState(1);
					}
				}
			}
		}
		break;
	case GROUND_EFFECT_OFF:
		{
			m_pArea->m_GroundEffect2.Stop();
			for(i = 0; i < m_nCount; i++)
			{
				for(j = 0; j < 5; j++)
				{
					if( m_RaidData[i].GetOutput(j) != NULL &&
							m_RaidData[i].GetOutput(j)->GetProto()->GetObjectType()  == type &&
							m_RaidData[i].GetOutput(j)->GetProto()->GetObjectIndex() == index )
					{
						m_RaidData[i].GetOutput(j)->SetState(1);
					}
				}
			}
		}
		break;
	default:
		break;
	}
}

int CAreaRaidInfoList::GetSavePoint()
{
	return m_nSavePoint;
}

