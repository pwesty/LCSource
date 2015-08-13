#ifndef __EXTREME_CUBE_H__
#define __EXTREME_CUBE_H__

#include "NetMsg.h"
#include "LCList.h"
#include "LCString.h"
#include "Character.h"

#define MAX_CUBESPACE		30
#define BATTLECUBE_START	0
#define BOSSCUBE_START		25

#define GUILDCUBE_TIME		3600		// 2시간
#define EXTREME_CUBE_SUBSERVER 1


#define CUBEMASTER_RED		521
#define CUBEMASTER_BLUE		522
#define CUBEMASTER_YELLOW	523
#define CUBEMASTER_PURPLE	524
#define CUBEMASTER_ORANGE	525
#define CUBE_RED			526
#define CUBE_BLUE			527
#define CUBE_YELLOW			528
#define CUBE_PURPLE			529
#define CUBE_ORANGE			530

#define CRYSTALCOUNT		5 
 

// 큐브상태
typedef enum _tagCubeState
{
	CUBESTATE_RED = 0,
	CUBESTATE_ORANGE,
	CUBESTATE_BLUE,
	CUBESTATE_PURPLE,
	CUBESTATE_YELLOW,
}CUBESTATE;

// 큐브멤버타입
typedef enum _tagCubeMemType
{
	CUBEMEM_PARTY,
	CUBEMEM_GUILD,
}CUBEMEMTYPE;

// 큐브공간타입
typedef enum _tagCubeSpaceType
{
	BATTLE_SPACE,
	BOSS_SPACE,
}CUBESPACETYPE;

// 몹리젠포인트
typedef struct _tagCubeRegenPoint
{
	float	regenX;
	float	regenZ;
	int	 	regenY;
	char	ylayer;
}CUBEREGENPOINT;


class CExtremeCube;

//////////////////////////////// 멤버리스트 ///////////////////////////////
class CCubeMemList {
       protected:
         int m_cubenum;
         char m_type;
         CLCList<CPC*> m_pcList;
         char m_clearCount;
         static int m_uniqueidx;
         int m_index;
         bool m_bEnable;

       public:
         CCubeMemList(void);
         int GetPCCount(void);
         void AddPC(CPC *);
         bool SetCubeNum(int);
         bool GetCubeNum(int &);
         bool JoinCube(CPC *, int);
         bool MoveToOtherCube(int);
         int GetUniqueIdx(void);
         bool IsPartyCubeMemList(void);
         bool IsGuildCubeMemList(void);
         virtual void DelPC(CPC *);
         virtual void DelSummonNpc(void);
         bool FindPC(CPC *);
         void GetOutAll(void);
         void IncreaseWinCount(void);
         int GetWinCount(void);
         void SendCompleteCount(void);
         int GetPersonalCubePoint(CPC *, int);
         int GetIndex(void);
         bool IsEnable(void);
         virtual void Disable(void);
         virtual bool IsBreak(void);
         void SendStartCount(void);
         void ApplyCubeEffect(void);
         ~CCubeMemList();
     };


class CPartyCubeMemList : public CCubeMemList {
       private:
         CParty *m_party;

       public:
         CPartyCubeMemList(void);
         ~CPartyCubeMemList();
         CParty * GetParty(void);
         void SetParty(CParty *);
         void Enable(CParty *, int);
         virtual void Disable(void);
         virtual bool IsBreak(void);
         virtual void DelPC(CPC *);
         virtual void DelSummonNpc(void);
     };


class CGuildCubeMemList : public CCubeMemList {
       private:
         CGuild *m_guild;

       public:
         CGuildCubeMemList(void);
         ~CGuildCubeMemList();
         void SetGuild(CGuild *);
         CGuild * GetGuild(void);
         void Enable(CGuild *, int);
         virtual void Disable(void);
         virtual bool IsBreak(void);
         virtual void DelPC(CPC *);
         virtual void DelSummonNpc(void);
     };




