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
	int a = 0;
};



void PigEnemy::Update(float timeDelta)
{
	this->pSkinnedAni->Update(timeDelta); // Animation ������Ʈ

	if (!isGameEvent && m_isActive)
	{
		
		m_CollisionSphere->SetBound(&pSkinnedTrans->GetLocalPosition(), &D3DXVECTOR3(0.4f, 2, 0.4f)); 	// �浹 �� ������Ʈ
		m_CollisionBox->SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0.6f, 2.5f, 0.6f));
		m_DetectSphere->SetBound(&(pSkinnedTrans->GetLocalPosition()), &D3DXVECTOR3(1.f, 4, 1.f)); 	//������ ������Ʈ

		m_isFind = this->CollisionEvent(timeDelta); // ���� �÷��̾� �����ߴ�?
		pHeadTrans->RotateLocal(0, 180 * ONE_RAD, 0);
		m_headPos = pHeadTrans->GetWorldPosition(); // ���� �밥�� ��ġ ����.

		m_LArm->CollisionUnit();
		m_LArm->setEnemy(m_vEnemy[0]);
		m_LArm->setTrans(m_LeftHandTrans);
		m_LArm->Update(timeDelta);

		//m_vEnemy[0]->getTrans()->GetWorldPosition()

		if (m_isFind)
		{
			m_Action->ClearVector();// ���� ã�� ��� m_DetectedUnit�� �̵�
			AlongPlayerMove(m_DetectedUnit->getTrans());
		}
		else if (!m_isFind) // ����  �� ã�� ��� �ƹ� ��� �ϳ� ��� �װ����� �̵�.
			AlongPlayerMove(&m_curScene->getVNode()[MyUtil::RandomIntRange(0, 50)]->getTrans());

		m_Action->Update(timeDelta);
		m_Action->setCurActionSpeed((*m_MState.find(m_Animation_Name)).second->getMoveSpeed());
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

}

bool PigEnemy::CollisionEvent(float timeDelta)
{
	// �������� �浹���� 
	bool isDetected;
	// �浹���� �浹����
	bool isCollisioned = SpCollisionCheck(this, m_vEnemy[0]);

	if ( SpDetectionCheck(this, m_vEnemy[0]))
		isDetected = true;
	else isDetected = false;


	if (RayCollisionCheck()) {
		isDetected = false;
	}
	else {
		isDetected =  true;
	}

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
			BodyCollisionEvent();
	}

	return isDetected;
}

bool PigEnemy::RayCollisionCheck()
{
	ray.direction = m_vEnemy[0]->getTrans()->GetWorldPosition() - pSkinnedTrans->GetWorldPosition();
	ray.origin = pSkinnedTrans->GetWorldPosition();

	ray.origin.y += 2.0f;
	ray.direction.y += 2.0f;

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

void PigEnemy::NULLCollisionEvent()
{
	m_DetectedUnit = NULL;
	StatePlayChange("WALK", 0.3f);
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
	m_LArm->setActive(true);
	m_DetectedUnit = m_vEnemy[0];  // ������ ĳ���� ����ְ�

	//pSkinnedTrans->LookDirection(m_vEnemy[0]->getTrans()->GetWorldPosition());

//	if (m_vEnemy[0]->getHp() >= 1)
		this->StateOneShotChange("ATTACK_LEFT", 0.3f); // ���� �ֵθ���!
	//else
	//{
	//	//pSkinnedTrans->LookDirection(m_vEnemy[0]->getTrans()->GetWorldPosition());
	//	this->StateOneShotChange("GRAB", 0.3f);
	//	if (m_Animation_Name == "GRAB" && pSkinnedAni->getisAniEnd())
	//	{
	//		this->StateOneShotHoldChange("GRAB_FATALITY", 0.3f);
	//	}
	//}
	m_Action->ClearVector(); // �� Ŭ��������~
	m_isAttacking = true;
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
		{ 0.05f, 0.2f, "WALK" },
		{ 0.11f, 0.2f, "RUN" },
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
	
	m_LArm->render();
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



