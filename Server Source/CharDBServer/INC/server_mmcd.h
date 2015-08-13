#ifndef _CON_DEF_H_
#define _CON_DEF_H_

#include <time.h>
#include <server_sock.h>

/* new MMCD */
#define LOGIN           1   /* success login */
#define INITLOGIN       0   /* don't try login */
#define NOLOGIN         -1  /* failed login */

/* MMCD FLAG */
#define MMCD_BINARY_MODE    0x00000000
#define MMCD_TEXT_MODE      0x00000001

#define DIS_ALM_LOAD 		1500
#define SET_ALM_LOAD    	1501
#define DIS_LOG_LVL    		1502
#define SET_LOG_LVL     	1503
#define DIS_PRC         	1504
#define ACT_PRC         	1505
#define DACT_PRC        	1506
#define DIS_LOAD_STS    	1507
#define AD_LOGIN        	1508
#define GM_LOGIN        	1509
#define DIS_ADMIN_LIST  	1510
#define DIS_ADMIN_INFO  	1511
#define ADD_ADMIN_INFO  	1512
#define DEL_ADMIN_INFO  	1513
#define MOD_ADMIN_INFO  	1514
#define DIS_PING_TIME		1520
#define CHG_PING_TIME		1521
#define DIS_GAP_TIME		1522
#define DIS_CUR_USER		1523
#define DIS_NET_STS			1524
#define DIS_NET_USE			1525
#define MOD_ADMIN_NOTI		1526
#define MOD_ADMIN_BROAD		1527
#define MOD_BILL_BROAD 		1528
#define SRCH_USER_INFO 		1529

#define ALIVE_REQ			1530
#define ALIVE_RESP			1531

#define DIS_NATION_INFO		1533
#define CHG_NATION_INFO		1534

#define MMCD_HEADER_LEN sizeof(st_MMCDHdr)

typedef struct _st_MMCDHdr
{
	UINT	uiMmlID;
	UINT	uiMmlSEQ;
	INT		dResult;
	UINT	uiFlag;
} st_MMCDHdr, *pst_MMCDHdr;

/* 2009. 10.19 jdkim add ---> */
typedef struct _st_GMNotice
{
	UINT 	uiMsgFlag;
	UINT 	uiChlNo;
	UINT 	uiReserved1;
	UINT 	uiMsgCnt;
	UINT 	uiMsgSize;
	UINT 	uiReserved2;
	UCHAR 	szMsg[256];
} st_GMNotice, *pst_GMNotice;
/*<---*/

/* 2009. 10.22 jdkim add ---> */
typedef struct _st_GMMsg
{
	UCHAR szMsg[256];
} st_GMMsg, *pst_GMMsg;

typedef struct _st_GMBDInfo
{
	UINT    uiMsgFlag;
	UINT    uiChlNo;
	UINT    uiReserved1;
	UINT    uiMsgCnt;
	UINT    uiSendPos;
	UINT    uiReserved2;
	TIME_T64 tInsTime;
	st_GMMsg stGMMsg[10];
} st_GMBDInfo, *pst_GMBDInfo;

typedef struct _st_GMBillbd
{
	st_GMBDInfo stGMBDInfo[10];
} st_GMBillbd, *pst_GMBillbd;

/* GM ELECTRIC BILL BOARD FOR SEND */
typedef struct _st_SendBroad
{
	UINT    uiMsgFlag;
	UINT    uiChlNo;
	UINT    uiReserved1;
	UINT    uiMsgCnt;
	UINT    uiMsgSize;
	UINT    uiReserved2;
	UCHAR 	szMsg[128];
} st_SendBroad, *pst_SendBroad;
/*<---*/

typedef struct _st_MsgTag
{
	UINT64  llKey;
	INT     dTimeSec;
	INT     dReserved;
} st_MsgTag, *pst_MsgTag;

typedef struct _st_AdminHdr
{
	UINT	uiCount;
	UINT	uiReserved;
} st_AdminHdr, *pst_AdminHdr;

typedef struct _st_Admin
{
	CHAR    szAdminID[16];
	CHAR	szAdminPW[16];
	UINT    uiAdminLevel;
	UINT    uiLastLoginTime;
	UINT    uiCurrentStatus;
	UINT    uiLoginIPAddr;

} st_Admin, *pst_Admin;

