
#pragma once

class CUIBase;
class CUIImage;
class CUIButton;
class CUIText;
class CUIEditBox;
class CUICheckButton;
class CUIImageBox;
class CUIComboBox;
class CUIProgressBar;
class CUISlideBar;
class CUIList;
class CUIScrollBar;
class CUISpinButton;
class CUIIcon;
class CUIArray;
class CUIArrayItem;
class CUITextBox;
class CUIImageFont;
class CUISpriteAni;
class CUIImageArray;
class CUIImageSplit;
class CUISpinControl;
class CUITree;
class CUITextBoxEx;

class COleVariant;
class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CMFCPropertyGridPropertyEx : public CMFCPropertyGridProperty
{
public :
	CMFCPropertyGridPropertyEx(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);

	// Simple property
	CMFCPropertyGridPropertyEx(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);

	BOOL OnActivateByTab();
	CMFCPropertyGridProperty* GetNextChildProp();
	CMFCPropertyGridProperty* GetNextProp(CMFCPropertyGridPropertyEx* pParent = NULL);

	CMFCPropertyGridProperty* GetPrevChildProp();
	CMFCPropertyGridProperty* GetPrevProp(CMFCPropertyGridPropertyEx* pParent = NULL);

	CMFCPropertyGridProperty* GetTopParent();

	BOOL					  GetInPlaceEdit()	{ return m_bInPlaceEdit;	}
	CWnd*					  GetInPlaceEditPtr()	{ return m_pWndInPlace;		}
};

class CMFCPropertyGridCtrlEx : public CMFCPropertyGridCtrl
{
public:
	CMFCPropertyGridCtrlEx();
	CMFCPropertyGridProperty* GetTabItem();
	CMFCPropertyGridProperty* GetNextItem();
	CMFCPropertyGridProperty* GetPrevItem();
	BOOL					  PreTranslateMessage(MSG* pMsg);
	bool					  m_bShift;
};

#define		DEF_PROPERTY_BASE	0x0001
#define		DEF_PROPERTY_UV		0x0002
#define		DEF_PROPERTY_T1		0x0004
#define		DEF_PROPERTY_T2		0x0008
#define		DEF_PROPERTY_T3		0x0010

enum ePROP_TYPE
{
	ePROP_TYPE_NONE = -1,
	ePROP_TYPE_ID = 0,
	ePROP_TYPE_PID,
	ePROP_TYPE_HIDE,
	ePROP_TYPE_X,
	ePROP_TYPE_Y,
	ePROP_TYPE_W,
	ePROP_TYPE_H,
	ePROP_TYPE_ALIGN_H,
	ePROP_TYPE_ALIGN_V,
	ePROP_TYPE_TOOLTIP,
	ePROP_TYPE_TOOLTIP_WIDTH,

	ePROP_IMG_TYPE_L,
	ePROP_IMG_TYPE_T,
	ePROP_IMG_TYPE_R,
	ePROP_IMG_TYPE_B,
	ePROP_IMG_TYPE_TEX,

	ePROP_BTN_TEX,
	ePROP_BTN_STR_IDX,
	ePROP_BTN_STR,
	ePROP_BTN_NEWTYPE,
	ePROP_BTN_RS_UNIT,
	ePROP_BTN_EDGE,
	ePROP_BTN_TEXT_COLOR,

	ePROP_BTN_IDLE_L,
	ePROP_BTN_IDLE_T,
	ePROP_BTN_IDLE_R,
	ePROP_BTN_IDLE_B,

	ePROP_BTN_ON_L,
	ePROP_BTN_ON_T,
	ePROP_BTN_ON_R,
	ePROP_BTN_ON_B,

	ePROP_BTN_CLICK_L,
	ePROP_BTN_CLICK_T,
	ePROP_BTN_CLICK_R,
	ePROP_BTN_CLICK_B,

	ePROP_BTN_DISABLE_L,
	ePROP_BTN_DISABLE_T,
	ePROP_BTN_DISABLE_R,
	ePROP_BTN_DISABLE_B,

	ePROP_TEXT_TEX,
	ePROP_TEXT_STR_IDX,
	ePROP_TEXT_STR,
	ePROP_TEXT_STR_ELLIPSIS,
	ePROP_TEXT_COLOR,
	ePROP_TEXT_SHADOW,
	ePROP_TEXT_EDGE,
	ePROP_TEXT_SHADOW_COLOR,
	ePROP_TEXT_ALIGN,
	//ePROP_TEXT_DEPTH,

