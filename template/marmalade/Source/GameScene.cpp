#include "GameScene.h"
#include "string.h"
#include "pugixml.hpp"
#include "DataProcessor/Settings.h"
#include "Gadgets/Gadget.h"
#include "SurferContactListner.h"
#include "DataProcessor/PlayerView.h"
#include "DataProcessor/Items.h"
#include "Utils/Box2dHelper.h"
#include "Objects/WaterTrampoline.h"
#include "DataProcessor/NPC.h"
#include "ControllerAll.h"
#include "time.h"
#include "Editor.h"
#include "SimpleAudioEngine.h"
#include "DataProcessor/AdModule.h"
#include "DataProcessor/TextFromXML.h"
#include "Dialog/CreditsDialog.h"
#include "Dialog/PauseDialog.h"
#include "Dialog/BonusDialog.h"
#include "Dialog/BuyPaidVersionDialog.h"
#include "s3eOSExec.h"
#include "Utils/RateMe.h"
#include "Dialog/ExitDialog.h"

using namespace CocosDenshion;

GameScene::GameScene():
	m_lDebugLayer(NULL)
{
}

GameScene::~GameScene()
{
	delete world;
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

    isShowDebugLayer = false;
	sizeScene  = CCDirector::sharedDirector()->getWinSize();

	if(Settings::getResolution() == other){
		scaleXparam = sizeScene.width/MAX_WIN_SIZE_X;
		scaleYparam = sizeScene.height/MAX_WIN_SIZE_Y;
	}
	else {
		scaleXparam = 1;
		scaleYparam = 1;
	}
    
    DebugPinCode tmpCode;
    tmpCode.isEnter = false;
    tmpCode.x = 0.0f;
    tmpCode.y = 0.0f;
    firstPinCode.push_back(tmpCode);
    tmpCode.isEnter = false;
    tmpCode.x = 0.0f;
    tmpCode.y = 0.0f;
    firstPinCode.push_back(tmpCode);
	
	tmpCode.isEnter = false;
    tmpCode.x = 0.0f;
    tmpCode.y = 1536.0f;
	secondPinCode.push_back(tmpCode);
	tmpCode.isEnter = false;
    tmpCode.x = 0.0f;
    tmpCode.y = 1536.0f;
	secondPinCode.push_back(tmpCode);

	sharkAnim = false;
	this->setScaleX(PlayerInfo::Instance()->scaleX);
	//this->setScaleX(PlayerInfo::Instance()->scaleX * 0.25f);
	//setScaleY(0.25f);
	PlayerInfo::Instance()->restart(); 
	ControllerAll::Instance()->initGameScene(this);
	signBig = CCSprite::create();
	addChild(signBig, 100);
	_ratingCount = CCLabelBMFont::create("0","./Fonts/MisterEarlBT.fnt");
	_ratingCount->setAnchorPoint(ccp(0.0f, 0.5f));
 	_ratingCount->setScale(CCDirector::sharedDirector()->getContentScaleFactor());

    m_pTouchCounter = TouchCounter::create();
    addChild(m_pTouchCounter);
    
	starMult = CCSprite::create("game/star.png");
	

	starMultTxt = CCLabelBMFont::create("1","./Fonts/MisterEarlBT.fnt");
	starMultTxt->setPosition(ccp(starMult->getContentSize().width/2, starMult->getContentSize().height/2));
	starMultTxt->setScale(CCDirector::sharedDirector()->getContentScaleFactor()*0.8);
	starMult->addChild(starMultTxt);
    starMultTxt->setVisible(false);
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("./Sound/Surf_Menu_Reggae2.mp3", true);
	std::stringstream ss;
	ss<<UserDataLocal::Instance()->getMoney() << " ";
	_moneyShow = CCLabelBMFont::create(ss.str().c_str(), "./Fonts/MisterEarlBT.fnt");
	_moneyShow->setAnchorPoint(ccp(0.0f, 0.5f));
	//_moneyShow->setPosition(ccp(sizeScene.width / 2 + sizeScene.width / 8, sizeScene.height - sizeScene.height / 10));
	_moneyShow->setColor(ccc3(255, 255, 255));
	//_moneyShow->setVisible(false);
	_moneyShow->setScale(CCDirector::sharedDirector()->getContentScaleFactor());
	
    
    _moneyIndikator = CCSprite::create("game/coin.png");
	
    panelMoney = CCSprite::create("game/button_1.png");
    panelMoney->setPosition(ccp(sizeScene.width / 2 + sizeScene.width / 8, sizeScene.height - sizeScene.height / 10 + 5));
    addChild(panelMoney, 11);
    
    _moneyShow->setPosition(ccp(sizeScene.width / 2 + sizeScene.width / 8, sizeScene.height - sizeScene.height / 10 + 5));
    addChild(_moneyShow,11);
    _moneyIndikator->setPosition(ccp(sizeScene.width / 2 + sizeScene.width / 8 - 20, sizeScene.height - sizeScene.height / 10 + 5));
    addChild(_moneyIndikator,11);
    
    panelMoney->setVisible(false);
    _moneyShow->setVisible(false);
    _moneyIndikator->setVisible(false);
    
    panelRating = CCSprite::create("game/button_1.png");
    starMult->setPosition(ccp(sizeScene.width / 2 - sizeScene.width / 8 - starMult->getContentSize().width/4, sizeScene.height - sizeScene.height / 10 + starMult->getContentSize().height/16 + 5));
    _ratingCount->setPosition(ccp(sizeScene.width / 2 - sizeScene.width / 8 - starMult->getContentSize().width/4, sizeScene.height - sizeScene.height / 10 + starMult->getContentSize().height/16 + 5));
    panelRating->setPosition(ccp(sizeScene.width / 2 - sizeScene.width / 8 - starMult->getContentSize().width/4, sizeScene.height - sizeScene.height / 10 + 5));
    
    addChild(panelRating, 11);
    addChild(starMult, 11);
    addChild(_ratingCount, 11);
    panelRating->setVisible(false);
    starMult->setVisible(false);
    _ratingCount->setVisible(false);

	leftGadgetGrid = CCSprite::create();
	leftGadgetGrid->setContentSize(CCSize(250,1536));
	leftGadgetGrid->setAnchorPoint(ccp(0.0f, 0.0f));
	leftGadgetGrid->setPosition(CCPointZero);
	addChild(leftGadgetGrid,11);
	isleftGadgetShow = false;
    
    isOnScooter = false;

	ccBlendFunc m_tBlendFunc;
	m_tBlendFunc.src = GL_ONE;
	m_tBlendFunc.dst = GL_ZERO;
    CCLOG("--------- START LOAD BACKGROUND!!!");
	addChild(Background::Instance(), -10);
	Background::Instance()->setAnchorPoint(ccp(0.5f, 0.0f));
	Background::Instance()->setPosition(ccp(0.0f, 0.0f));
	Background::Instance()->setScaleX(1 / PlayerInfo::Instance()->scaleX);
	Background::Instance()->changeLevel(1, true);
	
	setupWorld();

	debugDraw = new b2DebugDraw(PTM);
	if (DRAW_BOX2D_LINES)
		world->SetDebugDraw(debugDraw);
	int32 flags = 0;
	flags += b2Draw::e_shapeBit;
 	flags += b2Draw::e_jointBit;
// 	flags += b2Draw::e_centerOfMassBit;
//	flags += b2Draw::e_aabbBit;
// 	flags += b2Draw::e_pairBit;
	debugDraw->SetFlags(flags);
	//setScale(0.35f);

 	stats = CCLabelBMFont::create("", "./Fonts/MisterEarlBT.fnt");
 	stats->setAnchorPoint(ccp(0,0));
 	stats->setPosition(ccp(1800, 0));
 	stats->setScale(CCDirector::sharedDirector()->getContentScaleFactor());
    stats->setVisible(false);
 	addChild(stats,11);

	/*Init all elements of gamescene*/
	//WAVE->init(world, false);
	//WAVE->resetWavesVertices();
	//WAVE->createBuoy();
	//addChild(WAVE, 2);

	this->setTouchEnabled(true);  

	if(PLAYER->getParent() != NULL)
		PLAYER->removeFromParentAndCleanup(true);

	PLAYER->initWithWorld(world);
	PLAYER->wake();
	PLAYER->inMainMenu = true;
	PLAYER_INFO->idleState = true;
	PLAYER->setVisible(true);
	PlayerView::Instance()->startAnimation();

	
	IDLE_WAVE->init(world, false);
	IDLE_WAVE->resetIdleWavesVertices();
	IDLE_WAVE->addChild(PLAYER, 2);
	addChild(IDLE_WAVE, 2);
	
	WAVE->init(world, false);

	touch = false;
	debugTouch = false;
	touchUp = false;
	touchDown = false;
	
	scheduleUpdate();

	m_mPauseItem = CCMenuItemImage::create("game/pause.png", "game/pausePressed.png", this, menu_selector(GameScene::pauseClick));
	m_mPauseItem->setAnchorPoint(ccp(1,1));
	m_mPauseItem->setPosition(ccp(sizeScene.width + 1300*(1-PlayerInfo::Instance()->scaleX), sizeScene.height - 40));
	m_mPauseItem->setVisible(false);

	m_mSettingsItem = CCMenuItemImage::create("game/menuSettings.png", "game/menuSettingsPressed.png", this, menu_selector(GameScene::settingsClick));
	m_mSettingsItem->setAnchorPoint(ccp(1,1));
	m_mSettingsItem->setPosition(ccp(sizeScene.width - 20 + 1300*(1-PlayerInfo::Instance()->scaleX), sizeScene.height - 20));

	m_mPlayItem = CCMenuItemImage::create("game/startGame.png", "game/startGamePressed.png", this, menu_selector(GameScene::playClick));
	m_mPlayItem->setPosition(ccp(sizeScene.width/2, sizeScene.height/3));
	
	CCMenu *controls = CCMenu::create(m_mPauseItem, m_mSettingsItem, m_mPlayItem, NULL);
	controls->setPosition(CCPointZero);
	addChild(controls, 10);

	_sharkSign = CCSprite::create("game/shark_sign.png");
	_sharkSign->setPosition(ccp(sizeScene.width / 10, sizeScene.height - sizeScene.height / 8));
	_sharkSign->setVisible(false);
	addChild(_sharkSign, 150);
	sharSignAn = 1;
	sharSignAnB = false;
	
	_ifLeftTimeAndSharkSignWeCanShow = false;
	scheduleOnce(schedule_selector(GameScene::ifLeftTimeAndSharkSignWeCanShow), 2.0f);

	std::stringstream nforMult;
	nforMult << PlayerInfo::Instance()->_multiplicator << "x";
	starMultTxt->setString(nforMult.str().c_str());
    
    refreshCoinLabel();
    refreshRatingLabel();
    dxForRatingPanel = 0.0f;
    isJetPack = false;
    
    
    m_pTeachLayer = TeachLayer::create();
    addChild(m_pTeachLayer, 100);
    
    GameCenter::current()->login();
    AdModule::currentModule()->getTJ_CurrencyValue();

	return true;
}