typedef struct _st_AdminMod
{

	CHAR    szAdminID[16];
	CHAR    szAdminPW[16];
	UINT    uiAdminLevel;
	UINT    uiReserved;

} st_AdminMod, *pst_AdminMod;

typedef struct	_st_LogResp
{
	UINT	uiCount;
	UINT	uiReserved;
	UCHAR	szProcName[16];
	UCHAR	szLogLevel[8];
} st_LogResp, *pst_LogResp;


typedef struct _st_DisLoadSts
{

	FLOAT   fCPU_USER;
	FLOAT   fCPU_SYS;
	FLOAT   fCPU_NICE;
	FLOAT   fCPU_IDLE;
	FLOAT	fCPU_PER;

	INT   	dMEM_TOTAL;
	INT   	dMEM_USED;
	FLOAT   fMEM_PER;

} st_DisLoadSts, *pst_DisLoadSts;

typedef struct _st_SysLoad
{
	INT dCPULoad;
	INT dMEMLoad;
	INT dDISKLoad;
	INT dQUEUELoad;
	INT dPACKLoad;
	INT dReserved;
} st_SysLoad, *pst_SysLoad;

typedef struct _st_PingTime
{
	UINT	uiCheckTimeOut;
	UINT	uiSynTimeOut;
	UINT	uiIdleTimeOut;
	UINT	uiReserved;
} st_PingTime, *pst_PingTime;

/* RMI */

typedef struct _st_DisPrcResp
{
	CHAR	szProc[16];
	UINT	uiProcID;
	UINT	uiReserved;
	CHAR	szStatus[16];
} st_DisPrcResp, *pst_DisPrcResp;


/* old MMCD */

#define NUMOFCPU    			1
#define NUMOFNET    			6
#define NUMOFDISK   			10
#define NUMOFLOAD     			15

#define PROC_CHECK_LIMIT        2
#define STS_CHECK_LIMIT         5	/* chagned by uamyd 050627 : 2->5, used in CHSMD only */
#define FORK_CHECK_LIMIT        180
#define LOOF_COUNT          	250
#define QUEUE_COUNT        		15
#define INTERTIME   			10
#define MAX_OLD_COM     		100
#define SMSNAME_FILE            "SMSNAME"
#define MML_PV_MC               0
#define Two2Int(a)        		(((a)[0] - '0') * 10 + (a)[1] - '0')
#define MAX_PARSTR_LEN          64
#define DEF_REQUERY_TIME    	300     /* UPDATE DATA INTERVAL TIME */
#define DEF_MMCDHEAD_SIZE       sizeof(st_MmcdHead)
#define CLIENT_CHECK_TIME   	60
#define SECOND_FOR_DELAY    	3
#define MAX_ISTR_LEN    		512
#define MAX_ENUM_TBL    		32
#define DAY_TIME_T              86400
#define WEEK_TIME_T             604800
#define MAX_DAY_RECORD_CNT      50
#define MAX_ROW_CNT             15
#define MAX_TOT_RECORD          2000
#define MAX_RECORD_SIZE         100
#define MAX_HELP_STRING    		2500    /* 실제보다 10작은 값으로 설정 */
#define MAX_HELP_ALINE     		60
#define ERROR_PRINT_HANDLE      "MH_ErrMess"


/* Variable Type : 변경시 주의바람
 cmd_load.c에 상응하는 string있음 */

#define VT_DECSTR   0
#define VT_DECIMAL  1
#define VT_STRING   2
#define VT_ENUM     3
#define VT_HEXA     4
#define VT_HEXSTR   5
#define VT_HTIME    6
#define VT_YTIME    7
#define VT_NONABS   8
#define VT_EPOS     9
#define VT_LONGDECIMAL  10
#define VT_MTIME    11
#define VT_NUM_TYPE 12


