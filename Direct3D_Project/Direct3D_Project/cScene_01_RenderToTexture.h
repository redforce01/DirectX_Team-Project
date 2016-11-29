#pragma once

#include "cScene.h"

class cBaseObject;
class cCamera;
class cLight;

#define OBJECT_NUM 10

class cScene_01_RenderToTexture : public cScene
{
private:
	std::vector<cLight*>		lights;
	std::vector<cBaseObject*>	renderObjects;	//씬에 배치된 랜더 오브젝트 배열
	std::vector<cBaseObject*>	cullObjects;	//컬링된 오브젝트

	//추가 카메라
	cCamera*					pExtraCam;

	LPDIRECT3DTEXTURE9			pRenderTexture;		//랜더링될 Texture
	LPDIRECT3DSURFACE9			pRenderSurface;		//랜더 Texture Deapth 버퍼와 Stencil 버퍼가 있는 Surface

public:
	cScene_01_RenderToTexture();
	~cScene_01_RenderToTexture();

	virtual HRESULT Scene_Init();

	virtual void Scene_Release();

	virtual void Scene_Update(float timeDelta);

	virtual void Scene_Render1();

	virtual void Scene_RenderSprite();
};

