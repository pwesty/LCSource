#ifndef __AUTO_BUFFER_H__
#define __AUTO_BUFFER_H__

#include <vector>
#include <time.h>

struct BuffData {
	int skillID;
	int level;
};

class AutoBuffSystem {
public:
	AutoBuffSystem();
	~AutoBuffSystem();

	bool Init();

	void TurnOff();
	void TurnOn();

	// Check if update is required currentTime - lastTime > Loop time
	void TickSystem();

	// Reload the buff data table
	void ReloadTable();

	bool Started() { return m_bInit; }
private:
	std::vector<BuffData> m_buffs;

	time_t lastTime;

	int m_iLoopTimeSeconds;
	bool m_bOn, m_bInit;
};

#endif // __AUTO_BUFFER_H__