#include "StdH.h"
#include <Engine/Interface/UIWindow.h>
#include "UIMsgBoxMgr.h"

#include <Engine/Help/UISupport.h>
#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UIText.h>
#include <Engine/Interface/UITextBox.h>
#include <Engine/Interface/UIImageSplit.h>

CUIMsgBoxICon::CUIMsgBoxICon()
	: _pIcBack(NULL)
	, _pIcItem(NULL)
{
	setInherit(false);
}

CUIBase* CUIMsgBoxICon::Clone()
{
	CUIMsgBoxICon* pMsg = new CUIMsgBoxICon(*this);

	if (pMsg == NULL)
		return NULL;
	
	CUIBase::CloneChild(pMsg);

	return (CUIBase*)pMsg;
}

void CUIMsgBoxICon::initialize()
{
	CUIMsgBox::initialize();

	_pBtnNo = (CUIButton*)findUI("btn_cancle");

	_pIcBack = (CUIIcon*)findUI("icon_bg");
	_pIcItem = (CUIIcon*)findUI("icon_item");
}