	ePROP_TEXT_BACK_RECT_X,
	ePROP_TEXT_BACK_RECT_Y,
	ePROP_TEXT_BACK_RECT_W,
	ePROP_TEXT_BACK_RECT_H,

	ePROP_TEXT_BACK_UV_L,
	ePROP_TEXT_BACK_UV_T,
	ePROP_TEXT_BACK_UV_R,
	ePROP_TEXT_BACK_UV_B,

	ePROP_EDIT_TEX,
	ePROP_EDIT_MAX_CHAR,
	ePROP_EDIT_PASSWORD_SET,
	ePROP_EDIT_CURSOR_MOVER,
	ePROP_EDIT_READWINDOW_L,
	ePROP_EDIT_READWINDOW_T,
	ePROP_EDIT_READWINDOW_R,
	ePROP_EDIT_READWINDOW_B,

	ePROP_EDIT_CANDIDATE_UV_L,
	ePROP_EDIT_CANDIDATE_UV_T,
	ePROP_EDIT_CANDIDATE_UV_R,
	ePROP_EDIT_CANDIDATE_UV_B,

	ePROP_EDIT_BACK_UV_L,
	ePROP_EDIT_BACK_UV_T,
	ePROP_EDIT_BACK_UV_R,
	ePROP_EDIT_BACK_UV_B,

	ePROP_EDIT_BACK_RECT_X,
	ePROP_EDIT_BACK_RECT_Y,
	ePROP_EDIT_BACK_RECT_W,
	ePROP_EDIT_BACK_RECT_H,

	ePROP_CHECK_TEX,
	ePROP_CHECK_STR_IDX,
	ePROP_CHECK_STR,
	ePROP_CHECK_POS_LEFT,
	ePROP_CHECK_TEXT_X,
	ePROP_CHECK_REGION,
	ePROP_CHECK_COLOR_ON,
	ePROP_CHECK_COLOR_OFF,
	ePROP_CHECK_ALIGN,
	ePROP_CHECK_EDGE,
	ePROP_CHECK_TEXT_AREA,

	ePROP_CHECK_NONE_L,
	ePROP_CHECK_NONE_T,
	ePROP_CHECK_NONE_R,
	ePROP_CHECK_NONE_B,

	ePROP_CHECK_CHECK_L,
	ePROP_CHECK_CHECK_T,
	ePROP_CHECK_CHECK_R,
	ePROP_CHECK_CHECK_B,

	ePROP_COMBO_TEX,
	ePROP_COMBO_MAXLINE,
	ePROP_COMBO_SHOW_LINE,
	ePROP_COMBO_LINE_HEIGHT,
	ePROP_COMBO_SPACE_X,
	ePROP_COMBO_SPACE_Y,

	ePROP_COMBO_BACK_X,
	ePROP_COMBO_BACK_Y,
	ePROP_COMBO_BACK_W,
	ePROP_COMBO_BACK_H,

	ePROP_COMBO_BACK_L,
	ePROP_COMBO_BACK_T,
	ePROP_COMBO_BACK_R,
	ePROP_COMBO_BACK_B,

	ePROP_COMBO_BTN_X,
	ePROP_COMBO_BTN_Y,
	ePROP_COMBO_BTN_W,
	ePROP_COMBO_BTN_H,

	ePROP_COMBO_BTN0_L,
	ePROP_COMBO_BTN0_T,
	ePROP_COMBO_BTN0_R,
	ePROP_COMBO_BTN0_B,

	ePROP_COMBO_BTN1_L,
	ePROP_COMBO_BTN1_T,
	ePROP_COMBO_BTN1_R,
	ePROP_COMBO_BTN1_B,

	ePROP_COMBO_DROP_AREA_X,
	ePROP_COMBO_DROP_AREA_Y,
	ePROP_COMBO_DROP_AREA_W,
	ePROP_COMBO_DROP_AREA_H,

	ePROP_COMBO_DROP_AREA_L,
	ePROP_COMBO_DROP_AREA_T,
	ePROP_COMBO_DROP_AREA_R,
	ePROP_COMBO_DROP_AREA_B,

	ePROP_SLIDEBAR_TEX,
	ePROP_SLIDEBAR_BAR_W,
	ePROP_SLIDEBAR_BAR_H,
	ePROP_SLIDEBAR_CURPOS,
	ePROP_SLIDEBAR_RANGE,
	ePROP_SLIDEBAR_MIN,
	ePROP_SLIDEBAR_MAX,

