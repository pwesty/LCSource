#include "stdh.h"

#include "UIListBoxEx.h"

#include "UIManager.h"

CUIListBoxEx::CUIListBoxEx()
: m_eCurrentState( PS_NONE )
, m_iLastChangeLine( -1 )
, m_iPopBtnSpaceX( 0 )
, m_iPopBtnSpaceY( 0 )
, m_iPopBtnWidth( 0 )
, m_iPopBtnHeight( 0 )
, m_iLastCheckedIndex( -1 )
, m_iCheckBtnWidth( 0 )
, m_iCheckBtnHeight( 0 )
, m_iCheckBtnSpaceX( 0 )
, m_iCheckBtnSpaceY( 0 )
{
}

CUIListBoxEx::~CUIListBoxEx()
{
	Reset();
}

void CUIListBoxEx::Reset()
{
	for(int i=0; i<m_aPopLine.size(); ++i)
	{
		delete m_aPopLine[i].pChkButton;
		m_aPopLine[i].pChkButton = NULL;
	}
	m_aPopLine.clear();

	std::map<int, CUIImageBox*>::iterator iterBegin = m_mapImageBox.begin();
	std::map<int, CUIImageBox*>::iterator iterEnd = m_mapImageBox.end();
	std::map<int, CUIImageBox*>::iterator iter;
	for(iter=iterBegin; iter!=iterEnd; ++iter)
	{
		delete (*iter).second;
	}
	m_mapImageBox.clear();

	m_eCurrentState = PS_NONE;
	m_iLastChangeLine = -1;
}

void CUIListBoxEx::ChangeCurrentState(ePopState state)
{
	int currentLine = m_vecSelectable.size();
	ASSERT(m_iLastChangeLine != currentLine);

	m_eCurrentState = state;
	switch(state)
	{
	case PS_NONE:
	case PS_CHILD:
		{
			for(int i=m_iLastChangeLine; i<currentLine; ++i)
			{
				sPopLine line(state, NULL);
				m_aPopLine.push_back(line);
			}
		} break;
		// 2009. 06. 02 김정래
		// checkbutton 을 가지는 Child 처리
	case PS_CHECKCHILD:
		{
			for(int i=m_iLastChangeLine; i<currentLine; ++i)
			{
				CUICheckButton *pBtn = new CUICheckButton;
				pBtn->Create(this, m_iCheckBtnSpaceX, (currentLine+1) * m_nLineHeight + m_iCheckBtnSpaceY, m_iCheckBtnWidth, m_iCheckBtnHeight, CTString());
				for(int i=0; i<UCBS_TOTAL; ++i)
				{
					pBtn->SetUV((UICheckBtnState)i, m_rtCheckBtnUV[i]);
				}
				pBtn->SetCheck( FALSE );
				pBtn->SetEnable(TRUE);
				sPopLine line(state, pBtn);
				m_aPopLine.push_back(line);

			}
		} break;

	case PS_EXTEND:
	case PS_CLOSE:
		{
			CUICheckButton *pBtn = new CUICheckButton;
			pBtn->Create(this, m_iPopBtnSpaceX, (currentLine+1) * m_nLineHeight + m_iPopBtnSpaceY, m_iPopBtnWidth, m_iPopBtnHeight, CTString());
			for(int i=0; i<UCBS_TOTAL; ++i)
			{
				pBtn->SetUV((UICheckBtnState)i, m_rtPopBtnUV[i]);
			}
			pBtn->SetCheck( state == PS_CLOSE );
			pBtn->SetEnable(TRUE);
			sPopLine line(state, pBtn);
			m_aPopLine.push_back(line);
		};
	}
	m_iLastChangeLine = currentLine;
}

void CUIListBoxEx::ChangeState(int iLine, ePopState state)
{
	ASSERT(m_aPopLine.size() > iLine && iLine >= 0);
	ASSERT(state == PS_EXTEND || state == PS_CLOSE);
	ePopState reverseState;
	int cntValue;
	if(state == PS_CLOSE)
	{
		reverseState = PS_EXTEND;
		cntValue = -1;
	}
	else
	{
		reverseState = PS_CLOSE;
		cntValue = +1;
	}

	if(m_aPopLine[iLine].eState == reverseState )
	{
		int heightCnt = 0;
		m_aPopLine[iLine].eState = state;
		for(int i=iLine+1; i<m_aPopLine.size(); ++i)
		{
			if(m_aPopLine[i].eState == PS_EXTEND || m_aPopLine[i].eState == PS_CLOSE)
			{
				break;
			}
			heightCnt += cntValue;
		}
		m_sbScrollBar.SetCurItemCount( m_sbScrollBar.GetCurItemCount() + heightCnt );
	}
}

