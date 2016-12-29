#include "stdafx.h"
#include "cEndingMain.h"
#include "cCamera.h"
#include "cXmesh_Static.h"
#include "cLight.h"
#include "cLight_Direction.h"
#include "Unit.h"
#include "cScene_Game.h"


cEndingMain::cEndingMain()
{
}


cEndingMain::~cEndingMain()
{
}

HRESULT cEndingMain::Scene_Init()
{
	cScene_Game::SetBroken(true);

	SOUNDDATA->stopSound(SOUND_TYPE_CUSTOM, SOUND_PLAY_TYPE_CUSTOM, 1);
	SOUNDDATA->stopSound(SOUND_TYPE_CUSTOM, SOUND_PLAY_TYPE_CUSTOM, 5);
	isEnding = true;
	isLoding = false;
	EndingLogo = RESOURCE_TEXTURE->GetResource("../Resources/EndingLogo.jpg");
	pMainCamera = vCamera[PLAYER];
	town = new cTown();
	town->Scene_Init();
	rain = new cRainParticle();
	rain->Scene_Init();
	movieEvent = new cMovieEvent();
	movieEvent->Scene_Init();

	D3DXMATRIXA16 matRota;
	D3DXMatrixRotationY(&matRota, 180 * ONE_RAD);
	Miles = new Player("../Resources/Meshes/Miles/miles.X", { 0,0,0 });

	Miles->getTrans()->SetWorldPosition(36.01f, 0.0f, -9.73f);

	cLight_Direction* pLight1 = new cLight_Direction;
	pLight1->Color = D3DXCOLOR(0.4, 0.4, 0.4, 1);
	pLight1->pTransform->SetWorldPosition(0, 1, 0);

	cLight_Direction* pLight2 = new cLight_Direction;
	pLight2->Color = D3DXCOLOR(0.4, 0.4, 0.4, 1);
	pLight2->pTransform->SetWorldPosition(0, 1, 0);
	pLight2->pTransform->LookDirection(D3DXVECTOR3(0.f, -1.f, 0.f), D3DXVECTOR3(0.f, 0.f, 1.f));
	this->lights.push_back(pLight1);
	this->lights.push_back(pLight2);

	Miles->CameraAttachToUnit(vCamera[PLAYER]);
	Miles->setScene(this);
	scene = LOOK_CHURCH;

	pMainCamera->RotateSelf(0, 200 * ONE_RAD, 0);

	count = 0;
	Soundcount = 0;
	isEndinLogo = false;
	isSmileSound = true;
	isScreamSound = false;
	return S_OK;
}

void cEndingMain::Scene_Release()
{
	town->Scene_Release();
	SAFE_DELETE(town);
	rain->Scene_Release();
	SAFE_DELETE(rain);
	for (int i = 0; i < lights.size(); i++)
		SAFE_DELETE(lights[i]);
	Miles->Release();
	SAFE_DELETE(Miles);
	movieEvent->Scene_Release();
	SAFE_DELETE(movieEvent);

}

void cEndingMain::Scene_Update(float timeDelta)
{
	SOUNDDATA->stopSound(SOUND_TYPE_CUSTOM, SOUND_PLAY_TYPE_CUSTOM, 1);
	SOUNDDATA->stopSound(SOUND_TYPE_CUSTOM, SOUND_PLAY_TYPE_CUSTOM, 5);

	lights[1]->pTransform = vCamera[PLAYER];

	town->Scene_Update(timeDelta);
	rain->Scene_Update(timeDelta);
	rain->RainPosition((Player*)Miles);
	Miles->Update(timeDelta);
	//vCamera[PLAYER]->DefaultControl4(Miles, 5, timeDelta);
	vCamera[PLAYER]->SetWorldPosition(Miles->getHeadCamPos());
	movieEvent->Scene_Update(timeDelta);
	EndingMove(timeDelta);
	pMainCamera->DefaultControl2(timeDelta);
	if (scene == MILES_WALK || scene == LOOK_SMILE || scene == LOOK_END)
	{
		Soundcount++;
	}
}

