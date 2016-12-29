#pragma once

class Unit;

// Command�� �÷��̾ ����� �Է��ϸ� �����ϴ� Ŭ������. �׷��ϱ� �÷��̾� ���� Ŀ�ǵ� �Է� Ŭ���� ��������.

class Command
{
	int walkTime = 0;
public:
	virtual ~Command() {};
	void PlaySoundWalk();
	void PlaySoundRun();
	void PlaySoundCamTakeOut();
	virtual void execute(Unit& gameActor, float timeDelta) = 0;
};

class ForwardCommand : public Command
{
public:
	ForwardCommand() {};
	virtual void execute(Unit& gameActor, float timeDelta);

};

class LeftCommand : public Command
{
public:
	LeftCommand() {};
	virtual void execute(Unit& gameActor, float timeDelta);

};

class RightCommand : public Command
{
public:
	RightCommand() {};
	virtual void execute(Unit& gameActor, float timeDelta);

};


class BackwardCommand : public Command
{
public:
	BackwardCommand() {};
	virtual void execute(Unit& gameActor, float timeDelta);

};

class NULLCommand : public Command
{
public:
	NULLCommand() {};
	virtual void execute(Unit& gameActor, float timeDelta);

};

class CrouchCommand : public Command
{
public:
	CrouchCommand() {}
	virtual void execute(Unit& gameActor, float timeDelta);
};


class InputHandler
{
	Command*      m_ButtonW;
	Command*      m_ButtonA;
	Command*      m_ButtonD;
	Command*      m_ButtonS;
	Command*      m_ButtonNULL;
	Command*	  m_ButtonCtrl;


	Unit*         m_CurUnit;

public:
	InputHandler();
	Command* HandleInput();
};



