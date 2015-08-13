//안태훈 수정 시작	//(Remake Effect)(0.1)

#include "stdH.h"

#include "CRandomTable.h"
#include "CParticlesProcessDynamicState.h"
#include <Engine/Base/Memory.h>

//NEW_DELETE_IMPLEMENT(CParticlesProcessDynamicState);

CParticlesProcessDynamicState::CParticlesProcessDynamicState()
: m_bDynamicColor( FALSE )
, m_bDynamicAlpha( FALSE )
, m_bDynamicTexPos( FALSE )
, m_bDynamicSize( FALSE )
, m_bDynamicMass( FALSE )
, m_bDynamicDeltaPos( FALSE )
, m_bDynamicAngle( FALSE )
, m_fFadeInTime( 0 )
, m_fFadeOutTime( 0 )
, m_fTwinklePeriod( 0 )
{
	m_eType = PCPT_DYNAMIC_STATE;
}

CParticlesProcessDynamicState::~CParticlesProcessDynamicState()
{
}

CParticlesCommonProcess *CParticlesProcessDynamicState::Copy()
{
	CParticlesProcessDynamicState *pPPDS = new CParticlesProcessDynamicState;
	pPPDS->m_bDynamicColor = m_bDynamicColor;
	pPPDS->m_ssColor = m_ssColor;
	pPPDS->m_bDynamicAlpha = m_bDynamicAlpha;
	pPPDS->m_ssAlpha = m_ssAlpha;
	pPPDS->m_bDynamicTexPos = m_bDynamicTexPos;
	pPPDS->m_ssTexPos = m_ssTexPos;
	pPPDS->m_fTwinklePeriod = m_fTwinklePeriod;
	pPPDS->m_fFadeInTime = m_fFadeInTime;
	pPPDS->m_fFadeOutTime = m_fFadeOutTime;
	pPPDS->m_bDynamicSize = m_bDynamicSize;
	pPPDS->m_ssSize = m_ssSize;
	pPPDS->m_bDynamicMass = m_bDynamicMass;
	pPPDS->m_ssMass = m_ssMass;
	pPPDS->m_bDynamicDeltaPos = m_bDynamicDeltaPos;
	pPPDS->m_ssDeltaPos = m_ssDeltaPos;
	pPPDS->m_bDynamicAngle = m_bDynamicAngle;
	pPPDS->m_ssAngle = m_ssAngle;
	return pPPDS;
}

