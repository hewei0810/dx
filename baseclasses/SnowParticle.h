// SnowParticle.h

#pragma once

struct SNOW_PARTICLE
{
	FLOAT x,y,z;
	FLOAT RotX,RotY;
	FLOAT FallSpeed;
	FLOAT RotSpeed;
	int TexIndex;
};

class CSnowParticle
{
public:
	CSnowParticle(IDirect3DDevice9* pD3DDev);
	~CSnowParticle();

public:
	bool InitSnows(int nNum, int nX, int nY, int nZ);
	void UpdateSnows(FLOAT fElapsedTime);
	void DrawSnows();

private:
	int m_nNum, m_nX, m_nY, m_nZ;

	IDirect3DDevice9* m_pD3DDev;
	IDirect3DVertexBuffer9* m_pVB;
	IDirect3DTexture9* m_arrTexPtr[6];

	SNOW_PARTICLE* m_pSnows;
};