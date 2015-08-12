#include "StdH.h"

#include <Engine/Build.h>
#include <Engine/Base/Profiling.h>
#include <Engine/Base/Input.h>
#include <Engine/Base/Protection.h>
#include <Engine/Base/Console.h>
#include <Engine/Base/Console_internal.h>
#include <Engine/Base/Statistics_Internal.h>
#include <Engine/Base/Shell.h>
#include <Engine/Base/CRC.h>
#include <Engine/Base/CRCTable.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Sound/SoundLibrary.h>
#include <Engine/Graphics/GfxLibrary.h>
#include <Engine/Graphics/Font.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/WebAddress.h>
#include <Engine/Network/NetworkProfile.h>
#include <Engine/templates/DynamicContainer.cpp>
#include <Engine/Templates/Stock_CAnimData.h>
#include <Engine/Templates/Stock_CTextureData.h>
#include <Engine/Templates/Stock_CFontData.h>
#include <Engine/Templates/Stock_CSoundData.h>
#include <Engine/Templates/Stock_CModelData.h>
#include <Engine/Templates/Stock_CEntityClass.h>
#include <Engine/Templates/Stock_CMesh.h>
#include <Engine/Templates/Stock_CSkeleton.h>
#include <Engine/Templates/Stock_CAnimSet.h>
#include <Engine/Templates/Stock_CShader.h>
#include <Engine/Templates/Stock_CModelInstance.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Base/IFeel.h>
#include <Engine/Base/StackDump.h>
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#include <Engine/Effect/EffectCommon.h>
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
#include <Engine/Interface/UIManager.h>
//안태훈 수정 시작	//(5th Closed beta)(0.2)

#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/GameStageManager/StageMgr.h>
#include <Engine/Contents/Base/UIMsgBoxMgr.h>

#include <Engine/GameState.h>
#include <Engine/MiniDump.h>
#include <Engine/Network/Web.h>
#include <Engine/JobInfo.h>
#include <Engine/Entities/SkillTree.h>
#include <Engine/LoginJobInfo.h>
#include <Engine/Entities/ArmorPreview.h>
#include <Engine/LocalDefine.h>
#include <Engine/Object/ActorMgr.h>
#include <Engine/Loading.h>
#include <Engine/Entities/TEventString.h>
#include <Engine/Entities/TradeItem.h>
#include <Engine/Info/MyInfo.h>
#include <Engine/Entities/LevelupGuide.h>
#include <Engine/Contents/function/ItemCollectionData.h>
#include <Engine/Info/ServerInfo.h>

#ifndef NO_GAMEGUARD
//	#include <NPGameLib.h>
	#include <Engine\GameState.h>
#endif

#include <Engine/Help/LoadString.h>
#include <Engine/Help/Util_Help.h>

#ifdef XTRAP_SECURE_CKBANG_2010_07_20
#include <Engine/XTrapInterface/XTrapInterface.h>
XTrap_CS_Step2_Interface g_pXTrap_CS_Step2Func = NULL;
XTrap_C_SetUserInfoEx_Interface g_pXTrap_C_SetUserInfoEx_Interface = NULL;
pHexaDump4XTrap_V1 g_pHexaDump4XTrap_V1 = NULL;

#endif

#ifndef NO_GAMEGUARD
#include <Engine/GameGuardInterface.h>
GAME_GUARD_RECV_AUTH g_pGameGuardRecvAuth = NULL;
GAME_GUARD_SEND_USERID g_pGameGuardSendUserID = NULL;
#endif 

ENGINE_API cWeb g_web;

//#define USE_MINIDUMP
#ifdef USE_MINIDUMP
MiniDumper	g_MiniDumper(TRUE);
#endif
//안태훈 수정 끝	//(5th Closed beta)(0.2)
//안태훈 수정 시작	//(5th Closed beta)(0.2)
#ifdef KALYDO
CTFileName fnmPersistentSymbols = CTString("ps.dat");
#else
CTFileName fnmPersistentSymbols = CTString("Data\\etc\\ps.dat");
#endif
//안태훈 수정 끝	//(5th Closed beta)(0.2)

// this version string can be referenced from outside the engine
ENGINE_API CTString _strEngineBuild  = "";
ENGINE_API ULONG _ulEngineBuildMajor = _SE_BUILD_MAJOR;
ENGINE_API ULONG _ulEngineBuildMinor = _SE_BUILD_MINOR;

// <-- ErrorLog.txt에 디스플레이 정보를 기록하기 위한 부분
ENGINE_API CTString _strDisplayDriver = "";
ENGINE_API CTString _strDisplayDriverVersion = "";
ENGINE_API CTString _strSoundDriver = "";
ENGINE_API CTString _strTotalMemory = "";
// -->

ENGINE_API BOOL _bDedicatedServer = FALSE;
#if defined WORLD_EDITOR
ENGINE_API BOOL _bWorldEditorApp  = TRUE;
#else
ENGINE_API BOOL _bWorldEditorApp  = FALSE;
#endif

//	김영환
ENGINE_API BOOL _bClientApp = FALSE;			// TRUE 설정되면 게임 클라이언트 이다.

ENGINE_API BOOL _bUseBloomInWorldEditor = TRUE;
ENGINE_API BOOL _bTranslucentModel = FALSE;
ENGINE_API BOOL _bInvisibleOff =FALSE;
ENGINE_API BOOL _bShowPortalPolygon =FALSE;
ENGINE_API BOOL _bAttributemap_DepthTest =FALSE;

ENGINE_API BOOL _bLoginProcess	= TRUE;
//ENGINE_API BOOL _bUseSocket		  = FALSE;
ENGINE_API BOOL _bSkaStudioApp = FALSE;
ENGINE_API BOOL _bInTestGame  = FALSE;					// yjpark
ENGINE_API BOOL _bShowPolygonAttribute = FALSE;			// yjpark
ENGINE_API CTString _strLogFile = "";
//안태훈 수정 시작
ENGINE_API BOOL g_bBadWeather = FALSE;
ENGINE_API COLOR g_colWeather = C_WHITE|CT_OPAQUE;
ENGINE_API INDEX g_bRenderDecoration = TRUE;
ENGINE_API INDEX g_iUseBloom = 1;	//0 : none, 1 : normal bloom, 2 : strong bloom
FLOAT g_fFarClipPlus = 0.0f;
ENGINE_API INDEX g_bTestClient = FALSE;
extern UINT g_uiEngineVersion;
ENGINE_API char *g_szExitError = NULL;
//안태훈 수정 끝

ENGINE_API int g_iLocalVersion = 700;

//강동민 수정 시작 클로즈 2차 작업	08.18
ENGINE_API extern INDEX g_iShadowDetail			= 4;
ENGINE_API extern INDEX g_iReflectionDetail		= 0;
//강동민 수정 끝 클로즈 2차 작업		08.18

ENGINE_API INDEX idPlayerWhole_Animation[ANIM_TOTAL];


//안태훈 수정 시작	//(5th Closed beta)(0.1)
ENGINE_API FLOAT g_fFramePerSecond = FLT_MAX;
ENGINE_API BOOL g_bNoPlaySnd = FALSE;
//안태훈 수정 끝	//(5th Closed beta)(0.1)
//안태훈 수정 시작	//(Taiwan Closed beta)(0.2)
ENGINE_API INDEX g_iCountry = 0;	//0 : Korea
INDEX g_iEnterChat = 1;
INDEX g_iShowName = 0;
INDEX g_iShowNameItem = 1;
// wooss 070401 ----------->><<
// kw : WSS_HELP_SYSTEM_1
INDEX g_iShowHelp1Icon = 1;

// [090715: selo] - 아이디 저장 여부 
INDEX g_iSaveID = 0;
extern CTString g_strSaveID = "";

#ifdef EUROUPEAN_SERVER_LOGIN
// 유로피안 서버 접속 여부 [10/18/2012 Ranma]
INDEX g_iConnectEuroupean = 0;
#endif

// [7/9/2009 rumist] rejection.
INDEX g_iRejectExchange = 0;
INDEX g_iRejectParty = 0; 

// Map Info. [9/15/2009 rumist]
INDEX g_iZoomInMap = 12;
INDEX g_iOpacityInMap = 75;

// 1106 UI 개편 [09/05/11 trylord]
INDEX g_iQuestBookOpen = 1;

