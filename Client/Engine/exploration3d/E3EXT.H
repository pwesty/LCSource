/*
Copyright (c) 1999  X Dimension Software

Project:
            3D Exploration
Module Name:
            e3ext.h
Author:
            Alexander Shelemekhov
            alex@xdsoft.com

THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.

http://www.xdsoft.com/explorer
info@xdsoft.com

*/

#ifndef _0BE81F20_7149_11D4_9F76_F6D558E42026_
#define _0BE81F20_7149_11D4_9F76_F6D558E42026_
#include <windows.h>
/*
  YOU MUST declare e3_API*api; in your source files;
  if name not "api" then you must declare your own name of 3D exploration extension
#define E3_API_POINTER my_own_api
*/
#ifndef E3_API_POINTER
#define E3_API_POINTER api
#endif
#ifdef  _MSC_VER
/*
 * Currently, all MS C compilers for Win32 platforms default to 8 byte
 * alignment.
 */
#pragma pack(push,1)
#endif  /* _MSC_VER */

#ifndef E3INTERNAL
#ifdef  _MSC_VER
#define E3_CALLBACK  __stdcall
#define DllExport
// need DEF file
#else
// Borland C++ , def file not need
#define E3_CALLBACK  extern "C" __export __stdcall
#define DllExport
#endif
#else
#define DllExport
#endif

#include "e3types.h"
#include "e3base.h"

typedef DWORD(WINAPI*E3DLLFUNC)(DWORD,DWORD,DWORD);

// The E3DLLFUNC function is an plugin-defined callback function that processes
// messages sent to a plugin.

typedef DWORD(WINAPI*E3ARCHIVEPROC)(UINT32 code,UINT32 param1,UINT32 param2);
// Archive function prototype

struct e3_FILE3D;
struct e3_PACKFILE;
struct e3_UNPACKFILE3D;
struct e3_FILETYPE;
struct e3_MENU;
struct e3_PRIMITIVE;
struct e3_NOTIFY;

E3_INTERFACE e3_MEM;
E3_INTERFACE e3_ARRAY2D;
E3_INTERFACE e3_ARRAY3D;
E3_INTERFACE e3_PARAM;
E3_INTERFACE e3_EXTENSION;
E3_INTERFACE e3_API;
E3_INTERFACE e3_STREAM;
E3_INTERFACE e3_COLLECTION;
E3_INTERFACE e3_SCENE;
E3_INTERFACE e3_PICTURE;

extern e3_API *E3_API_POINTER;
extern DWORD   e3_language;

// main 3D exploration interface

