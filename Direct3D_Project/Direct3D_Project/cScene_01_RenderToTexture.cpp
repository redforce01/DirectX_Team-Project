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
	//원래 있던 매쉬에서 다음과 같은 행렬로 보정시켜 
	//모든 정보를 틀어버린다.
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, 0.01f, 0.01f, 0.01f);
	D3DXMATRIXA16 matRotate;
	D3DXMatrixRotationY(&matRotate, 180.0f * ONE_RAD);
	D3DXMATRIXA16 matCorrection = matScale * matRotate;

	//원본 메쉬 로딩
	cXMesh_Static* mesh =
		RESOURCE_STATICXMESH->GetResource("../../Resources/Meshes/Fighter/Fighter.X", &matCorrection);

	//라이트 푸쉬
	cLight_Direction* pLight1 = new cLight_Direction();
	pLight1->Color = D3DXCOLOR(1, 1, 1, 1);
	pLight1->Intensity = 1.0f;

	this->lights.push_back(pLight1);

	//오브젝트 생성
	for (int i = 0; i < OBJECT_NUM; i++)
	{
		for (int j = 0; j < OBJECT_NUM; j++)
		{
			cBaseObject* pNewObject = new cBaseObject();
			pNewObject->SetMesh(mesh);
			pNewObject->SetActive(true);

			//위치 셋팅
			pNewObject->pTransform->SetWorldPosition(
				i * 10, 0.0f, j * 20);


			//랱더 오브젝트 푸쉬
			this->renderObjects.push_back(pNewObject);
		}
	}


	//추가카메라..
	pExtraCam = new cCamera();
	pExtraCam->camFar = 100.0f;

	int width = 512;
	int height = 512;

	//엑스트라Cam 종횡비 
	pExtraCam->aspect = (float)width / (float)height;

	//RenderTarget 빈 Texture 만들기
	Device->CreateTexture(
		width,					//Texture 가로 해상도 
		height,					//Texture 세로 해상도
		1,							//밉맵체인 레벨
		D3DUSAGE_RENDERTARGET,		//RenderTarget 용 Texture이다
		D3DFMT_A8R8G8B8,			//TEXTURE 포맷 RenderTexture 용은D3DFMT_A8R8G8B8 로하자
		D3DPOOL_DEFAULT,			//RenderTarget 용 Texture 는 Pool 을 Default
		&pRenderTexture,			//생성된 Texture 받아올 포인터
		NULL
		);

	//Render 할 Surface 
	Device->CreateDepthStencilSurface(
		width,					//Texture 가로 해상도 
		height,					//Texture 세로 해상도
		D3DFMT_D24S8,				//Deapth 는 24 비트 Stencil 은 8 비트	
		D3DMULTISAMPLE_NONE,		//멀티 샘플링 안티알리아싱은 존재하지 않는다, 
		0,							//멀티 샘플링 퀄리티는 0
		TRUE,						//버퍼 교체시 이전 퍼버내용을 유지하지 않니? ( TRUE 로 쓰면 버퍼교체 될때 이전에 써진 버퍼내용을 기억하지 않는다 )
		&pRenderSurface,			//얻어올 포인터...
		NULL);

	//.랜더링 표면생성(해당 디바이스의)

	//	D3DXCreateRenderToSurface(디바이스, 가로크기, 세로크기,
	//	포맷형식, FALSE, D3DFMT_UNKNOWN,
	//	&g_RenderToSuface);

	//※ 주의 : 가로, 세로 크기와 포멧형식은 사용할 텍스쳐나 표면의 크기, 포맷형식과 반드시 일치해야 한다.그렇지 않으면 사용할 수 없다.


	return S_OK;
}

void cScene_01_RenderToTexture::Scene_Release()
{
	//라이트 해재
	for (int i = 0; i < this->lights.size(); i++)
		SAFE_DELETE(this->lights[i]);

	//오브젝트 해재
	for (int i = 0; i < this->renderObjects.size(); i++)
		SAFE_DELETE(this->renderObjects[i]);

	//추가카메라 해제
	SAFE_DELETE(this->pExtraCam);
}

void cScene_01_RenderToTexture::Scene_Update(float timeDelta)
{
	//추가 카메라 컨트롤
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

		//프러스텀 안에 있니?
		if (this->pMainCamera->Frustum.IsInFrustum(this->renderObjects[i]))
		{
			this->cullObjects.push_back(this->renderObjects[i]);
		}
	}
}

void cScene_01_RenderToTexture::Scene_Render1()
{
	//화면에 랜더링 되는 대신 Texture에 랜더링한다.

	//현 디바이스의 Target 버퍼의표면과 DepthStencil 버퍼의 표면정보를 기억
	LPDIRECT3DSURFACE9		pDeviceTargetSurface;
	LPDIRECT3DSURFACE9		pDeviceDepthAndStencilSurface;
	Device->GetRenderTarget(0, &pDeviceTargetSurface);
	Device->GetDepthStencilSurface(&pDeviceDepthAndStencilSurface);

	//RenderTexture 의 Surface 를 얻는다.
	LPDIRECT3DSURFACE9 texSurface = NULL;
	if (SUCCEEDED(this->pRenderTexture->GetSurfaceLevel(0, &texSurface)))
	{
		//Texture 표면을 Device 의 Target 버퍼로 셋팅한다.
		Device->SetRenderTarget(0, texSurface);

		//셋팅된 Surface 정보는 바로 날려주는 예의를 갖추자...
		SAFE_RELEASE(texSurface);
	}


	//Depth 버퍼와 Stencil 버퍼의 Surface 로 m_pRenderSurface 셋팅
	Device->SetDepthStencilSurface(pRenderSurface);

	//디바이스 버퍼 클리어 ( 사실 위에서 Setting 된 Textuer 랑 Surface 가 클리어 된다  )
	Device->Clear(0, NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		0xff00ff00,
		1.0f,
		0);

	cXMesh_Static::SetCamera(this->pExtraCam);
	cXMesh_Static::SetLighting(&this->lights);

	for (int i = 0; i < this->cullObjects.size(); i++)
		this->cullObjects[i]->Render();

	//프러텀을 그려보장
	//this->pMainCamera->Frustum.RenderGizmo();
	//Extra 카메러 프러스텀
	//this->pExtraCam->Frustum.RenderGizmo();

	//Render Texture 에 그릴 작업이 끝났으면 다시 원상복귀하는 센스....
	Device->SetRenderTarget(0, pDeviceTargetSurface);
	Device->SetDepthStencilSurface(pDeviceDepthAndStencilSurface);

	//셋팅된 Surface 정보는 바로 날려주는 예의를 갖추자...
	SAFE_RELEASE(pDeviceTargetSurface);
	SAFE_RELEASE(pDeviceDepthAndStencilSurface);



	cXMesh_Static::SetCamera(this->pMainCamera);
	cXMesh_Static::SetLighting(&this->lights);

	for (int i = 0; i < this->cullObjects.size(); i++)
		this->cullObjects[i]->Render();

	//프러텀을 그려보장
	this->pMainCamera->Frustum.RenderGizmo();


	//Extra 카메러 프러스텀
	this->pExtraCam->Frustum.RenderGizmo();
}

void cScene_01_RenderToTexture::Scene_RenderSprite()
{
	RECT rcSouc = { 0, 0, 512, 512 };
	RECT rcDest = { 0, 0, 512, 512 };
	SPRITE_MGR->DrawArea(pRenderTexture, &rcSouc, &rcDest);
}