void GameScene::ifLeftTimeAndSharkSignWeCanShow(float dt){
	_ifLeftTimeAndSharkSignWeCanShow = true;
}

void GameScene::endGameLayerShow(float dt){	
	
	addChild(TheEndDialog::create(), 20);
	PLAYER->setVisible(false);
	_sharkSign->setVisible(false);
}

void GameScene::sharkSignAnimation(){
	if(PLAYER_INFO->idleState)
		return;

	if (sharSignAnB){
		sharSignAn *= -1;
		_sharkSign->setRotation(2.0f * sharSignAn);
		runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.5f), CCCallFunc::create(this, callfunc_selector(GameScene::sharkSignAnimation))));
	}else{
		_sharkSign->setRotation(0.0f);
	}
}


void GameScene::update(float dt)
{
	static double timeAccumulator = 0;

	timeAccumulator += dt;    
	if (timeAccumulator > (MAX_CYCLES_PER_FRAME * UPDATE_INTERVAL)) {
		timeAccumulator = UPDATE_INTERVAL;
	}    

	int32 velocityIterations = 6;
	int32 positionIterations = 4;

	//CCLOG("[ BEGIN WHILE STATEMENT ]");
	while (timeAccumulator >= UPDATE_INTERVAL) {
		//CCLOG("[ WHILE STATEMENT BEGIN ]");
		timeAccumulator -= UPDATE_INTERVAL;       

		PLAYER->limitVelocity();

		if(!PLAYER_INFO->idleState  && !WAVE->_editor){
			//HELICOPTER->limitVelocity();

			processTouch();
		}

		//CCLOG("[ WORLD STEP ]");
		world->Step(UPDATE_INTERVAL, velocityIterations, positionIterations);  
		//CCLOG("[ CLEAR FORCES ]");
		world->ClearForces();
	}

	//CCLOG("[ PLAYER UPDATE ]");
	PLAYER->update(dt);

	if(!PLAYER_INFO->idleState && !WAVE->_editor){
		DOLPHIN->update();
		//HELICOPTER->update();
        if (UserDataLocal::Instance()->_tutorial)
            m_pTeachLayer->update();
        
        if (!PlayerInfo::Instance()->showRecordPoint && UserDataLocal::Instance()->_bestSerf > 0 && UserDataLocal::Instance()->_bestSerf < PLAYER->body->GetPosition().x){
            PlayerInfo::Instance()->showRecordPoint = true;
            NPC::Instance()->addAchivement(TextFromXML::Instance()->getString("longestsurf"));
        }
	}

	//CCLOG("[ END GADGETS ERASING ]");

	float offsetX = PLAYER->getPosition().x;
	offsetY = 0;

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	float limit = winSize.height*3/4;
	float limit2 = limit/6;

	float scale = limit /  PLAYER->getPosition().y;
	if(scale < 0) {
		scale = limit /  (limit + (limit2 - 0));
	}
	else if ((scale < 6) && (scale > 1)){
		scale = 1;
		//offsetY = (PLAYER->getPositionY() - (PLAYER_RADIUS*scaleYparam) - winSize.height/5)*scale;
	}
	else if(scale > 6){
		scale = limit /  (limit + (limit2 - PLAYER->getPosition().y));
	}
	if(PLAYER->getPositionY() < winSize.height/3)
		offsetY = winSize.height/3 - PLAYER->getPositionY();
	if(PLAYER->getPositionY() < 0)
		offsetY = (winSize.height/3 + abs(PLAYER->getPositionY()* scale));
	if(PLAYER->getPositionY() < (-WAVES_DEEP*PTM)*scale + winSize.height/3){
		float waveDeep = (-WAVES_DEEP*PTM)*scale;
		float playerPos = PLAYER->getPositionY()*scale;
		float diff = waveDeep - playerPos;
		offsetY =  abs(playerPos + diff);
	}
	if(PLAYER->getPositionY() >= (WORLD_HEIGHT*PTM))
		scale = limit / (WORLD_HEIGHT*PTM);

	if (!PLAYER->theEnd)
		WAVE->setScale(scale);

	if(!PLAYER_INFO->idleState)
	{
		WAVE->setOffsetX(offsetX, offsetY);	
	}
	else
	{
		IDLE_WAVE->setOffsetX(offsetX, offsetY);	
	}
}

void GameScene::sharkAnumationKill(CCNode*, void* data){
	}

