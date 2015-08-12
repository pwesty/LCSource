#pragma once

#include "IPatcher.h"
#include "../Common/ISingleton.h"
#include <list>
#include <string>

typedef std::list<std::string> _StringList;
typedef _StringList::iterator _StringIterator;

struct SOrganizeData
{
	std::string strRootPath;
	_PatchDataList listPatchData;
	_StringList listFolders;
};

class CBatchFilePatcher : public IPatcher, public ISingleton<CBatchFilePatcher>
{
public:
	virtual bool patch(_LogDataList& pLogDataList, const SPatchConfig& Config);

private:
	bool _organizeData(_LogDataList& pLogDataList, SOrganizeData& Organize);
	std::string _makeExportCommand(const SPatchData& PatchData, const SPatchConfig& Config) const;
};

#define BatchFilePatcher CBatchFilePatcher::instance()