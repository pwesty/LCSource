
#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "UIFactory.h"

#include <Engine/Templates/Stock_CTextureData.h>
#include <Engine/Base/Stream.h>

UIFactory::UIFactory()
{

}

UIFactory::~UIFactory()
{

}

bool UIFactory::loadXML( CUIBase* root, const char* pathname )
{
	if( root == NULL )
		return false;

	//const char* prefix = "Data/Interface/xml/";
	std::string strPath;// = prefix;
	strPath += pathname;

	TiXmlDocument	doc;
	std::string		strTmp;
	std::string		strComment;
	
	if( doc.LoadFile( strPath.c_str() ) == false )
		return false;

	CUIWindow*	pCurUI = NULL;
	
	//TiXmlElement* element;
	TiXmlNode* node;
	node = doc.FirstChild();
	
	while( node )
	{
		// element 만 처리한다.
		if( node->Type() == TiXmlNode::TINYXML_ELEMENT )
		{
			strTmp = node->Value();

			if( strcmpi(strTmp.c_str(), "window") == 0 )
			{
				TiXmlElement* rootelement = node->ToElement();

				bool bRet = getBaseValue(root, rootelement);

				if( bRet )
				{
					ReadElement(rootelement, root);
				}
			}
		}
		else if( node->Type() == TiXmlNode::TINYXML_COMMENT )
		{
			strComment = node->Value();
		}
		
		node = node->NextSibling();		
	}
	return true;
}

//------------------------------------------------------------------------

void UIFactory::ReadElement( TiXmlElement* rootelement, CUIBase* pParent )
{
	TiXmlElement* element = rootelement->FirstChildElement();

	int		tab_idx = 0;

	for( ; element != NULL; element = element->NextSiblingElement() )
	{
		CUIBase* pUI = BuildControlFromElement( element );

		if( pUI )
		{
			ReadElement(element, pUI);
			pUI->initialize();

			eUI_CONTROL eType = pParent->getType();
			if (eType != eUI_CONTROL_LIST && 
				eType != eUI_CONTROL_ARRAY && 
				eType != eUI_CONTROL_TREE)
			{
				addChild(pParent, pUI);
			}
			else
			{
				if (pUI->getType() == eUI_CONTROL_LIST_ITEM)
				{
					// ListItem 은 Child 로 넣지말자
					CUIList* pList = static_cast<CUIList*>(pParent);
					pList->SetListItemTemplate( static_cast<CUIListItem*>(pUI) );

					bool bPre = pList->GetPreCreate();

					if (bPre == true)
					{
						int		i;
						for (i = 0; i < pList->GetItemCount(); ++i)
						{
							pList->AddListItem( static_cast<CUIListItem*>(pUI->Clone()) );
						}

						pList->UpdateList();
					}					

				}
				else if( pUI->getType() == eUI_CONTROL_ARRAY_ITEM )
				{
					// ArrayItem은 Child로 넣지 말자....
					CUIArray* pArray = static_cast<CUIArray*>(pParent);
					pArray->SetArrayTemplate( static_cast<CUIArrayItem*>(pUI) );

					int i, row, col;
					pArray->GetRowCol(row, col);
					for ( i = 0; i < row * col ; i++ )
					{
						pArray->AddArrayItem( static_cast<CUIArrayItem*>(pUI->Clone()) );
					}
				}
				else if(pUI->getType() == eUI_CONTROL_SCROLL)
				{
					if( pParent->getType() == eUI_CONTROL_LIST )
					{
						addChild(pParent, pUI);
						static_cast<CUIList*>(pParent)->setScroll( static_cast<CUIScrollBar*>(pUI) );
					}
					else if( pParent->getType() == eUI_CONTROL_ARRAY )
					{
						addChild(pParent, pUI);
						static_cast<CUIArray*>(pParent)->SetScroll( static_cast<CUIScrollBar*>(pUI) );
					}
				}
				else if (eType == eUI_CONTROL_TREE && pUI->getType() == eUI_CONTROL_CHECKBOX)
				{
					CUITree* pTree = static_cast<CUITree*>(pParent);
					pTree->setCheck((CUICheckButton*)pUI);
				}
				else
				{
					addChild(pParent, pUI);
				}
			}

			if (eType == eUI_CONTROL_TAB)
			{
				CUITab* pTab = static_cast<CUITab*>(pParent);

				if (pUI->getType() == eUI_CONTROL_TABPAGE)
					pTab->addTabPage(static_cast<CUITabPage*>(pUI));
				else if (pUI->getType() == eUI_CONTROL_CHECKBOX)
					pTab->addTabButton(static_cast<CUICheckButton*>(pUI));
			}
			else if (eType == eUI_CONTROL_COMBOBOX)
			{
				if (pUI->getType() == eUI_CONTROL_SCROLL)
					static_cast<CUIComboBox*>(pParent)->setScroll( static_cast<CUIScrollBar*>(pUI) );
			}
			else if (eType == eUI_CONTROL_TEXTBOX)
			{
				if (pUI->getType() == eUI_CONTROL_SCROLL)
					static_cast<CUITextBox*>(pParent)->SetScroll( static_cast<CUIScrollBar*>(pUI) );
			}
			else 
			{
				tab_idx = 0;
			}
			pUI->postInit();
		}
	}
}

void UIFactory::addChild( CUIBase* pParent, CUIBase* pChild )
{
	std::string strPID;
	strPID = pChild->getPID();

	if (strPID.empty() == false)
	{
		CUIBase* pBase = pParent->findUI(strPID.c_str());
		if (pBase)
		{
			pBase->addChild( pChild );
			return;
		}
	}

	pParent->addChild( pChild );
}