void GameScene::processTouch()
{
	if(PLAYER_INFO->getIsControlEnabled() == true){
		if (touch) {
			if (PLAYER->body->IsActive()==false) {
				PLAYER->wake();
				touch = false;
			}

				if((PLAYER->body->GetContactList() == NULL) && (PLAYER->body->GetGravityScale() > 0)){
					//CCLOG("1");
					PLAYER->flyDown();
				}
				else if((PLAYER->body->GetContactList() != NULL) && (PLAYER->body->GetGravityScale() > 0)){
					//CCLOG("2");
					PLAYER->speedUp();
				}
		}

		if (touchUp || touchDown) {
			if (!PLAYER->getIsAwake()) {
				//CCLOG("3");
				PLAYER->wake();
				touchDown = false;
				touchUp = false;
			} 
		}
		if(touchUp){
			float impulse = PLAYER->body->GetMass();
			PLAYER->body->ApplyLinearImpulse(b2Vec2(JUMP_IMPULSE_X, impulse), PLAYER->body->GetWorldCenter());
			//CCLOG("4");
		}
		if(touchDown){
			if(PLAYER->currentGadgetAttachment == ATTACHED_TO_DOLPHIN){
				float impulse = PLAYER->body->GetMass()*100;
				PLAYER->body->ApplyLinearImpulse(b2Vec2(JUMP_IMPULSE_X, -impulse), PLAYER->body->GetWorldCenter());
				if(PLAYER->body->GetJointList() != NULL)
					PLAYER->body->GetJointList()->other->SetGravityScale(-3);
			}
			else if (PLAYER_INFO->underwaterControl){
				float impulse = PLAYER->body->GetMass();
				PLAYER->body->ApplyLinearImpulse(b2Vec2(JUMP_IMPULSE_X, -impulse), PLAYER->body->GetWorldCenter());
			}
			//CCLOG("5");
		}

		if (debugTouch && (PLAYER->body->GetContactList() == NULL) && (PLAYER->getIsUnderwater() == false)){
			PLAYER->body->SetLinearVelocity(b2Vec2(PLAYER->body->GetLinearVelocity().x, PLAYER->body->GetLinearVelocity().y+1));
			//CCLOG("6");
		}
	}
}

void GameScene::setupWorld()
{
	b2Vec2 gravity = b2Vec2(GRAVITY);
	bool doSleep = true;
	world = new b2World(gravity);    
	world->SetAllowSleeping(doSleep);
	world->SetContactListener(SurferContactListner::Instance());
}

bool GameScene::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
    CCPoint touchPoint = pTouch->getLocationInView();
    /*
     For debug want show Layer
     */
    isTouchJetPack = true;
    /*int sizeOfFirstPinCode = (int)firstPinCode.size();
    int i;
    for (i=0; i < sizeOfFirstPinCode; i++)
        if (!firstPinCode[i].isEnter){
            if (touchPoint.x > (firstPinCode[i].x - 150) && touchPoint.x < (firstPinCode[i].x + 150) &&
                touchPoint.y > (firstPinCode[i].y - 150) && touchPoint.y < (firstPinCode[i].y + 150)){
                firstPinCode[i].isEnter = true;
                //CCLog("1Good enter %d", i);
            }else{
                for (int j = 0; j < sizeOfFirstPinCode; j++)
                    firstPinCode[j].isEnter = false;
                //CCLog("1FAIL");
            }
            break;
        }
    
    if (isShowDebugLayer && (touchPoint.x > (firstPinCode[i].x - 250) && touchPoint.x < (firstPinCode[i].x + 250) &&
                             touchPoint.y > (firstPinCode[i].y - 250) && touchPoint.y < (firstPinCode[i].y + 250)))
        m_lDebugLayer->debugStop(this);
    
    if (!isShowDebugLayer && i == (sizeOfFirstPinCode - 1)){
        isShowDebugLayer = true;
        showDebugLayer();
		for (int j = 0; j < sizeOfFirstPinCode; j++)
                    secondPinCode[j].isEnter = false;
        //CCLog("1PIN CODE!!");
    }
    
	for (i=0; i < 2; i++)
        if (!secondPinCode[i].isEnter){
            if (touchPoint.x > (secondPinCode[i].x - 150) && touchPoint.x < (secondPinCode[i].x + 150) &&
                touchPoint.y > (secondPinCode[i].y - 150) && touchPoint.y < (secondPinCode[i].y + 150)){
                secondPinCode[i].isEnter = true;
                //CCLog("2Good enter %d", i);
            }else{
                for (int j = 0; j < sizeOfFirstPinCode; j++)
                    secondPinCode[j].isEnter = false;
                //CCLog("2FAIL");
            }
            break;
        }
	if (isShowDebugLayer && i == 1){
        isShowDebugLayer = false;
		hideDebugLayer();
		for (int j = 0; j < sizeOfFirstPinCode; j++)
                    firstPinCode[j].isEnter = false;
        //CCLog("2PIN CODE!!");
    }*/
    //CCLOG("%f %f", touchPoint.x, touchPoint.y);
	if(PLAYER_INFO->idleState)
		return false;
	
	
	if((touchPoint.y > 0) && (PLAYER->currentGadgetAttachment < 1000)){
		touch = true;
		debugTouch = false;
		touchDown = true;

		if(PLAYER->body->GetContactList() == NULL)
			PLAYER_INFO->isTouched = true;

		return true;
	}
	else if(PLAYER->currentGadgetAttachment == ATTACHED_TO_DOLPHIN){
		touchDown = true;
		return true;
	}
	else if(PLAYER->currentGadgetAttachment == ATTACHED_TO_SWIMMER){
		return false;
	}
	else if(PLAYER->currentGadgetAttachment == ATTACHED_TO_KITE){
		return false;
	}
	else if(PLAYER->currentGadgetAttachment == ATTACHED_TO_HELICOTER){
		return false;
	}
	else if(PLAYER->currentGadgetAttachment == ATTACHED_TO_DOLPHIN){
		return false;
	}
	else if(PLAYER->currentGadgetAttachment == ATTACHED_TO_JETSKI){
		return false;
	}
	else if(PLAYER->currentGadgetAttachment == ATTACHED_TO_TORTOISE){
		return false;
	}
	else {
		touch = false;
		debugTouch = false;
		PLAYER_INFO->isTouched = false;
		return false;
	}
}

void GameScene::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	PlayerView::Instance()->changeStatus(PlayerViewStatus_DRIVE);
	if(PLAYER->body->GetGravityScale() > 0){
		PLAYER->body->SetGravityScale(1);
	}
	touchUp = false;
	touchDown = false;
	touch = false;
	debugTouch = false;
	PLAYER_INFO->isTouched = false;
    isTouchJetPack = false;
}

void GameScene::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	touchUp = false;
	touchDown = false;
	touch = false;
	debugTouch = false;
	PLAYER_INFO->isTouched = false;
    isTouchJetPack = false;
}

void GameScene::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}


void GameScene::draw()
{     
	if (!DRAW_BOX2D_LINES)
		return;
 	ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
 	kmGLPushMatrix();
 	
 	ccDrawColor4F(1.0, 0.0, 0.0, 1.0); 
 	CCPoint pnt1 = ccp(PLAYER->rp1.x*PTM - (PLAYER->getPosition().x - CCDirector::sharedDirector()->getWinSize().width/8),PLAYER->rp1.y*PTM);
 	CCPoint pnt2 = ccp(PLAYER->rp2.x*PTM - (PLAYER->getPosition().x - CCDirector::sharedDirector()->getWinSize().width/8), PLAYER->rp2.y*PTM);
 	ccDrawLine(pnt1, pnt2);
 
 	ccDrawColor4F(1.0, 1.0, 0.0, 1.0);
 	ccPointSize(5);
 	ccDrawPoint(ccp(PLAYER->cp.points[0].x*PTM -  (PLAYER->getPosition().x - CCDirector::sharedDirector()->getWinSize().width/8), PLAYER->cp.points[0].y*PTM)); 
 	
 	debugDraw->setOffsetX( PLAYER->getPosition().x,  PLAYER->getPosition().y, WAVE->getScale());
 	kmGLScalef(WAVE->getScaleX(), WAVE->getScaleY(), 1);
 	kmGLTranslatef(PLAYER->getPosition().x, offsetY, 0);
 	world->DrawDebugData();
 	kmGLPopMatrix();
}

