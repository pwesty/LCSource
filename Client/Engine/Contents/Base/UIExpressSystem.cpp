#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "UIExpressSystem.h"
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Help/Util_Help.h>
#include "ExpressSystem.h"
#include "UIExpressItemList.h"


enum eLCE_UI_Start_Info
{
	//BASE
	LCE_UI_UV_FRAME_WIDTH = 750,
	LCE_UI_UV_FRAME_HEIGHT = 491,
	//Title
	LCE_UI_INFO_TITLE_X = LCE_UI_INFO_START_X,
	LCE_UI_INFO_TITLE_Y = LCE_UI_INFO_START_Y,
	LCE_UI_INFO_TITLE_WIDTH = LCE_UI_INFO_WIDTH,
	LCE_UI_INFO_TITLE_HEIGHT = 39,
	TITLE_STRING_POSX	= 104,
	TITLE_STRING_POSY	= 16,
	TITLE_STRING_WIDTH	= 542,
	//Middle
	LCE_UI_INFO_MIDDLE_X = LCE_UI_INFO_START_X,
	LCE_UI_INFO_MIDDLE_Y = LCE_UI_INFO_TITLE_HEIGHT,
	LCE_UI_INFO_MIDDLE_WIDTH = LCE_UI_INFO_WIDTH,
	LCE_UI_INFO_MIDDLE_HEIGHT = LCE_UI_INFO_MIDDLE_Y + 392,
	//bottom
	LCE_UI_INFO_BOTTOM_X = LCE_UI_INFO_START_X,
	LCE_UI_INFO_BOTTOM_Y = LCE_UI_INFO_MIDDLE_HEIGHT,
	LCE_UI_INFO_BOTTOM_WIDTH = LCE_UI_INFO_WIDTH,
	LCE_UI_INFO_BOTTOM_HEIGHT = LCE_UI_INFO_BOTTOM_Y + LCE_UI_INFO_HEIGHT,
	//CLOSE BTN
	LCE_UI_INFO_BTN_CLOSE_X			= 730,
	LCE_UI_INFO_BTN_CLOSE_Y			= 4,
	LCE_UI_INFO_BTN_CLOSE_SIZE		= 16,

	LCE_UI_INFO_BTN_CLOSE_UV_OVER_X = 731,
	LCE_UI_INFO_BTN_CLOSE_UV_OVER_Y = 492,
	LCE_UI_INFO_BTN_CLOSE_UV_OVER_WIDTH = LCE_UI_INFO_BTN_CLOSE_UV_OVER_X + 14,
	LCE_UI_INFO_BTN_CLOSE_UV_OVER_HEIGHT = LCE_UI_INFO_BTN_CLOSE_UV_OVER_Y + 14,
	
