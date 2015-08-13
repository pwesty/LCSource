612
%{
#include "StdH.h"
#include <Engine/Base/Timer.h>

inline BOOL IsNull(CEntityPointer &pen)	{ return pen.ep_pen == NULL; }
inline BOOL NotNull(CEntityPointer &pen)	{ return pen.ep_pen != NULL; }
%}

uses "EntitiesMP/ModelHolder2";
uses "EntitiesMP/ModelHolder3";
uses "EntitiesMP/Light";

enum ColorControllerType {
	0 CCT_ONLYCOLOR			"Effect Only Color",			// only r,g,b
	1 CCT_ONLYALPHA			"Effect Only Alpha",			// only a
	2 CCT_COLORANDALPHA		"Effect Color and Alpha",		// r,g,b,a
	3 CCT_DAYNIGHTCOLOR		"Day & Night Color & Alpha",	//special type
};

class CColorController : CRationalEntity
{
name      "Color controller";
thumbnail "Thumbnails\\BlendController.tbn";
features  "HasName", "IsTargetable";

properties:

	 1 enum ColorControllerType m_cctType	"Color Controller Type" = CCT_COLORANDALPHA,
	 2 CTString			m_strName			"Name" = "",

	 5 COLOR			m_colGoal			"Goal Color" = (C_WHITE|CT_OPAQUE),
	 9 FLOAT			m_fFadeInTime		"Fade-In Time (gwt)(s)" = 1200.0f,
	10 FLOAT			m_fFadeOutTime		"Fade-Out Time (gwt)(s)" = 1200.0f,
	11 CEntityPointer	m_penTarget01		"Target 01",
	12 CEntityPointer	m_penTarget02		"Target 02",
	13 CEntityPointer	m_penTarget03		"Target 03",
	14 CEntityPointer	m_penTarget04		"Target 04",
	15 CEntityPointer	m_penTarget05		"Target 05",
	16 CEntityPointer	m_penTarget06		"Target 06",
	17 CEntityPointer	m_penTarget07		"Target 07",
	18 CEntityPointer	m_penTarget08		"Target 08",
	19 CEntityPointer	m_penTarget09		"Target 09",
	20 CEntityPointer	m_penTarget10		"Target 10",
	21 CEntityPointer	m_penTarget11		"Target 11",
	22 CEntityPointer	m_penTarget12		"Target 12",
	23 CEntityPointer	m_penTarget13		"Target 13",
	24 CEntityPointer	m_penTarget14		"Target 14",
	25 CEntityPointer	m_penTarget15		"Target 15",
	26 CEntityPointer	m_penTarget16		"Target 16",
	27 CEntityPointer	m_penTarget17		"Target 17",
	28 CEntityPointer	m_penTarget18		"Target 18",
	29 CEntityPointer	m_penTarget19		"Target 19",
	30 CEntityPointer	m_penTarget20		"Target 20",
	 {
		 BOOL		m_bIsFadeIn;
		 FLOAT		m_fPrevGWTime;
		 FLOAT		m_fAllFadeTime;
		 FLOAT		m_fLeftFadeTime;
		 COLOR		m_colTarget01;
		 COLOR		m_colTarget02;
		 COLOR		m_colTarget03;
		 COLOR		m_colTarget04;
		 COLOR		m_colTarget05;
		 COLOR		m_colTarget06;
		 COLOR		m_colTarget07;
		 COLOR		m_colTarget08;
		 COLOR		m_colTarget09;
		 COLOR		m_colTarget10;
		 COLOR		m_colTarget11;
		 COLOR		m_colTarget12;
		 COLOR		m_colTarget13;
		 COLOR		m_colTarget14;
		 COLOR		m_colTarget15;
		 COLOR		m_colTarget16;
		 COLOR		m_colTarget17;
		 COLOR		m_colTarget18;
		 COLOR		m_colTarget19;
		 COLOR		m_colTarget20;
	 }

components:

	1 editor model   MODEL_CONTROLLER          "Data\\Models\\Editor\\BlendController.mdl",
	2 editor texture TEXTURE_CONTROLLER        "Data\\Models\\Editor\\BlendController.tex",

functions:
	BOOL IsTargetValid(SLONG slPropertyOffset, CEntity *penTarget)
	{
		if(penTarget==NULL) { return FALSE; }
		
		ULONG ulFirst, ulLast;

		// if modelholder2 or modelholder3
		ulFirst = offsetof(CColorController, m_penTarget01);
		ulLast = offsetof(CColorController, m_penTarget10);
		if(slPropertyOffset >= ulFirst && slPropertyOffset <= ulLast)
		{
			return IsDerivedFromClass( penTarget, &CModelHolder3_DLLClass)
				|| IsDerivedFromClass( penTarget, &CModelHolder2_DLLClass)
				|| IsDerivedFromClass( penTarget, &CLight_DLLClass);
		}
		return CEntity::IsTargetValid(slPropertyOffset, penTarget);
	}

