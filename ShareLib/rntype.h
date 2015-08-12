#ifndef __RENEW_TYPE_H__
#define __RENEW_TYPE_H__

#if defined(_WIN32) || defined(WIN32)
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <map>
#include <string.h>

typedef char					tCHAR;
typedef unsigned char			tUCHAR;
typedef unsigned char			tBYTE;
typedef short int				tSINT;
typedef unsigned short int		tUSINT;
typedef int						tINT;
typedef unsigned int			tUINT;
typedef long					tLONG;
typedef unsigned long			tULONG;
typedef float					tFLOAT;
typedef double					tDOUBLE;
typedef long long				tINT64;
typedef	unsigned long long		tUINT64;

#ifndef WIN32
typedef time_t					tTIME;
typedef size_t					tSIZE;
//typedef bool					BOOL;
typedef unsigned int			DWORD;
typedef unsigned char			BYTE;
typedef DWORD					RET_VAL;
#else
#define snprintf _snprintf
#endif

inline tINT utf8_charbytes( tINT c )
{
	if( (c & 0x80) == 0 )
		return 1;

	if( (c & 0xe0) == 0xc0 )
		return 2;

	if( (c & 0xf0) == 0xe0 )
		return 3;

	if( (c & 0xf8) == 0xf0 )
		return 4;

	return -1;
}

inline tINT utf8_strlen( const tCHAR* utf8_str )
{
	tINT byte_len = (tINT)strlen( utf8_str );
	tINT char_len = 0;

	while( byte_len > 0 )
	{
		tINT len = utf8_charbytes( *utf8_str );
		if( len == -1 )
			return -1;

		utf8_str += len;
		byte_len -= len;

		char_len++;
	}

	return char_len;
}

#endif
