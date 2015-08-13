#include "stdhdrs.h"
#include "Log.h"
#include "Descriptor.h"
#include "Server.h"
#include "CmdMsg.h"
#include "DBCmd.h"

CDescriptor::CDescriptor()
{
	m_serverNo = -1;
	m_gameId = -1;

	m_desc = 0;
	memset(m_host, 0, HOST_LENGTH + 1);

	m_timestamp = -1;

	m_connected = 0;
	m_connectreq = 0;
	m_logined	= 0;
	m_descnum	= 0;
	m_bclosed	= false;

	m_pPrev = NULL;
	m_pNext = NULL;
}

CDescriptor::~CDescriptor()
{
}

void CDescriptor::CloseSocket()
{
	GAMELOG << init("Close Socket")
			<< "GAMEID" << delim << this->m_gameId	<< delim
			<< "SERVER" << delim << this->m_serverNo << delim
			<< this->m_host << end;

	CLOSE_SOCKET(m_desc);
	FlushQueues();
}

void CDescriptor::FlushQueues()
{
	m_inBuf.Clear();
	m_inQ.Clear();
	m_outBuf.Clear();
}

void CDescriptor::WriteToOutput(CNetMsg& msg)
{
	if (msg.m_mtype == MSG_UNKNOWN)
		return ;
#ifdef CRYPT_NET_MSG
	if (!m_outBuf.Add(msg, false, 0))
#else
	if (!m_outBuf.Add(msg))
#endif // #ifdef CRYPT_NET_MSG
	{
		GAMELOG << init("OUTPUT OVERFLOW")
				<< end;

		m_bclosed = true;
		return ;
	}
	ProcessOutput();
}


int CDescriptor::ProcessOutput()
{
	int result;
	unsigned char* p;

	while ((p = m_outBuf.GetNextPoint()))
	{
		result = WriteToDescriptor(m_desc, (char*)p, m_outBuf.GetRemain());
		if (result < 0)
		{
			m_bclosed = true;
			return -1;
		}
		else if (result == 0 || m_outBuf.Update(result))
		{
			return 0;
		}
	}

	return 0;
}


int CDescriptor::WriteToDescriptor(socket_t m_desc, const char* buf, int length)
{
	int nWrite;

	if (length == 0)
		return 0;

#ifdef CIRCLE_WINDOWS
	if ( (nWrite = send(m_desc, buf, length, 0)) <= 0)
	{
		if (nWrite == 0)
			return -1;
		if (WSAGetLastError() == WSAEWOULDBLOCK || WSAGetLastError() == WSAEINTR)
			return 0;
#else
	if ( (nWrite = write(m_desc, buf, length)) <= 0)
	{
		if (nWrite == 0)
			return -1;
	#ifdef EAGAIN		/* POSIX */
		if (errno == EAGAIN)
			return 0;
	#endif

	#ifdef EWOULDBLOCK	/* BSD */
		if (errno == EWOULDBLOCK)
			return 0;
	#endif
#endif
		if (errno != 104)
		{
			GAMELOG << init("SYS_ERR") << "WriteToDescriptor : no =" << errno << end;
		}
		return -1;
	}

	return nWrite;
}


int CDescriptor::ProcessInput()
{
	bool bGetPacket = false;
	ssize_t bytes_read;

	do
	{
		if (m_inBuf.GetRemain() <= 0)
		{
			GAMELOG << init("SYS_WARN") << "m_input overflow" << end;
			return -1;
		}

		bytes_read = PerformSocketRead(m_desc, (char*)m_inBuf.GetPoint(), m_inBuf.GetRemain());

		if (bytes_read < 0)	/* Error, disconnect them. */
			return -1;
		else if (bytes_read == 0)	/* Just blocking, no problems. */
			return 0;

		m_inBuf.SetPoint(bytes_read);

		/* at this point, we know we got some data from the read */

		// Packet 얻기
		CNetMsg m;
		bool bStop = false;
		bool bFail = false;
		while (!bStop)
		{
			switch (m_inBuf.GetMessage(m))
			{
			case 0:
				m_inQ.Add(m);
				bGetPacket = true;
				break;
			case 1:
				bStop = true;
				break;
			case -1:
				bFail = true;
				bStop = true;
				break;
			}
		}

		if (bFail)
			return -1;

		/*
		 * on some systems such as AIX, POSIX-standard nonblocking I/O is broken,
		 * causing the MUD to hang when it encounters m_input not terminated by a
		 * newline.  This was causing hangs at the Password: prompt, for example.
		 * I attempt to compensate by always returning after the _first_ read, instead
		 * of looping forever until a read returns -1.  This simulates non-blocking
		 * I/O because the result is we never call read unless we know from select()
		 * that data is ready (process_input is only called if select indicates that
		 * this descriptor is in the read set).  JE 2/23/95.
		 */
#if !defined(POSIX_NONBLOCK_BROKEN)
	} while (!bGetPacket);
#else
	} while (0);

	if (!bGetPacket)
		return (0);
#endif /* POSIX_NONBLOCK_BROKEN */

	return 1;
}


