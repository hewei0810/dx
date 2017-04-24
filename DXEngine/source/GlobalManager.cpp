#include "stdafx.h"
#include "GlobalManager.h"

CGlobalManager* CGlobalManager::sm_pManager = NULL;

CGlobalManager::CGlobalManager()
: m_pRenderer(NULL)
, m_pInputer(NULL)
, m_pCurScene(NULL)
{
	
}

CGlobalManager::~CGlobalManager()
{
	SAFE_DELETE(m_pCurScene)
	SAFE_DELETE(m_pInputer)
	SAFE_DELETE(m_pRenderer)
}

void CGlobalManager::CreateManager()
{
	if (sm_pManager == NULL)
	{
		sm_pManager = new CGlobalManager;
	}
}

void CGlobalManager::ExitManager()
{
	SAFE_DELETE(sm_pManager)	
}

bool CGlobalManager::InitManager(HINSTANCE hInst, HWND hWnd)
{
	m_hInst = hInst;
	m_hWnd = hWnd;
	::GetClientRect(m_hWnd, &m_rcRender);

	m_pRenderer = new CD3DRenderer;
	if (!m_pRenderer->CreateDevice(m_hWnd))
		return false;
	if (!m_pRenderer->InitSetting())
		return false;

	m_pInputer = new CDInputer;
	if (!m_pInputer->CreateDevice(m_hInst, m_hWnd))
		return false;

	return true;
}

void CGlobalManager::SetCurScene(CScene* pScene)
{
	SAFE_DELETE(m_pCurScene)
	m_pCurScene = pScene;
}

int CGlobalManager::LoopMessage()
{
	MSG msg = {0};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//ÏÐÊ±´¦Àí
			if (m_pCurScene)
			{
				m_pRenderer->Update(m_pCurScene);
				m_pRenderer->Render(m_pCurScene);
			}
		}
	}

	CGlobalManager::ExitManager();

	return msg.wParam;
}

LRESULT CALLBACK CGlobalManager::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bHandled = FALSE;
	CScene* pScene = GetManager()->GetCurScene();
	if (pScene)
	{
		LRESULT lRes = pScene->HandleMessage(uMsg, wParam, lParam, bHandled);
		if (bHandled)
			return lRes;
	}

	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}
