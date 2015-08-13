
#include "stdh.h"
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Entities/Lacarette.h>

std::vector< INDEX > CLacarette::vecCoinIndex;

// ----------------------------------------------------------------------------
//  Name 	: CLacarette()
//  Desc 	: 
// ----------------------------------------------------------------------------
CLacarette::CLacarette()
{
	courseIndex = -1;
}

// ----------------------------------------------------------------------------
//  Name 	: ~CLacarette()
//  Desc 	: 
// ----------------------------------------------------------------------------
CLacarette::~CLacarette()
{

}

// ----------------------------------------------------------------------------
//  Name 	: LoadLacaretteDataFromFile()
//  Desc 	: lod파일에서 라카렛 data를 읽는다
// ----------------------------------------------------------------------------
void CLacarette::LoadLacaretteDataFromFile( CStaticArray<CLacarette> &apLacaretteData, const char *fileName )
{

#define LOADBYTE(d)			readBytes = fread(&d, sizeof(BYTE), 1, fp);
#define LOADINT(d)			readBytes = fread(&d, sizeof(int), 1, fp);
#define LOADSTR(d)			{ int len; LOADINT(len); readBytes = fread(&d, len, 1, fp); }
#define LOADQUAD(d)			readBytes = fread(&d, sizeof(SQUAD), 1, fp);

	FILE *fp		= NULL;
	if ((fp = fopen(fileName, "rb")) == NULL) 
	{
		MessageBox(NULL, "Lacarette File is not Exist.", "error!", MB_OK);
		return;
	}

	int readBytes	 = 0;
	int courseCount = 0;
	int index = -1;
	int coinCount = 0;
	int retteNumCount = 0;
	char buff[1024];

	readBytes = fread(&courseCount, sizeof(int), 1, fp);				// 코스 개수
	apLacaretteData.New(courseCount);
	ASSERT(apLacaretteData.Count() > 0 && "Invalid Lacarette Data");		
	ASSERT(courseCount > 0 && "Invalid Lacarette Data");

	for( int i=0; i<courseCount; ++i )
	{
		LOADINT( index );
		if( readBytes <= 0 )	break;
		ASSERT( index != -1 && "Invalid Lacarette Index" );	
		
		CLacarette& lacarette = apLacaretteData[i];
		
		lacarette.courseIndex = index;

		memset(buff, 0, 1024);
		LOADSTR(buff);
		lacarette.SetName(buff);

		LOADINT(coinCount)

		for( int j=0; j<coinCount; ++j )
		{
			stCoinData coinData;
			LOADINT(coinData.itemIndex)
			LOADINT(coinData.startCount)
			LOADBYTE(coinData.addCount)

			if( !IsCoinItem( coinData.itemIndex ) ) // vecCoinIndex에 없다면 추가
				lacarette.vecCoinIndex.push_back( coinData.itemIndex );

			lacarette.vecCoinData.push_back(coinData);
		}

		for( int k=0; k<LACARETTE_GIFTDATA_TOTAL; ++k )
		{
			stGiftData giftData;
			LOADINT(giftData.itemIndex)
			LOADQUAD(giftData.itemCount)
			
			LOADINT(retteNumCount)

			lacarette.vecGiftData.push_back(giftData);
			for( int l=0; l<retteNumCount; ++l )
			{
				BYTE retteNum;
				LOADBYTE(retteNum)
				lacarette.vecGiftData[k].vecRetteNum.push_back(retteNum);

				lacarette.retteData[retteNum-1] = k;
			}
		}

	}

#undef LOADBYTE
#undef LOADINT
#undef LOADSTR
#undef LOADQUAD

	fclose(fp);
}