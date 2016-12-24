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

bool cScene_Game::m_bIsBroken = false;
bool cScene_Game::m_bNightVision = false;

cScene_Game::cScene_Game()
{
}


cScene_Game::~cScene_Game()
{

}

void cScene_Game::NodeInit()
{
	DijkNodeFileIO("����1.txt");
	DijkNodeFileIO("����2.txt");
	DijkNodeFileIO("������.txt");
	DijkNodeFileIO("��1.txt");
	DijkNodeFileIO("â��.txt");
	DijkNodeFileIO("ȭ���.txt");
	DijkNodeFileIO("��2.txt");
	DijkNodeFileIO("��3.txt");
	DijkNodeFileIO("��4.txt");
	DijkNodeFileIO("��5.txt");

	m_vNode[13]->pushEdgeNode(m_vNode[12]);
	m_vNode[12]->pushEdgeNode(m_vNode[13]);

	//�����ǰ� ����1 ����
	m_vNode[5]->pushEdgeNode(m_vNode[26]);
	m_vNode[26]->pushEdgeNode(m_vNode[5]);

	//��1 �� ����1 ����
	m_vNode[0]->pushEdgeNode(m_vNode[41]);
	m_vNode[41]->pushEdgeNode(m_vNode[0]);

	//��1�� â�� ����
	m_vNode[46]->pushEdgeNode(m_vNode[47]);
	m_vNode[47]->pushEdgeNode(m_vNode[46]);

	//ȭ��� ����
	m_vNode[51]->pushEdgeNode(m_vNode[14]);
	m_vNode[14]->pushEdgeNode(m_vNode[51]);

	// ������ ��2 ����
	m_vNode[16]->pushEdgeNode(m_vNode[55]);
	m_vNode[55]->pushEdgeNode(m_vNode[16]);

	// ���ڰ� ��3 ����
	m_vNode[19]->pushEdgeNode(m_vNode[57]);
	m_vNode[57]->pushEdgeNode(m_vNode[19]);

	// ħ�� ��4 ����
	m_vNode[21]->pushEdgeNode(m_vNode[59]);
	m_vNode[59]->pushEdgeNode(m_vNode[21]);

	// ħ�� ��4 ����
	m_vNode[23]->pushEdgeNode(m_vNode[61]);
	m_vNode[61]->pushEdgeNode(m_vNode[23]);



	//int a = 0;
}

