//안태훈 수정 시작	//(Add Sun Moon Entity and etc)(0.2)
1001
%{
#include "stdH.h"
#include <Engine/Base/Timer.h>
#include <Engine/Terrain/Terrain.h>
#include <math.h>
#include <Engine/Graphics/Color.h>
%}

uses "EntitiesMP/Light";
uses "EntitiesMP/EnvironmentParticlesHolder";
uses "EntitiesMP/BackgroundViewer";
uses "EntitiesMP/WorldSettingsController";
uses "EntitiesMP/ModelHolder3";
uses "EntitiesMP/HazeMarker";
uses "EntitiesMP/FogMarker";

//선택적인 C 블락
%{
extern COLOR g_colNewGameShadeColor;

#define C_WHITE_OPAQUE	(C_WHITE|CT_OPAQUE)

	inline COLOR AverageColor(COLOR col1, COLOR col2)
	{
		UBYTE uR1, uG1, uB1;
		UBYTE uR2, uG2, uB2;
		ColorToRGB(col1, uR1, uG1, uB1);
		ColorToRGB(col2, uR2, uG2, uB2);
		uR1 = (uR1>>1) + (uR2>>1);
		uG1 = (uG1>>1) + (uG2>>1);
		uB1 = (uB1>>1) + (uB2>>1);
		return RGBToColor(uR1, uG1, uB1);
	}
%}

