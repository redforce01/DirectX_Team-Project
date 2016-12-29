#pragma once


class cMap_Bbox_Load;
class playerTest;
class Unit;
class Player;

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

class cMap_Active 
{
private:
	TAGcollision BoundBoxColl;
	
	//bool isColl_Inspection;

private:
	vector<cBaseObject*> m_CurBoundbox;
	//전방선언
	cMap_Bbox_Load* box_Load;
	//playerTest* player;
	Unit* player;
	Player* pp;
public:
	cMap_Active();
	~cMap_Active();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);
	virtual void Scene_Render1();

	//각방의 바운딩박스를 호출할지 말지할 함수
	void Collision_Inspection(Unit* player);

	//플레이어 오브젝트충돌함수
	void BoxCollision(Unit* player,std::vector<cBaseObject*> boundBox);
	void BoxActive(Unit* player);

	vector<cBaseObject*> BoxRayActive();

	//전방선언 링크d
	void SetLink_BoxLoad(cMap_Bbox_Load* boxload) { box_Load = boxload; }
	void SetLink_Player(Unit* pt) { player = pt; }

	vector<cBaseObject*> GetCutObjV() { return m_CurBoundbox;  }

	TAGcollision* Get_Is_Active() { return &BoundBoxColl; }

};

