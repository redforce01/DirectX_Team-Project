#pragma once
#include "cScene.h"
class cScene_Main : public cScene
{
private:
	


public:


public:
	cScene_Main();
	~cScene_Main();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);

	virtual void Scene_Render1();

	virtual void Scene_RenderSprite();
};

