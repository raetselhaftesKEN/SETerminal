/**
* @file HelloWorldScene.cpp
*/

#include "HelloWorldScene.h"
#include "GameOverScene.h"
#include "./Item/Bullet/Bullet.h"
#include "Item/Medkit/Medkit.h"
#include "Const/Const.h"
#include "../Item/Obstacle/Obstacle.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool HelloWorld::init()
{
    //调用父类初始化
    if (!Scene::init())
    {
        return false;
    }
    if (!Scene::initWithPhysics())
    {
        return false;
    }

    //获取窗口大小和原点坐标
    auto winSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    //绘制灰色背景
    auto background = DrawNode::create();
    background->drawSolidRect(origin, winSize, cocos2d::Color4F(0.6, 0.6, 0.6, 1));
    //将其改为-2层次，防止盖住下边的tmx地图文件
    this->addChild(background, -2);

    //get tmx pic from files  从文件中搞到tmx地图文件
    _tileMap = TMXTiledMap::create("myfirst.tmx");
    _tileMap->setPosition(Vec2(0, 0));
    this->addChild(_tileMap, -1);

    //生成玩家角色实例
    player_ = Player::create("MIKU/idle_down/idle_down1.png");
    this->addChild(player_, 2);

    healthBar_ = HealthBar::create(player_);
    healthBar_->setAnchorPoint(cocos2d::Point(0.f, 1.f));
    healthBar_->setPosition(cocos2d::Point(10, winSize.height));
    addChild(healthBar_, 2);

    auto medkit = Medkit::create();
    addChild(medkit, 2);
    medkit->setPosition(500, 500);

    auto obstacle = Obstacle::create("wall.png");
    obstacle->setPosition(300, 100);
    obstacle->setTag(133);
    addChild(obstacle, 0);

    //调用addMonster方法在随机位置生成怪物
    srand((unsigned int)time(nullptr));
    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::addMonster), 1.5);

    //生成屏幕触摸（即鼠标单击）事件的监听器
   auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, player_);

    //电脑专用的鼠标监听器
    auto mouseListener = cocos2d::EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(HelloWorld::onMouseMove, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, player_);

    //生成场景内物理碰撞事件的监听器
    auto contactListener = cocos2d::EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegan, this);
    contactListener->onContactSeparate = CC_CALLBACK_1(HelloWorld::onContactSeparated, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    //生成键盘操作事件的监听器
    auto keyboardListener = cocos2d::EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(Player::listenToKeyPress, player_);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(Player::listenToKeyRelease, player_);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);


    return true;
}


void HelloWorld::addMonster(float dt)
{
    //生成怪物实例
    auto monster = Monster::create("MONSTER2/idle_down/idle_down1.png");
    if (monster == nullptr)
    {
        problemLoading("monster.png");
    }
    else 
    {
        addChild(monster);
        monster->move();
    }
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* unusedEvent)
{
    //取得点击屏幕位置的坐标
    auto touchLocation = touch->getLocation();
    auto offset = touchLocation - player_->getPosition();
    //取得子弹发射方向的单位向量
    offset.normalize();
    //在场景内创建子弹实例
    auto bullet = Bullet::create(player_->getBulletName());
    bullet->setPosition(player_->getPosition());
    this->addChild(bullet, 1);
    //为子弹实例绑定播放发射的飞行动画
    bullet->shoot(offset);

    return true;
}

void HelloWorld::onMouseMove(cocos2d::EventMouse* mouse)
{   
    player_->listenToMouseEvent(convertToNodeSpace(mouse->getLocationInView()), false);
}

