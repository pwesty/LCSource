// UIAffinity.cpp: implementation of the CUIAffinity class.
//
//////////////////////////////////////////////////////////////////////
#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIShop.h>
#include <vector>
#include <Engine/Interface/UIAffinity.h>
#include <Engine/Interface/UIMultList.h>
#include <Common/Packet/ptype_old_do_affinity.h>
#include <Engine/Help/ItemHelp.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Contents/Base/UIQuestBookNew.h>
#include <Engine/Info/MyInfo.h>

//////////////////////////////////////////////////////////////////////////

void CUIAffinity::Control::Create(CUIWindow *pParentWnd, int nX, int nY,
								  FLOAT fTexWidth, FLOAT fTexHeight)
{
	CTString noText = "";

	btnPlus.Create(pParentWnd, noText, nX, nY, 16, 13);
	btnPlus.SetUV(UBS_IDLE, 923, 15, 923+16, 15+13, fTexWidth, fTexHeight);
	btnPlus.SetUV(UBS_ON, 939, 15, 939+16, 15+13, fTexWidth, fTexHeight);
	btnPlus.SetUV(UBS_CLICK, 955, 15, 955+16, 15+13, fTexWidth, fTexHeight);
	btnPlus.CopyUV(UBS_IDLE, UBS_DISABLE);
	
	btnMinus.Create(pParentWnd, noText, nX+17, nY, 16, 13);
	btnMinus.SetUV(UBS_IDLE, 923, 29, 923+16, 29+13, fTexWidth, fTexHeight);
	btnMinus.SetUV(UBS_ON, 939, 29, 939+16, 29+13, fTexWidth, fTexHeight);
	btnMinus.SetUV(UBS_CLICK, 955, 29, 955+16, 29+13, fTexWidth, fTexHeight);
	btnMinus.CopyUV(UBS_IDLE, UBS_DISABLE);
	
	btnMax.Create(pParentWnd, noText, nX, nY+15, 33, 13);
	btnMax.SetUV(UBS_IDLE, 923, 1, 923+33, 1+13, fTexWidth, fTexHeight);
	btnMax.SetUV(UBS_ON, 956, 1, 956+33, 1+13, fTexWidth, fTexHeight);
	btnMax.SetUV(UBS_CLICK, 989, 1, 989+33, 1+13, fTexWidth, fTexHeight);
	btnMax.CopyUV(UBS_IDLE, UBS_DISABLE);

	bEnable = true;
}

void CUIAffinity::Control::Render()
{
	if (!bEnable)
		return;
	btnPlus.Render();
	btnMinus.Render();
	btnMax.Render();
}

WMSG_RESULT CUIAffinity::Control::MouseMessage(MSG* pMsg, int& nBtn)
{
	if (!bEnable)
		return WMSG_FAIL;

	if (btnPlus.MouseMessage(pMsg) != WMSG_FAIL)
	{
		nBtn = 1;
		if (pMsg->message == WM_MOUSEMOVE && pMsg->wParam == 0)
		{
			btnMinus.SetBtnState(UBS_IDLE);
			btnMax.SetBtnState(UBS_IDLE);
		}
		return WMSG_SUCCESS;
	}
	if (btnMax.MouseMessage(pMsg) != WMSG_FAIL)
	{
		nBtn = 3;
		if (pMsg->message == WM_MOUSEMOVE && pMsg->wParam == 0)
		{
			btnPlus.SetBtnState(UBS_IDLE);
			btnMinus.SetBtnState(UBS_IDLE);
		}
		return WMSG_SUCCESS;
	}
	if (btnMinus.MouseMessage(pMsg) != WMSG_FAIL)
	{
		nBtn = 2;
		if (pMsg->message == WM_MOUSEMOVE && pMsg->wParam == 0)
		{
			btnPlus.SetBtnState(UBS_IDLE);
			btnMax.SetBtnState(UBS_IDLE);
		}
		return WMSG_SUCCESS;
	}
	nBtn = 0;
	return WMSG_FAIL;
}

//////////////////////////////////////////////////////////////////////////

