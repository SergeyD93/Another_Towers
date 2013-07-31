#include "AppDelegate.h"
#include "cocos2d.h"
#include "LoadingScene.h"
#include "Utils/Box2dHelper.h"
#include "IwGL.h"
#include "SimpleAudioEngine.h"
#include "Properties/defines.h"
#include "ControllerAll.h"
#include "DataProcessor/PlayerInfo.h"
#include "DataProcessor/AdModule.h"
#include "DataProcessor/UserDataLocal.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::initInstance()
{
	return true;
}

bool AppDelegate::applicationDidFinishLaunching()
{
	srand(time(NULL));
	 // (?)
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1.0f);
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0f);	
	//SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("./Sound/Surf_Menu_Reggae2.mp3");
	//s3eSoundSetInt( S3E_SOUND_DEFAULT_FREQ , 44100 );
	CCDirector *pDirector = CCDirector::sharedDirector();
	pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
	pDirector->setProjection(kCCDirectorProjection2D);
	s3eSurfaceRegister(S3E_SURFACE_SCREENSIZE, (s3eCallback)updateOrientation, this);
	s3eDeviceRegister(S3E_DEVICE_PAUSE,		   (s3eCallback)pauseCallback, NULL);
	s3eDeviceRegister(S3E_DEVICE_UNPAUSE ,	   (s3eCallback)unPauseCallback, NULL);
	s3eDeviceRegister(S3E_DEVICE_FOREGROUND,   (s3eCallback)foregroundCallback, NULL);
	s3eDeviceRegister(S3E_DEVICE_EXIT ,		   (s3eCallback)exitCallback, NULL);
	s3eDeviceRegister(S3E_DEVICE_BACKGROUND,   (s3eCallback)backgroundCallback, NULL);
	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT,(s3eCallback)backKeyCallback, NULL);
	updateProjection();
    UserDataLocal::Instance();
    AdModule::currentModule()->loginAllSystem();
	// Set the design resolution

	CCSize frameSize = CCEGLView::sharedOpenGLView()->getFrameSize();
	float aspect = frameSize.width / frameSize.height;
	if(aspect < 1024.0f/768.0f || aspect > 1.8f)
	{
		pDirector->getOpenGLView()->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionShowAll);
	}
	else
	{
		pDirector->getOpenGLView()->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionExactFit);
	}

	if(Utils::isIOS())
	{
		if(Utils::isIphone())
		{
			CCFileUtils::sharedFileUtils()->setResourceDirectory(iPhone.directory);
			pDirector->setContentScaleFactor(iPhone.size.height/designResolutionSize.height);
		}
		else
		{
			if ((frameSize.width >= iPadHD.size.width) || (frameSize.height >= iPadHD.size.height))
			{ 
				CCFileUtils::sharedFileUtils()->setResourceDirectory(iPadHD.directory);
				pDirector->setContentScaleFactor(iPadHD.size.height/designResolutionSize.height);
			}
			else
			{ 
				CCFileUtils::sharedFileUtils()->setResourceDirectory(iPad.directory);
				pDirector->setContentScaleFactor(iPad.size.height/designResolutionSize.height);
			}
		}
	}
	else if(Utils::isAndroid())
	{
		if ((frameSize.width >= iPadHD.size.width) || (frameSize.height >= iPadHD.size.height))
		{
            CCLOG("This ?");
			CCFileUtils::sharedFileUtils()->setResourceDirectory(iPadHD.directory);
			pDirector->setContentScaleFactor(iPadHD.size.height/designResolutionSize.height);
		}
		else if ((frameSize.width >= iPad.size.width) || (frameSize.height >= iPad.size.height))
		{
            CCLOG("This !");
			CCFileUtils::sharedFileUtils()->setResourceDirectory(iPad.directory);
			pDirector->setContentScaleFactor(iPad.size.height/designResolutionSize.height);
		}
		else
		{
			CCFileUtils::sharedFileUtils()->setResourceDirectory(LowRes.directory);
			pDirector->setContentScaleFactor(LowRes.size.height/designResolutionSize.height);
		}
	}
	else
	{
		//assert(false, "UNKNOWN PLATFORM");
	}

	pDirector->setDisplayStats(true);
	//pDirector->setAnimationInterval(1.0 / 60);
	//pDirector->setDepthTest(false);
	//pDirector->enableRetinaDisplay(true);
	Box2dHelper::initBox2dHelper();
	CCScene * pScene = CCScene::create();
	CCLayer * pLayer = LoadingScene::create();
	pScene->addChild(pLayer);
	pDirector->runWithScene(pScene);
	return true;
}

