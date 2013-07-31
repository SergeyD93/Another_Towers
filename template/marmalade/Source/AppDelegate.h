#ifndef  __APP_DELEGATE_H__
#define  __APP_DELEGATE_H__

#include "CCApplication.h"

/**
@brief	The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by CCDirector.
*/
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