void GameScene::backgroundAnimation(CCNode*, void* data)
{
	float time = 5.0f;
	background1->setPosition(ccp(0, 0));
	background2->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width, 0));
	background1->runAction(CCMoveTo::create(time,ccp( 0 - CCDirector::sharedDirector()->getWinSize().width, 0)));	
	background2->runAction(CCSequence::create(
		CCMoveTo::create(time,ccp( 0, 0)),
		CCCallFuncND::create(this, callfuncND_selector(GameScene::backgroundAnimation), NULL),
		NULL));
}

void GameScene::checkGadgetsPos()
{
	//TODO: Optimize this
	//Damn...it's rly slow and ugly...shoulda get moar attention on it
	 //return;
// 	for(std::vector<Object>::iterator it = WAVE->gadgetsObjects.begin(); it != WAVE->gadgetsObjects.end(); ){
// 		if(it->x < PLAYER->getPositionX()){
// 			Gadget* gadget = NULL;
// 			if (strcmp(it->type.c_str(),"helicopter") == 0){
// 				gadget = Helicopter::create(world,  b2Vec2((PLAYER->getPosition().x - CCDirector::sharedDirector()->getWinSize().width/3)/PTM, it->y/PTM));
// 				WAVE->addChild(gadget, 1);
// 				gadgets->addObject(gadget);
// 				WAVE->gadgetsObjects.erase(it);
// 			}
// 			if (strcmp(it->type.c_str(),"dolphin") == 0){
// 				gadget = Dolphin::create(world,  b2Vec2((PLAYER->getPosition().x - CCDirector::sharedDirector()->getWinSize().width/3)/PTM, it->y/PTM));
// 				WAVE->addChild(gadget, 1);
// 				gadgets->addObject(gadget);
// 				WAVE->gadgetsObjects.erase(it);
// 			}
// 		}
// 		//NOTE: if both objects will be erased then the game will crush,
// 		//objects shouldn't be placed at same pos.
// 		else {
// 			++it;
// 		}
// 	}
}

void GameScene::testCount(float dt){
    moneyTest++;
    if (moneyTest > 10)
        moneyTest += 10;
    if (moneyTest > 100)
        moneyTest += 100;
    if (moneyTest > 1000)
        moneyTest += 1000;
    if (moneyTest > 10000)
        moneyTest += 10000;
    if (moneyTest > 100000)
        moneyTest += 100000;
    PlayerInfo::Instance()->rating++;
    if (PlayerInfo::Instance()->rating > 10)
        PlayerInfo::Instance()->rating += 10;
    if (PlayerInfo::Instance()->rating > 100)
        PlayerInfo::Instance()->rating += 100;
    if (PlayerInfo::Instance()->rating > 1000)
        PlayerInfo::Instance()->rating += 1000;
    if (PlayerInfo::Instance()->rating > 10000)
        PlayerInfo::Instance()->rating += 10000;
    if (PlayerInfo::Instance()->rating > 100000)
        PlayerInfo::Instance()->rating += 100000;
    refreshCoinLabel();
    refreshRatingLabel();
}

void GameScene::refreshCoinLabel(){
	std::stringstream ss;
    int colMoney = PlayerInfo::Instance()->moneyInSession;
    ss << colMoney  << " ";
	_moneyShow->setString(ss.str().c_str());
    if (colMoney  < 10){
        dxForRatingPanel = 0.0f;
        panelMoney->setScaleX(0.5f);
        panelMoney->setPositionX(1700.0f);
        _moneyIndikator->setPositionX(1680.0f);
        _moneyShow->setPositionX(1730.0f);
    }
    if (colMoney  >= 10){
        dxForRatingPanel = 30.0f;
        panelMoney->setScaleX(0.6f);
        panelMoney->setPositionX(1690.0f);
        _moneyIndikator->setPositionX(1650.0f);
        _moneyShow->setPositionX(1710.0f);
    }
    if (colMoney  >= 100){
        dxForRatingPanel = 60.0f;
        panelMoney->setScaleX(0.7f);
        panelMoney->setPositionX(1680.0f);
        _moneyIndikator->setPositionX(1640.0f);
        _moneyShow->setPositionX(1700.0f);
    }
    if (colMoney  >= 1000){
        dxForRatingPanel = 90.0f;
        panelMoney->setScaleX(0.8f);
        panelMoney->setPositionX(1670.0f);
        _moneyIndikator->setPositionX(1610.0f);
        _moneyShow->setPositionX(1670.0f);
    }
    if (colMoney  >= 10000){
        dxForRatingPanel = 120.0f;
        panelMoney->setScaleX(0.9f);
        panelMoney->setPositionX(1660.0f);
        _moneyIndikator->setPositionX(1590.0f);
        _moneyShow->setPositionX(1660.0f);
    }
    if (colMoney  >= 100000){
        dxForRatingPanel = 140.0f;
        panelMoney->setScaleX(1.0f);
        panelMoney->setPositionX(1650.0f);
        _moneyIndikator->setPositionX(1560.0f);
        _moneyShow->setPositionX(1630.0f);
    }
    panelMoney->setPositionX(panelMoney->getPositionX()  + 1000*(1-PlayerInfo::Instance()->scaleX));
    _moneyIndikator->setPositionX(_moneyIndikator->getPositionX()  + 1000*(1-PlayerInfo::Instance()->scaleX));
    _moneyShow->setPositionX(_moneyShow->getPositionX()  + 1000*(1-PlayerInfo::Instance()->scaleX));
}

void GameScene::refreshRatingLabel(){
	std::stringstream ss;
	ss << PlayerInfo::Instance()->rating  << " ";
	_ratingCount->setString(ss.str().c_str());
	if (PlayerInfo::Instance()->rating < 10){
        panelRating->setPositionX(1520 - dxForRatingPanel);
        starMult->setPositionX(1500 - dxForRatingPanel);
        _ratingCount->setPositionX(1560 - dxForRatingPanel);
        panelRating->setScaleX(0.5f);
    }
    if (PlayerInfo::Instance()->rating >= 10){
        panelRating->setPositionX(1500 - dxForRatingPanel);
        starMult->setPositionX(1470 - dxForRatingPanel);
        _ratingCount->setPositionX(1530 - dxForRatingPanel);
        panelRating->setScaleX(0.6f);
    }
    if (PlayerInfo::Instance()->rating >= 100){
        panelRating->setPositionX(1480 - dxForRatingPanel);
        starMult->setPositionX(1430 - dxForRatingPanel);
        _ratingCount->setPositionX(1500 - dxForRatingPanel);
        panelRating->setScaleX(0.7f);
    }
    if (PlayerInfo::Instance()->rating >= 1000){
        panelRating->setPositionX(1460 - dxForRatingPanel);
        starMult->setPositionX(1400 - dxForRatingPanel);
        _ratingCount->setPositionX(1470 - dxForRatingPanel);
        panelRating->setScaleX(0.8f);
    }
    if (PlayerInfo::Instance()->rating >= 10000){
        panelRating->setPositionX(1440 - dxForRatingPanel);
        starMult->setPositionX(1370 - dxForRatingPanel);
        _ratingCount->setPositionX(1440 - dxForRatingPanel);
        panelRating->setScaleX(0.9f);
    }
    if (PlayerInfo::Instance()->rating >= 100000){
        panelRating->setPositionX(1420 - dxForRatingPanel);
        starMult->setPositionX(1330 - dxForRatingPanel);
        _ratingCount->setPositionX(1400 - dxForRatingPanel);
        panelRating->setScaleX(1.0f);
    }
    if (PlayerInfo::Instance()->rating >= 1000000){
        panelRating->setPositionX(1400 - dxForRatingPanel);
        starMult->setPositionX(1300 - dxForRatingPanel);
        _ratingCount->setPositionX(1370 - dxForRatingPanel);
        panelRating->setScaleX(1.1f);
    }
    panelRating->setPositionX(panelRating->getPositionX()  + 1000*(1-PlayerInfo::Instance()->scaleX));
    starMult->setPositionX(starMult->getPositionX()  + 1000*(1-PlayerInfo::Instance()->scaleX));
    _ratingCount->setPositionX(_ratingCount->getPositionX()  + 1000*(1-PlayerInfo::Instance()->scaleX));
}

