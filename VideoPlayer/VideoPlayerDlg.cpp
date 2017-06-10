
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
#pragma region AutoFit
	m_ptMinTrackSize.x = 0;
	m_ptMinTrackSize.y = 0;
#pragma endregion
}

#pragma region AutoFit
CVideoPlayerDlg::~CVideoPlayerDlg()
{
	FreeCtrlInfoList();
}

void CVideoPlayerDlg::SetMinSize(int nWidth, int nHeight)
{
	ASSERT(nWidth > 0);
	ASSERT(nHeight > 0);

	//设置窗口最小值
	m_ptMinTrackSize.x = nWidth;
	m_ptMinTrackSize.y = nHeight;
}

void CVideoPlayerDlg::FreeCtrlInfoList()
{
	INT_PTR	nCount = m_listCtrlInfo.GetSize();

	for (int i = 0; i < nCount; i++)
	{
		lpControlInfo pCtrlInfo = m_listCtrlInfo.ElementAt(i);
		delete pCtrlInfo;
	}

	m_listCtrlInfo.RemoveAll();
}

void CVideoPlayerDlg::MakeCtrlFit(CWnd* pWnd, int nMoveXPercent, int nMoveYPercent, int nZoomXPercent, int nZoomYPercent)
{
	ASSERT(pWnd);									//指针是否为空
	ASSERT(nMoveXPercent >= 0 && nMoveXPercent <= 100);	//nMoveXPercent值是否有效
	ASSERT(nMoveYPercent >= 0 && nMoveYPercent <= 100);	//nMoveXPercent值是否有效
	ASSERT(nZoomXPercent >= 0 && nZoomXPercent <= 100);	//nMoveXPercent值是否有效
	ASSERT(nZoomYPercent >= 0 && nZoomYPercent <= 100);	//nMoveXPercent值是否有效

	lpControlInfo	pCtrlInfo = new ControlInfo;	//创建结构指针

													//填充变量
	pCtrlInfo->m_pWnd = pWnd;
	pCtrlInfo->m_nMoveXPercent = nMoveXPercent;
	pCtrlInfo->m_nMoveYPercent = nMoveYPercent;
	pCtrlInfo->m_nZoomXPercent = nZoomXPercent;
	pCtrlInfo->m_nZoomYPercent = nZoomYPercent;

	pWnd->GetWindowRect(pCtrlInfo->m_rectWnd);
	ScreenToClient(&pCtrlInfo->m_rectWnd);

	m_listCtrlInfo.Add(pCtrlInfo);	//加入维护列表
}

void CVideoPlayerDlg::CancelCtrlFit(HWND hWnd)
{
	INT_PTR	nCount = m_listCtrlInfo.GetSize();

	for (int i = 0; i < nCount; i++)
	{
		lpControlInfo pCtrlInfo = m_listCtrlInfo.ElementAt(i);
		if (pCtrlInfo->m_pWnd->GetSafeHwnd() == hWnd)
		{
			delete pCtrlInfo;
			m_listCtrlInfo.RemoveAt(i);

			break;
		}
	}



}

void CVideoPlayerDlg::OnSize(UINT nType, int cx, int cy)
{
	//计算窗口宽度和高度的改变量
	int nIncrementX = cx - m_nWinWidth;
	int nIncrementY = cy - m_nWinHeight;

	INT_PTR	nCount = m_listCtrlInfo.GetSize();

	UINT	uFlags = SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOCOPYBITS;
	for (int i = 0; i < nCount; i++)
	{
		//获取变化控制系数
		int	nMoveXPercent = m_listCtrlInfo[i]->m_nMoveXPercent;
		int	nMoveYPercent = m_listCtrlInfo[i]->m_nMoveYPercent;
		int	nZoomXPercent = m_listCtrlInfo[i]->m_nZoomXPercent;
		int	nZoomYPercent = m_listCtrlInfo[i]->m_nZoomYPercent;

		CWnd*	pWndCtrl = m_listCtrlInfo[i]->m_pWnd;
		HWND	hWnd = pWndCtrl->GetSafeHwnd();
		if ((NULL != pWndCtrl) && IsWindow(hWnd))
		{
			int nLeft = m_listCtrlInfo[i]->m_rectWnd.left;
			int nTop = m_listCtrlInfo[i]->m_rectWnd.top;
			int nWidth = m_listCtrlInfo[i]->m_rectWnd.Width();
			int nHeight = m_listCtrlInfo[i]->m_rectWnd.Height();

			//设置新的位置参数
			nLeft += (nIncrementX*nMoveXPercent / 100);
			nTop += (nIncrementY*nMoveYPercent / 100);
			nWidth += (nIncrementX*nZoomXPercent / 100);
			nHeight += (nIncrementY*nZoomYPercent / 100);

			//  把控件移动到新位置
			pWndCtrl->MoveWindow(nLeft, nTop, nWidth, nHeight);

		}

	}
	if (GetDlgItem(IDC_PIC) != NULL)
		GetDlgItem(IDC_PIC)->GetClientRect(&m_rectPic);
	Invalidate(TRUE);//强制刷新窗口，防止出现鬼影
	UpdateWindow(); //updateWindow必须和invalidate配合使用才能有效果
	if(m_frmCtl.m_bIsInitialized && m_frmCtl.GetFrameIdx() != -1) //防止未初始化的时候读取到错误帧
		OnDisplay(m_frmCtl.GetCurrentFrame()); //防止暂停后拖动窗口被背景色填充掉
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}


void CVideoPlayerDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//设置窗口的最小大小
	lpMMI->ptMinTrackSize = m_ptMinTrackSize;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


#pragma endregion

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
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
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

#ifdef PLAY_YUV
	this->SetWindowTextW(_T("YUV播放器 By YXP"));
#endif
#ifdef PLAY_SEQ
	this->SetWindowTextW(_T("序列图播放器 By YXP"));
#endif

#pragma region AutoFit
	CRect	rectTemp;

	//获取客户区大小
	GetClientRect(rectTemp);
	//保存客户区信息
	m_nWinWidth = rectTemp.Width();
	m_nWinHeight = rectTemp.Height();
	//获取窗口大小
	GetWindowRect(rectTemp);
	//设置窗口最小值
	SetMinSize(rectTemp.Width(), rectTemp.Height());

	//适配控件
	MakeCtrlFit(GetDlgItem(IDC_PIC), 0, 0, 100, 100);

	MakeCtrlFit(GetDlgItem(IDC_BTN_START), 0, 100, 20); //前两个参数用于控制位置的变动(不考虑缩放)(值代表其所在行或列的起始位置百分比)
	MakeCtrlFit(GetDlgItem(IDC_BTN_SUSPEND), 20, 100, 20); //后两个参数用于控制缩放(数值代表其所在行或列所占的比例)
	MakeCtrlFit(GetDlgItem(IDC_BTN_STOP), 40, 100, 20);
	MakeCtrlFit(GetDlgItem(IDC_BTN_EXIT), 80, 100, 20);
	MakeCtrlFit(GetDlgItem(IDC_LB_FRM), 100, 100, 20);

	MakeCtrlFit(GetDlgItem(IDC_BTN_FRM_PRE), 0, 100, 20);
	MakeCtrlFit(GetDlgItem(IDC_BTN_FRM_NEXT), 20, 100, 20);
	MakeCtrlFit(GetDlgItem(IDC_STATIC_DELAY), 80, 100, 40);
	MakeCtrlFit(GetDlgItem(IDC_EDIT_DELAY), 80, 100, 20);
	MakeCtrlFit(GetDlgItem(IDC_SPIN_DELAY), 100, 100);

#pragma endregion


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
	if (m_vecFiles.size() < 1)
	{
		MessageBox(_T("找不到序列图!"));
		return;
	}
	m_frmCtl.InitParams(m_vecFiles);
#endif // PLAY_SEQ

#ifdef PLAY_YUV

	CFileDialog fDlg(true); //没指定那么详细了
	if (fDlg.DoModal() == IDCANCEL)
		return;
	CString YUVFileName = fDlg.GetPathName();
	//printf("%ls", fDlg.GetFileExt());
	if (fDlg.GetFileExt() != _T("yuv") && fDlg.GetFileExt() != _T("YUV"))
	{
		MessageBox(_T("请打开yuv格式的文件!"));
		return;
	}
	//弹出对话框选择帧大小
	CDlgYUVParams dlg;
	if (dlg.DoModal() == IDOK)
	{
		this->m_nFrmHeight = dlg.m_nFrmHeight;
		this->m_nFrmWidth = dlg.m_nFrmWidth;
	}
	else
		return;

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


