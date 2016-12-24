#include "stdafx.h"
#include "cEffect.h"
#include "cCamera.h"

cEffect::cEffect()
{
}


cEffect::~cEffect()
{
}

HRESULT cEffect::Scene_Init()
{
	//피
	this->blood_Tex = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/blad.png");
	this->blood_hit = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/bloodeffect2.png");
	this->RED4 = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/RED/RED4.png");


	blood1 = false;
	blood2 = false;
	hit_blood = false;
	blood = 0;
	hitalpha = 255;
	redalpha = 10;
	hpdown = false;
	hpup = false;
	count = 0;
	requcount = 0;
	this->hp = 4;
	this->x = 0;
	this->y = 0;
	this->red[0] = false;
	this->red[1] = false;
	this->red[2] = false;
	return S_OK;
}

void cEffect::Scene_Release()
{
}

void cEffect::Scene_Update(float timDelta)
{

}

void cEffect::Scene_Render1()
{
}

void cEffect::Scene_RenderSprite()
{
	if (red[0])
	{
		RECT rc3 = { 0,0,1200,1600 };
		SPRITE_MGR->DrawTexture(this->RED4, &rc3, -100, -350, 1.5f, 1.4f, NULL, D3DCOLOR_ARGB(this->redalpha, 255, 0, 0), &D3DXVECTOR3(0, 0, 0));

		RECT rc = { 0, 0, 1000, 800 };
		SPRITE_MGR->DrawTexture(this->blood_Tex, &rc, -350, -100, 0.8f, 1.2f, NULL, D3DCOLOR_ARGB(this->redalpha, 255, 0, 0), &D3DXVECTOR3(0, 0, 0));
	}
	if (drw)
	{
		RECT rc = { 0,0,240,255 };
		SPRITE_MGR->DrawTexture(this->blood_hit, &rc, this->x, this->y, D3DCOLOR_ARGB(this->hitalpha, 255, 0, 0), &D3DXVECTOR3(0, 0, 0));
	}
}

void cEffect::Effect(float timeDelta, cCamera * camera)
{
	camera->ShakeUpdate(timeDelta);//<--카메라 업데이트(무조건 써야함)
	if (isHitPly)//<-체력 감소
	{

		this->hp -= 1;
		drw = true;
		hpup = false;
		hpdown = true;
		camera->ShakePos(0.15f, 0.15f);//<--피격시 카메라흔들림
		count = 90;
		requcount = 50;
		isHitPly = false;
	}
	if (count <= 90)
	{
		count -= 1;
	}
	if (count <= 0)
	{
		hpdown = false;
		hpup = true;
		count = 0;
	}
	if (hpup)
	{
		if (requcount <= 50)
		{
			requcount -= 1;
			if (requcount == 0)
			{
				if (hp <= 3)
				{
					hp += 1;
				}
				requcount = 50;
			}
			if (hp >= 2)
			{
				if (redalpha >= 150)
				{
					redalpha -= 3;
				}
			}
			if (hp >= 3)
			{
				if (redalpha >= 90)
				{
					redalpha -= 3;
				}

			}
			if (hp >= 4)
			{
				if (redalpha >= 0)
				{
					redalpha -= 3;

				}
			}
			if (redalpha <= 0)
			{
				redalpha = 0;
				red[0] = false;
			}
		}
	}
	//if (KEY_MGR->IsOnceDown('6'))//<-체력증가
	//{
	//	this->hp += 1;
	//	drw = true;
	//}
	if (x == NULL&& y == NULL)
	{
		this->x = rand() % 600;
		this->y = rand() % 350;
	}


	if (hpdown)
	{
		if (hp <= 3)
		{
			blood = 160;
			red[0] = true;

			if (red[0])
			{
				if (this->redalpha < 50)
				{
					this->redalpha += 3;
				}
			}
			if (hp <= 2)
			{
				if (this->redalpha < 150)
				{
					redalpha += 3;
				}

				if (hp <= 1)
				{
					if (this->redalpha < 210)
					{
						redalpha += 3;
					}
				}
			}
		}
	}

	if (hitalpha == 0)
	{
		drw = false;
		hitalpha = 255;
		x = 0;
		y = 0;
	}

	if (drw)
	{
		hitalpha -= 3;
	}
}
