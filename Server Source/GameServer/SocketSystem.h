#ifndef __SOCKET_H__
#define __SOCKET_H__

class CItemProto;
class COption;

class CSocketPos
{
public:
	CSocketPos();
	~CSocketPos();
	bool		SetJewel(int itemidx, int pos);
	void		SetSocketOpen();
	bool		IsSuccessDisable();
	void		Clear();

	int			GetJewelDBIdx() const ;

	void		ApplyJewelOption(CPC* pc, CItem* pItem, bool isOption) const;

	COption*	GetOption() const;

	CSkill*		GetSkill() const;

	void		GetSocketInfo(int& itemidx, int& optionidx, int& optionlevel) const;

	void		SetItemIdx( int itemIndex )
	{
		m_itemidx = itemIndex;
	}

	CItemProto* getItemProto()
	{
		return m_itemproto;
	}

	int			m_skillProb;

private:
	int			m_itemidx;
	CItemProto*	m_itemproto;
	COption*	m_option;
	CSkill*		m_skill;
};

class CSocketList
{
	friend class CItemList;
public:

	enum SOCKETMAXCOUNT
	{
		MAXSOCKET0 = 0,
		MAXSOCKET2 = 2,
		MAXSOCKET4 = 4,
	};

	CSocketList();
	~CSocketList();

	bool	SetJewelAt(int pos, int itemdbindex);

	bool	SetSocketCount(int scrollidx);
	void	InitSocketData();
	int		IsSuccessDisable(int pos);
	void	ClearOne(int pos);
	bool	SetSocketCountForAdmin(int count, bool IsOld = false);
	void	ClearAll();

	void	ApplyJewelOption(CPC* pc, CItem* pItem) const;

	char	GetSocketCount() const;

	char	GetMaxSocketCount() const
	{
		return m_maxSocketCount;
	}

	int		GetJewelAt(char pos) const;

	COption* GetJewelOptionAt(char pos) const
	{
		if(m_socketpos == NULL || pos < 0 || pos >= MAX_SOCKET_COUNT)
			return NULL;

		return m_socketpos[(int)pos].GetOption();
	}

	bool	IsCombinedJewel() const;

	char	GetCombinedCount() const;

	void	GetSocketInfo(char pos, int& itemidx, int& optionidx, int& optionlevel) const;

	void	GetSocketInfoString(char* buf) const;

	CSocketPos* getSocketPos(int position)
	{
		return &m_socketpos[position];
	}

	CSocketPos*		m_socketpos;

	bool	CopySocketInfo(const CSocketList& socketlist, int& flag);

private:
	bool			InitSocketFromDB(int typeIdx, int subtypeIdx, char socketcount, int* jewelidx, int* flag);
	bool			createsocketFromDB(char socketcount, int* jewelidx);
	bool			createsocket(char socketcount);
	const static int m_maxSocketCount = MAX_SOCKET_COUNT;
	char			m_createdSocketCount;
};

typedef struct _st_prob_table
{
	int socket_prob_table[MAX_NORMAL_SOCKET_COUNT];
} st_prob_table;

class socket_config
{
private:
	socket_config();
public:
	~socket_config();
	static socket_config* Get_instance();
	static st_prob_table *Get_prob_table(int itemindex);
	static bool Is_seperate_scroll_item(int itemindex);
	static int Get_chaos_jewel_composite_cost();
	static int Get_normal_jewel_composite_cost();
	static int Get_chaos_jewel_clean_factor();
	static int Get_jewel_clean_factor();
private:
	static std::vector<int> seperate_scroll;
	static std::map<int, st_prob_table*> prob_map;
	static socket_config* Instance;
	static int chaos_jewel_composite_need_nas;
	static int normal_jewel_composite_need_nas;
	static int jewel_clean_factor;
	static int chaos_jewel_clean_factor;
	static void Release_instance();
	static bool Init_socket_config();
};

#endif // __SOCKET_H__
//
