#include "stdhdrs.h"

#ifdef COMPRESSION_MESSAGE

#include "Compression.h"

#include <zlib.h>

#define MAX_SLONG ((int)0x7FFFFFFFL)
#define MAX_SWORD ((short int)0x7FFF)
#define MAX_SBYTE ((char)0x7F)

#define MIN_SLONG ((int)0x80000000L)
#define MIN_SWORD ((short int)0x8000)
#define MIN_SBYTE ((char)0x80)


/*
 * Calculate needed size for destination buffer when packing memory.
 */
int CRLEBBCompressor::NeededDestinationSize(int slSourceSize)
{
  // calculate worst case possible for size of RLEBB packed data
  // *129/128+1 would be enough, but we add some more to ensure that we don't
  // overwrite the temporary buffer
   return slSourceSize*129/128 + 5;
}

// on entry, slDstSize holds maximum size of output buffer,
// on exit, it is filled with resulting size
/* Pack a chunk of data using given compression. */
bool CRLEBBCompressor::Pack(const void *pvSrc, int slSrcSize, void *pvDst, int &slDstSize)
{
  // cannot pack zero bytes
  //ASSERT(slSrcSize>=1);

  // calculate limits for source and destination buffers
  const char *pbSourceFirst = (const char *)pvSrc;            // start marker
  const char *pbSourceLimit = (const char *)pvSrc+slSrcSize;   // end marker

//  SLONG slDestinationSize=NeededDestinationSize(slSrcSize);
  char *pbDestinationFirst = (char *)pvDst;            // start marker
  char *pbDestinationLimit = (char *)pvDst+slDstSize;  // end marker

  char *pbCountFirst = (char *)pbDestinationLimit-slSrcSize; // start marker
  char *pbCountLimit = (char *)pbDestinationLimit;           // end marker

  {
    /* PASS 1: Use destination buffer to cache number of forward-same bytes. */

    // set the count of the last byte to one
    char *pbCount = pbCountLimit-1;
    *pbCount-- = 1;

    // for all bytes from one before last to the first one
    for(const char *pbSource = pbSourceLimit-2;
        pbSource>=pbSourceFirst;
        pbSource--, pbCount--) {
      // if the byte is same as its successor, and the count will fit in code
      if (pbSource[0]==pbSource[1] && (int)pbCount[1]+1<=-(int)MIN_SBYTE) {
        // set its count to the count of its successor plus one
        pbCount[0] = pbCount[1]+1;
      // if the byte is different than its successor
      } else {
        // set its count to one
        pbCount[0] = 1;
      }
    }
  }


  /* PASS 2: Pack bytes from source to the destination buffer. */

  // start at the beginning of the buffers
  const char *pbSource      = pbSourceFirst;
  const char *pbCount       = pbCountFirst;
  char       *pbDestination = pbDestinationFirst;

  // while there is some data to pack
  while(pbSource<pbSourceLimit) {
//    ASSERT(pbCount<pbCountLimit);

    // if current byte is replicated
    if (*pbCount>1) {
      // write the replicate-packed data
      int ctSameBytes = (int)*pbCount;
      int slCode = -ctSameBytes+1;
//      ASSERT((SLONG)MIN_SBYTE<=slCode && slCode<0);
      *pbDestination++ = (char)slCode;
      *pbDestination++ = pbSource[0];
      pbSource+=ctSameBytes;
      pbCount +=ctSameBytes;
    // if current byte is not replicated
    } else {
      // count bytes to copy before encountering byte replicated more than 3 times
      int ctDiffBytes=1;
      while( (ctDiffBytes < (int)MAX_SBYTE + 1)
          && (&pbSource[ctDiffBytes]<pbSourceLimit) ) {
        if ((int)pbCount[ctDiffBytes-1]<=3) {
          ctDiffBytes++;
        } else {
          break;
        }
      }
      // write the copy-packed data
      int slCode = ctDiffBytes-1;
//      ASSERT(0<=slCode && slCode<=(int)MAX_SBYTE);
      *pbDestination++ = (char)slCode;
      memcpy(pbDestination, pbSource, ctDiffBytes);
      pbSource      += ctDiffBytes;
      pbCount       += ctDiffBytes;
      pbDestination += ctDiffBytes;
    }
  }
  // packing must exactly be finished now
//  ASSERT(pbSource==pbSourceLimit);
//  ASSERT(pbCount ==pbCountLimit);

  // calculate size of packed data
  slDstSize = pbDestination-pbDestinationFirst;
  return TRUE;
}

