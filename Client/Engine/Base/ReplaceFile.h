#ifndef SE_INCL_REPLACEFILE_H
#define SE_INCL_REPLACEFILE_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

extern BOOL _bFileReplacingApplied;

BOOL GetReplacingFile(CTFileName fnSourceTexture, CTFileName &fnReplacingTexture,
                      char *pFilter);
void SetTextureWithPossibleReplacing_t(CTextureObject &to, CTFileName &fnmTexture);

// read/write a texture object
void ReadTextureObject_t(CTStream &strm, CTextureObject &to);
void SkipTextureObject_t(CTStream &strm);
void WriteTextureObject_t(CTStream &strm, CTextureObject &to);

// read/write a model and its texture(s) from a file
void ReadModelObject_t(CTStream &strm, CModelObject &mo);
void SkipModelObject_t(CTStream &strm);
void WriteModelObject_t(CTStream &strm, CModelObject &mo);

// read/write a ska model from a file
ENGINE_API extern void WriteModelInstance_t(CTStream &strm, CModelInstance &mi, BOOL bFromStock=TRUE);
ENGINE_API extern void ReadModelInstance_t(CTStream &strm, CModelInstance &mi, BOOL bMarkInStock=TRUE);
ENGINE_API extern void SkipModelInstance_t(CTStream &strm);


// read/write an anim object from a file
void ReadAnimObject_t(CTStream &strm, CAnimObject &mo);
void SkipAnimObject_t(CTStream &strm);
void WriteAnimObject_t(CTStream &strm, CAnimObject &mo);

// read/write a sound object from a file
void ReadSoundObject_t(CTStream &strm, CSoundObject &mo, BOOL bNetwork = FALSE);
void SkipSoundObject_t(CTStream &strm, BOOL bNetwork = FALSE);
void WriteSoundObject_t(CTStream &strm, CSoundObject &mo, BOOL bNetwork = FALSE);


#endif  /* include-once check. */

