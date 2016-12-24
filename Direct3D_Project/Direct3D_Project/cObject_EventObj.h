#pragma once

#include "cObject_Sound.h"

#define EVENT_ACTION_DISTANCE 5

class cXMesh_Skinned;
class cSkinnedAnimation;
class cBoundBox;
class cLight;
class cObject_EventObj
{
private:
	cSkinnedAnimation* pAnimation;
	cBoundBox* pEventBox;
	cTransform* pEventTrans;
	D3DXVECTOR3 m_EventPos;

	D3DXVECTOR3 m_PlayerPos;
	bool	m_IsStatic;
	bool	m_IsAction;
	string	m_PoseName;
	bool	m_isActionFinish;

	std::vector<cLight*>	lights;
	cLight_Direction*		pLightDir;
	

public:
	cObject_EventObj();
	~cObject_EventObj();

	//정적인 시체 오브젝트
	HRESULT Init(string fileName, D3DXVECTOR3 position, string poseName);

	//정적인 시체 오브젝트
	HRESULT Init(string fileName, D3DXVECTOR3 position, string poseName, D3DXVECTOR3 eventPos);

	//동적인 시체 오브젝트
	HRESULT Init(string fileName, D3DXVECTOR3 position, string poseName, bool isStatic, D3DXVECTOR3 eventPos);

	void Release();
	void Update(float timeDelta, D3DXVECTOR3 playerPos);
	void Render(const cCamera* pCamera, vector<cLight*> lights);

	void setLocalPosition(D3DXVECTOR3 positionVec);
	void setWorldPosition(D3DXVECTOR3 positionVec);

	cTransform* getEventTrans() { return pEventTrans; }

	bool UpdateColPlayer(D3DXVECTOR3 playerPos);
};

