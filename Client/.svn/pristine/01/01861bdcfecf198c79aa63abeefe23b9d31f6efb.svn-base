//안태훈 수정 시작	//(Remake Effect)(0.1)
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)

#include "stdH.h"

#include "CParticle.h"

//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)

#include <Engine/Base/Stream.h>
#include <Engine/Base/Memory.h>

//-------------- CParticle -------------//
/*
#define CURRENT_VERSION 1

void CParticle::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;

	is.ExpectID_t("PATC");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		is >> m_dwFlag;
		is >> m_fLifeTime;
		is >> m_fAge;
		is >> m_fWidth;
		is >> m_fHeight;
		is >> m_ubRow;
		is >> m_ubCol;
		is >> m_fMass;
		is >> m_colColor;
		is >> m_vPosition;
		is >> m_quatDirection.q_w;
		is >> m_quatDirection.q_x;
		is >> m_quatDirection.q_y;
		is >> m_quatDirection.q_z;
		is >> m_vVelocity;
		is >> m_vAcceleration;
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CParticle::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;

	os.WriteID_t("PATC");
	os << (UBYTE)CURRENT_VERSION;

	os << m_dwFlag;
	os << m_fLifeTime;
	os << m_fAge;
	os << m_fWidth;
	os << m_fHeight;
	os << m_ubRow;
	os << m_ubCol;
	os << m_fMass;
	os << m_colColor;
	os << m_vPosition;
	os << m_quatDirection.q_w;
	os << m_quatDirection.q_x;
	os << m_quatDirection.q_y;
	os << m_quatDirection.q_z;
	os << m_vVelocity;
	os << m_vAcceleration;
}

#undef CURRENT_VERSION
*/

//-------------- CParticlePrototype -------------//
#define CURRENT_VERSION 1

void CParticlePrototype::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;

	is.ExpectID_t("PTPT");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		is >> m_particleLower.m_fLifeTime;
		is >> m_particleUpper.m_fLifeTime;
		is >> m_particleLower.m_fWidth;
		is >> m_particleUpper.m_fWidth;
		is >> m_particleLower.m_fHeight;
		is >> m_particleUpper.m_fHeight;
		is >> m_particleLower.m_ubRow;
		is >> m_particleLower.m_ubCol;
		is >> m_particleLower.m_fMass;
		is >> m_particleUpper.m_fMass;
		is >> m_particleLower.m_colColor;
		is >> m_particleUpper.m_colColor;
		//pos 생략
		is >> m_particleLower.m_quatDirection.q_w;
		is >> m_particleUpper.m_quatDirection.q_w;
		is >> m_particleLower.m_quatDirection.q_x;
		is >> m_particleUpper.m_quatDirection.q_x;
		is >> m_particleLower.m_quatDirection.q_y;
		is >> m_particleUpper.m_quatDirection.q_y;
		is >> m_particleLower.m_quatDirection.q_z;
		is >> m_particleUpper.m_quatDirection.q_z;
		//velocity, acceleration 생략
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CParticlePrototype::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;

	os.WriteID_t("PTPT");
	os << (UBYTE)CURRENT_VERSION;

	os << m_particleLower.m_fLifeTime;
	os << m_particleUpper.m_fLifeTime;
	os << m_particleLower.m_fWidth;
	os << m_particleUpper.m_fWidth;
	os << m_particleLower.m_fHeight;
	os << m_particleUpper.m_fHeight;
	os << m_particleLower.m_ubRow;
	os << m_particleLower.m_ubCol;
	os << m_particleLower.m_fMass;
	os << m_particleUpper.m_fMass;
	os << m_particleLower.m_colColor;
	os << m_particleUpper.m_colColor;
	//pos 생략
	os << m_particleLower.m_quatDirection.q_w;
	os << m_particleUpper.m_quatDirection.q_w;
	os << m_particleLower.m_quatDirection.q_x;
	os << m_particleUpper.m_quatDirection.q_x;
	os << m_particleLower.m_quatDirection.q_y;
	os << m_particleUpper.m_quatDirection.q_y;
	os << m_particleLower.m_quatDirection.q_z;
	os << m_particleUpper.m_quatDirection.q_z;
	//velocity, acceleration 생략
}



#undef CURRENT_VERSION

//-------------- CConeSpace -------------//
#define CURRENT_VERSION 2
#include "CConeSpace.h"

void CConeSpace::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;

	is.ExpectID_t("CNSP");

	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		is >> m_fHeightUpper;
		is >> m_fHeightLower;
		is >> m_fAngle;
		is >> m_vCenter;
		is >> m_fCloseRange;
		is >> m_fLerpRatio;
	}
	else if(ubVer == 1)
	{
		is >> m_fHeightUpper;
		is >> m_fHeightLower;
		is >> m_fAngle;
		is >> m_vCenter;
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CConeSpace::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;

	os.ExpectID_t("CNSP");
	os << (UBYTE)CURRENT_VERSION;

	os << m_fHeightUpper;
	os << m_fHeightLower;
	os << m_fAngle;
	os << m_vCenter;
	os << m_fCloseRange;
	os << m_fLerpRatio;
}

