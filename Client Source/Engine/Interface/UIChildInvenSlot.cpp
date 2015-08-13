#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIQuickSlot.h>
#include <Engine/Interface/UIChildQuickSlot.h>
#include "UIChildInvenSlot.h"
#include <Engine/Contents/Base/InvenData.h>
#include <Engine/Interface/UIReformSystem.h>
#include <Engine/Interface/UICollectBox.h>
#include <Engine/Interface/UIWareHouse.h>
#include <Engine/Interface/UIMix.h>
#include <Engine/Interface/UICompound.h>
#include <Engine/Interface/UIGWMix.h>


//#include <Engine/GameState.h>
extern INDEX g_iXPosInInvenSlot1;
extern INDEX g_iYPosInInvenSlot1;
extern INDEX g_iXPosInInvenSlot2;
extern INDEX g_iYPosInInvenSlot2;
extern INDEX g_iXPosInInvenSlot3;
extern INDEX g_iYPosInInvenSlot3;

enum EInvenTex
{
	EInvenNormalSelect,
		EInvenWearSelect,
		EInvenMixing,
		EInvenSealed,
		EInvenLeaseMark,
		EInvenTotal,
};

// ----------------------------------------------------------------------------
// Name : CUIChildInvenSlot()
// Desc : Constructorreturn ;
// ----------------------------------------------------------------------------
CUIChildInvenSlot::CUIChildInvenSlot(int _bag_index) 
: m_pAllItem(NULL)
, m_nIndexBag(_bag_index)
, m_pImgSelect(NULL)
{
	m_bPickTitle = FALSE;
	m_nSelSlotItemID = -1;
	m_NewItemEffectSurface.Clear();
	ClearNewItemEffect();
}

