/******************************************************************************
*  NmFileIO.cpp -- File IO routines for normal mapper.
******************************************************************************
$Header: //depot/3darg/Tools/NormalMapper/NmFileIO.cpp#15 $
******************************************************************************
*  (C) 2000 ATI Research, Inc.  All rights reserved.
******************************************************************************/

//안태훈이 추가한 파일, 윗부분의 Copyright관련 주석 제거 말것.

//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
#include <stdH.h>
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
#include <stdarg.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>

#include "NmFileIO.h"
#include <Engine/Util.h>

#define EPSILON 1.0e-5
static const double ATI_VERT_TINY = EPSILON;
static const double ATI_NORM_TINY = EPSILON;
static const double ATI_TEX_TINY = EPSILON;
static const double ATI_WEIGHT_TINY = EPSILON;
static const double ATI_TAN_ANGLE = 0.3;


//==========================================================================
// Normalize a vector
//==========================================================================
static inline void
NormalizeD (double v[3])
{
	double len = sqrt((v[0]*v[0])+(v[1]*v[1])+(v[2]*v[2]));
	if (len < EPSILON)
	{
		v[0] = 1.0;
		v[1] = 0.0;
		v[2] = 0.0;
	}
	else
	{
		v[0] = v[0]/len;
		v[1] = v[1]/len;
		v[2] = v[2]/len;
	}
}

//==========================================================================
// Calculates the dot product of two vectors.
//==========================================================================
static inline double
DotProduct3D (double vectorA[3], double vectorB[3])
{
	return( (vectorA[0] * vectorB[0]) + (vectorA[1] * vectorB[1]) +
		(vectorA[2] * vectorB[2]) );
} // end of DotProduct

//=============================================================================
// Compare the values
//=============================================================================
static int
NmCompareD (NmTangentPointD* v0, NmTangentPointD* v1, int texCoordCnt, int texCoordTan)
{
	if (fabs(v0->vertex[0] - v1->vertex[0]) > ATI_VERT_TINY)
	{
		if (v0->vertex[0] > v1->vertex[0]) return -1;
		else return 1;
	}
	if (fabs(v0->vertex[1] - v1->vertex[1]) > ATI_VERT_TINY)
	{
		if (v0->vertex[1] > v1->vertex[1]) return -1;
		else return 1;
	}
	if (fabs(v0->vertex[2] - v1->vertex[2]) > ATI_VERT_TINY)
	{
		if (v0->vertex[2] > v1->vertex[2]) return -1;
		else return 1;
	}
	
	if (fabs(v0->normal[0] - v1->normal[0]) > ATI_NORM_TINY)
	{
		if (v0->normal[0] > v1->normal[0]) return -1;
		else return 1;
	}
	if (fabs(v0->normal[1] - v1->normal[1]) > ATI_NORM_TINY)
	{
		if (v0->normal[1] > v1->normal[1]) return -1;
		else return 1;
	}
	if (fabs(v0->normal[2] - v1->normal[2]) > ATI_NORM_TINY)
	{
		if (v0->normal[2] > v1->normal[2]) return -1;
		else return 1;
	}
	
	if(v0->u != NULL && v0->v != NULL && v1->u != NULL && v1->v != NULL && texCoordCnt > 0 && texCoordTan >= 0 && texCoordTan < texCoordCnt)
	{
		int iUV = 0;
		for(iUV=0; iUV<texCoordCnt; ++iUV)
		{
			if (fabs(v0->u[iUV] - v1->u[iUV]) > ATI_TEX_TINY)
			{
				if (v0->u[iUV] > v1->u[iUV]) return -1;
				else return 1;
			}
			if (fabs(v0->v[iUV] - v1->v[iUV]) > ATI_TEX_TINY)
			{
				if (v0->v[iUV] > v1->v[iUV]) return -1;
				else return 1;
			}
		}
	}

	if(v0->useWeight)
	{
		if( v0->iBlend[0] != v1->iBlend[0] )
		{
			if( v0->iBlend[0] > v1->iBlend[0] ) return -1;
			else return 1;
		}
		if( v0->iBlend[1] != v1->iBlend[1] )
		{
			if( v0->iBlend[1] > v1->iBlend[1] ) return -1;
			else return 1;
		}
		if( v0->iBlend[2] != v1->iBlend[2] )
		{
			if( v0->iBlend[2] > v1->iBlend[2] ) return -1;
			else return 1;
		}
		if( v0->iBlend[3] != v1->iBlend[3] )
		{
			if( v0->iBlend[3] > v1->iBlend[3] ) return -1;
			else return 1;
		}

		if( v0->weights[0] != v1->weights[0] )
		{
			if( v0->weights[0] > v1->weights[0] ) return -1;
			else return 1;
		}
		if( v0->weights[1] != v1->weights[1] )
		{
			if( v0->weights[1] > v1->weights[1] ) return -1;
			else return 1;
		}
		if( v0->weights[2] != v1->weights[2] )
		{
			if( v0->weights[2] > v1->weights[2] ) return -1;
			else return 1;
		}
		if( v0->weights[3] != v1->weights[3] )
		{
			if( v0->weights[3] > v1->weights[3] ) return -1;
			else return 1;
		}
	}
	
	double dp3;
	
	double t0[3] = {v0->tangent[0], v0->tangent[1], v0->tangent[2]};
	NormalizeD (t0);
	double t1[3] = {v1->tangent[0], v1->tangent[1], v1->tangent[2]};
	NormalizeD (t1);
	dp3 = DotProduct3D (t0, t1);
	if (dp3 < ATI_TAN_ANGLE)
	{
		if (dp3 < 0.0) return -1;
		else return 1;
	}
	double b0[3] = {v0->binormal[0], v0->binormal[1], v0->binormal[2]};
	NormalizeD (b0);
	double b1[3] = {v1->binormal[0], v1->binormal[1], v1->binormal[2]};
	NormalizeD (b1);
	dp3 = DotProduct3D (b0, b1);
	if (dp3 < ATI_TAN_ANGLE)
	{
		if (dp3 < 0.0) return -1;
		else return 1;
	}
	return 0;
}