E3_INTERFACE e3_API{
E3_ACCESS
 HWND    hMain;   // main window descriptor
 E3_METHOD(DWORD  ,Version())   // api version
 E3_METHOD(DWORD  ,RegisterTYPE(e3_FILETYPE*)) // register file type
 E3_METHOD(DWORD  ,RegisterICON(HICON h16_16,HICON h32_32)) // register new icon
 E3_METHOD(void   ,ImportMessage(int style,char*)) // puts string to Import log
 // where style is one of the E3_IM_XXX flags
 E3_METHOD(TTRI*,Triangulate(POINT3D *points,int count,int*nums,int* counts,
                  int ncontours,int flags=0,int *tricount=0))
 // triangulate single polygon or set of polygong to triangle list
 E3_METHOD(e3_MEM*,CreateMEM(int dw=65535u))
 // function create e3_MEM interface
 E3_METHOD(e3_ARRAY2D*,CreateARRAY2D())
 // function create e3_ARRAY2D interface
 E3_METHOD(e3_ARRAY3D*,CreateARRAY3D())
 // function create e3_ARRAY3D interface
 E3_METHOD(e3_STREAM*,CreateStream(HINSTANCE hinst,// handle of the instance that contains the data
                                   LPCTSTR lpszName,	// name or identifier of data
                                   UINT uType	))    // type of stream
// create file, memory or resource stream interface

 E3_METHOD(e3_EXTENSION*,CreateExtension(char*name))
// get 3D Exploration exetnsion
// for example:
// e3_NURBS *  _nobj=(e3_NURBS*)api->CreateExtension("NURBS");
// do something
//  nobj->DeleteThis(); // delete extension interface

 E3_METHOD(e3_PARAM*,CreateParams())
 // function create e3_PARAM interface
 E3_METHOD(BOOL,e3_ASSERT(char* _file,int _line,char*))
 // assertion checking implementation

 E3_METHOD(void   ,FaceNormal(POINT3D*p1,POINT3D*p2,POINT3D*p3,float&a,float&b,float&c))
 // calculate face normal vertex
 E3_METHOD(void   ,FaceABCD(POINT3D*p1,POINT3D*p2,POINT3D*p3,float&a,float&b,float&c,float&d))
 // calculate face a,b,c,d koefficients
 E3_METHOD(BOOL   ,SetWindow(HWND))
 // this function replace 3D View window with other window
 // this window will be destroyed when new file will be loaded
 E3_METHOD(HWND   ,GetWindow(int))
 // function retrives handdle of one of the following 3d Exploration windows
 // [0] - mainwindow - main 3D exploration window
 //  | - [1] -topwindow  - parent of 3d window and 2d window
 //  |    | - [2] - view3d     - OpenGL window
 //  |    | - [3] - view2d     - 2D file view window
 //  |    \ - [X] - objects list [not acessible]
 //  |
 //  | - [X] - history/folders/favorites   [not acessible]
 //  | - [X] - directory files list window [not acessible]
 //  .
 //

// Registry
// This method is called when the plugin needs to store
// or retrive a value into or from system registry.
// There are functions for each type of value to set (UINT32, float, and char).
// Each method has free parameters.  Below is the UINT32 version - the others are similar.
 E3_METHOD(UINT32 ,GetValueI(char*szSubKey,char*szValueName,UINT32 _default=0))
 E3_METHOD(void   ,SetValueI(char*szSubKey,char*szValueName,UINT32 value))
 E3_METHOD(BOOL   ,GetValueC(char*szSubKey,char*szValueName,char*szValueData,int size))
 E3_METHOD(void   ,SetValueC(char*szSubKey,char*szValueName,char*szValueData))
 E3_METHOD(float  ,GetValueF(char*szSubKey,char*szValueName,float _default=0))
 E3_METHOD(void   ,SetValueF(char*szSubKey,char*szValueName,float value))
// szSubKey - plugin name
// szValueName - parameters name
// for example:
// api->GetValueI("STL","ascii",0)
//
 E3_METHOD(void   ,DeleteValue(char*szSubKey,char*szValueName))

 E3_METHOD(void   ,SetProgressValue(float))// from 0 to 1
// This methods allows the plug-in to update the import/export progress indicator.
// when you use standart e3_STREAM - passed to you function and read file only trought
// this stream progress indicator updated automaticly
 E3_METHOD(void   ,SetImportStatusText(int style,char*))
// Allows the plug-in to set the string displayed in status line.
// This style can be one from the E3_STATUS_XXX flags

 E3_METHOD(TTRI*,Triangulate2d(POINT2Dd *points,int* counts,
                  int ncontours,int *tricount=0))
//
 E3_METHOD(BOOL   ,GetSiblingFile(char*mainFile,char*relative,char*full))
// this function provides simple way to determine full filename by any file and
// relative path
// for example
// char _file[MAX_PATH],buf[MAX_PATH];
// stream->FileName(_file,sizeof(_file));
// api->GetSiblingFile(_file,"data.mat",buf);
// e3_STREAM*stream=api->CreateStream(0,buf,E3_STREAM_FILE|E3_READONLY|E3_NOWARNING);

 E3_METHOD(int    ,RegisterCommand(char*command,BYTE fVirt=0, WORD key=0)) // 3D Exploration ONLY
//  function registers a new command. This command can then be used as a valid menu item command.
//  command - maximum 30 chars
//  fVirt - Specifies the accelerator flags.
//  key - Specifies the accelerator key.
 E3_METHOD(BOOL   ,GetDlgItemFloat(HWND hwnd,int id,float&ret,float _min,float _max,BOOL _msg))
// helper function for get float from EDIT control
 E3_METHOD(BOOL   ,RegisterExtension(HINSTANCE,char*name,BOOL regsitry=TRUE))
// register Your extension
 E3_METHOD(DWORD  ,IsRegistered(DWORD))
// Developers may wish to determine , registered status of 3D Exploration
 E3_METHOD(DWORD* ,GetBreakState())
// return pointer to DWORD value that received status of Escape key
 E3_METHOD(e3_COLLECTION*,CreateCollection())

 E3_METHOD(BOOL   ,GetActiveFile(char*lpString,int nMaxCount )) // 3D Exploration ONLY
 //lpString - Points to the buffer that will receive the text.
 //nMaxCount - Specifies the maximum number of characters to copy to the buffer. If the text exceeds this limit, it is truncated.
 E3_METHOD(e3_SCENE*     ,GetActiveScene())                      // 3D Exploration ONLY
 E3_METHOD(e3_PICTURE*   ,GetActivePicture())                   // 3D Exploration ONLY

 E3_METHOD(HGLOBAL       ,LoadImage2d(char*file,int flags,int type=0))// flags & 1- load from memory
 E3_METHOD(BOOL          ,FileDialog(HWND ,char*,char*,UINT32))// flags & 1 - save dialog
 E3_METHOD(BOOL          ,FolderDialog(HWND ,char*folder,char*title))
 E3_METHOD(BOOL          ,PasswordDialog(HWND ,char*title,char*password))

 E3_METHOD(int           ,GetFileType(char*ext))

 E3_METHOD(void          ,MT_SetView(MATRIX3D*m1,POINT3D*,POINT3D*,POINT3D*))
 E3_METHOD(void          ,MT_Set3DSCamera(MATRIX3D*m1,POINT3D*,float f))
 // matrix operations
 E3_METHOD(void   ,MT_Init(MATRIX3D*)) // fill matrix with zeros and diagonal with 1.0f
 E3_METHOD(void   ,MT_Initf(MATRIX3D*,float*,int size))  // fill matrix from
 // float array, size can be 12 or 16
 // For example, do something like:
 //     float f[16];
 //     stream->Read(f,sizeof(f));
 //     MATRIX3D m;
 //     api->MT_Init(&m,f,16);
 //     object->SetMatrix(&m);

 E3_METHOD(void   ,MT_Initd(MATRIX3D*,double*,int size)) // fill matrix from
 // double array, size can be 12 or 16
 E3_METHOD(void   ,MT_Mul(MATRIX3D*in1,MATRIX3D*in2,MATRIX3D*out))
 // mul two matricies and put result to out
 E3_METHOD(void   ,MT_MulPoints(MATRIX3D*,POINT3D*src,POINT3D*dst,int count))
 // transforming points with specified matrix
 E3_METHOD(void   ,MT_MulVectors(MATRIX3D*,POINT3D*src,POINT3D*dst,int count))
 // transforming vectors with specified matrix
 E3_METHOD(void   ,MT_Invert(MATRIX3D*))
 // invert matrix
 E3_METHOD(void   ,MT_MoveTo(MATRIX3D*m,POINT3D*))
 // absolute move matrix to new position
 E3_METHOD(void   ,MT_Offset(MATRIX3D*m,POINT3D*))
 //  relative offset matrix to new position
 E3_METHOD(void   ,MT_Scale(MATRIX3D*m,POINT3D*))
 // sacle matrix
 E3_METHOD(void   ,MT_Rotate(MATRIX3D*m,POINT3D*)) // x,y,z
 // rotate matrix
 E3_METHOD(void   ,MT_RotateR(MATRIX3D*m,ANGLEAXISex*))
 // rotate matrix use rotation centre, axis and angle
// matrix new
 E3_METHOD(void   ,MT_RotateAxis(MATRIX3D*m,ANGLEAXIS*))
// rotate matrix use axis and angle
 E3_METHOD(void    ,MT_QUATToMatrix(MATRIX3D*m,QUAT * quat))
//Convert the specified quaternion to 4x4 rotation matrix.
 E3_METHOD(BOOL    ,MT_Decompose(MATRIX3D*m,POINT3D*position,
                        POINT3D*rotation,
                        POINT3D*scale,
                        POINT3D*sheap=NULL,
                        POINT4D*perspective=NULL
                        ))
 E3_METHOD(BOOL          ,MT_AffineDecompose(MATRIX3D*m, AFFINEPARTS*   ))
// Allows the plug-in to decompose matrix to components
 E3_METHOD(BOOL          ,MT_InvertAffine( AFFINEPARTS*parts, AFFINEPARTS* inverse ))

 E3_METHOD(void          ,MT_Mul2(MATRIX3D*m1,MATRIX3D*m2))
 E3_METHOD(BOOL          ,MT_IsIdent(MATRIX3D*m))
 E3_METHOD(BOOL          ,MT_Equals(MATRIX3D*m1,MATRIX3D*m2,double epsilon=0))
 E3_METHOD(void          ,MT_SetRX(MATRIX3D*m,float))
 E3_METHOD(void          ,MT_SetRY(MATRIX3D*m,float))
 E3_METHOD(void          ,MT_SetRZ(MATRIX3D*m,float))
 E3_METHOD(void          ,MT_SetANGLEAXIS(MATRIX3D*m,ANGLEAXIS*))
 E3_METHOD(void          ,MT_MF1(MATRIX3D*m1,MATRIX3D*m2,MATRIX3D*m3))
 E3_METHOD(void          ,MT_MF2(MATRIX3D*m1,MATRIX3D*m2,MATRIX3D*m3))
 E3_METHOD(void          ,MT_MF3(MATRIX3D*m1,MATRIX3D*m2,MATRIX3D*m3))
 E3_METHOD(void          ,MT_GR1(MATRIX3D*m1,POINT3D*))
 E3_METHOD(void          ,MT_GR2(MATRIX3D*m1,QUAT*))
 E3_METHOD(void          ,MT_GR3(MATRIX3D*m1,ANGLEAXIS*))
 E3_METHOD(void          ,MT_QUATInvert(QUAT*))
 E3_METHOD(void          ,MT_QUATMul(QUAT*,QUAT*,QUAT*))

 E3_METHOD(BOOL          ,ShowHelp(char*topic,char*file=0))
 E3_METHOD(void          ,MT_ANGAXISfromQUAT(ANGLEAXIS*,QUAT*))
 E3_METHOD(void          ,MT_ANGAXISfromMAT(ANGLEAXIS*,MATRIX3D*))
 E3_METHOD(void          ,MT_ANGAXISfromEUL(ANGLEAXIS*,POINT3D*))
 E3_METHOD(void          ,MT_QUATfromEUL(QUAT*,POINT3D*))
 E3_METHOD(void          ,MT_QUATfromANGAXIS(QUAT*,ANGLEAXIS*))
 E3_METHOD(void          ,MT_QUATfromMAT(QUAT*,MATRIX3D*))
 E3_METHOD(void          ,MT_QUATinterpolate(QUAT*c,QUAT*a,QUAT*b,float alpha,int spin))

 E3_METHOD(void          ,MT_POINTlimit(POINT3D* p,float _max,float _min))
 E3_METHOD(void          ,MT_POINTnormalize(POINT3D* p))
 E3_METHOD(float         ,MT_POINTlength(POINT3D* p))
 E3_METHOD(void          ,MT_GS(MATRIX3D*,POINT3D* p))
 E3_METHOD(BOOL          ,GetSpecialFolder(int nFolder,char*szFolder,int nMaxCount ))
 E3_METHOD(BOOL          ,GetPathFromFile(char*szFile,char*szFolder)) // sizeo of dst buffer - MAX_PATH
 E3_METHOD(void          ,AddSlash(char*szFile))

 E3_METHOD(DWORD         ,GetLANG(HINSTANCE))
 E3_METHOD(char*         ,LGetString(DWORD,int))
 E3_METHOD(void          ,LTranslateDialog(DWORD,HWND))
 E3_METHOD(void          ,LTranslateMenu(DWORD,HMENU))

};

