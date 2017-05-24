
// VideoPlayerDlg.h : ͷ�ļ�
//

#pragma once
#include "FrameManagerSeq.h"
#include "FrameManagerYUVVedio.h"
#include "DlgYUVParams.h"

#define PLAY_SEQ
//#define PLAY_YUV
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
	afx_msg void OnBnClickedBtnSuspend();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnBnClickedBtnFrmPre();
	afx_msg void OnBnClickedBtnFrmNext();
	afx_msg void OnDeltaposSpinDelay(NMHDR *pNMHDR, LRESULT *pResult);
public:
	void OnDisplay(cv::Mat & curFrame); //������ʾ��ǰ֡��picture�ؼ�
	CEvent m_event; //���ڿ�����ͣ�Ļ������
	CRect m_rectPic; //picture�ؼ�λ�ñ���
	bool m_bIsStarted; //����״̬
	bool m_bIsPaused; //����״̬
	CString m_strShowFrm; //��ʾ��ǰ֡���ı�ǩ��������
	int m_nFrmDelay; //����֡����ʱ����
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
