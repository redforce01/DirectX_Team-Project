#pragma once

#include "cScene.h"
#include "cMap_Bbox_Save.h"
#include "cMap_Bbox_Load.h"
#include "cDoorPosition.h"
#include "cMap_Active.h"


class cLight;
class cXMesh_Static;
class Unit;
class MoveMap;
class cNode;
class cBaseObject;
class cGameItem;

class cScene_Game : public cScene
{
private:
	Unit* Pig;
	Unit* Miles;
	std::vector<cLight*> lights;

	cSkinnedAnimation*		pSkinned1;
	cTransform*				pSkinnedTrans;


	cMap_Bbox_Save* bBox_Save;
	cMap_Bbox_Load* bBox_Load;
//	playerTest* test;
	cDoorPosition* door;
	cMap_Active* mapAcive1;
	cMap_Active* mapAcive2;

	cBaseObject* house;
	cBaseObject* air;
	D3DXMATRIXA16 matWorld;

	//*Ray
	D3DXVECTOR3 hitPos;
	D3DXVECTOR3 hitNormal;

	Ray			camRay;
	Ray			EnemycamRay;
	bool		bHit;
	int i = 0; //로그 테스트용(삭제가능)

			   //*RayPoint
	LPDIRECT3DTEXTURE9	unlockCrosshairTex;
	LPDIRECT3DTEXTURE9	lockCrosshairTex;

	//*CamTest
	cTransform*  pRightHandTrans;
	cBaseObject* testmap;

	//*GameItem
	cGameItem* keyItem;
	cGameItem* fileItem;

	vector<cGameItem*> vecGameItem;



public:
	cScene_Game();
	~cScene_Game();

	virtual void NodeInit();
	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);
	virtual void Scene_Render1();

	virtual void Scene_RenderSprite();
};

