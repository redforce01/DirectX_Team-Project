#pragma once
#include "cBaseObject.h"

class cBoundBox;
class Unit;
class cScene;

class cEventBox
{
protected:
	cScene* m_CurScene;
	cBoundBox* m_Box;
	cTransform* m_Trans;
	Unit* m_unit;
	Unit* m_enemy;
	bool m_isCollisionActive;
	bool m_isActive;

public:
	cEventBox(Unit* unit, Unit* enemy, D3DXVECTOR3 pos, D3DXVECTOR3 Size, cScene* scene);
	~cEventBox();

	virtual void SetBound(D3DXVECTOR3 pos, D3DXVECTOR3 HalfSize) { m_Box->SetBound(&pos, &HalfSize); }
	virtual bool CollisionCheck();

	//virtual void update();

	virtual void setActive(bool TF) { m_isActive = TF; }
	virtual bool getActive() { return m_isActive; }

	virtual void setCollisionActive(bool TF) { m_isCollisionActive = TF; }
	virtual bool getCollisionActive() { return m_isCollisionActive; }
	
	virtual void render();
	virtual void EventUpdate() = 0;
	virtual void EventCall() = 0;
};

class cFirstEvent :public cEventBox
{

public:
	cFirstEvent(Unit* unit, Unit* enemy, D3DXVECTOR3 pos, D3DXVECTOR3 Size, cScene* scene) : cEventBox(unit, enemy, pos, Size, scene) {}

	~cFirstEvent() {};

	virtual void EventCall();
	virtual void EventUpdate();
};