enum eAffinitySelection
{
	AFFINITY_CONTRACT,
	AFFINITY_DONATE,
	AFFINITY_SHOP,
	AFFINITY_PRESENT,
	AFFINITY_TALK,
	AFFINITY_EVENT,
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
//  Name 	: CUIAffinity()
//  Desc 	: Constructor
// ----------------------------------------------------------------------------
CUIAffinity::CUIAffinity()
	: m_ptdButtonTexture(NULL)
	, m_bShowToolTip(TRUE)
	, m_bPickTitle(FALSE)
	, m_slNPCIdx(-1)
	, m_slNPCVIdx(-1)
	, m_nAffinityPoint(0)
	, m_nAffinityIndex(0)
{
}

// ----------------------------------------------------------------------------
//  Name 	: ~CUIAffinity()
//  Desc 	: Destructor
// ----------------------------------------------------------------------------
CUIAffinity::~CUIAffinity()
{
	DonationReset();

	Destroy();
	STOCK_RELEASE(m_ptdButtonTexture);
}

// ----------------------------------------------------------------------------
//  Name 	: Create()
//  Desc 	: 
// ----------------------------------------------------------------------------
void CUIAffinity::Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight)
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Create Texture.
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Affinity.tex" ) );
	m_ptdButtonTexture = CreateTexture(CTString("Data\\Interface\\CommonBtn.tex"));
	// Get Texture size.
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();
	
	// Set size of UI on this texture.
	m_rtsBase.AddRectSurface(UIRect(0,0,nWidth,nHeight), UIRectUV(512,0,512+410,240,fTexWidth,fTexHeight));	// 위에 틀.

	// Set title.
	m_rtTitle.SetRect(0, 0, nWidth, 37);
	// Set Item Area.
	m_rtItemSlot.SetRect(12, 46, 12+375, 46+121);

	//////////////////////////////////////////////////////////////////////////
	// setting to Donation button.
	
	// Get Button Texture Size.
	fTexWidth = m_ptdButtonTexture->GetPixWidth();
	fTexHeight = m_ptdButtonTexture->GetPixHeight();

	// close button.
	m_btnClose.Create(this,CTString(""),390,4,16,16);
	m_btnClose.SetUV(UBS_IDLE,211, 33, 227, 49,fTexWidth,fTexHeight);
	m_btnClose.SetUV(UBS_CLICK,229, 33, 245, 49,fTexWidth,fTexHeight);
	m_btnClose.CopyUV(UBS_IDLE,UBS_ON);
	m_btnClose.CopyUV(UBS_IDLE,UBS_DISABLE);

	// 버튼 위치 지정.
	UIRect rcLeft, rcMiddel, rcRight;
	UIRectUV3 rtIdleBtn, rtClickBtn;

	rcLeft.SetRect(0, 0, 14, 21);
	rcMiddel.SetRect(14, 0, 71, 21);
	rcRight.SetRect(71, 0, 85, 21);

	rtIdleBtn.SetUV3(113, 0, 127, 22, 127, 0, 168, 22, 168, 0, 183, 22, fTexWidth, fTexHeight);
	rtClickBtn.SetUV3(187, 0, 201, 22, 201, 0, 242, 22, 242, 0, 256, 22, fTexWidth, fTexHeight);

	m_btnOK.Create( this, _S( 191, "확인" ), 102, 208, 85, 21 );
	m_btnOK.SetRTSurface( UBS_IDLE, rcLeft, rtIdleBtn.rtL );
	m_btnOK.SetRTSurface( UBS_IDLE, rcMiddel, rtIdleBtn.rtM );
	m_btnOK.SetRTSurface( UBS_IDLE, rcRight, rtIdleBtn.rtR );
	m_btnOK.SetRTSurface( UBS_CLICK, rcLeft, rtClickBtn.rtL );
	m_btnOK.SetRTSurface( UBS_CLICK, rcMiddel, rtClickBtn.rtM );
	m_btnOK.SetRTSurface( UBS_CLICK, rcRight, rtClickBtn.rtR );
	m_btnOK.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnOK.CopyRTSurface( UBS_IDLE, UBS_DISABLE );
	m_btnOK.SetNewType(TRUE);

	m_btnCancel.Create( this, _S( 139, "취소" ), 223, 208, 85, 21 );
	m_btnCancel.SetRTSurface( UBS_IDLE, rcLeft, rtIdleBtn.rtL );
	m_btnCancel.SetRTSurface( UBS_IDLE, rcMiddel, rtIdleBtn.rtM );
	m_btnCancel.SetRTSurface( UBS_IDLE, rcRight, rtIdleBtn.rtR );
	m_btnCancel.SetRTSurface( UBS_CLICK, rcLeft, rtClickBtn.rtL );
	m_btnCancel.SetRTSurface( UBS_CLICK, rcMiddel, rtClickBtn.rtM );
	m_btnCancel.SetRTSurface( UBS_CLICK, rcRight, rtClickBtn.rtR );
	m_btnCancel.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyRTSurface( UBS_IDLE, UBS_DISABLE );
	m_btnCancel.SetNewType(TRUE);

	int nLineHeight = 40;
	int nColumn = 4;
	//m_mlItems.Create(this, nX, nY, nWidth, nHeight, nLineHeight, nSpaceX, nSpaceY, nColumn, bSelectList, nLineGap, nIconOffset);
	m_mlItems.Create(this, 12, 46, 375, 121, nLineHeight, 0, 0, nColumn, FALSE, 2, 2);
	m_mlItems.SetColumnType(0, TYPE_ICON);
	m_mlItems.SetColumnPosX(1, nLineHeight + 4);
	m_mlItems.SetColumnPosX(2, 320, TEXT_CENTER);
	m_mlItems.SetColumnPosX(3, 352, TEXT_CENTER);

	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	fTexHeight = m_ptdBaseTexture->GetPixHeight();
	for (int i = 0; i < CONTROL_MAX; i++)
		m_pCtrl[i].Create(this, 278, 51 + i*nLineHeight, fTexWidth, fTexHeight);
}

