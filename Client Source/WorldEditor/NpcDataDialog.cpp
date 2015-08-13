// NpcDataDialog.cpp : implementation file
//
// FIXME : 리젠 관련된 부분 모두 뜯어고칠것.
// FIXME : DB에 쿼리하는 부분이 너무 중복되는 부분이 많고,
// FIXME : 상점, Respawner, Mob, Npc등등 일관성이 떨어짐.

#include "stdafx.h"
#include "WorldEditor.h"
#include "resource.h"
#include "NpcDataDialog.h"
#include "WEDefinition.h"
#include "dbcmd.h"
#include <Engine/Entities/MobData.h>

#include <Engine/Entities/SmcParser.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNpcDataDialog dialog

CNpcDataDialog::CNpcDataDialog(CWnd* pParent /*=NULL*/)
: CDialog(CNpcDataDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNpcDataDialog)		
	m_bShowEnemyModel = FALSE;
	//}}AFX_DATA_INIT
}

void CNpcDataDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNpcDataDialog)
	DDX_Control(pDX, IDC_FIND_BY_YLAYER, m_ctlcbFindByYLayer);
	DDX_Control(pDX, IDC_FIND_BY_NAME, m_ctlcbFindByName);
	DDX_Control(pDX, IDC_FIND_BY_TYPE,  m_ctlcbFindByType);
	DDX_Control(pDX, IDC_NPC_GRID, m_ctlNpcGrid);
	DDX_Check(pDX, IDC_SHOW_ENEMY_MODEL, m_bShowEnemyModel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNpcDataDialog, CDialog)
//{{AFX_MSG_MAP(CNpcDataDialog)	
ON_BN_CLICKED(ID_NPC_DATA_DELETE, OnNpcDataDelete)		
ON_BN_CLICKED(IDC_ENEMY_SCAN, OnEnemyUpdate)
ON_BN_CLICKED(ID_NPC_GENERATE, OnNpcGenerate)
ON_CBN_SELCHANGE(IDC_FIND_BY_YLAYER, OnSelchangeFindByYlayer)
ON_CBN_SELCHANGE(IDC_FIND_BY_NAME, OnSelchangeFindByName)
ON_CBN_SELCHANGE(IDC_FIND_BY_TYPE, OnSelchangeFindByType)

ON_COMMAND(IDM_IMPORT_DB_DATA, OnImportDbData)
ON_COMMAND(IDM_EXPORT_DB_DATA, OnExportDbData)
ON_WM_NCDESTROY()
ON_BN_CLICKED(IDC_ENEMY_EDIT, OnEnemyEdit)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNpcDataDialog message handlers

BOOL CNpcDataDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CListCtrl* pWnd = (CListCtrl*)GetDlgItem(IDC_NPC_GRID);	
	pWnd->SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FLATSB | LVS_EX_FULLROWSELECT);

	//-------------------------------------------------------------------
	// Grid 형태 설정.
	//-------------------------------------------------------------------
	CString aStrTitle[] = 
	{ _T("IDX"), _T("NPC Type"), _T("X"), _T("Layer"), _T("Y"), _T("Z"), _T("R"), _T("Regen Sec"), _T("Total Num") };

	int aiColWidth[] =
	{ 50, 120, 80, 50, 50, 50, 80, 50, 50};

	//--------------------------------------------------------
	// 전체 리스트 목록 출력창.
	//--------------------------------------------------------
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	
	// 컬럼 헤더를 중앙에 출력.
	lvcolumn.fmt = LVCFMT_CENTER;
	
	int i;
	for (i = 0; i < 9; ++i)
	{
		lvcolumn.iSubItem	= i;
		lvcolumn.cx			= aiColWidth[i];
		lvcolumn.pszText	= (LPSTR)(LPCTSTR)aStrTitle[i];
		m_ctlNpcGrid.InsertColumn(i, &lvcolumn);
	}

	// NOTE : 에러가 발생해서 주석 처리함.
	//-------------------------------------------------------------------
	// ComboBox 텍스트 설정.		- Mob 
	//-------------------------------------------------------------------		
	CWorldEditorDoc *pDoc			= theApp.GetDocument();
	m_ctlcbFindByName.InsertString(0, _T("전체"));
	int j = 1;

	int nMax = CMobData::getsize();

	for (i = 0; i < nMax; ++i)
	{
		CMobData* MD = CMobData::getDataSeq(i);

		if(MD->GetMobIndex() <= 0)
		{
			TRACE(_T("Cannot find Mob Data!!!\n"));
			continue;
		}		
		
		m_ctlcbFindByName.InsertString(j, MD->GetName());
		m_ctlcbFindByName.SetItemData(j, MD->GetMobIndex());
		j++;
	}
	m_ctlcbFindByName.SetCurSel(0);

	//-------------------------------------------------------------------
	// ComboBox 텍스트 설정.		- YLayer
	//-------------------------------------------------------------------	
	for(i = 0; i < TYPE_TOTAL; ++i)
	{
		m_ctlcbFindByType.InsertString(i, _aStrMobType[i]);
	}
	m_ctlcbFindByType.SetCurSel(0);

	//-------------------------------------------------------------------
	// ComboBox 텍스트 설정.		- YLayer
	//-------------------------------------------------------------------	
	CString aStrYLayer[] = 
	{ _T("전체"), _T("1층"), _T("2층"), _T("3층"), _T("4층"), _T("5층")};

	for(i = 0; i < 6; i++)
	{
		m_ctlcbFindByYLayer.InsertString(i, aStrYLayer[i]);
	}
	m_ctlcbFindByYLayer.SetCurSel(0);
	RefreshNpcData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
	RefreshShopData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//-----------------------------------------------------------------------------
// Purpose: World 번호에 해당하는 데이터를 제거함.
//-----------------------------------------------------------------------------
void CNpcDataDialog::OnNpcDataDelete() 
{
	// TODO: Add your control notification handler code here	
	if (AfxMessageBox(_T("Delete?"), MB_OKCANCEL) == IDCANCEL)
		return;	

	DeleteShopData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
	DeleteNpcData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
	RefreshNpcData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
	RefreshShopData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
	return;
}

//-----------------------------------------------------------------------------
// Purpose: DeleteShopData
// Input  : iWorldNum - 
//			iNpcIndex - 
//			iYLayer - 
// Output : Returns TRUE on success, FALSE on failure.
//-----------------------------------------------------------------------------
BOOL CNpcDataDialog::DeleteShopData(int iWorldNum, int iNpcIndex, int iNpcType, int iYLayer)
{
	ASSERT(theApp.m_pDBConn != NULL && "Invalid DB Connection");
	CWorldEditorDoc *pDoc			= theApp.GetDocument();
	CString strQuery = _T("");
	
	//---------------------------------------------------------------------------------------
	strQuery.Format(
		_T("DELETE FROM t_shop"
		" WHERE a_zone_num=%d"),
		iWorldNum);
	
	if(iNpcIndex != 0)
	{
		CMobData* MD = CMobData::getData(iNpcIndex);
		if(MD->IsShopper())
		{
			CString strNpcIndex = _T("");
			strNpcIndex.Format(_T(" AND a_keeper_idx = %d"), m_ctlcbFindByName.GetItemData(iNpcIndex));
			strQuery += strNpcIndex;
		}
	}
	
	if(iYLayer != -1)
	{
		CString strYLayer = _T("");
		strYLayer.Format(_T(" AND a_y_layer = %d"), iYLayer);
		strQuery += strYLayer;
	}
	
	CDBCmd cmd;
	cmd.Init(theApp.m_pDBConn);
	cmd.SetQuery(strQuery);
	if (!cmd.Update())
	{
		AfxMessageBox(_T("상점 데이터를 삭제할수 없습니다."));
		return FALSE;
	}
	return TRUE;
}

