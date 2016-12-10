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

	//위에서 로딩괸 SkinnedMesh 인스턴스를 만든다.
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

	//대가리 Transform 동적할당
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


	//다익스트라 경로 찾고 저장하기.
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
	this->pSkinnedAni->Update(timeDelta); // Animation 업데이트
	//m_CollisionSphere->SetBound(&pSkinnedTrans->GetLocalPosition(), &D3DXVECTOR3(2, 2, 2)); 	// 충돌 구 업데이트
	//m_DetectSphere->SetBound(&(pSkinnedTrans->GetLocalPosition()), &D3DXVECTOR3(15, 4, 15)); 	//감지구 업데이트


	m_isFind = this->CollisionEvent(timeDelta); // 적이 플레이어 감지했니?
	m_headPos = pHeadTrans->GetLocalPosition(); // 적의 대갈빡 위치 고정.

	if (m_isFind) // 적을 찾은 경우 m_DetectedUnit로 이동
		AlongPlayerMove(m_DetectedUnit->getTrans());
	else if(!m_isFind) // 적을  못 찾은 경우 아무 노드 하나 찍고 그곳으로 이동.
			AlongPlayerMove(&m_curScene->getVNode()[MyUtil::RandomIntRange(0,2)]->getTrans());

     m_Action->Update(timeDelta);
	  m_Action->setCurActionSpeed((*m_MState.find(m_Animation_Name)).second->getMoveSpeed());


		//  m_Action->ClearVector();
	  


	//지우면 안됨. 이 부분.
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
	// 감지구에 충돌된지 
	bool isDetected = SpDetectionCheck(this, m_vEnemy[0]);
	// 충돌구에 충돌된지
	bool isCollisioned = SpCollisionCheck(this, m_vEnemy[0]);


	// 아무것도 감지가 안된 경우 -> 그냥 정찰돌자~
	if (!isDetected && !isCollisioned)
	{
		m_DetectedUnit = NULL;
		StatePlayChange("WALK", 0.3f);

		// 적이 플레이어 쫓다가 -> 플레이어를 놓쳐서 정찰 돌때, 플레이어 쫓던 경로를 Clear 해주어, 정찰 경로를 만들수 있게 한다.
		if (m_isChange) {
			m_Action->ClearVector();
			m_isChange = false;
		}
		m_isAttacking = false;

	}
	// 감지는 했으나 몸 충돌 없음 -> 플레이어를 쫓자!.
	else if (isDetected && !isCollisioned)
	{
		m_DetectedUnit = m_vEnemy[0];  // 감지된 유닛을 디텍티드에 넣어주고
		this->StatePlayChange("RUN", 0.3f);  //모션을 달리자!

       // 적이 정찰돌다가 -> 플레이어를 쫓을때, 정찰 경로를 Clear하고 새로운 경로를 만들수 있게 함.
		if (!m_isChange) {
			m_Action->ClearVector();
			m_isChange = true;
		}
		m_isAttacking = false;
		//m_pCurState->Move(timeDelta, *m_DetectedUnit->getTrans()); // 감지된 캐릭터로 이동!
	}
	//몸 충돌 일어난 경우 -> 공격하자!
	else if (isCollisioned && isDetected)
	{
		m_DetectedUnit = m_vEnemy[0];  // 감지된 캐릭터 집어넣고
		this->StatePlayChange("ATTACK_LEFT", 0.3f); // 왼팔 휘두르자!

		m_Action->ClearVector(); // 걍 클리어하자~
		m_isAttacking = true;
	}
	
	return isDetected;

}


void PigEnemy::InitAnimation()
{
	// 이동속도 , 회전속도, Transform 넣을 것.
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
	//라이트 푸쉬
	//라이트 푸쉬
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
	// 액션 벡터가 비면 적이 어떻게 액션할지 경로를 만들어줘야 함.
	// 그렇기에 액션 벡터가 0이면 실행하자.
   if( m_Action->getActionSize() == 0)
    {
		
		vector<cTransform*> vecWay; //적이 경로따라 움직일 cTransform 벡터 생성
		Dijkstra* vDijk = NULL;  //다익스트라가 경로를 찾아주는 역할함. 그렇기에 변수 만듦.

		if (!m_isAttacking) // // 돼지가 플레이어 두들겨 팰때는 다익스트라 경로 No필요.
		{
			m_Dijk->FindToFrom(pSkinnedTrans->GetLocalPosition(), DestTrans->GetLocalPosition()); // 현재 적에서 가장 가까운 Node, 그리고 DestTrans랑 가장 가까운 Node를 각각 찾는다!
			vDijk = (*m_mWay.find(make_pair(m_Dijk->getStart()->getID(), m_Dijk->getEnd()->getID()))).second; // 노드를 찾았다면 다익스트라 Key에 값을 넣고 경로를 찾자.	
			vecWay = vDijk->getNodeWay();
		}

		vecWay.insert(vecWay.begin(), pSkinnedTrans );  //현재 자신 위치를 벡터의 첫번째에 넣고
		vecWay.push_back(DestTrans);  // 목표 위치를 마지막에 넣어주자...
		 
		if(m_isFind) // 적이 플레이어를 찾은 경우
			this->PushActionSeq(vecWay, 1); // 적 찾을 경우 VecSize - 1 만큼
		else this->PushActionSeq(vecWay, 2); // 적 못 찾은 경우 VecSize - 2 만큼 파라미터를 따로 넣어주자.
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

		m_Action->setMaxSize(vWay.size()- MinusNum); // 최대 크기 설정 (최대크기만큼 Action 돌음)
		m_Action->Init(this); // 액션 벡터의 초기값 설정

}



