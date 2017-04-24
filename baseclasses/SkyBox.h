#ifndef __SKY_BOX_H__
#define __SKY_BOX_H__

class CSkyBox
{
public:
	CSkyBox(IDirect3DDevice9* pD3DDev);
	~CSkyBox();

public:
	void SetWorld(D3DXMATRIX& matWorld) {m_matWorld = matWorld;}
	bool InitSkyBox(float fLen, const char* pTexPathname[6]);
	void DrawSkyBox();

protected:
private:
	IDirect3DDevice9* m_pD3DDev;
	D3DXMATRIX m_matWorld;

	IDirect3DVertexBuffer9* m_pVB;
	IDirect3DTexture9* m_arrTexPtr[6];
	float m_fLen;
};

#endif // __SKY_BOX_H__