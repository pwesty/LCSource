#ifndef SE_INCL_TIMER_H
#define SE_INCL_TIMER_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#ifndef _MT
#error Multithreading support is required!
#endif

#include <Engine/Base/Lists.h>
#include <Engine/Base/Synchronization.h>

#define TARGET_RESOLUTION 1 // 1 - millisecond target resolution

//안태훈 수정 시작	//(Add Sun Moon Entity and etc)(0.2)
#define RWTOneSec2GWTSec (60.0f)
#define GWTOneSec2RWTSec (1.0f / RWTOneSec2GWTSec)

//value는 game world time임.
#define GAMETIME_ONEMINUTE_SECOND	(60)
#define GAMETIME_ONEHOUR_MINUTE		(60)
#define GAMETIME_ONEHOUR_SECOND		(GAMETIME_ONEHOUR_MINUTE*GAMETIME_ONEMINUTE_SECOND)
#define GAMETIME_ONEDAY_HOUR		(30)
#define GAMETIME_ONEDAY_MINUTE		(GAMETIME_ONEDAY_HOUR*GAMETIME_ONEHOUR_MINUTE)
#define GAMETIME_ONEDAY_SECOND		(GAMETIME_ONEDAY_HOUR*GAMETIME_ONEHOUR_MINUTE*GAMETIME_ONEMINUTE_SECOND)
#define GAMETIME_NIGHT_HOUR			(10)
#define GAMETIME_NOONDAY_HOUR		(GAMETIME_ONEDAY_HOUR-GAMETIME_NIGHT_HOUR)
#define GAMETIME_NIGHT_MINUTE		(GAMETIME_NIGHT_HOUR*GAMETIME_ONEHOUR_MINUTE)
#define GAMETIME_NOONDAY_MINUTE		(GAMETIME_NOONDAY_HOUR*GAMETIME_ONEHOUR_MINUTE)
#define GAMETIME_NIGHT_SECOND		(GAMETIME_NIGHT_HOUR*GAMETIME_ONEHOUR_SECOND)
#define GAMETIME_NOONDAY_SECOND		(GAMETIME_NOONDAY_HOUR*GAMETIME_ONEHOUR_SECOND)
#define GAMETIME_MORNING_HOUR		(5)
#define GAMETIME_MORNING_MINUTE		(GAMETIME_MORNING_HOUR*GAMETIME_ONEHOUR_MINUTE)
#define GAMETIME_MORNING_SECOND		(GAMETIME_MORNING_HOUR*GAMETIME_ONEHOUR_SECOND)
//안태훈 수정 끝	//(Add Sun Moon Entity and etc)(0.2)

// current game time 
ENGINE_API extern TIME _CurrentTickTimer;

/*
 * Class that holds and manipulates with high-precision timer values.
 */
class CTimerValue {
public:
  __int64 tv_llValue;       // 64 bit integer (MSVC specific!)
  /* Constructor from quad integer. */
  inline CTimerValue(__int64 llValue) : tv_llValue(llValue) {};
public:
  /* Constructor. */
  inline CTimerValue(void) {};
  /* Constructor from seconds. */
  inline CTimerValue(double dSeconds);
  /* Clear timer value (set it to zero). */
  inline void Clear(void);
  /* Addition. */
  inline CTimerValue &operator+=(const CTimerValue &tvOther);
  inline CTimerValue operator+(const CTimerValue &tvOther) const;
  /* Substraction. */
  inline CTimerValue &operator-=(const CTimerValue &tvOther);
  inline CTimerValue operator-(const CTimerValue &tvOther) const;
  /* Comparisons. */
  inline BOOL operator<(const CTimerValue &tvOther) const;
  inline BOOL operator>(const CTimerValue &tvOther) const;
  inline BOOL operator<=(const CTimerValue &tvOther) const;
  inline BOOL operator>=(const CTimerValue &tvOther) const;
  /* Get the timer value in seconds. - use for time spans only! */
  inline double GetSeconds(void);
  /* Get the timer value in milliseconds as integral value. */
  inline __int64 GetMilliseconds(void);
};
// a base class for hooking on timer interrupt
class CTimerHandler {
public:
  CListNode th_Node;
public:
  virtual ~CTimerHandler(void) {}  /* rcg10042001 */
  /* This is called every TickQuantum seconds. */
  ENGINE_API virtual void HandleTimer(void)=0;
};

