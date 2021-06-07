/**
* @file HelloWorldScene.cpp
*/

#include "HelloWorldScene.h"
#include "GameOverScene.h"
#include "./Item/Bullet/Bullet.h"
#include "Item/Medkit/Medkit.h"
#include "Const/Const.h"
#include "Obstacle/Obstacle.h"

USING_NS_CC;

cocos2d::Node* HelloWorld::generateNode_ = nullptr;

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

    Obstacle::getObstacles()->clear();

    //获取窗口大小和原点坐标
    auto winSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

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

    //auto obstacle = Obstacle::create("wall.png");
    //obstacle->setPosition(500, 300);
    //addChild(obstacle, 0);
    //auto obs2 = Obstacle::create("wall.png");
    //obs2->setPosition(300, 100);
    //addChild(obs2, 0);

    auto weapon = Weapon::create("AK47.png");
    addChild(weapon, 2);
    weapon->setPosition(500, 300);
    weapon->setScale(0.3);

    auto weapon2 = Weapon::create("MP5.png");
    addChild(weapon2, 2);
    weapon2->setPosition(700, 500);
    weapon2->setScale(0.3);

    //调用addMonster方法在随机位置生成怪物
    srand((unsigned int)time(nullptr));
    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::addMonster), 1.5);

    //生成屏幕触摸（即鼠标单击）事件的监听器
   auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
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
    player_->isAttacking = true;
    TouchHolding = true;
    return true;

    return true;
}

bool HelloWorld::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unusedEvent)
{
    player_->isAttacking = false;
    TouchHolding = false;
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
        if (nodeA->getTag() == PLAYER_TAG && nodeB->getTag() == ITEM_TAG && !dynamic_cast<Item*>(nodeB)->isHeld())
        {
            contactBetweenPlayerAndItem(dynamic_cast<Player*>(nodeA), dynamic_cast<Item*>(nodeB));
        }
        else if (nodeA->getTag() == ITEM_TAG && nodeB->getTag() == PLAYER_TAG && !dynamic_cast<Item*>(nodeA)->isHeld())
        {
            contactBetweenPlayerAndItem(dynamic_cast<Player*>(nodeB), dynamic_cast<Item*>(nodeA));
        }
        else if ((nodeA->getTag() == PLAYER_TAG || nodeA->getTag() == MONSTER_TAG)
            && (nodeB->getTag() == PLAYER_BULLET_TAG || nodeB->getTag() == MONSTER_BULLET_TAG))
        {
            contactBetweenCharacterAndBullet(dynamic_cast<Character*>(nodeA), dynamic_cast<Bullet*>(nodeB));
        }
        else if ((nodeA->getTag() == PLAYER_BULLET_TAG || nodeA->getTag() == MONSTER_BULLET_TAG)
            && (nodeB->getTag() == PLAYER_TAG || nodeB->getTag() == MONSTER_TAG))
        {
            contactBetweenCharacterAndBullet(dynamic_cast<Character*>(nodeB), dynamic_cast<Bullet*>(nodeA));
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

void HelloWorld::contactBetweenCharacterAndBullet(Character* character, Bullet* bullet)
{
    if (character && bullet)
    {
        character->receiveDamage(bullet->getBulletAtk());
        bullet->removeFromParentAndCleanup(true);
    }
}

void HelloWorld::generateNode(float dt)
{
    if (generateNode_ != nullptr)
    {
        auto node = generateNode_;
        auto dropTo = generateNode_->getPosition();
        generateNode_->setPosition(cocos2d::Point(dropTo.x, dropTo.y + 30));
        auto dropAction = cocos2d::MoveTo::create(0.2f, dropTo);
        generateNode_->runAction(dropAction);
        addChild(generateNode_, 2);
    }
}

cocos2d::Node*& HelloWorld::getGenerateNode()
{
    return generateNode_;
}