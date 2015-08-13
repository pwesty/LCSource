#ifndef __CHATTING_UI_H__
#define __CHATTING_UI_H__

#include <functional>

enum ChattingMsgType
{
	CHATMSG_NORMAL					= 0,
	CHATMSG_PARTY					= 1,
	CHATMSG_GUILD					= 2,
	CHATMSG_TRADE					= 3,
	CHATMSG_WHISPER					= 4,
	CHATMSG_SHOUT					= 5,
	CHATMSG_NOTICE					= 6,
	CHATMSG_GM						= 7,
	CHATMSG_DUMMY					= 8,		// 메신저 시스템과 같이 맞춰 주기 위함(이기환)
	CHATMSG_LORD					= 9,		// 성주
	CHATMSG_RANKER					= 10,		// 랭커
	CHATMSG_RANKER_CONN				= 11,		// 랭커
	CHATMSG_NOTICE_SERVERDOWN		= 12,		// 서버 종료 메시지 060420
	CHATMSG_DUMMY2					= 13,		// 인덱스 맞춰주기 위해.
	CHATMSG_DUMMY3					= 14,		// 인덱스 맞춰주기 위해.
	CHATMSG_GMTOOL					= 15,		// GM TOOL을 이용한 메시지 처리 WSS_GMTOOL 070517 (13,14 서버용)
	CHATMSG_PRESSCORPS				= 16,
	CHATMSG_EXPEDITION				= 17,		// [sora] 원정대
	CHATMSG_NPCNOTICE				= 18,
	CHATMSG_CHANNEL_LEVEL			= 19,		// 일반 전체 채팅(같은서버에서 채널간 채팅)
	CHATMSG_CHANNEL_TRADE			= 20,		// 매매 채팅(같은서버에서 채널간 채팅)
	CHATMSG_TOTAL					,		
	CHATMSG_COMMAND					,			// [/command]
};

// System message type
enum SystemMsgType
{
	SYSMSG_NORMAL					= 0,
	SYSMSG_ATTACK					= 1,
	SYSMSG_ATTACKED					= 2,
	SYSMSG_ERROR					= 3,
	SYSMSG_NOTIFY					= 4,
	SYSMSG_USER						= 5, // wooss 070307 kw:WSS_WHITEDAY_2007
	SYSMSG_TOTAL					= 6,
};

#define	MAX_WHISPER_SAVE			5
#define CHAT_MEMORY_COUNT			5

class CUITab;
class CUITabPage;
class CUIList;
class CUIImage;
class CUIImageSplit;
class CUIText;
class CUITextBoxEx;
class CUIButton;
class CUICheckButton;
class CUIBase;

class CChattingUI : public CUIWindow
{
public:
	CChattingUI();
	~CChattingUI();

	void initialize();

	CUIEditBox* GetInputBox()	{ return m_pChatInput;	}
	
	void ResetChatting();

	void OpenChatOption();
	void ChangeChatOption(int nTab, int nOptionIndex);
	void UpdateChatOption(int nTab);

	void ChangeMainTab(int nMainTab);
	void ChangeSubTab(int nSubTab);

	ENGINE_API void AddSysMessage(CTString strMsg, int nSysType = SYSMSG_NORMAL, COLOR col = 0xFFFFFFFF);
	ENGINE_API void AddChatMessage(UBYTE ubChatType, SLONG slSendIndex, CTString &strSendName,
		CTString &strChatMessage , BYTE channelNumber = 0, SLONG slGroup = -1);
	void AddUserNotice(CTString strOwner, CTString strMsg);
	void AddScheduleSystemMessage(time_t timeStart, time_t timeEnd, DWORD dwRepeatDelday, CTString strMessage, COLOR colMessage);

	void SendChatMessage( char *pcChatString, BOOL bLord = FALSE );

	void SpamerLift ( CTString& strName );	//해제

	void GetChatWindowEndPos(int &nX, int &nY);
	// save system msg
	void SaveSysMessageStart();
	void SaveSysMessageComplete(int iType = 0);
	void WriteSysMessage(const CTString& strSysMessage);

	void ProcessParty(char* szMsg);
	void PetTransform(char* szMsg);
	void WearingArmortest(char *szMsg);
	void TakeOffArmortest(char *szMsg);
	ENGINE_API void ShowNPCTalk(INDEX iMobIdx, INDEX iStrIndex );
	
