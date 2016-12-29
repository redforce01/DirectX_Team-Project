#include "stdafx.h"
#include "cScene_01_RenderToTexture.h"
#include "cLight_Direction.h"
#include "cXMesh_Static.h"
#include "cBaseObject.h"
#include "cTransform.h"
#include "cCamera.h"

cScene_01_RenderToTexture::cScene_01_RenderToTexture()
{
}


cScene_01_RenderToTexture::~cScene_01_RenderToTexture()
{
}


HRESULT cScene_01_RenderToTexture::Scene_Init()
{
	//���� �ִ� �Ž����� ������ ���� ��ķ� �������� 
	//��� ������ Ʋ�������.
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, 0.01f, 0.01f, 0.01f);
	D3DXMATRIXA16 matRotate;
	D3DXMatrixRotationY(&matRotate, 180.0f * ONE_RAD);
	D3DXMATRIXA16 matCorrection = matScale * matRotate;

	//���� �޽� �ε�
	cXMesh_Static* mesh =
		RESOURCE_STATICXMESH->GetResource("../../Resources/Meshes/Fighter/Fighter.X", &matCorrection);

	//����Ʈ Ǫ��
	cLight_Direction* pLight1 = new cLight_Direction();
	pLight1->Color = D3DXCOLOR(1, 1, 1, 1);
	pLight1->Intensity = 1.0f;

	this->lights.push_back(pLight1);

	//������Ʈ ����
	for (int i = 0; i < OBJECT_NUM; i++)
	{
		for (int j = 0; j < OBJECT_NUM; j++)
		{
			cBaseObject* pNewObject = new cBaseObject();
			pNewObject->SetMesh(mesh);
			pNewObject->SetActive(true);

			//��ġ ����
			pNewObject->pTransform->SetWorldPosition(
				i * 10, 0.0f, j * 20);


			//�M�� ������Ʈ Ǫ��
			this->renderObjects.push_back(pNewObject);
		}
	}


	//�߰�ī�޶�..
	pExtraCam = new cCamera();
	pExtraCam->camFar = 100.0f;

	int width = 512;
	int height = 512;

	//����Ʈ��Cam ��Ⱦ�� 
	pExtraCam->aspect = (float)width / (float)height;

	//RenderTarget �� Texture �����
	Device->CreateTexture(
		width,					//Texture ���� �ػ� 
		height,					//Texture ���� �ػ�
		1,							//�Ӹ�ü�� ����
		D3DUSAGE_RENDERTARGET,		//RenderTarget �� Texture�̴�
		D3DFMT_A8R8G8B8,			//TEXTURE ���� RenderTexture ����D3DFMT_A8R8G8B8 ������
		D3DPOOL_DEFAULT,			//RenderTarget �� Texture �� Pool �� Default
		&pRenderTexture,			//������ Texture �޾ƿ� ������
		NULL
		);

	//Render �� Surface 
	Device->CreateDepthStencilSurface(
		width,					//Texture ���� �ػ� 
		height,					//Texture ���� �ػ�
		D3DFMT_D24S8,				//Deapth �� 24 ��Ʈ Stencil �� 8 ��Ʈ	
		D3DMULTISAMPLE_NONE,		//��Ƽ ���ø� ��Ƽ�˸��ƽ��� �������� �ʴ´�, 
		0,							//��Ƽ ���ø� ����Ƽ�� 0
		TRUE,						//���� ��ü�� ���� �۹������� �������� �ʴ�? ( TRUE �� ���� ���۱�ü �ɶ� ������ ���� ���۳����� ������� �ʴ´� )
		&pRenderSurface,			//���� ������...
		NULL);

	//.������ ǥ�����(�ش� ����̽���)

	//	D3DXCreateRenderToSurface(����̽�, ����ũ��, ����ũ��,
	//	��������, FALSE, D3DFMT_UNKNOWN,
	//	&g_RenderToSuface);

	//�� ���� : ����, ���� ũ��� ���������� ����� �ؽ��ĳ� ǥ���� ũ��, �������İ� �ݵ�� ��ġ�ؾ� �Ѵ�.�׷��� ������ ����� �� ����.


	return S_OK;
}

