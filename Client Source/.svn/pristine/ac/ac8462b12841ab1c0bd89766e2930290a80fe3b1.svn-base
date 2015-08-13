// MobDataDialog.cpp : implementation file
//
#include "stdafx.h"
#include "WorldEditor.h"
#include "MobDataDialog.h"
#include "ZoneFlagDialog.h"
#include "MerchantDialog.h"
#include "WEDefinition.h"
#include "resource.h"
#include <Engine/Entities/MobData.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLNUM			(5)

/////////////////////////////////////////////////////////////////////////////
// CMobDataDialog dialog


CMobDataDialog::CMobDataDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMobDataDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMobDataDialog)
	//}}AFX_DATA_INIT
}


void CMobDataDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMobDataDialog)
	DDX_Control(pDX, IDC_MOB_FIND_COMBO, m_ctlcbFindMobByType);
	DDX_Control(pDX, IDC_MOB_GRID, m_ctlMobCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMobDataDialog, CDialog)
	//{{AFX_MSG_MAP(CMobDataDialog)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_MOB_FIND_COMBO, OnSelchangeMobFindCombo)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MAKE_ZONE_FLAG, OnMakeZoneFlag)
	ON_NOTIFY(NM_DBLCLK, IDC_MOB_GRID, OnDblclkMobGrid)
	ON_NOTIFY(LVN_KEYDOWN, IDC_MOB_GRID, OnKeydownMobGrid)
	ON_BN_CLICKED(IDC_EDIT_BUTTON, OnEditButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMobDataDialog message handlers

BOOL CMobDataDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CWorldEditorDoc *pDoc	= theApp.GetDocument();
	int iMobCount = 0;	
	
	CString strFileName		= _T("");
	CString strZoneFlag		= _T("");
	
	CListCtrl* pWnd = (CListCtrl*)GetDlgItem(IDC_MOB_GRID);	
	pWnd->SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FLATSB | LVS_EX_FULLROWSELECT);

	//strFileName.Format("Mob%d.lod", WORLDNUM);	// 원본.
	strFileName.Format(_T("Data\\MobAll.lod"));
	strZoneFlag.Format(_T("Data\\ZoneFlag.lod"));
	CTString fnTemp;
	CTString strFullPath	= _fnmApplicationPath.FileDir();
	fnTemp					= strFullPath + strFileName;
	CMobData::loadEx(fnTemp);
	fnTemp					= strFullPath + strZoneFlag;
	CMobData::LoadZoneData(fnTemp);
	pDoc->m_woWorld.LoadStringNPCName();


	//-------------------------------------------------------------------
	// ComboBox 텍스트 설정.		- MOB Type
	//-------------------------------------------------------------------	
	int i;
	for(i = 0; i < TYPE_TOTAL; ++i)
	{
		m_ctlcbFindMobByType.InsertString(i, _aStrMobType[i]);
	}
	m_ctlcbFindMobByType.SetCurSel(0);

	CString aStrTitle[] = 
	{ "IDX", "Name", "LEV", "HP", "CNT"};

	int aiColWidth[] =
	{ 50, 120, 50, 70, 50};
	
	const int iColNum = 5;
	
	//--------------------------------------------------------
	// 전체 리스트 목록 출력창.
	//--------------------------------------------------------
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	
	// 컬럼 헤더를 중앙에 출력.
	lvcolumn.fmt = LVCFMT_CENTER;
	
	for(i = 0; i < iColNum; ++i)
	{
		lvcolumn.iSubItem	= i;
		lvcolumn.cx			= aiColWidth[i];
		lvcolumn.pszText	= (LPSTR)(LPCTSTR)aStrTitle[i];
		m_ctlMobCtrl.InsertColumn(i, &lvcolumn);
	}

	RefreshData();

	CString strTitle = _T("");
	strTitle.Format(_T("MOB LIST - [%s] - [%d]"), strFileName, iMobCount);
	this->SetWindowText((LPCTSTR)strTitle);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMobDataDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CMobDataDialog::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	((CMainFrame*)AfxGetMainWnd())->m_pMobDataDialog = NULL;
	delete this;
	CDialog::PostNcDestroy();
}

// 엔터키 입력 방지.
void CMobDataDialog::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
}

