#ifndef __SCENE_H__
#define __SCENE_H__ 

class CScene
{
public:
	CScene();
	virtual ~CScene();

public:
	virtual bool InitScene() = 0;
	virtual bool UpdateScene(float fElapsedTime);
	virtual void DrawScene() = 0;

protected:
private:
};

#endif // __SCENE_H__