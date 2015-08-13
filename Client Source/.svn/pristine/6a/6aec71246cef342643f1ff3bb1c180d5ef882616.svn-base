#ifndef __SHAREDWEBDATA_H__
#define __SHAREDWEBDATA_H__


//-------------- CSharedWebData --------------//
///Web thread와 Game thread간 공유될 객체의 class
//--------------------------------------------//

class CSharedWebData
{
public:
	CSharedWebData();
	~CSharedWebData();
	
	void	ResetAll();

	HANDLE	GetSendWriteEventHandle()		{ return m_hSendWriteEvent;		}
	HANDLE	GetSendReadEventHandle()		{ return m_hSendReadEvent;		}
	HANDLE	GetReceiveWriteEventHandle()	{ return m_hReceiveWriteEvent;	}
	HANDLE	GetReceiveReadEventHandle()		{ return m_hReceiveReadEvent;	}

	void	AllocSendMsgBuffer(int nSize);
	void	FreeSendMsgBuffer();
	char*	GetSendMsgBuffer()				{ return m_szSendMsgBuffer;		}

	void	AllocReceiveMsgBuffer(int nSize);
	void	FreeReceiveMsgBuffer();
	char*	GetReceiveMsgBuffer()			{ return m_szReceiveMsgBuffer;	}

	void	SetExit(bool bExit)				{ m_bExit = bExit;				}
	bool	GetExit()						{ return m_bExit;				}

	void	AllocErrorMsgBuffer(int nSize);
	void	FreeErrorMsgBuffer();
	char*	GetErrorMsgBuffer()				{ return m_szReceiveMsgBuffer;	}

	char*	AllocMsgBuffer(int nSize);

private:
	HANDLE	m_hSendWriteEvent;		//Send Message Buffer를 쓰기 위해 접근할 때 확인할 event
	HANDLE	m_hSendReadEvent;		//           〃         읽기            〃
	char*	m_szSendMsgBuffer;

	HANDLE	m_hReceiveWriteEvent;	//Receive Message Buffer를 쓰기 위해 접근할 때 확인할 event
	HANDLE	m_hReceiveReadEvent;	//           〃            읽기            〃
	char*	m_szReceiveMsgBuffer;

	bool	m_bExit;
	char*	m_szErrorMsgBuffer;		//접근 권한은 Receive Message Buffer와 공유
};

#endif	// __SHAREDWEBDATA_H__