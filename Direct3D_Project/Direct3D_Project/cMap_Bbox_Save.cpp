#include "stdafx.h"
#include "cMap_Bbox_Save.h"

#include <fstream>
#include "cCamera.h"
#include "cSetBoundObject.h"

cMap_Bbox_Save::cMap_Bbox_Save()
	:selectBound(NULL)
{
}

cMap_Bbox_Save::~cMap_Bbox_Save()
{
}

HRESULT cMap_Bbox_Save::Scene_Init()
{
	return S_OK;
}

void cMap_Bbox_Save::Scene_Release()
{
	for (int i = 0; i < this->boundObj.size(); i++)
	{
		SAFE_DELETE(this->boundObj[i]);
	}
}

void cMap_Bbox_Save::Scene_Update(float timeDelta)
{
	CreateBoundBox(timeDelta);
	//ChoiseBoundBox();
	DeleteBoundBox();
	if (KEY_MGR->IsStayDown(VK_LCONTROL) && KEY_MGR->IsOnceDown('Z'))
	{
		this->SaveBoundBox();
	}
}

void cMap_Bbox_Save::Scene_Render1()
{
	for (int i = 0; i < this->boundObj.size(); i++)
		this->boundObj[i]->Render();

	if (this->selectBound != NULL)
	{
		this->selectBound->pTransform->RenderGimozo();
	}
}

void cMap_Bbox_Save::CreateBoundBox(float timeDelta)
{
	if (KEY_MGR->IsStayDown(VK_LCONTROL) && KEY_MGR->IsOnceDown(VK_RETURN))
	{
		cSetBoundObject* pNewBound = new cSetBoundObject();
		pNewBound->SetActive(true);
		pNewBound->Update(timeDelta);
		this->boundObj.push_back(pNewBound);

		this->selectBound = pNewBound;
	}

	if (this->selectBound != NULL)
	{
		this->selectBound->Update(timeDelta);
	}
}

void cMap_Bbox_Save::ChoiseBoundBox(cCamera* pMainCamera)
{
	if (KEY_MGR->IsStayDown(VK_LCONTROL) && KEY_MGR->IsOnceDown(VK_LBUTTON))
	{
		Ray ray;
		POINT ptMouse = GetMousePos();
		D3DXVECTOR2 screenPos(ptMouse.x, ptMouse.y);
		pMainCamera->ComputeRay(&ray, &screenPos);

		vector<cSetBoundObject*> hitBounds;
		vector<float>	hitdistances;

		//���� �ִ� ��� ����üũ
		for (int i = 0; i < this->boundObj.size(); i++)
		{
			D3DXVECTOR3 hitPos;
			if (PHYSICS_MGR->IsRayHitBound(
				&ray,
				&this->boundObj[i]->BoundBox,
				this->boundObj[i]->pTransform,
				&hitPos,
				NULL)) {

				//�浹�� ���̶��...
				hitBounds.push_back(this->boundObj[i]);

				//ī�޶�κ����� �Ÿ� ������ Ǫ��
				hitdistances.push_back(D3DXVec3LengthSq(
					&(hitPos - pMainCamera->GetWorldPosition())));

			}
		}

		//��Ʈ �� ���� �ִٸ�...
		if (hitBounds.size() > 0)
		{
			//�ϴ� ù��° 
			cSetBoundObject* pTarget = hitBounds[0];
			float nearest = hitdistances[0];

			for (int i = 1; i < hitBounds.size(); i++)
			{
				//����
				if (nearest > hitdistances[i])
				{
					nearest = hitdistances[i];
					pTarget = hitBounds[i];
				}
			}
			this->selectBound = pTarget;
		}
	}
}

void cMap_Bbox_Save::DeleteBoundBox()
{
	//���õȰ� ����.....
	if (KEY_MGR->IsOnceDown(VK_BACK))
	{
		if (this->selectBound != NULL)
		{
			//���Ϳ��� ������
			for (vector<cSetBoundObject*>::iterator iter = this->boundObj.begin();
				iter != this->boundObj.end();
				++iter)
			{
				if (*iter == this->selectBound)
				{
					this->boundObj.erase(iter);
					break;
				}
			}
			delete this->selectBound;
			this->selectBound = NULL;
		}
	}
}

void cMap_Bbox_Save::SaveBoundBox()
{
	fstream file;
	file.open("OutLast_Hallway_8.txt", fstream::out); //fstream::out ���� ���� ���

	for (int i = 0; i < this->boundObj.size(); i++)
	{
		cSetBoundObject* pBounds = this->boundObj[i];

		//��ġ �� ��´�.
		D3DXVECTOR3 pos = pBounds->pTransform->GetWorldPosition();

		//����� ��´�.
		D3DXQUATERNION quat = pBounds->pTransform->GetWorldRotateQuaternion();

		//������ ��´�.
		D3DXVECTOR3 scale = pBounds->pTransform->GetScale();

		//�ٿ�� ����
		D3DXVECTOR3 boundCenter = pBounds->center;
		D3DXVECTOR3 boundHalf = pBounds->halfSize;


		file <<
			"[" << pos.x << "," << pos.y << "," << pos.z << "]" <<  //Pos ����.
			"[" << quat.x << "," << quat.y << "," << quat.z << "," << quat.w << "]" <<		//����� ����
			"[" << scale.x << "," << scale.y << "," << scale.z << "]" <<	//�����Ͼ���.
			"[" << boundCenter.x << "," << boundCenter.y << "," << boundCenter.z << "]" <<	//�ٿ�� ���� ����.
			"[" << boundHalf.x << "," << boundHalf.y << "," << boundHalf.z << "]" <<
			endl;
	}
	file.close(); //�پ����� ��Ʈ���� Ŭ����

	LOG_MGR->AddLog("SaveComplete!!!");
}
