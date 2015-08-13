#ifndef __CUICHARACTER_CREATE_NEW_H__
#define __CUICHARACTER_CREATE_NEW_H__

enum eCreateBtnType
{
	BTN_TYPE_PREV = 0,
	BTN_TYPE_NEXT,
};

enum eUI_BASE_TYPE
{
	UI_BASE_TOP = 0,
	UI_BASE_BOTTOM,
	UI_BASE_LEFT,
	UI_BASE_RIGHT,
	UI_BASE_MAX,
};

enum eUI_Ani_state
{
	UI_ANI_NONE = -1,
	UI_ANI_START = 0,
	UI_ANI_IDLE1,
	UI_ANI_IDLE2,
	UI_ANI_SOCIAL,
};

class ENGINE_API CUICharacterCreateNew : public CUIWindow
{
public:
	CUICharacterCreateNew(void);
	virtual ~CUICharacterCreateNew(void);

	// interface
	void	OpenUI();
	void	CloseUI();
	void	ReturnUI();

	// command
	void	SelJob(eCreateBtnType eType);
	void	SelectSocial();
	void	SelectArmor();
	void	SelectFace();
	void	SelectHair();
	void	SelSocial(eCreateBtnType eType);

	void	Zoom(eCreateBtnType eType);
	void	Rotation(eCreateBtnType eType);
	void	PressOk();
	void	SetCharaterAngle(FLOAT f_inL, FLOAT f_inR);
	// reset
	void	ResetName();
	void	ResetSelect();
	void	ResetWearing(int nJob);
	void	ResetModelPos(int nJob, BOOL bDefault, BOOL bZoomIn = FALSE);

	void	Lock(BOOL bLock);

	//--------------------------- 
	void	initialize();
	void	OnUpdate( float fDeltaTime, ULONG ElapsedTime );
	void	OnUpdatePosition();
	//---------------------------
	WMSG_RESULT OnKeyMessage(MSG* pMsg);
	WMSG_RESULT OnCharMessage(MSG* pMsg);
private:
	//---------------------------
	CTString GetClassDesc(eJob eClass);
	CTString GetClass2Desc(eJob eClass, eJob2 eClass2 );
	void	HideWeapon(BOOL bHide, int nJob);
	int		CheckAnim(int nJob);
	void	UpdateAnim(int nJob);
	void	SetSelectJobInfo();
	void	SetDefaultArmorWearing();
	void	SetArrayList(CUIArray* pArray);
	void	SetSocialInfo();
	void	CreateCharacter();
	void	ChangeHair(int idx);
	void	ChangeFace(int idx);
	void	RenderTargetModel_Wearing( INDEX iJob, int nItemIdx, int nWearPos );
	int		GetClassSex(int nJob);
	int		GetActionAniidx(int ActionIdx);
	
	//---------------------------
	void	initializeCenterUI();
	void	initializeLeftUI();
	void	initializeRightUI();

	CUIBase*		m_pUIBase[UI_BASE_MAX];
	// Center Top UI
	CUIImageArray*	m_pImgArrSelJob_left;
	CUIImageArray*	m_pImgArrSelJob_center;
	CUIImageArray*	m_pImgArrSelJob_right;
	CUIText*		m_pstrSelJob;
	CUIButton*		m_pbtnSelJob_left;
	CUIButton*		m_pbtnSelJob_right;

	// Center Bottom UI
	CUIButton*		m_pbtnRot_left;
	CUIButton*		m_pbtnRot_right;
	CUIButton*		m_pbtnZoom_In;
	CUIButton*		m_pbtnZoom_Out;
	CUIButton*		m_pbtnChar_Create;
	CUIEditBox*		m_pebCharName;

	// Left UI
	CUIText*		m_pstrJob;
	CUITextBox*		m_ptbJobDesc;
	CUIText*		m_pstrDualJob;
	CUITextBox*		m_ptbDualJobDesc;
	CUIScrollBar*	m_pscrollJobDesc;
	CUIScrollBar*	m_pscrollDualJobDesc;

	// Right UI
	CUIArray*		m_pArrayFace;
	CUIButton*		m_pbtnFace_left;
	CUIButton*		m_pbtnFace_right;

	CUIArray*		m_pArrayHair;
	CUIButton*		m_pbtnHair_left;
	CUIButton*		m_pbtnHair_right;

	CUIArray*		m_pArrayWear;
	CUIButton*		m_pbtnWear_left;
	CUIButton*		m_pbtnWear_right;

	CUIArray*		m_pArraySocial;
	CUIButton*		m_pbtnSocial_left;
	CUIButton*		m_pbtnSocial_right;

	// Main UI
	CUIButton*		m_pbtnClose;
	CUIImage*		m_pMouseWheel;

	FLOAT			m_fZSpeed, m_fYSpeed;
	FLOAT			m_fAngle;

	eUI_Ani_state	m_eAniState;
	int				m_nPlayActionId;
		
	FLOAT			m_fMinDistance, m_fMaxDistance;
	FLOAT			m_fMinHeight, m_fMaxHeight;
};



#endif	//	__CUICHARACTER_CREATE_NEW_H__