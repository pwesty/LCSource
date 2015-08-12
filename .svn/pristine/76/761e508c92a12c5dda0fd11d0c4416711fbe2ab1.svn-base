#ifndef __CONFIG_FILE_H__
#define __CONFIG_FILE_H__

#define MAX_CONFIG_DATA		1024
#define CRYPTHEADER			"!crypt!"
#define LEN_CRYPTHEADER		7

class CConfigFileData
{
public:
	char m_group[256];
	char m_name[256];
	char m_value[256];

public:
	CConfigFileData()
	{
		m_group[0] = m_name[0] = m_value[0] = '\0';
	}
	~CConfigFileData()
	{
		m_group[0] = m_name[0] = m_value[0] = '\0';
	}

	void Set(const char* group, const char* name, const char* value)
	{
		strcpy(m_group, group);
		strcpy(m_name, name);
		strcpy(m_value, value);
	}

	const char* Value()
	{
		return (const char*)m_value;
	}
	const char* Group()
	{
		return (const char*)m_group;
	}
	const char* Name()
	{
		return (const char*)m_name;
	}
};

class CConfigFile
{
public:
	CConfigFileData*	m_data[MAX_CONFIG_DATA];
	int					m_cnt;

public:
	CConfigFile();
	~CConfigFile();

	bool Load(const char* filename, bool bCrypt = false);
	bool Load(const char* buf, int size);
	bool Add(const char* group, const char* name, const char* value);

	void Sort();

	const char* Find(const char* group, const char* name);

private:
	static int Comp(const void* p1, const void* p2);
};

#endif
//
