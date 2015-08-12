#ifndef	UI_TARGET_INFO_NEW_H_
#define	UI_TARGET_INFO_NEW_H_

class CUIImage;
class CUIText;
class CUIImageSplit;
class CUIImageArray;

// 툴팁 타입
enum eTARGET_TOOLTIP
{
	eTOOLTIP_FAME = 0,
	eTOOLTIP_ATTRATT,
	eTOOLTIP_ATTRDEF,
	eTOOLTIP_SYNDICATE,
	eTOOLTIP_MAX
};

class CTargetInfoUI : public CUIWindow
{
public:
	CTargetInfoUI();
	~CTargetInfoUI();

	// 현재 타겟의 속성 인덱스 체크
	void	CheckAttrIdx();

	// Buff
	void	SetTargetBuff( BOOL bMe );
	void	ShowPKTargetHP( BOOL bOn) { m_bShowPkHp = bOn; }

	// UI function
	void	OpenUI();
	void	CloseUI();
	void	UpdateUI();
	// 백그라운드 체크
	void	UpdateFrame();
	// 인포 셋팅
	void	UpdateTargetInfo();
	void	UpdateHPInfo();
	// 부가 정보 업데이트
	void	UpdateFameIcon();
	void	UpdateAttrIcon();
	// 알버맵
	void	UpdateSyndicateIcon();
	// 사이즈 조절
	void	UpdateFrameSize(int nWidth);

	// 툴팁 스트링 set
	void	UpdateTooltip(eTARGET_TOOLTIP eType);
	// --------------------------------------------<<

	void	initialize();
	void	OnPostRender(CDrawPort* pDraw);

	// MSG Proc
	WMSG_RESULT	OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT	OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT	OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT	OnRButtonDown(UINT16 x, UINT16 y);

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	BOOL	CloseWindowByEsc();

protected:
	enum eSUB_INFO_TYPE
	{
		eSUB_INFO_FAME = 0,
		eSUB_INFO_ATTR,
		eSUB_INFO_SYNDICATE
	};

	bool IsAddSubInfo(eSUB_INFO_TYPE eType);
	CTString _getFameTooltip(); // 명성 스트링
	CTString _getAttrTooltip(int attrType); // 속성 스트링
	CTString _getSyndicateTooltip(int& nWidth); // 결사대 스트링

	int		_getFame();
	void	_getAttr(int attrType, int& attr, int& attrLv);

	BOOL	m_bShowBuff;
	BOOL	m_bShowPkHp;
	BOOL	m_bShowHpBar;
	bool	m_bDrag;
	int		m_nOriX, m_nOriY;

	//이미지 타입
	enum eIMG_TYPE
	{
		eIMG_MOVE = 0,  // 타이틀바 이동
		eIMG_HP,		// HP 바
		eIMG_FAMEMARK,	// 명성 아이콘
		eIMG_MAX
	};

	CUIImage* m_pImg[eIMG_MAX];

	//스트링 타입
	enum eTEXT_TYPE
	{
		eTEXT_LEVEL = 0, // 타겟 레벨
		eTEXT_HP,		 // 서치 라이프 사용 효과
		eTEXT_MAX
	};

	CUIText* m_pText[eTEXT_MAX];

	//Frame 타입
	enum eFRAME_TYPE
	{
		eFRAME_NORMAL = 0, // NPC, PC 배경
		eFRAME_MOB,		// 일반 몹 배경
		eFRAME_PMOB,	// 엘리트, 보스, 파티 몹 배경
		eFRAME_MAX
	};
	
	CUIImageSplit* m_pImgSplit[eFRAME_MAX];
	CUIText* m_pStrName[eFRAME_MAX];		
	eFRAME_TYPE m_eBackGround;

	int		m_nFrameCenterWidth[eFRAME_MAX];
	int		m_nNameWidth[eFRAME_MAX];
	int		m_nAddWidthGap;
	int		m_nHpBarWidth;

	// 이미지 어레이 타입
	enum eIMG_ARRAY_TYPE
	{
		eARRAY_ATTR_ATT = 0, // 공격 속성
		eARRAY_ATTR_DEF,	// 방어속성
		eARRAY_SYNDICATE,	// RVR
		eARRAY_MAX
	};

	CUIImageArray* m_pImgArr[eARRAY_MAX];
};


#endif	// UI_TARGET_INFO_NEW_H_

