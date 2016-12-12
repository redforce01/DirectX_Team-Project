#include "stdafx.h"
#include "cObject_Sound.h"

#include "cTransform.h"
#include "cBoundBox.h"

cObject_Sound::cObject_Sound()
	: m_SoundName(NULL)
	, m_SoundVolume(0)
	, m_SoundPlay(FALSE)
	, m_IsStatic(FALSE)
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
	m_SoundPlay = FALSE;
	m_IsStatic	= FALSE;
	m_PlayerPos	= D3DXVECTOR3(0, 0, 0);

	m_pTrans->SetLocalPosition(position);
}

void cObject_Sound::Init(string soundName, float soundVolume, bool isPlay, bool isStatic, D3DXVECTOR3 position)
{
	m_SoundName = soundName;
	m_SoundVolume = soundVolume;
	m_SoundPlay = isPlay;
	m_IsStatic	= isStatic;
	m_PlayerPos	= D3DXVECTOR3(0, 0, 0);

	m_pTrans->SetLocalPosition(position);
}

void cObject_Sound::Init(string soundName, float soundVolume, bool isPlay, bool isStatic, D3DXVECTOR3 position, cBoundBox eventBox)
{
	m_SoundName = soundName;
	m_SoundVolume = soundVolume;
	m_SoundPlay = isPlay;
	m_IsStatic = isStatic;
	m_PlayerPos = D3DXVECTOR3(0, 0, 0);

	m_pTrans->SetLocalPosition(position);
	m_EventBox = eventBox;

}

void cObject_Sound::Release()
{
}

void cObject_Sound::Update(float timeDelta)
{
	if (! m_IsStatic)
	{
		float value = FunctionDistance() * SOUND_DISTANCE_CORRECTION;
		
		if (value > 30)
		{
			m_SoundVolume = 0.1;
		}
		else if (value > 20)
		{
			m_SoundVolume = 0.3;
		}
		else if (value > 10)
		{
			m_SoundVolume = 0.5;
		}
		else if (value > 0)
		{
			m_SoundVolume = 0.7;
		}
		
	}


	if (m_SoundPlay && ! SOUNDMANAGER->isPlaySound(m_SoundName))
	{
		SOUNDMANAGER->play(m_SoundName, m_SoundVolume);
	}
}

float cObject_Sound::FunctionDistance()
{
	D3DXVECTOR3 pos;
	pos = m_pTrans->GetLocalPosition() - m_PlayerPos;

	return D3DXVec3Length(&pos);
}

void cObject_Sound::SoundPlay(bool isPlay)
{
	m_SoundPlay = isPlay;
}

void cObject_Sound::SoundPlay(bool isPlay, float volume)
{
	m_SoundPlay = isPlay;
	m_SoundVolume = volume;
}

void cObject_Sound::SetPosition(D3DXVECTOR3 position)
{
	m_pTrans->SetLocalPosition(position);
}

void cObject_Sound::SetPlayerPos(D3DXVECTOR3 playerPos)
{
	m_PlayerPos = playerPos;
}

void cObject_Sound::SetEventBox(cBoundBox eventBox)
{
	m_EventBox = eventBox;
}

