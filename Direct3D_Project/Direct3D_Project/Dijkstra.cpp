#include "stdafx.h"
#include "Dijkstra.h"
#include "cScene.h"
#include "cNode.h"



Dijkstra::~Dijkstra()
{
}


void Dijkstra::Init(cScene * scene)
{
	m_Map = scene;
	m_vNode = m_Map->getVNode();

	while (m_MatAdjacency.size() < m_Map->getVNode().size())
	{
		vector<float> temp;
		temp.resize(m_Map->getVNode().size());
		m_MatAdjacency.push_back(temp);
	}

	for (int i = 0; i < m_MatAdjacency.size(); i++)
	{
		cNode* v1;
		v1 = m_Map->getVNode()[i];

		for (int j = 0; j < m_MatAdjacency.size(); j++)
		{
			if (i == j)
			{
				m_MatAdjacency[i][j] = 0.0f;
				continue;
			}

			m_MatAdjacency[i][j] = v1->CalEdge(j);
		}
	}

	
	
}

void Dijkstra::initDijk()
{
	//��������Ʈ�� ��Ʈ���� �ʱ�ȭ

	m_dijMat.clear();
	m_dijMat.resize(m_vNode.size());
	for (int i = 0; i < m_vNode.size(); i++)
	{
		m_dijMat[i].cost = UNKNOWNLENGTH;
		m_dijMat[i].close = false;
		m_dijMat[i].viaNum = UNKNOWNVIA;

		if (i == m_vFrom->getID())
		{
			m_dijMat[i].cost = 0.0f;
			m_dijMat[i].viaNum = FROMVIA;
		}
	}
}

void Dijkstra::FindToFrom(D3DXVECTOR3 from, D3DXVECTOR3 to)
{
	float fromLength = UNKNOWNLENGTH;
	float toLength = UNKNOWNLENGTH;

	//������ ã��
	for (int i = 0; i < m_vNode.size(); i++)
	{
		float toL, fromL;
		fromL = D3DXVec3Length(&(m_vNode[i]->getVecPos() - from));
		toL = D3DXVec3Length(&(m_vNode[i]->getVecPos() - to));

		if (fromL < fromLength)
		{
			fromLength = fromL;
			m_vFrom = m_vNode[i];
		}
		if (toL < toLength)
		{
			toLength = toL;
			m_vTo = m_vNode[i];
		}
	}

}

float Dijkstra::CalcCost(vector<stDijcstraMat>& dijMat, int curNum)
{
	//��� ���� �������̸� 0.0f ��ȯ
	if (dijMat[curNum].viaNum == FROMVIA)
		return 0.0f;

	float rtn = 0.0f;
	int via = dijMat[curNum].viaNum;
	int target = curNum;
	while (via > FROMVIA)
	{
		rtn += m_MatAdjacency[via][target];
		target = via;
		via = dijMat[via].viaNum;
	}

	return rtn;
}

vector <cTransform*> Dijkstra::FindWay()
{

	//��������Ʈ�� ��Ʈ�������� ��� ��尡 ����������
	m_vecWay.clear();

	while (true)
	{
		cNode* checkNode = NULL;
		float tempcost = UNKNOWNLENGTH;

		int closeNum = 0;
		//���� ���������鼭 ���� �ڽ�Ʈ�� ���� ��带 ã�´�
		for (int i = 0; i < m_dijMat.size(); i++)
		{
			//���������� ������� ������ ������Ų�� ��Ƽ��
			if (m_dijMat[i].close)
			{
				closeNum++;
				continue;
			}

			if (!m_dijMat[i].close &&
				m_dijMat[i].cost < tempcost)
			{
				tempcost = m_dijMat[i].cost;
				checkNode = m_vNode[i];
			}
		}

		//���� ��� ��尡 ������ �극��ũ
		if (closeNum >= m_dijMat.size())
			break;

		//���õ� ���� �ݾ��ش�
		m_dijMat[checkNode->getID()].close = true;

		//ã�� ���� ����� �����ڽ�Ʈ�� �� �������� �� �ڽ�Ʈ�� ���Ͽ� ������ ����
		for (int i = 0; i < m_dijMat.size(); i++)
		{
			//�����ִ� ���� �ѱ�
			if (m_dijMat[i].close)
				continue;

			float t = m_MatAdjacency[checkNode->getID()][i];
			if (t < m_dijMat[i].cost)
			{
				m_dijMat[i].cost = t + CalcCost(m_dijMat, checkNode->getID());
				m_dijMat[i].viaNum = checkNode->getID();

				if (i == m_vFrom->getID())
					m_dijMat[i].viaNum = FROMVIA;
			}
		}
	}
	//��������Ʈ�� ��Ʈ������ ���� ä��� ����
	//���������� ���� ���������� ���� ��θ� �����Ѵ�

	vector<int> vecN;
	vecN.push_back(m_vTo->getID());
	int via = m_dijMat[m_vTo->getID()].viaNum;
	while (via >= 0)
	{
		vecN.push_back(via);
		via = m_dijMat[via].viaNum;
	}

	//���������� �����߱� ������ �������ְ� �ٽ� �ݴ�� �����ش�
	D3DXVECTOR3 v;
	for (int i = vecN.size() - 1; i >= 0; i--)
	{
		int Idx = vecN[i];
	   cTransform* tran = new cTransform();
	   tran->SetWorldPosition(m_vNode[Idx]->getTrans().GetWorldPosition());
		m_vecWay.push_back(tran);
	}

	return m_vecWay;
}

