#include <time.h>

#include <chsmd_api.h>
#include <chsmd_extern.h>

#include <msgdef.h>
#include <utillib.h>
#include <server_shm.h>
#include <server_sock.h>

extern st_SYS_ALMLOAD *sys_almload;
int dCondFlag;

unsigned char chksts();
int FidbPrint();

int CompareLoad_CPU( int loadidx, int loadinfo , int *AlmLevel );
int CompareLoad_MEM( int loadidx, int loadinfo , int *AlmLevel );
int CompareLoad_DISK( int loadidx, int loadinfo , int *AlmLevel );

int dSend_Status_Info()
{
	int 	i;
	int 	pos = 0;
	int		dETHIndex = 0;
	int		dDBLOADStIdx = 0;
	char 	szBuf[4];

	memset( szBuf, 0x00, 4 );

	for( i=0; i<fidb->cCurrCnt[SW_COM_IDX]; i++ ){

		if( fidb->stSWInfo[i].ucActFlag > 0 ){

			szBuf[pos++] = i;
			szBuf[pos++] = fidb->stStatus[i].stSts.sts;
			szBuf[pos] = (char)chksts( i ); 
			if( fidb->stStatus[i].stSts.mask == MASK )
				szBuf[pos] += tMASK;
			pos++;

			pos = 0;
		}
	}

	for( i=0; i<HW_ETH_IDX; i++ )
		dETHIndex += fidb->cCurrCnt[i];

	for( i=0; i<fidb->cCurrCnt[HW_ETH_IDX]; i++ ) {

		szBuf[pos++] = i;
        szBuf[pos++] = fidb->stStatus[dETHIndex + i].stSts.sts;
        szBuf[pos] = (char)chksts(dETHIndex + i);
        if( fidb->stStatus[dETHIndex + i].stSts.mask == MASK )
            szBuf[pos] += tMASK;
        pos++;

        pos = 0;	
	}

	/* FOR DB LOAD */
	for( i = LOAD_CPU_IDX; i < HW_FAN_IDX; i++ )
    	dDBLOADStIdx += fidb->cCurrCnt[i];

	return 1;
}

unsigned char chksts( int idx )
{
	return fidb->stStatus[idx].stSts.sts;
}

