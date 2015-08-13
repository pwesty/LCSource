#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <vector>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIQuestBook.h>
#include <Engine/Interface/UIPetTraining.h>
#include <Engine/Interface/UIShop.h>
#include <Engine/Interface/UIInventory.h>
#include <algorithm>
#include <Engine/Interface/UIQuest.h>
#include <Engine/Interface/UIPortal.h>
#include <Engine/Interface/UISiegeWarfare.h>
#include <Engine/Interface/UIWareHouse.h>
#include <Engine/Interface/UIGuild.h>
#include <Engine/Interface/UIInitJob.h>
#include <Engine/Interface/UIRefine.h>
#include <Engine/Interface/UIChangeWeapon.h>
#include <Engine/Interface/UIPlayerInfo.h>
#include <Engine/Interface/UIRemission.h>
#include <Engine/Interface/UIProcessNPC.h>
#include <Engine/Interface/UISkillLearn.h>
#include <Engine/Interface/UIProduct.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/Quest.h>
#include <Engine/Contents/Base/UIQuestNew.h>

static int	_iMaxDescStringChar = 0;
static int	_iMaxSelStringChar = 0;
static int	_nMsgBoxLineHeight = 0;
static int	_iMaxDescStringCharNew = 0;				// [090601: selo] 신규 퀘스트 북을 위한 한 열에 들어가는 문자열 최대 길이
static int  _iMaxSelStringCharNew = 0;				// [090601: selo] 신규 퀘스트 북을 위한 한 열에 들어가는 선택된 문자열 최대 길이
#define MAX_QUEST		16

extern INDEX g_iXPosInQuest;
extern INDEX g_iYPosInQuest;

const int iQuestListTitleTextOffsetX	= 5;		// 타이틀 Text X 오프셋
const int iQuestListTitleTextOffsetY	= 25;		// 타이틀 Text Y 오프셋
const int iQuestListTopHeight			= 26;
const int iQuestListDescHeight			= 164;
const int iQuestListBottomHeight		= 7;
const int iQuestListWidth				= 475;		// UI 전체 너비
const int iQuestListHeight				= 331;		// UI 전체 높이

const int iQuestListTitleHeight			= 36;		// 타이틀 높이
const CTString strQuestListTexFileName  = "Data\\Interface\\QuestBook.tex";
const CTString strCommonBtnTexFileName  = "Data\\Interface\\CommonBtn.tex";

const int iQuestNoticeCnt				= 5;

enum __tag_QUEST_TUTORIAL_RESPONSE
{
	RESPONSE_OK = 0,
	RESPONSE_CALCEL,
};

// ----------------------------------------------------------------------------
// Name : CUIQuestBook()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIQuestBook::CUIQuestBook( int nUIIndex )
: m_nUIIndex( nUIIndex )
{
	m_bTitleBarClick		= FALSE;
	m_nSelectedQuestIndex	= -1;
	m_nTargetIndex			= -1;
	m_nTargetVirIndex		= -1;
	m_nTargetUIType			= -1;	
	m_pQuestItem			= NULL;
	m_bProceedListExtended	= TRUE;
	m_bCompleteListExtended	= TRUE;	
	m_bRaidMessageListExtended = TRUE;
	m_nPrevNPCIndex			= -1;
	m_nPrevNPCUIType		= -1;
	m_nSortType				= 0;
	m_bLockQuestList		= FALSE;
	m_dQuestNoticeStartTime = 0;
	m_dRaidRemainTime		= 0;		// [090907: selo] 레이드 남은 시간 초기화
	m_bIsRaidNow			= FALSE;	// [090907: selo] 레이드 중인지 플래그 초기화	
	m_dRaidRemainTimeRecvTime = 0;		// [090908: selo] 레이드 남은 시간 받은 시간 초기화
	
	for( int i = 0; i < QCATEGORY_END; ++i )
		m_bCategoryListExtended[i] = TRUE;

	m_ptdCommonBtnTexture = NULL;
}

// ----------------------------------------------------------------------------
// Name : ~CUIQuestBook()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIQuestBook::~CUIQuestBook()
{
	Destroy();

	STOCK_RELEASE(m_ptdCommonBtnTexture);
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CreateQuestListNew(pParentWnd, nX, nY, nWidth, nHeight);
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	switch(m_nUIIndex)
	{
	case UI_QUESTBOOK_LIST:
		{
			SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2 - 44, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 - 44 );
		}break;
	}
}
void CUIQuestBook::ResetSavePosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );

	g_iXPosInQuest = GetPosX();
	g_iYPosInQuest = GetPosY();
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : ToggleVisible()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::ToggleVisible()
{
	if( IsVisible() )
	{
		g_iXPosInQuest = GetPosX();
		g_iYPosInQuest = GetPosY();
		CloseQuestBook();
	}
	else
	{
		SetPos( g_iXPosInQuest, g_iYPosInQuest );
		OpenQuestBook();
	}
}

// ----------------------------------------------------------------------------
// Name : OpenQuestBook()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::OpenQuestBook( int iQuestIndex, CItems* pQuestItem )
{	
	switch(m_nUIIndex)
	{
	case UI_QUESTBOOK_LIST:
		{
			m_strTitle	= _S( 1700, "퀘스트 북" );	
		}break;
	}

	if( IsVisible() && m_nUIIndex == UI_QUESTBOOK_LIST )
		return;//이 함수 대신 RefreshQuestList를 사용
	// 퀘스트 인덱스가 변화가 없는데, 보여지는 상태라면...
	if( IsVisible() && m_nSelectedQuestIndex == iQuestIndex )
		return;

	if( ( m_nUIIndex != UI_QUESTBOOK_LIST ) )
		InitQuestBook();

	if (iQuestIndex > 0)
		m_nSelectedQuestIndex	= iQuestIndex;

	// 퀘스트 목록
	if( m_nUIIndex == UI_QUESTBOOK_LIST )
	{
		m_btnCancel.SetEnable( FALSE ); // 퀘스트 목록에서 닫기 버튼을 없애고, 보상 버튼으로 바꿈
		m_lbDescription.SetEnable( TRUE );
		m_lbQuestList.SetEnable( TRUE );

		RefreshQuestList();
	}

	CQuestSystem::Instance().DeleteCurrentRequest();
	CUIManager::getSingleton()->RearrangeOrder( m_nUIIndex, TRUE );
}

// ----------------------------------------------------------------------------
// Name : RefreshQuestContent()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::RefreshQuestContent(INDEX questIndex)
{
	m_btnGiveUp.SetEnable(TRUE);
	if( isRaidMessage(questIndex) )
	{
		RefreshQuestContentByRaid(questIndex);
		return;
	}

	if(questIndex != m_nSelectedQuestIndex)
		return;
	int oldSbPos = m_lbDescription.GetScrollBarPos();
	m_lbDescription.Reset();
	m_lbDescription.ResetAllStrings();

	CQuestDynamicData *pQuestDD = NULL;
	if( !isRaidMessage(questIndex) )
	{
		pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex(questIndex);	
	}
	else
	{
		CQuestDynamicData qdd(CQuestSystem::Instance().GetStaticData(questIndex));
		pQuestDD = &qdd;	
	}

	
	// 2009. 06. 01 김정래
	// 확장팩 퀘스트 북 변경 처리에 의한 설명글의 순서 변경
	AddDescString( pQuestDD->GetTitleDesc( 0 ), pQuestDD->GetColorTitleDesc( 0 ) );	
	AddDescString( CTString("\n"), 0xFFFFFFFF );		
	AddDescString( CTString(pQuestDD->GetDesc3()), 0x34B603FF );
	AddDescString( CTString("\n"), 0xFFFFFFFF );
	if(pQuestDD->GetCountStatusDesc() > 0)
		AddDescString( _S( 1703, "\n진행상황" ), 0xFFFFFFFF );
	for(INDEX i=0; i<pQuestDD->GetCountStatusDesc(); ++i)
		AddDescString( pQuestDD->GetStatusDesc( i ), pQuestDD->GetColorStatusDesc( i ) );
	
	CTString strSpace = CTString("     ");
#if defined(G_RUSSIA)
		strSpace = CTString("        ");
#endif
	//고정보상
	if(pQuestDD->IsPrizeExist())
	{
		AddDescString( CTString("\n"), 0xFFFFFFFF );
		AddDescString( pQuestDD->GetPrizeDesc( 0 ), pQuestDD->GetColorPrizeDesc( 0 ) );
		for( INDEX i = 1; i < pQuestDD->GetCountPrizeDesc(); ++i )
		{
			CTString strPrize = pQuestDD->GetPrizeDesc( i );
			strPrize = strSpace + strPrize;				

			// [090812: selo] 보상 설명 텍스트 처리 변경
			bool bMultiLine = AddDescString( strPrize, pQuestDD->GetColorPrizeDesc( i ), FALSE, TRUE );
			if( !bMultiLine )
				AddDescString( strSpace, pQuestDD->GetColorPrizeDesc( i ) );

			m_lbDescription.SetImageBox(m_lbDescription.GetCurItemCount(0)-2
				, CUIImageBox::eImageType(pQuestDD->GetPrizeType(i-1))
				, pQuestDD->GetPrizeIndex(i-1), 0, CTString(""), pQuestDD->GetSyndicateType());
		}
	}

	//선택 보상
	if(pQuestDD->IsOptionPrizeExist())
	{
		AddDescString( CTString("\n"), 0xFFFFFFFF );
		AddDescString( pQuestDD->GetOptionPrizeDesc( 0 ), pQuestDD->GetColorOptionPrizeDesc( 0 ) );
		for( INDEX i = 1; i < pQuestDD->GetCountOptionPrizeDesc(); ++i )
		{
			CTString strPrize = pQuestDD->GetOptionPrizeDesc( i );
			strPrize = strSpace + strPrize;

			// [090812: selo] 보상 설명 텍스트 처리 변경
			bool bMultiLine = AddDescString( strPrize, pQuestDD->GetColorOptionPrizeDesc( i ), FALSE, TRUE );
			if( !bMultiLine )
				AddDescString( strSpace, pQuestDD->GetColorOptionPrizeDesc( i ) );

			m_lbDescription.SetImageBox(m_lbDescription.GetCurItemCount(0)-2
				, CUIImageBox::eImageType(pQuestDD->GetOptionPrizeType(i-1))
				, pQuestDD->GetOptionPrizeIndex(i-1), 0, CTString(""), pQuestDD->GetSyndicateType());
		}
	}

	AddDescString( CTString("\n"), 0xFFFFFFFF );
	AddDescString( CTString(pQuestDD->GetDesc()), pQuestDD->GetColorIntroDesc() );
	AddDescString( _S( 1704, "수행가능 레벨" ), 0xFFFFFFFF );
	CTString strTemp;
	if(pQuestDD->GetNeedMinLevel() == pQuestDD->GetNeedMaxLevel())
	{
		strTemp.PrintF(_S( 1705, "레벨 %d" ), pQuestDD->GetNeedMinLevel());
	}
	else if(pQuestDD->GetNeedMaxLevel()==999)
	{
		strTemp.PrintF(_S( 5667, "레벨 : %d ~ MAX" ), pQuestDD->GetNeedMinLevel());
	}
	else
	{
		strTemp.PrintF(_S( 1706, "레벨 %d ~ 레벨 %d" ), pQuestDD->GetNeedMinLevel(), pQuestDD->GetNeedMaxLevel());
	}
	AddDescString( strTemp, 0xD0D0FFFF );

	m_lbDescription.SetScrollBarPos(oldSbPos);
	if (IsCompleteQuest(pQuestDD->GetQuestIndex()) && pQuestDD->GetPrizeNPCIndex() == 0)
		m_btnCancel.SetEnable(TRUE); // 보상 버튼 활성화
	else
		m_btnCancel.SetEnable(FALSE);
}

