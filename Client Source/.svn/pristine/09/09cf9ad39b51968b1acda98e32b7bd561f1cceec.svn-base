//안태훈 수정 시작	//(Remake Effect)(0.1)

#ifndef __ADDITIONALFUNCTION_H__
#define __ADDITIONALFUNCTION_H__

#include <math.h>

#include <Engine/Base/Types.h>
#include <Engine/Math/Vector.h>
#include <Engine/Math/Quaternion.h>
#include <Engine/Math/Matrix12.h>

#include <float.h>
inline bool IsNormalFloat(FLOAT fVal)
{
	return (_fpclass(fVal) == 0x0008	/* negative normal */
		 || _fpclass(fVal) == 0x0020	/* -0 */
		 || _fpclass(fVal) == 0x0040	/* +0 */
		 || _fpclass(fVal) == 0x0100);	/* positive normal */
}

inline FLOAT3D Lerp(const FLOAT3D &x0, const FLOAT3D &x1, FLOAT fRatio)
{
		 if( fRatio==0) return x0;
	else if( fRatio==1) return x1;
	else return x0+(x1-x0)*fRatio;
}

template<class T> class CValue
{
public:
	CValue() : m_value( 0 )			{}
	CValue(T& val) : m_value( val )	{}
	CValue(CValue<T>& val) : m_value( val.m_value )	{}

	inline operator T&(void)		{ return m_value; }
	inline operator T&(void) const	{ return m_value; }

	T	m_value;
};
typedef CValue<float> CFloatValue;

inline FLOAT3D VectorRotate(const FLOAT3D &vec, const FLOATquat3D &quat)
{
	FLOAT3D quatVec(quat.q_x, quat.q_y, quat.q_z);
	FLOAT3D cpQuatVec = quatVec * vec;
	FLOAT3D vRet = ( vec * (quat.q_w * quat.q_w)	)
				 + ( cpQuatVec * (2.0f * quat.q_w)	)
				 + ( quatVec * (quatVec % vec)		)
				 - ( cpQuatVec * quatVec			);
	return vRet;
}

inline FLOAT3D VectorOnlyYRotate(const FLOAT3D &vec, const FLOATquat3D &quat)
{
	return FLOAT3D(
		quat.q_z * vec(2) * (-vec(2) - 2.0f * quat.q_w),
		vec(2) * (2.0f * quat.q_w * quat.q_w + vec(2)),
		quat.q_z * vec(2) * (2.0f * quat.q_x - vec(2))
		);
}

inline FLOAT3D VectorOnlyYRotate(const FLOAT &h, const FLOATquat3D &quat)
{
	return FLOAT3D(
		quat.q_z * h * (-h - 2.0f * quat.q_w),
		h * (2.0f * quat.q_w * quat.q_w + h),
		quat.q_z * h * (2.0f * quat.q_x - h)
		);
}

inline bool FloatSame(const FLOAT f1, const FLOAT f2)
{
	return fabs(f1 - f2) < 0.000001f;
}

inline void RotateVector(FLOAT3D &vInOutVector, const FLOATmatrix3D &matRot)
{
	Matrix12 matrix12;
	matrix12[0] = matRot(1, 1); matrix12[1] = matRot(1, 2); matrix12[2] = matRot(1, 3); matrix12[3] = 0;
	matrix12[4] = matRot(2, 1); matrix12[5] = matRot(2, 2); matrix12[6] = matRot(2, 3); matrix12[7] = 0;
	matrix12[8] = matRot(3, 1); matrix12[9] = matRot(3, 2); matrix12[10] = matRot(3, 3); matrix12[11] = 0;
	::RotateVector(vInOutVector.vector, matrix12);
}

inline void RotateVector(FLOAT3D &vInOutVector, const FLOATquat3D &qRot)
{
	FLOATmatrix3D matrix;
	qRot.ToMatrix(matrix);
	RotateVector(vInOutVector, matrix);
}

inline ANGLE3D GetEulerAngleFromDir(FLOAT3D &vDir)
{
	ANGLE3D aRetAngle;

	//head
	FLOAT fLenYPrj = vDir(1) * vDir(1) + vDir(3) * vDir(3);
	FLOAT fLen = sqrtf(fLenYPrj + vDir(2) * vDir(2));
	aRetAngle(2) = fLen != 0 ? AngleRad( acos(vDir(2) / fLen) ) : 0;	//0~180, 180이 넘어가면 Head에서 처리된다.

	//pitch
	fLenYPrj = sqrtf(fLenYPrj);
	aRetAngle(1) = fLenYPrj != 0 ? AngleRad( acos(vDir(3)/fLenYPrj) ) : 0;	//0~360
	//Temp: 이상하게 6도 차이남. 나중에 수정 필요할지도
	aRetAngle(1) = vDir(1) < 0 ? 2 * PI - aRetAngle(1) - 6.0f : aRetAngle(1);

	//banking
	aRetAngle(3) = 0;

	return aRetAngle;
}

inline ANGLE3D GetEulerAngleFromQuaternion(const FLOATquat3D &quat)
{
	FLOAT3D vDir(0,1,0);
	FLOATmatrix3D mRot;
	quat.ToMatrix(mRot);
	RotateVector(vDir, mRot);
	return GetEulerAngleFromDir(vDir);
}


#endif //__ADDITIONALFUNCTION_H__
//안태훈 수정 끝	//(Remake Effect)(0.1)
