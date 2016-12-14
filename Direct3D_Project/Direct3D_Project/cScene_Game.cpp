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
cScene_Game::cScene_Game()
{
}


cScene_Game::~cScene_Game()
{

}

void cScene_Game::NodeInit()
{

	cNode* node = new cNode();

	node = new cNode();
	node->Init(D3DXVECTOR3(3, 0.0f, 0.f));
	node->setID(m_vNode.size());
	m_vNode.push_back(node);

	node = new cNode();
	node->Init(D3DXVECTOR3(3, 0.0f, 3.f));
	node->setID(m_vNode.size());

	m_vNode.push_back(node);



	node = new cNode();
	node->Init(D3DXVECTOR3(3.3, 0.0f, 6.f));
	node->setID(m_vNode.size());
	m_vNode.push_back(node);




	node = new cNode();
	node->Init(D3DXVECTOR3(6.0f, 0.0f, 6.f));
	node->setID(m_vNode.size());

	m_vNode.push_back(node);



	node = new cNode();
	node->Init(D3DXVECTOR3(11, 0.0f, 8.5f));
	node->setID(m_vNode.size());
	m_vNode.push_back(node);

	node = new cNode();
	node->Init(D3DXVECTOR3(13, 0.0f, 8.92f));
	node->setID(m_vNode.size());
	m_vNode.push_back(node);

	node = new cNode();
	node->Init(D3DXVECTOR3(17.44, 0.0f, 7.f));
	node->setID(m_vNode.size());
	m_vNode.push_back(node);


	node = new cNode();
	node->Init(D3DXVECTOR3(20.36f, 0.0f, 7.f));
	node->setID(m_vNode.size());
	m_vNode.push_back(node);


	node = new cNode();
	node->Init(D3DXVECTOR3(23.32f, 0.0f, 7.f));
	node->setID(m_vNode.size());
	m_vNode.push_back(node);

	node = new cNode();
	node->Init(D3DXVECTOR3(25.70f, 0.0f, 7.f));
	node->setID(m_vNode.size());
	m_vNode.push_back(node);


	node = new cNode();
	node->Init(D3DXVECTOR3(30.32f, 0.0f, 7.f));
	node->setID(m_vNode.size());
	m_vNode.push_back(node);

	node = new cNode();
	node->Init(D3DXVECTOR3(33.78f, 0.0f, 4.32));
	node->setID(m_vNode.size());
	m_vNode.push_back(node);

	node = new cNode();
	node->Init(D3DXVECTOR3(37.32f, 0.0f, 4.32f));
	node->setID(m_vNode.size());
	m_vNode.push_back(node);

	node = new cNode();
	node->Init(D3DXVECTOR3(40.00f, 0.0f, 4.68f));
	node->setID(m_vNode.size());
	m_vNode.push_back(node);

	node = new cNode();
	node->Init(D3DXVECTOR3(42.32f, 0.0f, 5.62f));
	node->setID(m_vNode.size());
	m_vNode.push_back(node);

	node = new cNode();
	node->Init(D3DXVECTOR3(45.32f, 0.0f, 8.72f));
	node->setID(m_vNode.size());
	m_vNode.push_back(node);

	m_vNode[0]->pushEdgeNode(m_vNode[1]);



	for (int i = 1; i < m_vNode.size() - 1; i++)
	{
		m_vNode[i]->pushEdgeNode(m_vNode[i - 1]);
		m_vNode[i]->pushEdgeNode(m_vNode[i + 1]);
	}
	m_vNode[m_vNode.size() - 1]->pushEdgeNode(m_vNode[m_vNode.size() - 2]);
}

