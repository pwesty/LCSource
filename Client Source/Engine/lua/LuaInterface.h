#ifndef _LUA_INTERFACE_H_
#define _LUA_INTERFACE_H_

typedef void        (*pLua_pushnumber)(double n);
typedef void        (*pLua_pushstring)(const char* s);
typedef void        (*pLua_register)(const char* s, lua_CFunction fn);
typedef int         (*pLua_dofile)(const char* s);
typedef int         (*pLua_gettop)();
typedef int         (*pLua_isnumber)(int idx);
typedef int         (*pLua_isstring)(int idx);
typedef double      (*pLua_tonumber)(int idx);
typedef const char* (*pLua_tostring)(int idx);

extern ENGINE_API pLua_pushnumber       ILUA_pushnumber;
extern ENGINE_API pLua_pushstring       ILUA_pushstring;
extern ENGINE_API pLua_register         ILUA_register;
extern ENGINE_API pLua_dofile           ILUA_dofile;
extern ENGINE_API pLua_gettop           ILUA_gettop;
extern ENGINE_API pLua_isnumber         ILUA_isnumber;
extern ENGINE_API pLua_isstring         ILUA_isstring;
extern ENGINE_API pLua_tonumber         ILUA_tonumber;
extern ENGINE_API pLua_tostring         ILUA_tostring;    

#endif
