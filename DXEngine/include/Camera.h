//Camera.h:

#pragma once

class DXENGINE_API CCamera
{
public:
	CCamera(IDirect3DDevice9* pD3DDev);
	~CCamera();

public:
	void InitViewMatrix(D3DXVECTOR3& vecPos, D3DXVECTOR3& vecMaxPos);
	void GetViewMatrix(D3DMATRIX* pMatView) { memcpy(pMatView, &m_matView, sizeof(D3DMATRIX)); }

	void TranslationRight(FLOAT fStep);
	void TranslationUp(FLOAT fStep);
	void TranslationLook(FLOAT fStep);

	void RotationRight(FLOAT fAngle);
	void RotationUp(FLOAT fAngle);
	void RotationLook(FLOAT fAngle);

private:
	void CalcAndSetViewMatrix();

private:
	IDirect3DDevice9* m_pD3DDev;

	D3DXVECTOR3 m_vecLook;
	D3DXVECTOR3 m_vecUp;
	D3DXVECTOR3 m_vecRight;
	D3DXVECTOR3 m_vecCameraPos;

	D3DMATRIX m_matView;

	D3DXVECTOR3 m_vecMaxPos;
};