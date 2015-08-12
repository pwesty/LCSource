/*
 *  TerrainEditing.cpp - Terrain editing implementation
 *  World Editor
 *  Copyright (c) 1997, CroTeam. All rights reserved.
 */
#include "StdAfx.h"
#include <Engine/Templates/Stock_CTextureData.h>
 
// global variables used in terrain editing functions
UWORD*	_puwBuffer = NULL;
ULONG*	_pulBuffer = NULL;		// yjpark
UBYTE*	_pubBuffer = NULL;		// yjpark
CTRect _rect;
PIX _srcExtraW=0;
PIX _srcExtraH=0;
CTextureData *_ptdBrush=NULL;
CTextureData *_ptdDistributionRandomNoise=NULL;
CTextureData *_ptdContinousRandomNoise=NULL;
UWORD *_puwNoiseTarget=NULL;
PIX _pixNoiseTargetW=0;
PIX _pixNoiseTargetH=0;
FLOAT _fStrength=0.0f;

// fbm noise buffer
FLOAT *_pafWhiteNoise=NULL;
#define WNOISE 64

// undo variables
UWORD *_puwUndoTerrain=NULL;
ULONG *_pulUndoTerrain=NULL;

CTRect _rectUndo;
BOOL _bUndoStart=FALSE;
BufferType _btUndoBufferType=BT_INVALID;
INDEX _iUndoBufferData=-1;
INDEX _iTerrainEntityID=-1;

// filter matrices
FLOAT _afFilterFineBlur[5][5]=
{
	{0.0f,       0.0f,       0.0f,       0.0f,    0.0f},
	{0.0f,    0.0625f,    0.0625f,    0.0625f,    0.0f},
	{0.0f,    0.0625f,       0.5f,    0.0625f,    0.0f},
	{0.0f,    0.0625f,    0.0625f,    0.0625f,    0.0f},
	{0.0f,       0.0f,       0.0f,       0.0f,    0.0f},
};

FLOAT _afFilterBlurMore[5][5]=
{
	{0.0277777f,    0.0277777f,    0.0277777f,    0.0277777f,    0.0277777f},
	{0.0277777f,    0.0555555f,    0.0555555f,    0.0555555f,    0.0277777f},
	{0.0277777f,    0.0555555f,    0.0111111f,    0.0555555f,    0.0277777f},
	{0.0277777f,    0.0555555f,    0.0555555f,    0.0555555f,    0.0277777f},
	{0.0277777f,    0.0277777f,    0.0277777f,    0.0277777f,    0.0277777f},
};

FLOAT _afFilterEdgeDetect[5][5]=
{
	{0.0f,    0.0f,    0.0f,    0.0f,    0.0f},
	{0.0f,    1.0f,    1.0f,    1.0f,    0.0f},
	{0.0f,    1.0f,   -7.0f,    1.0f,    0.0f},
	{0.0f,    1.0f,    1.0f,    1.0f,    0.0f},
	{0.0f,    0.0f,    0.0f,    0.0f,    0.0f},
};

FLOAT _afFilterEmboss[5][5]=
{
	{0.0f,    0.0f,    0.0f,    0.0f,    0.0f},
	{0.0f,    1.0f,    1.0f,   -1.0f,    0.0f},
	{0.0f,    1.0f,    1.0f,   -1.0f,    0.0f},
	{0.0f,    1.0f,   -1.0f,   -1.0f,    0.0f},
	{0.0f,    0.0f,    0.0f,    0.0f,    0.0f},
};

FLOAT _afFilterSharpen[5][5]=
{
	{0.0f,    0.0f,    0.0f,    0.0f,    0.0f},
	{0.0f,   -1.0f,   -1.0f,   -1.0f,    0.0f},
	{0.0f,   -1.0f,   16.0f,   -1.0f,    0.0f},
	{0.0f,   -1.0f,   -1.0f,   -1.0f,    0.0f},
	{0.0f,    0.0f,    0.0f,    0.0f,    0.0f},
};

FLOAT GetBrushMultiplier(INDEX x, INDEX y)
{
	if(_ptdBrush==NULL) return 1.0f;
	{
		COLOR col=_ptdBrush->GetTexel(x,y);
		FLOAT fResult=FLOAT(col>>24)/255.0f;
		return fResult;
	}
}

void ApplyAddPaint(UWORD uwMin, UWORD uwMax)
{
	for(INDEX y=0; y<_rect.GetHeight(); y++)
	{
		for(INDEX x=0; x<_rect.GetWidth(); x++)
		{
			FLOAT fBrushMultiplier=GetBrushMultiplier(x,y);
			if( fBrushMultiplier==0.0f) continue;
			INDEX iOffset=y*_rect.GetWidth()+x;
			FLOAT fValue=_puwBuffer[iOffset];
			FLOAT temp =fValue;
			fValue+=fBrushMultiplier*_fStrength*32.0f*theApp.m_fPaintPower;
			fValue=Clamp(fValue,FLOAT(uwMin),FLOAT(uwMax));
			_puwBuffer[iOffset]=fValue;
		}
	}
}

// yjpark |<--
void ApplyShadowMapAddPaint( GFXColor colShadow, INDEX iBufferData, BOOL bAdd=TRUE )
{
	for( INDEX y = 0; y < _rect.GetHeight(); y++ )
	{
		for( INDEX x = 0; x < _rect.GetWidth(); x++ )
		{
			FLOAT	fBrushMultiplier = GetBrushMultiplier( x, y ) * 0.3f;
			if( fBrushMultiplier == 0.0f )
				continue ;

			INDEX	iOffset = y * _rect.GetWidth() + x;
			
			SINT	siR = colShadow.r;
			SINT	siG = colShadow.g;
			SINT	siB = colShadow.b;
			
			ULONG	ulSrc = _pulBuffer[iOffset];
			SINT	siSrcR = ulSrc & 0x000000FF;
			SINT	siSrcG = ( ulSrc & 0x0000FF00 ) >> 8;
			SINT	siSrcB = ( ulSrc & 0x00FF0000 ) >> 16;

			SINT	siDstR, siDstG, siDstB;
			FLOAT	fStrength = fBrushMultiplier * _fStrength;

			if( fStrength >= 0.0f )
			{
				siDstR = siSrcR - ( ( siSrcR - siR ) * fStrength );
				siDstG = siSrcG - ( ( siSrcG - siG ) * fStrength );
				siDstB = siSrcB - ( ( siSrcB - siB ) * fStrength );
			}
			else
			{
				siDstR = siSrcR + ( ( siSrcR - 255 ) * fStrength );
				siDstG = siSrcG + ( ( siSrcG - 255 ) * fStrength );
				siDstB = siSrcB + ( ( siSrcB - 255 ) * fStrength );
			}

			//siDstR = Clamp( siDstR, 0, 255 );
			//siDstG = Clamp( siDstG, 0, 255 );
			//siDstB = Clamp( siDstB, 0, 255 );
			if( bAdd )
			{
				siDstR = Clamp( siDstR, siR, siSrcR );
				siDstG = Clamp( siDstG, siG, siSrcG );
				siDstB = Clamp( siDstB, siB, siSrcB );
			}
			else
			{
				siDstR = Clamp( siDstR, siSrcR, 255);
				siDstG = Clamp( siDstG, siSrcG, 255);
				siDstB = Clamp( siDstB, siSrcB, 255);
			}

			//g_vShadowBuffer[]
			_pulBuffer[iOffset] = ( siDstB << 16 ) | ( siDstG << 8 ) | siDstR | 0xFF000000;
		}
	}
}

void ApplyAttributeMapAddPaint()
{
	for( INDEX y = 0; y < _rect.GetHeight(); y++ )
	{
		for( INDEX x = 0; x < _rect.GetWidth(); x++ )
		{
			FLOAT	fBrushMultiplier = GetBrushMultiplier( x, y );
			if( fBrushMultiplier == 0.0f )
				continue ;

			INDEX	iOffset = y * _rect.GetWidth() + x;
			if( _fStrength > 0.0f )
			{
				_pubBuffer[iOffset] = 255;
			}
			else
			{
				_pubBuffer[iOffset] = 0;
			}
		}
	}
}
// yjpark     -->|

void ApplyRNDNoise(void)
{
	CTerrain *ptrTerrain=GetTerrain();
	if( ptrTerrain==NULL) return;
	FLOAT fMaxNoise=theApp.m_fNoiseAltitude/TR_GetTerrainSize(ptrTerrain)(2)*65535.0f;
	for(INDEX y=0; y<_rect.GetHeight(); y++)
	{
		for(INDEX x=0; x<_rect.GetWidth(); x++)
		{
			FLOAT fBrushMultiplier=GetBrushMultiplier(x,y);
			INDEX iPixSrc=y*_rect.GetWidth()+x;
			FLOAT fInfluence=_puwBuffer[iPixSrc];
			FLOAT fRnd=FLOAT(rand())/RAND_MAX-0.5f;
			FLOAT fValue=_puwBuffer[iPixSrc];
			FLOAT fMaxRandomized=fValue+fRnd*fMaxNoise;
			FLOAT fFilterPower=Clamp(fBrushMultiplier*_fStrength,0.0f,1.0f);
			FLOAT fResult=Lerp( fValue, fMaxRandomized, fFilterPower);
			UWORD uwResult=(UWORD)Clamp(fResult,float(MIN_UWORD),float(MAX_UWORD));
			_puwBuffer[iPixSrc]=uwResult;
		}
	}
}

FLOAT GetDistributionNoise( INDEX x, INDEX y, FLOAT fRandom)
{
	INDEX iw=_ptdDistributionRandomNoise->GetPixWidth();
	INDEX ih=_ptdDistributionRandomNoise->GetPixHeight();
	INDEX ctSize=iw*ih;
	INDEX iOffset=abs(INDEX(iw*y+x+fRandom*ctSize)%ctSize);
	COLOR col=_ptdDistributionRandomNoise->td_pulFrames[iOffset];
	FLOAT fResult=FLOAT(col&0xFF)/255.0f;
	return fResult;
}

FLOAT GetContinousNoise( INDEX x, INDEX y, FLOAT fRandom)
{
	INDEX iw=_ptdContinousRandomNoise->GetPixWidth();
	INDEX ih=_ptdContinousRandomNoise->GetPixHeight();
	INDEX ctSize=iw*ih;
	INDEX iOffset=abs(INDEX(iw*y+x+fRandom*ctSize)%ctSize);
	COLOR col=_ptdContinousRandomNoise->td_pulFrames[iOffset];
	FLOAT fResult=FLOAT(col&0xFF)/255.0f;
	return fResult;
}