class CSunMoon : CMovableModelEntity
{
name      "SunMoon";
thumbnail "SunMoon.tbn";
features  "IsTargetable", "HasName";

properties:
	 1 CTString				m_strName				" Name" = "SunMoon ",
	 3 CEntityPointer		m_epLight				"Light Target",
	 4 CEntityPointer		m_epSky					"Sky Target",
	 7 CEntityPointer		m_epSkyHorizon			"Sky Horizon Target",
	 5 CEntityPointer		m_epHaze				"Haze Target",
	 6 CEntityPointer		m_epFog					"Fog Target",
	11 FLOAT				m_fLength				"Length" = 10,
	12 FLOAT				m_fHeight				"Height" = 5,
	16 BOOL					m_bWeatherSun			= TRUE,
	21 COLOR				m_colRainAmbient		"Rain Ambient" = C_WHITE_OPAQUE,
	22 COLOR				m_colHazeDefault		"Haze Color" = C_WHITE_OPAQUE,
	23 COLOR				m_colFogDefault			"Fog Color" = C_WHITE_OPAQUE,
	31 BOOL					m_bActive				"Active" = TRUE,
	41 enum LensFlareType   m_lftSun				"Sun Lens Flare" = LFT_NONE,
	42 enum LensFlareType   m_lftMoon				"Moon Lens Flare" = LFT_NONE,
	51 CTFileName			m_fnmColAniFile			"Sky Color Ani File" = CTString(""),
	52 ANIMATION			m_iSkyColAni			"Sky Color Ani" = 0,
	53 CAnimObject			m_aoSkyColAni,
	61 CEntityPointer		m_epSunModel			"Sun Model Target",
	62 CEntityPointer		m_epMoonModel			"Moon Model Target",
	66 enum EventEType		m_eetMorningEvent		"Event type Mornig" = EET_START,
	67 enum EventEType		m_eetEveningEvent		"Event type Evening" = EET_STOP,
	68 CEntityPointer		m_epMorningTarget		"Target Morning"	COLOR(0xFF22FF|0xFF),
	69 CEntityPointer		m_epEveningTarget		"Target Evening"	COLOR(0xFF22FF|0xFF),
	71 BOOL					m_bNoon = TRUE,
	72 BOOL					m_bNight = TRUE,
	{
		 DOUBLE				m_dPrevGWTime;
		 FLOAT				m_fFadeTime;
		 FLOAT				m_fAllFadeTime;
		 CLensFlareType		*m_plftDefault;
	}
components:
	 1 editor model		MODEL_SUNMOON		"data\\world\\skybox\\Sunmoon.mdl",
functions:
	// Init model holder
	void InitModelHolder(void) 
	{
		InitAsEditorModel();
	 
		BOOL bLoadOK = TRUE;
		// try to load the model
		SetModel(MODEL_SUNMOON);

		GetModelObject()->StretchModel(FLOAT3D(m_fLength,m_fHeight,m_fLength));
		
		SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
		SetCollisionFlags(ECF_IMMATERIAL);

		SetFlags(GetFlags()&~ENF_CLUSTERSHADOWS);

		return;
	};
	// Validate offered target for one property
	BOOL IsTargetValid(SLONG slPropertyOffset, CEntity *penTarget)
	{
		if(penTarget==NULL) { return FALSE; }
		
		ULONG ulFirst;

		// if light
		ulFirst = offsetof(CSunMoon, m_epLight);
		if( slPropertyOffset==ulFirst) { return IsDerivedFromClass( penTarget, &CLight_DLLClass); }
		// if modelholder3
		ulFirst = offsetof(CSunMoon, m_epSky);
		if( slPropertyOffset==ulFirst) { return IsDerivedFromClass( penTarget, &CModelHolder3_DLLClass); }
		ulFirst = offsetof(CSunMoon, m_epSkyHorizon);
		if( slPropertyOffset==ulFirst) { return IsDerivedFromClass( penTarget, &CModelHolder3_DLLClass); }
		// if HazeMarker
		ulFirst = offsetof(CSunMoon, m_epHaze);
		if( slPropertyOffset==ulFirst) { return IsDerivedFromClass( penTarget, &CHazeMarker_DLLClass); }
		// if FogMarker
		ulFirst = offsetof(CSunMoon, m_epFog);
		if( slPropertyOffset==ulFirst) { return IsDerivedFromClass( penTarget, &CFogMarker_DLLClass); }
		// if Sun Model
		ulFirst = offsetof(CSunMoon, m_epSunModel);
		if( slPropertyOffset==ulFirst) { return (IsDerivedFromClass( penTarget, &CModelHolder3_DLLClass) || IsDerivedFromClass( penTarget, &CModelHolder2_DLLClass)); }
		// if Moon Model
		ulFirst = offsetof(CSunMoon, m_epMoonModel);
		if( slPropertyOffset==ulFirst) { return (IsDerivedFromClass( penTarget, &CModelHolder3_DLLClass) || IsDerivedFromClass( penTarget, &CModelHolder2_DLLClass)); }

		return CEntity::IsTargetValid(slPropertyOffset, penTarget);
	}
	CEnvironmentParticlesHolder *GetEPH()
	{
		CWorld *pWorld = this->GetWorld();
		if( pWorld != NULL )
		{
			CBackgroundViewer *pBV = (CBackgroundViewer*)(CEntity*)(pWorld->wo_penBackgroundViewer);
			if( pBV != NULL )
			{
				CWorldSettingsController *pWSC = (CWorldSettingsController*)(CEntity*)(pBV->m_penWorldSettingsController);
				if( pWSC != NULL )
				{
					return (CEnvironmentParticlesHolder*)(CEntity*)(pWSC->m_penEnvPartHolder);
				}
			}
		}
		return NULL;
	}
	CLight *GetLight()
	{
		if( (CEntity*)m_epLight != NULL)
		{
			return (CLight*)(CEntity*)m_epLight;
		}
		return NULL;
	}
	CModelHolder3 *GetSky()
	{
		if( (CEntity*)m_epSky != NULL)
		{
			return (CModelHolder3*)(CEntity*)m_epSky;
		}
		return NULL;
	}
	CModelHolder3 *GetSkyHorizon()
	{
		if( (CEntity*)m_epSkyHorizon != NULL)
		{
			return (CModelHolder3*)(CEntity*)m_epSkyHorizon;
		}
		return NULL;
	}
	CHazeMarker *GetHaze()
	{
		if( (CEntity*)m_epHaze != NULL)
		{
			return (CHazeMarker*)(CEntity*)m_epHaze;
		}
		return NULL;
	}
	CFogMarker *GetFog()
	{
		if( (CEntity*)m_epFog != NULL)
		{
			return (CFogMarker*)(CEntity*)m_epFog;
		}
		return NULL;
	}
	void SetSunModelColor(COLOR col)
	{
		if( (CEntity*)m_epSunModel != NULL)
		{
			if(IsDerivedFromClass( m_epSunModel, &CModelHolder3_DLLClass))
			{
				if(m_epSunModel->GetModelInstance())
				{
					m_epSunModel->GetModelInstance()->SetModelColor(col);
					((CModelHolder3*)(CEntity*)m_epSunModel)->m_colModelColor = col;
				}
			}
			else //if(IsDerivedFromClass( m_epSunModel, &CModelHolder2_DLLClass))
			{
				if(m_epSunModel->GetModelObject())
				{
					m_epSunModel->GetModelObject()->mo_colBlendColor = col;
					((CModelHolder2*)(CEntity*)m_epSunModel)->m_colModelColor = col;
				}
			}
		}
	}
	void SetMoonModelColor(COLOR col)
	{
		if( (CEntity*)m_epMoonModel != NULL)
		{
			if(IsDerivedFromClass( m_epMoonModel, &CModelHolder3_DLLClass))
			{
				if(m_epMoonModel->GetModelInstance())
				{
					m_epMoonModel->GetModelInstance()->SetModelColor(col);
					((CModelHolder3*)(CEntity*)m_epMoonModel)->m_colModelColor = col;
				}
			}
			else //if(IsDerivedFromClass( m_epMoonModel, &CModelHolder2_DLLClass))
			{
				if(m_epMoonModel->GetModelObject())
				{
					m_epMoonModel->GetModelObject()->mo_colBlendColor = col;
					((CModelHolder2*)(CEntity*)m_epMoonModel)->m_colModelColor = col;
				}
			}
		}
	}

