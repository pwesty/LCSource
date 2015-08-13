#ifndef	UIMULTILIST_H_
#define	UIMULTILIST_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIListBox.h>
#include <Engine/Interface/UIImageBox.h>

#define ICON_WIDTH	18
#define ICON_HEIGHT 15

enum CONTENT_TYPE
{
	TYPE_TEXT,	// 문자
	TYPE_ICON,	// 이미지
};

typedef COLOR		ALPHA;
//------------------------------------------------------------------------------
// CUIMultiList
// Explain: 이미지와 TEXT를 담을 수 있는 MultiListBox
// Date : 2005-05-18,Author: Lee Ki-hwan
// History:
//------------------------------------------------------------------------------
class CUIMultiList : public CUIListBox
{
protected:
	struct sVecIcon {
		std::vector<CUIImageBox*> vecIcon;
	};

protected:
	std::vector<CONTENT_TYPE>	m_vecConType;
	std::vector<sVecIcon>		m_vecIcon;
	CTextureData* m_ptdCommonBtnTexture;

	int m_nLineGap;
	int m_nIconOffset;

public:
	CUIMultiList();
	virtual ~CUIMultiList();

	// Create
	void Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight, int nLineHeight,
				int nSpaceX, int nSpaceY, int nColumn, BOOL bSelectList,
				int nLineGap, int nIconOffset);

	WMSG_RESULT	MouseMessage(MSG *pMsg);

	void Render();
	void RenderIcon(int nColumn, int nX, int nY);
	void RenderIconPopup();
	void RenderString(int nColumn, int nX, int nY);

	void SetColumnType(int nColumn, CONTENT_TYPE eContentType);
	void ResetAllData();

	// Get count of current item
	int GetCurItemCount(int nCol) const
	{
		ASSERT(nCol < m_nColumnCount);
		return m_vecString[nCol].vecString.size();
	} 

	// Set Icon
	void AddIcon(int nCol, CUIImageBox::eImageType type, int index,
				 BOOL bShowPopup = FALSE, CTString popupInfo = CTString(""),
				 BOOL bSelectable = TRUE, COLOR Color = 0xFFFFFFFF);

	int GetTopItem();
};

#endif	// UISCROLLBAR_H_
