#ifndef __LISTEN_SESSION_IMPL_H__
#define __LISTEN_SESSION_IMPL_H__

#include <string>

class rnSocketIOHandler;

class ListenSessionImpl
{
public:
	ListenSessionImpl() {};
	virtual ~ListenSessionImpl() {};

	virtual bool Run( std::string& host, const int port, rnSocketIOHandler* func, size_t receive_buffer_size = 0, size_t send_buffer_size = 0 ) = 0;

	virtual void Close() = 0;

	virtual void closeOnExecOn() = 0;

	virtual void closeOnExecOff() = 0;
};

#endif
