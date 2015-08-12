// RaidData.h: interface for the CRaidData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAIDDATA_H__5D52A1E1_A4DC_4D73_A16B_0C042B625BA5__INCLUDED_)
#define AFX_RAIDDATA_H__5D52A1E1_A4DC_4D73_A16B_0C042B625BA5__INCLUDED_


class CRaidDataProto;
class CRaidDataProtoList;
class CRaidObjectDataProto;
class CRaidObjectDataProtoList;
class CAreaRaidInfo;
class CAreaRaidInfoList;
class CRaidZoneDataProto;
class CRaidZoneDataProtoList;
class CRaidObjectList;

class CRaidObjectData
{
public:
	void SetAttr();
	CRaidObjectData();
	virtual ~CRaidObjectData();
	void SetState(int state)
	{
		m_nState = state;
	}
	void SetProto(CRaidObjectDataProto* proto)
	{
		m_proto = proto;
	}

	int GetState()
	{
		return m_nState;
	}
	CRaidObjectDataProto* GetProto()
	{
		return m_proto;
	}
private:
	CRaidObjectDataProto* m_proto;
	int m_nState;						// state는 많은 정보를 담게 된다.
};

class CRaidDataProto
{
public:
	CRaidDataProto();
	virtual ~CRaidDataProto();
	int		GetIndex()
	{
		return m_nDBIndex;
	}
	int		GetRaidNo()
	{
		return m_nRaidNo;
	}
	int		GetSceneNo()
	{
		return m_nSceneNo;
	}
	int		GetPreCond(int idx)
	{
		return m_nPreCond[idx];
	}
	CRaidObjectDataProto* GetTodo()
	{
		return m_TodoProto;
	}
	CRaidObjectDataProto* GetOutput(int idx);

	void	SetIndex(int index)
	{
		m_nDBIndex = index;
	}
	void	SetRaidNo(int No)
	{
		m_nRaidNo = No;
	}
	void	SetSceneNo(int No)
	{
		m_nSceneNo = No;
	}
	void	SetPreCond(int idx, int val)
	{
		m_nPreCond[idx] = val;
	}
	void	SetName(const char* name)
	{
		strcpy(m_strName, name);
	}
	void	SetOutput(int index, int val);
	void	SetTodo(int index);
	void	SetDifficulty(int level)
	{
		m_nDifficulty = level;
	}
	int		GetDifficulty()
	{
		return m_nDifficulty;
	}
	void	SetRepeat(int i)
	{
		m_nRepeat = i;
	}
	int		GetRepeat()
	{
		return m_nRepeat;
	}
private:
	int m_nDBIndex;
	int m_nRaidNo;
	int m_nSceneNo;
	char m_strName[100];
	CRaidObjectDataProto* m_TodoProto;
	CRaidObjectDataProto* m_OutputProto[5];
	int m_nPreCond[3];
	int m_nDifficulty;
	int m_nRepeat;
};

class CRaidDataProtoList
{
public:
	typedef std::map<int, CRaidDataProto*> map_t;

	int GetZoneCount(int ZoneIndex, int nDifficulty);
	CRaidDataProto* Find(int nDBIndex);
	CRaidDataProto* FindUseArray(int nArrayIndex);

	bool Load();
	int GetCount()
	{
		return m_nCount;
	}
	CRaidDataProtoList();
	virtual ~CRaidDataProtoList();

private:
	map_t			map_;
	CRaidDataProto* m_RaidDataProto;
	int m_nCount;
};

class CRaidObjectDataProto
{
public:
	CRaidObjectDataProto();
	virtual ~CRaidObjectDataProto();
	int		GetIndex()
	{
		return m_nDBIndex;
	}
	int		GetAssignZone()
	{
		return m_nAssignZone;
	}
	int		GetObjectIndex()
	{
		return m_nObjectIndex;
	}
	int		GetObjectData()
	{
		return m_nObjectData;
	}
	int		GetTodoOutput()
	{
		return m_nTodoOutput;
	}
	int		GetObjectType()
	{
		return m_nObjectType;
	}
	int		GetFlag()
	{
		return m_flag;
	}

	void	SetIndex(int idx)
	{
		m_nDBIndex = idx;
	}
	void	SetAssignZone(int zone)
	{
		m_nAssignZone = zone;
	}
	void	SetObjectIndex(int idx)
	{
		m_nObjectIndex = idx;
	}
	void	SetObjectData(int data)
	{
		m_nObjectData = data;
	}
	void	SetObjectType(int type)
	{
		m_nObjectType = type;
	}
	void	SetName(const char* Name)
	{
		strcpy(m_strObjectName, Name);
	}
	void	SetTodoOutput(int type)
	{
		m_nTodoOutput = type;
	}
	void	SetFlag(int flag)
	{
		m_flag = flag;
	}

private:
	int m_nDBIndex;
	int m_nAssignZone;
	int m_nTodoOutput;
	int m_nObjectType;
	int m_nObjectIndex;
	char m_strObjectName[50];
	int m_nObjectData;
	int m_flag;
};

class CRaidObjectDataProtoList
{
public:
	typedef std::map<int, CRaidObjectDataProto*> map_t;

	CRaidObjectDataProto* Find(int index);
	bool Load();
	CRaidObjectDataProtoList();
	virtual ~CRaidObjectDataProtoList();

private:
	map_t		map_;
	CRaidObjectDataProto* m_RaidObjectDataProto;
	int m_nCount;
};