	COLOR GetColor(CEntityPointer pen)
	{
		if(IsNull(pen)) {return C_BLACK;}

		if(IsDerivedFromClass(pen, &CModelHolder3_DLLClass) )
		{
			return ((CModelHolder3*)(CEntity*)pen)->m_colModelColor;
		}
		else if(IsDerivedFromClass(pen, &CModelHolder2_DLLClass) )
		{
			return ((CModelHolder2*)(CEntity*)pen)->m_colModelColor;
		}
		else if(IsDerivedFromClass(pen, &CLight_DLLClass) )
		{
			return ((CLight*)(CEntity*)pen)->m_colColor;
		}
		return C_WHITE|CT_OPAQUE;
	}
	void SetColor(CEntityPointer pen, COLOR color)
	{
		if(IsNull(pen)) {return;}

		if(IsDerivedFromClass(pen, &CModelHolder3_DLLClass) )
		{
			((CModelHolder3*)(CEntity*)pen)->SetModelColor(color);
		}
		else if(IsDerivedFromClass(pen, &CModelHolder2_DLLClass) )
		{
			((CModelHolder2*)(CEntity*)pen)->SetModelColor(color);
		}
		else if(IsDerivedFromClass(pen, &CLight_DLLClass) )
		{
			((CLight*)(CEntity*)pen)->m_lsLightSource.ls_colColor = color;
		}
	}

	void FillTargetOriginalColor()
	{
		if(NotNull(m_penTarget01)) {m_colTarget01 = GetColor(m_penTarget01);}
		if(NotNull(m_penTarget02)) {m_colTarget02 = GetColor(m_penTarget02);}
		if(NotNull(m_penTarget03)) {m_colTarget03 = GetColor(m_penTarget03);}
		if(NotNull(m_penTarget04)) {m_colTarget04 = GetColor(m_penTarget04);}
		if(NotNull(m_penTarget05)) {m_colTarget05 = GetColor(m_penTarget05);}
		if(NotNull(m_penTarget06)) {m_colTarget06 = GetColor(m_penTarget06);}
		if(NotNull(m_penTarget07)) {m_colTarget07 = GetColor(m_penTarget07);}
		if(NotNull(m_penTarget08)) {m_colTarget08 = GetColor(m_penTarget08);}
		if(NotNull(m_penTarget09)) {m_colTarget09 = GetColor(m_penTarget09);}
		if(NotNull(m_penTarget10)) {m_colTarget10 = GetColor(m_penTarget10);}
		if(NotNull(m_penTarget11)) {m_colTarget11 = GetColor(m_penTarget11);}
		if(NotNull(m_penTarget12)) {m_colTarget12 = GetColor(m_penTarget12);}
		if(NotNull(m_penTarget13)) {m_colTarget13 = GetColor(m_penTarget13);}
		if(NotNull(m_penTarget14)) {m_colTarget14 = GetColor(m_penTarget14);}
		if(NotNull(m_penTarget15)) {m_colTarget15 = GetColor(m_penTarget15);}
		if(NotNull(m_penTarget16)) {m_colTarget16 = GetColor(m_penTarget16);}
		if(NotNull(m_penTarget17)) {m_colTarget17 = GetColor(m_penTarget17);}
		if(NotNull(m_penTarget18)) {m_colTarget18 = GetColor(m_penTarget18);}
		if(NotNull(m_penTarget19)) {m_colTarget19 = GetColor(m_penTarget19);}
		if(NotNull(m_penTarget20)) {m_colTarget20 = GetColor(m_penTarget20);}
	}
	FLOAT GetRatio()
	{
		if(m_bIsFadeIn)	{ return Clamp(1.0f - m_fLeftFadeTime / m_fAllFadeTime, 0.0f, 1.0f);	}
		else			{ return Clamp(m_fLeftFadeTime / m_fAllFadeTime, 0.0f, 1.0f);			}
	}
	void ColorProcess(CEntityPointer pen, COLOR colOriginal, FLOAT fRatio)
	{
		if(IsNull(pen)) {return;}

		COLOR color;
		if(m_cctType == CCT_COLORANDALPHA || m_cctType == CCT_DAYNIGHTCOLOR)
		{
			color = LerpColorInline(colOriginal, m_colGoal, fRatio);
		}
		else if(m_cctType == CCT_ONLYCOLOR) 
		{
			UBYTE ubR0, ubG0, ubB0, ubA0;
			UBYTE ubR1, ubG1, ubB1;
			ColorToRGBA( colOriginal, ubR0, ubG0, ubB0, ubA0 );
			ColorToRGB( m_colGoal, ubR1, ubG1, ubB1 );
			ubR0 = Lerp( ubR0, ubR1, fRatio);
			ubG0 = Lerp( ubG0, ubG1, fRatio);
			ubB0 = Lerp( ubB0, ubB1, fRatio);
			color = RGBAToColor( ubR0, ubG0, ubB0, ubA0);
		}
		else if(m_cctType == CCT_ONLYALPHA)
		{
			UBYTE ubR0, ubG0, ubB0, ubA0;
			UBYTE ubA1;
			ColorToRGBA( colOriginal, ubR0, ubG0, ubB0, ubA0 );
			ubA1 = m_colGoal & CT_OPAQUE;
			ubA0 = Lerp( ubA0, ubA1, fRatio );
			color = RGBAToColor( ubR0, ubG0, ubB0, ubA0);
		}

		SetColor(pen, color);
	}

