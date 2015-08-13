#ifndef __CCAMERAEFFECT_H__
#define __CCAMERAEFFECT_H__

#include "CEffect.h"
#include "CSampleSpline.h"
#include <Engine/Base/Stream.h>
#include <Engine/Math/Vector.h>

class ENGINE_API CCameraEffect : public CEffect
{
public:
	class CCameraValue
	{
	public:
		CCameraValue()
		{
			Reset();
		}
		CCameraValue(const CCameraValue &other)
		{
			m_fHorizonal = other.m_fHorizonal;
			m_fVertical = other.m_fVertical;
			m_fTwist = other.m_fTwist;
			m_fZoom = other.m_fZoom;
			m_vMoveByCharCoord = other.m_vMoveByCharCoord;
		}
		CCameraValue &operator=(const CCameraValue &other)
		{
			m_fHorizonal = other.m_fHorizonal;
			m_fVertical = other.m_fVertical;
			m_fTwist = other.m_fTwist;
			m_fZoom = other.m_fZoom;
			m_vMoveByCharCoord = other.m_vMoveByCharCoord;
			return *this;
		}
		void Reset()
		{
			m_fHorizonal = 0;
			m_fVertical = 0;
			m_fTwist = 0;
			m_fZoom = 0;
			m_vMoveByCharCoord = FLOAT3D(0,0,0);
		}
		CCameraValue operator -() const
		{
			CCameraValue ret;
			ret.m_fHorizonal = -m_fHorizonal;
			ret.m_fVertical = -m_fVertical;
			ret.m_fTwist = -m_fTwist;
			ret.m_fZoom = -m_fZoom;
			ret.m_vMoveByCharCoord = -m_vMoveByCharCoord;
			return ret;
		}
		CCameraValue &operator +=(const CCameraValue &other)
		{
			m_fHorizonal += other.m_fHorizonal;
			m_fVertical += other.m_fVertical;
			m_fTwist += other.m_fTwist;
			m_fZoom += other.m_fZoom;
			m_vMoveByCharCoord += other.m_vMoveByCharCoord;
			return *this;
		}
		CCameraValue operator +(const CCameraValue &other) const
		{
			return CCameraValue(*this) += other;
		}
		CCameraValue &operator -=(const CCameraValue &other)
		{
			*this += -other;
			return *this;
		}
		CCameraValue operator -(const CCameraValue &other) const
		{
			return CCameraValue(*this) -= other;
		}
		CCameraValue &operator *=(const FLOAT &val)
		{
			m_fHorizonal *= val;
			m_fVertical *= val;
			m_fTwist *= val;
			m_fZoom *= val;
			m_vMoveByCharCoord *= val;
			return *this;
		}
		CCameraValue operator *(const FLOAT &val) const
		{
			return CCameraValue(*this) *= val;
		}
#		define CAMERAVALUE_VERSION 1
		friend CTStream &operator<<(CTStream &out, const CCameraValue &cv)
		{
			out << CAMERAVALUE_VERSION;
			out << cv.m_fHorizonal;
			out << cv.m_fVertical;
			out << cv.m_fTwist;
			out << cv.m_fZoom;
			out << cv.m_vMoveByCharCoord;
			return out;
		}
		friend CTStream &operator>>(CTStream &in, CCameraValue &cv)
		{
			INDEX ver;
			in >> ver;
			if(ver == CAMERAVALUE_VERSION)
			{
				in >> cv.m_fHorizonal;
				in >> cv.m_fVertical;
				in >> cv.m_fTwist;
				in >> cv.m_fZoom;
				in >> cv.m_vMoveByCharCoord;
			}
			return in;
		}
#		undef CAMERAVALUE_VERSION
	public:
		FLOAT	m_fHorizonal;
		FLOAT	m_fVertical;
		FLOAT	m_fTwist;
		FLOAT	m_fZoom;
		FLOAT3D	m_vMoveByCharCoord;
	};
public:
	CCameraEffect();
	virtual ~CCameraEffect();

	//NEW_DELETE_DEFINITION(CFlameEffect);

	virtual void Read(CTStream *pIS);
	virtual void Write(CTStream *pOS);
	
	virtual void Start(FLOAT time, BOOL restart = FALSE);
	virtual BOOL Process(FLOAT time);
	virtual CEffect *Copy();
	virtual void Render();
	virtual void Stop(FLOAT leftTime);

	inline UINT AddShakeValue(FLOAT time, const CCameraValue &cvShakeValue)
												{ return m_ssCameraShake.AddSample(time, cvShakeValue);	}
	inline UINT RemoveShakeValue(SINT index)	{ return m_ssCameraShake.RemoveSample(index);			}
	inline const CCameraValue &GetShakeValueValue(UINT index)
												{ return m_ssCameraShake.GetValue(index);				}
	inline FLOAT GetShakeValueTime(UINT index)	{ return m_ssCameraShake.GetKey(index);					}
	inline UINT GetShakeValueCount()			{ return m_ssCameraShake.GetSampleCount();				}
protected:
	CCameraEffect(const CCameraEffect& other) {}
	CCameraEffect &operator =(const CCameraEffect &other) { return *this; }

	void ClearInstance();
protected:
	//contents
	CSampleSpline< CCameraValue > m_ssCameraShake;
	//instance
	FLOAT			m_fSampleSplineLife;
	CCameraValue	m_cvCameraShakeSum;
	CCameraValue	m_cvOldCameraShakeSum;
	INDEX			m_iOnlyStartFadeOut;
};

#endif //__CCAMERAEFFECT_H__
