
#include "StdH.h"
#include "DefineHelp.h"
#include <Engine/GlobalDefinition.h>

CTString DefHelp::getNationPostfix( int nNation, bool bFolder )
{
	CTString strRet;

	if (bFolder == false)
		strRet = "_";

	switch (nNation)
	{
	case KOREA:			strRet += "kr"; break;
	case GERMANY:		strRet += "de";	break;
	case SPAIN:			strRet += "es";	break;
	case FRANCE:		strRet += "fr";	break;
	case ITALY:			strRet += "it";	break;
	case POLAND:		strRet += "pl";	break;
	case ENGLAND:		strRet += "uk"; break;
	case USA:			strRet += "us";	break;

	case BRAZIL:		strRet += "br";	break;
	case MEXICO:		strRet += "mx";	break;

	case RUSSIA:		strRet += "ru";	break;

	case THAILAND:		strRet += "th";	break;
	default:
		break;
	}

	if (bFolder == false && nNation == KOREA)
		strRet = "";

	return strRet;
}
