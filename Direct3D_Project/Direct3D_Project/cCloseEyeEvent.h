#pragma once

#include "cScene.h"

class Unit;
class cScene;

//헤더입니다

class cCloseEyeEvent 
{
private:
	//페이드
	Unit* m_CurUnit;
	cScene* m_CurScene;
	LPDIRECT3DTEXTURE9 upeye;
	LPDIRECT3DTEXTURE9 downeye;
	int upeyeplaceY;
	int downeyeplaceY;
	bool event[20];
	bool eventstart;
	bool m_isCompleteEyeClose;
	int eyealpha;
	int count;

	int EyeCloseCount;
	float Speed;
	////////
	bool eyeup = false;

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

	///////
	void SetCloseTime();

	bool GetCompleteClosed() { return m_isCompleteEyeClose; }
	void SetCloseNum(int num) { EyeCloseCount = num; }
	void SetCloseSpeed(float speed) { Speed = speed; }
	bool SetisEvent() { event[0]; }
	void GetPlayerMemory(Unit* unit) { m_CurUnit = unit; }
	void GetPlayerMemory(cScene* scene) { m_CurScene = scene; }
};