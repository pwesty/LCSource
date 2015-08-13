#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <map>
#include <vector>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/Ska/Render.h>
#include <Engine/Entities/InternalClasses.h>
#include <iostream>
#include <Engine/Object/ActorMgr.h>
#include <Engine/Info/MyInfo.h>

#define	SLEARN_TAB_WIDTH				96
#define	SLEARN_COMMAND_TAB_CX			60
#define	SLEARN_SKILL_TAB_CX				156

#define	SLEARN_TAB_SY					34

#define PETINFO_SLOT_INFO_CX			122

enum eSelection
{
	PET_TRAINING,
	PET_CHANGE,
	PET_TALK,
};

enum eChangeSelection
{
	CHANGE_RIDE,						// 탈것으로...
};

// Date : 2005-03-07,   By Lee Ki-hwan
static int	_iMaxMsgStringChar = 0;

static int m_nSelSkillIndex = -1;

// Global Variable.
// UI renewal [9/16/2009 rumist]
extern INDEX g_iXPosInPetInfo;
extern INDEX g_iYPosInPetInfo;

// ----------------------------------------------------------------------------
// Name : CUIPetInfo()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIPetInfo::CUIPetInfo()
{
	m_bShowSkillInfo	= FALSE;
	m_nCurrentTab		= PETINFO_TAB_COMMAND;
}

// ----------------------------------------------------------------------------
// Name : ~CUIPetInfo()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIPetInfo::~CUIPetInfo()
{
	Destroy();

	SPetSkill_map::iterator	iter = m_mapPetSkill.begin();
	SPetSkill_map::iterator	eiter = m_mapPetSkill.end();

	for (; iter != eiter; ++iter)
	{
		{
			UIButton_map::iterator citer = iter->second.m_btnmapCommands.begin();
			UIButton_map::iterator ceiter = iter->second.m_btnmapCommands.end();

			for (; citer != ceiter; ++citer)
			{
				SAFE_DELETE(citer->second);
			}

			iter->second.m_btnmapCommands.clear();
		}

		{
			UIButton_map::iterator citer = iter->second.m_btnmapSkills.begin();
			UIButton_map::iterator ceiter = iter->second.m_btnmapSkills.end();

			for (; citer != ceiter; ++citer)
			{
				SAFE_DELETE(citer->second);
			}

			iter->second.m_btnmapSkills.clear();
		}
	}
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetInfo::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	// Set Pet Name
	PetInfo().SetName(HORSE_CHILD,_S(2423,"포니"));
	PetInfo().SetName(HORSE_BOY,_S(2424,"호스"));
	PetInfo().SetName(HORSE_ADULT,_S(2425,"나이트메어"));
	PetInfo().SetName(DRAGON_CHILD,_S(2426,"해츨링"));
	PetInfo().SetName(DRAGON_BOY,_S(2427,"드레이크"));
	PetInfo().SetName(DRAGON_ADULT,_S(2428,"드래곤"));
	
	// 레어펫 이름
	PetInfo().SetName(BLUE_HORSE_CHILD,_S(2423,"포니"));
	PetInfo().SetName(BLUE_HORSE_BOY,_S(2424,"호스"));
	PetInfo().SetName(BLUE_HORSE_ADULT,_S(2425,"나이트메어"));
	PetInfo().SetName(PINK_DRAGON_CHILD,_S(2426,"해츨링"));
	PetInfo().SetName(PINK_DRAGON_BOY,_S(2427,"드레이크"));
	PetInfo().SetName(PINK_DRAGON_ADULT,_S(2428,"드래곤"));

	PetInfo().SetName(MISTERY_HORSE_CHILD,_S(2423,"포니"));
	PetInfo().SetName(MISTERY_HORSE_BOY,_S(2424,"호스"));
	PetInfo().SetName(MISTERY_HORSE_ADULT,_S(2425,"나이트메어"));
	PetInfo().SetName(MISTERY_DRAGON_CHILD,_S(2426,"해츨링"));
	PetInfo().SetName(MISTERY_DRAGON_BOY,_S(2427,"드레이크"));
	PetInfo().SetName(MISTERY_DRAGON_ADULT,_S(2428,"드래곤"));

	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	_iMaxMsgStringChar = SLEARN_DESC_CHAR_WIDTH / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );
	m_rcIcons.SetRect( 17, 55, 51, 200 );
	m_rcTab.SetRect( 12, 31, 204, 50 );

	// Create skill learn texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\SkillLearn.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	m_rtBackgroundTop.SetUV( 0, 0, 216, 340, fTexWidth, fTexHeight );
	m_rtBackgroundBottom.SetUV( 0, 358, 216, 370, fTexWidth, fTexHeight );
	m_rtBackgroundBottom2.SetUV( 0, 393, 216, 400, fTexWidth, fTexHeight );


	m_rtSelOutline.SetUV( 218, 80, 250, 112, fTexWidth, fTexHeight );
	m_rtTabLine.SetUV( 12, 31, 13, 50, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 219, 15, 233, 29, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );	

	// Cancel button
	m_btnOK.Create( this, _S(191,"확인" ), 141, 345, 63, 21 );
	m_btnOK.SetUV( UBS_IDLE, 0, 403, 63, 424, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 66, 403, 129, 424, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Scroll bar of active skill icons
	m_sbCommandIcon.Create( this, 193, 51, 9, 153 );
	m_sbCommandIcon.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbCommandIcon.SetScrollPos( 0 );
	m_sbCommandIcon.SetScrollRange( SLEARN_SLOT_ROW_TOTAL );
	m_sbCommandIcon.SetCurItemCount( 0 );
	m_sbCommandIcon.SetItemsPerPage( SLEARN_SLOT_ROW );
	// Up button
	m_sbCommandIcon.SetUpUV( UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight );
	m_sbCommandIcon.SetUpUV( UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight );
	m_sbCommandIcon.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbCommandIcon.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbCommandIcon.SetBarTopUV( 219, 32, 228, 42, fTexWidth, fTexHeight );
	m_sbCommandIcon.SetBarMiddleUV( 219, 43, 228, 45, fTexWidth, fTexHeight );
	m_sbCommandIcon.SetBarBottomUV( 219, 46, 228, 56, fTexWidth, fTexHeight );
	// Down button
	m_sbCommandIcon.SetDownUV( UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight );
	m_sbCommandIcon.SetDownUV( UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight );
	m_sbCommandIcon.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbCommandIcon.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Wheel region
	m_sbCommandIcon.SetWheelRect( -181, -1, 180, 154 );

	// Scroll bar of passive skill icons
	m_sbSkillIcon.Create( this, 193, 51, 9, 153 );
	m_sbSkillIcon.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbSkillIcon.SetScrollPos( 0 );
	m_sbSkillIcon.SetScrollRange( SLEARN_SLOT_ROW_TOTAL );
	m_sbSkillIcon.SetCurItemCount( 0 );
	m_sbSkillIcon.SetItemsPerPage( SLEARN_SLOT_ROW );
	// Up button
	m_sbSkillIcon.SetUpUV( UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight );
	m_sbSkillIcon.SetUpUV( UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight );
	m_sbSkillIcon.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbSkillIcon.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbSkillIcon.SetBarTopUV( 219, 32, 228, 42, fTexWidth, fTexHeight );
	m_sbSkillIcon.SetBarMiddleUV( 219, 43, 228, 45, fTexWidth, fTexHeight );
	m_sbSkillIcon.SetBarBottomUV( 219, 46, 228, 56, fTexWidth, fTexHeight );
	// Down button
	m_sbSkillIcon.SetDownUV( UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight );
	m_sbSkillIcon.SetDownUV( UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight );
	m_sbSkillIcon.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbSkillIcon.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Wheel region
	m_sbSkillIcon.SetWheelRect( -181, -1, 180, 154 );

	// List box of skill description
	m_lbPetDesc.Create( this, 13, 207, 180, 124, _pUIFontTexMgr->GetLineHeight(), 7, 6, 2, FALSE );
	m_lbPetDesc.CreateScroll( TRUE, 0, 0, 9, 124, 9, 7, 0, 0, 10 );
	m_lbPetDesc.SetColumnPosX( 1, 168, TEXT_RIGHT );
	// Up button
	m_lbPetDesc.SetScrollUpUV( UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight );
	m_lbPetDesc.SetScrollUpUV( UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight );
	m_lbPetDesc.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbPetDesc.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbPetDesc.SetScrollBarTopUV( 219, 32, 228, 42, fTexWidth, fTexHeight );
	m_lbPetDesc.SetScrollBarMiddleUV( 219, 43, 228, 45, fTexWidth, fTexHeight );
	m_lbPetDesc.SetScrollBarBottomUV( 219, 46, 228, 56, fTexWidth, fTexHeight );
	// Down button
	m_lbPetDesc.SetScrollDownUV( UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight );
	m_lbPetDesc.SetScrollDownUV( UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight );
	m_lbPetDesc.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbPetDesc.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetInfo::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void CUIPetInfo::ResetSavePosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );

	g_iXPosInPetInfo = GetPosX();
	g_iYPosInPetInfo = GetPosY();
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetInfo::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : InitPetInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetInfo::InitPetInfo( )
{
	// Reset description
	m_lbPetDesc.ResetAllStrings();
	LONG	nPetIndex = MY_PET_INFO()->lIndex;

//	m_mapPetSkill[nPetIndex].m_btnmapSkills.sort();	// 자동으로 정렬 될껄 .... 
//	m_mapPetSkill[nPetIndex].m_btnmapCommands.sort();

	// Set Command scroll bar
	m_sbCommandIcon.SetScrollPos( 0 );
	m_sbCommandIcon.SetCurItemCount( m_mapPetSkill[nPetIndex].m_btnmapCommands.size() );
	
	// Set passive scroll bar
	m_sbSkillIcon.SetScrollPos( 0 );
	m_sbSkillIcon.SetCurItemCount( m_mapPetSkill[nPetIndex].m_btnmapCommands.size() );
}

