
extern INDEX mdl_iShadowQuality;
// model shadow precision
// 0 = no shadows
// 1 = one simple shadow
// 2 = one complex shadow
// 3 = all shadows

//강동민 수정 시작 테스트 클라이언트 작업	06.30
static const float _fWTPlayerDistance = 45.0f;
static const float _fWTCameraDistance = 45.0f;
//강동민 수정 끝 테스트 클라이언트 작업		06.30

/*
 * Compare two models for sorting.
 */

static int qsort_CompareDelayedModels( const void *ppdm0, const void *ppdm1)
{
	const CDelayedModel &dm0 = **(CDelayedModel**)ppdm0;
	const CDelayedModel &dm1 = **(CDelayedModel**)ppdm1;
	const BOOL bHasAlpha0 = dm0.dm_ulFlags & DMF_HASALPHA;
	const BOOL bHasAlpha1 = dm1.dm_ulFlags & DMF_HASALPHA;
	ASSERT( DMF_HASALPHA==1); // we need this flag to be 1 for optimization

	// do some arithmetic mumbo-jumbos to avoid compares
/*	const SLONG slAlphaSgn  =   bHasAlpha0 - bHasAlpha1;
	const SLONG slAlphaDiff = -(bHasAlpha0 ^ bHasAlpha1);   // all 0s if same, all 1s if different
	const FLOAT fDistDiff = dm0.dm_fDistance - dm1.dm_fDistance;
	const SLONG slDistSgn = (SLONG&)fDistDiff;
	// done
	return slAlphaSgn | (~slAlphaDiff & slDistSgn); */

	// 먼저 거리를 가지고 크기를 재고, 거리가 같다면 알파테스트로 앞뒤를 가려야 한다고 생각된다.
	const FLOAT fDistDiff = dm0.dm_fDistance - dm1.dm_fDistance;

	if (fDistDiff > 0)
	{
		if (dm0.dm_penModel->GetAlBackground() && !dm1.dm_penModel->GetAlBackground())
			return -1;

		return 1;
	}
	else if (fDistDiff < 0)
	{
		if (!dm0.dm_penModel->GetAlBackground() && dm1.dm_penModel->GetAlBackground())
			return 1;

		return -1;
	}

	const SLONG slAlphaSgn = bHasAlpha0 - bHasAlpha1;
	return slAlphaSgn;

	/* if both models are opaque or translucent
	if( !bHasAlpha0 == !bHasAlpha1) 
	{
		// sort by distance
		return (dm0.dm_fDistance - dm1.dm_fDistance);
	}
	// else sort by opacity
	return (bHasAlpha0 - bHasAlpha1);   //*/

	/*
	if( !bHasAlpha0 &&  bHasAlpha1) return -1;
	if(  bHasAlpha0 && !bHasAlpha1) return +1;
	if( dm0.dm_fDistance < dm1.dm_fDistance) return -1;
	if( dm0.dm_fDistance > dm1.dm_fDistance) return +1;
	return 0;   //*/
}


static inline FLOAT IntensityAtDistance( FLOAT fFallOff, FLOAT fHotSpot, FLOAT fDistance)
{
	// intensity is zero if further than fall-off range
	if( fDistance>fFallOff) return 0.0f;
	// intensity is maximum if closer than hot-spot range
	if( fDistance<fHotSpot) return 1.0f;
	// interpolate if between fall-off and hot-spot range
	return (fFallOff-fDistance)/(fFallOff-fHotSpot);
}


//안태훈 수정 시작	//(For Performance)(0.1)
void CRenderer::PrepareTerrainLights()
{
	m_vectorLights.clear();
	FOREACHINDYNAMICCONTAINER(re_pwoWorld->wo_cenEntities, CEntity, iten)
	{
		CLightSource *pLS = iten->GetLightSource();
		if(pLS != NULL)
		{
			m_vectorLights.push_back(pLS);
		}
	}
}
//안태훈 수정 끝	//(For Performance)(0.1)

