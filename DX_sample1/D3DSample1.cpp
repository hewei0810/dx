#include "stdafx.h"
#include "D3DSample1.h"

#define MAX_X 50000.0f
#define MAX_Y 50000.0f
#define MAX_Z 50000.0f

CD3DSample1::CD3DSample1(HINSTANCE hInstance, HWND hWnd)
: CD3DBase(hInstance, hWnd)
, m_pDInput(NULL)
, m_pCamera(NULL)
, m_pFont1(NULL)
//, m_pMesh3DText(NULL)
, m_pSkyBox(NULL)
//, m_pSnow(NULL)
, m_pBoardGirl(NULL)
//, m_pMeshMiki(NULL)
, m_pBoneMesh(NULL)
{

}

CD3DSample1::~CD3DSample1()
{
	
}

bool CD3DSample1::LoadRes()
{
	// 创建输入设备
	m_pDInput = new CDInputer;
	m_pDInput->CreateDevice(m_hInst, m_hWnd);

	// 创建摄像机
	m_pCamera = new CCamera(m_pD3DDev);
	m_pCamera->InitViewMatrix(D3DXVECTOR3(0.0f, 100.0f, -1000.0f), D3DXVECTOR3(MAX_X-10, MAX_Y-10, MAX_Z-10));

	// 创建GUI
	m_pGui = new CD3DGui(m_pD3DDev);
	if (!m_pGui->AddImage(m_rcRender, ".\\res\\bg\\maingui.jpg"))
		return false;
	RECT rc1 = {300, 100, 500, 200};
	if (!m_pGui->AddStatic(rc1, "刺客信条", 32, ".\\res\\bg\\bg3.jpg"))
		return false;
	RECT rc2 = {300, 250, 500, 350};
	if (!m_pGui->AddButton(rc2, "res\\bg\\btn_normal.png", "res\\bg\\btn_focused.png", "res\\bg\\btn_pushed.png",
		"res\\bg\\btn_disabled.png", "开始游戏"))
		return false;

	// 创建字体
	if ( FAILED(D3DXCreateFont(m_pD3DDev, 28, 0, 0, 1, FALSE, 
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
					0, _T("宋体"), &m_pFont1)) )
		return false;

	// 创建3D字
	//if( FAILED(Create3DText(_T("3D艺术字"), _T("宋体"), 0, false, false, &m_pMesh3DText)) )
	//	return false;

	// 创建天空
	m_pSkyBox = new CSkyBox(m_pD3DDev);
	const char* pathname[6];
	//pathname[0] = ".\\res\\skybox\\front.jpg";
	//pathname[1] = ".\\res\\skybox\\back.jpg";
	//pathname[2] = ".\\res\\skybox\\left.jpg";
	//pathname[3] = ".\\res\\skybox\\right.jpg";
	//pathname[4] = ".\\res\\skybox\\top.jpg";
	//pathname[5] = ".\\res\\skybox\\bottom.jpg";

	pathname[0] = ".\\res\\skybox\\front.bmp";
	pathname[1] = ".\\res\\skybox\\back.bmp";
	pathname[2] = ".\\res\\skybox\\left.bmp";
	pathname[3] = ".\\res\\skybox\\right.bmp";
	pathname[4] = ".\\res\\skybox\\top.bmp";
	pathname[5] = ".\\res\\skybox\\bottom.bmp";

	if (!m_pSkyBox->InitSkyBox(2 * max(MAX_X, max(MAX_Y, MAX_Z)), pathname))
		return false;

	// 创建雪花
	//m_pSnow = new CSnowParticle(m_pD3DDev);
	//if (!m_pSnow->InitSnows(1000, 2*MAX_X, 2*MAX_Y, 2*MAX_Z))
	//	return false;

	// 创建公告板
	m_pBoardGirl = new CCallBoard(m_pD3DDev);
	if (!m_pBoardGirl->InitBoard(100.0f, 300.0f, ".\\res\\girl.png"))
		return false;

	// 创建人物
	//m_pMeshMiki = new CMeshModel(m_pD3DDev);
	//if (!m_pMeshMiki->InitMesh(".\\res\\miki\\miki.X"))
	//	return false;
	//D3DXMATRIX matWorldMiki;
	//D3DXMatrixTranslation(&matWorldMiki, 0.0f, 81.531906f, 0.0f);
	//m_pMeshMiki->SetWorld(matWorldMiki);

	// 创建骨骼动画
	m_pBoneMesh = new CBoneMesh(m_pD3DDev);
	if (!m_pBoneMesh->InitMesh("lxq.x"))
		return false;
	if (!m_pBoneMesh->SetAnimSet(1, "sworddance"))
		return false;

	return true;
}