void CParticlesProcessDynamicState::Process(CParticles &particles, const FLOAT fDeltaTime)
{
	if(fDeltaTime == 0.0f) return;
	UINT count = particles.Size();

	for(UINT i=0; i<count; ++i)
	{
		CParticle &ptc = particles[i];

		COLOR ptcColor = C_WHITE;
		COLOR ptcAlpha = CT_OPAQUE;
		CParticleSize size(1.0f, 1.0f);
		CParticleTexPos texPos(0,0);
		FLOAT mass = 1.0f;
		FLOAT3D vDeltaPos;
		ANGLE3D a3dAngle;

		//color 처리, 있다면 기존 컬러 무시
		if(m_bDynamicColor)
			ptcColor = m_ssColor.Value(ptc.GetAge());
		else ptcColor = ptc.GetColor() & C_WHITE;
		ptc.SetColorNoAlpha(ptcColor);
		//알파 처리, 있다면 기존 알파 무시
		BOOL alphaProcess = FALSE;
		if(m_bDynamicAlpha)
		{
			alphaProcess = TRUE;
			FLOAT life = m_ssAlpha.GetKey(m_ssAlpha.GetSampleCount()-1) - m_ssAlpha.GetKey(0);
			INDEX cnt = 0;
			
			if ( life > 0.0f || life < 0.0f ) {
				cnt = INDEX(ptc.GetAge() / life);
			}

			if(m_ssAlpha.GetSampleCount() <= 1) cnt = 0;
			ptcAlpha = m_ssAlpha.Value(ptc.GetAge() - cnt * life);
		}
		//twinkle 처리
		FLOAT fTwinkleValue = 1.0f;
		if(m_fTwinklePeriod > 0.0001f)
		{
			alphaProcess = TRUE;
			int value = (int)floorf(ptc.GetAge() / m_fTwinklePeriod);
			if(value & 0x00000001) fTwinkleValue = ( ptc.GetAge() - m_fTwinklePeriod * value ) / m_fTwinklePeriod;
			else fTwinkleValue = ( m_fTwinklePeriod * (value + 1) - ptc.GetAge() ) / m_fTwinklePeriod;
		}
		//fade 처리
		FLOAT fFadeValue = 1.0f;
		if(ptc.GetAge() < m_fFadeInTime)
		{
			alphaProcess = TRUE;
			fFadeValue = ptc.GetAge() / m_fFadeInTime;
		}
		else if(ptc.GetLifeTime() - ptc.GetAge() < m_fFadeOutTime)
		{
			alphaProcess = TRUE;
			fFadeValue = (ptc.GetLifeTime() - ptc.GetAge()) / m_fFadeOutTime;
		}
		//alpha 세팅
		if(alphaProcess)
		{
			ptcAlpha = UBYTE(ptcAlpha * (fTwinkleValue * fFadeValue));
		}
		else ptcAlpha = ptc.GetColor() & CT_OPAQUE;
		ptc.SetColorOnlyAlpha(ptcAlpha);
		//TexPos 처리
		if(m_bDynamicTexPos)
		{
			FLOAT len = m_ssTexPos.GetKey(m_ssTexPos.GetSampleCount()-1) - m_ssTexPos.GetKey(0);
			FLOAT cnt = INDEX(ptc.GetAge() / len);
			if(m_ssTexPos.GetSampleCount() <= 1) cnt = 0;
			//texPos = m_ssTexPos.Value(ptc.GetAge() - cnt * len);
			FLOAT key = ptc.GetAge() - cnt * len;
			int i;
			for(i = 0; i < m_ssTexPos.GetSampleCount(); ++i)
				if(m_ssTexPos.GetKey(i) >= key)
					break;
			texPos = m_ssTexPos.GetValue(i);
			ptc.SetTexPos(texPos.GetRow(), texPos.GetCol());
		}
		//size 처리
		if(m_bDynamicSize)
		{
			FLOAT life = m_ssSize.GetKey(m_ssSize.GetSampleCount()-1) - m_ssSize.GetKey(0);
			INDEX cnt = INDEX(ptc.GetAge() / life);
			if(m_ssSize.GetSampleCount() <= 1) cnt = 0;
			size = m_ssSize.Value(ptc.GetAge() - cnt * life);
			ptc.SetWidth(size.GetWidth());
			ptc.SetHeight(size.GetHeight());
		}
		//Mass 처리
		if(m_bDynamicMass)
		{
			mass = m_ssMass.Value(ptc.GetAge());
			ptc.SetMass(mass);
		}
		//DeltaPos 처리
		if(m_bDynamicDeltaPos)
		{
			vDeltaPos = ptc.GetPosition()
				+ m_ssDeltaPos.Value(ptc.GetAge())
				- m_ssDeltaPos.Value(ptc.GetAge()-fDeltaTime);
			ptc.SetPosition( vDeltaPos );
		}
		//Angle 처리
		if(m_bDynamicAngle)
		{
			FLOATquat3D quatAngle;
			ANGLE3D angle = m_ssAngle.Value(ptc.GetAge()) - m_ssAngle.Value(ptc.GetAge()-fDeltaTime);
			quatAngle.FromEuler(angle);
			ptc.SetDirection( ptc.GetDirection() * quatAngle );
		}
	}
}

#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 5

void CParticlesProcessDynamicState::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;

	is.ExpectID_t("PPDS");
	UBYTE ubVer;
	is >> ubVer;

