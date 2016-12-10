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
//#include "CollisionEvent.h"
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

	m_CollisionSphere = new cBoundSphere;
	//m_CollisionSphere->SetBound(&pSkinnedTrans->GetLocalPosition(), &D3DXVECTOR3(25,2,25));

	ShortDetectSphere = new cBoundSphere;
	//ShortDetectSphere->SetBound(&pSkinnedTrans->GetLocalPosition(), &D3DXVECTOR3(2, 2, 2));

	m_DetectSphere = new cBoundSphere;
	//m_DetectSphere->SetBound(&pSkinnedTrans->GetLocalPosition(), &D3DXVECTOR3(11, 4, 11));

	m_CollisionBox = new cBoundBox;
	//m_CollisionBox->SetBound(&pSkinnedTrans->GetWorldPosition(), &D3DXVECTOR3(25, 3, 25));

	//�밡�� Transform �����Ҵ�
	this->pHeadTrans = new cTransform();
	this->pSkinnedAni->AddBoneTransform("Eye_R", pHeadTrans);

	this->pHeadTrans2 = new cTransform();
	this->pSkinnedAni->AddBoneTransform("NPCLarge-Head", pHeadTrans2);

	m_Action = new cActionSeq;
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
				m_mWay.insert(make_pair( make_pair(i,j) , Dijk));
		}
	}
};



void PigEnemy::Update(float timeDelta)
{
	this->pSkinnedAni->Update(timeDelta); // Animation ������Ʈ
	//m_CollisionSphere->SetBound(&pSkinnedTrans->GetLocalPosition(), &D3DXVECTOR3(2, 2, 2)); 	// �浹 �� ������Ʈ
	//m_DetectSphere->SetBound(&(pSkinnedTrans->GetLocalPosition()), &D3DXVECTOR3(15, 4, 15)); 	//������ ������Ʈ


	m_isFind = this->CollisionEvent(timeDelta); // ���� �÷��̾� �����ߴ�?
	m_headPos = pHeadTrans->GetLocalPosition(); // ���� �밥�� ��ġ ����.

	if (m_isFind) // ���� ã�� ��� m_DetectedUnit�� �̵�
		AlongPlayerMove(m_DetectedUnit->getTrans());
	else if(!m_isFind) // ����  �� ã�� ��� �ƹ� ��� �ϳ� ��� �װ����� �̵�.
			AlongPlayerMove(&m_curScene->getVNode()[MyUtil::RandomIntRange(0,2)]->getTrans());

     m_Action->Update(timeDelta);
	  m_Action->setCurActionSpeed((*m_MState.find(m_Animation_Name)).second->getMoveSpeed());


		//  m_Action->ClearVector();
	  


	//����� �ȵ�. �� �κ�.
	//ray.direction = (this->m_DetectedUnit->getTrans()->GetWorldPosition() - this->pSkinnedTrans->GetLocalPosition());
	//ray.origin = headPos;
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
	bool isDetected = SpDetectionCheck(this, m_vEnemy[0]);
	// �浹���� �浹����
	bool isCollisioned = SpCollisionCheck(this, m_vEnemy[0]);


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
		if (!m_isChange) {
			m_Action->ClearVector();
			m_isChange = true;
		}
		m_isAttacking = false;
		//m_pCurState->Move(timeDelta, *m_DetectedUnit->getTrans()); // ������ ĳ���ͷ� �̵�!
	}
	//�� �浹 �Ͼ ��� -> ��������!
	else if (isCollisioned && isDetected)
	{
		m_DetectedUnit = m_vEnemy[0];  // ������ ĳ���� ����ְ�
		this->StatePlayChange("ATTACK_LEFT", 0.3f); // ���� �ֵθ���!

		m_Action->ClearVector(); // �� Ŭ��������~
		m_isAttacking = true;
	}
	
	return isDetected;

}


void PigEnemy::InitAnimation()
{
	// �̵��ӵ� , ȸ���ӵ�, Transform ���� ��.
	Action* action;

	action = new EnemyAction(0, 0.1, this, "IDLE");
	m_MState.insert(make_pair("IDLE", action));

	action = new EnemyAction(0.08, 0.02, this, "WALK");
	m_MState.insert(make_pair("WALK", action));

	action = new EnemyAction(0.12, 0.02, this, "RUN");
	m_MState.insert(make_pair("RUN", action));

	action = new EnemyAction(0, 0.2, this, "IDLE_CHASE");
	m_MState.insert(make_pair("IDLE_CHASE", action));

	action = new EnemyAction(0, 0.2, this, "IDLE_SEARCH1");
	m_MState.insert(make_pair("IDLE_SEARCH1", action));

	action = new EnemyAction(0, 0.2, this, "IDLE_SEARCH2");
	m_MState.insert(make_pair("IDLE_SEARCH2", action));

	action = new EnemyAction(0.00, 0.2, this, "ATTACK_LEFT");
	m_MState.insert(make_pair("ATTACK_LEFT", action));

	action = new EnemyAction(0.01, 0.2, this, "ATTACK_RIGHT");
	m_MState.insert(make_pair("ATTACK_RIGHT", action));

	action = new EnemyAction(0.01, 0.2, this, "GRAB_FATALITY");
	m_MState.insert(make_pair("GRAB_FATALITY", action));

	action = new EnemyAction(0.4, 0.2, this, "GRAB");
	m_MState.insert(make_pair("GRAB", action));


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
   if( m_Action->getActionSize() == 0)
    {
		
		vector<cTransform*> vecWay; //���� ��ε��� ������ cTransform ���� ����
		Dijkstra* vDijk = NULL;  //���ͽ�Ʈ�� ��θ� ã���ִ� ������. �׷��⿡ ���� ����.

		if (!m_isAttacking) // // ������ �÷��̾� �ε�� �Ӷ��� ���ͽ�Ʈ�� ��� No�ʿ�.
		{
			m_Dijk->FindToFrom(pSkinnedTrans->GetLocalPosition(), DestTrans->GetLocalPosition()); // ���� ������ ���� ����� Node, �׸��� DestTrans�� ���� ����� Node�� ���� ã�´�!
			vDijk = (*m_mWay.find(make_pair(m_Dijk->getStart()->getID(), m_Dijk->getEnd()->getID()))).second; // ��带 ã�Ҵٸ� ���ͽ�Ʈ�� Key�� ���� �ְ� ��θ� ã��.	
			vecWay = vDijk->getNodeWay();
		}

		vecWay.insert(vecWay.begin(), pSkinnedTrans );  //���� �ڽ� ��ġ�� ������ ù��°�� �ְ�
		vecWay.push_back(DestTrans);  // ��ǥ ��ġ�� �������� �־�����...
		 
		if(m_isFind) // ���� �÷��̾ ã�� ���
			this->PushActionSeq(vecWay, 1); // �� ã�� ��� VecSize - 1 ��ŭ
		else this->PushActionSeq(vecWay, 2); // �� �� ã�� ��� VecSize - 2 ��ŭ �Ķ���͸� ���� �־�����.
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

		m_Action->setMaxSize(vWay.size()- MinusNum); // �ִ� ũ�� ���� (�ִ�ũ�⸸ŭ Action ����)
		m_Action->Init(this); // �׼� ������ �ʱⰪ ����

}