	void SetLensFlare(LensFlareType lft)
	{
		if(GetLight() == NULL) {return;}

		CLightSource lsNew;
		GetLight()->SetupLightSource(lsNew);
		switch(lft)
		{
			case LFT_NONE:
				lsNew.ls_plftLensFlare = NULL;
				break;    
			case LFT_STANDARD:
				lsNew.ls_plftLensFlare = &_lftStandard;                   
				break;
			case LFT_STANDARD_REFLECTIONS:
				lsNew.ls_plftLensFlare = &_lftStandardReflections;        
				break;
			case LFT_YELLOW_STAR_RED_RING:
				lsNew.ls_plftLensFlare = &_lftYellowStarRedRing;          
				break;
			case LFT_WHITE_GLOW_STAR_RED_RING:
				lsNew.ls_plftLensFlare = &_lftWhiteGlowStarRedRing;       
				break;
			case LFT_WHITE_GLOW_STAR:
				lsNew.ls_plftLensFlare = &_lftWhiteGlowStar;              
				break;
			case LFT_WHITE_STAR_RED_RING_STREAKS:
				lsNew.ls_plftLensFlare = &_lftWhiteStarRedRingStreaks;    
				break;
			case LFT_WHITE_STAR_RED_REFLECTIONS:
				lsNew.ls_plftLensFlare = &_lftWhiteStarRedReflections;       
				break;
			case LFT_BLUE_STAR_BLUE_REFLECTIONS:
				lsNew.ls_plftLensFlare = &_lftBlueStarBlueReflections;       
				break;
//			case LFT_PV_SPACE_SHIP_WINDOW_FLARE:
//				lsNew.ls_plftLensFlare = &_lftPVSpaceShipWindowFlare;
//				break;
			case LFT_WHITE_GLOW_STAR_RED_RING_FAR:
				lsNew.ls_plftLensFlare = &_lftYellowStarRedRingFar;
				break;
			case LFT_WHITE_GLOW_FAR:
				lsNew.ls_plftLensFlare = &_lftWhiteGlowFar;
				break;
			case LFT_WHITE_GLOW_STAR_NG:
				lsNew.ls_plftLensFlare = &_lftWhiteGlowStarNG;              
				break;
		}
		GetLight()->m_lsLightSource.SetLightSourceWithNoDiscarding(lsNew);
	}

	COLOR GetCurrentSkyAniColor(COLOR colBase)
	{
		if(m_aoSkyColAni.GetData() != NULL)
		{
			UBYTE ubR, ubG, ubB;
			ColorToRGB( colBase, ubR, ubG, ubB);
			FLOAT fRatio;
			COLOR col0, col1;
			UBYTE ubMR, ubMG, ubMB;
			m_aoSkyColAni.GetFrame( (SLONG&)col0, (SLONG&)col1, fRatio);
			LerpColor( col0, col1, fRatio, ubMR, ubMG, ubMB);
			ubR = ( ((((SLONG)ubR)<<8)|ubR) * ((((SLONG)ubMR)<<8)|ubMR) ) >>24;
			ubG = ( ((((SLONG)ubG)<<8)|ubG) * ((((SLONG)ubMG)<<8)|ubMG) ) >>24;
			ubB = ( ((((SLONG)ubB)<<8)|ubB) * ((((SLONG)ubMB)<<8)|ubMB) ) >>24;
			colBase = RGBToColor(ubR, ubG, ubB) | ((colBase & CT_AMASK) >> CT_ASHIFT);
		}
		return colBase;
	}
	COLOR GetCurrentSkyAniColor()
	{
		return GetCurrentSkyAniColor(C_WHITE_OPAQUE);
	}
	CAnimData *GetAnimData(SLONG slPropertyOffset) 
	{
		if (slPropertyOffset==offsetof(CSunMoon, m_iSkyColAni))
		{
			return m_aoSkyColAni.GetData();
		}
		else
		{
			return CEntity::GetAnimData(slPropertyOffset);
		}
	};

