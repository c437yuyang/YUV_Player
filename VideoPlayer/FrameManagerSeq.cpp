#include "stdafx.h"
#include "FrameManagerSeq.h"


CFrameManagerSeq::CFrameManagerSeq()
{
}


CFrameManagerSeq::~CFrameManagerSeq()
{
}


cv::Mat CFrameManagerSeq::GetNextFrame()
{
	m_frmIdXCtl.NextFrame();
	return imread(m_vecFiles[m_frmIdXCtl.GetFrameIndex()]);
}


cv::Mat CFrameManagerSeq::GetPreFrame()
{
	m_frmIdXCtl.PreFrame();
	return imread(m_vecFiles[m_frmIdXCtl.GetFrameIndex()]);
}


cv::Mat CFrameManagerSeq::GetCurrentFrame()
{
	return imread(m_vecFiles[m_frmIdXCtl.GetFrameIndex()]);
}

void CFrameManagerSeq::InitParams(std::vector<std::string> files)
{
	m_vecFiles = files;
	m_frmIdXCtl.SetFrameCount(m_vecFiles.size());
	m_frmIdXCtl.SetFrameIndex(-1);
	m_frmIdXCtl.SetReverse(false);
	m_bIsInitialized = true;
}

void CFrameManagerSeq::QuitOps()
{
	m_bIsInitialized = false;

}
