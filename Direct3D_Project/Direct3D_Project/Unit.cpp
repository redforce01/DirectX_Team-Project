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
#include <math.h>
#include"cBoundBox.h"
#include "cBoundSphere.h"



void Unit::StatePlayChange(string StateName, float ChangeTime)
{
	m_MiState = m_MState.find(StateName);

	if (m_MiState != m_MState.end()   // Map �˻� �� ����,  
		&& m_Animation_Name != StateName)  // ���� ���°� �ְ��� �ϴ� ���¿� ���ٸ� ���� �ȹٲ㵵 ��.
	{
		m_Animation_Name = StateName;
		this->pSkinnedAni->Play(StateName, ChangeTime);
		m_pCurAction = (*m_MiState).second;
	}

}

void Unit::StateOneShotChange(string StateName, float ChangeTime)
{
	m_MiState = m_MState.find(StateName);

	if (m_MiState != m_MState.end()   // Map �˻� �� ����,  
		&& m_Animation_Name != StateName)  // ���� ���°� �ְ��� �ϴ� ���¿� ���ٸ� ���� �ȹٲ㵵 ��.
	{
		m_Animation_Name = StateName;
		this->pSkinnedAni->PlayOneShot(StateName, ChangeTime, ChangeTime);

		m_pCurAction = (*m_MiState).second;
	}
}

void Unit::StateOneShotHoldChange(string StateName, float ChangeTime)
{
	m_MiState = m_MState.find(StateName);

	if (m_MiState != m_MState.end()   // Map �˻� �� ����,  
		&& m_Animation_Name != StateName)  // ���� ���°� �ְ��� �ϴ� ���¿� ���ٸ� ���� �ȹٲ㵵 ��.
	{
		m_Animation_Name = StateName;
		this->pSkinnedAni->PlayOneShotAfterHold(StateName, ChangeTime);
		m_pCurAction = (*m_MiState).second;
	}
	//	this->pSkinnedAni->PlayOneShotAfterHold(StateName, ChangeTime);
}


// �������� �̿��Ͽ� �����̴� �Լ�. ���̳� AI�� ���� ����.
void Unit::LerpMoveControll(float timeDelta, cNode * PreNode, cNode* NextNode)
{

}


void Unit::AttachToCamera(cTransform * camera)
{
	camera->AddChild(pSkinnedTrans);

}

void Unit::CameraAttachToUnit(cTransform * camera)
{
	pSkinnedTrans->AddChild(camera);
	pMainCamera = (cCamera*)camera;
}



void Unit::pushEnemyUnitVector(Unit * unit)
{
	m_vEnemy.push_back(unit);
}

bool Unit::SpDetectionCheck(Unit* Sub, Unit* Obj)
{
	D3DXVECTOR3 DistVec = (Sub->pSkinnedTrans->GetWorldPosition() - Obj->pSkinnedTrans->GetLocalPosition());
	float DistLen = D3DXVec3Length(&DistVec);

	if (DistLen <= 8.0f)
	{
		m_DetectedUnit = Obj;
		return true;
	}
	else
	{
		//	m_DetectedUnit = NULL;
		return false;
	}
}

bool Unit::SpCollisionCheck(Unit* Sub, Unit* Obj)
{
	D3DXVECTOR3 DistVec = (pSkinnedTrans->GetWorldPosition() - Obj->pSkinnedTrans->GetLocalPosition());
	float DistLen = D3DXVec3Length(&DistVec);

	if (DistLen <= 2.0f)
	{
		m_DetectedUnit = Obj;
		return true;
	}
	else
	{
		//	m_DetectedUnit = NULL;
		return false;
	}
}

bool Unit::BoxCollisionCheck(Unit * Sub, Unit * Obj)
{

	return false;
}




void Unit::Render()
{

	//D3DXMATRIXA16 matLights[10];
	//for (int i = 0; i < this->lights.size(); i++)
	//	matLights[i] = this->lights[i]->GetLightMatrix();

	////���̴��� ������ ����
	//cXMesh_Skinned::sSkinnedMeshEffect->SetMatrixArray("matLights", matLights, 10);
	//cXMesh_Skinned::sSkinnedMeshEffect->SetInt("LightNum", this->lights.size());

	this->pSkinnedAni->Render(pSkinnedTrans);
	//GIZMO_MGR->Circle(pSkinnedTrans->GetWorldPosition(), 1);

	//m_CollisionSphere->RenderGizmo(pSkinnedTrans);
	//m_DetectSphere->RenderGizmo(pSkinnedTrans);
	//m_CollisionBox->RenderGizmo(pSkinnedTrans);
	
//	GIZMO_MGR->Circle(m_headPos, 1);


	//if(m_isRayBlocking)
	//GIZMO_MGR->Line(ray.origin, ray.direction);

//	RESOURCE_SKINNEDXMESH->GetResource(m_FilePath)->ShowAnimationName(0, 26);
}