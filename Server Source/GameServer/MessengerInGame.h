#ifndef __MESSENGER_IN_GAME_H__
#define __MESSENGER_IN_GAME_H__

#include "../ShareLib/rnsocketiohandler.h"
#include "../ShareLib/rnsocketioservice.h"

class MessengerInGameConnectTimer : public rnSocketIOHandler
{
public:
	MessengerInGameConnectTimer() {}
	~MessengerInGameConnectTimer() {}

	virtual void operate(rnSocketIOService* service);

	static MessengerInGameConnectTimer* instance();
};

//////////////////////////////////////////////////////////////////////////

class MessengerInGame : public rnSocketIOHandler
{
public:
	MessengerInGame() {}
	~MessengerInGame() {}

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
	static MessengerInGame* instance();

private:
	rnSocketIOService*		service_;
	std::string				host_;
	int						port_;
};

#endif