#undef CURRENT_VERSION

//-------------- CConeDoubleSpace -------------//
#define CURRENT_VERSION 2

void CConeDoubleSpace::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;

	is.ExpectID_t("CDSP");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		is >> m_fHeightUpper;
		is >> m_fHeightLower;
		is >> m_fHotspot;
		is >> m_fFalloff;
		is >> m_vCenter;
		is >> m_fCloseRange;
		is >> m_fLerpRatio;
	}
	//old version
	else if(ubVer == 1)
	{
		is >> m_fHeightUpper;
		is >> m_fHeightLower;
		is >> m_fHotspot;
		is >> m_fFalloff;
		is >> m_vCenter;
	}
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CConeDoubleSpace::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;

	os.WriteID_t("CDSP");
	os << (UBYTE)CURRENT_VERSION;

	os << m_fHeightUpper;
	os << m_fHeightLower;
	os << m_fHotspot;
	os << m_fFalloff;
	os << m_vCenter;
	os << m_fCloseRange;
	os << m_fLerpRatio;
}

#undef CURRENT_VERSION

//-------------- CCylinderSpace -------------//
#define CURRENT_VERSION 1

#include "CCylinderSpace.h"

void CCylinderSpace::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;

	is.ExpectID_t("CNSP");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		is >> m_fHeight;
		is >> m_fRadius;
		is >> m_vCenter;
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CCylinderSpace::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;

	os.ExpectID_t("CNSP");
	os << (UBYTE)CURRENT_VERSION;

	os << m_fHeight;
	os << m_fRadius;
	os << m_vCenter;
}

#undef CURRENT_VERSION

//-------------- CCylinderDoubleSpace -------------//
#define CURRENT_VERSION 1

void CCylinderDoubleSpace::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;

	is.ExpectID_t("CDSP");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		is >> m_fHeight;
		is >> m_fRadiusInner;
		is >> m_fRadiusOuter;
		is >> m_vCenter;
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CCylinderDoubleSpace::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;

	os.WriteID_t("CDSP");
	os << (UBYTE)CURRENT_VERSION;

	os << m_fHeight;
	os << m_fRadiusInner;
	os << m_fRadiusOuter;
	os << m_vCenter;
}

#undef CURRENT_VERSION

//-------------- CForce -------------//
#define CURRENT_VERSION 1

#include "CForce.h"

void CForce::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;

	is.ExpectID_t("FOCE");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		is >> (DWORD&)m_ftType;
		is >> m_fPower;
		is >> m_vPostion(1);
		is >> m_vPostion(2);
		is >> m_vPostion(3);
		is >> m_vDirection(1);
		is >> m_vDirection(2);
		is >> m_vDirection(3);
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CForce::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;

	os.WriteID_t("FOCE");
	os << (UBYTE)CURRENT_VERSION;

	os << (DWORD)m_ftType;
	os << m_fPower;
	os << m_vPostion(1);
	os << m_vPostion(2);
	os << m_vPostion(3);
	os << m_vDirection(1);
	os << m_vDirection(2);
	os << m_vDirection(3);
}

#undef CURRENT_VERSION

//-------------- CSphereSpace -------------//
#define CURRENT_VERSION 1

#include "CSphereSpace.h"

void CSphereSpace::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;

	is.ExpectID_t("SPSP");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		is >> m_fRadius;
		is >> m_vCenter(1);
		is >> m_vCenter(2);
		is >> m_vCenter(3);
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CSphereSpace::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;

	os.ExpectID_t("SPSP");
	os << (UBYTE)CURRENT_VERSION;

	os << m_fRadius;
	os << m_vCenter(1);
	os << m_vCenter(2);
	os << m_vCenter(3);
}

#undef CURRENT_VERSION

//-------------- CSphereDoubleSpace -------------//
#define CURRENT_VERSION 1

void CSphereDoubleSpace::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;

	is.ExpectID_t("SDSP");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		is >> m_fRadiusOuter;
		is >> m_fRadiusInner;
		is >> m_vCenter(1);
		is >> m_vCenter(2);
		is >> m_vCenter(3);
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CSphereDoubleSpace::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;

	os.WriteID_t("SDSP");
	os << (UBYTE)CURRENT_VERSION;

	os << m_fRadiusOuter;
	os << m_fRadiusInner;
	os << m_vCenter(1);
	os << m_vCenter(2);
	os << m_vCenter(3);
}

#undef CURRENT_VERSION

//-------------- ??? -------------//
//#define CURRENT_VERSION 1


//안태훈 수정 끝	//(Remake Effect)(0.1)