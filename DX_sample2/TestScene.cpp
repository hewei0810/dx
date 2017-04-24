#include "stdafx.h"
#include "TestScene.h"

#define MAX_X 50000.0f
#define MAX_Y 50000.0f
#define MAX_Z 50000.0f

LRESULT notify(const char* szName, EEventType type)
{
	if (stricmp(szName, "game") == 0)
	{
		if (type == EventType_BLICK)
		{
			CMainScene* pScene = new CMainScene;
			if (!pScene->InitScene())
				return -1;
			CGlobalManager::GetManager()->SetCurScene(pScene);
		}
	}

	return 0;
}

CMenuScene::CMenuScene()
: m_pGui(NULL)
{
	
}

CMenuScene::~CMenuScene()
{
	SAFE_DELETE(m_pGui)
}

bool CMenuScene::InitScene()
{
	IDirect3DDevice9* m_pD3DDev = CGlobalManager::GetManager()->GetRenderer()->GetD3DDev();

	// 创建GUI
	m_pGui = new CD3DGui(m_pD3DDev, notify);
	RECT rc = {0,0,1280,720};
	if (!m_pGui->AddImage(rc, "77\\77k.jpg"))
		return false;
	RECT rc1 = {0, 0, 1280, 500};
	if (!m_pGui->AddStatic(rc1, "人生中有那么多后悔的事情\n但是,从不后悔遇见你\n...\n你给予我的一切,慢慢成为我的一部分\n美妙与痛苦,都根深蒂固\n...\n七夕节快乐,陈悦！", 48))
		return false;
	RECT rc2 = {1000, 600, 1280, 720};
	if (!m_pGui->AddButton("game", rc2, "control\\22.jpg", NULL, NULL, NULL, "开启爱的篇章>>>", 32))
		return false;
	RECT rc3 = {0,300,300,720};
	if (!m_pGui->AddImage(rc3, "77\\cy2.jpg"))
		return false;


	return true;
}

bool CMenuScene::UpdateScene(float fElapsedTime)
{
	return true;
}

bool CMenuScene::DrawScene()
{
	// 画GUI
	m_pGui->DrawGui();

	return true;
}

LRESULT CMenuScene::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_pGui->HandleMessage(uMsg, wParam, lParam, bHandled);

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
CMainScene::CMainScene()
: m_pCamera(NULL)
, m_pSkyBox(NULL)
, m_pBoneMesh(NULL)
{

}

CMainScene::~CMainScene()
{
	SAFE_DELETE(m_pCamera)
	SAFE_DELETE(m_pSkyBox)
	SAFE_DELETE(m_pBoneMesh)
}

bool CMainScene::InitScene()
{
	IDirect3DDevice9* m_pD3DDev = CGlobalManager::GetManager()->GetRenderer()->GetD3DDev();

	// 创建摄像机
	m_pCamera = new CCamera(m_pD3DDev);
	m_pCamera->InitViewMatrix(D3DXVECTOR3(0.0f, 100.0f, -1000.0f), D3DXVECTOR3(MAX_X-10, MAX_Y-10, MAX_Z-10));

	// 创建天空
	m_pSkyBox = new CSkyBox(m_pD3DDev);
	const char* pathname[6];
	//pathname[0] = ".\\res\\skybox\\front.jpg";
	//pathname[1] = ".\\res\\skybox\\back.jpg";
	//pathname[2] = ".\\res\\skybox\\left.jpg";
	//pathname[3] = ".\\res\\skybox\\right.jpg";
	//pathname[4] = ".\\res\\skybox\\top.jpg";
	//pathname[5] = ".\\res\\skybox\\bottom.jpg";

	pathname[0] = "skybox\\front.bmp";
	pathname[1] = "skybox\\back.bmp";
	pathname[2] = "skybox\\left.bmp";
	pathname[3] = "skybox\\right.bmp";
	pathname[4] = "skybox\\top.bmp";
	pathname[5] = "skybox\\bottom.bmp";

	if (!m_pSkyBox->InitSkyBox(2 * max(MAX_X, max(MAX_Y, MAX_Z)), pathname))
		return false;

	// 创建骨骼动画
	m_pBoneMesh = new CBoneMesh(m_pD3DDev);
	if (!m_pBoneMesh->InitMesh("zou.x"))
		return false;
	if (!m_pBoneMesh->SetAnimSet(1, "Anim-1"))
		return false;

	return true;
}

bool CMainScene::UpdateScene(float fElapsedTime)
{
	// 更新输入控制
	CDInputer* m_pDInput = CGlobalManager::GetManager()->GetInputer();
	if (m_pDInput->GetKeyInput())
	{
		if (m_pDInput->IsKeyDown(DIK_ESCAPE))
		{
			PostQuitMessage(0);
		}

		if (m_pDInput->IsKeyDown(DIK_W))
		{
			m_pCamera->TranslationLook(1.0f);
		}

		if (m_pDInput->IsKeyDown(DIK_S))
		{
			m_pCamera->TranslationLook(-1.0f);
		}

		if (m_pDInput->IsKeyDown(DIK_A))
		{
			m_pCamera->TranslationRight(-1.0f);
		}

		if (m_pDInput->IsKeyDown(DIK_D))
		{
			m_pCamera->TranslationRight(1.0f);
		}

		if (m_pDInput->IsKeyDown(DIK_SPACE))
		{
			m_pCamera->TranslationUp(1.0f);
		}

		if (m_pDInput->IsKeyDown(DIK_LSHIFT))
		{
			m_pCamera->TranslationUp(-1.0f);
		}
	}

	if (m_pDInput->GetMouseInput())
	{
		if (m_pDInput->MouseX()/3)
		{
			m_pCamera->RotationUp(m_pDInput->MouseX()/3 * D3DX_PI/1800);
		}

		if (m_pDInput->MouseY()/3)
		{
			m_pCamera->RotationRight(m_pDInput->MouseY()/3 * D3DX_PI/1800);
		}
	}

	// 更新骨骼动画
	m_pBoneMesh->UpdateFrame(fElapsedTime);

	return true;
}

bool CMainScene::DrawScene()
{
	// 画天空
	m_pSkyBox->DrawSkyBox();

	// 画骨骼动画
	m_pBoneMesh->DrawFrame();

	return true;
}

LRESULT CMainScene::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}