#include "stdafx.h"
#include "cItem.h"

HRESULT cAccessory::Init(std::string name, std::string filePath, D3DXVECTOR3 scale, D3DXVECTOR3 rotation, D3DXVECTOR3 translation)
{
	this->objName = name;
	this->SetScale(scale);
	this->RotateSelf(rotation);
	this->SetLocalPosition(translation);

	D3DXMATRIXA16 temp = this->GetFinalMatrix();
	this->mesh = RESOURCE_STATICXMESH->GetResource(filePath, temp);

	return 0;
}


void cAccessory::Update(float timeDelta)
{
	////* 아이템 월드로 다시 오브젝트를 설정해줘야함
	//this->obj->SetMesh(RESOURCE_STATICXMESH->GetResource(itemFilePath, matWorld));
	//this->obj->SetActive(true);

	D3DXVECTOR3 temp = this->GetWorldPosition();

}

void cAccessory::Release()
{
	SAFE_DELETE(this->mesh);
}

void cAccessory::Render()
{
	this->mesh->Render(this);
}

void cAccessory::ComputeBoundBox()
{
	if (mesh != NULL)
	{
		this->BoundBox.localCenter = mesh->Bound_Center;
		this->BoundBox.halfSize = mesh->Bound_HalfSize;
		this->BoundBox.localMinPos = mesh->Bound_Min;
		this->BoundBox.localMaxPos = mesh->Bound_Max;
		this->BoundBox.radius = mesh->Bound_Radius;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
void cGameItem::Init(int type, std::string path, D3DXVECTOR3 scale, D3DXVECTOR3 rotation, D3DXVECTOR3 trans)
{
	D3DXMATRIXA16 matCorrection;
	D3DXMatrixScaling(&matCorrection, scale.x, scale.y, scale.z);

	this->pMesh = RESOURCE_STATICXMESH->GetResource(path, &matCorrection);

	this->pTransform->SetRotateWorld(rotation.x, rotation.y, rotation.z);
	this->pTransform->SetWorldPosition(trans);

	this->SetActive(true);

	this->m_type = type;

}
