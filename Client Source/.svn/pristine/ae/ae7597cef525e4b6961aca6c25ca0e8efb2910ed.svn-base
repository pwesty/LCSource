// EnemySpawnerDialog.cpp : implementation file
//

#include "stdafx.h"
#include "worldeditor.h"
#include "EnemySpawnerDialog.h"
#include <Engine/Entities/MobData.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnemySpawnerDialog dialog


CEnemySpawnerDialog::CEnemySpawnerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEnemySpawnerDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEnemySpawnerDialog)
	m_uiTotalCount = 0;
	//}}AFX_DATA_INIT
}


void CEnemySpawnerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEnemySpawnerDialog)
	DDX_Control(pDX, IDC_SPAWNER_GRID, m_ctlSpawnerList);
	DDX_Text(pDX, IDC_TOTAL_COUNT, m_uiTotalCount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEnemySpawnerDialog, CDialog)
	//{{AFX_MSG_MAP(CEnemySpawnerDialog)
	ON_BN_CLICKED(IDC_ENEMY_EDIT, OnEnemyEdit)
	ON_BN_CLICKED(ID_NPC_DATA_DELETE, OnNpcDataDelete)
	ON_BN_CLICKED(ID_SPAWNER_RESCAN, OnSpawnerRescan)
	ON_BN_CLICKED(ID_SPAWNER_SELECT, OnSpawnerSelect)
	ON_BN_CLICKED(IDC_ENEMY_UPDATE, OnEnemyUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnemySpawnerDialog message handlers

BOOL CEnemySpawnerDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_ctlSpawnerList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FLATSB | LVS_EX_FULLROWSELECT);
	if(theApp.m_iWorldType	!= ZONE_SDUNGEON)		return TRUE;
	//-------------------------------------------------------------------
	// Grid 형태 설정.
	//-------------------------------------------------------------------
	CString aStrTitle[] = 
	{ _T("ID"), _T("Type"), _T("Name"), _T("X"), _T("Y"), _T("Z"), _T("Angle"), _T("LV"), _T("W Spd"), _T("R Spd"), _T("Attack Area"), _T("Count Total") };

	int aiColWidth[] =
	{ 50, 50, 80, 50, 50, 50, 50, 40, 50, 50, 50, 50};

	//--------------------------------------------------------
	// 전체 리스트 목록 출력창.
	//--------------------------------------------------------
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	
	// 컬럼 헤더를 중앙에 출력.
	lvcolumn.fmt = LVCFMT_CENTER;
	
	for(int i = 0; i < sizeof(aiColWidth)/sizeof(int); ++i)
	{
		lvcolumn.iSubItem	= i;
		lvcolumn.cx			= aiColWidth[i];
		lvcolumn.pszText	= (LPSTR)(LPCTSTR)aStrTitle[i];
		m_ctlSpawnerList.InsertColumn(i, &lvcolumn);
	}	
	
	OnSpawnerRescan();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEnemySpawnerDialog::OnEnemyEdit() 
{
	// TODO: Add your control notification handler code here
	CEntity			*penSpawner			= NULL;
	if(penSpawner)
	{
		CDLLEntityClass *pdecDLLClass	= penSpawner->GetClass()->ec_pdecDLLClass;
		CEntityProperty &epMobType		= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 80);		// Mob Type
		INDEX			iMobType		= ENTITYPROPERTY( &*penSpawner, epMobType.ep_slOffset, INDEX);
		
		CEntityProperty &epCountTotal	= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 8);		// Count Total
		INDEX			iCountTotal		= ENTITYPROPERTY( &*penSpawner, epCountTotal.ep_slOffset, INDEX);
	}
}

void CEnemySpawnerDialog::OnNpcDataDelete() 
{
	// TODO: Add your control notification handler code here	
}

