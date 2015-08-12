#ifndef	UITATOO_H_
#define	UITATOO_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// Include Files
// #include <Engine/Interface/UIWindow.h>
// #include <Engine/Interface/UIButton.h>
// #include <Engine/Interface/UIListBox.h>
// #include <Engine/Interface/UIEditBox.h>

// Define size of Tatoo
// 텍스쳐 왼쪽 위 (254, 609)
#define	TATOO_WIDTH						256
#define	TATOO_HEIGHT					352

#define TATOO_TOTAL_ROW					6
#define TATOO_TOTAL_COL					2

//------------------------------------------------------------------------------
// CUISingleBattle
// Explain:  길드 전투 
//------------------------------------------------------------------------------
class ENGINE_API CUITatoo : public CUIWindow
{
public:
	struct sTatooButton
	{
		CUIButton	btnButton;
		COLOR		color;
		bool		exist;

		sTatooButton() : color(0xFFFFFFFF), exist(false)
		{
			
		}

		void Render()
		{
			btnButton.Render(color);
		}
	};

protected:
	// Region of each part
	UIRect				m_rcTitle;

	// UV
	UIRectUV			m_rtMain;
	
	// Buttons
	CUIButton			m_btnClose;							
	CUIButton			m_btnOK;							
	CUIButton			m_btnCancel;	
	
	CTextureData*		m_ptdCommonBtnTexture;
	CTextureData*		m_ptdNewInventoryTexture;
	
	sTatooButton		m_btnColor[TATOO_TOTAL_ROW][TATOO_TOTAL_COL];
	sTatooButton		m_btnShape[TATOO_TOTAL_ROW][TATOO_TOTAL_COL];

	// Properties
	int					m_iSelectedColorIndex;		// 선택된 색상 인덱스
	int					m_iSelectedShapeIndex;		// 선택된 문양 인덱스

public:

	CUITatoo();
	~CUITatoo();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	Clear();	//
	void	Close();	// 종료

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void	SetFocus( BOOL bVisible );
	void	OpenTatoo();
	void	SendItemUse();				// 선택한 색상 및 문양을 보낸다

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );	

	// Render
	void	Render();					// Render	

	void	RenderColorButtons();
	void	RenderShapeButtons();

	void	RenderSelectedColorOutline();
	void	RenderSelectedShapeOutline();

private:
	void	AddColorButton(int iRow, int iCol, COLOR color);				// 색상 버튼 추가
	void	AddShapeButton(int iRow, int iCol, int texLeft, int texTop);	// 모양 버튼 추가

	void	CheckShowShape();			// 현재 선택한 색상에서 문양을 선택 할 수 있는지 체크	
	
	int		GetColorAndShapeIndex();	// 서버로 보낼 문양 인덱스를 얻는다.
	void	MakeMessageBox();			// 메시지 박스를 만든다
};

#endif	// UIGB_H_