// ----------------------------------------------------------------------------
// Name : OpenPetInfo()
// Desc : nMasterType은 일반 스킬일때는 직업, 특수 스킬일때는 특수 스킬타입이 됩니다.
// ----------------------------------------------------------------------------
void CUIPetInfo::OpenPetInfo( )
{
	// If this is already exist
	if( IsVisible() )
		return;	

	GetPetDesc();
	CUIManager::getSingleton()->RearrangeOrder( UI_PETINFO, TRUE );

	m_nSelCommandID		= -1;
	m_nSelSkillID	= -1;
}

// ----------------------------------------------------------------------------
// Name : ClosePetInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetInfo::ClosePetInfo()
{
	CUIManager::getSingleton()->RearrangeOrder( UI_PETINFO, FALSE );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetInfo::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set skill learn texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	// Background
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + 340,
										m_rtBackgroundTop.U0, m_rtBackgroundTop.V0, m_rtBackgroundTop.U1, m_rtBackgroundTop.V1,
										0xFFFFFFFF );

	pDrawPort->AddTexture( m_nPosX, m_nPosY + 340, m_nPosX + m_nWidth, m_nPosY + m_nHeight - 7,
										m_rtBackgroundBottom.U0, m_rtBackgroundBottom.V0, m_rtBackgroundBottom.U1, m_rtBackgroundBottom.V1,
										0xFFFFFFFF );

	pDrawPort->AddTexture( m_nPosX, m_nPosY + m_nHeight - 7, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
										m_rtBackgroundBottom2.U0, m_rtBackgroundBottom2.V0, m_rtBackgroundBottom2.U1, m_rtBackgroundBottom2.V1,
										0xFFFFFFFF );
										
	// Tab
	pDrawPort->AddTexture( m_nPosX + m_rcTab.Left + SLEARN_TAB_WIDTH, m_nPosY + m_rcTab.Top,
		m_nPosX + m_rcTab.Left + SLEARN_TAB_WIDTH + 1, m_nPosY + m_rcTab.Bottom,
		m_rtTabLine.U0, m_rtTabLine.V0, m_rtTabLine.U1, m_rtTabLine.V1,
		0xFFFFFFFF );	
	
	// Close button
	m_btnClose.Render();

	// Cancel button
	m_btnOK.Render();

	// Scroll bar of skill icon
	if( m_nCurrentTab == PETINFO_TAB_COMMAND )
		m_sbCommandIcon.Render();
	else if( m_nCurrentTab == PETINFO_TAB_SKILL )
		m_sbSkillIcon.Render();

	// List box of skill desc
	m_lbPetDesc.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Skill buttons
	RenderSkillBtns();
	
	// Text in skill learn
	pDrawPort->PutTextEx( _S(2173, "펫 정보창" ), m_nPosX + SLEARN_TITLE_TEXT_OFFSETX,	
										m_nPosY + SLEARN_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );
	
	pDrawPort->PutTextExCX( _S(2174, "커맨드" ), m_nPosX + SLEARN_COMMAND_TAB_CX,	
		m_nPosY + SLEARN_TAB_SY,
		m_nCurrentTab == PETINFO_TAB_COMMAND ? 0xFFCB00FF : 0x6B6B6BFF );
	pDrawPort->PutTextExCX( _S(91, "스킬" ), m_nPosX + SLEARN_SKILL_TAB_CX,	
		m_nPosY + SLEARN_TAB_SY,
		m_nCurrentTab == PETINFO_TAB_SKILL ? 0xFFCB00FF : 0x6B6B6BFF );	

	// Flush all render text queue
	pDrawPort->EndTextEx();
	//pDrawPort->FlushRenderingQueue();

	// Set skill learn texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	
	// Render all elements
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();
	
}

