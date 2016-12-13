#include "stdafx.h"
#include "cScene_Main.h"

#include "cCamera.h"
#include "cXMesh_Skinned.h"
#include "cSkinnedAnimation.h"
#include "cLight.h"
#include "cLight_Direction.h"
#include "cLight_Point.h"

cScene_Main::cScene_Main()
{
}


cScene_Main::~cScene_Main()
{
}

HRESULT cScene_Main::Scene_Init()
{
	//SOUNDMANAGER->play("MenuSound_MenuBGM0", 0.5);
	SOUNDDATA->playSound(SOUND_TYPE_MENU, SOUND_PLAY_TYPE_MENUBGM, 0);

	D3DXMATRIXA16 matCorrection;
	D3DXMatrixScaling(&matCorrection, 0.1f, 0.1f, 0.1f);
	cXMesh_Skinned* pSkinnedMesh = RESOURCE_SKINNEDXMESH->GetResource("../Resources/NPC/NPC_Death.X", &matCorrection);

	pAnimation = new cSkinnedAnimation();
	pAnimation->Init(pSkinnedMesh);

	pPatientTrans = new cTransform();
	pPatientTrans->SetLocalPosition(0, 0, 10);
	pPatientBox = new cBoundBox();


	//라이트 푸쉬
	cLight_Direction* pLight1 = new cLight_Direction();
	pLight1->Color = D3DXCOLOR(20, 20, 20, 1);
	pLight1->Intensity = 0.1f;

	cLight_Point* pLight2 = new cLight_Point();
	pLight2->Color = D3DXCOLOR(1, 0, 0, 0);
	pLight2->minRange = 5.0f;
	pLight2->maxRange = 10.0f;
	pLight2->distancePow = 10.0f;

	cLight_Point* pLight3 = new cLight_Point();
	pLight3->Color = D3DXCOLOR(1, 1, 1, 0);
	pLight3->minRange = 5.0f;
	pLight3->maxRange = 10.0f;
	pLight3->distancePow = 10.0f;
	pLight3->pTransform->SetWorldPosition(3, 3, 0);
	pLight3->Intensity = 0.5f;

	this->lights.push_back(pLight1);
	this->lights.push_back(pLight2);
	this->lights.push_back(pLight3);


	m_bFlag = FALSE;

	return S_OK;
}

void cScene_Main::Scene_Release()
{
	SAFE_DELETE(pPatientTrans);
	SAFE_DELETE(pPatientBox);

	pAnimation->Release();
}

void cScene_Main::Scene_Update(float timeDelta)
{
	pAnimation->Update(timeDelta);

	if (KEY_MGR->IsOnceDown(VK_RETURN))
	{
		m_bFlag = !m_bFlag;
		if (m_bFlag) pAnimation->Play("POSE0", 0.3);
		else pAnimation->Play("POSE1", 0.3);
	}

	lights[0]->pTransform->DefaultControl2(timeDelta);
}

void cScene_Main::Scene_Render1()
{
	//적용되는 LightMatrix
	D3DXMATRIXA16 matLights[10];
	for (int i = 0; i < this->lights.size(); i++)
		matLights[i] = this->lights[i]->GetLightMatrix();

	//셰이더에 라이팅 셋팅
	cXMesh_Skinned::sSkinnedMeshEffect->SetMatrixArray("matLights", matLights, 10);
	cXMesh_Skinned::sSkinnedMeshEffect->SetInt("LightNum", this->lights.size());

	cXMesh_Skinned::SetCamera(this->pMainCamera);
	pAnimation->Render(pPatientTrans);
}