// ----------------------------------------------------------------------------
// Name : ~CUIChildInvenSlot()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIChildInvenSlot::~CUIChildInvenSlot()
{
	Destroy();

	if (m_ptdCommonTexture)
	{
		_pTextureStock->Release(m_ptdCommonTexture);
		m_ptdCommonTexture = NULL;
	}
	if (m_ptdBaseTexture)
	{
		_pTextureStock->Release(m_ptdBaseTexture);
		m_ptdBaseTexture = NULL;
	}
	if (m_ptdBGSlotTexture)
	{
		_pTextureStock->Release(m_ptdBGSlotTexture);
		m_ptdBGSlotTexture = NULL;
	}
	if (m_pAllItem)
	{
		m_pAllItem = NULL;
	}

	SAFE_DELETE(m_pImgSelect);
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------

void CUIChildInvenSlot::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, ITEM_SLOT_TOTAL_WIDTH, ITEM_SLOT_TOTAL_HEIGHT);
	
	// UI개편 인벤토리 [10/4/2012 Ranma]
	m_ptdBGSlotTexture = CreateTexture(CTString("Data\\Interface\\new_PetInfo.tex"));
	FLOAT fTexWidth = m_ptdBGSlotTexture->GetPixWidth();
	FLOAT fTexHeight = m_ptdBGSlotTexture->GetPixHeight();
	
	// Region of each part
	m_rcTitle.SetRect( 0, 0, ITEM_SLOT_TOTAL_WIDTH, ITEM_SLOT_TITLE_HEIGHT );
	m_rcItemSlot.SetRect( ITEM_SLOT_SX, ITEM_SLOT_SY, ITEM_SLOT_SX + ITEM_SLOT_WIDTH, ITEM_SLOT_SY + ITEM_SLOT_HEIGHT );
	
	m_rsItemSlotSurface.AddRectSurface(UIRect(0, 0, ITEM_SLOT_TOTAL_WIDTH, ITEM_SLOT_TITLE_HEIGHT),
		UIRectUV(ITEM_SLOT_TITLE_UV_SX, ITEM_SLOT_TITLE_UV_SY, ITEM_SLOT_TITLE_UV_WIDTH, ITEM_SLOT_TITLE_UV_HEIGHT,fTexWidth,fTexHeight)); // 배경 타이틀
	
	m_rsItemSlotSurface.AddRectSurface(UIRect(ITEM_SLOT_MIDDLE_SX, ITEM_SLOT_MIDDLE_SY, ITEM_SLOT_TOTAL_WIDTH, ITEM_SLOT_MIDDLE_SY + ITEM_SLOT_MIDDLE_HEIGHT),
		UIRectUV(ITEM_SLOT_MIDDLE_UV_SX, ITEM_SLOT_MIDDLE_UV_SY, ITEM_SLOT_MIDDLE_UV_WIDTH, ITEM_SLOT_MIDDLE_UV_HEIGHT,fTexWidth,fTexHeight)); // 배경 미들
	
	m_rsItemSlotSurface.AddRectSurface(UIRect(ITEM_SLOT_BOTTOM_SX, ITEM_SLOT_BOTTOM_SY, ITEM_SLOT_TOTAL_WIDTH, ITEM_SLOT_BOTTOM_SY + ITEM_SLOT_BOTTOM_HEIGHT),
		UIRectUV(ITEM_SLOT_BOTTOM_UV_SX, ITEM_SLOT_BOTTOM_UV_SY, ITEM_SLOT_BOTTOM_UV_WIDTH, ITEM_SLOT_BOTTOM_UV_HEIGHT,fTexWidth,fTexHeight)); // 배경 바텀
	
	// Create inventory texture
	m_ptdBaseTexture = CreateTexture(CTString("Data\\Interface\\NewInventory.tex"));
	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	fTexHeight = m_ptdBaseTexture->GetPixHeight();
	
	m_rcItemSlotUV.SetUV(ITEM_SLOT_UV_SX, ITEM_SLOT_UV_SY, ITEM_SLOT_UV_WIDTH, ITEM_SLOT_UV_HEIGHT,fTexWidth,fTexHeight); // 배경 슬롯 UV
	
	m_SlotInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(3,441,36,474,fTexWidth,fTexHeight)); // 인벤 아이템 공간 선택이미지
	m_SlotInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(41,441,74,474,fTexWidth,fTexHeight)); // 인벤 장비 공간 선택이미지
	m_SlotInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(79,441,112,474,fTexWidth,fTexHeight)); // 인벤 조합 중인 버튼 이미지
	m_SlotInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(117,441,150,474,fTexWidth,fTexHeight)); // 인벤 봉인 중인 버튼 이미지
	m_SlotInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(173,421,184,432,fTexWidth,fTexHeight)); // 인벤 대여 마크 이미지
	
	m_NewItemEffectSurface.AddRectSurface(UIRect(0, 0, 0, 0), UIRectUV(157, 441, 191, 475, fTexWidth, fTexHeight)); // 새로 습득한 아이템 깜빡이 연출이미지
	
	m_ptdCommonTexture = CreateTexture(CTString("Data\\Interface\\CommonBtn.tex"));
	fTexWidth = m_ptdCommonTexture->GetPixWidth();
	fTexHeight = m_ptdCommonTexture->GetPixHeight();
	
	m_btnClose.Create(this, CTString(""), ITEM_SLOT_TOTAL_WIDTH - CLOSE_BTN_SIZE - CLOSE_BTN_WIDTH_GAP, 4, CLOSE_BTN_SIZE, CLOSE_BTN_SIZE);
	m_btnClose.SetUV(UBS_IDLE, 211, 33, 226, 48, fTexWidth, fTexHeight);
	m_btnClose.SetUV(UBS_CLICK, 229, 33, 244, 48, fTexWidth, fTexHeight);
	m_btnClose.CopyUV(UBS_IDLE, UBS_ON);
	m_btnClose.CopyUV(UBS_IDLE, UBS_DISABLE);

	m_pImgSelect = UISUPPORT()->clone_Image(eIMG_TYPE_SELECT);
	m_pImgSelect->setParent(this);
	m_pImgSelect->Hide(TRUE);
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIChildInvenSlot::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void CUIChildInvenSlot::ResetSavePosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );

	switch( m_nRearrangeOrderID )
	{
	case UI_INVEN_SLOT1:
		{
			g_iXPosInInvenSlot1 = GetPosX();
			g_iYPosInInvenSlot1 = GetPosY();
			break;	
		}
	case UI_INVEN_SLOT2:
		{
			g_iXPosInInvenSlot2 = GetPosX();
			g_iYPosInInvenSlot2 = GetPosY();
			break;	
		}
	case UI_INVEN_SLOT3:
		{
			g_iXPosInInvenSlot3 = GetPosX();
			g_iYPosInInvenSlot3 = GetPosY();
			break;	
		}
	}	
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIChildInvenSlot::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIChildInvenSlot::Render()
{
	
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	
	m_SlotInvenSurface.SetPos(m_nPosX, m_nPosY);
	m_NewItemEffectSurface.SetPos(m_nPosX, m_nPosY);

	// 백그라운드 랜더//////////////////
	pDrawPort->InitTextureData( m_ptdBGSlotTexture );
	
	m_rsItemSlotSurface.SetPos(m_nPosX, m_nPosY);
	m_rsItemSlotSurface.RenderRectSurface(pDrawPort, 0xFFFFFFFF);
	pDrawPort->FlushRenderingQueue();
	
	pDrawPort->EndTextEx();
	///////////////////////////////////

	CTString strTitle;
	//COLOR	 colStrTemp;
	if(m_nRearrangeOrderID == UI_INVEN_SLOT1)
		strTitle =  _S(5780,"2번가방");
	else if(m_nRearrangeOrderID == UI_INVEN_SLOT2)
		strTitle =  _S(5781,"3번가방");
	else
		strTitle =  _S(5782,"4번가방");
	
	pDrawPort->PutTextExCX( strTitle, m_nPosX+ITEM_SLOT_TOTAL_WIDTH/2, m_nPosY+17, 0xFFFFFFFF );
	pDrawPort->EndTextEx();
	
	// 아이템 슬롯 랜더////////////////
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	
	pDrawPort->AddTexture( m_nPosX + m_rcItemSlot.Left, m_nPosY + m_rcItemSlot.Top, m_nPosX + m_rcItemSlot.Right, m_nPosY + m_rcItemSlot.Bottom,
		m_rcItemSlotUV.U0, m_rcItemSlotUV.V0, m_rcItemSlotUV.U1, m_rcItemSlotUV.V1, 0xFFFFFFFF );
	pDrawPort->FlushRenderingQueue();
	
	pDrawPort->EndTextEx();
	////////////////////////////////////
	
	// 아이템 슬롯 랜더////////////////
	pDrawPort->InitTextureData( m_ptdCommonTexture );
	m_btnClose.Render();
	pDrawPort->FlushRenderingQueue();
	
	pDrawPort->EndTextEx();
	////////////////////////////////////
	ItemRender();
	PostRender();
	///////////////////////////////////////////
}

