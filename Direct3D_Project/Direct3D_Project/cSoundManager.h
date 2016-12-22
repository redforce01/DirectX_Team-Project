#pragma once
#include "cSingletonBase.h"

#include <map>
#include <string>

//fmod.hpp �߰�
#include "inc/fmod.h"
#include "inc/fmod.hpp"
#include "inc/fmod_codec.h"
#include "inc/fmod_dsp.h"
#include "inc/fmod_errors.h"
#include "inc/fmod_memoryinfo.h"
#include "inc/fmod_output.h"

//lib ��ũ
#pragma comment ( lib, "lib/fmodex_vc.lib" )
#pragma comment ( lib, "lib/fmodex64_vc.lib")

//#pragma comment ( lib, "lib/fmodex_bc.lib")
//#pragma comment ( lib, "lib/fmodex_lcc.lib")
//#pragma comment ( lib, "lib/fmodexL_bc.lib")
//#pragma comment ( lib, "lib/fmodexL_lcc.lib")
//#pragma comment ( lib, "lib/fmodexL_vc.lib")
//#pragma comment ( lib, "lib/fmodexL64_vc.lib")

//�������� ä�� ���� ����(����)
#define EXTRA_SOUND_CHANNEL 500			//������ ä��
#define SOUNDBUFFER 1000				//���� ���� ����

//�� ���� ����
#define TOTAL_SOUND_CHANNEL (SOUNDBUFFER + EXTRA_SOUND_CHANNEL)

using namespace FMOD;

class cSoundManager : public cSingletonBase <cSoundManager>
{
private:
	typedef map <string, Sound**> arrSounds;
	typedef map <string, Sound**>::iterator arrSoundsIter;
	typedef map <string, Channel**> arrChannels;
	typedef map <string, Channel**>::iterator arrChannelIter;

private:
	System* _system;
	Sound** _sound;

	Channel** _channel;

	arrSounds _mTotalSounds;


public:
	HRESULT init( void );
	void release( void );

	//���� �߰�
	void addSound( string keyName, string soundName, bool background = false, bool loop = false );

	//���
	void play( string keyName, float volume );

	//����
	void stop( string keyName );

	//����
	void pause( string keyName );

	//�ٽ� ���
	void resume( string keyName );

	//�����ִ�?
	bool isPauseSound( string keyName );

	//������̴�?
	bool isPlaySound( string keyName );

	//Fmod �ý����� ����
	void update( void );

	//�� �� ¥����?
	unsigned int getLength( string keyName );


	cSoundManager();
	~cSoundManager();
};

#define SOUNDMANAGER cSoundManager::GetInstance()