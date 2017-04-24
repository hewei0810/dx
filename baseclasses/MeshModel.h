// MeshLoader.h:

#pragma once

class CMeshModel
{
public:
	CMeshModel(IDirect3DDevice9* pD3DDev);
	~CMeshModel();

public:
	bool InitMesh(const char* pathname);
	void SetWorld(D3DXMATRIX& matWorld) {m_matWorld = matWorld;}
	void DrawMesh();

protected:
private:
	IDirect3DDevice9* m_pD3DDev;
	D3DXMATRIX m_matWorld;

	ID3DXMesh* m_pMesh;
	DWORD m_dwNumMaterials;
	D3DMATERIAL9* m_pArrMtrls;
	LPDIRECT3DTEXTURE9* m_pArrTexPtr;
};