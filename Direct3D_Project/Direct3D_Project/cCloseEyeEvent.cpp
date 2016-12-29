#include "stdafx.h"
#include "cCloseEyeEvent.h"
#include "cScene.h"
#include "Unit.h"

cCloseEyeEvent::cCloseEyeEvent()
{
}


cCloseEyeEvent::~cCloseEyeEvent()
{
}

HRESULT cCloseEyeEvent::Scene_Init()
{
	//페이드
	this->upeye = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/upeye.png");
	this->downeye = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/downeye.png");
	upeyeplaceY = -795;
	downeyeplaceY = 795;
	event[20] = false;
	eyealpha = 255;
	eventstart = false;
	count = 0;
	EyeCloseCount = 1;
	Speed = 5;
	isPlayEye = false;
	eyeup = false;

	return S_OK;
}

void cCloseEyeEvent::Scene_Release()
{
}

void cCloseEyeEvent::Scene_Update(float timeDelta)
{
	//페이드
	if (isPlayEye&&count <1)
	{
		event[0] = true;
		eventstart = true;
		upeyeplaceY = -795;
		downeyeplaceY = 795;
		count = 1;
	}
	SetCloseTime();
	
}

void cCloseEyeEvent::Scene_Render1()
{
}

void cCloseEyeEvent::Scene_RenderSprite()
{
	if (eventstart)//<--페이드
	{
		RECT rc = { 0,0,1500,1000 };
		SPRITE_MGR->DrawTexture(this->upeye, &rc, -100, upeyeplaceY, 1.0f, 0.8f, NULL, D3DCOLOR_ARGB(255, 255, 255, 255), &D3DXVECTOR3(0, 0, 0));
		RECT rc2 = { 0,0,1500,1000 };
		SPRITE_MGR->DrawTexture(this->downeye, &rc2, -100, downeyeplaceY, 1.0f, 0.8f, NULL, D3DCOLOR_ARGB(255, 255, 255, 255), &D3DXVECTOR3(0, 0, 0));
	}
}

void cCloseEyeEvent::SetCloseTime()
{
	for (int i = 0; i < EyeCloseCount; i++)
	{

		if (event[i])
		{

			if (!eyeup)
			{
				if (upeyeplaceY < -20)
				{
					upeyeplaceY += Speed;
				}
				if (downeyeplaceY > 20)
				{
					downeyeplaceY -= Speed;
				}
				if (upeyeplaceY >= -20 && downeyeplaceY <= 20)
				{
					eyeup = true;
					if (m_CurUnit->IsDead())
					{
						SCENE_MGR->ChangeScene("title");
						return;
					}
				}
			}
			else if (eyeup == true)
			{
				if (upeyeplaceY >= -800)
				{
					upeyeplaceY -= Speed;
				}
				if (downeyeplaceY <= 800)
				{
					downeyeplaceY += Speed;
				}
				if (upeyeplaceY < -800 && downeyeplaceY > 800)
				{
					isPlayEye = false;
					eyeup = false;
					count = 0;
					event[i + 1] = true;
					event[i] = false;

				}
			}
		}

	}
}
