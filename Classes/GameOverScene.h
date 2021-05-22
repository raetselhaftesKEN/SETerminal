#ifndef GAMEOVER_SCENE_H__
#define GAMEOVER_SCENE_H__

#include "cocos2d.h"

class GameOver : public cocos2d::Scene
{
public:
    virtual bool init() override;

    static cocos2d::Scene* createScene();

    // a selector callback
    void menuCloseCallback(Ref* sender);

    // implement the "static create()" method manually
    CREATE_FUNC(GameOver);

};

#endif