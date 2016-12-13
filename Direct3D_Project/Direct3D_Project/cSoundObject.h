#pragma once

#include "cTransform.h"

#define SOUND_DISTANCE_CORRECTION 1000

class cSoundObject : public cTransform
{
private:
	string	m_SoundName;
	float	m_SoundVolume;
	bool	m_SoundPlay;
	bool	m_IsStatic;

	D3DXVECTOR3 m_PlayerPos;
	
public:
	cSoundObject();
	~cSoundObject();

	void Init(string soundName, D3DXVECTOR3 position);
	void Init(string soundName, float soundVolume, bool isPlay, bool isStatic, D3DXVECTOR3 position);
	void Release();
	void Update(float timeDelta);

	float FunctionDistance();

	void SoundPlay(bool isPlay);
	void SoundPlay(bool isPlay, float volum);

	void SetPosition(D3DXVECTOR3 position);
	void SetPlayerPos(D3DXVECTOR3 playerPos);
};

