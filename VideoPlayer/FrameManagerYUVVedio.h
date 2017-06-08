#pragma once

#include "FrameManager.h"
#include <fstream>
//CFrameManagerYUVVedio类，从指定.YUV文件返回帧
class CFrameManagerYUVVedio:public CFrameManager
{
public:
	CFrameManagerYUVVedio();
	~CFrameManagerYUVVedio();

private:
	std::vector<std::string> m_vecFiles;
	std::ifstream m_ifs;
	int m_nFrameSize; //一帧图像的大小
	int m_nFrameWidth;
	int m_nFrameHeight;
	cv::Mat CFrameManagerYUVVedio::GetFrameByPos();
public:
#pragma region 接口里的方法
	cv::Mat GetNextFrame();
	cv::Mat GetPreFrame();
	cv::Mat GetCurrentFrame();
	void QuitOps();
#pragma endregion
	void InitParams(CString YUVFile, int width, int height); //后面考虑做成一个接口形式

};