/* Find lights for one model. */
BOOL CRenderer::FindModelLights( CEntity &en, const CPlacement3D &plModel,
								COLOR &colLight, COLOR &colAmbient, FLOAT &fTotalShadowIntensity,
								FLOAT3D &vTotalLightDirection, FLOATplane3D &plFloorPlane)
{
	// NOTE : 그림자를 렌더링할 때, 광원의 방향만 있으면 된다.  나머지 계산을 필요없으므로, 제외시킬것.
	extern BOOL _bRenderProjectionShadow;

	// find shading info if not already cached
	if (en.en_psiShadingInfo!=NULL && !(en.en_ulFlags&ENF_VALIDSHADINGINFO)) 
	{
		if (en.en_ulFlags&ENF_NOSHADINGINFO) 
		{
			if(en.en_psiShadingInfo!=NULL) 
			{
				delete en.en_psiShadingInfo;
				en.en_psiShadingInfo=NULL;
			}
		}
		else 
		{
			en.FindShadingInfo();
		}
	}

	CModelInstance &mi = *en.GetModelInstance();

	int iCount = _amlLights.Count();
	// clear list of active lights
	_amlLights.PopAll();

	// if there is no valid shading info
	if( en.en_psiShadingInfo==NULL/* || en.en_psiShadingInfo->si_pbpoPolygon==NULL*/)
	{ // no shadow
		return FALSE;
	}
	else
	{
		// if there is valid shading info
		if(en.en_psiShadingInfo->si_pbpoPolygon!=NULL) 
		{ 
			// Polygon 위에 있을때...
			// if full bright rendering
			if( _wrpWorldRenderPrefs.wrp_shtShadows==CWorldRenderPrefs::SHT_NONE) 
			{
				// no model shading
				colLight = C_BLACK;
				colAmbient = C_GRAY;
				vTotalLightDirection = FLOAT3D(1.0f, -1.0f, 1.0f);
				return FALSE;
			}
			
			// create floor plane for shadow
			ASSERT(en.en_psiShadingInfo->si_lnInPolygon.IsLinked());
			plFloorPlane = en.en_psiShadingInfo->si_pbpoPolygon->bpo_pbplPlane->bpl_plAbsolute;
			
			// if full bright polygon
			if( en.en_psiShadingInfo->si_pbpoPolygon->bpo_ulFlags&BPOF_FULLBRIGHT) 
			{
				// take light from polygon shadow
				COLOR col  = en.en_psiShadingInfo->si_pbpoPolygon->bpo_colShadow;
	//안태훈 수정 시작	//(Modify Worldbase Overbright to NonOver)(0.1)
				//col |= 0xFFFFFF00;
				colLight   = LerpColor( C_BLACK, col, 0.5f);
				colAmbient = LerpColor( C_BLACK, col, 0.5f);
				fTotalShadowIntensity = NormByteToFloat((en.en_psiShadingInfo->si_pbpoPolygon->bpo_colShadow&CT_AMASK)>>CT_ASHIFT);
				vTotalLightDirection  = FLOAT3D(0.0f, -1.0f, 0.0f);
	//안태훈 수정 끝	//(Modify Worldbase Overbright to NonOver)(0.1)
				return TRUE;
			}
			
			// get the shadow map of the underlying polygon
			CBrushShadowMap &bsm = en.en_psiShadingInfo->si_pbpoPolygon->bpo_smShadowMap;
			// get ambient light
			UBYTE ubAR, ubAG, ubAB;
			ColorToRGB( bsm.GetBrushPolygon()->bpo_pbscSector->bsc_colAmbient, ubAR, ubAG, ubAB);
			SLONG slSAR=ubAR, slSAG=ubAG, slSAB=ubAB;
			
	//안태훈 수정 시작	//(Modify Light Ambient)(0.1)
			FLOAT fAmbientR=0, fAmbientG=0, fAmbientB=0;
	//안태훈 수정 끝	//(Modify Light Ambient)(0.1)
			fTotalShadowIntensity = 0.0f;
			// for each shadow layer
			{FOREACHINLIST(CBrushShadowLayer, bsl_lnInShadowMap, bsm.bsm_lhLayers, itbsl)
			{
				// get the light source
				CLightSource *plsLight = itbsl->bsl_plsLightSource;
				
				// remember the light parameters
				UBYTE ubR, ubG, ubB;
				UBYTE ubDAR, ubDAG, ubDAB;
				plsLight->GetLightColorAndAmbient( ubR, ubG, ubB, ubDAR, ubDAG, ubDAB);
				
				// add directional ambient if needed
	//안태훈 수정 시작	//(Modify Light Ambient)(0.1)
				if( en.en_psiShadingInfo->si_pbpoPolygon->bpo_ulFlags & BPOF_HASDIRECTIONALAMBIENT && plsLight->ls_ulFlags & LSF_DIRECTIONAL ) 
	//안태훈 수정 끝	//(Modify Light Ambient)(0.1)
				{
					slSAR += ubDAR;
					slSAG += ubDAG;
					slSAB += ubDAB;
				}
				
				// get the layer intensity at the point
				FLOAT fShadowFactor;
				if (en.en_ulFlags&ENF_CLUSTERSHADOWS) {
					fShadowFactor = 1.0f;
				}
				else {
					fShadowFactor = itbsl->GetLightStrength( en.en_psiShadingInfo->si_pixShadowU, en.en_psiShadingInfo->si_pixShadowV,
															 en.en_psiShadingInfo->si_fUDRatio,   en.en_psiShadingInfo->si_fLRRatio);
				}
				// skip this light if no intensity
				if( fShadowFactor<0.01f) continue;
				
				const CPlacement3D &plLight = plsLight->ls_penEntity->GetPlacement();
				const FLOAT3D &vLight = plLight.pl_PositionVector;
				// get its parameters at the model position
				FLOAT3D vDirection;
				FLOAT fDistance;
				FLOAT fFallOffFactor;
				
				if (plsLight->ls_ulFlags&LSF_DIRECTIONAL) 
				{
					fFallOffFactor = 1.0f;
					AnglesToDirectionVector(plLight.pl_OrientationAngle, vDirection);
					plModel.pl_PositionVector-vLight;
					if (!(en.en_psiShadingInfo->si_pbpoPolygon->bpo_ulFlags&BPOF_HASDIRECTIONALLIGHT)) 
					{
						ubR = ubG = ubB = 0;
					}
					fDistance = 1.0f;
				}
				else 
				{
					vDirection = plModel.pl_PositionVector-vLight;
					fDistance = vDirection.Length();
					
					if (fDistance>plsLight->ls_rFallOff) {
						continue;
					} else if (fDistance<plsLight->ls_rHotSpot) {
						fFallOffFactor = 1.0f;
					} else {
						fFallOffFactor = (plsLight->ls_rFallOff-fDistance)/
							(plsLight->ls_rFallOff-plsLight->ls_rHotSpot);
	//안태훈 수정 시작	//(Modify Light Ambient)(0.1)
						if(fFallOffFactor < -100 || fFallOffFactor > 100)
						{
							static char buf[256];
							sprintf(buf, "-----------: %f\n", fFallOffFactor);
							OutputDebugString(buf);
						}
					}
					/*fFallOffFactor = Clamp(fFallOffFactor, 0.0f, 1.0f);
					fAmbientR += fFallOffFactor*ubDAR;
					fAmbientG += fFallOffFactor*ubDAG;
					fAmbientB += fFallOffFactor*ubDAB;*/
	//안태훈 수정 끝	//(Modify Light Ambient)(0.1)
				}
				// add the light to active lights
				struct ModelLight &ml = _amlLights.Push();
				ml.ml_plsLight = plsLight;
				// normalize direction vector
				if (fDistance>0.001f) {
					ml.ml_vDirection = vDirection/fDistance;
				} else {
					ml.ml_vDirection = FLOAT3D(0.0f,0.0f,0.0f);
				}
				// special case for substract sector ambient light
				if (plsLight->ls_ulFlags&LSF_SUBSTRACTSECTORAMBIENT) 
				{
					ubR = (UBYTE)Clamp( (SLONG)ubR-slSAR, 0L, 255L);
					ubG = (UBYTE)Clamp( (SLONG)ubG-slSAG, 0L, 255L);
					ubB = (UBYTE)Clamp( (SLONG)ubB-slSAB, 0L, 255L);
				}
				// calculate light intensity
				FLOAT fShade = (ubR+ubG+ubB)*(2.0f/(3.0f*255.0f));
				ml.ml_fShadowIntensity = fShade*fShadowFactor;
				fTotalShadowIntensity += ml.ml_fShadowIntensity;
				// special case for dark light
				if (plsLight->ls_ulFlags&LSF_DARKLIGHT) {
					ml.ml_fR = -ubR*fFallOffFactor; 
					ml.ml_fG = -ubG*fFallOffFactor; 
					ml.ml_fB = -ubB*fFallOffFactor;
				} else {
					ml.ml_fR = +ubR*fFallOffFactor; 
					ml.ml_fG = +ubG*fFallOffFactor; 
					ml.ml_fB = +ubB*fFallOffFactor;
				}
			}}
			
			FLOAT fTR=0.0f; FLOAT fTG=0.0f; FLOAT fTB=0.0f;
			FLOAT3D vDirection(0.0f,0.0f,0.0f);
			// for each active light
			{for(INDEX iLight=0; iLight<_amlLights.Count(); iLight++) 
			{
				struct ModelLight &ml = _amlLights[iLight];
				// add it to total intensity
				fTR += ml.ml_fR;
				fTG += ml.ml_fG;
				fTB += ml.ml_fB;
				// add it to direction vector
				FLOAT fWeight = Abs(ml.ml_fR+ml.ml_fG+ml.ml_fB) * (1.0f/(3.0f*255.0f));
				vDirection+=ml.ml_vDirection*fWeight;
			}}

	//안태훈 수정 시작	//(Modify Light Ambient)(0.1)
	/*			fTR += Clamp(fAmbientR, 0.0f, 255.0f);
			fTG += Clamp(fAmbientG, 0.0f, 255.0f);
			fTB += Clamp(fAmbientB, 0.0f, 255.0f);*/
	//안태훈 수정 끝	//(Modify Light Ambient)(0.1)
			// normalize average direction vector
			FLOAT fDirection = vDirection.Length();
			if (fDirection>0.001f) {
				vDirection /= fDirection;
			} else {
				vDirection = FLOAT3D(0.0f,0.0f,0.0f);
			}
			
			// for each active light
			FLOAT fDR=0.0f; FLOAT fDG=0.0f; FLOAT fDB=0.0f;
			{for(INDEX iLight=0; iLight<_amlLights.Count(); iLight++) {
				struct ModelLight &ml = _amlLights[iLight];
				// find its contribution to direction vector
				const FLOAT fFactor = ClampDn( vDirection%ml.ml_vDirection, 0.0f);
				// add it to directional intensity
				fDR += ml.ml_fR*fFactor;
				fDG += ml.ml_fG*fFactor;
				fDB += ml.ml_fB*fFactor;
			}}
			
			// adjust ambient light with gradient if needed
			ULONG ulGradientType = en.en_psiShadingInfo->si_pbpoPolygon->bpo_bppProperties.bpp_ubGradientType;
			if( ulGradientType>0) 
			{
				CGradientParameters gp;
				COLOR colGradientPoint;
				CEntity *pen = en.en_psiShadingInfo->si_pbpoPolygon->bpo_pbscSector->bsc_pbmBrushMip->bm_pbrBrush->br_penEntity;
				if( pen!=NULL && pen->GetGradient( ulGradientType, gp)) {
					FLOAT fGrPt = (en.en_psiShadingInfo->si_vNearPoint % gp.gp_vGradientDir - gp.gp_fH0) / (gp.gp_fH1-gp.gp_fH0);
					fGrPt = Clamp( fGrPt, 0.0f, 1.0f);
					colGradientPoint = LerpColor( gp.gp_col0, gp.gp_col1, fGrPt);
					UBYTE ubGR,ubGG,ubGB;
					ColorToRGB( colGradientPoint, ubGR,ubGG,ubGB);
					// add or substract gradient component to total ambient
					if( gp.gp_bDark) { slSAR-=ubGR;  slSAG-=ubGG;  slSAB-=ubGB; }
					else             { slSAR+=ubGR;  slSAG+=ubGG;  slSAB+=ubGB; }
				}
			}
			// clamp ambient component
			slSAR = Clamp( slSAR, 0L, 255L);
			slSAG = Clamp( slSAG, 0L, 255L);
			slSAB = Clamp( slSAB, 0L, 255L);
			
			// calculate average light properties
			SLONG slAR = Clamp( (SLONG)FloatToInt(fTR-fDR) +slSAR, 0L, 255L);
			SLONG slAG = Clamp( (SLONG)FloatToInt(fTG-fDG) +slSAG, 0L, 255L);
			SLONG slAB = Clamp( (SLONG)FloatToInt(fTB-fDB) +slSAB, 0L, 255L);
			SLONG slLR = Clamp( (SLONG)FloatToInt(fDR), 0L, 255L);
			SLONG slLG = Clamp( (SLONG)FloatToInt(fDG), 0L, 255L);
			SLONG slLB = Clamp( (SLONG)FloatToInt(fDB), 0L, 255L);
			colLight   = RGBToColor( slLR,slLG,slLB);
			colAmbient = RGBToColor( slAR,slAG,slAB);
			
			// adjust for changed polygon shadow color
			COLOR colShadowMap = en.en_psiShadingInfo->si_pbpoPolygon->bpo_colShadow;
			CTextureBlending &tbShadow = re_pwoWorld->wo_atbTextureBlendings[
				en.en_psiShadingInfo->si_pbpoPolygon->bpo_bppProperties.bpp_ubShadowBlend];
			COLOR colShadowMapAdjusted = MulColors(colShadowMap, tbShadow.tb_colMultiply);
			colLight   = MulColors( colLight,   colShadowMapAdjusted);
			colAmbient = MulColors( colAmbient, colShadowMapAdjusted);
			vTotalLightDirection = vDirection;
			// else no valid shading info
		}
		else
		{
			// if full bright rendering
			if (_wrpWorldRenderPrefs.wrp_shtShadows==CWorldRenderPrefs::SHT_NONE) 
			{
				// no model shading
				colLight = C_BLACK;
				colAmbient = C_GRAY;
				vTotalLightDirection = FLOAT3D(1.0f, -1.0f, 1.0f);
				return FALSE;
			}

			CWorld *pwld = en.GetWorld();
			CBrushShadowMap &bsm = en.en_psiShadingInfo->si_pbpoPolygon->bpo_smShadowMap;
			// get ambient light
			SLONG slSAR=0, slSAG=0, slSAB=0;
	//안태훈 수정 시작	//(Modify Light Ambient)(0.1)
			FLOAT fAmbientR=0, fAmbientG=0, fAmbientB=0;
	//안태훈 수정 끝	//(Modify Light Ambient)(0.1)
			fTotalShadowIntensity = 0.0f;
			{
	//안태훈 수정 시작	//(For Performance)(0.2)
				for(INDEX i=0; i<pwld->m_vectorLights.size(); ++i)
				{
					// get the light source
					CLightSource *plsLight = pwld->m_vectorLights[i]->GetLightSource();
	//안태훈 수정 끝	//(For Performance)(0.2)
					if(plsLight == NULL) continue;
					if(plsLight->ls_ulFlags & LSF_LENSFLAREONLY) continue;
					
					// remember the light parameters
					UBYTE ubR, ubG, ubB;
					UBYTE ubDAR, ubDAG, ubDAB;
					plsLight->GetLightColorAndAmbient( ubR, ubG, ubB, ubDAR, ubDAG, ubDAB);
					
	//안태훈 수정 시작	//(Modify Light Ambient)(0.1)
					// add directional ambient
					if( plsLight->ls_ulFlags & LSF_DIRECTIONAL ) 
					{
						slSAR += ubDAR;
						slSAG += ubDAG;
						slSAB += ubDAB;
					}
	//안태훈 수정 끝	//(Modify Light Ambient)(0.1)
					
					// get the layer intensity at the point
					FLOAT fShadowFactor;
					if (en.en_ulFlags&ENF_CLUSTERSHADOWS) 
					{
						fShadowFactor = 1.0f;
					}
					else 
					{
						fShadowFactor = 1.0;
					}
					// skip this light if no intensity
					if( fShadowFactor<0.01f) continue;
					
					const CPlacement3D &plLight = plsLight->ls_penEntity->GetPlacement();
					const FLOAT3D &vLight = plLight.pl_PositionVector;
					// get its parameters at the model position
					FLOAT3D vDirection;
					FLOAT fDistance;
					FLOAT fFallOffFactor;
					
					if (plsLight->ls_ulFlags&LSF_DIRECTIONAL) 
					{
						fFallOffFactor = 1.0f;
						AnglesToDirectionVector(plLight.pl_OrientationAngle, vDirection);
						plModel.pl_PositionVector-vLight;
						fDistance = 1.0f;
					}
					else 
					{
						vDirection = plModel.pl_PositionVector-vLight;
						fDistance = vDirection.Length();
						
						if (fDistance>plsLight->ls_rFallOff) 
						{
							continue;
						}
						else if (fDistance<plsLight->ls_rHotSpot) 
						{
							fFallOffFactor = 1.0f;
						}
						else 
						{
							fFallOffFactor = (plsLight->ls_rFallOff-fDistance)/
								(plsLight->ls_rFallOff-plsLight->ls_rHotSpot);
	//안태훈 수정 시작	//(Modify Light Ambient)(0.1)
							if(fFallOffFactor < -100 || fFallOffFactor > 100)
							{
								static char buf[256];
								sprintf(buf, "-----------: %f\n", fFallOffFactor);
								OutputDebugString(buf);
							}
						}
						fFallOffFactor = Clamp(fFallOffFactor, 0.0f, 1.0f);
						fAmbientR += fFallOffFactor*ubDAR;
						fAmbientG += fFallOffFactor*ubDAG;
						fAmbientB += fFallOffFactor*ubDAB;
	//안태훈 수정 끝	//(Modify Light Ambient)(0.1)
					}
					// add the light to active lights
					struct ModelLight &ml = _amlLights.Push();
					ml.ml_plsLight = plsLight;
					// normalize direction vector
					if (fDistance>0.001f) 
					{
						ml.ml_vDirection = vDirection/fDistance;
					}
					else 
					{
						ml.ml_vDirection = FLOAT3D(0.0f,0.0f,0.0f);
					}
					// special case for substract sector ambient light
					if (plsLight->ls_ulFlags&LSF_SUBSTRACTSECTORAMBIENT) 
					{
						ubR = (UBYTE)Clamp( (SLONG)ubR-slSAR, 0L, 255L);
						ubG = (UBYTE)Clamp( (SLONG)ubG-slSAG, 0L, 255L);
						ubB = (UBYTE)Clamp( (SLONG)ubB-slSAB, 0L, 255L);
					}
					// calculate light intensity
					FLOAT fShade = (ubR+ubG+ubB)*(2.0f/(3.0f*255.0f));
					ml.ml_fShadowIntensity = fShade*fShadowFactor;
					fTotalShadowIntensity += ml.ml_fShadowIntensity;
					// special case for dark light
					if (plsLight->ls_ulFlags&LSF_DARKLIGHT) 
					{
						ml.ml_fR = -ubR*fFallOffFactor; 
						ml.ml_fG = -ubG*fFallOffFactor; 
						ml.ml_fB = -ubB*fFallOffFactor;
					}
					else 
					{
						ml.ml_fR = +ubR*fFallOffFactor; 
						ml.ml_fG = +ubG*fFallOffFactor; 
						ml.ml_fB = +ubB*fFallOffFactor;
					}
				}
			}
			FLOAT fTR=0.0f; FLOAT fTG=0.0f; FLOAT fTB=0.0f;
			FLOAT3D vDirection(0.0f,0.0f,0.0f);
			// for each active light
			{
				for(INDEX iLight=0; iLight<_amlLights.Count(); iLight++) 
				{
					struct ModelLight &ml = _amlLights[iLight];
					// add it to total intensity
					fTR += ml.ml_fR;
					fTG += ml.ml_fG;
					fTB += ml.ml_fB;
					// add it to direction vector
					FLOAT fWeight = Abs(ml.ml_fR+ml.ml_fG+ml.ml_fB) * (1.0f/(3.0f*255.0f));
					vDirection+=ml.ml_vDirection*fWeight;
				}
			}
	//안태훈 수정 시작	//(Modify Light Ambient)(0.1)
			fTR += Clamp(fAmbientR, 0.0f, 255.0f);
			fTG += Clamp(fAmbientG, 0.0f, 255.0f);
			fTB += Clamp(fAmbientB, 0.0f, 255.0f);
	//안태훈 수정 끝	//(Modify Light Ambient)(0.1)
			// normalize average direction vector
			FLOAT fDirection = vDirection.Length();
			if (fDirection>0.001f) 
			{
				vDirection /= fDirection;
			}
			else 
			{
				vDirection = FLOAT3D(0.0f,0.0f,0.0f);
			}

			// for each active light
			FLOAT fDR=0.0f; FLOAT fDG=0.0f; FLOAT fDB=0.0f;
			{
				for(INDEX iLight=0; iLight<_amlLights.Count(); iLight++) 
				{
					struct ModelLight &ml = _amlLights[iLight];
					// find its contribution to direction vector
					const FLOAT fFactor = ClampDn( vDirection%ml.ml_vDirection, 0.0f);
					// add it to directional intensity
					fDR += ml.ml_fR*fFactor;
					fDG += ml.ml_fG*fFactor;
					fDB += ml.ml_fB*fFactor;
				}
			}

			// clamp ambient component
			slSAR = Clamp( slSAR, 0L, 255L);
			slSAG = Clamp( slSAG, 0L, 255L);
			slSAB = Clamp( slSAB, 0L, 255L);
			
			// calculate average light properties
			SLONG slAR = Clamp( (SLONG)FloatToInt(fTR-fDR) +slSAR, 0L, 255L);
			SLONG slAG = Clamp( (SLONG)FloatToInt(fTG-fDG) +slSAG, 0L, 255L);
			SLONG slAB = Clamp( (SLONG)FloatToInt(fTB-fDB) +slSAB, 0L, 255L);
			SLONG slLR = Clamp( (SLONG)FloatToInt(fDR), 0L, 255L);
			SLONG slLG = Clamp( (SLONG)FloatToInt(fDG), 0L, 255L);
			SLONG slLB = Clamp( (SLONG)FloatToInt(fDB), 0L, 255L);
			colLight   = RGBToColor( slLR,slLG,slLB);
			colAmbient = RGBToColor( slAR,slAG,slAB);
			vTotalLightDirection = vDirection;
			//안태훈 수정 끝	//(Modify Light Process On Terrain)(0.1)

			if (en.en_psiShadingInfo->si_ptrTerrain != NULL)
			{
				// Terrain의 정보를 갖고 있을 경우...
				CTerrain *ptrTerrain = en.en_psiShadingInfo->si_ptrTerrain;
				Matrix12 mTerrain;
				TR_GetMatrixFromEntity(mTerrain,ptrTerrain);
				plFloorPlane = TR_GetPlaneFromPoint(ptrTerrain, mTerrain, en.en_psiShadingInfo->si_vNearPoint);
			}
		}
	}
	// has shadow
	return TRUE;
}


