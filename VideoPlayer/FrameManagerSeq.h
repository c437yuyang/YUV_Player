#pragma once

#include "FrameManager.h"
#include <vector>
#include <string>

//CFrameManagerSeq�࣬��������ͼ
class CFrameManagerSeq:public CFrameManager
{
public:
	CFrameManagerSeq();
	~CFrameManagerSeq();
	

private:
	std::vector<std::string> m_vecFiles;
public:
#pragma region �ӿ���ķ���
	cv::Mat GetNextFrame();
	cv::Mat GetPreFrame();
	void QuitOps(); //���ӿڵķ���ûʵ�֣�����һ�������࣬����ʵ����
#pragma endregion
	void InitParams(std::vector<std::string> files);
};