// UI 개편. [9/16/2009 rumist]
INDEX g_iXPosInMap = 100;
INDEX g_iYPosInMap = 100;
INDEX g_iXPosInInventory=100;
INDEX g_iYPosInInventory=100;
INDEX g_iXPosInInvenSlot1=0;
INDEX g_iYPosInInvenSlot1=0;
INDEX g_iXPosInInvenSlot2=0;
INDEX g_iYPosInInvenSlot2=0;
INDEX g_iXPosInInvenSlot3=0;
INDEX g_iYPosInInvenSlot3=0;
// UI 퀵슬롯 확장 개편 작업 확장 슬롯 위치 [12/18/2012 Ranma]
INDEX g_iXPosQuickSlotEX1=0;
INDEX g_iYPosQuickSlotEX1=0;
INDEX g_iXPosQuickSlotEX2=0;
INDEX g_iYPosQuickSlotEX2=0;
INDEX g_bQuickSlotEX1=0;
INDEX g_bQuickSlotEX2=0;
INDEX g_bQuickSlot1HorOrVer=1;
INDEX g_bQuickSlot1Lock=0;
INDEX g_bQuickSlot2HorOrVer=1;
INDEX g_bQuickSlot2Lock=0;

INDEX g_iXPosInCharInfo=100;
INDEX g_iYPosInCharInfo=100;
INDEX g_iXPosInQuest=100;
INDEX g_iYPosInQuest=100;
INDEX g_iXPosInPetInfo=100;
INDEX g_iYPosInPetInfo=100;
INDEX g_iXPosInWPetInfo=100;
INDEX g_iYPosInWPetInfo=100;
INDEX g_iXPosInMessanger=100;
INDEX g_iYPosInMessanger=100;
INDEX g_iXPosInSystemMenu = 600;
INDEX g_iYPosInSystemMenu = 500;
INDEX g_iXPosInNPCScroll = 20;
INDEX g_iYPosInNPCScroll = 100;



// UI_REFORM :Su-won
ENGINE_API INDEX g_bSlaveNoTarget =FALSE;

ENGINE_API BOOL g_bIsMalEng = FALSE;	//Add for malaysia 060330

ENGINE_API INDEX g_iAutoAttack = 1;
//안태훈 수정 끝	//(Taiwan Closed beta)(0.2)

// global handle for application window
extern ENGINE_API HWND  _hwndMain = NULL;
extern ENGINE_API HWND	_hDlgWeb = NULL;
extern BOOL  _bFullScreen = FALSE;
extern INDEX _iGfxAPI = -1; // -1=NONE, 0=OGL, 1=D3D

// WSS_NPROTECT 070402 ------------------------------->>
#ifndef NO_GAMEGUARD
//	ENGINE_API CNPGameLib npgl("LastChaosHK");
#endif
// ---------------------------------------------------<<

// 태국버전 패치 flag 051109
INDEX g_iTempFlag;

// critical section for access to zlib functions
CTCriticalSection zip_csLock; 


// OS info
static CTString sys_strOS = "";
static INDEX sys_iOSMajor = 0;
static INDEX sys_iOSMinor = 0;
static INDEX sys_iOSBuild = 0;
static CTString sys_strOSMisc = "";
extern BOOL _bNTKernel = FALSE; // runing under NT kernel

// CPU info
static CTString sys_strCPUVendor = "";
static INDEX sys_iCPUType = 0;
static INDEX sys_iCPUFamily = 0;
static INDEX sys_iCPUModel = 0;
static INDEX sys_iCPUStepping = 0;
static BOOL  sys_bCPUHasMMX = 0;
static BOOL  sys_bCPUHasSSE = 0;
static BOOL  sys_bCPUHasCMOV = 0;
static INDEX sys_iCPUMHz = 0;
			 INDEX sys_iCPUMisc = 0;

// RAM info
static INDEX sys_iRAMPhys = 0;
static INDEX sys_iRAMSwap = 0;

// HDD info
static INDEX sys_iHDDSize = 0;
static INDEX sys_iHDDFree = 0;
static INDEX sys_iHDDMisc = 0;

// MOD info
static CTString sys_strModName = "";
static CTString sys_strModExt  = "";

extern INDEX gam_iMaxSaveGameSize = 1024*1024*4; // 4MB

// debugging 
extern ENGINE_API INDEX dbg_bAtHome = FALSE;
// profiling
extern ENGINE_API INDEX _iStatsMode = 0;

// global socket
extern ENGINE_API SOCKET g_hSocket = INVALID_SOCKET;

// enables paranoia checks for allocation array
extern BOOL _bAllocationArrayParanoiaCheck = FALSE;


// internal tables to speed the stuff up

// fast sin/cos table (input in "degrees" mapped to -255 - +255 range)
#define ANGTABSIZE 256
static FLOAT afSinTable[  ANGTABSIZE+ANGTABSIZE+ANGTABSIZE/4];
static FLOAT af1oSinTable[ANGTABSIZE+ANGTABSIZE+ANGTABSIZE/4];
extern ENGINE_API const FLOAT *_pfSinTable   = afSinTable   +ANGTABSIZE;
extern ENGINE_API const FLOAT *_pfCosTable   = afSinTable   +ANGTABSIZE+ANGTABSIZE/4;
extern ENGINE_API const FLOAT *_pf1oSinTable = af1oSinTable +ANGTABSIZE;
extern ENGINE_API const FLOAT *_pf1oCosTable = af1oSinTable +ANGTABSIZE+ANGTABSIZE/4;

// fast arcsin/arccos tables (input from -255 to +255; returns degrees mapped to -255 - +255 range)
static INDEX aiASinTable[ANGTABSIZE+ANGTABSIZE];
static INDEX aiACosTable[ANGTABSIZE+ANGTABSIZE];
extern ENGINE_API const INDEX *_piASinTable = aiASinTable+ANGTABSIZE;
extern ENGINE_API const INDEX *_piACosTable = aiACosTable+ANGTABSIZE;

// table for clipping [-512..+1024] to [0..255]
static UBYTE aubClipByte[256*2+ 256 +256*3];
extern ENGINE_API const UBYTE *_pubClipByte = &aubClipByte[256*2];

// fast square root and 1/sqrt tables
extern UBYTE _aubSqrt[SQRTTABLESIZE]   = {0};
extern UWORD _auw1oSqrt[SQRTTABLESIZE] = {0};

//안태훈 수정 시작	//(Option)(0.1)
extern ENGINE_API FLOAT ter_fLODMul = 1.0f;
extern ENGINE_API FLOAT g_fChaLODMul = 1.0f;
//안태훈 수정 끝	//(Option)(0.1)

