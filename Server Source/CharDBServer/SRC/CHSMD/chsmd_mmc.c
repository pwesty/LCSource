/********************************************************
	- changed by uamyd 050428: CURR_SW_COM -> g_dProcCnt
	- changed by uamyd 050428: STR_TSW_COM -> szProcName
	- moved functin by UAMYD 050715 : 
		int dis_mask_svc, int mask_svc,
		int dis_mask_alm, int mask_alm
********************************************************/
#include <unistd.h>
#include <chsmd_def.h>
#include <chsmd_api.h>
#include <chsmd_extern.h>

#include <server_mmcerr.h>
#include <server_mmcid.h>
#include <server_mmcd.h>
#include <server_define.h>
#include <msgdef.h>
#include <utillib.h>
#include <server_shm.h>

int dGET_PRC(mml_msg *ml);
char *ACT_COM[] = {	"DEACTIVE",	"ACTIVE" };

int dProc_mmcd(pst_MsgQ pstMsgQ)
{
	/* MsgQ, MsgQHeader + MsgQBody(MMCDHeader + MMCDBody) */
	INT			dBodyLen = 0, dRet;
	UINT		uiMMCDID;
	UCHAR		szData[MAX_MSGBODY_SIZE - MMCD_HEADER_LEN];
	UCHAR*		pszOldData;
	pst_MMCDHdr pstMMCDHdr;

	memset( szData, 0x00, MAX_MSGBODY_SIZE );
	pszOldData  = &pstMsgQ->szBody[MMCD_HEADER_LEN];
	pstMMCDHdr	= (pst_MMCDHdr)pstMsgQ->szBody;
	uiMMCDID	= pstMMCDHdr->uiMmlID;

	switch(uiMMCDID) {
		/* dBodyLen <  0, Error
		   dBodyLen >= 0, Success & bodyLen	*/
		   
		case DIS_PRC:
			dBodyLen = dDIS_PRC(szData);
			break;
		case ACT_PRC:
			dBodyLen = dACT_PRC(pszOldData, szData);
			break;
		case DACT_PRC:
			dBodyLen = dDACT_PRC(pszOldData, szData);
			break;
		case DIS_LOAD_STS:
			dBodyLen = dDIS_LOAD_STS(szData);
			break;
		default:
			dAppLog(LOG_DEBUG, "UnAcceptable COMMAND INPUT MMCDID(%d)", uiMMCDID );
			break;
	}

	if( dBodyLen < 0 ) {
		dAppLog(LOG_DEBUG, "FAILED IN COMMAND PROC MMCDID(%d)", uiMMCDID );
		pstMMCDHdr->dResult = dBodyLen;
		dBodyLen = 0;
	}

	memcpy(pszOldData, szData, dBodyLen);
	pstMsgQ->usBodyLen = MMCD_HEADER_LEN + dBodyLen;

    dRet = msgsnd(dMmcdQid, pstMsgQ, 
		sizeof(st_MsgQ) - MAX_MSGBODY_SIZE + pstMsgQ->usBodyLen - sizeof(long int) , 0);
    if ( dRet < 0) {
        dAppLog(LOG_CRI,"SEND FAIL for MMCD is not delivered : MMCDID:%u, %s", 
			uiMMCDID, strerror(errno));
        return -1;
    } else {
	    dAppLog(LOG_INFO," * SUCCESS SEND TO MSG [%u]SEQ:%u", uiMMCDID, pstMMCDHdr->uiMmlSEQ);
	}

    return 1;

}

INT dDIS_PRC( UCHAR* pszData )
{

	INT	dBodyLen = 0, i, dRet;
	UINT* puiProcCnt;
	pst_DisPrcResp pstResp;

	/* Dis_prc */

	dBodyLen	= 8;
	puiProcCnt	= (UINT*)&pszData[0];
 
	for(i = 0; i < g_dProcCnt; i++)
	{
		pstResp 	= (pst_DisPrcResp)&pszData[dBodyLen];

		dRet = 0;
		if(fidb->stSWInfo[i].uiPid > 0)	dRet = 1;

		if( dRet > 0 ){
			sprintf( pstResp->szProc, "%s", szProcName[i] );
			pstResp->uiProcID = fidb->stSWInfo[i].uiPid;
			sprintf( pstResp->szStatus, "%s", ACT_COM[dRet] );
			*puiProcCnt += 1;
			dBodyLen += sizeof(st_DisPrcResp);
		} else if( fidb->stSWInfo[i].ucActFlag ) {
			sprintf( pstResp->szProc, "%s", szProcName[i] );
			pstResp->uiProcID = fidb->stSWInfo[i].uiPid;
			sprintf( pstResp->szStatus, "%s", ACT_COM[dRet] );
			*puiProcCnt += 1;
			dBodyLen += sizeof(st_DisPrcResp);
		}
		else
			continue;
	}

	return dBodyLen;
}

