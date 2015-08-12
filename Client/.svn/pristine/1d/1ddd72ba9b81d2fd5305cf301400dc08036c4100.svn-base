#include "stdh.h"
#include <Engine/Base/DamageLogInfo.h>

bool CDamageLogInfo::writeInfo(LogDamage& In)
{
	if (fLog)
	{
		char strDamageType[32];
		memset(strDamageType, 0, 32);

		time(&In.dam_Time);
		//struct tm* timeinfo;
		//timeinfo = localtime(&In.dam_Time);

		switch(In.dam_Type)
		{
		case HITTYPE_MISS:
			strcpy(strDamageType, "MISS");
			break;
		case HITTYPE_WEAK:
			strcpy(strDamageType, "WEAK");
			break;
		case HITTYPE_NORMAL:
			strcpy(strDamageType, "NORMAL");
			break;
		case HITTYPE_STRONG:
			strcpy(strDamageType, "STRONG");
			break;
		case HITTYPE_HARD:
			strcpy(strDamageType, "HARD");
			break;
		case HITTYPE_CRITICAL:
			strcpy(strDamageType, "CRITICAL");
			break;
		case HITTYPE_DEADLY:
			strcpy(strDamageType, "DEADLY");
			break;
		default: // None type
			break;
		}

		/*fprintf(fLog, "Att_Name: %s, Tar_Name: %s, Dam_Type: %s, Dam_Amount: %d, Time: %d년 %d월 %d일 %d시간 %d분 %d초\n",
			In.att_EntityInfo.strName,
			In.tar_EntityInfo.strName,
			strDamageType,
			In.dam_Amount,
			timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec); // damage info*/
		fprintf(fLog, "%s\t%s\t%s\t%d\n", In.att_EntityInfo.strName, In.tar_EntityInfo.strName, strDamageType, In.dam_Amount);

		fflush(fLog);
		return true;
	}

	return false;
}

bool CDamageLogInfo::writeInfo(const LogEntityInfo& In, bool bStart)
{
	if (fLog)
	{
		if (bStart)
		{
			fprintf(fLog, "S_ID: %d, C_ID: %d, DB_ID: %d, HP: %d, MP: %d, Name: %s, Level: %d\n",
				In.ServerID,
				In.ClientID,
				In.db_Index,
				In.prehp, In.premp, In.strName, In.level);			
		}
		else
		{
			fprintf(fLog, "S_ID: %d, C_ID: %d, DB_ID: %d, HP: %d, MP: %d, Name: %s, Level: %d\n",
				In.ServerID,
				In.ClientID,
				In.db_Index,
				In.hp, In.mp, In.strName, In.level);
		}

		fflush(fLog);
		return true;
	}

	return false;
}

bool CDamageLogInfo::StartDamageMode(const char* logName)
{
	if (logName != NULL)
	{
		SetLogName(logName);
	}

	return CreateLogFile(m_LogName);
}

void CDamageLogInfo::EndDamageMode(void) 	// attacker or target Entity가 죽을 경우까지
{
	PutString("/******************************************************************/\n");

//	std::vector<LogDamage>::reverse_iterator revItr = m_listLogDamage.rbegin();
//	LogDamage EndDamage = (*revItr);

	time(&endTime);
	//struct tm* timeinfo;
	//timeinfo = localtime(&endTime);

/*	fprintf(fLog, "/********* EndTime: %d년 %d월 %d일 %d시간 %d분 %d초 ***********\n"
		, timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);*/
	fprintf(fLog, "Total Damage\tInterval Time\tDPS\tAtt_HP\tAtt_Mp\tCritical Count\tDeadly Count\tEtc Count\n");
	fflush(fLog);
	//writeInfo(EndDamage.att_EntityInfo);
	//writeInfo(EndDamage.tar_EntityInfo);
	CalculateDPS();
	CloseLogFile();
	clear();
}

void CDamageLogInfo::CalculateDPS()
{
	DWORD totalDamage = 0;
	time_t intervalTime = endTime - startTime;
	int Critical = 0, Deadly = 0;

	std::vector<LogDamage>::reverse_iterator revItr = m_listLogDamage.rbegin();
	LogDamage EndDamage = (*revItr);
	std::vector<LogDamage>::iterator Itr_Damage;

	for (Itr_Damage = m_listLogDamage.begin(); Itr_Damage != m_listLogDamage.end(); ++Itr_Damage)
	{
		LogDamage tmpDamage = (*Itr_Damage);
		totalDamage += tmpDamage.dam_Amount;

		if (tmpDamage.dam_Type == HITTYPE_CRITICAL) { ++Critical; }
		else if (tmpDamage.dam_Type == HITTYPE_DEADLY) { ++Deadly; }
	}

	if (intervalTime > 0)
	{
		m_DPS = totalDamage / intervalTime;
	}
	else
	{
		m_DPS = 0.0f;
	}

	//fprintf(fLog, "Total Damage: %d, Interval Time : %d초, DPS : %02f\n", totalDamage, intervalTime, m_DPS);
	fprintf(fLog, "%d\t%d\t%02f\t%d\t%d\t%d\t%d\n", totalDamage, intervalTime, m_DPS, EndDamage.att_EntityInfo.hp, EndDamage.att_EntityInfo.mp,
		Critical, Deadly);
	fflush(fLog);
}