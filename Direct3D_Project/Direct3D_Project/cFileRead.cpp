#include "stdafx.h"
#include "cFileRead.h"


cFileRead::cFileRead()
{
}


cFileRead::~cFileRead()
{
}

HRESULT cFileRead::Scene_Init()
{
	this->file_01 = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/file_01.png");
	this->file_02 = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/file_02.png");
	this->file_03 = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/file_03.png");
	this->file_04 = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/file_04.png");
	this->file_05 = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/file_05.png");
	//파일입출력
	this->read[5] = false;
	letterx = 1300;
	lettery = 50;
	letteralpha = 255;

	File.is_1 = false;
	File.is_2 = false;
	File.is_3 = false;
	File.is_4 = false;
	File.is_5 = false;
	File.count = 0;

	return S_OK;
}

void cFileRead::Scene_Release()
{
}

void cFileRead::Scene_Update(float timeDelta)
{
	////////////////////////////////////////////////////////////////////////
	if (File.is_1)
		read[0] = true;
	if (File.is_2)
		read[1] = true;
	if (File.is_3)
		read[2] = true;
	if (File.is_4)
		read[3] = true;
	if (File.is_5)
		read[4] = true;

	if (read[0] == true)
	{
		if (letterx >= 800)
		{
			letterx -= 100;
		}
		if (letterx >= 600)
		{
			letterx -= 40;
		}
		if (letterx >= 450)
		{
			letterx -= 30;
		}
		if (letterx <= 450)
		{
			if (letteralpha >= 180)
			{
				letteralpha -= 5;
			}
		}
	}
	if (read[1] == true)
	{

		if (letterx >= 800)
		{
			letterx -= 100;
		}
		if (letterx >= 600)
		{
			letterx -= 40;
		}
		if (letterx >= 450)
		{
			letterx -= 30;
		}
		if (letterx <= 450)
		{
			if (letteralpha >= 180)
			{
				letteralpha -= 5;
			}
		}
	}
	if (read[2] == true)
	{

		if (letterx >= 800)
		{
			letterx -= 100;
		}
		if (letterx >= 600)
		{
			letterx -= 40;
		}
		if (letterx >= 450)
		{
			letterx -= 30;
		}
		if (letterx <= 450)
		{
			if (letteralpha >= 180)
			{
				letteralpha -= 5;
			}
		}
	}
	if (read[3] == true)
	{

		if (letterx >= 800)
		{
			letterx -= 100;
		}
		if (letterx >= 600)
		{
			letterx -= 40;
		}
		if (letterx >= 450)
		{
			letterx -= 30;
		}
		if (letterx <= 450)
		{
			if (letteralpha >= 180)
			{
				letteralpha -= 5;
			}
		}
	}
	if (read[4] == true)
	{

		if (letterx >= 800)
		{
			letterx -= 100;
		}
		if (letterx >= 600)
		{
			letterx -= 40;
		}
		if (letterx >= 450)
		{
			letterx -= 30;
		}
		if (letterx <= 450)
		{
			if (letteralpha >= 180)
			{
				letteralpha -= 5;
			}
		}
	}



	if (File.count == 2)
	{
		File.is_1 = File.is_2 = File.is_3 = File.is_4 = File.is_5 = false;
		read[0] = read[1] = read[2] = read[3] = read[4] = false;
		letterx = 1300;
		letteralpha = 255;
		File.count = 0;
	}
}

void cFileRead::Scene_Render1()
{

}

void cFileRead::Scene_RenderSprite()
{
	if (File.is_1)
		loadInfo(file_01);//loadInfo("텍스트파일이름.txt");텍스트는 무조건 한줄로.

	if (File.is_2)
		loadInfo(file_02);//loadInfo("텍스트파일이름.txt");텍스트는 무조건 한줄로.

	if (File.is_3)
		loadInfo(file_03);//loadInfo("텍스트파일이름.txt");텍스트는 무조건 한줄로.

	if (File.is_4)
		loadInfo(file_04);//loadInfo("텍스트파일이름.txt");텍스트는 무조건 한줄로.

	if (File.is_5)
		loadInfo(file_05);//loadInfo("텍스트파일이름.txt");텍스트는 무조건 한줄로.
}

void cFileRead::loadInfo(LPDIRECT3DTEXTURE9 name)
{

	RECT rc = { 0,0,500,1200 };
	SPRITE_MGR->DrawTexture(name, &rc, letterx, lettery, 0.9f, 0.5f, NULL, D3DCOLOR_ARGB(/*letteralpha*/255, 255, 255, 255), &D3DXVECTOR3(0, 0, 0));

}