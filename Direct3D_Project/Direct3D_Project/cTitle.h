#pragma once

#include "cscene.h"

class cTitle : public cScene
{
private:
	LPDIRECT3DTEXTURE9 main;
	LPDIRECT3DTEXTURE9 mouse;
	LPDIRECT3DTEXTURE9 NewGame;
	LPDIRECT3DTEXTURE9 Option;
	LPDIRECT3DTEXTURE9 Credits;
	LPDIRECT3DTEXTURE9 Exit;
	LPDIRECT3DTEXTURE9 background;
	LPDIRECT3DTEXTURE9 logo;
	LPDIRECT3DTEXTURE9 Sound;
	LPDIRECT3DTEXTURE9 Soundcircle;


	//Texture Rc ¸ðÀ½
	RECT mainpicture = { 0,0,WINSIZE_X ,500 };
	RECT backg = { 0,0,WINSIZE_X + 100 ,600 };
	RECT mouserect = { 0,0,35,60 };
	RECT newgameRect = { 0,0,257,33 };
	RECT optionRect = { 0,0,257,33 };
	RECT optioncoliRect = { 0,0,257,33 };
	RECT creditRect = { 0,0,257,33 };
	RECT exitRect = { 0,0,130,33 };
	RECT exitcoliRect;
	RECT BREA = { 0,0,35,60 };
	RECT Logo = { 0,0,257,120 };
	RECT soundRect = { 0,0,500,400 };
	RECT soundcircleRect = { 0,0,500,400 };
	RECT newgamecoliRect = { 0,0,257,33 };

	RECT rctemp;
	int black;
	int back;
	int count;
	int logoalpha;
	bool isoption;

	
public:
	cTitle();
	~cTitle();

	HRESULT Scene_Init();

	void Scene_Release();

	void Scene_Update(float timDelta);

	void Scene_Render1();


	void Scene_RenderSprite();
};