void CUIChildInvenSlot::ItemRender()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	// 아이템 랜더 /////////////////////
	int i, t = GetChildInvenTab();

	for (i = 0; i < INVEN_ONE_BAG; ++i)
	{
		if (IsPlayNewItemEffect(i) == TRUE)
		{
			CInvenData* pInven = CInvenData::getSingleton();

			if (pInven)
			{
				if (pInven->IsNewItemEffectRender() == TRUE &&
					IsCheckEffectPlayCount(i) == TRUE)
				{
					(*m_pAllItem)[t][GetChildInvenIdx(i)]->setImageHide(eIMG_TYPE_NEW, FALSE);
				}
				else
				{
					(*m_pAllItem)[t][GetChildInvenIdx(i)]->setImageHide(eIMG_TYPE_NEW, TRUE);
				}
			}
		}

		(*m_pAllItem)[t][GetChildInvenIdx(i)]->Render(pDrawPort);
	}

	pDrawPort->EndTextEx();

	pDrawPort->FlushRenderingQueue();

	if (m_pImgSelect != NULL)
		m_pImgSelect->Render(pDrawPort);
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIChildInvenSlot::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;
	// Title bar
	static BOOL bLButtonClick = FALSE;
	
	// Item clicked
	static BOOL	bLButtonDownInItem = FALSE;
	
	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );
	
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			
			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;
			
			if( IsInside( nX, nY ) )
				pUIManager->SetMouseCursorInsideUIs();
			
			// Move UI window.
			if( m_bPickTitle && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;
				
				Move( ndX, ndY );
				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else
			{
				int i, t = GetChildInvenTab();
				for (i = 0; i < INVEN_ONE_BAG; ++i)
				{
					(*m_pAllItem)[t][GetChildInvenIdx(i)]->MouseMessage(pMsg);
				}
			}
			
			// Hold item button
			if (pUIManager->GetDragIcon() == NULL && bLButtonDownInItem && 
				(pMsg->wParam& MK_LBUTTON) && (ndX != 0 || ndY != 0))
			{
				if (pUIManager->GetInventory()->IsLocked() == FALSE)
				{
					// Slot items
					if( m_nSelSlotItemID >= 0 )
					{
						// Close message box of inventory
						pUIManager->CloseMessageBox( MSGCMD_DROPITEM );
											
						pUIManager->SetHoldBtn((*m_pAllItem)[GetChildInvenTab()][m_nSelSlotItemID]);
						
						if(pUIManager->DoesUIExist(UI_REFORM_SYSTEM))
						{
							// 리폼 유아이가 떳을 때 아이템 이동이 발생할 경우 검사 하기 위해 보내 준다. [9/6/2012 Ranma]
							pUIManager->GetReformSystem()->SendReformItemBtnInitUniIndex(
								_pNetwork->MySlotItem[GetChildInvenTab()][m_nSelSlotItemID].Item_UniIndex);
						}
					}
				}
				
				bLButtonDownInItem = FALSE;
			}
			
			if( IsInside( nX, nY ) )
			{
				return WMSG_SUCCESS;
			}
		}
		break;
		
	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
				
				nOldX = nX;		nOldY = nY;
				
				bLButtonClick = TRUE;
				
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					
				}
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					m_bPickTitle = TRUE;
				}			
				else if( IsInsideRect( nX, nY, m_rcItemSlot ) )
				{					
					CUIManager* pUIManager = CUIManager::getSingleton();

					int i, t = GetChildInvenTab();

					if (m_nSelSlotItemID >= 0)
					{
						(*m_pAllItem)[t][m_nSelSlotItemID]->setImageHide(eIMG_TYPE_SELECT, TRUE);
					}

					m_nSelSlotItemID = -1;

					for (i = 0; i < INVEN_ONE_BAG; ++i)
					{
						// Update selected item
						if (pUIManager->GetInventory()->IsLockSelect() == true)
							break;

						if ((*m_pAllItem)[t][GetChildInvenIdx(i)]->IsInside(nX, nY))
						{
							m_nSelSlotItemID = GetChildInvenIdx(i);
							pUIManager->GetInventory()->SetCurSelectItem((*m_pAllItem)[t][GetChildInvenIdx(i)]);
							UpdateSelectItem();
						}

						if( (*m_pAllItem)[t][GetChildInvenIdx(i)]->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							bLButtonDownInItem = TRUE;

							if( pUIManager->GetCollectBox()->IsVisible() )
							{// 곤충 채집 상자는 무조건 닫는다.
								pUIManager->RearrangeOrder( UI_COLLECTBOX, FALSE );
							}

							pUIManager->RearrangeOrder( m_nRearrangeOrderID, TRUE );
							return WMSG_SUCCESS;
						}
					}
				}
				
				pUIManager->RearrangeOrder( m_nRearrangeOrderID, TRUE );
				return WMSG_SUCCESS;
			}
		}	
		break;
		
	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			
			bLButtonClick = FALSE;
			bLButtonDownInItem = FALSE;
			CTString	strMessage;
			
			pUIManager->GetInventory()->SetSelWearItemID(-1);

			// If holding button doesn't exist
			if (pUIManager->GetDragIcon() == NULL)
			{
				// Title bar
				m_bPickTitle = FALSE;
				
				// If inventory isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						// nothing
						switch( m_nRearrangeOrderID )
						{
						case UI_INVEN_SLOT1:
							{
								pUIManager->GetInventory()->SetTabSlotInfo(FALSE, 0);
								break;	
							}
						case UI_INVEN_SLOT2:
							{
								pUIManager->GetInventory()->SetTabSlotInfo(FALSE, 1);
								break;	
							}
						case UI_INVEN_SLOT3:
							{
								pUIManager->GetInventory()->SetTabSlotInfo(FALSE, 2);
								break;	
							}
						case UI_INVEN_CASH1:
							{
								pUIManager->GetInventory()->SetTabSlotInfo(FALSE, INVENSLOT_CASH1);
								break;	
							}
						case UI_INVEN_CASH2:
							{
								pUIManager->GetInventory()->SetTabSlotInfo(FALSE, INVENSLOT_CASH2);
								break;	
							}
						}
					}
					
					return WMSG_SUCCESS;
				}

				// Slot items
				else if( IsInsideRect( nX, nY, m_rcItemSlot ) )
				{
					int i, t = GetChildInvenTab();
					for (i = 0; i < INVEN_ONE_BAG; ++i)
						(*m_pAllItem)[t][GetChildInvenIdx(i)]->MouseMessage(pMsg);

					return WMSG_SUCCESS;
				}
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					CUIIcon* pDrag = pUIManager->GetDragIcon();
					// If holding button is item and comes from inventory
					if (pUIManager->GetDragIcon()->getBtnType() == UBET_ITEM &&
						pUIManager->GetDragIcon()->GetWhichUI() == UI_INVENTORY)
					{					
						// Slot items
						if( IsInsideRect( nX, nY, m_rcItemSlot ) )
						{							
							// Swap
							int i, t = GetChildInvenTab();
							for (i = 0; i < INVEN_ONE_BAG; ++i)
							{
								if ((*m_pAllItem)[t][GetChildInvenIdx(i)]->IsInside(nX, nY))
								{
									CItems* pItems = pUIManager->GetDragIcon()->getItems();

									if (pItems == NULL)
										return WMSG_FAIL;

									int nVIdx = pItems->Item_UniIndex;

									if (nVIdx == -1 || 
										nVIdx != _pNetwork->MySlotItem[t][GetChildInvenIdx(i)].Item_UniIndex)
									{
										CUIInventory* pInven = pUIManager->GetInventory();
										// 캐쉬 인벤일 경우 아이템 이동가능 여부 체크, GetchildInvenTab() == 0 : 일반 인벤 GetchildInvenTab() > 0 캐쉬 인벤.
										int nRetMovable = pInven->IsCashInvenMoveItem(pItems->Item_Tab, pItems->InvenIndex, 
											t, GetChildInvenIdx(i));

										if (nRetMovable > eERR_NONE)
										{
											pInven->ErrMessage(nRetMovable);
											pUIManager->ResetHoldBtn();
											return WMSG_SUCCESS;
										}

										if (pUIManager->GetDragIcon()->IsWearTab() == true)
										{
											if (pUIManager->GetInventory()->IsCostumeInventory() == TRUE)
											{
												int nVirIdx = pItems->Item_UniIndex;
												CItemData* pItemData = pItems->ItemData;

												SBYTE subType = MSG_ITEM_WEAR_COSTUME_TAKEOFF;

												if (pItemData == NULL)
													return WMSG_SUCCESS;

												if (_pNetwork->MyCharacterInfo.bOneSuit == TRUE)
												{
													subType = MSG_ITEM_WEAR_COSTUME_SUIT_TAKEOFF;
												}

												if (pUIManager->GetInventory()->IsLockedArrange() == FALSE)
												{
													_pNetwork->SendItemWearingMSG(subType, pItems->Item_Wearing,
														t, GetChildInvenIdx(i), 0, nVirIdx);
												}
																			
											}
											else
											{
												if (pUIManager->GetInventory()->IsLockedArrange() == FALSE)
												{
													_pNetwork->SendItemWearingMSG(MSG_ITEM_WEAR_TAKE_OFF, pItems->Item_Wearing,
														t, GetChildInvenIdx(i), 0);
												}												
											}
										}
										else
										{
											if (pMsg->wParam & MK_SHIFT)							
											{
												if (pInven->IsLocked() == FALSE)
												{
													if (pInven->IsDivideItem(pItems->Item_Tab, pItems->InvenIndex, 
														t, GetChildInvenIdx(i)) == true)
													{
														pInven->ShowDivideMsgBox(t, GetChildInvenIdx(i));
													}
												}
											}
											else
											{
												pInven->SendSwapSlotItems(pItems->Item_Tab, pItems->InvenIndex,
													t, GetChildInvenIdx(i) );
											}
										}
									}
									// Reset holding button
									pUIManager->ResetHoldBtn();

									return WMSG_SUCCESS;
								} // inside
							}
						}
						
					}
					else if( pDrag->getBtnType() == UBET_ITEM &&
						pDrag->GetWhichUI() == UI_WAREHOUSE )
					{
						// Slot items
						if (IsInsideRect(nX, nY, m_rcItemSlot))
						{
							pUIManager->GetWareHouse()->DelWareHouseItemToInventory();							
						} // If - Slot items
					}
					else if( pDrag->getBtnType() == UBET_ITEM &&
						pDrag->GetWhichUI() == UI_WILDPET_INFO)
					{
						// Slot items
						if( IsInsideRect( nX, nY, m_rcItemSlot ) )
						{
							CItems* pItems = pDrag->getItems();

							if (pItems != NULL)
							{
								_pNetwork->SendUseWildPetItem(pItems->Item_Wearing, -1, -1, 
									pDrag->getIndex());
							}
						} // If - Slot items
						
					}
					// Reset holding button
					pUIManager->ResetHoldBtn();
					
					return WMSG_SUCCESS;
				} // If - IsInside
			} // If - If holding button doesn't exist
		}
		break;
		
	case WM_LBUTTONDBLCLK:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			
			if( IsInside( nX, nY ) )
			{
				// Slot items(아이템 부분 더블 클릭시...)
				if (pUIManager->GetInventory()->IsLocked() == FALSE && IsInsideRect( nX, nY, m_rcItemSlot ))
				{
					m_nSelSlotItemID = -1;
					
					int i, t = GetChildInvenTab();
					
					for (i = 0; i < INVEN_ONE_BAG; ++i)
					{		
						if ((*m_pAllItem)[t][GetChildInvenIdx(i)]->MouseMessage(pMsg) == WMSG_COMMAND)
						{
							// Close message box of inventory
							pUIManager->CloseMessageBox( MSGCMD_DROPITEM );
								
							// [090609: selo] : 아이템의 Flag가 퀘스트Flag를 사용 한다면
							//					위치를 계산해 제한 사용 범위 안에 있지 않을 때에는 
							//					서버로 메시지를 보내지 않는다.								
							if(pUIManager->GetInventory()->IsRestritionItem(t, GetChildInvenIdx(i)))
							{
								// [090707: selo] : 서버로 메시지를 보내지 않을 경우 아이템 사용 실패 메시지 박스를 띄움
								CTString strTitle;
								CTString strMessage1;
								CUIMsgBox_Info	MsgBoxInfo;
									
								strTitle	=	_S(191,"확인");
								strMessage1	=	_S( 4428, "여기서는 사용 할 수 없습니다" ); 
								MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
								MsgBoxInfo.AddString(strMessage1);
								pUIManager->CreateMessageBox(MsgBoxInfo);
									
								return WMSG_SUCCESS;
							}
								
								
							// 이기환 수정 (12.9) 조합창이 활성화 되어 있을 때는 조합창으로 아이템을 보냄 
							if( pUIManager->GetMix()->IsEnabled() && pUIManager->GetMix()->IsVisible() )
							{	
								pUIManager->SetHoldBtn((*m_pAllItem)[t][GetChildInvenIdx(i)]);									
								pUIManager->GetMix()->SetMixItem ();
							}
							else if( pUIManager->GetCompound()->IsEnabled() && pUIManager->GetCompound()->IsVisible() )
							{
								pUIManager->SetHoldBtn((*m_pAllItem)[t][GetChildInvenIdx(i)]);								
								pUIManager->GetCompound()->SetCompoundItem ();
							}
							else if( pUIManager->GetGWMix()->IsEnabled() && pUIManager->GetGWMix()->IsVisible() )
							{
								pUIManager->SetHoldBtn((*m_pAllItem)[t][GetChildInvenIdx(i)]);									
								pUIManager->GetGWMix()->SetMixItem();
							}
							else
							{
								// [2012/04/20 : Sora]  인벤토리 lock 상태에서도 더블클릭시
								// SetUseItemSlotInfo함수를 실행하여 실제 사용된 아이템과
								// 차이가 발생, 해당 내용 수정(ITS 8280)
								if (pUIManager->GetInventory()->IsLocked() == FALSE &&
									pUIManager->GetInventory()->IsLockedArrange() == FALSE)
								{
									pUIManager->GetInventory()->SetUseItemSlotInfo(t, GetChildInvenIdx(i));
									// 아이템 부분의 슬롯 더블 클릭시...
									pUIManager->GetInventory()->SendUseSlotItem(t, GetChildInvenIdx(i) );
								}
								else
								{
									pUIManager->GetInventory()->ShowLockErrorMessage();
								}
							}
								
							return WMSG_SUCCESS;
						}
					}
				}
				return WMSG_SUCCESS;
			}
		}
		break;
		
	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				return WMSG_SUCCESS;		// 오동작 막기
			}
		}
		break;
	}
	return WMSG_FAIL;
}

