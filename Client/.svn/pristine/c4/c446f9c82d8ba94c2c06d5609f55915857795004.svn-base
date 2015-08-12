#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIProcessNPC.h>
#include <Engine/Interface/UISkillLearn.h>
#include <Engine/Help/ItemHelp.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIProduct.h>


// Date : 2005-03-07,   By Lee Ki-hwan
static int	_iMaxMsgStringChar = 0;

enum eSelProcessNPC
{
	SEL_MINERAL = 0,	// 광석 정련
	SEL_CROP,			// 식물 가공
	SEL_ENERGY,			// 원소 정제
};

enum eSelMineral
{
	SEL_MINERAL_1 = 0,	// 스톤 온
	SEL_MINERAL_2,		// 스톤 리스
	SEL_MINERAL_3,		// 스톤 웨버
	SEL_MINERAL_4,		// 스톤 비스트
	SEL_MINERAL_5,		// 스톤 웰스던
};

enum eSelEnergy
{
	SEL_ENERGY_1 = 0,	// E
	SEL_ENERGY_2,		// D
	SEL_ENERGY_3,		// C
	SEL_ENERGY_4,		// B
	SEL_ENERGY_5,		// A
};

enum eSelCrop
{
	SEL_CROP_1 = 0,		// 노란잎
	SEL_CROP_2,			// 줄기
	SEL_CROP_3,			// 파란잎
	SEL_CROP_4,			// 가시
	SEL_CROP_5,			// 꽃
};

// ----------------------------------------------------------------------------
// Name : CUIProcessNPC()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIProcessNPC::CUIProcessNPC()
{
	Clear ();
}

