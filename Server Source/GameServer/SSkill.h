// SSkill.h: interface for the CSSkill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SSKILL_H__6305E9D0_9AE3_4F29_A7D2_D0C8F9DCF5A1__INCLUDED_)
#define AFX_SSKILL_H__6305E9D0_9AE3_4F29_A7D2_D0C8F9DCF5A1__INCLUDED_

class CSSkillProto
{
public:

	// DB
	int		m_index;			// DB Index
	int		m_job;				// Job
	int		m_type;				// Type : Config.h
	int		m_maxLevel;			// Max Level
	int		m_preference;		// Preference

	// 필요조건
	int		m_needLevel[SSKILL_MAX_LEVEL];	// Level
	int		m_needSP[SSKILL_MAX_LEVEL];		// SP
	int		m_needSSkill;					// SSkill index
	int		m_needSSkillLevel;				// SSkill Level

	int		m_num0[SSKILL_MAX_LEVEL];
	int		m_num1[SSKILL_MAX_LEVEL];
	int		m_num2[SSKILL_MAX_LEVEL];
	int		m_num3[SSKILL_MAX_LEVEL];
	int		m_num4[SSKILL_MAX_LEVEL];
	int		m_num5[SSKILL_MAX_LEVEL];
	int		m_num6[SSKILL_MAX_LEVEL];
	int		m_num7[SSKILL_MAX_LEVEL];
	int		m_num8[SSKILL_MAX_LEVEL];
	int		m_num9[SSKILL_MAX_LEVEL];

	CSSkillProto();
};

class CSSkill
{
public:
	CSSkillProto*	m_proto;		// SSkill Proto
	char			m_level;		// Level

	CSSkill(CSSkillProto* proto, char level);
};

class CSSkillProtoList
{
public:
	typedef std::map<int, CSSkillProto*> map_t;
	map_t			map_;
	CSSkillProto*	m_proto;		// SSkill 프로토 리스트
	int				m_nCount;		// SSkill 수

	CSSkillProtoList();
	~CSSkillProtoList();

	bool Load();							// DB 읽기
	CSSkillProto* FindProto(int index);		// SSkill proto 찾기
	CSSkill* Create(int index, int level = 1);	// SSKill 생성

private:
	std::vector<std::string>	a_level_need_level_str;
	std::vector<std::string>	a_level_need_sp_str;

	std::vector<std::string>	a_level_num0_str;
	std::vector<std::string>	a_level_num1_str;
	std::vector<std::string>	a_level_num2_str;
	std::vector<std::string>	a_level_num3_str;
	std::vector<std::string>	a_level_num4_str;
	std::vector<std::string>	a_level_num5_str;
	std::vector<std::string>	a_level_num6_str;
	std::vector<std::string>	a_level_num7_str;
	std::vector<std::string>	a_level_num8_str;
	std::vector<std::string>	a_level_num9_str;
};

class CSSkillNode
{
public:
	CSSkill*		m_sskill;
	CSSkillNode*	m_next;
	CSSkillNode*	m_prev;

	CSSkillNode(CSSkill* sskill)
	{
		m_sskill = sskill;
		m_next = m_prev = NULL;
	}
};

class CSSkillList
{
public:

	CSSkillNode*		m_head;
	int					m_nCount;

	CSSkillList();
	~CSSkillList();

	void Init();

	bool Add(CSSkill* sskill);		// SSkill 추가
	bool Remove(CSSkill* sskill);	// SSkill 삭제
	CSSkill* Find(int index);		// SSkillList에서 해당 인덱스 SSkill 찾기

	char* GetString(char* buf);		// DB 저장 String 만들기
	void GetString(std::string& buf);
};

#endif // !defined(AFX_SSKILL_H__6305E9D0_9AE3_4F29_A7D2_D0C8F9DCF5A1__INCLUDED_)
//
