#ifndef SE_INCL_MATRIX12_H
#define SE_INCL_MATRIX12_H
#ifdef PRAGMA_ONCE
	#pragma once
#endif

// asm shortcuts
#define O offset
#define Q qword ptr
#define D dword ptr
#define W  word ptr
#define B  byte ptr

// set given matrix as identity matrix
inline void MakeIdentityMatrix(Matrix12 &mat)
{
	memset(&mat,0,sizeof(mat));
	mat[ 0]  = 1;
	mat[ 5]  = 1;
	mat[10] = 1;
}

// create matrix from vector without rotations
inline void MakeStretchMatrix(Matrix12 &c, const FLOAT3D &v)
{
	c[ 0] = v(1); c[ 1] = 0.0f; c[ 2] = 0.0f; c[ 3] = 0.0f; 
	c[ 4] = 0.0f; c[ 5] = v(2); c[ 6] = 0.0f; c[ 7] = 0.0f; 
	c[ 8] = 0.0f; c[ 9] = 0.0f; c[10] = v(3); c[11] = 0.0f; 
}

// load 3x4 matrix from old-fashioned matrix+vector combination
inline void MatrixVectorToMatrix12(Matrix12 &m12,const FLOATmatrix3D &m, const FLOAT3D &v)
{
	m12[ 0] = m(1,1); m12[ 1] = m(1,2); m12[ 2] = m(1,3); m12[ 3] = v(1); 
	m12[ 4] = m(2,1); m12[ 5] = m(2,2); m12[ 6] = m(2,3); m12[ 7] = v(2); 
	m12[ 8] = m(3,1); m12[ 9] = m(3,2); m12[10] = m(3,3); m12[11] = v(3); 
}

// convert matrix12 to old matrix 3x3 and vector
inline void Matrix12ToMatrixVector(FLOATmatrix3D &c, FLOAT3D &v, const Matrix12 &m12)
{
	c(1,1) = m12[ 0]; c(1,2) = m12[ 1]; c(1,3) = m12[ 2]; v(1) = m12[ 3]; 
	c(2,1) = m12[ 4]; c(2,2) = m12[ 5]; c(2,3) = m12[ 6]; v(2) = m12[ 7]; 
	c(3,1) = m12[ 8]; c(3,2) = m12[ 9]; c(3,3) = m12[10]; v(3) = m12[11]; 
}

// transform vector with given matrix
inline void TransformVector( FLOAT v[3], const Matrix12 &m)
{
	const FLOAT x = v[0];
	const FLOAT y = v[1];
	const FLOAT z = v[2];
	v[0] = m[0]*x + m[1]*y + m[ 2]*z + m[ 3];
	v[1] = m[4]*x + m[5]*y + m[ 6]*z + m[ 7];
	v[2] = m[8]*x + m[9]*y + m[10]*z + m[11];
}

// rotate vector with given matrix ( does not translate vector )
inline void RotateVector( FLOAT v[3], const Matrix12 &m)
{
	FLOAT x = v[0];
	FLOAT y = v[1];
	FLOAT z = v[2];
	v[0] = m[0]*x + m[1]*y + m[ 2]*z;
	v[1] = m[4]*x + m[5]*y + m[ 6]*z;
	v[2] = m[8]*x + m[9]*y + m[10]*z;
}

// copy one matrix12 to another
inline void MatrixCopy(Matrix12 &c, const Matrix12 &m)
{
	c[0] = m[0];  c[1] = m[1];  c[2]  = m[2];   c[3]  = m[3];
	c[4] = m[4];  c[5] = m[5];  c[6]  = m[6];   c[7]  = m[7];
	c[8] = m[8];  c[9] = m[9];  c[10] = m[10];  c[11] = m[11];
}