void CMobDataDialog::OnSelchangeMobFindCombo() 
{
	// TODO: Add your control notification handler code here
	CWorldEditorDoc *pDoc = theApp.GetDocument();
	RefreshData();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CMobDataDialog::RefreshData()
{
	CWorldEditorDoc *pDoc = theApp.GetDocument();
	m_ctlMobCtrl.DeleteAllItems();

	int i, nMax = CMobData::getsize();
	//-----------------------------------------------------------------------------
	// 타이틀 부분 설정.
	//-----------------------------------------------------------------------------
	CString strGridText = _T("");
	
	//for (;iter != eiter; ++iter)
	for (i = 0; i < nMax; ++i)
	{
		//CMobData* MD = (*iter).second;
		CMobData* MD = CMobData::getDataSeq(i);

		if (MD == NULL)
			continue;
		
		if(MD->GetMobIndex() == -1)
		{
			TRACE(_T("Cannot find Mob Data!!!\n"));
			continue;
		}

		// FIXME : 하드 코딩한 부분(맘에 안듬)
		switch(m_ctlcbFindMobByType.GetCurSel())
		{
		case TYPE_SHOPPER:
			{
				if(!(MD->IsShopper()))		continue;
				break;
			}
		case TYPE_QUEST:
			{
				if(!(MD->IsQuest()))			continue;
				break;
			}
		case TYPE_EVENT:
			{
				if(!(MD->IsEvent()))			continue;
				break;
			}
		case TYPE_WAREHOUSE:
			{
				if(!(MD->IsWareHouse()))		continue;
				break;
			}
		case TYPE_REFINER:
			{
				if(!(MD->IsRefiner()))		continue;
				break;
			}
		case TYPE_ZONEMOVING:
			{
				if(!(MD->IsZoneMoving()))	continue;
				break;
			}
		case TYPE_SKILLMASTER:
			{
				if(!(MD->IsSkillMaster()))	continue;
				break;
			}
		case TYPE_SSKILLMASTER:
			{
				if(!(MD->IsSSkillMaster()))	continue;
				break;
			}
		case TYPE_GUILD:
			{
				if(!(MD->IsGuild()))			continue;
				break;
			}
		case TYPE_GUARD:
			{
				if(!(MD->IsGuard()))			continue;
				break;
			}
		case TYPE_RESETSTAT:
			{
				if(!(MD->IsResetStat()))			continue;
				break;
			}
		case TYPE_CHANGEWEAPON:
			{
				if(!(MD->IsChangeWeapon()))			continue;
					break;
			}
		case TYPE_PRODUCE:
			{
				if(!MD->IsResource())	continue;
				break;
			}
		case TYPE_REMISSION:
			{
				if(!(MD->IsRemission()))	continue;
				break;
			}
		case TYPE_WARCASTLE:
			{
				if(!(MD->IsWarCastle()))	continue;
				break;
			}
		case TYPE_ENEMY:
			{
				if( MD->IsResource() || MD->IsSkillMaster() || MD->IsSSkillMaster() || MD->IsNPC() )
					continue;
				break;
			}
		}
		
		//--------------------------------------------------------
		// 아이템 추가하기...
		//--------------------------------------------------------
		LV_ITEM lvitem;
		strGridText.Format(_T("%d"), MD->GetMobIndex());
		lvitem.iItem	= m_ctlMobCtrl.GetItemCount();
		lvitem.mask		= LVIF_TEXT;
		lvitem.iSubItem = 0;
		lvitem.pszText	= (LPSTR)(LPCTSTR)strGridText;
		m_ctlMobCtrl.InsertItem(&lvitem);		
		
		strGridText.Format("%d", MD->GetMobIndex());
		m_ctlMobCtrl.SetItemText(lvitem.iItem, 0, strGridText);		

		strGridText.Format("%s", MD->GetName());
		m_ctlMobCtrl.SetItemText(lvitem.iItem, 1, strGridText);		

		strGridText.Format("%d", MD->GetLevel());
		m_ctlMobCtrl.SetItemText(lvitem.iItem, 2, strGridText);		

		strGridText.Format("%.2f", MD->GetHealth());
		m_ctlMobCtrl.SetItemText(lvitem.iItem, 3, strGridText);
	}
}

void CMobDataDialog::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CMenu menu;
	
	// 팝업 메뉴~!!!
	if( menu.LoadMenu(IDR_MOB_POPUP))
	{
		CMenu* pPopup = menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(
			TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_LEFTALIGN,
			point.x, point.y, this);
	}	
}

