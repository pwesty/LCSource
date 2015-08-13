#include <stdlib.h>
#include "stdhdrs.h"

#include "ConfigFile.h"
#include "../ShareLib/CryptMem.h"

CConfigFile::CConfigFile()
{
	int i;
	for (i = 0; i < MAX_CONFIG_DATA; i++)
		m_data[i] = NULL;
	m_cnt = 0;
}

CConfigFile::~CConfigFile()
{
	int i;
	for (i = 0; i < MAX_CONFIG_DATA; i++)
	{
		if (m_data[i])
		{
			delete m_data[i];
			m_data[i] = NULL;
		}
	}
}

bool CConfigFile::Load(const char* filename, bool bCrypt)
{
	FILE* fp = fopen(filename, "rb");
	if (!fp)
	{
		LOG_INFO("File Open Error");
		return false;
	}

	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char* buf = new char[size];
	fread(buf, 1, size, fp);
	fclose(fp);
	if (bCrypt)
		DecryptMem(buf, size);
	Load(buf, size);
	delete [] buf;
	buf = NULL;

	return true;
}

bool CConfigFile::Load(const char* buf, int size)
{
	int i = 0;
	int gp = 0;
	int np = 0;
	int vp = 0;
	char group[1024] = "";
	char name[1024] = "";
	char value[1024] = "";

SKIP_SPACE:
	if (i >= size)
		goto END_FUNC;

	if (strchr("\t\r\n ", buf[i]))
	{
		i++;
		goto SKIP_SPACE;
	}

	if (buf[i] == '[')
	{
		i++;
		goto START_GROUP;
	}
	else if (buf[i] == ';')
		goto START_COMMENTS;
	else
		goto START_NAME;

START_GROUP:
	gp = 0;
	memset(group, 0, 1024);
	goto GROUP_DATA;

GROUP_DATA:
	if (i >= size)
		goto END_FUNC;

	if (buf[i] == ']')
	{
		i++;
		goto END_GROUP;
	}

	group[gp++] = buf[i];
	i++;
	goto GROUP_DATA;

END_GROUP:
	group[gp++] = '\0';
	goto SKIP_SPACE;

START_NAME:
	np = 0;
	memset(name, 0, 1024);
	goto NAME;

NAME:
	if (i >= size)
		goto END_FUNC;

	if (buf[i] == '=')
	{
		i++;
		goto END_NAME;
	}

	name[np++] = buf[i];
	i++;
	goto NAME;

END_NAME:
	name[np++] = '\0';
	goto START_FIND_EOL;

START_FIND_EOL:
	vp = 0;
	memset(value, 0, 1024);
	goto FIND_EOL;

FIND_EOL:
	if (i >= size)
		goto SAVE;

	if (strchr("\r\n", buf[i]))
	{
		i++;
		goto SAVE;
	}

	value[vp++] = buf[i];
	i++;
	goto FIND_EOL;

SAVE:
	value[vp++] = '\0';
#ifdef BUGFIX_REMOVE_SPACE
	if (name[0] && value[0])
	{
		TrimString(group);
		TrimString(name);
		TrimString(value);
		Add(group, name, value);
	}
#else
	if (name[0] && value[0])
		Add(group, name, value);
#endif
	goto SKIP_SPACE;

START_COMMENTS:
	if (i >= size)
		goto END_FUNC;

	if (strchr("\r\n", buf[i]))
	{
		i++;
		goto SKIP_SPACE;
	}

	i++;
	goto START_COMMENTS;

END_FUNC:
	return true;
}

bool CConfigFile::Add(const char* group, const char* name, const char* value)
{
	if(group == NULL || name == NULL || value == NULL) return false;

	char buf[1024];
	if (strncmp(value, CRYPTHEADER, LEN_CRYPTHEADER) == 0)
	{
		value += LEN_CRYPTHEADER;
		int len = strlen(value) / 2;
		char* p = buf;
		while (*value)
		{
			unsigned char v = 0;
			switch (*value)
			{
			case '0':
				v += 0;
				break;
			case '1':
				v += 1;
				break;
			case '2':
				v += 2;
				break;
			case '3':
				v += 3;
				break;
			case '4':
				v += 4;
				break;
			case '5':
				v += 5;
				break;
			case '6':
				v += 6;
				break;
			case '7':
				v += 7;
				break;
			case '8':
				v += 8;
				break;
			case '9':
				v += 9;
				break;
			case 'A':
				v += 10;
				break;
			case 'B':
				v += 11;
				break;
			case 'C':
				v += 12;
				break;
			case 'D':
				v += 13;
				break;
			case 'E':
				v += 14;
				break;
			case 'F':
				v += 15;
				break;
			}
			value++;
			v <<= 4;
			switch (*value)
			{
			case '0':
				v += 0;
				break;
			case '1':
				v += 1;
				break;
			case '2':
				v += 2;
				break;
			case '3':
				v += 3;
				break;
			case '4':
				v += 4;
				break;
			case '5':
				v += 5;
				break;
			case '6':
				v += 6;
				break;
			case '7':
				v += 7;
				break;
			case '8':
				v += 8;
				break;
			case '9':
				v += 9;
				break;
			case 'A':
				v += 10;
				break;
			case 'B':
				v += 11;
				break;
			case 'C':
				v += 12;
				break;
			case 'D':
				v += 13;
				break;
			case 'E':
				v += 14;
				break;
			case 'F':
				v += 15;
				break;
			}
			value++;

			*p = (char)v;
			p++;
		}
		DecryptMem(buf, len);
		value = buf;
	}
	const char* ret = Find(group, name);
	if (ret[0])
	{
		strcpy((char*)ret, value);
		return true;
	}

	if (m_cnt == MAX_CONFIG_DATA)
		return false;

	m_data[m_cnt] = new CConfigFileData;
	m_data[m_cnt]->Set(group, name, value);
	m_cnt++;

	Sort();

	return true;
}

void CConfigFile::Sort()
{
	if (m_cnt > 1)
		qsort(m_data, m_cnt, sizeof(CConfigFileData*), Comp);
}

const char* CConfigFile::Find(const char* group, const char* name)
{
	CConfigFileData* newdata = new CConfigFileData;
	newdata->Set(group, name, "");

	CConfigFileData** ret = (CConfigFileData**)bsearch(&newdata, m_data, m_cnt, sizeof(CConfigFileData*), Comp);

	delete newdata;
	newdata = NULL;

	if (ret == NULL)
		return "";

	return (*ret)->Value();
}

int CConfigFile::Comp(const void* p1, const void* p2)
{
	CConfigFileData* d1 = *((CConfigFileData**)p1);
	CConfigFileData* d2 = *((CConfigFileData**)p2);

	if (d1 == d2)
		return 0;

	if (d1 == NULL)
		return 1;
	if (d2 == NULL)
		return -1;

	int ret = strcmp(d1->Group(), d2->Group());
	if (ret != 0)
		return (ret < 0) ? -1 : 1;

	ret = strcmp(d1->Name(), d2->Name());
	if (ret != 0)
		return (ret < 0) ? -1 : 1;

	return 0;
}

