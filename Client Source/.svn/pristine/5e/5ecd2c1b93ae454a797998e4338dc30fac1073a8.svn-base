#ifndef SE_INCL_TEXTURE_H
#define SE_INCL_TEXTURE_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Base/Lists.h>
#include <Engine/Base/Anim.h>
#include <Engine/Graphics/GfxLibrary.h>

#define BYTES_PER_TEXEL 4   // all textures in engine are 4 bytes per pixel

// td_ulFlags bits
// (on disk and in memory)
#define TEX_ALPHACHANNEL    (1UL<<0)   // texture has alpha channel (for old version support)
#define TEX_32BIT           (1UL<<1)   // texture needs to be in 32-bit quality uploaded if can
#define TEX_COMPRESSED      (1UL<<2)   // texture is stored on disk in compressed format
#define TEX_TRANSPARENT     (1UL<<3)   // only one bit of alpha channel is enough (internal format GL_RGB5_A1)
#define TEX_EQUALIZED       (1UL<<4)   // texture has 128-gray last mipmap (i.e. can be discarded in shade mode)
#define TEX_COMPRESSEDALPHA (1UL<<5)   // compressed texture is using interpoladed alpha (DXT5) rather than fixed one (DXT3)
// (only in memory)
#define TEX_STATIC          (1UL<<8)   // remain loaded after being bound (i.e. uploaded - for base textures)
#define TEX_CONSTANT        (1UL<<9)   // cannot be changed (no mip-map disposing, no LOD biasing, no colorizing, nothing!)
#define TEX_GRAY            (1UL<<10)  // grayscale texture
#define TEX_COMPRESS        (1UL<<16)  // this texture will need to be compressed when saved
#define TEX_COMPRESSALPHA   (1UL<<17)  // this texture will have alpha interpolated when compressed
#define TEX_SINGLEMIPMAP    (1UL<<18)  // set if last uploading was in single-mipmap
#define TEX_PROBED          (1UL<<19)  // set if last binding was as probe-texture
// (flags that shows if texture mipmaps has been changed)
#define TEX_DISPOSED        (1UL<<20)  // largest mip-map(s) has been left-out
#define TEX_DITHERED        (1UL<<21)  // dithering has been applied on this texture
#define TEX_FILTERED        (1UL<<22)  // flitering has been applied on this texture
#define TEX_COLORIZED       (1UL<<24)  // mipmaps has been colorized on this texture
#define TEX_WASOLD          (1UL<<30)  // loaded from old format (version 3)


/*
 * Bitmap data for a class of texture objects
 */
class ENGINE_API CTextureData : public CAnimData {
#ifdef KALYDO
  static CTString strDefaultTexturePath;
#endif
public:
// implementation:
  ULONG td_ulFlags;             // see defines
  MEX   td_mexWidth, td_mexHeight; // texture dimensions
  INDEX td_iFirstMipLevel;      // the highest quality mip level
  INDEX td_ctFineMipLevels;     // number of bilineary created mip levels
  SLONG td_slFrameSize;         // sum of sizes of all mip-maps for one frame
  INDEX td_ctFrames;            // number of different frames

  class CTexParams td_tpLocal;  // local texture parameters
  ULONG td_ulInternalFormat;    // format in which texture will be uploaded
  CTimerValue td_tvLastDrawn;   // timer for probing
  ULONG td_ulProbeObject;
  union {
    ULONG  td_ulObject;
    ULONG *td_pulObjects;
  };
  ULONG *td_pulFrames;          // all frames with their mip-maps and private palettes
  UBYTE *td_pubBuffer1, *td_pubBuffer2;       // buffers for effect textures
  PIX td_pixBufferWidth, td_pixBufferHeight;  // effect buffer dimensions
  class CTextureData *td_ptdBaseTexture;      // base texure for effects (if any)
  class CTextureEffectGlobal *td_ptegEffect;  // all data for effect textures

  INDEX td_iRenderFrame; // frame number currently rendering (for profiling)

  // constructor and destructor
	CTextureData();
	~CTextureData();

