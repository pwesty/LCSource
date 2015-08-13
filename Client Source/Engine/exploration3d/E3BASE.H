/*
Copyright (c) 1999-2000  X Dimension Software

Project:
            3D Exploration
Module Name:
            e3base.h
Author:
            Alexander Shelemekhov
            alex@xdsoft.com

THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.

http://www.xdsoft.com/explorer
info@xdsoft.com

*/

#ifndef _645696C0_7147_11D4_9F76_F6D558E42026_
#define _645696C0_7147_11D4_9F76_F6D558E42026_

#ifndef _15238E60_14A8_11D4_9F76_AC94217A99AA_
#include "e3call.h"
#endif

#ifdef  _MSC_VER
/*
 * Currently, all MS C compilers for Win32 platforms default to 8 byte
 * alignment.
 */
#pragma pack(push,1)
#endif  /* _MSC_VER */

#ifndef _A1874300_7147_11D4_9F76_F6D558E42026_
#include "xdsoft\e3types.h"
#endif

#define E3_MIN_API_VERSION 0x67
#define E3_API_VERSION     0x6E

#ifdef E3INTERNAL
class TSCENE3D;
#endif

E3_INTERFACE e3_interface;
E3_INTERFACE e3_GENERIC;

E3_INTERFACE e3_SCENE;      // Import/Export Interface
E3_INTERFACE e3_API;        // 3D Exploration API
E3_INTERFACE e3_STREAM;     // Stream Interface
E3_INTERFACE e3_MEM;        // Memory Manager Interface
E3_INTERFACE e3_ARRAY2D;    // 2d Array Interface
E3_INTERFACE e3_ARRAY3D;    // 3d Array Interface
E3_INTERFACE e3_PARAM;      // Params block interface
E3_INTERFACE e3_OBJECT;   // Mesh Object Interface
E3_INTERFACE e3_NODE;
E3_INTERFACE e3_LIGHT;      // Light Source
E3_INTERFACE e3_DUMMY;      // Dummy object
E3_INTERFACE e3_CAMERA;
E3_INTERFACE e3_MATERIAL;
E3_INTERFACE e3_COLLECTION;
E3_INTERFACE e3_KEYFRAME;   // Keyframe Animation

struct       e3_FILETYPE;   // FILETYPE structure

E3_INTERFACE e3_interface{
E3_ACCESS
_E3_METHOD(void,    DeleteThis())     // delete this object
_E3_METHOD(BOOL,    CanDeleteThis())
};

typedef BOOL(WINAPI*E3ENUMOBJECTS)(e3_NODE*,DWORD);
// Nodes enumaration function prototype
typedef DWORD(WINAPI*E3ANIMATE3D)(e3_SCENE*,DWORD command,DWORD data,DWORD data2 /*=0*/,float*f /*=NULL*/); //
// Animate function prototype
typedef DWORD(WINAPI*E3IMPORT3D)(e3_STREAM*,e3_SCENE*,DWORD data,DWORD command);
// Import function prototype

E3_INTERFACE e3_COLLECTION:public e3_interface{
E3_ACCESS
E3_METHOD(int,     Count())
E3_METHOD(pointer, At(int index))
E3_METHOD(void, 	 AtDelete(int index))
E3_METHOD(void, 	 AtInsert(int index,pointer item))
E3_METHOD(void, 	 AtPut(int index,pointer item))
E3_METHOD(void, 	 DeleteItem(pointer item))
E3_METHOD(void, 	 DeleteAll())
E3_METHOD(int ,	 IndexOf(pointer item))
E3_METHOD(int ,    Insert(pointer item))
E3_METHOD(void, 	 Swap(int i,int j))
E3_METHOD(BOOL,    IsIndexValid(int Index))
// if you storing classes delivered from e3_GENERIC then you can use:
E3_METHOD(e3_GENERIC*,GetByID(UINT32 id))
E3_METHOD(e3_GENERIC*,GetByName(char*name))
// axess to internal array of pointers
E3_METHOD(pointer*  ,_GetArrayBase())
};

// used by GetThumbnail
struct e3_BMPEXPORT{
 enum e3_FLAGS{
         e_AA=1,        // antialliasing
         e_FILEHEADER=2,  // put bitmap file header in start of memory block
         e_HIDENONOBJECTS=4,
         e_ENABLEQUALITY=8,
         e_EXPORTDIALOG=16,
         e_PROGRESSDIALOG=32,
         e_NOPROGRESSDIALOG=64,
      };
 enum e3_RENDER{
         e_ZBUFFER=0,e_RAYTRACE=1
      };
 DWORD e_size;
 DWORD e_width;
 DWORD e_height;
 DWORD e_flags;
 DWORD e_render; // e_ZBUFFER or E_RAYTRACE
 HGLOBAL e_dib;
 DWORD e_image_size; // memory in bytes used by image
 float e_quality;
};

// struct e3_HITTEST used by GetObjectFromPoint
struct e3_HITTEST{
 POINT2D point;
 DWORD   flags; // 1 - object only, 2 - lights, 4 - cameras
 int     triangle;
 int     quadrangle;
 e3_NODE*object;
};
#define E3_HITOBJECTS 1
#define E3_HITLIGHTS  2
#define E3_HITCAMERAS 4

enum e3_TYPE{E3T_NONE,E3T_OBJECT,E3T_MATERIAL,E3T_LIGHT,E3T_CAMERA,E3T_DUMMY};

// style flags for e3_GENERIC::style
#define E3S_HIDDEN          1  // hidden object
#define E3S_PARENT          2  // have childs , valid after postprocess only
#define E3S_PRIVATE         4  // special object (like grid and axis)
#define E3S_CASTSHADOWS     8
#define E3S_RCVSHADOWS     16
#define E3S_LINKED         32
#define E3S_MARKED         256

#define E3S_CH_OTHER       0x00100000
#define E3S_CH_MAPS        0x08000000
#define E3S_CH_GEOMETRY    0x10000000
#define E3S_CH_MATRIX      0x20000000
#define E3S_CH_NAME        0x40000000
#define E3S_TEMP           0x80000000  // temporal flags
#define E3S_CH_MASK        0x7F000000


enum  e3_PROPERTIES {
 E3P_INFO=1,

 E3P_VISIBLE=100,
 E3P_CASTSHADOWS,
 E3P_RCVSHADOWS,

 E3P_AMBIENT=200,     // POINT3D*
 E3P_DIFFUSE ,        // POINT3D*
 E3P_SPECULAR,        // POINT3D*
 E3P_MFLAGS,           // 4 - double sided,8 - E3P_MODULATEDIFFUSE
 E3P_TEXTUREFILE,        // Texture File Name (char*)
 E3P_XTILE,           //                      (float)
 E3P_YTILE,           // Texture mappint data (float)
 E3P_XORG,            //                      (float)
 E3P_YORG,            // Texture mapping data (float)
 E3P_SHADER,          // shader type (flat,phong,guro,metal,blin) int
 E3P_OPACITY,         // opacity (0 to 1.0 float)
 E3P_AUTOFACET,       // autofacet angle      (float)
 E3P_DIBTEXTURE,         // DIB - data (HGLOBAL)
 E3P_REFLECTION,      // reflection           (float)
 E3P_BUMPFILE,        // (char*)
 E3P_EMISSION,        // POINT3D*
 E3P_MODULATEDIFFUSE, // default 0 (BOOL)
 E3P_ENABLEALPHA,     // Enable Alpha in texture map, Off by default (BOOL)
 E3P_WRAPX,         // WRAP or CLAMP (BOOL)
 E3P_WRAPY,         // WRAP or CLAMP (BOOL)
 E3P_SPECULARLEVEL,   // specular level(float)
 E3P_GLOSSINESS,      // glossiness    (float)
 E3P_SOFTEN,          // while not used(float)
 E3P_DOUBLESIDED,     // material- not used    (BOOL)  object - Set reset flag  F3D_DBLSIDE in each face
 E3P_PHONGEXP,        // set/get phong exponent - glossiness (float)
 E3P_LOCALPATH,        // additional path for textures, (used when you use LoadObject from different folders) (char*)
 E3P_REFRACTION,      // refraction           (float)
 E3P_GLOWCOLOR,       // glow in test now
 E3P_GLOWINNERRADIUS,
 E3P_GLOWOUTRERADIUS,
 E3P_GLOWMIDDLERADIUS,
 E3P_GLOWINNERINTENSITY,


