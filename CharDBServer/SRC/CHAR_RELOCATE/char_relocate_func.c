#include <stdio.h>
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
#include <libhashshm.h>
#include <server_msgq.h>
#include <DBInfodef.h>

#include <mysql.h>


extern int dSend_Message( pst_MsgQ pstMsgQ, int qid );
extern int g_dMySysNo;


int messagePHInit( pst_MsgQ pstMsgQ, int targetServerNo, int size, unsigned short req_or_res )
{
	st_PktHdr stPH;
	memset(&stPH, 0x00, sizeof(stPH));

	pstMsgQ->usType = DEF_SVC;
	pstMsgQ->usSvcID = SID_RELOCATE;
	pstMsgQ->usMsgID = req_or_res;
	pstMsgQ->usSystemNo = targetServerNo;
	pstMsgQ->usBodyLen = stPH.st1stPH.usTotLen = size+PACK_HEADER_LEN;

	stPH.st1stPH.usSerial = g_dMySysNo;
	stPH.st2ndPH.usSvcID = pstMsgQ->usSvcID;
	stPH.st2ndPH.usMsgID = pstMsgQ->usMsgID;

	memcpy(pstMsgQ->szBody, &stPH, sizeof(stPH));

	return 0;
}

int mysqlConnection(pst_MYSQL charDB1,pst_MYSQL charDB2,pst_MYSQL dataDB,pst_DBInfo pstDBInfo)
{
	char *server = pstDBInfo->serverIP;
	char *user = pstDBInfo->userID;
	char *password = pstDBInfo->password;
	char *database_db = pstDBInfo->db_database;
	char *database_data = pstDBInfo->data_database;

	//pstMYSQL->conn_db = mysql_init(NULL);
	//pstMYSQL->conn_data = mysql_init(NULL);
	//pstMYSQL->conn_db_2 = mysql_init(NULL);
	charDB1->conn = mysql_init(NULL);
	charDB2->conn = mysql_init(NULL);
	dataDB->conn = mysql_init(NULL);

	/*Connect to Database*/
	//if(!mysql_real_connect(pstMYSQL->conn_db, server, user, password, database_db, 0, NULL, 0)){
	if(!mysql_real_connect(charDB1->conn, server, user, password, database_db, 0, NULL, 0)){
		dAppLog(LOG_CRI, "%s \n", mysql_error(charDB1->conn));
		return -1;
	}
	if(!mysql_real_connect(charDB2->conn, server, user, password, database_db, 0, NULL, 0)){
		dAppLog(LOG_CRI, "%s \n", mysql_error(charDB2->conn));
		return -1;
	}
	if(!mysql_real_connect(dataDB->conn, server, user, password, database_data, 0, NULL, 0)){
		dAppLog(LOG_CRI, "%s \n", mysql_error(dataDB->conn));
		return -1;
	}
	return 0;
}

int mysqlQuery_newprojectDB_2(pst_MYSQL pstMYSQL, char *queryString)
{
	if(mysql_query(pstMYSQL->conn, queryString)){
		dAppLog(LOG_CRI, "%s \n", mysql_error(pstMYSQL->conn));
		return -1;
	}
	pstMYSQL->res = mysql_store_result(pstMYSQL->conn);
	return 0;
}

int mysqlQuery_newprojectDB(pst_MYSQL pstMYSQL, char *queryString)
{
	/*Send SQL query*/
	if(mysql_query(pstMYSQL->conn, queryString)){
		dAppLog(LOG_CRI, "%s \n", mysql_error(pstMYSQL->conn));
		return -1;
	}
	pstMYSQL->res = mysql_store_result(pstMYSQL->conn);
	return 0;
}
int mysqlQuery_newprojectData(pst_MYSQL pstMYSQL, char *queryString)
{
	if(mysql_query(pstMYSQL->conn, queryString)){
		dAppLog(LOG_CRI, "%s \n", mysql_error(pstMYSQL->conn));
		return -1;
	}
	pstMYSQL->res = mysql_store_result(pstMYSQL->conn);
	return 0;
}

/* response msg funtion */
int response_msg(pst_MYSQL pst, int moveServer, int charIndex, unsigned char* szBody, int arrayPoint  )
{
	st_Response_Msg stResponseMsg;
	memset(&stResponseMsg, 0x00, sizeof(st_Response_Msg));
	char query[1024] = {'0', };

	sprintf(query, "update t_server_trans set a_start_time = unix_timestamp(now())");
	mysqlQuery_newprojectDB(pst, query);

	sprintf(query, "select a_user_index from t_characters where a_index = %d", charIndex);
	mysqlQuery_newprojectDB(pst, query);
	pst->row = mysql_fetch_row(pst->res);

	stResponseMsg.userIndex = atoi(pst->row[0]);
	stResponseMsg.charIndex = charIndex;
	stResponseMsg.currentServer = g_dMySysNo;
	stResponseMsg.moveServer = moveServer;

	memcpy(szBody + arrayPoint, &stResponseMsg, sizeof(st_Response_Msg));

	mysql_free_result(pst->res);

	return 0;
}


/* select funtion */