HRESULT cScene_Game::Scene_Init()
{
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

	//라이트 푸쉬


	bBox_Save = new cMap_Bbox_Save();
	bBox_Save->Scene_Init();

	bBox_Load = new cMap_Bbox_Load();
	bBox_Load->Scene_Init();

	door = new cDoorPosition();
	door->Scene_Init();

	mapAcive1 = new cMap_Active;
	mapAcive1->Scene_Init();

	mapAcive2 = new cMap_Active;
	mapAcive2->Scene_Init();

	//cLight_Direction* pLight2 = new cLight_Direction;

	//pLight2->Color = D3DXCOLOR(0.1, 0.1, 0.1, 0);
	//pLight2->pTransform->SetWorldPosition(0, 1, 0);
	////pLight2->Intensity = 10.0f;
	//

	cLight_Point* pLight3 = new cLight_Point();
	pLight3->Color = D3DXCOLOR(0.3, 0.3, 0.3, 0);
	pLight3->minRange = 1.0f;
	pLight3->maxRange = 5.0f;
	pLight3->distancePow = 10.0f;

	pLight3->Intensity = 1.f;
	pLight3->pTransform = pMainCamera;
	NodeInit();
	//this->lights.push_back(pLight2);
	this->lights.push_back(pLight3);


	Pig = new PigEnemy("../Resources/Meshes/Pig/20161129_Pig.X", { 8,0,8 }, this);
	Pig->setObj(house);
	Pig->CameraAttachToUnit(vCamera[ENEMY]);

	Miles = new Player("../Resources/OLGame/Meshes/Miles/miles.X", { 45,0,8 });
	Miles->CameraAttachToUnit(vCamera[PLAYER]);

	/* 크로스헤어 */
	this->lockCrosshairTex = RESOURCE_TEXTURE->LoadResource("../Resources/OLGame/UI/Crosshair/lock.png");
	this->unlockCrosshairTex = RESOURCE_TEXTURE->LoadResource("../Resources/OLGame/UI/Crosshair/unlock.png");

	//* 아이템 추가
	this->keyItem = new cGameItem();
	this->fileItem = new cGameItem();

	//* 아이템도 이제 곧...
	this->keyItem->Init(
		"key",
		"../Resources/OLGame/Meshes/Item/Key/Key.X",
		D3DXVECTOR3(0.03f, 0.03f, 0.03f), //크기
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), //회전
		D3DXVECTOR3(1.f, 1.8f, 0.2f) //위치
		);

	this->fileItem->Init(
		"file",
		"../Resources/OLGame/Meshes/Item/File/File.X",
D3DXVECTOR3(0.03f, 0.03f, 0.03f),
D3DXVECTOR3(0.0f, 0.0f, 0.0f),
D3DXVECTOR3(2.f, 0.f, 2.f)
);

this->vecGameItem.push_back(this->keyItem);
this->vecGameItem.push_back(this->fileItem);

Pig->pushEnemyUnitVector(Miles);
Miles->pushEnemyUnitVector(Pig);


mapAcive1->SetLink_BoxLoad(bBox_Load);
mapAcive1->SetLink_Player(Miles);

mapAcive2->SetLink_BoxLoad(bBox_Load);
mapAcive2->SetLink_Player(Pig);


bBox_Load->SetLink_MapActive(mapAcive1);
bBox_Load->SetLink_MapActive(mapAcive2);
return S_OK;
}

void cScene_Game::Scene_RenderSprite()
{
	//*point
	RECT rc = { 0, 0, 70, 70 };

	if (bHit == true)
		SPRITE_MGR->DrawTexture(this->lockCrosshairTex, &rc, WINSIZE_X / 2, WINSIZE_Y / 2, 0xffffffff, &D3DXVECTOR3(0, 0, 0));
	else
		SPRITE_MGR->DrawTexture(this->unlockCrosshairTex, &rc, WINSIZE_X / 2, WINSIZE_Y / 2, 0xffffffff, &D3DXVECTOR3(0, 0, 0));

}


void cScene_Game::Scene_Release()
{

	bBox_Save->Scene_Release();
	SAFE_DELETE(bBox_Save);
	bBox_Load->Scene_Release();
	SAFE_DELETE(bBox_Load);

	door->Scene_Release();
	SAFE_DELETE(door);
	mapAcive1->Release();
	SAFE_DELETE(mapAcive1);
	mapAcive2->Release();
	SAFE_DELETE(mapAcive2);

	SAFE_DELETE(this->house);
	for (int i = 0; i < this->lights.size(); i++)
		SAFE_DELETE(this->lights[i]);

	for (int i = 0; i < this->vecGameItem.size(); i++)
		SAFE_DELETE(this->vecGameItem[i]);

}

