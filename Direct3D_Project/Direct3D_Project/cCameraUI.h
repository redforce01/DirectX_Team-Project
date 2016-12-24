#pragma once

#include "cScene.h"

class Unit;

class cCameraUI : public cScene
{
private:
	bool isTrunOn;
	LPDIRECT3DTEXTURE9 STBY;
	LPDIRECT3DTEXTURE9 W_T;
	LPDIRECT3DTEXTURE9 HD;
	LPDIRECT3DTEXTURE9 zoombar;
	LPDIRECT3DTEXTURE9 battery;
	LPDIRECT3DTEXTURE9 rec;
	LPDIRECT3DTEXTURE9 cube;
	int reccount;
	int zoombarx;

	bool isRec;
	bool isdelay;
	int delayCount;

	bool zoomin;
	bool zoomout;
	Unit* m_CurUnit;

	float CamFov;

public:
	cCameraUI(cCamera* Camera,  Unit* unit );
	~cCameraUI();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);
	virtual void Scene_Render1();
	virtual void Scene_RenderSprite();

	void SetisRec(bool is) { isRec = is; }
	bool GetisRec() { return isRec; }
	void TurnOnCam();

};

