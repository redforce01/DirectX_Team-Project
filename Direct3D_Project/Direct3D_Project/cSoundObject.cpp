#include "stdafx.h"
#include "cSoundObject.h"


cSoundObject::cSoundObject()
	: m_SoundName(NULL)
	, m_SoundVolume(0)
	, m_SoundPlay(FALSE)
	, m_IsStatic(FALSE)
	, m_PlayerPos(D3DXVECTOR3(0, 0, 0))
{
}


cSoundObject::~cSoundObject()
{
}

void cSoundObject::Init(string soundName, D3DXVECTOR3 position)
{
	m_SoundName = soundName;
	m_SoundVolume = 0.0f;
	m_SoundPlay = FALSE;
	m_IsStatic	= FALSE;
	m_PlayerPos	= D3DXVECTOR3(0, 0, 0);

	this->position = position;
}

void cSoundObject::Init(string soundName, float soundVolume, bool isPlay, bool isStatic, D3DXVECTOR3 position)
{
	m_SoundName = soundName;
	m_SoundVolume = soundVolume;
	m_SoundPlay = isPlay;
	m_IsStatic	= isStatic;
	m_PlayerPos	= D3DXVECTOR3(0, 0, 0);

	this->position = position;

}

void cSoundObject::Release()
{
}

void cSoundObject::Update(float timeDelta)
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

float cSoundObject::FunctionDistance()
{
	D3DXVECTOR3 pos;
	pos = this->position - m_PlayerPos;

	return D3DXVec3Length(&pos);
}

void cSoundObject::SoundPlay(bool isPlay)
{
	m_SoundPlay = isPlay;
}

void cSoundObject::SoundPlay(bool isPlay, float volume)
{
	m_SoundPlay = isPlay;
	m_SoundVolume = volume;
}

void cSoundObject::SetPosition(D3DXVECTOR3 position)
{
	this->position = position;
}

void cSoundObject::SetPlayerPos(D3DXVECTOR3 playerPos)
{
	m_PlayerPos = playerPos;
}

