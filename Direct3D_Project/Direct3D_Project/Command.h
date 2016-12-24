#pragma once

class Unit;

// Command는 플레이어가 명령을 입력하면 실행하는 클래스임. 그러니깐 플레이어 전용 커맨드 입력 클래스 모음집임.

class Command
{
protected:
	int walkTime = 0;
public:
	virtual ~Command() {};
	virtual void execute(Unit& gameActor, float timeDelta) = 0;

	void PlaySoundWalk();
	void PlaySoundRun();
	void PlaySoundCamTakeOut();
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

class CamUpCommand : public Command
{
public:
	CamUpCommand() {};
	virtual void execute(Unit& gameActor, float timeDelta);
};

class CamDownCommand : public Command
{
public:
	CamDownCommand() {};
	virtual void execute(Unit& gameActor, float timeDelta);
};

class TestCommand : public Command
{
public:
	TestCommand() {}
	virtual void execute(Unit& gameActor, float timeDelta);
};



class InputHandler
{
	Command*      m_ButtonWASD;
	Command*      m_ButtonCtrl;
	Command*      m_ButtonNULL;
	Command*      m_ButtonShift;
	Command*      m_ButtonCtrlUo;
	Command*      m_MouseRight1;
	Command*      m_MouseRight2;


	Unit*         m_CurUnit;

public:
	InputHandler();
	Command* HandleInput();
};



