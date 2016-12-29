#pragma once

class cCamera;
class cLight_Direction;
class cTerrain;
class cNode;

class cScene
{
private:
	typedef struct SCENE_VERTEX {
		D3DXVECTOR3 pos;
		D3DXVECTOR2 uv;
		enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
	}SCENE_VERTEX, *LPSCENE_VETEX;


protected:

	bool m_isGameOver;
	LPDIRECT3DTEXTURE9 pTexScreenDiff;
	LPDIRECT3DTEXTURE9 pTexScreenNorm;
	LPDIRECT3DTEXTURE9 pTexScreenNoise;

	vector <cNode*> m_vNode;
	vector <cCamera*>           vCamera;
	vector <cCamera*>::iterator viCamera;
	cCamera*						pMainCamera;

	LPDIRECT3DCUBETEXTURE9			evironmentTexture;			//ȯ�� Texture
	LPD3DXMESH						evironmemtSphereMesh;		//ȯ�� ��
	LPD3DXEFFECT					evironmentEffect;			//ȯ�� Effect

	SCENE_VERTEX					scenePlaneVertex[4];		//���÷� ���ؽ�
	WORD							scenePlaneIndex[6];			//�ε���

	LPD3DXEFFECT					postEffect;					//PostEffect

	cLight_Direction*				pSceneBaseDirectionLight;	//���� ��ġ�� �⺻������
	cCamera*						pDirectionLightCamera;		//���⼺ ������ ���� ī�޶�...
	Ray			camRay;
	float							shadowDistance;				//�׸��� �Ÿ�

public:
	bool isEnding = false;
	cScene(void);
	virtual ~cScene(void);

	HRESULT Init();
	void Release();
	void Update(float timeDelta);
	void Render();
	vector <cNode*> getVNode() { return m_vNode; }
	void SetEnvironment(std::string cubeFilePath);
	virtual void CloseEye(int num) {};
	void ReadyShadowMap(std::vector<cBaseObject*>* renderObjects, cTerrain* pTerrain = NULL);


	virtual void InnerOpenDoor(int Idx) {};
	virtual void OutterOpenDoor(int Idx) {};
	virtual void CloseDoor(int Idx) {};

	virtual void ControllNightVision() {};
	virtual void ControllCamMode() {};

	virtual void Event1Start() {};
	virtual void Event1End() {};
	virtual void setRay(Ray ray) { camRay = ray; }
	//
	//�߰�
	//
	virtual void SetGameOver(bool TF) { m_isGameOver = TF; }
	virtual void DeadEvent() {} ;
	//���� ī�޶��� RenderToTexture �� ������Ʈ�Ѵ�.
	void RenderToMainCamTexture();

	virtual void SetCamFov(float fov) {};
	virtual float GetCamFov() { return 0; };
	//����ī�޶��� ���� Texture �� ��´�.
	LPDIRECT3DTEXTURE9 GetTexture();





private:

	//������ ȣ��� �Լ����� �߻��Լ���...
	virtual HRESULT Scene_Init() = 0;
	virtual void Scene_Release() = 0;
	virtual void Scene_Update(float timDelta) = 0;

	virtual void Scene_Render0() {}
	virtual void Scene_Render1() = 0;
	virtual void Scene_Render2() {}

	virtual void Scene_RenderSprite() {}

	//ȯ�汸 ����
	void RenderEnvironment();
	void ChangeCameraMod();
	void ChangeCameraMod(int IDx);
};

