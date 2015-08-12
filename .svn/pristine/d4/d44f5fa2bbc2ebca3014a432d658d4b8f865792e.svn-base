#ifndef __CLIENT_FOR_CLIENT_H__
#define __CLIENT_FOR_CLIENT_H__

#include "../ShareLib/MemoryPoolBase.h"
#include "../ShareLib/rnsocketiohandler.h"


class ClientForClient : public rnSocketIOHandler, public MemoryPoolBase<ClientForClient>
{
public:
	ClientForClient() {}
	~ClientForClient() {}

	virtual void operate(rnSocketIOService* service);
	virtual void onClose(rnSocketIOService* service);
};

class ClientForClientListen : public rnSocketIOHandler
{
public:
	ClientForClientListen() {}
	~ClientForClientListen() {}

	static ClientForClientListen* instance();

	virtual void operate(rnSocketIOService* service);
};

#endif