#define E3_FOLDER_PLUGINS  0
#define E3_FOLDER_CORE     1

#define E3_ARC_GETLASTERROR  0x100  // return code and lstrcpy((char*)param1,"error description" param2)
#define E3_ARC_OPEN          0x101  // handle of archive must returned
#define E3_ARC_DIR           0x102  // return e3_PACKFILE*
#define E3_ARC_CLOSE         0x103  // close previously opened archive
#define E3_ARC_EXTRACT       0x104  // extract single file e3_UNPACKFILE3D*param2

// flags for e3_FILE3D
#define E3_PF3D_FOLDER       0x0001  // item is a folder

struct e3_FILE3D{  // single item in archive
   char*name;
   UINT32     flags; // flags
   e3_FILE3D* next;  // next file in compressed folder
   e3_FILE3D* items; // contents of child folder
   e3_FILE3D* parent;// filled by 3D Exploration
   UINT32     packsize;
   UINT32     unpacksize;
   FILETIME   time;
   UINT32     data;   // used internally by 3D Exploration
};

struct e3_PACKFILE{ // archive itself
	e3_FILE3D *files;  // contentes of archive
	UINT32     flags;

   UINT32     TotalPacksize;
   UINT32     TotalUnpacksize;
   UINT32     data;   // used internally by 3D Exploration
};


