#ifndef  __APP_DELEGATE_H__
#define  __APP_DELEGATE_H__

#include "CCApplication.h"

/**
@brief	The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by CCDirector.
*/
static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(2048.0f, 1536.0f);

typedef struct tagResource
{
	cocos2d::CCSize size;
	char directory[100];
}Resource;

static Resource iPhone		=  { cocos2d::CCSizeMake(960, 640),   "./Textures/960_640/" };
static Resource iPhone5		=  { cocos2d::CCSizeMake(1136, 640),  "./Textures/1136_640/" };
static Resource iPad		=  { cocos2d::CCSizeMake(1024, 768),  "./Textures/1024_768/"   };
static Resource iPadHD		=  { cocos2d::CCSizeMake(2048, 1536), "./Textures/2048_1536/" };
static Resource LowRes		=  { cocos2d::CCSizeMake(512, 384), "./Textures/512_384/" };

class  AppDelegate : private cocos2d::CCApplication
{
public:
		AppDelegate();
		virtual ~AppDelegate();

    virtual bool initInstance();
    virtual bool applicationDidFinishLaunching();
    virtual void applicationDidEnterBackground();
    virtual void applicationWillEnterForeground();
	void updateProjection(void);
};

void updateOrientation(s3eSurfaceOrientation* orien, void* userData);
void pauseCallback(void *systemdata, void *userdata);
void unPauseCallback(void *systemdata, void *userdata);
void backgroundCallback(void *systemdata, void *userdata);
void foregroundCallback(void *systemdata, void *userdata);
void exitCallback(void *systemdata, void *userdata);
void backKeyCallback(void *systemdata, void *userdata);

#endif

