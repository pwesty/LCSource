#ifndef __EVENTMANAGER_H_
#define __EVENTMANAGER_H_

#include <map>
#include <vector>

struct LoaderAble {
       public:
         LoaderAble(void);
         ~LoaderAble();
         virtual bool init(void);
     };

struct RewardEventItem {
       private:
         int rewardDate;
         int index;
         int plus;
         int flag;
         int count;

       public:
         RewardEventItem(void);
         ~RewardEventItem();
         void set(int, int, int, int, int);
         int GetRewardDate(void);
         int GetIndex(void);
         int GetPlus(void);
         int GetFlag(void);
         int GetCount(void);
     };

struct RewardEventItemContainer {
       private:
         int idx;
         std::vector<RewardEventItem*, std::allocator<RewardEventItem*> > items;

       public:
         RewardEventItemContainer(int);
         ~RewardEventItemContainer();
     };

struct RewardEvnetItemManager : public LoaderAble {
       private:
         std::map<int, RewardEventItemContainer*, std::less<int>, std::allocator<std::pair<int const, RewardEventItemContainer*> > > rewardItems;

       public:
         RewardEvnetItemManager(void);
         ~RewardEvnetItemManager();
         virtual bool init(void);
         bool loadRewardItem(void);
         bool rewardItemPushback(int, int, int, int, int, std::vector<RewardEventItem*, std::allocator<RewardEventItem*> > &);
         int getRewardItems(int, std::vector<RewardEventItem*, std::allocator<RewardEventItem*> > &);
     };
#endif