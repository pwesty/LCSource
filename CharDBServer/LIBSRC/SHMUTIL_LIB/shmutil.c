/*******************************************************************************
				KTF IPAF Project

   Author	: joonhk
   Section	: 
   SCCS ID	: %W%
   Date		: %G%
   Revision History :
   	'03.  1.  6	Initial


   Description :

   Copyright (c) Infravalley 2003
*******************************************************************************/

/**A.1*	FILE INCLUSION ********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <server_shm.h>
#include <utillib.h>
#include <init_shm.h>
#include <server_define.h>
#include <server_msgq.h>
#include <server_sock.h>
#include <msgdef.h>
#include <server_sts.h>

/**B.1*	DEFINITION OF NEW CONSTANTS *******************************************/

/**B.2*	DEFINITION OF NEW TYPE ************************************************/

/**C.1*	DECLARATION OF VARIABLES **********************************************/
extern	T_CAPBUF		*shm_capbuf;
extern	T_CAP			*shm_cap;
st_FIDB			*fidb;
st_GEN_INFO		*gen_info;
extern	st_PACKET_VERSION *stPacketVer;


UINT		g_uiServerNo;

#define BYTE_1_REP          0x80    /* if <, will be represented in 1 byte */
#define BYTE_2_REP          0x800   /* if <, will be represented in 2 bytes */

#define SURROGATE_MIN       0xd800
#define SURROGATE_MAX       0xdfff

#define SIGMASK_2_1         0xc0
#define SIGMASK_3_1         0xe0

/* Pre-defined  End */

int dGet_UnicodeStrlen(UCHAR* ucString, int dMaxlen)
{
    USHORT usString;
    int i = 0;
//    int dStrlen = 0;

    if(dMaxlen % 2 != 0)
        dMaxlen ++;

    while(i < dMaxlen) {
        usString = (ucString[i+1] << 8) | ucString[i];

        if((usString & 0xFFFF) == 0)
            break;

        i +=2;
//        dStrlen ++;
    }

    return i;
}


/**C.2*	DECLARATION OF FUNCTIONS **********************************************/

/*******************************************************************************
 * Shared Memory 읽는 함수 
*******************************************************************************/


int Init_shm( key_t shm_key, int size, void **memptr )
{
    int shm_id, ret_val;

    if( (shm_id = shmget(shm_key, size, SHMPERM | IPC_CREAT | IPC_EXCL)) < 0 )
    {
        if( errno == EEXIST )
        {
            shm_id = shmget( shm_key, size, SHMPERM | IPC_CREAT );
            if( shm_id < 0 )
                return -errno;

            *memptr = shmat( shm_id, 0, 0 );
            ret_val = SHM_EXIST;    /* exist already */
        }
        else
            return -errno;
    }
    else
    {
        *memptr = shmat( shm_id, 0, 0 );
        ret_val = SHM_CREATE;   /* created */
    }

    if( (long)*memptr == -1 )
        return -1;

    return ret_val;
}



