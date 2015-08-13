#pragma once

#ifndef XMLWRITE_H_
#define XMLWRITE_H_

#include <Engine/tinyxml/tinyxml.h>
class CUIBase;

class XmlWrite
{
public:
	XmlWrite(void);
	~XmlWrite(void);

public:
	void WriteXml( CUIBase* pUI, char* szFileName );

private:
	void			Rearrange4Write();
	CUIBase*		RearrangeFind(eUI_CONTROL eType, CUIBase* pUI);
	CUIBase*		RearrangeFindChild(eUI_CONTROL eType, CUIBase* pUI);

	void			ChildControl( CUIBase* pUI, TiXmlElement* pRoot );
	void			ChildSpecialControl( CUIBase* pUI, TiXmlElement* pRoot, eUI_CONTROL eType );
	TiXmlElement*	writeControlProp( CUIBase* pUI );

	bool			setBaseValue( CUIBase* pUI, TiXmlElement* pElement );
	// 속성에 uv 값 추가
	bool			AddAttUV( UIRectUV* uv, TiXmlElement* pElement ); 
	bool			AddAttUV( std::string szName, UIRectUV* uv ,TiXmlElement* pElement);
	void			AddAttRect( UIRect	rc, TiXmlElement* pElement );
	void			AddComboBtn( UIRectUV* uv0, UIRectUV* uv1, UIRect rc, TiXmlElement* pElement);
	const char*		ConvertColorToString( COLOR color, std::string& strbuff );

	void			writeWindow( CUIBase* pBase );
	TiXmlElement*	writeBase( CUIBase* pBase );
	TiXmlElement*	writeImage( CUIBase* pBase );
	TiXmlElement*	writeButton( CUIBase* pBase );
	TiXmlElement*	writeEditBox( CUIBase* pBase );
	TiXmlElement*	writeCheckButton( CUIBase* pBase );
	TiXmlElement*	writeText( CUIBase* pBase );
	TiXmlElement*	writeComboBox( CUIBase* pBase );
	TiXmlElement*	writeSlideBar( CUIBase* pBase );
	TiXmlElement*	writeProgressBar( CUIBase* pBase );
	TiXmlElement*	writeList( CUIBase* pBase );
	TiXmlElement*	writeListItem(CUIBase* pBase );
	TiXmlElement*	writeTab( CUIBase* pBase ); 
	TiXmlElement*	writeTabPage( CUIBase* pBase );
	TiXmlElement*	writeScrollBar( CUIBase* pBase );
	TiXmlElement*	writeSpinButton( CUIBase* pBase );
	TiXmlElement*	writeIcon( CUIBase* pBase );
	TiXmlElement*	writeImageFont( CUIBase* pBase );
	TiXmlElement*	writeArray( CUIBase* pBase );
	TiXmlElement*	writeArrayItem( CUIBase* pBase );
	TiXmlElement*	writeTextBox( CUIBase* pBase );
	TiXmlElement*	writeSpriteAni( CUIBase* pBase );
	TiXmlElement*	writeImageArray( CUIBase* pBase );
	TiXmlElement*	writeImageSplit( CUIBase* pBase );
	TiXmlElement*	writeSpincontrol( CUIBase* pBase );
	TiXmlElement*	writeTreecontrol( CUIBase* pBase );
	TiXmlElement*	writeTreeItemcontrol( CUIBase* pBase );
	TiXmlElement*	writeTextBoxEx( CUIBase* pBase );
	TiXmlDocument m_MyDoc;
	TiXmlElement* m_MyWindowElement;

};
#endif // XMLWRITE_H_