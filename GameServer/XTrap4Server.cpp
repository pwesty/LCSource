/***********************************************************************************************************/
/* [TAG] Xtrap4Server.c | 2010.10.06 | (Sample) X-TRAP Interface Library For Server                        */
/*                                                                                                         */
/*  Copyright (C)WiseLogic 2005 - 2010 All Rights Reserved                                                 */
/***********************************************************************************************************/

// If "stdafx.h" is being used, "XTrap4Server.c" need to be renamed as "XTrap4Server.cpp"
//#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>

#include "../ShareLib/Config.h"

#ifdef XTRAP

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#include <io.h>
#include <windows.h>
#else
#include <dlfcn.h>
#include <unistd.h>
#endif

#include "Xtrap_S_Interface.h"

PFN_XTrap_S_Start			XTrap_S_Start=0;
PFN_XTrap_S_SessionInit		XTrap_S_SessionInit=0;
PFN_XTrap_CS_Step1			XTrap_CS_Step1=0;
PFN_XTrap_CS_Step3			XTrap_CS_Step3=0;

PFN_XTrap_S_SetActiveCode	XTrap_S_SetActiveCode=0;
PFN_XTrap_S_SetOption		XTrap_S_SetOption=0;
PFN_XTrap_S_SetAllowDelay	XTrap_S_SetAllowDelay=0;
PFN_XTrap_S_SendGamePacket	XTrap_S_SendGamePacket=0;
PFN_XTrap_S_RecvGamePacket	XTrap_S_RecvGamePacket=0;

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)

HINSTANCE hXTrap4Server = 0;

unsigned int XTrap_S_LoadDll()
{
	unsigned int ulError = 0;

	char sDllBinFile[]	="XTrap4Server.Dll";

	hXTrap4Server = LoadLibrary(sDllBinFile);

	if (hXTrap4Server == NULL)
	{
		ulError = GetLastError();
		return 0x10000000|ulError;
	}

	XTrap_S_Start			= (PFN_XTrap_S_Start)			GetProcAddress(hXTrap4Server, "XTrap_S_Start");
	XTrap_S_SessionInit		= (PFN_XTrap_S_SessionInit)		GetProcAddress(hXTrap4Server, "XTrap_S_SessionInit");
	XTrap_CS_Step1			= (PFN_XTrap_CS_Step1)			GetProcAddress(hXTrap4Server, "XTrap_CS_Step1");
	XTrap_CS_Step3			= (PFN_XTrap_CS_Step3)			GetProcAddress(hXTrap4Server, "XTrap_CS_Step3");
	XTrap_S_SetActiveCode	= (PFN_XTrap_S_SetActiveCode)	GetProcAddress(hXTrap4Server, "XTrap_S_SetActiveCode");
	XTrap_S_SetOption		= (PFN_XTrap_S_SetOption)		GetProcAddress(hXTrap4Server, "XTrap_S_SetOption");
	XTrap_S_SetAllowDelay	= (PFN_XTrap_S_SetAllowDelay)	GetProcAddress(hXTrap4Server, "XTrap_S_SetAllowDelay");
	XTrap_S_SendGamePacket  = (PFN_XTrap_S_SendGamePacket)	GetProcAddress(hXTrap4Server, "XTrap_S_SendGamePacket");
	XTrap_S_RecvGamePacket  = (PFN_XTrap_S_RecvGamePacket)	GetProcAddress(hXTrap4Server, "XTrap_S_RecvGamePacket");

	if (XTrap_S_Start			== NULL ||
			XTrap_S_SessionInit		== NULL ||
			XTrap_CS_Step1			== NULL ||
			XTrap_CS_Step3			== NULL ||
			XTrap_S_SetOption		== NULL ||
			XTrap_S_SetAllowDelay	== NULL ||
			XTrap_S_SendGamePacket	== NULL	||
			XTrap_S_RecvGamePacket	== NULL)
	{
		return	0x20000000;
	}

	return XTRAP_API_RETURN_OK;
}