 // e3_OBJECT_PROP
// E3P_DOUBLESIDED=300,
 E3P_SMOOTHANGLE=300,
 E3P_RENDERMODE,
 E3P_OBJMATERIAL,
 E3P_URL,
 E3P_VNORMAL_OK,
 E3P_SMOOTHGROUP_OK,
 E3P_SMOOTHMATERIALS,    // enable/disable smothing between different materialsdisable by default

 // LIGHT PROPS

 E3P_LIGHTCOLOR=400,
 E3P_LIGHTSCALE,
 E3P_INNERRAIDUS,
 E3P_OUTERRADIUS,
 E3P_OUTERANGLE,
 E3P_INNERANGLE,

 // CAMERA PROPS
 E3P_ROLL=500,
 E3P_FOV =501,


 };


E3_INTERFACE e3_GENERIC :public e3_interface{
E3_ACCESS
// base interface for 3d Object, 3d Material and 3d Scene
 char    *name;        // object name
 UINT32   objectID;    // object ID (like name)
 UINT32   style;       // object style
 UINT32   readdata;    // used internaly for several reasons
 UINT16   TYPE;        // type
 BYTE     processed;   // was handled by Post Proces operation
//  < \READ  ONLY >
 DWORD    importdata;  // data for IMPORT scene
 DWORD    exportdata;  // data for EXPoRT scene
 DWORD    htreeitem;   // handle in Objects tree
 char    *info;        // read only
#ifdef E3INTERNAL
 TSCENE3D* scene;
#else
 e3_SCENE* scene;    // current SCENE
#endif

#ifdef E3INTERNAL
 e3_GENERIC();
  void Init(int size,UINT16 _type);
  void NotifyUpdate(UINT32);
#endif
 __E3_METHOD( ~e3_GENERIC())
  _E3_METHOD(BOOL        , SetName(char*))
  _E3_METHOD(char*       , GetName())

  _E3_METHOD(BOOL        , SetParam(int nIndex,DWORD data)) // set integer params
  _E3_METHOD(DWORD       , GetParam(int nIndex,DWORD =0)) // get integer params
  _E3_METHOD(void        , Update(UINT32 updateFlags=0))
  _E3_METHOD(int         , GetDisplayName(char*_name,int iMaxLength ))
  _E3_METHOD(e3_GENERIC *, Clone(UINT32=0))

  inline BOOL  SetParamF(int index,float f)  {return SetParam(index,(DWORD)&f);}
  inline float GetParamF(int index){float f; return GetParam(index,(DWORD)&f)?f:0.0f;}
  inline BOOL  SetParamC(int index,char* str){return SetParam(index,(DWORD)str);}
  inline BOOL  GetParamC(int index,char*str) {return GetParam(index,(DWORD)str);}
  inline void  SetInfo(char*_info){SetParamC(E3P_INFO,_info);}
};

E3_INTERFACE e3_NODE:public e3_GENERIC {
 E3_ACCESS

 e3_NODE*parent;

#if defined(E3INTERNAL) && defined(_MATRIXES_CLASSES_)
 TMatrix2 matrix;
#else
 MATRIX3D  matrix;
 MATRIX3D  inversematrix;
 BOOL      inversematrixflag;               // inverse matrix invalid
#endif
  _E3_METHOD(BOOL    , SetParam(int nIndex,DWORD))
  _E3_METHOD(DWORD   , GetParam(int nIndex,DWORD =0))
 _E3_METHOD(BOOL     ,SetMatrix(MATRIX3D*))
 // set object matrix
 _E3_METHOD(BOOL     ,GetMatrix(MATRIX3D*))
 // retrive object matrix
 _E3_METHOD( void    ,LocalToWorld(POINT3D*src,POINT3D*dst,int count))
 // Convert points from local object coordinate system to world coordinate system
 _E3_METHOD(BOOL     ,Transform(MATRIX3D*m))
// multiply object matrix on m and apply this transformation to each child object
 _E3_METHOD(BOOL     ,GetWorldBoundBox(BOX3D*,BOOL recurse=TRUE))
 _E3_METHOD(BOOL     ,GetLocalBoundBox(BOX3D*,BOOL recurse=TRUE))
 _E3_METHOD(BOOL     ,GetXBoundBox(BOX3D*,MATRIX3D*,BOOL recurse=TRUE))

// Sets a single texture coordinate in the texture coordinates array.
// Show or hide object
 inline void          Show(BOOL show){SetParam(E3P_VISIBLE,show);};
 inline BOOL          Visible(){return GetParam(E3P_VISIBLE);};
 inline void          SetParent(e3_NODE*_parent){parent=_parent;};
 inline e3_NODE*      GetParent(){return parent;};
 inline void          SetCastShadows(BOOL data){SetParam(E3P_CASTSHADOWS,data);}
 inline void          SetRcvShadows(BOOL data){SetParam(E3P_RCVSHADOWS,data);}
 inline BOOL          IsCastShadows(){return GetParam(E3P_CASTSHADOWS);}
 inline BOOL          IsRcvShadows(){return GetParam(E3P_RCVSHADOWS);}

};

enum e3_SCALE{E3_NEAREST,E3_BILINEAR};

#ifdef NOGDI
E3_INTERFACE e3_PICTURE;
#else
E3_INTERFACE e3_PICTURE:public e3_interface{
E3_ACCESS


 DWORD    importdata;  // data for IMPORT scene
 DWORD    exportdata;  // data for EXPORT scene

 char*    filename;     // source file name ( may be NULL )
 FILETIME time;         // source file data ( may be NULL )
 long     filesize;     // source file data ( may be 0 )
 int      file_type;    // type of source file

 HGLOBAL hblock;        // Global Memory Handle
 BITMAPINFOHEADER*Info;
 BYTE *bits;            // bitmap bits
 int W,H;               // Width, Height

 int perline;           // bytes per line
 int ColorCount;
 union{
   COLORREF *palette;
   RGBQUAD*_palette;
 };

 int    refcount;       // reference count
 DWORD  driverdata;     // pointer to driver data
 BYTE   loadtodriver;   // internal data
 int   _dW,_dH;         // driver data

 E3_METHOD(BOOL   ,Save(char*file,int type,float quality=1.0))     // save picture to file
 E3_METHOD(HBITMAP,GetBitmap(BOOL devicedepended=TRUE))
 E3_METHOD(BYTE*  ,GetScanLine(int y))
 E3_METHOD(e3_PICTURE*,Rescale(int w,int h,e3_SCALE type=E3_BILINEAR ))
// E3_METHOD(e3_PICTURE*,Copy(UINT32 flags=0)) // 1 convert to true color

};
#endif

E3_INTERFACE e3_PICTURE_INFO{
E3_ACCESS
  E3_METHOD(BOOL ,SetInfo(char*key,char*info))
};

struct e3_SAVEPIC{
char *fullname;
char *localname;
BOOL  fileexist;
DWORD data;
e3_PICTURE*picture;
e3_SAVEPIC*next;
};

