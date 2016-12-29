#include "stdafx.h"
#include "Unit.h"
#include "cTransform.h"
#include "cXMesh_Skinned.h"
#include "cSkinnedAnimation.h"
#include "cLight.h"
#include "cBaseObject.h"
#include "cLight_Direction.h"
#include "cLight_Point.h"
#include "Action.h"
#include "cCamera.h"
#include "cScene.h"
#include "cNode.h"
#include"cBoundBox.h"
#include "cBoundSphere.h"
#include "cActionSeq.h"
#include "Dijkstra.h"
#include "LeftArm.h"

PigEnemy::PigEnemy(std::string filePath, D3DXVECTOR3 pos, cScene* Scene)
{
	m_isDead = false;
	m_FilePath = filePath;
	D3DXMATRIXA16 matCorrection;
	D3DXMatrixScaling(&matCorrection, 0.02f, 0.02f, 0.02f);

	cXMesh_Skinned* pSkinned = RESOURCE_SKINNEDXMESH->
		GetResource(filePath,
			&matCorrection);

	//������ �ε��� SkinnedMesh �ν��Ͻ��� �����.
	this->pSkinnedAni = new cSkinnedAnimation();
	this->pSkinnedAni->Init(pSkinned);
	m_isActive = false;
	this->pSkinnedTrans = new cTransform();
	pSkinnedTrans->SetLocalPosition(pos);

	InitAnimation();
	InitLight();

	m_DetectedUnit = NULL;



	m_CollisionSphere = new cBoundSphere;
	//m_CollisionSphere->SetBound(&pSkinnedTrans->GetLocalPosition(), &D3DXVECTOR3(25,2,25));

	ShortDetectSphere = new cBoundSphere;
	//ShortDetectSphere->SetBound(&pSkinnedTrans->GetLocalPosition(), &D3DXVECTOR3(2, 2, 2));

	m_DetectSphere = new cBoundSphere;
	//m_DetectSphere->SetBound(&pSkinnedTrans->GetLocalPosition(), &D3DXVECTOR3(11, 4, 11));

	m_CollisionBox = new cBoundBox;
	m_CollisionBox->SetBound(&pSkinnedTrans->GetWorldPosition(), &D3DXVECTOR3(0.5f, 2, 0.5f));
	//m_CollisionBox->SetBound(&pSkinnedTrans->GetWorldPosition(), &D3DXVECTOR3(25, 3, 25));

	//�밡�� Transform �����Ҵ�
	this->pHeadTrans = new cTransform();
	this->pSkinnedAni->AddBoneTransform("NPCLarge-Head", pHeadTrans);

	this->pHeadTrans2 = new cTransform();
	this->pSkinnedAni->AddBoneTransform("NPCLarge-Head", pHeadTrans2);

	this->m_LeftHandTrans = new cTransform();
	this->pSkinnedAni->AddBoneTransform("NPCLarge-L-Hand", m_LeftHandTrans);

	//*������ Transform �����Ҵ�
	this->m_pRightHandTrans = new cTransform();
	this->pSkinnedAni->AddBoneTransform("NPCLarge-R-Hand", m_pRightHandTrans);

	m_LArm = new LeftArm(this);
	//m_LArm->AddToBody(pSkinnedTrans);

	m_Action = new cActionSeq(this);
	m_curScene = Scene;

	m_Dijk = new Dijkstra;
	m_Dijk->Init(m_curScene);

	m_isFind = false;
	isPatroll = false;
	m_isChange = true;

	//���ͽ�Ʈ�� ��� ã�� �����ϱ�.
	for (int i = 0; i < m_curScene->getVNode().size(); i++)
	{
		m_Dijk->setStart(m_curScene->getVNode()[i]); // ����� ����
		m_Dijk->initDijk(); // ���ͽ�Ʈ�� ���̺� �ʱ�ȭ

		for (int j = 0; j < m_curScene->getVNode().size(); j++)
		{
			vector<cTransform*> cTran;
			m_Dijk->setEnd(m_curScene->getVNode()[j]); // ������ ����
			cTran = m_Dijk->FindWay(); // ��ã�´�
			m_mvWay.insert(make_pair(make_pair(i, j), cTran)); // ã���� map�� �ִ´�.
		}
	}

	walkTime = 0;
	cObject_Sound* temp = new cObject_Sound;
	temp->Init(SOUNDDATA->getSoundKey(SOUND_TYPE_NPC, SOUND_PLAY_TYPE_NPC_WALKING_HEAVY, 1), 0, false, false, D3DXVECTOR3(0, 0, 0));
	m_vObjSound.push_back(temp);

	temp = new cObject_Sound;
	temp->Init(SOUNDDATA->getSoundKey(SOUND_TYPE_NPC, SOUND_PLAY_TYPE_NPC_WALKING_HEAVY, 1), 0, false, false, D3DXVECTOR3(0, 0, 0));
	m_vObjSound.push_back(temp);

	 temp = new cObject_Sound;
	temp->Init(SOUNDDATA->getSoundKey(SOUND_TYPE_NPC, SOUND_PLAY_TYPE_NPC_CHAIN, 0),0, false, false, D3DXVECTOR3(0, 0, 0));
	m_vObjSound.push_back(temp);

};



