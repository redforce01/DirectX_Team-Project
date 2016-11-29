#include "StdAfx.h"
#include "cPhysicManager.h"
#include "cCamera.h"

cCamera::cCamera(void)
	: m_pRenderSurface(NULL)
	, m_pRenderTexture(NULL)
	, m_pDeviceTargetSurface(NULL)
	, m_pDeviceDepthAndStencilSurface(NULL)
{
	//�⺻ȭ�� 60 ��
	this->fov = 60.0f * ONE_RAD;

	//�⺻ Near
	this->camNear = 0.1f;

	//�⺻ Far
	this->camFar = 1000.0f;

	//��Ⱦ�� default
	this->aspect = static_cast<float>(WINSIZE_X) / static_cast<float>(WINSIZE_Y);

}


cCamera::~cCamera(void)
{
	SAFE_RELEASE(m_pRenderTexture);
	SAFE_RELEASE(m_pRenderSurface);
}




//������İ� View ����� ������Ʈ
void cCamera::UpdateMatrix()
{
	//ȭ���� ���� Projection ��� ������Ʈ
	D3DXMatrixPerspectiveFovLH(
		&matProjection,
		this->fov,
		static_cast<float>(WINSIZE_X) / static_cast<float>(WINSIZE_Y),
		this->camNear,
		this->camFar);

	//����� ī�޶� ������ġ�� ���� ������̴�.
	D3DXMatrixInverse(&matView, NULL, &matFinal);


	this->matViewProjection = this->matView * this->matProjection;


}

//View ��İ� ���� ����� ����
void cCamera::UpdateCamToDevice(LPDIRECT3DDEVICE9 pDevice)
{
	//��� ������Ʈ ���ְ� 
	this->UpdateMatrix();

	//����
	pDevice->SetTransform(D3DTS_VIEW, &this->matView);
	pDevice->SetTransform(D3DTS_PROJECTION, &this->matProjection);


}

//�������� ������Ʈ
void cCamera::UpdateFrustum()
{
	Frustum.UpdateFrustum(&this->matViewProjection);
}


//ȭ���� ��ġ�� ������ ī�޶��� ���� ���̸� ��´�
void cCamera::ComputeRay(LPRay pOutRay, const D3DXVECTOR2* screenPos)
{
	//Device �� ����Ʈ�� ��´�.
	D3DVIEWPORT9 viewPort;
	Device->GetViewport(&viewPort);

	//��ũ���� ��ġ ������ ���� 
	float factorX = screenPos->x / viewPort.Width;
	float factorY = (1.0f - (screenPos->y / viewPort.Height));

	//0 ~ 1 => -1 ~ 1
	factorX = factorX * 2.0f - 1.0f;
	factorY = factorY * 2.0f - 1.0f;

	//��������� ��´�.
	D3DXMATRIXA16 matProj = this->GetProjectionMatrix();

	//

	//������ ��ġ�� ȭ�� �����Ϸ��� ������.
	D3DXVECTOR3 direction(
		factorX / matProj._11,
		factorY / matProj._22,
		1.0f);

	//ī�޶� ���� ���
	D3DXMATRIXA16 matCamWorld = GetFinalMatrix();

	//������ ������ �������� ���� ���͸� ī�޶� ���� ��Ʈ����
	//���� ������ ���´�.
	D3DXVec3TransformNormal(
		&direction, &direction, &matCamWorld);

	//���⺤�� ����
	D3DXVec3Normalize(&direction, &direction);

	//������ ����
	pOutRay->direction = direction;

	//������ �������� ī�޶� ��ġ�� �ȴ�.
	pOutRay->origin = this->GetWorldPosition();

}


