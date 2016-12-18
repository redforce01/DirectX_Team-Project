#pragma once
#include "cScene.h"

class cCamera;
class cScene_Loading : public cScene
{
private:
	LPDIRECT3DTEXTURE9 pBackground;
	RECT rcBackground;

	LPDIRECT3DTEXTURE9 main;
	RECT mainpicture = { 0,0,WINSIZE_X ,500 };

public:
	cScene_Loading();
	~cScene_Loading();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timDelta);

	virtual void Scene_Render1();

	virtual void Scene_RenderSprite();
};

