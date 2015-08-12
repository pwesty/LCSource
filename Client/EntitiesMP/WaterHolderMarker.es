1921
%{
#include "StdH.h"
%}
uses "EntitiesMP/Marker";
uses "EntitiesMP/Terrain";

class CWaterHolderMarker: CMarker{
name      "WaterHolderMarker";
thumbnail "Thumbnails\\Terrain.tbn";
features  "HasName", "IsTargetable";

properties:		
	1	CTString m_strName	" Name" = "WaterHolderMarker",
	//2 FLOAT				m_fHeight		"Water Height"	= 0.0f,	
	4 INDEX				m_iGridX		"Row"	= 1,
	5 INDEX				m_iGridY		"Col"	= 1,
	6 FLOAT				m_fWidthSize	"Tile Width"	= 2.0f,
	7 FLOAT				m_fHeightSize	"Tile Height"	= 2.0f,
	8 BOOL				m_bGenerate		"Generate" = FALSE,
	20 INDEX			m_iStartID		"Start ID" = 0,
components:	
  1 editor model   MODEL_AREAMARKER     "Data\\Models\\Editor\\Axis.mdl",
  2 editor texture TEXTURE_AREAMARKER   "Data\\Models\\Editor\\Vector.tex"
functions:
	void InitWaterMarker()
	{		
		InitAsEditorModel();		
		SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
		SetCollisionFlags(ECF_IMMATERIAL);

		// set appearance
		SetModel(MODEL_AREAMARKER);
		SetModelMainTexture(TEXTURE_AREAMARKER);
		return;
	}
	void GenerateWaterEntity()
	{
		if(m_bGenerate)
		{			
			CPlacement3D plPlacement;
			FLOAT3D vPosition = GetLerpedPlacement().pl_PositionVector;
			for(int i = 0; i < m_iGridX; ++i)
			{
				for(int j = 0; j < m_iGridY ; ++j)
				{					
					CEntity* penEntity = NULL;
					plPlacement.pl_PositionVector = FLOAT3D(vPosition(1) + i * m_fWidthSize, vPosition(2), vPosition(3) + j * m_fHeightSize);
					plPlacement.pl_OrientationAngle = ANGLE3D(0.0f, 0.0f, 0.0f);
				
					if(m_iStartID == 0)
					{
						penEntity = en_pwoWorld->CreateEntity_t(plPlacement,CTFILENAME("Classes\\WaterHolder.ecl"));
					}
					else
					{
						penEntity = en_pwoWorld->CreateEntity_t(plPlacement,CTFILENAME("Classes\\WaterHolder.ecl"), m_iStartID + (i * m_iGridX + j));
					}
					ASSERT(penEntity != NULL && "Invalid Entity Pointer");
					//en_pwoWorld->CopyEntityInWorld(*penEntity, plPlacement);
					//penEntity->en_penParent = this;
					//penEntity = NULL;
				}
			}
			m_bGenerate = FALSE;
		}
	}

procedures:
	Main()
	{
		InitWaterMarker();		
		GenerateWaterEntity();
		return;
	}
};