/*
 * Render one model with shadow (eventually)
 */
void CRenderer::RenderOneModel( CEntity &en, CModelObject &moModel, const CPlacement3D &plModel,
								const FLOAT fDistanceFactor, BOOL bRenderShadow, ULONG ulDMFlags)
{
	// skip invisible models
	if( moModel.mo_Stretch==FLOAT3D(0,0,0)) return;
	
	// do nothing, if rendering shadows and this model doesn't cast cluster shadows
	if( re_bRenderingShadows && !(en.en_ulFlags&ENF_CLUSTERSHADOWS)) return;

	// create a default light
	COLOR colLight   = C_GRAY;
	COLOR colAmbient = C_dGRAY;
	FLOAT3D vTotalLightDirection( 1.0f, -1.0f, 1.0f);
	FLOATplane3D plFloorPlane(FLOAT3D( 0.0f, 1.0f, 0.0f), 0.0f);
	FLOAT fTotalShadowIntensity = 0.0f;
	BOOL bRenderModelShadow = FALSE;

	// if not rendering cluster shadows
	if( !re_bRenderingShadows) 
	{
		mdl_iShadowQuality = Clamp( mdl_iShadowQuality, 0L, 3L);
		// find model lights only if not custom shaded
		if( en.IsFlagOff(ENF_CUSTOMSHADING)) 
		{
			bRenderModelShadow = FindModelLights( en, plModel, colLight, colAmbient,
												fTotalShadowIntensity, vTotalLightDirection, plFloorPlane);
		} // let the entity adjust shading parameters if it wants to
		const BOOL bAllowShadows = en.AdjustShadingParameters( vTotalLightDirection, colLight, colAmbient);
		bRenderModelShadow = (bRenderModelShadow && bAllowShadows && bRenderShadow && mdl_iShadowQuality>0);
	}

	// skip if fully transparent (MUST BE TESTED AFTER AdjustShadingParameters()!)
	const SLONG slModelAlpha = (moModel.mo_colBlendColor & CT_AMASK) >>CT_ASHIFT;
	if( slModelAlpha<4) return;

	// prepare render model structure
	CRenderModel rm;
	rm.rm_vLightDirection = vTotalLightDirection;
	rm.rm_fDistanceFactor = fDistanceFactor;
	rm.rm_colLight   = colLight;
	rm.rm_colAmbient = colAmbient;
	rm.SetObjectPlacement(plModel);
	if( ulDMFlags & DMF_FOG)      rm.rm_ulFlags |= RMF_FOG;
	if( ulDMFlags & DMF_HAZE)     rm.rm_ulFlags |= RMF_HAZE;
	if( ulDMFlags & DMF_MIRROR)   rm.rm_ulFlags |= RMF_MIRROR;
	if( ulDMFlags & DMF_INSIDE)   rm.rm_ulFlags |= RMF_INSIDE;
	if( ulDMFlags & DMF_INMIRROR) rm.rm_ulFlags |= RMF_INMIRROR;

	// mark that we don't actualy need entire model
	if( re_penViewer==&en) 
	{
		rm.rm_ulFlags |= RMF_SPECTATOR;
		bRenderModelShadow = FALSE;
	}

	// disable Truform usage on weapon models
	if( _pGfx->gl_iTessellationLevel>0 && IsOfClass( &en, "Player Weapons")) rm.rm_ulFlags |= RMF_WEAPON; 

	// set tesselation level of models
	rm.rm_iTesselationLevel = en.GetMaxTessellationLevel();

	// prepare CRenderModel structure for rendering of one model
	moModel.SetupModelRendering(rm);

	// determine shadow intensity
	fTotalShadowIntensity *= NormByteToFloat(slModelAlpha);
	fTotalShadowIntensity  = Clamp( fTotalShadowIntensity, 0.0f, 1.0f);

	// if should render shadow for this model
	if( bRenderModelShadow && !(en.en_ulFlags&ENF_CLUSTERSHADOWS) && moModel.HasShadow(rm.rm_iMipLevel)) 
	{
		// if only simple shadow
		if( mdl_iShadowQuality==1) 
		{
			// render simple shadow
			fTotalShadowIntensity = 0.1f + fTotalShadowIntensity*0.9f;
			moModel.AddSimpleShadow( rm, fTotalShadowIntensity, plFloorPlane);
		}
		// if only one shadow
		else if( mdl_iShadowQuality==2) 
		{
			// render one shadow of model from shading light direction
			const FLOAT fHotSpot = 1E10f;
			const FLOAT fFallOff = 1E11f;
			CPlacement3D plLight;
			plLight.pl_PositionVector = plModel.pl_PositionVector - rm.rm_vLightDirection*1000.0f;
			moModel.RenderShadow( rm, plLight, fFallOff, fHotSpot, fTotalShadowIntensity, plFloorPlane);
		}
		// if full shadows
		else if( mdl_iShadowQuality==3) 
		{
			// for each active light
			for( INDEX iLight=0; iLight<_amlLights.Count(); iLight++) 
			{
				struct ModelLight &ml = _amlLights[iLight];
				// skip light if doesn't cast shadows
				if( !(ml.ml_plsLight->ls_ulFlags&LSF_CASTSHADOWS)) continue;
				// get light parameters
				CPlacement3D plLight = ml.ml_plsLight->ls_penEntity->en_plPlacement;
				FLOAT fHotSpot = ml.ml_plsLight->ls_rHotSpot;
				FLOAT fFallOff = ml.ml_plsLight->ls_rFallOff;
				if (ml.ml_plsLight->ls_ulFlags & LSF_DIRECTIONAL) 
				{
					fHotSpot = 1E10f;
					fFallOff = 1E11f;
					FLOAT3D vDirection;
					AnglesToDirectionVector( plLight.pl_OrientationAngle, vDirection);
					plLight.pl_PositionVector = plModel.pl_PositionVector-(vDirection*1000.0f);
				} 
				// render one shadow of model
				const FLOAT fShadowIntensity  = Clamp( ml.ml_fShadowIntensity, 0.0f, 1.0f);
				moModel.RenderShadow( rm, plLight, fFallOff, fHotSpot, fShadowIntensity, plFloorPlane);
			}
		}
	}

	// if the entity is not the viewer, or this is not primary renderer
	if( re_penViewer!=&en) 
	{
		// mask in the need for skipping the occlusion test
		moModel.mo_bSkipOcclusionTest |= (ulDMFlags & DMF_VERYNEAR);
		// render model
		moModel.RenderModel(rm);
	// if the entity is viewer
	}
	else 
	{
		// just remember the shading info (needed for first-person-weapon rendering)
		_vViewerLightDirection = rm.rm_vLightDirection;
		_colViewerLight   = rm.rm_colLight;
		_colViewerAmbient = rm.rm_colAmbient;
	}
}

