#ifndef __LC_CRC32_H__
#define __LC_CRC32_H__

class CLCCRC32
{
	unsigned int m_table[256];
public:
	CLCCRC32();

	unsigned int CalcCRC32(const void* buf, int nLen);
}; // class CLCCRC32

inline CLCCRC32::CLCCRC32()
{
	const unsigned int nPoly = 0xedb88320;
	unsigned int nCRC;
	unsigned int i, j;

	for (i = 0; i < 256; i++)
	{
		nCRC = i;
		for (j = 8; j > 0; j--)
		{
			if (nCRC & 1)
				nCRC = (nCRC >> 1) ^ nPoly;
			else
				nCRC >>= 1;
		}
		m_table[i] = nCRC;
	}
}

inline unsigned int CLCCRC32::CalcCRC32(const void* buf, int nLen)
{
	register unsigned int nCRC = 0xffffffff;
	const unsigned char* pData = (unsigned char*)buf;
	while (nLen > 0)
	{
		nCRC = (nCRC >> 8) ^ m_table[(nCRC ^ (*pData)) & 0xff];
		pData++;
		nLen--;
	}
	return nCRC ^ 0xffffffff;
}

#endif // __LC_CRC32_H__
