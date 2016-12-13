#pragma once
#include "cScene.h"

#define MAXDOOR 8

class cBaseObject;

class cDoorPosition : public cScene
{
private:
	std::vector<cBaseObject*> door;

public:
	cDoorPosition();
	~cDoorPosition();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);
	virtual void Scene_Render1();

	void DoorSetPosition();
};

