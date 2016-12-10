#pragma once

#include "cScene.h"

class cMap_Bbox_Load;
class playerTest;

struct TAGcollision
{
	bool isRoom_0;
	bool isRoom_1;
	bool isRoom_2;
	bool isRoom_3;
	bool isRoom_4;
	bool isRoom_5;
	bool isRoom_6;

	bool isHallway_7;
	bool isHallway_8;
};

class cMap_Active :public cScene
{
private:
	TAGcollision BoundBoxColl;
	
	//bool isColl_Inspection;

private:
	//���漱��
	cMap_Bbox_Load* box_Load;
	playerTest* player;

public:
	cMap_Active();
	~cMap_Active();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);
	virtual void Scene_Render1();

	//������ �ٿ���ڽ��� ȣ������ ������ �Լ�
	void Collision_Inspection();

	//�÷��̾� ������Ʈ�浹�Լ�
	void BoxCollision(cBaseObject* playerBoundBox,std::vector<cBaseObject*> boundBox);

	//���漱�� ��ũ
	void SetLink_BoxLoad(cMap_Bbox_Load* boxload) { box_Load = boxload; }
	void SetLink_Player(playerTest* pt) { player = pt; }

	TAGcollision* Get_Is_Active() { return &BoundBoxColl; }

};

