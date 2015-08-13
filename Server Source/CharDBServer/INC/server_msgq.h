#ifndef _SERVER_MSGQ___H__
#define _SERVER_MSGQ___H__

#define DEF_MAX_LOG_QNUM 10000
#define MAX_MSGBODY_SIZE 65535

typedef struct _st_MsgQ
{
	unsigned short	usType;			/* 서비스 타입 */
	unsigned short	usSvcID;		/* 서비스 ID */
	unsigned short	usMsgID;		/* 메시지 ID */
	unsigned short	usSystemNo;		/* TEMS 서버 INDEX */

	unsigned short  ucProID;		/* Process ID : Example = SEQ_PROC_SESSIF */
	unsigned short	usBodyCount;	/* Message Body에 포함된 Message들의 개수 */
	int         	dMsgQID;		/* Source Queue ID */

	unsigned short  usBodyLen;		/* Receive Message Body Length */
	unsigned short  usRetCode;		/* RetCode */
	unsigned int	uiIndex;		/* INDEX : 명령어 처리시 필요 */

	unsigned long long llSockSessID;

	unsigned char   szBody[MAX_MSGBODY_SIZE];   /* Real Message : 10000 */
} st_MsgQ, *pst_MsgQ;

#define set_msgq_content( pq, sid, mid, sysno, proid, sessid ) \
	(pq)->usType = DEF_SVC;			\
	(pq)->usSvcID = sid;			\
	(pq)->usMsgID = mid;			\
	(pq)->usSystemNo = sysno;		\
	(pq)->ucProID = proid;			\
	(pq)->llSockSessID = sessid;


#define DEF_MSGQ_SIZE       sizeof(st_MsgQ)
#define DEF_MSGHEAD_LEN     (DEF_MSGQ_SIZE - MAX_MSGBODY_SIZE)


typedef struct _st_Character
{
	int a_user_index;
	int a_server;
	char a_name[20];
	char a_nick[20];
	int a_enable;
	char datestamp[30];
	int a_lcdatestamp;
	char a_createdate[30];
	int a_deletedelay;
	int a_admin;
	int a_flag;
	int a_job;
	int a_hair_style;
	int a_face_style;
	int a_level;
	long long a_exp;
	int a_str;
	int a_dex;
	int a_int;
	int a_con;
	int a_hp;
	int a_max_hp;
	int a_mp;
	int a_max_mp;
	int a_statpt_remain;
	int a_statpt_str;
	int a_statpt_dex;
	int a_statpt_con;
	int a_statpt_int;
	long long a_skill_point;
	int a_blood_point;
	char a_active_skill_index[255];
	char a_active_skill_level[255];
	char a_passive_skill_index[255];
	char a_passive_skill_level[255];
	char a_etc_skill_index[255];
	char a_etc_skill_level[255];
	char a_seal_skill_index[255];
	char a_seal_skill_exp[255];
	char a_quest_index[255];
	char a_quest_value[255];
	char a_quest_complete[255];
	char a_quest_abandon[255];
	float a_was_x;
	float a_was_z;
	float a_was_h;
	float a_was_r;
	int a_was_yLayer;
	int a_was_zone;
	int a_was_area;
	char a_wearing[255];
	int a_silence_pulse;
	char a_sskill[255];
	int a_pkpenalty;
	int a_pkcount;
	int a_pkrecover;
	int a_pkpenaltyhp;
	int a_pkpenaltymp;
	int a_guildoutdate;
	int a_pluseffect_option;
	char a_teach_idx[255];
	char a_teach_sec[255];
	int a_teach_type;
	int a_fame;
	int a_teach_list;
	int a_teach_complete;
	int a_teach_fail;
	int a_use_sp;
	int a_total_sp;
	long long a_loseexp;
	long long a_losesp;
	int a_job2;
	int a_subjob;
	int a_pd3time;
	int a_superstone;
	int a_guardian;
	long long a_etc_event;
//	int a_index_old;
//	int a_server_old;
	int a_lastpktime;
	int a_phoenix;
	int a_title_index;
	int a_newtuto_complete;
	long long a_exp_weekly;
	char a_levelup_date[30];
	int a_cp;
	char a_cp_lstupdate[30];
	int a_trans_time;
} st_Character, *pst_Character;


