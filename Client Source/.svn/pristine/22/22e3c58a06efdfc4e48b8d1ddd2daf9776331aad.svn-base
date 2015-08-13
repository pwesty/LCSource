// UIChildQuickSlot.h: interface for the UIChildQuickSlot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UICHILDQUICKSLOT_H__5712C6A8_FA0C_47ED_841B_D4226A464533__INCLUDED_)
#define AFX_UICHILDQUICKSLOT_H__5712C6A8_FA0C_47ED_841B_D4226A464533__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define QSLOT_VER_BASE_TEXTURE_STARTX		102
#define QSLOT_VER_BASE_TEXTURE_STARTY		515
#define QSLOT_VER_BASE_TEXTURE_ENDX			555
#define	QSLOT_VER_BASE_TEXTURE_ENDY			552

#define QSLOT_HORZ_BASE_TEXTURE_STARTX		0
#define QSLOT_HORZ_BASE_TEXTURE_STARTY		515
#define QSLOT_HORZ_BASE_TEXTURE_ENDX		37
#define	QSLOT_HORZ_BASE_TEXTURE_ENDY		968

// [12/10/2012 박훈] UIQuickslot 개편
#define	CHILD_QUICKSLOT_WIDTH			453
#define	CHILD_QUICKSLOT_HEIGHT			37

// 아이템 슬롯 영역 [1/2/2013 Ranma]
#define CHILD_SLOT_TITLE_SIZE			15
#define CHILD_SLOT_TAIL_SIZE			20
#define CHILD_QUICKSLOT_RCSLOT_AREA		((CHILD_QUICKSLOT_WIDTH) - (CHILD_SLOT_TAIL_SIZE))

// rotation버튼과 lock 버튼 사이즈가 같으므로 한가지로 사용
#define QSLOT_OUTSIDE_BTN_SIZE			17

// ----------------------------------------------------------------------------
// Name : CUIChildQuickSlot
// Desc :
// ----------------------------------------------------------------------------
class CUIChildQuickSlot  : public CUIWindow
{
private:
	UIRectUV			m_rtVerBackground;									// UV of vertical background
	UIRectUV			m_rtHorzBackground;									// UV of horizontal background
	UIRectUV			m_rtVerSlotNumber;
	UIRectUV			m_rtHorzSlotNumber;
	// UV of rotation
	UIRectUV			m_rtVerOver;
	UIRectUV			m_rtVerClick;
	UIRectUV			m_rtHorzOver;
	UIRectUV			m_rtHorzClick;
	// UV of lock
	UIRectUV			m_rtLockIdle;
	UIRectUV			m_rtLockOver;
	UIRectUV			m_rtLockClick;
	UIRectUV			m_rtUnLockIdle;
	UIRectUV			m_rtUnLockOver;
	UIRectUV			m_rtUnLockClick;

	CTString			m_strPageNumber;
	int					m_nID;
	BOOL				m_bEXSlot1On;
	BOOL				m_bEXSlot2On;
	int					m_nSelSlotBtnID;								// Selected button ID of quick slot
	int					m_nStartX, m_nStartY;
	BOOL				m_bIsInside;

public:
	enum ROTATION { ROT_VERTICAL = 0, ROT_HORIZONTAL, ROT_END };
	enum CHILDQSLOT { CHILD_QS_1 = 0, CHILD_QS_2, QS_END };
	INDEX*				m_bRotation[QS_END];
	INDEX*				m_bLock[QS_END];
	int					m_nCurSlot;

	UIRect				m_rcDragArea;									// Region of DragArea
	UIRect				m_rcSlot;										// Region of slot
	// button
	abtnItems*			m_abtnItems;
	CUIButton			m_btnVertical;
	CUIButton			m_btnHorizontal;
	CUIButton			m_btnLock;
	CUIButton			m_btnUnLock;
public:
	CUIChildQuickSlot(int nid);
	 ~CUIChildQuickSlot();

public:
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	Render();
	void	SlotRender();
	void	SlotNumberRender();
	
	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	ResetSavePosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	void	Move( int ndX, int ndY ) { m_nPosX += ndX; m_nPosY += ndY; }
	void	Rotation();
	void	RotationRectSet();
	void	SetItem(abtnItems* btnItems); // 퀵슬롯과 아이템 연결
	void	OpenUI();
	void	_CloseUI();
	void	ToggleVisible(const BOOL bResultView /* = false */);
	void	SetPosEXQSlot();

	void	FirstOpen();
private:
	void	CheckOnOff();	

};

#endif // !defined(AFX_UICHILDQUICKSLOT_H__5712C6A8_FA0C_47ED_841B_D4226A464533__INCLUDED_)
