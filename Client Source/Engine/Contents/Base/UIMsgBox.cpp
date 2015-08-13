#include "StdH.h"
#include <Engine/Interface/UIWindow.h>
#include "UIMsgBoxMgr.h"

#include <Engine/Help/UISupport.h>
#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UIText.h>
#include <Engine/Interface/UITextBox.h>
#include <Engine/Interface/UIImageSplit.h>

CUIMsgBox::CUIMsgBox()
	: _pBtnOk(NULL)
	, _pBtnClose(NULL)
	, _pTextTitle(NULL)
	, _pTbString(NULL)
	, _pimgTitle(NULL)
	, _pimgBack(NULL)
	, _pBtnNo(NULL)
{
	setInherit(false);
}

CUIBase* CUIMsgBox::Clone()
{
	CUIMsgBox* pMsg = new CUIMsgBox(*this);

	if (pMsg == NULL)
		return NULL;
	
	CUIBase::CloneChild(pMsg);

	return (CUIBase*)pMsg;
}

void CUIMsgBox::initialize()
{
	_pBtnOk = (CUIButton*)findUI("btn_ok");
	_pBtnClose = (CUIButton*)findUI("btn_close");
	_pTextTitle = (CUIText*)findUI("text_title");
	_pTbString = (CUITextBox*)findUI("tb_string");
	_pimgTitle = (CUIImageSplit*)findUI("is_title");
	_pimgBack = (CUIImageSplit*)findUI("is_back");
}

void CUIMsgBox::Open()
{
	SetVisible(TRUE);
	Hide(FALSE);
}

void CUIMsgBox::Close()
{
	SetVisible(FALSE);
	Hide(TRUE);
	CUIBase::CloseProc();
}

void CUIMsgBox::SetTitle( const char* strTitle )
{
	if (_pTextTitle != NULL)
		_pTextTitle->SetText(CTString(strTitle));
}

void CUIMsgBox::SetString( const char* strContent, COLOR color )
{
	if (_pTbString != NULL)
		_pTbString->SetText(CTString(strContent), color);
}

void CUIMsgBox::SetOkCommand( Command* pCmd )
{
	if (_pBtnOk != NULL)
		_pBtnOk->SetCommandUp(pCmd);
	else
	{
		if (pCmd != NULL)
			SAFE_DELETE(pCmd);
	}
}

void CUIMsgBox::SetCloseCommand( Command* pCmd )
{
	if (_pBtnClose != NULL)
		_pBtnClose->SetCommandUp(pCmd);
	else
	{
		if (pCmd != NULL)
			SAFE_DELETE(pCmd);
	}
}

void CUIMsgBox::SetNoCommand( Command* pCmd )
{
	if (_pBtnNo != NULL)
		_pBtnNo->SetCommandUp(pCmd);
	else
	{
		if (pCmd != NULL)
			SAFE_DELETE(pCmd);
	}
}

