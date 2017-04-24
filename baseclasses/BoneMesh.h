#ifndef __BONE_MESH_H__
#define __BONE_MESH_H__

#include "AllocateHierarchyClass.h"

class CBoneMesh
{
public:
	CBoneMesh(IDirect3DDevice9* pD3DDev);
	~CBoneMesh();

public:
	bool InitMesh(const char* pathname);
	void SetWorld(D3DXMATRIX& matWorld) {m_matWorld = matWorld;}
	bool SetAnimSet(UINT track, const char* name);
	void UpdateFrame(float fElapsedTime);
	void DrawFrame();

protected:
private:
	IDirect3DDevice9* m_pD3DDev;
	D3DXMATRIX m_matWorld;

	CAllocateHierarchy* m_pMeshHier;
	D3DXFRAME* m_pFrameRoot;
	ID3DXAnimationController* m_pAnimCtrl;
};

#endif // __BONE_MESH_H__