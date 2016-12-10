#pragma once
#include "Unit.h"

class cTransform;
class Unit;

class Action
{
protected:
	// 이동 활성화 시킬것인지
	bool m_isActive;
	bool m_isRotateActive;
	bool m_isFinish;
	// 이동할 때 보간 비율 
	float m_MoveLerfPt;
	// 회전할때 보간 비율 
	float m_RotateLerfPt;

	string m_stateName;

	float m_Speed; // (유닛의 이동 스피드 )
	float m_RotateSpeed; // (회전 얼마나 빨리 할것인가
	cTransform* m_Trans;  // 유닛의 트랜스폼 
	
	cTransform* m_SourTrans;  // 유닛의 트랜스폼
	cTransform* m_DestTrans;

	
	//이 State를 갖는 유닛
	Unit* m_CurUnit;

	// 플레이어의 Unit 임시저장
	Unit* PlayerUnit;
	
	
public:
	Action() {};
	Action(float speed, float RotateSpeed, Unit* unit, string name ) : m_Speed(speed), m_RotateSpeed(RotateSpeed), m_CurUnit(unit), m_stateName(name)
	{   
		m_Trans = unit->getTrans();
		m_isRotateActive = false;
		m_isActive = false;
		m_isFinish = false;
		//보간 비율 처음은 0~
		m_MoveLerfPt = 0;
		m_RotateLerfPt = 0;
	}
	~Action() {};
	//virtual void Update(float timeUpdate);

//	virtual void update(float timeDelta, cTransform* Sour = NULL, cTransform* Dest = NULL);

	// 유닛 이동 시킬때 보간을 사용한 이동법, 
	//목적지와 도착지 설정하자.
	virtual void Move(float timeUpdate) = 0;
	virtual void MoveTo(float timeUpdate);
	//virtual void Move(float timeUpdate);

	virtual void setFrom(cTransform* from) { m_SourTrans = from; };
	virtual void setTo(cTransform* to) { m_DestTrans =  to; }

	// 유닛 회전할때 Angle or Vector 지점만큼 돌려라. 보간법을 이용한 천천히 돌리는 함수임.
	virtual void Rotate(float timeUpdate, float Angle);

	//Set 함수들
	virtual void setActive(bool TorF) { m_isActive = TorF; }
	virtual void setRotateActive(bool TorF) { m_isRotateActive = TorF; }
	virtual void setMoveSpeed(float speed){ m_Speed = speed; }
	virtual void setRotateSpeed(float speed) { m_RotateSpeed = speed; }
	virtual void setTransform(cTransform* Trans) {	m_Trans = Trans; }

	//Get 함수들!
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











