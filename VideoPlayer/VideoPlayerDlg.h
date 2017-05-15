
// VideoPlayerDlg.h : ͷ�ļ�
//

#pragma once
#include "FrameManager.h"
#include "FrameManagerYUVVedio.h"

#define PLAY_YUV
// CVideoPlayerDlg �Ի���
class CVideoPlayerDlg : public CDialogEx
{
	// ����
public:
	CVideoPlayerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIDEOPLAYER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();
	
public:
	CEvent m_event;
	vector<string> m_vecFiles;
	void OnDisplay(cv::Mat & curFrame);
	CRect m_rectPic;
	bool m_bIsStarted;
	bool m_bIsPaused;
#ifdef PLAY_SEQ
	CFrameManager m_frmCtl;
#endif

#ifdef PLAY_YUV
	CFrameManagerYUVVedio m_frmCtl;
#endif
	afx_msg void OnBnClickedBtnSuspend();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnExit();
	CString m_strShowFrm;
};
