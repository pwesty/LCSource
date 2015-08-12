#ifndef SE_INCL_COMPRESSION_H
#define SE_INCL_COMPRESSION_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

/*
 * Abstract base class for objects that can compress memory blocks.
 */
class CCompressor {
public:
  /* Calculate needed size for destination buffer when packing memory with given compression. */
  virtual SLONG NeededDestinationSize(SLONG slSourceSize) = 0;

  // on entry, slDstSize holds maximum size of output buffer,
  // on exit, it is filled with resulting size
  /* Pack a chunk of data using given compression. */
  virtual BOOL   Pack(const void *pvSrc, SLONG slSrcSize, void *pvDst, SLONG &slDstSize) = 0;
  /* Unpack a chunk of data using given compression. */
  virtual BOOL Unpack(const void *pvSrc, SLONG slSrcSize, void *pvDst, SLONG &slDstSize) = 0;

  /* Pack/unpack from stream to stream. */
  void UnpackStream_t(CTStream &strmSrc, CTStream &strmDst); // throw char *
  void PackStream_t(CTStream &strmSrc, CTStream &strmDst); // throw char *
};

/*
 * Compressor for compressing memory blocks using RLE BYTE-BYTE compression
 */
class CRLEBBCompressor : public CCompressor {
public:
  /* Calculate needed size for destination buffer when packing memory. */
  SLONG NeededDestinationSize(SLONG slSourceSize);

  // on entry, slDstSize holds maximum size of output buffer,
  // on exit, it is filled with resulting size
  /* Pack a chunk of data using given compression. */
  BOOL   Pack(const void *pvSrc, SLONG slSrcSize, void *pvDst, SLONG &slDstSize);
  /* Unpack a chunk of data using given compression. */
  BOOL Unpack(const void *pvSrc, SLONG slSrcSize, void *pvDst, SLONG &slDstSize);
};


/*
 * Compressor for compressing memory blocks using LZ compression
 * (uses LZRW1 - a modification by Ross Williams)
 */
class CLZCompressor : public CCompressor {
public:
  /* Calculate needed size for destination buffer when packing memory. */
  SLONG NeededDestinationSize(SLONG slSourceSize);

  // on entry, slDstSize holds maximum size of output buffer,
  // on exit, it is filled with resulting size
  /* Pack a chunk of data using given compression. */
  BOOL   Pack(const void *pvSrc, SLONG slSrcSize, void *pvDst, SLONG &slDstSize);
  /* Unpack a chunk of data using given compression. */
  BOOL Unpack(const void *pvSrc, SLONG slSrcSize, void *pvDst, SLONG &slDstSize);
};

/*
 * Compressor for compressing memory blocks using zlib compression
 * (zlib uses LZ77 - algorithm)
 */
class CzlibCompressor : public CCompressor {
public:
  /* Calculate needed size for destination buffer when packing memory. */
  SLONG NeededDestinationSize(SLONG slSourceSize);

  // on entry, slDstSize holds maximum size of output buffer,
  // on exit, it is filled with resulting size
  /* Pack a chunk of data using given compression. */
  BOOL   Pack(const void *pvSrc, SLONG slSrcSize, void *pvDst, SLONG &slDstSize);
  /* Unpack a chunk of data using given compression. */
  BOOL Unpack(const void *pvSrc, SLONG slSrcSize, void *pvDst, SLONG &slDstSize);
};



ENGINE_API SWORD PackFloatToWord(FLOAT fUnpacked);
ENGINE_API FLOAT UnpackFloatFromWord(SWORD swPacked);
ENGINE_API ULONG PackFloat(UBYTE ubExponent,UBYTE ubMantissa,UBYTE ubBias,FLOAT fUnpacked);
ENGINE_API FLOAT UnpackFloat(UBYTE ubExponent,UBYTE ubMantissa,UBYTE ubBias,ULONG ulPacked);
ENGINE_API ULONG PackVectorToULONG(FLOAT3D &vPosition);
ENGINE_API void  UnpackVectorFromULONG(ULONG ulPacked,FLOAT3D &vPosition);
ENGINE_API ULONG PackFloatsToULONG(FLOAT f1,FLOAT f2,FLOAT f3);
ENGINE_API void  UnpackFloatsFromULONG(ULONG ulPacked,FLOAT &f1,FLOAT &f2,FLOAT &f3);
ENGINE_API ULONG PackAngle(const ANGLE3D &aAngle3D);
ENGINE_API void  UnpackAngle(ULONG ulPackedAngle,ANGLE3D &aAngle3D);


#endif  /* include-once check. */