// ----------------------------------------------------------------------------
//  Name 	: Render()
//  Desc 	: 
// ----------------------------------------------------------------------------
void CUIAffinity::Render()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - pUIManager->GetAffinity()->GetNPCPosX();
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - pUIManager->GetAffinity()->GetNPCPosZ();
	
	if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST )
	{	
		CloseAllUI();
	}

	pDrawPort->InitTextureData( m_ptdBaseTexture );
	m_rtsBase.SetPos( m_nPosX, m_nPosY );
	m_rtsBase.RenderRectSurface( pDrawPort, 0xFFFFFFFF );

	for (int i = 0; i < CONTROL_MAX; i++)
		m_pCtrl[i].Render();

	pDrawPort->FlushRenderingQueue();
	
	// text setting
	CTString strTemp;
	strTemp.PrintF( _S(4461, "기부") );

	pDrawPort->PutTextExCX(strTemp, m_nPosX+NPCAFFINITYDONATION_WIDTH/2, m_nPosY+17, 0xDED9A0FF);
	pDrawPort->EndTextEx();

	pDrawPort->InitTextureData( m_ptdButtonTexture );

	m_btnCancel.Render();
	m_btnClose.Render();
	m_btnOK.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	m_mlItems.Render();

	strTemp.PrintF(_S(5861, "친화도 수치"));
	pDrawPort->PutTextEx(strTemp, m_nPosX + 10, m_nPosY + 177, 0xC5C5C5FF);

	// 친화도 개편2 친화도[현재, 최대] 수치 얻어오기 [2/5/2013 Ranma]
	int nCur = 0;
	int nMax = 0;	
	CNetworkLibrary::_AffinityInfo::mapAffIter iterPoint = _pNetwork->AffinityInfo.mapAffinityList.find(m_nAffinityIndex);
	if (iterPoint != _pNetwork->AffinityInfo.mapAffinityList.end())
	{
		nCur = iterPoint->second.current;
		nMax = iterPoint->second.max;
	}
	
	strTemp.PrintF("%d/%d", nCur, nMax);
	pDrawPort->PutTextExRX(strTemp, m_nPosX + 185, m_nPosY + 177, 0XC5C5C5FF);

	strTemp.PrintF("%8d", m_nAffinityPoint);
	pDrawPort->PutTextExCX(strTemp, m_nPosX + 345, m_nPosY + 177, 0XC5C5C5FF);
	pDrawPort->EndTextEx();

	m_mlItems.RenderIconPopup();
}

