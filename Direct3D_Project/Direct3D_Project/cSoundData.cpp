#include "stdafx.h"
#include "cSoundData.h"


cSoundData::cSoundData()
{
}


cSoundData::~cSoundData()
{
}

void cSoundData::Init()
{
	LoadSoundList();
	addSoundList();

}

void cSoundData::Release()
{
	for (iterTotalSound = arrTotalSound.begin(); iterTotalSound != arrTotalSound.end(); ++iterTotalSound)
	{
		SAFE_DELETE(iterTotalSound->second);
	}
}

void cSoundData::LoadSoundList()
{
	vector<string> vTemp = TXTDATA->txtLoad("Database/testSoundList.txt");

	string category;
	string part;
	string soundName;

	for (unsigned int i = 0; i < vTemp.size(); i++)
	{
		if (strchr(vTemp[i].c_str(), '#')) continue;

		if (strchr(vTemp[i].c_str(), '*'))
		{
			category = strstr(vTemp[i].c_str(), "*");
			category = category.substr(1, category.size());
			cSoundStruct* sound = new cSoundStruct;
			sound->DirectoryHigh = category;
			arrTotalSound.insert(make_pair(category, sound));
			iterTotalSound = arrTotalSound.begin();
		}

		if (strchr(vTemp[i].c_str(), '-'))
		{
			part = strstr(vTemp[i].c_str(), "-");
			part = part.substr(1, part.size());
			arrTotalSound.find(category)->second->vDirectoryLow.push_back(part);
			arrTotalSound.find(category)->second->arrPartSound.insert(make_pair(part, new tagSoundList));
			arrTotalSound.find(category)->second->iterPartSound = arrTotalSound.find(category)->second->arrPartSound.begin();
		}

		if (strchr(vTemp[i].c_str(), '='))
		{
			soundName = strstr(vTemp[i].c_str(), "=");
			soundName = soundName.substr(1, soundName.size());
			arrTotalSound.find(category)->second->arrPartSound.find(part)->second->arrSound.push_back(soundName);
			arrTotalSound.find(category)->second->arrPartSound.find(part)->second->iterSound =
				arrTotalSound.find(category)->second->arrPartSound.find(part)->second->arrSound.begin();
		}
	}
}

void cSoundData::addSoundList()
{
	for (iterTotalSound = arrTotalSound.begin(); iterTotalSound != arrTotalSound.end(); ++iterTotalSound)
	{
		iterTotalSound->second->SoundMgrAdd();
	}
}

void cSoundData::playSound(SoundUtil::SOUND_DATA_TYPE soundType, SoundUtil::SOUND_DATA_PLAY_TYPE situation, int soundNum)
{

}

void cSoundStruct::SoundMgrAdd()
{
	string soundKey;
	string soundFile;

	char soundKeyBuf[10];
	int soundCount = 0;
	int soundPartNum = 0;

	for (iterPartSound = arrPartSound.begin(); iterPartSound != arrPartSound.end(); ++iterPartSound)
	{
		soundCount = 0;
		int size = iterPartSound->second->arrSound.size();
		for (int i = 0; i < size; i++)
		{
			soundKey = DirectoryHigh;
			soundKey = soundKey.append("_");
			soundKey = soundKey.append(vDirectoryLow[soundPartNum]);
			sprintf_s(soundKeyBuf, "%d", soundCount);
			soundKey = soundKey.append(soundKeyBuf);

			iterPartSound->second[i];
			soundFile = "SoundData";
			soundFile = soundFile.append("/");
			soundFile = soundFile.append(DirectoryHigh);
			soundFile = soundFile.append("/");
			soundFile = soundFile.append(vDirectoryLow[soundPartNum]);
			soundFile = soundFile.append("/");
			soundFile = soundFile.append(iterPartSound->second->arrSound[i]);
			soundFile = soundFile.append(".mp3");

			SOUNDMANAGER->addSound(soundKey, soundFile, FALSE, FALSE);
			soundCount++;
		}
		soundPartNum++;
	}
}