struct e3_UNPACKFILE3D{  // single unpacked item
e3_PACKFILE*pack;
e3_FILE3D*  item;
char     *  dstName;
UINT32      flags;
};

// FILETYPE
struct e3_FILETYPE{
 DWORD e3_size;         // structure size
 DWORD e3_flags;        // Specifies the format style(s).
                        // This member can be any combination of the E3FT_XX values
 char *e3_ext;          // Points to a null-terminated character string that
                        // specifies file extension, for example  "3ds"
 char *e3_name;         // Points to a null-terminated character string that
                        // specifies the ile type name, for example "3D Studio Mesh"
 DWORD e3_icon;         // Identifies the format icon.
                        // This member must be a number returned by RegisterICON
                        // If this member is NULL, then default icon will be used
 DWORD e3_type;         // type number
 union{
 E3IMPORT3D e3_import;  // Pointer to the Import/Export procedure
 E3ARCHIVEPROC  e3_unpack;  // Pointer to the archive handler procedure
 };
 DWORD e3_userdata;     // Specifies the additional user data
};

#define E3FT_IMPORT 1       // Allow IMPORT
#define E3FT_2D     4       // 2D file
#define E3FT_3D     8       // 3D file
#define E3FT_THUMB  16      // file may conatin Thumbnail image
#define E3FT_EXPORT 32      // Allow export to this format
#define E3FT_PACKFILE 512   // Fles like ZIP,ARJ,LZH,RAR ...
#define E3FT_HIDDEN   1024  // Hide in save as and in load dialog
#define E3FT_VIDEO    2048  // Video file

// for archive files you must support :
// return contents of folder
// unpack single file
// unpack whole archive to specified folder

// STREAM interface

