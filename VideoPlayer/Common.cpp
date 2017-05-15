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
			//::AfxMessageBox(_T("Ŀ¼���ڣ�"));
			return true;
		}
	}

	if (!::CreateDirectory(A2W(pszPath), NULL))
	{
		//char szDir[MAX_PATH];
		::AfxMessageBox(_T("����Ŀ¼ʧ��"));
		return false;
	}
	else
	{
		return true;
		//::AfxMessageBox(_T("����Ŀ¼�ɹ�"));
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
		if (!strcmp(filefind.name, ".."))  //�ô˷�����һ���ҵ����ļ�����Զ��".."��������Ҫ�����ж�
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
		fseek(pFile, 0, SEEK_END);   ///���ļ�ָ���ƶ��ļ���β
		size = ftell(pFile); ///�����ǰ�ļ�ָ������ļ���ʼ���ֽ���
		fclose(pFile);
		printf("Size of file.cpp: %ld bytes.\n", size);
	}
	
	return size;
}
