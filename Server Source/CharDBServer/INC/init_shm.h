#ifndef __INIT_SHM_HEADER___
#define __INIT_SHM_HEADER___

/**********************************************************
                 KTF IPAF Project

   Author   : joonhk
   Section  :
   SCCS ID  : init_shm.h
   Date     : 04/04/06
   Revision History :
    '03.  1. 7  Initial
    '04.  4. 6  Update By LSH For SHM ERROR Definition

   Description:


   Copyright (c) Infravalley 2003
***********************************************************/

/**A.1*  File Inclusion ***********************************/

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
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>

/* add mmdb structure define header */
#include <server_shm.h>

/*
#include <ipaf_define.h>
#include <ipaf_shm2.h>
#include <ipaf_define.h>

#include <mmdb_obj.h>
#include <mmdb_sess.h>
#include <mmdb_cdr.h>
#include <mmdb_destip.h>
#include <mmdb_destport.h>
*/

/**B.1*  Definition of New Constants **********************/

#define SHM_CREATE          1       /* Shared Memory Created */
#define SHM_EXIST           5       /* Shared Memory Exist */
#define SHM_ERROR1          -1      /* Shared Memory shmget() Error & EXIST shmget() Error -6 */
#define SHM_ERROR2          -2      /* Shared Memory shmat() Error & EXIST shmat() Error -7 */
#define SHM_ERROR3          -3      /* Shared Memory shmdt() Error used REMOVE SHM Funcation */
#define SHM_ERROR4          -4      /* Shared Memory shmctl() Error used REMOVE SHM Funcation */
#define SHM_ERROR_ETC       -5      /* Shared Memory ETC Error used REMOVE SHM Funcation */

#define ERR_SHM_FIDB        -100    /* FIDB SHM ERROR */
#define ERR_SHM_KEEP        -200    /* KEEPALIVE SHM ERROR */
#define ERR_SHM_CAPBUF      -50     /* CAPBUF SHM ERROR with CAP ERROR */
#define ERR_SHM_CAP         -300    /* CAP SHM ERROR */
#define ERR_SHM_GEN         -400    /* GENINFO SHM ERROR */
#define ERR_SHM_OBJ         -500    /* MMDB OBJ SHM ERROR */
#define ERR_SHM_SESS        -600    /* MMDB SESS SHM ERROR */
#define ERR_SHM_CDR         -700    /* MMDB CDR SHM ERROR */
#define ERR_SHM_DESTIP      -800    /* MMDB DESTIP SHM ERROR */
#define ERR_SHM_DESTPORT    -900    /* MMDB DESTPORT SHM ERROR */

#define ERR_SHM_MESTAT		-1000	/* ME STAT SHM */
#define ERR_SHM_KUNSTAT		-1100	/* KUN STAT SHM */
#define ERR_SHM_ADSSTAT		-1200	/* ADS STAT SHM */
#define ERR_SHM_MARSSTAT	-1300	/* MARS STAT SHM */
#define ERR_SHM_SESSSTAT	-1400	/* SESS STAT SHM */

#define ERR_SHM_VERSION		-1500	/* VERSION */

#define ERR_SHM_MESVC		-1600	/* MESVC SHM */		/*** added by SJS ***/
#define ERR_SHM_KUNSVC		-1700	/* KUNSVC SHM */
#define ERR_SHM_ADSSVC		-1800	/* ADSSVC SHM */
#define ERR_SHM_MARSSVC		-1900	/* MARSSVC SHM */
#define ERR_SHM_SESSANA		-2000	/* SESSANA SHM */

#define ERR_SHM_RDRSEQ		-2100	/* RDR SEQ SHM */

#define ERR_SHM_MACSSTAT	-2200	/* MACSSTAT SHM */
#define ERR_SHM_MACSSVC		-2300	/* NACSSVC	SHM */
#define ERR_SHM_VODANA		-2400	/* VODANA	SHM */
#define ERR_SHM_VODMANA		-2500	/* VODMANA 	SHM */
#define ERR_SHM_VODDANA		-2600	/* VODDANA 	SHM */
#define ERR_SHM_WICGSSTAT	-2700	/* WICGSSTAT	SHM */
#define ERR_SHM_WICGSSVC	-2800	/* WICGSSVC	SHM */
#define ERR_SHM_VODUDP		-2900	/* VODUDP	SHM */

#define ERR_SHM_VODSSTAT	-3000
#define ERR_SHM_VODMSTAT	-3100
#define ERR_SHM_VODDSTAT	-3200

/**B.2*  Definition of New Type  **************************/
/**C.1*  Declaration of Variables  ************************/
extern T_CAP *shm_cap;


#define DUP_METER       1000000


/**D.1*  Definition of Functions  *************************/

int Init_shm_all( int dFidb );
int Init_shm( key_t shm_key, int size, void **memptr );
int Init_Cap();
int Remove_shm_all();
int Init_GEN_INFO(void);
int set_version(int prc_idx, char *ver);
unsigned long long timestamp(void);
int	dInit_NationConf();

#endif