int t_character_map( pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint )
{
	st_Character stCharacter;
	char query[1024] = {'0', };
	memset(&stCharacter, 0x00, sizeof(stCharacter));

	sprintf(query, "select * from t_characters where a_index = %d", charIndex);
	mysqlQuery_newprojectDB(pst, query);

	pst->row = mysql_fetch_row(pst->res);	

	stCharacter.a_user_index = atoi(pst->row[1]);
	stCharacter.a_server = atoi(pst->row[2]);
	strcpy(stCharacter.a_name, pst->row[3]);
	strcpy(stCharacter.a_nick, pst->row[4]);
	stCharacter.a_enable = atoi(pst->row[5]);
	strcpy(stCharacter.datestamp, pst->row[6]);
	stCharacter.a_lcdatestamp = atoi(pst->row[7]);
	strcpy(stCharacter.a_createdate, pst->row[8]);
	stCharacter.a_deletedelay = atoi(pst->row[9]);
	stCharacter.a_admin = atoi(pst->row[10]);
	stCharacter.a_flag = atoi(pst->row[11]);
	stCharacter.a_job = atoi(pst->row[12]);
	stCharacter.a_hair_style = atoi(pst->row[13]);
	stCharacter.a_face_style = atoi(pst->row[14]);
	stCharacter.a_level = atoi(pst->row[15]);
	stCharacter.a_exp = atoi(pst->row[16]);
	stCharacter.a_str = atoi(pst->row[17]);
	stCharacter.a_dex = atoi(pst->row[18]);
	stCharacter.a_int = atoi(pst->row[19]);
	stCharacter.a_con = atoi(pst->row[20]);
	stCharacter.a_hp = atoi(pst->row[21]);
	stCharacter.a_max_hp = atoi(pst->row[22]);
	stCharacter.a_mp = atoi(pst->row[23]);
	stCharacter.a_max_mp = atoi(pst->row[24]);
	stCharacter.a_statpt_remain = atoi(pst->row[25]);
	stCharacter.a_statpt_str = atoi(pst->row[26]);
	stCharacter.a_statpt_dex = atoi(pst->row[27]);
	stCharacter.a_statpt_con = atoi(pst->row[28]);
	stCharacter.a_statpt_int = atoi(pst->row[29]);
	stCharacter.a_skill_point = atoi(pst->row[30]);
	stCharacter.a_blood_point = atoi(pst->row[31]);
	strcpy(stCharacter.a_active_skill_index,pst->row[32]);
	strcpy(stCharacter.a_active_skill_level, pst->row[33]);
	strcpy(stCharacter.a_passive_skill_index, pst->row[34]);
	strcpy(stCharacter.a_passive_skill_level, pst->row[35]);
	strcpy(stCharacter.a_etc_skill_index, pst->row[36]);
	strcpy(stCharacter.a_etc_skill_level, pst->row[37]);
	strcpy(stCharacter.a_seal_skill_index, pst->row[38]);
	strcpy(stCharacter.a_seal_skill_exp, pst->row[39]);
	strcpy(stCharacter.a_quest_index, pst->row[40]);
	strcpy(stCharacter.a_quest_value, pst->row[41]);
	strcpy(stCharacter.a_quest_complete, pst->row[42]);
	strcpy(stCharacter.a_quest_abandon, pst->row[43]);
	stCharacter.a_was_x = atof(pst->row[44]);
	stCharacter.a_was_z = atof(pst->row[45]);
	stCharacter.a_was_h = atof(pst->row[46]);
	stCharacter.a_was_r = atof(pst->row[47]);
	stCharacter.a_was_yLayer = atoi(pst->row[48]);
	stCharacter.a_was_zone = atoi(pst->row[49]);
	stCharacter.a_was_area = atoi(pst->row[50]);
	strcpy(stCharacter.a_wearing, pst->row[51]);
	stCharacter.a_silence_pulse = atoi(pst->row[52]);
	strcpy(stCharacter.a_sskill, pst->row[53]);
	stCharacter.a_pkpenalty = atoi(pst->row[54]);
	stCharacter.a_pkcount = atoi(pst->row[55]);
	stCharacter.a_pkrecover = atoi(pst->row[56]);
	stCharacter.a_pkpenaltyhp = atoi(pst->row[57]);
	stCharacter.a_pkpenaltymp = atoi(pst->row[58]);
	stCharacter.a_guildoutdate = atoi(pst->row[59]);
	stCharacter.a_pluseffect_option = atoi(pst->row[60]);
	strcpy(stCharacter.a_teach_idx, pst->row[61]);
	strcpy(stCharacter.a_teach_sec, pst->row[62]);
	stCharacter.a_teach_type = atoi(pst->row[63]);
	stCharacter.a_fame = atoi(pst->row[64]);
	stCharacter.a_teach_list = atoi(pst->row[65]);
	stCharacter.a_teach_complete = atoi(pst->row[66]);
	stCharacter.a_teach_fail = atoi(pst->row[67]);
	stCharacter.a_use_sp = atoi(pst->row[68]);
	stCharacter.a_total_sp = atoi(pst->row[69]);
	stCharacter.a_loseexp = atol(pst->row[70]);
	stCharacter.a_losesp = atol(pst->row[71]);
	stCharacter.a_job2 = atoi(pst->row[72]);
	stCharacter.a_subjob = atoi(pst->row[73]);
	stCharacter.a_pd3time = atoi(pst->row[74]);
	stCharacter.a_superstone = atoi(pst->row[75]);
	stCharacter.a_guardian = atoi(pst->row[76]);
	stCharacter.a_etc_event = atoi(pst->row[77]);
//	stCharacter.a_index_old = atoi(pst->row[78]);
//	stCharacter.a_server_old = atoi(pst->row[79]);
	stCharacter.a_lastpktime = atoi(pst->row[78]);
	stCharacter.a_phoenix = atoi(pst->row[79]);
	stCharacter.a_title_index = atoi(pst->row[80]);
	stCharacter.a_newtuto_complete = atoi(pst->row[81]);
	stCharacter.a_exp_weekly = atoi(pst->row[82]);
	strcpy(stCharacter.a_levelup_date, pst->row[83]);
	stCharacter.a_cp = atoi(pst->row[84]);
	strcpy(stCharacter.a_cp_lstupdate, pst->row[85]);
	stCharacter.a_trans_time = atoi(pst->row[86]);

	memcpy(szBody+arrayPoint, &stCharacter, sizeof(stCharacter));

	mysql_free_result(pst->res);

	return 0;
}

