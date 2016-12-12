#include "stdafx.h"
#include "cScene_Main.h"

cScene_Main::cScene_Main()
{
}


cScene_Main::~cScene_Main()
{
}

HRESULT cScene_Main::Scene_Init()
{
	for (int i = 0; i < 3; i++)
	{
		float x = RandomFloatRange(0, 100);
		float z = RandomFloatRange(0, 100);

		cObject_EventObj* newEvent = new cObject_EventObj;
		newEvent->Init();
		newEvent->setLocalPosition(D3DXVECTOR3(x, 0, z));

		vEventObj.push_back(newEvent);
	}


	cObject_Sound* newSoundObj = new cObject_Sound;
	string soundName = SOUNDDATA->getSoundKey(SOUND_TYPE_STRUCT_BASEMENT, SOUND_PLAY_TYPE_MOOD_ROOM, 0);
	newSoundObj->Init(soundName, D3DXVECTOR3(0, 0, 0));
	
	vEventSound.push_back(newSoundObj);

	return S_OK;
}

void cScene_Main::Scene_Release()
{
	for (int i = 0; i < vEventObj.size(); i++)
	{
		vEventObj[i]->Release();
		SAFE_DELETE(vEventObj[i]);
	}
	vEventObj.clear();

	for (int i = 0; i < vEventSound.size(); i++)
	{
		vEventSound[i]->Release();
		SAFE_DELETE(vEventSound[i]);
	}
	vEventSound.clear();
}

void cScene_Main::Scene_Update(float timeDelta)
{
	for (int i = 0; i < vEventObj.size(); i++)
	{
		vEventObj[i]->Update(timeDelta);
	}

	for (int i = 0; i < vEventObj.size(); i++)
	{
		vEventSound[i]->Update(timeDelta);
	}

	vEventSound[0]->SoundPlay(true);
}

void cScene_Main::Scene_Render1()
{
	for (int i = 0; i < vEventObj.size(); i++)
	{
		vEventObj[i]->Render(pMainCamera);
	}
}
