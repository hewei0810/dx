#pragma once

class CMenuScene : public CScene
{
public:
	CMenuScene();
	~CMenuScene();

public:
	virtual bool InitScene();
	virtual bool UpdateScene(float fElapsedTime);
	virtual bool DrawScene();

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
private:
	CD3DGui* m_pGui;
};

class CMainScene : public CScene
{
public:
	CMainScene();
	~CMainScene();

public:
	virtual bool InitScene();
	virtual bool UpdateScene(float fElapsedTime);
	virtual bool DrawScene();

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
private:
	CCamera* m_pCamera;

	CSkyBox* m_pSkyBox;
	CBoneMesh* m_pBoneMesh;
};