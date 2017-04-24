// SnowParticle.cpp

#include "stdafx.h"
#include "SnowParticle.h"

CSnowParticle::CSnowParticle(IDirect3DDevice9* pD3DDev)
: m_pD3DDev(pD3DDev)
, m_pVB(NULL)
, m_pSnows(NULL)
{
	for (int i = 0; i < 6; i++)
		m_arrTexPtr[i] = NULL;
}

CSnowParticle::~CSnowParticle()
{
	SAFE_RELEASE(m_pVB)
	for (int i = 0; i < 6; i++)
		SAFE_RELEASE(m_arrTexPtr[i])

	SAFE_DELETE_ARRAY(m_pSnows)
}

bool CSnowParticle::InitSnows(int nNum, int nX, int nY, int nZ)
{	
	m_nNum = nNum;
	m_nX = nX;
	m_nY = nY;
	m_nZ = nZ;

	if (FAILED(D3DXCreateTextureFromFileA(m_pD3DDev, "res\\snow\\snow1.jpg", &m_arrTexPtr[0])))
		return false;
	if (FAILED(D3DXCreateTextureFromFileA(m_pD3DDev, "res\\snow\\snow2.jpg", &m_arrTexPtr[1])))
		return false;
	if (FAILED(D3DXCreateTextureFromFileA(m_pD3DDev, "res\\snow\\snow3.jpg", &m_arrTexPtr[2])))
		return false;
	if (FAILED(D3DXCreateTextureFromFileA(m_pD3DDev, "res\\snow\\snow4.jpg", &m_arrTexPtr[3])))
		return false;
	if (FAILED(D3DXCreateTextureFromFileA(m_pD3DDev, "res\\snow\\snow5.jpg", &m_arrTexPtr[4])))
		return false;
	if (FAILED(D3DXCreateTextureFromFileA(m_pD3DDev, "res\\snow\\snow6.jpg", &m_arrTexPtr[5])))
		return false;

	m_pSnows = new SNOW_PARTICLE[m_nNum];

	srand(GetTickCount());

	for (int i = 0; i < nNum; i++)
	{
		m_pSnows[i].x = (FLOAT)(rand()%m_nX - m_nX/2);
		m_pSnows[i].y = (FLOAT)(rand()%m_nY - m_nY/2);
		m_pSnows[i].z = (FLOAT)(rand()%m_nZ - m_nZ/2);
		m_pSnows[i].RotX = rand()%100 * 2*D3DX_PI/100.0f;
		m_pSnows[i].RotY = rand()%100 * 2*D3DX_PI/100.0f;
		m_pSnows[i].FallSpeed = 20.0f + rand()%30;
		m_pSnows[i].RotSpeed = 2*D3DX_PI/36 + rand()%30 * 2*D3DX_PI/360;
		m_pSnows[i].TexIndex = rand()%6;
	}

	m_pD3DDev->CreateVertexBuffer(4*sizeof(VERTEX_NT), 0, D3DFVF_VERTEX_NT, D3DPOOL_MANAGED, &m_pVB, NULL);

	VERTEX_NT* pVertices = NULL;
	m_pVB->Lock(0, 4*sizeof(VERTEX_NT), (void**)&pVertices, 0);
	pVertices[0] = VERTEX_NT(-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	pVertices[1] = VERTEX_NT(-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	pVertices[2] = VERTEX_NT(1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	pVertices[3] = VERTEX_NT(1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	m_pVB->Unlock();

	return true;
}

void CSnowParticle::UpdateSnows(FLOAT fElapsedTime)
{
	for (int i = 0; i < m_nNum; i++)
	{
		m_pSnows[i].y -= m_pSnows[i].FallSpeed * fElapsedTime;
		if (m_pSnows[i].y < 0)
			m_pSnows[i].y = m_nY/2.0f;
		m_pSnows[i].RotX += m_pSnows[i].RotSpeed * fElapsedTime;
		m_pSnows[i].RotY += m_pSnows[i].RotSpeed * fElapsedTime;
	}
}

void CSnowParticle::DrawSnows()
{
	//ÉèÖÃAlpha»ìºÏ
	m_pD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pD3DDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int i = 0; i < m_nNum; i++)
	{
		D3DXMATRIX matWorld, matRotX, matRotY, matTrans;
		D3DXMatrixRotationX(&matRotX, m_pSnows[i].RotX);
		D3DXMatrixRotationY(&matRotY, m_pSnows[i].RotY);
		D3DXMatrixTranslation(&matTrans, m_pSnows[i].x, m_pSnows[i].y, m_pSnows[i].z);
		matWorld = matRotX * matRotY * matTrans;
		m_pD3DDev->SetTransform(D3DTS_WORLD, &matWorld);

		m_pD3DDev->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_NT));
		m_pD3DDev->SetFVF(D3DFVF_VERTEX_NT);
		D3DMATERIAL9 mtrl;
		ZeroMemory(&mtrl, sizeof(mtrl));
		mtrl.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_pD3DDev->SetMaterial(&mtrl);
		m_pD3DDev->SetTexture(0, m_arrTexPtr[m_pSnows[i].TexIndex]);
		m_pD3DDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	m_pD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}