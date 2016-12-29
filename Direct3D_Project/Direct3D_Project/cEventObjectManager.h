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

	//SOUND OBJECT 추가 : 파일 이름, SOUND볼륨, LoopPlay여부, 이벤트 사운드 여부, 위치 값
	void AddSoundObject(string soundName, float soundVolume, bool isLoopPlay, bool isEvent, D3DXVECTOR3 position);

	//SOUND OBJECT 추가 : 파일 이름, SOUND볼륨, LoopPlay여부, 이벤트 사운드 여부, 위치 값, 이벤트 박스
	void AddSoundObject(string soundName, float soundVolume, bool isLoopPlay, bool isEvent, D3DXVECTOR3 position, cBoundBox eventBox);

	//EVENT OBJECT 추가 : 오브젝트 파일 이름, 위치 값, Pose Name
	void AddEventObject(string objectName, D3DXVECTOR3 position, string poseName);

	//EVENT OBJECT 추가 : 오브젝트 파일 이름, 위치 값, Pose Name, 이벤트 박스
	void AddEventObject(string objectName, D3DXVECTOR3 position, string poseName, D3DXVECTOR3 eventPos);

	//EVENT OBJECT 추가 : 오브젝트 파일 이름, 위치 값, Pose Name, 정적 오브젝트 여부, 이벤트 박스
	void AddEventObject(string objectName, D3DXVECTOR3 position, string poseName, bool isStatic, D3DXVECTOR3 eventPos);

	cObject_EventObj* GetEventObj(int objNum) { return vEventObj[objNum]; }
};

#define EVENT_MGR cEventObjectManager::GetInstance()