	void DayNightProcess()
	{
		DOUBLE tmOffset = GAMETIME_ONEDAY_SECOND-GAMETIME_MORNING_SECOND;
		DOUBLE dGWTSecondFromMorning = g_fGWTime + tmOffset;
		//하루시간안에서만 계산하기 위해
		if(dGWTSecondFromMorning > GAMETIME_ONEDAY_SECOND)		
		{
			dGWTSecondFromMorning -= GAMETIME_ONEDAY_SECOND * (INDEX)(dGWTSecondFromMorning / GAMETIME_ONEDAY_SECOND);
		}

		//해일때 처리
		//낮
		FLOAT fRatio = 1.0f;
		if(dGWTSecondFromMorning < GAMETIME_NOONDAY_SECOND)
		{
			fRatio = ClampUp(dGWTSecondFromMorning / (DOUBLE)m_fFadeOutTime, 1.0);
		}
		//밤
		else
		{
			fRatio = 1.0f - ClampUp((dGWTSecondFromMorning - GAMETIME_NOONDAY_SECOND) / (DOUBLE)m_fFadeInTime, 1.0);
		}
		if(NotNull(m_penTarget01)) {ColorProcess(m_penTarget01, m_colTarget01, fRatio);}
		if(NotNull(m_penTarget02)) {ColorProcess(m_penTarget02, m_colTarget02, fRatio);}
		if(NotNull(m_penTarget03)) {ColorProcess(m_penTarget03, m_colTarget03, fRatio);}
		if(NotNull(m_penTarget04)) {ColorProcess(m_penTarget04, m_colTarget04, fRatio);}
		if(NotNull(m_penTarget05)) {ColorProcess(m_penTarget05, m_colTarget05, fRatio);}
		if(NotNull(m_penTarget06)) {ColorProcess(m_penTarget06, m_colTarget06, fRatio);}
		if(NotNull(m_penTarget07)) {ColorProcess(m_penTarget07, m_colTarget07, fRatio);}
		if(NotNull(m_penTarget08)) {ColorProcess(m_penTarget08, m_colTarget08, fRatio);}
		if(NotNull(m_penTarget09)) {ColorProcess(m_penTarget09, m_colTarget09, fRatio);}
		if(NotNull(m_penTarget10)) {ColorProcess(m_penTarget10, m_colTarget10, fRatio);}
		if(NotNull(m_penTarget11)) {ColorProcess(m_penTarget11, m_colTarget11, fRatio);}
		if(NotNull(m_penTarget12)) {ColorProcess(m_penTarget12, m_colTarget12, fRatio);}
		if(NotNull(m_penTarget13)) {ColorProcess(m_penTarget13, m_colTarget13, fRatio);}
		if(NotNull(m_penTarget14)) {ColorProcess(m_penTarget14, m_colTarget14, fRatio);}
		if(NotNull(m_penTarget15)) {ColorProcess(m_penTarget15, m_colTarget15, fRatio);}
		if(NotNull(m_penTarget16)) {ColorProcess(m_penTarget16, m_colTarget16, fRatio);}
		if(NotNull(m_penTarget17)) {ColorProcess(m_penTarget17, m_colTarget17, fRatio);}
		if(NotNull(m_penTarget18)) {ColorProcess(m_penTarget18, m_colTarget18, fRatio);}
		if(NotNull(m_penTarget19)) {ColorProcess(m_penTarget19, m_colTarget19, fRatio);}
		if(NotNull(m_penTarget20)) {ColorProcess(m_penTarget20, m_colTarget20, fRatio);}
	}

procedures:
	Main()
	{
		// init model
		//InitAsVoid();
		InitAsEditorModel();

		SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
		SetCollisionFlags(ECF_IMMATERIAL);

		// set appearance
		SetModel(MODEL_CONTROLLER);
		SetModelMainTexture(TEXTURE_CONTROLLER);

		m_fFadeInTime = ClampDn(m_fFadeInTime, 0.0f);
		m_fFadeOutTime = ClampDn(m_fFadeOutTime, 0.0f);
		if(m_fFadeInTime <= 0) {m_fFadeInTime = 0.000001f;}
		if(m_fFadeOutTime <= 0) {m_fFadeOutTime = 0.000001f;}

		m_bIsFadeIn = TRUE;
		FillTargetOriginalColor();

		if(m_cctType == CCT_DAYNIGHTCOLOR)
		{
			DayNightProcess();
			jump DyaNightLoop();
		}
		else
		{
			jump MainLoop();
		}
	}