int	FIDB_Handle_Proc( st_AlmStat *almsts )
{
	int 		AlmLevel = NORMAL;
	int			i;
	int			pos = 0;
	unsigned char		szBuf[100];
    int         k;
	unsigned char idx;
	unsigned char type;
	unsigned char old;
	unsigned char status;
	unsigned short usload;

	memset( szBuf, 0x00, 100 );
	dCondFlag = 0;


	idx = almsts->ucLocIdx;
	type = almsts->ucLocType;
	status = almsts->ucStatus;

	switch(type)
	{
	case SW_COM_IDX:
		if( idx > fidb->cCurrCnt[SW_COM_IDX] )
		{
			dAppLog(LOG_WARN, "FIDB Invalid Index[%d]", idx);
			return -1;
		}
		if( atoi(almsts->Info)< 0 ){
			
			fidb->stSWInfo[idx].ucActFlag = 0;
			fidb->stStatus[idx].stSts.init = NOT_INIT;
			dAppLog( LOG_DEBUG, "FIDB : Detected Activation flag disable Block [%d]", idx );
			return 1;
			
		}
		else{
			
			fidb->stSWInfo[idx].ucActFlag = 1;
		}

		fidb->stSWInfo[idx].uiPid = atoi( almsts->Info );
		sprintf( fidb->stSWInfo[idx].szProcName, "%s", &almsts->Info[32] );

		old = SetStatus( almsts, idx, status, 0 );

		szBuf[pos++] = idx;
		szBuf[pos++] = old;
		szBuf[pos] = chksts( idx ); 
		if( fidb->stStatus[idx].stSts.mask == MASK )
			szBuf[pos] += tMASK;
		pos++;

		if( (almsts->ucStatus == SET_START ) || (almsts->ucStatus == SET_STOP) ) {
			;
		} else {

			if( dCondFlag > 0 && oldAlmLevel[idx] != szBuf[2] ){
				oldAlmLevel[idx] = szBuf[2];
			}
		}

		break; 

	case DB_CONN_IDX:
		if ( idx > fidb->cCurrCnt[LOAD_DB_IDX] ){
			dAppLog(LOG_WARN, " DB_CONN_IDX INVALID [%d]",idx );
			return -1;
		}
		for( i = 0; i< DB_CONN_IDX; i++ )
			idx += fidb->cCurrCnt[i];

		old = SetStatus( almsts, idx, status, 0 );
		
		szBuf[pos++] = almsts->ucLocIdx;
		szBuf[pos++] = old;
		szBuf[pos++] = almsts->ucAlmLevel;

		dAppLog(LOG_INFO,"FIDB[ DB_CONNECT : %s ] STATUS=[%d] ",
		( fidb->fDBConnect == ALIVE )? "ALIVE":"DEAD", almsts->ucAlmLevel );

		break;
		
	case HW_ETH_IDX: 

		if( idx > fidb->cCurrCnt[HW_ETH_IDX] )
		{
			dAppLog(LOG_WARN, " HW_ETH IDX INVALID [%d]", idx);
			return -1;
		}
		idx += fidb->cCurrCnt[SW_COM_IDX];	

		old = SetStatus( almsts, idx, status, 0 );

		szBuf[pos++] = almsts->ucLocIdx;
		szBuf[pos++] = old;
		szBuf[pos] = chksts( idx ); 
		if( fidb->stStatus[idx].stSts.mask == MASK )
			szBuf[pos] += tMASK;
		pos++;

		if( dCondFlag > 0 && oldAlmLevel[idx] != szBuf[2] ){
			oldAlmLevel[idx] = szBuf[2];
		}

		break;
	
	case LOAD_CPU_IDX:
		if( idx > fidb->cCurrCnt[LOAD_CPU_IDX] )
		{
			dAppLog(LOG_WARN, " LOAD_CPU IDX INVALID [%d]", idx);
			return -1;
		}

		CompareLoad_CPU( idx, atoi( almsts->Info ) , &AlmLevel ); 

		/* 10 second delay ******/
		for(k = 0; k < DEF_CPU_ALM_CNT - 1; k++)
			oldcpu[k] = oldcpu[k + 1];
	
		oldcpu[DEF_CPU_ALM_CNT - 1] = AlmLevel;

		if( (oldcpu[0] != oldcpu[1]) || (oldcpu[0] != oldcpu[2]) )
			
			AlmLevel = CRITICAL;

			for(k=0; k<DEF_CPU_ALM_CNT; k++) {
                if( AlmLevel >= oldcpu[k] )
                    AlmLevel = oldcpu[k];
		}

		for( i = 0; i < LOAD_CPU_IDX; i++ )
			idx += fidb->cCurrCnt[i];

		old = SetStatus( almsts, idx, status, AlmLevel );
		usload = atoi(almsts->Info);


#ifdef CVT_ORDER
		usload = htons(usload);
#endif

		szBuf[pos++] = almsts->ucLocIdx;
		szBuf[pos++] = old;
		memcpy( &szBuf[pos], &usload, 2 );
		pos += 2;
		szBuf[pos] = chksts( idx ); 
		if( fidb->stStatus[idx].stSts.mask == MASK )
			szBuf[pos] += tMASK;
		pos++;

		if( dCondFlag > 0 && oldAlmLevel[idx] != AlmLevel ){
			oldAlmLevel[idx] = AlmLevel;
		}

		break;

	case LOAD_MEM_IDX:
		if( idx > fidb->cCurrCnt[LOAD_MEM_IDX] )
		{
			dAppLog(LOG_DEBUG, " LOAD_MEM IDX INVALID [%d]", idx);
			return -1;
		}

		for( i = LOAD_CPU_IDX; i< LOAD_MEM_IDX; i++ )
			idx += fidb->cCurrCnt[i];

		CompareLoad_MEM( idx, atoi( almsts->Info ), &AlmLevel ); 
	
		for( i = 0; i< LOAD_MEM_IDX; i++ )
			idx += fidb->cCurrCnt[i];

		old = SetStatus( almsts, idx, status, AlmLevel );
		usload = atoi(almsts->Info);

#ifdef CVT_ORDER
		usload = htons(usload);
#endif

		szBuf[pos++] = almsts->ucLocIdx;
		szBuf[pos++] = old;
		memcpy( &szBuf[pos], &usload, 2 );
		pos += 2;
		szBuf[pos] = chksts( idx ); 
		if( fidb->stStatus[idx].stSts.mask == MASK )
			szBuf[pos] += tMASK;
		pos++;

		if( dCondFlag > 0 && oldAlmLevel[idx] != AlmLevel ){
			oldAlmLevel[idx] = AlmLevel;
		}

		break;

	case LOAD_DISK_IDX:
		if( idx > fidb->cCurrCnt[LOAD_DISK_IDX] )
		{
			dAppLog(LOG_DEBUG, " LOAD_DISK IDX INVALID [%d]", idx);
			return -1;
		}
		for( i = LOAD_CPU_IDX; i < LOAD_DISK_IDX; i++ )
			idx += fidb->cCurrCnt[i];

		CompareLoad_DISK( idx, atoi( almsts->Info ) , &AlmLevel ); 

		for( i = 0; i< LOAD_DISK_IDX; i++ )
			idx += fidb->cCurrCnt[i];

		old = SetStatus( almsts, idx, status, AlmLevel );
		usload = atoi(almsts->Info);

#ifdef CVT_ORDER
		usload = htons(usload);
#endif

		szBuf[pos++] = almsts->ucLocIdx;
		szBuf[pos++] = old;
		memcpy( &szBuf[pos], &usload, 2 );
		pos += 2;
		szBuf[pos] = chksts( idx ); 
		if( fidb->stStatus[idx].stSts.mask == MASK )
			szBuf[pos] += tMASK;
		pos++;

		if( dCondFlag > 0 && oldAlmLevel[idx] != AlmLevel ){
			oldAlmLevel[idx] = AlmLevel;
		}

		break;

	default :
		dAppLog(LOG_DEBUG," DON'T SEARCH TYPE : LOCTYPE(%d) LOCIDX(%d) STATUS(%d)", 
		type, idx, status ); 
		break;	
	}

	fidb->tChsmdWhen = time(NULL);

	return 0;
}

