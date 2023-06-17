
// FBXConverterDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "FBXConverter/FBXConverter.h"
#include "FBXConverterDlg/FBXConverterDlg.h"
#include "afxdialogex.h"
#include "FBXParser/FBXParser.h"
#include "YAMLParser\YAMLParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFBXConverterDlg 대화 상자



CFBXConverterDlg::CFBXConverterDlg(CString _argv, int _argc, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FBXCONVERTER_DIALOG, pParent)
{
	argv = _argv;

	argc = _argc;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// argv가 있다면 process 생성되서 인자 넘겨주는 녀석이라고 하고
	// 함수를 실행하고 바로 끝내자
	if (_argv != "" && argc > 1)
	{
		ProcessLoadFBX(argv, argc);

		AfxGetApp()->m_pMainWnd->PostMessageW(WM_CLOSE);
	}
}

void CFBXConverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CFBXConverterDlg::ProcessLoadFBX(CString _argv, int _argc)
{
	// FBXConverter.exe lobby.fbx tuto.fbx가 전달되었다면, _argc 값은 3이 되며 
	// _argv[0]에는 FBXConverter.exe가, _argv[1]에는 lobby.fbx가, _argv[2]에는 tuto.fbx가 저장이 되있을것이다.. 아마..?
	// 라고 생각했는데 MFC는 좀 다르다. CComandLineInfo로 명령줄 인수를 처리하는데 CString으로 받을수있다.
	// lobby.fbx tuto.fbx 이런식으로 넘어온다. 
	std::string argvInfo = std::string(CT2CA(_argv));

	std::stringstream stream;

	stream.str(argvInfo);

	std::string fbxPath = "Resources/Model";

	std::vector<std::string> fbxFileNameList;

	std::string tmpStr;

    while(stream >> tmpStr)
	{
        fbxFileNameList.emplace_back(tmpStr);
    }

	for (int i = 0; i < fbxFileNameList.size(); i++)
	{
		std::shared_ptr<FBXParser> fbxParser = std::make_shared<FBXParser>();
		
		std::wstring fileName = fs::path(fbxFileNameList[i]).filename();

		std::string strFileName = std::string(CT2CA(fileName.c_str()));

		std::string tmpFbxFileName = strFileName.substr(0, strFileName.size() - 4);

		fbxParser->LoadFbx(fbxPath + '/' + fbxFileNameList[i], tmpFbxFileName);
	}

	ExitProcess(0);
	//MessageBox(L"FBXProcess Complete!", L"FBXProcess Converter", MB_OK);
}

BEGIN_MESSAGE_MAP(CFBXConverterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONVERT, &CFBXConverterDlg::OnBnClickedConvert)
	ON_BN_CLICKED(IDC_LOAD, &CFBXConverterDlg::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_SBUTTON, &CFBXConverterDlg::OnBnClickedSbutton)
	ON_BN_CLICKED(IDC_SCONVERT, &CFBXConverterDlg::OnBnClickedSconvert)
	ON_BN_CLICKED(IDC_All_CONVERT, &CFBXConverterDlg::OnBnClickedAllFBXConvert)
END_MESSAGE_MAP()


// CFBXConverterDlg 메시지 처리기

BOOL CFBXConverterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetDlgItemText(IDC_EDIT1, m_loadFilePath);
	SetDlgItemText(IDC_EDIT2, m_loadSceneFilePath);
	SetDlgItemText(IDC_EDIT3, L"2");

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CFBXConverterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CFBXConverterDlg::OnPaint()
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
HCURSOR CFBXConverterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFBXConverterDlg::OnBnClickedConvert()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 변환 버튼 눌렀을 때 일어나는 일 여기에 작성.
	if (m_loadFilePath != "")
	{
		std::shared_ptr<FBXParser> fbxParser = std::make_shared<FBXParser>();
		
		std::string strPath = std::string(CT2CA(m_loadFilePath));

		fbxParser->LoadFbx(strPath, fbxFileName);

		MessageBox(L"Convert Complete!", L"FBXConverter", MB_OK);
	}
}


void CFBXConverterDlg::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog dlg(TRUE, L"fbx", L"*.fbx", OFN_OVERWRITEPROMPT, L"FBX File(*.fbx) | *.fbx|| *.FBX||", this);

	TCHAR szPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\FBX");
	dlg.m_ofn.lpstrInitialDir = szPath;

	if(dlg.DoModal())
	{
		m_loadFilePath = dlg.GetPathName();
		SetDlgItemText(IDC_EDIT1, m_loadFilePath);

		std::wstring strName = std::wstring(m_loadFilePath.operator LPCWSTR());

		std::wstring fileName = fs::path(strName).filename();

		std::string strFileName = std::string(CT2CA(fileName.c_str()));

		fbxFileName = strFileName.substr(0, strFileName.size() - 4);
	}
}

void CFBXConverterDlg::OnBnClickedSbutton()
{
	CFileDialog dlg(TRUE, L"unity", L"*.unity", OFN_OVERWRITEPROMPT, L"UNITY File(*.unity) | *.unity|| *.UNITY||", this);

	TCHAR szPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\UNITY");
	dlg.m_ofn.lpstrInitialDir = szPath;

	if (dlg.DoModal())
	{
		m_loadSceneFilePath = dlg.GetPathName();
		SetDlgItemText(IDC_EDIT2, m_loadSceneFilePath);

		std::wstring strName = std::wstring(m_loadSceneFilePath.operator LPCWSTR());

		std::wstring fileName = fs::path(strName).filename();

		std::string strFileName = std::string(CT2CA(fileName.c_str()));

		sceneFileName = strFileName.substr(0, strFileName.size() - 6);
	}
}