void AppDelegate::applicationDidEnterBackground()
{
	CCDirector::sharedDirector()->stopAnimation();
	CCDirector::sharedDirector()->pause();
	// if you use SimpleAudioEngine, it must be pause
	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void AppDelegate::applicationWillEnterForeground()
{
	CCDirector::sharedDirector()->resume();
	CCDirector::sharedDirector()->startAnimation();

	// if you use SimpleAudioEngine, it must resume here
	SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    CCNotificationCenter::sharedNotificationCenter()->postNotification("foreground");
}

void AppDelegate::updateProjection(void)
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	s3eSurfaceBlitDirection direction = (s3eSurfaceBlitDirection)IwGLGetInt(IW_GL_ROTATE);

	kmGLMatrixMode(KM_GL_PROJECTION);
	kmGLLoadIdentity();
	kmMat4 orthoMatrix;
	if (direction == S3E_SURFACE_BLIT_DIR_ROT270
		||direction == S3E_SURFACE_BLIT_DIR_ROT90)
		kmMat4OrthographicProjection(&orthoMatrix, 0, size.height / CC_CONTENT_SCALE_FACTOR(), 0, size.width / CC_CONTENT_SCALE_FACTOR(), -1024, 1024 );
	else
		kmMat4OrthographicProjection(&orthoMatrix, 0, size.width / CC_CONTENT_SCALE_FACTOR(), 0, size.height / CC_CONTENT_SCALE_FACTOR(), -1024, 1024 );
	kmGLMultMatrix(&orthoMatrix);
	kmGLMatrixMode(KM_GL_MODELVIEW);
	kmGLLoadIdentity();
	switch(direction)
	{
	case S3E_SURFACE_BLIT_DIR_ROT270:
		kmGLRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		kmGLTranslatef(0.0f, -size.height, 0.0f);
		CCLOG("270");
		break;
	case S3E_SURFACE_BLIT_DIR_ROT90:
		kmGLRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
		kmGLTranslatef(-size.width, 0.0f, 0.0f);
		CCLOG("90");
		break;
	case S3E_SURFACE_BLIT_DIR_ROT180:
		kmGLRotatef(180.0f, 0.0f, 0.0f, 1.0f);
		kmGLTranslatef(-size.width, -size.height, 0.0f);
		CCLOG("180");
		break;
	}
}

void updateOrientation(s3eSurfaceOrientation* orien, void* userData)
{
	s3eDebugTracePrintf( "Orientation Changed: width=%d, height=%d, surfacewidth=%d, surfaceheight=%d, blitdirection=%s", orien->m_Width, orien->m_Height,   s3eSurfaceGetInt(S3E_SURFACE_WIDTH),   s3eSurfaceGetInt(S3E_SURFACE_HEIGHT), S3E_SURFACE_BLIT_DIR_ROT180 ? "180" : "0" );
	AppDelegate* game = (AppDelegate*)userData;
	game->updateProjection();
}

void pauseCallback( void *systemdata, void *userdata )
{
	if(!GAME_SCENE)
	{
		return;
	}

	CCLOG("PAUSE 1");
    if (!PlayerInfo::Instance()->idleState && !GAME_SCENE->isAnyWindowShowed())
	{
        GAME_SCENE->pauseClick(NULL);
    }
    CCLOG("PAUSE 2");
}

void unPauseCallback( void *systemdata, void *userdata )
{
    CCLog("UNPAUSE 1");
	CCLog("UNPAUSE 2");
}

void backgroundCallback( void *systemdata, void *userdata )
{
	CCLOG("BACKGROUND");
}

void foregroundCallback( void *systemdata, void *userdata )
{
	CCLOG("FOREGROUND");
}

void exitCallback( void *systemdata, void *userdata )
{
	CCLOG("EXIT");
}

void backKeyCallback( void *systemdata, void *userdata )
{
	s3eKeyboardEvent* event = (s3eKeyboardEvent*)systemdata;

	if(event->m_Key == s3eKeyAbsBSK && !event->m_Pressed)
	{
		CCNotificationCenter::sharedNotificationCenter()->postNotification("back_clicked");
	}
}
