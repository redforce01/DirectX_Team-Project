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

void cSoundData::playSound(SOUND_TYPE soundType, SOUND_PLAY_TYPE situation, int soundNum)
{
	switch (situation)
	{
	case SoundUtil::SOUND_PLAY_TYPE_MENUBGM:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_MENUEFFECT:
		break;
	case SoundUtil::SOUND_PLAY_GAME_BGM:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_WALKING_NORMAL:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_WALKING_WOOD:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_WALKING_IRON:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_WALKING_SOIL:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_ATTACKED:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_ATTACKED_RELAX:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_ROPE:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_BREATH_ROUGH:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_DAED:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_FEAR:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_FEARHIGH:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_NERVOUS:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_PAIN:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_RELAX:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_SCRAG:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_SURPRISE:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_TRY_USE_KEY:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_TRY_OPEN_CABINET:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_TRY_TURN_VALVE:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_TRY_CLOSE_VALVE:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_OPEN_TAPE:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_SEARCH_DESK:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_TYPEKEYBOARD:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_CAM_EFFECT:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_NPC_SCISSORS:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_NPC_KNIFE:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_ATTACK:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_NPC_WALKING_HEAVY:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_NPC_WALKING_NORMAL:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_NPC_WALKING_STAIR:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_NPC_WALKING_IRON:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_NPC_WALKING_WATER:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_NPC_BREECHING:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_NPC_TRY_BREECHING:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_NPC_CHAIN:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_NPC_CUTBODY:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_NPC_TRYOPEN_DOOR:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_NPC_OPEN_DOOR:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_ROOM_RUN_FAR:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_ROOM_RUN_CLOSE:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_ROOM_SHOWER:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_HALLWAYRUN_FAR:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_HALLWAYRUN_CLOSE:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_LOOKEVENT:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_THUNDER:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_MOOD_HALL:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_MOOD_ROOM:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_BEND:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_MACHINE_01:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_MACHINE_02:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_ELECTRIC:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_ELECTRIC_NOICE:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_SWITCH:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_WATERFALL:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_CAN:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_DOORCLOSE:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_FALLOBJECT:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_HALLWAYRUN:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_HALLWAY:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_IRONEFFECT:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_ROPE:
		break;
	default:
		break;
	}

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
			soundFile.append("/");
			soundFile.append(DirectoryHigh);
			soundFile.append("/");
			soundFile.append(vDirectoryLow[soundPartNum]);
			soundFile.append("/");
			soundFile.append(iterPartSound->second->arrSound[i]);
			soundFile.append(".mp3");

			SOUNDMANAGER->addSound(soundKey, soundFile, FALSE, FALSE);
			soundCount++;
		}
		soundPartNum++;
	}
}