const int PIC_REPLACEDIALOG =1; // show dialog if files alredy exist
const int PIC_ALWAYSDIALOG  =2; // show dialog if textures exist
const int PIC_BMPONLY       =4; // export all files to BMP only
const int PIC_SAVEALL       =8; // save all files
const int PIC_FILE          =16;// path is a main file name
const int PIC_DONOTSAVE     =32;// do not perform saving
const int PIC_SETTOEXPORTDATA =64;// set e3_SAVEPIC to material->GetPicture()->exportdata
const int PIC_POW2SIZE      = 128;// rescale images to 2^n
const int PIC_UNIQUENAME    = 256;// create new file ,if file with such name alredy exist
const int PIC_ORIGINALNAME  = 512;// use file name of original texture file, instead of mask (but mask will use for internal textures)

const int PIC_DEFAULT=PIC_ORIGINALNAME|PIC_UNIQUENAME;

struct e3_SAVE_PICTURES{
e3_COLLECTION*pictures;   // list of pictures to save
e3_MEM       *mem;        // pointer to e3_MEM object
e3_SAVEPIC    *_pictures;  // saved pictures
char         *path;       // path to textures directory
char         *mask;       // file mask for saved bitmaps
int           typescount; // count of supported image types
int          *types;      // image types supported by target system and skiped by converted procedure
int           savetype;   // type of output image type
float         quality;    // quality of output image (for jpeg)
DWORD         flags;      // bit or PIC_XXX flags
int           maxdimension; // PIC_POW2SIZE
char         *defextension; // used with PIC_ORIGINALNAME if you want to change extension of saved files
};

#define E3_BIND_MIN    0
#define E3_BIND_MAX    1
#define E3_BIND_HORZ   2
#define E3_BIND_VERT   3

E3_INTERFACE e3_CAMERA:public e3_NODE{
E3_ACCESS
e3_NODE*cTarget;
// camera origin
E3_METHOD(void, 	SetOrg(POINT3D*,BOOL world=TRUE))
E3_METHOD(void, 	GetOrg(POINT3D*,BOOL world=TRUE))
// direction target location
E3_METHOD(void, 	SetTarget(POINT3D*,BOOL world=TRUE))
E3_METHOD(void, 	GetTarget(POINT3D*,BOOL world=TRUE))
// up location
E3_METHOD(void, 	SetUp(POINT3D*,BOOL world=TRUE))
E3_METHOD(void, 	GetUp(POINT3D*,BOOL world=TRUE))

E3_METHOD(void, 	SetAspect(float))
E3_METHOD(float, 	GetAspect())

E3_METHOD(void, 	SetFOV(float))
E3_METHOD(float, 	GetFOV())

E3_METHOD(void, 	SetFOVBinding(int))
E3_METHOD(int, 	GetFOVBinding())

E3_METHOD(void, 	SetFarNearClip(float _nearclcip,float _farclip))
E3_METHOD(void, 	GetFarNearClip(float&_nearclcip,float&_farclip))

inline  void     SetTargetObject(e3_NODE*_node){cTarget=_node;}
inline  e3_NODE* GetTargetObject(){return cTarget;}


E3_METHOD(void, 	Activate())
};
E3_INTERFACE e3_DUMMY:public e3_NODE{
E3_ACCESS

};

 enum LIGHT_TYPE {LIGHT_LOCAL=0,LIGHT_POINT=LIGHT_LOCAL,  // point light
                  LIGHT_SPOT=1,
                  LIGHT_INFINITE=2,                       // Caligari INFINITE light
                  LIGHT_DIRECTIONAL=3};                   // MAX Directional light source
  enum LIGHT_ATT {
                    ATT_NONE,   // no attenuation
                    ATT_LINEAR, // linear attenuation between lOuterRadius and lInnerRadius
                    ATT_ABC     // use ata,atb,atc
                   };

E3_INTERFACE e3_LIGHT:public e3_NODE{
E3_ACCESS

   LIGHT_TYPE lType;          // - light type
   LIGHT_ATT  lAttenuaton;    // - attenuation
 	POINT3D  lLocalOrg;        // - light source origin in local coordinate space
	POINT3D  lLocalDir;        // - light source target in local coordinate space
 	POINT3D  lOrg;             // - light source origin    in World coordinate space  ( Read Only )
	POINT3D  lDir;             // - light source direction in World coordinate space  ( Read Only )
   /*
     lOrg=lLocalOrg*matrix;
   */
   UINT16   lflags;           // - flags
   POINT3D  lColor;           // - light color
   float    lScale;           // - light scale factor
   float    lInnerRadius;     // - expect LIGHT_INFINITE
   float    lOuterRadius;
   float    lOuterConeAngle;  // - for LIGHT_SPOT , for LIGHT_DIRECTIONAL - Hotspot radius
   float    lInnerConeAngle;  // - for LIGHT_SPOT , for LIGHT_DIRECTIONAL - Falloff radius
   float    ata,atb,atc;      // - attenuation factor
   e3_NODE*lTarget;
   };

// SHADER TYPE FLAG
#define E3_SHADER_FLAT    0
#define E3_SHADER_GURO    1
#define E3_SHADER_PHONG   2
#define E3_SHADER_METAL   3
#define E3_SHADER_BLINN   4


#define E3_MAP_NORMALIZE 1

