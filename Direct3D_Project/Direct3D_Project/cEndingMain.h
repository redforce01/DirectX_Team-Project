#pragma once
#include "cScene.h"
#include "cTown.h"
#include "cRainParticle.h"
#include "cMovieEvent.h"

class cCamera;
class cLight;
class Unit;

enum SCENES
{
	LOOK_CHURCH,
	LOOK_CHURCH_UP,
	LOOK_CHURCH_DOWN,
	LOOK_BACK,
	LOOK_FORWARD,
	MILES_WALK,
	LOOK_SMILE,
	LOOK_END,
};

class cEndingMain : public cScene
{
private:
	cTown* town;
	cRainParticle * rain;
	Unit* Miles;
	std::vector<cLight*> lights;
	cMovieEvent* movieEvent;
	SCENES scene;
	LPDIRECT3DTEXTURE9 EndingLogo;

	bool isLoding;
	bool isEndinLogo;
	bool isSmileSound;
	bool isScreamSound;
	int count;
	int Soundcount;
public:
	
	cEndingMain();
	~cEndingMain();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);
	virtual void Scene_Render1();
	virtual void Scene_RenderSprite();

	void EndingMove(float TimeDelta);

};