int t_inven_map( pst_MYSQL pstMysql, unsigned char* szBody, int charIndex, int arrayPoint, int *subsize ){
	int i = 0;
	st_Inven stInven;
	char query[1024] = {'0', };

	sprintf(query, "select * from t_inven0%d where a_char_idx = %d",charIndex%10, charIndex);
	mysqlQuery_newprojectDB(pstMysql, query);
	*subsize = mysql_num_rows(pstMysql->res);
	memcpy(szBody + arrayPoint, subsize, sizeof(*subsize));

	for(i=0; i<*subsize; i++)
	{
		pstMysql->row = mysql_fetch_row(pstMysql->res);

		stInven.a_char_idx = atoi(pstMysql->row[0]);
		stInven.a_tab_idx = atoi(pstMysql->row[1]);
		stInven.a_row_idx = atoi(pstMysql->row[2]);
		stInven.a_item_idx0 = atoi(pstMysql->row[3]);
		stInven.a_plus0 = atoi(pstMysql->row[4]);
		stInven.a_wear_pos0 = atoi(pstMysql->row[5]);
		stInven.a_flag0 = atoi(pstMysql->row[6]);
		strcpy(stInven.a_serial0, pstMysql->row[7]);
		stInven.a_count0 = atoi(pstMysql->row[8]);
		stInven.a_used0 = atoi(pstMysql->row[9]);
		stInven.a_item_idx1 = atoi(pstMysql->row[10]);
		stInven.a_plus1 = atoi(pstMysql->row[11]);
		stInven.a_wear_pos1 = atoi(pstMysql->row[12]);
		stInven.a_flag1 = atoi(pstMysql->row[13]);
		strcpy(stInven.a_serial1, pstMysql->row[14]);
		stInven.a_count1 = atoi(pstMysql->row[15]);
		stInven.a_used1 = atoi(pstMysql->row[16]);
		stInven.a_item_idx2 = atoi(pstMysql->row[17]);
		stInven.a_plus2 = atoi(pstMysql->row[18]);
		stInven.a_wear_pos2 = atoi(pstMysql->row[19]);
		stInven.a_flag2 = atoi(pstMysql->row[20]);
		strcpy(stInven.a_serial2, pstMysql->row[21]);
		stInven.a_count2 = atoi(pstMysql->row[22]);
		stInven.a_used2 = atoi(pstMysql->row[23]);
		stInven.a_item_idx3 = atoi(pstMysql->row[24]);
		stInven.a_plus3 = atoi(pstMysql->row[25]);
		stInven.a_wear_pos3 = atoi(pstMysql->row[26]);
		stInven.a_flag3 = atoi(pstMysql->row[27]);
		strcpy(stInven.a_serial3, pstMysql->row[28]);
		stInven.a_count3 = atoi(pstMysql->row[29]);
		stInven.a_used3 = atoi(pstMysql->row[30]);
		stInven.a_item_idx4 = atoi(pstMysql->row[31]);
		stInven.a_plus4 = atoi(pstMysql->row[32]);
		stInven.a_wear_pos4 = atoi(pstMysql->row[33]);
		stInven.a_flag4 = atoi(pstMysql->row[34]);
		strcpy(stInven.a_serial4, pstMysql->row[35]);
		stInven.a_count4 = atoi(pstMysql->row[36]);
		stInven.a_used4 = atoi(pstMysql->row[37]);
		stInven.a_item0_option0 = atoi(pstMysql->row[38]);
		stInven.a_item0_option1 = atoi(pstMysql->row[39]);
		stInven.a_item0_option2 = atoi(pstMysql->row[40]);
		stInven.a_item0_option3 = atoi(pstMysql->row[41]);
		stInven.a_item0_option4 = atoi(pstMysql->row[42]);
		stInven.a_item1_option0 = atoi(pstMysql->row[43]);
		stInven.a_item1_option1 = atoi(pstMysql->row[44]);
		stInven.a_item1_option2 = atoi(pstMysql->row[45]);
		stInven.a_item1_option3 = atoi(pstMysql->row[46]);
		stInven.a_item1_option4 = atoi(pstMysql->row[47]);
		stInven.a_item2_option0 = atoi(pstMysql->row[48]);
		stInven.a_item2_option1 = atoi(pstMysql->row[49]);
		stInven.a_item2_option2 = atoi(pstMysql->row[50]);
		stInven.a_item2_option3 = atoi(pstMysql->row[51]);
		stInven.a_item2_option4 = atoi(pstMysql->row[52]);
		stInven.a_item3_option0 = atoi(pstMysql->row[53]);
		stInven.a_item3_option1 = atoi(pstMysql->row[54]);
		stInven.a_item3_option2 = atoi(pstMysql->row[55]);
		stInven.a_item3_option3 = atoi(pstMysql->row[56]);
		stInven.a_item3_option4 = atoi(pstMysql->row[57]);
		stInven.a_item4_option0 = atoi(pstMysql->row[58]);
		stInven.a_item4_option1 = atoi(pstMysql->row[59]);
		stInven.a_item4_option2 = atoi(pstMysql->row[60]);
		stInven.a_item4_option3 = atoi(pstMysql->row[61]);
		stInven.a_item4_option4 = atoi(pstMysql->row[62]);
		stInven.a_used0_2 = atoi(pstMysql->row[63]);
		stInven.a_used1_2 = atoi(pstMysql->row[64]);
		stInven.a_used2_2 = atoi(pstMysql->row[65]);
		stInven.a_used3_2 = atoi(pstMysql->row[66]);
		stInven.a_used4_2 = atoi(pstMysql->row[67]);
		strcpy(stInven.a_socket0, pstMysql->row[68]);
		strcpy(stInven.a_socket1, pstMysql->row[69]);
		strcpy(stInven.a_socket2, pstMysql->row[70]);
		strcpy(stInven.a_socket3, pstMysql->row[71]);
		strcpy(stInven.a_socket4, pstMysql->row[72]);

		memcpy(szBody + arrayPoint + sizeof(*subsize) + (sizeof(stInven) * i), &stInven, sizeof(stInven));
	}

	mysql_free_result(pstMysql->res);
	return 0;
}
int t_pet_map( pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int *subsize )
{
	int i = 0;
	st_Pet stPet;
	memset(&stPet, 0x00, sizeof(stPet));

	char query[1024] = {'0', };

	sprintf(query, "select * from t_pet where a_owner = %d", charIndex);
	mysqlQuery_newprojectDB(pst, query);
	*subsize = mysql_num_rows(pst->res);
	memcpy(szBody + arrayPoint, subsize, sizeof(*subsize));

	for(i=0; i<*subsize; i++)
	{
		pst->row = mysql_fetch_row(pst->res);

		stPet.a_index = atoi(pst->row[0]);
		stPet.a_owner = atoi(pst->row[1]);
		stPet.a_enable = atoi(pst->row[2]);
		stPet.a_lastupdate = atoi(pst->row[3]);
		stPet.a_type = atoi(pst->row[4]);
		stPet.a_level = atoi(pst->row[5]);
		stPet.a_hp = atoi(pst->row[6]);
		stPet.a_hungry = atoi(pst->row[7]);
		stPet.a_sympathy = atoi(pst->row[8]);
		stPet.a_exp = atoi(pst->row[9]);
		stPet.a_ability = atoi(pst->row[10]);
		strcpy(stPet.a_skill_index, pst->row[11]);
		strcpy(stPet.a_skill_level, pst->row[12]);
		stPet.a_time_rebirth = atoi(pst->row[13]);
		stPet.a_index_old = atoi(pst->row[14]);
		strcpy(stPet.a_color, pst->row[15]);
		stPet.a_pet_turnto_npc = atoi(pst->row[16]);
		stPet.a_pet_size = atoi(pst->row[17]);
		memcpy(szBody + arrayPoint + sizeof(*subsize) + (sizeof(stPet) * i), &stPet, sizeof(stPet));
	}
	mysql_free_result(pst->res);
	return 0;
}
int t_apet_map( pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int *subsize )
{
	int i = 0;
	st_Apets stApets;
	char query[1024] = {'0', };

	sprintf(query, "select * from t_apets where a_owner = %d", charIndex);
	mysqlQuery_newprojectDB(pst, query);
	*subsize = mysql_num_rows(pst->res);
	memcpy(szBody + arrayPoint, subsize, sizeof(*subsize));

	for(i=0; i<*subsize; i++)
	{
		pst->row = mysql_fetch_row(pst->res);

		stApets.a_index = atoi(pst->row[0]);
		stApets.a_owner = atoi(pst->row[1]);
		strcpy(stApets.a_enable, pst->row[2]);
		stApets.a_seal = atoi(pst->row[3]);
		stApets.a_proto_index = atoi(pst->row[4]);
		strcpy(stApets.a_name, pst->row[5]);
		strcpy(stApets.a_create_date, pst->row[6]);
		strcpy(stApets.a_lastupdate_date, pst->row[7]);
		stApets.a_level = atoi(pst->row[8]);
		stApets.a_exp = atoi(pst->row[9]);
		stApets.a_remain_stat = atoi(pst->row[10]);
		stApets.a_plus_str = atoi(pst->row[11]);
		stApets.a_plus_con = atoi(pst->row[12]);
		stApets.a_plus_dex = atoi(pst->row[13]);
		stApets.a_plus_int = atoi(pst->row[14]);
		stApets.a_skill_point = atoi(pst->row[15]);
		strcpy(stApets.a_skill_index, pst->row[16]);
		strcpy(stApets.a_skill_level, pst->row[17]);
		stApets.a_hp = atoi(pst->row[18]);
		stApets.a_mp = atoi(pst->row[19]);
		stApets.a_faith = atoi(pst->row[20]);
		stApets.a_stm = atoi(pst->row[21]);
		strcpy(stApets.a_ai_enable, pst->row[22]);
		stApets.a_ai_slot = atoi(pst->row[23]);
		stApets.a_accFaith = atoi(pst->row[24]);
		stApets.a_accStm = atoi(pst->row[25]);
		stApets.a_accExp = atoi(pst->row[26]);
		stApets.a_cooltime = atoi(pst->row[27]);
		memcpy(szBody + arrayPoint + sizeof(*subsize) + (sizeof(stApets) * i), &stApets, sizeof(stApets));
	}
	mysql_free_result(pst->res);
	return 0;
}
int t_pet_name_map( pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int *subsize )
{
	int i = 0;
	st_Pet_Name stPetName;
	char query[1024] = {'0', };

	sprintf(query, "select * from t_pet_name where a_pet_owner = %d", charIndex);
	mysqlQuery_newprojectDB(pst, query);
	*subsize = mysql_num_rows(pst->res);
	for(i=0; i<*subsize; i++)
	{
		pst->row = mysql_fetch_row(pst->res);

		stPetName.a_pet_index = atoi(pst->row[0]);
		stPetName.a_pet_owner = atoi(pst->row[1]);
		strcpy(stPetName.a_pet_name, pst->row[2]);
		stPetName.a_pet_enable = atoi(pst->row[3]);
		memcpy(szBody + arrayPoint + sizeof(*subsize) + (sizeof(stPetName) * i), &stPetName, sizeof(stPetName));
	}
	mysql_free_result(pst->res);
	return 0;
}
int t_apets_inven_map( pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int *subsize )
{
	int i = 0;
	int apets[20] = {'0', };
	st_Apets_Inven stApetsInven;
	char query[1024] = {'0', };

	sprintf(query, "select a_index from t_apets where a_owner = %d", charIndex);
	mysqlQuery_newprojectDB(pst, query);
	*subsize = mysql_num_rows(pst->res);

	for(i=0; i<*subsize; i++)
	{
		pst->row = mysql_fetch_row(pst->res);
		apets[i] = atoi(pst->row[0]);
	}
	memcpy(szBody + arrayPoint, subsize, sizeof(*subsize));
	if(*subsize > 0)
		mysql_free_result(pst->res);

	for(i=0; i<*subsize; i++)
	{
		sprintf(query, "select * from t_apets_inven where a_apet_idx = %d", apets[i]);
		mysqlQuery_newprojectDB(pst, query);

		pst->row = mysql_fetch_row(pst->res);

		stApetsInven.a_apet_idx = atoi(pst->row[0]); 
		stApetsInven.a_item_idx0 = atoi(pst->row[1]);
		stApetsInven.a_plus0 = atoi(pst->row[2]);
		stApetsInven.a_wear_pos0 = atoi(pst->row[3]);
		stApetsInven.a_flag0 = atoi(pst->row[4]);
		strcpy(stApetsInven.a_serial0, pst->row[5]);
		stApetsInven.a_count0 = atol(pst->row[6]);
		stApetsInven.a_used0 = atoi(pst->row[7]);
		stApetsInven.a_used0_2 = atoi(pst->row[8]);
		stApetsInven.a_item0_option0 = atoi(pst->row[9]);
		stApetsInven.a_item0_option1 = atoi(pst->row[10]);
		stApetsInven.a_item0_option2 = atoi(pst->row[11]);
		stApetsInven.a_item0_option3 = atoi(pst->row[12]);
		stApetsInven.a_item0_option4 = atoi(pst->row[13]);
		stApetsInven.a_item_idx1 = atoi(pst->row[14]);
		stApetsInven.a_plus1 = atoi(pst->row[15]);
		stApetsInven.a_wear_pos1 = atoi(pst->row[16]);
		stApetsInven.a_flag1 = atoi(pst->row[17]);
		stApetsInven.a_serial1[255] = atoi(pst->row[18]);
		stApetsInven.a_count1 = atol(pst->row[19]);
		stApetsInven.a_used1 = atoi(pst->row[20]);
		stApetsInven.a_used1_2 = atoi(pst->row[21]);
		stApetsInven.a_item1_option0 = atoi(pst->row[22]);
		stApetsInven.a_item1_option1 = atoi(pst->row[23]);
		stApetsInven.a_item1_option2 = atoi(pst->row[24]);
		stApetsInven.a_item1_option3 = atoi(pst->row[25]);
		stApetsInven.a_item1_option4 = atoi(pst->row[26]);
		stApetsInven.a_item_idx2 = atoi(pst->row[27]);
		stApetsInven.a_plus2 = atoi(pst->row[28]);
		stApetsInven.a_wear_pos2 = atoi(pst->row[29]);
		stApetsInven.a_flag2 = atoi(pst->row[30]);
		stApetsInven.a_serial2[255] = atoi(pst->row[31]);
		stApetsInven.a_count2 = atol(pst->row[32]);
		stApetsInven.a_used2 = atoi(pst->row[33]);
		stApetsInven.a_used2_2 = atoi(pst->row[34]);
		stApetsInven.a_item2_option0 = atoi(pst->row[35]);
		stApetsInven.a_item2_option1 = atoi(pst->row[36]);
		stApetsInven.a_item2_option2 = atoi(pst->row[37]);
		stApetsInven.a_item2_option3 = atoi(pst->row[38]);
		stApetsInven.a_item2_option4 = atoi(pst->row[39]);
		stApetsInven.a_item_idx3 = atoi(pst->row[40]);
		stApetsInven.a_plus3 = atoi(pst->row[41]);
		stApetsInven.a_wear_pos3 = atoi(pst->row[42]);
		stApetsInven.a_flag3 = atoi(pst->row[43]);
		stApetsInven.a_serial3[255] = atoi(pst->row[44]);
		stApetsInven.a_count3 = atol(pst->row[45]);
		stApetsInven.a_used3 = atoi(pst->row[46]);
		stApetsInven.a_used3_2 = atoi(pst->row[47]);
		stApetsInven.a_item3_option0 = atoi(pst->row[48]);
		stApetsInven.a_item3_option1 = atoi(pst->row[49]);
		stApetsInven.a_item3_option2 = atoi(pst->row[50]);
		stApetsInven.a_item3_option3 = atoi(pst->row[51]);
		stApetsInven.a_item3_option4 = atoi(pst->row[52]);
		memcpy(szBody + arrayPoint + sizeof(*subsize) + (sizeof(stApetsInven) * i) , &stApetsInven, sizeof(stApetsInven));
	}
	mysql_free_result(pst->res);
	return 0;
}
int t_assist_abstime_map( pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int *subsize )
{
	int i = 0;
	st_Assist_Abstime stAssistAbstime;
	char query[1024] = {'0', };

	sprintf(query, "select * from t_assist_abstime where a_char_index = %d", charIndex);
	mysqlQuery_newprojectDB(pst, query);
	*subsize = mysql_num_rows(pst->res);
	memcpy(szBody+arrayPoint, subsize, sizeof(*subsize));
	for( i=0; i<*subsize; i++)
	{
		pst->row = mysql_fetch_row(pst->res);

		stAssistAbstime.a_index = atoi(pst->row[0]);
		stAssistAbstime.a_char_index = atoi(pst->row[1]);
		stAssistAbstime.a_item_index = atoi(pst->row[2]);
		stAssistAbstime.a_skill_index = atoi(pst->row[3]);
	    stAssistAbstime.a_skill_level = atoi(pst->row[4]);
	    stAssistAbstime.a_hit0 = atoi(pst->row[5]);
		stAssistAbstime.a_hit1 = atoi(pst->row[6]);
		stAssistAbstime.a_hit2 = atoi(pst->row[7]);
		stAssistAbstime.a_end_time = atoi(pst->row[8]);

		memcpy(szBody + arrayPoint + sizeof(*subsize) + (sizeof(stAssistAbstime) * i), &stAssistAbstime, sizeof(stAssistAbstime));
	}
	mysql_free_result(pst->res);
	return 0;
}
int t_questdata_map( pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int *subsize )
{
	int i = 0;
	st_Questdata stQuestdata;
	char query[1024] = {'0', };

	sprintf(query, "select * from t_questdata0%d where a_char_index = %d", charIndex%10, charIndex);
	mysqlQuery_newprojectDB(pst, query);
	*subsize = mysql_num_rows(pst->res);
	memcpy(szBody + arrayPoint, subsize, sizeof(*subsize));

	for(i=0; i<*subsize; i++)
	{
		pst->row = mysql_fetch_row(pst->res);

		stQuestdata.a_char_index = atoi(pst->row[0]);
		stQuestdata.a_quest_index = atoi(pst->row[1]); 
		stQuestdata.a_state = atoi(pst->row[2]);
		stQuestdata.a_value0 = atoi(pst->row[3]);
		stQuestdata.a_value1 = atoi(pst->row[4]);
		stQuestdata.a_value2 = atoi(pst->row[5]);
		stQuestdata.a_quest_failvalue = atoi(pst->row[6]);
		stQuestdata.a_complete_time = atoi(pst->row[7]);
		memcpy(szBody + arrayPoint + sizeof(*subsize) + (sizeof(stQuestdata) * i), &stQuestdata, sizeof(stQuestdata));
	}
	mysql_free_result(pst->res);
	return 0;
}
int t_title_map( pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int *subsize )
{
	int i = 0;
	st_Title stTitle;
	char query[1024] = {'0', };

	sprintf(query, "select * from t_titlelist where a_char_index = %d", charIndex);
	mysqlQuery_newprojectDB(pst, query);
	*subsize = mysql_num_rows(pst->res);
	memcpy(szBody + arrayPoint, subsize, sizeof(*subsize));

	for(i=0; i<*subsize; i++)
	{
		pst->row = mysql_fetch_row(pst->res);

		stTitle.a_char_index = atoi(pst->row[0]);
		stTitle.a_title_index = atoi(pst->row[1]);
		stTitle.a_endtime = atoi(pst->row[2]);
		memcpy(szBody + arrayPoint + sizeof(*subsize) + (sizeof(stTitle) * i), &stTitle, sizeof(stTitle));
	}
	mysql_free_result(pst->res);
	return 0;
}
int t_affinity_keep_map( pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int *subsize )
{
	int i = 0;
	st_Affinity_Keep stAffinityKeep;
	char query[1024] = {'0', };

	sprintf(query, "select * from t_affinity_keep where a_charidx = %d", charIndex);
	mysqlQuery_newprojectDB(pst, query);
	*subsize = mysql_num_rows(pst->res);
	memcpy(szBody + arrayPoint, subsize, sizeof(*subsize));

	for(i=0; i<*subsize; i++)
	{
		pst->row = mysql_fetch_row(pst->res);

		stAffinityKeep.a_charidx = atoi(pst->row[0]);
		stAffinityKeep.a_affinity_idx = atoi(pst->row[1]);
		stAffinityKeep.a_point = atoi(pst->row[2]);
		memcpy(szBody + arrayPoint + sizeof(*subsize) + (sizeof(stAffinityKeep) * i), &stAffinityKeep, sizeof(stAffinityKeep));
	}
	mysql_free_result(pst->res);
	return 0;
}
int t_affinity_reward_step_map( pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int *subsize )
{
	int i = 0;
	st_Affinity_Reward_Step stAffinityRewardStep;
	char query[1024] = {'0', };

	sprintf(query, "select * from t_affinity_reward_step where a_charidx = %d", charIndex);
	mysqlQuery_newprojectDB(pst, query);
	*subsize = mysql_num_rows(pst->res);
	memcpy(szBody + arrayPoint, subsize, sizeof(*subsize));

	for(i=0; i<*subsize; i++)
	{
		pst->row = mysql_fetch_row(pst->res);

		stAffinityRewardStep.a_charidx = atoi(pst->row[0]);
		stAffinityRewardStep.a_npcidx = atoi(pst->row[1]);
		stAffinityRewardStep.a_reward_step = atoi(pst->row[2]);
		memcpy(szBody + arrayPoint + sizeof(*subsize) + (sizeof(stAffinityRewardStep) * i), &stAffinityRewardStep, sizeof(stAffinityRewardStep));
	}
	mysql_free_result(pst->res);
	return 0;
}
int t_attendance_exp_system_map( pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint)
{
	st_Attendance_Exp_System stAttendanceExpSystem;
	char query[1024] = {'0', };

	sprintf(query, "select * from t_attendance_exp_system where a_char_index = %d", charIndex);
	mysqlQuery_newprojectDB(pst, query);
	if(mysql_num_rows(pst->res) > 0)
	{

		pst->row = mysql_fetch_row(pst->res);

		stAttendanceExpSystem.a_index = atoi(pst->row[0]);
		stAttendanceExpSystem.a_char_index = atoi(pst->row[1]);
		strcpy(stAttendanceExpSystem.a_last_date, pst->row[2]);
		stAttendanceExpSystem.a_acc_count = atoi(pst->row[3]);
		stAttendanceExpSystem.a_max_acc = atoi(pst->row[4]);
		stAttendanceExpSystem.a_last_reward_point = atoi(pst->row[5]);
	}
	memcpy(szBody+arrayPoint, &stAttendanceExpSystem, sizeof(stAttendanceExpSystem));
	mysql_free_result(pst->res);
	return 0;
}