E3_INTERFACE e3_MATERIAL:public e3_GENERIC{
E3_ACCESS

   E3_METHOD(e3_PICTURE*, GetPicture(int =0)) // get texture
   E3_METHOD(BOOL ,MapTexturePoints(POINT2D*src,POINT2D*dst,DWORD *ref,int count,DWORD flags=E3_MAP_NORMALIZE)) //E3_MAP_NORMALIZE

   inline BOOL   SetTextureFile(char*filename){ return SetParamC(E3P_TEXTUREFILE,filename); };
   inline BOOL   GetTextureFile(char*filename){ return GetParamC(E3P_TEXTUREFILE,filename); }
   inline BOOL   SetBumpFile(char*filename){ return SetParamC(E3P_BUMPFILE,filename); };
   inline BOOL   GetBumpFile(char*filename){ return GetParamC(E3P_BUMPFILE,filename); }
   inline BOOL   SetTexture(HGLOBAL dib){ return SetParam(E3P_DIBTEXTURE,(DWORD)dib); };

   inline BOOL   SetAmbient(POINT3D*point){ return SetParam(E3P_AMBIENT,(DWORD)(point)); };
   inline BOOL   SetDiffuse(POINT3D*point){ return SetParam(E3P_DIFFUSE,(DWORD)(point)); };
   inline BOOL   SetSpecular(POINT3D*point){ return SetParam(E3P_SPECULAR,(DWORD)(point)); };
   inline BOOL   SetEmission(POINT3D*point){ return SetParam(E3P_EMISSION,(DWORD)(point)); };

   inline POINT3D GetAmbient(){ POINT3D p;  GetParam(E3P_AMBIENT,(DWORD)(&p));return p; };
   inline POINT3D GetDiffuse(){ POINT3D p;  GetParam(E3P_DIFFUSE,(DWORD)(&p));return p; };
   inline POINT3D GetSpecular(){ POINT3D p; GetParam(E3P_SPECULAR,(DWORD)(&p));return p; };
   inline POINT3D GetEmission(){ POINT3D p; GetParam(E3P_EMISSION,(DWORD)(&p));return p; };

   inline BOOL   SetAlpha(float op){ return SetParamF(E3P_OPACITY,op); };
   inline float  GetAlpha(){ return GetParamF(E3P_OPACITY); };

   inline BOOL   SetSpecularLevel(float value){ return SetParamF(E3P_SPECULARLEVEL,value); };
   inline float  GetSpecularLevel(){ return GetParamF(E3P_SPECULARLEVEL); };
   inline BOOL   SetGlossiness(float value){ return SetParamF(E3P_GLOSSINESS,value); };
   inline float  GetGlossiness(){ return GetParamF(E3P_GLOSSINESS); };
   inline BOOL   SetPhongExp(float value){ return SetParamF(E3P_PHONGEXP,value); };
   inline float  GetPhongExp(){ return GetParamF(E3P_PHONGEXP); };

   inline BOOL   SetReflection(float value){ return SetParamF(E3P_REFLECTION,value); };
   inline float  GetReflection(){ return GetParamF(E3P_REFLECTION); };

   inline BOOL   SetRefraction(float value){ return SetParamF(E3P_REFRACTION,value); };
   inline float  GetRefraction(){ return GetParamF(E3P_REFRACTION); };

   inline BOOL   SetXTile(float op){ return SetParamF(E3P_XTILE,op); };
   inline float  GetXTile(){ return GetParamF(E3P_XTILE); };
   inline BOOL   SetYTile(float op){ return SetParamF(E3P_YTILE,op); };
   inline float  GetYTile(){ return GetParamF(E3P_YTILE); };

   inline BOOL   SetXOrg(float op){ return SetParamF(E3P_XORG,op); };
   inline float  GetXOrg(){ return GetParamF(E3P_XORG); };
   inline BOOL   SetYOrg(float op){ return SetParamF(E3P_YORG,op); };
   inline float  GetYOrg(){ return GetParamF(E3P_YORG); };

   inline BOOL   SetWrapX(BOOL w){ return SetParam(E3P_WRAPX,w); };
   inline BOOL   GetWrapX(){ return GetParam(E3P_WRAPX); };
   inline BOOL   SetWrapY(BOOL w){ return SetParam(E3P_WRAPY,w); };
   inline BOOL   GetWrapY(){ return GetParam(E3P_WRAPY); };

   inline BOOL   SetShader(int shader){ return SetParam(E3P_SHADER,shader); };
   inline int    GetShader(){ return GetParam(E3P_SHADER); };

   inline BOOL   SetSmoothAngle(float sm){ return SetParamF(E3P_AUTOFACET,sm); };
   inline float  GetSmoothAngle(){ return GetParamF(E3P_AUTOFACET); };

   inline BOOL   SetAlphaInTexture(BOOL enable=1){ return SetParam(E3P_ENABLEALPHA,enable); };

};

// e3_TFACE - single triangular face

struct e3_TFACE{
  WORD       flags; // triangle flags
                    // This member can be any combination of the F3D_XXX values
                    // exept F3D_POLYLINE, F3D_POLYLINE for e3_QFACE only
  e3_MATERIAL*material; // point to material, or NULL for default material
  union{
  DWORD      nums[3]; // old style name
  DWORD      v[3];    // onew style name
  };// These are indices into an object's array of vertices ( points ).
  union{
  DWORD      txtpoints[3];
  DWORD      t[3];
  };//These are indices into the object's txtpoints array.
  inline  void SetEdgeVisFlags(BYTE a,BYTE b,BYTE c)
      {
       flags = (flags&((UINT16)0xfff8u))|(UINT16)((a!=0)|((b!=0)<<1)|((c!=0)<<2));
      };
// helper function for sets the visibility of the all the edges.
  inline  void SetPoints(int a, int b, int c){v[0]=a;v[1]=b;v[2]=c;}
//Sets the vertices of this triangle.  The specified indexes are zero based indices into the object's array of vertices.
  inline  void SetTPoints(int a, int b, int c){t[0]=a;t[1]=b;t[2]=c;}
};

// single QUAD face or POLYLINE depending on flag F3D_POLYLINE

struct e3_QFACE{
  WORD       flags;     // This member can be any combination of the following values
                        //F3D_POLYLINE,F3D_DBLSIDE, F3D_HIDDEN
  e3_MATERIAL*material; // point to material, or NULL for default material
  WORD       count;     // count of verticies in face (only 4 for polygon and any other number for polyine)
  union{
  DWORD     *nums;
  DWORD     *v;
  };// These are 0 based indices into an object's array of vertices ( points ).
  union{
  DWORD     *txtpoints;
  DWORD     *t;
  };//These are indices into the object's txtpoints array.
 // ! You must allocate memory for v and t by call to object->Alloc
 // for example:
 //  e3_QFACE*of=object->_GetFace(i);
 //  of->v=(DWORD*)object->Alloc(sizeof(long)*4);

};

// FACE flags
#define F3D_POLYLINE 1
#define F3D_EDGE1   1
#define F3D_EDGE2   2
#define F3D_EDGE3   4
#define F3D_EDGES   7
#define F3D_DBLSIDE 8
#define F3D_HIDDEN  16      // not used
#define F3D_UPPERFACE 0x20  // reserved
#define F3D_TXTCOORD 0x40

#define F3D_VISIBLE 0x80    // used internally


#define VALID_SMOOTH   1
#define VALID_SPECULAR 2

#define VALID_ALL      0xffff

 enum UV_MAP_TYPE{UV_MAP_PLANAR,UV_MAP_CYLINDRICAL,UV_MAP_SPHERICAL,UV_MAP_BALL,UV_MAP_BOX};
 enum RENDER_MODE{RM_DEFAULT,RM_BOUNDS,RM_TRBOUNDS,RM_TRBOUNDS2,RM_POINTS,RM_WIRED,RM_SOLID,RM_SPRITE};


