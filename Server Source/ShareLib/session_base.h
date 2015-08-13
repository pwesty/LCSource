#ifndef __SESSION_BASE_H__
#define __SESSION_BASE_H__

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

typedef unsigned int session_type;
typedef unsigned int session_handle;

class SessionBase
{
public:
	enum { INVALID_SESSION_HANDLE = 0 };

	enum
	{
		SESSION,
		CONNECT_SESSION,
		TIMER_SESSION,
		LISTEN_SESSION,
		EVENT_SESSION,
		EXIT_SESSION
	};

public:
	SessionBase(boost::asio::io_service& io_service, session_type type)	: io_service_(io_service)
	{
		type_ = type;
		handle_ = INVALID_SESSION_HANDLE;
		user_data_ = NULL;
		close_error_ = false;
	}
	virtual ~SessionBase()
	{
		user_data_ = NULL;
	}

	void SetType(session_type type)
	{
		type_ = type;
	}
	session_type GetType()
	{
		return type_;
	}

	void SetHandle(session_handle handle)
	{
		handle_ = handle;
	}
	session_handle GetHandle()
	{
		return handle_;
	}

	void SetUserData(void *data)
	{
		user_data_ = data;
	}
	void *GetUserData()
	{
		return user_data_;
	}

	void SetErrorMessage(std::string str)
	{
		error_message_ = str;
	}
	std::string& GetErrorMessage()
	{
		return error_message_;
	}

	bool isSesseion()
	{
		return (type_ == SESSION);
	}
	bool isListenSession()
	{
		return (type_ == LISTEN_SESSION);
	}
	bool isTimerSession()
	{
		return (type_ == TIMER_SESSION);
	}
	bool isConnectSession()
	{
		return (type_ == CONNECT_SESSION);
	}
	bool isEventSession()
	{
		return (type_ == EVENT_SESSION);
	}

	virtual bool isValid()
	{
		return (handle_ != INVALID_SESSION_HANDLE);
	}

	bool isCloseError()
	{
		return close_error_;
	}

protected:
	session_type				type_;
	session_handle				handle_;
	void*						user_data_;

	std::string					error_message_;

	boost::asio::io_service&	io_service_;
	bool						close_error_;
};

#endif
