#include "stdafx.h"
#include "Common.h"


CCommon::CCommon()
{
}


CCommon::~CCommon()
{
}

bool CCommon::FindOrCreateDirectory(const char *pszPath)
{
	USES_CONVERSION;
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFileW(A2W(pszPath), &fd);
	while (hFind != INVALID_HANDLE_VALUE)
	{
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//::AfxMessageBox(_T("目录存在！"));
			return true;
		}
	}

	if (!::CreateDirectory(A2W(pszPath), NULL))
	{
		//char szDir[MAX_PATH];
		::AfxMessageBox(_T("创建目录失败"));
		return false;
	}
	else
	{
		return true;
		//::AfxMessageBox(_T("创建目录成功"));
	}
}



void CCommon::MyShowImage(const string winName, Mat &Image, unsigned nScale)
{
	namedWindow(winName, CV_WINDOW_NORMAL);
	resizeWindow(winName, Image.cols / nScale, Image.rows / nScale);
	imshow(winName, Image);
}


void CCommon::GetFileName(const string &strFolder, vector<string> &strVecFileNames)
{
	strVecFileNames.clear();
	struct _finddata_t filefind;
	string  curr = strFolder + "\\*.*";
	int  done = 0;
	int  handle;
	if ((handle = _findfirst(curr.c_str(), &filefind)) == -1)
		return;
	string tempfolder = strFolder + "\\";
	while (!(done = _findnext(handle, &filefind)))
	{
		if (!strcmp(filefind.name, ".."))  //用此方法第一个找到的文件名永远是".."，所以需要单独判断
			continue;
		strVecFileNames.push_back(tempfolder + filefind.name);
	}
	_findclose(handle);
}

long CCommon::GetFileSize(const std::string &FileName)
{
	FILE * pFile;
	long size;

	fopen_s(&pFile,FileName.c_str(), "rb");
	if (pFile == NULL)
		cout << "open Error" << endl;
	else
	{
		fseek(pFile, 0, SEEK_END);   ///将文件指针移动文件结尾
		size = ftell(pFile); ///求出当前文件指针距离文件开始的字节数
		fclose(pFile);
		printf("Size of file.cpp: %ld bytes.\n", size);
	}
	
	return size;
}
