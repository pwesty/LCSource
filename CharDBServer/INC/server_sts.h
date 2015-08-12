#ifndef __SERVER_STS__H
#define __SERVER_STS_H

#include <typedef.h>

typedef struct _st_AlmStat
{
	UCHAR	ucLocType;      /* INDEX */
	UCHAR	ucLocIdx;       /* Process Seq Num or Device seq num */
	UCHAR	ucStatus;       /* DEAD, ALIVE등의 상태, LOAD인경우 그냥 NORMAL */
	UCHAR	ucAlmLevel;     /* NTAF/NTAM 연동에 사용 FSTATD만 사용 */
	CHAR	Info[124];      /* load 관련 percentage, 오류 메시지 출력에 필요한 기타 정보를 용도에 따라 할당 */
	UCHAR	ucOldAlmLevel;  /* NTAF/NTAM 연동에 사용 FSTATD만 사용 */
	UCHAR	ucTafno;
	UCHAR	ucTamno;
	UCHAR	ucReserved;
	INT		tWhen;          /* 발생 시간 */
} st_AlmStat, *pst_AlmStat;

#define MAXTEMS_PER_SRV     5
#define CONNPNT_PER_TEMS    10
#define CONNPNT_PER_SRV     (MAXTEMS_PER_SRV * CONNPNT_PER_TEMS)
#define CHANNEL_PER_SRV     100

#define MAX_SERVERMNG_CNT   100

typedef struct _st_SrvConnMng
{
	UINT    uiSrvIPAddr;
	USHORT  usSrvPort;
	UCHAR   szReserved1[2];
	UINT    uiMaxUserCnt;
	UINT    uiCurrUserCnt;
	TIME_T	tCreateTime;
} st_SrvConnMng, *pst_SrvConnMng;

typedef struct _st_ChlStatMng
{
	UINT    uiChlNo;
	UINT    uiMaxUserCnt;
	UINT    uiCurrUserCnt;
	UINT    uiChlType;
	UCHAR   szChlName[16];
} st_ChlStatMng, *pst_ChlStatMng;

typedef struct _st_SrvStatMng
{
	UINT    uiSrvNo;
	UINT    uiMaxUserCnt;
	UINT    uiCurrUserCnt;
	UINT    uiReserved1;
	UCHAR   szSrvName[16];
	UINT    uiConnPntCnt;
	UINT    uiCurrChlCnt;
	st_SrvConnMng stSCM[CONNPNT_PER_SRV];
	st_ChlStatMng stCSM[CHANNEL_PER_SRV];
} st_SrvStatMng, *pst_SrvStatMng;

#define MAX_PORT_COUNT 20

typedef struct _st_SrvConnMng_List
{
	UINT    uiCount;
	UINT    uiIPAddr;
	UINT    uiServerNo; //TEMS NUMBER
	UINT    uiResult;

	st_SrvConnMng stSrvConnMng[MAX_PORT_COUNT];
} st_SrvConnMng_List, *pst_SrvConnMng_List;

#if 0
typedef struct _st_GameLog
{
	UINT64  ullGameID;
	UINT    uiSecTime;
	UINT    uiUSecTime;
	UINT    uiSvcID;
	UINT    uiMsgID;
	UINT    uiResult;
	UINT    uiItemID;		/* for Item Buy, Sell */
	UINT    uiItemValue;	/* for Item Buy, Sell */
	UINT    uiKeron;		/* for Item Buy, Sell, LabFund */
	UINT    uiCash;			/* for Item Buy, Sell */
	UINT	uiServerNo;
	//UINT	uiClientIPAddr;
	UCHAR	ucMode;			/* for Game End */
	UCHAR	ucTeamFlag;		/* for Game End */
	UCHAR	ucItemFlag;		/* for Game End */
	UCHAR	ucWin;			/* for Game End */
	UINT	uiExpAdd;		/* for Game End, Mission */
	UINT	uiKeronAdd;		/* for Game End, Mission */
	UINT	uiChlNo;		/* for Game End, Additional */
	UINT	uiRoomNo;		/* for Game End, Additional */
	UINT	uiStage;		/* for Game End, Additional 2 */
	/* padding clear, till here */
	UINT	uiStageReal;	/* for Game End, Additional 3 */
	UINT	uiExpAddDup;	/* for Game End, Additional 3 */
	UINT	uiKeronAddDup;	/* for Game End, Additional 3 */
	UINT	uiQuestID;		/* for Mission */
	UINT	uiMassItemID[4];	/* for Lab, Game End Reward */
	UCHAR	szCharName[16];		/* for Charname Change */
	UINT64	ullWithGameID;	/* for Couple */
	UINT 	uiRecipeID;		/* for Lab */
	UINT	uiLapMinimum;	/* for Lap Time check */
	UINT	uiLapAverage;	/* for Lap Time check */
	UINT	uiReserved;

	/* KHLEE 2009.0408 ADD */
	UINT 	uiKillCnt;
	UINT 	uiDeathCnt;
} st_GameLog, *pst_GameLog;
#endif

typedef struct _st_BillLog
{
	UINT64  ullGameID;
	UINT    uiReserved;
	UINT    uiSecTime;
	UINT    uiUSecTime;
	CHAR    szGoormID[16];
	UINT    uiSvcID;
	UINT    uiMsgID;
	UINT    uiResult;
	UINT    uiServerNo;
	time_t  tRecv;
	time_t  tSent;
	UINT    uiItemID;       /* for Item Cash Buy */
	UINT    uiItemValue;    /* for Item Cash Buy */
	UINT    uiGameMoney;    /* for Item Cash Inq, Buy */
	UINT    uiBonusGameMoney;   /* for Item Cash Inq, Buy */
} st_BillLog, *pst_BillLog;


typedef struct _st_ChnlSts
{
	UINT	uiTadPole;
	UINT	uiOneStar;
	UINT	uiTwoStar;
	UINT	uiThreeStar;
	UINT	uiFree;
	UINT	uiPlay;
	UINT	uiFourStar;
	UINT	uiPractice;
	UINT	uiReserved;
	UINT	uiServerNo;
} st_ChnlSts, *pst_ChnlSts;

typedef struct st_SystemInfo
{
	UINT	uiServerNo;
	UINT	uiSystemIndex;
} st_SystemInfo,  *pst_SystemInfo;

int dGet_CSMSConfig( pst_SystemInfo pstSI );
#endif
