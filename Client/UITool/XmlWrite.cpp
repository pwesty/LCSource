#include "StdAfx.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "XmlWrite.h"

#include <Engine/Util.h>


XmlWrite::XmlWrite(void)
	:m_MyWindowElement(NULL)
{
}


XmlWrite::~XmlWrite(void)
{
}

void XmlWrite::WriteXml( CUIBase* pUI, char* szFileName )
{
	if( pUI == NULL )
	 	return;

	// ListItem 등 특수한 컨트롤을 저장할 모습으로 만든다.
	Rearrange4Write();
	
	TiXmlDeclaration* pDec = new TiXmlDeclaration("1.0","UTF-8","");
	
	m_MyDoc.LinkEndChild(pDec);

	if( pUI->getType() == eUI_CONTROL_WINDOW )
		writeWindow( pUI );

	if( m_MyWindowElement )
		ChildControl(pUI, m_MyWindowElement);

	std::string strTmp;
	int len = strlen(szFileName);
	for( int i = 0; i < len + 1; i++ )
	{
		if( szFileName[i] == '.' )
		{
			strTmp = szFileName[i];
			if( i+3 > len + 1)
				break;
			strTmp += szFileName[i+1];
			strTmp += szFileName[i+2];
			strTmp += szFileName[i+3];
			if( strcmpi(strTmp.c_str(), ".xml") != 0 )
				continue;
		}
	}
	if( strcmpi(strTmp.c_str(), ".xml") != 0 )
	{
		strTmp = szFileName;
		strTmp += ".xml";
	}
	else
		strTmp = szFileName;
	
	m_MyDoc.SaveFile(strTmp.c_str());
}

void XmlWrite::Rearrange4Write()
{
	// 특수 컨트롤을 찾는다.
	CUIManager* pUIMgr = CUIManager::getSingleton();

	CUIBase* pUI = pUIMgr->getChildAt(0), *pChild, *pChildChild, *pFinder;
	//CUIList* pList; 

	int		i, nMax, nChild, nChildMax;
	eUI_CONTROL eType;
	//pList = (CUIList*)RearrangeFind(eUI_CONTROL_LIST, pUI);
	pFinder = RearrangeFind(eUI_CONTROL_LIST, pUI);
	eType = eUI_CONTROL_LIST_ITEM;
	if( pFinder == NULL )
	{
		pFinder = RearrangeFind(eUI_CONTROL_ARRAY, pUI);
		eType = eUI_CONTROL_ARRAY_ITEM;
	}

	while(pFinder)
	{
		pFinder->SetRearrangeMark(true);

		nMax = pFinder->getChildCount();

		if (nMax <= 0)
			return;

		for (i = 0; i < nMax; ++i)
		{
			pChild = pFinder->getChildAt(i);

			if ( pChild &&  pChild->getType() == eType )
			{
				nChildMax = pChild->getChildCount();

				for (nChild = 0; nChild < nChildMax; ++nChild)
				{
					pChildChild = pChild->getChildAt(nChild);

					if (strlen(pChildChild->getPID()) > 0)
					{
						pChild->eraseChild(pChildChild);
						pFinder->addChild(pChildChild);

						nChildMax = pChild->getChildCount();
						--nChild;	// erase 가 되면, '--' 해준다. 
					}
				}
			}
		}

		pFinder = RearrangeFind(eType, pChild);
	}
}

CUIBase* XmlWrite::RearrangeFind(eUI_CONTROL eType, CUIBase* pUI)
{
	if( pUI == NULL )
		return pUI;
	if (pUI->getType() == eType && pUI->GetRearrangeMark() != false)
		return pUI;

	return RearrangeFindChild(eType, pUI);
}

CUIBase* XmlWrite::RearrangeFindChild(eUI_CONTROL eType, CUIBase* pUI)
{
	CUIBase* pChild;
	CUIBase* pRetUI;
	int i,  nMax = pUI->getChildCount();

	for (i = 0; i < nMax; ++i)
	{
		pChild = pUI->getChildAt(i);

		pRetUI = RearrangeFind(eType, pChild);

		if (pRetUI != NULL)
			return (CUIBase*)pRetUI;
	}

	return NULL;
}

