#include <math.h>
#include "stdhdrs.h"

#include "Log.h"
#include "Server.h"
#include "../ShareLib/DBCmd.h"

static float	a_sin[360];
static float	a_cos[360];
struct timeval	a_NullTime;

/////////////////////////////////////////
// 수치 데이터에서 문자열로 변환하는 함수

////////////////////
// Function name	: __Itoa
// Description	    : 32비트 정수를 문자열로
// Return type		: char*
//					: 변환 완료된 문자열 포인터
// Argument         : int value
//					: 변환 하고자 하는 정수
// Argument         : char *string
//					: 변환된 데이터를 저장할 문자열 포인터
char* __ltoa(int value, char *string)
{
	char p_buf[128];
	int idx, i;

	// 초기화
	*string = '\0';
	idx = 0;
	i = 0;

	// 음수면 -부호를 붙이고 value는 양수로 무조건
	if (value < 0)
	{
		strcpy(string, "-");
		value = -value;
		i = 1;
	}
	if (value == 0)
	{
		strcpy(string, "0");
		return string;
	}

	// 뒷자리부터 문자열로 변경
	while (value)
	{
		switch (value % 10)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			p_buf[idx++] = (char)((value % 10) + '0');
			break;
		default:
			p_buf[idx++] = '0';
			break;
		}

		value /= 10;
	}

	while (idx)
	{
		string[i++] = p_buf[--idx];
	}
	string[i++] = '\0';

	return string;
}

////////////////////
// Function name	: lltoa
// Description	    : 64비트 정수를 문자열로 변환
// Return type		: char*
//					: 변환된 문자열
// Argument         : LONGLONG value
//					: 64비트 정수
// Argument         : char *string
//					: 변환에 사용하는 버퍼
char* __ltoa(LONGLONG value, char *string)
{
	char p_buf[128];
	int idx, i;

	// 초기화
	*string = '\0';
	idx = 0;
	i = 0;

	// 음수면 -부호를 붙이고 value는 양수로 무조건
	if (value < 0)
	{
		strcpy(string, "-");
		value = -value;
		i = 1;
	}
	if (value == 0)
	{
		strcpy(string, "0");
		return string;
	}

	// 뒷자리부터 문자열로 변경
	while (value)
	{
		switch (value % 10)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			p_buf[idx++] = (char)((value % 10) + '0');
			break;
		default:
			p_buf[idx++] = '0';
			break;
		}

		value /= 10;
	}

	while (idx)
	{
		string[i++] = p_buf[--idx];
	}
	string[i++] = '\0';

	return string;
}

////////////////////
// Function name	: __ftoa
// Description	    : float 데이터를 문자열로 변환
// Return type		: char*
//					: 변환된 문자열
// Argument         : float f
//					: 입력 실수
// Argument         : char *buf
//					: 출력 포인터
// Argument         : int place
//					: 자리수
char* __ftoa(float f, char *buf, int place)
{
	char p_buf[128];

	*buf = '\0';

	// 소수점 앞자리 문자열로
	// 양수 음수 가려서
	if (f < 0)
	{
		// 음수면 양수로 만들고
		f = -f;
		__ltoa((int)f, p_buf);
		// 문자열 앞에 -를 붙이고
		strcpy(buf, "-");
		strcat(buf, p_buf);
	}
	else
	{
		__ltoa((int)f, buf);
	}

	// 소수점 몇자리 까지?
	if (place <= 0)
	{
		return buf;
	}

	// 소수점 8자리까지만
	if (place > 8)
		place = 8;

	// 소수점 뒤부분만 남기고
	f -= (int)f;

	// 원하는 소수 자리까지 곱해서
	while (place > 0)
	{
		f *= 10;
		place--;
	}

	// 정수 부분만 다시 변환
	__ltoa((int)f, p_buf);
	strcat(buf, ".");
	strcat(buf, p_buf);

	return buf;
}

/////////////////////////////////////////////
// 수학 관련

////////////////////
// Function name	: MakeMath
// Description	    : 코사인, 사인 값을 1도 단위로 미리 계산해 둠
// Return type		: void
//					:
void MakeMath()
{
	for (int i = 0; i < 360; i++)
	{
		a_sin[i] = sinf((float) i / 360.0f * PI_2);
		a_cos[i] = cosf((float) i / 360.0f * PI_2);
	}
}

