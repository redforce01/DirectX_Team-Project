#include "stdafx.h"
#include "cElevator_passageway.h"

#include "cCamera.h"

cElevator_passageway::cElevator_passageway()
{
}


cElevator_passageway::~cElevator_passageway()
{
}

HRESULT cElevator_passageway::Scene_Init()
{
	D3DXMATRIXA16 matSclae;
	D3DXMatrixScaling(&matSclae, OBJSIZE, OBJSIZE, OBJSIZE);

	e_passagewayMesh_1 = new cBaseObject();
	e_passagewayMesh_1->SetMesh(RESOURCE_STATICXMESH->GetResource("../Resources/Elevator_passageway/Elevator_passageway.X", &matSclae));
	e_passagewayMesh_1->SetActive(true);
	e_passagewayMesh_1->pTransform->SetWorldPosition(5.38f, -13.13f, 14.08f);

	e_passagewayMesh_2 = new cBaseObject();
	e_passagewayMesh_2->SetMesh(RESOURCE_STATICXMESH->GetResource("../Resources/Elevator_passageway/Elevator_passageway.X", &matSclae));
	e_passagewayMesh_2->SetActive(true);
	e_passagewayMesh_2->pTransform->SetWorldPosition(5.38f, -24.3f, 14.08f);

	_gameStart_loding = 0;
	isEleavotr_Light = true;
	isGameStart = false;
	return S_OK;
}

void cElevator_passageway::Scene_Release()
{
	SAFE_DELETE(e_passagewayMesh_1);
	SAFE_DELETE(e_passagewayMesh_2);
}

void cElevator_passageway::Scene_Update(float timeDelta)
{

}

void cElevator_passageway::Scene_Render1()
{
	e_passagewayMesh_1->Render();
	e_passagewayMesh_2->Render();

}