void PigEnemy::Update(float timeDelta)
{
	this->pSkinnedAni->Update(timeDelta); // Animation ������Ʈ
	m_headPos = pHeadTrans->GetWorldPosition(); // ���� �밥�� ��ġ ����.
	m_rightHandPos = m_pRightHandTrans->GetWorldPosition();
	if ( m_isActive)
	{
	
		m_CollisionBox->SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0.6f, 2.5f, 0.6f));
		m_DetectSphere->SetBound(&(pSkinnedTrans->GetLocalPosition()), &D3DXVECTOR3(4.f, 4, 4.f)); 	//������ ������Ʈ

		m_isFind = this->CollisionEvent(timeDelta); // ���� �÷��̾� �����ߴ�?
		m_LArm->CollisionUnit();
		m_LArm->setEnemy(m_vEnemy[0]);
		m_LArm->setTrans(m_LeftHandTrans);
		m_LArm->Update(timeDelta);

		if (m_isFind)
		{
			m_Action->ClearVector();// ���� ã�� ��� m_DetectedUnit�� �̵�
			AlongPlayerMove(m_DetectedUnit->getTrans());
		}
		else if (!m_isFind) // ����  �� ã�� ��� �ƹ� ��� �ϳ� ��� �װ����� �̵�.
			AlongPlayerMove(&m_curScene->getVNode()[MyUtil::RandomIntRange(0, 50)]->getTrans());

		m_Action->Update(timeDelta);
		m_Action->setCurActionSpeed((*m_MState.find(m_Animation_Name)).second->getMoveSpeed());

		for (int i = 0; i < m_vObjSound.size(); i++)
		{
			m_vObjSound[i]->Update(timeDelta, m_vEnemy[0]->getTrans()->GetWorldPosition());
			m_vObjSound[i]->SetPosition(pSkinnedTrans->GetWorldPosition());
		}

	}
	//����� �ȵ�. �� �κ�.
}

void PigEnemy::Release()
{
	SAFE_DELETE(this->m_CollisionBox);
	SAFE_DELETE(this->m_CollisionSphere);
	SAFE_DELETE(this->m_DetectSphere);

	SAFE_DELETE(this->pSkinnedTrans);
	this->pSkinnedAni->Release();
	SAFE_DELETE(this->pSkinnedAni);
	SAFE_DELETE(this->pHeadTrans);
	SAFE_DELETE(this->m_Action);

}

