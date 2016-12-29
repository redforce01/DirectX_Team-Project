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
	//다이젝스트라 매트릭스 초기화

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

	//시작점 찾기
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
	//대상 점이 시작점이면 0.0f 반환
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

	//다이젝스트라 매트릭스안의 모든 노드가 닫힐때까지
	m_vecWay.clear();

	while (true)
	{
		cNode* checkNode = NULL;
		float tempcost = UNKNOWNLENGTH;

		int closeNum = 0;
		//먼저 열려있으면서 가장 코스트가 작은 노드를 찾는다
		for (int i = 0; i < m_dijMat.size(); i++)
		{
			//닫혀있으면 닫힌노드 갯수를 증가시킨뒤 컨티뉴
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

		//만일 모든 노드가 닫히면 브레이크
		if (closeNum >= m_dijMat.size())
			break;

		//선택된 노드는 닫아준다
		m_dijMat[checkNode->getID()].close = true;

		//찾은 노드와 연결된 엣지코스트를 그 노드까지의 총 코스트와 비교하여 적으면 갱신
		for (int i = 0; i < m_dijMat.size(); i++)
		{
			//닫혀있는 노드는 넘김
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
	//다이젝스트라 매트릭스를 전부 채우고 나면
	//도착점에서 부터 시작점까지 구한 경로를 저장한다

	vector<int> vecN;
	vecN.push_back(m_vTo->getID());
	int via = m_dijMat[m_vTo->getID()].viaNum;
	while (via >= 0)
	{
		vecN.push_back(via);
		via = m_dijMat[via].viaNum;
	}

	//끝에서부터 저장했기 때문에 직관성있게 다시 반대로 돌려준다
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

	//다이젝스트라 매트릭스안의 모든 노드가 닫힐때까지
	//m_vecWay.clear();
	while (true)
	{
		cNode* checkNode = NULL;
		float tempcost = UNKNOWNLENGTH;

		int closeNum = 0;
		//먼저 열려있으면서 가장 코스트가 작은 노드를 찾는다
		for (int i = 0; i < dijMat.size(); i++)
		{
			//닫혀있으면 닫힌노드 갯수를 증가시킨뒤 컨티뉴
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

		//만일 모든 노드가 닫히면 브레이크
		if (closeNum >= dijMat.size())
			break;

		//선택된 노드는 닫아준다
		dijMat[checkNode->getID()].close = true;

		//찾은 노드와 연결된 엣지코스트를 그 노드까지의 총 코스트와 비교하여 적으면 갱신
		for (int i = 0; i < dijMat.size(); i++)
		{
			//닫혀있는 노드는 넘김
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
	//다이젝스트라 매트릭스를 전부 채우고 나면
	//도착점에서 부터 시작점까지 구한 경로를 저장한다

	vector<int> vecN;
	vecN.push_back(m_vTo->getID());
	int via = dijMat[m_vTo->getID()].viaNum;
	while (via >= 0)
	{
		vecN.push_back(via);
		via = dijMat[via].viaNum;
	}

	//끝에서부터 저장했기 때문에 직관성있게 다시 반대로 돌려준다
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

	////////출발점 최적화
	//FindNode(pla, from, 1);

	////////도착점 최적화
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

	// 끝과 시작점과 가장 가까운 노드를 찾는다.
	FindToFrom(from, to);

	// 다익스트라로 내가 어떻게 갈것인지 길을 정한다.
	FindWay();

	//최적화를 통해 꼭 다익스트라를 통해 하깆 않도록 한다.
//	optimization(from, to);

	// 최종적인 길 반환.

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

