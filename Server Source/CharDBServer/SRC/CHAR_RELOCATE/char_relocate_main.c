#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>

#include <server_names.h>
#include <server_msgq.h>
#include <utillib.h>
#include <libhashshm.h>
#include <init_shm.h>

#include <server_sock.h>
#include <DBInfodef.h>
#include <querydef.h>
#include <msgdef.h>

extern st_HashedShmHandle stHandle;

extern int g_dMyQid, g_dSOCKQid;
extern int g_dSeq;
extern int JiSTOPFlag;
extern int g_dMySysNo;

extern void SetUpSignal();
extern int dInit_IPC();
extern void FinishProgram();
extern int dIsRcvedMessage( st_MsgQ* stMsgQ, int dQid );
extern int dSend_Message( st_MsgQ* stMsgQ, int dQid );
extern int dProc_Message( st_MsgQ* stMsgQ );
extern int dSwitch_Message( st_MsgQ* stMsgQ, pst_MYSQL pstMysql );
extern int dGet_Server_Connection_Info( pst_DBInfo pstDBInfo );
extern int mysqlConnection(pst_MYSQL charDB1, pst_MYSQL charDB2, pst_MYSQL dataDB, pst_DBInfo pstDBInfo);
extern int messagePHInit(pst_MsgQ pstMsgQ, int moveServer, int size, unsigned short res_or_req);
extern int mysqlQuery_newprojectData(pst_MYSQL pstMYSQL, char *queryString);
extern int mysqlQuery_newprojectDB(pst_MYSQL pstMYSQL, char *queryString);
extern int mysqlQuery_newprojectDB_2(pst_MYSQL pstMYSQL, char *queryString);

extern int t_character_map( pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint );
extern int t_inven_map( pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int *subsize );
extern int t_pet_map( pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int *subsize );
extern int t_apet_map( pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int *subsize );
extern int t_pet_name_map( pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int *subsize );
extern int t_apets_inven_map( pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int *subsize );
extern int t_assist_abstime_map( pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int *subsize );
extern int t_questdata_map( pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int *subsize );
extern int t_title_map( pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int *subsize );
extern int t_affinity_keep_map( pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int *subsize );
extern int t_affinity_reward_step_map( pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int *subsize );
extern int t_attendance_exp_system_map( pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint);
extern int response_msg(pst_MYSQL pst, int moveServer, int charIndex, unsigned char* szBody, int arrayPoint);


char*	version = "R1.0.0";