// ----------------------------------------------------------------------------
// Name : InitQuestBook()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::InitQuestBook()
{
	m_nSelectedQuestIndex	= -1;
	m_nTargetIndex			= -1;
	m_nTargetUIType			= -1;
	m_lbDescription.Reset();
	m_lbDescription.ResetAllStrings();
	m_lbQuestList.Reset();
	m_lbQuestList.ResetAllStrings();
	m_pQuestItem = NULL;
}

// ----------------------------------------------------------------------------
// Name : CloseQuestBook()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::CloseQuestBook()
{
	InitQuestBook();
	if(m_nUIIndex != UI_QUESTBOOK_LIST) ClearQuestList();

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_QUEST );

	g_iXPosInQuest = GetPosX();
	g_iYPosInQuest = GetPosY();

	pUIManager->RearrangeOrder( m_nUIIndex, FALSE );
}

// 퀘스트 목록에 추가
// ----------------------------------------------------------------------------
// Name : AddToQuestList()
// Desc :
// ----------------------------------------------------------------------------
BOOL CUIQuestBook::AddToQuestList( int iQuestIndex, BOOL bComplete )
{
	if( m_vectorCompleteQuestList.size() + m_vectorProceedQuestList.size() >= MAX_QUEST )
	{	
		// [090527: selo] 메시지 박스 추가
		CUIMsgBox_Info	MsgBoxInfo;		
		MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );				
		MsgBoxInfo.AddString( _S(4417, "퀘스트 최대 수량을 초과하여 수락할 수 없습니다.") );
		CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );

		return FALSE;
	}

	CQuestDynamicData qdd(CQuestSystem::Instance().GetStaticData(iQuestIndex));
	sQuestInfo	TempQuest;
	TempQuest.iQuestIndex	= iQuestIndex;
	TempQuest.strQuestTitle	= qdd.GetName();
	TempQuest.iQuestType	= qdd.GetQuestCategory();
	TempQuest.iQuestScale	= qdd.GetQuestPartyScale();
	
	// [090603: selo] 카테고리별로 구분하는 리스트에 추가
	if( TempQuest.iQuestType >= QCATEGORY_NORMAL && TempQuest.iQuestType <= QCATEGORY_SUPPLEMENT )
	{
		// 해당 카테고리에 없으면 추가
		if(m_vectorCategoryQuestList[TempQuest.iQuestType].end()
			== std::find_if(m_vectorCategoryQuestList[TempQuest.iQuestType].begin(), m_vectorCategoryQuestList[TempQuest.iQuestType].end(), FindQuest(TempQuest) ) )
		{
			m_vectorCategoryQuestList[TempQuest.iQuestType].push_back(TempQuest);
		}
	}


	if( bComplete )
	{
		if(m_vectorCompleteQuestList.end()
		!= std::find_if(m_vectorCompleteQuestList.begin(), m_vectorCompleteQuestList.end(), FindQuest(TempQuest)))
			return FALSE;

		m_vectorCompleteQuestList.push_back( TempQuest );
		return TRUE;
	}
	else
	{
		if(m_vectorProceedQuestList.end()
		!= std::find_if(m_vectorProceedQuestList.begin(), m_vectorProceedQuestList.end(), FindQuest(TempQuest)))
			return FALSE;

		m_vectorProceedQuestList.push_back( TempQuest );
		return TRUE;
	}
	return FALSE;
}

// 퀘스트 목록에서 제거
// ----------------------------------------------------------------------------
// Name : DelFromQuestList()
// Desc :
// ----------------------------------------------------------------------------
BOOL CUIQuestBook::DelFromQuestList( int iQuestIndex, BOOL bComplete )
{
	sQuestInfo TempQuest;
	TempQuest.iQuestIndex	= iQuestIndex;
	
	// [090602: selo] : 전체화면에 표시하기 위해 체크되어 있는 퀘스트를 제거한다.
	RemoveSelectedQuest(iQuestIndex);


	// [090603: selo] : 카테고리별로 구분하는 리스트에서 지우기
	for( int i = 0; i < QCATEGORY_END; ++i )
	{
		std::vector<sQuestInfo>::iterator it = std::find_if( m_vectorCategoryQuestList[i].begin(), m_vectorCategoryQuestList[i].end(), FindQuest( TempQuest ) );
		if( it != m_vectorCategoryQuestList[i].end() )
		{
			m_vectorCategoryQuestList[i].erase( it );
			break;
		}
	}

	m_btnCancel.SetEnable(FALSE);
	m_lbDescription.Reset();
	m_lbDescription.ResetAllStrings();
	// [090609: selo] : 불필요하게 assert가 걸리게 되는 부분을 막기 위해
	//					진행중과 완료인 것중에 있기만 하면 삭제 하도록 하였음.
	std::vector<sQuestInfo>::iterator it = std::find_if( m_vectorCompleteQuestList.begin(), m_vectorCompleteQuestList.end(), FindQuest( TempQuest ) );
	if( it != m_vectorCompleteQuestList.end() )
	{
		m_vectorCompleteQuestList.erase( it );
		return TRUE;
	}
	
	it = std::find_if( m_vectorProceedQuestList.begin(), m_vectorProceedQuestList.end(), FindQuest( TempQuest ) );
	if( it != m_vectorProceedQuestList.end() )
	{
		m_vectorProceedQuestList.erase( it );
		return TRUE;
	}

	return FALSE;
}

// ----------------------------------------------------------------------------
// Name : ClearQuestList()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::ClearQuestList()
{
	m_bProceedListExtended = TRUE;
	if(!m_vectorCompleteQuestList.empty())
	{
		m_vectorCompleteQuestList.clear();
	}
	m_bCompleteListExtended = TRUE;
	if(!m_vectorProceedQuestList.empty())
	{
		m_vectorProceedQuestList.clear();
	}
	for( int i = 0; i < QCATEGORY_END; ++i )
	{
		m_bCategoryListExtended[i] = TRUE;
		if (!m_vectorCategoryQuestList[i].empty())
		{
			m_vectorCategoryQuestList[i].clear();
		}
	}
	m_bRaidMessageListExtended = TRUE;
	if (!m_vectorRaidMessageList.empty())
	{
		m_vectorRaidMessageList.clear();
	}
}

// ----------------------------------------------------------------------------
// Name : RefreshQuestList()
// Desc : 서버에서 메세지를 다 받은 후 갱신할때...
// ----------------------------------------------------------------------------
void CUIQuestBook::RefreshQuestList()
{
	RefreshQuestListNew();
}

// ----------------------------------------------------------------------------
// Name : AddDescString()
// Desc : [090812: selo] 보상 설명 처리 추가
//						 반환값 : 멀티라인 인가? true / false
// ----------------------------------------------------------------------------
bool CUIQuestBook::AddDescString( CTString &strMessage, const COLOR colDesc, BOOL bSelectable, BOOL bPrizeDesc )
{
	// Get length of string
	INDEX	nLength = strMessage.Length();
	INDEX	limitstrpos = _iMaxDescStringChar;
	if( nLength == 0 )
		return false;
	// wooss 051002
// 브라질친화도보상아이탬버그 [1/16/2013 Ranma]
#if defined(G_THAI) 
	nLength = FindThaiLen(strMessage);
	limitstrpos = (_iMaxDescStringChar-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
	if( UI_QUESTBOOK_LIST == m_nUIIndex )
		limitstrpos =  (_iMaxDescStringCharNew-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
#else
	nLength = strMessage.Length();
	limitstrpos = _iMaxDescStringChar;
	if( UI_QUESTBOOK_LIST == m_nUIIndex )
			limitstrpos = _iMaxDescStringCharNew;
#endif
	int		iPos;
	// If length of string is less than max char
	if( nLength <= limitstrpos )
	{ 
		// Check line character		
		for( iPos = 0; iPos < strMessage.Length(); ++iPos )
		{
			if( strMessage[iPos] == '\n' || strMessage[iPos] == '\r' )
				break;	
		}
		
		// Not exist
		if( iPos == nLength )
		{
			m_lbDescription.ChangeCurrentState(CUIListBoxEx::PS_NONE);
			m_lbDescription.AddString( 0, strMessage, colDesc, bSelectable );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strMessage.Split( iPos, strTemp2, strTemp );
			
			m_lbDescription.ChangeCurrentState(CUIListBoxEx::PS_NONE);
			m_lbDescription.AddString( 0, strTemp2, colDesc, bSelectable ); 
			
			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );
			
			AddDescString( strTemp, colDesc, bSelectable );
		}
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = limitstrpos;
		BOOL	b2ByteChar = FALSE;
#if defined(G_THAI) 	
		for( iPos = 0; iPos < strMessage.Length(); iPos++ )
		{
			if(nSplitPos < FindThaiLen(strMessage,0,iPos))
				break;
		}
		nSplitPos = iPos;
#else
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strMessage[iPos] & 0x80 )
				b2ByteChar = !b2ByteChar;
			else
				b2ByteChar = FALSE;
		}
		
		if( b2ByteChar )
			nSplitPos--;
#endif
		// Check line character
		for( iPos = 0; iPos < nSplitPos; ++iPos )
		{
			if( strMessage[iPos] == '\n' || strMessage[iPos] == '\r' )
				break;
		}
		
		// Not exist
		if( iPos == nSplitPos )
		{
			// Split string
			CTString	strTemp, strTemp2;
#ifdef LINE_CHANGE_BY_WORD
			if( strMessage[nSplitPos] != ' ' )
			{
				for(int i = iPos; i>=0; --i)
				{
					if( strMessage[i] == ' ' )
					{
						iPos = i;
						break;
					}
				}
			}
#endif
			strMessage.Split( iPos, strTemp2, strTemp );
			m_lbDescription.ChangeCurrentState(CUIListBoxEx::PS_NONE);
			m_lbDescription.AddString( 0, strTemp2, colDesc, bSelectable );
			
			// Trim space
			if( strTemp[0] == ' ' )
			{
				int	nTempLength = strTemp.Length();
				for( iPos = 1; iPos < nTempLength; ++iPos )
				{
					if( strTemp[iPos] != ' ' )
						break;
				}
				
				strTemp.TrimLeft( strTemp.Length() - iPos );
			}
			// [090812: selo] 보상품 인 경우 "     " 를 추가
			if( TRUE == bPrizeDesc )
			{
				CTString strSpace = CTString("     ");
#if defined(G_RUSSIA)
				strSpace = CTString("        ");
#endif
				
				strTemp = strSpace + strTemp;
			}
			
			AddDescString( strTemp, colDesc, bSelectable );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strMessage.Split( iPos, strTemp2, strTemp );
			m_lbDescription.ChangeCurrentState(CUIListBoxEx::PS_NONE);
			m_lbDescription.AddString( 0, strTemp2, colDesc, bSelectable );
			
			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );
			
			AddDescString( strTemp, colDesc, bSelectable );
		}
		// 멀티 라인은 true 리턴
		return true;
	}		
	// 멀티 라인 아니면 false 리턴
	return false;
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::Render()
{
	RenderQuestListNew();
}

// ----------------------------------------------------------------------------
// Name : AcceptQuest()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::AcceptQuest()				// 퀘스트 수락
{
	if(m_nSelectedQuestIndex == -1) return;
	
	if( !m_pQuestItem )
	{
		this->SendQuestStart(m_nSelectedQuestIndex);
	}
	else
	{
		_pNetwork->SendItemUse(m_pQuestItem->Item_Tab, m_pQuestItem->InvenIndex, m_pQuestItem->Item_UniIndex, m_nSelectedQuestIndex);		
	}
}