E3_INTERFACE e3_OBJECT:public e3_NODE{
E3_ACCESS

 DWORD  pointcount;   // Number of vertices
 DWORD  txtcount;     // Number of texture vertices
 DWORD  facecount;    // Number of triangular faces
 DWORD _facecount;    // Number of quadrangular faces or polylines
 POINT3D*points;      // Array of vertex coordinates
 POINT2D*txtpoints;   // Array of texture vertices
 BOX3D   bbox;        // bounding box minimum and maximum coordinates valid after call GetBoundingBox()
 BYTE    subLevel;    // sub Face in OpenFlight
 E3_METHOD(BOOL      ,CreatePoints(DWORD,BOOL keepold=FALSE))
 //Sets the number of geometric vertices in the object
 //Specifies if the previous vertices should be kept.
 //If TRUE the previous vertices are kept; otherwise they are discarded
 E3_METHOD(BOOL      ,CreateTxtPoints(DWORD,BOOL keepold=FALSE))

 E3_METHOD(BOOL      ,CreateFACE3D(DWORD,BOOL keepold=FALSE))
 //Sets the number of triangles.
 E3_METHOD(BOOL     ,_CreateFACE3D(DWORD,BOOL keepold=FALSE))
 //Sets the number of quadrangles or polylines.
 E3_METHOD(e3_TFACE*,GetFace(DWORD))
 // Returns a pointer to the 'i-th' triangle
 E3_METHOD(e3_QFACE*,_GetFace(DWORD))
 // Returns a pointer to the 'i-th' quadrangle or face
 E3_METHOD(pointer    ,Alloc(int size))
 // Alooc size bytes in local object heap
 E3_METHOD(void      ,InvertFaces())
 // Invert all objects faces

 E3_METHOD(BOOL      ,GetBoundingBox())
 // Calculate objects bounding box
 E3_METHOD(void      ,MergePoints(POINT3D*p))
 // remove all duplicated verticies
 E3_METHOD(void      ,ConvertToDBLSIDE())
 E3_METHOD(void      ,InvertFace(int i))
 // Invert single triangle
 E3_METHOD(void      ,_InvertFace(int i))
 // Invert single quadrange
 E3_METHOD(int       ,DelUnUsedPoints())
 // remove all unused verticies
 E3_METHOD(int       ,DelUnUsedTxtPoints())
 // remove all unused texture coordinates
 E3_METHOD(BOOL      ,GetFaceNormal(DWORD iFace,int iVertex,POINT3D*))
 E3_METHOD(BOOL      ,_GetFaceNormal(DWORD iFace,int iVertex,POINT3D*))
 E3_METHOD(BOOL      ,CheckGeometryValidity(UINT32 flags=VALID_ALL))
 E3_METHOD(BOOL      ,GetFaceColor(DWORD iFace,int iVertex,POINT3D*))
 E3_METHOD(BOOL      ,_GetFaceColor(DWORD iFace,int iVertex,POINT3D*))
 E3_METHOD(BOOL      ,GetFaceCoordinate(DWORD iFace,int iVertex,POINT3D*))
 E3_METHOD(BOOL      ,_GetFaceCoordinate(DWORD iFace,int iVertex,POINT3D*))
 E3_METHOD(void      ,SetDefaultTextureCoords()) // set in each face t equal to v
 E3_METHOD(BOOL      ,ApplyUVMap(UV_MAP_TYPE type,float utile, float vtile,MATRIX3D*tm, UINT32 flags=0))
 E3_METHOD(e3_COLLECTION*,GetUsedMaterialsList(BOOL lines=TRUE))
// Inline Function's
 inline void          SetPoint(int i,float x,float y,float z){points[i]=POINT3D(x,y,z);}
// Sets a single vertex in the verts array.
 inline void          SetPoint(int i,POINT3D*p){points[i]=(*p);}
// Sets a single vertex in the verts array.
 inline void          SetTPoint(int i,float u,float v){txtpoints[i].x=u;txtpoints[i].y=v;}
 inline void          SetSmoothAngle(float f){SetParamF(E3P_SMOOTHANGLE,f);}
 inline float         GetSmoothAngle(){{float f; return GetParam(E3P_SMOOTHANGLE,(DWORD)&f)?f:-1.0f;}}
 // set smooth angle for object, this angle overload global scene smooth angle
 inline void          SetSmoothDiffMaterials(BOOL data){SetParam(E3P_SMOOTHMATERIALS,data);}
 inline float         GetSmoothDiffMaterials(){return GetParamF(E3P_SMOOTHMATERIALS);}
 // smothing between faces with different materials

 inline e3_MATERIAL*  GetMaterial(){return (e3_MATERIAL*)GetParam(E3P_OBJMATERIAL);};
 // get object material, may return NULL
 // return first used material
 inline void          SetMaterial(e3_MATERIAL*m){SetParam(E3P_OBJMATERIAL,(DWORD)m);};
 // set material for all object faces
 inline void          SetRenderMode(RENDER_MODE mode){SetParam(E3P_RENDERMODE,mode);}
 inline RENDER_MODE   GetRenderMode(){return (RENDER_MODE)GetParam(E3P_RENDERMODE);}

 inline int           IsVNormalsOk(){return GetParam(E3P_VNORMAL_OK);}
 inline int           IsSmoothGroupsOk(){return GetParam(E3P_SMOOTHGROUP_OK);}
 inline void          MarkDoubleFlags(BOOL _double){SetParam(E3P_DOUBLESIDED,_double);}


};

// update object constants  Update()
#define E3_U_POINTS  1
#define E3_U_SMOOTH  2
#define E3_U_MATRIX  4
#define E3_U_BOUNDS  8
#define E3_U_COLORS  16
#define E3_U_FACES   32 // UpdateFaces
#define E3_U_FACES2  64 // UpdateFaces
#define E3_U_STYLE  128 // style changed,
#define E3_U_MATERIAL 256 // material of face(s) hase been chaged



// update screen constants UpdateScreen(UINT32 );
#define US_RESIZE      1
#define US_CLEAR       2
#define US_REPAINT     2
#define US_PERSPECTIVE 4      // camera changed
#define US_INVALIDATE  8
#define US_MATERIALS   16
#define US_COLORS      32

enum E3P_SCENEPROP{
  E3_SCENEFACECOUNT,
  E3_SCENEFORMATNAME,
  E3_SCENEFACECOUNTADD,
  E3_SCENEASCIIBIN , // 1- ASCII,2 -BIN
  E3_SCENEANIMATEPROC,
  E3_SCENEGRIDPOS,    // // 0 - minz, 1 - maxz, 2 - minx*, 3 - maxx*, 4 - miny,5 - maxy (* while not realized)
  E3_SCENECAMERAPOS,       // POINT3D*  (camera angles XYX - rotation)
  E3_SCENEAMBIENT,         // POINT3D*   (scene ambient light)
  E3_SCENEERRSTR,     // error message ,if scene have not read
  E3_SCENEIMPDLG,
  E3_SCENEEXPDLG,
  E3_SCENEINFO,       // char * to Scene notes
  E3_SCENEINFOADD,    // incremental info
  E3_SCENEDISABLESAVE,     // protect scene from save
  E3_SCENEBKPIC,
  E3_SCENEBKPICFILE,

  E3_SCENEDEFGLOSSINESS,
  E3_SCENEDEFSPECULARLEVEL,
  E3_SCENEDEFREFLECTION,
  E3_SCENEDEFREFRACTION,
  E3_SCENEINFINITEOFFSET,
  E3_SCENESMOOTHANGLE,
  E3_SCENE_WIDTH2D,
  E3_SCENE_HEIGHT2D,

};

// commands for E3ANIMATE3D

#define E3_AGETTYPE         1 // GetAnimation type
#define E3_ASETPOS          2 // Set current position (f)
#define E3_AGETPOS          3 // return active frame  (f)
#define E3_AGETLENGTH       4 // return length        (f)
#define E3_ASETFPS          5 // Get FPS              (f)
#define E3_AGETFPS          6 // Get FPS              (f)

#define E3_AGETSEQCOUNT     7 // return sequence count
#define E3_AGETSEQNAME      8 // copy to (char*)data2 sequence name max[256] ,num -data
#define E3_ASETSEQ          9 // data - set active sequence from 0 to SequenceCount -1
#define E3_AGETACTIVESEQ   10 // return active seuqence

#define E3_AGETINFO        13 // data1 -object, dat2 - e3_STREAM
#define E3_AGETRESTORED    14 // locked
#define E3_ASETRESTORE     15 //

#define E3_AOBJECTDELETING 17 //
#define E3_AGETSTART       20 // return start of animation       (f)

#define E3_ADONE           100 // Destroy animate data

// for E3ANIMATE3D animate constants
// animate flags E3_AGETTYPE
#define E3_A_COMMON   1
#define E3_A_PROGRESS 2
#define E3_A_FRAME    4
#define E3_A_PLAY     8
#define E3_A_SEQUENCE 16
#define E3_A_RELATIVE 32
#define E3_A_RESTORE  64 // can restore original data


// flags for e3_ZOOM structure
#define E3Z_DEFAULT    0
#define E3Z_VISIBLE    1
#define E3Z_HIDDEN     2
#define E3Z_LIGHTS     4
#define E3Z_DEFANGLES  8
#define E3Z_SUBOBJECTS 16
#define E3Z_MOVEGRID   32
#define E3Z_ACTIVECAMERA 64

#define E3Z_V_LEFT     0x100
#define E3Z_V_RIGHT    0x200
#define E3Z_V_TOP      0x300
#define E3Z_V_BOTTOM   0x400
#define E3Z_V_BACK     0x500
#define E3Z_V_FRONT    0x600


 struct e3_ZOOM{
  UINT32  flags;
  e3_NODE *node;
  float   percentage;
  e3_ZOOM(){flags=0;node=0;percentage=1.0;}
 };

