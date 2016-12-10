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

	this->pMainCamera = new cCamera();
}


cScene::~cScene()
{
	SAFE_RELEASE(evironmentTexture);
	SAFE_RELEASE(evironmemtSphereMesh);

	SAFE_DELETE(this->pMainCamera);
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
	pMainCamera->DefaultControl(timeDelta);
	pMainCamera->UpdateCamToDevice(Device);

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
	//GIZMO_MGR->WorldGrid(1, 10);
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
	for (UINT i = 0; i < iPass; i++){
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

void cScene::ReadyShadowMap(std::vector<cBaseObject*>* renderObjects, cTerrain * pTerrain)
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

LPDIRECT3DTEXTURE9 cScene::GetTexture()
{
	return this->pMainCamera->GetRenderTexture();
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