vector<cTransform*> Dijkstra::FindWay2()
{
	vector<stDijcstraMat> dijMat;
	dijMat.resize(m_vNode.size());
	for (int i = 0; i < m_vNode.size(); i++)
	{
		dijMat[i].cost = UNKNOWNLENGTH;
		dijMat[i].close = false;
		dijMat[i].viaNum = UNKNOWNVIA;

		if (i == m_vFrom->getID())
		{
			dijMat[i].cost = 0.0f;
			dijMat[i].viaNum = FROMVIA;
		}
	}

	//��������Ʈ�� ��Ʈ�������� ��� ��尡 ����������
	//m_vecWay.clear();
	while (true)
	{
		cNode* checkNode = NULL;
		float tempcost = UNKNOWNLENGTH;

		int closeNum = 0;
		//���� ���������鼭 ���� �ڽ�Ʈ�� ���� ��带 ã�´�
		for (int i = 0; i < dijMat.size(); i++)
		{
			//���������� ������� ������ ������Ų�� ��Ƽ��
			if (dijMat[i].close)
			{
				closeNum++;
				continue;
			}

			if (!dijMat[i].close &&
				dijMat[i].cost < tempcost)
			{
				tempcost = dijMat[i].cost;
				checkNode = m_vNode[i];
			}
		}

		//���� ��� ��尡 ������ �극��ũ
		if (closeNum >= dijMat.size())
			break;

		//���õ� ���� �ݾ��ش�
		dijMat[checkNode->getID()].close = true;

		//ã�� ���� ����� �����ڽ�Ʈ�� �� �������� �� �ڽ�Ʈ�� ���Ͽ� ������ ����
		for (int i = 0; i < dijMat.size(); i++)
		{
			//�����ִ� ���� �ѱ�
			if (dijMat[i].close)
				continue;

			float t = m_MatAdjacency[checkNode->getID()][i];
			if (t < dijMat[i].cost)
			{
				dijMat[i].cost = t + CalcCost(dijMat, checkNode->getID());
				dijMat[i].viaNum = checkNode->getID();

				if (i == m_vFrom->getID())
					dijMat[i].viaNum = FROMVIA;
			}
		}
	}
	//��������Ʈ�� ��Ʈ������ ���� ä��� ����
	//���������� ���� ���������� ���� ��θ� �����Ѵ�

	vector<int> vecN;
	vecN.push_back(m_vTo->getID());
	int via = dijMat[m_vTo->getID()].viaNum;
	while (via >= 0)
	{
		vecN.push_back(via);
		via = dijMat[via].viaNum;
	}

	//���������� �����߱� ������ �������ְ� �ٽ� �ݴ�� �����ش�
	D3DXVECTOR3 v;
	for (int i = vecN.size() - 1; i >= 0; i--)
	{
		int Idx = vecN[i];
		cTransform* tran = new cTransform();
		tran->SetWorldPosition(m_vNode[Idx]->getTrans().GetWorldPosition());
		m_vecWay.push_back(tran);
	}

	return m_vecWay;
}

void Dijkstra::optimization(D3DXVECTOR3 pla, D3DXVECTOR3 goal)
{
	//int from = 0;
	//int to = m_vecWay.size() - 1;
	//vector<cTransform*> vecWay;

	////////����� ����ȭ
	//FindNode(pla, from, 1);

	////////������ ����ȭ
	//FindNode(goal, to, -1);

	//if (to < from)
	//	to = from;

	//for (int i = from; i <= to; i++)
	//{
	//	vecWay.push_back(m_vecWay[i]);
	//}

	//m_vecWay.clear();

	//m_vecWay = vecWay;

	//for (int i = 0; i < m_vecWay.size(); i++)
	//{
	//	for (int j = 0; j < m_Map->getVNode().size(); j++)
	//	{
	//		if (m_vecWay[i]->GetLocalPosition() == m_Map->getVNode()[j]->getTrans().GetLocalPosition())
	//		{
	//			m_Map->getVNode()[j]->setVisit(true);
	//		}
	//	}
	//}
}

vector <cTransform*> Dijkstra::CalcWay( D3DXVECTOR3 from, D3DXVECTOR3 to)
{
	m_vNode = m_Map->getVNode();

	// ���� �������� ���� ����� ��带 ã�´�.
	FindToFrom(from, to);

	// ���ͽ�Ʈ��� ���� ��� �������� ���� ���Ѵ�.
	FindWay();

	//����ȭ�� ���� �� ���ͽ�Ʈ�� ���� �ϱ� �ʵ��� �Ѵ�.
//	optimization(from, to);

	// �������� �� ��ȯ.

	return m_vecWay;
	
}

void Dijkstra::FindNode(IN D3DXVECTOR3 from, OUT int & num, IN int isStart)
{
	float length;

	D3DXVECTOR3 ray = from;
	D3DXVECTOR3 dir = (m_vecWay[num]->GetLocalPosition() - from);
	length = D3DXVec3Length(&dir);

	float minLength = UNKNOWNLENGTH;


	if (minLength > length)
	{
		num += isStart;

		if (num < 0)
		{
			num = 0;
			return;
		}
		else if (num >= m_vecWay.size())
		{
			num = m_vecWay.size() - 1;
			return;
		}

		FindNode(from, num, isStart);
	}
	else
	{
		num -= isStart;
		if (num < 0)
			num = 0;
		else if (num >= m_vecWay.size())
			num = m_vecWay.size() - 1;

		return;
	}
}

void Dijkstra::Render()
{
}

