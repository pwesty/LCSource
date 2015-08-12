#ifndef __NTAF_SHM_HEADER_FILE__
#define __NTAF_SHM_HEADER_FILE__

#include <time.h>
#include <define.h>
#include <typedef.h>
#include "server_names.h"

#define NO_MSG				0
#define NOMSG           	0

/* write shm */
#define Err_BUF_FULL		-200
#define Err_BUF_FULL2		-201

/* read shm */
#define Err_BUF_EMPTY		-202
#define Err_CRITICAL		-203
#define Err_POINTER			-204

typedef struct
{
	double idle;
	double wait;
	double user;
	double syst;
} T_CPU_LOAD;

typedef struct
{
	double phiscal;
	double free;
} T_MEM_LOAD;

/***
#define ALIVE		0x03
#define DEAD		0x02
#define ABSENT		0x00
#define MASKED		0x01
#define UNMASKED	0x00
#define NORMAL		0x00
#define MINOR		0x01
#define MAJOR		0x02
#define CRITICAL	0x03
#define INITIAL		0x01
#define LISTEN		0x01

#define STOP		0x01
#define NONSTOP		0x00
***/

/* STAT */
#define DEF_TCP_TIMEOUT	300
#define DEF_PPP_TIMEOUT	300

#define MAX_LOAD_SIZE	6
#define MAX_LINK_COUNT	2
#define MAX_DISK_COUNT	2


#if 0
/* KEEPALIVE DEFINE */
#define     READY       0
#define     START       1
#define     FINISH      2
#define     PROCESSING  3
#define     BROKEN  	-1
#endif

/*
	LOAD 상태 관리를 위한 구조체
		alarm : NORMAL, MINOR, MAJOR, CRITICAL
		mask  : MASK, UNMASK
	 	usage :  DISK 사용율(%)[xx.xx]
*/

typedef struct
{
	char  		alarm;
	char		mask;
	char		load[MAX_LOAD_SIZE];
	long long   used;
	long long   total;
} T_MP_LOAD;


/*
	ETHERNET PORT 상태 관리를 위한 구조체
		status :  ALIVE, DEAD, ABSENT
		alarm  :  NORMAL, CRITICAL
		mask   :  MASK, UNMASK
*/

typedef struct
{
	char	status;
	char	alarm;
	char	mask;
	char	reserved;
} T_MP_LINK;



/*
	Process 상태를 관리하기 위한 구조체
		status 	:  	ALIVE, DEAD, ABSENT
		alarm  	:  	NORMAL, MINOR, MAJOR, CRITICAL
		mask	:  	MASK, UNMASK
		stop	:	STOP, NONSTOP
		when	: 	프로세스 상태 변경 시간
		pid		:  	pid
*/

typedef struct
{
	char	status;
	char	alarm;
	char	mask;
	char	stop;
	time_t	when;
	int		pid;
} T_MP_SW;



/*
	Process 상태를 전송하기 위한 구조체
		status 	:  	ALIVE, DEAD, ABSENT
		alarm  	:  	NORMAL, MINOR, MAJOR, CRITICAL
		mask	:  	MASK, UNMASK
		stop	:	STOP, NONSTOP
		when	: 	프로세스 상태 변경 시간
*/

typedef struct
{
	char	status;
	char	alarm;
	char	mask;
	char	stop;
	time_t	when;
} T_MP_SW_ALMD;


/*
	DTAF 시스템 전체를 관리하기 위한 정보 테이블
		alarmlevel 	:  	시스템 전체 장애 등급
		mpmqful		:	IPC MESSAGE QUEUE FULL 상태 관리
		mpcpuload   :	CPU 부하율
		mpmemload	:	MEM 부하율
		mpdisk		:	DISK 부하율 정보
		mplink		:	ETHERNET 상태 정보
		mpsw		: 	PROCESS 상태 정보
*/

typedef struct
{
	char  		alarm;
	char		mask;
	char		mpmqful;
	char 		reserved;
	long		pid;
} T_MP_MQ;

typedef struct
{
	short  sDtafNo;
	short  sLineNo;
	short  sSvcNo;
	short  sLineInfo;
} T_MP_DTAF;

typedef struct
{
	char 		alarmlevel;
	char		stop;
	char		mask;
	char		status;
	char		reserved[4];
	T_MP_DTAF	mpdtaf;
	T_MP_LOAD	mpcpu;
	T_MP_LOAD	mpmem;
	T_MP_LOAD	mpdisk;
	T_MP_LINK 	mpethlink[MAX_LINK_COUNT];
	T_MP_SW		mpsw[MAX_SW_COUNT];
} T_TMF_STATUS;

typedef struct
{
	char 			alarmlevel;
	char			stop;
	char			mask;
	char			status;
	char			reserved[4];
	T_MP_DTAF		mpdtaf;
	T_MP_LOAD		mpcpu;
	T_MP_LOAD		mpmem;
	T_MP_LOAD		mpdisk;
	T_MP_LINK 		mpethlink[MAX_LINK_COUNT];
	T_MP_SW_ALMD	mpsw[MAX_SW_COUNT];
} T_TMF_STATUS_ALMD;


