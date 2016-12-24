#pragma once

#include "cScene.h"
#define OBJSIZE 0.02f

#define MilESPOSX 5.07f
#define MilESPOSY -20.5f
#define MilESPOSZ 15.14f

class cBaseObject;
class Unit;
class cElevator_Door;
class Player;


class cElevator : public cScene
{
private:
	cBaseObject* elevator_mesh;
	Unit* Miles;
	cElevator_Door* elevator_door;
	Player* pp;

	int elevatorRoop;
	bool isPlayerMove;
	bool isEleavotr_Light;
public:
	cElevator();
	~cElevator();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);
	virtual void Scene_Render1();

	void SetLink_Player(Unit* pp) { Miles = pp; }
	void SetLink_Player_1(Player* pl) { pp = pl; }
	void ElevatorRoop(float timeDelta);
	void ElevatorLight();
	void ElevatorEvent(float timeDelta);
	void ElevatorBlock();

	void SetLink_EleDoor(cElevator_Door* cd) { elevator_door = cd; }

	void SetElevatorLight(bool is) { isEleavotr_Light = is; }
	bool GetElevatorLight() { return isEleavotr_Light; }

	void DeBugMode();
};

