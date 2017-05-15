#pragma once
#include <io.h>

#include <string> //这里最好不要去包含stdAfx，第一可读性差，这里写清楚用到哪些很好，第二互相包含问题
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

