#include "stdafx.h"
#include "cScene.h"
#include "cCamera.h"
#include "cLight_Direction.h"
#include "cTerrain.h"

cScene::cScene()
	: evironmentTexture(NULL), evironmemtSphereMesh(NULL)
{
	//ȯ�� Effect ���� ���´�.
	this->evironmentEffect =
		RESOURCE_FX->GetResource("../Resources/Shaders/EvironmentCUBE.fx");

	cCamera* DebuggingCamera = new FreeCamera;
	cCamera* MilesCamera = new PlayerCamera;

	vCamera.push_back(MilesCamera);
	vCamera.push_back(DebuggingCamera);

	viCamera = vCamera.begin();
	pMainCamera = vCamera[PLAYER];

	//�⺻ ���� ����
	this->pSceneBaseDirectionLight = new cLight_Direction();

	//���⼺ ������ ī�޶� �� �����Ѵ�.
	this->pDirectionLightCamera = new FreeCamera();

	//�⺻ ��
	this->pSceneBaseDirectionLight->Color = D3DXCOLOR(1, 1, 1, 1);
	this->pSceneBaseDirectionLight->Intensity = 1.0f;

	//�׸��� �Ÿ�
	shadowDistance = 100.0f;


	//ī�޶��� ��������� �ٲ۴�...
	this->pDirectionLightCamera->bOrtho = true;
	this->pDirectionLightCamera->camNear = 0.1f;
	this->pDirectionLightCamera->camFar = shadowDistance * 2.0f;
	this->pDirectionLightCamera->aspect = 1;
	this->pDirectionLightCamera->orthoSize = shadowDistance * 1.5f;	//����ũ��� �׸���ũ���...



																	//���⼺���� ī�޶��� RenderToTexture �غ�
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
	//���� �ʱ�ȭ �̷������.
	if (FAILED(this->Scene_Init()))
		return E_FAIL;

	//����ī�޶� RenderToTexture �غ�
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
	//���� ������ �Ͼ��.
	this->Scene_Release();
}

void cScene::Update(float timeDelta)
{
	//pMainCamera->DefaultControl(timeDelta);
	pMainCamera->CameraUpdate(timeDelta);
	pMainCamera->UpdateCamToDevice(Device);
	pMainCamera->UpdateFrustum();
	this->ChangeCameraMod();

	//����ī�޶� DirectionLight �� ������ ������ü ����ٴϰ�....

	//���� ��ġ
	D3DXVECTOR3 camPos = pMainCamera->GetWorldPosition();		//����ī�޶��� ��ġ
	D3DXVECTOR3 camFront = pMainCamera->GetForward();			//����ī�޶��� ����
	D3DXVECTOR3 lightDir = pSceneBaseDirectionLight->pTransform->GetForward();	//���⼺ ������ ����

	D3DXVECTOR3 lightPos = camPos +
		(camFront * (shadowDistance * 0.5f)) +
		(-lightDir * shadowDistance);

	this->pDirectionLightCamera->SetWorldPosition(lightPos.x, lightPos.y, lightPos.z);
	this->pDirectionLightCamera->LookDirection(lightDir);


	//���� ������Ʈ�� �Ͼ��.
	this->Scene_Update(timeDelta);
}

void cScene::Render()
{
	this->pMainCamera->RenderTextureBegin(0x00101010);

	//ȯ�� ����
	this->RenderEnvironment();

	//�����ȴ�.
	this->Scene_Render0();
	this->Scene_Render1();
	this->Scene_Render2();

#ifdef _DEBUG		//����� ��忡���� ����
	//����̽� ������ ���� ���
	//���� �׸���
	GIZMO_MGR->WorldGrid(1, 10);
#endif

	this->pMainCamera->RenderTextureEnd();

	//Scene ����
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


	//���ö���Ʈ ����
	SPRITE_MGR->BeginSpriteRender();
	this->Scene_RenderSprite();
	SPRITE_MGR->EndSpriteRender();
}

void cScene::SetEnvironment(std::string cubeFilePath)
{
	//������ ȯ����� ���õǾ������� ����..
	SAFE_RELEASE(evironmentTexture);

	//ȯ��� �ε�
	D3DXCreateCubeTextureFromFile(
		Device,
		cubeFilePath.c_str(),
		&evironmentTexture);

	//ȯ�汸 �ε��Ǿ����� �ʴٸ�....
	if (this->evironmemtSphereMesh == NULL)
	{
		D3DXCreateSphere(Device,
			3.0f,
			20,
			20,
			&evironmemtSphereMesh, NULL);
	}
}

