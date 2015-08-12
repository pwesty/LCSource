/*
Copyright (c) 1999-2000  X Dimension Software, LLC

Project:
            3D Exploration
Module Name:
            e3types.h
Author:
            Alexander Shelemekhov
            alex@xdsoft.com

THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.

http://www.xdsoft.com/explorer
info@xdsoft.com
*/

#ifndef _A1874300_7147_11D4_9F76_F6D558E42026_
#define _A1874300_7147_11D4_9F76_F6D558E42026_

#ifdef  _MSC_VER
/*
 * Currently, all MS C compilers for Win32 platforms default to 8 byte
 * alignment.
 */
#pragma pack(push,1)
#endif  /* _MSC_VER */

#ifndef RGB
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#endif

typedef   signed short   INT16;
typedef   unsigned short UINT16;
#ifdef  _MSC_VER
typedef  unsigned int    UINT32;
typedef    signed int    INT32;
#else
typedef  unsigned long   UINT32;
typedef    signed long   INT32;
#endif
typedef          void*   pointer;

struct QUAT;        //This class provides a compact representation for orientation in three space
struct ANGLEAXIS;   //This class provides a representation for orientation in three space using an angle and axis.
struct POINT4D;     //This class describes a point using float x, y, z and w coordinates.
struct POINT2Di;    //This class describes a 2D point using int x and y coordinates.
struct POINT2D;     //This class describes a 2D point using float x and y coordinates.
struct POINT2Dd;    //This class describes a 2D point using double x and y coordinates.
struct POINT3D;     //This class describes a 3D point using float x, y and z coordinates.
struct MATRIX3D;    //This class implements a 4x4 3D transformation matrix object.
struct ANGLEAXISex; //This class provides a representation for orientation in three space using an angle and ratation axis with origin and direction.
struct TTRI;        //This class describes triangle after triangulation
struct AFFINEPARTS; //This structure provide a way to decompose an MATRIX3D into its translation, rotation, and scale components.
struct BOX3D;       //This class represents a 3D box volume described by two 3D corner coordinates.

/*
  POINT3D struct
*/

struct POINT3D{
 float x,y,z;
 POINT3D(){};

 POINT3D(COLORREF rgb){x=((float)((BYTE)(rgb)))/255.0f;y=((float)((BYTE)(((WORD)(rgb)) >> 8)))/255.0f;z=((float)((BYTE)((rgb)>>16)))/255.0f;};
 POINT3D(float a){x=y=z=a;}
 POINT3D(float _x,float _y,float _z){x=_x;y=_y;z=_z;};

 float& operator[](int i) { return (&x)[i]; }
 const float& operator[](int i) const { return (&x)[i]; }
 operator float*() { return(&x); }

 friend POINT3D operator + (const POINT3D&u,const POINT3D&v){return POINT3D(u.x+v.x,u.y+v.y,u.z+v.z);}
 friend POINT3D operator - (const POINT3D&u,const POINT3D&v){return POINT3D(u.x-v.x,u.y-v.y,u.z-v.z);}
 friend POINT3D operator * (const POINT3D&u,const POINT3D&v){return POINT3D(u.x*v.x,u.y*v.y,u.z*v.z);}
 friend POINT3D operator / (const POINT3D&u,const POINT3D&v){return POINT3D(u.x/v.x,u.y/v.y,u.z/v.z);}
 friend POINT3D operator % (const POINT3D&u,const POINT3D&v) // cross product
           {
            return POINT3D(u.y*v.z-u.z*v.y, u.z*v.x-u.x*v.z, u.x*v.y-u.y*v.x);
           }
 friend float  operator & (const POINT3D&u,const POINT3D&v)// dot product
           {
            return (u.x*v.x+u.y*v.y+u.z*v.z);
           }
 friend POINT3D operator * (const POINT3D&u,float a){return POINT3D(u.x*a,u.y*a,u.z*a);}
 friend POINT3D operator * (float a,const POINT3D&u){return POINT3D(u.x*a,u.y*a,u.z*a);}
 friend POINT3D operator / (const POINT3D&u,float a){return POINT3D(u.x/a,u.y/a,u.z/a);}

