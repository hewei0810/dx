#pragma once

#include "resource.h"

// ��������
class CMainWnd : public CWndBase
{
public:
	virtual LPCTSTR GetWindowClassName() {return _T("MainWnd");}
};
