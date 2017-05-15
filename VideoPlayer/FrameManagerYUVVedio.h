#pragma once


#include <opencv2/opencv.hpp>
#include <fstream>
#include "FrameIndexControl.h"
//CFrameManagerYUVVedio�࣬��ָ��.YUV�ļ�����֡
class CFrameManagerYUVVedio
{
public:
	CFrameManagerYUVVedio();
	~CFrameManagerYUVVedio();

private:
	CFrameIndexControl m_frmIdXCtl;
	std::vector<std::string> m_vecFiles;

	std::ifstream m_ifs;
	int m_nFrameSize; //һ֡ͼ��Ĵ�С
	int m_nFrameWidth;
	int m_nFrameHeight;
public:
	cv::Mat GetNextFrame();
	void InitParams(CString YUVFile, int width, int height); //���濼������һ���ӿ���ʽ
	void QuitOps();
	int GetFrameCount() { return m_frmIdXCtl.GetFrameCount(); }
	int GetFrameIdx() { return m_frmIdXCtl.GetFrameIndex(); }
};

