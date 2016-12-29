#include "stdafx.h"
#include "cMovieEvent.h"


cMovieEvent::cMovieEvent()
{
}


cMovieEvent::~cMovieEvent()
{
}

HRESULT cMovieEvent::Scene_Init()
{
	//무비이벤트
	this->movievent = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/event.png");
	run = false;
	stop = false;

	return S_OK;
}

void cMovieEvent::Scene_Release()
{
}

void cMovieEvent::Scene_Update(float timeDelta)
{
	if (isGameEvent)
		run = true;
	else
		stop = true;

	if (run == true)
	{
		if (moviealpha >= 0 && moviealpha < 255)
		{
			moviealpha += 5;
		}
	}
	if (stop == true)
	{
		if (moviealpha <= 255 && moviealpha > 0)
		{
			moviealpha -= 10;
		}
		if (moviealpha <= 0)
		{
			moviealpha = 0;
			run = false;
			stop = false;
		}
	}
}

void cMovieEvent::Scene_Render1()
{
}

void cMovieEvent::Scene_RenderSprite()
{
	if (run)//<-영화이벤트
	{
		RECT rc = { 0,0,1500,1000 };
		SPRITE_MGR->DrawTexture(this->movievent, &rc, -100, 0, 1.0f, 0.8f, NULL, D3DCOLOR_ARGB(this->moviealpha, 255, 255, 255), &D3DXVECTOR3(0, 0, 0));
	}
}
