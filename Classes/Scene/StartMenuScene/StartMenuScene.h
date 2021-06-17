/**
* @file StartMenuScene.h
*/

#ifndef __START_MENU_SCENE_H__
#define __START_MENU_SCENE_H__

#include "cocos2d.h"
#include "Scene/FightScene/FightScene.h"

class FightScene;

class StartMenuScene : public cocos2d::Scene
{
public:

	CREATE_FUNC(StartMenuScene);

	cocos2d::Scene* createScene();

	virtual bool init() override;

	void startCallback(cocos2d::Ref* pSender);

	void exitCallback(cocos2d::Ref* pSender);

	void goToFightScene();

	void initStartButton();

	void initExitButton();

	void loadingFightScene();

protected:

	cocos2d::Menu* startButton_;

	cocos2d::Menu* exitButton_;

	FightScene* fightScene_;

};

#endif // !__START_MENU_SCENE_H__