void XmlWrite::ChildControl( CUIBase* pUI, TiXmlElement* pRoot )
{
	int			i, nMaxChild = pUI->getChildCount();
	if( nMaxChild < 0 )
		return;

	/////////////////////////// 특별 컨트롤 관리 /////////////////////////////
	if( pUI->getType() == eUI_CONTROL_LIST )
	{
		ChildSpecialControl(pUI, pRoot, eUI_CONTROL_LIST_ITEM);
		return;
	}
	else if( pUI->getType() == eUI_CONTROL_ARRAY )
	{
		ChildSpecialControl(pUI, pRoot, eUI_CONTROL_ARRAY_ITEM);
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	CUIBase*	pChild = NULL;

	for( i = 0; i < nMaxChild; ++i )
	{
		pChild = pUI->getChildAt(i);
		if( pChild != NULL && pChild->getType() != eUI_CONTROL_TEXT_EX)
		{
			TiXmlElement* pChildRoot  = writeControlProp(pChild);
			
			ChildControl( pChild, pChildRoot );
			if( pRoot && pChildRoot )
				pRoot->LinkEndChild(pChildRoot);
		}
	}
}


void XmlWrite::ChildSpecialControl( CUIBase* pUI, TiXmlElement* pRoot, eUI_CONTROL eType )
{
	int	i, nMaxChild = pUI->getChildCount();

	// 툴에서 리스트나 어레이 생성시 템플릿으로 등록시키지 않기때문에 차일드에서 찾아야 한다.
	bool bTemplate = false;

	CUIBase*	pChild = NULL;

	// 리스트아이템, 어레이 아이템을 제외한 차일드 컨트롤 저장.
	for( i = 0; i < nMaxChild; ++i )
	{
		pChild = pUI->getChildAt(i);

		if (pChild == NULL)
			continue;

		if( pChild->getType() == eType )
		{
			if (bTemplate == false)
				bTemplate = true;
			else
				continue;
		}

		TiXmlElement* pChildRoot  = writeControlProp(pChild);

		ChildControl( pChild, pChildRoot );

		if( pRoot && pChildRoot )
			pRoot->LinkEndChild(pChildRoot);
	}

	// 리스트 아이템이나 어레이 아이템을 찾지 못했다면 템플릿을 가져와서 사용.
	if (bTemplate == false)
	{
		CUIBase* pTmpItem = NULL;

		if (eType == eUI_CONTROL_LIST_ITEM)
		{
			CUIList* pList = dynamic_cast<CUIList*>(pUI);
			pTmpItem = (CUIBase*)pList->GetListItemTemplate();
		}
		else // if (eType == eUI_CONTROL_ARRAY_ITEM)
		{
			CUIArray* pArray = dynamic_cast<CUIArray*>(pUI);
			pTmpItem = (CUIBase*)pArray->GetArrayItemTemplate();
		}

		if (pTmpItem != NULL)
		{
			TiXmlElement* pChildRoot  = writeControlProp(pTmpItem);

			ChildControl( pTmpItem, pChildRoot );

			if( pRoot && pChildRoot )
				pRoot->LinkEndChild(pChildRoot);
		}
	}
}


TiXmlElement* XmlWrite::writeControlProp( CUIBase* pUI )
{
	TiXmlElement* root = NULL;

	switch(pUI->getType())
	{
	case eUI_CONTROL_BASE:
		return writeBase( pUI );
	case eUI_CONTROL_IMAGE:
		return writeImage( pUI );
	case eUI_CONTROL_BUTTON:
		return writeButton( pUI );
	case eUI_CONTROL_TEXT:
		return writeText( pUI );
	case eUI_CONTROL_TEXT_EX:
		break;
	case eUI_CONTROL_TEXTBOX:
		return writeTextBox( pUI );
	case eUI_CONTROL_TEXTBOX_EX:
		return writeTextBoxEx(pUI);
	case eUI_CONTROL_EDIT:
		return writeEditBox( pUI );
	case eUI_CONTROL_SCROLL:
		return writeScrollBar( pUI );
	case eUI_CONTROL_CHECKBOX:
		return writeCheckButton( pUI );
	case eUI_CONTROL_COMBOBOX:
		return writeComboBox( pUI );
	case eUI_CONTROL_SLIDEBAR:
		return writeSlideBar( pUI );
	case eUI_CONTROL_PROGRESSBAR:
		return writeProgressBar( pUI );
	case eUI_CONTROL_LIST:
		return writeList( pUI );
	case eUI_CONTROL_LIST_ITEM:
		return writeListItem( pUI );
	case eUI_CONTROL_TAB:
		return writeTab( pUI );
	case eUI_CONTROL_TABPAGE:
		return writeTabPage( pUI );
	case eUI_CONTROL_ICON:
		return writeIcon( pUI );
	case eUI_CONTROL_SPINBUTTON:
		return writeSpinButton( pUI );
	case eUI_CONTROL_IMAGEFONT:
		return writeImageFont( pUI );
	case eUI_CONTROL_ARRAY:
		return writeArray( pUI );
	case eUI_CONTROL_ARRAY_ITEM:
		return writeArrayItem( pUI );
	case eUI_CONTROL_SPRITEANI:
		return writeSpriteAni( pUI );
	case eUI_CONTROL_IMAGE_ARRAY:
		return writeImageArray( pUI );
	case eUI_CONTROL_IMAGE_SPLIT:
		return writeImageSplit( pUI );
	case eUI_CONTROL_TREE:
		return writeTreecontrol(pUI);
	case eUI_CONTROL_TREEITEM:
		return writeTreeItemcontrol(pUI);
	case eUI_CONTROL_SPIN_CONTROL:
		return writeSpincontrol(pUI);
	}

	return root;
}


bool XmlWrite::setBaseValue( CUIBase* pUI, TiXmlElement* pElement )
{
	if( pUI == NULL && pElement == NULL )
		return false;

	std::string strID = pUI->getID();
	if( !strID.empty() )
		pElement->SetAttribute("id", strID.c_str());

	strID = pUI->getPID();
	if( !strID.empty() )
		pElement->SetAttribute("parent", strID.c_str());

	int x, y;
	pUI->GetOrigPos(x, y);

	AddAttRect(UIRect(x, y, pUI->GetWidth(), pUI->GetHeight()), pElement);

	pElement->SetAttribute("hide", pUI->GetHide() ? 1 : 0);

	std::string strTex = pUI->getTexString();
	if( strTex.empty() == false )
		pElement->SetAttribute("tex", strTex.c_str() );

	int nAlignH = (int)pUI->getAlignH();
	int nAlignV = (int)pUI->getAlignV();

	pElement->SetAttribute("align_h", nAlignH);
	pElement->SetAttribute("align_v", nAlignV);

	int nTooltip = pUI->GetTooltipIndex();
	pElement->SetAttribute("tooltip", nTooltip);

	int nTooltip_width = pUI->getTooltipWidth();
	pElement->SetAttribute("tooltip_width", nTooltip_width);

	return true;
}

bool XmlWrite::AddAttUV( UIRectUV* uv, TiXmlElement* pElement )
{
	pElement->SetAttribute("l", uv->U0);
	pElement->SetAttribute("t", uv->V0);
	pElement->SetAttribute("r", uv->U1);
	pElement->SetAttribute("b", uv->V1);

	return true;
}

bool XmlWrite::AddAttUV( std::string szName, UIRectUV* uv, TiXmlElement* pElement)
{
	if( szName.empty() == TRUE )
		return false;

	TiXmlElement* pChildElement = new TiXmlElement(szName.c_str());

	AddAttUV(uv, pChildElement);
	pElement->LinkEndChild(pChildElement);
	return true;
}


void XmlWrite::AddAttRect( UIRect rc, TiXmlElement* pElement )
{
	pElement->SetAttribute("x", rc.Left);
	pElement->SetAttribute("y", rc.Top);
	pElement->SetAttribute("w", rc.Right);
	pElement->SetAttribute("h", rc.Bottom);
}


void XmlWrite::AddComboBtn( UIRectUV* uv0, UIRectUV* uv1, UIRect rc, TiXmlElement* pElement )
{
	if( !( uv0 || uv1 || pElement ) )
		return;

	AddAttRect(rc, pElement);
	pElement->SetAttribute("l0", uv0->U0);
	pElement->SetAttribute("t0", uv0->V0);
	pElement->SetAttribute("r0", uv0->U1);
	pElement->SetAttribute("b0", uv0->V1);

	pElement->SetAttribute("l1", uv1->U0);
	pElement->SetAttribute("t1", uv1->V0);
	pElement->SetAttribute("r1", uv1->U1);
	pElement->SetAttribute("b1", uv1->V1);
}

const char* XmlWrite::ConvertColorToString( COLOR color, std::string& strbuff )
{
	if( color < 0 )
		return "";

	char szColor[128];
	ltoa(color, szColor, 16);

	strbuff = "0x";
	strbuff += szColor;

	return strbuff.c_str();
}

TiXmlElement* XmlWrite::writeBase( CUIBase* pBase )
{
	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );

	BOOL bRet = setBaseValue(pBase, pElement);

	return pElement;
}