// ----------------------------------------------------------------------------
// Name : ReserveQuest()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::ReserveQuest()				// 퀘스트 보류
{
	CQuestSystem::Instance().DeleteCurrentRequest();	
	for(int i=0; i<m_vectorProceedQuestList.size(); ++i)
	{
		if(m_nSelectedQuestIndex == m_vectorProceedQuestList[i].iQuestIndex)
		{
			if(m_vectorProceedQuestList.size() != i+1)
				m_iNextQuestIndex = m_vectorProceedQuestList[i+1].iQuestIndex;
			else if(i == 0)
				m_iNextQuestIndex = m_vectorProceedQuestList[i-1].iQuestIndex;
			else 
				m_iNextQuestIndex = -1;
		}
	}

	CloseQuestBook();
}

// ----------------------------------------------------------------------------
// Name : DenyQuest()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::DenyQuest()				// 퀘스트 거절
{
	if(m_nSelectedQuestIndex == -1)
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox(MSGCMD_QUEST_GIVEUP);
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;	
	MsgBoxInfo.SetMsgBoxInfo( _S( 99, "퀘스트" ), UMBS_OKCANCEL, m_nUIIndex, MSGCMD_QUEST_GIVEUP);
	CQuestDynamicData QuestDD(CQuestSystem::Instance().GetStaticData(m_nSelectedQuestIndex));
	if(QuestDD.GetQuestType2() == QTYPE_REPEAT_ONCE)
	{
		strMessage.PrintF( _S( 1709, "이 퀘스트를 거절하면 다시는 이 퀘스트를 받을 수 없습니다. 정말로 거절하시겠습니까?" ) );
	}
	else if(QuestDD.GetQuestType2() == QTYPE_REPEAT_UNLIMITED)
	{
		strMessage.PrintF( _S( 1710, "정말 이 퀘스트를 거절하시겠습니까?" ) );
	}
	// [090616: selo] 일일 퀘스트 거절
	else if(QuestDD.GetQuestType2() == QTYPE_REPEAT_DAY)
	{
		strMessage.PrintF( _S( 1710, "정말 이 퀘스트를 거절하시겠습니까?" ) );
	}
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : GiveUpQuest()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::GiveUpQuest()				// 퀘스트 포기
{
	if(m_nSelectedQuestIndex == -1) return;

	if( !m_pQuestItem )
	{
		this->SendQuestCancel(m_nSelectedQuestIndex);
	}
	else
	{
		// 아이템을 통해서 퀘스트를 얻으려 할 경우
		_pNetwork->SendItemUse( m_pQuestItem->Item_Tab, m_pQuestItem->InvenIndex, m_pQuestItem->Item_UniIndex, -1 );
		CloseQuestBook();
	}
}

// ----------------------------------------------------------------------------
// Name : PrizeQuest()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::PrizeQuest()			// 퀘스트 완료
{
	CQuestDynamicData *pDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex(m_nSelectedQuestIndex);
	if(pDD == NULL)
	{
		ASSERT(0 && "Something wrong");
		return;
	}

	INDEX iItemIndex = -1;
	INDEX iItemPlus = 0;
	if(pDD->IsOptionPrizeExist())
	{
		INDEX iOptionPrizeIndex = m_lbDescription.GetCurSel() - m_nSelectPrizeFirstIndex;
		if(m_lbDescription.GetCurSel() == -1
		|| iOptionPrizeIndex >= m_nSelectPrizeCount
		|| iOptionPrizeIndex < 0)
		{
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 99, "퀘스트" ), UMBS_OK, UI_QUESTBOOK_COMPLETE, MSGCMD_QUEST_NOTIFY );
			CTString strMessage;
			strMessage.PrintF( _S( 1711, "받고 싶은 보상을 선택하여 주십시오." ) );
			MsgBoxInfo.AddString( strMessage );
			CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
			m_lbDescription.SetCurSel(-1);
			return;
		}

		iOptionPrizeIndex = iOptionPrizeIndex >> 1;
		iItemIndex = pDD->GetOptionPrizeIndex(iOptionPrizeIndex);
		if(iItemIndex == -1)
		{
			ASSERT(0 && "Something wrong");
			return;
		}
		iItemPlus = pDD->GetOptionPrizePlus(iOptionPrizeIndex);
	}

	// tutorial bug fix. [8/24/2010 rumist]
	if (pDD->GetPrizeNPCIndex() == 0) { // 퀘스트 북 스크롤 보상
		this->SendQuestPrize(m_nSelectedQuestIndex, pDD->GetPrizeNPCIndex(), iItemIndex, iItemPlus);
	} else {
		this->SendQuestPrize(m_nSelectedQuestIndex, m_nTargetIndex, iItemIndex, iItemPlus);
	}
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{	
	switch( nCommandCode )
	{
	case MSGCMD_QUEST_GIVEUP:		// 퀘스트 포기
		if(bOK)
		{
			GiveUpQuest();
		}
		m_bLockQuestList = FALSE;
		break;
	case MSGCMD_QUEST_TUTORIAL_START:
		if( bOK )
		{
			;	// nothing.
		}
		else
		{
			this->OpenTutorialWindow();
		}
		break;
	case MSGCMD_QUEST_TUTORIAL_CANCEL:
		if( bOK )
		{
			this->SendQuestCancel(45);
		}
		else
		{
			this->OpenTutorialWindow();
		}
		break;
	}
}
void CUIQuestBook::MsgBoxLCommand(int nCommandCode, int nResult )
{
	switch( nCommandCode )
	{
		case MSGLCMD_QUEST_TUTORIAL_OPEN:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			if( nResult == RESPONSE_OK )
			{
				CUIMsgBox_Info MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 928, "튜토리얼" ), UMBS_OKCANCEL, UI_QUESTBOOK_LIST, MSGCMD_QUEST_TUTORIAL_START );
				MsgBoxInfo.AddString( _S( 5042, "튜토리얼을 진행하시겠습니까?" ) );
				pUIManager->CreateMessageBox( MsgBoxInfo );
			}

			if( nResult == RESPONSE_CALCEL )
			{
				CUIMsgBox_Info MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 928, "튜토리얼" ), UMBS_OKCANCEL, UI_QUESTBOOK_LIST, MSGCMD_QUEST_TUTORIAL_CANCEL );
				MsgBoxInfo.AddString( _S( 5043, "튜토리얼에서 나가시겠습니까?" ) );
				pUIManager->CreateMessageBox( MsgBoxInfo );
			}
		}
		break;
	}
}
// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIQuestBook::MouseMessage( MSG *pMsg )
{
	if( UI_QUESTBOOK_LIST == m_nUIIndex )
	{
		return MsgProcQuestListNew(pMsg);		
	}

	WMSG_RESULT	wmsgResult;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			// If message box isn't focused
			if( !IsFocused() )
				return WMSG_FAIL;

			// Move message box
			if( m_bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// List box
			else if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_lbQuestList.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnPrev.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					m_bTitleBarClick = TRUE;
				}
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				}
				// List box
				else if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					if(m_lbDescription.GetCurSel() != -1)
					{
						int iOptionPrizeSelected = m_lbDescription.GetCurSel() - m_nSelectPrizeFirstIndex;
						m_lbDescription.SetCurSel(iOptionPrizeSelected - (iOptionPrizeSelected & 0x00000001) + m_nSelectPrizeFirstIndex);
					}
				}
				// Quest List Box
				else if( m_lbQuestList.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					int groupNameLine = 0;
					for(int i=0; i<m_lbQuestList.GetCurItemCount(0); ++i)
					{
						if(m_lbQuestList.GetState(i) == CUIListBoxEx::PS_CLOSE
						|| m_lbQuestList.GetState(i) == CUIListBoxEx::PS_EXTEND)
						{
							if(groupNameLine == 0)//proceed
							{
								m_bProceedListExtended = m_lbQuestList.GetState(i) == CUIListBoxEx::PS_EXTEND;
								++groupNameLine;
							}
							else if(groupNameLine == 1)//complete
							{
								m_bCompleteListExtended = m_lbQuestList.GetState(i) == CUIListBoxEx::PS_EXTEND;
								++groupNameLine;
								break;
							}
						}
					}
				}

				CUIManager::getSingleton()->RearrangeOrder( m_nUIIndex, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			// If holding button doesn't exist
			if( pUIManager->GetHoldBtn().IsEmpty() )
			{
				// Title bar
				m_bTitleBarClick = FALSE;
				
				// If message box isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;
				
				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseQuestBook();
					
					return WMSG_SUCCESS;
				}
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseQuestBook();
					return WMSG_SUCCESS;
				}
				// List box
				else if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_lbQuestList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_nUIIndex == UI_QUESTBOOK_COMPLETE )
				{
					// 퀘스트 완료
					if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							PrizeQuest();
						return WMSG_SUCCESS;				
					}
					// NPC 메뉴 리스트로 돌아감
					else if( ( wmsgResult = m_btnPrev.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							//ReturnToNPCMenu();
						return WMSG_SUCCESS;				
					}
				}
				else if( m_nUIIndex == UI_QUESTBOOK_LIST )
				{
					if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							CloseQuestBook();
						return WMSG_SUCCESS;				
					}
				}
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					// Reset holding button
					pUIManager->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				// List box
				if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_lbQuestList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				// List box
				if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_lbQuestList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : RequestQuest()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::RequestQuest( int nNpcIndex, int nNpcVirIndex, SBYTE sbUIType, FLOAT fX, FLOAT fZ )
{

}
void CUIQuestBook::OpenTutorialWindow()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// If this is alreay exist.
	if( pUIManager->DoesMessageBoxLExist( MSGLCMD_QUEST_TUTORIAL_OPEN ) )
		return;

	pUIManager->CloseMessageBox(MSGCMD_QUEST_TUTORIAL_START);
	pUIManager->CloseMessageBox(MSGCMD_QUEST_TUTORIAL_CANCEL);

	// Create refine message box.
	pUIManager->CreateMessageBoxL( _S( 928,"튜토리얼" ), UI_QUESTBOOK_LIST, MSGLCMD_QUEST_TUTORIAL_OPEN );

	pUIManager->AddMessageBoxLString( MSGLCMD_QUEST_TUTORIAL_OPEN, TRUE, _S( 928, "튜토리얼" ), -1, 0xE18600FF );
	pUIManager->AddMessageBoxLString( MSGLCMD_QUEST_TUTORIAL_OPEN, TRUE, _S( 5039, "라스트카오스에 오신것을 환영합니다.\n이곳은 처음 접속하시는 유저분들을 위해\n기본 조작 방법을 안내하는 곳입니다.\n\n앞에 있는 문을 열고 통로를 따라가시면 각종\n몬스터가 등장하고, 통로 끝에 존재하는 몬스터를\n사냥하면 쥬노 란돌 마을로 이동하실 수 있습니다." ), -1, 0xA3A1A3FF );

	CTString strMessage;

	// accept
	strMessage.PrintF( _S( 5040, "확인 (튜토리얼을 진행합니다.)" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_QUEST_TUTORIAL_OPEN, FALSE, strMessage, RESPONSE_OK );

	// cancel.
	strMessage.PrintF( _S( 5041, "나가기 (튜토리얼을 진행하지 않습니다.)") );
	pUIManager->AddMessageBoxLString( MSGLCMD_QUEST_TUTORIAL_OPEN, FALSE, strMessage, RESPONSE_CALCEL );
}
// ----------------------------------------------------------------------------
// Name : OpenWindow()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::OpenWindow( BOOL bHasQuestList )
{
	
}

// ----------------------------------------------------------------------------
// Name : SendQuestReq()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::SendQuestReq( int nNpcIndex )
{
	ASSERT(nNpcIndex != -1);
	_pNetwork->SendQuestMessage( MSG_QUEST_REQUEST, nNpcIndex );
	//m_bLockRequestQuest = TRUE;
}

