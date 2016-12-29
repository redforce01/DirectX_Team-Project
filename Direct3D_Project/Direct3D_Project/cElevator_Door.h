#pragma once


#define DOORSIZE 0.02f

class cSkinnedAnimation;
class cTransform;
class cBoundBox;

class cElevator_Door
{
private:
	cSkinnedAnimation* e_doorSkinned;
	cTransform* e_doorTransform;
	cBoundBox*  e_doorBoundBox;
	cTransform* e_BoundBoxTransform;

	cSkinnedAnimation* e_doorSkinned_1;
	cTransform* e_doorTransform_1;
	cBoundBox*  e_doorBoundBox_1;
	cTransform* e_BoundBoxTransform_1;


public:
	cElevator_Door();
	~cElevator_Door();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);
	virtual void Scene_Render1();

	cTransform* GetTrans() { return e_doorTransform_1; }
	void SetTrans(cTransform* ct) { e_doorTransform_1 = ct; }

	cSkinnedAnimation* GetAni_1() { return e_doorSkinned; }
	cSkinnedAnimation* GetAni_2() { return e_doorSkinned_1; }
	cBoundBox* GetdoorBoundBox() { return e_doorBoundBox; }
	cTransform* GetdoorTrans() { return e_doorTransform; }
};

