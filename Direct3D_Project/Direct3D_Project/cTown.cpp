#include "stdafx.h"
#include "cTown.h"


cTown::cTown()
{
}


cTown::~cTown()
{
}

HRESULT cTown::Scene_Init()
{
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, 2.f, 2.f, 2.f);
	D3DXMATRIXA16 SkyScale;
	D3DXMatrixScaling(&SkyScale, 0.001f, 0.001f, 0.001f);
	D3DXMATRIXA16 matRota;
	D3DXMatrixRotationY(&matRota, 100.f * ONE_RAD);
	_town = new cBaseObject();
	_town->SetMesh(RESOURCE_STATICXMESH->GetResource("../Resources/town/house_2.X", &matScale));
	_town->SetActive(true);
	skyBox = new cBaseObject();
	skyBox->SetMesh(RESOURCE_STATICXMESH->GetResource("../Resources/skybox/doomSky.X", &SkyScale));
	skyBox->pTransform->SetRotateWorld(matRota);
	skyBox->SetActive(true);
	return S_OK;
}

void cTown::Scene_Release()
{
	SAFE_DELETE(_town);
	SAFE_DELETE(skyBox);
}

void cTown::Scene_Update(float timeDelta)
{
	skyBox->Update(timeDelta);
}

void cTown::Scene_Render1()
{
	_town->Render();
	skyBox->Render();
}

void cTown::Scene_RenderSprite()
{
}