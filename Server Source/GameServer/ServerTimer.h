#ifndef __SERVER_TIMER_H__
#define __SERVER_TIMER_H__

#include "../ShareLib/rnsocketiohandler.h"
#include "../ShareLib/rnsocketioservice.h"

// 1초
class ServerTimerPerSecond : public rnSocketIOHandler
{
public:
	ServerTimerPerSecond() {}
	~ServerTimerPerSecond() {}

	void Init();
	virtual void operate(rnSocketIOService* service);

	static ServerTimerPerSecond* instance();
};

//////////////////////////////////////////////////////////////////////////
// 1/10초
class ServerTimerPerHundredMSecond : public rnSocketIOHandler
{
public:
	ServerTimerPerHundredMSecond() {}
	~ServerTimerPerHundredMSecond() {}

	void Init();
	virtual void operate(rnSocketIOService* service);

	static ServerTimerPerHundredMSecond* instance();
};

//////////////////////////////////////////////////////////////////////////
// 1분
class ServerTimerPerMinute : public rnSocketIOHandler
{
public:
	ServerTimerPerMinute() {}
	~ServerTimerPerMinute() {}

	void Init();
	virtual void operate(rnSocketIOService* service);

	static ServerTimerPerMinute* instance();
};

//////////////////////////////////////////////////////////////////////////
// 매 시
class ServerTimerPerHour : public rnSocketIOHandler
{
public:
	ServerTimerPerHour() {}
	~ServerTimerPerHour() {}

	void Init();
	virtual void operate(rnSocketIOService* service);

	static ServerTimerPerHour* instance();
};

//////////////////////////////////////////////////////////////////////////
// 자정
class ServerTimerPerMidnight : public rnSocketIOHandler
{
public:
	ServerTimerPerMidnight() {}
	~ServerTimerPerMidnight() {}

	void Init();
	virtual void operate(rnSocketIOService* service);

	static ServerTimerPerMidnight* instance();
};

//////////////////////////////////////////////////////////////////////////
// endgame(GM 명령어) 처리
class EndGameTimer : public rnSocketIOHandler
{
public:
	EndGameTimer() : run_flag_(false) {}
	~EndGameTimer() {}

	bool isRun()
	{
		return run_flag_;
	}
	void Init(int sec);
	virtual void operate(rnSocketIOService* service);

	static EndGameTimer* instance();

private:
	bool		run_flag_;
};

//////////////////////////////////////////////////////////////////////////
// shutdown(GM 명령어) 처리
class ShutdownTimer : public rnSocketIOHandler
{
public:
	ShutdownTimer() : run_flag_(false), remain_sec_(0) {}
	~ShutdownTimer() {}

	bool isRun()
	{
		return run_flag_;
	}
	void Init(int sec);
	virtual void operate(rnSocketIOService* service);

	static ShutdownTimer* instance();

private:
	bool		run_flag_;
	int			remain_sec_;
};

//////////////////////////////////////////////////////////////////////////
// shutdown(GM 명령어) 처리
class RebootTimer : public rnSocketIOHandler
{
public:
	RebootTimer() : run_flag_(false), remain_sec_(0) {}
	~RebootTimer() {}

	bool isRun()
	{
		return run_flag_;
	}
	void Init(int sec);
	virtual void operate(rnSocketIOService* service);

	static RebootTimer* instance();

private:
	bool		run_flag_;
	int			remain_sec_;
};

#endif