 friend POINT3D operator + (const POINT3D&u,float a){return POINT3D(u.x+a,u.y+a,u.z+a);}
 friend POINT3D operator + (float a,const POINT3D&u){return POINT3D(u.x+a,u.y+a,u.z+a);}
 friend POINT3D operator - (const POINT3D&u,float a){return POINT3D(u.x-a,u.y-a,u.z-a);}

 friend BOOL operator == (const POINT3D&u,const POINT3D&v){
        return (u.x==v.x && u.y==v.y && u.z==v.z);
        };
 friend BOOL operator != (const POINT3D&u,const POINT3D&v){
        return (u.x!=v.x || u.y!=v.y || u.z!=v.z);
        };

 inline POINT3D& operator *=(float a){x*=a;y*=a;z*=a;return *this;}

 inline POINT3D& operator +=(float a){x+=a;y+=a;z+=a;return *this;}
 inline POINT3D& operator -=(float a){x-=a;y-=a;z-=a;return *this;}
 inline POINT3D& operator /=(float a){x/=a;y/=a;z/=a;return *this;}
 inline POINT3D& operator +=(const POINT3D&u){x+=u.x;y+=u.y;z+=u.z;return *this;}
 inline POINT3D& operator -=(const POINT3D&u){x-=u.x;y-=u.y;z-=u.z;return *this;}

 inline POINT3D& operator /=(const POINT3D&u){x/=u.x;y/=u.y;z/=u.z;return *this;}
 inline POINT3D& operator *=(const POINT3D&u){x*=u.x;y*=u.y;z*=u.z;return *this;}

 inline POINT3D  operator-() const { return(POINT3D(-x,-y,-z)); }

 void   Limit(float _min=0.0f,float _max=1.0f);
 void   Normalize();
 float  Middle(){return (x+y+z)/3.0f;};
 float  Length();
 float  Max(){return x>y?(x>z?x:z):(y>z?y:z);};
 float  Min(){return x<y?(x<z?x:z):(y<z?y:z);};

 COLORREF rgb(){return RGB(x*255.0,y*255.0,z*255.0);}
 COLORREF bgr(){return RGB(z*255.0,y*255.0,x*255.0);}
};

/*
  POINT2D struct
*/

struct POINT2D{
 float x;
 float y;
 POINT2D(){};
 POINT2D(float a){x=y=a;}
 POINT2D(float _x,float _y){x=_x;y=_y;};
 friend POINT2D operator + (const POINT2D&u,const POINT2D&v){return POINT2D(u.x+v.x,u.y+v.y);}
 friend POINT2D operator - (const POINT2D&u,const POINT2D&v){return POINT2D(u.x-v.x,u.y-v.y);}
 friend POINT2D operator * (const POINT2D&u,const POINT2D&v){return POINT2D(u.x*v.x,u.y*v.y);}
 friend POINT2D operator / (const POINT2D&u,const POINT2D&v){return POINT2D(u.x/v.x,u.y/v.y);}

 friend POINT2D operator * (const POINT2D&u,float a){return POINT2D(u.x*a,u.y*a);}
 friend POINT2D operator * (float a,const POINT2D&u){return POINT2D(u.x*a,u.y*a);}
 friend POINT2D operator / (const POINT2D&u,float a){return POINT2D(u.x/a,u.y/a);}

 friend POINT2D operator + (const POINT2D&u,float a){return POINT2D(u.x+a,u.y+a);}
 friend POINT2D operator + (float a,const POINT2D&u){return POINT2D(u.x+a,u.y+a);}
 friend POINT2D operator - (const POINT2D&u,float a){return POINT2D(u.x-a,u.y-a);}

 friend BOOL operator == (const POINT2D&u,const POINT2D&v){
        return (u.x==v.x && u.y==v.y );
        };
 friend BOOL operator != (const POINT2D&u,const POINT2D&v){
        return (u.x!=v.x || u.y!=v.y );
        };

 inline POINT2D& operator *=(float a){x*=a;y*=a;return *this;}
 inline POINT2D& operator +=(float a){x+=a;y+=a;return *this;}
 inline POINT2D& operator -=(float a){x-=a;y-=a;return *this;}
 inline POINT2D& operator /=(float a){x/=a;y/=a;return *this;}
 inline POINT2D& operator +=(const POINT2D&u){x+=u.x;y+=u.y;return *this;}
 inline POINT2D& operator -=(const POINT2D&u){x-=u.x;y-=u.y;return *this;}
 inline POINT2D& operator *=(const POINT2D&u){x*=u.x;y*=u.y;return *this;}
 inline POINT2D& operator /=(const POINT2D&u){x/=u.x;y/=u.y;return *this;}

