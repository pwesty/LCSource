// TitleSystem.h: interface for the CTitleProtoList class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_TITLESYSTEM_H__060344F0_5F3D_42FD_8B8C_CFC42F759C27__INCLUDED_)
#define AFX_TITLESYSTEM_H__060344F0_5F3D_42FD_8B8C_CFC42F759C27__INCLUDED_

class CTitle;
class CTitleProto;
class CTitleNode;

class CTitleProtoList
{
public:
	typedef std::map<int, CTitleProto*> map_t;
	map_t	map_;
	CTitle* Create(int index, int time);
	CTitleProto* FindProto(int index);
	bool Load();
	int m_nCount;
	CTitleProto* m_proto;
	CTitleProtoList();
	virtual ~CTitleProtoList();
};

class CTitleProto
{
public:
	int m_itemidx;
	int m_index;
	int m_time;
	int m_option[MAX_TITLE_OPTION][2];		// [0] 옵션인덱스 , [1] 레벨
	int m_nOptionCount;
	int m_castleNum;

	CTitleProto();
	virtual ~CTitleProto();
};

class CTitle
{
public:
	CTitleProto* m_proto;
	int m_endtime;
	CTitle();
	virtual ~CTitle();
};

class CTitleList
{
public:
	bool CheckTitleTime(int CurrentTitle);
	bool HaveTitle(int index);
	CTitle* Find(int index);
	CTitleNode* m_head;
	int m_nCount;
	bool Remove(CTitle* title);
	bool Add(CTitle* title);
	CTitleList();
	virtual ~CTitleList();
};

class CTitleNode
{
public:
	CTitleNode* m_prev;
	CTitleNode* m_next;
	CTitle* m_title;
	CTitleNode(CTitle* title);
	virtual ~CTitleNode();
};

#endif // !defined(AFX_TITLESYSTEM_H__060344F0_5F3D_42FD_8B8C_CFC42F759C27__INCLUDED_)
//