//-----------------------------------------------------------------------------
// Purpose: DeleteNpcData
// Input  : iWorldNum - 
//			iNpcIndex - 
//			iYLayer - 
// Output : Returns TRUE on success, FALSE on failure.
//-----------------------------------------------------------------------------
BOOL CNpcDataDialog::DeleteNpcData(int iWorldNum, int iNpcIndex, int iNpcType, int iYLayer)
{
	ASSERT(theApp.m_pDBConn != NULL && "Invalid DB Connection");
	CWorldEditorDoc *pDoc			= theApp.GetDocument();
	CString strQuery = _T("");
	strQuery.Format(
		_T("DELETE t_npc_regen FROM t_npc, t_npc_regen"
		" WHERE t_npc.a_index=t_npc_regen.a_npc_idx AND t_npc_regen.a_zone_num=%d"),
		iWorldNum);	

	if(iNpcType != TYPE_WHOLE)
	{			
		CString strNpcType	= _T("");
		// FiXME : 맘에 들지 않는 부분.
		switch(iNpcType)
		{
		case TYPE_ENEMY:		// 몬스터			
			strNpcType.Format(_T(" AND ((t_npc.a_flag & (%ld | %ld | %ld | %ld | %ld)))"), 
						NPC_FIRSTATTACK, NPC_ATTACK, NPC_MOVING, NPC_MBOSS, NPC_BOSS );
			break;
		case TYPE_ZONEMOVING:	// 존이동
			strNpcType.Format(_T(" AND t_npc.a_flag & %ld"), NPC_ZONEMOVER);
			break;
		case TYPE_GUILD:		// 길드
			strNpcType.Format(_T(" AND t_npc.a_flag & %ld"), NPC_GUILD);
			break;
		case TYPE_REFINER:		// 연금술사
			strNpcType.Format(_T(" AND t_npc.a_flag & %ld"), NPC_REFINER);
			break;
		case TYPE_QUEST:		// 퀘스트
			strNpcType.Format(_T(" AND t_npc.a_flag & %ld"), NPC_QUEST);
			break;
		case TYPE_PRODUCE:		// 생산품
			strNpcType.Format(_T(" AND ((t_npc.a_flag & (%ld | %ld | %ld | %ld)))"), NPC_MINERAL, NPC_CROPS, NPC_ENERGY, NPC_ETERNAL );
			break;
		case TYPE_GUARD:		// 경비병.
			strNpcType.Format(_T(" AND t_npc.a_flag & %ld"), NPC_GUARD);
			break;
		case TYPE_EVENT:		// 이벤트
			strNpcType.Format(_T(" AND t_npc.a_flag & %ld"), NPC_EVENT);
			break;
		case TYPE_WAREHOUSE:	// 창고
			strNpcType.Format(_T(" AND t_npc.a_flag & %ld"), NPC_KEEPER);
			break;
		case TYPE_SHOPPER:		// 상인
			strNpcType.Format(_T(" AND t_npc.a_flag & %ld"), NPC_SHOPPER);
			break;
		case TYPE_RESETSTAT:	// 스탯 초기화
			strNpcType.Format(_T(" AND t_npc.a_flag & %ld"), NPC_RESETSTAT);
			break;				
		case TYPE_REMISSION:	// 면죄부
			strNpcType.Format(_T(" AND t_npc.a_flag & %ld"), NPC_REMISSION);
			break;
		case TYPE_WARCASTLE:	// 공성
			strNpcType.Format(_T(" AND t_npc.a_flag & %ld"), NPC_WARCASTLE);
			break;
		case TYPE_CHANGEWEAPON:	// 무기 교환
			strNpcType.Format(_T(" AND t_npc.a_flag & %ld"), NPC_CHANGEWEAPON);
			break;

		default:
			break;
			
		}			
		strQuery += strNpcType;
	}

	if(iNpcIndex != 0)
	{		
		CString strNpcIndex = _T("");
		strNpcIndex.Format(_T(" AND t_npc_regen.a_npc_idx = %d"), m_ctlcbFindByName.GetItemData(iNpcIndex));
		strQuery += strNpcIndex;
	}

	if(iYLayer != -1)
	{
		CString strYLayer = _T("");
		strYLayer.Format(_T(" AND t_npc_regen.a_y_layer = %d"), iYLayer);
		strQuery += strYLayer;
	}

	CDBCmd cmd;
	cmd.Init(theApp.m_pDBConn);
	cmd.SetQuery(strQuery);
	if (!cmd.Update())
	{
		AfxMessageBox(_T("리젠 정보를 삭제할수 없습니다."));
		return FALSE;
	}
	return TRUE;
}