	LCE_UI_INFO_BTN_CLOSE_UV_CLICK_X = 716,
	LCE_UI_INFO_BTN_CLOSE_UV_CLICK_Y = 492,
	LCE_UI_INFO_BTN_CLOSE_UV_CLICK_WIDTH = LCE_UI_INFO_BTN_CLOSE_UV_CLICK_X + 14,
	LCE_UI_INFO_BTN_CLOSE_UV_CLICK_HEIGHT = LCE_UI_INFO_BTN_CLOSE_UV_CLICK_Y + 14,
	// BTN INFO
	LCE_UI_INFO_BTN_START_GAP_X			= 18,
	LCE_UI_INFO_BTN_START_GAP_Y			= 12,
	LCE_UI_INFO_BTN_LEFT_WIDTH			= 7,
	LCE_UI_INFO_BTN_MIDDLE_WIDTH		= 81,
	LCE_UI_INFO_BTN_RIGHT_WIDTH			= 7,
	LCE_UI_INFO_BTN_SIZE_WIDTH			= LCE_UI_INFO_BTN_LEFT_WIDTH + LCE_UI_INFO_BTN_MIDDLE_WIDTH + LCE_UI_INFO_BTN_RIGHT_WIDTH,
	LCE_UI_INFO_BTN_SIZE_HEIGHT			= 21,
	LCE_UI_INFO_BTN_GAP					= 8,
	// RECV, RECVALL BTN
	LCE_UI_INFO_BTN_RECV_START_X		= LCE_UI_INFO_WIDTH - LCE_UI_INFO_BTN_START_GAP_X,
	LCE_UI_INFO_BTN_RECV_START_Y		= LCE_UI_INFO_BOTTOM_Y + LCE_UI_INFO_BTN_START_GAP_Y,
	// DELETE, DELETEALL BTN
	LCE_UI_INFO_BTN_DELETE_ALL_START_X	= LCE_UI_INFO_BTN_START_GAP_X,
	LCE_UI_INFO_BTN_DELETE_ALL_START_Y	= LCE_UI_INFO_BOTTOM_Y + LCE_UI_INFO_BTN_START_GAP_Y,
	// BTN IDLE UV
	LCE_UI_INFO_BTN_IDLE_LEFT_UV_X			= 0,
	LCE_UI_INFO_BTN_IDLE_LEFT_UV_Y			= 562,
	LCE_UI_INFO_BTN_IDLE_LEFT_UV_WIDTH		= LCE_UI_INFO_BTN_IDLE_LEFT_UV_X + LCE_UI_INFO_BTN_LEFT_WIDTH,
	LCE_UI_INFO_BTN_IDLE_LEFT_UV_HEIGHT		= LCE_UI_INFO_BTN_IDLE_LEFT_UV_Y + LCE_UI_INFO_BTN_SIZE_HEIGHT,
	LCE_UI_INFO_BTN_IDLE_MIDDLE_UV_X		= LCE_UI_INFO_BTN_IDLE_LEFT_UV_WIDTH,
	LCE_UI_INFO_BTN_IDLE_MIDDLE_UV_Y		= LCE_UI_INFO_BTN_IDLE_LEFT_UV_Y,
	LCE_UI_INFO_BTN_IDLE_MIDDLE_UV_WIDTH	= LCE_UI_INFO_BTN_IDLE_MIDDLE_UV_X + LCE_UI_INFO_BTN_MIDDLE_WIDTH,
	LCE_UI_INFO_BTN_IDLE_MIDDLE_UV_HEIGHT	= LCE_UI_INFO_BTN_IDLE_MIDDLE_UV_Y + LCE_UI_INFO_BTN_SIZE_HEIGHT,
	LCE_UI_INFO_BTN_IDLE_RIGHT_UV_X			= LCE_UI_INFO_BTN_IDLE_MIDDLE_UV_WIDTH,
	LCE_UI_INFO_BTN_IDLE_RIGHT_UV_Y			= LCE_UI_INFO_BTN_IDLE_LEFT_UV_Y,
	LCE_UI_INFO_BTN_IDLE_RIGHT_UV_WIDTH		= LCE_UI_INFO_BTN_IDLE_RIGHT_UV_X + LCE_UI_INFO_BTN_RIGHT_WIDTH,
	LCE_UI_INFO_BTN_IDLE_RIGHT_UV_HEIGHT	= LCE_UI_INFO_BTN_IDLE_RIGHT_UV_Y + LCE_UI_INFO_BTN_SIZE_HEIGHT,
	// BTN CLICK UV
	LCE_UI_INFO_BTN_CLICK_LEFT_UV_X			= 0,
	LCE_UI_INFO_BTN_CLICK_LEFT_UV_Y			= 584,
	LCE_UI_INFO_BTN_CLICK_LEFT_UV_WIDTH		= LCE_UI_INFO_BTN_CLICK_LEFT_UV_X + LCE_UI_INFO_BTN_LEFT_WIDTH,
	LCE_UI_INFO_BTN_CLICK_LEFT_UV_HEIGHT	= LCE_UI_INFO_BTN_CLICK_LEFT_UV_Y + LCE_UI_INFO_BTN_SIZE_HEIGHT,
	LCE_UI_INFO_BTN_CLICK_MIDDLE_UV_X		= LCE_UI_INFO_BTN_CLICK_LEFT_UV_WIDTH,
	LCE_UI_INFO_BTN_CLICK_MIDDLE_UV_Y		= LCE_UI_INFO_BTN_CLICK_LEFT_UV_Y,
	LCE_UI_INFO_BTN_CLICK_MIDDLE_UV_WIDTH	= LCE_UI_INFO_BTN_CLICK_MIDDLE_UV_X + LCE_UI_INFO_BTN_MIDDLE_WIDTH,
	LCE_UI_INFO_BTN_CLICK_MIDDLE_UV_HEIGHT	= LCE_UI_INFO_BTN_CLICK_MIDDLE_UV_Y + LCE_UI_INFO_BTN_SIZE_HEIGHT,
	LCE_UI_INFO_BTN_CLICK_RIGHT_UV_X		= LCE_UI_INFO_BTN_CLICK_MIDDLE_UV_WIDTH,
	LCE_UI_INFO_BTN_CLICK_RIGHT_UV_Y		= LCE_UI_INFO_BTN_CLICK_LEFT_UV_Y,
	LCE_UI_INFO_BTN_CLICK_RIGHT_UV_WIDTH	= LCE_UI_INFO_BTN_CLICK_RIGHT_UV_X + LCE_UI_INFO_BTN_RIGHT_WIDTH,
	LCE_UI_INFO_BTN_CLICK_RIGHT_UV_HEIGHT	= LCE_UI_INFO_BTN_CLICK_RIGHT_UV_Y + LCE_UI_INFO_BTN_SIZE_HEIGHT,
	// BTN DISABLE UV
	LCE_UI_INFO_BTN_DISABLE_LEFT_UV_X		= 0,
	LCE_UI_INFO_BTN_DISABLE_LEFT_UV_Y		= 606,
	LCE_UI_INFO_BTN_DISABLE_LEFT_UV_WIDTH	= LCE_UI_INFO_BTN_DISABLE_LEFT_UV_X + LCE_UI_INFO_BTN_LEFT_WIDTH,
	LCE_UI_INFO_BTN_DISABLE_LEFT_UV_HEIGHT	= LCE_UI_INFO_BTN_DISABLE_LEFT_UV_Y + LCE_UI_INFO_BTN_SIZE_HEIGHT,
	LCE_UI_INFO_BTN_DISABLE_MIDDLE_UV_X		= LCE_UI_INFO_BTN_DISABLE_LEFT_UV_WIDTH,
	LCE_UI_INFO_BTN_DISABLE_MIDDLE_UV_Y		= LCE_UI_INFO_BTN_DISABLE_LEFT_UV_Y,
	LCE_UI_INFO_BTN_DISABLE_MIDDLE_UV_WIDTH	= LCE_UI_INFO_BTN_DISABLE_MIDDLE_UV_X + LCE_UI_INFO_BTN_MIDDLE_WIDTH,
	LCE_UI_INFO_BTN_DISABLE_MIDDLE_UV_HEIGHT= LCE_UI_INFO_BTN_DISABLE_MIDDLE_UV_Y + LCE_UI_INFO_BTN_SIZE_HEIGHT,
	LCE_UI_INFO_BTN_DISABLE_RIGHT_UV_X		= LCE_UI_INFO_BTN_DISABLE_MIDDLE_UV_WIDTH,
	LCE_UI_INFO_BTN_DISABLE_RIGHT_UV_Y		= LCE_UI_INFO_BTN_DISABLE_LEFT_UV_Y,
	LCE_UI_INFO_BTN_DISABLE_RIGHT_UV_WIDTH	= LCE_UI_INFO_BTN_DISABLE_RIGHT_UV_X + LCE_UI_INFO_BTN_RIGHT_WIDTH,
	LCE_UI_INFO_BTN_DISABLE_RIGHT_UV_HEIGHT	= LCE_UI_INFO_BTN_DISABLE_RIGHT_UV_Y + LCE_UI_INFO_BTN_SIZE_HEIGHT,
	// ITEMLIST_BTN
	LCE_UI_ITEM_BTN_START_X					= LCE_UI_INFO_MIDDLE_X + 18,
	LCE_UI_ITEM_BTN_START_Y					= LCE_UI_INFO_MIDDLE_Y + 32,
	LCE_UI_ITEM_LIST_WIDTH					= 714,
	LCE_UI_ITEM_LIST_HEIGHT					= BTN_SIZE,
	LCE_UI_ITEM_BTN_LIST_Y_GEP				= BTN_SIZE + 4,
	// ITEMLIST_COLUM
	LCE_UI_ITEM_LIST_COLUM_START_X			= (LCE_UI_ITEM_BTN_START_X),
	LCE_UI_ITEM_LIST_COLUM_START_Y			= (LCE_UI_INFO_MIDDLE_Y + 13),
	LCE_UI_ITEM_LIST_COLUM_ITEM_NAME		= LCE_UI_ITEM_LIST_COLUM_START_X ,
	LCE_UI_ITEM_LIST_COLUM_QUANTITY			= LCE_UI_ITEM_LIST_COLUM_ITEM_NAME + COLUM_X_GAP + CULUM_X_WIDTH_ITME_NAME,
	LCE_UI_ITEM_LIST_COLUM_AMOUNT			= LCE_UI_ITEM_LIST_COLUM_QUANTITY + COLUM_X_GAP + CULUM_X_WIDTH_QUANTITY,
	LCE_UI_ITEM_LIST_COLUM_SENDER			= LCE_UI_ITEM_LIST_COLUM_AMOUNT + COLUM_X_GAP + CULUM_X_WIDTH_AMOUNT,
	LCE_UI_ITEM_LIST_COLUM_TYPE				= LCE_UI_ITEM_LIST_COLUM_SENDER + COLUM_X_GAP + CULUM_X_WIDTH_SENDER,
	LCE_UI_ITEM_LIST_COLUM_TIME				= LCE_UI_ITEM_LIST_COLUM_TYPE + CULUM_X_WIDTH_TYPE,	
	// ITEM_NAME_STRINGELLIPSIS
	ITEM_NAME_STRING_ELLIPSIS_MAX			= CULUM_X_WIDTH_ITME_NAME - BTN_SIZE - (COLUM_X_GAP * 2),
};

enum eLCE_UI_Frame_Info
{
	FRAME_SELECT = 0,
};

CUIExpressSystem::CUIExpressSystem(void)
	:	m_ptdLCETexture(NULL)
	,	m_pExPressData(NULL)
	,	m_bItemList(false)
	,	m_strCurPage("0")
	,	m_nItemCount(0)
	,	m_SelectItem_index(-1)
{
	for ( int i = 0; i < MAX_ITEM_LIST; i++ )
	{
		m_pExpressItemList[i] = NULL;
	}
}

CUIExpressSystem::~CUIExpressSystem(void)
{
	if (m_ptdLCETexture)
	{
		_pTextureStock->Release(m_ptdLCETexture);
		m_ptdLCETexture = NULL;
	}

	for (int i = 0; i < MAX_ITEM_LIST; i++)
	{
		if (m_pExpressItemList[i])
		{
			delete m_pExpressItemList[i];
			m_pExpressItemList[i] = NULL;
		}
	}
}

//------------------------Interface--------------------------------

void CUIExpressSystem::UpdateExpressItemList()
{
	if( m_bItemList = _SetItemList() )
	{
		m_btnAllRecv.SetBtnState(UBS_IDLE);
		m_btnAllDelete.SetBtnState(UBS_IDLE);
	}
	else
	{
		m_btnAllRecv.SetBtnState(UBS_DISABLE);
		m_btnAllDelete.SetBtnState(UBS_DISABLE);
	}
	_SetPage();
}

void CUIExpressSystem::OpenExpressSystem()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (IsVisible() == TRUE)
		return;

	UpdateExpressItemList();
	pUIManager->RearrangeOrder( UI_LCE_SYSTEM, TRUE );
}

void CUIExpressSystem::CloseExpressSystem()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (m_pExPressData)
		m_pExPressData->ClearNPCInfo();

	ClearExpress();	

	pUIManager->RearrangeOrder( UI_LCE_SYSTEM, FALSE );
}

//----------------------------------------------------------------

bool CUIExpressSystem::SetLinkData()
{
	GameDataManager* pGameDataManager = GameDataManager::getSingleton();
	if ( pGameDataManager == NULL )
		return false;

	m_pExPressData = pGameDataManager->GetExpressData();
	if ( m_pExPressData == NULL )
		return false;

	if ( m_pExPressData->getExpressCount() <= 0 )
		return false;

	return true;
}