#define ENUM_LIST_PATH          START_PATH"/DATA/SERVER_ENUM"
#define SO_LIB_PATH         	START_PATH"/BIN/libsmsprn.so"
#define DEFAULT_MMC_FUNC    	"P_default_function"
#define MMC_LOG             	START_PATH"/DATA/MMC"
#define DEF_HIS_LOG_PATH        START_PATH"/HIS/"
#define COMMAND_FILE_PATH       START_PATH"/DATA/SERVER_COM"
#define DEF_ADMIN_LIST_FILE     START_PATH"/DATA/ADMIN_LIST.dat"
#define DEF_ADMIN_TEMP_FILE     START_PATH"/DATA/ADMIN_LIST.tmp"
#define DEF_ADMIN_TMP1_FILE     START_PATH"/DATA/ADMIN_LIST.tmp1"
#define SYS_CONFIG_FILE         START_PATH"/DATA/sysconfig.dat"
#define ALMCLASS_FILE           START_PATH"/DATA/AlmClass"
#define MASK_DATA_FILE          START_PATH"/DATA/MASK_DATA"
#define MCInit_FILE             START_PATH"/DATA/PROCESS_LIST.conf"
#define DEF_MASK_FILE           START_PATH"/DATA/MASK.dat"
#define DEF_INH_MSG_FILE   		START_PATH"/DATA/INHMSG_LIST.dat"
#define DEF_BDF_FILE			START_PATH"/DATA/BDF_CFG"
#define DEF_FLT_FILE            START_PATH"/DATA/FLT_INFO"
#define FILE_NAME 				"/etc/mnttab"
#define DEF_BDFCHNL_FILE		START_PATH"/DATA/BDFCHNL_CFG"
#define DEF_DBCHK_FILE			START_PATH"/DATA/DB_CHK"
#define DEF_TIMEOUT_FILE		START_PATH"/DATA/timer.dat"
#define DEF_LOGLVL_FILE			START_PATH"/DATA/LOGLVL.dat"


/* FLT */
#define FLT_TMF_FILE         	START_PATH"/DATA/FLT/FLT_TMF"
#define FLT_DEST_FILE         	START_PATH"/DATA/FLT/FLT_DEST"
#define FLT_ALMLVL_GTAM         START_PATH"/DATA/FLT/FLT_ALMLVL_GTAM"
#define FLT_ALMLVL_NTAM         START_PATH"/DATA/FLT/FLT_ALMLVL_NTAM"
#define FLT_ALMLVL_NTAF         START_PATH"/DATA/FLT/FLT_ALMLVL_NTAF"

#define FLT_COMM_FILE         	START_PATH"/DATA/FLT/FLT_COMM"
#define FLT_LOGLVL_FILE        	START_PATH"/DATA/FLT/FLT_LOGLVL"
#define FLT_MNIP_FILE         	START_PATH"/DATA/FLT/FLT_MNIP"
#define FLT_HTTPINFO_FILE       START_PATH"/DATA/FLT/FLT_HTTPINFO"
#define FLT_USERINFO_FILE       START_PATH"/DATA/FLT/FLT_USERINFO"
#define FLT_SVCINFO_FILE       	START_PATH"/DATA/FLT/FLT_SVCINFO"

#define MAX_COM_SYMBOL          512
#define MAX_ENUM_LIST_ELEM      2048

#define MAX_PARA_SYMBOL         2048
#define MAX_STRING_BUFFER 		262144  /* 256K User String Area */

#define MAX_FDSET2      		100
#define LISTEN_PORT_NUM2 		16

#define MAX_LIB_TABLE           10000
#define MAX_HIS         		21
#define MAX_MNG_PKT_BUFSIZE     1000000

typedef struct _st_MSGBUF
{
	int     dWidx;
	int     dRidx;
	int     dFullTag;
	int     dReserv;
	char    szBuf[MAX_MNG_PKT_BUFSIZE];
} st_MSG_BUF, *pst_MSG_BUF;


#define MAX_SENDBUF_LEN 204800
typedef struct _st_ConTbl
{
	char   cSockBlock;
	char   cRetryCnt;
	char   Reserv[6];
	int    dSfd;
	unsigned int uiCliIP;
	unsigned int uiCheckTime;
	int     dBufLen;
	char    adminid[16];
	char    szSendBuffer[MAX_SENDBUF_LEN];
} st_ConTbl, *pstConTbl;