/* Data insert function */

int insert_Data_t_Character( pst_MYSQL pst, unsigned char* szBody, int arrayPoint, int* charIndex ){
	st_Character stCharacter;
	memcpy(&stCharacter, szBody+arrayPoint, sizeof(stCharacter));
	char query[1024] = {'0', };

	sprintf(query, "insert into t_characters value (0, %d, %d, '%d_%s', '%d_%s', %d, '%s', %d, '%s', %d, %d, %d, %d, %d, %d, %d, %lld, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %lld, %d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %f, %f, %f, %f, %d, %d, %d, '%s', %d, '%s', %d, %d, %d, %d, %d, %d, %d, '%s', '%s', %d, %d, %d, %d, %d, %d, %d, %lld, %lld, %d, %d, %d, %d, %d, %lld, %d, %d, %d, %d, %lld, '%s', %d, '%s', %d)", 
	stCharacter.a_user_index, g_dMySysNo, g_dMySysNo, stCharacter.a_name, g_dMySysNo, stCharacter.a_nick, 1,
	stCharacter.datestamp, stCharacter.a_lcdatestamp, stCharacter.a_createdate, stCharacter.a_deletedelay, stCharacter.a_admin,
	stCharacter.a_flag, stCharacter.a_job, stCharacter.a_hair_style, stCharacter.a_face_style, stCharacter.a_level, 
	stCharacter.a_exp, stCharacter.a_str, stCharacter.a_dex, stCharacter.a_int, stCharacter.a_con, 
	stCharacter.a_hp, stCharacter.a_max_hp, stCharacter.a_mp, stCharacter.a_max_mp, stCharacter.a_statpt_remain, 
	stCharacter.a_statpt_str, stCharacter.a_statpt_dex, stCharacter.a_statpt_con, stCharacter.a_statpt_int, stCharacter.a_skill_point,
	stCharacter.a_blood_point, stCharacter.a_active_skill_index, stCharacter.a_active_skill_level, stCharacter.a_passive_skill_index, stCharacter.a_passive_skill_level, 
	stCharacter.a_etc_skill_index, stCharacter.a_etc_skill_level, stCharacter.a_seal_skill_index, stCharacter.a_seal_skill_exp, stCharacter.a_quest_index, 
	stCharacter.a_quest_value, stCharacter.a_quest_complete, stCharacter.a_quest_abandon, stCharacter.a_was_x, stCharacter.a_was_z, 
	stCharacter.a_was_h, stCharacter.a_was_r, stCharacter.a_was_yLayer, stCharacter.a_was_zone, stCharacter.a_was_area, 
	stCharacter.a_wearing, stCharacter.a_silence_pulse, stCharacter.a_sskill, stCharacter.a_pkpenalty, stCharacter.a_pkcount, 
	stCharacter.a_pkrecover, stCharacter.a_pkpenaltyhp, stCharacter.a_pkpenaltymp, stCharacter.a_guildoutdate, stCharacter.a_pluseffect_option, 
	stCharacter.a_teach_idx, stCharacter.a_teach_sec, stCharacter.a_teach_type, stCharacter.a_fame, stCharacter.a_teach_list, 
	stCharacter.a_teach_complete, stCharacter.a_teach_fail, stCharacter.a_use_sp, stCharacter.a_total_sp, stCharacter.a_loseexp, 
	stCharacter.a_losesp, stCharacter.a_job2, stCharacter.a_subjob, stCharacter.a_pd3time, stCharacter.a_superstone, 
	stCharacter.a_guardian, stCharacter.a_etc_event, stCharacter.a_lastpktime, 
	stCharacter.a_phoenix, stCharacter.a_title_index, stCharacter.a_newtuto_complete, stCharacter.a_exp_weekly, stCharacter.a_levelup_date, 
	stCharacter.a_cp, stCharacter.a_cp_lstupdate, stCharacter.a_trans_time);
	if((mysqlQuery_newprojectDB(pst, query) != 0)){
		dAppLog( LOG_CRI, "'%s'", query );
		return -1;
	}

	memset(query, 0x00, sizeof(query));
	sprintf(query, "select a_index from t_characters where a_name = '%d_%s'", g_dMySysNo, stCharacter.a_name); 

	mysqlQuery_newprojectDB(pst, query);
	
	if(mysql_num_rows(pst->res) > 0)
	{
		pst->row = mysql_fetch_row(pst->res);
		*charIndex = atoi(pst->row[0]);
		mysql_free_result(pst->res);
		return 0;
	}
	else
		return -1;
}