//=============================================================================
// Binary traversal function
//=============================================================================
static int 
NmIndexBinaryTraverseD(NmTangentPointD* value, // The reference element
					   NmTangentPointD* data,// pointer to the indexed data
					   int* indices,         // pointer index
					   int count,            // number of items in the array
					   int* result // buffer for the result of the last compare
					   , int texCoordCnt, int texCoordTan)
{
	int high;
	int low;
	int mid;
	int nextMid;
	
	high = count;
	low = 0;
	mid = low + ((high - low) >> 1);
	*result = -1;
	
	while (low != high) 
	{
		*result = NmCompareD (value, &(data[indices[mid]]), texCoordCnt, texCoordTan); 
		if (*result == 0)
		{
			break;
		}
		else if (*result < 0)
		{
			high = mid;
		}
		else
		{
			low = mid;
		}
		
		nextMid = low + ((high - low) >> 1);
		if (mid == nextMid)
		{
			break;
		}
		
		mid = nextMid;
	}
	
	return mid;
}

#define DcVec2Add(r, a, b)           ((r)[0] = (a)[0] + (b)[0], (r)[1] = (a)[1] + (b)[1])
#define DcVec2AddTo(a, b)            ((a)[0] += (b)[0], (a)[1] += (b)[1])
#define DcVec2AddScalar(r, a, b)     ((r)[0] = (a)[0] + (b), (r)[1] = (a)[1] + (b))
#define DcVec2AddScalarTo(a, b)      ((a)[0] += (b), (a)[1] += (b))
#define DcVec2Copy(a, b)             ((b)[0] = (a)[0], (b)[1] += (a)[1])
#define DcVec2Divide(r, a, b)        ((r)[0] = (a)[0] / (b)[0], (r)[1] = (a)[1] / (b)[1])
#define DcVec2DivideBy(a, b)         ((a)[0] /= (b)[0], (a)[1] /= (b)[1])
#define DcVec2DivideByScalar(v, s)   ((v)[0] /= s, (v)[1] /= s)
#define DcVec2DivideScalar(r, v, s)  ((r)[0] = (v)[0] / s, (r)[1] = (v)[1] / s)
#define DcVec2MidPoint(c, a, b)      ((r)[0] = (b)[0] + ((a)[0] - (b)[0]) * 0.5f, (r)[1] = (b)[1] + ((a)[1] - (b)[1]) * 0.5f)
#define DcVec2Mult(a, b, c)          ((c)[0] = (a)[0] * (b)[0], (c)[1] = (a)[1] * (b)[1])
#define DcVec2MultBy(a, b)           ((a)[0] *= (b)[0], (a)[1] *= (b)[1])
#define DcVec2MultByScalar(v, s)     ((v)[0] *= s, (v)[1] *= s)
#define DcVec2MultScalar(r, v, s)    ((r)[0] = (v)[0] * s, (r)[1] = (v)[1] * s)
#define DcVec2Mad(r, a, b, c)        ((r)[0] = (a)[0] * (b)[0] + (c)[0], (r)[1] = (a)[1] * (b)[1] + (c)[1])
#define DcVec2Negate(a, b)           ((b)[0] = -(a)[0], (b)[1] = -(a)[1])
#define DcVec2Scale(r, v, s)         ((r)[0] = (v)[0] * s, (r)[1] = (v)[1] * s)
#define DcVec2ScaleBy(v, s)          ((v)[0] *= s, (v)[1] *= s)
#define DcVec2Set(v, x, y)           ((v)[0] = x, (v)[1] = y)
#define DcVec2Subtract(r, a, b)      ((r)[0] = (a)[0] - (b)[0], (r)[1] = (a)[1] - (b)[1])
#define DcVec2SubtractFrom(a, b)     ((a)[0] -= (b)[0], (a)[1] -= (b)[1])
#define DcVec2Magnitude(v)           ( sqrtf((v)[0] * (v)[0] + (v)[1] * (v)[1]))
#define DcVec2MagnitudeDouble(v)     ( sqrt((v)[0] * (v)[0] + (v)[1] * (v)[1]))
#define DcVec2NormalizeDouble(v)     { double __mag = 1.0 / DcVec2MagnitudeDouble(v); \
(v)[0] *= __mag; (v)[1] *= __mag; }
#define DcVec2Normalize(v)           { float __mag = 1.0f / (float)DcVec2Magnitude(v); \
(v)[0] *= __mag; (v)[1] *= __mag; }


