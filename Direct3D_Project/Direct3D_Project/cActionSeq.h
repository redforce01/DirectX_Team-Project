#pragma once
#include "Action.h"

class cTransform;
class Unit;

class cActionSeq
{
	int m_MaxActionIdx;
	int m_CurActionIdx;
	bool m_isSeqFinish;

	vector <Action*> m_vAction;
	Action* m_CurAction;
	Unit* m_CurUnit;


public:
	cActionSeq(Unit* unit) :m_CurUnit(unit) { }
	~cActionSeq();

	virtual void Init();


	virtual void Update(float timeDelta);
	virtual void pushAction(Action* action);

	virtual void SetActionMove(int Idx, cTransform* from, cTransform* to);
	virtual void CheckFinish();
	virtual void setMaxSize(int size) { m_MaxActionIdx = size; }
	virtual void setCurActionSpeed(float speed) { m_CurAction->setMoveSpeed(speed); }


	virtual int getActionSize() { return m_vAction.size(); }
	virtual Action* getCurAction() { return  m_vAction[m_CurActionIdx]; }

	virtual int getMaxIdx() { return m_MaxActionIdx; }
	virtual int getCurIdx() { return m_CurActionIdx; }

	virtual void ClearVector() { m_vAction.clear(); }

	//	virtual void PlayerMove();

};

