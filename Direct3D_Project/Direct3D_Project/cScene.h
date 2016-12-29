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

	LPDIRECT3DCUBETEXTURE9			evironmentTexture;			//환경 Texture
	LPD3DXMESH						evironmemtSphereMesh;		//환경 구
	LPD3DXEFFECT					evironmentEffect;			//환경 Effect

	SCENE_VERTEX					scenePlaneVertex[4];		//씬플랜 버텍스
	WORD							scenePlaneIndex[6];			//인덱스

	LPD3DXEFFECT					postEffect;					//PostEffect

	cLight_Direction*				pSceneBaseDirectionLight;	//씬에 배치된 기본라이팅
	cCamera*						pDirectionLightCamera;		//방향성 광원에 따른 카메라...
	Ray			camRay;
	float							shadowDistance;				//그림자 거리

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
	//추가
	//
	virtual void SetGameOver(bool TF) { m_isGameOver = TF; }
	virtual void DeadEvent() {} ;
	//메인 카메라의 RenderToTexture 만 업데이트한다.
	void RenderToMainCamTexture();

	virtual void SetCamFov(float fov) {};
	virtual float GetCamFov() { return 0; };
	//메인카메라의 랜더 Texture 를 얻는다.
	LPDIRECT3DTEXTURE9 GetTexture();





private:

	//씬에서 호출될 함수들을 추상함수로...
	virtual HRESULT Scene_Init() = 0;
	virtual void Scene_Release() = 0;
	virtual void Scene_Update(float timDelta) = 0;

	virtual void Scene_Render0() {}
	virtual void Scene_Render1() = 0;
	virtual void Scene_Render2() {}

	virtual void Scene_RenderSprite() {}

	//환경구 랜더
	void RenderEnvironment();
	void ChangeCameraMod();
	void ChangeCameraMod(int IDx);
};