////////////////////
// Function name	: get_sin
// Description	    : 미리 구해둔 사인 값을 얻음
// Return type		: float
//					: 사인값
// Argument         : float radian
//					: 각도, 단위는 도, 범위는 -180 ~ +180
float get_sin(float radian)
{
	int i = (int)(radian / PI_2 * 360);

	if (i < 0)
	{
		i %= 360;
		i += 360;
	}

	if (i >= 360)
	{
		i %= 360;
	}
	/*while (i >= 360) {
		i -= 360;
	}
	while (i < 0) {
		i += 360;
	}*/

	return a_sin[i];
}

////////////////////
// Function name	: get_cos
// Description	    : 미리 구해둔 코사인 값을 얻음
// Return type		: float
//					: 사인값
// Argument         : float radian
//					: 각도, 단위는 도, 범위는 -180 ~ +180
float get_cos(float radian)
{
	int i = (int)(radian / PI_2 * 360);

	if (i < 0)
	{
		i %= 360;
		i += 360;
	}

	if (i >= 360)
	{
		i %= 360;
	}
	/*while (i >= 360) {
		i -= 360;
	}
	while (i < 0) {
		i += 360;
	}*/

	return a_cos[i];
}

///////////////////////////////////////////
// 시간 관련

////////////////////
// Function name	: GetTimeofDay
// Description	    : 현재 시간을 수함
// Return type		: void
//					:
// Argument         : struct timeval *t
//					: 구한 시간을 설정
// Argument         : struct timezone *dummy
//					: 사용하지 않음
void GetTimeofDay(struct timeval *t, struct timezone* dummy)
{
#ifdef CIRCLE_WINDOWS
	unsigned long millisec = GetTickCount();
	t->tv_sec = (int) (millisec / 1000);
	t->tv_usec = (millisec % 1000) * 1000;
#else
	gettimeofday (t, dummy);
#endif
}

////////////////////
// Function name	: TimeDiff
// Description	    : 시간 차이를 구한다, a - b = rslt
// Return type		: void
//					:
// Argument         : struct timeval *rslt
//					: 결과를 저장할 변수
// Argument         : struct timeval *a
//					: 시간 값, 기준 값
// Argument         : struct timeval *b
//					: 시간 값, 빼는 값
void TimeDiff(struct timeval *rslt, struct timeval *a, struct timeval *b)
{
	// a가 작으면 0
	if (a->tv_sec < b->tv_sec)
		*rslt = a_NullTime;

	// a와 b의 초단위 비교
	else if (a->tv_sec == b->tv_sec)
	{
		// 초단위 같을때
		// a가 작으면 0
		if (a->tv_usec < b->tv_usec)
			*rslt = a_NullTime;
		else
		{
			// 차이를 구함
			rslt->tv_sec = 0;
			rslt->tv_usec = a->tv_usec - b->tv_usec;
		}
	}

	else
	{
		// 초단위 다를때
		// 초단위 차이 구함
		rslt->tv_sec = a->tv_sec - b->tv_sec;
		// a가 마이크로초 단위는 작다면
		if (a->tv_usec < b->tv_usec)
		{
			// 마이크로초 단위에서 연산하고
			rslt->tv_usec = a->tv_usec + 1000000 - b->tv_usec;
			// 초단위는 자리내림으로 감소
			rslt->tv_sec--;
		}
		else
			rslt->tv_usec = a->tv_usec - b->tv_usec;
	}
}

/////////////////////////////////////////
// 거리 계산 함수

////////////////////
// Function name	: GetDistance
// Description	    : 거리 계산
// Return type		: float
//					: 거리 반환
// Argument         : float x1
//					: 시작 x 좌표
// Argument         : float x2
//					: 끝 x 좌표
// Argument         : float z1
//					: 시작 z 좌표
// Argument         : float z2
//					: 끝 z 좌표
// Argument         : float h1
//					: 시작 높이
// Argument         : float h2
//					: 끝 높이
float GetDistance(float x1, float x2, float z1, float z2, float h1, float h2)
{
	float dx = ABS(x1 - x2);
	float dz = ABS(z1 - z2);
	float dh = ABS(h1 - h2);

	float ret = (float)sqrt(dx*dx + dz*dz + dh*dh);
	return (ret > 0.0f) ? ret : 0.0f;
}

