/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "GameOverScene.h"
#include "Player.h"
#include "Bullet.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }
    if (!Scene::initWithPhysics())
    {
        return false;
    }

    auto winSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    //set background with grey color
    /*auto background = DrawNode::create();
    background->drawSolidRect(origin, winSize, cocos2d::Color4F(0.6, 0.6, 0.6, 1));
    this->addChild(background);*/

    player_ = Player::create("player.png");
    this->addChild(player_);

    //get tmx pic from files  从文件中搞到tmx地图文件
    _tileMap = TMXTiledMap::create("myfirst.tmx");
    _tileMap->setPosition(Vec2(0,0));
    this->addChild(_tileMap, -1);

    srand((unsigned int)time(nullptr));
    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::addMonster), 1.5);


    auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, player_);

    auto contactListener = cocos2d::EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegan, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    auto keyboardListener = cocos2d::EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(Player::listenToKeyPress, player_);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(Player::listenToKeyRelease, player_);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    return true;
}

void HelloWorld::addMonster(float dt)
{
    auto monster = Sprite::create("monster.png");
    if (monster == nullptr)
    {
        problemLoading("monster.png");
    }
    else {
        auto minY = monster->getContentSize().height / 2;
        auto maxY = this->getContentSize().height - minY;
        auto rangeY = maxY - minY;
        int randomY = (rand() % static_cast<int>(rangeY)) + minY;
        int maxX = this->getContentSize().width;
        int randomX = maxX - (rand() % (int)(maxX / 2));
        float monsterSpeed = 160;
        float randomDuration2 = (float)randomX / monsterSpeed;
        float randomDuration1 = (float)(maxX - randomX) / monsterSpeed;
        monster->setPosition(this->getContentSize().width + monster->getContentSize().width / 2, randomY);
        auto move1 = MoveTo::create(randomDuration1, Vec2(randomX, randomY));
        auto move2 = MoveTo::create(randomDuration2, Vec2(-monster->getContentSize().width / 2, randomY));

        auto physicsBody = PhysicsBody::createBox(monster->getContentSize(), cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
        physicsBody->setDynamic(false);
        physicsBody->setContactTestBitmask(1);
        physicsBody->setCategoryBitmask(3);
        monster->setPhysicsBody(physicsBody);
        monster->setTag(ENEMY);
        this->addChild(monster);

        int minDuration = 2, maxDuration = 4, rangeDuration = maxDuration - minDuration;
        int randomDuration = (rand() % rangeDuration) + minDuration;
        auto actionMove = MoveTo::create(randomDuration, Vec2(-monster->getContentSize().width / 2, randomY));
        auto actionRemove = RemoveSelf::create();

        auto shootStar = CallFunc::create([=]() {
            Sprite* eDart = Sprite::create("dart_enemy.png");
            if (eDart == nullptr)
            {
                problemLoading("dart_enemy.png");
            }
            else
            {
                eDart->setPosition(monster->getPosition());
                auto physicsBody = PhysicsBody::createBox(eDart->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
                physicsBody->setDynamic(false);
                physicsBody->setCategoryBitmask(3);
                physicsBody->setContactTestBitmask(4);
                eDart->setPhysicsBody(physicsBody);
                eDart->setTag(ENEMY_BULLET);
                this->addChild(eDart);

                float starSpeed = 120;
                float maxX = this->getContentSize().width;
                float starDuration = (float)randomX / starSpeed;
                auto eDartMove = MoveTo::create(starDuration, player_->getPosition());
                auto eDartRemove = RemoveSelf::create();
                eDart->runAction(Sequence::create(eDartMove, eDartRemove, nullptr));
            }
            });

        auto delay = cocos2d::DelayTime::create(0.05);
        monster->runAction(Sequence::create(move1, delay, shootStar, move2, actionRemove, nullptr));
    }
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* unusedEvent)
{
    auto touchLocation = touch->getLocation();
    auto offset = touchLocation - player_->getPosition();
    offset.normalize();
    auto realDest = offset * 1000 + touchLocation;

    auto bullet = Bullet::create(player_->getBulletName());
    bullet->setPosition(player_->getPosition());
    this->addChild(bullet, 1);
    bullet->shoot(offset);

    return true;
}

bool HelloWorld::onContactBegan(cocos2d::PhysicsContact& physicsContact)
{
    auto nodeA = physicsContact.getShapeA()->getBody()->getNode();
    auto nodeB = physicsContact.getShapeB()->getBody()->getNode();

    if (nodeA && nodeB)
    {
        auto tagA = nodeA->getTag();
        auto tagB = nodeB->getTag();

        //player kills enemy or enemy's bullet
        if (tagA == ME_BULLET)
        {
            nodeB->removeFromParentAndCleanup(true);
        }
        else if (tagB == ME_BULLET)
        {
            nodeA->removeFromParentAndCleanup(true);
        }

        //enemy kills player
        if (tagA == ME || tagB == ME)
        {
            Director::getInstance()->replaceScene(TransitionSlideInT::create(0.2f, GameOver::create()));
        }
    }

    return true;
}