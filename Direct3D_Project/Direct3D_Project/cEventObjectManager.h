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
	void Update(float timeDelta, D3DXVECTOR3 playerPos);
	void Render(const cCamera* pCamera, vector<cLight*> lights);

	//SOUND OBJECT 추가 : 파일 이름, 위치 값
	void AddSoundObject(string soundName, D3DXVECTOR3 position);

	//SOUND OBJECT 추가 : 파일 이름, 바로 플레이 여부, 정적 사운드 여부, 위치 값
	void AddSoundObject(string soundName, float soundVolume, bool isPlay, bool isStatic, D3DXVECTOR3 position);

	//SOUND OBJECT 추가 : 파일 이름, 바로 플레이 여부, 정적 사운드 여부, 위치 값, 이벤트 박스
	void AddSoundObject(string soundName, float soundVolume, bool isPlay, bool isStatic, D3DXVECTOR3 position, cBoundBox eventBox);

	//EVENT OBJECT 추가 : 오브젝트 파일 이름, 위치 값
	void AddEventObject(string objectName, D3DXVECTOR3 position);

	//EVENT OBJECT 추가 : 오브젝트 파일 이름, 위치 값, 이벤트 박스
	void AddEventObject(string objectName, D3DXVECTOR3 position, cBoundBox eventBox);

	//EVENT OBJECT 추가 : 오브젝트 파일 이름, 위치 값, 정적 오브젝트 여부, 이벤트 박스
	void AddEventObject(string objectName, D3DXVECTOR3 position, bool isStatic, cBoundBox eventBox);
};

#define EVENT_MGR cEventObjectManager::GetInstance()
