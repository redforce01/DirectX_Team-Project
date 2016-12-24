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

	//파일입출력
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
	//편지
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
	 //편지
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
		loadInfo("text/help.txt");//loadInfo("텍스트파일이름.txt");텍스트는 무조건 한줄로.

	if (File.is_2)
		loadInfo("text/psycho.txt");//loadInfo("텍스트파일이름.txt");텍스트는 무조건 한줄로.

	if (File.is_3)
		loadInfo("text/thesis.txt");//loadInfo("텍스트파일이름.txt");텍스트는 무조건 한줄로.

	if (File.is_4)
		loadInfo("text/hint.txt");//loadInfo("텍스트파일이름.txt");텍스트는 무조건 한줄로.
}

void cFileRead::Scene_RenderSprite()
{
}

void cFileRead::loadInfo(const char * name)
{
	LPD3DXFONT pxfont;
	D3DXCreateFont(
		Device,				//디바이스
		24.f,				//폰트의 높이
		0.f,				//폰트의 폭(이 값이 0이면 앞에서 설정한 폰트 높이에 비례)
		FW_BOLD,			//폰트의 두께 설정(FW_BOLD 폰트의 BOLD 스타일 크기를 갖는다)
		1,					//밉맵(그냥 1)
		TRUE,				//이텔릭 여부
		DEFAULT_CHARSET,	//캐릭터 set( 그냥 DEFAULT_CHARSET )
		OUT_DEFAULT_PRECIS,	//외각 처리방식 ( 그냥 OUT_DEFAULT_PRECIS )
		DEFAULT_QUALITY,	//퀄리티 ( 그냥 DEFAULT_QUALITY )
		DEFAULT_PITCH | FF_DONTCARE,// 글씨체 패밀리셋 설정
		"궁서체",			//사용될 폰트 이름 
		&pxfont			//생성된 폰트객체 받을 더블 포인터
	);
	FILE* file = fopen(name, "r");


	//파일 끝까지 읽는다.

	char s[2048] = { 0 };
	string line;
	fgets(s, sizeof(s), file);
	//file 한줄 문자열을 읽어 line에 대입


	fclose(file); //다쓴 파일 스트림 닫는다.


				  //위치
	RECT rc = { 0,0,500,500 };
	SPRITE_MGR->DrawTexture(this->letter, &rc, letterx, lettery, 1.f, 1.3f, NULL, D3DCOLOR_ARGB(/*letteralpha*/255, 255, 255, 255), &D3DXVECTOR3(0, 0, 0));


	RECT rcText2 = { letterx + 20, lettery + 50, letterx + 500, lettery + 800 };
	pxfont->DrawText(
		NULL,				//스프라이트에 그리는 경우 스프라이트 객체
		s,			//들어갈 문자열
		-1,					//출력 문자열 갯수 -1이면 다 찍어라
		&rcText2,			//출력 영역
		DT_LEFT | DT_WORDBREAK,//왼쪽정렬, DT_NOCLIP하면 영역을 무시하고 다 출력
		D3DCOLOR_ARGB(255, 0, 0, 0)			//문자열 색상
	);
}