CUIBase* UIFactory::BuildControlFromElement( TiXmlElement* pElement )
{
	std::string strTmp = pElement->Value();
	CUIBase*	pUI = NULL;
		
	if (strcmpi(strTmp.c_str(), "uibase") == 0)
		pUI = createBase(pElement);
	else if( strcmpi(strTmp.c_str(), "uiimage") == 0 )
		pUI = createImage(pElement);
	else if( strcmpi(strTmp.c_str(), "uibutton") == 0 )
		pUI = createButton(pElement);
	else if( strcmpi(strTmp.c_str(), "uiedit") == 0 )
		pUI = createEdit(pElement);
	else if( strcmpi(strTmp.c_str(), "uicheck") == 0 )
		pUI = createCheck(pElement);
	else if( strcmpi(strTmp.c_str(), "uitext") == 0 )
		pUI = createText(pElement);
	else if( strcmpi(strTmp.c_str(), "uitextbox") == 0 )
		pUI = createTextBox(pElement);
	else if( strcmpi(strTmp.c_str(), "uiscrollbar") == 0 )
		pUI = createScroll(pElement);
	else if( strcmpi(strTmp.c_str(), "uicombobox") == 0 )
		pUI = createComboBox(pElement);
	else if( strcmpi(strTmp.c_str(), "uislidebar") == 0 )
		pUI = createSlideBar(pElement);
	else if( strcmpi(strTmp.c_str(), "uiprogressbar") == 0 )
		pUI = createProgressBar(pElement);
	else if( strcmpi(strTmp.c_str(), "uilist") == 0 )
		pUI = createList(pElement);
	else if( strcmpi(strTmp.c_str(), "uilistitem") == 0 )
		pUI = createListItem(pElement);
	else if( strcmpi(strTmp.c_str(), "uitab") == 0 )
		pUI = createTab(pElement);
	else if( strcmpi(strTmp.c_str(), "uitabpage") == 0 )
		pUI = createTabPage(pElement);
	else if( strcmpi(strTmp.c_str(), "uiicon") == 0 )
		pUI = createIcon(pElement);
	else if( strcmpi(strTmp.c_str(), "uispinbutton") == 0 )
		pUI = createSpinButton(pElement);
	else if( strcmpi(strTmp.c_str(), "uiimagefont") == 0 )
		pUI = createImageFont(pElement);
	else if( strcmpi(strTmp.c_str(), "uiarray") == 0 )
		pUI = createArray(pElement);
	else if( strcmpi(strTmp.c_str(), "uiarrayitem") == 0 )
		pUI = createArrayItem(pElement);
	else if( strcmpi(strTmp.c_str(), "uispriteani") == 0 )
		pUI = createSpriteAni(pElement);
	else if( strcmpi(strTmp.c_str(), "uiimagearray") == 0 )
		pUI = createImageArray(pElement);
	else if( strcmpi(strTmp.c_str(), "uiimagesplit") == 0 )
		pUI = createImageSprit(pElement);
	else if( strcmpi(strTmp.c_str(), "uitree") == 0 )
		pUI = createTree(pElement);
	else if( strcmpi(strTmp.c_str(), "uitreeitem") == 0 )
		pUI = createTreeItem(pElement);
	else if( strcmpi(strTmp.c_str(), "uispincontrol") == 0)
		pUI = createSpinControl(pElement);
	else if (strcmpi(strTmp.c_str(), "uitextboxex") == 0)
		pUI = createTextBoxEx(pElement);

	return pUI;
}

//------------------------------------------------------------------------

UIRect UIFactory::getBaseInfo( TiXmlElement* pElement )
{
	UIRect	rect;

	pElement->Attribute("x", &rect.Left);
	pElement->Attribute("y", &rect.Top);
	pElement->Attribute("w", &rect.Right);
	pElement->Attribute("h", &rect.Bottom);

	return rect;
}

bool UIFactory::getBaseValue( CUIBase* pUI, TiXmlElement* pElement )
{
	if( pUI == NULL || pElement == NULL )
		return false;

	const char* szID = pElement->Attribute("id");
	if( szID )
		pUI->setID( szID );

	szID = pElement->Attribute("parent");
	if (szID)
		pUI->setPID( szID );

	const char* szTex = pElement->Attribute("tex");
	if( szTex )
		pUI->setTexString( szTex );

	int nHide = 0;
	pElement->Attribute( "hide", &nHide );
	if(	(BOOL)nHide )
		pUI->Hide( (BOOL)nHide );

	UIRect rc = getBaseInfo(pElement);

	pUI->InitPos( rc.Left, rc.Top, rc.Right, rc.Bottom );

	int nTooltip = 0;
	if (pElement->Attribute("tooltip", &nTooltip) && nTooltip >= 0)
	{
#ifdef UI_TOOL
		pUI->SetTooltipIndex(nTooltip);
#endif // UI_TOOL
		pUI->setTooltip( CUIBase::getText( (INDEX)nTooltip ) );
	}

	int nTooltipWidth = 0;
	if (pElement->Attribute("tooltip_width", &nTooltipWidth) && nTooltipWidth >= 0)
		pUI->setTooltipWidth(nTooltipWidth);

	int align = 0;

	pElement->Attribute("align_h", &align);
	if (align != 0)
		pUI->setAlignH((eALIGN_H)align);

	align = 0;
	pElement->Attribute("align_v", &align);
	if (align != 0)
		pUI->setAlignV((eALIGN_V)align);

	return true;
}

bool UIFactory::getUV( UIRectUV* uv, TiXmlElement* pElement)
{
	if( uv == NULL )
		return false;

	double		l = 0, t = 0, r = 0, b = 0;
	pElement->Attribute("l", &l);
	pElement->Attribute("t", &t);
	pElement->Attribute("r", &r);
	pElement->Attribute("b", &b);

	uv->SetUV( (FLOAT)l, (FLOAT)t, (FLOAT)r, (FLOAT)b );

	return true;
}

bool UIFactory::getUVN( int n, UIRectUV* uv, TiXmlElement* pElement )
{
	if( uv == NULL )
		return false;

	int		i;
	char buf[4][64];

	sprintf( buf[0], "l%d", n );
	sprintf( buf[1], "t%d", n );
	sprintf( buf[2], "r%d", n );
	sprintf( buf[3], "b%d", n );

	double		dnum[4] = {0,};

	for (i = 0; i < 4; ++i)
		pElement->Attribute(buf[i], &dnum[i]);

	uv->SetUV( (FLOAT)dnum[0], (FLOAT)dnum[1], (FLOAT)dnum[2], (FLOAT)dnum[3] );

	return true;
}


