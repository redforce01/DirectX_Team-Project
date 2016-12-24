#pragma once
#include "cScene.h"
#include "cDoor.h"
#define MAXDOOR 8

class cBaseObject;
class cDoor;
class Unit;

class cDoorPosition : public cScene
{
private:
	std::vector<cBaseObject*> door;
	Unit* m_unit;
	Unit* m_EnemyUnit;
public:
	cDoorPosition(Unit* unit, Unit* enemy);
	~cDoorPosition();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);
	virtual void Scene_Render1();

	void DoorSetPosition();

	void setRay(Ray ray) { camRay = ray; }

	void InnerOpen(int Idx) { door[Idx]->InnerOpen(); }
	void OutterOpen(int Idx) { door[Idx]->OutterOpen(); }
	void DoorClose(int Idx) { 
		if(door[Idx]->getOpen())
		door[Idx]->CloseDoor(); }
};

