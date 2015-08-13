#ifndef __CONNECT_IN_GAME_H__
#define __CONNECT_IN_GAME_H__

#include "../ShareLib/rnsocketiohandler.h"
#include "../ShareLib/rnsocketioservice.h"

class ConnectorInGameConnectTimer : public rnSocketIOHandler
{
public:
	ConnectorInGameConnectTimer() {}
	~ConnectorInGameConnectTimer() {}

	virtual void operate(rnSocketIOService* service);

	static ConnectorInGameConnectTimer* instance();
};

//////////////////////////////////////////////////////////////////////////

class ConnectorInGame : public rnSocketIOHandler
{
public:
	ConnectorInGame() {}
	~ConnectorInGame() {}

	void connect();
	bool isRun()
	{
		return (service_ != NULL);
	}
	void WriteToOutput(CNetMsg::SP& msg);

public:
	virtual void operate(rnSocketIOService* service);
	virtual void onClose(rnSocketIOService* service);

	virtual void onConnect(rnSocketIOService* service);
	virtual void onConnectFail(rnSocketIOService* service);

public:
	static ConnectorInGame* instance();

private:
	rnSocketIOService*		service_;
	std::string				host_;
	int						port_;
};

#endif