/*
 * Render one ska model with shadow (eventually)
 */
void CRenderer::RenderOneSkaModel( CEntity &en, const CPlacement3D &plModel,
																	 BOOL bRenderShadow, FLOAT fDMDistance, ULONG ulDMFlags)
{
	extern INDEX g_iShadowDetail;

	// skip invisible models
	CModelInstance &mi = *en.GetModelInstance();
	if( mi.mi_vStretch==FLOAT3D(0,0,0)) return;
	
	// do nothing, if rendering shadows and this model doesn't cast cluster shadows
	if( re_bRenderingShadows && !(en.en_ulFlags&ENF_CLUSTERSHADOWS)) return;

	// create a default light
	COLOR colLight   = C_GRAY;
	COLOR colAmbient = C_dGRAY;
	FLOAT3D vTotalLightDirection( 1.0f, -1.0f, 1.0f);
	FLOATplane3D plFloorPlane(FLOAT3D( 0.0f, 1.0f, 0.0f), 0.0f);

	FLOAT fTotalShadowIntensity = 0.0f;	// 원본
	BOOL bRenderModelShadow = FALSE;		// 원본
	//mi.mi_bHasShadow = FALSE;
	//mi.mi_fShadowIntensity = 0.0f;

	// NOTE : 그림자가 실제로 그려지는 부분은 Terrain이므로, 
	// NOTE : 아래에서 그림자의 렌더링 유무 및 Intensity를 결정하고,
	// NOTE : 그 값을 바탕으로 Terrain에서 그림자를 그립니다.
	// if not rendering cluster shadows
	if( !re_bRenderingShadows)
	{
		mdl_iShadowQuality = Clamp( mdl_iShadowQuality, 0L, 3L);
		// find model lights only if not custom shaded
		if( en.IsFlagOff(ENF_CUSTOMSHADING))
		{
			bRenderModelShadow = FindModelLights( en, plModel, colLight, colAmbient, 
				fTotalShadowIntensity, vTotalLightDirection, plFloorPlane);
	//		mi.mi_bHasShadow = FindModelLights( en, plModel, colLight, colAmbient,
	//			mi.mi_fShadowIntensity, vTotalLightDirection, plFloorPlane);
		} // let the entity adjust shading parameters if it wants to
		const BOOL bAllowShadows = en.AdjustShadingParameters( vTotalLightDirection, colLight, colAmbient);
		bRenderModelShadow = (bRenderModelShadow && bAllowShadows && bRenderShadow && mdl_iShadowQuality>0);
		//mi.mi_bHasShadow = (mi.mi_bHasShadow && bAllowShadows && bRenderShadow && mdl_iShadowQuality>0);
	}

	// skip if fully transparent (MUST BE TESTED AFTER AdjustShadingParameters()!)
	const SLONG slModelAlpha = (mi.mi_colModelColor & CT_AMASK) >>CT_ASHIFT;
	if( slModelAlpha<4) return;

	ULONG &ulRenFlags = RM_GetRenderFlags();
	ulRenFlags = NONE;
	if( ulDMFlags & DMF_FOG)      ulRenFlags |= SRMF_FOG;
	if( ulDMFlags & DMF_HAZE)     ulRenFlags |= SRMF_HAZE;
	if( ulDMFlags & DMF_MIRROR)   ulRenFlags |= SRMF_MIRROR;
	if( ulDMFlags & DMF_INSIDE)   ulRenFlags |= SRMF_INSIDE;
	if( ulDMFlags & DMF_INMIRROR) ulRenFlags |= SRMF_INMIRROR;
	
	// mark that we don't actualy need entire model
	if( re_penViewer==&en) 
	{
		ulRenFlags |= SRMF_SPECTATOR;
		bRenderModelShadow = FALSE;		// 원본
		//mi.mi_bHasShadow = FALSE;
	}

	RM_SetObjectPlacement(plModel);
	RM_SetLightColor(colAmbient,colLight);
	RM_SetLightDirection(vTotalLightDirection);

	// determine shadow intensity
	fTotalShadowIntensity *= NormByteToFloat(slModelAlpha);				// 원본
	fTotalShadowIntensity  = Clamp( fTotalShadowIntensity, 0.0f, 1.0f);	// 원본
	//mi.mi_fShadowIntensity *= NormByteToFloat(slModelAlpha);
	//mi.mi_fShadowIntensity = Clamp( mi.mi_fShadowIntensity, 0.0f, 1.0f);

	if( bRenderModelShadow && 
		!(en.en_ulFlags&ENF_CLUSTERSHADOWS) && 
		!(en.en_ulFlags&ENF_NICEWATER) && 
		!(en.en_ulFlags&ENF_ITEM) &&
		mi.HasShadow(1) && 
		!re_prProjection->pr_bNiceWater &&
		( g_iShadowDetail == SHADOW_SIMPLE_ONLY || 
		g_iShadowDetail == SHADOW_SIMPLE_ALL )
		)
	{
		// if only simple shadow
		if( mdl_iShadowQuality>0) 
	{
			// render simple shadow
			fTotalShadowIntensity = 0.1f + fTotalShadowIntensity*0.9f;

			/*
			if(en.en_psiShadingInfo->si_ptrTerrain!=NULL) 
			{				
				// 시현에서는 다음 줄을 주석 처리.
				en.GetModelInstance()->AddSimpleShadow(fTotalShadowIntensity, plFloorPlane);
			}
			// 추가한 부분.
			// Terrain위에 있지 않다면 둥근 그림자로 처리합니다.
			else
			{				
			*/			
				// 플레이어 그림자만 궁근걸로...
				if(g_iShadowDetail == SHADOW_SIMPLE_ONLY)
				{
					if(en.IsPlayer())
				{
						en.GetModelInstance()->AddSimpleShadow(fTotalShadowIntensity, plFloorPlane);
				}
			}
				else
				{
					en.GetModelInstance()->AddSimpleShadow(fTotalShadowIntensity, plFloorPlane);
		}
			//}
	}
	}

	// if the entity is not the viewer, or this is not primary renderer
	if( re_penViewer!=&en) 
	{
		// Call callback func for adjusting skaleton bones
		RM_SetBoneAdjustCallback(&EntityAdjustBonesCallback,&en);
		// Call callback func for adjusting shading params
		RM_SetShaderParamsAdjustCallback(&EntityAdjustShaderParamsCallback,&en);
		// Set current distance
		RM_SetCurrentDistance(fDMDistance);
		// render model
		CModelInstance &mi = *en.GetModelInstanceForRendering();
		// mask in the need for skipping the occlusion test
		mi.mi_bSkipOcclusionTest |= (ulDMFlags & DMF_VERYNEAR);

		extern INDEX gfx_bRenderReflection;
		if(gfx_bRenderReflection)
		{
			RM_RenderSKA(mi, FALSE);
		}
		else
		RM_RenderSKA(mi, TRUE);
//안태훈 수정 시작	//(Easy Use World Editor)(0.1)
		extern INDEX ska_bShowTriangles;
		if(ska_bShowTriangles) RM_PreviewSKA(mi, re_pdpDrawPort, SPF_WIREFRAME);
//안태훈 수정 끝	//(Easy Use World Editor)(0.1)
	// if the entity is viewer
	}
	else 
	{
		// just remember the shading info (needed for first-person-weapon rendering)
		_vViewerLightDirection = vTotalLightDirection;
		_colViewerLight   = colLight;
		_colViewerAmbient = colAmbient;
	}
}

