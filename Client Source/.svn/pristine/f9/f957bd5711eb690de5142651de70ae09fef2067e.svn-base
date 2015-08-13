/*
Copyright (c) 1999  X Dimension Software

Project:
            3D Exploration / Active X / DLL / LIB
Module Name:
            explor3d.h
Author:
            Alexander Shelemekhov
            alex@xdsoft.com
Description:
            3D Exploration  module
            Based on 3D Exploration engine
Version:
            1.4.00

WWW         http://www.xdsoft.com/explordll/
eMail       alex@xdsoft.com
*/

#ifndef _DD9C9EC0_7148_11D4_9F76_F6D558E42026_
#define _DD9C9EC0_7148_11D4_9F76_F6D558E42026_

#ifdef  _MSC_VER
/*
 * Currently, all MS C compilers for Win32 platforms default to 8 byte
 * alignment.
 */
#pragma pack(push,1)
#endif  /* _MSC_VER */

#define EXPLORATION_WINDOW   "EXPLORATION_3D_WINDOW"
#define EXPLORATION_LIBRRAY  "eview3d.dll"

#define E3_DRAG_NONE    0
#define E3_DRAG_ROTATE  1
#define E3_DRAG_SCALE   2
#define E3_DRAG_PAN     3
#define E3_DRAG_ROTATEZ 4
#define E3_DRAG_CANCEL  0x8000

struct e3_INIT{
 DWORD e_size;
 DWORD e_flags;
 HKEY  e_rootkey;   //HKEY_CLASSES_ROOT or HKEY_CURRENT_USER or HKEY_LOCAL_MACHINE or HKEY_USERS or 0 for HKEY_LOCAL_MACHINE
 char *e_registry;  // registry root dir  "Software\\X Dimension\\3D Exploration" - for 3D Exploration
 char *e_plugins;   // plugins root dir
 char *e_dllfolder; // folder , where located additional dlls dxbmp32.dll, triang32.dll ... e.t.c
                    // if NULL the your APP path
};

class e3_SCENE;
class e3_OBJECT;

typedef e3_API*  (WINAPI * TInitExploration3D)( e3_INIT*);
typedef BOOL  (WINAPI * TDoneExploration3D)(  );

#define E3DM_BASE        WM_USER

#define E3DM_SETFILE     (E3DM_BASE+1)
// wParam - type = 0 , lParam  - full file name
#define E3DM_GETFILE     (E3DM_BASE+2)
// wParam - buffersize , lParam  - pointer to buffer

#define E3DM_SETRENDER   (E3DM_BASE+3)
// wParam 0,1  where 0 - Software, 1 - Open GL
#define E3DM_GETRENDER   (E3DM_BASE+4)

#define E3DM_SET_PARAMI  (E3DM_BASE+5)
#define E3DM_GET_PARAMI  (E3DM_BASE+6)
#define E3DM_SET_PARAMF  (E3DM_BASE+7)
#define E3DM_GET_PARAMF  (E3DM_BASE+8)
#define E3DM_GET_SCENE   (E3DM_BASE+9)
#define E3DM_GET_BITMAP  (E3DM_BASE+10) // lParam e3_BMPEXPORT* , return TRUE/FALSE

#define E3DM_SET_COLOR   (E3DM_BASE+11)
#define E3DM_GET_COLOR   (E3DM_BASE+12)
#define E3DM_LOADOBJECT  (E3DM_BASE+13)
#define E3DM_SAVEFILE    (E3DM_BASE+14)

#define E3DM_SETMATERIAL (E3DM_BASE+15)       // wParam -object
#define E3DM_SETTEXTURE  (E3DM_BASE+16)       // wParam -object
#define E3DM_SETUNIQUEMATERIAL (E3DM_BASE+17) // wParam -object
#define E3DM_SCALE       (E3DM_BASE+18)           // wParam -object,lParam *POINT3D
#define E3DM_ROTATE      (E3DM_BASE+19)           // wParam -object,lParam *POINT3D
#define E3DM_MOVE        (E3DM_BASE+20)           // wParam -object,lParam *POINT3D
#define E3DM_SETOPACITY  (E3DM_BASE+21)           // wParam -object,lParam *float
#define E3DM_ABOUT       (E3DM_BASE+22)           // wParam HWND
#define E3DM_SETVIEW     (E3DM_BASE+23)           // wParam 0 - perspective, 1 - top , 2 - bottom
#define E3DM_AUTOROTATE  (E3DM_BASE+24)           // lparam - POINT2D*
#define E3DM_SETBKPICTURE (E3DM_BASE+25)
#define E3DM_GETBKPICTURE (E3DM_BASE+26)


