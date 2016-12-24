#include "stdafx.h"
#include "cCloseEyeEvent.h"


cCloseEyeEvent::cCloseEyeEvent()
{
}


cCloseEyeEvent::~cCloseEyeEvent()
{
}

HRESULT cCloseEyeEvent::Scene_Init()
{
	//���̵�
	this->upeye = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/upeye.png");
	this->downeye = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/downeye.png");
	upeyeplaceY = -795;
	downeyeplaceY = 795;
	event[20] = false;
	eyealpha = 255;
	eventstart = false;
	count = 0;
	isPlayEye = false;
	return S_OK;
}

void cCloseEyeEvent::Scene_Release()
{
}

void cCloseEyeEvent::Scene_Update(float timeDelta)
{
	//���̵�
	if (isPlayEye&&count <1 )
	{
		event[0] = true;
		eventstart = true;
		upeyeplaceY = -795;
		downeyeplaceY = 795;
		count = 1;
	}
	
	if (event[0] == true)
	{
		if (upeyeplaceY < 0)//<-��ġ��
			upeyeplaceY += 15;//<-�ӵ�
		if (downeyeplaceY > 0)//<-��ġ��
			downeyeplaceY -= 15;//<-�ӵ�
		if (upeyeplaceY == 0 && downeyeplaceY == 0)//<-��ġ��
		{
			event[1] = true;
		}
	}
	if (event[1] == true)
	{
		event[0] = false;
		if (upeyeplaceY <= 0 && upeyeplaceY > -495)//<-��ġ��
			upeyeplaceY -= 15;//<-�ӵ�
		if (downeyeplaceY >= 0 && downeyeplaceY < 495)//<-��ġ��
			downeyeplaceY += 15;//<-�ӵ�
		if (upeyeplaceY == -495 && downeyeplaceY == 495)//<-��ġ��
		{
			event[2] = true;
		}
	}
	if (event[2] == true)
	{
		event[1] = false;
		if (upeyeplaceY >= -495 && upeyeplaceY <-90)//<-��ġ��
		{
			upeyeplaceY += 15;//<-�ӵ�
		}
		if (downeyeplaceY <= 495 && downeyeplaceY >90)//<-��ġ��
		{
			downeyeplaceY -= 15;//<-�ӵ�
		}
		if (upeyeplaceY == -90 && downeyeplaceY == 90)//<-��ġ��
		{
			event[3] = true;

		}
	}
	if (event[3] == true)
	{
		event[2] = false;
		if (upeyeplaceY > -390 && upeyeplaceY <= -90)//<-��ġ��
		{
			upeyeplaceY -= 15;//<-�ӵ�
		}
		if (downeyeplaceY < 390 && downeyeplaceY >= 90)//<-��ġ��
		{
			downeyeplaceY += 15;//<-�ӵ�
		}
		if (upeyeplaceY == -390 && downeyeplaceY == 390)//<-��ġ��
		{
			event[4] = true;//<-�ӵ�

		}
	}
	if (event[4] == true)
	{
		event[3] = false;
		if (upeyeplaceY >= -390 && upeyeplaceY < 0)//<-��ġ��
		{
			upeyeplaceY += 15;//<-�ӵ�
		}
		if (downeyeplaceY <= 390 && downeyeplaceY > 0)//<-��ġ��
		{
			downeyeplaceY -= 15;//<-�ӵ�
		}
		if (upeyeplaceY == 0 && downeyeplaceY == 0)//<-��ġ��
		{
			event[5] = true;
		}
	}
	if (event[5] == true)
	{
		event[4] = false;
		if (upeyeplaceY <= 0 && upeyeplaceY > -795)//<-��ġ��
		{
			upeyeplaceY -= 15;//<-�ӵ�
		}
		if (downeyeplaceY >= 0 && downeyeplaceY < 795)//<-��ġ��
		{
			downeyeplaceY += 15;//<-�ӵ�
		}
		if (upeyeplaceY == -795 && downeyeplaceY == 795)//<-��ġ��
		{
			event[5] = false;
			eventstart = false;
			isPlayEye = false;
			count = 0;
		}
	}
}

void cCloseEyeEvent::Scene_Render1()
{
}

void cCloseEyeEvent::Scene_RenderSprite()
{
	if (eventstart)//<--���̵�
	{
		RECT rc = { 0,0,1500,1000 };
		SPRITE_MGR->DrawTexture(this->upeye, &rc, -100, upeyeplaceY, 1.0f, 0.8f, NULL, D3DCOLOR_ARGB(255, 255, 255, 255), &D3DXVECTOR3(0, 0, 0));
		RECT rc2 = { 0,0,1500,1000 };
		SPRITE_MGR->DrawTexture(this->downeye, &rc2, -100, downeyeplaceY, 1.0f, 0.8f, NULL, D3DCOLOR_ARGB(255, 255, 255, 255), &D3DXVECTOR3(0, 0, 0));
	}
}
