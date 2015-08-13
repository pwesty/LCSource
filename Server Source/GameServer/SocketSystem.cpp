#include "stdhdrs.h"

#include "../ShareLib/DBCmd.h"
#include "Server.h"
#include "SocketSystem.h"

#include <json/json.h>
socket_config *socket_config::Instance = NULL;
std::map<int, st_prob_table*> socket_config::prob_map;
std::vector<int> socket_config::seperate_scroll;
int socket_config::chaos_jewel_composite_need_nas;
int socket_config::normal_jewel_composite_need_nas;
int socket_config::jewel_clean_factor;
int socket_config::chaos_jewel_clean_factor;

CSocketPos::CSocketPos()
	: m_itemidx(-1),
	  m_itemproto(NULL),
	  m_option(NULL),
	  m_skill(NULL)
{
}

CSocketPos::~CSocketPos()
{
	Clear();
}

void CSocketPos::Clear()
{
	if(m_itemidx != -1)
		m_itemidx	= 0;
	else
		m_itemidx = -1;

	m_itemproto = NULL;
	if(m_option)
	{
		delete m_option;
		m_option = NULL;
	}
	if(m_skill)
	{
		delete m_skill;
		m_skill = NULL;
	}
}

bool CSocketPos::SetJewel(int itemidx, int pos)
{
	CItemProto* itemproto = gserver->m_itemProtoList.FindIndex(itemidx);

	if(!itemproto)
	{
		m_itemidx = 0;
		return false;
	}

	if(itemproto->getItemTypeIdx() != ITYPE_ETC)
		return false;

	if(pos == 0 && itemproto->getItemSubTypeIdx() != IETC_CHAOSJEWEL)
		return false;

	if(pos > 0 && itemproto->getItemSubTypeIdx() != IETC_JEWEL)
		return false;
	if(itemproto->getItemRareIndex(0) == -1 /*|| itemproto->getItemRareIndex(0) == 0*/)
		return false;

//	if(itemproto->getItemSet1() == 0 && pos > 0)		//카오스 보석이 들어가면 테스트
	if(itemproto->getItemSet1() == 0)
	{
		COptionProto* m_optionProto = gserver->m_optionProtoList.FindProto(itemproto->getItemRareIndex(0));
		if(!m_optionProto)
			return false;

		if(m_option)
		{
			delete m_option;
			m_option = NULL;
		}
		m_option = new COption;
		m_option->m_proto = m_optionProto;
		m_option->m_type = m_optionProto->m_type;
		m_option->m_level = itemproto->getItemRareProb(0);
		m_option->m_value = m_optionProto->m_levelValue[m_option->m_level - 1];
		m_option->SetDBValue();
		m_itemproto = itemproto;
		SetItemIdx(itemidx);
	}
//	else if((itemproto->getItemSet1() == 1 || itemproto->getItemSet0() == 2) && pos == 0)	//카오스 보석이 들어가면 테스트
	else if((itemproto->getItemSet1() == 1 || itemproto->getItemSet1() == 2))
	{
		if(m_skill)
		{
			delete m_skill;
			m_skill = NULL;
		}
		m_skill = new CSkill(gserver->m_skillProtoList.Find(itemproto->getItemRareIndex(0)), 1);
		m_skillProb = itemproto->getItemRareProb(0);
		m_itemproto = itemproto;
		SetItemIdx(itemidx);
	}

	return true;
}

int CSocketPos::GetJewelDBIdx() const
{
	if (this == NULL)
	{
		return -1;
	}

	return m_itemidx;
}

