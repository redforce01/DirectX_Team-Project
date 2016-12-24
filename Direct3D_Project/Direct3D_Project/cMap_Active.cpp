#include "stdafx.h"
#include "cMap_Active.h"
//전방선언
#include "cMap_Bbox_Load.h"
//#include "playerTest.h"
#include "Unit.h"


cMap_Active::cMap_Active()
{
	BoundBoxColl.isRoom_0 = false;
	BoundBoxColl.isRoom_1 = false;
	BoundBoxColl.isRoom_2 = false;
	BoundBoxColl.isRoom_3 = false;
	BoundBoxColl.isRoom_4 = false;
	BoundBoxColl.isRoom_5 = false;
	BoundBoxColl.isRoom_6 = false;

	BoundBoxColl.isHallway_7 = false;
	BoundBoxColl.isHallway_8 = false;

	//isColl_Inspection = false;
}


cMap_Active::~cMap_Active()
{
}

HRESULT cMap_Active::Scene_Init()
{

	return S_OK;
}

void cMap_Active::Scene_Release()
{
}

void cMap_Active::Scene_Update(float timeDelta)
{
	//Collision_Inspection();
	//플레이어에 cbaseobj 넣자~


}

void cMap_Active::Scene_Render1()
{
	//LOG_MGR->AddLog("%d", BoundBoxColl.isRoom_6);
}

void cMap_Active::BoxActive(Unit * player)
{
	if (BoundBoxColl.isRoom_0)
		BoxCollision(player, box_Load->Get_V_Room0_BB());
	if (BoundBoxColl.isRoom_1)
		BoxCollision(player, box_Load->Get_V_Room1_BB());
	if (BoundBoxColl.isRoom_2)
		BoxCollision(player, box_Load->Get_V_Room2_BB());
	if (BoundBoxColl.isRoom_3)
		BoxCollision(player, box_Load->Get_V_Room3_BB());
	if (BoundBoxColl.isRoom_4)
		BoxCollision(player, box_Load->Get_V_Room4_BB());
	if (BoundBoxColl.isRoom_5)
		BoxCollision(player, box_Load->Get_V_Room5_BB());
	if (BoundBoxColl.isRoom_6)
		BoxCollision(player, box_Load->Get_V_Room6_BB());
	if (BoundBoxColl.isHallway_7)
		BoxCollision(player, box_Load->Get_V_Hallway7_BB());
	if (BoundBoxColl.isHallway_8)
		BoxCollision(player, box_Load->Get_V_Hallway8_BB());
}



vector<cBaseObject*> cMap_Active::BoxRayActive()
{
	if (BoundBoxColl.isRoom_0)
		return box_Load->Get_V_Room0_BB();
	else if (BoundBoxColl.isRoom_1)
		return box_Load->Get_V_Room1_BB();
	else if (BoundBoxColl.isRoom_2)
		return box_Load->Get_V_Room2_BB();
	else if (BoundBoxColl.isRoom_3)
		return box_Load->Get_V_Room3_BB();
	else if (BoundBoxColl.isRoom_4)
		return box_Load->Get_V_Room4_BB();
	else if (BoundBoxColl.isRoom_5)
		return box_Load->Get_V_Room5_BB();
	else if (BoundBoxColl.isRoom_6)
		return box_Load->Get_V_Room6_BB();
	else if (BoundBoxColl.isHallway_7)
		return box_Load->Get_V_Hallway7_BB();
	else if (BoundBoxColl.isHallway_8)
		return box_Load->Get_V_Hallway8_BB();
}



void cMap_Active::Collision_Inspection(Unit* player)
{
	////플레이어에 cbaseobj 넣자~
	BoundBoxColl.isRoom_0 = PHYSICS_MGR->IsOverlap(player->getTrans(), player->getCollisionBox(),
		box_Load->Get_V_Collision_Inspection()[0]->pTransform, &box_Load->Get_V_Collision_Inspection()[0]->BoundBox);
	BoundBoxColl.isRoom_1 = PHYSICS_MGR->IsOverlap(player->getTrans(), player->getCollisionBox(),
		box_Load->Get_V_Collision_Inspection()[1]->pTransform, &box_Load->Get_V_Collision_Inspection()[1]->BoundBox);
	BoundBoxColl.isRoom_2 = PHYSICS_MGR->IsOverlap(player->getTrans(), player->getCollisionBox(),
		box_Load->Get_V_Collision_Inspection()[2]->pTransform, &box_Load->Get_V_Collision_Inspection()[2]->BoundBox);
	BoundBoxColl.isRoom_3 = PHYSICS_MGR->IsOverlap(player->getTrans(), player->getCollisionBox(),
		box_Load->Get_V_Collision_Inspection()[3]->pTransform, &box_Load->Get_V_Collision_Inspection()[3]->BoundBox);
	BoundBoxColl.isRoom_4 = PHYSICS_MGR->IsOverlap(player->getTrans(), player->getCollisionBox(),
		box_Load->Get_V_Collision_Inspection()[4]->pTransform, &box_Load->Get_V_Collision_Inspection()[4]->BoundBox);
	BoundBoxColl.isRoom_5 = PHYSICS_MGR->IsOverlap(player->getTrans(), player->getCollisionBox(),
		box_Load->Get_V_Collision_Inspection()[5]->pTransform, &box_Load->Get_V_Collision_Inspection()[5]->BoundBox);
	BoundBoxColl.isRoom_6 = PHYSICS_MGR->IsOverlap(player->getTrans(), player->getCollisionBox(),
		box_Load->Get_V_Collision_Inspection()[6]->pTransform, &box_Load->Get_V_Collision_Inspection()[6]->BoundBox);
	BoundBoxColl.isHallway_7 = PHYSICS_MGR->IsOverlap(player->getTrans(), player->getCollisionBox(),
		box_Load->Get_V_Collision_Inspection()[7]->pTransform, &box_Load->Get_V_Collision_Inspection()[7]->BoundBox);
	BoundBoxColl.isHallway_8 = PHYSICS_MGR->IsOverlap(player->getTrans(), player->getCollisionBox(),
		box_Load->Get_V_Collision_Inspection()[8]->pTransform, &box_Load->Get_V_Collision_Inspection()[8]->BoundBox);
}

void cMap_Active::BoxCollision(Unit* player,std::vector<cBaseObject*> boundBox)
{
	/*for (int i = 0; i < boundBox.size(); i++)
	{
		bool isteat = PHYSICS_MGR->IsBlocking(playerBoundBox,
			boundBox[i], 1.0f);
	}*/
	for (int i = 0; i < boundBox.size(); i++)
	{
		bool isteat = PHYSICS_MGR->IsBlocking(player->getTrans(),player->getCollisionBox(),
			boundBox[i]->pTransform, &boundBox[i]->BoundBox ,1.0f);
	}
}


