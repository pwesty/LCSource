#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "UITooltipResource.h"

CUITooltipRes::CUITooltipRes()
    : m_pTextBase(NULL)
    , m_pTextBoxBase(NULL)
    , m_pImgNormalBack(NULL)
    , m_pImgStringBack(NULL)
	, m_pImageText(NULL)
{
}

CUITooltipRes::~CUITooltipRes()
{
    Destroy();

    m_pTextBase = NULL;
    m_pTextBoxBase = NULL;
    m_pImgNormalBack = NULL;
    m_pImgStringBack = NULL;
	m_pImageText = NULL;
}

void CUITooltipRes::initialize()
{
    m_pTextBase = (CUIText*)findUI("text");
    m_pTextBoxBase = (CUITextBox*)findUI("text_box");
    m_pImgNormalBack = (CUIImageSplit*)findUI("img_normal_back");
    m_pImgStringBack = (CUIImageSplit*)findUI("img_string_back");
	m_pImageArray	= (CUIImageArray*)findUI("img_array_socket");
	m_pIcon			= (CUIIcon*)findUI("icon");

	m_pImageText = new CUIImageText;
	m_pImageText->SetSize(140, 14);
	addChild(m_pImageText);
}
