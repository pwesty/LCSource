#ifndef __UTILLIB_HEADER_FILE___
#define __UTILLIB_HEADER_FILE___

#include <time.h>
#include <sys/types.h>
#include <typedef.h>
#include <server_msgq.h>

#define VALIDCHAR(c) ( ( 33 <= (c) )? 1: -1 )
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define BUF_SIZE   1024
#define TOKEN      "#@"
#define F_CONF_PARSE_ERR	-3000
#define SUCCESS        		1
#define SHMPERM        		0666
//#define IPADDR_FILE "/SERVERAPP/DATA/SERVER_ADDR.dat"
#define IPADDR_FILE "/home/SERVERAPP/DATA/SERVER_ADDR.dat"
#define DEF_GENINFO_DATA "/SERVERAPP/DATA/GEN_INFO.dat"

#define USLEEP_TIME			250
#define TIMER_USLEEP_TIME	1000

// extern st_FIDB          *fidb;
// extern st_LOGLEVEL 		*loglevel;
// extern st_GEN_INFO		*gen_info;

extern UINT        		g_uiServerNo;

typedef struct
{
	int  id;             /* field identifier        */
	int  count;          /* field count max 10      */
	char data[10][75];   /* field data max 75 chars */
} T_CONF_DATA;

typedef struct _st_LOGMSG
{
	char    szLogPath[128];
	char    szProcName[16];
	int     dPid;
	int     dReserved;
} st_LogMsg, *pst_LogMsg;

typedef struct _st_NationConf
{
	UINT	uiSystem;
	UINT	uiReserved;
	char 	szNationName[16];
	UINT	uiNationCode;
	UINT	uiCryptFlag;	//0:no 1:md5 2:sha256
} st_NationConf, *pst_NationConf;

#define DEF_LOGMSG_SIZE sizeof(st_LogMsg)


int Init_logdebug(int dSeq,  pid_t proc_idx, char *proc_name, char *logfilepath );
int Init_logbeacon( char *logfilepath );
int Init_logerror( char *logfilepath );

int log_close();
int log_debug(char *fmt, ... );
int log_write(char *fmt, ... );
int log_hexa(unsigned char *fmt, int dSize );
int log_hexa2(unsigned char *fmt, int dSize );
int log_hexa3(unsigned char *fmt, int dSize );
int log_beacon( unsigned char *puBuf, int size, int iFlag, int iRetCode, int iLogLevel );
int log_packet( unsigned char *puBuf, int size, int iFlag, int iRetCode, int iLogLevel );
int log_message( unsigned char *puBuf, int size, int iFlag, int iRetCode, int iLogLevel );
int dAppWrite( int dLevel, char *szMsg );
int dAppDebug( int dType, int dLevel, char *szMsg, struct tm *pstCheckTime, char *szName );
int dAppLog(int dIndex, char *fmt, ...);
void HEXA_DUMP(UCHAR *str, int size);

time_t convert_time_t( time_t value );
short convert_short( short value );
unsigned short convert_ushort( unsigned short value );
int convert_int( int value );

long convert_long( long value );
unsigned long convert_ulong( unsigned long value );
long long convert_llong(long long value);

int dGetHostIP();
int dGetHostIP2( char *ipaddr );
int dGetIPAddr();
int dGetIPAddr_TAS();

int dGetIPAddr2(char *ipaddr);

/* url.c */
char*   getEndTag( const char* tar, const char* end );
int     strcpy_url( char *dst, const char *src );
char*   findTopDir(const char* url, const char* endurl);
char*   getLastDir(const char* endurl, const char* topurl);
short   rearrangeUrl( const char* tarurl, const char* topdir, const char* endurl, char* returl );
int     getAbsUrl( char* szReqUrl,
				   unsigned short usReqUrlOptPos,
				   const char* szResUrl,
				   int dResUrlLen,
				   char* szResAbsUrl
				 );
int     dGetOptPos( const char* resabsurl );
int     urldecode(unsigned char *source, unsigned char *dest);
int     urlndecode(unsigned char *source, unsigned char *dest, int maxsize);
int     dMakeHashCode( const char *pszURLBuf, char *pszHash, int dLength );
char*   passInvalidChar(unsigned char* sc1, const unsigned char* endPtr );
char*   valParsing( char* sc1, char* endPtr, int* len );
char 	*strnpbrk(const char *s1, const char *s2, size_t n);
int 	set_status_running(int proc_idx);
int UnicodeToUTF8(UCHAR* szSrc, int dSrcLen, UCHAR* szDest, int dDestLen);
int UTF8ToUnicode(const CHAR *szSrc, int dSrcLen, USHORT *szDest, int dDestLen);
int get_UTF8StringLen(CHAR *pszSrc, UINT uiMax);

// GET CONNECTION SERVER IP ADDRESS
int 	dGet_ServerIP_From_File(char *szSystem, char *szIPAddr);
//int 	dSendToReplay( pst_MsgQ pstMsgQ, int qid, pst_ReplayKey pstReplayKey);

/* type_cvt.c */
char* CVT_IPADDR( UINT uiIP );
char* pTrimLeft( char* szSrc, char* szSkipChar );

/* make_nid.c */
int dMakeNID(UCHAR ucSysType, INT64 *pllNID);

/* time.c */
long long llGetMicroDelta(struct timeval *tv1, struct timeval *tv2);

int dLog_TraceBeacon_IP( unsigned char *puBuf, int size, char* szIP );
UINT GET_GAPTIME(struct timeval *t1, struct timeval *t2);

int Init_shm_common();

extern UINT         CVT_UINT(UINT );
extern USHORT       CVT_USHORT(USHORT );
extern INT64        CVT_INT64(INT64 );
extern UINT64 		CVT_UINT64( UINT64 value );

int wlog_qid;


#endif