E3_INTERFACE e3_STREAM:public e3_interface{
E3_ACCESS
 enum seek_modes {FROM_START=0,FROM_CURRENT=1, FROM_END=2};
 E3_METHOD(DWORD ,Seek(long,seek_modes mode=FROM_START))
// function moves the file pointer of an stream
 E3_METHOD(DWORD ,GetPos())   // the return value is the file pointer
 E3_METHOD(DWORD ,GetSize())  // function retrieves the size, in bytes of stream
 E3_METHOD(DWORD ,GetStatus())
 E3_METHOD(DWORD ,Read(void*,DWORD))
 E3_METHOD(DWORD ,Write(void*,DWORD))
 E3_METHOD(DWORD ,ReadLn(char*,int))
 E3_METHOD(BOOL  ,WriteLn(char*))
 E3_METHOD(BOOL  ,FileName(char*,int))
// funstions alows retrive name of file associated with stream
// this function accepatble for file streams only
 E3_METHOD(BOOL  ,WriteWord(WORD))
 E3_METHOD(WORD  ,ReadWord())
 E3_METHOD(BOOL  ,WriteLong(long))
 E3_METHOD(DWORD ,ReadLong())
 E3_METHOD(BOOL  ,SetBUFFER(int size=32768))
// function set buffer for accelerate ASCII text reading

 E3_METHOD(void  ,_CLOSEFILE()) // close file associated with stream
//  char file[MAX_PATH];
//  if(stream->FileName(file,sizeof(file)))
//  WCHAR _file[MAX_PATH];
//  AtoW(file,_file);
//  HRESULT hResult=StgOpenStorage(_file,NULL,STGM_READ | STGM_SHARE_EXCLUSIVE  ,NULL,0,&storage);

// You can delete stream, created by function CreateStream
 E3_METHOD(DWORD  ,CopyTo(e3_STREAM*dst,int from=0,int size=-1))
// copy size bytes to other stream
 inline int Ok(){return (GetStatus()==0);}
};


// Small and FAST memory allocator.
// All blocks free automatically by destroy e3_MEM

E3_INTERFACE e3_MEM:public e3_interface{
E3_ACCESS
 E3_METHOD(pointer,GetMem(WORD size)) // allocates a block of size bytes from the memory heap
 E3_METHOD(pointer,GetMemZ(WORD size))
 E3_METHOD(char*  ,NewStr(char*))
 E3_METHOD(void   ,ReleaseMem())      // releases a all memory previously allocated from the heap.

 inline   pointer Alloc(WORD size){return GetMem(size);};

};

// e3_ARRAY3D - 3D points array
// e3_ARRAY2D - 2D points array
// used for storing verticies, when it's count don't known or when
// vertex can have same coordinates
// 3D Vertex Array

E3_INTERFACE e3_ARRAY3D:public e3_interface{
E3_ACCESS
 struct list{
 POINT3D point;    // point or vertex
 list*next;        // next item
 };
 E3_METHOD(list*,Root())              // function retrieves first item in a verticies list
 E3_METHOD(POINT3D*,Add(POINT3D*p,int&index)) //function adds an vertex to an verticies list
// E3_METHOD(void ,Delete())          // destroy e3_ARRAY3D object
 E3_METHOD(int  ,GetCount())        // function retrieves the number of verticies in an verticies list
 E3_METHOD(void ,Set(e3_OBJECT*)) // function copies the all verticies to an specified 3d object 3D vertecies list
 E3_METHOD(BOOL ,EnableCompacting(BOOL)) // function enable or disable points compacting algorithm
                                         // If enable all verticies with equal coordinates will be joined in single vertex
                                         // by default compacting enabled
};

// 2D Vertex Array
// most often used to storing texture coordiantes

E3_INTERFACE e3_ARRAY2D:public e3_interface{
E3_ACCESS
 struct list{
  POINT2D point;
  list*next;
 };
 E3_METHOD(list*,Root())            // function retrieves first item in a verticies list
 E3_METHOD(POINT2D*,Add(POINT2D*p,int&index)) //function adds an vertex to an verticies list
 //E3_METHOD(void ,Delete())          // destroy e3_ARRAY3D object
 E3_METHOD(int  ,GetCount())        // function retrieves the number of verticies in an verticies list
 E3_METHOD(void ,Set(e3_OBJECT*)) // function copies the all verticies to an
                                    // specified 3d object texture coordinates list
                                    // call to this function equal to
/*
void SetTxtPoints(e3_OBJECT *object){
  int count=txtdata->GetCount();
  if(count && object->CreateTxtPoints(count)){
   e3_ARRAY2D::list*f=txtdata->Root();
   POINT2D*p=object->txtpoints;
   while(f){
    *p=f->point;
    f=f->next;
    p++;
   }
  }
};
*/
 E3_METHOD(BOOL ,EnableCompacting(BOOL))
};


// Param blocks for 3D Primitives

E3_INTERFACE e3_PARAM:public e3_interface{
E3_ACCESS
// Set Values
//This method is called when the plugin needs to store a value into a variable.
// There are overloaded functions for each type of value to set (int, float, and POINT3D).
//  Each method has two parameters.  Below is the int version - the others are similar.
 E3_METHOD(BOOL ,SetValue(int,int)) // value index and value to set.
 E3_METHOD(BOOL ,SetValue(int,float))
 E3_METHOD(BOOL ,SetValue(int,POINT3D&))
// Get Values
//Whenever the developer needs to retrieve a value from the parameter block,
// the GetValue() method is used.
//There are overloaded functions for each type of value to retrieve
// (int, float, POINT3D).  Each method has three parameters:
 E3_METHOD(BOOL ,GetValue(int,int&,int _default=0 )) // value index, value buffer and default value.
 E3_METHOD(BOOL ,GetValue(int,float&,float _default=0))
 E3_METHOD(BOOL ,GetValue(int,POINT3D&))
// Delete All Params
 E3_METHOD(void ,Clear())   // clear all parameters
 //E3_METHOD(void ,Delete())  // destroy e3_PARAM object

 E3_METHOD(BOOL ,SetValue(int,int size,void*))
 E3_METHOD(int  ,GetValue(int,int size,void*))
};