typedef struct _NM_E3D {
    NMHDR    hdr;
    UINT     action;
//    e3_NODE* object;
    POINT    ptDrag;
    char*    text;     // for status text
} NM_E3D;
typedef NM_E3D FAR *LPNM_E3D;

#define NM_FIRST        (0U-  0U)

#define E3DN_LDOWN       0x1000
#define E3DN_LUP         E3DN_LDOWN+1
#define E3DN_LCLICK      (NM_FIRST-2)
#define E3DN_RDOWN       E3DN_LDOWN+3
#define E3DN_RUP         E3DN_LDOWN+4
#define E3DN_RCLICK      (NM_FIRST-5)
#define E3DN_LDBLCLICK   (NM_FIRST-3)/*NM_DBLCLK*/
#define E3DN_RDBLCLICK   (NM_FIRST-6)
#define E3DN_STATUSTEXT  E3DN_LDOWN+10     // action -part,text - text info
#define E3DN_RENDERING   E3DN_LDOWN+11     // action phase of rendering
#define E3DN_NEWSCENE    E3DN_LDOWN+12     // use e3_GetScene to determine pointer to scene
#define E3DN_AUTOROTATE  E3DN_LDOWN+13     //

enum E3_WND_PARAM{

 E3_RENDERMODE,      // 0-7
 E3_BOUNDINGBOX,     // bool
 E3_WORLDAXIS,       // bool
 E3_LIGHTSCAMERAS,   // bool
 E3_VERTEXES,        // bool
 E3_FOG,             // bool
 E3_HIDDEN,          // bool
 E3_LIGHTMODE,       // 0-9
 E3_FILELIGHTS,       //bool
 E3_SPECULAR,        // bool
 E3_CHANGELIGHT,     // { 0 - restore, 1 - increment , 2 - decrement  for PARAMI } ,{ current * data for PARAMF } set only
 E3_GRIDMODE,        // 0 -3
 E3_GRIDSIZE,
 E3_SMOOTHANGLE,     // PARAMF
 E3_SMOOTH,          // bool
 E3_USESMOOTHGROUPS, // bool


 E3_DEFAULTMATERIAL, // bool
 E3_TRANSPARENCY,    // bool transparency ON/OFF
 E3_TEXTURES,        // bool textures ON/OFF
 E3_ROTATION,        // bool
 E3_LMOUSEMODE,      //  0 - DRAG_ROTATE  , 1 - DRAG_SCALE  , 2 - DRAG_PAN ,  4- DRAG_ROTATEZ
 E3_RMOUSEMODE,
 E3_ZOOMFACTOR,       // float zoom factor

 E3_DEFREFLECTION,
 E3_DEFGLOSSINESS,
 E3_DEFSPECLEVEL,

 E3_SYS_TXTFILTER,
 E3_SYS_TXTRES,
 E3_SYS_GLEXT,
 E3_SYS_TXTPERSPECTIVE,
 E3_SYS_GLSWAPHINT,
};

#define API_OPENGL 1
#define API_SOFT   0

inline void e3_About(HWND hwnd,HWND _hwnd){ SendMessage(hwnd,E3DM_ABOUT,(WPARAM)_hwnd,0); }
inline BOOL e3_SetView(HWND hwnd,int m){ return SendMessage(hwnd,E3DM_SETVIEW,m,0); }
                                                                                                            
inline BOOL e3_RenderToBitmap(HWND hwnd,e3_BMPEXPORT*bmp){ return SendMessage(hwnd,E3DM_GET_BITMAP,0,(LPARAM)(&bmp)); }

