#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#include <sys/time.h>

typedef unsigned char   UCHAR;
typedef unsigned short  USHORT;
typedef int             INT;
typedef int             INT32;
typedef unsigned int    UINT;
typedef unsigned int    UINT32;
typedef long long       INT64;
typedef long            LONG;
typedef unsigned long   ULONG;
typedef short           SHORT;
typedef char            CHAR;
typedef struct timeval  st_TimeVal;
typedef time_t			TIME_T;
typedef float			FLOAT;
typedef double			DOUBLE;

typedef unsigned long long TIME_T64;

typedef unsigned long long  UINT64;

typedef struct _STRING_SHORT
{
	char    szValue[40];
	USHORT  usValue;
} string_short;

#endif

