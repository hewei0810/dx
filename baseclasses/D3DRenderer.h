#ifndef __D3D_RENDERER_H__
#define __D3D_RENDERER_H__

#include "InputDev.h"
#include "Scene.h"

class CD3DRenderer
{
private:
	CD3DRenderer(HINSTANCE hInstance, HWND hWnd);
	~CD3DRenderer();

public:
	static bool InitInstance(HINSTANCE hInstance, HWND hWnd);
	static void ExitInstance();
	static CD3DRenderer* GetInstance() {return sm_pD3DRenderer;}

public:
	IDirect3DDevice9* GetD3DDev() {return m_pD3DDev;}
	CDInput* GetInputDev() {return m_pInputDev;}
	
public:
	bool SetScene(CScene* pScene);
	bool Update();
	bool Render();

protected:
	bool CreateDevice();
	bool InitSetting();

private:
	static CD3DRenderer* sm_pD3DRenderer;

	HINSTANCE m_hInst;
	HWND m_hWnd;
	RECT m_rcRender;

	D3DADAPTER_IDENTIFIER9 m_adapter;
	D3DCAPS9 m_d3dcaps;
	D3DPRESENT_PARAMETERS m_d3dpp;

	IDirect3DDevice9* m_pD3DDev;
	CDInput* m_pInputDev;

	CScene* m_pScene;
};

#endif // __D3D_RENDERER_H__