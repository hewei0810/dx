// DX_sample2.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "DX_sample2.h"

#include "TestScene.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 创建全局管理器
	CGlobalManager::CreateManager();

	// 创建并显示主窗口
	CMainWnd mainWnd;
	WNDCLASSEX wcex = {0};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= CGlobalManager::WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DX_SAMPLE2));
	wcex.hIconSm		= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DX_SAMPLE2));
	wcex.hCursor		= LoadCursor(NULL, IDC_HAND);
	wcex.hbrBackground	= GetStockBrush(BLACK_BRUSH);
	wcex.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDC_DX_SAMPLE2);
	wcex.lpszClassName	= mainWnd.GetWindowClassName();
	mainWnd.RegisterWindowClass(wcex);

	mainWnd.Create(_T("Meet You"), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU);
	mainWnd.ResizeClient(1280, 720);
	mainWnd.CenterWindow();
	mainWnd.ShowWindow(nCmdShow);

	// 初始化全局管理器
	CGlobalManager::GetManager()->InitManager(hInstance, mainWnd.GetHWnd());
	CStdString strPath = CGlobalManager::GetManager()->GetModulePath() + "res";
	CGlobalManager::GetManager()->SetResPath(strPath.GetData());

	// 设置当前创建
	CMenuScene* pScene1 = new CMenuScene;
	if (!pScene1->InitScene())
		return -1;
	CGlobalManager::GetManager()->SetCurScene(pScene1);

	// 启动窗口消息循环
	int nExitCode = CGlobalManager::GetManager()->LoopMessage();

	return nExitCode;
}