#define DcVecAdd(r, a, b)            ((r)[0] = (a)[0] + (b)[0], (r)[1] = (a)[1] + (b)[1], (r)[2] = (a)[2] + (b)[2])
#define DcVecAddTo(a, b)             ((a)[0] += (b)[0], (a)[1] += (b)[1], (a)[2] += (b)[2])
#define DcVecAddScalar(r, a, b)      ((r)[0] = (a)[0] + (b), (r)[1] = (a)[1] + (b), (r)[2] = (a)[2] + (b))
#define DcVecAddScalarTo(a, b)       ((a)[0] += (b), (a)[1] += (b), (a)[2] += (b))
#define DcVecCopy(a, b)              ((b)[0] = (a)[0], (b)[1] += (a)[1], (b)[2] += (a)[2])
#define DcVecCross(r, a, b)          ((r)[0] = (a)[1] * (b)[2] - (a)[2] * (b)[1], (r)[1] = (a)[2] * (b)[0] - (a)[0] * (b)[2], (r)[2] = (a)[0] * (b)[1] - (a)[1] * (b)[0])
#define DcVecDivide(r, a, b)         ((r)[0] = (a)[0] / (b)[0], (r)[1] = (a)[1] / (b)[1], (r)[2] = (a)[2] / (b)[2])
#define DcVecDivideBy(a, b)          ((a)[0] /= (b)[0], (a)[1] /= (b)[1], (a)[2] /= (b)[2])
#define DcVecDivideByScalar(v, s)    ((v)[0] /= s, (v)[1] /= s, (v)[2] /= s)
#define DcVecDivideScalar(r, v, s)   ((r)[0] = (v)[0] / s, (r)[1] = (v)[1] / s, (r)[2] = v[2] / s)
#define DcVecMidPoint(c, a, b)       ((r)[0] = (b)[0] + ((a)[0] - (b)[0]) * 0.5f, (r)[1] = (b)[1] + ((a)[1] - (b)[1]) * 0.5f, (r)[2] = (b)[2] + ((a)[2] - (b)[2]) * 0.5f)
#define DcVecMult(a, b, c)           ((c)[0] = (a)[0] * (b)[0], (c)[1] = (a)[1] * (b)[1], (c)[2] = (a)[2] * (b)[2])
#define DcVecMultBy(a, b)            ((a)[0] *= (b)[0], (a)[1] *= (b)[1], (a)[2] *= (b)[2])
#define DcVecMultByScalar(v, s)      ((v)[0] *= s, (v)[1] *= s, (v)[2] *= s)
#define DcVecMultScalar(r, v, s)     ((r)[0] = (v)[0] * s, (r)[1] = (v)[1] * s, (r)[2] = v[2] * s)
#define DcVecMad(r, a, b, c)         ((r)[0] = (a)[0] * (b)[0] + (c)[0], (r)[1] = (a)[1] * (b)[1] + (c)[1], (r)[2] = (a)[2] * (b)[2] + (c)[2])
#define DcVecNegate(a)               ((a)[0] = -(a)[0], (a)[1] = -(a)[1], (a)[2] = -(a)[2])
#define DcVecScale(r, v, s)          ((r)[0] = (v)[0] * s, (r)[1] = (v)[1] * s, (r)[2] = v[2] * s)
#define DcVecScaleBy(v, s)           ((v)[0] *= s, (v)[1] *= s, (v)[2] *= s)
#define DcVecSet(v, x, y, z)         ((v)[0] = x, (v)[1] = y, (v)[2] = z)
#define DcVecSubtract(r, a, b)       ((r)[0] = (a)[0] - (b)[0], (r)[1] = (a)[1] - (b)[1], (r)[2] = (a)[2] - (b)[2])
#define DcVecSubtractFrom(a, b)      ((a)[0] -= (b)[0], (a)[1] -= (b)[1], (a)[2] -= (b)[2])
#define DcVecMagnitude(v)            ( sqrtf((v)[0] * (v)[0] + (v)[1] * (v)[1] + (v)[2] * (v)[2]))
#define DcVecMagnitudeDouble(v)      ( sqrt((v)[0] * (v)[0] + (v)[1] * (v)[1] + (v)[2] * (v)[2]))
#define DcVecNormalizeDouble(v,def)  { double __mag = DcVecMagnitudeDouble(v); \
	if (__mag < EPSILON) {(v)[0] = (def)[0]; (v)[1] = (def)[1]; (v)[2] = (def)[2];} else \
{(v)[0] *= __mag; (v)[1] *= __mag; (v)[2] *= __mag;} }
#define DcVecNormalize(v)            { float __mag = 1.0f / (float)DcVecMagnitude(v); \
					   (v)[0] *= __mag; (v)[1] *= __mag; (v)[2] *= __mag; }