// main window canvas											// yjpark |<--
extern ENGINE_API BOOL		_bWindowChanging = FALSE;
extern ENGINE_API CDrawPort	*_pdpMain = NULL;
extern ENGINE_API CDrawPort	*_pdpNormalMain = NULL;
extern ENGINE_API CDrawPort	*_pdpWideScreenMain = NULL;
extern ENGINE_API CViewPort	*_pvpViewPortMain = NULL;
extern ENGINE_API HINSTANCE	_hInstanceMain = NULL;
extern ENGINE_API INDEX		sam_bFullScreenActive = TRUE;
extern ENGINE_API INDEX		sam_iScreenSizeI = 1024;
extern ENGINE_API INDEX		sam_iScreenSizeJ = 768;
extern ENGINE_API INDEX		sam_iDisplayDepth  = 0;
extern ENGINE_API INDEX		sam_iDisplayAdapter = 0;
extern ENGINE_API INDEX		sam_iGfxAPI = 1;	// 0==OpenGL
extern ENGINE_API INDEX		sam_bWideScreen = FALSE;
extern ENGINE_API INDEX		cmd_iWindowLeft = -1;
extern ENGINE_API INDEX		cmd_iWindowTop = -1;	 			// yjpark     -->|
/*
typedef struct _SERVICE_TABLE_ENTRY
{
	unsigned int *ServiceTableBase; // SSDT(KiServiceTable)의 주소, 각 서비스 별 매핑 함수 포인터들의 목록이 저장된 테이블의 포인터
	unsigned int *ServiceCounterTableBase; // 무시
	unsigned int NumberOfServices; // Native API의 총 개수, (Service = Native API)
	unsigned char *ParamTableBase; // KiArgumentTable의 주소 값, 각 서비스 별 파라미터의 크기를 나타내는 값을 가진 테이블의 포인터
} SERVICE_DESCRIPTOR_ENTRY, *PSERVICE_DESCRIPTOR_ENTRY;
// direct import KeServiceDescriptorTable
__declspec(dllimport)  SERVICE_DESCRIPTOR_ENTRY KeServiceDescriptorTable;

// native api function address get macro
#define SYSCALL_INDEX(_Function) *(PULONG)((PUCHAR)_Function+1)
#define ORG_SYSCALL_PTR(_orgFunc) \
	&(((PLONG)KeServiceDescriptorTable.ServiceTableBase)[SYSCALL_INDEX(_orgFunc)])

// 커널모드의 메모리 쓰기 권한을 위해 Write Protection 해제
// -- SSDT[System Service Discription Table] = (KiServiceTable) 변경을 위해 커널 메모리 쓰기가 가능해야 함
// -- CR0(Control Register 0)의 WP(Write Protection)비트를 0으로 만든다.
// -- 다른 방법 : MDL (Memory Descriptor List)를 이용

#define CR0_WP_MASK		0x0FFFEFFFF

VOID ClearWriteProtect(VOID)
{
	__asm
	{
		push eax;
		mov eax, cr0;
		and eax, CR0_WP_MASK; // WP 클리어
		mov cr0, eax;
		pop eax;
	}
}

VOID SetWriteProtect(VOID)
{
	__asm
	{
		push eax;
		mov eax, cr0;
		or eax, not CR0_WP_MASK; // WP 비트 세팅
		mov cr0, eax;
		pop eax;
	}
}

// zwopenprocess export
NTKERNELAPI NTSTATUS ZwOpenProcess(
								   PHANDLE ProcessHandle,
								   ACCESS_MASK DesiredAccess,
								   POBJECT_ATTRIBUTES ObjectAttributes,
								   PCLIENT_ID ClientId );
typedef NTSTATUS (*pfnZwOpenProcess) (
									  OUT PHANDLE			ProcessHandle,
									  IN ACCESS_MASK		AccessMark,
									  IN POBJECT_ATTRIBUTES	ObjectAttributes,
									  IN PCLIENT_ID			ClientId
									  );

pfnZwOpenProcess OldZwOpenProcess = NULL;

NTSTATUS NewZwOpenProcess(
						  OUT PHANDLE ProcessHandle,
						  IN ACCESS_MASK DesiredAccess,
						  IN POBJECT_ATTRIBUTES ObjectAttributes,
						  IN PCLIENT_ID ClientId OPTIONAL)
{
	NTSTATUS status;

	status = OldZwOpenProcess(ProcessHandle, DesiredAccess, ObjectAttributes, ClientId);
	return status;
}
*/
// WSS_NPROTECT 070402 ------------------------------->>
#ifndef NO_GAMEGUARD
ENGINE_API CTString g_szHackMsg;
// ---------------------------------------------------<<
// 메시지 처리 CallBack 함수
// 게임 종료시에는 false를 return해 주고 종료하지 않은 경우는 true를 return합니다.
/*
int CALLBACK NPGameMonCallback(DWORD dwMsg,DWORD dwArg)
{	
	// Debug...
//	CPrintF("NPGAMEMON : %lu, %lu", dwMsg,dwArg);

	BOOL tAppExit =FALSE;
	
	tAppExit = _pNetwork->NPGameMonCallbackErrChk( dwMsg, dwArg);
	
	if(tAppExit)
	{
		// 게임 종료
		_pGameState->Running()		= FALSE;
		_pGameState->QuitScreen()	= FALSE;
		return FALSE;
	}
	return TRUE;
}
/**/
#endif
// ---------------------------------------------------<<


// root of evil :)
// 중복 클라이언트 방지용 shared 메모리 사용. [2/4/2010 rumist]
// 원래는 크리티컬 섹션을 써도 된다. 어느게 효율이 좋을지는.. 고민.
// 뮤텍스 계열의 단점은 실행시 호환성이 변경되면 뮤텍스 역시 다른 코드영역에서
// 실행된다. 또한 마찬가지로 강제 종료시 정상 처리가 되지 않으면 클라이언트를
// 다시 실행할 수 없다.
//- detect multi client.
#pragma data_seg( ".SHAREDATA" )
int g_nEngineDllRefCnt = 0;
#pragma data_seg()
#pragma comment( linker, "/SECTION:.SHAREDATA,RWS" )

void	IncRef()	{ ++g_nEngineDllRefCnt;	}
void	DecRef()	{ g_nEngineDllRefCnt > 0 ? --g_nEngineDllRefCnt : g_nEngineDllRefCnt=0; }
ENGINE_API const int	SE_GetEngineDllRefCnt() { return g_nEngineDllRefCnt;	}

#ifdef KALYDO
ENGINE_API HWND	g_KalydoWindowHandle = NULL;
ENGINE_API void SE_UpdateStreamingData()
{
	if( g_deqLoadData.empty() )
		return;
	
	DWORD start = GetTickCount();
	while(!g_deqLoadData.empty())
	{
		SLS* pSLS = reinterpret_cast<SLS*>(g_deqLoadData.front());
		// if still alive, step in.
		if( pSLS->pTarget->IsUsed() )
		{
			pSLS->pTarget->Clear();
			pSLS->pTarget->Load_Delay_t( pSLS->pTargetFilePath );
			//pSLS->pTarget->Reload();
		}

		delete pSLS;

		g_deqLoadData.pop_front();

		if (GetTickCount() - start > 10)
			// take 10ms max to prevent frame drops
			break;
	}
}
#endif

//- end.
BOOL APIENTRY DllMain( HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			IncRef();
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			{
				std::vector<TLVarBase*>::iterator i = TLVarBase::m_Vars.begin();
				for (; i != TLVarBase::m_Vars.end(); ++i)
				{
					TLVarBase* var = *i;
					var->cleanUp();
				}
			}
			
			break;
		case DLL_PROCESS_DETACH:
			DecRef();
			break;
		default:
			ASSERT(FALSE);
	}
	return TRUE;
}


static void DetectCPU(void)
{
	char strVendor[12+1];
	strVendor[12] = 0;
	ULONG ulTFMS;
	ULONG ulFeatures;

	// test MMX presence and update flag
	__asm {
		mov     eax,0 // request for basic id
		cpuid
		mov     dword ptr [strVendor+0],ebx
		mov     dword ptr [strVendor+4],edx
		mov     dword ptr [strVendor+8],ecx
		mov     eax,1 // request for TFMS feature flags
		cpuid
		mov     dword ptr [ulTFMS],eax   // remember type, family, model and stepping
		mov     dword ptr [ulFeatures],edx
	}

	const INDEX iType     = (ulTFMS>>12)&0x3;
	const INDEX iFamily   = (ulTFMS>> 8)&0xF;
	const INDEX iModel    = (ulTFMS>> 4)&0xF;
	const INDEX iStepping = (ulTFMS>> 0)&0xF;

	CPrintF(TRANS("Vendor: %s\n"), strVendor);
	CPrintF(TRANS("  Type: %d, Family: %d, Model: %d, Stepping: %d\n"),
								iType, iFamily, iModel, iStepping);

	const BOOL bMMX  = ulFeatures & (1<<23);
	const BOOL bCMOV = ulFeatures & (1<<15);
	const BOOL bSSE  = ulFeatures & (1<<25);
	const FLOAT fMHz = _pTimer->tm_llCPUSpeedHZ/1E6;

	CTString strYes = TRANS("Yes");
	CTString strNo  = TRANS("No");

	CPrintF(TRANS("   MMX: %s\n"), bMMX  ? strYes : strNo);
	CPrintF(TRANS("   SSE: %s\n"), bSSE  ? strYes : strNo);
	CPrintF(TRANS("  CMOV: %s\n"), bCMOV ? strYes : strNo);
	CPrintF(TRANS(" Clock: %.0f MHz\n"), fMHz);

	sys_strCPUVendor = strVendor;
	sys_iCPUType     = iType;
	sys_iCPUFamily   = iFamily;
	sys_iCPUModel    = iModel;
	sys_iCPUStepping = iStepping;
	sys_bCPUHasMMX  = bMMX !=0;
	sys_bCPUHasSSE  = bSSE !=0;
	sys_bCPUHasCMOV = bCMOV!=0;
	sys_iCPUMHz = (INDEX)fMHz;

	if( !bMMX) FatalError( TRANS("MMX support required but not present!"));
}

static void DetectCPUWrapper(void)
{
	__try {
		DetectCPU();
	} __except(EXCEPTION_EXECUTE_HANDLER) {
		CPrintF( TRANS("Cannot detect CPU: exception raised.\n"));
	}
}



