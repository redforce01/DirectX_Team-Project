#include "stdafx.h"
#include "cScene_Game.h"
#include "cLight.h"
#include "cLight_Direction.h"
#include "cLight_Point.h"
#include "cCamera.h"
#include "cBaseObject.h"
#include "cXMesh_Static.h"
#include "cXMesh_Skinned.h"
#include "cSkinnedAnimation.h"
#include "Unit.h"
#include "Action.h"
#include "cNode.h"
#include "../Resources/Shaders/LightEffect/cDustFog.h"
#include "../Resources/Shaders/LightEffect/cLightGlow.h"

bool cScene_Game::m_bIsBroken = false;
bool cScene_Game::m_bNightVision = false;

cScene_Game::cScene_Game()
{
}


cScene_Game::~cScene_Game()
{

}


HRESULT cScene_Game::Scene_Init()
{


	pMainCamera = vCamera[PLAYER];
	isEvent1End = false;
	D3DXMATRIXA16 matScale;
	D3DXMATRIXA16 matTrans;
	D3DXMatrixScaling(&matScale, 0.02f, 0.02f, 0.02f);
	this->house = new cBaseObject();
	this->house->SetMesh(RESOURCE_STATICXMESH->GetResource("../Resources/Outlast_Map/1_Outlast_Map.X", matScale));
	this->house->SetActive(true);
	//holl/holl.x
	//room_1/room_1.x
	//room_2/room_2.x
	//passage/passage.x

	NodeInit();

	//라이트 푸쉬
	Pig = new PigEnemy("../Resources/Meshes/Pig/20161129_Pig.X", { 10, 0, -17.50 }, this);
	//Pig->CameraAttachToUnit(vCamera[ENEMY]);

	effect = new cEffect();
	effect->Scene_Init();

	Miles = new Player("../Resources/Meshes/Miles/miles.X", { 0,0,0 });
	Miles->setScene(this);
	Miles->CameraAttachToUnit(vCamera[PLAYER]);
	Miles->MemoryEffectGet(effect);

	Miles->getTrans()->SetWorldPosition(MilESPOSX, MilESPOSY, MilESPOSZ);


	cameraUI = new cCameraUI(vCamera[PLAYER], Miles);
	cameraUI->Scene_Init();

	Miles->SetMemoryCameraUI(cameraUI);

	fileRead = new cFileRead();
	fileRead->Scene_Init();
	movieEvent = new cMovieEvent();
	movieEvent->Scene_Init();
	eyeEvent = new cCloseEyeEvent();
	eyeEvent->Scene_Init();
	eyeEvent->GetPlayerMemory(Miles);

	bBox_Save = new cMap_Bbox_Save();
	bBox_Save->Scene_Init();

	bBox_Load = new cMap_Bbox_Load();
	bBox_Load->Scene_Init();

	door = new cDoorPosition(Miles, Pig);
	door->Scene_Init();
	door->InnerOpen(0);
	door->OutterOpen(1);

	m_Player_mapAcive = new cMap_Active;
	m_Player_mapAcive->Scene_Init();

	m_Enemy_mapAcive = new cMap_Active;
	m_Enemy_mapAcive->Scene_Init();


	Ele_Door = new cElevator_Door();
	Ele_Door->Scene_Init();

	elevatorl = new cElevator;
	elevatorl->Scene_Init();

	e_passageway = new cElevator_passageway();
	e_passageway->Scene_Init();

	//cLight_Direction* pLight2 = new cLight_Direction;

	//pLight2->Color = D3DXCOLOR(0.03, 0.03, 0.03, 1);
	//pLight2->pTransform->SetWorldPosition(0, -6, 0);

	//this->lights.push_back(pLight2);
	//this->lights.push_back(pLight3);

	/* 크로스헤어 */
	this->lockCrosshairTex = RESOURCE_TEXTURE->LoadResource("../Resources/OLGame/UI/Crosshair/lock.png");
	this->unlockCrosshairTex = RESOURCE_TEXTURE->LoadResource("../Resources/OLGame/UI/Crosshair/unlock.png");

	//2016-12-21
	this->noneTex = RESOURCE_TEXTURE->LoadResource("../Resources/OLGame/UI/Crosshair/none.png");

	//* 아이템 추가

	ItemInit();

	Pig->SetEventMod(true);
	Pig->pushEnemyUnitVector(Miles);
	Miles->pushEnemyUnitVector(Pig);

	m_Player_mapAcive->SetLink_BoxLoad(bBox_Load);
	m_Player_mapAcive->SetLink_Player(Miles);

	m_Enemy_mapAcive->SetLink_BoxLoad(bBox_Load);
	m_Enemy_mapAcive->SetLink_Player(Pig);

	bBox_Load->SetLink_MapActive(m_Player_mapAcive);
	bBox_Load->SetLink_MapActive(m_Enemy_mapAcive);

	elevatorl->SetLink_Player(Miles);
	elevatorl->SetLink_EleDoor(Ele_Door);
	elevatorl->SetLink_Player_1((Player*)Miles);

	cEventBox* Box1 = new cFirstEvent(Miles, Pig, D3DXVECTOR3(4, 3.36, -11.8), D3DXVECTOR3(2, 1, 2), this);
	cEventBox* Box2 = new cDoorCloseEvent(Miles, Pig, D3DXVECTOR3(3.22, 0, 2.68), D3DXVECTOR3(1, 1, 1), this);

	m_EventManager = new cEventManager();
	m_EventManager->PushEventBox(Box1);
	m_EventManager->PushEventBox(Box2);

	//* gameitem 이 가지고 있는거만큼 boundBox를 만들어서 배치해준다.
	for (int i = 0; i < vecGameItem.size(); i++)
	{ //* gameItem의 실제 월드 위치에다가!
		this->m_vecItemBoundBox.push_back(new cBaseObject());
		this->m_vecItemBoundBox[i]->BoundBox.SetBound(
			&this->vecGameItem[i]->pTransform->GetWorldPosition(),
			&D3DXVECTOR3(2.0f, 3.0f, 2.0f));
	}

	this->ItemPocket = this->Miles->GetItemPocketTrans();
	this->ItemSpeed = 0.0f;
	this->bItemClick = false;

	//* cam
	this->camFov = vCamera[PLAYER]->fov;

	cLight_Direction* tempDLight;
	tempDLight = new cLight_Direction();
	tempDLight->Color = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tempDLight->Intensity = 0.1f;

	lights.push_back(tempDLight);

	cLight_Point* tempLight;
	tempLight = new cLight_Point();
	tempLight->Color = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.f);
	tempLight->distancePow = 5.f;
	tempLight->Intensity = 0.7f;
	tempLight->maxRange = 10.f;
	tempLight->minRange = 0.01f;
	tempLight->pTransform->SetWorldPosition(-21.02f, 5.12f, -31.56f);

	lights.push_back(tempLight);

	cLight_Point* tempLight2;
	tempLight2 = new cLight_Point();
	tempLight2->Color = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.f);
	tempLight2->distancePow = 5.f;
	tempLight2->Intensity = 0.9f;
	tempLight2->maxRange = 17.f;
	tempLight2->minRange = 0.01f;
	tempLight2->pTransform->SetWorldPosition(38.21f, 5.12f, 11.55f);

	lights.push_back(tempLight2);

	cLight_Point* pLight4 = new cLight_Point();
	pLight4->Color = D3DXCOLOR(0.7, 0.7, 0.7, 0.7);
	pLight4->minRange = 0.2f;
	pLight4->maxRange = 7.0f;
	pLight4->distancePow = 10.0f;

	pLight4->Intensity = 0.9f;
	pLight4->pTransform->AttachTo(elevatorl->GetElevatorTrans());
	pLight4->pTransform->SetWorldPosition(4.79f, -15.59f, 13.81f);

	this->lights.push_back(pLight4);

	//===========================================빛 주변 이펙트====================================
	cDustFog* tempFog = new cDustFog();
	cTransform* tempTrans = new cTransform();
	tempTrans->SetScale(0.04f, 0.01f, 0.02f);
	tempTrans->SetWorldPosition(38.21f, 5.12f, 11.55f);
	tempTrans->RotateSelf(D3DXVECTOR3(0.f, 90.f * ONE_RAD, 0.f));

	tempFog->SetTrans(tempTrans);
	tempFog->SetActive(true);

	LPD3DXVECTOR4 tempVec = new D3DXVECTOR4(38.21f, 5.12f, 11.55f, 1.f);

	tempFog->SetLightPos(tempVec);

	arrDustFog.push_back(tempFog);
	//=====================================안개1=====================================
	tempFog = new cDustFog();
	tempTrans = new cTransform();
	tempTrans->SetScale(0.015f, 0.006f, 0.01f);
	tempTrans->SetWorldPosition(-21.02f, 5.32f, -31.56f);
	tempTrans->RotateSelf(D3DXVECTOR3(0.f, 0.f, 0.f));

	tempFog->SetTrans(tempTrans);
	tempFog->SetActive(true);

	tempVec = new D3DXVECTOR4(-21.02f, 5.12f, -31.56f, 1.f);

	tempFog->SetLightPos(tempVec);

	arrDustFog.push_back(tempFog);
	//=====================================안개2=====================================
	tempFog = new cDustFog();
	tempTrans = new cTransform();

	tempTrans->AttachTo(elevatorl->GetElevatorTrans());
	tempTrans->SetScale(0.02f, 0.01f, 0.015f);
	tempTrans->SetWorldPosition(4.87f, -15.39f, 13.81f);
	tempTrans->RotateSelf(D3DXVECTOR3(0.f, 0.f, 0.f));

	tempFog->SetTrans(tempTrans);
	tempFog->SetActive(true);

	tempVec = new D3DXVECTOR4(4.87f, -15.59f, 13.81f, 1.f);

	tempFog->SetLightPos(tempVec);

	arrDustFog.push_back(tempFog);
	//=====================================안개3=====================================
	//===========================================빛 주변 이펙트====================================
	cLightGlow* tempGlow = new cLightGlow();

	tempTrans = new cTransform();
	tempTrans->SetScale(1.8f, 1.f, 0.5f);

	tempTrans->SetWorldPosition(38.21f, 5.08f, 11.6f);
	tempTrans->RotateSelf(D3DXVECTOR3(0.f, 0.f, 0.f));

	tempGlow->SetTrans(tempTrans);
	tempGlow->SetActive(true);
	arrGlow.push_back(tempGlow);
	//======================================가짜광원1=======================================
	tempGlow = new cLightGlow();

	tempTrans = new cTransform();
	tempTrans->SetScale(1.2f, 1.f, 0.4f);
	tempTrans->SetWorldPosition(-21.02f, 5.14f, -31.56f);
	tempTrans->RotateSelf(D3DXVECTOR3(0.f, 90.f * ONE_RAD, 0.f));

	tempGlow->SetTrans(tempTrans);
	tempGlow->SetActive(true);
	arrGlow.push_back(tempGlow);
	//======================================가짜광원2=======================================

	tempGlow = new cLightGlow();
	tempTrans = new cTransform();
	tempTrans->RotateSelf(D3DXVECTOR3(0.f, 0.f, 0.f));
	tempTrans->AttachTo(elevatorl->GetElevatorTrans());
	tempTrans->SetScale(0.35f, 1.f, 1.6f);
	tempTrans->SetWorldPosition(4.79f, -15.59f, 13.81f);

	tempGlow->SetTrans(tempTrans);
	tempGlow->SetActive(true);
	arrGlow.push_back(tempGlow);
	//======================================가짜광원3=======================================
	//====================================glow=======

	isEscape = false;
	isRoom_Five = false;

	InitEventObject();
	InitSoundObject();

	SOUNDDATA->playSound(SOUND_TYPE_CUSTOM, SOUND_PLAY_TYPE_CUSTOM, 5, 0.3);

	return S_OK;
}