void XmlWrite::writeWindow( CUIBase* pBase )
{
	m_MyWindowElement = new TiXmlElement( str_ui_control[pBase->getType()] );
	CUIWindow* pUI = dynamic_cast<CUIWindow*>(pBase);

	BOOL bRet = setBaseValue(pUI, m_MyWindowElement);

	if( m_MyWindowElement )
		m_MyDoc.LinkEndChild( m_MyWindowElement );

	ASSERT( bRet );
}

TiXmlElement* XmlWrite::writeImage( CUIBase* pBase )
{ 
	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );

	CUIImage* pUI = dynamic_cast<CUIImage*>(pBase);
	BOOL bRet = setBaseValue(pUI, pElement);
	AddAttUV(&pUI->GetAbsUV(), pElement);

	return pElement;
}

TiXmlElement* XmlWrite::writeButton( CUIBase* pBase )
{
	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );
	CUIButton* pUI = dynamic_cast<CUIButton*>(pBase);

	setBaseValue(pUI, pElement);

	int			iIdx	= (int)pUI->getStringIdx();
	std::string strTmp;
	bool bEdge = pUI->GetEdge();

	if( iIdx > 0 )
		strTmp = CUIBase::getText((INDEX)iIdx);
	else
		strTmp = pUI->GetText();

	pElement->SetAttribute( "str_idx", iIdx );

	{
		StackMultiByteToWideChar(CP_ACP, strTmp.c_str(), -1, wcsResult);
		StackWideCharToMultiByte(CP_UTF8, wcsResult, -1, szResult);
		pElement->SetAttribute( "str", szResult );
	}

	pElement->SetAttribute("newtype", pUI->GetNewType() ? 1 : 0);

	pElement->SetAttribute("edge", bEdge ? 1 : 0);

	COLOR textCol = pUI->GetTextColor((UIBtnState)0);
	pElement->SetAttribute("text_color", ConvertColorToString(textCol, strTmp) );

	std::string strImg = pUI->getTexString();
	std::string strPath = "Data\\Interface\\";
	float fTexW = 1.0f, fTexH = 1.0f;
	if( strImg.empty() == false )
	{
		strPath += strImg.c_str();
		CTextureData* pTex = _pTextureStock->Obtain_t( strPath.c_str() );
		fTexW = pTex->GetPixWidth();
		fTexH = pTex->GetPixHeight();
	}

	for (int i = 0; i < UBS_TOTAL; ++i)
	{
		if( pUI->GetNewType() == TRUE )
		{
			UIRect rcSrc, rcTmp;
			UIRectUV uvSrc, uvTmp;
			bool	 bCopy = false;
#ifdef UI_TOOL
			int cnt, max = pUI->GetRectSurface((UIBtnState)i).GetCount();
			for( cnt = 0; cnt < max ; cnt++ )
			{
				pUI->GetRectSurface(UBS_IDLE).GetRectAndUV(rcSrc, uvSrc, cnt);
				pUI->GetRectSurface((UIBtnState)i).GetRectAndUV(rcTmp, uvTmp, cnt);
				if ( (rcSrc == rcTmp && uvSrc == uvTmp) && i != 0)
				{
					if( bCopy == false )
					{
						bCopy = true;
						TiXmlElement* pChildElement = new TiXmlElement("copyrsurf");
						pChildElement->SetAttribute("src", 0);
						pChildElement->SetAttribute("dst", i);
						pElement->LinkEndChild( pChildElement );
					}
				}
				else
				{
					TiXmlElement* pChildElement = new TiXmlElement("RSurface");
					pChildElement->SetAttribute("type", i);
					AddAttRect(rcTmp, pChildElement);
					uvTmp.U0 *= fTexW;
					uvTmp.V0 *= fTexH;
					uvTmp.U1 *= fTexW;
					uvTmp.V1 *= fTexH;
					AddAttUV(&uvTmp, pChildElement);
					pElement->LinkEndChild(pChildElement);
				}
			}
#endif // UI_TOOL
		}
		else
		{
			UIRectUV uvSrc = pUI->GetUV(UBS_IDLE);
			UIRectUV uvTmp = pUI->GetUV((UIBtnState)i);
			if (uvSrc == uvTmp && i != 0)
			{
				// copy uv
				TiXmlElement* pChildElement = new TiXmlElement("copyuv");
				pChildElement->SetAttribute("src", 0);
				pChildElement->SetAttribute("dst", i);
				pElement->LinkEndChild( pChildElement );
			}
			else
			{
				TiXmlElement* pChildElement = new TiXmlElement("uv");
				pChildElement->SetAttribute("type", i);
				AddAttUV( &uvTmp, pChildElement );
				pElement->LinkEndChild( pChildElement );
			}
		}
	}

	return pElement;
}

