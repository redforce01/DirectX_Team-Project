#include "stdafx.h"
#include "cTitle.h"


cTitle::cTitle()
{
}


cTitle::~cTitle()
{
}

HRESULT cTitle::Scene_Init()
{
	this->main = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/rrmain.png");
	this->mouse = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/maininter/mouse.png");
	this->background = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/background.png");
	this->NewGame = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/maininter/New.png");


	this->Exit = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/maininter/Exit.png");
	this->logo = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/logo.png");
	//this->Sound = RESOURCE_TEXTURE->GetResource("Resource/sound.png");
	//this->Soundcircle = RESOURCE_TEXTURE->GetResource("Resource/soundcircle.png");
	this->isoption = false;
	//////////////////
	/*
	可记 持阑 巴 : 荤款靛


	*/
	/////////////////
	mainpicture = { 0,0,WINSIZE_X + 400 ,WINSIZE_Y };
	backg = { 0,0,WINSIZE_X + 500 ,WINSIZE_Y };
	exitcoliRect = { 600,580,730,612 };
	newgamecoliRect = { 550,500,807,533 };
	Logo = { 0,0,257,130 };
	ShowCursor(false);
	black = 0;
	back = 0;
	logoalpha = 0;
	newgamecoliRect = { 550,500,807,533 };

	SOUNDDATA->playSound(SOUND_TYPE_MENU, SOUND_PLAY_TYPE_MENUBGM, 0, 0.3);

	return S_OK;
}

void cTitle::Scene_Release()
{
}

void cTitle::Scene_Update(float timeDelta)
{	
	count += 1;
	if (count >= 40)
	{
		if (black < 80)
		{
			black += 1;
		}
		if (back < 255)
		{
			back += 5;
		}
		if (black >= 80 && black < 150)
		{
			black += 10;
		}
		if (black == 150)
		{
			black = 160;
		}
		if (black == 160)
		{
			black = 70;
		}

	}
	if (count == 160)
	{
		count = 0;
		black = 50;
	}
	if (logoalpha < 160)
		logoalpha += 1;
	
	if (KEY_MGR->IsOnceDown(VK_LBUTTON))
	{		
		if (PtInRect(&exitcoliRect, GetMousePos()))
		{
			exit(0);
			SOUNDDATA->stopSound(SOUND_TYPE_MENU, SOUND_PLAY_TYPE_MENUBGM, 0);
		}
		else if (PtInRect(&newgamecoliRect, GetMousePos()))
		{
			SCENE_MGR->ChangeScene("game", 1);
			SOUNDDATA->stopSound(SOUND_TYPE_MENU, SOUND_PLAY_TYPE_MENUBGM, 0);
		}
	}
}

void cTitle::Scene_Render1()
{
}

void cTitle::Scene_RenderSprite()
{
	SPRITE_MGR->DrawTexture(this->main, &mainpicture, 0, 15, 1.3f, 1.3f, NULL, D3DCOLOR_ARGB(255, black, black, black), &D3DXVECTOR3(0, 0, 0));


	SPRITE_MGR->DrawTexture(this->background, &backg, -50, 0, 1.3f, 1.3f, NULL, D3DCOLOR_ARGB(255, back, back, back), &D3DXVECTOR3(0, 0, 0));

	SPRITE_MGR->DrawTexture(this->logo, &Logo, 420, 80, 1.8f, 1.8f, NULL, D3DCOLOR_ARGB(logoalpha, 255, 255, 255), &D3DXVECTOR3(0, 0, 0));

	SPRITE_MGR->DrawTexture(this->NewGame, &newgameRect, 550, 500, 0.7f, 0.7f, NULL, D3DCOLOR_ARGB(logoalpha, 255, 255, 255), &D3DXVECTOR3(0, 0, 0));




	SPRITE_MGR->DrawTexture(this->Exit, &exitRect, 600, 580, 0.55f, 0.55f, NULL, D3DCOLOR_ARGB(logoalpha, 255, 255, 255), &D3DXVECTOR3(0, 0, 0));

	SPRITE_MGR->DrawTexture(this->mouse, &mouserect, GetMousePos().x, GetMousePos().y, 0.3f, 0.3f, -0.1f, D3DCOLOR_ARGB(logoalpha, 255, 255, 255), &D3DXVECTOR3(0, 0, 0));
}