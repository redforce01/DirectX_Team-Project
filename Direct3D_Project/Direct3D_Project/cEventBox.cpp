#include "stdafx.h"
#include "cEventBox.h"
#include "cTransform.h"
#include "cBoundBox.h"
#include "Unit.h"


cEventBox::cEventBox( Unit* unit, Unit* enemy, D3DXVECTOR3 pos, D3DXVECTOR3 Size, cScene* scene) : m_unit(unit), m_enemy(enemy), m_CurScene(scene)
{
	m_isCollisionActive = true;
	m_isActive = false;
	m_Box = new cBoundBox;

	m_Trans = new cTransform;
	m_Trans->SetWorldPosition(pos);

	m_Box->SetBound(&D3DXVECTOR3(0,0,0) , &Size);
}


cEventBox::~cEventBox()
{
}


bool cEventBox::CollisionCheck()
{
	if (m_isCollisionActive)
	{
		if (PHYSICS_MGR->IsOverlap(m_Trans, m_Box, m_unit->getTrans(), m_unit->getCollisionBox()))
		{
			return true;
		}
		else return false;
	}
	else
		return false;

}



void cEventBox::render()
{
	m_Box->RenderGizmo(m_Trans);
}

void cFirstEvent::EventCall()
{
	if (m_isCollisionActive)
	{
		m_CurScene->Event1Start();
	}
}

void cFirstEvent::EventUpdate()
{
	if (m_unit->getSkinned()->getisAniEnd())
	{
		m_CurScene->Event1End();
		m_isActive = false;
	}
}
