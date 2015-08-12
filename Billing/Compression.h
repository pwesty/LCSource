#ifdef COMPRESSION_MESSAGE

#ifndef __ACOMPRESSION_H__
#define __ACOMPRESSION_H__

/*
 * Abstract base class for objects that can compress memory blocks.
 */
class CCompressor {
public:
  /* Calculate needed size for destination buffer when packing memory with given compression. */
  virtual int NeededDestinationSize(int slSourceSize) = 0;

  // on entry, slDstSize holds maximum size of output buffer,
  // on exit, it is filled with resulting size
  /* Pack a chunk of data using given compression. */
  virtual bool   Pack(const void *pvSrc, int slSrcSize, void *pvDst, int &slDstSize) = 0;
  /* Unpack a chunk of data using given compression. */
  virtual bool Unpack(const void *pvSrc, int slSrcSize, void *pvDst, int &slDstSize) = 0;

};

/*
 * Compressor for compressing memory blocks using RLE BYTE-BYTE compression
 */
class CRLEBBCompressor : public CCompressor {
public:
  /* Calculate needed size for destination buffer when packing memory. */
  int NeededDestinationSize(int slSourceSize);

  // on entry, slDstSize holds maximum size of output buffer,
  // on exit, it is filled with resulting size
  /* Pack a chunk of data using given compression. */
  bool   Pack(const void *pvSrc, int slSrcSize, void *pvDst, int &slDstSize);
  /* Unpack a chunk of data using given compression. */
  bool Unpack(const void *pvSrc, int slSrcSize, void *pvDst, int &slDstSize);
};


/*
 * Compressor for compressing memory blocks using LZ compression
 * (uses LZRW1 - a modification by Ross Williams)
 */
class CLZCompressor : public CCompressor {
public:
  /* Calculate needed size for destination buffer when packing memory. */
  int NeededDestinationSize(int slSourceSize);

  // on entry, slDstSize holds maximum size of output buffer,
  // on exit, it is filled with resulting size
  /* Pack a chunk of data using given compression. */
  bool   Pack(const void *pvSrc, int slSrcSize, void *pvDst, int &slDstSize);
  /* Unpack a chunk of data using given compression. */
  bool Unpack(const void *pvSrc, int slSrcSize, void *pvDst, int &slDstSize);
};

/*
 * Compressor for compressing memory blocks using zlib compression
 * (zlib uses LZ77 - algorithm)
 */
class CzlibCompressor : public CCompressor {
public:
  /* Calculate needed size for destination buffer when packing memory. */
  int NeededDestinationSize(int slSourceSize);

  // on entry, slDstSize holds maximum size of output buffer,
  // on exit, it is filled with resulting size
  /* Pack a chunk of data using given compression. */
  bool   Pack(const void *pvSrc, int slSrcSize, void *pvDst, int &slDstSize);
  /* Unpack a chunk of data using given compression. */
  bool Unpack(const void *pvSrc, int slSrcSize, void *pvDst, int &slDstSize);
};

#endif

#endif // #ifdef COMPRESSION_MESSAGE