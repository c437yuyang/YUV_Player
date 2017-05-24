#pragma once

#include "FrameManager.h"
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include "FrameIndexControl.h"

class CFrameManagerSeq
{
public:
	CFrameManagerSeq();
	~CFrameManagerSeq();
	

private:
	CFrameIndexControl m_frmIdXCtl;
	std::vector<std::string> m_vecFiles;
public:
	cv::Mat GetNextFrame();
	cv::Mat GetPreFrame();
	void InitParams(std::vector<std::string> files);
	void QuitOps();
	int GetFrameCount() { return m_frmIdXCtl.GetFrameCount(); }
	int GetFrameIdx() { return m_frmIdXCtl.GetFrameIndex(); }

};

