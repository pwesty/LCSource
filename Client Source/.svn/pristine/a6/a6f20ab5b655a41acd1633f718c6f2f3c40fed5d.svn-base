#pragma once

#include "../Common/DLLAPI.h"
#include "SLogData.h"

struct SPatchConfig
{
	std::string strSvnUrl;
	std::string strFilePath;
};

class IPatcher
{
public:
	virtual bool patch(_LogDataList& pLogDataList, const SPatchConfig& Config) = 0;
};

DLL_API IPatcher* getBatchFilePatcher();