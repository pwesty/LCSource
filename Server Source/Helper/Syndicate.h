#include <map>
#include "../ShareLib/rnsocketiohandler.h"
#include "../ShareLib/rnsocketioservice.h"

#include "../ShareLib/DBCmd.h"
#include "../ShareLib/LCString.h"
#include "Descriptor.h"

#pragma once

struct Syndicate2ndUser
{
	int charIndex;
	LONGLONG point;
	char userName[MAX_CHAR_NAME_LENGTH];
	int isKing;

	Syndicate2ndUser()
		:charIndex(0), point(0), isKing(0)
	{
		memset(userName, 0x00, MAX_CHAR_NAME_LENGTH);
	}
};

class Syndicate
{
public:
	struct info
	{
		int			joinCount_;
		int			jewelPoint_;
		LONGLONG	kingPoint;
		int			kingIndex;
		char		kingName[MAX_CHAR_NAME_LENGTH];
	};

public:
	Syndicate();
	~Syndicate() {};

	void increaseJewelPoint(int syndicateType, int point);
	void decreaseJewelPoint(int syndicateType, int point);

	int getJewelPoint(int syndicateType);
	int	getUserCount(int syndicateType);
	char* getKingName(int syndicateType);
	LONGLONG getKingPoint(int syndicateType);
	int	getKingIndex(int syndicateType);

	void setJewelPoint(int syndicateType, int point);
	void setUserCount(int syndicateType, int count);
	void setKingPoint(int syndicateType, LONGLONG point);
	void setKingName(int syndicateType, const char* kingName);
	void setKingIndex(int syndicateType, int charIndex);

	void sydicateJoin(int syndicateType);
	void sydicateLeave(int syndicateType);

	void sendInfo();
	void sendKingInfo();
	void sendDownGrade(int type, int charIndex, int grade, LONGLONG point);

	void load();
	void save();

	void add2ndUser(Syndicate2ndUser& st, int syndicateType);
	void del2ndUser(int charIndex, int syndicateType);

	bool checkKing(int type, LONGLONG point);
	void setKingData(int type, int charIndex, LONGLONG point, char* name);

	std::vector<Syndicate2ndUser>* find2ndInfo(int syndicateType);

	std::vector<Syndicate2ndUser> userList2nd_k;
	std::vector<Syndicate2ndUser> userList2nd_d;

private:
	info	info_[SYNDICATE::eSYNDICATE_MAX];	// [1], [2]만 사용함
};
