#ifndef		ITEM_HELP_H_
#define		ITEM_HELP_H_

#include <Engine/Entities/Items.h>

class ENGINE_API ItemHelp
{
public:
	static bool HaveItem(int ItemIndex, OUT SWORD* nTab = NULL, SWORD* nInvenIdx = NULL, __int64 nCnt = -1);
	static bool HasReuseTimeItem(CItems& rItem);

	static DOUBLE GetCoolTime(DOUBLE reUseTime, DOUBLE startTime);

};

#endif		// ITEM_HELP_H_