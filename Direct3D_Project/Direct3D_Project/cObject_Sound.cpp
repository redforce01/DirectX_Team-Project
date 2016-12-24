#include "stdafx.h"
#include "cObject_Sound.h"

#include "cTransform.h"
#include "cBoundBox.h"

cObject_Sound::cObject_Sound()
	: m_SoundVolume(0)
	, m_SoundLoopPlay(FALSE)
	, m_IsEvent(FALSE)
	, m_DoEvent(FALSE)
	, m_IsBoundEvent(FALSE)
	, m_PlayerPos(D3DXVECTOR3(0, 0, 0))
{
}


cObject_Sound::~cObject_Sound()
{
}

void cObject_Sound::Init(string soundName, D3DXVECTOR3 position)
{
	m_SoundName = soundName;
	m_SoundVolume = 0.0f;
	m_SoundLoopPlay = FALSE;
	m_IsEvent = FALSE;
	m_DoEvent = FALSE;
	m_IsBoundEvent = FALSE;
	m_PlayerPos	= D3DXVECTOR3(0, 0, 0);

	m_pEventTrans = new cTransform;
	m_pEventTrans->SetWorldPosition(position);
}

void cObject_Sound::Init(string soundName, float soundVolume, bool isLoopPlay, bool isEvent, D3DXVECTOR3 position)
{
	m_SoundName = soundName;
	m_SoundVolume = soundVolume;
	m_SoundLoopPlay = isLoopPlay;
	m_IsEvent = isEvent;
	m_DoEvent = FALSE;
	m_IsBoundEvent = FALSE;
	m_PlayerPos	= D3DXVECTOR3(0, 0, 0);

	m_pEventTrans = new cTransform;
	m_pEventTrans->SetWorldPosition(position);
}

void cObject_Sound::Init(string soundName, float soundVolume, bool isLoopPlay, bool isEvent, D3DXVECTOR3 position, cBoundBox eventBox)
{
	m_SoundName = soundName;
	m_SoundVolume = soundVolume;
	m_SoundLoopPlay = isLoopPlay;
	m_IsEvent = isEvent;
	m_DoEvent = FALSE;
	m_IsBoundEvent = TRUE;
	m_PlayerPos = D3DXVECTOR3(0, 0, 0);

	m_pEventTrans = new cTransform;
	m_pEventTrans->SetWorldPosition(position);
	m_EventBox = eventBox;

}

void cObject_Sound::Release()
{
	SAFE_DELETE(m_pEventTrans);
}

void cObject_Sound::Update(float timeDelta, D3DXVECTOR3 playerPos)
{
	//이벤트 사운드를 출력 한 적이 있다면 더 이상 Update를 하지말자
	if (m_DoEvent) return;

	m_PlayerPos = playerPos;
	
	//만약 Event용 사운드 라면?
	if (m_IsEvent)
	{
		//Event 사운드니까 Player와 충돌났는지 검사
		FunctionEventSound();
	}
	else
	{
		//Event 사운드가 아니니 플레이어와의 거리를 체크(거리 값에 따라 볼륨을 조절)
		FunctionFarSound();
	}


	if(FALSE == SOUNDMANAGER->isPlaySound(m_SoundName))
	{
		if (m_SoundLoopPlay)
		{
			SOUNDMANAGER->play(m_SoundName, m_SoundVolume);
		}
	}
}

void cObject_Sound::FunctionFarSound()
{
	float value = FunctionDistance();

	if (value > 20)
	{
		m_SoundVolume = 0.0;
		//SOUNDMANAGER->stop(m_SoundName);
	}
	else if (value >= 15 && value < 20)
	{
		m_SoundVolume = 0.1;
	}
	else if (value >= 10 && value < 15)
	{
		m_SoundVolume = 0.3;
	}
	else if (value >= 0 && value < 10)
	{
		m_SoundVolume = 0.5;
	}
}

void cObject_Sound::FunctionEventSound()
{
	float value = FunctionDistance();
	if (FALSE == m_DoEvent)	
	{
		if (value < 5)
		{
			SOUNDMANAGER->play(m_SoundName, m_SoundVolume);
			m_DoEvent = TRUE;
		}
	}
}

float cObject_Sound::FunctionDistance()
{
	D3DXVECTOR3 pos;
	pos = m_pEventTrans->GetWorldPosition() - m_PlayerPos;

	return D3DXVec3Length(&pos);
}

void cObject_Sound::SoundPlay(bool isPlay)
{
	if (isPlay)
	{
		SOUNDMANAGER->play(m_SoundName, m_SoundVolume);
	}
	else
	{
		SOUNDMANAGER->stop(m_SoundName);
	}
}

void cObject_Sound::SoundPlay(bool isPlay, float volume)
{
	if (isPlay)
	{
		SOUNDMANAGER->play(m_SoundName, volume);
	}
	else
	{
		SOUNDMANAGER->stop(m_SoundName);
	}
}

void cObject_Sound::SoundStop()
{
	SOUNDMANAGER->stop(m_SoundName);
}

void cObject_Sound::SetPosition(D3DXVECTOR3 position)
{
	m_pEventTrans->SetWorldPosition(position);
}

void cObject_Sound::SetPlayerPos(D3DXVECTOR3 playerPos)
{
	m_PlayerPos = playerPos;
}

void cObject_Sound::SetEventBox(cBoundBox eventBox)
{
	m_EventBox = eventBox;
}

bool cObject_Sound::GetIsSoundPlay()
{
	return SOUNDMANAGER->isPlaySound(m_SoundName);
}