// on entry, slDstSize holds maximum size of output buffer,
// on exit, it is filled with resulting size
/* Unpack a chunk of data using given compression. */
bool CRLEBBCompressor::Unpack(const void *pvSrc, int slSrcSize, void *pvDst, int &slDstSize)
{
  const char *pbSource      = (const char *)pvSrc;            // current pointer
  const char *pbSourceLimit = (const char *)pvSrc+slSrcSize;  // end marker

  char *pbDestination      = (char *)pvDst;  // current pointer
  char *pbDestinationFirst = (char *)pvDst;  // start marker

  // repeat
  do {
    // get code
    int slCode = *pbSource++;
    // if it is replication
    if (slCode<0) {
      // get next byte and replicate it given number of times
      int ctSameBytes = -slCode+1;
      memset(pbDestination, *pbSource++, ctSameBytes);
      pbDestination += ctSameBytes;
    // if it is copying
    } else {
      // copy given number of next bytes
      int ctCopyBytes = slCode+1;
      memcpy(pbDestination, pbSource, ctCopyBytes);
      pbSource      += ctCopyBytes;
      pbDestination += ctCopyBytes;
    }
  // until all data is unpacked
  } while (pbSource<pbSourceLimit);

  // data must be unpacked correctly
//  ASSERT(pbSource==pbSourceLimit);

  // calculate size of data that was unpacked
  slDstSize = pbDestination-pbDestinationFirst;
  return TRUE;
}

/////////////////////////////////////////////////////////////////////
// LZRW1 compressor
// uses algorithm by Ross Williams

//#define TRUE 1

//#define UBYTE unsigned char /* Unsigned     byte (1 byte )        */
//#define UWORD unsigned int  /* Unsigned     word (2 bytes)        */
//#define ULONG unsigned long /* Unsigned longword (4 bytes)        */
#define FLAG_BYTES    1     /* Number of bytes used by copy flag. */
#define FLAG_COMPRESS 0     /* Signals that compression occurred. */
#define FLAG_COPY     1     /* Signals that a copyover occurred.  */
//void fast_copy(p_src,p_dst,len) /* Fast copy routine.             */
//UBYTE *p_src,*p_dst; {while (len--) *p_dst++=*p_src++;}
inline void fast_copy(const char *p_src, char *p_dst, int len)
{
  memcpy(p_dst, p_src, len);
}

/******************************************************************************/

void lzrw1_compress(const char *p_src_first, int src_len,char *p_dst_first, int *p_dst_len)
/* Input  : Specify input block using p_src_first and src_len.          */
/* Input  : Point p_dst_first to the start of the output zone (OZ).     */
/* Input  : Point p_dst_len to a ULONG to receive the output length.    */
/* Input  : Input block and output zone must not overlap.               */
/* Output : Length of output block written to *p_dst_len.               */
/* Output : Output block in Mem[p_dst_first..p_dst_first+*p_dst_len-1]. */
/* Output : May write in OZ=Mem[p_dst_first..p_dst_first+src_len+256-1].*/
/* Output : Upon completion guaranteed *p_dst_len<=src_len+FLAG_BYTES.  */
#define PS *p++!=*s++  /* Body of inner unrolled matching loop.         */
#define ITEMMAX 16     /* Maximum number of bytes in an expanded item.  */
{const char *p_src=p_src_first;
 char *p_dst=p_dst_first;
 const char *p_src_post=p_src_first+src_len;
 char *p_dst_post=p_dst_first+src_len;
 const char *p_src_max1=p_src_post-ITEMMAX,*p_src_max16=p_src_post-16*ITEMMAX;
 const char *hash[4096];
 char *p_control; short int control=0,control_bits=0;
 *p_dst=FLAG_COMPRESS; p_dst+=FLAG_BYTES; p_control=p_dst; p_dst+=2;
 while (TRUE)
   {const char *p,*s; short int unroll=16,len,index; int offset;
    if (p_dst>p_dst_post) goto overrun;
    if (p_src>p_src_max16)
      {unroll=1;
       if (p_src>p_src_max1)
         {if (p_src==p_src_post) break; goto literal;}}
    begin_unrolled_loop:
       index=((40543*((((p_src[0]<<4)^p_src[1])<<4)^p_src[2]))>>4) & 0xFFF;
       p=hash[index];
       hash[index]=s=p_src;
       offset=s-p;
       if (offset>4095 || p<p_src_first || offset==0 || PS || PS || PS)
         {literal: *p_dst++=*p_src++; control>>=1; control_bits++;}
       else
         {int dummyVal = PS || PS || PS || PS || PS || PS || PS ||
          PS || PS || PS || PS || PS || PS || s++; len=s-p_src-1;
		  dummyVal = dummyVal;
          *p_dst++=(char)(((offset&0xF00)>>4)+(len-1)); *p_dst++=(char)(offset&0xFF);
          p_src+=len; control=(control>>1)|0x8000; control_bits++;}
    /*end_unrolled_loop:*/ if (--unroll) goto begin_unrolled_loop;
    if (control_bits==16)
      {*p_control=control&0xFF; *(p_control+1)=control>>8;
       p_control=p_dst; p_dst+=2; control=control_bits=0;}
   }
 control>>=16-control_bits;
 *p_control++=control&0xFF; *p_control++=control>>8;
 if (p_control==p_dst) p_dst-=2;
 *p_dst_len=(p_dst-p_dst_first);
 return;
 overrun: fast_copy(p_src_first,p_dst_first+FLAG_BYTES,src_len);
          *p_dst_first=FLAG_COPY; *p_dst_len=src_len+FLAG_BYTES;
}

/******************************************************************************/