INT dACT_PRC(UCHAR* pszOldData, UCHAR* pszData)
{
	INT dRet, i, flag, pid;
	time_t	tTime;
	st_AlmStat	stAlm;

	for( i = 0, flag = 1; i < g_dProcCnt; i++) {
        if (strncasecmp( (CHAR*)pszOldData, szProcName[i], 16) == 0) {
            flag = 0;
            break;
        }
    }

    if(flag) return -100;

	if(fidb->stStatus[i].stSts.stop == NOSTOP) return -101;	/* Already alive */
	else
	{
		if((pid = auto_restart_the_process(i)) > 0)
		{
			dAppLog(LOG_DEBUG,"dAct_prc : Restart [%s]",szProcName[i]);
			
			lastpid[i] = pid;
			proc[i] = ALIVE;

            time(&tTime);
            stAlm.ucLocType = SW_COM_IDX;
            stAlm.ucLocIdx = (char)i;
            stAlm.ucStatus = SET_UNSTOP;
            stAlm.tWhen = tTime;
            sprintf(stAlm.Info,"%d",pid);
			sprintf(&stAlm.Info[32],"%s",szProcName[i]);

            dRet = FIDB_Handle_Proc(&stAlm);
            if(dRet < 0)
            {
                dAppLog(LOG_CRI,"dAct_prc : Failed in FIDB_Handle_Proc");
            }
			
			return 0;
		}
		else return -102;	
	}

	return 0;
}

INT dDACT_PRC(UCHAR* pszOldData, UCHAR* pszData)
{
   
	INT dRet, i, flag, p;
	time_t	tTime;
	st_AlmStat	stAlm;

	for( i = 0, flag = 1; i < g_dProcCnt; i++) {
        if (strncasecmp( (CHAR*)pszOldData, szProcName[i], 16) == 0) {
            flag = 0;
            break;
        }
    }

    if(flag) return -100;

    if((fidb->stStatus[i].stSts.stop == STOP)) return -101;
	else {

		if (i > 0 && i < g_dProcCnt)
		{
			p = get_proc_id(szProcName[i]);
			if (p > 0)
			{
				lastpid[i] = 0;
				proc[i] = DEAD;

				time(&tTime);
            	stAlm.ucLocType = SW_COM_IDX;
            	stAlm.ucLocIdx = (char)i;
            	stAlm.ucStatus = SET_STOP;
            	stAlm.tWhen = tTime;
				sprintf(stAlm.Info,"%d",lastpid[i]);
				sprintf(&stAlm.Info[32],"%s",szProcName[i]);

            	dRet = FIDB_Handle_Proc(&stAlm);
            	if(dRet < 0)
            	{
                	dAppLog(LOG_CRI,"dAct_prc : Failed in FIDB_Handle_Proc");
            	}
			
				if(i == 5) /* FSTATD IDX : 5 */
					sleep(1);

				kill(p, SIGTERM);	
            	return 0;
			} else return -101;
		}
		else if (i == 0) return -102;
		else return -103;
	}

	return 0;
}

