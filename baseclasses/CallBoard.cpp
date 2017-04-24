#include "stdafx.h"
#include "CallBoard.h"

CCallBoard::CCallBoard(IDirect3DDevice9* pD3DDev)
: m_pD3DDev(pD3DDev)
, m_pVB(NULL)
, m_pTex(NULL)
{
	D3DXMatrixIdentity(&m_matWorldOrigin);
	D3DXMatrixIdentity(&m_matWorldTrans);
}

CCallBoard::~CCallBoard()
{
	SAFE_RELEASE(m_pVB)
	SAFE_RELEASE(m_pTex)
}

bool CCallBoard::InitBoard(float fX, float fY, const char* pTex)
{
	if (FAILED(D3DXCreateTextureFromFileA(m_pD3DDev, pTex, &m_pTex)))
		return false;

	VERTEX_T vertices[] = 
	{
		{-fX,	0.0f,	0.0f, 0.0f, 1.0f},
		{-fX,	fY,		0.0f, 0.0f, 0.0f},
		{fX,	0.0f,	0.0f, 1.0f, 1.0f},
		{fX,	fY,		0.0f, 1.0f, 0.0f},
	};
	m_pD3DDev->CreateVertexBuffer( sizeof(vertices), 0, 
		D3DFVF_VERTEX_T, D3DPOOL_MANAGED, &m_pVB, NULL);
	void* pVertices = NULL;
	m_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0);
	memcpy(pVertices, vertices, sizeof(vertices));
	m_pVB->Unlock();

	return true;
}

void CCallBoard::SetWorld(D3DXMATRIX& matWorld)
{
	m_matWorldOrigin = matWorld;
}

void CCallBoard::UpdateBoard(D3DXMATRIX& matView)
{
	D3DXMATRIX matBoard;
	D3DXMatrixIdentity(&matBoard);
	matBoard._11 = matView._11;
	matBoard._13 = matView._13;
	matBoard._31 = matView._31;
	matBoard._33 = matView._33;
	D3DXMatrixInverse(&matBoard, NULL, &matBoard);
	m_matWorldTrans = matBoard * m_matWorldOrigin;
}

void CCallBoard::DrawBoard()
{
	m_pD3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);  
	m_pD3DDev->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );  
	m_pD3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	m_pD3DDev->SetTransform(D3DTS_WORLD, &m_matWorldTrans);
	m_pD3DDev->SetTexture(0, m_pTex);
	m_pD3DDev->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_T));
	m_pD3DDev->SetFVF(D3DFVF_VERTEX_T);
	m_pD3DDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	m_pD3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE); 
}