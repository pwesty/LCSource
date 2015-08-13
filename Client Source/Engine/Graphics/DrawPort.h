#ifndef SE_INCL_DRAWPORT_H
#define SE_INCL_DRAWPORT_H
#ifdef PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Base/Lists.h>


// public drawport's text flags
#define DPTF_BOLD   (1L<<0)
#define DPTF_ITALIC (1L<<1)
// internals
#define DPTF_PRINTSPECIALCODES  (1L<<10)
#define DPTF_IGNORESPECIALCODES (1L<<11)

struct UIRectUV;

class ENGINE_API CDrawPort {
// implementation:
public:
	class CRaster *dp_Raster;	     // pointer to the Raster this refers to
	class CFontData *dp_FontData;  // pointer to the current text font
	// drawport box relative to the raster's origin
	PIX   dp_MinI, dp_MinJ;
	PIX   dp_MaxI, dp_MaxJ;
	// drawport scissoring box relative to the raster's origin
	PIX   dp_ScissorMinI, dp_ScissorMinJ;
	PIX   dp_ScissorMaxI, dp_ScissorMaxJ;
	PIX	  dp_pixTextCharSpacing;	 // space between chars in text
	PIX	  dp_pixTextLineSpacing;	 // space between lines in text
	FLOAT dp_fTextScaling;				 // scaling factor for font size
	FLOAT dp_fTextAspect;				   // aspect ratio for font (x/y)
	FLOAT dp_fTextShadow;          // drop-shadow direction and offset (<0=up-left, >0=down-right, 0=none)
	ULONG dp_ulTextFlags;          // see DPTF_...
	ULONG dp_ulTextBlendingType;   // prredefinedBlendType (PBT_BLEND and such) - cannot use wrapper's enum here 
	FLOAT dp_fWideAdjustment;      // for wide (16:9 or 16:10) screen support (needed in some calculations) = 1.0f or >1 (10/12 perhaps)
	BOOL  dp_bRenderingOverlay;    // set when scene renderer requires overlay mode (don't clear z-buffer)

	CTextureData* stCurrentTextureData; //텍스트출력시 현재 텍스쳐를 저장

	// adjust this during frame to be used for screen blending
	ULONG dp_ulBlendingRA, dp_ulBlendingGA, dp_ulBlendingBA; // r*a, g*a, b*a
	ULONG dp_ulBlendingA;

	// (DEPRECATED!)
	void InitCloned(CDrawPort *pdpBase, double rMinI,double rMinJ, double rSizeI,double rSizeJ);
	BOOL Lock_internal(void);
	void Unlock_internal(void);

	// Recalculate pixel dimensions from relative dimensions and raster size
	void RecalculateDimensions(void);

	// set orthogonal projection
	void SetOrtho(void) const;
	// set given projection
	void SetProjection(CAnyProjection3D &apr);

//interface:
	// default constructor
	CDrawPort(void);
	// copy constructor
	CDrawPort(const CDrawPort *pdpOriginal);
	// assignment operation
	CDrawPort &operator=(CDrawPort &dpOriginal);
	// destructor
	~CDrawPort(void);

	// set physical rectangle in physical coordinates
	void SetPhysicalRectInPhysical(const PIXaabbox2D &rect);
	// set physical rectangle in logical coordinates
	void SetPhysicalRectInLogical(const PIXaabbox2D &rect);
	// set physical rectangle in raster coordinates (low-level function)
	void SetPhysicalRectInRaster(const PIXaabbox2D &rect);

	// set logical rectangle in logical coordinates
	void SetLogicalRectInLogical(const PIXaabbox2D &rect);
	// set logical rectangle in physical coordinates
	void SetLogicalRectInPhysical(const PIXaabbox2D &rect);
	// set logical rectangle in raster coordinates (low-level function)
	void SetLogicalRectInRaster(const PIXaabbox2D &rect);

	// create a smaller subdrawport
	CDrawPort CreateSubDrawPort(const PIXaabbox2D &rect);
	CDrawPort CreateSubDrawPortRatio(const FLOATaabbox2D &rect);

