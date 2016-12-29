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
	, m_isActionFinish(FALSE)
	, m_EventPos(0, 0, 0)
	, m_PlayerPos(0, 0, 0)
{
}


cObject_EventObj::~cObject_EventObj()
{
}

HRESULT cObject_EventObj::Init(string fileName, D3DXVECTOR3 position, string poseName)
{	
	D3DXMATRIXA16 matCorrection;
	D3DXMatrixScaling(&matCorrection, 0.02f, 0.02f, 0.02f);
	//cXMesh_Skinned* pSkinnedMesh = RESOURCE_SKINNEDXMESH->GetResource(fileName, &matCorrection);
	cXMesh_Skinned* pSkinnedMesh = RESOURCE_SKINNEDXMESH->GetResource("../Resources/NPC_DEAD/NPC_DEAD.x", &matCorrection);
	pAnimation = new cSkinnedAnimation();
	pAnimation->Init(pSkinnedMesh);
	pEventTrans = new cTransform();
	pEventTrans->SetWorldPosition(position);
	m_IsStatic = TRUE;
	m_PoseName = poseName;
	m_isActionFinish = FALSE;

	return S_OK;
}

HRESULT cObject_EventObj::Init(string fileName, D3DXVECTOR3 position, string poseName, D3DXVECTOR3 eventPos)
{
	D3DXMATRIXA16 matCorrection;
	D3DXMatrixScaling(&matCorrection, 0.02, 0.02f, 0.02f);
	//cXMesh_Skinned* pSkinnedMesh = RESOURCE_SKINNEDXMESH->GetResource(fileName, &matCorrection);
	cXMesh_Skinned* pSkinnedMesh = RESOURCE_SKINNEDXMESH->GetResource("../Resources/NPC_DEAD/NPC_DEAD.x", &matCorrection);
	pAnimation = new cSkinnedAnimation();
	pAnimation->Init(pSkinnedMesh);
	pEventTrans = new cTransform();
	pEventTrans->SetWorldPosition(position);
	m_IsStatic = TRUE;
	m_EventPos = eventPos;
	m_PoseName = poseName;
	m_isActionFinish = FALSE;

	return S_OK;
}

HRESULT cObject_EventObj::Init(string fileName, D3DXVECTOR3 position, string poseName, bool isStatic, D3DXVECTOR3 eventPos)
{
	D3DXMATRIXA16 matCorrection;
	D3DXMatrixScaling(&matCorrection, 0.02f, 0.02f, 0.02f);
	//cXMesh_Skinned* pSkinnedMesh = RESOURCE_SKINNEDXMESH->GetResource(fileName, &matCorrection);
	cXMesh_Skinned* pSkinnedMesh = RESOURCE_SKINNEDXMESH->GetResource("../Resources/NPC_DEAD/NPC_DEAD.x", &matCorrection);
	pAnimation = new cSkinnedAnimation();
	pAnimation->Init(pSkinnedMesh);
	pEventTrans = new cTransform();
	pEventTrans->SetWorldPosition(position);
	m_IsStatic = isStatic;
	m_EventPos = eventPos;
	m_PoseName = poseName;
	m_isActionFinish = FALSE;

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
	pAnimation->Play(m_PoseName, 0);
	//pAnimation->PlayOneShotAfterHold(m_PoseName, 0);

	//정적인 오브젝트가 아닌 이벤트 오브젝트인지를 체크
	if (! m_IsStatic)
	{		
		if (m_IsAction)
		{
			D3DXVECTOR3 pos = pEventTrans->GetWorldPosition();
			if (pos.y > -1.8)
			{
				pos.y -= 0.2f;
				pEventTrans->SetWorldPosition(pos);
			}
			else
			{
				if (!m_isActionFinish)
				{
					SOUNDDATA->playSound(SOUND_TYPE_STRUCT_FEMALEWARD, SOUND_PLAY_TYPE_FALLOBJECT, 0, 0.5);
					m_isActionFinish = TRUE;
				}
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

	pAnimation->Render(pEventTrans);
}

void cObject_EventObj::setLocalPosition(D3DXVECTOR3 positionVec)
{
	pEventTrans->SetLocalPosition(positionVec);
}

void cObject_EventObj::setWorldPosition(D3DXVECTOR3 positionVec)
{
	pEventTrans->SetWorldPosition(positionVec);
}

bool cObject_EventObj::UpdateColPlayer(D3DXVECTOR3 playerPos)
{
	D3DXVECTOR3 pos;
	pos = m_EventPos - playerPos;

	if (D3DXVec3Length(&pos) < EVENT_ACTION_DISTANCE)
	{
		return true;
	}

	return false;
}