// ----------------------------------------------------------------------------
// Name : GetPetDesc()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetInfo::GetPetDesc( )
{
	m_lbPetDesc.ResetAllStrings();

	ObjInfo* pInfo = ObjInfo::getSingleton();
	CPetTargetInfom* pPetInfo = pInfo->GetMyPetInfo();

	if (pPetInfo == NULL)
		return;

	// Make description of skill
	CTString	strTemp;
	
	CNetworkLibrary::sPetInfo	TempPet;
	TempPet.lIndex				= pPetInfo->lIndex;
	std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
		std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
	if( iter != _pNetwork->m_vectorPetList.end() )
	{
		if( (*iter).strNameCard.Length()>0 )
		{
			m_lbPetDesc.AddString( 0, _S(3526, "명찰"), 0xc5c5c5ff );	
			m_lbPetDesc.AddString( 1, (*iter).strNameCard, 0xffe3a8ff );				
		}

		strTemp.PrintF( "%s", PetInfo().GetName(pPetInfo->iType, pPetInfo->iAge) );
		m_lbPetDesc.AddString( 0, strTemp );	

		strTemp.PrintF( _S(2176,"%d 레벨"), (*iter).lLevel );	
		m_lbPetDesc.AddString( 1, strTemp, 0xff6000ff );		
		m_lbPetDesc.AddString( 0, _S(2177,"생명력"), 0xc5c5c5ff );	

		strTemp.PrintF("%d/%d", (*iter).lHP, (*iter).lMaxHP);
		m_lbPetDesc.AddString( 1, strTemp, 0xffe3a8ff );	

		m_lbPetDesc.AddString( 0, _S(2178,"배고픔정도"), 0xc5c5c5ff );	
		strTemp.PrintF("%d/%d", (*iter).lHungry, (*iter).lMaxHungry);
		m_lbPetDesc.AddString( 1, strTemp, 0xffe3a8ff );			
		
		m_lbPetDesc.AddString( 0, _S(89,"경험치"), 0xc5c5c5ff );
		strTemp.PrintF("%I64d/%I64d", (*iter).llExp, (*iter).llNeedExp);
		m_lbPetDesc.AddString( 1, strTemp, 0xffe3a8ff );	

		m_lbPetDesc.AddString( 0, _S(2180,"교감도"), 0xc5c5c5ff );		
		strTemp.PrintF("%d/%d", (*iter).lSympathy, (*iter).lMaxSympathy);
		m_lbPetDesc.AddString( 1, strTemp, 0xffe3a8ff );	
		
// 		m_lbPetDesc.AddString( 0, CTString("실행중인 커맨드"), 0xc5c5c5ff );	
//		m_lbPetDesc.AddString( 1, CTString("닝기리"), 0xffe3a8ff );	
		
		m_lbPetDesc.AddString( 0, _S(2181,"기술 포인트"), 0xc5c5c5ff );

		strTemp.PrintF("%d", (*iter).lAbility);
		m_lbPetDesc.AddString( 1, strTemp, 0xffe3a8ff );	

#ifdef PET_SEAL_TIME
		if( (*iter).lRemainRebirth > 0 )
		{
			m_lbPetDesc.AddString( 0, _S( 2454, "봉인해제시간"), 0xc5c5c5ff );		
			strTemp.PrintF(_S( 2455, "%d시간 %d분"), (*iter).lRemainRebirth/3600, ((*iter).lRemainRebirth%3600)/60 );
			m_lbPetDesc.AddString( 1, strTemp, 0xffe3a8ff );	
		}
#endif
	}
}

// ----------------------------------------------------------------------------
// Name : RenderSkillBtns()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetInfo::RenderSkillBtns()
{
	int	nX = SLEARN_SLOT_SX, nY = SLEARN_SLOT_SY;
	int	iRowE;
	int nCount = 0;
	LONG	nPetIndex = MY_PET_INFO()->lIndex;
	int nSelectID = -1;
	UIButton_map::iterator	iter;
	UIButton_map::iterator	iterSelect;
	UIButton_map::iterator	iterBegin;
	UIButton_map::iterator	iterEnd;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	LONG nCommandIconPos = m_sbCommandIcon.GetScrollPos();
	LONG nSkillIconPos = m_sbSkillIcon.GetScrollPos();
	int nIconPos = 0;

// Button Render 
	// Command tab
	if( m_nCurrentTab == PETINFO_TAB_COMMAND )
	{
		iterBegin	= m_mapPetSkill[nPetIndex].m_btnmapCommands.begin();
		iterEnd		= m_mapPetSkill[nPetIndex].m_btnmapCommands.end();
		nIconPos	= nCommandIconPos;
		iterSelect	= m_mapPetSkill[nPetIndex].m_btnmapCommands.find( m_nSelSkillIndex );
			
		if( iterSelect != iterEnd )
		{
			nSelectID = std::distance( iterBegin, iterSelect );
		}
	}
	// Skill tab
	else if( m_nCurrentTab == PETINFO_TAB_SKILL )
	{
		iterBegin	= m_mapPetSkill[nPetIndex].m_btnmapSkills.begin();
		iterEnd		= m_mapPetSkill[nPetIndex].m_btnmapSkills.end();
		int size = m_mapPetSkill[nPetIndex].m_btnmapSkills.size();
		nIconPos	= nSkillIconPos;	
		
		iterSelect	= m_mapPetSkill[nPetIndex].m_btnmapSkills.find( m_nSelSkillIndex );
			
		if( iterSelect != iterEnd )
		{
			nSelectID = std::distance( iterBegin, iterSelect  );
		}
	}
	
	iter = iterBegin;
	
	for( nCount = 0; nCount < nIconPos; iter++, nCount++ );
			
	// Render 
	for( nCount = 0; ( iter != iterEnd ) && ( nCount < SLEARN_SLOT_ROW ); iter++, nCount++, nY += SLEARN_SLOT_OFFSETY )
	{      
		iter->second->SetPos( nX, nY );

		if( iter->second->IsEmpty() )		
			continue;
		
		iter->second->Render(pDrawPort);
	}

	//Render Select OutLine
	iRowE = nIconPos + SLEARN_SLOT_ROW;
			
	if( nSelectID >= 0 && nIconPos <= nSelectID && nSelectID < iRowE )
	{
		// Set skill learn texture
		pDrawPort->InitTextureData( m_ptdBaseTexture );

		iterSelect->second->GetAbsPos( nX, nY );

		pDrawPort->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
											m_rtSelOutline.U0, m_rtSelOutline.V0,
											m_rtSelOutline.U1, m_rtSelOutline.V1,
											0xFFFFFFFF );
		// Render all elements
		pDrawPort->FlushRenderingQueue();
	}
	
// Render Skill Info
	nY = SLEARN_NAME_SY;
	int	nCharLevel	= _pNetwork->MyCharacterInfo.level;
	int nCharSP		= _pNetwork->MyCharacterInfo.sp;

	iter = iterBegin;
	for( nCount = 0; nCount < nIconPos; iter++, nCount++ );
	
	for( nCount = 0; ( iter != iterEnd ) && ( nCount < SLEARN_SLOT_ROW ); iter++, nCount++, nY += SLEARN_SLOT_OFFSETY )
	{   
		if( iter->second->IsEmpty() ) continue;

		LONG nSkillIndex = iter->second->getIndex();
		CSkill	&rSkill = _pNetwork->GetSkillData( nSkillIndex );
		SBYTE	sbLevel = MY_INFO()->GetPetSkillLevel(nPetIndex, nSkillIndex);

		GetSkillDesc( nSkillIndex, sbLevel );

		pDrawPort->PutTextExCX( m_strShortDesc, m_nPosX + PETINFO_SLOT_INFO_CX, m_nPosY + nY, 0xC5C5C5FF );
		pDrawPort->PutTextExCX( m_strShortDesc2, m_nPosX + PETINFO_SLOT_INFO_CX, m_nPosY + nY + 15, 0xC5C5C5FF );
	
	}
	
	m_sbCommandIcon.SetCurItemCount( m_mapPetSkill[nPetIndex].m_btnmapCommands.size() );
	m_sbSkillIcon.SetCurItemCount( m_mapPetSkill[nPetIndex].m_btnmapSkills.size() );
}

