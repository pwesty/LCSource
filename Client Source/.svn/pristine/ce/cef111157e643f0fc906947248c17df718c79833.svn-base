// ----------------------------------------------------------------------------
//  File : UISelectServer.h
//  Desc : Created by ant
// ----------------------------------------------------------------------------

#ifndef	UISELECTSERVER_H_
#define	UISELECTSERVER_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// #include <Engine/Interface/UIListBox.h>
// #include <vector>


// Define max count of server name
#ifdef EUROUPEAN_SERVER_LOGIN
#define	MAX_SERVER_NAME						50
#else
#define	MAX_SERVER_NAME						40
#endif


// Define text position
#define	SELECTSERVER_TITLE_TEXT_OFFSETX		25
#define	SELECTSERVER_TITLE_TEXT_OFFSETY		5
#define	SELECTSERVER_GROUP_TEXT_POSX		20
#define	SELECTSERVER_GROUP_TEXT_POSY		31
#define	SELECTSERVER_SERVER_TEXT_POSX		218
#define	SELECTSERVER_SERVER_TEXT_POSY		31


// Define size of select server
#define	SELECTSERVER_WIDTH					416
#define	SELECTSERVER_HEIGHT					236


// ----------------------------------------------------------------------------
// Name : CUISelectServer
// Desc :
// ----------------------------------------------------------------------------
class CUISelectServer : public CUIWindow
{
protected:
	// Controls
	CUIButton			m_btnOK;								// OK button
	CUIButton			m_btnCancel;							// Cancel button
	CUIButton			m_btnExit;								// [2012/10/18 : Sora] 재시작시 자동 로그인
	CUIListBox			m_lbServerGroup;						// Server group list box
	CUIListBox			m_lbServer;								// Server list box

	// Server info
	CTString			m_astrServerName[MAX_SERVER_NAME];		// String of server name
	int					m_nRecentServerGroup;					// Index of recent server group
	int					m_nRecentServer;						// Index of recebt server

	// UV of each part
	UIRectUV			m_rtBackTop;							// UV of top background
	UIRectUV			m_rtBackTop1;							// UV of top background
	UIRectUV			m_rtBackTop2;							// UV of top background
	UIRectUV			m_rtBackMiddle0;							// UV of middle background
	UIRectUV			m_rtBackMiddle;							// UV of middle background
	UIRectUV			m_rtBackBottom;							// UV of bottom background
 
	// 추천이벤트 이미지 wooss 060426
	UIRectUV			m_rtRecommend;							// UV of bottom background
	UIRectUV			m_rtSpeed;								// UV of bottom background

	CUIListBox*		m_plbServer;
	
	// 서버.
	struct sServerInfo
	{
		ULONG			iSubNum;			// Sub Server 번호.
		ULONG			iPlayerNum;			// 플레이어 수.
		CTString		strAddress;			// IP Address
		ULONG			iPortNum;			// Port Num

		bool operator<(const sServerInfo &other) const
		{			
			/*if(iPlayerNum < other.iPlayerNum)   return true;
			else if(iPlayerNum > other.iPlayerNum)   return false;
			else
			{
				// NOTE : 서브 번호를 기준으로 할까? 플레이어 수를 기준으로 할까???
				if(iSubNum < other.iSubNum)			return true;
				else if(iSubNum > other.iSubNum)	return false;
			}
			return false;*/
			if( iSubNum < other.iSubNum )
				return true;
			return false;
		}
		bool operator>(const sServerInfo &other) const
		{			
			return other.operator < (*this);
		}
	};

	// 서버 군
	struct sGroupInfo
	{
		sGroupInfo()
		{
			iGroupState		= 0;
		}
		~sGroupInfo()
		{
			vectorServerInfo.clear();
		}

		ULONG			iServerNo;		// 서버 군 번호
		UBYTE			ubServerType;	// 서버 군 타입
		int				nOrder;
		
		// FULL이거나 점검 중이면 -1씩 더함, 원할이면 +1씩 더함.
		int				iGroupState;						// 군 상태에 따른 점수.
		std::vector<sServerInfo>	vectorServerInfo;		// 서버 목록.

		bool operator<(const sGroupInfo &other) const
		{			
			// GroupState가 높은 순으로 정렬을 하고, 같다면 서버 번호를 기준으로 정렬합니다.
			/*
			if(iServerNo < other.iServerNo)			return true;
			else if(iServerNo > other.iServerNo)	return false;
			else									return false;
			*/
			if(iGroupState > other.iGroupState)			return true;
			else if(iGroupState < other.iGroupState)	return false;
			else
			{
				if(iServerNo < other.iServerNo)			return true;
				else if(iServerNo > other.iServerNo)	return false;
			}
			return false;
		}
		bool operator>(const sGroupInfo &other) const
		{			
			return other.operator < (*this);
		}
	};	

	// Function Object
	struct FindGroup
	{
		int iNum;
		FindGroup(sGroupInfo& info) : iNum(info.iServerNo) {};
		bool operator() (const sGroupInfo& info)
		{
			if(info.iServerNo == iNum)
			{
				return true;
			}
			else
				return false;
		}
	};
	
	struct FindServer
	{
		int iNum;
		FindServer(sServerInfo& info) : iNum(info.iSubNum) {};
		bool operator() (const sServerInfo& info)
		{
			if(info.iSubNum == iNum)
			{
				return true;
			}
			else
				return false;
		}
	};

	std::vector<sGroupInfo>		m_vectorGroupInfo;

	// 추천 서버 이벤트
	int FindRecomendSvr();
	
protected:
	// Internal functions

	// NOTE : 자동 로그인을 위해서 위치 변경.
	//void	ConnectToServer( CTString strIP, ULONG ulPortNum );		// 서버와 접속 시도
	void	SetServerList();

public:
	CUISelectServer();
	~CUISelectServer();

	ENGINE_API void	ConnectToServer( CTString strIP, ULONG ulPortNum );		// 서버와 접속 시도

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();
	
	// Reset
	void	Reset();

	void initialize();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	WMSG_RESULT	KeyMessage( MSG *pMsg );

	void	Lock(BOOL bLock);

	//Zone Info Manage
	void ENGINE_API	ResetServerList();	
	void ENGINE_API AddToServerList(LONG lServerNo, LONG lSubNum, LONG lPlayerNum, CTString &strAddress, LONG lPortNum ,UBYTE ubServerType = 0);
	void ENGINE_API SetRecentServer(int iRecentGroup = -1, int iRecentServer = -1);

	BOOL	IsEmptyGroup() { return m_vectorGroupInfo.empty(); };
	CTString GetServerGroupName( int iGroupNo );
	void	PressOKBtn();
	void	Close();

};


#endif	// UISELECTSERVER_H_

