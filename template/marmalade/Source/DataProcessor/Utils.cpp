//
//  Utils.cpp
//  Mystery_slots
//
//  Created by MacBook Pro on 09.11.12.
//
//

#include "Utils.h"
#include <sstream>
#include "../Properties/defines.h"


CCAnimate * Utils::createAnimation(std::string fileName, std::string path, float time, int size)
{
    std::string fileN = path + fileName + ".plist";
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(fileN.c_str());
    CCArray *frames = CCArray::create();
    for (int i = 1; i <= size; i++)
    {
        std::ostringstream file;
        file << fileName << "_" << i << ".png";
        CCSpriteFrame * tmpFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(file.str().c_str());
        frames->addObject(tmpFrame);
    }    
	CCAnimation *animationSprite = CCAnimation::create(frames, time);
    frames->removeAllObjects();
    return CCAnimate::create(animationSprite);
}

CCAnimate* Utils::createAnimation( const std::string &path, const std::string &frameName, float time, unsigned from /*= 1*/, unsigned to /*= UINT_MAX*/ )
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(path.c_str());
	CCArray *frames = CCArray::create();
	for (unsigned i = from; i <= to; i++)
	{
		CCString *sprFrameName = CCString::createWithFormat("%s_%u.png", frameName.c_str(), i);
		CCSpriteFrame * sprFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(sprFrameName->getCString());
		if(sprFrame)
		{
			CCAnimationFrame* frame = new CCAnimationFrame();
			frame->initWithSpriteFrame(sprFrame, 1, NULL);
			frames->addObject(frame);
			frame->release();
		}
		else
		{
			break;
		}
	}    
	CCAnimation *animationSprite = CCAnimation::create(frames, time);
	return CCAnimate::create(animationSprite);
}

void Utils::createAndRunAnimation(CCSprite* sprite, std::string fileName, std::string path, float time, int size, bool repirForever)
{
	std::string fileN = path + fileName + ".plist";
    //CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(fileN.c_str());
    //CCLOG("=== %s", fileN.c_str());
	CCArray * frames = CCArray::create();
    for (int i = 1; i <= size; i++)
    {
        std::ostringstream file;
        file << fileName << "_" << i << ".png";
        CCSpriteFrame * tmpFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(file.str().c_str());
        
		CCAnimationFrame* frame = new CCAnimationFrame();
		frame->initWithSpriteFrame(tmpFrame, 1, NULL);
		frames->addObject(frame);
		frame->release();
    }
    
    CCAnimation *animationSprite = CCAnimation::create(frames, time);
    CCAnimate::create(animationSprite);

    frames->removeAllObjects();
	frames->release();

    if (repirForever)
    {
         sprite->runAction(CCRepeatForever::create(CCAnimate::create(animationSprite)));
    }
    else
    {
         sprite->runAction(CCAnimate::create(animationSprite));
    }
}

void Utils::createAndRunAnimation(CCSprite* sprite, std::string fileName, std::string path, float time, int delayPerAnimatinCycle, int size, bool repirForever)
{
	std::string fileN = path + fileName + ".plist";
	//CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(fileN.c_str());
	CCArray * frames = CCArray::create();
	for (int i = 1; i <= size; i++)
	{
		std::ostringstream file;
		file << fileName << "_" << i << ".png";
		CCSpriteFrame * tmpFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(file.str().c_str());

		CCAnimationFrame* frame = new CCAnimationFrame();
		frame->initWithSpriteFrame(tmpFrame, (i == size) ? delayPerAnimatinCycle : 1, NULL);
		frames->addObject(frame);
		frame->release();
	}

	CCAnimation *animationSprite = CCAnimation::create(frames, time);
	CCAnimate::create(animationSprite);

	frames->removeAllObjects();
	frames->release();

	if (repirForever)
	{
		sprite->runAction(CCRepeatForever::create(CCAnimate::create(animationSprite)));
	}
	else
	{
		sprite->runAction(CCAnimate::create(animationSprite));
	}
}