void ApplyContinousNoise(void)
{
	CTerrain *ptrTerrain=GetTerrain();
	if( ptrTerrain==NULL) return;
	FLOAT fMaxNoise=theApp.m_fNoiseAltitude/TR_GetTerrainSize(ptrTerrain)(2)*65535.0f;
	for(INDEX y=0; y<_rect.GetHeight(); y++)
	{
		INDEX oy=y+_rect.rc_slTop;
		for(INDEX x=0; x<_rect.GetWidth(); x++)
		{
			INDEX ox=x+_rect.rc_slLeft;
			FLOAT fBrushMultiplier=GetBrushMultiplier(x,y);
			INDEX iPixSrc=y*_rect.GetWidth()+x;
			FLOAT fInfluence=_puwBuffer[iPixSrc];
			FLOAT fRnd=GetContinousNoise( ox, oy, 0.0f)-0.5f;
			FLOAT fValue=_puwBuffer[iPixSrc];
			FLOAT fMaxRandomized=fValue+fRnd*fMaxNoise;
			fMaxRandomized=Clamp(fMaxRandomized,(FLOAT)MIN_UWORD,(FLOAT)MAX_UWORD);
			FLOAT fFilterPower=Clamp(fBrushMultiplier*_fStrength,0.0f,1.0f);
			FLOAT fResult=Lerp( fValue, fMaxRandomized, fFilterPower);
			UWORD uwResult=Clamp((UWORD)fResult,MIN_UWORD,MAX_UWORD);
			_puwBuffer[iPixSrc]=uwResult;
		}
	}
}

void ApplyPosterize(void)
{
	CTerrain *ptrTerrain=GetTerrain();
	if( ptrTerrain==NULL) return;
	FLOAT fStepUW=theApp.m_fPosterizeStep/TR_GetTerrainSize(ptrTerrain)(2)*65535.0f;
	for(INDEX y=0; y<_rect.GetHeight(); y++)
	{
		for(INDEX x=0; x<_rect.GetWidth(); x++)
		{
			FLOAT fBrushMultiplier=GetBrushMultiplier(x,y);
			if(fBrushMultiplier==0.0f) continue;
			INDEX iPixSrc=y*_rect.GetWidth()+x;
			FLOAT fValue=_puwBuffer[iPixSrc];
			FLOAT fPosterized=(INDEX(fValue/fStepUW))*fStepUW+1.0f;
			UWORD uwResult=Clamp(UWORD(fPosterized),MIN_UWORD,MAX_UWORD);
			_puwBuffer[iPixSrc]=uwResult;
		}
	}
}

void ApplyFilterMatrix(FLOAT afFilterMatrix[5][5])
{
	INDEX ctBuffBytes=_rect.GetWidth()*_rect.GetHeight()*sizeof(UWORD);
	UWORD *puwDst=(UWORD*)AllocMemory(ctBuffBytes);
	memcpy(puwDst,_puwBuffer,ctBuffBytes);
	for(INDEX y=0; y<_rect.GetHeight()-_srcExtraH*2; y++)
	{
		for(INDEX x=0; x<_rect.GetWidth()-_srcExtraW*2; x++)
		{
			INDEX iPixDst=(y+_srcExtraH)*_rect.GetWidth()+x+_srcExtraW;
			FLOAT fBrushMultiplier=GetBrushMultiplier(x,y);

			FLOAT fDivSum=0.0f;
			FLOAT fSum=0.0f;
			for(INDEX j=0; j<5; j++)
			{
				for(INDEX i=0; i<5; i++)
				{
					FLOAT fWeight=(afFilterMatrix)[i][j];
					fDivSum+=fWeight;
					INDEX iPixSrc=(y+j)*_rect.GetWidth()+(x+i);
					FLOAT fInfluence=_puwBuffer[iPixSrc];
					fSum+=fInfluence*fWeight;
				}
			}
			UWORD uwMax=Clamp(UWORD(fSum/fDivSum),MIN_UWORD,MAX_UWORD);
			FLOAT fFilterPower=Clamp(fBrushMultiplier*_fStrength/64.0f,0.0f,1.0f);
			UWORD uwResult=Lerp( puwDst[iPixDst], uwMax, fFilterPower);
			puwDst[iPixDst]=uwResult;
		}
	}
	
	memcpy(_puwBuffer,puwDst,ctBuffBytes);
	TRE_FreeBufferForEditing( puwDst);
}

static INDEX _iTerrainWidth=0;
static INDEX _iTerrainHeight=0;
static UWORD *_puwHeightMap=NULL;
static INDEX _iRandomDX=0;

void SetHMPixel( UWORD pix, INDEX x, INDEX y)
{
	UWORD *pdest=_puwHeightMap+y*_iTerrainWidth+x;
	if (*pdest==65535) {
		*pdest=pix;
	}
}

UWORD GetHMPixel(INDEX x, INDEX y)
{
	UWORD *pdest=_puwHeightMap+y*_iTerrainWidth+x;
	return *pdest;
}

UWORD RandomizePixel(FLOAT fmid, FLOAT fdmax)
{
	FLOAT fRand=((FLOAT)rand())/RAND_MAX-0.5f;
	FLOAT fd=fdmax*fRand;
	FLOAT fres=Clamp(fmid+fd,0.0f,65536.0f);
	return fres;
}

void SubdivideAndDisplace(INDEX x, INDEX y, INDEX idx, FLOAT fdMax)
{
	FLOAT flu=GetHMPixel(x,y);
	FLOAT fru=GetHMPixel(x+idx,y);
	FLOAT frd=GetHMPixel(x+idx,y+idx);
	FLOAT fld=GetHMPixel(x,y+idx);

	if( fdMax<_iRandomDX)
	{
		SetHMPixel(RandomizePixel((flu+fru)/2.0f,fdMax),  x+idx/2, y      );  // middle top
		SetHMPixel(RandomizePixel((fld+frd)/2.0f,fdMax),  x+idx/2, y+idx  );  // middle bottom
		SetHMPixel(RandomizePixel((fru+frd)/2.0f,fdMax),  x+idx,   y+idx/2);  // right  middle 
		SetHMPixel(RandomizePixel((flu+fld)/2.0f,fdMax),  x,       y+idx/2);  // left   middle 
	
		SetHMPixel(RandomizePixel((flu+fru+fld+frd)/4.0f,fdMax),  x+idx/2, y+idx/2);  // middle
	}
	else
	{
		SetHMPixel(RandomizePixel(65536.0f/2.0f,fdMax),  x+idx/2, y      );  // middle top
		SetHMPixel(RandomizePixel(65536.0f/2.0f,fdMax),  x+idx/2, y+idx  );  // middle bottom
		SetHMPixel(RandomizePixel(65536.0f/2.0f,fdMax),  x+idx,   y+idx/2);  // right  middle 
		SetHMPixel(RandomizePixel(65536.0f/2.0f,fdMax),  x,       y+idx/2);  // left   middle 
	
		SetHMPixel(RandomizePixel(65536.0f/2.0f,fdMax),  x+idx/2, y+idx/2);  // middle
	}
	
	fdMax*=0.5f;
	if(idx>1)
	{
		SubdivideAndDisplace(x      ,y      , idx/2, fdMax);
		SubdivideAndDisplace(x+idx/2,y      , idx/2, fdMax);
		SubdivideAndDisplace(x      ,y+idx/2, idx/2, fdMax);
		SubdivideAndDisplace(x+idx/2,y+idx/2, idx/2, fdMax);
	}
}

CTRect GetTerrainRect(void)
{
	CTRect rect;
	rect.rc_slLeft=0;
	rect.rc_slRight=0;
	rect.rc_slTop=0;
	rect.rc_slBottom=0;

	CTerrain *ptrTerrain=GetTerrain();
	if( ptrTerrain==NULL) return rect;

	rect.rc_slLeft=0;
	rect.rc_slRight=TR_GetHeightMapWidth(ptrTerrain);
	rect.rc_slTop=0;
	rect.rc_slBottom=TR_GetHeightMapHeight(ptrTerrain);
	return rect;
}

FLOAT GetWrappedPixelValue( INDEX x, INDEX y)
{
	INDEX iWrapX=(x+WNOISE)%WNOISE;
	INDEX iWrapY=(y+WNOISE)%WNOISE;
	return _pafWhiteNoise[iWrapY*WNOISE+iWrapX];
}

void RandomizeWhiteNoise(void)
{
	if(_pafWhiteNoise==NULL)
	{
		_pafWhiteNoise=(FLOAT *)AllocMemory(WNOISE*WNOISE*sizeof(FLOAT));
	}

	FLOAT *pfTemp=_pafWhiteNoise;
	for(INDEX i=0; i<WNOISE*WNOISE; i++)
	{
		FLOAT fRnd=FLOAT(rand())/RAND_MAX-0.5f;
		*pfTemp=fRnd;
		pfTemp++;
	}    
}

FLOAT *GenerateTerrain_FBMBuffer(PIX pixW, PIX pixH, INDEX ctOctaves, FLOAT fHighFrequencyStep,
																 FLOAT fStepFactor, FLOAT fMaxAmplitude, FLOAT fAmplitudeDecreaser,
																 BOOL bAddNegativeValues, BOOL bRandomOffest, FLOAT &fMin, FLOAT &fMax)
{
	if(_pafWhiteNoise==NULL)
	{
		RandomizeWhiteNoise();
	}
	FLOAT *pfTemp=_pafWhiteNoise;

	FLOAT fTmpMaxAmplitude=fMaxAmplitude;
	INDEX ctMemory=pixW*pixH*sizeof(FLOAT);
	FLOAT *pafFBM=(FLOAT *)AllocMemory(ctMemory);
	memset(pafFBM,0,ctMemory);

	FLOAT fPixStep=fHighFrequencyStep/pow(fStepFactor,ctOctaves);
	fMin=1e6;
	fMax=-1e6;
	for(INDEX iOctave=ctOctaves-1; iOctave>=0; iOctave--)
	{
		FLOAT fOctaveOffset=0.0f;
		if( bRandomOffest)
		{
			fOctaveOffset=_pafWhiteNoise[iOctave];
		}
		for(INDEX y=0; y<pixH; y++)
		{
			for(INDEX x=0; x<pixW; x++)
			{
				FLOAT fY=y*fPixStep+fOctaveOffset;
				FLOAT fX=x*fPixStep+fOctaveOffset;
				// calculate bilinear value
				FLOAT fLU=GetWrappedPixelValue( fX  , fY);
				FLOAT fRU=GetWrappedPixelValue( fX+1, fY);
				FLOAT fLD=GetWrappedPixelValue( fX  , fY+1);
				FLOAT fRD=GetWrappedPixelValue( fX+1, fY+1);
				FLOAT fFX=fX-INDEX(fX);
				FLOAT fFY=fY-INDEX(fY);
				FLOAT fBil=Lerp(Lerp(fLU,fRU,fFX),Lerp(fLD,fRD,fFX),fFY);
				INDEX iOffset=pixW*y+x;
				FLOAT fValue=pafFBM[iOffset];
				FLOAT fAdd=fBil*fTmpMaxAmplitude;
				if(bAddNegativeValues || fAdd>0)
				{
					fValue=fValue+fAdd;
				}
				pafFBM[iOffset]=fValue;
				if(fValue>fMax) fMax=fValue;
				if(fValue<fMin) fMin=fValue;
			}
		}
		fPixStep*=fStepFactor;
		fTmpMaxAmplitude*=fAmplitudeDecreaser;
	}
	return pafFBM;
}

