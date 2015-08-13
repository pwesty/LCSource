/***********************************************************************************************************/
/* [TAG] Xtrap_S_Interface.h | 2010.10.06 | (Dynamic Link) X-TRAP Interface Library For Server             */
/*                                                                                                         */
/*  Copyright (C)WiseLogic 2005 - 2010 All Rights Reserved                                                 */
/***********************************************************************************************************/

#ifndef __WISELOGIC_Xtrap_S_Interface_H_INCLUDED
#define __WISELOGIC_Xtrap_S_Interface_H_INCLUDED

#ifdef WIN32
#define XTRAPCC_CALLCONV __cdecl
#else
#define XTRAPCC_CALLCONV
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (X-TRAP) Reference Macro Definition
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define XTRAP_SETINFO_PACKETBUFF_SIZE				128
#define XTRAP_CS4_BUFSIZE_MAP						13000

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (X-TRAP) Return Values
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define XTRAP_API_RETURN_OK							0x00000000
#define XTRAP_API_RETURN_ERROR						0x00000001
#define XTRAP_API_RETURN_UNKNOWN					0x00000002	/*unused*/
#define XTRAP_API_RETURN_INVALID_PARAMETER			0x00000003	/*XTrap_CS_Step3*/
#define XTRAP_API_RETURN_INVALID_CRC				0x00000004	/*unused*/
#define XTRAP_API_RETURN_TIMEOUT					0x00000005	/*unused*/
#define XTRAP_API_RETURN_DETECTTIMEOUT				0x00000006	/*unused*/
#define XTRAP_API_RETURN_INVALID_FILEVERSION		0x00000007
#define XTRAP_API_RETURN_INVALID_COMMAND			0x00000008  /*XTrap_CS_Step3*/
#define XTRAP_API_RETURN_DETECTHACK					0x0000000F	/*XTrap_CS_Step1, XTrap_CS_Step3*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (X-TRAP) (Example) Internal Definition Function
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned int XTrap_S_LoadDll();
unsigned int XTrap_S_FreeDll();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (X-TRAP) Export Function
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int (XTRAPCC_CALLCONV *PFN_XTrap_S_Start) (
	IN		unsigned int	ulTimeOut,
	IN		unsigned int	ulMapCount,
	IN		const void *	pBufMap,
	IN OUT	void *			Reserved
);

typedef unsigned int (XTRAPCC_CALLCONV *PFN_XTrap_S_SessionInit) (
	IN		unsigned int	ulTimeOut,
	IN		unsigned int	ulMapCount,
	IN		const void *	pBufMap,
	IN OUT	void *			pBufSession
);

typedef unsigned int (XTRAPCC_CALLCONV *PFN_XTrap_CS_Step1)	(
	IN OUT	void *			pBufSession,
	IN		void *			pBufPackData
);

typedef unsigned int (XTRAPCC_CALLCONV *PFN_XTrap_CS_Step3)	(
	IN OUT	void *			pBufSession,
	IN		const void *	pBufPackData
);

#ifndef __Xtrap4Server_a_import_H
extern PFN_XTrap_S_Start			XTrap_S_Start;
extern PFN_XTrap_S_SessionInit		XTrap_S_SessionInit;
extern PFN_XTrap_CS_Step1			XTrap_CS_Step1;
extern PFN_XTrap_CS_Step3			XTrap_CS_Step3;
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (X-TRAP) Export (Reserved) Function
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define XTRAP_ACTIVE_CODE_DEFAULT					0x0FFFFFFF
#define XTRAP_ACTIVE_CODE_THEMIDA					0x0FFFFFC3
#define XTRAP_ACTIVE_CODE_NOTUSE_MAPFILE			0x0FFFFF0C

#define XTRAP_ACTIVE_CODE_LEVEL1					XTRAP_ACTIVE_CODE_DEFAULT
#define XTRAP_ACTIVE_CODE_LEVEL2					XTRAP_ACTIVE_CODE_THEMIDA
#define XTRAP_ACTIVE_CODE_LEVEL3					XTRAP_ACTIVE_CODE_NOTUSE_MAPFILE

#define XTRAP_CS_OPTION_NULL						0x00000000
#define XTRAP_CS_OPTION_USETIME						0x00000001
#define XTRAP_CS_OPTION_DELAY						0x00000002

#define XTRAP_ALLOW_DELAY_1							0x00000001
#define XTRAP_ALLOW_DELAY_2							0x00000002
#define XTRAP_ALLOW_DELAY_3							0x00000003
#define XTRAP_ALLOW_DELAY_DEFAULT					XTRAP_ALLOW_DELAY_1
#define XTRAP_ALLOW_DELAY_MAX						XTRAP_ALLOW_DELAY_3

typedef unsigned int (XTRAPCC_CALLCONV *PFN_XTrap_S_SetActiveCode) (
	IN		unsigned int	ulActiveCode
);

typedef unsigned int (XTRAPCC_CALLCONV *PFN_XTrap_S_SetOption) (
	IN		unsigned int	ulMethod,
	IN OUT	void *			pArray
);

typedef unsigned int (XTRAPCC_CALLCONV *PFN_XTrap_S_SetAllowDelay) (
	IN		unsigned int	uAllowDelayCycle
);

typedef unsigned int (XTRAPCC_CALLCONV *PFN_XTrap_S_SendGamePacket)	(
	IN		void *			pBufSession
);

typedef unsigned int (XTRAPCC_CALLCONV *PFN_XTrap_S_RecvGamePacket)	(
	IN		void *			pBufSession
);

#ifndef __Xtrap4Server_a_import_H
extern PFN_XTrap_S_SetActiveCode	XTrap_S_SetActiveCode;
extern PFN_XTrap_S_SetOption		XTrap_S_SetOption;
extern PFN_XTrap_S_SetAllowDelay	XTrap_S_SetAllowDelay;
extern PFN_XTrap_S_SendGamePacket	XTrap_S_SendGamePacket;
extern PFN_XTrap_S_RecvGamePacket	XTrap_S_RecvGamePacket;
#endif

#ifdef __cplusplus
}
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (X-TRAP) Compatible Preprocessor Macro
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Buffer Size
#define XTRAP_CS4_COMSIZE_PACKDATA					128
#define XTRAP_CS4_BUFSIZE_SESSION					320
#define XTRAP_CS4_BUFSIZE_SESSIONADDON				80

// Timeout Limit
#define XTRAP_CS4_MAXTIMEOUT						(int)600
#define XTRAP_CS4_MINTIMEOUT						(int)100

// Protect Option
#define  XTRAP_PROTECT_PE                           0x00000001
#define  XTRAP_PROTECT_TEXT                         0x00000010
#define  XTRAP_PROTECT_RDATA                        0x00000100
#define  XTRAP_PROTECT_EDATA                        0x00001000
#define  XTRAP_PROTECT_RSRC                         0x00010000
#define  XTRAP_PROTECT_RELOC                        0x00100000
#define XTRAP_PROTECT_EXCEPT_VIRUS                  0x00000002

// XTrap_Module Method
#define XTRAP_CS_MODULE_DM_TIMEOUT					0x00000001       // Detect Method	: Timeout   (on)
#define XTRAP_CS_MODULE_LOG_BUF                     0x00000100       // Log Method      : Buffer    (on)
#define XTRAP_CS_MODULE_LOG_FILE                    0x00000200       // Log Method      : File		(off)

#endif
//