static void PrepareTables(void)
{
	INDEX i;
	// prepare array for fast clamping to 0..255
	for( i=-256*2; i<256*4; i++) aubClipByte[i+256*2] = (UBYTE)Clamp( i, 0L, 255L);
	// prepare fast sqrt tables
	for( i=0; i<SQRTTABLESIZE; i++) _aubSqrt[i]   = (UBYTE)(sqrt((float)i*65536/SQRTTABLESIZE));
	for( i=1; i<SQRTTABLESIZE; i++) _auw1oSqrt[i] = (UWORD)(sqrt((FLOAT)(SQRTTABLESIZE-1)/i)*255.0f);
	_auw1oSqrt[0] = MAX_UWORD;
	// prepare fast sin/cos table
	for( i=-ANGTABSIZE; i<ANGTABSIZE+ANGTABSIZE/4; i++) afSinTable[i+ANGTABSIZE] = -Sin((i-ANGTABSIZE/2)*360.0f/ANGTABSIZE);
	for( i=0; i<ANGTABSIZE+ANGTABSIZE+ANGTABSIZE/4; i++) {
		if( afSinTable[i]==0) af1oSinTable[i] = 16384.0f;
		else af1oSinTable[i] = 1.0f / afSinTable[i];
	}
	// prepare fast arcsin/arccos tables
	for( i=-ANGTABSIZE; i<ANGTABSIZE; i++) aiASinTable[i+ANGTABSIZE] = asin((FLOAT)i/ANGTABSIZE) *ANGTABSIZE*ANGLE_180/PI/360.0f;
	for( i=-ANGTABSIZE; i<ANGTABSIZE; i++) aiACosTable[i+ANGTABSIZE] = acos((FLOAT)i/ANGTABSIZE) *ANGTABSIZE*ANGLE_180/PI/360.0f;
}

char strExePath[MAX_PATH] = "";
char strDirPath[MAX_PATH] = "";

static void AnalyzeApplicationPath(void)
{
	strcpy(strDirPath, "T:\\");
	strcpy(strExePath, "T:\\SeriousSam.xbe");
	char strTmpPath[MAX_PATH] = "";
	// get full path to the exe
	GetModuleFileName( NULL, strExePath, sizeof(strExePath)-1);
	// copy that to the path
	strncpy(strTmpPath, strExePath, sizeof(strTmpPath)-1);
	strDirPath[sizeof(strTmpPath)-1] = 0;

	std::string strPath = strTmpPath;
	int nBackSlash = strPath.rfind('\\');
#if		_DEBUG || UI_TOOL || WORLD_EDITOR
	// Debug 제거 - Debug 파일은 한폴더 안쪽에 존재 : 한겹 더 벗긴다.
	nBackSlash = strPath.rfind('\\', --nBackSlash);	
#endif	// _DEBUG
	// Bin 제거
	nBackSlash = strPath.rfind('\\', --nBackSlash);
	strPath = strPath.substr(0, nBackSlash+1);

	char* pstrFin = (char*)strPath.c_str();

	strncpy( strDirPath, pstrFin, sizeof(strDirPath)-1);
	strDirPath[sizeof(strDirPath)-1] = 0;
}

// [2013/01/16] sykim70
inline UINT GetFileLastWriteTime(HANDLE hFile)
{
	FILETIME fileTime;
	GetFileTime(hFile, NULL, NULL, &fileTime);

	SYSTEMTIME sysTime;
	FileTimeToSystemTime(&fileTime, &sysTime);
	
	tm stTm = {
		(int)sysTime.wSecond,
		(int)sysTime.wMinute,
		(int)sysTime.wHour,
		(int)sysTime.wDay,
		(int)sysTime.wMonth - 1,
		(int)sysTime.wYear - 1900,
		0, 0, 0
	};
	return (UINT)mktime(&stTm);
}

// [2013/01/16] sykim70
inline bool GetFileInfo(const char* filename, UINT& len, UINT& time)
{
	DWORD dwFlags = FILE_ATTRIBUTE_NORMAL|FILE_FLAG_NO_BUFFERING;
	HANDLE hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, dwFlags, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;
	len  = GetFileSize(hFile, NULL);
	time = GetFileLastWriteTime(hFile);
	CloseHandle(hFile);
	return true;
}

// [2013/01/16] sykim70
inline CTString OpenPersistentSymbolFile2(const CTFileName &fnmPS)
{
	FILE* fp = fopen(fnmPS.str_String, "rb");
	if (fp == INVALID_HANDLE_VALUE)
		return "";
	int len = -1;
	fread(&len, sizeof(len), 1, fp);
	UBYTE *bufferSrc = new UBYTE[len];
	char *bufferDecoded = new char[len + 1];
	memset(bufferDecoded, 0, len+1);
	fread(bufferSrc, len, 1, fp);
	
	extern void DecodePersistentSymbols(unsigned char *buf, int dummyLen, const unsigned char *bufSrc, int len);
	DecodePersistentSymbols((UBYTE*)bufferDecoded, 0, bufferSrc, len);
	
	CTString strContents = bufferDecoded;
	delete[] bufferSrc;
	delete[] bufferDecoded;
	fclose(fp);
	return strContents;
}

// [2013/01/16] sykim70
inline bool GetOneLine(CTString& str, CTString& line)
{
	INDEX iLen = str.Length();
	if (iLen <= 0)
		return false;

	INDEX iPos = 0;
	while (iPos < iLen)
	{
		if (str[iPos] != '\n' && str[iPos] != '\r')
		{
			if (iPos > 0)
			{
				str.TrimLeft(iLen - iPos);
				iLen = str.Length();
				iPos = 0;
			}
			break;
		}
		iPos++;
	}
	while (iPos < iLen)
	{
		if (str[iPos] == '\n' || str[iPos] == '\r')
			break;
		iPos++;
	}
	if (iPos == iLen)
	{
		line = str;
		str = "";
		return true;
	}
	CTString temp;
	str.Split(iPos, line, temp);
	str = temp;
	return true;
}

// [2013/01/16] sykim70
ENGINE_API bool SE_CheckEngine()
{
#if !defined(_DEBUG) && !defined(KALYDO) && !defined(G_KOR) && !defined(VER_TEST) && !defined(G_CHINA)
	AnalyzeApplicationPath();

	// check binary.lod
	CTString strFullPath = strDirPath;
	CTString fnTemp = strFullPath + "data\\etc\\binary.lod";
	CTString data = OpenPersistentSymbolFile2(fnTemp);

	CTString oneline;
	if (!GetOneLine(data, oneline))
		return false;
	int nCount = atoi(oneline.str_String);

	if (!GetOneLine(data, oneline))	// stamp
		return false;

	UINT srcLen, targetLen, targetTime;
	UINT minTime = -1;
	UINT maxTime = 0;
	for (int i = 0; i < nCount; i++)
	{
		if (!GetOneLine(data, oneline))
			return false;
		fnTemp = strFullPath + oneline;

		if (!GetOneLine(data, oneline))
			return false;
		srcLen = (UINT)atoi(oneline.str_String);

		if (!GetFileInfo(fnTemp.str_String, targetLen, targetTime))
			return false;

		if (srcLen != targetLen)
			return false;

		minTime = min(minTime, targetTime);
		maxTime = max(maxTime, targetTime);
	}
	if (maxTime-minTime > 60*3)	// 3 min
		return false;

#endif
	return true;
}