void GenerateTerrain_SubdivideAndDisplace(void)
{
	// inside subdivide and displace functions we will use these global variables
	_iTerrainWidth=_rect.GetWidth();
	_iTerrainHeight=_rect.GetHeight();
	_puwHeightMap=_puwBuffer;
	UWORD uwScrollValue=8.0f-Clamp(theApp.m_iRNDSubdivideAndDisplaceItterations, INDEX(0), INDEX(8));
	_iRandomDX=(_iTerrainWidth-1)<<uwScrollValue;
	
	UWORD uwrnd;
	FLOAT fdMax=65536.0f;
	for (INDEX i=0; i<_rect.GetWidth()*_rect.GetHeight(); i++) {
		_puwHeightMap[i] = 65535;
	}
	uwrnd=RandomizePixel(fdMax/2.0f,fdMax); SetHMPixel(uwrnd,                0,                0);
	uwrnd=RandomizePixel(fdMax/2.0f,fdMax); SetHMPixel(uwrnd, _iTerrainWidth-1,                0);
	uwrnd=RandomizePixel(fdMax/2.0f,fdMax); SetHMPixel(uwrnd, _iTerrainWidth-1, _iTerrainHeight-1);
	uwrnd=RandomizePixel(fdMax/2.0f,fdMax); SetHMPixel(uwrnd,                0, _iTerrainHeight-1);
	// generate rest of terrain pixels using recursion
	SubdivideAndDisplace(0,0,_iTerrainWidth-1,fdMax/2.0f);
}

void GenerateTerrain(void)
{
	CTerrain *ptrTerrain=GetTerrain();
	if( ptrTerrain==NULL) return;

	switch(theApp.m_iTerrainGenerationMethod)
	{
		case 0:
		{
			GenerateTerrain_SubdivideAndDisplace();
			break;
		}
		case 1:
		{
			FLOAT fMin, fMax;
			PIX pixTerrainW=TR_GetHeightMapWidth(ptrTerrain);
			PIX pixTerrainH=TR_GetHeightMapHeight(ptrTerrain);
			FLOAT *pafFBM=GenerateTerrain_FBMBuffer( pixTerrainW, pixTerrainH, theApp.m_iFBMOctaves,
				theApp.m_fFBMHighFrequencyStep,theApp.m_fFBMStepFactor, theApp.m_fFBMMaxAmplitude,
				theApp.m_fFBMfAmplitudeDecreaser, theApp.m_bFBMAddNegativeValues, theApp.m_bFBMRandomOffset, fMin, fMax);
			// convert buffer to height map
			FLOAT fSizeY=TR_GetTerrainSize(ptrTerrain)(2);
			FLOAT fConvertFactor=(theApp.m_fFBMMaxAmplitude/fSizeY)*MAX_UWORD;
			// set height map
			for(INDEX iPix=0; iPix<pixTerrainW*pixTerrainH; iPix++)
			{
				FLOAT fValue=pafFBM[iPix];
				UWORD uwValue=UWORD(Clamp((fValue-fMin)/(fMax-fMin)*fConvertFactor,0.0f,65535.0f));
				_puwBuffer[iPix]=uwValue;
			}
	
			FreeMemory( pafFBM);
			break;
		}
	}
}

void EqualizeBuffer(void)
{
	UWORD uwHeightMax=0;
	UWORD uwHeightMin=MAX_UWORD;
	INDEX x,y;

	for(y=0; y<_rect.GetHeight(); y++)
	{
		for(x=0; x<_rect.GetWidth(); x++)
		{
			INDEX iOffset = y*_rect.GetWidth()+x;
			UWORD uwHeight = _puwBuffer[iOffset];
			if( uwHeight>uwHeightMax) uwHeightMax=uwHeight;
			if( uwHeight<uwHeightMin) uwHeightMin=uwHeight;
		}
	}
	
	FLOAT fFactor=65535.0f/(uwHeightMax-uwHeightMin);
	// equalize (normalize from 0 to 65535)
	for(y=0; y<_rect.GetHeight(); y++)
	{
		for(x=0; x<_rect.GetWidth(); x++)
		{
			INDEX iOffset = y*_rect.GetWidth()+x;
			UWORD uwHeight = _puwBuffer[iOffset];
			FLOAT fNormalized=(uwHeight-uwHeightMin)*fFactor;
			_puwBuffer[iOffset]=fNormalized;
		}
	}
}

BOOL SetupContinousNoiseTexture( void)
{
	try
	{
		_ptdContinousRandomNoise=_pTextureStock->Obtain_t( theApp.m_fnContinousNoiseTexture);
		_ptdContinousRandomNoise->Force(TEX_STATIC|TEX_CONSTANT);
	}
	catch( char *strError)
	{
		(void) strError;
		WarningMessage("Unable to obtain continous random noise texture!\nError: %s", strError);
		return FALSE;
	}
	return TRUE;
}

void FreeContinousNoiseTexture( void)
{
	_pTextureStock->Release( _ptdContinousRandomNoise);
}

BOOL SetupDistributionNoiseTexture( void)
{
	try
	{
		_ptdDistributionRandomNoise=_pTextureStock->Obtain_t( theApp.m_fnDistributionNoiseTexture);
		_ptdDistributionRandomNoise->Force(TEX_STATIC|TEX_CONSTANT);
	}
	catch( char *strError)
	{
		(void) strError;
		WarningMessage("Unable to obtain distribution random noise texture!\nError: %s", strError);
		return FALSE;
	}
	return TRUE;
}

void FreeDistributionNoiseTexture( void)
{
	_pTextureStock->Release( _ptdDistributionRandomNoise);
}

FLOAT3D NormalFrom4Points(const FLOAT3D &v0, const FLOAT3D &v1, const FLOAT3D &v2, const FLOAT3D &v3,
													FLOAT fLerpX, FLOAT fLerpZ)
{
	FLOAT fHDeltaX = Lerp(v1(2)-v0(2), v3(2)-v2(2), fLerpZ);
	FLOAT fHDeltaZ = Lerp(v0(2)-v2(2), v1(2)-v3(2), fLerpX);
	FLOAT fDeltaX  = v1(1) - v0(1);
	FLOAT fDeltaZ  = v0(3) - v2(3);

	FLOAT3D vNormal;
	vNormal(2) = sqrt(1 / (((fHDeltaX*fHDeltaX)/(fDeltaX*fDeltaX)) + ((fHDeltaZ*fHDeltaZ)/(fDeltaZ*fDeltaZ)) + 1));
	vNormal(1) = sqrt(vNormal(2)*vNormal(2) * ((fHDeltaX*fHDeltaX) / (fDeltaX*fDeltaX)));
	vNormal(3) = sqrt(vNormal(2)*vNormal(2) * ((fHDeltaZ*fHDeltaZ) / (fDeltaZ*fDeltaZ)));
	if (fHDeltaX>0) {
		vNormal(1) = -vNormal(1);
	}
	if (fHDeltaZ<0) {
		vNormal(3) = -vNormal(3);
	}
	//ASSERT(Abs(vNormal.Length()-1)<0.01);
	return vNormal;
}

FLOAT3D GetPoint(CTerrain *ptrTerrain, INDEX iX, INDEX iY)
{
	const FLOAT3D &vStretch = TR_GetTerrainStretch(ptrTerrain);;
	iX = Clamp(iX, INDEX(0), TR_GetHeightMapWidth(ptrTerrain));
	iY = Clamp(iY, INDEX(0), TR_GetHeightMapHeight(ptrTerrain));

	return FLOAT3D(
		FLOAT(iX)*vStretch(1), 
		(FLOAT)ptrTerrain->tr_ptrTerrain->tr_puwHeightMap[iX+iY*TR_GetHeightMapWidth(ptrTerrain)] * vStretch(2),
		FLOAT(iY)*vStretch(3));
}

UWORD GetSlope(CTerrain *ptrTerrain, INDEX iX, INDEX iY)
{
	FLOAT3D av[9];
	INDEX iHMapWidth = TR_GetHeightMapWidth(ptrTerrain);
	FLOAT3D vStretch = TR_GetTerrainStretch(ptrTerrain);

	av[0] = GetPoint(ptrTerrain, iX-1, iY-1);
	av[1] = GetPoint(ptrTerrain, iX  , iY-1);
	av[2] = GetPoint(ptrTerrain, iX+1, iY-1);
	av[3] = GetPoint(ptrTerrain, iX-1, iY  );
	av[4] = GetPoint(ptrTerrain, iX  , iY  );
	av[5] = GetPoint(ptrTerrain, iX+1, iY  );
	av[6] = GetPoint(ptrTerrain, iX-1, iY+1);
	av[7] = GetPoint(ptrTerrain, iX  , iY+1);
	av[8] = GetPoint(ptrTerrain, iX+1, iY+1);

	FLOAT3D avN[4];
	FLOAT3D vNormal;
	avN[0] = NormalFrom4Points(av[0], av[1], av[3], av[4], 1, 1);
	avN[1] = NormalFrom4Points(av[1], av[2], av[4], av[5], 0, 1);
	avN[2] = NormalFrom4Points(av[3], av[4], av[6], av[7], 1, 0);
	avN[3] = NormalFrom4Points(av[4], av[5], av[7], av[8], 0, 0);

	vNormal = avN[0]+avN[1]+avN[2]+avN[3];
	vNormal.Normalize();

	return (1-vNormal(2))*65535;
}

