#pragma once

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
	D3DXVECTOR3 m_PlayerPos;
	bool	m_IsStatic;

	std::vector<cLight*>	lights;
	cLight_Direction*		pLightDir;
	

public:
	cObject_EventObj();
	~cObject_EventObj();

	HRESULT Init(string fileName, D3DXVECTOR3 position);
	HRESULT Init(string fileName, D3DXVECTOR3 position, cBoundBox eventBox);
	HRESULT Init(string fileName, D3DXVECTOR3 position, bool isStatic, cBoundBox eventBox);
	void Release();
	void Update(float timeDelta, D3DXVECTOR3 playerPos);
	void Render(const cCamera* pCamera);

	void setLocalPosition(cTransform* positionTrans);
	void setLocalPosition(D3DXVECTOR3 positionVec);
};