void CRenderer::RenderOneSkaModelToTexture(CEntity &en, const CPlacement3D &plModel, const CPlacement3D &plLight,
										   BOOL bRenderShadow, FLOAT fDMDistance, ULONG ulDMFlags, BOOL bIsDirectional)
{
	//if( re_bRenderingShadows && !(en.en_ulFlags&ENF_PROJECTIONSHADOWS)) 
	//	return;

 	float fTextureSize = SHADOWTEXTURESIZE;		// 임시 변수.

	// skip invisible models
	// 안보이는 모델은 스킵.
	CModelInstance &mi = *en.GetModelInstance();
	//if( mi.mi_vStretch==FLOAT3D(0,0,0)) return;
	
	// skip if fully transparent (MUST BE TESTED AFTER AdjustShadingParameters()!)
	// 완전 투명이면 스킵.
	//const SLONG slModelAlpha = (mi.mi_colModelColor & CT_AMASK) >>CT_ASHIFT;
	//if( slModelAlpha<4) return;

	//-----------------------------------------------------------------------------
	// 광원에서 원점을 바라보도록 처리...
	// Projection 정보를 설정합니다.
	// NOTE : Projection.Prepare()은 복잡한 계산을 수행하고 있다.
	// NOTE : 쓸모없는 부분은 빼네고 나중에 함수로 만들던지 해야할듯...
	GetViewMatrix(plLight, _matWorldToLight);

	//-----------------------------------------------------------------------------
	// NOTE : 적당한 FOV값을 계산하기 위해서, 모델의 높이의 절반만큼 이동시키는 부분.
	// NOTE : 모델의 높이를 구하는 부분은 광원의 위치 설정등에 자주 쓰일 가능성이 있기 때문에,
	// NOTE : 미리 계산해놓는 방향으로 처리해야 할듯.
	FLOATaabbox3D FrameBBox;
	mi.GetAllFramesBBox(FrameBBox);
	FrameBBox.StretchByVector(mi.mi_vStretch);
	FLOAT3D vMin = FrameBBox.minvect;
	FLOAT3D vMax = FrameBBox.maxvect;
	FLOAT fHeight = fabs((vMax(2) - vMin(2))/2.0f);

	Matrix12 matTrans;
	MakeIdentityMatrix(matTrans);
	matTrans[3] = 0.0f;
	matTrans[7] = -fHeight;
	matTrans[11] = 0.0f;
	
	//-----------------------------------------------------------------------------
	// 광원에서...
	// 투영행렬을 만들기 위한 계산을 수행함.
//강동민 수정 시작 접속 시퀀스 작업	06.07
	/*
	SetAdjustedProjectionMatrix(_matShadowProj, D3DX_PI/4.0f, 1.0f, 0.05f, 500.0f,
		0.0f, 0.0f, fTextureSize, fTextureSize);
		*/
	/*
	// 직교투영으로 적용해보기.
	//D3DXMatrixOrthoRH((D3DXMATRIX*)matShadowProj, 0.98f/fXMax, 0.98f/fYMax, 1.0f, 500.0f);
	D3DXMatrixOrthoOffCenterRH((D3DXMATRIX*)matShadowProj, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 500.0f);
	matShadowProj[0] = 0.12f/fXMax;
	matShadowProj[5] = 0.12f/fYMax;
	*/
//강동민 수정 끝 접속 시퀀스 작업	06.07
	
	Matrix12 matTemp;
	MatrixMultiply(matTemp, _matWorldToLight, matTrans);

//강동민 수정 시작 접속 시퀀스 작업	06.07
	/*
	FLOAT fXMax, fYMax;
	FindBestFOV(FrameBBox, matTemp, fXMax, fYMax);		
	
	fXMax = Max(fXMax, fYMax);
	fYMax = fXMax;

	_matShadowProj[0] = 0.98f/fXMax;
	_matShadowProj[5] = 0.98f/fYMax;
	*/
//강동민 수정 끝 접속 시퀀스 작업	06.07

	FLOAT fBBoxXSize		= vMax(1) - vMin(1);
	FLOAT fBBoxYSize		= vMax(2) - vMin(2);
	FLOAT fBBoxZSize		= vMax(3) - vMin(3);
	FLOAT fMaxValue			= Max(Max(fBBoxXSize, fBBoxYSize), fBBoxZSize) * 1.5f;

	// 직교 투영.
	D3DXMatrixOrthoRH(
		(D3DXMATRIX*)_matShadowProj, 
		fMaxValue, fMaxValue,			// FrameBBox의 Width
		0.1f, 300.0f);					// FrameBBox의 Height

	// NOTE : 직접적으로 투영행렬을 설정하는 부분.  Serious 엔진에서 제공하는 함수를 이용하는 방향으로 변환할것.
	HRESULT hr = _pGfx->gl_pd3dDevice->SetTransform( D3DTS_PROJECTION, (const _D3DMATRIX*)&_matShadowProj);
	D3D_CHECKERROR(hr);
	
	//-----------------------------------------------------------------------------
	// 모델을 렌더링할 때 사용할 VIEW 행렬로 지정합니다.
	// World -> View
	RM_SetAbsToView(_matWorldToLight);

	//-----------------------------------------------------------------------------
	// 오브젝트의 월드 행렬 설정.
	RM_SetObjectPlacement(CPlacement3D(FLOAT3D(0.0f,-fHeight,0.0f), plModel.pl_OrientationAngle));

	// Call callback func for adjusting skaleton bones
	RM_SetBoneAdjustCallback(&EntityAdjustBonesCallback,&en);
	// Call callback func for adjusting shading params
	RM_SetShaderParamsAdjustCallback(&EntityAdjustShaderParamsCallback,&en);
	// Set current distance
	RM_SetCurrentDistance(fDMDistance);

	// NOTE : 추후에 RM_PreviewSKA()로 변경할것.
	// NOTE : RM_PreviewSKA()는 인자가 너무 많이 필요함.  고로, 보류...
	RM_RenderSKA(mi);
}

/* 
 * Render models that were kept for delayed rendering.
 */
