#ifndef __NEWS_H__
#define __NEWS_H__

#include <Common/Packet/ptype_notice.h>
#include <vector>
class CNews
{
public:
	CNews();
	~CNews();

	void ClearData();
	void RecvMsg(CNetworkMessage* istr);

	void UpdateRevision(CNetworkMessage* istr);
	void UpdateWebNoti(CNetworkMessage* istr);
	void UpdateWebClose(CNetworkMessage* istr);
	void UpdateWebClear(CNetworkMessage* istr);

	void ListReq(int nPage = 0);

	bool CheckRevision()	{ return (m_nCurRevision != m_nSaveRevision ? true : false);	}
	ENGINE_API	int  GetRevision()		{ return m_nSaveRevision;	}
	ENGINE_API void LoadRevision(int nRevision)	{ m_nSaveRevision = nRevision;	}
	ResponseClient::NoticeList* GetNotiList()	{return m_pNoticeList;	}

	int GetCurPage()	{ return m_nCurPage;	}
	bool GetNextPage()	{ return m_bNextPage;	}
	void ResetPage()	{ m_nCurPage = 0; m_bNextPage = false;	}

	int	GetWebUrl(int nIdx);
private:

	void RecvDataList(CNetworkMessage* istr);

	int m_nCurRevision;
	int	m_nSaveRevision;
	int m_nCurPage;
	bool m_bNextPage;
	ResponseClient::NoticeList* m_pNoticeList;
	typedef std::vector<int>	vecUrl;
	typedef vecUrl::iterator	vecUrliter;
	vecUrl	m_vecUrl;
};
#endif // __NEWS_H__