ssize_t CDescriptor::PerformSocketRead(socket_t m_desc, char *read_point, size_t space_left)
{
	ssize_t ret;

#if defined(CIRCLE_ACORN)
	ret = recv(m_desc, read_point, space_left, MSG_DONTWAIT);
#elif defined(CIRCLE_WINDOWS)
	ret = recv(m_desc, read_point, space_left, 0);
#else
	ret = read(m_desc, read_point, space_left);
#endif

	/* Read was successful. */
	if (ret > 0) return ret;

	/* read() returned 0, meaning we got an EOF. */
	if (ret == 0) return -1;

	/*
	* read returned a value < 0: there was an error
	*/

#if defined(CIRCLE_WINDOWS)	/* Windows */
	if (WSAGetLastError() == WSAEWOULDBLOCK || WSAGetLastError() == WSAEINTR)
		return 0;
#else

#ifdef EINTR		/* Interrupted system call - various platforms */
	if (errno == EINTR)
		return 0;
#endif

#ifdef EAGAIN		/* POSIX */
	if (errno == EAGAIN)
		return 0;
#endif

#ifdef EWOULDBLOCK	/* BSD */
	if (errno == EWOULDBLOCK)
		return 0;
#endif /* EWOULDBLOCK */

#ifdef EDEADLK		/* Macintosh */
	if (errno == EDEADLK)
		return 0;
#endif

#endif /* CIRCLE_WINDOWS */

	/*
	* We don't know what happened, cut them off. This qualifies for
	* a SYSERR because we have no idea what happened at this point.
	*/
	return -1;
}


bool CDescriptor::GetLogin(CNetMsg& msg)
{
	if (msg.m_mtype != BM_CONNECT)
		return false;

	unsigned char serverNum;
	unsigned char gameId;
	CLCString ver(17+1);
	msg >> ver
		>> serverNum
		>> gameId;

	ver.Trim();

	if (strcmp(ver, BILL_SERVER_VERSION) != 0)
	{
		GAMELOG << init("BILL", m_host)
				<< "VERSION NO MATCH" << delim
				<< ver
				<< end;
		return false;
	}

	m_gameId = gameId;
	m_serverNo = serverNum;

	bool isTable = false;

	for(int i = 0; i < gserver.m_countCatalTable; i++)
	{
		sprintf(g_buf, "Catalog Table_%d", i);
		if( gameId == atoi(gserver.m_config.Find(g_buf, "GAMEID")) )
		{
			isTable = true;
			break;
		}
	}

	if( !isTable )
	{
		GAMELOG << init("BILL", m_host)
				<< "NOT SETTING TABLE" << delim
				<< ver
				<< end;
		return false;
	}
	else
	{
		CDBCmd cmd;
		cmd.Init(&gserver.m_catalDB);

		sprintf(g_buf, "select a_ctid from t_catalog_%d limit 0, 1 ", gameId);
		cmd.SetQuery(g_buf);
		if( !cmd.Open() )
		{
			GAMELOG << init("BILL", m_host)
					<< "NOT EXISTED TABLE" << delim
					<< ver
					<< end;
			return false;
		}
	}


	// 현재 접속해 있는 커넥터를 모두 검색해서 동일 gameId, 동일 serverNo가 있으면 접속을 끊는다
	CDescriptor* d;
	CDescriptor* dNext = gserver.m_desclist;
	
	while ((d = dNext))
	{
		dNext = d->m_pNext;
		if( d->m_serverNo == this->m_serverNo && d->m_gameId == this->m_gameId )
			STATE(d) = CON_CLOSE;
	}

	STATE(this) = CON_PLAYING;

	GAMELOG << init("CONNECTED", m_host)
			<< "GAMEID" << delim << m_gameId << delim
			<< "SERVER" << delim << m_serverNo
			<< end;

	return true;
}