// context menu structure
// This structure provides access to 3d object and 3d scene context menu
struct e3_MENU{
 HWND          HWindow;
 HMENU         HMenu;
 HMENU         HMenu_Tools;
 e3_GENERIC*   object;
 e3_SCENE*     scene;
 DWORD         addData;
};

// Base Extension interface
E3_INTERFACE e3_EXTENSION:public e3_interface{
E3_ACCESS
 E3_METHOD(char* ,GetName())
 E3_METHOD(int   ,GetVersion())
};

// CreatePrimitive structure
struct e3_PRIMITIVE{
e3_SCENE*scene;
int type;
e3_PARAM*params;
};

struct e3_NOTIFY{
 enum ACTION{LCLICK,RCLICK,MOVE,OBJSELECTED,LUP,LUPMOVE};
 enum ITEM{NONE,TRIANGLE,QUADRANGLE,LINE,VERTEX};
 ACTION action;
 BYTE   lbutton;
 BYTE   rbutton;
 POINT  point;
 HWND   HWindow;
 e3_SCENE*scene;
 e3_GENERIC*object;
 ITEM   itemtype;
 int    index;        // index of face,_face,vertex;
 union{
  e3_TFACE*face;
  e3_QFACE*_face;
 };
};
// implementation of matrix functions

 inline MATRIX3D::MATRIX3D(float*data,int size){E3_API_POINTER->MT_Initf(this,data,size);};
 inline MATRIX3D::MATRIX3D(double*data,int size){E3_API_POINTER->MT_Initd(this,data,size);};
 inline void MATRIX3D::SetRotateX(float angle){E3_API_POINTER->MT_SetRX(this,angle);};
 inline void MATRIX3D::SetRotateY(float angle){E3_API_POINTER->MT_SetRY(this,angle);};
 inline void MATRIX3D::SetRotateZ(float angle){E3_API_POINTER->MT_SetRZ(this,angle);};
 inline void MATRIX3D::SetTranslate(POINT3D& p){IdentityMatrix();E3_API_POINTER->MT_MoveTo(this,&p);};
 inline void MATRIX3D::SetRotate(QUAT& q)  {E3_API_POINTER->MT_QUATToMatrix(this,&q);};
 inline void MATRIX3D::SetRotate(ANGLEAXIS& aa){E3_API_POINTER->MT_SetANGLEAXIS(this,&aa);};
 inline void MATRIX3D::SetRotateYPW(float yaw, float pitch, float roll)
  {
       IdentityMatrix();RotateZ(roll); RotateX(pitch); RotateY(yaw);
  };

 inline BOOL MATRIX3D::Equal(MATRIX3D&m, float epsilon)
  {
   return E3_API_POINTER->MT_Equals(this,&m,epsilon);
  };
 inline void MATRIX3D::SetView(POINT3D& Org,POINT3D &Dir,POINT3D&Up)
  {
   E3_API_POINTER->MT_SetView(this,&Org,&Dir,&Up);
  };
 inline void MATRIX3D::Set3DSCamera(POINT3D& Direction,float roll)
  {
   E3_API_POINTER->MT_Set3DSCamera(this,&Direction,roll);
  };
 inline void MATRIX3D::SetRotate(POINT3D& r)
  {
    IdentityMatrix();
    E3_API_POINTER->MT_Rotate(this,&r);
  };
 inline void MATRIX3D::SetRotate(ANGLEAXISex*r)
  {
    IdentityMatrix();
    E3_API_POINTER->MT_RotateR(this,r);
  };
 inline void MATRIX3D::SetScale(POINT3D& s){IdentityMatrix();E3_API_POINTER->MT_Scale(this,&s);};
 inline void MATRIX3D::Invert(){E3_API_POINTER->MT_Invert(this);};

 inline void MATRIX3D::MoveTo(POINT3D& p){E3_API_POINTER->MT_MoveTo(this,&p);};
 inline void MATRIX3D::Offset(POINT3D& p){E3_API_POINTER->MT_Offset(this,&p);};
 inline void MATRIX3D::RotateX(float angle){E3_API_POINTER->MT_Rotate(this,&POINT3D(angle,0,0));};
 inline void MATRIX3D::RotateY(float angle){E3_API_POINTER->MT_Rotate(this,&POINT3D(0,angle,0));};
 inline void MATRIX3D::RotateZ(float angle){E3_API_POINTER->MT_Rotate(this,&POINT3D(0,0,angle));};
 inline void MATRIX3D::Rotate(ANGLEAXIS& aa){E3_API_POINTER->MT_RotateAxis(this,&aa);};
 inline void MATRIX3D::Rotate(POINT3D&p){E3_API_POINTER->MT_Rotate(this,&p);}
 inline void MATRIX3D::Rotate(QUAT& q)
       { MATRIX3D r;
         E3_API_POINTER->MT_QUATToMatrix(&r,&q);
         E3_API_POINTER->MT_Mul2(this,&r);
       }
 inline void MATRIX3D::Rotate(ANGLEAXISex*r)
  {
    E3_API_POINTER->MT_RotateR(this,r);
  }
 inline void MATRIX3D::Scale(POINT3D& s){E3_API_POINTER->MT_Scale(this,&s);}
 inline void MATRIX3D::IdentityMatrix(){E3_API_POINTER->MT_Init(this);}
 inline BOOL MATRIX3D::IsIdentity(){return E3_API_POINTER->MT_IsIdent(this);}
 inline void MATRIX3D::AffineDecompose(AFFINEPARTS&parts){E3_API_POINTER->MT_AffineDecompose(this, &parts);}

 inline void MATRIX3D::Decompose(POINT3D&position,POINT3D&rotation,POINT3D&scale,POINT3D&sheap)
      {
        E3_API_POINTER->MT_Decompose(this,&position,&rotation,&scale,&sheap);
      };
 inline POINT3D MATRIX3D::PointTransform(POINT3D& p)
   {
     POINT3D _p;
     E3_API_POINTER->MT_MulPoints(this,(POINT3D*)&p,&_p,1);
     return _p;
   }
 inline POINT3D MATRIX3D::VectorTransform(POINT3D& p)
   {
     POINT3D _p;
     E3_API_POINTER->MT_MulVectors(this,(POINT3D*)&p,&_p,1);
     return _p;
   }
 inline void MATRIX3D::TransformPoints(POINT3D *array, int n) { E3_API_POINTER->MT_MulPoints(this,array,array,n); }
 inline void MATRIX3D::TransformPoints(POINT3D *from, POINT3D *to, int n){ E3_API_POINTER->MT_MulPoints(this,from,to,n); }
 inline void MATRIX3D::TransformVectors(POINT3D*array, int n) { E3_API_POINTER->MT_MulVectors(this,array,array,n); }
 inline void MATRIX3D::TransformVectors(POINT3D*from, POINT3D*to, int n) { E3_API_POINTER->MT_MulVectors(this,from,to,n); }
 inline void MATRIX3D::Transform(MATRIX3D&m) { E3_API_POINTER->MT_Mul2(this,&m); }

 inline void MATRIX3D::GetRotation(POINT3D& r){E3_API_POINTER->MT_GR1(this,&r);}
 inline void MATRIX3D::GetRotation(QUAT& q){E3_API_POINTER->MT_GR2(this,&q);}
 inline void MATRIX3D::GetRotation(ANGLEAXIS& aa){E3_API_POINTER->MT_GR3(this,&aa);}
 inline POINT3D MATRIX3D::GetScale(){POINT3D p;E3_API_POINTER->MT_GS(this,&p);return p;}

 // HELPER function M3=M1*M2

 inline void MATRIX3D::SetLocalMatrix(MATRIX3D&parent,MATRIX3D&child)
  {
     E3_API_POINTER->MT_MF1(&child,this,&parent);
  }
 inline void MATRIX3D::SetM1(MATRIX3D&m3,MATRIX3D&m2)
  {
     E3_API_POINTER->MT_MF1(&m3,this,&m2);
  }
 inline void MATRIX3D::SetM2(MATRIX3D&m3,MATRIX3D&m1)
  {
     E3_API_POINTER->MT_MF2(&m3,&m1,this);
  }
 inline void MATRIX3D::SetMul(MATRIX3D&m1,MATRIX3D&m2)
  {
    E3_API_POINTER->MT_Mul(&m1,&m2,this);
  };

 inline void QUAT::Invert()
  {
    E3_API_POINTER->MT_QUATInvert(this);
  };

