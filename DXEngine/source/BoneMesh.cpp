#include "stdafx.h"
#include "BoneMesh.h"

CBoneMesh::CBoneMesh(IDirect3DDevice9* pD3DDev)
: m_pD3DDev(pD3DDev)
, m_pMeshHier(NULL)
, m_pFrameRoot(NULL)
, m_pAnimCtrl(NULL)
{
	D3DXMatrixIdentity(&m_matWorld);
}

CBoneMesh::~CBoneMesh()
{
	SAFE_DELETE(m_pMeshHier)
	SAFE_DELETE(m_pFrameRoot)
	SAFE_RELEASE(m_pAnimCtrl)
}

bool CBoneMesh::InitMesh(const char* pathname)
{
	CStdString strTmp = CGlobalManager::GetManager()->GetResPath() + pathname;
	m_pMeshHier = new CAllocateHierarchy;
	if (FAILED(D3DXLoadMeshHierarchyFromXA(strTmp.GetData(), D3DXMESH_MANAGED, m_pD3DDev, m_pMeshHier, NULL, &m_pFrameRoot, &m_pAnimCtrl)))
		return false;
	::SetupBoneMatrixPointers(m_pFrameRoot, m_pFrameRoot);

	return true;
}

bool CBoneMesh::SetAnimSet(UINT track, const char* name)
{
	ID3DXAnimationSet* m_pAnimSet;
	if (FAILED(m_pAnimCtrl->GetAnimationSetByName(name, &m_pAnimSet)))
		return false;

	m_pAnimCtrl->SetTrackAnimationSet(track, m_pAnimSet);

	return true;
}

void CBoneMesh::UpdateFrame(float fElapsedTime)
{
	m_pAnimCtrl->AdvanceTime(fElapsedTime, NULL);
}

void CBoneMesh::DrawFrame()
{
	::UpdateFrameMatrices(m_pFrameRoot, &m_matWorld);
	::DrawFrame(m_pD3DDev, m_pFrameRoot);
}