// startup engine 
//ENGINE_API void SE_InitEngine(CTString strGameID, BOOL bTcp)		// by seo 40225
ENGINE_API void SE_InitEngine(CTString strGameID)
{
#if MEMORY_TRACKING
	TrackMemoryAlloc((void*)1, 1, 1);
	TrackMemoryFree((void*)1);
#endif //MEMORY_TRACKING

	AnalyzeApplicationPath();
	_fnmApplicationPath = CTString(strDirPath);
	_fnmApplicationExe = CTString(strExePath);
	try {
		_fnmApplicationExe.RemoveApplicationPath_t();
	} catch (char *strError) {
		(void) strError;
		ASSERT(FALSE);
	}

	// create some internal tables
	PrepareTables();

#if		defined(VER_TEST)
	_pConsole = new CConsole;
#else
	_pConsole = NULL;
#endif

	if (_strLogFile=="") {
		_strLogFile = CTFileName(CTString(strExePath)).FileName();
	}
	CTFileName fnmLog = _fnmApplicationPath+_strLogFile+".log";
	
	if (_pConsole != NULL)
	{
		_pConsole->Initialize(fnmLog, 80, 512);
	}
	

	CTFileName fnmExt	= CTFileName("ErrorLog.txt");
	CTFileName fnmError = _fnmApplicationPath+fnmExt;
	// Init exception handler
	InitExceptionHandler(fnmError.str_String);

	_pAnimStock        = new CStock_CAnimData;
	_pTextureStock     = new CStock_CTextureData;
	_pFontStock        = new CStock_CFontData;
	_pSoundStock       = new CStock_CSoundData;	
	_pModelStock       = new CStock_CModelData;
	_pEntityClassStock = new CStock_CEntityClass;
	_pMeshStock        = new CStock_CMesh;
	_pSkeletonStock    = new CStock_CSkeleton;
	_pAnimSetStock     = new CStock_CAnimSet;
	_pShaderStock      = new CStock_CShader;
	_pModelInstanceStock = new CStock_CModelInstance;

#ifdef KALYDO
	g_deqLoadData.clear();
#endif

	_pTimer = new CTimer;
	_pGfx   = new CGfxLibrary;
	_pSound = new CSoundLibrary;	
	_pInput = new CInput;	

	_pGameState = new CGameState;

	// init main shell
	_pShell = new CShell;
	_pShell->Initialize();

	CRCT_Init();

	_strEngineBuild.PrintF( TRANS("SeriousEngine Build: %d.%d"), _SE_BUILD_MAJOR, _SE_BUILD_MINOR);

	// print basic engine info
	CPrintF(TRANS("--- Serious Engine Startup ---\n"));
	CPrintF("  %s\n\n", _strEngineBuild);

	// print info on the started application
	CPrintF(TRANS("Executable: %s\n"), strExePath);
	CPrintF(TRANS("Assumed engine directory: %s\n"), _fnmApplicationPath);

	CPrintF("\n");

	// report os info
	CPrintF(TRANS("Examining underlying OS...\n"));
	OSVERSIONINFO osv;
	memset(&osv, 0, sizeof(osv));
	osv.dwOSVersionInfoSize = sizeof(osv);
	if (GetVersionEx(&osv)) {
		switch (osv.dwPlatformId) {
		case VER_PLATFORM_WIN32s:         sys_strOS = "Win32s";  break;
		case VER_PLATFORM_WIN32_WINDOWS:  sys_strOS = "Win9x"; break;
		case VER_PLATFORM_WIN32_NT:       sys_strOS = "WinNT"; break;
		default: sys_strOS = "Unknown\n"; break;
		}

		// signal kernel type to the engine
		_bNTKernel = (sys_strOS == "WinNT");

		sys_iOSMajor = osv.dwMajorVersion;
		sys_iOSMinor = osv.dwMinorVersion;
		sys_iOSBuild = osv.dwBuildNumber & 0xFFFF;
		sys_strOSMisc = osv.szCSDVersion;

		CPrintF(TRANS("  Type: %s\n"), (const char*)sys_strOS);
		CPrintF(TRANS("  Version: %d.%d, build %d\n"), 
			osv.dwMajorVersion, osv.dwMinorVersion, osv.dwBuildNumber & 0xFFFF);
		CPrintF(TRANS("  Misc: %s\n"), osv.szCSDVersion);
	} else {
		CPrintF(TRANS("Error getting OS info: %s\n"), GetWindowsError(GetLastError()) );
	}
	CPrintF("\n");

	// report CPU
	CPrintF(TRANS("Detecting CPU...\n"));
	DetectCPUWrapper();
	CPrintF("\n");

	// report memory info
	extern void ReportGlobalMemoryStatus(CTFileStream *pstrm);
	ReportGlobalMemoryStatus(NULL);

	MEMORYSTATUS ms;
	GlobalMemoryStatus(&ms);

	#define MB (1024*1024)
	sys_iRAMPhys = ms.dwTotalPhys    /MB;
	sys_iRAMSwap = ms.dwTotalPageFile/MB;

	// initialize zip semaphore
	zip_csLock.cs_iIndex = -1;  // not checked for locking order


	// get info on the first disk in system
	DWORD dwSerial;
	DWORD dwFreeClusters;
	DWORD dwClusters;
	DWORD dwSectors;
	DWORD dwBytes;

	char strDrive[] = "C:\\";
	strDrive[0] = strExePath[0];

	GetVolumeInformation(strDrive, NULL, 0, &dwSerial, NULL, NULL, NULL, 0);
	GetDiskFreeSpace(strDrive, &dwSectors, &dwBytes, &dwFreeClusters, &dwClusters);
	sys_iHDDSize = __int64(dwSectors)*dwBytes*dwClusters/MB;
	sys_iHDDFree = __int64(dwSectors)*dwBytes*dwFreeClusters/MB;
	sys_iHDDMisc = dwSerial;
 
	// add console variables
	extern INDEX con_bNoWarnings;
	extern INDEX wld_bFastObjectOptimization;
	extern INDEX fil_bPreferZips;
	extern INDEX fil_iReportStats;
	extern FLOAT mth_fCSGEpsilon;
	_pShell->DeclareSymbol("user INDEX con_bNoWarnings;", &con_bNoWarnings);
	_pShell->DeclareSymbol("user INDEX wld_bFastObjectOptimization;", &wld_bFastObjectOptimization);
	_pShell->DeclareSymbol("user FLOAT mth_fCSGEpsilon;", &mth_fCSGEpsilon);
	_pShell->DeclareSymbol("persistent user INDEX fil_bPreferZips;", &fil_bPreferZips);
	_pShell->DeclareSymbol("persistent user INDEX fil_iReportStats;", &fil_iReportStats);
	// OS info
	_pShell->DeclareSymbol("user const CTString sys_strOS    ;", &sys_strOS);
	_pShell->DeclareSymbol("user const INDEX sys_iOSMajor    ;", &sys_iOSMajor);
	_pShell->DeclareSymbol("user const INDEX sys_iOSMinor    ;", &sys_iOSMinor);
	_pShell->DeclareSymbol("user const INDEX sys_iOSBuild    ;", &sys_iOSBuild);
	_pShell->DeclareSymbol("user const CTString sys_strOSMisc;", &sys_strOSMisc);
	// CPU info
	_pShell->DeclareSymbol("user const CTString sys_strCPUVendor;", &sys_strCPUVendor);
	_pShell->DeclareSymbol("user const INDEX sys_iCPUType       ;", &sys_iCPUType    );
	_pShell->DeclareSymbol("user const INDEX sys_iCPUFamily     ;", &sys_iCPUFamily  );
	_pShell->DeclareSymbol("user const INDEX sys_iCPUModel      ;", &sys_iCPUModel   );
	_pShell->DeclareSymbol("user const INDEX sys_iCPUStepping   ;", &sys_iCPUStepping);
	_pShell->DeclareSymbol("user const INDEX sys_bCPUHasMMX     ;", &sys_bCPUHasMMX  );
	_pShell->DeclareSymbol("user const INDEX sys_bCPUHasSSE     ;", &sys_bCPUHasSSE  );
	_pShell->DeclareSymbol("user const INDEX sys_bCPUHasCMOV    ;", &sys_bCPUHasCMOV );
	_pShell->DeclareSymbol("user const INDEX sys_iCPUMHz        ;", &sys_iCPUMHz     );
	_pShell->DeclareSymbol("     const INDEX sys_iCPUMisc       ;", &sys_iCPUMisc    );
	// RAM info
	_pShell->DeclareSymbol("user const INDEX sys_iRAMPhys;", &sys_iRAMPhys);
	_pShell->DeclareSymbol("user const INDEX sys_iRAMSwap;", &sys_iRAMSwap);
	_pShell->DeclareSymbol("user const INDEX sys_iHDDSize;", &sys_iHDDSize);
	_pShell->DeclareSymbol("user const INDEX sys_iHDDFree;", &sys_iHDDFree);
	_pShell->DeclareSymbol("     const INDEX sys_iHDDMisc;", &sys_iHDDMisc);
	// MOD info
	_pShell->DeclareSymbol("user const CTString sys_strModName;", &sys_strModName);
	_pShell->DeclareSymbol("user const CTString sys_strModExt;",  &sys_strModExt);

	// Stock clearing
	extern void FreeUnusedStock(void);
	_pShell->DeclareSymbol("user void FreeUnusedStock(void);", &FreeUnusedStock);
	
	// Timer tick quantum
	_pShell->DeclareSymbol("user const FLOAT fTickQuantum;", (FLOAT*)&_pTimer->TickQuantum);
	_pShell->DeclareSymbol("persistent user INDEX gam_iMaxSaveGameSize;",&gam_iMaxSaveGameSize);
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
	_pShell->DeclareSymbol("user FLOAT g_fGWTime;",&g_fGWTime);
	_pShell->DeclareSymbol("user FLOAT g_fGWTimeMul;",&g_fGWTimeMul);
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
//안태훈 수정 시작	//(For Performance)(0.1)
	_pShell->DeclareSymbol("persistent user INDEX g_bRenderDecoration;", &g_bRenderDecoration);
//안태훈 수정 끝	//(For Performance)(0.1)
//안태훈 수정 시작	//(Option)(0.1)
	_pShell->DeclareSymbol("persistent user FLOAT ter_fLODMul;", &ter_fLODMul);
	_pShell->DeclareSymbol("persistent user FLOAT g_fChaLODMul;", &g_fChaLODMul);
	_pShell->DeclareSymbol("persistent user INDEX g_iUseBloom;", &g_iUseBloom);
	_pShell->DeclareSymbol("           user FLOAT g_fFarClipPlus;", &g_fFarClipPlus);
//안태훈 수정 끝	//(Option)(0.1)

//강동민 수정 시작 클로즈 2차 작업	08.18
	_pShell->DeclareSymbol("persistent user INDEX g_iShadowDetail;",&g_iShadowDetail);
	_pShell->DeclareSymbol("persistent user INDEX g_iReflectionDetail;",&g_iReflectionDetail);
//강동민 수정 끝 클로즈 2차 작업		08.18
//안태훈 수정 시작	//(Taiwan Closed beta)(0.2)
	_pShell->DeclareSymbol("persistent      INDEX g_iCountry;", &g_iCountry);
	_pShell->DeclareSymbol("persistent      INDEX g_iEnterChat;", &g_iEnterChat);
	_pShell->DeclareSymbol("persistent      INDEX g_iShowName;", &g_iShowName);
	_pShell->DeclareSymbol("persistent      INDEX g_iShowNameItem;", &g_iShowNameItem);
	_pShell->DeclareSymbol("persistent      INDEX g_iAutoAttack;", &g_iAutoAttack);
//안태훈 수정 끝	//(Taiwan Closed beta)(0.2)
	// wooss 070401 ---------------------------------------------------------------->><<
	// kw : WSS_HELP_SYSTEM_1
	_pShell->DeclareSymbol("persistent      INDEX g_iShowHelp1Icon;", &g_iShowHelp1Icon);

	// [090715: selo]
	_pShell->DeclareSymbol("persistent      INDEX g_iSaveID;", &g_iSaveID);	
	_pShell->DeclareSymbol("persistent user CTString g_strSaveID;", &g_strSaveID);

#ifdef EUROUPEAN_SERVER_LOGIN
	// 유로피안 서버 접속 여부 [10/18/2012 Ranma]
	_pShell->DeclareSymbol("persistent      INDEX g_iConnectEuroupean;", &g_iConnectEuroupean);
#endif

	// [7/9/2009 rumist] rejection
	_pShell->DeclareSymbol("persistent		INDEX g_iRejectExchange;", &g_iRejectExchange);
	_pShell->DeclareSymbol("persistent		INDEX g_iRejectParty;", &g_iRejectParty);

	// 1106 UI 개편 - 퀘스트 북 열기 [09/05/11 trylord]
	_pShell->DeclareSymbol("persistent		INDEX g_iQuestBookOpen;", &g_iQuestBookOpen);

	// Map Info. [9/15/2009 rumist]
	_pShell->DeclareSymbol("persistent		INDEX g_iZoomInMap;", &g_iZoomInMap);
	_pShell->DeclareSymbol("persistent		INDEX g_iOpacityInMap;", &g_iOpacityInMap);

	// UI 개편 작업. [9/16/2009 rumist]
	_pShell->DeclareSymbol("persistent		INDEX g_iXPosInMap;", &g_iXPosInMap);
	_pShell->DeclareSymbol("persistent		INDEX g_iYPosInMap;", &g_iYPosInMap);
	_pShell->DeclareSymbol("persistent		INDEX g_iXPosInInventory;", &g_iXPosInInventory);
	_pShell->DeclareSymbol("persistent		INDEX g_iYPosInInventory;", &g_iYPosInInventory);
	_pShell->DeclareSymbol("persistent		INDEX g_iXPosInInvenSlot1;", &g_iXPosInInvenSlot1);
	_pShell->DeclareSymbol("persistent		INDEX g_iYPosInInvenSlot1;", &g_iYPosInInvenSlot1);
	_pShell->DeclareSymbol("persistent		INDEX g_iXPosInInvenSlot2;", &g_iXPosInInvenSlot2);
	_pShell->DeclareSymbol("persistent		INDEX g_iYPosInInvenSlot2;", &g_iYPosInInvenSlot2);
	_pShell->DeclareSymbol("persistent		INDEX g_iXPosInInvenSlot3;", &g_iXPosInInvenSlot3);
	_pShell->DeclareSymbol("persistent		INDEX g_iYPosInInvenSlot3;", &g_iYPosInInvenSlot3);
	
	// UI 퀵슬롯 확장 개편 작업 확장 슬롯 위치 [12/18/2012 Ranma]
	_pShell->DeclareSymbol("persistent		INDEX g_iXPosQuickSlotEX1;", &g_iXPosQuickSlotEX1);
	_pShell->DeclareSymbol("persistent		INDEX g_iYPosQuickSlotEX1;", &g_iYPosQuickSlotEX1);
	_pShell->DeclareSymbol("persistent		INDEX g_iXPosQuickSlotEX2;", &g_iXPosQuickSlotEX2);
	_pShell->DeclareSymbol("persistent		INDEX g_iYPosQuickSlotEX2;", &g_iYPosQuickSlotEX2);
	_pShell->DeclareSymbol("persistent		INDEX g_bQuickSlotEX1;", &g_bQuickSlotEX1);
	_pShell->DeclareSymbol("persistent		INDEX g_bQuickSlotEX2;", &g_bQuickSlotEX2);
	_pShell->DeclareSymbol("persistent		INDEX g_bQuickSlot1HorOrVer;", &g_bQuickSlot1HorOrVer);
	_pShell->DeclareSymbol("persistent		INDEX g_bQuickSlot1Lock;", &g_bQuickSlot1Lock);
	_pShell->DeclareSymbol("persistent		INDEX g_bQuickSlot2HorOrVer;", &g_bQuickSlot2HorOrVer);
	_pShell->DeclareSymbol("persistent		INDEX g_bQuickSlot2Lock;", &g_bQuickSlot2Lock);
	
	_pShell->DeclareSymbol("persistent		INDEX g_iXPosInCharInfo;", &g_iXPosInCharInfo);
	_pShell->DeclareSymbol("persistent		INDEX g_iYPosInCharInfo;", &g_iYPosInCharInfo);
	_pShell->DeclareSymbol("persistent		INDEX g_iXPosInQuest;", &g_iXPosInQuest);
	_pShell->DeclareSymbol("persistent		INDEX g_iYPosInQuest;", &g_iYPosInQuest);
	_pShell->DeclareSymbol("persistent		INDEX g_iXPosInPetInfo;", &g_iXPosInPetInfo);
	_pShell->DeclareSymbol("persistent		INDEX g_iYPosInPetInfo;", &g_iYPosInPetInfo);
	_pShell->DeclareSymbol("persistent		INDEX g_iXPosInWPetInfo;", &g_iXPosInWPetInfo);
	_pShell->DeclareSymbol("persistent		INDEX g_iYPosInWPetInfo;", &g_iYPosInWPetInfo);
	_pShell->DeclareSymbol("persistent		INDEX g_iXPosInMessanger;", &g_iXPosInMessanger);
	_pShell->DeclareSymbol("persistent		INDEX g_iYPosInMessanger;", &g_iYPosInMessanger);
	_pShell->DeclareSymbol("persistent		INDEX g_iXPosInSystemMenu;", &g_iXPosInSystemMenu);
	_pShell->DeclareSymbol("persistent		INDEX g_iYPosInSystemMenu;", &g_iYPosInSystemMenu);

	// NPC Scroll UI Position [9/21/2009 rumist]
	_pShell->DeclareSymbol("persistent		INDEX g_iXPosInNPCScroll;", &g_iXPosInNPCScroll);
	_pShell->DeclareSymbol("persistent		INDEX g_iYPosInNPCScroll;", &g_iYPosInNPCScroll);

	// UI_REFORM :Su-won
	_pShell->DeclareSymbol("persistent      INDEX g_bSlaveNoTarget;", &g_bSlaveNoTarget);

	// Debugging symbols 
	_pShell->DeclareSymbol("INDEX dbg_bAtHome;", &dbg_bAtHome);

	_pShell->DeclareSymbol("persistent      INDEX g_bTestClient;", &g_bTestClient);
	
	_pShell->DeclareSymbol("persistent      INDEX g_iTempFlag;", &g_iTempFlag);

	// init MODs and stuff ...
	extern void InitStreams(void);
	InitStreams();

	// load persistent symbols
	if (!_bDedicatedServer) {
//안태훈 수정 시작	//(Taiwan Closed beta)(0.2)
		_pShell->Execute(CTString("decode \"")+fnmPersistentSymbols+"\";");
//안태훈 수정 끝	//(Taiwan Closed beta)(0.2)
	}

	// Create log file for console if not allready created
//안태훈 수정 시작	//(Block Log)(0.1)
    //_pConsole->CreateLogFile(fnmLog);
//안태훈 수정 시작	//(Block Log)(0.1)

	// keep mod name in sys cvar
	sys_strModName = _strModName;
	sys_strModExt  = _strModExt;

// checking of crc
#if 0
	ULONG ulCRCActual = -2;
	SLONG ulCRCExpected = -1;
	try {
		// get the checksum of engine
		#ifndef NDEBUG
			//#define SELFFILE "Bin\\Debug\\EngineD.dll"
			#define SELFCRCFILE "Bin\\Debug\\EngineD.crc"
		#	if		defined(_MSC_VER) && (_MSC_VER >= 1600)
		#		define SELFFILE "Bin\\Debug2010\\EngineD.dll"
		#	else
		#		define SELFFILE "Bin\\Debug\\EngineD.dll"
		#	endif
		#else
			#define SELFFILE "Bin\\Engine.dll"
			#define SELFCRCFILE "Bin\\Engine.crc"
		#endif
		ulCRCActual = GetFileCRC32_t(CTString(SELFFILE));
		// load expected checksum from the file on disk
		ulCRCExpected = 0;
		LoadIntVar(CTString(SELFCRCFILE), ulCRCExpected);
	} catch (char *strError) {
		CPrintF("%s\n", strError);
	}
	// if not same
	if (ulCRCActual!=ulCRCExpected) {
		// don't run
		//FatalError(TRANS("Engine CRC is invalid.\nExpected %08x, but found %08x.\n"), ulCRCExpected, ulCRCActual);
	}
#endif

	_pNetwork = new CNetworkLibrary;

	_pInput->Initialize();

	_pGfx->Init();
	_pSound->Init();

	if (strGameID!="") {
		_pNetwork->Init(strGameID);
/*    // just make classes declare their shell variables
		try {
			_pEntityClassStock->Release(_pEntityClassStock->Obtain_t(
				CTString("Classes\\Player.ecl")));  // this must not be a dependency!
		// if couldn't find player class
		} catch (char *strError) {
			FatalError(TRANS("Cannot initialize classes:\n%s"), strError);
		}
		*/
	} else {
		_pNetwork = NULL;
	}

	// mark that default fonts aren't loaded (yet)
	_pfdDisplayFont = NULL;
	_pfdConsoleFont = NULL;
	//////////////////////////////////////////////////////////////////////////
	_pfdGameFont = NULL; 
	//////////////////////////////////////////////////////////////////////////

	// init IFeel
	HWND hwnd = NULL;//GetDesktopWindow();
	HINSTANCE hInstance = GetModuleHandle(NULL);
	if(IFeel_InitDevice(hInstance,hwnd))
	{
		CTString strDefaultProject = "Data\\Default.ifr";
		// get project file name for this device
		CTString strIFeel = IFeel_GetProjectFileName();
		// if no file name is returned use default file
		if(strIFeel.Length()==0) strIFeel = strDefaultProject;
		if(!IFeel_LoadFile(strIFeel))
		{
			if(strIFeel!=strDefaultProject)
			{
				IFeel_LoadFile(strDefaultProject);
			}
		}
		CPrintF("\n");
	}
}


