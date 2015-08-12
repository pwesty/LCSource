#ifndef  __GAME_UTILS_H__
#define  __GAME_UTILS_H__

#include "LCString.h"
#include "LCList.h"

#include "Character.h"
float GetDistance(CCharacter* ch, CCharacter* tch);
float GetDistance(float x, float z, float h, CCharacter* tch);
void GetSerial(char *buf, int index);
void GetSerial(CLCString& buf, int index);

//char* __ltoa(int value, char *string)
//{
//	char p_buf[128];
//	int idx, i;
//
//	// 초기화
//	*string = '\0';
//	idx = 0;
//	i = 0;
//
//	// 음수면 -부호를 붙이고 value는 양수로 무조건
//	if (value < 0)
//	{
//		strcpy(string, "-");
//		value = -value;
//		i = 1;
//	}
//	if (value == 0)
//	{
//		strcpy(string, "0");
//		return string;
//	}
//
//	// 뒷자리부터 문자열로 변경
//	while (value)
//	{
//		switch (value % 10)
//		{
//		case 0:
//		case 1:
//		case 2:
//		case 3:
//		case 4:
//		case 5:
//		case 6:
//		case 7:
//		case 8:
//		case 9:
//			p_buf[idx++] = (char)((value % 10) + '0');
//			break;
//		default:
//			p_buf[idx++] = '0';
//			break;
//		}
//
//		value /= 10;
//	}
//
//	while (idx)
//	{
//		string[i++] = p_buf[--idx];
//	}
//	string[i++] = '\0';
//
//	return string;
//}
//
//char* __ftoa(float f, char *buf, int place)
//{
//	char p_buf[128];
//
//	*buf = '\0';
//
//	// 소수점 앞자리 문자열로
//	// 양수 음수 가려서
//	if (f < 0)
//	{
//		// 음수면 양수로 만들고
//		f = -f;
//		__ltoa((int)f, p_buf);
//		// 문자열 앞에 -를 붙이고
//		strcpy(buf, "-");
//		strcat(buf, p_buf);
//	}
//	else
//	{
//		__ltoa((int)f, buf);
//	}
//
//	// 소수점 몇자리 까지?
//	if (place <= 0)
//	{
//		return buf;
//	}
//
//	// 소수점 8자리까지만
//	if (place > 8)
//		place = 8;
//
//	// 소수점 뒤부분만 남기고
//	f -= (int)f;
//
//	// 원하는 소수 자리까지 곱해서
//	while (place > 0)
//	{
//		f *= 10;
//		place--;
//	}
//
//	// 정수 부분만 다시 변환
//	__ltoa((int)f, p_buf);
//	strcat(buf, ".");
//	strcat(buf, p_buf);
//
//	return buf;
//}


#endif