void GenerateLayerDistribution(INDEX iForLayer, CTRect rect)
{
	if(iForLayer==-1) {
		return;
	}
	if(!SetupDistributionNoiseTexture()) return;

	CTerrain *ptrTerrain=GetTerrain();
	if( ptrTerrain==NULL) return;

	// obtain buffer
	UWORD *puwAltitude=TRE_GetBufferForEditing(ptrTerrain, rect, BT_HEIGHT_MAP, 0);
	INDEX ctSize=rect.GetWidth()*rect.GetHeight()*sizeof(UWORD);
	UWORD *puwSlope=(UWORD *)AllocMemory(ctSize);

	// prepare slope buffer
	for(INDEX y=0; y<rect.GetHeight(); y++)
	{
		for(INDEX x=0; x<rect.GetWidth(); x++)
		{
			INDEX iOffset = y*rect.GetWidth()+x;
			puwSlope[iOffset] = GetSlope(ptrTerrain, x+rect.rc_slLeft, y+rect.rc_slTop);
		}
	}
	
	// for each layer
	for(INDEX iLayer=0; iLayer<ptrTerrain->tr_ptrTerrain->tr_atlLayers.Count(); iLayer++)
	{
		if( iForLayer!=-1 && iLayer!=iForLayer) continue;
		CTerrainLayer *ptlLayer=GetLayer(iLayer);
		if(!ptlLayer->tl_bAutoRegenerated) continue;
		// get layer
		UWORD *puwMask=TRE_GetBufferForEditing(ptrTerrain, rect, BT_LAYER_MASK, iLayer);

		for(INDEX y=0; y<rect.GetHeight(); y++)
		{
			INDEX oy=y+rect.rc_slTop;
			for(INDEX x=0; x<rect.GetWidth(); x++)
			{
				INDEX ox=x+rect.rc_slLeft;
				INDEX iOffset = y*rect.GetWidth()+x;
				FLOAT fAltitudeRatio = puwAltitude[iOffset]/65535.0f;
				FLOAT fSlopeRatio = puwSlope[iOffset]/65535.0f;
				FLOAT fAltitudeRange=ptlLayer->tl_fMaxAltitude-ptlLayer->tl_fMinAltitude;
				FLOAT fSlopeRange=ptlLayer->tl_fMaxSlope-ptlLayer->tl_fMinSlope;

				// get coverage influence
				FLOAT fCoverageInfluence=1.0f;
				FLOAT fCoverageRandom=GetDistributionNoise( ox, oy, ptlLayer->tl_fCoverageRandom);
				fCoverageInfluence=StepDown(fCoverageRandom, ptlLayer->tl_fCoverage, ptlLayer->tl_fCoverage+ptlLayer->tl_fCoverageNoise);

				// get min altitude influence
				FLOAT fMinAltitudeInfluence=1.0f;
				if(ptlLayer->tl_bApplyMinAltitude)
				{
					FLOAT fMinAltitudeNoise=(GetDistributionNoise( ox, oy, ptlLayer->tl_fMinAltitudeRandom)-0.5f)*ptlLayer->tl_fMinAltitudeNoise;
					FLOAT fAltMinFade1=ptlLayer->tl_fMinAltitude+fAltitudeRange*ptlLayer->tl_fMinAltitudeFade;
					fMinAltitudeInfluence=StepUp(fAltitudeRatio+fMinAltitudeNoise, ptlLayer->tl_fMinAltitude, fAltMinFade1);
				}

				// get max altitude influence
				FLOAT fMaxAltitudeInfluence=1.0f;
				if(ptlLayer->tl_bApplyMaxAltitude)
				{
					FLOAT fMaxAltitudeNoise=(GetDistributionNoise( ox, oy, ptlLayer->tl_fMaxAltitudeRandom)-0.5f)*ptlLayer->tl_fMaxAltitudeNoise;
					FLOAT fAltMaxFade1=ptlLayer->tl_fMaxAltitude-fAltitudeRange*ptlLayer->tl_fMaxAltitudeFade;
					fMaxAltitudeInfluence=StepDown(fAltitudeRatio+fMaxAltitudeNoise, fAltMaxFade1, ptlLayer->tl_fMaxAltitude);
				}

				// get min slope influence
				FLOAT fMinSlopeInfluence=1.0f;
				if(ptlLayer->tl_bApplyMinSlope)
				{
					FLOAT fMinSlopeNoise=(GetDistributionNoise( ox, oy, ptlLayer->tl_fMinSlopeRandom)-0.5f)*ptlLayer->tl_fMinSlopeNoise;
					FLOAT fSlopeMinFade1=ptlLayer->tl_fMinSlope+fSlopeRange*ptlLayer->tl_fMinSlopeFade;
					fMinSlopeInfluence=StepUp(fSlopeRatio+fMinSlopeNoise, ptlLayer->tl_fMinSlope, fSlopeMinFade1);
				}

				// get max slope influence
				FLOAT fMaxSlopeInfluence=1.0f;
				if(ptlLayer->tl_bApplyMaxSlope)
				{
					FLOAT fMaxSlopeNoise=(GetDistributionNoise( ox, oy, ptlLayer->tl_fMaxSlopeRandom)-0.5f)*ptlLayer->tl_fMaxSlopeNoise;
					FLOAT fSlopeMaxFade1=ptlLayer->tl_fMaxSlope-fSlopeRange*ptlLayer->tl_fMaxSlopeFade;
					fMaxSlopeInfluence=StepDown(fSlopeRatio+fMaxSlopeNoise, fSlopeMaxFade1, ptlLayer->tl_fMaxSlope);
				}

				// calculate result of all influences
				puwMask[iOffset]= 65535.0f*
					fCoverageInfluence*
					fMinAltitudeInfluence*
					fMaxAltitudeInfluence*
					fMinSlopeInfluence*
					fMaxSlopeInfluence;
			}
		}
		// apply buffer change
		TRE_SetBufferForEditing(ptrTerrain, puwMask, rect, BT_LAYER_MASK, iLayer);
		TRE_FreeBufferForEditing(puwMask);
		theApp.GetActiveDocument()->SetModifiedFlag( TRUE);
	}
	FreeMemory(puwSlope);
	TRE_FreeBufferForEditing(puwAltitude);
	FreeDistributionNoiseTexture();
	theApp.m_ctTerrainPageCanvas.MarkChanged();
}

void GenerateLayerDistribution(INDEX iForLayer)
{
	CTerrain *ptrTerrain=GetTerrain();
	if( ptrTerrain==NULL) return;

	CTRect rect;
	rect.rc_slLeft=0;
	rect.rc_slRight=TR_GetHeightMapWidth(ptrTerrain);
	rect.rc_slTop=0;
	rect.rc_slBottom=TR_GetHeightMapHeight(ptrTerrain);
	
	GenerateLayerDistribution(iForLayer, rect);
}
					
void RecalculateShadows(void)
{
	CTerrain *ptrTerrain=GetTerrain();
	if( ptrTerrain==NULL) return;
	Matrix12 mTerrain;
	TR_GetMatrixFromEntity(mTerrain,ptrTerrain);
	TR_UpdateShadowMap(ptrTerrain,mTerrain,FLOATaabbox3D());
}

void OptimizeLayers(void)
{
	CTerrain *ptrTerrain=GetTerrain();
	if( ptrTerrain==NULL) return;

	PIX		pixHeightMapWidth = TR_GetHeightMapWidth( ptrTerrain );			// yjpark |<--
	PIX		pixHeightMapHeight = TR_GetHeightMapHeight( ptrTerrain );
	CTRect	rect;
	rect.rc_slLeft=0;
	rect.rc_slTop=0;															// yjpark     -->|

	CStaticArray<UWORD*> apuwLayers;
	// obtain buffer
	INDEX ctLayers=ptrTerrain->tr_ptrTerrain->tr_atlLayers.Count();
	apuwLayers.New(ctLayers);

	INDEX iLayer, iMaskStretch;																	// yjpark |<--
	for( iLayer=0; iLayer<ctLayers; iLayer++)
	{
		iMaskStretch = 1 << ptrTerrain->tr_ptrTerrain->tr_atlLayers[iLayer].tl_slMaskStretch;
		rect.rc_slRight = ( pixHeightMapWidth - 1 ) * iMaskStretch;
		rect.rc_slBottom = ( pixHeightMapHeight - 1 ) * iMaskStretch;								// yjpark     -->|
	
		UWORD *puw=TRE_GetBufferForEditing(ptrTerrain, rect, BT_LAYER_MASK, iLayer);
		apuwLayers[iLayer]=puw;
	}

	// count overdraw before optimisation
	INDEX		ctDrawnBefore = 0, ctVisiblePix;											// yjpark |<--
	INDEX		x, y;
	PIX		pixW, pixH, pixSW, pixSH;
	for( iLayer = 0; iLayer < ctLayers; iLayer++ )
	{
		iMaskStretch = 1 << ptrTerrain->tr_ptrTerrain->tr_atlLayers[iLayer].tl_slMaskStretch;
		pixW = ( pixHeightMapWidth - 1 ) * iMaskStretch;
		pixH = ( pixHeightMapHeight - 1 ) * iMaskStretch;
		for( y = 0; y < pixH; y += iMaskStretch )
		{
			for( x = 0; x < pixW; x += iMaskStretch )
			{
				ctVisiblePix = 0;
				for( pixSH = 0; pixSH < iMaskStretch; pixSH++ )
				{
					for( pixSW = 0; pixSW < iMaskStretch; pixSW++ )
					{
						UWORD	*puwCurr = apuwLayers[iLayer] + ( x + pixSW ) + ( y + pixSH ) * pixW;
						if( (*puwCurr) >> 8 != 0 ) ctVisiblePix++;
					}
				}
				if( ctVisiblePix > 0 ) ctDrawnBefore++;
			}
		}
	}

	// optimize for overdraw
	INDEX	ctAllPixValue, ctVisPixValue;
	for( iLayer = ctLayers - 1; iLayer >= 1; iLayer-- )
	{
		CTerrainLayer &tl=ptrTerrain->tr_ptrTerrain->tr_atlLayers[iLayer];
		if(tl.tl_ltType!=LT_NORMAL) continue;
		
		iMaskStretch = 1 << tl.tl_slMaskStretch;
		pixW = ( pixHeightMapWidth - 1 ) * iMaskStretch;
		pixH = ( pixHeightMapHeight - 1 ) * iMaskStretch;
		ctAllPixValue = ( iMaskStretch + 2 ) * ( iMaskStretch + 2 ) * 255;
		for( y = 0; y < pixH; y += iMaskStretch )
		{
			for( x = 0; x < pixW; x += iMaskStretch )
			{
				ctVisPixValue = 0;
				INDEX	sx = x - 1, sy = y - 1;
				for( pixSH = 0; pixSH < iMaskStretch + 2; pixSH++ )
				{
					for( pixSW = 0; pixSW < iMaskStretch + 2; pixSW++ )
					{
						UBYTE	ubMaskValue = ( *( apuwLayers[iLayer] +
								               ( Clamp( INDEX(sy + pixSH), INDEX(0), INDEX(pixH - 1) ) * pixW + 
									             Clamp( INDEX(sx + pixSW), INDEX(0), INDEX(pixW - 1) ) ) ) ) >> 8;
						ctVisPixValue += ubMaskValue;
					}
				}
				if( ctVisPixValue == ctAllPixValue )
				{
					// clear all layers below tested pixel
					for(INDEX iOptLayer=iLayer-1; iOptLayer>=0; iOptLayer--)
					{
						CTerrainLayer &tl=ptrTerrain->tr_ptrTerrain->tr_atlLayers[iOptLayer];
						if(tl.tl_ltType!=LT_NORMAL) continue;
						INDEX	iBelowMaskStretch = 1 << tl.tl_slMaskStretch;
						INDEX	iMaskRatio;
						if( iMaskStretch > iBelowMaskStretch )
						{
							iMaskRatio = iMaskStretch / iBelowMaskStretch;
							sx = x / iMaskRatio;
							sy = y / iMaskRatio;
						}
						else
						{
							iMaskRatio = iBelowMaskStretch / iMaskStretch;
							sx = x * iMaskRatio;
							sy = y * iMaskRatio;
						}
						PIX		pixBelowW = ( pixHeightMapWidth - 1 ) * iBelowMaskStretch;
						for( pixSH = 0; pixSH < iBelowMaskStretch; pixSH++ )
						{
							for( pixSW = 0; pixSW < iBelowMaskStretch; pixSW++ )
							{
								*( apuwLayers[iOptLayer] + ( sx + pixSW ) + ( sy + pixSH ) * pixBelowW ) = 0;
							}
						}
					}
				}
			}
		}
	}

	// count overdraw after optimisation
	INDEX ctDrawnAfter=0;
	for( iLayer = 0; iLayer < ctLayers; iLayer++ )
	{
		iMaskStretch = 1 << ptrTerrain->tr_ptrTerrain->tr_atlLayers[iLayer].tl_slMaskStretch;
		pixW = ( pixHeightMapWidth - 1 ) * iMaskStretch;
		pixH = ( pixHeightMapHeight - 1 ) * iMaskStretch;
		for( y = 0; y < pixH; y += iMaskStretch )
		{
			for( x = 0; x < pixW; x += iMaskStretch )
			{
				ctVisiblePix = 0;
				for( pixSH = 0; pixSH < iMaskStretch; pixSH++ )
				{
					for( pixSW = 0; pixSW < iMaskStretch; pixSW++ )
					{
						UWORD	*puwCurr = apuwLayers[iLayer] + ( x + pixSW ) + ( y + pixSH ) * pixW;
						if( (*puwCurr) >> 8 != 0 ) ctVisiblePix++;
					}
				}
				if( ctVisiblePix > 0 ) ctDrawnAfter++;
			}
		}
	}																							// yjpark     -->|

	// make a report about optimisation success
	CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	CTString strReport;
	strReport.PrintF( "Overdraw before optimization: %d tris. Overdraw after optimization: %d tris",	// yjpark
										ctDrawnBefore, ctDrawnAfter );
	pMainFrame->SetStatusBarMessage( strReport, STATUS_LINE_PANE, 5.0f);

	// free buffers
	for( iLayer=0; iLayer<ctLayers; iLayer++)
	{
		iMaskStretch = 1 << ptrTerrain->tr_ptrTerrain->tr_atlLayers[iLayer].tl_slMaskStretch;		// yjpark |<--
		rect.rc_slRight = ( pixHeightMapWidth - 1 ) * iMaskStretch;
		rect.rc_slBottom = ( pixHeightMapHeight - 1 ) * iMaskStretch;								// yjpark     -->|

		TRE_SetBufferForEditing(ptrTerrain, apuwLayers[iLayer], rect, BT_LAYER_MASK, iLayer);
		TRE_FreeBufferForEditing(apuwLayers[iLayer]);
	}
	theApp.GetActiveDocument()->SetModifiedFlag( TRUE);
	theApp.m_ctTerrainPageCanvas.MarkChanged();
}