int insert_Data_t_Inven( pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint)
{
	st_Inven stInven;
	memcpy(&stInven, szBody+arrayPoint, sizeof(stInven));
	char query[2048] = {'0', };

	sprintf(query, "insert into t_inven0%d value (%d, %d, %d, %d, %d, %d, %d, '%s', %lld, %d, %d, %d, %d, %d, '%s', %lld, %d, %d, %d, %d, %d, '%s', %lld, %d, %d, %d, %d, %d, '%s', %lld, %d, %d, %d, %d, %d, '%s', %lld, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, '%s', '%s', '%s', '%s', '%s',	%d, %d, %d,	%d, %d, %d, %d, %d, %d,	%d, %d, %d,	%d, %d, %d,	%d, %d, %d,	%d, %d, %d,	%d, %d, %d,	%d, %d, %d,	%d, %d, %d)",
			charIndex%10, charIndex, stInven.a_tab_idx, stInven.a_row_idx, 
			stInven.a_item_idx0, stInven.a_plus0, stInven.a_wear_pos0, stInven.a_flag0, stInven.a_serial0, stInven.a_count0, stInven.a_used0, 
			stInven.a_item_idx1, stInven.a_plus1, stInven.a_wear_pos1, stInven.a_flag1, stInven.a_serial1, stInven.a_count1, stInven.a_used1,
			stInven.a_item_idx2, stInven.a_plus2, stInven.a_wear_pos2, stInven.a_flag2, stInven.a_serial2, stInven.a_count2, stInven.a_used2, 
			stInven.a_item_idx3, stInven.a_plus3, stInven.a_wear_pos3, stInven.a_flag3, stInven.a_serial3, stInven.a_count3, stInven.a_used3,  
			stInven.a_item_idx4, stInven.a_plus4, stInven.a_wear_pos4, stInven.a_flag4,	stInven.a_serial4,stInven.a_count4, stInven.a_used4, 
			stInven.a_item0_option0, stInven.a_item0_option1, stInven.a_item0_option2, stInven.a_item0_option3, stInven.a_item0_option4,
			stInven.a_item1_option0, stInven.a_item1_option1, stInven.a_item1_option2, stInven.a_item1_option3, stInven.a_item1_option4,
			stInven.a_item2_option0, stInven.a_item2_option1, stInven.a_item2_option2, stInven.a_item2_option3,	stInven.a_item2_option4, 
			stInven.a_item3_option0, stInven.a_item3_option1, stInven.a_item3_option2, stInven.a_item3_option3,	stInven.a_item3_option4, 
			stInven.a_item4_option0, stInven.a_item4_option1, stInven.a_item4_option2, stInven.a_item4_option3, stInven.a_item4_option4,
			stInven.a_used0_2, stInven.a_used1_2, stInven.a_used2_2, stInven.a_used3_2, stInven.a_used4_2, 
			stInven.a_socket0, stInven.a_socket1, stInven.a_socket2, stInven.a_socket3, stInven.a_socket4,
			stInven.a_item_0_origin_var0, stInven.a_item_0_origin_var1, stInven.a_item_0_origin_var2, 
			stInven.a_item_0_origin_var3, stInven.a_item_0_origin_var4, stInven.a_item_0_origin_var5,
			stInven.a_item_1_origin_var0, stInven.a_item_1_origin_var1, stInven.a_item_1_origin_var2, 
			stInven.a_item_1_origin_var3, stInven.a_item_1_origin_var4, stInven.a_item_1_origin_var5,
			stInven.a_item_2_origin_var0, stInven.a_item_2_origin_var1, stInven.a_item_2_origin_var2, 
			stInven.a_item_2_origin_var3, stInven.a_item_2_origin_var4, stInven.a_item_2_origin_var5,
			stInven.a_item_3_origin_var0, stInven.a_item_3_origin_var1, stInven.a_item_3_origin_var2, 
			stInven.a_item_3_origin_var3, stInven.a_item_3_origin_var4, stInven.a_item_3_origin_var5,
			stInven.a_item_4_origin_var0, stInven.a_item_4_origin_var1, stInven.a_item_4_origin_var2, 
			stInven.a_item_4_origin_var3, stInven.a_item_4_origin_var4, stInven.a_item_4_origin_var5
			);
	
	if((mysqlQuery_newprojectDB(pst, query) != 0)){
        dAppLog( LOG_CRI, "%s\n%s", mysql_error(pst->conn), query );
        return -1;
    }
	return 0;
}


