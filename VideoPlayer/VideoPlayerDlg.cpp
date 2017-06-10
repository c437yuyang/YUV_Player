
// VideoPlayerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VideoPlayer.h"
#include "VideoPlayerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD WINAPI PlayVideo(LPVOID lpParam);


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CVideoPlayerDlg �Ի���



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

	//���ô�����Сֵ
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
	ASSERT(pWnd);									//ָ���Ƿ�Ϊ��
	ASSERT(nMoveXPercent >= 0 && nMoveXPercent <= 100);	//nMoveXPercentֵ�Ƿ���Ч
	ASSERT(nMoveYPercent >= 0 && nMoveYPercent <= 100);	//nMoveXPercentֵ�Ƿ���Ч
	ASSERT(nZoomXPercent >= 0 && nZoomXPercent <= 100);	//nMoveXPercentֵ�Ƿ���Ч
	ASSERT(nZoomYPercent >= 0 && nZoomYPercent <= 100);	//nMoveXPercentֵ�Ƿ���Ч

	lpControlInfo	pCtrlInfo = new ControlInfo;	//�����ṹָ��

													//������
	pCtrlInfo->m_pWnd = pWnd;
	pCtrlInfo->m_nMoveXPercent = nMoveXPercent;
	pCtrlInfo->m_nMoveYPercent = nMoveYPercent;
	pCtrlInfo->m_nZoomXPercent = nZoomXPercent;
	pCtrlInfo->m_nZoomYPercent = nZoomYPercent;

	pWnd->GetWindowRect(pCtrlInfo->m_rectWnd);
	ScreenToClient(&pCtrlInfo->m_rectWnd);

	m_listCtrlInfo.Add(pCtrlInfo);	//����ά���б�
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
	//���㴰�ڿ�Ⱥ͸߶ȵĸı���
	int nIncrementX = cx - m_nWinWidth;
	int nIncrementY = cy - m_nWinHeight;

	INT_PTR	nCount = m_listCtrlInfo.GetSize();

	UINT	uFlags = SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOCOPYBITS;
	for (int i = 0; i < nCount; i++)
	{
		//��ȡ�仯����ϵ��
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

			//�����µ�λ�ò���
			nLeft += (nIncrementX*nMoveXPercent / 100);
			nTop += (nIncrementY*nMoveYPercent / 100);
			nWidth += (nIncrementX*nZoomXPercent / 100);
			nHeight += (nIncrementY*nZoomYPercent / 100);

			//  �ѿؼ��ƶ�����λ��
			pWndCtrl->MoveWindow(nLeft, nTop, nWidth, nHeight);

		}

	}
	if (GetDlgItem(IDC_PIC) != NULL)
		GetDlgItem(IDC_PIC)->GetClientRect(&m_rectPic);
	Invalidate(TRUE);//ǿ��ˢ�´��ڣ���ֹ���ֹ�Ӱ
	UpdateWindow(); //updateWindow�����invalidate���ʹ�ò�����Ч��
	if(m_frmCtl.m_bIsInitialized && m_frmCtl.GetFrameIdx() != -1) //��ֹδ��ʼ����ʱ���ȡ������֡
		OnDisplay(m_frmCtl.GetCurrentFrame()); //��ֹ��ͣ���϶����ڱ�����ɫ����
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}


void CVideoPlayerDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//���ô��ڵ���С��С
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


// CVideoPlayerDlg ��Ϣ�������

BOOL CVideoPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	GetDlgItem(IDC_PIC)->GetClientRect(&m_rectPic);

#ifdef PLAY_YUV
	this->SetWindowTextW(_T("YUV������ By YXP"));
#endif
#ifdef PLAY_SEQ
	this->SetWindowTextW(_T("����ͼ������ By YXP"));
#endif

#pragma region AutoFit
	CRect	rectTemp;

	//��ȡ�ͻ�����С
	GetClientRect(rectTemp);
	//����ͻ�����Ϣ
	m_nWinWidth = rectTemp.Width();
	m_nWinHeight = rectTemp.Height();
	//��ȡ���ڴ�С
	GetWindowRect(rectTemp);
	//���ô�����Сֵ
	SetMinSize(rectTemp.Width(), rectTemp.Height());

	//����ؼ�
	MakeCtrlFit(GetDlgItem(IDC_PIC), 0, 0, 100, 100);

	MakeCtrlFit(GetDlgItem(IDC_BTN_START), 0, 100, 20); //ǰ�����������ڿ���λ�õı䶯(����������)(ֵ�����������л��е���ʼλ�ðٷֱ�)
	MakeCtrlFit(GetDlgItem(IDC_BTN_SUSPEND), 20, 100, 20); //�������������ڿ�������(��ֵ�����������л�����ռ�ı���)
	MakeCtrlFit(GetDlgItem(IDC_BTN_STOP), 40, 100, 20);
	MakeCtrlFit(GetDlgItem(IDC_BTN_EXIT), 80, 100, 20);
	MakeCtrlFit(GetDlgItem(IDC_LB_FRM), 100, 100, 20);

	MakeCtrlFit(GetDlgItem(IDC_BTN_FRM_PRE), 0, 100, 20);
	MakeCtrlFit(GetDlgItem(IDC_BTN_FRM_NEXT), 20, 100, 20);
	MakeCtrlFit(GetDlgItem(IDC_STATIC_DELAY), 80, 100, 40);
	MakeCtrlFit(GetDlgItem(IDC_EDIT_DELAY), 80, 100, 20);
	MakeCtrlFit(GetDlgItem(IDC_SPIN_DELAY), 100, 100);

