/*
 * il_crypt.h - Implementation of useful cryptographic algorithms.
 *
 * Copyright (C) 2001  Southern Storm Software, Pty Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef	_IL_CRYPT_H
#define	_IL_CRYPT_H

#include "il_values.h"

#ifdef	__cplusplus
extern	"C" {
#endif

/*
 * The size of SHA1 hash values.
 */
#define	IL_SHA_HASH_SIZE		20

/*
 * Context block for SHA1.
 */
typedef struct _tagILSHAContext
{
	unsigned char	input[64];
	ILUInt32		inputLen;
	ILUInt32		A, B, C, D, E;
	ILUInt64		totalLen;

} ILSHAContext;

/*
 * Initialize a SHA1 context block.
 */
void ILSHAInit(ILSHAContext *sha);

/*
 * Input more data into a SHA1 context block.
 */
void ILSHAData(ILSHAContext *sha, const void *buffer, unsigned long len);

/*
 * Finalize a SHA1 context block and output the hash.
 */
void ILSHAFinalize(ILSHAContext *sha, unsigned char hash[IL_SHA_HASH_SIZE]);

/*
 * The size of MD5 hash values.
 */
#define	IL_MD5_HASH_SIZE		16

/*
 * Context block for MD5.
 */
typedef struct _tagILMD5Context
{
	unsigned char	input[64];
	ILUInt32		inputLen;
	ILUInt32		A, B, C, D;
	ILUInt64		totalLen;

} ILMD5Context;

/*
 * Initialize a MD5 context block.
 */
void ILMD5Init(ILMD5Context *md5);

/*
 * Input more data into a MD5 context block.
 */
void ILMD5Data(ILMD5Context *md5, const void *buffer, unsigned long len);

/*
 * Finalize a MD5 context block and output the hash.
 */
void ILMD5Finalize(ILMD5Context *md5, unsigned char hash[IL_MD5_HASH_SIZE]);

/*
 * The size of SHA-256 hash values.
 */
#define	IL_SHA256_HASH_SIZE		32

#define IL_SHA256_HASH_STRING_SIZE	65

/*
 * Context block for SHA-256.
 */
typedef struct _tagILSHA256Context
{
	unsigned char	input[64];
	ILUInt32		inputLen;
	ILUInt32		A, B, C, D, E, F, G, H;
	ILUInt64		totalLen;

} ILSHA256Context;

/*
 * Initialize a SHA-256 context block.
 */
void ILSHA256Init(ILSHA256Context *sha);

/*
 * Input more data into a SHA-256 context block.
 */
void ILSHA256Data(ILSHA256Context *sha, const void *buffer, unsigned long len);

/*
 * Finalize a SHA-256 context block and output the hash.
 */
void ILSHA256Finalize(ILSHA256Context *sha,
					  unsigned char hash[IL_SHA256_HASH_SIZE]);

/*
 * The size of SHA-512 hash values.
 */
#define	IL_SHA512_HASH_SIZE		64

/*
 * Context block for SHA-512.
 */
typedef struct _tagILSHA512Context
{
	unsigned char	input[128];
	ILUInt32		inputLen;
	ILUInt64		A, B, C, D, E, F, G, H;
	ILUInt64		totalLen;

} ILSHA512Context;

/*
 * Initialize a SHA-512 context block.
 */
void ILSHA512Init(ILSHA512Context *sha);

/*
 * Input more data into a SHA-512 context block.
 */
void ILSHA512Data(ILSHA512Context *sha, const void *buffer, unsigned long len);

/*
 * Finalize a SHA-512 context block and output the hash.
 */
void ILSHA512Finalize(ILSHA512Context *sha,
					  unsigned char hash[IL_SHA512_HASH_SIZE]);

/*
 * The size of SHA-384 hash values.
 */
#define	IL_SHA384_HASH_SIZE		48

/*
 * Context block for SHA-384, which is the same as SHA-512.
 */
typedef ILSHA512Context ILSHA384Context;

/*
 * Initialize a SHA-384 context block.
 */
void ILSHA384Init(ILSHA384Context *sha);

/*
 * Input more data into a SHA-384 context block.
 */
#define	ILSHA384Data(sha,buffer,len)	(ILSHA512Data((sha), (buffer), (len)))

/*
 * Finalize a SHA-384 context block and output the hash.
 */
void ILSHA384Finalize(ILSHA384Context *sha,
					  unsigned char hash[IL_SHA384_HASH_SIZE]);

