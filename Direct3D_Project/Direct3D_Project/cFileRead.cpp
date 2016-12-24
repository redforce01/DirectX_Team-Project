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
	this->letter = RESOURCE_TEXTURE->GetResource("../Resources/Ui_Effect/letter3.png");

	//���������
	this->read[4] = false;
	letterx = 1300;
	lettery = 50;
	letteralpha = 255;

	File.is_1 = false;
	File.is_2 = false;
	File.is_3 = false;
	File.is_4 = false;
	File.count = 0;

	return S_OK;
}

void cFileRead::Scene_Release()
{
}

void cFileRead::Scene_Update(float timeDelta)
{
	//����
	/*if (read == false)
	{
		if (KEY_MGR->IsOnceDown('R'))
		{
			read = true;
		}
	}*/
	// if (File.is_1|| File.is_2||
	//	 File.is_3|| File.is_4)
	//{

	//	if (letterx >= 800)
	//	{
	//		letterx -= 100;
	//	}
	//	if (letterx >= 600)
	//	{
	//		letterx -= 40;
	//	}
	//	if (letterx >= 450)
	//	{
	//		letterx -= 30;
	//	}
	//	if (letterx <= 450)
	//	{
	//		if (letteralpha >= 180)
	//		{
	//			letteralpha -= 5;
	//		}
	//	}
	///*	if (KEY_MGR->IsOnceDown('R'))
	//	{
	//		letterx = 1300;
	//		letteralpha = 255;
	//		read = false;
	//	}*/
	//}
	

	/*File.is_1 || File.is_2 ||
		File.is_3 || File.is_4)*/

	if (File.is_1)
		read[0] = true;
	if (File.is_2)
		read[1] = true;
	if (File.is_3)
		read[2] = true;
	if (File.is_4)
		read[3] = true;

	 ////////////////////////////////////////////////////////////////////////
	 //����
	/* if (read[0] == false)
	 {
		 if (KEY_MGR->IsOnceDown('R'))
		 {
			 read[0] = true;
		 }
	 }*/
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
		/* if (KEY_MGR->IsOnceDown('R'))
		 {
			 letterx = 1300;
			 letteralpha = 255;
			 read[0] = false;
		 }*/
	 }
	 /*if (read[1] == false)
	 {
		 if (KEY_MGR->IsOnceDown('T'))
		 {
			 read[1] = true;
		 }
	 }*/
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
		/* if (KEY_MGR->IsOnceDown('T'))
		 {
			 letterx = 1300;
			 letteralpha = 255;
			 read[1] = false;
		 }*/
	 }
	/* if (read[2] == false)
	 {
		 if (KEY_MGR->IsOnceDown('Y'))
		 {
			 read[2] = true;
		 }
	 }*/
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
		/* if (KEY_MGR->IsOnceDown('Y'))
		 {
			 letterx = 1300;
			 letteralpha = 255;
			 read[2] = false;
		 }*/
	 }

	/* if (read[3] == false)
	 {
		 if (KEY_MGR->IsOnceDown('U'))
		 {
			 read[3] = true;
		 }
	 }*/
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

	 if (File.count == 2)
	 {
		 File.is_1 = File.is_2 = File.is_3 = File.is_4 = false;
		 read[0] = read[1] = read[2] = read[3] = false;
		 letterx = 1300;
		 letteralpha = 255;
		 File.count = 0;
	 }
	
}

void cFileRead::Scene_Render1()
{
	if (File.is_1)
		loadInfo("text/help.txt");//loadInfo("�ؽ�Ʈ�����̸�.txt");�ؽ�Ʈ�� ������ ���ٷ�.

	if (File.is_2)
		loadInfo("text/psycho.txt");//loadInfo("�ؽ�Ʈ�����̸�.txt");�ؽ�Ʈ�� ������ ���ٷ�.

	if (File.is_3)
		loadInfo("text/thesis.txt");//loadInfo("�ؽ�Ʈ�����̸�.txt");�ؽ�Ʈ�� ������ ���ٷ�.

	if (File.is_4)
		loadInfo("text/hint.txt");//loadInfo("�ؽ�Ʈ�����̸�.txt");�ؽ�Ʈ�� ������ ���ٷ�.
}

void cFileRead::Scene_RenderSprite()
{
}

void cFileRead::loadInfo(const char * name)
{
	LPD3DXFONT pxfont;
	D3DXCreateFont(
		Device,				//����̽�
		24.f,				//��Ʈ�� ����
		0.f,				//��Ʈ�� ��(�� ���� 0�̸� �տ��� ������ ��Ʈ ���̿� ���)
		FW_BOLD,			//��Ʈ�� �β� ����(FW_BOLD ��Ʈ�� BOLD ��Ÿ�� ũ�⸦ ���´�)
		1,					//�Ӹ�(�׳� 1)
		TRUE,				//���ڸ� ����
		DEFAULT_CHARSET,	//ĳ���� set( �׳� DEFAULT_CHARSET )
		OUT_DEFAULT_PRECIS,	//�ܰ� ó����� ( �׳� OUT_DEFAULT_PRECIS )
		DEFAULT_QUALITY,	//����Ƽ ( �׳� DEFAULT_QUALITY )
		DEFAULT_PITCH | FF_DONTCARE,// �۾�ü �йи��� ����
		"�ü�ü",			//���� ��Ʈ �̸� 
		&pxfont			//������ ��Ʈ��ü ���� ���� ������
	);
	FILE* file = fopen(name, "r");


	//���� ������ �д´�.

	char s[2048] = { 0 };
	string line;
	fgets(s, sizeof(s), file);
	//file ���� ���ڿ��� �о� line�� ����


	fclose(file); //�پ� ���� ��Ʈ�� �ݴ´�.


				  //��ġ
	RECT rc = { 0,0,500,500 };
	SPRITE_MGR->DrawTexture(this->letter, &rc, letterx, lettery, 1.f, 1.3f, NULL, D3DCOLOR_ARGB(/*letteralpha*/255, 255, 255, 255), &D3DXVECTOR3(0, 0, 0));


	RECT rcText2 = { letterx + 20, lettery + 50, letterx + 500, lettery + 800 };
	pxfont->DrawText(
		NULL,				//��������Ʈ�� �׸��� ��� ��������Ʈ ��ü
		s,			//�� ���ڿ�
		-1,					//��� ���ڿ� ���� -1�̸� �� ����
		&rcText2,			//��� ����
		DT_LEFT | DT_WORDBREAK,//��������, DT_NOCLIP�ϸ� ������ �����ϰ� �� ���
		D3DCOLOR_ARGB(255, 0, 0, 0)			//���ڿ� ����
	);
}
