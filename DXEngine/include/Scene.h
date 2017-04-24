#ifndef __SCENE_H__
#define __SCENE_H__ 

class DXENGINE_API CScene
{
public:
	CScene() {}
	virtual ~CScene() {}

public:
	virtual bool InitScene() = 0;
	virtual bool UpdateScene(float fElapsedTime) {return true;}
	virtual bool DrawScene() = 0;

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

protected:
private:
};

#endif // __SCENE_H__