#define DcVecLineLength(a, b)        ( sqrtf(((a)[0] - (b)[0]) * ((a)[0] - (b)[0]) + \
	((a)[1] - (b)[1]) * ((a)[1] - (b)[1]) + \
																						 ((a)[2] - (b)[2]) * ((a)[2] - (b)[2])))
#define DcVecLerp(r, a, b, c)        { DcVecSubtract(r, b, a);  \
	DcVecMultByScalar(r, c);   \
DcVecAddTo(r, a); } 

#define DcVecDotProduct(a, b)        ((a)[0] * (b)[0] + (a)[1] * (b)[1] + (a)[2] * (b)[2])
#define DcVecDot4(a, b)              ((a)[0] * (b)[0] + (a)[1] * (b)[1] + (a)[2] * (b)[2] + (a)[3] * (b)[3])
#define DcVecDot3(a, b)              ((a)[0] * (b)[0] + (a)[1] * (b)[1] + (a)[2] * (b)[2])

#define DC_INVALID_ARG false
#define DC_OK true

bool DcTangentSpace(const float* v1, const float* v2, const float* v3, 
					const float* t1, const float* t2, const float* t3, 
					const double* normal, double *tangent, double *binormal)
{
	typedef struct
	{
		double    v[3];
		double    t[2];
	} tanSpaceVert;
	
	tanSpaceVert   verts[3];
	tanSpaceVert   tempVert;
	double          dot;
	double          interp;
	double          tempVector[3];
	
	//===================================//
	// make sure the arguments are valid //
	//===================================//
	if ((v1 != NULL) && 
		(v2 != NULL) && 
		(v3 != NULL) &&
		(t1 != NULL) && 
		(t2 != NULL) && 
		(t3 != NULL) &&
		(normal != NULL) && 
		(tangent != NULL) && 
		(binormal != NULL))
	{
		//========================================//
		// fill in the tangent space vertex array //
		//========================================//
		verts[0].v[0] = v1[0];
		verts[0].v[1] = v1[1];
		verts[0].v[2] = v1[2];
		verts[0].t[0] = t1[0];
		verts[0].t[1] = t1[1];
		
		verts[1].v[0] = v2[0];
		verts[1].v[1] = v2[1];
		verts[1].v[2] = v2[2];
		verts[1].t[0] = t2[0];
		verts[1].t[1] = t2[1];
		
		verts[2].v[0] = v3[0];
		verts[2].v[1] = v3[1];
		verts[2].v[2] = v3[2];
		verts[2].t[0] = t3[0];
		verts[2].t[1] = t3[1];
		
		//============================//
		// compute the tangent vector //
		//============================//
		{
			//=======================================================//
			// sort the vertices based on their t texture coordinate //
			//=======================================================//
			if (verts[0].t[1] < verts[1].t[1])
			{
				tempVert = verts[0];
				verts[0] = verts[1];
				verts[1] = tempVert;
			}
			if (verts[0].t[1] < verts[2].t[1])
			{
				tempVert = verts[0];
				verts[0] = verts[2];
				verts[2] = tempVert;
			}
			if (verts[1].t[1] < verts[2].t[1])
			{
				tempVert = verts[1];
				verts[1] = verts[2];
				verts[2] = tempVert;
			}
			
			//=======================================================================//
			// compute the parametric offset along edge02 to the middle t coordinate //
			//=======================================================================//
			if (fabs((verts[2].t[1] - verts[0].t[1])) < EPSILON)
			{
				interp = 1.0;
			}
			else
			{
				interp = (verts[1].t[1] - verts[0].t[1]) / (verts[2].t[1] - verts[0].t[1]);
			}
			
			//============================================================================//
			// use the interpolation paramter to compute the vertex position along edge02 //
			// that coresponds to the same t coordinate as v[1]                           //
			//============================================================================//
			DcVecLerp(tempVector, verts[0].v, verts[2].v, interp);
			
			//========================================//
			// compute the interpolated s coord value //
			//========================================//
			interp = verts[0].t[0] + (verts[2].t[0] - verts[0].t[0]) * interp;
			
			//===========================================================================//
			// the tangent vector is the ray from the middle vertex to the lerped vertex //
			//===========================================================================//
			DcVecSubtract(tangent, tempVector, verts[1].v);
			
			//=====================================================//
			// make sure the tangent points in the right direction //
			//=====================================================//
			if (interp < verts[1].t[0])
			{
				DcVecNegate(tangent);
			}
			
			//=============================================================//
			// make sure the tangent vector is perpendicular to the normal //
			//=============================================================//
			dot = DcVecDot3(normal, tangent);            
			tangent[0] = tangent[0] - normal[0] * dot;
			tangent[1] = tangent[1] - normal[1] * dot;
			tangent[2] = tangent[2] - normal[2] * dot;
			
			//==============================//
			// normalize the tangent vector //
			//==============================//
			static double defTan[3] = {1.0, 0.0, 0.0};
			DcVecNormalizeDouble(tangent, defTan);
		}
		
		//=============================//
		// compute the binormal vector //
		//=============================//
		{
			//=======================================================//
			// sort the vertices based on their s texture coordinate //
			//=======================================================//
			if (verts[0].t[0] < verts[1].t[0])
			{
				tempVert = verts[0];
				verts[0] = verts[1];
				verts[1] = tempVert;
			}
			if (verts[0].t[0] < verts[2].t[0])
			{
				tempVert = verts[0];
				verts[0] = verts[2];
				verts[2] = tempVert;
			}
			if (verts[1].t[0] < verts[2].t[0])
			{
				tempVert = verts[1];
				verts[1] = verts[2];
				verts[2] = tempVert;
			}
			
			//=======================================================================//
			// compute the parametric offset along edge02 to the middle s coordinate //
			//=======================================================================//
			if (fabs((verts[2].t[0] - verts[0].t[0])) < EPSILON)
			{
				interp = 1.0;
			}
			else
			{
				interp = (verts[1].t[0] - verts[0].t[0]) / (verts[2].t[0] - verts[0].t[0]);
			}
			
			//============================================================================//
			// use the interpolation paramter to compute the vertex position along edge02 //
			// that coresponds to the same t coordinate as v[1]                           //
			//============================================================================//
			DcVecLerp(tempVector, verts[0].v, verts[2].v, interp);
			
			//========================================//
			// compute the interpolated t coord value //
			//========================================//
			interp = verts[0].t[1] + (verts[2].t[1] - verts[0].t[1]) * interp;
			
			//============================================================================//
			// the binormal vector is the ray from the middle vertex to the lerped vertex //
			//============================================================================//
			DcVecSubtract(binormal, tempVector, verts[1].v);
			
			//======================================================//
			// make sure the binormal points in the right direction //
			//======================================================//
			if (interp < verts[1].t[1])
			{
				DcVecNegate(binormal);
			}
			
			//==============================================================//
			// make sure the binormal vector is perpendicular to the normal //
			//==============================================================//
			dot = DcVecDot3(normal, binormal);            
			binormal[0] = binormal[0] - normal[0] * dot;
			binormal[1] = binormal[1] - normal[1] * dot;
			binormal[2] = binormal[2] - normal[2] * dot;
			
			//===============================//
			// normalize the binormal vector //
			//===============================//
			static double defBi[3] = {0.0, 0.0, 1.0};
			DcVecNormalizeDouble(binormal, defBi);
		}
	 }
	 else
	 {
		 return DC_INVALID_ARG;
	 }
	 
	 return DC_OK;
}
/*
bool DcTangentSpace_ROTATE_METHOD(const float* v1, const float* v2, const float* v3, 
								  const float* t1, const float* t2, const float* t3, 
								  const double* normal, double *tangent, double *binormal)
{
	typedef struct
	{
		double    v[3];
		double    t[2];
	} tanSpaceVert;
	
	tanSpaceVert   verts[3];
	tanSpaceVert   tempVert;
	double          dot;
	double          interp;
	double          tempVector[3];
	
	//===================================//
	// make sure the arguments are valid //
	//===================================//
	if ( !((v1 != NULL) && (v2 != NULL) && (v3 != NULL)
		&& (t1 != NULL) && (t2 != NULL) && (t3 != NULL)
		&& (normal != NULL) && (tangent != NULL) && (binormal != NULL))
		)
	{
		 return DC_INVALID_ARG;
	}
	//========================================//
	// fill in the tangent space vertex array //
	//========================================//
	verts[0].v[0] = v1[0];
	verts[0].v[1] = v1[1];
	verts[0].v[2] = v1[2];
	verts[0].t[0] = t1[0];
	verts[0].t[1] = t1[1];
	
	verts[1].v[0] = v2[0];
	verts[1].v[1] = v2[1];
	verts[1].v[2] = v2[2];
	verts[1].t[0] = t2[0];
	verts[1].t[1] = t2[1];
	
	verts[2].v[0] = v3[0];
	verts[2].v[1] = v3[1];
	verts[2].v[2] = v3[2];
	verts[2].t[0] = t3[0];
	verts[2].t[1] = t3[1];

	//아직 구현하지 않았음.
	ASSERTALWAYS("미구현");

	return DC_OK;
}

bool DcTangentSpace_SIMPLE_METHOD(const float* v1, const float* v2, const float* v3, 
								  const float* t1, const float* t2, const float* t3, 
								  const double* normal, double *tangent, double *binormal)
{
	typedef struct
	{
		double    v[3];
		double    t[2];
	} tanSpaceVert;
	
	tanSpaceVert   verts[3];
	tanSpaceVert   tempVert;
	double          dot;
	double          interp;
	double          tempVector[3];
	
	//===================================//
	// make sure the arguments are valid //
	//===================================//
	if ( !((v1 != NULL) && (v2 != NULL) && (v3 != NULL)
		&& (t1 != NULL) && (t2 != NULL) && (t3 != NULL)
		&& (normal != NULL) && (tangent != NULL) && (binormal != NULL))
		)
	{
		 return DC_INVALID_ARG;
	}
	//========================================//
	// fill in the tangent space vertex array //
	//========================================//
	verts[0].v[0] = v1[0];
	verts[0].v[1] = v1[1];
	verts[0].v[2] = v1[2];
	verts[0].t[0] = t1[0];
	verts[0].t[1] = t1[1];
	
	verts[1].v[0] = v2[0];
	verts[1].v[1] = v2[1];
	verts[1].v[2] = v2[2];
	verts[1].t[0] = t2[0];
	verts[1].t[1] = t2[1];
	
	verts[2].v[0] = v3[0];
	verts[2].v[1] = v3[1];
	verts[2].v[2] = v3[2];
	verts[2].t[0] = t3[0];
	verts[2].t[1] = t3[1];

	//아직 구현하지 않았음.
	ASSERTALWAYS("미구현");

	return DC_OK;
}

bool DcTangentSpace_ANT_METHOD(const float* v1, const float* v2, const float* v3, 
							   const float* t1, const float* t2, const float* t3, 
							   const double* normal, double *tangent, double *binormal)
{
	typedef struct
	{
		double    v[3];
		double    t[2];
	} tanSpaceVert;
	
	tanSpaceVert   verts[3];
	tanSpaceVert   tempVert;
	double          dot;
	double          interp;
	double          tempVector[3];
	
	//===================================//
	// make sure the arguments are valid //
	//===================================//
	if ( !((v1 != NULL) && (v2 != NULL) && (v3 != NULL)
		&& (t1 != NULL) && (t2 != NULL) && (t3 != NULL)
		&& (normal != NULL) && (tangent != NULL) && (binormal != NULL))
		)
	{
		 return DC_INVALID_ARG;
	}
	//========================================//
	// fill in the tangent space vertex array //
	//========================================//
	verts[0].v[0] = v1[0];
	verts[0].v[1] = v1[1];
	verts[0].v[2] = v1[2];
	verts[0].t[0] = t1[0];
	verts[0].t[1] = t1[1];
	
	verts[1].v[0] = v2[0];
	verts[1].v[1] = v2[1];
	verts[1].v[2] = v2[2];
	verts[1].t[0] = t2[0];
	verts[1].t[1] = t2[1];
	
	verts[2].v[0] = v3[0];
	verts[2].v[1] = v3[1];
	verts[2].v[2] = v3[2];
	verts[2].t[0] = t3[0];
	verts[2].t[1] = t3[1];

	//아직 구현하지 않았음.
	ASSERTALWAYS("미구현");

	return DC_OK;
}
*/
////////////////////////////////////////////////////////////////////
// Compute tangent space for the given triangle
////////////////////////////////////////////////////////////////////
static void
ComputeTangentVectorsD (NmRawTriangle* pgon, int idx,
						double tan[3], double norm[3], double binorm[3], int texCoordCnt, int texCoordTan)
{
	ASSERT(texCoordCnt > 0 && (texCoordTan >= 0 && texCoordTan < texCoordCnt));

	// Clear outputs.
	tan[0] = 1.0; tan[1] = 0.0; tan[2] = 0.0;
	binorm[0] = 0.0; binorm[1] = 0.0; binorm[2] = 1.0;
	norm[0] = pgon->norm[idx].x;
	norm[1] = pgon->norm[idx].y;
	norm[2] = pgon->norm[idx].z;
	
	// Make sure we have valid data.
	if (pgon->texCoord[0][texCoordTan].u == 0.0 && 
		pgon->texCoord[1][texCoordTan].u == 0.0 && 
		pgon->texCoord[2][texCoordTan].u == 0.0)
	{
		return;
	}
	
	DcTangentSpace(pgon->vert[0].v, pgon->vert[1].v, pgon->vert[2].v, 
		pgon->texCoord[0][texCoordTan].uv, pgon->texCoord[1][texCoordTan].uv,
		pgon->texCoord[2][texCoordTan].uv, norm, tan, binorm);
} // end ComputeTangentVectors

