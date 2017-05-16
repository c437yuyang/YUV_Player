
// VideoPlayerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoPlayer.h"
#include "VideoPlayerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD WINAPI PlayVideo(LPVOID lpParam);


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


// CVideoPlayerDlg 对话框



CVideoPlayerDlg::CVideoPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VIDEOPLAYER_DIALOG, pParent)
	, m_strShowFrm(_T(""))
	, m_nFrmDelay(50)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bIsPaused = false;
	m_bIsStarted = false;
}

void CVideoPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LB_FRM, m_strShowFrm);
	DDX_Text(pDX, IDC_EDIT_DELAY, m_nFrmDelay);
}

BEGIN_MESSAGE_MAP(CVideoPlayerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, &CVideoPlayerDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_SUSPEND, &CVideoPlayerDlg::OnBnClickedBtnSuspend)
	ON_BN_CLICKED(IDC_BTN_STOP, &CVideoPlayerDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CVideoPlayerDlg::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_FRM_PRE, &CVideoPlayerDlg::OnBnClickedBtnFrmPre)
	ON_BN_CLICKED(IDC_BTN_FRM_NEXT, &CVideoPlayerDlg::OnBnClickedBtnFrmNext)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DELAY, &CVideoPlayerDlg::OnDeltaposSpinDelay)
END_MESSAGE_MAP()


// CVideoPlayerDlg 消息处理程序

BOOL CVideoPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// TODO: 在此添加额外的初始化代码
	GetDlgItem(IDC_PIC)->GetClientRect(&m_rectPic);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CVideoPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CVideoPlayerDlg::OnPaint()
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
HCURSOR CVideoPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVideoPlayerDlg::OnBnClickedBtnStart()
{

#ifdef PLAY_SEQ

	// TODO: 在此添加控件通知处理程序代码
	TCHAR wchPath[MAX_PATH];     //存放选择的目录路径 
	CString cStrPath;
	ZeroMemory(wchPath, sizeof(wchPath));
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = wchPath;
	bi.lpszTitle = _T("请选择需要打开的目录：");
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	if (lp && SHGetPathFromIDList(lp, wchPath))
	{
		cStrPath.Format(_T("%s"), wchPath);//转为Cstring类型
	}
	if (cStrPath.IsEmpty())
		return;

	USES_CONVERSION;
	string strPath = W2A(cStrPath.GetBuffer());

	//选择要打开的目录后，播放指定的序列
	//1.先获取所有文件名
	CCommon::GetFileName(strPath, m_vecFiles); //C++的静态成员是::的调用。。。。

	m_frmCtl.InitParams(m_vecFiles);
#endif // PLAY_SEQ

#ifdef PLAY_YUV

	CFileDialog fDlg(true); //没指定那么详细了
	if (fDlg.DoModal() == IDCANCEL)
		return;
	CString YUVFileName = fDlg.GetPathName();

	//弹出对话框选择帧大小
	CDlgYUVParams dlg;
	if (dlg.DoModal() == IDOK) 
	{
		this->m_nFrmHeight = dlg.m_nFrmHeight;
		this->m_nFrmWidth = dlg.m_nFrmWidth;
	}

	m_frmCtl.InitParams(YUVFileName, m_nFrmWidth, m_nFrmHeight);

#endif // PLAY_YUV

	if (m_bIsStarted)
		OnBnClickedBtnStop();

	HANDLE hThreadSend;			//创建独立线程发送数据
	DWORD ThreadSendID;

	m_event.SetEvent();
	hThreadSend = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PlayVideo, (LPVOID)this, 0, &ThreadSendID);
	m_bIsPaused = false;
	m_bIsStarted = true;
	CloseHandle(hThreadSend);

}


DWORD WINAPI PlayVideo(LPVOID lpParam)
{
	CVideoPlayerDlg* pThis = (CVideoPlayerDlg*)lpParam;

	while (true)
	{
		WaitForSingleObject(pThis->m_event, INFINITE);
		pThis->m_event.SetEvent();

		if (pThis->m_bIsStarted == false) //点击结束后标志置位，停止播放
			_endthreadex(0);

		Mat frm = pThis->m_frmCtl.GetNextFrame();
		pThis->OnDisplay(frm);
		pThis->m_strShowFrm.Format(_T("%d/%d"), pThis->m_frmCtl.GetFrameIdx() + 1, pThis->m_frmCtl.GetFrameCount());
		pThis->UpdateData(false);

		Sleep(pThis->m_nFrmDelay);

	}
	return 0;
}