	ePROP_SLIDEBAR_BACK_L,
	ePROP_SLIDEBAR_BACK_T,
	ePROP_SLIDEBAR_BACK_R,
	ePROP_SLIDEBAR_BACK_B,

	ePROP_SLIDEBAR_BAR_L,
	ePROP_SLIDEBAR_BAR_T,
	ePROP_SLIDEBAR_BAR_R,
	ePROP_SLIDEBAR_BAR_B,

	ePROP_LIST_TEX,
	ePROP_LIST_START_X,
	ePROP_LIST_START_Y,
	ePROP_LIST_GAP,
	ePROP_LIST_SHOW_COUNT,
	ePROP_LIST_ITEM_COUNT,
	ePROP_LIST_PRE_CREATE,

	ePROP_LIST_SELECT_RECT_X,
	ePROP_LIST_SELECT_RECT_Y,
	ePROP_LIST_SELECT_RECT_W,
	ePROP_LIST_SELECT_RECT_H,

	ePROP_LIST_SELECT_UV_L,
	ePROP_LIST_SELECT_UV_T,
	ePROP_LIST_SELECT_UV_R,
	ePROP_LIST_SELECT_UV_B,

	ePROP_LIST_OVER_RECT_X,
	ePROP_LIST_OVER_RECT_Y,
	ePROP_LIST_OVER_RECT_W,
	ePROP_LIST_OVER_RECT_H,

	ePROP_LIST_OVER_UV_L,
	ePROP_LIST_OVER_UV_T,
	ePROP_LIST_OVER_UV_R,
	ePROP_LIST_OVER_UV_B,

	ePROP_PROGRESS_TEX,
	ePROP_PROGRESS_LOWER,
	ePROP_PROGRESS_UPPER,
	ePROP_PROGRESS_STEP,
	ePROP_PROGRESS_OFFSETPOS,
	ePROP_PROGRESS_AUTO_COMPLETE,

	ePROP_PROGRESS_UV_BACK_L,
	ePROP_PROGRESS_UV_BACK_T,
	ePROP_PROGRESS_UV_BACK_R,
	ePROP_PROGRESS_UV_BACK_B,

	ePROP_PROGRESS_BAR_SX,
	ePROP_PROGRESS_BAR_SY,
	ePROP_PROGRESS_UV_BAR_L,
	ePROP_PROGRESS_UV_BAR_T,
	ePROP_PROGRESS_UV_BAR_R,
	ePROP_PROGRESS_UV_BAR_B,

 	ePROP_PROGRESS_IDLE_STR_IDX,
 	ePROP_PROGRESS_IDLE_STR,
 	ePROP_PROGRESS_IDLE_SHOW_PERCENT,
 	ePROP_PROGRESS_IDLE_COLOR,
 
 	ePROP_PROGRESS_DOING_STR_IDX,
 	ePROP_PROGRESS_DOING_STR,
 	ePROP_PROGRESS_DOING_SHOW_PERCENT,
 	ePROP_PROGRESS_DOING_COLOR,
 
 	ePROP_PROGRESS_COMPLETE_STR_IDX,
 	ePROP_PROGRESS_COMPLETE_STR,
 	ePROP_PROGRESS_COMPLETE_SHOW_PERCENT,
 	ePROP_PROGRESS_COMPLETE_COLOR,

	ePROP_SB_TEX,
	ePROP_SB_VERTICAL,
	ePROP_SB_RIGHT,
	ePROP_SB_SCROLL_POS,
	ePROP_SB_MOVE_UNIT,
	ePROP_SB_CUR_ITEM_CNT,
	ePROP_SB_ITEM_PER_PAGE,

	ePROP_SB_BACK_RECT_X,
	ePROP_SB_BACK_RECT_Y,
	ePROP_SB_BACK_RECT_W,
	ePROP_SB_BACK_RECT_H,

	ePROP_SB_BACK_UV_L,
	ePROP_SB_BACK_UV_T,
	ePROP_SB_BACK_UV_R,
	ePROP_SB_BACK_UV_B,

	ePROP_SB_BUTTON_1_RECT_X,
	ePROP_SB_BUTTON_1_RECT_Y,
	ePROP_SB_BUTTON_1_RECT_W,
	ePROP_SB_BUTTON_1_RECT_H,

	ePROP_SB_BUTTON_1_UP_UV_L,
	ePROP_SB_BUTTON_1_UP_UV_T,
	ePROP_SB_BUTTON_1_UP_UV_R,
	ePROP_SB_BUTTON_1_UP_UV_B,

