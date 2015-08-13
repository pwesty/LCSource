#ifndef __CLIENT_FOR_SERVER_H__
#define __CLIENT_FOR_SERVER_H__

#include "../ShareLib/MemoryPoolBase.h"
#include "../ShareLib/rnsocketiohandler.h"


class ClientForServer : public rnSocketIOHandler, public MemoryPoolBase<ClientForServer>
{
public:
	ClientForServer() {}
	~ClientForServer() {}

	virtual void operate(rnSocketIOService* service);
	virtual void onClose(rnSocketIOService* service);
};

class ClientForServerListen : public rnSocketIOHandler
{
public:
	ClientForServerListen() {}
	~ClientForServerListen() {}

	static ClientForServerListen* instance();

	virtual void operate(rnSocketIOService* service);
};

#endif