void CVideoPlayerDlg::OnDisplay(cv::Mat &curFrame)  //负责调整大小后调用display显示到picture控件
{
	CvSize newsize;
	newsize.width = m_rectPic.Width();
	newsize.height = m_rectPic.Height();

	Mat imgResized;
	imgResized.create(newsize, CV_8UC3);
	resize(curFrame, imgResized, imgResized.size());

	IplImage iplImage(imgResized); //转换成IplImage类型

	CDC* pDC = GetDlgItem(IDC_PIC)->GetDC();
	CvvImage cvImg;
	cvImg.CopyOf(&iplImage);
	cvImg.DrawToHDC(pDC->m_hDC, &m_rectPic);
	ReleaseDC(pDC);
}


void CVideoPlayerDlg::OnBnClickedBtnSuspend()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_bIsStarted)
		return;

	CString buttonText;
	GetDlgItemTextW(IDC_BTN_SUSPEND, buttonText);

	//正在运行时
	if (buttonText.Compare(_T("暂停")) == 0)
	{
		m_event.ResetEvent();
		SetDlgItemTextW(IDC_BTN_SUSPEND, _T("继续"));
		m_bIsPaused = true;
	}
	else //暂停时
	{
		m_event.SetEvent();
		SetDlgItemTextW(IDC_BTN_SUSPEND, _T("暂停"));
		m_bIsPaused = false;
		//g_dScaleFactor = 1.0;
	}
}


void CVideoPlayerDlg::OnBnClickedBtnStop()
{
	// TODO: 在此添加控件通知处理程序代码

	if (!m_bIsStarted)
		return;
	m_bIsStarted = false;
	m_bIsPaused = true;
	m_frmCtl.QuitOps();
	SetDlgItemTextW(IDC_BTN_SUSPEND, _T("暂停"));
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_PIC);
	CRect lRect;
	pStatic->GetClientRect(&lRect);
	pStatic->GetDC()->FillSolidRect(lRect.left, lRect.top, lRect.Width(), lRect.Height(), RGB(160, 160, 160));
#ifdef VERBOSE
	cout << "点击了结束" << endl;
#endif // VERBOSE
}


void CVideoPlayerDlg::OnBnClickedBtnExit()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bIsStarted)
		OnBnClickedBtnStop();
	OnCancel();
}


void CVideoPlayerDlg::OnBnClickedBtnFrmPre()
{
	// TODO: 在此添加控件通知处理程序代码


	if (m_bIsStarted && !m_bIsPaused)
		OnBnClickedBtnSuspend();

	Mat frm = m_frmCtl.GetPreFrame();
	OnDisplay(frm);
	m_strShowFrm.Format(_T("%d/%d"), m_frmCtl.GetFrameIdx() + 1, m_frmCtl.GetFrameCount());
	UpdateData(false);

}


void CVideoPlayerDlg::OnBnClickedBtnFrmNext()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bIsStarted && !m_bIsPaused)
		OnBnClickedBtnSuspend();

	Mat frm = m_frmCtl.GetNextFrame();
	OnDisplay(frm);
	m_strShowFrm.Format(_T("%d/%d"), m_frmCtl.GetFrameIdx() + 1, m_frmCtl.GetFrameCount());
	UpdateData(false);
}


void CVideoPlayerDlg::OnDeltaposSpinDelay(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(1);
	if (pNMUpDown->iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往上的箭头
	{
		if (m_nFrmDelay < 1000)
			m_nFrmDelay += 10;
	}
	else if (pNMUpDown->iDelta == 1) // 如果此值为1, 说明点击了Spin的往下的箭头
	{
		if (m_nFrmDelay > 20)
			m_nFrmDelay -= 10;
	}
	UpdateData(false);
	*pResult = 0;
}
