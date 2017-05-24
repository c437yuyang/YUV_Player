#pragma once

#include <opencv2/opencv.hpp>
#include "FrameIndexControl.h"

//CFrameManager类,所有帧管理器的基类
class CFrameManager
{
public:
	CFrameManager();
	~CFrameManager();

protected:
	CFrameIndexControl m_frmIdXCtl;

public:
	virtual cv::Mat GetNextFrame() = 0;
	virtual cv::Mat GetPreFrame() = 0;
	virtual	void QuitOps() = 0;

	int GetFrameCount() { return m_frmIdXCtl.GetFrameCount(); }
	int GetFrameIdx() { return m_frmIdXCtl.GetFrameIndex(); }
};

