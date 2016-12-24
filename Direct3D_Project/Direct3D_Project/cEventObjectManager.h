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

	//SOUND OBJECT �߰� : ���� �̸�, ��ġ ��
	void AddSoundObject(string soundName, D3DXVECTOR3 position);

	//SOUND OBJECT �߰� : ���� �̸�, SOUND����, LoopPlay����, �̺�Ʈ ���� ����, ��ġ ��
	void AddSoundObject(string soundName, float soundVolume, bool isLoopPlay, bool isEvent, D3DXVECTOR3 position);

	//SOUND OBJECT �߰� : ���� �̸�, SOUND����, LoopPlay����, �̺�Ʈ ���� ����, ��ġ ��, �̺�Ʈ �ڽ�
	void AddSoundObject(string soundName, float soundVolume, bool isLoopPlay, bool isEvent, D3DXVECTOR3 position, cBoundBox eventBox);

	//EVENT OBJECT �߰� : ������Ʈ ���� �̸�, ��ġ ��, Pose Name
	void AddEventObject(string objectName, D3DXVECTOR3 position, string poseName);

	//EVENT OBJECT �߰� : ������Ʈ ���� �̸�, ��ġ ��, Pose Name, �̺�Ʈ �ڽ�
	void AddEventObject(string objectName, D3DXVECTOR3 position, string poseName, D3DXVECTOR3 eventPos);

	//EVENT OBJECT �߰� : ������Ʈ ���� �̸�, ��ġ ��, Pose Name, ���� ������Ʈ ����, �̺�Ʈ �ڽ�
	void AddEventObject(string objectName, D3DXVECTOR3 position, string poseName, bool isStatic, D3DXVECTOR3 eventPos);

	cObject_EventObj* GetEventObj(int objNum) { return vEventObj[objNum]; }
};

#define EVENT_MGR cEventObjectManager::GetInstance()