void CSocketPos::ApplyJewelOption(CPC* pc, CItem* pItem, bool isOption) const
{
	if(!pc || !pItem || !m_itemproto)
		return ;

	if(m_itemidx == -1 || m_itemidx == 0 || !m_itemproto)
		return ;

	if(isOption)
		m_option->ApplyOptionValue(pc, pItem);
	else
	{
		CSkill* skill = gserver->m_skillProtoList.Create(m_itemproto->getItemRareIndex(0));

		if(m_itemproto->getItemSet1() == 1) 						//공격 발동형 스킬인 경우에
		{
			pc->m_optionAttSkillList.AddOptionSkill(skill, m_skillProb, pItem->m_itemProto->getItemWearing());
		}

		if(m_itemproto->getItemSet1() == 2) 						//방어 발동형 스킬인 경우에
		{
			pc->m_optionDefSkillList.AddOptionSkill(skill, m_skillProb, pItem->m_itemProto->getItemWearing());
		}
	}
}

COption* CSocketPos::GetOption() const
{
	if(!m_itemproto || m_itemidx == -1 || m_itemproto->getItemIndex() != m_itemidx || !m_option)
		return NULL;

	return m_option;
}

CSkill* CSocketPos::GetSkill() const
{
	if(m_itemidx == -1 || m_itemidx == 0 || !m_skill )
		return NULL;
	return m_skill;
}

void CSocketPos::GetSocketInfo(int& itemidx, int& optionidx, int& optionlevel) const
{
	if(m_itemidx == -1 || m_itemproto == NULL || m_option == NULL)
	{
		itemidx = -1;
		optionidx = -1;
		optionlevel = -1;
		return ;
	}

	itemidx = m_itemidx;
	optionidx = m_option->m_type;
	optionlevel = m_option->m_level;
}

CSocketList::CSocketList() :
	m_socketpos(NULL),
	m_createdSocketCount(0)
{
}

CSocketList::~CSocketList()
{
	if(m_socketpos)
	{
		delete [] m_socketpos;
		m_socketpos = NULL;
	}
}

char CSocketList::GetSocketCount() const
{
	if(m_createdSocketCount < 0 || m_createdSocketCount > m_maxSocketCount)
		return 0;

	return m_createdSocketCount;
}

char CSocketList::GetCombinedCount() const
{
	char i, count = 0;
	if (m_socketpos == NULL)
		return count;

	for(i = 0; i < MAX_SOCKET_COUNT; i++)
	{
		if( m_socketpos[(int)i].GetJewelDBIdx() > 0)
			count++;
	}

	return count;
}

void CSocketList::ClearAll()
{
	if (m_socketpos == NULL)
		return ;

	for(int i = 0; i < MAX_SOCKET_COUNT; ++i)
		m_socketpos[i].Clear();
}

bool CSocketList::SetJewelAt(int pos, int itemdbindex)
{
	if(m_createdSocketCount <= 0 || m_createdSocketCount > m_maxSocketCount)
		return false;

	if(pos < 0 || pos > m_createdSocketCount)
		return false;

	if (m_socketpos == NULL)
		return false;
	if(!m_socketpos[pos].SetJewel(itemdbindex, pos))
		return false;

	return true;
}

int CSocketList::GetJewelAt(char pos) const
{
	if(m_createdSocketCount <= 0 || m_createdSocketCount > m_maxSocketCount)
		return -1;

	if(pos < 0 || pos > m_createdSocketCount)
		return -1;

	if (m_socketpos == NULL)
		return -1;

	return m_socketpos[(int)pos].GetJewelDBIdx();
}

void CSocketList::GetSocketInfo(char pos, int& itemidx, int& optionidx, int& optionlevel) const
{
	if(m_createdSocketCount <= 0 || m_createdSocketCount > m_maxSocketCount)
	{
		itemidx		= -1;
		optionidx	= -1;
		optionlevel = -1;
		return ;
	}

	if(pos < 0 || pos >= m_createdSocketCount)
	{
		itemidx		= -1;
		optionidx	= -1;
		optionlevel = -1;
		return ;
	}

	if (m_socketpos == NULL)
	{
		itemidx		= -1;
		optionidx	= -1;
		optionlevel = -1;
		return ;
	}

	m_socketpos[(int)pos].GetSocketInfo( itemidx,  optionidx,  optionlevel);
}