////////////////////////////////////////////////////////////////////
// Copy data into a point structure
////////////////////////////////////////////////////////////////////
static void
NmCopyPointD (NmRawTriangle* tri, int v, NmTangentPointD* point, int texCoordCnt, int texCoordTan)
{
	point->vertex[0] = tri->vert[v].x;
	point->vertex[1] = tri->vert[v].y;
	point->vertex[2] = tri->vert[v].z;
	point->normal[0] = tri->norm[v].x;
	point->normal[1] = tri->norm[v].y;
	point->normal[2] = tri->norm[v].z;
	for(int iUV=0; iUV<texCoordCnt; ++iUV)
	{
		point->u[iUV] = tri->texCoord[v][iUV].u;
		point->v[iUV] = tri->texCoord[v][iUV].v;
	}
	point->useWeight = tri->weights[v].use;
	if(point->useWeight)
	{
		point->iBlend[0] = tri->weights[v].iBlend[0];
		point->iBlend[1] = tri->weights[v].iBlend[1];
		point->iBlend[2] = tri->weights[v].iBlend[2];
		point->iBlend[3] = tri->weights[v].iBlend[3];
		point->weights[0] = tri->weights[v].weight[0];
		point->weights[1] = tri->weights[v].weight[1];
		point->weights[2] = tri->weights[v].weight[2];
		point->weights[3] = tri->weights[v].weight[3];
	}
	double norm[3];
 	ComputeTangentVectorsD (tri, v, point->tangent, norm, point->binormal, texCoordCnt, texCoordTan);
	point->count = 1;
}

