//안태훈 수정 시작	//(Remake Effect)(0.1)
#ifndef __CSAMPLESPLINE_H__
#define __CSAMPLESPLINE_H__

#include <Engine/Base/Types.h>
#include <Engine/Math/Quaternion.h>
#include <Engine/Base/Memory.h>
#include <vector>
#include <algorithm>
#include <Engine/Graphics/Color.h>
#include <Engine/Base/Memory.h>

#define ssReadFromStream(Stream, ssName, NameType) \
{\
	DWORD count;\
	float key;\
	Stream >> count;\
	for(DWORD i=0; i<count; ++i)\
	{\
		NameType nameValue;\
		Stream >> key;\
		Stream >> nameValue;\
		ssName.AddSample(key, nameValue);\
	}\
}

#define ssWriteToStream(Stream, ssName) \
{\
	Stream << (DWORD)ssName.GetSampleCount();\
	for(int i=0; i<ssName.GetSampleCount(); ++i)\
	{\
		Stream << ssName.GetKey(i);\
		Stream << ssName.GetValue(i);\
	}\
}

//기본적으로 사용되는 linear interpolate 함수자
template<class ValueType>
class CLerp
{
public:
	ValueType operator()(const ValueType &val1, const ValueType &val2, float ratio)
	{
		return (val2 - val1) * ratio + val1;
	}
	inline void Prepare(unsigned int count, const std::vector<ValueType> &values, std::vector<ValueType> &precomputeValues) {}
};

template<>
class CLerp<COLOR>
{
public:
	COLOR operator()(const COLOR &val1, const COLOR &val2, float ratio)
	{
		return LerpColorInline(val1, val2, ratio);
	}

	inline void Prepare(unsigned int count, const std::vector<COLOR> &values, std::vector<COLOR> &precomputeValues) {}
private:
};

template<>
class CLerp<FLOATquat3D>
{
public:
	FLOATquat3D operator()(const FLOATquat3D &val1, const FLOATquat3D &val2, float ratio)
	{
		return Slerp(ratio, val1, val2);
	}

	inline void Prepare(unsigned int count, const std::vector<FLOATquat3D> &values, std::vector<FLOATquat3D> &precomputeValues) {}
private:
};

//Natural Cubic Spline interpolate 함수자
template<class ValueType>
class CCubicSplineInterpolate
{
public:
	ValueType operator()(const ValueType &val1, const ValueType &val2
						, const ValueType &valPC1, const ValueType &valPC2
						, float ratio)
	{
		return val1
			+ valPC1 * ratio
			+ ((val2 - val1) * 3.0f - valPC1 * 2.0f - valPC2) * ratio * ratio
			+ ((val1 - val2) * 2.0f + valPC1 + valPC2) * ratio * ratio * ratio;
	}

	inline void Prepare( unsigned int count
						, const std::vector<ValueType> &values
						, std::vector<ValueType> &precomputeValues )
	{
		if(count <= 2)
		{
			memset(&precomputeValues[0], 0, sizeof(ValueType)*count);
			return;
		}
		unsigned int n = count - 1, i;
		std::vector<float> gamma;
		gamma.resize(count);
		std::vector<ValueType> delta;
		delta.resize(count);
		std::vector<ValueType> &vectorTemp = precomputeValues;

		gamma[0] = 1.0f/2.0f;
		for( i=1; i<n; ++i) gamma[i] = 1.0f/(4.0f-gamma[i-1]);
		gamma[n] = 1.0f/(2.0f-gamma[n-1]);

		delta[0] = (values[1] - values[0]) * (3.0f * gamma[0]);
		for( i=1; i<n; ++i) delta[i] = ( (values[i+1] - values[i-1]) * 3.0f - delta[i-1] ) * gamma[i];
		delta[n] = ( (values[n] - values[n-1]) * 3.0f - delta[n-1] ) * gamma[n];
    
		vectorTemp[n] = delta[n];
		for( i=n-1; i>=0; --i) vectorTemp[i] = delta[i] - gamma[i] * vectorTemp[i+1];
	}
};

template<>
class CCubicSplineInterpolate<FLOAT3D>
{
public:
	FLOAT3D operator()(const FLOAT3D &val1, const FLOAT3D &val2
						, const FLOAT3D &valPC1, const FLOAT3D &valPC2
						, float ratio)
	{
		//return val1
			//+ valPC1 * ratio
			//+ ((val2 - val1) * 3.0f - valPC1 * 2.0f - valPC2) * (ratio * ratio)
			//+ ((val1 - val2) * 2.0f + valPC1 + valPC2) * (ratio * ratio * ratio);
		float r2 = ratio * ratio;
		float r3 = ratio * ratio * ratio;
		return val1 * (1 - (r2+r2+r2) + (r3+r3))
			+ val2 * ((r2+r2+r2) - (r3+r3))
			+ valPC1 * (ratio - (r2+r2) + r3)
			+ valPC2 * (-r2 + r3);
	}

