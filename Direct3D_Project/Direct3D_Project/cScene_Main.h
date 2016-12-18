#pragma once
#include "cScene.h"

class cScene_Main : public cScene
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
	RECT rctemp;
	int black;
	int back;
	int count;
	int logoalpha;
	bool isoption;

public:
	cScene_Main();
	~cScene_Main();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);

	virtual void Scene_Render1();
	virtual void Scene_RenderSprite();
};