//-----------------------------------------------------------------------------
// Purpose: 읽어들인 NPC 정보를 Grid에 출력함.
// Input  : iWorldNum - 
//			iNpcIndex - 
//			iYLayer - 
//-----------------------------------------------------------------------------
void CNpcDataDialog::RefreshNpcData(int iWorldNum, int iNpcIndex, int iNpcType, int iYLayer)
{
	// TODO: Add your control notification handler code here
	ASSERT(theApp.m_pDBConn != NULL && "Invalid DB Connection");

	try
	{
		//m_ctlNpcGrid.SetRows(1);
		// 임의적으로 추가.
		m_ctlNpcGrid.DeleteAllItems();
		
		CWorldEditorDoc *pDoc			= theApp.GetDocument();
		CEntity			*penEntity		= NULL;
		CPlacement3D	plPlacement;
		
		m_paNPCWithEntity.PopAll();
		
		CString strQuery = _T("");
		if(iNpcType == 0)		// 전체
		{
			strQuery.Format(_T("SELECT a_index npcindex, a_npc_idx, a_pos_x, a_y_layer, a_pos_h, a_pos_z, a_pos_r, a_regen_sec, a_total_num FROM t_npc_regen WHERE a_zone_num = %d"), iWorldNum);
			
			if(iNpcIndex != 0)
			{
				CString strNpcIndex = _T("");
				strNpcIndex.Format(_T(" AND a_npc_idx = %d"), m_ctlcbFindByName.GetItemData(iNpcIndex));
				strQuery += strNpcIndex;
			}
			
			if(iYLayer != -1)
			{
				int iFindLayer = iYLayer * 2;
				CString strYLayer = _T("");
				strYLayer.Format(_T(" AND a_y_layer = %d"), iFindLayer);
				strQuery += strYLayer;
			}
			
			strQuery += CString(" ORDER BY a_npc_idx, npcindex");
		}
		else
		{	
			strQuery.Format(_T("SELECT reg.a_index npcindex, a_npc_idx, a_pos_x, a_y_layer, a_pos_h, a_pos_z, a_pos_r, a_regen_sec, a_total_num, reg.* FROM t_npc npc, t_npc_regen reg WHERE npc.a_index=reg.a_npc_idx AND reg.a_zone_num = %d"), iWorldNum);
			if(iNpcType != TYPE_WHOLE)
			{			
				CString strNpcType	= _T("");
				// FiXME : 맘에 들지 않는 부분.
				switch(iNpcType)
				{
				case TYPE_ENEMY:		// 몬스터
										/*
										strNpcType.Format(_T(" AND !((npc.a_flag & (%ld | %ld | %ld | %ld | %ld | %ld | %ld | %ld | %ld | %ld)))"), 
										NPC_ZONEMOVER, NPC_REFINER, NPC_QUEST, NPC_REMISSION, NPC_GUARD, 
										NPC_KEEPER, NPC_MINERAL, NPC_CROPS, NPC_ENERGY, NPC_ETERNAL, 
										NPC_SHOPPER, NPC_RESETSTAT, NPC_REMISSION, NPC_WARCASTLE, NPC_CHANGEWEAPON );
					*/
					strNpcType.Format(_T(" AND ((npc.a_flag & (%ld | %ld | %ld | %ld | %ld)))"), 
						NPC_FIRSTATTACK, NPC_ATTACK, NPC_MOVING, NPC_MBOSS, NPC_BOSS );
					break;
				case TYPE_ZONEMOVING:	// 존이동
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_ZONEMOVER);
					break;
				case TYPE_GUILD:		// 길드
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_GUILD);
					break;
				case TYPE_REFINER:		// 연금술사
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_REFINER);
					break;
				case TYPE_QUEST:		// 퀘스트
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_QUEST);
					break;
				case TYPE_PRODUCE:		// 생산품
					strNpcType.Format(_T(" AND ((npc.a_flag & (%ld | %ld | %ld | %ld)))"), NPC_MINERAL, NPC_CROPS, NPC_ENERGY, NPC_ETERNAL );
					break;
				case TYPE_GUARD:		// 경비병.
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_GUARD);
					break;
				case TYPE_EVENT:		// 이벤트
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_EVENT);
					break;
				case TYPE_WAREHOUSE:	// 창고
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_KEEPER);
					break;
				case TYPE_SHOPPER:		// 상인
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_SHOPPER);
					break;
					/*
					case TYPE_SKILLMASTER:	// 스킬 마스터
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_SKILLMASTER);
					break;
					case TYPE_SSKILLMASTER:	// 특수 스킬마스터
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_SSKILLMASTER);
					break;
					*/
				case TYPE_RESETSTAT:	// 스탯 초기화
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_RESETSTAT);
					break;				
				case TYPE_REMISSION:	// 면죄부
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_REMISSION);
					break;
				case TYPE_WARCASTLE:	// 공성
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_WARCASTLE);
					break;
				case TYPE_CHANGEWEAPON:	// 무기 교환
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_CHANGEWEAPON);
					break;
					
				default:
					break;
					
				}			
				strQuery += strNpcType;
			}
			
			if(iNpcIndex != 0)
			{
				CString strNpcIndex = _T("");
				strNpcIndex.Format(_T(" AND a_npc_idx = %d"), m_ctlcbFindByName.GetItemData(iNpcIndex));
				strQuery += strNpcIndex;
			}
			
			if(iYLayer != -1)
			{
				int iFindLayer = iYLayer * 2;
				CString strYLayer = _T("");
				strYLayer.Format(_T(" AND reg.a_y_layer = %d"), iFindLayer);
				strQuery += strYLayer;
			}
			
			strQuery += CString(" ORDER BY a_npc_idx, npcindex");
		}
		
		CDBCmd cmd;
		cmd.Init(theApp.m_pDBConn);
		cmd.SetQuery(strQuery);
		if (!cmd.Open())
		{
			AfxMessageBox(_T("Cannot open table"));
			return;
		}
		if (!cmd.MoveFirst())		return;	
		
		//---------------------------------------------------------------------------
		// FIXME : CEntity의 en_ulID와 DB에 올라갈 데이터를 1:1 매칭 시켜서,
		// FIXME : DB의 데이터로 엔티티를 생성할수 있도록 할것.
		// FIXME : 여기에 NPCWithEntity 구조체가 작성이 되어야함.
		//---------------------------------------------------------------------------
		//m_ctlNpcGrid.SetRows(cmd.m_nrecords + 1);
		LV_ITEM lvitem;
		do
		{
			int		iIndex				= -1;				// DB 상의 인덱스.
			int		iNpcIndex			= -1;				// NPC의 인덱스.
			float	fPosX				= 0.0f;				// NPC의 위치값.
			int		iPosY				= -1;
			float	fPosZ				= 0.0f;
			float	fPosY				= 0.0f;
			float	fRot				= 0.0f;
			int		iRegenTime			= -1;				// NPC의 리젠 타임
			int		iTotalNum			= -1;				// NPC의 최대 생성 갯수.
			cmd.GetRec("npcindex",		iIndex);
			cmd.GetRec("a_npc_idx",		iNpcIndex);
			cmd.GetRec("a_pos_x",		fPosX);
			cmd.GetRec("a_y_layer",		iPosY);
			cmd.GetRec("a_pos_h",		fPosY);
			cmd.GetRec("a_pos_z",		fPosZ);		
			cmd.GetRec("a_pos_r",		fRot);
			cmd.GetRec("a_regen_sec",	iRegenTime);
			cmd.GetRec("a_total_num",	iTotalNum);
			
			if(iNpcIndex > 0)
			{			
				//-------------------------------------------------------------------
				// Grid의 값 설정.
				//-------------------------------------------------------------------
				CString strGridText = _T("");		
				
				strGridText.Format(_T("%d"), iIndex);		
				lvitem.iItem	= m_ctlNpcGrid.GetItemCount();
				lvitem.mask		= LVIF_TEXT;
				lvitem.iSubItem = 0;
				lvitem.pszText	= (LPSTR)(LPCTSTR)strGridText;
				m_ctlNpcGrid.InsertItem(&lvitem);
				
				CMobData* MD = CMobData::getData(iNpcIndex);
				strGridText.Format(_T("[%d]%s"), iNpcIndex, MD->GetName());
				m_ctlNpcGrid.SetItemText(lvitem.iItem, 1, strGridText);			
				
				NPCWithEntity &npcEntity = m_paNPCWithEntity.Push();
				npcEntity.iNpcIndex = iNpcIndex;
				npcEntity.pMobData	= MD;
				npcEntity.fPosX		= fPosX;
				npcEntity.fPosZ		= fPosZ;
				npcEntity.fPosY		= fPosY;
				npcEntity.fAngle	= fRot;
				npcEntity.iPosY		= iPosY;	
				npcEntity.iRegenTime= iRegenTime;
				npcEntity.iTotalNum	= iTotalNum;
				
				strGridText.Format(_T("%.3f"), fPosX);
				m_ctlNpcGrid.SetItemText(lvitem.iItem, 2, strGridText);
				
				int iFloor = -1;
				if(iPosY % 2 == 0)
				{
					iFloor = iPosY / 2 + 1;
				}
				
				strGridText.Format(_T("%d층"), iFloor);
				m_ctlNpcGrid.SetItemText(lvitem.iItem, 3, strGridText);		
				
				strGridText.Format(_T("%.3f"), fPosY);
				m_ctlNpcGrid.SetItemText(lvitem.iItem, 4, strGridText);
				
				strGridText.Format(_T("%.3f"), fPosZ);
				m_ctlNpcGrid.SetItemText(lvitem.iItem, 5, strGridText);
				
				strGridText.Format(_T("%.3f"), fRot);
				m_ctlNpcGrid.SetItemText(lvitem.iItem, 6, strGridText);
				
				strGridText.Format(_T("%d"), iRegenTime);
				m_ctlNpcGrid.SetItemText(lvitem.iItem, 7, strGridText);
				
				strGridText.Format(_T("%d"), iTotalNum);
				m_ctlNpcGrid.SetItemText(lvitem.iItem, 8, strGridText);
			}		
		}
		while(cmd.MoveNext());	
	}
	catch(char *strError)
	{
		AfxMessageBox(strError);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : iWorldNum - 
//			iNpcIndex - 
//			iNpcType - 
//			iYLayer - 
//-----------------------------------------------------------------------------
void CNpcDataDialog::RefreshShopData(int iWorldNum, int iNpcIndex, int iNpcType, int iYLayer)
{
	// TODO: Add your control notification handler code here
	ASSERT(theApp.m_pDBConn != NULL && "Invalid DB Connection");
	try
	{
		
		CWorldEditorDoc *pDoc			= theApp.GetDocument();
		CEntity			*penEntity		= NULL;
		CPlacement3D	plPlacement;
		
		CString strQuery = _T("");
		if(iNpcType == TYPE_WHOLE || iNpcType == TYPE_SHOPPER)		// 전체
		{
			strQuery.Format(_T("SELECT * FROM t_shop WHERE a_zone_num = %d"), iWorldNum);
			
			if(iNpcIndex != 0)
			{
				CString strNpcIndex = _T("");
				strNpcIndex.Format(_T(" AND a_keeper_idx = %d"), m_ctlcbFindByName.GetItemData(iNpcIndex));
				strQuery += strNpcIndex;
			}
			
			if(iYLayer != -1)
			{
				CString strYLayer = _T("");
				strYLayer.Format(_T(" AND a_y_layer = %d"), iYLayer);
				strQuery += strYLayer;
			}	
			CDBCmd cmd;
			cmd.Init(theApp.m_pDBConn);
			cmd.SetQuery(strQuery);
			if (!cmd.Open())
			{
				AfxMessageBox(_T("Cannot open shop table"));
				return;
			}
			if (!cmd.MoveFirst())		return;	
			
			//---------------------------------------------------------------------------
			// FIXME : CEntity의 en_ulID와 DB에 올라갈 데이터를 1:1 매칭 시켜서,
			// FIXME : DB의 데이터로 엔티티를 생성할수 있도록 할것.
			// FIXME : 여기에 NPCWithEntity 구조체가 작성이 되어야함.
			//---------------------------------------------------------------------------
			//m_ctlNpcGrid.SetRows(cmd.m_nrecords + 1);
			LV_ITEM lvitem;
			do
			{
				int		iNpcIndex			= -1;				// NPC의 인덱스.
				float	fPosX				= 0.0f;				// NPC의 위치값.
				int		iPosY				= -1;
				float	fPosZ				= 0.0f;
				float	fPosY				= 0.0f;
				float	fAngle				= 0.0f;
				cmd.GetRec("a_keeper_idx",	iNpcIndex);
				cmd.GetRec("a_pos_x",		fPosX);
				cmd.GetRec("a_y_layer",		iPosY);
				cmd.GetRec("a_pos_h",		fPosY);
				cmd.GetRec("a_pos_z",		fPosZ);
				cmd.GetRec("a_pos_r",		fAngle);
				
				if(iNpcIndex > 0)
				{				
					CString strGridText = _T("");		
					
					strGridText.Format(_T("상인"));
					lvitem.iItem	= m_ctlNpcGrid.GetItemCount();
					lvitem.mask		= LVIF_TEXT;
					lvitem.iSubItem = 0;
					lvitem.pszText	= (LPSTR)(LPCTSTR)strGridText;
					m_ctlNpcGrid.InsertItem(&lvitem);
					
					NPCWithEntity &npcEntity = m_paNPCWithEntity.Push();
					CMobData* MD		= CMobData::getData(iNpcIndex);
					npcEntity.iNpcIndex = iNpcIndex;
					npcEntity.pMobData	= MD;
					npcEntity.fPosX		= fPosX;
					npcEntity.fPosZ		= fPosZ;
					npcEntity.fPosY		= fPosY;
					npcEntity.iPosY		= iPosY;
					npcEntity.fAngle	= fAngle;
					
					strGridText.Format(_T("[%d]%s"), iNpcIndex, MD->GetName());
					m_ctlNpcGrid.SetItemText(lvitem.iItem, 1, strGridText);
					
					strGridText.Format(_T("%.3f"), fPosX);
					m_ctlNpcGrid.SetItemText(lvitem.iItem, 2, strGridText);
					
					int iFloor = -1;
					if(iPosY % 2 == 0)
					{
						iFloor = iPosY / 2 + 1;
					}
					
					strGridText.Format(_T("%d층"), iPosY);
					m_ctlNpcGrid.SetItemText(lvitem.iItem, 3, strGridText);		
					
					strGridText.Format(_T("%.3f"), fPosY);
					m_ctlNpcGrid.SetItemText(lvitem.iItem, 4, strGridText);
					
					strGridText.Format(_T("%.3f"), fPosZ);
					m_ctlNpcGrid.SetItemText(lvitem.iItem, 5, strGridText);
					
					strGridText.Format(_T("%.3f"), fAngle);
					m_ctlNpcGrid.SetItemText(lvitem.iItem, 6, strGridText);
				}
			}
			while(cmd.MoveNext());	
		}
	}
	catch(char* strError)
	{
		AfxMessageBox(strError);
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNpcDataDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here	
}

//-----------------------------------------------------------------------------
// Purpose: 엔티티 목록을 DB에 삽입합니다.
// Input  : iNpcIndex - 
//			iWorldNum -
//			pEnt - 
//			YLayer - 
//			iRegenTime - 
//			iTotalNum - 
// Output : Returns TRUE on success, FALSE on failure.
//-----------------------------------------------------------------------------
BOOL CNpcDataDialog::InsertEntityList(int iNpcIndex, int iWorldNum, CEntity* pEnt, int YLayer, int iRegenTime, int iTotalNum)
{
	CWorldEditorDoc *pDoc			= theApp.GetDocument();
	ASSERT(theApp.m_pDBConn != NULL && "Invalid DB Connection");
	ASSERT(pEnt != NULL && "Invalid Entity Pointer");
	if(iNpcIndex < 0)
	{		
		AfxMessageBox(_T("Invalid Mob Index"));
		return FALSE;
	}	
	try
	{	
		// 해당 월드의 NPC 데이터를 제거함.
		FLOAT3D vPosition	= pEnt->GetLerpedPlacement().pl_PositionVector;
		ANGLE3D vAngle		= pEnt->GetLerpedPlacement().pl_OrientationAngle;
		
		CMobData* MD = CMobData::getData(iNpcIndex);
		CString strQuery = _T("");
		CDBCmd cmd;
		cmd.Init(theApp.m_pDBConn);
		CString strError;
		
		if(MD->IsShopper())
		{
			strQuery.Format(
				"SELECT a_zone_num FROM t_shop"
				" WHERE a_keeper_idx = %d"
				,iNpcIndex
				);		
			
			cmd.SetQuery(strQuery);
			if (!cmd.Open())
			{
				AfxMessageBox(_T("t_shop 테이블에 접근할 수 없습니다."));
				return FALSE;
			}
			if(cmd.MoveFirst())
			{
				int			a_zone_num;
				cmd.GetRec("a_zone_num", a_zone_num);
				
				strError.Format("[%s]는 [%2d]번 존에 이미 존재하는 상인입니다.", MD->GetName(), a_zone_num);
				AfxMessageBox(strError);
				return FALSE;
			}
			else
			{
				// 존재하지 않으면, 새로 추가해줌.
				strQuery.Format(
					"INSERT INTO t_shop"
					"(a_keeper_idx, a_zone_num, "
					"a_name, a_sell_rate, a_buy_rate, "
					"a_pos_x, a_pos_z, a_pos_h, a_pos_r, a_y_layer)"
					" VALUES ("
					"%d, %d, \"%s\", %d, %d, %f, %f, %f, %f, %d)"
					, iNpcIndex
					, iWorldNum
					, MD->GetName()
					, 40
					, 100
					, vPosition(1)
					, vPosition(3)
					, vPosition(2) + 0.5f
					, vAngle(1)
					, YLayer
					);
			}
		}
		else
		{	
			strQuery.Format(
				"INSERT INTO t_npc_regen"
				" (a_zone_num, a_npc_idx, a_pos_x, a_y_layer, a_pos_h, a_pos_r, a_pos_z,"
				"a_regen_sec, a_total_num)"
				
				" VALUES ("
				"%d,%d,%f,%d,%f,%f,%f,"
				"%d,%d)"
				
				,iWorldNum
				,iNpcIndex
				,vPosition(1)
				,YLayer
				,vPosition(2) + 0.5f
				,vAngle(1)
				,vPosition(3)
				,iRegenTime
				,iTotalNum
				);
		}
		
		cmd.SetQuery(strQuery);
		if (!cmd.Update())
		{
			strError.Format("[%s]의 데이터를 입력할 수 없습니다.", MD->GetName());
			AfxMessageBox(strError);
			return FALSE;
		}
	}
	catch(char* strError)
	{
		AfxMessageBox(strError);
	}
	return TRUE;
}

//-----------------------------------------------------------------------------
// Purpose: 월드에서 ENEMY 엔티티들의 목록을 얻습니다.  그리고 그 목록은 DB에 저장됩니다.
// Output : Returns TRUE on success, FALSE on failure.
//-----------------------------------------------------------------------------
BOOL CNpcDataDialog::ScanEntities()
{
	CWorldEditorDoc *pDoc	= theApp.GetDocument();
	INDEX	iCount			= pDoc->m_woWorld.wo_cenAllEntities.Count();
	pDoc->m_woWorld.wo_cenEnemyHolders.Clear();

	// 월드 내에서 ENF_ENEMY 속성을 갖고 있는 엔티티를 찾아서 배열에 넣습니다.
	FOREACHINDYNAMICCONTAINER(pDoc->m_woWorld.wo_cenAllEntities, CEntity, iten) 
	{
		if(iten->IsEnemy())
		{
			/*
			if(iNpcIndex != -1)
			{
				CDLLEntityClass *pdecDLLClass	= iten->GetClass()->ec_pdecDLLClass;
				CEntityProperty &epProperty		= pdecDLLClass->dec_aepProperties[2];		// NPC Type
				INDEX iEnemyType = ENTITYPROPERTY( &*iten, epProperty.ep_slOffset, INDEX);
				if(iEnemyType != iNpcIndex)		continue;
			}
			*/
			// 발견한 엔티티를 배열에 추가합니다.
			pDoc->m_woWorld.wo_cenEnemyHolders.Add(iten);
		}		
	}

	CString strTitle = _T("");
	strTitle.Format(_T("NPC REGEN LIST - [%d/%d]"), pDoc->m_woWorld.wo_cenEnemyHolders.Count(), iCount);
	this->SetWindowText((LPCTSTR)strTitle);
	return TRUE;
}

//-----------------------------------------------------------------------------
// Purpose: 엔티티 정보를 DB에 추가함...
// Input  : pEnt - 
//-----------------------------------------------------------------------------
void CNpcDataDialog::InsertNpcData(CEntity* pEnt, int iWorldNum)
{
	ASSERT(pEnt != NULL && "Invalid Entity Pointer");
	INDEX tmpID = pEnt->en_ulID;
	CWorldEditorDoc *pDoc			= theApp.GetDocument();
	// 프로퍼티 정보를 읽어들임.
	CDLLEntityClass *pdecDLLClass	= pEnt->GetClass()->ec_pdecDLLClass;	
	CEntityProperty &epProperty		= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 91);		// NPC Type
	if(epProperty.ep_strName!=NULL && CTString(epProperty.ep_strName) != "" )
	{
		// NOTE : 프로퍼티에 값을 넣는 부분입니다.  
		// NOTE : 캐릭터의 애니메이션 정보등을 이런식으로 처리할 수 있을듯...
		// NOTE : Enemy Holder를 생성한 후에, 몬스터의 타입을 지정하는 식???
		// NOTE : 아래 EnemyType는 1부터 시작해야 합니다.
		INDEX iEnemyType = ENTITYPROPERTY( &*pEnt, epProperty.ep_slOffset, INDEX);
		if(iEnemyType == 0)
			return;
		
		CEntityProperty &epRegenTime	= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 21);		// Regen Time
		INDEX iRegenTime = ENTITYPROPERTY( &*pEnt, epRegenTime.ep_slOffset, INDEX);
		
		CEntityProperty &epTotalCount	= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 150);		// Total Count
		INDEX iTotalCount= ENTITYPROPERTY( &*pEnt, epTotalCount.ep_slOffset, INDEX);

		// FallDownToFloor의 일부 코드.
		CPlacement3D plPlacement = pEnt->GetPlacement();
		plPlacement.pl_PositionVector(2) += 0.2f;
		FLOAT3D vRay[4];	
		FLOATaabbox3D box;
		pEnt->GetModelInstance()->GetCurrentColisionBox(box);	
		
		FLOAT3D vCenterUp	= box.Center();
		vCenterUp(2)		= box.Max()(2);
		vRay[0]				= vCenterUp;
		vRay[1]				= vCenterUp;
		vRay[2]				= vCenterUp;
		vRay[3]				= vCenterUp;
		
		FLOAT fMaxY			= -9999999.0f;
		CEntity* penHit		= NULL;
		BOOL bFloorHitted	= FALSE;
		for( INDEX iRay=0; iRay<4; iRay++)
		{
			FLOAT3D vSource = plPlacement.pl_PositionVector+vRay[iRay];
			FLOAT3D vTarget = vSource;
			vTarget(2)		-= (1000.0f - (iRay * 200)); // Ray길이를 점점 짧게
			CCastRay crRay( pEnt, vSource, vTarget);
			crRay.cr_ttHitModels			= CCastRay::TT_NONE; // CCastRay::TT_FULLSEETHROUGH;
			crRay.cr_bHitTranslucentPortals = TRUE;
			crRay.cr_bPhysical				= TRUE;
			pDoc->m_woWorld.CastRay(crRay);
			if( (crRay.cr_penHit != NULL) && (crRay.cr_vHit(2) > fMaxY)) 
			{
				penHit			= crRay.cr_penHit;
				fMaxY			= crRay.cr_vHit(2);
				bFloorHitted	= TRUE;
			}
		}
		if(bFloorHitted)
		{
			if(penHit->en_RenderType == CEntity::RT_TERRAIN)
			{
				InsertEntityList(iEnemyType, iWorldNum, pEnt, 0, iRegenTime, iTotalCount);
			}
			else
			{
				FLOAT3D			vPoint;
				FLOATplane3D	plPlane;
				FLOAT			fDistanceToEdge;
				CBrushPolygon	*pBrushPolygon = NULL;
				pBrushPolygon	= pEnt->GetNearestPolygon(vPoint, plPlane, fDistanceToEdge);

				// WSS_DRATAN_SEIGEWARFARE 2007/08/30
				// 필드 싱글 던전을 나눌 필요가 없다...
/*				if(pBrushPolygon == NULL || 
					theApp.m_iWorldType == ZONE_FIELD || 
					theApp.m_iWorldType == ZONE_SDUNGEON)
				{
					InsertEntityList(iEnemyType, iWorldNum, pEnt, 0, iRegenTime, iTotalCount);
				}
				else*/
				{
					UBYTE ubAttribute = pBrushPolygon->bpo_ubPolygonAttribute;

					// Unwalkable
					if((ubAttribute >= BPOA_UNWALKABLE1F && ubAttribute <= BPOA_UNWALKABLE5F) ||
						(ubAttribute >= BPOA_STAIRWALL1F2F && ubAttribute <= BPOA_STAIRWALL4F5F) ||
						(ubAttribute >= BPOA_STAIR1F2F && ubAttribute <= BPOA_STAIR4F5F) )
						
					{
						ULONG ulID = pEnt->en_ulID;
						CString strMsg = _T("");
						strMsg.Format(_T("[%ld] 에 해당하는 엔티티가 올바르지 않는 이동할수 없는 곳에 위치하고 있습니다."), ulID);
						//AfxMessageBox(strMsg, MB_OKCANCEL);
						AfxMessageBox(strMsg);
					}
					// Walkable
					else
					{
						// on the floor
						if((ubAttribute >= BPOA_WALKABLE1F && ubAttribute <= BPOA_WALKABLE5F))
						{
							InsertEntityList(iEnemyType, iWorldNum, pEnt, ((ubAttribute-1) % 10) * 2, iRegenTime, iTotalCount);
						}
						// on the terrain?
						else
						{
							InsertEntityList(iEnemyType, iWorldNum, pEnt, 0, iRegenTime, iTotalCount);
						}
					}
				}
			}
		}		
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNpcDataDialog::OnEnemyUpdate()
{
	// TODO: Add your control notification handler code here
	CWorldEditorDoc *pDoc			= theApp.GetDocument();
	if (AfxMessageBox(_T("현재의 리젠 정보를 모두 지우고 새로 갱신됩니다. 진행하시겠습니까?"), MB_OKCANCEL) == IDCANCEL)
		return;	
	ScanEntities();
	
	if(pDoc->m_woWorld.wo_cenEnemyHolders.Count() > 0)
	{
		DeleteShopData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
		DeleteNpcData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
		FOREACHINDYNAMICCONTAINER(pDoc->m_woWorld.wo_cenEnemyHolders, CEntity, itent) 
		{
			InsertNpcData(itent, theApp.m_iWorldNum);
		}
		RefreshNpcData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
		RefreshShopData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
	}
	else
	{
		AfxMessageBox(_T("월드상에 NPC가 한마리도 없습니다.  확인 후 재시도 하십시오."), MB_OK);
	}	
}

//-----------------------------------------------------------------------------
// Purpose: 
// 엔티티를 위치값에 따라서 생성해 주는 부분.
// 나중에 전역으로 빼둘것.
// Input  : MD - 
//			XPos - 
//			YLayer - 
//			ZPos - 
// Output : CEntity*
//-----------------------------------------------------------------------------
CEntity* CNpcDataDialog::CreateEntityByYLayer(CMobData& MD, float XPos, int YLayer, float YPos, float ZPos, float fAngle, int iRegenTime, int iTotalNum)
{
	// 일정한 높이에서 Ray를 쏴서...
	// 일정한 높이값을 서버에서 보내줍니다.
	// 서버에서 받은 값에 얼마를 더해서 처리해야 하는지는 미정...
	// 이 경우에, 월드 에디터
	// FallDownToFloor()
	//CCastRay crRay;
	//crRay.Init();

	CWorldEditorDoc *pDoc			= theApp.GetDocument();

	CPlacement3D	plPosition;
	//plPosition.pl_PositionVector	= FLOAT3D(XPos, YLayer, ZPos);		// 원본.
	plPosition.pl_PositionVector	= FLOAT3D(XPos, YPos, ZPos);		// 원본.
	plPosition.pl_OrientationAngle	= ANGLE3D(fAngle, 0.0f, 0.0f);

	UpdateData(TRUE);
	CEntity			*penEntity		= NULL;
	penEntity						= pDoc->m_woWorld.CreateEntity_t(plPosition, CLASS_ENEMY);

	CMobData::SetMobDataToNPC( penEntity, &MD, MD.GetName() );

	if(m_bShowEnemyModel && MD.GetMobIndex() != -1)
	{
		//CMobData::SetMobDataToNPC(penEntity, MD);

		//-----------------------------------------------------------------------------
		penEntity->InitAsSkaModel();
		penEntity->SetSkaModel(MD.GetMobSmcFileName());
	}

	// Set Regen Time
	CDLLEntityClass *pdecDLLClass	= penEntity->GetClass()->ec_pdecDLLClass;	
	CEntityProperty &epRegenTime	= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 21);		// Regen Time
	ENTITYPROPERTY( &*penEntity, epRegenTime.ep_slOffset, INDEX) = iRegenTime;	

	CEntityProperty &epTotalCount	= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 150);		// Total Count
	ENTITYPROPERTY( &*penEntity, epTotalCount.ep_slOffset, INDEX) = iTotalNum;		
	return penEntity;
}

