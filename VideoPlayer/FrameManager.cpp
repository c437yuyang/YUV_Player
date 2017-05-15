#include "stdafx.h"
#include "FrameManager.h"


CFrameManager::CFrameManager()
{

}

CFrameManager::~CFrameManager()
{
}

cv::Mat CFrameManager::GetNextFrame()
{
	m_frmIdXCtl.NextFrame();
	return imread(m_vecFiles[m_frmIdXCtl.GetFrameIndex()]);
}

void CFrameManager::InitParams(std::vector<std::string> files)
{
	m_vecFiles = files;
	m_frmIdXCtl.SetFrameCount(m_vecFiles.size());
	m_frmIdXCtl.SetFrameIndex(-1);
	m_frmIdXCtl.SetReverse(false);
}

void CFrameManager::QuitOps()
{

}