#if 0
int Read_m_shm( int dIndex, T_CAP *P, unsigned char *puBuffer )
{
	T_CAPHDR	*stHdr;

	int	dTemplength 	= 0;
	int dByteToRead		= 0;

	long lWritePos		= P->WritePos;
	long *lReadPos		= &P->ReadPos[dIndex];
	long *lWriteEndPos	= &P->WriteEndPos;

	/*** ERROR CHECK */
	if( ( lWritePos > MRG_MEM_SIZE ) || ( *lReadPos > MRG_MEM_SIZE ) ) {
		
		dAppLog( LOG_CRI, "[Read_m_shm][0] BUF ERROR : W[P:%d] R[P:%d] ",  
			P->WritePos, P->ReadPos[dIndex] );

		return Err_POINTER;
	}

	while(1)
	{
		if( lWritePos == *lReadPos ) {
			return Err_BUF_EMPTY;
		}
		else if( lWritePos < *lReadPos ) {
			if( *lWriteEndPos == *lReadPos ) {
                *lReadPos = 0;
                lReadPos    = &P->ReadPos[dIndex];
                lWriteEndPos = &P->WriteEndPos;

                continue;
            }
            else if( *lWriteEndPos < *lReadPos ) {
				dAppLog( LOG_CRI, "[Read_m_shm][1] READPOS[%d] GREATER THAN WRITEENDPOS[%d]",
					*lReadPos, *lWriteEndPos ); 

                return Err_CRITICAL;
            }
            else { /*** *lWriteEndPos > *lReadPos */
                dByteToRead = *lWriteEndPos - *lReadPos;
            }

            break;
		}

		dByteToRead = lWritePos - *lReadPos;

		break;	
	}

	/* 
	* READ A PACKET HEADER 
	*/
	stHdr = (T_CAPHDR *) &shm_capbuf->Mem[*lReadPos];
	dTemplength = sizeof( T_CAPHDR ) + stHdr->datalen;

	/*
	dAppLog( LOG_INFO, "[Read_m_shm][INFO] TMP_LEN[%d] TO_READ[%d]",
                dTemplength, dByteToRead );
	*/

	/* 
	* ERROR CHECK - 2002-07-08 : 읽어야 할 데이타의 Size에 대한 체크
	*/
	if( ( dTemplength < 0 ) || ( dTemplength > MAX_BUF_SIZE ) ) {

		dAppLog( LOG_CRI, "[Read_m_shm][2] LENGTH ERROR : LEN[%d] HDR[%d] DATA[%d]", 
			dTemplength, sizeof( T_CAPHDR ) , stHdr->datalen );

		return Err_POINTER;
	}

	/* 
	* ERROR CHECK 
	*/
	if( lWritePos > *lReadPos ) {
        if( lWritePos < (*lReadPos + dTemplength) ) {

            dAppLog( LOG_CRI, "[Read_m_shm][3] READ BUFFER ERROR : Wp[%d] Rp[%d] datalen[%d]",
                        lWritePos, *lReadPos, dTemplength);

            *lReadPos = lWritePos;

            return Err_POINTER;
        }
    }
    else {
        if( *lWriteEndPos < (*lReadPos + dTemplength) ) {

            dAppLog( LOG_CRI, "[Read_m_shm][4] READ BUFFER ERROR : WEndp[%d] Rp[%d] datalen[%d]",
                        *lWriteEndPos, *lReadPos, dTemplength);

            *lReadPos = *lWriteEndPos;
	
            return Err_POINTER;
        }
    }

	/*** READ A PACKET */
	memcpy( puBuffer, &shm_capbuf->Mem[*lReadPos], dTemplength );
	
	/*** UPDATE A READ POSITION */
	*lReadPos += dTemplength;
	dByteToRead -= dTemplength;

	return dByteToRead;
}
#endif

int dRead()
{
    FILE *fa;
    char szBuf[1024];
    char szType[64];
    char szTmp[64];
    char szInfo[64];
    int i;

    fa = fopen("/home/SERVERAPP/DATA/sysconfig.dat", "r");
    if(fa == NULL)
    {
        return -1;
    }

    i = 0;

    while(fgets(szBuf, 1024, fa) != NULL)
    {
        if(szBuf[0] != '#')
        {
            fclose(fa);
            return -2;
        }

        i++;

        if(szBuf[1] == '#')
            continue;
        else if(szBuf[1] == 'E')
            break;
        else if(szBuf[1] == '@') {
            if(sscanf(&szBuf[2],"%s %s %s" ,szType, szTmp, szInfo) == 3) {
                if(strcmp(szType, "NET") == 0) {
                    if(strcmp(szTmp, "NUM") == 0)
                        fidb->cCurrCnt[HW_ETH_IDX] = atoi(szInfo);
                } else if(strcmp(szType, "DBMS") == 0) {
                    if(strcmp(szTmp, "NUM") == 0)
                        fidb->cCurrCnt[LOAD_DB_IDX] = atoi(szInfo);
                } else if(strcmp(szType, "DISK") == 0) {
                    if(strcmp(szTmp, "NUM") == 0)
                        fidb->cCurrCnt[LOAD_DISK_IDX] = atoi(szInfo);
                }
            }
        }
    }
    fclose(fa);
    return 1;
}

