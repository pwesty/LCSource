#ifndef __CRYPTMEM_H__
#define __CRYPTMEM_H__

#define CRYPT_INIT_CHAR				128
#if defined (LC_KOR) || defined (LC_CHN)
#define CRYPT_XOR_STRING			"dkwk"
#else
#define CRYPT_XOR_STRING			"nako"
#endif
#define CRYPT_XOR_STRING_LENGTH		4

inline char* CryptMem(char* buf, int size)
{
	char ch = (char)CRYPT_INIT_CHAR;
	const char* strxor = CRYPT_XOR_STRING;
	int i;
	for (i = 0; i < size; i++)
	{
		buf[i] += ch;
		ch = buf[i];
		buf[i] ^= strxor[i % CRYPT_XOR_STRING_LENGTH];
	}
	return buf;
}

inline char* DecryptMem(char* buf, int size)
{
	char ch = (char)CRYPT_INIT_CHAR;
	const char* strxor = CRYPT_XOR_STRING;
	int i;
	for (i = 0; i < size; i++)
	{
		buf[i] ^= strxor[i % CRYPT_XOR_STRING_LENGTH];
		buf[i] -= ch;
		ch = buf[i] + ch;
	}
	return buf;
}

#endif
