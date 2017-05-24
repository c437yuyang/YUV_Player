
// VideoPlayerDlg.h : 头文件
//

#pragma once
#include "FrameManagerSeq.h"
#include "FrameManagerYUVVedio.h"
#include "DlgYUVParams.h"

#define PLAY_SEQ
//#define PLAY_YUV
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

};
