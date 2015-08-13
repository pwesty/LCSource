/**********************************************************
 *           ABLEX DEFINITION
 *
 *              Author   : LEE SANG HO
 *              Section  : IPAS Project
 *                 SCCS ID  : %W%
 *                 Date     : %G%
 *                          Revision History :
 *                                  '03.  1.  15 Initial
 *
 *                                     Description:
 *
 *                                        Copyright (c) ABLEX 2001
 *                                        ***********************************************************/

#ifndef _DEFINE____________H
#define _DEFINE____________H

#include <string.h>

#define MAX_SW_COUNT 		1024
#define MAX_VERINFO_LEN    	16

/******* CHSMD INFO ********/
#define SW_COM_IDX      0
#define HW_ETH_IDX      1

#define BSM_CHNL_IDX    2
#define MASS_CHNL_IDX   3

#define LOAD_CPU_IDX    4
#define LOAD_MEM_IDX    5
#define LOAD_QUE_IDX    6
#define LOAD_DISK_IDX   7
#define NET_LINK_IDX	8
/* ADD BY JHP #20 START(MMCD) 2008.01.08 */
#define LOAD_PKT_IDX    20

/* ADDED BY LDH 2005.11.02 */
#define HW_FAN_IDX      8
#define HW_PWR_IDX      9
#define HW_DSK_IDX      10

#define DHUB_CHNL_IDX   11
#define NTAF_CHNL_IDX   12
#define LOAD_DB_IDX     13
#define DB_CONN_IDX     14


#define MAX_STATUS_COM  256
#define MAX_HW_COM      8
#define MAX_LOAD_COM    40  /* CHANGED BY UAMYD0603 20050812 : 8->40 */
#define MAX_DB_COM      32  /* ADDED BY UAMYD0603 20050812 */
#define MAX_SW_COM      1024



#define DEF_CPU_ALM_CNT     3


#define PROC_NAME_LEN   16
#define PROC_DIR_LEN    128


#define DEF_MAX_IDX     16


/**** ucStatus values ********/
#define     DEAD            0
#define     ALIVE           1

#define     ERROR           50
#define     READY           51

#define     SET_STOP        128
#define     SET_UNSTOP      129
#define     SET_START       130

#define     LOAD_ST         180
#define     LOAD_INFO       255

/* DEVICE TYPE */
#define     DEF_PWRA        0x00
#define     DEF_PWRB        0x01
#define     DEF_SR1         0x02
#define     DEF_HR1         0x03
#define     DEF_SVR         0x04
#define     DEF_HUB         0x05
#define     DEF_SR2         0x06
#define     DEF_HR2         0x07
#define     DEF_FAN         0x08
#define     DEF_DTAFPWRA    0x09
#define     DEF_DTAFPWRB    0x0A
#define     DEF_DTAF        0x20
#define     DEF_DTAFC       0x21
#define     DEF_EDFC        0x22
#define     DEF_BDF         0x23


#define DEF_TYPENAME  		20
#define MAX_ALMTYPE   		7

#define CURR_LOAD_CPU		1
#define CURR_LOAD_MEM		1
#define CURR_QUE_COM		1

#define END					0


#define NOT_INIT			0x00
#define INIT				0x01

#define NOSTOP				0x00
#define STOP				0x01

#define UNMASK				0x00
#define MASK				0x01

#define NORMAL				0x00
#define MINOR				0x01
#define MAJOR				0x02
#define CRITICAL			0x03


/* DEFINE LOG LEVEL */
#define LOG_NOPRINT         0
#define LOG_CRI             1
#define LOG_WARN            2
#define LOG_DEBUG           3
#define LOG_INFO            4
#define LOG_TEST			5

#define LOG_TYPE_DEBUG		1
#define LOG_TYPE_WRITE		2

/* ADD BY HWH */
#define DEF_FILLTEROUT_OFF		0x00
#define DEF_FILLTEROUT_ON		0x01
#define DEF_FILLTEROUT_RPASS	0x02
#define DEF_QUD_SND_TIME		5


#define DEF_MAXLEN_TS_NAME      48
#define DEF_MAX_TS_NAME			10
#define DEF_MAX_TS				30


#define DEF_STAT_TIME			300
#define TIME_OUT				25


#define DEF_GATHER_DATA			0
#define DEF_SEND_DATA			1

#define DEF_DISCONNECT			0
#define DEF_CONNECT				1

#define CHANNEL_CONF			"/SERVERAPP/DATA/CHANNEL_STS.conf"

#endif