bool PigEnemy::CollisionEvent(float timeDelta)
{
	if (!m_vEnemy[0]->IsDead())
	{
		// �������� �浹���� 
		bool isDetected;
		// �浹���� �浹����
		bool isCollisioned = SpCollisionCheck(this, m_vEnemy[0]);

		//if (SpDetectionCheck(this, m_vEnemy[0]))
		//	isDetected = true;
		//else isDetected = false;

		if ( (RayCollisionCheck() || DoorRayCollisionCheck())) 
		{
			isDetected = false;
		}
		else {
			isDetected = true;
		}

		if (SpDetectionCheck(this, m_vEnemy[0]))
			isDetected = true;

		if (!isAttacking && pSkinnedAni->getisAniEnd())
		{
			// �ƹ��͵� ������ �ȵ� ��� -> �׳� ��������~
			if (!isDetected && !isCollisioned)
				NULLCollisionEvent();
			// ������ ������ �� �浹 ���� -> �÷��̾ ����!.
			else if (isDetected && !isCollisioned)
				DetectCollisionEvent();
			//�� �浹 �Ͼ ��� -> ��������!
			else if (isCollisioned && isDetected)
			{
				if (!m_isEventMod)
					BodyCollisionEvent();
				else EventCollisionEvent();
			}
		}

		return isDetected;
	}

	return false;

}

bool PigEnemy::RayCollisionCheck()
{
	ray.direction = m_vEnemy[0]->getTrans()->GetWorldPosition() - pSkinnedTrans->GetWorldPosition();
	ray.origin = pSkinnedTrans->GetWorldPosition();

	ray.origin.y += 3.f;
	ray.direction.y += 3.f;

	if (!m_vBox.empty()) {
		for (int i = 0; i < m_vBox.size(); i++)
		{
			if (PHYSICS_MGR->IsRayHitBound(&ray, &m_vBox[i]->BoundBox, m_vBox[i]->pTransform, NULL, NULL))
			{
				return true;
			}
		}
		return false;
	}
	else  {
		return false;
	}
}

bool PigEnemy::DoorRayCollisionCheck()
{
	ray.direction = m_vEnemy[0]->getTrans()->GetWorldPosition() - pSkinnedTrans->GetWorldPosition();
	ray.origin = pSkinnedTrans->GetWorldPosition();

	ray.origin.y += 3.f;
	ray.direction.y += 3.f;

	if (!m_vDoor.empty()) {
		for (int i = 0; i < m_vDoor.size(); i++)
		{
			if (PHYSICS_MGR->IsRayHitBound(&ray, &m_vDoor[i]->BoundBox, m_vDoor[i]->pTransform, NULL, NULL))
			{
				return true;
			}
		}
		return false;
	}
	else {
		return false;
	}
}

void PigEnemy::NULLCollisionEvent()
{
	m_DetectedUnit = NULL;
	StatePlayChange("WALK", 0.3f);

	PlayWalkSound();
	m_LArm->setActive(false);
	// ���� �÷��̾� �Ѵٰ� -> �÷��̾ ���ļ� ���� ����, �÷��̾� �Ѵ� ��θ� Clear ���־�, ���� ��θ� ����� �ְ� �Ѵ�.
	if (m_isChange) {
		m_Action->ClearVector();
		m_isChange = false;
	}
	m_isAttacking = false;
}

void PigEnemy::DetectCollisionEvent()
{
	m_DetectedUnit = m_vEnemy[0];  // ������ ������ ����Ƽ�忡 �־��ְ�
	this->StatePlayChange("RUN", 0.3f);  //����� �޸���!
	PlayRunSound();
	m_LArm->setActive(false);
	// ���� �������ٰ� -> �÷��̾ ������, ���� ��θ� Clear�ϰ� ���ο� ��θ� ����� �ְ� ��.
	if (!m_isChange)
	{
		m_Action->ClearVector();
		m_isChange = true;
	}
	m_isAttacking = false;
}

void PigEnemy::BodyCollisionEvent()
{
	SOUNDDATA->playSound(SOUND_TYPE_NPC, SOUND_PLAY_TYPE_ATTACK, 0);

	m_LArm->setActive(true);
	m_DetectedUnit = m_vEnemy[0];  // ������ ĳ���� ����ְ�
		this->StateOneShotChange("ATTACK_LEFT", 0.3f); // ���� �ֵθ���!
	m_Action->ClearVector(); // �� Ŭ��������~
	m_isAttacking = true;
}

