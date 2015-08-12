
#include "stdh.h"

#include <Engine/Interface/UIGroup.h>
#include <Engine/Interface/UITextureManager.h>
#include <algorithm>
#include <Engine/Interface/UIGroup.h>

using namespace std;

// Definition
#define GROUP_NAME_OFFSET_X 18	

// Color
#define COLOR_GROUP				0xe18600ff
#define COLOR_MEMBER			0xe2e2e2ff

#define ICON_WIDTH	18
#define ICON_HEIGHT 15

UIRectUV m_rcIcon[TOTAL_JOB][TOTAL_CONDITION];

//------------------------------------------------------------------------------
// CUIGroup::CUIGroup
// Explain:  
// Date : 2005-05-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUIGroup::CUIGroup()
{
	Clear();
}


//------------------------------------------------------------------------------
// CUIGroup
// Explain:  
// Date : 2005-05-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUIGroup::~CUIGroup()
{


}


//------------------------------------------------------------------------------
// CUIGroup::Clear
// Explain:  
// Date : 2005-05-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGroup::Clear()
{
	//m_btnExtension.Clear();	
	//m_btnReduction.Claer();	
	//m_mlMember.Clear();		

	//m_strGroupName = "Group";	// 생성시 기본 이름은 Group
	m_vecMember.clear();		// 멤버 삭제
	m_bExtension = false;		// 기본 설정은 확장 된 상태 ( 기억하고 있을 수는 없다. ) (X)
	//m_nScrollPos = 0;	
	m_bShowGroupName = true;
	
	m_nSelectLine = -2;
	m_bSelect =false;
	m_miSelectMember = CMemberInfo();

	m_nIndex =-2;
}




//------------------------------------------------------------------------------
// CUIGroup::Create
// Explain:  
// Date : 2005-05-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGroup::Create(  CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	m_nLineHeight =nHeight;

	//m_rtSelectBar.SetUV(3, 112, 11, 128);
	//m_rtSelectBar.SetUV(187, 46, 204, 61);
}


//------------------------------------------------------------------------------
// CUIGroup::SetGroupName
// Explain:  
// Date : 2005-05-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGroup::SetGroupName( CTString strGroupName )
{
	m_strGroupName = strGroupName;
}


//------------------------------------------------------------------------------
// CUIGroup::AddMember
// Explain:  
// Date : 2005-05-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGroup::AddMember( int nCharIndex, int nGroupIndex, CTString strName, eJob nJob, eCondition eState )
{
	CMemberInfo miTemp;

	miTemp.SetInfo( nCharIndex, nGroupIndex, strName, nJob, eState );

	m_vecMember.push_back( miTemp );
	/*****	
	m_mlMember.AddIcon( 0, m_rcIcon[nJob][eState] );
	m_mlMember.AddString( 1, strName, COLOR_MEMBER );

	m_mlMember.ChangeLineCount( m_vecMember.size() );
	*****/

	ReloadMember();
}


//------------------------------------------------------------------------------
// CUIGroup::AddMember
// Explain:  
// Date : 2005-05-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGroup::AddMember( CMemberInfo MemberInfo )
{
	std::vector<CMemberInfo>::iterator iter;
	iter =find_if(m_vecMember.begin(), m_vecMember.end(), bind2nd(ptr_fun(FindObj), MemberInfo.m_strName) );

	if( iter == m_vecMember.end() )
	{
	m_vecMember.push_back( MemberInfo );

		if( m_vecMember.size() ==1 )
			SetExtension(true);

		ReloadMember();
	}
}


//------------------------------------------------------------------------------
// CUIGroup::DeleteMember
// Explain:  
// Date : 2005-05-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
bool CUIGroup::DeleteMember( CTString strName )
{
	//친구리스트 검색 후 삭제
	std::vector<CMemberInfo>::iterator iterBegin = m_vecMember.begin();
	std::vector<CMemberInfo>::iterator iterEnd = m_vecMember.end();
	std::vector<CMemberInfo>::iterator iter;
	
	for( iter = iterBegin; iter != iterEnd; ++iter) 
	{
		if( iter->m_strName == strName )
		{
			iter = m_vecMember.erase( iter );
			iterEnd = m_vecMember.end();
			break;
		}
	}

	if( m_vecMember.empty() )
		SetExtension(false);
	
	if( m_nSelectLine < m_vecMember.size())
		m_miSelectMember =m_vecMember[m_nSelectLine];

	if( iter == iterEnd)
		return false;
	else
		return true;
}