void cScene_Game::Scene_RenderSprite()
{
	//*point
	RECT rc = { 0, 0, 70, 70 };

	if (bHitRay == true)
	{
		if (this->Miles->IsCamUp() == TRUE)
		{
			SPRITE_MGR->DrawTexture(this->noneTex, &rc, WINSIZE_X / 2, WINSIZE_Y / 2, 0xffffffff, &D3DXVECTOR3(0, 0, 0));
		}
		else
		{
			SPRITE_MGR->DrawTexture(this->lockCrosshairTex, &rc, WINSIZE_X / 2, WINSIZE_Y / 2, 0xffffffff, &D3DXVECTOR3(0, 0, 0));
		}
	}
	else if (bHitRay == false)
	{
		if (this->Miles->IsCamUp() == TRUE)
		{
			SPRITE_MGR->DrawTexture(this->noneTex, &rc, WINSIZE_X / 2, WINSIZE_Y / 2, 0xffffffff, &D3DXVECTOR3(0, 0, 0));
		}
		else
		{
			SPRITE_MGR->DrawTexture(this->unlockCrosshairTex, &rc, WINSIZE_X / 2, WINSIZE_Y / 2, 0xffffffff, &D3DXVECTOR3(0, 0, 0));
		}
	}
	fileRead->Scene_RenderSprite();
	effect->Scene_RenderSprite();
	cameraUI->Scene_RenderSprite();
	movieEvent->Scene_RenderSprite();
	eyeEvent->Scene_RenderSprite();

}