CUIBase* UIFactory::createBase( TiXmlElement* pElement )
{
	CUIBase* pUI = new CUIBase;
	getBaseValue(pUI, pElement);

	return pUI;
}

CUIWindow* UIFactory::createWindow( TiXmlElement* pElement )
{
	CUIWindow* pUI = new CUIWindow;
	getBaseValue(pUI, pElement);

	return pUI;
}

CUIImage* UIFactory::createImage( TiXmlElement* pElement )
{
	CUIImage* pUI = new CUIImage;
	getBaseValue(pUI, pElement);
	
	UIRectUV uv;
	getUV( &uv, pElement );
	pUI->SetUV( uv );

	return pUI;
}

CUIButton* UIFactory::createButton( TiXmlElement* pElement )
{
	CUIButton* pUI = new CUIButton;
	getBaseValue( pUI, pElement );

	int		nStr = 0;
	int		nNewType = 0;
	std::string strTmp;
	int		nEdge = 0;
	
	char* tmp;

	if( pElement->Attribute("str_idx", &nStr) && nStr >= 0 )
	{
		pUI->SetText( CUIBase::getText( (INDEX)nStr ) );
#ifdef UI_TOOL
		pUI->setStringIdx((INDEX)nStr);
#endif // UI_TOOL
	}
	else if( tmp = (char*)pElement->Attribute("str") )
	{
		StackMultiByteToWideChar(CP_UTF8, tmp, -1, wcsResult);
		StackWideCharToMultiByte(CP_ACP, wcsResult, -1, szResult);
		pUI->SetText( CTString(szResult) );
	}

	if( pElement->Attribute("newtype", &nNewType) )
	{
		pUI->SetNewType( nNewType ? TRUE : FALSE );
	}

	if (pElement->Attribute("edge", &nEdge))
		pUI->SetEdge(nEdge ? true : false);

	{
		int i;
		
		if (tmp = (char*)pElement->Attribute("text_color"))
		{
			strTmp = tmp;
			for (i = 0; i < UBS_TOTAL; ++i)
			{
				pUI->SetTextColor( (UIBtnState)i, strtoul(strTmp.c_str(), NULL, 16) );
			}
		}
	}

	TiXmlElement* element = pElement->FirstChildElement();
	UIRectUV uv;

	for( ; element != NULL; element = element->NextSiblingElement() )
	{
		strTmp = element->Value();
		getUV( &uv, element );

		if( strcmpi(strTmp.c_str(), "uv") == 0 )
		{
			int		type = 0;
			if( element->Attribute("type", &type) )
			{
				pUI->SetUV( (UIBtnState)type, uv );
			}
		}
		else if( strcmpi(strTmp.c_str(), "rsurface") == 0 )
		{
			int		x = 0, y = 0, w = 0, h = 0, type;
			element->Attribute("x", &x);
			element->Attribute("y", &y);
			element->Attribute("w", &w);
			element->Attribute("h", &h);

			if( element->Attribute("type", &type) )
			{
				pUI->SetRTSurfaceEx( (UIBtnState)type, UIRect(x,y,w,h), uv );
			}
		}
		else if( strcmpi(strTmp.c_str(), "copyuv") == 0 )
		{
			int		src, dst;

			if( element->Attribute("src", &src) != NULL &&
				element->Attribute("dst", &dst) != NULL )
			{
				pUI->CopyUV( (UIBtnState)src, (UIBtnState)dst );
			}
		}
		else if( strcmpi(strTmp.c_str(), "copyrsurf") == 0 )
		{
			int		src, dst;

			if( element->Attribute("src", &src) != NULL &&
				element->Attribute("dst", &dst) != NULL )
			{
				pUI->CopyRTSurface( (UIBtnState)src, (UIBtnState)dst );
			}
		}
	}

	return pUI;
}

CUIEditBox* UIFactory::createEdit( TiXmlElement* pElement )
{
	CUIEditBox* pUI = new CUIEditBox;
	getBaseValue( pUI, pElement );

	// max 값은 버퍼 초기화에 필요 기본값으로 10을 넣는다.
	int		max = 10, ipasswd = 0, nCursorMove, only_num;	
	
	pElement->Attribute("max", &max);
	pUI->SetMaxChar( max );

	if (pElement->Attribute("passwd", &ipasswd))
		pUI->SetPassWord( ipasswd ? TRUE : FALSE );

	pElement->Attribute("cursor_move", &nCursorMove );
	pUI->SetCursorMove( nCursorMove ? TRUE : FALSE );

	if (pElement->Attribute("only_num", &only_num ))
		pUI->SetOnlyIntegerMode( only_num ? TRUE : FALSE );

	{
		FLOAT fTexW = (FLOAT)pUI->getTexWidth();
		FLOAT fTexH = (FLOAT)pUI->getTexHeight();

		std::string strTmp;
		TiXmlElement* element = pElement->FirstChildElement();
		UIRectUV uv;

		for( ; element != NULL; element = element->NextSiblingElement() )
		{
			strTmp = element->Value();
			getUV( &uv, element );

			if( strcmpi(strTmp.c_str(), "readuv") == 0 )
				pUI->SetReadingWindowUV( uv.U0, uv.V0, uv.U1, uv.V1, fTexW, fTexH );
			else if( strcmpi(strTmp.c_str(), "candidateuv") == 0 )
				pUI->SetCandidateUV( uv.U0, uv.V0, uv.U1, uv.V1, fTexW, fTexH );
		}
	}

	std::string strTmp;
	TiXmlElement* element = pElement->FirstChildElement();

	for( ; element != NULL; element = element->NextSiblingElement() )
	{
		strTmp = element->Value();

		if (strcmpi(strTmp.c_str(), "back") == 0)
		{
			UIRect rect = getBaseInfo(element);
			UIRectUV uv;
			getUV(&uv, element);

			pUI->setBackGround(rect, uv);
		}
	}

	return pUI;
}