/*
	Process 수행 여부를 진단하기 위한 Heart Beat 처리용 구조체
*/

typedef struct
{
	long   minor;
	long   major;
	long   critical;
	long   reserved;
} T_AlarmLoad;

typedef struct
{
	long    oldcnt[MAX_SW_COUNT];
	long    cnt[MAX_SW_COUNT];
	short   aply_status;
	short   dtaf_status;
	short   Reserved1;
	short   Reserved2;
	time_t  inittime;
	time_t  resvtime;
	T_AlarmLoad  cpu;
	T_AlarmLoad  mem;
	T_AlarmLoad  disk;
	char	procnorm[MAX_SW_COUNT];
} T_Keepalive;

/* MRG_SHM : OLD
#define	MRG_READER_COUNT	1
#define MRG_MEM_SIZE   		32000000
#define MRG_MEM_COUNT  		3
*/

/***** MODIFIED BY LEE : 20040303 *****/
#define MRG_READER_COUNT   	1
#define MRG_MEM_SIZE        96000000

#define MRG_READER_ANA      0
#define MRG_READER_SVCANA   1


// Shared memory structure - MRG
typedef struct
{
	unsigned char	Mem[MRG_MEM_SIZE];
} T_CAPBUF, *PT_CAPBUF;

#define DEF_TCAPBUF_SIZE    sizeof(T_CAPBUF)

T_CAPBUF                *shm_capbuf;

typedef struct
{
	long    WritePos;
	long    WriteEndPos;
	long    ReadPos[MRG_READER_COUNT];
} T_CAP, *PT_CAP;

#define DEF_TCAP_SIZE       sizeof(T_CAP)

#define	SHM_EMPTY		0x00
#define	SHM_WRITE		0x01
#define	SHM_FINISH		0x02

#define MAX_STAT_SIZE	12

/* 040114,poopee */
#define DEF_VERSION_SIZE        7       /* Inserted By LSH in 2004.04.05 */

typedef struct _st_SW_Info
{
	unsigned int    uiPid;    /* 실행중인 프로세스의 PID */
	unsigned char	ucActFlag; /* 실행 여부를 판단하는 flag, McInit수정후 StartMC로 적용 */
	unsigned char	ucDoneFlag;
	unsigned char	szReserved[2];
	char            szProcName[PROC_NAME_LEN];
	char            szVersion[MAX_VERINFO_LEN];
} st_SW_Info;

typedef struct _st_VERSION
{
	char	ver[NTAF_SW_COM][DEF_VERSION_SIZE];
} st_VERSION;


typedef struct _st_Threshold
{
	unsigned int    Norm;
	unsigned int    Warn;
	unsigned int    Cri;
	unsigned int    Reserv;
} st_Threshold;

typedef struct _st_LOAD_Info
{
	st_Threshold    stThreshold;
	unsigned int    uiCurload;
	unsigned int    uiReserved;
} st_LOAD_Info;

typedef union _un_Status
{
	unsigned char   ucSts;
	struct
	{
		unsigned char   sts:2;      /* 0 : Normal 1 : Warning 2 : Critical */
		unsigned char   mask:2;     /* 0: unmask, 1:mask */
		unsigned char   stop:2;     /* 0: alive, 1:stop(passive) */
		unsigned char   init:2;     /* 0: not initial, 1:initial; */
	} stSts;
} un_Status, *pun_Status;

typedef struct _st_Ts_Info_
{
	char    szTsName[DEF_MAX_TS][DEF_MAXLEN_TS_NAME];
	long    lTotal[DEF_MAX_TS];
	long    lUsed[DEF_MAX_TS];
	long    lFree[DEF_MAX_TS];
	int     dPercent[DEF_MAX_TS];
} st_Ts_Info, *pst_Ts_Info;


typedef struct _st_Fidb
{
	int             dTotCompCnt;
	int             tChsmdWhen;
	int             fDBConnect;
	USHORT			cCurrCnt[DEF_MAX_IDX];
	char            cReserved[2];
	/* 0:SW_COM, 1:HW_ETH, 2:LOAD_CPU, 3:LOAD_MEM, */
	/* 4:LOAD_QUE, 5:LOAD_DISK */
	un_Status       stStatus[MAX_STATUS_COM];       /* MAX_STATUS_COM */
	int             stStatusWhen[MAX_STATUS_COM];   /* 4*MAX_STATUS_COM */
	st_LOAD_Info    stLoadInfo[MAX_LOAD_COM];       /* 8*MAX_LOAD_COM */
	st_SW_Info      stSWInfo[MAX_SW_COM];           /* 4*MAX_SW_COM */
} st_FIDB, *pst_FIDB;

st_FIDB *fidb;


/*** ADDED BY LYH 08.01.03  FOR LOG LEVEL ***/
typedef struct _st_loglevel
{
	INT		dSeq[MAX_SW_COM];
	char	szProcName[MAX_SW_COM][16];
	char	szLevel[MAX_SW_COM];
} st_LOGLEVEL, *pst_LOGLEVEL;