void cScene_01_RenderToTexture::Scene_Release()
{
	//����Ʈ ����
	for (int i = 0; i < this->lights.size(); i++)
		SAFE_DELETE(this->lights[i]);

	//������Ʈ ����
	for (int i = 0; i < this->renderObjects.size(); i++)
		SAFE_DELETE(this->renderObjects[i]);

	//�߰�ī�޶� ����
	SAFE_DELETE(this->pExtraCam);
}

void cScene_01_RenderToTexture::Scene_Update(float timeDelta)
{
	//�߰� ī�޶� ��Ʈ��
	this->pExtraCam->DefaultControl2(timeDelta);
	this->pExtraCam->UpdateMatrix();
	this->pExtraCam->UpdateFrustum();

	if (KEY_MGR->IsOnceDown(VK_RETURN))
	{
		this->pMainCamera->UpdateFrustum();
	}

	this->cullObjects.clear();

	for (int i = 0; i < this->renderObjects.size(); i++)
	{
		this->renderObjects[i]->Update(timeDelta);

		//�������� �ȿ� �ִ�?
		if (this->pMainCamera->Frustum.IsInFrustum(this->renderObjects[i]))
		{
			this->cullObjects.push_back(this->renderObjects[i]);
		}
	}
}

void cScene_01_RenderToTexture::Scene_Render1()
{
	//ȭ�鿡 ������ �Ǵ� ��� Texture�� �������Ѵ�.

	//�� ����̽��� Target ������ǥ��� DepthStencil ������ ǥ�������� ���
	LPDIRECT3DSURFACE9		pDeviceTargetSurface;
	LPDIRECT3DSURFACE9		pDeviceDepthAndStencilSurface;
	Device->GetRenderTarget(0, &pDeviceTargetSurface);
	Device->GetDepthStencilSurface(&pDeviceDepthAndStencilSurface);

	//RenderTexture �� Surface �� ��´�.
	LPDIRECT3DSURFACE9 texSurface = NULL;
	if (SUCCEEDED(this->pRenderTexture->GetSurfaceLevel(0, &texSurface)))
	{
		//Texture ǥ���� Device �� Target ���۷� �����Ѵ�.
		Device->SetRenderTarget(0, texSurface);

		//���õ� Surface ������ �ٷ� �����ִ� ���Ǹ� ������...
		SAFE_RELEASE(texSurface);
	}


	//Depth ���ۿ� Stencil ������ Surface �� m_pRenderSurface ����
	Device->SetDepthStencilSurface(pRenderSurface);

	//����̽� ���� Ŭ���� ( ��� ������ Setting �� Textuer �� Surface �� Ŭ���� �ȴ�  )
	Device->Clear(0, NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		0xff00ff00,
		1.0f,
		0);

	cXMesh_Static::SetCamera(this->pExtraCam);
	cXMesh_Static::SetLighting(&this->lights);

	for (int i = 0; i < this->cullObjects.size(); i++)
		this->cullObjects[i]->Render();

	//�������� �׷�����
	//this->pMainCamera->Frustum.RenderGizmo();
	//Extra ī�޷� ��������
	//this->pExtraCam->Frustum.RenderGizmo();

	//Render Texture �� �׸� �۾��� �������� �ٽ� ���󺹱��ϴ� ����....
	Device->SetRenderTarget(0, pDeviceTargetSurface);
	Device->SetDepthStencilSurface(pDeviceDepthAndStencilSurface);

	//���õ� Surface ������ �ٷ� �����ִ� ���Ǹ� ������...
	SAFE_RELEASE(pDeviceTargetSurface);
	SAFE_RELEASE(pDeviceDepthAndStencilSurface);



	cXMesh_Static::SetCamera(this->pMainCamera);
	cXMesh_Static::SetLighting(&this->lights);

	for (int i = 0; i < this->cullObjects.size(); i++)
		this->cullObjects[i]->Render();

	//�������� �׷�����
	this->pMainCamera->Frustum.RenderGizmo();


	//Extra ī�޷� ��������
	this->pExtraCam->Frustum.RenderGizmo();
}

void cScene_01_RenderToTexture::Scene_RenderSprite()
{
	RECT rcSouc = { 0, 0, 512, 512 };
	RECT rcDest = { 0, 0, 512, 512 };
	SPRITE_MGR->DrawArea(pRenderTexture, &rcSouc, &rcDest);
}