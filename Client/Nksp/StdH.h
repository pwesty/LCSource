
#pragma warning(disable : 4996)

//#define CKBANG_UI_TWITTER
#ifdef CKBANG_UI_TWITTER

extern "C"
{
    #include <Engine/lua/lua.h>
    #include <Engine/lua/lualib.h>
    #include <Engine/lua/lauxlib.h>
};

#endif

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#endif 

#include <boost/function.hpp>
#include <Engine/Engine.h>
#include <Engine/Templates/Stock_CTextureData.h>
#include <Engine/Templates/Stock_CModelData.h>
#include <GameMP/Game.h>

/* rcg10042001 protect against Visual C-isms. */
#ifdef _MSC_VER
#define DECL_DLL _declspec(dllimport)
#endif

#ifdef PLATFORM_UNIX
#define DECL_DLL 
#endif

#include <EntitiesMP/Global.h>
#include <EntitiesMP/Common/Common.h>
#include <EntitiesMP/Common/GameInterface.h>
#include <EntitiesMP/Player.h>
#undef DECL_DLL

#define TECHTESTONLY 0

#include "Nksp.h"
//#include "Menu.h"
//#include "MenuGadgets.h"

#pragma warning(disable : 4996)