	inline void Prepare( unsigned int count
						, const std::vector<FLOAT3D> &values
						, std::vector<FLOAT3D> &precomputeValues )
	{
		if(count <= 2)
		{
			memset(&precomputeValues[0], 0, sizeof(FLOAT3D)*count);
			return;
		}
		unsigned int n = count - 1, i;
		std::vector<float> gamma;
		gamma.resize(count);
		std::vector<FLOAT3D> delta;
		delta.resize(count);
		std::vector<FLOAT3D> &vectorTemp = precomputeValues;
		vectorTemp.resize(count);

		gamma[0] = 1.0f/2.0f;
		for( i=1; i<n; ++i) gamma[i] = 1.0f/(4.0f-gamma[i-1]);
		gamma[n] = 1.0f/(2.0f-gamma[n-1]);

		delta[0] = (values[1] - values[0]) * (3.0f * gamma[0]);
		for( i=1; i<n; ++i) delta[i] = ( (values[i+1] - values[i-1]) * 3.0f - delta[i-1] ) * gamma[i];
		delta[n] = ( (values[n] - values[n-1]) * 3.0f - delta[n-1] ) * gamma[n];
    
		vectorTemp[n] = delta[n];
		for(i=n; i>0; --i)
		{
			vectorTemp[i-1](1) = delta[i-1](1) - gamma[i] * vectorTemp[i](1);
			vectorTemp[i-1](2) = delta[i-1](2) - gamma[i] * vectorTemp[i](2);
			vectorTemp[i-1](3) = delta[i-1](3) - gamma[i] * vectorTemp[i](3);
		}
	}
};

//Hermite Spline Interpolate 함수자, 일단 Hermite의 alpha는 0이라고 단정함.
template<class ValueType>
class CHermiteSplineInterpolate
{
public:
	ValueType operator()(const ValueType &val1, const ValueType &val2
						, const ValueType &val0, const ValueType &val3
						, float ratio)
	{
		float ratio2 = ratio * ratio;
		float ratio3 = ratio2 * ratio;
		return (val0 * (-0.5f * ratio3 + ratio2 - 0.5f * ratio) +
				val1 * (1.5f * ratio3 - 2.5f * ratio2 + 1.0f) +
				val2 * (-1.5f * ratio3 + 2.0f * ratio2 + 0.5f * ratio) +
				val3 * (0.5f * ratio3 - 0.5f * ratio2));
	}

	inline void Prepare( unsigned int count
						, const std::vector<ValueType> &values
						, std::vector<ValueType> &precomputeValues )
	{
		//nothing
	}
};


/**
CSampleSpline
-float를 키로 갖는 sample들을 여러개 가지고 특정 입력(float)에 대해 sample값의 interpolate된 값을 리턴한다.
-interpolate함수는 입력으로 주어질 수 있으며 기본 함수는 linear interpolate이다.
-기본 interpolate는 함수로 구현되어 있지 않고 값의 연산을 이용하여 구현된다.
-필요한 연산은 값들 사이의 더하기, 빼기, float으로 곱하기이다.
*/
template<class ValueType, class InterpolateFunc = CLerp<ValueType> >
class CSampleSpline
{
private:
	//벡터에 저장되는 자료구조
	class CSample
	{
	public:
		float		m_fKey;
		ValueType	m_value;
		ValueType	m_valuePrecompute;
		//std::vector<ValueType>	m_vectorPrecomputeValue;

		CSample() {}
		CSample(float key, const ValueType &value) : m_fKey( key ), m_value( value ) {}
		bool operator < (const CSample &sample) const	{ return m_fKey < sample.m_fKey; }
		bool operator > (const CSample &sample) const	{ return m_fKey > sample.m_fKey; }
		CSample &operator=(const CSample &other)
		{
			m_fKey = other.m_fKey;
			m_value = other.m_value;
			m_valuePrecompute = other.m_valuePrecompute;
			return *this;
		}
	};
	typedef std::vector<CSample>			sample_vector;
	typedef typename std::vector<CSample>::iterator	sample_iterator;
public:
	typedef	ValueType ValueType;
	inline CSampleSpline() : m_iCount( 0 ), m_bPrepared( false ) {}
	inline ~CSampleSpline() {}

	inline void Clear();

	inline int AddSample(float key, const ValueType &value);	//샘플을 추가한다.
	inline int RemoveSample(float keyLower, float keyUpper);	//일정 키범위를 삭제한다.
	inline int RemoveSample(const ValueType &value);	//특정 value를 갖는다면 삭제한다.
	inline int RemoveSample(const int index);

	inline float GetKey(int index);
	inline const ValueType &GetValue(int index);
	inline int GetSampleCount()		{ return m_iCount;	}

