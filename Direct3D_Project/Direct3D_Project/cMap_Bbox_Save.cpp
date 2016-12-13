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

		//현제 있는 놈들 레이체크
		for (int i = 0; i < this->boundObj.size(); i++)
		{
			D3DXVECTOR3 hitPos;
			if (PHYSICS_MGR->IsRayHitBound(
				&ray,
				&this->boundObj[i]->BoundBox,
				this->boundObj[i]->pTransform,
				&hitPos,
				NULL)) {

				//충돌된 놈이라면...
				hitBounds.push_back(this->boundObj[i]);

				//카메라로부터의 거리 제곱도 푸쉬
				hitdistances.push_back(D3DXVec3LengthSq(
					&(hitPos - pMainCamera->GetWorldPosition())));

			}
		}

		//히트 된 놈이 있다면...
		if (hitBounds.size() > 0)
		{
			//일단 첫번째 
			cSetBoundObject* pTarget = hitBounds[0];
			float nearest = hitdistances[0];

			for (int i = 1; i < hitBounds.size(); i++)
			{
				//갱신
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
	//선택된거 삭제.....
	if (KEY_MGR->IsOnceDown(VK_BACK))
	{
		if (this->selectBound != NULL)
		{
			//벡터에서 날리고
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
	file.open("OutLast_Hallway_8.txt", fstream::out); //fstream::out 파일 쓰기 모드

	for (int i = 0; i < this->boundObj.size(); i++)
	{
		cSetBoundObject* pBounds = this->boundObj[i];

		//위치 값 얻는다.
		D3DXVECTOR3 pos = pBounds->pTransform->GetWorldPosition();

		//사원수 얻는다.
		D3DXQUATERNION quat = pBounds->pTransform->GetWorldRotateQuaternion();

		//스케일 얻는다.
		D3DXVECTOR3 scale = pBounds->pTransform->GetScale();

		//바운드 정보
		D3DXVECTOR3 boundCenter = pBounds->center;
		D3DXVECTOR3 boundHalf = pBounds->halfSize;


		file <<
			"[" << pos.x << "," << pos.y << "," << pos.z << "]" <<  //Pos 쓴다.
			"[" << quat.x << "," << quat.y << "," << quat.z << "," << quat.w << "]" <<		//사원수 쓴다
			"[" << scale.x << "," << scale.y << "," << scale.z << "]" <<	//스케일쓴다.
			"[" << boundCenter.x << "," << boundCenter.y << "," << boundCenter.z << "]" <<	//바운드 센터 쓴다.
			"[" << boundHalf.x << "," << boundHalf.y << "," << boundHalf.z << "]" <<
			endl;
	}
	file.close(); //다쓴파일 스트림을 클로즈

	LOG_MGR->AddLog("SaveComplete!!!");
}