int main()
{
	int		iRet, dFlag = 0;
	time_t		oldtime = 0;//time( NULL );
	int arrayPoint = 0;
	int subsize = 0;

	st_DBInfo 	stDBInfo;
	st_MYSQL 	stCharDB1, stCharDB2, stDataDB;

	st_MsgQ stMsgQ;

	Init_logdebug( SEQ_PROC_CHAR_RELOCATE, getpid(), "CHAR_RELOCATE", "/home/SERVERAPP/LOG" );

	iRet = Init_shm_common();
	if(iRet < 0 ){
		dAppLog( LOG_CRI,"MAIN : Failed in Init_shm_common iRet[%d][%s]",
				iRet, strerror(errno));
		FinishProgram();
	}

	iRet = set_version(SEQ_PROC_CHAR_RELOCATE, version);
	if( iRet < 0 )
		FinishProgram();


	SetUpSignal();

	iRet = dInit_IPC();
	if ( iRet < 0 )
		FinishProgram();

	iRet = dGet_Server_Connection_Info(&stDBInfo);
	if ( iRet < 0 )
		FinishProgram();

	iRet = mysqlConnection(&stCharDB1, &stCharDB2, &stDataDB, &stDBInfo);
	if ( iRet < 0 )
		FinishProgram();

	iRet = set_status_running(SEQ_PROC_CHAR_RELOCATE);
	if(iRet < 0)
		FinishProgram();



	dAppLog( LOG_CRI, "#### CHAR_RELOCATE PROCESS START!!! ####" );

	while ( JiSTOPFlag ){
		time_t newTime = time(NULL); 
		struct tm* nowtm = localtime(&newTime);

		if( newTime - oldtime >= 180 ){ 
//		if( nowtm->tm_hour == 15 && nowtm->tm_min == 0 && nowtm->tm_sec == 10 )
			dFlag ++;
		}

		if( dFlag ) {
			st_MsgQ stMsgQ;
			memset(&stMsgQ, 0x00, sizeof(stMsgQ));

			iRet = mysqlQuery_newprojectDB_2(&stCharDB1, SELECT_ENABLE_0_FROM_t_server_trans);
			if(iRet < 0)
				continue;

			while((stCharDB1.row = mysql_fetch_row(stCharDB1.res)) != NULL){
				int moveServer = atoi(stCharDB1.row[1]);
				int charIndex = atoi(stCharDB1.row[0]);
				fprintf(stderr, " moveServer : %d,  charIndex : %d\n", moveServer, charIndex);

				arrayPoint = sizeof(st_PktHdr);
				response_msg(&stCharDB2, moveServer, charIndex, stMsgQ.szBody, arrayPoint);

				arrayPoint += sizeof(st_Response_Msg);
				t_character_map(&stCharDB2, stMsgQ.szBody, charIndex, arrayPoint);

				arrayPoint += sizeof(st_Character);
				t_inven_map(&stCharDB2, stMsgQ.szBody, charIndex, arrayPoint, &subsize);

				arrayPoint += (sizeof(st_Inven) * subsize) + sizeof(int);
				t_pet_map(&stCharDB2, stMsgQ.szBody, charIndex, arrayPoint, &subsize);

				arrayPoint += (sizeof(st_Pet) * subsize) + sizeof(int);
				t_apet_map(&stCharDB2, stMsgQ.szBody, charIndex, arrayPoint, &subsize);

				arrayPoint += (sizeof(st_Apets) * subsize) + sizeof(int);
				t_pet_name_map(&stCharDB2, stMsgQ.szBody, charIndex, arrayPoint, &subsize);

				arrayPoint += (sizeof(st_Pet_Name) * subsize) + sizeof(int);
				t_apets_inven_map(&stCharDB2, stMsgQ.szBody, charIndex, arrayPoint, &subsize);

				arrayPoint += (sizeof(st_Apets_Inven) * subsize) + sizeof(int);
				t_assist_abstime_map(&stCharDB2, stMsgQ.szBody, charIndex, arrayPoint, &subsize);

				arrayPoint += (sizeof(st_Assist_Abstime) * subsize) + sizeof(int);
				t_questdata_map(&stCharDB2, stMsgQ.szBody, charIndex, arrayPoint, &subsize);

				arrayPoint += (sizeof(st_Questdata) * subsize) + sizeof(int);
				t_title_map(&stCharDB2, stMsgQ.szBody, charIndex, arrayPoint, &subsize);

				arrayPoint += (sizeof(st_Title) * subsize) + sizeof(int);
				t_affinity_keep_map(&stCharDB2, stMsgQ.szBody, charIndex, arrayPoint, &subsize);

				arrayPoint += (sizeof(st_Affinity_Keep) * subsize) + sizeof(int);
				t_affinity_reward_step_map(&stCharDB2, stMsgQ.szBody, charIndex, arrayPoint, &subsize);

				arrayPoint += (sizeof(st_Affinity_Reward_Step) * subsize) + sizeof(int);
				t_attendance_exp_system_map(&stCharDB2, stMsgQ.szBody, charIndex, arrayPoint);

				messagePHInit(&stMsgQ, moveServer, arrayPoint+sizeof(st_Attendance_Exp_System), INSERT_DATA_REQ);	//메시지 헤더
				dSend_Message(&stMsgQ, g_dSOCKQid);
				dAppLog( LOG_CRI, "SEND MESSAGE........CHARACTERINDEX : %d",charIndex);
			}
			oldtime = newTime;
			usleep(100000);
		}
		dFlag = 0;

		iRet = dIsRcvedMessage( &stMsgQ, g_dMyQid );
		if ( iRet > 0 ){
			dAppLog( LOG_CRI, "Rcv MESSAGE........");
			iRet = dSwitch_Message( &stMsgQ, &stCharDB2 );
		}
		else
			usleep( USLEEP_TIME );
		usleep(1000000);
	}
	FinishProgram();
	return 0;
}

 