inline BOOL e3_SetRenderAPI(HWND hwnd,int mode){ return SendMessage(hwnd,E3DM_SETRENDER,mode,0); }
inline int  e3_GetRenderAPI(HWND hwnd){ return SendMessage(hwnd,E3DM_GETRENDER,0,0); }

inline BOOL e3_LoadFile(HWND hwnd,char*filename,int type=0){ return SendMessage(hwnd,E3DM_SETFILE,type,(LPARAM)filename); }
inline BOOL e3_EmptyScene(HWND hwnd){ return SendMessage(hwnd,E3DM_SETFILE,0,0); }
inline BOOL e3_NewScene(HWND hwnd){ return SendMessage(hwnd,E3DM_SETFILE,0,1); }

inline BOOL e3_SetRenderMode(HWND hwnd,int mode){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_RENDERMODE,mode); }
inline int  e3_GetRenderMode(HWND hwnd) {  return SendMessage(hwnd,E3DM_GET_PARAMI,E3_RENDERMODE,0); }

inline BOOL e3_SetSceneBoundingBox(HWND hwnd,BOOL m){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_BOUNDINGBOX,m); }
inline BOOL e3_GetSceneBoundingBox(HWND hwnd){ return SendMessage(hwnd,E3DM_GET_PARAMI,E3_BOUNDINGBOX,0); }

inline BOOL e3_SetWorldAxis(HWND hwnd,BOOL m){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_WORLDAXIS,m); }
inline BOOL e3_GetWorldAxis(HWND hwnd){ return SendMessage(hwnd,E3DM_GET_PARAMI,E3_WORLDAXIS,0); }

inline BOOL e3_SetShowLights(HWND hwnd,BOOL m){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_LIGHTSCAMERAS,m); }
inline BOOL e3_GetShowLights(HWND hwnd){ return SendMessage(hwnd,E3DM_GET_PARAMI,E3_LIGHTSCAMERAS,0); }

inline BOOL e3_SetShowVertexes(HWND hwnd,BOOL m){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_VERTEXES,m); }
inline BOOL e3_GetShowVertexes(HWND hwnd){ return SendMessage(hwnd,E3DM_GET_PARAMI,E3_VERTEXES,0); }

inline BOOL e3_SetFog(HWND hwnd,BOOL m){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_FOG,m); }
inline BOOL e3_GetFog(HWND hwnd){ return SendMessage(hwnd,E3DM_GET_PARAMI,E3_FOG,0); }

inline BOOL e3_SetShowHidden(HWND hwnd,BOOL m){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_HIDDEN,m); }
inline BOOL e3_GetShowHidden(HWND hwnd){ return SendMessage(hwnd,E3DM_GET_PARAMI,E3_HIDDEN,0); }

inline BOOL e3_SetLightMode(HWND hwnd,int m){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_LIGHTMODE,m); }
inline int e3_GetLightMode(HWND hwnd) {  return SendMessage(hwnd,E3DM_GET_PARAMI,E3_LIGHTMODE,0); }

inline BOOL e3_EnableFileLight(HWND hwnd,BOOL m){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_FILELIGHTS,m); }
inline BOOL e3_IsFileLightEnabled(HWND hwnd) {  return SendMessage(hwnd,E3DM_GET_PARAMI,E3_FILELIGHTS,0); }

inline BOOL e3_SetSpecularLighting(HWND hwnd,BOOL m){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_SPECULAR,m); }
inline BOOL e3_GetSpecularLighting(HWND hwnd){ return SendMessage(hwnd,E3DM_GET_PARAMI,E3_SPECULAR,0); }

inline BOOL e3_IncrementLight(HWND hwnd){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_CHANGELIGHT,1); }
inline BOOL e3_DecrementLight(HWND hwnd){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_CHANGELIGHT,2); }
inline BOOL e3_RestoreLight(HWND hwnd){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_CHANGELIGHT,0); }

inline BOOL e3_SetLightsScale(HWND hwnd,float f) {
                                           return SendMessage(hwnd,E3DM_SET_PARAMF,E3_CHANGELIGHT,(DWORD)&f);
                                         };
