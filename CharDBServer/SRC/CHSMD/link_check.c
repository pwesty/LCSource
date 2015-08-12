/**********************************************************
            ABLEX DTAS Project
    Author      :   tundra
    Section     :   LDGS LOAD_STAT
    SCCS ID     :   %W%
    DATE        :   %G%

    Revision History :
        02. 01. 24      Initial

    Description:

    Copyright (C) ABLEX 2002
***********************************************************/

/*
**  Physical memory status monitoring module.
**  2000.5 Sangwook Kim, ABLEX.
*/


#include <chsmd_api.h>
#include <chsmd_extern.h>
#include <utillib.h>


char szLink[] = "Link detected: yes";
char szSpeed[] = "Speed: 1000Mb/s";
char szDuplex[] = "Duplex: Full";

int Link_Check()
{
    int 			dRet;
    st_AlmStat  	stAlm;
    int 			i;
	int				dUp;

	//dAppLog( LOG_INFO, "### LINK INFO ###" );
	for(i = 0; i < dNet; i++)
	{
		dUp = 0;

		dRet = dEthCheck(szNet[i]);
		if(dRet == 111)
			dUp = 1;

    	if(dUp == 1)
    	{
        	if(net[i] != ALIVE)
            {
            	stAlm.ucLocType = HW_ETH_IDX;
            	stAlm.ucLocIdx = i;
            	stAlm.ucStatus = ALIVE;
            	stAlm.tWhen = time(NULL);

            	dAppLog(LOG_INFO,
				"time=[%ld],ucLocType=[%d],ucLocIdx=[%d]",
            	stAlm.tWhen,stAlm.ucLocType,stAlm.ucLocIdx);

				dRet = FIDB_Handle_Proc( &stAlm );
				if( dRet < 0 ){
					dAppLog( LOG_CRI, "Failed in FIDB_Handle_Proc (%d:%s)",dRet, strerror(errno) );
				}
            	net[i] = ALIVE;
            }
    	}
    	else
    	{
			dAppLog(LOG_CRI, "LINK : NET NAME[%s] STS[DOWN]", szNet[i]);
        	if(net[i] != DEAD)
            {
            	stAlm.ucLocType = HW_ETH_IDX;
            	stAlm.ucLocIdx = i;
            	stAlm.ucStatus = DEAD;
            	stAlm.tWhen = time(NULL);

            	dAppLog(LOG_INFO,
				"LINK : time=[%ld],ucLocType=[%d],ucLocIdx=[%d]",
            	stAlm.tWhen,stAlm.ucLocType,stAlm.ucLocIdx);

				dRet = FIDB_Handle_Proc( &stAlm );
				if( dRet < 0 ){
					dAppLog( LOG_CRI, "LINK : Failed in FIDB_Handle_Proc (%d:%s)",dRet, strerror(errno) );
					return -1;
				}
            	net[i] = DEAD;
            }
    	}

	}
    return 0;
}




int dEthCheck(char *szNetStr)
{
	FILE		*fp = NULL;
    	char            szBuf[256];
    	char            szTmp[1024];
	char		*szPnt;
    	int             dRet;
	int		dLen;

	dRet = 0;

	memset(szTmp, 0, 1024);
	sprintf(szTmp, "/sbin/ethtool %s", szNetStr);


	fflush(NULL);
	fp = popen((const char *)(szTmp), "r");

	if(fp == NULL)
	{
		dAppLog(LOG_CRI, "dEthCheck : %s failed in popen [%s]",
		szTmp, strerror(errno));
		return -1;
	}

	while(fgets(szBuf, 256, fp) != NULL)
	{
		szPnt = strstr(szBuf, "Speed");
		if(szPnt != NULL)
		{
			dLen = strlen(szSpeed);
            if(strncmp(szPnt, szSpeed, dLen) == 0)
                dRet += 1;
			else
				dAppLog(LOG_CRI, "Speed[%s] [%s]", szPnt, szNetStr);
		}

        szPnt = strstr(szBuf, "Duplex");
        if(szPnt != NULL)
		{
			dLen = strlen(szDuplex);
            if(strncmp(szPnt, szDuplex, dLen) == 0)
                dRet += 10;
			else
				dAppLog(LOG_CRI, "Duplex[%s] [%s]", szPnt, szNetStr);
        }

        szPnt = strstr(szBuf, "Link");
        if(szPnt != NULL)
        {
			dLen = strlen(szLink);
            if(strncmp(szPnt, szLink, dLen) == 0)
                dRet += 100;
			else
				dAppLog(LOG_CRI, "Link[%s] [%s]", szPnt, szNetStr);
        }
	}

	pclose(fp);

	return dRet;
}