CUICheckButton* UIFactory::createCheck( TiXmlElement* pElement )
{
	CUICheckButton* pUI = new CUICheckButton;
	getBaseValue( pUI, pElement );

	int		iLeft, iTextX, iRegion, nString, textArea = 0, h_align;	
	int nEdge = 0;

	char* tmp;
	if( pElement->Attribute("str_idx", &nString) && nString >= 0 )
	{
		pUI->SetText( CUIBase::getText( (INDEX)nString ) );
#ifdef UI_TOOL
		pUI->setStringIndex((INDEX)nString);
#endif // UI_TOOL
	}
	else if( tmp = (char*)pElement->Attribute("str") )
	{
		StackMultiByteToWideChar(CP_UTF8, tmp, -1, wcsResult);
		StackWideCharToMultiByte(CP_ACP, wcsResult, -1, szResult);
		pUI->SetText( CTString(szResult) );
	}

	pElement->Attribute("posleft", &iLeft);
	pElement->Attribute("text_x", &iTextX);
	pElement->Attribute("region", &iRegion);

	pUI->SetCheckRegion( iLeft ? TRUE : FALSE, iTextX, iRegion );

	if (pElement->Attribute("text_area", &textArea))
		pUI->setTextArea(textArea);
	if (pElement->Attribute( "h_align", &h_align ))
		pUI->setAlignTextH( (eALIGN_H)h_align );

	std::string strTmp;

	strTmp = pElement->Attribute("color_on");
	if( strTmp.empty() == false )
		pUI->SetTextColor( TRUE, strtoul(strTmp.c_str(), NULL, 16) );

	strTmp = pElement->Attribute("color_off");
	if( strTmp.empty() == false )
		pUI->SetTextColor( FALSE, strtoul(strTmp.c_str(), NULL, 16) );

	if (pElement->Attribute("edge", &nEdge))
		pUI->setEdge(nEdge ? true : false);

	TiXmlElement* element = pElement->FirstChildElement();
	UIRectUV uv;

	for( ; element != NULL; element = element->NextSiblingElement() )
	{
		strTmp = element->Value();
		getUV( &uv, element );

		if( strcmpi(strTmp.c_str(), "none") == 0 )
			pUI->SetUVTex( UCBS_NONE, uv );
		else if( strcmpi(strTmp.c_str(), "check") == 0 )
			pUI->SetUVTex( UCBS_CHECK, uv );
	}

	return pUI;
}

CUIText* UIFactory::createText( TiXmlElement* pElement )
{
	CUIText* pUI = new CUIText;
	getBaseValue( pUI, pElement );

	char* tmp;
	int		nString, nShadow;
	int		h_align = 0;
	int		nEdge = 0;
	char* szEllipsis;

	if (szEllipsis = (char*)pElement->Attribute("str_ellipsis"))
	{
		StackMultiByteToWideChar(CP_UTF8, szEllipsis, -1, wcsResult);
		StackWideCharToMultiByte(CP_ACP, wcsResult, -1, szResult);
		szEllipsis = szResult;
	}
	else
	{
		szEllipsis = "";
	}

	if( pElement->Attribute("str_idx", &nString) && nString >= 0)
	{
		pUI->SetText( CUIBase::getText( (INDEX)nString ), szEllipsis );
#ifdef	UI_TOOL
		pUI->setStringIdx((INDEX)nString);
#endif	// UI_TOOL
	}
	else if( tmp = (char*)pElement->Attribute("str") )
	{
		StackMultiByteToWideChar(CP_UTF8, tmp, -1, wcsResult);
		StackWideCharToMultiByte(CP_ACP, wcsResult, -1, szResult);
		pUI->SetText( CTString(szResult), szEllipsis);
	}

	tmp = (char*)pElement->Attribute("color");	
	if( tmp != NULL )
		pUI->setFontColor( (COLOR)strtoul(tmp, NULL, 16) );

	pElement->Attribute("shadow", &nShadow);

	if( nShadow )
	{
		tmp = (char*)pElement->Attribute("shadow_color");
		if( tmp != NULL )
			pUI->setFontShadow( (COLOR)strtoul(tmp, NULL, 16), nShadow );
	}

	if (pElement->Attribute( "h_align", &h_align ))
		pUI->setAlignTextH( (eALIGN_H)h_align );

	if (pElement->Attribute("edge", &nEdge))
		pUI->setEdge(nEdge);

	std::string strTmp;
	TiXmlElement* element = pElement->FirstChildElement();

	for( ; element != NULL; element = element->NextSiblingElement() )
	{
		strTmp = element->Value();

		if (strcmpi(strTmp.c_str(), "back") == 0)
		{
			UIRect rect = getBaseInfo(element);
			UIRectUV uv;
			getUV(&uv, element);

			pUI->setBackGround(rect, uv);
		}
	}

	return pUI;
}

CUITextBox* UIFactory::createTextBox( TiXmlElement* pElement )
{
	CUITextBox* pUI = new CUITextBox;
	getBaseValue( pUI, pElement );

	int nSX, nSY, h_align, nGapY, nSplit = 0;
	
	if( pElement->Attribute( "sx", &nSX ) && pElement->Attribute( "sy", &nSY ) )
		pUI->SetStartXY( nSX, nSY );
	else if( pElement->Attribute( "space_x", &nSX) && pElement->Attribute( "space_y", &nSY))	// 기존에 사용하던 이름 변경.
		pUI->SetStartXY( nSX, nSY );

 	if (pElement->Attribute( "h_align", &h_align ))
 		pUI->SetAlignTextH( h_align );

	if (pElement->Attribute( "gap_y", &nGapY ))
		pUI->SetGapY( nGapY );

	if (pElement->Attribute( "split_mode", &nSplit ))
		pUI->SetSplitMode((eSPLIT_TYPE)nSplit);

	char* tmp;
	int	nString;
	
	if( pElement->Attribute("str_idx", &nString) && nString >= 0 )
	{
		if( tmp = (char*)pElement->Attribute("text_col") )
			pUI->SetText( CUIBase::getText( (INDEX)nString ), (COLOR)strtoul(tmp, NULL, 16) );
		else
		pUI->SetText( CUIBase::getText( (INDEX)nString ) );

#ifdef	UI_TOOL
		pUI->setStringIdx((INDEX)nString);
#endif	// UI_TOOL
	}
	else if( tmp = (char*)pElement->Attribute("str") )
	{
		StackMultiByteToWideChar(CP_UTF8, tmp, -1, wcsResult);
		StackWideCharToMultiByte(CP_ACP, wcsResult, -1, szResult);

		if( tmp = (char*)pElement->Attribute("text_col") )
			pUI->SetText( CTString(szResult) , (COLOR)strtoul(tmp, NULL, 16) );
		else
			pUI->SetText( CTString(szResult) );
	}

	std::string strTmp;
	TiXmlElement* element = pElement->FirstChildElement();

	for( ; element != NULL; element = element->NextSiblingElement() )
	{
		strTmp = element->Value();

		if (strcmpi(strTmp.c_str(), "back") == 0)
		{
			UIRect rect = getBaseInfo(element);
			UIRectUV uv;
			getUV(&uv, element);

			pUI->setBackGround(rect, uv);
		}
	}
	return pUI;
}

