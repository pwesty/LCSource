#ifndef		__ENGINE_H__
#define		__ENGINE_H__

// set this to 1 to enable checks whether somethig is deleted while iterating some array/container
#define CHECKARRAYLOCKING 0

#ifdef _WIN32
  #ifndef PLATFORM_WIN32
    #define PLATFORM_WIN32 1
  #endif
#endif

#pragma warning(disable : 4786)
#pragma warning(disable : 4200)

#include <stdlib.h>
#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <time.h>
#include <math.h>
#include <search.h>   // for qsort
#include <float.h>    // for FPU control

/* rcg10042001 !!! FIXME: Move these somewhere. */
#if (defined PLATFORM_WIN32)
#include <conio.h>
#include <crtdbg.h>
#include <winsock2.h>
#include <windows.h>
#include <mmsystem.h> // for timers
#endif

#include <Engine/Base/Base.h>
#include <Engine/Base/Types.h>

#include <Engine/Base/CTString.h>
#include <Engine/Base/CTWString.h>
#include <Engine/Base/Input.h>
#include <Engine/Base/KeyNames.h>
#include <Engine/Base/Updateable.h>
#include <Engine/Base/ErrorReporting.h>
#include <Engine/Base/ErrorTable.h>
#include <Engine/Base/ReplaceFile.h>
#include <Engine/Base/Stream.h>
#include <Engine/Base/Lists.h>
#include <Engine/Base/Timer.h>
#include <Engine/Base/ListIterator.inl>
#include <Engine/Base/Console.h>
#include <Engine/Base/Console_internal.h>
// #include <Engine/Base/Shell_internal.h>
#include <Engine/Base/Shell.h>
#include <Engine/Base/Statistics.h>
#include <Engine/Base/CRC.h>
#include <Engine/Base/Translation.h>
#include <Engine/Base/Registry.h>
#include <Engine/Base/IFeel.h>

#include <Engine/Entities/EntityClass.h>
// #include <Engine/Entities/EntityCollision.h>
// #include <Engine/Entities/EntityProperties.h>
// #include <Engine/Entities/Entity.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Entities/LastPositions.h>
#include <Engine/Entities/EntityCollision.h>
#include <Engine/Entities/ShadingInfo.h>
#include <Engine/Entities/FieldSettings.h>
#include <Engine/Entities/Precaching.h>

#include <Engine/Light/LightSource.h>
#include <Engine/Light/LensFlares.h>
#include <Engine/Light/Shadows_internal.h>
#include <Engine/Light/Gradient.h>

#include <Engine/Math/Geometry.inl>
#include <Engine/Math/Clipping.inl>
#include <Engine/Math/FixInt.h>
#include <Engine/Math/Float.h>
#include <Engine/Math/Object3D.h>
#include <Engine/Math/Functions.h>
#include <Engine/Math/Quaternion.h>
#include <Engine/Math/Projection.h>
#include <Engine/Math/Projection_DOUBLE.h>
//안태훈 수정 시작	//(For Performance)(0.1)
#include <Engine/Math/AdditionalFunction.h>
//안태훈 수정 끝	//(For Performance)(0.1)

#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
//#include <Engine/Network/NetworkMessage.h>
#include <Engine/Network/PlayerSource.h>
#include <Engine/Network/PlayerTarget.h>
#include <Engine/Network/Compression.h>

#include <Engine/Brushes/Brush.h>
#include <Engine/Brushes/BrushTransformed.h>
#include <Engine/Brushes/BrushArchive.h>


#include <Engine/World/World.h>
#include <Engine/World/WorldEditingProfile.h>
#include <Engine/World/WorldRayCasting.h>
#include <Engine/World/WorldSettings.h>
#include <Engine/World/WorldCollision.h>

#include <Engine/Rendering/Render.h>
#include <Engine/Rendering/Render_internal.h>

#include <Engine/Models/ModelObject.h>
#include <Engine/Models/ModelData.h>
#include <Engine/Models/Model_internal.h>
#include <Engine/Models/EditModel.h>
#include <Engine/Models/RenderModel.h>

