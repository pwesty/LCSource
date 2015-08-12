#ifndef __CUIDURABILITY_H__
#define __CUIDURABILITY_H__

class CUIDurability : public CUIWindow
{
public:
	enum eSTATE // 상태 가이드 
	{
		eNONE = 0,
		eREGISTER_ITEM,
		eREGISTER_MATERIAL,
		ePROGRESSING,
		eCOMPLETE,
		eSTATE_MAX
	};

	enum eOPEN_TYPE
	{
		eDURABILITY = 0,
		eRECOVERY,
		eOPEN_MAX
	};

	enum eBTN_TYPE
	{
		eFIRST = 0,
		eSECOND,
		eBTN_MAX
	};

	enum eICON_TYPE
	{
		eICON_ITEM = 0,
		eICON_MATERIAL,
		eICON_RESULT,
		eICON_MAX
	};

	enum eMSG_BOX
	{
		eMSG_REPAIR = 0,
		eMSG_SPECIAL_REPAIR,
		eMSG_RECOVER,
		eMSG_ITEMCOUNT,
		eMSG_TYPE_MAX,
		eMSG_ERROR_NO_DURABILITY = 10,
		eMSG_ERROR_NOT_ENOUGH_MONEY,
		eMSG_ERROR_NOT_ENOUGH_ITEM,
		eMSG_ERROR_WEARITEM,
		eMSG_ERROR_NO_MATERIAL,
		eMSG_ERROR_NOT_DAMAGED_ITEM,
		eMSG_ERROR_NO_RECOVER_ITEM,
		eMSG_ERROR_OVER_RECOVER,
	};

	CUIDurability(void);
	virtual ~CUIDurability(void);

	void	Open(eOPEN_TYPE eType);
	void	Close();

	void	initialize();
	void	OnUpdate( float fDeltaTime, ULONG ElapsedTime );

	void	PressBtn(eBTN_TYPE eType);
	void	SendEvent(eBTN_TYPE eType);
	void	UpdateResultIcon(UWORD tab, UWORD InvenIdx);
	void	ClearIcon();

	void	CheckBtnState(eICON_TYPE eType);

	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void	ShowMsgBox(eMSG_BOX eType);
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );

	WMSG_RESULT OnKeyMessage(MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
private:
	void			SetUI();
	void			StartProgress(eBTN_TYPE eType);
	LONGLONG		CalcConsumNas();
	void			CalcProgressTimer();
	
	void			SetState(eSTATE eState);
	CTString		GetStateString(eSTATE eState);

	BOOL			ItemCheck(eICON_TYPE eType);
	void			SetBtnState(eBTN_TYPE eType, BOOL bEnable);
		
	eOPEN_TYPE		m_eOpenType;
	eSTATE			m_eState;
	bool			m_bDrag;
	int				m_nOriX, m_nOriY;
	UIRect			m_rcOriginPos;

	BOOL			m_bProgress;
	__int64			m_uStartTime;
	__int64			m_uEndTime;
	eBTN_TYPE		m_eEventType;

	LONGLONG		m_llNas;
	int				m_nMaterialCount;

	CUIText*		m_pStrTitle;
	CUITextBox*		m_pTbStateGuid;
	CUIImage*		m_pImgRecover;
	CUIImage*		m_pDrag;
	CUIProgressBar* m_pPgBar;
	CUIButton*		m_pBtnClose;
	
	CUIButton*		m_pBtn[eBTN_MAX];
	CUIIcon*		m_pIcon[eICON_MAX];
};

#endif	//	__CUIDURABILITY_H__