class CRaidZoneDataProto
{
public:
	CRaidZoneDataProto();
	virtual ~CRaidZoneDataProto();
	int		GetIndex()
	{
		return m_nDBIndex;
	}
	int		GetRaidNo()
	{
		return m_nRaidNo;
	}
	int		GetDistNo()
	{
		return m_nDistNo;
	}
	int		GetDistSubNo()
	{
		return m_nDistSubNo;
	}
	int		GetLeftPos()
	{
		return m_nLeft;
	}
	int		GetTopPos()
	{
		return m_nTop;
	}
	int		GetRightPos()
	{
		return m_nRight;
	}
	int		GetBottomPos()
	{
		return m_nBottom;
	}
	void	SetIndex(int idx)
	{
		m_nDBIndex = idx;
	}
	void	SetRaidNo(int No)
	{
		m_nRaidNo = No;
	}
	void	SetDistNo(int DistNo)
	{
		m_nDistNo = DistNo;
	}
	void	SetDistSubNo(int DistSubNo)
	{
		m_nDistSubNo = DistSubNo;
	}
	void	SetLeftPos(int left)
	{
		m_nLeft = left;
	}
	void	SetTopPos(int top)
	{
		m_nTop = top;
	}
	void	SetRightPos(int right)
	{
		m_nRight = right;
	}
	void	SetBottomPos(int bottom)
	{
		m_nBottom = bottom;
	}
	void	SetName(const char* name)
	{
		strcpy(m_strName, name);
	}

private:
	int m_nDBIndex;
	int m_nRaidNo;
	int m_nDistNo;
	int m_nDistSubNo;
	char m_strName[50];
	int m_nLeft;
	int m_nTop;
	int m_nRight;
	int m_nBottom;
};

class CRaidZoneDataProtoList
{
public:
	typedef std::map<int, CRaidZoneDataProto*> map_t;

	CRaidZoneDataProto* Find(int index);
	bool Load();
	int GetCount()
	{
		return m_nCount;
	}

	CRaidZoneDataProtoList();
	virtual ~CRaidZoneDataProtoList();

private:
	map_t				map_;
	CRaidZoneDataProto* m_RaidZoneDataProto;
	int m_nCount;
};

class CRaidInfo
{
public:
	bool Load();
	CRaidInfo();
	virtual ~CRaidInfo();

	CRaidDataProtoList			m_RaidDataProtoList;
	CRaidObjectDataProtoList	m_RaidObjectDataProtoList;
//	CRaidZoneDataProtoList		m_RaidZoneDataProtoList;
};

class CRaidData
{
public:
	CRaidData();
	virtual ~CRaidData();
	void SetProto(CRaidDataProto* proto)
	{
		m_proto = proto;
	}
	void SetTodo(CRaidObjectData* todo)
	{
		m_Todo = todo;
	}
	void SetOutput(int ArrayIdx, CRaidObjectData* output)
	{
		m_Output[ArrayIdx] = output;
	}
	void SetState(int state)
	{
		m_nState = state;
	}

	CRaidDataProto* GetProto()
	{
		return m_proto;
	}
	CRaidObjectData* GetTodo()
	{
		return m_Todo;
	}
	CRaidObjectData* GetOutput(int ArrayIndex)
	{
		return m_Output[ArrayIndex];
	}
	int GetState()
	{
		return m_nState;
	}

private:
	CRaidDataProto* m_proto;
	CRaidObjectData* m_Todo;
	CRaidObjectData* m_Output[5];
	int m_nState;							// SceneState에는 많은 현재 Scene에 대한 정보를 남겨주게 된다.
	// 0 : 현재 Todo 및 Output은 대기 상태이다.
	// 1 : 현재 Todo 및 Output은 완료 상태이다.
};

class CRaidObjectList
{
public:
	int		GetObject(int nArrayIndex);
	int		GetObjectIndex(int nArrayIndex);
	int		GetObjectState(int nArrayIndex);
	bool	Load(int ZoneIndex);
	int		GetCountRow()
	{
		return m_nCountRow;
	}
	int		GetZoneNo()
	{
		return m_nZoneNo;
	}
	void	ObjectReset();
	void	SetObjectState(int object, int index, int state);
	CRaidObjectList();
	virtual ~CRaidObjectList();
private:
	int m_nCountRow;
	int m_nZoneNo;
	int* m_nObject;
	int* m_nObjectIndex;
	int* m_nState;
};

class CAreaRaidInfoList
{
public:
	int		GetSavePoint();
	void	ExcuteOutput(int type, int index, int data, CPC* ch);
	void	SetTheObjectState(int object, int index, int state);
	void	ExcuteTodo(int object_type, int object_index, int object_data, CPC* ch);
	int		LoadSavePointRaidInfo(int ZoneIndex, int roomindex);
	void	ResetRaidInfo(int ZoneIndex);
	bool	LoadRaidInfo(int ZoneIndex, int difficulty);
	void	SetCount(int nCount)
	{
		m_nCount = nCount;
	}
	int		GetCount()
	{
		return m_nCount;
	}
	CRaidObjectList m_objectlist;

	CAreaRaidInfoList();
	virtual ~CAreaRaidInfoList();
	CArea* m_pArea;
private:

	CRaidData* m_RaidData;
	int m_nCount;							// RaidData 최소 단위의 개수.
	int m_nSavePoint;
};

#endif // !defined(AFX_RAIDDATA_H__5D52A1E1_A4DC_4D73_A16B_0C042B625BA5__INCLUDED_)
//
