#pragma once
#include "cBaseObject.h"

class Unit;

#define DOORSPEED 3.0f
#define MAX_OPEN_ANGLE 150

enum DOORSTATE
{
	OUTER_OPENING,
	INNER_OPENING,
	INNER_OPENED,
	OUTER_OPENED,
	OUTER_CLOSING,
	INNER_CLOSING,
	CLOSED,
	LOCK
};


class cDoor : public cBaseObject
{

	float m_speed;
	float m_Angle;
	Unit* m_CurUnit;
	Unit* m_Enemy_Unit;

	bool m_isPlayerInner;
	bool m_isPlayerOuter;
	bool m_isLock;
	bool m_isOpen;

	cTransform* m_OuterTrans;
	cBoundBox* m_OuterBox;

	cTransform* m_InnerTrans;
	cBoundBox* m_InnerBox;

	DOORSTATE m_state;

public:
	cDoor(Unit* unit, Unit* enemy_Unit);
	~cDoor();

	
	virtual void InnerOpen();
	virtual void OutterOpen();

	virtual void CloseDoor();

	virtual bool  CheckInnerBound(Unit* unit);
	virtual bool  CheckOuterBound(Unit* unit);

	virtual void OpenDoorUpdate(float timeDelta);
	virtual void CloseDoorUpdate(float timeDelta);

	virtual void BaseObjectUpdate(float timeDelta);		//BaseObject 가 Update 때 실행....
	virtual void BaseObjectNoActiveUpdate(float timeDelte);	//BaseObject 가 비활성화시 업데이트 실행....

	virtual void BaseObjectRender();
	virtual void setOpen(bool TF) { m_isOpen = TF; }
	virtual bool getOpen() { return m_isOpen; }
	virtual void settingPoint();
	virtual void ComputeRay(Ray ray);
	virtual void setLock(bool TF) { m_isLock = TF; }



};

