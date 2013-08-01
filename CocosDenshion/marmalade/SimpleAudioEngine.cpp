/****************************************************************************
Copyright (c) 2010 cocos2d-x.org
Copyright (c) 2011 Giovanni Zito

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "SimpleAudioEngine.h"
#include "s3e.h"
#include "IwUtil.h"
//#include "../../template/marmalade/Source/DataProcessor/UserDataLocal.h"
//#include "UserDataLocal.h"
//#include "UserDataLocal.h"

#include <string>
#include <map>
#include <sstream>
#include "cocos2d.h"
//#include "s3eDialog.h"
using namespace cocos2d;

using namespace std ;

#define	GEOM_ONE_FP8		0x100		// volume is in .8 fixed point format

struct RiffHeader
{
    char m_ChunkID[4];
    int32 m_ChunkSize;
    char m_RIFFType[4];
    
    RiffHeader()
    {
        memcpy(m_ChunkID, "RIFF", 4);
        memcpy(m_RIFFType, "WAVE", 4);
    }
};

struct Chunk
{
    char m_ChunkID[4];
    uint32 m_ChunkSize;
};

int16* LoadWav(const char* filename, int* sizeOut)
{
    RiffHeader header;
    Chunk chunk;
    
    int16* rtn = NULL;
    FILE* f = fopen(filename, "r");
    if (!f) 
    {
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    fseek(f, 0, SEEK_SET);
    fread(&header, 1, sizeof(header), f);
    while (1)
    {
        fread(&chunk, 1, sizeof(chunk), f);
        if (!strncmp(chunk.m_ChunkID, "data", 4))
        {
            rtn = (int16*)malloc(chunk.m_ChunkSize);
            fread(rtn, 1, chunk.m_ChunkSize, f);
            *sizeOut = chunk.m_ChunkSize;
            break;
        }
        fseek(f, chunk.m_ChunkSize, SEEK_CUR);
    }
    
    fclose(f);
    return rtn;
}

namespace CocosDenshion
{
	class SoundFx {
	public:
		SoundFx() : data(0), size(0) {}
		SoundFx(void* pData, int32 nSize) : data((int16*)pData), size(nSize) {}

		int16*	data ;
		int32	size ;
	};

	static int16*		g_AudioBuffer = 0;
	static int32		g_AudioFileSize = 0;
	static bool muteSound = false;
	static bool muteMusic = false;

	typedef map<string,SoundFx> SoundFxMap ;
	static SoundFxMap* g_pSoundFxMap = 0 ;
	
	static SimpleAudioEngine *s_pEngine = 0 ;

	SimpleAudioEngine::SimpleAudioEngine():
		_fadeInTo(0),_fadeOut(false),_fadeStep(0),m_sNextMusic(""),m_bNextLoop(false)
	{
		g_pSoundFxMap = new SoundFxMap() ;
	}

	SimpleAudioEngine::~SimpleAudioEngine()
	{
	}

	SimpleAudioEngine* SimpleAudioEngine::sharedEngine()
	{
		if (! s_pEngine) {
			s_pEngine = new SimpleAudioEngine();
            s3eSoundSetInt(S3E_SOUND_DEFAULT_FREQ, 44100);
		}
        
		return s_pEngine;
	}

	void SimpleAudioEngine::end()
	{
		s3eAudioStop();

		if( g_AudioBuffer) {
			free(g_AudioBuffer);
			g_AudioBuffer = 0 ;
		}

		if(s_pEngine) {
			s_pEngine->stopAllEffects() ;
			delete s_pEngine;
			s_pEngine=0;
		} 

		for( SoundFxMap::iterator it = g_pSoundFxMap->begin(); it!=g_pSoundFxMap->end(); it++ ) {
			if( it->second.data != 0 ) free( it->second.data ) ;
		}
		if( g_pSoundFxMap ) {
			delete g_pSoundFxMap ;
			g_pSoundFxMap = 0 ;
		}
	}

	void SimpleAudioEngine::setResource(const char* pszZipFileName)
	{
		// todo
	}

    void SimpleAudioEngine::preloadBackgroundMusic(const char* pszFilePath)
	{
		s3eFile *fileHandle = s3eFileOpen(pszFilePath, "rb");
		
		IwAssertMsg(GAME, fileHandle, ("Open file %s Failed. s3eFileError Code : %i", pszFilePath, s3eFileGetError()));
		
		g_AudioFileSize = s3eFileGetSize(fileHandle);
		g_AudioBuffer = (int16*)malloc(g_AudioFileSize);
		memset(g_AudioBuffer, 0, g_AudioFileSize);
		s3eFileRead(g_AudioBuffer, g_AudioFileSize, 1, fileHandle);
		s3eFileClose(fileHandle);
	}

    void SimpleAudioEngine::preloadEffect(const char* pszFilePath)
	{
		SoundFxMap::iterator it = g_pSoundFxMap->find(pszFilePath) ;
		if( it==g_pSoundFxMap->end() ) {
//			s3eFile *fileHandle = s3eFileOpen(pszFilePath, "rb");
//            
//			IwAssertMsg(GAME, fileHandle, ("Open file %s Failed. s3eFileError Code : %i", pszFilePath, s3eFileGetError()));
//            
			int32 fileSize;// = s3eFileGetSize(fileHandle);
			int16* buff = LoadWav(pszFilePath, &fileSize);//(int16*)malloc(fileSize);
//            
			(*g_pSoundFxMap)[pszFilePath] = SoundFx(buff,fileSize) ;
//			memset(buff, 0, fileSize);
//			s3eFileRead(buff, fileSize, 1, fileHandle);
//			s3eFileClose(fileHandle);
		}
	}
    
	void SimpleAudioEngine::playBackgroundMusic(const char* pszFilePath, bool bLoop)
	{
//        CCLog("PLAY MUSIC %s", pszFilePath);
        if (s3eDeviceGetInt(S3E_DEVICE_UNIQUE_ID) == 1234567890) 
        {
            //return;
        }
//        if (s3eAudioGetInt(S3E_AUDIO_NUM_CHANNELS) != 1)
//        {
//            s3eAudioSetInt(S3E_AUDIO_CHANNEL, 2);
//        }
		s3eResult result;
		
		result = s3eAudioPlayFromBuffer(g_AudioBuffer, g_AudioFileSize, bLoop ? 0 : 1);
		
		if ( result == S3E_RESULT_ERROR)
		{
			result = s3eAudioPlay(pszFilePath, bLoop ? 0 : 1);
		}
		
		if ( result == S3E_RESULT_ERROR) 
		{
			IwAssert(GAME, ("Play music %s Failed. Error Code : %s", pszFilePath, s3eAudioGetErrorString()));
		}

	    if (muteMusic)
		{
			setBackgroundMusicVolume(0.0);
		}

		m_sNextMusic = "";
		m_bNextLoop = false;
	}

	void SimpleAudioEngine::stopBackgroundMusic(bool bReleaseData)
	{
//        if (s3eAudioGetInt(S3E_AUDIO_NUM_CHANNELS) != 1)
//        {
//            s3eAudioSetInt(S3E_AUDIO_CHANNEL, 1);
//        }
		stopMusicFadding();
		s3eAudioStop();
		
		if (bReleaseData)
		{
			free(g_AudioBuffer);
		}
	}

	void SimpleAudioEngine::pauseBackgroundMusic()
	{
		s3eAudioPause();
	}

	void SimpleAudioEngine::resumeBackgroundMusic()
	{
		s3eAudioResume();
	} 

	void SimpleAudioEngine::rewindBackgroundMusic()
	{
		//todo
	}

	bool SimpleAudioEngine::willPlayBackgroundMusic()
	{
		// todo
		return true;
	}

	bool SimpleAudioEngine::isBackgroundMusicPlaying()
	{
		return s3eAudioIsPlaying() == S3E_TRUE ;
	}

	float SimpleAudioEngine::getBackgroundMusicVolume()
	{

		return s3eAudioGetInt(S3E_AUDIO_VOLUME) / (float)GEOM_ONE_FP8;
	}

	void SimpleAudioEngine::setBackgroundMusicVolume(float volume)
	{
		//if (muteMusic) return;
	    if (muteMusic) volume = 0.0;
		s3eAudioSetInt(S3E_AUDIO_VOLUME, (int32)(volume * (float)GEOM_ONE_FP8));
	}

	float SimpleAudioEngine::getEffectsVolume()
	{
		return s3eSoundGetInt(S3E_SOUND_VOLUME) / (float)GEOM_ONE_FP8;
	}

	void SimpleAudioEngine::setEffectsVolume(float volume)
	{	
		if (muteSound) return;
		s3eSoundSetInt(S3E_SOUND_VOLUME, (int32)(volume * (float)GEOM_ONE_FP8));
	}

	unsigned int SimpleAudioEngine::playEffect(const char* pszFilePath, bool bLoop)
	{
		if (muteSound) return 0;
//        return 0;
		SoundFxMap::iterator it = g_pSoundFxMap->find(pszFilePath) ;
		
		int16* buff = 0 ;
		if( it==g_pSoundFxMap->end() ) {
			preloadEffect(pszFilePath) ;
		}
        
        buff = (*g_pSoundFxMap)[pszFilePath].data ;
        int size = (*g_pSoundFxMap)[pszFilePath].size;
        int channel;
//        if (s3eAudioGetInt(S3E_AUDIO_NUM_CHANNELS) == 1) 
//        {
            channel = s3eSoundGetFreeChannel();
        int volume = s3eSoundGetInt(S3E_SOUND_VOLUME);
            s3eSoundChannelSetInt(channel, S3E_CHANNEL_VOLUME, volume);
            s3eSoundChannelPlay(channel, buff, (*g_pSoundFxMap)[pszFilePath].size/2, (bLoop ? 0 : 1), 0);
//            CCLog("Play = %s %i ch = %i vol = %i", pszFilePath, size, channel, volume);
            if (s3eSoundGetError()!= S3E_SOUND_ERR_NONE) {
//                IwAssertMsg(GAME, false, ("Play sound %s Failed. Error Code : %s", pszFilePath, s3eSoundGetErrorString()));	
            }
//        }
//        else
//        {
//            s3eAudioSetInt(S3E_AUDIO_CHANNEL, 1);
////            s3eAudioPlay(pszFilePath, 1);
//            s3eResult result = s3eAudioPlayFromBuffer(buff, size, 1);
//            if (result == S3E_RESULT_ERROR) 
//            {
//                ostringstream str;
//                str << s3eAudioGetError() << " Error";
//                
//                char *h = new char[40];
//                strcpy(h, str.str().c_str());
//                CCLog("%s", h);
//                s3eDialogAlertInfo info;
//                info.m_Title= h;
//                info.m_Message = h;
//                info.m_Button[0] = "close";
//                s3eDialogAlertBox(&info);
//            }
//        }

		return channel;

	}
    
    unsigned int SimpleAudioEngine::playEffect(const char* pszFilePath, float volume)
	{
		if (muteSound) return 0;
        //        return 0;
//        CCLog("%s", pszFilePath);
		SoundFxMap::iterator it = g_pSoundFxMap->find(pszFilePath) ;
		
		int16* buff = 0 ;
		if( it==g_pSoundFxMap->end() ) 
        {
			preloadEffect(pszFilePath) ;
		}
        
        buff = (*g_pSoundFxMap)[pszFilePath].data ;
        int size = (*g_pSoundFxMap)[pszFilePath].size;
//        CCLog("Play = %s %i", pszFilePath, size);
        int channel = s3eSoundGetFreeChannel();
//        CCLog("Play = %s %i vol = %f ch = %i", pszFilePath, size, volume, channel);
        s3eSoundChannelSetInt(channel, S3E_CHANNEL_VOLUME, (int32)(volume * (float)GEOM_ONE_FP8));
        s3eSoundChannelPlay(channel, buff, (*g_pSoundFxMap)[pszFilePath].size/2, 1, 0);
        
        if (s3eSoundGetError()!= S3E_SOUND_ERR_NONE) {
//            IwAssertMsg(GAME, false, ("Play sound %s Failed. Error Code : %s", pszFilePath, s3eSoundGetErrorString()));	
        }
		return channel;
	}

	void SimpleAudioEngine::stopEffect(unsigned int nSoundId)
	{
		s3eSoundChannelStop(nSoundId);
	}

	void SimpleAudioEngine::unloadEffect(const char* pszFilePath)
	{
		// effect must not be playing!

		SoundFxMap::iterator it = g_pSoundFxMap->find(pszFilePath) ;
		if( it != g_pSoundFxMap->end() ) {
			free(it->second.data) ;
			g_pSoundFxMap->erase(it) ;
		}
	}

	void SimpleAudioEngine::pauseEffect(unsigned int nSoundId)
	{
		s3eSoundChannelPause(nSoundId);
	}

	void SimpleAudioEngine::pauseAllEffects()
	{
		s3eSoundPauseAllChannels();
	}

	void SimpleAudioEngine::resumeEffect(unsigned int nSoundId)
	{
		s3eSoundChannelResume(nSoundId);
	}

	void SimpleAudioEngine::resumeAllEffects()
	{
		s3eSoundResumeAllChannels();
	}

	void SimpleAudioEngine::stopAllEffects()
	{
		s3eSoundStopAllChannels();
	}
	void SimpleAudioEngine::setMuteMusic(bool mute)
	{
	    muteMusic = mute;
	}

	void SimpleAudioEngine::setMuteSound(bool mute)
	{
	    muteSound = mute;
	}

	bool SimpleAudioEngine::getMuteMusic(){ return muteMusic;};

	bool SimpleAudioEngine::getMuteSound(){ return muteSound;};

	void SimpleAudioEngine::fadeInMusic(float fadeStep, float volumeDelta)
	{
		_fadeStep = fadeStep;
		_fadeOut = false;
		_fadeInTo = (muteMusic ? 0 : volumeDelta);
	}

	void SimpleAudioEngine::fadeOutMusic(float fadeStep)
	{
		_fadeStep = fadeStep;
		_fadeOut = true;
		_fadeInTo = 0;
	}

	void SimpleAudioEngine::stopMusicFadding()
	{
		_fadeOut = false;
		_fadeInTo = 0;

		m_sNextMusic = "";
		m_bNextLoop = false;
	}

	void SimpleAudioEngine::update()
	{
		float bgMusicVolum = getBackgroundMusicVolume();

		if (_fadeOut)
		{
			setBackgroundMusicVolume(bgMusicVolum - _fadeStep);
			if (getBackgroundMusicVolume() <= 0)
			{
				_fadeOut = false;
				if(!m_sNextMusic.empty())
				{
					playBackgroundMusic(m_sNextMusic.c_str(), m_bNextLoop);
					//fadeInMusic(_fadeStep, UserDataLocal::Instance()->_musicVolume);
				}
			}
		}
		if (_fadeInTo > 0.0f)
		{
			if(muteMusic || bgMusicVolum >= _fadeInTo)
				_fadeInTo = 0.0f;
			else 
				setBackgroundMusicVolume(bgMusicVolum + _fadeStep);
		}
	}

	void SimpleAudioEngine::playNextWithFade( const std::string &next, bool loop, float fadeStep /*= 0.5f*/)
	{
		stopMusicFadding();
		m_sNextMusic = next;
		m_bNextLoop = loop;
		fadeOutMusic(fadeStep);
	}
}