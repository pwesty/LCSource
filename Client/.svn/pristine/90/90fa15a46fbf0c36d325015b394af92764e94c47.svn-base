#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "UIExpressItemList.h"

#define ITEM_LIST_STRING_OUTPUT_GAP 9

CUIExpressItemList::CUIExpressItemList(void)
	: m_pIcon(NULL)
{
	InitItemList();
}
CUIExpressItemList::~CUIExpressItemList(void)
{
	SAFE_DELETE(m_pIcon);
}

void CUIExpressItemList::SetString( eLCE_ITEM_STRING type, CTString strString )
{
	m_strString[type] = strString;
}

void CUIExpressItemList::SetStringColor( eLCE_ITEM_STRING_COLOR type, COLOR color )
{
	m_color[type] = color;
}

//-------------------------------------------------------------

void CUIExpressItemList::Create( CUIWindow* pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParent = pParentWnd;

	SetPos(nX, nY);
	SetSize(nWidth, nHeight);

	InitItemList();

	m_pIcon = new CUIIcon;
	m_pIcon->Create(this, 0, 0, BTN_SIZE, BTN_SIZE, UI_LCE_SYSTEM, UBET_ITEM);
}

void CUIExpressItemList::Render()
{
	int	nX, nY;
	GetAbsPos( nX, nY );

	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	// 아이템 이름
	pDrawPort->PutTextExCX(m_strString[LCE_STRING_NAME], nX + m_nItemNamePosX,
		nY + m_nItemNamePosY, m_color[LCE_COLOR_NAME]);

	// 아이템 갯수
	pDrawPort->PutTextExCX(m_strString[LCE_STRING_NUM], nX + m_nItemNumPosX,
		nY + m_nItemNumPosY, m_color[LCE_COLOR_NUM]);

	// 나스 금액
	if(m_bNas)
	{
		pDrawPort->PutTextExRX(m_strString[LCE_STRING_AMOUNT], nX + m_nNasAmountPosX,
			nY + m_nAmountPosY, m_color[LCE_COLOR_AMOUNT]);
	}
	else
	{
		pDrawPort->PutTextExCX(m_strString[LCE_STRING_AMOUNT], nX + m_nAmountPosX,
			nY + m_nAmountPosY, m_color[LCE_COLOR_AMOUNT]);
	}

	// 보낸 사람 이름
	pDrawPort->PutTextExCX(m_strString[LCE_STRING_SENDER],	nX + m_nSenderPosX,
		nY + m_nSenderPosY, m_color[LCE_COLOR_SENDER]);

	// 등록 타입
	pDrawPort->PutTextExCX(m_strString[LCE_STRING_TYPE], nX + m_nTypePosX,
		nY + m_nTypePosY, m_color[LCE_COLOR_TYPE]);	

	// 등록 시간
	pDrawPort->PutTextExCX(m_strString[LCE_STRING_DATE_START], nX + m_nStartTimePosX,
		nY + m_nStartTimePosY, m_color[LCE_COLOR_DATE]);

	// 등록 만료 시간
	pDrawPort->PutTextExCX(m_strString[LCE_STRING_DATE_END], nX + m_nEndTimePosX,
		nY + m_nEndTimePosY, m_color[LCE_COLOR_DATE]);

	pDrawPort->EndTextEx();

	// 아이템 아이콘 정보 (툴팁정보 포함)
	m_pIcon->Render(pDrawPort);

	pDrawPort->FlushRenderingQueue();
}

//--------------------------------------------------------------
// Msg Proc

WMSG_RESULT CUIExpressItemList::MouseMessage(MSG* pMsg)
{
	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD(pMsg->lParam);
	int	nY = HIWORD(pMsg->lParam);

	// Extended button clicked
	static BOOL	bLButtonDownInBtn = FALSE;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch (pMsg->message)
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				pUIManager->SetMouseCursorInsideUIs();

				m_pIcon->MouseMessage( pMsg );
				
				return WMSG_SUCCESS;			
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{			
			
				if (m_pIcon->MouseMessage(pMsg) != WMSG_FAIL)
				{					// 선택
					pUIManager->RearrangeOrder( UI_LCE_SYSTEM, TRUE );
					return WMSG_SUCCESS;
				}
				pUIManager->RearrangeOrder( UI_LCE_SYSTEM, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			if( IsInside( nX, nY ) )
			{		
				if (pUIManager->GetDragIcon() == NULL)
				{
					if (m_pIcon->IsEmpty() == true)
						return WMSG_FAIL;

					if (m_pIcon->MouseMessage(pMsg) != WMSG_FAIL)
					{	
						return WMSG_SUCCESS;
					}							

					return WMSG_SUCCESS;
				}
				return WMSG_FAIL;
				pUIManager->ResetHoldBtn();
			}
		}
		break;
	}

	return WMSG_FAIL;
}

//--------------------------------------------------------------
// ReSet

void CUIExpressItemList::InitItemList()
{
	if (m_pIcon != NULL)
		m_pIcon->clearIconData();

	_StringPosInit();
	_ListColorInit();
	_ListStringInit();
	m_bNas = false;
}

void CUIExpressItemList::_StringPosInit()
{
	m_nItemNamePosX = COLUM_START_NAME_GAP + ((CULUM_X_WIDTH_ITME_NAME - COLUM_START_NAME_GAP) / 2);
	m_nItemNamePosY = ITEM_LIST_STRING_OUTPUT_GAP;

	m_nItemNumPosX = LCE_UI_ITEMLIST_COLUM_QUANTITY + (CULUM_X_WIDTH_QUANTITY / 2);
	m_nItemNumPosY = ITEM_LIST_STRING_OUTPUT_GAP;

	m_nAmountPosX = LCE_UI_ITEMLIST_COLUM_AMOUNT + (CULUM_X_WIDTH_AMOUNT / 2);
	m_nNasAmountPosX = LCE_UI_ITEMLIST_COLUM_AMOUNT + CULUM_X_WIDTH_AMOUNT - COLUM_X_GAP;
	m_nAmountPosY = ITEM_LIST_STRING_OUTPUT_GAP;

	m_nSenderPosX = LCE_UI_ITEMLIST_COLUM_SENDER + (CULUM_X_WIDTH_SENDER / 2);
	m_nSenderPosY = ITEM_LIST_STRING_OUTPUT_GAP;

	m_nTypePosX = LCE_UI_ITEMLIST_COLUM_TYPE + (CULUM_X_WIDTH_TYPE / 2);
	m_nTypePosY = ITEM_LIST_STRING_OUTPUT_GAP;

	m_nStartTimePosX = LCE_UI_ITEMLIST_COLUM_TIME + (CULUM_X_WIDTH_TIME / 2);
	m_nStartTimePosY = 1;
	m_nEndTimePosX = LCE_UI_ITEMLIST_COLUM_TIME + (CULUM_X_WIDTH_TIME / 2);
	m_nEndTimePosY= 17;
}

void CUIExpressItemList::_ListColorInit()
{
	for ( int i = 0; i < LCE_COLOR_MAX; i++ )
	{
		m_color[i] = 0xFFFFFFFF;
	}
}

void CUIExpressItemList::_ListStringInit()
{
	for ( int i = 0; i < LCE_STRING_MAX; i++ )
	{
		m_strString[i] = CTString("");
	}
}

