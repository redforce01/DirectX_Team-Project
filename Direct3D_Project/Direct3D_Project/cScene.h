#pragma once

class cCamera;
class cLight_Direction;
class cTerrain;

class cScene
{
private:
	typedef struct SCENE_VERTEX{
		D3DXVECTOR3 pos;
		D3DXVECTOR2 uv;
		enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
	}SCENE_VERTEX, *LPSCENE_VETEX;


protected:
	cCamera*						pMainCamera;
	LPDIRECT3DCUBETEXTURE9			evironmentTexture;			//ȯ�� Texture
	LPD3DXMESH						evironmemtSphereMesh;		//ȯ�� ��
	LPD3DXEFFECT					evironmentEffect;			//ȯ�� Effect

	SCENE_VERTEX					scenePlaneVertex[4];		//���÷� ���ؽ�
	WORD							scenePlaneIndex[6];			//�ε���

	LPD3DXEFFECT					postEffect;					//PostEffect

	cLight_Direction*				pSceneBaseDirectionLight;	//���� ��ġ�� �⺻������
	cCamera*						pDirectionLightCamera;		//���⼺ ������ ���� ī�޶�...

	float							shadowDistance;				//�׸��� �Ÿ�

public:
	cScene( void );
	virtual ~cScene( void );

	HRESULT Init();
	void Release();
	void Update(float timeDelta);
	void Render();

	void SetEnvironment(std::string cubeFilePath);

	void ReadyShadowMap(std::vector<cBaseObject*>* renderObjects, cTerrain* pTerrain = NULL);

	//���� ī�޶��� RenderToTexture �� ������Ʈ�Ѵ�.
	void RenderToMainCamTexture();

	//����ī�޶��� ���� Texture �� ��´�.
	LPDIRECT3DTEXTURE9 GetTexture();

private:

	//������ ȣ��� �Լ����� �߻��Լ���...
	virtual HRESULT Scene_Init() = 0;
	virtual void Scene_Release() = 0;
	virtual void Scene_Update(float timeDelta) = 0;

	virtual void Scene_Render0(){}
	virtual void Scene_Render1() = 0;
	virtual void Scene_Render2(){}

	virtual void Scene_RenderSprite(){}

	//ȯ�汸 ����
	void RenderEnvironment();
};

