#include "stdafx.h"
#include "cScene_Loading.h"

#include "cCamera.h"

cScene_Loading::cScene_Loading()
{
}


cScene_Loading::~cScene_Loading()
{
}

HRESULT cScene_Loading::Scene_Init()
{
	pBackground = RESOURCE_TEXTURE->GetResource("../Resources/Loading/loading_Background.png");
	rcBackground = { 0, 0, WINSIZE_X, WINSIZE_Y };

	//this->main = RESOURCE_TEXTURE->GetResource("../Resources/UI_Resource/rrmain.png");

	this->pMainCamera->SetWorldPosition(0, 5, -10);

	return S_OK;
}

void cScene_Loading::Scene_Release()
{
}

void cScene_Loading::Scene_Update(float timDelta)
{
}

void cScene_Loading::Scene_Render1()
{
}

void cScene_Loading::Scene_RenderSprite()
{
	SPRITE_MGR->DrawTexture(pBackground, &rcBackground, 0, 0, 0.5f, 0.5f, NULL, D3DCOLOR_ARGB(255, 255, 255, 255), &D3DXVECTOR3(0, 0, 0));

	//SPRITE_MGR->DrawTexture(this->main, &mainpicture, 0, 15, 1.3f, 1.3f, NULL, D3DCOLOR_ARGB(255, 255, 255, 255), &D3DXVECTOR3(0, 0, 0));
}
