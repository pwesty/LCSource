#include <stdio.h>
#include <time.h>

bool HexaDump4XTrap_Std(const char *szFileName, const void *Buf4Dump, const unsigned long BufSize, const char *szDescription)
{
	FILE				*fpDump		= 0;
	char				*pSrc		= (char*)Buf4Dump;
	time_t			t_Local	= 0;
	struct tm		*ptmLocal = {0,};
	char		szTemp[260] = {0,};

	fpDump = fopen(szFileName, "a+");

	if( fpDump == 0 )
		return false;

	t_Local = time(0);
	ptmLocal = localtime(&t_Local);

	sprintf(szTemp, "[%4d/%02d/%02d %02d:%02d:%02d] ", (ptmLocal->tm_year + 1900), (ptmLocal->tm_mon + 1), ptmLocal->tm_mday, ptmLocal->tm_hour, ptmLocal->tm_min, ptmLocal->tm_sec);

	fprintf(fpDump, szTemp);
	fprintf(fpDump, szDescription);
	fprintf(fpDump, " :\t");

	for( unsigned int uiLoop = 0; uiLoop < BufSize; uiLoop++ )
	{
		fprintf(fpDump, "%02X", pSrc[uiLoop] & 0xff);
	}

	fprintf(fpDump, "\n");

	fclose(fpDump);

	return true;
}

//