void CNpcDataDialog::OnOK() 
{
	// TODO: Add extra validation here	
	//CDialog::OnOK();
}

//-----------------------------------------------------------------------------
// Purpose: 모든 ENEMY 엔티티들을 제거함.
//-----------------------------------------------------------------------------
void CNpcDataDialog::DestroyEntities()
{
	CWorldEditorDoc *pDoc	= theApp.GetDocument();
	
	// 선택된 열을 얻음.
	//int iStartRow			= m_ctlNpcGrid.GetRow();
	//int iEndRow			= m_ctlNpcGrid.GetRowSel();

	pDoc->m_selEntitySelection.Clear();
	FOREACHINDYNAMICCONTAINER(pDoc->m_woWorld.wo_cenEnemyHolders, CEntity, iten)
	{
		iten->Destroy();
	}
	pDoc->UpdateAllViews( NULL);
}

void CNpcDataDialog::OnNpcGenerate() 
{
	// TODO: Add your control notification handler code here
	CWorldEditorDoc *pDoc	= theApp.GetDocument();	
	if (AfxMessageBox(_T("월드 상의 모든 ENEMY 엔티티가 지워진 후에 다시 생성됩니다.  진행하시겠습니까?"), MB_OKCANCEL) == IDCANCEL)	
		return;	
	
	// 월드상의 모든 ENEMY를 스캔합니다.
	ScanEntities();
	DestroyEntities();

	// NOTE : 문제가 생기면 다음 줄의 주석을 제거해볼것.
	RefreshNpcData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
	RefreshShopData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
	
	// 해당 엔티티를 생성함.
	for(INDEX iEntity = 0; iEntity < m_paNPCWithEntity.Count(); iEntity++)
	{		
		NPCWithEntity &npcEntity	= m_paNPCWithEntity[iEntity];
		if(npcEntity.pMobData != NULL)
		{
			CEntity			*pEntity = NULL;
			if(theApp.m_iWorldType == ZONE_FIELD)
			{
				pEntity = CreateEntityByYLayer(*npcEntity.pMobData, npcEntity.fPosX, npcEntity.iPosY, 200.0f, npcEntity.fPosZ, npcEntity.fAngle, npcEntity.iRegenTime, npcEntity.iTotalNum);
			}			
			else
			{
				pEntity = CreateEntityByYLayer(*npcEntity.pMobData, npcEntity.fPosX, npcEntity.iPosY, npcEntity.fPosY, npcEntity.fPosZ, npcEntity.fAngle, npcEntity.iRegenTime, npcEntity.iTotalNum);
			}
			// 아래 함수를 외부로 빼둘것...
			//CEntity			*pEntity	= CreateEntityByYLayer(*npcEntity.pMobData, npcEntity.fPosX, npcEntity.iPosY, npcEntity.fPosY, npcEntity.fPosZ);
			//((CEnemy*)pEntity)->GetName();
			pEntity->Initialize();
			pEntity->FallDownToFloor();
		}
	}
}