bool CUIGroup::DeleteMember( int nCharIndex)
{
	//친구리스트 검색 후 삭제
	std::vector<CMemberInfo>::iterator iterBegin = m_vecMember.begin();
	std::vector<CMemberInfo>::iterator iterEnd = m_vecMember.end();
	std::vector<CMemberInfo>::iterator iter;
	int i = 0;

	for( iter = iterBegin; iter != iterEnd; ++iter) 
	{
		if( iter->m_nCharIndex == nCharIndex )
		{
			iter = m_vecMember.erase( iter );
			iterEnd = m_vecMember.end();
			break;
		}
		i++;
	}

	if( m_vecMember.empty() )
		SetExtension(false);

	if( m_nSelectLine < m_vecMember.size())
		m_miSelectMember =m_vecMember[m_nSelectLine];


	if( iter ==iterEnd )
		return false;
	else
		return true;
}

/************************************************************************************
//------------------------------------------------------------------------------
// CUIGroup::GetSelectMember
// Explain:  
// Date : 2005-05-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CMemberInfo CUIGroup::GetSelectMember()
{
	CMemberInfo miMemberInfo;
	int nSelectMember = m_mlMember.GetCurSel();

	if( nSelectMember < 0 ) return miMemberInfo ;
	if( nSelectMember >= m_vecMember.size() ) return miMemberInfo;
	
	return m_vecMember[nSelectMember];
}
**************************************************************************************/

//------------------------------------------------------------------------------
// CUIGroup::SetMemberCondition
// Explain:  
// Date : 2005-05-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
bool CUIGroup::SetMemberCondition( int nCharIndex, eCondition eState )
{	
	for(int i=0; i<m_vecMember.size(); ++i)
	{
		if( m_vecMember[i].m_nCharIndex ==nCharIndex )
		{
			m_vecMember[i].m_eCondition =eState;

			ReloadMember();
			return true;
		}
	}

	return false;
}


//------------------------------------------------------------------------------
// CUIGroup::SetExtension
// Explain:  
// Date : 2005-05-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGroup::SetExtension( bool bExtension )
{
	if( m_vecMember.empty() )
	{
		// 맴버가 전혀 없는 경우에는 그냥 넘어감~
		//m_bExtension = true;
		m_bExtension = false;
	}
	else 
		m_bExtension = bExtension;
	}
	

//------------------------------------------------------------------------------
// CUIGroup::ReloadMember
// Explain:  데이터에 저장된 데이터를 ListView에 정렬해서 넣는다.
//			호출시기 
//			1. 데이터가 새로 입력될 때
//			2. 맴버의 상태가 바뀔 때 
// Date : 2005-05-24,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGroup::ReloadMember()
{
	/*****
	std::vector<CMemberInfo>::iterator iterBegin = m_vecMember.begin();
	std::vector<CMemberInfo>::iterator iterEnd = m_vecMember.end();
	std::vector<CMemberInfo>::iterator iter;
	*****/
	
	//std::vector<CMemberInfo> vecOffLineMember;
	//std::vector<CMemberInfo> vecOnLineMember;

	//! Sort ( 가나다 순 ... )
	std::stable_sort( m_vecMember.begin(), m_vecMember.end(), CompareObj );
	//온라인 상태인 멤버를 앞으로...
	std::stable_partition(m_vecMember.begin(), m_vecMember.end(), CompareCondition);

	/**********
	iterBegin = m_vecMember.begin();
	iterEnd = m_vecMember.end();
	
	// 일단 다 지워
	m_mlMember.ResetAllData();
	m_mlMember.ChangeLineCount( 0 );
	
	int nCount = 0;
	for( iter = iterBegin; iter != iterEnd; ++iter) 
	{
		if( iter->m_eCondition == OFFLINE )
		{
			vecOffLineMember.push_back( (*iter) );
		}
		else
		{
			vecOnLineMember.push_back( (*iter) );
			m_mlMember.AddIcon( 0, m_rcIcon[iter->m_eJob][iter->m_eCondition] );
			m_mlMember.AddString( 1, iter->m_strName, COLOR_MEMBER );
		}
		
	}

	// 남겨뒀던 오프라인 맴버 처리 
	iterBegin = vecOffLineMember.begin();
	iterEnd = vecOffLineMember.end();
	
	for( iter = iterBegin; iter != iterEnd; ++iter) 
	{
		vecOnLineMember.push_back( *iter );
		m_mlMember.AddIcon( 0, m_rcIcon[iter->m_eJob][iter->m_eCondition] );
		m_mlMember.AddString( 1, iter->m_strName, COLOR_MEMBER );	
	}
	
	m_vecMember = vecOnLineMember;
	**********/
}