void Utils::createAndRunAnimationFromFrameThenNextAction(CCSprite* sprite, std::string fileName, std::string path, float time, int startFrame, int size, CCFiniteTimeAction* nextAction)
{
	std::string fileN = path + fileName + ".plist";
	//CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(fileN.c_str());
	CCArray * frames = CCArray::create();
	int i = startFrame;
	for (; i <= size; i++)
	{
		std::ostringstream file;
		file << fileName << "_" << i << ".png";
		CCSpriteFrame * tmpFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(file.str().c_str());

		CCAnimationFrame* frame = new CCAnimationFrame();
		frame->initWithSpriteFrame(tmpFrame, 1, NULL);
		frames->addObject(frame);
		frame->release();
	}

	CCAnimation *animationSprite = CCAnimation::create(frames, time);
	CCAnimate::create(animationSprite);

	frames->removeAllObjects();
	frames->release();

	sprite->runAction(CCSequence::create(CCAnimate::create(animationSprite), nextAction, NULL));
}

void Utils::createAndRunAnimation(CCSprite* sprite, std::string framesName, float time, int size, bool repirForever)
{
	CCArray * frames = CCArray::create();
	for (int i = 1; i <= size; i++)
	{
		std::ostringstream file;
		file << framesName << "_" << i << ".png";
		CCSpriteFrame * tmpFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(file.str().c_str());

		CCAnimationFrame* frame = new CCAnimationFrame();
		frame->initWithSpriteFrame(tmpFrame, 1, NULL);
		frames->addObject(frame);
		frame->release();
	}

	CCAnimation *animationSprite = CCAnimation::create(frames, time);
	CCAnimate::create(animationSprite);

	frames->removeAllObjects();
	frames->release();

	if (repirForever)
	{
		sprite->runAction(CCRepeatForever::create(CCAnimate::create(animationSprite)));
	}
	else
	{
		sprite->runAction(CCAnimate::create(animationSprite));
	}
}

CCSprite* Utils::createSpriteAndRunAnimation(std::string fileName, std::string path, float time, int size = 10, bool repirForever)
{
    std::string fileN = path + fileName + ".plist";
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(fileN.c_str());
    CCArray * frames = CCArray::create();
    for (int i = 1; i <= size; i++)
    {
        std::ostringstream file;
        file << fileName << "_" << i << ".png";
        CCSpriteFrame * tmpFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(file.str().c_str());
        frames->addObject(tmpFrame);
    }
    
    CCAnimation *animationSprite = CCAnimation::create(frames, time);
    CCAnimate::create(animationSprite);
	std::ostringstream file;
    file << fileName << "_0.png";
	CCSprite * sprite = CCSprite::createWithSpriteFrameName(file.str().c_str());
    frames->removeAllObjects();
    if (repirForever)
    {
         sprite->runAction(CCRepeatForever::create(CCAnimate::create(animationSprite)));
    }
    else
    {
         sprite->runAction(CCAnimate::create(animationSprite));
    }
    return sprite;
}

void Utils::split(std::string src, const char* token, strArray& vect)
{
    int nend=0;
    int nbegin=0;
    while(nend != -1)
    {
        nend = src.find(token, nbegin);
        if(nend == -1)
            vect.push_back(src.substr(nbegin, src.length()-nbegin));
        else
            vect.push_back(src.substr(nbegin, nend-nbegin));
        nbegin = nend + strlen(token);
    }
}

bool Utils::isIOS()
{
    if (IS_SIMUL){
        return true;
    }else{
        return s3eDeviceGetInt(S3E_DEVICE_OS) == S3E_OS_ID_IPHONE;
    }
}

bool Utils::isAndroid()
{
    return s3eDeviceGetInt(S3E_DEVICE_OS) == S3E_OS_ID_ANDROID;
}

bool Utils::isIphone(){
    int param;
    s3eConfigGetInt("MySettings", "isIphone", &param);
    return param;
}

bool Utils::isFree(){
    int param;
    s3eConfigGetInt("MySettings", "isFree", &param);
    return param;
}

bool Utils::isDebug(){
    int param;
    s3eConfigGetInt("MySettings", "isDebug", &param);
    return param;
}