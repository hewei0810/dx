#include "stdafx.h"
#include "SkyBox.h"

CSkyBox::CSkyBox(IDirect3DDevice9* pD3DDev)
: m_pD3DDev(pD3DDev)
, m_pVB(NULL)
, m_fLen(0.0f)
{
	D3DXMatrixIdentity(&m_matWorld);
	
	for (int i = 0; i < 6; i++)
		m_arrTexPtr[i] = NULL;
}

CSkyBox::~CSkyBox()
{
	SAFE_RELEASE(m_pVB)
	for (int i = 0; i < 6; i++)
		SAFE_RELEASE(m_arrTexPtr[i])
}

bool CSkyBox::InitSkyBox(float fLen, const char* pTexPathname[6])
{
	m_fLen = fLen;
	float r = m_fLen / 2;

	for (int i = 0; i < 6; i++)
	{	
		if (FAILED(D3DXCreateTextureFromFileA(m_pD3DDev, pTexPathname[i], &m_arrTexPtr[i])))
			return false;
	}

	m_pD3DDev->CreateVertexBuffer(24 * sizeof(VERTEX_T), 0, D3DFVF_VERTEX_T, D3DPOOL_MANAGED, &m_pVB, NULL);

	VERTEX_T vertices[] = 
	{
		// 前
		{-r,	0.0f,	r,	0.0f,	1.0f},
		{-r,	r,		r, 	0.0f,	0.0f},
		{r,		0.0f,	r,	1.0f,	1.0f},
		{r,		r,		r,	1.0f,	0.0f},

		// 后
		{r,		0.0f,	-r,	0.0f,	1.0f},
		{r,		r,		-r,	0.0f,	0.0f},
		{-r,	0.0f,	-r,	1.0f,	1.0f},
		{-r,	r,		-r,	1.0f,	0.0f},

		// 左
		{-r,	0.0f,	-r,	0.0f,	1.0f},
		{-r,	r,		-r,	0.0f,	0.0f},
		{-r,	0.0f,	r,	1.0f,	1.0f},
		{-r,	r,		r,	1.0f,	0.0f},

		// 右
		{r,		0.0f,	r,	0.0f,	1.0f},
		{r,		r,		r,	0.0f,	0.0f},
		{r,		0.0f,	-r,	1.0f,	1.0f},
		{r,		r,		-r,	1.0f,	0.0f},

		// 上
		{r,		r,		-r, 0.0f, 1.0f, },  
		{r,		r,		r,	0.0f, 0.0f, },  
		{-r,	r,		-r, 1.0f, 1.0f, }, 
		{-r,	r,		r,	1.0f, 0.0f, },

		// 下
		{-r,	0.0f,	-r, 0.0f, 1.0f, },  
		{-r,	0.0f,	r,	0.0f, 0.0f, },  
		{r,		0.0f,	-r, 1.0f, 1.0f, },  
		{r,		0.0f,	r,	1.0f, 0.0f, },
	};

	void* pBuf;
	m_pVB->Lock(0, 24 * sizeof(VERTEX_T), &pBuf, 0);
	memcpy(pBuf, vertices, sizeof(vertices));
	m_pVB->Unlock();

	return true;
}

void CSkyBox::DrawSkyBox()
{
	m_pD3DDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pD3DDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	m_pD3DDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pD3DDev->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_T));
	m_pD3DDev->SetFVF(D3DFVF_VERTEX_T);

	for (int i = 0; i < 6; i++)
	{
		m_pD3DDev->SetTexture(0, m_arrTexPtr[i]);
		m_pD3DDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, i*4, 2);
	}

	m_pD3DDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
}