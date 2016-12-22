#pragma once

#define EVENT_ACTION_DISTANCE 10

class cXMesh_Skinned;
class cSkinnedAnimation;
class cBoundBox;
class cLight;
class cObject_EventObj
{
private:
	cSkinnedAnimation* pAnimation;
	cTransform* pEventTrans;


	cBoundBox* pEventBox;
	cTransform* pEventTrans;

	D3DXVECTOR3 m_PlayerPos;
	bool	m_IsStatic;
	bool	m_IsAction;

	std::vector<cLight*>	lights;
	cLight_Direction*		pLightDir;
	

public:
	cObject_EventObj();
	~cObject_EventObj();

	//정적인 시체 오브젝트
	HRESULT Init(string fileName, D3DXVECTOR3 position);

	//정적인 시체 오브젝트
	HRESULT Init(string fileName, D3DXVECTOR3 position, cBoundBox eventBox);

	//동적인 시체 오브젝트
	HRESULT Init(string fileName, D3DXVECTOR3 position, bool isStatic, cBoundBox eventBox);

	void Release();
	void Update(float timeDelta, D3DXVECTOR3 playerPos);
	void Render(const cCamera* pCamera, vector<cLight*> lights);

	void setLocalPosition(cTransform* positionTrans);
	void setLocalPosition(D3DXVECTOR3 positionVec);

	bool UpdateColPlayer(D3DXVECTOR3 playerPos);
};

