#ifndef __ARTIFACT_MANAGER_H__
#define __ARTIFACT_MANAGER_H__

#define ARTIFACT_LEVEL1_ITEM_INDEX 10951
#define ARTIFACT_LEVEL2_ITEM_INDEX 10952
#define ARTIFACT_LEVEL3_ITEM_INDEX 10953

#define ARTIFACT_ITEM_LEVEL_1	1
#define ARTIFACT_ITEM_LEVEL_2	2
#define ARTIFACT_ITEM_LEVEL_3	4

#define ARTIFACT_MAX_ITEM_COUNT 4

struct ArtifactItemData
{
	CPC* pc;
	CItem* item;
	int  item_level;
};

class ArtifactManager
{
private:
	std::vector<ArtifactItemData*> _vec;
	void _insert(int char_index, CItem* item);
	void _delete(CItem* item);

public:
	ArtifactManager() : drop_count(0){};
	~ArtifactManager();
	static ArtifactManager* instance();

	int drop_count;
	
	int  getCount();
	int	 getSameZoneCount(CPC* pc);
	bool isOwnerPC(int char_index);
	bool isArtifactParty(CParty* party);
	CItem* FindWearItem(CPC* pc);
	void calcDropCount(CItem* item, bool isTake);
	
	void hunt(CPC* owner, CPC* target);
	void dead(CPC* owner);
	void disconnect(CPC* pc);
	void move_zone(CPC* pc, int zone_index);
	void expire_time(CItem* item);
	void item_drop(CItem* item);

	void addOwner(CPC* owner, CItem* item, bool isTake = false);
	void changeOnwer(CPC* newOwner, CItem* item);

	//아이템 합성
	void compose( CPC* owner, CItem* mat_item1, CItem* mat_item2,  CItem* result_item );

	void sendToAllSysMsg(CPC* owner, int item_index, int systype, int zone_index = -1);
	void sendSysMsg(CPC* owner, int systype);
	void sendOwnerGPSData(CPC* ch);

	void ApplyArtifact(CPC* pc);

	void eventOff();
	
	bool canMove(int zone_index);
	bool canDrop(int zone_index);

	int checkValidUser(CPC* ch);
};

#endif