// ----------------------------------------------------------------------------
// Name : SendQuestCancel()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::SendQuestCancel( int nQuestIndex )
{
	ASSERT(nQuestIndex != -1);
	_pNetwork->SendQuestMessage( MSG_QUEST_GIVEUP, nQuestIndex );
}

// ----------------------------------------------------------------------------
// Name : SendQuestStart()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::SendQuestStart( int nQuestIndex )
{
	ASSERT(nQuestIndex != -1);
	_pNetwork->SendQuestMessage( MSG_QUEST_START, nQuestIndex );
}

// ----------------------------------------------------------------------------
// Name : SendQuestPrize()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::SendQuestPrize(int iQuestIndex, INDEX iNpcIndex, INDEX iOptionItemIndex, INDEX iOptionItemPlus)
{
	ASSERT(iQuestIndex != -1 && iNpcIndex != -1);
	_pNetwork->SendQuestPrizeMessage(MSG_QUEST_PRIZE, iQuestIndex, iNpcIndex, iOptionItemIndex, iOptionItemPlus);
}

// ----------------------------------------------------------------------------
// Name : TalkWithNPC()
// Desc :
// ----------------------------------------------------------------------------
void CUIQuestBook::TalkWithNPC()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(GAMEDATAMGR()->GetQuest()->GetCompleteQuestCount())
	{
		// 2009. 05. 27 김정래
		// NPC 메뉴로 돌아갈 것을 대비해 NPC Index 를 저장해 둔다.
		if( pUIManager->GetQuestBookList()->m_nTargetIndex != -1 )
		{
			pUIManager->GetQuestBookComplete()->SetPrevNpcIdx(pUIManager->GetQuestBookList()->m_nTargetIndex);
			pUIManager->GetQuestBookComplete()->SetPrevNpcUIType(pUIManager->GetQuestBookComplete()->GetNpcUIType());
		}
		else
		{
			pUIManager->GetQuestBookList()->m_nTargetIndex = pUIManager->GetQuestBookComplete()->GetPrevNpcIdx();			
		}

		pUIManager->GetQuestBookComplete()->SetTargetIndex(pUIManager->GetQuestBookList()->m_nTargetIndex);
		pUIManager->GetQuestBookList()->m_nTargetIndex = -1;
		pUIManager->GetQuestAccept()->SetTargetIndex(-1);
	}
	else if(GAMEDATAMGR()->GetQuest()->GetProceedQuestCount())
	{
		// 2009. 05. 27 김정래
		// NPC 메뉴로 돌아갈 것을 대비해 NPC Index 를 저장해 둔다.
		if( pUIManager->GetQuestBookList()->m_nTargetIndex != -1 )
		{
			pUIManager->GetQuestAccept()->SetPrevNpcIdx(pUIManager->GetQuestBookList()->m_nTargetIndex);
			pUIManager->GetQuestAccept()->SetPrevNpcUIType(pUIManager->GetQuestAccept()->GetNpcUIType());
		}
		else
		{
			pUIManager->GetQuestBookList()->m_nTargetIndex = pUIManager->GetQuestAccept()->GetPrevNpcIdx();			
		}

		pUIManager->GetQuestAccept()->SetTargetIndex(pUIManager->GetQuestBookList()->m_nTargetIndex);
		pUIManager->GetQuestBookList()->m_nTargetIndex = -1;
	}
	else
	{
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 99, "퀘스트" ), UMBS_OK, UI_QUEST_ACCEPT, MSGCMD_QUEST_NOTIFY );
		CTString strMessage;
		strMessage.PrintF( _S( 584, "수행할 수 있는 퀘스트가 없습니다." ) );					
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}
}


// ======================================================
// [5/27/2009 selo]
// Name : GetProceedQuestIndex()
// Desc : 진행 가능한 퀘스트 인덱스를 리턴한다
// ======================================================
INDEX CUIQuestBook::GetProceedQuestIndex(INDEX index)
{
	if( m_vectorProceedQuestList.size() - 1 < index )	
		return -1;

	sQuestInfo info = m_vectorProceedQuestList[index];
	return info.iQuestIndex;
}

// ======================================================
//  [5/27/2009 selo]
// Name : GetCompleteQuestIndex()
// Desc : 보상 처리 할 퀘스트 인덱스를 리턴한다
// ======================================================
INDEX CUIQuestBook::GetCompleteQuestIndex(INDEX index)
{
	if( m_vectorCompleteQuestList.size() - 1 < index )
		return -1;

	sQuestInfo info = m_vectorCompleteQuestList[index];
	return info.iQuestIndex;
}

// =====================================================
//  [5/27/2009 selo]
// Name : AddQuestListToMessageBoxL()
// Desc : 나열 가능한 퀘스트를 메시지 박스에 올린다.
// =====================================================
void CUIQuestBook::AddQuestListToMessageBoxL(const int& iMessageBoxType)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	int iQuestIndex = -1;
	CTString strQuestTitle;
	int count;

	count = GAMEDATAMGR()->GetQuest()->GetCompleteQuestCount();

	for( int iComplete = 0; iComplete < count; ++iComplete )
	{
		iQuestIndex = GAMEDATAMGR()->GetQuest()->GetCompleteQuestIndex(iComplete);
		
		CQuestDynamicData qdd(CQuestSystem::Instance().GetStaticData(iQuestIndex));				
		strQuestTitle = qdd.GetName();

		// [090728: selo] 퀘스트 타이틀이 아무것도 없으면 없는 것으로 간주 하고 메시지 박스에 추가하지 않는다.
		if( 0 == strQuestTitle.Length() )
			continue;
		
		pUIManager->AddMessageBoxLString( iMessageBoxType, FALSE, strQuestTitle, ciQuestClassifier + iQuestIndex, 0xF2F200FF, CTString("A") );
	}

	count = GAMEDATAMGR()->GetQuest()->GetProceedQuestCount();

	for( int iNew = 0; iNew < count; ++iNew )
	{
		iQuestIndex = GAMEDATAMGR()->GetQuest()->GetProceedQuestIndex(iNew);
		
		CQuestDynamicData qdd(CQuestSystem::Instance().GetStaticData(iQuestIndex));				
		strQuestTitle = qdd.GetName();

		// [090728: selo] 퀘스트 타이틀이 아무것도 없으면 없는 것으로 간주 하고 메시지 박스에 추가하지 않는다.
		if( 0 == strQuestTitle.Length() )
			continue;
		
		pUIManager->AddMessageBoxLString( iMessageBoxType, FALSE, strQuestTitle, ciQuestClassifier + iQuestIndex, 0xF2F200FF, CTString("Q") );
	}
}

// =====================================================
//  [5/27/2009 selo]
// Name : SelectQuestFromMessageBox()
// Desc : 선택한 퀘스트에 따른 UI 를 띄운다.
// =====================================================
void CUIQuestBook::SelectQuestFromMessageBox(const int& iClassifierIndex)
{
	int iQuestIndex = iClassifierIndex - ciQuestClassifier;
	if( iQuestIndex <= 0 )
		return;

	SBYTE sbType = GetQuestType(iQuestIndex);	
	if( 1 == sbType )	// 수락
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		if( pUIManager->GetQuestBookList()->m_nTargetIndex != -1 )
		{
			pUIManager->GetQuestAccept()->SetPrevNpcIdx(pUIManager->GetQuestBookList()->m_nTargetIndex);
			pUIManager->GetQuestAccept()->SetPrevNpcUIType(pUIManager->GetQuestAccept()->GetNpcUIType());
		}
		else
		{
			pUIManager->GetQuestBookList()->m_nTargetIndex = pUIManager->GetQuestAccept()->GetPrevNpcIdx();			
		}

		pUIManager->GetQuestAccept()->open(iQuestIndex);
		pUIManager->GetQuestAccept()->SetTargetIndex(pUIManager->GetQuestBookList()->m_nTargetIndex);
		pUIManager->GetQuestBookList()->m_nTargetIndex = -1;		
	}
	else if( 2 == sbType ) // 보상
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		if( pUIManager->GetQuestBookList()->m_nTargetIndex != -1 )
		{
			pUIManager->GetQuestBookComplete()->SetPrevNpcIdx(pUIManager->GetQuestBookList()->m_nTargetIndex);
			pUIManager->GetQuestBookComplete()->SetPrevNpcUIType(pUIManager->GetQuestBookComplete()->GetNpcUIType());
			pUIManager->GetQuestAccept()->SetPrevNpcIdx(pUIManager->GetQuestBookList()->m_nTargetIndex);
			pUIManager->GetQuestAccept()->SetPrevNpcUIType(pUIManager->GetQuestAccept()->GetNpcUIType());
		}
		else
		{
			pUIManager->GetQuestBookList()->m_nTargetIndex = pUIManager->GetQuestBookComplete()->GetPrevNpcIdx();
		}

		pUIManager->GetQuestBookComplete()->open(iQuestIndex);
		pUIManager->GetQuestBookComplete()->SetTargetIndex(pUIManager->GetQuestBookList()->m_nTargetIndex);
		pUIManager->GetQuestBookList()->m_nTargetIndex = -1;
		pUIManager->GetQuestAccept()->SetTargetIndex(-1);
	}
}

// =====================================================
//  [5/27/2009 selo]
// Name : GetQuestType()
// Desc : 퀘스트 인덱스를 이용해 퀘스트의 타입을 알아낸다.
// Return : 0 - 실패, 1 - 수락, 2 - 보상
// =====================================================
SBYTE CUIQuestBook::GetQuestType(const INDEX& iQuestIndex)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	int iTempIndex = -1;
	int count;

	count = GAMEDATAMGR()->GetQuest()->GetCompleteQuestCount();

	for( int iComplete = 0; iComplete < count; ++iComplete )
	{
		iTempIndex = GAMEDATAMGR()->GetQuest()->GetCompleteQuestIndex(iComplete);
		
		if( iQuestIndex == iTempIndex )
			return 2;
	}

	count = GAMEDATAMGR()->GetQuest()->GetProceedQuestCount();
	for( int iNew = 0; iNew < count; ++iNew )
	{
		iTempIndex = GAMEDATAMGR()->GetQuest()->GetProceedQuestIndex(iNew);
		
		if( iQuestIndex == iTempIndex )
			return 1;
	}

	return 0;
}

// =====================================================
//  [6/2/2009 selo]
// Name : AddSelectedQuest()
// Desc : 전체 화면에 퀘스트 진행상황을 표시할 퀘스트를
//		  체크 하였을 때 list 에 추가
// =====================================================
void CUIQuestBook::AddSelectedQuest(const int& iQuestIndex)
{
	// 이미 리스트에 있으면 그냥 리턴
	if( std::count(m_listSelectedQuestIndex.begin(), m_listSelectedQuestIndex.end(), iQuestIndex) )
		return;

	m_listSelectedQuestIndex.push_back(iQuestIndex);

	CUIManager::getSingleton()->GetPlayerInfo()->UpdateSelectedQuest();
}

// =====================================================
//  [6/2/2009 selo]
// Name : RemoveSelectedQuest()
// Desc : 전체 화면에 뿌리고 있는 퀘스트를 체크 해제 할 때
//		  list 에서 제거
// =====================================================
void CUIQuestBook::RemoveSelectedQuest(const int& iQuestIndex)
{
	m_listSelectedQuestIndex.remove(iQuestIndex);

	CUIManager::getSingleton()->GetPlayerInfo()->UpdateSelectedQuest();
}

// =====================================================
//  [6/2/2009 selo]
// Name : IsSelectedQuest()
// Desc : 해당 인덱스가 선택되어 있는 지 확인
// =====================================================
bool CUIQuestBook::IsSelectedQuest(const int& iQuestIndex)
{
	if( std::count(m_listSelectedQuestIndex.begin(), m_listSelectedQuestIndex.end(), iQuestIndex) )
		return true;	

	return false;
}

