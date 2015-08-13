#ifndef _LEVELUP_MSG_H_
#define _LEVELUP_MSG_H_

#include <Engine/Help/LoadLod.h>
#include <common/header/def_base.h>

struct stLevelUpMsgData : public LodLoader<stLevelUpMsgData>
{
	int level;
	int strIndex;

	int getIndex() { return level;	}
};

class ENGINE_API CLevelUpGuide : public stLevelUpMsgData
{
public:
	static bool loadEx(const char* szFileName);
};

#endif // _LEVELUP_MSG_H_