#include "stdafx.h"
#include "cScene.h"
#include "cCamera.h"
#include "cLight_Direction.h"
#include "cTerrain.h"

cScene::cScene()
	: evironmentTexture(NULL), evironmemtSphereMesh(NULL)
{
	//환경 Effect 물어 놓는다.
	this->evironmentEffect =
		RESOURCE_FX->GetResource("../Resources/Shaders/EvironmentCUBE.fx");

	cCamera* DebuggingCamera = new FreeCamera;
	cCamera* MilesCamera = new PlayerCamera;

	vCamera.push_back(MilesCamera);
	vCamera.push_back(DebuggingCamera);

	viCamera = vCamera.begin();
	pMainCamera = vCamera[PLAYER];

	//기본 광원 생성
	this->pSceneBaseDirectionLight = new cLight_Direction();

	//방향성 광원에 카메라 를 생성한다.
	this->pDirectionLightCamera = new FreeCamera();

	//기본 값
	this->pSceneBaseDirectionLight->Color = D3DXCOLOR(1, 1, 1, 1);
	this->pSceneBaseDirectionLight->Intensity = 1.0f;

	//그림자 거리
	shadowDistance = 100.0f;


	//카메라의 투영방식을 바꾼다...
	this->pDirectionLightCamera->bOrtho = true;
	this->pDirectionLightCamera->camNear = 0.1f;
	this->pDirectionLightCamera->camFar = shadowDistance * 2.0f;
	this->pDirectionLightCamera->aspect = 1;
	this->pDirectionLightCamera->orthoSize = shadowDistance * 1.5f;	//투영크기는 그림자크기로...



																	//방향성광원 카메라의 RenderToTexture 준비
	this->pDirectionLightCamera->ReadyShadowTexture(4096);
}


cScene::~cScene()
{
	SAFE_RELEASE(evironmentTexture);
	SAFE_RELEASE(evironmemtSphereMesh);

	SAFE_DELETE(this->pMainCamera);

	SAFE_DELETE(this->pSceneBaseDirectionLight);
	SAFE_DELETE(this->pDirectionLightCamera);
}

HRESULT cScene::Init()
{
	//씬의 초기화 이루어진다.
	if (FAILED(this->Scene_Init()))
		return E_FAIL;

	//메인카메라 RenderToTexture 준비
	this->pMainCamera->ReadyRenderToTexture(WINSIZE_X, WINSIZE_Y);

	//0----1
	//|   /|
	//|  / |
	//| /  |
	//|/   |
	//3----2

	this->scenePlaneVertex[0].pos = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	this->scenePlaneVertex[1].pos = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	this->scenePlaneVertex[2].pos = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	this->scenePlaneVertex[3].pos = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);

	this->scenePlaneVertex[0].uv = D3DXVECTOR2(0.0f, 0.0f);
	this->scenePlaneVertex[1].uv = D3DXVECTOR2(1.0f, 0.0f);
	this->scenePlaneVertex[2].uv = D3DXVECTOR2(1.0f, 1.0f);
	this->scenePlaneVertex[3].uv = D3DXVECTOR2(0.0f, 1.0f);

	this->scenePlaneIndex[0] = 0;
	this->scenePlaneIndex[1] = 1;
	this->scenePlaneIndex[2] = 3;
	this->scenePlaneIndex[3] = 3;
	this->scenePlaneIndex[4] = 1;
	this->scenePlaneIndex[5] = 2;

	this->postEffect = RESOURCE_FX->GetResource("../Resources/Shaders/PostEffect.fx");

	return S_OK;
}

void cScene::Release()
{
	//씬의 해제가 일어난다.
	this->Scene_Release();
}

void cScene::Update(float timeDelta)
{
	//pMainCamera->DefaultControl(timeDelta);
	pMainCamera->CameraUpdate(timeDelta);
	pMainCamera->UpdateCamToDevice(Device);
	pMainCamera->UpdateFrustum();
	this->ChangeCameraMod();

	//메인카메라에 DirectionLight 를 방향을 유지한체 따라다니게....

	//광원 위치
	D3DXVECTOR3 camPos = pMainCamera->GetWorldPosition();		//메인카메라의 위치
	D3DXVECTOR3 camFront = pMainCamera->GetForward();			//메인카메라의 정면
	D3DXVECTOR3 lightDir = pSceneBaseDirectionLight->pTransform->GetForward();	//방향성 광원의 방향

	D3DXVECTOR3 lightPos = camPos +
		(camFront * (shadowDistance * 0.5f)) +
		(-lightDir * shadowDistance);

	this->pDirectionLightCamera->SetWorldPosition(lightPos.x, lightPos.y, lightPos.z);
	this->pDirectionLightCamera->LookDirection(lightDir);


	//씬의 업데이트가 일어난다.
	this->Scene_Update(timeDelta);
}