// ----------------------------------------------------------------------------
// Name : ~CUIProcessNPC()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIProcessNPC::~CUIProcessNPC()
{
	Destroy();
	clearVecBtnEX();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::Create(CUIWindow* pParentWnd, int nX, int nY, int nWidth, int nHeight)
{
    int diff = SKILLLEARN_HEIGHT - PROCESSNPC_HEIGHT;

    SetPos(nX, nY);
    SetSize(nWidth, nHeight);

    _iMaxMsgStringChar = PROCESSNPC_STRING_CHAR_WIDTH / (_pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing());

    // Region of each part
    m_rcTitle.SetRect(0, 0, 216, 22);
    m_rcIcons.SetRect(17, 55, 51, 200);
    //m_rcTab.SetRect( 21, 31, 204, 50 );

    m_rcItem.SetRect(13, 33, 218, 203);
    m_rcDesc.SetRect(13, 203, 218, 331);

    // ProcessNPC Texture 생성 - SkillLearn Texture 사용
    m_ptdBaseTexture = CreateTexture(CTString("Data\\Interface\\SkillLearn.tex"));
    FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
    FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

    // UV Coordinate of each part
    m_rtBackgroundTop.SetUV(0, 0, 216, PROCESSNPC_TOP_HEIGHT, fTexWidth, fTexHeight);
    m_rtBackgroundBtm.SetUV(0, PROCESSNPC_TOP_HEIGHT + diff, 216, SKILLLEARN_HEIGHT, fTexWidth, fTexHeight);

    m_rtSelOutline.SetUV(218, 80, 250, 112, fTexWidth, fTexHeight);
    m_rtInput.SetUV(218, 136, 248, 149, fTexWidth, fTexHeight);

    // Close button
    m_btnClose.Create(this, CTString(""), 184, 4, 14, 14);
    m_btnClose.SetUV(UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight);
    m_btnClose.SetUV(UBS_CLICK, 219, 15, 233, 29, fTexWidth, fTexHeight);
    m_btnClose.CopyUV(UBS_IDLE, UBS_ON);
    m_btnClose.CopyUV(UBS_IDLE, UBS_DISABLE);

    // ProcessNPC button
    m_btnOK.Create(this, _S(191, "확인"), 70, 372 - diff, 63, 21);
    m_btnOK.SetUV(UBS_IDLE, 0, 403, 63, 424, fTexWidth, fTexHeight);
    m_btnOK.SetUV(UBS_CLICK, 66, 403, 129, 424, fTexWidth, fTexHeight);
    m_btnOK.CopyUV(UBS_IDLE, UBS_ON);
    m_btnOK.CopyUV(UBS_IDLE, UBS_DISABLE);

    // Cancel button
    m_btnCancel.Create(this, _S(139, "취소"), 141, 372 - diff, 63, 21);
    m_btnCancel.SetUV(UBS_IDLE, 0, 403, 63, 424, fTexWidth, fTexHeight);
    m_btnCancel.SetUV(UBS_CLICK, 66, 403, 129, 424, fTexWidth, fTexHeight);
    m_btnCancel.CopyUV(UBS_IDLE, UBS_ON);
    m_btnCancel.CopyUV(UBS_IDLE, UBS_DISABLE);

    // Up button
    m_btnUP.Create(this, CTString(""), 141, 372 - diff, 7, 7);
    m_btnUP.SetUV(UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight);
    m_btnUP.SetUV(UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight);
    m_btnUP.CopyUV(UBS_IDLE, UBS_ON);
    m_btnUP.CopyUV(UBS_IDLE, UBS_DISABLE);
    m_btnUP.SetEnable(FALSE);

    // Down button
    m_btnDown.Create(this, CTString(""), 141, 372 - diff, 7, 7);
    m_btnDown.SetUV(UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight);
    m_btnDown.SetUV(UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight);
    m_btnDown.CopyUV(UBS_IDLE, UBS_ON);
    m_btnDown.CopyUV(UBS_IDLE, UBS_DISABLE);
    m_btnDown.SetEnable(FALSE);

    // Scroll bar of ProcessNPC Item
    m_sbProcessNPCItem.Create(this, 193, 51, 9, 153);
    m_sbProcessNPCItem.CreateButtons(TRUE, 9, 7, 0, 0, 10);
    m_sbProcessNPCItem.SetScrollPos(0);
    m_sbProcessNPCItem.SetScrollRange(PROCESSNPC_SLOT_ROW_TOTAL);
    m_sbProcessNPCItem.SetCurItemCount(0);
    m_sbProcessNPCItem.SetItemsPerPage(PROCESSNPC_SLOT_ROW);
    // Up button
    m_sbProcessNPCItem.SetUpUV(UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight);
    m_sbProcessNPCItem.SetUpUV(UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight);
    m_sbProcessNPCItem.CopyUpUV(UBS_IDLE, UBS_ON);
    m_sbProcessNPCItem.CopyUpUV(UBS_IDLE, UBS_DISABLE);
    // Bar button
    m_sbProcessNPCItem.SetBarTopUV(219, 32, 228, 42, fTexWidth, fTexHeight);
    m_sbProcessNPCItem.SetBarMiddleUV(219, 43, 228, 45, fTexWidth, fTexHeight);
    m_sbProcessNPCItem.SetBarBottomUV(219, 46, 228, 56, fTexWidth, fTexHeight);
    // Down button
    m_sbProcessNPCItem.SetDownUV(UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight);
    m_sbProcessNPCItem.SetDownUV(UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight);
    m_sbProcessNPCItem.CopyDownUV(UBS_IDLE, UBS_ON);
    m_sbProcessNPCItem.CopyDownUV(UBS_IDLE, UBS_DISABLE);
    // Wheel region
    m_sbProcessNPCItem.SetWheelRect(-181, -1, 180, 154);


    // List box of Precondition description
    m_lbPreconditionDesc.Create(this, 13, 207, 180, 124, _pUIFontTexMgr->GetLineHeight(), 7, 6, 1, FALSE);
    m_lbPreconditionDesc.CreateScroll(TRUE, 0, 0, 9, 124, 9, 7, 0, 0, 10);

    // Up button
    m_lbPreconditionDesc.SetScrollUpUV(UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight);
    m_lbPreconditionDesc.SetScrollUpUV(UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight);
    m_lbPreconditionDesc.CopyScrollUpUV(UBS_IDLE, UBS_ON);
    m_lbPreconditionDesc.CopyScrollUpUV(UBS_IDLE, UBS_DISABLE);
    // Bar button
    m_lbPreconditionDesc.SetScrollBarTopUV(219, 32, 228, 42, fTexWidth, fTexHeight);
    m_lbPreconditionDesc.SetScrollBarMiddleUV(219, 43, 228, 45, fTexWidth, fTexHeight);
    m_lbPreconditionDesc.SetScrollBarBottomUV(219, 46, 228, 56, fTexWidth, fTexHeight);
    // Down button
    m_lbPreconditionDesc.SetScrollDownUV(UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight);
    m_lbPreconditionDesc.SetScrollDownUV(UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight);
    m_lbPreconditionDesc.CopyScrollDownUV(UBS_IDLE, UBS_ON);
    m_lbPreconditionDesc.CopyScrollDownUV(UBS_IDLE, UBS_DISABLE);

    m_bSatisfied = FALSE;
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::ResetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ)
{
    SetPos((pixMaxI + pixMinI - GetWidth()) / 2, (pixMaxJ + pixMinJ - GetHeight()) / 2);
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ)
{
    if (m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
        m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ)
        ResetPosition(pixMinI, pixMinJ, pixMaxI, pixMaxJ);
}

// ----------------------------------------------------------------------------
// Name : Clear()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::Clear()
{
    m_iResourceType			= -1;
    m_iResourceSubType		= -1;
    m_nProcessItemCount		= 1;

    m_nSelectProcessNPCItem	= -1;
    m_bWaitProcessNPCResult	= TRUE;
    m_bSatisfied			= FALSE;

    m_nNeedItemCount		= 0;

    memset(m_NeedItems, 0, sizeof(CNeedItems) * MAX_MAKE_ITEM_MATERIAL);

    //m_StrProcessNPCType.Clear ();

    m_bWaitProcessNPCResult = FALSE;
    m_lbPreconditionDesc.ResetAllStrings();

    clearVecBtnEX();
}

// ----------------------------------------------------------------------------
// Name : InitProcessNPC()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::InitProcessNPC()
{
    clearVecBtnEX();

    // 임시 처리
    int nProcessType = m_iResourceType;
    int nProcessSubType = m_iResourceSubType;

    // 제조  Type 설정
    //m_StrProcessNPCType = szItemName;

    CUIIcon* pIcon;
    int bit = 0x00000001;

    //!! 제조 문서로 제조 할수 있는 아이템 검색
    CItemData::_map::iterator	iter = CItemData::_mapdata.begin();
    CItemData::_map::iterator	eiter = CItemData::_mapdata.end();

    for (; iter != eiter; ++iter)
    {
        CItemData* pItemData = (*iter).second;

        if (pItemData == NULL)
            continue;

        if (pItemData->GetFlag() & ITEM_FLAG_MADE)
        {
            if (pItemData->GetType() == CItemData::ITEM_ETC &&
                pItemData->GetSubType() == CItemData::ITEM_ETC_PROCESS &&  // 가공품 이라는 얘기
                pItemData->GetProcessType() == nProcessType
                && pItemData->GetProcessSubType() == nProcessSubType
               )
            {
                pIcon = new CUIIcon;
                pIcon->Create(this, 0, 0, BTN_SIZE, BTN_SIZE, UI_PROCESSNPC, UBET_ITEM);
                pIcon->setData(UBET_ITEM, pItemData->GetItemIndex());
                m_vecIcons.push_back(pIcon);
            }
        }
    }

    AddString(_S(561, "가공품을 선택해 주십시오."), COLOR_TEXT);

    m_sbProcessNPCItem.SetScrollPos(0);
    m_sbProcessNPCItem.SetCurItemCount(m_vecIcons.size());

    m_btnOK.SetEnable(m_bSatisfied);
}

// ----------------------------------------------------------------------------
// Name : OpenProcessNPC()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::OpenProcessNPC()
{
    CUIManager* pUIManager = CUIManager::getSingleton();

    pUIManager->CloseMessageBoxL(MSGLCMD_PROCESSNPC_REQ);

    pUIManager->CreateMessageBoxL(_S(1786, "가공 NPC") , UI_PROCESSNPC, MSGLCMD_PROCESSNPC_REQ);

    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_REQ, TRUE, _S(1787, "무엇을 가공하러 왔는가?"), -1, 0xA3A1A3FF);
    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_REQ, TRUE, _S(1788, "어떤 생산품이든 맡겨만 주면 내가 완벽하게 가공해 주겠네. 물론 나도 먹고 살아야 하니 무료로는 힘들어."), -1, 0xA3A1A3FF);
    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_REQ, TRUE, _S(1789, "수수료를 주는 것이 싫다면 어쩔 수 없지만 이 근처에서 나만큼 뛰어난 가공기술을 가지고 있는 사람은 보질 못했어.  잘 생각해 보라구."), -1, 0xA3A1A3FF);
    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_REQ, FALSE, _S(1790, "광석 정련"), SEL_MINERAL);
    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_REQ, FALSE, _S(1791, "식물 가공"), SEL_CROP);
    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_REQ, FALSE, _S(1792, "원소 정제"), SEL_ENERGY);
    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_REQ, FALSE, _S(1220, "취소한다."));
}

