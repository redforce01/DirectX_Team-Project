#include "stdafx.h"
#include "cCameraUI.h"
#include "cCamera.h"


cCameraUI::cCameraUI(cCamera* Camera, Unit* unit)
{

	pMainCamera = Camera;
	CamFov = pMainCamera->fov;
	m_CurUnit = unit;
}


cCameraUI::~cCameraUI()
{
}

HRESULT cCameraUI::Scene_Init()
{
	isTrunOn = true;
	this->isRec = false;
	isdelay = false;
	//카메라
	this->STBY = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/camera/STBY.png");
	this->HD = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/camera/HD2.png");
	this->W_T = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/camera/W-T.png");
	this->battery = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/camera/battery.png");
	this->cube = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/camera/cube.png");
	this->rec = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/camera/rec.png");
	this->zoombar = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/camera/zoombar.png");

	zoomin = false;
	zoomout = false;
	zoombarx = 640;
	delayCount = 0;

	return S_OK;
}

void cCameraUI::Scene_Release()
{
}

void cCameraUI::Scene_Update(float timeDelta)
{
	if (isRec)
	{
		delayCount++;
		if (delayCount > 50)
		{
			//* 카메라 올렸으면
			if (this->m_CurUnit->IsCamUp())
			{
				//* 약간 줌인 된 상태에서 시작함.
				pMainCamera->fov = this->CamFov + g_Czoom - 0.1f;
			}
			else
			{
				//* 카메라를 내리면 초기 세팅된 그 값으로
				pMainCamera->fov = this->CamFov;
				g_Czoom = 0;
			}
			isdelay = true;
		}
		
	}
	//////rec깜빡임
	if (this->isdelay == true)
	{
		reccount += 1;

		if (reccount == 40)
		{
			reccount = 0;
		}
	}
	else
	{
		reccount = 0;
	}
}

void cCameraUI::Scene_Render1()
{
}

void cCameraUI::Scene_RenderSprite()
{
	if (this->isdelay == true)
	{
		//카메라
		RECT rc = { 0,0,63,17 };
		SPRITE_MGR->DrawTexture(this->STBY, &rc, 20, 40, 1.2f, 1.2f, NULL, 0xffffffff, &D3DXVECTOR3(0, 0, 0));

		RECT rc2 = { 0,0,64,127 };
		SPRITE_MGR->DrawTexture(this->HD, &rc2, 20, 600, 1.3f, 1.2f, NULL, 0xffffffff, &D3DXVECTOR3(0, 0, 0));

		RECT rc3 = { 0,0,262,15 };
		SPRITE_MGR->DrawTexture(this->W_T, &rc3, 480, 40, 1.2f, 1.2f, NULL, D3DCOLOR_ARGB(150, 255, 255, 255), &D3DXVECTOR3(0, 0, 0));


		RECT rc4 = { 0,0,127,32 };
		SPRITE_MGR->DrawTexture(this->battery, &rc4, 1100, 34, 1.2f, 1.2f, NULL, 0xffffffff, &D3DXVECTOR3(0, 0, 0));

		RECT rc5 = { 0,0,128,64 };//l
		SPRITE_MGR->DrawTexture(this->cube, &rc5, 595, 350, 1.2f, 1.2f, NULL, 0xffffffff, &D3DXVECTOR3(0, 0, 0));
		if (this->reccount <= 20)
		{
			RECT rc6 = { 0,0,300,300 };
			SPRITE_MGR->DrawTexture(this->rec, &rc6, 1130, 680, 1.0f, 1.0f, NULL, D3DCOLOR_ARGB(200, 255, 255, 255), &D3DXVECTOR3(0, 0, 0));
		}
		RECT rc7 = { 0,0,5,13 };
		SPRITE_MGR->DrawTexture(this->zoombar, &rc7, zoombarx, 41, 1.0f, 1.2f, NULL, 0xffffffff, &D3DXVECTOR3(0, 0, 0));
	}
}

void cCameraUI::TurnOnCam()
{
	if (isRec) {
		isdelay = false;
		isRec = false;
		delayCount = 0;
	}
	else if (!isRec) isRec = true;
}
