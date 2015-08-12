
// PatchParserToolDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <map>
#include <vector>

// CPatchParserToolDlg 대화 상자
class CPatchParserToolDlg : public CDialogEx
{
// 생성입니다.
public:
	CPatchParserToolDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PATCHPARSERTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);		// limit editbox에서 enterkey 지원


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	std::map<CString, CString> urlMap;
	CString url;
	CString filename;
	int limit;
	BOOL bMakeXml;

	std::vector<int> selectedRev;	// 선택된 revision
	std::vector<int> loadedRev;		// 탐색용 revision list

	CListBox NationList;
	CListBox RevisionList;
	CEdit LogBox;

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();

	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnLbnSelchangeList2();

	void loadRecent();
	void refreshLog();
};