void CUIListBoxEx::Extend(int iLine)
{
	ChangeState(iLine, PS_EXTEND);
}

void CUIListBoxEx::Close(int iLine)
{
	ChangeState(iLine, PS_CLOSE);
}

void CUIListBoxEx::SetImageBox(int row, CUIImageBox::eImageType type, int index, BOOL bShowPopup /* = FALSE */, CTString popupInfo/* = CTString("") */, int nSyndiType/* = 0*/)
{
	CUIImageBox *pImageBox;
	pImageBox = new CUIImageBox;
	pImageBox->Create(this, 0, 0, 32, 32);
	pImageBox->SetImageByType(type, index, nSyndiType);
	pImageBox->SetRenderRegion(0,0,32,16);
	pImageBox->SetPopupInfo( popupInfo, bShowPopup );
	std::map<int, CUIImageBox*>::value_type value1(row, pImageBox);
	m_mapImageBox.insert(value1);
}

// =====================================================================
//  [6/2/2009 selo]
// Name : GetCheckCount()
// Desc : 체크된 child 항목들의 수 반환.
//		  m_aPopLine 에서 PS_CHECKCHILD 인 녀석중에 체크된 것이 있으면
//		  Count를 올려 반환한다.
// =====================================================================
int	CUIListBoxEx::GetCheckCount()
{
	int iCount = 0;

	for( int i = 0; i < m_aPopLine.size(); ++i )
	{
		if( PS_CHECKCHILD == m_aPopLine[i].eState && m_aPopLine[i].pChkButton )
		{
			if( m_aPopLine[i].pChkButton->IsChecked() )			
				++iCount;
		}		
	}

	return iCount;
}

// =====================================================================
//  [6/2/2009 selo]
// Name : SetCheckState()
// Desc : 해당 인덱스의 check 상태를 변경.
//		  m_aPopLine 에서 PS_CHECKCHILD 인 녀석중에 인덱스가 맞으면
//		  Check 상태를 변경한다.
// =====================================================================
void CUIListBoxEx::SetCheckState(int index, BOOL bChecked)
{	
	if( PS_CHECKCHILD == m_aPopLine[index].eState && m_aPopLine[index].pChkButton )
	{
		m_aPopLine[index].pChkButton->SetCheck(bChecked);
	}	
}

// =====================================================================
//  [6/2/2009 selo]
// Name : GetCheckState()
// Desc : 해당 인덱스의 check 상태를 반환.
//		  m_aPopLine 에서 PS_CHECKCHILD 인 녀석중에 인덱스가 맞으면
//		  Check 상태를 반환한다.
// =====================================================================
BOOL CUIListBoxEx::GetCheckState(int index)
{
	if( PS_CHECKCHILD == m_aPopLine[index].eState && m_aPopLine[index].pChkButton )
	{
		return m_aPopLine[index].pChkButton->IsChecked();
	}

	return false;
}


