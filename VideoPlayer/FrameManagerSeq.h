#pragma once

#include "FrameManager.h"
#include <vector>
#include <string>

//CFrameManagerSeq类，播放序列图
class CFrameManagerSeq:public CFrameManager
{
public:
	CFrameManagerSeq();
	~CFrameManagerSeq();
	

private:
	std::vector<std::string> m_vecFiles;
public:
#pragma region 接口里的方法
	cv::Mat GetNextFrame();
	cv::Mat GetPreFrame();
	void QuitOps(); //若接口的方法没实现，则还是一个抽象类，不能实例化
#pragma endregion
	void InitParams(std::vector<std::string> files);
};