#define MAX_USER_NAME_LEN    24
typedef struct _st_MmcdHead
{
	short				sResult;
	unsigned short		usSrcProc;
	unsigned short		usTotPage;
	unsigned short		usCurPage;
	unsigned char		ucmmlid[2];
	unsigned short		usStatFlag;
	unsigned char		ucBinFlag;
	unsigned char		ucUserLevel;
	char				TimeStamp[40];
	char				userName[MAX_USER_NAME_LEN];
	unsigned short      usSysNo;

	char                szSystemName[16];

} st_MmcdHead, *pst_MmcdHead;

typedef struct _st_MngHead
{
	st_SockHeader		stSockHead;
	st_MmcdHead			stMmcdHead;
} st_MngHead, *pst_MngHead;

#define MNG_PKT_HEAD_SIZE       sizeof(st_MngHead)

#define MAX_MNGPKT_BODY_SIZE 8000
typedef struct _st_MngPkt
{
	st_MngHead  head;
	char        data[MAX_MNGPKT_BODY_SIZE];
} st_MngPkt, *pst_MngPkt;


#define MMLCONTENTS                            64
typedef struct
{
	unsigned short     	para_id;
	unsigned short     	para_type; /* VT_DECIMAL,VT_STRING,VT_ENUM,VT_YTIME(2003091511) */
	char        		para_cont[MMLCONTENTS]; /* values of parameter */
} mml_body;


#define MMLNUMOFPARA                           16
typedef struct
{
	unsigned short     	src_proc;
	unsigned short     	src_func;
	unsigned short     	dst_proc;
	unsigned short     	dst_func;
	unsigned short     	msg_id; /* M Code value(command key value) */
	/* dbm_msg_t.head.msg_id=mml_msg.msg_id*/
	unsigned short     	cmd_id; /* dbm_msg_t.head.cmd_id = mml_msg.cmdid */
	unsigned short     	msg_len; /* strlen(msg_body) */
	unsigned short     	dummy;
	char        		adminid[24];

	unsigned short     	num_of_para; /* Number of parameter */
	unsigned short     	eom_mark;

	unsigned char      	szSystemName[14];
	unsigned short     	usSysNo;
	unsigned short		usReserved[2];

	mml_body    msg_body[MMLNUMOFPARA];
} mml_msg;


typedef struct _st_INHMSG
{
	unsigned int  uiCount;
	unsigned int  uiReserv;
	unsigned char ucIDF;
	unsigned char ucReserv;
	char szCODE[6];
} st_INHMSG, *pst_INHMSG;

#define MAX_INH_MSG 32

typedef struct _st_INHMSGList
{
	int dCount;
	int dRerved;
	st_INHMSG  stList[MAX_INH_MSG];
} st_INHMSGList, *pst_INHMSGList;


typedef struct ixpc
{
	short      src_proc;
	short      src_func;
	short      dst_proc;
	short      dst_func;
	short      msg_id; /* dbm_msg_t.head.msg_id = mml_msg.mag_id, mml_msg의 msg_id 값을 대입 */
	short      cmd_id; /* dbm_msg_t.head.cmd_id = mml_msg.cmd_id, mml_msg의 cmd_id 값을 대입 */
	short      msg_len; /* size of dbm_msg_t.data */
	short      dummy;
} IXPC_HEAD;

typedef struct
{
	short   mml_err;    /* 명령어 실행시 error code를 나타내는 필드 ,  성공의 경우 DBM_SUCCESS 샤용 */
	/* 실패의 경우 error_code.h 에 사용값 정의 */
	short   cont_flag;  /* 1 : continue, 0 : end 여부  사용값 : CONTINUE, END*/
	short   curr_cnt;   /* 현재 return되는 통계수집 횟수 */
	/* 현재 RECORD의 개수 */
	short   total_cnt;  /* 통계수집 총 횟수 */
	/* 전체 RECORD의 개수 */
	short   time_out;
	short   TotPage;    /* PAGE단위의 전체 PAGE : 하나의 통계 DATA에 대한 */
	short   CurPage;    /* PAGE단위의 현재 PAGE : 하나의 통계 DATA에 대한 */
	short   StatFlag;
} MML_COMMON;