void CD3DSample1::UnloadRes()
{
	SAFE_DELETE(m_pDInput)
	SAFE_DELETE(m_pCamera)

	SAFE_RELEASE(m_pFont1)
	//SAFE_RELEASE(m_pMesh3DText)

	SAFE_DELETE(m_pSkyBox)
	//SAFE_DELETE(m_pSnow)
	SAFE_DELETE(m_pBoardGirl)
	//SAFE_DELETE(m_pMeshMiki)
	SAFE_DELETE(m_pBoneMesh)
}

void CD3DSample1::OnUpdate(float fElapsedTime)
{
	// 更新输入控制
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

	// 更新雪花
	//m_pSnow->UpdateSnows(fElapsedTime);

	// 更新公告板
	D3DXMATRIX matView;
	m_pCamera->GetViewMatrix(&matView);
	m_pBoardGirl->UpdateBoard(matView);

	// 更新骨骼动画
	m_pBoneMesh->UpdateFrame(fElapsedTime);
}

void CD3DSample1::OnRender()
{
	// 画GUI
	//m_pGui->DrawGui();

	// 标题栏
	m_pFont1->DrawTextA(NULL, m_adapter.Description, -1, &m_rcRender, DT_TOP | DT_LEFT, D3DCOLOR_XRGB(255, 255, 0));
	m_pFont1->DrawTextA(NULL, "D3DSample1", -1, &m_rcRender, DT_TOP | DT_CENTER, D3DCOLOR_XRGB(0, 255, 0));
	char szFPS[20];
	sprintf_s(szFPS, 20, "FPS:%d", GetFPS());
	m_pFont1->DrawTextA(NULL, szFPS, -1, &m_rcRender, DT_TOP | DT_RIGHT, D3DCOLOR_XRGB(0, 0, 255));

	// 3D字
	//D3DXMATRIX matWorld;
	//D3DXMatrixIdentity(&matWorld);
	//D3DXMATRIX matScale;
	//D3DXMatrixScaling(&matScale, 10.0f, 10.0f, 10.0f);
	//D3DXMATRIX matMove;
	//D3DXMatrixTranslation(&matMove, -30.0f, 0.0f, 0.0f);
	//D3DXMATRIX matRotX;
	//D3DXMatrixRotationX(&matRotX, GetTickCount()/100 * D3DX_PI / 18);
	//matWorld = matWorld * matScale * matMove * matRotX ;
	//m_pD3DDev->SetTransform(D3DTS_WORLD, &matWorld);

	//D3DMATERIAL9 mtrl;
	//ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
	//mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	//mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	//mtrl.Diffuse.b = mtrl.Ambient.b = 0.0f;
	//mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	//m_pD3DDev->SetMaterial( &mtrl );

	//m_pD3DDev->SetTexture(0, NULL);

	//m_pMesh3DText->DrawSubset(0);

	// 画天空
	m_pSkyBox->DrawSkyBox();

	// 画雪花
	//m_pSnow->DrawSnows();

	// 画公告板
	m_pBoardGirl->DrawBoard();

	// 画人物
	//m_pMeshMiki->DrawMesh();

	// 画骨骼动画
	m_pBoneMesh->DrawFrame(); 
}