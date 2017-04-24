// DX_sample2.cpp : ����Ӧ�ó������ڵ㡣
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

	// ����ȫ�ֹ�����
	CGlobalManager::CreateManager();

	// ��������ʾ������
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

	// ��ʼ��ȫ�ֹ�����
	CGlobalManager::GetManager()->InitManager(hInstance, mainWnd.GetHWnd());
	CStdString strPath = CGlobalManager::GetManager()->GetModulePath() + "res";
	CGlobalManager::GetManager()->SetResPath(strPath.GetData());

	// ���õ�ǰ����
	CMenuScene* pScene1 = new CMenuScene;
	if (!pScene1->InitScene())
		return -1;
	CGlobalManager::GetManager()->SetCurScene(pScene1);

	// ����������Ϣѭ��
	int nExitCode = CGlobalManager::GetManager()->LoopMessage();

	return nExitCode;
}