void CUIListBoxEx::Render()
{
	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Add render regions
	// Selection bar
	if( m_bShowSelectBar && m_nSelectList >= 0 )
	{
		int	nSelLine = m_nSelectList - m_sbScrollBar.GetScrollPos();
		
		if( nSelLine >= 0 )
		{
			if( !m_bScrollBar || nSelLine < m_sbScrollBar.GetScrollRange() )
			{
			
				int	nBarY = nY + m_nTextSY + nSelLine * m_nLineHeight;
				pDrawPort->AddTexture( nX + m_rcSelectOver.Left, nBarY + m_rcSelectOver.Top,
													nX + m_rcSelectOver.Right, nBarY + m_rcSelectOver.Bottom,
													m_rtSelectOver.U0, m_rtSelectOver.V0,
													m_rtSelectOver.U1, m_rtSelectOver.V1,
													0xFFFFFFFF );
			}
		}
	}

	// Scroll bar
	if( m_bScrollBar )
		m_sbScrollBar.Render();

	int	nRowS = m_sbScrollBar.GetScrollPos();
	int	nRowE = nRowS + m_nLinePerPage;
	if( nRowE > m_sbScrollBar.GetCurItemCount() )
		nRowE = m_sbScrollBar.GetCurItemCount();

	// Text in list box
	for( int nCol = 0; nCol < m_vecString.size(); ++nCol )
	{
		COLOR	colText;
		int		nTextX = nX + m_nTextSX + m_vecColumnSX[nCol];
		int		nTextY = nY + m_nTextSY;
		int		nSelList = -1, nOverList = -1;
		if( m_bSelectList )
		{
			nSelList = m_nSelectList;
			nOverList = nRowS + m_nOverList;
		}

		ePopState eParentState = PS_NONE;
		for( int nList = 0, nListEx = 0; nList < m_vecSelectable.size(); ++nList )
		{
			bool bContinue = false;
			bool bRenderCheckButton = false;
			switch(m_aPopLine[nList].eState)
			{
			case PS_NONE:
				{
					eParentState = m_aPopLine[nList].eState;
					++nListEx;
				} break;
			case PS_EXTEND:
			case PS_CLOSE:
				{
					eParentState = m_aPopLine[nList].eState;
					bRenderCheckButton = true;
					++nListEx;
				} break;
			case PS_CHILD:
				{
					if(eParentState == PS_EXTEND)
						++nListEx;
					else
						bContinue = true;
				} break;
			case PS_CHECKCHILD:	// [090602: selo] checkbox 를 가지는 child 처리 
				{
					if(eParentState == PS_EXTEND)
					{
						bRenderCheckButton = true;
						++nListEx;
					}
					else
						bContinue = true;
				} break;
			}
			if(bContinue) continue;
			if(nRowS >= nListEx) continue;
			if(nRowE < nListEx) break;

			if(nCol == 0 && bRenderCheckButton)
			{
				m_aPopLine[nList].pChkButton->SetPos(m_nTextSX/2, nTextY - nY);
				m_aPopLine[nList].pChkButton->Render();
			}
			
			if( m_vecSelectable[nList] )
			{
				if( nList == nSelList )
					colText = m_colSelectList;
				else if( nList == nOverList )
					colText = m_colOverList;
				else
					colText = m_vecColor[nCol].vecColor[nList];
			}
			else
				colText = m_vecColor[nCol].vecColor[nList];

			switch( m_vecAlign[nCol] )
			{
			case TEXT_LEFT:
				pDrawPort->PutTextEx( m_vecString[nCol].vecString[nList], nTextX, nTextY, colText );
				break;
			case TEXT_CENTER:
				pDrawPort->PutTextExCX( m_vecString[nCol].vecString[nList], nTextX, nTextY, colText );
				break;
			case TEXT_RIGHT:
				pDrawPort->PutTextExRX( m_vecString[nCol].vecString[nList], nTextX, nTextY, colText );
				break;
			}

			nTextY += m_nLineHeight;
		}
	}
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();
	
	//주의 팝업-다운과 ImageBox는 같이 쓸 수 없음.
	std::map<int, CUIImageBox*>::iterator iterBegin = m_mapImageBox.begin();
	std::map<int, CUIImageBox*>::iterator iterEnd = m_mapImageBox.end();
	std::map<int, CUIImageBox*>::iterator iter;
	int		nTextY = nY + m_nTextSY;
	for(iter=iterBegin; iter!=iterEnd; ++iter)
	{
		int myRowS = (*iter).first;
		CUIImageBox *pIBox = (*iter).second;

		bool bRender = false;
		int visibleCount = int( ceilf( pIBox->GetHeight() / float(m_nLineHeight) ) );
		int myRowE = myRowS + visibleCount + 1;
		int nUpper = 0;
		int nLower = pIBox->GetHeight();

		//윗부분이 짤림. 아래 부분만 보임
		if(myRowS < nRowS && myRowE > nRowS)
		{
			bRender = true;
			nUpper = (nRowS - myRowS) * m_nLineHeight;
		}
		//아래부분이 짤림. 위 부분만 보임.
		if(myRowS < nRowE && myRowE > nRowE)
		{
			bRender = true;
			nLower = (nRowE - myRowS) * m_nLineHeight;
			nLower = ClampUp(nLower, pIBox->GetHeight());
		}
		//다 보임.
		if(myRowS >= nRowS && myRowE <= nRowE)
		{
			bRender = true;
		}

		if(bRender)
		{
			pIBox->SetRenderRegion(0, nUpper, pIBox->GetWidth(), nLower);
			(*iter).second->SetPos(m_nTextSX/2, m_nTextSY + (myRowS - nRowS) * m_nLineHeight);
			(*iter).second->Render();
		}
	}

}