void cEndingMain::Scene_Render1()
{
	if (isLoding)
	{
		D3DXMATRIXA16 matLights[10];
		for (int i = 0; i < 2; i++)
		{
			matLights[i] = this->lights[i]->GetLightMatrix();
		}

		//셰이더에 라이팅 셋팅
		//cXMesh_Static::sStaticMeshEffect->SetTechnique("ReceiveShadow");
		cXMesh_Static::SetLighting(&lights);
		cXMesh_Static::SetCamera(this->pMainCamera);

		cXMesh_Skinned::SetLighting(&lights);
		cXMesh_Skinned::SetCamera(this->pMainCamera);

		town->Scene_Render1();
		rain->Scene_Render1();
		Miles->Render();
		Miles->getHeadTrans()->SetWorldPosition(0.0f, -10.f, 0.0f);
		//LOG_MGR->AddLog("ddd");
		/*   LOG_MGR->AddLog("%f,%f,%f",
		pMainCamera->GetUp().x,
		pMainCamera->GetUp().y,
		pMainCamera->GetUp().z);*/

		/*LOG_MGR->AddLog("%f,%f,%f",
		Miles->getTrans()->GetLocalPosition().x,
		Miles->getTrans()->GetLocalPosition().y,
		Miles->getTrans()->GetLocalPosition().z);*/

		/*   LOG_MGR->AddLog("%f",
		pMainCamera->GetLocalPosition().z
		);*/

		//LOG_MGR->AddLog("%d", Soundcount);
	}

	count++;
	if (count > 200)
	{
		if (!isGameEvent)
			isGameEvent = true;
		isLoding = true;
		SOUNDDATA->playSound(SOUND_TYPE_STRUCT_COURTYARD, SOUND_PLAY_TYPE_RAINY, 0, 0.3);
		SOUNDDATA->playSound(SOUND_TYPE_STRUCT_COURTYARD, SOUND_PLAY_TYPE_GROUND, 0, 0.15);
	}
}

void cEndingMain::Scene_RenderSprite()
{
	movieEvent->Scene_RenderSprite();

	if (isEndinLogo)
	{
		RECT rc = { 0,0,1800,1000 };
		SPRITE_MGR->DrawTexture(EndingLogo, &rc, -380, -150, 1, 1, NULL, D3DCOLOR_ARGB(/*letteralpha*/255, 255, 255, 255), &D3DXVECTOR3(0, 0, 0));
	}
}

void cEndingMain::EndingMove(float TimeDelta)
{
	if (!isLoding)
		return;
	else if (count < 300)
		return;
	float move = 0.25f*TimeDelta;

	switch (scene)
	{
	case LOOK_CHURCH:
		if (pMainCamera->GetForward().x > 0.99f)
			scene = LOOK_CHURCH_UP;
		pMainCamera->RotateWorld(0, -move, 0);
		break;
	case LOOK_CHURCH_UP:
		if (pMainCamera->GetUp().y < 0.73f)
			scene = LOOK_CHURCH_DOWN;
		pMainCamera->RotateSelf(-move, 0, 0);
		break;
	case LOOK_CHURCH_DOWN:
		if (pMainCamera->GetUp().y > 0.99f)
			scene = LOOK_BACK;
		if (count>900)
			pMainCamera->RotateSelf(move, 0, 0);
		break;
	case LOOK_BACK:
		if (pMainCamera->GetForward().x < 0.0f)
			scene = LOOK_FORWARD;
		pMainCamera->RotateWorld(0, move, 0);
		break;
	case LOOK_FORWARD:
		if (pMainCamera->GetForward().z > 0.61f)
			scene = MILES_WALK;
		Miles->getTrans()->RotateWorld(0, move, 0);
		//pMainCamera->RotateWorld(0, move, 0);
		break;
	case MILES_WALK:
		if (Soundcount >= 400)
		{
			if (isSmileSound)
			{
				SOUNDDATA->playSound(SOUND_TYPE_STRUCT_COURTYARD, SOUND_PLAY_TYPE_SMILE, 0, 1.0);
				isSmileSound = false;
				isScreamSound = true;
			}
			scene = LOOK_SMILE;
		}
		Miles->getSkinned()->Play("WALK_FORWARD", 0.0f);
		SOUNDDATA->playSound(SOUND_TYPE_PLAYER_EFFECT, SOUND_PLAY_TYPE_PLAYER_WALKING_SOIL, 10, 1.0);
		//걷는 사운드 넣고
		Miles->getTrans()->MovePositionWorld(-1.5*TimeDelta, 0.f, 1.5*TimeDelta);
		break;
	case LOOK_SMILE:
		if (pMainCamera->GetForward().x > 0.20f)
			scene = LOOK_END;

		Miles->getSkinned()->Play("STAND", 0.0f);

		if (isScreamSound == true && Soundcount >= 450)
		{
			SOUNDDATA->playSound(SOUND_TYPE_PLAYER_VOICE, SOUND_PLAY_TYPE_PLAYER_SURPRISE, 18, 0.5);
			isScreamSound = false;
		}

		if (Soundcount >= 450)
			pMainCamera->RotateWorld(0, -move*5.0f, 0);
		break;
	case LOOK_END:
		isEndinLogo = true;
		break;
	default:
		break;
	}
	//Miles->getTrans()->MovePositionWorld(0, 0, 0);
}