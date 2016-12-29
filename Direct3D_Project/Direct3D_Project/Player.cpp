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
#include "cCameraUI.h"


Player::Player(std::string filePath, D3DXVECTOR3 pos)
{
	m_Hp = 3;
	m_FilePath = filePath;
	D3DXMATRIXA16 matCorrection;
	D3DXMatrixScaling(&matCorrection, 0.04f, 0.04f, 0.04f);

	cXMesh_Skinned* pSkinned1 = RESOURCE_SKINNEDXMESH->
		GetResource(filePath,
			&matCorrection);

	m_isDead = false;
	m_isCanControll = true;
	m_isCanCamControll = true;


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

	bCamUp = false;
	isHoldingCam = false;

	this->input = new InputHandler;

	InitAnimation();
	InitLight();

	m_CollisionSphere = new cBoundSphere;
	m_CollisionSphere->SetBound(&pSkinnedTrans->GetLocalPosition(), &D3DXVECTOR3(1, 2, 1));

	m_DetectSphere = new cBoundSphere;
	m_DetectSphere->SetBound(&pSkinnedTrans->GetLocalPosition(), &D3DXVECTOR3(2, 2, 2));

	m_CollisionBox = new cBoundBox;
	//   m_CollisionBox->SetBound(&pSkinnedTrans->GetWorldPosition(), &D3DXVECTOR3(0.7f,4.f, 0.7f));

	m_CurStamina = 100;
	m_MaxStamina = 100;

	camUpDelayCount = 0;
}

void Player::Update(float timeDelta)
{
	pSkinnedAni->Update(timeDelta);
	m_CollisionBox->SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0.7f, 5.f, 0.7f));

	this->m_headCamPos = this->m_pHeadCamTrans->GetWorldPosition();
	this->m_rightHandPos = this->m_pRightHandTrans->GetWorldPosition();
	this->m_headPos = this->pHeadTrans->GetWorldPosition();

	PlayerControll(timeDelta);
	StaminaSound();

	if (m_Hp == 1 && (m_Animation_Name == "STAND" || m_Animation_Name == "CROUCH"))
	{
		SOUNDDATA->playSound(SOUND_TYPE_PLAYER_VOICE, SOUND_PLAY_TYPE_PLAYER_BREATH_ROUGH, 0);
	}

	if (this->isHoldingCam == true)
	{
		camUpDelayCount++;
		if (camUpDelayCount > 30)
		{
			this->isHoldingCam = false;
			camUpDelayCount = 0;
		}
	}
}

void Player::Release()
{
	SAFE_DELETE(m_ItemPocketBound);
	SAFE_DELETE(m_ItemPocketTrans);
	SAFE_DELETE(this->m_CollisionBox);
	SAFE_DELETE(this->m_CollisionSphere);
	SAFE_DELETE(this->m_DetectSphere);

	SAFE_DELETE(this->pSkinnedTrans);
	this->pSkinnedAni->Release();
	SAFE_DELETE(this->pSkinnedAni);

	SAFE_DELETE(this->pHeadTrans);
	SAFE_DELETE(this->m_Action);
	SAFE_DELETE(this->m_blood);
	SAFE_DELETE(this->m_Eye);
	SAFE_DELETE(this->m_pHeadCamTrans);
	SAFE_DELETE(this->pHeadTrans);
	m_MState.clear();

}

void Player::EventCollisionEvent()
{
	SOUNDDATA->playSound(SOUND_TYPE_PLAYER_VOICE, SOUND_PLAY_TYPE_PLAYER_SCRAG, 7);
	//
	m_isCanControll = false;
	m_isCanCamControll = false;
	pSkinnedTrans->SetWorldPosition(m_vEnemy[0]->getRightHandPos().x - 0.5, 0.8, m_vEnemy[0]->getRightHandPos().z + 0.5);
	pSkinnedTrans->LookDirection(-m_vEnemy[0]->getTrans()->GetWorldPosition()); //마일즈는 돼지 보고
	pSkinnedTrans->GetChild()->LookPosition(
		D3DXVECTOR3(
			m_vEnemy[0]->getTrans()->GetWorldPosition().x,
			3.63,
			m_vEnemy[0]->getTrans()->GetWorldPosition().z), D3DXVECTOR3(0, 1, 0)); //마일즈는 돼지 보고
	this->m_headCamPos = this->m_pHeadCamTrans->GetWorldPosition();
	StateOneShotHoldChange("GRAB_AND_THROW", 0.5f); // 잡히는 애니메이션

}

void Player::PlayerControll(float timeDelta)
{
	if (!m_isDead && !m_curScene->isEnding)
	{
		if (m_isCanControll)
		{
			Command* command = input->HandleInput();
			command->execute(*this, timeDelta);

			m_curScene->ControllNightVision();
			CamUP_DOWNControll();

		}
		if (m_isCanCamControll)
			pSkinnedTrans->GetChild()->DefaultControl4(this, 5, timeDelta);
	}

	pSkinnedTrans->GetChild()->SetWorldPosition(this->getHeadCamPos());
}