TiXmlElement* XmlWrite::writeEditBox( CUIBase* pBase )
{
	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );
	CUIEditBox* pUI = dynamic_cast<CUIEditBox*>(pBase);

	setBaseValue(pUI, pElement);

	BOOL bPassWord, bMsgInput, bCursorMove, bOnlyNum;
	pElement->SetAttribute("max", pUI->GetMaxChar());

	pUI->GetPassWord( bPassWord, bMsgInput );
	pElement->SetAttribute("passwd", bPassWord ? 1 : 0 );
	
	bCursorMove = pUI->GetCursorMove();
	pElement->SetAttribute("cursor_move", bCursorMove ? 1 : 0 );

	bOnlyNum = pUI->GetOnlyIntegerMode();
	pElement->SetAttribute("only_num", bOnlyNum ? 1 : 0);

	UIRectUV uvCandi = pUI->getCandidateUV();
	UIRectUV uvReadingWnd = pUI->getReadingWindowUV();

	TiXmlElement* pChild = new TiXmlElement("CandidateUV");
	AddAttUV(&uvCandi, pChild);
	pElement->LinkEndChild(pChild);

	pChild = new TiXmlElement("ReadUV");
	AddAttUV(&uvCandi, pChild);
	pElement->LinkEndChild(pChild);

	pChild = new TiXmlElement("back");
	UIRectUV uvBackGround = pUI->getBackGroundUV();
	UIRect	 rcBackGround = pUI->getBackGroundRect();
	AddAttUV(&uvBackGround, pChild);
	AddAttRect(rcBackGround, pChild);
	pElement->LinkEndChild(pChild);

	return pElement;
}

TiXmlElement* XmlWrite::writeCheckButton( CUIBase* pBase )
{
	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );
	CUICheckButton* pUI = dynamic_cast<CUICheckButton*>(pBase);

	setBaseValue(pUI, pElement);

	int			iIdx	= (int)pUI->getStringIndex();
	BOOL	bEdge = pUI->getEdge() ? TRUE : FALSE;

	std::string strTmp;
	if( iIdx > 0 )
		strTmp = CUIBase::getText((INDEX)iIdx);
	else
		strTmp = pUI->getText();

	pElement->SetAttribute( "str_idx", iIdx );
	{
		StackMultiByteToWideChar(CP_ACP, strTmp.c_str(), -1, wcsResult);
		StackWideCharToMultiByte(CP_UTF8, wcsResult, -1, szResult);
		pElement->SetAttribute( "str", szResult );
	}

	pElement->SetAttribute("posleft", pUI->GetLeft() );
	pElement->SetAttribute("text_x", pUI->GetTextSX() );
	pElement->SetAttribute("region", pUI->GetCheckRegion() );

	int nTextArea = pUI->getTextArea();
	int nAlign	  = pUI->getAlignTextH();

	pElement->SetAttribute("text_area", nTextArea);
	pElement->SetAttribute("h_align", nAlign);

	COLOR colOn, colOff;
	pUI->GetTextColor(colOn, colOff);
	
	pElement->SetAttribute("color_on", ConvertColorToString(colOn, strTmp));
	pElement->SetAttribute("color_off", ConvertColorToString(colOff, strTmp));

	pElement->SetAttribute("edge", bEdge);

	std::string strPath = pUI->getTexString();
	UIRectUV	uv = pUI->GetUV(UCBS_NONE);
	AddAttUV("none",&uv , pElement);
	uv = pUI->GetUV(UCBS_CHECK);
	AddAttUV("check",&uv , pElement);

	return pElement;
}

TiXmlElement* XmlWrite::writeText( CUIBase* pBase )
{
	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );
	CUIText* pUI = dynamic_cast<CUIText*>(pBase);

	setBaseValue(pUI, pElement);
	
	bool  bShadow = pUI->getShadow();
	float fDepth  = pUI->getDepth();
	BOOL bEdge	= pUI->getEdge();

	COLOR col = pUI->getFontColor(), colShadow = pUI->getFontShadow();

	int			iIdx	= (int)pUI->getStringIdx();
	int			nAlign	= pUI->getAlignTextH();

	std::string strTmp;
	if( iIdx >= 0 )
		strTmp = CUIBase::getText((INDEX)iIdx);
	else if( pUI->getText() )
		strTmp = pUI->getText();

	pElement->SetAttribute("str_idx", iIdx );
	{
		StackMultiByteToWideChar(CP_ACP, strTmp.c_str(), -1, wcsResult);
		StackWideCharToMultiByte(CP_UTF8, wcsResult, -1, szResult);
		pElement->SetAttribute("str", szResult );
	}

	pElement->SetAttribute("str_ellipsis", pUI->getEllipsisText());

	pElement->SetAttribute("color", ConvertColorToString(col, strTmp) );

	pElement->SetAttribute("shadow", bShadow );

	pElement->SetAttribute("shadow_color", ConvertColorToString(colShadow, strTmp) );
	//pElement->SetAttribute("depth", fDepth );

	pElement->SetAttribute("h_align", nAlign);

	pElement->SetAttribute("edge", bEdge);

	TiXmlElement* pChild = new TiXmlElement("back");
	UIRectUV uv;
	UIRect	 rc;
	pUI->getBackGround(rc, uv);
	AddAttRect(rc, pChild);
	AddAttUV(&uv, pChild);
	pElement->LinkEndChild(pChild);
	
	return pElement;
}

