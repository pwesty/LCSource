#ifndef __CITEMCOMPOSE_UI_H__
#define __CITEMCOMPOSE_UI_H__

enum eCOMPOSE_POPUP_TYPE
{
	eCOMPOSE_POPUP_DONT_IN_DUNGEON = 0,
	eCOMPOSE_POPUP_NOT_ENOUGH_NAS,
	eCOMPOSE_POPUP_NOT_ENOUGH_MAT,
	eCOMPOSE_POPUP_HAVE_NOT_RELIC,
	eCOMPOSE_POPUP_NO_COMPOSE_ITEM,
	eCOMPOSE_POPUP_EQUIP_ITEM,
	eCOMPOSE_POPUP_COMPOSE_FAIL,
	eCOMPOSE_POPUP_MAX
};

class CItemComposeUI : public CUIWindow
{
public:
	CItemComposeUI(void);

	enum eSTATE
	{
		eNONE = 0,
		eREGIST,
		ePROGRESSING,
		eCOMPLETE,
		eSTATE_MAX
	};

	enum eBTN_TYPE
	{
		eBTN_CLOSE = 0,
		eBTN_OK,
		eBTN_CANCEL,
		eBTN_MAX
	};

	enum eICON_TYPE
	{
		eICON_MAT1 = 0,
		eICON_MAT2,
		eICON_MAT3,
		eICON_MAT4,
		eICON_MAT5,
		eICON_RESULT,
		eICON_MAX
	};

	enum eCOMPOSE_GUIDE_TYPE
	{
		eGUIDE_BASE = 0,
		eGUIDE_ALL_READY,
		eGUIDE_NOT_ENOUGH_NAS,
		eGUIDE_COMPOSE_OK,
		eGUIDE_COMPOSE_FAIL,
		eGUIDE_MAX
	};

	void openUI();
	void closeUI();
		 
	void PressOK();
	void UpdateResultIcon(int ItemIdx = -1);
	void ShowErrorPopup(int nErrorIdx);
	void ShowGuideString(int nIdx = -1);
	void ClearIcon();
		 
	void initialize();
	void OnUpdate( float fDeltaTime, ULONG ElapsedTime );
	
	// Reset position
	void ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Msg Proc
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnRButtonDown(UINT16 x, UINT16 y);
private:
	void			StartProgress();
	void			SendEvent(); // 프로그래스 완료시 보내는 이벤트
	LONGLONG		CalcConsumNas(); // 소비 나스 계산
	void			CalcProgressTimer();

	void			SlotRegistCheck(); // 빈슬롯에 재료아이템 등록 체크 첫번재 슬롯부터 체워짐
	void			UpdateIconBg(); // 아이템 등록시 슬롯 BackGround 업데이트
	void			UpdateNasString();
	void			CheckOkBtn(); // Ok버튼 enable 체크
	bool			IsComposeItem(int nIdx); //등록가능한 아이템인지 체크 
	bool			IsCheckIcon(int nIndex); // 슬롯이 비었는지 검사
	bool			IsRegist(int nSlotIdx, int nItemIdx);
	bool			IsAlreadyRegist(int nVirIndex); // 이미 등록된 아이템인지 체크
	
	bool			SetBtnState(eBTN_TYPE eType, BOOL bEnable); // 버튼 상태 변경
	void			SetState(eSTATE eState); // UI 상태 변경
	
	eSTATE			m_eState; // 가이드 상태
	bool			m_bDrag;
	int				m_nOriX, m_nOriY;
	UIRect			m_rcOriginPos;

	BOOL			m_bProgress; // 프로그래스 상태
	__int64			m_uStartTime;
	__int64			m_uEndTime;
	
	LONGLONG		m_llNas; // 소비 나스 표시용
	
	CUIText*		m_pStrTitle;
	CUITextBox*		m_pTbStateGuide;
	CUIImage*		m_pDrag;
	CUIProgressBar* m_pPgBar;
	CUIButton*		m_pBtn[eBTN_MAX];
	CUIIcon*		m_pIcon[eICON_MAX];
	CUIImage*		m_pIconBg[eICON_MAX];
};

#endif	//	__CITEMCOMPOSE_UI_H__