// class for an object that maintains global timer(s)
class ENGINE_API CTimer {
// implementation:
public:

  __int64 tm_llPerformanceCounterFrequency; // frequency of Win32 performance counter
  __int64 tm_llCPUSpeedHZ;  // CPU speed in HZ

  CTimerValue tm_tvLastTimeOnTime;  // last time when timer was on time
  TIME        tm_tmLastTickOnTime;  // last tick when timer was on time
  //! tick의 합.실제 경과한 시간.
  TIME tm_RealTimeTimer;  // this really ticks at 1/TickQuantum frequency
  TIME tmTimeDelay;			// calculate how long has passed since we have last been on time
  TIME tmTickDelay;	
  FLOAT tm_fLerpFactor;   // factor used for lerping between frames
  FLOAT tm_fLerpFactor2;  // secondary lerp-factor used for unpredicted movement

  ULONG tm_TimerID;       // windows timer ID

  CTCriticalSection tm_csHooks;   // access to timer hooks
  CListHead         tm_lhHooks;   // a list head for timer hooks
  BOOL tm_bInterrupt;       // set if interrupt is added

  UINT uPeriod;
  
  //0216
  CTimerValue tm_tvStartTime;

//안태훈 수정 시작	//(Add Sun Moon Entity and etc)(0.1)
	void StartTimer()
	{
		//0216
		tm_tvStartTime = GetHighPrecisionTimer();
	}
	CTimerValue GetOffsetFromStart()
	{
		return GetHighPrecisionTimer() - tm_tvStartTime;
	}
//안태훈 수정 끝	//(Add Sun Moon Entity and etc)(0.1)

// interface:
public:
  // interval defining frequency of the game ticker
  static const TIME TickQuantum;		// 20 ticks per second

  /* Constructor. */
  CTimer(BOOL bInterrupt=TRUE);
  /* Destructor. */
  ~CTimer(void);
  /* Add a timer handler. */
  void AddHandler(CTimerHandler *pthNew);
  /* Remove a timer handler. */
  void RemHandler(CTimerHandler *pthOld);
  /* Handle timer handlers manually. */
  void HandleTimerHandlers(void);

  /* Set the real time tick value. */
  void SetRealTimeTick(TIME tNewRealTimeTick);
  /* Get the real time tick value. */
  TIME GetRealTimeTick(void) const;

  /* NOTE: CurrentTick is local to each thread, and every thread must take
     care to increment the current tick or copy it from real time tick if
     it wants to make animations and similar to work. */

  /* Set the current game tick used for time dependent tasks (animations etc.). */
  void SetCurrentTick(TIME tNewCurrentTick);
  /* Get current game time, always valid for the currently active task. */
  __forceinline const TIME CurrentTick(void) const { 
    ASSERT(this!=NULL);
    return _CurrentTickTimer;
  };
  /* Get lerped game time. */
  __forceinline const TIME GetLerpedCurrentTick(void) const {
    ASSERT(this!=NULL);
    return _CurrentTickTimer+tm_fLerpFactor*TickQuantum;
  };

  // Set factor for lerping between ticks.
  void SetLerp(FLOAT fLerp);    // sets both primary and secondary
  void SetLerp2(FLOAT fLerp);   // sets only secondary
  // Disable lerping factor (set both factors to 1)
  void DisableLerp(void);
  // Get current factor used for lerping between game ticks.
  inline FLOAT GetLerpFactor(void) const { return tm_fLerpFactor; };
  // Get current factor used for lerping between game ticks.
  inline FLOAT GetLerpFactor2(void) const { return tm_fLerpFactor2; };

  /* Get current timer value of high precision timer. */
  CTimerValue GetHighPrecisionTimer(void) ;
};

// pointer to global timer object
ENGINE_API extern CTimer *_pTimer;

//안태훈 수정 시작	//(Add Sun Moon Entity and etc)(0.2)
ENGINE_API extern float g_fGWTime;			// Game world time
ENGINE_API extern float g_fGWTimeMul;			// Game world time's speed
//안태훈 수정 끝	//(Add Sun Moon Entity and etc)(0.2)

// convert a time value to a printable string (hh:mm:ss)
ENGINE_API CTString TimeToString(FLOAT fTime);

#include <Engine/Base/Timer.inl>


#endif  /* include-once check. */