ENGINE_API void CheckEngineVersion()
{
	if( !((g_bTestClient && g_uiEngineVersion >= 10000) || (!g_bTestClient && g_uiEngineVersion < 10000)) )
	{
		if(g_szExitError) delete[] g_szExitError;
		g_szExitError = new char[4096];
		strcpy(g_szExitError, _S( 864, "버전 정보가 맞지 않습니다." ));
		_pGameState->Running() = FALSE;
		_pGameState->QuitScreen() = FALSE;
	}
}

ENGINE_API void EndLodData()
{
	CSpecialSkill::release();
	CItemData::release();
	COptionData::release();
	CFortuneData::release();
	CItemRareOption::release();
	CSetItemData::release();
	CAction::release();
	CMissionCase::release();
	CNickNameData::release();
	CQuestStaticData::release();
	CMobData::release();
	CNpcHelp::release();
	CMobHelp::release();
	CWildPetData::release();
	CLevelUpGuide::release();
	CItemCollectionData::Release();
}


// shutdown entire engine
ENGINE_API void SE_EndEngine(void)
{
	Finalize_EffectSystem();
	// save persistent symbols
#ifndef UI_TOOL
	if (!_bDedicatedServer) {
		_pShell->StoreEncodePersistentSymbols(fnmPersistentSymbols);
	}
#endif // UI_TOOL

	StringLoader::destroy();
	if(_pGameState)
		_pGameState->DeleteSelectModel(); // Free Stocks 보다 먼저 삭제하여야 한다.

	CLoginJobInfo::destroy(); // 게임 스테이트의 모델보다 나중에 지워져야 한다.
	CJobInfo::destroy();

	CUIManager::destroy();

	GameDataManager::getSingleton()->DestroyAll();
	GameDataManager::destroy();

	StageMgr::getSingleton()->DestroyAll();
	StageMgr::destroy();

	CSkillTree::Destroy();
	CArmorPreview::Destroy();
	CTEventString::Destroy();
	CTradeItem::Destroy();

	EndLodData();

	CZoneInfo::destroy();

	CLoadingImage::destroy();

	ActorMgr::destroy();
	MyInfo::destroy();
	ServerInfo::destroy();
	ObjInfo::destroy();

	_pfdDisplayFont->Clear();
	FONT_STOCK_RELEASE(_pfdDisplayFont);
	_pfdConsoleFont->Clear();
	FONT_STOCK_RELEASE(_pfdConsoleFont);

#ifdef G_RUSSIA
	_pfdDefaultFont->Clear();
	FONT_STOCK_RELEASE(_pfdDefaultFont);
#endif // G_RUSSIA

	// free stocks
	delete _pEntityClassStock;   _pEntityClassStock   = NULL;
	delete _pModelInstanceStock; _pModelInstanceStock = NULL;
	delete _pModelStock;         _pModelStock         = NULL; 
	delete _pSoundStock;         _pSoundStock         = NULL; 
	delete _pFontStock;          _pFontStock          = NULL; 
	delete _pModelInstanceStock; _pModelInstanceStock = NULL;
	delete _pTextureStock;       _pTextureStock       = NULL; 
	delete _pAnimStock;          _pAnimStock          = NULL; 
	delete _pMeshStock;          _pMeshStock          = NULL; 
	delete _pSkeletonStock;      _pSkeletonStock      = NULL; 
	delete _pAnimSetStock;       _pAnimSetStock       = NULL; 
	delete _pShaderStock;        _pShaderStock        = NULL; 

	// free all memory used by the crc cache
	CRCT_Clear();

	// shutdown
	if( _pNetwork != NULL) { 
		delete _pNetwork;  
		_pNetwork = NULL; 
	}
	delete _pInput;    _pInput   = NULL;  
	delete _pSound;    _pSound   = NULL;  
	delete _pGfx;      _pGfx     = NULL;    
	delete _pTimer;    _pTimer   = NULL;  
	delete _pShell;    _pShell   = NULL;  
	delete _pConsole;  _pConsole = NULL;

	if(_pGameState)
	{
		delete _pGameState;
		_pGameState = NULL;
	}

	extern void EndStreams(void);
	EndStreams();

	// shutdown profilers
	_sfStats.Clear();
	_pfNetworkProfile.pf_apcCounters.Clear();
	_pfNetworkProfile.pf_aptTimers.Clear();
	_pfWorldEditingProfile.pf_apcCounters.Clear();
	_pfWorldEditingProfile.pf_aptTimers.Clear();

	// deinit IFeel
	IFeel_DeleteDevice();
	// Close exception handler
	CloseExceptionHandler();
}


