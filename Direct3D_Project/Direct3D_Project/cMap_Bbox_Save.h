#pragma once



class cSetBoundObject;

class cMap_Bbox_Save
{
private:
	std::vector<cSetBoundObject*> boundObj;
	cSetBoundObject*	selectBound;

public:
	cMap_Bbox_Save();
	~cMap_Bbox_Save();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);
	virtual void Scene_Render1();

	void CreateBoundBox(float timeDelta);
	void ChoiseBoundBox(cCamera* pMainCamera);
	void DeleteBoundBox();
	void SaveBoundBox();

};