BOOL _bIsUpToDate=TRUE;
CTRect _rectDiscarded;
void DiscardLayerDistribution(CTRect rect)
{
	if(_bIsUpToDate)
	{
		_rectDiscarded.rc_slLeft=rect.rc_slLeft;    
		_rectDiscarded.rc_slRight=rect.rc_slRight;      
		_rectDiscarded.rc_slTop=rect.rc_slTop;          
		_rectDiscarded.rc_slBottom=rect.rc_slBottom;
	}
	else
	{
		if(rect.rc_slLeft   < _rectDiscarded.rc_slLeft)    _rectDiscarded.rc_slLeft=rect.rc_slLeft;
		if(rect.rc_slRight  > _rectDiscarded.rc_slRight)   _rectDiscarded.rc_slRight=rect.rc_slRight;
		if(rect.rc_slTop    < _rectDiscarded.rc_slTop)     _rectDiscarded.rc_slTop=rect.rc_slTop;
		if(rect.rc_slBottom > _rectDiscarded.rc_slBottom)  _rectDiscarded.rc_slBottom=rect.rc_slBottom;
	}
	_bIsUpToDate=FALSE;
}

void UpdateLayerDistribution(void)
{
	if(_bIsUpToDate || !theApp.m_Preferences.ap_bAutoUpdateTerrainDistribution) return;
	// update layer distribution
	GenerateLayerDistribution(-1, _rectDiscarded);
	_bIsUpToDate=TRUE;
}

void ApplyFilterOntoTerrain(void)
{
	EditTerrain(NULL, FLOAT3D(0,0,0), theApp.m_fFilterPower*16.0f, TE_ALTITUDE_FILTER);
}

void ApplySmoothOntoTerrain(void)
{
	EditTerrain(NULL, FLOAT3D(0,0,0), theApp.m_fSmoothPower*16.0f, TE_ALTITUDE_SMOOTH);
}

void ApplyEqualizeOntoTerrain(void)
{
	EditTerrain(NULL, FLOAT3D(0,0,0), 1.0f, TE_ALTITUDE_EQUALIZE);
}

void ApplyGenerateTerrain(void)
{
	EditTerrain(NULL, FLOAT3D(0,0,0), 1.0f, TE_GENERATE_TERRAIN);
}

void ApplyRndNoiseOntoTerrain(void)
{
	EditTerrain(NULL, FLOAT3D(0,0,0), theApp.m_fNoiseAltitude, TE_ALTITUDE_RND_NOISE);
}

void ApplyContinousNoiseOntoTerrain(void)
{
	EditTerrain(NULL, FLOAT3D(0,0,0), theApp.m_fNoiseAltitude, TE_ALTITUDE_CONTINOUS_NOISE);
}

void ApplyMinimumOntoTerrain(void)
{
	EditTerrain(NULL, FLOAT3D(0,0,0), 1.0f, TE_ALTITUDE_MINIMUM);
}

void ApplyMaximumOntoTerrain(void)
{
	EditTerrain(NULL, FLOAT3D(0,0,0), 1.0f, TE_ALTITUDE_MAXIMUM);
}

void ApplyFlattenOntoTerrain(void)
{
	EditTerrain(NULL, FLOAT3D(0,0,0), 1.0f, TE_ALTITUDE_FLATTEN);
}

void ApplyPosterizeOntoTerrain(void)
{
	EditTerrain(NULL, FLOAT3D(0,0,0), theApp.m_fPosterizeStep, TE_ALTITUDE_POSTERIZE);
}

CEntity *GetEntityForID(ULONG iEntityID)
{
	CWorldEditorDoc* pDoc = theApp.GetActiveDocument();
	FOREACHINDYNAMICCONTAINER(pDoc->m_woWorld.wo_cenEntities, CEntity, iten)
	{
		if(iten->en_ulID==iEntityID) return &*iten;
	}
	return NULL;
}

// constructor
CTerrainUndo::CTerrainUndo()
{
	tu_puwUndoBuffer=NULL;
	tu_pulUndoBuffer=NULL;
	tu_puwRedoBuffer=NULL;
	tu_pulRedoBuffer=NULL;
}

void DeleteOneUndo(CTerrainUndo *ptrud)
{
	if(ptrud->tu_puwUndoBuffer!=NULL)    TRE_FreeBufferForEditing(ptrud->tu_puwUndoBuffer);
	if(ptrud->tu_puwRedoBuffer!=NULL)    TRE_FreeBufferForEditing(ptrud->tu_puwRedoBuffer);
	delete ptrud;
}

void DeleteTerrainUndo(CWorldEditorDoc* pDoc)
{
	for(INDEX iUndo=0; iUndo<pDoc->m_dcTerrainUndo.Count(); iUndo++)
	{
		CTerrainUndo *ptu=&pDoc->m_dcTerrainUndo[iUndo];
		pDoc->m_dcTerrainUndo.Remove(ptu);
		DeleteOneUndo(ptu);
	}
}

CTerrain *GetUndoTerrain(ULONG ulEntityID)
{
	// obtain terrain entity
	CEntity *penTerrain=GetEntityForID(_iTerrainEntityID);
	if(penTerrain==NULL)
	{
		return NULL;
	 }
	// obtain terrain
	CTerrain *ptrTerrain=penTerrain->GetTerrain();
	return ptrTerrain;
}

void ApplyTerrainUndo(CTerrainUndo *ptrud)
{
	CWorldEditorDoc* pDoc = theApp.GetActiveDocument();
	CTerrain *ptrTerrain=GetUndoTerrain(ptrud->tu_ulEntityID);
	if(ptrTerrain==NULL)
	{
		DeleteOneUndo(ptrud);
		return;
	}

	// obtain and store redo buffer
	if(ptrud->tu_puwRedoBuffer==NULL)
	{
		ptrud->tu_puwRedoBuffer=TRE_GetBufferForEditing(ptrTerrain, ptrud->tu_rcRect, 
		ptrud->tu_btUndoBufferType, ptrud->tu_iUndoBufferData);
	}

	if(ptrud->tu_pulRedoBuffer==NULL)
	{
		ptrud->tu_pulRedoBuffer=TRE_Get4ByteBufferForEditing(ptrTerrain, ptrud->tu_rcRect, 
		ptrud->tu_btUndoBufferType, ptrud->tu_iUndoBufferData);
	}

	// apply buffer change (undo)
	if( ptrud->tu_puwUndoBuffer )
	{
	TRE_SetBufferForEditing(ptrTerrain, ptrud->tu_puwUndoBuffer, ptrud->tu_rcRect,
	ptrud->tu_btUndoBufferType, ptrud->tu_iUndoBufferData);
	}
	if( ptrud->tu_pulUndoBuffer )
	{
		TRE_Set4ByteBufferForEditing( ptrTerrain, ptrud->tu_pulUndoBuffer, ptrud->tu_rcRect, 
		ptrud->tu_btUndoBufferType, ptrud->tu_iUndoBufferData);
	}

	pDoc->m_iCurrentTerrainUndo--;

	DiscardLayerDistribution(ptrud->tu_rcRect);
	pDoc->SetModifiedFlag( TRUE);
	theApp.m_ctTerrainPageCanvas.MarkChanged();
}

