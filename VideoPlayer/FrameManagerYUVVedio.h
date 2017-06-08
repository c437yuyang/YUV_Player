#pragma once

#include "FrameManager.h"
#include <fstream>
//CFrameManagerYUVVedio�࣬��ָ��.YUV�ļ�����֡
class CFrameManagerYUVVedio:public CFrameManager
{
public:
	CFrameManagerYUVVedio();
	~CFrameManagerYUVVedio();

private:
	std::vector<std::string> m_vecFiles;
	std::ifstream m_ifs;
	int m_nFrameSize; //һ֡ͼ��Ĵ�С
	int m_nFrameWidth;
	int m_nFrameHeight;
	cv::Mat CFrameManagerYUVVedio::GetFrameByPos();
public:
#pragma region �ӿ���ķ���
	cv::Mat GetNextFrame();
	cv::Mat GetPreFrame();
	cv::Mat GetCurrentFrame();
	void QuitOps();
#pragma endregion
	void InitParams(CString YUVFile, int width, int height); //���濼������һ���ӿ���ʽ

};

