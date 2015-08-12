226
%{
#include "StdH.h"
#include <Engine/Interface/UISignBoard.h>
extern INDEX ent_bReportBrokenChains;
%}

class CMessageHolder : CRationalEntity
{
name      "MessageHolder";
thumbnail "Thumbnails\\MessageHolder.tbn";
features  "HasName", "IsTargetable";

properties:
//9 BOOL m_bShpere			"Sphere Type?" = TRUE,
1 CTString m_strName		"Name" 'N' = "MessageHolder",
2 INDEX m_iStringIndex		"String Index" = -1,
3 CTString m_strRealString	"Real String" = "",
//4 RANGE m_rngAreaRadius		"Area Radius" = 10,
//5 CEntityPointer m_penNext	"Next (In Group)" 'X',
{
	//FLOAT	m_fRadiusSquare;
	//BOOL	m_bIsInArea;
}

components:
1 editor model   MODEL_MARKER     "Data\\Models\\Editor\\MessageHolder.mdl",
2 editor texture TEXTURE_MARKER   "Data\\Models\\Editor\\MessageHolder.tex"

functions:
/*
	BOOL IsTargetValid(SLONG slPropertyOffset, CEntity *penTarget)
	{
		if(penTarget==NULL) { return FALSE; }
		
		ULONG ulFirst;

		// if CMessageHolder
		ulFirst = offsetof(CMessageHolder, m_penNext);
		if(slPropertyOffset == ulFirst) { return IsDerivedFromClass( penTarget, &CMessageHolder_DLLClass); }

		return CEntity::IsTargetValid(slPropertyOffset, penTarget);
	}
*/
procedures:
	Main()
	{
		InitAsEditorModel();
		SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
		SetCollisionFlags(ECF_IMMATERIAL);
		
		// set appearance
		SetModel(MODEL_MARKER);
		SetModelMainTexture(TEXTURE_MARKER);

		//m_fRadiusSquare = m_rngAreaRadius * m_rngAreaRadius;
		//m_bIsInArea = FALSE;
		//set real string by string index
		//TODO : m_strRealString = ...

		autowait(0.01f);

		//if(m_bShpere)	{jump MainSphereType();	}
		//else			{jump MainMessageType();}
		jump MainMessageType();
		return;
	}
	
/*
	MainSphereType()
	{
		while(TRUE)
		{
			FLOAT time = 10.01f;
			m_fRadiusSquare = m_rngAreaRadius * m_rngAreaRadius;
			if(CEntity::GetPlayerEntity(0) != NULL)
			{
				FLOAT mag = (CEntity::GetPlayerEntity(0)->GetPlacement().pl_PositionVector
							- this->GetPlacement().pl_PositionVector).Magnitude();
				OutputDebugString(m_strName.str_String);
				OutputDebugString("<- Check Distance\n");
				//캐릭의 최대 이동 속도를 초당 10m/s로 가정.
				if(mag - m_fRadiusSquare > 10000.0f)		{ time = 10.0f;	m_bIsInArea = FALSE;	}
				else if(mag - m_fRadiusSquare > 1600.0f)	{ time = 4.0f;	m_bIsInArea = FALSE;	}
				else if(mag - m_fRadiusSquare > 0.0f)		{ time = 1.0f;	m_bIsInArea = FALSE;	}
				else if(!m_bIsInArea)	//in area
				{
					time = 1.0f;
					BOOL bInOtherArea = FALSE;
					for(CEntity *pen = this->m_penNext.ep_pen;
						pen != NULL && pen != this && !bInOtherArea;
						pen = ((CMessageHolder*)pen)->m_penNext.ep_pen)
					{
							bInOtherArea = ((CMessageHolder*)pen)->m_bIsInArea;
					}
					if(!bInOtherArea)
					{
						//send messge to engine for print message on screen
						//TODO : PrintMessge ...
					//	ShowSignBoard ( m_iStringIndex );
						OutputDebugString(m_strName.str_String);
						OutputDebugString("<- Use Sphere Check\n");
					}
					m_bIsInArea = TRUE;
				}
			}
			autowait(time);
		}
		return;
	}
*/

	MainMessageType()
	{
		while(TRUE)
		{
			wait()
			{
				on(EBegin):
				{
					resume;
				}
				on(ETimer):
				{
					stop;
				}
				on(ETrigger):
				{
					//send messge to engine for print message on screen
					//TODO : PrintMessge ...
					ShowSignBoard ( m_iStringIndex );
					OutputDebugString(m_strName.str_String);
					OutputDebugString("<- Use Touch Field\n");
					resume;
				}
			}
		}
	}
};

