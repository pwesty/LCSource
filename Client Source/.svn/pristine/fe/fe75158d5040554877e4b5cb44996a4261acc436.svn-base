#ifndef __PTYPE_LOGIN_H__
#define __PTYPE_LOGIN_H__

#include "ptype_base.h"

#define VERSION_FOR_CLIENT			(20141219)

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
	struct LoginFromClient : public pTypeBase
	{
		unsigned char		mode;
		unsigned char		nation;
		int					version;
		char				id[MAX_ID_NAME_LENGTH + 1];
		char				pw[MAX_PWD_LENGTH + 1];
	};
}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
