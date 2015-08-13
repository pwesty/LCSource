//안태훈 수정 시작	//(Remake Effect)(0.1)

#include "stdH.h"

#include "CRandomTable.h"
#include "CEffectControl.h"
#include <Engine/Base/Memory.h>

//NEW_DELETE_IMPLEMENT(CEffectControl);

CEffectControl::CEffectControl()
: m_bDynamicColor( FALSE )
, m_bDynamicAlpha( FALSE )
, m_bDynamicStretch( FALSE )
, m_bDynamicPos( FALSE )
, m_bDynamicAngle( FALSE )
, m_fFadeInTime( 0 )
, m_fFadeOutTime( 0 )
, m_fTwinklePeriod( 0 )
{
}

CEffectControl::~CEffectControl()
{
}

CEffectControl &CEffectControl::operator=(const CEffectControl& other)
{
	m_fTwinklePeriod = other.m_fTwinklePeriod;
	m_fFadeInTime = other.m_fFadeInTime;
	m_fFadeOutTime = other.m_fFadeOutTime;

	m_bDynamicColor = other.m_bDynamicColor;
	m_ssColor = other.m_ssColor;
	m_bDynamicAlpha = other.m_bDynamicAlpha;
	m_ssAlpha = other.m_ssAlpha;
	m_bDynamicStretch = other.m_bDynamicStretch;
	m_ssStretch = other.m_ssStretch;
	m_bDynamicPos = other.m_bDynamicPos;
	m_ssPos = other.m_ssPos;
	m_bDynamicAngle = other.m_bDynamicAngle;
	m_ssAngle = other.m_ssAngle;
	return *this;
}

void CEffectControl::Process(COLOR *pColor, FLOAT3D *pStretch, FLOAT3D *pPos, ANGLE3D *pAngle
							, const FLOAT fAge, const FLOAT fLifeTime, const FLOAT fDeltaTime )
{
	COLOR colColor = C_WHITE;
	COLOR colAlpha = CT_OPAQUE;
	FLOAT3D vDeltaPos;

	if(pColor != NULL)
	{
		//color 처리, 있다면 기존 컬러 무시
		if(m_bDynamicColor)
		{
			colColor = m_ssColor.Value(fAge);
			colColor &= C_WHITE;
		}
		else colColor = (*pColor) & C_WHITE;
		//알파 처리, 있다면 기존 알파 무시
		BOOL alphaProcess = FALSE;
		if(m_bDynamicAlpha)
		{
			alphaProcess = TRUE;
			FLOAT life = m_ssAlpha.GetKey(m_ssAlpha.GetSampleCount()-1) - m_ssAlpha.GetKey(0);
			INDEX cnt = 0;
			
			if ( life > 0.0f || life < 0.0f ) {
				cnt = INDEX(fAge / life);
			}

			if(m_ssAlpha.GetSampleCount() <= 1) cnt = 0;
			colAlpha = m_ssAlpha.Value(fAge - life * cnt);
		}
		//twinkle 처리
		FLOAT fTwinkleValue = 1.0f;
		if(m_fTwinklePeriod > 0.0001f)
		{
			alphaProcess = TRUE;
			int value = (int)floorf(fAge / m_fTwinklePeriod);
			if(value & 0x00000001) fTwinkleValue = ( fAge - m_fTwinklePeriod * value ) / m_fTwinklePeriod;
			else fTwinkleValue = ( m_fTwinklePeriod * (value + 1) - fAge ) / m_fTwinklePeriod;
		}
		//fade 처리
		FLOAT fFadeValue = 1.0f;
		if(fAge <= m_fFadeInTime && m_fFadeInTime != 0)
		{
			alphaProcess = TRUE;
			fFadeValue = fAge / m_fFadeInTime;
		}
		else if(fLifeTime - fAge <= m_fFadeOutTime && m_fFadeOutTime != 0)
		{
			alphaProcess = TRUE;
			fFadeValue = (fLifeTime - fAge) / m_fFadeOutTime;
		}
		else fFadeValue = 1.0f;
		//alpha 세팅
		if(alphaProcess)
		{
			colAlpha = UBYTE(colAlpha * (fTwinkleValue * fFadeValue));
		}
		else colAlpha = *pColor & CT_OPAQUE;
		*pColor = colColor | colAlpha;
	}
	//Stretch 처리
	if(m_bDynamicStretch && pStretch != NULL)
	{
		FLOAT life = m_ssStretch.GetKey(m_ssStretch.GetSampleCount()-1) - m_ssStretch.GetKey(0);
		INDEX cnt = INDEX(fAge / life);
		if(m_ssStretch.GetSampleCount() <= 1) cnt = 0;
		*pStretch = m_ssStretch.Value(fAge - cnt * life);
	}
	//Pos 처리
	if(m_bDynamicPos && pPos != NULL)
	{
		m_ssPos.Prepare();
		*pPos = m_ssPos.Value(fAge);
	}
	//Angle 처리
	if(m_bDynamicAngle && pAngle != NULL)
	{// ska모델의 방향이 북쪽만 향해서 임시 처리
		*pAngle = m_ssAngle.Value(fAge);
	}
}

#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 1

void CEffectControl::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;

	is.ExpectID_t("ECTL");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		is >> m_fTwinklePeriod;
		is >> m_fFadeInTime;
		is >> m_fFadeOutTime;

#		define ReadFromStream(Stream, Name, NameType) \
		Stream >> m_bDynamic##Name;\
		if(m_bDynamic##Name)\
		{\
			DWORD count;\
			float key;\
			Stream >> count;\
			for(DWORD i=0; i<count; ++i)\
			{\
				NameType nameValue;\
				Stream >> key;\
				Stream >> nameValue;\
				m_ss##Name.AddSample(key, nameValue);\
			}\
		}
		ReadFromStream(is, Color, COLOR);
		ReadFromStream(is, Alpha, UBYTE);
		ReadFromStream(is, Stretch, FLOAT3D);
		ReadFromStream(is, Pos, FLOAT3D);
		ReadFromStream(is, Angle, ANGLE3D);
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CEffectControl::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;

	os.WriteID_t("ECTL");
	os << (UBYTE)CURRENT_VERSION;

	os << m_fTwinklePeriod;
	os << m_fFadeInTime;
	os << m_fFadeOutTime;

#	define WriteToStream(Stream, Name) \
	Stream << m_bDynamic##Name;\
	if(m_bDynamic##Name)\
	{\
		Stream << (DWORD)m_ss##Name.GetSampleCount();\
		for(int i=0; i<m_ss##Name.GetSampleCount(); ++i)\
		{\
			Stream << m_ss##Name.GetKey(i);\
			Stream << m_ss##Name.GetValue(i);\
		}\
	}
	WriteToStream(os, Color);
	WriteToStream(os, Alpha);
	WriteToStream(os, Stretch);
	WriteToStream(os, Pos);
	WriteToStream(os, Angle);
}

//안태훈 수정 끝	//(Remake Effect)(0.1)
