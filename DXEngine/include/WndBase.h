#ifndef __WND_BASE_H__
#define __WND_BASE_H__ 

#include <Windows.h>
#include <WindowsX.h>

class DXENGINE_API CWndBase
{
public:
	CWndBase();
	virtual ~CWndBase();

public:
	virtual LPCTSTR GetWindowClassName() = 0;

	HWND GetHWnd() const {return m_hWnd;}

	void RegisterWindowClass(WNDCLASSEX& wcex);
	void Create(LPCTSTR lpWindowName,DWORD dwStyle = WS_OVERLAPPEDWINDOW,int x = CW_USEDEFAULT,int y = CW_USEDEFAULT,int w = CW_USEDEFAULT,int h = CW_USEDEFAULT, HWND hWndParent = NULL,HMENU hMenu = NULL,LPVOID lpParam = NULL);
	void Close();

	void ShowWindow(int nCmdShow);
	void CenterWindow();

	void ResizeClient(int w, int h);

protected:
	HWND m_hWnd;
	HINSTANCE m_hInst;
};

#endif // __WND_BASE_H__