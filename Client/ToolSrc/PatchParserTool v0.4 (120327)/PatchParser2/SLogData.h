#pragma once

#include <string>
#include <list>

struct SResourcePath
{
	std::string action;
	std::string text;

	SResourcePath(const std::string& a, const std::string& t)
		: action(a), text(t) {}

	bool operator==(const SResourcePath& other)
	{
		return text == other.text;
	}
};

typedef std::list<SResourcePath> _PathList;
typedef _PathList::iterator _PathIterator;

struct SLogData
{
	int revision;
	std::string author;
	std::string date;
	_PathList paths;
	std::string msg;

	SLogData()
		: revision(-1)
	{
		author.clear();
		date.clear();
		paths.clear();
		msg.clear();
	}
};

typedef std::list<SLogData> _LogDataList;
typedef _LogDataList::iterator _LogDataIterator;

struct SPatchData
{
	int revision;
	std::string path;

	SPatchData(int r, const std::string& p)
		: revision(r), path(p) {}

	// std::find()에서 SPatchData, std::string 비교를 위한 함수
	bool operator==(const std::string& string) const
	{
		return path == string;
	}
};

typedef std::list<SPatchData> _PatchDataList;
typedef _PatchDataList::iterator _PatchDataIterator;