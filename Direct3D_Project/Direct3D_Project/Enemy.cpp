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

	//위에서 로딩괸 SkinnedMesh 인스턴스를 만든다.
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

	//대가리 Transform 동적할당
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

	//다익스트라 경로 찾고 저장하기.
	for (int i = 0; i < m_curScene->getVNode().size(); i++)
	{
		m_Dijk->setStart(m_curScene->getVNode()[i]); // 출발지 설정
		m_Dijk->initDijk(); // 다익스트라 테이블 초기화

		for (int j = 0; j < m_curScene->getVNode().size(); j++)
		{
			vector<cTransform*> cTran;
			m_Dijk->setEnd(m_curScene->getVNode()[j]); // 도착지 설정
			cTran = m_Dijk->FindWay(); // 길찾는다
			m_mvWay.insert(make_pair(make_pair(i, j), cTran)); // 찾은길 map에 넣는다.
		}
	}
	int a = 0;
};



void PigEnemy::Update(float timeDelta)
{
	this->pSkinnedAni->Update(timeDelta); // Animation 업데이트

	if (!isGameEvent && m_isActive)
	{
		
		m_CollisionSphere->SetBound(&pSkinnedTrans->GetLocalPosition(), &D3DXVECTOR3(0.4f, 2, 0.4f)); 	// 충돌 구 업데이트
		m_CollisionBox->SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0.6f, 2.5f, 0.6f));
		m_DetectSphere->SetBound(&(pSkinnedTrans->GetLocalPosition()), &D3DXVECTOR3(1.f, 4, 1.f)); 	//감지구 업데이트

		m_isFind = this->CollisionEvent(timeDelta); // 적이 플레이어 감지했니?
		pHeadTrans->RotateLocal(0, 180 * ONE_RAD, 0);
		m_headPos = pHeadTrans->GetWorldPosition(); // 적의 대갈빡 위치 고정.

		m_LArm->CollisionUnit();
		m_LArm->setEnemy(m_vEnemy[0]);
		m_LArm->setTrans(m_LeftHandTrans);
		m_LArm->Update(timeDelta);

		//m_vEnemy[0]->getTrans()->GetWorldPosition()

		if (m_isFind)
		{
			m_Action->ClearVector();// 적을 찾은 경우 m_DetectedUnit로 이동
			AlongPlayerMove(m_DetectedUnit->getTrans());
		}
		else if (!m_isFind) // 적을  못 찾은 경우 아무 노드 하나 찍고 그곳으로 이동.
			AlongPlayerMove(&m_curScene->getVNode()[MyUtil::RandomIntRange(0, 50)]->getTrans());

		m_Action->Update(timeDelta);
		m_Action->setCurActionSpeed((*m_MState.find(m_Animation_Name)).second->getMoveSpeed());
	}
	//지우면 안됨. 이 부분.
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
	// 감지구에 충돌된지 
	bool isDetected;
	// 충돌구에 충돌된지
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
		// 아무것도 감지가 안된 경우 -> 그냥 정찰돌자~
		if (!isDetected && !isCollisioned)
			NULLCollisionEvent();
		// 감지는 했으나 몸 충돌 없음 -> 플레이어를 쫓자!.
		else if (isDetected && !isCollisioned)
			DetectCollisionEvent();
		//몸 충돌 일어난 경우 -> 공격하자!
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
	// 적이 플레이어 쫓다가 -> 플레이어를 놓쳐서 정찰 돌때, 플레이어 쫓던 경로를 Clear 해주어, 정찰 경로를 만들수 있게 한다.
	if (m_isChange) {
		m_Action->ClearVector();
		m_isChange = false;
	}
	m_isAttacking = false;
}