void CSocketList::ApplyJewelOption(CPC* pc, CItem* pItem) const
{
	bool isJewelSkill = false;

	if (m_socketpos == NULL)
		return ;

	for(int i = 0; i < MAX_SOCKET_COUNT; i++)
	{
		if(pItem->m_socketList.m_socketpos[i].GetOption())
		{
			m_socketpos[i].ApplyJewelOption(pc, pItem, true);
		}
		if(pItem->m_socketList.m_socketpos[i].GetSkill())
		{
			m_socketpos[i].ApplyJewelOption(pc, pItem, false);
		}
	}
}

bool CSocketList::SetSocketCount(int scrollindex )
{
	socket_config *probtable = socket_config::Get_instance();
	st_prob_table *spt;
	spt = probtable->Get_prob_table(scrollindex);
	if(!spt)
		return false;

	int prob = GetRandom(1, 10000);
	int socketcount = 0;

	for (; socketcount < MAX_NORMAL_SOCKET_COUNT; socketcount++)
	{
		if (prob < spt->socket_prob_table[socketcount])
		{
			socketcount++;
			break;
		}
		prob = prob - spt->socket_prob_table[socketcount];
	}

	if(MAX_NORMAL_SOCKET_COUNT < (socketcount) )
	{
		return false;
	}

	if(!createsocket(socketcount))
		return false;

	return true;
}

bool CSocketList::InitSocketFromDB(int typeIdx, int subtypeIdx, char socketcount, int* jewelidx, int* flag)
{
	if(!createsocketFromDB(socketcount, jewelidx))
	{
		*flag &= ~FLAG_ITEM_SOCKET;
		return false;
	}
	if (jewelidx == NULL || m_socketpos == NULL)
		return false;

	bool bSetJewel = false;
	int i;

	if( jewelidx[0] >= 0 )
	{
		for(i=0; i < m_createdSocketCount; i++)
		{
			m_socketpos[i].SetJewel(jewelidx[i], i);
			bSetJewel = true;
		}
	}
	else
	{
		m_socketpos[0].SetItemIdx(-1);
		for(i = 1; i <= m_createdSocketCount; i++)
		{
			if(m_socketpos[i].SetJewel(jewelidx[i], i))
				bSetJewel = true;
		}
	}

	if(bSetJewel)
		*flag |= FLAG_ITEM_SOCKET;

	return true;
}

bool CSocketList::createsocketFromDB(char socketcount, int* jewelidx)
{
	if(socketcount > m_maxSocketCount || socketcount < 1)
	{
		return false;
	}

	if(m_socketpos)
	{
		delete [] m_socketpos;
		m_socketpos = NULL;
	}

	m_createdSocketCount = socketcount;
	int max_socket_count = MAX_SOCKET_COUNT;
	m_socketpos = new CSocketPos[max_socket_count];

	if( jewelidx[0] >= 0 )
	{
		for (int i = 0; i < m_createdSocketCount; i++)
		{
			if(jewelidx[i] >= 0)
				m_socketpos[i].SetSocketOpen();
			else
				m_socketpos[i].SetItemIdx(-1);
		}
	}
	else
	{
		for (int i = 1; i <= m_createdSocketCount; i++)
		{
			if(jewelidx[i] >= 0)
				m_socketpos[i].SetSocketOpen();
			else
				m_socketpos[i].SetItemIdx(-1);
		}
	}
	return true;
}

bool CSocketList::createsocket(char socketcount)
{
	if(socketcount > m_maxSocketCount || socketcount < 1)
	{
		return false;
	}

	if(m_socketpos)
	{
		delete [] m_socketpos;
		m_socketpos = NULL;
	}

	//socketcount; // normal socket + chaos socket(1)
	m_socketpos = new CSocketPos[MAX_SOCKET_COUNT];
	m_createdSocketCount = socketcount + 1;
	for (int i = 0; i < m_createdSocketCount; i++)
	{
		m_socketpos[i].SetSocketOpen();
	}

	return true;
}

