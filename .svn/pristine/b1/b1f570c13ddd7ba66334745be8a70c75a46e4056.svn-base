#ifndef __SYNDICATEMANGER_H__
#define __SYNDICATEMANGER_H__

#include <map>
#include <vector>
#include <deque>
#include <string>
#include <memory.h>

#define SYNDICATE_HISTORY_MAX		(20)

class CPC;
typedef struct __tagSyndicateHistory
{
	int syndicateType;			// 결사대 타입
	int time;					// 자신의 활동 시간
	int grade;					// 직위
	int type;					// 활동 타입 enum으로 정리 - 승급, 강등, 가입, 탈퇴, 승리, 패배
	int targetSyndicateType;	// 전투시 대상 결사대 타입
	std::string targetName;
} SYNDICATE_HISTORY;

class SyndicateHistoryManager
{
public:
	explicit SyndicateHistoryManager(CPC* pc);
	~SyndicateHistoryManager();

	void	initDBLoading(int syndicateType, SYNDICATE_HISTORY& historys);
	// 히스토리 로그
	void	joinHistory(int syndicateType);
	void	removeHistory(int syndicateType);
	void	degradeHistory(int syndicateType, int grade);
	void	promotionHistory(int syndicateType, int grade);
	void	battleWinHistory(int syndicateType, int targetSyndicateType, const char* targetName);
	void	battleLoseHistory(int syndicateType, int targetSyndicateType, const char* targetName);

	void	sendHistory(int syndicateType);

	void	save(std::vector< std::string > & vec);

private:
	void	pushHistory(SYNDICATE_HISTORY& data);

private:
	typedef std::deque<SYNDICATE_HISTORY> HISTORY_DATA_ELE;
	typedef std::map< int, HISTORY_DATA_ELE > HISTORY_DATAS;
	HISTORY_DATAS historys_;
	CPC* _owner;
};

typedef struct __tagSyndicateData
{
	int				syndicateType;			// 타입
	LONGLONG		syndicate_point_k;
	LONGLONG		syndicate_point_d;
	int				syndicate_join_bit;
	int				syndicate_grade_k;
	int				syndicate_grade_d;

	__tagSyndicateData():syndicateType(0), syndicate_point_k(0), syndicate_point_d(0), syndicate_grade_d(0), syndicate_grade_k(0), syndicate_join_bit(0) {}
} SYNDICATE_DATA;


class SyndicateManager
{
public:
	explicit SyndicateManager(CPC* pc);
	~SyndicateManager();

	CPC* _owner;

	void	initHistoryDBLoading(int syndicateType, SYNDICATE_HISTORY& historys);
	bool	isFirstJoin(int syndicateType);

	void	joinSyndicate(int syndicateType);
	void	removeSyndicate(CItem* item);

	void	setSyndicateType(int syndicateType);
	void	setSyndicateGrade(int syndicateType, int syndicateGrade);

	//기여도
	void		setSyndicatePoint(LONGLONG point);
	LONGLONG	getSyndicatePoint(int syndicateType);

	//기여도
	void	increaseSyndicatePoint(LONGLONG point);
	void	decreaseSyndicatePoint(LONGLONG point);
	void	resetSyndicatePoint();

	//등급 설정
	int		getSyndicateGrade(int syndicateType);

	//보석 기부
	void	syndicateAddJewel(int vItemIndex);
	void	syndicateDelJewel(int count);

	bool	isKing();

	void	save(std::vector<std::string>& vec);
	void	send();					//update userinfo
	void	sendRemove();
	void	sendUserInfo();			//response userinfo

	void	applySyndicateSkill(int syndicateType);

	SyndicateHistoryManager historyManager_;
	SYNDICATE_DATA		syndicateData_;

private:
	void	changeGrade(int syndicateType);
	int		getContributionPointByJoin(int syndicateType);
	void	sendJoinCreateJewel(int syndicateType);
	void	sendLeaveCreateJewel(int syndicateType);
};
#endif // __SYNDICATEMANGER_H__