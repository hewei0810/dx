#ifndef __GLOBAL_MANAGER_H__
#define __GLOBAL_MANAGER_H__

#include "D3DRenderer.h"
#include "InputDev.h"

class DXENGINE_API CGlobalManager
{
private:
	CGlobalManager();
	~CGlobalManager();

public:
	static void CreateManager();
	static CGlobalManager* GetManager() {return sm_pManager;}

	static LRESULT CALLBACK	WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	bool InitManager(HINSTANCE hInst, HWND hWnd);
	int LoopMessage();

	CD3DRenderer* GetRenderer() const {return m_pRenderer;}
	CDInputer* GetInputer() const {return m_pInputer;}
	
	void SetCurScene(CScene* pScene);
	CScene* GetCurScene() const {return m_pCurScene;}

	CStdString GetModulePath()
	{
		TCHAR tszModule[MAX_PATH + 1] = { 0 };
		::GetModuleFileName(m_hInst, tszModule, MAX_PATH);
		CStdString sInstancePath = tszModule;
		int pos = sInstancePath.RFind(_T('\\'));
		if( pos >= 0 ) sInstancePath = sInstancePath.Left(pos + 1);
		return sInstancePath;
	}
	void SetResPath(const char* path)
	{
		CStdString strTmp(path);
		if (path[strlen(path) - 1] != '\\')
			strTmp += "\\";
		m_strResPath = strTmp;
	}
	CStdString GetResPath() const {return m_strResPath;}

private:
	static void ExitManager();

private:
	static CGlobalManager* sm_pManager;

	HINSTANCE m_hInst;
	HWND m_hWnd;
	RECT m_rcRender;

	CD3DRenderer* m_pRenderer;
	CDInputer* m_pInputer;

	CScene* m_pCurScene;

	CStdString m_strResPath;
};

#endif // __GLOBAL_MANAGER_H__