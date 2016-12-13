#pragma once
#include "cScene.h"

class cXMesh_Skinned;
class cSkinnedAnimation;
class cBoundBox;
class cLight;
class cLight_Direction;

class cBoundBox;

class cScene_Main : public cScene
{
private:
	cSkinnedAnimation* pAnimation;

	cTransform* pPatientTrans;
	cBoundBox* pPatientBox;

	std::vector<cLight*>	lights;
	cLight_Direction*		pLightDir;
	bool					m_bFlag;

public:
	

public:
	cScene_Main();
	~cScene_Main();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);

	virtual void Scene_Render1();
};

