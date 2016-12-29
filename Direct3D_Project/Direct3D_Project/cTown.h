#pragma once

class cTown
{
private:
	cBaseObject* _town;
	cBaseObject* skyBox;

public:
	cTown();
	~cTown();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);
	virtual void Scene_Render1();
	virtual void Scene_RenderSprite();
};