int insert_Data_t_Pet(pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int *old_pet_Index)
{
	st_Pet stPet;
	memcpy(&stPet, szBody+arrayPoint, sizeof(stPet));
	*old_pet_Index = stPet.a_index;

	char query[1024] = {'0', };
	sprintf(query, "insert into t_pet value (0, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, '%s', '%s', %d, %d, '%s', %d, %d)", 
			charIndex, stPet.a_enable, stPet.a_lastupdate, stPet.a_type, stPet.a_level, stPet.a_hp, stPet.a_hungry, stPet.a_sympathy, stPet.a_exp, 
			stPet.a_ability, stPet.a_skill_index, stPet.a_skill_level, stPet.a_time_rebirth, stPet.a_index_old, stPet.a_color, 
			stPet.a_pet_turnto_npc, stPet.a_pet_size);

	if((mysqlQuery_newprojectDB(pst, query) != 0)){
        dAppLog( LOG_CRI, "'%s'", query );
        return -1;
    }
	return 0;
}


int insert_Data_t_Apets(pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int *old_apet_Index)
{
	st_Apets stApets;
	memcpy(&stApets, szBody+arrayPoint, sizeof(stApets));
	*old_apet_Index = stApets.a_index;
	char query[1024] = {'0', };
	sprintf(query, "insert into t_apets value(0, %d, '%s', %d, %d, '%s', '%s', '%s', %d, %lld, %d, %d, %d, %d, %d, %d, '%s', '%s', %d, %d, %d, %d, '%s', %d, %d, %d, %lld, %d)", 
			charIndex, stApets.a_enable, stApets.a_seal, stApets.a_proto_index, stApets.a_name, stApets.a_create_date, stApets.a_lastupdate_date, 
			stApets.a_level, stApets.a_exp, stApets.a_remain_stat, stApets.a_plus_str, stApets.a_plus_con, stApets.a_plus_dex, stApets.a_plus_int, 
			stApets.a_skill_point, stApets.a_skill_index, stApets.a_skill_level, stApets.a_hp, stApets.a_mp, stApets.a_faith, stApets.a_stm, 
			stApets.a_ai_enable, stApets.a_ai_slot, stApets.a_accFaith, stApets.a_accStm, stApets.a_accExp, stApets.a_cooltime);
	
	if((mysqlQuery_newprojectDB(pst, query) != 0)){
        dAppLog( LOG_CRI, "'%s'", query );
        return -1;
    }
	return 0;
}