 inline POINT2D  operator-() const { return(POINT2D(-x,-y)); }

};
/*
  POINT2Di used in several internal algorithms
*/

struct POINT2Di{
 INT16 x;
 INT16 y;
};

struct POINT2Dd{
  double x, y;
};

struct POINT4D{
 float x,y,z,w;
};
/*
  ANGLEAXIS struct
*/
struct ANGLEAXIS{
	POINT3D axis;
	float   angle;
   ANGLEAXIS(){};
   ANGLEAXIS(const QUAT&);
   ANGLEAXIS(const MATRIX3D&);
   ANGLEAXIS(const POINT3D& _axis,float _angle) {axis=_axis;angle=_angle;}
   ANGLEAXIS(const POINT3D&euler);
};

struct QUAT:public POINT4D{
   QUAT(){x=y=w=0.0f;z=1.0f;};
   QUAT(float _w,float _x,float _y,float _z){w=_w;x=_x;y=_y;z=_z;};
   QUAT(const POINT3D&);      // EulerAngles
   QUAT(const ANGLEAXIS&);
   QUAT(const MATRIX3D&);
   QUAT(                /* output interpolated quaternion to itself*/
	   float alpha,		/* interpolation parameter (0 to 1) */
	   QUAT &a,QUAT &b,		/* start and end unit quaternions */
	   int spin			/* number of extra spin rotations */
      );

friend QUAT operator * (const QUAT&u,const QUAT&v);
friend BOOL operator == (const QUAT&u,const QUAT&v){
        return ((u.x==v.x) && (u.y==v.y) && (u.z==v.z) && (v.w==u.w));
        };
    QUAT operator* (float c) const  { return QUAT(c*w,c*x,c*y,c*z); };
    friend QUAT operator* (float c, const QUAT& q){return QUAT(c*q.w,c*q.x,c*q.y,c*q.z);};
    QUAT operator+ (const QUAT q) const {return QUAT(w+q.w,x+q.x,y+q.y,z+q.z);};
float Length();
void Normalize();
void Invert();
};

// ANGLEAXISex
struct ANGLEAXISex{
 POINT3D Org,Dir; // origin, direction and angle
 float angle;
};
//This structure provides a representation for orientation in three space using
//an origin, angle and axis.

struct TTRI{
 int  p[3];
 BYTE flags;
 TTRI*next;
}; // Polygon Triangulator triangle, used in tesselating polygons

/*
  This struct implements a 4x4 3D transformation matrix object.
*/

struct MATRIX3D{

 double a[4][4];

 MATRIX3D(){};
 MATRIX3D(BOOL b){if(b)IdentityMatrix();};
 MATRIX3D(float*,int size=16);
 MATRIX3D(double*,int size=16);

 double* operator[](int i) { return(a[i]);  }

 void SetTranslate(POINT3D& p);          // makes translation matrix
 void SetRotateX(float angle);           // makes rotation matrix
 void SetRotateY(float angle);
 void SetRotateZ(float angle);
 void SetRotate(POINT3D& r);
 void SetRotate(QUAT& q);
 void SetRotate(ANGLEAXIS& aa);
 void SetRotate(ANGLEAXISex*);
 void SetRotateYPW(float yaw, float pitch, float roll);
 void SetScale(POINT3D& s);              // makes scale matrix
 void SetMul(MATRIX3D&m1,MATRIX3D&m2);
 void Invert();                          // in place
 void SetView(POINT3D& Org,POINT3D &Dir,POINT3D&Up); // Org , Dir , Up - absolute values
 void Set3DSCamera(POINT3D& Direction,float roll);

 void Offset(POINT3D& p);
 inline void Translate(POINT3D& p){Offset(p);};
 void MoveTo(POINT3D& p);
 void RotateX(float angle);
 void RotateY(float angle);
 void RotateZ(float angle);
 void Rotate(POINT3D&);
 void Rotate(ANGLEAXIS&);
 void Rotate(QUAT&);
 void Rotate(ANGLEAXISex*r);

