#pragma once

class Unit;

// Command�� �÷��̾ ����� �Է��ϸ� �����ϴ� Ŭ������. �׷��ϱ� �÷��̾� ���� Ŀ�ǵ� �Է� Ŭ���� ��������.

class Command
{	
public:
	int walkTime;

	virtual ~Command() {};
	virtual void execute(Unit& gameActor, float timeDelta) = 0;
};

class WalkCommand : public Command
{
public:
	WalkCommand() {};
	virtual void execute(Unit& gameActor, float timeDelta);

};

class CrouchCommand : public Command
{
public:
	CrouchCommand() {};
	virtual void execute(Unit& gameActor, float timeDelta);

};

class RunCommand : public Command
{
public:
	RunCommand() {};
	virtual void execute(Unit& gameActor, float timeDelta);

};


class UpStandCommand : public Command
{
public:
	UpStandCommand() {};
	virtual void execute(Unit& gameActor, float timeDelta);

};

class NULLCommand : public Command
{
public:
	NULLCommand() {};
	virtual void execute(Unit& gameActor, float timeDelta);

};

class CamCommand : public Command
{
public:
	CamCommand() {};
	virtual void execute(Unit& gameActor, float timeDelta);
};

class JumpCommand : public Command
{
public:
	JumpCommand() {};
	virtual void execute(Unit& gameActor, float timeDelta);
};

class InputHandler
{
	Command* m_ButtonWASD;
	Command* m_ButtonCtrl;
	Command* m_ButtonNULL;
	Command* m_ButtonShift;
	Command* m_ButtonCtrlUo;
	Command* m_MouseRight;
	Command* m_ButtonSpace;

	Unit* m_CurUnit;

public:
	InputHandler();
	Command* HandleInput();

};




