#pragma once
#include "cScene.h"

class cLight;
class cScene_Test : public cScene
{
private:
	float m_WalkingTime;
	vector<cLight*> lights;


public:
	cScene_Test();
	~cScene_Test();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);

	virtual void Scene_Render1();

	virtual void Scene_RenderSprite();
};