// make transpose matrix 
inline void MatrixTranspose(Matrix12 &r, const Matrix12 &m)
{
	r[ 0] = m[ 0];
	r[ 5] = m[ 5];
	r[10] = m[10];
	r[ 3] = m[ 3];
	r[ 7] = m[ 7];
	r[11] = m[11];

	r[ 1] = m[ 4];
	r[ 2] = m[ 8];
	r[ 4] = m[ 1];
	r[ 8] = m[ 2];
	r[ 6] = m[ 9];
	r[ 9] = m[ 6];

	r[ 3] = -r[ 0]*m[ 3] - r[ 1]*m[ 7] - r[ 2]*m[11];
	r[ 7] = -r[ 4]*m[ 3] - r[ 5]*m[ 7] - r[ 6]*m[11];
	r[11] = -r[ 8]*m[ 3] - r[ 9]*m[ 7] - r[10]*m[11];
}

//강동민 수정 시작
// NOTE : Matrix12의 헤더에 추가했음.  다른 곳에 선언하는게 더 좋을거 같음.
inline void MatrixMultiply(Matrix16 &c, const Matrix16 &m, const Matrix16 &n)
{
	for(int j = 0; j < 4; ++j)
	{
		for(int i = 0; i < 4; ++i)
		{
			c[i*4 + j] = m[i * 4 + 0] * n[0 * 4 + j] + m[i * 4 + 1] * n[1 * 4 + j] + m[i * 4 + 2] * n[2 * 4 + j] + m[i * 4 + 3] * n[3 * 4 + j];
		}
	}	
}
//강동민 수정 끝

