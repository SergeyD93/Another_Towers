#include "s3eOSExec.h"
#include "LoadingScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;


// LoadingScene
LoadingScene::LoadingScene()
{
}

LoadingScene::~LoadingScene()
{  
	//CCDirector::sharedDirector()->purgeCachedData();
}

// on "init" you need to initialize your instance
bool LoadingScene::init()
{
	// super init first
	if (!CCLayer::init())
		return false;

	CCSize  sizeScene  = CCDirector::sharedDirector()->getWinSize();	
    CCLOG("This -");
	//CCSprite *bgFon = CCSprite::create("logo.png");
	//bgFon->setPosition(ccp(sizeScene.width/2, sizeScene.height/2));
	//addChild(bgFon);
    CCLOG("))");
	scheduleOnce(schedule_selector(LoadingScene::load), 0);

	return true;
}

void LoadingScene::menu_skip_callback(cocos2d::CCObject* sender)
{
	
}

void LoadingScene::load(float dt)
{
	/*Load All Info*/
	runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.1f), CCCallFunc::create(this, callfunc_selector(LoadingScene::intro_end))));
}

void LoadingScene::show_intro()
{
	
}

void LoadingScene::show_next(CCTime t)
{

}

void LoadingScene::intro_end()
{
	CCScene * scene = CCScene::create();    
	GameScene * layer = GameScene::create();    
	scene->addChild(layer);    
	CCDirector::sharedDirector()->replaceScene(scene);
}
