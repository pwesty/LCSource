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
#include <typedef.h>
#include <msgdef.h>
#include <server_msgq.h>
#include <DBInfodef.h>

#include <stdio.h>

extern int g_dSOCKQid;
extern int insert_Data_t_Character( pst_MYSQL pst, unsigned char* szBody, int arrayPoint, int* charIndex );
extern int insert_Data_t_Inven( pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint);
extern int insert_Data_t_Pet(pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int *old_pet_Index);
extern int insert_Data_t_Apets(pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int *old_apet_Index);
extern int insert_Data_t_Pet_Name(pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int pet_index);
extern int insert_Data_t_Apets_Inven(pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int apet_index);
extern int insert_Data_t_Assist_Abstime(pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint);
extern int insert_Data_t_QuestData(pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint);
extern int insert_Data_t_Title(pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint);
extern int insert_Data_t_Affinity_Keep(pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint);
extern int insert_Data_t_Affinity_Reward_Step(pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint);
extern int insert_Data_t_Attendance_Exp_System(pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint);
extern int check_Data_CharacterSlot(pst_MYSQL pst, unsigned char* szBody, int userIndex);

extern int dSend_Message( pst_MsgQ pstMsgQ, int qid );
extern int get_subsize(unsigned char* szBody, int* arrayPoint);
extern int messagePHInit( pst_MsgQ pstMsgQ, int targetServerNo, int size, unsigned short req_or_res );
extern int select_Data_Apets(pst_MYSQL pst, int* apetIndex, int charIndex);
extern int select_Data_Pet(pst_MYSQL pst, int* petIndex, int charIndex);
extern int update_Inven_Petdata(pst_MYSQL pst, int charIndex, int* pet_Index, int* old_pet_Index, int subsize);
extern int update_Inven_Apetdata(pst_MYSQL pst, int charIndex, int* apet_Index, int* old_apet_Index, int subsize);
extern int rollback_databases(pst_MYSQL pst, int charIndex, int* apetIndex);
extern int set_serverTransState(pst_MYSQL pst, int state, int charIndex, int errorCode, int enable);