	inline void Prepare();
	inline const ValueType Value(float key);

	inline CSampleSpline<ValueType, InterpolateFunc> &operator=(const CSampleSpline<ValueType, InterpolateFunc> &other)
	{
		m_vectorSample = other.m_vectorSample;
		m_bPrepared = other.m_bPrepared;
		m_iCount = other.m_iCount;
		m_interpolateFunc = other.m_interpolateFunc;
		return *this;
	}
private:
	//content
	sample_vector	m_vectorSample;
	//instance
	bool			m_bPrepared;
	int				m_iCount;
	InterpolateFunc m_interpolateFunc;
};

template<class ValueType, class InterpolateFunc>
inline void CSampleSpline<ValueType, InterpolateFunc>::Clear()
{
	m_vectorSample.clear();
	m_bPrepared = false;
	m_iCount = 0;
}

template<class ValueType, class InterpolateFunc>
int CSampleSpline<ValueType, InterpolateFunc>::AddSample(float key, const ValueType &value)
{
	if(m_vectorSample.capacity() <= m_vectorSample.size() + 1) m_vectorSample.reserve(m_vectorSample.size() * 2);
	m_bPrepared = false;
	CSample sample(key, value);
	sample_iterator iter = std::upper_bound(m_vectorSample.begin(), m_vectorSample.end(), sample);
	if( m_iCount > 0 && m_vectorSample.begin() != iter && (*(iter-1)).m_fKey == key ) return -1;
	int ret = std::distance(m_vectorSample.begin(), iter);
	m_vectorSample.insert(iter, sample);
	++m_iCount;
	return ret;
}

template<class ValueType, class InterpolateFunc>
int CSampleSpline<ValueType, InterpolateFunc>::RemoveSample(float keyLower, float keyUpper)
{
	if(keyLower >= keyUpper) return 0;
	if(m_vectorSample.empty()) return 0;
	m_bPrepared = false;
	CSample sample( keyLower, GetValue(0) );
	sample_iterator iterLower = std::lower_bound(m_vectorSample.begin(), m_vectorSample.end(), sample);
	sample.m_fKey = keyUpper;
	sample_iterator iterUpper = std::upper_bound(iterLower, m_vectorSample.end(), sample);
	unsigned int ret = iterUpper - iterLower;
	m_vectorSample.erase( iterLower, iterUpper );
	m_iCount = m_vectorSample.size();
	return ret;
}

template<class ValueType, class InterpolateFunc>
int CSampleSpline<ValueType, InterpolateFunc>::RemoveSample(const ValueType &value)
{
	if(m_vectorSample.empty()) return 0;
	m_bPrepared = false;
	sample_vector vectorNonRemoved;
	for(unsigned int i=0; i<m_iCount; ++i)
	{
		if(m_vectorSample[i].m_value != value) vectorNonRemoved.push_back(m_vectorSample[i]);
	}
	m_vectorSample.swap(vectorNonRemoved);
	m_iCount = m_vectorSample.size();
	return vectorNonRemoved.size() - m_iCount;
}

template<class ValueType, class InterpolateFunc>
int CSampleSpline<ValueType, InterpolateFunc>::RemoveSample(const int index)
{
	if(m_vectorSample.empty()) return 0;
	if(m_vectorSample.size() <= index) return 0;
	m_bPrepared = false;
	m_vectorSample.erase( m_vectorSample.begin() + index );
	m_iCount = m_vectorSample.size();
	return 1;
}

template<class ValueType, class InterpolateFunc>
float CSampleSpline<ValueType, InterpolateFunc>::GetKey(int index)
{
	return m_vectorSample[index].m_fKey;
}

template<class ValueType, class InterpolateFunc>
const ValueType &CSampleSpline<ValueType, InterpolateFunc>::GetValue(int index)
{
	return m_vectorSample[index].m_value;
}

//template의 특성을 이용함. 사용하지 않으면 코드가 생성되어 컴파일 되지 않는다.
//혹은 컴파일러의 최적화 기능을 믿는다. 실제 사용되지 않는 임시변수의 제거.
template<class ValueType, class InterpolateFunc>
void CSampleSpline<ValueType, InterpolateFunc>::Prepare()
{
	m_bPrepared = true;
}

//Natural Cubic이외의 것에서는 안쓰임.
template<>
void CSampleSpline< FLOAT3D, CCubicSplineInterpolate<FLOAT3D> >::Prepare()
{
	if(m_bPrepared) return;
	m_bPrepared = true;
	
	unsigned int count = m_vectorSample.size();
	std::vector<ValueType> vectorValue;
	vectorValue.resize(count);
	std::vector<ValueType> vectorPrecomputeValue;
	vectorPrecomputeValue.resize(count);

	unsigned int i;
	for(i=0; i<count; ++i)
	{
		vectorValue[i] = m_vectorSample[i].m_value;
	}

	m_interpolateFunc.Prepare(count, vectorValue, vectorPrecomputeValue);

	for(i=0; i<count; ++i)
	{
		m_vectorSample[i].m_valuePrecompute = vectorPrecomputeValue[i];
	}
}