int dSetInit()
{
    int dRet;
    int i;
    int j;
    int lastidx = 0;
    int dIdx = 0;
        
    fidb->dTotCompCnt = DB_CONN_IDX + 1;
            
    fidb->cCurrCnt[SW_COM_IDX] = MAX_SW_COM;    /* CHANGED BY UAMYD 050617 FOR CURR_SW_COM -> MAX_SW_COM */
    fidb->cCurrCnt[LOAD_CPU_IDX] = CURR_LOAD_CPU;
    fidb->cCurrCnt[LOAD_MEM_IDX] = CURR_LOAD_MEM;
    fidb->cCurrCnt[LOAD_QUE_IDX] = CURR_QUE_COM;
    fidb->cCurrCnt[DB_CONN_IDX] = 1;
                
    dRet = dRead();
    if(dRet < 0)
        return -1;

    return 0;
}



int dInit_LogLevel()
{
    FILE    *pro_fa;
    char    szBuf[1024];
    int     dseq, dflag, dRet, i;
    char    szName[24], szpath[64];

    memset(loglevel, 0x00, sizeof(st_LOGLEVEL));

    for( i = 0 ; i < MAX_SW_COM ; i++ ) {
        loglevel->szLevel[i] = 3;
    }

    pro_fa = fopen("/SERVERAPP/DATA/PROCESS_LIST.conf", "r");
    if( pro_fa == NULL ) {
        fprintf(stderr, "%s FILE OPEN FAILED\n", "SERVERAPP/DATA/PROCESS_LIST.conf");
        return -1;
    }

    while(fgets(szBuf, 1024, pro_fa) != NULL) {
        if(szBuf[0] != '#') {
            fclose(pro_fa);
            return -2;
        }

        if(szBuf[1] == '#')
            continue;
        else if(szBuf[1] == 'E')
            break;
        else if(szBuf[1] == '@') {
            if(sscanf(&szBuf[2],"%d %s %d %s" , &dseq, szName, &dflag, szpath) == 4) {
                sprintf(loglevel->szProcName[dseq], "%s", szName);
            }
        }
    }

    fclose(pro_fa);
    return 1;
}

INT dINIT_SERVERNO_INFORMATION()
{
    FILE    *fa;
    int     i = 0, dChnNo, dMax;
    char    szBuf[1024];
    char    szType[24], szName[16];
    
    fa = fopen("/SERVERAPP/DATA/CHANNEL_INFO.dat","r");
    if(fa == NULL) {
        dAppLog(LOG_CRI,"dSet_ServerIP_From_File: %s FILE OPEN FAILED (%s)",
                "/SERVERAPP/DATA/CHANNEL_INFO.dat", strerror(errno));
        return -1;
    }

    while( fgets( szBuf, 1024, fa ) != NULL ) {
        if(szBuf[0] != '#') {
            dAppLog(LOG_CRI,"dSet_ServerIP_From_File : %s Format Error", 
						"/SERVERAPP/DATA/CHANNEL_INFO.dat");
            fclose(fa);
            return -2;
        }

        if(szBuf[1] == '#') {
            continue;
        } else if(szBuf[1] == 'E') {
            break;
        } else if(szBuf[1] == '@') {
            if(sscanf(&szBuf[2],"%s %d %s %d", szType, &dChnNo, szName, &dMax) == 4) {
                if( strcasecmp(szType, "SERVER") == 0) { //서버 정보
                    g_uiServerNo = dChnNo;
					i++;
					return 0;
                }
            }
        }
    }

	if( i == 0 )
		return -1;
}


int Init_shm_common()
{
    int     dID;
    int     i;
    int     j;
    int     dRet;

	if((dID = shmget(S_SSHM_FIDB, sizeof(st_FIDB), 0666|IPC_CREAT|IPC_EXCL)) < 0)
    {
        if(errno == EEXIST)
        {
            dID = shmget(S_SSHM_FIDB, sizeof(st_FIDB), 0666|IPC_CREAT);
            if(dID < 0)
            {
                return -errno;
            }

            if( (fidb = (st_FIDB *)shmat(dID, 0, 0)) == (st_FIDB *)-1 )
            {
                return -errno;
            }
        }
        else
        {
            return -errno;
        }
    } else {
        if( (fidb = (st_FIDB *)shmat(dID, 0, 0)) == (st_FIDB *)-1 )
            return -errno;

        dRet = dSetInit();
        if(dRet < 0)
            return -1;
    }

    return 0;
}


