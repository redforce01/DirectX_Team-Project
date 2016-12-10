#pragma once
#include "Unit.h"

class cTransform;
class Unit;

class Action
{
protected:
	// �̵� Ȱ��ȭ ��ų������
	bool m_isActive;
	bool m_isRotateActive;
	bool m_isFinish;
	// �̵��� �� ���� ���� 
	float m_MoveLerfPt;
	// ȸ���Ҷ� ���� ���� 
	float m_RotateLerfPt;

	string m_stateName;

	float m_Speed; // (������ �̵� ���ǵ� )
	float m_RotateSpeed; // (ȸ�� �󸶳� ���� �Ұ��ΰ�
	cTransform* m_Trans;  // ������ Ʈ������ 
	
	cTransform* m_SourTrans;  // ������ Ʈ������
	cTransform* m_DestTrans;

	
	//�� State�� ���� ����
	Unit* m_CurUnit;

	// �÷��̾��� Unit �ӽ�����
	Unit* PlayerUnit;
	
	
public:
	Action() {};
	Action(float speed, float RotateSpeed, Unit* unit, string name ) : m_Speed(speed), m_RotateSpeed(RotateSpeed), m_CurUnit(unit), m_stateName(name)
	{   
		m_Trans = unit->getTrans();
		m_isRotateActive = false;
		m_isActive = false;
		m_isFinish = false;
		//���� ���� ó���� 0~
		m_MoveLerfPt = 0;
		m_RotateLerfPt = 0;
	}
	~Action() {};
	//virtual void Update(float timeUpdate);

//	virtual void update(float timeDelta, cTransform* Sour = NULL, cTransform* Dest = NULL);

	// ���� �̵� ��ų�� ������ ����� �̵���, 
	//�������� ������ ��������.
	virtual void Move(float timeUpdate) = 0;
	virtual void MoveTo(float timeUpdate);
	//virtual void Move(float timeUpdate);

	virtual void setFrom(cTransform* from) { m_SourTrans = from; };
	virtual void setTo(cTransform* to) { m_DestTrans =  to; }

	// ���� ȸ���Ҷ� Angle or Vector ������ŭ ������. �������� �̿��� õõ�� ������ �Լ���.
	virtual void Rotate(float timeUpdate, float Angle);

	//Set �Լ���
	virtual void setActive(bool TorF) { m_isActive = TorF; }
	virtual void setRotateActive(bool TorF) { m_isRotateActive = TorF; }
	virtual void setMoveSpeed(float speed){ m_Speed = speed; }
	virtual void setRotateSpeed(float speed) { m_RotateSpeed = speed; }
	virtual void setTransform(cTransform* Trans) {	m_Trans = Trans; }

	//Get �Լ���!
	virtual bool GetisActive() { return m_isActive; }
	virtual string getAniname() { return m_stateName;  }
	virtual float getMoveSpeed() { return m_Speed; }
	virtual bool GetisFinish() { return m_isFinish;  }
	

};


class EnemyAction : public Action
{
public:
	EnemyAction(Action& action) : Action::Action(action) { }
	EnemyAction() {}
	EnemyAction(float speed, float RotateSpeed, Unit* unit, string name) : Action::Action(speed, RotateSpeed, unit, name)
	{
	}
	~EnemyAction() {};


	virtual void Move(float timeUpdate);
};

class PlayerAction : public Action
{

public:
	PlayerAction() {}
	PlayerAction(float speed, float RotateSpeed, Unit* unit, string name) : Action::Action(speed, RotateSpeed, unit, name)
	{
	}
	~PlayerAction() {};


	virtual void Move(float timeUpdate);
};











