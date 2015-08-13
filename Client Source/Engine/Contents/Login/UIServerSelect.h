#ifndef __UI_SERVER_SELECT_H__
#define __UI_SERVER_SELECT_H__

class CUIList;
struct sChannelInfo;
struct sServerInfo;

class CUIServerSelect : public CUIWindow
{
public:
	CUIServerSelect();

	enum eERRTYPE
	{
		eERR_NOT_CONNECT = 0,
	};

	enum eLOCAL
	{
		LOCAL_NONE = 0,
		LOCAL_EUROPEAN,

		LOCAL_MAX,

		SERVER_MAX = 10
	};

	void	initialize();
	void	OpenUI();
	void	CloseUI();

	void	Lock(BOOL bLock);
	void	EnableButton();
	void	ConnectServer();
	void	GotoLogin();

	CTString GetServerName(int serverNum);
	CTString GetChannelSubString(int nServer, int nChannel);

	void	ErrMessage(int nErrCode);

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnKeyMessage(MSG* pMsg);
private:
	void	UpdateServerList();
	void	UpdateChannelList(int serverNum);
	void	ChildHide(CUIList* pList, BOOL bHide);
	int		getNomalChannelStrIdx(int nChannel);
	int		getEUChannelStrIdx(int nChannel);

	CUIList*	m_pServerList;
	CUIList*	m_pChannelList;
	CUIButton*	m_pBtnCancel;
	CUIButton*	m_pBtnOk;

	CTString m_strServerName[LOCAL_MAX][SERVER_MAX];
};
#endif // __UI_SERVER_SELECT_H__