// prepare and load some default fonts
ENGINE_API void SE_LoadDefaultFonts(void)
{
	// try to load default fonts
	try {
		_pfdDisplayFont = _pFontStock->Obtain_t( CTFILENAME( "Fonts\\Standard.fnt"));
		_pfdConsoleFont = _pFontStock->Obtain_t( CTFILENAME( "Fonts\\Console1.fnt"));

		//_pfdGameFont = _pFontStock->Obtain_t( CTFILENAME( "Fonts\\Korean.fnt") );
#if defined (G_RUSSIA)
		_pfdDefaultFont = _pFontStock->Obtain_t( CTFILENAME( "Fonts\\Russia.fnt"));
#endif
	}
	catch (char *strError) {
		FatalError( TRANS("Error loading font: %s."), strError);
	}
	// change fonts' default spacing a bit
	_pfdConsoleFont->SetFixedWidth();
	_pfdDisplayFont->SetCharSpacing( 0);
	_pfdConsoleFont->SetCharSpacing(-1);
	_pfdDisplayFont->SetLineSpacing(-1);
	_pfdConsoleFont->SetLineSpacing( 0);
}


// updates main windows' handles for windowed mode and fullscreen
ENGINE_API void SE_UpdateWindowHandle( HWND hwndMain, HWND hdlgWebInterface)
{
	ASSERT( hwndMain!=NULL);
	_hwndMain = hwndMain;
	_hDlgWeb = hdlgWebInterface;
	_bFullScreen = _pGfx!=NULL && (_pGfx->gl_ulFlags&GLF_FULLSCREEN);
}