E3_INTERFACE e3_SCENE{
E3_ACCESS
// interface used for direct access to the scene.

 DWORD    importdata;  // data for IMPORT scene
 DWORD    exportdata;  // data for EXPoRT scene

 e3_COLLECTION*list_objects;
 e3_COLLECTION*list_lights;
 e3_COLLECTION*list_cameras;
 e3_COLLECTION*list_nodes;
 e3_COLLECTION*list_materials;
 e3_COLLECTION*list_deflights;          // default lights

 E3ANIMATE3D animate;

 E3_METHOD(e3_OBJECT*  ,CreateObject())   // Create New Mesh Object
 E3_METHOD(e3_MATERIAL*,CreateMaterial()) // Create New material
 E3_METHOD(e3_LIGHT*   ,CreateLight())    // Create New Light
 E3_METHOD(e3_CAMERA*  ,CreateCamera())   // Create New Light
 E3_METHOD(e3_DUMMY*   ,CreateDummy())    // Create Dummy Object
 E3_METHOD(e3_CAMERA*  ,GetActiveCamera())


 E3_METHOD(pointer       ,Alloc(int size))   // Alloc size bytes in local scene heap
 E3_METHOD(DWORD         ,SetParam(E3P_SCENEPROP,DWORD)) // set scene parameter
 E3_METHOD(DWORD         ,GetParam(E3P_SCENEPROP,DWORD=0)) // get scene parameter
 E3_METHOD(void          ,UpdateScreen(UINT32 =US_CLEAR|US_INVALIDATE))
 E3_METHOD(e3_OBJECT*    ,CreatePrimitive(int type,e3_PARAM*))
 // create primitive  (sphere,box,cylinder,char ...)
 E3_METHOD(int           ,DeleteUnUsedMaterials()) //in Import Only
 E3_METHOD(int           ,MergeMaterials())    //in Import Only
 E3_METHOD(int           ,AddFile(char*shortName,char*fullName,char*type,DWORD flags)) // add file to list of used files
 // Add file to external file list
 // type - name of file type, or predefined type in loword

 E3_METHOD(BOOL          ,GetBoundingBox(BOX3D*,DWORD flags)) // flags |1 include hidden objects
 E3_METHOD(BOOL          ,GetRescaleMatrix(MATRIX3D*,float extent,DWORD flags)) // flags | 1 move to 0.0.0
 // example usage in Caligari Export

 E3_METHOD(e3_COLLECTION*,GetUsedMaterialsList())
 E3_METHOD(e3_COLLECTION*,GetPicturesList(e3_COLLECTION*materials=0,int flags=0))
 E3_METHOD(BOOL,          StdSavePictures(e3_SAVE_PICTURES*))
 E3_METHOD(e3_KEYFRAME* , GetKeyFrameController(BOOL create=FALSE))
 E3_METHOD(BOOL         , SelectFace(e3_OBJECT*,int index,int _triangle=1))
 E3_METHOD(e3_NODE* ,     GetObjectFromPoint(e3_HITTEST*))  // please do not call this function before first paint
 E3_METHOD(e3_NODE* ,     LoadObject(char*,UINT32 flags,int type=0)) // flags : E3_LOF_XXX

 E3_METHOD(BOOL,          SetCustomData(pointer,UINT32 id,pointer,UINT32 size)) // id < 1000 reserved by system
 E3_METHOD(UINT32,        GetCustomData(pointer,UINT32 id,pointer,UINT32 size)) // id < 1000 reserved by system
 E3_METHOD(BOOL,          DelCustomData(pointer))
 E3_METHOD(BOOL,          GetThumbnailImage(e3_BMPEXPORT*))
 E3_METHOD(BOOL,          EnumObjects(e3_NODE*parent,UINT32 flags,E3ENUMOBJECTS,DWORD value))

 E3_METHOD(BOOL,          SetActive(e3_NODE*))
 E3_METHOD(e3_NODE*,      GetActive())

 E3_METHOD(BOOL,          SetColor(int,COLORREF))  // COLOR_XXX
 E3_METHOD(COLORREF,      GetColor(int))           // COLOR_XXX
 E3_METHOD(BOOL,          Save(char*file,int type,UINT32 flags)) //E3_SAVE
 E3_METHOD(BOOL,          ChangeView(int mode,float a,float b=0))
 E3_METHOD(BOOL,          Zoom(e3_ZOOM*))

 // Project line/face o screen, return 0 if not visible on screen

 E3_METHOD(BOOL,          ProjectLine(POINT3D*,POINT3D*))
 E3_METHOD(int,           ProjectFace(POINT3D*v,POINT3D*c,POINT2D*t,int count)) // 4 maximum
 E3_METHOD(void,          PostProcess())                         // used internally

 E3_METHOD(void,          TimeToString(float ,char*))
 E3_METHOD(BOOL,          StringToTime(char*,float &))
 E3_METHOD(void,          Lock())                      // lock scene repaint
 E3_METHOD(void,          UnLock())                    // unlock scene repaint


 inline  BOOL         ShowImportDialog(){return GetParam(E3_SCENEIMPDLG);}
 inline  BOOL         ShowExportDialog(){return GetParam(E3_SCENEEXPDLG);}
 inline  e3_OBJECT*   GetObjectByName(char*name){return (e3_OBJECT*)list_objects->GetByName(name);}
 inline  e3_MATERIAL* GetMaterialByName(char*name){return (e3_MATERIAL*)list_materials->GetByName(name);}
 inline  DWORD        ObjectCount(){return list_objects->Count();}
 inline  DWORD        MaterialCount(){return list_materials->Count();}
 inline  e3_OBJECT*   GetObject3d(DWORD index){return (e3_OBJECT*)list_objects->At(index);}
 inline  e3_MATERIAL* GetMaterial(DWORD index){return (e3_MATERIAL*)list_materials->At(index);}
 inline  BOOL         SetAmbient(POINT3D*color){return SetParam(E3_SCENEAMBIENT,(DWORD)&color);}
 inline  POINT3D      GetAmbient(){ POINT3D p;  GetParam(E3_SCENEAMBIENT,(DWORD)(&p));return p; };

 inline  BOOL         SetFaceCount(int i){return SetParam(E3_SCENEFACECOUNT,i);}
 inline  BOOL         IncrementFaceCount(int i){return SetParam(E3_SCENEFACECOUNTADD,i);}
 inline  BOOL         SetASCIIMode(int i){return SetParam(E3_SCENEASCIIBIN,i);} // 1- ASCII,2 -BIN
 inline  BOOL         SetFormatName(char *name){return SetParam(E3_SCENEFORMATNAME,(DWORD)name);}
 inline  BOOL         GetFormatName(char *name){return GetParam(E3_SCENEFORMATNAME,(DWORD)name);}

 inline  BOOL         SetGridPos(int i){return SetParam(E3_SCENEGRIDPOS,i);} // 0 - minz, 1 - maxz, 2 - minx*, 3 - maxx*, 4 - miny,5 - maxy (* while not realized)

 inline  BOOL         SetCameraPos(POINT3D*angles){return SetParam(E3_SCENECAMERAPOS,(DWORD)angles);} // default position
 inline  BOOL         SetNotes(char *notes){return SetParam(E3_SCENEINFO,(DWORD)notes);}
 inline  BOOL         AddNotes(char *notes){return SetParam(E3_SCENEINFOADD,(DWORD)notes);}
 inline  BOOL         SetErrorString(char *msg){return SetParam(E3_SCENEERRSTR,(DWORD)msg);}

 inline  BOOL         SetAnimateProc(E3ANIMATE3D AnimateProc){return SetParam(E3_SCENEANIMATEPROC,(DWORD)AnimateProc);}

 inline float         GetSmoothAngle(){{float f; return GetParam(E3_SCENESMOOTHANGLE,(DWORD)&f)?f:-1.0f;}}

// animation valid after loading only
 inline DWORD         AnimateProc(DWORD command,DWORD data,DWORD data2=0,float*f=NULL)
         {
           if(animate)return animate(this,command,data,data2,f);else return 0;
         };
 float  GetTime(){float f;return AnimateProc(E3_AGETPOS,0,0,&f)?f:0;}
 void   SetTime(float f){AnimateProc(E3_ASETPOS,0,0,&f);}
 float  GetAnimLength(){float f;return AnimateProc(E3_AGETLENGTH,0,0,&f)?f:0;}
 //void   SetAnimLength(float f){AnimateProc(E3_ASETLENGTH,0,0,&f);};
 float  GetFPS(){float f;return AnimateProc(E3_AGETFPS,0,0,&f)?f:-1;}
 void   SetFPS(float f){AnimateProc(E3_ASETFPS,0,0,&f);}
 float  GetAnimStart(){float f;return AnimateProc(E3_AGETSTART,0,0,&f)?f:0;}
 float  GetAnimEnd(){return (GetAnimLength()+GetAnimStart());  }
 BOOL   Animatable(){return (animate!=0);}

};