	void CheckUserNoitce(int msgID);
	void PopupUserNotice();
	void LoadScheduleSystemMessage();
	void StopScheduleSystemMessage(INDEX idxEventID);
	static void CALLBACK	ScheduleSystemMessageCallback(UINT uTimerID, UINT uMsg, DWORD_PTR pdwUser, DWORD dw1, DWORD dw2);

	void OnUpdate( float fDeltaTime, ULONG ElapsedTime );

	void SetInputString(CTString strMsg);

	BOOL IsEditBoxFocused()
	{
		if (m_pChatInput == NULL)
			return FALSE;
		return m_pChatInput->IsFocused();
	}
	void KillFocusEditBox()
	{
		if( m_pChatInput )
			m_pChatInput->SetFocus(FALSE);
	}

	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnKeyMessage(MSG* pMsg);
	WMSG_RESULT OnCharMessage(MSG* pMsg);
	WMSG_RESULT OnIMEMessage(MSG* pMsg);

	void reg_gm_cmd();
	void SendChatCommand( char *pcChatString, int nLength );
	void CmdGMExeCmdLine( const char* filename );

#if		!defined(WORLD_EDITOR)
	typedef		std::function<bool(const char* args)>		_func_gm;
	typedef		std::map< std::string, _func_gm >			_map_gmCmd;
	typedef		std::map< std::string, _func_gm >::iterator _map_gmCmd_iter;

	_map_gmCmd	m_CmdGM;
#endif	// WORLD_EDITOR

	void SystemMessageColorInI();

private:
	void VisibleSysWindow(bool bHide);
	void ChatColorInI();
	void UpdateChatMsg(bool bSys, CUIList* pList, CTString strChat, COLOR colChat, CTString strName = "", COLOR colName = 0xFFFFFFFF);
	void AddWhisperTarget(CTString& strWhisper);

	void InsertChatPrefix( ChattingMsgType cmtType );

	// 스팸
	BOOL CheckSpamCount ( CTString& strName, CTString& strChat );
	void SpamerBlock ( CTString& strName );
	void ClearSpamMap ();
	void RemoveSpamerFromMap ( std::string& strTemp );

	int	CheckExpeditionChatGroup( char* pcChatString); // 원정대채팅시 그룹 채팅인지 체크
	int CheckChatType(char szFirst);

	BOOL CeilWritingCut_CompareStr(const CTString& strChat);
	void SetIsCeilWritingCheck(bool bCeil)			{ m_bIsCeilWritingCheck = bCeil; }
	void EraseListCeilWriting();

	BOOL CheckNearMyCharacter( FLOAT3D	vCharPos );
	void MakeLowChar(char* tmpChar);
	CTString ArrangeCharString(const CTString& strChat);
	void AddListCeilWriting(const CTString& str);
	BOOL CheckInputChannelChat(ChattingMsgType cmtType, CTString strMessage); // 채널 채팅 입력이 가능한지 확인

	void ConditionsysMsg(ChattingMsgType cmtType);
	void UpdateUserNotice(float fDeltaTime, ULONG ElapsedTime);
	void UpdateAlert();
	enum eRESIZING_WINDOW
	{
		eRW_SYS = 0,
		eRW_GENERAL,
		eRW_END
	};
	void ResizingWindow(eRESIZING_WINDOW eType, int val);
public:
	bool m_bSaveSysMsg;
	CTString m_strSaveSysMsgFileName;

private:
	FILE*			m_fpSaveSysMsg;

	enum eMAIN_CHAT_MODE
	{
		eMAIN_CHANNEL = 0,
		eMAIN_GENERAL,
		eMAIN_END
	};
	enum eCHAT_MODE
	{
		eCM_GENERAL = 0,
		eCM_GROUP,
		eCM_GUILD,
		eCM_TRANSACTION,
		eCM_END,
		eCM_WHISPER = eCM_END,
		eCM_SHOUT,
		eCM_SYSTEM,
		eCM_MAX
	};

	int m_nCurWhisper;
	int m_nWhisperCount;
	int m_nCeilWritingCheck;
	int	m_iCurrentMsgBuffer;
	int m_msgID;
	int m_nResizingOldValue;
	int m_nChatAlertType[eMAIN_END][eCM_END];

	bool m_bMouseInOption;
	bool m_bRearrangeOption;
	bool m_bChatOption[eCM_END][eCM_MAX];
	bool m_bChatAlert[eMAIN_END][eCM_END];
	bool m_bShowWhisperHistory;
	bool m_bActiveUserNotice;
	bool m_bClickUserNotice;
	bool m_bClickResizeWindow[eRW_END];
	bool m_bIsCeilWritingCheck;