#define ASMOPT 1
inline void MatrixMultiply(Matrix12 &c,const Matrix12 &m, const Matrix12 &n)
{
#if ASMOPT == 1
	__asm {
		mov     esi,D [c]
		mov     edi,D [m]
		mov     edx,D [n]
		fld     D [edi+ 0*4]
		fmul    D [edx+ 0*4] //  1-1/0-0
		fld     D [edi+ 1*4]    
		fmul    D [edx+ 4*4] //  1-2=1-4
		fld     D [edi+ 2*4]    
		fmul    D [edx+ 8*4] //  1-3=2-8;   m[2]*n[8], m[1]*n[4], m[0]*n[0]
		fld     D [edi+ 0*4]    
		fmul    D [edx+ 1*4] //  2-1=0-1
		fld     D [edi+ 1*4]    
		fmul    D [edx+ 5*4] //  2-2=1-5
		fld     D [edi+ 2*4]    
		fmul    D [edx+ 9*4] //  2-3=2-9;   m[2]*n[9], m[1]*n[5], m[0]*n[1], m[2]*n[8], m[1]*n[4], m[0]*n[0]
		fxch    st(5)        //             m[0]*n[0], m[1]*n[5], m[0]*n[1], m[2]*n[8], m[1]*n[4], m[2]*n[9]
		faddp   st(4),st(0)  //             m[1]*n[5], m[0]*n[1], m[2]*n[8], m[1]*n[4]+m[0]*n[0], m[2]*n[9]
		fld     D [edi+ 0*4]    
		fmul    D [edx+ 2*4] //  3-1=0-2
		fld     D [edi+ 1*4]    
		fmul    D [edx+ 6*4] //  3-2=1-6
		fld     D [edi+ 2*4]    
		fmul    D [edx+10*4] //  3-3=2-10;  m[2]*n[10], m[1]*n[6], m[0]*n[2], m[1]*n[5], m[0]*n[1], m[2]*n[8], m[1]*n[4]+m[0]*n[0], m[2]*n[9]
		fxch    st(6)        //             m[1]*n[4]+m[0]*n[0], m[1]*n[6], m[0]*n[2], m[1]*n[5], m[0]*n[1], m[2]*n[8], m[2]*n[10], m[2]*n[9]
		faddp   st(5),st(0)  //             m[1]*n[6], m[0]*n[2], m[1]*n[5], m[0]*n[1], c[0], m[2]*n[10], m[2]*n[9]
		fxch    st(3)        //             m[0]*n[1], m[0]*n[2], m[1]*n[5], m[1]*n[6], c[0], m[2]*n[10], m[2]*n[9]
		faddp   st(2),st(0)  //             m[0]*n[2], m[0]*n[1]+m[1]*n[5], m[1]*n[6], c[0], m[2]*n[10], m[2]*n[9]
		fld     D [edi+ 0*4]                
		fmul    D [edx+ 3*4] //  4-1=0-3    
		fld     D [edi+ 1*4]                
		fmul    D [edx+ 7*4] //  4-2=1-7;   m[1]*n[7], m[0]*n[3], m[0]*n[2], m[0]*n[1]+m[1]*n[5], m[1]*n[6], c[0], m[2]*n[10], m[2]*n[9]
		fxch    st(5)        //             c[0], m[0]*n[3], m[0]*n[2], m[0]*n[1]+m[1]*n[5], m[1]*n[6], m[1]*n[7], m[2]*n[10], m[2]*n[9]
		fstp    D [esi+ 0*4] //             m[0]*n[3], m[0]*n[2], m[0]*n[1]+m[1]*n[5], m[1]*n[6], m[1]*n[7], m[2]*n[10], m[2]*n[9]
		fxch    st(6)        //             m[2]*n[9], m[0]*n[2], m[0]*n[1]+m[1]*n[5], m[1]*n[6], m[1]*n[7], m[2]*n[10], m[0]*n[3]
		faddp   st(2),st(0)  //             m[0]*n[2], c[1], m[1]*n[6], m[1]*n[7], m[2]*n[10], m[0]*n[3]
		faddp   st(2),st(0)  //             c[1], m[0]*n[2]+m[1]*n[6], m[1]*n[7], m[2]*n[10], m[0]*n[3]
		fld     D [edi+ 2*4]                
		fmul    D [edx+11*4] //  4-3=2-11;  m[2]*n[11], c[1], m[0]*n[2]+m[1]*n[6], m[1]*n[7], m[2]*n[10], m[0]*n[3]
		fxch    st(5)        //             m[0]*n[3], c[1], m[0]*n[2]+m[1]*n[6], m[1]*n[7], m[2]*n[10], m[2]*n[11]
		fadd    D [edi+ 3*4] //  4-4=3      m[0]*n[3]+m[3], c[1], m[0]*n[2]+m[1]*n[6], m[1]*n[7], m[2]*n[10], m[2]*n[11]
		fxch    st(2)        //             m[0]*n[2]+m[1]*n[6], c[1], m[0]*n[3]+m[3], m[1]*n[7], m[2]*n[10], m[2]*n[11]
		faddp   st(4),st(0)  //             c[1], m[0]*n[3]+m[3], m[1]*n[7], c[2], m[2]*n[11]
		fstp    D [esi+ 1*4] //             m[0]*n[3]+m[3], m[1]*n[7], c[2], m[2]*n[11]
																				
		fld     D [edi+ 4*4]                
		fmul    D [edx+ 0*4] //  5-1=4-0    
		fld     D [edi+ 5*4]                
		fmul    D [edx+ 4*4] //  5-2=5-4    
		fld     D [edi+ 6*4]                
		fmul    D [edx+ 8*4] //  5-3=6-8;   m[6]*n[8], m[5]*n[4], m[4]*n[0], m[0]*n[3]+m[3], m[1]*n[7], c[2], m[2]*n[11]
		fxch    st(6)        //             m[2]*n[11], m[5]*n[4], m[4]*n[0], m[0]*n[3]+m[3], m[1]*n[7], c[2], m[6]*n[8]
		faddp   st(4),st(0)  //             m[5]*n[4], m[4]*n[0], m[0]*n[3]+m[3], m[1]*n[7]+m[2]*n[11], c[2], m[6]*n[8]
		fxch    st(4)        //             c[2], m[4]*n[0], m[0]*n[3]+m[3], m[1]*n[7]+m[2]*n[11], m[5]*n[4], m[6]*n[8]
		fstp    D [esi+ 2*4] //             m[4]*n[0], m[0]*n[3]+m[3], m[1]*n[7]+m[2]*n[11], m[5]*n[4], m[6]*n[8]
		fld     D [edi+ 4*4]                
		fmul    D [edx+ 1*4] //  6-1=4-1    
		fld     D [edi+ 5*4]                
		fmul    D [edx+ 5*4] //  6-2=5-5    
		fld     D [edi+ 6*4]                
		fmul    D [edx+ 9*4] //  6-3=6-9;   m[6]*n[9], m[5]*n[5], m[4]*n[1], m[4]*n[0], m[0]*n[3]+m[3], m[1]*n[7]+m[2]*n[11], m[5]*n[4], m[6]*n[8]
		fxch    st(4)        //             m[0]*n[3]+m[3], m[5]*n[5], m[4]*n[1], m[4]*n[0], m[6]*n[9], m[1]*n[7]+m[2]*n[11], m[5]*n[4], m[6]*n[8]
		faddp   st(5),st(0)  //             m[5]*n[5], m[4]*n[1], m[4]*n[0], m[6]*n[9], c[3], m[5]*n[4], m[6]*n[8]
		fxch    st(2)        //             m[4]*n[0], m[4]*n[1], m[5]*n[5], m[6]*n[9], c[3], m[5]*n[4], m[6]*n[8]
		faddp   st(5),st(0)  //             m[4]*n[1], m[5]*n[5], m[6]*n[9], c[3], m[4]*n[0]+m[5]*n[4], m[6]*n[8]
		fld     D [edi+ 4*4]                
		fmul    D [edx+ 2*4] //  7-1=4-2    
		fld     D [edi+ 5*4]                
		fmul    D [edx+ 6*4] //  7-2=5-6;   m[4]*n[2], m[5]*n[6], m[4]*n[1], m[5]*n[5], m[6]*n[9], c[3], m[4]*n[0]+m[5]*n[4], m[6]*n[8]
		fxch    st(2)        //             m[4]*n[1], m[5]*n[6], m[4]*n[2], m[5]*n[5], m[6]*n[9], c[3], m[4]*n[0]+m[5]*n[4], m[6]*n[8]
		faddp   st(3),st(0)  //             m[5]*n[6], m[4]*n[2], m[4]*n[1]+m[5]*n[5], m[6]*n[9], c[3], m[4]*n[0]+m[5]*n[4], m[6]*n[8]
		fxch    st(6)        //             m[6]*n[8], m[4]*n[2], m[4]*n[1]+m[5]*n[5], m[6]*n[9], c[3], m[4]*n[0]+m[5]*n[4], m[5]*n[6]
		faddp   st(5),st(0)  //             m[4]*n[2], m[4]*n[1]+m[5]*n[5], m[6]*n[9], c[3], c[4], m[5]*n[6]
		fxch    st(3)        //             c[3], m[4]*n[1]+m[5]*n[5], m[6]*n[9], m[4]*n[2], c[4], m[5]*n[6]
		fstp    D [esi+ 3*4] //             m[4]*n[1]+m[5]*n[5], m[6]*n[9], m[4]*n[2], c[4], m[5]*n[6]
		fld     D [edi+ 6*4]                
		fmul    D [edx+10*4] //  7-3=6-10;  m[6]*n[10], m[4]*n[1]+m[5]*n[5], m[6]*n[9], m[4]*n[2], c[4], m[5]*n[6]
		fxch    st(3)        //             m[4]*n[2], m[4]*n[1]+m[5]*n[5], m[6]*n[9], m[6]*n[10], c[4], m[5]*n[6]
		faddp   st(5),st(0)  //             m[4]*n[1]+m[5]*n[5], m[6]*n[9], m[6]*n[10], c[4], m[4]*n[2]+m[5]*n[6]
		fxch    st(3)        //             c[4], m[6]*n[9], m[6]*n[10], m[4]*n[1]+m[5]*n[5], m[4]*n[2]+m[5]*n[6]
		fstp    D [esi+ 4*4] //             m[6]*n[9], m[6]*n[10], m[4]*n[1]+m[5]*n[5], m[4]*n[2]+m[5]*n[6]
		faddp   st(2),st(0)  //             m[6]*n[10], c[5], m[4]*n[2]+m[5]*n[6]
		fld     D [edi+ 4*4]                
		fmul    D [edx+ 3*4] //  8-1=4-3    
		fld     D [edi+ 5*4]                
		fmul    D [edx+ 7*4] //  8-2=5-7    
		fld     D [edi+ 6*4]                
		fmul    D [edx+11*4] //  8-3=6-11;  m[6]*n[11], m[5]*n[7], m[4]*n[3], m[6]*n[10], c[5], m[4]*n[2]+m[5]*n[6]
		fxch    st(3)        //             m[6]*n[10], m[5]*n[7], m[4]*n[3], m[6]*n[11], c[5], m[4]*n[2]+m[5]*n[6]
		faddp   st(5),st(0)  //             m[5]*n[7], m[4]*n[3], m[6]*n[11], c[5], c[6]
		fxch    st(3)        //             c[5], m[4]*n[3], m[6]*n[11], m[5]*n[7], c[6]
		fstp    D [esi+ 5*4] //             m[4]*n[3], m[6]*n[11], m[5]*n[7], c[6]
																				
		fld     D [edi+ 8*4]                
		fmul    D [edx+ 0*4] //  9-1=8-0    
		fld     D [edi+ 9*4]                
		fmul    D [edx+ 4*4] //  9-2=9-4    
		fld     D [edi+10*4]                
		fmul    D [edx+ 8*4] //  9-3=10-8;  m[10]*n[8], m[9]*n[4], m[8]*n[0], m[4]*n[3], m[6]*n[11], m[5]*n[7], c[6]
		fxch    st(3)        //             m[4]*n[3], m[9]*n[4], m[8]*n[0], m[10]*n[8], m[6]*n[11], m[5]*n[7], c[6]
		faddp   st(5),st(0)  //             m[9]*n[4], m[8]*n[0], m[10]*n[8], m[6]*n[11], m[4]*n[3]+m[5]*n[7], c[6]
		fxch    st(5)        //             c[6], m[8]*n[0], m[10]*n[8], m[6]*n[11], m[4]*n[3]+m[5]*n[7], m[9]*n[4]
		fstp    D [esi+ 6*4] //             m[8]*n[0], m[10]*n[8], m[6]*n[11], m[4]*n[3]+m[5]*n[7], m[9]*n[4]
		fxch    st(2)        //             m[6]*n[11], m[10]*n[8], m[8]*n[0], m[4]*n[3]+m[5]*n[7], m[9]*n[4]
		fadd    D [edi+ 7*4] //  8-4=7      m[6]*n[11]+m[7], m[10]*n[8], m[8]*n[0], m[4]*n[3]+m[5]*n[7], m[9]*n[4]
																				
		fld     D [edi+ 8*4]                
		fmul    D [edx+ 1*4] // 10-1=8-1    
		fld     D [edi+ 9*4]                
		fmul    D [edx+ 5*4] // 10-2=9-5    
		fld     D [edi+10*4]                
		fmul    D [edx+ 9*4] // 10-3=10-9;  m[10]*n[9], m[9]*n[5], m[8]*n[1], m[6]*n[11]+m[7], m[10]*n[8], m[8]*n[0], m[4]*n[3]+m[5]*n[7], m[9]*n[4]
		fxch    st(3)        //             m[6]*n[11]+m[7], m[9]*n[5], m[8]*n[1], m[10]*n[9], m[10]*n[8], m[8]*n[0], m[4]*n[3]+m[5]*n[7], m[9]*n[4]
		faddp   st(6),st(0)  //             m[9]*n[5], m[8]*n[1], m[10]*n[9], m[10]*n[8], m[8]*n[0], c[7], m[9]*n[4]
		fxch    st(4)        //             m[8]*n[0], m[8]*n[1], m[10]*n[9], m[10]*n[8], m[9]*n[5], c[7], m[9]*n[4]
		faddp   st(6),st(0)  //             m[8]*n[1], m[10]*n[9], m[10]*n[8], m[9]*n[5], c[7], m[8]*n[0]+m[9]*n[4]
		fld     D [edi+ 8*4]                
		fmul    D [edx+ 2*4] // 11-1=8-2    
		fld     D [edi+ 9*4]                
		fmul    D [edx+ 6*4] // 11-2=9-6;   m[9]*n[6], m[8]*n[2], m[8]*n[1], m[10]*n[9], m[10]*n[8], m[9]*n[5], c[7], m[8]*n[0]+m[9]*n[4]
		fxch    st(2)        //             m[8]*n[1], m[8]*n[2], m[9]*n[6], m[10]*n[9], m[10]*n[8], m[9]*n[5], c[7], m[8]*n[0]+m[9]*n[4]
		faddp   st(5),st(0)  //             m[8]*n[2], m[9]*n[6], m[10]*n[9], m[10]*n[8], m[8]*n[1]+m[9]*n[5], c[7], m[8]*n[0]+m[9]*n[4]
		fxch    st(5)        //             c[7], m[9]*n[6], m[10]*n[9], m[10]*n[8], m[8]*n[1]+m[9]*n[5], m[8]*n[2], m[8]*n[0]+m[9]*n[4]
		fstp    D [esi+ 7*4] //             m[9]*n[6], m[10]*n[9], m[10]*n[8], m[8]*n[1]+m[9]*n[5], m[8]*n[2], m[8]*n[0]+m[9]*n[4]
		fld     D [edi+10*4]    
		fmul    D [edx+10*4] // 11-3=10-10; m[10]*n[10], m[9]*n[6], m[10]*n[9], m[10]*n[8], m[8]*n[1]+m[9]*n[5], m[8]*n[2], m[8]*n[0]+m[9]*n[4] 
		fxch    st(3)        //             m[10]*n[8], m[9]*n[6], m[10]*n[9], m[10]*n[10], m[8]*n[1]+m[9]*n[5], m[8]*n[2], m[8]*n[0]+m[9]*n[4] 
		faddp   st(6),st(0)  //             m[9]*n[6], m[10]*n[9], m[10]*n[10], m[8]*n[1]+m[9]*n[5], m[8]*n[2], c[8]
		fxch    st(1)        //             m[10]*n[9], m[9]*n[6], m[10]*n[10], m[8]*n[1]+m[9]*n[5], m[8]*n[2], c[8]
		faddp   st(3),st(0)  //             m[9]*n[6], m[10]*n[10], c[9], m[8]*n[2], c[8]
		fld     D [edi+ 8*4]                
		fmul    D [edx+ 3*4] // 12-1=8-3    
		fld     D [edi+ 9*4]                
		fmul    D [edx+ 7*4] // 12-2=9-7    
		fld     D [edi+10*4]                
		fmul    D [edx+11*4] // 12-3=10-11; m[10]*n[11], m[9]*n[7], m[8]*n[3], m[9]*n[6], m[10]*n[10], c[9], m[8]*n[2], c[8]
		fxch    st(3)        //             m[9]*n[6], m[9]*n[7], m[8]*n[3], m[10]*n[11], m[10]*n[10], c[9], m[8]*n[2], c[8]
		faddp   st(6),st(0)  //             m[9]*n[7], m[8]*n[3], m[10]*n[11], m[10]*n[10], c[9], m[8]*n[2]+m[9]*n[6], c[8]
		fxch    st(1)        //             m[8]*n[3], m[9]*n[7], m[10]*n[11], m[10]*n[10], c[9], m[8]*n[2]+m[9]*n[6], c[8]
		fadd    D [edi+11*4] // 12-4=11     m[8]*n[3]+m[11], m[9]*n[7], m[10]*n[11], m[10]*n[10], c[9], m[8]*n[2]+m[9]*n[6], c[8]
		fxch    st(6)        //             c[8], m[9]*n[7], m[10]*n[11], m[10]*n[10], c[9], m[8]*n[2]+m[9]*n[6], m[8]*n[3]+m[11]
		fstp    D [esi+ 8*4] //             m[9]*n[7], m[10]*n[11], m[10]*n[10], c[9], m[8]*n[2]+m[9]*n[6], m[8]*n[3]+m[11]
		fxch    st(2)        //             m[10]*n[10], m[10]*n[11], m[9]*n[7], c[9], m[8]*n[2]+m[9]*n[6], m[8]*n[3]+m[11]
		faddp   st(4),st(0)  //             m[10]*n[11], m[9]*n[7], c[9], c[10], m[8]*n[3]+m[11]
		faddp   st(1),st(0)  //             m[9]*n[7]+m[10]*n[11], c[9], c[10], m[8]*n[3]+m[11]
		fxch    st(1)        //             c[9], m[9]*n[7]+m[10]*n[11], c[10], m[8]*n[3]+m[11]
		fstp    D [esi+ 9*4] //             m[9]*n[7]+m[10]*n[11], c[10], m[8]*n[3]+m[11]
		faddp   st(2),st(0)  //             c[10], c[11]
		fstp    D [esi+10*4]
		fstp    D [esi+11*4]
	}
#else
	c[0]  = m[0]*n[0] + m[1]*n[4] + m[2]*n[ 8];
	c[1]  = m[0]*n[1] + m[1]*n[5] + m[2]*n[ 9];
	c[2]  = m[0]*n[2] + m[1]*n[6] + m[2]*n[10];
	c[3]  = m[0]*n[3] + m[1]*n[7] + m[2]*n[11] + m[3];

	c[4]  = m[4]*n[0] + m[5]*n[4] + m[6]*n[ 8];
	c[5]  = m[4]*n[1] + m[5]*n[5] + m[6]*n[ 9];
	c[6]  = m[4]*n[2] + m[5]*n[6] + m[6]*n[10];
	c[7]  = m[4]*n[3] + m[5]*n[7] + m[6]*n[11] + m[7];

	c[8]  = m[8]*n[0] + m[9]*n[4] + m[10]*n[ 8];
	c[9]  = m[8]*n[1] + m[9]*n[5] + m[10]*n[ 9];
	c[10] = m[8]*n[2] + m[9]*n[6] + m[10]*n[10];
	c[11] = m[8]*n[3] + m[9]*n[7] + m[10]*n[11] + m[11];
#endif
}

// multiply two matrices into first one
inline void MatrixMultiplyCP(Matrix12 &c,const Matrix12 &m, const Matrix12 &n)
{
	Matrix12 mTemp;
	MatrixMultiply(mTemp,m,n);
	MatrixCopy(c,mTemp);
}


// Remove rotation from matrix (make it front face)
inline void RemoveRotationFromMatrix(Matrix12 &mat)
{
	// use just object banking for rotation
	FLOAT fSinP = -mat[ 6];
	FLOAT fCosP = Sqrt(1-fSinP*fSinP);
	FLOAT fSinB, fCosB;
	if( fCosP>0.001f) {
		const FLOAT f1oCosP = 1.0f/fCosP;
		fSinB = mat[ 4]*f1oCosP;
		fCosB = mat[ 5]*f1oCosP;
	} else {
		fSinB = 0.0f;
		fCosB = 1.0f;
	}
	mat[ 0] = +fCosB; mat[ 1] = -fSinB; mat[ 2] = 0;
	mat[ 4] = +fSinB; mat[ 5] = +fCosB; mat[ 6] = 0;
	mat[ 8] =      0; mat[ 9] =      0; mat[10] = 1;
}


#endif  /* include-once check. */
