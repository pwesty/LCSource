#ifndef UI_PETSTASH_H_
#define UI_PETSTASH_H_

enum eMSG_PetStash_Type
{
	MSG_TYPE_INSTASH = 0,
	MSG_TYPE_OUTSTASH,
};

class CUIListItem;
class CUIArrayItem;
class CUIArray;
class CUITab;
class CUIText;
class CUICheckButton;
class CUIList;
class CUIIcon;

class CUIPetStash : public CUIWindow
{
public:
	CUIPetStash();
	~CUIPetStash();
	////// interface /////////////////
	void	OpenPetStash();
	void	ClosePetStash();

	void	UpdateUI();
	void	UpdateReuseTime();

	void	ClearUI();

	////// Command //////////////////
	// 대리육성 아이템 클릭
	void	SetArrayItemClick();
	// 리스트 펫 클릭
	void	SetListItemClick();
	// 탭 클릭
	void	ChangeTab();
	// 대리육성 등록 클릭
	void	SelectPet();
	// 맡기기, 찾기 버튼 클릭시
	void	CreateMSGBox(eMSG_PetStash_Type type);

	//////////////////////////////////
	void	initialize();
		
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	
	BOOL	CloseWindowByEsc()	{ ClosePetStash(); return TRUE; }
		
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	// Msg Proc
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);

private:
	//-------------------------------------------------------------------
	// 탭페이지1 펫 리스트 아이템 관련

	// 펫 리스트 아이템 업데이트
	BOOL	_UpdatePetListStash();
	// 펫 리스트 아이템 셋팅
	BOOL	_SetPetStashList(CUIListItem* pListItem, int PetNum);

	// 펫 리스트 아이템 클리어
	void	_ClearPetStashList(CUIListItem* pListItem);
	void	_ClearImage(CUIListItem* pListItem);
	
	//--------------------------------------------------------------------
	// 탭페이지2 대리육성 아이템 관련

	// 대리육성 펫 리스트 업데이트
	BOOL	_UpdateDeputyList();
	// 대리육성 펫 리스트 셋팅
	BOOL	_SetDeputyList(CUIArrayItem* pArrayItem, int PetNum);

	// 대리육성 펫 리스트 클리어
	void	_ClearDeputyList(CUIArrayItem* pArrayItem);	
	void	_ClearSelectPet();

	// titleMove
	bool			m_bTitleClick;
	UIRect			m_rcOriginPos;
	bool			m_bDrag;
	int				m_nOriX, m_nOriY;

	//main
	CUITab*			m_pTab;
	CUIText*		m_pTxtTitle;
	CUIText*		m_pMoveTitle;
	CUIText*		m_pTxtReuseTime;
	CUIButton*		m_pbtnClose;
	CUICheckButton*	m_pbtnPetstash;
	CUICheckButton*	m_pbtnPetDeputy;

	//탭페이지1 펫 리스트
	CUIButton*		m_pbtnInStash;
	CUIButton*		m_pbtnOutStash;
	CUIText*		m_pTxtPetCount;
	CUIList*		m_pListPetInfo;
	CUIScrollBar*	m_pListScroll;

	//탭페이지2 대리육성
	CUIButton*		m_pbtnSelect;
	CUIIcon*		m_pIconSelectPet;
	
	CUIText*		m_pTxtPetName;
	CUIText*		m_pTxtPetLevel;
	CUIText*		m_pTxtPetExp;
	CUIText*		m_pTxtPetLoyalty;
	CUIText*		m_pTxtPetHungry;
	CUIArray*		m_pPetArray;
};
#endif // UI_PETSTASH_H_