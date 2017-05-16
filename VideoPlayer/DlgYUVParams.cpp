// DlgYUVParams.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoPlayer.h"
#include "DlgYUVParams.h"
#include "afxdialogex.h"


// CDlgYUVParams 对话框

IMPLEMENT_DYNAMIC(CDlgYUVParams, CDialogEx)

CDlgYUVParams::CDlgYUVParams(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_YUV_PARAMS, pParent)
	, m_nFrmWidth(176)
	, m_nFrmHeight(144)
	, m_nYUVType(0)
{

}

CDlgYUVParams::~CDlgYUVParams()
{
}

void CDlgYUVParams::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FRM_WIDTH, m_nFrmWidth);
	DDX_Text(pDX, IDC_EDIT_FRM_HEIGHT, m_nFrmHeight);
	DDX_Radio(pDX, IDC_RADIO_YUV420, m_nYUVType);
}


BEGIN_MESSAGE_MAP(CDlgYUVParams, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgYUVParams::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgYUVParams 消息处理程序


void CDlgYUVParams::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if (m_nFrmHeight < 0 || m_nFrmWidth < 0) 
	{
		MessageBox(_T("参数无效!"));
		return;
	}

	CDialogEx::OnOK();
}
