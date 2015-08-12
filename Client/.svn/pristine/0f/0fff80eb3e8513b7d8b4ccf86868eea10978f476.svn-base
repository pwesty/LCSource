#ifndef __PREMIUM_CHAR_UI_H__
#define __PREMIUM_CHAR_UI_H__

class CUIImage;
class CUIText;
class CUIList;
class CUIScrollBar;
class CUIButton;

class CPremiumCharDesign;

enum ePASSIVE_TYPE
{
	ePASSIVE_EXP_SP_UP = 0, // exp, sp 10% 증가
	ePASSIVE_LEVEL_DOWN, // 180랩 이하 모든 장비 -3 착용 레벨 감소
	ePASSIVE_AUCTION_NO_FEE, // 거래대행 수수료 무료
	ePASSIVE_CASH_INVEN1_USE, // 캐쉬 인벤 1번 가방 사용가능
	ePASSIVE_EXPRESS_REMOTE_USE, // 익스프레스 시스템 원격 사용가능
	ePASSIVE_CHANGE_FACE_PREE_USE, // 외형 변경 아이템 없이 사용 가능
};

class CPremiumCharUI : public CUIWindow
{
public:
	CPremiumCharUI();
	~CPremiumCharUI();

	void OpenUI();
	void CloseUI();

	void initialize();
	void updateUI();
	void updatePassive();
	void updateActive();
	void updateAc_PetRevival();
	void updateAc_Jump();
	void updateExpireTime();
	
	void PetRemoteRevival();
	void OpenMessenger();

	BOOL	CloseWindowByEsc()	{ CloseUI(); return TRUE; }

	WMSG_RESULT OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg );
	WMSG_RESULT OnLButtonUp( UINT16 x, UINT16 y );
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
private:

	CUIListItem* getPassiveItem(ePASSIVE_TYPE eItem);
	CTString getPassiveItemString(ePASSIVE_TYPE eItem);
	
	bool		m_bDrag;
	int			m_nOriX, m_nOriY;

	CUIImage*	m_pMoveArea;

	enum
	{
		eBTN_CLOSE = 0,
		eBTN_PET_REMOTE,
		eBTN_JUMP,
		eBTN_CANCEL,
		eBTN_MAX
	};

	enum
	{
		eLIST_PASSIVE = 0,
		eLIST_PET_REMOTE,
		eLIST_JUMP,
		eLIST_MAX
	};

	CPremiumCharDesign* m_pDesign;

	CUIButton*	m_pBtn[eBTN_MAX];
	CUIList*	m_pList[eLIST_MAX];
	CUIText*	m_pstrExpire;
	CUIScrollBar* m_plistScroll;
};	
#endif // __PREMIUM_CHAR_UI_H__