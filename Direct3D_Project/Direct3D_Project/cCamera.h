#pragma once

#include "cTransform.h"
#include "cFrustum.h"

class cCamera : public cTransform
{
public:
	float fov;			//ī�޶� ȭ��
	float camNear;		//ī�޶� Near
	float camFar;		//ī�޶� Far
	float aspect;		//ī�޶� ��Ⱦ��
	bool bOrtho;		//������?
	float orthoSize;	//�����û�����

protected:
	D3DXMATRIXA16		matView;		//�����
	D3DXMATRIXA16		matProjection;	//�������
	D3DXMATRIXA16		matViewProjection;	//�� * �������


private:
	LPDIRECT3DTEXTURE9		m_pRenderTexture;		//�������� Texture
	LPDIRECT3DSURFACE9		m_pRenderSurface;		//���� Texture Depth ���ۿ� Stencil ���۰� �ִ� Surface
	LPDIRECT3DSURFACE9		m_pDeviceTargetSurface;
	LPDIRECT3DSURFACE9		m_pDeviceDepthAndStencilSurface;


public:
	cFrustum			Frustum;

public:
	cCamera(void);
	~cCamera(void);



	virtual void CameraUpdate(float timeDelta) = 0;
	//������İ� View ����� ������Ʈ
	void UpdateMatrix();

	//View ��İ� ���� ����� ����
	void UpdateCamToDevice(LPDIRECT3DDEVICE9 pDevice);


	//�������� ������Ʈ
	void UpdateFrustum();


	//ī�޶� ���� ����� ��´�.
	D3DXMATRIXA16 GetViewMatrix()const {
		return this->matView;
	}

	D3DXMATRIXA16 GetProjectionMatrix()const {
		return this->matProjection;
	}

	D3DXMATRIXA16 GetViewProjectionMatrix()const {
		return this->matViewProjection;
	}

	//ȭ�� ����
	void SetFov(float fov) {
		this->fov = fov;
	}

	float GetFov() {
		return this->fov;
	}

	//ȭ���� ��ġ�� ������ ī�޶��� ���� ���̸� ��´�
	void ComputeRay(LPRay pOutRay, const D3DXVECTOR2* screenPos);

	//���� ��ġ��  ȭ���� ��ġ�� ��´�.
	bool GetWorldPosToScreenPos(D3DXVECTOR2* pScreenPos, const D3DXVECTOR3* pWorldPos);

	//���� Texture �� �غ� �Ѵ�.
	void ReadyRenderToTexture(int width, int height);

	//Shadow Map Texture�� �غ��Ѵ�.
	void ReadyShadowTexture(int size);

	//���� Texture �� ������ ����
	void RenderTextureBegin(DWORD backColor);

	//���� Texture �� ������ ����
	void RenderTextureEnd();

	//���� Texture ��´�.
	LPDIRECT3DTEXTURE9 GetRenderTexture();
};


class PlayerCamera : public cCamera
{
public:
	PlayerCamera() : cCamera()
	{
		//�⺻ȭ�� 60 ��
		this->fov = 60.0f * ONE_RAD;

		//�⺻ Near
		this->camNear = 0.1f;

		//�⺻ Far
		this->camFar = 1000.0f;
	}

	void CameraUpdate(float timeDelta)
	{
	//	this->DefaultControl4(timeDelta);
	}
};

class FreeCamera : public cCamera
{
public:
	FreeCamera() : cCamera()
	{
		//�⺻ȭ�� 60 ��
		this->fov = 60.0f * ONE_RAD;

		//�⺻ Near
		this->camNear = 0.1f;

		//�⺻ Far
		this->camFar = 1000.0f;
	}
	void CameraUpdate(float timeDelta)
	{
		this->DefaultControl(timeDelta);
	}
};

class MainMenuCamera : public cCamera
{
public:
	MainMenuCamera() : cCamera()
	{
		//�⺻ȭ�� 60 ��
		this->fov = 60.0f * ONE_RAD;

		//�⺻ Near
		this->camNear = 0.1f;

		//�⺻ Far
		this->camFar = 1000.0f;
	}
	void CameraUpdate(float timeDelta)
	{
		this->DefaultControl3(timeDelta);
	}
};

