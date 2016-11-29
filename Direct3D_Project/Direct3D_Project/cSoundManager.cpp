#include "StdAfx.h"
#include "cSoundManager.h"

cSoundManager::cSoundManager( void )
	:_system( NULL ),
	_channel( NULL ),
	_sound( NULL )
{
}

cSoundManager ::~cSoundManager( void )
{
}

HRESULT cSoundManager::init( void )
{
	//���� �ý��� ����
	System_Create( &_system );

	//ä�μ� ���� �� �ʱ�ȭ
	_system->init( TOTAL_SOUND_CHANNEL, FMOD_INIT_NORMAL, 0 );

	//ä�ΰ� ���带 �����Ҵ�...
	//�޸� ���� �� ���� ����...
	_sound = new Sound *[TOTAL_SOUND_CHANNEL];
	_channel = new Channel *[TOTAL_SOUND_CHANNEL];

	//�ʱ�ȭ
	memset( _sound, 0, sizeof( Sound* )* ( TOTAL_SOUND_CHANNEL ) );
	memset( _channel, 0, sizeof( Channel* )* ( TOTAL_SOUND_CHANNEL ) );

	return S_OK;
}

void cSoundManager::release( void )
{
	//���� ����
	if ( _channel != NULL || _sound != NULL )
	{
		for ( int i = 0; i < TOTAL_SOUND_CHANNEL; i++ )
		{
			if ( _channel != NULL )
			{
				if ( _channel[i] ) _channel[i]->stop();
			}

			if ( _sound != NULL )
			{
				if ( _sound != NULL ) _sound[i]->release();
			}
		}
	}

	//�޸� �����
	SAFE_DELETE_ARR( _channel );
	SAFE_DELETE_ARR( _sound );

	//�ý��� �ݱ�
	if ( _system != NULL )
	{
		_system->release();
		_system->close();
	}
}

//���� �߰�
void cSoundManager::addSound( string keyName, string soundName, bool background, bool loop )
{
	//�����뷡��?
	if ( loop )
	{
		//������̳�?
		if ( background )
		{
			_system->createStream( soundName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSounds.size()] );
		}
		else
		{
			_system->createSound( soundName.c_str(), FMOD_DEFAULT, 0, &_sound[_mTotalSounds.size()] );
		}
	}
	//���� �ƴϴ�
	else
	{
		//�ѹ��� �÷���~~
		_system->createSound( soundName.c_str(), FMOD_DEFAULT, 0, &_sound[_mTotalSounds.size()] );
	}

	//�ʿ� ���带 Ű���� �Բ� �־��ش�
	_mTotalSounds.insert( make_pair( keyName, &_sound[_mTotalSounds.size()] ) );
}

//���� �÷���
void cSoundManager::play( string keyName, float volume )
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;
	for ( iter; iter != _mTotalSounds.end(); ++iter, count++ )
	{
		if ( keyName == iter->first )
		{
			//���� �÷���~~~
			_system->playSound( FMOD_CHANNEL_FREE, *iter->second, false, &_channel[count] );

			//���� ����
			_channel[count]->setVolume( volume );
			break;
		}
	}
}

//���� ����
void cSoundManager::stop( string keyName )
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;
	for ( iter; iter != _mTotalSounds.end(); ++iter, count++ )
	{
		if ( keyName == iter->first )
		{
			//���� ����~~~
			_channel[count]->stop();
			break;
		}
	}
}

//���� �Ͻ�����
void cSoundManager::pause( string keyName )
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;
	for ( iter; iter != _mTotalSounds.end(); ++iter, count++ )
	{
		if ( keyName == iter->first )
		{
			//���� ����~~~
			_channel[count]->setPaused( true );
			break;
		}
	}
}

//���� �����
void cSoundManager::resume( string keyName )
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;
	for ( iter; iter != _mTotalSounds.end(); ++iter, count++ )
	{
		if ( keyName == iter->first )
		{
			//���� ����~~~
			_channel[count]->setPaused( false );
			break;
		}
	}
}

//�Ͻ����� �Ǿ���?
bool cSoundManager::isPauseSound( string keyName )
{
	bool isPause;
	int count = 0;
	arrSoundsIter iter = _mTotalSounds.begin();

	for ( iter; iter != _mTotalSounds.end(); ++iter, count++ )
	{
		if ( keyName == iter->first )
		{
			//���� ����~~~
			_channel[count]->getPaused( &isPause );
			break;
		}
	}
	return isPause;
}

//�÷��� ���̳�?
bool cSoundManager::isPlaySound( string keyName )
{
	bool isPlay;
	int count = 0;
	arrSoundsIter iter = _mTotalSounds.begin();

	for ( iter; iter != _mTotalSounds.end(); ++iter, count++ )
	{
		if ( keyName == iter->first )
		{
			//���� ����~~~
			_channel[count]->isPlaying( &isPlay );
			break;
		}
	}
	return isPlay;
}

//Fmod �ý����� ����
void cSoundManager::update( void )
{
	//���� System ��������� ������Ʈ
	_system->update();

	//������ �ٲ�ų�
	//����� ���� ���带 ä�ο��� �����µ��� �پ���
	//�۾��� �ڵ����� ���ش�
}

//�� �� ¥����?
unsigned int cSoundManager::getLength( string keyName )
{
	arrSoundsIter iter = _mTotalSounds.begin();

	for ( iter; iter != _mTotalSounds.end(); ++iter )
	{
		if ( keyName == iter->first )
		{
			unsigned int length = 0;
			( *iter->second )->getLength( &length, FMOD_TIMEUNIT_MS );
			return length;
		}
	}

	return 0;
}