	// DEPRECATED!  --  old drawport cloning functions
	CDrawPort( CDrawPort *pdpBase, double rMinI,double rMinJ, double rSizeI,double rSizeJ);
	CDrawPort( CDrawPort *pdpBase, const PIXaabbox2D &box);
	CDrawPort( CDrawPort *pdpBase, const INDEX iMonitor);  // iMonitor: -1=left, 0=center, +1=right
	void MakeWideScreen(CDrawPort *pdp);

	// check if a drawport is dualhead, triplehead or widescreen
	BOOL IsDualHead(  void);
	BOOL IsTripleHead(void);
	BOOL IsWideScreen(void);

	// set/get rendering in overlay mode
	inline void SetOverlappedRendering(BOOL bOverlay) { dp_bRenderingOverlay = bOverlay; };
	inline BOOL IsOverlappedRendering(void) const     { return dp_bRenderingOverlay; }; 

	// returns unique drawport number
	ULONG GetID(void);

	// Get dimensions and location of drawport
	inline PIX GetWidth( void) const { return dp_MaxI-dp_MinI; };
	inline PIX GetHeight(void) const { return dp_MaxJ-dp_MinJ; };

	// text manipulation
	void SetFont( CFontData *pfd);   // WARNING: this resets text variables
	inline void SetTextCharSpacing( PIX pixSpacing) { dp_pixTextCharSpacing = pixSpacing; };
	inline void SetTextLineSpacing( PIX pixSpacing) { dp_pixTextLineSpacing = pixSpacing; };
	inline void SetTextScaling( FLOAT fScalingFactor) { dp_fTextScaling = fScalingFactor; };
	inline void SetTextAspect(  FLOAT fAspectRatio)   { dp_fTextAspect  = fAspectRatio; };
	inline void SetTextShadow( FLOAT fShadow) { dp_fTextShadow = fShadow; };
	inline void SetTextBlendingType( ULONG ulBlendingType) { dp_ulTextBlendingType = ulBlendingType; };
	inline void SetTextFlags( ULONG ulFlags) { dp_ulTextFlags = ulFlags; };

	// text output mode (-1 = print special codes, 0 = ignore special codes, 1 = parse special codes)
	inline void SetTextMode( INDEX iMode) { 
		dp_ulTextFlags &= ~(DPTF_PRINTSPECIALCODES|DPTF_IGNORESPECIALCODES);
				 if( iMode <0) dp_ulTextFlags |= DPTF_PRINTSPECIALCODES;
		else if( iMode==0) dp_ulTextFlags |= DPTF_IGNORESPECIALCODES;
	};

	// returns width of entire text string (with scale included)
	ULONG GetTextWidth(const char* strText);
	CTString GetTextWidth(const CTString &strText, INDEX nWidth) const;
	ULONG GetTextWidth2(const CTString &strText);
	 
	// writes text string on drawport (left-aligned)
	void PutText( const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend=0xFFFFFFFF) const;
	// writes text string on drawport (centered arround X)
	void PutTextC( const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend=0xFFFFFFFF);
	// writes text string on drawport (centered arround X and Y)
	void PutTextCXY( const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend=0xFFFFFFFF);
	// writes text string on drawport (right-aligned)
	void PutTextR( const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend=0xFFFFFFFF);

