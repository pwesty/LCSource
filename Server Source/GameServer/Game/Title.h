#pragma once

struct CTitleProto {
           public:
             int m_itemidx;
             int m_index;
             int m_time;
             int m_option[5][2];
             int m_nOptionCount;
             int m_castleNum;

             CTitleProto(void);
             ~CTitleProto();
         };

class CTitle {
         public:
           CTitleProto *m_proto;
           int m_endtime;

           CTitle(void);
           ~CTitle();
       };
class CTitleNode {
             public:
               CTitleNode *m_prev;
               CTitleNode *m_next;
               CTitle *m_title;

               CTitleNode(CTitle *);
               ~CTitleNode();
           };
class CTitleList {
       public:
         CTitleNode *m_head;
         int m_nCount;

         CTitleList(void);
         bool CheckTitleTime(int);
         bool HaveTitle(int);
         CTitle * Find(int);
         bool Remove(CTitle *);
         bool Add(CTitle *);
         ~CTitleList();
};

class CTitleProtoList {
   public:
     int m_nCount;
     CTitleProto *m_proto;

     CTitleProtoList(void);
     CTitle * Create(int, int);
     CTitleProto * FindProto(int);
     bool Load(void);
     ~CTitleProtoList();
 };