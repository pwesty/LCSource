

#ifndef		UI_FACTORY_H_
#define		UI_FACTORY_H_

#include <Engine/DesignClasses/Singleton/SingletonBase.h>
//#include <Engine/Interface/UIWindow.h>
#include <Engine/tinyxml/tinyxml.h>
#include "UIBase.h"

class CUIWindow;
class CUIImage;
class CUIButton;
class CUIEditBox;
class CUICheckButton;
class CUIText;
class CUITextBox;
class CUIScrollBar;
class CUIComboBox;
class CUISlideBar;
class CUIProgressBar;
class CUIList;
class CUIListItem;
class CUITab;
class CUITabPage;
class CUIIcon;
class CUISpinButton;
class CUIImageFont;
class CUIArray;
class CUIArrayItem;
class CUISpriteAni;
class CUIImageArray;
class CUIImageSplit;
class CUITree;
class CUITreeItem;
class CUISpinControl;
class CUITextBoxEx;

class ENGINE_API UIFactory : public CSingletonBase< UIFactory >
{
public:
	UIFactory();
	~UIFactory();

	//-------------------------------------------------------------------

	bool loadXML( CUIBase* root, const char* pathname );

private:
	//-------------------------------------------------------------------
	CUIBase* BuildControlFromElement( TiXmlElement* pElement );
	void ReadElement( TiXmlElement* rootelement, CUIBase* pParent );
	UIRect getBaseInfo( TiXmlElement* pElement );
	bool getBaseValue( CUIBase* pUI, TiXmlElement* pElement );
	bool getUV( UIRectUV* uv, TiXmlElement* pElement);
	bool getUVN( int n, UIRectUV* uv, TiXmlElement* pElement );

	void addChild( CUIBase* pParent, CUIBase* pChild );

	//-------------------------------------------------------------------
	CUIBase*		createBase( TiXmlElement* pElement );
	CUIWindow*		createWindow( TiXmlElement* pElement );
	CUIImage*		createImage( TiXmlElement* pElement );
	CUIButton*		createButton( TiXmlElement* pElement );
	CUIEditBox*		createEdit( TiXmlElement* pElement );
	CUICheckButton* createCheck( TiXmlElement* pElement );
	CUIText*		createText( TiXmlElement* pElement );
	CUITextBox*		createTextBox( TiXmlElement* pElement );
	CUIScrollBar*	createScroll( TiXmlElement* pElement );
	CUIComboBox*	createComboBox( TiXmlElement* pElement );
	CUISlideBar*	createSlideBar( TiXmlElement* pElement );
	CUIProgressBar* createProgressBar( TiXmlElement* pElement );
	CUIList*		createList( TiXmlElement* pElement );
	CUIListItem*	createListItem( TiXmlElement* pElement );
	CUITab*			createTab( TiXmlElement* pElement );
	CUITabPage*		createTabPage( TiXmlElement* pElement );
	CUIIcon*		createIcon( TiXmlElement* pElement );
	CUISpinButton*	createSpinButton( TiXmlElement* pElement );
	CUIImageFont*	createImageFont( TiXmlElement* pElement );
	CUIArray*		createArray( TiXmlElement* pElement );
	CUIArrayItem*	createArrayItem( TiXmlElement* pElement );
	CUISpriteAni*	createSpriteAni( TiXmlElement* pElement );
	CUIImageArray*	createImageArray( TiXmlElement* pElement );
	CUIImageSplit*	createImageSprit( TiXmlElement* pElement );
	CUITree*		createTree( TiXmlElement* pElement );
	CUITreeItem*	createTreeItem( TiXmlElement* pElement );
	CUISpinControl*	createSpinControl( TiXmlElement* pElement );
	CUITextBoxEx*	createTextBoxEx( TiXmlElement* pElement );
};


#endif		// UI_FACTORY_H_