	ePROP_SB_BUTTON_1_DOWN_UV_L,
	ePROP_SB_BUTTON_1_DOWN_UV_T,
	ePROP_SB_BUTTON_1_DOWN_UV_R,
	ePROP_SB_BUTTON_1_DOWN_UV_B,

	ePROP_SB_BUTTON_2_RECT_X,
	ePROP_SB_BUTTON_2_RECT_Y,
	ePROP_SB_BUTTON_2_RECT_W,
	ePROP_SB_BUTTON_2_RECT_H,
					
	ePROP_SB_BUTTON_2_UP_UV_L,
	ePROP_SB_BUTTON_2_UP_UV_T,
	ePROP_SB_BUTTON_2_UP_UV_R,
	ePROP_SB_BUTTON_2_UP_UV_B,

	ePROP_SB_BUTTON_2_DOWN_UV_L,
	ePROP_SB_BUTTON_2_DOWN_UV_T,
	ePROP_SB_BUTTON_2_DOWN_UV_R,
	ePROP_SB_BUTTON_2_DOWN_UV_B,

	ePROP_SB_THUMB_UNIT,
	ePROP_SB_THUMB_RECT_X,
	ePROP_SB_THUMB_RECT_Y,
	ePROP_SB_THUMB_RECT_W,
	ePROP_SB_THUMB_RECT_H,

	ePROP_SB_THUMB_UV_L,
	ePROP_SB_THUMB_UV_T,
	ePROP_SB_THUMB_UV_R,
	ePROP_SB_THUMB_UV_B,

	ePROP_SPIN_TEX,
	ePROP_SPIN_TITLE_STR,
	ePROP_SPIN_AUTO_INFO,
	ePROP_SPIN_BTN_W,
	ePROP_SPIN_BTN_H,
	ePROP_SPIN_BTN_GAP,
	ePROP_SPIN_DATA_BACK_WIDTH,

	ePROP_SPIN_BACK_UV_L,
	ePROP_SPIN_BACK_UV_T,
	ePROP_SPIN_BACK_UV_R,
	ePROP_SPIN_BACK_UV_B,

	ePROP_SPIN_WHEEL_L,
	ePROP_SPIN_WHEEL_T,
	ePROP_SPIN_WHEEL_R,
	ePROP_SPIN_WHEEL_B,

	ePROP_ICON_TYPE,
	ePROP_ICON_TEX_ID,
	ePROP_ICON_ROW,
	ePROP_ICON_COL,

	ePROP_ARRAY_ROW,
	ePROP_ARRAY_COL,
	ePROP_ARRAY_SX,
	ePROP_ARRAY_SY,
	ePROP_ARRAY_GAPX,
	ePROP_ARRAY_GAPY,
	ePROP_ARRAY_IMG,
	ePROP_ARRAY_SHOW_ROW,

	ePROP_ARRAY_IMG_SELECT_RECT_X,
	ePROP_ARRAY_IMG_SELECT_RECT_Y,
	ePROP_ARRAY_IMG_SELECT_RECT_W,
	ePROP_ARRAY_IMG_SELECT_RECT_H,

	ePROP_ARRAY_IMG_SELECT_UV_L,
	ePROP_ARRAY_IMG_SELECT_UV_T,
	ePROP_ARRAY_IMG_SELECT_UV_R,
	ePROP_ARRAY_IMG_SELECT_UV_B,

	ePROP_ARRAY_IMG_OVER_RECT_X,
	ePROP_ARRAY_IMG_OVER_RECT_Y,
	ePROP_ARRAY_IMG_OVER_RECT_W,
	ePROP_ARRAY_IMG_OVER_RECT_H,

	ePROP_ARRAY_IMG_OVER_UV_L,
	ePROP_ARRAY_IMG_OVER_UV_T,
	ePROP_ARRAY_IMG_OVER_UV_R,
	ePROP_ARRAY_IMG_OVER_UV_B,

	ePROP_TB_TEX,
	ePROP_TB_STR_IDX,
	ePROP_TB_STR,
	ePROP_TB_STR_COLOR,
	ePROP_TB_START_X,
	ePROP_TB_START_Y,
	ePROP_TB_GAP_Y,
	ePROP_TB_ALIGN,
	ePROP_TB_SPLIT,

	ePROP_TB_BACK_RECT_X,
	ePROP_TB_BACK_RECT_Y,
	ePROP_TB_BACK_RECT_W,
	ePROP_TB_BACK_RECT_H,