////////////////////////////////////////////////////////////////////
// Insert a point and get it's index.
////////////////////////////////////////////////////////////////////
static int
NmInsertD (NmRawTriangle* tri, int v, int* num, int* sortIndex,
		   NmTangentPointD* point, int texCoordCnt, int texCoordTan)
{
	// Make sure we have some stuff to check.
	int ret = 0;
	if ((*num) > 0)
	{
		// Copy point into available slot.
		NmTangentPointD* pt = &(point[(*num)]);
		NmCopyPointD (tri, v, pt, texCoordCnt, texCoordTan);
		
		// Search for it.
		int compValue;
		int pos = NmIndexBinaryTraverseD (pt, point, sortIndex, (*num),
			&compValue, texCoordCnt, texCoordTan);
		
		// Now see if we need to insert.
		if (compValue == 0)
		{
			point[sortIndex[pos]].tangent[0] += pt->tangent[0];
			point[sortIndex[pos]].tangent[1] += pt->tangent[1];
			point[sortIndex[pos]].tangent[2] += pt->tangent[2];
			point[sortIndex[pos]].binormal[0] += pt->binormal[0];
			point[sortIndex[pos]].binormal[1] += pt->binormal[1];
			point[sortIndex[pos]].binormal[2] += pt->binormal[2];
			point[sortIndex[pos]].count++;
			ret = sortIndex[pos];
		}
		else if (compValue < 0) // we are inserting before this index
		{
			ret = (*num);
			memmove (&(sortIndex[pos + 1]), &(sortIndex[pos]), 
				((*num) - pos) * sizeof(int));
			
			sortIndex[pos] = (*num);
			(*num)++;
		}
		else // we are appending after this index
		{
			ret = (*num);
			if (pos < ((*num) - 1))
			{
				memmove(&(sortIndex[pos + 2]), &(sortIndex[pos + 1]), 
					((*num) - pos - 1) * sizeof(int));
			}
			sortIndex[pos + 1] = (*num);
			(*num)++;
		}
	}
	else
	{  // First point just add it into our list.
		NmCopyPointD (tri, v, &(point[(*num)]), texCoordCnt, texCoordTan);
		sortIndex[(*num)] = 0;
		ret = (*num);
		(*num)++;
	}
	return ret;
}