//기본은 Lerp임.
template<class ValueType, class InterpolateFunc>
const ValueType CSampleSpline< ValueType, InterpolateFunc >::Value(float key)
{
	if(m_iCount == 0) return ValueType();

	if(key <= m_vectorSample[0].m_fKey) return m_vectorSample[0].m_value;
	else if(key >= m_vectorSample[m_iCount-1].m_fKey) return m_vectorSample[m_iCount-1].m_value;
	else
	{
		unsigned int i;
		for(i = 0; i < m_iCount; ++i)
			if(m_vectorSample[i].m_fKey >= key)
				break;
		ASSERT(i > 0 && i <= m_iCount-1);
		unsigned int  uiCurrent = i-1;
		unsigned int  uiNext = i;
		float ratio = (key - m_vectorSample[uiCurrent].m_fKey)
					/ (m_vectorSample[uiNext].m_fKey - m_vectorSample[uiCurrent].m_fKey);
		return m_interpolateFunc( m_vectorSample[uiCurrent].m_value, m_vectorSample[uiNext].m_value, ratio);
	}
}

//현재 Natural Cubic Spline은 FLOAT3D하고만 같이 쓰임.
template<>
const FLOAT3D CSampleSpline< FLOAT3D, CCubicSplineInterpolate<FLOAT3D> >::Value(float key)
{
	if(m_iCount == 0) return ValueType();

	if(key <= m_vectorSample[0].m_fKey) return m_vectorSample[0].m_value;
	else if(key >= m_vectorSample[m_iCount-1].m_fKey) return m_vectorSample[m_iCount-1].m_value;
	else
	{
		unsigned int i;
		for(i = 0; i < m_iCount; ++i)
			if(m_vectorSample[i].m_fKey >= key)
				break;
		ASSERT(i > 0 && i <= m_iCount-1);
		unsigned int  uiCurrent = i-1;
		unsigned int  uiNext = i;
		float ratio = (key - m_vectorSample[uiCurrent].m_fKey)
					/ (m_vectorSample[uiNext].m_fKey - m_vectorSample[uiCurrent].m_fKey);
		return m_interpolateFunc( m_vectorSample[uiCurrent].m_value, m_vectorSample[uiNext].m_value
								, m_vectorSample[uiCurrent].m_valuePrecompute, m_vectorSample[uiNext].m_valuePrecompute
								, ratio);
	}
}

//현재 Hermite Spline은 FLOAT3D하고만 같이 쓰임.
template<>
const FLOAT3D CSampleSpline< FLOAT3D, CHermiteSplineInterpolate<FLOAT3D> >::Value(float key)
{
	if(m_iCount == 0) return ValueType();

	if(key <= m_vectorSample[0].m_fKey) return m_vectorSample[0].m_value;
	else if(key >= m_vectorSample[m_iCount-1].m_fKey) return m_vectorSample[m_iCount-1].m_value;
	else
	{
		unsigned int i;
		for(i = 0; i < m_iCount; ++i)
			if(m_vectorSample[i].m_fKey >= key)
				break;
		ASSERT(i > 0 && i <= m_iCount-1);
		unsigned int  uiCurrent = i-1;
		unsigned int  uiNext = i;
		float ratio = (key - m_vectorSample[uiCurrent].m_fKey)
					/ (m_vectorSample[uiNext].m_fKey - m_vectorSample[uiCurrent].m_fKey);
		unsigned int  uiBeforeCurrent = uiCurrent - 1;
		if(uiCurrent == 0)//0 ~ 1사이
		{
			return Lerp(m_vectorSample[uiCurrent].m_value, m_vectorSample[uiNext].m_value, ratio);
			//uiBeforeCurrent = 0;
		}
		unsigned int  uiAfterNext = uiNext + 1;
		if(uiAfterNext > m_iCount-1)//m_iCount-2 ~ m_iCount-1사이
		{
			return Lerp(m_vectorSample[uiCurrent].m_value, m_vectorSample[uiNext].m_value, ratio);
			//uiBeforeCurrent = m_iCount-1;
		}
		return m_interpolateFunc( m_vectorSample[uiCurrent].m_value, m_vectorSample[uiNext].m_value
								, m_vectorSample[uiBeforeCurrent].m_value, m_vectorSample[uiAfterNext].m_value
								, ratio);
	}
}

#endif //__CSAMPLESPLINE_H__
//안태훈 수정 끝	//(Remake Effect)(0.1)
