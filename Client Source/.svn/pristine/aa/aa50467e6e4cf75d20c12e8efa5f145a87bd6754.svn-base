#include "StdH.h"
#include <Engine/Interface/UIWindow.h>
#include "UIMsgBoxMgr.h"

#include <Engine/Help/UISupport.h>
#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UIText.h>
#include <Engine/Interface/UITextBox.h>
#include <Engine/Interface/UIImageSplit.h>

CUIMsgBoxTime::CUIMsgBoxTime()
	: _remain_time(0)
	, _pTextTime(NULL)
{
	setInherit(false);
}

CUIBase* CUIMsgBoxTime::Clone()
{
	CUIMsgBoxTime* pMsg = new CUIMsgBoxTime(*this);

	if (pMsg == NULL)
		return NULL;
	
	CUIBase::CloneChild(pMsg);

	return (CUIBase*)pMsg;
}

void CUIMsgBoxTime::initialize()
{
	CUIMsgBox::initialize();

	_pBtnNo = (CUIButton*)findUI("btn_cancle");

	_pTextTime = (CUIText*)findUI("text_time");
}
