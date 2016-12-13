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
	}
	else if (KEY_MGR->IsStayDown('A'))
	{
		gameActor.StatePlayChange("WALK_FORWARD", 0.5f);
	}
	else if (KEY_MGR->IsStayDown('D'))
	{
		gameActor.StatePlayChange("WALK_FORWARD", 0.5f);
	}
	else if (KEY_MGR->IsStayDown('S'))
	{
		gameActor.StatePlayChange("WALK_FORWARD", 0.5f);
	}

	gameActor.getAction()->Move(timeDelta);
}

void CrouchCommand::execute(Unit& gameActor, float timeDelta)
{
	if (KEY_MGR->IsStayDown('W'))
	{
		gameActor.StatePlayChange("CROUCH_FORWARD", 1.5f);
	}
	else if (KEY_MGR->IsStayDown('A'))
	{
		gameActor.StatePlayChange("CROUCH_LEFT", 1.5f);
	}
	else if (KEY_MGR->IsStayDown('D'))
	{
		gameActor.StatePlayChange("CROUCH_RIGHT", 1.5f);
	}
	else if (KEY_MGR->IsStayDown('S'))
	{
		gameActor.StatePlayChange("CROUCH_BACKWARD", 1.5f);
	}
	else
	{
		gameActor.StatePlayChange("CROUCH_IDLE", 1.5f);
	}

	gameActor.getAction()->Move(timeDelta);

}

void RunCommand::execute(Unit& gameActor, float timeDelta)
{
	if (KEY_MGR->IsStayDown('W'))
	{
		gameActor.StatePlayChange("RUN_FORWARD", 0.5f);
	}
	else if (KEY_MGR->IsStayDown('A'))
	{
		gameActor.StatePlayChange("RUN_LEFT", 0.5f);
	}
	else if (KEY_MGR->IsStayDown('D'))
	{
		gameActor.StatePlayChange("RUN_RIGHT", 0.5f);
	}
	else if (KEY_MGR->IsStayDown('S'))
	{
		gameActor.StatePlayChange("RUN_FORWARD", 0.5f);
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

void CamCommand::execute(Unit& gameActor, float timeDelta)
{

	//if ((KEY_MGR->IsStayDown('W') || KEY_MGR->IsStayDown('S') || KEY_MGR->IsStayDown('D') || KEY_MGR->IsStayDown('A')))
	//{
	gameActor.StateOneShotHoldChange("STAND_CAM_RAISE", 0.5f);
	gameActor.getAction()->Move(timeDelta);
	//	}

}

void JumpCommand::execute(Unit& gameActor, float timeDelta)
{
	gameActor.StateOneShotChange("JUMP_ON_SPOT", 0.5f);
}

InputHandler::InputHandler()
{
	m_ButtonCtrl = new CrouchCommand;
	m_ButtonWASD = new WalkCommand;
	m_ButtonCtrlUo = new UpStandCommand;
	m_ButtonShift = new RunCommand;
	m_ButtonNULL = new NULLCommand;
	m_MouseRight = new CamCommand;
	m_ButtonSpace = new JumpCommand;
}

Command * InputHandler::HandleInput()
{
	if (KEY_MGR->IsStayDown(VK_RBUTTON))
	{
		//		g_HandycamState = HANDYCAM_YES;
		return m_MouseRight;
	}
	else
	{
		//		g_HandycamState = HANDYCAM_NO;
		if (KEY_MGR->IsStayDown(VK_CONTROL))
		{
			return m_ButtonCtrl;
		}
		else if (KEY_MGR->IsStayDown(VK_SHIFT))
		{
			return m_ButtonShift;
		}
		else if (KEY_MGR->IsStayDown(VK_SPACE))
		{
			return m_ButtonSpace;
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
}