 void Scale(POINT3D& s);
 void Transform(MATRIX3D&);

 void IdentityMatrix();
 BOOL IsIdentity();
 BOOL Equal(MATRIX3D&m, float epsilon = 1E-6f);

 void Decompose(POINT3D&position,POINT3D&rotation,POINT3D&scale,POINT3D&sheap);
 void AffineDecompose(AFFINEPARTS&);

 void GetRotation(POINT3D& r);
 void GetRotation(QUAT& q);
 void GetRotation(ANGLEAXIS& aa);
 POINT3D GetTranslation(){  return POINT3D((float)a[3][0],(float)a[3][1],(float)a[3][2]);}
 POINT3D GetScale();

 POINT3D  PointTransform(POINT3D& p);
 POINT3D  VectorTransform(POINT3D& p);
 void     TransformPoints(POINT3D *array, int n);
 void     TransformPoints(POINT3D *from, POINT3D *to, int n);
 void     TransformVectors(POINT3D*array, int n);
 void     TransformVectors(POINT3D*array, POINT3D*to, int n);

 friend   POINT3D operator * (MATRIX3D&m,POINT3D&p){return m.PointTransform(p);}
 friend   BOOL operator == (MATRIX3D&u,MATRIX3D&v){return u.Equal(v,0.0);};
 friend   BOOL operator != (MATRIX3D&u,MATRIX3D&v){return !u.Equal(v,0.0);};
 MATRIX3D& operator *=(MATRIX3D&a){Transform(a);return *this;};
 friend MATRIX3D operator * (MATRIX3D&m1,MATRIX3D&m2){MATRIX3D m;m.SetMul(m1,m2);return m;}

 // HELPER function
 void SetLocalMatrix(MATRIX3D&parent,MATRIX3D&child);// calculate local matrix, relative to parent
 // M3 = M1*M2
 void SetM2(MATRIX3D&m3,MATRIX3D&m1); // solve M2 matrix
 void SetM1(MATRIX3D&m3,MATRIX3D&m2); // solve M1 matrix

};


struct BOX3D{
POINT3D _min,_max;
BOX3D(){};
BOX3D(POINT3D&p1,POINT3D&p2){_min=p1;_max=p2;};
BOX3D(POINT3D&p,float w){_max=_min=p;_max+=w;_min-=w;};
void    Init(){_min=POINT3D(1e38f);_max=POINT3D(-1e38f);}
void    SetZero(){_min=_max=POINT3D(0.0f);}
BOOL    IsEmpty(){return _max.x<_min.x;}
POINT3D Width(){return ( _max.x>_min.x)?(_max-_min):POINT3D(0.0f);}
POINT3D Center(){return ( _max.x>_min.x)?((_max+_min)/2.0f):POINT3D(0.0f);}

// inline POINT3D& operator *=(float a){x*=a;y*=a;z*=a;return *this;}
inline BOX3D& operator *=(float v){
          POINT3D c=Center();
          POINT3D w=Width()*(v/(2.0f));
          _min=c-w;_max=c+w;
          return *this;
          };
BOX3D& operator +=(const POINT3D&u);
BOX3D& operator +=(const BOX3D&u);

POINT3D operator[](int i) {
               return POINT3D(  (i&1)?_max.x:_min.x,(i&2)?_max.y:_min.y, (i&4)?_max.z:_min.z);
              };
 friend BOOL operator == (const BOX3D&u,const BOX3D&v){
          return (u._min==v._min && v._max==u._max);
        };
 friend BOOL operator != (const BOX3D&u,const BOX3D&v){
          return (u._min!=v._min || v._max!=u._max);
        };
};

struct AFFINEPARTS{
	POINT3D t;	/* Translation components */
	QUAT    q;  /* Essential rotation	  */
	QUAT    u;  /* Stretch rotation	  */
	POINT3D k;	/* Stretch factors	  */
	float f;	   /* Sign of determinant	  */
   AFFINEPARTS(){};
   AFFINEPARTS(MATRIX3D&matrix){matrix.AffineDecompose(*this);};
   BOOL Invert();
};


#ifdef  _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif










