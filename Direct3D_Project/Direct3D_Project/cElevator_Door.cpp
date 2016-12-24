#include "stdafx.h"
#include "cElevator_Door.h"

#include "cTransform.h"
#include "cSkinnedAnimation.h"

#include "cLight.h"
#include "cLight_Direction.h"
#include "cLight_Point.h"
#include "cCamera.h"

cElevator_Door::cElevator_Door()
{
}


cElevator_Door::~cElevator_Door()
{
}

HRESULT cElevator_Door::Scene_Init()
{
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, DOORSIZE, DOORSIZE, DOORSIZE);
	D3DXMATRIXA16 matRote;
	D3DXMatrixRotationY(&matRote, 180 * ONE_RAD);
	D3DXMATRIXA16 matDoorWorld;
	matDoorWorld = matRote*matScale;
	cXMesh_Skinned* doorani = RESOURCE_SKINNEDXMESH->GetResource("../Resources/door_ani/door_ani.X", &matScale);
	this->e_doorSkinned = new cSkinnedAnimation();
	this->e_doorSkinned->Init(doorani);

	this->e_doorTransform = new cTransform();
	this->e_doorTransform->SetRotateWorld(0.0f, 180 * ONE_RAD, 0.0f);
	this->e_doorTransform->SetWorldPosition(6.28f, 0.21f, 10.3f);

	this->e_BoundBoxTransform = new cTransform();
	this->e_doorBoundBox = new cBoundBox();
	this->e_doorBoundBox->SetBound(&D3DXVECTOR3(1.2, 1.5, 0), &D3DXVECTOR3(1.4, 1.7, 0.3));

	this->e_doorSkinned->AddBoneTransform("Root_Door", e_BoundBoxTransform);
	this->e_doorSkinned->PlayOneShotAfterHold("idle", 0.0f);

	this->e_doorSkinned_1 = new cSkinnedAnimation();
	this->e_doorSkinned_1->Init(doorani);

	this->e_doorTransform_1 = new cTransform();
	this->e_doorTransform_1->SetWorldPosition(3.87f, -20.4f, 11.41f);

	this->e_BoundBoxTransform_1 = new cTransform();
	this->e_doorBoundBox_1 = new cBoundBox();
	this->e_doorBoundBox_1->SetBound(&D3DXVECTOR3(1.2, 1.5, 0), &D3DXVECTOR3(1.4, 1.7, 0.3));

	this->e_doorSkinned_1->AddBoneTransform("Root_Door", e_BoundBoxTransform_1);
	this->e_doorSkinned_1->PlayOneShotAfterHold("idle", 0.0f);

	return S_OK;
}

void cElevator_Door::Scene_Release()
{
	SAFE_DELETE(e_doorSkinned);
	SAFE_DELETE(e_doorTransform);
	SAFE_DELETE(e_BoundBoxTransform);
	SAFE_DELETE(e_doorBoundBox);

	SAFE_DELETE(e_doorSkinned_1);
	SAFE_DELETE(e_doorTransform_1);
	SAFE_DELETE(e_BoundBoxTransform_1);
	SAFE_DELETE(e_doorBoundBox_1);

}

void cElevator_Door::Scene_Update(float timeDelta)
{
	this->e_doorSkinned->Update(timeDelta);
	this->e_doorSkinned_1->Update(timeDelta);
}

void cElevator_Door::Scene_Render1()
{
	this->e_doorSkinned->Render(e_doorTransform);
	this->e_doorSkinned_1->Render(e_doorTransform_1);
	e_doorBoundBox->RenderGizmo(e_doorTransform);
	e_doorBoundBox_1->RenderGizmo(e_doorTransform_1);
}
