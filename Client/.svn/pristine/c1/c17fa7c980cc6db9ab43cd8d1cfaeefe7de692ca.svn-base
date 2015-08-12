
/***********************************************************************************************************/
/* [TAG] Xtrap_C_Interface.h | 2009.11.03 | X-TRAP Interface Library For Client                            */
/*                                                                                                         */
/*  Copyright (C)WiseLogic 2005 - 2009 All Rights Reserved                                                 */
/***********************************************************************************************************/

#ifndef __Xtrap_C_Interface_H
#define __Xtrap_C_Interface_H

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#define _USE_XTRAP_MODULE

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Use for Cluod Service
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define _XTRAP_CLOUDSERVICE_MODE
#ifdef _XTRAP_CLOUDSERVICE_MODE
#undef _USE_XTRAP_MODULE
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Return Values
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define XTRAP_API_RETURN_OK							0x00000000
#define XTRAP_API_RETURN_ERROR						0x00000001


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// XTrap_C_SecureEngineDetour Mode
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define XTRAP_SE_DETOUR_MODE_DEFALULT			0x00010001
#define XTRAP_SE_DETOUR_MODE_EXTENTION			0x00010010

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Library Export Declaration (for CLIENT) Function
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _USE_XTRAP_MODULE

	void XTrap_C_Start( 
			IN	LPCSTR	lpArgv, 
			IN	LPCSTR	lpXTrapPath 
	);

	extern "C"
	void XTrap_C_StartW( 
			IN	LPCSTR	lpArgv, 
			IN	LPCWSTR lpXTrapPath
	);


	void XTrap_C_KeepAlive();

#else

	#define XTrap_C_Start			
	#define XTrap_C_StartW			
	#define XTrap_C_KeepAlive		

#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Library Export Declaration Extended (for CLIENT) Function
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _USE_XTRAP_MODULE

	void XTrap_C_SetUserInfoEx(
			IN	LPCSTR	lpUserID, 
			IN	LPCSTR	lpServerName, 
			IN	LPCSTR	lpCharacterName, 
			IN	LPCSTR	lpCharacterClass, 
			IN	DWORD	dwReserve,
			IN	SOCKET	Socket);

	void XTrap_C_CallbackAlive(
			IN	DWORD	Period/*milliseconds*/);
#else

	#define XTrap_C_SetUserInfoEx
	#define XTrap_C_CallbackAlive

#endif

#ifdef _USE_XTRAP_MODULE

	DWORD XTrap_C_Detour();
	DWORD XTrap_C_SecureEngineDetour(DWORD dwMode);

#else

	#define XTrap_C_Detour
	#define XTrap_C_SecureEngineDetour

#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Library Export Declaration (for SERVER) Function
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// XTrap_CS_Step2 . dwMethod ////////////////////////////////////////////////////////////////////////////////
// XTrap_CS_Step2_M . dwMethod //////////////////////////////////////////////////////////////////////////////

#define	XTRAP_PROTECT_PE							0x00000001
#define	XTRAP_PROTECT_TEXT							0x00000010
#define XTRAP_PROTECT_CODE							XTRAP_PROTECT_TEXT
#define	XTRAP_PROTECT_RDATA							0x00000100
#define	XTRAP_PROTECT_EDATA							0x00001000
#define	XTRAP_PROTECT_RSRC							0x00010000
#define	XTRAP_PROTECT_RELOC							0x00100000
#define XTRAP_PROTECT_EXCEPT_VIRUS					0x00000002

#ifdef _USE_XTRAP_MODULE

	DWORD __cdecl XTrap_CS_Step2(
			IN  LPCVOID	lpBufPackData_IN, 
			OUT LPVOID	lpBufPackData_OUT,
			IN	DWORD	dwMethod
	);

	DWORD __cdecl XTrap_CS_Step2_M/*MultiSession*/(
			IN  LPCVOID	lpBufPackData_IN, 
			OUT LPVOID	lpBufPackData_OUT, 
			IN  DWORD	dwMethod,
			IN  SOCKET	ClientSocket
	);

	DWORD __cdecl XTrap_CS_Stop_M/*MultiSession*/(
			IN  SOCKET	ClientSocket
	);

#else

	#define XTrap_CS_Step2			

	#define XTrap_CS_Step2_M
	#define XTrap_CS_Stop_M

#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Library Export Declaration Undocument (for CLIENT) Function
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WINAPI XTRAP_CALLBACK__XtrapCallbackProcedure (
	IN unsigned int Reserve1,
	IN unsigned int Reserve2,
	IN unsigned int Reserve3,
	IN unsigned int Reserve4,
	IN unsigned char *pXtrapCode	// [in, out] Pointer to a XTRAP_CODE data structure
);

#ifdef _USE_XTRAP_MODULE

	void __cdecl XTrap_C_SetSecondaryProcess(
			IN	unsigned int ProcessId
	);

	void __cdecl XTrap_C_Start_HGW( 
			IN	LPCSTR	lpArgv, 
			IN	LPCSTR	lpXTrapPath
	);

	void __cdecl XTrap_C_Start_Callback( 
			IN	LPCSTR	lpArgv, 
			IN	LPCSTR	lpXTrapPath,
			IN	void *	pXtrapCallbackProcedure		// Pointer to the Xtrap-defined function. see XTRAP_CALLBACK__XtrapCallbackProcedure
	);

	void __cdecl XTrap_C_Terminate();

#else

	#define XTrap_C_SetSecondaryProcess
	#define XTrap_C_Start_HGW
	#define XTrap_C_Start_Callback
	#define XTrap_C_Terminate

#endif

#endif
