#include "stdafx.h"
#include "cDoor.h"
#include "cTransform.h"
#include "cBaseObject.h"
#include "cBoundBox.h"
#include "Unit.h"


cDoor::cDoor(Unit* unit, Unit* enemy_Unit) : m_CurUnit(unit), m_Enemy_Unit(enemy_Unit)
{

	m_OuterBox = new cBoundBox();
	m_InnerBox = new cBoundBox();
	
	m_OuterTrans = new cTransform();
	m_InnerTrans = new cTransform();

	bActive = true;
	m_isOpen = false;
	m_Angle = 0;
	m_state = CLOSED;
	m_isLock = false;
	m_isPlayerInner = false;
	m_isPlayerOuter = false;
}

cDoor::~cDoor()
{
}

void cDoor::InnerOpen() {
	
	bActive = false;
	m_isPlayerInner = true;
	m_isPlayerOuter = false;
}

void cDoor::OutterOpen() {

	bActive = false;
	m_isPlayerInner = false;
	m_isPlayerOuter = true;
}

void cDoor::CloseDoor()
{

	bActive = false;
}

bool cDoor::CheckInnerBound(Unit* unit)
{
		if (PHYSICS_MGR->IsOverlap(m_InnerTrans, m_InnerBox, unit->getTrans(), unit->getCollisionBox()))
		{
			return true;
		}
		return false;
	
}

bool cDoor::CheckOuterBound(Unit* unit)
{
	if (PHYSICS_MGR->IsOverlap(m_OuterTrans, m_OuterBox, unit->getTrans(), unit->getCollisionBox()))
	{
		return true;
	}
	return false;
}

void cDoor::OpenDoorUpdate(float timeDelta)
{
	if (!m_isOpen && m_state == CLOSED)
	{
		if (m_isPlayerOuter  && !m_isPlayerInner)
		{
			m_Angle += DOORSPEED;
			if (m_Angle >= MAX_OPEN_ANGLE) {
				m_Angle = 0;
				bActive = true;
				m_isOpen = true;
				m_state = INNER_OPENED;
				m_isPlayerInner = false;
				m_isPlayerOuter = false;
			}
			
			m_InnerTrans->RotateLocal(0, DOORSPEED * ONE_RAD , 0);
			m_OuterTrans->RotateLocal(0, DOORSPEED * ONE_RAD , 0);
			pTransform->RotateLocal(0, DOORSPEED * ONE_RAD , 0);
		}
		else if (m_isPlayerInner && !m_isPlayerOuter)
		{
			m_Angle += DOORSPEED;
			if (m_Angle >= MAX_OPEN_ANGLE) {
				m_Angle = 0;
				bActive = true;
				m_isOpen = true;
				m_state = OUTER_OPENED;
				m_isPlayerInner = false;
				m_isPlayerOuter = false;
			}

			m_InnerTrans->RotateLocal(0, -DOORSPEED * ONE_RAD, 0);
			m_OuterTrans->RotateLocal(0, -DOORSPEED * ONE_RAD , 0);
			pTransform->RotateLocal(0, -DOORSPEED * ONE_RAD , 0);
		}
		
	}


	
}

void cDoor::CloseDoorUpdate(float timeDelta)
{

	if (m_isOpen && m_state != CLOSED)
	{
		if (m_state == OUTER_OPENED)
		{
			m_Angle += DOORSPEED;
			if (m_Angle >= MAX_OPEN_ANGLE) {
				m_Angle = 0;
				bActive = true;
				m_isOpen = false;
				m_state = CLOSED;
			}
			m_InnerTrans->RotateLocal(0, DOORSPEED * ONE_RAD , 0);
			m_OuterTrans->RotateLocal(0, DOORSPEED * ONE_RAD , 0);
			pTransform->RotateLocal(0, DOORSPEED * ONE_RAD , 0);
		}
		else if (m_state == INNER_OPENED)
		{
			m_Angle += DOORSPEED;
			if (m_Angle >= MAX_OPEN_ANGLE) {
				m_Angle = 0;
				bActive = true;
				m_isOpen = false;
				m_state = CLOSED;
				
			}
			m_InnerTrans->RotateLocal(0, -DOORSPEED * ONE_RAD, 0);
			m_OuterTrans->RotateLocal(0, -DOORSPEED * ONE_RAD , 0);
			pTransform->RotateLocal(0, -DOORSPEED * ONE_RAD , 0);
		}
		
	}

	/*pTransform->RotateLocal(0.00f, -90 * ONE_RAD, 0);
	m_OuterTrans->RotateLocal(0.00f, -90 * ONE_RAD, 0.00);
	m_InnerTrans->RotateLocal(0.00f, -90 * ONE_RAD, 0.00);*/

}