#ifndef E3INTERNAL
   inline ANGLEAXIS::ANGLEAXIS(const QUAT&q){E3_API_POINTER->MT_ANGAXISfromQUAT(this,(QUAT*)&q);};
   inline ANGLEAXIS::ANGLEAXIS(const MATRIX3D&m){E3_API_POINTER->MT_ANGAXISfromMAT(this,(MATRIX3D*)&m);};
   inline ANGLEAXIS::ANGLEAXIS(const POINT3D&euler){E3_API_POINTER->MT_ANGAXISfromEUL(this,(POINT3D*)&euler);};

   inline QUAT::QUAT(const POINT3D&eul){E3_API_POINTER->MT_QUATfromEUL(this,(POINT3D*)&eul);};
   inline QUAT::QUAT(const ANGLEAXIS&axis){E3_API_POINTER->MT_QUATfromANGAXIS(this,(ANGLEAXIS*)&axis);};
   inline QUAT::QUAT(const MATRIX3D&m){E3_API_POINTER->MT_QUATfromMAT(this,(MATRIX3D*)&m);};
   inline QUAT::QUAT(float alpha, QUAT &a,QUAT &b, int spin ){E3_API_POINTER->MT_QUATinterpolate(this,&a,&b,alpha,spin);};
   inline QUAT operator * (const QUAT&u,const QUAT&v){QUAT q; E3_API_POINTER->MT_QUATMul((QUAT*)&q,(QUAT*)&u,(QUAT*)&v);return q;};

   inline void  POINT3D::Limit(float _min,float _max){E3_API_POINTER->MT_POINTlimit(this,_min,_max);};
   inline void  POINT3D::Normalize(){E3_API_POINTER->MT_POINTnormalize(this);};
   inline float POINT3D::Length(){return E3_API_POINTER->MT_POINTlength(this);};

 inline char*  e3Str(int id){return E3_API_POINTER->LGetString(e3_language,id);}
 inline void   e3TranslateDialog(HWND hwnd){E3_API_POINTER->LTranslateDialog(e3_language,hwnd);}
 inline void   e3TranslateMenu(HMENU hmenu){E3_API_POINTER->LTranslateMenu(e3_language,hmenu);}

