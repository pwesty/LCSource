
// PatchParserToolDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "PatchParserTool.h"
#include "PatchParserToolDlg.h"
#include "afxdialogex.h"

#include <string>
#include <fstream>

#include "../PatchParser2/SLogData.h"
#include "../PatchParser2/IParser.h"
#include "../PatchParser2/IPatcher.h"
#include "../SvnController/ISvnController.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_RECENT 32


// CPatchParserToolDlg 대화 상자




CPatchParserToolDlg::CPatchParserToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPatchParserToolDlg::IDD, pParent)
	, urlMap()
	, url("")
	, filename("patch")
	, limit(10)
	, bMakeXml(false)
	, selectedRev()
	, loadedRev()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	urlMap["DEV"] = "svn://10.1.90.3/LastChaos/LastChaosClient";
	urlMap["KOR"] = "svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosKOR";
	urlMap["HKG"] = "svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosHK_EP2";
	urlMap["USA"] = "svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosUSA";
	urlMap["BRZ"] = "svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosBRZ(EP2)";
	urlMap["THA"] = "svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosThai";
	urlMap["GER"] = "svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosGER";
	urlMap["ESP"] = "svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosEspanol(Ep2)";
	urlMap["POL"] = "svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosPoland(Ep2)";
	urlMap["RUS"] = "svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosRUS";
	urlMap["TUR"] = "svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosTUR(EP2)";
	urlMap["FRA"] = "svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosFRA(Ep2)";
	urlMap["MEX"] = "svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosMEX(EP2)";
	urlMap["ITA"] = "svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosITA(EP2)";
}

void CPatchParserToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, limit);
	DDX_Check(pDX, IDC_CHECK1, bMakeXml);
	DDX_Control(pDX, IDC_LIST1, NationList);
	DDX_Control(pDX, IDC_LIST2, RevisionList);
	DDX_Control(pDX, IDC_EDIT5, LogBox);
}

BOOL CPatchParserToolDlg::PreTranslateMessage(MSG* pMsg)
{
	/// limit editbox이 포커스를 얻었을 경우
	if (GetDlgItem(IDC_EDIT3) == GetFocus())
	{
		/// EnterKey 입력시
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		{
			// refresh
			OnBnClickedButton2();
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CPatchParserToolDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPatchParserToolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPatchParserToolDlg::OnBnClickedButton2)
	ON_LBN_SELCHANGE(IDC_LIST1, &CPatchParserToolDlg::OnLbnSelchangeList1)
	ON_LBN_SELCHANGE(IDC_LIST2, &CPatchParserToolDlg::OnLbnSelchangeList2)
END_MESSAGE_MAP()


// CPatchParserToolDlg 메시지 처리기

BOOL CPatchParserToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	/// map에서 Nation읽어와서 AddString
	std::map<CString, CString>::const_iterator iter = urlMap.begin();
	while (iter != urlMap.end())
	{
		NationList.AddString(iter->first.GetString());
		++iter;
	}

	// recent 정보 로드
	loadRecent();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CPatchParserToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CPatchParserToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPatchParserToolDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/// 선택되지 않았을경우
	if (RevisionList.GetSelCount() <= 0)
		return;

	CFileDialog save(false, "*.bat", filename.GetBuffer(), OFN_OVERWRITEPROMPT, "bat Files(*.bat)|*.bat|");
	save.m_ofn.lpstrInitialDir = ".";

	if (save.DoModal() == IDOK)
	{
		UpdateData(true);

		/// Svn Command
		ISvnController* svn = getSvnController();
		std::string log = svn->log(url.GetBuffer(), true, true, selectedRev);
		std::string xml = svn->dispatchToString(log);

		/// log.Xml Parsing
		_LogDataList list;
		IParser* parser = getXmlParser();
		parser->parser(xml, list);

		/// Config
		SPatchConfig config;
		config.strSvnUrl = url;
		config.strFilePath = save.GetPathName();

		/// patch.bat Patching
		IPatcher* patcher = getBatchFilePatcher();
		patcher->patch(list, config);

		/// xml file creating
		if (bMakeXml)
		{
			std::string path = save.GetPathName();
			std::string ext = save.GetFileExt();

			path = path.substr(0, path.rfind(ext));
			path += "xml";

			std::ofstream f(path, std::ios_base::trunc);
			if (f.is_open())
			{
				f << xml;
				f.close();
			}
		}

		/// recent creating
		std::ofstream f("recent", std::ios_base::trunc);
		if (f.is_open())
		{
			filename = save.GetFileTitle();
			int size = selectedRev.size();

			f << NationList.GetCurSel() << " ";
			f << filename.GetBuffer() << " ";
			f << limit << " ";
			f << bMakeXml << " ";
			f << size << " ";

			for (int i = 0; i < size; ++i)
				f << selectedRev[i] << " ";

			f.close();
		}
	}
}

void CPatchParserToolDlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	refreshLog();
}

