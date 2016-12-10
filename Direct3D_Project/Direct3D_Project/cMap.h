#pragma once

class cTransform;
class cBaseObject;
class cLight;
class cMap
{
private:
	cBaseObject* pMap;
	std::vector<cLight*>	lights;

public:
	cMap();
	~cMap();

	HRESULT Init();
	void Release();
	void Update(float timeDelta);
	void Render1(cCamera* camera);
};