#define MSG_DATA_LEN                           8000
typedef struct
{
	IXPC_HEAD head;
	MML_COMMON common;
	char    data[MSG_DATA_LEN];
} dbm_msg_t;


#define            USER_NAME_LEN           16
#define            USER_PASS_LEN           16
#define            MAX_USER_CNT            200

typedef struct
{
	char        szUserName[USER_NAME_LEN];
	char        szUserPass[USER_PASS_LEN];
	int         dUserLevel;
	int         dIPFlag;
	time_t      tLastLogin;
	time_t      tLastLogout;
	unsigned int        lLastLoginIP;
	unsigned int        lRegIP;
} T_ADMIN_DATA;

typedef struct
{
	unsigned short  usUseFlag;
	unsigned short  usTotalCnt;
	int             dReserved;
	int             dConnectFlag[MAX_USER_CNT];
	T_ADMIN_DATA    stUserList[MAX_USER_CNT];
} T_ADMIN_LIST;

typedef struct _st_User_Info_
{
	char            szUserName[MAX_USER_CNT][USER_NAME_LEN];
	char            szUserPass[MAX_USER_CNT][USER_PASS_LEN];
	short           sUserLevel[MAX_USER_CNT];
	time_t          tLastLogin[MAX_USER_CNT];
	unsigned int    uiLoginIP[MAX_USER_CNT];
} st_User_Info, *pst_User_Info;

typedef struct _st_Cmd_Info_
{
	char            szUserName[MAX_TOT_RECORD][USER_NAME_LEN];
	unsigned int    tTime[MAX_TOT_RECORD];
	short           sResult[MAX_TOT_RECORD];
	char            szCommand[MAX_TOT_RECORD][256];
} st_Cmd_Info, *pst_Cmd_Info;

#define  MAX_TMR_REC            128

typedef struct
{
	long long       llIndex[MAX_TMR_REC];
	short           exe_proc[MAX_TMR_REC];
	short           cmd_id[MAX_TMR_REC];
	short           msg_id[MAX_TMR_REC];
	short           sfd[MAX_TMR_REC];
	char            time_stamp[MAX_TMR_REC][40];
	char            user_name[MAX_TMR_REC][24];
	short           inv_proc[MAX_TMR_REC];
	short           time[MAX_TMR_REC];          /*** 0이상이면 현재 실행중인 명>령 ***/
	char            type[MAX_TMR_REC][12];
	short           period[MAX_TMR_REC];
	short           stat_TOTAL[MAX_TMR_REC];    /*** 통계 결과의 전체 자료의 개>수 ***/
	short           stat_TOT_NUM[MAX_TMR_REC];  /*** 통계 결과의 변하지 않는 전>체 개수 ***/
	short           stat_tot_cnt[MAX_TMR_REC];  /*** 하나의 통계 자료에 대한 전>체 RECORD 수 ***/
	short           stat_cur_cnt[MAX_TMR_REC];  /*** 하나의 통계 자료에 대한 현>재 RECORD 수 ***/
	short           stat_tot_page[MAX_TMR_REC]; /*** 하나의 통계 자료에 대한 전>체 PAGE 수 ***/
	short           stat_cur_page[MAX_TMR_REC]; /*** 하나의 통계 자료에 대한 현>재 PAGE 수 ***/
	unsigned short  usmmlid[MAX_TMR_REC];
	short           blockcode[MAX_TMR_REC];     /*** 해당 프로세스의 Message Queue ID ***/
	unsigned char   ucbinflag[MAX_TMR_REC];
	time_t          stat_check_time[MAX_TMR_REC];   /*** STAT CHECK TIME ***/
	mml_msg         stMML_MSG[MAX_TMR_REC];
	char            szCommandString[MAX_TMR_REC][256];
	TIME_T64		tRegTime[MAX_TMR_REC];
	TIME_T64		tActionStartDate[MAX_TMR_REC];
	TIME_T64		tActionEndDate[MAX_TMR_REC];
	UCHAR			szComment[MAX_TMR_REC][64];
	UINT64			ullGameID[MAX_TMR_REC];
	UINT			dResult[MAX_TMR_REC];
	UCHAR			szCommand[MAX_TMR_REC][64];
} RUN_TBL;

