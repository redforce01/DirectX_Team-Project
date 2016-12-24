#pragma once

#include "cScene.h"

class cMovieEvent : public cScene
{
private:
	//�̺�Ʈ
	bool run;
	bool stop;
	int moviealpha;
	LPDIRECT3DTEXTURE9 movievent;
public:
	cMovieEvent();
	~cMovieEvent();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);
	virtual void Scene_Render1();
	virtual void Scene_RenderSprite();
};