void CSocketPos::SetSocketOpen()
{
	m_itemidx = 0;
}

bool CSocketList::IsCombinedJewel() const
{
	if (m_socketpos == NULL)
		return false;

	if(GetJewelAt(0) >= 0)
	{
		for(int i = 0 ; i < m_createdSocketCount; i++)
		{
			if(m_socketpos[i].GetJewelDBIdx() > 0)
			{
				return true;
			}
		}
		return false;
	}
	else
	{
		for(int i = 1 ; i <= m_createdSocketCount; i++)
		{
			if(m_socketpos[i].GetJewelDBIdx() > 0)
			{
				return true;
			}
		}
		return false;
	}
}

void CSocketList::GetSocketInfoString(char* buf) const
{
	*buf = 0;
	if(m_createdSocketCount > 0 && m_createdSocketCount <= this->m_maxSocketCount)
	{
		IntCat(buf, GetJewelAt(0), false);
		IntCat(buf, (m_createdSocketCount), true);

		for(int i=1; i < MAX_SOCKET_COUNT; i++)
		{
			IntCat(buf, GetJewelAt(i), true);
		}
	}
}

bool CSocketList::CopySocketInfo(const CSocketList& socketlist, int& flag)
{
	if(socketlist.GetMaxSocketCount() > MAX_SOCKET_COUNT)
	{
		flag &= ~FLAG_ITEM_SOCKET;
		return false;
	}

	char createsocketcount = socketlist.GetSocketCount() - 1;
	if(!createsocket(createsocketcount))
	{
		flag &= ~FLAG_ITEM_SOCKET;
		return false;
	}

	char i;
	int itemidx;
	if( socketlist.GetJewelAt(0) >= 0)
	{
		SetJewelAt(0, socketlist.GetJewelAt(0));
	}
	for(i=1; i < createsocketcount + 1; i++)
	{
		itemidx = socketlist.GetJewelAt(i);
		if(itemidx > 0)
		{
			if(SetJewelAt(i, itemidx))
			{
				flag |= FLAG_ITEM_SOCKET;
			}
		}
	}
	return true;
}

void CSocketList::InitSocketData()
{
	if (m_socketpos == NULL)
		return ;

	if(GetJewelAt(0) >= 0)
	{
		for (int i = 0; i < m_createdSocketCount; i++)
		{
			m_socketpos[i].Clear();
		}
	}
	else
	{
		for (int i = 1; i <= m_createdSocketCount; i++)
		{
			m_socketpos[i].Clear();
		}
	}
	delete [] m_socketpos;
	m_socketpos = NULL;
	m_createdSocketCount = 0;
}

void CSocketList::ClearOne(int pos)
{
	if (m_socketpos)
		m_socketpos[pos].Clear();
}

