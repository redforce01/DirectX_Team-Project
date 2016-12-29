#pragma once

class cTransform;
class cBoundBox;
class Unit;

class LeftArm
{

	bool m_isActive;
	cTransform* m_Trans;
	cBoundBox* m_Box;
	Unit* m_Unit;
	Unit* m_Enemy;
public:
	LeftArm(Unit* unit);
	~LeftArm();

	void AddToBody(cTransform* cBody);

	void Update(float timeDelta);
	void render();


	void CollisionUnit();

	void setActive(bool TF) { m_isActive = TF; }
	bool getisActive() { return m_isActive;  }

	cTransform* getTrans() { return m_Trans; }
	cBoundBox* getBox() { return m_Box; }

	void setTrans(cTransform* Trans) { m_Trans = Trans; }
	void setEnemy(Unit* enemy) { m_Enemy = enemy;  }

};