void CUIExpressSystem::ClearExpress()
{
	ClearItemList();	
}

void CUIExpressSystem::ClearItemList()
{
	m_bItemList		= false;
	m_pExPressData	= NULL;
	m_strCurPage	= CTString("0");
	m_nItemCount	= 0;
	m_SelectItem_index = -1;

	m_btnRecv.SetBtnState(UBS_DISABLE);
	m_btnDelete.SetBtnState(UBS_DISABLE);

	for (int i = 0; i < MAX_ITEM_LIST; i++)
	{
		if (m_pExpressItemList[i])
				m_pExpressItemList[i]->InitItemList();
	}

	_CheckExistMsgBox();
}

//---------------------------------------------------------------
// 아이템 리스트 관련 func

bool CUIExpressSystem::_SetItemListString(int nIndex, bool bItem)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	UtilHelp* pHelp = UtilHelp::getSingleton();

	if ( pHelp == NULL )
	{
		_FailReset();
		return false;
	}

	ExpressSystemItemInfo* pInfo = NULL;

	pInfo = m_pExPressData->getExpressListAt(nIndex);

	if ( pInfo == NULL )
	{
		_FailReset();
		return false;
	}

	CUIExpressItemList* pItemList = m_pExpressItemList[nIndex];

	if ( pItemList == NULL )
	{
		_FailReset();
		return false;
	}

	CTString strTemp;
	COLOR	tempColor = DEF_UI_COLOR_WHITE;

	// 아이템일 경우
	if (bItem)
	{
		int nItemIndex = pInfo->item_index;

		CItemData* pItemData = _pNetwork->GetItemData(pInfo->item_index);

		if ( pItemData == NULL )
		{
			_FailReset();
			return false;
		}

		CTString strItemName = _pNetwork->GetItemName( nItemIndex );		
		int RareGrade = pItemList->GetBtn()->getItems()->Item_RareIndex; // pInfo->option_level[0]

		// 아이템 카운트
		strTemp.PrintF( "%d", pInfo->item_count );
		pItemList->SetString( LCE_STRING_NUM, strTemp );

		// 나스 금액 (아이템 일 경우)
		strTemp.PrintF( "-" );
		pItemList->SetString( LCE_STRING_AMOUNT, strTemp );
		pItemList->SetNasItem(false);

		// 아이템 이름 색
		tempColor = pHelp->GetRareItemColor(RareGrade, pItemData->GetFlag(), pInfo->option_count); 
		pItemList->SetStringColor( LCE_COLOR_NAME, tempColor );

		// 아이템 레어일 경우 이름 프리픽스 얻어오기
		if ( pItemData->IsFlag( ITEM_FLAG_RARE ) )
		{
			CItemRareOption* pkRareOpt = CItemRareOption::getData(RareGrade);

			if (pkRareOpt != NULL)
			{
				CTString strPrefix = pkRareOpt->GetPrefix();

				if( strPrefix.Length() >0)
					strItemName.PrintF("%s %s", strPrefix, strItemName);
			}			
		}

		// 이름이 길경우 짜르기
		strItemName = pHelp->GetCalcStringEllipsis(strItemName, ITEM_NAME_STRING_ELLIPSIS_MAX, "..");
		
		// 아이템 이름 설정
		pItemList->SetString( LCE_STRING_NAME, strItemName );
	}
	else // 나스일 경우
	{
		// 나스 이름 셋팅
		pItemList->SetString( LCE_STRING_NAME, _S(6026, "나스") );

		// 나스 수량
		strTemp.PrintF( "%I64d", pInfo->nas );
		pUIManager->InsertCommaToString( strTemp );

		pItemList->SetString( LCE_STRING_AMOUNT, strTemp );
		pItemList->SetNasItem(true);

		// 나스 수량 색 설정
		pItemList->SetStringColor( LCE_COLOR_AMOUNT, pUIManager->GetNasColor(pInfo->nas) );
		
		// 나스일경우 아이템 갯수 설정
		pItemList->SetString( LCE_STRING_NUM, _s("-") );
	}

	// Sender
	strTemp.PrintF( "%s", pInfo->sender );
	pItemList->SetString( LCE_STRING_SENDER, strTemp );

	// Type
	strTemp = _GetSenderType( pInfo->send_type );
	pItemList->SetString( LCE_STRING_TYPE, strTemp );
	
	// 등록 시간
	tm*  pTimeStart = NULL;
	tm*  pTimeEnd = NULL;
	char registe[25] = {0,};
	char expire[25] = {0,};

	pTimeStart = localtime((time_t*)&pInfo->registe_date);

	if (pTimeStart)
	{
		strftime(registe, sizeof(registe), "%Y/%m/%d %I:%M:%S", pTimeStart);

		strTemp.PrintF("%s", registe);
		pItemList->SetString( LCE_STRING_DATE_START, strTemp );
	}

	pTimeEnd = localtime((time_t*)&pInfo->expire_date);

	if (pTimeEnd)
	{
		strftime(expire, sizeof(expire), "%Y/%m/%d %I:%M:%S", pTimeEnd);

		strTemp.PrintF("%s", expire);
		pItemList->SetString( LCE_STRING_DATE_END, strTemp );
	}

	// 등록시간 컬러
	pItemList->SetStringColor( LCE_COLOR_DATE, pHelp->GetReuseTimeColor(pInfo->expire_date) );
	
	return true;
}

bool CUIExpressSystem::_SetItemList()
{
	if ( m_pExPressData == NULL )
		return false;

	int nMax = m_pExPressData->getExpressCount();

	if (nMax <= 0)
		return false;

	ExpressSystemItemInfo* pInfo = NULL;

	for ( int i= 0; i < nMax; ++i)
	{
		if (m_pExpressItemList[i] == NULL)
		{
			_FailReset();
			return false;
		}

		CUIIcon* pExpressItemBtn = m_pExpressItemList[i]->GetBtn();

		if ( pExpressItemBtn == NULL )
			continue;

		pInfo = m_pExPressData->getExpressListAt(i);

		if ( pInfo == NULL )
			continue;

		// 나스일경우 체크
		if (pInfo && pInfo->item_index <= 0 )
		{
			if ( pInfo->nas > 0)
			{
				// nas로 셋팅
				pExpressItemBtn->setData(UBET_ITEM, NAS_INDEX, pInfo->index);
				pExpressItemBtn->setCount(pInfo->nas);				

				if ( _SetItemListString(i, false) == false )
				{
					_FailReset();
					return false;
				}				
				continue;
			}
			else
			{
				_FailReset();
				return false;			
			}
		}

		CItems* pItems = new CItems;

		// 아이템일 경우 셋팅
		pItems->Item_Index = pInfo->item_index;
		pItems->Item_UniIndex = pInfo->index;
		pItems->Item_Flag = pInfo->flag;
		pItems->Item_Plus =  pInfo->plus;
		pItems->Item_State_Plus = pInfo->plus2;

#ifdef	DURABILITY
		pItems->Item_durability_now = pInfo->now_durability;
		pItems->Item_durability_max = pInfo->max_durability;
#endif	// DURABILITY

		CItemData* pItemData = _pNetwork->GetItemData(pInfo->item_index);

		if ( pItemData == NULL )
			continue;	

		pItems->ItemData = pItemData;

		// 레어 아이템 옵션 셋팅
		if( pItemData->GetFlag() & ITEM_FLAG_RARE )
		{
			if (pInfo->option_count == 0)
			{
				pItems->SetRareIndex(0);
			}
			else
			{
				LONG iRareIndex = pInfo->option_level[0];

				pItems->SetRareIndex(iRareIndex);

				WORD iRareOption = pInfo->option_level[1];
				WORD wCBit =1;
				SBYTE sbOption =-1;

				for(int iBit = 0; iBit < 10; ++iBit)
				{
					if(iRareOption & wCBit)
					{
						CItemRareOption* prItem = CItemRareOption::getData(iRareIndex);

						if (prItem == NULL)
							continue;

						if (prItem->GetIndex() < 0)
							continue;

						int OptionType = prItem->rareOption[iBit].OptionIdx;
						int OptionLevel = prItem->rareOption[iBit].OptionLevel;
						pItems->SetOptionData( ++sbOption, OptionType, OptionLevel, ORIGIN_VAR_DEFAULT );
					}

					wCBit <<=1;
				}
			}
		}
		else // 레어 아이템이 아닐 경우
		{			
			// 오리진 아이템일 경우
			if (pItemData->GetFlag() & ITEM_FLAG_ORIGIN)
			{
				int		OpCount;

				for (OpCount = 0; OpCount < MAX_ORIGIN_OPTION; OpCount++)
				{
					pItems->SetOptionData( OpCount, pItemData->GetOptionOriginType(OpCount), 
						pItemData->GetOptionOriginLevel(OpCount), pInfo->item_origin[OpCount] );
				}

				// 아이템 스킬 세팅
				for (OpCount = 0; OpCount < MAX_ITEM_SKILL; OpCount++)
				{
					pItems->SetItemSkill(OpCount, pItemData->GetOptionSkillType(OpCount),
						pItemData->GetOptionSkillLevel(OpCount));
				}
			}
			else // 레어도 오리진도 아닌 아이템
			{
				for (int OpCount = 0; OpCount < pInfo->option_count; OpCount++)
				{
					pItems->SetOptionData( OpCount, pInfo->option_type[OpCount], 
						pInfo->option_level[OpCount], ORIGIN_VAR_DEFAULT );
				}
			}
		}

		pItems->InitSocketInfo();

		int	socket;
		SBYTE sbSocketCreateCount = 0;

		for (socket = 0; socket < MAX_SOCKET_COUNT; socket++)
		{
			if ( pInfo->socket[socket] >= 0 )
				sbSocketCreateCount++;
		}

		pItems->SetSocketCount( sbSocketCreateCount );

		for (socket = 0; socket < MAX_SOCKET_COUNT; socket++)
		{
			if ( pInfo->socket[socket] >= 0 )
				pItems->SetSocketOption( socket, pInfo->socket[socket] );
		}

		pExpressItemBtn->setData(pItems, false);
		pExpressItemBtn->setCount( pInfo->item_count );

		if ( _SetItemListString(i, true) == false )
		{
			_FailReset();
			return false;
		}
	}	

	return true;
}

