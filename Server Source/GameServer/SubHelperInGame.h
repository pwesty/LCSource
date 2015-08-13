#ifndef __SUBHELPER_IN_GAME_H__
#define __SUBHELPER_IN_GAME_H__

#include "../ShareLib/rnsocketiohandler.h"
#include "../ShareLib/rnsocketioservice.h"

class SubHelperInGameConnectTimer : public rnSocketIOHandler
{
public:
	SubHelperInGameConnectTimer() {}
	~SubHelperInGameConnectTimer() {}

	virtual void operate(rnSocketIOService* service);

	static SubHelperInGameConnectTimer* instance();
};

//////////////////////////////////////////////////////////////////////////

class SubHelperInGame : public rnSocketIOHandler
{
public:
	SubHelperInGame() {}
	~SubHelperInGame() {}

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
	static SubHelperInGame* instance();

private:
	rnSocketIOService*		service_;
	std::string				host_;
	int						port_;
};

#endif
