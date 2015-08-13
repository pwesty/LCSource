// DBCmd.h: interface for the CDBCmd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBCMD_H__6DD4D28A_37B8_43D4_BAB0_E11A88453639__INCLUDED_)
#define AFX_DBCMD_H__6DD4D28A_37B8_43D4_BAB0_E11A88453639__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __DBCMD_H__
#define __DBCMD_H__

#include "../include/mysql.h"
#include "LCString.h"
#include "stdhdrs.h"

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
	CLCString		m_sql;
	int				m_sqlLen;

	void Init(MYSQL* conn);
	bool Open();
	void Close();
	bool MoveNext();
	bool MoveFirst();
	bool Seek(int nOffset);	
	void SetQuery(const char* sql);
	bool Update();

	bool GetRec(const char* fieldname, int& n);
	bool GetRec(const char* fieldname, unsigned int& n);
	bool GetRec(const char* fieldname, short& n);
	bool GetRec(const char* fieldname, unsigned short& n);
	bool GetRec(const char* fieldname, char& ch);
	bool GetRec(const char* fieldname, unsigned char& ch);
	bool GetRec(const char* fieldname, CLCString& str, bool bTrim = true);
	bool GetRec(const char* fieldname, float& f);
	bool GetRec(const char* fieldname, LONGLONG& ll);

	bool GetRec(int fieldno, int& n);
	bool GetRec(int fieldno, unsigned int& n);
	bool GetRec(int fieldno, short& n);
	bool GetRec(int fieldno, unsigned short& n);
	bool GetRec(int fieldno, char& ch);
	bool GetRec(int fieldno, unsigned char& ch);
	bool GetRec(int fieldno, CLCString& str, bool bTrim = true);
	bool GetRec(int fieldno, float& f);
	bool GetRec(int fieldno, LONGLONG& ll);

	char* GetRec(const char* fieldname);
	char* GetRec(int fieldno);
	int FindField(const char* fieldname);

	void BeginTrans() { mysql_real_query(m_dbconn, "BEGIN", 5); }
	void Commit()  { mysql_real_query(m_dbconn, "COMMIT", 6); }
	void Rollback()  { mysql_real_query(m_dbconn, "ROLLBACK", 8); }
	const char* ErrorString() { return (const char*)mysql_error(m_dbconn); }
	int insertid() { return (int)mysql_insert_id(m_dbconn); }
};
extern volatile LC_THREAD_T		gThreadIDGameThread;
extern volatile LC_THREAD_T		gThreadIDDBThread;
#endif // __DBCMD_H__

#endif // !defined(AFX_DBCMD_H__6DD4D28A_37B8_43D4_BAB0_E11A88453639__INCLUDED_)
