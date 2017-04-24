// MeshLoader.cpp

#include "stdafx.h"
#include "MeshModel.h"

CMeshModel::CMeshModel(IDirect3DDevice9* pD3DDev)
: m_pD3DDev(pD3DDev)
, m_pMesh(NULL)
, m_dwNumMaterials(0)
{
	D3DXMatrixIdentity(&m_matWorld);
}

CMeshModel::~CMeshModel()
{
	SAFE_DELETE_ARRAY(m_pArrMtrls)
	for (DWORD i = 0; i < m_dwNumMaterials; i++)
	{
		SAFE_RELEASE(m_pArrTexPtr[i])
	}

	SAFE_RELEASE(m_pMesh)

	m_dwNumMaterials = 0;
}

bool CMeshModel::InitMesh(const char* pathname)
{
	char szPath[MAX_PATH];
	strcpy(szPath, pathname);
	char* pos = strrchr(szPath, '\\');
	if (!pos)
		return false;
	*(pos+1) = '\0';

	CComPtr<ID3DXBuffer> pBufferMaterials = NULL;
	if ( FAILED(D3DXLoadMeshFromXA(pathname, D3DXMESH_MANAGED, m_pD3DDev, NULL, &pBufferMaterials, NULL, &m_dwNumMaterials, &m_pMesh) ) )
		return false;

	m_pArrMtrls = new D3DMATERIAL9[m_dwNumMaterials];
	m_pArrTexPtr = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];

	D3DXMATERIAL* pMaterials = (D3DXMATERIAL*)pBufferMaterials->GetBufferPointer();

	LPDIRECT3DTEXTURE9 pTex = NULL;
	for (DWORD i = 0; i < m_dwNumMaterials; i++)
	{
		memcpy(&m_pArrMtrls[i], &pMaterials->MatD3D, sizeof(D3DMATERIAL9));
		char szPathname[MAX_PATH];
		strcpy(szPathname, szPath);
		strcat(szPathname, pMaterials[i].pTextureFilename);
		if ( FAILED(D3DXCreateTextureFromFileA(m_pD3DDev, szPathname, &pTex)) )
			return false;
		m_pArrTexPtr[i] = pTex;
	}

	return true;
}

void CMeshModel::DrawMesh()
{
	m_pD3DDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	for (DWORD i = 0; i < m_dwNumMaterials; i++)
	{
		m_pD3DDev->SetMaterial(&m_pArrMtrls[i]);
		m_pD3DDev->SetTexture(0, m_pArrTexPtr[i]);
		m_pMesh->DrawSubset(i);
	}
}