/**************************************************************************************
//------------------------------------------------------------------------------
// CUIGroup::ResetAllMember
// Explain:  
// Date : 2005-05-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGroup::ResetAllMember()
{
	m_mlMember.ResetAllStrings();
	m_vecMember.clear();
	SetExtension( true );
}
	

//------------------------------------------------------------------------------
// CUIGroup::SetScrollPos
// Explain:  
// Date : 2005-05-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
int CUIGroup::SetScrollPos( int nPos )
{ 
	m_mlMember.SetScrollBarPos( nPos ); 
	return 0;
}
**************************************************************************************/

//------------------------------------------------------------------------------
// CUIGroup::Render
// Explain:  
// Date : 2005-05-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
//int CUIGroup::Render( int& nShowHeight )
int CUIGroup::Render( int& nLineY, int nShowHeight )
{
	int nGroupY = nLineY *m_nLineHeight;
	SetPos( m_nPosX, GROUP_Y +nGroupY );

	int	nX, nY;
	GetAbsPos( nX, nY );

	int nShowCount = 0;
	int nShowRange = GetShowRange();
	int nLeftShowRange = 0;
	int nStartLine =0;

	SetSize( m_nWidth, nShowRange);

	//if( nGroupY+nShowRange <= GROUP_Y)
	if( (nGroupY+nShowRange) <=0)
	//if( nLineY +m_vecMember.size()+1 <=0)
	{
		SetShowGroupName(false);
		nLineY +=nShowRange/m_nLineHeight;
		SetVisible(false);
		return 0;
	}
	//else if( nGroupY < GROUP_Y)
	else if( nGroupY < 0)
	{
		nStartLine = -nLineY -1;
		SetShowGroupName(false);
		
		nLineY = 0;
		SetVisible(true);
	}
	else if( nGroupY< nShowHeight)
	{
		SetShowGroupName(true);
		nStartLine =0;
		SetVisible(true);
	}
	else
	{
		SetShowGroupName(false);
		SetVisible(false);
		return 0;
	}

	if( m_bShowGroupName)
	{
		// 버튼 출력
		if( m_bExtension ) 
		{
			m_btnReduction.Render();
		}
		else
		{
			m_btnExtension.Render();
		}
		++nLineY;
	}
			
	if( IsExtension() )
	{
		CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

		for(int i=nStartLine;; ++i)
		{
			if( nLineY*m_nLineHeight >=nShowHeight || i>m_vecMember.size()-1)
				break;

			//친구 그룹일 때
			if( m_nIndex >=0)
			{
				pDrawPort->PutTextEx( m_vecMember[i].m_strName, nX +GROUP_MEMBER_NAME_OFFSET_X,
													nY +m_nLineHeight*(i+1),
													0xf2f2f2ff);
									
				pDrawPort->AddTexture( nX + GROUP_MEMBER_ICON_OFFSET_X, nY +m_nLineHeight*(i+1),
												nX + GROUP_MEMBER_ICON_OFFSET_X+ ICON_WIDTH, nY +m_nLineHeight*(i+1) +ICON_HEIGHT,
												m_rcIcon[m_vecMember[i].m_eJob][m_vecMember[i].m_eCondition].U0, m_rcIcon[m_vecMember[i].m_eJob][m_vecMember[i].m_eCondition].V0,
												m_rcIcon[m_vecMember[i].m_eJob][m_vecMember[i].m_eCondition].U1, m_rcIcon[m_vecMember[i].m_eJob][m_vecMember[i].m_eCondition].V1,
												0Xffffffff );
				++nLineY;
				/***
				else
				{
					if( i ==m_nSelectLine )
						++m_nSelectLine;
				}
				***/
			}
			//차단 그룹일 때
			else
			{
				pDrawPort->PutTextEx( m_vecMember[i].m_strName, nX +GROUP_MEMBER_NAME_OFFSET_X,
														nY +m_nLineHeight*(i+1),
														0xf2f2f2ff);

				if( m_vecMember[i].m_nGroupIndex >=0 )
				{
					pDrawPort->AddTexture( nX + GROUP_MEMBER_ICON_OFFSET_X, nY +m_nLineHeight*(i+1),
														nX + GROUP_MEMBER_ICON_OFFSET_X+ ICON_WIDTH, nY +m_nLineHeight*(i+1) +ICON_HEIGHT,
														m_rcIcon[m_vecMember[i].m_eJob][m_vecMember[i].m_eCondition].U0, m_rcIcon[m_vecMember[i].m_eJob][m_vecMember[i].m_eCondition].V0,
														m_rcIcon[m_vecMember[i].m_eJob][m_vecMember[i].m_eCondition].U1, m_rcIcon[m_vecMember[i].m_eJob][m_vecMember[i].m_eCondition].V1,
														0Xffffffff );
				}

				++nLineY;
	}
	
			//SelectBar render
			if( i==m_nSelectLine && IsFocused())
			{
				pDrawPort->AddTexture( nX + GROUP_MEMBER_NAME_OFFSET_X, nY + m_nLineHeight*(i+1),
													//nX + GROUP_MEMBER_NAME_OFFSET_X +m_vecMember[i].m_strName.Length()*8, 
													nX + GROUP_MEMBER_NAME_OFFSET_X +m_vecMember[i].m_strName.Length()*( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() +1),
													nY + m_nLineHeight*(i+2),
													m_rtSelectBar.U0, m_rtSelectBar.V0,
													m_rtSelectBar.U1, m_rtSelectBar.V1,
													0xFFFFFFFF );
			}
		}
	}


	// 그룹 이름 출력 ( 인원 수도 표시 ) 
	if( m_bShowGroupName)
	{
		CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
		CTString strGroupName;

		strGroupName.PrintF( "%s (%d)", m_strGroupName, m_vecMember.size() );
		//strGroupName.PrintF( "%s (%d)", m_strGroupName, (nShowRange/m_nLineHeight) -1 );
		
		if( m_nSelectLine ==-1 && IsFocused())
		{
			pDrawPort->AddTexture( nX + GROUP_NAME_OFFSET_X, nY,
												//nX + GROUP_MEMBER_NAME_OFFSET_X +m_vecMember[i].m_strName.Length()*8, 
												nX + GROUP_NAME_OFFSET_X +strGroupName.Length()*( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() +1),
												nY + m_nLineHeight,
												m_rtSelectBar.U0, m_rtSelectBar.V0,
												m_rtSelectBar.U1, m_rtSelectBar.V1,
												0xFFFFFFFF );
		}
		pDrawPort->PutTextEx( strGroupName, nX + GROUP_NAME_OFFSET_X, nY, 
			COLOR_GROUP);
	}

	if( !IsFocused() )
		m_nSelectLine =-2;

	return 1;
}


