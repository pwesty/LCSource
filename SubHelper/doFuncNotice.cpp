#include "../ShareLib/bnf.h"
#include "stdhdrs.h"
#if defined (NOTICE_SYSTEM)
#include "Server.h"
#include "doFunc.h"
#include "CmdMsg.h"
#include "../ShareLib/DBCmd.h"
#include "NoticeSystemAgent.h"

void do_Notice(CNetMsg::SP& msg, CDescriptor* d)
{
	int _type;

	RefMsg(msg) >> _type;

	switch ( _type )
	{
	case MSG_SUBHELPER_USER_NOTICE_REG_REQ:
		{
			int _characterIndex = 0;
			CLCString _name, _contents(81);

			RefMsg(msg) >> _characterIndex
				>> _name
				>> _contents;

			CNoticeSystemAgent::Instance()->reg(_characterIndex, _name, _contents);

			CNetMsg::SP rmsg(new CNetMsg);
			SubHelperUserNoticeRegRep(rmsg, _characterIndex);
			SEND_Q(rmsg, d);
		}
		break;
	}
}

#endif // NOTICE_SYSTEM
//