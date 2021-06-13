/**
* @file EndMenuScene.h
*/

#ifndef __END_MENU_SCENE_H__
#define __END_MENU_SCENE_H__

#include "cocos2d.h"

class EndMenuScene : public cocos2d::Scene
{
public:

	CREATE_FUNC(EndMenuScene);

	cocos2d::Scene* createScene();

	virtual bool init() override;

	void startCallback(cocos2d::Ref* pSender);

	void exitCallback(cocos2d::Ref* pSender);

	void bindFirstFightScene(cocos2d::Scene* scene);

	void initRestartButton();

	void initExitButton();

protected:

	cocos2d::Scene* firstFightScene_;

	cocos2d::Menu* startButton_;

	cocos2d::Menu* exitButton_;

};

#endif // !__END_MENU_SCENE_H__
