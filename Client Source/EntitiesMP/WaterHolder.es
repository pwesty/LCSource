1500
%{
#include "StdH.h"
#define C_WHITE_OPAQUE	(C_WHITE|CT_OPAQUE)
%}

class CWaterHolder: CRationalEntity {
name      "WaterHolder";
thumbnail "Thumbnails\\Terrain.tbn";
//강동민 수정 시작 2차 작업			05.06
features  "HasName", "IsTargetable";
//강동민 수정 끝 2차 작업			05.06

properties:
	1 CTFileName m_fnModel      "Water Model file (.smc)" 'M' = CTFILENAME(""),
	2 CTFileName m_fnmWaterTex	"Water Texture"		= CTFILENAME(""),
	3 CTFileName m_fnmBumpTex	"Bump Texture"		= CTFILENAME(""),
	5 FLOAT m_fStretchAll       "StretchAll" 'S'	= 1.0f,
	6 ANGLE3D m_vStretchXYZ     "StretchXYZ" 'X'	= FLOAT3D(1.0f, 1.0f, 1.0f),
	8 BOOL		m_bColliding    "Collision" 'L'		= FALSE,    // set if model is not immatierial	
	9 BOOL		m_bActive		"Active"			= TRUE,
	10 BOOL		m_bWireFrame	"WireFrame"			= FALSE,
	12 BOOL		m_bReflection	"Reflection"		= TRUE,
	14 BOOL		m_bWave			"Wave"				= FALSE,	

	35 FLOAT	m_fBumpMat11	"BumpMatrix 11"		= 0.03f,
	36 FLOAT	m_fBumpMat12	"BumpMatrix 12"		= -0.03f,
	37 FLOAT	m_fBumpMat21	"BumpMatrix 21"		= -0.03f,
	38 FLOAT	m_fBumpMat22	"BumpMatrix 22"		= 0.03f,

	39 FLOAT	m_fTexFlowX		"TextureFlow X"		= 0.001f,
	40 FLOAT	m_fTexFlowY		"TextureFlow Y"		= 0.001f,
	61 COLOR	m_colWater		"Color of Water"	= C_WHITE_OPAQUE,
	80 CTString m_strName		"Name"				= "WaterHolder",
components:	
	40 skamodel EDITOR_WATER_MODEL "Data\\World\\Water\\WaterPlane.smc"
functions:
	void Precache(void) 
	{
		PrecacheSkaModel(EDITOR_WATER_MODEL);		
	};

//강동민 수정 시작 2차 작업			05.06
/*
	// returns bytes of memory used by this object
	SLONG GetUsedMemory(void)
	{	
		// initial
		SLONG slUsedMemory = sizeof(CWaterHolder) - sizeof(CRationalEntity) + CRationalEntity::GetUsedMemory();
		slUsedMemory += m_fnModel.Length();
		slUsedMemory += m_fnmWaterTex.Length();
		slUsedMemory += m_fnmBumpTex.Length();
		slUsedMemory += m_strName.Length();
		return slUsedMemory;
	};
	*/
//강동민 수정 끝 2차 작업			05.06

	void InitNiceWater()
	{
		if (m_fnModel=="") 
		{			
//강동민 수정 시작 접속 시퀀스 작업	06.01
			//m_fnModel=CTFILENAME("aaa\\111.smc");		// 원본.
			m_fnModel=CTFILENAME("Data\\World\\Water\\WaterPlane.smc");
//강동민 수정 끝 접속 시퀀스 작업	06.01
		}

//강동민 수정 시작 Water 구현		04.23
		if (m_fnmWaterTex == "")
		{
			m_fnmWaterTex = CTFILENAME("Data\\World\\Water\\Texture\\WaterFX.tex");
		}
//강동민 수정 끝 Water 구현			04.23

//강동민 수정 시작 Water 구현		04.20
		if (m_fnmBumpTex == "")
		{
			m_fnmBumpTex = CTFILENAME("Data\\World\\Water\\Texture\\OceanBump.tex");
		}
		/*
//강동민 수정 시작 Water 구현		04.22
		// NOTE : 애니메이션 텍스쳐 사용법인듯...
		//CTextureData *ptd = (CTextureData*)pto->GetData();
		//ptd->SetAsCurrent(pto->GetFrame());
//강동민 수정 끝 Water 구현			04.22
		*/
//강동민 수정 끝 Water 구현			04.20

		InitAsSkaModel();
		SetFlagOn(ENF_NICEWATER);	

		BOOL bLoadOK = TRUE;
		// try to load the model
		try {
		  SetSkaModel_t(m_fnModel);
		  // if failed
		} catch(char *) {            
		  bLoadOK = FALSE;		  
		  
		}
		if (!bLoadOK)
		{
			SetSkaModel(CTFILENAME("Data\\Models\\Editor\\Ska\\Axis.smc"));
			WarningMessage(TRANS("Cannot load ska model '%s'"), (CTString&)m_fnModel);
		}

		GetModelInstance()->SetModelColor(m_colWater);
		
		// NOTE : Stretch시 FrameBBox도 같이 늘려줘야 함...
		StretchModel();
		ModelChangeNotify();
		//FillWaterInformation();
		return;
	};

//강동민 수정 시작 2차 작업			05.17
/*
	void Read_t( CTStream *istr,BOOL bNetwork) // throw char *
	{
	    CRationalEntity::Read_t(istr,bNetwork); 
		//FillWaterInformation();
	}
  
	void Write_t( CTStream *istr,BOOL bNetwork) // throw char *
	{ 
	    CRationalEntity::Write_t(istr,bNetwork);    
	}
	*/
//강동민 수정 끝 2차 작업			05.17

	void StretchModel(void) 
	{
		// stretch factors must not have extreme values
		if (Abs(m_vStretchXYZ(1))  < 0.01f) { m_vStretchXYZ(1)   = 0.01f;  }
		if (Abs(m_vStretchXYZ(2))  < 0.01f) { m_vStretchXYZ(2)   = 0.01f;  }
		if (Abs(m_vStretchXYZ(3))  < 0.01f) { m_vStretchXYZ(3)   = 0.01f;  }
		if (m_fStretchAll< 0.01f) { m_fStretchAll = 0.01f;  }

		if (Abs(m_vStretchXYZ(1))  >1000.0f) { m_vStretchXYZ(1)   = 1000.0f*Sgn(m_vStretchXYZ(1)); }
		if (Abs(m_vStretchXYZ(2))  >1000.0f) { m_vStretchXYZ(2)   = 1000.0f*Sgn(m_vStretchXYZ(2)); }
		if (Abs(m_vStretchXYZ(3))  >1000.0f) { m_vStretchXYZ(3)   = 1000.0f*Sgn(m_vStretchXYZ(3)); }
		if (m_fStretchAll>1000.0f) { m_fStretchAll = 1000.0f; }
		GetModelInstance()->StretchModel( m_vStretchXYZ*m_fStretchAll );
		return;
	};

procedures:
	Main()
	{
		InitNiceWater();		
		return;
	}
};
