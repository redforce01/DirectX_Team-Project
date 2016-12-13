#include "stdafx.h"
#include "Unit.h"
#include "cTransform.h"
#include "cXMesh_Skinned.h"
#include "cSkinnedAnimation.h"
#include "cLight.h"
#include "cLight_Direction.h"
#include "cLight_Point.h"
#include "Action.h"
#include "cCamera.h"
#include "cNode.h"
#include"cBoundBox.h"
#include "cBoundSphere.h"
#include "Command.h"

Player::Player(std::string filePath, D3DXVECTOR3 pos)
{	
	m_FilePath = filePath;
	D3DXMATRIXA16 matCorrection;
	D3DXMatrixScaling(&matCorrection, 0.04f, 0.04f, 0.04f);

	cXMesh_Skinned* pSkinned1 = RESOURCE_SKINNEDXMESH->
		GetResource(filePath,
			&matCorrection);

	//위에서 로딩괸 SkinnedMesh 인스턴스를 만든다.
	this->pSkinnedAni = new cSkinnedAnimation();
	this->pSkinnedAni->Init(pSkinned1);

	this->pSkinnedTrans = new cTransform();
	pSkinnedTrans->SetLocalPosition(pos);

	//대가리 카메라 Transform 동적할당
	this->m_pHeadCamTrans = new cTransform();
	this->pSkinnedAni->AddBoneTransform("Hero-Camera", m_pHeadCamTrans);

	//*오른손 Transform 동적할당
	this->m_pRightHandTrans = new cTransform();
	this->pSkinnedAni->AddBoneTransform("Hero-R-Finger2", m_pRightHandTrans);

	//* 머리 Transform 동적할당
	this->pHeadTrans = new cTransform();
	this->pSkinnedAni->AddBoneTransform("Hero-Head", pHeadTrans);

	//*Player 가 가지고 있는 아이템 생성 및 추가 ////////////////////////////////////////////////////////////////////////////
	this->head = new cAccessory();
	this->handycam = new cAccessory();

	this->vecItem.push_back(this->head); //0번 머리
	this->vecItem.push_back(this->handycam); //1번 핸디캠
	this->vecItem[0]->Init("head", "../Resources/OLGame/Meshes/Item/Head/miles_head.X", D3DXVECTOR3(0.2f, 0.2f, 0.2f));
	this->vecItem[1]->Init("handycam", "../Resources/OLGame/Meshes/Item/Handycam/handycam.X",
		D3DXVECTOR3(0.2f, 0.2f, 0.2f),
		D3DXVECTOR3(0.0f, 1.5f, 0.0f),
		D3DXVECTOR3(0.0f, -0.2f, -0.8f)
		);

	this->input = new InputHandler;
 
	InitAnimation();
	InitLight();

	m_CollisionSphere = new cBoundSphere;
	m_CollisionSphere->SetBound(&pSkinnedTrans->GetLocalPosition(), &D3DXVECTOR3(1, 2, 1));

	m_DetectSphere = new cBoundSphere;
	m_DetectSphere->SetBound(&pSkinnedTrans->GetLocalPosition(), &D3DXVECTOR3(2, 2, 2));

	m_CollisionBox = new cBoundBox;
	m_CollisionBox->SetBound(&pSkinnedTrans->GetWorldPosition(), &D3DXVECTOR3(0.4f, 3.0f, 0.4f));
}

void Player::Update(float timeDelta)
{
	pSkinnedAni->Update(timeDelta);
	//this->pSkinned->AddBoneTransform("Hero-Camera", pHeadTrans);
	//* 카메라, 핸디캠, 머리 붙일 위치 얻어옴
	this->m_headCamPos = this->m_pHeadCamTrans->GetWorldPosition();
	this->m_rightHandPos = this->m_pRightHandTrans->GetWorldPosition();
	this->m_headPos = this->pHeadTrans->GetWorldPosition();

	PlayerControll(timeDelta);
}

void Player::Release()
{
	SAFE_DELETE(this->pSkinnedTrans);
	this->pSkinnedAni->Release();
	SAFE_DELETE(this->pSkinnedAni);
}

