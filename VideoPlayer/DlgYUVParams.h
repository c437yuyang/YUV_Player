#pragma once


// CDlgYUVParams �Ի���

class CDlgYUVParams : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgYUVParams)

public:
	CDlgYUVParams(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgYUVParams();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_YUV_PARAMS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_nFrmWidth;
	int m_nFrmHeight;
	afx_msg void OnBnClickedOk();
	int m_nYUVType;
};
