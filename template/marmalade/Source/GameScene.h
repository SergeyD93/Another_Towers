#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "cocos2d.h"
#include "Objects/Wave.h"
#include "Objects/IdleWave.h"
#include "Objects/Player.h"
#include "Objects/Coin.h"
#include "Objects/Chest.h"
#include "Utils/b2DebugDraw.h"
#include "Properties/defines.h"
#include "Gadgets/Dolphin.h"
#include "Gadgets/Helicopter.h"
#include "Gadgets/Gadget.h"
#include "s3eOSReadString.h"
#include "DataProcessor/LoadObjects.h"
#include "DataProcessor/Background.h"
#include "DataProcessor/Downloader.h"
#include "DataProcessor/Preloader.h"
#include "Dialog/ShopDialog.h"
#include "Dialog/TheEndDialog.h"
#include "Dialog/MissionsDialog.h"
#include "Dialog/ProfileDialog.h"
#include "Dialog/SettingsDialog.h"
#include "DataProcessor/TeachLayer.h"
#include "DataProcessor/TouchCounter.h"
#include "DebugLayer.h"
#include "DataProcessor/AdModule.h"

using namespace cocos2d;

struct DebugPinCode{
    bool isEnter;
    float x;
    float y;
};

class GameScene : public CCLayer, public GameCenterCallBackProtocol
{
public:	
	~GameScene();
    
    TeachLayer * m_pTeachLayer;
    TouchCounter * m_pTouchCounter;
    
	static cocos2d::CCScene* scene();

	bool init(); 

	CREATE_FUNC(GameScene);

	void onEnter();
	void onExit();

	int32 axisX;
	int32 axisY;
	int32 axisZ;
	float idleOffset;

	CCArray* gadgets;
	CCParticleFireworks *waterSplash;

	unsigned long d;
	int randGadget;
	char *dist;
	char *downFValue;
	CCLabelBMFont *stats;
	CCSprite* sharkSpr;
	CCLabelBMFont *_ratingCount;

	CCLabelBMFont *_moneyShow;
	CCSprite *_moneyIndikator;

	CCSize  sizeScene;	
	float scaleXparam;
	float scaleYparam;

	CCSprite * starMult;
	CCLabelBMFont * starMultTxt;

	Wave *waves;
	Wave *idleWaves;
	b2World * world;
	Player *player;

	bool touch;
	bool debugTouch;

	bool _ifLeftTimeAndSharkSignWeCanShow;
	void ifLeftTimeAndSharkSignWeCanShow(float dt);

	bool touchUp;
	bool touchDown;

	CCMenuItemImage *spinLeftButton;
	CCMenuItemImage *spinRightButton;
	
	void draw();
	void update(float dt);
	void setupWorld();

	void processTouch();
	void checkGadgetsPos();
	void refreshCoinLabel();
	void refreshRatingLabel();

	b2DebugDraw * debugDraw;
	float offsetY;

	bool sharkAnim;

	CCSprite * background1;
	CCSprite * background2;
	CCSprite * bar0;
	CCSprite * bar1;
	CCSprite * bar2;
	CCSprite * bar3;
	CCSprite *  _sharkSign;

	float backgroundAnimationCount;
	int sharSignAn;
	bool sharSignAnB;
	void backgroundAnimation(CCNode*, void* data) ;
	void sharkSignAnimation() ;
	void sharkAnumationKill(CCNode*, void* data);
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();

    
    void pauseGame();
	void unpauseGame();
	void resetGame();
	void endGame();

	void playInitialization();

	void endGameLayerShow(float dt);

	void playClick(CCObject* obj);
	void pauseClick(CCObject *obj);
	void resumeClick(CCObject *obj);
	void restartClick(CCObject *obj);
	void homeClick(CCObject *obj);
	void missionsClick(CCObject *obj);
	void profileClick(CCObject *obj);
	void storeClick(CCObject *obj);
	void settingsClick(CCObject *obj);
	void gCClick(CCObject *obj);
	void creditsClick(CCObject *obj);
	void bonusClick(CCObject *obj);
	void moreGamesClick(CCObject *obj);
	void backClicked(CCObject *obj);

	CCSprite * leftGadgetGrid;
	bool isleftGadgetShow;
	struct LeftGadgetGridtStr{
		bool on;
		int time;
		CCSprite * icon;
		CCLabelBMFont * timer;
		int type;
	};

	std::vector <LeftGadgetGridtStr> leftGadgetGridElements;

	void addGadgetToGridList(int idG, int time);
	void refreshLeftGadgetGrid(float dt);
	void clearLeftGadgetGridt();
	void deleteElementFromLeftGadgetGrif(int idG);
	bool hasElementInLeftGadgetGrif(int idG);

	void showSignBig(const std::string &name);
	CCSprite * signBig;

	
	int timerForStart;
	void tiktakForStart(float dt);
    
    bool isOnScooter;
    
	std::vector <DebugPinCode> firstPinCode;
	std::vector <DebugPinCode> secondPinCode;
    void showDebugLayer();
    void hideDebugLayer();
    
    CCSprite * panelMoney;
    CCSprite * panelRating;
    int moneyTest;
    
    float dxForRatingPanel;
    
    void testCount(float dt);
    bool isShowDebugLayer;
    
    bool isJetPack;
    void jetPackUpdate(float dt);
    void jetPackStop(float dt);
    bool isTouchJetPack;
    
    void saveInfoForTime(float dt);
    
    void animatedSharkAtStart(float dt);

	virtual void FriendsLeaderboardLoadComplete(){};
	virtual void LoginFALSE(){};
    
	bool isAnyWindowShowed(){ return (getChildByTag(WINDOW_TAG) != NULL); }
	bool isDebugSharkStatus(){ return m_lDebugLayer && m_lDebugLayer->isDebugSharkStatus(); }

protected:
	GameScene();

	CCMenuItem *m_mPlayItem;
	CCMenuItem *m_mPauseItem;
	CCMenuItem *m_mSettingsItem;

	DebugLayer *m_lDebugLayer;

	void pauseNode(CCNode *node);
	void unpauseNode(CCNode *node);

	void relocateIsland();
};

#endif