// ----------------------------------------------------------------------------
// Name : OpenProcessList()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::OpenProcessList(int iType, int iSubType)
{
    // If this is already exist
    if (IsVisible() == TRUE)
        return;

    CUIManager* pUIManager = CUIManager::getSingleton();

    // If inventory is already locked
    if (pUIManager->GetInventory()->IsLocked())
    {
        pUIManager->GetInventory()->ShowLockErrorMessage();
        return;
    }

    Clear();

    if (!pUIManager->GetInventory()->IsVisible())
        pUIManager->GetInventory()->ToggleVisible();

    //m_nProcessNPCText	= nItemIndex;
    m_iResourceType = iType;
    m_iResourceSubType = iSubType;

    pUIManager->RearrangeOrder(UI_PROCESSNPC, TRUE);

    InitProcessNPC();
}

// ----------------------------------------------------------------------------
// Name : CloseProcessNPC()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::CloseProcessNPC()
{
    // Close refine
    CUIManager::getSingleton()->RearrangeOrder(UI_PROCESSNPC, FALSE);

    Clear();
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::Render()
{
    CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

    // Set skill learn texture
    pDrawPort->InitTextureData(m_ptdBaseTexture);

    // Add render regions -----------------------------------------------------------------------------------------------
    // Background up
    pDrawPort->AddTexture(m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + PROCESSNPC_TOP_HEIGHT,
                          m_rtBackgroundTop.U0, m_rtBackgroundTop.V0, m_rtBackgroundTop.U1, m_rtBackgroundTop.V1,
                          0xFFFFFFFF);

    // Background down
    pDrawPort->AddTexture(m_nPosX, m_nPosY + PROCESSNPC_TOP_HEIGHT, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
                          m_rtBackgroundBtm.U0, m_rtBackgroundBtm.V0, m_rtBackgroundBtm.U1, m_rtBackgroundBtm.V1,
                          0xFFFFFFFF);

    // Render Title Text
    pDrawPort->PutTextEx(_S(560, "가공") , m_nPosX + PROCESSNPC_TITLE_TEXT_OFFSETX,
                         m_nPosY + PROCESSNPC_TITLE_TEXT_OFFSETY);

    // Render ProcessNPC Type
    //pDrawPort->PutTextExCX( m_StrProcessNPCType, m_nPosX + 106, m_nPosY + 35, COLOR_TITLE );


    // ProcessNPC Item Scroll bar
    m_sbProcessNPCItem.Render();

    // List box of Need Item Desc
    m_lbPreconditionDesc.Render();

    // Close button
    m_btnClose.Render();

    // ProcessNPC button
    m_btnOK.Render();

    // Cancel button
    m_btnCancel.Render();

    pDrawPort->FlushRenderingQueue();


    // Render ProcessNPC Item
    CTString m_strShortDesc;
    int	nX = SLEARN_SLOT_SX;
    int	nY = SLEARN_SLOT_SY;

    int	iEnd = m_sbProcessNPCItem.GetScrollPos() + PRODUCT_SLOT_ROW;

    if (iEnd > m_vecIcons.size())
        iEnd = m_vecIcons.size();

    for (int i = m_sbProcessNPCItem.GetScrollPos(); i < iEnd; i++)
    {
        if (m_vecIcons[i] != NULL && !m_vecIcons[i]->IsEmpty())
        {
            // render Item
            m_vecIcons[i]->SetPos(nX, nY);
            m_vecIcons[i]->Render(pDrawPort);

            // render Item desc
            CItemData* pItemData = _pNetwork->GetItemData(m_vecIcons[i]->getIndex());
            const char* szItemName = _pNetwork->GetItemName(m_vecIcons[i]->getIndex());
            CTString strOutput = szItemName;

            if (i == m_nSelectProcessNPCItem) //현재 선택되어 있다면
            {
                int index = m_nSelectProcessNPCItem - m_sbProcessNPCItem.GetScrollPos();
                char buf[32];
                sprintf(buf, _S(766, "%d 개"), m_nProcessItemCount);
                strOutput += buf;
            }

            pDrawPort->PutTextExCX(strOutput, m_nPosX + 114, m_nPosY + nY + 8, 0xC3C3C3ff);

            nY += SLEARN_SLOT_OFFSETY;
        }
    }

    // Flush all render text queue
    pDrawPort->EndTextEx();

    pDrawPort->InitTextureData(m_ptdBaseTexture);

    nX = SLEARN_SLOT_SX;
    nY = SLEARN_SLOT_SY;

    m_btnDown.SetEnable(FALSE);
    m_btnUP.SetEnable(FALSE);

    // render sel outline
    if (m_nSelectProcessNPCItem != -1)
    {
        int BoxX, BoxY;

        int nOffset = m_nSelectProcessNPCItem - m_sbProcessNPCItem.GetScrollPos();

        if (nOffset >= 0 && nOffset < PROCESSNPC_SLOT_ROW)
        {
            BoxX = m_nPosX + nX - 1 ;
            BoxY = m_nPosY + nY + (SLEARN_SLOT_OFFSETY *  nOffset) - 1;

            pDrawPort->AddTexture(BoxX, BoxY,	BoxX + 34, BoxY + 34,
                                  m_rtSelOutline.U0, m_rtSelOutline.V0, m_rtSelOutline.U1, m_rtSelOutline.V1,
                                  0xffffffff);

            // Input
            pDrawPort->AddTexture(m_nPosX + 156, BoxY + 19, m_nPosX + 156 + 31, BoxY + 19 + 14,
                                  m_rtInput.U0, m_rtInput.V0, m_rtInput.U1, m_rtInput.V1, 0xFFFFFFFF);

            m_btnUP.SetPos(180, nY + (SLEARN_SLOT_OFFSETY *  nOffset) + 19 + 13 - 14);
            m_btnDown.SetPos(180, nY + (SLEARN_SLOT_OFFSETY *  nOffset) + 19 + 13 - 7);

            m_btnUP.SetEnable(TRUE);
            m_btnDown.SetEnable(TRUE);
            m_btnUP.Render();
            m_btnDown.Render();
        }
    }

    pDrawPort->FlushRenderingQueue();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIProcessNPC::MouseMessage(MSG* pMsg)
{
    WMSG_RESULT	wmsgResult;

    // Title bar
    static BOOL bTitleBarClick = FALSE;

    // Mouse point
    static int	nOldX, nOldY;
    int	nX = LOWORD(pMsg->lParam);
    int	nY = HIWORD(pMsg->lParam);

    // Mouse message
    switch (pMsg->message)
    {
    case WM_MOUSEMOVE:
        {
            if (IsInside(nX, nY))
            {
                CUIManager::getSingleton()->SetMouseCursorInsideUIs();

                int	iRowS = m_sbProcessNPCItem.GetScrollPos();
                int	iRowE = iRowS + PROCESSNPC_SLOT_ROW;

                if (m_vecIcons.size() < iRowE)
                {
                    iRowE = m_vecIcons.size();
                }

                if (IsInsideRect(nX, nY, m_rcItem))
                {
                    for (int iRow = iRowS; iRow < iRowE; iRow++)
                    {
                        m_vecIcons[iRow]->MouseMessage(pMsg);
                    }
                }
            }


            // Move refine
            if (bTitleBarClick && (pMsg->wParam & MK_LBUTTON))
            {
                int	ndX = nX - nOldX;
                int	ndY = nY - nOldY;
                nOldX = nX;
                nOldY = nY;

                Move(ndX, ndY);

                return WMSG_SUCCESS;
            }
            // Close button
            else if (m_btnClose.MouseMessage(pMsg) != WMSG_FAIL)
                return WMSG_SUCCESS;
            // OK button
            else if (m_btnOK.MouseMessage(pMsg) != WMSG_FAIL)
                return WMSG_SUCCESS;
            // Cancel button
            else if (m_btnCancel.MouseMessage(pMsg) != WMSG_FAIL)
                return WMSG_SUCCESS;
            // List box of skill desc
            else if (m_sbProcessNPCItem.MouseMessage(pMsg) != WMSG_FAIL)
                return WMSG_SUCCESS;
            else if (m_lbPreconditionDesc.MouseMessage(pMsg) != WMSG_FAIL)
                return WMSG_SUCCESS;
            else if (m_btnUP.MouseMessage(pMsg) != WMSG_FAIL)
                return WMSG_SUCCESS;
            else if (m_btnDown.MouseMessage(pMsg) != WMSG_FAIL)
                return WMSG_SUCCESS;
        }

        break;

    case WM_LBUTTONDOWN:
        {
            if (IsInside(nX, nY))
            {
                nOldX = nX;
                nOldY = nY;

                // Close button
                if (m_btnClose.MouseMessage(pMsg) != WMSG_FAIL)
                {
                    // Nothing
                }
                // Title bar
                else if (IsInsideRect(nX, nY, m_rcTitle))
                {
                    bTitleBarClick = TRUE;
                }
                // OK button
                else if (m_btnOK.MouseMessage(pMsg) != WMSG_FAIL)
                {
                    // Nothing
                }
                // Cancel button
                else if (m_btnCancel.MouseMessage(pMsg) != WMSG_FAIL)
                {
                    // Nothing
                }
                else if (m_sbProcessNPCItem.MouseMessage(pMsg) != WMSG_FAIL)
                {
                    return WMSG_SUCCESS;
                }
                else if (m_btnUP.MouseMessage(pMsg) != WMSG_FAIL)
                {
                    ++m_nProcessItemCount;

                    if (m_nProcessItemCount > 10) m_nProcessItemCount = 10;

                    SelectItem(m_nSelectProcessNPCItem);
                    return WMSG_SUCCESS;
                }
                else if (m_btnDown.MouseMessage(pMsg) != WMSG_FAIL)
                {
                    --m_nProcessItemCount;

                    if (m_nProcessItemCount < 1) m_nProcessItemCount = 1;

                    SelectItem(m_nSelectProcessNPCItem);
                    return WMSG_SUCCESS;
                }

                CUIManager::getSingleton()->RearrangeOrder(UI_PROCESSNPC, TRUE);
                return WMSG_SUCCESS;
            }
        }
        break;

    case WM_LBUTTONUP:
        {
            // If holding button doesn't exist
			if (UIMGR()->GetDragIcon() == NULL)
            {
                // Title bar
                bTitleBarClick = FALSE;

                // If refine isn't focused
                if (!IsFocused())
                    return WMSG_FAIL;

                // Close button
                if ((wmsgResult = m_btnClose.MouseMessage(pMsg)) != WMSG_FAIL)
                {
                    if (wmsgResult == WMSG_COMMAND)
                        CloseProcessNPC();

                    return WMSG_SUCCESS;
                }
                // OK button
                else if ((wmsgResult = m_btnOK.MouseMessage(pMsg)) != WMSG_FAIL)
                {
                    if (wmsgResult == WMSG_COMMAND)
                        SendProcessNPCReq();

                    return WMSG_SUCCESS;
                }
                // Cancel button
                else if ((wmsgResult = m_btnCancel.MouseMessage(pMsg)) != WMSG_FAIL)
                {
                    if (wmsgResult == WMSG_COMMAND)
                        CloseProcessNPC();

                    return WMSG_SUCCESS;
                }
                else if (m_sbProcessNPCItem.MouseMessage(pMsg) != WMSG_FAIL)
                {
                    return WMSG_SUCCESS;
                }
                else if (m_btnUP.MouseMessage(pMsg) != WMSG_FAIL)
                    return WMSG_SUCCESS;
                else if (m_btnDown.MouseMessage(pMsg) != WMSG_FAIL)
                    return WMSG_SUCCESS;
                else if (IsInsideRect(nX, nY, m_rcIcons))
                {
                    int	iRowS = m_sbProcessNPCItem.GetScrollPos();
                    int	iRowE = iRowS + PROCESSNPC_SLOT_ROW;

                    if (m_vecIcons.size() < iRowE)
                    {
                        iRowE = m_vecIcons.size();
                    }

                    for (int iRow = iRowS; iRow < iRowE; iRow++)
                    {
                        if (m_vecIcons[iRow]->MouseMessage(pMsg) != WMSG_FAIL)
                        {
                            if (m_nSelectProcessNPCItem != iRow) m_nProcessItemCount = 1;

                            m_nSelectProcessNPCItem = iRow;

                            SelectItem(m_nSelectProcessNPCItem);
                            return WMSG_SUCCESS;
                        }
                    }
                }

            }
        }
        break;

    case WM_LBUTTONDBLCLK:
        {
            if (IsInside(nX, nY))
                return WMSG_SUCCESS;
        }

        break;

    case WM_MOUSEWHEEL:
        {
            if (IsInside(nX, nY))
            {
                if (IsInsideRect(nX, nY, m_rcItem))
                {
                    return m_sbProcessNPCItem.MouseMessage(pMsg);
                }
                else if (IsInsideRect(nX, nY, m_rcDesc))
                {
                    return m_lbPreconditionDesc.MouseMessage(pMsg);
                }

                return WMSG_SUCCESS;
            }
        }
        break;
    }

    return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : SelectItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::SelectItem(int _nIndex)
{
    if (_nIndex == -1)
    {
        _nIndex = m_nSelectProcessNPCItem;

        if (_nIndex == -1) return;
    }

    if (_nIndex >= m_vecIcons.size())
        return;

    BOOL bNeedItem			= FALSE;
    BOOL bNeedItemCount		= FALSE;

    m_lbPreconditionDesc.ResetAllStrings();

    // 생산 하고자 하는 아이템 모록
    if (m_vecIcons[_nIndex]->getIndex() == -1) return;

    CItemData*		pProcessNPCItemData	= _pNetwork-> GetItemData(m_vecIcons[_nIndex]->getIndex());

    // 필요 아이템
    int nIndexTemp = 0;

    // 필요 아이템 종류와 갯수 얻기
    nIndexTemp = pProcessNPCItemData->GetNeedItemIndex(0);

    if (nIndexTemp == -1) return;

    m_NeedItems[0].ItemData	= _pNetwork->GetItemData(nIndexTemp);
    m_NeedItems[0].llCount	= pProcessNPCItemData->GetNeedItemCount(0);
    __int64 myMoney = 0;
    myMoney = _pNetwork->MyCharacterInfo.money;
    // 인벤에서 필요 아이템 검사 ( nUniIndex 처리 안했음 )

    SWORD nTab, nIndex;

    if (ItemHelp::HaveItem(m_NeedItems[0].ItemData->GetItemIndex(), &nTab, &nIndex, m_NeedItems[0].llCount))
    {
        bNeedItem = TRUE;
        bNeedItemCount = TRUE;

        m_NeedItems[0].MatTab = nTab;
        m_NeedItems[0].inven_idx = nIndex;
    }

    // 조건 출력
    CTString strTitle;
    CTString strSpace = "";

    // 필요 스킬 출력
    //AddString ( _S( 562, "필요 스킬" ), COLOR_SUB_TITLE );
    //AddString ( strSpace + NeedSSkill.GetName(), bNeedSSkill? COLOR_TEXT : COLOR_NONE );
    //m_bSatisfied = bNeedSSkill;

    //AddString ( CTString ( " " ) );

    // 필요 물품
    switch (m_iResourceType)
    {
    case CItemData::PROCESS_DOC_STONE :
        strTitle = _S(563, "필요 스톤");
        break;

    case CItemData::PROCESS_DOC_PLANT :
        strTitle = _S(564, "필요 크락");
        break;

    case CItemData::PROCESS_DOC_ELEMENT :
        strTitle = _S(565, "필요 원소");
        break;
    }

    AddString(strTitle, COLOR_SUB_TITLE);


    bNeedItem = bNeedItem && bNeedItemCount;
    const char* szItemName = NULL;

    if (m_NeedItems[0].ItemData != NULL)
    {
        szItemName = _pNetwork->GetItemName(m_NeedItems[0].ItemData->GetItemIndex());
        strTitle.PrintF(_S(576, "%s : %d개 "), strSpace + szItemName
                        , m_nProcessItemCount * m_NeedItems[0].llCount);
    }

    AddString(strTitle, bNeedItem ? COLOR_TEXT : COLOR_NONE);
    m_bSatisfied = bNeedItem;

    AddString(CTString(""), COLOR_TEXT);
    AddString(_S(1793, "수수료"), COLOR_SUB_TITLE);
    CTString strMoney;
    static int s_iMoney[3][5] =
    {
        {10, 21, 40, 63, 94}
        , {10, 21, 40, 63, 94}
        , {10, 21, 40, 63, 94}
    };
    __int64 needMoney = s_iMoney[m_iResourceType][m_iResourceSubType] * m_nProcessItemCount * m_NeedItems[0].llCount;
    strMoney.PrintF("%I64d %s", needMoney, _S(1762, "나스"));
    AddString(strMoney, needMoney <= myMoney ? COLOR_TEXT : COLOR_NONE);

    m_bSatisfied = m_bSatisfied && needMoney <= myMoney;
    m_btnOK.SetEnable(m_bSatisfied);
}


// ----------------------------------------------------------------------------
// Name : AddString()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::AddString(CTString& strText, COLOR colText)
{
    int nLength =  strText.Length();

    if (nLength >= _iMaxMsgStringChar)
    {
        nLength -= nLength - _iMaxMsgStringChar;

        do
        {
            if (strText[nLength] == ' ')
            {
                break;
            }

        }
        while (nLength--);

        CTString strTemp2, strTemp;

        strText.Split(nLength, strTemp2, strTemp);
        m_lbPreconditionDesc.AddString(0, strTemp2, colText);

        strTemp2.PrintF("%s", strTemp);

        m_lbPreconditionDesc.AddString(0, strTemp2, colText);

    }
    else
    {
        m_lbPreconditionDesc.AddString(0, strText, colText);
    }

}

// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendProcessNPCReq()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::SendProcessNPCReq()
{
    if (m_bWaitProcessNPCResult == TRUE)
        return;

    if (m_nSelectProcessNPCItem >= m_vecIcons.size())
        return;

    CUIManager* pUIManager = CUIManager::getSingleton();

    if (pUIManager->IsCSFlagOn(CSF_TELEPORT))
    {
        pUIManager->GetChattingUI()->AddSysMessage(_S(942, "순간 이동중에는 가공을 할 수 없습니다.") , SYSMSG_ERROR);
        return;
    }

    _pNetwork->SendNPCProcessMessage(m_vecIcons[m_nSelectProcessNPCItem]->getIndex(), m_nProcessItemCount);

    m_bWaitProcessNPCResult = TRUE;

    m_btnOK.SetEnable(!m_bWaitProcessNPCResult);
}

// ========================================================================= //
//                         Receive message functions                         //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : ProcessNPCRep()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::ProcessNPCRep(LONG processItemDBIndex, LONG count, LONG result)
{
    CUIManager* pUIManager = CUIManager::getSingleton();

    m_bWaitProcessNPCResult = FALSE;

    // Close message box
    pUIManager->CloseMessageBox(MSGCMD_PROCESS_REP);

    // Show result
    CTString	strMessage;
    bool bRetProcessNPCList = false;
    CTString strTitle;
    CTString strContent;

    int commandCode = -1;

    strTitle = _S(1794, "가공 실패");

    switch (result)
    {
    case MSG_ITEM_PROCESS_NPC_ERROR_OK:		// 성공
        {
            strTitle = _S(1795, "가공 성공");
            strContent = _S(1796, "물품 가공에 성공하였습니다. 수수료를 지불합니다.");
            commandCode = MSGCMD_PROCESS_REP;
        }
        break;

    case MSG_ITEM_PROCESS_NPC_ERROR_FAIL:	// 실패
        {
            strContent = _S(1797, "물품 가공에 실패하였습니다. 수수료는 지불하지 않습니다.");
            commandCode = MSGCMD_PROCESS_REP;
        }
        break;

    case MSG_ITEM_PROCESS_NPC_ERROR_COUNT:	// 카운트 오류
        {
            strContent = _S(1798, "가공할 재료의 개수를 입력하여 주십시오.");
            commandCode = MSGCMD_PROCESS_NOTIFY;
        }
        break;

    case MSG_ITEM_PROCESS_NPC_ERROR_NOHAVE:	// 재료 부족
        {
            strContent = _S(1799, "재료가 부족합니다. 확인하고 다시 시도하여 주십시오.");
            commandCode = MSGCMD_PROCESS_NOTIFY;
        }
        break;

    case MSG_ITEM_PROCESS_NPC_ERROR_MONEY:	// 나스 부족
        {
            strContent = _S(1800, "수수료가 부족합니다. 확인하고 다시 시도하여 주십시오.");
            commandCode = MSGCMD_PROCESS_NOTIFY;
        }
        break;
    }

    m_iLastResourceType = m_iResourceType;
    m_iLastResourceSubType = m_iResourceSubType;
    CloseProcessNPC();

    pUIManager->CloseMessageBox(MSGCMD_PROCESS_NOTIFY);
    CUIMsgBox_Info	MsgBoxInfo;
    MsgBoxInfo.SetMsgBoxInfo(strTitle, UMBS_OK, UI_PROCESSNPC, commandCode);
    MsgBoxInfo.AddString(strContent);
    pUIManager->CreateMessageBox(MsgBoxInfo);

    m_bWaitProcessNPCResult = FALSE;
}

void CUIProcessNPC::MsgBoxCommand(int nCommandCode, BOOL bOK, CTString& strInput)
{
    switch (nCommandCode)
    {
    case MSGCMD_PROCESS_NOTIFY:
        {
        } break;

    case MSGCMD_PROCESS_REP:
        {
            OpenProcessList(m_iLastResourceType, m_iLastResourceSubType);
        }
        break;
    }
}

void CUIProcessNPC::MsgBoxLCommand(int nCommandCode, int nResult)
{
    switch (nCommandCode)
    {
    case MSGLCMD_PROCESSNPC_REQ:
        {
            CUIManager* pUIManager = CUIManager::getSingleton();

            switch (nResult)
            {
            case SEL_MINERAL:		// 광석 정련
                {
                    pUIManager->CloseMessageBoxL(MSGLCMD_PROCESSNPC_MINERAL_REQ);

                    pUIManager->CreateMessageBoxL(_S(1790, "광석 정련") , UI_PROCESSNPC, MSGLCMD_PROCESSNPC_MINERAL_REQ);

                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_MINERAL_REQ, TRUE, _S(1801, "그래 잘 생각했네."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_MINERAL_REQ, TRUE, _S(1802, "자 이제 어떤 스톤을 정련할 것인지 한번 골라보라구."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_MINERAL_REQ, TRUE, _S(1803, "아 나라고 무조건 정련에 성공하는 것은 아니네. 가끔 실패를 하기도 하지. 물론 실패한다면 수수료는 받지 않네."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_MINERAL_REQ, FALSE, _S(1804, "스톤 온 정련"), SEL_MINERAL_1);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_MINERAL_REQ, FALSE, _S(1805, "스톤 리스 정련"), SEL_MINERAL_2);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_MINERAL_REQ, FALSE, _S(1806, "스톤 웨버 정련"), SEL_MINERAL_3);
                    //pUIManager->AddMessageBoxLString( MSGLCMD_PROCESSNPC_MINERAL_REQ, FALSE, _S( 1807, "스톤 비스트 정련" ), SEL_MINERAL_4 );
                    //pUIManager->AddMessageBoxLString( MSGLCMD_PROCESSNPC_MINERAL_REQ, FALSE, _S( 1808, "스톤 웰스던 정련" ), SEL_MINERAL_5 );
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_MINERAL_REQ, FALSE, _S(1220, "취소한다."));
                }
                break;

            case SEL_ENERGY:		// 원소 정제
                {
                    pUIManager->CloseMessageBoxL(MSGLCMD_PROCESSNPC_ENERGY_REQ);

                    pUIManager->CreateMessageBoxL(_S(1792, "원소 정제") , UI_PROCESSNPC, MSGLCMD_PROCESSNPC_ENERGY_REQ);

                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_ENERGY_REQ, TRUE, _S(1801, "그래 잘 생각했네."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_ENERGY_REQ, TRUE, _S(1809, "자 이제 어떤 원소를 정제할 것인지 한번 골라보라구."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_ENERGY_REQ, TRUE, _S(1810, "아 나라고 무조건 정제에 성공하는 것은 아니네. 가끔 실패를 하기도 하지. 물론 실패한다면 수수료는 받지 않네."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_ENERGY_REQ, FALSE, _S(1811, "E등급 원소 정제"), SEL_ENERGY_1);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_ENERGY_REQ, FALSE, _S(1812, "D등급 원소 정제"), SEL_ENERGY_2);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_ENERGY_REQ, FALSE, _S(1813, "C등급 원소 정제"), SEL_ENERGY_3);
                    //pUIManager->AddMessageBoxLString( MSGLCMD_PROCESSNPC_ENERGY_REQ, FALSE, _S( 1814, "B등급 원소 정제" ), SEL_ENERGY_4 );
                    //pUIManager->AddMessageBoxLString( MSGLCMD_PROCESSNPC_ENERGY_REQ, FALSE, _S( 1815, "A등급 원소 정제" ), SEL_ENERGY_5 );
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_ENERGY_REQ, FALSE, _S(1220, "취소한다."));
                }
                break;

            case SEL_CROP:			// 식물 가공.
                {
                    pUIManager->CloseMessageBoxL(MSGLCMD_PROCESSNPC_CROP_REQ);

                    pUIManager->CreateMessageBoxL(_S(1791, "식물 가공") , UI_PROCESSNPC, MSGLCMD_PROCESSNPC_CROP_REQ);

                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_CROP_REQ, TRUE, _S(1801, "그래 잘 생각했네."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_CROP_REQ, TRUE, _S(1816, "자 이제 크락의 어떤 부분을 가공할 것인지 한번 골라보라구."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_CROP_REQ, TRUE, _S(1817, "아 나라고 무조건 가공에 성공하는 것은 아니네. 가끔 실패를 하기도 하지. 물론 실패한다면 수수료는 받지 않네."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_CROP_REQ, FALSE, _S(1818, "크락의 노란잎 가공"), SEL_CROP_1);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_CROP_REQ, FALSE, _S(1819, "크락의 줄기 가공"), SEL_CROP_2);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_CROP_REQ, FALSE, _S(1820, "크락의 파란잎 가공"), SEL_CROP_3);
                    //pUIManager->AddMessageBoxLString( MSGLCMD_PROCESSNPC_CROP_REQ, FALSE, _S( 1821, "크락의 가시 가공" ), SEL_CROP_4 );
                    //pUIManager->AddMessageBoxLString( MSGLCMD_PROCESSNPC_CROP_REQ, FALSE, _S( 1822, "크락의 꽃 가공" ), SEL_CROP_5 );
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_CROP_REQ, FALSE, _S(1220, "취소한다."));
                }
                break;
            }
        }
        break;

    case MSGLCMD_PROCESSNPC_MINERAL_REQ:
        {
            switch (nResult)
            {
            case SEL_MINERAL_1:		// 광석 정련
            case SEL_MINERAL_2:		// 광석 정련
            case SEL_MINERAL_3:		// 광석 정련
            case SEL_MINERAL_4:		// 광석 정련
            case SEL_MINERAL_5:		// 광석 정련
                OpenProcessList(SEL_MINERAL, nResult);
                break;
            }
        }
        break;

    case MSGLCMD_PROCESSNPC_ENERGY_REQ:
        {
            switch (nResult)
            {
            case SEL_ENERGY_1:		// 원소 정제
            case SEL_ENERGY_2:		// 원소 정제
            case SEL_ENERGY_3:		// 원소 정제
            case SEL_ENERGY_4:		// 원소 정제
            case SEL_ENERGY_5:		// 원소 정제
                {
                    OpenProcessList(SEL_ENERGY, nResult);
                }
                break;
            }
        }
        break;

    case MSGLCMD_PROCESSNPC_CROP_REQ:
        {
            switch (nResult)
            {
            case SEL_CROP_1:		// 크락 가공
            case SEL_CROP_2:		// 크락 가공
            case SEL_CROP_3:		// 크락 가공
            case SEL_CROP_4:		// 크락 가공
            case SEL_CROP_5:		// 크락 가공
                {
                    OpenProcessList(SEL_CROP, nResult);
                }
                break;
            }
        }
        break;
    }
}

//------------------------------------------------------------------------

void CUIProcessNPC::clearVecBtnEX()
{
    vecBtnEX_Iter iter = m_vecIcons.begin();
    vecBtnEX_Iter eiter = m_vecIcons.end();

    for (; iter != eiter; ++iter)
    {
        SAFE_DELETE(*iter);
    }

    m_vecIcons.clear();
}
