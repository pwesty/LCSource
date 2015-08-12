#include "stdh.h"
#include <Engine/Interface/UIMultList.h>
#include <Engine/Interface/UITextureManager.h>

//------------------------------------------------------------------------------
// CUIMultiList::CUIMultiList
// Explain:
// Date : 2005-05-17
// Author: Lee Ki-hwan
// Modified: [2013/02/06] sykim70
//------------------------------------------------------------------------------
CUIMultiList::CUIMultiList()
	: m_ptdCommonBtnTexture(NULL)
	, m_nLineGap(0)
	, m_nIconOffset(0)
{
}

//------------------------------------------------------------------------------
// CUIMultiList::~CUIMultiList
// Explain:
// Date : 2005-05-17
// Author: Lee Ki-hwan
// Modified: [2013/02/06] sykim70
//------------------------------------------------------------------------------
CUIMultiList::~CUIMultiList()
{
	ResetAllData();

	Destroy();

	STOCK_RELEASE(m_ptdCommonBtnTexture);
}

//------------------------------------------------------------------------------
// CUIMultiList::Create
// Explain:  MultiList생성시 추가 작업 필요로
// Date : 2005-05-18
// Author: Lee Ki-hwan
// Modified: [2013/02/06] sykim70
//------------------------------------------------------------------------------
void CUIMultiList::Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight,
						  int nLineHeight, int nSpaceX, int nSpaceY, int nColumn, BOOL bSelectList,
						  int nLineGap, int nIconOffset)
{
	CUIListBox::Create(pParentWnd, nX, nY, nWidth, nHeight, nLineHeight, nSpaceX, nSpaceY, nColumn, bSelectList);

	m_nLineGap = nLineGap;
	m_nIconOffset = nIconOffset;

	// Todo :~ 추가된 데이터 타입에 따른 추가 작업...
	m_vecIcon.reserve(nColumn);
	m_vecConType.reserve(nColumn);

	for (int iCol = 0; iCol < nColumn; iCol++)
	{
		sVecIcon sVecTempIcon;
		m_vecIcon.push_back(sVecTempIcon);
		m_vecConType.push_back(TYPE_TEXT);
	}

	//CreateScroll(bRight, nOffsetX, nOffsetY, nWidth, nHeight, nBtnWidth, nBtnHeight, nBarWidthGap, nBarHeightGap, nBarVertEdgeSize);
	CreateScroll(TRUE, 0, 0, 10, nHeight, 10, 10, 0, 0, 10);

	m_ptdCommonBtnTexture = CreateTexture(CTString("Data\\Interface\\CommonBtn.tex"));

	// Get Button Texture Size.
	FLOAT fTexWidth = m_ptdCommonBtnTexture->GetPixWidth();
	FLOAT fTexHeight = m_ptdCommonBtnTexture->GetPixHeight();

	// Up button
	SetScrollUpUV(UBS_IDLE, 156, 33, 166, 43, fTexWidth, fTexHeight);
	SetScrollUpUV(UBS_CLICK, 168, 33, 178, 43, fTexWidth, fTexHeight);
	CopyScrollUpUV(UBS_IDLE, UBS_ON);
	CopyScrollUpUV(UBS_IDLE, UBS_DISABLE);
	// Down button
	SetScrollDownUV(UBS_IDLE, 156, 45, 166, 55, fTexWidth, fTexHeight);
	SetScrollDownUV(UBS_CLICK, 168, 45, 178, 55, fTexWidth, fTexHeight);
	CopyScrollDownUV(UBS_IDLE, UBS_ON);
	CopyScrollDownUV(UBS_IDLE, UBS_DISABLE);
	// Bar button
	SetScrollBarTopUV(185, 32, 195, 42, fTexWidth, fTexHeight);
	SetScrollBarMiddleUV(185, 41, 195, 51, fTexWidth, fTexHeight);
	SetScrollBarBottomUV(185, 61, 195, 71, fTexWidth, fTexHeight);
}

