#include "stdafx.h"
#include "cRainParticle.h"
#include "cCamera.h"
#include "cParticleEmitter.h"

cRainParticle::cRainParticle()
{
}


cRainParticle::~cRainParticle()
{
}

HRESULT cRainParticle::Scene_Init()
{
	_rain = new cParticleEmitter();

	_rain->SetActive(true);

	_rain->pTransform->SetWorldPosition(0, 10, 0);
	//D3DXMATRIXA16 matRota;
	//D3DXMatrixRotationZ(&matRota, 180 * ONE_RAD);
	//m_pParticle->pTransform->SetRotateWorld(matRota);

	//배열을 2 개이상 
	VEC_COLOR colors;
	colors.push_back(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	colors.push_back(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	VEC_SCALE scales;
	scales.push_back(0.1f);
	scales.push_back(0.1f);


	LPDIRECT3DTEXTURE9 pTex = RESOURCE_TEXTURE->GetResource("../Resources/RainParticle.png");

	//파티클 랜더러 셋팅
	_rain->Init(
		5000,            //최대 파티클 수
		2000.0f,            //초당 파티클 발생 량
		1,               //하나의 파티클 입자 라이프 최소값
		1,               //하나의 파티클 입자 라이프 최대값
		D3DXVECTOR3(0, -10.0f, 0),   //파티클 입자 속도 최소값 ( 로컬기준 )
		D3DXVECTOR3(0, -15.0f, 0),   //파티클 입자 속도 최대값 ( 로컬기준 )
		D3DXVECTOR3(1, -3, 1),   //파티클 입자 가속 최소값 ( 로컬기준 )
		D3DXVECTOR3(-1, -5, -1), //파티클 입자 가속 최대값 ( 로컬기준 )
		colors,            //컬러 배열
		scales,            //스케일 배열
		3.1f,            //입자크기 최소값
		4.2f,            //최대값
		pTex,            //텍스쳐
		false);

	_rain->EmissionType = PATICLE_EMISSION_TYPE::SPHERE_OUTLINE;
	_rain->SphereEmissionRange = 6.0f;


	_rain->StartEmission();
	return S_OK;
}

void cRainParticle::Scene_Release()
{
	_rain->Release();
	SAFE_DELETE(_rain);
}

void cRainParticle::Scene_Update(float timeDelta)
{
	_rain->Update(timeDelta);
	//_rain->pTransform->DefaultControl2(timDelta);
}

void cRainParticle::Scene_Render1()
{
	_rain->Render();
}

void cRainParticle::Scene_RenderSprite()
{
}

void cRainParticle::RainPosition(Player* pl)
{
	_rain->pTransform->SetWorldPosition(
		pl->getTrans()->GetWorldPosition().x,
		10,
		pl->getTrans()->GetWorldPosition().z);
}