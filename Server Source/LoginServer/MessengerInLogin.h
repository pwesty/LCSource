#ifndef __MESSENGER_IN_LOGIN_H__
#define __MESSENGER_IN_LOGIN_H__

#include "../ShareLib/rnsocketiohandler.h"
#include "../ShareLib/rnsocketioservice.h"

class MessengerInLoginTimer : public rnSocketIOHandler
{
public:
	MessengerInLoginTimer() {}
	~MessengerInLoginTimer() {}

	virtual void operate(rnSocketIOService* service);

	static MessengerInLoginTimer* instance();
};

//////////////////////////////////////////////////////////////////////////

class MessengerInLogin : public rnSocketIOHandler
{
public:
	MessengerInLogin(void) : service_(NULL) {}
	~MessengerInLogin(void) {}

	void Connect();
	void WriteToOutput(CNetMsg::SP& msg);

	virtual void operate(rnSocketIOService* service);
	virtual void onClose(rnSocketIOService* service);

	virtual void onConnect(rnSocketIOService* service);
	virtual void onConnectFail(rnSocketIOService* service);

	static MessengerInLogin* instance();

private:
	rnSocketIOService* service_;
	std::string			host_;
	int					port_;
};

#endif