void ApplyTerrainRedo(CTerrainUndo *ptrud)
{
	CWorldEditorDoc* pDoc = theApp.GetActiveDocument();
	CTerrain *ptrTerrain=GetUndoTerrain(ptrud->tu_ulEntityID);
	if(ptrTerrain==NULL)
	{
		DeleteOneUndo(ptrud);
		return;
	}

	if( ptrud->tu_puwRedoBuffer==NULL && ptrud->tu_pulRedoBuffer==NULL) 
		return;

	// apply buffer change (redo)
	if( ptrud->tu_puwUndoBuffer )
	{
	TRE_SetBufferForEditing(ptrTerrain, ptrud->tu_puwRedoBuffer, ptrud->tu_rcRect,
	ptrud->tu_btUndoBufferType, ptrud->tu_iUndoBufferData);
	}
	if( ptrud->tu_pulUndoBuffer )
	{
		TRE_Set4ByteBufferForEditing( ptrTerrain, ptrud->tu_pulRedoBuffer, ptrud->tu_rcRect, 
		ptrud->tu_btUndoBufferType, ptrud->tu_iUndoBufferData);
	}

	pDoc->m_iCurrentTerrainUndo++;

	DiscardLayerDistribution(ptrud->tu_rcRect);
	pDoc->SetModifiedFlag( TRUE);
	theApp.m_ctTerrainPageCanvas.MarkChanged();
}

UWORD *ExtractUndoRect(PIX pixTerrainWidth)
{
	if( _puwUndoTerrain == NULL )			// yjpark
	  return NULL;						// yjpark

	INDEX ctBuffBytes=_rectUndo.GetWidth()*_rectUndo.GetHeight()*sizeof(UWORD);
	UWORD *puwBuff=(UWORD*)AllocMemory(ctBuffBytes);
	if(puwBuff==NULL) return NULL;
	UWORD *puwBuffTemp=puwBuff;
	for(INDEX y=_rectUndo.rc_slTop; y<_rectUndo.rc_slBottom; y++)
	{
		for(INDEX x=_rectUndo.rc_slLeft; x<_rectUndo.rc_slRight; x++)
		{
			INDEX iOffset=y*pixTerrainWidth+x;
			UWORD uwValue=_puwUndoTerrain[iOffset];
			*puwBuffTemp=uwValue;
			puwBuffTemp++;
		}
	}
	return puwBuff;
}

// Su-won |------------------------->
ULONG *ExtractShadowUndoRect(PIX pixTerrainWidth)
{
	if( _pulUndoTerrain == NULL )
	  return NULL;

	INDEX ctBuffBytes=_rectUndo.GetWidth()*_rectUndo.GetHeight()*sizeof(ULONG);
	ULONG *pulBuff=(ULONG*)AllocMemory(ctBuffBytes);
	if(pulBuff==NULL) return NULL;
	ULONG *pulBuffTemp=pulBuff;
	for(INDEX y=_rectUndo.rc_slTop; y<_rectUndo.rc_slBottom; y++)
	{
		for(INDEX x=_rectUndo.rc_slLeft; x<_rectUndo.rc_slRight; x++)
		{
			INDEX iOffset=y*pixTerrainWidth+x;
			ULONG ulValue=_pulUndoTerrain[iOffset];
			*pulBuffTemp=ulValue;
			pulBuffTemp++;
		}
	}
	return pulBuff;
}
// Su-won <-------------------------|
void TerrainEditBegin(void)
{
	if( !(theApp.m_Preferences.ap_iMemoryForTerrainUndo>0))
	{
		return;
	}
	_bUndoStart=TRUE;
}

void RemoveRedoList(void)
{
	CWorldEditorDoc* pDoc = theApp.GetActiveDocument();
	CDynamicContainer<CTerrainUndo> dcTemp;
	for( INDEX itu=0; itu<pDoc->m_iCurrentTerrainUndo+1; itu++)
	{
		dcTemp.Add(&pDoc->m_dcTerrainUndo[itu]);
	}
	for( INDEX ituDel=pDoc->m_iCurrentTerrainUndo+1; ituDel<pDoc->m_dcTerrainUndo.Count(); ituDel++)
	{
		DeleteOneUndo(&pDoc->m_dcTerrainUndo[ituDel]);
	}
	pDoc->m_dcTerrainUndo.MoveContainer(dcTemp);
}

void LimitMemoryConsumption(INDEX iNewConsumption)
{
	CWorldEditorDoc* pDoc = theApp.GetActiveDocument();
	INDEX ctUndos=pDoc->m_dcTerrainUndo.Count();
	INDEX iLastValid=-1;
	INDEX iSum=iNewConsumption;
	for(INDEX iUndo=ctUndos-1; iUndo>=0; iUndo--)
	{
		CTerrainUndo *ptu=&pDoc->m_dcTerrainUndo[iUndo];
		INDEX iMemory=ptu->tu_rcRect.GetWidth()*ptu->tu_rcRect.GetHeight()*sizeof(UWORD);
		if(ptu->tu_puwRedoBuffer!=NULL)
		{
			iSum=iSum+iMemory*2;
		}
		else
		{
			iSum=iSum+iMemory;
		}

		if(iSum>theApp.m_Preferences.ap_iMemoryForTerrainUndo*1024*1024)
		{
			iLastValid=iUndo+1;
			break;
		}
	}
	if( iLastValid!=-1)
	{
		CDynamicContainer<CTerrainUndo> dcTemp;
		for( INDEX itu=iLastValid; itu<ctUndos; itu++)
		{
			dcTemp.Add(&pDoc->m_dcTerrainUndo[itu]);
		}
		for( INDEX ituDel=0; ituDel<iLastValid; ituDel++)
		{
			DeleteOneUndo(&pDoc->m_dcTerrainUndo[ituDel]);
		}
		pDoc->m_dcTerrainUndo.MoveContainer(dcTemp);
		if(pDoc->m_iCurrentTerrainUndo>=iLastValid)
		{
			pDoc->m_iCurrentTerrainUndo=pDoc->m_iCurrentTerrainUndo-iLastValid;
		}
	}
}

void TerrainEditEnd( int iTerrainWidth )
{
	if( !(theApp.m_Preferences.ap_iMemoryForTerrainUndo>0))
	{
		return;
	}
	CWorldEditorDoc* pDoc = theApp.GetActiveDocument();
	// obtain terrain entity
	CEntity *penTerrain=GetEntityForID(_iTerrainEntityID);
	if(penTerrain==NULL)
	{
		if(_puwUndoTerrain!=NULL)
		{
			TRE_FreeBufferForEditing(_puwUndoTerrain);
			_puwUndoTerrain=NULL;
		}
		return;
	}
	// obtain terrain
	CTerrain *ptrTerrain=penTerrain->GetTerrain();
	if(ptrTerrain==NULL)
	{
		if(_puwUndoTerrain!=NULL)
		{
			TRE_FreeBufferForEditing(_puwUndoTerrain);
			_puwUndoTerrain=NULL;
		}
		return;
	}

	// we will add undo, clear all existing redo's
	RemoveRedoList();

	// remember undo
	CTerrainUndo *ptrud=new CTerrainUndo;
	
	INDEX iNewConsumption=_rectUndo.GetWidth()*_rectUndo.GetHeight()*sizeof(UWORD);
	LimitMemoryConsumption(iNewConsumption);
	
	ptrud->tu_ulEntityID=_iTerrainEntityID;
	ptrud->tu_rcRect=_rectUndo;
	ptrud->tu_btUndoBufferType=_btUndoBufferType;
	ptrud->tu_iUndoBufferData=_iUndoBufferData;
	if( iTerrainWidth ==0)
	ptrud->tu_puwUndoBuffer=ExtractUndoRect(TR_GetHeightMapWidth(ptrTerrain));
	else
		ptrud->tu_puwUndoBuffer=ExtractUndoRect(iTerrainWidth);
	
	SLONG slMaskStretch = ptrTerrain->tr_ptrTerrain->tr_slShadowMapSizeAspect;
	PIX	pixHeightMapWidth = TR_GetHeightMapWidth( ptrTerrain );
	PIX pixShadowWidth =GetShadowMapWidth( pixHeightMapWidth, slMaskStretch );
	ptrud->tu_pulUndoBuffer=ExtractShadowUndoRect(pixShadowWidth);
	
	if(ptrud->tu_puwUndoBuffer!=NULL || ptrud->tu_pulUndoBuffer!=NULL)
	{
		pDoc->m_dcTerrainUndo.Add(ptrud);
	}
	else
	{
		delete ptrud;
	}
	pDoc->m_iCurrentTerrainUndo=pDoc->m_dcTerrainUndo.Count()-1;
	// release obtained terrain buffer
	if(_puwUndoTerrain!=NULL)
	{
		TRE_FreeBufferForEditing(_puwUndoTerrain);
		_puwUndoTerrain=NULL;
	}
	if(_pulUndoTerrain!=NULL)
	{
		TRE_Free4ByteBufferForEditing(_pulUndoTerrain);
		_pulUndoTerrain=NULL;
	}
}

CTileInfo::CTileInfo()
{
	ti_ix=-1;
	ti_iy=-1;
	ti_bSwapXY=FALSE;
	ti_bFlipX=FALSE;
	ti_bFlipY=FALSE;
}