// =====================================================
//  [8/3/2009 sora]
// Name : ClearSelectedQuestList()
// Desc : list에 저장되어 있는 선택된 퀘스트 리스트를 clear
// =====================================================
void CUIQuestBook::ClearSelectedQuestList()
{
	if(!m_listSelectedQuestIndex.empty())
	{
		m_listSelectedQuestIndex.clear();
	}
}

// =====================================================
//  [6/3/2009 selo]
// Name : SortQuestListByCondition()
// Desc : 진행 상태에 따른 정렬.
//		  진행 중과 완료로 구분하여 정렬한다.
// =====================================================
void CUIQuestBook::SortQuestListByCondition()
{
	CTString strTemp;
	CQuestDynamicData *pQuestDD = NULL;
	
	m_lbQuestList.ResetAllStrings();
	m_lbQuestList.Reset();

	m_lbQuestList.ChangeCurrentState(m_bProceedListExtended ? CUIListBoxEx::PS_EXTEND : CUIListBoxEx::PS_CLOSE);
	m_lbQuestList.AddString( 0, CTString( "" ), 0xFFFFFFFF, FALSE );
	strTemp.PrintF(_S( 1707, "진행중인 퀘스트 (%d)" ), m_vectorProceedQuestList.size()); 
	m_lbQuestList.AddString( 1, strTemp, 0xffb72dff );
	
	std::vector<sQuestInfo>::iterator it = m_vectorProceedQuestList.begin();
	std::vector<sQuestInfo>::iterator itend = m_vectorProceedQuestList.end();
	for( ; it != itend; ++it )
	{
		pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex((*it).iQuestIndex);
		CTString strTitle("");
		
		if( pQuestDD )
		{
			strTitle.PrintF("[%d] %s", pQuestDD->GetNeedMinLevel(), (*it).strQuestTitle);

			// [090810: selo] 제목이 길면 ... 으로 줄인다
			strTitle = MakeTitleString(strTitle, 20);
			
			// [090728: selo] 조건에 따른 정렬에서도 파티 규모에 따라 꼬리말을 붙인다.
			if( IsCompleteQuest((*it).iQuestIndex) )
				strTitle += _S(4422, " (완료)");
			else if( QSCALE_PARTY == (*it).iQuestScale )
				strTitle += _S(4423, " (파티)");
			else if( QSCALE_EXPEDITION == (*it).iQuestScale )
				strTitle += _S(4424, " (원정대)");
		}
		else
		{
			// [090810: selo] 제목이 길면 ... 으로 줄인다
			strTitle = MakeTitleString((*it).strQuestTitle, 20);			
		}
		
		m_lbQuestList.ChangeCurrentState(CUIListBoxEx::PS_CHECKCHILD);
		
		if( IsSelectedQuest((*it).iQuestIndex) )
			m_lbQuestList.SetCheckState(m_lbQuestList.GetLastChangedLine(), TRUE);
		
		m_lbQuestList.AddString( 0, CTString( "" ), 0xF2F2F2FF );
		m_lbQuestList.AddString( 1, strTitle, 0xF2F2F2FF );
	}
	
	m_lbQuestList.ChangeCurrentState(m_bCompleteListExtended ? CUIListBoxEx::PS_EXTEND : CUIListBoxEx::PS_CLOSE);
	m_lbQuestList.AddString( 0, CTString( "" ), 0xFFFFFFFF, FALSE );
	strTemp.PrintF(_S( 1708, "완료된 퀘스트 (%d)" ), m_vectorCompleteQuestList.size()); 
	m_lbQuestList.AddString( 1, strTemp, 0xffb72dff );
	
	it = m_vectorCompleteQuestList.begin();
	itend = m_vectorCompleteQuestList.end();
	for( ; it != itend; ++it )
	{
		pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex((*it).iQuestIndex);
		CTString strTitle("");
		
		if( pQuestDD )
		{
			strTitle.PrintF("[%d] %s", pQuestDD->GetNeedMinLevel(), (*it).strQuestTitle);

			// [090810: selo] 제목이 길면 ... 으로 줄인다
			strTitle = MakeTitleString(strTitle, 20);

			// [090728: selo] 조건에 따른 정렬에서도 파티 규모에 따라 꼬리말을 붙인다.
			if( IsCompleteQuest((*it).iQuestIndex) )
				strTitle += _S(4422, " (완료)");
			else if( QSCALE_PARTY == (*it).iQuestScale )
				strTitle += _S(4423, " (파티)");
			else if( QSCALE_EXPEDITION == (*it).iQuestScale )
				strTitle += _S(4424, " (원정대)");
		}
		else
		{
			// [090810: selo] 제목이 길면 ... 으로 줄인다
			strTitle = MakeTitleString((*it).strQuestTitle, 20);
			strTitle.PrintF("%s (%s)", strTitle, _S( 429, "완료" ));
		}
		
		m_lbQuestList.ChangeCurrentState(CUIListBoxEx::PS_CHECKCHILD);
		
		if( IsSelectedQuest((*it).iQuestIndex) )
			m_lbQuestList.SetCheckState(m_lbQuestList.GetLastChangedLine(), TRUE);
		
		m_lbQuestList.AddString( 0, CTString( "" ), 0xF2F2F2FF );
		m_lbQuestList.AddString( 1, strTitle, 0xF2F2F2FF );
	}

	// [090708: selo] 레이드 퀘스트 관련 추가
	m_lbQuestList.ChangeCurrentState(m_bRaidMessageListExtended ? CUIListBoxEx::PS_EXTEND : CUIListBoxEx::PS_CLOSE);
	m_lbQuestList.AddString( 0, CTString( "" ), 0xFFFFFFFF, FALSE );
	strTemp.PrintF(_S( 4429, "레이드 진행 메시지 (%d)" ), m_vectorRaidMessageList.size()); 
	m_lbQuestList.AddString( 1, strTemp, 0xffb72dff );

	it = m_vectorRaidMessageList.begin();
	itend = m_vectorRaidMessageList.end();
	for( ; it != itend; ++it )
	{
		pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex((*it).iQuestIndex);
		CTString strTitle("");		
		
		strTitle.PrintF("%s", (*it).strQuestTitle);
		
		m_lbQuestList.ChangeCurrentState(CUIListBoxEx::PS_CHECKCHILD);
		
		if( IsSelectedQuest((*it).iQuestIndex) )
			m_lbQuestList.SetCheckState(m_lbQuestList.GetLastChangedLine(), TRUE);
		
		m_lbQuestList.AddString( 0, CTString( "" ), 0xF2F2F2FF );
		m_lbQuestList.AddString( 1, strTitle, 0xF2F2F2FF );
	}
}

// =====================================================
//  [6/3/2009 selo]
// Name : SortQuestListByType()
// Desc : 종류에 따른 정렬.
//		  퀘스트의 종류에 따라 구분하여 정렬한다.
// =====================================================
void CUIQuestBook::SortQuestListByType()
{
	CTString strTemp;
	CQuestDynamicData *pQuestDD = NULL;
	
	m_lbQuestList.ResetAllStrings();
	m_lbQuestList.Reset();

	for( int i = 0; i < QCATEGORY_END; ++i )
	{	
		m_lbQuestList.ChangeCurrentState(m_bCategoryListExtended[i] ? CUIListBoxEx::PS_EXTEND : CUIListBoxEx::PS_CLOSE);
		m_lbQuestList.AddString( 0, CTString( "" ), 0xFFFFFFFF, FALSE );
		
		if( QCATEGORY_NORMAL == i )
		{
			strTemp.PrintF(_S(4418, "일반 (%d)"), m_vectorCategoryQuestList[i].size()); 
		}
		else if( QCATEGORY_SCENARIO == i )
		{
			strTemp.PrintF(_S(4419, "시나리오 (%d)"), m_vectorCategoryQuestList[i].size()); 
		}
		else if( QCATEGORY_SUPPLEMENT == i )
		{
			strTemp.PrintF(_S(4420, "외전 (%d)"), m_vectorCategoryQuestList[i].size()); 
		}
		/*else if( QCATEGORY_NIGHTSHADOW == i )
		{
			strTemp.PrintF(_S(4421, "나이트 쉐도우 (%d)"), m_vectorCategoryQuestList[i].size()); 
		}*/		
		m_lbQuestList.AddString( 1, strTemp, 0xffb72dff );
		
		std::vector<sQuestInfo>::iterator it = m_vectorCategoryQuestList[i].begin();
		std::vector<sQuestInfo>::iterator itend = m_vectorCategoryQuestList[i].end();
		for( ; it != itend; ++it )
		{
			pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex((*it).iQuestIndex);
			CTString strTitle("");
			
			if( pQuestDD )
			{
				strTitle.PrintF("[%d] %s", pQuestDD->GetNeedMinLevel(), (*it).strQuestTitle);

				// [090810: selo] 제목이 길면 ... 으로 줄인다
				strTitle = MakeTitleString(strTitle, 20);

				if( IsCompleteQuest((*it).iQuestIndex) )
					strTitle += _S(4422, " (완료)");
				else if( QSCALE_PARTY == (*it).iQuestScale )
					strTitle += _S(4423, " (파티)");
				else if( QSCALE_EXPEDITION == (*it).iQuestScale )
					strTitle += _S(4424, " (원정대)");
			}
			else
			{
				// [090810: selo] 제목이 길면 ... 으로 줄인다
				strTitle = MakeTitleString((*it).strQuestTitle, 20);				
			}
			
			m_lbQuestList.ChangeCurrentState(CUIListBoxEx::PS_CHECKCHILD);
			
			if( IsSelectedQuest((*it).iQuestIndex) )
				m_lbQuestList.SetCheckState(m_lbQuestList.GetLastChangedLine(), TRUE);
			
			m_lbQuestList.AddString( 0, CTString( "" ), 0xF2F2F2FF );
			m_lbQuestList.AddString( 1, strTitle, 0xF2F2F2FF );
		}
	}

	// [090708: selo] 레이드 퀘스트 관련 추가
	m_lbQuestList.ChangeCurrentState(m_bRaidMessageListExtended ? CUIListBoxEx::PS_EXTEND : CUIListBoxEx::PS_CLOSE);
	m_lbQuestList.AddString( 0, CTString( "" ), 0xFFFFFFFF, FALSE );
	strTemp.PrintF(_S( 4429, "레이드 진행 메시지 (%d)" ), m_vectorRaidMessageList.size()); 
	m_lbQuestList.AddString( 1, strTemp, 0xffb72dff );

	std::vector<sQuestInfo>::iterator it = m_vectorRaidMessageList.begin();
	std::vector<sQuestInfo>::iterator itend = m_vectorRaidMessageList.end();
	for( ; it != itend; ++it )
	{
		pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex((*it).iQuestIndex);
		CTString strTitle("");		
		
		strTitle.PrintF("%s", (*it).strQuestTitle);
		
		m_lbQuestList.ChangeCurrentState(CUIListBoxEx::PS_CHECKCHILD);
		
		if( IsSelectedQuest((*it).iQuestIndex) )
			m_lbQuestList.SetCheckState(m_lbQuestList.GetLastChangedLine(), TRUE);
		
		m_lbQuestList.AddString( 0, CTString( "" ), 0xF2F2F2FF );
		m_lbQuestList.AddString( 1, strTitle, 0xF2F2F2FF );
	}
}

// =====================================================
//  [6/3/2009 selo]
// Name : IsCompleteQuest()
// Desc : 해당 인덱스의 퀘스트가 완료 중인지 판단한다.
// =====================================================
bool CUIQuestBook::IsCompleteQuest(int nQuestIndex)
{
	sQuestInfo TempQuest;
	TempQuest.iQuestIndex	= nQuestIndex;

	if(m_vectorCompleteQuestList.end()
		!= std::find_if(m_vectorCompleteQuestList.begin(), m_vectorCompleteQuestList.end(), FindQuest(TempQuest)))
		return true;

	return false;
}

// =====================================================
//  [6/9/2009 selo]
// Name : GetTimeAttackRemainTime()
// Desc : 타임어택 퀘스트 중 제일 먼저 수락 받은 퀘스트의 초시간을 반환한다.
//		  
// =====================================================
DOUBLE	CUIQuestBook::GetTimeAttackRemainTime()
{
	DOUBLE dTempTime = -1;
	DOUBLE dCurTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
	
	std::vector<sQuestInfo>::iterator iter = m_vectorProceedQuestList.begin();
	while( iter != m_vectorProceedQuestList.end() )
	{
		if( (*iter).dRemainTime > 0 && (*iter).dOldTime > 0 )
		{	
			(*iter).dRemainTime -= (dCurTime - (*iter).dOldTime);
			(*iter).dOldTime = dCurTime;
			
			if( (*iter).dRemainTime > 0 )
				return (*iter).dRemainTime;
		}

		++iter;
	}

	return -1;
}

// =====================================================
//  [6/9/2009 selo]
// Name : SetQuestRemainTime()
// Desc : 타임어택 퀘스트의 남은 시간을 설정한다.
// =====================================================
void CUIQuestBook::SetQuestRemainTime(int iQuestIndex, int iRemainTime)
{
	DOUBLE dCurTime = _pTimer->GetHighPrecisionTimer().GetSeconds();

	sQuestInfo TempQuest;
	TempQuest.iQuestIndex	= iQuestIndex;

	std::vector<sQuestInfo>::iterator iter = std::find_if(m_vectorProceedQuestList.begin(), m_vectorProceedQuestList.end(), FindQuest(TempQuest));
	if( iter != m_vectorProceedQuestList.end() )
	{
		(*iter).dRemainTime = (DOUBLE)iRemainTime;
		(*iter).dOldTime = dCurTime;
	}	
}


// =====================================================
//  [8/10/2009 selo]
// Name : MakeTitleString
// Desc : 제목이 길어지면 ... 으로 변경하여 준다
// =====================================================
CTString CUIQuestBook::MakeTitleString(CTString strTitle, int iSplitPos)
{
	if( strTitle.Length() <= iSplitPos )
		return strTitle;

	BOOL b2ByteChar = FALSE;

	for( int iPos = 0; iPos < iSplitPos; ++iPos )
	{
		if( strTitle[iPos] & 0x80 )
			b2ByteChar = !b2ByteChar;
		else
			b2ByteChar = FALSE;
	}
	
	if( b2ByteChar )
		iSplitPos--;

	CTString	strTemp, strTemp2;
	strTitle.Split( iSplitPos, strTemp, strTemp2 );

	strTemp += _s("...");

	return strTemp;
}

// =====================================================
//  [7/8/2009 selo]
// Name : AddRaidMessage
// Desc : 레이드 진행 메시지에 추가한다 
// =====================================================
void CUIQuestBook::AddRaidMessage(const int& iQuestIndex)
{
	CQuestDynamicData qdd(CQuestSystem::Instance().GetStaticData(iQuestIndex));
	sQuestInfo	TempQuest;
	TempQuest.iQuestIndex	= iQuestIndex;
	TempQuest.strQuestTitle	= qdd.GetName();	
	
	if(m_vectorRaidMessageList.end()
		== std::find_if(m_vectorRaidMessageList.begin(), m_vectorRaidMessageList.end(), FindQuest(TempQuest) ) )
	{
		m_vectorRaidMessageList.push_back(TempQuest);
	}

	// [090709: selo] 인덱스 작은게 앞으로 가게 정렬한다.
	std::sort(m_vectorRaidMessageList.begin(), m_vectorRaidMessageList.end());
	
	// [090820: selo] 이미 발생한 퀘스트 공지인지 체크하게 한다.
	bool bExist = false;
	for( int i = 0; i < m_aAlreadyShowQuestNotice.Count(); ++i )
	{
		if( iQuestIndex == m_aAlreadyShowQuestNotice[i] )
		{
			bExist = true;
			break;		
		}		
	}
	if( !bExist )
	{
		for( int index = 0; index < m_aAlreadyShowQuestNotice.Count(); ++index )
		{
			if( 0 == m_aAlreadyShowQuestNotice[index] )
			{
				m_aAlreadyShowQuestNotice[index] = iQuestIndex;
				
				// [090817: selo] 퀘스트 공지를 나오게 한다.
				CUIManager::getSingleton()->GetPlayerInfo()->UpdateQuestNotice(iQuestIndex);
				
				// [090818: selo] 퀘스트 공지 시작된 시간을 기록한다.
				m_dQuestNoticeStartTime = _pTimer->GetHighPrecisionTimer().GetSeconds();

				break;
			}
		}
	}	
}

// =====================================================
//  [7/8/2009 selo]
// Name : RemoveRaidMessage
// Desc : 해당 인덱스의 레이드 메시지를 제거한다
// =====================================================
void CUIQuestBook::RemoveRaidMessage(const int& iQuestIndex)
{
	sQuestInfo	TempQuest;
	TempQuest.iQuestIndex	= iQuestIndex;

	// 아래의 구문이 이해가 안되는 사람은
	// " remove-erase 합성문 " 으로 구글 검색 해보시길
	m_vectorRaidMessageList.erase(std::remove(m_vectorRaidMessageList.begin(), m_vectorRaidMessageList.end(), TempQuest));
	
	m_listSelectedQuestIndex.remove(iQuestIndex);
}

// =====================================================
//  [7/8/2009 selo]
// Name : RemoveRaidMessageAll
// Desc : 레이드 메시지를 모두 제거한다
// =====================================================
void CUIQuestBook::RemoveRaidMessageAll(void)
{
	std::vector<sQuestInfo>::iterator iter = m_vectorRaidMessageList.begin();
	while( iter != m_vectorRaidMessageList.end() )
	{
		m_listSelectedQuestIndex.remove((*iter).iQuestIndex);

		++iter;
	}
	CUIManager::getSingleton()->GetPlayerInfo()->UpdateSelectedQuest();

	m_vectorRaidMessageList.clear();
	RaidEnd();
}

// =====================================================
//  [7/8/2009 selo]
// Name : RefreshQuestContentByRaid
// Desc : 퀘스트 리스트에서 레이드 퀘스트를 선택하면
//		  RefreshQuestContent() 함수 대신에 불린다
// =====================================================
void CUIQuestBook::RefreshQuestContentByRaid(const int& iQuestIndex)
{
	m_btnGiveUp.SetEnable(FALSE);

	if( iQuestIndex != m_nSelectedQuestIndex )
		return;
	
	int oldSbPos = m_lbDescription.GetScrollBarPos();
	m_lbDescription.Reset();
	m_lbDescription.ResetAllStrings();

	CQuestDynamicData pQuestDD(CQuestSystem::Instance().GetStaticData(iQuestIndex));	

	AddDescString( pQuestDD.GetTitleDesc( 0 ), pQuestDD.GetColorTitleDesc( 0 ) );
	AddDescString( CTString("\n"), 0xFFFFFFFF );

	AddDescString( _S(3852, "내용" ), 0xFFFFFFFF );
	AddDescString( CTString(pQuestDD.GetDesc()), pQuestDD.GetColorIntroDesc() );		
	AddDescString( CTString("\n"), 0xFFFFFFFF );		

	AddDescString( CTString(pQuestDD.GetDesc3()), 0x0070C0FF );

	m_lbDescription.SetScrollBarPos(oldSbPos);
}

// =====================================================
//  [7/8/2009 selo]
// Name : isRaidMessage
// Desc : 해당 퀘스트가 레이드 메시지인지 확인
// =====================================================
bool CUIQuestBook::isRaidMessage(const int& iQuestIndex)
{
	sQuestInfo	TempQuest;
	TempQuest.iQuestIndex	= iQuestIndex;

	if( std::count(m_vectorRaidMessageList.begin(), m_vectorRaidMessageList.end(), TempQuest) )
		return true;	

	return false;	
}

// =====================================================
//  [8/18/2009 selo]
// Name : GetQuestNoticeElapsedTime
// Desc : 퀘스트 공지를 띄운 후 흐른 시간을 알려준다.
// =====================================================
DOUBLE CUIQuestBook::GetQuestNoticeElapsedTime()
{
	return _pTimer->GetHighPrecisionTimer().GetSeconds() - m_dQuestNoticeStartTime;	
}

// =====================================================
//  [9/7/2009 selo]
// Name : RaidEnd
// Desc : 레이드 끝
// =====================================================
void CUIQuestBook::RaidEnd(void)
{
	m_bIsRaidNow = false;
	m_dRaidRemainTime = -1;	
}

// =====================================================
//  [9/7/2009 selo]
// Name : RaidStart
// Desc : 레이드 남은 시간 설정
// =====================================================
void CUIQuestBook::SetRaidRemainTime(INDEX iRemainTime)
{
	m_bIsRaidNow = true;
	m_dRaidRemainTime = iRemainTime;
	m_dRaidRemainTimeRecvTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
}

// =====================================================
//  [9/7/2009 selo]
// Name : GetRaidTargetTime
// Desc : 레이드 목표 시간 얻기
// =====================================================
DOUBLE CUIQuestBook::GetRaidReminTime(void)
{	
	DOUBLE dOffset = _pTimer->GetHighPrecisionTimer().GetSeconds() - m_dRaidRemainTimeRecvTime;
	return m_dRaidRemainTime - dOffset;
}

// =====================================================
//  [9/7/2009 selo]
// Name : IsRaidNow
// Desc : 지금 레이드 중인가?
// =====================================================
BOOL CUIQuestBook::IsRaidNow(void)
{
	return (CUIManager::getSingleton()->IsPlayInZone() && m_bIsRaidNow);
}

