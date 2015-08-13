#ifndef __SYNDICATEINFODATAMANAGER_H__
#define __SYNDICATEINFODATAMANAGER_H__

#include <map>
#include "Server.h"

class SyndicateInfoDataManager
{
public:
	struct info
	{
		int			joinCount_;
		int			jewelPoint_;
		LONGLONG	kingPoint;
		int			kingCharIndex;
		char		kingName[MAX_CHAR_NAME_LENGTH + 1];

		info() {
			memset(kingName, 0, sizeof(kingName));
		}
	};

public:
	SyndicateInfoDataManager();
	~SyndicateInfoDataManager();
	static SyndicateInfoDataManager* instance();

	void setJewelPoint(int syndicateType, int createJewelPoint);
	int getJewelPoint(int syndicateType);

	void setKingName(int syndicateType, const char* kingName);
	char* getKingName(int syndicateType);

	void setKingPoint(int syndicateType, LONGLONG point);
	LONGLONG getKingPoint(int syndicateType);

	void setKingIndex(int syndicateType, int charIndex);
	int	getKingIndex(int syndicateType);

	/*CSkill* getBufSkill(int syndicateType, int grade);
	void setBufSkill(int syndicateType, int grdae, CSkill* skill);*/

	void setJoinCount(int syndicateType, int joinCount);
	int	getJoinCount(int syndicateType);

	void sendKingNameInfo(CDescriptor* dest = NULL);
	void sendJewelPercentageInfo(CDescriptor* dest = NULL);
	void sendSyndicateInfo(CDescriptor* dest = NULL);

private:
	info	info_[SYNDICATE::eSYNDICATE_MAX];
};

#endif  // __SYNDICATEINFODATAMANAGER_H__