	ePROP_TB_BACK_UV_L,
	ePROP_TB_BACK_UV_T,
	ePROP_TB_BACK_UV_R,
	ePROP_TB_BACK_UV_B,

	ePROP_IMGFONT_SRC_TEX,
	ePROP_IMGFONT_FSIZE_X,
	ePROP_IMGFONT_FSIZE_Y,
	ePROP_IMGFONT_FGAP,
	ePROP_IMGFONT_OGAP,
	ePROP_IMGFONT_STR,
	ePROP_IMGFONT_OSTR,

	ePROP_IMGFONT_SRC_UV_L,
	ePROP_IMGFONT_SRC_UV_T,
	ePROP_IMGFONT_SRC_UV_R,
	ePROP_IMGFONT_SRC_UV_B,

	ePROP_IMGSPLIT_TYPE_L,
	ePROP_IMGSPLIT_TYPE_T,
	ePROP_IMGSPLIT_TYPE_R,
	ePROP_IMGSPLIT_TYPE_B,
	ePROP_IMGSPLIT_TYPE_TEX,
	ePROP_IMGSPLIT_SPLIT_MODE,
	ePROP_IMGSPLIT_SPLIT_UNIT,

	ePROP_SP_TEX,
	ePROP_SP_MIN,
	ePROP_SP_MAX,
	ePROP_SP_CURSOR_MOVER,
	ePROP_SP_CURSOR_SX,
	ePROP_SP_BACK_UNIT,

	ePROP_SP_BACK_UV_L,
	ePROP_SP_BACK_UV_T,
	ePROP_SP_BACK_UV_R,
	ePROP_SP_BACK_UV_B,

	ePROP_SP_BACK_RECT_X,
	ePROP_SP_BACK_RECT_Y,
	ePROP_SP_BACK_RECT_W,
	ePROP_SP_BACK_RECT_H,

	ePROP_TB_EX_STR,
	ePROP_TB_EX_COL,
	ePROP_TB_EX_SPLIT_MODE,
	ePROP_TB_EX_SX,
	ePROP_TB_EX_SY,
	ePROP_TB_EX_GAP_Y

};

enum ePROP_IMAGE_ARRAY_TYPE
{
	ePROP_IMAGE_ARRAY_TEX = 10000,
	ePROP_IMAGE_ARRAY_CUR_IDX,

	ePROP_IMAGE_ARRAY_RECT_X = 11000,
	ePROP_IMAGE_ARRAY_RECT_Y,
	ePROP_IMAGE_ARRAY_RECT_W,
	ePROP_IMAGE_ARRAY_RECT_H,

	ePROP_IMAGE_ARRAY_UV_L = 12000,
	ePROP_IMAGE_ARRAY_UV_T,
	ePROP_IMAGE_ARRAY_UV_R,
	ePROP_IMAGE_ARRAY_UV_B,

	ePROP_IMAGE_ARRAY_DESC = 13000,
	
	ePROP_IMAGE_ARRAY_ADD = 14000,
};

enum ePROP_SPRITE_ANI
{
	ePROP_SPRITE_TEX = 20000,
	ePROP_SPRITE_CUR_IDX,

	ePROP_SPRITE_UV_L,
	ePROP_SPRITE_UV_T,
	ePROP_SPRITE_UV_R,
	ePROP_SPRITE_UV_B,

	ePROP_SPRITE_ADD = 21000,
	ePROP_SPRITE_LOOP,
	
};

class CPropertiesWnd : public CDockablePane
{
// 생성입니다.
public:
	CPropertiesWnd();

	void AdjustLayout();

	void parseUI( CUIBase* pUI );
	void ResetSelectUI()	
	{
		m_pSelectedUI = NULL; 
		m_wndPropListEx.RemoveAll();
	}
	void UpdatePos(int x, int y);

// 특성입니다.
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropListEx.SetVSDotNetLook(bSet);
		m_wndPropListEx.SetGroupNameFullWidth(bSet);
	}
