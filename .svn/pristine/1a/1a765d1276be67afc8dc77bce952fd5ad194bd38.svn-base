
/*********************************************************
                 NUMENSOFT SYSTEM DEFINITION

   Author   : HWANG WOO HYOUNG
   Section  : GAME SERVER Project
   SCCS ID  : %W%
   Date     : %G%
   Revision History :
        '07.  8. 13     Initial

   Description:
        Recovery

   Copyright (c) NUMENSOFT 2001
***********************************************************/

#ifndef __SERVER_DEFINE_HEADER_FILE__
#define __SERVER_DEFINE_HEADER_FILE__

#define INTEL				1

#define PROC_PATH			"/proc"
#define PARENT_PATH			".."
#define HOME_PATH			"."

#define START_PATH			"/home/SERVERAPP"
#define APP_PATH			"/SERVERAPP/NEW"
#define BACKUP_PATH			"/SERVERAPP/OLD"

#define BIN_PATH			START_PATH"/BIN/"
#define APP_HOME_BIN		BIN_PATH
#define APP_HOME  			BIN_PATH

/* configure data path */
#define DATA_PATH			START_PATH"/DATA/"
#define MC_INIT				DATA_PATH"McInit"

#define DEF_NATION_CONF		DATA_PATH"NATION_INFO.conf"

/* log path */
#define LOG_PATH			START_PATH"/LOG/"
#define CHSMD_LOG			LOG_PATH"CHSMD"
#define ALMD_LOG			LOG_PATH"ALMD"
#define COND_LOG			LOG_PATH"COND"
#define MMCD_LOG			LOG_PATH"MMCD"

/* replay path */
#define REPLAY_PATH			START_PATH"/REP/"

#define SI_TCP_IN_LOG		LOG_PATH"SI_TCP_IN"
#define DUP_TEST_LOG		LOG_PATH"DUP_TEST"
#define MAX_MSGQ_COUNT  	100
#define CPU_ASSIGN 			(prevent_cpu(0,1))

#define DEF_ITEM_SERIAL		DATA_PATH"ITEM_SERIAL.bin"

#define DEF_CHANNELSTS_FILE	DATA_PATH"CHNL_STAT.conf"

/* MQMAP path */
#define MQMAP_CONF			DATA_PATH"MQMAP"

/* MSGMAP path */
#define MSGMAP_CONF			DATA_PATH"MSGMAP"


//#define MAX_CLIENT_NUM		1023

/* message queue, shared memory */
#define MSGQ_DEFINE			8000
#define PORT_DEFINE			18000
#define PROC_DEFINE			9000
#define SSHM_DEFINE			10000
#define SEMA_DEFINE			11000

/* Management Block Definition */
/* WARNING!! MSGQ Key number must be short range inside: 0x0000 ~ 0xFFFF */

/* common queue */
#define S_MSGQ_W_LOG			( MSGQ_DEFINE + 0 ) 	/*** 1f40 ***/
#define S_MSGQ_CHSMD			( MSGQ_DEFINE + 1 ) 	/*** 1f41 ***/
#define S_MSGQ_CHAR_RELOCATE	( MSGQ_DEFINE + 2 )		/*** 1f42 ***/
#define S_MSGQ_CDB_STSOC		( MSGQ_DEFINE + 3 )		/*** 1f43 ***/
#define S_MSGQ_MMCD				( MSGQ_DEFINE + 4 )		/*** 1f44 ***/
#define S_MSGQ_COND				( MSGQ_DEFINE + 5 )		/*** 1f45 ***/

/* Management Port Definition */
#define S_PORT_CDB_STSOC     	( PORT_DEFINE + 400 ) 	/* 18400 */

/* Shared Memory Key */
#define S_SSHM_CHARDBSSOCK      ( SSHM_DEFINE + 300 )   /*** 283c ***/
#define S_SSHM_CHARDBCSOCK		( SSHM_DEFINE + 301 )	/*** 283d ***/
#define S_SSHM_ALMLOAD			( SSHM_DEFINE + 302 )	/*** 283e ***/
#define S_SSHM_FIDB				( SSHM_DEFINE + 303 )	/*** 283f ***/
#define S_SSHM_GENINFO			( SSHM_DEFINE + 304 )	/*** 2840 ***/

#endif
