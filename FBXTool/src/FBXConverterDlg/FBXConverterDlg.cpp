
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



CFBXConverterDlg::CFBXConverterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FBXCONVERTER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFBXConverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

std::vector<std::string> CFBXConverterDlg::GetFilesInDirectory(const std::string& path)
{
	std::vector<std::string> fileNameList;

	

	return fileNameList;
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

	// Material 폴더에는 Material.json 파일 하나 다시 생성
	rapidjson::Document doc;
	doc.SetObject();

	rapidjson::Value material_array(rapidjson::kArrayType);
	doc.AddMember("Materials", material_array, doc.GetAllocator());

	std::ofstream ofs{ R"(BinaryFile/Material/MaterialList.json)" };
	rapidjson::OStreamWrapper osw{ ofs };
	rapidjson::Writer<rapidjson::OStreamWrapper> writer{ osw };
	doc.Accept(writer);

	std::string fbxPath = "Resources/Model";

	std::vector<std::string> fbxFileNameList;

	for (const auto& entry : std::filesystem::directory_iterator(fbxPath))
	{
		fbxFileNameList.emplace_back(entry.path().filename().string());
	}

	for (int i = 0; i < fbxFileNameList.size(); i++)
	{
		std::shared_ptr<FBXParser> fbxParser = std::make_shared<FBXParser>();
		
		std::wstring fileName = fs::path(fbxFileNameList[i]).filename();

		std::string strFileName = std::string(CT2CA(fileName.c_str()));

		std::string tmpFbxFileName = strFileName.substr(0, strFileName.size() - 4);

		fbxParser->LoadFbx(fbxPath + '/' + fbxFileNameList[i], tmpFbxFileName);
	}

	MessageBox(L"ALL FBX ReConvert Complete!", L"ALL FBX Converter", MB_OK);
}