	// yjpark |<--
	// Writes text on drawport ( expansion version ) - internal
	void	PutTextExKor( const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend,
							FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const;
	void	PutTextExCht( const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend,
							FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const;
	void	PutTextExChs( const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend,
							FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const;
	void	PutTextExThai( const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend,
							FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const;
	void	PutTextExJap( const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend,	// wooss 051021
							FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const;
	void	PutTextExBrz( const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend, // eons 061102 
							FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const;
	void	PutTextExRus( const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend, // eons 061102 
							FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const;

	void	PutTextCharExKor( const char *pText, int nLength, PIX pixX0, PIX pixY0, const COLOR colBlend,
								FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const;
	void	PutTextCharExCht( const char *pText, int nLength, PIX pixX0, PIX pixY0, const COLOR colBlend,
								FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const;
	void	PutTextCharExChs( const char *pText, int nLength, PIX pixX0, PIX pixY0, const COLOR colBlend,
								FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const;
	void	PutTextCharExThai( const char *pText, int nLength, PIX pixX0, PIX pixY0, const COLOR colBlend,
								FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const;
	void	PutTextCharExJap( const char *pText, int nLength, PIX pixX0, PIX pixY0, const COLOR colBlend,
								FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const;
	void	PutTextCharExBrz( const char *pText, int nLength, PIX pixX0, PIX pixY0, const COLOR colBlend,
								FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const; // eons 061102
	void	PutTextCharExRus( const char *pText, int nLength, PIX pixX0, PIX pixY0, const COLOR colBlend,
								FLOAT fZ, BOOL bShadow, const COLOR colShadow ) const; // eons 061102

	// Writes text on drawport ( expansion version )
	void	PutTextEx( const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend = 0xF2F2F2FF,
						FLOAT fZ = 0, BOOL bShadow = FALSE, const COLOR colShadow = 0x181818B2 ) const;
	void	PutTextCharEx( const char *pText, int nLength, PIX pixX0, PIX pixY0, const COLOR colBlend = 0xF2F2F2FF,
							FLOAT fZ = 0, BOOL bShadow = FALSE, const COLOR colShadow = 0x181818B2 ) const;
	// Writes text string on drawport ( centered arround X, expansion version )
	void	PutTextExCX( const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend = 0xF2F2F2FF,
							FLOAT fZ = 0, BOOL bShadow = FALSE, const COLOR colShadow = 0x181818B2 );
	// Writes text string on drawport ( right-aligned, expansion version )
	void	PutTextExRX( const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend = 0xF2F2F2FF,
							FLOAT fZ = 0, BOOL bShadow = FALSE, const COLOR colShadow = 0x181818B2 );
	// Writes text string on drawport ( centered arround X, expansion version, expansion version )
	void	PutTextCharExCX( const char *pText, int nLength, PIX pixX0, PIX pixY0, const COLOR colBlend = 0xF2F2F2FF,
								FLOAT fZ = 0, BOOL bShadow = FALSE, const COLOR colShadow = 0x181818B2 ) const;
	// Writes text string on drawport ( right-aligned, expansion version, expansion version )
	void	PutTextCharExRX( const char *pText, int nLength, PIX pixX0, PIX pixY0, const COLOR colBlend = 0xF2F2F2FF,
								FLOAT fZ = 0, BOOL bShadow = FALSE, const COLOR colShadow = 0x181818B2 );
	// Flush rendering queue for text ( expansion versoin )
	void	EndTextEx( BOOL bDepthTest = FALSE );

	// Adds one part of button texture to rendering queue
	void AddBtnTexture( const int nTexID, const FLOAT fI0, const FLOAT fJ0, const FLOAT fI1, const FLOAT fJ1, 
						const FLOAT fU0, const FLOAT fV0, const FLOAT fU1, const FLOAT fV1,
						const COLOR col ) const;
	void AddBtnTexture( const int nTexID, const FLOAT fI0, const FLOAT fJ0, const FLOAT fI1, const FLOAT fJ1, 
						const UIRectUV uv, const COLOR col ) const;
	// Flush rendering queue for button
	void	FlushBtnRenderingQueue( int nBtnType, const ULONG ulPBT = 203 );
	// yjpark     -->|

	// plain texture display
	void PutTexture( class CTextureObject *pTO, const PIXaabbox2D &boxScreen,
									 const COLOR colBlend=0xFFFFFFFF, const ULONG ulPBT=203) const;
	void PutTexture( class CTextureObject *pTO, const PIXaabbox2D &boxScreen,
									 const COLOR colUL, const COLOR colUR, const COLOR colDL, const COLOR colDR,
									 const ULONG ulPBT=203) const;
	void PutTexture( class CTextureObject *pTO, const PIXaabbox2D &boxScreen,
									 const MEXaabbox2D &boxTexture, const COLOR colBlend=0xFFFFFFFF,
									 const ULONG ulPBT=203) const;
	void PutTexture( class CTextureObject *pTO, const PIXaabbox2D &boxScreen, const MEXaabbox2D &boxTexture,
									 const COLOR colUL, const COLOR colUR, const COLOR colDL, const COLOR colDR,
									 const ULONG ulPBT=203) const;

	// advanced texture display
	void InitTexture( class CTextureObject *pTO, const BOOL bClamp=FALSE, 
							const ULONG ulPBT=203, const BOOL bDepthTest = FALSE) const; // prepares texture and rendering arrays
	void InitTextureData( class CTextureData *pTD, const BOOL bClamp = FALSE, const ULONG ulPBT = 203,
							const BOOL bDepthTest = FALSE ) const;
	// adds one full texture to rendering queue
	void AddTexture( const FLOAT fI0, const FLOAT fJ0, const FLOAT fI1, const FLOAT fJ1, const COLOR col) const; 
	// adds one part of texture to rendering queue
	void AddTexture( const FLOAT fI0, const FLOAT fJ0, const FLOAT fI1, const FLOAT fJ1, 
							const FLOAT fU0, const FLOAT fV0, const FLOAT fU1, const FLOAT fV1,
							const COLOR col, FLOAT fZ = 0 ) const;
	void AddTexture( const FLOAT fI0, const FLOAT fJ0, const FLOAT fI1, const FLOAT fJ1, 
							const UIRectUV uv, const COLOR col, FLOAT fZ = 0 ) const;
	// adds one textured quad to rendering queue (up-left start, counter-clockwise)
	void AddTexture( const FLOAT fI0, const FLOAT fJ0, const FLOAT fU0, const FLOAT fV0, const COLOR col0,
							const FLOAT fI1, const FLOAT fJ1, const FLOAT fU1, const FLOAT fV1, const COLOR col1,
							const FLOAT fI2, const FLOAT fJ2, const FLOAT fU2, const FLOAT fV2, const COLOR col2,
							const FLOAT fI3, const FLOAT fJ3, const FLOAT fU3, const FLOAT fV3, const COLOR col3) const;
	// adds one textured quad to rendering queue (up-left start, counter-clockwise)
	void AddTexture( const FLOAT fI0, const FLOAT fJ0, const FLOAT fI1, const FLOAT fJ1,		// yjpark
							const FLOAT fU0, const FLOAT fV0, const FLOAT fU1, const FLOAT fV1,
							const FLOAT fU2, const FLOAT fV2, const FLOAT fU3, const FLOAT fV3, const COLOR col ) const;
	// adds one flat triangle rendering queue (up-left start, counter-clockwise)
	void AddTriangle( const FLOAT fI0, const FLOAT fJ0, const FLOAT fI1, const FLOAT fJ1,
							const FLOAT fI2, const FLOAT fJ2, const COLOR col, const float fz = 0) const; 
	// 사각형 버텍스 추가. left, top, right, bottom
	void AddQuadrangle( const FLOAT fI0, const FLOAT fJ0, const FLOAT fI1, const FLOAT fJ1, const COLOR col, const FLOAT fz = 0 ) const;

	// renders all textures from rendering queue and flushed rendering arrays
	void FlushRenderingQueue(void) const; 

	// (DEPRECATED!) lock and unlock raster thru drawport functions
	BOOL Lock(void) { return Lock_internal(); }
	void Unlock(void) { Unlock_internal(); }

	// set this drawport as current for rendering on it
	void SetAsCurrent(void);

	// draw point (can be several pixels - depends on radius)
	void DrawPoint( PIX pixI, PIX pixJ, COLOR col, PIX pixRadius=1) const;
	void DrawPoint3D( FLOAT3D v, COLOR col, FLOAT fRadius=1.0f) const; // in 3D
	// draw line
	void DrawLine( PIX pixI0, PIX pixJ0, PIX pixI1, PIX pixJ1, COLOR col, ULONG typ=_FULL_) const;
	void DrawLine3D( FLOAT3D v0, FLOAT3D v1, COLOR col) const; // in 3D
	// draw border
	void DrawBorder( PIX pixI, PIX pixJ, PIX pixWidth, PIX pixHeight, COLOR col, ULONG typ=_FULL_) const;

	// fill with blending part of a drawport with a given color
	void Fill( PIX pixI, PIX pixJ, PIX pixWidth, PIX pixHeight, COLOR col) const;
	// fill with blending part of a drawport with a four corner colors
	void Fill( PIX pixI, PIX pixJ, PIX pixWidth, PIX pixHeight,
						 COLOR colUL, COLOR colUR, COLOR colDL, COLOR colDR) const;
	// fill an part of Z-Buffer with a given value
	void FillZBuffer( PIX pixI, PIX pixJ, PIX pixWidth, PIX pixHeight, FLOAT zval) const;

	// fill without blending an entire drawport with a given color
	void Fill( COLOR col) const;
	// fill an entire Z-Buffer with a given value
	void FillZBuffer( FLOAT zval) const;

	// grab screen (iGrabZBuffer: 0=no, 1=if allowed, 2=yes)
	void GrabScreen( class CImageInfo &iiGrabbedImage, INDEX iGrabZBuffer=0) const;
	// functions for getting depth of points on drawport
//강동민 수정 시작 Water 구현		04.22
	//BOOL IsPointVisible(PIX pixI, PIX pixJ, FLOAT fOoK, INDEX iID, INDEX iMirrorLevel=0) const;		// 원본.
	BOOL IsPointVisible(CAnyProjection3D &apr, PIX pixI, PIX pixJ, FLOAT fOoK, INDEX iID, INDEX iMirrorLevel=0) const;
//강동민 수정 끝 Water 구현			04.22
	// render one lens flare
	void RenderLensFlare( CTextureObject *pto, FLOAT fI, FLOAT fJ,
												FLOAT fSizeI, FLOAT fSizeJ, ANGLE aRotation, COLOR colLight) const;

	// blend entire drawport with accumulated colors
	void BlendScreen(void);
	
	int CheckShowCharLength(char* strString, int nWidth);
	int GetTextSectionWidth(char* strString, int nSectionEnd, BOOL bIsPassWordBox);

	void StockCurrentTextureData(CTextureData* cT) { stCurrentTextureData = cT; };

	void PutTextRus(const CTString &strText, PIX pixX0, PIX pixY0, const COLOR colBlend);
};



void  ENGINE_API Particle_PrepareSystem( CDrawPort *pdpDrawPort, CAnyProjection3D &prProjection);
void  ENGINE_API Particle_EndSystem( BOOL bRestoreOrtho=TRUE);
void  ENGINE_API Particle_PrepareEntity( const FLOAT fMipFactor, const BOOL bHasFog, const BOOL bHasHaze,
																				 const BOOL bSkipClipping, CEntity *penViewer);
FLOAT ENGINE_API Particle_GetMipFactor(void);
INDEX ENGINE_API Particle_GetDrawPortID(void);
ENGINE_API CEntity *Particle_GetViewer(void);
ENGINE_API CProjection3D *Particle_GetProjection(void);
void ENGINE_API Particle_PrepareTexture( CTextureObject *pto, const enum PredefinedBlendType pbt);
void ENGINE_API Particle_SetTexturePart( const MEX mexWidth, const MEX mexHeight, const INDEX iCol, const INDEX iRow);
void ENGINE_API Particle_SetTextureSection( const PIX pixLeft, const PIX pixTop, const PIX pixRight, const PIX pixBottom);
void ENGINE_API Particle_RenderSquare( const FLOAT3D &vPos, FLOAT fSize, ANGLE aRotation, COLOR col, FLOAT fYRatio=1.0f);
void ENGINE_API Particle_RenderQuad3D( const FLOAT3D &vPos0, const FLOAT3D &vPos1, const FLOAT3D &vPos2,
																			 const FLOAT3D &vPos3, COLOR col);
void ENGINE_API Particle_RenderLine( const FLOAT3D &vPos0, const FLOAT3D &vPos1, FLOAT fWidth, COLOR col);
void ENGINE_API Particle_Sort( BOOL b3D=FALSE);
void ENGINE_API Particle_Flush(void);

int ENGINE_API FindVowel(const char*);
int ENGINE_API FindThaiLen(const char*,int fromText=0,int toText=-1);

#endif  /* include-once check. */

