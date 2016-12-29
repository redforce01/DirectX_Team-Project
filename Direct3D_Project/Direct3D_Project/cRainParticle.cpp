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

	//�迭�� 2 ���̻� 
	VEC_COLOR colors;
	colors.push_back(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	colors.push_back(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	VEC_SCALE scales;
	scales.push_back(0.1f);
	scales.push_back(0.1f);


	LPDIRECT3DTEXTURE9 pTex = RESOURCE_TEXTURE->GetResource("../Resources/RainParticle.png");

	//��ƼŬ ������ ����
	_rain->Init(
		5000,            //�ִ� ��ƼŬ ��
		2000.0f,            //�ʴ� ��ƼŬ �߻� ��
		1,               //�ϳ��� ��ƼŬ ���� ������ �ּҰ�
		1,               //�ϳ��� ��ƼŬ ���� ������ �ִ밪
		D3DXVECTOR3(0, -10.0f, 0),   //��ƼŬ ���� �ӵ� �ּҰ� ( ���ñ��� )
		D3DXVECTOR3(0, -15.0f, 0),   //��ƼŬ ���� �ӵ� �ִ밪 ( ���ñ��� )
		D3DXVECTOR3(1, -3, 1),   //��ƼŬ ���� ���� �ּҰ� ( ���ñ��� )
		D3DXVECTOR3(-1, -5, -1), //��ƼŬ ���� ���� �ִ밪 ( ���ñ��� )
		colors,            //�÷� �迭
		scales,            //������ �迭
		3.1f,            //����ũ�� �ּҰ�
		4.2f,            //�ִ밪
		pTex,            //�ؽ���
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