void cDoor::BaseObjectUpdate(float timeDelta)
{
	cTransform Trans;
	
	if (PHYSICS_MGR->IsRayHitStaticMeshObject(&m_ray, this, NULL, NULL))
	{
		if (!m_isLock)
		{
			if (KEY_MGR->IsOnceDown(VK_LBUTTON))
			{
				if (this->CheckInnerBound(m_CurUnit))
				{
					SOUNDDATA->playSound(SOUND_TYPE_NPC, SOUND_PLAY_TYPE_NPC_OPEN_DOOR, 0, 0.1);
					//m_state = OUTER_OPENING;
					bActive = false;
					m_isPlayerInner = true;
					m_isPlayerOuter = false;
					return;
				}
				else if (CheckOuterBound(m_CurUnit))
				{
					SOUNDDATA->playSound(SOUND_TYPE_NPC, SOUND_PLAY_TYPE_NPC_OPEN_DOOR, 0, 0.1);
					//m_state = INNER_OPENING;
					bActive = false;
					m_isPlayerInner = false;
					m_isPlayerOuter = true;
					return;
				}
			}
		}
		else
		{
			if (KEY_MGR->IsOnceDown(VK_LBUTTON))
			{
				m_isPlayerInner = false;
				m_isPlayerOuter = false;
				SOUNDDATA->playSound(SOUND_TYPE_NPC, SOUND_PLAY_TYPE_NPC_TRYOPEN_DOOR, 0, 0.1);
			}
		}
		
	}

	if (m_isOpen || m_isLock)
		return;

	if (CheckOuterBound(m_Enemy_Unit))
	{
		bActive = false;
		m_isPlayerInner = false;
		m_isPlayerOuter = true;
		SOUNDDATA->playSound(SOUND_TYPE_NPC, SOUND_PLAY_TYPE_NPC_OPEN_DOOR, 0, 0.1);
		return;

	}
	if (CheckInnerBound(m_Enemy_Unit))
	{

		bActive = false;
		m_isPlayerInner = true;
		m_isPlayerOuter = false;
		SOUNDDATA->playSound(SOUND_TYPE_NPC, SOUND_PLAY_TYPE_NPC_OPEN_DOOR, 0, 0.1);
		return;
	}
		
	
}

void cDoor::BaseObjectNoActiveUpdate(float timeDelte)
{
	if (!m_isOpen)
	{
		OpenDoorUpdate(timeDelte);
		return;
	}
	else
	{
		CloseDoorUpdate(timeDelte);
		return;
	}
}

void cDoor::BaseObjectRender()
{
	cBaseObject::BaseObjectRender();
	if(m_OuterBox != NULL)
	m_OuterBox->RenderGizmo(m_OuterTrans);
	if (m_InnerBox != NULL)
	m_InnerBox->RenderGizmo(m_InnerTrans);
}

void cDoor::settingPoint()
{

	m_OuterTrans->SetLocalPosition(pTransform->GetWorldPosition());
	m_OuterTrans->SetRotateWorld(pTransform->GetWorldRotateMatrix());

	m_InnerTrans->SetLocalPosition(pTransform->GetWorldPosition());
	m_InnerTrans->SetRotateWorld(pTransform->GetWorldRotateMatrix());

	ComputeBoundBox();
	m_OuterBox->SetBound(&D3DXVECTOR3(-0.55f,0, BoundBox.halfSize.z), &BoundBox.halfSize);
	m_InnerBox->SetBound(&D3DXVECTOR3(0.55f, 0, BoundBox.halfSize.z ), &BoundBox.halfSize);
}

void cDoor::ComputeRay(Ray ray)
{
	m_ray = ray;
}