#endif
  inline BOOL AFFINEPARTS::Invert(){return E3_API_POINTER->MT_InvertAffine(this,this );};


// SetImportStatusText
#define E3_STATUS_TEXT   0
#define E3_STATUS_FILE   1
#define E3_STATUS_OBJECT 2

// CreateStream types
#define E3_STREAM_FILE     1
#define E3_STREAM_RESOURCE 2
#define E3_STREAM_MEMORY   3
// file stream
#define E3_READONLY       0x100
#define E3_CREATE         0x200
#define E3_READWRITE      0x300
#define E3_NOWARNING      0x800
// memory stream
#define E3_FREEMEMORY     0x100 // if handle ==0
#define E3_ALLOCMEMORY    0x200 // if handle ==0


#define E3_TRIF_NORMAL      0
#define E3_TRIF_LOCALNUMBER 1

// ImportMessage
#define E3_IM_NORMAL  0
#define E3_IM_FAILED  1
#define E3_IM_WARNING 2
#define E3_IM_INFO    3


// for DWORD DLLFunction(DWORD,DWORD,DWORD);
#define E3_INIT         0x0001
// Call when Plugin loaded
// p1 - e3_API ,p2 - hInsance of the plugin
// return value one of the following E3_PLUGIN_XXX values
//
#define E3_PLUGIN_OK     0x01   // all Ok
#define E3_PLUGIN_FAILED 0x296  // unload immediately
#define E3_PLUGIN_UNLOAD 0x297  // do not load in next time

// message code for plugin
#define E3_DONE         0x0002  // Call when Plugin Unloaded
#define E3_ABOUTDIALOG  0x0003  // (HWND)p1 parent window
#define E3_CONFIGDIALOG 0x0005  // (HWND)p1 parent window
#define E3_MENUPOPUP    0x0006  // (e3_MENU*)p1
#define E3_COMMAND      0x0007  // (HWND)p1,(int)p2 - command
#define E3_NOTIFY       0x0008  // (e3_NOTIFY*)p1
#define E3_NEWSCENE     0x0009  // (e3_SCENE*)p1 ,p2=1 if merge
#define E3_SCENEDONE    0x000A  // (e3_SCENE*)p1
#define E3_POSTLOAD      0x000E  // (e3_SCENE*)p1 - scene loaded , but not postprocessed,p2=1 if merge

#define E3_GET_API_VERSION 0x0010 // First Message to Plugin

#define E3_VERSION      0x0020  // p1 point to string
#define E3_INFO         0x0021  // p1 point to string
#define E3_EXTINFO      0x0022  // p1 point to string
#define E3_GETFILETYPE  0x0030  // p1 e3_STREAM

#define E3_ITEMCHANGED   0x0100  // (e3_GENERIC*)p1,p2- E3S_CH_XXXX - flags
#define E3_DELETEITEM    0x0101  // (e3_GENERIC*)p1, not sended when space destroyed
                                 // notification message notifies a that an object/material has been deleted
#define E3_ITEMCREATED   0x0102  // (e3_GENERIC*)p1, not sended when space loading
#define E3_SCENECHANGED  0x0103  // (e3_SCENE*)p1, - you must return TRUE if you want to receive E3_ITEMCHANGED message

#define E3_file_NOTYPE 32000

// internal messages
#define E3_MKPRIMITIVE  0xf001 // used by CreatePrimitive
#define E3_GETEXTENSION 0xf002 // (char*)p1 - extension name

// Additional Window Controls (3D Exploration only)
#define COLORB_GETCOLOR 0x570
// Get Button Color
#define COLORB_SETCOLOR 0x571  // *((POINT3D*)lParam)
// Set color (in lParam)
#define COLORB_COLORCHANGE 0x572
// color was changed
#define COLORB_ISCHANGE 0x572
// Is color changed ?


#ifdef  _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif
