#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <vector>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIRemission.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Contents/Base/UIQuestNew.h>
#include <Engine/Contents/Base/UIQuestBookNew.h>

enum eSelection
{
	REMISSION_OK,
	REMISSION_TALK,
	REMISSION_EVENT,
};

// Date : 2005-03-07,   By Lee Ki-hwan
static int	_iMaxMsgStringChar = 0;

// ----------------------------------------------------------------------------
// Name : CUIRemission()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIRemission::CUIRemission()
{
	m_nSelRemissionID = -1;
}

// ----------------------------------------------------------------------------
// Name : ~CUIRemission()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIRemission::~CUIRemission()
{
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIRemission::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);
	
	_iMaxMsgStringChar = REMISSION_DESC_CHAR_WIDTH / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );
	m_rcIcons.SetRect( 17, 55, 51, 200 );
	
	// Create remission texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\SkillLearn.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();
	
	// UV Coordinate of each part
	m_rtBackground.SetUV( 0, 0, 216, 400, fTexWidth, fTexHeight );
	m_rtSelOutline.SetUV( 218, 80, 250, 112, fTexWidth, fTexHeight );
	m_rtTabLine.SetUV( 12, 31, 13, 50, fTexWidth, fTexHeight );
	
	// Close button
	m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 219, 15, 233, 29, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	// Buy button
	m_btnOK.Create( this, _S( 600, "구입" ), 70, 372, 63, 21 );		
	m_btnOK.SetUV( UBS_IDLE, 0, 403, 63, 424, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 66, 403, 129, 424, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	// Cancel button
	m_btnCancel.Create( this, _S( 139, "취소" ), 141, 372, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 403, 63, 424, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 403, 129, 424, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	// Scroll bar of special remission icons
	m_sbRemissionIcon.Create( this, 193, 51, 9, 153 );
	m_sbRemissionIcon.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbRemissionIcon.SetScrollPos( 0 );
	m_sbRemissionIcon.SetScrollRange( REMISSION_SLOT_ROW_TOTAL );
	m_sbRemissionIcon.SetCurItemCount( 0 );
	m_sbRemissionIcon.SetItemsPerPage( REMISSION_SLOT_ROW );
	// Up button
	m_sbRemissionIcon.SetUpUV( UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight );
	m_sbRemissionIcon.SetUpUV( UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight );
	m_sbRemissionIcon.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbRemissionIcon.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbRemissionIcon.SetBarTopUV( 219, 32, 228, 42, fTexWidth, fTexHeight );
	m_sbRemissionIcon.SetBarMiddleUV( 219, 43, 228, 45, fTexWidth, fTexHeight );
	m_sbRemissionIcon.SetBarBottomUV( 219, 46, 228, 56, fTexWidth, fTexHeight );
	// Down button
	m_sbRemissionIcon.SetDownUV( UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight );
	m_sbRemissionIcon.SetDownUV( UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight );
	m_sbRemissionIcon.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbRemissionIcon.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Wheel region
	m_sbRemissionIcon.SetWheelRect( -181, -1, 180, 154 );
	
	// List box of remission description
	m_lbRemissionDesc.Create( this, 13, 207, 180, 124, _pUIFontTexMgr->GetLineHeight(), 7, 6, 1, FALSE );
	m_lbRemissionDesc.CreateScroll( TRUE, 0, 0, 9, 124, 9, 7, 0, 0, 10 );
	// Up button
	m_lbRemissionDesc.SetScrollUpUV( UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight );
	m_lbRemissionDesc.SetScrollUpUV( UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight );
	m_lbRemissionDesc.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbRemissionDesc.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbRemissionDesc.SetScrollBarTopUV( 219, 32, 228, 42, fTexWidth, fTexHeight );
	m_lbRemissionDesc.SetScrollBarMiddleUV( 219, 43, 228, 45, fTexWidth, fTexHeight );
	m_lbRemissionDesc.SetScrollBarBottomUV( 219, 46, 228, 56, fTexWidth, fTexHeight );
	// Down button
	m_lbRemissionDesc.SetScrollDownUV( UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight );
	m_lbRemissionDesc.SetScrollDownUV( UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight );
	m_lbRemissionDesc.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbRemissionDesc.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIRemission::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIRemission::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : InitRemission()
// Desc :
// ----------------------------------------------------------------------------
BOOL CUIRemission::InitRemission(  )
{
	// Reset description
	m_lbRemissionDesc.ResetAllStrings();
	m_vectorbtnRemissions.clear();
	m_nSelRemissionID	= -1;

	CUIButtonEx TempUIButtonEx;

	if(_pNetwork->MyCharacterInfo.hpcount > 0)		// HP 패널티 카운트가 있다면...
	{
		TempUIButtonEx.Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_REMISSION, UBET_REMISSION );
		TempUIButtonEx.SetRemissionInfo( REMISSION_HP );
		m_vectorbtnRemissions.push_back ( TempUIButtonEx );
	}
	
	if(_pNetwork->MyCharacterInfo.mpcount > 0)		// MP 패널티 카운트가 있다면...
	{
		TempUIButtonEx.Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_REMISSION, UBET_REMISSION ); 
		TempUIButtonEx.SetRemissionInfo( REMISSION_MP );
		m_vectorbtnRemissions.push_back ( TempUIButtonEx );
	}

	int	i;
	for( i = 0; i < ITEM_COUNT_IN_INVENTORY_NORMAL; i++ )
	{
		const SBYTE sbWearPos	= _pNetwork->MySlotItem[0][i].Item_Wearing;
		CItemData*	pItemData = _pNetwork->MySlotItem[0][i].ItemData;
		
		if (pItemData != NULL &&
			((pItemData->GetType() == CItemData::ITEM_WEAPON) ||		//착용 중이 아닌 아이템도 봉인 해제될 수 있게
			(pItemData->GetType() == CItemData::ITEM_SHIELD)) )
		{	
			if( _pNetwork->MySlotItem[0][i].Item_Flag & FLAG_ITEM_SEALED )
			{	
				const SWORD nTab		= _pNetwork->MySlotItem[0][i].Item_Tab;
				const SWORD nIdx		= _pNetwork->MySlotItem[0][i].InvenIndex;		
				const LONG lIndex		= _pNetwork->MySlotItem[0][i].Item_Index;
				const LONG lUniIndex	= _pNetwork->MySlotItem[0][i].Item_UniIndex;					
					
				TempUIButtonEx.Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_REMISSION, UBET_REMISSION ); 
				TempUIButtonEx.SetRemissionInfo( REMISSION_ITEM, nTab, nIdx, lIndex, lUniIndex, sbWearPos );
				m_vectorbtnRemissions.push_back ( TempUIButtonEx );
			}
		}
	}

	// Set special scroll bar
	m_sbRemissionIcon.SetScrollPos( 0 );
	m_sbRemissionIcon.SetCurItemCount( m_vectorbtnRemissions.size()  );

	CUIManager::getSingleton()->GetInventory()->Lock( FALSE, FALSE, LOCK_REMISSION );

	if(m_vectorbtnRemissions.empty())
		return FALSE;
	return TRUE;
}

// ----------------------------------------------------------------------------
// Name : OpenRemission()
// Desc : 
// ----------------------------------------------------------------------------
void CUIRemission::OpenRemission( int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// If this is already exist
	if( pUIManager->DoesMessageBoxLExist( MSGLCMD_REMISSION_REQ ) || IsVisible() )
		return;

	// If inventory is already locked
	if( pUIManager->GetInventory()->IsLocked() )
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	// Set position of target npc
	m_fNpcX = fX;
	m_fNpcZ = fZ;

	CMobData* MD = CMobData::getData(iMobIndex);
	
	//if(MD->IsRemission())		// FIXME : 면죄부 NPC 인지 확인.
	{
		// Create remission message box
		pUIManager->CreateMessageBoxL( _S( 601, "면죄부" ), UI_REMISSION, MSGLCMD_REMISSION_REQ );		

		CTString	strNpcName = CMobData::getData(iMobIndex)->GetName();
		pUIManager->AddMessageBoxLString( MSGLCMD_REMISSION_REQ, TRUE, strNpcName, -1, 0xE18600FF );

		pUIManager->AddMessageBoxLString( MSGLCMD_REMISSION_REQ, TRUE, _S( 602, "그대의 어깨를 누르고 있는 짐이 무거워 보이는구려." ), -1, 0xA3A1A3FF );		
		pUIManager->AddMessageBoxLString( MSGLCMD_REMISSION_REQ, TRUE, _S( 603, "그대가 아무리 큰 죄를 지었다고 하더라도, 아들을 사랑하는 신의 마음은 그보다 더 깊고 넓으시다오! 지금이라도 신께 용서를 구하시오." ), -1, 0xA3A1A3FF );		
		pUIManager->AddMessageBoxLString( MSGLCMD_REMISSION_REQ, TRUE, _S( 604, "자 어리석은 그대를 위하여, 내 조금이나마 고통을 덜어내 드리겠소." ), -1, 0xA3A1A3FF );		
		
		pUIManager->AddMessageBoxLString( MSGLCMD_REMISSION_REQ, FALSE, _S( 605, "수락한다." ), REMISSION_OK  );		
		pUIManager->AddMessageBoxLString( MSGLCMD_REMISSION_REQ, FALSE, _S( 606, "돌아간다." ) );		

		if(bHasQuest)
		{
			// 2009. 05. 27 김정래
			// 이야기한다 변경 처리
			CUIQuestBook::AddQuestListToMessageBoxL(MSGLCMD_REMISSION_REQ);				
		}

		// FIXME : 퀘스트가 없을 경우에 문제가 됨.
		// FIXME : 고로, 이벤트 NPC는 되도록 퀘스트를 갖고 있는 형태로???
		if(MD->IsEvent())
		{
			pUIManager->AddMessageBoxLString( MSGLCMD_REMISSION_REQ, FALSE, _S( 100, "이벤트" ), REMISSION_EVENT  );		
		}
	}
	
	m_nSelRemissionID	= -1;
	
	// Character state flags
	pUIManager->SetCSFlagOn( CSF_REMISSION );
}

// ----------------------------------------------------------------------------
// Name : CloseRemission()
// Desc :
// ----------------------------------------------------------------------------
void CUIRemission::CloseRemission()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box of remission
	pUIManager->CloseMessageBox( MSGCMD_REMISSION_NOTIFY );
	pUIManager->CloseMessageBox( MSGCMD_REMISSION_BUY );

	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_REMISSION );
	
	pUIManager->RearrangeOrder( UI_REMISSION, FALSE );
	
	// Character state flags
	pUIManager->SetCSFlagOff( CSF_REMISSION );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIRemission::Render()
{
	// Check distance
	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;
	if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST )
		CloseRemission();

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set remission texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	
	// Add render regions
	// Background
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
		m_rtBackground.U0, m_rtBackground.V0, m_rtBackground.U1, m_rtBackground.V1,
		0xFFFFFFFF );	
	
	// Close button
	m_btnClose.Render();
	
	// Buy button
	m_btnOK.Render();
	
	// Cancel button
	m_btnCancel.Render();
	
	m_sbRemissionIcon.Render();
	
	// List box of remission desc
	m_lbRemissionDesc.Render();
	
	// Render all elements
	pDrawPort->FlushRenderingQueue();
	
	// Remission buttons
	RenderRemissionBtns();
	
	// Text in remission
	pDrawPort->PutTextEx( _S( 601, "면죄부" ), m_nPosX + REMISSION_TITLE_TEXT_OFFSETX,	
		m_nPosY + REMISSION_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );
	
	pDrawPort->PutTextExCX( _S( 601, "면죄부" ), m_nPosX + REMISSION_TAB_CX, m_nPosY + REMISSION_TAB_SY,		
		0x6B6B6BFF );
	
	pDrawPort->PutTextEx( _S( 609, "현재 나스" ), m_nPosX + REMISSION_CURSP_SX,	
		m_nPosY + REMISSION_CURSP_SY );

	CUIManager* pUIManager = CUIManager::getSingleton();

	// Set money
	CTString strPlayerMoney;
	if( _pNetwork->MyCharacterInfo.money > 0 )
	{
		strPlayerMoney.PrintF( "%I64d", _pNetwork->MyCharacterInfo.money );
		pUIManager->InsertCommaToString( strPlayerMoney );
	}

	pDrawPort->PutTextExRX( strPlayerMoney,
		m_nPosX + REMISSION_CURSP_RX, m_nPosY + REMISSION_CURSP_SY, pUIManager->GetNasColor( strPlayerMoney ) );
	
	// Flush all render text queue
	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : AddRemissionDescString()
// Desc :
// ----------------------------------------------------------------------------
void CUIRemission::AddRemissionDescString( CTString &strDesc, const COLOR colDesc )
{
	// Get length of string
	INDEX	nLength = strDesc.Length();
	if( nLength == 0 )
		return;
	
	int		iPos;
	// wooss 051002
#if defined G_THAI
	// Get length of string
	INDEX	nThaiLen = FindThaiLen(strDesc);
	INDEX	nChatMax= (_iMaxMsgStringChar-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
	if( nLength == 0 )
		return;
	// If length of string is less than max char
	if( nThaiLen <= nChatMax )
	{
		// Check line character
		for( iPos = 0; iPos < nLength; iPos++ )
		{
			if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
				break;
		}
		
		// Not exist
		if( iPos == nLength )
		{
			m_lbRemissionDesc.AddString( 0, strDesc, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbRemissionDesc.AddString( 0, strTemp2, colDesc );
			
			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );
			
			AddRemissionDescString( strTemp, colDesc );
		}
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = _iMaxMsgStringChar;
		BOOL	b2ByteChar = FALSE;
		for( iPos = 0; iPos < nLength; iPos++ )
		{
			if(nChatMax < FindThaiLen(strDesc,0,iPos))
				break;
		}
		nSplitPos = iPos;

		// Check line character
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
				break;
		}
		
		// Not exist
		if( iPos == nSplitPos )
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( nSplitPos, strTemp2, strTemp );
			m_lbRemissionDesc.AddString( 0, strTemp2, colDesc );
			
			// Trim space
			if( strTemp[0] == ' ' )
			{
				int	nTempLength = strTemp.Length();
				for( iPos = 1; iPos < nTempLength; iPos++ )
				{
					if( strTemp[iPos] != ' ' )
						break;
				}
				
				strTemp.TrimLeft( strTemp.Length() - iPos );
			}
			
			AddRemissionDescString( strTemp, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbRemissionDesc.AddString( 0, strTemp2, colDesc );
			
			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );
			
			AddRemissionDescString( strTemp, colDesc );
		}

	}
#else	
	// If length of string is less than max char
	if( nLength <= _iMaxMsgStringChar )
	{
		// Check line character		
		for( iPos = 0; iPos < nLength; iPos++ )
		{
			if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
				break;
		}
		
		// Not exist
		if( iPos == nLength )
		{
			m_lbRemissionDesc.AddString( 0, strDesc, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbRemissionDesc.AddString( 0, strTemp2, colDesc );
			
			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );
			
			AddRemissionDescString( strTemp, colDesc );
		}
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = _iMaxMsgStringChar;
		BOOL	b2ByteChar = FALSE;
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strDesc[iPos] & 0x80 )
				b2ByteChar = !b2ByteChar;
			else
				b2ByteChar = FALSE;
		}
		
		if( b2ByteChar )
			nSplitPos--;
		
		// Check line character
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
				break;
		}
		
		// Not exist
		if( iPos == nSplitPos )
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( nSplitPos, strTemp2, strTemp );
			m_lbRemissionDesc.AddString( 0, strTemp2, colDesc );
			
			// Trim space
			if( strTemp[0] == ' ' )
			{
				int	nTempLength = strTemp.Length();
				for( iPos = 1; iPos < nTempLength; iPos++ )
				{
					if( strTemp[iPos] != ' ' )
						break;
				}
				
				strTemp.TrimLeft( strTemp.Length() - iPos );
			}
			
			AddRemissionDescString( strTemp, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbRemissionDesc.AddString( 0, strTemp2, colDesc );
			
			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );
			
			AddRemissionDescString( strTemp, colDesc );
		}
	}
#endif
}

