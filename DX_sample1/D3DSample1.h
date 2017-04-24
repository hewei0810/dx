#ifndef __D3D_SAMPLE1_H__
#define __D3D_SAMPLE1_H__

#include "D3DBase.h"
#include "Camera.h"
#include "InputDev.h"

#include "D3DGui.h"

#include "SkyBox.h"
#include "SnowParticle.h"

#include "CallBoard.h"
#include "MeshModel.h"
#include "BoneMesh.h"

class CD3DSample1 : public CD3DBase
{
public:
	CD3DSample1(HINSTANCE hInstance, HWND hWnd);
	~CD3DSample1();

protected:
	virtual bool LoadRes();
	virtual void UnloadRes();
	virtual void OnUpdate(float fElapsedTime);
	virtual void OnRender();	

private:
	CCamera* m_pCamera;
	CDInputer* m_pDInput;

	ID3DXFont* m_pFont1;
	//ID3DXMesh* m_pMesh3DText;

	CSkyBox* m_pSkyBox;
	//CSnowParticle* m_pSnow;

	CCallBoard* m_pBoardGirl;

	CMeshModel* m_pMeshMiki;

	CBoneMesh* m_pBoneMesh;

	CD3DGui* m_pGui;
};

#endif // __D3D_SAMPLE1_H__