int insert_Data_t_Pet_Name(pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int pet_index)
{
	st_Pet_Name stPetName;
	memcpy(&stPetName, szBody+arrayPoint, sizeof(stPetName));
	char query[1024] = {'0', };
	sprintf(query, "insert into t_pet_name value (%d, %d, '%s', %d)", pet_index, charIndex, stPetName.a_pet_name, stPetName.a_pet_enable);
	
	if((mysqlQuery_newprojectDB(pst, query) != 0)){
        dAppLog( LOG_CRI, "'%s'", query );
        return -1;
    }	
	return 0;
}


int insert_Data_t_Apets_Inven(pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint, int apet_index)
{
	st_Apets_Inven stApetsInven;
	memcpy(&stApetsInven, szBody+arrayPoint, sizeof(stApetsInven));
	char query[1024] = {'0', };

	sprintf(query, "insert into t_apets_inven value (%d, %d, %d, %d, %d, '%s', %lld, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, '%s', %lld, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, '%s', %lld, %d, %d, %d, %d, %d, %d, %d)",
			apet_index, 
			stApetsInven.a_item_idx0, stApetsInven.a_plus0, stApetsInven.a_wear_pos0, stApetsInven.a_flag0, stApetsInven.a_serial0, stApetsInven.a_count0, 
			stApetsInven.a_used0, stApetsInven.a_used0_2, 
			stApetsInven.a_item0_option0, stApetsInven.a_item0_option1, stApetsInven.a_item0_option2, stApetsInven.a_item0_option3, stApetsInven.a_item0_option4, 
			stApetsInven.a_item_idx1, stApetsInven.a_plus1, stApetsInven.a_wear_pos1, stApetsInven.a_flag1, stApetsInven.a_serial1, stApetsInven.a_count1, 
			stApetsInven.a_used1, stApetsInven.a_used1_2, 
			stApetsInven.a_item1_option0, stApetsInven.a_item1_option1, stApetsInven.a_item1_option2, stApetsInven.a_item1_option3, stApetsInven.a_item1_option4, 
			stApetsInven.a_item_idx2, stApetsInven.a_plus2, stApetsInven.a_wear_pos2, stApetsInven.a_flag2, stApetsInven.a_serial2, stApetsInven.a_count2, 
			stApetsInven.a_used2, stApetsInven.a_used2_2, 
			stApetsInven.a_item2_option0, stApetsInven.a_item2_option1, stApetsInven.a_item2_option2, stApetsInven.a_item2_option3, stApetsInven.a_item2_option4, 
			stApetsInven.a_item_idx3, stApetsInven.a_plus3, stApetsInven.a_wear_pos3, stApetsInven.a_flag3, stApetsInven.a_serial3, stApetsInven.a_count3, 
			stApetsInven.a_used3, stApetsInven.a_used3_2, 
			stApetsInven.a_item3_option0, stApetsInven.a_item3_option1, stApetsInven.a_item3_option2, stApetsInven.a_item3_option3, stApetsInven.a_item3_option4);

	if((mysqlQuery_newprojectDB(pst, query) != 0)){
        dAppLog( LOG_CRI, "'%s'", query );
        return -1;
    }
	return 0;
}


int insert_Data_t_Assist_Abstime(pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint)
{   
	st_Assist_Abstime stAssistAbstime;
	memcpy(&stAssistAbstime, szBody+arrayPoint, sizeof(stAssistAbstime));
	char query[1024] = {'0', };
	sprintf(query, "insert into t_assist_abstime value (0, %d, %d, %d, %d, %d, %d, %d, %d)",
			charIndex, stAssistAbstime.a_item_index, stAssistAbstime.a_skill_index, stAssistAbstime.a_skill_level, 
			stAssistAbstime.a_hit0, stAssistAbstime.a_hit1, stAssistAbstime.a_hit2, stAssistAbstime.a_end_time);

	if((mysqlQuery_newprojectDB(pst, query) != 0)){
        dAppLog( LOG_CRI, "'%s'", query );
        return -1;
    }
	return 0;
}


int insert_Data_t_QuestData(pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint)
{
	int questTableNum;
	st_Questdata stQuestdata;
	memcpy(&stQuestdata, szBody+arrayPoint, sizeof(stQuestdata));
	questTableNum = charIndex % 10;
	char query[1024] = {'0', };
	sprintf(query, "insert into t_questdata0%d value (%d, %d, %d, %d, %d, %d, %d, %d)", 
			questTableNum, charIndex, stQuestdata.a_quest_index, stQuestdata.a_state, 
			stQuestdata.a_value0, stQuestdata.a_value1, stQuestdata.a_value2, stQuestdata.a_quest_failvalue, stQuestdata.a_complete_time);

	if((mysqlQuery_newprojectDB(pst, query) != 0)){
        dAppLog( LOG_CRI, "'%s'", query );
        return -1;
    }
	return 0;
}


int insert_Data_t_Title(pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint)
{
	st_Title stTitle;
	memcpy(&stTitle, szBody+arrayPoint, sizeof(stTitle));
	char query[1024] = {'0', };
	sprintf(query, "insert into t_titlelist value(%d, %d, %d)", 
			charIndex, stTitle.a_title_index, stTitle.a_endtime);

	if((mysqlQuery_newprojectDB(pst, query) != 0)){
        dAppLog( LOG_CRI, "'%s'", query );
        return -1;
    }
	return 0;
}


int insert_Data_t_Affinity_Keep(pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint)
{
	st_Affinity_Keep stAffinityKeep;
	memcpy(&stAffinityKeep, szBody+arrayPoint, sizeof(stAffinityKeep));
	char query[1024] = {'0', };
	sprintf(query, "insert into t_affinity_keep value (%d, %d, %d)", 
			charIndex, stAffinityKeep.a_affinity_idx, stAffinityKeep.a_point);

	if((mysqlQuery_newprojectDB(pst, query) != 0)){
        dAppLog( LOG_CRI, "'%s'", query );
        return -1;
    }
	return 0;
}


int insert_Data_t_Affinity_Reward_Step(pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint)
{
	st_Affinity_Reward_Step stAffinityRewardStep;
	memcpy(&stAffinityRewardStep, szBody+arrayPoint, sizeof(stAffinityRewardStep));
	char query[1024] = {'0', };
	sprintf(query, "insert into t_affinity_reward_step value ( %d, %d, %d )", 
			charIndex, stAffinityRewardStep.a_npcidx, stAffinityRewardStep.a_reward_step);

	if((mysqlQuery_newprojectDB(pst, query) != 0)){
        dAppLog( LOG_CRI, "'%s'", query );
        return -1;
    }
	return 0;
}


