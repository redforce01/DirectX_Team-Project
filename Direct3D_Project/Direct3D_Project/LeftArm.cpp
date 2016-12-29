#include "stdafx.h"
#include "LeftArm.h"
#include "cTransform.h"
#include "cBoundBox.h"
#include "Unit.h"

LeftArm::LeftArm(Unit* unit)
{
	m_Box = new cBoundBox;
	m_Unit = unit;
}


LeftArm::~LeftArm()
{


}

void LeftArm::AddToBody(cTransform * cBody)
{
	m_Trans->AttachTo(cBody);
}

void LeftArm::Update(float timeDelta)
{
	m_Box->SetBound(&D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0.2f, 0.2f, 0.2f));
}

void LeftArm::render()
{
	if(m_isActive)
	m_Box->RenderGizmo(m_Trans);
}

void LeftArm::CollisionUnit()
{
	if (m_isActive && m_Enemy != NULL)
	{
		if (PHYSICS_MGR->IsOverlap(m_Trans, m_Box, m_Enemy->getTrans(), m_Enemy->getCollisionBox()))
		{
			m_Enemy->setDamage(1);
			m_isActive = false;
		}
	}
}