unsigned int XTrap_S_FreeDll()
{
	if (hXTrap4Server)
		FreeLibrary((HMODULE)hXTrap4Server);

	return XTRAP_API_RETURN_OK;
}

#else

void *hXTrap4Server	= 0;

unsigned int XTrap_S_LoadDll()
{
	unsigned int ulError = 0;

	char sDllBinFile[]	="/usr/lib/libXTrap4Server.so";

	hXTrap4Server = dlopen(sDllBinFile, RTLD_LAZY);

	if (hXTrap4Server == 0)
	{
		printf("dlopen error %s\n", dlerror());

		return 0x10000000|(unsigned int)ulError;
	}

	XTrap_S_Start		   = (PFN_XTrap_S_Start)			dlsym(hXTrap4Server, "XTrap_S_Start");
	XTrap_S_SessionInit	   = (PFN_XTrap_S_SessionInit)		dlsym(hXTrap4Server, "XTrap_S_SessionInit");
	XTrap_CS_Step1		   = (PFN_XTrap_CS_Step1)			dlsym(hXTrap4Server, "XTrap_CS_Step1");
	XTrap_CS_Step3		   = (PFN_XTrap_CS_Step3)			dlsym(hXTrap4Server, "XTrap_CS_Step3");
	XTrap_S_SetActiveCode  = (PFN_XTrap_S_SetActiveCode)	dlsym(hXTrap4Server, "XTrap_S_SetActiveCode");
	XTrap_S_SetOption	   = (PFN_XTrap_S_SetOption)		dlsym(hXTrap4Server, "XTrap_S_SetOption");
	XTrap_S_SetAllowDelay  = (PFN_XTrap_S_SetAllowDelay)	dlsym(hXTrap4Server, "XTrap_S_SetAllowDelay");
	XTrap_S_SendGamePacket = (PFN_XTrap_S_SendGamePacket)	dlsym(hXTrap4Server, "XTrap_S_SendGamePacket");
	XTrap_S_RecvGamePacket = (PFN_XTrap_S_RecvGamePacket)	dlsym(hXTrap4Server, "XTrap_S_RecvGamePacket");

	if (XTrap_S_Start			== NULL ||
			XTrap_S_SessionInit		== NULL ||
			XTrap_CS_Step1			== NULL ||
			XTrap_CS_Step3			== NULL ||
			XTrap_S_SetOption		== NULL ||
			XTrap_S_SetAllowDelay	== NULL ||
			XTrap_S_SendGamePacket	== NULL	||
			XTrap_S_RecvGamePacket	== NULL)
	{
		return	0x20000000;
	}

	return XTRAP_API_RETURN_OK;
}

unsigned int XTrap_S_FreeDll()
{
	if (hXTrap4Server)
		dlclose(hXTrap4Server);

	return XTRAP_API_RETURN_OK;
}

#endif

unsigned int XTrap_S_LoadCMF(void* pBuf, unsigned int ulSize, unsigned int ulCount, char* pFileName, unsigned int ulMax)
{
	/*
	unsigned int	ulfcnt	= 0;
	unsigned int	ulbcnt	= 0;
	unsigned int	ulRead	= 0;

	FILE	*fi = 0;

	char	sFileName[1024]	= {0,};

	for (ulfcnt = ulMax; ulfcnt >= 0; ulfcnt--)
	{
		sprintf(sFileName, pFileName, ulfcnt);

		if (_access(sFileName, 0) == 0)
		{
			fi = fopen(sFileName, "rb");
			if (fi == NULL) return -1;

			ulRead = fread((unsigned char*)((unsigned char*)pBuf + ulbcnt*ulSize), ulSize, 1, fi);
			if (ulRead != 1) return -1;

			if (fclose(fi))	return -1;

			ulbcnt++;

			if (ulbcnt == ulCount)
				break;
		}
	}
	*/

	return 0;
}

// example)
//
// XTrap_S_LoadCMF(g_sMapBuffer, XTRAP_CS4_BUFSIZE_MAP, 2, "map%d.cs3", 255);
//
// LOAD  map1.cs3 ~ map255.cs3
//

#endif
