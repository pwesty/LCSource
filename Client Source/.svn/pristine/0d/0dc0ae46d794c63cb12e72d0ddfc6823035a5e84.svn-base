#include "StdH.h"
#include <Engine/Interface/UIWindow.h>
#include "UIMsgBoxMgr.h"

#include <Engine/Help/UISupport.h>
#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UIText.h>
#include <Engine/Interface/UITextBox.h>
#include <Engine/Interface/UIImageSplit.h>
#include <Engine/Interface/UIEditBox.h>

CUIMsgBoxInput::CUIMsgBoxInput()
	: _pEdInput(NULL)
{
	setInherit(false);
}

CUIBase* CUIMsgBoxInput::Clone()
{
	CUIMsgBoxInput* pMsg = new CUIMsgBoxInput(*this);

	if (pMsg == NULL)
		return NULL;
	
	CUIBase::CloneChild(pMsg);

	return (CUIBase*)pMsg;
}

void CUIMsgBoxInput::initialize()
{
	CUIMsgBox::initialize();
	_pBtnNo = (CUIButton*)findUI("btn_cancle");
	_pEdInput = (CUIEditBox*)findUI("eb_input");
}

void CUIMsgBoxInput::SetInputBox( int maxChar, char* strPriveiw /*= NULL*/ )
{
	if (_pEdInput != NULL)
	{
		_pEdInput->ResetShowCount();
		_pEdInput->SetEnable( TRUE );
		_pEdInput->SetFocus( TRUE );
		_pEdInput->SetMaxChar( maxChar );

		if (strPriveiw != NULL)
			_pEdInput->SetString(strPriveiw);
	}
}

CTString CUIMsgBoxInput::GetInputString()
{
	if (_pEdInput != NULL)
	{
		return CTString(_pEdInput->GetString());
	}

	return CTString("");
}

BOOL CUIMsgBoxInput::IsEditBoxFocused()
{
	if (_pEdInput != NULL)
	{
		return _pEdInput->IsFocused();
	}

	return FALSE;
}


WMSG_RESULT CUIMsgBoxInput::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (_pEdInput != NULL)
	{
		_pEdInput->SetFocus(TRUE);
	}

	return WMSG_FAIL;
}
