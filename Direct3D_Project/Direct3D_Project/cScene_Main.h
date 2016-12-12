#pragma once
#include "cScene.h"

#include "cObject_EventObj.h"
#include "cObject_Sound.h"

class cScene_Main : public cScene
{
private:
	vector<cObject_EventObj*>	vEventObj;
	vector<cObject_Sound*>		vEventSound;

public:
	cScene_Main();
	~cScene_Main();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);

	virtual void Scene_Render1();
};