void CUIChildInvenSlot::_openUI(const BOOL bType )
{
	if (IsVisible()) // 중복 실행 방지
		return;
	
	CUIManager* pUIManager = CUIManager::getSingleton();
	
	switch( m_nRearrangeOrderID )
	{
	case UI_INVEN_SLOT1:
		{
			if(g_iXPosInInvenSlot1 == 0 && g_iYPosInInvenSlot1 == 0)
				break;
			
			SetPos( g_iXPosInInvenSlot1, g_iYPosInInvenSlot1 );
			break;	
		}
	case UI_INVEN_SLOT2:
		{
			if(g_iXPosInInvenSlot2 == 0 && g_iYPosInInvenSlot2 == 0)
				break;
			
			SetPos( g_iXPosInInvenSlot2, g_iYPosInInvenSlot2 );
			break;	
		}
	case UI_INVEN_SLOT3:
		{
			if(g_iXPosInInvenSlot2 == 0 && g_iYPosInInvenSlot2 == 0)
				break;
			
			SetPos( g_iXPosInInvenSlot3, g_iYPosInInvenSlot3 );
			break;	
		}
	}

	for (int i = 0; i < INVEN_ONE_BAG; i++)
	{
		SetStartItemEffect(i);
	}

	pUIManager->RearrangeOrder( m_nRearrangeOrderID, TRUE );
}

