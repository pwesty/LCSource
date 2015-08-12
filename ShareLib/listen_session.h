#ifndef __LISTEN_SESSION_H__
#define __LISTEN_SESSION_H__

#include <string>
#include <boost/function.hpp>

#include "session_base.h"
#include "rnsocketiohandler.h"

#include "listen_sessionImpl.h"

class ListenSession : public SessionBase, public ListenSessionImpl
{
public:
	ListenSession(boost::asio::io_service& io_service, session_type type) : SessionBase(io_service, type) {};
	virtual ~ListenSession() {};

	virtual bool Run( std::string& host, const int port, rnSocketIOHandler* func, size_t receive_buffer_size = 0, size_t send_buffer_size = 0 ) = 0;

	virtual void Close() = 0;

	virtual void closeOnExecOn() = 0;
	virtual void closeOnExecOff() = 0;
};

#endif