int dLoad_GEN_INFO()
{
    FILE            *fa;
    int             dRet, i;
    st_GEN_INFO  	read_info;
	char    		szType[24], szData[32], szBuf[1024];

    fa = fopen(DEF_GENINFO_DATA, "r");
    if( fa == NULL ) {
        fa = fopen(DEF_GENINFO_DATA, "w");
        if( fa == NULL ) {
            return -1;
        }

        gen_info->uiExpBase 		= 100;
		gen_info->uiGameMoneyBase 	= 100;
		gen_info->uiMaxRoomCnt		= 140;

        dRet = fwrite(gen_info, sizeof(st_GEN_INFO), 1, fa);
        if( dRet < 0 ) {
            dAppLog(LOG_CRI, "%s FILE WRITE FAIL [%d][%s]", DEF_GENINFO_DATA, errno, strerror(errno));
            return -4;
        }

    } else {
        dRet = fread(&read_info, sizeof(st_GEN_INFO), 1, fa);
        if( dRet < 0 ) {
            dAppLog(LOG_CRI, "%s FILE READ FAIL [%d][%s]", DEF_GENINFO_DATA, errno, strerror(errno));
            return -3;
        }

		memcpy(gen_info, &read_info, sizeof(st_GEN_INFO));
    }

    fclose(fa);

#if 0
	fa = fopen("/SERVERAPP/DATA/NATION_INFO.conf","r");
    if(fa == NULL) {
        dAppLog(LOG_CRI,"dLoad_GEN_INFO: %s FILE OPEN FAILED (%s)",
               "/SERVERAPP/DATA/sysconfig", strerror(errno));
        return -1;
    }
    
    while( fgets( szBuf, 1024, fa ) != NULL ) {
        if(szBuf[0] != '#') {
            dAppLog(LOG_CRI,"dLoad_GEN_INFO : %s Format Error",
                    "/SERVERAPP/DATA/sysconfig");
            fclose(fa);
            return -2;
        }

        if(szBuf[1] == '#') {
            continue;
        } else if(szBuf[1] == 'E') {
            break;
        } else if(szBuf[1] == '@') {
            if(sscanf(&szBuf[2],"%u %s %u %u", gen_info->uiSystem, gen_info->szLocalName,
								gen_info->uiLocalCode, gen_info->uiCryptF ) == 4) {
				break;
            }
        }
    }

	fclose(fa);
#endif
	return 0;
}

int set_version(int proc_idx, char *version)
{
    if( proc_idx < 0 || proc_idx >= MAX_SW_COM )
        return -1;

    sprintf(fidb->stSWInfo[proc_idx].szVersion, "%s", version);
    return 0;
}

USHORT CVT_USHORT( USHORT value )
{
    union {
        USHORT xValue;
        char ml[2];
    } u1, u2;

    u1.xValue = value;

    u2.ml[0] = u1.ml[1];
    u2.ml[1] = u1.ml[0];

    return u2.xValue;
}

UINT CVT_UINT( UINT value )
{       
    union { 
        UINT xValue;
        char ml[4];
    } u1, u2;   
        
    u1.xValue = value;

    u2.ml[0] = u1.ml[3];
    u2.ml[1] = u1.ml[2];
    u2.ml[2] = u1.ml[1];
    u2.ml[3] = u1.ml[0];

    return u2.xValue;
}   

UINT64 CVT_UINT64( UINT64 value ) 
{
    union {
        UINT64 	xValue;
        char 	ml[8];
    } u1, u2;

    u1.xValue = value;

    u2.ml[0] = u1.ml[7];
    u2.ml[1] = u1.ml[6];
    u2.ml[2] = u1.ml[5];
    u2.ml[3] = u1.ml[4];
    u2.ml[4] = u1.ml[3];
    u2.ml[5] = u1.ml[2];
    u2.ml[6] = u1.ml[1];
    u2.ml[7] = u1.ml[0];

    return u2.xValue;
}