void CUIChildInvenSlot::_closeUI()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	
	switch( m_nRearrangeOrderID )
	{
	case UI_INVEN_SLOT1:
		{
			g_iXPosInInvenSlot1 = GetPosX();
			g_iYPosInInvenSlot1 = GetPosY();
			break;	
		}
	case UI_INVEN_SLOT2:
		{
			g_iXPosInInvenSlot2 = GetPosX();
			g_iYPosInInvenSlot2 = GetPosY();
			break;	
		}
	case UI_INVEN_SLOT3:
		{
			g_iXPosInInvenSlot3 = GetPosX();
			g_iYPosInInvenSlot3 = GetPosY();
			break;	
		}
	}	
	
	pUIManager->RearrangeOrder( m_nRearrangeOrderID, FALSE );
	ClearNewItemEffect();
	
	//좌표 저장
}

void CUIChildInvenSlot::ToggleVisible(const BOOL bResultView /* = false */)
{	
	if( IsVisible() )
	{
		// 닫을지 말지 결정.
		if (bResultView)
			return;
		else
			_closeUI();
	}
	else
	{
		if (bResultView)
		{
			_openUI(bResultView);
			
			// 아무것도 선택하지 않았다면 셀렉트 이미지 안보이게 처리.
			if (m_nSelSlotItemID < 0)
				UpdateSelectItem();
		}		
	}	
}