void PigEnemy::EventCollisionEvent()
{
	isGameEvent = true;
	m_DetectedUnit = m_vEnemy[0];  // ������ ĳ���� ����ְ�
	this->StateOneShotHoldChange("GRAB_FATALITY", 0.0f); // ���� �ֵθ���!
	m_headPos = pHeadTrans->GetWorldPosition();

	pSkinnedTrans->LookDirection(m_DetectedUnit->getTrans()->GetWorldPosition());
	SOUNDDATA->playSound(SOUND_TYPE_NPC, SOUND_PLAY_TYPE_NPC_CHAIN, 0);
	m_Action->ClearVector(); // �� Ŭ��������~
	m_isActive = false;
	m_rightHandPos = m_pRightHandTrans->GetWorldPosition();
	m_DetectedUnit->EventCollisionEvent();
	//m_Action->ClearVector(); // �� Ŭ��������~
}


void PigEnemy::InitAnimation()
{

	typedef struct animationClip
	{
		float move_Speed;
		float rotate_Speed;
		std::string name;
	} AnimationClip;

	animationClip animationList[14] = {
		{ 0.0f,  0.1f, "IDLE" },
		{ 0.06f, 0.2f, "WALK" },
		{ 0.13f, 0.2f, "RUN" },
		{ 0.0f, 0.2f,"IDLE_CHASE" },
		{ 0.0f, 0.2f,"IDLE_SEARCH1" },
		{ 0.0f, 0.2f,"IDLE_SEARCH2" },
		{ 0.0f, 0.2f,"IDLE_SEARCH3" },
		{ 0.0f, 0.2f,"ATTACK_LEFT" },
		{ 0.0f, 0.2f,"GRAB_END" },
		{ 0.0f, 0.2f,"GRAB_FATALITY" },
		{ 0.0f,0.2f, "THROW_LEFT" },
		{ 0.0f,0.2f, "THROW_RIGHT" },
		{ 0.0f,0.2f, "THROW_PLAYER" },
		{ 0.0f,0.2f, "GRAB" }
	};

	for (int i = 0; i < sizeof(animationList) / sizeof(*animationList); i++)
	{
		Action* temp;
		temp = new EnemyAction(animationList[i].move_Speed, animationList[i].rotate_Speed, this, animationList[i].name);
		m_MState.insert(make_pair(animationList[i].name, temp));
	}

	StatePlayChange("GRAB", 0.3f);
}

void PigEnemy::InitLight()
{
	//����Ʈ Ǫ��
	//����Ʈ Ǫ��
	cLight_Direction* pLight1 = new cLight_Direction();
	pLight1->Color = D3DXCOLOR(1, 1, 1, 1);
	pLight1->Intensity = 0.01f;

	this->lights.push_back(pLight1);
}

void PigEnemy::Render()
{
	Unit::Render();
	
	//m_LArm->render();

#ifdef _DEBUG
	//char szTemp[128];
	//for (int i = 0; i < m_vObjSound.size(); i++)
	//{		
	//	sprintf_s(szTemp, "obj[%d] Dis : %.2f", i, m_vObjSound[i]->FunctionDistance());
	//	DXFONT_MGR->PrintTextOutline(szTemp, WINSIZE_X - 400, 200 + (i * 60), 0xffffff00, 0xff000000);

	//	sprintf_s(szTemp, "obj[%d] Vol : %.2f", i, m_vObjSound[i]->GetSoundVolume());
	//	DXFONT_MGR->PrintTextOutline(szTemp, WINSIZE_X - 400, 230 + (i * 60), 0xffffff00, 0xff000000);
	//}

#endif // DEBUG
}

