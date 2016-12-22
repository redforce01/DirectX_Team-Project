#pragma once
#include "cSingletonBase.h"

#include <map>
#include <string>

//fmod.hpp 추가
#include "inc/fmod.h"
#include "inc/fmod.hpp"
#include "inc/fmod_codec.h"
#include "inc/fmod_dsp.h"
#include "inc/fmod_errors.h"
#include "inc/fmod_memoryinfo.h"
#include "inc/fmod_output.h"

//lib 링크
#pragma comment ( lib, "lib/fmodex_vc.lib" )
#pragma comment ( lib, "lib/fmodex64_vc.lib")

//#pragma comment ( lib, "lib/fmodex_bc.lib")
//#pragma comment ( lib, "lib/fmodex_lcc.lib")
//#pragma comment ( lib, "lib/fmodexL_bc.lib")
//#pragma comment ( lib, "lib/fmodexL_lcc.lib")
//#pragma comment ( lib, "lib/fmodexL_vc.lib")
//#pragma comment ( lib, "lib/fmodexL64_vc.lib")

//여유분의 채널 갯수 설정(버퍼)
#define EXTRA_SOUND_CHANNEL 500			//여유분 채널
#define SOUNDBUFFER 1000				//사운드 파일 갯수

//총 사운드 갯수
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

	//사운드 추가
	void addSound( string keyName, string soundName, bool background = false, bool loop = false );

	//재생
	void play( string keyName, float volume );

	//종료
	void stop( string keyName );

	//정지
	void pause( string keyName );

	//다시 재생
	void resume( string keyName );

	//멈춰있니?
	bool isPauseSound( string keyName );

	//재생중이니?
	bool isPlaySound( string keyName );

	//Fmod 시스템을 갱신
	void update( void );

	//몇 분 짜리냐?
	unsigned int getLength( string keyName );


	cSoundManager();
	~cSoundManager();
};

#define SOUNDMANAGER cSoundManager::GetInstance()