/////////////////////////////////
// 문자열 관련

////////////////////
// Function name	: SkipSpaces
// Description	    : 앞부분 공백 넘김, 원본에 변형 없음
// Return type		: char*
//					: 처음 나오는 비공백 문자위 위치를 반환
// Argument         : char* string
//					: 공백을 넘기고자 하는 문자열
const char* SkipSpaces(const char* string)
{
	// 공백이면 다음으로 넘기기
	for (; string && *string && is_space_in_util(*string); (string)++)
		;
	// 포인터 반환
	return string;
}

const char* SkipSpecialCharacter(const char* string)
{
	for(; string && *string && is_special_in_util(*string); (string)++)
		;
	return string;
}

////////////////////
// Function name	: TrimString
// Description	    : 앞뒤 공백을 제거, 원본이 수정됨
// Return type		: char*
//					: 입력 문자열
// Argument         : char* string
//					: 공백을 제거하고자 하는 문자열
char* TrimString(char* string)
{
	if(string == NULL) return NULL;

	int len = strlen(string);
	int index = 0, i;

	// 공백이 아닌 처음 문자 구함
	while (string[index] == ' ')
		index ++;

	// 공백만큼 당기기
	if (index > 0)
	{
		int count = len - index;
		for (i = 0; i <= count; i++)
			string[i] = string[i+index];

		string[i] = '\0';
	}

	// 뒤의 공백 제거
	for (i = strlen(string) - 1; i > 0; i--)
	{
		if (string[i] != ' ')
			break;
	}

	string[i + 1] = '\0';

	return string;
}

////////////////////
// Function name	: AnyOneArg
// Description	    : 공백 단위로 문자열 토큰화
// Return type		: char*
//					: 다음 토큰 위치
// Argument         : char* argument
//					: 원본 문자열
// Argument         : char* first_arg
//					: 토큰화된 문자열을 담을 곳
// Argument         : bool toLower
//					: 소문자로 강제 변환할지 여부
const char* AnyOneArg(const char* argument, char* first_arg, bool toLower)
{
	argument = SkipSpaces(argument);

	while (argument && *argument && !is_space_in_util(*argument))
	{
		if (toLower)
		{
			*(first_arg++) = LOWER(*argument);
		}
		else
		{
			*(first_arg++) = *argument;
		}
		argument++;
	}

	*first_arg = '\0';

	return (argument);
}
////////////////////
// Function name	: GetToken
// Description	    : 특수문자 단위로 문자열 토큰화(숫자나 영문자만 읽음, 그외 문자는 특수문자로 분류함.)
// Return type		: char*
//					: 다음 토큰 위치
// Argument         : char* arg1
//					: 원본 문자열
// Argument         : char* arg2
//					: 토큰화된 문자열을 담을 곳
const char* GetToken(const char* arg1, char* arg2, bool toLower)
{
	arg1 = SkipSpecialCharacter(arg1);

	while(arg1 && *arg1 && !is_special_in_util(*arg1))
	{
		if(toLower)
		{
			*(arg2++) = LOWER(*arg1);
		}
		else
		{
			*(arg2++) = *arg1;
		}
		arg1++;
	}

	*arg2 = '\0';

	return (arg1);
}

////////////////////
// Function name	: IntCat
// Description	    : 문자열에 정수 붙이기
// Return type		: char*
//					: 붙여진 문자열
// Argument         : char *dest
//					: 대상 문자열
// Argument         : int i
//					: 붙여질 정수
char* IntCat(char *dest, int i, bool bInsertSpace)
{
	char i_buf[128];
	if (bInsertSpace)
		strcat(dest, " ");
	strcat(dest, __ltoa(i, i_buf));
	return dest;
}