int SetStatus( st_AlmStat *almsts , int idx, int sts, int AlmLevel )
{
	unsigned char	ucRet_OldAlmLevel=NORMAL;
    time_t tCur;
 	tCur = time(NULL);

	ucRet_OldAlmLevel = fidb->stStatus[idx].stSts.sts;

    switch( sts )
    {
    case  DEAD :

		if( almsts->ucLocType == DB_CONN_IDX ){
	
			almsts->ucAlmLevel = CRITICAL;
			almsts->ucOldAlmLevel = NORMAL;
			SendToCond(almsts);
			ucRet_OldAlmLevel = NORMAL;
			break;
		}

        if( fidb->stStatus[idx].stSts.mask == UNMASK 
		&& fidb->stStatus[idx].stSts.sts != CRITICAL) 
		{
			fidb->stStatusWhen[idx] = tCur;
			almsts->ucAlmLevel = CRITICAL;
			almsts->ucOldAlmLevel = fidb->stStatus[idx].stSts.sts;
            SendToCond(almsts);
        }

        fidb->stStatus[idx].stSts.sts = CRITICAL;
        break;

    case  ALIVE :

		if( almsts->ucLocType == DB_CONN_IDX ){
	
			almsts->ucAlmLevel = NORMAL;
			almsts->ucOldAlmLevel = CRITICAL;
			SendToCond(almsts);
			ucRet_OldAlmLevel = CRITICAL;
			break;
		}
        if( fidb->stStatus[idx].stSts.mask == UNMASK 
		&& fidb->stStatus[idx].stSts.sts != NORMAL )
        {
			fidb->stStatusWhen[idx] = tCur;
			almsts->ucAlmLevel = NORMAL;
			almsts->ucOldAlmLevel = CRITICAL;
			ucRet_OldAlmLevel = CRITICAL;
            SendToCond(almsts);
        }

        fidb->stStatus[idx].stSts.sts = NORMAL;
        break;
	
	case SET_STOP :
		if( fidb->stStatus[idx].stSts.mask == UNMASK
        && fidb->stStatus[idx].stSts.sts != CRITICAL )
        {
            fidb->stStatusWhen[idx] = tCur;
            almsts->ucAlmLevel = CRITICAL;
            almsts->ucOldAlmLevel = fidb->stStatus[idx].stSts.sts;
            SendToCond(almsts);
        }

        fidb->stStatus[idx].stSts.stop = STOP;
        fidb->stStatus[idx].stSts.init = INIT;
        fidb->stStatus[idx].stSts.sts = CRITICAL;
        break;

		break;

	case SET_UNSTOP:
        if( fidb->stStatus[idx].stSts.mask == UNMASK 
		&& fidb->stStatus[idx].stSts.sts != NORMAL )
        {
			fidb->stStatusWhen[idx] = tCur;	
           	SendToCond(almsts);
        }

		fidb->stStatus[idx].stSts.stop = NOSTOP;
		fidb->stStatus[idx].stSts.init = INIT;
		fidb->stStatus[idx].stSts.sts = NORMAL;

		break;

	case SET_START:
        if( fidb->stStatus[idx].stSts.mask == UNMASK 
		&& fidb->stStatus[idx].stSts.sts != NORMAL )
        {
		   	fidb->stStatusWhen[idx] = tCur;	
			almsts->ucAlmLevel = NORMAL;
			almsts->ucOldAlmLevel = fidb->stStatus[idx].stSts.sts;
           	SendToCond(almsts);
        }

		fidb->stStatus[idx].stSts.stop = NOSTOP;
		fidb->stStatus[idx].stSts.init = INIT;
		fidb->stStatus[idx].stSts.sts = NORMAL;
		break;

	case LOAD_INFO :
		if( fidb->stStatus[idx].stSts.mask == UNMASK && 
		fidb->stStatus[idx].stSts.sts != AlmLevel )
		{
			fidb->stStatusWhen[idx] = tCur;

			almsts->ucAlmLevel = AlmLevel;
            almsts->ucOldAlmLevel = fidb->stStatus[idx].stSts.sts;
            SendToCond(almsts);

            ucRet_OldAlmLevel = fidb->stStatus[idx].stSts.sts;
		}
		fidb->stStatus[idx].stSts.sts = AlmLevel;
		fidb->stStatus[idx].stSts.stop = NOSTOP;

		break;

    default :
        break;
    }
	return ucRet_OldAlmLevel;
}