typedef struct _st_BINRUN_TBL
{
	TIME_T	tActionStartDate;
	TIME_T	tActionEndDate;
	TIME_T	tExTime;
	USHORT	usMsgID;
	SHORT	sFd;
	USHORT	usMmlID;
	USHORT	usReserved;

	UINT    dResult;
	UINT	uiReserved;

	UINT64	ullCmdSeq;

	char 	szUserName[24];
	UINT64  ullGameID;
	UCHAR   szCommand[64];
} st_BINRUN_TBL, *pst_BINRUN_TBL;

typedef struct _st_BINDATA
{
	USHORT	usMmlID;
	USHORT	usSysNo;
	INT		dResult;
	UINT64	ullCmdSeq;
} st_BINDATA, *pst_BINDATA;


typedef struct
{
	int  mcode;
	int  block;
	void (*mmc_res) (char *, void *, short *, short * );
	char *msg_header;   /* MMC  RESULT의 HEAD PRINT용 */
	char *com_name;
} LIB_TBL;


typedef struct
{
	char   name[32];
	char   value[64];
} In_Arg;



typedef struct
{
	int         idx;
	int         type;
	long       def_value;
	long       start;
	long       end;
	char        *para_name;
	char        *def_name;
} PARA_CONT;


#define MAX_PAR_IN_COM            24
typedef struct
{
	char    *com_name;
	char    *help[3];       /* HELP string */
	int     code;           /* Command index */
	int     dest;           /* Block Name */
	char    access;         /* 접근 코드 : 사용자 등급 */
	char    p_man;          /* 필수 파라메터 갯수 */
	char    p_opt;          /* 사용하지 않음; num_para - p_man */
	char    num_para;       /* 파라메터 전체 갯수 */
	PARA_CONT para[MAX_PAR_IN_COM];
} COM_TBL;


typedef struct
{
	char        *para_name;
	char        *para_help;     /* Parameter에 대한 상세 설명 */
	long        code;
} PARA_TBL;


typedef struct
{
	int     code;
	char    *para_name;
} tENUMLIST;


typedef struct _st_HisData
{
	char        szUserID[16];
	time_t      tTime;
	int         dResult;
	char        szCommand[256];
} st_HisData, *pst_HisData;


typedef struct
{
	char    szSpecial[50];
	char    szMountp[20];
	int     dPercent;
	long long   llUsed;
	long long   llTotal;
} st_Soldf;

typedef struct
{
	int         dCount;
	st_Soldf    stSoldf[20];
} st_SoldfList;

typedef struct
{
	int     number;
	char    cmd[256];
} his_t;


typedef struct
{
	char    enum_name[16];
	int     start;
	int     end;
} ENUM_TBL;

typedef struct _st_Stat_Info
{
	time_t	tStatTime;
	UINT    uiIPAddr;
	USHORT  usPort;
	UINT    uiUpSize;
	UINT    uiDownSize;
	UINT    uiPktCnt;
	UINT    uiCurSize;
	UINT    uiTotalPktCnt;
	UINT    uiTotalSize;
} st_StatInfo, *pst_StatInfo;

typedef struct _st_PROCESSLIST_HEADER
{
	UINT    uiProcCnt;
	UINT    uiReserved;
} st_PROCESSLIST_HEADER, *pst_PROCESSLIST_HEADER;

typedef struct _st_PROCESSLIST_BODY
{
	CHAR    szProcName[16];
	UINT    uiSeq;
	UINT    uiReserved;
} st_PROCESSLIST_BODY, *pst_PROCESSLIST_BODY;

typedef struct _st_GMToolLog_Insert
{
	UCHAR		szMMCDID[8];
	TIME_T64	tRegDate;
	UINT		uiMsgID;
	INT			dResult;
	UINT64		ullGameID;
	TIME_T64	tActionStartDate;
	TIME_T64	tActionEndDate;
	UCHAR		szCmd[64];
	UCHAR		szComment[64];
} st_GMToolLog_Insert, *pst_GMToolLog_Insert;
#endif