CUIScrollBar* UIFactory::createScroll( TiXmlElement* pElement )
{
	CUIScrollBar* pUI = new CUIScrollBar;
	getBaseValue( pUI, pElement );

	int		nVertical = 1, nRight = 1;
	int		nScroll_Pos, item_count, nItem_Per_Page;
	double	move_unit;

	if (pElement->Attribute( "vertical", &nVertical ))
		pUI->setVertical( nVertical ? TRUE : FALSE );

	if (pElement->Attribute("right", &nRight))
		pUI->setRight(nRight ? true : false);

 	if (pElement->Attribute( "scroll_pos", &nScroll_Pos ))
 		pUI->SetScrollPos( nScroll_Pos ); 	

	if (pElement->Attribute( "move_unit", &move_unit))
		pUI->setItemSize( (float)move_unit );

	std::string strTmp;
	TiXmlElement* element = pElement->FirstChildElement();
	
	for( ; element != NULL; element = element->NextSiblingElement() )
	{
		strTmp = element->Value();
		UIRectUV uv;
		BOOL bUV = getUV( &uv, element );
		if( strcmpi(strTmp.c_str(), "back") == 0 )
		{
			UIRect rect = getBaseInfo(element);
			UIRectUV uv;
			getUV(&uv, element);

			pUI->setBackGround(rect, uv);
		}
		else if (strcmpi(strTmp.c_str(), "button1") == 0 || 
				strcmpi(strTmp.c_str(), "button2") == 0)
		{
			int idx = 0;
			if (strcmpi(strTmp.c_str(), "button2") == 0)
				idx = 1;

			UIRect rect = getBaseInfo(element);			

			UIRectUV uv0, uv1;
			getUVN( 0, &uv0, element );
			getUVN( 1, &uv1, element );

			pUI->setButton( idx, rect, uv0, uv1 );
		}
		else if (strcmpi(strTmp.c_str(), "thumb") == 0)
		{
			int		unit = 1;
			UIRect rect = getBaseInfo(element);
			UIRectUV uv;
			getUV(&uv, element);

			element->Attribute("unit", &unit);			

			pUI->setThumb(rect, uv, unit);
		}
	}

	// 아이템 갯수 처리를 마지막에
	if (pElement->Attribute( "item_count", &item_count ))
		pUI->SetCurItemCount( item_count );
	// 
	// 	pElement->Attribute( "cur_item_cnt", &nCur_ItemCnt );
	// 	pUI->SetCurItemCount( nCur_ItemCnt );
	// 
	if (pElement->Attribute( "item_per_page", &nItem_Per_Page ))
		pUI->SetItemsPerPage( nItem_Per_Page );

	return pUI;
}

CUIComboBox* UIFactory::createComboBox( TiXmlElement* pElement )
{
	CUIComboBox* pUI = new CUIComboBox;
	getBaseValue( pUI, pElement );

	int nMaxLine, nLineHeight, nSpaceX, nSpaceY, show_line, h_align;
	pElement->Attribute( "maxline", &nMaxLine );
	pUI->setMaxLine( nMaxLine );

	pElement->Attribute( "lineheight", &nLineHeight );
	pUI->setLineHeight( nLineHeight );

	pElement->Attribute( "space_x", &nSpaceX );
	pElement->Attribute( "space_y", &nSpaceY );
	pUI->setSpace( nSpaceX, nSpaceY );

	if (pElement->Attribute( "showline", &show_line ))
		pUI->setShowLine(show_line);

	if (pElement->Attribute( "h_align", &h_align ))
		pUI->setAlignTextH( h_align );

	std::string strTmp;
	TiXmlElement* element = pElement->FirstChildElement();

	for( ; element != NULL; element = element->NextSiblingElement() )
	{
		strTmp = element->Value();

		if (strcmpi(strTmp.c_str(), "back") == 0)
		{
			UIRect rect = getBaseInfo(element);
			UIRectUV uv;
			getUV(&uv, element);

			pUI->setBackGround(rect, uv);
		}
		else if( strcmpi(strTmp.c_str(), "button") == 0 )
		{
			UIRect rect = getBaseInfo(element);			

			UIRectUV uv0, uv1;
			getUVN( 0, &uv0, element );
			getUVN( 1, &uv1, element );

			pUI->setComboButton( rect, uv0, uv1 );
		}
		else if (strcmpi(strTmp.c_str(), "drop_bg") == 0)
		{
			UIRect rect = getBaseInfo(element);
			UIRectUV uv;
			getUV(&uv, element);

			pUI->setDropBG(rect, uv);
		}
	}

#ifdef	UI_TOOL
	{
		int		i;
		char	buf[32] = {0,};

		for (i = 0; i < nMaxLine; ++i)
		{
			sprintf(buf, "%c%c%c", (65+i), (65+i), (65+i));
			pUI->AddString(CTString(buf));
		}
	}
	// test end
#endif	// UI_TOOL

	return pUI;
}

