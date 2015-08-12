#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <errno.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <utillib.h>
#include <server_sock.h>
#include <server_define.h>
#include <server_shm.h>
#include <server_names.h>
#include <init_shm.h>

#include <DBInfodef.h>


int JiSTOPFlag, Finishflag;

int g_dMyQid, g_dSOCKQid;

int g_dMySysNo;

void UserControlledSignal( int sign )
{
	JiSTOPFlag = 0;
	Finishflag = sign;
}				/* end of UserControlledSignal */

void FinishProgram()
{
	dAppLog( LOG_CRI,
			"FinishProgram : PROGRAM IS NORMALLY TERMINATED, Cause = %d",
			Finishflag );
	exit( 0 );
}				/* end of FinishProgram */

void IgnoreSignal( int sign )
{
	if ( sign != SIGALRM )
		dAppLog( LOG_CRI,
				"IgnoreSignal : UNWANTED SIGNAL IS RECEIVED, signal = %d",
				sign );
	signal( sign, IgnoreSignal );
}				/* end of IgnoreSignal */


void SetUpSignal()
{
	JiSTOPFlag = 1;

	/* WANTED SIGNALS   */
	signal( SIGTERM, UserControlledSignal );
	signal( SIGINT, UserControlledSignal );
	signal( SIGQUIT, UserControlledSignal );

	/* UNWANTED SIGNALS */
	signal( SIGHUP, IgnoreSignal );
	signal( SIGALRM, IgnoreSignal );
	signal( SIGPIPE, IgnoreSignal );
	signal( SIGPOLL, IgnoreSignal );
	signal( SIGPROF, IgnoreSignal );
	signal( SIGUSR1, IgnoreSignal );
	signal( SIGUSR2, IgnoreSignal );
	signal( SIGVTALRM, IgnoreSignal );
	signal( SIGCLD, SIG_IGN );

	dAppLog( LOG_DEBUG, "SetUpSignal : SIGNAL HANDLER WAS INSTALLED[%d]",
			JiSTOPFlag );

	return;
}				/* end of SetUpSignal */

int dInit_IPC()
{
	/*** My Queue ***/
	if((g_dSOCKQid = msgget(S_MSGQ_CDB_STSOC, 0666|IPC_CREAT)) < 0) {
		dAppLog(LOG_CRI, "[init_ipcs] [FAIL:%d] MSGGET S_MSGQ_GS_MESSENGER : [%s]\n",
				errno, strerror(errno) );
		return -1;
	}

	if((g_dMyQid = msgget(S_MSGQ_CHAR_RELOCATE, 0666|IPC_CREAT)) < 0) {
		dAppLog(LOG_CRI, "[init_ipcs] [FAIL:%d] MSGGET S_MSGQ_GS_MESSENGER : [%s]\n",
				errno, strerror(errno) );
		return -1;
	}

	return 0;
}


int dGet_Server_Connection_Info(pst_DBInfo pstDBInfo)
{
	FILE    *fa;
	int     i = 0, dSysNo;
	char    szBuf[1024] ={'0', };
	char 	userID[24]={'0', };
	char 	password[24]={'0', };
	char 	ip[30]={'0', };
	char 	db_database[30]={'0', };
	char 	data_database[30]={'0', };


	fa = fopen( IPADDR_FILE,"r" );
	if(fa == NULL) {
		dAppLog( LOG_CRI,"%s : FILE OPEN FAILED (%s)", 
				__func__, IPADDR_FILE, strerror(errno) );
		return -1; 
	}   

	while( fgets( szBuf, 1024, fa ) != NULL ) { 
		if( szBuf[0] != '#' ) { 
			dAppLog( LOG_CRI,"%s : %s Format Error",
					__func__, IPADDR_FILE );
			fclose( fa );
			return -2; 
		}   

		if( szBuf[1] == '#' ) {
			memset(szBuf, 0x00, sizeof(szBuf));
			continue;
		} else if( szBuf[1] == 'E' ) {
			i++;
			break;
		} else if( szBuf[1] == 'S' ) { 
			if(sscanf( &szBuf[2],"%d %s %s %s %s %s", &dSysNo, ip, userID, password, db_database, data_database) == 6 ) {    
				//memset(pstDBInfo, 0x00, sizeof(st_DBInfo));
				g_dMySysNo = dSysNo;
				memcpy(pstDBInfo->serverIP, ip, sizeof(ip));
				memcpy(pstDBInfo->userID, userID, sizeof(userID));
				memcpy(pstDBInfo->password, password, sizeof(password));
				memcpy(pstDBInfo->db_database, db_database, sizeof(db_database));
				memcpy(pstDBInfo->data_database, data_database, sizeof(data_database));
			}
			else
				dAppLog(LOG_CRI, "file read error data : S ....... %d %s %s %s %s %s", dSysNo, ip, userID, password, db_database, data_database);
		}
	}
	fclose(fa);

	if(!i)
		return -1;

	return 0;
}