// ----------------------------------------------------------------------------
//  Name 	: ResetPosition()
//  Desc 	: 
// ----------------------------------------------------------------------------
void CUIAffinity::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
//  Name 	: AdjustPosition()
//  Desc 	: 
// ----------------------------------------------------------------------------
void CUIAffinity::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
//  Name 	: MouseMessage()
//  Desc 	: 
// ----------------------------------------------------------------------------
WMSG_RESULT CUIAffinity::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	switch( pMsg->message )
	{
		case WM_MOUSEMOVE:
			{
				CUIManager* pUIManager = CUIManager::getSingleton();

				if( IsInside( nX, nY ) )
					pUIManager->SetMouseCursorInsideUIs();

				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				
				// Move UI window.
				if( m_bPickTitle && ( pMsg->wParam & MK_LBUTTON ) )
				{
					nOldX = nX;	nOldY = nY;

					Move( ndX, ndY );

					return WMSG_SUCCESS;
				}
				
				else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if (m_mlItems.MouseMessage(pMsg) != WMSG_FAIL)
					return WMSG_SUCCESS;
				else	// slot item.
				{
					int nBtn = 0;
					for (int i = 0; i < CONTROL_MAX; i++)
						if (m_pCtrl[i].MouseMessage(pMsg, nBtn) != WMSG_FAIL)
							return WMSG_SUCCESS;
					return WMSG_SUCCESS;
				}
			}
			break;

		case WM_LBUTTONDOWN:
			{
				if( IsInside( nX, nY ) )
				{
					CUIManager* pUIManager = CUIManager::getSingleton();

					nOldX = nX;
					nOldY = nY;
					m_nControlButtonDown = -1;

					// Close button
					if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nothing
					}else if( IsInsideRect( nX, nY, m_rtTitle ) )
					{
						m_bPickTitle = TRUE;
					}
					else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
					{

					}
					else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						
					}
					else if (m_mlItems.MouseMessage(pMsg) != WMSG_FAIL)
					{
					}
					else if (IsInsideRect(nX, nY, m_rtItemSlot))
					{
						int nBtn = 0;
						for (int i = 0; i < CONTROL_MAX; i++)
							if (m_pCtrl[i].MouseMessage(pMsg, nBtn) != WMSG_FAIL)
								m_nControlButtonDown = i*100 + nBtn;
					}

					pUIManager->RearrangeOrder(UI_NPC_AFFINITY, TRUE);
					return WMSG_SUCCESS;
				}
			}	
		break;

		case WM_LBUTTONUP:
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
				m_bPickTitle = FALSE;

				if (pUIManager->GetDragIcon() == NULL)
				{
					// if donation view isn't focused.
					if( !IsFocused() )
						return WMSG_FAIL;
					
					// Close button
					if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							CloseDonation();
						return WMSG_SUCCESS;
					}
					else if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							_sendDonateMessage();
						return WMSG_SUCCESS;
					}
					else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							CloseDonation();
						return WMSG_SUCCESS;
					}
					else if (m_mlItems.MouseMessage(pMsg) != WMSG_FAIL)
					{
						return WMSG_SUCCESS;
					}
					else if (IsInsideRect(nX, nY, m_rtItemSlot))
					{
						for (int i = 0; i < CONTROL_MAX; i++)
						{
							int nBtn = 0;
							if (m_pCtrl[i].MouseMessage(pMsg, nBtn) != WMSG_FAIL)
							{
								if (m_nControlButtonDown == (i*100 + nBtn))
								{
									int nRow = m_mlItems.GetTopItem() + m_nControlButtonDown / 100;
									CTString strCount = m_mlItems.GetString(2, nRow);
									CTString strMax = m_mlItems.GetString(3, nRow);
									int nCount = atoi(strCount);
									int nMax = atoi(strMax);
									int nNewCount;

									switch (nBtn)
									{
									case 1:	// plus
										nNewCount = nCount + 1;
										if (nNewCount > nMax)
											nNewCount = nMax;
										break;
									case 2:	// minus
										nNewCount = nCount - 1;
										if (nNewCount < 0)
											nNewCount = 0;
										break;
									case 3:	// max
										nNewCount = nMax;
										break;
									default:
										return WMSG_FAIL;
									}
									if (nNewCount != nCount)
									{
										strCount.PrintF("%d", nNewCount);
										m_mlItems.SetString(2, nRow, strCount);
										m_vecItem[nRow].count = nNewCount;
										m_nAffinityPoint += (nNewCount - nCount)
											* _pNetwork->GetAffinityData()->GetAffinityPoint(m_slNPCIdx, m_vecItem[nRow].index);
									}
									return WMSG_SUCCESS;
								}
								break;
							}
						}
					}
				}
				pUIManager->ResetHoldBtn();
			}
		break;
		case WM_LBUTTONDBLCLK:
			{}
		break;

		case WM_MOUSEWHEEL:
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
				if (pUIManager->GetDragIcon() == NULL)
				{
					if (IsInside(nX, nY))
					{
						if (m_mlItems.MouseMessage(pMsg) != WMSG_FAIL)
							return WMSG_SUCCESS;
					}
				}
			}
		break;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
