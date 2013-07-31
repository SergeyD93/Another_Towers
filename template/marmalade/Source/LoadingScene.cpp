#include "s3eOSExec.h"
#include "LoadingScene.h"
#include "GameScene.h"
#include "DataProcessor/Items.h"
#include "DataProcessor/PlayerInfo.h"
#include "DataProcessor/LoadObjects.h"
#include "DataProcessor/PlayerView.h"
#include "DataProcessor/Quest.h"
#include "DataProcessor/NPC.h"
#include "DataProcessor/Settings.h"
#include "SimpleAudioEngine.h"
#include "DataProcessor/TextFromXML.h"
#include "DataProcessor/Windows.h"
#include "Utils/RateMe.h"
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
	CCSprite *bgFon = CCSprite::create("logo.png");
	bgFon->setPosition(ccp(sizeScene.width/2, sizeScene.height/2));
	addChild(bgFon);
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

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/background/bird.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/coins/coin.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/dolphin/dolphin.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/shark/shark_eat.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/player/hair.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/player/water_splash_2.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/player/water_splash_3.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/player/water_splash_4.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/player/splash.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/player/jetpack.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/player/dust.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/menuAnimation/flag.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/menuAnimation/girl.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/wave_gr/bubles.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/water_element.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game/foreground.plist");
	CCTextureCache::sharedTextureCache()->addImage("game/background/level1_1.png");
	CCTextureCache::sharedTextureCache()->addImage("game/background/level1_2.png");
	CCTextureCache::sharedTextureCache()->addImage("game/background/level1_3.png");
	CCTextureCache::sharedTextureCache()->addImage("game/background/level2_1.png");
	CCTextureCache::sharedTextureCache()->addImage("game/background/level2_2.png");
	CCTextureCache::sharedTextureCache()->addImage("game/background/level2_3.png");
	CCTextureCache::sharedTextureCache()->addImage("game/background/level3_1.png");
	CCTextureCache::sharedTextureCache()->addImage("game/background/level3_2.png");
	CCTextureCache::sharedTextureCache()->addImage("game/background/level3_3.png");
	CCTextureCache::sharedTextureCache()->addImage("game/background/level3_16.png");
	CCTextureCache::sharedTextureCache()->addImage("game/background/level3_12.png");
	CCTextureCache::sharedTextureCache()->addImage("game/background/level3_13.png");
	CCTextureCache::sharedTextureCache()->addImage("game/foreground/grt_MidleAndBot.png");
	CCTextureCache::sharedTextureCache()->addImage("game/foreground/grt_BotMidle.png");
	CCTextureCache::sharedTextureCache()->addImage("game/foreground/grt_TopBegin.png");
	CCTextureCache::sharedTextureCache()->addImage("game/foreground/grt_BotBegin.png");
	CCTextureCache::sharedTextureCache()->addImage("game/foreground/grt_TopEnd.png");
	CCTextureCache::sharedTextureCache()->addImage("game/foreground/grt_BotEnd.png");
	CCTextureCache::sharedTextureCache()->addImage("game/buoy.png");
	CCTextureCache::sharedTextureCache()->addImage("Fonts/MisterEarlBT.fnt");
	CCTextureCache::sharedTextureCache()->addImage("game/shop/icons/wings1.png");  
	TextFromXML::Instance();
	CCLog("load 1");
	CCLog("load 2");
	Items::Instance();
	CCLog("load 3");
	PlayerInfo::Instance();
	CCLog("load 4");
	LoadObjects::Instance();
	CCLog("load 5");
	LoadObjects::Instance()->setLevel(LoadObjects::Instance()->_levelList[0].c_str());
	//LoadObjects::Instance()->load();
	CCLog("load 6");
	PlayerView::Instance();
	CCLog("load 7");
	Quest::Instance();
	CCLog("load 8");
	NPC::Instance();
	CCLog("load 9");
	Windows::parseData();
	CCLog("load 10");
	RateMe::Instance();
	CCLog("load 11");

	CCLOG("Load sound!");
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(UserDataLocal::Instance()->_musicVolume);
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(UserDataLocal::Instance()->_soundVolume);
	CCLOG("1");
	SimpleAudioEngine::sharedEngine()->preloadEffect("./Sound/achievement.wav");
	CCLOG("22222");
	SimpleAudioEngine::sharedEngine()->preloadEffect("./Sound/Banzai_03.wav");
	CCLOG("3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("./Sound/button_click.wav");
	CCLOG("4");
	SimpleAudioEngine::sharedEngine()->preloadEffect("./Sound/buy.wav");
	CCLOG("5");
	SimpleAudioEngine::sharedEngine()->preloadEffect("./Sound/gadget_activation.wav");
	CCLOG("6");
	SimpleAudioEngine::sharedEngine()->preloadEffect("./Sound/game_over.wav");
	CCLOG("7");
	SimpleAudioEngine::sharedEngine()->preloadEffect("./Sound/Gul_01.wav");
	CCLOG("8");
	SimpleAudioEngine::sharedEngine()->preloadEffect("./Sound/open_chest.wav");
	CCLOG("9");
	SimpleAudioEngine::sharedEngine()->preloadEffect("./Sound/Pain_02.wav");
	CCLOG("10");
	SimpleAudioEngine::sharedEngine()->preloadEffect("./Sound/play_button.wav");
	CCLOG("11");
	SimpleAudioEngine::sharedEngine()->preloadEffect("./Sound/reincarnation.wav");
	CCLOG("12");
	SimpleAudioEngine::sharedEngine()->preloadEffect("./Sound/Splash_01.wav");
	CCLOG("13");
	SimpleAudioEngine::sharedEngine()->preloadEffect("./Sound/surfer_slide.wav");
	CCLOG("14");
	SimpleAudioEngine::sharedEngine()->preloadEffect("./Sound/take_coin_v.2.wav");
	CCLOG("15");
	SimpleAudioEngine::sharedEngine()->preloadEffect("./Sound/window_appear.wav");
	SimpleAudioEngine::sharedEngine()->preloadEffect("./Sound/dolphin.wav");
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
