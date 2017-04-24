#include "stdafx.h"
#include "D3DRenderer.h"

CD3DRenderer::CD3DRenderer()
: m_pD3DDev(NULL)
{
	
}

CD3DRenderer::~CD3DRenderer()
{
	SAFE_RELEASE(m_pD3DDev)
}

bool CD3DRenderer::CreateDevice(HWND hWnd)
{
	if (!IsWindow(hWnd))
		return false;

	::GetClientRect(hWnd, &m_rcRender);

	// 获取D3D9接口
	CComPtr<IDirect3D9> pD3D = Direct3DCreate9(DIRECT3D_VERSION);
	if (!pD3D)
		return false;

	// 获取显卡信息
	pD3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &m_adapter);

	// 获取设备能力
	ZeroMemory(&m_d3dcaps, sizeof(m_d3dcaps));
	if ( FAILED(pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_d3dcaps)) )
		return false;

	int vp = 0;
	if (m_d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// 设置显示参数
	m_d3dpp.BackBufferWidth            = m_rcRender.right;  
	m_d3dpp.BackBufferHeight           = m_rcRender.bottom;
	m_d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;  
	m_d3dpp.BackBufferCount            = 1;  
	m_d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;  
	m_d3dpp.MultiSampleQuality         = 0;  
	m_d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD;   
	m_d3dpp.hDeviceWindow              = hWnd;  
	m_d3dpp.Windowed                   = true;  
	m_d3dpp.EnableAutoDepthStencil     = true;   
	m_d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;  
	m_d3dpp.Flags                      = 0;  
	m_d3dpp.FullScreen_RefreshRateInHz = 0;  
	m_d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	// 创建D3D设备
	if ( FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &m_d3dpp, &m_pD3DDev)) )
		return false;

	return true;
}

bool CD3DRenderer::InitSetting()
{
	// 设置投影矩阵
	D3DXMATRIX matPrj;
	D3DXMatrixPerspectiveFovLH(&matPrj, D3DX_PI / 4.0f, (float)m_rcRender.right/m_rcRender.bottom, 1.0f, 1000000.0f);
	m_pD3DDev->SetTransform(D3DTS_PROJECTION, &matPrj);

	// 设置视口
	D3DVIEWPORT9 vp;
	vp.X      = 0;        
	vp.Y      = 0;       
	vp.Width  = m_rcRender.right;  
	vp.Height = m_rcRender.bottom;   
	vp.MinZ   = 0.0f;  
	vp.MaxZ   = 1.0f;
	m_pD3DDev->SetViewport(&vp);

	// 归一化法线
	m_pD3DDev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	// 设置背面消隐
	m_pD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); 

	//设置填充模式
	m_pD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	// 设置采样过滤方式
	m_pD3DDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	// 设置纹理颜色混合
	m_pD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

	//设置光照
	m_pD3DDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pD3DDev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(36,36,36));
	m_pD3DDev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

	D3DLIGHT9 light1, light2, light3;
	//点光源
	memset(&light1, 0, sizeof(D3DLIGHT9));
	light1.Type = D3DLIGHT_POINT;
	light1.Ambient = D3DXCOLOR(0.8f,0.8f,0.8f,1.0f);
	light1.Diffuse = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	light1.Specular = D3DXCOLOR(0.3f,0.3f,0.3f,1.0f);
	light1.Position = D3DXVECTOR3(0.0f, 200.0f, 0.0f); //光源位于y正方向远处
	light1.Range = 300.0f;
	light1.Attenuation0 = 1.0f;
	light1.Attenuation1 = 0.0f;
	light1.Attenuation2 = 0.0f;

	//方向光源
	memset(&light2, 0, sizeof(D3DLIGHT9));
	light2.Type = D3DLIGHT_DIRECTIONAL;
	light2.Ambient = D3DXCOLOR(0.5f,0.5f,0.5f,1.0f);
	light2.Diffuse = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	light2.Specular = D3DXCOLOR(0.3f,0.3f,0.3f,1.0f);
	light2.Direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f); //x正方向

	//聚光灯
	memset(&light3, 0, sizeof(D3DLIGHT9));
	light3.Type = D3DLIGHT_SPOT; 
	light3.Ambient       = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);  
	light3.Diffuse       = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  
	light3.Specular      = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.3f);
	light3.Position      = D3DXVECTOR3(100.0f, 100.0f, 100.0f);  //光源位于前方右上角处
	light3.Direction     = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);     
	light3.Range         = 300.0f;
	light3.Attenuation0  = 1.0f;  
	light3.Attenuation1  = 0.0f;  
	light3.Attenuation2  = 0.0f;    
	light3.Falloff       = 0.1f;  
	light3.Phi           = D3DX_PI / 3.0f;  
	light3.Theta         = D3DX_PI / 6.0f; 

	m_pD3DDev->SetLight(1, &light1);
	m_pD3DDev->SetLight(2, &light2);
	m_pD3DDev->SetLight(3, &light3);
	m_pD3DDev->LightEnable(1, TRUE);
	//m_pD3DDev->LightEnable(2, TRUE);
	//m_pD3DDev->LightEnable(3, TRUE);

	return true;
}

bool CD3DRenderer::Update(CScene* pScene)
{
	static DWORD dwLastTime = GetTickCount(), dwCurTime = GetTickCount();
	static FLOAT fElapsedTime = 0.0f;
	dwCurTime = GetTickCount();
	fElapsedTime = (dwCurTime - dwLastTime) / 1000.0f;
	dwLastTime = dwCurTime;

	pScene->UpdateScene(fElapsedTime);

	return true;
}

bool CD3DRenderer::Render(CScene* pScene)
{
	m_pD3DDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(50,100,250), 1.0f, 0);

	m_pD3DDev->BeginScene();

	pScene->DrawScene();

	m_pD3DDev->EndScene();

	m_pD3DDev->Present(NULL, NULL, NULL, NULL);

	return true;
}