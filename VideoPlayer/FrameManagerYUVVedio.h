#pragma once


#include <opencv2/opencv.hpp>
#include <fstream>
#include "FrameIndexControl.h"
//CFrameManagerYUVVedio类，从指定.YUV文件返回帧
class CFrameManagerYUVVedio
{
public:
	CFrameManagerYUVVedio();
	~CFrameManagerYUVVedio();

private:
	CFrameIndexControl m_frmIdXCtl;
	std::vector<std::string> m_vecFiles;

	std::ifstream m_ifs;
	int m_nFrameSize; //一帧图像的大小
	int m_nFrameWidth;
	int m_nFrameHeight;
public:
	cv::Mat GetNextFrame();
	void InitParams(CString YUVFile, int width, int height); //后面考虑做成一个接口形式
	void QuitOps();
	int GetFrameCount() { return m_frmIdXCtl.GetFrameCount(); }
	int GetFrameIdx() { return m_frmIdXCtl.GetFrameIndex(); }
};

