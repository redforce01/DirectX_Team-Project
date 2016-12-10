#include "stdafx.h"
#include "cMap_Bbox_Load.h"

#include <fstream>
#include "cBaseObject.h"
#include "cCamera.h"

#include "cMap_Active.h"

cMap_Bbox_Load::cMap_Bbox_Load()
{
	InitLoadBound("OutLast_Room_0.txt", &v_Room_0_boundBox);
	InitLoadBound("OutLast_Room_1.txt", &v_Room_1_boundBox);
	InitLoadBound("OutLast_Room_2.txt", &v_Room_2_boundBox);
	InitLoadBound("OutLast_Room_3.txt", &v_Room_3_boundBox);
	InitLoadBound("OutLast_Room_4.txt", &v_Room_4_boundBox);
	InitLoadBound("OutLast_Room_5.txt", &v_Room_5_boundBox);
	InitLoadBound("OutLast_Room_6.txt", &v_Room_6_boundBox);
	InitLoadBound("OutLast_Hallway_7.txt", &v_Hallway_7_boundBox);
	InitLoadBound("OutLast_Hallway_8.txt", &v_Hallway_8_boundBox);
	InitLoadBound("OutLast_Active.txt", &v_Collision_Inspection);
	
}


cMap_Bbox_Load::~cMap_Bbox_Load()
{
}

HRESULT cMap_Bbox_Load::Scene_Init()
{
	return S_OK;
}

void cMap_Bbox_Load::Scene_Release()
{
	for (int i = 0; i < this->boundObjects.size(); i++)
	{
		SAFE_DELETE(this->boundObjects[i]);
	}
}

void cMap_Bbox_Load::Scene_Update(float timeDelta)
{
	//this->bHit = PHYSICS_MGR->IsBlocking(&this->objects, &this->object2, 0.5f);
}

void cMap_Bbox_Load::Scene_Render1()
{
	if(map_active->Get_Is_Active()->isRoom_0)
	RenderBoundBox(v_Room_0_boundBox);
	if (map_active->Get_Is_Active()->isRoom_1)
	RenderBoundBox(v_Room_1_boundBox);
	if (map_active->Get_Is_Active()->isRoom_2)
	RenderBoundBox(v_Room_2_boundBox);
	if (map_active->Get_Is_Active()->isRoom_3)
	RenderBoundBox(v_Room_3_boundBox);
	if (map_active->Get_Is_Active()->isRoom_4)
	RenderBoundBox(v_Room_4_boundBox);
	if (map_active->Get_Is_Active()->isRoom_5)
	RenderBoundBox(v_Room_5_boundBox);
	if (map_active->Get_Is_Active()->isRoom_6)
	RenderBoundBox(v_Room_6_boundBox);
	if (map_active->Get_Is_Active()->isHallway_7)
		RenderBoundBox(v_Hallway_7_boundBox);
	if (map_active->Get_Is_Active()->isHallway_8)
		RenderBoundBox(v_Hallway_8_boundBox);
	//RenderBoundBox(v_Collision_Inspection);
}

void cMap_Bbox_Load::RenderBoundBox(std::vector<cBaseObject*> vectorSize)
{
	for (int i = 0; i < vectorSize.size(); i++)
	{
		vectorSize[i]->Render();
	}
}

void cMap_Bbox_Load::InitLoadBound(string str, std::vector<cBaseObject*>* v_base)
{
	fstream file;
	file.open(str, fstream::in); //fstream::in -> ���� �б� ���

	std::vector<std::string> strLine;

	//���� ������ �д´�.
	while (file.eof() == false) //���� ���� ������ trur �� ��ȯ
	{
		std::string line;
		file >> line;			//file ���� ���ڿ��� �о� line�� ����
		strLine.push_back(line);
	}
	file.close(); //�پ� ���� ��Ʈ�� �ݴ´�.

				  //�о�� ���� ��� ����
	for (int i = 0; i < strLine.size(); i++)
	{
		if (strLine[i].size() == 0)
			continue;

		//���� ������ ���⿡ ���Եȴ�.
		char cStr[2048];
		strcpy(cStr, strLine[i].c_str());

		char* pc;

		//[3.6819,2.71903,13.0518]
		//[-0.621861,-0.277318,-0.272702,0.679719]
		//[1,1,1][0,0,0][7.13382,5.48357,6.06699]

		//��ġ
		D3DXVECTOR3 pos;
		pc = strtok(cStr, "[],"); //ó��¥������ ¥�� ���ڿ��� �ּҸ� �ִ´�.
		pos.x = atof(pc);
		pc = strtok(NULL, "[],"); //�� �������ʹ� NULL(©���� ���ڿ� ��ġ)
		pos.y = atof(pc);
		pc = strtok(NULL, "[],");
		pos.z = atof(pc);


		//�����
		D3DXQUATERNION quat;
		pc = strtok(NULL, "[],");
		quat.x = atof(pc);
		pc = strtok(NULL, "[],");
		quat.y = atof(pc);
		pc = strtok(NULL, "[],");
		quat.z = atof(pc);
		pc = strtok(NULL, "[],");
		quat.w = atof(pc);

		//������
		D3DXVECTOR3 scale;
		pc = strtok(NULL, "[],");
		scale.x = atof(pc);
		pc = strtok(NULL, "[],");
		scale.y = atof(pc);
		pc = strtok(NULL, "[],");
		scale.z = atof(pc);

		//�ٿ�� ���ü���
		D3DXVECTOR3 localCenter;
		pc = strtok(NULL, "[],");
		localCenter.x = atof(pc);
		pc = strtok(NULL, "[],");
		localCenter.y = atof(pc);
		pc = strtok(NULL, "[],");
		localCenter.z = atof(pc);

		//�ٿ�� ��������
		D3DXVECTOR3 localHalf;
		pc = strtok(NULL, "[],");
		localHalf.x = atof(pc);
		pc = strtok(NULL, "[],");
		localHalf.y = atof(pc);
		pc = strtok(NULL, "[],");
		localHalf.z = atof(pc);

		//���� ������ ������Ʈ ����
		cBaseObject* pNewBound = new cBaseObject;
		pNewBound->SetActive(true);
		pNewBound->pTransform->SetWorldPosition(pos);
		pNewBound->pTransform->SetRotateWorld(quat);
		pNewBound->pTransform->SetScale(scale);
		pNewBound->BoundBox.SetBound(&localCenter, &localHalf);

		//Ǫ��
		v_base->push_back(pNewBound);
	}
}

