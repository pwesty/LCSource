//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)

#include "stdH.h"

#include "CParticles.h"
#include <Engine/Base/Memory.h>
#include <Engine/Graphics/DrawPort.h>

#define MAX_PARTICLE_COUNT	100

UINT	CParticles::m_uiAllCount = 0;

UINT CParticles::AddNew(UINT wantAddCount)
{
	m_vectorParticle.resize(m_vectorParticle.size() + wantAddCount);
	return wantAddCount;	//Temp : 임시 코드, 전체 파티클 개수에 제한을 둘 필요는 분명히 있음.
	if(m_uiAllCount + wantAddCount <= MAX_PARTICLE_COUNT)
	{
		m_uiAllCount += wantAddCount;
		m_vectorParticle.resize(m_vectorParticle.size() + wantAddCount);
	}
	else
	{
		wantAddCount = MAX_PARTICLE_COUNT - m_uiAllCount;
		if(wantAddCount > 0) m_vectorParticle.resize(m_vectorParticle.size() + wantAddCount);
	}
	return wantAddCount;
}

void CParticles::RenderQuad(MEX mexWidth, MEX mexHeight)
{
	UBYTE lastRow = 0xFF, lastCol = 0xFF;
	UINT count = m_vectorParticle.size();
	UINT count2 = count >> 12;	//4096으로 나누기
	count -= count2 << 12; //4096으로 나눈 나머지
	for(UINT i2=0; i2<count2; ++i2)
	{
		for(UINT i=0; i<4096; ++i)
		{
			if(m_vectorParticle[i].Live())
			{
				if(lastRow != m_vectorParticle[i].GetTexRow() || lastCol != m_vectorParticle[i].GetTexCol())
				{
					Particle_SetTexturePart(mexWidth, mexHeight,
						m_vectorParticle[i].GetTexCol(), m_vectorParticle[i].GetTexRow());
					lastRow = m_vectorParticle[i].GetTexRow();
					lastCol = m_vectorParticle[i].GetTexCol();
				}
				m_vectorParticle[i].RenderQuad();
			}
		}
		Particle_Flush();
	}
	for(UINT i=0; i<count; ++i)
	{
		if(m_vectorParticle[i].Live())
		{
			if(lastRow != m_vectorParticle[i].GetTexRow() || lastCol != m_vectorParticle[i].GetTexCol())
			{
				Particle_SetTexturePart(mexWidth, mexHeight,
					m_vectorParticle[i].GetTexCol(), m_vectorParticle[i].GetTexRow());
				lastRow = m_vectorParticle[i].GetTexRow();
				lastCol = m_vectorParticle[i].GetTexCol();
			}
			m_vectorParticle[i].RenderQuad();
		}
	}
}

void CParticles::RenderLine(MEX mexWidth, MEX mexHeight)
{
	UBYTE lastRow = 0xFF, lastCol = 0xFF;
	UINT count = m_vectorParticle.size();
	UINT count2 = count >> 12;	//4096으로 나누기
	count -= count2 << 12; //4096으로 나눈 나머지
	for(UINT i2=0; i2<count2; ++i2)
	{
		for(UINT i=0; i<4096; ++i)
		{
			if(m_vectorParticle[i].Live())
			{
				if(lastRow != m_vectorParticle[i].GetTexRow() || lastCol != m_vectorParticle[i].GetTexCol())
				{
					Particle_SetTexturePart(mexWidth, mexHeight,
						m_vectorParticle[i].GetTexCol(), m_vectorParticle[i].GetTexRow());
					lastRow = m_vectorParticle[i].GetTexRow();
					lastCol = m_vectorParticle[i].GetTexCol();
				}
				m_vectorParticle[i].RenderLine();
			}
		}
		Particle_Flush();
	}
	for(UINT i=0; i<count; ++i)
	{
		if(m_vectorParticle[i].Live())
		{
			if(lastRow != m_vectorParticle[i].GetTexRow() || lastCol != m_vectorParticle[i].GetTexCol())
			{
				Particle_SetTexturePart(mexWidth, mexHeight,
					m_vectorParticle[i].GetTexCol(), m_vectorParticle[i].GetTexRow());
				lastRow = m_vectorParticle[i].GetTexRow();
				lastCol = m_vectorParticle[i].GetTexCol();
			}
			m_vectorParticle[i].RenderLine();
		}
	}
}

void CParticles::RenderTrail(MEX mexWidth, MEX mexHeight)
{
	UBYTE lastRow = 0xFF, lastCol = 0xFF;
	UINT count = m_vectorParticle.size();
	UINT count2 = count >> 12;	//4096으로 나누기
	count -= count2 << 12; //4096으로 나눈 나머지
	for(UINT i2=0; i2<count2; ++i2)
	{
		for(UINT i=0; i<4096; ++i)
		{
			if(m_vectorParticle[i].Live())
			{
				if(lastRow != m_vectorParticle[i].GetTexRow() || lastCol != m_vectorParticle[i].GetTexCol())
				{
					Particle_SetTexturePart(mexWidth, mexHeight,
						m_vectorParticle[i].GetTexCol(), m_vectorParticle[i].GetTexRow());
					lastRow = m_vectorParticle[i].GetTexRow();
					lastCol = m_vectorParticle[i].GetTexCol();
				}
				m_vectorParticle[i].RenderTrail();
			}
		}
		Particle_Flush();
	}
	for(UINT i=0; i<count; ++i)
	{
		if(m_vectorParticle[i].Live())
		{
			if(lastRow != m_vectorParticle[i].GetTexRow() || lastCol != m_vectorParticle[i].GetTexCol())
			{
				Particle_SetTexturePart(mexWidth, mexHeight,
					m_vectorParticle[i].GetTexCol(), m_vectorParticle[i].GetTexRow());
				lastRow = m_vectorParticle[i].GetTexRow();
				lastCol = m_vectorParticle[i].GetTexCol();
			}
			m_vectorParticle[i].RenderTrail();
		}
	}
}

void CParticles::RenderTrailVelocity(MEX mexWidth, MEX mexHeight)
{
	UBYTE lastRow = 0xFF, lastCol = 0xFF;
	UINT count = m_vectorParticle.size();
	UINT count2 = count >> 12;	//4096으로 나누기
	count -= count2 << 12; //4096으로 나눈 나머지
	for(UINT i2=0; i2<count2; ++i2)
	{
		for(UINT i=0; i<4096; ++i)
		{
			if(m_vectorParticle[i].Live())
			{
				if(lastRow != m_vectorParticle[i].GetTexRow() || lastCol != m_vectorParticle[i].GetTexCol())
				{
					Particle_SetTexturePart(mexWidth, mexHeight,
						m_vectorParticle[i].GetTexCol(), m_vectorParticle[i].GetTexRow());
					lastRow = m_vectorParticle[i].GetTexRow();
					lastCol = m_vectorParticle[i].GetTexCol();
				}
				m_vectorParticle[i].RenderTrailVelocity();
			}
		}
		Particle_Flush();
	}
	for(UINT i=0; i<count; ++i)
	{
		if(m_vectorParticle[i].Live())
		{
			if(lastRow != m_vectorParticle[i].GetTexRow() || lastCol != m_vectorParticle[i].GetTexCol())
			{
				Particle_SetTexturePart(mexWidth, mexHeight,
					m_vectorParticle[i].GetTexCol(), m_vectorParticle[i].GetTexRow());
				lastRow = m_vectorParticle[i].GetTexRow();
				lastCol = m_vectorParticle[i].GetTexCol();
			}
			m_vectorParticle[i].RenderTrailVelocity();
		}
	}
}
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)