CTString CUIExpressSystem::_GetSenderType( short SendType )
{
	CTString strType;
	// 등록 타입
	switch ( SendType )
	{
	case EXPRESS_SEND_TYPE_LOGIN:	
		strType.PrintF( _S( 1695, "보상") );
		break;

	case EXPRESS_SEND_TYPE_GM:
		strType.PrintF( _S( 100, "이벤트") );
		break;

	case EXPRESS_SEND_TYPE_TRADE_AGENT_NPC_BUY:		// 판매 대행 시스템 (구매완료)
		strType.PrintF( _S( 6056, "거래대행(구매완료)") );
		break;

	case EXPRESS_SEND_TYPE_TRADE_AGENT_NPC_SELL:		// 판매 대행 시스템 (판매완료)
		strType.PrintF( _S( 6057, "거래대행(판매완료)") );
		break;

	case EXPRESS_SEND_TYPE_TRADE_AGENT_NPC_CANCEL:		// 판매 대행 시스템 (판매취소)
		strType.PrintF( _S( 6059, "거래대행(판매취소)") );
		break;

	case EXPRESS_SEND_TYPE_TRADE_AGENT_NPC_RETURN:	// 판매 대행 시스템 (반송)
		strType.PrintF( _S( 6058, "거래대행(반송)") );
		break;

	case EXPRESS_SEND_TYPE_ETC:
		strType.PrintF( _S( 5247, "기타") );
		break;

	case EXPRESS_SEND_UNSEARCHABLE_STONESTATUS:
		strType.PrintF( _S( 6050, "구슬") );
		break;

	default:
		strType.PrintF(  _S( 5247, "기타") );
		break;
	}

	return strType;
}

void CUIExpressSystem::_ExpressRender()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	int i;
	for ( i = 0; i < m_nItemCount; i++ )
	{	
		if ( m_pExpressItemList[i] )
		{
			m_pExpressItemList[i]->Render();
		}
	}

	if (m_SelectItem_index >= 0 && m_pExpressItemList[m_SelectItem_index])
	{
		pDrawPort->InitTextureData( m_ptdLCETexture );

		m_rtsSelectFrame.m_RectSurfaceArray[FRAME_SELECT].m_RT = UIRect(m_nPosX + m_pExpressItemList[m_SelectItem_index]->GetPosX() - 1,
			m_nPosY + m_pExpressItemList[m_SelectItem_index]->GetPosY() - 1,
			m_nPosX + m_pExpressItemList[m_SelectItem_index]->GetPosX() + 714,
			m_nPosY + m_pExpressItemList[m_SelectItem_index]->GetPosY() + 33);

		m_rtsSelectFrame.RenderRectSurface(pDrawPort, 0xFFFFFFFF, FRAME_SELECT);

		pDrawPort->FlushRenderingQueue();
	}
}

//----------------------------------------------------------------------
// 체크 func

void CUIExpressSystem::_CheckExistMsgBox()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( MSGCMD_EXPRESS_RECVALL) 
		|| pUIManager->DoesMessageBoxExist( MSGCMD_EXPRESS_RECV) 
		|| pUIManager->DoesMessageBoxExist( MSGCMD_EXPRESS_DELETE) 
		|| pUIManager->DoesMessageBoxExist( MSGCMD_EXPRESS_DELETEALL) )
	{
		pUIManager->CloseMessageBox( MSGCMD_EXPRESS_RECVALL );
		pUIManager->CloseMessageBox( MSGCMD_EXPRESS_RECV );
		pUIManager->CloseMessageBox( MSGCMD_EXPRESS_DELETE );
		pUIManager->CloseMessageBox( MSGCMD_EXPRESS_DELETEALL );
	}
}

void CUIExpressSystem::_CreateMsgBox(eMsgType msgType)
{
	CTString strMessage, strTitle;
	CUIMsgBox_Info msgBoxInfo;
	CUIManager* pUIManager = CUIManager::getSingleton();

	strTitle.PrintF(_S( 191, "확인" ));

	switch( msgType )
	{
	case MSG_TYPE_RECV:
		{
			ExpressSystemItemInfo* pInfo = NULL;
			pInfo = m_pExPressData->getExpressListAt(m_SelectItem_index);

			if (pInfo == NULL)
				return;

			msgBoxInfo.SetMsgBoxInfo( strTitle, UMBS_OKCANCEL, UI_LCE_SYSTEM, MSGCMD_EXPRESS_RECV );

			if (pInfo->send_type == EXPRESS_SEND_TYPE_TRADE_AGENT_NPC_BUY)
			{
				if (pInfo->tradeagent_nas >= 0)
				{
					CTString strName = _pNetwork->GetItemName(pInfo->item_index);
	
					strMessage.PrintF( _S( 7041, "%s %d개를 %I64d 나스로 구입 하였습니다. 보관 중인 아이템..."),
						strName.str_String, pInfo->item_count, pInfo->tradeagent_nas);
				}
			}
			else if (pInfo->send_type == EXPRESS_SEND_TYPE_TRADE_AGENT_NPC_SELL)
			{
				if (pInfo->tradeagent_itemIndex >= 0 && pInfo->tradeagent_itemCount >= 0)
				{
					CTString strName = _pNetwork->GetItemName(pInfo->tradeagent_itemIndex);
	
					strMessage.PrintF( _S( 7040, "%s %d개를 판매하여 %I64d 나스를 보관 중입니다. 보관 중인 나스를..."),
						strName.str_String, pInfo->tradeagent_itemCount, pInfo->nas);
				}
			}
			
			if (strMessage.IsEmpty() == TRUE)
				strMessage.PrintF( _S( 6018, "보관 된 아이템을 받으시겠습니까?") );
		}
		break;

	case MSG_TYPE_RECVALL:
		{
			msgBoxInfo.SetMsgBoxInfo( strTitle, UMBS_OKCANCEL, UI_LCE_SYSTEM, MSGCMD_EXPRESS_RECVALL );
			strMessage.PrintF( _S( 6019, "해당 페이지에 존재하는 아이템을 받으시겠습니까?") );
			m_btnAllRecv.SetBtnState(UBS_DISABLE);
			m_btnAllDelete.SetBtnState(UBS_DISABLE);
		}
		break;

	case MSG_TYPE_DELETE:
		{
			msgBoxInfo.SetMsgBoxInfo( strTitle, UMBS_OKCANCEL, UI_LCE_SYSTEM, MSGCMD_EXPRESS_DELETE );
			strMessage.PrintF( _S( 6033, "보관 된 아이템을 버리시겠습니까?") );
		}
		break;

	case MSG_TYPE_DELETEALL:
		{
			msgBoxInfo.SetMsgBoxInfo( strTitle, UMBS_OKCANCEL, UI_LCE_SYSTEM, MSGCMD_EXPRESS_DELETEALL );
			strMessage.PrintF( _S( 6034, "해당 페이지에 존재하는 아이템을 버리시겠습니까?") );
			m_btnAllRecv.SetBtnState(UBS_DISABLE);
			m_btnAllDelete.SetBtnState(UBS_DISABLE);
		}
		break;

	default:
		return;
	}	
	msgBoxInfo.AddString(strMessage);	
	if ( pUIManager )
		pUIManager->CreateMessageBox( msgBoxInfo );

	m_btnRecv.SetBtnState(UBS_DISABLE);
	m_btnDelete.SetBtnState(UBS_DISABLE);
}

