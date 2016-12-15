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
	vector<string> vTemp = TXTDATA->txtLoad("Database/shortSoundList.txt");
	//vector<string> vTemp = TXTDATA->txtLoad("Database/SoundList.txt");

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
			cSoundStruct* newSoundPart = new cSoundStruct;
			newSoundPart->DirectoryHigh = category;
			arrTotalSound.insert(make_pair(category, newSoundPart));
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
	string category = findSoundCategory(soundType);
	string soundKey = findSoundKey(situation);


	char strNum[10];
	sprintf_s(strNum, "%d", soundNum);
	string keyCount = strNum;

	string applyKey = category + "_" + soundKey + keyCount;

	if (!SOUNDMANAGER->isPlaySound(applyKey))
	{
		SOUNDMANAGER->play(applyKey, 0.5);
	}
}

string cSoundData::findSoundCategory(SoundUtil::SOUND_DATA_TYPE soundType)
{
	string category;
	switch (soundType)
	{
	case SoundUtil::SOUND_TYPE_BGM:
		category = "BGMMusic";
		break;
	case SoundUtil::SOUND_TYPE_MENU:
		category = "MenuSound";
		break;
	case SoundUtil::SOUND_TYPE_PLAYER_EFFECT:
		category = "PLAYERsSound";
		break;
	case SoundUtil::SOUND_TYPE_PLAYER_VOICE:
		category = "PLAYERsVoice";
		break;
	case SoundUtil::SOUND_TYPE_NPC:
		category = "NPCsSound";
		break;
	case SoundUtil::SOUND_TYPE_STRUCT_BASEMENT:
		category = "Basement";
		break;
	case SoundUtil::SOUND_TYPE_STRUCT_FEMALEWARD:
		category = "FemaleWard";
		break;
	case SoundUtil::SOUND_TYPE_ETC:
		break;
	}

	return category;
}

string cSoundData::findSoundKey(SoundUtil::SOUND_DATA_PLAY_TYPE situation)
{
	string soundKey;
	switch (situation)
	{
		//MENU & BGM SOUND
	case SoundUtil::SOUND_PLAY_TYPE_MENUBGM:
		soundKey = "MenuBGM";
		break;
	case SoundUtil::SOUND_PLAY_TYPE_MENUEFFECT:
		soundKey = "MenuEffect";
		break;
	case SoundUtil::SOUND_PLAY_GAME_BGM:
		soundKey = "BackgroundMusic";
		break;

		//PLAYER SOUND
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_WALKING_NORMAL:
		soundKey = "WalkingOnNormal";
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_WALKING_WOOD:
		soundKey = "WalkingOnWood";
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_WALKING_IRON:
		soundKey = "WalkingOnIron";
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_WALKING_SOIL:
		soundKey = "WalkingOnSoil";
		break;

	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_ATTACKED:
		soundKey = "Attacked";
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_ATTACKED_RELAX:
		soundKey = "AttackedRelax";
		break;

	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_ROPE:
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_BREATH_ROUGH:
		soundKey = "BreathRough";
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_DAED:
		soundKey = "Dead";
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_FEAR:
		soundKey = "Fear";
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_FEARHIGH:
		soundKey = "FearHigh";
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_NERVOUS:
		soundKey = "Nervous";
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_PAIN:
		soundKey = "Pain";
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_RELAX:
		soundKey = "Relax";
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_SCRAG:
		soundKey = "Scrag";
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_SURPRISE:
		soundKey = "Surprise";
		break;

	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_TRY_USE_KEY:
		soundKey = "OpenDoorUseKey";
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_TRY_OPEN_CABINET:
		soundKey = "TryOpenCabinet";
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_TRY_TURN_VALVE:
		soundKey = "TryTurnValve";
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_TRY_CLOSE_VALVE:
		soundKey = "CloseValve";
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_OPEN_TAPE:
		soundKey = "OpenTape";
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_SEARCH_DESK:
		soundKey = "SearchSound";
		break;
	case SoundUtil::SOUND_PLAY_TYPE_PLAYER_TYPEKEYBOARD:
		soundKey = "TypeKeyboard";
		break;

		//CAM SOUND 
	case SoundUtil::SOUND_PLAY_TYPE_CAM_EFFECT:
		break;

		//NPC SOUND
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

		//ROOM SOUND
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
		soundKey = "RoomMood";
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

	return soundKey;
}

string cSoundData::getSoundKey(SoundUtil::SOUND_DATA_TYPE soundType, SoundUtil::SOUND_DATA_PLAY_TYPE situation, int soundNum)
{
	string category = findSoundCategory(soundType);
	string soundKey = findSoundKey(situation);


	char strNum[10];
	sprintf_s(strNum, "%d", soundNum);
	string keyCount = strNum;

	string applyKey = category + "_" + soundKey + keyCount;

	return applyKey;
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
