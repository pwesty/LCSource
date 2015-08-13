#pragma once

#define SHOW_LOG

// 사용 예:
// LOG("simple type");
// LOG("printf type : %s, %d", 문자열 포인터, 정수);
// ASSERT(0)
// ASSERT(정수형변수, "simple type");
// ASSERT(정수형변수, ""printf type : %s, %d", 문자열 포인터, 정수);
// ASSERT((정수형변수>0), ""printf type : %s, %d", 문자열 포인터, 정수);
// INT3(0)  강제 인트럽트 건다. 바로 코드로 갈수 있어 편함
// VINT3(min,max,value) 값이 범위를 벗어나면 인트럽트를 건다.

#ifdef SHOW_LOG
class Trace
{
public:
	Trace(const char* pFile, int nRet);

	void Log(char* pStrF, ...);
	void Log(int level, char* pData, ...);
	void Watch(int line, char* pData, ...);
	void Watch(int level, int line, char* pData, ...);
	void Assert(BOOL bFlag, char *pStrF, ...);
	void Interrupt(BOOL bFlag);
	void ValueInterrupt(float fmin, float fmax, float fvalue);

	void Dump(const char* name, const char* buf, unsigned int bufLen);
	void Dump(int level, const char* name, const char* buf, unsigned int bufLen);

private:
	const char* m_pFileName;
	int m_nLineNumber;
};
#endif

#ifdef SHOW_LOG
	#define LOG			Trace(__FILE__, __LINE__).Log
	#define WATCH		Trace(__FILE__, __LINE__).Watch
	#define ASSERT0		Trace(__FILE__, __LINE__).Assert
	#define INT3		Trace(__FILE__, __LINE__).Interrupt
	#define VINT3		Trace(__FILE__, __LINE__).ValueInterrupt
	#define LOG2(f,l)	Trace(f,l).Log
	#define LOG_DUMP	Trace(__FILE__, __LINE__).Dump
#else
	#if _MSC_VER >= 1400
		#define LOG			__noop
		#define WATCH		__noop
		#define ASSERT0		__noop
		#define INT3		__noop
		#define VINT3		__noop
		#define LOG2(f,l)	__noop
		#define LOG_DUMP	__noop
	#else
		#define LOG			(void)0
		#define WATCH		(void)0
		#define ASSERT0		(void)0
		#define INT3		(void)0
		#define VINT3		(void)0
		#define LOG2(f,l)	(void)0
		#define LOG_DUMP	(void)0
	#endif
#endif

#ifdef SHOW_LOG

#define TRACETOOLLIB_NAME "TraceTool"

#if (_MSC_VER == 1600) //VC10.0
	#define TRACETOOLLIB_VS_VERSION "VC100"
#elif (_MSC_VER == 1500) //VC9.0
	#define TRACETOOLLIB_VS_VERSION "VC90"
#elif (_MSC_VER == 1400) //VC8.0
	#define TRACETOOLLIB_VS_VERSION "VC80"
#elif (_MSC_VER == 1200) //VC6.0
	#define TRACETOOLLIB_VS_VERSION "VC60"
#else
	#error Unsupported version of Visual Studio
#endif

#ifdef _DLL
	#ifdef _DEBUG
		#define TRACETOOLLIB_SUFFIX "D.lib"
	#else
		#define TRACETOOLLIB_SUFFIX "R.lib"
	#endif
#else
	#ifdef _DEBUG
		#define TRACETOOLLIB_SUFFIX "DS.lib"
	#else
		#define TRACETOOLLIB_SUFFIX "RS.lib"
	#endif
#endif

#pragma comment(lib, TRACETOOLLIB_NAME TRACETOOLLIB_VS_VERSION TRACETOOLLIB_SUFFIX)

#endif //SHOW_LOG
