#pragma once
#include "cScene.h"
class cScene_Loading : public cScene
{
private:


public:
	cScene_Loading();
	~cScene_Loading();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timDelta);

	virtual void Scene_Render1();

	virtual void Scene_RenderSprite();

};

