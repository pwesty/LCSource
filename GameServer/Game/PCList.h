#ifndef PCLIST_H_INCLUDED
#define PCLIST_H_INCLUDED

enum _tagCPCList_ErrCode {CPCList_NONE, CPCList_CPC_NULL, CPCList_MAX_LIST, CPCList_INDEX_OVER, CPCList_NO_FIND, CPCList_NO_NAME};

class CPCList {
     int *m_pcList[8192];
     int m_max;
     int m_cur;
     int *m_ErrList[20];
     int m_ErrCode;
   public:
     CPCList(void);
     ~CPCList();
     bool Init(void);
     void Insert_Err(int *);
     void Input_Err(_tagCPCList_ErrCode);
     int Output_Err(void);
     int Find_Index(int *);
     int Add(int *);
     void Remove(int *);
     bool Playing(int *);
     int * Get(int);
     int * Find(int);
     int * Find(const char *, char);
     void SendToAll(CNetMsg &);
     void SendGuildWarPointMsg(int *, int);
     void SendToWarJoinUser(CNetMsg &, int, bool);
     void SendToAllTeacher(CNetMsg &);
 };

#endif //PCLIST_H_INCLUDED