  // reference counting (override from CAnimData)
  void RemReference_internal(void);

  // converts global mip level to the corresponding one of texture
  INDEX ClampMipLevel( FLOAT fMipFactor) const;

  // gets values from some of texture data members
  inline MEX GetWidth(void)     const { return td_mexWidth;  };
  inline MEX GetHeight(void)    const { return td_mexHeight; };
  inline PIX GetPixWidth(void)  const { return td_mexWidth >>td_iFirstMipLevel; };
  inline PIX GetPixHeight(void) const { return td_mexHeight>>td_iFirstMipLevel; };
  inline ULONG GetFlags(void)   const { return td_ulFlags; };
  inline ULONG GetNoOfMips(void)     const { return GetNoOfMipmaps( GetPixWidth(), GetPixHeight()); };
  inline ULONG GetNoOfFineMips(void) const { return td_ctFineMipLevels; };

  // mark that texture has been used
  inline void MarkDrawn(void) { td_tvLastDrawn = _pTimer->GetHighPrecisionTimer(); };

  // get string description of texture size, mips and parameters
  CTString GetDescription(void);

  // sets new texture mex width and changes height remaining texture's aspect ratio
  inline void ChangeSize( MEX mexNewWidth) {
    td_mexHeight = MEX( ((FLOAT)mexNewWidth)/td_mexWidth * td_mexHeight);
    td_mexWidth  = mexNewWidth;
  };

  // check if texture frame(s) has been somehow altered (dithering, filtering, saturation, colorizing...)
  inline BOOL IsModified(void) {
    return td_ulFlags & (TEX_DISPOSED|TEX_DITHERED|TEX_FILTERED|TEX_COLORIZED);
  };
  
  // export finest mipmap of one texture's frame to imageinfo
  void Export_t( class CImageInfo &iiExportedImage, INDEX iFrame);

  // set texture frame as current for accelerator (this will upload texture that needs or wants uploading)
  void SetAsCurrent( INDEX iFrameNo=0, BOOL bForceUpload=FALSE);

  // creates new effect texture with one frame
  void CreateEffectTexture( PIX pixWidth, PIX pixHeight, MEX mexWidth,
                            CTextureData *ptdBaseTexture, ULONG ulGlobalEffect);
  // creates new texture with one frame (iType; 0=default, 1=forced 32-bit, 2=compressed, 3=compress alpha channel also)
  void Create_t( const CImageInfo *pII, MEX mexWanted, INDEX ctFineMips, ULONG ulFlags=NONE);
  // adds one frame to created texture
  void AddFrame_t( const CImageInfo *pII);

  // remove texture from gfx API (driver)
  void Unbind(void);
  // free memory allocated for texture
  void Clear(void);
#ifdef	KALYDO
  void Load_t(const CTFileName &fnFileName);
  void Load_Delay_t(const CTFileName &fnFileName);
  void Reload();
#else	// KALYDO
  void Reload();
#endif	// KALYDO

  // read texture from file
  void Read_t( CTStream *inFile);
  // read pnk
  void Read_PNG( CTStream* inFile );
  // write texture to file
  void Write_t( CTStream *outFile);
  // force texture to be re-loaded (if needed) in corresponding manner
  void Force( ULONG ulTexFlags);

  // get texel from texture's largest mip-map
  COLOR GetTexel( MEX mexU, MEX mexV);
  // copy (and eventually convert to floats) one row from texture to an array (iChannel is 1=R,2=G,3=B,4=A)
  void FetchRow( PIX pixRow, void *pfDst, INDEX iChannel=4, BOOL bConvertToFloat=TRUE);
  // get pointer to one row of texture
  ULONG *GetRowPointer( PIX pixRow);

  // compress texture (returns pointer to compressed frames)
  // for translucent textures, bCompressAlpha signals useage of interpolated alpha channel (DXT5) instead of fixed (DXT3)
  // NOTE: compressed texture frames have one SLONG at the begining that holds compressed size of one frame,
  //       and one SLONG at the begining of each mipmap that holds the size of that mipmap (everything in bytes!)
  UBYTE *Compress( SLONG &slCompressedFrameSize, BOOL bCompressAlpha);

