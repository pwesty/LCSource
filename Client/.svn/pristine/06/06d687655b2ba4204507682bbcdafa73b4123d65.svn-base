#ifndef __UILISTBOX_H__
#define __UILISTBOX_H__

#include <Engine/Interface/UIListBox.h>
#include <Engine/Interface/UICheckButton.h>
#include <Engine/Interface/UIImageBox.h>

#include <vector>
#include <map>

///ListBox내 그래픽 출력(혹은 버튼 출력)과 Popup-down을 처리하기 위한 class
class CUIListBoxEx : public CUIListBox
{
public:
	enum ePopState
	{
		PS_NONE,	//always show this.
		PS_CHILD,	//follow parent's state
		PS_EXTEND,	//parent
		PS_CLOSE,	//parent
		PS_CHECKCHILD,	// [090602: selo] child with checkbox
	};
public:
	CUIListBoxEx();
	~CUIListBoxEx();

	void Reset();

	void ChangeCurrentState(ePopState state);
	ePopState GetCurrentState()	{ return m_eCurrentState;	}
	ePopState GetState(int iLine)
	{
		if(iLine < 0 || iLine >= m_aPopLine.size()) return PS_NONE;
		return m_aPopLine[iLine].eState;
	}

	void Extend(int iLine);
	void Close(int iLine);

	void Render();

	WMSG_RESULT	MouseMessage( MSG *pMsg );

	void	SetPopBtnUV( UICheckBtnState bsState,
					FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_rtPopBtnUV[bsState].SetUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	SetPopBtnUV( UICheckBtnState bsState, UIRectUV &rt)
	{
		m_rtPopBtnUV[bsState].CopyUV(rt);
	}
	void	CopyUV( UICheckBtnState bsSrcState, UICheckBtnState bsDstState )
	{
		m_rtPopBtnUV[bsDstState].CopyUV( m_rtPopBtnUV[bsSrcState] );
	}
	void	SetPopBtnSpace(int sx, int sy)
	{
		m_iPopBtnSpaceX = sx;
		m_iPopBtnSpaceY = sy;
	}
	void	SetPopBtnSize(int width, int height)
	{
		m_iPopBtnWidth = width;
		m_iPopBtnHeight = height;
	}
	void	SetImageBox(int row, CUIImageBox::eImageType type, int index, BOOL bShowPopup = FALSE, CTString popupInfo = CTString(""), int nSyndiType = 0);

	// 2009. 06. 02 김정래	
	// CheckButton 을 가지는 Child 항목을 위해 
	// 관련 UI 함수 추가
	void	SetCheckBtnUV( UICheckBtnState bsState,
					FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_rtCheckBtnUV[bsState].SetUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	SetCheckBtnUV( UICheckBtnState bsState, UIRectUV &rt)
	{
		m_rtCheckBtnUV[bsState].CopyUV(rt);
	}	
	void	SetCheckBtnSpace(int sx, int sy)
	{
		m_iCheckBtnSpaceX = sx;
		m_iCheckBtnSpaceY = sy;
	}
	void	SetCheckBtnSize(int width, int height)
	{
		m_iCheckBtnWidth = width;
		m_iCheckBtnHeight = height;
	}
	int		GetCheckCount();		// 체크된 child 항목들의 수 반환
	int		GetLastCheckedIndex()	// 마지막에 체크된 항목의 인덱스를 반환
	{ 
		return m_iLastCheckedIndex;
	}
	void	SetCheckState(int index, BOOL bChecked);	// 해당 인덱스의 check 상태를 변경
	BOOL	GetCheckState(int index);
	int		GetLastChangedLine()
	{
		return m_iLastChangeLine;
	}

protected:
	void ChangeState(int iLine, ePopState state);//only for extend or close

	struct sPopLine
	{
		sPopLine(ePopState state, CUICheckButton *pBtn) : eState(state), pChkButton(pBtn) {}

		ePopState eState;
		CUICheckButton *pChkButton;
	};
	std::vector<sPopLine>			m_aPopLine;
	std::map<int, CUIImageBox*>		m_mapImageBox;
	ePopState						m_eCurrentState;
	int								m_iLastChangeLine;
	int								m_iPopBtnWidth;
	int								m_iPopBtnHeight;
	int								m_iPopBtnSpaceX;
	int								m_iPopBtnSpaceY;
	UIRectUV						m_rtPopBtnUV[UCBS_TOTAL];

	// 2009. 06. 02 김정래
	// CheckButton 을 가지는 Child 항목을 위해 
	// CheckButton 의 UI 변수들을 추가
	int								m_iCheckBtnWidth;
	int								m_iCheckBtnHeight;
	int								m_iCheckBtnSpaceX;
	int								m_iCheckBtnSpaceY;
	UIRectUV						m_rtCheckBtnUV[UCBS_TOTAL];
	// 2009. 06. 02 김정래
	// 체크된 항목의 개수에 제한을 두려 할 때 사용하기 위해
	// 제일 마지막에 체크한 인덱스를 저장한다.
	int								m_iLastCheckedIndex;		
};

#endif //__UILISTBOX_H__