INT dDIS_LOAD_STS(UCHAR* pszData)
{
	INT		dRet, dBodyLen = 0;
	ULONG 	ulSys, ulUser, ulNice, ulIdle, ulCpuTotal;
	INT		dMemTotal, dMemUsed;
	FLOAT	fMemPer;
	pst_DisLoadSts pstLoad;

	pstLoad = (pst_DisLoadSts)pszData;

	dRet = Read_CpuLoad( &ulSys, &ulUser, &ulNice, &ulIdle, &ulCpuTotal );
	if( dRet < 0 || ulCpuTotal == 0 ) return -104;

	pstLoad->fCPU_USER 	= ((FLOAT)ulSys/(FLOAT)ulCpuTotal)*100.0f;
    pstLoad->fCPU_SYS 	= ((FLOAT)ulNice/(FLOAT)ulCpuTotal)*100.0f;
    pstLoad->fCPU_NICE	= ((FLOAT)ulUser/(FLOAT)ulCpuTotal)*100.0f;
    pstLoad->fCPU_IDLE 	= ((FLOAT)ulIdle/(FLOAT)ulCpuTotal)*100.0f;
    pstLoad->fCPU_PER 	= 100.0f - pstLoad->fCPU_IDLE;

	dRet = Read_MemLoad( &dMemTotal, &dMemUsed, &fMemPer );
	if( dRet < 0 ) return -105;

	pstLoad->dMEM_TOTAL		= dMemTotal;
	pstLoad->dMEM_USED		= dMemUsed;
	pstLoad->fMEM_PER		= fMemPer;

	dBodyLen = sizeof(st_DisLoadSts);

#if 0
	FILE *read_fp;
	CHAR ch;

	if(( read_fp = popen( "df -h", "r")) == NULL ) return -1010;

	while( ( fscanf( read_fp, ) == 6) ) {
		pszData[dBodyLen] = ch;
		dBodyLen++;
	}

	pclose(read_fp);
#endif
	return dBodyLen;
}


int mask_alm( mml_msg *ml, dbm_msg_t *smsg )
{
	int mask;
	int loctype;
	int locidx;
	int	i, idx;
	int system;

    smsg->common.mml_err = DBM_SUCCESS;
    smsg->common.cont_flag = DBM_END;
    smsg->head.msg_len = 0;
	smsg->data[0] = 0x00;


	for( i=0, idx=0 ; i<ml->num_of_para; i++ )
	{
		dAppLog(LOG_DEBUG, "INPUT PARAM (%d) id(%d) type(%d) value(%s)",
			i, ml->msg_body[i].para_id, ml->msg_body[i].para_type, 
			ml->msg_body[i].para_cont );

	}

	mask = atoi(ml->msg_body[0].para_cont);
	system = atoi(ml->msg_body[1].para_cont);
	loctype = atoi(ml->msg_body[2].para_cont) - 600;
	locidx = atoi(ml->msg_body[3].para_cont);


    if(system > 0)
    {
		dAppLog(LOG_WARN, "mask_alm : Unvalid para_cont");
        return 0;
    }
#ifdef FORNTAM
	if( ((loctype > NTAF_CHNL_IDX) && (loctype < BDF_CHNL_IDX)) || (loctype > BDF_CHNL_IDX) )
	{
		smsg->common.mml_err = eENUM_LIST_ELEMENT;
		dSendMess(ml, smsg);

		return 0;
	}	
#else
	#ifdef FORNTAF
	if( loctype > LOAD_DISK_IDX ){

		smsg->common.mml_err = eENUM_LIST_ELEMENT;
		dSendMess(ml, smsg);

		return 0;
	}
	#endif
#endif

	if(( fidb->cCurrCnt[loctype] <= locidx ) || ( locidx < 0))
	{
		smsg->common.mml_err = eCannotBlockMask;
		dSendMess(ml, smsg);

		return 0;

	}

#ifdef FORNTAM
	if(loctype == BDF_CHNL_IDX)
	{
    	if( mask == 171 )
    	{
        	if( fidb->stBDFInfo[locidx].PWR_A.stSts.mask == 1 )
        	{
            	smsg->common.mml_err = eAlreadyMASK;
            	dSendMess(ml, smsg);
            	return 0;
        	}

        	fidb->stBDFInfo[locidx].PWR_A.stSts.mask = 1;
            fidb->stBDFInfo[locidx].PWR_B.stSts.mask = 1;
            fidb->stBDFInfo[locidx].SVR_A.stSts.mask = 1;
            fidb->stBDFInfo[locidx].SVR_B.stSts.mask = 1;
            fidb->stBDFInfo[locidx].SVR.stSts.mask = 1;
            fidb->stBDFInfo[locidx].HUB.stSts.mask = 1;
            fidb->stBDFInfo[locidx].HUB_A.stSts.mask = 1;
            fidb->stBDFInfo[locidx].HUB_B.stSts.mask = 1;
    	}
    	else if( mask == 172 )
    	{
        	if( fidb->stBDFInfo[locidx].PWR_A.stSts.mask == 0 )
        	{
            	smsg->common.mml_err = eAlreadyUMASK;
            	dSendMess(ml, smsg);
            	return 0;
        	}

            fidb->stBDFInfo[locidx].PWR_A.stSts.mask = 0;
            fidb->stBDFInfo[locidx].PWR_B.stSts.mask = 0;
            fidb->stBDFInfo[locidx].SVR_A.stSts.mask = 0;
            fidb->stBDFInfo[locidx].SVR_B.stSts.mask = 0;
            fidb->stBDFInfo[locidx].SVR.stSts.mask = 0;
            fidb->stBDFInfo[locidx].HUB.stSts.mask = 0;
            fidb->stBDFInfo[locidx].HUB_A.stSts.mask = 0;
            fidb->stBDFInfo[locidx].HUB_B.stSts.mask = 0;

    	}
    	else
    	{
        	smsg->common.mml_err = eENUM_LIST_ELEMENT;
        	dSendMess(ml, smsg);

        	return 0;

    	}

	}
#endif

	idx = 0;

	for( i=0; i<loctype; i++)
	{
		idx += fidb->cCurrCnt[i];
	}	


	idx += locidx;

	dAppLog(LOG_DEBUG, "STATUS IDX (%d) MASKINFO(%d)", idx , fidb->stStatus[idx].stSts.mask ); 

	if( mask == 171 )
	{
		if( fidb->stStatus[idx].stSts.mask == 1 )
		{
			smsg->common.mml_err = eAlreadyMASK;
			dSendMess(ml, smsg);
			return 0;
		}
		
		fidb->stStatus[idx].stSts.mask = 1;
		
	} 
	else if( mask == 172 )
	{
		if( fidb->stStatus[idx].stSts.mask == 0 )
		{
			smsg->common.mml_err = eAlreadyUMASK;
			dSendMess(ml, smsg);
			return 0;
		}
		
		fidb->stStatus[idx].stSts.mask = 0;

	}
	else
	{
		smsg->common.mml_err = eENUM_LIST_ELEMENT;
		dSendMess(ml, smsg);

		return 0;

	}

    smsg->common.mml_err = DBM_SUCCESS;
    smsg->common.cont_flag = DBM_END;
    smsg->head.msg_len = 0;
	smsg->data[0] = 0x00;

	dSendMess(ml, smsg);

	return 0;
}