// =====================================================
//  [5/29/2009 selo]
// Name : CreateQuestListNew()
// Desc : 퀘스트 리스트를 확장하기 위한 새로운 생성 함수
// =====================================================
void CUIQuestBook::CreateQuestListNew( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	// ! Notice : nWidth, nHeight 는 사용하지 않고 
	// 미리 설정한 Width와 Height를 사용 하기로한다.

	CUIWindow::Create(pParentWnd, nX, nY, iQuestListWidth, iQuestListHeight + 30);

	// 브라질친화도보상아이탬버그 [1/16/2013 Ranma]
	_iMaxDescStringCharNew = 192 / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	_iMaxSelStringCharNew = 191 / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	_iMaxDescStringChar = _iMaxDescStringCharNew;
	_iMaxSelStringChar = _iMaxSelStringCharNew;
	_nMsgBoxLineHeight = _pUIFontTexMgr->GetFontHeight() + 4;
	

	// Region of each part
	m_rcTitle.SetRect( 0, 0, iQuestListWidth, iQuestListTitleHeight );	

	// Create message box texture
	m_ptdBaseTexture = CreateTexture( strQuestListTexFileName );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	
	if( UI_QUESTBOOK_LIST == m_nUIIndex )
		m_rtTopL.SetUV( 0, 0, iQuestListWidth, iQuestListHeight, fTexWidth, fTexHeight );
	else
		m_rtTopL.SetUV( 0, 386, 251, 386+332, fTexWidth, fTexHeight );


	// Sort Combobox	
	m_cmbSort.Create( this, 9, 306, 155, 17, 155-20, 1, 15, 15,
						8, _pUIFontTexMgr->GetFontHeight() + 4, 4, 4 );
	m_cmbSort.SetBackUV( 9, 306, 164, 323, fTexWidth, fTexHeight );
	m_cmbSort.SetDownBtnUV( 144, 307, 159, 322, fTexWidth, fTexHeight );
	m_cmbSort.SetUpBtnUV( 144, 307, 159, 322, fTexWidth, fTexHeight );	
	m_cmbSort.SetDropListUV( 0, 363, 155, 378, fTexWidth, fTexHeight );	
	
	m_cmbSort.AddString(_S(4425, "진행 상태에 따른 정렬"));
	m_cmbSort.AddString(_S(4426, "종류에 따른 정렬"));
	m_cmbSort.SetCurSel(0);

	m_ptdCommonBtnTexture = CreateTexture( strCommonBtnTexFileName );
	fTexWidth = m_ptdCommonBtnTexture->GetPixWidth();
	fTexHeight = m_ptdCommonBtnTexture->GetPixHeight();

	// OK button
	m_btnOK.Create( this, _S( 191, "확인" ), 165, 304, 78, 22 );
	m_btnOK.SetUV( UBS_IDLE, 113, 0, 182, 22, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 186, 0, 256, 22, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );
	

	// Deny button
	m_btnDeny.Create( this, _S( 1696, "거절" ), 165, 304, 78, 22 );			
	m_btnDeny.SetUV( UBS_IDLE, 113, 0, 182, 22, fTexWidth, fTexHeight );
	m_btnDeny.SetUV( UBS_CLICK, 186, 0, 256, 22, fTexWidth, fTexHeight );
	m_btnDeny.CopyUV( UBS_IDLE, UBS_ON );
	m_btnDeny.CopyUV( UBS_IDLE, UBS_DISABLE );
		
	
	// Accept button	
	m_btnAccept.Create( this, _S( 1698, "수락" ), 7, 304, 78, 22 );		
	m_btnAccept.SetUV( UBS_IDLE, 113, 0, 182, 22, fTexWidth, fTexHeight );
	m_btnAccept.SetUV( UBS_CLICK, 186, 0, 256, 22, fTexWidth, fTexHeight );
	m_btnAccept.CopyUV( UBS_IDLE, UBS_ON );
	m_btnAccept.CopyUV( UBS_IDLE, UBS_DISABLE );
	

	// Reserve button
	m_btnReserve.Create( this, _S( 1699, "보류" ), 86, 304, 78, 22 );		
	m_btnReserve.SetUV( UBS_IDLE, 113, 0, 182, 22, fTexWidth, fTexHeight );
	m_btnReserve.SetUV( UBS_CLICK, 186, 0, 256, 22, fTexWidth, fTexHeight );
	m_btnReserve.CopyUV( UBS_IDLE, UBS_ON );
	m_btnReserve.CopyUV( UBS_IDLE, UBS_DISABLE );

	
	// Prev button
	m_btnPrev.Create( this, _S( 2681, "이전" ), 86, 304, 78, 22 );		
	m_btnPrev.SetUV( UBS_IDLE, 113, 0, 182, 22, fTexWidth, fTexHeight );
	m_btnPrev.SetUV( UBS_CLICK, 186, 0, 256, 22, fTexWidth, fTexHeight );
	m_btnPrev.CopyUV( UBS_IDLE, UBS_ON );
	m_btnPrev.CopyUV( UBS_IDLE, UBS_DISABLE );

	
	// Close button
	if( UI_QUESTBOOK_LIST == m_nUIIndex )
		m_btnClose.Create( this, CTString( "" ), 452, 4, 16, 16 );
	else
		m_btnClose.Create( this, CTString( "" ), 227, 4, 16, 16 );		
	m_btnClose.SetUV( UBS_IDLE, 211, 33, 227, 49, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 229, 33, 245, 49, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );	

	
	// Cancel button
	if( UI_QUESTBOOK_LIST == m_nUIIndex )
	{
		m_btnCancel.Create( this, _S( 1695, "보상" ), 388, 304, 78, 22 );
	}
	else
		m_btnCancel.Create( this, _S( 870, "닫기" ), 165, 304, 78, 22 );
	m_btnCancel.SetUV( UBS_IDLE, 113, 0, 182, 22, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 186, 0, 256, 22, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );
	
			
	// Give Up Button
	UIRect rcLeft, rcMiddel, rcRight;
	UIRectUV3 rtIdleBtn, rtClickBtn;

	rcLeft.SetRect(0, 0, 14, 21);
	rcMiddel.SetRect(14, 0, 88, 21);
	rcRight.SetRect(88, 0, 102, 21);

	rtIdleBtn.SetUV3(113, 0, 127, 22, 127, 0, 168, 22, 168, 0, 183, 22, fTexWidth, fTexHeight );
	rtClickBtn.SetUV3(187, 0, 201, 22, 201, 0, 242, 22, 242, 0, 256, 22, fTexWidth, fTexHeight);
	
	m_btnGiveUp.Create( this, _S( 1697, "퀘스트 포기" ), 280, 304, 102, 22 );
	m_btnGiveUp.SetRTSurface( UBS_IDLE, rcLeft, rtIdleBtn.rtL );
	m_btnGiveUp.SetRTSurface( UBS_IDLE, rcMiddel, rtIdleBtn.rtM );
	m_btnGiveUp.SetRTSurface( UBS_IDLE, rcRight, rtIdleBtn.rtR );
	m_btnGiveUp.SetRTSurface( UBS_CLICK, rcLeft, rtClickBtn.rtL );
	m_btnGiveUp.SetRTSurface( UBS_CLICK, rcMiddel, rtClickBtn.rtM );
	m_btnGiveUp.SetRTSurface( UBS_CLICK, rcRight, rtClickBtn.rtR );
	m_btnGiveUp.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnGiveUp.CopyRTSurface( UBS_IDLE, UBS_DISABLE );
	m_btnGiveUp.SetNewType(TRUE);


	// Quest List box	
	m_lbQuestList.Create( this, 14, 46, 204, 246, _nMsgBoxLineHeight, 12, 8, 2, TRUE );
	m_lbQuestList.CreateScroll( TRUE, 0, 0, 10, 246, 10, 10, 0, 0, 10 );
	m_lbQuestList.SetSelBar( 204, _pUIFontTexMgr->GetLineHeight(), 145, 138, 240, 172, fTexWidth, fTexHeight );
	m_lbQuestList.SetOverColor( 0xF8E1B5FF );
	m_lbQuestList.SetSelectColor( 0xF8E1B5FF );
	m_lbQuestList.SetColumnPosX( 1, 10 );
	// Up button
	m_lbQuestList.SetScrollUpUV( UBS_IDLE, 156, 33, 166, 43, fTexWidth, fTexHeight );
	m_lbQuestList.SetScrollUpUV( UBS_CLICK, 168, 33, 178, 43, fTexWidth, fTexHeight );
	m_lbQuestList.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbQuestList.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbQuestList.SetScrollDownUV( UBS_IDLE, 156, 45, 166, 55, fTexWidth, fTexHeight );
	m_lbQuestList.SetScrollDownUV( UBS_CLICK, 168, 45, 178, 55, fTexWidth, fTexHeight );
	m_lbQuestList.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbQuestList.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbQuestList.SetScrollBarTopUV( 185, 32, 195, 42, fTexWidth, fTexHeight );
	m_lbQuestList.SetScrollBarMiddleUV( 185, 41, 195, 51, fTexWidth, fTexHeight );
	m_lbQuestList.SetScrollBarBottomUV( 185, 61, 195, 71, fTexWidth, fTexHeight );
	// pop button	
	m_lbQuestList.SetPopBtnSpace(1, 0);
	m_lbQuestList.SetPopBtnSize(15, 15);
	m_lbQuestList.SetPopBtnUV( UCBS_NONE, 241, 57, 241+15, 57+15, fTexWidth, fTexHeight);
	m_lbQuestList.SetPopBtnUV( UCBS_CHECK, 227, 57, 227+15, 57+15, fTexWidth, fTexHeight);
	m_lbQuestList.SetPopBtnUV( UCBS_NONE_DISABLE, 241, 57, 241+15, 57+15, fTexWidth, fTexHeight);
	m_lbQuestList.SetPopBtnUV( UCBS_CHECK_DISABLE, 227, 57, 227+15, 57+15, fTexWidth, fTexHeight);	
	m_lbQuestList.SetCheckBtnSpace(1, 0);
	m_lbQuestList.SetCheckBtnSize(13, 13);
	m_lbQuestList.SetCheckBtnUV( UCBS_NONE, 139, 75, 152, 88, fTexWidth, fTexHeight );
	m_lbQuestList.SetCheckBtnUV( UCBS_CHECK, 119, 75, 132, 88, fTexWidth, fTexHeight );
	m_lbQuestList.SetCheckBtnUV( UCBS_NONE, 139, 75, 152, 88, fTexWidth, fTexHeight );
	m_lbQuestList.SetCheckBtnUV( UCBS_CHECK, 119, 75, 132, 88, fTexWidth, fTexHeight );	
	

	
	// Description List box
	if( UI_QUESTBOOK_LIST == m_nUIIndex )
		m_lbDescription.Create( this, 244, 46, 204, 246, _nMsgBoxLineHeight, 12, 8, 1, TRUE );
	else
		m_lbDescription.Create( this, 14, 46, 210, 246, _nMsgBoxLineHeight, 12, 8, 1, TRUE );
	m_lbDescription.CreateScroll( TRUE, 0, 0, 10, 246, 10, 10, 0, 0, 10 );
	// Up button
	m_lbDescription.SetScrollUpUV( UBS_IDLE, 156, 33, 166, 43, fTexWidth, fTexHeight );
	m_lbDescription.SetScrollUpUV( UBS_CLICK, 168, 33, 178, 43, fTexWidth, fTexHeight );
	m_lbDescription.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbDescription.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbDescription.SetScrollDownUV( UBS_IDLE, 156, 45, 166, 55, fTexWidth, fTexHeight );
	m_lbDescription.SetScrollDownUV( UBS_CLICK, 168, 45, 178, 55, fTexWidth, fTexHeight );
	m_lbDescription.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbDescription.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbDescription.SetScrollBarTopUV( 185, 32, 195, 42, fTexWidth, fTexHeight );
	m_lbDescription.SetScrollBarMiddleUV( 185, 41, 195, 51, fTexWidth, fTexHeight );
	m_lbDescription.SetScrollBarBottomUV( 185, 61, 195, 71, fTexWidth, fTexHeight );
	m_lbDescription.SetSelectColor( 0xF8E1B5FF );

	// [090820: selo] 이미 보여진 퀘스트 공지를 저장할 배열
	m_aAlreadyShowQuestNotice.New(iQuestNoticeCnt);
	for( int i = 0; i < m_aAlreadyShowQuestNotice.Count(); ++i )
	{
		m_aAlreadyShowQuestNotice[i] = 0;
	}
}

// =====================================================
//  [5/29/2009 selo]
// Name : RefreshQuestListNew()
// Desc : 기존의 RefreshQuestList() 함수를 후킹하여
//		  이 곳에서 작업이 이루어지게 한다.
// =====================================================
void CUIQuestBook::RefreshQuestListNew()
{
	ASSERT(m_nUIIndex == UI_QUESTBOOK_LIST);
	
	m_nSelectedQuestIndex = -1;

	if( 0 == m_nSortType )		// 진행상태에 따른 정렬 일 때
	{	
		SortQuestListByCondition();		
	}
	else
	{
		SortQuestListByType();
	}
}

