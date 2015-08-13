#ifndef __COMMON_UTIL_H__
#define __COMMON_UTIL_H__

#ifdef WIN32
#pragma
#endif

#include <vector>
#include <boost/tokenizer.hpp>

inline void makeSocketFromStringToArray(std::string ori, std::vector<int>& vec)
{
	vec.resize(MAX_SOCKET_COUNT);
	std::fill(vec.begin(), vec.end(), -1);
	if (ori.empty())
	{
		return;
	}

	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sep(" ");
	tokenizer tok(ori, sep);
	std::vector<std::string> strvec;
	strvec.assign(tok.begin(), tok.end());

	vec[0] = atoi(strvec[0].c_str());

	for (int i = 2; i < strvec.size(); ++i)
	{
		vec[i - 1] = atoi(strvec[i].c_str());
	}
}

#endif