  // upload texture from compressed format (returns FALSE if texture hasn't been uploaded!)
  BOOL UploadCompressed(void);

  // has texture been stored in compressed format
  inline BOOL IsCompressed(void);
  
// overridden from CSerial:

  // check if this kind of objects is auto-freed
  virtual BOOL IsAutoFreed(void);
  // get amount of memory used by this object
  virtual SLONG GetUsedMemory(void);
};


/*
 * An instance of a texture object
 */
class ENGINE_API CTextureObject : public CAnimObject {
// implementation:
public:
// interface:
public:
  CTextureObject(void);
  // copy from another object of same class
  void Copy(CTextureObject &toOther);
  MEX GetWidth(void) const;
  MEX GetHeight(void) const;
  ULONG GetFlags(void) const;
  void Read_t(  CTStream *istrFile); // throw char * //	read and
	void Write_t( CTStream *ostrFile); // throw char * //	write functions

  // obtain texture and set it for this object
  void SetData_t(const CTFileName &fnmTexture); // throw char *
  // get filename of texture or empty string if no texture
  const CTFileName &GetName(void);
};

ENGINE_API extern void CreateTexture_t( const CTFileName &inFileName,
                                        MEX inMex, INDEX inMipmaps, ULONG ulFlags=NONE);
ENGINE_API extern void CreateTexture_t( const CTFileName &inFileName, const CTFileName &outFileName,
                                        MEX inMex, INDEX inMipmaps, ULONG ulFlags=NONE);
ENGINE_API extern void ProcessScript_t( const CTFileName &inFileName);



/*
 * Render-to-texture class
 */

//안태훈 수정 시작	//(Add CRenderTexture class for Render to Texture)(0.1)
class ENGINE_API CRenderTexture
{
public:
	CRenderTexture();
	~CRenderTexture();

	// sehan D3DFORMAT fmt = D3DFMT_A8R8G8B8 추가 // sehan end
	BOOL Init(INDEX width, INDEX height, ULONG flag = TEX_32BIT, D3DFORMAT fmt = D3DFMT_A8R8G8B8);
	void Begin();	// SetRenderTarget current
	// Must be call this func in begin-end block
	void Clear(COLOR colClear=0xFFFFFFFF, FLOAT fZVal=ZBUF_BACK);
	void End();		// SetRenderTarget old

	//기본 정보들
	IDirect3DSurface8 *rt_pSurface;	// IDirect3DSurface8* in IDirect3DTexture8
	CTextureData rt_tdTexture;

protected:
	//begin, end block에서 사용되는 변수들, 외부로 들어나서 좋을거 없다.
	IDirect3DSurface8 *m_pOldRenderTarget;
	IDirect3DSurface8 *m_pOldDepthStencil;
	IDirect3DSurface8 *m_pDepthStencil;
	BOOL m_bOldZEnable;
};
//안태훈 수정 끝	//(Add CRenderTexture class for Render to Texture)(0.1)

/*
class ENGINE_API CRenderTexture
{
// implementation:
public:
  CListNode rt_lnInGfx          // for linking in list of all renderable textures
  ULONG rt_ulFlags;             // see defines (only alpha and 32bit, for now)
  PIX   tt_pixWidth, rt_pixHeight; // texture dimensions
  ULONG td_ulInternalFormat;    // format in which texture will be uploaded
  ULONG *rt_pulImage;           // image in memory (no mipmaps for now!)
  class CTexParams td_tpLocal;  // local texture parameters

// interface:
public:
  CRenderTexture(void);
  ~CRenderTexture(void);
  // prepare
  BOOL Init( PIX pixWidth, PIX pixHeight, BOOL b32bit, BOOL bAlpha=FALSE);
  // reset (i.e. prepare again - after display mode switch and stuff like that)
  void Reset(void);
  // set texture as current for accelerator
  void SetAsCurrent(void);
  // set texture as target for rendering
  void SetAsTarget(void);
};
*/

#endif  /* include-once check. */