typedef struct _st_INVEN
{
	int a_char_idx;
	short a_tab_idx;
	short a_row_idx;
	int a_item_idx0;
	int a_plus0;
	short a_wear_pos0;
	int a_flag0;
	char a_serial0[255];
	long long a_count0;
	int a_used0;
	int a_item_idx1;
	int a_plus1;
	short a_wear_pos1;
	int a_flag1;
	char a_serial1[255];
	long long a_count1;
	int a_used1;
	int a_item_idx2;
	int a_plus2;
	short a_wear_pos2;
	int a_flag2;
	char a_serial2[255];
	long long a_count2;
	int a_used2;
	int a_item_idx3;
	int a_plus3;
	short a_wear_pos3;
	int a_flag3;
	char a_serial3[255];
	long long a_count3;
	int a_used3;
	int a_item_idx4;
	int a_plus4;
	short a_wear_pos4;
	int a_flag4;
	char a_serial4[255];
	long long a_count4;
	int a_used4;
	short a_item0_option0;
	short a_item0_option1;
	short a_item0_option2;
	short a_item0_option3;
	short a_item0_option4;
	short a_item1_option0;
	short a_item1_option1;
	short a_item1_option2;
	short a_item1_option3;
	short a_item1_option4;
	short a_item2_option0;
	short a_item2_option1;
	short a_item2_option2;
	short a_item2_option3;
	short a_item2_option4;
	short a_item3_option0;
	short a_item3_option1;
	short a_item3_option2;
	short a_item3_option3;
	short a_item3_option4;
	short a_item4_option0;
	short a_item4_option1;
	short a_item4_option2;
	short a_item4_option3;
	short a_item4_option4;
	int a_used0_2;
	int a_used1_2;
	int a_used2_2;
	int a_used3_2;
	int a_used4_2;
	char a_socket0[50];
	char a_socket1[50];
	char a_socket2[50];
	char a_socket3[50];
	char a_socket4[50];
	short a_item_0_origin_var0;
	short a_item_0_origin_var1;
	short a_item_0_origin_var2;
	short a_item_0_origin_var3;
	short a_item_0_origin_var4;
	short a_item_0_origin_var5;
	short a_item_1_origin_var0;
	short a_item_1_origin_var1;
	short a_item_1_origin_var2;
	short a_item_1_origin_var3;
	short a_item_1_origin_var4;
	short a_item_1_origin_var5;
	short a_item_2_origin_var0;
	short a_item_2_origin_var1;
	short a_item_2_origin_var2;
	short a_item_2_origin_var3;
	short a_item_2_origin_var4;
	short a_item_2_origin_var5;
	short a_item_3_origin_var0;
	short a_item_3_origin_var1;
	short a_item_3_origin_var2;
	short a_item_3_origin_var3;
	short a_item_3_origin_var4;
	short a_item_3_origin_var5;
	short a_item_4_origin_var0;
	short a_item_4_origin_var1;
	short a_item_4_origin_var2;
	short a_item_4_origin_var3;
	short a_item_4_origin_var4;
	short a_item_4_origin_var5;
} st_Inven, *pst_Inven;

typedef struct _st_PET
{
	int a_index;
	int a_owner;
	int a_enable;
	int a_lastupdate;
	int a_type;
	int a_level;
	int a_hp;
	int a_hungry;
	int a_sympathy;
	int a_exp;
	int a_ability;
	char a_skill_index[255];
	char a_skill_level[255];
	int a_time_rebirth;
	int a_index_old;
	char a_color[2];
	int a_pet_turnto_npc;
	int a_pet_size;
} st_Pet, *pst_Pet;

