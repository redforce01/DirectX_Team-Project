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
	//m_Box->RenderGizmo(m_Trans);
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
	if (m_unit->getSkinned()->getisAniEnd() && m_unit->getCurAniName() == "GRAB_AND_THROW")
	{
		m_CurScene->Event1End();
		m_isActive = false;
	}
}

//void cFirstEvent::Event1End()
//{
//	eyeEvent->isPlayEye = true;
//	Pig->getTrans()->SetWorldPosition(-14.94, 0.00, -24.81);
//	isEvent1End = true;
//}
//
//void cFirstEvent::EventEndToNormal()
//{
//	if (!eyeEvent->isPlayEye && this->isEvent1End)
//	{
//		this->isEvent1End = false;
//		isGameEvent = false;
//		Pig->setisActive(true);
//		Miles->getTrans()->SetWorldPosition(6.78, 0.0f, -13.80); // ������ ��ġ ����
//	}
//
//}
//
//void cFirstEvent::Event1Start()
//{
//	isGameEvent = true;
//
//	m_enemy->getTrans()->SetLocalPosition(6.78, 0, -15.50); // ���� �ڸ����
//	m_enemy->getTrans()->LookDirection(m_unit->getTrans()->GetWorldPosition()); // ������ �ٶ󺸰� �ϰ�
//	m_enemy->StateOneShotHoldChange("GRAB_FATALITY", 0.5f); // �ִϸ��̼ǽ���
//	m_enemy->setisActive(false); // Ȱ��ȭ false
//
//	m_unit->getTrans()->LookDirection(-Pig->getTrans()->GetWorldPosition()); //������� ���� ����
//	m_unit->StateOneShotHoldChange("GRAB_AND_THROW", 0.5f); // ������ �ִϸ��̼�
//	m_unit->getTrans()->SetWorldPosition(6.78, 0.8f, -14.3); // ������ ��ġ ����
//}

void cDoorCloseEvent::EventCall()
{

	m_CurScene->InnerOpenDoor(1);

}

 void cDoorCloseEvent::EventUpdate()
{
}
