#include "stdh.h"
#include <Engine/Interface/UIIME.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIMessageBox.h>

HKL			_hKL		= NULL;
BOOL		_bIMEProc	= false;

// 국가 설정 ( 하드 코딩 )
DWORD _dwLocalLanguage	 =   LANG_KOREAN;		// 한국 
							// LANG_CHT;		// 대만 
							// LANG_CHS;		// 중국 (PRC)
							// LANG_JAPANESE;	// 일본

// ----------------------------------------------------------------------------
// Name : GetNationName()
// Desc :
// ----------------------------------------------------------------------------
char* GetNationName ( DWORD dwNations )
{
	switch ( PRIMARYLANGID ( dwNations ) )
	{
	case LANG_CHINESE :
		switch ( SUBLANGID ( dwNations ) )
		{
		case SUBLANG_CHINESE_SIMPLIFIED :
			return "중국(PRC)";
							
		case SUBLANG_CHINESE_TRADITIONAL :
			return "대만";
		}
		return "중국";

	case LANG_KOREAN :
		return "한국";

	case LANG_JAPANESE :
		return "일본";
	}
	return "Unknow";
}


//------------------------------------------------------------------------------
// SetEngMode
// Explain: 키보드 입력을 영어로 전환 하는 함수
// Date : 2005-01-17,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void SetEngMode ( HWND hWnd )
{
	HIMC hImc = ImmGetContext ( hWnd );
	DWORD dwConv, dwSent;
	DWORD dwTemp;

	ImmGetConversionStatus ( hImc, &dwConv, &dwSent );

	dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
	dwTemp |= IME_CMODE_ALPHANUMERIC;
	
	dwConv = dwTemp;

	ImmSetConversionStatus ( hImc, dwConv, dwSent );
	ImmReleaseContext ( hWnd, hImc );
}