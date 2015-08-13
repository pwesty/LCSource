#include "rnsocketiohandler.h"
#include "logsystem.h"
#include "bnf.h"

rnSocketIOHandler::rnSocketIOHandler()
{
}

rnSocketIOHandler::~rnSocketIOHandler()
{
}

ServiceHandler::ServiceHandler() : service_( NULL )
{
}

ServiceHandler::~ServiceHandler()
{
}

bool ServiceHandler::connect( const std::string host, tINT port )
{
	if( service_ != NULL )
		return false;

	session_handle chandle = bnf::instance()->CreateConnect( host, port, this );
	if (chandle == SessionBase::INVALID_SESSION_HANDLE)
	{
		LOG_ERROR( "Can't connect server. %s:%d", host.c_str(), port );
		return false;
	}

	service_ = (rnSocketIOService*)bnf::instance()->GetSessionPointer( chandle );
	return true;
}

bool ServiceHandler::asyncConnect( std::string host, tINT port )
{
	if( service_ != NULL )
		return false;

	session_handle handle = bnf::instance()->CreateAsyncConnect( host, port, this );
	if( handle == SessionBase::INVALID_SESSION_HANDLE )
	{
		LOG_ERROR( "Can't connect server. %s:%d", host.c_str(), port );
		return false;
	}

	service_ = (rnSocketIOService*)bnf::instance()->GetSessionPointer( handle );
	return true;
}