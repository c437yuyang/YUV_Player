#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

#include "FrameIndexControl.h"

//CFrameManager�࣬��ָ������ͼ(vector<string> files) ����֡
class CFrameManager
{
public:
	CFrameManager();
	~CFrameManager();

private:
	CFrameIndexControl m_frmIdXCtl;
	std::vector<std::string> m_vecFiles;
public:
	cv::Mat GetNextFrame();
	void InitParams(std::vector<std::string> files);
	void QuitOps();
};