	void SetLightPlacement3D()
	{
		if(GetLight() == NULL) { return; }

		DOUBLE tmOffset = GAMETIME_ONEDAY_SECOND-GAMETIME_MORNING_SECOND;
		DOUBLE dGWTSecondFromMorning = g_fGWTime + tmOffset;

		//하루시간안에서만 계산하기 위해
		if(dGWTSecondFromMorning > GAMETIME_ONEDAY_SECOND)		
		{
			dGWTSecondFromMorning -= GAMETIME_ONEDAY_SECOND * (INDEX)(dGWTSecondFromMorning / GAMETIME_ONEDAY_SECOND);
		}
		CPlacement3D place;
		FLOAT theta = 0.0f;
		//해일때 처리
		//낮
		if(dGWTSecondFromMorning < GAMETIME_NOONDAY_SECOND)
		{
			theta = PI * dGWTSecondFromMorning / (FLOAT)GAMETIME_NOONDAY_SECOND;
			if(m_bNoon)	//낮이 되기 시작
			{
				SendToTarget(m_epMorningTarget, m_eetMorningEvent, this);
				SetLensFlare(m_lftSun);
				m_bNoon = FALSE;
				m_bNight = TRUE;
				SetSunModelColor(0xFFFFFFFF);
				SetMoonModelColor(0x00000000);
			}
		}
		//밤
		else
		{
			theta = PI - PI * (dGWTSecondFromMorning-GAMETIME_NOONDAY_SECOND) / (FLOAT)GAMETIME_NIGHT_SECOND;
			if(m_bNight)	//밤이 되기 시작
			{
				SendToTarget(m_epEveningTarget, m_eetEveningEvent, this);
				SetLensFlare(m_lftMoon);
				m_bNoon = TRUE;
				m_bNight = FALSE;
				SetSunModelColor(0x00000000);
				SetMoonModelColor(0xFFFFFFFF);
			}
		}
		
		place.pl_PositionVector(1) = m_fLength * cos(theta);
		place.pl_PositionVector(2) = m_fHeight * sin(theta);
		place.pl_PositionVector(3) = 0;
		place.pl_OrientationAngle(1) = 270.0f;
		place.pl_OrientationAngle(2) = 180.0f + theta * 180.0f / PI;
		place.pl_OrientationAngle(3) = 0.0f;

		CPlacement3D placeOld = this->en_plPlacement;
		CPlacement3D placeZero( FLOAT3D(0,0,0), ANGLE3D(0,0,0) );

		GetLight()->SetParent(NULL);
		this->SetPlacement(placeZero);
		GetLight()->SetPlacement(place);
		GetLight()->SetParent(this);
		
		this->SetPlacement(placeOld);

	};
	
