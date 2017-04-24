#pragma once

#include "resource.h"

#include "wndbase.h"
#include "D3DSample1.h"

// Ö÷´°¿ÚÀà
class CMainWnd : public CWndBase
{
public:
	virtual LPCTSTR GetWindowClassName() {return _T("MainWnd");}
};
