#include "stdafx.h"
#include "cEventObjectManager.h"


cEventObjectManager::cEventObjectManager()
{
}


cEventObjectManager::~cEventObjectManager()
{
}

HRESULT cEventObjectManager::Init()
{
	//for (int i = 0; i < 3; i++)
	//{
	//	float x = RandomFloatRange(0, 100);
	//	float z = RandomFloatRange(0, 100);

	//	cObject_EventObj* newEvent = new cObject_EventObj;
	//	newEvent->Init();
	//	newEvent->setLocalPosition(D3DXVECTOR3(x, 0, z));

	//	vEventObj.push_back(newEvent);
	//}


	//cObject_Sound* newSoundObj = new cObject_Sound;
	//string soundName = SOUNDDATA->getSoundKey(SOUND_TYPE_STRUCT_BASEMENT, SOUND_PLAY_TYPE_MOOD_ROOM, 0);
	//newSoundObj->Init(soundName, 0.5, FALSE, TRUE, D3DXVECTOR3(0, 0, 0));

	//vEventSound.push_back(newSoundObj);


	//"../Resources/NPC/NPC_Death.X"

	return S_OK;
}

void cEventObjectManager::Release()
{
	for (viEventObj = vEventObj.begin(); viEventObj != vEventObj.end(); ++viEventObj)
	{
		(*viEventObj)->Release();
		SAFE_DELETE((*viEventObj));
		viEventObj = vEventObj.erase(viEventObj);
	}
	vEventObj.clear();

	for (viEventSound = vEventSound.begin(); viEventSound != vEventSound.end(); ++viEventSound)
	{
		(*viEventSound)->Release();
		SAFE_DELETE((*viEventSound));
		viEventSound = vEventSound.erase(viEventSound);
	}
	vEventSound.clear();
}

void cEventObjectManager::Update(float timeDelta, D3DXVECTOR3 playerPos)
{
	int size = vEventObj.size();
	for (int i = 0; i < size; i++)
	{
		vEventObj[i]->Update(timeDelta, playerPos);
	}

	size = vEventSound.size();
	for (int i = 0; i < size; i++)
	{
		vEventSound[i]->Update(timeDelta, playerPos);
	}
}

void cEventObjectManager::Render(const cCamera * pCamera)
{
	for (int i = 0; i < vEventObj.size(); i++)
	{
		vEventObj[i]->Render(pCamera);
	}
}

void cEventObjectManager::AddSoundObject(string soundName, D3DXVECTOR3 position)
{
	cObject_Sound* newSoundObj = new cObject_Sound;
	newSoundObj->Init(soundName, position);
	vEventSound.push_back(newSoundObj);
}

void cEventObjectManager::AddSoundObject(string soundName, float soundVolume, bool isPlay, bool isStatic, D3DXVECTOR3 position)
{
	cObject_Sound* newSoundObj = new cObject_Sound;
	newSoundObj->Init(soundName, soundVolume, isPlay, isStatic, position);
	vEventSound.push_back(newSoundObj);
}

void cEventObjectManager::AddSoundObject(string soundName, float soundVolume, bool isPlay, bool isStatic, D3DXVECTOR3 position, cBoundBox eventBox)
{
	cObject_Sound* newSoundObj = new cObject_Sound;
	newSoundObj->Init(soundName, soundVolume, isPlay, isStatic, position, eventBox);
	vEventSound.push_back(newSoundObj);
}

void cEventObjectManager::AddEventObject(string objectName, D3DXVECTOR3 position)
{
	cObject_EventObj* newEventObj = new cObject_EventObj;
	newEventObj->Init(objectName, position);
	vEventObj.push_back(newEventObj);
}

void cEventObjectManager::AddEventObject(string objectName, D3DXVECTOR3 position, cBoundBox eventBox)
{
	cObject_EventObj* newEventObj = new cObject_EventObj;
	newEventObj->Init(objectName, position, eventBox);
	vEventObj.push_back(newEventObj);
}

void cEventObjectManager::AddEventObject(string objectName, D3DXVECTOR3 position, bool isStatic, cBoundBox eventBox)
{
	cObject_EventObj* newEventObj = new cObject_EventObj;
	newEventObj->Init(objectName, position, isStatic, eventBox);
	vEventObj.push_back(newEventObj);
}