void CRenderer::RenderModels( BOOL bBackground)
{
	if( _bMultiPlayer) gfx_bRenderModels = 1; // must render in multiplayer mode!
	if( !gfx_bRenderModels && !re_bRenderingShadows) return;

	/* debug
	extern CTString _strDebug;
	extern PIX _pixDebugStringX, _pixDebugStringY;
	_pixDebugStringX = 128;                               
	_pixDebugStringY = 48;
	_strDebug.PrintF( "%d", re_admDelayedModels.Count()); //*/

	// sort all the delayed models by distance
	qsort( re_admDelayedModels.GetArrayOfPointers(), re_admDelayedModels.Count(),
				 sizeof(CDelayedModel*), qsort_CompareDelayedModels);

	extern BOOL _bTranslucentModel;
	extern BOOL _bWorldEditorApp;
	UWORD* wAlpha =NULL;
	if( _bWorldEditorApp && _bTranslucentModel )
	{
		wAlpha = new WORD[re_admDelayedModels.Count()];

		for( INDEX iModel=0; iModel<re_admDelayedModels.Count(); ++iModel)
		{	
			CDelayedModel &dm = re_admDelayedModels[iModel];
			CEntity &en = *dm.dm_penModel;

			CModelInstance &mi = *en.GetModelInstance();
			wAlpha[iModel] =mi.mi_colModelColor & 0xFF;
			mi.mi_colModelColor =mi.mi_colModelColor & ~0xFF;
			mi.mi_colModelColor =mi.mi_colModelColor | 0x77;
		}
	}

	CAnyProjection3D *papr;
	if( bBackground) 
	{
		papr = &re_prBackgroundProjection;
	}
	else 
	{
		papr = &re_prProjection;
	}

	// begin model rendering
	if( !re_bRenderingShadows) 
	{
		BeginModelRenderingView( *papr, re_pdpDrawPort);
		RM_BeginRenderingView(   *papr, re_pdpDrawPort);
	}
	else 
	{
		BeginModelRenderingMask(    *papr, re_pubShadow, re_slShadowWidth, re_slShadowHeight);
		RM_BeginModelRenderingMask( *papr, re_pubShadow, re_slShadowWidth, re_slShadowHeight);
	}

	re_admDelayedModels_AddWater.Clear();

	// for each of models that were kept for delayed rendering
	for( INDEX iModel=0; iModel<re_admDelayedModels.Count(); ++iModel) 
	{
		CDelayedModel &dm = re_admDelayedModels[iModel];
		CEntity &en = *dm.dm_penModel;
		BOOL bIsBackground = re_bBackgroundEnabled && (en.en_ulFlags&ENF_BACKGROUND);

		// skip if not rendered in this pass or not visible
		if(  (bBackground && !bIsBackground)
		 || (!bBackground &&  bIsBackground)
		 || !(dm.dm_ulFlags & DMF_VISIBLE)
		 || (en.en_ulFlags & ENF_INVISIBLE) ) continue; // eons "skip if Invisible flag on "

//강동민 수정 시작 테스트 클라이언트 작업	06.29
		extern INDEX gfx_bRenderReflection;
		if(gfx_bRenderReflection)
		{
			// FIXME : 임시적으로 Enemy와 Player등은 반사되지 않도록 처리함.
			// FIXME : Enemy가 물가까지 따라온다는거 자체가 문제 있음...ㅡ.ㅡ
			if(en.IsEnemy() || en.IsCharacter() /*|| en.IsPet()*/ )		continue;

			if(!bBackground)
			{
				if(!(en.en_ulFlags & ENF_RENDERREFLECTION))	continue;
			}
		}
//강동민 수정 끝 테스트 클라이언트 작업		06.29

		// NICEWATER에 해당하는 모델은 렌더링하지 않음.
		if(en.en_ulFlags & ENF_NICEWATER)
		{
			re_admDelayedModels_AddWater.Add(dm);
			continue;
		}
//안태훈 수정 시작	//(For Performance)(0.1)
		extern INDEX g_bRenderDecoration;
		if(!g_bRenderDecoration && en.IsFirstExtraFlagOn(ENF_EX1_DECORATION)) continue;
//안태훈 수정 끝	//(For Performance)(0.1)

		// SKA model?
		if( en.en_RenderType == CEntity::RT_SKAMODEL || en.en_RenderType == CEntity::RT_SKAEDITORMODEL)
		{
			//Editor 상에서 Active가 비활성화된 모델을 렌더링할 때 검은색 반투명으로 렌더링	:Su-won		|--->
			if( en.en_RenderType == CEntity::RT_SKAEDITORMODEL )
			{
				CModelInstance &mi = *en.GetModelInstance();
				if( mi.GetName() != CTString("Axis") )
				{
					UWORD wAlpha =mi.mi_colModelColor & 0xFF;
					mi.mi_colModelColor =mi.mi_colModelColor & 0x00000000;
					mi.mi_colModelColor =mi.mi_colModelColor | 0x40;
				}
			}
			//Editor 상에서 Active가 비활성화된 모델을 렌더링할 때 검은색 반투명으로 렌더링	:Su-won		<---|

			RenderOneSkaModel(en, en.GetLerpedPlacement(), TRUE, dm.dm_fMipFactor, dm.dm_ulFlags);

			// if selected entities should be drawn and this one is selected
			if( !re_bRenderingShadows && _wrpWorldRenderPrefs.wrp_stSelection==CWorldRenderPrefs::ST_ENTITIES
			 && _wrpWorldRenderPrefs.wrp_pmoSelectedEntity!=NULL && en.IsSelected(ENF_SELECTED))
			{ // get bounding box of current frame
				FLOATaabbox3D boxModel;
				en.GetModelInstance()->GetCurrentColisionBox(boxModel);
				// if model has collision
				if( en.en_pciCollisionInfo!=NULL ) 
				{
					// get its collision box
					INDEX iCollision = en.GetCollisionBoxIndex();
					FLOAT3D vMin = en.GetModelInstance()->GetCollisionBoxMin(iCollision);
					FLOAT3D vMax = en.GetModelInstance()->GetCollisionBoxMax(iCollision);
					// extend the box by the collision box
					boxModel|=FLOATaabbox3D(vMin, vMax);
				}
				// set position of marker at top of the model and it size to be proportional to the model
				boxModel.StretchByVector(en.GetModelInstance()->mi_vStretch);
				FLOAT fSize = boxModel.Size().Length()*0.3f;
				_wrpWorldRenderPrefs.wrp_pmoSelectedEntity->mo_Stretch = FLOAT3D( fSize, fSize, fSize);
				CPlacement3D plSelection = en.GetLerpedPlacement();
				plSelection.Translate_OwnSystem( FLOAT3D(0.0f, boxModel.Max()(2), 0.0f));

				// render the selection model without shadow
				RenderOneModel( en, *_wrpWorldRenderPrefs.wrp_pmoSelectedEntity, plSelection, dm.dm_fMipFactor, FALSE, DMF_VERYNEAR);
			}
		}
		// old model! 
		else
		{
			// render the model with its shadow
			CModelObject &moModelObject = *dm.dm_pmoModel;
			RenderOneModel( en, moModelObject, en.GetLerpedPlacement(), dm.dm_fMipFactor, TRUE, dm.dm_ulFlags);

			// if selected entities should be drawn and this one is selected
			if( !re_bRenderingShadows && _wrpWorldRenderPrefs.wrp_stSelection==CWorldRenderPrefs::ST_ENTITIES
			 && _wrpWorldRenderPrefs.wrp_pmoSelectedEntity!=NULL && en.IsSelected(ENF_SELECTED))
			{ // get bounding box of current frame
				FLOATaabbox3D boxModel;
				moModelObject.GetCurrentFrameBBox(boxModel);
				// if model has collision
				if( en.en_pciCollisionInfo!=NULL && 
					(en.GetRenderType()==CEntity::RT_MODEL || en.GetRenderType()==CEntity::RT_EDITORMODEL)) 
				{
					// get its collision box
					INDEX iCollision = en.GetCollisionBoxIndex();
					FLOAT3D vMin = moModelObject.GetCollisionBoxMin(iCollision);
					FLOAT3D vMax = moModelObject.GetCollisionBoxMax(iCollision);
					// extend the box by the collision box
					boxModel|=FLOATaabbox3D(vMin, vMax);
				}
				// set position of marker at top of the model and it size to be proportional to the model
				boxModel.StretchByVector(moModelObject.mo_Stretch);
				FLOAT fSize = boxModel.Size().Length()*0.3f;
				_wrpWorldRenderPrefs.wrp_pmoSelectedEntity->mo_Stretch = FLOAT3D( fSize, fSize, fSize);
				CPlacement3D plSelection = en.GetLerpedPlacement();
				plSelection.Translate_OwnSystem( FLOAT3D(0.0f, boxModel.Max()(2), 0.0f));
				// render the selection model without shadow
				RenderOneModel( en, *_wrpWorldRenderPrefs.wrp_pmoSelectedEntity, plSelection, dm.dm_fMipFactor, FALSE, DMF_VERYNEAR);
			}
		}
	}
	// end model rendering
	if( !re_bRenderingShadows) 
	{
		EndModelRenderingView(FALSE); // don't restore ortho projection for now
		RM_EndRenderingView(FALSE);
	}
	else 
	{
		EndModelRenderingMask();
		RM_EndModelRenderingMask();
	}

	if( wAlpha !=NULL)
	{
		for(INDEX iModel = 0; iModel < re_admDelayedModels.Count(); ++iModel)
		{
			CModelInstance &mi =*(re_admDelayedModels[iModel].dm_penModel->GetModelInstance());

			mi.mi_colModelColor =mi.mi_colModelColor & 0xFFFFFF00;
			mi.mi_colModelColor =mi.mi_colModelColor | wAlpha[iModel];
			//mi.mi_colModelColor =mi.mi_colModelColor | 0xff;
		}
		delete[] wAlpha;
	}
}

void GetWaterInformation(CEntity* pEntity, CNiceWater* pWaterInformation)
{
	ASSERT(pEntity != NULL && "Invalid Entity Pointer!!!");
	ASSERT(pWaterInformation != NULL && "Invalid WaterInformation Pointer!!!");
	// 프로퍼티 정보를 읽어들임.
	CDLLEntityClass *pdecDLLClass		=	pEntity->GetClass()->ec_pdecDLLClass;

	CEntityProperty &epPropertyWaterTex	= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_FILENAME, 2);	// Water Texture
	pWaterInformation->m_fnWaterName = ENTITYPROPERTY( &*pEntity, epPropertyWaterTex.ep_slOffset, CTFileName);

	CEntityProperty &epPropertyBumpTex	= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_FILENAME, 3);	// Bump Texture
	pWaterInformation->m_fnBumpName = ENTITYPROPERTY( &*pEntity, epPropertyBumpTex.ep_slOffset, CTFileName);

	CEntityProperty &epPropertyWireFrame= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_BOOL, 10);		// WireFrame
	pWaterInformation->m_bWireFrame = ENTITYPROPERTY( &*pEntity, epPropertyWireFrame.ep_slOffset, BOOL);

	CEntityProperty &epPropertyReflect	= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_BOOL, 12);		// Reflection
	pWaterInformation->m_bReflection = ENTITYPROPERTY( &*pEntity, epPropertyReflect.ep_slOffset, BOOL);

	CEntityProperty &epPropertyWave		= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_BOOL, 14);		// Wave
	pWaterInformation->m_bWave = ENTITYPROPERTY( &*pEntity, epPropertyWave.ep_slOffset, BOOL);

	CEntityProperty &epPropertyMat11	= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 35);		// Bump Matrix 11
	pWaterInformation->m_fBumpMat11 = ENTITYPROPERTY( &*pEntity, epPropertyMat11.ep_slOffset, FLOAT);

	CEntityProperty &epPropertyMat12	= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 36);		// Bump Matrix 12
	pWaterInformation->m_fBumpMat12 = ENTITYPROPERTY( &*pEntity, epPropertyMat12.ep_slOffset, FLOAT);

	CEntityProperty &epPropertyMat21	= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 37);		// Bump Matrix 21
	pWaterInformation->m_fBumpMat21 = ENTITYPROPERTY( &*pEntity, epPropertyMat21.ep_slOffset, FLOAT);

	CEntityProperty &epPropertyMat22	= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 38);		// Bump Matrix 22
	pWaterInformation->m_fBumpMat22 = ENTITYPROPERTY( &*pEntity, epPropertyMat22.ep_slOffset, FLOAT);

	CEntityProperty &epPropertyTexX		= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 39);		// Tex Flow X
	pWaterInformation->m_fTexFlowX = ENTITYPROPERTY( &*pEntity, epPropertyTexX.ep_slOffset, FLOAT);

	CEntityProperty &epPropertyTexY		= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 40);		// Tex Flow Y
	pWaterInformation->m_fTexFlowY = ENTITYPROPERTY( &*pEntity, epPropertyTexY.ep_slOffset, FLOAT);

	pWaterInformation->m_toBump.SetData_t(pWaterInformation->m_fnBumpName);
	pWaterInformation->m_toWater.SetData_t(pWaterInformation->m_fnWaterName);
}

