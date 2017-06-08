
// VideoPlayerDlg.h : 头文件
//

#pragma once
#include "FrameManagerSeq.h"
#include "FrameManagerYUVVedio.h"
#include "DlgYUVParams.h"


#pragma region AutoFit
/**
*  控件随窗口变化的控制单元
**/
typedef struct tagCONTROL
{
	CWnd*	m_pWnd;				//指向控件的指针
	CRect	m_rectWnd;			//控件所占区域
	int		m_nMoveXPercent,	//控件沿x轴移动的百分比
		m_nMoveYPercent,	//控件沿y轴移动的百分比
		m_nZoomXPercent,	//控件沿x轴缩放的百分比
		m_nZoomYPercent;	//控件沿y轴缩放的百分比
}ControlInfo, *lpControlInfo;

typedef		CArray<lpControlInfo>	CTRLLIST;	//控件的适配信息表
#pragma endregion

//#define PLAY_SEQ
#define PLAY_YUV
// CVideoPlayerDlg 对话框
class CVideoPlayerDlg : public CDialogEx
{
	// 构造
public:
	CVideoPlayerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIDEOPLAYER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnSuspend();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnBnClickedBtnFrmPre();
	afx_msg void OnBnClickedBtnFrmNext();
	afx_msg void OnDeltaposSpinDelay(NMHDR *pNMHDR, LRESULT *pResult);
public:
	void OnDisplay(cv::Mat & curFrame); //用于显示当前帧到picture控件
	CEvent m_event; //用于控制暂停的互斥变量
	CRect m_rectPic; //picture控件位置变量
	bool m_bIsStarted; //播放状态
	bool m_bIsPaused; //播放状态
	CString m_strShowFrm; //显示当前帧数的标签关联变量
	int m_nFrmDelay; //控制帧间延时变量
#ifdef PLAY_SEQ
	CFrameManagerSeq m_frmCtl;
	vector<string> m_vecFiles;
#endif

#ifdef PLAY_YUV
	CFrameManagerYUVVedio m_frmCtl;
	int m_nFrmWidth;
	int m_nFrmHeight;
#endif

#pragma region AutoFit
public:
	~CVideoPlayerDlg();

	void	SetMinSize(int nWidth, int nHeight);
	void	FreeCtrlInfoList();
	void	MakeCtrlFit(CWnd* pWnd, int nMoveXPercent = 0, int nMoveYPercent = 0, int nZoomXPercent = 0, int nZoomYPercent = 0);
	void	CancelCtrlFit(HWND hWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

	int			m_nWinWidth,		//窗口的宽度
		m_nWinHeight;		//窗口的高度
	POINT		m_ptMinTrackSize;	//窗口的最小大小
	CTRLLIST	m_listCtrlInfo;		//控件适配信息的数组  
#pragma endregion



};
