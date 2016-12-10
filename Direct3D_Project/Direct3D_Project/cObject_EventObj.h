#pragma once

class cXMesh_Skinned;
class cSkinnedAnimation;
class cBoundBox;
class cLight;
class cLight_Direction;
class cBoundBox;
class cObject_EventObj
{
private:
	cSkinnedAnimation* pAnimation;

	cTransform* pPatientTrans;
	cBoundBox* pPatientBox;

	std::vector<cLight*>	lights;
	cLight_Direction*		pLightDir;

public:
	cObject_EventObj();
	~cObject_EventObj();

	HRESULT Init();
	void Release();
	void Update(float timeDelta);
	void Render();
};

