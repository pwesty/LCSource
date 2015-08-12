#include "StdH.h"
#include <Engine/Interface/UIWindow.h>
#include "UIMsgBoxMgr.h"

#include <Engine/Help/UISupport.h>
#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UIText.h>
#include <Engine/Interface/UITextBox.h>
#include <Engine/Interface/UIImageSplit.h>

CUIMsgBoxYesNo::CUIMsgBoxYesNo()
{
	setInherit(false);
}

CUIBase* CUIMsgBoxYesNo::Clone()
{
	CUIMsgBoxYesNo* pMsg = new CUIMsgBoxYesNo(*this);

	if (pMsg == NULL)
		return NULL;
	
	CUIBase::CloneChild(pMsg);

	return (CUIBase*)pMsg;
}

void CUIMsgBoxYesNo::initialize()
{
	CUIMsgBox::initialize();
	_pBtnNo = (CUIButton*)findUI("btn_cancle");
}