// =====================================================
//  [5/29/2009 selo]
// Name : RenderQuestListNew()
// Desc : 퀘스트 리스트 확장을 위한 랜더 함수
// =====================================================
void CUIQuestBook::RenderQuestListNew()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nY, nX2;
	nX2 = m_nPosX + m_nWidth;
	// Background
	// Top
	nY = m_nPosY + QUESTBOOK_TOP_HEIGHT;
	if( UI_QUESTBOOK_LIST == m_nUIIndex )
	{	
		pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + iQuestListWidth, m_nPosY + iQuestListHeight,
											m_rtTopL.U0, m_rtTopL.V0, m_rtTopL.U1, m_rtTopL.V1,
											0xFFFFFFFF );
		m_cmbSort.Render();
	}
	else
	{
		pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + 251, m_nPosY + 332,
											m_rtTopL.U0, m_rtTopL.V0, m_rtTopL.U1, m_rtTopL.V1,
											0xFFFFFFFF );
	}

	pDrawPort->FlushRenderingQueue();

	pDrawPort->InitTextureData(m_ptdCommonBtnTexture);
	
	// Close box
	m_btnClose.Render();
	if( m_nUIIndex == UI_QUESTBOOK_LIST )
	{	
		m_btnGiveUp.Render();
		m_btnCancel.Render();					// 닫기
		m_lbQuestList.Render();

		pDrawPort->InitTextureData(m_ptdCommonBtnTexture);
		m_lbDescription.Render();
	}

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Title
	pDrawPort->PutTextEx( m_strTitle, m_nPosX + 209, m_nPosY + 15, 0xDED9A0FF );

	// Flush all render text queue
	pDrawPort->EndTextEx();
}

// =====================================================
//  [5/29/2009 selo]
// Name : MsgProcQuestListNew()
// Desc : 퀘스트 리스트 확장 구현을 위해 마우스 메시지를 후킹한다.
// =====================================================
WMSG_RESULT CUIQuestBook::MsgProcQuestListNew( MSG *pMsg )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// quest book bug fix. [8/26/2010 rumist]
	if( m_bLockQuestList && (!pUIManager->DoesMessageBoxExist( MSGCMD_QUEST_GIVEUP )) )
		m_bLockQuestList = FALSE;

	if( TRUE == m_bLockQuestList )
		return WMSG_FAIL;

	WMSG_RESULT	wmsgResult;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				pUIManager->SetMouseCursorInsideUIs();

			// If message box isn't focused
			if( !IsFocused() )
				return WMSG_FAIL;

			// Move message box
			if( m_bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// buttons
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnGiveUp.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			// List box
			else if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_lbQuestList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;	
			
			else if( m_cmbSort.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					m_bTitleBarClick = TRUE;
				}
				// Cancel button
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				}
				// Sort Combobox				
				else if( ( wmsgResult = m_cmbSort.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						m_nSortType = m_cmbSort.GetCurSel();
						RefreshQuestListNew();
					}
				}				
				// List box
				else if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				}
				// Quest List Box
				else if( ( wmsgResult = m_lbQuestList.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					// [090731: selo] 스크롤바 위치 초기화
					m_lbDescription.SetScrollBarPos(0);
					
					// 진행 상태에 따른 정렬이면
					if( 0 == m_nSortType )
					{
						// 선택된 퀘스트.						
						int	iSelQuest = m_lbQuestList.GetCurSel();
						if( iSelQuest != -1 )
						{
							// 진행중인 퀘스트 중의 하나 선택.
							if( iSelQuest >= 1 && iSelQuest <= m_vectorProceedQuestList.size() + 1 )
							{
								m_nSelectedQuestIndex = m_vectorProceedQuestList[iSelQuest - 1].iQuestIndex;
								RefreshQuestContent( m_nSelectedQuestIndex );								
							}
							else if( iSelQuest >= ( m_vectorProceedQuestList.size() + 2 ) && iSelQuest < ( m_vectorProceedQuestList.size() + 2 ) + m_vectorCompleteQuestList.size() )
							{
								m_nSelectedQuestIndex = m_vectorCompleteQuestList[iSelQuest - m_vectorProceedQuestList.size() - 2].iQuestIndex;
								RefreshQuestContent( m_nSelectedQuestIndex );								
							}
							else if( iSelQuest >= ( m_vectorProceedQuestList.size() + m_vectorCompleteQuestList.size() + 3 ) &&
									 iSelQuest <  ( m_vectorProceedQuestList.size() + m_vectorCompleteQuestList.size() + 3 ) + m_vectorRaidMessageList.size() )
							{
								m_nSelectedQuestIndex = m_vectorRaidMessageList[iSelQuest - ( m_vectorProceedQuestList.size() + m_vectorCompleteQuestList.size() + 3 )].iQuestIndex;
								RefreshQuestContentByRaid( m_nSelectedQuestIndex );
							}

							// [090602: selo] 체크가 변경이면
							if( WMSG_COMMAND == wmsgResult )
							{
								// [090727: selo] 체크 개수가 6개 넘어가면 메시지 띄우고 무시
								if( m_lbQuestList.GetCheckCount() > 6 )
								{
									// 체크 풀고
									// 메시지 박스 띄운다
									m_lbQuestList.SetCheckState(iSelQuest, FALSE);

									CUIMsgBox_Info	MsgBoxInfo;		
									MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );				
									MsgBoxInfo.AddString( _S(4694, "6개 이상 수행 리스트를 출력할 수 없습니다.") );
									pUIManager->CreateMessageBox( MsgBoxInfo );									
								}
								else
								{								
									if( m_lbQuestList.GetCheckState(iSelQuest) )
										AddSelectedQuest(m_nSelectedQuestIndex);
									else
										RemoveSelectedQuest(m_nSelectedQuestIndex);
								}
							}

							return WMSG_SUCCESS;
						}
						int groupNameLine = 0;
						for(int i=0; i<m_lbQuestList.GetCurItemCount(0); ++i)
						{
							if(m_lbQuestList.GetState(i) == CUIListBoxEx::PS_CLOSE
								|| m_lbQuestList.GetState(i) == CUIListBoxEx::PS_EXTEND)
							{
								if(groupNameLine == 0)//proceed
								{
									m_bProceedListExtended = m_lbQuestList.GetState(i) == CUIListBoxEx::PS_EXTEND;
									++groupNameLine;
								}
								else if(groupNameLine == 1)//complete
								{
									m_bCompleteListExtended = m_lbQuestList.GetState(i) == CUIListBoxEx::PS_EXTEND;
									++groupNameLine;									
								}
								else if(groupNameLine == 2) // [090708: selo] 레이드 퀘스트
								{
									m_bRaidMessageListExtended = m_lbQuestList.GetState(i) == CUIListBoxEx::PS_EXTEND;
									++groupNameLine;
									break;
								}
							}
						}
					}
					else
					{
						// 선택된 퀘스트.						
						int	iSelQuest = m_lbQuestList.GetCurSel();
						if( iSelQuest != -1 )
						{
							// [090715: selo] 종류에 따른 정렬 처리 버그 수정
							// 아래의 식은 접은 것을 펼친것 으로 계산하여 작업한 것임

							int iGroupCnt = -1;
							int iLastGroupIndex = 0;
							
							for( int i = 0; i < m_lbQuestList.GetCurItemCount(0); ++i )
							{
								if(m_lbQuestList.GetState(i) == CUIListBoxEx::PS_CLOSE
								|| m_lbQuestList.GetState(i) == CUIListBoxEx::PS_EXTEND)
								{
									++iGroupCnt;
									iLastGroupIndex = i;

									bool bBreak = false;

									if( m_lbQuestList.GetState(i) == CUIListBoxEx::PS_CLOSE )
									{
										for( int j = i + 1; j < m_lbQuestList.GetCurItemCount(0); ++j )
										{
											if( m_lbQuestList.GetState(j) == CUIListBoxEx::PS_CLOSE
												|| m_lbQuestList.GetState(j) == CUIListBoxEx::PS_EXTEND )
											{												
												iSelQuest += (j - (i + 1));
												i += (j - (i +1));												
												bBreak = true;
												
												break;
											}
										}
									}

									if( bBreak )
										continue;
								}

								if( i == iSelQuest )
								{
									int nTemp = iSelQuest - (iLastGroupIndex + 1);	// 해당 그룹의 몇번째 라인인지
									if( iGroupCnt >= 0  && iGroupCnt < QCATEGORY_END )
									{
										m_nSelectedQuestIndex = m_vectorCategoryQuestList[iGroupCnt][nTemp].iQuestIndex;
										RefreshQuestContent(m_nSelectedQuestIndex);
									}
									else if( iGroupCnt >= QCATEGORY_END )
									{
										m_nSelectedQuestIndex = m_vectorRaidMessageList[nTemp].iQuestIndex;
										RefreshQuestContentByRaid(m_nSelectedQuestIndex);
									}
								}
							}

							// [090602: selo] 체크가 변경이면
							if( WMSG_COMMAND == wmsgResult )
							{
								// [090727: selo] 체크 개수가 6개 넘어가면 메시지 띄우고 무시
								if( m_lbQuestList.GetCheckCount() > 6 )
								{
									// 체크 풀고
									// 메시지 박스 띄운다
									m_lbQuestList.SetCheckState(iSelQuest, FALSE);

									CUIMsgBox_Info	MsgBoxInfo;		
									MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );				
									MsgBoxInfo.AddString( _S(4694, "6개 이상 수행 리스트를 출력할 수 없습니다.") );
									pUIManager->CreateMessageBox( MsgBoxInfo );									
								}
								else
								{								
									if( m_lbQuestList.GetCheckState(iSelQuest) )
										AddSelectedQuest(m_nSelectedQuestIndex);
									else
										RemoveSelectedQuest(m_nSelectedQuestIndex);
								}
							}

							return WMSG_SUCCESS;
						}
						int groupNameLine = 0;
						for(int i=0; i<m_lbQuestList.GetCurItemCount(0); ++i)
						{
							if(m_lbQuestList.GetState(i) == CUIListBoxEx::PS_CLOSE
								|| m_lbQuestList.GetState(i) == CUIListBoxEx::PS_EXTEND)
							{
								if( groupNameLine < QCATEGORY_END)
								{
									m_bCategoryListExtended[groupNameLine] = m_lbQuestList.GetState(i) == CUIListBoxEx::PS_EXTEND;
									++groupNameLine;								
								}
								else	// [090708: selo] 레이드 퀘스트
								{
									m_bRaidMessageListExtended = m_lbQuestList.GetState(i) == CUIListBoxEx::PS_EXTEND;
									++groupNameLine;
									break;
								}
							}
						}

					}
				}
				else if( m_btnGiveUp.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				}				

				pUIManager->RearrangeOrder( m_nUIIndex, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			// If holding button doesn't exist
			if( pUIManager->GetHoldBtn().IsEmpty() )
			{
				// Title bar
				m_bTitleBarClick = FALSE;
				
				// If message box isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;
				
				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseQuestBook();
					
					return WMSG_SUCCESS;
				}
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						pUIManager->GetQuestBookComplete()->open(m_nSelectedQuestIndex);
					}

					return WMSG_SUCCESS;
				}
				// Give up button				
				else if( m_nSelectedQuestIndex != -1 && ( wmsgResult = m_btnGiveUp.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{						
					pUIManager->CloseMessageBox(MSGCMD_QUEST_GIVEUP);
					CTString	strMessage;
					CUIMsgBox_Info	MsgBoxInfo;	
					MsgBoxInfo.SetMsgBoxInfo( _S( 99, "퀘스트" ), UMBS_OKCANCEL, m_nUIIndex, MSGCMD_QUEST_GIVEUP);	
					strMessage.PrintF( _S( 504, "정말 이 퀘스트를 취소하시겠습니까?" ));	
					MsgBoxInfo.AddString( strMessage );
					pUIManager->CreateMessageBox( MsgBoxInfo );
					m_bLockQuestList = TRUE;
					return WMSG_SUCCESS;
				}
				// List box
				else if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_lbQuestList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_cmbSort.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					// Reset holding button
					pUIManager->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				// List box
				if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_lbQuestList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				// List box
				if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_lbQuestList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}