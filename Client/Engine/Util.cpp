
#include "StdH.h"
#include "Util.h"
#include <string>
#if		defined(VER_TEST)
#include <Engine/Interface/UIManager.h>
#include <Engine/Contents/Base/ChattingUI.h>
#endif	// VER_TEST

void ENGINE_API LogOutput( const char* format, ... )
{
	std::string strFormat = format;
	strFormat += "\n";

	va_list args;
	va_start(args, format);
	char buffer[1025];
	_vsnprintf(buffer, 1024, strFormat.c_str(), args);

	OutputDebugString(buffer);
}

#ifdef	VER_TEST
void ENGINE_API LogOutputChat( const char* format, ... )
{
	std::string strFormat = format;
	strFormat += "\n";

	va_list args;
	va_start(args, format);
	char buffer[1025];
	_vsnprintf(buffer, 1024, strFormat.c_str(), args);

	CUIManager* pUIMgr = CUIManager::getSingleton();

	if (pUIMgr->GetDebugOutMode() == true)
	{
		pUIMgr->GetChattingUI()->AddSysMessage(CTString(buffer));
	}
}
#endif	// VER_TEST