////////////////////
// Function name	: IntCat
// Description	    : 문자열에 정수 붙이기
// Return type		: char*
//					: 붙여진 문자열
// Argument         : char *dest
//					: 대상 문자열
// Argument         : LONGLONG ll
//					: 붙여질 정수
char* IntCat(char *dest, LONGLONG ll, bool bInsertSpace)
{
	char i_buf[128];
	if (bInsertSpace)
		strcat(dest, " ");
	strcat(dest, __ltoa(ll, i_buf));
	return dest;
}

////////////////////
// Function name	: IntCat0
// Description	    : 정수를 문자열에 붙이기, 지정 폭만큼 0으로 채움
// Return type		: char*
//					: 결과 문자열
// Argument         : char* dest
//					: 저장할 공간
// Argument         : int n
//					: 붙일 정수
// Argument         : int width
//					: 폭
char* IntCat0(char* dest, int n, int width, bool bInsertSpace)
{
	if(dest == NULL) return NULL;

	char tbuf[128];
	int len = strlen(__ltoa(n, tbuf));
	int wasLen = strlen(dest);

	if (bInsertSpace)
		strcat(dest, " ");
	if (width > len)
	{
		width -= len;
		for (int i = wasLen; i < width + wasLen ; i++)
		{
			dest[i] = '0';
			dest[i + 1] = '\0';
		}
	}
	strcat(dest, tbuf);
	return dest;
}

////////////////////
// Function name	: IntCat0
// Description	    : 정수를 문자열에 붙이기, 지정 폭만큼 0으로 채움
// Return type		: char*
//					: 결과 문자열
// Argument         : char* dest
//					: 저장할 공간
// Argument         : LONGLONG n
//					: 붙일 정수
// Argument         : int width
//					: 폭
char* IntCat0(char* dest, LONGLONG n, int width, bool bInsertSpace)
{
	if(dest == NULL) return NULL;

	char tbuf[128];
	int len = strlen(__ltoa(n, tbuf));
	int wasLen = strlen(dest);

	if (bInsertSpace)
		strcat(dest, " ");
	if (width > len)
	{
		width -= len;
		for (int i = wasLen; i < width + wasLen ; i++)
		{
			dest[i] = '0';
			dest[i + 1] = '\0';
		}
	}
	strcat(dest, tbuf);
	return dest;
}

////////////////////
// Function name	: strinc
// Description	    : 특정 문자열을 포함하는지 검사
// Return type		: bool
//					: 포함하면 true, 아니면 return
// Argument         : const char *str
//					: 기준 문자열
// Argument         : const char *inc
//					: 비교 문자열
bool strinc(const char *str, const char *inc)
{
	if(str == NULL || inc == NULL) return false;

	int str_len = strlen(str);
	int inc_len = strlen(inc);

	if (str_len < inc_len)
		return false;

	for (int i = 0; i <= str_len - inc_len; i++)
	{
		if (!strncmp(str+i, inc, inc_len))
			return true;
	}

	return false;
}

const char* DAnyOneArg(const char *argument, char *first_arg)
{
	while (*argument && (*argument != '.'))
	{
		*(first_arg++) = *argument;
		argument++;
	}

	*first_arg = '\0';
	argument++;
	return (argument);
}

void IPtoi(const char *org_IP,int *i_one, int *i_two, int *i_thr, int *i_for)
{
	const char *p_IP;
	char tempStr[10];

	p_IP = org_IP;
	p_IP = DAnyOneArg(p_IP,tempStr);
	*i_one = atoi(tempStr);
	p_IP = DAnyOneArg(p_IP,tempStr);
	*i_two = atoi(tempStr);
	p_IP = DAnyOneArg(p_IP,tempStr);
	*i_thr = atoi(tempStr);
	*i_for = atoi(p_IP);
}

// [selo: 101115] 러시아어 이외의 글자가 있는지 확인
// 러시아어 이외의 글자가 있으면 true 반환, 없으면 false 반환
bool CheckNoRussianCharacter(const CLCString& str)
{
	int len = str.Length();
	for( int i = 0; i < len; i++ )
	{
		if( !(str[i] >= 48 && str[i] <= 57)
				&& !(str[i] >= -64 && str[i] <= -1)
				&& !(str[i] == -88)
				&& !(str[i] == -72)	)
		{
			return true;
		}
	}

	return false;
}
//