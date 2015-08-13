#include "stdhdrs.h"
#include "Log.h"
#include "Server.h"

#ifdef __GAME_SERVER__
// 시리얼 코드
static char		code62[62] = 
{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
#endif

static float	a_sin[360];
static float	a_cos[360];
struct timeval	a_NullTime;

char			g_buf[MAX_STRING_LENGTH];
char			g_buf2[MAX_STRING_LENGTH];
char			g_buf3[MAX_STRING_LENGTH];



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
	for (int i = 0; i < 360; i++) {
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


////////////////////
// Function name	: TimeAdd
// Description	    : 시간의 함을 구한다, a + b = rslt
// Return type		: void 
//					: 
// Argument         : struct timeval *rslt
//					: 결과 값
// Argument         : struct timeval *a
//					: 시간 값, 기준 값
// Argument         : struct timeval *b
//					: 시간 값, 더해지는 값
void TimeAdd(struct timeval *rslt, struct timeval *a, struct timeval *b)
{
	rslt->tv_sec = a->tv_sec + b->tv_sec;
	rslt->tv_usec = a->tv_usec + b->tv_usec;

	// 자리올림 처리
	while (rslt->tv_usec >= 1000000)
	{
		rslt->tv_usec -= 1000000;
		rslt->tv_sec++;
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
	for (; string && *string && is_space(*string); (string)++)
		;
	// 포인터 반환
	return string;
}


////////////////////
// Function name	: RemoveTailSpaces
// Description	    : 끝의 공백을 제거, 원본을 수정
// Return type		: char* 
//					: 입력 문자열 포인터 반환
// Argument         : char* string
//					: 공백을 제거하고자 하는 문자열
char* RemoveTailSpaces(char* string)
{
	// 길이를 구하고
	int len = strlen(string);

	// 끝부터 공백을 검사하여
	for (int i = len - 1; i >= 0; i++)
	{
		// 공백이면 NULL 문자로 대체
		if (is_space(string[i]))
			string[i] = '\0';
		else
			break;
	}

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
	int len = strlen(string);
	int index = 0, i;

	// 공백이 아닌 처음 문자 구함
	while (string[index] == ' ')
		index ++;

	// 공백만큼 당기기
	if (index > 0)
	{
		for (i = 0; i <= (len-index); i++)
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
//					: 토큰화된 문자열을 담을 곧
// Argument         : bool toLower
//					: 소문자로 강제 변환할지 여부
const char* AnyOneArg(const char* argument, char* first_arg, bool toLower)
{
	argument = SkipSpaces(argument);

	while (argument && *argument && !is_space(*argument))
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

char* TransDateStr(int time, char* convbuf)//0627 // 광호 수정
{
	struct tm ti;
	
	time_t t = (long)time;
	
	ti = *localtime(&t);

	sprintf(convbuf, "%04d/%02d/%02d %02d:%02d:%02d", ti.tm_year + 1900, ti.tm_mon + 1, ti.tm_mday, ti.tm_hour, ti.tm_min, ti.tm_sec);

	return convbuf;
}

#ifndef __BILLING_SERVER__
////////////////////
// Function name	: StrDup
// Description	    : 문자열 복사, C의 라이브러리와 다르게 new 연산자를 이용하여 메모리 할당
// Return type		: char* 
//					: 복사된 문자열 포인터
// Argument         : const char* source
//					: 원본 문자열
char* StrDup(const char* source)
{
	char *new_z;
	new_z = new char[strlen(source) + 1];
	return (strcpy(new_z, source));
}


////////////////////
// Function name	: StrCat
// Description	    : 문자열 붙이기, 앞에 공백을 하나 삽입
// Return type		: char* 
//					: 붙여진 문자열
// Argument         : char* dest
//					: 대상 문자열
// Argument         : const char* src
//					: 붙여질 문자열
char* StrCat(char* dest, const char* src)
{
	strcat(dest, " ");
	strcat(dest, src);
	return dest;
}
#endif


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
// Function name	: FloatCat
// Description	    : 문자열에 실수 붙이기
// Return type		: char* 
//					: 붙여진 문자열
// Argument         : char *dest
//					: 대상 문자열
// Argument         : float f
//					: 붙여질 실수
// Argument         : int place
//					: 소수점 이하 자리수
char* FloatCat(char *dest, float f, int place, bool bInsertSpace)
{
	char f_buf[128];
	if (bInsertSpace)
		strcat(dest, " ");
	strcat(dest, __ftoa(f, f_buf, place));
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



// 게임 서버 전용 함수들
#ifdef __GAME_SERVER__

//////////////////////////////////////////
// 아이템 시리얼 관련

////////////////////
// Function name	: GetSerial
// Description	    : 시리얼 넘버를 구함
// Return type		: void 
//					: 
// Argument         : char *buf
//					: 
// Argument         : int index
//					: 
void GetSerial(char *buf, int index)
{
	int temp = (gserver.m_serverno * 20 + gserver.m_subno) % 62; // (a_WorldSubNum % 50 + a_ServerNum);
	if (temp < 0) temp=0;
	buf[0] = code62[temp % 62];
	buf[1] = code62[gserver.m_serverTime.year % 62];
	buf[2] = code62[gserver.m_serverTime.month % 62];
	buf[3] = code62[gserver.m_serverTime.day % 62];
	buf[4] = code62[gserver.m_serverTime.hour % 62];
	buf[5] = code62[gserver.m_serverTime.min % 62];
	
	if (index < 0) index = -index;
	index %= 238328;
	buf[8] = code62[index % 62];	index /= 62;
	buf[7] = code62[index % 62];	index /= 62;
	buf[6] = code62[index % 62];
	buf[9] = '\0';
}
void GetSerial(CLCString& buf, int index)
{
	char tmp[MAX_SERIAL_LENGTH + 1];
	GetSerial(tmp, index);
	buf = tmp;
}


////////////////////
// Function name	: GetDistance
// Description	    : 캐릭터 사이의 거리 계산
// Return type		: float 
//					: 거리 반환
// Argument         : CCharacter* ch
//					: 기준 캐릭터
// Argument         : CCharacter* tch
//					: 측정 캐릭터
float GetDistance(CCharacter* ch, CCharacter* tch)
{
	if (ch == tch)
		return 0;
	if (ch == NULL || tch == NULL)
		return 0;
	float ret = GetDistance(GET_X(ch), GET_X(tch), GET_Z(ch), GET_Z(tch), GET_H(ch), GET_H(tch)) - ch->GetSize() - tch->GetSize();
	return (ret > 0.0f) ? ret : 0.0f;
}


////////////////////
// Function name	: GetDistance
// Description	    : 특정 좌표와 캐릭터 사이의 거리
float GetDistance(float x, float z, float h, CCharacter* tch)
{
	float ret = GetDistance(x, GET_X(tch), z, GET_Z(tch), h, GET_H(tch)) - tch->GetSize();
	return (ret > 0.0f) ? ret : 0.0f;
}

#endif

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

#if defined(LC_KOR) || defined(LC_CHN)

bool CheckIP(const char *u_IP)
{
	// 사내 아이피 대역은 제외
	// 대만 아이피 막기

	// BClassIp: B Class 하위 65536개 모두 막기 xxx.xxx.nnn.xxx
	// FirType: C Class 하위 256개 막기 xxx.xxx.xxx.nnn
	// SecType: B Class 일부 xxx.xxx.nnn.xxx ~ xxx.xxx.mmm.xxx
	// ThrType: A Class 일부 xxx.nnn.xxx.xxx ~ xxx.mmm.xxx.xxx
	// 순서는 thr, sec, fir, bclass
	int BClassIp[64][2] = {
		{222,250},		{210,85},		{218,184},		{218,35},
		{218,34},		{210,58},		{61,20},		{163,29},
		{210,69},		{211,23},		{210,65},		{203,69},
		{61,216},		{211,20},		{203,75},		{60,248},
		{61,217},		{211,22},		{211,21},		{210,61},
		{203,74},		{203,66},		{220,229},		{218,32},
		{61,66},		{220,228},		{219,80},		{219,81},
		{61,30},		{61,31},		{221,169},		{210,68},
		{203,73},		{210,64},		{203,67},		{61,59},
		{210,66},		{203,70},		{219,84},		{219,85},
		{61,62},		{140,135},		{140,132},		{140,129},
		{140,113},		{140,110},		{210,240},		{203,71},
		{140,138},		{140,133},		{140,130},		{140,115},
		{140,111},		{210,60},		{203,72},		{203,68},
		{140,114},		{140,134},		{140,131},		{140,128},
		{140,112},		{140,109},		{210,70},		{203,64}
	};	

	
	int FirType[2][3] = {
		{202,3,129},	{203,119,3}
	};

	int SecType[183][3] = {
		{202,8,14},		{202,5,4},		{202,6,104},	{202,5,12},
		{202,5,8},		{210,62,248},	{210,67,248},	{210,62,240},
		{211,79,192},	{61,64,32},		{61,57,240},	{61,57,192},
		{61,67,112},	{61,56,48},		{61,61,192},	{61,57,208},
		{61,60,240},	{61,58,48},		{61,58,16},		{211,79,96},
		{211,78,80},	{61,56,64},		{61,56,80},		{210,67,96},
		{211,78,240},	{202,168,192},	{61,64,48},		{210,62,176},
		{211,76,224},	{211,76,96},	{61,56,16},		{61,57,144},
		{61,67,48},		{61,56,0},		{211,78,64},	{61,67,0},
		{211,79,32},	{211,79,208},	{61,61,240},	{211,78,0},
		{211,78,16},	{61,57,160},	{61,57,176},	{61,60,208},
		{61,58,0},		{210,62,160},	{211,79,48},	{61,56,96},
		{211,76,160},	{211,76,240},	{211,76,112},	{61,58,112},
		{61,57,224},	{211,78,208},	{61,56,112},	{61,60,192},
		{211,76,144},	{61,56,128},	{61,56,144},	{211,78,192},
		{211,76,128},	{210,67,128},	{202,39,96},	{210,67,0},
		{210,67,112},	{210,67,16},	{210,62,224},	{211,78,224},
		{61,58,80},		{61,58,64},		{61,60,224},	{210,67,192},
		{210,67,208},	{210,67,144},	{61,58,32},		{61,56,32},
		{61,58,96},		{211,76,176},	{61,67,96},		{210,63,224},
		{210,63,192},	{211,76,192},	{211,73,32},	{61,63,128},
		{61,60,128},	{61,60,160},	{210,62,96},	{210,241,160},
		{211,73,128},	{210,241,192},	{203,65,160},	{210,62,0},
		{210,62,32},	{210,62,192},	{210,67,160},	{61,57,32},
		{211,79,128},	{61,67,64},		{211,79,160},	{61,60,0},
		{203,65,128},	{210,241,224},	{211,73,96},	{61,63,0},
		{211,78,32},	{210,244,192},	{211,76,0},		{210,244,224},
		{211,73,64},	{61,56,224},	{211,78,160},	{61,56,192},
		{211,78,128},	{211,73,192},	{203,65,192},	{211,73,224},
		{210,67,64},	{210,241,128},	{61,64,0},		{211,73,0},
		{211,79,64},	{210,67,32},	{210,63,0},		{210,63,32},
		{210,62,64},	{61,58,160},	{61,63,96},		{61,67,16},
		{61,58,128},	{211,78,96},	{202,160,64},	{210,243,96},
		{210,63,160},	{211,79,0},		{210,63,128},	{61,57,0},
		{210,243,64},	{61,56,160},	{211,76,32},	{61,61,208},
		{203,65,224},	{211,77,64},	{211,77,0},		{210,241,0},
		{210,241,64},	{61,65,192},	{220,128,0},	{61,63,32},
		{210,63,64},	{61,58,192},	{61,65,128},	{61,64,64},
		{210,243,0},	{61,57,64},		{61,63,160},	{61,60,32},
		{221,120,0},	{202,39,0},		{202,178,128},	{222,251,0},
		{211,77,128},	{203,65,0},		{211,75,128},	{210,59,128},
		{211,72,128},	{211,72,0},		{210,242,0},	{210,71,128},
		{202,39,128},	{211,75,0},		{210,242,128},	{61,67,128},
		{61,65,0},		{211,74,0},		{211,74,128},	{210,244,0},
		{210,243,128},	{61,64,128},	{210,71,0},		{210,59,0},
		{210,17,0},		{220,128,64},	{61,61,0}
	};
	int SecTypeLimit[183] = {
		15,		5,		105,	15,
		11,		255,	255,	247,
		207,	47,		255,	207,
		127,	63,		207,	223,
		255,	63,		31,		111,
		95,		79,		95,		111,
		255,	207,	63,		191,
		239,	111,	31,		159,
		63,		15,		79,		15,
		47,		223,	255,	15,
		31,		175,	191,	223,
		15,		175,	63,		111,
		175,	255,	127,	127,
		239,	223,	127,	207,
		159,	143,	159,	207,
		143,	143,	111,	15,
		127,	31,		239,	239,
		95,		79,		239,	207,
		223,	159,	47,		47,
		111,	191,	111,	255,
		223,	223,	63,		159,
		159,	191,	127,	191,
		159,	223,	191,	31,
		63,		223,	191,	63,
		159,	95,		191,	31,
		159,	255,	127,	31,
		63,		223,	31,		255,
		95,		255,	191,	223,
		159,	223,	223,	255,
		95,		159,	31,		31,
		95,		63,		31,		63,
		95,		191,	127,	47,
		159,	127,	95,		127,
		191,	31,		159,	31,
		95,		191,	63,		239,
		255,	127,	63,		63,
		127,	255,	63,		95,
		127,	255,	191,	127,
		63,		127,	255,	127,
		95,		95,		255,	127,
		255,	127,	255,	255,
		255,	127,	127,	255,
		255,	127,	255,	255,
		127,	127,	255,	127,
		255,	255,	127,	127,
		127,	255,	191
	};

	int ThrType[17][2] = {
		{218,166},		{61,218},		{218,164},		{59,104},
		{140,136},		{163,30},		{61,220},		{59,120},
		{61,224},		{218,160},		{61,228},		{140,116},
		{59,112},		{218,168},		{140,120},		{220,129},
		{163,13}
	};
	int ThrTypeLimit[17] = {
		167,		219,		165,		105,
		137,		32,			223,		123,
		227,		163,		231,		119,
		119,		175,		127,		143,
		28
	};

	char org_IP[20];
	int i_Ip[4];
	int i;
	
	strcpy(org_IP,u_IP);
	IPtoi(org_IP,&i_Ip[0],&i_Ip[1],&i_Ip[2],&i_Ip[3]);
	
	if (i_Ip[0] == 211 && i_Ip[1] == 181 && i_Ip[2] == 248) //나코 피씨방은 풀어주자
		return true;

#ifdef LC_KOR
	// 예외 : 220.130.35.*
	if (i_Ip[0] == 220 && i_Ip[1] == 130 && i_Ip[2] == 35)
		return true;

// 050225 : bs : 대만에서 테스트 서버 접속 허용
#ifdef TEST_SERVER
	// 예외 : 220.130.170.145 ~ 158
	//if (i_Ip[0] == 220 && i_Ip[1] == 130 && i_Ip[2] == 170 && i_Ip[3] >= 145 && i_Ip[3] <= 158)
	//	return true;
	if (i_Ip[0] == 60 && i_Ip[1] == 248 && i_Ip[2] == 77 && i_Ip[3] == 245)
		return true;
#endif
// --- 050225 : bs : 대만에서 테스트 서버 접속 허용
#endif

	for(i = 0; i < 17; i++)
	{
		if(ThrType[i][0] == i_Ip[0])
			if((ThrType[i][1] <= i_Ip[1]) && (i_Ip[1] <= ThrTypeLimit[i]))
				return false;
	}
	for(i = 0; i < 183; i++)
	{
		if(SecType[i][0] == i_Ip[0] && SecType[i][1] == i_Ip[1])
			if((SecType[i][2] <= i_Ip[2]) && (i_Ip[2] <= SecTypeLimit[i]))
				return false;
	}
	for(i = 0; i < 2; i++)
	{
		if(FirType[i][0] == i_Ip[0] && FirType[i][1] == i_Ip[1] 
										&& FirType[i][2] == i_Ip[2])
			return false;
	}
	for(i = 0; i < 64; i++)
	{
		if(BClassIp[i][0] == i_Ip[0] && BClassIp[i][1] == i_Ip[1])
			return false;
	}

	return true;

} //한국IP막기 여기까지 

#elif defined(LC_TWN)

bool CheckIP(const char *u_IP)
{
	// 대만 아이피 대역만 가능하게 하기
	// BClassIp: B Class 하위 65536개 모두 막기 xxx.xxx.nnn.xxx
	// FirType: C Class 하위 256개 막기 xxx.xxx.xxx.nnn
	// SecType: B Class 일부 xxx.xxx.nnn.xxx ~ xxx.xxx.mmm.xxx
	// ThrType: A Class 일부 xxx.nnn.xxx.xxx ~ xxx.mmm.xxx.xxx
	// 순서는 thr, sec, fir, bclass
	int BClassIp[64][2] = {
		{222,250},		{210,85},		{218,184},		{218,35},
		{218,34},		{210,58},		{61,20},		{163,29},
		{210,69},		{211,23},		{210,65},		{203,69},
		{61,216},		{211,20},		{203,75},		{60,248},
		{61,217},		{211,22},		{211,21},		{210,61},
		{203,74},		{203,66},		{220,229},		{218,32},
		{61,66},		{220,228},		{219,80},		{219,81},
		{61,30},		{61,31},		{221,169},		{210,68},
		{203,73},		{210,64},		{203,67},		{61,59},
		{210,66},		{203,70},		{219,84},		{219,85},
		{61,62},		{140,135},		{140,132},		{140,129},
		{140,113},		{140,110},		{210,240},		{203,71},
		{140,138},		{140,133},		{140,130},		{140,115},
		{140,111},		{210,60},		{203,72},		{203,68},
		{140,114},		{140,134},		{140,131},		{140,128},
		{140,112},		{140,109},		{210,70},		{203,64}
	};	

	
	int FirType[2][3] = {
		{202,3,129},	{203,119,3}
	};

	int SecType[183][3] = {
		{202,8,14},		{202,5,4},		{202,6,104},	{202,5,12},
		{202,5,8},		{210,62,248},	{210,67,248},	{210,62,240},
		{211,79,192},	{61,64,32},		{61,57,240},	{61,57,192},
		{61,67,112},	{61,56,48},		{61,61,192},	{61,57,208},
		{61,60,240},	{61,58,48},		{61,58,16},		{211,79,96},
		{211,78,80},	{61,56,64},		{61,56,80},		{210,67,96},
		{211,78,240},	{202,168,192},	{61,64,48},		{210,62,176},
		{211,76,224},	{211,76,96},	{61,56,16},		{61,57,144},
		{61,67,48},		{61,56,0},		{211,78,64},	{61,67,0},
		{211,79,32},	{211,79,208},	{61,61,240},	{211,78,0},
		{211,78,16},	{61,57,160},	{61,57,176},	{61,60,208},
		{61,58,0},		{210,62,160},	{211,79,48},	{61,56,96},
		{211,76,160},	{211,76,240},	{211,76,112},	{61,58,112},
		{61,57,224},	{211,78,208},	{61,56,112},	{61,60,192},
		{211,76,144},	{61,56,128},	{61,56,144},	{211,78,192},
		{211,76,128},	{210,67,128},	{202,39,96},	{210,67,0},
		{210,67,112},	{210,67,16},	{210,62,224},	{211,78,224},
		{61,58,80},		{61,58,64},		{61,60,224},	{210,67,192},
		{210,67,208},	{210,67,144},	{61,58,32},		{61,56,32},
		{61,58,96},		{211,76,176},	{61,67,96},		{210,63,224},
		{210,63,192},	{211,76,192},	{211,73,32},	{61,63,128},
		{61,60,128},	{61,60,160},	{210,62,96},	{210,241,160},
		{211,73,128},	{210,241,192},	{203,65,160},	{210,62,0},
		{210,62,32},	{210,62,192},	{210,67,160},	{61,57,32},
		{211,79,128},	{61,67,64},		{211,79,160},	{61,60,0},
		{203,65,128},	{210,241,224},	{211,73,96},	{61,63,0},
		{211,78,32},	{210,244,192},	{211,76,0},		{210,244,224},
		{211,73,64},	{61,56,224},	{211,78,160},	{61,56,192},
		{211,78,128},	{211,73,192},	{203,65,192},	{211,73,224},
		{210,67,64},	{210,241,128},	{61,64,0},		{211,73,0},
		{211,79,64},	{210,67,32},	{210,63,0},		{210,63,32},
		{210,62,64},	{61,58,160},	{61,63,96},		{61,67,16},
		{61,58,128},	{211,78,96},	{202,160,64},	{210,243,96},
		{210,63,160},	{211,79,0},		{210,63,128},	{61,57,0},
		{210,243,64},	{61,56,160},	{211,76,32},	{61,61,208},
		{203,65,224},	{211,77,64},	{211,77,0},		{210,241,0},
		{210,241,64},	{61,65,192},	{220,128,0},	{61,63,32},
		{210,63,64},	{61,58,192},	{61,65,128},	{61,64,64},
		{210,243,0},	{61,57,64},		{61,63,160},	{61,60,32},
		{221,120,0},	{202,39,0},		{202,178,128},	{222,251,0},
		{211,77,128},	{203,65,0},		{211,75,128},	{210,59,128},
		{211,72,128},	{211,72,0},		{210,242,0},	{210,71,128},
		{202,39,128},	{211,75,0},		{210,242,128},	{61,67,128},
		{61,65,0},		{211,74,0},		{211,74,128},	{210,244,0},
		{210,243,128},	{61,64,128},	{210,71,0},		{210,59,0},
		{210,17,0},		{220,128,64},	{61,61,0}
	};
	int SecTypeLimit[183] = {
		15,		5,		105,	15,
		11,		255,	255,	247,
		207,	47,		255,	207,
		127,	63,		207,	223,
		255,	63,		31,		111,
		95,		79,		95,		111,
		255,	207,	63,		191,
		239,	111,	31,		159,
		63,		15,		79,		15,
		47,		223,	255,	15,
		31,		175,	191,	223,
		15,		175,	63,		111,
		175,	255,	127,	127,
		239,	223,	127,	207,
		159,	143,	159,	207,
		143,	143,	111,	15,
		127,	31,		239,	239,
		95,		79,		239,	207,
		223,	159,	47,		47,
		111,	191,	111,	255,
		223,	223,	63,		159,
		159,	191,	127,	191,
		159,	223,	191,	31,
		63,		223,	191,	63,
		159,	95,		191,	31,
		159,	255,	127,	31,
		63,		223,	31,		255,
		95,		255,	191,	223,
		159,	223,	223,	255,
		95,		159,	31,		31,
		95,		63,		31,		63,
		95,		191,	127,	47,
		159,	127,	95,		127,
		191,	31,		159,	31,
		95,		191,	63,		239,
		255,	127,	63,		63,
		127,	255,	63,		95,
		127,	255,	191,	127,
		63,		127,	255,	127,
		95,		95,		255,	127,
		255,	127,	255,	255,
		255,	127,	127,	255,
		255,	127,	255,	255,
		127,	127,	255,	127,
		255,	255,	127,	127,
		127,	255,	191
	};

	int ThrType[17][2] = {
		{218,166},		{61,218},		{218,164},		{59,104},
		{140,136},		{163,30},		{61,220},		{59,120},
		{61,224},		{218,160},		{61,228},		{140,116},
		{59,112},		{218,168},		{140,120},		{220,129},
		{163,13}
	};
	int ThrTypeLimit[17] = {
		167,		219,		165,		105,
		137,		32,			223,		123,
		227,		163,		231,		119,
		119,		175,		127,		143,
		28
	};

	char org_IP[20];
	int i_Ip[4];
	int i;
	
	strcpy(org_IP,u_IP);
	IPtoi(org_IP,&i_Ip[0],&i_Ip[1],&i_Ip[2],&i_Ip[3]);
	
	if (i_Ip[0] == 211 && i_Ip[1] == 181 && i_Ip[2] == 248) //나코 피씨방은 풀어주자
		return true;

	for(i = 0; i < 17; i++)
	{
		if(ThrType[i][0] == i_Ip[0])
			if((ThrType[i][1] <= i_Ip[1]) && (i_Ip[1] <= ThrTypeLimit[i]))
				return true;
	}
	for(i = 0; i < 183; i++)
	{
		if(SecType[i][0] == i_Ip[0] && SecType[i][1] == i_Ip[1])
			if((SecType[i][2] <= i_Ip[2]) && (i_Ip[2] <= SecTypeLimit[i]))
				return true;
	}
	for(i = 0; i < 2; i++)
	{
		if(FirType[i][0] == i_Ip[0] && FirType[i][1] == i_Ip[1] 
										&& FirType[i][2] == i_Ip[2])
			return true;
	}
	for(i = 0; i < 64; i++)
	{
		if(BClassIp[i][0] == i_Ip[0] && BClassIp[i][1] == i_Ip[1])
			return true;
	}

	return false;
}

#elif defined(LC_JPN)

bool CheckIP(const char *u_IP)
{
	// 다음 대역에 속하는 ip는 접근을 제한한다
	// IPv4에서 각 아이피 자리를 숫자로 변경하여 그 범위를 검사한다.
	// 1.2.3.4는 (1 << 24) | (2 << 16) | (3 << 8) | 4로 계산한다
	static unsigned int tableBlockIP[][2] = {
		{3544711168UL, 3544973311UL},
		{3538944000UL, 3539468287UL},
		{3536846848UL, 3536928767UL},
		{3536322560UL, 3536347135UL},
		{3535798272UL, 3535863807UL},
		{3527081984UL, 3527933951UL},
		{3414491136UL, 3414507519UL},
		{3412787200UL, 3412803583UL},
		{3412033536UL, 3412066303UL},
		{3410952192UL, 3411017727UL},
		{3410886656UL, 3410894847UL},
		{3410821120UL, 3410853887UL},
		{3409969152UL, 3410755583UL},
		{3414491136UL, 3414507519UL},
		{3412787200UL, 3412803583UL},
		{3412033536UL, 3412066303UL},
		{3410952192UL, 3411017727UL},
		{3410886656UL, 3410894847UL},
		{3410821120UL, 3410853887UL},
		{3409969152UL, 3410755583UL},
		{3414491136UL, 3414507519UL},
		{3412787200UL, 3412803583UL},
		{3412033536UL, 3412066303UL},
		{3410952192UL, 3411017727UL},
		{3410886656UL, 3410894847UL},
		{3410821120UL, 3410853887UL},
		{3409969152UL, 3410755583UL},
		{3389917184UL, 3389919231UL},
		{3389382656UL, 3389390847UL},
		{3389142016UL, 3389143039UL},
		{2355953664UL, 2357919743UL},
		{2355101696UL, 2355167231UL},
		{2354839552UL, 2354905087UL},
		{2343501824UL, 2343567359UL},
		{2261778432UL, 2261843967UL},
		{2245984256UL, 2346713087UL},
		{3345708800UL, 3345709055UL},
		{3278942208UL, 3278942211UL},
		{3262474071UL, 3262474071UL},
		{3262473995UL, 3262473995UL},
		{3262474001UL, 3262474001UL},
		{3233590016UL, 3233590271UL},
		{3262474116UL, 3262474116UL},
		{3262474009UL, 3262474009UL},
		{2168848384UL, 2168913919UL},
		{889192448UL,  905969663UL},
		{2168782848UL, 2168848383UL},
		{2213937152UL, 2214002687UL},
		{3412721664UL, 3412738047UL},
		{2710437888UL, 2710503423UL},
		{2710450176UL, 2710450431UL},
		{3412721664UL, 3412738047UL},
		{3412721664UL, 3412738047UL},
		{3389603840UL, 3389612031UL},
		{3389021696UL, 3389021951UL},
		{3546152960UL, 3546808319UL},
		{3527933952UL, 3528458239UL},
		{3412000768UL, 3412017151UL},
		{3411935232UL, 3411951615UL},
		{3411804160UL, 3411820543UL},
		{3411673088UL, 3411689471UL},
		{3411607552UL, 3411623935UL},
		{3411533824UL, 3411558399UL},
		{3412000768UL, 3412017151UL},
		{3411935232UL, 3411951615UL},
		{3411804160UL, 3411820543UL},
		{3411738624UL, 3411755007UL},
		{3411673088UL, 3411689471UL},
		{3411533824UL, 3411558399UL},
		{3412000768UL, 3412017151UL},
		{3411935232UL, 3411951615UL},
		{3411804160UL, 3411820543UL},
		{3411738624UL, 3411755007UL},
		{3411673088UL, 3411689471UL},
		{3411533824UL, 3411558399UL},
		{3411533824UL, 3411558399UL},
		{3389025792UL, 3389026047UL},
		{2682388480UL, 2682454015UL},
		{2714697728UL, 2714763263UL},
		{2682388736UL, 2682389503UL},
		{3233589760UL, 3233590015UL},
		{3389957120UL, 3389957375UL},
		{3389956608UL, 3389957119UL},
		{3389945088UL, 3389945343UL},
		{3389939200UL, 3389939455UL},
		{3389938688UL, 3389939199UL},
		{3389938176UL, 3389938687UL},
		{3389813760UL, 3389814015UL},
		{3389808896UL, 3389809151UL},
		{3389803008UL, 3389803263UL},
		{3389802752UL, 3389803007UL},
		{3389540352UL, 3389541375UL},
		{3389538304UL, 3389540351UL},
		{3389419264UL, 3389419519UL},
		{3389418240UL, 3389418495UL},
		{3389416448UL, 3389416959UL},
		{3389413888UL, 3389414143UL},
		{3389027584UL, 3389028607UL},
		{3389026048UL, 3389026303UL},
		{3389017856UL, 3389018111UL},
		{3233590272UL, 3233590527UL},
		{3542089728UL, 3543400447UL},
		{3537371136UL, 3537895423UL},
		{3536584704UL, 3536846847UL},
		{3534880768UL, 3535298559UL},
		{3529113600UL, 3531603967UL},
		{3389957376UL, 3389957631UL},
		{3389941504UL, 3389941759UL},
		{3389415168UL, 3389415423UL},
		{3534749696UL, 3534815231UL},
		{3528980480UL, 3528982527UL},
		{3414417408UL, 3414425599UL},
		{3412656128UL, 3412672511UL},
		{3411730432UL, 3411738623UL},
		{3411017728UL, 3411476479UL},
		{3414417408UL, 3414425599UL},
		{3412656128UL, 3412672511UL},
		{3411730432UL, 3411738623UL},
		{3411017728UL, 3411476479UL},
		{3414417408UL, 3414425599UL},
		{3412656128UL, 3412672511UL},
		{3411730432UL, 3411738623UL},
		{3411017728UL, 3411476479UL},
		{3389971968UL, 3389972223UL},
		{3389935616UL, 3389935871UL},
		{3389932544UL, 3389932799UL},
		{3389932288UL, 3389932543UL},
		{3389785088UL, 3389786111UL},
		{3389489152UL, 3389497343UL},
		{3389300480UL, 3389317119UL},
		{3389145088UL, 3389153279UL},
		{3389136896UL, 3389142015UL},
		{3389064704UL, 3389065215UL},
		{3389043712UL, 3389044735UL},
		{3389038592UL, 3389043711UL},
		{3389030400UL, 3389035519UL},
		{3389028864UL, 3389029375UL},
		{3389026304UL, 3389026559UL},
		{3389024256UL, 3389024511UL},
		{3389023232UL, 3389023487UL},
		{3389017344UL, 3389017855UL},
		{3329272064UL, 3329272319UL},
		{3237335040UL, 3237335295UL},
		{3234814720UL, 3234814975UL},
		{3234588672UL, 3234588927UL},
		{2815950848UL, 2816016383UL},
		{2705326080UL, 2705391615UL},
		{2662727680UL, 2662793215UL},
		{2659450880UL, 2659516415UL},
		{2466775040UL, 2466840575UL},
		{2429943808UL, 2430009343UL},
		{2404974592UL, 2405040127UL},
		{2310864896UL, 2310930431UL},
		{180813824UL,  184418303UL},
		{3530844672UL,	3530844927UL},
		{3555278848UL,	3555279103UL},
		{3542863872UL,	3542864127UL},
		{3733454848UL,	3733979135UL},
		{1038352384UL,	1038614527UL},
		{3708125184UL,	3708157951UL},
		{3659005952UL,	3659137023UL},
		{3542863908UL,	3542863908UL},


	};
	char org_IP[20];
	int i_Ip[4];
	unsigned int i;
	
	strcpy(org_IP,u_IP);
	IPtoi(org_IP,&i_Ip[0],&i_Ip[1],&i_Ip[2],&i_Ip[3]);

	unsigned int nIPAddr	= ((unsigned int)i_Ip[0] << 24)
							| ((unsigned int)i_Ip[1] << 16)
							| ((unsigned int)i_Ip[2] <<  8)
							| ((unsigned int)i_Ip[3]      );
	
	if (i_Ip[0] == 211 && i_Ip[1] == 181 && i_Ip[2] == 248) //나코 피씨방은 풀어주자
		return true;

	for (i = 0; i < sizeof(tableBlockIP) / sizeof(unsigned int); i++)
	{
		if (tableBlockIP[i][0] <= nIPAddr && nIPAddr <= tableBlockIP[i][1])
			return false;
	}

	return true;
}

#else

bool CheckIP(const char *u_IP)
{
	return true;
}

#endif
