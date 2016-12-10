#include "stdafx.h"
#include "cMap_Active.h"
//전방선언
#include "cMap_Bbox_Load.h"
//#include "playerTest.h"


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
	//if (BoundBoxColl.isRoom_0)
	//	//플레이어에 cbaseobj 넣자~
	//BoxCollision(player->GetPlayer()->baseobj, box_Load->Get_V_Room0_BB());
	//if (BoundBoxColl.isRoom_1)
	//	BoxCollision(player->GetPlayer()->baseobj, box_Load->Get_V_Room1_BB());
	//if (BoundBoxColl.isRoom_2)
	//	BoxCollision(player->GetPlayer()->baseobj, box_Load->Get_V_Room2_BB());
	//if (BoundBoxColl.isRoom_3)
	//	BoxCollision(player->GetPlayer()->baseobj, box_Load->Get_V_Room3_BB());
	//if (BoundBoxColl.isRoom_4)
	//	BoxCollision(player->GetPlayer()->baseobj, box_Load->Get_V_Room4_BB());
	//if (BoundBoxColl.isRoom_5)
	//	BoxCollision(player->GetPlayer()->baseobj, box_Load->Get_V_Room5_BB());
	//if (BoundBoxColl.isRoom_6)
	//	BoxCollision(player->GetPlayer()->baseobj, box_Load->Get_V_Room6_BB());
	//if (BoundBoxColl.isHallway_7)
	//	BoxCollision(player->GetPlayer()->baseobj, box_Load->Get_V_Hallway7_BB());
	//if (BoundBoxColl.isHallway_8)
	//	BoxCollision(player->GetPlayer()->baseobj, box_Load->Get_V_Hallway8_BB());
}

void cMap_Active::Scene_Render1()
{
	LOG_MGR->AddLog("%d", BoundBoxColl.isRoom_6);
}

void cMap_Active::Collision_Inspection()
{
	////플레이어에 cbaseobj 넣자~
	//BoundBoxColl.isRoom_0 = PHYSICS_MGR->IsOverlap(player->GetPlayer()->baseobj,
	//	box_Load->Get_V_Collision_Inspection()[0]);
	//BoundBoxColl.isRoom_1 = PHYSICS_MGR->IsOverlap(player->GetPlayer()->baseobj,
	//	box_Load->Get_V_Collision_Inspection()[1]);
	//BoundBoxColl.isRoom_2 = PHYSICS_MGR->IsOverlap(player->GetPlayer()->baseobj,
	//	box_Load->Get_V_Collision_Inspection()[2]);
	//BoundBoxColl.isRoom_3 = PHYSICS_MGR->IsOverlap(player->GetPlayer()->baseobj,
	//	box_Load->Get_V_Collision_Inspection()[3]);
	//BoundBoxColl.isRoom_4 = PHYSICS_MGR->IsOverlap(player->GetPlayer()->baseobj,
	//	box_Load->Get_V_Collision_Inspection()[4]);
	//BoundBoxColl.isRoom_5 = PHYSICS_MGR->IsOverlap(player->GetPlayer()->baseobj,
	//	box_Load->Get_V_Collision_Inspection()[5]);
	//BoundBoxColl.isRoom_6 = PHYSICS_MGR->IsOverlap(player->GetPlayer()->baseobj,
	//	box_Load->Get_V_Collision_Inspection()[6]);
	//BoundBoxColl.isHallway_7 = PHYSICS_MGR->IsOverlap(player->GetPlayer()->baseobj,
	//	box_Load->Get_V_Collision_Inspection()[7]);
	//BoundBoxColl.isHallway_8 = PHYSICS_MGR->IsOverlap(player->GetPlayer()->baseobj,
	//	box_Load->Get_V_Collision_Inspection()[8]);
}

void cMap_Active::BoxCollision(cBaseObject* playerBoundBox,std::vector<cBaseObject*> boundBox)
{
	for (int i = 0; i < boundBox.size(); i++)
	{
		bool isteat = PHYSICS_MGR->IsBlocking(playerBoundBox,
			boundBox[i], 1.0f);
	}
}
