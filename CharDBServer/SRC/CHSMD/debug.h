#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <logutillib.h>
#include <fidb_info.h>
#include <msgdef.h>
#include <shmutillib.h>
#include <sockdef.h>

#ifndef tMASK
#define tMASK 0x80
#endif	/* tMASK */
#ifndef cSTOP
#define cSTOP 3
#endif	/* cSTOP */

/**********************************************************************
FOR DEBUG
**********************************************************************/
void pprint2( unsigned char *p, int size )
{
	int pos,i,len;
	char buf[64];
	unsigned char *dp;

	dp = p;

	memset(buf,0x00,64);
	for( i=0, pos=0, len=0; i< size; i++)
	{
		if( i%16 == 0 )
		{
			sprintf(&buf[len], "%04d ",i);
			len += strlen(&buf[len]);
		}
		else if( i%4 == 0 )
		{
			sprintf(&buf[len],": ");
			len += strlen(&buf[len]);
		}
		sprintf(&buf[len], "%02X ", dp[i] );
		len += strlen(&buf[len]);

		if( i%16 == 15 )
		{
			dAppLog(LOG_INFO,"%s",buf);
			len =0;
		}
	}
	if( len != 0 )
	{
		dAppLog(LOG_INFO,"%s",buf);
	}
}
void chkpacket( unsigned char *p, int sockflag )
{
	pst_MsgQ	pmsg;
	pst_SockHeader	psock;
	pst_Almmsg	palm;
	pst_AlmStat	palmsts;
	pst_AType	pa;
	int			datalen;
	int			len;
	int			cnt;
	int			pos;
	unsigned char sts;
	unsigned char old;
	unsigned char alm;
	unsigned short	load;

	pos = 0;

	/* MSGQ HEADER */
	pmsg = (pst_MsgQ)p;

	/*** FORMAT ***
		MSGHDR : TYPE(5000) SVCID(1) MSGID(1) NID(39483984) BODYLEN(44) PROID(0) SYSNO(3)
	***/
	dAppLog(LOG_INFO," MSGHDR : TYPE(%d) SVCID(%d) MSGID(%d) NID(%lld) BODYLEN(%d) PROID(%d) SYSNO(%d) ",
			pmsg->usType, pmsg->ucSvcID, pmsg->ucMsgID, pmsg->llNID,
			pmsg->usBodyLen, pmsg->usProID, pmsg->ucSystemNo );

	pos += DEF_MSGHEAD_LEN;
	datalen = pmsg->usBodyLen - DEF_ALMHDR_LEN;

	if( sockflag == 1 )
	{

		/* SOCK_HEADER ADDED CASE */
		psock = (pst_SockHeader)&p[pos];

		/*** LOG FORMAT ***
				SOCKINFO MAGIC(0x32139382748374) TOTAL(399) BODY(393) SYSTEM(3) SVCID(38) MSGID(34)

		***/
		dAppLog(LOG_INFO,"SOCKINFO MAGIC(%llx) TOTAL(%d) BODY(%d) SYSTEM(%d) SVCID(%d) MSGID(%d)",
				psock->llMagicNumber, psock->usTotLen, psock->usBodyLen,
				psock->ucSystemNo, psock->ucSvcID, psock->ucMsgID );

		pos += SOCK_HEADER_LEN;
		datalen -= SOCK_HEADER_LEN;
	}

	if( pmsg->ucMsgID == MID_CONSOL )
	{

		/* MID_CONSOL */
		palmsts = (pst_AlmStat)&p[pos];

		/*** FORMAT ***
				ALMSTS : TYPE(3) IDX(4) STS(1) NORMAL -> CRITICAL, TIME(8383838383)
				ALMSTS : TAFNO(2) TAMNO(1) INFO()
		***/
#ifndef CVT_ORDER
		dAppLog(LOG_INFO,"ALMSTS : TYPE(%d) IDX(%d) STS(%d) %s -> %s, TIME(%ld)",
				palmsts->ucLocType, palmsts->ucLocIdx, palmsts->ucStatus,
				palmsts->ucOldAlmLevel == CRITICAL? "CRITICAL" : palmsts->ucOldAlmLevel == WARN ? "WARNING" : "NORMAL",
				palmsts->ucAlmLevel == CRITICAL ? "CRITICAL" : palmsts->ucAlmLevel == WARN ? "WARNING" : "NORMAL",
				palmsts->tWhen );
		dAppLog(LOG_INFO,"ALMSTS : TAFNO(%d) TAMNO(%d) INFO(%s)",
				palmsts->ucTafno, palmsts->ucTamno, palmsts->Info);
#else
		dAppLog(LOG_INFO,"ALMSTS : TYPE(%d) IDX(%d) STS(%d) %s -> %s, TIME(%ld)",
				palmsts->ucLocType, palmsts->ucLocIdx, palmsts->ucStatus,
				palmsts->ucOldAlmLevel == CRITICAL? "CRITICAL" : palmsts->ucOldAlmLevel == WARN ? "WARNING" : "NORMAL",
				palmsts->ucAlmLevel == CRITICAL ? "CRITICAL" : palmsts->ucAlmLevel == WARN ? "WARNING" : "NORMAL",
				ntohl(palmsts->tWhen) );
		dAppLog(LOG_INFO,"ALMSTS : TAFNO(%d) TAMNO(%d) INFO(%s)",
				palmsts->ucTafno, palmsts->ucTamno, palmsts->Info);
#endif

		return;
	}

	/* MID_ALARM */
	palm = (pst_Almmsg)&p[pos];
	alm = palm->head.ucTAlmsts;

	/*** FORMAT ***
		ALMMSG : [NTAF] TAFNO(0) TAMNO(3) WHOLE_ALM( WARNING ) TIME(303030303)
		ALMMSG : [NTAF] TAFNO(0) TAMNO(3) WHOLE_ALM( MASKED(WARNING) ) TIME(303030303)
	***/
#ifndef CVT_ORDER
	dAppLog(LOG_INFO," ALMMSG : [%s] TAFNO(%d) TAMNO(%d) WHOLE_ALM( %s ) TIME(%ld) ",
			palm->head.ucSysType == 0 ? "NTAF": palm->head.ucSysType == 1 ? "NTAM" : "GTAM",
			palm->head.ucTafno, palm->head.ucTamno,
			( alm >> 7 ) == MASK ?\
			( ( alm & CRITICAL ) == CRITICAL ? "MASKED(CRITICAL)" : ( alm & WARN ) == WARN ? "MASKED(WARNING)" : "MASKED(NORMAL)" ) :\
			( alm & CRITICAL ) == CRITICAL ? "CRITICAL" : ( alm & WARN ) == WARN ? "WARNING" : "NORMAL",
			palm->head.tChkWhen );

#else
	dAppLog(LOG_INFO," ALMMSG : [%s] TAFNO(%d) TAMNO(%d) WHOLE_ALM(%s) TIME(%ld) ",
			palm->head.ucSysType == 0 ? "NTAF": palm->head.ucSysType == 1 ? "NTAM" : "GTAM",
			palm->head.ucTafno, palm->head.ucTamno,
			( alm >> 7 ) == MASK ?\
			( ( alm & CRITICAL ) == CRITICAL ? "MASKED(CRITICAL)" : ( alm & WARN ) == WARN ? "MASKED(WARNING)" : "MASKED(NORMAL)" ) :\
			( alm & CRITICAL ) == CRITICAL ? "CRITICAL" : ( alm & WARN ) == WARN ? "WARNING" : "NORMAL",
			ntohl(palm->head.tChkWhen) );
#endif

	pos += DEF_ALMHDR_LEN;
	p = &p[pos];

	pos = 0;

	do
	{

		/* AType Header */
		pa = (pst_AType)&p[pos];
		len = pa->ucCLen;
		cnt = pa->ucCCnt;
		pos += DEF_AHEAD_LEN;

		if( cnt <= 0 )
			break;

		/*** FORMAT ***
				AHEAD : TYPE(3) CLEN(43) CCNT(3) UNMASKED, WARNING
		***/
		dAppLog(LOG_INFO," ATYPE :  TYPE(%d) CLEN(%d) CCNT(%d) %s, %s ",
				pa->ucAType, pa->ucCLen, pa->ucCCnt,
				( alm >> 7 ) == MASK ? "MASKED" : "UNMASKED",
				( alm & cSTOP) == cSTOP ? "STOP" : ( alm & CRITICAL ) == CRITICAL ? "CRITICAL": ( alm & WARN ) == WARN ? "WARNING" : "NORMAL" );

		while(cnt)
		{

			/* sub */
			old = p[pos +1];

			if( pa->ucAType == SW_COM_IDX || pa->ucAType == HW_ETH_IDX || pa->ucAType == DB_CONN_IDX )
			{
				/* sub1 */
				sts = p[pos +2];
				/*** FORMAT ***
				                SUBTYPE1 : IDX(%d) ucCID(%d) MASKED, NORMAL -> CRITICAL
				***/
				dAppLog(LOG_INFO,"SUBTYPE1 IDX(%d) ucCID(%d) %s, %s -> %s ",
						pa->ucCCnt - cnt +1, p[pos],
						( sts >> 7 ) == MASK ? "MASKED" : "UNMASKED",
						( old == CRITICAL ) ? "CRITICAL" : ( old == WARN) ? "WARNING" : "NORMAL",
						( sts & cSTOP) == cSTOP ? "STOP" : ( sts & CRITICAL ) == CRITICAL ? "CRITICAL": ( sts & WARN ) == WARN ? "WARNING" : "NORMAL" );
			}
			else
			{
				/* sub2 - with load_value */
				memcpy( &load, (void*)&p[pos +2], 2 );
				sts = p[pos +4];
				/*** FORMAT ***
				                SUBTYPE2 : IDX(%d) ucCID(%d) LOAD( 38.90% ), UNMASKED, NORMAL -> CRITICAL
				***/
#ifndef CVT_ORDER

				dAppLog(LOG_INFO,"SUBTYPE2 IDX(%d) ucCID(%d) LOAD(%d.%02d%%), %s, %s -> %s ",
						pa->ucCCnt - cnt +1, p[pos],
						load/100, load%100,
						( sts >> 7 ) == MASK ? "MASKED" : "UNMASKED",
						( old == CRITICAL ) ? "CRITICAL" : ( old == WARN ) ? "WARNING" : "NORMAL",
						( sts & cSTOP) == cSTOP ? "STOP" : ( sts & CRITICAL ) == CRITICAL ? "CRITICAL": ( sts & WARN ) == WARN ? "WARNING" : "NORMAL" );

#else
				dAppLog(LOG_INFO,"SUBTYPE2 IDX(%d) ucCID(%d) LOAD(%d.%02d%%), %s, %s -> %s ",
						pa->ucCCnt - cnt +1, p[pos],
						ntohs(load)/100, ntohs(load)%100,
						( sts >> 7 ) == MASK ? "MASKED" : "UNMASKED",
						( old == CRITICAL ) ? "CRITICAL" : ( old == WARN ) ? "WARNING" : "NORMAL",
						( sts & cSTOP) == cSTOP ? "STOP" : ( sts & CRITICAL ) == CRITICAL ? "CRITICAL": ( sts & WARN ) == WARN ? "WARNING" : "NORMAL" );
#endif
			}/* if */
			pos += len;
			cnt--;

		}/* while */

	}
	while( datalen > pos );
	return;

}/* end of chkpacket */

#endif /* _DEBUG_H_ */
