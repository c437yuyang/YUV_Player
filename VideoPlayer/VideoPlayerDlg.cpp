
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
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bIsPaused = false;
	m_bIsStarted = false;
}

void CVideoPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LB_FRM, m_strShowFrm);
}

BEGIN_MESSAGE_MAP(CVideoPlayerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, &CVideoPlayerDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_SUSPEND, &CVideoPlayerDlg::OnBnClickedBtnSuspend)
	ON_BN_CLICKED(IDC_BTN_STOP, &CVideoPlayerDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CVideoPlayerDlg::OnBnClickedBtnExit)
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

	m_frmCtl.InitParams(m_vecFiles);
#endif // PLAY_SEQ

#ifdef PLAY_YUV
	
	CFileDialog fDlg(true); //ûָ����ô��ϸ��
	if (fDlg.DoModal() == IDCANCEL)
		return;
	CString YUVFileName = fDlg.GetPathName();
	m_frmCtl.InitParams(YUVFileName, 176, 144);

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

		pThis->OnDisplay(pThis->m_frmCtl.GetNextFrame());
		pThis->m_strShowFrm.Format(_T("%d/%d"), pThis->m_frmCtl.GetFrameIdx()+1, pThis->m_frmCtl.GetFrameCount());
		pThis->UpdateData(false);
		Sleep(200);

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
}


void CVideoPlayerDlg::OnBnClickedBtnExit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_bIsStarted) 
	{
		OnBnClickedBtnStop();
	}
	OnCancel();
}