WMSG_RESULT	CUIListBoxEx::MouseMessage( MSG *pMsg )
{
	// If list box is disabled
	if( !IsEnabled() )
		return WMSG_FAIL;

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( m_bSelectList )
			{
				std::map<int, CUIImageBox*>::const_iterator itr;
				itr = m_mapImageBox.begin();
				for ( ; itr != m_mapImageBox.end() ; ++itr )
				{
					CUIImageBox* pImageBox = (*itr).second;
					pImageBox->MouseMessage( pMsg );
				}
				m_nOverList = -1;

				if( IsInside( nX, nY ) )
				{
					int	nAbsY = GetAbsPosY();
					m_nOverList = ( nY - nAbsY - m_nTextSY + m_nSelectOffsetY ) / m_nLineHeight;

					return WMSG_SUCCESS;
				}
			}

			if( m_bScrollBar )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( m_bSelectList )
			{
				if( IsInside( nX, nY ) )
				{
					int	nRowS = m_sbScrollBar.GetScrollPos();
					int	nRowE = nRowS + m_nLinePerPage;
					if( nRowE > m_sbScrollBar.GetCurItemCount() )
						nRowE = m_sbScrollBar.GetCurItemCount();

					int	nAbsY = GetAbsPosY();
					m_nOverList = ( nY - nAbsY - m_nTextSY + m_nSelectOffsetY ) / m_nLineHeight;
					int	nRealSel = m_nOverList + m_sbScrollBar.GetScrollPos();

					int nListEx = -1;
					ePopState eParentState = PS_NONE;
					for(int i=0; i<m_aPopLine.size(); ++i)
					{
						bool bContinue = false;
						bool bRenderCheckButton = false;
						bool bCheckChildButton = false;
						switch(m_aPopLine[i].eState)
						{
						case PS_NONE:
							{
								eParentState = m_aPopLine[i].eState;
								++nListEx;
							} break;
						case PS_EXTEND:
						case PS_CLOSE:
							{
								eParentState = m_aPopLine[i].eState;
								bRenderCheckButton = true;
								++nListEx;
							} break;
						case PS_CHILD:
							{
								if(eParentState == PS_EXTEND)
									++nListEx;
								else
									bContinue = true;
							} break;
						case PS_CHECKCHILD: // [090602: selo] checkbutton 있는 child 처리
							{
								if(eParentState == PS_EXTEND)
								{
									bCheckChildButton = true;
									++nListEx;
								}
								else
									bContinue = true;
							} break;
						}
						if(bContinue) continue;
						if(nRowS > nListEx) continue;
						if(nRowE <= nListEx) break;

						if(bRenderCheckButton
							&& m_aPopLine[i].pChkButton
							&& m_aPopLine[i].pChkButton->IsInside(nX, nY)
							&& m_aPopLine[i].pChkButton->IsEnabled())
						{
							if(m_aPopLine[i].pChkButton->IsChecked())
							{
								m_aPopLine[i].pChkButton->SetCheck(FALSE);
								Extend(i);
							}
							else
							{
								m_aPopLine[i].pChkButton->SetCheck(TRUE);
								Close(i);
							}
							m_nSelectList = -1;
							return WMSG_SUCCESS;
						}

						// [090602: selo] Checkbutton 가지는 Child인 경우
						if(bCheckChildButton 
							&& m_aPopLine[i].pChkButton
							&& m_aPopLine[i].pChkButton->IsInside(nX, nY)
							&& m_aPopLine[i].pChkButton->IsEnabled())
						{
							if(nListEx == nRealSel)
							{
								if(m_vecSelectable[i])
								{
									if(m_aPopLine[i].pChkButton->IsChecked())
									{
										m_aPopLine[i].pChkButton->SetCheck(FALSE);
									}							
									else
									{
										m_aPopLine[i].pChkButton->SetCheck(TRUE);
									}
									m_iLastCheckedIndex = nRealSel;
									m_nSelectList = nRealSel;
								}
								else
								{
									m_nSelectList = -1;
								}
								
								return WMSG_COMMAND;
							}
						}
						
						if(nListEx == nRealSel)
						{
							if(m_vecSelectable[i])
							{
								m_nSelectList = nRealSel;
							}
							else
							{
								m_nSelectList = -1;
							}
							return WMSG_COMMAND;
						}
					}
					return WMSG_SUCCESS;
				}
			}

			if( m_bScrollBar )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			if( m_bScrollBar )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( m_bSelectList )
			{
				if( IsInside( nX, nY ) )
				{
					int	nAbsY = GetAbsPosY();
					m_nOverList = ( nY - nAbsY - m_nTextSY + m_nSelectOffsetY ) / m_nLineHeight;

					int	nRealSel = m_nOverList + m_sbScrollBar.GetScrollPos();
					if( nRealSel >= 0 && nRealSel < m_sbScrollBar.GetCurItemCount()
						&& m_vecSelectable[nRealSel] )
					{
						m_nSelectList = nRealSel;
						return WMSG_COMMAND;
					}

					return WMSG_SUCCESS;
				}
			}

			if( m_bScrollBar )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( m_bScrollBar )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;
	}

	return CUIListBox::MouseMessage(pMsg);
}
