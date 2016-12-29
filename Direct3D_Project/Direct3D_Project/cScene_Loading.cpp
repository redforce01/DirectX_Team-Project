#include "stdafx.h"
#include "cScene_Loading.h"


cScene_Loading::cScene_Loading()
{
}


cScene_Loading::~cScene_Loading()
{
}

HRESULT cScene_Loading::Scene_Init()
{
	LoadingImage = RESOURCE_TEXTURE->GetResource("../Resources/Loading/resize_loading.bmp");
	LoadingContinue = RESOURCE_TEXTURE->GetResource("../Resources/Loading/continue.bmp");

	LoadingRect = { 0, 0, 1920, 1080 };
	LoadingRectContinue = { 0, 0, 250, 150 };
	
	scaleX = scaleY = 0.5;

	ContinueAlpha = 255;
	AlphaChange = TRUE;

	return S_OK;
}

void cScene_Loading::Scene_Release()
{
}

void cScene_Loading::Scene_Update(float timeDelta)
{
	SOUNDDATA->stopSound(SOUND_TYPE_MENU, SOUND_PLAY_TYPE_MENUBGM, 0);
	SOUNDDATA->playSound(SOUND_TYPE_BGM, SOUND_PLAY_GAME_BGM, 4, 0.1);	

	if (AlphaChange)
	{
		ContinueAlpha -= 5;
		if (ContinueAlpha <= 0) AlphaChange = !AlphaChange;
	}
	else
	{
		ContinueAlpha += 5;
		if (ContinueAlpha >= 255) AlphaChange = !AlphaChange;
	}

	if (KEY_MGR->IsOnceDown('R'))
	{
		scaleX += 0.01;
	}
	if (KEY_MGR->IsOnceDown('F'))
	{
		scaleX -= 0.01;
	}
	if (KEY_MGR->IsOnceDown('T'))
	{
		scaleY += 0.01;
	}
	if (KEY_MGR->IsOnceDown('G'))
	{
		scaleY -= 0.01;
	}
}

void cScene_Loading::Scene_Render1()
{
	
}

void cScene_Loading::Scene_RenderSprite()
{
	SPRITE_MGR->DrawTexture(LoadingImage, &LoadingRect, 0, 0, 0.67, 0.75, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	SPRITE_MGR->DrawTexture(LoadingContinue, &LoadingRectContinue, WINSIZE_X / 2, WINSIZE_Y / 2 + 200, scaleX, scaleY, NULL, D3DCOLOR_ARGB(ContinueAlpha, 255, 255, 255));

	char szTemp[128];
	sprintf(szTemp, "%.2f 퍼센트 로딩중", SCENE_MGR->fProgress);
	DXFONT_MGR->PrintText(szTemp, 100, 130, 0xffffff00);
}
