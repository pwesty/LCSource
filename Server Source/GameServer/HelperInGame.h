#ifndef __HELPER_IN_GAME_H__
#define __HELPER_IN_GAME_H__

#include "../ShareLib/rnsocketiohandler.h"
#include "../ShareLib/rnsocketioservice.h"

class HelperInGameConnectTimer : public rnSocketIOHandler
{
public:
	HelperInGameConnectTimer() {}
	~HelperInGameConnectTimer() {}

	virtual void operate(rnSocketIOService* service);

	static HelperInGameConnectTimer* instance();
};

//////////////////////////////////////////////////////////////////////////

class HelperInGame : public rnSocketIOHandler
{
public:
	HelperInGame() {}
	~HelperInGame() {}

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
	static HelperInGame* instance();

private:
	rnSocketIOService*		service_;
	std::string				host_;
	int						port_;
};

#endif
