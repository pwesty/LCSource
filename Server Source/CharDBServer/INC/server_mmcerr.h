#ifndef _MMCERR_H_
#define _MMCERR_H_

/**A.1*  File Inclusion ***********************************/
/**B.1*  Definition of New Constants *********************/

#define	DBM_SUCCESS					0
#define DBM_END						0
#define DBM_CONTINUE				1
#define DBM_NOT_LOGON				2
#define DBM_TIMEOUT                 3
#define DBM_FAILURE					-1

#define CONT						20

#define eDBQUERYERROR           -610
#define ePRNTNOTFOUND           -611
#define eCHDRCDFOUND            -612
#define eOVERMAXSVC				-613



#define eMMCDTimeOut				-23

/* General Error */
#define eBadParameter               -1001   /* BAD PARAMETER */

#define eFileNotFound				-1002  /* Conf File Not Found */
#define eDataReadError				-1003  /* Data Read Error */
#define eAlreadyMASK				-1004  /* Already MASK ALM */
#define eAlreadyUMASK				-1005  /* Already MASK ALM */
#define eNotSupportIdx				-1006  /* UnInitial Component Idx */
#define eBiggerWARNToCRI			-1007  /* Bigger WARN Than CRI */

#define eMissingMandatory			-1008 	/* MISSING MANDATORY */

#define eBlockNotRegistered			-1020
#define eProcAliveError             -1021
#define eCHSMDNotDEAD               -1022
#define eCHSMDNotALIVE              -1023
#define eProcDeadError              -1024
#define eProcMaskError              -1025
#define eProcUMaskError				-1026
#define eNeedProcName				-1027

#define eCannotBlockMask			-1030

#define eInvalidSysNo				-1040

#define eInvalidNasInfo				-1050
#define eNotFoundData				-1051

#define eGeneralError  -1102

#define eMANDATORY_MISSED       -509

#define eENUM_LIST_FORMAT		-510
#define eENUM_LIST_ELEMENT		-511

#define eLOAD_CMD_FILE          -520
#define eINVALID_CMD            -521
#define eINVALID_PAPA_IN_CMD    -522
#define eINVALID_SUBSYSTEM      -523
#define eINVALID_PAPA_CONTENT   -524
#define eINVALID_PAPA_ID   		-525


#define eNOT_FOUND_PRN_FUNC     -530

#define eDUPLICATE_DSCP_IP		-534
#define eNO_SESSION             -535
#define eNO_SUCH_SOCKET_FD      -536
#define eINVALID_DSCP_ID        -537
#define eRELOAD_FAIL_DSCP_INFO  -538
#define eCONNECT_REFUSE_BY_DSCP -539
#define eINACT_DSCP_ID          -540
#define eMAX_SESSION_CNT_OVER   -541
#define eALREADY_INACT_DSCPID   -542
#define eALREADY_ACT_DSCPID     -543
#define eREQUIRED_CHANGE_INFO   -544
#define eDUPLICATE_DSCP_ID      -545
#define eMAX_DSCP_LIST_OVER     -546
#define eREQUIRED_MORE_PARA     -547
#define eNOTREG_WIN_SERVICE     -548
#define eALREADY_EXIST_ROUTE    -549
#define ePFX_OUT_OF_BOUND       -550
#define eNO_SUCH_DSCPID         -551
#define eNO_SUCH_DSCP_ROUTE     -552
#define eINVALID_DSCP_TIMER     -553
#define eFAIL_CREATE_ROUTE_LIST	-554
#define eOVERFLOW_ROUTE_LIST	-555

/* MASK CHANNEL : 2003. 5. 7 *************/
#define	eALREADY_MASK_CHNL		-556
#define	eALREADY_UMASK_CHNL		-557
#define	eNO_SUCH_IPADDRESS		-558
#define eINVALIDE_BDF_RANGE		-559

#define eSETIPAFCONFFILE		 -560
#define eSETIPAFCONFPARAUNDEFINE -561

#define eSERCATFILENOTFOUND      -562
#define eIPPOOLLISTFILENOTFOUND  -563
#define eINVALIDE_IPAM_RANGE		-564
#define eINVALIDE_SEARCH_TIME	-565
#define eINVALIDE_IPAM_LOAD_RANGE		-566

/* 2003. 7. 22 */
#define	eMAX_SVC_LIST_OVER		-570
#define	eDUPLICATE_SVC_LIST		-571
#define	eNO_SUCH_WIN_SERVICE	-572

#define eINVALID_IP				-700
#define eINVALID_IP_RANGE		-701

#define eNEED_CHG_DATA			-705

/* 2003/4/14 hwh add */
#define eINVALID_IPAFNO			-2001
#define eINVALID_IPADDRESS		-2002
#define eINVALID_NASTYPE		-2003
#define eALREADY_IP				-2004
#define eNOTREG_IP				-2005
#define ePREFIX_ERR				-2006
#define eNOTREG_PREFIX			-2007
#define eALREADY_PREFIX			-2008
#define eNOTREG_SERCAT			-2009
#define ePKG_VER_ERR			-2010
#define eIPAF_NOT_LOGIN			-2011
#define eALREADY_VER			-2012
#define eSERCAT_MAX_OVER		-2013
#define eCATEGORY_MAX_OVER		-2014
#define eALREADY_SERVICE		-2015
#define eGROUPPORT				-2016
#define ePERIOD_OVER			-2017
#define eNETMASK_ERROR			-2018
#define eGRUPPORTERR			-2019
#define eNASTYPEERROR			-2020

#define eUNKNOWN_PRCNAME		-3001	/* added by LYH 20050812 */

#define	eBDFMaskSetFail			-573

#define eEQUIPNotRegistered		-574

#define eAlreadyMaskNtafChn		-2030
#define eAlreadyUMaskNtafChn	-2031
#define eNotFindSysNo			-2032

#endif
