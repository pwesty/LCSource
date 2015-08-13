//안태훈 수정 시작	//(Remake Effect)(0.1)

#ifndef _PARTICLEGROUP_H_
#define _PARTICLEGROUP_H_
#ifdef PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Base/Memory.h>
#include <Engine/Base/Types.h>
#include <Engine/Graphics/Texture.h>
#include <Engine/Graphics/Gfx_wrapper.h>
#include <vector>
#include <list>
#include <string>
#include "CParticle.h"
#include "CParticles.h"
#include "CParticlesCommonProcess.h"
#include "CParticlesEmitter.h"
#include "CParticlesAbsorption.h"
#include "FreeMemStack.h"

class ENGINE_API CParticleGroup
{
protected:
	typedef std::vector<CParticlesCommonProcess*> vector_common_process;
public:
	// Constructor & destructor
	CParticleGroup();
	~CParticleGroup();

	//NEW_DELETE_DEFINITION(CParticleGroup);

//안태훈 수정 시작	//(Remake Effect)(0.1)
	void Read(CTStream *istrFile);
	void Write(CTStream *ostrFile);
//안태훈 수정 끝	//(Remake Effect)(0.1)

	// Main functions
	CParticleGroup *Copy();
	BOOL Process( FLOAT fTimeDelta );
	void Stop();
	void Render();

	inline void Restart()	{ if(m_pParticlesEmitter) m_pParticlesEmitter->Restart(); }

	// Access functions
	inline const std::string &GetName()			{ return m_strName; }
	inline void SetName(std::string &name)		{ m_strName = name; }
	inline void SetName(const char *name)		{ m_strName = name; }

	inline void SetRenderType(PARTICLE_RENDER_TYPE type)
	{
		m_eRenderType = type;
		if(m_pParticlesEmitter) m_pParticlesEmitter->SetParticleType(m_eRenderType);
	}
	inline BOOL GetRenderType()								{ return m_eRenderType;	}

	inline void SetBlendType(PredefinedBlendType blendType)	{ m_eBlendType = blendType; }
	inline PredefinedBlendType GetBlendType()				{ return m_eBlendType;		}

	inline void SetTextureInfo(MEX width, MEX height, INDEX col, INDEX row)
	{
		m_mexWidth = width;
		m_mexHeight = height;
		m_iCol = col;
		m_iRow = row;
	}
	inline MEX GetTexInfoMexWidth()		{ return m_mexWidth;	}
	inline MEX GetTexInfoMexHeight()	{ return m_mexHeight;	}
	inline INDEX GetTexInfoColumn()		{ return m_iCol;		}
	inline INDEX GetTexInfoRow()		{ return m_iRow;		}

	void SetTexture(const CTFileName &filename);
	const CTFileName GetTexFileName()	{ return CTFileName(m_fnTexture.c_str()); }

	//process관련
	inline void SetPosition(const FLOAT3D &vPos)
	{
		if(m_pParticlesEmitter) m_pParticlesEmitter->SetPosition(vPos);
		if(m_pParticlesAbsorption) m_pParticlesAbsorption->SetPosition(vPos);
		for(UINT i=0; i<m_vectorCommonProcess.size(); ++i) m_vectorCommonProcess[i]->SetPosition(vPos);
	}

	inline void SetDirection(const FLOATquat3D &quatDir)
	{
		if(m_pParticlesEmitter) m_pParticlesEmitter->SetDirection(quatDir);
		if(m_pParticlesAbsorption) m_pParticlesAbsorption->SetDirection(quatDir);
		for(UINT i=0; i<m_vectorCommonProcess.size(); ++i) m_vectorCommonProcess[i]->SetDirection(quatDir);
	}
	
	inline void AddCommonProcess(CParticlesCommonProcess *pPCP) { m_vectorCommonProcess.push_back(pPCP->Copy());	}
	inline void RemoveCommonProcess(UINT index)
	{
		delete m_vectorCommonProcess[index];
		m_vectorCommonProcess.erase( m_vectorCommonProcess.begin() + index);
	}
	inline void SwapCommonProcess(UINT index1, UINT index2)
	{
		if(index1 == index2) return;
		ASSERT(index1 >= 0 && index2 >= 0 && index1 < m_vectorCommonProcess.size() && index2 < m_vectorCommonProcess.size());
		CParticlesCommonProcess *pTemp1 = *(m_vectorCommonProcess.begin() + index1);
		CParticlesCommonProcess *pTemp2 = *(m_vectorCommonProcess.begin() + index2);
		CParticlesCommonProcess *pTemp = pTemp1;
		pTemp1 = pTemp2;
		pTemp2 = pTemp;
	}
	inline CParticlesCommonProcess *GetCommonProcess(UINT index)	{ return m_vectorCommonProcess[index];	}
	inline UINT GetCommonProcessCount()		{ return m_vectorCommonProcess.size(); }

	inline void SetEmitter(CParticlesEmitter *pEmitter)
	{
		delete m_pParticlesEmitter;
		m_pParticlesEmitter = pEmitter->Copy();
		if(m_pParticlesEmitter) m_pParticlesEmitter->SetParticleType(m_eRenderType);
	}
	inline CParticlesEmitter *GetEmitter() { return m_pParticlesEmitter;	}
	inline FLOAT GetMaxParticleLifeTime()
	{
		if(m_pParticlesEmitter != NULL) return m_pParticlesEmitter->GetParticlePrototype().GetMaxLifeTime();
		else return 0.0f;
	}

	inline void SetAbsorption(CParticlesAbsorption *pAbsorption)
	{ delete m_pParticlesAbsorption; m_pParticlesAbsorption = pAbsorption->Copy();	}
	
	inline CParticlesAbsorption *GetAbsorption() { return m_pParticlesAbsorption;	}

protected:
	// Content variable
	std::string		m_strName;
	std::string		m_fnTexture;
	PARTICLE_RENDER_TYPE	m_eRenderType;
	PredefinedBlendType		m_eBlendType;
	MEX		m_mexWidth;
	MEX		m_mexHeight;
	INDEX	m_iCol;
	INDEX	m_iRow;
	vector_common_process	m_vectorCommonProcess;
	CParticlesEmitter		*m_pParticlesEmitter;
	CParticlesAbsorption	*m_pParticlesAbsorption;

	// Instance variable
	CParticles		m_particles;		//파티클들
	CTextureObject	m_toTexture;		//파티클의 텍스쳐
};

#endif // _PARTICLEGROUP_H_

//안태훈 수정 끝	//(Remake Effect)(0.1)