void cScene_Game::Scene_Update(float timeDelta)
{

	//* 마우스 사라지게
	ShowCursor(false);

	this->house->Update(timeDelta);
	this->Miles->Update(timeDelta);
	this->Pig->Update(timeDelta);

	//bBox_Save->Scene_Update(timeDelta);
	//bBox_Save->ChoiseBoundBox(this->pMainCamera);

//* 픽킹
	POINT ptMouse = GetMousePos();
	D3DXVECTOR2 screenPos(ptMouse.x, ptMouse.y);

	this->pMainCamera->ComputeRay(
		&this->camRay,
		&screenPos);

	for (int i = 0; i < this->vecGameItem.size(); i++)
	{
		this->bHit = PHYSICS_MGR->IsRayHitStaticMeshObject(
			&this->camRay,
			this->vecGameItem[i],
			&this->hitPos,
			&this->hitNormal);
		if (bHit == true)
			break;
	}

	//D3DXVECTOR2 EnemyscreenPos(WINSIZE_X / 2, WINSIZE_Y/2);

	//this->vCamera[ENEMY]->ComputeRay(
	//	&this->EnemycamRay,
	//	&EnemyscreenPos);

	//if (PHYSICS_MGR->IsRayHitBound(&this->EnemycamRay, Miles->getCollisionBox(), Miles->getTrans(), NULL, NULL))
	//{
	//	Pig->setRayCollision(false);
	//}
	//else
	//	Pig->setRayCollision(true);

	//this->vCamera[ENEMY]->SetWorldPosition(Pig->getHeadPos());
	//플레이어 카메라는 계속 업데이트 해줘야 한다.
	this->vCamera[PLAYER]->SetWorldPosition(Miles->getHeadCamPos());

	lights[0]->pTransform = vCamera[PLAYER];

	//this->Pig->Update(timeDelta);
	//this->Pig->LerpMoveControll(timeDelta, NodeList->GetCurNode(), NodeList->NextNode());

	//if (!Pig->GetState()->GetisActive())
	//{
	//	if(Pig->getCurAniName() == "RUN" )
	//	Pig->StatePlayChange("WALK", 0.5f);
	//	else if (Pig->getCurAniName() == "WALK")
	//		Pig->StatePlayChange("RUN", 0.4f);

	//	NodeList->setCurNodeNext();
	//	Pig->GetState()->setActive(true);
	//}	

	bBox_Load->Scene_Update(timeDelta);
	door->Scene_Update(timeDelta);

	mapAcive1->Collision_Inspection(Miles);
	mapAcive1->BoxActive(Miles);

	mapAcive2->Collision_Inspection(Pig);
	mapAcive2->BoxActive(Pig);
	mapAcive2->BoxRayActive(Pig);

}

void cScene_Game::Scene_Render1()
{
	char szTemp[128];

	sprintf_s(szTemp, "%.2f", Miles->getTrans()->GetLocalPosition().x);
	DXFONT_MGR->PrintTextOutline(szTemp, WINSIZE_X - 200, 100, 0xffffff00, 0xff000000);

	sprintf_s(szTemp, " %.2f", Miles->getTrans()->GetLocalPosition().z);
	DXFONT_MGR->PrintTextOutline(szTemp, WINSIZE_X - 130, 100, 0xffffff00, 0xff000000);

	if (Pig->getRAy())
	{
		sprintf_s(szTemp, "O,");
		DXFONT_MGR->PrintTextOutline(szTemp, WINSIZE_X - 155, 150, 0xffffff00, 0xff000000);
	}
	else
	{
		sprintf_s(szTemp, "X,");
		DXFONT_MGR->PrintTextOutline(szTemp, WINSIZE_X - 155, 150, 0xffffff00, 0xff000000);
	}

	if (bHit == true)
	{
	//	LOG_MGR->AddLog("hit!");
	}


	//적용되는 LightMatrix
	D3DXMATRIXA16 matLights[10];
	for (int i = 0; i < 1; i++)
	{
		matLights[i] = this->lights[i]->GetLightMatrix();
		//	this->lights[i]->RenderGizmo();
	}

	//셰이더에 라이팅 셋팅
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

	//* 아이템
	int size = this->vecGameItem.size();
	for (int i = 0; i < size; i++)
	{
		vecGameItem[i]->Render();
	}
	//	this->air->Render();
	bBox_Load->Scene_Render1();
	door->Scene_Render1();
}