TiXmlElement* XmlWrite::writeTextBox( CUIBase* pBase )
{
	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );
	CUITextBox* pUI = dynamic_cast<CUITextBox*>(pBase);

	setBaseValue(pUI, pElement);

	int nSX, nSY, nGapY;

	pUI->GetStartXY(nSX, nSY);

	int nH_Align = pUI->GetAlignTextH();

	int strIdx = pUI->getStringIdx();

	int nSplit = pUI->GetSplitMode();

	nGapY = pUI->GetGapY();

	std::string str = pUI->GetString();

	pElement->SetAttribute("sx", nSX);
	pElement->SetAttribute("sy", nSY);
	pElement->SetAttribute("gap_y", nGapY);
	pElement->SetAttribute("h_align", nH_Align);
	pElement->SetAttribute("str_idx", strIdx);
	pElement->SetAttribute("split_mode", nSplit);

	{
		StackMultiByteToWideChar(CP_ACP, str.c_str(), -1, wcsResult);
		StackWideCharToMultiByte(CP_UTF8, wcsResult, -1, szResult);
		COLOR color = pUI->GetTextColor();
		std::string strTmp;
		pElement->SetAttribute("text_col", ConvertColorToString(color, strTmp));
		pElement->SetAttribute("str", szResult);
	}

	TiXmlElement* pChild = new TiXmlElement("back");
	UIRect rc;
	UIRectUV uv;
	pUI->getBackGround(rc, uv);
	AddAttRect(rc, pChild);
	AddAttUV(&uv, pChild);
	pElement->LinkEndChild(pChild);

	return pElement;
}

TiXmlElement* XmlWrite::writeScrollBar( CUIBase* pBase )
{
	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );
	CUIScrollBar* pUI = dynamic_cast<CUIScrollBar*>(pBase);

	setBaseValue(pUI, pElement);

	BOOL bVertical		 = pUI->getVertical();
	int  nScroll_Pos	 = pUI->GetScrollPos();
	BOOL bRight			 = pUI->getRight() ? TRUE : FALSE;
	int  nItemSize		 = pUI->getItemSize();

	pElement->SetAttribute("vertical", bVertical);
	pElement->SetAttribute("right", bRight);
	pElement->SetAttribute("scroll_pos", nScroll_Pos);
	pElement->SetAttribute("move_unit", nItemSize);

	{
		UIRectUV uv0, uv1;
		UIRect	 rc;
		pUI->getBackGround(rc, uv0);
		TiXmlElement* pChild = new TiXmlElement("back");
		AddAttRect(rc, pChild);
		AddAttUV(&uv0, pChild);
		pElement->LinkEndChild(pChild);

		pUI->getScrollBtn(0, rc, uv0, uv1);
		pChild = new TiXmlElement("button1");
		AddAttRect(rc, pChild);
		AddComboBtn(&uv0, &uv1, rc, pChild);
		pElement->LinkEndChild(pChild);

		pUI->getScrollBtn(1, rc, uv0, uv1);
		pChild = new TiXmlElement("button2");
		AddAttRect(rc, pChild);
		AddComboBtn(&uv0, &uv1, rc, pChild);
		pElement->LinkEndChild(pChild);

		int nUnit;
		pUI->getThumb(rc, uv0, nUnit);
		pChild = new TiXmlElement("thumb");
		AddAttRect(rc, pChild);
		pChild->SetAttribute("unit", nUnit);
		AddAttUV(&uv0, pChild);
		pElement->LinkEndChild(pChild);
	}

	// 아이템 갯수 처리를 마지막에. pElement의 Attribute로 셋
	int nItemCount = pUI->GetCurItemCount();
	int nItemPerPage = pUI->GetItemsPerPage();
	pElement->SetAttribute("item_count", nItemCount);
	pElement->SetAttribute("item_per_page", nItemPerPage);

	return pElement;
}

TiXmlElement* XmlWrite::writeComboBox( CUIBase* pBase )
{
	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );
	CUIComboBox* pUI = dynamic_cast<CUIComboBox*>(pBase);

	setBaseValue(pUI, pElement);

	int nMaxLine = pUI->GetMaxLine();
	int	nLineHeight = pUI->getlineHeight();
	int nSpacePosX, nSpacePosY;
	int nShowLine = pUI->getShowLine();
	int nDropAreaUnit = 0;
	UIRect rcBase, rcComboBtn, rcDropArea;
	UIRectUV uvBase, uvComboBtn0, uvComboBtn1, uvDropArea;

	pUI->getSpace(nSpacePosX, nSpacePosY);
	pElement->SetAttribute("maxline", nMaxLine);
	pElement->SetAttribute("lineheight", nLineHeight);
	pElement->SetAttribute("space_x", nSpacePosX);
	pElement->SetAttribute("space_y", nSpacePosY);
	pElement->SetAttribute("showline", nShowLine);

	TiXmlElement* pChild = new TiXmlElement("back");
	pUI->getBackGround(rcBase, uvBase);
	AddAttRect(rcBase, pChild);
	AddAttUV(&uvBase, pChild);
	pElement->LinkEndChild(pChild);

	pUI->getComboButton(rcComboBtn, uvComboBtn0, uvComboBtn1);
	pChild = new TiXmlElement("button");
	AddComboBtn(&uvComboBtn0, &uvComboBtn1, rcComboBtn, pChild);
	pElement->LinkEndChild(pChild);

	pUI->getDropArea(rcDropArea, uvDropArea, nDropAreaUnit);
	TiXmlElement* pDropArea = new TiXmlElement("drop_bg");
	//pDropArea->SetAttribute("unit", nDropAreaUnit);
	AddAttRect(rcDropArea, pDropArea);
	AddAttUV(&uvDropArea, pDropArea);
	pElement->LinkEndChild(pDropArea);
	return pElement;
}

