#pragma once
#include "cScene.h"
class cScene_Loading : public cScene
{
private:
	LPDIRECT3DTEXTURE9 LoadingImage;
	LPDIRECT3DTEXTURE9 LoadingContinue;

	RECT LoadingRect;
	RECT LoadingRectContinue;

	int ContinueAlpha;
	BOOL AlphaChange;

	float scaleX, scaleY;
	char szTemp[128];

public:
	cScene_Loading();
	~cScene_Loading();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);

	virtual void Scene_Render1();

	virtual void Scene_RenderSprite();
};

