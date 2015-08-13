#ifndef __RARE_OPTION_PROTO_H__
#define __RARE_OPTION_PROTO_H__


typedef struct __tagRareOptionData
{
	int						optiontype;		// option proto type
	int						optionlevel;	// option level
	int						optionval;		// option value
	int						optionprob;		// option prob.
	int						rarebit;		// bit field : 현재 데이터의 비트 필드 값
} RAREOPTIONDATA;

class CRareOptionProto
{
	int				m_nIndex;
	int				m_nDamageUp;
	int				m_nDefenseUp;
	int				m_nMagicUp;
	int				m_nResistUp;
	int				m_nGrade;
	RAREOPTIONDATA	m_rod[MAX_RARE_OPTION_SETTING];

public:
	CRareOptionProto();
	~CRareOptionProto();

	int GetIndex() const
	{
		return m_nIndex;
	}
	void SetIndex(int nIndex)
	{
		m_nIndex = nIndex;
	}

	const RAREOPTIONDATA* GetData(int n) const
	{
		return (const RAREOPTIONDATA*)(m_rod + n);
	}

	int GetDamageUp() const
	{
		return m_nDamageUp;
	}
	int GetDefenseUp() const
	{
		return m_nDefenseUp;
	}
	int GetMagicUp() const
	{
		return m_nMagicUp;
	}
	int GetResistUp() const
	{
		return m_nResistUp;
	}
	int	GetGrade()	const
	{
		return m_nGrade;
	}

	void InitData(	int nIndex,
					int nDamageUp,
					int nDefenseUp,
					int nMagicUp,
					int nResistUp,
					int nGrade,
					int optiontype[MAX_RARE_OPTION_SETTING],
					int optionlevel[MAX_RARE_OPTION_SETTING],
					int optionval[MAX_RARE_OPTION_SETTING],
					int optionprob[MAX_RARE_OPTION_SETTING]);
};

class CRareOptionProtoList
{
public:
	typedef std::map<int, CRareOptionProto*> map_t;

	CRareOptionProtoList();
	~CRareOptionProtoList();

	bool Load();

	const CRareOptionProto* Find(int nIndex);

private:
	map_t				map_;
	CRareOptionProto*	m_listRareOption;
	int					m_nCount;
};


#endif // __RARE_OPTION_PROTO_H__
//
