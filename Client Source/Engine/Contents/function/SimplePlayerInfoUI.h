#ifndef __SIMPLE_PLAYER_INFO_UI_H__
#define __SIMPLE_PLAYER_INFO_UI_H__

class CSimplePlayerInfoUI : public CUIWindow
{
public:
	CSimplePlayerInfoUI();
	~CSimplePlayerInfoUI();

	void initialize();

	// Hp, Mp, Exp Update
	ENGINE_API void UpdatePlayerBaseInfo();

	ENGINE_API void UpdateHPMP();
	void UpdateEXP();
	void UpdateEP();

	// 나이트셰도우 전용.
	void UpdateSoul();

	void SetPlayerName();
	void SetPlayerLevel();
	void SetPlayerFace();

	void StartDungeonTime();
	void StopDungeonTime();
	void SetDungeonList(SLONG slExp);

	void SetPremiumChar(char szType);

	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnRButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);

private:
	enum 
	{
		eHP = 0,
		eMP,
		eEXP,
		eMAX
	};
	CUIProgressBar* m_pProgressBar[eMAX];
	CUIText*		m_pText[eMAX];

	enum
	{
		eEP_LV0 = 0,
		eEP_LV1,
		eEP_LV2,
		eEP_LV3,
		eEP_LV4,
		eEP_MAX
	};
	
	CUIButton*	m_pBtnEp;
	CUIImage*	m_pImgEpGage;
	CUIImageArray*	m_pImgSoulBack;
	CUIImageArray*	m_pImgBaseBack;
	CUIImageArray*	m_pImgEPBack;
	CUIImage*	m_pImgSoulGage[10];
	CUIList*	m_pListDungeon;
	CUISpriteAni* m_pAniDungeonTime;

	// 던전 타임 팝업 배경
	CUIImageSplit* m_pImgDTPBack;

	UIRectUV	m_uvEPGage[eEP_MAX];
	UIRectUV	m_uvEPBtn_Idle[eEP_MAX];
	UIRectUV	m_uvEPBtn_Click[eEP_MAX];

	UIRect		m_rcMouseEventArea;
};

#endif //__SIMPLE_PLAYER_INFO_UI_H__