#include <Engine/Ska/ModelInstance.h>
#include <Engine/Ska/Mesh.h>
#include <Engine/Ska/Skeleton.h>
#include <Engine/Ska/AnimSet.h>
#include <Engine/Ska/StringTable.h>
#include <Engine/Ska/Render.h>

#include <Engine/Sound/SoundObject.h>
#include <Engine/Sound/SoundLibrary.h>

#include <Engine/Graphics/Texture.h>
#include <Engine/Graphics/Color.h>
#include <Engine/Graphics/Font.h>
#include <Engine/Graphics/GfxLibrary.h>
#include <Engine/Graphics/ViewPort.h>
#include <Engine/Graphics/DrawPort.h>
#include <Engine/Graphics/ImageInfo.h>
#include <Engine/Graphics/RenderScene.h>
#include <Engine/Graphics/RenderPoly.h>
#include <Engine/Graphics/Fog.h>
#include <Engine/Graphics/Stereo.h>

#include <Engine/Templates/BSP.h>
#include <Engine/Templates/BSP_internal.h>
#include <Engine/Templates/DynamicStackArray.h>
#include <Engine/Templates/DynamicStackArray.cpp>
#include <Engine/Templates/LinearAllocator.h>
#include <Engine/Templates/LinearAllocator.cpp>
#include <Engine/Templates/DynamicArray.h>
#include <Engine/Templates/DynamicArray.cpp>
#include <Engine/Templates/DynamicContainer.h>
#include <Engine/Templates/DynamicContainer.cpp>
#include <Engine/Templates/StaticArray.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Templates/StaticStackArray.h>
#include <Engine/Templates/StaticStackArray.cpp>
#include <Engine/Templates/Selection.h>
#include <Engine/Templates/Selection.cpp>



typedef struct lua_State lua_State;
// some global stuff
ENGINE_API bool SE_CheckEngine();	// [2013/01/16] sykim70
//ENGINE_API void SE_InitEngine( CTString strGameID, BOOL bTcp = FALSE);  // by seo
ENGINE_API void SE_InitEngine( CTString strGameID);
ENGINE_API void SE_EndEngine(void);
ENGINE_API void SE_LoadDefaultFonts(void);
ENGINE_API void SE_UpdateWindowHandle( HWND hwndWindowed, HWND hdlgWebInterface = NULL);
ENGINE_API void SE_PretouchIfNeeded(void);
ENGINE_API const int	SE_GetEngineDllRefCnt();

#ifdef KALYDO
extern ENGINE_API HWND	g_KalydoWindowHandle;
ENGINE_API void SE_UpdateStreamingData(void);
#endif

// Singleton Classes
class CWebAddress;
class CUIManager;
class GameDataManager;
class StageMgr;
class UtilHelp;

ENGINE_API CWebAddress* SE_Get_WebAddressPtr();
ENGINE_API CUIManager*	SE_Get_UIManagerPtr();
ENGINE_API GameDataManager* SE_Get_GameDataManagerPtr();
ENGINE_API StageMgr*	SE_Get_StageMgrPtr();
ENGINE_API void			SE_Destroy_WebAddressPtr();
ENGINE_API void			SE_Destroy_UIManagerPtr();
ENGINE_API void			SE_Destroy_GameDataManagerPtr();
ENGINE_API void			SE_Destroy_StageMgrPtr();
ENGINE_API void			EndLodData();
ENGINE_API UtilHelp*	SE_Get_UtilHelpPtr();

extern ENGINE_API CTString _strEngineBuild;  // not valid before InitEngine()!
extern ENGINE_API ULONG _ulEngineBuildMajor;
extern ENGINE_API ULONG _ulEngineBuildMinor;

