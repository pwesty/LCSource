// DBCmd.h: interface for the CDBCmd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBCMD_H__1A4004A6_17D2_4B7F_B25A_6EA90421E943__INCLUDED_)
#define AFX_DBCMD_H__1A4004A6_17D2_4B7F_B25A_6EA90421E943__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDBCmd  
{
public:
	CDBCmd();
	virtual ~CDBCmd();

	MYSQL*			m_dbconn;
	int				m_nrecords;
	MYSQL_RES*		m_res;
	MYSQL_ROW 		m_row;
	MYSQL_FIELD*	m_fieldinfo;
	int				m_nfields;
	char			m_sql[4096];
	int				m_sqlLen;

	void Init(MYSQL* conn);
	bool Open();
	void Close();
	bool MoveNext();
	bool MoveFirst();
	void SetQuery(const char* sql);
	bool Update();

	bool GetRec(const char* fieldname, int& n);
	bool GetRec(const char* fieldname, unsigned int& n);
	bool GetRec(const char* fieldname, short& n);
	bool GetRec(const char* fieldname, unsigned short& n);
	bool GetRec(const char* fieldname, char& ch);
	bool GetRec(const char* fieldname, unsigned char& ch);
	bool GetRec(const char* fieldname, char* str, bool bTrim = true);
	bool GetRec(const char* fieldname, float& f);
	bool GetRec(const char* fieldname, LONGLONG& ll);

	bool GetRec(int fieldno, int& n);
	bool GetRec(int fieldno, unsigned int& n);
	bool GetRec(int fieldno, short& n);
	bool GetRec(int fieldno, unsigned short& n);
	bool GetRec(int fieldno, char& ch);
	bool GetRec(int fieldno, unsigned char& ch);
	bool GetRec(int fieldno, char* str, bool bTrim = true);
	bool GetRec(int fieldno, float& f);
	bool GetRec(int fieldno, LONGLONG& ll);

	char* GetRec(const char* fieldname);
	char* GetRec(int fieldno);
	int FindField(const char* fieldname);
	void Throw_t(char *strFormat, ...);

};

#endif // !defined(AFX_DBCMD_H__1A4004A6_17D2_4B7F_B25A_6EA90421E943__INCLUDED_)
