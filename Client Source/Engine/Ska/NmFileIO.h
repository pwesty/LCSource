/******************************************************************************
 *  NmFileIO.h -- The interface into the normal map file IO routines.
 ******************************************************************************
 $Header: //depot/3darg/Tools/NormalMapper/NmFileIO.h#7 $
 ******************************************************************************
 *  (C) 2000 ATI Research, Inc.  All rights reserved.
 ******************************************************************************/

//안태훈이 추가한 파일, 윗부분의 Copyright관련 주석 제거 말것.

#ifndef __NMFILEIO__H
#define __NMFILEIO__H

#include <stdio.h>

#pragma pack (push)
#pragma pack (1)   //dont pad the following struct

typedef union
{
   struct { float x, y, z; };
   struct { float v[3]; };
} NmRawPoint;

typedef union
{
   struct { double x, y, z; };
   struct { double v[3]; };
} NmRawPointD;

typedef union
{
	struct
	{
		BOOL use;
		UBYTE iBlend[4];
		UBYTE weight[4];
	};
} NmRawWeights;

union NmRawTexCoord
{
   struct { float u, v; };
   struct { float uv[2]; };
};

struct NmRawTriangle
{
	NmRawTriangle()
	{
		texCoord[0] = texCoord[1] = texCoord[2] = NULL;
	}
	~NmRawTriangle()
	{
		if(texCoord[0] != NULL) delete[] texCoord[0];
		if(texCoord[1] != NULL) delete[] texCoord[1];
		if(texCoord[2] != NULL) delete[] texCoord[2];
	}
	NmRawPoint vert[3];
	NmRawPoint norm[3];
	NmRawWeights weights[3];
	NmRawTexCoord *texCoord[3];
};

// Tangent space structure.
typedef struct
{
   NmRawPoint tangent[3];
   NmRawPoint binormal[3];
} NmRawTangentSpace;
typedef struct
{
   NmRawPointD tangent[3];
   NmRawPointD binormal[3];
} NmRawTangentSpaceD;


// index structure.
typedef struct
{
   int idx[3];
} NmIndex;

// A tangent point for computing smooth tangent space
struct NmTangentPointD
{
	NmTangentPointD()
	{
		u = v = NULL;
	}
	~NmTangentPointD()
	{
		if(u != NULL) delete[] u;
		if(v != NULL) delete[] v;
	}
	double vertex[3];
	double normal[3];
	double *u;
	double *v;
	BOOL useWeight;
	UBYTE iBlend[4];
	UBYTE weights[4];
	double tangent[3];
	double binormal[3];
	int count;
};

#pragma pack (pop)

extern bool NmCreateVertexBuffers (int numTris, NmRawTriangle* tris, 
                                   int* numVerts, NmTangentPointD** verts, NmIndex** indices,
								   int texCoordCnt = 1, int texCoordTan = 0);

#endif // __NMFILEIO__H
