#pragma once

#include "cScene.h"
#include "cMap_Bbox_Save.h"
#include "cMap_Bbox_Load.h"
#include "cDoorPosition.h"
#include "cMap_Active.h"
#include "cElevator.h"
#include "cElevator_Door.h"
#include "cElevator_passageway.h"
#include "cEventBox.h"
#include "cEventManager.h"

#include "cEffect.h"
#include "cCameraUI.h"
#include "cFileRead.h"
#include "cMovieEvent.h"
#include "cCloseEyeEvent.h"

#define EndingX -21.21
#define EndingZ -36.49

class cDustFog;
class cLightGlow;
class cLight;
class cXMesh_Static;
class Unit;
class MoveMap;
class cNode;
class cBaseObject;
class cGameItem;


#define MilESPOSX 5.07f
#define MilESPOSY -20.5f
#define MilESPOSZ 15.14f

enum EVENTNUM
{
	FIRST_MEET

};




class cScene_Game : public cScene
{
	
private:
	LPDIRECT3DTEXTURE9   noneTex;
	vector<cDustFog*>         arrDustFog;
	vector<cLightGlow*>      arrGlow;
	vector<cLight*> vLight;
	cEffect* effect;
	cCameraUI* cameraUI;
	cFileRead* fileRead;
	cMovieEvent* movieEvent;
	cCloseEyeEvent* eyeEvent;

	static bool m_bNightVision;         //카메라 나이트비전 실행할거냐.
	static bool m_bIsBroken;            //카메라 이펙트 깨진거 넣을거냐.
	bool isEvent1End;
	Unit* Pig;
	Unit* Miles;
	std::vector<cLight*> lights;

	cSkinnedAnimation*		pSkinned1;
	cTransform*				pSkinnedTrans;

	cEventManager* m_EventManager;
	cMap_Bbox_Save* bBox_Save;
	cMap_Bbox_Load* bBox_Load;
//	playerTest* test;
	cDoorPosition* door;
	cMap_Active* m_Player_mapAcive;
	cMap_Active* m_Enemy_mapAcive;

	cBaseObject* house;
	cBaseObject* air;
	D3DXMATRIXA16 matWorld;

	cElevator_Door* Ele_Door;
	cElevator* elevatorl;
	cElevator_passageway* e_passageway;

	//*Ray
	D3DXVECTOR3 hitPos;
	D3DXVECTOR3 hitNormal;

	
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
	cGameItem* keyItem_0;
	cGameItem* keyItem_1;
	cGameItem* noteItem_2;
	cGameItem* noteItem_3;
	cGameItem* noteItem_4;
	cGameItem* noteItem_5;
	cGameItem* noteItem_6;

	vector<cGameItem*> vecGameItem;
	int         m_itemIndex;

	vector <cEventBox*> m_vEventBox;
	vector <cEventBox*>::iterator m_viEventBox;


	// 누나꺼 //

	cTransform*            ItemPocket;
	bool               bHitRay;
	bool               bItemClick;
	float               ItemSpeed;

	vector<cBaseObject*>   m_vecItemBoundBox;
	bool               bHitBoundBox;

	float camFov;

	bool isEscape;
	bool isRoom_Five;

public:


	cScene_Game();
	~cScene_Game();

	virtual void NodeInit();
	virtual void DijkNodeFileIO(const char* filename);
	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);
	virtual void Scene_Render1();
	virtual void GainItem();
	virtual void ItemMoveToMiles(float timeDelta);

	virtual void InnerOpenDoor(int Idx);
	virtual void OutterOpenDoor(int Idx);
	virtual void CloseDoor(int Idx);

	virtual void Event1Start();
	virtual void Event1End();
	virtual void EventEndToNormal();
	virtual void Scene_RenderSprite();
	virtual void ControllNightVision();
	virtual void ControllCamMode();

	virtual void DeadEvent();

	static void SetBroken(bool isBroken) { m_bIsBroken = isBroken; }
	static void SetNightVision(bool isNightVision) { m_bNightVision = isNightVision; }
	static bool GetBroken() { return m_bIsBroken; }
	static bool GetNightVision() { return m_bNightVision; }

	virtual void CloseEye(int num, float speed);
	virtual void SetCamFove(float fov) { camFov = fov; }
	virtual float GetCamFov() { return camFov; }
	void ItemInit();
	void ItemUseFile(bool rayHit, int i);

	bool GetIsEscape() { return isEscape; }
	bool GetIsRoom_Five() { return isRoom_Five; }
	
	void InitEventObject();
	void InitSoundObject();
};