void CPatchParserToolDlg::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int curr = NationList.GetCurSel();

	CString nation;
	NationList.GetText(curr, nation);

	/// 선택되어있던 url 저장 후 비교
	CString prevcheck(url);
	url = urlMap[nation];

	/// refresh
	if (url != prevcheck)		// 이전과 같은 국가이면 skip
		refreshLog();
}

void CPatchParserToolDlg::OnLbnSelchangeList2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 초기화
	selectedRev.clear();

	// 선택된 revision count 읽어오기
	int count = RevisionList.GetSelCount();

	/// nothing selected
	if (count <= 0)
		return;

	CString temp;
	int rev = -1;

	/// 선택된 revision list index 읽어오기
	CArray<int> list;
	list.SetSize(count);
	RevisionList.GetSelItems(count, list.GetData());

	/// index를 통해 탐색용 list에서 읽어오기
	selectedRev.clear();
	for (int i = 0; i < count; ++i)
	{
		int index = list[i];
		selectedRev.push_back(loadedRev[index]);
	}

	/// svn log
	ISvnController* svn = getSvnController();
	std::string log = svn->log(url.GetBuffer(), false, false, selectedRev);
	std::string xml = svn->dispatchToString(log);

	/// 선택된 revision log 표시
	CString logedit(xml.c_str());
	LogBox.SetWindowText(logedit.GetBuffer());
}

void CPatchParserToolDlg::loadRecent()
{
	std::ifstream f("recent", std::ios_base::in);

	if (!f.is_open())
		return;

	int nation = -1;
	int recentRevSize = -1;
	std::string name;
	std::vector<int> rev;

	f >> nation >> name >> limit >> bMakeXml;
	f >> recentRevSize;

	for (int i = 0; i < recentRevSize; ++i)
	{
		int r = -1;
		f >> r;
		rev.push_back(r);
	}

	f.close();

	UpdateData(false);

	/// nation list 선택 -> revision list 새로고침
	NationList.SetCurSel(nation);
	OnLbnSelchangeList1();

	// filename
	filename.SetString(name.c_str());

	/// 선택되었던 revision이 있다면
	if (recentRevSize > 0)
	{
		// 선택된 revision list 저장
		selectedRev = rev;

		/// 선택되었던 revision을 현재 revision list에서 같은 revision 탐색
		int loadRevSize = loadedRev.size();
		for (int i = 0; i < loadRevSize; ++i)
		{
			for (int j = 0; j < recentRevSize; ++j)
			{
				if (loadedRev[i] == selectedRev[j])
					RevisionList.SetSel(i);			// RevisionList 선택
			}
		}

		// revision list 선택 -> log box 새로고침
		OnLbnSelchangeList2();
	}
}

void CPatchParserToolDlg::refreshLog()
{
	UpdateData(true);

	/// 초기화
	selectedRev.clear();			// 선택된 revision 초기화
	RevisionList.ResetContent();	// 표시된 revision list 초기화
	LogBox.SetWindowText("");		// log box 초기화
	loadedRev.clear();			// 탐색용 revision list 초기화

	/// 원하는 limit 만큼 읽어오기
	ISvnController* svn = getSvnController();
	std::string log = svn->log(url.GetBuffer(), limit, true);
	std::string xml = svn->dispatchToString(log);

	/// revision list parsing
	_LogDataList list;
	IParser* parser = getXmlParser();
	parser->parser(CString(xml.c_str()).GetBuffer(), list);
	
	_LogDataIterator iter = list.begin();

	while (iter != list.end())
	{
		int rev = iter->revision;
		std::string msg = iter->msg;

		// 탐색을 위한 revision list 따로 저장
		loadedRev.push_back(rev);

		/// revision list 표시
		CString add;
		add.Format("%d %s", rev, msg.c_str());
		RevisionList.AddString(add);

		++iter;
	}
}