// ----------------------------------------------------------------------------
// Name : GetRemissionDesc()
// Desc :
// ----------------------------------------------------------------------------
void CUIRemission::GetRemissionDesc( int iRemissionType, int nIndex, SWORD nIdx )
{
	m_lbRemissionDesc.ResetAllStrings();
	
	// If remission is not exist
	if( iRemissionType == REMISSION_NONE )
		return;
	
	// Make description of remission
	CTString	strTemp;

	switch(iRemissionType)
	{
	case REMISSION_HP:
		{
			strTemp.PrintF( _S( 610, "육체의 면죄부\n\n" ) );	
			AddRemissionDescString( strTemp, 0xFFC672FF );
			strTemp.PrintF( _S( 611, "HP패널티 받은 수 : %d회\n\n" ), _pNetwork->MyCharacterInfo.hpcount);	
			AddRemissionDescString( strTemp, 0xFFC672FF );
		}
		break;
	case REMISSION_MP:
		{
			strTemp.PrintF( _S( 612, "영혼의 면죄부\n\n" ) );	
			AddRemissionDescString( strTemp, 0xFFC672FF );
			strTemp.PrintF( _S( 613, "MP패널티 받은 수 : %d회\n\n" ), _pNetwork->MyCharacterInfo.mpcount);	
			AddRemissionDescString( strTemp, 0xFFC672FF );
		}
		break;
	case REMISSION_ITEM:
		{		
			strTemp.PrintF( _S( 614, "봉인된 장비명\n\n" ) );							
			AddRemissionDescString( strTemp, 0xFFC672FF );

			strTemp.PrintF( "%s\n\n", _pNetwork->GetItemName( nIndex ) );	
			AddRemissionDescString( strTemp, 0xFFC672FF );
		}
		break;
	}
	strTemp.PrintF( _S( 615, "면죄부 가격 : %I64d 나스" ), CalculatePrice(iRemissionType, nIdx) );	
	AddRemissionDescString( strTemp, 0xFFC672FF );
}