void cScene::Render()
{
	this->pMainCamera->RenderTextureBegin(0x00101010);

	//환경 랜더
	this->RenderEnvironment();

	//랜더된다.
	this->Scene_Render0();
	this->Scene_Render1();
	this->Scene_Render2();

#ifdef _DEBUG		//디버그 모드에서만 실행
	//디바이스 랜더링 종료 명령
	//월드 그리드
	GIZMO_MGR->WorldGrid(1, 10);
#endif

	this->pMainCamera->RenderTextureEnd();

	//Scene 랜더
	this->postEffect->SetTechnique("Base");

	this->postEffect->SetTexture("screenTex", this->pMainCamera->GetRenderTexture());

	float pixelU = 1.0f / WINSIZE_X;
	float pixelV = 1.0f / WINSIZE_Y;
	this->postEffect->SetFloat("pixelSizeU", pixelU);
	this->postEffect->SetFloat("pixelSizeV", pixelV);
	this->postEffect->SetFloat("pixelHalfSizeU", pixelU * 0.5f);
	this->postEffect->SetFloat("pixelHalfSizeV", pixelV * 0.5f);


	UINT iPass;
	this->postEffect->Begin(&iPass, 0);
	for (UINT i = 0; i < iPass; i++) {
		this->postEffect->BeginPass(i);

		Device->SetFVF(SCENE_VERTEX::FVF);
		Device->DrawIndexedPrimitiveUP(
			D3DPT_TRIANGLELIST,
			0,
			4,
			2,
			this->scenePlaneIndex,
			D3DFMT_INDEX16,
			this->scenePlaneVertex,
			sizeof(SCENE_VERTEX));


		this->postEffect->EndPass();
	}
	this->postEffect->End();


	//스플라이트 랜더
	SPRITE_MGR->BeginSpriteRender();
	this->Scene_RenderSprite();
	SPRITE_MGR->EndSpriteRender();
}

void cScene::SetEnvironment(std::string cubeFilePath)
{
	//기존의 환경맵이 셋팅되어있을지 몰라..
	SAFE_RELEASE(evironmentTexture);

	//환경맵 로딩
	D3DXCreateCubeTextureFromFile(
		Device,
		cubeFilePath.c_str(),
		&evironmentTexture);

	//환경구 로딩되어있지 않다면....
	if (this->evironmemtSphereMesh == NULL)
	{
		D3DXCreateSphere(Device,
			3.0f,
			20,
			20,
			&evironmemtSphereMesh, NULL);
	}
}

//환경구 랜더
void cScene::RenderEnvironment()
{
	//환경 맵 로딩된게 없다면 리턴해라.....
	if (this->evironmentTexture == NULL)
		return;

	//환경 Effect 셋팅
	this->evironmentEffect->SetTexture("MyCube_Tex", this->evironmentTexture);

	//WVP 매트릭스
	D3DXMATRIXA16 matWorld = this->pMainCamera->GetFinalMatrix();
	D3DXMATRIXA16 matViewProj = this->pMainCamera->GetViewProjectionMatrix();
	D3DXMATRIXA16 matWVP = matWorld * matViewProj;

	this->evironmentEffect->SetMatrix("matWVP", &matWVP);
	this->evironmentEffect->SetMatrix("matWorld", &matWorld);


	//그려라...
	UINT numPass;
	this->evironmentEffect->Begin(&numPass, 0);		//셰이더로 그린다는 것을 알리고 pass 수를 얻는다.

	for (UINT i = 0; i < numPass; i++)	//공정 수대로 돈다.
	{
		this->evironmentEffect->BeginPass(i);			//i번째 공정시작

		this->evironmemtSphereMesh->DrawSubset(0);

		this->evironmentEffect->EndPass();				//i번째 공정끝
	}
	this->evironmentEffect->End();						//셰이더로 그리기 끝


}



