#ifndef		_UI_LIST_H_
#define		_UI_LIST_H_

class CUIBase;
class CUIListItem;

class ENGINE_API CUIList : public CUIBase
{
public:
	CUIList();
	~CUIList();

	enum eEVENT_IMAGE
	{
		eTYPE_SELECT = 0,
		eTYPE_OVER,
		eTYPE_MAX
	};
	//------------------------------------------------------

	CUIBase* Clone();

	void AddListItem(CUIBase* pItem);
	void RemoveListItem(int idx);
	bool DeleteAllListItem();

	void deleteChildList();
	void deleteListItem(int nPos);

	// 인덱스로 리스트 아이템 얻기
	CUIBase* GetListItem(int idx);

	inline void SetListItemTemplate(CUIListItem* pItem)	{ m_pListItemTemplate = pItem; }
	inline CUIListItem* GetListItemTemplate()			{ return m_pListItemTemplate; }

	//------------------------------------------------------

	void SetItemStart(int x, int y);
	void GetItemStart(int& x, int& y) { x = m_nItemStartX; y = m_nItemStartY; }

	void SetItemShowNum(int nShow)	{ m_nNumShow = nShow; }
	int  GetItemShowNum()		 { return m_nNumShow; }

	void SetGap(int gap)		 { m_nGap = gap; }
	int GetGap()				 { return m_nGap; }

	void UpdateList();
	void UpdateScroll(int nItemCount);
	void UpdateHeight(int nHeight);

	void SetAlignBottom()	{ m_bAlignTop = false; }
	CUIScrollBar* GetScroll()	{ return m_pScroll;	}
	
	//------------------------------------------------------
	// 이전에 선택된 아이템이 있을 경우 호출 되는 커맨드
	void SetCommandUn(Command* pCmd)	{ SAFE_DELETE(m_pCmdUnSelect); m_pCmdUnSelect = pCmd; }

	//------------------------------------------------------

	void OnPostRender(CDrawPort* pDraw);

	//------------------------------------------------------

	void setScroll(CUIScrollBar* pScroll);

	// xml 에서 미리 리스트를 등록시킬때만 사용
	void SetItemCount(int cnt)		{ m_nItemCount = cnt; }
	int	GetItemCount()				{ return m_nItemCount;}

	void SetPreCreate(bool bPre)	{ m_bPreCreate = bPre; }
	bool GetPreCreate()				{ return m_bPreCreate; }

	void addEventImage( UIRect rect, UIRectUV uv, int type );
	int HitTest(UINT16 x, UINT16 y);

	void setCurSel(int idx);
	int	 getCurSel()			{ return m_nCurSel; }

	void setMouseOverItem(int idx);
	int	 getMouseOverItem()			{ return m_nMouseOverItem;	}

	int getListItemCount()			{ return m_vecListArray.size(); }

	int	GetItemTotalHeight()		{ return m_nItemTotalHeight;	}

	void SetRectSurface(CUIRectSurface* pRS) { m_pimgEvent = pRS; }
	CUIRectSurface* GetRectSurface()		 { return m_pimgEvent; }

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT	OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDBLClick(UINT16 x, UINT16 y);

#ifdef UI_TOOL
	void		GetEventImg(UIRect& rc, UIRectUV& uv, int type)	{ rc = m_rcEventImg[type]; uv = m_uvEventImg[type];	}

private:
	UIRect		m_rcEventImg[eTYPE_MAX];
	UIRectUV	m_uvEventImg[eTYPE_MAX];
#endif // UI_TOOL

private:
	void UpdateListTop();
	void UpdateListBottom();

protected:

	typedef std::vector< CUIBase*>::iterator	vec_listitem_iter;
	std::vector< CUIBase*> 		m_vecListArray;
	
	bool	m_bUseSelect;
	bool	m_bPreCreate;
	bool	m_bAlignTop;

	int		m_nNumItems;		// 아이템 수
	int		m_nGap;				// 아이템 간격
	int		m_nNumShow;			// 리스트에 보여질 아이템 수

	int		m_nItemStartIndex;	// 보여질 아이템의 시작 인덱스
	int		m_nItemCount;		// 전체 아이템의 수 (툴이나, 미리 생성시에 필요)

	int		m_nItemStartX;		// 
	int		m_nItemStartY;		// 

	bool	m_bSelected;
	bool	m_bMouseOver;
	int		m_nCurSel;	// 선택된 아이템 인덱스
	int		m_nMouseOverItem;	// 마우스 오버된 아이템 인덱스
	POINT	m_ptSelOffset;
	POINT	m_ptMouseOverOffset;

	int		m_nCurStart;		// 스크롤이 있을 경우 시작 인덱스
	int		m_nItemTotalHeight;	// 리스트 아이템의 총 높이.

	CUIListItem*	m_pListItemTemplate;	// 리스트 아이템 용으로 미리 만들어진 아이템 템플릿. Clone하여 사용한다.
	
	CUIScrollBar*	m_pScroll;

	CUIRectSurface*	m_pimgEvent;	

	Command*	m_pCmdUnSelect;
};


#endif		// _UI_LIST_H_