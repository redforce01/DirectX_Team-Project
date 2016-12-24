#include "stdafx.h"
#include "Command.h"
#include "Action.h"
#include "Unit.h"

#include "cTransform.h"
#include "Unit.h"


void WalkCommand::execute(Unit& gameActor, float timeDelta)
{
	if (KEY_MGR->IsStayDown('W'))
	{
		gameActor.StatePlayChange("WALK_FORWARD", 0.5f);
		PlaySoundWalk();
	}
	else if (KEY_MGR->IsStayDown('A'))
	{
		gameActor.StatePlayChange("WALK_LEFT", 0.5f);
		PlaySoundWalk();

	}
	else if (KEY_MGR->IsStayDown('D'))
	{
		gameActor.StatePlayChange("WALK_RIGHT", 0.5f);
		PlaySoundWalk();
	}
	else if (KEY_MGR->IsStayDown('S'))
	{
		gameActor.StatePlayChange("WALK_BACKWARD", 0.5f);
		PlaySoundWalk();
	}

	gameActor.getAction()->Move(timeDelta);
}

void CrouchCommand::execute(Unit& gameActor, float timeDelta)
{
	if (KEY_MGR->IsStayDown('W'))
	{
		gameActor.StatePlayChange("CROUCH_FORWARD", 5.0f);
	}
	else if (KEY_MGR->IsStayDown('A'))
	{
		gameActor.StatePlayChange("CROUCH_LEFT", 5.0f);
	}
	else if (KEY_MGR->IsStayDown('D'))
	{
		gameActor.StatePlayChange("CROUCH_RIGHT", 5.0f);
	}
	else if (KEY_MGR->IsStayDown('S'))
	{
		gameActor.StatePlayChange("CROUCH_BACKWARD", 5.0f);
	}
	else
	{
		gameActor.StatePlayChange("CROUCH_IDLE", 0.8f);
	}

	gameActor.getAction()->setMoveSpeed(6.0f);
	gameActor.getAction()->Move(timeDelta);

}

void RunCommand::execute(Unit& gameActor, float timeDelta)
{
	if (KEY_MGR->IsStayDown('W'))
	{
		gameActor.StatePlayChange("RUN_FORWARD", 0.5f);
		PlaySoundRun();
	}
	else if (KEY_MGR->IsStayDown('A'))
	{
		gameActor.StatePlayChange("RUN_LEFT", 0.5f);
		PlaySoundRun();
	}
	else if (KEY_MGR->IsStayDown('D'))
	{
		gameActor.StatePlayChange("RUN_RIGHT", 0.5f);
		PlaySoundRun();
	}
	else if (KEY_MGR->IsStayDown('S'))
	{
		gameActor.StatePlayChange("RUN_BACKWARD", 0.5f);
		PlaySoundRun();
	}
	gameActor.getAction()->Move(timeDelta);
}

void NULLCommand::execute(Unit& gameActor, float timeDelta)
{
	if (!KEY_MGR->IsStayDown(VK_CONTROL))
		gameActor.StateOneShotHoldChange("STAND", 0.5f);

}
void UpStandCommand::execute(Unit & gameActor, float timeDelta)
{
	gameActor.StateOneShotChange("CROUCH_TO_STAND", 0.5f);
}

void CamUpCommand::execute(Unit& gameActor, float timeDelta)
{
	gameActor.StateOneShotHoldChange("STAND_CAM_RAISE", 0.1f);

	PlaySoundCamTakeOut();
}

void CamDownCommand::execute(Unit& gameActor, float timeDelta)
{
	gameActor.StateOneShotHoldChange("STAND_CAM_LOWER2", 0.1f);

	PlaySoundCamTakeOut();
}

void TestCommand::execute(Unit& gameActor, float timeDelta)
{
	gameActor.StatePlayChange("GRAB_AND_THROW", 1.0f);
}

InputHandler::InputHandler()
{
	m_ButtonCtrl = new CrouchCommand;
	m_ButtonWASD = new WalkCommand;
	m_ButtonCtrlUo = new UpStandCommand;
	m_ButtonShift = new RunCommand;
	m_ButtonNULL = new NULLCommand;
	m_MouseRight1 = new CamUpCommand;
	m_MouseRight2 = new CamDownCommand;
}

Command * InputHandler::HandleInput()
{

	if (KEY_MGR->IsStayDown(VK_CONTROL))
	{
		return m_ButtonCtrl;
	}
	else if (KEY_MGR->IsStayDown(VK_SHIFT))
	{
		return m_ButtonShift;
	}
	else if ((KEY_MGR->IsStayDown('W') || KEY_MGR->IsStayDown('S') || KEY_MGR->IsStayDown('D') || KEY_MGR->IsStayDown('A')))
	{
		return m_ButtonWASD;
	}
	else
	{
		return m_ButtonNULL;
	}

}

void Command::PlaySoundWalk()
{
	walkTime++;
	if (walkTime % 32 == 0)
	{
		walkTime = 0;
		if (SOUNDMANAGER->isPlaySound(SOUNDDATA->getSoundKey(SOUND_TYPE_PLAYER_EFFECT, SOUND_PLAY_TYPE_PLAYER_WALKING_NORMAL, 0)))
		{
			SOUNDDATA->playSound(SOUND_TYPE_PLAYER_EFFECT, SOUND_PLAY_TYPE_PLAYER_WALKING_NORMAL, 1, 0.2);
			SOUNDDATA->stopSound(SOUND_TYPE_PLAYER_EFFECT, SOUND_PLAY_TYPE_PLAYER_WALKING_NORMAL, 0);
		}
		else
		{
			SOUNDDATA->playSound(SOUND_TYPE_PLAYER_EFFECT, SOUND_PLAY_TYPE_PLAYER_WALKING_NORMAL, 0, 0.2);
			SOUNDDATA->stopSound(SOUND_TYPE_PLAYER_EFFECT, SOUND_PLAY_TYPE_PLAYER_WALKING_NORMAL, 1);
		}
	}
}

void Command::PlaySoundRun()
{
	walkTime++;
	if (walkTime % 24 == 0)
	{
		walkTime = 0;
		if (SOUNDMANAGER->isPlaySound(SOUNDDATA->getSoundKey(SOUND_TYPE_PLAYER_EFFECT, SOUND_PLAY_TYPE_PLAYER_WALKING_NORMAL, 0)))
		{
			SOUNDDATA->playSound(SOUND_TYPE_PLAYER_EFFECT, SOUND_PLAY_TYPE_PLAYER_WALKING_NORMAL, 1, 0.2);
			SOUNDDATA->stopSound(SOUND_TYPE_PLAYER_EFFECT, SOUND_PLAY_TYPE_PLAYER_WALKING_NORMAL, 0);
		}
		else
		{
			SOUNDDATA->playSound(SOUND_TYPE_PLAYER_EFFECT, SOUND_PLAY_TYPE_PLAYER_WALKING_NORMAL, 0, 0.2);
			SOUNDDATA->stopSound(SOUND_TYPE_PLAYER_EFFECT, SOUND_PLAY_TYPE_PLAYER_WALKING_NORMAL, 1);
		}
	}
}

void Command::PlaySoundCamTakeOut()
{
	//SOUNDDATA->stopSound(SOUND_TYPE_PLAYER_EFFECT, SOUND_PLAY_TYPE_CAM_OUT);
	SOUNDDATA->playSound(SOUND_TYPE_PLAYER_EFFECT, SOUND_PLAY_TYPE_CAM_OUT);
}