//  Name 	: MsgBoxCommand()
//  Desc 	: 
// ----------------------------------------------------------------------------
void CUIAffinity::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
		case MSGCMD_AFFINITY_CONTRACT:
			{
				if( bOK )
				{
					// [6/3/2009 rumist] contract to force.
					// send success message to server.
					//_pNetwork->SendAffinityConnectReq( m_nNPCIdx );
					// Temporary NPC Index.
					CPrintF("Mob Index %d\n", INFO()->GetTargetEntity(eTARGET)->GetNetworkID() );
					_pNetwork->SendAffinityConnectReq( m_slNPCVIdx );
				}
			}
			break;
		case MSGCMD_AFFINITY_TAKEPRESENT:
			{
				if( bOK )
				{
					_pNetwork->SendAffinityGiftReq( m_slNPCVIdx );
				}
			}
			break;
	}
}

// ----------------------------------------------------------------------------
//  Name 	: MsgBoxLCommand()
//  Desc 	: 
// ----------------------------------------------------------------------------
void CUIAffinity::MsgBoxLCommand( int nCommandCode, int nResult )
{
	switch( nCommandCode )
	{
		case MSGLCMD_AFFINITY_DONATE_REQ:
			{
				if( nResult == AFFINITY_CONTRACT )
				{
					CUIMsgBox_Info MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo( _S(4464, "친구 맺기"), UMBS_OKCANCEL, UI_NPC_AFFINITY, MSGCMD_AFFINITY_CONTRACT );

					CTString strTemp;

					CAffinityData* pData = _pNetwork->GetAffinityData();
					
					strTemp.PrintF( _S( 5297, "%s 세력과 친구 맺기를 하시겠습니까?"), 
									_pNetwork->GetAffinityData()->GetAffinityName( m_slNPCIdx ) );	
					
					MsgBoxInfo.AddString( strTemp );
					
					int needLevel = pData->GetAffinityData( m_slNPCIdx )->needLevel;

					if( needLevel > 0 )
					{
						strTemp.PrintF( _S( 256, "필요 레벨 : %d" ), needLevel );
						MsgBoxInfo.AddString( strTemp );
					}
					
					int needItemCount = pData->GetAffinityData( m_slNPCIdx )->needItemCount;

					if( needItemCount > 0 )	// 필요 아이템이 있다
					{
						CTString strItemName;
						INDEX needItemIndex = pData->GetAffinityData( m_slNPCIdx )->needItemIndex;

						if( needItemIndex == 19 )	// 나스
							strItemName.PrintF(_S( 836,"%I64d 나스"), needItemCount );
						else
							strItemName.PrintF( _S( 61, "%s %d개" ), _pNetwork->GetItemName(needItemIndex), needItemCount );

						strTemp.PrintF( _s("%s : %s"), _S( 351, "필요 아이템" ), strItemName );
						MsgBoxInfo.AddString( strTemp );
					}

					int needAffinityPoint = pData->GetAffinityData( m_slNPCIdx )->needAffinityPoint;

					if( needAffinityPoint > 0 )
					{
						int needAffinityIndex = pData->GetAffinityData( m_slNPCIdx )->needAffinityIndex;
						
						strTemp.PrintF( _S( 5316, "필요 친화도 : %s 세력 %d" ), 
										_pNetwork->GetAffinityData()->GetAffinityNameByIndex( needAffinityIndex ),
										needAffinityPoint);

						MsgBoxInfo.AddString( strTemp );
					}
					CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
				}
				else if( nResult == AFFINITY_DONATE )
				{
					CUIManager* pUIManager = CUIManager::getSingleton();

					// [2013/02/05] sykim70 친화도 기부 UI 개선
					CNetworkLibrary::_AffinityInfo* pInfo = _pNetwork->GetAffinityPointer();
					if (pInfo->count == 0 || (pInfo->mapAffinityList.find(m_nAffinityIndex) == pInfo->mapAffinityList.end()))
					{
						CUIMsgBox_Info MsgBoxInfo;
						MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
						MsgBoxInfo.AddString(_S(4678, "아직 친구를 맺지 않았습니다. 먼저 친구를 맺어주세요."));
						pUIManager->CreateMessageBox(MsgBoxInfo);
						return;
					}

					DonationReset();

					int i, nItemCount = _pNetwork->GetAffinityData()->GetAffinityData(m_slNPCIdx)->vecDonateList.size();
					int nControlMax = nItemCount > CONTROL_MAX ? CONTROL_MAX : nItemCount;
					for (i = 0; i < nControlMax; i++)
						m_pCtrl[i].SetEnable();

					CUIInventory* pInven = pUIManager->GetInventory();

					CTString textItem;
					std::vector<CAffinityData::_DonationItemInfo>::iterator b, e;
					b = _pNetwork->GetAffinityData()->GetAffinityData(m_slNPCIdx)->vecDonateList.begin();
					e = _pNetwork->GetAffinityData()->GetAffinityData(m_slNPCIdx)->vecDonateList.end();
					for (i = 0; b != e; b++, i++)
					{
						textItem.PrintF(_S(5857, "%s, 친화도를 %d 상승 시켜줍니다."), _pNetwork->GetItemDesc(b->index), b->point);
						m_mlItems.AddIcon(0, CUIImageBox::IT_ITEM, b->index, TRUE, textItem);
						m_mlItems.AddString(1, CTString(_pNetwork->GetItemName(b->index)));
						m_mlItems.AddString(2, CTString("0"), 0x00F200FF);
						textItem.PrintF("%d", (int)pInven->GetItemCount(b->index, false));
						m_mlItems.AddString(3, textItem);

						m_vecItem.push_back(Item(b->index));
					}

					OpenDonation();
				}
				else if( nResult == AFFINITY_SHOP )
				{
					// shop open code.
					_pNetwork->SendAffinityShopReq( m_slNPCVIdx );
				}
				else if( nResult == AFFINITY_PRESENT )
				{
					//take a present from friendly force.
					_pNetwork->SendAffinityGiftInfoReq( m_slNPCVIdx );
					
				}
				else if( nResult == AFFINITY_TALK )
				{
					CUIQuestBook::TalkWithNPC();
				}
				else if( nResult == AFFINITY_EVENT )
				{
				}
				else
				{
				}
			}
	}
}