//------------------------------------------------------------------------------
// CUIGroup::MouseMessage
// Explain:  
// Date : 2005-05-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT	CUIGroup::MouseMessage( MSG *pMsg )
{
	if( !IsEnabled() )
		return WMSG_FAIL;

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	switch( pMsg->message )
	{
		case WM_MOUSEMOVE :
			break;
		case WM_LBUTTONDOWN :
		case WM_RBUTTONDOWN :
			if( IsInside( nX, nY ) )
			{
				int dY = nY - GetAbsPosY();
				m_nSelectLine = dY/m_nLineHeight -1;
				if( dY > m_nLineHeight)
				{			
					m_bSelect =true;
					m_miSelectMember =m_vecMember[m_nSelectLine];
				}
				else
					m_miSelectMember =CMemberInfo();
								
				return WMSG_SUCCESS;
			}

			m_bSelect = false;
			m_nSelectLine =-2;
			break;
		case WM_LBUTTONUP:
			m_bSelect =false;
			if( IsInside( nX, nY ) )
			{
				if( nY -GetAbsPosY() <m_nLineHeight )
				{
					SetExtension( !IsExtension() );
					return WMSG_SUCCESS;
				}

				if( m_bShowGroupName)
				{
					if( !IsExtension() )
					{
						if( m_btnExtension.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							SetExtension( TRUE );
							return WMSG_SUCCESS;
						}
					}
					else
					{
						if( m_btnReduction.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							SetExtension( FALSE );
							return WMSG_SUCCESS;
						}
					}
				}
				return WMSG_SUCCESS;
			}
			break;
		case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
				return WMSG_COMMAND;
		}
		break;

	}

	return WMSG_FAIL;
}


