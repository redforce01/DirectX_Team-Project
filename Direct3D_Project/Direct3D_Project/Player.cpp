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
#include "cEffect.h"


Player::Player(std::string filePath, D3DXVECTOR3 pos)
{	
	m_Hp = 3;
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

	//* 머리 Transform 
	this->pHeadTrans = new cTransform();
	this->pSkinnedAni->AddBoneTransform("Hero-Head", pHeadTrans);

	this->m_ItemPocketTrans = new cTransform();
	this->pSkinnedAni->AddBoneTransform("Hero-R-Clavicle", m_ItemPocketTrans);

	this->m_ItemPocketBound = new cBoundBox();
	this->m_ItemPocketBound->SetBound(&m_ItemPocketTrans->GetWorldPosition(), &D3DXVECTOR3(1, 1, 1));





	//*Player 가 가지고 있는 아이템 생성 및 추가 ////////////////////////////////////////////////////////////////////////////
	this->head = new cAccessory();
	this->handycam = new cAccessory();
	this->head->Init("head", "../Resources/OLGame/Meshes/Item/Head/miles_head.X",
		D3DXVECTOR3(0.2f, 0.2f, 0.2f)
		);
	this->handycam->Init("handycam", "../Resources/OLGame/Meshes/Item/Handycam/handycam.X",
		D3DXVECTOR3(0.2f, 0.2f, 0.2f),
		D3DXVECTOR3(0.0f, 1.5f, 0.0f),
		D3DXVECTOR3(0.0f, -0.2f, -0.8f)
		);

	m_bCamUp = false;
	isHoldingCam = false;

	this->input = new InputHandler;
 
	InitAnimation();
	InitLight();

	m_CollisionSphere = new cBoundSphere;
	m_CollisionSphere->SetBound(&pSkinnedTrans->GetLocalPosition(), &D3DXVECTOR3(1, 2, 1));

	m_DetectSphere = new cBoundSphere;
	m_DetectSphere->SetBound(&pSkinnedTrans->GetLocalPosition(), &D3DXVECTOR3(2, 2, 2));

	m_CollisionBox = new cBoundBox;
//	m_CollisionBox->SetBound(&pSkinnedTrans->GetWorldPosition(), &D3DXVECTOR3(0.7f,4.f, 0.7f));
}

void Player::Update(float timeDelta)
{
	pSkinnedAni->Update(timeDelta);


	m_CollisionBox->SetBound(&D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0.7f, 4.f, 0.7f));

		
	this->m_headCamPos = this->m_pHeadCamTrans->GetWorldPosition();
	this->m_rightHandPos = this->m_pRightHandTrans->GetWorldPosition();
	this->m_headPos = this->pHeadTrans->GetWorldPosition();
	
	PlayerControll(timeDelta);



}

void Player::Release()
{

	SAFE_DELETE(this->m_CollisionBox);
	SAFE_DELETE(this->m_CollisionSphere);
	SAFE_DELETE(this->m_DetectSphere);

	SAFE_DELETE(this->pSkinnedTrans);
	this->pSkinnedAni->Release();
	SAFE_DELETE(this->pSkinnedAni);
}

void Player::PlayerControll(float timeDelta)
{
	if (!isGameEvent)
	{
		Command* command = input->HandleInput();
		command->execute(*this, timeDelta);



	}
}

void Player::InitAnimation()
{
	typedef struct animationClip
	{
		float speed;
		std::string name;
	} AnimationClip;

	animationClip animationList[36] = {
		{ 0, "STAND" },
		{ 0, "CROUCH_IDLE" },
		{ 5, "CROUCH_FORWARD" },
		{ 5, "CROUCH_BACKWARD" },
		{ 5, "CROUCH_RIGHT" },
		{ 5, "CROUCH_LEFT" },
		{ 5, "WALK_FORWARD" },
		{ 5, "WALK_BACKWARD" },
		{ 5, "RUN_FORWARD" },
		{ 5, "RUN_BACKWARD" },
		{ 5, "RUN_RIGHT" },
		{ 5, "RUN_LEFT" },
		{ 0, "STAND_CAM_IDLE" },
		{ 5, "STAND_CAM_RAISE" },
		{ 5, "STAND_CAM_LOWER1" },
		{ 5, "STAND_CAM_LOWER2" },
		{ 0, "CROUCH_CAM_IDLE" },
		{ 5, "CROUCH_CAM_RAISE" },
		{ 5, "CROUCH_CAM_LOWER" },
		{ 5, "STAND_TO_CROUCH" },
		{ 5, "CROUCH_TO_STAND" },
		{ 5, "WALK_LEFT" },
		{ 5, "WALK_RIGHT" },
		{ 0, "GRAB_HIGH" },
		{ 0, "GRAB" },
		{ 0, "GRAB_AND_THROW" }

	};

	for (int i = 0; i < sizeof(animationList) / sizeof(*animationList); i++)
	{
		Action* temp;
		temp = new PlayerAction(animationList[i].speed, 0.02, this, animationList[i].name);
		m_MState.insert(make_pair(animationList[i].name, temp));
	}

	StatePlayChange("STAND", 0.3f);
}



void Player::InitLight()
{
	//라이트 푸쉬
	cLight_Direction* pLight1 = new cLight_Direction();
	pLight1->Color = D3DXCOLOR(0.2, 0.2, 0.2, 1);
	pLight1->Intensity = 0.1f;

	//cLight_Point* pLight2 = new cLight_Point();
	//pLight2->Color = D3DXCOLOR(1, 1, 1, 0);
	//pLight2->minRange = 5.0f;
	//pLight2->maxRange = 99.0f;
	//pLight2->distancePow = 50.0f;

	this->lights.push_back(pLight1);
//	this->lights.push_back(pLight2);

}

void Player::Render()
{
	//pSkinnedAni->SetUpBoneTrans(pSkinnedTrans);
	Unit::Render();

	D3DXMATRIXA16 mat16;
   

	//* 이렇게 머리 카메라 위치도 계속 업데이트 해주고
	this->pHeadTrans->SetWorldPosition(this->m_headCamPos);
	this->head->SetWorldPosition(this->m_headPos);
	this->handycam->SetWorldPosition(this->m_rightHandPos);

	this->head->SetRotateWorld(this->pSkinnedTrans->GetWorldRotateMatrix());
	//this->head->Render();

	this->handycam->SetRotateWorld(this->pSkinnedTrans->GetWorldRotateMatrix());
	this->handycam->Render();

}

void Player::AttachToItem(cTransform* item)
{
	pSkinnedTrans->AttachTo(item);
}

void Player::AttachItemToUnit(cTransform* item)
{
	this->pSkinnedTrans->AddChild(item);
}

void Player::setDamage(int Damage)
{
	m_Hp -= Damage;
	m_blood->SetisHitPly(true);

}