// ----------------------------------------------------------------------------
//  Name 	: OpenAffinity()
//  Desc 	: 
// ----------------------------------------------------------------------------
void CUIAffinity::OpenAffinity( int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// If this is alreay exist.
	if( pUIManager->DoesMessageBoxLExist( MSGLCMD_AFFINITY_DONATE_REQ )|| IsVisible() )
		return;

	pUIManager->CloseMessageBox(MSGCMD_AFFINITY_CONTRACT);

	// Set position of target npc.
	m_fNpcX = fX;
	m_fNpcZ = fZ;
	
	m_slNPCVIdx = INFO()->GetTargetEntity(eTARGET)->GetNetworkID();
	m_slNPCIdx = iMobIndex;
	m_nAffinityIndex = _pNetwork->GetAffinityData()->GetAffinityIndex(iMobIndex);
	CMobData* MD = CMobData::getData( iMobIndex );
	
	// Create refine message box.
	pUIManager->CreateMessageBoxL( _S( 4462,"친화도" ), UI_NPC_AFFINITY, MSGLCMD_AFFINITY_DONATE_REQ );

	CTString strNpcName = CMobData::getData(iMobIndex )->GetName();
	pUIManager->AddMessageBoxLString( MSGLCMD_AFFINITY_DONATE_REQ, TRUE, strNpcName, -1, 0xE18600FF );
	// 친화도 개편2 NPC 대화 얻기 [2/5/2013 Ranma]
	pUIManager->AddMessageBoxLString( MSGLCMD_AFFINITY_DONATE_REQ, TRUE, 
									pUIManager->GetString(_pNetwork->GetAffinityData()->GetAffinityStringIndex(iMobIndex)),
									-1, 0xA3A1A3FF );

	CTString strMessage;
	// [100323: selo] 플래그에 따라 메뉴를 보여준다
	CTString strAffinityName = _pNetwork->GetAffinityData()->GetAffinityName( iMobIndex );

	if( _pNetwork->GetAffinityData()->IsContract(iMobIndex) )
	{	
		// contract to force.
		strMessage.PrintF( _S( 5298, "%s 세력과 친구 맺기"), strAffinityName );
		pUIManager->AddMessageBoxLString( MSGLCMD_AFFINITY_DONATE_REQ, FALSE, strMessage, AFFINITY_CONTRACT );
	}
	if( _pNetwork->GetAffinityData()->IsDonate(iMobIndex) )
	{	
		// donate item to force.
		strMessage.PrintF( _S( 5299, "%s 세력에 아이템 기부하기"), strAffinityName );
		pUIManager->AddMessageBoxLString( MSGLCMD_AFFINITY_DONATE_REQ, FALSE, strMessage, AFFINITY_DONATE );
	}
	if( _pNetwork->GetAffinityData()->IsShop(iMobIndex) )
	{	
		// using force shop.
		strMessage.PrintF( _S( 5300, "%s 세력의 상점 이용하기"), strAffinityName );
		pUIManager->AddMessageBoxLString( MSGLCMD_AFFINITY_DONATE_REQ, FALSE, strMessage, AFFINITY_SHOP );
	}
	if( _pNetwork->GetAffinityData()->IsPresent(iMobIndex) )
	{	
		// take a present.
		strMessage.PrintF( _S( 5301, "%s 세력에게 선물 받기"), strAffinityName );
		pUIManager->AddMessageBoxLString( MSGLCMD_AFFINITY_DONATE_REQ, FALSE, strMessage, AFFINITY_PRESENT );
	}
	// quest
	if( bHasQuest )
	{
		strMessage.PrintF( _S( 1053, "이야기한다." ) );
		pUIManager->AddMessageBoxLString( MSGLCMD_AFFINITY_DONATE_REQ, FALSE, strMessage, AFFINITY_TALK );
	}
	// event
	if( MD->IsEvent() )
	{
		strMessage.PrintF( _S( 100, "이벤트." ) );
		pUIManager->AddMessageBoxLString( MSGLCMD_AFFINITY_DONATE_REQ, FALSE, strMessage, AFFINITY_EVENT );
	}
	// cancel
	strMessage.PrintF( _S( 1220, "취소한다." ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_AFFINITY_DONATE_REQ, FALSE, strMessage );
}
// ----------------------------------------------------------------------------
//  Name 	: ToggleVisible()
//  Desc 	: 
// ----------------------------------------------------------------------------
void CUIAffinity::ToggleVisible()
{
	BOOL bVisible = !( IsVisible() );

	if( bVisible )
	{
		OpenDonation();
	}
	else
	{
		CloseDonation();
	}
}

// ----------------------------------------------------------------------------
//  Name 	: OpenDonation()
//  Desc 	: 
// ----------------------------------------------------------------------------
void CUIAffinity::OpenDonation()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pUIManager->GetInventory()->IsLocked() == TRUE ||
		pUIManager->GetInventory()->IsLockedArrange() == TRUE)
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	if( IsVisible() == TRUE )
		return;

	CUIManager::getSingleton()->RearrangeOrder( UI_NPC_AFFINITY, TRUE );
	CUIManager::getSingleton()->GetInventory()->Lock(TRUE, TRUE, LOCK_AFFINITY_DONATION);
}