void CFBXConverterDlg::OnBnClickedSconvert()
{
	if (m_loadSceneFilePath != "")
	{
		std::shared_ptr<YAMLParser> yamlParser = std::make_shared<YAMLParser>();

		std::string strPath = std::string(CT2CA(m_loadSceneFilePath));

		yamlParser->OpenFile(strPath, sceneFileName);

		MessageBox(L"Convert Complete!", L"SceneConverter", MB_OK);
	}
}

void CFBXConverterDlg::OnBnClickedAllFBXConvert()
{
	// BinaryFile에 있던 Mesh, Material, Animation을 모두 삭제
	std::string meshPath = "BinaryFile/Mesh";
	std::string materialPath = "BinaryFile/Material";
	std::string animationPath = "BinaryFile/Animation";
	std::string modelPath = "BinaryFile/Model";

	for (const auto& entry : std::filesystem::directory_iterator(meshPath))
	{
		std::filesystem::remove(entry.path());
	}

	for (const auto& entry : std::filesystem::directory_iterator(materialPath))
	{
		std::filesystem::remove(entry.path());
	}

	for (const auto& entry : std::filesystem::directory_iterator(animationPath))
	{
		std::filesystem::remove(entry.path());
	}

	for (const auto& entry : std::filesystem::directory_iterator(modelPath))
	{
		std::filesystem::remove(entry.path());
	}

	std::string fbxPath = "Resources/Model";

	std::vector<std::string> fbxFileNameList;

	// fbx 목록을 읽어옵니다.
	for (const auto& entry : std::filesystem::directory_iterator(fbxPath))
	{
		fbxFileNameList.emplace_back(entry.path().filename().string());
	}

	// process 몇개인지 가져오기
	CString strText;
	GetDlgItemText(IDC_EDIT3, strText);

	processCnt = _ttoi(strText);

	// 0은 숫자가 아닌 문자열을 int로 변환했을때 반환되는 값
	// 그리고 음수를 넣을수도 있으니까..
	if (processCnt <= 0)
	{
		// default로 2를 주자.
		processCnt = 2;
	}
	// 너무 많이 줬을경우 방지
	else if (processCnt >= 10)
	{
		processCnt = 10;
	}

	int fbxCount = fbxFileNameList.size();
	int filesPerProcess = fbxCount / processCnt;
	int remainingFiles = fbxCount % processCnt;

	std::vector<std::vector<std::string>> processFbxFileLists;
	int startIdx = 0;

	// 각 프로세스에게 할당될 파일 목록을 분할합니다.
	for (int i = 0; i < processCnt; i++)
	{
		std::vector<std::string> fbxFileList;

		int filesForThisProcess = filesPerProcess;

		if (i < remainingFiles) 
		{
			filesForThisProcess++;
		}

		int endIdx = startIdx + filesForThisProcess;

		for (int j = startIdx; j < endIdx; j++)
		{
			fbxFileList.push_back(fbxFileNameList[j]);
		}

		processFbxFileLists.push_back(fbxFileList);

		startIdx = endIdx;
	}

	// CreateProcess에서 반환된 프로세스 핸들을 저장할 벡터를 만듭니다.
	std::vector<HANDLE> processHandles;

	// 각 프로세스를 생성하고 파일 목록을 전달합니다.
	for (int i = 0; i < processCnt; i++)
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		ZeroMemory(&pi, sizeof(pi));
		si.cb = sizeof(si);

		std::wstring commandLine = L"FBXConverter.exe";

		for (const auto& file : processFbxFileLists[i])
		{
			std::wstring wstr(file.begin(), file.end());

			commandLine += L" " + wstr;
		}

		// 분배받지 못한녀석
		if (commandLine == L"")
		{
			MessageBox(L"이 프로세스는 분배 받지 못함.", L"FBXProcess Converter", MB_OK);
			continue;
		}

		// 프로세스를 생성하고 파일 목록을 전달합니다.
		// 인자로 string으로 한다음에 넘겨주면 파일을 못찾는다.(엄청 헤맴) 이유는 형변환할때 뭔가 꼬인다는데 자세히는 모르겠다 ㅠ
		// 그래서 wstring으로 했더니 됐다.
		if (!CreateProcess(NULL, (LPWSTR)commandLine.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		{
			auto error = GetLastError();
			std::cout << "Error: Failed to create process. Error code: " << error << std::endl;
		}
		else
		{
			processHandles.push_back(pi.hProcess);
			CloseHandle(pi.hThread);
		}
	}

	CProgressCtrl progressBar;
	progressBar.SubclassDlgItem(IDC_PROGRESS2, this);
	progressBar.SetRange(0, 100);
	progressBar.SetPos(0);

	int percentPerProcess = 100 / processCnt; // 하나의 프로세스가 차지하는 % 비율

	// 모든 자식 프로세스가 종료될 때까지 대기
	while (!processHandles.empty())
	{
		// 하나라도 완료되면..
		DWORD waitResult = WaitForMultipleObjects(processHandles.size(), processHandles.data(), FALSE, INFINITE);

		if (waitResult >= WAIT_OBJECT_0 && waitResult < WAIT_OBJECT_0 + processHandles.size())
		{
			// 종료된 자식 프로세스의 핸들을 벡터에서 제거
			int index = waitResult - WAIT_OBJECT_0;

			CloseHandle(processHandles[index]);

			processHandles.erase(processHandles.begin() + index);

			// 진행 상황 표시
			int completedProcessCount = processCnt - processHandles.size();
			int completedPercent = completedProcessCount * percentPerProcess;

			progressBar.SetPos(completedPercent);
		}
	}

	MessageBox(L"모든 FBX가 로드되었습니다.", L"FBXProcess Converter", MB_OK);

	ExitProcess(0);
}