#	define ReadFromStream(Stream, Name, NameType) \
	Stream >> m_bDynamic##Name;\
	if(m_bDynamic##Name)\
	{\
		DWORD count;\
		float key;\
		is >> count;\
		for(DWORD i=0; i<count; ++i)\
		{\
			NameType nameValue;\
			is >> key;\
			is >> nameValue;\
			m_ss##Name.AddSample(key, nameValue);\
		}\
	}
	if(ubVer == CURRENT_VERSION)
	{
		is >> m_fTwinklePeriod;
		is >> m_fFadeInTime;
		is >> m_fFadeOutTime;

		ReadFromStream(is, Color, COLOR);
		ReadFromStream(is, Alpha, UBYTE);
		ReadFromStream(is, TexPos, CParticleTexPos);
		ReadFromStream(is, Size, CParticleSize);
		ReadFromStream(is, Mass, FLOAT);
		ReadFromStream(is, DeltaPos, FLOAT3D);
		ReadFromStream(is, Angle, FLOAT3D);
	}
	else if(ubVer == 4)
	{
		is >> m_fTwinklePeriod;
		is >> m_fFadeInTime;
		is >> m_fFadeOutTime;

		ReadFromStream(is, Color, COLOR);
		ReadFromStream(is, Alpha, UBYTE);
		ReadFromStream(is, TexPos, CParticleTexPos);
		ReadFromStream(is, Size, CParticleSize);
		ReadFromStream(is, Mass, FLOAT);
		ReadFromStream(is, DeltaPos, FLOAT3D);
		//ReadFromStream(is, Angle, FLOATquat3D);
		is >> m_bDynamicAngle;
		if(m_bDynamicAngle)
		{
			DWORD count;
			is >> count;
			for(DWORD i=0; i<count; ++i)
			{
				FLOATquat3D quatAngle;
				FLOAT key;
				is >> key;
				is >> quatAngle;
				FLOAT3D vAxis;
				FLOAT angle;
				quatAngle.ToAxisAngle(vAxis, angle);
				ANGLE3D a3dAngle( AngleRad(angle), 0, 0);
				m_ssAngle.AddSample(key, a3dAngle);
			}
		}
	}
	else if(ubVer == 3)
	{
		is >> m_fTwinklePeriod;
		is >> m_fFadeInTime;
		is >> m_fFadeOutTime;

		ReadFromStream(is, Color, COLOR);
		ReadFromStream(is, Alpha, UBYTE);
		ReadFromStream(is, TexPos, CParticleTexPos);
		ReadFromStream(is, Size, CParticleSize);
		ReadFromStream(is, Mass, FLOAT);
		ReadFromStream(is, DeltaPos, FLOAT3D);
	}
	else if(ubVer == 2)
	{
		FLOAT fRandom;
		is >> fRandom;
		BOOL bCombineColorAndAlpha;
		is >> bCombineColorAndAlpha;
		is >> m_fFadeInTime;
		is >> m_fFadeOutTime;
		is >> m_bDynamicColor;

		DWORD count;
		float key;
		if(m_bDynamicColor)
		{
			is >> count;
			for(DWORD i=0; i<count; ++i)
			{
				COLOR color;
				is >> key;
				is >> color;
				m_ssColor.AddSample(key, color);
			}
		}
		is >> m_bDynamicAlpha;
		if(m_bDynamicAlpha)
		{
			is >> count;
			for(DWORD i=0; i<count; ++i)
			{
				UBYTE alpha;
				is >> key;
				is >> alpha;
				m_ssAlpha.AddSample(key, alpha);
			}
		}
		is >> m_bDynamicTexPos;
		if(m_bDynamicTexPos)
		{
			is >> count;
			for(DWORD i=0; i<count; ++i)
			{
				UBYTE row, col;
				is >> key;
				is >> row;
				is >> col;
				m_ssTexPos.AddSample(key, CParticleTexPos(row, col));
			}
		}
		is >> m_bDynamicSize;
		if(m_bDynamicSize)
		{
			is >> count;
			for(DWORD i=0; i<count; ++i)
			{
				float width, height;
				is >> key;
				is >> width;
				is >> height;
				m_ssSize.AddSample(key, CParticleSize(width, height));
			}
		}
		is >> m_bDynamicMass;
		if(m_bDynamicMass)
		{
			is >> count;
			for(DWORD i=0; i<count; ++i)
			{
				float mass;
				is >> key;
				is >> mass;
				m_ssMass.AddSample(key, mass);
			}
		}
		is >> m_bDynamicDeltaPos;
		if(m_bDynamicDeltaPos)
		{
			is >> count;
			for(DWORD i=0; i<count; ++i)
			{
				FLOAT3D vDeltaPos;
				is >> key;
				is >> vDeltaPos;
				m_ssDeltaPos.AddSample(key, vDeltaPos);
			}
		}
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CParticlesProcessDynamicState::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;

	os.WriteID_t("PPDS");
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
	WriteToStream(os, TexPos);
	WriteToStream(os, Size);
	WriteToStream(os, Mass);
	WriteToStream(os, DeltaPos);
	WriteToStream(os, Angle);
}

//안태훈 수정 끝	//(Remake Effect)(0.1)
