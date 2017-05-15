#include "stdafx.h"
#include "FrameIndexControl.h"


CFrameIndexControl::CFrameIndexControl()
{
	m_nFrameCount = 0;
	m_bReverse = false;
}


CFrameIndexControl::~CFrameIndexControl()
{
}


bool CFrameIndexControl::GetReverse()
{
	return m_bReverse;
}

void CFrameIndexControl::SetFrameCount(int i)
{
	m_nFrameCount = i;
}

void CFrameIndexControl::SetFrameIndex(int index)
{
	m_nFrameIndex = index;
}

void CFrameIndexControl::NextFrame()
{
	if (!m_bReverse)
	{
		if (m_nFrameIndex < m_nFrameCount - 1)
		{
			++m_nFrameIndex;
		}
		else
		{
			m_nFrameIndex = 0;
		}

	}
	else
	{
		if (m_nFrameIndex == 0)
		{
			m_nFrameIndex = m_nFrameCount - 1;
		}
		else
		{
			--m_nFrameIndex;
		}
	}
}


void CFrameIndexControl::PreFrame()
{
	if (m_bReverse)
	{
		if (m_nFrameIndex < m_nFrameCount - 1)
		{
			++m_nFrameIndex;
		}
		else
		{
			m_nFrameIndex = 0;
		}

	}
	else
	{
		if (m_nFrameIndex == 0)
		{
			m_nFrameIndex = m_nFrameCount - 1;
		}
		else
		{
			--m_nFrameIndex;
		}
	}
}

void CFrameIndexControl::SetReverse(bool b)
{
	m_bReverse = b;
}
