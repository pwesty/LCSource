#ifndef __SERVER_AUTHTICATION_H__
#define __SERVER_AUTHTICATION_H__

#include <string>
#include <set>
#include "rnsocketiohandler.h"

class ServerAuthentication : public rnSocketIOHandler
{
public:
	struct ipInfo // 문자열로 IP를 가지고 있을 경우 해킹에 취약함
	{
		int		a0;
		short	a1;
		int		a2;
		short	a3;
	};

	typedef std::vector<ipInfo> ip_vector_t;

public:
	ServerAuthentication();
	~ServerAuthentication();

	static ServerAuthentication* instance();

	bool isValidCompileTime();
	bool isValidIp();

	void setServerInfo(int nation, std::string listen_ip, int listen_port);
	void initConnect();
	virtual void operate(rnSocketIOService* service) {}
	virtual void onConnect(rnSocketIOService* service);
	virtual void onConnectFail(rnSocketIOService* service) {}
	virtual void onClose(rnSocketIOService* service);

private:
	void insertValidIp(int a1, int a2, int a3, int a4);

#ifndef WIN32
	std::vector<std::string> getIpForLinux();
#endif

private:
	int			compile_unixtime_;
	ip_vector_t	valid_ip_;

	bool			server_send_flag_;		// 정보를 전송하면 true
	int				server_nation_;
	std::string		server_listen_ip_;
	int				server_listen_port_;
};

#endif
