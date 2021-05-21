#include "GameOverScene.h"
USING_NS_CC;

Scene* GameOver::createScene()
{
    return GameOver::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in NinjiaAttackScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameOver::init()
{
    if (!Scene::init()) 
    { 
        return false; 
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    auto sprite = Sprite::create("GameOver.png");
    sprite->setPosition(Vec2(visibleSize / 2) + origin);
    this->addChild(sprite);

    return true;
}

void GameOver::menuCloseCallback(Ref* sender)
{
    Director::getInstance()->end();
    //Director::getInstance()->pushScene(TransitionSplitCols::create(2.0f, GameOver::createScene()));
}