void GameScene::tiktakForStart(float dt)
{
	std::stringstream s;
	s << timerForStart;
	NPC::Instance()->addMessage(s.str());
	timerForStart--;
    if (UserDataLocal::Instance()->_soundVolume > 0.0f)
		SimpleAudioEngine::sharedEngine()->playEffect("./Sound/count_tick.wav");
	if (timerForStart > 0){
		
	}else{
		playInitialization();
		unschedule(schedule_selector(GameScene::tiktakForStart));
	}
}

void GameScene::playInitialization()
{
    //PRINT_MEMORY("1")
	Background::Instance()->unpauseAnimation();
	
	m_mSettingsItem->setVisible(false);
	m_mPauseItem->setVisible(true);

	relocateIsland();

	PLAYER_INFO->idleState = false;
	PLAYER->inMainMenu = false;
	PLAYER->removeFromParent();
	float allDist = IDLE_WAVE->IdleWaveKeyPoints[9].x/PTM;
	float oneDist = 6500*Box2dHelper::scaleYparam/PTM;
	float playerPos = PLAYER->body->GetPosition().x;
	b2Vec2 newPos = b2Vec2(oneDist - ( allDist - playerPos), PLAYER->body->GetPosition().y);
	PLAYER->body->SetTransform(newPos, 0);
	
	PlayerView::Instance()->startAnimation();

    //PRINT_MEMORY("1.25")
	
    //PRINT_MEMORY("1.26")
	sharkAnim=true;
    //PRINT_MEMORY("1.4")
	WAVE->reuseMe();
    //PRINT_MEMORY("1.45")
	addChild(WAVE, 2);
    //PRINT_MEMORY("1.5")
    IDLE_WAVE->clearMe();

	DOLPHIN->initWithWorld(world, b2Vec2(-1000, -1000));
	DOLPHIN->wake();
	WAVE->addChild(DOLPHIN, 60);

	//HELICOPTER->initWithWorld(world, b2Vec2(-1000, -1000));
	//HELICOPTER->wake();
	//WAVE->getObjectLayer()->addChild(HELICOPTER, 1);

    //PRINT_MEMORY("2")
	_moneyIndikator->setVisible(true);
	_moneyShow->setVisible(true);
	_ratingCount->setVisible(true);
	starMult->setVisible(true);
    panelMoney->setVisible(true);
    panelRating->setVisible(true);
	PlayerInfo::Instance()->rating = 0;
	refreshCoinLabel();
	refreshRatingLabel();
	if (Items::Instance()->_powers[0].level > 0)
		addGadgetToGridList(5,0);
    isJetPack = false;
    if (Items::Instance()->_powers[1].level > 0 && !UserDataLocal::Instance()->_tutorial){
        Items::Instance()->_powers[1].level--;
        Items::Instance()->saveItems();
        PLAYER->body->SetLinearVelocity(b2Vec2(PLAYER->body->GetLinearVelocity().x, 10.0f));
        isJetPack = true;
        isTouchJetPack = false;
        PlayerView::Instance()->dontChange = true;
        PlayerView::Instance()->changeStatus(PlayerViewStatus_JETPACK);
        schedule(schedule_selector(GameScene::jetPackUpdate));
        scheduleOnce(schedule_selector(GameScene::jetPackStop), JETPACK_TIME + Items::Instance()->headstartLevel);
        addGadgetToGridList(6, JETPACK_TIME + Items::Instance()->headstartLevel);
		PLAYER_INFO->setIsControlEnabled(true);
    }
	else
	{
		PLAYER_VIEW->changeStatus(PlayerViewStatus_START_PLAY);
	}

    PLAYER_VIEW->dontShowSurfingAnim = false;

    schedule(schedule_selector(GameScene::saveInfoForTime), 5.0f);
	update(0);
    WAVE->sharkSpr->setPositionX(Player::Instance()->getPositionX() - WAVE->sharkDist);
    schedule(schedule_selector(GameScene::animatedSharkAtStart));
    if (UserDataLocal::Instance()->_tutorial)
        m_pTeachLayer->showText();
    //PRINT_MEMORY("3")
}

void GameScene::jetPackUpdate(float dt){
    if (!isJetPack){
        unschedule(schedule_selector(GameScene::jetPackUpdate));
    }
    if (isTouchJetPack)
        return;
    float playerLinerVelocityY = PLAYER->body->GetLinearVelocity().y;
    if (playerLinerVelocityY < JETPACK_UP)
        playerLinerVelocityY = JETPACK_UP;
    if (PLAYER->body->GetPosition().y > JETPACK_HEIGHT){
        playerLinerVelocityY = 0.0f;
        //PLAYER->body->SetTransform(b2Vec2(PLAYER->body->GetPosition().x, JETPACK_HEIGHT), 0.0f);
    }
    
    float playerLinerVelocityX = PLAYER->body->GetLinearVelocity().x;
    if (playerLinerVelocityX < JETPACK_DOWN)
        playerLinerVelocityX = JETPACK_DOWN;
    
    PLAYER->body->SetLinearVelocity(b2Vec2(playerLinerVelocityX, playerLinerVelocityY));
}

void GameScene::jetPackStop(float dt){
    isJetPack = false;
    PlayerView::Instance()->dontChange = false;
    PlayerView::Instance()->changeStatus(PlayerViewStatus_DRIVE);
	PlayerView::Instance()->stopJetPack();
}