//------------------------------------------------------------------------------
// CUIMultiList::Create
// Explain:  MultiList생성시 추가 작업 필요로
// Date : 2013/02/06
// Author: sykim70
//------------------------------------------------------------------------------
WMSG_RESULT	CUIMultiList::MouseMessage(MSG *pMsg)
{
	if (pMsg->message == WM_MOUSEMOVE)
	{
		int nColumn = 0;
		int	nRowS = m_sbScrollBar.GetScrollPos();
		int	nRowE = nRowS + m_nLinePerPage;
		if (nRowE > m_vecIcon[nColumn].vecIcon.size())
			nRowE = m_vecIcon[nColumn].vecIcon.size();

		for (int nList = nRowS; nList < nRowE; nList++)
		{
			CUIImageBox* pImageBox = m_vecIcon[nColumn].vecIcon[nList];
			//pImageBox->SetPos(nIconX, nIconY);
			pImageBox->MouseMessage(pMsg);
		}
	}
	return CUIListBox::MouseMessage(pMsg);
}
//------------------------------------------------------------------------------
// CUIMultiList::SetColumnType
// Explain:  
// Date : 2005-05-18
// Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiList::SetColumnType(int nColumn, CONTENT_TYPE eContentType)
{
	m_vecConType[nColumn] = eContentType;
}

//------------------------------------------------------------------------------
// CUIMultiList::Render
// Explain:  기본 루틴 그대로 가져옴.. 일부 수정
// Date : 2005-05-18
// Author: Lee Ki-hwan
// Modified: [2013/02/06] sykim70
//------------------------------------------------------------------------------
void CUIMultiList::Render()
{
	if (IsVisible() == FALSE)
		return;

	// Get position
	int	nX, nY;
	GetAbsPos(nX, nY);

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	pDrawPort->InitTextureData(m_ptdCommonBtnTexture);

	// Add render regions
	// Selection bar
	if (m_bShowSelectBar && m_nSelectList >= 0)
	{
		int	nSelLine = m_nSelectList - m_sbScrollBar.GetScrollPos();
		if (nSelLine >= 0)
		{
			//!m_bScrollBar ||	
			if (nSelLine < m_nLinePerPage) // Date : 2005-05-23(오후 5:26:26), By Lee Ki-hwan
			{
				int	nBarY = nY + m_nTextSY + nSelLine * m_nLineHeight;
				pDrawPort->AddTexture(nX + m_rcSelectOver.Left, nBarY + m_rcSelectOver.Top,
									nX + m_rcSelectOver.Right, nBarY + m_rcSelectOver.Bottom,
									m_rtSelectOver.U0, m_rtSelectOver.V0,
									m_rtSelectOver.U1, m_rtSelectOver.V1,
									0xFFFFFFFF);
			}
		}
	}

	// Scroll bar
	if (m_bScrollBar)
		m_sbScrollBar.Render();

	pDrawPort->FlushRenderingQueue();

	// Text부분만 아니라 Icon부분도 렌더링
	// Text in list box
	for (int nCol = 0; nCol < m_vecString.size(); nCol++)
	{
		switch (m_vecConType[nCol])
		{
		case TYPE_TEXT:
			RenderString(nCol, nX, nY);
			break;
		case TYPE_ICON:
			RenderIcon(nCol, nX, nY);
			break;
		}
	}
}

//------------------------------------------------------------------------------
// CUIMultiList::RenderIcon
// Explain:
// Date : 2005-05-18
// Author: Lee Ki-hwan
// Modified: [2013/02/06] sykim70
//------------------------------------------------------------------------------
void CUIMultiList::RenderIcon(int nColumn, int nX, int nY)
{
	int	nRowS = m_sbScrollBar.GetScrollPos();
	int	nRowE = nRowS + m_nLinePerPage;
	if (nRowE > m_vecIcon[nColumn].vecIcon.size())
		nRowE = m_vecIcon[nColumn].vecIcon.size();
	
	int nIconX = m_nIconOffset + m_vecColumnSX[nColumn];
	int nIconY = m_nIconOffset;
	
	for (int nList = nRowS; nList < nRowE; nList++)
	{
		CUIImageBox* pImageBox = m_vecIcon[nColumn].vecIcon[nList];
		pImageBox->SetPos(nIconX, nIconY);
		pImageBox->Render();
		nIconY += m_nLineHeight;
	}
}

void CUIMultiList::RenderIconPopup()
{
	for (int nCol = 0; nCol < m_vecString.size(); nCol++)
	{
		switch (m_vecConType[nCol])
		{
		case TYPE_ICON:
			{
				int	nRowS = m_sbScrollBar.GetScrollPos();
				int	nRowE = nRowS + m_nLinePerPage;
				if (nRowE > m_vecIcon[nCol].vecIcon.size())
					nRowE = m_vecIcon[nCol].vecIcon.size();
							
				for (int nList = nRowS; nList < nRowE; nList++)
				{
					CUIImageBox* pImageBox = m_vecIcon[nCol].vecIcon[nList];
					pImageBox->PopupRender();
				}
			}
			break;
		}
	}
	
}