// NICEWATER 플래그를 가진 모델들만 렌더링함.
void CRenderer::RenderNiceWaterModels()
{	
	extern INDEX gfx_bRenderReflection;	
	gfxDisableClipPlane();
	if(re_iIndex == IDX_MAIN_RENDERER)
	{
		gfxEnableDepthBias();
		if( _bMultiPlayer) gfx_bRenderModels = 1; // must render in multiplayer mode!
		if( !gfx_bRenderModels && !re_bRenderingShadows) return;
		
		// sort all the delayed models by distance
		// NOTE : 이전에 RenderModels()에서 정렬되었는데 머하러 한번 더 정렬하지?ㅡ.ㅡ
		//qsort( re_admDelayedModels.GetArrayOfPointers(), re_admDelayedModels.Count(),
		//	sizeof(CDelayedModel*), qsort_CompareDelayedModels);
		
		CAnyProjection3D *papr = NULL;	
		papr = &re_prProjection;
		
		// begin model rendering
		if( !re_bRenderingShadows) 
		{
			// NOTE : 아래 두 함수는 내부적으로 비슷한 부분이 많다.
			// NOTE : 왜 저렇게 했을까???
			BeginModelRenderingView( *papr, re_pdpDrawPort);
			RM_BeginRenderingView(   *papr, re_pdpDrawPort);		
		}	
		BOOL bBackground = FALSE;
		
		// for each of models that were kept for delayed rendering
		for( INDEX iModel=0; iModel<re_admDelayedModels_AddWater.Count(); ++iModel) 
		{
			CDelayedModel &dm = re_admDelayedModels_AddWater[iModel];
			CEntity &en = *dm.dm_penModel;
			//if(!(en.en_ulFlags & ENF_NICEWATER)) continue;
			
			BOOL bIsBackground = re_bBackgroundEnabled && (en.en_ulFlags&ENF_BACKGROUND);
			
			// skip if not rendered in this pass or not visible
			if(  (bBackground && !bIsBackground)
				|| (!bBackground &&  bIsBackground)
				|| !(dm.dm_ulFlags&DMF_VISIBLE)) continue;
			
			// SKA model?
			if( en.en_RenderType == CEntity::RT_SKAMODEL || en.en_RenderType == CEntity::RT_SKAEDITORMODEL)
			{
				gfxCullFace(GFX_NONE);
				GetWaterInformation(&en, &_WaterInformation);				
				
				if(_WaterInformation.m_bWireFrame)
					gfxPolygonMode(GFX_LINE);				
				
				gfxSetTextureMatrix(NULL);
				RM_AddFlag(RMF_SHOWNICEWATER);
				RenderOneSkaModel(en, en.GetLerpedPlacement(), TRUE, dm.dm_fMipFactor, dm.dm_ulFlags);
				RM_RemoveFlag(RMF_SHOWNICEWATER);
				
				gfxCullFace(GFX_FRONT);
				if(_WaterInformation.m_bWireFrame)
					gfxPolygonMode(GFX_FILL);
			}
		}
		// end model rendering
		if( !re_bRenderingShadows) 
		{
			EndModelRenderingView(FALSE); // don't restore ortho projection for now
			RM_EndRenderingView(FALSE);
		}
		gfxDisableDepthBias();
	}
	else if(gfx_bRenderReflection)
	{		
		if( _bMultiPlayer) gfx_bRenderModels = 1; // must render in multiplayer mode!
		if( !gfx_bRenderModels && !re_bRenderingShadows) return;
		
		//qsort( re_admDelayedModels.GetArrayOfPointers(), re_admDelayedModels.Count(),
		//	sizeof(CDelayedModel*), qsort_CompareDelayedModels);
		
		CAnyProjection3D *papr = NULL;	
		papr = &re_prProjection;
		
		// begin model rendering
		if( !re_bRenderingShadows) 
		{
			BeginModelRenderingView( *papr, re_pdpDrawPort);
			RM_BeginRenderingView(   *papr, re_pdpDrawPort);		
		}	
		BOOL bBackground = FALSE;
		
		// for each of models that were kept for delayed rendering
		for( INDEX iModel=0; iModel<re_admDelayedModels_AddWater.Count(); iModel++) 
		{
			CDelayedModel &dm = re_admDelayedModels_AddWater[iModel];
			CEntity &en = *dm.dm_penModel;		
			//if(!(en.en_ulFlags & ENF_NICEWATER)) continue;
			
			BOOL bIsBackground = re_bBackgroundEnabled && (en.en_ulFlags&ENF_BACKGROUND);
			
			// skip if not rendered in this pass or not visible
			if(  (bBackground && !bIsBackground)
				|| (!bBackground &&  bIsBackground)
				|| !(dm.dm_ulFlags&DMF_VISIBLE)) continue;
			
			// SKA model?
			if( en.en_RenderType == CEntity::RT_SKAMODEL || en.en_RenderType == CEntity::RT_SKAEDITORMODEL)
			{
				gfxCullFace(GFX_NONE);
				RenderOneSkaModel(en, en.GetLerpedPlacement(), TRUE, dm.dm_fMipFactor, dm.dm_ulFlags);
				gfxCullFace(GFX_FRONT);
			}
		}
		// end model rendering
		if( !re_bRenderingShadows) 
		{
			EndModelRenderingView(FALSE); // don't restore ortho projection for now
			RM_EndRenderingView(FALSE);
		}	
	}
}

extern CEntity *_Particle_penCurrentViewer;
extern FLOAT _Particle_fCurrentMip;
extern BOOL _Particle_bHasFog;
extern BOOL _Particle_bHasHaze;
extern ENGINE_API BOOL _Particle_bSkipClipping;

void Particle_PrepareEntity( const FLOAT fMipFactor, const BOOL bHasFog, const BOOL bHasHaze,
														 const BOOL bSkipClipping, CEntity *penViewer)
{
	_Particle_fCurrentMip = fMipFactor;
	_Particle_bHasFog     = bHasFog;
	_Particle_bHasHaze    = bHasHaze;
	_Particle_bSkipClipping = bSkipClipping;
	_Particle_penCurrentViewer = penViewer;
}


/* Render particles for models that were kept for delayed rendering. */
void CRenderer::RenderParticles(BOOL bBackground)
{
	if( _bMultiPlayer) gfx_bRenderParticles = 1; // must render in multiplayer mode!
	if( re_bRenderingShadows || !gfx_bRenderParticles) return;

	// prepare gfx library for particles
	if( bBackground) 
	{
		Particle_PrepareSystem(re_pdpDrawPort, re_prBackgroundProjection);
	}
	else 
	{
		Particle_PrepareSystem(re_pdpDrawPort, re_prProjection);
	}

	// for each of models that were kept for delayed rendering
	for( INDEX iModel=0; iModel<re_admDelayedModels.Count(); iModel++)
	{
		// fetch
		CDelayedModel &dm = re_admDelayedModels[iModel];
		CEntity &en = *dm.dm_penModel;
		// if this model doesn't have particles atteched
		if( en.IsFlagOn(ENF_NOPARTICLES)) 
		{
			// skip it
			continue; 
		}
		// if not rendered in this pass
		const BOOL bIsBackground = re_bBackgroundEnabled && (en.en_ulFlags&ENF_BACKGROUND);
		if( !bBackground != !bIsBackground) 
		{
			// skip it
			continue;
		}
		// render particles made by this entity
		Particle_PrepareEntity( dm.dm_fMipFactor, dm.dm_ulFlags&DMF_FOG, dm.dm_ulFlags&DMF_HAZE,
														dm.dm_ulFlags&DMF_INSIDE, re_penViewer);
		en.RenderParticles();
		_Particle_penCurrentViewer = NULL;
	}

	// end gfx library for particles
	Particle_EndSystem(FALSE);
}


void DeleteLensFlare(CLightSource *pls)
{
	// for each renderer
	for (INDEX ire = 0; ire<MAX_RENDERERS; ire++) 
	{
		CRenderer &re = _areRenderers[ire];

		// for each lens flare of this light source
		for(INDEX iFlare=0; iFlare<re.re_alfiLensFlares.Count(); iFlare++) 
		{
			CLensFlareInfo &lfi = re.re_alfiLensFlares[iFlare];
			if (lfi.lfi_plsLightSource == pls) 
			{
				// invalidate lens flare info to be deleted in next rendering
				lfi.lfi_plsLightSource = NULL;
			}
		}
	}
}