void ObtainLayerTileInfo(CDynamicContainer<CTileInfo> *pdcTileInfo, CTextureData *ptdTexture, INDEX &ctTilesPerRow)
{
	CTFileName fnTexture=ptdTexture->GetName();
	CTFileName fnTileInfo=fnTexture.NoExt()+CTString(".tli");

	INDEX ctParsedLines=0;
	try
	{
		char achrLine[ 256];
		CTFileStream strm;
		strm.Open_t( fnTileInfo);

		FOREVER
		{
			CDynamicContainer<CTString> dcTokens;

			strm.GetLine_t(achrLine, 256);
			ctParsedLines++;
			char achrSeparators[]   = " ,";

			char *pchrToken = strtok( achrLine, achrSeparators);
			while( pchrToken != NULL )
			{
				CTString *pstrToken=new CTString();
				*pstrToken=CTString( pchrToken);
				dcTokens.Add(pstrToken);
				// next token
				pchrToken = strtok( NULL, achrSeparators);
			}

			// if no tokens parsed
			if(dcTokens.Count()==0) continue;

			INDEX iToken=0;
			// analyze parsed tokens
			if(dcTokens[iToken]=="TilesPerRow")
			{
				// there must be at least 1 token for 'TilesPerRow' indentifier
				if(dcTokens.Count()-1-iToken<1)
				{
					throw("You must enter number of tiles per raw.");
				}
				ctTilesPerRow=0;
				INDEX iResultTPR=sscanf(dcTokens[iToken+1], "%d", &ctTilesPerRow);
				if(iResultTPR<=0)
				{
					ctTilesPerRow=0;
					throw("Unable to parse count of tiles per row.");
				}
			}
			else if(dcTokens[iToken]=="Tile")
			{
				// there must be at least 2 tokens for 'Tile' indentifier
				if(dcTokens.Count()-1-iToken<2)
				{
					throw("You must enter 2 coordinates per tile.");
				}
				INDEX x,y;

				INDEX iResultX=sscanf(dcTokens[iToken+1], "%d", &x);
				if(iResultX<=0)
				{
					throw("Unable to parse x coordinate.");
				}
				INDEX iResultY=sscanf(dcTokens[iToken+2], "%d", &y);
				if(iResultY<=0)
				{
					throw("Unable to parse y coordinate.");
				}
				if(x<=0 || y<=0)
				{
					throw("Tile coordinates must be greater than 0.");
				}
				// jump over coordinate tokens
				iToken+=3;

				// add tile info
				CTileInfo *pti=new CTileInfo();
				pti->ti_ix=x-1;
				pti->ti_iy=y-1;

				for( INDEX iFlagToken=iToken; iFlagToken<dcTokens.Count(); iFlagToken++)
				{
					if(dcTokens[iFlagToken]=="SwapXY")
					{
						pti->ti_bSwapXY=TRUE;
					}
					else if(dcTokens[iFlagToken]=="FlipX")
					{
						pti->ti_bFlipX=TRUE;
					}
					else if(dcTokens[iFlagToken]==";")
					{
						break;
					}
					else if(dcTokens[iFlagToken]=="FlipY")
					{
						pti->ti_bFlipY=TRUE;
					}
					else
					{
						throw("Unrecognizable character found.");
					}
				}
				pdcTileInfo->Add(pti);
			}

			// clear allocated tokens
			for(INDEX i=0; i<dcTokens.Count(); i++)
			{
				delete &dcTokens[i];
			}
			dcTokens.Clear();
		}
	}
	catch(char *strError)
	{
		(void) strError;
	}
}

void TilePaintTool(void)
{
	#pragma message(">> Code commented")
	/*
	CTerrain *ptrTerrain=GetTerrain();
	CTerrainLayer *ptlLayer=GetLayer();

	if(ptrTerrain==NULL || ptlLayer==NULL || ptlLayer->tl_ltType!=LT_TILE || ptlLayer->tl_ptdTexture==NULL) return;
	CDynamicContainer<CTileInfo> dcTileInfo;
	INDEX ctTilesPerRaw=0;
	ObtainLayerTileInfo( &dcTileInfo, ptlLayer->tl_ptdTexture, ctTilesPerRaw);
	INDEX ctTiles=dcTileInfo.Count();
	if(ctTilesPerRaw==0 || ctTiles==0) return;
	ptlLayer->SetTilesPerRow(ctTilesPerRaw);
	ptlLayer->tl_iSelectedTile= Clamp( ptlLayer->tl_iSelectedTile, (INDEX)0, INDEX(ctTiles-1) );
	if(ptlLayer->tl_iSelectedTile==-1) return;
	CTileInfo &ti=dcTileInfo[ptlLayer->tl_iSelectedTile];

	// _rect holds terrain size
	if(_fStrength>0)
	{
		UWORD uwValue=
			dcTileInfo[ptlLayer->tl_iSelectedTile].ti_iy*ctTilesPerRaw+
			dcTileInfo[ptlLayer->tl_iSelectedTile].ti_ix;
		if(ti.ti_bFlipX) uwValue|=TL_FLIPX;
		if(ti.ti_bFlipY) uwValue|=TL_FLIPY;
		if(ti.ti_bSwapXY) uwValue|=TL_SWAPXY;
		uwValue|=TL_VISIBLE;
		_puwBuffer[0]=uwValue<<8;
	}
	else
	{
		_puwBuffer[0]=0;
	}

#if 0
	vidjeti sta ne radi sa brisanjem tile-ova
	ne pogadja se dobro lokacija tile-a ispod misa
	+view layer on/off ne discarda pretender texture
	+ne crta se dobro trenutno selektirani tile i preklapa se animirani (under mouse)
	+rotirane i flipane tileove crtati
	+nesto zapinje kad se prvi put otvara info window
	+ubaciti skrolanje tileova na mouse wheel
	+pick tile
#endif

	// free allocated tile info structures
	for(INDEX i=0; i<dcTileInfo.Count(); i++)
	{
		delete &dcTileInfo[i];
	}
	dcTileInfo.Clear();
*/
}