void PigEnemy::AlongPlayerMove(cTransform* DestTrans)
{
	// �׼� ���Ͱ� ��� ���� ��� �׼����� ��θ� �������� ��.
	// �׷��⿡ �׼� ���Ͱ� 0�̸� ��������.
	if (m_Action->getActionSize() == 0)
	{
		if (!m_isAttacking) // // ������ �÷��̾� �ε�� �Ӷ��� ���ͽ�Ʈ�� ��� No�ʿ�.
		{
			vector<cTransform*> vecWay; //���� ��ε��� ������ cTransform ���� ����
			m_Dijk->FindToFrom(pSkinnedTrans->GetLocalPosition(), DestTrans->GetLocalPosition()); // ���� ������ ���� ����� Node, �׸��� DestTrans�� ���� ����� Node�� ���� ã�´�!
			vecWay = (*m_mvWay.find(make_pair(m_Dijk->getStart()->getID(), m_Dijk->getEnd()->getID()))).second; // ��带 ã�Ҵٸ� ���ͽ�Ʈ�� Key�� ���� �ְ� ��θ� ã��.	
			//vecWay = vDijk->getNodeWay();
			//	vecWay.insert(vecWay.begin(), pSkinnedTrans );  //���� �ڽ� ��ġ�� ������ ù��°�� �ְ�
			vecWay.push_back(DestTrans);  // ��ǥ ��ġ�� �������� �־�����...

			if (m_isFind) // ���� �÷��̾ ã�� ���
				this->PushActionSeq(vecWay, 1); // �� ã�� ��� VecSize - 1 ��ŭ
			else this->PushActionSeq(vecWay, 2); // �� �� ã�� ��� VecSize - 2 ��ŭ �Ķ���͸� ���� �־�����.

		}

	}

	//vecWay = Dij.CalcWay(this->pSkinnedTrans->GetWorldPosition(), m_DetectedUnit->getTrans()->GetLocalPosition());
}

void PigEnemy::PlayWalkSound()
{
	walkTime++;
	if (walkTime % 32 == 0)
	{
		walkTime = 0;
		if (m_vObjSound[0]->GetIsSoundPlay())
		{
			m_vObjSound[1]->SoundPlay(true);
			m_vObjSound[2]->SoundPlay(true);
		}
		else
		{
			m_vObjSound[0]->SoundPlay(true);
			m_vObjSound[2]->SoundPlay(true);
		}
	}
}

void PigEnemy::PlayRunSound()
{
	walkTime++;
	if (walkTime % 16 == 0)
	{
		walkTime = 0;
		if (m_vObjSound[0]->GetIsSoundPlay())
		{
			m_vObjSound[1]->SoundPlay(true);
			m_vObjSound[2]->SoundPlay(true);
		}
		else
		{
			m_vObjSound[0]->SoundPlay(true);
			m_vObjSound[2]->SoundPlay(true);
		}
	}
}

void PigEnemy::PlayDoorSound()
{
}

// ���� ���� ��θ� Actoin ���Ϳ� �־ �� ���͵��� �����̵��� �Ѵ�!
void PigEnemy::PushActionSeq(vector<cTransform*> vWay, int MinusNum)
{
	// ��� ��ŭ �׼��� �־�����!  �� �׼��̶�? -> ���� �� ��忡�� �ٸ� ������ ���� Move
	for (int i = 0; i < vWay.size() - MinusNum; i++)
	{
		Action* pActionMove = new EnemyAction(*(*m_MState.find(m_Animation_Name)).second);  // ���� �ִϸ��̼ǿ� �´� Action ����
		pActionMove->setFrom(vWay[i]); // ������ ����
		pActionMove->setTo(vWay[i + 1]); // ��ǥ�� ����
		pActionMove->setActive(true); // Active On
		m_Action->pushAction(pActionMove); // Action Vector�� push �ؼ� Vector ������� Action ��������.
	}

	m_Action->setMaxSize(vWay.size() - MinusNum); // �ִ� ũ�� ���� (�ִ�ũ�⸸ŭ Action ����)
	m_Action->Init(); // �׼� ������ �ʱⰪ ����

}