#pragma endregion


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CVideoPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CVideoPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVideoPlayerDlg::OnBnClickedBtnStart()
{

#ifdef PLAY_SEQ

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR wchPath[MAX_PATH];     //���ѡ���Ŀ¼·�� 
	CString cStrPath;
	ZeroMemory(wchPath, sizeof(wchPath));
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = wchPath;
	bi.lpszTitle = _T("��ѡ����Ҫ�򿪵�Ŀ¼��");
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	//����ѡ��Ŀ¼�Ի���
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	if (lp && SHGetPathFromIDList(lp, wchPath))
	{
		cStrPath.Format(_T("%s"), wchPath);//תΪCstring����
	}
	if (cStrPath.IsEmpty())
		return;

	USES_CONVERSION;
	string strPath = W2A(cStrPath.GetBuffer());

	//ѡ��Ҫ�򿪵�Ŀ¼�󣬲���ָ��������
	//1.�Ȼ�ȡ�����ļ���
	CCommon::GetFileName(strPath, m_vecFiles); //C++�ľ�̬��Ա��::�ĵ��á�������
	if (m_vecFiles.size() < 1)
	{
		MessageBox(_T("�Ҳ�������ͼ!"));
		return;
	}
	m_frmCtl.InitParams(m_vecFiles);
#endif // PLAY_SEQ

#ifdef PLAY_YUV

	CFileDialog fDlg(true); //ûָ����ô��ϸ��
	if (fDlg.DoModal() == IDCANCEL)
		return;
	CString YUVFileName = fDlg.GetPathName();
	//printf("%ls", fDlg.GetFileExt());
	if (fDlg.GetFileExt() != _T("yuv") && fDlg.GetFileExt() != _T("YUV"))
	{
		MessageBox(_T("���yuv��ʽ���ļ�!"));
		return;
	}
	//�����Ի���ѡ��֡��С
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

	HANDLE hThreadSend;			//���������̷߳�������
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

		if (pThis->m_bIsStarted == false) //����������־��λ��ֹͣ����
			_endthreadex(0);

		Mat frm = pThis->m_frmCtl.GetNextFrame();
		pThis->OnDisplay(frm);
		pThis->m_strShowFrm.Format(_T("%d/%d"), pThis->m_frmCtl.GetFrameIdx() + 1, pThis->m_frmCtl.GetFrameCount());
		pThis->UpdateData(false);

		Sleep(pThis->m_nFrmDelay);

	}
	return 0;
}



void CVideoPlayerDlg::OnDisplay(cv::Mat &curFrame)  //���������С�����display��ʾ��picture�ؼ�
{
	CvSize newsize;
	newsize.width = m_rectPic.Width();
	newsize.height = m_rectPic.Height();

	Mat imgResized;
	imgResized.create(newsize, CV_8UC3);
	resize(curFrame, imgResized, imgResized.size());

	IplImage iplImage(imgResized); //ת����IplImage����

	CDC* pDC = GetDlgItem(IDC_PIC)->GetDC();
	CvvImage cvImg;
	cvImg.CopyOf(&iplImage);
	cvImg.DrawToHDC(pDC->m_hDC, &m_rectPic);
	ReleaseDC(pDC);
}


void CVideoPlayerDlg::OnBnClickedBtnSuspend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_bIsStarted)
		return;

	CString buttonText;
	GetDlgItemTextW(IDC_BTN_SUSPEND, buttonText);

	//��������ʱ
	if (buttonText.Compare(_T("��ͣ")) == 0)
	{
		m_event.ResetEvent();
		SetDlgItemTextW(IDC_BTN_SUSPEND, _T("����"));
		m_bIsPaused = true;
	}
	else //��ͣʱ
	{
		m_event.SetEvent();
		SetDlgItemTextW(IDC_BTN_SUSPEND, _T("��ͣ"));
		m_bIsPaused = false;
		//g_dScaleFactor = 1.0;
	}
}


void CVideoPlayerDlg::OnBnClickedBtnStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (!m_bIsStarted)
		return;
	m_bIsStarted = false;
	m_bIsPaused = true;
	m_frmCtl.QuitOps();
	SetDlgItemTextW(IDC_BTN_SUSPEND, _T("��ͣ"));
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_PIC);
	CRect lRect;
	pStatic->GetClientRect(&lRect);
	pStatic->GetDC()->FillSolidRect(lRect.left, lRect.top, lRect.Width(), lRect.Height(), RGB(160, 160, 160));
#ifdef VERBOSE
	cout << "����˽���" << endl;
#endif // VERBOSE
}


void CVideoPlayerDlg::OnBnClickedBtnExit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_bIsStarted)
		OnBnClickedBtnStop();
	OnCancel();
}


void CVideoPlayerDlg::OnBnClickedBtnFrmPre()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������


	if (m_bIsStarted && !m_bIsPaused)
		OnBnClickedBtnSuspend();

	Mat frm = m_frmCtl.GetPreFrame();
	OnDisplay(frm);
	m_strShowFrm.Format(_T("%d/%d"), m_frmCtl.GetFrameIdx() + 1, m_frmCtl.GetFrameCount());
	UpdateData(false);

}


void CVideoPlayerDlg::OnBnClickedBtnFrmNext()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UpdateData(1);
	if (pNMUpDown->iDelta == -1) // �����ֵΪ-1 , ˵�������Spin�����ϵļ�ͷ
	{
		if (m_nFrmDelay < 1000)
			m_nFrmDelay += 10;
	}
	else if (pNMUpDown->iDelta == 1) // �����ֵΪ1, ˵�������Spin�����µļ�ͷ
	{
		if (m_nFrmDelay > 20)
			m_nFrmDelay -= 10;
	}
	UpdateData(false);
	*pResult = 0;
}


