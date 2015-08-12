#include "CheckPassword.h"

#ifndef WIN32

#include "md5.h"
#include <botan/botan.h>
#include <botan/bcrypt.h>
bool CCheckPassword::checkPass(const std::string& pass, const std::string& dbpass)
{
	//if(crypted) return Botan::check_bcrypt(pass, dbpass);
	//else		return (pass.compare(dbpass) == 0 ? true : false );
	int crypttype = kPlainText;
	if		(dbpass.size() == 60)	crypttype = kBcryptedText;
	else if	(dbpass.size() == 32)	crypttype = kMD5Text;

	switch(crypttype)
	{
	case kPlainText:
		{
			return (pass.compare(dbpass) == 0 ? true : false );
		}
		break;

	case kMD5Text:
		{
			char tmpPass[1024];
			char tmpResult[256];
			md5_buffer(pass.c_str(), pass.size(), tmpPass);
			md5_sig_to_string(tmpPass, tmpResult, sizeof(tmpResult));
			std::string md5String(tmpResult);
			std::transform(md5String.begin(), md5String.end(), md5String.begin(), toupper);
			std::string dbpassUpper(dbpass);
			std::transform(dbpassUpper.begin(), dbpassUpper.end(), dbpassUpper.begin(), toupper);
			return (md5String.compare(dbpassUpper) == 0 ? true : false);
		}
		break;

	case kBcryptedText:
		{
			return Botan::check_bcrypt(pass, dbpass);
		}
		break;

	default:
		return false;
		break;
	}
	return false;
}

#else

bool CCheckPassword::checkPass(const std::string& pass, const std::string& dbpass)
{
	return true;
}

#endif