#ifndef __OPTION_H__
#define __OPTION_H__

#include "Config.h"
#include "Character.h"
#include "APetProto.h"
#include <vector>

class CPC;
class APet;
class CItem;

class COptionProto {
public:
     int m_idNum;
     int m_type;
     std::vector<int, std::allocator<int> > m_levelValue;
     std::vector<int, std::allocator<int> > m_probValue;
     int m_weaponType;
     int m_wearType;
     int m_accessoryType;
   public:
     COptionProto(void);
 };


class COptionProtoList {
   public:
     COptionProto *m_proto;
     int m_nCount;

     COptionProtoList(void);
     ~COptionProtoList();
     bool Load(void);
     COptionProto * FindProto(int);
   protected:
     static int CompIndex(const void *, const void *);
 };

class COption {
public:
     COptionProto *m_proto;
     int m_type;
     int m_level;
     int m_value;
     short m_dbValue;
   public:
     COption(void);
     void MakeOptionValue(COptionProto *, int, int);
     void SetOptionValue(short);
     void ApplyOptionValue(CPC *, CItem *);
     //static void ApplyOptionValue(CPC *, int, int, CItem *);
     //void ApplyOptionValue(CAPet *, CItem *);
     //static void ApplyOptionValue(CAPet *, int, int, CItem *);
     //static void ApplyOptionValue(CCharacter *, int, int, CItem *);
     void GetDBValue(short);
     void SetDBValue(void);
 };

#endif