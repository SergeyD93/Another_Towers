#include "GameScene.h"
#include "string.h"
#include "pugixml.hpp"
#include "time.h"
#include "SimpleAudioEngine.h"
#include "s3eOSExec.h"


using namespace CocosDenshion;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	
}

CCScene* GameScene::scene()
{
	CCScene *scene = CCScene::create();
	GameScene *layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameScene::init()
{
	if (!CCLayer::init())
		return false;


	return true;
}


void GameScene::update(float dt)
{

}

bool GameScene::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
    CCPoint touchPoint = pTouch->getLocationInView();
    return true;
}

void GameScene::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{

}

void GameScene::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
}

void GameScene::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}



void GameScene::onEnter()
{
	CCLayer::onEnter();
}

void GameScene::onExit()
{
	CCLayer::onExit();
}
