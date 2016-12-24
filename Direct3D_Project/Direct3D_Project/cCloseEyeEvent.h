#pragma once

#include "cScene.h"


class cCloseEyeEvent : public cScene
{
private:
	//∆‰¿ÃµÂ
	LPDIRECT3DTEXTURE9 upeye;
	LPDIRECT3DTEXTURE9 downeye;
	int upeyeplaceY;
	int downeyeplaceY;
	bool event[20];
	bool eventstart;
	int eyealpha;
	int count;


public:
	bool isPlayEye;

	cCloseEyeEvent();
	~cCloseEyeEvent();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);
	virtual void Scene_Render1();
	virtual void Scene_RenderSprite();

	void SetisPlayEye(bool is) { isPlayEye = is; }
	bool GetisPlayEye() { return isPlayEye; }
};

