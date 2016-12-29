#include "stdafx.h"
#include "cBaseObject.h"
#include "cTransform.h"
#include "cSkinnedAnimation.h"

cBaseObject::cBaseObject()
	: bActive(false), pMesh(NULL), pSkinned(NULL)
{
	this->pTransform = new cTransform();
	IgnoreCreateShadow = false;
}


cBaseObject::~cBaseObject()
{
	SAFE_DELETE(this->pTransform);
	if (this->pSkinned != NULL)
	{
		this->pSkinned->Release();
		SAFE_DELETE(this->pSkinned);
	}
}

void cBaseObject::Update(float timeDelta)
{
	if (this->bActive)
	{
		this->BaseObjectUpdate(timeDelta);

		if (this->pSkinned != NULL)
			this->pSkinned->Update(timeDelta);
	}
	else
		this->BaseObjectNoActiveUpdate(timeDelta);
}

//Ȱ��ȭ ����
void cBaseObject::SetActive(bool bActive)
{
	//Ȱ��ȭ �Ǿ��ִ� ���¿��� ��Ȱ��ȭ ���ý�
	if (bActive == false && this->bActive)
	{
		this->bActive = false;
		this->BaseObjectDisable();
	}

	//��Ȱ��ȭ �Ǿ��ִ� ���¿��� Ȱ��ȭ ���ý�
	else if (bActive && this->bActive == false)
	{
		this->bActive = true;
		this->BaseObjectEnable();
	}
}

//���õ� �޽��� ���� �ٿ��ڽ��� ���� �Ѵ�.
void cBaseObject::ComputeBoundBox()
{
	if (this->pMesh != NULL)
	{
		//���õ� �޽��� cXMesh_Static ���...
		cXMesh_Static* pStaticMesh = dynamic_cast< cXMesh_Static* >(this->pMesh);
		if (pStaticMesh != NULL)
		{
			this->BoundBox.localCenter = pStaticMesh->Bound_Center;
			this->BoundBox.halfSize = pStaticMesh->Bound_HalfSize;
			this->BoundBox.localMinPos = pStaticMesh->Bound_Min;
			this->BoundBox.localMaxPos = pStaticMesh->Bound_Max;
			this->BoundBox.radius = pStaticMesh->Bound_Radius;
		}

		//���õ� �޽��� cXMesh_Skinned ���... ( �ӽ÷� �ٿ�� �ڽ� )
		else
		{
			this->BoundBox.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(10.0f, 10.0f, 10.0f));
		}
	}
}

void cBaseObject::ComputeBoundBox(float radius)
{
	if (this->pMesh != NULL)
	{
		//���õ� �޽��� cXMesh_Static ���...
		cXMesh_Static* pStaticMesh = dynamic_cast< cXMesh_Static* >(this->pMesh);
		if (pStaticMesh != NULL)
		{
			this->BoundBox.localCenter = pStaticMesh->Bound_Center;
			this->BoundBox.halfSize = pStaticMesh->Bound_HalfSize;
			this->BoundBox.localMinPos = pStaticMesh->Bound_Min;
			this->BoundBox.localMaxPos = pStaticMesh->Bound_Max;
			this->BoundBox.radius = radius;
		}

		//���õ� �޽��� cXMesh_Skinned ���... ( �ӽ÷� �ٿ�� �ڽ� )
		else
		{
			this->BoundBox.SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(10.0f, 10.0f, 10.0f));
		}
	}



}

void cBaseObject::BaseObjectRender()
{
	if (this->pMesh != NULL)
	{
		if (this->pSkinned)
			this->pSkinned->Render(this->pTransform);

		else
			this->pMesh->Render(this->pTransform);
	}
	//this->BoundBox.RenderGizmo(this->pTransform);
}

//������ �� �޽��� �����Ѵ�.
void  cBaseObject::SetMesh(cXMesh*	pMesh)
{
	this->pMesh = pMesh;
	cXMesh_Skinned* pSkin = dynamic_cast<cXMesh_Skinned*>(pMesh);
	this->ComputeBoundBox();

	//StaticMesh ���...
	if (pSkin == NULL)
	{
		if (this->pSkinned != NULL)
		{
			this->pSkinned->Release();
			SAFE_DELETE(this->pSkinned);
		}

		pSkinned = NULL;
	}


	//SkinnedMesh ���...
	else
	{
		this->pSkinned = new cSkinnedAnimation();
		this->pSkinned->Init(pSkin);
	}

}

void cBaseObject::ItemRender()
{
	if (this->pMesh != NULL)
	{
		this->pMesh->ItemRender(this->pTransform);
	}
	//this->BoundBox.RenderGizmo(this->pTransform);
}