char* CVT_IPADDR( UINT uiIP )
{
    struct in_addr  inaddr;
    inaddr.s_addr = CVT_UINT(uiIP);

    return (char *)inet_ntoa(inaddr);
}

int dGet_ServerIP_From_File(char *szSystem, char *szIPAddr)
{
    FILE    *fa;
    int     i =0;
    char    szBuf[1024];
    char    szSys[24], szIP[16];

    fa = fopen(IPADDR_FILE,"r");
    if(fa == NULL) {
        dAppLog(LOG_CRI,"dSet_ServerIP_From_File: %s FILE OPEN FAILED (%s)",
                IPADDR_FILE, strerror(errno));
        return -1;
    }

    while( fgets( szBuf, 1024, fa ) != NULL ) {
        if(szBuf[0] != '#') {
            dAppLog(LOG_CRI,"dSet_ServerIP_From_File : %s Format Error",
                    IPADDR_FILE);
            fclose(fa);
            return -2;
        }

        if(szBuf[1] == '#') {
            continue;
        } else if(szBuf[1] == 'E') {
            break;
        } else if(szBuf[1] == '@') {
            if(sscanf(&szBuf[2],"%s %s", szSys, szIP) == 2) {
                if(strcasecmp( szSys, szSystem ) == 0) {
                    sprintf(szIPAddr, "%s", szIP);
                    i++;
                    break;
                }

            }
        }
    }

    fclose(fa);
    if( i == 0 ) {
        return -1;
    } else {
        return 0;
    }
}


/* Elenoa: 2008. 1. 18: timestamp function, same as TEMS */
unsigned long long timestamp( void )
{
    struct timeval		tv;
    unsigned long long	ullRetval;

    memset( &tv, 0, sizeof( struct timeval ) );

    gettimeofday( &tv, NULL );
    ullRetval = ( tv.tv_sec * DUP_METER ) + tv.tv_usec;

    return ullRetval;
}

#if 0
int dSendToReplay( pst_MsgQ pstMsgQ, int qid, pst_ReplayKey pstReplayKey)
{
    INT     isize, dRet;

	st_MsgQ				stMsgQ;
    pst_PktHdr          pstHeader;
    pst_ReplayHeader    pstReplayH;

	/* Packet header copy */
	memcpy(&stMsgQ.szBody[0], &pstMsgQ->szBody[0], PACK_HEADER_LEN);

	/* Replay header copy */
    memcpy(&stMsgQ.szBody[PACK_HEADER_LEN], pstReplayKey, sizeof(st_ReplayKey));

	/* Original body copy */
    memcpy(&stMsgQ.szBody[PACK_HEADER_LEN + REPLAY_HEADER_LEN], &pstMsgQ->szBody[0], pstMsgQ->usBodyLen);

    pstHeader   = (pst_PktHdr)&stMsgQ.szBody[0];
    pstReplayH  = (pst_ReplayHeader)&stMsgQ.szBody[PACK_HEADER_LEN];

	stMsgQ.usSvcID	 = SID_GAME_REPLAY;
	stMsgQ.usMsgID	 = MID_REPDATASAVE;
	stMsgQ.dMsgQID	 = pstMsgQ->dMsgQID;
	stMsgQ.usRetCode = pstMsgQ->usRetCode;
	stMsgQ.uIndex	 = pstMsgQ->uIndex;
	stMsgQ.llSockSessID = pstMsgQ->llSockSessID;

	pstReplayH->usReplayDataLen = sizeof(st_ReplayFileDataHeader) + pstHeader->st1stPH.usTotLen;
	pstReplayH->tEventTime 		= timestamp();

    stMsgQ.usBodyLen = PACK_HEADER_LEN + REPLAY_HEADER_LEN + pstMsgQ->usBodyLen;

    isize = sizeof(st_MsgQ) - MAX_MSGBODY_SIZE + stMsgQ.usBodyLen - sizeof(long int);

    dRet = msgsnd(qid, &stMsgQ, isize, 0);
    if ( dRet < 0) {
        dAppLog( LOG_CRI, "[Qid = %d] ERROR SEND : %d[%s]", qid, errno, strerror(errno));
        return -1;
    }

    return 0;
}
#endif