bool HelloWorld::onContactBegan(cocos2d::PhysicsContact& physicsContact)
{
    auto nodeA = physicsContact.getShapeA()->getBody()->getNode();
    auto nodeB = physicsContact.getShapeB()->getBody()->getNode();

    if (nodeA && nodeB)
    {
        if (nodeA->getTag() == PLAYER_TAG && nodeB->getTag() == ITEM_TAG)
        {
            contactBetweenPlayerAndItem(dynamic_cast<Player*>(nodeA), dynamic_cast<Item*>(nodeB));
        }
        else if (nodeA->getTag() == ITEM_TAG && nodeB->getTag() == PLAYER_TAG)
        {
            contactBetweenPlayerAndItem(dynamic_cast<Player*>(nodeB), dynamic_cast<Item*>(nodeA));
        }
        else if (nodeA->getTag() == PLAYER_TAG && nodeB->getTag() == MONSTER_BULLET_TAG)
        {
            contactBetweenPlayerAndBullet(dynamic_cast<Player*>(nodeA), dynamic_cast<cocos2d::Sprite*>(nodeB));
        }
        else if (nodeA->getTag() == MONSTER_BULLET_TAG && nodeB->getTag() == PLAYER_TAG)
        {
            contactBetweenPlayerAndBullet(dynamic_cast<Player*>(nodeB), dynamic_cast<cocos2d::Sprite*>(nodeA));
        }
        else if (nodeA->getTag() == MONSTER_TAG && nodeB->getTag() == PLAYER_BULLET_TAG)
        {
            contactBetweenMonsterAndBullet(dynamic_cast<Monster*>(nodeA), dynamic_cast<cocos2d::Sprite*>(nodeB));
        }
        else if (nodeA->getTag() == PLAYER_BULLET_TAG && nodeB->getTag() == MONSTER_TAG)
        {
            contactBetweenMonsterAndBullet(dynamic_cast<Monster*>(nodeB), dynamic_cast<cocos2d::Sprite*>(nodeA));
        }
    }

    return true;
}

bool HelloWorld::onContactSeparated(cocos2d::PhysicsContact& physicsContact)
{
    auto nodeA = physicsContact.getShapeA()->getBody()->getNode();
    auto nodeB = physicsContact.getShapeB()->getBody()->getNode();
    if (nodeA && nodeB)
    {
        Player* playerNode = nullptr;
        if (nodeA->getTag() == PLAYER_TAG && nodeB->getTag() == ITEM_TAG)
        {
            playerNode = dynamic_cast<Player*>(nodeA);
        }
        else if (nodeA->getTag() == ITEM_TAG && nodeB->getTag() == PLAYER_TAG)
        {
            playerNode = dynamic_cast<Player*>(nodeB);
        }

        if (playerNode != nullptr && playerNode->getInteractItem() == nodeB)
        {
            cocos2d::log("item separate");
            playerNode->setInteractItem(nullptr);
            if (dynamic_cast<Item*>(nodeB)->getItemInfo() && dynamic_cast<Item*>(nodeB)->getItemInfo()->getParent() == nodeB)
            {
                dynamic_cast<Item*>(nodeB)->getItemInfo()->setVisible(false);
            }
        }
    }
    return true;
}

void HelloWorld::contactBetweenPlayerAndItem(Player* player, Item* item)
{
    if (player != nullptr && player->getInteractItem() == nullptr && item != nullptr)
    {
        player->setInteractItem(item);
        if (item->getItemInfo() && item->getItemInfo()->getParent() == item)
        {
            item->getItemInfo()->setVisible(true);
        }
    }
}

void HelloWorld::contactBetweenPlayerAndBullet(Player* player, cocos2d::Sprite* bullet)
{
    if (player && bullet)
    {
        player->receiveDamage(10);
        if (player->isAlive() == false)
        {
            Director::getInstance()->replaceScene(TransitionSlideInT::create(0.2f, GameOver::create()));
        }
    }
}

void HelloWorld::contactBetweenMonsterAndBullet(Monster* monster, cocos2d::Sprite* bullet)
{
    if (monster && bullet)
    {
        monster->receiveDamage(10);
        if (monster->isAlive() == false)
        {
            monster->removeFromParentAndCleanup(true);
        }
    }
}