void GameScene::addGadgetToGridList(int idG, int time){
	bool isGadgetInGrid = false;
	int cLGG = (int)leftGadgetGridElements.size();
	for (int i = 0; i < cLGG; i++){		
		if (leftGadgetGridElements[i].type == idG){
			leftGadgetGridElements[i].time = time;
			isGadgetInGrid = true;
			switch (idG){
			case 0://Pelican
				showSignBig("pelican");
				break;
			case 1://dolphin
				showSignBig("dolphin");
				break;
			case 2://dMoney
				showSignBig("coin");
                if (leftGadgetGridElements[i].icon->getPositionY() < 0){
					PlayerInfo::Instance()->doubleMoney = true;
                    leftGadgetGridElements[i].time += Items::Instance()->coinLevel;
                    _moneyShow->setColor(ccc3(255,255,0));
                }
				break;
			case 3://dPoint
				showSignBig("star");
				if (leftGadgetGridElements[i].icon->getPositionY() < 0){
					std::stringstream sss;
					_ratingCount->setColor(ccc3(255,255,0));
					PlayerInfo::Instance()->_multiplicator *= 2;
					//sss<<PlayerInfo::Instance()->_multiplicator<<"x";
					//starMultTxt->setString(sss.str().c_str());
                    leftGadgetGridElements[i].time += Items::Instance()->starLevel;
				}
				break;
			case 4://Magnet
				showSignBig("magnet");
				if (leftGadgetGridElements[i].icon->getPositionY() < 0)
					PlayerInfo::Instance()->magnet = true;
                leftGadgetGridElements[i].time += Items::Instance()->magnetLevel;
				break;
			case 5://Heart
				
				break;
            case 6://Head start
                break;
            case 7://Scooter
                showSignBig("scooter");
                isOnScooter = true;
                SHARK->changeStatus(false);
                PlayerView::Instance()->changeStatus(PlayerViewStatus_SCOOTER);
                PLAYER_INFO->underwaterControl = true;
                break;
			}
		}
	}


	if (!isGadgetInGrid){
		LeftGadgetGridtStr tmpLGGS;
		tmpLGGS.type = idG;
		tmpLGGS.time = time;
		tmpLGGS.on = true;
		std::stringstream sss;
		switch (idG){
		case 0://Pelican
			tmpLGGS.icon = CCSprite::create("game/shop/icons/pelican.png");		
			showSignBig("pelican");
			break;
		case 1://dolphin
			tmpLGGS.icon = CCSprite::create("game/shop/icons/dolphin.png");		
			showSignBig("dolphin");
			break;
		case 2://dMoney
			PlayerInfo::Instance()->doubleMoney = true;
			tmpLGGS.icon = CCSprite::create("game/shop/icons/coin.png");
			showSignBig("coin");
            tmpLGGS.time += Items::Instance()->coinLevel;
            _moneyShow->setColor(ccc3(255,255,0));
			break;
		case 3://dPoint
			PlayerInfo::Instance()->doublePoints = true;
			tmpLGGS.icon = CCSprite::create("game/shop/icons/star.png");
			showSignBig("star");
			_ratingCount->setColor(ccc3(255,255,0));
			PlayerInfo::Instance()->_multiplicator *= 2;
			//sss<<PlayerInfo::Instance()->_multiplicator<<"x";
			//starMultTxt->setString(sss.str().c_str());
            tmpLGGS.time += Items::Instance()->starLevel;
			break;
		case 4://Magnet
			PlayerInfo::Instance()->magnet = true;
			tmpLGGS.icon = CCSprite::create("game/shop/icons/magnet.png");
			showSignBig("magnet");
            tmpLGGS.time += Items::Instance()->magnetLevel;
			break;
		case 5://Heart
			tmpLGGS.icon = CCSprite::create("game/shop/icons/wings1.png");
			break;
        case 6://Head start
            tmpLGGS.icon = CCSprite::create("game/shop/icons/arrow2.png");
            break;
        case 7://Scooter
            showSignBig("scooter");
            isOnScooter = true;
            SHARK->changeStatus(false);
            tmpLGGS.icon = CCSprite::create("game/shop/icons/scooter.png");
            PlayerView::Instance()->changeStatus(PlayerViewStatus_SCOOTER);
            PLAYER_INFO->underwaterControl = true;
            break;
		}
		tmpLGGS.timer = CCLabelBMFont::create("0","./Fonts/MisterEarlBT.fnt");
		tmpLGGS.timer->setAnchorPoint(ccp(1.0f, 1.0f));
		tmpLGGS.timer->setPosition(ccp(tmpLGGS.icon->getContentSize().width*0.9f, tmpLGGS.icon->getContentSize().height*0.97f));
 		tmpLGGS.timer->setScale(CCDirector::sharedDirector()->getContentScaleFactor()*1.3f);
		tmpLGGS.icon->addChild(tmpLGGS.timer);
		tmpLGGS.icon->setVisible(false);
		tmpLGGS.icon->setScale(0.5f);
		tmpLGGS.icon->setAnchorPoint(CCPointZero);
		tmpLGGS.icon->setPositionX(10 - 1300*(1-PlayerInfo::Instance()->scaleX));
		leftGadgetGrid->addChild(tmpLGGS.icon);
		leftGadgetGridElements.push_back(tmpLGGS);
	}


	cLGG = (int)leftGadgetGridElements.size();
	int dx = 15;
	std::stringstream ss;
	for (int i = 0; i < cLGG; i++){		
		if (leftGadgetGridElements[i].time > 1 || (leftGadgetGridElements[i].type == 5 && leftGadgetGridElements[i].time > -1)){			
			leftGadgetGridElements[i].icon->setPositionY(dx);
			dx += (leftGadgetGridElements[i].icon->getContentSize().height/2 + 10);
			leftGadgetGridElements[i].icon->setVisible(true);
			ss.str("");
			ss<<leftGadgetGridElements[i].time;
			if (leftGadgetGridElements[i].type == 5)
				ss.str("");
			leftGadgetGridElements[i].timer->setString(ss.str().c_str());
		}
	}


	if (!isleftGadgetShow){
		schedule(schedule_selector(GameScene::refreshLeftGadgetGrid),1.0f);	
	}
}

void GameScene::refreshLeftGadgetGrid(float dt){
	isleftGadgetShow = true;
	int cLGG = (int)leftGadgetGridElements.size();
	int dx = 15;
	std::stringstream ss;
	bool isShow = false;
    
    //CCLOG("refreshGadgetList");
	for (int i = 0; i < cLGG; i++){	
		if (leftGadgetGridElements[i].time > 1 || (leftGadgetGridElements[i].type == 5 && leftGadgetGridElements[i].time > -1)){			
			leftGadgetGridElements[i].icon->setPositionY(dx);
			dx += (leftGadgetGridElements[i].icon->getContentSize().height/2 + 10);
			leftGadgetGridElements[i].icon->setVisible(true);
			if (leftGadgetGridElements[i].type != 5)
                leftGadgetGridElements[i].time--;
            if (leftGadgetGridElements[i].time < 4 && leftGadgetGridElements[i].type != 5){
                leftGadgetGridElements[i].icon->runAction(CCSequence::create(
                                                                             CCFadeTo::create(0.2f, 0),
                                                                             CCDelayTime::create(0.2f),
                                                                             CCFadeTo::create(0.2f, 255),
                                                                             NULL
                                                          ));
                leftGadgetGridElements[i].timer->runAction(CCSequence::create(
                                                                              CCFadeTo::create(0.2f, 0),
                                                                              CCDelayTime::create(0.2f),
                                                                              CCFadeTo::create(0.2f, 255),
                                                                              NULL
                                                                              ));
            }
			ss.str("");
			ss<<leftGadgetGridElements[i].time;
			if (leftGadgetGridElements[i].type == 5)
				ss.str("");
			leftGadgetGridElements[i].timer->setString(ss.str().c_str());
			isShow = true;
		}else{
			if (leftGadgetGridElements[i].icon->getPositionY() > 0){
				leftGadgetGridElements[i].icon->setPositionY(-1000);
				std::stringstream sss;
				switch(leftGadgetGridElements[i].type){
				case 2:
					PlayerInfo::Instance()->doubleMoney = false;
                    _moneyShow->setColor(ccc3(255,255,255));
					break;
				case 3:
					PlayerInfo::Instance()->doublePoints = false;
					PlayerInfo::Instance()->_multiplicator = PlayerInfo::Instance()->_multiplicator / 2;
					_ratingCount->setColor(ccc3(255,255,255));
					//sss<<PlayerInfo::Instance()->_multiplicator<<"x";
					//starMultTxt->setString(sss.str().c_str());
					break;
				case 4:
					PlayerInfo::Instance()->magnet = false;
					break;
                case 7:
                    //CCLOG("isOnScooter FALSE");
                    isOnScooter = false;
                    break;
				}
			}
		}
	}
	if (!isShow){
		isleftGadgetShow = false;
		unschedule(schedule_selector(GameScene::refreshLeftGadgetGrid));
	}
}

void GameScene::deleteElementFromLeftGadgetGrif(int idG){
	int cLGG = (int)leftGadgetGridElements.size();
	for (int i = 0; i < cLGG; i++){		
		if ((leftGadgetGridElements[i].time > 0 && leftGadgetGridElements[i].type == idG)||(leftGadgetGridElements[i].type == idG && idG == 5)){
			leftGadgetGridElements[i].time = -1;
			leftGadgetGridElements[i].icon->setPositionY(-1000);
		}
	}

}

bool GameScene::hasElementInLeftGadgetGrif(int idG)
{
	int cLGG = (int)leftGadgetGridElements.size();
	for (int i = 0; i < cLGG; i++)
	{		
		if(leftGadgetGridElements[i].type == idG && leftGadgetGridElements[i].icon->getPositionY() > 0)
		{
			return true;
		}
	}
	return false;
}