	time_t m_timeUserNoticeDelay;
	
	CTString m_strWhisper[MAX_WHISPER_SAVE];
	CTString m_strWhisperTarget;
	CTString m_strUserNoticeOwner;
	
	COLOR	m_colSysType[SYSMSG_TOTAL];
	COLOR	m_colChat[CHATMSG_TOTAL];
	COLOR	m_colName[CHATMSG_TOTAL];
	COLOR	m_colExGroup[4];

	struct sChatSpamCheck
	{
		sChatSpamCheck()
		{		
			nCount						= 0;
			nIndex						= 0;
			llStartTime					= 0;
			llLongSentenceCheckTime		= 0;
			llCurTime					= 0;
			nStrLength					= 0;
			bBlocked					= false;		

		}
		__int64  llStartTime;
		__int64	 llCurTime;
		// 처음 입력 후 흐른 시간을 기준으로 누적켜서 1분을 확인 할 경우 사용.
		//ex) 30초동안 29번 입력하고 1분 후 다시 29번 입력한다면 1분마다 29번은 입력가능 그걸 막으려면 누적시간을 따져서 해야 할 듯 ) 
		__int64	   llLongSentenceCheckTime; //미국은 5초동안 일정량의 문자를 입력하면 블럭한다. 5초를 축적한다. 5초후에 다시 5초로 리셋 

		int		 nCount;
		int		 nIndex;
		int		 nStrLength;
		bool	bBlocked;	
	};

	struct sCeilWriting
	{
		sCeilWriting()
		{
			strCeilWriting = CTString("");
			iCnt		   = 0;
			llStartTime    = 0;
		}

		CTString		strCeilWriting;
		int				iCnt;
		__int64			llStartTime;
	};

	typedef struct _SScheduleSystemMessage
	{
		void*		pOwner;
		INDEX		idxEventID;
		DWORD		dwTimerID;
		CTString	strMessage;
		DWORD		dwRepeatDelay;
		time_t		timeDateStart;
		time_t		timeDateEnd;
		COLOR		colMessage;
		CTString	strLink;

		void operator = (const _SScheduleSystemMessage& oData)
		{
			pOwner			= oData.pOwner;
			idxEventID		= oData.idxEventID;
			dwTimerID		= oData.dwTimerID;
			strMessage		= oData.strMessage;
			dwRepeatDelay	= oData.dwRepeatDelay;
			timeDateStart	= oData.timeDateStart;
			timeDateEnd		= oData.timeDateEnd;
			colMessage		= oData.colMessage;
			strLink			= oData.strLink;
		}
	}SScheduleSystemMessage;

	typedef std::map<std::string, sChatSpamCheck> MAP_CHAT_RECV_COUNT;
	typedef MAP_CHAT_RECV_COUNT::iterator MAP_CHAT_RECV_COUNT_IT;

	MAP_CHAT_RECV_COUNT	m_mapChatCount; //분당 30회 확인

	std::list<sCeilWriting>		m_listCeilWriting;

	std::vector<CTString>		m_vecMsgBuffer;
	std::vector<SScheduleSystemMessage*>	m_vScheduleSystemMessage;

private:
	
	// 채널채팅, 일반채팅.
	CUITab*	m_pTabMainChat;
	CUITab*	m_pTabSubChat[eMAIN_END];
	
	CUITabPage* m_pTPMain[eMAIN_END];
	CUITabPage* m_pChatMode[eMAIN_END][eCM_END];

	CUIList* m_pListSys;
	CUIList* m_pListChat[eMAIN_END][eCM_END];

	CUIImage* m_pImgUserNotice;

	CUIImageSplit* m_pImgOptionBack;
	CUIImageSplit* m_pImgSysback;
	CUIImageSplit* m_pImgGeneralBack;

	CUIText* m_pTextOption[eCM_MAX];
	CUIText* m_pTextUserNotice;

	CUITextBoxEx* m_pTextBoxDesign;
	CUIEditBox* m_pChatInput;

	CUIButton*	m_pBtnOption;

	CUICheckButton* m_pChatOption[eCM_MAX];
	CUICheckButton* m_pCheckChannelChat;

	CUIBase* m_pResizingWindow[eRW_END];
};

#define		UICHAT()	CUIManager::getSingleton()->GetChattingUI()

#endif // __CHATTING_UI_H__