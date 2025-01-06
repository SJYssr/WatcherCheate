
// WatcherCheaterDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "WatcherCheater.h"
#include "WatcherCheaterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CWatcherCheaterDlg 对话框



CWatcherCheaterDlg::CWatcherCheaterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WATCHERCHEATER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWatcherCheaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, PC_ICON, m_PictureCtrl);
}

BEGIN_MESSAGE_MAP(CWatcherCheaterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_EXIT, &CWatcherCheaterDlg::OnBnClickedExit)
	ON_BN_CLICKED(BTN_HIDE, &CWatcherCheaterDlg::OnBnClickedHide)
	ON_BN_CLICKED(BTN_SHOW, &CWatcherCheaterDlg::OnBnClickedShow)
END_MESSAGE_MAP()


// CWatcherCheaterDlg 消息处理程序

BOOL CWatcherCheaterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_NORMAL);

	// TODO: 在此添加额外的初始化代码
	m_PictureCtrl.SetParentHwnd(GetSafeHwnd());

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWatcherCheaterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWatcherCheaterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWatcherCheaterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CWatcherCheaterDlg::OnBnClickedExit()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnOK();
}

typedef struct _THREAD_PARAM {
	FARPROC pFunc[2]; // 存放两个函数 LoadLibraryA; Getprocaddress;
	char szBuff[2][128];
	HWND Wnd;
	DWORD dwAffinity;
}THREAD_PARAM, * PTHREAD_PARAM;

typedef HMODULE(WINAPI* PLOADLIBARAYA)(LPCSTR lpLibFileName);
typedef FARPROC(WINAPI* PGETPROCADDRESS)(HMODULE hModule, LPCSTR lpProcName);
typedef BOOL(WINAPI* PSETWINDOWDISPLAYAFFINITY)(HWND, DWORD);

static DWORD WINAPI InjectFunc(LPVOID pData) {
	/*
		牢记：在代码注入中
        1. 不能使用系统函数。
        2. 不能使用全局变量。
        2. 不能使用字符串(因为这会被当成全局函数)
    */
	PTHREAD_PARAM pParam = (PTHREAD_PARAM) pData;
	HMODULE hMod = NULL;
	FARPROC pFunc = NULL;
	hMod = ((PLOADLIBARAYA)pParam->pFunc[0])(pParam->szBuff[0]); //LoadLibraryA(kernel32.dll)
	if (!hMod) return -4;
	pFunc = (FARPROC)((PGETPROCADDRESS)pParam->pFunc[1])(hMod, pParam->szBuff[1]);
	if (!pFunc) return -5;
	return ((PSETWINDOWDISPLAYAFFINITY)pFunc)(pParam->Wnd, pParam->dwAffinity);
}
static void AfterInjectFunc(void) {
}

BOOL CWatcherCheaterDlg::ChangeAffinity(HWND Wnd, DWORD dwAffinity) {
	// get tid, pid, process handle
	DWORD PID, TID;
	TID = ::GetWindowThreadProcessId(Wnd, &PID);
	HANDLE hProcess;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, PID);
	// 在目标进程中配变量地址空间
	HMODULE hModule = GetModuleHandleA(("kernel32.dll"));
	THREAD_PARAM param = { 0, };
	param.pFunc[0] = GetProcAddress(hModule, "LoadLibraryA");
	param.pFunc[1] = GetProcAddress(hModule, "GetProcAddress");
	strcpy_s(param.szBuff[0], "user32.dll");
	strcpy_s(param.szBuff[1], "SetWindowDisplayAffinity");
	param.Wnd = Wnd;
	param.dwAffinity = dwAffinity;

	DWORD cbParamSize = sizeof(THREAD_PARAM);
	void* pDataRemote = (char*)VirtualAllocEx(hProcess, 0, cbParamSize, MEM_COMMIT,
		PAGE_READWRITE);
	//写内容到目标进程中分配的变量空间
	if (pDataRemote == NULL) {
		return -1;
	}
	::WriteProcessMemory(hProcess, pDataRemote, &param, cbParamSize, NULL);

	//在目标进程中分配代码地址空间, 计算代码大小, 分配代码地址空间
	SIZE_T cbCodeSize;
	if ((LPBYTE)InjectFunc > (LPBYTE)AfterInjectFunc)
		cbCodeSize = ((LPBYTE)InjectFunc - (LPBYTE)AfterInjectFunc);
	else 
		cbCodeSize = ((LPBYTE)AfterInjectFunc - (LPBYTE)InjectFunc);
	PDWORD pCodeRemote = (PDWORD) VirtualAllocEx(hProcess, 0, cbCodeSize, MEM_COMMIT,
		PAGE_EXECUTE_READWRITE);
	if (pCodeRemote == NULL) {
		return -2;
	}
	WriteProcessMemory(hProcess, pCodeRemote, &InjectFunc, cbCodeSize, NULL);

	//在目标进程中执行代码
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
		(LPTHREAD_START_ROUTINE)pCodeRemote,
		pDataRemote, 0, NULL);

	DWORD h = -3;
	if (hThread)
	{
		::WaitForSingleObject(hThread, INFINITE);
		::GetExitCodeThread(hThread, &h);
		TRACE("run and return %d\n", h);
		::CloseHandle(hThread);
	}
	::VirtualFreeEx(hProcess, pCodeRemote,
		0, MEM_RELEASE);

	::VirtualFreeEx(hProcess, pDataRemote,
		0, MEM_RELEASE);
	::CloseHandle(hProcess);
	return h;
}

void CWatcherCheaterDlg::OnBnClickedHide()
{
	// TODO: 在此添加控件通知处理程序代码
	TRACE("HIDE");
	BOOL resp = ChangeAffinity(m_PictureCtrl.m_ChosenWindow, WDA_EXCLUDEFROMCAPTURE);
	if (resp == FALSE) {
		MessageBox(TEXT("操作失败"), TEXT("Error"), MB_OK | MB_ICONERROR);
	}
	else if (resp != TRUE) {
		CString str;
		str.Format(_T("%d"), resp);
		MessageBox(str, TEXT("Error"), MB_OK | MB_ICONERROR);
	}
}


void CWatcherCheaterDlg::OnBnClickedShow()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL resp = ChangeAffinity(m_PictureCtrl.m_ChosenWindow, WDA_NONE);
	if (resp == FALSE) {
		MessageBox(TEXT("操作失败"), TEXT("Error"), MB_OK | MB_ICONERROR);
	}
	else if (resp != TRUE) {
		CString str;
		str.Format(_T("%d"), resp);
		MessageBox(str, TEXT("Error"), MB_OK | MB_ICONERROR);
	}
}