void CUIExpressSystem::_FailReset()
{
	if(m_pExPressData)
		m_pExPressData->ClearList();
	ClearExpress();
}

//----------------------------------------------------------------------
// Setting func

void CUIExpressSystem::_SetPage()
{
	UWORD nPage = 0;

	if (m_pExPressData && m_pExPressData->GetNextPage() > 0)
		m_btnNextPage.SetBtnState(UBS_IDLE);
	else
		m_btnNextPage.SetBtnState(UBS_DISABLE);
	
	if ( m_pExPressData )
	{
		nPage = m_pExPressData->GetCurrentPage();
	}

	m_strCurPage.PrintF( "%d", nPage + 1 );
	
	if( nPage > 0 )
		m_btnPrevPage.SetBtnState(UBS_IDLE);
	else
		m_btnPrevPage.SetBtnState(UBS_DISABLE);
}

void CUIExpressSystem::_SetBtnRect(int _nMiddleWidth)
{
	m_rcLeft.SetRect(0, 0, LCE_UI_INFO_BTN_LEFT_WIDTH, LCE_UI_INFO_BTN_SIZE_HEIGHT);
	m_rcMiddel.SetRect(LCE_UI_INFO_BTN_LEFT_WIDTH, 0, LCE_UI_INFO_BTN_LEFT_WIDTH + _nMiddleWidth, LCE_UI_INFO_BTN_SIZE_HEIGHT);
	m_rcRight.SetRect(_nMiddleWidth, 0, _nMiddleWidth + LCE_UI_INFO_BTN_RIGHT_WIDTH, LCE_UI_INFO_BTN_SIZE_HEIGHT);
}

int CUIExpressSystem::_GetBtnMiddleSize(CTString strText, int nCurrentSize)
{	
	int _nMiddleWidth = nCurrentSize;
	int _nTextSize = strText.Length() * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	if (_nMiddleWidth >= _nTextSize)
		return nCurrentSize;
	else
		return _nTextSize;
}
//------------------------------------------------------------------

