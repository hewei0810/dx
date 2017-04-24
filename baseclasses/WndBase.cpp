#include "stdafx.h"
#include "wndbase.h"
#include <assert.h>

CWndBase::CWndBase()
: m_hWnd(NULL)
, m_hInst(NULL)
{

}

CWndBase::~CWndBase()
{

}

void CWndBase::RegisterWindowClass(WNDCLASSEX& wcex)
{
	m_hInst = wcex.hInstance;
	if (wcex.lpfnWndProc == NULL)
		wcex.lpfnWndProc = DefWindowProc;
	::RegisterClassEx(&wcex);
}

void CWndBase::Create(LPCTSTR lpWindowName,DWORD dwStyle /*= WS_OVERLAPPEDWINDOW*/,int x /*= CW_USEDEFAULT*/,int y /*= CW_USEDEFAULT*/,int w /*= CW_USEDEFAULT*/,int h /*= CW_USEDEFAULT*/, HWND hWndParent /*= NULL*/,HMENU hMenu /*= NULL*/,LPVOID lpParam /*= NULL*/)
{
	if (::IsWindow(m_hWnd))
		return;

	m_hWnd = ::CreateWindow(GetWindowClassName(), lpWindowName, dwStyle, x, y, w, h, hWndParent, hMenu, m_hInst, lpParam);
}

void CWndBase::Close()
{
	assert(::IsWindow(m_hWnd));

	::CloseWindow(m_hWnd);
}

void CWndBase::CenterWindow()
{
	assert(::IsWindow(m_hWnd));

	RECT rcThis = {0};
	::GetWindowRect(m_hWnd, &rcThis);
	RECT rcArea = {0};
	RECT rcCenter = {0};
	HWND hWndParent = ::GetParent(m_hWnd);
	HWND hWndOwner = ::GetWindowOwner(m_hWnd);
	::SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea, NULL);
	if (hWndOwner == NULL)
		rcCenter = rcArea;
	else
		::GetWindowRect(hWndOwner, &rcCenter);

	int left = (rcCenter.left + rcCenter.right) / 2 - (rcThis.right - rcThis.left) / 2;
	int top  = (rcCenter.top + rcCenter.bottom) / 2 - (rcThis.bottom - rcThis.top) / 2;

	::SetWindowPos(m_hWnd, NULL, left, top, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void CWndBase::ShowWindow(int nCmdShow)
{
	assert(::IsWindow(m_hWnd));

	::ShowWindow(m_hWnd, nCmdShow);
}

void CWndBase::ResizeClient(int w, int h)
{
	assert (::IsWindow(m_hWnd));

	RECT rc = {0};
	rc.right = w;
	rc.bottom = h;
	::AdjustWindowRect(&rc, GetWindowStyle(m_hWnd), ::GetMenu(m_hWnd) != NULL);
	::SetWindowPos(m_hWnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
}

int CWndBase::LoopMessage()
{
	MSG msg = {0};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}

//int CWndBase::LoopPeekMessage()
//{
//	MSG msg = {0};
//	while (msg.message != WM_QUIT)
//	{
//		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//		{
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//		else
//		{
//			//œ– ±¥¶¿Ì
//			OnIdle();
//		}
//	}
//
//	return (int) msg.wParam;
//}