inline float e3_GetLightsScale(HWND hwnd) { float f=0;
                                           SendMessage(hwnd,E3DM_GET_PARAMF,E3_CHANGELIGHT,(DWORD)&f);
                                           return f;
                                         };

inline BOOL e3_SetGridMode(HWND hwnd,int m){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_GRIDMODE,m); }
inline int  e3_GetGridMode(HWND hwnd){ return SendMessage(hwnd,E3DM_GET_PARAMI,E3_GRIDMODE,0); }

inline BOOL e3_SetGridSize(HWND hwnd,int m){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_GRIDSIZE,m); }
inline int  e3_GetGridSize(HWND hwnd){ return SendMessage(hwnd,E3DM_GET_PARAMI,E3_GRIDSIZE,0); }

inline BOOL  e3_SetDefReflection(HWND hwnd,float m){ return SendMessage(hwnd,E3DM_SET_PARAMF,E3_DEFREFLECTION,(DWORD)&m); }
inline float e3_GetDefReflection(HWND hwnd){ float f=0;SendMessage(hwnd,E3DM_GET_PARAMF,E3_DEFREFLECTION,(DWORD)&f);return f;}

inline BOOL  e3_SetDefGlossiness(HWND hwnd,float m){ return SendMessage(hwnd,E3DM_SET_PARAMF,E3_DEFGLOSSINESS,(DWORD)&m); }
inline float e3_GetDefGlossiness(HWND hwnd){ float f=0;SendMessage(hwnd,E3DM_GET_PARAMF,E3_DEFGLOSSINESS,(DWORD)&f);return f;}

inline BOOL  e3_SetDefSpecLevel(HWND hwnd,float m){ return SendMessage(hwnd,E3DM_SET_PARAMF,E3_DEFSPECLEVEL,(DWORD)&m); }
inline float e3_GetDefSpecLevel(HWND hwnd){ float f=0;SendMessage(hwnd,E3DM_GET_PARAMF,E3_DEFSPECLEVEL,(DWORD)&f);return f;}


inline BOOL  e3_SetSmoothAngle(HWND hwnd,float m){ return SendMessage(hwnd,E3DM_SET_PARAMF,E3_SMOOTHANGLE,(DWORD)&m); }
inline float e3_GetSmoothAngle(HWND hwnd) { float f=0;
                                           SendMessage(hwnd,E3DM_GET_PARAMF,E3_SMOOTHANGLE,(DWORD)&f);
                                           return f;
                                         };
inline BOOL e3_SetZoomFactor(HWND hwnd,float f){ return SendMessage(hwnd,E3DM_SET_PARAMF,E3_ZOOMFACTOR,(DWORD)&f); }
inline float e3_GetZoomFactor(HWND hwnd){ float f=0; SendMessage(hwnd,E3DM_SET_PARAMF,E3_ZOOMFACTOR,(DWORD)&f);  return f;
		 }

inline BOOL e3_SetSmoothing(HWND hwnd,BOOL m){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_SMOOTH,m); }
inline BOOL e3_GetSmoothing(HWND hwnd){ return SendMessage(hwnd,E3DM_GET_PARAMI,E3_SMOOTH,0); }

inline BOOL e3_SetSmoothGroups(HWND hwnd,BOOL m){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_USESMOOTHGROUPS,m); }
inline BOOL e3_GetSmoothGroups(HWND hwnd){ return SendMessage(hwnd,E3DM_GET_PARAMI,E3_USESMOOTHGROUPS,0); }

inline BOOL e3_SetDefaultMaterial(HWND hwnd,BOOL m){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_DEFAULTMATERIAL,m); }
inline BOOL e3_GetDefaultMaterial(HWND hwnd){ return SendMessage(hwnd,E3DM_GET_PARAMI,E3_DEFAULTMATERIAL,0); }

inline BOOL e3_SetTransparency(HWND hwnd,BOOL m){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_TRANSPARENCY,m); }
inline BOOL e3_GetTransparency(HWND hwnd){ return SendMessage(hwnd,E3DM_GET_PARAMI,E3_TRANSPARENCY,0); }