st_LOGLEVEL	*loglevel;

typedef struct _st_AlmLevel
{
	char    		szTypeName[DEF_TYPENAME];
	unsigned int    sCriticalLevel;
	unsigned int    sMajorLevel;
	unsigned int    sMinorLevel;
} st_AlmLevel, *pst_AlmLevel;

typedef struct _st_AlmLevel_List
{
	int 		dCount;
	char 		reserved[4];
	st_AlmLevel stAlmLevel[MAX_ALMTYPE];
} st_AlmLevel_List, *pst_AlmLevel_List;

extern st_AlmLevel_List            *gstAlmlvl;

typedef struct _st_SYS_ALMLOAD
{
	int         dPACKLoad;
	int         dCPULoad;
	int         dMEMLoad;
	int         dDISKLoad;
	int         dQUEUELoad;
	int			dPACKDelay;
} st_SYS_ALMLOAD, *pst_SYS_ALMLOAD;

st_SYS_ALMLOAD  *sys_almload;

/**** STRUCTURE ****/
/* PACKET VERSION STRUCTURE */
typedef struct _st_PACKET_VERSION
{
	USHORT  usPacketVersion;
	UCHAR   szReserverd[6];
} st_PACKET_VERSION, *pst_PACKET_VERSION;

st_PACKET_VERSION   *stPacketVer;


/* Elenoa: 2008. 1. 18: to Check system delay */
long long *pllTemsTimestamp;

#define MAX_PORT_NUM	10
#define BASE_DUPLE		100

typedef struct _st_GEN_INFO
{
	UINT64  ullDelayMax[MAX_PORT_NUM];      // TEMS PACKET DELAY TIME(T_ECRPT)
	UINT64  ullDelayTime[MAX_PORT_NUM];     // TEMS PACKET DELAY TIME(T_ECRPT)
	UINT64  ullDelayCnt[MAX_PORT_NUM];      // TEMS PACKET DELAY COUNT(T_ECRPT)

	UINT64  ullUpPktCnt[MAX_PORT_NUM];     	// 최근 10초 동안의 Up Packet Count(BSMS)
	UINT64  ullDownPktCnt[MAX_PORT_NUM];   	// 최근 10초 동안의 Down Packet Count(BSMS)
	UINT    ullUpBytes[MAX_PORT_NUM];      	// 최근 10초 동안의 Up Packet Bytes(BSMS)
	UINT    ullDownBytes[MAX_PORT_NUM];    	// 최근 10초 동안의 Down Packet Bytes(BSMS)
	TIME_T  tGatherStartTime[MAX_PORT_NUM];	// 수집 시작 시간

	INT     dPortUser[MAX_PORT_NUM];        // TEMS 포트별 접속 유저 수(T_PINGER)

	UINT    uiExpBase;
	UINT    uiGameMoneyBase;

	USHORT  usMaxChnlUser[10];
	UINT    uiMaxRoomCnt;

	INT     dLastSendTime;
	INT     dSPingTime;
	INT     dRPingTime;
	UINT    uiCheckTimeOut;

	UINT    uiSynTimeOut;
	UINT    uiIdleTimeOut;

	UINT	uiCryptF;
	UINT	uiSystem;
	UINT	uiLocalCode;
	CHAR	szLocalName[4];

	UINT	uiReserved[12];
} st_GEN_INFO, *pst_GEN_INFO;

st_GEN_INFO *gen_info;

typedef struct _st_PACKET_DEALY
{
	UINT64  ullDelayMax[MAX_PORT_NUM];      // TEMS PACKET DELAY TIME(T_ECRPT)
	UINT64  ullDelayTime[MAX_PORT_NUM];     // TEMS PACKET DELAY TIME(T_ECRPT)
	UINT64  ullDelayCnt[MAX_PORT_NUM];      // TEMS PACKET DELAY COUNT(T_ECRPT)
	UINT64  ullDelayTimePrev[MAX_PORT_NUM];	// TEMS PACKET DELAY TIME(T_ECRP T)
	UINT64  ullDelayCntPrev[MAX_PORT_NUM];	// TEMS PACKET DELAY COUNT(T_ECR PT)
} st_PACKET_DELAY, *pst_PACKET_DELAY;

st_PACKET_DELAY *stPacketDelay;

typedef struct _st_StageLapMinimum
{
	unsigned int uiMapNo;
	unsigned int uiLapSec;
	unsigned int uiLapHourMinFix;
	unsigned int uiLapHourMin;
	unsigned int uiLapDayMinFix;
	unsigned int uiLapDayMin;
	unsigned int uiReserved;
	unsigned int uiIllegalLapCnt;
	time_t		 tLastUpdated;
	time_t tLastSummed;
} st_StageLapMinimum, *pst_StageLapMinimum;

typedef struct _st_past_UserInfo
{
	UINT64      ullGameID;
	INT64       llSockSessID;
	TIME_T64    tSessTime;

	USHORT      usSvcID;
	USHORT      usMsgID;
	UINT        uiIPAddr;
} st_past_UserInfo, *pst_past_UserInfo;

#endif
