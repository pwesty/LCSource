#ifndef __CHECK_PASSWORD_H__
#define __CHECK_PASSWORD_H__

#include <string>

class CCheckPassword
{
public:
	enum
	{
		kPlainText = 0,
		kMD5Text = 1,
		kBcryptedText = 2
	};
	static bool checkPass(const std::string& pass, const std::string& dbpass);
};

#endif