///////////////////////////////// 큐브 /////////////////////////////////////
class CCubeSpace {
       public:
         CArea *m_area;
         const CExtremeCube *m_extremeCube;
         int m_extra;
         CNPC *m_crystalkind[5];
         CNPC *m_crystal;
         int m_waitTime;
       protected:
         char m_type;
         CUBESTATE m_state;

       public:
         CCubeSpace(void);
         ~CCubeSpace();
         virtual void CheckCubeState(void);
         virtual bool SetMemList(CCubeMemList *);
         virtual bool DelMemList(CCubeMemList *);
         virtual void DelMob(CNPC *);
         void ChageState(_tagCubeState);
         virtual void Clean(void);
         virtual void RegenMob(void);
         bool IsBattleCubeSpace(void);
         bool IsBossCubeSpace(void);
         void DelCrystal(bool);
         CUBESTATE GetState(void);
     };


// 일반큐브
class CBattleSpace : public CCubeSpace {
       private:
         CLCList<CNPC*> m_mobList;
         CCubeMemList *m_memList1;
         CCubeMemList *m_memList2;

       public:
         CBattleSpace(void);
         ~CBattleSpace();
         virtual void CheckCubeState(void);
         virtual bool SetMemList(CCubeMemList *);
         virtual bool DelMemList(CCubeMemList *);
         virtual void DelMob(CNPC *);
         virtual void Clean(void);
         virtual void RegenMob(void);
         bool IsAvailableJoin(bool &);
         bool IsAvailableJoinPersonal(void);
         CCubeMemList * WinMemList(void);
         CCubeMemList * GetMemList1(void);
         CCubeMemList * GetMemList2(void);
     };


class CBossSpace : public CCubeSpace {
   private:
     CNPC *m_boss;
     CCubeMemList *m_memList;

   public:
     CBossSpace(void);
     ~CBossSpace();
     virtual void CheckCubeState(void);
     virtual bool SetMemList(CCubeMemList *);
     virtual void DelMob(CNPC *);
     virtual bool DelMemList(CCubeMemList *);
     virtual void Clean(void);
     virtual void RegenMob(void);
     bool IsAvailableJoin(void);
     CCubeMemList * GetMemList(void);
 };



class CExtremeCube {
   public:
     int m_mobCount;
     int *m_mobidx;
     int m_regencount;
     CUBEREGENPOINT *m_regenPoint;
     int m_bossCount;
     int *m_bossidx;
     time_t m_nextGuildCubeTime;
   private:
     bool m_bGuildCubeStart;
     bool m_bGuildCubeRemainStartNotice;
     bool m_bGuildCubeRemainEndNotice;
     CCubeSpace *m_cube[30];
     CPartyCubeMemList m_partyCubeMemList[60];
     CGuildCubeMemList m_guildCubeMemList[60];

   public:
     CExtremeCube(void);
     ~CExtremeCube();
     void CheckCubeSpace(void);
     int GetGuildCubeTime(void);
     void SetGuildCubeTime(long &);
     bool IsGuildCubeTime(void);
     CCubeSpace * GetExtremeCube(int);
     bool Load(void);
     CCubeMemList * FindMemList(CGuild *);
     CCubeMemList * FindMemList(CParty *);
     CCubeMemList * FindMemList(int);
     bool Init(void);
     int GetAvailableBattleCube(bool &);
     int GetAvailableBossCube(void);
     bool IsExtremeCubeServer(void);
     void StartGuildCube(void);
     void EndGuildCube(void);
     void StartGuildCubeRemainTime(void);
     void EndGuildCubeRemainTime(void);
     CPartyCubeMemList * GetAvailablePartyCubeMemList(void);
     CGuildCubeMemList * GetAvailableGuildCubeMemList(void);
   private:
     void GetOutAllMemList(void);
     bool LoadCubeData(void);
     bool CreateCubeSpace(void);
     bool CheckCubeMasterNPC(void);
 };

#endif // __EXTREME_CUBE_H__
