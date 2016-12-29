#include "stdafx.h"
#include "cNode.h"
#include "cTransform.h"

cNode::cNode()
{
	
}

void cNode::Init(D3DXVECTOR3 vec)
{
	m_PosVec = vec;
	m_isVisit = false;
	m_isCheck = false;
	m_posTrans.SetLocalPosition(vec);
	m_NodeSpere = new cBoundSphere;
	m_NodeSpere->SetBound(&vec, &D3DXVECTOR3(0, 1, 0));

	for (size_t i = 0; i < m_vEdgeNode.size(); i++)
	{
		m_vEdgeNode[i] = NULL;
	}
	m_vEdgeNode.clear();
}

void cNode::pushEdgeNode(cNode* edgeNode)
{
	for (int i = 0; i < m_vEdgeNode.size(); i++)
	{
		if (m_vEdgeNode[i]->getID() == edgeNode->getID())
			return;
	}
	m_vEdgeNode.push_back(edgeNode);
}

float cNode::CalEdge(int ToNum)
{
		for (int i = 0; i < m_vEdgeNode.size(); i++)
		{
			if (ToNum == m_vEdgeNode[i]->getID())
			{
				D3DXVECTOR3 v1, v2, rtn;

				v1 = m_PosVec;
				v2 = m_vEdgeNode[i]->getVecPos();

				rtn = v2 - v1;

				return D3DXVec3Length(&rtn);
			}
		}
		return UNKNOWNLENGTH;
	
}

void cNode::Render()
{
	GIZMO_MGR->Circle(this->m_PosVec, 1.0f);

	for (int i = 0; i <m_vEdgeNode.size(); i++)
	{
		GIZMO_MGR->Line(this->m_PosVec, m_vEdgeNode[i]->m_PosVec);
	}

}

