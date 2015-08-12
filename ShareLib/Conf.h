#ifndef __CONF_H__
#define __CONF_H__

#ifdef  WIN32

#ifndef CIRCLE_WINDOWS
#define CIRCLE_WINDOWS
#endif // CIRCLE_WINDOWS

#define STDC_HEADERS 1
#define HAVE_INET_ADDR 1
#define HAVE_ASSERT_H 1
#define HAVE_ERRNO_H 1
#define HAVE_FCNTL_H 1
#define HAVE_LIMITS_H 1
#define HAVE_STRING_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_STRUCT_IN_ADDR 1

typedef __int64				LONGLONG;
typedef unsigned __int64	ULONGLONG;
#define ATOLL(str)			_atoi64(str)

#include <WinSock2.h>

#else // _MSC_VER

typedef long long			LONGLONG;
typedef unsigned long long	ULONGLONG;
#define ATOLL(str)			atoll(str)

#include <arpa/inet.h>

#define HAVE_SYS_WAIT_H 1
#define STDC_HEADERS 1
#define TIME_WITH_SYS_TIME 1
#define CIRCLE_CRYPT 1
#define HAVE_STRUCT_IN_ADDR 1
#define HAVE_INET_ADDR 1
#define HAVE_INET_ATON 1
#define HAVE_ARPA_INET_H 1
#define HAVE_ARPA_TELNET_H 1
#define HAVE_ASSERT_H 1
#define HAVE_CRYPT_H 1
#define HAVE_ERRNO_H 1
#define HAVE_FCNTL_H 1
#define HAVE_LIMITS_H 1
#define HAVE_MEMORY_H 1
#define HAVE_NETDB_H 1
#define HAVE_NETINET_IN_H 1
#define HAVE_SIGNAL_H 1
#define HAVE_STRING_H 1
#define HAVE_STRINGS_H 1
#define HAVE_SYS_FCNTL_H 1
#define HAVE_SYS_RESOURCE_H 1
#define HAVE_SYS_SELECT_H 1
#define HAVE_SYS_SOCKET_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TIME_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_SYS_UIO_H 1
#define HAVE_UNISTD_H 1

#endif
//

#endif