// ----------------------------------------------------------------------------
//  Name 	: CloseDonation()
//  Desc 	: 
// ----------------------------------------------------------------------------
void CUIAffinity::CloseDonation()
{
	DonationReset();
	CUIManager::getSingleton()->RearrangeOrder( UI_NPC_AFFINITY, FALSE );
	CUIManager::getSingleton()->GetInventory()->Lock(FALSE, FALSE, LOCK_AFFINITY_DONATION);
}

// ----------------------------------------------------------------------------
//  Name 	: DonationReset()
//  Desc 	: 
// ----------------------------------------------------------------------------
void CUIAffinity::DonationReset()
{
	m_nAffinityPoint = 0;

	m_bShowToolTip = FALSE;
	m_bPickTitle = FALSE;

	m_mlItems.ResetAllData();
	m_nControlButtonDown = -1;
	for (int i = 0; i < CONTROL_MAX; i++)
		m_pCtrl[i].SetEnable(false);
	m_vecItem.clear();
}

// ----------------------------------------------------------------------------
//  Name 	: OpenShop()
//  Desc 	: 
// ----------------------------------------------------------------------------
void CUIAffinity::OpenShop(const int npcIdx)
{
	CUIManager::getSingleton()->GetShop()->OpenShop(npcIdx, -1, 0, m_fNpcX, m_fNpcZ);
}