void Player::PlayerControll(float timeDelta)
{
    Command* command = input->HandleInput();
	command->execute(*this, timeDelta);
}

void Player::InitAnimation()
{
	typedef struct animationClip
	{
		float speed;
		std::string name;
	} AnimationClip;

	animationClip animationList[33] = {
		{ 0, "STAND" },
		{ 0, "CROUCH_IDLE" },
		{ 5, "CROUCH_FORWARD" },
		{ 5, "CROUCH_BACKWARD" },
		{ 5, "CROUCH_RIGHT" },
		{ 5, "CROUCH_LEFT" },
		{ 5, "WALK_FORWARD" },
		{ 5, "WALK_BACKWARD" },
		{ 8, "RUN_FORWARD" },
		{ 8, "RUN_BACKWARD" },
		{ 8, "RUN_RIGHT" },
		{ 8, "RUN_LEFT" },
		{ 5, "JUMP_ON_SPOT" },
		{ 5, "JUMP_FROM_WALK" },
		{ 5, "JUMP_FROM_RUN" },
		{ 0, "JUMPOVER_FROM_WALK" },
		{ 5, "JUMPOVER_FROM_RUN" },
		{ 5, "STAND_CAM_IDLE" },
		{ 5, "STAND_CAM_RAISE" },
		{ 5, "STAND_CAM_LOWER" },
		{ 5, "CROUCH_CAM_IDLE" },
		{ 5, "CROUCH_CAM_RAISE" },
		{ 5, "CROUCH_CAM_LOWER" },
		{ 5, "STAND_TO_CROUCH" },
		{ 5, "CROUCH_TO_STAND" },
		{ 5, "BED" },
		{ 5, "DOOR_LOCKED" },
		{ 5, "DOOR_INSIDE_OPEN" },
		{ 5, "DOOR_INSIDE_CLOSE" },
		{ 5, "DOOR_OUTSIDE_OPEN" },
		{ 5, "DOOR_OUTSIDE_CLOSE" },
		{ 5, "WALK_RIGHT" },
		{ 5, "WALK_LEFT" }
	};

	for (int i = 0; i < sizeof(animationList) / sizeof(*animationList); i++)
	{
		Action* temp;
		temp = new PlayerAction(animationList[i].speed, 0.02, this, animationList[i].name);
		m_MState.insert(make_pair(animationList[i].name, temp));
	}

	StatePlayChange("STAND", 0.3f);
}

void Player::SitDown()
{
	if (!pSkinnedAni->isPlay() && m_Animation_Name == "STAND_TO_CROUCH")
		StatePlayChange("CROUCH_IDLE", 0.3f);
	else
	{
		StatePlayChange("STAND_TO_CROUCH", 0.3f);
	}
}


void Player::InitLight()
{
	//라이트 푸쉬
	cLight_Direction* pLight1 = new cLight_Direction();
	pLight1->Color = D3DXCOLOR(1, 1, 1, 1);
	pLight1->Intensity = 0.1f;

	cLight_Point* pLight2 = new cLight_Point();
	pLight2->Color = D3DXCOLOR(1, 1, 1, 0);
	pLight2->minRange = 5.0f;
	pLight2->maxRange = 99.0f;
	pLight2->distancePow = 50.0f;

	this->lights.push_back(pLight1);
	this->lights.push_back(pLight2);

}

void Player::Render()
{
	Unit::Render();
	this->vecItem[0]->SetWorldPosition(this->m_headPos);
	this->vecItem[1]->SetWorldPosition(this->m_rightHandPos);

	//* 핸디캠, 머리 회전 값 적용!
	int size = this->vecItem.size();

	for (int i = 0; i < size; i++)
	{
		vecItem[i]->SetRotateWorld(this->pSkinnedTrans->GetWorldRotateMatrix());
		vecItem[i]->Render();
	}

}

void Player::AttachToItem(cTransform* item)
{
	pSkinnedTrans->AttachTo(item);
}

void Player::AttachItemToUnit(cTransform* item)
{
	this->pSkinnedTrans->AddChild(item);
}

