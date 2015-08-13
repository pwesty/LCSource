#include "StdH.h"
#include <Engine/lua/LuaInterface.h>

pLua_pushnumber         ILUA_pushnumber = NULL;
pLua_pushstring         ILUA_pushstring = NULL;
pLua_register           ILUA_register = NULL;
pLua_dofile             ILUA_dofile = NULL;
pLua_gettop             ILUA_gettop = NULL;
pLua_isnumber           ILUA_isnumber = NULL;
pLua_isstring           ILUA_isstring = NULL;
pLua_tonumber           ILUA_tonumber = NULL;
pLua_tostring           ILUA_tostring = NULL;