	void SetCurrentColor()
	{
		FLOAT fGWTime = g_fGWTime;
		DOUBLE rwtime = - g_fGWTime * GWTOneSec2RWTSec;
		TIME tmNow = _pTimer->CurrentTick() + _pTimer->GetLerpFactor()*_pTimer->TickQuantum;
		DOUBLE rwOffsetTime = rwtime + tmNow;
	
		if(GetLight())
		{
			GetLight()->m_aoLightAnimation.ResetAnim();
			GetLight()->m_aoAmbientLightAnimation.ResetAnim();
			GetLight()->m_aoLightAnimation.ao_tmAnimStart = rwOffsetTime;
			GetLight()->m_aoAmbientLightAnimation.ao_tmAnimStart = rwOffsetTime;
			GetLight()->m_lsLightSource.ls_colAmbient = MulColors( GetLight()->m_colAmbient, g_colWeather );
			g_colNewGameShadeColor = AddColors(
				GetLight()->m_lsLightSource.GetLightAmbient()
				, GetLight()->m_lsLightSource.GetLightColor()) | CT_OPAQUE;
			if(GetSkyHorizon())
			{
				ENGINE_API INDEX g_iUseBloom;
				static COLOR col2nd = 0x9E9E9EFF;
				static COLOR col1st = 0xA5A5A5FF;
				COLOR color = g_iUseBloom == 0
					? g_colNewGameShadeColor
					: MulColors(g_colNewGameShadeColor, g_iUseBloom > 2 ? col2nd : col1st );
				GetSkyHorizon()->GetModelInstance()->SetModelColor(
					MulColors(GetSkyHorizon()->m_colModelColor, color)
					);
				GetSkyHorizon()->m_aShadingDirection = GetLight()->en_plPlacement.pl_OrientationAngle;
				AnglesToDirectionVector(GetSkyHorizon()->m_aShadingDirection, GetSkyHorizon()->m_vShadingDirection);
				//GetSkyHorizon()->m_vShadingDirection = -GetSkyHorizon()->m_vShadingDirection;
			}
			if(GetHaze())
			{
				GetHaze()->m_colBase = MulColors(m_colHazeDefault, g_colNewGameShadeColor);
			}
			if(GetFog())
			{
				GetFog()->m_colColor = MulColors(m_colFogDefault, g_colNewGameShadeColor);
			}
		}
		if(m_aoSkyColAni.GetData() != NULL)
		{
			m_aoSkyColAni.ResetAnim();
			m_aoSkyColAni.ao_tmAnimStart = rwOffsetTime;
			COLOR colSky = GetCurrentSkyAniColor( g_colWeather );
			if(GetSky())
			{
				GetSky()->GetModelInstance()->SetModelColor(
					MulColors( GetSky()->m_colModelColor, colSky )
					);
			}
		}
	};

	//fog, haze처리, SetLightColor이후에 호출되어야 함.
	void LightProcess()
	{
		if(m_bActive)
		{
			SetLightPlacement3D();
			SetCurrentColor();
		}
	};


procedures:
	Main()
	{
		// Init entity
		InitModelHolder();

		g_colWeather = C_WHITE_OPAQUE;
		FLOAT fRWDaySecond = (FLOAT)GAMETIME_ONEDAY_SECOND * GWTOneSec2RWTSec;

		try
		{
			m_aoSkyColAni.SetData_t(m_fnmColAniFile);
		}
		catch (char *strError)
		{
			WarningMessage(TRANS("Cannot load '%s': %s"), (CTString&)m_fnmColAniFile, strError);
			m_fnmColAniFile = "";
		}
		if(m_aoSkyColAni.GetData() != NULL)
		{
			m_aoSkyColAni.PlayAnim(m_iSkyColAni, AOF_LOOPING);
			m_aoSkyColAni.ao_tmAnimStart = 0;
			m_aoSkyColAni.GetData()->SetSpeed(
				m_iSkyColAni
				, fRWDaySecond/(FLOAT)m_aoSkyColAni.GetFramesInCurrentAnim()
				);
		}

		m_fFadeTime = 0.0f;
		m_fAllFadeTime = 1.0f;
		m_dPrevGWTime = g_fGWTime;
		m_bNoon = TRUE;
		m_bNight = TRUE;

		CLight *pLight = GetLight();
		if(pLight != NULL)
		{
			pLight->SetParent(this);
			m_plftDefault = pLight->m_lsLightSource.ls_plftLensFlare;
			if(pLight->m_aoLightAnimation.GetData() != NULL)
			{
				pLight->m_aoLightAnimation.GetData()->SetSpeed(
					pLight->m_iLightAnimation
					, fRWDaySecond/(FLOAT)pLight->m_aoLightAnimation.GetFramesInCurrentAnim()
					);
			}
			if(pLight->m_aoAmbientLightAnimation.GetData() != NULL)
			{
				pLight->m_aoAmbientLightAnimation.GetData()->SetSpeed(
					pLight->m_iAmbientLightAnimation
					, fRWDaySecond/(FLOAT)pLight->m_aoAmbientLightAnimation.GetFramesInCurrentAnim()
					);
			}
		}
		m_strName.RemovePrefix("SunMoon ");
		m_strName = CTString("SunMoon ") + m_strName;
		if(pLight != NULL)
		{
			pLight->m_strName.RemovePrefix("SunMoon ");
			pLight->m_strName = CTString("SunMoon ") + pLight->m_strName;
		}

		if(GetSky())
		{
			GetSky()->GetModelInstance()->SetModelColor(
				GetCurrentSkyAniColor( GetSky()->m_colModelColor )
				);
		}

		jump MainLoop();
	}

