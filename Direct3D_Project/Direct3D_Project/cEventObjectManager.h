#pragma once
#include "cSingletonBase.h"

#include "cObject_EventObj.h"
#include "cObject_Sound.h"

class cEventObjectManager : public cSingletonBase<cEventObjectManager>
{
private:
	vector<cObject_EventObj*>				vEventObj;
	vector<cObject_EventObj*>::iterator		viEventObj;

	vector<cObject_Sound*>					vEventSound;
	vector<cObject_Sound*>::iterator		viEventSound;

public:
	cEventObjectManager();
	~cEventObjectManager();

	HRESULT Init();
	void Release();
	void Update(float timeDelta);
	void Render(const cCamera* pCamera);

	void AddSoundObject(string soundName, D3DXVECTOR3 position);
	void AddSoundObject(string soundName, float soundVolume, bool isPlay, bool isStatic, D3DXVECTOR3 position);
	void AddSoundObject(string soundName, float soundVolume, bool isPlay, bool isStatic, D3DXVECTOR3 position, cBoundBox eventBox);

	void AddEventObject();
};

#define EVENT_MGR cEventObjectManager::GetInstance()