void CEnemySpawnerDialog::OnSpawnerRescan() 
{
	// TODO: Add your control notification handler code here
	CWorldEditorDoc *pDoc	= theApp.GetDocument();
	if( (theApp.m_iWorldType != ZONE_SDUNGEON ) || !pDoc )	return;

	m_uiTotalCount			= 0;
	m_ctlSpawnerList.DeleteAllItems();

	LV_ITEM lvitem;
	CString strGridText		= _T("");

	// 전체 엔티티중에서 Enemy Spawner에 해당하는 엔티티만 얻어서, 카운트를 계산함.
	FOREACHINDYNAMICCONTAINER(pDoc->m_woWorld.wo_cenAllEntities, CEntity, iten)
	{		
		if(iten->IsEnemySpawner())
		{			
			CDLLEntityClass *pdecDLLClass		= iten->GetClass()->ec_pdecDLLClass;				
			CEntityProperty &epPropertyMobIndex	= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 80);	// Enemy Index
			INDEX			iMobIndex			= ENTITYPROPERTY( &*iten, epPropertyMobIndex.ep_slOffset, INDEX);

			if(iMobIndex > 0)
			{
				CPlacement3D	plPos				= iten->GetPlacement();
				CEntityProperty &epPropertyCount	= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 8);	// Enemy Count
				CMobData*		MD					= CMobData::getData(iMobIndex);
				if(ENTITYPROPERTY( &*iten, epPropertyMobIndex.ep_slOffset, INDEX) != 0)
				{
					const ULONG ulID= iten->en_ulID;
					strGridText.Format(_T("%ld"), ulID);
					const int iItem	= m_ctlSpawnerList.GetItemCount();
					lvitem.iItem	= iItem;
					lvitem.mask		= LVIF_TEXT;
					lvitem.iSubItem = 0;
					lvitem.pszText	= (LPSTR)(LPCTSTR)strGridText;					
					m_ctlSpawnerList.InsertItem(&lvitem);
					m_ctlSpawnerList.SetItemData(iItem, ulID);

					strGridText.Format(_T("%d"), iMobIndex);						// Mob Index
					m_ctlSpawnerList.SetItemText(lvitem.iItem, 1, strGridText);
					
					strGridText.Format(_T("%s"), MD->GetName());				// Name
					m_ctlSpawnerList.SetItemText(lvitem.iItem, 2, strGridText);
					
					strGridText.Format(_T("%.3f"), plPos.pl_PositionVector(1));		// X
					m_ctlSpawnerList.SetItemText(lvitem.iItem, 3, strGridText);

					strGridText.Format(_T("%.3f"), plPos.pl_PositionVector(2));		// Y
					m_ctlSpawnerList.SetItemText(lvitem.iItem, 4, strGridText);

					strGridText.Format(_T("%.3f"), plPos.pl_PositionVector(3));		// Z
					m_ctlSpawnerList.SetItemText(lvitem.iItem, 5, strGridText);

					strGridText.Format(_T("%.3f"), plPos.pl_OrientationAngle(1));	// Angle
					m_ctlSpawnerList.SetItemText(lvitem.iItem, 6, strGridText);

					strGridText.Format(_T("%d"), MD->GetLevel());					// Level
					m_ctlSpawnerList.SetItemText(lvitem.iItem, 7, strGridText);

					strGridText.Format(_T("%.3f"), MD->GetWalkSpeed());				// Speed(Walk)
					m_ctlSpawnerList.SetItemText(lvitem.iItem, 8, strGridText);

					strGridText.Format(_T("%.3f"), MD->GetRunSpeed());				// Speed(Run)
					m_ctlSpawnerList.SetItemText(lvitem.iItem, 9, strGridText);

					strGridText.Format(_T("%.3f"), MD->GetAttackDistance());			// Attack Area
					m_ctlSpawnerList.SetItemText(lvitem.iItem, 10, strGridText);

					const int iCount		= ENTITYPROPERTY( &*iten, epPropertyCount.ep_slOffset, INDEX);		// Count

					strGridText.Format(_T("%d"), iCount);							// Count
					m_ctlSpawnerList.SetItemText(lvitem.iItem, 11, strGridText);

					m_uiTotalCount			+= iCount;
				}
			}
		}
	}
	UpdateData(FALSE);	
}