void cScene_Game::ControllNightVision()
{
	if (Miles->IsCamUp() == true)
	{
		if (KEY_MGR->IsOnceDown('F'))
		{
			SOUNDDATA->stopSound(SOUND_TYPE_PLAYER_EFFECT, SOUND_PLAY_TYPE_CAM_VISION);
			SOUNDDATA->playSound(SOUND_TYPE_PLAYER_EFFECT, SOUND_PLAY_TYPE_CAM_VISION);

			if (m_bNightVision == true)
			{
				m_bNightVision = false;
			}
			else
			{
				m_bNightVision = true;
			}
		}
	}
	else
	{
		m_bNightVision = false;
	}
}

void cScene_Game::ControllCamMode()
{
	cameraUI->TurnOnCam();
}

void cScene_Game::DeadEvent()
{
	CloseEye(1, 2);
}

void cScene_Game::CloseEye(int num, float speed)
{
	eyeEvent->SetCloseNum(num);
	eyeEvent->SetCloseSpeed(speed);
	eyeEvent->SetisPlayEye(true);
}

void cScene_Game::ItemInit()
{
	this->noteItem_2 = new cGameItem();
	this->noteItem_2->Init(NOTEITEM, "../Resources/OLGame/Meshes/Item/File/File.X",
		D3DXVECTOR3(0.02f, 0.02f, 0.02f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(3.36f, 0.045f, -1.20f)
		);
	this->vecGameItem.push_back(this->noteItem_2);

	this->noteItem_3 = new cGameItem();
	this->noteItem_3->Init(NOTEITEM, "../Resources/OLGame/Meshes/Item/File/File.X",
		D3DXVECTOR3(0.02f, 0.02f, 0.02f),
		D3DXVECTOR3(0.0f, 45 * ONE_RAD, 0.0f),
		D3DXVECTOR3(7.96f, 2.72f, -21.99f)
		);
	this->vecGameItem.push_back(this->noteItem_3);

	this->noteItem_4 = new cGameItem();
	this->noteItem_4->Init(NOTEITEM, "../Resources/OLGame/Meshes/Item/File/File.X",
		D3DXVECTOR3(0.02f, 0.02f, 0.02f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(40.27f, 1.78f, 8.3f)
		);
	this->vecGameItem.push_back(this->noteItem_4);

	this->noteItem_5 = new cGameItem();
	this->noteItem_5->Init(NOTEITEM, "../Resources/OLGame/Meshes/Item/File/File.X",
		D3DXVECTOR3(0.02f, 0.02f, 0.02f),
		D3DXVECTOR3(0.0f, 45.0f*ONE_RAD, 0.0f),
		D3DXVECTOR3(-15.33f, 1.37f, 17.9f)
		);
	this->vecGameItem.push_back(this->noteItem_5);

	this->noteItem_6 = new cGameItem();
	this->noteItem_6->Init(NOTEITEM, "../Resources/OLGame/Meshes/Item/File/File.X",
		D3DXVECTOR3(0.02f, 0.02f, 0.02f),
		D3DXVECTOR3(0.0f, 45.0f*ONE_RAD, 0.0f),
		D3DXVECTOR3(-30.61f, 0.04f, -10.29f)
		);
	this->vecGameItem.push_back(this->noteItem_6);


	this->keyItem_1 = new cGameItem();
	this->keyItem_1->Init(KEYITEM, "../Resources/OLGame/Meshes/Item/Key/Key.X",
		D3DXVECTOR3(0.03f, 0.03f, 0.03f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(15.22f, 0.02f, -15.71f)
		);
	this->vecGameItem.push_back(this->keyItem_1);

	this->keyItem_0 = new cGameItem();
	this->keyItem_0->Init(KEYITEM, "../Resources/OLGame/Meshes/Item/Key/Key.X",
		D3DXVECTOR3(0.03f, 0.03f, 0.03f),
		D3DXVECTOR3(0.0f, 45.0f * ONE_RAD, 0.0f),
		D3DXVECTOR3(22.09f, 0.02f, 4.17f)
		);
	this->vecGameItem.push_back(this->keyItem_0);

	for (int i = 0; i <vecGameItem.size(); i++)
	{
		vecGameItem[i]->ComputeBoundBox(1.0f);
	}
}

void cScene_Game::ItemUseFile(bool rayHit, int i)
{
	int count = fileRead->GetFileInfo()->count;
	if (rayHit&&count == 0)
	{
		if (KEY_MGR->IsOnceDown(VK_LBUTTON))
		{
			if (i == 0)
			{
				fileRead->GetFileInfo()->is_1 = true;
				fileRead->GetFileInfo()->count = 1;
			}
			else if (i == 1)
			{
				fileRead->GetFileInfo()->is_2 = true;
				fileRead->GetFileInfo()->count = 1;
			}
			else if (i == 2)
			{
				fileRead->GetFileInfo()->is_3 = true;
				fileRead->GetFileInfo()->count = 1;
			}
			else if (i == 3)
			{
				fileRead->GetFileInfo()->is_4 = true;
				fileRead->GetFileInfo()->count = 1;
			}
			else if (i == 4)
			{
				fileRead->GetFileInfo()->is_5 = true;
				fileRead->GetFileInfo()->count = 1;
			}

			Miles->IsCamContoroll(false);
			Miles->IsContoroll(false);
			Pig->setisActive(false);
		}
	}
}

void cScene_Game::InitEventObject()
{
	/*Object Dead Pose List
	POSE_0 ~ 4	: Sit
	POSE_5		: Looping
	POSE_6		: Cross
	POSE_7 ~ 12 : Floor
	*/

	EVENT_MGR->AddEventObject("-", D3DXVECTOR3(12.60, -1.0, 6.8), "POSE_4");					//휠체어 배치
	EVENT_MGR->GetEventObj(0)->getEventTrans()->RotateSelf(D3DXVECTOR3(0, ONE_RAD * 120, 0));

	EVENT_MGR->AddEventObject("-", D3DXVECTOR3(31.50, -1.7, 21.50), "POSE_8");					//왼쪽 방 << 구석지
	EVENT_MGR->GetEventObj(1)->getEventTrans()->RotateSelf(D3DXVECTOR3(0, ONE_RAD * 60, 0));

	EVENT_MGR->AddEventObject("-", D3DXVECTOR3(46.00, -1.7, 3.81), "POSE_7");					//왼쪽 방 >> 구석지
	EVENT_MGR->GetEventObj(2)->getEventTrans()->RotateSelf(D3DXVECTOR3(0, ONE_RAD * 60, 0));

	EVENT_MGR->AddEventObject("-", D3DXVECTOR3(-26.00, 1.3, 5.45), "POSE_5");					//샤워룸 커튼 뒤 대롱이
	EVENT_MGR->GetEventObj(3)->getEventTrans()->RotateSelf(D3DXVECTOR3(0, ONE_RAD * 60, 0));

	EVENT_MGR->AddEventObject("-", D3DXVECTOR3(-12.00, -1.8, -23.25), "POSE_11");				//T자복도 출구 왼쪽 방 침대 밑
	EVENT_MGR->AddEventObject("-", D3DXVECTOR3(0.24, -1.8, -1.05), "POSE_9");					//엘리베이터 바로 앞 책상 밑

	EVENT_MGR->AddEventObject("-", D3DXVECTOR3(-28.00, 2.8, -7.0), "POSE_9", FALSE, D3DXVECTOR3(-27.0, 3.51, -7.5));

}

void cScene_Game::InitSoundObject()
{

	string soundName = SOUNDDATA->getSoundKey(SOUND_TYPE_STRUCT_FEMALEWARD, SOUND_PLAY_TYPE_CAN, 0);		//엘리베이터 좌측
	EVENT_MGR->AddSoundObject(soundName, 0.3, FALSE, TRUE, D3DXVECTOR3(17.96, 3.51, 6.93));
	soundName = SOUNDDATA->getSoundKey(SOUND_TYPE_STRUCT_FEMALEWARD, SOUND_PLAY_TYPE_BEND, 1);				//왼쪽 방 문앞
	EVENT_MGR->AddSoundObject(soundName, 0.3, FALSE, TRUE, D3DXVECTOR3(27.69, 3.51, 6.93));
	soundName = SOUNDDATA->getSoundKey(SOUND_TYPE_STRUCT_FEMALEWARD, SOUND_PLAY_TYPE_BEND, 2);				//왼쪽방 왼쪽 구석
	EVENT_MGR->AddSoundObject(soundName, 0.3, FALSE, TRUE, D3DXVECTOR3(31.83, 3.51, 20.27));
	soundName = SOUNDDATA->getSoundKey(SOUND_TYPE_STRUCT_FEMALEWARD, SOUND_PLAY_TYPE_FALLOBJECT, 23);		//왼쪽방 오른쪽 구석
	EVENT_MGR->AddSoundObject(soundName, 0.3, FALSE, TRUE, D3DXVECTOR3(45.98, 3.51, 3.67));
	soundName = SOUNDDATA->getSoundKey(SOUND_TYPE_STRUCT_FEMALEWARD, SOUND_PLAY_TYPE_FALLOBJECT, 1);
	EVENT_MGR->AddSoundObject(soundName, 0.3, FALSE, FALSE, D3DXVECTOR3(-14.30, 3.51, 20.92));				//오른쪽방
	soundName = SOUNDDATA->getSoundKey(SOUND_TYPE_STRUCT_FEMALEWARD, SOUND_PLAY_TYPE_BEND, 3);
	EVENT_MGR->AddSoundObject(soundName, 0.3, FALSE, FALSE, D3DXVECTOR3(-27.67, 3.51, 8.87));				//복도 가운데 1번 방
	soundName = SOUNDDATA->getSoundKey(SOUND_TYPE_STRUCT_FEMALEWARD, SOUND_PLAY_TYPE_CAN, 4);
	EVENT_MGR->AddSoundObject(soundName, 0.3, FALSE, FALSE, D3DXVECTOR3(-28.12, 3.51, -16.97));				//복도 가운데 2번 방
	soundName = SOUNDDATA->getSoundKey(SOUND_TYPE_STRUCT_FEMALEWARD, SOUND_PLAY_TYPE_CAN, 5);
	EVENT_MGR->AddSoundObject(soundName, 0.3, FALSE, FALSE, D3DXVECTOR3(-16.27, 3.51, -24.82));				//복도 끝 좌측 방
	soundName = SOUNDDATA->getSoundKey(SOUND_TYPE_STRUCT_FEMALEWARD, SOUND_PLAY_TYPE_CAN, 6);
	EVENT_MGR->AddSoundObject(soundName, 0.3, FALSE, FALSE, D3DXVECTOR3(-27.18, 3.51, -17.36));				//복도 끝 우측 방

	soundName = SOUNDDATA->getSoundKey(SOUND_TYPE_STRUCT_FEMALEWARD, SOUND_PLAY_TYPE_LOOKEVENT, 0);
	EVENT_MGR->AddSoundObject(soundName, 0.5, TRUE, FALSE, D3DXVECTOR3(27.69, 3.51, 6.93));					//왼쪽 방 문앞
	soundName = SOUNDDATA->getSoundKey(SOUND_TYPE_STRUCT_FEMALEWARD, SOUND_PLAY_TYPE_LOOKEVENT, 0);
	EVENT_MGR->AddSoundObject(soundName, 0.5, TRUE, FALSE, D3DXVECTOR3(-14.30, 3.51, 20.92));				//오른쪽방

}

void cScene_Game::Scene_Release()
{
	Pig->Release();
	SAFE_DELETE(Pig);

	Miles->Release();
	SAFE_DELETE(Miles);

	bBox_Save->Scene_Release();
	SAFE_DELETE(bBox_Save);
	bBox_Load->Scene_Release();
	SAFE_DELETE(bBox_Load);

	door->Scene_Release();
	SAFE_DELETE(door);
	m_Player_mapAcive->Scene_Release();
	SAFE_DELETE(m_Player_mapAcive);
	m_Enemy_mapAcive->Scene_Release();
	SAFE_DELETE(m_Enemy_mapAcive);


	cameraUI->Release();
	SAFE_DELETE(cameraUI);
	fileRead->Scene_Release();
	SAFE_DELETE(fileRead);
	movieEvent->Scene_Release();
	SAFE_DELETE(movieEvent);
	eyeEvent->Scene_Release();
	SAFE_DELETE(eyeEvent);


	Ele_Door->Scene_Release();
	SAFE_DELETE(Ele_Door);
	elevatorl->Scene_Release();
	SAFE_DELETE(elevatorl);
	e_passageway->Scene_Release();
	SAFE_DELETE(e_passageway)

	SAFE_DELETE(this->house);



	vCamera.clear();

	for (int i = 0; i < this->lights.size(); i++)
		SAFE_DELETE(this->lights[i]);

	lights.clear();

	for (int i = 0; i < this->vecGameItem.size(); i++)
		SAFE_DELETE(this->vecGameItem[i]);

	vecGameItem.clear();

	for (int i = 0; i < m_vNode.size(); i++)
	{
		SAFE_DELETE(this->m_vNode[i]);
	}
	m_vNode.clear();

	for (int i = 0; i < this->m_vecItemBoundBox.size(); i++)
		SAFE_DELETE(this->m_vecItemBoundBox[i]);

	m_vecItemBoundBox.clear();

	//SAFE_DELETE(keyItem_0);
	//SAFE_DELETE(keyItem_1);
	//SAFE_DELETE(noteItem_2);
	//SAFE_DELETE(noteItem_3);
	//SAFE_DELETE(noteItem_4);
	//SAFE_DELETE(noteItem_5);
	//SAFE_DELETE(noteItem_6);

}

void cScene_Game::Scene_Update(float timeDelta)
{

	if (!Pig->GetisFind() )
	{
		SOUNDDATA->stopSound(SOUND_TYPE_MENU, SOUND_PLAY_TYPE_MENUBGM, 0);
		SOUNDDATA->stopSound(SOUND_TYPE_BGM, SOUND_PLAY_GAME_BGM, 4);
		SOUNDDATA->stopSound(SOUND_TYPE_CUSTOM, SOUND_PLAY_TYPE_CUSTOM, 1);
		SOUNDDATA->playSound(SOUND_TYPE_CUSTOM, SOUND_PLAY_TYPE_CUSTOM, 5, 0.3);
	}
	else if (Pig->GetisFind() && !Pig->GetEventMod())
	{
		SOUNDDATA->stopSound(SOUND_TYPE_MENU, SOUND_PLAY_TYPE_MENUBGM, 0);
		SOUNDDATA->stopSound(SOUND_TYPE_BGM, SOUND_PLAY_GAME_BGM, 4);
		SOUNDDATA->stopSound(SOUND_TYPE_CUSTOM, SOUND_PLAY_TYPE_CUSTOM, 5);
		SOUNDDATA->playSound(SOUND_TYPE_CUSTOM, SOUND_PLAY_TYPE_CUSTOM, 1, 0.3);
	}

	//SOUNDDATA->playSound(SOUND_TYPE_STRUCT_BASEMENT, SOUND_PLAY_TYPE_ELEVATOR2, 0, 1.0);
	//SOUNDDATA->playSound(SOUND_TYPE_STRUCT_BASEMENT, SOUND_PLAY_TYPE_ELEVATOR2, 1, 1.0);
	//SOUNDDATA->playSound(SOUND_TYPE_STRUCT_BASEMENT, SOUND_PLAY_TYPE_ELEVATOR2, 2, 1.0);
	//SOUNDDATA->playSound(SOUND_TYPE_STRUCT_BASEMENT, SOUND_PLAY_TYPE_ELEVATOR2, 3, 1.0);
	//SOUNDDATA->playSound(SOUND_TYPE_STRUCT_BASEMENT, SOUND_PLAY_TYPE_ELEVATOR2, 4, 1.0);

	//SOUNDDATA->playSound(SOUND_TYPE_STRUCT_BASEMENT, SOUND_PLAY_TYPE_OPENDOOR, 0, 0.5);
	//SOUNDDATA->playSound(SOUND_TYPE_STRUCT_BASEMENT, SOUND_PLAY_TYPE_LOCKDOOR, 0, 0.5);

	if (KEY_MGR->IsOnceDown('0'))
	{
		elevatorl->DeBugMode();
	}

	m_EventManager->update();
	//* 마우스 사라지게
	ShowCursor(false);
	
	this->house->Update(timeDelta);
	this->Miles->Update(timeDelta);
	this->Pig->Update(timeDelta);

	//bBox_Save->Scene_Update(timeDelta);
	//bBox_Save->ChoiseBoundBox(this->pMainCamera);

	GainItem();
	ItemMoveToMiles(timeDelta);

	for (int i = 0; i < vecGameItem.size(); i++)
	{
		vecGameItem[i]->Update(timeDelta);
	}

	//* 카메라 올렸으면
	//if (this->Miles->IsCamUp())
	//{
	//	//* 약간 줌인 된 상태에서 시작함.
	//	vCamera[PLAYER]->fov = this->camFov + g_Czoom - 0.1f;
	//}
	//else
	//{
	//	//* 카메라를 내리면 초기 세팅된 그 값으로
	//	vCamera[PLAYER]->fov = this->camFov;
	//	g_Czoom = 0;
	//}

	lights[0]->pTransform = vCamera[PLAYER];



	bBox_Load->Scene_Update(timeDelta);
	door->setRay(camRay);
	door->Scene_Update(timeDelta);
	
	Ele_Door->Scene_Update(timeDelta);
	elevatorl->Scene_Update(timeDelta);
	e_passageway->Scene_Update(timeDelta);

	m_Player_mapAcive->Collision_Inspection(Miles);
	m_Player_mapAcive->BoxActive(Miles);

	if (Pig->getisActive())
	{
		m_Enemy_mapAcive->Collision_Inspection(Pig);
		Pig->setObjBox(bBox_Load->GetAllBox());
		m_Enemy_mapAcive->BoxActive(Pig);
	}

	effect->Effect(timeDelta, pMainCamera);
	cameraUI->Scene_Update(timeDelta);
	fileRead->Scene_Update(timeDelta);
	movieEvent->Scene_Update(timeDelta);
	eyeEvent->Scene_Update(timeDelta);


     ////vCamera[PLAYER]->DefaultControl4(Miles, 5, timeDelta);
     ////vCamera[PLAYER]->SetWorldPosition(Miles->getHeadCamPos());
	 EventEndToNormal();
	 D3DXVECTOR4* elevLightPos = new D3DXVECTOR4(arrDustFog[2]->GetTrans()->GetWorldPosition(), 1.f);
	 arrDustFog[2]->SetLightPos(elevLightPos);

	 //if (Miles->getTrans()->GetWorldPosition().x < EndingX &Miles->getTrans()->GetWorldPosition().z < EndingZ)
	 if(KEY_MGR->IsOnceDown('U'))
	 {
		 SCENE_MGR->ChangeScene("ending", 1);
	 }
	 EVENT_MGR->Update(timeDelta, Miles->getTrans()->GetWorldPosition());
}

void cScene_Game::Scene_Render1()
{
	/*char szTemp[128];

	sprintf_s(szTemp, "%.2f", Miles->getTrans()->GetLocalPosition().x);
	DXFONT_MGR->PrintTextOutline(szTemp, WINSIZE_X - 200, 100, 0xffffff00, 0xff000000);

	sprintf_s(szTemp, " %.2f", Miles->getTrans()->GetLocalPosition().z);
	DXFONT_MGR->PrintTextOutline(szTemp, WINSIZE_X - 130, 100, 0xffffff00, 0xff000000);

	sprintf_s(szTemp, " %.2f", pMainCamera->GetWorldPosition().x);
	DXFONT_MGR->PrintTextOutline(szTemp, WINSIZE_X - 300, 200, 0xffffff00, 0xff000000);

	sprintf_s(szTemp, " %.2f", pMainCamera->GetWorldPosition().y);
	DXFONT_MGR->PrintTextOutline(szTemp, WINSIZE_X - 200, 200, 0xffffff00, 0xff000000);

	sprintf_s(szTemp, " %.2f", pMainCamera->GetWorldPosition().z);
	DXFONT_MGR->PrintTextOutline(szTemp, WINSIZE_X - 100, 200, 0xffffff00, 0xff000000);

*/


	//for (int i = 0; i < this->m_vecItemBoundBox.size(); i++)
	//{
	//	this->m_vecItemBoundBox[i]->BoundBox.RenderGizmo(this->m_vecItemBoundBox[i]->pTransform);
	//}

	//if (bHit == true)
	//{
	////	LOG_MGR->AddLog("hit!");
	//}

	//m_EventManager->Render();

	//적용되는 LightMatrix
	D3DXMATRIXA16 matLights[10];
	for (int i = 0; i < 1; i++)
	{
		matLights[i] = this->lights[i]->GetLightMatrix();
		//	this->lights[i]->RenderGizmo();
	}

	//셰이더에 라이팅 셋팅
	//cXMesh_Static::sStaticMeshEffect->SetTechnique("ReceiveShadow");

	cXMesh_Static::SetLighting(&lights);
	cXMesh_Static::SetCamera(this->pMainCamera);

	cXMesh_Skinned::SetLighting(&lights);
	cXMesh_Skinned::SetCamera(this->pMainCamera);

	//for (int i = 0; i < m_vNode.size(); i++)
	//{ 
	//	m_vNode[i]->Render();
	//}
	Pig->Render();
	Miles->Render();
	this->house->Render();

	//* 아이템
	int size = this->vecGameItem.size();

	for (int i = 0; i < size; i++)
	{
		vecGameItem[i]->ItemRender();
		//vecGameItem[i]->Render();
	}

	//for (int i = 0; i < this->m_vecItemBoundBox.size(); i++)
	//{
	//	this->m_vecItemBoundBox[i]->BoundBox.RenderGizmo(this->m_vecItemBoundBox[i]->pTransform);
	//}
	//bBox_Load->Scene_Render1();
	door->Scene_Render1();

	Ele_Door->Scene_Render1();
	elevatorl->Scene_Render1();
	e_passageway->Scene_Render1();

	//페이크 빛 렌더============================================================
	D3DXMATRIXA16 matTempVP = pMainCamera->GetViewProjectionMatrix();
	cDustFog::SetMatVP(&matTempVP);
	for (int i = 0; i < arrDustFog.size(); i++)
		arrDustFog[i]->Render();

	cLightGlow::SetMatVP(&matTempVP);
	for (int i = 0; i < arrGlow.size(); i++)
		arrGlow[i]->Render();
	//페이크 빛 렌더============================================================
	EVENT_MGR->Render(pMainCamera, lights);

}

void cScene_Game::GainItem()
{
	if (fileRead->GetFileInfo()->count == 1)
	{
		if (KEY_MGR->IsOnceDown(VK_LBUTTON))
		{
			fileRead->GetFileInfo()->count = 2;
			Miles->IsCamContoroll(true);
			Miles->IsContoroll(true);
			if (!Pig->GetEventMod()) Pig->setisActive(true);
			return;
		}
	}
	POINT ptMouse = GetMousePos();
	D3DXVECTOR2 screenPos(ptMouse.x, ptMouse.y);

	this->pMainCamera->ComputeRay(
		&this->camRay,
		&screenPos);

	//* map 에 있는 바운드박스의 개수만큼
	for (int i = 5; i < this->m_vecItemBoundBox.size(); i++)
	{
		bHitBoundBox = PHYSICS_MGR->IsOverlap(Miles->GetItemPocketTrans(), Miles->GetItemPocketBound(),
			m_vecItemBoundBox[i]->pTransform, &m_vecItemBoundBox[i]->BoundBox);

		//* boundbox랑 부딪히면
		if (this->bHitBoundBox == true)
		{
			this->bHitRay = PHYSICS_MGR->IsRayHitBound(
				&this->camRay,
				&this->vecGameItem[i]->BoundBox,
				this->vecGameItem[i]->pTransform,
				&this->hitPos,
				&this->hitNormal);
			//* Ray 가 감지하면
			if (this->bHitRay == true)
			{

				this->m_itemIndex = i;
				//* 아이템 클릭하면
				if (KEY_MGR->IsOnceDown(VK_LBUTTON) && vecGameItem[this->m_itemIndex]->GetItemType() == KEYITEM)
				{
					this->bItemClick = true;
				}
				ItemUseFile(bHitRay, i);
			}
		}
	}

	for (int i = 0; i < 5; i++)
	{
		bHitBoundBox = PHYSICS_MGR->IsOverlap(Miles->GetItemPocketTrans(), Miles->GetItemPocketBound(),
			m_vecItemBoundBox[i]->pTransform, &m_vecItemBoundBox[i]->BoundBox);

		//* boundbox랑 부딪히면
		if (this->bHitBoundBox == true)
		{
			this->bHitRay = PHYSICS_MGR->IsRayHitBound(
				&this->camRay,
				&this->vecGameItem[i]->BoundBox,
				this->vecGameItem[i]->pTransform,
				&this->hitPos,
				&this->hitNormal);
			//* Ray 가 감지하면
			if (this->bHitRay == true)
			{
				ItemUseFile(bHitRay, i);
			}
		}
	}
}

void cScene_Game::ItemMoveToMiles(float timeDelta)
{
	//* 클릭했을 경우 아이템을 보간이동하는 코드를 가지고 있음
	if (bItemClick == true)
	{
		//* 열쇠일 경우에만
		if (vecGameItem[this->m_itemIndex]->GetItemType() == KEYITEM)
		{
			//* 아이템이 마일즈쪽으로 움직이게
			vecGameItem[this->m_itemIndex]->pTransform->PositionLerp(
				*vecGameItem[this->m_itemIndex]->pTransform,
				*this->ItemPocket,
				this->ItemSpeed += 0.2 * timeDelta);

			//* 마일즈와 닿으면 
			if (PHYSICS_MGR->IsOverlap(Miles->GetItemPocketTrans(), Miles->GetItemPocketBound(),
				vecGameItem[this->m_itemIndex]->pTransform,
				&vecGameItem[this->m_itemIndex]->BoundBox))
			{
				if (this->m_itemIndex == 5)
					door->UnLockDoor(3);
				if (this->m_itemIndex == 6)
					door->UnLockDoor(5);

				//*아이템을 먹으면 해당 아이템, 그리고 아이템을 감싸고 있던 바운드 박스를 삭제해준다.
				this->vecGameItem.erase(vecGameItem.begin() + m_itemIndex);
				this->m_vecItemBoundBox.erase(m_vecItemBoundBox.begin() + m_itemIndex);

				//* 감지하는 bool 값들을 다시 false로!
				this->bHitBoundBox = false; //바운드박스
				this->bHitRay = false; //레이
				this->bItemClick = false; //클릭
			}
		}
	}
}



void cScene_Game::NodeInit()
{
	DijkNodeFileIO("복도1.txt");
	DijkNodeFileIO("복도2.txt");
	DijkNodeFileIO("수술실.txt");
	DijkNodeFileIO("방1.txt");
	DijkNodeFileIO("창고.txt");
	DijkNodeFileIO("화장실.txt");
	DijkNodeFileIO("방2.txt");
	DijkNodeFileIO("방3.txt");
	DijkNodeFileIO("방4.txt");
	DijkNodeFileIO("방5.txt");

	m_vNode[13]->pushEdgeNode(m_vNode[12]);
	m_vNode[12]->pushEdgeNode(m_vNode[13]);

	//수술실과 복도1 연결
	m_vNode[5]->pushEdgeNode(m_vNode[26]);
	m_vNode[26]->pushEdgeNode(m_vNode[5]);

	//방1 과 복도1 연결
	m_vNode[0]->pushEdgeNode(m_vNode[41]);
	m_vNode[41]->pushEdgeNode(m_vNode[0]);

	//방1과 창고 연결
	m_vNode[46]->pushEdgeNode(m_vNode[47]);
	m_vNode[47]->pushEdgeNode(m_vNode[46]);

	//화장실 연결
	m_vNode[50]->pushEdgeNode(m_vNode[14]);
	m_vNode[14]->pushEdgeNode(m_vNode[50]);

	// 문없는 방2 연결
	m_vNode[16]->pushEdgeNode(m_vNode[54]);
	m_vNode[54]->pushEdgeNode(m_vNode[16]);

	// 십자가 방3 연결
	m_vNode[19]->pushEdgeNode(m_vNode[56]);
	m_vNode[56]->pushEdgeNode(m_vNode[19]);

	// 침대 방4 연결
	m_vNode[21]->pushEdgeNode(m_vNode[58]);
	m_vNode[58]->pushEdgeNode(m_vNode[21]);

	// 침대 방4 연결
	m_vNode[23]->pushEdgeNode(m_vNode[60]);
	m_vNode[60]->pushEdgeNode(m_vNode[23]);

	int a = 0;
}

void cScene_Game::DijkNodeFileIO(const char* filename)
{

	FILE* fp;
	char* Str = NULL;
	Str = new char[255];
	int i = 0;
	int NodeNum;
	int StartNum = m_vNode.size();

	// Vertice Num과 Point Num을 파일에서 가져오자!
	if (fopen_s(&fp, filename, "r") == NULL) {
		fscanf_s(fp, "%d", &NodeNum);
	}
	else {
		puts("File Open Error");
		exit(0);
	}

	for (int i = 0; i < NodeNum; i++) {
		cNode* node = new cNode();
		float pt[3];
		int Idx, EdgeNum;

		for (int j = 0; j < 3; j++)
		{
			fscanf_s(fp, "%f", &pt[j]);
			pt[j] = pt[j];
		}

		node->Init(D3DXVECTOR3(pt[0], pt[1], pt[2]));
		node->setID(m_vNode.size());

		fscanf_s(fp, "%d", &EdgeNum);
		node->setEdgeNum(EdgeNum);

		for (int k = 0; k < EdgeNum; k++) {
			int Idx;
			fscanf_s(fp, "%d", &Idx);
			node->PushIdx(Idx + StartNum);
		}

		m_vNode.push_back(node);
	}

	for (int i = StartNum; i < NodeNum + StartNum; i++)
	{
		for (int j = 0; j < m_vNode[i]->getEdgeNum(); j++)
			m_vNode[i]->pushEdgeNode(m_vNode[m_vNode[i]->getEdgeIdx(j)]);
	}

	fclose(fp);
}

void cScene_Game::InnerOpenDoor(int Idx) 
{
	door->InnerOpen(Idx);
}

void cScene_Game::OutterOpenDoor(int Idx) 
{
	door->OutterOpen(Idx);
}
void cScene_Game::CloseDoor(int Idx)
{
	door->DoorClose(Idx);
}


void cScene_Game::Event1End()
{
	SOUNDDATA->playSound(SOUND_TYPE_PLAYER_VOICE, SOUND_PLAY_TYPE_PLAYER_FEAR, 0);
	CloseEye(1, 5);
	Pig->getTrans()->SetWorldPosition(-14.94, 0.00, -24.81);
	isEvent1End = true;
}

void cScene_Game::EventEndToNormal()
{
	if (!eyeEvent->isPlayEye && this->isEvent1End)
	{
		SOUNDDATA->playSound(SOUND_TYPE_PLAYER_VOICE, SOUND_PLAY_TYPE_PLAYER_ATTACKED_RELAX, 0);
		this->isEvent1End = false;
		isGameEvent = false;
		Miles->IsContoroll(true);
		Miles->IsCamContoroll(true);
		Pig->setisActive(true);
		Pig->SetEventMod(false);
		Miles->getTrans()->SetWorldPosition(Miles->getTrans()->GetWorldPosition().x, 0.0f, Miles->getTrans()->GetWorldPosition().z); // 마일즈 위치 조정
	}

}

void cScene_Game::Event1Start()
{
	//isGameEvent = true;
	SOUNDDATA->playSound(SOUND_TYPE_NPC, SOUND_PLAY_TYPE_NPC_BREECHING, 0);

	//Pig->getTrans()->SetLocalPosition(6.78, 0, -16.50); // 돼지 자리잡고
	//Pig->getTrans()->LookDirection(Pig->getTrans()->GetWorldPosition()); // 마일즈 바라보게 하고
	//Pig->StateOneShotHoldChange("GRAB_FATALITY", 0.5f); // 애니메이션실행
	//	Pig->getTrans()->SetLocalPosition(10, 0, -17.50); // 돼지 자리잡고
	Pig->setisActive(true); // 활성화 false
	Pig->getTrans()->SetWorldPosition(D3DXVECTOR3(6.46, 0, -17.00));

	//Miles->getTrans()->LookDirection(-Pig->getTrans()->GetWorldPosition()); //마일즈는 돼지 보고
	//Miles->StateOneShotHoldChange("GRAB_AND_THROW", 0.5f); // 잡히는 애니메이션
//	Miles->IsContoroll(false);
//	Miles->IsCamContoroll(false);
}