//------------------------------------------------------------------------------
// CUIGroup::GetShowRange
// Explain:  그룹리스트 출력시 사용할 영역 크기 리턴 ( 그룹이름은 제외 )
//			1. Scroll Pos
//			2. Extension
// Date : 2005-05-22,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
int CUIGroup::GetShowRange()
{
	int nShowHeight = m_nLineHeight;
	
	if( IsExtension() )
	{
		/*
		if( m_nIndex >=0 )
		{
			for(int i=0; i<m_vecMember.size(); ++i)
			{
				if( !m_vecMember[i].m_bBlock )
					nShowHeight += m_nLineHeight;
			}
		}
		else
		*/
		nShowHeight += m_vecMember.size() *m_nLineHeight;
	}

	return nShowHeight;
}

/**************************************************************************************
//------------------------------------------------------------------------------
// CUIGroup::GetMemberInfo
// Explain:  
// Date : 2005-05-25,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CMemberInfo CUIGroup::GetMemberInfo( int nIndex )
{
	CMemberInfo miMemberInfo;
		
	std::vector<CMemberInfo>::iterator iterBegin = m_vecMember.begin();
	std::vector<CMemberInfo>::iterator iterEnd = m_vecMember.end();
	std::vector<CMemberInfo>::iterator iter;

	for( iter = iterBegin; iter != iterEnd; ++iter) 
	{
		if( iter->m_nCharIndex == nIndex )
		{
			miMemberInfo = *iter;
			break;
		}
	}
	return miMemberInfo;
}

//------------------------------------------------------------------------------
// CUIGroup::GetMemberInfo
// Explain:  
// Date : 2005-05-25,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CMemberInfo CUIGroup::GetMemberInfo( CTString strName )
{
	CMemberInfo miMemberInfo;
		
	std::vector<CMemberInfo>::iterator iterBegin = m_vecMember.begin();
	std::vector<CMemberInfo>::iterator iterEnd = m_vecMember.end();
	std::vector<CMemberInfo>::iterator iter;

	for( iter = iterBegin; iter != iterEnd; ++iter) 
	{
		if( iter->m_strName == strName )
		{
			miMemberInfo = *iter;
			break;
		}
	}
	return miMemberInfo;
}
**************************************************************************************/

//------------------------------------------------------------------------------
// CompareObj
// Explain:  
// Date : 2005-05-24,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
bool CompareObj( CMemberInfo first, CMemberInfo second )
{
	std::string strFirst, strSecond;

	strFirst = first.m_strName;
	strSecond = second.m_strName;

	return strFirst < strSecond;

}

bool CompareCondition(CMemberInfo info)
{
	bool IsOnline =false;
	if(info.m_eCondition != OFFLINE)
		IsOnline =true;

	return IsOnline;
}

bool FindObj( CMemberInfo mi, CTString strName)
{
	if( strcmp(mi.m_strName , strName) == 0)
		return true;
	else
		return false;

}

void CUIGroup::SetIndex(int nIndex)
{
	m_nIndex =nIndex;
}

CMemberInfo CUIGroup::GetMemberInfo(int nCharIndex)
{
	for(int i=0; i<m_vecMember.size(); ++i)
	{
		if( m_vecMember[i].m_nCharIndex ==nCharIndex )
		{
			return m_vecMember[i];
		}
	}

	return CMemberInfo();
}