// flags for e3_SCENE::Save
#define E3_SAVE_ALL        0
#define E3_SAVE_VISIBLE    1
#define E3_SAVE_ACTIVE     2
#define E3_SAVE_MARKED     3
#define E3_SAVE_SHOWDIALOG 4
#define E3_SAVE_NODIALOG   8


//  flags for LoadObject
#define E3_LOF_CREATEGROUP        1  // Put all objects in first object in the file
#define E3_LOF_CREATEGROUPDUMMY   2  // Create new dummy object and put all new objects in this object (if more >1 objects was loadedd)
#define E3_LOF_CREATEGROUPDUMMY2  3  // Create new dummy object and put all new objects in this object (if more >=1 objects was loadedd)
#define E3_LOF_ASNEWSCENE         8  // Simulate new scene when loading (only default material and no objects)
#define E3_LOF_NEWDEFMATERIAL     16 // Create new default material
#define E3_LOF_NOPOSTPROCESS      32 // Do not perform postprocess operation

#define E3_ENUM_OBJECT     1
#define E3_ENUM_LIGHT      2
#define E3_ENUM_CAMERA     4
#define E3_ENUM_DUMMY      8

#define E3_ENUM_ALLNODES   0xff
#define E3_ENUM_RECURSE    0x100
#define E3_ENUM_PARENT     0x200  // include parent in enum

// for AddFile (flags)
#define E3_AF_OK           0
#define E3_AF_NOTFOUND     2
#define E3_AF_INVALID      3
#define E3_AF_NOTLOADED    4

// for AddFile (in LOWORD of type)
#define E3_AF_TEXTURE      1
#define E3_AF_MOVIETEXTURE 2
#define E3_AF_SOUND        3
#define E3_AF_INLINE       4
#define E3_AF_PROTOTYPE    5
#define E3_AF_MATERIAL     6
#define E3_AF_MATERIALLIB  7


#define E3_FILTERIMPORT       0x01
#define E3_FILTEREXPORT       0x02
#define E3_FILTERTHUMBNAIL    0x03 // retrive thumbnail for specified file, return HGLOBAL with DIB image
#define E3_FILTERDONE         0x04 // Scene Done
#define E3_FILTEREXPORTDLG    0x05 // before save, return 0 - default, 1 - Ok - pressed, 2 - Cancel
#define E3_FILTERIMPORT2D     0x06 // import 2d image e3_SCENE*=0, return HGLOBAL with dib
#define E3_FILTEREXPORT2D     0x07 // export 2d image e3_SCENE*=0, data - e3_PICTURE* return TRUE on success, if FALSE then file will be deleted
#define E3_FILTEREXPORT2DCAPS 0x08 // return 1 - <256 colors supported, 2 - 256 colors supported, 4 - 32 bit image supported
#define E3_FILTERIMPORTDLG    0x09 // before save, return 0 - default, 1 - Ok - pressed, 2 - Cancel


// Colors  SetColor/GetColor
#define COLOR_BACKGROUND3D 0 //"Scene background"
#define COLOR_BOUNDRECT  1 //"Scene bounded rect"
#define COLOR_WIREFRAME  2 //"Lines in wireframe render mode"
#define COLOR_OBJBOUND   3 // Обрамляющий прямоугольник для объект

#define COLOR_SOBJBOUND  4 // Обрамляющий прямоугольник для объект

#define COLOR_VERTEX     5 // Цвет точе?для отображения только точе

#define COLOR_GRID1L     6
#define COLOR_GRID1D     7
#define COLOR_GRID2L     8
#define COLOR_GRID2D     9
#define COLOR_OUTLINE   10 //"Lines in solid or transparented render mode"
#define COLOR_MDIFFUSE  11 //"Default material diffuse color"
#define COLOR_MAMBIENT  12 //"Default material ambient color"
#define COLOR_MSPECULAR 13 // Default material specular color
#define COLOR_AXISX     14 //Мировы?ос

#define COLOR_AXISY     15 //Мировы?ос

#define COLOR_AXISZ     16 //Мировы?ос

#define COLOR_WIREDGRID 17 //Мировы?ос

#define COLOR_TRBOUNDED 18 //Прозрачный ку

#define COLOR_CROSS     19 //"Vertex cross color"
#define COLOR_FOG       20 //"FOG color"
#define COLOR_MEMISSIVE 21 // Default material emissive color

#define COLOR_COUNT     22

// standart keyframe animation

 enum KEYTYPE{KEY_POSITION=1,KEY_ROTATION=2,KEY_SCALE=3,KEY_MORPH=4};

E3_INTERFACE e3_KEY{
E3_ACCESS
	float time;
	float tens,cont,bias;
	float easeTo,easeFrom;

   DWORD    importdata;  // data for IMPORT scene
   DWORD    exportdata;  // data for EXPoRT scene
#ifdef E3INTERNAL
   void Init();
#endif
   E3_METHOD(void ,GetInfo(e3_STREAM*stream))
};

E3_INTERFACE e3_POSITIONKEY:public e3_KEY{
  E3_ACCESS
  E3_METHOD(BOOL ,SetPosition(POINT3D*))
  E3_METHOD(BOOL ,GetPosition(POINT3D*))
};

E3_INTERFACE  e3_ROTATIONKEY:public e3_KEY{
E3_ACCESS
  E3_METHOD(BOOL ,SetAngleAxis(ANGLEAXIS*))
  E3_METHOD(BOOL ,GetAngleAxis(ANGLEAXIS*))
  E3_METHOD(BOOL ,SetQuaternion(QUAT*))
  E3_METHOD(BOOL ,GetQuaternion(QUAT*))
  E3_METHOD(BOOL ,SetEulerAngles(POINT3D*))
  E3_METHOD(BOOL ,SetEulerAngles2(POINT3D*,int orders))
  E3_METHOD(BOOL ,GetEulerAngles2(POINT3D*,int &orders))
  E3_METHOD(int  ,GetType()) // 0 - ANGLEAXIS,1- QUAT,2 - EULER
  // orders = 0|(1<<2)|(2<<4) XYZ
  // orders = 2|(0<<2)|(1<<4) ZXY

};

E3_INTERFACE  e3_SCALEKEY:public e3_KEY{
  E3_ACCESS
  E3_METHOD(BOOL ,SetScale(POINT3D*))
  E3_METHOD(BOOL ,GetScale(POINT3D*))
};