static BOOL TouchBlock(UBYTE *pubMemoryBlock, INDEX ctBlockSize)
{
	// cannot pretouch block that are smaller than 64KB :(
	ctBlockSize -= 16*0x1000;
	if( ctBlockSize<4) return FALSE; 

	__try {
		// 4 times should be just enough
		for( INDEX i=0; i<4; i++) {
			// must do it in asm - don't know what VC will try to optimize
			__asm {
				// The 16-page skip is to keep Win 95 from thinking we're trying to page ourselves in
				// (we are doing that, of course, but there's no reason we shouldn't) - THANX JOHN! :)
				mov   esi,dword ptr [pubMemoryBlock]
				mov   ecx,dword ptr [ctBlockSize]
				shr   ecx,2
touchLoop:
				mov   eax,dword ptr [esi]
				mov   ebx,dword ptr [esi+16*0x1000]
				add   eax,ebx     // BLA, BLA, TROOCH, TRUCH
				add   esi,4
				dec   ecx
				jnz   touchLoop
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER) { 
		return FALSE;
	}
	return TRUE;
}

// pretouch all memory commited by process
extern BOOL _bNeedPretouch = FALSE;
ENGINE_API extern void SE_PretouchIfNeeded(void)
{
	// only if pretouching is needed?
	extern INDEX gam_bPretouch;
	if( !_bNeedPretouch || !gam_bPretouch) return;
	_bNeedPretouch = FALSE;

	CallProgressHook_t(0.0f);

	// need to do this two times - 1st for numerations, and 2nd for real (progress bar and that shit)
	BOOL bPretouched = TRUE;
	INDEX ctFails, ctBytes, ctBlocks;
	INDEX ctPassBytes, ctTotalBlocks;
	for( INDEX iPass=1; iPass<=2; iPass++)
	{ 
		// flush variables
		ctFails=0; ctBytes=0; ctBlocks=0; ctTotalBlocks=0;
		void *pvNextBlock = NULL;
		MEMORY_BASIC_INFORMATION mbi;
		// lets walk thru memory blocks
		while( VirtualQuery( pvNextBlock, &mbi, sizeof(mbi)))
		{ 
			// don't mess with kernel's memory and zero-sized blocks    
			if( ((ULONG)pvNextBlock)>0x7FFF0000UL || mbi.RegionSize<1) break;

			// if this region of memory belongs to our process
			BOOL bCanAccess = (mbi.Protect==PAGE_READWRITE); // || (mbi.Protect==PAGE_EXECUTE_READWRITE);
			if( mbi.State==MEM_COMMIT && bCanAccess && mbi.Type==MEM_PRIVATE) // && !IsBadReadPtr( mbi.BaseAddress, 1)
			{ 
				// increase counters
				ctBlocks++;
				ctBytes += mbi.RegionSize;
				// in first pass we only count
				if( iPass==1) goto nextRegion;
				// update progress bar
				CallProgressHook_t( (FLOAT)ctBytes/ctPassBytes);
				// pretouch
				ASSERT( mbi.RegionSize>0);
				BOOL bOK = TouchBlock((UBYTE *)mbi.BaseAddress, mbi.RegionSize);
				if( !bOK) { 
					// whoops!
					ctFails++;
				}
				// for easier debugging (didn't help much, though)
				//Sleep(5);  
			}
nextRegion:
			// advance to next region
			pvNextBlock = ((UBYTE*)mbi.BaseAddress) + mbi.RegionSize;
			ctTotalBlocks++;
		}
		// done with one pass
		ctPassBytes = ctBytes;
		if( (ctPassBytes/1024/1024)>sys_iRAMPhys) {
			// not enough RAM, sorry :(
			bPretouched = FALSE;
			break;
		}
	}

	// report
	if( bPretouched) {
		// success
		CPrintF( TRANS("Pretouched %d KB of memory in %d blocks.\n"), ctBytes/1024, ctBlocks); //, ctTotalBlocks);
	} else {
		// fail
		CPrintF( TRANS("Cannot pretouch due to lack of physical memory (%d KB of overflow).\n"), ctPassBytes/1024-sys_iRAMPhys*1024);
	}
	// some blocks failed?
	if( ctFails>1) CPrintF( TRANS("(%d blocks were skipped)\n"), ctFails);
	//_pShell->Execute("StockDump();");
}


ENGINE_API CWebAddress* SE_Get_WebAddressPtr()
{
	return CWebAddress::getSingleton();
}


ENGINE_API CUIManager* SE_Get_UIManagerPtr()
{
	return CUIManager::getSingleton();
}

ENGINE_API GameDataManager* SE_Get_GameDataManagerPtr()
{
	return GameDataManager::getSingleton();
}

ENGINE_API void SE_Destroy_WebAddressPtr()
{
	CWebAddress::destroy();
}


ENGINE_API void SE_Destroy_UIManagerPtr()
{
	CUIManager::destroy();
}

ENGINE_API void SE_Destroy_GameDataManagerPtr()
{
	GameDataManager::getSingleton()->destroy();
}


ENGINE_API UtilHelp* SE_Get_UtilHelpPtr()
{
	return UtilHelp::getSingleton();
}

#if 0

			// printout block info
			CPrintF("--------\n");
			CTString strTmp1, strTmp2;
			CPrintF("Base/Alloc Address: 0x%8X / 0x%8X - Size: %d KB\n", mbi.BaseAddress, mbi.AllocationBase, mbi.RegionSize/1024);
			switch( mbi.Protect) {
			case PAGE_READONLY:          strTmp1 = "PAGE_READONLY";          break;
			case PAGE_READWRITE:         strTmp1 = "PAGE_READWRITE";         break;
			case PAGE_WRITECOPY:         strTmp1 = "PAGE_WRITECOPY";         break;
			case PAGE_EXECUTE:           strTmp1 = "PAGE_EXECUTE";           break;
			case PAGE_EXECUTE_READ:      strTmp1 = "PAGE_EXECUTE_READ";      break;
			case PAGE_EXECUTE_READWRITE: strTmp1 = "PAGE_EXECUTE_READWRITE"; break;
			case PAGE_GUARD:             strTmp1 = "PAGE_GUARD";             break;
			case PAGE_NOACCESS:          strTmp1 = "PAGE_NOACCESS";          break;
			case PAGE_NOCACHE:           strTmp1 = "PAGE_NOCACHE";           break;
			}
			switch( mbi.AllocationProtect) {
			case PAGE_READONLY:          strTmp2 = "PAGE_READONLY";          break;
			case PAGE_READWRITE:         strTmp2 = "PAGE_READWRITE";         break;
			case PAGE_WRITECOPY:         strTmp2 = "PAGE_WRITECOPY";         break;
			case PAGE_EXECUTE:           strTmp2 = "PAGE_EXECUTE";           break;
			case PAGE_EXECUTE_READ:      strTmp2 = "PAGE_EXECUTE_READ";      break;
			case PAGE_EXECUTE_READWRITE: strTmp2 = "PAGE_EXECUTE_READWRITE"; break;
			case PAGE_GUARD:             strTmp2 = "PAGE_GUARD";             break;
			case PAGE_NOACCESS:          strTmp2 = "PAGE_NOACCESS";          break;
			case PAGE_NOCACHE:           strTmp2 = "PAGE_NOCACHE";           break;
			}
			CPrintF("Current/Alloc protect: %s / %s\n", strTmp1, strTmp2);
			switch( mbi.State) {
			case MEM_COMMIT:  strTmp1 = "MEM_COMMIT";  break;
			case MEM_FREE:    strTmp1 = "MEM_FREE";    break;
			case MEM_RESERVE: strTmp1 = "MEM_RESERVE"; break;
			}
			switch( mbi.Type) {
			case MEM_IMAGE:   strTmp2 = "MEM_IMAGE";   break;
			case MEM_MAPPED:  strTmp2 = "MEM_MAPPED";  break;
			case MEM_PRIVATE: strTmp2 = "MEM_PRIVATE"; break;
			}

			CPrintF("State/Type: %s / %s\n", strTmp1, strTmp2);

#endif
