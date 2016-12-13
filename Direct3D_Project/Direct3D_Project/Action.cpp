#include "stdafx.h"
#include "Action.h"
#include "Unit.h"
#include "cTransform.h"




void Action::MoveTo(float timeUpdate)
{
	cTransform Trans;
	D3DXVECTOR3 Dist = m_Trans->GetLocalPosition() - m_DestTrans->GetLocalPosition();
	float Lenth = D3DXVec3Length(&Dist);
	Trans.LookPosition(Dist);

	D3DXVec3Normalize(&Dist, &Dist);

	if (Lenth >= 0.22) {

		if (m_RotateLerfPt < 1.0)
			m_Trans->RotateSlerp(*m_Trans, Trans, m_RotateLerfPt += (m_RotateSpeed / 2));

		m_Trans->MovePositionWorld(-Dist.x * m_Speed, 0, -Dist.z * m_Speed);

		if (m_RotateLerfPt >= 1.0)
			m_RotateLerfPt = 0;
	}
	else {
		// ���� ���ϴ� �������� ������ Avtive ����!.
		m_MoveLerfPt = 0;
		m_RotateLerfPt = 0;
		m_Trans->SetWorldPosition(m_DestTrans->GetWorldPosition());

		m_isActive = false;  // Ȱ��ȭ ��
		m_isFinish = true;  // ���� �� �̰� ������.
							//	m_CurUnit->setMoving(false);
	}
}


// �����Ͽ� Angle��ŭ ȸ����.
void Action::Rotate(float timeUpdate, float Angle)
{
	if (m_isRotateActive)
	{
		m_RotateLerfPt += m_RotateSpeed;

		if (m_RotateLerfPt < D3DXToDegree(Angle))
		{
			m_Trans->RotateSelf({ 0, D3DXToRadian(m_RotateSpeed) , 0 });
		}
		else
		{
			m_isRotateActive = false;
			m_RotateLerfPt = 0;
		}
	}
}

//AI�� �ڵ��̵� �����Ҷ� ������, �������� ������� �����ϸ� ������ �Ἥ �̵���~
void EnemyAction::Move(float timeUpdate)
{
	cTransform Trans;
	D3DXVECTOR3 Dist = m_SourTrans->GetLocalPosition() - m_DestTrans->GetLocalPosition();
	Trans.LookPosition(Dist);

	//������ �̿��� �����ϴµ� �Ÿ��� ��� ª�簣�� ���ǵ�� �����ؾ� �ϴ�,  ���ǵ� �ٽ� ���
	float Speed = fabs(m_Speed / (float)D3DXVec3Length(&Dist));

	if (m_isActive)
	{
		m_MoveLerfPt += Speed;
		m_Trans->SetWorldPosition(MyUtil::VecLerp(m_SourTrans->GetWorldPosition(), m_DestTrans->GetWorldPosition(), m_MoveLerfPt));

		if (m_RotateLerfPt <= 1.0) m_Trans->RotateSlerp(*m_SourTrans, Trans, m_RotateLerfPt += m_RotateSpeed);

		if (m_MoveLerfPt >= 1.0) {
			m_RotateLerfPt = 0;
			m_MoveLerfPt = 0;
			m_isActive = false;
			m_isFinish = true;  // ���� �� �̰� ������.
		}
	}
}

void PlayerAction::Move(float timeUpdate)
{
	//	float deltaAngle = 90.0f * ONE_RAD * timeDelta;
	if (KEY_MGR->IsStayDown('A'))
	{
		m_Trans->MovePositionSelf(m_Speed * timeUpdate, 0.0f, 0.0f);
	}
	else if (KEY_MGR->IsStayDown('D'))
	{
		m_Trans->MovePositionSelf(-m_Speed* timeUpdate, 0.0f, 0.0f);
	}

	if (KEY_MGR->IsStayDown('W'))
	{
		m_Trans->MovePositionSelf(0.0f, 0.0f, -m_Speed* timeUpdate);
	}
	else if (KEY_MGR->IsStayDown('S'))
	{
		m_Trans->MovePositionSelf(0.0f, 0.0f, m_Speed* timeUpdate);
	}
}