TiXmlElement* XmlWrite::writeSlideBar( CUIBase* pBase )
{
	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );
	CUISlideBar* pUI = dynamic_cast<CUISlideBar*>(pBase);

	setBaseValue(pUI, pElement);

	int nBarW	 = pUI->getBarWidth();
	int nBarH	 = pUI->getBarHeight();
	int nCurPos	 = pUI->GetCurPos();
	int nRange	 = pUI->GetRange();
	int nMin	 = pUI->GetMinPos();
	int nMax	 = pUI->GetMaxPos();

	pElement->SetAttribute("bar_w", nBarW);
	pElement->SetAttribute("bar_h", nBarH);
	pElement->SetAttribute("cur_pos", nCurPos);
	pElement->SetAttribute("range", nRange);
	pElement->SetAttribute("min", nMin);
	pElement->SetAttribute("max", nMax);

	UIRectUV uv[2];
	uv[0] = pUI->getBackUV();
	uv[1] = pUI->getBarUV();

	//TiXmlElement* pChild = new TiXmlElement("back");
	AddAttUV( "back", &uv[0], pElement );
	AddAttUV( "bar", &uv[1], pElement );
	//pChild = new TiXmlElement("bar");
	return pElement;
}

TiXmlElement* XmlWrite::writeProgressBar( CUIBase* pBase )
{	
	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );
	CUIProgressBar* pUI = dynamic_cast<CUIProgressBar*>(pBase);

	setBaseValue(pUI, pElement);

	UIRectUV uv = pUI->getUVBack();
	AddAttUV(&uv, pElement);

	int nLower, nUpper, nStep, nOffsetPos, nAutoComplete;

	pUI->GetProgressRange(nLower, nUpper);
	nStep			= pUI->getStep();
	nOffsetPos		= pUI->GetProgressPos();
	nAutoComplete	= pUI->getAutoComplete() ? 1 : 0;

	pElement->SetAttribute("lower", nLower);
	pElement->SetAttribute("upper", nUpper);
	pElement->SetAttribute("step", nStep);
	pElement->SetAttribute("offset_pos", nOffsetPos);
	pElement->SetAttribute("auto_complete", nAutoComplete);

	TiXmlElement* pBarElement = new TiXmlElement("bar");
	int nSX, nSY;
	pUI->getBarStartPos(nSX, nSY);

	pBarElement->SetAttribute("sx", nSX);
	pBarElement->SetAttribute("sy", nSY);
	uv = pUI->getUVBar();
	AddAttUV( &uv, pBarElement );
	pElement->LinkEndChild(pBarElement);

	COLOR color;
	bool  bShowPercent;
	//char  szColor[128];

	const char* szName[] = {"idle_text", "doing_text", "complete_text"};
	int i;
	for( i = 0 ; i < PR_STAT_TAG_END; i++ )
	{
		TiXmlElement* pChild = new TiXmlElement(szName[i]);

		int			iIdx	= (int)pUI->getStringIdx((_tagProgressStatus)i);

		std::string strTmp;
		if( iIdx >= 0 )
		{
			strTmp = CUIBase::getText((INDEX)iIdx);
			pChild->SetAttribute( "str_idx", iIdx );
		}
		else if( pUI->getProgressText((_tagProgressStatus)i) )
			strTmp = pUI->getProgressText((_tagProgressStatus)i);
		{
			StackMultiByteToWideChar(CP_ACP, strTmp.c_str(), -1, wcsResult);
			StackWideCharToMultiByte(CP_UTF8, wcsResult, -1, szResult);
			pChild->SetAttribute( "str", szResult );
		}

		color			 = pUI->getColProgressText((_tagProgressStatus)i);
		bShowPercent	 = pUI->getShowPercentText((_tagProgressStatus)i) ? true : false;

		pChild->SetAttribute("color", ConvertColorToString(color, strTmp));
		pChild->SetAttribute("show_percent", bShowPercent);
		pElement->LinkEndChild(pChild);
	}

	return pElement;
}

TiXmlElement* XmlWrite::writeList( CUIBase* pBase )
{
	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );
	CUIList* pUI = dynamic_cast<CUIList*>(pBase);

	setBaseValue(pUI, pElement);

	int nItemX, nItemY;
	int nItemShow_Num = pUI->GetItemShowNum();
	int nItemGap	  = pUI->GetGap();
	int nItemCount	  = pUI->GetItemCount();
	bool bPreCreate	  = pUI->GetPreCreate();
	pUI->GetItemStart(nItemX, nItemY);

	pElement->SetAttribute("itemx", nItemX);
	pElement->SetAttribute("itemy", nItemY);
	pElement->SetAttribute("itemshow_num", nItemShow_Num);
	pElement->SetAttribute("itemgap", nItemGap);
	pElement->SetAttribute("item_count", nItemCount);
	pElement->SetAttribute("pre_create", bPreCreate ? 1 : 0 );

	UIRect rc;
	UIRectUV uv;

	pUI->GetEventImg(rc, uv, CUIList::eTYPE_SELECT);
	TiXmlElement* pChild = new TiXmlElement( "select" );
	AddAttRect(rc, pChild);
	AddAttUV(&uv, pChild);
	pElement->LinkEndChild(pChild);

	pUI->GetEventImg(rc, uv, CUIList::eTYPE_OVER);
	pChild = new TiXmlElement( "over" );
	AddAttRect(rc, pChild);
	AddAttUV(&uv, pChild);
	pElement->LinkEndChild(pChild);

	return pElement;
}

