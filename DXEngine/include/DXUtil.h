// DXUtil.h

#pragma once

#include <atlbase.h> // CComPtr的使用声明

// 纹理顶点
struct VERTEX_T
{
	float x,y,z;
	float u,v;
};
#define D3DFVF_VERTEX_T (D3DFVF_XYZ | D3DFVF_TEX1)

// 法线纹理顶点
struct VERTEX_NT
{
	float x,  y,  z;
	float nx, ny, nz;
	float u,  v;
	VERTEX_NT(float x, float y, float z, 
		float nx, float ny, float nz, float u, float v)
	{
		this->x  = x;  this->y  = y;  this->z  = z;
		this->nx = nx; this->ny = ny; this->nz = nz;
		this->u  = u;  this->v  = v;
	}
};
#define D3DFVF_VERTEX_NT  (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

// 带颜色的屏幕坐标
struct VERTEX_RD
{
	float x, y, z, rhw;
	DWORD diffuse;
};
#define D3DFVF_VERTEX_RD  (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

// 带颜色、纹理的屏幕坐标
struct VERTEX_RDT
{
	float x, y, z, rhw;
	DWORD diffuse;
	float u,v;
};
#define D3DFVF_VERTEX_RDT (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)