////////////////////////////////////////////////////////////////////
// Turn raw triangles into vertex/index buffers.
////////////////////////////////////////////////////////////////////
bool 
NmCreateVertexBuffers (int numTris, NmRawTriangle* tris, 
					   int* numVerts, NmTangentPointD** verts, NmIndex** indices,
					   int texCoordCnt, int texCoordTan)
{
	// Check inputs
	if ((numTris == 0) || (tris == NULL) || (verts == NULL) || (indices == NULL)
		|| (texCoordCnt < 1) || (texCoordTan < 0 || texCoordTan >= texCoordCnt))
	{
		return false;
	}
	
	int i;
	// Allocate storage structures
	(*indices) = new NmIndex [numTris];
	if ((*indices) == NULL)
	{
		return false;
	}
	NmIndex* index = (*indices);
	int* sortIndex = new int [numTris*3]; // Brute force it
	if (sortIndex == NULL)
	{
		return false;
	}
	(*verts) = new NmTangentPointD [numTris*3]; // Brute force it
	if ((*verts) == NULL)
	{
		SAFE_ARRAY_DELETE(sortIndex);
		return false;
	}
	NmTangentPointD* point = (*verts);
	for(i=0; i<numTris*3; ++i)
	{
		point[i].u = new double[texCoordCnt];
		point[i].v = new double[texCoordCnt];
	}
	
	// Now go through finding matching vertices and computing tangents.
	int count = 0;
	for (i = 0; i < numTris; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			index[i].idx[j] = NmInsertD (&tris[i], j, &count, sortIndex, point, texCoordCnt, texCoordTan);
		}
	}
	(*numVerts) = count;
	
	// Next we renormalize
	for (i = 0; i < count; i++)
	{
		point[i].tangent[0] = point[i].tangent[0]/(double)point[i].count;
		point[i].tangent[1] = point[i].tangent[1]/(double)point[i].count;
		point[i].tangent[2] = point[i].tangent[2]/(double)point[i].count;
		NormalizeD (point[i].tangent);
		
		point[i].binormal[0] = point[i].binormal[0]/(double)point[i].count;
		point[i].binormal[1] = point[i].binormal[1]/(double)point[i].count;
		point[i].binormal[2] = point[i].binormal[2]/(double)point[i].count;
		NormalizeD (point[i].binormal);
	}
	
	// Success!
	SAFE_ARRAY_DELETE(sortIndex);
	return true;
} // end of NmCreateVertexBuffers
