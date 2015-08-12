#ifndef __ITEMCOLLECTION_UI_H__
#define __ITEMCOLLECTION_UI_H__

class CUIImage;
class CUITabPage;
class CItemCollectionData;
class CUICheckButton;
class CUIList;
class CUIText;
class CUITree;
struct stItemCollection;

enum eOUTPUT_MSG
{
	OUTPUT_GIVE = 0,
	OUTPUT_AMEND,

};

#define DEF_CATEGORY_COUNT (11)

class CItemCollectionUI : public CUIWindow
{
public:
	CItemCollectionUI();
	~CItemCollectionUI();

	void OpenUI();
	void CloseUI();
	void initialize();

	void UpdateSummaryTab();

	// CItemCollectionData에 키로 설정된 값.
	void UpdateThemeTab(int nThemeIdx);
	void UpdateSelectItem(int nThemeIdx);

	// 검색 리스트
	bool ArrangeSearchList();
	void UpdateSearchList();

	void NextCategory();
	void PrevCategory();

	// 보상 받기.
	void SendAmend();
	void SendGiveItem();
	void UnCheck(CUICheckButton* pCheck);

	// 요약, 테마 탭
	void ChangeMainTab(int nTab);

	// 테마목록, 검색결과 탭.
	void ChangeListTab(int nTab);

	void SelectTheme(int nTheme);
	void UpdateScrollPos(int nCat, int nTheme);
	void SelectImage(int nCat, int nTheme);
	void ChangeTreeListEnterColor(int nCat, int nTheme, bool bEnter);

	bool ArrangeTopThemeList();

	void NeedItemCountSum(CUIArrayItem* pArrayItem, int nThemeIndex, int nItemIndex);
	void NeedItemCountSub(CUIArrayItem* pArrayItem, int nThemeIndex, int nItemIndex);
	void NeedItemCountMax(CUIArrayItem* pArrayItem, int nThemeIndex, int nItemIndex);

	int CalcNeedItemPercentage(CItemCollectionData* pItem);
	int CalcNeedItemTotal(CItemCollectionData* pItem);
	int CalcPossessionTotal(CItemCollectionData* pItem);

	void OutputMessage(eOUTPUT_MSG eType);
	BOOL IsEditBoxFocused()
	{
		if (m_pEditSearch == NULL)
			return FALSE;
		return m_pEditSearch->IsFocused();
	}
	void KillFocusEditBox()
	{
		if( m_pEditSearch )
			m_pEditSearch->SetFocus(FALSE);
	}
	BOOL	CloseWindowByEsc() {	CloseUI();	return TRUE;	}

	WMSG_RESULT OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg );
	WMSG_RESULT OnLButtonUp( UINT16 x, UINT16 y );
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnKeyMessage(MSG* pMsg);
private:
	enum 
	{	
		eTAB_SUMMARY = 0,
		eTAB_THEME,
		eTAB_MAX	
	};

	enum
	{
		eTAB_THEME_LIST = 0,
		eTAB_SEARCH_LIST,
	};

	enum eSEARCH_TYPE
	{
		eSEARCH_TYPE_NEED_ITEM = 0,
		eSEARCH_TYPE_RESULT_ITEM
	};

	enum 
	{
		eCHECK_ING = 0,
		eCHECK_COMPELTE,
		eCHECK_MAX
	};

	int	m_nOriX;
	int m_nOriY;
	
	// 테마탭의 진행사항 카테고리 페이지.
	int m_nCurPage;
	int m_nMaxPage;

	// 현재 선택된 테마 인덱스. map에 저장되어 있는 인덱스.
	int m_nSelectTheme;
	bool m_bDrag;
	
	CUIBase* m_pDragArea;
	CUIImage* m_pImgSummaryMask;
	CUITree* m_pTreeDesign;
	CUITree* m_pTreeThemeList;
	CUITree* m_pTreeSearchList;
	CUIEditBox* m_pEditSearch;
	CUIComboBox* m_pCombo;
	CUIList*	m_pListTopTheme;

	CUICheckButton* m_pCheck[eCHECK_MAX];

	CUITabPage* m_pTabPage[eTAB_MAX];
	CUITab*	m_pTab;
	CUITab* m_pTreeTab;
private:
	// 요약탭의 상, 하
	void UpdateSummaryCategory();
	void UpdateSummaryTopTheme();

	// 테마탭의 상, 하
	void UpdateTheme();
	void UpdateThemeNeed4ItemList();

	// 더미 데이타 클리어.
	void ClearNeed4ItemTempData();

	// 테마 트리 리스트.
	void UpdateThemeList();

	void SetThemeImageUV(CUIImage* pImg, int id, int row, int col);
	CUITree* GetTreeItem(CUITree* pParent, int nCat, int nTheme);
	bool GetTreeIndex(CItemCollectionData* pData, int& nCat, int& nTheme);

	// 등록 불가능한 아이템 체크
	bool IsCanRegistItem(int nItemIndex);

	typedef std::vector<CItemCollectionData*> vecTheme;
	typedef std::vector<vecTheme>		   vecCategory;
	vecCategory m_vecSearchList;
	vecTheme m_vecTopThemeList;

	// 아이템 기부할 경우 이 변수를 사용한다.
	int m_nTmpPossessionItemCount[6];
	int m_nTmpInvenItemCount[6];
	int m_msgID;

	// 
	int m_nOldCat;
	int m_nOldTheme;
	CUITree* m_pOldTree;
	int m_nCatName[DEF_CATEGORY_COUNT];
};	
#endif // __UI_ITEMCOLLECTION_H__