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

	//SOUND OBJECT �߰� : ���� �̸�, �ٷ� �÷��� ����, ���� ���� ����, ��ġ ��
	void AddSoundObject(string soundName, float soundVolume, bool isPlay, bool isStatic, D3DXVECTOR3 position);

	//SOUND OBJECT �߰� : ���� �̸�, �ٷ� �÷��� ����, ���� ���� ����, ��ġ ��, �̺�Ʈ �ڽ�
	void AddSoundObject(string soundName, float soundVolume, bool isPlay, bool isStatic, D3DXVECTOR3 position, cBoundBox eventBox);

	//EVENT OBJECT �߰� : ������Ʈ ���� �̸�, ��ġ ��
	void AddEventObject(string objectName, D3DXVECTOR3 position);

	//EVENT OBJECT �߰� : ������Ʈ ���� �̸�, ��ġ ��, �̺�Ʈ �ڽ�
	void AddEventObject(string objectName, D3DXVECTOR3 position, cBoundBox eventBox);

	//EVENT OBJECT �߰� : ������Ʈ ���� �̸�, ��ġ ��, ���� ������Ʈ ����, �̺�Ʈ �ڽ�
	void AddEventObject(string objectName, D3DXVECTOR3 position, bool isStatic, cBoundBox eventBox);
};

#define EVENT_MGR cEventObjectManager::GetInstance()