CUISlideBar* UIFactory::createSlideBar( TiXmlElement* pElement )
{
	CUISlideBar* pUI = new CUISlideBar;
	getBaseValue( pUI, pElement );
	std::string strTmp;

	int nBarW, nBarH, nCurPos, nRange, nMin, nMax;
	pElement->Attribute("bar_w", &nBarW);
	pElement->Attribute("bar_h", &nBarH);
	pUI->setBar(nBarW, nBarH);

	pElement->Attribute("cur_pos", &nCurPos);
	pUI->SetCurPos(nCurPos);

	pElement->Attribute("range", &nRange);
	pUI->SetRange(nRange);

	pElement->Attribute("min", &nMin);
	pUI->SetMinPos(nMin);

	pElement->Attribute("max", &nMax);
	pUI->SetMaxPos(nMax);

	FLOAT fTexW = 1.0f;
	FLOAT fTexH = 1.0f;

	TiXmlElement* element = pElement->FirstChildElement();
	for( ; element != NULL; element = element->NextSiblingElement() )
	{
		fTexW = (FLOAT)pUI->getTexWidth();
		fTexH = (FLOAT)pUI->getTexHeight();

		strTmp = element->Value();
		UIRectUV uv;
		BOOL bUV = getUV( &uv, element );

		if( bUV == FALSE ) 
			continue;

		if( strcmpi(strTmp.c_str(), "back") == 0 )
		{
			pUI->SetBackgroundUV(uv.U0, uv.V0, uv.U1, uv.V1, fTexW, fTexH);
		}
		else if( strcmpi(strTmp.c_str(), "bar") == 0 )
		{
			pUI->SetBarUV(uv.U0, uv.V0, uv.U1, uv.V1, fTexW, fTexH);
		}
	}

	return pUI;
}

CUIProgressBar* UIFactory::createProgressBar( TiXmlElement* pElement )
{
	CUIProgressBar* pUI = new CUIProgressBar;
	getBaseValue( pUI, pElement );
	std::string strTmp;

	UIRectUV uv;
	bool bUV = getUV(&uv, pElement);
	if( bUV )
		pUI->setUVBack(uv);

	int i, nLower, nUpper, nStep, nOffsetPos, nAutoComplete;

	if( pElement->Attribute("lower", &nLower) &&
		pElement->Attribute("upper", &nUpper) )
		pUI->SetProgressRange(nLower, nUpper);

	if( pElement->Attribute("step", &nStep) )
		pUI->SetProgressStep(nStep);

	if( pElement->Attribute("offset_pos", &nOffsetPos) )
		pUI->OffsetProgressPos(nOffsetPos);

	if( pElement->Attribute("auto_complete", &nAutoComplete) )
		pUI->SetAutoComplete( nAutoComplete ? TRUE : FALSE );

	const char* szName[] = {"idle_text", "doing_text", "complete_text"};

	TiXmlElement* element = pElement->FirstChildElement();
 	for( ; element != NULL; element = element->NextSiblingElement() )
 	{
 		strTmp = element->Value();
		if( strcmpi(strTmp.c_str(), "bar") == 0 )
		{
			int nSX, nSY;
			if( element->Attribute("sx", &nSX) && nSX >= 0 &&
				element->Attribute("sy", &nSY) && nSY >= 0 )
				pUI->setBarStartPos(nSX, nSY);

			bUV = getUV(&uv, element);
			if( bUV )
				pUI->setUVBar(uv);
		}

		for( i = 0; i < PR_STAT_TAG_END; i++ )
		{
			if( strcmpi(strTmp.c_str(), szName[i]) == 0 )
			{
				int		nStr, nShowPercent = FALSE;
				char*   strText;
				std::string strCol;

				strCol = element->Attribute("color");
				element->Attribute("show_percent", &nShowPercent);

				if( element->Attribute("str_idx", &nStr) && nStr >= 0)
				{
#ifdef UI_TOOL
					pUI->setStringIdx((_tagProgressStatus)i, (INDEX)nStr);
#endif // UI_TOOL
					pUI->setProgressText( (_tagProgressStatus)i, CUIBase::getText( (INDEX)nStr ), nShowPercent ? TRUE:FALSE
							, (COLOR)strtoul(strCol.c_str(), NULL, 16) );
				}
				else if( strText = (char*)element->Attribute("str") )
				{
					StackMultiByteToWideChar(CP_UTF8, strText, -1, wcsResult);
					StackWideCharToMultiByte(CP_ACP, wcsResult, -1, szResult);
					pUI->setProgressText( (_tagProgressStatus)i, CTString(szResult), nShowPercent ? TRUE:FALSE
						, (COLOR)strtoul(strCol.c_str(), NULL, 16) );
				}
			}
		}
 	}

	return pUI;
}

CUIList* UIFactory::createList( TiXmlElement* pElement )
{
	CUIList* pUI = new CUIList;
	getBaseValue( pUI, pElement );

	int		ix, iy, ishow, igap, count, pre_create;

	if (pElement->Attribute("itemx", &ix) &&
		pElement->Attribute("itemy", &iy) )
		pUI->SetItemStart(ix, iy);

	if (pElement->Attribute("itemshow_num", &ishow))
		pUI->SetItemShowNum(ishow);
	if (pElement->Attribute("itemgap", &igap))
		pUI->SetGap(igap);

	if (pElement->Attribute("item_count", &count))
		pUI->SetItemCount(count);

	if (pElement->Attribute("pre_create", &pre_create))
		pUI->SetPreCreate(pre_create ? true : false);

	std::string strTmp;
	TiXmlElement* element = pElement->FirstChildElement();

	UIRect rc;
	UIRectUV uv;

	for( ; element != NULL; element = element->NextSiblingElement() )
	{
		strTmp = element->Value();
		if(strcmpi(strTmp.c_str(), "select") == 0)
		{
			rc = getBaseInfo(element);
			getUV(&uv, element);

			pUI->addEventImage(rc, uv, CUIList::eTYPE_SELECT);
		}
		else if(strcmpi(strTmp.c_str(), "over") == 0)
		{
			rc = getBaseInfo(element);
			getUV(&uv, element);

			pUI->addEventImage(rc, uv, CUIList::eTYPE_OVER);
		}
	}

	return pUI;
}

CUIListItem* UIFactory::createListItem( TiXmlElement* pElement )
{
	CUIListItem* pUI = new CUIListItem;
	getBaseValue( pUI, pElement );

	return pUI;
}

CUITab* UIFactory::createTab( TiXmlElement* pElement )
{
	CUITab* pUI = new CUITab;
	getBaseValue( pUI, pElement );

	return pUI;
}