void EditTerrain(CTextureData *ptdBrush, FLOAT3D &vHitPoint, FLOAT fStrength, ETerrainEdit teTool)
{
	_ptdBrush=ptdBrush;
	_fStrength=fStrength;

	CTerrain *ptrTerrain=GetTerrain();
	CTerrainLayer *ptlLayer=GetLayer();
	INDEX iLayer=GetLayerIndex();
	INDEX	iShadowMap = GetShadowMapIndex();		// yjpark
	if(ptrTerrain==NULL || ptlLayer==NULL) return;

	// obtain buffer type
	BufferType btBufferType=BT_INVALID;
	INDEX iBufferData=-1;
	SLONG	slMaskStretch = 0;			// yjpark
	if( (teTool>TE_BRUSH_ALTITUDE_START && teTool<TE_BRUSH_ALTITUDE_END) ||
			(teTool>TE_ALTITUDE_START       && teTool<TE_ALTITUDE_END) ||
			(teTool==TE_GENERATE_TERRAIN) ||
			(teTool==TE_ALTITUDE_EQUALIZE) )
	{
		btBufferType=BT_HEIGHT_MAP;
	}
	else if( (teTool>TE_BRUSH_LAYER_START && teTool<TE_BRUSH_LAYER_END) ||
					 (teTool>TE_LAYER_START       && teTool<TE_LAYER_END) ||
						teTool==TE_TILE_PAINT)
	{
		btBufferType=BT_LAYER_MASK;
		iBufferData=iLayer;
		slMaskStretch = ptlLayer->tl_slMaskStretch;			// yjpark
	}
	else if( teTool>TE_BRUSH_EDGE_START && teTool<TE_BRUSH_EDGE_END)
	{
		btBufferType=BT_EDGE_MAP;
	}
	else if( ( teTool > TE_BRUSH_SHADOW_START && teTool < TE_BRUSH_SHADOW_END ) ||		// yjpark |<--
		   ( teTool > TE_SHADOW_START && teTool < TE_SHADOW_END ) )
	{
		btBufferType = BT_SHADOW_MAP;
		iBufferData = iShadowMap;
		slMaskStretch = ptrTerrain->tr_ptrTerrain->tr_slShadowMapSizeAspect;
	}
	else if( ( teTool > TE_BRUSH_ATTRIBUTE_START && teTool < TE_BRUSH_ATTRIBUTE_END ) ||
		   ( teTool > TE_ATTRIBUTE_START && teTool < TE_ATTRIBUTE_END ) )
	{
		btBufferType = BT_ATTRIBUTE_MAP;
		slMaskStretch = ptrTerrain->tr_ptrTerrain->tr_slAttributeMapSizeAspect;
	}																					// yjpark     -->|
	else
	{
		return;
	}

	_puwBuffer=NULL;
	_srcExtraW=0;
	_srcExtraH=0;

	if( teTool==TE_BRUSH_ALTITUDE_SMOOTH ||
		teTool==TE_BRUSH_ALTITUDE_FILTER ||
		teTool==TE_BRUSH_LAYER_SMOOTH ||
		teTool==TE_ALTITUDE_SMOOTH ||
		teTool==TE_ALTITUDE_FILTER ||
		teTool==TE_LAYER_SMOOTH ||
		teTool==TE_LAYER_FILTER )
	{
		_srcExtraW=2;
		_srcExtraH=2;
	}

	// extract source rectangle
	Matrix12 mTerrain;
	TR_GetMatrixFromEntity(mTerrain,ptrTerrain);
	CTPoint pt = TR_RelFromAbsPoint( ptrTerrain, vHitPoint, mTerrain, btBufferType, slMaskStretch );	// yjpark |<--
	PIX	pixHeightMapWidth = TR_GetHeightMapWidth( ptrTerrain );
	PIX	pixHeightMapHeight = TR_GetHeightMapHeight( ptrTerrain );										// yjpark     -->|

	// perform operation on brush rect
	if(teTool==TE_TILE_PAINT)
	{
		_rect.rc_slLeft=pt(1);
		_rect.rc_slRight=_rect.rc_slLeft+1;
		_rect.rc_slTop=pt(2);
		_rect.rc_slBottom=_rect.rc_slTop+1;
	}
	else if(_ptdBrush!=NULL)
	{
		_rect.rc_slLeft=pt(1)-ptdBrush->GetPixWidth()/2-_srcExtraW;
		_rect.rc_slRight=pt(1)+(ptdBrush->GetPixWidth()-ptdBrush->GetPixWidth()/2)+_srcExtraW;
		_rect.rc_slTop=pt(2)-ptdBrush->GetPixHeight()/2-_srcExtraH;
		_rect.rc_slBottom=pt(2)+(ptdBrush->GetPixHeight()-ptdBrush->GetPixHeight()/2)+_srcExtraH;
	}
	// perform operation on whole terrain area
	else
	{
		_rect.rc_slLeft = 0;																// yjpark |<--
		_rect.rc_slTop = 0;
		if( btBufferType == BT_LAYER_MASK )
		{
			_rect.rc_slRight = ( pixHeightMapWidth - 1 ) << slMaskStretch;
			_rect.rc_slBottom = ( pixHeightMapHeight - 1 ) << slMaskStretch;
		}
		else if( btBufferType == BT_SHADOW_MAP )
		{
			_rect.rc_slRight = GetShadowMapWidth( pixHeightMapWidth, slMaskStretch );
			_rect.rc_slBottom = GetShadowMapHeight( pixHeightMapHeight, slMaskStretch );
		}
		else if( btBufferType == BT_ATTRIBUTE_MAP )
		{
			_rect.rc_slRight = ( pixHeightMapWidth - 1 ) * slMaskStretch;
			_rect.rc_slBottom = ( pixHeightMapHeight - 1 ) * slMaskStretch;
		}
		else
		{
			_rect.rc_slRight = pixHeightMapWidth;
			_rect.rc_slBottom = pixHeightMapHeight;
		}																					// yjpark     -->|
	}

	CTRect rectTerrain;
	rectTerrain.rc_slLeft=0;
	rectTerrain.rc_slTop=0;
	if( btBufferType == BT_LAYER_MASK )														// yjpark |<--
	{
		rectTerrain.rc_slRight = ( pixHeightMapWidth - 1 ) << slMaskStretch;
		rectTerrain.rc_slBottom = ( pixHeightMapHeight - 1 ) << slMaskStretch;
	}
	else if( btBufferType == BT_SHADOW_MAP )
	{
		rectTerrain.rc_slRight = GetShadowMapWidth( pixHeightMapWidth, slMaskStretch );
		rectTerrain.rc_slBottom = GetShadowMapHeight( pixHeightMapHeight, slMaskStretch );
	}
	else if( btBufferType == BT_ATTRIBUTE_MAP )
	{
		rectTerrain.rc_slRight = ( pixHeightMapWidth - 1 ) * slMaskStretch;
		rectTerrain.rc_slBottom = ( pixHeightMapHeight - 1 ) * slMaskStretch;
	}
	else
	{
		rectTerrain.rc_slRight = pixHeightMapWidth;
		rectTerrain.rc_slBottom = pixHeightMapHeight;
	}																						// yjpark     -->|

	BOOL bAutoRememberUndo=FALSE;
	// if should apply undo for whole terrain
	if( (teTool>TE_ALTITUDE_START && teTool<TE_ALTITUDE_END) ||
			(teTool>TE_LAYER_START    && teTool<TE_LAYER_END) ||
			(teTool==TE_GENERATE_TERRAIN) ||
			(teTool==TE_ALTITUDE_EQUALIZE) )
	{
		TerrainEditBegin();
		bAutoRememberUndo=TRUE;
	}

	// edit start, undo starts
	if(_bUndoStart)
	{
		_bUndoStart=FALSE;
		_btUndoBufferType=btBufferType;
		_iUndoBufferData=iBufferData;
		_rectUndo=_rect;
		_iTerrainEntityID=ptrTerrain->tr_penEntity->en_ulID;

		_puwUndoTerrain=TRE_GetBufferForEditing(ptrTerrain, rectTerrain, btBufferType, iBufferData);
		if( btBufferType == BT_SHADOW_MAP )
			_pulUndoTerrain=TRE_Get4ByteBufferForEditing( ptrTerrain, rectTerrain, btBufferType, iBufferData );
	}
	// editing in progress, update undo data
	else
	{
		// update undo rect
		if(_rect.rc_slLeft   < _rectUndo.rc_slLeft)    _rectUndo.rc_slLeft=_rect.rc_slLeft;
		if(_rect.rc_slRight  > _rectUndo.rc_slRight)   _rectUndo.rc_slRight=_rect.rc_slRight;
		if(_rect.rc_slTop    < _rectUndo.rc_slTop)     _rectUndo.rc_slTop=_rect.rc_slTop;
		if(_rect.rc_slBottom > _rectUndo.rc_slBottom)  _rectUndo.rc_slBottom=_rect.rc_slBottom;
	}

	// clamp undo rect to terrain size
	_rectUndo.rc_slLeft=Clamp(_rectUndo.rc_slLeft, rectTerrain.rc_slLeft, rectTerrain.rc_slRight);
	_rectUndo.rc_slRight=Clamp(_rectUndo.rc_slRight, rectTerrain.rc_slLeft, rectTerrain.rc_slRight);
	_rectUndo.rc_slTop=Clamp(_rectUndo.rc_slTop, rectTerrain.rc_slTop, rectTerrain.rc_slBottom);
	_rectUndo.rc_slBottom=Clamp(_rectUndo.rc_slBottom, rectTerrain.rc_slTop, rectTerrain.rc_slBottom);

	// obtain buffer
	if( btBufferType == BT_SHADOW_MAP )															// yjpark |<--
	{
		_pulBuffer = TRE_Get4ByteBufferForEditing( ptrTerrain, _rect, btBufferType, iBufferData );
	}
	else if( btBufferType == BT_ATTRIBUTE_MAP )
	{
		_pubBuffer = TRE_Get1ByteBufferForEditing( ptrTerrain, _rect, btBufferType );
	}
	else
		_puwBuffer = TRE_GetBufferForEditing( ptrTerrain, _rect, btBufferType, iBufferData );	// yjpark     -->|

	switch( teTool)
	{
	case TE_BRUSH_ALTITUDE_PAINT:
	{
		ApplyAddPaint(MIN_UWORD,MAX_UWORD);
		break;
	}
	case TE_BRUSH_LAYER_ERASE:
	case TE_BRUSH_EDGE_ERASE:
	{
		_fStrength=-fStrength*32.0f;
		ApplyAddPaint(MIN_UWORD,MAX_UWORD);
		break;
	}
	case TE_BRUSH_LAYER_PAINT:
	{
		_fStrength=fStrength*32.0f;
		ApplyAddPaint(MIN_UWORD,MAX_UWORD);
		break;
	}
	case TE_BRUSH_SHADOWMAP_PAINT:			// yjpark |<--
	{
		_fStrength = fStrength * theApp.m_fPaintPower*0.05;
		ApplyShadowMapAddPaint( ptrTerrain->tr_ptrTerrain->tr_acolObjShadowColor[iBufferData], iBufferData );
		break;
	}
	
	//Su-won
	case TE_BRUSH_SHADOWMAP_SMOOTH:
	{
		_fStrength = -fStrength * theApp.m_fSmoothPower*0.005;
		ApplyShadowMapAddPaint( ptrTerrain->tr_ptrTerrain->tr_acolObjShadowColor[iBufferData], iBufferData, FALSE );
		break;
	}

	case TE_BRUSH_ATTRIBUTEMAP_PAINT:
	{
		_fStrength = fStrength * theApp.m_fPaintPower;
		ApplyAttributeMapAddPaint();
		break;
	}										// yjpark     -->|
	case TE_BRUSH_ALTITUDE_SMOOTH:
	case TE_BRUSH_LAYER_SMOOTH:
	case TE_ALTITUDE_SMOOTH:
	case TE_LAYER_SMOOTH:
	{
		_fStrength=fStrength*theApp.m_fSmoothPower;
		ApplyFilterMatrix(_afFilterBlurMore);
		break;
	}
	case TE_BRUSH_ALTITUDE_FILTER:
	case TE_BRUSH_LAYER_FILTER:
	case TE_LAYER_FILTER:
	case TE_ALTITUDE_FILTER:
	{
		_fStrength=fStrength*theApp.m_fFilterPower;
		switch(theApp.m_iFilter)
		{
			case FLT_FINEBLUR:    ApplyFilterMatrix(_afFilterFineBlur    ); break;
			case FLT_SHARPEN:     ApplyFilterMatrix(_afFilterSharpen     ); break;
			case FLT_EMBOSS:      ApplyFilterMatrix(_afFilterEmboss      ); break;
			case FLT_EDGEDETECT:  ApplyFilterMatrix(_afFilterEdgeDetect  ); break;
		}
		break;
	}
	case TE_BRUSH_ALTITUDE_MINIMUM:
	case TE_ALTITUDE_MINIMUM:
	{
		_fStrength=0;
		ApplyAddPaint(theApp.m_uwEditAltitude,MAX_UWORD);
		break;
	}
	case TE_BRUSH_ALTITUDE_MAXIMUM:
	case TE_ALTITUDE_MAXIMUM:
	{
		_fStrength=0;
		ApplyAddPaint(MIN_UWORD, theApp.m_uwEditAltitude);
		break;
	}
	case TE_BRUSH_ALTITUDE_FLATTEN:
	case TE_ALTITUDE_FLATTEN:
	{
		_fStrength=0;
		ApplyAddPaint(theApp.m_uwEditAltitude, theApp.m_uwEditAltitude);
		break;
	}    
	case TE_BRUSH_ALTITUDE_POSTERIZE:
	case TE_ALTITUDE_POSTERIZE:
	{
		ApplyPosterize();
		break;
	}    
	case TE_BRUSH_ALTITUDE_RND_NOISE:
	case TE_BRUSH_LAYER_RND_NOISE:
	case TE_ALTITUDE_RND_NOISE:
	case TE_LAYER_RND_NOISE:
	{
		ApplyRNDNoise();
		break;
	}    
	case TE_BRUSH_ALTITUDE_CONTINOUS_NOISE:
	case TE_BRUSH_LAYER_CONTINOUS_NOISE:
	case TE_ALTITUDE_CONTINOUS_NOISE:
	case TE_LAYER_CONTINOUS_NOISE:
	{
		if(!SetupContinousNoiseTexture()) return;
		ApplyContinousNoise();
		FreeContinousNoiseTexture();
		break;
	}    
	case TE_GENERATE_TERRAIN:
	{
		GenerateTerrain();
		break;
	}
	case TE_ALTITUDE_EQUALIZE:
	{
		EqualizeBuffer();
		break;
	}
	case TE_CLEAR_LAYER_MASK:
	{
		for(INDEX i=0; i<_rect.GetWidth()*_rect.GetHeight(); i++)
		{
			_puwBuffer[i]=0;
		}
		break;
	}
	case TE_FILL_LAYER_MASK:
	{
		for(INDEX i=0; i<_rect.GetWidth()*_rect.GetHeight(); i++)
		{
			_puwBuffer[i]=MAX_UWORD;
		}
		break;
	}
	case TE_TILE_PAINT:
	{
		TilePaintTool();
		break;
	}
	}

	// apply buffer change
	if( btBufferType == BT_SHADOW_MAP )															// yjpark |<--
	{
		TRE_Set4ByteBufferForEditing( ptrTerrain, _pulBuffer, _rect, btBufferType, iBufferData );
		TRE_Free4ByteBufferForEditing( _pulBuffer );
	}
	else if( btBufferType == BT_ATTRIBUTE_MAP )
	{
		TRE_Set1ByteBufferForEditing( ptrTerrain, _pubBuffer, _rect, btBufferType );
		TRE_Free1ByteBufferForEditing( _pubBuffer );
	}
	else
	{
		TRE_SetBufferForEditing( ptrTerrain, _puwBuffer, _rect, btBufferType, iBufferData );
		TRE_FreeBufferForEditing( _puwBuffer );
	}																							// yjpark     -->|
	theApp.GetActiveDocument()->SetModifiedFlag( TRUE);

	// mark rect for layer distribution updating
	if(teTool!=TE_TILE_PAINT)
	{
		DiscardLayerDistribution(_rect);
	}

	theApp.m_ctTerrainPageCanvas.MarkChanged();

	if(bAutoRememberUndo)
	{
		TerrainEditEnd(rectTerrain.rc_slRight - rectTerrain.rc_slLeft);
	}
}
