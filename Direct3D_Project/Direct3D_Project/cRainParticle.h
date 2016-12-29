#pragma once

class cParticleEmitter;

class cRainParticle
{
private:
	cParticleEmitter* _rain;
public:
	cRainParticle();
	~cRainParticle();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);
	virtual void Scene_Render1();
	virtual void Scene_RenderSprite();

	void RainPosition(Player* pl);
};