// ----------------------------------------------------------------------------
// Name : RenderItems()
// Desc :
// ----------------------------------------------------------------------------
/*
void CUIChildInvenSlot::RenderItemSurfaces()
{
	int tv_posX,tv_posY;
	int nTab;
	
	nTab = 0;
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();
	
	// Set inventory texture
	pDrawPort->InitTextureData(m_ptdBaseTexture);
	
	// Outline of wearing items in item slot
	int i, t = GetChildInvenTab();
	for (i = 0; i < INVEN_ONE_BAG; ++i)
	{
		// UI 개편 인벤토리 최신  [10/12/2012 Ranma]
			
		CItems*		pItems = &_pNetwork->MySlotItem[t][GetChildInvenIdx(i)];
		CItemData*	pItemData = pItems->ItemData;
			
		(*m_pAllItem)[t][GetChildInvenIdx(i)].GetPos(tv_posX, tv_posY);
			
		if (pItemData == NULL)
			continue;

		// 봉인 아이템 표시		
		if(pItems->IsFlag(FLAG_ITEM_COMPOSITION) && !pItemData->IsFlag(ITEM_FLAG_COMPOSITE))
		{
			m_SlotInvenSurface.m_RectSurfaceArray[EInvenSealed].m_RT = UIRect(tv_posX,tv_posY,tv_posX+BTN_SIZE,tv_posY+BTN_SIZE);
			m_SlotInvenSurface.RenderRectSurface(pDrawPort, 0xFFFFFFFF, EInvenSealed);
		}
		// 대여 아이템 표시
		if(pItems->IsFlag(FLAG_ITEM_LENT))
		{
			m_SlotInvenSurface.m_RectSurfaceArray[EInvenLeaseMark].m_RT = UIRect(tv_posX+17, tv_posY+17,tv_posX+29,tv_posY+29);
			m_SlotInvenSurface.RenderRectSurface(pDrawPort, 0xFFFFFFFF, EInvenLeaseMark);
		}
			
		// [090608: selo] 퀘스트 제한 아이템의 경우 사용 가능한지 체크하여 비활성 랜더 처리
		if(pUIManager->GetInventory()->IsRestritionItem(t, GetChildInvenIdx(i)))
		{
			m_SlotInvenSurface.m_RectSurfaceArray[EInvenSealed].m_RT = UIRect(tv_posX,tv_posY,tv_posX+BTN_SIZE,tv_posY+BTN_SIZE);
			m_SlotInvenSurface.RenderRectSurface(pDrawPort, 0xFFFFFFFF, EInvenSealed);
		}

		// 새로 얻은 아이템의 깜빡이 이펙트
		if (IsPlayNewItemEffect(i) == TRUE)
		{
			CInvenData* pInven = CInvenData::getSingleton();

			if (pInven)
			{
				if (pInven->IsNewItemEffectRender() == TRUE &&
					IsCheckEffectPlayCount(i) == TRUE)
				{
					m_NewItemEffectSurface.m_RectSurfaceArray[NEW_EFFECT_ITEM].m_RT = UIRect(tv_posX, tv_posY, tv_posX + BTN_SIZE, tv_posY + BTN_SIZE);
					m_NewItemEffectSurface.RenderRectSurface(pDrawPort, 0xFFFFFFFF, NEW_EFFECT_ITEM);
				}
			}
		}
	}
	
	//  edit by cpp2angel (04.12.21) : 조합중인 아이템 표시
	
	if ( pUIManager->GetMix()->IsVisible() )
	{
		for ( int i = 0; i < MIX_ITEM_SLOT_COUNT; i++ )
		{
			CUIButtonEx* ButtonEx = pUIManager->GetMix()->GetItemSlot ( i );
			
			if ( !ButtonEx->IsEmpty() ) // 비어 있지 않다면....
			{
				(*m_pAllItem)[ButtonEx->GetItemTab()][ButtonEx->GetInvenIndex()].GetPos( tv_posX, tv_posY );
				
				// 				tv_posX = tv_posX + m_nPosX - pUIManager->GetInventory()->GetPosX();
				// 				tv_posY = tv_posY + m_nPosY - pUIManager->GetInventory()->GetPosY();
				
				m_SlotInvenSurface.m_RectSurfaceArray[EInvenMixing].m_RT = UIRect(tv_posX,tv_posY,tv_posX+BTN_SIZE,tv_posY+BTN_SIZE);
				m_SlotInvenSurface.RenderRectSurface(pDrawPort, 0xFFFFFFFF, EInvenMixing);
			}
			
		}
	}
	
	if ( pUIManager->GetCompound()->IsVisible() )
	{
		for ( int i = 0; i < MIX_ITEM_SLOT_COUNT; i++ )
		{
			CUIButtonEx* ButtonEx = pUIManager->GetCompound()->GetItemSlot ( i );
			
			if ( !ButtonEx->IsEmpty() ) // 비어 있지 않다면....
			{
				(*m_pAllItem)[ButtonEx->GetItemTab()][ButtonEx->GetInvenIndex()].GetPos( tv_posX, tv_posY );
				
				// 				tv_posX = tv_posX + m_nPosX - pUIManager->GetInventory()->GetPosX();
				// 				tv_posY = tv_posY + m_nPosY - pUIManager->GetInventory()->GetPosY();
				
				m_SlotInvenSurface.m_RectSurfaceArray[EInvenMixing].m_RT = UIRect(tv_posX,tv_posY,tv_posX+BTN_SIZE,tv_posY+BTN_SIZE);
				m_SlotInvenSurface.RenderRectSurface(pDrawPort, 0xFFFFFFFF, EInvenMixing);
			}
			
		}
	}
	
	// Outline of selected item
	if (m_nSelSlotItemID >= 0)
	{		
		(*m_pAllItem)[t][m_nSelSlotItemID].GetPos( tv_posX, tv_posY );
		
		// 		tv_posX = tv_posX + m_nPosX - pUIManager->GetInventory()->GetPosX();
		// 		tv_posY = tv_posY + m_nPosY - pUIManager->GetInventory()->GetPosY();
		
		m_SlotInvenSurface.m_RectSurfaceArray[EInvenNormalSelect].m_RT = UIRect(tv_posX,tv_posY,tv_posX+BTN_SIZE,tv_posY+BTN_SIZE);
		m_SlotInvenSurface.RenderRectSurface(pDrawPort, 0xFFFFFFFF, EInvenNormalSelect);
	}
	
	pDrawPort->FlushRenderingQueue();
}
*/