/* Render lens flares. */
void CRenderer::RenderLensFlares(void)
{
	// make sure we're have orthographic projection 
	re_pdpDrawPort->SetOrtho(); 

	// if there are no flares of flares are off, do nothing
	gfx_iLensFlareQuality = Clamp( gfx_iLensFlareQuality, 0L, 3L);
	if( gfx_iLensFlareQuality==0 || re_alfiLensFlares.Count()==0) return;

	// get drawport ID
	ASSERT( re_pdpDrawPort!=NULL);
	const ULONG ulDrawPortID = re_pdpDrawPort->GetID();
	INDEX iFlare;
	
	// for each lens flare of this drawport
	for( iFlare=0; iFlare<re_alfiLensFlares.Count(); iFlare++) 
	{
		CLensFlareInfo &lfi = re_alfiLensFlares[iFlare];
		// skip if not in this drawport
		if( lfi.lfi_ulDrawPortID!=ulDrawPortID && lfi.lfi_iMirrorLevel==0) continue;
		// test if it is still visible
//강동민 수정 시작 Water 구현		04.22
		//if( re_pdpDrawPort->IsPointVisible(lfi.lfi_fI, lfi.lfi_fJ, lfi.lfi_fOoK, lfi.lfi_iID, lfi.lfi_iMirrorLevel))	// 원본.
		if( re_pdpDrawPort->IsPointVisible(re_prProjection, lfi.lfi_fI, lfi.lfi_fJ, lfi.lfi_fOoK, lfi.lfi_iID, lfi.lfi_iMirrorLevel))
//강동민 수정 끝 Water 구현			04.22
		{
			lfi.lfi_ulFlags |= LFF_VISIBLE;
		}
		else 
		{
			lfi.lfi_ulFlags &= ~LFF_VISIBLE;
		}
	}

	// get count of currently existing flares and time
	INDEX ctFlares   = re_alfiLensFlares.Count();
	const TIME tmNow = _pTimer->GetRealTimeTick();

	// for each lens flare
	iFlare = 0;
	while( iFlare<ctFlares) 
	{
		CLensFlareInfo &lfi = re_alfiLensFlares[iFlare];
		// if the flare is not active any more, or its drawport was not refreshed long
		if( lfi.lfi_plsLightSource==NULL || // marked when entity is deleted
			 (lfi.lfi_ulDrawPortID==ulDrawPortID && 
		 (!(lfi.lfi_ulFlags&LFF_ACTIVE) || lfi.lfi_plsLightSource->ls_plftLensFlare==NULL)) ||
			 (lfi.lfi_ulDrawPortID!=ulDrawPortID && lfi.lfi_tmLastFrame<tmNow-5.0f)) 
		{
			// delete it by moving the last one on its place
			lfi = re_alfiLensFlares[ctFlares-1];
			re_alfiLensFlares[ctFlares-1].Clear();
			ctFlares--;
		// just proceed to next flare, if this flare is still active
		} else iFlare++;
	}

	// remove unused flares at the end
	if( ctFlares==0) re_alfiLensFlares.PopAll();
	else re_alfiLensFlares.PopUntil(ctFlares-1);

	// calculate general screen factors and lens flare type
	const FLOAT fScreenSizeI   = re_pdpDrawPort->GetWidth();
	const FLOAT f1oScreenSizeI = 1.0f / fScreenSizeI;
	const FLOAT fScreenCenterI = fScreenSizeI * 0.5f;
	const FLOAT fScreenCenterJ = re_pdpDrawPort->GetHeight() * 0.5f;
	const BOOL bReflections = gfx_iLensFlareQuality>=2 && _wrpWorldRenderPrefs.wrp_lftLensFlares>=CWorldRenderPrefs::LFT_REFLECTIONS;
	const BOOL bGlare       = gfx_iLensFlareQuality==3 && _wrpWorldRenderPrefs.wrp_lftLensFlares>=CWorldRenderPrefs::LFT_REFLECTIONS_AND_GLARE;

	// for each lens flare of this drawport
	for( iFlare=0; iFlare<re_alfiLensFlares.Count(); iFlare++)
	{
		// skip if not in this drawport or has been "switched off"
		CLensFlareInfo &lfi = re_alfiLensFlares[iFlare];
		if( lfi.lfi_ulDrawPortID != ulDrawPortID
		 || lfi.lfi_plsLightSource->ls_plftLensFlare==NULL) continue;

		// clear active flag for next frame
		lfi.lfi_ulFlags &= ~LFF_ACTIVE;

		// fade the flare in/out
		#define FLAREINSPEED  (+1/0.2f)
		#define FLAREOUTSPEED (-1/0.1f)
		const FLOAT fFlareFadeSpeed = (lfi.lfi_ulFlags&LFF_VISIBLE) ? FLAREINSPEED : FLAREOUTSPEED;
		lfi.lfi_fFadeFactor += (tmNow-lfi.lfi_tmLastFrame) * fFlareFadeSpeed;
		lfi.lfi_fFadeFactor  = Clamp( lfi.lfi_fFadeFactor, 0.0f, 1.0f);

		// reset timer and skip if invisible
		lfi.lfi_tmLastFrame = tmNow;
		if( lfi.lfi_fFadeFactor<0.01f) continue;

		// calculate general flare factors
		const FLOAT fLightI = lfi.lfi_fI;
		const FLOAT fLightJ = lfi.lfi_fJ;
		const FLOAT fPositionFactorI = (fLightI-fScreenCenterI) * f1oScreenSizeI;
		const FLOAT fReflectionDirI = fScreenCenterI-fLightI;
		const FLOAT fReflectionDirJ = fScreenCenterJ-fLightJ;
		const COLOR colLight = lfi.lfi_plsLightSource->GetLightColor();
		const FLOAT fR = (colLight&CT_RMASK)>>CT_RSHIFT;
		const FLOAT fG = (colLight&CT_GMASK)>>CT_GSHIFT;
		const FLOAT fB = (colLight&CT_BMASK)>>CT_BSHIFT;
		const FLOAT fI = fR*0.3f + fG*0.5f + fB*0.2f;
		const FLOAT fReflectionDistance = sqrt(fReflectionDirI*fReflectionDirI + fReflectionDirJ*fReflectionDirJ);
		const FLOAT fOfCenterFadeFactor = Max( 1.0f - 2.0f*fReflectionDistance*f1oScreenSizeI, 0.0f);

		FLOAT fFogHazeFade = 1.0f;
		// if in haze
		if( lfi.lfi_ulFlags&LFF_HAZE) 
		{
			// get haze strength at light position and fade flare with haze
			const FLOAT fS = (-lfi.lfi_vProjected(3)+_haze_fAdd)*_haze_fMul;
			const FLOAT fHazeStrength = NormByteToFloat(GetHazeAlpha(fS));
			fFogHazeFade *= 1-fHazeStrength;
		}
		// if in fog
		if( lfi.lfi_ulFlags&LFF_FOG) 
		{
			// get fog strength at light position and fade flare with fog
			GFXTexCoord tex;
			tex.s = -lfi.lfi_vProjected(3)*_fog_fMulZ;
			tex.t = (lfi.lfi_vProjected%_fog_vHDirView+_fog_fAddH)*_fog_fMulH;
			const FLOAT fFogStrength = NormByteToFloat(GetFogAlpha(tex));
			fFogHazeFade *= 1-fFogStrength;
		}

		// get number of flares to render for this light
		CStaticArray<COneLensFlare> &aolf = lfi.lfi_plsLightSource->ls_plftLensFlare->lft_aolfFlares;
		const INDEX ctReflections = bReflections ? aolf.Count() : 1;  // clamp number reflections if required

		// for each reflection in the lens flare effect
		for( INDEX iReflection=0; iReflection<ctReflections; iReflection++) 
		{
			COneLensFlare &olf = aolf[iReflection];
			// calculate its fading factors
			FLOAT fFadeFactor = fFogHazeFade * lfi.lfi_fFadeFactor * IntensityAtDistance(
				lfi.lfi_plsLightSource->ls_rFallOff*olf.oft_fFallOffFactor,
				lfi.lfi_plsLightSource->ls_rHotSpot*olf.oft_fFallOffFactor,
				lfi.lfi_fDistance);
			if( olf.olf_ulFlags&OLF_FADEOFCENTER) fFadeFactor *= fOfCenterFadeFactor;

			// calculate flare intensity
			FLOAT fIntensityFactor = olf.olf_fLightAmplification;
			if( olf.olf_ulFlags&OLF_FADEINTENSITY) fIntensityFactor *= fFadeFactor; 
			if( fIntensityFactor<0.01f) continue;  // skip if this flare is invisible

			// calculate flare size
			FLOAT fSizeIFactor = fScreenSizeI;
			if( olf.olf_ulFlags&OLF_FADESIZE) fSizeIFactor *= fFadeFactor; 
			const FLOAT fSizeI = olf.olf_fSizeIOverScreenSizeI * fSizeIFactor;
			const FLOAT fSizeJ = olf.olf_fSizeJOverScreenSizeI * fSizeIFactor;
			if( fSizeI<2.0f || fSizeJ<2.0f) continue;  // skip if this flare is invisible
			
			// determine color
			const FLOAT fThisR  = (fR + (fI-fR)*olf.olf_fLightDesaturation) * fIntensityFactor;
			const FLOAT fThisG  = (fG + (fI-fG)*olf.olf_fLightDesaturation) * fIntensityFactor;
			const FLOAT fThisB  = (fB + (fI-fB)*olf.olf_fLightDesaturation) * fIntensityFactor;
			const SLONG slThisR = Min( FloatToInt(fThisR), 255L);
			const SLONG slThisG = Min( FloatToInt(fThisG), 255L);
			const SLONG slThisB = Min( FloatToInt(fThisB), 255L);
			const COLOR colBlending = RGBToColor( slThisR,slThisG,slThisB);

			// render the flare
			re_pdpDrawPort->RenderLensFlare(
				&olf.olf_toTexture, 
				fLightI+olf.olf_fReflectionPosition*fReflectionDirI,
				fLightJ+olf.olf_fReflectionPosition*fReflectionDirJ,
				fSizeI, fSizeJ,
				olf.olf_aRotationFactor*fPositionFactorI,
				colBlending);
		} // (for each flare in the lens flare effect)

		// skip to next if screen glare is off
		CLensFlareType &lft = *lfi.lfi_plsLightSource->ls_plftLensFlare;
		if( !bGlare || lft.lft_fGlareIntensity<0.01f) continue;

		// calculate glare factor for current position
		const FLOAT fIntensity = IntensityAtDistance( lfi.lfi_plsLightSource->ls_rFallOff*lft.lft_fGlareFallOffFactor,
																									lfi.lfi_plsLightSource->ls_rHotSpot*lft.lft_fGlareFallOffFactor,
																									lfi.lfi_fDistance);
		const FLOAT fCenterFactor = 1.0f - fOfCenterFadeFactor;
		const FLOAT fGlare = lft.lft_fGlareIntensity * fIntensity
							 * (exp(1/(1+lft.lft_fGlareCompression*fCenterFactor*fCenterFactor)) -1) * 0.7677f; // /(exp(1)-1)
		const ULONG ulGlareA = ClampUp( NormFloatToByte(fGlare), 255UL);
		// if there is any relevant glare
		if( ulGlareA>1 && fI>0.01f) 
		{
			// calculate glare color
			FLOAT fGlareR = fR + (fI-fR)*lft.lft_fGlareDesaturation;
			FLOAT fGlareG = fG + (fI-fG)*lft.lft_fGlareDesaturation;
			FLOAT fGlareB = fB + (fI-fB)*lft.lft_fGlareDesaturation;
			const FLOAT fMax = Max( fGlareR,Max(fGlareG,fGlareB));
			const FLOAT fBrightFactor = 255.0f/fMax;
			fGlareR *= fBrightFactor;
			fGlareG *= fBrightFactor;
			fGlareB *= fBrightFactor;
			const ULONG ulGlareR = ClampUp( FloatToInt(fGlareR), 255L);
			const ULONG ulGlareG = ClampUp( FloatToInt(fGlareG), 255L);
			const ULONG ulGlareB = ClampUp( FloatToInt(fGlareB), 255L);
			// add the glare to screen blending
			re_pdpDrawPort->dp_ulBlendingRA += ulGlareR*ulGlareA;
			re_pdpDrawPort->dp_ulBlendingGA += ulGlareG*ulGlareA;
			re_pdpDrawPort->dp_ulBlendingBA += ulGlareB*ulGlareA;
			re_pdpDrawPort->dp_ulBlendingA  += ulGlareA;
		}
	}
}
