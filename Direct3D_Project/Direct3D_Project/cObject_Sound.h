#pragma once

#define SOUND_DISTANCE_CORRECTION 10

class cTransform;
class cBoundBox;
class cObject_Sound
{
private:
	string	m_SoundName;			//���� �̸�
	float	m_SoundVolume;			//���� ���� ��
	bool	m_SoundLoopPlay;		//���� �÷��̸� �ؾ� �ϴ°�?
	bool	m_IsEvent;				//�̺�Ʈ ���������� Ȯ�ο� BOOL ��
	bool	m_DoEvent;				//�̺�Ʈ ���带 ����ߴ°��� ���� ��Ͽ� BOOL ��
	bool	m_IsBoundEvent;

	D3DXVECTOR3 m_PlayerPos;		//�޾ƿ� PLAYER�� POSITION
	cTransform*	m_pEventTrans;		//EVENT�ڽ��� cTransform
	cBoundBox	m_EventBox;			//EVENT cBoundBox
	
public:
	cObject_Sound();
	~cObject_Sound();

	//������ ���� ������Ʈ
	void Init(string soundName, D3DXVECTOR3 position);

	//����/���� ���� ������Ʈ
	void Init(string soundName, float soundVolume, bool isLoopPlay, bool isEvent, D3DXVECTOR3 position);

	//����/���� ���� ������Ʈ + �̺�Ʈ �ڽ�
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