//------------------------------------------------------------------------------
// CUIMultiList::RenderString
// Explain:
// Date : 2005-05-18
// Author: Lee Ki-hwan
// Modified: [2013/02/06] sykim70
//------------------------------------------------------------------------------
void CUIMultiList::RenderString(int nColumn, int nX, int nY)
{
	int	nRowS = m_sbScrollBar.GetScrollPos();
	int	nRowE = nRowS + m_nLinePerPage;
	if (nRowE > m_vecString[nColumn].vecString.size())
		nRowE = m_vecString[nColumn].vecString.size();

	COLOR colText;
	int nTextX = nX + m_nTextSX + m_vecColumnSX[nColumn];
	int nTextY = nY + m_nTextSY + (m_nLineHeight - m_nLineGap - _pUIFontTexMgr->GetFontHeight()) / 2;
	int nSelList = -1, nOverList = -1;
	if (m_bSelectList)
	{
		nSelList = m_nSelectList;
		nOverList = nRowS + m_nOverList;
	}

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	for (int nList = nRowS; nList < nRowE; nList++)
	{
		if (nList == nSelList)
			colText = m_colSelectList;
		else if (nList == nOverList)
			colText = m_colOverList;
		else
			colText = m_vecColor[nColumn].vecColor[nList];

		switch (m_vecAlign[nColumn])
		{
		case TEXT_LEFT:
			pDrawPort->PutTextEx(m_vecString[nColumn].vecString[nList], nTextX, nTextY, colText);
			break;
		case TEXT_CENTER:
			pDrawPort->PutTextExCX(m_vecString[nColumn].vecString[nList], nTextX, nTextY, colText);
			break;
		case TEXT_RIGHT:
			pDrawPort->PutTextExRX(m_vecString[nColumn].vecString[nList], nTextX, nTextY, colText);
			break;
		}

		nTextY += m_nLineHeight;
	}
}

//------------------------------------------------------------------------------
// CUIMultiList::AddIcon
// Explain:  
// Date : 2005-05-18
// Author: Lee Ki-hwan
// Modified: [2013/02/06] sykim70
//------------------------------------------------------------------------------
void CUIMultiList::AddIcon(int nCol, CUIImageBox::eImageType type, int index, BOOL bShowPopup, CTString popupInfo, BOOL bSelectable, COLOR Color)
{
	ASSERT(m_vecConType[nCol] == TYPE_ICON);
	ASSERT(nCol < m_nColumnCount);

	int nSize = m_nLineHeight - m_nLineGap - 2*m_nIconOffset;

	CUIImageBox* pImageBox;
	pImageBox = new CUIImageBox;
	pImageBox->Create(this, 0, 0, nSize, nSize);
	pImageBox->SetImageByType(type, index);
	pImageBox->SetRenderRegion(0, 0, nSize, nSize);
	pImageBox->SetPopupInfo(popupInfo, bShowPopup, Color);
	m_vecIcon[nCol].vecIcon.push_back(pImageBox);

	// Date : 2005-06-14(오후 3:44:55), By Lee Ki-hwan
	if (nCol == 0)
	{
		if (bSelectable && m_bAtLeastOneSelect && m_nSelectList == -1)
			m_nSelectList = m_vecSelectable.size();
		m_vecSelectable.push_back(bSelectable);
	}

	// Scroll resizing
	if (m_sbScrollBar.GetCurItemCount() < m_vecIcon[nCol].vecIcon.size())
		m_sbScrollBar.SetCurItemCount(m_vecIcon[nCol].vecIcon.size());
}

//------------------------------------------------------------------------------
// CUIMultiList::ResetAllData
// Explain:
// Date : 2005-05-18
// Author: Lee Ki-hwan
// Modified: [2013/02/06] sykim70
//------------------------------------------------------------------------------
void CUIMultiList::ResetAllData()
{
	CUIListBox::ResetAllStrings();

	for (int nCol = 0; nCol < m_vecIcon.size(); nCol++)
	{
		for (int nRow = 0; nRow < m_vecIcon[nCol].vecIcon.size(); nRow++)
			delete m_vecIcon[nCol].vecIcon[nRow];
		m_vecIcon[nCol].vecIcon.clear();
	}
}

//------------------------------------------------------------------------------
// CUIMultiList::ResetAllData
// Explain:
// Date : 2013/02/07
// Author: sykim70
//------------------------------------------------------------------------------
int CUIMultiList::GetTopItem()
{
	return m_sbScrollBar.GetScrollPos();
}