void CMobDataDialog::OnMakeZoneFlag() 
{
	// TODO: Add your command handler code here	
	CWorldEditorDoc *pDoc = theApp.GetDocument();
	CString strFilter	= _T("Lod Files(*.lod) | *.lod||");
	CString szFilename	= _T("");

	szFilename	= _T("ZoneFlag.lod");

    CFileDialog fDlg(FALSE, NULL, szFilename,
			OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_OVERWRITEPROMPT, strFilter, NULL);

	//---------------------------------------------------------------------
	// 파일에 데이터를 쓰는 부분.
	//---------------------------------------------------------------------
	CFile wFile;
	if(fDlg.DoModal() == IDOK)
    {
		szFilename = fDlg.m_ofn.lpstrFile;

		if(!wFile.Open(szFilename, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
			return;

		int nCount = 0;
		int nIndex;
		ZONEFLAGS ulZoneFlag	= 0;
		ZONEFLAGS ulExtraFlag	= 0;
		
		int i, nMax = CMobData::getsize();

		for (i = 0; i < nMax; ++i)
		{
			CMobData* MD = CMobData::getDataSeq(i);
			ulZoneFlag	= MD->GetZoneMovingFlag();
						
			if (ulZoneFlag != 0)
				nCount++;			
		}
		
		wFile.Write(&nCount, sizeof(int));		// 공간술사 카운트

		for (i = 0; i < nMax; ++i)
		{
			CMobData* MD = CMobData::getDataSeq(i);
			nIndex = MD->getindex();
			ulZoneFlag	= MD->GetZoneMovingFlag();
			ulExtraFlag	= MD->GetExtraFlag();
			
			if (ulZoneFlag != 0)
			{
				wFile.Write(&nIndex, sizeof(int));
				wFile.Write(&ulZoneFlag, sizeof(ZONEFLAGS));
				wFile.Write(&ulExtraFlag, sizeof(ZONEFLAGS));
			}			
		}
		wFile.Close();
    }
}

void CMobDataDialog::OnDblclkMobGrid(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here	
	CWorldEditorDoc *pDoc = theApp.GetDocument();

	POSITION pos = m_ctlMobCtrl.GetFirstSelectedItemPosition();
	if (pos == NULL)
		TRACE0("No items were selected!\n");
	else
	{
		int nItem		= m_ctlMobCtrl.GetNextSelectedItem(pos);
		CString strData = m_ctlMobCtrl.GetItemText(nItem, 0);		
		CString strNpcName = m_ctlMobCtrl.GetItemText(nItem, 1);
		int iindex		= atoi(strData);
		if(pDoc->m_selEntitySelection.Count() > 0)
		{
			FOREACHINDYNAMICCONTAINER(pDoc->m_selEntitySelection, CEntity, iten)
			{
				if(iten->IsEnemy())
				{
					// NOTE : 상인과 존이동 장치의 경우 카운트를 체크할 필요가 있음.
					// 프로퍼티 정보를 읽어들임.
					CDLLEntityClass *pdecDLLClass	= iten->GetClass()->ec_pdecDLLClass;
					CEntityProperty &epProperty		= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 91);		// Mob Index
						ENTITYPROPERTY( &*iten, epProperty.ep_slOffset, INDEX) = iindex;

					CEntityProperty &epPropertyName		= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_STRING, 200);	// Mob Name
					ENTITYPROPERTY( &*iten, epPropertyName.ep_slOffset, CTString)	= strNpcName;
					}
				}
			}
		}
	pDoc->UpdateAllViews( NULL);
	*pResult = 0;
}

