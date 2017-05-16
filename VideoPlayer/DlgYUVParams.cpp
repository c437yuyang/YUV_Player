// DlgYUVParams.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VideoPlayer.h"
#include "DlgYUVParams.h"
#include "afxdialogex.h"


// CDlgYUVParams �Ի���

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


// CDlgYUVParams ��Ϣ�������


void CDlgYUVParams::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if (m_nFrmHeight < 0 || m_nFrmWidth < 0) 
	{
		MessageBox(_T("������Ч!"));
		return;
	}

	CDialogEx::OnOK();
}