int CompareLoad_CPU( int loadidx, int loadinfo , int *AlmLevel ) 
{

	if( sys_almload->dCPULoad*100.0 == 0 )
		*AlmLevel = NORMAL;
	else if( sys_almload->dCPULoad*100.0 <= loadinfo )
		*AlmLevel = CRITICAL;	
	else
		*AlmLevel = NORMAL;

	fidb->stLoadInfo[loadidx].uiCurload = loadinfo ;

	return 0;
}


int CompareLoad_MEM( int loadidx, int loadinfo , int *AlmLevel )
{

	dAppLog(LOG_INFO,"%d MEM LOAD : %d", sys_almload->dMEMLoad, loadinfo);
    if( sys_almload->dMEMLoad*100.0 == 0 ) 
        *AlmLevel = NORMAL;
    else if( sys_almload->dMEMLoad*100.0 <= loadinfo )
        *AlmLevel = CRITICAL;
    else
        *AlmLevel = NORMAL;

    fidb->stLoadInfo[loadidx].uiCurload = loadinfo ;

    return 0;
}


int CompareLoad_DISK( int loadidx, int loadinfo , int *AlmLevel )
{

    if( sys_almload->dDISKLoad*100.0 == 0 ) 
        *AlmLevel = NORMAL;
    else if( sys_almload->dDISKLoad*100.0 <= loadinfo )
        *AlmLevel = CRITICAL;
    else
        *AlmLevel = NORMAL;

    fidb->stLoadInfo[loadidx].uiCurload = loadinfo ;

    return 0;
}

char *cvtTime(time_t when)
{
	static char crmtime_str[128];

    strftime(crmtime_str, 80, "%Y-%m-%d %T %a", localtime((time_t *)&when));
    crmtime_str[21] = toupper(crmtime_str[21]);
    crmtime_str[22] = toupper(crmtime_str[22]);
    return crmtime_str;
}

int SendToCond( pst_AlmStat almsts )
{

	st_MsgQ 		stMsgQ;
	pst_SockHeader 	psock;
	int				dMsgLen;
	int				dRet;

	memset( &stMsgQ, 0x00, sizeof(st_MsgQ) );
	psock = (pst_SockHeader)stMsgQ.szBody;
	dCondFlag = 1;

	stMsgQ.usType 	= DEF_SYS;
	stMsgQ.usSvcID 	= SID_STATUS;
	stMsgQ.usMsgID 	= MID_CONSOL;
	stMsgQ.ucProID 	= SEQ_PROC_CHSMD;
	stMsgQ.usRetCode = 0;

	stMsgQ.usBodyLen = SOCK_HEADER_LEN + sizeof(st_AlmStat);

	psock->llMagicNumber = MAGIC_NUMBER;

    psock->usTotLen = stMsgQ.usBodyLen;

    psock->usSvcID = stMsgQ.usSvcID;
    psock->usMsgID = stMsgQ.usMsgID;

    psock->usSerial = 0;

	memcpy( &stMsgQ.szBody[SOCK_HEADER_LEN], almsts, sizeof(st_AlmStat) );
	dMsgLen = DEF_MSGHEAD_LEN + stMsgQ.usBodyLen - sizeof(long);

#ifdef CVT_ORDER
    almsts->tWhen = htonl(almsts->tWhen);
#endif

	dRet = msgsnd( dCondQid, &stMsgQ, dMsgLen, 0 );
	if(dRet < 0 ){
		dAppLog( LOG_CRI, "FALIED in msgsnd COND_INFO : %d. %s", 
		errno, strerror(errno) );
		return -1;
	}

	return 0;
}


