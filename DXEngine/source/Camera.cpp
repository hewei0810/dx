//Camera.cpp:
#include "stdafx.h"
#include "Camera.h"

CCamera::CCamera(IDirect3DDevice9* pD3DDev)
{
	m_pD3DDev = pD3DDev;
}

CCamera::~CCamera()
{

}

void CCamera::InitViewMatrix(D3DXVECTOR3& vecPos, D3DXVECTOR3& vecMaxPos)
{
	m_vecRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_vecUp    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vecLook  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	m_vecCameraPos = vecPos;
	m_vecMaxPos = vecMaxPos;

	CalcAndSetViewMatrix();
}

void CCamera::TranslationRight(FLOAT fStep)
{
	m_vecCameraPos += m_vecRight * fStep;

	CalcAndSetViewMatrix();	
}
void CCamera::TranslationUp(FLOAT fStep)
{
	m_vecCameraPos += m_vecUp * fStep;

	CalcAndSetViewMatrix();
}

void CCamera::TranslationLook(FLOAT fStep)
{
	m_vecCameraPos += m_vecLook * fStep;

	CalcAndSetViewMatrix();
}

void CCamera::RotationRight(FLOAT fAngle)
{
	D3DXMATRIX matRot;
	D3DXMatrixRotationAxis(&matRot, &m_vecRight, fAngle);

	//D3DXVec3TransformCoord(&m_vecRight, &m_vecRight, &matRot);
	D3DXVec3TransformCoord(&m_vecUp, &m_vecUp, &matRot);
	D3DXVec3TransformCoord(&m_vecLook, &m_vecLook, &matRot);

	CalcAndSetViewMatrix();
}

void CCamera::RotationUp(FLOAT fAngle)
{
	D3DXMATRIX matRot;
	D3DXMatrixRotationAxis(&matRot, &m_vecUp, fAngle);

	D3DXVec3TransformCoord(&m_vecRight, &m_vecRight, &matRot);
	//D3DXVec3TransformCoord(&m_vecUp, &m_vecUp, &matRot);
	D3DXVec3TransformCoord(&m_vecLook, &m_vecLook, &matRot);

	CalcAndSetViewMatrix();
}

void CCamera::RotationLook(FLOAT fAngle)
{
	D3DXMATRIX matRot;
	D3DXMatrixRotationAxis(&matRot, &m_vecLook, fAngle);

	D3DXVec3TransformCoord(&m_vecRight, &m_vecRight, &matRot);
	D3DXVec3TransformCoord(&m_vecUp, &m_vecUp, &matRot);
	//D3DXVec3TransformCoord(&m_vecLook, &m_vecLook, &matRot);

	CalcAndSetViewMatrix();
}

void CCamera::CalcAndSetViewMatrix()
{
	//规范化与修正
	m_vecUp.x = 0;
	m_vecRight.y = 0;
	D3DXVec3Normalize(&m_vecLook, &m_vecLook);

	D3DXVec3Cross(&m_vecUp, &m_vecLook, &m_vecRight);
	D3DXVec3Normalize(&m_vecUp, &m_vecUp);
	
	D3DXVec3Cross(&m_vecRight, &m_vecUp, &m_vecLook);
	D3DXVec3Normalize(&m_vecRight, &m_vecRight);

	if (m_vecCameraPos.x < -m_vecMaxPos.x)
		m_vecCameraPos.x = -m_vecMaxPos.x;

	if (m_vecCameraPos.y > m_vecMaxPos.x)
		m_vecCameraPos.x = m_vecMaxPos.x;

	if (m_vecCameraPos.y < 10)
		m_vecCameraPos.y = 10;

	if (m_vecCameraPos.y > m_vecMaxPos.y)
		m_vecCameraPos.y = m_vecMaxPos.y;

	if (m_vecCameraPos.z < -m_vecMaxPos.z)
		m_vecCameraPos.z = -m_vecMaxPos.z;

	if (m_vecCameraPos.z > m_vecMaxPos.z)
		m_vecCameraPos.z = m_vecMaxPos.z;

	// 计算观察矩阵
	m_matView._11 = m_vecRight.x;
	m_matView._12 = m_vecUp.x;
	m_matView._13 = m_vecLook.x;
	m_matView._14 = 0.0f;

	m_matView._21 = m_vecRight.y;
	m_matView._22 = m_vecUp.y;
	m_matView._23 = m_vecLook.y;
	m_matView._24 = 0.0f;

	m_matView._31 = m_vecRight.z;
	m_matView._32 = m_vecUp.z;
	m_matView._33 = m_vecLook.z;
	m_matView._34 = 0.0f;

	m_matView._41 = -D3DXVec3Dot(&m_vecRight, &m_vecCameraPos);
	m_matView._42 = -D3DXVec3Dot(&m_vecUp, &m_vecCameraPos);
	m_matView._43 = -D3DXVec3Dot(&m_vecLook, &m_vecCameraPos);
	m_matView._44 = 1.0f;

	// 设置观察矩阵
	m_pD3DDev->SetTransform(D3DTS_VIEW, &m_matView);
}