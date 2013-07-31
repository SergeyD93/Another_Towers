//
//  Utils.h
//  Mystery_slots
//
//  Created by MacBook Pro on 09.11.12.
//
//

#ifndef __Mystery_slots__Utils__
#define __Mystery_slots__Utils__

#include "cocos2d.h"
using namespace cocos2d;

typedef std::vector<std::string> strArray;

class Utils
{
    
    
public:
    static CCAnimate * createAnimation(std::string fileName, std::string path, float time, int size);
	static CCAnimate* createAnimation(const std::string &path, const std::string &frameName, float time, unsigned from = 1, unsigned to = UINT_MAX);
	static void createAndRunAnimationFromFrameThenNextAction(CCSprite* sprite, std::string fileName, std::string path, float time, int startFrame, int size, CCFiniteTimeAction* nextAction);
    static void createAndRunAnimation(CCSprite* sprite, std::string fileName, std::string path, float time, int size, bool repirForever = false);
	static void createAndRunAnimation(CCSprite* sprite, std::string fileName, std::string path, float time, int delayPerAnimatinCycle, int size, bool repirForever = false);
	static void createAndRunAnimation(CCSprite* sprite, std::string framesName, float time, int size, bool repirForever = false);
	static CCSprite * createSpriteAndRunAnimation(std::string fileName, std::string path, float time, int size, bool repirForever = false);
    static void split(std::string src, const char* token, strArray& vect);
    static bool isIOS();
    static bool isAndroid();
    static bool isIphone();
    static bool isFree();
    static bool isDebug();
};

#endif /* defined(__Mystery_slots__Utils__) */
