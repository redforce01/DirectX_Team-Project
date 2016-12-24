#pragma once

#include "cScene.h"
#include <fstream>

struct tagFileBool
{
	bool is_1;
	bool is_2;
	bool is_3;
	bool is_4;
	int count;
};

class cFileRead : public cScene
{
private:
	LPDIRECT3DTEXTURE9 letter;
	bool read[4];
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

	void loadInfo(const char* name);

	tagFileBool* GetFileInfo() { return &File; }
};

