#ifndef LOADING_SCENE_H
#define LOADING_SCENE_H

#include "cocos2d.h"
using namespace cocos2d;

class LoadingScene : public CCLayer 
{
public:
	LoadingScene();
	~LoadingScene();

	virtual bool init();  

	float scale_x; 
	float scale_y; 

	CREATE_FUNC(LoadingScene);

private:
	void load(float dt);
	void show_intro();
	void show_next(CCTime t);
	void intro_end();

	virtual void menu_skip_callback(CCObject* sender);
};

#endif //!LOADING_SCENE_H