	FadeInRain()
	{
		m_bNoon = TRUE;
		m_bNight = TRUE;

		g_bBadWeather = TRUE;
		m_dPrevGWTime = g_fGWTime;
		g_colWeather = C_WHITE_OPAQUE;
		LightProcess();

		while(m_fFadeTime >= 0.0f)
		{
			FLOAT percent = Clamp(1.0f - m_fFadeTime / m_fAllFadeTime, 0.0f, 1.0f);
			g_colWeather = LerpColor( C_WHITE_OPAQUE, m_colRainAmbient, percent) | CT_OPAQUE;
			
			LightProcess();

			m_fFadeTime -= (g_fGWTime-m_dPrevGWTime);
			m_dPrevGWTime = g_fGWTime;
			autowait(0.1f);
		}
		g_colWeather = m_colRainAmbient;
		m_bNoon = m_bNight = FALSE;
		SetLensFlare(LFT_NONE);
		jump MainLoop();
		return;
	}
	FadeOutRain()
	{
		m_bNoon = TRUE;
		m_bNight = TRUE;

		g_bBadWeather = FALSE;
		m_dPrevGWTime = g_fGWTime;
		g_colWeather = m_colRainAmbient;
		LightProcess();

		while(m_fFadeTime >= 0.0f)
		{
			FLOAT percent = Clamp(1.0f - m_fFadeTime / m_fAllFadeTime, 0.0f, 1.0f);
			g_colWeather = LerpColor( m_colRainAmbient, C_WHITE_OPAQUE, percent)| CT_OPAQUE;

			LightProcess();

			m_fFadeTime -= (g_fGWTime-m_dPrevGWTime);
			m_dPrevGWTime = g_fGWTime;
			autowait(0.1f);
		}
		g_colWeather = C_WHITE_OPAQUE;
		m_bNoon = m_bNight = TRUE;
		jump MainLoop();
		return;
	}
	GoodWeather()
	{
		m_bNoon = TRUE;
		m_bNight = TRUE;

		while(TRUE)
		{
			if(GetLight())
			{
				LightProcess();
			}
			autowait(0.100f);
		}
	}

	MainLoop(EVoid)
	{
		m_dPrevGWTime = g_fGWTime;
		m_bNoon = TRUE;
		m_bNight = TRUE;
		LightProcess();

		while(TRUE)
		{
			LightProcess();
			wait()
			{
				on( EBegin ):		{ call GoodWeather(); }
				on( EActivate ):	{ m_bActive = TRUE; resume; }
				on( EDeactivate ):	{ m_bActive = FALSE; resume; }
				on( EStart ):		{
					if(GetEPH() != NULL)
					{
						m_fAllFadeTime = m_fFadeTime = GetEPH()->m_fRainAppearLen * RWTOneSec2GWTSec;
					}
					else
					{
						m_fAllFadeTime = m_fFadeTime = 1.0f * RWTOneSec2GWTSec;
					}
					m_bWeatherSun = TRUE;
					if(GetLight() != NULL && g_colWeather == C_WHITE_OPAQUE)
					{
						jump FadeInRain();
					}
					else
					{
						stop;
					}
				}
				on( EStop ):		{
					if( GetEPH() != NULL )
					{
						m_fAllFadeTime = m_fFadeTime = GetEPH()->m_fRainAppearLen * RWTOneSec2GWTSec;
					}
					else
					{
						m_fAllFadeTime = m_fFadeTime = 1.0f * RWTOneSec2GWTSec;
					}
					m_bWeatherSun = FALSE;
					if(GetLight() != NULL && g_colWeather == m_colRainAmbient)
					{
						jump FadeOutRain();
					}
					else
					{
						stop;
					}
				}
				on( ETrigger et ):	{ stop; }
			};
			m_dPrevGWTime = g_fGWTime;
		}
		int a = 0;
	};
};
//안태훈 수정 끝	//(Add Sun Moon Entity and etc)(0.2)