/*
 * The size of RIPEMD160 hash values.
 */
#define	IL_RIPEMD160_HASH_SIZE		20

/*
 * Context block for RIPEMD160.
 */
typedef struct _tagILRIPEMD160Context
{
	unsigned char	input[64];
	ILUInt32		inputLen;
	ILUInt32		A, B, C, D, E;
	ILUInt64		totalLen;

} ILRIPEMD160Context;

/*
 * Initialize a RIPEMD1601 context block.
 */
void ILRIPEMD160Init(ILRIPEMD160Context *ripem);

/*
 * Input more data into a RIPEMD1601 context block.
 */
void ILRIPEMD160Data(ILRIPEMD160Context *ripem,
					 const void *buffer, unsigned long len);

/*
 * Finalize a RIPEMD1601 context block and output the hash.
 */
void ILRIPEMD160Finalize(ILRIPEMD160Context *ripem,
						 unsigned char hash[IL_RIPEMD160_HASH_SIZE]);

/*
 * Define the structure of a DES encryption context.
 */
typedef struct _tagILDESContext
{
	ILUInt32	ks[32];

} ILDESContext;

/*
 * Define the structure of a Triple-DES encryption context.
 */
typedef struct _tagILDES3Context
{
	ILDESContext k1;
	ILDESContext k2;
	ILDESContext k3;

} ILDES3Context;

/*
 * Initialize a DES encryption context.
 */
void ILDESInit(ILDESContext *des, unsigned char *key, int decrypt);

/*
 * Process a single 64-bit block using DES.  The input and
 * output buffers can overlap.
 */
void ILDESProcess(ILDESContext *des, unsigned char *input,
				  unsigned char *output);

/*
 * Finalize a DES encryption context, clearing all sensitive values.
 */
void ILDESFinalize(ILDESContext *des);

/*
 * Initialize a Triple-DES encryption context.
 */
void ILDES3Init(ILDES3Context *des3, unsigned char *key,
				int keyBits, int decrypt);

/*
 * Process a single 64-bit block using Triple-DES.  The input and
 * output buffers can overlap.
 */
void ILDES3Process(ILDES3Context *des3, unsigned char *input,
				   unsigned char *output);

/*
 * Finalize a Triple-DES encryption context, clearing all sensitive values.
 */
void ILDES3Finalize(ILDES3Context *des3);

/*
 * Determine if a DES key is weak.
 */
int ILDESIsWeakKey(unsigned char *key);

/*
 * Determine if a DES key is semi-weak.
 */
int ILDESIsSemiWeakKey(unsigned char *key);

/*
 * Structure of an AES cipher context object.
 */
typedef struct
{
	int			numRounds;
	ILInt32		keySchedule[15 * 4];

} ILAESContext;

/*
 * Initialize an AES encryption context.
 */
void ILAESInit(ILAESContext *aes, unsigned char *key, int keyBits);

/*
 * Encrypt a single 128-bit block using AES.  The input and
 * output buffers can overlap.
 */
void ILAESEncrypt(ILAESContext *aes, unsigned char *input,
				  unsigned char *output);

/*
 * Decrypt a single 128-bit block using AES.  The input and
 * output buffers can overlap.
 */
void ILAESDecrypt(ILAESContext *aes, unsigned char *input,
				  unsigned char *output);

/*
 * Finalize an AES encryption context, clearing all sensitive values.
 */
void ILAESFinalize(ILAESContext *aes);

/*
 * Structure of an RC2 encryption context.
 */
typedef struct _tagILRC2Context
{
	ILUInt16	key[64];

} ILRC2Context;

/*
 * Initialize an RC2 encryption context.
 */
void ILRC2Init(ILRC2Context *rc2, unsigned char *key, int keyBits);

/*
 * Encrypt a single 64-bit block using RC2.  The input and
 * output buffers can overlap.
 */
void ILRC2Encrypt(ILRC2Context *rc2, unsigned char *input,
				  unsigned char *output);

/*
 * Decrypt a single 64-bit block using RC2.  The input and
 * output buffers can overlap.
 */
void ILRC2Decrypt(ILRC2Context *rc2, unsigned char *input,
				  unsigned char *output);

/*
 * Finalize an RC2 encryption context, clearing all sensitive values.
 */
void ILRC2Finalize(ILRC2Context *rc2);

#ifdef	__cplusplus
};
#endif

#endif	/* _IL_CRYPT_H */
//

