#ifndef __CENTITYEFFECT_H__
#define __CENTITYEFFECT_H__

#include "CEffect.h"

class CEntity;
class CEffectControl;
class CModelInstance;

extern ENGINE_API BOOL g_bEntityEffectTestMode;

class ENGINE_API CEntityEffect : public CEffect
{
public:
public:
	CEntityEffect();
	virtual ~CEntityEffect();

	//NEW_DELETE_DEFINITION(CFlameEffect);

	virtual void Read(CTStream *pIS);
	virtual void Write(CTStream *pOS);
	
	virtual void Start(FLOAT time, BOOL restart = FALSE);
	virtual BOOL Process(FLOAT time);
	virtual CEffect *Copy();
	
	void SetEffectControl(const CEffectControl &effectControl);
	CEffectControl *GetEffectControl()	{ return m_pEffectControl;	}
	void EraseEffectControl();

	void SetTestValue(CModelInstance *pMI)
	{
		m_pMIForTest = pMI;
	}

protected:
	CEntityEffect(const CEntityEffect& other) {}
	CEntityEffect &operator =(const CEntityEffect &other) { return *this; }
protected:
	//contents
	CEffectControl		*m_pEffectControl;		//effect control 없을때는 NULL
	//instance
	COLOR	m_colModelOriginal;
	FLOAT3D m_vStretchOriginal;
	CModelInstance *m_pMIForTest;
};

#endif //__CENTITYEFFECT_H__