int dis_mask_alm(mml_msg *ml, dbm_msg_t *smsg)
{
	FILE *fa;
    char szBuf[1024];
    char szName[64];
	char szData[MAX_STATUS_COM][64];
	int i, j, k;
	int flag, idx, ddisk;
	int SystemNo;

	if((SystemNo = atoi(ml->msg_body[0].para_cont)) > 0)
    {
		dAppLog(LOG_WARN, "dis_mask_alm : Unvalid para_cont");
        return 0;
    }

    fa = fopen(MASK_DATA_FILE,"r");
    if(fa == NULL)
    {
        dAppLog(LOG_CRI,"dis_mask_alm : %s FILE OPEN FAIL [%s]",MASK_DATA_FILE,strerror(errno));
        return -1;
    }

	i = 0;
    while(fgets(szBuf,1024,fa) != NULL)
    {
        dAppLog(LOG_DEBUG, "ReadBuf (%s)", &szBuf[2]);
        if(szBuf[0] != '#')
        {
            dAppLog(LOG_CRI,"dis_mask_alm : %s FILE format error",MASK_DATA_FILE);
            fclose(fa);
            return -1;
        }

        if(szBuf[1] == '#')
            continue;
        else if(szBuf[1] == 'E')
            break;
        else if(szBuf[1] == '@')
        {
            if(sscanf(&szBuf[2],"%s",szName) == 1)
            {
            	sprintf (szData[i], "%s", szName);
				i++;
            }
        }
    }
    fclose(fa);

    idx = 0;

    for( j=0; j<fidb->dTotCompCnt; j++)
    {
        idx += fidb->cCurrCnt[j];
		if( j == LOAD_QUE_IDX )
			ddisk = idx;
    }

    memset(szBuf,0x00,1024);
    sprintf(smsg->data,
    "--------------------------------------------------------------------------------");
    sprintf(szBuf,"\nNAME(INDEX)                STATUS");
    strcat(smsg->data,szBuf);
    sprintf(szBuf,
    "\n--------------------------------------------------------------------------------");
    strcat(smsg->data,szBuf);
	flag = 0;
    for(k=0;k<idx;k++)
    {
	    if(fidb->stStatus[k].stSts.mask == 1)
        {
            sprintf(szBuf,"\n%-20s       %-8s",szData[k], "MASK");
            strcat(smsg->data,szBuf);
            flag = 1;
        }
    }

	if(flag == 0)
	{
		sprintf(szBuf,"\n All UMASK");
		strcat(smsg->data,szBuf);
	} 

    sprintf(szBuf,
    "\n--------------------------------------------------------------------------------");
    strcat(smsg->data,szBuf);

    smsg->common.mml_err = DBM_SUCCESS;
    smsg->common.cont_flag = DBM_END;
    smsg->head.msg_len = strlen(smsg->data) + 1;

    dSendMess(ml, smsg);

    return 1;
}

