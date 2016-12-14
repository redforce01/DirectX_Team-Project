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

	this->pSkinnedTrans = new cTransform();
	pSkinnedTrans->SetLocalPosition(pos);

	InitAnimation();
	InitLight();

	m_DetectedUnit = NULL;

	m_LeftHandBox = new cBaseObject;
	m_LeftHand->BoundBox

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
		for (int j = 0; j < m_curScene->getVNode().size(); j++)
		{
			Dijkstra* Dijk = new Dijkstra;
			Dijk->Init(m_curScene);
			Dijk->setStart(m_curScene->getVNode()[i]);
			Dijk->setEnd(m_curScene->getVNode()[j]);
			Dijk->FindWay();
			m_mWay.insert(make_pair(make_pair(i, j), Dijk));
		}
	}
};



void PigEnemy::Update(float timeDelta)
{
	this->pSkinnedAni->Update(timeDelta); // Animation ������Ʈ
	m_CollisionSphere->SetBound(&pSkinnedTrans->GetLocalPosition(), &D3DXVECTOR3(0.5f, 2, 0.5f)); 	// �浹 �� ������Ʈ
	m_CollisionBox->SetBound(&D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0.5f, 2.5f, 0.5f));
	m_DetectSphere->SetBound(&(pSkinnedTrans->GetLocalPosition()), &D3DXVECTOR3(0.3, 4, 0.3)); 	//������ ������Ʈ

	ray.direction = m_vEnemy[0]->getTrans()->GetWorldPosition();
	ray.origin = pSkinnedTrans->GetWorldPosition();

	m_isFind = this->CollisionEvent(timeDelta); // ���� �÷��̾� �����ߴ�?
	pHeadTrans->RotateLocal(0, 180 * ONE_RAD, 0);
	m_headPos = pHeadTrans->GetWorldPosition(); // ���� �밥�� ��ġ ����.


	if (m_isFind)
	{
		m_Action->ClearVector();// ���� ã�� ��� m_DetectedUnit�� �̵�
		AlongPlayerMove(m_DetectedUnit->getTrans());
	}
	else if (!m_isFind) // ����  �� ã�� ��� �ƹ� ��� �ϳ� ��� �װ����� �̵�.
		AlongPlayerMove(&m_curScene->getVNode()[MyUtil::RandomIntRange(0, 10)]->getTrans());

	m_Action->Update(timeDelta);
	m_Action->setCurActionSpeed((*m_MState.find(m_Animation_Name)).second->getMoveSpeed());
	//����� �ȵ�. �� �κ�.
}

void PigEnemy::Release()
{
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


	if (!m_isRayBlocking || SpDetectionCheck(this, m_vEnemy[0]))
	{
		isDetected = true;
	}
	else
	{
		isDetected = false;
	}



	//�� �浹 �Ͼ ��� -> ��������!

	if (!isAttacking && pSkinnedAni->getisAniEnd())
	{
		// �ƹ��͵� ������ �ȵ� ��� -> �׳� ��������~
		if (!isDetected && !isCollisioned)
		{
			m_DetectedUnit = NULL;
			StatePlayChange("WALK", 0.3f);

			// ���� �÷��̾� �Ѵٰ� -> �÷��̾ ���ļ� ���� ����, �÷��̾� �Ѵ� ��θ� Clear ���־�, ���� ��θ� ����� �ְ� �Ѵ�.
			if (m_isChange) {
				m_Action->ClearVector();
				m_isChange = false;
			}
			m_isAttacking = false;

		}
		// ������ ������ �� �浹 ���� -> �÷��̾ ����!.
		else if (isDetected && !isCollisioned)
		{
			m_DetectedUnit = m_vEnemy[0];  // ������ ������ ����Ƽ�忡 �־��ְ�
			this->StatePlayChange("RUN", 0.3f);  //����� �޸���!

												 // ���� �������ٰ� -> �÷��̾ ������, ���� ��θ� Clear�ϰ� ���ο� ��θ� ����� �ְ� ��.
			if (!m_isChange)
			{
				m_Action->ClearVector();
				m_isChange = true;
			}
			m_isAttacking = false;
			//m_pCurState->Move(timeDelta, *m_DetectedUnit->getTrans()); // ������ ĳ���ͷ� �̵�!
		}
		else if (isCollisioned && isDetected)
		{
			m_DetectedUnit = m_vEnemy[0];  // ������ ĳ���� ����ְ�
			this->StateOneShotChange("ATTACK_LEFT", 0.3f); // ���� �ֵθ���!
			m_Action->ClearVector(); // �� Ŭ��������~
			m_isAttacking = true;
		}
	}



	return isDetected;
}


