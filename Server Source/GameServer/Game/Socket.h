#pragma once

class CItemProto;

class CSocketPos {
   private:
     int m_itemidx;
     CItemProto *m_itemproto;
     COption *m_option;

   public:
     CSocketPos(void);
     ~CSocketPos();
     bool SetJewel(int);
     void Clear(void);
     int GetJewelDBIdx(void) const;
     void ApplyJewelOption(CPC *, CItem *) const;
     COption * GetOption(void) const;
     void GetSocketInfo(int &, int &, int &) const;
 };
enum SOCKETMAXCOUNT {MAXSOCKET0, MAXSOCKET2 = 2, MAXSOCKET4 = 4};
class CSocketList {
   private:
     SOCKETMAXCOUNT m_maxSocketCount;
     CSocketPos *m_socketpos;
     char m_createdSocketCount;

   public:
     CSocketList(void);
     ~CSocketList();
     bool SetJewelAt(int, int);
     bool SetSocketCount(int);
     bool SetSocketCount(int, int *);
     void ClearAll(void);
     void ApplyJewelOption(CPC *, CItem *) const;
     char GetSocketCount(void) const;
     char GetMaxSocketCount(void) const;
     int GetJewelAt(char) const;
     COption * GetJewelOptionAt(char) const;
     bool IsCombinedJewel(void) const;
     char GetCombinedCount(void) const;
     void GetSocketInfo(char, int &, int &, int &) const;
     void GetSocketInfoString(char *) const;
     bool CopySocketInfo(const CSocketList &, int &);
   private:
     bool InitSocket(int, int);
     bool InitSocketFromDB(int, int, char, int *, int *);
     bool createsocket(char);
 };