TiXmlElement* XmlWrite::writeListItem( CUIBase* pBase )
{
	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );
	CUIListItem* pUI = dynamic_cast<CUIListItem*>(pBase);

	setBaseValue(pUI, pElement);

	return pElement;
}

TiXmlElement* XmlWrite::writeTab( CUIBase* pBase )
{
	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );
	CUITab* pUI = dynamic_cast<CUITab*>(pBase);

	setBaseValue(pUI, pElement);
	return pElement;
}

TiXmlElement* XmlWrite::writeTabPage( CUIBase* pBase )
{
	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );
	CUITabPage* pUI = dynamic_cast<CUITabPage*>(pBase);

	setBaseValue(pUI, pElement);
	return pElement;
}

TiXmlElement* XmlWrite::writeIcon( CUIBase* pBase )
{
	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );
	CUIIcon* pUI = dynamic_cast<CUIIcon*>(pBase);
	setBaseValue(pUI, pElement);

	int			nRow, nCol, nType;
	SBYTE		sbTexID;
	pUI->getInfo(nType, sbTexID, nRow, nCol);

	pElement->SetAttribute("type", nType);
	pElement->SetAttribute("texid", (int)sbTexID);
	pElement->SetAttribute("row", nRow);
	pElement->SetAttribute("col", nCol);

	return pElement;
}

TiXmlElement* XmlWrite::writeSpinButton( CUIBase* pBase )
{
	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );
	CUISpinButton* pUI = dynamic_cast<CUISpinButton*>(pBase);
	setBaseValue(pUI, pElement);

	std::string strTitle = pUI->getTitleText();
	UIRectUV uv = pUI->getUVDataBack();
	bool	bAutoInfo	= pUI->getBtnAutoInfo();
	int		nAdjust		= pUI->getAdjust();
	int		nBtnW, nBtnH, nBtnGap;
	int		nDataBackWidth = pUI->getDataBackWidth();
	UIRect	rcWheel		= pUI->getWheelRect();

	pUI->getSpinButton(nBtnW, nBtnH, nBtnGap);

	if( strTitle.empty() == false )
		pElement->SetAttribute("title", strTitle.c_str());

	AddAttUV(&uv, pElement);

	pElement->SetAttribute("data_back_w", nDataBackWidth);
	pElement->SetAttribute("btn_w", nBtnW);
	pElement->SetAttribute("btn_h", nBtnH);
	pElement->SetAttribute("btn_gap", nBtnGap);
	pElement->SetAttribute("auto_info", bAutoInfo);

	TiXmlElement* pChildElement	= new TiXmlElement( "wheel" );
	AddAttRect(rcWheel, pChildElement);
	pElement->LinkEndChild(pChildElement);

	return pElement;
}

TiXmlElement* XmlWrite::writeImageFont( CUIBase* pBase )
{
	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );
	CUIImageFont* pUI = dynamic_cast<CUIImageFont*>(pBase);
	setBaseValue(pUI, pElement);

	int sx, sy, gap, ogap;
	pUI->getFontSizeGap(sx, sy, gap);
	
	pElement->SetAttribute("fsizex", sx);
	pElement->SetAttribute("fsizey", sy);
	pElement->SetAttribute("fgap", gap);

	ogap = pUI->getOutputGap();
	pElement->SetAttribute("ogap", ogap);

	std::string str = pUI->getOrigString();

	{
	StackMultiByteToWideChar(CP_ACP, str.c_str(), -1, wcsResult);
	StackWideCharToMultiByte(CP_UTF8, wcsResult, -1, szResult);
	pElement->SetAttribute("src", szResult);
	}

	str = pUI->getString();

	{
	StackMultiByteToWideChar(CP_ACP, str.c_str(), -1, wcsResult);
	StackWideCharToMultiByte(CP_UTF8, wcsResult, -1, szResult);
	pElement->SetAttribute("str", szResult);
	}
	
	TiXmlElement* pChild = new TiXmlElement("srcuv");
	UIRectUV uv = pUI->getSourceImageUV();
	AddAttUV(&uv, pChild);
	pElement->LinkEndChild(pChild);

	return pElement;
}

TiXmlElement* XmlWrite::writeArray( CUIBase* pBase )
{
	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );
	CUIArray* pUI = dynamic_cast<CUIArray*>(pBase);

	setBaseValue(pUI, pElement);

	int row, col, sx, sy, gapX, gapY, nShowrow;
	UIRect rc;
	UIRectUV uv;

	pUI->GetRowCol(row, col);
	pUI->GetStartItem(sx, sy);
	pUI->GetGap(gapX, gapY);
	nShowrow = pUI->GetShowRowCount();

	pElement->SetAttribute("row", row);
	pElement->SetAttribute("col", col);
	pElement->SetAttribute("sx", sx);
	pElement->SetAttribute("sy", sy);
	pElement->SetAttribute("gapx", gapX);
	pElement->SetAttribute("gapy", gapY);
	pElement->SetAttribute("show_row", nShowrow);

	pUI->GetMouseEventImage(rc, uv, 0);
	TiXmlElement* pChild = new TiXmlElement( "select" );
	AddAttRect(rc, pChild);
	AddAttUV(&uv, pChild);
	pElement->LinkEndChild(pChild);

	pUI->GetMouseEventImage(rc, uv, 1);
	pChild = new TiXmlElement( "mouse_over" );
	AddAttRect(rc, pChild);
	AddAttUV(&uv, pChild);
	pElement->LinkEndChild(pChild);

	return pElement;
}

TiXmlElement* XmlWrite::writeArrayItem( CUIBase* pBase )
{
	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );
	CUIArrayItem* pUI = dynamic_cast<CUIArrayItem*>(pBase);

	setBaseValue(pUI, pElement);

	return pElement;
}