void PigEnemy::InitAnimation()
{

	typedef struct animationClip
	{
		float move_Speed;
		float rotate_Speed;
		std::string name;
	} AnimationClip;

	animationClip animationList[10] = {
		{ 0.0f,  0.1f, "IDLE" },
		{ 0.05f, 0.2f, "WALK" },
		{ 0.11f, 0.2f, "RUN" },
		{ 0.0f, 0.2f,"IDLE_CHASE" },
		{ 0.0f, 0.2f,"IDLE_SEARCH1" },
		{ 0.0f, 0.2f,"IDLE_SEARCH2" },
		{ 0.0f, 0.2f,"ATTACK_LEFT" },
		{ 0.0f, 0.2f,"ATTACK_RIGHT" },
		{ 0.0f, 0.2f,"GRAB_FATALITY" },
		{ 0.0f,0.2f, "GRAB" },
	};

	for (int i = 0; i < sizeof(animationList) / sizeof(*animationList); i++)
	{
		Action* temp;
		temp = new EnemyAction(animationList[i].move_Speed, animationList[i].rotate_Speed, this, animationList[i].name);
		m_MState.insert(make_pair(animationList[i].name, temp));
	}

	StatePlayChange("WALK", 0.3f);
}

void PigEnemy::InitLight()
{
	//����Ʈ Ǫ��
	//����Ʈ Ǫ��
	cLight_Direction* pLight1 = new cLight_Direction();
	pLight1->Color = D3DXCOLOR(1, 1, 1, 1);
	pLight1->Intensity = 0.9f;

	cLight_Point* pLight3 = new cLight_Point();
	pLight3->Color = D3DXCOLOR(1, 1, 1, 0);
	pLight3->minRange = 5.0f;
	pLight3->maxRange = 222.0f;
	pLight3->distancePow = 10.0f;
	pLight3->pTransform->SetWorldPosition(11, 11, 0);
	pLight3->Intensity = 0.7f;

	this->lights.push_back(pLight1);
	this->lights.push_back(pLight3);

}

void PigEnemy::AlongPlayerMove(cTransform* DestTrans)
{
	// �׼� ���Ͱ� ��� ���� ��� �׼����� ��θ� �������� ��.
	// �׷��⿡ �׼� ���Ͱ� 0�̸� ��������.
	if (m_Action->getActionSize() == 0)
	{
		vector<cTransform*> vecWay; //���� ��ε��� ������ cTransform ���� ����
		Dijkstra* vDijk = NULL;  //���ͽ�Ʈ�� ��θ� ã���ִ� ������. �׷��⿡ ���� ����.
		if (!m_isAttacking) // // ������ �÷��̾� �ε�� �Ӷ��� ���ͽ�Ʈ�� ��� No�ʿ�.
		{
			m_Dijk->FindToFrom(pSkinnedTrans->GetLocalPosition(), DestTrans->GetLocalPosition()); // ���� ������ ���� ����� Node, �׸��� DestTrans�� ���� ����� Node�� ���� ã�´�!
			vDijk = (*m_mWay.find(make_pair(m_Dijk->getStart()->getID(), m_Dijk->getEnd()->getID()))).second; // ��带 ã�Ҵٸ� ���ͽ�Ʈ�� Key�� ���� �ְ� ��θ� ã��.	
			vecWay = vDijk->getNodeWay();

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



