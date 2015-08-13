#include "BatchFilePatcher.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include "windows.h"

DLL_API IPatcher* getBatchFilePatcher()
{
	//IPatcher* pPatcher = new CBatchFilePatcher;
	//return pPatcher;
	return CBatchFilePatcher::instance();
}

bool CBatchFilePatcher::patch(_LogDataList& pLogDataList, const SPatchConfig& Config)
{
	if (pLogDataList.empty())
		return false;

	// 필요한 데이터, list 생성
	SOrganizeData organize;

	// 파일 경로 정리를 위한 RootPath
	organize.strRootPath = Config.strSvnUrl.substr(Config.strSvnUrl.rfind('/') + 1);

	// 정리
	_organizeData(pLogDataList, organize);

	std::ofstream fPatcher(Config.strFilePath, std::ios_base::trunc);

	// 폴더 생성
	_StringIterator iter = organize.listFolders.begin();

	while (iter != organize.listFolders.end())
	{
		fPatcher << "md " << "\"" << *iter << "\"" << std::endl;

		++iter;
	}

	// 파일 생성
	_PatchDataIterator iter2 = organize.listPatchData.begin();

	while (iter2 != organize.listPatchData.end())
	{
		std::string path = _makeExportCommand(*iter2, Config);
		fPatcher << path << std::endl;

		++iter2;
	}

	fPatcher.close();

	organize.listPatchData.clear();
	organize.listFolders.clear();

	return true;
}

bool CBatchFilePatcher::_organizeData(_LogDataList& pLogDataList, SOrganizeData& Organize)
{
	std::string& strRootPath = Organize.strRootPath;
	_PatchDataList& listPatchData = Organize.listPatchData;
	_StringList& listFolders = Organize.listFolders;

	// 경로 정리
	_LogDataIterator iterData = pLogDataList.begin();

	while (iterData != pLogDataList.end())
	{
		_PathIterator iterPath = iterData->paths.begin();
		int revision = iterData->revision;

		while (iterPath != iterData->paths.end())
		{
			std::string path = iterPath->text;

			// 상위 루트폴더까지 경로 제거
			std::string parsed = path.substr(path.find(strRootPath) + strRootPath.length() + 1);

			if (iterPath->action != "D") // A, M
			{
				// 추가 되었는지 확인
				_PatchDataIterator iter = std::find(listPatchData.begin(), listPatchData.end(), parsed);

				// 추가 되지 않았을 경우
				if (iter == listPatchData.end())
					listPatchData.push_back(SPatchData(revision, parsed));
				else if (iter->revision < revision)
					iter->revision = revision;
			}
			else
			{
				// 삭제해야할 파일 검색
				_PatchDataIterator iter = std::find(listPatchData.begin(), listPatchData.end(), parsed);

				if (iter != listPatchData.end())
					listPatchData.erase(iter);
			}

			++iterPath;
		}

		++iterData;
	}

	// 폴더 정리
	_PatchDataIterator iter = listPatchData.begin();

	while (iter != listPatchData.end())
	{
		std::string path = iter->path;
		std::string::size_type pos = path.find('/');

		while (pos != std::string::npos)
		{
			path.replace(pos, 1, "\\");
			pos = path.find('/');
		}

		std::string front = path.substr(0, path.rfind('\\'));

		_StringIterator iter2 = std::find(listFolders.begin(), listFolders.end(), front);

		// 추가 되었는지 확인
		if (iter2 == listFolders.end())
			listFolders.push_back(front);

		++iter;
	}

	return true;
}

std::string CBatchFilePatcher::_makeExportCommand(const SPatchData& PatchData, const SPatchConfig& Config) const
{
	std::string strExport = PatchData.path;

	// '/' -> "\\"
	std::string::size_type pos = strExport.find('/');

	while (pos != std::string::npos)
	{
		strExport.replace(pos, 1, "\\");
		pos = strExport.find('/');
	}

	std::stringstream command;
	command << "svn export ";
	command << "-r " << PatchData.revision << " ";
	command << "\"" << Config.strSvnUrl << "/" << PatchData.path << "\" ";
	command << "\"" << strExport << "\"";

	return command.str();
}