// ----------------------------------------------------------------------------
//  Name 	: CloseAllUI()
//  Desc 	: 
// ----------------------------------------------------------------------------
void CUIAffinity::CloseAllUI()
{
	CloseDonation();

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBoxL( MSGLCMD_AFFINITY_DONATE_REQ );
	pUIManager->CloseMessageBox( MSGCMD_AFFINITY_CONTRACT );
	pUIManager->CloseMessageBox( MSGCMD_AFFINITY_TAKEPRESENT );
}

// ----------------------------------------------------------------------------
//  Name 	: IsOpenUI()
//  Desc 	: 
// ----------------------------------------------------------------------------
const BOOL CUIAffinity::IsOpenUI() const
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	return IsVisible() ||
		pUIManager->DoesMessageBoxLExist( MSGLCMD_AFFINITY_DONATE_REQ ) ||
		pUIManager->DoesMessageBoxExist( MSGCMD_AFFINITY_CONTRACT ) || pUIManager->DoesMessageBoxExist( MSGCMD_AFFINITY_TAKEPRESENT );
}

// ----------------------------------------------------------------------------
//  Name 	: _sendDonateMessage()
//  Desc 	: 
// ----------------------------------------------------------------------------
void CUIAffinity::_sendDonateMessage()
{
	int nItemCnt = 0, nAffinityPoint = 0;
	std::vector<Item>::iterator b;
	for (b = m_vecItem.begin(); b != m_vecItem.end(); b++)
	{
		if (b->count > 0)
		{
			nItemCnt++;
			nAffinityPoint += b->count * _pNetwork->GetAffinityData()->GetAffinityPoint(m_slNPCIdx, b->index);
		}
	}
	if (nItemCnt > 0)
	{
		CNetworkMessage nm;

		RequestClient::doAffinityContribute* packet = reinterpret_cast<RequestClient::doAffinityContribute*>(nm.nm_pubMessage);

		packet->type = MSG_EXTEND;
		packet->subType = htonl(MSG_EX_AFFINITY);
		packet->thirdType = MSG_EX_AFFINITY_ITEM_CONTRIBUTE_REQ;
		packet->npcIndex = m_slNPCVIdx;
		packet->clientPoint = nAffinityPoint;
		packet->listCount = nItemCnt;
		
		CPrintF("Donation Item count is %d\n", nItemCnt);
		
		int idx = 0;
		CUIInventory* pInven = CUIManager::getSingleton()->GetInventory();
		for (b = m_vecItem.begin(); b != m_vecItem.end(); b++)
		{
			if (b->count > 0 && 
				ItemHelp::HaveItem(b->index) &&
				pInven->GetItemCount(b->index, false))
			{
				packet->list[idx].itemDBIndex = b->index;
				packet->list[idx].count = b->count;

				++idx;
			}
		}

		nm.setSize(sizeof(*packet) + (sizeof(packet->list[0]) * nItemCnt));

		_pNetwork->SendToServerNew(nm);
	}

	CloseDonation();
}