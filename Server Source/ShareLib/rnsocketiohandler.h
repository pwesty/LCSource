#ifndef __RENEW_SOCKETIOHANDLER_H__
#define __RENEW_SOCKETIOHANDLER_H__

#include "rntype.h"
#include "rnpacket.h"
#include "rnsocketioservice.h"

class rnSocketIOHandler
{
public:
	rnSocketIOHandler();
	virtual ~rnSocketIOHandler();

	virtual void operate(rnSocketIOService* service) = 0;
	virtual void onClose(rnSocketIOService* service) {}

	virtual void onConnect(rnSocketIOService* service) {}
	virtual void onConnectFail(rnSocketIOService* service) {}
};

class ServiceHandler : public rnSocketIOHandler
{
public:
	ServiceHandler();
	~ServiceHandler();

	virtual void operate(rnSocketIOService* service) = 0;

	virtual void onConnect(rnSocketIOService* service) {}
	virtual void onConnectFail(rnSocketIOService* service) {}

	virtual void onEvent( void* data ) {}

	bool connect( const std::string host, tINT port );
	bool asyncConnect( std::string host, tINT port );

	bool deliver( CNetMsg* packet );
	bool deliver( CNetMsg::SP packet );

private:
	rnSocketIOService* service_;
};

inline bool ServiceHandler::deliver( CNetMsg* packet )
{
	return deliver(CNetMsg::SP(packet));
}

inline bool ServiceHandler::deliver( CNetMsg::SP packet )
{
	if( service_ == NULL )
		return false;

	service_->deliver( packet );
	return true;
}

#endif