CUITabPage* UIFactory::createTabPage( TiXmlElement* pElement )
{
	CUITabPage* pUI = new CUITabPage;
	getBaseValue( pUI, pElement );

	return pUI;
}

CUIIcon* UIFactory::createIcon( TiXmlElement* pElement )
{
	CUIIcon* pUI = new CUIIcon;
	getBaseValue( pUI, pElement );

	int type, texid, row, col;
	if ( pElement->Attribute("type", &type) &&
		 pElement->Attribute("texid", &texid) &&
		 pElement->Attribute("row", &row) &&
		 pElement->Attribute("col", &col) )
	{
		pUI->setInfo((UIBtnExType)type, texid, row, col);
	}

	return pUI;
}

CUISpinButton* UIFactory::createSpinButton( TiXmlElement* pElement )
{
	CUISpinButton* pUI = new CUISpinButton;
	getBaseValue( pUI, pElement );

	char*		szTitle;
	std::string strTmp;
	if( szTitle = (char*)pElement->Attribute("title") )
		pUI->SetTitle(szTitle);

	UIRectUV	uv;
	int			nBtnW, nBtnH, nBtnGap, nDataBackWidth;
	bool		bUV			= getUV(&uv, pElement);

	if( bUV )
		pUI->setUVDataBack(uv);

	if( pElement->Attribute("data_back_w", &nDataBackWidth) )
		pUI->setDataBackWidth(nDataBackWidth);
	
	if( pElement->Attribute("btn_w", &nBtnW) >= 0 &&
		pElement->Attribute("btn_h", &nBtnH) >= 0 &&
		pElement->Attribute("btn_gap", &nBtnGap) >= 0 )
	{
		pUI->setSpinButton( nBtnW, nBtnH, nBtnGap );
	}

	TiXmlElement* element = pElement->FirstChildElement();
	for( ; element != NULL; element = element->NextSiblingElement() )
	{
		strTmp = element->Value();
		if( strcmpi(strTmp.c_str(), "wheel") == 0 )
		{
			UIRect	rcWheel = getBaseInfo(element);
			pUI->SetWheelRect(rcWheel.Left, rcWheel.Top, rcWheel.Right, rcWheel.Bottom);
		}
	}
	
#ifdef UI_TOOL
	BOOL		bAutoInfo;
	pElement->Attribute("auto_info", &bAutoInfo);
	pUI->setBtnAutoInfo(bAutoInfo ? true : false);

	pUI->AddData("aaa");
	pUI->AddData("bbb");
	pUI->AddData("ccc");
	pUI->AddData("ddd");
#endif // UI_TOOL
	return pUI;
}

CUIImageFont* UIFactory::createImageFont( TiXmlElement* pElement )
{
	CUIImageFont* pUI = new CUIImageFont;
	getBaseValue( pUI, pElement );

	int sx, sy, gap, ogap;
	if (pElement->Attribute("fsizex", &sx) &&
		pElement->Attribute("fsizey", &sy) &&
		pElement->Attribute("fgap", &gap) )
	{
		pUI->setFontSizeGap(sx, sy, gap);
	}

	if (pElement->Attribute("ogap", &ogap))
		pUI->setOutputGap(ogap);

	char* szTmp;
	if ( szTmp = (char*)pElement->Attribute("src") )
	{
		StackMultiByteToWideChar(CP_UTF8, szTmp, -1, wcsResult);
		StackWideCharToMultiByte(CP_ACP, wcsResult, -1, szResult);
		pUI->setOrigString( szResult ); 
	}

	if ( szTmp = (char*)pElement->Attribute("str") )
	{
		StackMultiByteToWideChar(CP_UTF8, szTmp, -1, wcsResult);
		StackWideCharToMultiByte(CP_ACP, wcsResult, -1, szResult);
		pUI->setString( szResult );
	}

	std::string strTmp;
	TiXmlElement* element = pElement->FirstChildElement();

	for( ; element != NULL; element = element->NextSiblingElement() )
	{
		strTmp = element->Value();

		if (strcmpi(strTmp.c_str(), "srcuv") == 0)
		{
			UIRectUV uv;
			getUV(&uv, element);

			pUI->setSourceImage(uv);
		}
	}

	return pUI;
}

CUIArray* UIFactory::createArray( TiXmlElement* pElement )
{
	CUIArray* pUI = new CUIArray;
	getBaseValue( pUI, pElement );

	int row, col, sx, sy, gapX, gapY, nShowRow;

	if( pElement->Attribute("row", &row) && 
		pElement->Attribute("col", &col) )
		pUI->SetRowCol(row, col);

	if( pElement->Attribute("sx", &sx) &&
		pElement->Attribute("sy", &sy) )
		pUI->SetStartItem(sx, sy);

	if( pElement->Attribute("gapx", &gapX) &&
		pElement->Attribute("gapy", &gapY) )
		pUI->SetGap(gapX, gapY);

	if( pElement->Attribute("show_row", &nShowRow) )
		pUI->SetShowRowCount(nShowRow);
		
	TiXmlElement* element = pElement->FirstChildElement();
	
	std::string strTmp;
	UIRect rect;
	UIRectUV uv;
	for( ; element != NULL; element = element->NextSiblingElement() )
	{
		strTmp = element->Value();
		if(strcmpi(strTmp.c_str(), "select") == 0)
		{
			rect = getBaseInfo(element);
			getUV(&uv, element);

			pUI->SetMouseEventImage(rect, uv, 0);
		}
		else if(strcmpi(strTmp.c_str(), "mouse_over") == 0)
		{
			rect = getBaseInfo(element);
			getUV(&uv, element);

			pUI->SetMouseEventImage(rect, uv, 1);
		}
	}

	return pUI;
}

CUIArrayItem* UIFactory::createArrayItem( TiXmlElement* pElement )
{
	CUIArrayItem* pUI = new CUIArrayItem;
	getBaseValue( pUI, pElement );

	return pUI;
}