void CUIExpressSystem::Create( CUIWindow* pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParent = pParentWnd;
	SetPos(nX, nY);
	SetSize(nWidth, nHeight);

	// Create Texture.
	m_ptdLCETexture = CreateTexture( CTString( "Data\\Interface\\LCESystem.tex" ) );

	// Get Texture size.
	FLOAT	fTexWidth = m_ptdLCETexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdLCETexture->GetPixHeight();
	
	// Set size of UI on this texture.
	m_rtsFrame.AddRectSurface( UIRect( 0, 0, nWidth, nHeight ), UIRectUV( 0, 0, LCE_UI_UV_FRAME_WIDTH, LCE_UI_UV_FRAME_HEIGHT, fTexWidth, fTexHeight ) );// Base

	m_rtsSelectFrame.AddRectSurface( UIRect(0, 0, 715,34 ), UIRectUV( 0, 492, 715, 526, fTexWidth, fTexHeight) ); // 선택

	m_rcTitle.SetRect( LCE_UI_INFO_TITLE_X, LCE_UI_INFO_TITLE_Y, LCE_UI_INFO_TITLE_WIDTH, LCE_UI_INFO_TITLE_HEIGHT );
	m_rcArea.SetRect( LCE_UI_INFO_MIDDLE_X, LCE_UI_INFO_MIDDLE_Y, LCE_UI_INFO_MIDDLE_WIDTH, LCE_UI_INFO_MIDDLE_HEIGHT );
	m_rcBottom.SetRect( LCE_UI_INFO_BOTTOM_X, LCE_UI_INFO_BOTTOM_Y, LCE_UI_INFO_BOTTOM_WIDTH, LCE_UI_INFO_BOTTOM_HEIGHT );

	// close button.
	m_btnClose.Create( this,CTString(""), LCE_UI_INFO_BTN_CLOSE_X, LCE_UI_INFO_BTN_CLOSE_Y, LCE_UI_INFO_BTN_CLOSE_SIZE, LCE_UI_INFO_BTN_CLOSE_SIZE );

	m_btnClose.SetUV(UBS_IDLE, LCE_UI_INFO_BTN_CLOSE_UV_OVER_X, LCE_UI_INFO_BTN_CLOSE_UV_OVER_Y, 
		LCE_UI_INFO_BTN_CLOSE_UV_OVER_WIDTH, LCE_UI_INFO_BTN_CLOSE_UV_OVER_HEIGHT,fTexWidth,fTexHeight);

	m_btnClose.SetUV(UBS_CLICK, LCE_UI_INFO_BTN_CLOSE_UV_CLICK_X, LCE_UI_INFO_BTN_CLOSE_UV_CLICK_Y,
		LCE_UI_INFO_BTN_CLOSE_UV_CLICK_WIDTH, LCE_UI_INFO_BTN_CLOSE_UV_CLICK_HEIGHT,fTexWidth,fTexHeight);

	// 버튼 위치 지정.
	m_rtIdleBtn.SetUV3( 
				LCE_UI_INFO_BTN_IDLE_LEFT_UV_X, LCE_UI_INFO_BTN_IDLE_LEFT_UV_Y, LCE_UI_INFO_BTN_IDLE_LEFT_UV_WIDTH, LCE_UI_INFO_BTN_IDLE_LEFT_UV_HEIGHT,
				LCE_UI_INFO_BTN_IDLE_MIDDLE_UV_X, LCE_UI_INFO_BTN_IDLE_MIDDLE_UV_Y, LCE_UI_INFO_BTN_IDLE_MIDDLE_UV_WIDTH, LCE_UI_INFO_BTN_IDLE_MIDDLE_UV_HEIGHT,
				LCE_UI_INFO_BTN_IDLE_RIGHT_UV_X, LCE_UI_INFO_BTN_IDLE_RIGHT_UV_Y, LCE_UI_INFO_BTN_IDLE_RIGHT_UV_WIDTH, LCE_UI_INFO_BTN_IDLE_RIGHT_UV_HEIGHT, fTexWidth, fTexHeight );

	m_rtClickBtn.SetUV3( 
				LCE_UI_INFO_BTN_CLICK_LEFT_UV_X, LCE_UI_INFO_BTN_CLICK_LEFT_UV_Y, LCE_UI_INFO_BTN_CLICK_LEFT_UV_WIDTH, LCE_UI_INFO_BTN_CLICK_LEFT_UV_HEIGHT,
				LCE_UI_INFO_BTN_CLICK_MIDDLE_UV_X, LCE_UI_INFO_BTN_CLICK_MIDDLE_UV_Y, LCE_UI_INFO_BTN_CLICK_MIDDLE_UV_WIDTH, LCE_UI_INFO_BTN_CLICK_MIDDLE_UV_HEIGHT,
				LCE_UI_INFO_BTN_CLICK_RIGHT_UV_X, LCE_UI_INFO_BTN_CLICK_RIGHT_UV_Y, LCE_UI_INFO_BTN_CLICK_RIGHT_UV_WIDTH, LCE_UI_INFO_BTN_CLICK_RIGHT_UV_HEIGHT, fTexWidth, fTexHeight);

	m_rtDisableBtn.SetUV3( 
				LCE_UI_INFO_BTN_DISABLE_LEFT_UV_X, LCE_UI_INFO_BTN_DISABLE_LEFT_UV_Y, LCE_UI_INFO_BTN_DISABLE_LEFT_UV_WIDTH, LCE_UI_INFO_BTN_DISABLE_LEFT_UV_HEIGHT,
				LCE_UI_INFO_BTN_DISABLE_MIDDLE_UV_X, LCE_UI_INFO_BTN_DISABLE_MIDDLE_UV_Y, LCE_UI_INFO_BTN_DISABLE_MIDDLE_UV_WIDTH, LCE_UI_INFO_BTN_DISABLE_MIDDLE_UV_HEIGHT,
				LCE_UI_INFO_BTN_DISABLE_RIGHT_UV_X, LCE_UI_INFO_BTN_DISABLE_RIGHT_UV_Y, LCE_UI_INFO_BTN_DISABLE_RIGHT_UV_WIDTH, LCE_UI_INFO_BTN_DISABLE_RIGHT_UV_HEIGHT,fTexWidth, fTexHeight);
	///////////////////// 받기 버튼 /////////////////////////////////////
	CTString strText = _S( 6014, "받기");
	int _nMiddleWidth = _GetBtnMiddleSize(strText, LCE_UI_INFO_BTN_MIDDLE_WIDTH);
	_SetBtnRect(_nMiddleWidth);
	int _nRecvWidth = _nMiddleWidth + LCE_UI_INFO_BTN_LEFT_WIDTH + LCE_UI_INFO_BTN_RIGHT_WIDTH;

	m_btnRecv.Create( this, strText, LCE_UI_INFO_BTN_RECV_START_X - _nRecvWidth, LCE_UI_INFO_BTN_RECV_START_Y, _nRecvWidth , LCE_UI_INFO_BTN_SIZE_HEIGHT );
	m_btnRecv.SetRTSurface( UBS_IDLE, m_rcLeft, m_rtIdleBtn.rtL );
	m_btnRecv.SetRTSurface( UBS_IDLE, m_rcMiddel, m_rtIdleBtn.rtM );
	m_btnRecv.SetRTSurface( UBS_IDLE, m_rcRight, m_rtIdleBtn.rtR );
	m_btnRecv.SetRTSurface( UBS_CLICK, m_rcLeft, m_rtClickBtn.rtL );
	m_btnRecv.SetRTSurface( UBS_CLICK, m_rcMiddel, m_rtClickBtn.rtM );
	m_btnRecv.SetRTSurface( UBS_CLICK, m_rcRight, m_rtClickBtn.rtR );
	m_btnRecv.SetRTSurface( UBS_DISABLE, m_rcLeft, m_rtDisableBtn.rtL );
	m_btnRecv.SetRTSurface( UBS_DISABLE, m_rcMiddel, m_rtDisableBtn.rtM );
	m_btnRecv.SetRTSurface( UBS_DISABLE, m_rcRight, m_rtDisableBtn.rtR );
	m_btnRecv.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnRecv.SetNewType(TRUE);
	m_btnRecv.SetEnable(TRUE);
	m_btnRecv.SetBtnState(UBS_DISABLE);
	///////////////////// 모두 받기 버튼 /////////////////////////////////////
	strText = _S( 6015, "모두 받기");
	_nMiddleWidth = _GetBtnMiddleSize(strText, LCE_UI_INFO_BTN_MIDDLE_WIDTH);
	_SetBtnRect(_nMiddleWidth);
	int _nRecvAllWidth = _nMiddleWidth + LCE_UI_INFO_BTN_LEFT_WIDTH + LCE_UI_INFO_BTN_RIGHT_WIDTH;
	int nRecvAllBtnStartX = m_btnRecv.GetPosX() - _nRecvWidth - LCE_UI_INFO_BTN_GAP;

	m_btnAllRecv.Create( this, strText, nRecvAllBtnStartX, LCE_UI_INFO_BTN_RECV_START_Y, _nRecvAllWidth , LCE_UI_INFO_BTN_SIZE_HEIGHT );
	m_btnAllRecv.SetRTSurface( UBS_IDLE, m_rcLeft, m_rtIdleBtn.rtL );
	m_btnAllRecv.SetRTSurface( UBS_IDLE, m_rcMiddel, m_rtIdleBtn.rtM );
	m_btnAllRecv.SetRTSurface( UBS_IDLE, m_rcRight, m_rtIdleBtn.rtR );
	m_btnAllRecv.SetRTSurface( UBS_CLICK, m_rcLeft, m_rtClickBtn.rtL );
	m_btnAllRecv.SetRTSurface( UBS_CLICK, m_rcMiddel, m_rtClickBtn.rtM );
	m_btnAllRecv.SetRTSurface( UBS_CLICK, m_rcRight, m_rtClickBtn.rtR );
	m_btnAllRecv.SetRTSurface( UBS_DISABLE, m_rcLeft, m_rtDisableBtn.rtL );
	m_btnAllRecv.SetRTSurface( UBS_DISABLE, m_rcMiddel, m_rtDisableBtn.rtM );
	m_btnAllRecv.SetRTSurface( UBS_DISABLE, m_rcRight, m_rtDisableBtn.rtR );
	m_btnAllRecv.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnAllRecv.SetNewType(TRUE);
	m_btnAllRecv.SetEnable(TRUE);
	m_btnAllRecv.SetBtnState(UBS_DISABLE);
	/////////////////// 모두 버리기 버튼 ////////////////////////////////////////
	strText = _S( 6036, "모두 버리기");
	_nMiddleWidth = _GetBtnMiddleSize(strText, LCE_UI_INFO_BTN_MIDDLE_WIDTH);
	_SetBtnRect(_nMiddleWidth);
	int _nDeleteAllWidth = _nMiddleWidth + LCE_UI_INFO_BTN_LEFT_WIDTH + LCE_UI_INFO_BTN_RIGHT_WIDTH;
	
	m_btnAllDelete.Create( this, strText, LCE_UI_INFO_BTN_DELETE_ALL_START_X, LCE_UI_INFO_BTN_DELETE_ALL_START_Y, _nDeleteAllWidth, LCE_UI_INFO_BTN_SIZE_HEIGHT );
	m_btnAllDelete.SetRTSurface( UBS_IDLE, m_rcLeft, m_rtIdleBtn.rtL );
	m_btnAllDelete.SetRTSurface( UBS_IDLE, m_rcMiddel, m_rtIdleBtn.rtM );
	m_btnAllDelete.SetRTSurface( UBS_IDLE, m_rcRight, m_rtIdleBtn.rtR );
	m_btnAllDelete.SetRTSurface( UBS_CLICK, m_rcLeft, m_rtClickBtn.rtL );
	m_btnAllDelete.SetRTSurface( UBS_CLICK, m_rcMiddel, m_rtClickBtn.rtM );
	m_btnAllDelete.SetRTSurface( UBS_CLICK, m_rcRight, m_rtClickBtn.rtR );
	m_btnAllDelete.SetRTSurface( UBS_DISABLE, m_rcLeft, m_rtDisableBtn.rtL );
	m_btnAllDelete.SetRTSurface( UBS_DISABLE, m_rcMiddel, m_rtDisableBtn.rtM );
	m_btnAllDelete.SetRTSurface( UBS_DISABLE, m_rcRight, m_rtDisableBtn.rtR );
	m_btnAllDelete.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnAllDelete.SetNewType(TRUE);
	m_btnAllDelete.SetEnable(TRUE);
	m_btnAllDelete.SetBtnState(UBS_DISABLE);
	/////////////////// 버리기 버튼 ////////////////////////////////////////
	strText = _S( 6035, "버리기");
	_nMiddleWidth = _GetBtnMiddleSize(strText, LCE_UI_INFO_BTN_MIDDLE_WIDTH);
	_SetBtnRect(_nMiddleWidth);
	int nDeleteBtnStartX = m_btnAllDelete.GetPosX() + _nDeleteAllWidth + LCE_UI_INFO_BTN_GAP;
	int nDeleteBtnWidth = _nMiddleWidth + LCE_UI_INFO_BTN_LEFT_WIDTH + LCE_UI_INFO_BTN_RIGHT_WIDTH;

	m_btnDelete.Create( this, strText, nDeleteBtnStartX, LCE_UI_INFO_BTN_DELETE_ALL_START_Y, nDeleteBtnWidth, LCE_UI_INFO_BTN_SIZE_HEIGHT );
	m_btnDelete.SetRTSurface( UBS_IDLE, m_rcLeft, m_rtIdleBtn.rtL );
	m_btnDelete.SetRTSurface( UBS_IDLE, m_rcMiddel, m_rtIdleBtn.rtM );
	m_btnDelete.SetRTSurface( UBS_IDLE, m_rcRight, m_rtIdleBtn.rtR );
	m_btnDelete.SetRTSurface( UBS_CLICK, m_rcLeft, m_rtClickBtn.rtL );
	m_btnDelete.SetRTSurface( UBS_CLICK, m_rcMiddel, m_rtClickBtn.rtM );
	m_btnDelete.SetRTSurface( UBS_CLICK, m_rcRight, m_rtClickBtn.rtR );
	m_btnDelete.SetRTSurface( UBS_DISABLE, m_rcLeft, m_rtDisableBtn.rtL );
	m_btnDelete.SetRTSurface( UBS_DISABLE, m_rcMiddel, m_rtDisableBtn.rtM );
	m_btnDelete.SetRTSurface( UBS_DISABLE, m_rcRight, m_rtDisableBtn.rtR );
	m_btnDelete.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnDelete.SetNewType(TRUE);
	m_btnDelete.SetEnable(TRUE);
	m_btnDelete.SetBtnState(UBS_DISABLE);

	m_btnNextPage.Create( this, CTString(""), 396, 444, 11, 19);
	m_btnNextPage.SetUV( UBS_ON, 137, 562, 148, 581, fTexWidth, fTexHeight );
	m_btnNextPage.SetUV( UBS_CLICK, 137, 582, 148, 601, fTexWidth, fTexHeight );
	m_btnNextPage.CopyUV( UBS_ON, UBS_IDLE );
	m_btnNextPage.CopyUV( UBS_ON, UBS_DISABLE );
	m_btnNextPage.SetEnable(TRUE);
	m_btnNextPage.SetBtnState(UBS_DISABLE);

	m_btnPrevPage.Create( this, CTString(""), 333, 444, 11, 19);
	m_btnPrevPage.SetUV( UBS_ON, 119, 562, 130, 581, fTexWidth, fTexHeight );
	m_btnPrevPage.SetUV( UBS_CLICK, 119, 582, 130, 601, fTexWidth, fTexHeight );
	m_btnPrevPage.CopyUV( UBS_ON, UBS_IDLE );
	m_btnPrevPage.CopyUV( UBS_ON, UBS_DISABLE );
	m_btnPrevPage.SetEnable(TRUE);
	m_btnPrevPage.SetBtnState(UBS_DISABLE);

	int nStartX = LCE_UI_ITEM_BTN_START_X;
	int nStartY = LCE_UI_ITEM_BTN_START_Y;

	for (int i = 0; i < MAX_ITEM_LIST; i++)
	{
		if (m_pExpressItemList[i] == NULL)
		{
			m_pExpressItemList[i] = new CUIExpressItemList;
			m_pExpressItemList[i]->Create(this, nStartX, nStartY, LCE_UI_ITEM_LIST_WIDTH, LCE_UI_ITEM_LIST_HEIGHT);
			nStartY += LCE_UI_ITEM_BTN_LIST_Y_GEP;
		}
	}
	m_strTitle.PrintF( _S( 6037, "신비한 석상") );
	m_strCulumName.PrintF( _S( 4288, "아이템" ) );
	m_strCulumQuantity.PrintF( _S( 2396, "수량" ) );
	m_strCulumAmount.PrintF( _S( 4289, "총액" ) );
	m_strCulumSender.PrintF( _S( 6016, "발신자" ) );
	m_strCulumType.PrintF( _S( 5964, "종류" ) );
	m_strCulumTime.PrintF( _S( 1718, "시간" ) );
}

