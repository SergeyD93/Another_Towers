#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "cocos2d.h"
#include "s3eOSReadString.h"


using namespace cocos2d;

struct DebugPinCode{
    bool isEnter;
    float x;
    float y;
};

class GameScene : public CCLayer
{
public:	
	~GameScene();
    
	static cocos2d::CCScene* scene();

	bool init(); 

	CREATE_FUNC(GameScene);

	void onEnter();
	void onExit();

	void update(float dt);

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();

    
protected:
	GameScene();
};

#endif
