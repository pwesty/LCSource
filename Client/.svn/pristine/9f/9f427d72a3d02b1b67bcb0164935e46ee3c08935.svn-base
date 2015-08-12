#include "StdH.h"

#include <Engine/Interface/UIImageText.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UITooltipMgr.h>
#include <Engine/Interface/UITooltipResource.h>

CUIImageText::CUIImageText()
{
}

CUIBase* CUIImageText::Clone()
{
	CUIImageText* pImageText = NULL;

	pImageText = new CUIImageText(*this);

	if (pImageText == NULL)
		return NULL;

	pImageText->setTexString( getTexString() );
	

	CUIBase::CloneChild(pImageText);

	return (CUIBase*)pImageText;
}

void CUIImageText::initialize()
{

}

void CUIImageText::SetSocket( std::string str,  COLOR col /*= 0xF2F2F2FF*/, int nImageType /*= -1*/, bool bImgLeft /*= true*/ )
{
	CUIImageArray*	pImgArray = NULL;
	CUITextBox*		pTextBox  = NULL;

	pTextBox = (CUITextBox*)TOOLTIPMGR()->GetTooltipRes()->CloneTextBox();
	pImgArray = (CUIImageArray*)TOOLTIPMGR()->GetTooltipRes()->CloneImageArray();

	if (pImgArray == NULL || pTextBox == NULL)
		return;

	if (nImageType >= 0)
		pImgArray->SetRenderIdx(nImageType);

	UpdatePos(pTextBox, (CUIBase*)pImgArray, str, col, bImgLeft);
}

void CUIImageText::SetFortune( std::string str, int nSkillIndex, COLOR col /*= 0xF2F2F2FF*/, bool bImgLeft /*= true*/ )
{
	CUIIcon*		pIcon = NULL;
	CUITextBox*		pTextBox  = NULL;

	pTextBox = (CUITextBox*)TOOLTIPMGR()->GetTooltipRes()->CloneTextBox();
	pIcon = (CUIIcon*)TOOLTIPMGR()->GetTooltipRes()->CloneIcon();

	if (pIcon == NULL || pTextBox == NULL)
		return;

	pIcon->setData(UBET_SKILL, nSkillIndex);

	UpdatePos(pTextBox, (CUIBase*)pIcon, str, col, bImgLeft);
}

void CUIImageText::UpdatePos( CUITextBox* pTextBox, CUIBase* pImg, std::string str, COLOR col, bool bLeft )
{
	if (pTextBox == NULL || pImg == NULL)
		return;

	if (bLeft == true)
	{
		pImg->SetPos(GetPosX(), GetPosY());
		pTextBox->SetPos(pImg->GetWidth() + 3, GetPosY());
		pTextBox->SetWidth(GetWidth() - pTextBox->GetPosX());
	}
	else
	{
		pTextBox->SetPos(GetPosX(), GetPosY());
		pImg->SetPos(pTextBox->GetWidth() + 3, GetPosY());
		pTextBox->SetWidth(pImg->GetPosX() - pTextBox->GetPosX());
	}

	int nImgWidth = pImg->GetWidth();

	pTextBox->SetText(CTString(str.c_str()), col, TRUE);

	int nHeight = pTextBox->GetTextTotalHeight();
	pTextBox->SetHeight(nHeight);

	int nTextH = pTextBox->GetHeight();
	int nImgH  = pImg->GetHeight();

	int nH = nTextH > nImgH ? nTextH : nImgH;

	if (nTextH > nImgH)
		pImg->SetPosY((nH / 2) - (nImgH / 2));
	else
		pTextBox->SetPosY((nH / 2) - (nTextH / 2));


	SetSize(GetWidth(), nH);

	addChild((CUIBase*)pTextBox);
	addChild(pImg);
}