void cScene_Game::DijkNodeFileIO(const char* filename)
{

	FILE* fp;
	char* Str = NULL;
	Str = new char[255];
	int i = 0;
	int NodeNum;
	int StartNum = m_vNode.size();

	// Vertice Num�� Point Num�� ���Ͽ��� ��������!
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


	//����Ʈ Ǫ��
	Pig = new PigEnemy("../Resources/Meshes/Pig/20161129_Pig.X", { 5,0,0 }, this);
	//Pig->CameraAttachToUnit(vCamera[ENEMY]);

	effect = new cEffect();
	effect->Scene_Init();

	Miles = new Player("../Resources/Meshes/Miles/miles.X", { 0,0,0 });
	Miles->CameraAttachToUnit(vCamera[PLAYER]);
	Miles->MemoryEffectGet(effect);
	Miles->getTrans()->SetWorldPosition(MilESPOSX, MilESPOSY, MilESPOSZ);


	cameraUI = new cCameraUI(vCamera[PLAYER], Miles);
	cameraUI->Scene_Init();
	fileRead = new cFileRead();
	fileRead->Scene_Init();
	movieEvent = new cMovieEvent();
	movieEvent->Scene_Init();
	eyeEvent = new cCloseEyeEvent();
	eyeEvent->Scene_Init();


	bBox_Save = new cMap_Bbox_Save();
	bBox_Save->Scene_Init();

	bBox_Load = new cMap_Bbox_Load();
	bBox_Load->Scene_Init();

	door = new cDoorPosition(Miles, Pig);
	door->Scene_Init();

	door->InnerOpen(0); // 1���� �������� ������

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

	cLight_Direction* pLight2 = new cLight_Direction;

	pLight2->Color = D3DXCOLOR(0.1, 0.1, 0.1, 1);
	pLight2->pTransform->SetWorldPosition(0, 1, 0);

	this->lights.push_back(pLight2);
	//this->lights.push_back(pLight3);


	/* ũ�ν���� */
	this->lockCrosshairTex = RESOURCE_TEXTURE->LoadResource("../Resources/OLGame/UI/Crosshair/lock.png");
	this->unlockCrosshairTex = RESOURCE_TEXTURE->LoadResource("../Resources/OLGame/UI/Crosshair/unlock.png");

	//* ������ �߰�
	this->keyItem = new cGameItem();
	this->keyItem->Init(KEYITEM, "../Resources/OLGame/Meshes/Item/Key/Key.X",
		D3DXVECTOR3(0.035f, 0.035f, 0.035f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(3.0f, 0.02f, 0.0f)
		);
	this->vecGameItem.push_back(this->keyItem);


	this->noteItem = new cGameItem();
	this->noteItem->Init(NOTEITEM, "../Resources/OLGame/Meshes/Item/File/File.X",
		D3DXVECTOR3(0.02f, 0.02f, 0.02f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f)
		);
	this->vecGameItem.push_back(this->noteItem);

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

	cEventBox* Box = new cFirstEvent(Miles, Pig, D3DXVECTOR3(6.49, 3.36, -14.8), D3DXVECTOR3(1, 1, 1), this);
	m_EventManager = new cEventManager();
	m_EventManager->PushEventBox(Box);

	//* gameitem �� ������ �ִ°Ÿ�ŭ boundBox�� ���� ��ġ���ش�.
	for (int i = 0; i < this->vecGameItem.size(); i++)
	{ //* gameItem�� ���� ���� ��ġ���ٰ�!
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



	//SOUNDDATA->playSound(SOUND_TYPE_BGM, SOUND_PLAY_GAME_BGM, 0, 0.1);


	InitEventObject();
	InitSoundObject();

	return S_OK;
}

void cScene_Game::Scene_RenderSprite()
{
	//*point
	RECT rc = { 0, 0, 70, 70 };

	if (bHitRay == true)
		SPRITE_MGR->DrawTexture(this->lockCrosshairTex, &rc, WINSIZE_X / 2, WINSIZE_Y / 2, 0xffffffff, &D3DXVECTOR3(0, 0, 0));
	else
		SPRITE_MGR->DrawTexture(this->unlockCrosshairTex, &rc, WINSIZE_X / 2, WINSIZE_Y / 2, 0xffffffff, &D3DXVECTOR3(0, 0, 0));

	effect->Scene_RenderSprite();
	cameraUI->Scene_RenderSprite();
	movieEvent->Scene_RenderSprite();
	eyeEvent->Scene_RenderSprite();

}

void cScene_Game::ControllNightVision()
{	
	if (KEY_MGR->IsOnceDown('F') && Miles->IsCamUp())
	{
		SOUNDDATA->stopSound(SOUND_TYPE_PLAYER_EFFECT, SOUND_PLAY_TYPE_CAM_VISION);
		SOUNDDATA->playSound(SOUND_TYPE_PLAYER_EFFECT, SOUND_PLAY_TYPE_CAM_VISION);
		m_bNightVision = !m_bNightVision;
	}
}

void cScene_Game::ControllCamMode()
{
	if (KEY_MGR->IsOnceDown(VK_RBUTTON))
	{
		Miles->CamControll();
		cameraUI->TurnOnCam();
	}
}


void cScene_Game::InitEventObject()
{
	//EVENT_MGR->AddEventObject("test1", D3DXVECTOR3(-27.58, 3.51, -7.65));

	/*Object Dead Pose List
	POSE_0 ~ 4	: Sit
	POSE_5		: Looping
	POSE_6		: Cross
	POSE_7 ~ 12 : Floor
	*/

	EVENT_MGR->AddEventObject("-", D3DXVECTOR3(12.60, -1.0, 6.8),	 "POSE_4");					//��ü�� ��ġ
	EVENT_MGR->GetEventObj(0)->getEventTrans()->RotateSelf(D3DXVECTOR3(0, ONE_RAD * 120, 0));

	EVENT_MGR->AddEventObject("-", D3DXVECTOR3(31.50, -1.7, 21.50),	 "POSE_8");					//���� �� << ������
	EVENT_MGR->GetEventObj(1)->getEventTrans()->RotateSelf(D3DXVECTOR3(0, ONE_RAD * 60, 0));

	EVENT_MGR->AddEventObject("-", D3DXVECTOR3(46.00, -1.7, 3.81),	 "POSE_7");					//���� �� >> ������
	EVENT_MGR->GetEventObj(2)->getEventTrans()->RotateSelf(D3DXVECTOR3(0, ONE_RAD * 60, 0));

	EVENT_MGR->AddEventObject("-", D3DXVECTOR3(-26.00, 1.3, 5.45),	 "POSE_5");					//������ Ŀư �� �����
	EVENT_MGR->GetEventObj(3)->getEventTrans()->RotateSelf(D3DXVECTOR3(0, ONE_RAD * 60, 0));

	EVENT_MGR->AddEventObject("-", D3DXVECTOR3(-12.00, -1.8, -23.25), "POSE_11");				//T�ں��� �ⱸ ���� �� ħ�� ��
	EVENT_MGR->AddEventObject("-", D3DXVECTOR3(0.24, -1.8, -1.05),	 "POSE_9");					//���������� �ٷ� �� å�� ��

	EVENT_MGR->AddEventObject("-", D3DXVECTOR3(-21.00, 2.8, 7.0), "POSE_9", FALSE, D3DXVECTOR3(-3.0, 3.51, 7.0));


	
}

void cScene_Game::InitSoundObject()
{
	string soundName = SOUNDDATA->getSoundKey(SOUND_TYPE_STRUCT_FEMALEWARD, SOUND_PLAY_TYPE_CAN, 0);		//���������� ����
	EVENT_MGR->AddSoundObject(soundName, 0.3, FALSE, TRUE, D3DXVECTOR3(17.96, 3.51, 6.93));
	soundName = SOUNDDATA->getSoundKey(SOUND_TYPE_STRUCT_FEMALEWARD, SOUND_PLAY_TYPE_BEND, 1);				//���� �� ����
	EVENT_MGR->AddSoundObject(soundName, 0.3, FALSE, TRUE, D3DXVECTOR3(27.69, 3.51, 6.93));
	soundName = SOUNDDATA->getSoundKey(SOUND_TYPE_STRUCT_FEMALEWARD, SOUND_PLAY_TYPE_BEND, 2);				//���ʹ� ���� ����
	EVENT_MGR->AddSoundObject(soundName, 0.3, FALSE, TRUE, D3DXVECTOR3(31.83, 3.51, 20.27));
	soundName = SOUNDDATA->getSoundKey(SOUND_TYPE_STRUCT_FEMALEWARD, SOUND_PLAY_TYPE_FALLOBJECT, 23);		//���ʹ� ������ ����
	EVENT_MGR->AddSoundObject(soundName, 0.3, FALSE, TRUE, D3DXVECTOR3(45.98, 3.51, 3.67));
	soundName = SOUNDDATA->getSoundKey(SOUND_TYPE_STRUCT_FEMALEWARD, SOUND_PLAY_TYPE_FALLOBJECT, 1);
	EVENT_MGR->AddSoundObject(soundName, 0.3, FALSE, FALSE, D3DXVECTOR3(-14.30, 3.51, 20.92));				//�����ʹ�
	soundName = SOUNDDATA->getSoundKey(SOUND_TYPE_STRUCT_FEMALEWARD, SOUND_PLAY_TYPE_BEND, 3);
	EVENT_MGR->AddSoundObject(soundName, 0.3, FALSE, FALSE, D3DXVECTOR3(-27.67, 3.51, 8.87));				//���� ��� 1�� ��
	soundName = SOUNDDATA->getSoundKey(SOUND_TYPE_STRUCT_FEMALEWARD, SOUND_PLAY_TYPE_CAN, 4);
	EVENT_MGR->AddSoundObject(soundName, 0.3, FALSE, FALSE, D3DXVECTOR3(-28.12, 3.51, -16.97));				//���� ��� 2�� ��
	soundName = SOUNDDATA->getSoundKey(SOUND_TYPE_STRUCT_FEMALEWARD, SOUND_PLAY_TYPE_CAN, 5);
	EVENT_MGR->AddSoundObject(soundName, 0.3, FALSE, FALSE, D3DXVECTOR3(-16.27, 3.51, -24.82));				//���� �� ���� ��
	soundName = SOUNDDATA->getSoundKey(SOUND_TYPE_STRUCT_FEMALEWARD, SOUND_PLAY_TYPE_CAN, 6);
	EVENT_MGR->AddSoundObject(soundName, 0.3, FALSE, FALSE, D3DXVECTOR3(-27.18, 3.51, -17.36));				//���� �� ���� ��
	
	soundName = SOUNDDATA->getSoundKey(SOUND_TYPE_STRUCT_FEMALEWARD, SOUND_PLAY_TYPE_LOOKEVENT, 0);			
	EVENT_MGR->AddSoundObject(soundName, 0.5, TRUE, FALSE, D3DXVECTOR3(27.69, 3.51, 6.93));					//���� �� ����
	soundName = SOUNDDATA->getSoundKey(SOUND_TYPE_STRUCT_FEMALEWARD, SOUND_PLAY_TYPE_LOOKEVENT, 0);			
	EVENT_MGR->AddSoundObject(soundName, 0.5, TRUE, FALSE, D3DXVECTOR3(-14.30, 3.51, 20.92));				//�����ʹ�

}

void cScene_Game::CloseEye(int num)
{
	eyeEvent->SetisPlayEye(true);
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
	m_Player_mapAcive->Release();
	SAFE_DELETE(m_Player_mapAcive);
	m_Enemy_mapAcive->Release();
	SAFE_DELETE(m_Enemy_mapAcive);

	effect->Release();
	SAFE_DELETE(effect);
	cameraUI->Release();
	SAFE_DELETE(cameraUI);
	fileRead->Release();
	SAFE_DELETE(fileRead);
	movieEvent->Release();
	SAFE_DELETE(movieEvent);
	eyeEvent->Release();
	SAFE_DELETE(eyeEvent);


	Ele_Door->Release();
	SAFE_DELETE(Ele_Door);
	elevatorl->Release();
	SAFE_DELETE(elevatorl);
	e_passageway->Release();
	SAFE_DELETE(e_passageway)

	SAFE_DELETE(this->house);
	for (int i = 0; i < this->lights.size(); i++)
		SAFE_DELETE(this->lights[i]);

	for (int i = 0; i < this->vecGameItem.size(); i++)
		SAFE_DELETE(this->vecGameItem[i]);

	for (int i = 0; i < m_vNode.size(); i++)
	{
		SAFE_DELETE(this->m_vNode[i]);
	}

	for (int i = 0; i < this->m_vecItemBoundBox.size(); i++)
		SAFE_DELETE(this->m_vecItemBoundBox[i]);

}

void cScene_Game::Scene_Update(float timeDelta)
{		
	if (KEY_MGR->IsOnceDown('0'))
	{
		elevatorl->DeBugMode();

	}

	m_EventManager->update();
	//* ���콺 �������
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

	//* ī�޶� �÷�����
	//if (this->Miles->IsCamUp())
	//{
	//	//* �ణ ���� �� ���¿��� ������.
	//	vCamera[PLAYER]->fov = this->camFov + g_Czoom - 0.1f;
	//}
	//else
	//{
	//	//* ī�޶� ������ �ʱ� ���õ� �� ������
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
		Pig->setObjBox(m_Enemy_mapAcive->BoxRayActive());
		m_Enemy_mapAcive->BoxActive(Pig);
	}



	effect->Effect(timeDelta, pMainCamera);
	cameraUI->Scene_Update(timeDelta);
	fileRead->Scene_Update(timeDelta);
	movieEvent->Scene_Update(timeDelta);
	eyeEvent->Scene_Update(timeDelta);


     vCamera[PLAYER]->DefaultControl4(Miles, 5, timeDelta);
     vCamera[PLAYER]->SetWorldPosition(Miles->getHeadCamPos());
	 ControllNightVision();
	 ControllCamMode();
	 EventEndToNormal();

	 EVENT_MGR->Update(timeDelta, Miles->getTrans()->GetWorldPosition());
}

void cScene_Game::Scene_Render1()
{
	char szTemp[128];

	//sprintf_s(szTemp, "%.2f", Miles->getTrans()->GetLocalPosition().x);
	//DXFONT_MGR->PrintTextOutline(szTemp, WINSIZE_X - 200, 100, 0xffffff00, 0xff000000);

	//sprintf_s(szTemp, " %.2f", Miles->getTrans()->GetLocalPosition().z);
	//DXFONT_MGR->PrintTextOutline(szTemp, WINSIZE_X - 130, 100, 0xffffff00, 0xff000000);

	sprintf_s(szTemp, " x :%.2f y:%.2f z:%.2f", pMainCamera->GetWorldPosition().x, pMainCamera->GetWorldPosition().y, pMainCamera->GetWorldPosition().z);
	DXFONT_MGR->PrintTextOutline(szTemp, WINSIZE_X - 400, 150, 0xffffff00, 0xff000000);

	//if (Miles->getRAy())
	//{
	//	sprintf_s(szTemp, "O,");
	//	DXFONT_MGR->PrintTextOutline(szTemp, WINSIZE_X - 155, 150, 0xffffff00, 0xff000000);
	//}
	//else
	//{
	//	sprintf_s(szTemp, "X,");
	//	DXFONT_MGR->PrintTextOutline(szTemp, WINSIZE_X - 155, 150, 0xffffff00, 0xff000000);
	//}


	for (int i = 0; i < this->m_vecItemBoundBox.size(); i++)
	{
		this->m_vecItemBoundBox[i]->BoundBox.RenderGizmo(this->m_vecItemBoundBox[i]->pTransform);
	}

	if (bHit == true)
	{
	//	LOG_MGR->AddLog("hit!");
	}

	m_EventManager->Render();

	//����Ǵ� LightMatrix
	D3DXMATRIXA16 matLights[10];
	for (int i = 0; i < 1; i++)
	{
		matLights[i] = this->lights[i]->GetLightMatrix();
		//	this->lights[i]->RenderGizmo();
	}

	//���̴��� ������ ����
	//cXMesh_Static::sStaticMeshEffect->SetTechnique("ReceiveShadow");
	cXMesh_Static::sStaticMeshEffect->SetMatrixArray("matLights", matLights, 1);
	cXMesh_Static::sStaticMeshEffect->SetInt("LightNum", this->lights.size());
	cXMesh_Static::SetCamera(this->pMainCamera);

	cXMesh_Skinned::sSkinnedMeshEffect->SetMatrixArray("matLights", matLights, 10);
	cXMesh_Skinned::sSkinnedMeshEffect->SetInt("LightNum", this->lights.size());
	cXMesh_Skinned::SetCamera(this->pMainCamera);


	for (int i = 0; i < m_vNode.size(); i++)
	{ 
		m_vNode[i]->Render();
	}
	Pig->Render();
	Miles->Render();
	this->house->Render();

	//* ������
	int size = this->vecGameItem.size();
	for (int i = 0; i < size; i++)
	{
		vecGameItem[i]->Render();
	}

	for (int i = 0; i < this->m_vecItemBoundBox.size(); i++)
	{
		this->m_vecItemBoundBox[i]->BoundBox.RenderGizmo(this->m_vecItemBoundBox[i]->pTransform);
	}
	//	this->air->Render();
	bBox_Load->Scene_Render1();
	door->Scene_Render1();

	Ele_Door->Scene_Render1();
	elevatorl->Scene_Render1();
	e_passageway->Scene_Render1();

	fileRead->Scene_Render1();


	EVENT_MGR->Render(pMainCamera, lights);
}

void cScene_Game::GainItem()
{

	POINT ptMouse = GetMousePos();
	D3DXVECTOR2 screenPos(ptMouse.x, ptMouse.y);

	this->pMainCamera->ComputeRay(
		&this->camRay,
		&screenPos);

	//* map �� �ִ� �ٿ��ڽ��� ������ŭ
	for (int i = 0; i < this->m_vecItemBoundBox.size(); i++)
	{
		bHitBoundBox = PHYSICS_MGR->IsOverlap(Miles->GetItemPocketTrans(), Miles->GetItemPocketBound(),
			m_vecItemBoundBox[i]->pTransform, &m_vecItemBoundBox[i]->BoundBox);

		//* boundbox�� �ε�����
		if (this->bHitBoundBox == true)
		{
			this->bHitRay = PHYSICS_MGR->IsRayHitStaticMeshObject(
				&this->camRay,
				this->vecGameItem[i],
				&this->hitPos,
				&this->hitNormal);

			//* Ray �� �����ϸ�
			if (this->bHitRay == true)
			{
				this->m_itemIndex = i;
				//* ������ Ŭ���ϸ�
				if (KEY_MGR->IsOnceDown(VK_LBUTTON))
				{
					this->bItemClick = true;
				}
			}
		}
	}
}

void cScene_Game::ItemMoveToMiles(float timeDelta)
{
	//* Ŭ������ ��� �������� �����̵��ϴ� �ڵ带 ������ ����
	if (bItemClick == true)
	{
		//* ������ ��쿡��
		if (vecGameItem[this->m_itemIndex]->GetItemType() == KEYITEM)
		{
			//* �������� ������������ �����̰�
			vecGameItem[this->m_itemIndex]->pTransform->PositionLerp(
				*vecGameItem[this->m_itemIndex]->pTransform,
				*this->ItemPocket,
				this->ItemSpeed += 0.2 * timeDelta);

			//* ������� ������ 
			if (PHYSICS_MGR->IsOverlap(Miles->GetItemPocketTrans(), Miles->GetItemPocketBound(),
				vecGameItem[this->m_itemIndex]->pTransform,
				&vecGameItem[this->m_itemIndex]->BoundBox))
			{
				//*�������� ������ �ش� ������, �׸��� �������� ���ΰ� �ִ� �ٿ�� �ڽ��� �������ش�.
				this->vecGameItem.erase(vecGameItem.begin() + m_itemIndex);
				this->m_vecItemBoundBox.erase(m_vecItemBoundBox.begin() + m_itemIndex);

				//* �����ϴ� bool ������ �ٽ� false��!
				this->bHitBoundBox = false; //�ٿ��ڽ�
				this->bHitRay = false; //����
				this->bItemClick = false; //Ŭ��

			}
		}
	}
}

void cScene_Game::Event1End()
{
	eyeEvent->isPlayEye = true;
	Pig->getTrans()->SetWorldPosition(-14.94 ,0.00, - 24.81);
	Pig->setisActive(true);
	isEvent1End = true;
	
}

void cScene_Game::EventEndToNormal()
{
	if (!eyeEvent->isPlayEye && this->isEvent1End)
	{
		this->isEvent1End = false;
		isGameEvent = false;
		Miles->getTrans()->SetWorldPosition(6.78, 0.0f, -13.80); // ������ ��ġ ����
	}

}

void cScene_Game::Event1Start()
{
	isGameEvent = true;
	door->InnerOpen(1); // 1���� �������� ������

	Pig->getTrans()->SetLocalPosition(6.78, 0, -15.50); // ���� �ڸ����
	Pig->getTrans()->LookDirection(Miles->getTrans()->GetWorldPosition()); // ������ �ٶ󺸰� �ϰ�
	Pig->StateOneShotHoldChange("GRAB_FATALITY", 0.5f); // �ִϸ��̼ǽ���
	Pig->setisActive(false); // Ȱ��ȭ false


	Miles->getTrans()->LookDirection(-Pig->getTrans()->GetWorldPosition()); //������� ���� ����
	Miles->StateOneShotHoldChange("GRAB_AND_THROW", 0.5f); // ������ �ִϸ��̼�
	Miles->getTrans()->SetWorldPosition(6.78, 0.8f, -14.20); // ������ ��ġ ����
}



