#ifndef  __GAME_UTILS_H__
#define  __GAME_UTILS_H__

#include <string>
#include <time.h>
#include "../ShareLib/LCString.h"

// 널 타임
extern struct timeval a_NullTime;

// 나중에 지워야할 전역 버퍼
extern char		g_buf[MAX_STRING_LENGTH];
extern char		g_buf2[MAX_STRING_LENGTH];
extern char		g_buf3[MAX_STRING_LENGTH];


char* __ltoa(int value, char *string);
char* __ltoa(LONGLONG value, char *string);
char* __ftoa(float f, char *buf, int place);
void MakeMath();
float get_sin(float radian);
float get_cos(float radian);
void GetTimeofDay(struct timeval *t, struct timezone *dummy);
void TimeDiff(struct timeval *rslt, struct timeval *a, struct timeval *b);
void TimeAdd(struct timeval *rslt, struct timeval *a, struct timeval *b);
float GetDistance(float x1, float x2, float z1, float z2, float h1, float h2);
const char* SkipSpaces(const char* string);
const char* SkipSpecialCharacter(const char* string);
char* RemoveTailSpaces(char* string);
char* TrimString(char* string);
const char* AnyOneArg(const char* argument, char* first_arg, bool toLower = false);
const char* GetToken(const char* arg1, char* arg2, bool toLower);
#ifndef __BILLING_SERVER__
char* StrDup(const char* source);
char* StrCat(char* dest, const char* src);
#endif
char* IntCat(char *dest, int i, bool bInsertSpace = true);
char* IntCat(char *dest, LONGLONG ll, bool bInsertSpace = true);
char* IntCat0(char* dest, int i, int width, bool bInsertSpace = true);
char* IntCat0(char* dest, LONGLONG i, int width, bool bInsertSpace = true);
char* FloatCat(char *dest, float f, int place, bool bInsertSpace = true);
bool strinc(const char *str, const char *inc);
void IPtoi(const char *org_IP,int *i_one, int *i_two, int *i_thr, int *i_for);

int GetRandom(int from, int to);

bool CheckIP(const char *u_IP);

char* TransDateStr(int time, char* convbuf);	//0627 // 광호 수정

// [selo: 101115] 러시아어 이외의 글자가 있는지 확인
bool CheckNoRussianCharacter(const CLCString& str);

///////////////////////////////////
// 매크로 & 인라인 함수들

#define MAX(a, b)	((a > b) ? a : b)										// 최대
#define MIN(a, b)	((a < b) ? a : b)										// 최소
#define LOWER(c)   (((c)>='A'  && (c) <= 'Z') ? ((c)+('a'-'A')) : (c))		// 소문자로
#define UPPER(c)   (((c)>='a'  && (c) <= 'z') ? ((c)+('A'-'a')) : (c) )		// 대문자로
#define ABS(c)		((c) < 0 ? -(c) : (c))									// 절대값
#define SIGN(i)		((i) < 0 ? (-1) : (1))									// 부호
#define ISNEWL(ch) ((ch) == '\n' || (ch) == '\r')							// 개행 문자 검사
#define is_space_in_util(c)		(c == 0x20 || (c >= 0x09 && c <= 0x0D))				// 공백 검사
#define is_special_in_util(c)	( !(c >=0x30 && c <= 0x39) && !(c >= 0x41 && c <= 0x5A) && !(c >= 0x61 && c <= 0x7A) ) // 숫자, 영문자를 제외한 특수문자 검사


// LINKED LIST MANAGEMENT

// REMOVE NODE FROM LIST
#define REMOVE_FROM_BILIST(item, head, before, next) { \
			if (item->before) { \
				item->before->next = item->next; \
				if (item->next) { \
					item->next->before = item->before; \
				} \
			} else { \
				head = item->next; \
				if (item->next) { \
					item->next->before = NULL; \
				} \
			} \
			item->before = NULL; \
			item->next = NULL; }

// ADD NODE TO LIST
#define ADD_TO_BILIST(item, head, before, next) { \
			if (head) { \
				head->before = item; \
			} \
			item->next = head; \
			item->before = NULL; \
			head = item; }


////////////////////
// Function name	: Rad2Deg
// Description	    : 라디안을 도 단위 각도로 변환
// Return type		: float
//					: 도 단위 각도
// Argument         : float rad
//					: 라디안 각도
inline float Rad2Deg(float rad)
{
	float deg = rad * 180 / PI;
	if (deg > 180)
		deg -= 360;
	else if (deg < -180)
		deg += 360;
	return deg;
}


////////////////////
// Function name	: Deg2Rad
// Description	    : 도 단위 각도를 라디안으로 변환
// Return type		: float
//					: 라디안 각도
// Argument         : float deg
//					: 도 단위 각도
inline float Deg2Rad(float deg)
{
	if (deg > 180)
		deg -= 360;
	else if (deg < -180)
		deg += 360;

	return deg * PI / 180;
}

#ifdef __BILLING_SERVER__
////////////////////
// Function name	: DT2TM
// Description	    : DBTIMESTAMP에서 struct tm으로 변환 (분단위만)
inline struct tm DT2TM(DBTIMESTAMP dt)
{
	struct tm ret;
	memset(&ret, 0, sizeof(ret));
	ret.tm_year = dt.year - 1900;
	ret.tm_mon = dt.month - 1;
	ret.tm_mday = dt.day;
	ret.tm_hour = dt.hour;
	ret.tm_min = dt.minute;
	return ret;
}
#endif

inline struct tm NOW()
{
	time_t t;
	time(&t);
	struct tm ret;
	memcpy(&ret, localtime(&t), sizeof(ret));
	return ret;
}

// nSizeOfDest는 NULL을 포함
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

inline void StrUpr(char* src)
{
	while (*src)
	{
		if( *src >= 97 && *src <= 122 )
		{
			*src -= 32;
		}
		src++;
	}
}

inline void StrLwr(char* src)
{
	while (*src)
	{
		if( *src >= 65 && *src <= 90 )
		{
			*src += 32;
		}
		src++;
	}
}

#ifdef CIRCLE_WINDOWS
#define strcmp2(a, b)	_stricmp(a, b)
#else
#define strcmp2(a, b)	strcasecmp(a, b)
#endif

#ifdef CHECK_LIMIT_AGE
#ifdef JUMIN_DB_CRYPT
#ifdef KOR_SHUTDOWN
bool CheckBirthDay(const char* strJumin, int nFirst, int nLimitAge, bool& bShutdown);
#else
bool CheckBirthDay(const char* strJumin, int nFirst, int nLimitAge);
#endif
#else
bool CheckBirthDay(const char* strJumin, int nLimitAge);
#endif
#endif // CHECK_LIMIT_AGE

#ifdef CASH_ITEM_COUPON
struct CASH_COUPON
{
	CLCString	name;
	int			id;
	CLCString	serial;
	int			limitAmount;
	int			amount;
	CASH_COUPON():name(51),serial(21)
	{
		id=0;
		amount=0;
		limitAmount=0;
	}
};
#endif //CASH_ITEM_COUPON

struct RECTF
{
	float left;
	float top;
	float right;
	float bottom;
	RECTF()
	{
		left=top=right=bottom=0.0f;
	}
};

struct POSF
{
	float x;
	float y;
	POSF()
	{
		x=y=0.0f;
	}
};

#endif
//