TiXmlElement* XmlWrite::writeSpriteAni( CUIBase* pBase )
{
	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );
	CUISpriteAni* pUI = dynamic_cast<CUISpriteAni*>(pBase);

	setBaseValue(pUI, pElement);

	int nCurFrame = pUI->GetRenderIdx();

	pElement->SetAttribute("cur", nCurFrame);

	UIRectUV uv;
	int i, nMax = pUI->GetAniCount();


	for (i = 0; i < nMax; ++i)
	{
		pUI->DataAt(i, uv);
	
		AddAttUV("uv", &uv, pElement);
	}

	return pElement;
}

TiXmlElement* XmlWrite::writeImageArray( CUIBase* pBase )
{
 	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );
 	CUIImageArray* pUI = dynamic_cast<CUIImageArray*>(pBase);
 
 	setBaseValue(pUI, pElement);

	int curIdx = pUI->GetRenderIdx();
	std::string strTmp;

	pElement->SetAttribute("render_idx", curIdx);
 
 	UIRect rc;
 	UIRectUV uv;
	std::string desc;
 	CTString strName;

	int i, max = pUI->GetImageCount();

	for (i = 0; i < max; ++i)
	{
		strName.PrintF("surface%d", i);

		desc = "";
		pUI->DataAt(i, rc, uv, desc);
		
		TiXmlElement* pChild = new TiXmlElement(strName);

		AddAttRect(rc, pChild);
		AddAttUV(&uv, pChild);

		StackMultiByteToWideChar(CP_ACP, desc.c_str(), -1, wcsResult);
		StackWideCharToMultiByte(CP_UTF8, wcsResult, -1, szResult);

		pChild->SetAttribute("desc", szResult);

		pElement->LinkEndChild(pChild);
	}

	return pElement;
}

TiXmlElement* XmlWrite::writeImageSplit( CUIBase* pBase )
{
	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );
	CUIImageSplit* pUI = dynamic_cast<CUIImageSplit*>(pBase);

	setBaseValue(pUI, pElement);
	AddAttUV(&pUI->GetAbsUV(), pElement);

	int nUnit = pUI->GetUnit();
	pElement->SetAttribute("unit", nUnit);

	int nSplit = pUI->GetMode();
	pElement->SetAttribute("splitmode", nSplit);

	return pElement;
}

TiXmlElement* XmlWrite::writeSpincontrol( CUIBase* pBase )
{
	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );
	CUISpinControl* pUI = dynamic_cast<CUISpinControl*>(pBase);

	setBaseValue(pUI, pElement);

	int nMin;
	SQUAD llMax;
	pUI->GetMinMax(nMin, llMax);

	pElement->SetAttribute("min", nMin);
	pElement->SetAttribute("max", llMax);

	int nCursor_move = pUI->GetCursorMove();
	pElement->SetAttribute("cursor_move", nCursor_move);

	int nCursor_Sx = pUI->GetCursorSX();
	pElement->SetAttribute("cursor_sx", nCursor_Sx);

	int nOnlyIntegerMode = pUI->GetOnlyIntegerMode();
	pElement->SetAttribute("integer_mode", nOnlyIntegerMode);

	TiXmlElement* pChild = new TiXmlElement("back");

	UIRectUV uv;
	UIRect	 rc;
	int nUnit;

	pUI->GetBackGround(rc, uv, nUnit);
	AddAttUV(&uv, pChild);
	AddAttRect(rc, pChild);
	
	pChild->SetAttribute("unit", nUnit);

	pElement->LinkEndChild(pChild);

	return pElement;
}

TiXmlElement* XmlWrite::writeTreecontrol( CUIBase* pBase )
{
	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );
	CUITree* pUI = dynamic_cast<CUITree*>(pBase);

	setBaseValue(pUI, pElement);

 	int nCount = pUI->GetShow();
 	pElement->SetAttribute("show_num", nCount);
 
 	bool bPre = pUI->GetShow();
 	pElement->SetAttribute("pre", bPre == true ? 1 : 0);

	if (pUI->getCheck() != NULL)
	{
		TiXmlElement* pChildElement = writeCheckButton(pUI->getCheck());
		pElement->LinkEndChild(pChildElement);
	}

	return pElement;
}

TiXmlElement* XmlWrite::writeTreeItemcontrol( CUIBase* pBase )
{
	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );
	CUITreeItem* pUI = dynamic_cast<CUITreeItem*>(pBase);

	setBaseValue(pUI, pElement);

	return pElement;
}

TiXmlElement* XmlWrite::writeTextBoxEx( CUIBase* pBase )
{
	TiXmlElement* pElement = new TiXmlElement( str_ui_control[pBase->getType()] );
	CUITextBoxEx* pUI = dynamic_cast<CUITextBoxEx*>(pBase);

	setBaseValue(pUI, pElement);

	int nSX, nSY, nGapY;

	nSX = pUI->GetStartX();
	nSY = pUI->GetStartY();
	nGapY = pUI->GetGapY();

	int nSplit = pUI->GetSplitMode();

	std::string str = pUI->GetText();

	pElement->SetAttribute("sx", nSX);
	pElement->SetAttribute("sy", nSY);
	pElement->SetAttribute("gap_y", nGapY);
	pElement->SetAttribute("split_mode", nSplit);

	{
		StackMultiByteToWideChar(CP_ACP, str.c_str(), -1, wcsResult);
		StackWideCharToMultiByte(CP_UTF8, wcsResult, -1, szResult);
		COLOR color = pUI->GetBaseColor();
		std::string strTmp;
		pElement->SetAttribute("text_base_col", ConvertColorToString(color, strTmp));
		pElement->SetAttribute("str", szResult);
	}

	return pElement;
}
