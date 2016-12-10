#include "stdafx.h"
#include "cActionSeq.h"
#include "cTransform.h"

cActionSeq::cActionSeq()
{
}


cActionSeq::~cActionSeq()
{
}

void cActionSeq::Init(Unit* unit)
{
	m_CurActionIdx = 0;
	m_CurAction = m_vAction[0];
	m_CurUnit = unit;

}

void cActionSeq::Update(float timeDelta)
{
	if (!m_CurAction->GetisFinish())
	{
		m_CurAction->MoveTo(timeDelta);
	}
	
	else if( m_CurActionIdx < m_MaxActionIdx - 1)
		m_CurAction = m_vAction[++m_CurActionIdx];
	else m_vAction.clear();

	// isFinish ÀÖÀ¸¸é erase 
}

void cActionSeq::pushAction(Action* action)
{
	if (action)
	{
		m_vAction.push_back(action);
	}
}

void cActionSeq::SetActionMove(int Idx, cTransform* from, cTransform* to)
{
	m_vAction[Idx]->setFrom(from);
	m_vAction[Idx]->setFrom(to);
	m_vAction[Idx]->setActive(true);
}
void cActionSeq::CheckFinish()
{
	vector<Action*> ::iterator viAction = m_vAction.begin();
	for ( ; viAction != m_vAction.end();  )
	{
		if ((*viAction)->GetisFinish())
		{
			viAction = m_vAction.erase(viAction);
			break;
		}
		else
		{
			viAction++;
		}
	}
}