UINT GET_GAPTIME(struct timeval *t1, struct timeval *t2)
{
	INT64	Gap;
	
	Gap = (t1->tv_sec*1000000 + t1->tv_usec) - (t2->tv_sec*1000000 + t2->tv_usec);

	if( Gap < 0 )
		return 0;

    return Gap;
}

int set_status_running(int proc_idx)
{
    if(proc_idx < 0 || proc_idx >= MAX_SW_COM)
        return -1;
    fidb->stSWInfo[proc_idx].ucDoneFlag = 1;
    return 0;
}

void Change_HeaderByteOrder( pst_UDPHeader pstUPH )
{
    pstUPH->usPKLen = htons(pstUPH->usPKLen);
    pstUPH->usSvcID = htons(pstUPH->usSvcID);
    pstUPH->usMsgID = htons(pstUPH->usMsgID);
    pstUPH->ullSessID = CVT_UINT64(pstUPH->ullSessID);
    pstUPH->usPKVer = htons(pstUPH->usPKVer);
    pstUPH->usPKVer = htons(pstUPH->usPKVer);
    pstUPH->usResult = htons(pstUPH->usResult);
    pstUPH->uiMySeq = CVT_UINT(pstUPH->uiMySeq);
    pstUPH->uiServSeq = CVT_UINT(pstUPH->uiServSeq);
}

int UnicodeToUTF8(UCHAR* szSrc, int dSrcLen, UCHAR* szDest, int dDestLen)
{
    int     i;
    int     dCurOutput=0;
    UCHAR   ch_tmp_byte;
    USHORT  usUnicode;
 
    memset(szDest, 0x00, dDestLen);
    for ( i = 0; i < dSrcLen; i = i + 2 )
    {
        usUnicode = (szSrc[i+1] << 8) | szSrc[i];

        if((usUnicode & 0xFFFF) == 0)
            break;

        if( BYTE_1_REP > usUnicode ) { /* 1 byte utf8 representation */
            if( dCurOutput + 1 < dDestLen ) {
                szDest[dCurOutput++] = (UCHAR)usUnicode;
            } else {
                break; /* ERROR_INSUFFICIENT_BUFFER */
            }
        } else if( BYTE_2_REP > usUnicode ) { /* 2 byte utf8 representation */
            if( dCurOutput + 2 < dDestLen ) {
                szDest[dCurOutput++] = (UCHAR)((usUnicode >> 6) | 0xc0);
                szDest[dCurOutput++] = (UCHAR)((usUnicode & 0x3f) | 0x80);
            } else {
                break; /* ERROR_INSUFFICIENT_BUFFER */
            }
        } else if( SURROGATE_MAX > usUnicode && SURROGATE_MIN < usUnicode ) { /* 4 byte surrogate pair representation 
*/      
            if ( dCurOutput + 4 < dDestLen ) {
                ch_tmp_byte = (UCHAR)(((usUnicode & 0x3c0) >> 6) + 1);
                szDest[dCurOutput++] = (UCHAR)((ch_tmp_byte >> 2) | 0xf0);
                szDest[dCurOutput++] = (UCHAR)(((ch_tmp_byte & 0x03) | 0x80) | ((usUnicode & 0x3e) >> 2));
            } else {
                break; /* ERROR_INSUFFICIENT_BUFFER */
            }
        } else { /* 3 byte utf8 representation */
            if( dCurOutput + 3 < dDestLen ) {
                szDest[dCurOutput++] = (UCHAR)((usUnicode >> 12) | 0xe0);
                szDest[dCurOutput++] = (UCHAR)(((usUnicode >> 6) & 0x3f) | 0x80);
                szDest[dCurOutput++] = (UCHAR)((usUnicode & 0x3f) | 0x80);
            } else {
                break; /* ERROR_INSUFFICIENT_BUFFER */
            }
        }
    }

    szDest[dCurOutput] = '\0'; /* Terminate string */

    return dCurOutput;
}

