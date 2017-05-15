#pragma once
#include <io.h>

#include <string> //������ò�Ҫȥ����stdAfx����һ�ɶ��Բ����д����õ���Щ�ܺã��ڶ������������
#include <vector>
#include <opencv2/opencv.hpp>


class CCommon
{
public:
	CCommon();
	~CCommon();

	//static  
	static bool FindOrCreateDirectory(const char *pszPath);
	static void MyShowImage(const std::string winName, cv::Mat &Image, unsigned int nScale);
	static void GetFileName(const std::string &strFolder, std::vector<std::string> &strVecFileNames);
	static long GetFileSize(const std::string &FileName);
};

