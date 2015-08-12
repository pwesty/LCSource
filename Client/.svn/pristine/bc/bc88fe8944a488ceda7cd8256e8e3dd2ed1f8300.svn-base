
#ifndef		MAPINFO_DATA_H_
#define		MAPINFO_DATA_H_

struct stDetailmap
{
	char dmapcount;

	struct stDetailinfo
	{
		int dmapleft;
		int dmaptop;
		int dmapright;
		int dmapbottom;
		int dmapwleft;
		int dmapwtop;
		int dmapwright;
		int dmapwbottom;
		float dmapx;
		float dmapz;
		float dmaprate;
	} dmlist[0];
};

struct stMapDungeon
{
	char dgcount;

	struct stDungeonInfo
	{
		int dgindex;
		float dgx;
		float dgz;
		char dgtype;
	} dglist[0];
};

struct stMapNpc
{
	int npcindex;

	struct stMapNpcInfo
	{
		int ylayer;
		float npcx;
		float npcz;
	} npclist[0];
};


struct stMapInfo
{
	int zoneindex;
	char ylayer;
	int mapleft;
	int maptop;
	int mapright;
	int mapbottom;
	float maprate;
	int mapoffsetx;
	int mapoffsetz;

	int sleft;
	int	stop;
	int	sright;
	int	sbottom;
	int	swleft;
	int	swtop;
	int	swright;
	int	swbottom;
	int	sx;
	int	sz;
	int	srate;
	int	sindex;
	int	sx2;
	int	sz2;
	int	stype;

	stDetailmap dmlist;
	stMapDungeon dglist;
	stMapNpc	npclist;
};

struct stMapOnlyTool
{
	char szdleft[256];
	char szdtop[256];
	char szdright[256];
	char szdbottom[256];
	char szdwleft[256];
	char szdwtop[256];
	char szdwright[256];
	char szdwbottom[256];
	char szdx[256];
	char szdz[256];
	char szdrate[256];
	char szdgindex[256];
	char szdgx[256];
	char szdgz[256];
	char szdgtype[256];
};

#endif		// MAPINFO_DATA_H_