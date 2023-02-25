
// FBXConverterDlg.h: 헤더 파일
//

#pragma once


// CFBXConverterDlg 대화 상자
class CFBXConverterDlg : public CDialogEx
{
// 생성입니다.
public:
	CFBXConverterDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FBXCONVERTER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

public:
	std::vector<std::string> GetFilesInDirectory(const std::string& path);

// 구현입니다.
protected:
	HICON m_hIcon;
	CString m_loadFilePath = L"None";
	CString m_loadSceneFilePath = L"None";
	std::string fbxFileName = "";
	std::string sceneFileName;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedConvert();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedSbutton();
	afx_msg void OnBnClickedSconvert();
	afx_msg void OnBnClickedAllFBXConvert();
};