extern ENGINE_API BOOL _bDedicatedServer;
extern ENGINE_API BOOL _bWorldEditorApp;			// is this world edtior app
//	김영환 : 클라이언트 전용 엔진 설정 변수
extern ENGINE_API BOOL _bClientApp;			// TRUE 설정되면 게임 클라이언트 이다.
extern ENGINE_API BOOL _bUseBloomInWorldEditor;		// 에디터에 Bloom 효과 On/Off를 위해 추가	:Su-won
extern ENGINE_API BOOL _bTranslucentModel;			// 에디터에 모델을 반투명으로 나타내기 위해 추가	:Su-won
extern ENGINE_API BOOL _bInvisibleOff;				// 에디터에 Invisible된 폴리곤을 나타내기 위해 추가	:Su-won
extern ENGINE_API BOOL _bShowPortalPolygon;			// 에디터에서 Portal속성으로 되어있는 폴리곤을 화면에 나타낼지 여부 :Su-won
extern ENGINE_API BOOL _bAttributemap_DepthTest;	//에디터에서 Attribute 맵을 그릴 때 DepthTest 여부 설정 :Su-won

//extern ENGINE_API BOOL _bUseSocket;
extern ENGINE_API BOOL _bSkaStudioApp;				// is this ska studio app
extern ENGINE_API BOOL _bInTestGame;				// Is this test game in world editor					// yjpark
extern ENGINE_API BOOL _bShowPolygonAttribute;		// If attributes of polygons are shown in world editor	// yjpark
extern ENGINE_API BOOL _bLoginProcess;
extern ENGINE_API CTString _strLogFile;

// <-- ErrorLog.txt에 디스플레이 정보를 기록하기 위한 부분
extern ENGINE_API CTString _strDisplayDriver;
extern ENGINE_API CTString _strDisplayDriverVersion;
extern ENGINE_API CTString _strSoundDriver;
extern ENGINE_API CTString _strTotalMemory;
// -->

// main window canvas									// yjpark |<--
extern ENGINE_API BOOL		_bWindowChanging;
extern ENGINE_API CDrawPort	*_pdpMain;
extern ENGINE_API CDrawPort	*_pdpNormalMain;
extern ENGINE_API CDrawPort	*_pdpWideScreenMain;
extern ENGINE_API CViewPort	*_pvpViewPortMain;
extern ENGINE_API HINSTANCE	_hInstanceMain;
extern ENGINE_API INDEX		sam_bFullScreenActive;
extern ENGINE_API INDEX		sam_iScreenSizeI;
extern ENGINE_API INDEX		sam_iScreenSizeJ;
extern ENGINE_API INDEX		sam_iDisplayDepth;
extern ENGINE_API INDEX		sam_iDisplayAdapter;
extern ENGINE_API INDEX		sam_iGfxAPI;
extern ENGINE_API INDEX		sam_bWideScreen;
extern ENGINE_API INDEX		cmd_iWindowLeft;
extern ENGINE_API INDEX		cmd_iWindowTop;	 			// yjpark     -->|

// temporary vars for adjustments
ENGINE_API extern FLOAT tmp_af[20];
ENGINE_API extern INDEX tmp_ai[10];
ENGINE_API extern INDEX tmp_i;
ENGINE_API extern INDEX tmp_fAdd;

//안태훈 수정 시작	//(Add Sun Moon Entity and etc)(0.2)
ENGINE_API extern BOOL g_bBadWeather;
ENGINE_API extern COLOR g_colWeather;
//안태훈 수정 끝	//(Add Sun Moon Entity and etc)(0.2)

// 일본 자동 로그인 060209 wooss 
ENGINE_API BOOL g_bAutoLogin;
ENGINE_API BOOL g_bAutoRestart;
ENGINE_API CTString g_nmID ;
ENGINE_API CTString g_nmPW ;
ENGINE_API CTString g_nmCID ;
ENGINE_API CTString g_nmVER ;

// wooss 070228 ---------------------------->>
// kw : WSS_EVENT_LOD
// 이벤트 플래그 변수
ENGINE_API std::map<int,int> g_mapEvent;
// -----------------------------------------<<
	
//070308_ttos : 브라질 나스 표기 번환
ENGINE_API BOOL g_bNasTrans;	

#endif		// __ENGINE_H__