void CUIExpressSystem::Render()
{
	if (IsVisible() && m_pExPressData)
	{
		if (m_pExPressData->CheckDistance())
			CloseExpressSystem();
	}

	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();
	// info is view
	pDrawPort->InitTextureData( m_ptdLCETexture );
	m_rtsFrame.SetPos( m_nPosX, m_nPosY );
	m_rtsFrame.RenderRectSurface( pDrawPort, 0xFFFFFFFF );

	m_btnClose.Render();
	m_btnRecv.Render();
	m_btnAllRecv.Render();
	m_btnDelete.Render();
	m_btnAllDelete.Render();

	if( m_pExPressData && m_pExPressData->GetNextPage() > 0 )
		m_btnNextPage.Render();

	if ( m_pExPressData && m_pExPressData->GetCurrentPage() > 0)
		m_btnPrevPage.Render();

	pDrawPort->FlushRenderingQueue();
	
	pDrawPort->PutTextExCX(m_strTitle, 
		m_nPosX + TITLE_STRING_POSX + (TITLE_STRING_WIDTH / 2), m_nPosY + TITLE_STRING_POSY, 0xFFFFFFFF);
	pDrawPort->PutTextExCX(m_strCulumName, 
		m_nPosX + LCE_UI_ITEM_LIST_COLUM_ITEM_NAME + (CULUM_X_WIDTH_ITME_NAME/2), m_nPosY + LCE_UI_ITEM_LIST_COLUM_START_Y, 0xFFFFFFFF);
	pDrawPort->PutTextExCX(m_strCulumQuantity, 
		m_nPosX + LCE_UI_ITEM_LIST_COLUM_QUANTITY + (CULUM_X_WIDTH_QUANTITY/2), m_nPosY + LCE_UI_ITEM_LIST_COLUM_START_Y, 0xFFFFFFFF);
	pDrawPort->PutTextExCX(m_strCulumAmount,
		m_nPosX + LCE_UI_ITEM_LIST_COLUM_AMOUNT  + (CULUM_X_WIDTH_AMOUNT/2), m_nPosY + LCE_UI_ITEM_LIST_COLUM_START_Y, 0xFFFFFFFF);
	pDrawPort->PutTextExCX(m_strCulumSender, 
		m_nPosX + LCE_UI_ITEM_LIST_COLUM_SENDER  + (CULUM_X_WIDTH_SENDER/2), m_nPosY + LCE_UI_ITEM_LIST_COLUM_START_Y, 0xFFFFFFFF);
	pDrawPort->PutTextExCX(m_strCulumType, 
		m_nPosX + LCE_UI_ITEM_LIST_COLUM_TYPE  + (CULUM_X_WIDTH_TYPE/2), m_nPosY + LCE_UI_ITEM_LIST_COLUM_START_Y, 0xFFFFFFFF);
	pDrawPort->PutTextExCX(m_strCulumTime,
		m_nPosX + LCE_UI_ITEM_LIST_COLUM_TIME  + (CULUM_X_WIDTH_TIME/2), m_nPosY + LCE_UI_ITEM_LIST_COLUM_START_Y, 0xFFFFFFFF);
	
	pDrawPort->PutTextExCX(m_strCurPage, m_nPosX + 348  + (42/2), m_nPosY + 447, 0xFFFFFFFF);

	pDrawPort->EndTextEx();

 	if(m_bItemList)
 		_ExpressRender();

// 	pDrawPort->FlushRenderingQueue();
// 	pDrawPort->EndTextEx();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void CUIExpressSystem::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void CUIExpressSystem::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

WMSG_RESULT CUIExpressSystem::MouseMessage( MSG* pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD(pMsg->lParam);
	int	nY = HIWORD(pMsg->lParam);
	// Title bar
	static BOOL bTitleBarClick = FALSE;
	// Extended button clicked
	static BOOL	bLButtonDownInBtn = FALSE;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch (pMsg->message)
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				pUIManager->SetMouseCursorInsideUIs();

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( ( wmsgResult = m_btnRecv.MouseMessage( pMsg ) ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( ( wmsgResult = m_btnAllRecv.MouseMessage( pMsg ) ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( ( wmsgResult = m_btnDelete.MouseMessage( pMsg ) ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( ( wmsgResult = m_btnAllDelete.MouseMessage( pMsg ) ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( ( wmsgResult = m_btnNextPage.MouseMessage( pMsg ) ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( ( wmsgResult = m_btnPrevPage.MouseMessage( pMsg ) ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else
			{
				if ( IsInsideRect(nX, nY, m_rcArea) )
				{
					for (int i = 0; i < MAX_ITEM_LIST; i++)
					{
						if (m_pExpressItemList[i])
							m_pExpressItemList[i]->MouseMessage( pMsg );	
					}
					return WMSG_SUCCESS;
				}				
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;
				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )			{}
				else if( ( wmsgResult = m_btnRecv.MouseMessage( pMsg ) ) != WMSG_FAIL )		{}
				else if( ( wmsgResult = m_btnAllRecv.MouseMessage( pMsg ) ) != WMSG_FAIL )	{}
				else if( ( wmsgResult = m_btnDelete.MouseMessage( pMsg ) ) != WMSG_FAIL )	{}
				else if( ( wmsgResult = m_btnAllDelete.MouseMessage( pMsg ) ) != WMSG_FAIL ){}
				else if( ( wmsgResult = m_btnNextPage.MouseMessage( pMsg ) ) != WMSG_FAIL ) {}
				else if( ( wmsgResult = m_btnPrevPage.MouseMessage( pMsg ) ) != WMSG_FAIL ) {}
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				else if( IsInsideRect(nX, nY, m_rcArea) )
				{
					for (int i = 0; i < MAX_ITEM_LIST; i++)
					{
						if(m_pExpressItemList[i] && m_pExpressItemList[i]->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							pUIManager->RearrangeOrder( UI_LCE_SYSTEM, TRUE );
							return WMSG_SUCCESS;
						}							
					}					
				}
				pUIManager->RearrangeOrder( UI_LCE_SYSTEM, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			bTitleBarClick = FALSE;

			if ( IsInside( nX, nY ) )
			{
				if (pUIManager->GetDragIcon() == NULL)
				{
					if( !IsFocused() )
						return WMSG_FAIL;

					// Close button
					if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							CloseExpressSystem();
						}
						return WMSG_SUCCESS;
					}
					else if( ( wmsgResult = m_btnRecv.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							if (m_bItemList && m_SelectItem_index >= 0 && m_pExpressItemList[m_SelectItem_index])
							{
								_CheckExistMsgBox();

								_CreateMsgBox(MSG_TYPE_RECV);
							}
						}
						return WMSG_SUCCESS;
					}
					else if( ( wmsgResult = m_btnAllRecv.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							if (m_bItemList && m_nItemCount)
							{
								_CheckExistMsgBox();

								_CreateMsgBox(MSG_TYPE_RECVALL);
							}
						}
						return WMSG_SUCCESS;
					}
					else if( ( wmsgResult = m_btnDelete.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							if (m_bItemList && m_SelectItem_index >= 0 && m_pExpressItemList[m_SelectItem_index])
							{
								_CheckExistMsgBox();

								_CreateMsgBox(MSG_TYPE_DELETE);
							}
						}
						return WMSG_SUCCESS;
					}
					else if( ( wmsgResult = m_btnAllDelete.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							if (m_bItemList && m_nItemCount)
							{
								_CheckExistMsgBox();

								_CreateMsgBox(MSG_TYPE_DELETEALL);
							}
						}
						return WMSG_SUCCESS;
					}
					else if( ( wmsgResult = m_btnNextPage.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							if (m_pExPressData && m_pExPressData->GetNextPage() > 0)
							{
								m_pExPressData->AddCurrnetPage();
								m_pExPressData->SendListReq();
								m_btnNextPage.SetBtnState(UBS_DISABLE);
							}						
							// 다음 페이지
						}
						return WMSG_SUCCESS;
					}
					else if( ( wmsgResult = m_btnPrevPage.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							if (m_pExPressData->GetCurrentPage() > 0)
							{
								m_pExPressData->SubCurrnetPage();
								m_pExPressData->SendListReq();
								m_btnPrevPage.SetBtnState(UBS_DISABLE);
							}	
							// 이전 페이지
						}
						return WMSG_SUCCESS;
					}
					else
					{
						if( IsInsideRect(nX, nY, m_rcArea) )
						{
							for (int i = 0; i < MAX_ITEM_LIST; i++)
							{
								if( m_pExpressItemList[i] && m_pExpressItemList[i]->MouseMessage( pMsg ) != WMSG_FAIL )
								{
									// 선택
									m_SelectItem_index = i;
									m_btnRecv.SetBtnState( UBS_IDLE );
									m_btnDelete.SetBtnState( UBS_IDLE );
									return WMSG_SUCCESS;
								}							
							}
						}
						return WMSG_SUCCESS;
					}

				}
				pUIManager->ResetHoldBtn();
			}
			
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if ( IsInside( nX, nY ) )
			{
				if( IsInsideRect(nX, nY, m_rcArea) )
				{
					if (m_bItemList && m_SelectItem_index >= 0 && m_pExpressItemList[m_SelectItem_index])
					{
						_CheckExistMsgBox();
						_CreateMsgBox(MSG_TYPE_RECV);
					}
				}
			}
		}
		break;
	}

	return WMSG_FAIL;
}

void CUIExpressSystem::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	if(!m_bItemList)
		return;
	if (m_nItemCount <= 0)
		return;

	if (bOK == TRUE)
	{
		switch ( nCommandCode )
		{
		case  MSGCMD_EXPRESS_RECV:
			{
				if (m_SelectItem_index < 0 || !m_pExpressItemList[m_SelectItem_index] )
					return;

				int nIndex = m_pExpressItemList[m_SelectItem_index]->GetBtn()->getItems()->Item_UniIndex;
				m_pExPressData->SendRecvReq(nIndex);				
			}
			break;

		case  MSGCMD_EXPRESS_DELETE:
			{
				if (m_SelectItem_index < 0 || !m_pExpressItemList[m_SelectItem_index] )
					return;

				int nIndex = m_pExpressItemList[m_SelectItem_index]->GetBtn()->getItems()->Item_UniIndex;
				m_pExPressData->SendDeleteReq(nIndex);				
			}
			break;

		case  MSGCMD_EXPRESS_RECVALL:
			{
				for (int i = 0; i < m_nItemCount; i++)
				{
					if ( m_pExpressItemList[i] != NULL )
					{
						if(m_pExpressItemList[i]->GetBtn()->IsEmpty())
							continue;

						m_pExPressData->SendRecvReq(m_pExpressItemList[i]->GetBtn()->getItems()->Item_UniIndex);
					}
				}
			}
			break;

		case  MSGCMD_EXPRESS_DELETEALL:
			{
				for (int i = 0; i < m_nItemCount; i++)
				{
					if ( m_pExpressItemList[i] != NULL )
					{
						if(m_pExpressItemList[i]->GetBtn()->IsEmpty())
							continue;

						m_pExPressData->SendDeleteReq(m_pExpressItemList[i]->GetBtn()->getItems()->Item_UniIndex);
					}
				}
			}
		}

		m_bItemList = false;
		m_SelectItem_index = -1;
	}
	else // bOK == FALSE
	{
		if (m_SelectItem_index >= 0)
		{
			m_btnRecv.SetBtnState(UBS_IDLE);
			m_btnDelete.SetBtnState(UBS_IDLE);
		}
		if (m_bItemList)
		{
			m_btnAllRecv.SetBtnState(UBS_IDLE);
			m_btnAllDelete.SetBtnState(UBS_IDLE);
		}
	}
}

BOOL CUIExpressSystem::CloseWindowByEsc()
{
	CloseExpressSystem();
	return TRUE;
}



