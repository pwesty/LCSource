#ifndef LOG_DAMAGE_INFO
#define LOG_DAMAGE_INFO

#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <vector>
#include <time.h>
#include <Engine/Base/CTString.h>

class ClogInfo
{
protected:
	bool start;
	time_t startTime;
	time_t endTime;
	FILE *fLog;
public:
	ClogInfo() : start(false), startTime(0), endTime(0), fLog(NULL)	{ }

	virtual ~ClogInfo()
	{
		if (fLog != NULL)
		{
			fclose(fLog);
		}
	}

	bool CreateLogFile(char* strName)
	{
		fLog = fopen(strName, "wt");
		
		if (fLog == NULL)
		{
			return false;
		}

		start = true;

		return true;
	}

	void CloseLogFile(void)
	{
		if (fLog)
		{
			fclose(fLog);
		}

		start = false;
		fLog = NULL;
	}

	void PutString(const char* str)
	{
		if (fLog)
		{
			fprintf(fLog, "%s", str);
			fflush(fLog);
		}
	}

	time_t GetStartTime(void) { return startTime; }
	time_t GetEndTime(void) { return endTime; }

	bool GetStart(void) { return start; }

	virtual void clear()
	{
		start = false; startTime = 0; endTime = 0;
	}
};

#define HITTYPE_MISS	(1 << 0) // 회피
#define HITTYPE_WEAK	(1 << 1) // 약하게
#define HITTYPE_NORMAL	(1 << 2) // 보통
#define HITTYPE_STRONG	(1 << 3) // 강하게
#define HITTYPE_HARD	(1 << 4) // 매우 강하게
#define HITTYPE_CRITICAL (1 << 5) // 크리티컬
#define HITTYPE_DEADLY	(1 << 6) // 죽도록 강하게

class ENGINE_API CDamageLogInfo : public ClogInfo
{
public:
	struct LogEntityInfo 
	{
		int hp, mp, prehp, premp, ServerID, ClientID, db_Index, level;
		char strName[64];

		LogEntityInfo() : hp(0), mp(0), prehp(0), premp(0), ServerID(0), ClientID(0), db_Index(0), level(0)
		{
			memset(strName, 0, 64);
		}
		LogEntityInfo(const LogEntityInfo& r)
		{
			(*this) = r;
		}

		void operator = (const LogEntityInfo& r)
		{
			hp = r.hp; mp = r.mp; prehp = r.prehp; premp = r.premp;
			ServerID = r.ServerID; ClientID = r.ClientID; db_Index = r.db_Index; level = r.level;
			strcpy(strName, r.strName);
		}
	};

	struct LogDamage 
	{
		LogEntityInfo att_EntityInfo;
		LogEntityInfo tar_EntityInfo;
		unsigned char dam_Type;	// bit flag
		int dam_Amount;
		time_t dam_Time;
		bool bWrited;

		LogDamage() : dam_Type(0), dam_Amount(0), dam_Time(0), bWrited(false) {}
		LogDamage (const LogDamage& r)
		{
			(*this) = r;
		}

		void operator = (const LogDamage& r)
		{
			att_EntityInfo = r.att_EntityInfo;
			tar_EntityInfo = r.tar_EntityInfo;
			dam_Type = r.dam_Type;
			dam_Amount = r.dam_Amount;
			dam_Time = r.dam_Time;
		}

		void SetTime(void)
		{
			time(&dam_Time);
		}
	};
public:
	CDamageLogInfo() : m_DPS(0) { memset(m_LogName, 0, 64); }
	virtual ~CDamageLogInfo() { clear(); }

	virtual void clear()
	{
		memset(m_LogName, 0, 64);
		m_listLogDamage.clear();
	}

	bool writeInfo(LogDamage& In);
	bool writeInfo(const LogEntityInfo& In, bool bStart=false);

	void AddDamage(LogDamage In)
	{
		// 첫번째 Damage메세지를 start time으로 결정
		if (m_listLogDamage.empty())
		{
			time(&startTime);
			//struct tm* timeinfo;
			//timeinfo = localtime(&startTime);

			/*fprintf(fLog, "/********* StartTime: %d년 %d월 %d일 %d시간 %d분 %d초 ***********\n",
				timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
			fflush(fLog);*/

			//writeInfo(In.att_EntityInfo);
			//writeInfo(In.tar_EntityInfo, true);
			PutString("/******************************************************************/\n");
			fprintf(fLog, "Att_Name\tTar_Name\tDam_Type\tDam_Amount\n");
		}

		writeInfo(In);

		m_listLogDamage.push_back(In);
	}

	bool StartDamageMode(const char* logName = NULL);
	void EndDamageMode(void);
	void SetLogName(const char* names) { strcpy(m_LogName, names); }
	void CalculateDPS(void);

protected:
	std::vector<LogDamage> m_listLogDamage;
	char m_LogName[64];
	float m_DPS; // 초당 데미지 량 (총데미지/시간(초))
};

#endif