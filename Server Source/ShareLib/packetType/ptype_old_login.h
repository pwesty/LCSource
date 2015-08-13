#ifndef __PTYPE_LOGIN_H__
#define __PTYPE_LOGIN_H__

#include "ptype_base.h"

// 클라이언트와 동일하게 맞출것
// 빌드가 나가는(?) 날짜를 기준으로 숫자를 만들면 됨
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