int dSwitch_Message( pst_MsgQ pstMsgQ, pst_MYSQL pstMysql )
{
	//st_Character stCharacter;
	//int		iRet;
	USHORT	usSvcID, usMsgID, usType;
	st_Response_Msg stResponseMsg;
	int charIndex;
	int pet_Index[100] = {'0', };
	int apet_Index[50] = {'0', };
	int old_pet_Index[100] = {'0', };
	int old_apet_Index[50] = {'0', };
	int errorCode;
	int subsize;
	int i = 0;

	pst_PktHdr	pstPH;

	usSvcID = pstMsgQ->usSvcID;
	usMsgID = pstMsgQ->usMsgID;
	usType = pstMsgQ->usType;

	pstPH = (pst_PktHdr)pstMsgQ->szBody;
	memset(&stResponseMsg, 0x00, sizeof(stResponseMsg));

	switch(usSvcID) {
		case SID_RELOCATE:
			switch(usMsgID) {
				case INSERT_DATA_REQ:
					memcpy(&stResponseMsg, pstMsgQ->szBody + sizeof(st_PktHdr), sizeof(stResponseMsg));
					messagePHInit(pstMsgQ, stResponseMsg.currentServer, sizeof(stResponseMsg), INSERT_DATA_RES);
					int arrayPoint = sizeof(stResponseMsg) + sizeof(st_PktHdr);

					//캐릭터 슬롯 여분 체크
					if((errorCode = (check_Data_CharacterSlot(pstMysql, pstMsgQ->szBody, stResponseMsg.userIndex))) != 0)
					{
						stResponseMsg.isSuccess = 0;
						stResponseMsg.errorCode = errorCode;
						strcpy(stResponseMsg.errorMsg, "(Error) Not enough Character Slot......");
						memcpy(pstMsgQ->szBody + sizeof(st_PktHdr), &stResponseMsg, sizeof(stResponseMsg));
						dSend_Message( pstMsgQ, g_dSOCKQid );
						break;
					}

					if(insert_Data_t_Character(pstMysql, pstMsgQ->szBody, arrayPoint, &charIndex) == -1){
						dAppLog(LOG_CRI, "(DB INSERT ERROR) Insert_Data_t_Character error.........");
						stResponseMsg.isSuccess = 0;
						stResponseMsg.errorCode = 804;
						strcpy(stResponseMsg.errorMsg, "(DB INSERT ERROR) Insert_Data_t_Character error.........");
						memcpy(pstMsgQ->szBody + sizeof(st_PktHdr), &stResponseMsg, sizeof(stResponseMsg));
						dSend_Message( pstMsgQ, g_dSOCKQid );
						break;
					}
					arrayPoint += sizeof(st_Character);
					subsize = get_subsize(pstMsgQ->szBody, &arrayPoint);
					for(i=0; i<subsize; i++)
					{
						if(insert_Data_t_Inven(pstMysql, pstMsgQ->szBody, charIndex, arrayPoint) == -1){
							dAppLog(LOG_CRI, "((DB INSERT ERROR) Insert_Data_t_Inven error.........");
							rollback_databases(pstMysql, charIndex, apet_Index);
							stResponseMsg.isSuccess = 0;
							stResponseMsg.errorCode = 804;
							strcpy(stResponseMsg.errorMsg, "(DB INSERT ERROR) Insert_Data_t_Inven error.........");
							memcpy(pstMsgQ->szBody + sizeof(st_PktHdr), &stResponseMsg, sizeof(stResponseMsg));
							dSend_Message( pstMsgQ, g_dSOCKQid );
							break;
						}
						arrayPoint += sizeof(st_Inven);
					}
					
					subsize = get_subsize(pstMsgQ->szBody, &arrayPoint);
					for(i=0; i<subsize; i++)
					{
						if(insert_Data_t_Pet(pstMysql, pstMsgQ->szBody, charIndex, arrayPoint, &old_pet_Index[i]) == -1){
							dAppLog(LOG_CRI, "((DB INSERT ERROR) Insert_Data_t_Pet error.........");
							rollback_databases(pstMysql, charIndex, apet_Index);
							stResponseMsg.isSuccess = 0;
							stResponseMsg.errorCode = 804;
							strcpy(stResponseMsg.errorMsg, "(DB INSERT ERROR) Insert_Data_t_Pet error.........");

							memcpy(pstMsgQ->szBody + sizeof(st_PktHdr), &stResponseMsg, sizeof(stResponseMsg));
							dSend_Message( pstMsgQ, g_dSOCKQid );
							break;
						}
						arrayPoint += sizeof(st_Pet);
					}

					select_Data_Pet(pstMysql, pet_Index, charIndex);
					update_Inven_Petdata(pstMysql, charIndex, pet_Index, old_pet_Index, subsize);

					subsize = get_subsize(pstMsgQ->szBody, &arrayPoint);
					for(i=0; i<subsize; i++)
					{
						if(insert_Data_t_Apets(pstMysql, pstMsgQ->szBody, charIndex, arrayPoint, &old_apet_Index[i]) == -1){
							dAppLog(LOG_CRI, "((DB INSERT ERROR) Insert_Data_t_Apets error.........");
							rollback_databases(pstMysql, charIndex, apet_Index);
							stResponseMsg.isSuccess = 0;
							stResponseMsg.errorCode = 804;
							strcpy(stResponseMsg.errorMsg, "(DB INSERT ERROR) Insert_Data_t_Apets error.........");
							memcpy(pstMsgQ->szBody + sizeof(st_PktHdr), &stResponseMsg, sizeof(stResponseMsg));
							dSend_Message( pstMsgQ, g_dSOCKQid );
							break;
						}
						arrayPoint += sizeof(st_Apets);
					}
					select_Data_Apets(pstMysql, apet_Index, charIndex);
					update_Inven_Apetdata(pstMysql, charIndex, apet_Index, old_apet_Index, subsize);

					subsize = get_subsize(pstMsgQ->szBody, &arrayPoint);
					for(i=0; i<subsize; i++)
					{
						if(insert_Data_t_Pet_Name(pstMysql, pstMsgQ->szBody, charIndex, arrayPoint, pet_Index[i]) == -1){
							dAppLog(LOG_CRI, "((DB INSERT ERROR) Insert_Data_t_pet_name error.........");
							rollback_databases(pstMysql, charIndex, apet_Index);
							stResponseMsg.isSuccess = 0;
							stResponseMsg.errorCode = 804;
							strcpy(stResponseMsg.errorMsg, "(DB INSERT ERROR) Insert_Data_t_pet_name error.........");
							memcpy(pstMsgQ->szBody + sizeof(st_PktHdr), &stResponseMsg, sizeof(stResponseMsg));
							dSend_Message( pstMsgQ, g_dSOCKQid );
							break;
						}
						arrayPoint += sizeof(st_Pet_Name);
					}

					subsize = get_subsize(pstMsgQ->szBody, &arrayPoint);
					for(i=0; i<subsize; i++)
					{
						if(insert_Data_t_Apets_Inven(pstMysql, pstMsgQ->szBody, charIndex, arrayPoint, apet_Index[i]) == -1){
							dAppLog(LOG_CRI, "((DB INSERT ERROR) Insert_Data_t_Apets_Inven error.........");
							rollback_databases(pstMysql, charIndex, apet_Index);
							stResponseMsg.isSuccess = 0;
							stResponseMsg.errorCode = 804;
							strcpy(stResponseMsg.errorMsg, "(DB INSERT ERROR) Insert_Data_t_Apets_Inven error.........");
							memcpy(pstMsgQ->szBody + sizeof(st_PktHdr), &stResponseMsg, sizeof(stResponseMsg));
							dSend_Message( pstMsgQ, g_dSOCKQid );
							break;
						}
						arrayPoint += sizeof(st_Apets_Inven);
					}

					subsize = get_subsize(pstMsgQ->szBody, &arrayPoint);
					for(i=0; i<subsize; i++)
					{
						if(insert_Data_t_Assist_Abstime(pstMysql, pstMsgQ->szBody, charIndex, arrayPoint) == -1){
							dAppLog(LOG_CRI, "((DB INSERT ERROR) Insert_Data_t_Assist_Abstime error.........");
							rollback_databases(pstMysql, charIndex, apet_Index);
							stResponseMsg.isSuccess = 0;
							stResponseMsg.errorCode = 804;
							strcpy(stResponseMsg.errorMsg, "(DB INSERT ERROR) Insert_Data_t_Assist_Abstime error.........");
							memcpy(pstMsgQ->szBody + sizeof(st_PktHdr), &stResponseMsg, sizeof(stResponseMsg));
							dSend_Message( pstMsgQ, g_dSOCKQid );
							break;
						}
						arrayPoint += sizeof(st_Assist_Abstime);
					}

					subsize = get_subsize(pstMsgQ->szBody, &arrayPoint);
					for(i=0; i<subsize; i++)
					{
						if(insert_Data_t_QuestData(pstMysql, pstMsgQ->szBody, charIndex, arrayPoint) == -1){
							dAppLog(LOG_CRI, "((DB INSERT ERROR) Insert_Data_t_QuestData error.........");
							rollback_databases(pstMysql, charIndex, apet_Index);
							stResponseMsg.isSuccess = 0;	
							stResponseMsg.errorCode = 804;
							strcpy(stResponseMsg.errorMsg, "(DB INSERT ERROR) Insert_Data_t_QuestData error.........");
							memcpy(pstMsgQ->szBody + sizeof(st_PktHdr), &stResponseMsg, sizeof(stResponseMsg));
							dSend_Message( pstMsgQ, g_dSOCKQid );
							break;
						}
						arrayPoint += sizeof(st_Questdata);
					}

					subsize = get_subsize(pstMsgQ->szBody, &arrayPoint);
					for(i=0; i<subsize; i++)
					{
						if(insert_Data_t_Title(pstMysql, pstMsgQ->szBody,  charIndex,  arrayPoint) == -1){
							dAppLog(LOG_CRI, "((DB INSERT ERROR) Insert_Data_t_titlelist error.........");
							rollback_databases(pstMysql, charIndex, apet_Index);
							stResponseMsg.isSuccess = 0;
							stResponseMsg.errorCode = 804;
							strcpy(stResponseMsg.errorMsg, "(DB INSERT ERROR) Insert_Data_t_titlelist error.........");
							memcpy(pstMsgQ->szBody + sizeof(st_PktHdr), &stResponseMsg, sizeof(stResponseMsg));
							dSend_Message( pstMsgQ, g_dSOCKQid );
							break;
						}
						arrayPoint += sizeof(st_Title);
					}

					subsize = get_subsize(pstMsgQ->szBody, &arrayPoint);
					for(i=0; i<subsize; i++)
					{
						if(insert_Data_t_Affinity_Keep(pstMysql, pstMsgQ->szBody,  charIndex, arrayPoint) == -1){
							dAppLog(LOG_CRI, "((DB INSERT ERROR) Insert_Data_t_Affinity_Keep error.........");
							rollback_databases(pstMysql, charIndex, apet_Index);
							stResponseMsg.isSuccess = 0;
							stResponseMsg.errorCode = 804;
							strcpy(stResponseMsg.errorMsg, "(DB INSERT ERROR) Insert_Data_t_Affinity_Keep error.........");
							memcpy(pstMsgQ->szBody + sizeof(st_PktHdr), &stResponseMsg, sizeof(stResponseMsg));
							dSend_Message( pstMsgQ, g_dSOCKQid );
							break;
						}
						arrayPoint += sizeof(st_Affinity_Keep);
					}

					subsize = get_subsize(pstMsgQ->szBody, &arrayPoint);
					for(i=0; i<subsize; i++)
					{
						if(insert_Data_t_Affinity_Reward_Step(pstMysql, pstMsgQ->szBody,  charIndex, arrayPoint) == -1){
							dAppLog(LOG_CRI, "((DB INSERT ERROR) Insert_Data_t_Affinity_Reward_Step error.........");
							rollback_databases(pstMysql, charIndex, apet_Index);
							stResponseMsg.isSuccess = 0;
							stResponseMsg.errorCode = 804;
							strcpy(stResponseMsg.errorMsg, "(DB INSERT ERROR) Insert_Data_t_Affinity_Reward_Step error.........");
							memcpy(pstMsgQ->szBody + sizeof(st_PktHdr), &stResponseMsg, sizeof(stResponseMsg));
							dSend_Message( pstMsgQ, g_dSOCKQid );
							break;
						}
						arrayPoint += sizeof(st_Affinity_Reward_Step);
					}

					if(insert_Data_t_Attendance_Exp_System(pstMysql, pstMsgQ->szBody,  charIndex, arrayPoint) == -1){
						dAppLog(LOG_CRI, "((DB INSERT ERROR) Insert_Data_t_Attendance_Exp_System error.........");
						rollback_databases(pstMysql, charIndex, apet_Index);
						stResponseMsg.isSuccess = 0;	
						stResponseMsg.errorCode = 804;
						strcpy(stResponseMsg.errorMsg, "(DB INSERT ERROR) Insert_Data_t_Attendance_Exp_System error.........");
						memcpy(pstMsgQ->szBody + sizeof(st_PktHdr), &stResponseMsg, sizeof(stResponseMsg));
						dSend_Message( pstMsgQ, g_dSOCKQid );
						break;
					}

					stResponseMsg.isSuccess = 1;
					memcpy(pstMsgQ->szBody + sizeof(st_PktHdr), &stResponseMsg, sizeof(stResponseMsg));
					memset(pstMsgQ->szBody + sizeof(st_PktHdr) + sizeof(stResponseMsg), 0x00, sizeof(pstMsgQ->szBody) - sizeof(st_PktHdr) - sizeof(stResponseMsg));
					dSend_Message( pstMsgQ, g_dSOCKQid );
					break;


				case INSERT_DATA_RES:
					memcpy(&stResponseMsg, pstMsgQ->szBody + sizeof(st_PktHdr), sizeof(stResponseMsg));
					fprintf(stderr, "isSuccess : %d moveServer : %d\n", stResponseMsg.isSuccess, stResponseMsg.moveServer);
					if(stResponseMsg.isSuccess == 1){
						set_serverTransState(pstMysql, 1, stResponseMsg.charIndex, 0, 0);
						time_t now;
						struct tm t;
						time(&now);
						t = *localtime(&now);
						dAppLog( LOG_CRI,
								" INSERT DATA SUCCESS USER_INDEX = %d, CurrentServer = %d, MoveServer = %d, FinishTime = %d-%d-%d %d:%d:%d",
								stResponseMsg.userIndex, stResponseMsg.currentServer, stResponseMsg.moveServer, t.tm_year+1900, t.tm_mon+1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec );
					}
					if(stResponseMsg.isSuccess == 0){
						set_serverTransState(pstMysql, 2, stResponseMsg.charIndex, stResponseMsg.errorCode, 3);
						dAppLog( LOG_CRI,
								" (ERROR) INSERT DATA ERROR....USER_INDEX = %d, CurrentServer = %d, MoveServer = %d, ErrorCode = %d, ErrorMsg = %s",
								stResponseMsg.userIndex, stResponseMsg.currentServer, stResponseMsg.moveServer, stResponseMsg.errorCode, stResponseMsg.errorMsg );
					}
					break;
				default:
					break;
			}
			break;
		default:
			dAppLog( LOG_CRI, "INVALID SERVICE ID RECEIVED SID:0x%x MID:0x%x",
					usSvcID, usMsgID );
			break;
	}
	return 0;
}
