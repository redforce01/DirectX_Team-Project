#pragma once

#define SOUND_DISTANCE_CORRECTION 10

class cTransform;
class cBoundBox;
class cObject_Sound
{
private:
	string	m_SoundName;			//사운드 이름
	float	m_SoundVolume;			//사운드 볼륨 값
	bool	m_SoundLoopPlay;		//루프 플레이를 해야 하는가?
	bool	m_IsEvent;				//이벤트 사운드인지의 확인용 BOOL 값
	bool	m_DoEvent;				//이벤트 사운드를 출력했는가에 대한 기록용 BOOL 값
	bool	m_IsBoundEvent;

	D3DXVECTOR3 m_PlayerPos;		//받아올 PLAYER의 POSITION
	cTransform*	m_pEventTrans;		//EVENT박스용 cTransform
	cBoundBox	m_EventBox;			//EVENT cBoundBox
	
public:
	cObject_Sound();
	~cObject_Sound();

	//정적인 사운드 오브젝트
	void Init(string soundName, D3DXVECTOR3 position);

	//정적/동적 사운드 오브젝트
	void Init(string soundName, float soundVolume, bool isLoopPlay, bool isEvent, D3DXVECTOR3 position);

	//정적/동적 사운드 오브젝트 + 이벤트 박스
	void Init(string soundName, float soundVolume, bool isLoopPlay, bool isEvent, D3DXVECTOR3 position, cBoundBox eventBox);

	void Release();
	void Update(float timeDelta, D3DXVECTOR3 playerPos);

	void FunctionFarSound();
	void FunctionEventSound();
	float FunctionDistance();

	void SoundPlay(bool isPlay);
	void SoundPlay(bool isPlay, float volum);
	void SoundStop();

	void SetPosition(D3DXVECTOR3 position);
	void SetPlayerPos(D3DXVECTOR3 playerPos);
	void SetEventBox(cBoundBox eventBox);

	bool GetIsSoundPlay();
	float GetSoundVolume() { return m_SoundVolume; }
	
};

