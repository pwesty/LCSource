// DBCmd.cpp: implementation of the CDBCmd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DBCmd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBCmd::CDBCmd()
{
	m_dbconn = NULL;
	m_fieldinfo = NULL;
	m_nfields = 0;
	m_nrecords = 0;
	m_res = NULL;
	m_sql[0] = '\0';
	m_sqlLen = 0;
}

CDBCmd::~CDBCmd()
{
	Close();
	m_dbconn = NULL;
	m_nrecords = 0;
	m_res = NULL;
	m_fieldinfo = NULL;
	m_nfields = 0;
	m_sqlLen = 0;
}

void CDBCmd::Init(MYSQL* conn)
{
	m_dbconn = conn;
}

bool CDBCmd::Open()
{
	Close();

	if (mysql_real_query(m_dbconn, m_sql, m_sqlLen) == 0)
	{
		m_res = mysql_store_result(m_dbconn);

		if (m_res)
		{
			m_nrecords = (int)mysql_num_rows(m_res);
			m_nfields = (int)mysql_num_fields(m_res);
			m_fieldinfo = mysql_fetch_fields(m_res);
			return true;
		}
	}
	Throw_t("Open() Error [SQL Query : %s]\n", m_sql);

	return false;
}

void CDBCmd::Close()
{
	if (m_res)
	{
		mysql_free_result(m_res);
		m_res = NULL;
	}
}

bool CDBCmd::MoveFirst()
{
	if (m_res == NULL)
		return false;

	mysql_data_seek(m_res, 0);

	m_row = mysql_fetch_row(m_res);
	if (m_row == NULL)
		return false;

	return true;
}

bool CDBCmd::MoveNext()
{
	if (m_res == NULL)
		return false;

	m_row = mysql_fetch_row(m_res);
	if (m_row == NULL)
		return false;

	return true;
}

void CDBCmd::SetQuery(const char* sql)
{
	strcpy(m_sql, sql);
	m_sqlLen = strlen(sql);
}

bool CDBCmd::Update()
{
	Close();

	if (mysql_real_query(m_dbconn, m_sql, m_sqlLen) == 0)
	{
		m_nrecords = (int)mysql_affected_rows(m_dbconn);
		return true;
	}

	Throw_t("Update() Error [SQL Query : %s]\n", m_sql);
	return false;
}

bool CDBCmd::GetRec(const char* fieldname, int& n)
{
	return GetRec(FindField(fieldname), n);
}

bool CDBCmd::GetRec(const char* fieldname, unsigned int& n)
{
	return GetRec(FindField(fieldname), n);
}

bool CDBCmd::GetRec(const char* fieldname, short& n)
{
	return GetRec(FindField(fieldname), n);
}

bool CDBCmd::GetRec(const char* fieldname, unsigned short& n)
{
	return GetRec(FindField(fieldname), n);
}

bool CDBCmd::GetRec(const char* fieldname, char& ch)
{
	return GetRec(FindField(fieldname), ch);
}

bool CDBCmd::GetRec(const char* fieldname, unsigned char& ch)
{
	return GetRec(FindField(fieldname), ch);
}

bool CDBCmd::GetRec(const char* fieldname, char* str, bool bTrim)
{
	return GetRec(FindField(fieldname), str, bTrim);
}

bool CDBCmd::GetRec(const char* fieldname, float& f)
{
	return GetRec(FindField(fieldname), f);
}

bool CDBCmd::GetRec(const char* fieldname, LONGLONG& ll)
{
	return GetRec(FindField(fieldname), ll);
}

bool CDBCmd::GetRec(int fieldno, int& n)
{
	char* ret = GetRec(fieldno);
	if (ret == NULL)
		return false;

	n = atoi(ret);
	return true;
}

bool CDBCmd::GetRec(int fieldno, unsigned int& n)
{
	int t;
	if (!GetRec(fieldno, t))
		return false;

	n = (unsigned int)t;
	return true;
}

bool CDBCmd::GetRec(int fieldno, short& n)
{
	int t;
	if (!GetRec(fieldno, t))
		return false;

	n = (short)t;
	return true;
}

bool CDBCmd::GetRec(int fieldno, unsigned short& n)
{
	int t;
	if (!GetRec(fieldno, t))
		return false;

	n = (unsigned short)t;
	return true;
}

bool CDBCmd::GetRec(int fieldno, char& ch)
{
	int t;
	if (!GetRec(fieldno, t))
		return false;

	ch = (char)t;
	return true;
}

bool CDBCmd::GetRec(int fieldno, unsigned char& ch)
{
	int t;
	if (!GetRec(fieldno, t))
		return false;

	ch = (unsigned char)t;
	return true;
}

bool CDBCmd::GetRec(int fieldno, char* str, bool bTrim)
{
	char* ret = GetRec(fieldno);
	if (ret == NULL)
		return false;
	char* p = ret;
	if (bTrim)
	{
		while (*p && isspace(*p))
			p++;
	}
	strcpy(str, p);
	if (bTrim)
	{
		p = str + strlen(str) - 1;
		while (p > str && *p && isspace(*p))
			p--;
		p++;
		*p = '\0';
	}
	return true;
}

bool CDBCmd::GetRec(int fieldno, float& f)
{
	char* ret = GetRec(fieldno);
	if (ret == NULL)
		return false;

	f = (float)atof(ret);
	return true;
}

bool CDBCmd::GetRec(int fieldno, LONGLONG& ll)
{
	char* ret = GetRec(fieldno);
	if (ret == NULL)
		return false;

	ll = _atoi64(ret);
	return true;
}

char* CDBCmd::GetRec(const char* fieldname)
{
	return GetRec(FindField(fieldname));
}

char* CDBCmd::GetRec(int fieldno)
{
	if (m_res == NULL || fieldno < 0 || fieldno >= m_nfields)
	{		
		Throw_t("GetRec() Error [fieldno : %d]\n", fieldno);
		return NULL;
	}

	return m_row[fieldno];
}

int CDBCmd::FindField(const char* fieldname)
{
	if (m_fieldinfo == NULL) 
	{
		Throw_t("FindField() Error [fieldname : %s]\n", fieldname);
		return -1;
	}

	int i;
	for (i = 0; i < m_nfields ; i++)
		if (strcmp(fieldname, m_fieldinfo[i].name) == 0)
			return i;
	return -1;
}

void CDBCmd::Throw_t(char *strFormat, ...)  // throws char *
{
	const SLONG slBufferSize = 256;
	char strFormatBuffer[slBufferSize];
	char strBuffer[slBufferSize];
	// add the stream description to the format string
	_snprintf(strFormatBuffer, slBufferSize, "%s", strFormat);
	// format the message in buffer
	va_list arg;
	va_start(arg, strFormat); // variable arguments start after this argument
	_vsnprintf(strBuffer, slBufferSize, strFormatBuffer, arg);
	throw strBuffer;
}