void Player::InitAnimation()
{
	typedef struct animationClip
	{
		float speed;
		std::string name;
	} AnimationClip;

	animationClip animationList[37] = {
		{ 0, "STAND" },
		{ 0, "CROUCH_IDLE" },
		{ 1, "CROUCH_FORWARD" },
		{ 1, "CROUCH_BACKWARD" },
		{ 1, "CROUCH_RIGHT" },
		{ 1, "CROUCH_LEFT" },
		{ 2.5f, "WALK_FORWARD" },
		{ 2.5f, "WALK_BACKWARD" },
		{ 5.1f, "RUN_FORWARD" },
		{ 5.1f, "RUN_BACKWARD" },
		{ 5.1f, "RUN_RIGHT" },
		{ 5.1f, "RUN_LEFT" },
		{ 0, "STAND_CAM_IDLE" },
		{ 5, "STAND_CAM_RAISE" },
		{ 5, "STAND_CAM_LOWER1" },
		{ 5, "STAND_CAM_LOWER2" },
		{ 0, "CROUCH_CAM_IDLE" },
		{ 5, "CROUCH_CAM_RAISE" },
		{ 5, "CROUCH_CAM_LOWER" },
		{ 5, "STAND_TO_CROUCH" },
		{ 5, "CROUCH_TO_STAND" },
		{ 2.5f, "WALK_LEFT" },
		{ 2.5f, "WALK_RIGHT" },
		{ 0, "GRAB_HIGH" },
		{ 0, "GRAB" },
		{ 0, "GRAB_AND_THROW" },
		{ 0,"FALL" }

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
	//cLight_Direction* pLight1 = new cLight_Direction();
	//pLight1->Color = D3DXCOLOR(0.2, 0.2, 0.2, 1);
	//pLight1->Intensity = 0.1f;

	//cLight_Point* pLight2 = new cLight_Point();
	//pLight2->Color = D3DXCOLOR(1, 1, 1, 0);
	//pLight2->minRange = 5.0f;
	//pLight2->maxRange = 99.0f;
	//pLight2->distancePow = 50.0f;

	//this->lights.push_back(pLight1);
	//   this->lights.push_back(pLight2);

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
	//   this->head->Render();

	this->handycam->SetRotateWorld(this->pSkinnedTrans->GetWorldRotateMatrix());

	if (this->isHoldingCam == true)
		this->handycam->Render();

}

void Player::CamUP_DOWNControll()
{
	if (KEY_MGR->IsOnceDown(VK_RBUTTON))
	{
		isHoldingCam = true;
		if (bCamUp)
		{
			pSkinnedAni->PlayOneShot("STAND_CAM_LOWER1", 0.3f);
			g_Czoom = -0.025f;
		}
		//* 카메라를 안들고 있을 때
		else
		{
			pSkinnedAni->PlayOneShot("STAND_CAM_RAISE", 0.3f);
			g_Czoom = 0.00f;
		}
		m_CameraUI->TurnOnCam();
		bCamUp = !bCamUp;
	}

	if (bCamUp)
	{
		if (m_CameraUI->GetisRec())
		{
			m_CameraUI->IncreaseDelayCount();
			if (m_CameraUI->GetDelayCount() > 30)
			{
				pMainCamera->fov = m_curScene->GetCamFov() + g_Czoom;
				m_CameraUI->SetIsDelay(true);
			}
		}
	}
	else
	{
		pMainCamera->fov = m_curScene->GetCamFov();
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

void Player::setDamage(int Damage)
{
	if (m_isCanControll)
	{
		m_Hp -= Damage;
		m_blood->SetisHitPly(true);
		SOUNDDATA->playSound(SOUND_TYPE_NPC, SOUND_PLAY_TYPE_NPC_CUTBODY, 0);

		if (m_Hp > 0)
			SOUNDDATA->playSound(SOUND_TYPE_PLAYER_VOICE, SOUND_PLAY_TYPE_PLAYER_ATTACKED, 0);

		if (m_Hp <= 0)
		{
			SOUNDDATA->stopSound(SOUND_TYPE_PLAYER_VOICE, SOUND_PLAY_TYPE_PLAYER_FEAR, 0);
			SOUNDDATA->playSound(SOUND_TYPE_PLAYER_VOICE, SOUND_PLAY_TYPE_PLAYER_DAED, 0);
			m_isDead = true;
			StateOneShotHoldChange("FALL", 0.3f);
			this->pSkinnedTrans->SetWorldPosition(pSkinnedTrans->GetWorldPosition().x, 0.8f, pSkinnedTrans->GetWorldPosition().z);
			m_curScene->DeadEvent();
		}
	}

}


void Player::RunningStamina()
{
	if (m_CurStamina >= 1)
	{
		m_CurStamina += MINUS_STAMINA;
	}
	else
	{
		m_CurStamina = 0;
	}
}
void Player::PlusStamina()
{
	if (m_CurStamina <= MAX_STAMINA)
	{
		m_CurStamina += HEAL_STAMINA;
	}
	else m_CurStamina = MAX_STAMINA;
}

void Player::StaminaSound()
{

	if (m_CurStamina >= 30 && m_CurStamina < 60)
	{
		SOUNDDATA->playSound(SOUND_TYPE_PLAYER_VOICE, SOUND_PLAY_TYPE_PLAYER_RELAX, 0, 0.1f);
	}
	if (m_CurStamina < 30 && m_CurStamina > 0)
	{
		SOUNDDATA->playSound(SOUND_TYPE_PLAYER_VOICE, SOUND_PLAY_TYPE_PLAYER_RELAX, 0);
	}

}
