#ifndef		CMD_UI_TOOL_H_
#define		CMD_UI_TOOL_H_

#include <Engine/Base/Command.h>

//------------------------------------------------------------------------

class CmdOpenFile : public Command
{
public:
	void setData(const char* path)	{ strPath_ = path; }
	void execute();

private:
	std::string		strPath_;
};

//------------------------------------------------------------------------


#endif		// CMD_UI_TOOL_H_