private:
	void ShowPropBase( CUIBase* pBase );
	void ShowPropImage( CUIImage* pUI );
	void ShowPropButton( CUIButton* pUI );
	void ShowPropText( CUIText* pUI );
	void ShowPropEdit( CUIEditBox* pUI );
	void ShowPropCheckButton( CUICheckButton* pUI );
	void ShowPropComboBox( CUIComboBox* pUI );
	void ShowPropSlideBar( CUISlideBar* pUI );
	void ShowPropProgressBar( CUIProgressBar* pUI );
	void ShowPropList( CUIList* pUI );
	void ShowPropScrollBar( CUIScrollBar* pUI );
	void ShowPropSpinButton( CUISpinButton* pUI );
	void ShowPropIcon( CUIIcon* pUI );
	void ShowPropArray( CUIArray* pUI );
	void ShowPropTextBox( CUITextBox* pUI );
	void ShowPropImageFont( CUIImageFont* pUI );
	void ShowPropSpriteAni( CUISpriteAni* pUI );
	void ShowPropImageArray( CUIImageArray* pUI );
	void ShowPropImageSplit( CUIImageSplit* pUI );
	void ShowPropSpinControl( CUISpinControl* pUI );
	void ShowPropTree( CUITree* pUI );
	void ShowPropTextBoxEx( CUITextBoxEx* pUI );

	void SetUV(int nMsgID_L, int nMsgID_T, int nMsgID_R, int nMsgID_B
		, UIRectUV uv, CMFCPropertyGridProperty* pGroup, std::string strName = " UV ");
	void SetRect(int nMsgID_L, int nMsgID_T, int nMsgID_R, int nMsgID_B
		, UIRect rc, CMFCPropertyGridProperty* pGroup);

	void SetTextString(ePROP_TYPE eMsg_type, CMFCPropertyGridProperty* pGroup, std::string str, std::string name, std::string desc = "");

	void	 SetImage( int nMsgID, std::string strImage, CMFCPropertyGridProperty* pGroup );
	bool	 SetBtnUV( ePROP_TYPE eType, CUIButton* pBtn, std::string& strVal );
	void	 ExtractionUV( ePROP_TYPE eType, ePROP_TYPE eStart, UIRectUV& uv, std::string& val );
	void	 ExtractionRect( ePROP_TYPE eType, ePROP_TYPE eStart, UIRect& rc, int nVal );
	void	 ShowHide( BOOL bHide );
	std::string ExtracFileName(char* FilePath);
	COLOR	 ConvertToColor(COLORREF col);
	void Refresh();

private:
	void ChangeImageControlProp(CMFCPropertyGridProperty* p);
	void ChangeButtonControlProp(LPARAM lparam);
	void ChangeTextControlProp(LPARAM lparam);
	void ChangeEditControlProp(CMFCPropertyGridProperty* p);
	void ChangeCheckControlProp(LPARAM lparam);
	void ChangeComboBoxControlProp(CMFCPropertyGridProperty* p);
	void ChangeSlideBarControlProp(CMFCPropertyGridProperty* p);
	void ChangeProgressBarControlProp(LPARAM lparam);
	void ChangeListControlProp(CMFCPropertyGridProperty* p);
	void ChangeScrollBarControlProp(CMFCPropertyGridProperty* p);
	void ChangeSpinButtonControlProp(CMFCPropertyGridProperty* p);
	void ChangeIconControlProp(CMFCPropertyGridProperty* p);
	void ChangeArrayControlProp(CMFCPropertyGridProperty* p);
	void ChangeTextBoxControlProp(LPARAM lparam);
	void ChangeImageFontControlProp(CMFCPropertyGridProperty* p);
	void ChangeSpriteAniControlProp(CMFCPropertyGridProperty* p);
	void ChangeImageArrayControlProp(CMFCPropertyGridProperty* p);
	void ChangeImageSplitControlProp(CMFCPropertyGridProperty* p);
	void ChangeSpinControlProp(CMFCPropertyGridProperty* p);
	void ChangeTreeControlProp(CMFCPropertyGridProperty* p);
	void ChangeTextBoxExControlProp(LPARAM lparam);

	int m_nImageArrayOldCount;
	int m_nSpriteAniOldCount;

	void SetPropImageArray(CUIImageArray* pUI, CMFCPropertyGridProperty* pProp);
protected:
	CUIBase* m_pSelectedUI;
	CFont m_fntPropList;
	CComboBox m_wndObjectCombo;
	CPropertiesToolBar m_wndToolBar;
	CMFCPropertyGridCtrl m_wndPropList;
	CMFCPropertyGridProperty* m_pTmpProp;

// 구현입니다.
public:
	virtual ~CPropertiesWnd();

	CMFCPropertyGridCtrlEx m_wndPropListEx;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnProperties1();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnProperties2();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnPropertyChanged(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();

private:
	
};

