#pragma once


#define OBJSIZE 0.02f

class cElevator_passageway 
{
private:

	cBaseObject* e_passagewayMesh_1;
	cBaseObject* e_passagewayMesh_2;

	int _gameStart_loding;
	bool isGameStart;
	bool isEleavotr_Light;

public:
	cElevator_passageway();
	~cElevator_passageway();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);
	virtual void Scene_Render1();

	void SetElevatorLight(bool is) { isEleavotr_Light = is; }
	bool GetElevatorLight() { return isEleavotr_Light; }
};