int CUIChildInvenSlot::GetChildInvenTab( )
{
	int	nTab = 0;

	if (m_nIndexBag == INVEN_TAB_CASH1) 
		nTab = INVENTORY_TAB_CASH_1;
	else if (m_nIndexBag == INVEN_TAB_CASH2) 
		nTab = INVENTORY_TAB_CASH_2;

	return nTab;
}

int CUIChildInvenSlot::GetChildInvenIdx( int slot )
{
	int idx = -1;
	if (m_nIndexBag < INVEN_TAB_CASH1)
		idx = (m_nIndexBag * INVEN_ONE_BAG) + slot;
	else if (m_nIndexBag <= INVEN_TAB_CASH2)
		idx = slot;

	return idx;
}

void CUIChildInvenSlot::ClearNewItemEffect()
{
	int		t = GetChildInvenTab();

	for (int i = 0; i < INVEN_ONE_BAG; i++)
	{
		m_bNewItem[i] = FALSE;
		m_nPlayCount[i] = 0;
		m_bStartEffect[i] = FALSE;

		if (m_pAllItem != NULL)
			(*m_pAllItem)[t][GetChildInvenIdx(i)]->setImage(eIMG_TYPE_NEW, false);
	}
}

BOOL CUIChildInvenSlot::IsPlayNewItemEffect(int InvenIdx)
{
	if (InvenIdx < 0 || InvenIdx >= INVEN_ONE_BAG)
		return FALSE;

	return m_bNewItem[InvenIdx];
}