socket_config::socket_config()
{
}
socket_config::~socket_config()
{
	Release_instance();
}
socket_config* socket_config::Get_instance()
{
	if (Instance == NULL)
	{
		Instance = new socket_config();
		if(!Init_socket_config())
			return NULL;
	}
	return Instance;
}
void socket_config::Release_instance()
{
	std::map<int, st_prob_table*>::iterator itr = prob_map.begin();
	st_prob_table *spt;
	for (; itr != prob_map.end(); ++itr)
	{
		spt = itr->second;
		if(spt)
		{
			delete spt;
			spt = NULL;
		}
	}
	prob_map.clear();
}
bool socket_config::Init_socket_config()
{
	int file_size;
	FILE* r = fopen("./data/socket_config.json", "rb");
	if (NULL == r)
	{
		GAMELOG << init("ERROR CANNOT OPEN SOCKET_PROB_TABLE") << end;
		return false;
	}

	fseek(r, 0L, SEEK_END);
	file_size = ftell(r);
	fseek(r, 0L, SEEK_SET);

	char *readBuffer = new char[file_size];

	fread(readBuffer, 1, file_size, r);
	fclose(r);

	std::string config = readBuffer;
	Json::Value root;
	Json::Reader reader;

	if (!reader.parse(config, root))
	{
		GAMELOG << init("ERROR CANNOT PARSE SOCKET_PROB_TABLE") << end;
		return false;
	}

	Json::Value table = root["socket_prob"];
	st_prob_table *spt;
	int item_index;
	for (Json::Value::iterator itr = table.begin(); itr != table.end(); itr++)
	{
		Json::Value key_name = itr.key();
		Json::Value v = table[key_name.asString()];
		spt = new st_prob_table();

		for (unsigned int i = 0; i < v.size(); i++)
		{
			if (i == 0)
			{
				item_index = v[i].asInt();
			}
			else
			{
				if (i > 6) //socket_prob_table size is 6
				{
					continue;
				}
				else
					spt->socket_prob_table[i-1] = v[i].asInt();
			}
		}
		prob_map[item_index] = spt;
	}

	Json::Value scroll = root["seperate_scroll"];
	if ( !scroll.isArray() )
		seperate_scroll.push_back(scroll.asInt());
	else
	{
		for(unsigned int i = 0; i < scroll.size(); i++)
		{
			seperate_scroll.push_back(scroll[i].asInt());
		}
	}
	Json::Value cost = root["chaos_jewel_composite_need_nas"];
	chaos_jewel_composite_need_nas = cost.asInt();

	cost = root["normal_jewel_composite_need_nas"];
	normal_jewel_composite_need_nas = cost.asInt();

	cost = root["jewel_clean_factor"];
	jewel_clean_factor = cost.asInt();

	cost = root["chaos_jewel_clean_factor"];
	chaos_jewel_clean_factor = cost.asInt();

	delete [] readBuffer;
	return true;
}
st_prob_table* socket_config::Get_prob_table(int itemindex)
{
	return prob_map[itemindex];
}

bool socket_config::Is_seperate_scroll_item(int itemindex)
{
	std::vector<int>::iterator itr = std::find(seperate_scroll.begin(), seperate_scroll.end(), itemindex);
	if (itr != seperate_scroll.end())
		return true;
	return false;
}

int socket_config::Get_chaos_jewel_composite_cost()
{
	return chaos_jewel_composite_need_nas;
}

int socket_config::Get_normal_jewel_composite_cost()
{
	return normal_jewel_composite_need_nas;
}

int socket_config::Get_chaos_jewel_clean_factor()
{
	return chaos_jewel_clean_factor;
}
int socket_config::Get_jewel_clean_factor()
{
	return jewel_clean_factor;
}

int	CSocketList::IsSuccessDisable(int pos)
{
	if (m_socketpos && pos < MAX_SOCKET_COUNT)
	{
		if (m_socketpos[pos].IsSuccessDisable())
			return m_socketpos[pos].GetJewelDBIdx();
	}
	return -1;
}
bool CSocketList::SetSocketCountForAdmin(int count, bool IsOld)
{
	if(IsOld)
	{
		if(count - 9 > m_maxSocketCount || count - 9 < 1)
		{
			return false;
		}

		if(m_socketpos)
		{
			delete [] m_socketpos;
			m_socketpos = NULL;
		}

		//socketcount; // normal socket + chaos socket(1)
		m_socketpos = new CSocketPos[MAX_SOCKET_COUNT];
		m_createdSocketCount = count - 9;
		m_socketpos[0].SetItemIdx(-1);
		for (int i = 1; i < m_createdSocketCount; i++)
		{
			m_socketpos[i].SetSocketOpen();
		}

		return true;
	}
	else
	{
		if(MAX_NORMAL_SOCKET_COUNT < (count) )
		{
			return false;
		}

		if(!createsocket(count))
			return false;
		return true;
	}
}

bool CSocketPos::IsSuccessDisable()
{
	if ( !m_itemproto || m_itemproto->getItemNum2() * gserver->m_jewelDataList.m_jewelData->getEventCollectProb() / 100 < GetRandom(1, 10000) ) // fail
		return false;
	else
		return true;
}
