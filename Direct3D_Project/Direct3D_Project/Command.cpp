#include "stdafx.h"
#include "Command.h"
#include "Action.h"
#include "Unit.h"

#include "cTransform.h"
#include "Unit.h"


void ForwardCommand::execute(Unit& gameActor, float timeDelta)
{
	if (KEY_MGR->IsStayDown(VK_SHIFT) )
	{
		if (gameActor.GetCurStamina() >= 1)
		{
			PlaySoundRun();
			gameActor.StatePlayChange("RUN_FORWARD", 0.5f);
			gameActor.RunningStamina();
		}
		else
		{
			PlaySoundWalk();
			gameActor.StatePlayChange("WALK_FORWARD", 0.5f);
		}
	}
	else
	{
		PlaySoundWalk();
		gameActor.StatePlayChange("WALK_FORWARD", 0.5f);
		gameActor.PlusStamina();
	}
	gameActor.getAction()->Move(timeDelta);
}

void LeftCommand::execute(Unit& gameActor, float timeDelta)
{
	if (KEY_MGR->IsStayDown(VK_SHIFT) )
	{
		if (gameActor.GetCurStamina() >= 1)
		{
			PlaySoundRun();
			gameActor.StatePlayChange("RUN_LEFT", 0.5f);
			gameActor.RunningStamina();
		}
		else
		{
			PlaySoundWalk();
			gameActor.StatePlayChange("WALK_LEFT", 0.5f);
		}

	}
	else
	{
		PlaySoundWalk();
		gameActor.StatePlayChange("WALK_LEFT", 0.5f);
		gameActor.PlusStamina();
	}
	gameActor.getAction()->Move(timeDelta);
}


void RightCommand::execute(Unit& gameActor, float timeDelta)
{
	if (KEY_MGR->IsStayDown(VK_SHIFT))
	{
		if (gameActor.GetCurStamina() >= 1)
		{
			PlaySoundRun();
			gameActor.StatePlayChange("RUN_RIGHT", 0.5f);
			gameActor.RunningStamina();
		}
		else
		{
			PlaySoundWalk();
		gameActor.StatePlayChange("WALK_RIGHT", 0.5f);
	    }
	}
	else
	{
		PlaySoundWalk();
		gameActor.StatePlayChange("WALK_RIGHT", 0.5f);
		gameActor.PlusStamina();
	}
	gameActor.getAction()->Move(timeDelta);
}



void BackwardCommand::execute(Unit& gameActor, float timeDelta)
{
	if (KEY_MGR->IsStayDown(VK_SHIFT) )
	{
		if (gameActor.GetCurStamina() >= 1)
		{
			PlaySoundRun();
			gameActor.StatePlayChange("RUN_BACKWARD", 0.5f);
			gameActor.RunningStamina();
		}
		else
		{
			gameActor.StatePlayChange("RUN_BACKWARD", 0.5f);
			PlaySoundWalk();
		}

	}
	else
	{
		PlaySoundWalk();
		gameActor.StatePlayChange("WALK_BACKWARD", 0.5f);
		gameActor.PlusStamina();
	}
	gameActor.getAction()->Move(timeDelta);
}


void NULLCommand::execute(Unit& gameActor, float timeDelta)
{
	if (!KEY_MGR->IsStayDown(VK_CONTROL))
		gameActor.StateOneShotHoldChange("STAND", 0.5f);

	gameActor.PlusStamina();
}

void CrouchCommand::execute(Unit& gameActor, float timeDelta)
{
	gameActor.StateOneShotHoldChange("CROUCH_IDLE", 0.5f);
	gameActor.PlusStamina();
}


InputHandler::InputHandler()
{
	m_ButtonW = new ForwardCommand;
	m_ButtonA = new LeftCommand;
	m_ButtonD = new RightCommand;
	m_ButtonS = new BackwardCommand;
	m_ButtonNULL = new NULLCommand;
	m_ButtonCtrl = new CrouchCommand();
}

Command * InputHandler::HandleInput()
{

	if (KEY_MGR->IsStayDown('W'))
	{
		return m_ButtonW;
	}
	else if (KEY_MGR->IsStayDown('A'))
	{
		return m_ButtonA;
	}
	else if (KEY_MGR->IsStayDown('D'))
	{
		return m_ButtonD;
	}
	else if (KEY_MGR->IsStayDown('S'))
	{
		return m_ButtonS;
	}
	else if (KEY_MGR->IsStayDown(VK_CONTROL))
	{
		return m_ButtonCtrl;
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
	if (walkTime % 18 == 0)
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
	SOUNDDATA->playSound(SOUND_TYPE_PLAYER_EFFECT, SOUND_PLAY_TYPE_CAM_OUT);
}