E3_INTERFACE  e3_MORPHKEY:public e3_KEY{
  E3_ACCESS
  E3_METHOD(BOOL         ,SetTarget(e3_OBJECT*,BOOL copy=TRUE))
  E3_METHOD(e3_OBJECT* ,GetTarget())
  E3_METHOD(BOOL         ,SetPoints(POINT3D*,int count))

};

E3_INTERFACE e3_KEYNODE:public e3_interface{
E3_ACCESS
 DWORD    importdata;                                 // data for IMPORT scene
 DWORD    exportdata;                                 // data for EXPORT scene
 E3_METHOD(void           ,SetPivotPoint(POINT3D*))   // pivot point, like in 3ds
 E3_METHOD(void           ,GetPivotPoint(POINT3D*))
 E3_METHOD(void           ,SetLocalMatrix(MATRIX3D*))
 E3_METHOD(BOOL           ,GetLocalMatrix(MATRIX3D*))
/* LocalMatrix used :
I  NODE_3DS
1. when animation tracks not present, in this case LocalMatrix reprsent
  coordinate system relative to parent node
2. Not all tracks present. In this case first keys of absent tracks can be reconstructed from this matrix
   in this case LocalMatrix will not be used anymore.
*/
 E3_METHOD(e3_ROTATIONKEY*,AddRotationKey(float time))// standard animation keys
 E3_METHOD(e3_POSITIONKEY*,AddPositionKey(float time))
 E3_METHOD(e3_SCALEKEY*   ,AddScaleKey(float time))
 E3_METHOD(e3_MORPHKEY*   ,AddMorphKey(float time))
 E3_METHOD(int            ,GetKeysCount(KEYTYPE))
 E3_METHOD(e3_KEY*        ,GetKey(KEYTYPE,int))
 E3_METHOD(BOOL           ,DeleteKey(KEYTYPE,int))
 E3_METHOD(void           ,GetInfo(e3_STREAM*stream))                          // used internally
 E3_METHOD(e3_NODE*       ,GetObject())                                        // return node associated with keynode
 E3_METHOD(BOOL           ,Transform(float time,MATRIX3D*,BOOL allkeys=FALSE)) // not realized
 E3_METHOD(BOOL           ,SetTrackFlags(KEYTYPE,UINT32))
 E3_METHOD(UINT32         ,GetTrackFlags(KEYTYPE))
 E3_METHOD(BOOL           ,Animatable())                                       // return parent KEYNODE
 E3_METHOD(e3_KEYNODE*    ,GetParent())                                        //
 E3_METHOD(BOOL           ,GetTransform(float time,MATRIX3D*mlocal,MATRIX3D*mchilds))
 // возвращает матриц?трансформаци? для заданног?времен


 //----------------------------------------//
 E3_METHOD(void           ,SetPostMatrix(MATRIX3D*))
 E3_METHOD(BOOL           ,GetPostMatrix(MATRIX3D*))
 // NODE MODE NODE_3DS - default
 E3_METHOD(void           ,SetNodeFlags(UINT32))            //E3_KEYNODE_XXX flags
 E3_METHOD(UINT32         ,GetNodeFlags())
 E3_METHOD(void           ,SetAxisMatrix(MATRIX3D*))  // for Truespace type
 E3_METHOD(BOOL           ,GetAxisMatrix(MATRIX3D*))            // Truespace

};

E3_INTERFACE e3_PARAMKEY:public e3_KEY{
  E3_ACCESS
  E3_METHOD(BOOL ,SetValueP(POINT3D*))
  E3_METHOD(BOOL ,GetValueP(POINT3D*))
  E3_METHOD(BOOL ,SetValueI(int))
  E3_METHOD(int  ,GetValueI())
  E3_METHOD(BOOL ,SetValueF(float))
  E3_METHOD(float,GetValueF())

};

E3_INTERFACE e3_PARAMNODE:public e3_interface{
E3_ACCESS
 DWORD    importdata;                                 // data for IMPORT scene
 DWORD    exportdata;                                 // data for EXPORT scene

 E3_METHOD(e3_NODE*       ,GetObject())
 E3_METHOD(e3_PROPERTIES  ,GetControlProp())
 E3_METHOD(e3_PARAMKEY*   ,AddKey(float time))
 E3_METHOD(int            ,GetKeysCount(KEYTYPE))
 E3_METHOD(e3_PARAMKEY*   ,GetKey(int))
 E3_METHOD(BOOL           ,DeleteKey(int))

 E3_METHOD(BOOL           ,SetTrackFlags(UINT32))
 E3_METHOD(UINT32         ,GetTrackFlags())

 E3_METHOD(BOOL           ,SetEpsilon(float))  // 1e-6 by defaut
 E3_METHOD(float          ,GetEpsilon())

};
// flasg for SetNodeFlags
#define E3_KEYNODE_3DS         0
//#define E3_KEYNODE_TRUESPACE   1 // - not realized yet
#define E3_KEYNODE_CHILDPIVOT  16 // affect pivot point to child (OFF by default: for  3DS, ON - for LW , for example)

//  flags SetTrackFlags
//enum TRACKFLAGS{TR_CYCLIC=1};

#define E3_TRACK_START_CYCLIC   1
#define E3_TRACK_END_CYCLIC     0x10


#define E3_LINEAR               0x00000
#define E3_BEZIER               0x10000
#define E3_TCB                  0x20000

E3_INTERFACE e3_KEYFRAME:public e3_interface{
E3_ACCESS
 enum {BUILDHIERARCHY=1,AUTOLENGTH=2,FILLEMTYTRACKS=4};
 DWORD flags;
 DWORD    importdata;  // data for IMPORT scene
 DWORD    exportdata;  // data for EXPoRT scene
 DWORD trackbar_flags; //
 E3_METHOD(e3_KEYNODE*, AddNode(e3_NODE*))
 E3_METHOD(e3_KEYNODE*, GetNode(int i))
 E3_METHOD(int        ,GetCount())      // nodes count
 E3_METHOD(float      ,GetPos())
 E3_METHOD(UINT32     ,SetPos(float))  // bit flags 1 -mesh changed, 2 - light(s) - changed, 4- camera changed, 8 - current camera changed
 E3_METHOD(BOOL       ,SetLength(float))
 E3_METHOD(BOOL       ,Restore())
 E3_METHOD(void       ,PostProcess(DWORD flags=AUTOLENGTH))           // postprocess init
 E3_METHOD(float      ,GetLength())
 E3_METHOD(BOOL       ,SmartCheck())
 E3_METHOD(e3_KEYNODE*, GetNodeByObject(e3_NODE*))
 E3_METHOD(BOOL       ,AnyAnimatableChild(e3_NODE*))
 E3_METHOD(BOOL       ,GetActualLength(float&_min,float&_max))
 E3_METHOD(void, SetFPS(float))
 E3_METHOD(float,GetFPS())

 E3_METHOD(e3_PARAMNODE*, AddParamNode(e3_NODE*,e3_PROPERTIES))
 E3_METHOD(e3_PARAMNODE*, GetParamNode(int i))
 E3_METHOD(int, GetParamCount())      // param nodes count

 E3_METHOD(BOOL       ,SetStart(float))
 E3_METHOD(float      ,GetStart())

};

#ifdef assert
#undef assert
#endif

#ifdef E3INTERNAL
#define assert( expr ) ( expr || __ASSERT( __FILE__ ,__LINE__,NULL) )
extern BOOL __ASSERT(const char* _file,int _line,const char* txt);
#else
#define assert( expr ) ( expr || E3_API_POINTER->e3_ASSERT( __FILE__ ,__LINE__,NULL) )
#endif

#ifdef  _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif
