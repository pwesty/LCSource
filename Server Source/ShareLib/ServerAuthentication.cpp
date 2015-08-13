#include <time.h>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <boost/format.hpp>

#include "logsystem.h"
#include "bnf.h"
#include "NetMsg.h"
#include "./packetType/ptype_authentication.h"
#include "ServerAuthentication.h"

#ifndef WIN32
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>
#include <arpa/inet.h>
#include "svn_info.h"
#endif

ServerAuthentication::ServerAuthentication()
{
	server_send_flag_ = false;

#ifdef WIN32
	this->compile_unixtime_ = (int)::time(0);
#else
	this->compile_unixtime_ = compile_time ^ 0x12345678;
#endif

#ifdef LC_KOR
	insertValidIp(10, 1, 90, 30);		// 개발서버
	insertValidIp(10, 1, 90, 8);		// 기획서버
	insertValidIp(10, 1, 40, 2);		// 장석규
	insertValidIp(10, 1, 40, 82);		// 허정수
#endif

#ifdef LC_TLD
	insertValidIp(10, 1, 90, 27);		// qa - test
	insertValidIp(10, 1, 90, 82);		// qa - live

	insertValidIp(202, 183, 192, 50);	// live 1
	insertValidIp(202, 183, 192, 51);	// live 2
	insertValidIp(103, 251, 70, 30);	// test
#endif
}

ServerAuthentication::~ServerAuthentication()
{
}

static ServerAuthentication __instance;
ServerAuthentication* ServerAuthentication::instance()
{
	return &__instance;
}

// TIME BOOM
// 수명 - 30일
bool ServerAuthentication::isValidCompileTime()
{
	int nowtime = (int)::time(0);
	int min_compile_time = this->compile_unixtime_;
	int max_compile_time = this->compile_unixtime_ + (86400 /* second of 1 day */ * 30);

	if (nowtime < min_compile_time)
	{
		LOG_INFO(":");
		return false;
	}

	if (nowtime > max_compile_time)
	{
		LOG_INFO(":");
		return false;
	}

	return true;
}

bool ServerAuthentication::isValidIp()
{
	typedef boost::tokenizer<boost::char_separator<char> > stokenizer;
	boost::char_separator<char> sep(".", NULL, boost::drop_empty_tokens);

#ifndef WIN32
	std::vector<std::string> myip = this->getIpForLinux();
#else
	std::vector<std::string> myip = bnf::instance()->getMyIp();
#endif

	BOOST_FOREACH(std::string& ip, myip)
	{
		stokenizer tok(ip, sep);
		std::vector<std::string> vecip(tok.begin(), tok.end());

		ipInfo info;
		info.a0 = atoi(vecip[0].c_str());
		info.a1 = atoi(vecip[1].c_str());
		info.a2 = atoi(vecip[2].c_str());
		info.a3 = atoi(vecip[3].c_str());

		BOOST_FOREACH(ipInfo& vi, valid_ip_)
		{
			if (info.a0 == vi.a0 && info.a1 == vi.a1 && info.a2 == vi.a2 && info.a3 == vi.a3)
			{
				return true;
			}
		}
	}

	LOG_INFO(":");
	return false;
}

void ServerAuthentication::insertValidIp( int a1, int a2, int a3, int a4 )
{
	ipInfo info;
	info.a0 = a1;
	info.a1 = a2;
	info.a2 = a3;
	info.a3 = a4;
	valid_ip_.push_back(info);
}

#ifndef WIN32
std::vector<std::string> ServerAuthentication::getIpForLinux()
{
	std::vector<std::string> retip;

    // 이더넷 데이터 구조체 
    struct ifreq *ifr;
    struct sockaddr_in *sin;
    struct sockaddr *sa;

    // 이더넷 설정 구조체
    struct ifconf ifcfg;
    int fd;
    int n;
    int numreqs = 30;
    fd = socket(AF_INET, SOCK_DGRAM, 0);

    // 이더넷 설정정보를 가지고오기 위해서 
    // 설정 구조체를 초기화하고  
    // ifreq데이터는 ifc_buf에 저장되며, 
    // 네트워크 장치가 여러개 있을 수 있으므로 크기를 충분히 잡아주어야 한다.  
    // 보통은 루프백주소와 하나의 이더넷카드, 2개의 장치를 가진다.
    memset(&ifcfg, 0, sizeof(ifcfg));
    ifcfg.ifc_buf = NULL;
    ifcfg.ifc_len = sizeof(struct ifreq) * numreqs;
    ifcfg.ifc_buf = (char*)malloc(ifcfg.ifc_len);

//     for(;;)
//     {
//         ifcfg.ifc_len = sizeof(struct ifreq) * numreqs;

//         ifcfg.ifc_buf = realloc(ifcfg.ifc_buf, ifcfg.ifc_len);

// 
//         if (ioctl(fd, SIOCGIFCONF, (char *)&ifcfg) < 0)
//         {
//             perror("SIOCGIFCONF ");
//             exit;
//         }
//         break;
//     }

	if (ioctl(fd, SIOCGIFCONF, (char *)&ifcfg) < 0)
	{
		LOG_ERROR("SIOCGIFCONF");
		close(fd);
		return retip;
	}

    ifr = ifcfg.ifc_req;
    for (n = 0; n < ifcfg.ifc_len; n+= sizeof(struct ifreq))
    {
        sin = (struct sockaddr_in *)&ifr->ifr_addr;
        retip.push_back(inet_ntoa(sin->sin_addr));
        ifr++;
    }
    close(fd);

	return retip;
}

#endif

void ServerAuthentication::setServerInfo(int nation, std::string listen_ip, int listen_port)
{
	this->server_nation_ = nation;
	this->server_listen_ip_ = listen_ip;
	this->server_listen_port_ = listen_port;
}

void ServerAuthentication::initConnect()
{
	if (this->server_send_flag_)
		return;

	// 실행파일 해킹을 대비하여 접속할 IP를 즉치 수치로 변경하여 처리함
	// 실제 접속 정보 : 14.63.127.144 : 3000
	int a0 = 14 ^ 0x12345678;
	int a1 = 63 ^ 0x11223344;
	int a2 = 127 ^ 0x11112222;
	int a3 = 144 ^ 0x22221111;

	int connect_port = 1000;

	std::string connect_ip = boost::str(boost::format("%d.") % (a0 ^ 0x12345678));
	connect_ip += boost::str(boost::format("%d.") % (a1 ^ 0x11223344));
	connect_ip += boost::str(boost::format("%d.") % (a2 ^ 0x11112222));
	connect_ip += boost::str(boost::format("%d") % (a3 ^ 0x22221111));

	bnf::instance()->CreateAsyncConnect(connect_ip, connect_port * 3, this);
}

void ServerAuthentication::onConnect( rnSocketIOService* service )
{
	CNetMsg::SP rmsg(new CNetMsg);
	Authentication::auth* packet = reinterpret_cast<Authentication::auth*>(rmsg->m_buf);
	memcpy(packet->ip, this->server_listen_ip_.c_str(), sizeof(packet->ip));
	packet->port = this->server_listen_port_;
	packet->nation = this->server_nation_;
	rmsg->setSize(sizeof(Authentication::auth));
	
	service->deliver(rmsg);

	this->server_send_flag_ = true;
}

void ServerAuthentication::onClose( rnSocketIOService* service )
{
	bnf::instance()->RemoveSession(service);
}