void CNpcDataDialog::OnSelchangeFindByYlayer() 
{
	// TODO: Add your control notification handler code here
	RefreshNpcData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
	RefreshShopData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
}

void CNpcDataDialog::OnSelchangeFindByName() 
{
	// TODO: Add your control notification handler code here
	RefreshNpcData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
	RefreshShopData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
}

void CNpcDataDialog::OnSelchangeFindByType()
{
	// TODO: Add your control notification handler code here
	RefreshNpcData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
	RefreshShopData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
}

void CNpcDataDialog::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	CMenu menu;
	
	// 팝업 메뉴~!!!
	if( menu.LoadMenu(IDR_DB_DATA_POPUP))
	{
		CMenu* pPopup = menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(
			TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_LEFTALIGN,
			point.x, point.y, this);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNpcDataDialog::OnExportDbData() 
{
	if (AfxMessageBox(_T("상점의 판매 아이템 정보는 백업되지 않습니다.  계속 하시겠습니까?"), MB_OKCANCEL) == IDCANCEL)
		return;	

	CString strQuery	= _T("");
	strQuery.Format(
			"SELECT a_npc_idx, a_pos_x, a_pos_z, a_pos_h, a_pos_r, a_y_layer, a_regen_sec, a_total_num"
			" FROM t_npc_regen WHERE a_zone_num = %d"
			" ORDER by a_npc_idx", theApp.m_iWorldNum);
	
	CDBCmd cmd;
	cmd.Init(theApp.m_pDBConn);
	cmd.SetQuery(strQuery);

	if (!cmd.Open())
	{
		AfxMessageBox("Cannot open regen table");
		return;
	}
	if (!cmd.MoveFirst())
	{
		AfxMessageBox("Not found!!!");
		return;
	}

	//ByteQueueData	tmpQueBuffer;
	CString strFilter	= _T("DB Data Files(*.sav) | *.sav||");
	CString szFileName	= _T("");
	szFileName.Format(_T("NpcRegen%d.sav"), theApp.m_iWorldNum);
	CFileDialog fDlg(FALSE, NULL, szFileName,
			OFN_EXPLORER | OFN_LONGNAMES | OFN_OVERWRITEPROMPT, strFilter, NULL);

	CFile wFile;
	if(fDlg.DoModal() == IDOK)
    {
		szFileName = fDlg.m_ofn.lpstrFile;
		
		if(!wFile.Open(szFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
			return;
		
		int iMobCount			= cmd.m_nrecords;			// Mob의 갯수.
		
		int i					= 0;
		wFile.Write(&theApp.m_iWorldNum, sizeof(int));		// 월드 번호.
		wFile.Write(&iMobCount, sizeof(int));				// Mob의 갯수.

/*		CString tmpflush;
		tmpflush.Format("World:%d Mob Count:%d",theApp.m_iWorldNum, iMobCount);
		tmpQueBuffer.WriteString(tmpflush.GetBuffer(0));
		tmpflush.Format("npcIndex,Posx,Posz,Posh,Rotation,yLayer,regen_time,totalNum");
		tmpQueBuffer.WriteString(tmpflush.GetBuffer(0));*/
		do
		{
			int			a_npc_idx;
			float		a_pos_x;
			float		a_pos_z;
			float		a_pos_h;
			float		a_pos_r;
			int			a_y_layer;
			int			a_regen_sec; 
			int			a_total_num;

			cmd.GetRec("a_npc_idx",		a_npc_idx);
			cmd.GetRec("a_pos_x",		a_pos_x);
			cmd.GetRec("a_pos_z",		a_pos_z);
			cmd.GetRec("a_pos_h",		a_pos_h);
			cmd.GetRec("a_pos_r",		a_pos_r);
			cmd.GetRec("a_y_layer",		a_y_layer);
			cmd.GetRec("a_regen_sec",	a_regen_sec);
			cmd.GetRec("a_total_num",	a_total_num);

			wFile.Write(&a_npc_idx,		sizeof(int));
			wFile.Write(&a_pos_x,		sizeof(float));
			wFile.Write(&a_pos_z,		sizeof(float));
			wFile.Write(&a_pos_h,		sizeof(float));
			wFile.Write(&a_pos_r,		sizeof(float));
			wFile.Write(&a_y_layer,		sizeof(int));
			wFile.Write(&a_regen_sec,	sizeof(int));
			wFile.Write(&a_total_num,	sizeof(int));
			
			/*CString strFlush;
			strFlush.Format(_T("%n,%d,%s,%f,%f,%f,%d\n"),*/

			i++;
		} while (cmd.MoveNext());

		//----------------------------------------------------------------------
		// 상인 정보 화일에 쓰기.

		int iNumOfShopper = 0;
		strQuery.Format(
			"SELECT a_keeper_idx, a_name, a_sell_rate, a_buy_rate, a_pos_x, a_pos_z, a_pos_h, a_pos_r, a_y_layer"
			" FROM t_shop WHERE a_zone_num = %d"
			" ORDER by a_keeper_idx", theApp.m_iWorldNum);
		
		cmd.SetQuery(strQuery);
		
		if (!cmd.Open())
		{
			AfxMessageBox("Cannot open shop table");
			wFile.Close();
			return;
		}

		iNumOfShopper		= cmd.m_nrecords;			// Shopper의 갯수.
		wFile.Write(&iNumOfShopper,		sizeof(int));

		if (!cmd.MoveFirst())
		{			
			wFile.Close();
			return;
		}

		do
		{
			int			a_keeper_idx;
			CString		strShopName;
			float		a_sell_rate;
			float		a_buy_rate;
			float		a_pos_x;
			float		a_pos_z;
			float		a_pos_h; 
			float		a_pos_r;
			int			a_y_layer;
			int			len;

			cmd.GetRec("a_keeper_idx",	a_keeper_idx);			
			strShopName.Format(_T("%s"), cmd.GetRec("a_name"));
			cmd.GetRec("a_sell_rate",	a_sell_rate);
			cmd.GetRec("a_buy_rate",	a_buy_rate);
			cmd.GetRec("a_pos_x",		a_pos_x);
			cmd.GetRec("a_pos_z",		a_pos_z);
			cmd.GetRec("a_pos_h",		a_pos_h);
			cmd.GetRec("a_pos_r",		a_pos_r);
			cmd.GetRec("a_y_layer",		a_y_layer);

			len = strShopName.GetLength();
			wFile.Write(&a_keeper_idx,	sizeof(int));	
			wFile.Write(&len,			sizeof(int));
			wFile.Write(strShopName,	len);
			wFile.Write(&a_sell_rate,	sizeof(float));
			wFile.Write(&a_buy_rate,	sizeof(float));
			wFile.Write(&a_pos_x,		sizeof(float));
			wFile.Write(&a_pos_z,		sizeof(float));
			wFile.Write(&a_pos_h,		sizeof(float));
			wFile.Write(&a_pos_r,		sizeof(float));
			wFile.Write(&a_y_layer,		sizeof(int));
		} while (cmd.MoveNext());
		wFile.Close();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNpcDataDialog::OnImportDbData()
{
	if (AfxMessageBox(_T("데이터를 복구할 경우 기존의 데이터에 추가됩니다.  이전 데이터를 지우시고 실행하십시요.  계속 하시겠습니까?"), MB_OKCANCEL) == IDCANCEL)
		return;	
	CDBCmd cmd;
	cmd.Init(theApp.m_pDBConn);
	CString strQuery	= _T("");
	CString strFilter	= _T("DB Data Files(*.sav) | *.sav||");
	CString szFileName	= _T("");
	szFileName.Format(_T("NpcRegen%d.sav"), theApp.m_iWorldNum);
	CFileDialog fDlg(TRUE, NULL, szFileName,
			OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_OVERWRITEPROMPT, strFilter, NULL);
	
	CFile wFile;
	if(fDlg.DoModal() == IDOK)
    {
		szFileName = fDlg.m_ofn.lpstrFile;
		
		if(!wFile.Open(szFileName, CFile::modeRead | CFile::typeBinary))
			return;
		
		// DB를 지우고 다시 쓴다.(npc regen 정보)
		strQuery.Format("delete from t_npc_regen where a_zone_num = %d", theApp.m_iWorldNum);
		cmd.SetQuery(strQuery);
		if (!cmd.Update())
		{
			AfxMessageBox(_T("Cannot npc_regen delete"));
			wFile.Close();
			return;
		}

		int iMobCount		= 0;
		int iWorldNum		= 0;

		wFile.Read(&iWorldNum, sizeof(int));			// 월드 번호.
		wFile.Read(&iMobCount, sizeof(int));			// Mob의 갯수.

		int i;

		for (i = 0; i < iMobCount; ++i)
		{
			int			a_npc_idx;
			float		a_pos_x;
			float		a_pos_z;
			float		a_pos_h;
			float		a_pos_r;
			int			a_y_layer;
			int			a_regen_sec; 
			int			a_total_num;

			wFile.Read(&a_npc_idx,		sizeof(int));
			wFile.Read(&a_pos_x,		sizeof(float));
			wFile.Read(&a_pos_z,		sizeof(float));
			wFile.Read(&a_pos_h,		sizeof(float));
			wFile.Read(&a_pos_r,		sizeof(float));
			wFile.Read(&a_y_layer,		sizeof(int));
			wFile.Read(&a_regen_sec,	sizeof(int));
			wFile.Read(&a_total_num,	sizeof(int));
			
			if(a_npc_idx < 0)	continue;
			
			strQuery.Format(
				"INSERT INTO t_npc_regen"
				" (a_zone_num, a_npc_idx, a_pos_x, a_pos_z, a_pos_r, a_pos_h, a_y_layer,"
				"a_regen_sec, a_total_num)"
				
				" VALUES ("
				"%d,%d,%f,%f,%f,%f,%d,"
				"%d,%d)"
				
				,theApp.m_iWorldNum
				,a_npc_idx
				,a_pos_x
				,a_pos_z
				,a_pos_r
				,a_pos_h
				,a_y_layer
				,a_regen_sec
				,a_total_num
				);

			cmd.SetQuery(strQuery);
			if (!cmd.Update())
			{
				AfxMessageBox(_T("Cannot Insert"));
				wFile.Close();
				return;
			}
		}
		
		// DB를 지우고 다시 쓴다.(shop 정보)
		strQuery.Format("delete from t_shop where a_zone_num = %d", theApp.m_iWorldNum);
		cmd.SetQuery(strQuery);
		if (!cmd.Update())
		{
			AfxMessageBox(_T("Cannot t_shop delete"));
			wFile.Close();
			return;
		}

		int iNumOfShopper;
		wFile.Read(&iNumOfShopper,		sizeof(int));
		for (i = 0; i < iNumOfShopper; ++i)
		{
			int			a_keeper_idx;
			char		a_name[50] = {0};
			float		a_sell_rate;
			float		a_buy_rate;
			float		a_pos_x;
			float		a_pos_z;
			float		a_pos_h; 
			float		a_pos_r;
			int			a_y_layer;
			int			len;

			wFile.Read(&a_keeper_idx,	sizeof(int));
			wFile.Read(&len,			sizeof(int));
			wFile.Read(&a_name,			len);
			wFile.Read(&a_sell_rate,	sizeof(float));
			wFile.Read(&a_buy_rate,		sizeof(float));
			wFile.Read(&a_pos_x,		sizeof(float));
			wFile.Read(&a_pos_z,		sizeof(float));
			wFile.Read(&a_pos_h,		sizeof(float));
			wFile.Read(&a_pos_r,		sizeof(float));
			wFile.Read(&a_y_layer,		sizeof(int));

			if(a_keeper_idx < 0)	continue;

			strQuery.Format(
				"INSERT INTO t_shop"
				" (a_keeper_idx, a_name, a_sell_rate, a_buy_rate, a_pos_x, a_pos_z, a_pos_r, a_pos_h, a_y_layer,"
				"a_zone_num)"
				
				" VALUES ("
				"%d,\"%s\",%f,%f,%f,%f,%f,%f,%d,"
				"%d)"				
				
				,a_keeper_idx
				,a_name
				,a_sell_rate
				,a_buy_rate
				,a_pos_x
				,a_pos_z
				,a_pos_r
				,a_pos_h
				,a_y_layer
				,theApp.m_iWorldNum
				);
			cmd.SetQuery(strQuery);
			if (!cmd.Update())
			{
				AfxMessageBox(_T("Cannot Insert"));
				wFile.Close();
				return;
			}
	}
	wFile.Close();
	}
}

void CNpcDataDialog::BackupData()
{
	OnImportDbData();
}

void CNpcDataDialog::RestoreData()
{
	OnExportDbData();
}

void CNpcDataDialog::OnEnemyEdit() 
{
	// TODO: Add your control notification handler code here
	
}

void CNpcDataDialog::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_paNPCWithEntity.PopAll();
	
	CDialog::PostNcDestroy();
}