//------------------------------------------------------------------------------
// CUIPetInfo::GetSkillDesc
// Explain:  
// Date : 2005-09-27,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIPetInfo::GetSkillDesc( int nSkillIndex, int nSkillLevel )
{
	CSkill	&rSelSkill = _pNetwork->GetSkillData( nSkillIndex );

	switch( rSelSkill.GetType() )
	{
	case CSkill::ST_PET_COMMAND:			// Command
		{
			m_strShortDesc.PrintF( "%s ", rSelSkill.GetName() );
#if defined(G_RUSSIA)
			m_strShortDesc2.PrintF( "%s %d", _S( 4414, "LV" ), nSkillLevel);
#else
			m_strShortDesc2.PrintF( "Lv %d", nSkillLevel );			
#endif
		}
		break;
	case CSkill::ST_PET_SKILL_ACTIVE:		// Skill
		{
			m_strShortDesc.PrintF( "%s ", rSelSkill.GetName() );
			int	nNeedMP = rSelSkill.GetNeedMP( nSkillLevel - 1 );
			int	nNeedHP = rSelSkill.GetNeedHP( nSkillLevel - 1 );
			if( nNeedHP == 0 )
			{
				if( nNeedMP == 0 )
				{
#if defined(G_RUSSIA)
					m_strShortDesc2.PrintF( "%s %d", _S( 4414, "LV" ), nSkillLevel);
#else
					m_strShortDesc2.PrintF( "Lv %d", nSkillLevel );
#endif
				}
				else
				{
#if defined(G_RUSSIA)
					m_strShortDesc2.PrintF( "%s %d %s %d", _S( 4414, "LV" ), nSkillLevel, _S( 4412, "MP" ), nNeedMP);						
#else
					m_strShortDesc2.PrintF( "Lv %d MP %d", nSkillLevel, nNeedMP );
#endif
				}
			}
			else
			{
				if( nNeedMP == 0 )
				{
#if defined(G_RUSSIA)
					m_strShortDesc2.PrintF( "%s %d %s %d", _S( 4414, "LV" ), nSkillLevel, _S( 4412, "MP" ) ,nNeedMP);
#else
					m_strShortDesc2.PrintF( "Lv %d HP %d", nSkillLevel, nNeedHP );
#endif
				}
				else
				{
#if defined(G_RUSSIA)
					m_strShortDesc2.PrintF( "%s %d %s %d %s %d",_S( 4414, "LV" ), nSkillLevel,_S( 4412, "MP" ), nNeedMP,_S( 4411, "HP" ), nNeedHP );
#else
					m_strShortDesc2.PrintF( "Lv %d MP %d HP %d", nSkillLevel, nNeedMP, nNeedHP );
#endif
				}
			}
		}
		break;
	case CSkill::ST_PET_SKILL_PASSIVE:		
		{
			m_strShortDesc.PrintF( "%s ", rSelSkill.GetName() );
#if defined(G_RUSSIA)
			m_strShortDesc2.PrintF( "%s %d", _S( 4414, "LV" ), nSkillLevel);
#else
			m_strShortDesc2.PrintF( "Lv %d", nSkillLevel );
#endif
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIPetInfo::MouseMessage( MSG *pMsg )
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

	ObjInfo* pInfo = ObjInfo::getSingleton();
	CPetTargetInfom* pPetInfo = pInfo->GetMyPetInfo();

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) == FALSE)
				return WMSG_FAIL;
				
			CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Move skill learn
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
			// Cancel button
			else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// List box of skill desc
			else if( m_lbPetDesc.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Active skill tab
			else if( m_nCurrentTab == PETINFO_TAB_COMMAND )
			{
				// Reset state of selected button
				if( bLButtonDownInBtn && m_nSelSkillIndex >= 0 && ( pMsg->wParam & MK_LBUTTON ) )
				{
					UIButton_map::iterator iter = m_mapPetSkill[pPetInfo->lIndex].m_btnmapCommands.find(m_nSelSkillIndex);
					UIButton_map::iterator iterBegin = m_mapPetSkill[pPetInfo->lIndex].m_btnmapCommands.begin();
					UIButton_map::iterator iterEnd = m_mapPetSkill[pPetInfo->lIndex].m_btnmapCommands.end();
					
					if( iter != iterEnd )
					{
						CUIManager* pUIManager = CUIManager::getSingleton();

						if (pUIManager->GetDragIcon() == NULL && (ndX != 0 || ndY != 0))
						{
							pUIManager->SetHoldBtn(iter->second);
						}

						bLButtonDownInBtn = FALSE;
					}
				}
				else if( m_sbCommandIcon.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else //if( IsInsideRect( nX, nY, m_rcIcons ) )
				{
					UIButton_map::iterator iter;
					UIButton_map::iterator iterBegin = m_mapPetSkill[pPetInfo->lIndex].m_btnmapCommands.begin();
					UIButton_map::iterator iterEnd = m_mapPetSkill[pPetInfo->lIndex].m_btnmapCommands.end();

					iter = iterBegin;
					int nIconPos = m_sbCommandIcon.GetScrollPos();
					int nCount;
					for( nCount = 0; nCount < nIconPos; iter++, nCount++ );
						
					bool bShowToolTip = false;
					// Render 
					for( nCount = 0; ( iter != iterEnd ) && ( nCount < SLEARN_SLOT_ROW ); iter++, nCount++ )
					{
						if( iter->second->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							return WMSG_SUCCESS;
						}
					}

					return WMSG_SUCCESS;
				}
			}
			// Passive skill tab
			else if( m_nCurrentTab == PETINFO_TAB_SKILL )
			{
				// Reset state of selected button
				if( bLButtonDownInBtn && m_nSelSkillIndex >= 0 && ( pMsg->wParam & MK_LBUTTON ) )
				{
					UIButton_map::iterator iter = m_mapPetSkill[pPetInfo->lIndex].m_btnmapSkills.find(m_nSelSkillIndex);
					UIButton_map::iterator iterBegin = m_mapPetSkill[pPetInfo->lIndex].m_btnmapSkills.begin();
					UIButton_map::iterator iterEnd = m_mapPetSkill[pPetInfo->lIndex].m_btnmapSkills.end();

					CUIManager* pUIManager = CUIManager::getSingleton();

					if (pUIManager->GetDragIcon() == NULL && ( ndX != 0 || ndY != 0 ))
					{
						if( _pNetwork->GetSkillData( iter->first ).GetType() == CSkill::ST_PET_SKILL_ACTIVE)
						{													
							pUIManager->SetHoldBtn(iter->second);
						}
					}

					bLButtonDownInBtn = FALSE;
				}
				// Passive icon scroll bar
				else if( m_sbSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else //if( IsInsideRect( nX, nY, m_rcIcons ) )
				{
					UIButton_map::iterator iter;
					UIButton_map::iterator iterBegin = m_mapPetSkill[pPetInfo->lIndex].m_btnmapSkills.begin();
					UIButton_map::iterator iterEnd = m_mapPetSkill[pPetInfo->lIndex].m_btnmapSkills.end();

					iter = iterBegin;
					int nIconPos = m_sbSkillIcon.GetScrollPos();
					int nCount;
					for( nCount = 0; nCount < nIconPos; iter++, nCount++ );
						
					bool bShowToolTip = false;
					// Render 
					for( nCount = 0; ( iter != iterEnd ) && ( nCount < SLEARN_SLOT_ROW ); iter++, nCount++ )
					{
						if( iter->second->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							return WMSG_SUCCESS;
						}
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
				// Skill tab
				else if( IsInsideRect( nX, nY, m_rcTab ) )
				{
					int	nOldTab = m_nCurrentTab;
					m_nCurrentTab = ( nX - m_nPosX - m_rcTab.Left ) / SLEARN_TAB_WIDTH;
					if( m_nCurrentTab < 0 || m_nCurrentTab > PETINFO_TAB_SKILL )
						m_nCurrentTab = nOldTab;

					if( nOldTab != m_nCurrentTab )
					{
						if( m_nSelSkillIndex < 0 )
						{
							GetPetDesc();
						}
						else
						{
							UIButton_map::iterator iter;
							
							if( m_nCurrentTab == PETINFO_TAB_COMMAND )
							{
								if( (iter = m_mapPetSkill[pPetInfo->lIndex].m_btnmapCommands.find( m_nSelSkillIndex ) )
									== m_mapPetSkill[pPetInfo->lIndex].m_btnmapCommands.end() )	break;								
							}
							else if( m_nCurrentTab == PETINFO_TAB_SKILL )
							{
								if( (iter = m_mapPetSkill[pPetInfo->lIndex].m_btnmapSkills.find( m_nSelSkillIndex ) )
									== m_mapPetSkill[pPetInfo->lIndex].m_btnmapSkills.end() )	break;																
							}
						
						}
						return WMSG_SUCCESS;
					}
					else
					{
					
					}
				}
				// Cancel button
				else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// List box of skill desc
				else if( m_lbPetDesc.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Active skill tab
				else if( m_nCurrentTab == PETINFO_TAB_COMMAND )
				{
					// Active icon scroll bar
					if( m_sbCommandIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Skill slot
					else if( IsInsideRect( nX, nY, m_rcIcons ) )
					{
						int	nOldSelSkillID = m_nSelSkillIndex;
						m_nSelSkillIndex = -1;
        
						int	nIconPos = m_sbCommandIcon.GetScrollPos();

						UIButton_map::iterator iter = m_mapPetSkill[pPetInfo->lIndex].m_btnmapCommands.begin();
						UIButton_map::iterator iterEnd = m_mapPetSkill[pPetInfo->lIndex].m_btnmapCommands.end();
										
						int nCount;
						for( nCount = 0; nCount < nIconPos; iter++, nCount++ );

						
						for( nCount = 0; ( iter != iterEnd ) && ( nCount < SLEARN_SLOT_ROW ); iter++, nCount++)
						{      							
							if ( iter->second->MouseMessage( pMsg ) != WMSG_FAIL )
							{
								m_nSelSkillIndex = iter->first;
								bLButtonDownInBtn = TRUE;
								pUIManager->RearrangeOrder( UI_PETINFO, TRUE );
								return WMSG_SUCCESS;
							}	
						}

					}
				}
				// Passive skill tab
				else if( m_nCurrentTab == PETINFO_TAB_SKILL )
				{
					// Passive icon scroll bar
					if( m_sbSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Skill slot
					else if( IsInsideRect( nX, nY, m_rcIcons ) )
					{
						int	nOldSelSkillID = m_nSelSkillIndex;
						m_nSelSkillIndex = -1;
        
						int	nIconPos = m_sbSkillIcon.GetScrollPos();

						UIButton_map::iterator iter = m_mapPetSkill[pPetInfo->lIndex].m_btnmapSkills.begin();
						UIButton_map::iterator iterEnd = m_mapPetSkill[pPetInfo->lIndex].m_btnmapSkills.end();
										
						int nCount;
						for( nCount = 0; nCount < nIconPos; iter++, nCount++ );

						// Render 
						for( nCount = 0; ( iter != iterEnd ) && ( nCount < SLEARN_SLOT_ROW ); iter++, nCount++)
						{      							
							if ( iter->second->MouseMessage( pMsg ) != WMSG_FAIL )
							{
								m_nSelSkillIndex = iter->first;
								bLButtonDownInBtn = TRUE;

								pUIManager->RearrangeOrder( UI_PETINFO, TRUE );
								return WMSG_SUCCESS;
							}	
						}					
					}
				}
				pUIManager->RearrangeOrder( UI_PETINFO, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			bLButtonDownInBtn = FALSE;

			CUIManager* pUIManager = CUIManager::getSingleton();

			// If holding button doesn't exist
			if (pUIManager->GetDragIcon() == NULL)
			{
				// Title bar
				bTitleBarClick = FALSE;

				// If skill learn isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						ClosePetInfo();

					return WMSG_SUCCESS;
				}				
				// Cancel button
				else if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						ClosePetInfo();

					return WMSG_SUCCESS;
				}
				// List box of skill desc
				else if( m_lbPetDesc.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// Active skill tab
				else if( m_nCurrentTab == PETINFO_TAB_COMMAND )
				{
					// Active icon scroll bar
					if( m_sbCommandIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Skill slot
					else if( IsInsideRect( nX, nY, m_rcIcons ) )
					{
						int	nIconPos = m_sbCommandIcon.GetScrollPos();

						UIButton_map::iterator iter = m_mapPetSkill[pPetInfo->lIndex].m_btnmapCommands.begin();
						UIButton_map::iterator iterEnd = m_mapPetSkill[pPetInfo->lIndex].m_btnmapCommands.end();
										
						int nCount;
						for( nCount = 0; nCount < nIconPos; iter++, nCount++ );

						// Render 
						for( nCount = 0; ( iter != iterEnd ) && ( nCount < SLEARN_SLOT_ROW ); iter++, nCount++)
						{      							
							if( (wmsgResult = iter->second->MouseMessage( pMsg )) != WMSG_FAIL )
							{
								if( wmsgResult == WMSG_COMMAND )
								{
									if (MY_INFO()->GetSkillDelay(iter->second->getIndex()) == false)
									{
										UseCommand( iter->second->getIndex() );
									}
								}
								return WMSG_SUCCESS;
							}	
						}				
					}
				}
				// Passive skill tab
				else if( m_nCurrentTab == PETINFO_TAB_SKILL )
				{
					// Passive icon scroll bar
					if( m_sbSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Skill slot
					else if( IsInsideRect( nX, nY, m_rcIcons ) )
					{
						int	nIconPos = m_sbSkillIcon.GetScrollPos();
						
						UIButton_map::iterator iter = m_mapPetSkill[pPetInfo->lIndex].m_btnmapSkills.begin();
						UIButton_map::iterator iterEnd = m_mapPetSkill[pPetInfo->lIndex].m_btnmapSkills.end();
										
						int nCount;
						for( nCount = 0; nCount < nIconPos; iter++, nCount++ );

						// Render 
						for( nCount = 0; ( iter != iterEnd ) && ( nCount < SLEARN_SLOT_ROW ); iter++, nCount++)
						{      							
							if( (wmsgResult = iter->second->MouseMessage( pMsg )) != WMSG_FAIL )
							{
								if( wmsgResult == WMSG_SUCCESS )
								{
									if (MY_INFO()->GetPetSkillDelay(pPetInfo->lIndex, iter->second->getIndex()) == false)
									{
										UseSkill( iter->second->getIndex() );
									}
								}
								return WMSG_SUCCESS;
							}		
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
				// List box of skill desc
				if( m_lbPetDesc.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Active skill tab
				else if( m_nCurrentTab == PETINFO_TAB_COMMAND )
				{
					// Active icon scroll bar
					if( m_sbCommandIcon.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nothing
					}
				}
				// Passive skill tab
				else if( m_nCurrentTab == PETINFO_TAB_SKILL )
				{
					// Passive icon scroll bar
					if( m_sbSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
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
				// List box of skill desc
				if( m_lbPetDesc.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// Active skill tab
				else if( m_nCurrentTab == PETINFO_TAB_COMMAND )
				{
					// Active icon scroll bar
					if( m_sbCommandIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				// Passive skill tab
				else if( m_nCurrentTab == PETINFO_TAB_SKILL )
				{
					// Passive icon scroll bar
					if( m_sbSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
			}
		}
		break;
	}

	return WMSG_FAIL;
}


//------------------------------------------------------------------------------
// CUIPetInfo::AddSkill
// Explain:  
// Date : 2005-09-27,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIPetInfo::AddSkill( int nPetIndex, int nSkillIndex, SBYTE sbSkillLevel )
{
	int nIndex = 0;
	SPetSkill_map::iterator	iter;

	ObjInfo* pInfo = ObjInfo::getSingleton();
	CPetTargetInfom* pPetInfo = pInfo->GetMyPetInfo();

	// 펫 데이터가 존재 하지 않다면 추가해 준다. ( Pet List ... )
	if( m_mapPetSkill.find( nPetIndex ) == m_mapPetSkill.end() )
	{
		SPetSkill 	sPetSkill;
			
		SPetSkill_map::value_type tmpVal( nPetIndex, sPetSkill);
		m_mapPetSkill.insert(tmpVal);
	}

	// 새로운 스킬을 추가한다
	CUIIcon* pbtnSkill = new CUIIcon();

	pbtnSkill->Create(this, 0, 0, BTN_SIZE, BTN_SIZE, UI_PETINFO, UBET_SKILL);
	pbtnSkill->setData(UBET_SKILL, nSkillIndex);
	UIButton_map::value_type tmpVal( nSkillIndex, pbtnSkill ); 
	
	int nSkillSize = -1;  

	switch( _pNetwork->GetSkillData( nSkillIndex ).GetType() )
	{
	case CSkill::ST_PET_COMMAND:			// Command
		{
			m_mapPetSkill[nPetIndex].m_btnmapCommands.insert( tmpVal );
			nSkillSize = m_mapPetSkill[nPetIndex].m_btnmapCommands.size();
		}
		break;

	case CSkill::ST_PET_SKILL_ACTIVE:		// Skill
	case CSkill::ST_PET_SKILL_PASSIVE:		
		{
			m_mapPetSkill[nPetIndex].m_btnmapSkills.insert( tmpVal );
			nSkillSize = m_mapPetSkill[nPetIndex].m_btnmapSkills.size();
		}
		break;
	default:
		SAFE_DELETE(pbtnSkill);
		break;
	}

	// 현재 펫이 활성화 되어 있고 해당 펫의 정보 일때만 스크롤 바를 조절함		
	if( pPetInfo->bIsActive && pPetInfo->lIndex == nPetIndex )
	{
		m_sbCommandIcon.SetCurItemCount( nSkillSize );
	}
}

//------------------------------------------------------------------------------
// CUIPetInfo::ClearSkills
// Explain:  
// Date : 2005-09-27,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIPetInfo::ClearSkills( int nPetIndex )
{
	if( nPetIndex == -1 )
	{
		m_mapPetSkill.clear();
	}
	else 
	{
		UIButton_map::iterator	iter;
		UIButton_map::iterator	eiter;

		iter = m_mapPetSkill[nPetIndex].m_btnmapCommands.begin();
		eiter = m_mapPetSkill[nPetIndex].m_btnmapCommands.end();

		for (;iter != eiter; ++iter)
		{
			SAFE_DELETE(iter->second);
		}

		m_mapPetSkill[nPetIndex].m_btnmapCommands.clear();

		iter = m_mapPetSkill[nPetIndex].m_btnmapSkills.begin();
		eiter = m_mapPetSkill[nPetIndex].m_btnmapSkills.end();

		for (;iter != eiter; ++iter)
		{
			SAFE_DELETE(iter->second);
		}

		m_mapPetSkill[nPetIndex].m_btnmapSkills.clear();
	}

	MY_INFO()->ClearPetSkill(nPetIndex);
}

//------------------------------------------------------------------------------
// CUIPetInfo::IsClearSkills
// Explain:  펫의 스킬 초기화 유무 
// Date : 2006-06-14(오전 11:17:42), By eons
//------------------------------------------------------------------------------
BOOL CUIPetInfo::IsClearSkills( int nPetIndex )
{
	BOOL	bIsClear = FALSE;

	if( nPetIndex == -1 )
	{
		if( m_mapPetSkill.empty() ) { bIsClear = TRUE; }
	}
	else
	{
		if( m_mapPetSkill[nPetIndex].m_btnmapCommands.empty() &&
				m_mapPetSkill[nPetIndex].m_btnmapSkills.empty() )
		{
			bIsClear = TRUE;
		}
	}

	return bIsClear;
}

//------------------------------------------------------------------------------
// CUIPetInfo::IsLearnSkill
// Explain:  
// Date : 2005-09-28,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
BOOL CUIPetInfo::IsLearnSkill( int nPetIndex, int nSkillIndex )
{
	switch( _pNetwork->GetSkillData( nSkillIndex ).GetType() )
	{
		case CSkill::ST_PET_COMMAND:			// Command
			{
				if( m_mapPetSkill[nPetIndex].m_btnmapCommands.find( nSkillIndex ) != m_mapPetSkill[nPetIndex].m_btnmapCommands.end() )
				{
					return TRUE;
				}
			}
			break;

		case CSkill::ST_PET_SKILL_ACTIVE:		// Skill
		case CSkill::ST_PET_SKILL_PASSIVE:
			{
				if( m_mapPetSkill[nPetIndex].m_btnmapSkills.find( nSkillIndex ) != m_mapPetSkill[nPetIndex].m_btnmapSkills.end() )
				{
					return TRUE;
				}
			}
			break;
	}

	return FALSE;
}

//------------------------------------------------------------------------------
// CPetInfo::UseCommand( 
// Explain:  
// 
//------------------------------------------------------------------------------
void CUIPetInfo::UseCommand( int nIndex )
{
	if( !((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsPetActing() )
	{
		_pNetwork->SendPetCommandMessage( nIndex, MY_PET_INFO()->pen_pEntity );
	}
}

//------------------------------------------------------------------------------
// CPetInfo::UseSkill( 
// Explain:  
// Date : 2005-09-29,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIPetInfo::UseSkill( int nIndex )
{
	if( !((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsPetActing() )
	{
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->UsePetSkill( nIndex );	
	}
}

// ----------------------------------------------------------------------------
// Name : StartSkillDelay()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetInfo::StartSkillDelay( int nIndex )
{
	if( nIndex == -1 ) 
		return;

	LONG	nPetIndex = MY_PET_INFO()->lIndex;

	if( nPetIndex == -1 )
		return;

	MY_INFO()->SetPetSkillDelay(nPetIndex, nIndex, true);
}

// ----------------------------------------------------------------------------
// Name : GetSkillDelay()
// Desc :
// ----------------------------------------------------------------------------
BOOL CUIPetInfo::GetSkillDelay( int nIndex )
{
	if( nIndex == -1 ) 
		return FALSE;
	LONG	nPetIndex = MY_PET_INFO()->lIndex;

	if( nPetIndex == -1 )
		return FALSE;

	UIButton_map::iterator	iter;	
	UIButton_map::iterator	iterBegin;
	UIButton_map::iterator	iterEnd;

	iterBegin	= m_mapPetSkill[nPetIndex].m_btnmapSkills.begin();
	iterEnd		= m_mapPetSkill[nPetIndex].m_btnmapSkills.end();
	iter		= iterBegin;
	for( INDEX nCount = 0; ( iter != iterEnd ) && ( nCount < SLEARN_SLOT_ROW ); iter++, nCount++ )
	{
		if( iter->second->getIndex() == nIndex )
		{
			if (iter->second->getIndex() > 0)
				return TRUE;
			else
				return FALSE;
		}
	}
	return FALSE;
}

//------------------------------------------------------------------------------
// CUIPetInfo::AddPetExchangeInfo
// Explain:  
// Date : 2005-10-04,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIPetInfo::AddPetExchangeInfo( SPetExchangInfo sPetExchangeInfo )
{
	SPetExchangInfo TempData;
	TempData = sPetExchangeInfo;
	int nPetIndex = TempData.lPetIndex;

	SPetExchangInfo_map::iterator iter = m_mapPetExchangeInfo.find( nPetIndex );
	
	if( iter != m_mapPetExchangeInfo.end() )	//이미 가지고 있는 펫 데이터이면 업데이트...
		iter->second =TempData;
	else										//가지고 있지 않는 펫 데이터이면 추가...
	{
		SPetExchangInfo_map::value_type tmpVal( nPetIndex, TempData );
		m_mapPetExchangeInfo.insert( tmpVal );
	}
}

//------------------------------------------------------------------------------
// CUIPetInfo::ClearExangeInfo
// Explain:  
// Date : 2005-10-04,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIPetInfo::ClearExangeInfo()
{
	m_mapPetExchangeInfo.clear();
}

//------------------------------------------------------------------------------
// CUIPetInfo::GetPetExchangeInfo
// Explain:  
// Date : 2005-10-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
BOOL CUIPetInfo::GetPetExchangeInfo( int nPetIndex, SPetExchangeInfoString& strPetExchangeInfo )
{
	CNetworkLibrary::sPetInfo	TempPet;
	TempPet.lIndex				= nPetIndex;

	std::vector<CNetworkLibrary::sPetInfo>::iterator iter;
	iter =std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );

	if( iter != _pNetwork->m_vectorPetList.end() )
		TempPet =*iter;
	else
	{
		SPetExchangInfo_map::iterator PetInfoiter = m_mapPetExchangeInfo.find( nPetIndex );

		if( PetInfoiter != m_mapPetExchangeInfo.end() )
		{
			TempPet.lLevel			= PetInfoiter->second.lLevel;
			TempPet.lHP				= PetInfoiter->second.lHP;
			TempPet.lMaxHP			= PetInfoiter->second.lMaxHP;
			TempPet.llExp			= PetInfoiter->second.llExp;
			TempPet.llNeedExp		= PetInfoiter->second.llNeedExp;
			TempPet.lHungry			= PetInfoiter->second.lHungry;	
			TempPet.lMaxSympathy	= PetInfoiter->second.lMaxSympathy;
			TempPet.lSympathy		= PetInfoiter->second.lSympathy;
			TempPet.lMaxSympathy	= PetInfoiter->second.lMaxSympathy;	
			TempPet.lAbility		= PetInfoiter->second.lAbility;
#ifdef PET_SEAL_TIME
			TempPet.lRemainRebirth	= PetInfoiter->second.lRemainRebirth;
#endif
			TempPet.strNameCard		= PetInfoiter->second.strNameCard;
		}
		else
		{
			return FALSE;
		}
	}

	if( TempPet.strNameCard.Length() >0 )
		strPetExchangeInfo.strNameCard.PrintF( _s("%s : %s"), _S(3526, "명찰"), TempPet.strNameCard);
	strPetExchangeInfo.strLevel.PrintF( _S(2254 , "레벨 : %d"), TempPet.lLevel );
	strPetExchangeInfo.strHP.PrintF( _S(2255,"생명력 : %d/%d"), TempPet.lHP, TempPet.lMaxHP );
	strPetExchangeInfo.strlExp.PrintF( _S(2256,"경험치 : %I64d/%I64d"), TempPet.llExp, TempPet.llNeedExp );
	strPetExchangeInfo.strHungry.PrintF( _S(2257,"배고픔 :  %d"),TempPet.lHungry );
	if( TempPet.lMaxSympathy == 0 ) TempPet.lMaxSympathy = 1;
//	strPetExchangeInfo.strSympathy.PrintF(_S(2258, "교감도 : %.0f%%"), iter->second.lSympathy/iter->second.lMaxSympathy*100 );
	strPetExchangeInfo.strSympathy.PrintF(_S(2258, "교감도 : %d%%"), TempPet.lSympathy );
	strPetExchangeInfo.strAbility.PrintF( _S(2259, "기술 포인트 : %d"), TempPet.lAbility );
#ifdef PET_SEAL_TIME
	if( TempPet.lRemainRebirth > 0 )
	{
		strPetExchangeInfo.strRemainTime.PrintF( _S( 2456, "봉인해제시간 : %d시간 %d분"), TempPet.lRemainRebirth/3600, (TempPet.lRemainRebirth%3600)/60 );	
	}
	else
	{
		strPetExchangeInfo.strRemainTime.Clear();
	}
#endif
	return TRUE;
}

LONG CUIPetInfo::GetRemainSealed(int nPetIdx)
{
	CNetworkLibrary::sPetInfo	TempPet;
	TempPet.lIndex				= nPetIdx;

	std::vector<CNetworkLibrary::sPetInfo>::iterator iter;
	iter =std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );

	if( iter != _pNetwork->m_vectorPetList.end() )
	{
		return (*iter).lRemainRebirth;
	}
	else
	{
		SPetExchangInfo_map::iterator PetInfoiter = m_mapPetExchangeInfo.find( nPetIdx );

		if( PetInfoiter != m_mapPetExchangeInfo.end() )
		{
			return PetInfoiter->second.lRemainRebirth;
		}
	}

	return 0;
}

//------------------------------------------------------------------------------
// CUIPetInfo::PetExchangeMount
// Explain:  
// Date : 2005-10-07,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIPetInfo::PetExchangeMount( LONG lResult )
{

	CTString strMessage;
	CTString strTitle;
	strTitle = _S(2182, "훈련 불가" );

	switch( lResult )
	{
	case MSG_EX_PET_CHANGE_MOUNT_ERROR_OK:
		strMessage = _S(2183, "애완동물이 탈것으로 변화되었습니다." );
		strTitle = _S(2184, "훈련 성공" );
		break;
	case MSG_EX_PET_CHANGE_MOUNT_ERROR_LEVEL:	// 레벨 부족
		strMessage = _S(2185, "애완동물의 레벨이 부족합니다. 애완동물은 31레벨 이상부터 탈 것 훈련을 받을 수 있습니다." );
		break;
	case MSG_EX_PET_CHANGE_MOUNT_ERROR_WEAR:	// 착용안함
		strMessage = _S(2186, "훈련 시킬 애완동물을 착용하여 주십시오." );
		break;
	case MSG_EX_PET_CHANGE_MOUNT_ERROR_GRADE:	// 이미 마운트 타입일 때
		strMessage = _S(2187, "이미 탈것으로 변화되었습니다." );
		break;
	case MSG_EX_PET_CHANGE_MOUNT_ERROR_JOB: // 마운트 할 수 없는 직업
		strMessage = _S(4701, "마운트를 할 수 없는 직업입니다."); //==! TODO[selo] 스트링
		break;		
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->SetCSFlagOff(CSF_PETRIDING);
		// Close message box of skill learn
	pUIManager->CloseMessageBox( MSGCMD_PET_CHANGE_NOTIFY );

	// Create message box of skill learn
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( strTitle, UMBS_OK,
								UI_NONE, MSGCMD_PET_CHANGE_NOTIFY );
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );

}

//------------------------------------------------------------------------------
// CUIMessenger::ToggleVisible
// Explain:  
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIPetInfo::ToggleVisible()
{
	if( IsVisible() )
	{
		g_iXPosInPetInfo = GetPosX();
		g_iYPosInPetInfo = GetPosY();
	}
	else
	{
		SetPos( g_iXPosInPetInfo, g_iYPosInPetInfo );
	}

	BOOL	bVisible = !IsVisible();
	CUIManager::getSingleton()->RearrangeOrder( UI_PETINFO, bVisible );

	if (bVisible == FALSE)
	{
		SPetSkill_map::iterator	iter = m_mapPetSkill.begin();
		SPetSkill_map::iterator	eiter = m_mapPetSkill.end();

		for (; iter != eiter; ++iter)
		{
			{
				UIButton_map::iterator citer = iter->second.m_btnmapCommands.begin();
				UIButton_map::iterator ceiter = iter->second.m_btnmapCommands.end();

				for (; citer != ceiter; ++citer)
				{
					citer->second->CloseProc();
				}				
			}

			{
				UIButton_map::iterator citer = iter->second.m_btnmapSkills.begin();
				UIButton_map::iterator ceiter = iter->second.m_btnmapSkills.end();

				for (; citer != ceiter; ++citer)
				{
					citer->second->CloseProc();
				}
			}
		}
	}
}

BOOL ENGINE_API PetStartEffectGroup(const char *szEffectGroupName, SLONG slPetIndex, CEntity *penPet )
{
	if( penPet == NULL )
	{
		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_PET, slPetIndex);

		if (pObject != NULL)
		{
			CEntity* pEntity = pObject->GetEntity();

			if (pEntity != NULL && 
				pEntity->en_pmiModelInstance != NULL && 
				pEntity->en_pmiModelInstance->GetName() != "")
			{
				StartEffectGroup(szEffectGroupName, 
					&(pEntity->en_pmiModelInstance->m_tmSkaTagManager),
					_pTimer->GetLerpedCurrentTick());
				return TRUE;
			}
		}
	}
	else
	{
		if( penPet != NULL && penPet->en_pmiModelInstance != NULL && penPet->en_pmiModelInstance->GetName() != "" )
		{
			StartEffectGroup( szEffectGroupName, &(penPet->en_pmiModelInstance->m_tmSkaTagManager),
								_pTimer->GetLerpedCurrentTick() );
			return TRUE;
		}
	}
	
	return FALSE;
}

SBYTE CUIPetInfo::GetPetTypeGrade( int nPetIndex )
{
	CNetworkLibrary::sPetInfo	TempPet;
	TempPet.lIndex				= nPetIndex;
		
	std::vector<CNetworkLibrary::sPetInfo>::iterator PetInfoiter = 
	std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
	//내 펫 목록에 펫 정보가 없을 경우 펫 교환 정보에서 검색
	if( PetInfoiter == _pNetwork->m_vectorPetList.end() )
	{
		SPetExchangInfo_map::iterator iter;
		SPetExchangInfo_map::iterator iterEnd = m_mapPetExchangeInfo.end();

		iter = m_mapPetExchangeInfo.find( nPetIndex );
		if( iter != iterEnd )
			return iter->second.sbPetTypeGrade;
		else
			return 0;
	}
	else
		return PetInfoiter->sbPetTypeGrade;
}

void CUIPetInfo::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	int nPetIdx = MY_PET_INFO()->lIndex;

	if (nPetIdx < 0)
		return;

	bool	bDelay;
	int	nIconPos = m_sbSkillIcon.GetScrollPos();

	UIButton_map::iterator iter = m_mapPetSkill[nPetIdx].m_btnmapSkills.begin();
	UIButton_map::iterator iterEnd = m_mapPetSkill[nPetIdx].m_btnmapSkills.end();

	int nCount;
	for( nCount = 0; nCount < nIconPos; iter++, nCount++ );

	// Render 
	for( nCount = 0; ( iter != iterEnd ) && ( nCount < SLEARN_SLOT_ROW ); iter++, nCount++)
	{
		bDelay = MY_INFO()->GetPetSkillDelay(nPetIdx, iter->second->getIndex());

		if (bDelay)
			iter->second->setCooltime(true);
	}		
}


