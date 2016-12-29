#pragma once

class Unit;

// Command는 플레이어가 명령을 입력하면 실행하는 클래스임. 그러니깐 플레이어 전용 커맨드 입력 클래스 모음집임.

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