CUISpriteAni* UIFactory::createSpriteAni( TiXmlElement* pElement )
{
	CUISpriteAni* pUI = new CUISpriteAni;
	getBaseValue( pUI, pElement );

	int nCurFrame;

	pElement->Attribute("cur", &nCurFrame);

	pUI->SetRenderIdx( nCurFrame );

	TiXmlElement* element = pElement->FirstChildElement();

	std::string strTmp;
	UIRect rect;
	UIRectUV uv;
	for( ; element != NULL; element = element->NextSiblingElement() )
	{
		strTmp = element->Value();
		if(strcmpi(strTmp.c_str(), "uv") == 0)
		{
			getUV(&uv, element);
			pUI->PushUV( uv );
		}
	}
	return pUI;
}

CUIImageArray* UIFactory::createImageArray( TiXmlElement* pElement )
{
	CUIImageArray* pUI = new CUIImageArray;
	getBaseValue( pUI, pElement );

	int curIdx;
	pElement->Attribute("render_idx", &curIdx);
	if (curIdx > 0)
		pUI->SetRenderIdx(curIdx);

//	std::string strCol = pElement->Attribute("color");

	UIRect rc;
	UIRectUV uv;
	std::string strTmp;
	CTString strName;
	int i = 0;

	TiXmlElement* element = pElement->FirstChildElement();

	for (; element != NULL; element = element->NextSiblingElement())
	{
		strTmp = element->Value();
		strName.PrintF("surface%d", i++);

		if (strcmpi(strTmp.c_str(), strName) == 0)
		{
			rc = getBaseInfo(element);
			getUV(&uv, element);

			char *tmp;
			if (tmp = (char*)element->Attribute("desc"))
			{
				StackMultiByteToWideChar(CP_UTF8, tmp, -1, wcsResult);
				StackWideCharToMultiByte(CP_ACP, wcsResult, -1, szResult);
				pUI->AddImage(rc, uv, -1, szResult);
			}
		}
	}

	return pUI;
}

CUIImageSplit* UIFactory::createImageSprit( TiXmlElement* pElement )
{
	CUIImageSplit* pUI = new CUIImageSplit;
	getBaseValue(pUI, pElement);

	UIRectUV uv;
	getUV( &uv, pElement );
	pUI->SetUV( uv );

	int nUnit;
	pElement->Attribute("unit", &nUnit);

	int nSplit;
	pElement->Attribute("splitmode", &nSplit);

	pUI->SetUnit(nUnit);
	pUI->SetSplitMode((CUIImageSplit::eSPLIT_TYPE)nSplit);

	pUI->UpdateSplit();

	return pUI;
}

CUITree* UIFactory::createTree( TiXmlElement* pElement )
{
	int		pre_create, ishow;

	CUITree* pUI = new CUITree;
	getBaseValue(pUI, pElement);

	if (pElement->Attribute("show_num", &ishow))
		pUI->setShow(ishow);

	if (pElement->Attribute("pre", &pre_create))
		pUI->setPreCreate(pre_create > 0 ? true : false);

	return pUI;
}

CUISpinControl* UIFactory::createSpinControl( TiXmlElement* pElement )
{
	CUISpinControl* pUI = new CUISpinControl;
	getBaseValue( pUI, pElement );

	// max 값은 버퍼 초기화에 필요 기본값으로 10을 넣는다.
	int		min = 0, nCursorMove, nCursorSX;	
	int nBuffer = 16;
	SQUAD max = 0;
	
	char* szTmp;
	
	if ( szTmp = (char*)pElement->Attribute("max") )
		max = _atoi64(szTmp);

	pElement->Attribute("min", &min);
	
	pUI->SetMinMax(min, max);
	pUI->SetMaxChar(nBuffer);

	pElement->Attribute("cursor_move", &nCursorMove );
	pUI->SetCursorMove( nCursorMove ? TRUE : FALSE );

	pElement->Attribute("cursor_sx", &nCursorSX );
	pUI->SetCursorSX( nCursorSX );

	pUI->SetOnlyIntegerMode( TRUE );

	std::string strTmp;
	TiXmlElement* element = pElement->FirstChildElement();

	for( ; element != NULL; element = element->NextSiblingElement() )
	{
		strTmp = element->Value();

		if (strcmpi(strTmp.c_str(), "back") == 0)
		{
			UIRect rect = getBaseInfo(element);
			UIRectUV uv;
			int nUnit;
			
			getUV(&uv, element);
			element->Attribute("unit", &nUnit);
			
			pUI->SetBackGround(rect, uv, nUnit);
		}
	}

	return pUI;
}

CUITreeItem* UIFactory::createTreeItem( TiXmlElement* pElement )
{
	CUITreeItem* pUI = new CUITreeItem;
	getBaseValue(pUI, pElement);

	return pUI;
}

CUITextBoxEx* UIFactory::createTextBoxEx( TiXmlElement* pElement )
{
	CUITextBoxEx* pUI = new CUITextBoxEx;
	getBaseValue(pUI, pElement);

	int nSX, nSY, nGapY, nSplit = 0;

	if( pElement->Attribute( "sx", &nSX ) && pElement->Attribute( "sy", &nSY ) )
	{
		pUI->SetStartX(nSX);
		pUI->SetStartY(nSY);
	}

	if (pElement->Attribute( "gap_y", &nGapY ))
		pUI->SetGapY( nGapY );

	if (pElement->Attribute( "split_mode", &nSplit ))
		pUI->SetSplitMode((eSPLIT_TYPE)nSplit);

	char* tmp;
	int	nString;

	if( pElement->Attribute("str_idx", &nString) && nString >= 0 )
	{
		if( tmp = (char*)pElement->Attribute("text_base_col") )
		{
			std::string strTemp = CUIBase::getText( (INDEX)nString );
			pUI->AddText( strTemp, (COLOR)strtoul(tmp, NULL, 16) );
		}
	}
	else if( tmp = (char*)pElement->Attribute("str") )
	{
		StackMultiByteToWideChar(CP_UTF8, tmp, -1, wcsResult);
		StackWideCharToMultiByte(CP_ACP, wcsResult, -1, szResult);

		if( tmp = (char*)pElement->Attribute("text_base_col") )
			pUI->AddText( std::string(szResult) , (COLOR)strtoul(tmp, NULL, 16) );
		else
			pUI->AddText( std::string(szResult) );
	}

	return pUI;
}

// ------------------------------------------------------------------------
