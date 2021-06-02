/**
* @file HelloWorldScene.cpp
*/

#include "HelloWorldScene.h"
#include "GameOverScene.h"
#include "./Character/Player.h"
#include "./Item/Bullet/Bullet.h"
#include "./Character/Monster.h"

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

    //auto barrier = cocos2d::Sprite::create("stone.png");
    //barrier->setScale(0.3);
    //barrier->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    //auto physicsBody = cocos2d::PhysicsBody::createBox(barrier->getContentSize(), cocos2d::PhysicsMaterial(0, 1, 0));
    //physicsBody->setDynamic(false);
    //physicsBody->setGravityEnable(false);
    //physicsBody->setRotationEnable(false);
    //physicsBody->setContactTestBitmask(1);
    //physicsBody->setCategoryBitmask(1);
    //physicsBody->setCollisionBitmask(3);
    //physicsBody->setMass(1e10);
    //barrier->setPhysicsBody(physicsBody);
    //addChild(barrier, 1);
    //barrier->setPosition(300, 500);

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
<<<<<<< Updated upstream
    else {
        //怪物在右侧随机位置出现，计算怪物生成和发射子弹的合法坐标范围
        auto minY = monster->getContentSize().height / 2;
        auto maxY = this->getContentSize().height - minY;
        auto rangeY = maxY - minY;
        int randomY = (rand() % static_cast<int>(rangeY)) + minY;
        int maxX = this->getContentSize().width;
        int randomX = maxX - (rand() % (int)(maxX / 2));
        float monsterSpeed = 160;
        float randomDuration2 = (float)randomX / monsterSpeed;
        float randomDuration1 = (float)(maxX - randomX) / monsterSpeed;
        //设置怪物生成坐标
        monster->setPosition(this->getContentSize().width + monster->getContentSize().width / 2, randomY);
        //move1：怪物从右侧移动到中间随机位置，花费时间为randomDuration1
        auto move1 = cocos2d::MoveTo::create(randomDuration1, cocos2d::Vec2(randomX, randomY));
        //move1：怪物从中间随机位置移动到左侧，花费时间为randomDuration2
        auto move2 = cocos2d::MoveTo::create(randomDuration2, cocos2d::Vec2(-monster->getContentSize().width / 2, randomY));
        //actionRemove：释放怪物对象
        auto actionRemove = RemoveSelf::create();
        //设置怪物物理躯干
        auto physicsBody = PhysicsBody::createBox(monster->getContentSize(), cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
        physicsBody->setDynamic(false);
        physicsBody->setContactTestBitmask(1);
        physicsBody->setCategoryBitmask(3);
        monster->setPhysicsBody(physicsBody);
        monster->setTag(ENEMY);
        this->addChild(monster);

        //怪物在move1和move2中间的随机位置发射子弹的动作，使用lambda表达式实现
        auto shootStar = CallFunc::create([=]() {
            //生成敌人子弹
            Sprite* enemyBullet = Sprite::create("dart_enemy.png");
            if (enemyBullet == nullptr)
            {
                problemLoading("dart_enemy.png");
            }
            else
            {
                enemyBullet->setPosition(monster->getPosition());
                //设置敌方子弹的物理躯干
                auto physicsBody = PhysicsBody::createBox(enemyBullet->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
                physicsBody->setDynamic(false);
                physicsBody->setCategoryBitmask(3);
                physicsBody->setContactTestBitmask(4);
                enemyBullet->setPhysicsBody(physicsBody);
                enemyBullet->setTag(ENEMY_BULLET);
                this->addChild(enemyBullet);
                //为敌方子弹绑定发射动画
                float starSpeed = 600;
                float maxX = this->getContentSize().width;
                float starDuration = (float)randomX / starSpeed;
                auto eDartMove = cocos2d::MoveTo::create(starDuration, player_->getPosition());
                auto eDartRemove = cocos2d::RemoveSelf::create();
                enemyBullet->runAction(cocos2d::Sequence::create(eDartMove, eDartRemove, nullptr));
            }
            });
        //怪物发射子弹时略微停顿
        auto delay = cocos2d::DelayTime::create(0.05);
        monster->runAction(Sequence::create(move1, delay, shootStar, move2, actionRemove, nullptr));
=======
    else 
    {
        addChild(monster);
        monster->move();
>>>>>>> Stashed changes
    }
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* unusedEvent)
{
<<<<<<< Updated upstream
    //取得点击屏幕位置的坐标
    auto touchLocation = touch->getLocation();
    auto offset = touchLocation - player_->getPosition();
    player_->listenToMouseEvent(offset, true);
    //取得子弹发射方向的单位向量
    offset.normalize();
=======
    ////取得点击屏幕位置的坐标
    //auto touchLocation = touch->getLocation();
    //auto offset = touchLocation - player_->getPosition();
    ////取得子弹发射方向的单位向量
    //offset.normalize();
>>>>>>> Stashed changes
    //在场景内创建子弹实例
    player_->isAttacking = true;
    TouchHolding = true;      
    return true;
}

bool HelloWorld::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unusedEvent)
{
    player_->isAttacking = false;
    TouchHolding = false;
    return true;
}

void HelloWorld::onMouseMove(cocos2d::EventMouse* mouse)
<<<<<<< Updated upstream
{
    auto mouseLocation = convertToNodeSpace(mouse->getLocationInView());
    auto offset = mouseLocation - player_->getPosition();
    player_->listenToMouseEvent(offset, false);
=======
{   
    player_->listenToMouseEvent(convertToNodeSpace(mouse->getLocationInView()), false);
>>>>>>> Stashed changes
}

bool HelloWorld::onContactBegan(cocos2d::PhysicsContact& physicsContact)
{
    auto nodeA = physicsContact.getShapeA()->getBody()->getNode();
    auto nodeB = physicsContact.getShapeB()->getBody()->getNode();

    if (nodeA && nodeB)
    {
        auto tagA = nodeA->getTag();
        auto tagB = nodeB->getTag();

        //玩家击杀怪物或怪物的子弹
        if (tagA == ME_BULLET)
        {
            nodeB->removeFromParentAndCleanup(true);
            cocos2d::log("player kill");
        }
        else if (tagB == ME_BULLET)
        {
            nodeA->removeFromParentAndCleanup(true);
            cocos2d::log("player kill");
        }

        //玩家被击杀
        if (tagA == ME)
        {
            auto tmp = dynamic_cast<Player*>(nodeA);
<<<<<<< Updated upstream
            tmp->getInjured(6);
=======
            tmp->receiveDamage(10);
>>>>>>> Stashed changes
            if (tmp->isAlive() == false)
            {
                //替换到Gameover场景
                Director::getInstance()->replaceScene(TransitionSlideInT::create(0.2f, GameOver::create()));
            }
        }
        if (tagB == ME)
        {
            auto tmp = dynamic_cast<Player*>(nodeB);
<<<<<<< Updated upstream
            tmp->getInjured(6);
=======
            tmp->receiveDamage(10);
>>>>>>> Stashed changes
            if (tmp->isAlive() == false)
            {
                //替换到Gameover场景
                Director::getInstance()->replaceScene(TransitionSlideInT::create(0.2f, GameOver::create()));
            }
        }
    }

    return true;
}