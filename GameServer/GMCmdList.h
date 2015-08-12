#ifndef __GM_CMD_LIST_H__
#define __GM_CMD_LIST_H__

#include <boost/unordered_map.hpp>

//////////////////////////////////////////////////////////////////////////

typedef void (*GMFunction) (CPC* pc, const char* arg, std::vector<std::string>&);

class CGMCmd
{
public:
	CGMCmd(std::string command, int level, GMFunction fun);
	~CGMCmd(void);

	// 실행
	void run(CPC* pc, const char* arg, std::vector<std::string>& vec);

	// get command
	std::string& getCommand(void);

	// get level
	int getLevel(void) const;

	// get function
	GMFunction getFunction(void) const;

private:
	std::string m_command;		// GM Command	: 명령어
	int m_level;				// GM Level		: 실행 가능 GM 레벨
	GMFunction m_function;		// GM Function	: 실행 함수 포인터
};

//////////////////////////////////////////////////////////////////////////

class CGMCmdList
{
private:
	CGMCmdList(void);

public:
	typedef boost::unordered_map<std::string, CGMCmd*> map_t;

	~CGMCmdList(void);

	// GMCmd 로드
	void load(void);

	// 실행
	void run(CPC* pc, const char* arg);

	static CGMCmdList* getInstance(void);

private:
	// GMCmd 추가
	void add(CGMCmd* gmcmd);

	// 검색
	CGMCmd* find(std::string& command);

	// 실행 가능한지 확인
	bool canRun(CGMCmd* gmcmd, CPC* pc);

private:
	static CGMCmdList* m_instance;		// singleton instacne

	map_t	m_GMCmdList;	// GMGmd List
};

#define gGMCmdList CGMCmdList::getInstance()

#endif
