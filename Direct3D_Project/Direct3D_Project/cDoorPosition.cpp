#include "stdafx.h"
#include "cDoorPosition.h"

#include "cBaseObject.h"
#include "cCamera.h"

cDoorPosition::cDoorPosition()
{
}

cDoorPosition::~cDoorPosition()
{
}

HRESULT cDoorPosition::Scene_Init()
{
	for (int i = 0; i < MAXDOOR; i++)
	{
		D3DXMATRIXA16 matDoorScale;
		D3DXMatrixScaling(&matDoorScale, 0.02f, 0.02f, 0.02f);
		cBaseObject* doortemp = new cBaseObject();
		doortemp->SetMesh(RESOURCE_STATICXMESH->GetResource("../Resources/door/door.X", &matDoorScale));
		doortemp->SetActive(true);
		door.push_back(doortemp);
	}
	DoorSetPosition();
	return S_OK;
}

void cDoorPosition::Scene_Release()
{
	for (int i = 0; i < door.size(); i++)
	{
		SAFE_DELETE(door[i]);
	}
}

void cDoorPosition::Scene_Update(float timeDelta)
{
	for (int i = 0; i < door.size(); i++)
	{
		door[i]->Update(timeDelta);
	}
	/*if (KEY_MGR->IsStayDown('3'))
	door[6]->pTransform->DefaultControl2(timeDelta);*/
}

void cDoorPosition::Scene_Render1()
{
	for (int i = 0; i < door.size(); i++)
	{
		door[i]->Render();
	}

	/*LOG_MGR->AddLog("%f, %f , %f", 
		door[6]->pTransform->GetLocalPosition().x,
		door[6]->pTransform->GetLocalPosition().y,
		door[6]->pTransform->GetLocalPosition().z);*/
}

void cDoorPosition::DoorSetPosition()
{
	D3DXMATRIXA16 matRotateY_1;
	D3DXMatrixRotationY(&matRotateY_1, 90 * ONE_RAD);
	D3DXMATRIXA16 matRotateY_2;
	D3DXMatrixRotationY(&matRotateY_2, 270 * ONE_RAD);
	D3DXMATRIXA16 matRotateY_3;
	D3DXMatrixRotationY(&matRotateY_3, 180 * ONE_RAD);

	door[0]->pTransform->SetWorldPosition(2.0f, 2.17f, 3.83f);
	door[0]->pTransform->SetRotateWorld(matRotateY_1);

	door[1]->pTransform->SetWorldPosition(5.29f, 2.17f, -14.f);
	door[1]->pTransform->SetRotateWorld(matRotateY_1);

	door[2]->pTransform->SetWorldPosition(-18.13f, 2.17f, -25.9f);

	door[3]->pTransform->SetWorldPosition(-22.15f, 2.17f, -36.f);
	door[3]->pTransform->SetRotateWorld(matRotateY_1);

	door[4]->pTransform->SetWorldPosition(-24.15f, 2.17f, -16.16f);
	door[4]->pTransform->SetRotateWorld(matRotateY_3);

	door[5]->pTransform->SetWorldPosition(-24.15f, 2.17f, -6.27f);
	door[5]->pTransform->SetRotateWorld(matRotateY_3);

	door[6]->pTransform->SetWorldPosition(-24.15f, 2.17f, 9.67f);
	door[6]->pTransform->SetRotateWorld(matRotateY_3);

	door[7]->pTransform->SetWorldPosition(27.74f, 2.17f, 5.92f);
}
