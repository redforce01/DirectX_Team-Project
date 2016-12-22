#pragma once

#define SOUND_DISTANCE_CORRECTION 100

class cTransform;
class cBoundBox;
class cObject_Sound
{
private:
	string	m_SoundName;
	float	m_SoundVolume;
	bool	m_SoundPlay;
	bool	m_IsStatic;

	D3DXVECTOR3 m_PlayerPos;
	cTransform*	m_pEventTrans;
	cBoundBox	m_EventBox;
	
public:
	cObject_Sound();
	~cObject_Sound();

	//������ ���� ������Ʈ
	void Init(string soundName, D3DXVECTOR3 position);

	//����/���� ���� ������Ʈ
	void Init(string soundName, float soundVolume, bool isPlay, bool isStatic, D3DXVECTOR3 position);

	//����/���� ���� ������Ʈ + �̺�Ʈ �ڽ�
	void Init(string soundName, float soundVolume, bool isPlay, bool isStatic, D3DXVECTOR3 position, cBoundBox eventBox);

	void Release();
	void Update(float timeDelta, D3DXVECTOR3 playerPos);

	float FunctionDistance();

	void SoundPlay(bool isPlay);
	void SoundPlay(bool isPlay, float volum);

	void SetPosition(D3DXVECTOR3 position);
	void SetPlayerPos(D3DXVECTOR3 playerPos);
	void SetEventBox(cBoundBox eventBox);
};