//ȯ�汸 ����
void cScene::RenderEnvironment()
{
	//ȯ�� �� �ε��Ȱ� ���ٸ� �����ض�.....
	if (this->evironmentTexture == NULL)
		return;

	//ȯ�� Effect ����
	this->evironmentEffect->SetTexture("MyCube_Tex", this->evironmentTexture);

	//WVP ��Ʈ����
	D3DXMATRIXA16 matWorld = this->pMainCamera->GetFinalMatrix();
	D3DXMATRIXA16 matViewProj = this->pMainCamera->GetViewProjectionMatrix();
	D3DXMATRIXA16 matWVP = matWorld * matViewProj;

	this->evironmentEffect->SetMatrix("matWVP", &matWVP);
	this->evironmentEffect->SetMatrix("matWorld", &matWorld);


	//�׷���...
	UINT numPass;
	this->evironmentEffect->Begin(&numPass, 0);		//���̴��� �׸��ٴ� ���� �˸��� pass ���� ��´�.

	for (UINT i = 0; i < numPass; i++)	//���� ����� ����.
	{
		this->evironmentEffect->BeginPass(i);			//i��° ��������

		this->evironmemtSphereMesh->DrawSubset(0);

		this->evironmentEffect->EndPass();				//i��° ������
	}
	this->evironmentEffect->End();						//���̴��� �׸��� ��


}



void cScene::ReadyShadowMap(std::vector<cBaseObject*>* renderObjects, cTerrain* pTerrain)
{
	//���⼺������ ���� ī�޶��� Frustum ������Ʈ
	this->pDirectionLightCamera->UpdateMatrix();
	this->pDirectionLightCamera->UpdateFrustum();


	//���̷��Ƕ����� ī�޶� ������ �ֵ鸸 �׸���...
	static std::vector<cBaseObject*>		shadowCullObject;
	shadowCullObject.clear();

	for (int i = 0; i < renderObjects->size(); i++) {

		//�������� �ȿ� �ִ�?
		if (this->pDirectionLightCamera->Frustum.IsInFrustum((*renderObjects)[i]))
		{
			shadowCullObject.push_back((*renderObjects)[i]);
		}
	}


	//������ �� �׸���.
	this->pDirectionLightCamera->RenderTextureBegin(0xffffffff);

	cXMesh_Static::SetCamera(this->pDirectionLightCamera);
	cXMesh_Static::SetTechniqueName("CreateShadow");

	cXMesh_Skinned::SetCamera(this->pDirectionLightCamera);
	cXMesh_Skinned::SetTechniqueName("CreateShadow");



	for (int i = 0; i < shadowCullObject.size(); i++) {
		if (shadowCullObject[i]->IgnoreCreateShadow == false)
			shadowCullObject[i]->Render();
	}

	//���� Terrain �� ������ ���� �׷����Ѵٸ�...
	if (pTerrain != NULL)
	{
		pTerrain->RenderShadow(this->pDirectionLightCamera);
	}


	this->pDirectionLightCamera->RenderTextureEnd();


	//���� Terrain �� ������ ���� �����Ѵٸ�...
	if (pTerrain != NULL)
	{
		pTerrain->m_pTerrainEffect->SetTexture("Shadow_Tex",
			this->pDirectionLightCamera->GetRenderTexture());

		pTerrain->m_pTerrainEffect->SetMatrix("matLightViewProjection",
			&this->pDirectionLightCamera->GetViewProjectionMatrix());
	}



	//������ Texture
	cXMesh_Static::sStaticMeshEffect->SetTexture("Shadow_Tex",
		this->pDirectionLightCamera->GetRenderTexture());

	//������ ���
	cXMesh_Static::sStaticMeshEffect->SetMatrix("matLightViewProjection",
		&this->pDirectionLightCamera->GetViewProjectionMatrix());


	//������ Texture
	cXMesh_Skinned::sSkinnedMeshEffect->SetTexture("Shadow_Tex",
		this->pDirectionLightCamera->GetRenderTexture());

	//������ ���
	cXMesh_Skinned::sSkinnedMeshEffect->SetMatrix("matLightViewProjection",
		&this->pDirectionLightCamera->GetViewProjectionMatrix());
}



//���� ī�޶��� RenderToTexture �� ������Ʈ�Ѵ�.
void cScene::RenderToMainCamTexture()
{
	this->pMainCamera->RenderTextureBegin(0x00101010);

	//ȯ�� ����
	this->RenderEnvironment();

	//�����ȴ�.
	this->Scene_Render0();
	this->Scene_Render1();
	this->Scene_Render2();


#ifdef _DEBUG		//����� ��忡���� ����
	//����̽� ������ ���� ���
	//���� �׸���
	GIZMO_MGR->WorldGrid(1, 10);
#endif


	this->pMainCamera->RenderTextureEnd();
}

//����ī�޶��� ���� Texture �� ��´�.
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