//���� ��ġ��  ȭ���� ��ġ�� ��´�.
bool cCamera::GetWorldPosToScreenPos(D3DXVECTOR2* pScreenPos, const D3DXVECTOR3* pWorldPos)
{
	//ī�޶��� ViewProjection ����� ��´�.
	D3DXMATRIXA16 matViewProj = this->GetViewProjectionMatrix();

	//���� ��ġ�� ������...
	D3DXVECTOR3 pos;
	D3DXVec3TransformCoord(&pos, pWorldPos, &matViewProj);

	//��������
	//				*-------*<-( 1, 1, 1 )
	//			   /|      /|
	//			  *-------* |
	//            | *-----|-*
	//		      |/      |/
	//(-1,-1, 0)->*-------* 

	//0 ~ 1 ���� ���� ������ �̸� ī�޶� �ڿ� �ִٴ¿���
	if (pos.z < 0.0f || pos.z > 1.0f)
		return false;

	//ȭ�� �߾ӿ� �ִٸ� pos xy ���� ������ ����.
	//pos.x == 0  
	//pos.y == 0

	//ȭ�� ���� ������
	float halfScreenW = WINSIZE_X * 0.5f;
	float halfScreenH = WINSIZE_Y * 0.5f;

	//���� ȭ�� ��ġ
	pScreenPos->x = halfScreenW + (halfScreenW * pos.x);
	pScreenPos->y = halfScreenH - (halfScreenH * pos.y);	//Y ���� ����

	return true;
}


//���� Texture �� �غ� �Ѵ�.
void cCamera::ReadyRenderToTexture(int width, int height)
{
	SAFE_RELEASE(m_pRenderTexture);
	SAFE_RELEASE(m_pRenderSurface);

	//RenderTarget �� Texture �����
	Device->CreateTexture(
		width,						//Texture ���� �ػ� 
		height,						//Texture ���� �ػ�
		1,							//�Ӹ�ü�� ����
		D3DUSAGE_RENDERTARGET,		//RenderTarget �� Texture�̴�
		D3DFMT_A8R8G8B8,			//TEXTURE ���� RenderTexture ����D3DFMT_A8R8G8B8 ������
		D3DPOOL_DEFAULT,			//RenderTarget �� Texture �� Pool �� Default
		&m_pRenderTexture,			//������ Texture �޾ƿ� ������
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
		&m_pRenderSurface,			//���� ������...
		NULL);


}


//���� Texture �� ������ ����
void cCamera::RenderTextureBegin(DWORD backColor)
{
	//�� ����̽��� Target ������ǥ��� DepthStencil ������ ǥ�������� ���
	Device->GetRenderTarget(0, &m_pDeviceTargetSurface);
	Device->GetDepthStencilSurface(&m_pDeviceDepthAndStencilSurface);

	//RenderTexture �� Surface �� ��´�.
	LPDIRECT3DSURFACE9 texSurface = NULL;
	if (SUCCEEDED(this->m_pRenderTexture->GetSurfaceLevel(0, &texSurface)))
	{
		//Texture ǥ���� Device �� Target ���۷� �����Ѵ�.
		Device->SetRenderTarget(0, texSurface);

		//���õ� Surface ������ �ٷ� �����ִ� ���Ǹ� ������...
		SAFE_RELEASE(texSurface);
	}

	//Depth ���ۿ� Stencil ������ Surface �� m_pRenderSurface ����
	Device->SetDepthStencilSurface(m_pRenderSurface);

	//����̽� ���� Ŭ���� ( ��� ������ Setting �� Textuer �� Surface �� Ŭ���� �ȴ�  )
	Device->Clear(0, NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		backColor,
		1.0f,
		0);
}


//���� Texture �� ������ ����
void cCamera::RenderTextureEnd()
{
	//Render Texture �� �׸� �۾��� �������� �ٽ� ���󺹱��ϴ� ����....
	Device->SetRenderTarget(0, m_pDeviceTargetSurface);
	Device->SetDepthStencilSurface(m_pDeviceDepthAndStencilSurface);

	//���õ� Surface ������ �ٷ� �����ִ� ���Ǹ� ������...
	SAFE_RELEASE(m_pDeviceTargetSurface);
	SAFE_RELEASE(m_pDeviceDepthAndStencilSurface);
}

//���� Texture ��´�.
LPDIRECT3DTEXTURE9 cCamera::GetRenderTexture()
{
	return m_pRenderTexture;
}