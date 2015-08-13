#include "StdH.h"

#include <Engine/Network/CNetwork.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Entities/TEventString.h>
#include <Engine/Util.h>

stTeventInfo* CTEventString::m_TEventString;

void CTEventString::Destroy()
{
	int i;
	for( i = 0; i < DEF_TEVENT_ROW; ++i )
	{
		SAFE_ARRAY_DELETE(m_TEventString);
	}
}

int CTEventString::LoadTEventStringDataFromFile(const char* FileName)
{
	FILE *fp		= NULL;
	if ((fp = fopen(FileName, "rb")) == NULL) 
	{
		MessageBox(NULL, "File is not Exist.", "error!", MB_OK);
		return -1;
	}

	fflush(fp);

	int iReadBytes	= 0;

	//////////////////////////////////////////////////////////////////////////	
	// MACRO DEFINITION
	//////////////////////////////////////////////////////////////////////////	
#define LOADINT(d)			iReadBytes = fread(&d, sizeof(int), 1, fp);

	int count = 0;
	// 초기 셋팅 클래스 갯수
	LOADINT(count);

	m_TEventString = new stTeventInfo;
		
	iReadBytes = fread(m_TEventString, sizeof(stTeventInfo), 1, fp);

	if( iReadBytes <= 0 )
	{
		fclose(fp);
		return iReadBytes;
	}	

#undef LOADINT

	fclose(fp);

	return iReadBytes;
}

BOOL CTEventString::IsValidEvent( int nID )
{
	if (m_TEventString == NULL)
		return FALSE;

	int i;

	for( i = 0; i < DEF_TEVENT_ROW; ++i )
	{
		if (m_TEventString->index[i][0] == nID)
			return TRUE;
	}

	return FALSE;
}

int CTEventString::GetTEventName( int nID )
{
	if (m_TEventString == NULL)
		return -1;

	int i;

	for( i = 0; i < DEF_TEVENT_ROW; ++i )
	{
		if (m_TEventString->index[i][0] == nID)
			return m_TEventString->index[i][1];
	}

	return -1;
}