void CUIChildInvenSlot::SetNewItemEffect( BOOL bShow, int InvenIdx )
{
	if (InvenIdx < 0 || InvenIdx >= INVEN_ONE_BAG)
		return;

	m_bNewItem[InvenIdx] = bShow;
	m_bStartEffect[InvenIdx] = FALSE;
	m_nPlayCount[InvenIdx] = 0;

	if (IsVisible() != FALSE)
		SetStartItemEffect(InvenIdx);
}

BOOL CUIChildInvenSlot::IsCheckEffectPlayCount(int InvenIdx)
{
	CInvenData* pInvenData = CInvenData::getSingleton();

	if ( pInvenData == NULL )
		return FALSE;

	if (InvenIdx < 0 || InvenIdx >= INVEN_ONE_BAG)
		return FALSE;

	int nEffectCount = pInvenData->GetPlayCount();

	if ( nEffectCount == -1)
		return TRUE;

	if ( m_bStartEffect[InvenIdx] == FALSE)
		return FALSE;

	int nCount = pInvenData->GetRenderCount() - m_nPlayCount[InvenIdx];

	if ( nCount > nEffectCount )
	{
		SetNewItemEffect(FALSE, InvenIdx);
		return FALSE;
	}

	return TRUE;
}

void CUIChildInvenSlot::SetStartItemEffect( int InvenIdx )
{
	if (InvenIdx < 0 || InvenIdx >= INVEN_ONE_BAG)
		return;

	if (m_bNewItem[InvenIdx] == FALSE)
		return;

	CInvenData* pInvenData = CInvenData::getSingleton();

	if ( pInvenData == NULL )
		return;

	m_bStartEffect[InvenIdx] = TRUE;
	m_nPlayCount[InvenIdx] = pInvenData->GetRenderCount();
}

void CUIChildInvenSlot::UpdateSelectItem()
{
	if (m_nSelSlotItemID >= 0)
	{
		int		x, y;
		(*m_pAllItem)[GetChildInvenTab()][m_nSelSlotItemID]->GetPos(x, y);
		m_pImgSelect->SetPos(x, y);
		m_pImgSelect->Hide(FALSE);
	}
	else
	{
		m_pImgSelect->Hide(TRUE);
	}	
}