int dis_mask_svc(mml_msg *ml, dbm_msg_t *smsg)
{
    return 1;
}

int mask_svc( mml_msg *ml, dbm_msg_t *smsg )
{
    return 0;
}


int ntaf_proc_ctl(mml_msg *ml)
{
	int		dRet, Snd_size;
    st_MsgQ stMsgQ;

    stMsgQ.usType = DEF_SYS;
    stMsgQ.usSvcID = SID_MML;
	stMsgQ.usMsgID = ml->msg_id;


    stMsgQ.usBodyLen = sizeof(mml_msg)+ PACK_HEADER_LEN;
    stMsgQ.ucProID = SEQ_PROC_CHSMD;
	
    memcpy(&stMsgQ.szBody[PACK_HEADER_LEN], ml, sizeof(mml_msg));
	
	Snd_size = sizeof(st_MsgQ) - MAX_MSGBODY_SIZE + stMsgQ.usBodyLen - sizeof(long);

	dRet = msgsnd(dSISVCQid, &stMsgQ, Snd_size, 0);
    if(dRet < 0)
    {
        dAppLog(LOG_CRI,"SEND FAIL for MMCD is not delivered : %s", strerror(errno));
        return -1;
    }

    return 1;

}

int dSendMess(mml_msg *ml,dbm_msg_t *smsg)
{
    int dRet;
    st_MsgQ stSndMsg;
	size_t		Snd_size;

    memset(&stSndMsg, 0x00, sizeof(st_MsgQ));

    smsg->head.src_proc = SEQ_PROC_CHSMD;
    smsg->head.dst_func = ml->src_func;
    smsg->head.dst_proc = ml->src_proc;
    smsg->head.cmd_id = ml->cmd_id;
    smsg->head.msg_id = ml->msg_id;

    smsg->common.TotPage = 0;
    smsg->common.CurPage = 0;

    stSndMsg.usType = DEF_SYS;
    stSndMsg.usSvcID = SID_MML;
    stSndMsg.usMsgID = MID_MML_RST;	

	stSndMsg.usRetCode = 0;				/* used in FSTATD */
	
	/* NOTE : +1 in FSTATD */
	stSndMsg.usBodyLen = sizeof(dbm_msg_t)-MSG_DATA_LEN+smsg->head.msg_len;

	memcpy( stSndMsg.szBody, smsg, stSndMsg.usBodyLen );	
	
	Snd_size = sizeof(st_MsgQ) - MAX_MSGBODY_SIZE + stSndMsg.usBodyLen - sizeof(long);

	dRet = msgsnd(dMmcdQid, &stSndMsg, Snd_size, 0);
    if(dRet < 0)
    {
        dAppLog(LOG_CRI,"SEND FAIL for MMCD is not delivered : %s", strerror(errno));
        return -1;
    }
	else{
	    dAppLog(LOG_DEBUG," * SUCCESS SEND TO MSG [%s]",smsg->data);
	}
    return 1;
}