void GameScene::clearLeftGadgetGridt(){
	int cLGG = (int)leftGadgetGridElements.size();
	for (int i = 0; i < cLGG; i++){
		if (leftGadgetGridElements[i].icon->getPositionY()>0){
			leftGadgetGridElements[i].icon->setPositionY(-1000);
			std::stringstream sss;
			switch(leftGadgetGridElements[i].type){
			case 2:
				PlayerInfo::Instance()->doubleMoney = false;
                _moneyShow->setColor(ccc3(255,255,255));
				break;
			case 3:
				PlayerInfo::Instance()->doublePoints = false;
				PlayerInfo::Instance()->_multiplicator = PlayerInfo::Instance()->_multiplicator / 2;
				_ratingCount->setColor(ccc3(255,255,255));
				//sss<<PlayerInfo::Instance()->_multiplicator<<"x";
				//starMultTxt->setString(sss.str().c_str());
				break;
			case 4:
				PlayerInfo::Instance()->magnet = false;
				break;
            case 7:
                isOnScooter = false;
                break;
			}
		}
	}
	leftGadgetGridElements.clear();
	isleftGadgetShow = false;
}

void GameScene::showSignBig(const std::string &name)
{	
	removeChild(signBig, true);
	signBig = CCSprite::create(("game/shop/icons/" + name + ".png").c_str());
	signBig->setPosition(ccp(1024,768));
	signBig->setScale(1.0f);
	signBig->setOpacity(255);
	addChild(signBig, 100);
	signBig->runAction(CCFadeTo::create(1.0f,0));
	signBig->runAction(CCScaleTo::create(1.0f,3.0f));
}

void GameScene::showDebugLayer()
{
    m_lDebugLayer = DebugLayer::create();
	addChild(m_lDebugLayer, 1300);
}

void GameScene::hideDebugLayer()
{
	removeChild(m_lDebugLayer);
	m_lDebugLayer = NULL;
}

void GameScene::saveInfoForTime(float dt){
	UserDataLocal::Instance()->save();
}

void GameScene::animatedSharkAtStart(float dt){
    if (!sharkAnim){
        unschedule(schedule_selector(GameScene::animatedSharkAtStart));
        return;
    }
    if (WAVE->sharkAnimIn){
        WAVE->sharkDist -= 8.0f;
        if (WAVE->sharkDist < 500){
            WAVE->sharkAnimIn = false;
        }
    }else{
        WAVE->sharkDist += 6.0f;
        if (WAVE->sharkDist > 2048){
            sharkAnim = false;
            WAVE->sharkSpr->setVisible(false);
            unschedule(schedule_selector(GameScene::animatedSharkAtStart));
        }
    }
    
    WAVE->sharkSpr->setPositionX(Player::Instance()->getPositionX() - WAVE->sharkDist);
    //	if (sharkAnim){
    //		if (WAVE->sharkAnimIn){
    //			WAVE->sharkDist -= 8.0f;
    //			if (WAVE->sharkDist < WAVE->shark->getContentSize().width){
    //				WAVE->sharkAnimIn = false;
    //			}
    //		}else{
    //			WAVE->sharkDist += 6.0f;
    //			if (WAVE->sharkDist > WAVE->shark->getContentSize().width*2.0f){
    //				sharkAnim = false;
    //				WAVE->shark->setVisible(false);
    //			}
    //		}
    //        //if (isJetPack)
    //        //    WAVE->sharkDist = WAVE->sharkDist * 2;
    //		WAVE->shark->setPositionX(Player::Instance()->getPositionX() - WAVE->sharkDist);// - distanceToP);
    //	}
    
}

void GameScene::playClick( CCObject* obj )
{
	if(m_pTouchCounter->getCounter() > 0 || getChildByTag(WINDOW_TAG))
	{
		return;
	}

	PRINT_MEMORY("1")
	
	timerForStart = 3;
	tiktakForStart(0.0f);
	schedule(schedule_selector(GameScene::tiktakForStart),1.0f);
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(false);
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("./Sound/sound_in_game.mp3", true);
	if (UserDataLocal::Instance()->_soundVolume > 0.0f)
	{
		SimpleAudioEngine::sharedEngine()->playEffect("./Sound/play_button.wav");
	}
	IDLE_WAVE->enableAllBtn(false);
	IDLE_WAVE->girl->stopAllActions();
	CCCallFunc *nextAction = CCCallFunc::create(this, callfunc_selector(GameScene::refreshCoinLabel));
	Utils::createAndRunAnimationFromFrameThenNextAction(IDLE_WAVE->girl, "girl", "game/menuAnimation", 0.18f, 11, 16, nextAction);

	m_mSettingsItem->setEnabled(false);
	m_mPlayItem->setVisible(false);
	clearLeftGadgetGridt();
	signBig->setVisible(true);
	leftGadgetGrid->setVisible(true);
	_sharkSign->setVisible(false);
	PlayerInfo::Instance()->magnet = false;
	PlayerInfo::Instance()->doubleMoney = false;
	PlayerInfo::Instance()->doublePoints = false;
	PlayerInfo::Instance()->distance = 0;
	PlayerInfo::Instance()->questDiveXTimes = 0;
	PlayerInfo::Instance()->questOpenXChests = 0;
	PlayerInfo::Instance()->questDolphin = 0;
	PlayerInfo::Instance()->coinInSession = 0;
	PlayerInfo::Instance()->goodJumpInSession = 0;
	PlayerInfo::Instance()->grottoInSession = 0;
	PlayerInfo::Instance()->moneyInSession = 0;
	PlayerInfo::Instance()->rating = 0;
	PlayerInfo::Instance()->showRecordMoney = false;
	PlayerInfo::Instance()->showRecordPoint = false;
	PlayerInfo::Instance()->showRecordRank = false;
	UserDataLocal::Instance()->_totalGames++;
	UserDataLocal::Instance()->save();
	RateMe::Instance()->resetSession();
	
	PRINT_MEMORY("2")
}

void GameScene::restartClick( CCObject *obj )
{
	unpauseGame();
	resetGame();
	playClick(NULL);
}

void GameScene::homeClick( CCObject *obj )
{
    CCLOG("In 1");
	unpauseGame();
    CCLOG("In 2");
	resetGame();
    CCLOG("In 3");
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("./Sound/Surf_Menu_Reggae2.mp3", true);
}

void GameScene::storeClick(CCObject* obj)
{
	addChild(ShopDialog::create(), 50);
}

void GameScene::missionsClick(CCObject* obj)
{	
	addChild(MissionsDialog::create(), 50);
}

void GameScene::profileClick(CCObject* obj)
{
	addChild(ProfileDialog::create(), 50);
}

void GameScene::pauseClick( CCObject *obj )
{
	addChild(PauseDialog::create(), 50);
	pauseGame();

	_moneyIndikator->setVisible(false);
	_moneyShow->setVisible(false);
	_ratingCount->setVisible(false);
	starMult->setVisible(false);
    _sharkSign->setOpacity(0);
    signBig->setVisible(false);
	panelMoney->setVisible(false);
	panelRating->setVisible(false);
	leftGadgetGrid->setVisible(false);
	m_mPauseItem->setVisible(false);
}

void GameScene::resumeClick( CCObject *obj )
{
	unpauseGame();

	_moneyIndikator->setVisible(true);
	_moneyShow->setVisible(true);
    _ratingCount->setVisible(true);
	starMult->setVisible(true);
	_sharkSign->setOpacity(255);
	signBig->setVisible(true);
	panelMoney->setVisible(true);
    panelRating->setVisible(true);
	leftGadgetGrid->setVisible(true);
	m_mPauseItem->setVisible(true);
}

void GameScene::settingsClick( CCObject *obj )
{
	addChild(SettingsDialog::create(), 50);
}