void CEnemySpawnerDialog::OnSpawnerSelect() 
{
	// TODO: Add your control notification handler code here
	CWorldEditorDoc *pDoc	= theApp.GetDocument();
	if(!pDoc)	return;

	pDoc->m_selEntitySelection.Clear();

	POSITION posSelected	= m_ctlSpawnerList.GetFirstSelectedItemPosition();	
	if (posSelected == NULL)
		TRACE0("No items were selected!\n");
	else
	{
		while (posSelected)
		{			
			int nItem			= m_ctlSpawnerList.GetNextSelectedItem(posSelected);
			const DWORD dwData	= m_ctlSpawnerList.GetItemData(nItem);
			CEntity*penEntity	= pDoc->m_woWorld.EntityFromID(dwData);
			if(penEntity)
			{
				pDoc->m_selEntitySelection.Select(*penEntity);
			}
		}
	}
}

void CEnemySpawnerDialog::OnEnemyUpdate() 
{
	// TODO: Add your control notification handler code here
	CWorldEditorDoc *pDoc			= theApp.GetDocument();
	ASSERT(theApp.m_pDBConn != NULL && "Invalid DB Connection");
	if (AfxMessageBox(_T("현재의 리젠 정보를 모두 지우고 새로 갱신됩니다. 진행하시겠습니까?"), MB_OKCANCEL) == IDCANCEL)
		return;
	const int iWorldNum				= theApp.m_iWorldNum;
	if(DelDataFromDB(iWorldNum))
	{
		UpdateDataToDB(iWorldNum);
	}
}

BOOL CEnemySpawnerDialog::DelDataFromDB(int iWorldNum)
{	
	ASSERT(theApp.m_pDBConn != NULL && "Invalid DB Connection");
	CString strQuery = _T("");
	strQuery.Format(
		_T("DELETE FROM t_npc_regen"
		" WHERE a_zone_num=%d"),
		iWorldNum);	

	CDBCmd cmd;
	cmd.Init(theApp.m_pDBConn);
	cmd.SetQuery(strQuery);
	if (!cmd.Update())
	{
		AfxMessageBox(_T("Cannot Delete"));
		return FALSE;
	}
	return TRUE;
}

BOOL CEnemySpawnerDialog::UpdateDataToDB(int iWorldNum)
{	
	for(int i = 0 ; i < m_ctlSpawnerList.GetItemCount(); ++i)
	{
		CString strData		= _T("");
		strData				= m_ctlSpawnerList.GetItemText(i, 1);		// Index
		int iIndex			= atof(strData);

		strData				= m_ctlSpawnerList.GetItemText(i, 3);		// XPos
		float fXPos			= atof(strData);

		strData				= m_ctlSpawnerList.GetItemText(i, 4);		// YPos
		float fYPos			= atof(strData);

		strData				= m_ctlSpawnerList.GetItemText(i, 5);		// ZPos
		float fZPos			= atof(strData);

		strData				= m_ctlSpawnerList.GetItemText(i, 6);		// Angle
		float fRot			= atof(strData);

		strData				= m_ctlSpawnerList.GetItemText(i, 11);		// Count
		int iCount			= atoi(strData);

		if(!InsertDataToDB(iWorldNum, iIndex, fXPos, fYPos, fZPos, fRot, iCount))
			return FALSE;
	}
	return TRUE;
}

BOOL CEnemySpawnerDialog::InsertDataToDB(int iWorldNum, int iMobIndex, 
										 float fXPos, float fYPos, float fZPos, float fRot, int iCount)
{
	if(iCount <= 0)		return FALSE;
	if(iMobIndex < 0)	return FALSE;
	for(int i = 0; i < iCount; ++i)
	{
		CString strQuery	= _T("");	
		strQuery.Format(
			"INSERT INTO t_npc_regen"
			" (a_zone_num, a_npc_idx, a_pos_x, a_y_layer, a_pos_h, a_pos_z, a_pos_r,"
			"a_regen_sec, a_total_num)"
			
			" VALUES ("
			"%d,%d,%f,%d,%f,%f,%f,"
			"%d,%d)"
			
			,iWorldNum
			,iMobIndex
			,fXPos
			,0
			,fYPos
			,fZPos
			,fRot
			,0
			,1
			);
		
		CDBCmd cmd;
		cmd.Init(theApp.m_pDBConn);
		cmd.SetQuery(strQuery);
		if (!cmd.Update())
		{
			AfxMessageBox(_T("데이터를 입력할 수 없습니다."));
			return FALSE;
		}
	}
	return TRUE;
}