void lzrw1_decompress(const char *p_src_first, int src_len, char *p_dst_first, int *p_dst_len)
/* Input  : Specify input block using p_src_first and src_len.          */
/* Input  : Point p_dst_first to the start of the output zone.          */
/* Input  : Point p_dst_len to a ULONG to receive the output length.    */
/* Input  : Input block and output zone must not overlap. User knows    */
/* Input  : upperbound on output block length from earlier compression. */
/* Input  : In any case, maximum expansion possible is eight times.     */
/* Output : Length of output block written to *p_dst_len.               */
/* Output : Output block in Mem[p_dst_first..p_dst_first+*p_dst_len-1]. */
/* Output : Writes only  in Mem[p_dst_first..p_dst_first+*p_dst_len-1]. */
{short int controlbits=0, control;
 const char *p_src=p_src_first+FLAG_BYTES;
 char *p_dst=p_dst_first;
 const char *p_src_post=p_src_first+src_len;
 if (*p_src_first==FLAG_COPY)
   {fast_copy(p_src_first+FLAG_BYTES,p_dst_first,src_len-FLAG_BYTES);
    *p_dst_len=src_len-FLAG_BYTES; return;}
 while (p_src!=p_src_post)
   {if (controlbits==0)
      {control=*p_src++; control|=(*p_src++)<<8; controlbits=16;}
    if (control&1)
      {short int offset,len; char *p;
       offset=(*p_src&0xF0)<<4; len=1+(*p_src++&0xF);
       offset+=*p_src++&0xFF; p=p_dst-offset;
       while (len--) *p_dst++=*p++;}
    else
       *p_dst++=*p_src++;
    control>>=1; controlbits--;
   }
 *p_dst_len=p_dst-p_dst_first;
}


/*
 * Calculate needed size for destination buffer when packing memory.
 */
int CLZCompressor::NeededDestinationSize(int slSourceSize)
{
  // calculate worst case possible for size of LZ packed data
  return slSourceSize+256;
}

// on entry, slDstSize holds maximum size of output buffer,
// on exit, it is filled with resulting size
/* Pack a chunk of data using given compression. */
bool CLZCompressor::Pack(const void *pvSrc, int slSrcSize, void *pvDst, int &slDstSize)
{
  // this is just wrapper for original function by Ross Williams
  int slDestinationSizeResult = slDstSize;
  lzrw1_compress(
    (const char *)pvSrc, (int)slSrcSize,
    (char *)pvDst, (int *)&slDestinationSizeResult);
  slDstSize = slDestinationSizeResult;
  return TRUE;
}

// on entry, slDstSize holds maximum size of output buffer,
// on exit, it is filled with resulting size
/* Unpack a chunk of data using given compression. */
bool CLZCompressor::Unpack(const void *pvSrc, int slSrcSize, void *pvDst, int &slDstSize)
{
  // this is just wrapper for original function by Ross Williams
  int slDestinationSizeResult = slDstSize;
  lzrw1_decompress(
    (const char *)pvSrc, (int)slSrcSize,
    (char *)pvDst, (int *)&slDestinationSizeResult);
  slDstSize = slDestinationSizeResult;
  return TRUE;
}

/* Calculate needed size for destination buffer when packing memory. */
int CzlibCompressor::NeededDestinationSize(int slSourceSize)
{
  // calculate worst case possible for size of zlib packed data
  // NOTE: zlib docs state 0.1% of uncompressed size + 12 bytes, 
  // we just want to be on the safe side
  return int(slSourceSize*1.1f)+32;
}

// on entry, slDstSize holds maximum size of output buffer,
// on exit, it is filled with resulting size
/* Pack a chunk of data using given compression. */
bool CzlibCompressor::Pack(const void *pvSrc, int slSrcSize, void *pvDst, int &slDstSize)
{
/*
int ZEXPORT compress (dest, destLen, source, sourceLen)
    Bytef *dest;
    uLongf *destLen;
    const Bytef *source;
    uLong sourceLen;
    */

//  CTSingleLock slZip(&zip_csLock, TRUE);
  int iResult = compress(
    (unsigned char *)pvDst, (unsigned long *)&slDstSize,
    (const unsigned char *)pvSrc, (unsigned long)slSrcSize);
  if (iResult==Z_OK) {
    return TRUE;
  } else {
    return FALSE;
  }
}

// on entry, slDstSize holds maximum size of output buffer,
// on exit, it is filled with resulting size
/* Unpack a chunk of data using given compression. */
bool CzlibCompressor::Unpack(const void *pvSrc, int slSrcSize, void *pvDst, int &slDstSize)
{
/*
int ZEXPORT uncompress (dest, destLen, source, sourceLen)
    Bytef *dest;
    uLongf *destLen;
    const Bytef *source;
    uLong sourceLen;
    */

//  CTSingleLock slZip(&zip_csLock, TRUE);
  int iResult = uncompress(
    (unsigned char *)pvDst, (unsigned long *)&slDstSize,
    (const unsigned char *)pvSrc, (unsigned long)slSrcSize);

  if (iResult==Z_OK) {
    return TRUE;
  } else {
    return FALSE;
  }
}

#endif