	Fade()
	{
		m_fPrevGWTime = g_fGWTime;
		while(m_fLeftFadeTime >= 0.0f)
		{
			m_fLeftFadeTime -= g_fGWTime - m_fPrevGWTime;
			m_fPrevGWTime = g_fGWTime;

			FLOAT fRatio = GetRatio();

			if(NotNull(m_penTarget01)) {ColorProcess(m_penTarget01, m_colTarget01, fRatio);}
			if(NotNull(m_penTarget02)) {ColorProcess(m_penTarget02, m_colTarget02, fRatio);}
			if(NotNull(m_penTarget03)) {ColorProcess(m_penTarget03, m_colTarget03, fRatio);}
			if(NotNull(m_penTarget04)) {ColorProcess(m_penTarget04, m_colTarget04, fRatio);}
			if(NotNull(m_penTarget05)) {ColorProcess(m_penTarget05, m_colTarget05, fRatio);}
			if(NotNull(m_penTarget06)) {ColorProcess(m_penTarget06, m_colTarget06, fRatio);}
			if(NotNull(m_penTarget07)) {ColorProcess(m_penTarget07, m_colTarget07, fRatio);}
			if(NotNull(m_penTarget08)) {ColorProcess(m_penTarget08, m_colTarget08, fRatio);}
			if(NotNull(m_penTarget09)) {ColorProcess(m_penTarget09, m_colTarget09, fRatio);}
			if(NotNull(m_penTarget10)) {ColorProcess(m_penTarget10, m_colTarget10, fRatio);}
			if(NotNull(m_penTarget11)) {ColorProcess(m_penTarget11, m_colTarget11, fRatio);}
			if(NotNull(m_penTarget12)) {ColorProcess(m_penTarget12, m_colTarget12, fRatio);}
			if(NotNull(m_penTarget13)) {ColorProcess(m_penTarget13, m_colTarget13, fRatio);}
			if(NotNull(m_penTarget14)) {ColorProcess(m_penTarget14, m_colTarget14, fRatio);}
			if(NotNull(m_penTarget15)) {ColorProcess(m_penTarget15, m_colTarget15, fRatio);}
			if(NotNull(m_penTarget16)) {ColorProcess(m_penTarget16, m_colTarget16, fRatio);}
			if(NotNull(m_penTarget17)) {ColorProcess(m_penTarget17, m_colTarget17, fRatio);}
			if(NotNull(m_penTarget18)) {ColorProcess(m_penTarget18, m_colTarget18, fRatio);}
			if(NotNull(m_penTarget19)) {ColorProcess(m_penTarget19, m_colTarget19, fRatio);}
			if(NotNull(m_penTarget20)) {ColorProcess(m_penTarget20, m_colTarget20, fRatio);}

			autowait(_pTimer->TickQuantum);
		}
		jump MainLoop();
	}
	MainLoop()
	{
		autowait(0.2f);
		FillTargetOriginalColor();
		wait()
		{
			on(EBegin):
			{
				resume;
			}
			on(EActivate):
			{
				m_bIsFadeIn = TRUE;
				m_fLeftFadeTime = m_fAllFadeTime = m_fFadeInTime;

				jump Fade();
			}
			on(EDeactivate):
			{
				m_bIsFadeIn = FALSE;
				m_fLeftFadeTime = m_fAllFadeTime = m_fFadeOutTime;

				jump Fade();
			}
		};
	}

	DyaNightLoop()
	{
		autowait(0.2f);
		FillTargetOriginalColor();
		while(TRUE)
		{
			autowait(0.5f * FRnd() + 0.5f);
			DayNightProcess();
		}
	}
};

