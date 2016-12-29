#pragma once

#include <fstream>

struct tagFileBool
{
	bool is_1;
	bool is_2;
	bool is_3;
	bool is_4;
	bool is_5;
	int count;
};

class cFileRead
{
private:
	LPDIRECT3DTEXTURE9 file_01;
	LPDIRECT3DTEXTURE9 file_02;
	LPDIRECT3DTEXTURE9 file_03;
	LPDIRECT3DTEXTURE9 file_04;
	LPDIRECT3DTEXTURE9 file_05;
	bool read[5];
	int letterx;
	int lettery;
	int  letteralpha;
	fstream file;

	tagFileBool File;
public:
	cFileRead();
	~cFileRead();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);
	virtual void Scene_Render1();
	virtual void Scene_RenderSprite();

	void loadInfo(LPDIRECT3DTEXTURE9 name);

	tagFileBool* GetFileInfo() { return &File; }
};
