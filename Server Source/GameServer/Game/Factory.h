#ifndef __FACTORY_H__
#define __FACTORY_H__

#include <map>
#include <set>

typedef struct __tagFactoryStuff
{
	int		nItemIdx;
	int		nCount;
} FACTORY_STUFF;

class CFactoryProto {
       private:
         int m_nIndex;
         int m_nJob;
         int m_nSealType;
         int m_nItemType;
         int m_nItemIdx;
         LONGLONG m_llMakeExp;
         LONGLONG m_llNeedExp;
         LONGLONG m_llNas;
         FACTORY_STUFF m_stuff[5];

       public:
         CFactoryProto(void);
         ~CFactoryProto();
         void InitData(int, int, int, int, int, int, long long, long long, long long, FACTORY_STUFF *);
         const int GetIndex(void) const;
         int GetSealType(void);
         int GetItemIdx(void);
         const FACTORY_STUFF * GetStuff(int);
         LONGLONG GetNas(void);
         LONGLONG GetNeedExp(void);
         LONGLONG GetMakeExp(void);
         void SetIndex(int);
     };
typedef std::map<int, CFactoryProto*, std::less<int>, std::allocator<std::pair<int const, CFactoryProto*> > > map_t;
class CFactoryProtoList {
   private:
     map_t map_;
     CFactoryProto *m_listFactoryItem;

   public:
     CFactoryProtoList(void);
     ~CFactoryProtoList();
     bool Load(void);
     CFactoryProto * Find(int);
 };

///////////////////////////
// 캐릭터 제작 아이템 리스트
//
typedef std::set<CFactoryProto*, std::less<CFactoryProto*>, std::allocator<CFactoryProto*> > set_t;
class CFactoryList {
   public:
     set_t m_listItem;

     CFactoryList(void);
     ~CFactoryList();
     int GetCount(void);
     bool Add(CFactoryProto *);
     bool Find(CFactoryProto *);
     CFactoryProto * Find(int);
     set_t & getList(void);
 };
#endif // __FACTORY_H__