// ----------------------------------------------------------------------------
// Name : RenderRemissionBtns()
// Desc :
// ----------------------------------------------------------------------------
void CUIRemission::RenderRemissionBtns()
{
	int	nX = REMISSION_SLOT_SX, nY = REMISSION_SLOT_SY;
	int	iRow, iRowS, iRowE;	
	
	// Remission remission button
	iRowS = m_sbRemissionIcon.GetScrollPos();		
	iRowE = iRowS + REMISSION_SLOT_ROW;

	if ( m_vectorbtnRemissions.size() < iRowE )
	{
		iRowE = m_vectorbtnRemissions.size();
	}

	for( iRow = iRowS; iRow < iRowE; iRow++, nY += REMISSION_SLOT_OFFSETY )
	{
		m_vectorbtnRemissions[iRow].SetPos( nX, nY );
		if( m_vectorbtnRemissions[iRow].IsEmpty() )		
			continue;
		
		m_vectorbtnRemissions[iRow].Render();
	}

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Render all button elements
	pDrawPort->FlushBtnRenderingQueue( UBET_REMISSION );
	
	if( m_nSelRemissionID >= 0 )
	{
		// Set remission learn texture
		pDrawPort->InitTextureData( m_ptdBaseTexture );
		
		m_vectorbtnRemissions[m_nSelRemissionID].GetAbsPos( nX, nY );
		pDrawPort->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
												m_rtSelOutline.U0, m_rtSelOutline.V0,
												m_rtSelOutline.U1, m_rtSelOutline.V1,
												0xFFFFFFFF );
		
		// Render all elements
		pDrawPort->FlushRenderingQueue();
	}
	
	nY = REMISSION_NAME_SY;
	
	iRowS = m_sbRemissionIcon.GetScrollPos();
	iRowE = iRowS + REMISSION_SLOT_ROW;
	if ( m_vectorbtnRemissions.size() < iRowE )
	{
		iRowE = m_vectorbtnRemissions.size();
	}
	for( iRow = iRowS; iRow < iRowE; iRow++, nY += REMISSION_SLOT_OFFSETY )
	{
		if( m_vectorbtnRemissions[iRow].IsEmpty() )
			continue;

		const SBYTE sbRemissionType = m_vectorbtnRemissions[iRow].GetRemissionType();
		if(sbRemissionType == REMISSION_NONE)
			continue;

		switch(sbRemissionType)
		{
		case REMISSION_HP:
			m_strShortDesc.PrintF( _S( 616, "육체의 면죄부" ) );		
			pDrawPort->PutTextExCX( m_strShortDesc, m_nPosX + REMISSION_NAME_CX, m_nPosY + nY,	0xFFC672FF);
			break;
		case REMISSION_MP:
			m_strShortDesc.PrintF( _S( 617, "영혼의 면죄부" ) );		
			pDrawPort->PutTextExCX( m_strShortDesc, m_nPosX + REMISSION_NAME_CX, m_nPosY + nY,	0xFFC672FF);
			break;
		case REMISSION_ITEM:
			{				
				m_strShortDesc.PrintF( _S( 618, "장비의 면죄부" ) );		
				pDrawPort->PutTextExCX( m_strShortDesc, m_nPosX + REMISSION_NAME_CX, m_nPosY + nY,	0xFFC672FF);

				m_strShortDesc.PrintF( "%s", _pNetwork->GetItemName( m_vectorbtnRemissions[iRow].GetItemIndex() ));
				pDrawPort->PutTextExCX( m_strShortDesc, m_nPosX + REMISSION_NAME_CX, m_nPosY + nY + 17, 0xBDA99FFF);
			}
			break;
		}
	}
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIRemission::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;
	
	// Title bar
	static BOOL bTitleBarClick = FALSE;
	
	// Extended button clicked
	static BOOL	bLButtonDownInBtn = FALSE;
	
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
			
			// Move remission
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
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
			// Buy button
			else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// List box of remission desc
			else if( m_lbRemissionDesc.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			
			// Remission remission tab
			else
			{
				// Reset state of selected button
				if( bLButtonDownInBtn && m_nSelRemissionID >= 0 && ( pMsg->wParam & MK_LBUTTON ) )
				{
					m_vectorbtnRemissions[m_nSelRemissionID].SetBtnState( UBES_IDLE );
					bLButtonDownInBtn = FALSE;
				}
				// Remission icon scroll bar
				else if( m_sbRemissionIcon.MouseMessage( pMsg ) != WMSG_FAIL )
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
				
				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				// Buy button
				else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel button
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// List box of remission desc
				else if( m_lbRemissionDesc.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Remission remission tab
				else
				{
					// Remission icon scroll bar
					if( m_sbRemissionIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Remission slot
					else if( IsInsideRect( nX, nY, m_rcIcons ) )
					{
						int	nOldSelRemissionID = m_nSelRemissionID;
						m_nSelRemissionID = -1;
						
						int	iRowS = m_sbRemissionIcon.GetScrollPos();
						int	iRowE = iRowS + REMISSION_SLOT_ROW;

						if ( m_vectorbtnRemissions.size() < iRowE )
						{
							iRowE = m_vectorbtnRemissions.size();
						}

						for( int iRow = iRowS; iRow < iRowE; iRow++ )
						{
							if( m_vectorbtnRemissions[iRow].MouseMessage( pMsg ) != WMSG_FAIL )
							{
								// Update selected remission
								m_nSelRemissionID = iRow;
								if( nOldSelRemissionID != m_nSelRemissionID )
								{
									GetRemissionDesc( m_vectorbtnRemissions[iRow].GetRemissionType(), 
										m_vectorbtnRemissions[iRow].GetItemIndex(), 
										m_vectorbtnRemissions[iRow].GetInvenIndex());
										//m_vectorbtnRemissions[iRow].GetItemWearType());
								}
								
								bLButtonDownInBtn = TRUE;
								
								pUIManager->RearrangeOrder( UI_REMISSION, TRUE );
								return WMSG_SUCCESS;
							}
						}						
						GetRemissionDesc( REMISSION_NONE );
					}
				}
				
				pUIManager->RearrangeOrder( UI_REMISSION, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;
		
	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			bLButtonDownInBtn = FALSE;

			// If holding button doesn't exist
			if( pUIManager->GetHoldBtn().IsEmpty() )
			{
				// Title bar
				bTitleBarClick = FALSE;
				
				// If remission isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;
				
				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseRemission();
					
					return WMSG_SUCCESS;
				}
				// Buy button
				else if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						PressOK();
					
					return WMSG_SUCCESS;
				}
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseRemission();
					
					return WMSG_SUCCESS;
				}
				// List box of remission desc
				else if( m_lbRemissionDesc.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				
				// Remission remission tab
				else
				{
					// Remission icon scroll bar
					if( m_sbRemissionIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Remission slot
					else if( IsInsideRect( nX, nY, m_rcIcons ) )
					{
						int	iRowS = m_sbRemissionIcon.GetScrollPos();
						int	iRowE = iRowS + REMISSION_SLOT_ROW;
						if ( m_vectorbtnRemissions.size() < iRowE )
						{
							iRowE = m_vectorbtnRemissions.size();
						}
						for( int iRow = iRowS; iRow < iRowE; iRow++ )
						{
							if( m_vectorbtnRemissions[iRow].MouseMessage( pMsg ) != WMSG_FAIL )
								return WMSG_SUCCESS;
						}
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
				// List box of remission desc
				if( m_lbRemissionDesc.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Remission remission tab
				else
				{
					// Remission icon scroll bar
					if( m_sbRemissionIcon.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nothing
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
				// List box of remission desc
				if( m_lbRemissionDesc.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// Remission remission tab
				else
				{
					// Remission icon scroll bar
					if( m_sbRemissionIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
			}
		}
		break;
	}
	
	return WMSG_FAIL;
}

// ========================================================================= //
//                             Command functions                             //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIRemission::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_REMISSION_NOTIFY:
		break;
	case MSGCMD_REMISSION_BUY:
		if(bOK)
		{
			SendRemission();
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : MsgBoxLCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIRemission::MsgBoxLCommand( int nCommandCode, int nResult )
{
	switch( nCommandCode )
	{
	case MSGLCMD_REMISSION_REQ:
		if( nResult == REMISSION_OK )
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			if(InitRemission())
			{
				pUIManager->GetInventory()->Lock( TRUE, FALSE, LOCK_REMISSION );
				pUIManager->RearrangeOrder( UI_REMISSION, TRUE );
			}
			else
			{
				// Close message box of remission
				pUIManager->CloseMessageBox( MSGCMD_REMISSION_NOTIFY );
				pUIManager->CloseMessageBox( MSGCMD_REMISSION_BUY );
				
				// Create message box of remission
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 601, "면죄부" ), UMBS_OK,			
					UI_REMISSION, MSGCMD_REMISSION_NOTIFY );
				MsgBoxInfo.AddString( _S( 619, "면죄받을 만한 내용이 없습니다." ) );	
				pUIManager->CreateMessageBox( MsgBoxInfo );
				pUIManager->SetCSFlagOff( CSF_REMISSION );
			}
		}
		else if( nResult == REMISSION_TALK )
		{
			//TODO : NewQuestSystem
			// 퀘스트 창 띄우기.
			CUIQuestBook::TalkWithNPC();
		}
		else if( nResult == REMISSION_EVENT )
		{
			//TODO : NewQuestSystem
			// 퀘스트 창 띄우기.
			//CUIQuestBook::TalkWithNPC();
		}
		
		// [090527: selo] 확장팩 퀘스트 수정
		else if( ciQuestClassifier < nResult )	
		{
			// 선택한 퀘스트에 대해 수락 또는 보상 창을 연다.
			CUIQuestBook::SelectQuestFromMessageBox( nResult );
		}

		else
		{
			// Character state flags
			CUIManager::getSingleton()->SetCSFlagOff( CSF_REMISSION );
		}
		break;		
	}
}

// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendRemission()
// Desc :
// ----------------------------------------------------------------------------
void CUIRemission::SendRemission()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box of remission learn
	pUIManager->CloseMessageBox( MSGCMD_REMISSION_NOTIFY );
	pUIManager->CloseMessageBox( MSGCMD_REMISSION_BUY );
	
	if( m_nSelRemissionID < 0 )
		return;
	
	if( m_vectorbtnRemissions[m_nSelRemissionID].IsEmpty() )
		return;
	
	SBYTE sbRemissionType = m_vectorbtnRemissions[m_nSelRemissionID].GetRemissionType();
	switch(sbRemissionType)
	{
	case REMISSION_HP:
		_pNetwork->SendRecoverHPMP(_pNetwork->MyCharacterInfo.hpcount, 0);
		break;
	case REMISSION_MP:
		_pNetwork->SendRecoverHPMP(0, _pNetwork->MyCharacterInfo.mpcount);
		break;
	case REMISSION_ITEM:
		{
			const SWORD nTab = m_vectorbtnRemissions[m_nSelRemissionID].GetItemTab();
			const SWORD nIdx = m_vectorbtnRemissions[m_nSelRemissionID].GetInvenIndex();

			const LONG lUniItemIndex = m_vectorbtnRemissions[m_nSelRemissionID].GetItemUniIndex();			
			_pNetwork->SendRecoverItemSeal(nTab, nIdx, lUniItemIndex);
		}
		break;
	}	
}

// ========================================================================= //
//                         Receive message functions                         //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : PressOK()
// Desc :
// ----------------------------------------------------------------------------
void CUIRemission::PressOK()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box of remission
	pUIManager->CloseMessageBox( MSGCMD_REMISSION_BUY );
	
	// Create message box of remission
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;	
	MsgBoxInfo.SetMsgBoxInfo( _S( 601, "면죄부" ), UMBS_OKCANCEL, UI_REMISSION, MSGCMD_REMISSION_BUY);		
	strMessage.PrintF( _S( 620, "정말 이 면죄부를 구입하시겠습니까?" ) );	
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : RemissionError()
// Desc :
// ----------------------------------------------------------------------------
void CUIRemission::RemissionError( UBYTE ubError )
{
	CTString	strMessage;
	
	switch(ubError)
	{
	case 0:
		strMessage = _S( 306, "나스가 부족합니다." );			
		break;
	case 1:
		strMessage = _S( 619, "면죄받을 만한 내용이 없습니다." );			
		break;
	case 2:
		strMessage = _S( 619, "면죄받을 만한 내용이 없습니다." );			
		break;
	case 3:
		strMessage = _S( 619, "면죄받을 만한 내용이 없습니다." );		
		break;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box of remission
	pUIManager->CloseMessageBox( MSGCMD_REMISSION_NOTIFY );
	pUIManager->CloseMessageBox( MSGCMD_REMISSION_BUY );
	
	// Create message box of remission
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 601, "면죄부" ), UMBS_OK,			
								UI_REMISSION, MSGCMD_REMISSION_NOTIFY );
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
	pUIManager->SetCSFlagOff( CSF_REMISSION );

	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_REMISSION );
}

// ----------------------------------------------------------------------------
// Name : CalculatePrice()
// Desc :
// ----------------------------------------------------------------------------
SQUAD CUIRemission::CalculatePrice(int iRemissionType, SWORD nIdx)
{	
	SQUAD iPrice = 0;
	switch(iRemissionType)
	{
	case REMISSION_HP:		// HP 면죄의 경우
		iPrice = _pNetwork->MyCharacterInfo.hpcount * _pNetwork->MyCharacterInfo.level * 100;
		break;
	case REMISSION_MP:		// MP 면죄의 경우
		iPrice = _pNetwork->MyCharacterInfo.mpcount * _pNetwork->MyCharacterInfo.level * 100;
		break;
	case REMISSION_ITEM:	// 아이템 면죄의 경우
		{
			int iWearPos = _pNetwork->MySlotItem[0][nIdx].Item_Wearing;
			float fWeight = 1.0f;
			if(iWearPos == WEAR_JACKET ||
				iWearPos == WEAR_PANTS)
			{
				fWeight = 1.5f;
			}
			else if(iWearPos == WEAR_WEAPON ||
				iWearPos == WEAR_SHIELD)
			{
				fWeight = 2.0f;
			}

			if( iWearPos != -1 )
			{
				const int iLevel = _pNetwork->MySlotItem[0][nIdx].ItemData->GetLevel();
				iPrice = 100 * iLevel * iLevel * fWeight;
			}

			/*
			if(_pNetwork->pMyWearItem[iWearPos])
			{
				const int iLevel = _pNetwork->pMyWearItem[iWearPos]->ItemData.GetLevel();
			iPrice = 100 * iLevel * iLevel * fWeight;
			}
			*/
			//const int iLevel = _pNetwork->MyCurrentWearing[iWearPos].ItemData->GetLevel();
		}
		break;
	}

	return iPrice;
}