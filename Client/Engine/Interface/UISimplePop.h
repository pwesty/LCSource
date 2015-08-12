#ifndef	UISIMPLEPOP_H_
#define	UISIMPLEPOP_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UITrackPopup.h>

// [sora] 원정대 command 추가
typedef enum _SimplePopState
{
	SPSO_NAME				= 0,			// 유저이름
	SPSO_WHISPER			= 1,			// 귓속말
	SPSO_FRIEND				= 2,			// 친구 등록
	SPSO_TRADE				= 3,			// 교환 신청
	SPSO_PARTY				= 4,			// 파티 신청 (일반 / 입수우선 / 전투형)
	SPSO_PARTY_OUT			= 5,			// 파티 강퇴
	SPSO_PARTY_JANG			= 6,			// 파티장 위임
	SPSO_GUILD_IN			= 7,			// 길드 가입
	SPSO_GUILD_WAR_START	= 8,			// 길드전투 신청
	SPSO_GUILD_WAR_END		= 9,			// 길드전투 중단
	SPSO_SETLABEL			= 10,
	SPSO_RESETLABEL			= 11,
	SPSO_CANCEL				= 12,			// 취소
	SPSO_END				= 13,			// SimplePopStateOne End

	SPST_START				= 14,			// SimplePopStateTwo End
	SPST_PARTY_JOIN_A		= SPST_START,	// 일반 파티 신청
	SPST_PARTY_JOIN_B		= 15,			// 입수우선 파티 신청
	SPST_PARTY_JOIN_C		= 16,			// 전투형 파티 신청

	SPST2_START				= 17,
	SPST2_SETLABEL1			= SPST2_START,
	SPST2_SETLABEL2			= 18,
	SPST2_SETLABEL3			= 19,
	SPST2_SETLABEL4			= 20,
	SPST2_SETLABEL5			= 21,
	SPST2_SETLABEL6			= 22,
	SPST2_SETLABEL7			= 23,
	SPSO_EXPEDITION_INVITE	= 24,			// 원정대 초대
	SPSO_EXPEDITION_KICK	= 25,			// 원정대 강퇴
	// brazil 요청 심플 팝업을 통한 채팅 블럭. [3/9/2011 rumist]
	SPSO_MUTE_CHARACTER		= 26,
} SIMPLE_POP_STATE;

class CUISimplePop : public CUITrackPopup
{
public:
//	INDEX m_iExeNum[SPSO_END];

	CUITrackPopup m_tpList;

	CTextureData*			m_ptdMenuTexture;			//툴팁에 사용될 화살표를 위한 텍스쳐
	CTextureData*			m_ptdExpeditionTexture;
	UIRectUV				m_rtArrow;					//화살표 텍스쳐 좌표
// [sora] RAID_SYSTEM	
	UIRectUV				m_rtLabelIcon[7];	// 표식지정 0 도끼, 1 유령, 2 발바닥, 3 조각칼, 4 똥, 5 악마, 6 해골 			
	bool					m_bTarget;
	CTString				m_strNoTargetName;
	int						m_nNoTargetCharIndex;
public:
	CUISimplePop();
	~CUISimplePop();

	void Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight);

	ENGINE_API void OpenPop(CTString strName, BOOL bParty, BOOL bGuild, INDEX ixPos, INDEX iyPos);
	ENGINE_API void OpenPopNoTarget(CTString strName, int nCharIndex, bool bParty, INDEX ixPos, INDEX iyPos); // 타겟없이 팝업창 열기
	void ClosePop();

	void Render();

	void SetExeList(CTString strUserName, BOOL bParty, BOOL bGuild);
	void SetExeListNoTarget(CTString strUserName, bool bParty);
	void SetSubExeList( INDEX iCommandNum );

	void Execution(INDEX exeNum);

	WMSG_RESULT	MouseMessage( MSG *pMsg );
};

#endif	// UISIMPLEPOP_H_