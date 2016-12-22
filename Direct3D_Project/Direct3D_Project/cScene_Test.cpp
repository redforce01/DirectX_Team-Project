#include "stdafx.h"
#include "cScene_Test.h"

#include "cLight_Point.h"

cScene_Test::cScene_Test()
{
}


cScene_Test::~cScene_Test()
{
}

HRESULT cScene_Test::Scene_Init()
{
	cBoundBox tempBox;
	tempBox.SetBound(&D3DXVECTOR3(0, 0, 10), &D3DXVECTOR3(1, 1, 1));
	EVENT_MGR->AddEventObject("dd", D3DXVECTOR3(0, 1, 0), tempBox);
	

	cLight_Point* pLight3 = new cLight_Point();
	pLight3->Color = D3DXCOLOR(0.3, 0.3, 0.3, 0);
	pLight3->minRange = 1.0f;
	pLight3->maxRange = 5.0f;
	pLight3->distancePow = 10.0f;

	pLight3->Intensity = 1.f;
	this->lights.push_back(pLight3);

	return S_OK;
}

void cScene_Test::Scene_Release()
{
}

void cScene_Test::Scene_Update(float timeDelta)
{
	SOUNDDATA->playSound(SOUND_TYPE_STRUCT_FEMALEWARD, SOUND_PLAY_TYPE_ROPE, 2);

	EVENT_MGR->Update(timeDelta, D3DXVECTOR3(0, 0, 0));
}

void cScene_Test::Scene_Render1()
{
	EVENT_MGR->Render(pMainCamera, lights);
}

void cScene_Test::Scene_RenderSprite()
{
}
