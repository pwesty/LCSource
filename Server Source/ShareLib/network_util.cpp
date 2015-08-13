#ifndef WIN32
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#include <winsock2.h>
#endif

#include <string.h>
#include "network_util.h"

bool getLocalIp(std::string& buf)
{
	tCHAR host_name[256];

	gethostname(host_name, 256);

	struct hostent* hostinfo = gethostbyname(host_name);
	if (!hostinfo)
		return false;

	if (!hostinfo->h_addr_list)
		return false;

	tCHAR** addrs = hostinfo->h_addr_list;

	buf = inet_ntoa(*(struct in_addr*)*addrs);
	return true;
}