typedef struct _st_APETS
{
	int a_index;
	int a_owner;
	char a_enable[1];
	int a_seal;
	int a_proto_index;
	char a_name[20];
	char a_create_date[30];
	char a_lastupdate_date[30];
	int a_level;
	long long a_exp;
	int a_remain_stat;
	int a_plus_str;
	int a_plus_con;
	int a_plus_dex;
	int a_plus_int;
	int a_skill_point;
	char a_skill_index[255];
	char a_skill_level[255];
	int a_hp;
	int a_mp;
	int a_faith;
	int a_stm;
	char a_ai_enable[5];
	int a_ai_slot;
	int a_accFaith;
	int a_accStm;
	long long a_accExp;
	int a_cooltime;
} st_Apets, *pst_Apets;

typedef struct _st_PET_NAME
{
	int a_pet_index;
	int a_pet_owner;
	char a_pet_name[20];
	int a_pet_enable;
} st_Pet_Name, *pst_Pet_Name;

typedef struct _st_APETS_INVEN
{
	int a_apet_idx;
	int a_item_idx0;
	int a_plus0;
	int a_wear_pos0;
	int a_flag0;
	char a_serial0[255];
	long long a_count0;
	int a_used0;
	int a_used0_2;
	int a_item0_option0;
	int a_item0_option1;
	int a_item0_option2;
	int a_item0_option3;
	int a_item0_option4;
	int a_item_idx1;
	int a_plus1;
	int a_wear_pos1;
	int a_flag1;
	char a_serial1[255];
	int a_count1;
	int a_used1;
	int a_used1_2;
	int a_item1_option0;
	int a_item1_option1;
	int a_item1_option2;
	int a_item1_option3;
	int a_item1_option4;
	int a_item_idx2;
	int a_plus2;
	int a_wear_pos2;
	int a_flag2;
	char a_serial2[255];
	long long a_count2;
	int a_used2;
	int a_used2_2;
	int a_item2_option0;
	int a_item2_option1;
	int a_item2_option2;
	int a_item2_option3;
	int a_item2_option4;
	int a_item_idx3;
	int a_plus3;
	int a_wear_pos3;
	int a_flag3;
	char a_serial3[255];
	long long a_count3;
	int a_used3;
	int a_used3_2;
	int a_item3_option0;
	int a_item3_option1;
	int a_item3_option2;
	int a_item3_option3;
	int a_item3_option4;
} st_Apets_Inven, *pst_Apets_Inven;

typedef struct _st_ASSIST_ABSTIME
{
	int a_index;
	int a_char_index;
	int a_item_index;
	int a_skill_index;
	int a_skill_level;
	int a_hit0;
	int a_hit1;
	int a_hit2;
	int a_end_time;
} st_Assist_Abstime, *pst_Assist_Abstime;

typedef struct _st_QUESTDATA
{
	int a_char_index;
	int a_quest_index;
	int a_state;
	int a_value0;
	int a_value1;
	int a_value2;
	int a_quest_failvalue;
	int a_complete_time;
} st_Questdata, *pst_Questdata;

typedef struct _st_TITLE
{
	int a_char_index;
	int a_title_index;
	int a_endtime;
} st_Title, *pst_Title;

typedef struct _st_AFFINITY_KEEP
{
	int a_charidx;
	int a_affinity_idx;
	int a_point;
} st_Affinity_Keep, *pst_Affinity_Keep;

typedef struct _st_AFFINITY_REWARD_STEP
{
	int a_charidx;
	int a_npcidx;
	int a_reward_step;
} st_Affinity_Reward_Step, *pst_Affinity_Reward_Step;

typedef struct _st_ATTENDANCE_EXP_SYSTEM
{
	int a_index;
	int a_char_index;
	char a_last_date[30];
	int a_acc_count;
	int a_max_acc;
	int a_last_reward_point;
} st_Attendance_Exp_System, *pst_Attendance_Exp_System;

typedef struct _st_RESPONSE_MSG
{
	int charIndex;
	int currentServer;
	int moveServer;
	int isSuccess;
	int userIndex;
	int errorCode;
	char errorMsg[100];
} st_Response_Msg;

#endif
