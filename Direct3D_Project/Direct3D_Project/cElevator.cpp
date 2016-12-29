#include "stdafx.h"
#include "cElevator.h"

#include "cCamera.h"
#include "cBaseObject.h"
#include "Unit.h"
#include "cElevator_Door.h"

cElevator::cElevator()
{
}


cElevator::~cElevator()
{
}

HRESULT cElevator::Scene_Init()
{
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, OBJSIZE, OBJSIZE, OBJSIZE);

	elevator_mesh = new cBaseObject();
	elevator_mesh->SetMesh(RESOURCE_STATICXMESH->GetResource("../Resources/Elevator/Elevator.X", &matScale));
	elevator_mesh->SetActive(true);
	elevator_mesh->pTransform->SetWorldPosition(5.33f, -20.6f, 13.5f);

	elevatorRoop = 0;
	isPlayerMove = false;
	isEleavotr_Light = false;
	return S_OK;
}

void cElevator::Scene_Release()
{
	SAFE_DELETE(elevator_mesh);
}

void cElevator::Scene_Update(float timeDelta)
{
	if (elevatorRoop == 7)
	{
		ElevatorBlock();
		return;
	}
	elevator_mesh->Update(timeDelta);
	ElevatorRoop(timeDelta);
	ElevatorLight();
	ElevatorEvent(timeDelta);
}

void cElevator::Scene_Render1()
{
	elevator_mesh->Render();

	//LOG_MGR->AddLog("%f",
	//	Miles->getTrans()->GetWorldPosition().z);
}

void cElevator::ElevatorRoop(float timeDelta)
{
	float Move = 3.0f*timeDelta;
	float posY = elevator_mesh->pTransform->GetWorldPosition().y;
	SOUNDDATA->playSound(SOUND_TYPE_STRUCT_BASEMENT, SOUND_PLAY_TYPE_ELEVATOR, 0, 0.5);

	if (elevatorRoop >= 6 && fabs(posY > 0.0f))
	{
		SOUNDDATA->stopSound(SOUND_TYPE_STRUCT_BASEMENT, SOUND_PLAY_TYPE_ELEVATOR, 0);
		//SOUNDDATA->playSound(SOUND_TYPE_BGM, SOUND_PLAY_GAME_BGM, 0, 0.5);
		return;
	}
	else
	{
		isGameEvent = true;
		Miles->IsContoroll(false);

		if (posY > -6.2f&&elevatorRoop <= 4)
		{
			elevator_mesh->pTransform->SetWorldPosition(5.33f, -20.6f, 13.5f);
			Miles->getTrans()->SetWorldPosition(MilESPOSX, MilESPOSY, MilESPOSZ);
			elevator_door->GetTrans()->SetWorldPosition(3.87f, -20.4f, 11.41f);
			elevatorRoop++;
		}
		else if (elevatorRoop == 5 && posY > -0.1f)
		{
			elevatorRoop++;
		}

		elevator_mesh->pTransform->MovePositionWorld(0.f, Move, 0.f);
		Miles->getTrans()->MovePositionWorld(0.f, Move, 0.f);
		elevator_door->GetTrans()->MovePositionWorld(0.f, Move, 0.f);
	}
}

void cElevator::ElevatorLight()
{
	float posY = elevator_mesh->pTransform->GetWorldPosition().y;
	if (elevatorRoop<1)
		isEleavotr_Light = true;
	else if (posY < -20.f)
		isEleavotr_Light = true;
	else if (posY > -7.0f)
		isEleavotr_Light = false;
}

void cElevator::ElevatorEvent(float timeDelta)
{
	float Move = 1.5f*timeDelta;
	float posY = elevator_mesh->pTransform->GetWorldPosition().y;
	float MilesZ = Miles->getTrans()->GetWorldPosition().z;
	if (MilesZ <= 8.0f)
	{
		Miles->IsContoroll(true);
		isPlayerMove = false;
		isGameEvent = false;
		Miles->getSkinned()->Play("STAND", 0.0f);
		elevator_door->GetAni_1()->PlayOneShotAfterHold("close", 0.0f);
		elevator_door->GetAni_2()->PlayOneShotAfterHold("close", 0.0f);
		elevatorRoop++;
		//SOUNDDATA->playSound(SOUND_TYPE_STRUCT_BASEMENT, SOUND_PLAY_TYPE_ELEVATOR, 1, 0.5);
	}

	if (elevatorRoop == 6 && fabs(posY > 0.0f))
	{
		elevator_door->GetAni_1()->PlayOneShotAfterHold("open", 0.0f);
		elevator_door->GetAni_2()->PlayOneShotAfterHold("open", 0.0f);
		isPlayerMove = true;
		SOUNDDATA->stopSound(SOUND_TYPE_STRUCT_BASEMENT, SOUND_PLAY_TYPE_ELEVATOR, 0);
		SOUNDDATA->playSound(SOUND_TYPE_STRUCT_BASEMENT, SOUND_PLAY_TYPE_ELEVATOR, 2, 0.5);
	}

	if (isPlayerMove)
	{
		Miles->getTrans()->MovePositionWorld(0.f,0.f, -Move);
		Miles->getSkinned()->Play("WALK_FORWARD",0.0f);
	}
}

void cElevator::ElevatorBlock()
{
	bool isBlock = PHYSICS_MGR->IsBlocking(Miles->getTrans(), Miles->getCollisionBox(),
		elevator_door->GetdoorTrans(), elevator_door->GetdoorBoundBox(), 1.0f);
}

void cElevator::DeBugMode()
{
	Miles->IsContoroll(true);
	isPlayerMove = false;
	isGameEvent = false;
	Miles->getTrans()->SetWorldPosition(3.87f, 0.0f, 8.0f);
	elevatorRoop = 7;
}

