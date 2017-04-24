#ifndef __D3D_BASE_H__
#define __D3D_BASE_H__

class CD3DBase
{
public:
	CD3DBase(HINSTANCE hInstance, HWND hWnd);
	virtual ~CD3DBase();

public:
	bool InitD3D();
	void ReleaseD3D();
	void Update();
	void Render();

protected:
	virtual void InitSetting();
	virtual bool LoadRes() = 0;
	virtual void UnloadRes() = 0;
	virtual void OnUpdate(float fElapsedTime) = 0;
	virtual void OnRender() = 0;

	static int GetFPS();
	HRESULT Create3DText(LPCTSTR pszText, LPCTSTR pszFontName, DWORD dwSize, bool bBold, bool bItalic, LPD3DXMESH* ppMesh);

protected:
	HINSTANCE m_hInst;
	HWND m_hWnd;
	RECT m_rcRender;

	IDirect3D9* m_pD3D;
	IDirect3DDevice9* m_pD3DDev;

	D3DADAPTER_IDENTIFIER9 m_adapter;
	D3DCAPS9 m_d3dcaps;
	D3DPRESENT_PARAMETERS m_d3dpp;
};

#endif //__D3D_BASE_H__