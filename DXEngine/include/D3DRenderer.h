#ifndef __D3D_RENDERER_H__
#define __D3D_RENDERER_H__

class CScene;

class DXENGINE_API CD3DRenderer
{
public:
	CD3DRenderer();
	~CD3DRenderer();

public:
	IDirect3DDevice9* GetD3DDev() const {return m_pD3DDev;}
	
public:
	bool CreateDevice(HWND hWnd);
	bool InitSetting();

public:
	bool Update(CScene* pScene);
	bool Render(CScene* pScene);

private:
	RECT m_rcRender;
	D3DADAPTER_IDENTIFIER9 m_adapter;
	D3DCAPS9 m_d3dcaps;
	D3DPRESENT_PARAMETERS m_d3dpp;
	IDirect3DDevice9* m_pD3DDev;
};

#endif // __D3D_RENDERER_H__