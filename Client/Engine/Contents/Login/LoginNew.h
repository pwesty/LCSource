#ifndef __LOGIN_H__
#define __LOGIN_H__
#include <Engine/LocalDefine.h>

class ENGINE_API LoginNew
{
public:
	LoginNew(void);
	virtual ~LoginNew(void);
	
	BOOL	ConnectToLoginServer();

	BOOL		GetSaveID()				{ return m_bSave; }
	CTString	GetUserId()				{ return m_strUserID; }
	CTString	GetPassword()			{ return m_strUserPW; }

	void	SetSaveID(BOOL bSave);
	void	SetUserId(CTString strId)		{ m_strUserID = strId; }
	void	SetPassword(CTString strPw)		{ m_strUserPW = strPw; }
	
private:
	void	SetVersion();

	BOOL	m_bSave;
	CTString m_strUserID;						// User ID
	CTString m_strUserPW;						// User Password

};



#endif	//	__LOGIN_H__