int insert_Data_t_Attendance_Exp_System(pst_MYSQL pst, unsigned char* szBody, int charIndex, int arrayPoint)
{
	st_Attendance_Exp_System stAttendanceExpSystem;
	memcpy(&stAttendanceExpSystem, szBody+arrayPoint, sizeof(stAttendanceExpSystem));
	char query[1024] = {'0', };
	sprintf(query, "insert into t_attendance_exp_system value(0, %d, '%s', %d, %d, %d)", 
			charIndex, stAttendanceExpSystem.a_last_date, stAttendanceExpSystem.a_acc_count, stAttendanceExpSystem.a_max_acc, stAttendanceExpSystem.a_last_reward_point);

	if((mysqlQuery_newprojectDB(pst, query) != 0)){
        dAppLog( LOG_CRI, "'%s'", query );
        return -1;
    }
	return 0;
}

int check_Data_CharacterSlot(pst_MYSQL pst, unsigned char* szBody, int userIndex)
{
	char query[1024] = {'0', };
	int errorCode = 0;
	sprintf(query, "select count(a_user_index) from t_characters where a_user_index = %d and a_enable <> 0", userIndex);
	mysqlQuery_newprojectDB(pst, query);

	pst->row = mysql_fetch_row(pst->res);

	if(atoi(pst->row[0]) >= 8)
		errorCode = 803;
	mysql_free_result(pst->res);
	return errorCode;
}

int select_Data_Pet(pst_MYSQL pst, int* petIndex, int charIndex)
{
	int subsize;
	int i;
	char query[1024] = {'0', };
	sprintf(query, "select a_index from t_pet where a_owner = %d", charIndex);
	mysqlQuery_newprojectDB(pst, query);
	
	subsize = mysql_num_rows(pst->res);
	for(i=0; i<subsize; i++)
	{
		pst->row = mysql_fetch_row(pst->res);
		petIndex[i] = atoi(pst->row[0]);
	}
	mysql_free_result(pst->res);
	return 0;
}

int select_Data_Apets(pst_MYSQL pst, int* apetIndex, int charIndex)
{
	int subsize;
	int i;
	char query[1024] = {'0', };
	sprintf(query, "select a_index from t_apets where a_owner = %d", charIndex); 
	mysqlQuery_newprojectDB(pst, query);

	subsize = mysql_num_rows(pst->res);
	for(i=0; i<subsize; i++)
	{
		pst->row = mysql_fetch_row(pst->res);
		apetIndex[i] = atoi(pst->row[0]);
	}
	mysql_free_result(pst->res);
	return 0;
}

int update_Inven_Petdata(pst_MYSQL pst, int charIndex, int* pet_Index, int* old_pet_Index, int subsize)
{
	int queryCount, i, j;
	int count = 0;
	char query[1024] = {'0', };
	int plusData[100] = {'0', };
	sprintf(query, "select a_plus0,a_plus1,a_plus2,a_plus3,a_plus4 from t_inven0%d where a_char_idx = %d", charIndex%10, charIndex);
	mysqlQuery_newprojectDB(pst, query);

	queryCount = mysql_num_rows(pst->res);
	for(i=0; i<queryCount; i++)
	{
		pst->row = mysql_fetch_row(pst->res);
		for(j=0; j<5; j++)
		{
			plusData[count] = atoi(pst->row[j]);
			count++;
		}
	}
	mysql_free_result(pst->res);
	
	for(i=0; i<subsize; i++)
	{
		for(j=0; j<100; j++)
		{
			if(old_pet_Index[i] == plusData[j])
			{
				sprintf(query, "update t_inven0%d set a_plus%d = %d where a_char_idx = %d and a_plus%d = %d", charIndex%10, j%5, pet_Index[i], charIndex, j%5, old_pet_Index[i]);
				mysqlQuery_newprojectDB(pst, query);
			}
		}
	}
	return 0;
}	

int update_Inven_Apetdata(pst_MYSQL pst, int charIndex, int* apet_Index, int* old_apet_Index, int subsize)
{
	int count = 0;
	int queryCount, i, j;
    char query[1024] = {'0', };
    int plusData[100] = {'0', };
    sprintf(query, "select a_plus0,a_plus1,a_plus2,a_plus3,a_plus4 from t_inven0%d where a_char_idx = %d", charIndex%10, charIndex);
    mysqlQuery_newprojectDB(pst, query);

	queryCount = mysql_num_rows(pst->res);
    for(i=0; i<queryCount; i++)
    {   
        pst->row = mysql_fetch_row(pst->res);
        for(j=0; j<5; j++)
        {
            plusData[count] = atoi(pst->row[j]);
            count++;
        }
    }  
    mysql_free_result(pst->res);

    for(i=0; i<subsize; i++)
    {
        for(j=0; j<100; j++)
        {
            if(old_apet_Index[i] == plusData[j])
            {
                sprintf(query, "update t_inven0%d set a_plus%d = %d where a_char_idx = %d and a_plus%d = %d", charIndex%10, j%5, apet_Index[i], charIndex, j%5, old_apet_Index[i]);
                mysqlQuery_newprojectDB(pst, query);
            }
        }
    }
    return 0;
}

int rollback_databases(pst_MYSQL pst, int charIndex, int* apetIndex)
{
	char query[1024] = {'0', };
	int i;
	sprintf(query, "delete from t_characters where a_index = %d", charIndex);
	mysqlQuery_newprojectDB(pst, query);
	sprintf(query, "delete from t_pet where a_owner = %d", charIndex);
	mysqlQuery_newprojectDB(pst, query);
	sprintf(query, "delete from t_inven0%d where a_char_idx = %d", charIndex%10, charIndex);
	mysqlQuery_newprojectDB(pst, query);
	sprintf(query, "delete from t_apets where a_owner = %d", charIndex);
	mysqlQuery_newprojectDB(pst, query);
	for(i=0; i<50; i++)
	{
		sprintf(query, "delete from t_apets_inven where a_apet_idx = %d", apetIndex[i]);
		mysqlQuery_newprojectDB(pst, query);
	}
	sprintf(query, "delete from t_pet_name where a_pet_owner = %d", charIndex);
	mysqlQuery_newprojectDB(pst, query);
	sprintf(query, "delete from t_assist_abstime where a_char_index = %d", charIndex);
	mysqlQuery_newprojectDB(pst, query);
	sprintf(query, "delete from t_questdata0%d where a_char_index = %d", charIndex%10, charIndex);
	mysqlQuery_newprojectDB(pst, query);
	sprintf(query, "delete from t_titlelist where a_char_index = %d", charIndex);
	mysqlQuery_newprojectDB(pst, query);
	sprintf(query, "delete from t_affinity_keep where a_charidx = %d", charIndex);
	mysqlQuery_newprojectDB(pst, query);
	sprintf(query, "delete from t_affinity_reward_step where a_charidx = %d", charIndex);
	mysqlQuery_newprojectDB(pst, query);
	sprintf(query, "delete from t_attendance_exp_system where a_char_index = %d", charIndex );
	mysqlQuery_newprojectDB(pst, query);

	return 0;
}


int get_subsize(unsigned char* szBody, int* arrayPoint)
{
	int subsize;

	memcpy(&subsize, szBody + *arrayPoint, sizeof(subsize));
	
	*arrayPoint += sizeof(int);

	return subsize;
}

int set_serverTransState(pst_MYSQL pst, int state, int charIndex, int errorCode, int enable)
{	
	char query[1024] = {'0', };

	sprintf(query, "update t_server_trans set a_state = %d, a_fail_code = %d, a_end_time = unix_timestamp(now()) where a_char_index = %d", state, errorCode, charIndex);

	mysqlQuery_newprojectDB(pst, query);

	sprintf(query, "update t_characters set a_enable = %d where a_index = %d", enable, charIndex);

	mysqlQuery_newprojectDB(pst, query);

	return 0;
}

