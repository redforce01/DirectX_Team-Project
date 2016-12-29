#pragma once


class cBaseObject;
class cMap_Active;

class cMap_Bbox_Load
{
private:
	std::vector<cBaseObject*> m_vAllBox;

	std::vector<cBaseObject*> v_Room_0_boundBox;
	std::vector<cBaseObject*>::iterator vI_Room_0_boundBox;
	std::vector<cBaseObject*> v_Room_1_boundBox;
	std::vector<cBaseObject*>::iterator vI_Room_1_boundBox;
	std::vector<cBaseObject*> v_Room_2_boundBox;
	std::vector<cBaseObject*>::iterator vI_Room_2_boundBox;
	std::vector<cBaseObject*> v_Room_3_boundBox;
	std::vector<cBaseObject*>::iterator vI_Room_3_boundBox;
	std::vector<cBaseObject*> v_Room_4_boundBox;
	std::vector<cBaseObject*>::iterator vI_Room_4_boundBox;
	std::vector<cBaseObject*> v_Room_5_boundBox;
	std::vector<cBaseObject*>::iterator vI_Room_5_boundBox;
	std::vector<cBaseObject*> v_Room_6_boundBox;
	std::vector<cBaseObject*>::iterator vI_Room_6_boundBox;
	std::vector<cBaseObject*> v_Hallway_7_boundBox;
	std::vector<cBaseObject*>::iterator vI_Hallway_7_boundBox;
	std::vector<cBaseObject*> v_Hallway_8_boundBox;
	std::vector<cBaseObject*>::iterator vI_Hallway_8_boundBox;

	//각방의 충돌확인을 활성화 시킬수있는 박스들~
	std::vector<cBaseObject*> v_Collision_Inspection;
	std::vector<cBaseObject*>::iterator vI_Collision_Inspection;
private:
	cMap_Active* map_active;


public:
	cMap_Bbox_Load();
	~cMap_Bbox_Load();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);
	virtual void Scene_Render1();


	void InitLoadBound(string str, std::vector<cBaseObject*>* v_base);
	void RenderBoundBox(std::vector<cBaseObject*> vectorSize);

	std::vector<cBaseObject*> Get_V_Room0_BB() { return v_Room_0_boundBox; }
	std::vector<cBaseObject*>::iterator Get_VI_Room1_BB() { return  vI_Room_0_boundBox; }

	std::vector<cBaseObject*> Get_V_Room1_BB() { return v_Room_1_boundBox; }
	std::vector<cBaseObject*>::iterator Get_VI_Room2_BB() { return  vI_Room_1_boundBox; }

	std::vector<cBaseObject*> Get_V_Room2_BB() { return v_Room_2_boundBox; }
	std::vector<cBaseObject*>::iterator Get_VI_Room3_BB() { return  vI_Room_2_boundBox; }

	std::vector<cBaseObject*> Get_V_Room3_BB() { return v_Room_3_boundBox; }
	std::vector<cBaseObject*>::iterator Get_VI_Room4_BB() { return  vI_Room_3_boundBox; }

	std::vector<cBaseObject*> Get_V_Room4_BB() { return v_Room_4_boundBox; }
	std::vector<cBaseObject*>::iterator Get_VI_Room5_BB() { return  vI_Room_4_boundBox; }

	std::vector<cBaseObject*> Get_V_Room5_BB() { return v_Room_5_boundBox; }
	std::vector<cBaseObject*>::iterator Get_VI_Room6_BB() { return  vI_Room_5_boundBox; }

	std::vector<cBaseObject*> Get_V_Room6_BB() { return v_Room_6_boundBox; }
	std::vector<cBaseObject*>::iterator Get_VI_Room7_BB() { return  vI_Room_6_boundBox; }

	std::vector<cBaseObject*> Get_V_Hallway7_BB() { return v_Hallway_7_boundBox; }
	std::vector<cBaseObject*>::iterator Get_VI_Hallway1_BB() { return  vI_Hallway_7_boundBox; }

	std::vector<cBaseObject*> Get_V_Hallway8_BB() { return v_Hallway_8_boundBox; }
	std::vector<cBaseObject*>::iterator Get_VI_Hallway2_BB() { return  vI_Hallway_8_boundBox; }

	std::vector<cBaseObject*> Get_V_Collision_Inspection() { return v_Collision_Inspection; }
	std::vector<cBaseObject*>::iterator Get_VI_Collision_Inspection() { return vI_Collision_Inspection; }

	std::vector<cBaseObject*> GetAllBox() { return m_vAllBox; }

	void SetLink_MapActive(cMap_Active* ma) { map_active = ma; }

};

