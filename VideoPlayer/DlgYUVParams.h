#pragma once


// CDlgYUVParams 对话框

class CDlgYUVParams : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgYUVParams)

public:
	CDlgYUVParams(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgYUVParams();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_YUV_PARAMS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nFrmWidth;
	int m_nFrmHeight;
	afx_msg void OnBnClickedOk();
	int m_nYUVType;
};
