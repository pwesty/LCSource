
#pragma warning(disable : 4391)		// Platform SDK Warning
#pragma warning(disable : 4996)

#if		!defined(WORLD_EDITOR)
#include <boost/function.hpp>
#endif	// WORLD_EDITOR
#include <Engine/Engine.h>
#include <GameMP/Game.h>
#include <GameMP/SEColors.h>

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
