#pragma once
#include <vector>
class cNode;
class cScene;
class cTransform;

struct stDijcstraMat
{
	bool close;		//´ÝÈû
	int	 viaNum;	//°æÀ¯ÇÏ´Â ³ëµå¹øÈ£
	float cost;		//ÃÑ°ý ÄÚ½ºÆ®
};

class Dijkstra
{
	vector< vector<float> > m_MatAdjacency;
	vector<cNode*> m_vNode;
	vector<cTransform*> m_vecWay;
	vector<stDijcstraMat> m_dijMat;
	cNode* m_vFrom;
	cNode* m_vTo;
	cScene* m_Map;

public:
	Dijkstra() {};
	~Dijkstra();

	void Init(cScene* scene);
	void initDijk();
	void FindToFrom(D3DXVECTOR3 from, D3DXVECTOR3 to);
	float CalcCost(vector<stDijcstraMat>& dijMat, int curNum);

	void optimization(D3DXVECTOR3 pla, D3DXVECTOR3 goal);

	void FindNode(IN D3DXVECTOR3 from, OUT int& num, IN int isStart);

	vector<cTransform*> getNodeWay() { return m_vecWay; }
	vector<cTransform*> CalcWay(D3DXVECTOR3 from, D3DXVECTOR3 to);
	vector <cTransform*> FindWay();
	vector <cTransform*> FindWay2();
	virtual void setStart(cNode* startTrans) { m_vFrom = startTrans; }
	virtual void setEnd(cNode* endTrans) { m_vTo = endTrans; }

	virtual cNode* getStart() { return m_vFrom;  }
	virtual cNode* getEnd() { return m_vTo; }


	void Render();


};

