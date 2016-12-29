#pragma once


class cEffect 
{
private:
	LPDIRECT3DTEXTURE9 blood_hit;
	LPDIRECT3DTEXTURE9 blood_Tex;
	LPDIRECT3DTEXTURE9 RED1;
	LPDIRECT3DTEXTURE9 RED2;
	LPDIRECT3DTEXTURE9 RED3;
	LPDIRECT3DTEXTURE9 RED4;
//	cCamera camera;
	int reccount;
	int zoombarx;
	bool red[3];
	int redalpha;
	int red2alpha;
	int red3alpha;
	int blood;
	bool blood1;
	bool blood2;
	bool hit_blood;
	int hp;
	int hitalpha;
	int x = NULL;
	int y = NULL;
	bool drw;
	int count = 0;
	int requcount;
	bool hpup;
	bool hpdown;

	/////////
	bool isHitPly;
public:
	cEffect();
	~cEffect();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);
	virtual void Scene_Render1();
	virtual void Scene_RenderSprite();

	void Effect(float timeDelta, cCamera* camera);


	void SetisHitPly(bool is) { isHitPly = is; }
	bool GetisHitPly() { return isHitPly; }

};

