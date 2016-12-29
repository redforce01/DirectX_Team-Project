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
	std::vector<cBaseObject*>	renderObjects;	//���� ��ġ�� ���� ������Ʈ �迭
	std::vector<cBaseObject*>	cullObjects;	//�ø��� ������Ʈ

	//�߰� ī�޶�
	cCamera*					pExtraCam;

	LPDIRECT3DTEXTURE9			pRenderTexture;		//�������� Texture
	LPDIRECT3DSURFACE9			pRenderSurface;		//���� Texture Deapth ���ۿ� Stencil ���۰� �ִ� Surface

public:
	cScene_01_RenderToTexture();
	~cScene_01_RenderToTexture();

	virtual HRESULT Scene_Init();

	virtual void Scene_Release();

	virtual void Scene_Update(float timeDelta);

	virtual void Scene_Render1();

	virtual void Scene_RenderSprite();
};