void GameScene::gCClick(CCObject* obj)
{
	GameCenter::current()->setDelegate(this);
	GameCenter::current()->showLeaderboard();
}

void GameScene::creditsClick( CCObject *obj )
{
	addChild(CreditsDialog::create(), 50);
}

void GameScene::bonusClick( CCObject *obj )
{
	if(IDLE_WAVE->openChest())
	{
        if (IS_FREE){
            addChild(BuyPaidVersionDialog::create(), 50);
        }else{
            addChild(BonusDialog::create(), 50);
        }
	}
}

void GameScene::moreGamesClick( CCObject *obj )
{
	if(Utils::isIOS())
	{
		s3eOSExecExecute("http://appstore.com/Nekki", false);
	}
	else
	{
		s3eOSExecExecute("http://google.ru", false);
	}
}

void GameScene::backClicked( CCObject *obj )
{
	if(getChildByTag(WINDOW_TAG))
	{
		return;
	}

	if(m_mPauseItem->isVisible())
	{
		pauseClick(obj);
	}
	else if(m_mSettingsItem->isEnabled())
	{
		addChild(ExitDialog::create(), 50);
	}
}

void GameScene::pauseGame()
{
	if (UserDataLocal::Instance()->_soundVolume > 0.0f)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseAllEffects();
        CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	}
    m_pTeachLayer->pause();
	pauseNode(this);
}

void GameScene::unpauseGame()
{
	if (UserDataLocal::Instance()->_soundVolume > 0.0f)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeAllEffects();
        CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	}
    m_pTeachLayer->pause();
	unpauseNode(this);
}

void GameScene::resetGame()
{
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    unschedule(schedule_selector(GameScene::saveInfoForTime));
    CCLOG("IIIIIIIII");
    m_pTeachLayer->stop();
    CCLOG("IIIIIIIII - 2");
	if (isJetPack)
	{
        isJetPack = false;
        unschedule(schedule_selector(GameScene::jetPackUpdate));
        unschedule(schedule_selector(GameScene::jetPackStop));
        PlayerView::Instance()->dontChange = false;
        PlayerView::Instance()->changeStatus(PlayerViewStatus_DRIVE);
		PlayerView::Instance()->stopJetPack();
        deleteElementFromLeftGadgetGrif(6);
    }
	CCLOG("IIIIIIIII - 3");
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(UserDataLocal::Instance()->_musicVolume);
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(false);
	CCLOG("IIIIIIIII - 4");
	sharkAnim = false;

	m_mPauseItem->setVisible(false);
	m_mSettingsItem->setVisible(true);
	m_mSettingsItem->setEnabled(true);
	m_mPlayItem->setVisible(true);
    CCLOG("IIIIIIIII - 5");
	PLAYER->setVisible(true);

	_sharkSign->setVisible(false);
	_sharkSign->setOpacity(255);

	_moneyIndikator->setVisible(false);
	_moneyShow->setVisible(false);
	_ratingCount->setVisible(false);
	starMult->setVisible(false);
	panelRating->setVisible(false);
	panelMoney->setVisible(false);
    CCLOG("IIIIIIIII - 6");
	clearLeftGadgetGridt();
	Quest::Instance()->updateQuest(QuestSurfXMTotal, Player::Instance()->body->GetPosition().x);
	if (PlayerInfo::Instance()->rating > UserDataLocal::Instance()->_bestScore)
		UserDataLocal::Instance()->_bestScore = PlayerInfo::Instance()->rating;
	if (UserDataLocal::Instance()->_bestSerf < PLAYER->body->GetPosition().x)
		UserDataLocal::Instance()->_bestSerf = PLAYER->body->GetPosition().x;
	if (PlayerInfo::Instance()->coinInSession > UserDataLocal::Instance()->_bestCoin)
		UserDataLocal::Instance()->_bestCoin = PlayerInfo::Instance()->coinInSession;
	UserDataLocal::Instance()->_totalPoints += PlayerInfo::Instance()->rating;
	UserDataLocal::Instance()->_totalDistance += PLAYER->body->GetPosition().x;
	UserDataLocal::Instance()->save();
    CCLOG("IIIIIIIII - 7");

	Quest::Instance()->saveQuests();
	PLAYER_INFO->setIsControlEnabled(false);
	PLAYER_INFO->idleState = true;
	CCLOG("IIIIIIIII - 8");
	WAVE->clearMe();

    PRINT_MEMORY("--")

	world->DestroyBody(DOLPHIN->body);
	//world->DestroyBody(HELICOPTER->body);
	DOLPHIN->removeFromParentAndCleanup(true);
	//HELICOPTER->removeFromParentAndCleanup(true);

	PLAYER->removeFromParentAndCleanup(true);
	PLAYER->restart();

	IDLE_WAVE->init(world, false);
	IDLE_WAVE->resetIdleWavesVertices();
	IDLE_WAVE->addChild(PLAYER, 2);
	IDLE_WAVE->genIdleIdleWaves = true;
	addChild(IDLE_WAVE, 2);

	PLAYER_VIEW->startAnimation();

	Background::Instance()->changeLevel(1, true);

	PRINT_MEMORY("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
}

void GameScene::endGame()
{
	addChild(TheEndDialog::create(), 50);

	_moneyIndikator->setVisible(false);
	_moneyShow->setVisible(false);
	_ratingCount->setVisible(false);
	starMult->setVisible(false);
	panelMoney->setVisible(false);
	panelRating->setVisible(false);
	_sharkSign->setOpacity(0);
	signBig->setVisible(false);
	leftGadgetGrid->setVisible(false);
	m_mPauseItem->setVisible(false);

	SimpleAudioEngine::sharedEngine()->fadeOutMusic(0.01f);
	if (UserDataLocal::Instance()->_soundVolume > 0.0f) 
	{
		SimpleAudioEngine::sharedEngine()->playEffect("./Sound/game_over.wav");
	}
	Quest::Instance()->checkQuest(QuestDieXTimes, 0);
	UserDataLocal::Instance()->save();
}

void GameScene::pauseNode( CCNode *node )
{
	node->pauseSchedulerAndActions();

	CCObject *child;
	CCARRAY_FOREACH (node->getChildren(), child)
	{
		if(((CCNode*)child)->getTag() != WINDOW_TAG)
		{
			pauseNode((CCNode*)child);
		}
	}
}

void GameScene::unpauseNode( CCNode *node )
{
	node->resumeSchedulerAndActions();

	CCObject *child;
	CCARRAY_FOREACH (node->getChildren(), child)
	{
		if(((CCNode*)child)->getTag() != WINDOW_TAG)
		{
			unpauseNode((CCNode*)child);
		}
	}
}

void GameScene::relocateIsland()
{
	CCNode *island = IDLE_WAVE->getChildByTag(ISLAND_TAG);
	island->retain();
	IDLE_WAVE->removeChildByTag(ISLAND_TAG, false);
	WAVE->addChild(island, 2, ISLAND_TAG);
	island->release();
	float allDistNonPTM = IDLE_WAVE->IdleWaveKeyPoints[9].x;
	float oneDistNonPTM = 6500*Box2dHelper::scaleYparam;
	island->setPositionX(oneDistNonPTM - ( allDistNonPTM - island->getPositionX()));
	island->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(5.0f), CCHide::create()));

	CCNode *menu = IDLE_WAVE->getChildByTag(MAIN_MENU_TAG);
	menu->setPositionX(oneDistNonPTM - ( allDistNonPTM - menu->getPositionX()));
	menu->retain();
	IDLE_WAVE->removeChildByTag(MAIN_MENU_TAG, false);
	WAVE->addChild(menu, 11, MAIN_MENU_TAG);
	menu->release();
	menu->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(5.0f), CCHide::create()));
}

void GameScene::onEnter()
{
	CCLayer::onEnter();
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GameScene::backClicked), "back_clicked", NULL);
}

void GameScene::onExit()
{
	CCLayer::onExit();
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "back_clicked");
}
