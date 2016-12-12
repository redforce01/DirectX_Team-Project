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
	cBoundBox* pPatientBox;

	std::vector<cLight*>	lights;
	cLight_Direction*		pLightDir;

public:
	cObject_EventObj();
	~cObject_EventObj();

	HRESULT Init();
	void Release();
	void Update(float timeDelta);
	void Render(const cCamera* pCamera);

	void setLocalPosition(cTransform* positionTrans);
	void setLocalPosition(D3DXVECTOR3 positionVec);
};

