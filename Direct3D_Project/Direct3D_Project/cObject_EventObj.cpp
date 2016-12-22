#include "stdafx.h"
#include "cObject_EventObj.h"

#include "cCamera.h"
#include "cXMesh_Skinned.h"
#include "cSkinnedAnimation.h"
#include "cLight.h"
#include "cLight_Direction.h"
#include "cLight_Point.h"

cObject_EventObj::cObject_EventObj()
	: m_IsStatic(TRUE)
	, m_IsAction(FALSE)
	, pEventTrans(NULL)
	, pEventBox(NULL)
	, m_PlayerPos(0, 0, 0)
{
}


cObject_EventObj::~cObject_EventObj()
{
}

HRESULT cObject_EventObj::Init(string fileName, D3DXVECTOR3 position)
{	
	D3DXMATRIXA16 matCorrection;
	D3DXMatrixScaling(&matCorrection, 0.1f, 0.1f, 0.1f);
	//cXMesh_Skinned* pSkinnedMesh = RESOURCE_SKINNEDXMESH->GetResource(fileName, &matCorrection);
	cXMesh_Skinned* pSkinnedMesh = RESOURCE_SKINNEDXMESH->GetResource("../Resources/Npc_Dead_Poses/Reverse/GUARD_DEAD_REVERSE.x", &matCorrection);
	pAnimation = new cSkinnedAnimation();
	pAnimation->Init(pSkinnedMesh);
	pEventTrans = new cTransform();
	pEventTrans->SetLocalPosition(position);

	return S_OK;
}

HRESULT cObject_EventObj::Init(string fileName, D3DXVECTOR3 position, cBoundBox eventBox)
{
	D3DXMATRIXA16 matCorrection;
	D3DXMatrixScaling(&matCorrection, 0.1f, 0.1f, 0.1f);
	//cXMesh_Skinned* pSkinnedMesh = RESOURCE_SKINNEDXMESH->GetResource(fileName, &matCorrection);
	cXMesh_Skinned* pSkinnedMesh = RESOURCE_SKINNEDXMESH->GetResource("../Resources/Npc_Dead_Poses/Reverse/GUARD_DEAD_REVERSE.x", &matCorrection);
	pAnimation = new cSkinnedAnimation();
	pAnimation->Init(pSkinnedMesh);
	pEventTrans = new cTransform();
	pEventTrans->SetLocalPosition(position);
	pEventBox = new cBoundBox();
	pEventBox->SetBound(&eventBox.localCenter, &eventBox.halfSize);

	return S_OK;
}

HRESULT cObject_EventObj::Init(string fileName, D3DXVECTOR3 position, bool isStatic, cBoundBox eventBox)
{
	D3DXMATRIXA16 matCorrection;
	D3DXMatrixScaling(&matCorrection, 0.1f, 0.1f, 0.1f);
	//cXMesh_Skinned* pSkinnedMesh = RESOURCE_SKINNEDXMESH->GetResource(fileName, &matCorrection);
	cXMesh_Skinned* pSkinnedMesh = RESOURCE_SKINNEDXMESH->GetResource("../Resources/Npc_Dead_Poses/FallBody/GUARD_FALL_BODY.x", &matCorrection);
	pAnimation = new cSkinnedAnimation();
	pAnimation->Init(pSkinnedMesh);
	pEventTrans = new cTransform();
	pEventTrans->SetLocalPosition(position);
	pEventBox = new cBoundBox();
	pEventBox->SetBound(&eventBox.localCenter, &eventBox.halfSize);
	m_IsStatic = isStatic;

	return S_OK;
}

void cObject_EventObj::Release()
{
	SAFE_DELETE(pEventTrans);
	SAFE_DELETE(pEventBox);

	pAnimation->Release();
}

void cObject_EventObj::Update(float timeDelta, D3DXVECTOR3 playerPos)
{
	m_PlayerPos = playerPos;


	pAnimation->Update(timeDelta);	
	pAnimation->Play("POSE0", 0.3);
	//pAnimation->Play("POSE1", 0.3);

	if (! m_IsStatic)
	{
		if (m_IsAction)
		{
			D3DXVECTOR3 pos = pEventTrans->GetLocalPosition();
			if (pos.y > 0)
			{
				pos.y -= 0.1f;
				pEventTrans->SetLocalPosition(pos);
			}
		}
		else
		{
			m_IsAction = UpdateColPlayer(playerPos);
		}
	}
}

void cObject_EventObj::Render(const cCamera* pCamera, vector<cLight*> lights)
{
	//적용되는 LightMatrix
	D3DXMATRIXA16 matLights[10];
	for (int i = 0; i < this->lights.size(); i++)
		matLights[i] = this->lights[i]->GetLightMatrix();

	//셰이더에 라이팅 셋팅
	cXMesh_Skinned::sSkinnedMeshEffect->SetMatrixArray("matLights", matLights, 10);
	cXMesh_Skinned::sSkinnedMeshEffect->SetInt("LightNum", this->lights.size());

	cXMesh_Skinned::SetLighting(&lights);

	cXMesh_Skinned::SetCamera(pCamera);
	pAnimation->Render(pEventTrans);
}

void cObject_EventObj::setLocalPosition(cTransform * positionTrans)
{
	pEventTrans->SetLocalPosition(positionTrans->GetLocalPosition());
}

void cObject_EventObj::setLocalPosition(D3DXVECTOR3 positionVec)
{
	pEventTrans->SetLocalPosition(positionVec);
}

bool cObject_EventObj::UpdateColPlayer(D3DXVECTOR3 playerPos)
{
	D3DXVECTOR3 pos;
	pos = pEventTrans->GetLocalPosition() - playerPos;

	if (D3DXVec3Length(&pos) < EVENT_ACTION_DISTANCE)
	{
		return true;
	}

	return false;
}