inline BOOL e3_SetTextures(HWND hwnd,BOOL m){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_TEXTURES,m); }
inline BOOL e3_GetTextures(HWND hwnd){ return SendMessage(hwnd,E3DM_GET_PARAMI,E3_TEXTURES,0); }

inline BOOL e3_SetInertialRotation(HWND hwnd,BOOL m){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_ROTATION,m); }
inline BOOL e3_GetInertialRotation(HWND hwnd){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_ROTATION,0); }

inline BOOL e3_SetLeftButton(HWND hwnd,int m){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_LMOUSEMODE,m); }
inline int  e3_GetLeftButton(HWND hwnd){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_LMOUSEMODE,0); }

inline BOOL e3_SetRightButton(HWND hwnd,int m){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_RMOUSEMODE,m); }
inline int  e3_GetRightButton(HWND hwnd){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_RMOUSEMODE,0); }

inline e3_SCENE* e3_GetScene(HWND hwnd){return (e3_SCENE*)SendMessage(hwnd,E3DM_GET_SCENE,0,0);};

inline e3_OBJECT* e3_LoadObject(HWND hwnd,char*file,int flags=0){return (e3_OBJECT*)SendMessage(hwnd,E3DM_LOADOBJECT,flags,(DWORD)file);};

inline BOOL e3_SaveScene(HWND hwnd,char*file,int savemode=0){return SendMessage(hwnd,E3DM_SAVEFILE,savemode,(DWORD)file);};

inline BOOL e3_SetColor(HWND hwnd,int index,COLORREF color){ return SendMessage(hwnd,E3DM_SET_COLOR,index,color); }
inline COLORREF e3_GetColor(HWND hwnd,int index){ return SendMessage(hwnd,E3DM_GET_COLOR,index,0); }

inline BOOL e3_AutoRotate(HWND hwnd,float x,float y,BOOL randomrotate=FALSE)
     {
       float f[2];f[0]=x;f[1]=y;
       return SendMessage(hwnd,E3DM_AUTOROTATE,randomrotate,(LPARAM)&f);
     }
inline BOOL e3_SetBkPicture(HWND hwnd,char*filename){return SendMessage(hwnd,E3DM_SETBKPICTURE,0,(LPARAM)filename);}
inline int e3_GetBkPicture(HWND hwnd,char*filename,int bufLen){ return SendMessage(hwnd,E3DM_GETBKPICTURE,bufLen,(LPARAM)filename);};

inline BOOL e3_SetViewPort(HWND hwnd,int v){return SendMessage(hwnd,E3DM_SETVIEW,v,0);}

inline BOOL e3s_SetTxtFilter(HWND hwnd,BOOL m){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_SYS_TXTFILTER,m); }
inline BOOL e3s_GetTxtFilter(HWND hwnd){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_SYS_TXTFILTER,0); }

inline BOOL e3s_SetTxtResolution(HWND hwnd,int m){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_SYS_TXTRES,m); }
inline int  e3s_GetTxtResolution(HWND hwnd){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_SYS_TXTRES,0); }

inline BOOL e3s_SetGL_Ext(HWND hwnd,BOOL m){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_SYS_GLEXT,m); }
inline BOOL e3s_GetGL_Ext(HWND hwnd){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_SYS_GLEXT,0); }

inline BOOL e3s_SetTxtCorrection(HWND hwnd,BOOL m){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_SYS_TXTPERSPECTIVE,m); }
inline BOOL e3s_GetTxtCorrection(HWND hwnd){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_SYS_TXTPERSPECTIVE,0); }
inline BOOL e3s_SetGLSwapHint(HWND hwnd,BOOL m){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_SYS_GLSWAPHINT,m); }
inline BOOL e3s_GetGLSwapHint(HWND hwnd){ return SendMessage(hwnd,E3DM_SET_PARAMI,E3_SYS_GLSWAPHINT,0); }


/*
 Send WM_COPY to window  - copy current view to clipboard

*/
#ifdef  _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */


#endif