void PigEnemy::DetectCollisionEvent()
{
	m_DetectedUnit = m_vEnemy[0];  // 감지된 유닛을 디텍티드에 넣어주고
	this->StatePlayChange("RUN", 0.3f);  //모션을 달리자!
	m_LArm->setActive(false);
	// 적이 정찰돌다가 -> 플레이어를 쫓을때, 정찰 경로를 Clear하고 새로운 경로를 만들수 있게 함.
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
	m_DetectedUnit = m_vEnemy[0];  // 감지된 캐릭터 집어넣고

	//pSkinnedTrans->LookDirection(m_vEnemy[0]->getTrans()->GetWorldPosition());

//	if (m_vEnemy[0]->getHp() >= 1)
		this->StateOneShotChange("ATTACK_LEFT", 0.3f); // 왼팔 휘두르자!
	//else
	//{
	//	//pSkinnedTrans->LookDirection(m_vEnemy[0]->getTrans()->GetWorldPosition());
	//	this->StateOneShotChange("GRAB", 0.3f);
	//	if (m_Animation_Name == "GRAB" && pSkinnedAni->getisAniEnd())
	//	{
	//		this->StateOneShotHoldChange("GRAB_FATALITY", 0.3f);
	//	}
	//}
	m_Action->ClearVector(); // 걍 클리어하자~
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
	//라이트 푸쉬
	//라이트 푸쉬
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
	// 액션 벡터가 비면 적이 어떻게 액션할지 경로를 만들어줘야 함.
	// 그렇기에 액션 벡터가 0이면 실행하자.
	if (m_Action->getActionSize() == 0)
	{
		if (!m_isAttacking) // // 돼지가 플레이어 두들겨 팰때는 다익스트라 경로 No필요.
		{
			vector<cTransform*> vecWay; //적이 경로따라 움직일 cTransform 벡터 생성
			m_Dijk->FindToFrom(pSkinnedTrans->GetLocalPosition(), DestTrans->GetLocalPosition()); // 현재 적에서 가장 가까운 Node, 그리고 DestTrans랑 가장 가까운 Node를 각각 찾는다!
			vecWay = (*m_mvWay.find(make_pair(m_Dijk->getStart()->getID(), m_Dijk->getEnd()->getID()))).second; // 노드를 찾았다면 다익스트라 Key에 값을 넣고 경로를 찾자.	
			//vecWay = vDijk->getNodeWay();
			//	vecWay.insert(vecWay.begin(), pSkinnedTrans );  //현재 자신 위치를 벡터의 첫번째에 넣고
			vecWay.push_back(DestTrans);  // 목표 위치를 마지막에 넣어주자...

			if (m_isFind) // 적이 플레이어를 찾은 경우
				this->PushActionSeq(vecWay, 1); // 적 찾을 경우 VecSize - 1 만큼
			else this->PushActionSeq(vecWay, 2); // 적 못 찾은 경우 VecSize - 2 만큼 파라미터를 따로 넣어주자.

		}

	}

	//vecWay = Dij.CalcWay(this->pSkinnedTrans->GetWorldPosition(), m_DetectedUnit->getTrans()->GetLocalPosition());
}

// 이제 정한 경로를 Actoin 벡터에 넣어서 그 벡터따라 움직이도록 한다!
void PigEnemy::PushActionSeq(vector<cTransform*> vWay, int MinusNum)
{
	// 경로 만큼 액션을 넣어주자!  ※ 액션이란? -> 적이 한 노드에서 다른 노드까지 가는 Move
	for (int i = 0; i < vWay.size() - MinusNum; i++)
	{
		Action* pActionMove = new EnemyAction(*(*m_MState.find(m_Animation_Name)).second);  // 현재 애니메이션에 맞는 Action 넣자
		pActionMove->setFrom(vWay[i]); // 시작점 설정
		pActionMove->setTo(vWay[i + 1]); // 목표점 설정
		pActionMove->setActive(true); // Active On
		m_Action->pushAction(pActionMove); // Action Vector에 push 해서 Vector 순서대로 Action 진행하자.
	}

	m_Action->setMaxSize(vWay.size() - MinusNum); // 최대 크기 설정 (최대크기만큼 Action 돌음)
	m_Action->Init(); // 액션 벡터의 초기값 설정

}



