#include "StdH.h"
#include "SharedWebData.h"


CSharedWebData::CSharedWebData()
: m_szSendMsgBuffer(NULL)
, m_szReceiveMsgBuffer(NULL)
, m_bExit(false)
, m_szErrorMsgBuffer(NULL)
{
	m_hSendWriteEvent		= CreateEvent(NULL, TRUE, TRUE, NULL);
	m_hSendReadEvent		= CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hReceiveWriteEvent	= CreateEvent(NULL, TRUE, TRUE, NULL);
	m_hReceiveReadEvent		= CreateEvent(NULL, FALSE, FALSE, NULL);
}


CSharedWebData::~CSharedWebData()
{
	CloseHandle(m_hSendWriteEvent);
	CloseHandle(m_hSendReadEvent);
	CloseHandle(m_hReceiveWriteEvent);
	CloseHandle(m_hReceiveReadEvent);

	FreeSendMsgBuffer();
	FreeReceiveMsgBuffer();
	FreeErrorMsgBuffer();
}


///초기상태로 복구. 보통 오류가 났을 경우 초기상태로 되돌리기 위한 수단임.
void CSharedWebData::ResetAll()
{
	ResetEvent(m_hSendReadEvent);
	ResetEvent(m_hSendWriteEvent);
	FreeSendMsgBuffer();
	SetEvent(m_hSendWriteEvent);

	ResetEvent(m_hReceiveReadEvent);
	ResetEvent(m_hReceiveWriteEvent);
	FreeReceiveMsgBuffer();
	SetEvent(m_hReceiveWriteEvent);

	FreeErrorMsgBuffer();
}


void CSharedWebData::AllocSendMsgBuffer(int nSize)
{
	FreeSendMsgBuffer();
	m_szSendMsgBuffer = AllocMsgBuffer(nSize);
}


void CSharedWebData::AllocReceiveMsgBuffer(int nSize)
{
	FreeReceiveMsgBuffer();
	m_szReceiveMsgBuffer = AllocMsgBuffer(nSize);
}


void CSharedWebData::AllocErrorMsgBuffer(int nSize)
{
	FreeErrorMsgBuffer();
	m_szErrorMsgBuffer = AllocMsgBuffer(nSize);
}


char* CSharedWebData::AllocMsgBuffer(int nSize)
{
	char* szBuffer = new char[nSize];
	return szBuffer;
}


void CSharedWebData::FreeSendMsgBuffer()
{
	if( m_szSendMsgBuffer == NULL )
		return;

	delete[] m_szSendMsgBuffer;
	m_szSendMsgBuffer = NULL;
}


void CSharedWebData::FreeReceiveMsgBuffer()
{
	if( m_szReceiveMsgBuffer == NULL )
		return;

	delete[] m_szReceiveMsgBuffer;
	m_szReceiveMsgBuffer = NULL;
}


void CSharedWebData::FreeErrorMsgBuffer()
{
	if( m_szErrorMsgBuffer == NULL )
		return;

	delete[] m_szErrorMsgBuffer;
	m_szErrorMsgBuffer = NULL;
}