int UTF8ToUnicode(const CHAR *szSrc, int dSrcLen, USHORT *szDest, int dDestLen)
{
    int i = 0;
    int dCurOutput = 0;

    UCHAR *pszSrc = (UCHAR *)szSrc;  /* cast to avoid signed/unsigned promomtion problems */
    memset(szDest, 0x00, dDestLen);

    while ( i < dSrcLen ) {
        if ( SIGMASK_3_1 <= pszSrc[i] ) {  /* 1st byte of 3 byte representation */
            if ( (i + 2 < dSrcLen) && (dCurOutput + 1 < dDestLen) ) {
                szDest[dCurOutput++] =(USHORT)( ((USHORT)pszSrc[i] << 12) |
                                                (((USHORT)pszSrc[i+1] & 0x3f) << 6) |
                                                ((USHORT)pszSrc[i+2] & 0x3f));
                i+=3;
            } else {
                return 0; /* ERROR_INSUFFICIENT_BUFFER */
            }
        } else if ( SIGMASK_2_1 <= pszSrc[i] ) {  /* 1st byte of 2 byte representation */
            if ( (i + 1 < dSrcLen) && (dCurOutput + 1 < dDestLen) ) {
                szDest[dCurOutput++] = (USHORT)(((USHORT)pszSrc[i] & ~0xc0) << 6 |
                                       ((USHORT)pszSrc[i+1] & ~0x80));
                i+=2;
            } else {
                return 0; /* ERROR_INSUFFICIENT_BUFFER */
            }
        } else {  /* Single byte representation */
            if ( (i < dSrcLen) && (dCurOutput+1 < dDestLen) ) {
                szDest[dCurOutput++] = (USHORT)pszSrc[i];
                ++i;
            } else {
                return 0; /* ERROR_INSUFFICIENT_BUFFER */
            }
        }
    }

    szDest[dCurOutput] = 0; /* Terminate string */
    return dCurOutput;
}

int get_UTF8StringLen(CHAR *pszSrc, UINT uiMax)
{
	int i = 0, p = 0;

	while ( pszSrc[p] && (uiMax >= i) ) {
        if ( (SIGMASK_3_1 & pszSrc[p]) == SIGMASK_3_1 ) {  /* 1st byte of 3 byte representation */
        	p+=3;
        } else if ( (SIGMASK_2_1 & pszSrc[p]) == SIGMASK_2_1 ) {  /* 1st byte of 2 byte representation */
            p+=2; 
        } else {  /* Single byte representation */
			p++;
        }
		i++;
    }   
	
	return p;
}

#define SYSCONFIG_PATH "/SERVERAPP/DATA/sysconfig.dat"

int dGet_CSMSConfig( pst_SystemInfo pstSI ) 
{
	    FILE *fa;
    char szBuf[1024];
    char szType[64];
    char szTmp[64];
    char szInfo[64];
    int i;

    fa = fopen( SYSCONFIG_PATH, "r" );
    if( !fa ) {
        dAppLog(LOG_CRI,"%s --> %s File Open Fail (%s)", 
			__func__, SYSCONFIG_PATH, strerror(errno));
        return -1;
    }

    i = 0;

    while(fgets(szBuf,1024,fa) != NULL) {
        if(szBuf[0] != '#') {
            dAppLog(LOG_CRI,"%s --> %s File [%d] row format error",
				__func__, SYSCONFIG_PATH, i);
            fclose(fa);
            return -1;
        }

        if(szBuf[1] == '#')
            continue;
        else if(szBuf[1] == 'E')
            break;
        else if(szBuf[1] == '@') {
            if(sscanf(&szBuf[2],"%s %s %s", szType, szTmp, szInfo) == 3) {
                if(strcmp(szType,"CSMS") == 0) {
					pstSI->uiServerNo = atoi(szTmp);
					pstSI->uiSystemIndex = atoi(szInfo);
                }
            }
        }
    }

    fclose(fa);
    return 1;
}