void cScene::ReadyShadowMap(std::vector<cBaseObject*>* renderObjects, cTerrain* pTerrain)
{
	//방향성광원에 붙은 카메라의 Frustum 업데이트
	this->pDirectionLightCamera->UpdateMatrix();
	this->pDirectionLightCamera->UpdateFrustum();


	//다이렉션라이팅 카메라에 들어오는 애들만 그린다...
	static std::vector<cBaseObject*>		shadowCullObject;
	shadowCullObject.clear();

	for (int i = 0; i < renderObjects->size(); i++) {

		//프러스텀 안에 있니?
		if (this->pDirectionLightCamera->Frustum.IsInFrustum((*renderObjects)[i]))
		{
			shadowCullObject.push_back((*renderObjects)[i]);
		}
	}


	//쉐도우 맵 그린다.
	this->pDirectionLightCamera->RenderTextureBegin(0xffffffff);

	cXMesh_Static::SetCamera(this->pDirectionLightCamera);
	cXMesh_Static::SetTechniqueName("CreateShadow");

	cXMesh_Skinned::SetCamera(this->pDirectionLightCamera);
	cXMesh_Skinned::SetTechniqueName("CreateShadow");



	for (int i = 0; i < shadowCullObject.size(); i++) {
		if (shadowCullObject[i]->IgnoreCreateShadow == false)
			shadowCullObject[i]->Render();
	}

	//만약 Terrain 도 쉐도우 맵을 그려야한다면...
	if (pTerrain != NULL)
	{
		pTerrain->RenderShadow(this->pDirectionLightCamera);
	}


	this->pDirectionLightCamera->RenderTextureEnd();


	//만약 Terrain 도 쉐도우 맵을 셋팅한다면...
	if (pTerrain != NULL)
	{
		pTerrain->m_pTerrainEffect->SetTexture("Shadow_Tex",
			this->pDirectionLightCamera->GetRenderTexture());

		pTerrain->m_pTerrainEffect->SetMatrix("matLightViewProjection",
			&this->pDirectionLightCamera->GetViewProjectionMatrix());
	}



	//쉐도우 Texture
	cXMesh_Static::sStaticMeshEffect->SetTexture("Shadow_Tex",
		this->pDirectionLightCamera->GetRenderTexture());

	//쉐도우 행렬
	cXMesh_Static::sStaticMeshEffect->SetMatrix("matLightViewProjection",
		&this->pDirectionLightCamera->GetViewProjectionMatrix());


	//쉐도우 Texture
	cXMesh_Skinned::sSkinnedMeshEffect->SetTexture("Shadow_Tex",
		this->pDirectionLightCamera->GetRenderTexture());

	//쉐도우 행렬
	cXMesh_Skinned::sSkinnedMeshEffect->SetMatrix("matLightViewProjection",
		&this->pDirectionLightCamera->GetViewProjectionMatrix());
}



//메인 카메라의 RenderToTexture 만 업데이트한다.
void cScene::RenderToMainCamTexture()
{
	this->pMainCamera->RenderTextureBegin(0x00101010);

	//환경 랜더
	this->RenderEnvironment();

	//랜더된다.
	this->Scene_Render0();
	this->Scene_Render1();
	this->Scene_Render2();


#ifdef _DEBUG		//디버그 모드에서만 실행
	//디바이스 랜더링 종료 명령
	//월드 그리드
	GIZMO_MGR->WorldGrid(1, 10);
#endif


	this->pMainCamera->RenderTextureEnd();
}

//메인카메라의 랜더 Texture 를 얻는다.
LPDIRECT3DTEXTURE9 cScene::GetTexture()
{
	return this->pMainCamera->GetRenderTexture();
}

void cScene::ChangeCameraMod()
{
	//if (KEY_MGR->IsOnceDown(VK_TAB)) {
	//	viCamera++;

	//	if (viCamera == vCamera.end()) {
	//		viCamera = vCamera.begin();
	//	}
	//	pMainCamera = (*viCamera);
	//}
}

void cScene::ChangeCameraMod(int IDx)
{
	if (*(vCamera.begin() + IDx) != NULL) {
		pMainCamera = *(vCamera.begin() + IDx);
	}
}