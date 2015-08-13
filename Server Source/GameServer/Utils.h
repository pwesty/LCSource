#ifndef  __GAME_UTILS_H__
#define  __GAME_UTILS_H__

#include <string>
#include <time.h>
#include "../ShareLib/LCString.h"

#include "Character.h"
float GetDistance(CCharacter* ch, CCharacter* tch);
float GetDistance(float x, float z, float h, CCharacter* tch);
std::string GetSerial();

// 널 타임
extern struct timeval a_NullTime;


char* __ltoa(int value, char *string);
char* __ltoa(LONGLONG value, char *string);
char* __ftoa(float f, char *buf, int place);
void MakeMath();
float get_sin(float radian);
float get_cos(float radian);
void GetTimeofDay(struct timeval *t, struct timezone *dummy);
void TimeDiff(struct timeval *rslt, struct timeval *a, struct timeval *b);
float GetDistance(float x1, float x2, float z1, float z2, float h1, float h2);
const char* SkipSpaces(const char* string);
const char* SkipSpecialCharacter(const char* string);
char* TrimString(char* string);
void TrimString(std::string& data);
const char* AnyOneArg(const char* argument, char* first_arg, bool toLower = false);
const char* GetToken(const char* arg1, char* arg2, bool toLower);
char* IntCat(char *dest, int i, bool bInsertSpace = true);
char* IntCat(char *dest, LONGLONG ll, bool bInsertSpace = true);
char* IntCat0(char* dest, int i, int width, bool bInsertSpace = true);
char* IntCat0(char* dest, LONGLONG i, int width, bool bInsertSpace = true);
bool strinc(const char *str, const char *inc);
void IPtoi(const char *org_IP,int *i_one, int *i_two, int *i_thr, int *i_for);

int GetRandom(int from, int to);

bool CheckIP(const char *u_IP);


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

struct tm NOW();

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

bool isToday(time_t unixtime);

char* findPercentChar(char* buf);

#endif
//
