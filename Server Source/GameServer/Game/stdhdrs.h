#ifndef __STDHDRS_H__
#define __STDHDRS_H__

// 게임 서버만 설정하는 거
#define __GAME_SERVER__

#include "Conf.h"
#include "Config.h"
#include "Sysdep.h"

#ifdef  PROC_BILLING
#	undef PROC_BILLING
#else
#	define PROC_BILLING	0
#endif

#include "Utils.h"

inline void BackSlash(char* dest, int nSizeOfDest, const char* src)
{
	if (nSizeOfDest < 1)
		return ;
	while (*src)
	{
		nSizeOfDest--;
		if (nSizeOfDest < 1)
			break;
		*dest++ = *src;
		if (*src == '\\')
		{
			nSizeOfDest--;
			if (nSizeOfDest < 1)
				break;
			*dest++ = '\\';
		}
		src++;
	}
	*dest++ = '\0';
}

#ifdef USING_NPROTECT
#ifndef CIRCLE_WINDOWS
#include <dlfcn.h>
#endif // #ifndef CIRCLE_WINDOWS
#include <ggsrv25.h>
#define NPROTECT_PATH						"./nProtect/"					// 인증 모듈 경로
#define	NPROTECT_LOG						(NPLOG_DEBUG | NPLOG_ERROR)		// 사용 로그
#define	NPROTECT_SECOND_QUERY_TIME			10								// 두번째 인증 요청 메시지 보내는 시간 : 10초
#define	NPROTECT_QUERY_TIME					(3 * 60)						// 인증 요청 메시지 보내는 시간 : 3분
#define PULSE_NPROTECT_UPDATE				(5 * PULSE_REAL_MIN)			// nProtect GameGuard 모듈 업데이트 주기 : 5분
#ifdef _DEBUG
#define NPROTECT_ACTIVE_NUM					10								// 인증 모듈 업데이트 조건 : 새 버전 접속자 수
#define NPROTECT_UPDATE_CONDITION_TIME		15								// 인증 모듈 업데이트 조건 : 시간 제한 : 30분
#else
#define NPROTECT_ACTIVE_NUM					50								// 인증 모듈 업데이트 조건 : 새 버전 접속자 수 : 기본 동접 30%, 50정도
#define NPROTECT_UPDATE_CONDITION_TIME		30								// 인증 모듈 업데이트 조건 : 시간 제한 : 30분
#endif // _DEBUG
#define NPROTECT_UPDATE_CONDITION_COUNT		50								// 인증 모듈 업데이트 조건 : 모듈 사용 비율 : 50%
#endif // USING_NPROTECT

#endif