void CMobDataDialog::OnKeydownMobGrid(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDown = (LV_KEYDOWN*)pNMHDR;
	CWorldEditorDoc *pDoc = theApp.GetDocument();
	// TODO: Add your control notification handler code here
	 
	POSITION pos = m_ctlMobCtrl.GetFirstSelectedItemPosition();
	if (pos == NULL)
		TRACE0("No items were selected!\n");
		else
		{
		int nItem		= m_ctlMobCtrl.GetNextSelectedItem(pos);
		CString strData = m_ctlMobCtrl.GetItemText(nItem, 0);		
		int iindex		= atoi(strData);

		//If user press return...
		if( pLVKeyDown->wVKey == VK_BACK && iindex != -1)
		{
			CMobData* MD = CMobData::getData(iindex);
			if(MD->IsZoneMoving())
			{
				CZoneFlagDialog Dlg;
				Dlg.m_ulZoneFlag	= MD->GetZoneMovingFlag();
				Dlg.m_ulExtraFlag	= MD->GetExtraFlag();
				if(Dlg.DoModal() == IDOK)
				{
					ZONEFLAGS ulZoneFlag	= Dlg.GetZoneFlag();
					ZONEFLAGS ulExtraFlag	= Dlg.GetExtraFlag();
					MD->SetZoneMovingFlag(ulZoneFlag);
					MD->SetExtraFlag(ulExtraFlag);
				}
			}
			else if(MD->IsShopper())
			{
				int iSellRate;
				int iBuyRate;
				if(IsValidShop(iindex, iSellRate, iBuyRate))
				{
				CMerchantDialog Dlg;
				Dlg.m_strShopperName	= MD->GetName();
				Dlg.m_iSelectedShopper	= iindex;
					Dlg.m_iSellRate			= iSellRate;
					Dlg.m_iBuyRate			= iBuyRate;
				UpdateData(FALSE);
					Dlg.DoModal();					
				}
				else
				{
					AfxMessageBox(_T("상인을 먼저 배치하십시요!"));
				}
			}
		}
	}
	pDoc->UpdateAllViews( NULL);	
	*pResult = 0;
}

BOOL CMobDataDialog::IsValidShop(const int iNpcIndex, int& iSellRate, int& iBuyRate)
{
	ASSERT(theApp.m_pDBConn != NULL && "Invalid DB Connection");
	CWorldEditorDoc *pDoc			= theApp.GetDocument();
	CString strQuery = _T("");

	// 이벤트 아이템과 퀘스트 아이템과 돈을 제외한 목록.
	strQuery.Format(_T("SELECT a_sell_rate, a_buy_rate FROM t_shop WHERE a_keeper_idx = %d"), iNpcIndex);

	CDBCmd cmd;
	cmd.Init(theApp.m_pDBConn);
	cmd.SetQuery(strQuery);
	if (!cmd.Open())
	{
		AfxMessageBox(_T("Cannot open table"));
		return FALSE;
	}
	if (!cmd.MoveFirst())		
		return FALSE;

	cmd.GetRec("a_sell_rate", iSellRate);
	cmd.GetRec("a_buy_rate", iBuyRate);
	return TRUE;
}

void CMobDataDialog::OnEditButton() 
{
	// TODO: Add your control notification handler code here
	CWorldEditorDoc *pDoc = theApp.GetDocument();

	// FIXME : OnKeydownMobGrid()와 동일한 내용임.
	POSITION pos = m_ctlMobCtrl.GetFirstSelectedItemPosition();
	if (pos == NULL)
		TRACE0("No items were selected!\n");
	else
	{
		int nItem		= m_ctlMobCtrl.GetNextSelectedItem(pos);
		CString strData = m_ctlMobCtrl.GetItemText(nItem, 0);		
		int iindex		= atoi(strData);

		if( iindex != -1)
		{
			CMobData* MD = CMobData::getData(iindex);
			if(MD->IsZoneMoving())
			{
				CZoneFlagDialog Dlg;
				Dlg.m_ulZoneFlag	= MD->GetZoneMovingFlag();
				Dlg.m_ulExtraFlag	= MD->GetExtraFlag();
				if(Dlg.DoModal() == IDOK)
				{
					ZONEFLAGS ulZoneFlag	= Dlg.GetZoneFlag();
					ZONEFLAGS ulExtraFlag	= Dlg.GetExtraFlag();
					MD->SetZoneMovingFlag(ulZoneFlag);
					MD->SetExtraFlag(ulExtraFlag);
				}
			}
			else if(MD->IsShopper())
			{
				int iSellRate;
				int iBuyRate;
				if(IsValidShop(iindex, iSellRate, iBuyRate))
				{
					CMerchantDialog Dlg;
					Dlg.m_strShopperName	= MD->GetName();
					Dlg.m_iSelectedShopper	= iindex;
					Dlg.m_iSellRate			= iSellRate;
					Dlg.m_iBuyRate			= iBuyRate;
					UpdateData(FALSE);
					Dlg.DoModal();					
				}
				else
				{
					AfxMessageBox(_T("상인을 먼저 배치하십시요!"));
				}
			}
		}
	}	
}
